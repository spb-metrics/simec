<?php

function posAcaoVldsituacaoFALSE($iclid) {
	global $db;
	
	$conf = array(ENEM_EST_EM_VALIDACAO 	=> array('tpvid' => '2'),
				  ENEM_EST_EM_EXECUCAO  	=> array('tpvid' => '1'),
				  ENEM_EST_EM_CERTIFICACAO  => array('tpvid' => '3'));
				  
	$entidUsuario = $db->pegaUm("SELECT entid FROM projetos.usuarioresponsabilidade WHERE rpustatus = 'A' AND usucpf = '".$_SESSION['usucpf']."' AND entid is not null");
	if( !$entidUsuario ) {
		$entidUsuario = $db->pegaUm("SELECT entid FROM entidade.entidade WHERE entnumcpfcnpj = '".$_SESSION['usucpf']."' AND entstatus = 'A'");
	}
				  
	$esdid = $db->pegaUm("SELECT d.esdid FROM projetos.itemchecklist i 
						     INNER JOIN workflow.documento d ON i.docid = d.docid 
						     WHERE iclid='".$iclid."'");
	
	$sql = "SELECT vldid FROM projetos.validacao 
			WHERE tpvid='".$conf[$esdid]['tpvid']."' AND iclid='".$iclid."'";
	
	$vldid = $db->pegaUm($sql);
	
	if($vldid) {
		$db->executar("UPDATE projetos.validacao SET vldsituacao=NULL,vldobservacao=NULL WHERE vldid = ".$vldid."");
		$db->executar("UPDATE projetos.anexochecklist SET ancstatus = 'I' WHERE vldid = ".$vldid." AND ancstatus = 'A'");
		$db->commit();
	}
	
	return true;

}



// ATIVIDADE ///////////////////////////////////////////////////////////////////


function atividade_inserir( $atividade, $titulo, $tipo=null ){
	global $db;
	$sql = sprintf(
		"insert into projetos.atividade (
			atiidpai, atidescricao, atiordem, _atiprojeto, acaid, atitipoenem
		) values (
			%d,
			'%s',
			( select coalesce( max(atiordem), 0 ) + 1 from projetos.atividade where atistatus = 'A' and atiidpai = %d ),
			( select _atiprojeto from projetos.atividade where atiid = %d ),
			( select acaid from projetos.atividade where atiid = %d ),
			'%s'
		) returning atiid",
		$atividade,
		$titulo,
		$atividade,
		$atividade,
		$atividade, # adaptação necessária para que o módulo de monitoramento funcione
		$tipo
	);
	
	$atiid = $db->pegaUm($sql);
	
		// verifica se a atividade pai possui subacao, caso sim, a filha devera ser a mesma subacao
	$sql = "SELECT sbaid FROM monitora.pi_subacaoatividade WHERE atiid='".$atividade."'";
	$sbaids = $db->carregar($sql);
	
	if($sbaids[0]) {
		foreach($sbaids as $sbaid) {
			$sql = "INSERT INTO monitora.pi_subacaoatividade(sbaid, atiid) VALUES ('".$sbaid['sbaid']."', '".$atiid."');";
			$db->executar($sql);
		}
	}
	
	$db->commit();	
	return true;
}

function atividade_listar( $atividade, $profundidade = 0, $situacao = array(), $usuario = null, $perfil = array() ){
	global $db;
		
	// captura as opções
	$atividade    = (integer) $atividade;
	$profundidade = (integer) $profundidade;
	$situacao     = (array) $situacao;
	$usuario      = (string) $usuario;
	
	// identifica a atividade e o projeto
	$atividade = $atividade ? $atividade : PROJETO;
	$projeto   = (integer) $db->pegaUm( "select _atiprojeto from projetos.atividade where atiid = $atividade" );
	if ( $projeto != PROJETO && $_SESSION['sisid'] != 10) {
		$atividade = (integer) PROJETO;
		$projeto   = (integer) PROJETO;
	}
	//dbg($projeto);
	// identifica o nó de origem
	$sql_filhas = "";
	if ( $atividade ) {
		$numero = $db->pegaUm( "select _atinumero from projetos.atividade where atiid = $atividade" );
		if ( $numero ) {
			//$sql_filhas = " and ( a._atinumero like '$numero.%' ) ";
			$sql_filhas = " and ( substr( a._atinumero, 0, " . ( strlen( $numero ) + 2 ) . " ) = '" . $numero .  ".' ) ";
		}
	}
	
	// restringe a profundidade
	$sql_profundidade = "";
	if ( $profundidade > 0 ) {
		$sql_profundidade = " and ( a._atiprofundidade <= $profundidade ) ";
	}
	
	// restringe as situações
	$sql_situacao = "";
	if ( count( $situacao ) > 0 ) {
		$sql_situacao = " and a.esaid in (". implode( ',', $situacao ) .") ";
		$sql_situacao_restricao = " and a.esaid not in (". implode( ',', $situacao ) .") ";
	}
	
	// restringe por responsabilidade
	$sql_responsabilidade = "";
	if ( $usuario ) {
		$sql_perfil = "";
		if ( !empty( $perfil ) ) {
			$sql_perfil = " and ur.pflcod in ( ". implode( ",", $perfil ) ." ) ";
		}
		$sql = sprintf(
			"select a._atinumero
			from seguranca.usuario u
			inner join projetos.usuarioresponsabilidade ur on ur.usucpf = u.usucpf %s
			inner join seguranca.perfilusuario pu on pu.pflcod = ur.pflcod and pu.usucpf = ur.usucpf
			inner join projetos.atividade a on a.atiid = ur.atiid
			where
				u.suscod = 'A'
				and u.usucpf = '%s'
				and ur.rpustatus = 'A'
				and a._atiprojeto = %d
				and a.atiid != a._atiprojeto
				and a.atistatus = 'A'
				%s %s %s",
			$sql_perfil,
			$usuario,
			$projeto,
			$sql_filhas,
			$sql_profundidade,
			$sql_situacao
		);
		$numeros = array();
		foreach( (array) $db->carregar( $sql ) as $responsabilidade ) {
			$rastro = array();
			foreach( explode( ".", $responsabilidade['_atinumero'] ) as $item ){
				array_push( $rastro, sprintf( "%04d", $item ) );
			}
			$numero = implode( $rastro );
			//array_push( $numeros, " a._atiordem like '" . $numero ."%' " );
			array_push( $numeros, " substr( a._atinumero, 0, " . ( strlen( $numero ) + 1 ) . " ) = '" . $numero . "' " );
			foreach ( $rastro as $chave => $ordem ) {
				if ( $chave == 0 ) continue;
				$numero = implode( "", array_slice( $rastro, 0, $chave ) );
				array_push( $numeros, " a._atiordem = '" . $numero ."' " );
			}
		}
		$numeros = array_unique( $numeros );
		$sql_responsabilidade = " and ( ". implode( ' or ', $numeros ) ." ) ";
	}
	
	$sql_restricao = "";
	if ( $sql_situacao_restricao ) {
		$sql = sprintf(
			"select a._atinumero
			from projetos.atividade a
			where
				a._atiprojeto = %d
				and a.atiid != a._atiprojeto
				and a.atistatus = 'A'
				%s %s %s",
			$projeto,
			$sql_filhas,
			$sql_profundidade,
			$sql_situacao_restricao
		);
		$restricao = array();
		$atinumeros = array();
		foreach( (array) $db->carregar( $sql ) as $atividade ) {
			if ( !$atividade['_atinumero'] ) {
				break;
			}
			array_push( $atinumeros, $atividade['_atinumero'] );
		}
		$numerosFinais = array();
		foreach ( array_unique( $atinumeros ) as $atinumero ) {
			$tamanho = strlen($atinumero );
			if ( !array_key_exists( $tamanho, $numerosFinais ) )
			{
				$numerosFinais[$tamanho] = array();
			}
			array_push( $numerosFinais[$tamanho], $atinumero . "." );
		}
		foreach ( $numerosFinais as $tamanho => $valores )
		{
			array_push( $restricao, " substr( a._atinumero, 0, " . ( $tamanho + 2 ) . " ) not in ( '" . implode( "','", $valores ) . "' ) " );
		}
		if ( count( $restricao ) > 0 ) {
			$sql_restricao = " and ( ". implode( ' and ', $restricao ) . " ) ";
		}
	}
	
	$sql = sprintf(
		"select
			a.atiid,
			a.aticodigo,
			a.atidescricao,
			--a.atidetalhamento,
			--a.atimeta,
			--a.atiinterface,
			a.atidatainicio,
			a.atidatafim,
			--a.atisndatafixa,
			a.atistatus,
			a.atiordem,
			--a.atinumeracao,
			--a.atiidpredecessora,
			a.atiidpai,
			--a.usucpf,
			--a.tatcod,
			a.esaid,
			a.atidataconclusao,
			
			CASE 
			WHEN (a.atitipoandamento = 'p' OR a.atitipoandamento IS NULL) THEN a.atiporcentoexec
			WHEN (a.atitipoandamento = 'q') THEN ( ( coalesce(a.atiquantidadeexec, 0) / a.atimetanumerica ) * 100 )
			END as atiporcentoexec,
						
			a.atitipoandamento,
			a.atiquantidadeexec,
			a.atimetanumerica,
			
			a._atiprojeto,
			--a._atiordem,
			a._atinumero,
			a._atiprofundidade,
			a._atiirmaos,
			a._atifilhos,
			ea.esadescricao,
			u.usunome,
			u.usunomeguerra,
			--u.usucpf,
			u.usuemail,
			u.usufoneddd,
			u.usufonenum,
			uni.unidsc,
			ug.ungdsc,
			aga.graid,
			coalesce( restricoes, 0 ) as qtdrestricoes,
			coalesce( anexos, 0 ) as qtdanexos,
			a._atiprofundidade as profundidade,
			a._atinumero as numero,
			a._atifilhos as filhos,
			a.atitipoandamento, 
			a.atimetanumerica,
			a.atiquantidadeexec,
			a.atitipoenem
		from projetos.atividade a
		inner join projetos.estadoatividade ea on
			ea.esaid = a.esaid
		left join projetos.usuarioresponsabilidade ur on
			ur.atiid = a.atiid and
			ur.rpustatus = 'A' and
			ur.pflcod = %d
		left join projetos.atividadegrupoatividade aga on
			aga.atiid = a.atiid and aga.graid=1
		left join seguranca.perfilusuario pu on
			pu.pflcod = ur.pflcod and
			pu.usucpf = ur.usucpf
		left join seguranca.usuario u on
			u.usucpf = pu.usucpf and
			u.suscod = 'A'
		left join public.unidade uni on
			uni.unicod = u.unicod and
			uni.unitpocod = 'U' and
			uni.unistatus = 'A'
		left join public.unidadegestora ug on
			ug.ungcod = u.ungcod and
			ug.ungstatus = 'A'
		left join (
			select atiid, count(*) as restricoes
			from projetos.observacaoatividade
			where obsstatus = 'A' and obssolucao = false
			group by atiid ) restricao on
				restricao.atiid = a.atiid
		left join (
			select atiid, count(*) as anexos
			from projetos.anexoatividade
			where anestatus = 'A'
			group by atiid ) anexo on
				anexo.atiid = a.atiid
		where
			a._atiprojeto = %d
			and a.atiid != a._atiprojeto
			and a.atistatus = 'A'
			%s %s %s %s %s 
		order by _atiordem",
		PERFIL_GERENTE,
		$projeto,
		$sql_filhas,
		$sql_profundidade,
		$sql_situacao,
		$sql_restricao,
		$sql_responsabilidade
	);
	//dbg( $sql, 1 );
	return $db->carregar( $sql );
}

function atividade_excluir( $atiid ){
	global $db;
	// captura as informações da atividade a ser excluída
	$sql = sprintf( "select * from projetos.atividade a where a.atiid = %s and a.atistatus = 'A'", $atiid );
	$atividade = $db->pegaLinha( $sql );
	if ( !$atividade ) {
		return false;
	}
	// exclui a atividade
	$sql = sprintf( "update projetos.atividade set atistatus = 'I' where atiid = %s", $atividade['atiid'] );
	if ( !$db->executar( $sql ) ) {
		return false;
	}
	// reordena as atividades que tem o mesmo pai
	$sql = sprintf(
		"update projetos.atividade set atiordem = atiordem - 1 where atiidpai = %s and atiordem > %s and atistatus = 'A'",
		$atividade['atiidpai'],
		$atividade['atiordem']
	);
	if ( !$db->executar( $sql ) ) {
		return false;
	}
	return true;
}

function atividade_pegar( $atividade ){
	global $db;
	$sql = sprintf(
		"select a.*, e.esadescricao, sub.numero, sub.projeto, oa.obsmelhorias, oa.obsinfomelhorias
		from projetos.atividade a
		left join projetos.estadoatividade e on e.esaid = a.esaid
		left join projetos.observacaoatividade oa on a.atiid = oa.atiid and oa.obsstatus = 'A'
		inner join projetos.f_dadosatividade( %d ) as sub on sub.atiid = a.atiid
		where a.atiid = %d and atistatus = 'A'",
		(integer) $atividade,
		(integer) $atividade
	);
	$registro = $db->pegaLinha( $sql );
	if ( is_array( $registro ) ) {
		return $registro;
	}
	return null;
}

function atividade_pegar_projeto( $atividade ){
	global $db;
	$sql = sprintf( "select _atiprojeto from projetos.atividade where atiid = %d", $atividade );
	return $db->pegaUm( $sql );
}

function recuperar_tipo_enem( $atividade )
{
	global $db;
	$sql = sprintf( "select atitipoenem from projetos.atividade where atiid = %d", $atividade );
	return $db->pegaUm( $sql );
}

/**
 * Verifica se o andamento da atividade está corretamente cadastrado.
 * @param $atiid integer
 * @return boolean
 */
function verificaCadastroAndamentoAtividade($atiid)
{
	/*** Instância global do objeto de conexão do banco ***/
	global $db;
	/*** Variável de retorno ***/
	$retorno = false;
	
	/*** Recupera o tipo de andamento ***/
	$sql 				= "SELECT atitipoandamento FROM projetos.atividade WHERE atiid = " . $atiid;
	$atitipoandamento 	= $db->pegaUm($sql);
	
	/*** Se o tipo de andamento estiver corretamento cadastrado ***/
	if( $atitipoandamento )
	{
		/*** Se for percentual ***/
		if( $atitipoandamento == 'p' )
		{
			/*** Recupera o andamento ***/
			$sql 		= "SELECT atiporcentoexec FROM projetos.atividade WHERE atiid = " . $atiid;
			$andamento	= $db->pegaUm($sql);
		}
		/*** Se for quantitativo ***/
		if( $atitipoandamento == 'q' )
		{
			/*** Recupera o andamento ***/
			$sql 		= "SELECT atiquantidadeexec FROM projetos.atividade WHERE atiid = " . $atiid;
			$andamento	= $db->pegaUm($sql);
		}
		
		/*** Se o andamento estiver corretamente cadastrado, retorna true ***/
		$retorno = ( $andamento ) ? true : false;
	}
	
	return $retorno;
}

/**
 * Retorna as atividades que estão acima da atividade indicada exceto o projeto,
 * que é a atividade raiz. 
 * 
 * @return array
 */
function atividade_pegar_rastro( $numero ){
	global $db;
	$numero_original = $numero;
	$condicao = array();
	array_push( $condicao, " a._atinumero = '$numero' " );
	while( ( $posicao = strrpos( $numero, '.' ) ) !== false ) {
		$numero = substr( $numero, 0, $posicao );
		array_push( $condicao, " a._atinumero = '$numero' " );
	}
	if ( count( $condicao ) == 0 ) {
		return array();
	}
	$sql = sprintf(
		"select
			a._atinumero as numero,
			a._atiprofundidade as profundidade,
			a._atiirmaos as irmaos,
			a._atifilhos as filhos,
			a.atidescricao,
			a.atiid,
			a.atiidpai,
			a.atidatainicio,
			a.atidatafim,
			a.atiordem,
			a.atiporcentoexec,
			a.esaid,
			ea.esadescricao,
			u.usunome,
			u.usunomeguerra,
			u.usucpf,
			uni.unidsc,
			ug.ungdsc,
			a.atitipoenem
		from projetos.atividade a
			left join projetos.estadoatividade ea on
				ea.esaid = a.esaid
			left join projetos.usuarioresponsabilidade ur on
				ur.atiid = a.atiid and ur.rpustatus = 'A' and ur.pflcod = %d 
			left join seguranca.usuario u on
				u.usucpf = ur.usucpf and u.usustatus = 'A'
			left join public.unidade uni on
				uni.unicod = u.unicod and
				uni.unitpocod = 'U' and
				uni.unistatus = 'A'
			left join public.unidadegestora ug on
				ug.ungcod = u.ungcod and
				ug.ungstatus = 'A'
		where
			a._atiprojeto = %d and
			a.atiidpai is not null and
			a.atistatus = 'A' and
			( %s )
		order by a._atiordem",
		PERFIL_GERENTE,
		PROJETO,
		implode( ' or ', $condicao )
	);
	$rastro = $db->carregar( $sql );
	return $rastro && count( $rastro ) == substr_count( $numero_original, "." ) + 1 ? $rastro : array();
}

function atividade_pegar_filhas( $projeto, $atividade = null, $usuario = null, $profundidade = null ){
	global $db;
	$profundidade = (string) $profundidade;
	if ( $profundidade != '' ) {
		$profundidade = (integer) $profundidade;
		if ( $atividade ) {
			$sql = "select profundidade from projetos.f_dadosatividade( " . $atividade . " )";
			$profundidade = $db->pegaUm( $sql ) + $profundidade;
		} else {
			$profundidade++;
		}
		$condicao_profundidade = " and la.profundidade <= " . $profundidade;
	} else {
		$condicao_profundidade = "";
		$profundidade = null;
	}
	if ( $usuario ) {
		return atividade_pegar_sob_responsabilidade( $projeto, $usuario, $profundidade );
	}
	if ( $atividade ) {
		$sql = sprintf(
			"select
				la.numero,
				la.profundidade,
				la.irmaos,
				la.filhos,
				a.atidescricao,
				a.atiid,
				a.atiidpai,
				a.atidatainicio,
				a.atidatafim,
				a.atidataconclusao,
				a.atiordem,
				a.atiporcentoexec,
				a.esaid,
				ea.esadescricao,
				u.usunome,
				u.usunomeguerra,
				u.usucpf,
				u.usuemail,
				u.usufoneddd,
				u.usufonenum,
				uni.unidsc,
				ug.ungdsc,
				coalesce(qtdrestricoes,0) as qtdrestricoes,
				coalesce(qtdanexos,0) as qtdanexos
			from projetos.f_dadosatividade( %d ) as da
				inner join projetos.f_dadostodasatividades() as la on
					la.numero like da.numero || '.%%' or
					la.numero = da.numero 
				inner join projetos.atividade a on
					a.atiid = la.atiid
				left join projetos.estadoatividade ea on
					ea.esaid = a.esaid
				left join projetos.usuarioresponsabilidade ur on
					ur.atiid = la.atiid and
					ur.rpustatus = 'A' and
					ur.pflcod = %d
				left join seguranca.perfilusuario pu on
					pu.pflcod = ur.pflcod and
					pu.usucpf = ur.usucpf
				left join seguranca.usuario u on
					u.usucpf = pu.usucpf and
					u.suscod = 'A'
				left join public.unidade uni on
					uni.unicod = u.unicod and
					uni.unitpocod = 'U' and
					uni.unistatus = 'A'
				left join public.unidadegestora ug on
					ug.ungcod = u.ungcod and
					ug.ungstatus = 'A'
				left join (
					select atiid, count(*) as qtdrestricoes
					from projetos.observacaoatividade
					where obsstatus = 'A' and obssolucao = false
					group by atiid ) restricao on restricao.atiid = a.atiid
				left join (
					select atiid, count(*) as qtdanexos
					from projetos.anexoatividade
					where anestatus = 'A'
					group by atiid ) anexo on anexo.atiid = a.atiid
			where
				la.projeto = %d and
				la.projeto != la.atiid and
				a.atistatus = 'A'
				%s
			order
				by la.ordem",
			$atividade,
			PERFIL_GERENTE,
			$projeto,
			$condicao_profundidade
		);
	} else {
		$sql = sprintf(
			"
			select
				la.numero,
				la.profundidade,
				la.irmaos,
				la.filhos,
				a.atidescricao,
				a.atiid,
				a.atiidpai,
				a.atidatainicio,
				a.atidatafim,
				a.atidataconclusao,
				a.atiordem,
				a.atiporcentoexec,
				a.esaid,
				ea.esadescricao,
				u.usunome,
				u.usunomeguerra,
				u.usucpf,
				u.usuemail,
				u.usufoneddd,
				u.usufonenum,
				uni.unidsc,
				ug.ungdsc,
				coalesce(qtdrestricoes,0) as qtdrestricoes,
				coalesce(qtdanexos,0) as qtdanexos
			from projetos.f_dadostodasatividades() la
				inner join projetos.atividade a on
					a.atiid = la.atiid
				left join projetos.estadoatividade ea on
					ea.esaid = a.esaid
				left join projetos.usuarioresponsabilidade ur on
					ur.atiid = la.atiid and
					ur.rpustatus = 'A' and
					ur.pflcod = %d
				left join seguranca.perfilusuario pu on
					pu.pflcod = ur.pflcod and
					pu.usucpf = ur.usucpf
				left join seguranca.usuario u on
					u.usucpf = pu.usucpf and
					u.suscod = 'A'
				left join public.unidade uni on
					uni.unicod = u.unicod and
					uni.unitpocod = 'U' and
					uni.unistatus = 'A'
				left join public.unidadegestora ug on
					ug.ungcod = u.ungcod and
					ug.ungstatus = 'A'
				left join (
					select atiid, count(*) as qtdrestricoes
					from projetos.observacaoatividade
					where obsstatus = 'A' and obssolucao = false
					group by atiid ) restricao on restricao.atiid = a.atiid
				left join (
					select atiid, count(*) as qtdanexos
					from projetos.anexoatividade
					where anestatus = 'A'
					group by atiid ) anexo on anexo.atiid = a.atiid
			where
				la.projeto = %d and
				la.projeto != la.atiid and
				a.atistatus = 'A'
				%s
			order by
				la.ordem",
			PERFIL_GERENTE,
			$projeto,
			$condicao_profundidade
		);
	}
	$lista = $db->carregar( $sql );
	if ( is_array( $lista ) ) {
		return $lista;
	}
	return array();
}

function atividade_pegar_sob_responsabilidade( $projeto, $usuario, $profundidade = null ){
	global $db;
	if ( $profundidade !== null ) {
		$condicao_profundidade = " and folha.profundidade <= " . $profundidade;
	} else {
		$condicao_profundidade = "";
	}
	$sql = sprintf(
		"select
			folha.numero,
			folha.profundidade,
			folha.irmaos,
			folha.filhos,
			a.atidescricao,
			a.atiid,
			a.atiidpai,
			a.atidatainicio,
			a.atidatafim,
			a.atidataconclusao,
			a.atiordem,
			a.atiporcentoexec,
			a.esaid,
			ea.esadescricao,
			u.usunome,
			u.usunomeguerra,
			u.usucpf,
			u.usuemail,
			u.usufoneddd,
			u.usufonenum,
			uni.unidsc,
			ug.ungdsc,
			coalesce(qtdrestricoes,0) as qtdrestricoes,
			coalesce(qtdanexos,0) as qtdanexos
		from projetos.usuarioresponsabilidade ur
			inner join projetos.f_dadostodasatividades() as raiz on
				raiz.atiid = ur.atiid
			inner join seguranca.perfilusuario pu on pu.pflcod = ur.pflcod and pu.usucpf = ur.usucpf
			inner join projetos.f_dadostodasatividades() as folha on
				folha.atiid = raiz.atiid or folha.numero like raiz.numero || '.%%'
			inner join projetos.atividade a on
				a.atiid = folha.atiid
			left join projetos.estadoatividade ea on
				ea.esaid = a.esaid
			left join projetos.usuarioresponsabilidade ur2 on
				ur2.atiid = folha.atiid and ur2.rpustatus = 'A' and ur2.pflcod = %d 
			left join seguranca.perfilusuario pu2 on pu2.pflcod = ur2.pflcod and pu2.usucpf = ur2.usucpf
				left join seguranca.usuario u on
					u.usucpf = pu2.usucpf and
					u.suscod = 'A'
			left join public.unidade uni on
				uni.unicod = u.unicod and
				uni.unitpocod = 'U' and
				uni.unistatus = 'A'
			left join public.unidadegestora ug on
				ug.ungcod = u.ungcod and
				ug.ungstatus = 'A'
			left join (
				select atiid, count(*) as qtdrestricoes
				from projetos.observacaoatividade
				where obsstatus = 'A' and obssolucao = false
				group by atiid ) restricao on restricao.atiid = a.atiid
			left join (
				select atiid, count(*) as qtdanexos
				from projetos.anexoatividade
				where anestatus = 'A'
				group by atiid ) anexo on anexo.atiid = a.atiid
		where
			ur.rpustatus = 'A' and
			ur.usucpf = '%s' and
			folha.projeto = %d and
			folha.projeto != folha.atiid and
			raiz.projeto = %d
			%s
		order by folha.ordem",
		PERFIL_GERENTE,
		$usuario,
		$projeto,
		$projeto,
		$condicao_profundidade
	);

	$lista = $db->carregar( $sql );
	if ( !is_array( $lista ) ) {
		return array();
	}
	$lista_final = array();
	foreach ( $lista as $item ) {
		if ( array_key_exists( $item['numero'], $lista_final ) ) {
			continue;
		}
		// adiciona pais (caso o pai não esteja na lista)
		$numero_pai = substr( $item['numero'], 0, strrpos( $item['numero'], '.' ) );
		if ( $numero_pai && !array_key_exists( $numero_pai, $lista_final ) ) {
			$rastro_pai = atividade_pegar_rastro( $item['numero'] );
			foreach ( $rastro_pai as $item_pai ) {
				if ( !array_key_exists( $item_pai['numero'], $lista_final ) ) {
					$lista_final[$item_pai['numero']] = $item_pai;
				}
			}
		}
		// adiciona item à lista
		$lista_final[$item['numero']] = $item;
	}
	return array_values( $lista_final );
}

function atividade_calcular_dados( $atividade ){
	global $db;
	// pega dados da atividade
	$atividade = (integer) $atividade;
	$sql = "select _atiordem, _atinumero, _atiprofundidade, _atiprojeto from projetos.atividade where atiid = " . $atividade;
	$pai = $db->recuperar( $sql );
	
	// pega filhos
	$sql = "select atiid, atiordem from projetos.atividade where atiidpai = " . $atividade . " and atistatus = 'A'";
	$filhos = $db->carregar( $sql );
	$filhos = $filhos ? $filhos : array();
	$sql = "update projetos.atividade set _atifilhos = " . count( $filhos ) . " where atiid = " . $atividade;
	$db->executar( $sql, false );
	
	// atualiza filhos
	foreach ( $filhos as $filho ){
		$_atinumero  = ( $pai['_atinumero'] ? $pai['_atinumero'] . "." : '' ) . $filho['atiordem'];
		$_atiordem   = ( $pai['_atiordem'] ? $pai['_atiordem'] : '' ) . sprintf( '%04d', $filho['atiordem'] );
		$_atiprojeto = (integer) $pai['_atiprojeto']; 
		$sql = "
			update projetos.atividade
			set
				_atinumero = '" . $_atinumero . "',
				_atiordem = '" . $_atiordem . "',
				_atiprofundidade = " . ( $pai['_atiprofundidade'] + 1 ) . ",
				_atiirmaos = " . count( $filhos ) . ",
				_atiprojeto = " . $_atiprojeto . "
			where atiid = " . $filho['atiid'];
		$db->executar( $sql, false );
		atividade_calcular_dados( $filho['atiid'] );
	}
}

function atividade_calcular_possibilidade_mudar_data( $intIdAtividade , $strNovaDataInicio = null, $strNovaDataFim = null, $strNovaDataConclusao = null ){
	global $db;
	
	$sql = sprintf(
		"SELECT atidatainicio , atidatafim, atidataconclusao, esaid from projetos.atividade where atiid = %d",
		$intIdAtividade
	);
	
	$arrAtiDatas = $db->recuperar( $sql );

	if( $strNovaDataInicio !== null )
	{
		$arrAtiDatas[ 'atidatainicio'  ] = formata_data_sql( $strNovaDataInicio );
	}
	if( $strNovaDataFim !== null )
	{
		$arrAtiDatas[ 'atidatafim'  ] = formata_data_sql( $strNovaDataFim );
	}
	if( $strNovaDataConclusao !== null )
	{
		$arrAtiDatas[ 'atidataconclusao'  ] = formata_data_sql( $strNovaDataConclusao );
	}
	
	$intDataInicio	= strtotime( $arrAtiDatas[ 'atidatainicio' ] );
	
	if( (integer) $arrAtiDatas['esaid'] == (integer) STATUS_CONCLUIDO )
	{
		if( $arrAtiDatas[ 'atidataconclusao' ] != null )
		{
			$intDataTermino = strtotime(  $arrAtiDatas[ 'atidataconclusao' ] );
		}
		else
		{
			$intDataTermino = null;
		}
	}
	else
	{
		if(  $arrAtiDatas[ 'atidatafim' ] != null )
		{
			$intDataTermino = strtotime( $arrAtiDatas[ 'atidatafim' ] );
		}
		else
		{
			$intDataTermino = null; 
		}
	}
	if	(
			( $intDataInicio !== null )
			&&
			( $intDataTermino !== null )
			&&
			( $intDataInicio > $intDataTermino )
		)
	{
		return false;
	}
	return true;
}


// RESPONSABILIDADE ////////////////////////////////////////////////////////////


/**
 * Atribui responsabilidade aos usuários na atividade indicada segundo o perfil
 * especificado.
 * 
 * @return boolean
 */
function atividade_atribuir_responsavel( $atividade, $perfil, $usuarios ){
	global $db;
	$sql = sprintf(
		"update projetos.usuarioresponsabilidade
		set rpustatus = 'I'
		where pflcod = %d and atiid  = %d",
		$perfil,
		$atividade
	);
	if ( !$db->executar( $sql ) ) {
		$db->rollback();
		return false;
	}
	foreach ( $usuarios as $usuario ) {
	
		if ( empty( $usuario ) ) {
			continue;
		}
		$sql = "select count(*) from seguranca.perfilusuario where pflcod = $perfil and usucpf = '$usuario'";
		$possui_perfil = $db->pegaUm( $sql );
		if ( !$possui_perfil )
		{
			$sql = "insert into seguranca.perfilusuario ( pflcod, usucpf ) values ( $perfil, '$usuario' )";
			$db->executar( $sql );
		}
		$sql = sprintf(
			"select count(*) from projetos.usuarioresponsabilidade
			where usucpf = '%s' and pflcod = %d and atiid = %d",
			$usuario,
			$perfil,
			$atividade
		);
		if( (boolean) $db->pegaUm( $sql ) ) {
			$sql = sprintf(
				"update projetos.usuarioresponsabilidade
				set rpustatus = 'A'
				where usucpf = '%s' and pflcod = %d and atiid = %d",
				$usuario,
				$perfil,
				$atividade
			);
		} else {
			$sql = sprintf(
				"insert into projetos.usuarioresponsabilidade (
					usucpf, pflcod, atiid, rpustatus
				) values (
					'%s', %d, %d, 'A'
				)",
				$usuario,
				$perfil,
				$atividade
			);
		}
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			return false;
		}
		$db->alterar_status_usuario( $usuario, 'A', 'Atribuição de responsabilidade em atividade ou projeto.', $_SESSION['sisid'] );
		$usuariodados = $db->pegaLinha("SELECT * FROM seguranca.usuario WHERE usucpf='".$usuario."'");
		if($usuariodados['usuchaveativacao'] == "f") {
			$remetente = array("nome" => "Simec","email" => "simec@mec.gov.br");
			$destinatario = $usuariodados['usuemail'];
			$assunto = "Aprovação do Cadastro no Simec";
			$conteudo = "
						<br/>
						<span style='background-color: red;'><b>Esta é uma mensagem gerada automaticamente pelo sistema. </b></span>
						<br/>
						<span style='background-color: red;'><b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b></span>
						<br/>
						";
					$conteudo .= sprintf(
					"%s %s<p>Sua conta está ativa. Sua Senha de acesso é: %s</p>",
					$usuariodados['ususexo'] == 'M' ? 'Prezado Sr.' : 'Prezada Sra.',
					$usuariodados['usunome'],
					md5_decrypt_senha( $usuariodados['ususenha'], '' )
					);
					enviar_email( $remetente, $destinatario, $assunto, $conteudo );
		}
	
		
	}
	return true;
}

/**
 * @return boolean
 */
function atividade_verificar_responsabilidade( $atividade, $usuario = null ){
	global $db;
	static $permissoes = array(); # responsabilidades atribuídas
	if ( $db->testa_superuser() ) {
		return true;
	}
	$usuario = $usuario ? $usuario : $_SESSION['usucpf'];
	if ( $_SESSION["sisid"] == 1 ) {
		return acao_verificar_responsabilidade( $atividade, $usuario );
	}
	if ( !array_key_exists( $usuario, $permissoes ) ) {
		/*
		$sql = sprintf(
			"select folha.atiid
			from projetos.usuarioresponsabilidade ur
				inner join projetos.f_dadostodasatividades() as raiz on
					raiz.atiid = ur.atiid
				inner join seguranca.perfilusuario pu on pu.pflcod = ur.pflcod and pu.usucpf = ur.usucpf
				inner join projetos.f_dadostodasatividades() as folha on
					folha.atiid = raiz.atiid or folha.numero like raiz.numero || '.%%'
			where ur.rpustatus = 'A' and ur.usucpf = '%s' and ur.pflcod in ( %d, %d )
			group by folha.atiid",
			$usuario,
			PERFIL_GERENTE,
			PERFIL_EQUIPE_APOIO_GERENTE
		);
		*/
		
		
		$sql = sprintf(
			"select folha.atiid 
			from projetos.usuarioresponsabilidade ur 
				inner join projetos.atividade as raiz on 
					raiz.atiid = ur.atiid 
				inner join seguranca.perfilusuario pu on pu.pflcod = ur.pflcod and pu.usucpf = ur.usucpf 
				inner join projetos.atividade as folha on 
					folha.atiid = raiz.atiid or folha._atinumero like raiz._atinumero || '.%%' 
			where raiz.atistatus = 'A' and folha.atistatus = 'A' and ur.rpustatus = 'A' and ur.usucpf = '%s' and ur.pflcod in ( %d, %d ) 
			group by folha.atiid",
			$usuario,
			PERFIL_GERENTE,
			PERFIL_EQUIPE_APOIO_GERENTE
		);
		
		$lista = $db->carregar( $sql );
		$permissoes[$usuario] = array();
		if ( is_array( $lista ) ) {
			foreach ( $lista as $item ) {
				array_push( $permissoes[$usuario], $item['atiid'] );
			}
		}
	}
	
	if ( !in_array( $atividade, $permissoes[$usuario] ) ) {
		$projeto = $db->pegaUm( "select _atiprojeto from projetos.atividade where atiid = " . $atividade );
		if ( !$projeto ) {
			$projeto = $atividade;
		}
		
		return projeto_verificar_responsabilidade( $projeto );
	}
	return true;
}

/**
 * @return boolean
 */
function atividade_verificar_perfil( $atividade, $perfil, $usuario = null ){
	global $db;
	static $permissoes = array(); # responsabilidades atribuídas
	
	$usuario = $usuario ? $usuario : $_SESSION['usucpf'];
	
	// CASO ESPECÍFICO PARA GERENTE DE PROJETO
	if ( $perfil == PERFIL_GERENTE )
	{
		$sql = "select usucpf from projetos.atividade where atiid = " . $atividade;
		return $db->pegaUm( $sql ) == $usuario;
	}
	
	if ( $db->testa_superuser() ) {
		return true;
	}
	if ( !array_key_exists( $perfil, $permissoes ) ) {
		$sql = sprintf(
			"select folha.atiid
			from projetos.usuarioresponsabilidade ur
				inner join projetos.f_dadostodasatividades() as raiz on
					raiz.atiid = ur.atiid
				inner join projetos.f_dadostodasatividades() as folha on
					folha.atiid = raiz.atiid or folha.numero like raiz.numero || '.%%'
			where ur.rpustatus = 'A' and ur.usucpf = '%s' and ur.pflcod = %d
			group by folha.atiid",
			$usuario,
			$perfil
		);
		$lista = $db->carregar( $sql );
		$permissoes[$usuario] = array();
		if ( is_array( $lista ) ) {
			foreach ( $lista as $item ) {
				array_push( $permissoes[$usuario], $item['atiid'] );
			}
		}
	}
	return in_array( $atividade, $permissoes[$usuario] );
}

function projeto_verificar_responsabilidade( $projeto, $usuario = null ){
	global $db;
	static $permissoes = array(); # responsabilidades atribuídas
	if ( $db->testa_superuser() ) {
		return true;
	}
	$usuario = $usuario ? $usuario : $_SESSION['usucpf'];
	if ( $_SESSION["sisid"] == 1 ) {
		return acao_verificar_responsabilidade( $projeto, $usuario );
	}
	/*
	$sql = sprintf(
		"select count(*)
		from projetos.usuarioresponsabilidade ur
			inner join projetos.f_dadostodasatividades() as raiz on
				raiz.atiid = ur.atiid
			inner join seguranca.perfilusuario pu on pu.pflcod = ur.pflcod and pu.usucpf = ur.usucpf
			inner join projetos.f_dadostodasatividades() as folha on
				folha.atiid = raiz.atiid or folha.numero like raiz.numero || '.%%'
		where ur.atiid = %d and ur.rpustatus = 'A' and ur.usucpf = '%s' and ur.pflcod in ( %d, %d )
		",
		$projeto,
		$usuario,
		PERFIL_GESTOR,
		PERFIL_EQUIPE_APOIO_GESTOR
	);
	*/
	
	$sql = sprintf(
		"select count(*)
		from projetos.usuarioresponsabilidade ur
			inner join projetos.atividade as raiz on
				raiz.atiid = ur.atiid
			inner join seguranca.perfilusuario pu on pu.pflcod = ur.pflcod and pu.usucpf = ur.usucpf
			inner join projetos.atividade as folha on
				folha.atiid = raiz.atiid or folha._atinumero like raiz._atinumero || '.%%'
		where raiz.atistatus = 'A' and folha.atistatus = 'A' and ur.atiid = %d and ur.rpustatus = 'A' and ur.usucpf = '%s' and ur.pflcod in ( %d, %d )
		",
		$projeto,
		$usuario,
		PERFIL_GESTOR,
		PERFIL_EQUIPE_APOIO_GESTOR
	);
	
	return $db->pegaUm( $sql ) > 0;
}

/**
 * @return boolean
 */
function usuario_possui_perfil( $perfil, $usuario = null ){
	global $db;
	if ( $db->testa_superuser() ) {
		return true;
	}
	$usuario = $usuario ? $usuario : $_SESSION['usucpf'];
	$sql = sprintf(
		"select count( * )
		from seguranca.perfilusuario
		where
			usucpf = '%s' and
			pflcod = %d",
		$usuario,
		$perfil
	);
	return (boolean) $db->pegaUm( $sql );
}


// ORDEM E NÍVEL DAS ATIVIDADES //////////////////////////////////////////////////////


function atividade_ordem_subir( $atiid ){
	global $db;
	// verifica se está no topo
	$sql = sprintf(
		"select a.* from projetos.atividade a where a.atiid = %d and a.atistatus = 'A' and a.atiordem > 1",
		$atiid
	);
	$atividade = $db->pegaLinha( $sql );
	if ( !$atividade ) {
		return true;
	}
	// altera a posição dos irmãos
	$sql = sprintf(
		"update projetos.atividade set atiordem = %d where atiordem = %d and atiidpai = %d and atistatus = 'A'",
		$atividade['atiordem'],
		$atividade['atiordem'] - 1,
		$atividade['atiidpai']
	);
	if( !$db->executar( $sql ) ) {
		$db->rollback();
		return false;
	}
	// altera a posição da atividade
	$sql = sprintf(
		"update projetos.atividade set atiordem = %d where atiid = %d and atistatus = 'A'",
		$atividade['atiordem'] - 1,
		$atividade['atiid']
	);
	if( !$db->executar( $sql ) ) {
		$db->rollback();
		return false;
	}
	return true;
}

function atividade_ordem_descer( $atiid ){
	global $db;
	// verifica se está no final
	$sql = sprintf(
		"select a1.* from projetos.atividade a1 where atiid = %d and atistatus = 'A' and a1.atiordem < ( select count(*) from projetos.atividade a2 where a2.atiidpai = a1.atiidpai and atistatus = 'A' )",
		$atiid
	);
	$atividade = $db->pegaLinha( $sql );
	if ( !$atividade ) {
		return true;
	}
	// altera a posição dos irmãos
	$sql = sprintf(
		"update projetos.atividade set atiordem = %d where atiordem = %d and atiidpai = %d and atistatus = 'A'",
		$atividade['atiordem'],
		$atividade['atiordem'] + 1,
		$atividade['atiidpai']
	);
	if( !$db->executar( $sql ) ) {
		$db->rollback();
		return false;
	}
	$sql = sprintf(
		"update projetos.atividade set atiordem = %d where atiid = %d and atistatus = 'A'",
		$atividade['atiordem'] + 1,
		$atividade['atiid']
	);
	if( !$db->executar( $sql ) ) {
		$db->rollback();
		return false;
	}
	return true;
}

function atividade_profundidade_esquerda( $atiid ){
	global $db;
	// carrega os dados da atividade
	$sql = sprintf(
		"select a1.* from projetos.atividade a1 where atiid = %d and atistatus = 'A'",
		$atiid
	);
	$atividade = $db->pegaLinha( $sql );
	if ( !$atividade ) {
		return false;
	}
	// carrega os dados do antigo pai da atividade
	$sql = sprintf(
		"select a1.* from projetos.atividade a1 where atiid = %d and atistatus = 'A'",
		$atividade['atiidpai']
	);
	$atividade_pai = $db->pegaLinha( $sql );
	if ( !$atividade_pai ) {
		$db->rollback();
		return false;
	}
	// desloca os novos irmãos para baixo
	$sql = sprintf(
		"update projetos.atividade set atiordem = atiordem + 1 where atistatus = 'A' and atiidpai = %d and atiordem > %d",
		$atividade_pai['atiidpai'],
		$atividade_pai['atiordem']
	);
	if ( !$db->executar( $sql ) ) {
		$db->rollback();
		return false;
	}
	// desloca os antigos irmãos para cima
	$sql = sprintf(
		"update projetos.atividade set atiordem = atiordem - 1 where atistatus = 'A' and atiidpai = %d and atiordem > %d",
		$atividade['atiidpai'],
		$atividade['atiordem']
	);
	if ( !$db->executar( $sql ) ) {
		$db->rollback();
		return false;
	}
	// troca o pai (pelo avô)
	$sql = sprintf(
		"update projetos.atividade set atiidpai = %d, atiordem = %d where atistatus = 'A' and atiid = %d",
		$atividade_pai['atiidpai'],
		$atividade_pai['atiordem'] + 1,
		$atividade['atiid']
	);
	if ( !$db->executar( $sql ) ) {
		$db->rollback();
		return false;
	}
	return true;
}

function atividade_profundidade_direita( $atiid ){
	global $db;
	// carrega os dados da atividade
	$sql = sprintf(
		"select a1.* from projetos.atividade a1 where atiid = %d and atistatus = 'A'",
		$atiid
	);
	$atividade = $db->pegaLinha( $sql );
	if ( !$atividade ) {
		return false;
	}
	// carrega o novo pai (irmão que está uma posição acima)
	$sql = sprintf(
		"select a1.* from projetos.atividade a1 where atiidpai = %d and atiordem = %d and atistatus = 'A'",
		$atividade['atiidpai'],
		$atividade['atiordem'] - 1
	);
	$atividade_pai = $db->pegaLinha( $sql );
	if ( !$atividade_pai ) {
		$db->rollback();
		return false;
	}
	// desloca os antigos irmãos para cima
	$sql = sprintf(
		"update projetos.atividade set atiordem = atiordem - 1 where atiidpai = %d and atiordem > %d and atistatus = 'A' ",
		$atividade['atiidpai'],
		$atividade['atiordem']
	);
	if ( !$db->executar( $sql ) ) {
		$db->rollback();
		return false;
	}
	// troca o pai (pelo antigo irmão)
	$sql = sprintf(
		"update projetos.atividade set atiidpai = %d, atiordem = 1 + ( select count(*) from projetos.atividade where atiidpai = %d and atistatus = 'A' ) where atiid = %d and atistatus = 'A'",
		$atividade_pai['atiid'],
		$atividade_pai['atiid'],
		$atividade['atiid']
	);
	if ( !$db->executar( $sql ) ) {
		$db->rollback();
		return false;
	}
	return true;
}


// ÁRVORE //////////////////////////////////////////////////////////////////////


function arvore_ocultar_item( $atividade ){
	if ( !isset( $_SESSION['arvore'] ) ) {
		$_SESSION['arvore'] = array();
	}
	$_SESSION['arvore'][$atividade] = $atividade;
}

function arvore_exibir_item( $atividade ){
	if ( !isset( $_SESSION['arvore'] ) ) {
		$_SESSION['arvore'] = array();
	}
	unset( $_SESSION['arvore'][$atividade] );
}

function arvore_verificar_exibicao_item( $numero, $ignorar = array() ){
	if ( !array_key_exists( 'arvore', $_SESSION ) ) {
		arvore_iniciar_dados_sessao();
	}
	$ignorar = $ignorar ? $ignorar : array();
	$arvore = arvore_pegar_estado_exibicao( $numero );
	// verifica o estado de exibição do item
	$numero = explode( '.', substr( $numero, 0, strrpos( $numero, '.' ) ) );
	for ( $i = count( $numero ); $i > 0; $i-- ) {
		$numero_atual = implode( '.', array_slice( $numero, 0, $i ) );
		if ( in_array( $numero_atual, $arvore ) && !in_array( $numero_atual, $ignorar ) ) {
			return false;
		}
	}
	return true;
}

function arvore_verificar_exibicao_filhos( $numero ){
	if ( !array_key_exists( 'arvore', $_SESSION ) ) {
		arvore_iniciar_dados_sessao();
	}
	$arvore = arvore_pegar_estado_exibicao( $numero );
	return in_array( $numero, $arvore );
}

function arvore_pegar_estado_exibicao( $numero ){
	static $arvore = null;
	global $db;
	if ( !array_key_exists( 'arvore', $_SESSION ) ) {
		arvore_iniciar_dados_sessao();
	}
	// verifica se há alguma informação na sessão
	if ( empty( $_SESSION['arvore'] ) ) {
		return array();
	}
	// carrega os números a partir dos ids gravados na sessão
	if ( !is_array( $arvore ) ) {
		$sql = sprintf(
			"select numero from projetos.f_dadostodasatividades() where atiid in ( %s )",
			implode( ',', $_SESSION['arvore'] )
		);
		$arvore = array();
		$atividades = $db->carregar( $sql );
		if ( is_array( $atividades ) ) {
			foreach ( $atividades as $atividade ) {
				array_push( $arvore, $atividade['numero'] );
			}
		}
	}
	return $arvore;
}

function arvore_iniciar_dados_sessao(){
	global $db;
	$sql = "select atiid from projetos.atividade where atistatus = 'A' and _atiprojeto = " . PROJETO;
	$linhas = $db->carregar( $sql );
	$linhas = $linhas ? $linhas : array();
	foreach ( $linhas as $linha ){
		arvore_ocultar_item( (integer) $linha['atiid'] );
	}
} 


// OUTRAS FUNÇÕES //////////////////////////////////////////////////////////////


/**
 * Redireciona o navegador para a tela indicada.
 * 
 * @return void
 */
function redirecionar( $modulo, $acao, $parametros = array() ) {

	$parametros = http_build_query( (array) $parametros, '', '&' );
	header( "Location: ?modulo=$modulo&acao=$acao&$parametros" );
	exit();
}

/**
 * Verifica se um projeto está selecionado.
 * 
 * Caso uma atividade seja passada como parâmetro verifica se além de algum
 * projeto está selecionado essa atividade pertença ao projeto selecionado.
 * Essa função redireciona para a tela de projetos caso a verificação falhe.
 *
 * @param integer $atividade
 * @return void
 */
function projeto_verifica_selecionado( $atividade = null ) {
	global $db;
	
	$atividade = (integer) $atividade;
	// verifica se projeto está escolhido
	$sql = sprintf( "select count(atiid) from projetos.atividade where atiid = %d and atistatus = 'A'", $_SESSION['projeto'] );

	if ( $db->pegaUm( $sql ) != 1 ) {
		redirecionar( $_SESSION['paginainicial'], 'A' );
	}

	// verifica se a atividade indicada pertence ao projeto atual
	if ( !$atividade ) {
		return;
	}
	$sql = sprintf( "select _atiprojeto from projetos.atividade where atiid = %d", $atividade );

	if ( $db->pegaUm( $sql ) != $_SESSION['projeto'] ) {
		redirecionar( $_SESSION['paginainicial'], 'A' );
	}
}


// OUTRAS FUNÇÕES //////////////////////////////////////////////////////////////


function registrar_mensagem( $mensagem ){
	if ( !isset( $_SESSION['mensagem'] ) ) {
		$_SESSION['mensagem'] = array();
	}
	array_push( $_SESSION['mensagem'], $mensagem );
}

function exibir_mensagens(){
	if ( !isset( $_SESSION['mensagem'] ) ) {
		$_SESSION['mensagem'] = array();
	}
	if ( count( $_SESSION['mensagem'] ) == 0 ) {
		return;
	}
	$htm = '<script language="javascript" type="text/javascript">';
	$htm .= 'alert("'. implode( "\n", $_SESSION['mensagem'] ) .'")';
	$htm .= '</script>';
	$_SESSION['mensagem'] = array();
	return $htm;
}

function acao_verificar_responsabilidade( $atividade, $usuario ){
	global $db;
	$ano = $_SESSION['exercicio'];
	$sql = <<<EOS
		select count( u.usucpf )
		from projetos.atividade ati
		inner join monitora.acao aca on aca.acaid = ati.acaid
		inner join monitora.usuarioresponsabilidade ur on ur.acaid = aca.acaid
		inner join seguranca.perfil p on p.pflcod = ur.pflcod
		inner join seguranca.usuario u on u.usucpf = ur.usucpf
		inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf
		where
		ati.atistatus = 'A' and ati.atiid = $atividade
		and aca.acastatus = 'A'
		and ur.rpustatus = 'A' and ur.prsano = '$ano'
		and p.pflstatus = 'A'
		and u.suscod = 'A' and u.usucpf = '$usuario'
		and us.suscod = 'A'
EOS;
	return $db->pegaUm( $sql ) > 0;
}

function arrayPerfil(){ 
	global $db;

	$sql = sprintf("SELECT
					 pu.pflcod
					FROM
					 seguranca.perfilusuario pu
					 INNER JOIN seguranca.perfil p ON p.pflcod = pu.pflcod AND
					 	p.sisid = 11
					WHERE
					 pu.usucpf = '%s'
					ORDER BY
					 p.pflnivel",
				$_SESSION['usucpf']);
	return (array) $db->carregarColuna($sql,'pflcod');
}

function salvarAtividade()
{
	global $db;
	
	/*** Anexar arquivo ***/
	if( $_FILES["arquivo"] && $_POST["taaid"] && $_POST["anedescricao"] )
	{
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		
		$sql = "SELECT aneid,arqid FROM projetos.anexoatividade WHERE atiid = ".$_POST['atiid']." AND anestatus = 'A'";
		$dadosAnexo = $db->carregar($sql);
		
		if( $dadosAnexo )
		{
			$sql = "UPDATE projetos.anexoatividade SET anestatus = 'I' WHERE aneid = ".$dadosAnexo[0]['aneid'];
			$db->executar($sql);
			$sql = "UPDATE public.arquivo SET arqstatus = 'I' WHERE arqid = ".$dadosAnexo[0]['arqid'];
    		$db->executar($sql);
		}
		
		$campos	= array("anedescricao"     	=> "'".$_POST['anedescricao']."'",
						"atiid"     		=> $_POST['atiid'],
						"taaid"				=> $_POST["taaid"]
					   );	
		
		$file = new FilesSimec("anexoatividade", $campos ,"projetos");
		$file->setUpload( $_POST['anedescricao']);
	}
	
	extract($_POST);
	
	if(!$atidescricao){
		return array("msg" => "Favor preeencher o campo de descrição!");
	}else{
		$atidescricao = "'$atidescricao'";
	}
	
	$atidatainicio 	= !$atidatainicio ? "NULL" : "'".tratarDataAtividade($atidatainicio)."'";
	$atidatafim 	= !$atidatafim ? "NULL" : "'".tratarDataAtividade($atidatafim)."'";
	
	if($atiid){ //update
//		ver('aqui',$atiid,d);
		$sql = "update 
					projetos.atividade
				set
					atidescricao = $atidescricao,
					atidatainicio = $atidatainicio,
					atidatafim = $atidatafim,
					atidetalhamento = '$atidetalhamento',
					atimeta = '$atimeta'
				where
					atiid = $atiid;";
		
		//Inserir responsáveis
		$n = 0;
		
		$sql.= "update projetos.responsavelatividade set rpastatus = 'I' where atiid = $atiid;";
		
		if($entidresp){
			foreach($entidresp as $resp){
				$sql.= "insert into 
							projetos.responsavelatividade
						(entid,atiid,rpastatus,rpadtinclusao)
							values
						($resp,$atiid,'A',now());";
				$n++;
			}
		}
		
		$n = 0;
		
		$sql.= "update projetos.impacto set ipcstatus = 'I' where atiid = $atiid;";
		
		if($atiidprocesso){
			foreach($atiidprocesso as $atiidimpacto){
				$sql.= "insert into 
							projetos.impacto
						(atiid,atiidimpacto,ipcdsc,usucpfcadastro,usudatacadastro)
							values
						($atiid,$atiidimpacto,'".$ipcdsc[$n]."','".$_SESSION['usucpf']."',now());";
				$n++;
			}
		}
		
	}else{ //insert
		$sql = "insert into 
					projetos.atividade
				(atidescricao,atidatainicio,atidatafim,atidetalhamento,atimeta,atistatus)
					values
				($atidescricao,$atidatainicio,$atidatafim,$atidetalhamento,$atimeta,'A')";
	}
	
	if($db->executar($sql)){
		$db->commit();
		return array("msg" => "Operação realizada com sucesso!");
	}else{
		return array("msg" => "Não foi possível realizar a operação!");
	}
}

function tratarDataAtividade($data){
	
	if($data){
		$d = explode("/",$data);
		return $d[2]."-".$d[1]."-".$d[0];
	}else{
		return false;
	}
	
}
function salvarDadosAtividade()
{
	global $db;
	extract($_POST);
	
	if(!$atidescricao){
		return array("msg" => "Favor preeencher o campo de Nome!");
	}else{
		$atidescricao = "'$atidescricao'";
	}
	if(!$atidetalhamento){
		return array("msg" => "Favor preeencher o campo de descrição!");
	}else{
		$atidetalhamento = "'$atidetalhamento'";
	}
	
	$atidatafim 	= !$atidatainicio ? "NULL" : "'".$db->pegaUm("SELECT date '".formata_data_sql($atidatainicio)."' + integer '".$atiduracao."' as atidatafim")."'";
	
	$atidatainicio 	= !$atidatainicio ? "NULL" : "'".tratarDataAtividade($atidatainicio)."'";
	$nvcid		 	= !$nvcid ? "NULL" : $nvcid;
	
	if($atiid){ //update
		$sql = "update 
					projetos.atividade
				set
					atidescricao = $atidescricao,
					atidetalhamento = $atidetalhamento,
					atidatainicio = $atidatainicio,
					atidatafim = $atidatafim,
					atiduracao = $atiduracao,
					nvcid = $nvcid
				where
					atiid = $atiid;";
		
		$db->executar($sql);
		
		$sql = "select docid, '<p>'||i.iclid||' - '||i.icldsc||'</p><p>'||a._atinumero||' - '||a.atidescricao||'</p>' as descricao from projetos.itemchecklist i
		 		left join projetos.atividade a on i.atiid = a.atiid
		 		where a.atiid=".$atiid;
		
		$docsatualizar = $db->carregar($sql);
		
		if($docsatualizar[0]) {
			foreach($docsatualizar as $line) {
				$db->executar("update workflow.documento set docdsc='".$line['descricao']."' where docid='".$line['docid']."'");
			}
		}
		
		//Inserir responsáveis
		/*$n = 0;
		
		$sql.= "update projetos.responsavelatividade set rpastatus = 'I' where atiid = $atiid;";
		
		if($entidresp){
			foreach($entidresp as $resp){
				$sql.= "insert into 
							projetos.responsavelatividade
						(entid,atiid,rpastatus,rpadtinclusao)
							values
						($resp,$atiid,'A',now());";
				$n++;
			}
		}*/
		
	}else{ //insert
		$sql = "insert into 
					projetos.atividade
				(atidescricao,atidetalhamento,atidatainicio,atiduracao,nvcid,atistatus)
					values
				($atidescricao,$atidetalhamento,$atidatainicio,$atiduracao,$nvcid,'A')";
		
		$db->executar($sql);
	}


	
	if($db->commit($sql)){
		return array("msg" => "Operação realizada com sucesso!");
	}else{
		return array("msg" => "Não foi possível realizar a operação!");
	}
}

function temPerfilSomenteConsulta()
{
	global $db;
	
	if( $db->testa_superuser() )
	{
		return false;
	}
	else
	{
		$sql = "SELECT count(1) FROM perfilusuario WHERE usucpf = '".$_SESSION["usucpf"]."' AND pflcod = ".PERFIL_SOMENTE_CONSULTA;
		$perfil = $db->pegaUm($sql);
		
		if($perfil > 0)
			return true;
		else
			return false;
	}
}

function temPerfilAdministrador()
{
	global $db;
	
	if( $db->testa_superuser() )
	{
		return false;
	}
	else
	{
		$sql = "SELECT count(1) FROM perfilusuario WHERE usucpf = '".$_SESSION["usucpf"]."' AND pflcod = ".PERFIL_ADMINISTRADOR;
		$perfil = $db->pegaUm($sql);
		
		if($perfil > 0)
			return true;
		else
			return false;
	}
}

function temPerfilExecValidCertif()
{
	global $db;
	
	if( $db->testa_superuser() )
	{
		return false;
	}
	else
	{
		$sql = "SELECT count(1) FROM perfilusuario WHERE usucpf = '".$_SESSION["usucpf"]."' AND pflcod in (".PERFIL_EXECUTOR.",".PERFIL_VALIDADOR.",".PERFIL_CERTIFICADOR.")";
		$perfil = $db->pegaUm($sql);
		
		if($perfil > 0)
			return true;
		else
			return false;
	}
}

?>