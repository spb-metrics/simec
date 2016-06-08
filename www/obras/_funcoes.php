<?php


/**
 * Busca os orgï¿½os que o usuï¿½rio possui responsabilidade
 * @author Fernando Bagno 
 * @since 27/04/2010
 * 
 * @param string $usucpf
 * @return array
 * 
 */
function obrPegaOrgidPermitido( $usucpf ){
	
	global $db;
	
	if( $db->testa_superuser() || obras_possuiPerfilSemVinculo() ){
		return true;
			
	}else{

		
//		$sql = "(	
//					SELECT DISTINCT
//						coalesce(o.orgdesc, o2.orgdesc) as descricao,
//						coalesce(o.orgid, o2.orgid) as id,
//						ur.pflcod as perfil
//					FROM
//						obras.usuarioresponsabilidade ur 
//					LEFT JOIN 
//						obras.orgao o ON ur.orgid = o.orgid
//					LEFT JOIN 
//						seguranca.perfil p ON ur.pflcod = p.pflcod
//					LEFT JOIN 
//						seguranca.perfilusuario pu ON pu.pflcod = ur.pflcod 
//													  AND pu.usucpf = ur.usucpf
//					LEFT JOIN 
//						entidade.entidade en ON ur.entid = en.entid
//					LEFT JOIN
//						entidade.funcaoentidade ef ON ef.entid = en.entid
//					LEFT JOIN 
//						obras.orgaofuncao of ON ef.funid = of.funid
//					LEFT JOIN 
//						obras.orgao o2 ON of.orgid = o2.orgid
//					WHERE
//						ur.usucpf = '{$usucpf}' AND
//						ur.rpustatus = 'A' AND
//						ur.rpuid NOT IN (SELECT 
//											rpuid 
//										 FROM 
//											obras.usuarioresponsabilidade ur1 
//										 WHERE 
//											ur1.rpustatus = 'A'
//											AND ur1.usucpf = '{$usucpf}'
//											AND ur1.pflcod != " . PERFIL_EMPRESA . " 
//											AND ur1.pflcod != " . PERFIL_SUPERVISORUNIDADE . " 
//											AND ur1.obrid IS NOT NULL) AND
//						p.sisid = 15
//					ORDER BY
//						id
//				)UNION ALL(
//					SELECT DISTINCT
//						coalesce(o.orgdesc, o2.orgdesc) as descricao,
//						coalesce(o.orgid, o2.orgid) as id,
//						ur.pflcod as perfil
//					FROM
//						obras.usuarioresponsabilidade ur 
//					INNER JOIN
//						obras.obrainfraestrutura oi ON oi.obrid = ur.obrid
//					LEFT JOIN 
//						obras.orgao o ON o.orgid = oi.orgid
//					LEFT JOIN 
//						seguranca.perfil p ON ur.pflcod = p.pflcod
//					LEFT JOIN 
//						seguranca.perfilusuario pu ON pu.pflcod = ur.pflcod 
//													  AND pu.usucpf = ur.usucpf
//					LEFT JOIN 
//						entidade.entidade en ON ur.entid = en.entid
//					LEFT JOIN
//						entidade.funcaoentidade ef ON ef.entid = en.entid
//					LEFT JOIN 
//						obras.orgaofuncao of ON ef.funid = of.funid
//					LEFT JOIN 
//						obras.orgao o2 ON of.orgid = o2.orgid
//					WHERE
//						ur.usucpf = '{$usucpf}' AND
//						ur.rpustatus = 'A' AND
//						ur.rpuid NOT IN (SELECT 
//											rpuid 
//										 FROM 
//											obras.usuarioresponsabilidade ur1 
//										 WHERE 
//											ur1.rpustatus = 'A'
//											AND ur1.usucpf = '{$usucpf}'
//											AND ur1.pflcod != " . PERFIL_SUPERVISORUNIDADE . " 
//											AND ur1.obrid IS NOT NULL) AND
//						p.sisid = 15
//					ORDER BY
//						id
//				)UNION ALL(
//					SELECT 
//						DISTINCT
//						o.orgdesc AS descricao,
//						o.orgid AS id,
//						ur.pflcod AS perfil 
//					FROM
//						obras.usuarioresponsabilidade ur
//					JOIN obras.obrainfraestrutura oi ON oi.obrid = ur.obrid
//					JOIN obras.orgao 			   o ON  o.orgid = oi.orgid
//					WHERE
//						ur.rpustatus = 'A'
//						AND ur.obrid IS NOT NULL
//						AND ur.usucpf = '{$usucpf}'
//						AND ur.pflcod = " . PERFIL_EMPRESA . "
//					ORDER BY 
//						id
//				)";

		//busca as responsabilidades do usuário
		$responsabilidades = pegaResponsabilidadePorPerfil();
		//possui responsabilidade
		if(is_array($responsabilidades)){
			foreach ($responsabilidades as $index=>$value){
				$index = $index == 'entid' ? 'entidunidade' : $index;
				$responsabilidadesTmp[] = "{$index} IN ('".implode("','", $value)."')";
				if($index == 'orgid')
					$responsabilidadesOrgid[] = "{$index} IN ('".implode("','", $value)."')";
			}
				$responsabilidades      = "(".implode(" OR ", $responsabilidadesTmp).")";
				if(is_array($responsabilidadesOrgid))
					$responsabilidadesOrgid = "OR ".implode(" OR ", $responsabilidadesOrgid)."";
		}elseif($responsabilidades === true)
			$responsabilidades = 'TRUE'; //pode ver tudo
		else
			$responsabilidades = 'FALSE'; //não pode ver nada

		$sql = "SELECT DISTINCT
					org.orgid AS id,
					org.orgdesc AS descricao
				FROM
					obras.orgao org
				WHERE 
					orgid IN (SELECT 
								orgid
							  FROM
							  	obras.obrainfraestrutura
							  WHERE {$responsabilidades}) 
				 {$responsabilidadesOrgid};";

		$arDados = $db->carregar( $sql );
		if( is_array($arDados) ){
			foreach($arDados as $valor){
				if(!empty($valor['descricao']) ){
					$dados[] = $valor;
				}
			}
		}
		
		return $dados;
	}
	
}

/**
 * Monta as abas do menu inicial do mï¿½dulo, com os tipos de ensino
 * @author Fernando Bagno 
 * @since 27/04/2010
 * 
 * @param string $usucpf
 * @param integer $orgid
 * @return mixed
 * 
 */
function obrMontaAbasTipoEnsino( $usucpf, $orgid = "" ){
	
	// itens do menu padrï¿½o (super user, perfil sem responsabilidade)
	$arAbas = array( 0 => array( "descricao" => "Assistência Social", 	   
								 "link" 	 => "obras.php?modulo=inicio&acao=A&orgid=" . ORGAO_SESU  ),
					 1 => array( "descricao" => "Segurança Pública",
					 			 "link" 	 => "obras.php?modulo=inicio&acao=A&orgid=" . ORGAO_SETEC ),
					 2 => array( "descricao" => "Habitação",
					 			 "link" 	 => "obras.php?modulo=inicio&acao=A&orgid=" . ORGAO_FNDE  ),
 					 3 => array( "descricao" => "Educação",
					 			 "link" 	 => "obras.php?modulo=inicio&acao=A&orgid=" . ORGAO_ADM  ),
 					 4 => array( "descricao" => "Saúde",
					 			 "link" 	 => "obras.php?modulo=inicio&acao=A&orgid=" . ORGAO_REHUF ),
 					 5 => array( "descricao" => "Saneamento",
					 			 "link" 	 => "obras.php?modulo=inicio&acao=A&orgid=" . ORGAO_SANEAMENTO ),
 					 6 => array( "descricao" => "Gestão Ambiental",
					 			 "link" 	 => "obras.php?modulo=inicio&acao=A&orgid=" . ORGAO_AMBIENTAL ),
 					 7 => array( "descricao" => "Energia",
					 			 "link" 	 => "obras.php?modulo=inicio&acao=A&orgid=" . ORGAO_ENERGIA ),
 					 8 => array( "descricao" => "Transporte",
					 			 "link" 	 => "obras.php?modulo=inicio&acao=A&orgid=" . ORGAO_TRANSPORTE ),
 					 9 => array( "descricao" => "Desporto e Lazer",
					 			 "link" 	 => "obras.php?modulo=inicio&acao=A&orgid=" . ORGAO_LAZER ) );
					 
					 /*,
					 3 => array( "descricao" => "Administrativo",
					 			 "link" 	 => "obras.php?modulo=inicio&acao=A&orgid=" . ORGAO_ADM   ) )*/
					 
	// busca os orgï¿½os do usuario logado 
	$orgids = obrPegaOrgidPermitido( $usucpf );
	// cria os itens de menu com os orgï¿½os do usuï¿½rio
	if( is_array( $orgids ) ){
		
		$arAbas  = array();
		$arOrgid = array();
		for( $i = 0; $i < count($orgids); $i++ ){
			if ( in_array($orgids[$i]["id"], $arOrgid) ){
				continue;
			}
			$arOrgid[] = $orgids[$i]["id"];
			if( !empty( $orgids[$i]["descricao"] ) ){
				array_push( $arAbas, array( "descricao" => $orgids[$i]["descricao"],
											"link" 		=> "obras.php?modulo=inicio&acao=A&orgid={$orgids[$i]["id"]}" ) );
			}
		}
		
		$orgid = empty($orgid) ? $orgids[0]["id"] : $orgid;
		
	}
	
	// cria o id do orgï¿½o que o usuï¿½rio selecionou (default o primeiro orgï¿½o que o usuï¿½rio possuir responsabilidade)
	$orgid = empty($orgid) ? 1 : $orgid;
	
	$_REQUEST["orgid"] = empty($_REQUEST["orgid"]) ? $orgid : $_REQUEST["orgid"]; 
	
	return montarAbasArray( $arAbas, "obras.php?modulo=inicio&acao=A&orgid={$orgid}" );
								 
}

function obrBuscaCampusEntidade( $entid ){
	
	global $db;
	
	$sql = "SELECT 
				e2.entid as codigo
			FROM
				entidade.entidade e2
			INNER JOIN
				entidade.entidade e ON e2.entid = e.entid
			INNER JOIN
				entidade.funcaoentidade ef ON ef.entid = e.entid
			INNER JOIN
				entidade.funentassoc ea ON ea.fueid = ef.fueid
			WHERE
				ea.entid = {$entid} AND
				e.entstatus = 'A' AND 
				ef.funid IN (17,18)";
	
	return $db->carregar( $sql );
	
}

function obrBuscaCampusObras( $entid ){
	
	global $db;
	
	$sql = "SELECT
				obrid
			FROM
				obras.obrainfraestrutura oi
			WHERE
				oi.entidcampus = {$entid} AND
				oi.obsstatus = 'A'";
	
	return $db->carregar( $sql );
	
}

// ---------- FIM FUNï¿½ï¿½ES NOVA TELA INICIAL ----------
function obras_atribuiPermissaoPerfil(Array $arPermissao, $dados, Array $arParam = null){
	if ( $dados ){
		foreach ( $dados as $dado ){
			if ( $arParam['existepula'] ){
				$arParam['existepula'] = (array) $arParam['existepula'];
				
				switch(true){
					case in_array('unidade', $arParam['existepula']) :
						if ( in_array( $dado['unidade'], $arPermissao['unidade']) )
							continue 2;
						break;
					case in_array('campus', $arParam['existepula']):
						if ( in_array( $dado['campus'], $arPermissao['campus']) )
							continue 2;
						break;
					case in_array('obras', $arParam['existepula']):
						if ( in_array( $dado['obras'], $arPermissao['obras']) )
							continue 2;
						break;
					case in_array('orgao', $arParam['existepula']):
						if ( in_array( $dado['orgao'], $arPermissao['orgao']) )
							continue 2;
						break;
					case in_array('estado', $arParam['existepula']):
						if ( in_array( $dado['estado'], $arPermissao['estado']) )
							continue 2;
						break;
				}	
			}				
				
			if ( $dado['unidade'] )
				$arPermissao['unidade'][] = $dado['unidade']; 
			if ( $dado['campus'] )
				$arPermissao['campus'][]  = $dado['campus']; 
			if ( $dado['obra'] )
				$arPermissao['obra'][] 	  = $dado['obra'];
			if ( $dado['orgao'] )
				$arPermissao['orgao'][]   = $dado['orgao'];
			if ( $dado['estado'] )
				$arPermissao['estado'][]   = $dado['estado'];
		}
		$arPermissao['unidade'] = array_unique($arPermissao['unidade']);
		$arPermissao['campus']  = array_unique($arPermissao['campus']);
		$arPermissao['obra'] 	= array_unique($arPermissao['obra']);
		$arPermissao['orgao'] 	= array_unique($arPermissao['orgao']);
		$arPermissao['estado'] 	= array_unique($arPermissao['estado']);
	}		
	return $arPermissao;
}

function obras_permissaoPerfil(){
	global $db;
	$retorno = true;
	
	if ( !obras_possuiPerfilSemVinculo() ){
		$arPerfil = obras_arrayPerfil();
		$retorno = array("unidade"	=> array(),
						 "orgao"	=> array(),
						 "estado"	=> array(),
						 "campus"	=> array(),
						 "obra"		=> array());

		/*
		 * 
		 * BUSCA RESPONSABILIDADE DE PERFï¿½S SEM PARTICULARIDADES - INï¿½CIO
		 * 
		 */
		// PERMISSï¿½ES dos perfis sem particularidade por UNIDADE
		$sql = "SELECT
					ur.entid AS unidade
				FROM
					obras.usuarioresponsabilidade ur
				WHERE
					ur.usucpf = '{$_SESSION['usucpf']}' 
					AND ur.pflcod NOT IN(" . PERFIL_EMPRESA . ", 
										 " . PERFIL_GESTORUNIDADE . ", 
										 " . PERFIL_SUPERVISORUNIDADE . ", 
										 " . PERFIL_CONSULTAESTADUAL . ", 
										 " . PERFIL_CONSULTATIPOENSINO . ")
					AND ur.rpustatus = 'A'
					AND ur.entid IS NOT NULL";
		
		$arDado = $db->carregar( $sql );
		$retorno = obras_atribuiPermissaoPerfil($retorno, $arDado);			 
		// PERMISSï¿½ES dos perfis sem particularidade por ESTADO
		$sql = "SELECT
					ur.estuf AS estado
				FROM
					obras.usuarioresponsabilidade ur
				WHERE
					ur.usucpf = '{$_SESSION['usucpf']}' 
					AND ur.pflcod NOT IN(" . PERFIL_EMPRESA . ", 
										 " . PERFIL_GESTORUNIDADE . ", 
										 " . PERFIL_SUPERVISORUNIDADE . ", 
										 " . PERFIL_CONSULTAESTADUAL . ", 
										 " . PERFIL_CONSULTATIPOENSINO . ")
					AND ur.rpustatus = 'A'
					AND ur.estuf IS NOT NULL";
		
		$arDado = $db->carregar( $sql );
		$retorno = obras_atribuiPermissaoPerfil($retorno, $arDado);			 
		// PERMISSï¿½ES dos perfis sem particularidade por TIPO DE ENSINO (orgï¿½o)
		$sql = "SELECT
					ur.orgid AS orgao
				FROM
					obras.usuarioresponsabilidade ur
				WHERE
					ur.usucpf = '{$_SESSION['usucpf']}' 
					AND ur.pflcod NOT IN(" . PERFIL_EMPRESA . ", 
										 " . PERFIL_GESTORUNIDADE . ", 
										 " . PERFIL_SUPERVISORUNIDADE . ", 
										 " . PERFIL_CONSULTAESTADUAL . ", 
										 " . PERFIL_CONSULTATIPOENSINO . ")
					AND ur.rpustatus = 'A'
					AND ur.orgid IS NOT NULL";
		
		$arDado = $db->carregar( $sql );
		$retorno = obras_atribuiPermissaoPerfil($retorno, $arDado);			
		/*
		 * 
		 * BUSCA RESPONSABILIDADE DE PERFï¿½S SEM PARTICULARIDADES - FIM
		 * 
		 * BUSCA RESPONSABILIDADE DE PERFï¿½S COM PARTICULARIDADES - INï¿½CIO
		 * 
		 */		
		// PERMISSï¿½ES perfil "Consulta Estadual"
		// Este perfil tem atribuiï¿½ao de responsabilidade por "tipo de ensino" e "estado", para efeito de filtro
		// ele sï¿½ retorna o "estado", pois o "tipo de ensino" jï¿½ estarï¿½ definido na listagem da tela inicial.
		if (  in_array( PERFIL_CONSULTAESTADUAL, $arPerfil ) ):
			$sql = "SELECT
						DISTINCT 
						ur.estuf AS estado
					FROM
						obras.usuarioresponsabilidade ur
					WHERE
						ur.usucpf = '{$_SESSION['usucpf']}' 
						AND ur.pflcod = " . PERFIL_CONSULTAESTADUAL . "
						AND ur.rpustatus = 'A'						
						AND ur.estuf IS NOT NULL";
			
			$arDado = $db->carregar( $sql );
			$retorno = obras_atribuiPermissaoPerfil($retorno, $arDado);
		endif;
		// PERMISSï¿½ES perfil "Consulta tipo de ensino"
		if (  in_array( PERFIL_CONSULTATIPOENSINO, $arPerfil ) ):
		//Consulta por obra atribuï¿½da ao Perfil "PERFIL_SUPERVISORMEC", removida dia 27/10/2010 as 14:48 H. */
				/*
				if ( $_SESSION['obra']['orgid'] == ORGAO_FNDE ){
				  	$sql = "SELECT
								DISTINCT 
								--oi.entidunidade AS unidade,
								--oi.entidcampus AS campus,
								oi.obrid AS obra
							FROM
								obras.usuarioresponsabilidade ur
							INNER JOIN 
								obras.obrainfraestrutura oi USING (obrid)
							WHERE
								ur.usucpf = '{$_SESSION['usucpf']}' 
								AND ur.pflcod = " . PERFIL_CONSULTATIPOENSINO . "
								AND ur.rpustatus = 'A'						
								AND ur.obrid IS NOT NULL";
			 	}else{ 
			 	*/
			$sql = "SELECT
						DISTINCT ur.orgid AS orgao
					FROM
						obras.usuarioresponsabilidade ur
					WHERE
						ur.usucpf = '{$_SESSION['usucpf']}' 
						AND ur.pflcod = " . PERFIL_CONSULTATIPOENSINO . "
						AND ur.rpustatus = 'A'						
						AND ur.orgid IS NOT NULL";				
				/*
			  	}
			 	*/
			
			$arDado = $db->carregar( $sql );
			$arParam['existepula'] = 'unidade';
			$retorno = obras_atribuiPermissaoPerfil($retorno, $arDado, $arParam);
		endif;	

		// PERMISSï¿½ES perfil "Supervisor Unidade"
		if (  in_array( PERFIL_SUPERVISORUNIDADE, $arPerfil ) ):
//			if ( $_SESSION['obra']['orgid'] == ORGAO_FNDE ){
				$sql = "SELECT DISTINCT 
							--oi.entidunidade AS unidade,
							--oi.entidcampus AS campus,
							oi.obrid AS obra
						FROM
							obras.usuarioresponsabilidade ur
						INNER JOIN 
							obras.obrainfraestrutura oi USING (obrid)
						WHERE
							ur.usucpf = '{$_SESSION['usucpf']}' 
							AND ur.pflcod = " . PERFIL_SUPERVISORUNIDADE . "
							AND ur.rpustatus = 'A'						
							AND ur.obrid IS NOT NULL";
//			}else{
			$arDado1 = $db->carregar( $sql );
				$sql = "SELECT DISTINCT 
							ur.entid AS unidade
						FROM
							obras.usuarioresponsabilidade ur
						WHERE
							ur.usucpf = '{$_SESSION['usucpf']}' 
							AND ur.pflcod = " . PERFIL_SUPERVISORUNIDADE . "
							AND ur.rpustatus = 'A'						
							AND ur.entid IS NOT NULL";				
//			}
			
//			dbg($sql,1);
			
			$arDado = $db->carregar( $sql );
			if(is_array($arDado1)){
				if(is_array($arDado)){
					foreach($arDado1 as $k =>$dados){
						if(is_array($arDado)){
							array_push($arDado, array ('obra' => $dados['obra']));
						}else{
							$arDado = array($dado);
						}
					}
				}else{
					$arDado = $arDado1;
				}
			}
			$arParam['existepula'] = array('obra','unidade');
//			ver($retorno, $arDado, $arParam, $arDado1);
			$retorno = obras_atribuiPermissaoPerfil($retorno, $arDado, $arParam);			
		endif;		
		
		// PERMISSï¿½ES perfil "Gestor Unidade"
		if (  in_array( PERFIL_GESTORUNIDADE, $arPerfil ) ):
//			if ( $_SESSION['obra']['orgid'] == ORGAO_FNDE ){
//				$sql = "SELECT
//							DISTINCT 
//							--oi.entidunidade AS unidade,
//							--oi.entidcampus AS campus,
//							oi.obrid AS obra
//						FROM
//							obras.usuarioresponsabilidade ur
//						INNER JOIN 
//							obras.obrainfraestrutura oi USING (obrid)
//						WHERE
//							ur.usucpf = '{$_SESSION['usucpf']}' 
//							AND ur.pflcod = " . PERFIL_GESTORUNIDADE . "
//							AND ur.rpustatus = 'A'						
//							AND ur.obrid IS NOT NULL";
//			}else{
				$sql = "SELECT
							DISTINCT ur.entid AS unidade
						FROM
							obras.usuarioresponsabilidade ur
						WHERE
							ur.usucpf = '{$_SESSION['usucpf']}' 
							AND ur.pflcod = " . PERFIL_GESTORUNIDADE . "
							AND ur.rpustatus = 'A'						
							AND ur.entid IS NOT NULL";				
//			}
			
			$arDado = $db->carregar( $sql );
			$arParam['existepula'] = 'unidade';
			$retorno = obras_atribuiPermissaoPerfil($retorno, $arDado, $arParam);		
		endif;		
		
		// PERMISSï¿½ES perfil "Empresa Contratada"
		if (  in_array( PERFIL_EMPRESA, $arPerfil ) ):
			$sql = "SELECT
						DISTINCT 
						--oi.entidunidade AS unidade,
						--oi.entidcampus AS campus,
						oi.obrid AS obra
					FROM
						obras.usuarioresponsabilidade ur
					INNER JOIN 
						obras.obrainfraestrutura oi USING (obrid)
					WHERE
						ur.usucpf = '{$_SESSION['usucpf']}' 
						AND ur.pflcod = " . PERFIL_EMPRESA . "
						AND ur.rpustatus = 'A'						
						AND ur.obrid IS NOT NULL";
			
			$arDado = $db->carregar( $sql );
			$arParam['existepula'] = $_SESSION['obra']['orgid'] == ORGAO_FNDE ? '' : 'unidade';
			$retorno = obras_atribuiPermissaoPerfil($retorno, $arDado, $arParam);			 
		endif;	

	}
	
	return $retorno;	
}


function obras_verificasessao(){
	
	if (!$_SESSION["obra"]["obrid"]):
		print '<script>
				alert("A sessão da obra expirou, favor selecioná-la novamente!");
				window.location = "?modulo=inicio&acao=A";
			  </script>';
		exit;
	endif;
	
	return false;
	
}

/**
 * Funï¿½ï¿½o que pega os cï¿½digos do usuï¿½rio logado no sistema para
 * verificar se o mesmo pode ou nï¿½o cadastrar uma obra
 * 
 * @author Fernando Araï¿½jo Bagno da Silva
 * @return array
*/
function obras_podeCadastrarObra(){

	global $db;
	
	$sql = "
		SELECT 
			orgcod, ungcod 
		FROM 
			seguranca.usuario 
		WHERE 
			usucpf = '{$_SESSION["usucpf"]}'";
	
	$dados = $db->carregar($sql);
	
	return $dados;

}

/**
 * Funï¿½ï¿½o que verifica se o usuï¿½rio possui perfil para acessar as pï¿½ginas
 *
 * @author Fernando Araï¿½jo Bagno da Silva
 * @param array $pflcods
 * @return integer possui ou nï¿½o perfil
 */
function possuiPerfil( $pflcods )
{
	global $db;
	
	if ( $db->testa_superuser() ) {
		
		return true;
		
	}else{
		
		if ( is_array( $pflcods ) )
		{
			$pflcods = array_map( "intval", $pflcods );
			$pflcods = array_unique( $pflcods );
		}
		else
		{
			$pflcods = array( (integer) $pflcods );
		}
		if ( count( $pflcods ) == 0 )
		{
			return false;
		}
		$sql = "
			select
				count(*)
			from seguranca.perfilusuario
			where
				usucpf = '" . $_SESSION['usucpf'] . "' and
				pflcod in ( " . implode( ",", $pflcods ) . " ) ";
		return $db->pegaUm( $sql ) > 0;
			
	}
}

function obras_possuiPerfilOrgao( $arPerfilEntid, $arPerfilOrgid, $org ){
	
	global $db;
	
	if ( $db->testa_superuser() ){
		return true;
	}
	
	if ( is_array( $arPerfilEntid ) ){
		$arPerfilEntid = array_map( "intval", $arPerfilEntid );
		$arPerfilEntid = array_unique( $arPerfilEntid );
	}else{
		$arPerfilEntid = array( (integer) $arPerfilEntid );
	}
	
	if ( count( $arPerfilEntid ) == 0 ){
		return false;
	}
	
	$arOrg = obras_recuperarPermissoesEntid( $arPerfilEntid );
	if ($org){
		$org = (array) $org;
	}else{
		$org = array();
		$res = obras_pegarOrgaoPermitido();
		if (is_array($res)){
			foreach ($res as $r){
				if ($r['id']) $org[] = $r['id'];
			}
		}
	}
	
	$sqlOrgid = "select 
					count(*) 
				 from 
				 	obras.usuarioresponsabilidade
	 			 where 
	 			 	pflcod in ( ". implode( ", ", $arPerfilOrgid ) ." )
				 	and usucpf = '{$_SESSION["usucpf"]}'
				 	and rpustatus = 'A'
				 	and orgid IN (" . implode(", ", $org) . ")";
	$resOrgid = $db->pegaUm( $sqlOrgid );
	
	$resObrid = verificaPermissaoObra($_SESSION['usucpf'], $_SESSION['obra']['obrid']);
	
	$arrPerfilBlockTela = array(PERFIL_CONSULTAESTADUAL, 
								PERFIL_CONSULTAUNIDADE, 
								PERFIL_CONSULTATIPOENSINO,
								PERFIL_SAA,
								PERFIL_CONSULTAGERAL/*,
								PERFIL_GESTORUNIDADE*/);
	$arrPerfil = obras_arrayPerfil();
	
	return (boolean) (array_intersect($org, $arOrg ) || $resOrgid || ($resObrid && array_diff($arrPerfil, $arrPerfilBlockTela) ));
}

function obras_recuperarPermissoesEntid( $arPerfilEntid = array() ){
	
	global $db;
	
	$sqlEntid = "	select distinct
						case 
							when funid = 12 then 1
							when funid in ( 11, 14 ) then 2
							else 3
						end
					as orgid
					from obras.usuarioresponsabilidade ur
					inner join entidade.funcaoentidade fe on fe.entid = ur.entid
					where pflcod in ( ". implode( ", ", $arPerfilEntid ) ." )
					and usucpf = '{$_SESSION["usucpf"]}'
					and rpustatus = 'A'
					and funid in ( 1, 3, 6, 7, 11, 12, 14 )
					and ur.entid is not null;";
	
	$arOrgid = $db->carregar( $sqlEntid );
	$arOrgid = $arOrgid ? $arOrgid : array();
	
	$arOrg = array();
	foreach( $arOrgid as $orgid ){
		$arOrg[] = $orgid["orgid"];
	}
	
	return $arOrg ? $arOrg : array();
}

/**
 * Verifica se o perfil do usuï¿½rio possui algum vï¿½nculo de responsabilidade
 *
 * @author Fernando Araï¿½jo Bagno da Silva
 * @return integer possui perfil com vï¿½nculo ou nï¿½o
 */
function obras_possuiPerfilSemVinculo(){
	
	global $db;
	
	$sql = "
		SELECT
			count(*)
		FROM 
			seguranca.perfil p
		INNER JOIN 
			seguranca.perfilusuario u on
			u.pflcod = p.pflcod
		LEFT JOIN 
			obras.tprperfil tp on
			tp.pflcod = p.pflcod
		LEFT JOIN 
			obras.tiporesponsabilidade tr on
			tr.tprcod = tp.tprcod
		WHERE
			p.pflstatus = 'A' AND
			p.sisid = '15' AND
			u.usucpf = '" . $_SESSION['usucpf'] . "' AND
			tr.tprcod is null
	";
	return $db->pegaUm( $sql ) > 0;
}

function obras_arrayPerfil(){
	global $db;
	
	$sql = sprintf("SELECT
						pu.pflcod
					FROM
						seguranca.perfilusuario pu
					INNER JOIN 
						seguranca.perfil p ON p.pflcod = pu.pflcod AND
					 	p.sisid = 15
					WHERE
						pu.usucpf = '%s'
					ORDER BY
						p.pflnivel",
					$_SESSION['usucpf']);
					
	return (array) $db->carregarColuna( $sql, 'pflcod' );
}

/**
 * Pega o ï¿½rgï¿½o que o usuï¿½rio possui responsabilidade
 *
 * @author Fernando Araï¿½jo Bagno da Silva
 * @return mixed
 * 
 */
function obras_pegarOrgaoPermitido(){
	
	global $db;
	static $orgao = null;
	
	if ($orgao === null){
		
		if ($db->testa_superuser() || obras_possuiPerfilSemVinculo()){
			
			// pega todos os orgï¿½os
			$sql = "
				SELECT
					o.orgdesc                                               as descricao,
	                o.orgid                                                 as id,
                	'/obras/obras.php?modulo=inicio&acao=A&org=' || o.orgid as link
				FROM
					obras.orgao o
				ORDER BY
					o.orgid";
		}else {
			$sql = " 
				SELECT DISTINCT
					coalesce(o.orgdesc, coalesce(o3.orgdesc,o2.orgdesc)) as descricao,
					coalesce(o.orgid, coalesce(o3.orgid,o2.orgid)) as id,
					'/obras/obras.php?modulo=inicio&acao=A&org=' || coalesce(o.orgid, o2.orgid) as link,
					ur.pflcod as perfil
				FROM
					obras.usuarioresponsabilidade ur 
				-- Por obra	
				LEFT JOIN
					obras.obrainfraestrutura oi ON oi.obrid = ur.obrid
				LEFT JOIN
					obras.orgao               o ON ur.orgid = o.orgid OR 
													o.orgid = oi.orgid
				--Por Entidade
				LEFT JOIN 
					entidade.entidade       en ON ur.entid = en.entid
				LEFT JOIN
					entidade.funcaoentidade ef ON ef.entid = en.entid
				LEFT JOIN 
					obras.orgaofuncao       of ON ef.funid = of.funid
				LEFT JOIN 
					obras.orgao             o2 ON of.orgid = o2.orgid
				--Por Orgid
				LEFT JOIN 
					obras.orgao o3 ON ur.orgid = o3.orgid
									
				LEFT JOIN 
					seguranca.perfil p ON ur.pflcod = p.pflcod
				LEFT JOIN 
					seguranca.perfilusuario pu ON pu.pflcod = ur.pflcod AND pu.usucpf = ur.usucpf
				WHERE
					ur.usucpf = '{$_SESSION["usucpf"]}' AND
					ur.rpustatus = 'A' AND
					p.sisid = 15";
		}
		
		$orgao = $db->carregar($sql);
		
	}
	return $orgao;
}

/**
 * Pega as unidades que o usuï¿½rio possui responsabilidade
 *
 * @author Fernando Araï¿½jo Bagno da Silva
 * @return mixed
 * 
 */
function obras_pegarUnidadesPermitidas(){
	
	global $db;
	static $unidades = null;
	
	if ($unidades === null){
		if ($db->testa_superuser() || obras_possuiPerfilSemVinculo()){
			return false;
		}else{
			
			// pega as unidades do perfil do usuï¿½rio
			$sql = "SELECT
						ur.entid
					FROM
						obras.usuarioresponsabilidade ur
					INNER JOIN 
						entidade.entidade et ON
						et.entid = ur.entid 
					INNER JOIN 
						seguranca.perfil p ON
						p.pflcod = ur.pflcod
					INNER JOIN 
						seguranca.perfilusuario pu ON
						pu.pflcod = ur.pflcod AND
						pu.usucpf = ur.usucpf
					WHERE
						ur.usucpf = '" . $_SESSION['usucpf'] . "' AND
						ur.rpustatus = 'A' AND
						p.sisid = 15
				UNION ALL
					SELECT
						DISTINCT oi.entidunidade
					FROM
						obras.usuarioresponsabilidade ur
					INNER JOIN 
						obras.obrainfraestrutura oi USING (obrid)
					WHERE
						ur.usucpf = '" . $_SESSION['usucpf'] . "' 
						AND ur.rpustatus = 'A'						
						AND ur.obrid IS NOT NULL";
		}
		
		
		
		$dados = $db->carregar($sql);
		$dados = $dados ? $dados : array();
		$unidades = array();
		
		foreach ( $dados as $linha ){
			array_push( $unidades, $linha['entid'] );
		}
	}
	return $unidades;	
}

/**
 * Pega as uf's que o usuï¿½rio possui responsabilidades
 *
 * @author Fernando Araï¿½jo Bagno da Silva
 * @return array
 */
function obras_pegarUfsPermitidas(){
	
	global $db;
	static $ufs = null;
	
	if ($ufs === null){
		if ($db->testa_superuser() || obras_possuiPerfilSemVinculo()){
			
			// pega todos os estados
			$sql = "
				SELECT
					estuf
				FROM 
					territorios.estado";
		}else{
			
			// pega estados do perfil do usuï¿½rio
			$sql = "
				SELECT
					e.estuf
				FROM 
					territorios.estado e
				INNER JOIN 
					obras.usuarioresponsabilidade ur on
					ur.estuf = e.estuf
				INNER JOIN 
					seguranca.perfil p on
					p.pflcod = ur.pflcod
				INNER JOIN 
					seguranca.perfilusuario pu on
					pu.pflcod = ur.pflcod and
					pu.usucpf = ur.usucpf
				WHERE
					ur.usucpf = '" . $_SESSION['usucpf'] . "' and
					ur.rpustatus = 'A' and
					p.sisid =  15
				ORDER BY
					e.estuf";
			
		}
		
		$dados = $db->carregar($sql);
		$dados = $dados ? $dados : array();
		$ufs = array();
		
		foreach ( $dados as $linha ){
			array_push( $ufs, $linha['estuf'] );
		}
	}
	return $ufs;
}

/**
 * Pega os municï¿½pios que o usuï¿½rio possui responsabilidade
 *
 * @author Fernando Araï¿½jo Bagno da Silva
 * @return unknown
 */
function obras_pegarEntidadesPermitidos(){
	
	global $db;
	static $entidades = null;
	
	if ($entidades === null){
		if ($db->testa_superuser() || obras_possuiPerfilSemVinculo()){
			
			// pega todos as entidades
			$sql = "
				SELECT
					et.entid
				FROM 
					entidade.entidade et
				INNER JOIN
					obras.usuarioresponsabilidade ur ON ur.entid = et.entid
				WHERE
					funid in (1, 3, 7)";
		
		}else{
			
			// pega os municï¿½pios do perfil do usuï¿½rio
			$sql = "
				SELECT
					ed.muncod
				FROM
					entidade.endereco ed
				INNER JOIN
					entidade.entidade et ON et.entid = ed.entid
				INNER JOIN
					obras.usuarioresponsabilidade ur ON ur.entid = et.entid
				INNER JOIN 
					seguranca.perfil p ON p.pflcod = ur.pflcod
				INNER JOIN 
					seguranca.perfilusuario pu ON pu.pflcod = ur.pflcod AND
					pu.usucpf = ur.usucpf
				WHERE
					ur.usucpf = '" . $_SESSION['usucpf'] . "' AND
					ur.rpustatus = 'A' AND
					p.sisid =  15";
		
		}
		
		$dados = $db->carregar( $sql );
		$dados = $dados ? $dados : array();
		$municipios = array();
		
		foreach ( $dados as $linha ){
			array_push( $municipios, $linha['muncod'] );
		}
	}
	return $municipios;
}

/**
 * Funï¿½ï¿½o que carrega as obras da lista
 */
function carregaObras(){

	global $db, $somenteLeitura, $habilitado;
	
	
	
	if( !$db->testa_superuser() || !possuiPerfil( PERFIL_ADMINISTRADOR, PERFIL_SUPERVISORMEC ) ){
		$res   = obras_pegarOrgaoPermitido();
		$_SESSION['pesquisaObra']["org"] = $res[0]['id'];	
	}
	
	$stBotaoExcluir = "' '";
	if( $habilitado && $_SESSION['pesquisaObra']["org"] != ORGAO_FNDE  || $db->testa_superuser() ){
		$stBotaoExcluir = "'<img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"javascript:Excluir(\'?modulo=inicio&acao=A&requisicao=excluir\', ' || oi.obrid || ');\">'";
	}
	
	if ( !($db->testa_superuser()) && ( !possuiPerfil(PERFIL_CONSULTAGERAL) && 
		 !possuiPerfil( PERFIL_GESTORMEC ) && !possuiPerfil( PERFIL_ADMINISTRADOR ) ) && $_SESSION['pesquisaObra']["org"] == ORGAO_FNDE ){
		
		$joinObras  = "INNER JOIN obras.usuarioresponsabilidade ur ON ur.obrid = oi.obrid";
		$filtroObra = "AND ur.usucpf = '{$_SESSION["usucpf"]}'";
		
	}
	
	$stFiltro = retornarFiltroPesquisa();
	$stFiltro = !empty( $stFiltro ) ? ' WHERE ' . $stFiltro : $stFiltro;
	
	$stFiltro = utf8_decode($stFiltro); 
	
	$sql = "
		SELECT DISTINCT
		    acao,
		    documento,
		    foto,		    
		    restricao,
		    pi,
		    aditivo,
		    obrid,
		    nome_obra,
		    municipio,
		    situacao,
		    inicio,
		    final,
		    tipoobra,   		    
		    CASE WHEN codigo_situacao NOT IN (3,4,5,6,99) THEN
			(CASE WHEN ultimadata > (CURRENT_DATE - integer '30') THEN '<label style=\"color:#00AA00;\">' ||  to_char(ultimadata, 'DD/MM/YYYY') || '</label>'
			      WHEN (ultimadata <= (CURRENT_DATE - integer '30') AND ultimadata >= (CURRENT_DATE - integer '45')) THEN '<label style=\"color:#BB9900;\">' || to_char(ultimadata, 'DD/MM/YYYY') || '</label>'
			 ELSE '<label style=\"color:#DD0000;\">' || to_char(ultimadata, 'DD/MM/YYYY') || '</label>' END)
			 WHEN codigo_situacao = 2 THEN '<label style=\"color:#DD0000;\">' || to_char(ultimadata, 'DD/MM/YYYY') || '</label>'
			 WHEN codigo_situacao = 3 THEN '<label style=\"color:blue;\">' || to_char(ultimadata, 'DD/MM/YYYY') || '</label>'
		    ELSE to_char(ultimadata, 'DD/MM/YYYY') END as ultimadata,
		    percentual 
		FROM (
		    SELECT DISTINCT
		        '<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"javascript:Atualizar(\'?modulo=principal\/\cadastro&acao=A\',' || oi.obrid || ');\">' || 
			". $stBotaoExcluir ." || '</center>'   as acao,
		        CASE WHEN aa2.obrid is not null THEN '<img src=\"/imagens/anexo.gif\" border=0 title=\"Ver documentos\" style=\"cursor:pointer;\" onclick=\"javascript:Atualizar(\'?modulo=principal\/\documentos&acao=A\',' || oi.obrid || ');\">' ELSE '' END as documento,
		        CASE WHEN aa.obrid is not null THEN '<img src=\"/imagens/cam_foto.gif\" border=0 title=\"Galeria de fotos\" style=\"cursor:pointer;\" onclick=\"javascript:Atualizar(\'?modulo=principal\/\album&acao=A\',' || oi.obrid || ');\">' ELSE '' END as foto,
		        CASE WHEN r.obrid is null THEN '' ELSE '<img src=\"/imagens/restricao.png\" border=0 title=\"Restriï¿½ï¿½o\" style=\"cursor:pointer;\" onclick=\"javascript:Atualizar(\'?modulo=principal/restricao&acao=A\',' || oi.obrid || ');\">' END as restricao,
				CASE WHEN o.obrid is not null THEN '<img src=\"/imagens/money.gif\" border=0 title=\"Plano interno\" style=\"cursor:pointer;\" onclick=\"javascript:Atualizar(\'?modulo=principal\/\cadastro_pi&acao=A\',' || oi.obrid || ');\">' ELSE '' END as pi,
				CASE WHEN obridaditivo is not null THEN '<img src=\"/imagens/check_p.gif\" border=0 title=\"Esta obra ï¿½ um aditivo\" style=\"cursor:pointer;\" onclick=\"javascript:Atualizar(\'?modulo=principal/cadastroAditivo&acao=A\',' || oi.obrid || ');\">' ELSE '' END as aditivo,
				oi.obrid,
		        CASE WHEN fr.covnumero is not null THEN '<a style=\"margin: 0 -20px 0 20px; text-transform:capitalize;\" href=\"#\" onclick=\"javascript:Atualizar(\'?modulo=principal\/\cadastro&acao=A\',' || oi.obrid || ');\">' || fr.covnumero || ' - ' || UPPER( oi.obrdesc ) || '</a>'	        
		        ELSE '<a style=\"margin: 0 -20px 0 20px; text-transform:capitalize;\" href=\"#\" onclick=\"javascript:Atualizar(\'?modulo=principal\/\cadastro&acao=A\',' || oi.obrid || ');\">' || UPPER( oi.obrdesc ) || '</a>' END as nome_obra,
		        upper(et.entnome) as descricao,
		        mun.mundescricao || '/' || ed.estuf as municipio,
		        to_char(oi.obrdtinicio,'DD/MM/YYYY') as inicio,
		        to_char(oi.obrdttermino,'DD/MM/YYYY') as final,
		        CASE WHEN oi.tobraid is null THEN 'Nï¿½o informado' ELSE tp.tobadesc END as tipoobra,
		        sto.stodesc as situacao,
		        oi.stoid as codigo_situacao,
		        CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END as ultimadata,
		        (SELECT replace(coalesce(round(SUM(icopercexecutado), 2), '0') || ' %', '.', ',') as total FROM obras.itenscomposicaoobra WHERE obrid = oi.obrid) as percentual
		    FROM
		        obras.obrainfraestrutura oi
		    LEFT JOIN
		    	obras.tipoobra tp ON tp.tobaid = oi.tobraid
		    INNER JOIN
		        entidade.entidade et ON oi.entidunidade = et.entid
		    LEFT JOIN
		        obras.situacaoobra sto ON oi.stoid = sto.stoid
		    INNER JOIN
		        entidade.endereco ed ON ed.endid = oi.endid
		    LEFT JOIN
		        territorios.municipio mun ON mun.muncod = ed.muncod
		    INNER JOIN
		        obras.orgao org ON oi.orgid = org.orgid
		    LEFT JOIN (
		        SELECT fri.obrid, covi.covid, covi.covnumero
		        FROM obras.formarepasserecursos fri
		        INNER JOIN obras.conveniosobra covi ON covi.covid = fri.covid  )fr ON fr.obrid = oi.obrid
		    LEFT JOIN
			    obras.arquivosobra aa ON aa.obrid = oi.obrid and aa.tpaid = 21
			LEFT JOIN
			    obras.arquivosobra aa2 ON aa2.obrid = oi.obrid and aa2.tpaid <> 21
			LEFT JOIN
				monitora.pi_obra o ON o.obrid = oi.obrid 
			LEFT JOIN
				monitora.pi_planointerno mpi ON mpi.pliid = o.pliid AND mpi.plistatus = 'A'			        
		    LEFT JOIN
		        public.arquivo pa ON pa.arqid = aa.arqid and aa.aqostatus = 'A'
		    LEFT JOIN
		        (SELECT distinct obrid, rststatus
		         FROM obras.restricaoobra
		         WHERE rststatus = 'A') r ON r.obrid = oi.obrid
		    {$joinObras}   
			" . $stFiltro . $filtroObra . "
		    GROUP BY
		        org.orgdesc, oi.obrid, oi.obrdesc, oi.obrdtinicio, oi.obrdttermino,
		        oi.tobraid, tp.tobadesc, sto.stodesc, r.obrid, fr.covnumero, oi.obridaditivo, et.entnome,
		        mun.mundescricao, ed.estuf, aa.obrid, oi.obrpercexec,
		        oi.obsdtinclusao, oi.stoid, oi.obrdtvistoria, aa2.obrid, mpi.plistatus, o.obrid, mpi.plicod
		    ORDER BY
		        municipio) as foo ORDER BY obrid";

		    
		    
	$cabecalho = array( "Ação", "A", "F", "R", "PI", "AD", "ID", "Nome da Obra", "Município/UF", "Situação da Obra", "Início de Execução da Obra", "Término de Execução da Obra", "Tipo de Obra", "Última Atualização", "(%) Executado" );
	$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%');
}

/**
 * Funï¿½ï¿½o que carrega as obras da lista
 */
function carregaObrasRelacionadas($popup = false,$obridrel = false){

	global $db, $somenteLeitura, $habilitado;
	
	if($popup){
		$includes = '<script language="JavaScript" src="../includes/agrupador.js"></script>';
		$includes = '<script language="JavaScript" src="../includes/funcoes.js"></script>';
		$includes.= '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
		$includes.= '<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
		echo $includes;
	}
	
	if( !$db->testa_superuser() || !possuiPerfil( PERFIL_ADMINISTRADOR ) ){
		$res   = obras_pegarOrgaoPermitido();
		$_SESSION['pesquisaObra']["org"] = $res[0]['id'];	
	}
	
	$stBotaoExcluir = "' '";
	if( $habilitado && $_SESSION['pesquisaObra']["org"] != ORGAO_FNDE  || $db->testa_superuser() ){
		$stBotaoExcluir = "'<img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"javascript:Excluir(\'?modulo=inicio&acao=A&requisicao=excluir\', ' || oi.obrid || ');\">'";
	}
	
	if ( !($db->testa_superuser()) && ( !possuiPerfil(PERFIL_CONSULTAGERAL) && 
		 !possuiPerfil( PERFIL_GESTORMEC ) && !possuiPerfil( PERFIL_ADMINISTRADOR ) ) && $_SESSION['pesquisaObra']["org"] == ORGAO_FNDE ){
		
		$joinObras  = "INNER JOIN obras.usuarioresponsabilidade ur ON ur.obrid = oi.obrid";
		$filtroObra = "AND ur.usucpf = '{$_SESSION["usucpf"]}'";
		
	}
	
	$stFiltro = retornarFiltroPesquisa();
	
	$stFiltro = !empty( $stFiltro ) ? ' WHERE ' . $stFiltro : $stFiltro;
	
	if( !empty( $_SESSION["obra"]["obrid"] ) ){
		$sql = "SELECT entidunidade FROM obras.obrainfraestrutura WHERE obrid = {$_SESSION["obra"]["obrid"]}";
		$entidUnidade = $db->pegaUm( $sql );
		if( !empty( $entidUnidade ) ){
			$stFiltro .= " AND entidunidade = {$entidUnidade}";
		}	
	}
	
	
	$sqlPai = "select obridrelacionada from obras.obrainfraestrutura where obrid = {$_SESSION['obra']['obrid']}";
       
	if(!$popup){
		$stFiltro.= " AND( oi.obridrelacionada = {$_SESSION['obra']['obrid']} ";
		$stFiltro.= " or oi.obrid  in ($sqlPai) ) ";
		$stFiltro.= " AND oi.obrid != {$_SESSION['obra']['obrid']} ";
	}
 
	if($popup){
		$stFiltro.= " AND (oi.obridrelacionada = {$_SESSION['obra']['obrid']} OR oi.obridrelacionada is null )";
	}
	
	
	$stFiltro = utf8_decode($stFiltro); 
	
	$sql = "
		SELECT DISTINCT
		".($popup ? "( CASE WHEN obridrelacionada = $obridrel 
							THEN '<input type=\"checkbox\" onclick=\"carregaConteudoObraTabela(this,' || obrid || ')\" checked=\"checked\" name=\"obrid[' || obrid || ']\" id=\"obrid_' || obrid || '\" />'
							ELSE '<input type=\"checkbox\" onclick=\"carregaConteudoObraTabela(this,' || obrid || ')\" name=\"obrid[' || obrid || ']\" id=\"obrid_' || obrid || '\" />'
					 END) 
					 || '<span style=\"\display:none\" id=\"documento_hidden_' || obrid || '\" >' || documento || '</span>
					 	 <span style=\"\display:none\" id=\"foto_hidden_' || obrid || '\" >' || foto || '</span>
					 	 <span style=\"\display:none\" id=\"restricao_hidden_' || obrid || '\" >' || restricao || '</span>
					 	 <span style=\"\display:none\" id=\"pi_hidden_' || obrid || '\" >' || pi || '</span>' as acao," 
			: 
				"documento,foto,restricao,pi,")."
		    obrid,
		    nome_obra,
		    municipio,
		    situacao,
		    inicio,
		    final,
		    tipoobra,   		    
		    CASE WHEN codigo_situacao NOT IN (2,3,4,5) THEN
			(CASE WHEN ultimadata > (CURRENT_DATE - integer '30') THEN '<label style=\"color:#00AA00;\">' ||  to_char(ultimadata, 'DD/MM/YYYY') || '</label>'
			      WHEN (ultimadata <= (CURRENT_DATE - integer '30') AND ultimadata >= (CURRENT_DATE - integer '45')) THEN '<label style=\"color:#BB9900;\">' || to_char(ultimadata, 'DD/MM/YYYY') || '</label>'
			 ELSE '<label style=\"color:#DD0000;\">' || to_char(ultimadata, 'DD/MM/YYYY') || '</label>' END)
			 WHEN codigo_situacao = 2 THEN '<label style=\"color:#DD0000;\">' || to_char(ultimadata, 'DD/MM/YYYY') || '</label>'
			 WHEN codigo_situacao = 3 THEN '<label style=\"color:blue;\">' || to_char(ultimadata, 'DD/MM/YYYY') || '</label>'
		    ELSE to_char(ultimadata, 'DD/MM/YYYY') END as ultimadata,
		    percentual 
		FROM (
		    SELECT DISTINCT
		        '<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"javascript:Atualizar(\'?modulo=principal\/\cadastro&acao=A\',' || oi.obrid || ');\">' || 
			". $stBotaoExcluir ." || '</center>'   as acao,
		        CASE WHEN aa2.obrid is not null THEN '<img src=\"/imagens/anexo.gif\" border=0 title=\"Ver documentos\" style=\"cursor:pointer;\" onclick=\"javascript:Atualizar(\'?modulo=principal\/\documentos&acao=A\',' || oi.obrid || ');\">' ELSE '' END as documento,
		        CASE WHEN aa.obrid is not null THEN '<img src=\"/imagens/cam_foto.gif\" border=0 title=\"Galeria de fotos\" style=\"cursor:pointer;\" onclick=\"javascript:Atualizar(\'?modulo=principal\/\album&acao=A\',' || oi.obrid || ');\">' ELSE '' END as foto,
		        CASE WHEN r.obrid is null THEN '' ELSE '<img src=\"/imagens/restricao.png\" border=0 title=\"Restriï¿½ï¿½o\" style=\"cursor:pointer;\" onclick=\"javascript:Atualizar(\'?modulo=principal/restricao&acao=A\',' || oi.obrid || ');\">' END as restricao,
				CASE WHEN o.obrid is not null THEN '<img src=\"/imagens/money.gif\" border=0 title=\"Plano interno\" style=\"cursor:pointer;\" onclick=\"javascript:Atualizar(\'?modulo=principal\/\cadastro_pi&acao=A\',' || oi.obrid || ');\">' ELSE '' END as pi,
				oi.obrid,
				oi.obridrelacionada,
				".(!$popup ? "CASE WHEN fr.covnumero is not null 
								THEN '<a style=\"margin: 0 -20px 0 20px; text-transform:capitalize;\" href=\"#\" onclick=\"javascript:Atualizar(\'?modulo=principal\/\cadastro&acao=A\',' || oi.obrid || ');\">' || fr.covnumero || ' - ' || UPPER( oi.obrdesc ) || '</a>'	        
		        				ELSE '<a style=\"margin: 0 -20px 0 20px; text-transform:capitalize;\" href=\"#\" onclick=\"javascript:Atualizar(\'?modulo=principal\/\cadastro&acao=A\',' || oi.obrid || ');\">' || UPPER( oi.obrdesc ) || '</a>' 
		        			END as nome_obra," : 
		        			"CASE WHEN fr.covnumero is not null 
		        				THEN fr.covnumero || ' - ' || UPPER( oi.obrdesc )	        
		        				ELSE UPPER( oi.obrdesc )
		        			END as nome_obra,")."
		        upper(et.entnome) as descricao,
		        mun.mundescricao || '/' || ed.estuf as municipio,
		        to_char(oi.obrdtinicio,'DD/MM/YYYY') as inicio,
		        to_char(oi.obrdttermino,'DD/MM/YYYY') as final,
		        CASE WHEN oi.tobraid is null THEN 'Nï¿½o informado' ELSE tp.tobadesc END as tipoobra,
		        sto.stodesc as situacao,
		        oi.stoid as codigo_situacao,
		        CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END as ultimadata,
		        -- (SELECT replace(coalesce(round(SUM(icopercexecutado), 2), '0') || ' %', '.', ',') as total FROM obras.itenscomposicaoobra WHERE obrid = oi.obrid) as percentual
		        oi.obrpercexec|| ' %' as percentual
		    FROM
		        obras.obrainfraestrutura oi
		    LEFT JOIN
		    	obras.tipoobra tp ON tp.tobaid = oi.tobraid
		    INNER JOIN
		        entidade.entidade et ON oi.entidunidade = et.entid
		    LEFT JOIN
		        obras.situacaoobra sto ON oi.stoid = sto.stoid
		    INNER JOIN
		        entidade.endereco ed ON ed.endid = oi.endid
		    LEFT JOIN
		        territorios.municipio mun ON mun.muncod = ed.muncod
		    INNER JOIN
		        obras.orgao org ON oi.orgid = org.orgid
		    LEFT JOIN (
		        SELECT fri.obrid, covi.covid, covi.covnumero
		        FROM obras.formarepasserecursos fri
		        INNER JOIN obras.conveniosobra covi ON covi.covid = fri.covid  )fr ON fr.obrid = oi.obrid
		    LEFT JOIN
			    obras.arquivosobra aa ON aa.obrid = oi.obrid and aa.tpaid = 21
			LEFT JOIN
			    obras.arquivosobra aa2 ON aa2.obrid = oi.obrid and aa2.tpaid <> 21
			LEFT JOIN
				monitora.pi_obra o ON o.obrid = oi.obrid 
			LEFT JOIN
				monitora.pi_planointerno mpi ON mpi.pliid = o.pliid AND mpi.plistatus = 'A'			        
		    LEFT JOIN
		        public.arquivo pa ON pa.arqid = aa.arqid and aa.aqostatus = 'A'
		    LEFT JOIN
		        (SELECT distinct obrid, rststatus
		         FROM obras.restricaoobra
		         WHERE rststatus = 'A') r ON r.obrid = oi.obrid
		    {$joinObras}   
			" . $stFiltro . $filtroObra . "
		    GROUP BY
		        org.orgdesc, oi.obrid, oi.obridrelacionada, oi.obrdesc, oi.obrdtinicio, oi.obrdttermino,
		        oi.tobraid, tp.tobadesc, sto.stodesc, r.obrid, fr.covnumero, et.entnome,
		        mun.mundescricao, ed.estuf, aa.obrid, oi.obrpercexec,
		        oi.obsdtinclusao, oi.stoid, oi.obrdtvistoria, aa2.obrid, mpi.plistatus, o.obrid, mpi.plicod
		    ORDER BY
		        municipio) as foo ORDER BY ".($popup ? "acao," : "")." obrid";
	if($popup){
		$cabecalho = array("Ação", "ID", "Nome da Obra", "Município/UF", "Situação da Obra", "Data de Início", "Data de Término", "Tipo de Obra", "Última Atualização", "(%) Executado" );
		$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%');
	}else{
		$cabecalho = array("A","F", "R", "PI", "ID", "Nome da Obra", "Município/UF", "Situação da Obra", "Data de Início", "Data de Término", "Tipo de Obra", "Última Atualização", "(%) Executado" );
		
		$dados = $db->carregar($sql);
		echo '<table id="tbl_obras_relacionadas" width="100%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem" style="color: rgb(51, 51, 51);">';
		if(is_array($cabecalho))
			{
				echo '<thead><tr>';
				for ($i=0;$i<count($cabecalho);$i++)
				{
					echo '<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">'.$cabecalho[$i].'</td>';
				}
				echo '</tr> </thead>';
			}
		$num = 0;
		$dados = !$dados ? array() : $dados;
		foreach ($dados as $dado){
			$num++;
			$cor = $num%2 ? '' : '#F7F7F7';
			
			echo '<tr id="obrid_'.$dado['obrid'].'" bgcolor="'.$cor.'" >';
			
			foreach($dado as $k => $d){
				echo '<td valign="top" >'.($k == "obrid" ? " <input type='hidden' name='obrid[{$dado[$k]}]' value='{$dado[$k]}' /> " : "").$dado[$k].'</td>';
			}
			echo '</tr>';
		}
		echo '</table>';
		
		
	}
	
	
}

function salvaObrasRelacionadas($obridRel = null,$arrObrid = array()){
	global $db;
	
	$arrObrid = !$arrObrid ? array() : $arrObrid;
	
	if(!$obridRel){
		echo "<center><font style=\"color:#FF0000\" >Não foi possível relacionar as obras.</font></center>";
		return false;
	}
	
	foreach($arrObrid as $obrid => $dados){
		$arrObras[] = $obrid;
	}
	
	$sql = "update obras.obrainfraestrutura set obridrelacionada = null where obridrelacionada = $obridRel;";
	if($arrObras)
		$sql.= "update obras.obrainfraestrutura set obridrelacionada = $obridRel where obrid in (".implode(",",$arrObras).");";
	$db->executar($sql);
	$db->commit($sql);
}

/**
 * Monta e retornar o filtro de obras.
 * funï¿½ï¿½o criada para unificar e facilitar as modificaï¿½ï¿½es de critï¿½rios de filtros
 * @return string critï¿½rio de pesquisa do filtro/formulï¿½rio de obras
 * @author Cristiano Teles
 * @since 06/11/2008
 */
function retornarFiltroPesquisa(){
	
	$possui_traco 	   = strpos('_', $_REQUEST['carga']);
	$_REQUEST['carga'] = $possui_traco ? substr($_REQUEST['carga'], $possui_traco) : $_REQUEST['carga'];
	
	$stFiltro = " oi.obsstatus = 'A' ";
	$stFiltro .= !empty( $_REQUEST["orgid"] )    ? ' AND oi.orgid' . " 	 = ".$_REQUEST["orgid"] 	   : '';
	$stFiltro .= !empty( $_REQUEST["org"] ) 	 ? ' AND oi.orgid' . " 	 = ".$_REQUEST["org"] 		   : '';
	$stFiltro .= !empty( $_REQUEST["stoid"] ) 	 ? ' AND sto.stoid' . "  = ".$_REQUEST["stoid"] 	   : '';
	$stFiltro .= !empty( $_REQUEST["tobaid"] ) 	 ? ' AND oi.tobraid' . " = ".$_REQUEST["tobaid"] 	   : '';
	$stFiltro .= !empty( $_REQUEST["cloid"] ) 	 ? ' AND oi.cloid' . "   = ".$_REQUEST["cloid"] 	   : '';
	$stFiltro .= !empty( $_REQUEST["prfid"] ) 	 ? ' AND oi.prfid' . "   = ".$_REQUEST["prfid"] 	   : '';
	$stFiltro .= !empty( $_REQUEST["entid"] )    ? ' AND et.entid' . " 	 = ".$_REQUEST["entid"]. " "   : '';
	$stFiltro .= !empty( $_REQUEST["estuf"] ) 	 ? " AND ed.estuf 		 = '{$_REQUEST['estuf']}'" 	   : '';
	$stFiltro .= !empty( $_REQUEST["convenio"] ) ? " AND frrconvnum 	 = '{$_REQUEST["convenio"]}' " : ''; 
	$stFiltro .= !empty( $_REQUEST["carga"] ) 	 ? " AND oi.entidunidade = " . $_REQUEST['carga'] 	   : '';
	$stFiltro .= !empty( $_REQUEST["cargaCampus"] ) ? " AND oi.entidcampus = " . $_REQUEST['cargaCampus'] : '';
	$stFiltro .= !empty( $_REQUEST["naoCargaCampus"] ) ? " AND oi.entidcampus is null " : '';
	
	
	if( $_REQUEST["percentualinicial"] >= '0' && ( $_REQUEST["percentualfinal"] > '0' && $_REQUEST["percentualfinal"] < '100' ) ){
		$stFiltroPerc = " AND round(coalesce(obrpercexec,0),2) BETWEEN " . $_REQUEST["percentualinicial"] . ' AND ' .  $_REQUEST["percentualfinal"] . " ";
	}else if( $_REQUEST["percentualinicial"] == '0' && $_REQUEST["percentualfinal"] == '0' ){
		$stFiltroPerc = " AND (SELECT round(SUM(coalesce(icopercexecutado,0)), 0) FROM obras.itenscomposicaoobra WHERE obrid = oi.obrid) = 0";		
	}
	
	
	$stFiltro .= $stFiltroPerc;
	
	if ( isset( $_REQUEST["obrdesc"] ) && strlen( $_REQUEST["obrdesc"] ) > 1 ){
	
		$stFiltro .= " AND ( UPPER(oi.obrdesc) LIKE UPPER('%".$_REQUEST["obrdesc"]."%') ";
		$stFiltro .= " OR UPPER(fr.covnumero)  LIKE UPPER('%".$_REQUEST["obrdesc"]."%') ";
		#$stFiltro .= " )";
		$stFiltro .= " OR UPPER(et.entnome)       LIKE UPPER('%" .  $_REQUEST["obrdesc"]  . "%')";
		$stFiltro .= " OR UPPER(mun.mundescricao) LIKE UPPER('%".$_REQUEST["obrdesc"]."%'))";
		$stFiltro .= " OR UPPER(mpi.plicod)       LIKE UPPER('%" .  $_REQUEST["obrdesc"]  . "%')";
		
	}
	
	// Filtro de foto cadastrada na obra
	switch ( $_REQUEST["foto"] ) {
		case 'sim' : $stFiltro .= " and aa.obrid is not null and aa.aqostatus = 'A' "; break;
		case 'nao' : $stFiltro .= " and aa.obrid is null  "; break;
	}

	// Filtro de vistoria
	switch ( $_REQUEST["vistoria"] ) {
		case 'sim' : $stFiltro .= " and oi.obrdtvistoria is not null "; break;
		case 'nao' : $stFiltro .= " and oi.obrdtvistoria is null "; break;
		//default    : $stFiltro .= " and ( ( s.obrid is null ) OR ( s.obrid is not null AND s.supstatus <> 'I' ) ) "; break;
	}
	
	// Filtro de PI
	switch ( $_REQUEST["planointerno"] ) {
		case 'sim' : $stFiltro .= " and o.obrid is not null AND mpi.plistatus = 'A' "; break;
		case 'nao' : $stFiltro .= " and o.obrid is null  "; break;
	}
	
	// Possui Aditivo
	switch ( $_REQUEST["aditivo"] ) {
		case 'sim' : $stFiltro .= " and oi.obridaditivo is not null "; break;
		case 'nao' : $stFiltro .= " and oi.obridaditivo is null  "; break;
	}
	
	return $stFiltro;
}

/**
 * Monta e retornar o SQL de pesquisa de obras, quando agrupado por obras
 * funï¿½ï¿½o criada para unificar e facilitar as modificaï¿½ï¿½es de no sql para montar a lista de obras
 * @return string critï¿½rio de pesquisa do filtro/formulï¿½rio de obras
 * @author Cristiano Teles
 * @since 06/11/2008
 */
function retornarSQLPesquisa(){
	
	global $db, $habilitado;
	
	$res = obras_pegarOrgaoPermitido();
	
	//$stBotaoExcluir = "'<img src=\"/imagens/excluir_01.gif\" border=0 title=\"Excluir\"> '";
	$stBotaoExcluir = "' '";
	if( $habilitado && $_SESSION['pesquisaObra']["org"] != ORGAO_FNDE || $db->testa_superuser() ){
		$stBotaoExcluirHabilitado = '<img src="/imagens/excluir.gif" border=0 title="Excluir" style="cursor:pointer;" onclick="javascript:Excluir(\\\'?modulo=inicio&acao=A&requisicao=excluir\\\', \' || oi.obrid || \');">';
		$stBotaoExcluir = "	case when oi.obrdtvistoria is not null then " . $stBotaoExcluir . " else '" . $stBotaoExcluirHabilitado . "' end ";
	}
	
	$stFiltro = retornarFiltroPesquisa();
	$stFiltro = !empty( $stFiltro ) ? ' WHERE ' . $stFiltro : $stFiltro; 
	
	// Verifica as responsabilidades do usuï¿½rio
	if( !($db->testa_superuser()) && ( !possuiPerfil( PERFIL_CONSULTAGERAL) && 
									   !possuiPerfil( PERFIL_GESTORMEC ) ) ){
	
		// Pega os perfis do usuï¿½rio
		$perfis = obras_arrayPerfil();
		
		$filtroObra = $_REQUEST['org'] == ORGAO_FNDE ? "OR ( ur.obrid = oi.obrid )" : "";
		
		$argumento = "
					INNER JOIN
						obras.usuarioresponsabilidade ur ON ur.rpustatus = 'A' AND
						ur.pflcod in (" . implode("," , $perfis) . ") AND
						( ur.entid = oi.entidunidade OR
						( ur.estuf = ed.estuf AND
						  ur.orgid = org.orgid AND
						  ur.estuf IS NOT NULL ) OR
						( ur.orgid = org.orgid AND
						  ur.estuf IS NULL ) {$filtroObra} )"; 
		$stFiltro .= " AND ur.usucpf = '{$_SESSION["usucpf"]}' ";
	
	}else{
		$stFiltro .= " AND oi.orgid = " . ($_REQUEST['org'] ? $_REQUEST['org'] : $res[0]['id']);	
	}
	
	
	$stSql = "SELECT DISTINCT
			    acao,
			    documento,
			    foto,
			    restricao,
			    pi,
			    aditivo,
			    id,
			    nome_obra,
			    descricao,
			    municipio,
			    inicio,
			    final,
			    situacao,
			    CASE WHEN codigo_situacao NOT IN (3,4,5,6,99) THEN
				(CASE WHEN ultimadata > (CURRENT_DATE - integer '30') THEN '<label style=\"color:#00AA00;\">' ||  to_char(ultimadata, 'DD/MM/YYYY') || '</label>'
				      WHEN (ultimadata <= (CURRENT_DATE - integer '30') AND ultimadata >= (CURRENT_DATE - integer '45')) THEN '<label style=\"color:#BB9900;\">' || to_char(ultimadata, 'DD/MM/YYYY') || '</label>'
				 ELSE '<label style=\"color:#DD0000;\">' || to_char(ultimadata, 'DD/MM/YYYY') || '</label>' END)
				 WHEN codigo_situacao = 2 THEN '<label style=\"color:#DD0000;\">' || to_char(ultimadata, 'DD/MM/YYYY') || '</label>'
				 WHEN codigo_situacao = 3 THEN '<label style=\"color:blue;\">' || to_char(ultimadata, 'DD/MM/YYYY') || '</label>'
			    ELSE to_char(ultimadata, 'DD/MM/YYYY') END as ultimadata,
			    percentual,
			    id
			FROM (
			    SELECT DISTINCT
			        '<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"javascript:Atualizar(\'?modulo=principal\/\cadastro&acao=A\',' || oi.obrid || ');\">' || 
					". $stBotaoExcluir ." || '</center>'   as acao,
			        CASE WHEN aa2.obrid is not null THEN '<img src=\"/imagens/anexo.gif\" border=0 title=\"Ver documentos\" style=\"cursor:pointer;\" onclick=\"javascript:Atualizar(\'?modulo=principal\/\documentos&acao=A\',' || oi.obrid || ');\">' ELSE '' END as documento,
			        CASE WHEN aa.obrid is not null THEN '<img src=\"/imagens/cam_foto.gif\" border=0 title=\"Galeria de fotos\" style=\"cursor:pointer;\" onclick=\"javascript:Atualizar(\'?modulo=principal\/\album&acao=A\',' || oi.obrid || ');\">' ELSE '' END as foto,
			        CASE WHEN r.obrid is null THEN '' ELSE '<img src=\"/imagens/restricao.png\" border=0 title=\"Restriï¿½ï¿½o\" style=\"cursor:pointer;\" onclick=\"javascript:Atualizar(\'?modulo=principal/restricao&acao=A\',' || oi.obrid || ');\">' END as restricao,
			        CASE WHEN o.obrid is not null THEN '<img src=\"/imagens/money.gif\" border=0 title=\"Plano interno\" style=\"cursor:pointer;\" onclick=\"javascript:Atualizar(\'?modulo=principal\/\cadastro_pi&acao=A\',' || oi.obrid || ');\">' ELSE '' END as pi,
			        CASE WHEN obridaditivo is not null THEN '<img src=\"/imagens/check_p.gif\" border=0 title=\"Esta obra ï¿½ um aditivo\" style=\"cursor:pointer;\" onclick=\"javascript:Atualizar(\'?modulo=principal/cadastroAditivo&acao=A\',' || oi.obrid || ');\">' ELSE '' END as aditivo,
			        oi.obrid as id,
			        CASE WHEN fr.covnumero is not null THEN '' || fr.covnumero || ' - ' || UPPER( oi.obrdesc ) || '' ELSE '' || UPPER( oi.obrdesc ) || '' END as nome_obra,
			        upper(et.entnome) as descricao,
			        mun.mundescricao || '/' || ed.estuf as municipio,
			        to_char(oi.obrdtinicio,'DD/MM/YYYY') as inicio,
			        to_char(oi.obrdttermino,'DD/MM/YYYY') as final,
			        sto.stodesc as situacao,
			        oi.stoid as codigo_situacao,
			        CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END as ultimadata,
			        (SELECT replace(coalesce(round(SUM(icopercexecutado), 2), '0') || ' %', '.', ',') as total FROM obras.itenscomposicaoobra WHERE obrid = oi.obrid) as percentual
			    FROM
			        obras.obrainfraestrutura oi
			    INNER JOIN
			        entidade.entidade et ON oi.entidunidade = et.entid
			    LEFT JOIN
			        obras.situacaoobra sto ON oi.stoid = sto.stoid
			    INNER JOIN
			        entidade.endereco ed ON ed.endid = oi.endid
			    LEFT JOIN
			        territorios.municipio mun ON mun.muncod = ed.muncod
			    INNER JOIN
			        obras.orgao org ON oi.orgid = org.orgid
			    LEFT JOIN (
			        SELECT fri.obrid, covi.covid, covi.covnumero
			        FROM obras.formarepasserecursos fri
			        INNER JOIN obras.conveniosobra covi ON covi.covid = fri.covid  )fr ON fr.obrid = oi.obrid
			    LEFT JOIN
			        obras.arquivosobra aa ON aa.obrid = oi.obrid and aa.tpaid = 21
			    LEFT JOIN
			        obras.arquivosobra aa2 ON aa2.obrid = oi.obrid and aa2.tpaid <> 21
			    LEFT JOIN
					monitora.pi_obra o ON o.obrid = oi.obrid 
				LEFT JOIN
					monitora.pi_planointerno mpi ON mpi.pliid = o.pliid AND mpi.plistatus = 'A'
			    LEFT JOIN
			        public.arquivo pa ON pa.arqid = aa.arqid and aa.aqostatus = 'A'
			    LEFT JOIN
			        (SELECT distinct obrid, rststatus
			         FROM obras.restricaoobra
			         WHERE rststatus = 'A') r ON r.obrid = oi.obrid
				" . $argumento . $stFiltro . "
			    GROUP BY
			        org.orgdesc, oi.obrid, oi.obrdesc, oi.obrdtinicio, oi.obrdttermino,
			        sto.stodesc, r.obrid, fr.covnumero, oi.obridaditivo, et.entnome,
			        mun.mundescricao, ed.estuf, aa.obrid, oi.obrpercexec,
			        oi.obsdtinclusao, oi.stoid, oi.obrdtvistoria, aa2.obrid, mpi.plistatus, o.obrid, mpi.plicod
			    ORDER BY
			        municipio) as foo";
	
	return $stSql;
	
}

/**
 * Funï¿½ï¿½o que verifica se existe obra com o obrid informado
 * 
 */
function obras_verificaobras($obrid){
	
	global $db;
	
	$obra = $db->pegaLinha("SELECT 
								* 
							FROM 
								obras.obrainfraestrutura 
							WHERE 
								obrid = {$obrid} AND
								obsstatus = 'A'");

	return $obra;
	
}

/**
 * Funï¿½ï¿½o que verifica se o usuï¿½rio possui permissï¿½o na obra informada
 * 
 */
function obras_verificapermissao($obrid){
	
	global $db;
	
	if ( possuiPerfil( Array(PERFIL_CONSULTAGERAL,PERFIL_ADMINISTRADOR,PERFIL_GESTORMEC,PERFIL_SAMPR ) ) ) {
		
		return true;
		
	}else{
		
		$perfis = obras_arrayPerfil();
	
		$argumento = "
				INNER JOIN
					obras.usuarioresponsabilidade ur ON ur.rpustatus = 'A' AND
					ur.pflcod in (" . implode("," , $perfis) . ") AND
					( ur.entid = oi.entidunidade OR
					( ur.estuf = ed.estuf AND
					  ur.orgid = oi.orgid AND
					  ur.estuf IS NOT NULL ) OR
					( ur.orgid = oi.orgid AND
					  ur.estuf IS NULL ) OR
					( ur.obrid = oi.obrid AND ur.pflcod in (163,426) ) )";
		
		$obra = $db->pegaUm("SELECT 
								oi.obrid
							FROM
								obras.obrainfraestrutura oi
							INNER JOIN 
								entidade.entidade et ON oi.entidunidade = et.entid
							INNER JOIN
								entidade.endereco ed ON oi.endid = ed.endid 
							" . $argumento . "
							WHERE
								oi.obrid = {$obrid} AND
								ur.usucpf = '{$_SESSION["usucpf"]}' AND
								ur.rpustatus = 'A' AND
								oi.obsstatus = 'A'");
		
		return $obra;
			
	}
	
}

function obras_buscaconvenio($convenio){
	
	global $db;
	
	$dados = $db->pegaLinha("SELECT 
								* 
							FROM 
								obras.conveniosobra 
							WHERE 
								covid = '{$convenio}'");
	
	$dados["covdtinicio"] = formata_data($dados["covdtinicio"]);
	$dados["covdtfinal"]  = formata_data($dados["covdtfinal"]);
	$dados["covobjeto"] 	  = iconv("ISO-8859-1", "UTF-8", $dados["covobjeto"]);
	$dados["covdetalhamento"] = iconv("ISO-8859-1", "UTF-8", $dados["covdetalhamento"]);  
	
	echo json_encode($dados);

}

function carregaTipologiaClass( $cloid ){
	global $db;
	
//	if ( !empty( $cloid ) ){
//		$sql = "
//				SELECT 
//					tpoid as codigo,
//					tpodsc as descricao, tpodetalhe
//				FROM
//					obras.tipologiaobra				
//				WHERE cloid = " . $cloid ."
//				ORDER BY
//					cloid";
//	}else{
//		unset( $sql );
//		$sql = array();
//	}
	$sql = array();
	$db->monta_combo("tpoid", $sql, 'N', utf8_encode("Para habilitar selecione uma subação..."), 'mostraDescricaoTipologia', '', '', '340', 'N', 'tpoid');
}

function carregaTipologiaProg( $prfid ){
	global $db;
	
//	if ( !empty( $prfid ) ){
//		$sql = "
//				SELECT 
//					t.tpoid as codigo,
//					t.tpodsc as descricao, tpodetalhe
//				FROM
//					obras.tipologiaobra t
//					INNER JOIN obras.programatipologia AS p ON p.tpoid = t.tpoid
//				WHERE p.prfid =  ".$prfid."
//				ORDER BY
//					prfid";
//	}else{
//		unset( $sql );
//		$sql = array();
//	}
	$sql = array();
	$db->monta_combo("tpoid", $sql, 'N', utf8_encode("Para habilitar selecione uma classificação da obra..."), 'mostraDescricaoTipologia', '', '', '340', 'N', 'tpoid');
}

function carregaTipologia( $cloid, $prfid){
	
	global $db;
	global $tpoid;

	// SQL que montarï¿½ a tela de opï¿½ï¿½es do campo
	$sql_opcao = sprintf("SELECT
							tpodsc AS codigo, 
							tpodetalhe AS descricao,
							t.tpoid AS value  
						  FROM
						    obras.tipologiaobra t
						  INNER JOIN 
						   	obras.programatipologia pt ON pt.tpoid = t.tpoid
						  WHERE
						  	pt.cloid = %d
						  	AND pt.prfid = %d"
						, $cloid
						, $prfid);	

	if ($tpoid){	
		$sql = sprintf("SELECT
							tpodsc || ' - ' || tpodetalhe AS descricao
						FROM
							obras.tipologiaobra
						WHERE
							tpoid = %d"
						, $tpoid);
		$desc = $db->pegaUm($sql);				
		
		$tpoid = array(
						"descricao" => $desc,
						"value" 	=> $tpoid
					   );						   
	}
	
	campo_popup(
                 "tpoid",
                 $sql_opcao,
                 "Selecione a Tipologia da Obra",
                 "mostraDescricaoTipologia",
                 "400x400",
                 "62",
                 "",
                 1,
                 true
               );          
              	
	
//	if ( !empty( $cloid ) && !empty( $prfid ) ){
//		$sql = "
//				SELECT 
//					t.tpoid as codigo,
//					t.tpodsc as descricao, tpodetalhe
//				FROM
//					obras.tipologiaobra t
//				INNER JOIN 
//					obras.programatipologia AS p ON p.tpoid = t.tpoid
//				WHERE 
//					p.cloid = ". $cloid ." 
//					AND p.prfid =  ".$prfid."
//				ORDER BY
//					prfid";
//	}else{
//		unset( $sql );
//		$sql = array();
//	}
//	
//	$db->monta_combo("tpoid", $sql, $somenteLeitura, "Selecione...", 'mostraDescricaoTipologia', '', '', '340', 'N', 'tpoid', false);
}

/**
 * Funï¿½ï¿½o que verifica quem pode alterar e incluir uma obra
 *
 * @author Fernando A. Bagno da Silva
 * @param integer $supvid
 * @return string
 */
function obras_podeatualizarvistoria($supvid){
	
	global $db;
	
	if( $db->testa_superuser() || possuiPerfil(PERFIL_ADMINISTRADOR) ){
		
		return 'S';
		
	}else{
		
		$responsavel = $db->pegaUm("SELECT 
										usucpf 
									FROM 
										obras.supervisao 
									WHERE
										supvid = {$supvid} AND 
										usucpf = '{$_SESSION["usucpf"]}'");

		$responsavel ? $retorno = 'S' : $retorno = 'N';
		
		return $retorno;
		
	}
	
}

/**
 * Funï¿½ï¿½o que lista os itens de vistoria, caso existam
 *
 * @param integer $supvid
 */
function obras_listaitensvistoria( $supvid ){
	
	global $db; 
	
	$obra = $_REQUEST["obrid"] ? $_REQUEST["obrid"] : $_SESSION["obra"]["obrid"];
	if ($supvid != 'NULL'){
		
		$somenteLeitura = obras_podeatualizarvistoria($supvid);
		
		$sql = "SELECT
					s.supvid
				FROM
					obras.supervisao s
				WHERE
					s.supdtinclusao = (select max(ss.supdtinclusao) from obras.supervisao ss where ss.obrid = ".$obra." AND supstatus = 'A')";	
		
		if($db->pegaUm($sql) != $supvid){
			$somenteLeitura = "N";
		}
		
		$vigente = verificaItenVigente($obra, $supvid);
			
	}
		
	$qtdSupervisao  = 0;

	if( $supvid != 'NULL' && $vigente == '') {
		// Busca os itens cadastrados naquela obra.
		$sql = "
			SELECT
				itco.icoid,
				itc.itcdesc,
				itco.icovlritem,
				itco.icopercsobreobra, 
				itco.icodtinicioitem,
				itco.icodterminoitem,
				itco.icopercprojperiodo,
				itco.icopercexecutado,
				sup.supvlrinfsupervisor,
				sup.supvlritemexecanterior,
				sup.supvlritemsobreobraexecanterior,
				sup.supvid
			FROM 
				obras.itenscomposicao itc
			INNER JOIN	
				obras.itenscomposicaoobra itco ON itco.itcid = itc.itcid
			INNER JOIN
				obras.supervisaoitenscomposicao sup ON sup.icoid = itco.icoid AND
													   sup.supvid = " . $supvid . "
			WHERE								
				itc.itcid = itco.itcid AND
				itco.obrid = " . $obra . "
				AND itco.icostatus = 'A' --para nï¿½o exibir itens duplicados!
				--AND itco.icovigente = 'A'
			ORDER BY icoordem";
	
		
	}else{
		$traid = pegaObUltimoAditivo('traid', NULL, $obra);
		$whereAditivo = ($traid ? " AND itco.traid = $traid " : "");

		// Busca o nï¿½mero de supervisï¿½es existentes para a obra selecionada.
		$query = "SELECT
				  	count(*) 
				  FROM 
					obras.supervisao s 
				  WHERE 
					s.obrid = ".$obra." AND
					s.supstatus = 'A' ";
		
		$qtdSupervisao = (int)( $db->pegaUm($query) );
		
		// Se existirem supervisï¿½es cadastradas, busca os valores 
		// da ï¿½ltima realizada.
		if($qtdSupervisao == 0) {
			
			$sql = "SELECT
						itco.icoid,
						itc.itcdesc,
						itco.icovlritem,
						itco.icopercsobreobra, 
						itco.icodtinicioitem,
						itco.icodterminoitem
					FROM 
						obras.itenscomposicao itc
					INNER JOIN 
						obras.itenscomposicaoobra itco ON itc.itcid = itco.itcid
					WHERE
						itco.obrid = ".$obra . "
						AND itco.icostatus = 'A' --para nï¿½o exibir itens duplicados!
						$whereAditivo
					ORDER BY 
						itco.icoordem";
		
		}else {
			
			if($supvid == 'NULL' ){
				//Condiï¿½ï¿½o para inserï¿½ï¿½o de uma nova Vistoria.
				$supervisao =  " s.supdtinclusao = ( SELECT 
					    								max(ss.supdtinclusao) 
									    			 FROM 
									   					obras.supervisao ss 
									    			 WHERE 
									   					ss.obrid = itco.obrid AND 
									   					ss.supstatus = 'A' 
									   				)";
			}else{
				//Condiï¿½ï¿½o que exibe os Dados da Vistoria, jï¿½ existente.
				 $supervisao = " s.supvid = '".$supvid."'";
			} 
			
			$sql = "SELECT
						itco.icoid,
						itc.itcdesc,
						itco.icovlritem,
						itco.icopercsobreobra, 
						itco.icodtinicioitem,
						itco.icodterminoitem,
						itco.icopercexecutado,
						COALESCE(sup.supvlrinfsupervisor, sup2.supvlrinfsupervisor) AS supvlrinfsupervisor,
						sup.supvlritemexecanterior,
						sup.supvlritemsobreobraexecanterior,
						sup.supvid								
					FROM 
						obras.itenscomposicao itc
					INNER JOIN 
						obras.itenscomposicaoobra itco ON itc.itcid = itco.itcid
														  AND itco.icovigente = 'A'
					INNER JOIN
						obras.supervisao s ON s.obrid = itco.obrid AND  ".$supervisao."     -- Filtro inativado dia 16/12/2010 as 11:14 H.
																							-- s.supdtinclusao = ( SELECT 
																				    		--							max(ss.supdtinclusao) 
																							--			   			FROM 
																							--			   				obras.supervisao ss 
																							--			   			WHERE 
																							--			   				ss.obrid = itco.obrid AND 
																							--			   				ss.supstatus = 'A' )
					LEFT JOIN
						obras.supervisaoitenscomposicao sup ON sup.supvid = s.supvid AND sup.icoid = itco.icoid
					LEFT JOIN (
								SELECT
									itcid,
									sic.supvlrinfsupervisor
								FROM
									obras.supervisaoitenscomposicao sic
								JOIN obras.itenscomposicaoobra ico ON ico.icoid = sic.icoid
																	  AND ico.obrid = ".$obra."	
								WHERE
									supvid = (SELECT MAX(supvid) FROM obras.supervisao WHERE supstatus = 'A' AND obrid = ".$obra.")
						) sup2 ON sup2.itcid = itco.itcid 
					WHERE
						itco.obrid = ".$obra."
						AND itco.icostatus = 'A' --para nï¿½o exibir itens duplicados!
						$whereAditivo
					GROUP BY
						itco.icoordem,
						itco.icoid,
						itc.itcdesc,
						itco.icovlritem,
						itco.icopercsobreobra, 
						itco.icodtinicioitem,
						itco.icodterminoitem,
						itco.icopercexecutado,
						sup.supvlrinfsupervisor,
						sup2.supvlrinfsupervisor,
						sup.supvlritemexecanterior,
						sup.supvlritemsobreobraexecanterior,
						sup.supvid
					ORDER BY 
						itco.icoordem";
					
		}
	}
//	dbg($sql);
	// Cria o campo com o nï¿½mero de supervisï¿½es daquela obra.
	echo "<input type='hidden' name='qtdsupervisao' id='qtdsupervisao' value='" . $qtdSupervisao . "' />";
	
	// Executa o SQL dos itens buscados.
	$itens = ( $db->carregar($sql) );

	// Cria o array a ser utilizado na aplicaï¿½ï¿½o.
	$dados = array();
	
	// Se existirem valores no SQL executado.
	if(is_array($itens)) {
		
		$total_visto = 0;
		$total_execsobreobra = 0;
		
		foreach($itens as $i => $linha) {
			
			// Pega os valores reais
			$supervisao                 = ( isset($linha["supvlrinfsupervisor"]) ) 			   ? $linha['supvlrinfsupervisor']			   : 0;
			$exec_anterior              = ( isset($linha["supvlritemexecanterior"]) ) 		   ? $linha['supvlritemexecanterior']		   : 0;
			$exec_anterior_sobre_obra   = ( isset($linha["supvlritemsobreobraexecanterior"]) ) ? $linha['supvlritemsobreobraexecanterior'] : 0;
			$perc_sobre_obra            = ( isset($linha["icopercsobreobra"]) ) 			   ? $linha['icopercsobreobra']				   : 0;
			
			// Valores do % do item executado sobre a obra
			$supervisao_exec_sobre_obra = ( ((float)$supervisao * (float)$perc_sobre_obra) / 100 );
			$supervisao_exec_sobre_obra2 = number_format($supervisao_exec_sobre_obra, 2, ',', '.');
			
			// Nome do campo % da Supervisï¿½o.
			$vname = 'supvlrinfsuperivisor_'.$linha['icoid'];
			
			// Atribui os valores dos campos de % da Supervisï¿½o
			global $$vname;
			$$vname = !isset($supvid) ? $exec_anterior : $supervisao;

			// Realiza o total do % executado sobre a a obra
			$total_execsobreobra = $total_execsobreobra + (float)$supervisao_exec_sobre_obra;
			
			// Se nï¿½o existir supervisï¿½o, cria os campos com valores nulos.
			if( $supvid == 'NULL' ) {								
				
				$exec_anterior 			  = $supervisao;
				$exec_anterior_sobre_obra = $supervisao_exec_sobre_obra;
				$inputExec 				  = "<input type='text' 
											  style='border: 0px;' 
									  		  name='percexec_".$linha['icoid']."' 
											  id='percexec_".$linha['icoid']."' 
											  size='6' 
											  value='' 
											  readonly/>";
				$inputExecSobreObra 	  = "<input 
											  type='text' 
											  style='border: 0px; text-align: right;' 
											  name='percexecsobreobra_".$linha['icoid']."' 
											  id='percexecsobreobra_".$linha['icoid']."' 
											  value='" . $supervisao_exec_sobre_obra2 . "' 
											  size='6' 
											  readonly/>";
				
			}else { 

				// Se existir supervisï¿½o, cria os campos com os valores cadastrados.
			    $inputExec 			= "<input 
									    type='text' 
									    style='border: 0px;' 
									    name='percexec_" . $linha['icoid'] . "' 
									    id='percexec_" . $linha['icoid'] . "' 
									    size='6' 
									    value='" . $supervisao . "' 
									    readonly/>";
				$inputExecSobreObra = "<input 
										type='text' 
										style='border: 0px; text-align: right;' 
										name='percexecsobreobra_" . $linha['icoid'] . "' 
										id='percexecsobreobra_" . $linha['icoid'] . "' 
										value='" . $supervisao_exec_sobre_obra2 . "' 
										size='6' 
										readonly/>";
				
			}
			
			// Armazena os valores antigos das supervisï¿½es.
			echo '<script> valor_antigo['.$linha['icoid'].'] = Number('. $$vname .'); </script>';
			
			$$vname = number_format($$vname, 2, ',', '.');
			
			// Cria o array com os campos e valores que irï¿½o compor a lista.
			$dados[] = array(
							 $linha['itcdesc'] . "<input type='hidden' name='item[]' value='" . $linha['icoid'] . "' />",
							 $linha['icovlritem'],
							 $perc_sobre_obra,
							 formata_data($linha['icodtinicioitem']),
							 formata_data($linha['icodterminoitem']),
							 $exec_anterior,
							 $exec_anterior_sobre_obra,
							 campo_texto($vname, 'N', $somenteLeitura, '', 10, 6, '###,##', '', 'right', '', 0, 'id="supvlrinfsuperivisor_'.$linha['icoid'].'"', 'alteraValor('.$linha['icoid'].', \''.$perc_sobre_obra.'\', \''.$supervisao.'\'); obras_calculaTotalVistoria();', null, 'obras_verificaPercentual('.$linha['icoid'].'); alteraValor('.$linha['icoid'].', \''.$perc_sobre_obra.'\', \''.$supervisao.'\'); obras_calculaTotalVistoria();'),							 
							 $inputExecSobreObra . "<input type='hidden' name='execanterior_".$linha['icoid']."' id='execanterior_".$linha['icoid']."' value='" . number_format($supervisao, 2, ',', '.') . "' />
							 						<input type='hidden' name='execanteriorsobreobra_".$linha['icoid']."' id='execanteriorsobreobra_".$linha['icoid']."' value='" . $supervisao_exec_sobre_obra . "' />
							 						<input type='hidden' name='percrealobra_".$linha['icoid']."' id='percrealobra_".$linha['icoid']."' value='" . $supervisao_exec_sobre_obra . "' />"
							);
			
			
		}
	}
	
	$count = count($dados);
		
	// Cria as linhas da lista de itens com os seus respectivos valores.
	for ($i = 0; $i < $count; $i++){
		
		// Realiza a soma dos valores da lista.
		$total_valor = $total_valor + (float)$dados[$i][1];
		$total_percs = $total_percs + (float)$dados[$i][2];
		$total_percp = $total_percp + (float)$dados[$i][5];
		$total_perce = $total_perce + (float)$dados[$i][6];
		
		// Cria as linhas da tabela com os valroes dos itens.
		echo '  <tr>
					<td> ' . $dados[$i][0] . ' </td>
					<td align="right"> ' . number_format($dados[$i][1], 2, ',', '.') . ' </td>
					<td align="right"> ' . number_format($dados[$i][2], 2, ',', '.') . ' </td>
					<td align="right"> ' . $dados[$i][3] . ' </td>
					<td align="right"> ' . $dados[$i][4] . ' </td>
					<td align="right"> ' . number_format($dados[$i][5], 2, ',', '.') . ' </td>
					<td align="right"> ' . number_format($dados[$i][6], 2, ',', '.') . ' </td>
					<td align="right"> ' . $dados[$i][7] . ' </td>
					<td align="right"> ' . $dados[$i][8] . ' </td>
				</tr>';
	
	}
	
	// cria a sessï¿½o com o total real de valor
	$_SESSION["obras"]["totalvalor"] = $total_valor;
	
	// Cria a linha de total de valores da lista de itens.
	$total_valor = number_format($total_valor, 2, ',', '.');
	
	$total_percs = round($total_percs);
	$total_percs = number_format($total_percs, 2, ',', '.');
	
	$total_perce = round($total_perce);
	$total_perce = number_format($total_perce, 2, ',', '.');
	
//	$total_execsobreobra = round($total_execsobreobra);
	$total_execsobreobra = number_format($total_execsobreobra, 2, ',', '.');
	
	echo '<tr style="background-color: #cccccc;" align="right">
			 <td valign="middle" align="center"><b>Total</b></td>
			 <td> ' . $total_valor . ' </td>
			 <td> ' . $total_percs . ' </td>
			 <td></td>
			 <td></td>
			 <td></td>
			 <td> ' . $total_perce . ' </td>
			 <td></td>
			 <td> 
			 	<span id="sobreobra">
			 		<input type="text" size="10" readonly="readonly" class="disabled" name="percsupatual" value="'.$total_execsobreobra.'" style="text-align:right;">
			 	</span>
			 </td>
		  </tr>';

/*echo "<span id='supatual'>
	<input type="text" name="percsupatual" value="'$total_execsobreobra'>
</span>";*/
}

/**
 * Funï¿½ao que monta o sql para trazer o relatï¿½rio geral de obras
 *
 * @author Fernando A. Bagno da Silva
 * @since 20/02/2009
 * @return string
 */
function obras_monta_sql_relatio(){
	
	$where = array();
	
	extract($_REQUEST);
	
	$selectTerritorios = "territorios.municipio ";
	
	if( in_array( "tipomun", $agrupador ) ){
		if ( $selectTerritorios == "territorios.municipio " ){
				
				$selectTerritorios = "(SELECT 
											tm.muncod, tm.mundescricao, gt.gtmid, gt.gtmdsc, tpm.tpmdsc
										FROM
											territorios.municipio tm 
										INNER JOIN
											territorios.muntipomunicipio mtm ON mtm.muncod = tm.muncod
										INNER JOIN
											territorios.tipomunicipio tpm ON tpm.tpmid = mtm.tpmid 
										INNER JOIN
											territorios.grupotipomunicipio gt ON gt.gtmid = tpm.gtmid 
										WHERE 
											tpm.gtmid = 5 AND gt.gtmid = 5 )";
				
			}
			
			$selectTipoMun = "CASE WHEN tm.gtmid  = 5 THEN tm.tpmdsc ELSE 'Outros' END as tipomun, ";
			$dadosTipoMun  = "tipomun,";
			$groupByTipoMun = "tm.tpmdsc,";
			if( !$groupByGtmid ){
				$groupByGtmid    = "tm.gtmid, ";	
			}
	}
	
	// Obras
	if( $_SESSION['obras']['obrid_mapa'] ){
		array_push($where, " oi.obrid in (" . implode( ',', $_SESSION['obras']['obrid_mapa'] ) . ") ");
	}
	
	// tipo de ensino
	if( $orgid ){
		array_push($where, " oi.orgid in (" . implode( ',', $orgid ) . ") ");
	}
	
	// regiï¿½o
	if( $regiao[0] && $regiao_campo_flag ){
		array_push($where, " re.regcod " . (!$regiao_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $regiao ) . "') ");
	}
	
	// mesoregiï¿½o
	if( $mesoregiao[0] && $mesoregiao_campo_flag ){
		array_push($where, " me.mescod " . (!$mesoregiao_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $mesoregiao ) . "') ");
	}
	
	// UF
	if( $uf[0] && $uf_campo_flag ){
		array_push($where, " ed.estuf " . (!$uf_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $uf ) . "') ");
	}
	
	// grupo municipio
	if( $grupomun[0]  && $grupomun_campo_flag ){
		
		$selectTerritorios = "(SELECT 
									tm.muncod, tm.mundescricao, gt.gtmid, gt.gtmdsc, tpm.tpmdsc
								FROM
									territorios.municipio tm 
								INNER JOIN
									territorios.muntipomunicipio mtm ON mtm.muncod = tm.muncod
								INNER JOIN
									territorios.tipomunicipio tpm ON tpm.tpmid = mtm.tpmid 
								INNER JOIN
									territorios.grupotipomunicipio gt ON gt.gtmid = tpm.gtmid 
								WHERE 
									tpm.gtmid = 5 AND gt.gtmid = 5 )";
		
		$selectGrupoMun  = "CASE WHEN tm.gtmid is not null THEN tm.gtmdsc ELSE 'Outros' END as grupomun, ";
		$dadosGrupoMun   = "grupomun, ";
		$groupByGrupoMun = "tm.gtmdsc, ";
		$groupByGtmid    = "tm.gtmid, ";
		array_push($where, " gt.gtmid " . (!$grupomun_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $grupomun ) . "') ");
	}
	
	// tipo municipio
	if( $tipomun[0]  && $tipomun_campo_flag ){
		array_push($where, " tpm.tpmid " . (!$tipomun_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $tipomun ) . "') ");
	}
	
	// municipio
	if( $municipio[0]  && $municipio_campo_flag ){
		array_push($where, " ed.muncod " . (!$municipio_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ");
	}
	
	// unidade
	if( $unidade[0] && $unidade_campo_flag ){
		array_push($where, " oi.entidunidade " . (!$unidade_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $unidade ) . ") ");
	}
	
	// entidcampus
	if( $entidcampus[0] && $entidcampus_campo_flag ){
		array_push($where, " oi.entidcampus " . (!$entidcampus_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $entidcampus ) . ") ");
	}
	
	// programa fonte
	if( $prfid[0] && $prfid_campo_flag ){
		if ( !$prfid_campo_excludente ){
			array_push($where, " oi.prfid  IN (" . implode( ',', $prfid ) . ") ");	
		}else{
			array_push($where, " ( oi.prfid  NOT IN (" . implode( ',', $prfid ) . ") OR oi.prfid is null ) ");
		}
		
	}
	
	// tipologia da obra
	if( $tpoid[0] && $tpoid_campo_flag ){
		array_push($where, " oi.tpoid " . (!$tpoid_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $tpoid ) . ") ");
	}
	
	// classificaï¿½ï¿½o da obra
	if( $cloid[0] && $cloid_campo_flag ){
		array_push($where, " oi.cloid " . (!$cloid_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $cloid ) . ") ");
	}
	
	// situaï¿½ï¿½o da obra
	if( $stoid[0] && $stoid_campo_flag ){
		array_push($where, " oi.stoid " . (!$stoid_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $stoid ) . ") ");
	}
	
	// percentual da obra
	if( $percentualinicial ){
		array_push($where, " oi.obrpercexec BETWEEN {$percentualinicial} AND {$percentualfinal}");
	}
	
	// repositorio
	if( $flag_repositorio ){
		array_push($where, " oi.obrid IN ( select distinct obrid from obras.repositorio where repstatus = 'A') ");
	}
		
	// percentual da obra
	if( $latitudeElongitude ){
		array_push($where, " (TRIM(ed.medlatitude)<>'' AND TRIM(ed.medlongitude)<>'')");
	}
	
	// possui foto
	switch ( $foto ) {
		case 'sim' : $stFiltro .= " and (ao.obrid is not null and ao.aqostatus = 'A') "; break;
		case 'nao' : $stFiltro .= " and ao.obrid is null  "; break;
	}

	// Filtro de vistoria
	switch ( $_REQUEST["vistoria"] ) {
		case 'sim' : $stFiltro .= " and oi.obrdtvistoria is not null "; break;
		case 'nao' : $stFiltro .= " and oi.obrdtvistoria is null "; break;
		//default    : $stFiltro .= " and ( ( s.obrid is null ) OR ( s.obrid is not null AND s.supstatus <> 'I' ) ) "; break;
	}

	// Filtro de responsï¿½vel pela vistoria
	switch ( $_REQUEST["responsavel"] ) {
		case ''  : $stFiltro .= " "; break;
		case '1' : $stFiltro .= " and s.rsuid = 1 "; break;
		case '2' : $stFiltro .= " and s.rsuid = 2 "; break;
		case '3' : $stFiltro .= " and s.rsuid = 3 "; break;
		case '4' : $stFiltro .= " and s.rsuid = 4 "; break;
	}
	
	// Filtro de restricao
	switch ( $restricao ) {
		case 'sim' : $stFiltro .= " and (r.obrid is not null and r.rststatus = 'A')"; break;
		case 'nao' : $stFiltro .= " and r.obrid is null "; break;
	}

	// Filtro de 'Com retorno de (concluï¿½da) para (em execuï¿½ï¿½o)'
	switch( $concluidaexec )
	{
		case 'sim' : $stFiltro .= " and (SELECT DISTINCT super.stoid FROM obras.supervisao super WHERE super.supstatus = 'A' AND super.obrid = oi.obrid AND super.supvid = (SELECT DISTINCT max(su.supvid) FROM obras.supervisao su WHERE su.supstatus = 'A' AND su.obrid = oi.obrid)) = 1 and (SELECT DISTINCT super.stoid FROM obras.supervisao super WHERE super.supstatus = 'A' AND super.obrid = oi.obrid AND super.supvid = (SELECT DISTINCT max(su.supvid) FROM obras.supervisao su WHERE su.supstatus = 'A' AND su.obrid = oi.obrid AND su.supvid NOT IN (SELECT DISTINCT max(su.supvid) FROM obras.supervisao su WHERE su.supstatus = 'A' AND su.obrid = oi.obrid))) = 3 "; break;
		case 'nao' : $stFiltro .= " and (SELECT DISTINCT super.stoid FROM obras.supervisao super WHERE super.supstatus = 'A' AND super.obrid = oi.obrid AND super.supvid = (SELECT DISTINCT max(su.supvid) FROM obras.supervisao su WHERE su.supstatus = 'A' AND su.obrid = oi.obrid)) <> 1 or (SELECT DISTINCT super.stoid FROM obras.supervisao super WHERE super.supstatus = 'A' AND super.obrid = oi.obrid AND super.supvid = (SELECT DISTINCT max(su.supvid) FROM obras.supervisao su WHERE su.supstatus = 'A' AND su.obrid = oi.obrid AND su.supvid NOT IN (SELECT DISTINCT max(su.supvid) FROM obras.supervisao su WHERE su.supstatus = 'A' AND su.obrid = oi.obrid))) <> 3 "; break;
	}
	
	/*'<a style=\"cursor:pointer;\" onclick=\"parent.opener.window.location.href=\'/obras/obras.php?modulo=principal/cadastro&acao=A&obrid=' || oi.obrid || '\'; parent.opener.window.focus();\"> (' || oi.obrid || ') ' || oi.obrdesc || ' &nbsp;(' || (SELECT replace(coalesce(round(SUM(icopercexecutado), 2), '0') || ' % Executado', '.', ',') as total FROM obras.itenscomposicaoobra WHERE obrid = oi.obrid) || ') </a>' as nomedaobra,*/
	
	// monta o sql 
	$sql = "SELECT
				CASE WHEN metragem < 500 THEN '<span id=\"1\">Atï¿½ 500 (mï¿½)</span>'
					 WHEN metragem > 500 AND metragem < 1500  THEN '<span id=\"2\">500 atï¿½ 1500 (mï¿½)</span>'
				 	 WHEN metragem > 1500 AND metragem < 4500 THEN '<span id=\"3\">1500 atï¿½ 4500 (mï¿½)</span>'
					 WHEN metragem > 4500 AND metragem < 10000 THEN '<span id=\"4\">4500 atï¿½ 10000 (mï¿½)</span>'
					 WHEN metragem > 10000 THEN '<span id=\"5\">Maior que 10000 (mï¿½)</span>'
					 WHEN metragem is null THEN '<span id=\"6\">Nï¿½o Informado</span>'
				ELSE '<span id=\"6\">Nï¿½o Informado</span>' END as metragem,
				CASE WHEN metragem < 500 THEN 'Atï¿½ 500 (mï¿½)'
					 WHEN metragem > 500 AND metragem < 1500  THEN '500 atï¿½ 1500 (mï¿½)'
				 	 WHEN metragem > 1500 AND metragem < 4500 THEN '1500 atï¿½ 4500 (mï¿½)'
					 WHEN metragem > 4500 AND metragem < 10000 THEN '4500 atï¿½ 10000 (mï¿½)'
					 WHEN metragem > 10000 THEN 'Maior que 10000 (mï¿½)'
					 WHEN metragem is null THEN 'Nï¿½o Informado'
				ELSE 'Nï¿½o Informado' END as metragemxls, 
				mesoregiao,
				regiao,
				pais,
				unidade,
				campus,
				empresa,
				uf,
				{$dadosTipoMun}
				{$dadosGrupoMun}
				municipio,
				CASE WHEN codigo_situacao IN (1, 2) THEN
					(CASE WHEN DATE_PART('days', NOW() - nivelpreenchimento) <= 45  
						 	THEN '<span style=\"color: green;\">1 - Verde (Obras atualizadas hï¿½ menos de 45 dias atrï¿½s)</span>'
						 WHEN DATE_PART('days', NOW() - nivelpreenchimento) BETWEEN 45  AND 60
						 	THEN '<span style=\"color: #BB9900;\">2 - Amarelo (Obras atualizadas entre 45 e 60 dias)</span>'
						 ELSE '<span style=\"color: red;\">3 - Vermelho (Obras atualizadas hï¿½ mais de 60 dias)</span>'
					END)
					WHEN codigo_situacao = 3 THEN '<span style=\"color: blue;\">4 - Azul (Obras concluï¿½das)</span>'
				ELSE
					'5 - Nï¿½o se aplica (Obras Em Elaboraï¿½ï¿½o de Projetos/Em Licitaï¿½ï¿½o)' END as nivelpreenchimento,
				CASE WHEN codigo_situacao IN (1, 2) THEN
					(CASE WHEN DATE_PART('days', NOW() - nivelpreenchimento) <= 45  
						 	THEN '1 - Verde (Obras atualizadas hï¿½ menos de 45 dias atrï¿½s)'
						 WHEN DATE_PART('days', NOW() - nivelpreenchimento) BETWEEN 45  AND 60
						 	THEN '2 - Amarelo (Obras atualizadas entre 45 e 60 dias)'
						 ELSE '3 - Vermelho (Obras atualizadas hï¿½ mais de 60 dias)'
					END)
					WHEN codigo_situacao = 3 THEN '4 - Azul (Obras concluï¿½das)'
				ELSE
					'5 - Nï¿½o se aplica (Obras Em Elaboraï¿½ï¿½o de Projetos/Em Licitaï¿½ï¿½o)' END as nivelpreenchimentoxls,
				classificacao,
				situacao,
				tipologia,
				programa,
				nomedaobra,
				nomedaobra2,
				nomedaobraxls,
				coalesce(sum(superior),0) as superior,
				coalesce(sum(tecnico),0) as tecnico, 
				coalesce(sum(basico),0) as basico,
				coalesce(sum(administrativa),0) as administrativa,
				coalesce(sum(total),0) as total
			FROM	
				(SELECT
					oi.obrqtdconstruida as metragem,
					CASE WHEN oi.entidcampus is not null THEN ee2.entnome ELSE 'Nï¿½o informado' END as campus,
					CASE WHEN ee3.entnome is not null THEN ee3.entnome ELSE 'Nï¿½o informado' END as empresa,
					me.mesdsc as mesoregiao,
					re.regdescricao as regiao,
					pa.paidescricao as pais,
					ee.entnome as unidade,
					CASE WHEN ed.estuf <> '' THEN ed.estuf ELSE 'Nï¿½o Informado' END as uf,
					{$selectTipoMun}
					{$selectGrupoMun}
					tm.mundescricao as municipio,
					CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END as nivelpreenchimento,
					CASE WHEN oi.cloid is not null THEN cl.clodsc  ELSE 'Nï¿½o informado' END as classificacao,
					CASE WHEN oi.stoid is not null THEN st.stodesc ELSE 'Nï¿½o Informado' END as situacao,
					oi.stoid as codigo_situacao,
					CASE WHEN oi.tpoid is not null THEN tp.tpodsc  ELSE 'Nï¿½o informado' END as tipologia,
					CASE WHEN oi.prfid is not null THEN pf.prfdesc ELSE 'Nï¿½o informado' END as programa,
					oi.obrdesc as nomedaobraxls,
					
					'<a style=\"cursor:pointer;\" onclick=\"parent.opener.window.location.href=\'/obras/obras.php?modulo=principal/cadastro&acao=A&obrid=' || oi.obrid || '\'; parent.opener.window.focus();\"> (' || oi.obrid || ') ' || oi.obrdesc || ' &nbsp;(' || 
					(SELECT
						replace(
							(SELECT 
									trunc(coalesce( sum(( icopercsobreobra * supvlrinfsupervisor ) / 100) ,0 )::numeric, 2)
							  	 FROM 
									obras.itenscomposicaoobra i
							  	 INNER JOIN 
										obras.supervisaoitenscomposicao si ON i.icoid = si.icoid WHERE si.supvid = s.supvid AND obrid = oi.obrid AND i.icovigente = 'A' )
						 || ' % Executado', '.', ',') as percentual
						FROM
						obras.supervisao s
						INNER JOIN 
						obras.situacaoobra si ON si.stoid = s.stoid
						INNER JOIN
						seguranca.usuario u ON u.usucpf = s.usucpf
						LEFT JOIN
						entidade.entidade e ON e.entid = s.supvistoriador
						LEFT JOIN
						obras.realizacaosupervisao rs ON rs.rsuid = s.rsuid 
						WHERE
						s.obrid = oi.obrid AND
						s.supstatus = 'A'
						ORDER BY 
						s.supdtinclusao DESC LIMIT 1)
					 || ') </a>' as nomedaobra,
					 
					'<a style=\"cursor:pointer;\" onclick=\"abrebalao(' || oi.obrid || ');\">' || oi.obrdesc || '</a>' as nomedaobra2,
					CASE WHEN oi.orgid = 1 THEN count(oi.obrid) END as superior,
					CASE WHEN oi.orgid = 2 THEN count(oi.obrid) END as tecnico,
					CASE WHEN oi.orgid = 3 THEN count(oi.obrid) END as basico,
					CASE WHEN oi.orgid = 4 THEN count(oi.obrid) END as administrativa,
					count(oi.obrid) as total
				FROM
					obras.obrainfraestrutura oi
				INNER JOIN 
					entidade.endereco ed 	   ON oi.endid = ed.endid	
				LEFT JOIN 
					territorios.estado et 	   ON ed.estuf = et.estuf
				LEFT JOIN 
					territorios.regiao re 	   ON re.regcod = et.regcod
				LEFT JOIN 
					territorios.municipio tm2  ON tm2.muncod = ed.muncod
				LEFT JOIN 
					territorios.mesoregiao me  ON me.mescod = tm2.mescod
				LEFT JOIN 
					{$selectTerritorios} tm    ON tm.muncod = ed.muncod
				INNER JOIN 
					entidade.entidade ee 	   ON oi.entidunidade = ee.entid
				LEFT JOIN 
					territorios.pais pa 	   ON pa.paiid = re.paiid
				LEFT JOIN 
					entidade.entidade ee2 	   ON oi.entidcampus = ee2.entid
				LEFT JOIN 
					entidade.funcaoentidade ef ON ee2.entid = ef.entid AND ef.funid IN( 17 )
				LEFT JOIN
					entidade.entidade ee3 	   ON oi.entidempresaconstrutora = ee3.entid
				LEFT JOIN 
					obras.programafonte pf 	   ON oi.prfid = pf.prfid
				LEFT JOIN
					obras.classificacaoobra cl ON oi.cloid = cl.cloid
				LEFT JOIN
					obras.situacaoobra st 	   ON oi.stoid = st.stoid
				LEFT JOIN
					obras.tipologiaobra tp 	   ON oi.tpoid = tp.tpoid
				LEFT JOIN
					(SELECT
						rsuid,obrid
					FROM
						obras.supervisao s
					WHERE
						supvid = (SELECT max(supvid) FROM obras.supervisao ss WHERE ss.obrid = s.obrid) ) AS s ON s.obrid = oi.obrid
				LEFT JOIN 
					( SELECT DISTINCT obrid, aqostatus FROM obras.arquivosobra WHERE tpaid = 21 AND aqostatus = 'A' ) as ao ON ao.obrid = oi.obrid 
				LEFT JOIN 
					( SELECT DISTINCT obrid, rststatus FROM obras.restricaoobra WHERE rststatus = 'A' ) as r ON r.obrid = oi.obrid
				 ".$innerSupervisaoConcluida." 
				WHERE
					oi.obsstatus = 'A' " . ( is_array($where) ? ' AND' . implode(' AND ', $where) : '' ) 
				. $stFiltro . "   
				GROUP BY 
					oi.orgid, ed.estuf, tm.mundescricao, {$groupByGtmid} {$groupByGrupoMun} {$groupByTipoMun}
					ee.entnome, ee2.entnome, ee3.entnome, me.mesdsc,
					re.regdescricao, pa.paidescricao, cl.clodsc,
					st.stodesc, oi.stoid, tp.tpodsc, pf.prfdesc, oi.obrdesc,
					oi.prfid, oi.entidcampus, oi.cloid, oi.stoid, 
					oi.tpoid, oi.prfid, oi.obrid, oi.obrdtvistoria, oi.obsdtinclusao, oi.obrqtdconstruida ) as foo
			GROUP BY
				unidade, campus, uf, {$dadosTipoMun} {$dadosGrupoMun}
				municipio, nivelpreenchimento, mesoregiao, regiao, tipologia, 
				classificacao, programa, situacao, codigo_situacao, nomedaobra, nomedaobra2, empresa, nomedaobraxls,
				pais, metragem 
			ORDER BY
				" . (is_array( $agrupador ) ?  implode(",", $agrupador) : "pais") ;
	return $sql;
	
}

/**
 * Funï¿½ao que monta o agrupador do relatï¿½rio geral de obras
 *
 * @author Fernando A. Bagno da Silva
 * @since 20/02/2009
 * @return array
 */
function obras_monta_agp_relatorio(){
	
	$agrupador = $_REQUEST['agrupadorNovo'] ? $_REQUEST['agrupadorNovo'] : $_REQUEST['agrupador'];
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array("superior",
										  "tecnico",
										  "basico",
										  "administrativa",
										  "total"),
				"agrupadorDetalhamento" => array(
													array(
															"campo" => "mesoregiao",
															"label" => "Mesoregião"
														  ),
													array(
															"campo" => "campus",
															"label" => "Campus"
														  ),
													array(
															"campo" => "municipio",
															"label" => "Município"
														  ),
													array(
															"campo" => "pais",
													  		"label" => "País"										
											   			  ),
											   		array(
															"campo" => "regiao",
													  		"label" => "Região"										
											   			  ),
											   		array(
															"campo" => "nomedaobra",
													  		"label" => "Nome da Obra"										
											   				)					  
												)	  
				);
	
	foreach ( $agrupador as $val ){
		switch( $val ){
			case "campus":
				array_push($agp['agrupador'], array(
													"campo" => "campus",
											  		"label" => "Campus")										
									   				);
			break;
			case "mesoregiao":
				array_push($agp['agrupador'], array(
													"campo" => "mesoregiao",
											  		"label" => "Mesoregião")										
									   				);
			break;
			case "municipio":
				array_push($agp['agrupador'], array(
													"campo" => "municipio",
											  		"label" => "Município")										
									   				);
			break;
			case "pais":
				array_push($agp['agrupador'], array(
													"campo" => "pais",
											  		"label" => "País")										
									   				);
			break;
			case "regiao":
				array_push($agp['agrupador'], array(
													"campo" => "regiao",
											  		"label" => "Região")										
									   				);
			break;
			case "uf":
				array_push($agp['agrupador'], array(
													"campo" => "uf",
											  		"label" => "UF")										
									   				);
			break;
			case "unidade":
				array_push($agp['agrupador'], array(
													"campo" => "unidade",
											  		"label" => "Unidade")										
									   				);
			break;
			case "programa":
				array_push($agp['agrupador'], array(
													"campo" => "programa",
											  		"label" => "Programa Fonte")										
									   				);
			break;
			case "situacao":
				array_push($agp['agrupador'], array(
													"campo" => "situacao",
											  		"label" => "Situação da Obra")										
									   				);
			break;
			case "tipologia":
				array_push($agp['agrupador'], array(
													"campo" => "tipologia",
											  		"label" => "Tipologia da Obra")										
									   				);
			break;
			case "classificacao":
				array_push($agp['agrupador'], array(
													"campo" => "classificacao",
											  		"label" => "Classificação da Obra")										
									   				);
			break;
			case "nomedaobra":
				array_push($agp['agrupador'], array(
													"campo" => "nomedaobra",
											  		"label" => "Nome da Obra")										
									   				);
			break;
			case "nomedaobra2":
				array_push($agp['agrupador'], array(
													"campo" => "nomedaobra2",
											  		"label" => "Nome da Obra")										
									   				);
			break;
			case "nomedaobraxls":
				array_push($agp['agrupador'], array(
													"campo" => "nomedaobraxls",
											  		"label" => "Nome da Obra")										
									   				);
			break;
			case "nivelpreenchimento":
				array_push($agp['agrupador'], array(
													"campo" => "nivelpreenchimento",
											  		"label" => "Nível de Preenchimento")										
									   				);
			break;
			case "nivelpreenchimentoxls":
				array_push($agp['agrupador'], array(
													"campo" => "nivelpreenchimentoxls",
											  		"label" => "Nível de Preenchimento")										
									   				);
			break;
			
			case "empresa":
				array_push($agp['agrupador'], array(
													"campo" => "empresa",
											  		"label" => "Empresa Contratada")										
									   				);
			break;
			case "metragem":
				array_push($agp['agrupador'], array(
													"campo" => "metragem",
											  		"label" => "Metragem da Obra")										
									   				);
			break;
			case "metragemxls":
				array_push($agp['agrupador'], array(
													"campo" => "metragemxls",
											  		"label" => "Metragem da Obra")										
									   				);
			break;
			
			/*case "grupomun":
				array_push($agp['agrupador'], array(
													"campo" => "grupomun",
											  		"label" => "Grupo de Município")										
									   				);
			break;*/
			case "tipomun":
				array_push($agp['agrupador'], array(
													"campo" => "tipomun",
											  		"label" => "Território da Cidadania")										
									   				);
			break;
		}	
	}
	
	return $agp;
	
}

/**
 * Funçao que monta as colunas do relatï¿½rio geral de obras
 *
 * @author Fernando A. Bagno da Silva
 * @since 20/02/2009
 * @return array
 */
function obras_monta_coluna_relatorio(){
	
	$coluna = array();
	
	foreach ( $_REQUEST['orgid'] as $valor ){

		switch( $valor ){
			case '1':
				array_push( $coluna, array("campo" 	  => "superior",
								   		   "label" 	  => "Ensino Superior",
								   		   "blockAgp" => "nomedaobra",
								   		   "type"	  => "numeric") );
			break;
			case '2':
				array_push( $coluna, array("campo" 	  => "tecnico",
								   		   "label" 	  => "Ensino Profissional",
								   		   "blockAgp" => "nomedaobra",
								   		   "type"	  => "numeric") );
			break;
			case '3':
				array_push( $coluna, array("campo" 	  => "basico",
								   		   "label" 	  => "Ensino Básico",
								   		   "blockAgp" => "nomedaobra",
								   		   "type"	  => "numeric") );
			break;
			case '4':
				array_push( $coluna, array("campo" 	  => "administrativa",
								   		   "label" 	  => "Administrativas",
								   		   "blockAgp" => "",
								   		   "type"	  => "numeric") );
			break;
		}
		
	}
	
	array_push( $coluna, array("campo" 	 => "total",
								   		   "label" 	 => "Total de Obras",
								   		   "blockAgp" => "nomedaobra",
								   		   "type"	 => "numeric") );
	
	return $coluna;
	
}

/**
 * Enter description here...
 *
 */
function obras_verifica_sessao(){
	
	if (!$_SESSION["obra"]["obrid"]){
		print "<script>"
			. "		alert('A sessão da obra escolhida expirou!');"
			. "		window.location='/obras/obras.php?modulo=inicio&acao=A';"
			. "</script>";
	}
	
}

/**
 * Enter description here...
 *
 * @param unknown_type $obrid
 * @return unknown
 */
function obras_pega_situacao( $obrid ){
	
	global $db;
	
	if ($obrid <> ''){
		$sql = "SELECT stoid FROM obras.obrainfraestrutura WHERE obrid = {$obrid}";	
		return $db->pegaUm($sql);
	}
}

function obras_pega_situacao_vistoria( $obrid ){
	
	global $db;
	
	$sql = "SELECT 
				stoid 
			FROM 
				obras.supervisao 
			WHERE 
				supvid = ( SELECT max(supvid) as supvid 
						   FROM obras.supervisao 
						   WHERE supstatus = 'A' AND obrid = {$obrid} GROUP BY obrid )";
	
	$stoid = $db->pegaUm($sql);
//	$stoid = !empty($stoid) ? $stoid : 99;
	
	return $stoid;
		
}

/**
 * Funï¿½ï¿½o que verifica se possui alguma Vistoria que foi cadastrada pelo Perfil "EMPRESA(rsuid = 3)", na Lista de Vistorias 
 * @param $obrid
 */
function verifica_realizado_por_empresa($obrid){
	
	global $db;
	$sql = " SELECT 
					MAX(supvid)
			 FROM 
					obras.supervisao 
			 WHERE 
					supstatus = 'A' 
					AND obrid = {$obrid} 
					AND rsuid = 3
		   ";
	$supvid = $db->pegaUm($sql);
	
	return $supvid; 
}

/**
 * Funï¿½ï¿½o que recupera a Situaï¿½ï¿½o da Vistoria, quando a mesma for cadastrada pelo Perfil "EMPRESA(rsuid = 3)".
 * @param $obrid
 */
function pega_situacao_obra_empresa($obrid){
	
	global $db;
	
	$sql = " SELECT 
			        stoid
	         FROM 
					obras.supervisao 
			 WHERE 
					supstatus = 'A' 
					AND obrid = {$obrid} 
					AND rsuid = 3 /*Empresa*/
			ORDER BY
					supvid DESC		
			";
	
	$dadoStoid = $db->pegaUm($sql);
	
	return $dadoStoid;
}
/**
 * Funï¿½ï¿½o que recupera o Percentual Executado da Supervisï¿½o, quando a mesma for cadastrada pelo Perfil "EMPRESA(rsuid = 3)".
 * @param  $obrid
 */
function pega_percentual_obra_empresa($obrid) {
	
	global $db;
	
	 $sql = " SELECT 
					COALESCE(
								(SELECT 
										SUM(( icopercsobreobra * supvlrinfsupervisor ) / 100)
			 					 FROM 
										obras.itenscomposicaoobra i
								 INNER JOIN 
										obras.supervisaoitenscomposicao si ON i.icoid = si.icoid 
								 WHERE 
										si.supvid = s.supvid 
										AND obrid = {$obrid}
								 ),'0'
							 ) AS percentual	
				FROM 
					obras.supervisao s
				WHERE 
					supstatus = 'A' 
					AND obrid = {$obrid}  
					AND rsuid = 3 /*Empresa*/
				ORDER BY
					supvid DESC
		    ";
	$dadoObrsuppercexec = $db->pegaUm($sql);
	
	return $dadoObrsuppercexec;
} 

/**
 * Enter description here...
 *
 * @param unknown_type $stoid
 */
function obras_situacao_possivel( $stoid ){
	
	global $db;
	
	$ordem = null;
	
	if ( $stoid ){
		switch ( $stoid ){
			case 1:
				$ordem = "in ( 3,4,5,7 )";
				break;
			case 2:
				$ordem = "in ( 3,4,7 )";
				break;
			case 3:
				$ordem = "in ( 3,5,7 )";
				break;
			case 4:
				$ordem = "in ( 1,2,3,7 )";
				break;
			case 5:
				$ordem = "in ( 2,3,7 )";
				break;
			case 6:
				$ordem = "in ( 1,2,3,4,5,7 )";
				break;
			case 99:
				$ordem = "in ( 1,2,3,4,5,7 )";
				break;
		}
	
		$sql = "SELECT
					stoid as codigo, 
					stodesc as descricao 
				FROM 
					obras.situacaoobra
				WHERE
					stoordem {$ordem} AND stoid <> 99
				ORDER BY
					stoordem";
	}else{
		
		$sql = "SELECT obridaditivo
				FROM obras.obrainfraestrutura
				WHERE obrid = {$_SESSION["obra"]["obrid"]}
				  AND obsstatus = 'A'";
		
		$aditivo = $db->pegaUm($sql);
		
		if( $aditivo ){
			$sql = "SELECT
						stoid as codigo, 
						stodesc as descricao 
					FROM 
						obras.situacaoobra
					WHERE
						stoordem in ( 3,4,5,7 ) AND stoid <> 99
					ORDER BY
						stoordem";
		}else{
			$sql = "SELECT
						stoid as codigo, 
						stodesc as descricao 
					FROM 
						obras.situacaoobra
					WHERE
						stoid <> 99
					ORDER BY
						stoordem";
		}
		
	}
	
	return $sql;
} 

/**
 * Enter description here...
 *
 * @param unknown_type $obrid
 * @return unknown
 */
function obras_busca_dados_obra ( $obrid ){
	
	global $db;
	
	$sql = "SELECT
				orgid,
				entidunidade,
				entidcampus,
				obrdesc,
				prfid,
				cloid,
				tpoid,
				obrcomposicao,
				endid,
				obsobra
			FROM
				obras.obrainfraestrutura oi 
			WHERE
				oi.obrid = {$obrid}";
	
	return $db->pegaLinha($sql);
	
}
/**
 * Busca dados dos Contatos da Obra.
 * @param $obrid
 */
function obras_busca_dados_obra_contato ( $obrid ){

	global $db;
	
	 $sql="SELECT
				rc.tprcid as tipo,*, 
				rc.recoid as responsavel,
				rc.entid as entidade, 
				et.entnome as nome,
				et.entnumcpfcnpj as cpf  
			FROM 
				obras.responsavelobra r 
			INNER JOIN 
				obras.responsavelcontatos rc ON r.recoid = rc.recoid
			INNER JOIN 
				entidade.entidade et ON rc.entid = et.entid
			WHERE 
				r.obrid = '". $obrid . "'  AND rc.recostatus = 'A'";

	 return $db->carregar($sql);
}

/**
 * Busca dados dos Responsï¿½veis pela Obra.
 * @param $obrid
 */
function obras_busca_dados_obra_responsavel ( $obrid ){

	global $db;
	
	$sql = "SELECT DISTINCT
					*,
					ur.rpuid as id,
					su.usucpf as cpf, 
					su.usunome as nome
				FROM 
					seguranca.usuario su 
				JOIN 
					obras.usuarioresponsabilidade ur ON ur.usucpf = su.usucpf AND ur.rpustatus = 'A' 
				WHERE 
					ur.obrid = '". $obrid ."'";
					
	 return $db->carregar($sql);
}

/**
 * Enter description here...
 *
 * @param unknown_type $obrid
 */
function obras_cria_nova_obra( $obrid ){
	
	global $db;
	
	// busca os dados da obra
	$dados = obras_busca_dados_obra( $obrid );
	// busca os dados do Contato
	$dadosContato = obras_busca_dados_obra_contato( $obrid );
	// busca os dados do Responsï¿½vel
	$dadosResponsavel = obras_busca_dados_obra_responsavel ( $obrid );

	// Atribui valores nulos aos campos em branco e coloca aspas
	foreach( $dados as $campo=>$valor ){
		if( !is_array( $dados[$campo] ) ){
			if( $valor == "" ){
				$dados[$campo] = 'NULL';
			} else {
				$dados[$campo] = "'" . pg_escape_string(trim($valor))  .  "'";
			}
		}
	}
		
	// cria a nova obra
	$sql = "INSERT INTO 
				obras.obrainfraestrutura( orgid,
										  entidunidade,
										  entidcampus,
										  obrdesc,
										  prfid,
										  cloid,
										  tpoid,
										  obrcomposicao,
										  endid,
										  obsobra,
										  obridorigem ) 
				VALUES 
					( {$dados['orgid']},
					  {$dados['entidunidade']},
					  {$dados['entidcampus']},
					  {$dados['obrdesc']},
					  {$dados['prfid']},
					  {$dados['cloid']},
					  {$dados['tpoid']},
					  {$dados['obrcomposicao']},
					  {$dados['endid']},
					  {$dados['obsobra']},
					  {$obrid} ) returning obrid";
	
	$obridnova = $db->pegaUm( $sql );
	
	if(is_array($dadosContato)){
		foreach ($dadosContato as $campo=>$valor ){	
			$sqlNovoContato = " INSERT INTO 
										obras.responsavelobra(
		            											recoid, 
		            											obrid
		            										  )
		    					VALUES (
		    							'{$dadosContato[$campo]['recoid']}',
		    							 '{$obridnova}'
		    							)";
			$db->executar( $sqlNovoContato );
		}	
	}
	
	if( is_array($dadosResponsavel)){
		foreach ($dadosResponsavel as $campo=>$valor ){	
			$sqlNovoResponsavel =" INSERT INTO 
											obras.usuarioresponsabilidade(
																			usucpf, 
																			rpustatus, 
																			rpudata_inc, 
																			pflcod, 
																			obrid
									      								  )
							   		VALUES (
											 '{$dadosResponsavel[$campo]['usucpf']}',
									   		 'A', 
									   		 '{$dadosResponsavel[$campo]['rpudata_inc']}', 
									   		 '{$dadosResponsavel[$campo]['pflcod']}',
									   		 '{$obridnova}'
				            				)";
			$db->executar( $sqlNovoResponsavel );
		}	
	}
	
	// Inativa o Responsavel pela obra antigo.
	$sqlResponsavel = "UPDATE obras.usuarioresponsabilidade SET	rpustatus = 'I' WHERE obrid ='".$obrid."'";
	$db->executar( $sqlResponsavel );

	// inativa a obra antiga
	$sql = "UPDATE obras.obrainfraestrutura SET obsstatus = 'I' WHERE obrid = {$obrid}";
	$db->executar( $sql );
	
	$db->commit();
	echo "<script>
				alert('Operação realizada com sucesso!');
				window.location.href = '?modulo=principal/cadastro&acao=A&obrid=' + $obridnova;
		  </script>";
					
}

/*
 * Funï¿½ï¿½es que implementï¿½o a regra do ADITIVO
 */

function obraAditivoPossuiCronograma(){
	global $db;
	
	$obrid = $_SESSION['obra']['obrid'];
	if ( $obrid ){
		$sql = "SELECT 
					count(i.*) as cronograma,
					obridaditivo
				FROM 
					obras.obrainfraestrutura o	
				LEFT JOIN obras.itenscomposicaoobra i USING(obrid)
				WHERE 
					o.obrid = {$obrid}
					AND obridaditivo IS NOT NULL
				GROUP BY obridaditivo";
		
		$res = $db->pegaLinha($sql);
		return ( ( $res['cronograma'] > 0 || $res['obridaditivo'] == "" )  ? true : false);
	}
	return true;
}


function obraAditivoPossuiVistoria(){
	global $db;
	
	$obrid = $_SESSION['obra']['obrid'];
	if ( $obrid ){
		$sql = "SELECT 
					count(s.*) as vistoria,
					obridaditivo
				FROM 
					obras.obrainfraestrutura o	
				LEFT JOIN obras.supervisao s USING(obrid)	
				WHERE 
					obrid = {$obrid}
					AND obridaditivo IS NOT NULL
				GROUP BY
					obridaditivo;";
		
			$res = $db->pegaLinha($sql);
			return ( ( $res['vistoria'] > 0 || $res['obridaditivo'] == "" )  ? true : false);
	}
	return true;
}
/*
 * FIM => Funï¿½ï¿½es que implementï¿½o a regra do ADITIVO
 */




function pegaObMaiorVlrAditivo(Array $arParam = null){
	global $db;
	
	$where = array();
	if ( $arParam['traid'] ){
		array_push($where,  "traid <= {$arParam['traid']}"); 
	}
	
	if ( $arParam['traseq'] ){
		array_push($where,  "traseq <= {$arParam['traseq']}"); 
	}
	
	$obrid = $_SESSION["obra"]['obrid'];
	$sql = "SELECT 
				MAX(traid) AS traid,
				travlrfinalobra
			FROM 
				obras.termoaditivo ta
			JOIN obras.tipotermoaditivo tta ON tta.ttaid = ta.ttaid
			WHERE 
				obrid = $obrid 
				AND ta.ttaid IN (2, 3)
				" . ( count($where) ? " AND " . implode(" AND ", $where) : "" ) . "
				AND trastatus = 'A'
			GROUP BY
				travlrfinalobra
			ORDER BY
				traid DESC
			limit 1";

	$travlrfinalobra = $db->pegaUm($sql, 1);
	if($travlrfinalobra){
		return $travlrfinalobra;
	}else{
		return "0";
	}
}

function pegaSomaVlrAditivo($coluna, Array $arParam = null){
	global $db;
	
	$where = array();
	if ( $arParam['traid'] ){
		array_push($where,  "traid <= {$arParam['traid']}"); 
	}
	
	if ( $arParam['traseq'] ){
		array_push($where,  "traseq <= {$arParam['traseq']}"); 
	}
	
	$obrid = $_SESSION["obra"]['obrid'];
	$sql = "SELECT 
				SUM($coluna) AS total
			FROM 
				obras.termoaditivo ta
			WHERE 
				obrid = $obrid 
				" . ( count($where) ? " AND " . implode(" AND ", $where) : "" ) . "
				AND trastatus = 'A';";
	$total = $db->pegaUm($sql);
	
	return $total;
}


function pegaObAditivo($traid){
	global $db;
	
//	$obrid = $_SESSION["obra"]['obrid'];
	$sql = "SELECT 
				traid AS traid, ta.ttaid, tta.ttadsc, usucpf, umdidareaacresc, umdidareafinal, umdidareaalterada, 
			    obrid, tradsc, traseq, tradtassinatura, traprazovigencia, to_char(traterminovigencia, 'dd/mm/YYYY') AS traterminovigencia, 
			    traprazoaditivadoexec, travlraditivo, travlrfinalobra, travlrqtdareaacresc, 
			    travlrqtdareafinal, travlrqtdareaalterada, trajustificativa, 
				tradtinclusao, trastatus, to_char(traterminoexec, 'dd/mm/YYYY') AS traterminoexec 
			FROM 
				obras.termoaditivo ta
			JOIN obras.tipotermoaditivo tta ON tta.ttaid = ta.ttaid
			WHERE 
				traid = $traid
				-- obrid = $obrid 
				-- AND trastatus = 'A';";
	$obTermo = $db->pegaUmObjeto($sql);
	
	return $campo ? $obTermo->{$campo} : $obTermo;
}

function pegaObUltimoAditivo($campo = null, $tipoAditivo = null, $obrid = NULL){
	global $db;

	$where = $tipoAditivo ? " AND ta.ttaid = " . $tipoAditivo : "";
	
	if(isset($_SESSION["obra"]['obrid'])){
		$obrid = $_SESSION["obra"]['obrid'];
	}
	
	$sql = "SELECT 
			    traid, ta.ttaid, tta.ttadsc, usucpf, umdidareaacresc, umdidareafinal, umdidareaalterada, 
			    obrid, tradsc, traseq, tradtassinatura, traprazovigencia, to_char(traterminovigencia, 'dd/mm/YYYY') AS traterminovigencia, 
			    traprazoaditivadoexec, travlraditivo, travlrfinalobra, travlrqtdareaacresc, 
			    travlrqtdareafinal, travlrqtdareaalterada, trajustificativa, 
			    tradtinclusao, trastatus, to_char(traterminoexec, 'dd/mm/YYYY') AS traterminoexec 
			FROM 
				obras.termoaditivo ta
			JOIN obras.tipotermoaditivo tta ON tta.ttaid = ta.ttaid
			WHERE 
				obrid = $obrid
				AND trastatus = 'A'
				{$where}
				AND traid = (SELECT
								traid
							 FROM
								obras.termoaditivo
							 WHERE
								obrid = ta.obrid
								AND trastatus = 'A'
							 ORDER BY
								traseq DESC
							 LIMIT 1);";
	$obTermo = $db->pegaUmObjeto($sql);
	
	return $campo ? $obTermo->{$campo} : $obTermo;
}

function pegaObUltimoDadosAditivo($campo = null){
	global $db;
	
	$obrid = $_SESSION["obra"]['obrid'];
	
	// VALOR
	$sql = "SELECT 
				max(traid) AS traid, ta.ttaid, tta.ttadsc, usucpf, umdidareaacresc, umdidareafinal, umdidareaalterada, 
			    obrid, tradsc, traseq, tradtassinatura, traprazovigencia, to_char(traterminovigencia, 'dd/mm/YYYY') AS traterminovigencia, 
			    traprazoaditivadoexec, travlraditivo, travlrfinalobra, travlrqtdareaacresc, 
			    travlrqtdareafinal, travlrqtdareaalterada, trajustificativa, 
				tradtinclusao, trastatus, to_char(traterminoexec, 'dd/mm/YYYY') AS traterminoexec 
			FROM 
				obras.termoaditivo ta
			JOIN obras.tipotermoaditivo tta ON tta.ttaid = ta.ttaid
			WHERE 
				obrid = $obrid 
				AND trastatus = 'A'
				AND ta.ttaid = " . ADITIVO_VALOR . "
			GROUP BY 
				ta.ttaid, tta.ttadsc, usucpf, umdidareaacresc, umdidareafinal, umdidareaalterada, 
			    obrid, tradsc, traseq, tradtassinatura, traprazovigencia, traterminovigencia, 
			    traprazoaditivadoexec, travlraditivo, travlrfinalobra, travlrqtdareaacresc, 
			    travlrqtdareafinal, travlrqtdareaalterada, trajustificativa, 
			    tradtinclusao, trastatus, traterminoexec
			ORDER BY 
				traseq DESC
			LIMIT 1;";
	$obTermoValor = $db->pegaUmObjeto($sql);
	
	// PRAZO
	$sql = "SELECT 
				max(traid) AS traid, ta.ttaid, tta.ttadsc, usucpf, umdidareaacresc, umdidareafinal, umdidareaalterada, 
			    obrid, tradsc, traseq, tradtassinatura, traprazovigencia, to_char(traterminovigencia, 'dd/mm/YYYY') AS traterminovigencia, 
			    traprazoaditivadoexec, travlraditivo, travlrfinalobra, travlrqtdareaacresc, 
			    travlrqtdareafinal, travlrqtdareaalterada, trajustificativa, 
				tradtinclusao, trastatus, to_char(traterminoexec, 'dd/mm/YYYY') AS traterminoexec 
			FROM 
				obras.termoaditivo ta
			JOIN obras.tipotermoaditivo tta ON tta.ttaid = ta.ttaid
			WHERE 
				obrid = $obrid 
				AND trastatus = 'A'
				AND ta.ttaid = " . ADITIVO_PRAZO . "
			GROUP BY 
				ta.ttaid, tta.ttadsc, usucpf, umdidareaacresc, umdidareafinal, umdidareaalterada, 
			    obrid, tradsc, traseq, tradtassinatura, traprazovigencia, traterminovigencia, 
			    traprazoaditivadoexec, travlraditivo, travlrfinalobra, travlrqtdareaacresc, 
			    travlrqtdareafinal, travlrqtdareaalterada, trajustificativa, 
			    tradtinclusao, trastatus, traterminoexec
			ORDER BY 
				traseq DESC
			LIMIT 1;";
	$obTermoPrazo = $db->pegaUmObjeto($sql);
	
	// PRAZO/VALOR
	$sql = "SELECT 
				max(traid) AS traid, ta.ttaid, tta.ttadsc, usucpf, umdidareaacresc, umdidareafinal, umdidareaalterada, 
			    obrid, tradsc, traseq, tradtassinatura, traprazovigencia, to_char(traterminovigencia, 'dd/mm/YYYY') AS traterminovigencia, 
			    traprazoaditivadoexec, travlraditivo, travlrfinalobra, travlrqtdareaacresc, 
			    travlrqtdareafinal, travlrqtdareaalterada, trajustificativa, 
				tradtinclusao, trastatus, to_char(traterminoexec, 'dd/mm/YYYY') AS traterminoexec 
			FROM 
				obras.termoaditivo ta
			JOIN obras.tipotermoaditivo tta ON tta.ttaid = ta.ttaid
			WHERE 
				obrid = $obrid 
				AND trastatus = 'A'
				AND ta.ttaid = " . ADITIVO_PRAZO_VALOR . "
			GROUP BY 
				ta.ttaid, tta.ttadsc, usucpf, umdidareaacresc, umdidareafinal, umdidareaalterada, 
			    obrid, tradsc, traseq, tradtassinatura, traprazovigencia, traterminovigencia, 
			    traprazoaditivadoexec, travlraditivo, travlrfinalobra, travlrqtdareaacresc, 
			    travlrqtdareafinal, travlrqtdareaalterada, trajustificativa, 
			    tradtinclusao, trastatus, traterminoexec
			ORDER BY 
				traseq DESC
			LIMIT 1;";
	$obTermoPrazoValor = $db->pegaUmObjeto($sql);
	
	$obTermoDados = new stdClass();
	// Carregar dados de VALOR
	if ( intval($obTermoValor->traseq) > intval($obTermoPrazoValor->traseq) ){
		$obTermoDados->travlraditivo		= $obTermoValor->travlraditivo;	
		$obTermoDados->travlrfinalobra		= $obTermoValor->travlrfinalobra;	
		$obTermoDados->travlrqtdareaacresc	= $obTermoValor->travlrqtdareaacresc;	
		$obTermoDados->travlrqtdareafinal	= $obTermoValor->travlrqtdareafinal;	
	}elseif ($obTermoPrazoValor->traid){
		$obTermoDados->travlraditivo		= $obTermoPrazoValor->travlraditivo;	
		$obTermoDados->travlrfinalobra		= $obTermoPrazoValor->travlrfinalobra;	
		$obTermoDados->travlrqtdareaacresc	= $obTermoPrazoValor->travlrqtdareaalterada;	
		$obTermoDados->travlrqtdareafinal	= $obTermoPrazoValor->travlrqtdareafinal;	
	}
	// Carregar dados de PRAZO
	if ( intval($obTermoPrazo->traseq) > intval($obTermoPrazoValor->traseq) ){
		$obTermoDados->traprazovigencia		 = $obTermoPrazo->traprazovigencia;	
		$obTermoDados->traterminovigencia	 = $obTermoPrazo->traterminovigencia;	
		$obTermoDados->traprazoaditivadoexec = $obTermoPrazo->traprazoaditivadoexec;	
		$obTermoDados->traterminoexec		 = $obTermoPrazo->traterminoexec;	
	}elseif ($obTermoPrazoValor->traid){
		$obTermoDados->traprazovigencia		 = $obTermoPrazoValor->traprazovigencia;	
		$obTermoDados->traterminovigencia	 = $obTermoPrazoValor->traterminovigencia;	
		$obTermoDados->traprazoaditivadoexec = $obTermoPrazoValor->traprazoaditivadoexec;	
		$obTermoDados->traterminoexec		 = $obTermoPrazoValor->traterminoexec;	
	}
	
	if ($obTermoValor->traid > $obTermoPrazo->traid && $obTermoValor->traid > $obTermoPrazoValor->traid){
		$obTermoDados->traid  = $obTermoValor->traid;	
		$obTermoDados->tradsc = $obTermoValor->tradsc;	
	}elseif ($obTermoPrazo->traid > $obTermoValor->traid && $obTermoPrazo->traid > $obTermoPrazoValor->traid){
		$obTermoDados->traid  = $obTermoPrazo->traid;	
		$obTermoDados->tradsc = $obTermoPrazo->tradsc;	
	}else{
		$obTermoDados->traid  = $obTermoPrazoValor->traid;	
		$obTermoDados->tradsc = $obTermoPrazoValor->tradsc;	
	}
	
	return $campo ? $obTermoDados->{$campo} : $obTermoDados;
}

function apagaAditivo( $traid, $obrid = NULL ){
	global $db;
	
	if(isset($_SESSION["obra"]['obrid'])){
		$obrid = $_SESSION["obra"]['obrid'];
	}
	
	// Inativa o termo
	$sql = "UPDATE 
				obras.termoaditivo 
			SET 
				trastatus = 'I' 
			WHERE 
				traid={$traid}
				AND traseq = (SELECT MAX(traseq) FROM obras.termoaditivo WHERE trastatus = 'A' AND obrid={$obrid}) 
				AND obrid={$obrid}";
	
	$db->executar( $sql );
	// INATIVA a vigï¿½ncia (usado para o cï¿½lculo de execuï¿½ï¿½o) e o status dos itens
	$sql = "UPDATE obras.itenscomposicaoobra ico
			SET 
				icovigente = 'I',
				icostatus  = 'I' 
			WHERE 
				traid={$traid}
				AND obrid={$obrid}";	
	$db->executar( $sql );
	// ATIVA a vigï¿½ncia dos itens do ï¿½ltimo aditivo ativo ou do cronograma original
	$traidUlt   = pegaObUltimoAditivo('traid');
	$whereTraid = "traid" . ($traidUlt ? " = " . $traidUlt : " IS NULL ");
	$sql = "UPDATE obras.itenscomposicaoobra ico
			SET 
				icovigente = 'A' 
			WHERE 
				{$whereTraid}
				AND icostatus = 'A'
				AND obrid={$obrid}";	
	$db->executar( $sql );
	
	$db->commit();
}

//function duplicaObraAditivo( $obrid, Array $arrParam  ){
function salvaObraAditivo( $obrid, Array $arrParam  ){
	
	global $db;
	
	if ( $obrid ){
		$traidRef = $arrParam['traid'] ? $arrParam['traid'] : pegaObUltimoAditivo('traid');
		// Cadastra aditivo novo
		$traid = cadastraAditivo( $obrid, $arrParam );
//		atualizarObra( $obrid, $arrParam );
		duplicaCronograma($obrid, $traid, $traidRef, $arrParam);
		
		// atualizando o campo obrvlrrealobra da tabela obras.obrainfraestrutura
		$obrvlrrealobra = pegaObMaiorVlrAditivo();
		$sql = "UPDATE obras.obrainfraestrutura set obrvlrrealobra =".(($obrvlrrealobra)?"'".$obrvlrrealobra."'":"0")." WHERE obrid={$obrid}";
		
		$db->carregar($sql);
		
		$db->commit();
			
		return $obrid;	
	}
	
//	if( $obrid ){
//		
//		$obridnova = duplicaObra( $obrid, $arrParam );
//		
//		duplicaContatosObra( $obrid, $obridnova );
//		
//		duplicaLicitacao( $obrid, $obridnova );
//		
//		duplicaCronograma( $obrid, $obridnova, $arrParam );
//		
//		duplicaConvenio( $obrid, $obridnova );
//		
//		duplicaProjeto( $obrid, $obridnova );
//		
//		duplicaExecucao( $obrid, $obridnova );
//		duplicaRestricao( $obrid, $obridnova );
//		duplicaArquivos( $obrid, $obridnova );
//		atualizaRelacionados( $obrid, $obridnova );
////		duplicaAditivos( $obrid, $obridnova );
//		// Cadastra aditivo novo
//		cadastraAditivo( $obrid, $arrParam );
////		cadastraAditivo( $obridnova, $arrParam );
//		$db->commit();
//		
//		return $obridnova;
//	}
}

function atualizarObra( $obrid, Array $post ){
	global $db;
	
	$post['traterminovigencia'] = $post['traterminovigencia'] ? "'" . formata_data_sql($post['traterminovigencia']) . "'" : 'NULL';
	$post['traterminoexec']     = $post['traterminoexec'] 	  ? "'" . formata_data_sql($post['traterminoexec']) . "'" 	  : 'NULL';
	
	$post['travlraditivo'] 		   = $post['travlraditivo'] 		? Obras::MoedaToBd( $post['travlraditivo'] ) 		 : 'NULL';
	$post['travlrfinalobra'] 	   = $post['travlrfinalobra'] 		? Obras::MoedaToBd( $post['travlrfinalobra'] ) 		 : 'NULL';
	$post['travlrqtdareaacresc']   = $post['travlrqtdareaacresc'] 	? Obras::MoedaToBd( $post['travlrqtdareaacresc'] ) 	 : 'NULL';
	$post['travlrqtdareaalterada'] = $post['travlrqtdareaalterada'] ? Obras::MoedaToBd( $post['travlrqtdareaalterada'] ) : 'NULL';
	$post['travlrqtdareafinal']    = $post['travlrqtdareafinal'] 	? Obras::MoedaToBd( $post['travlrqtdareafinal'] ) 	 : 'NULL';
//	$post['travlrqtdareaalterada'] = $post['travlrqtdareaalterada'] ? Obras::MoedaToBd( $post['travlrqtdareaalterada'] ) : 'NULL';
	$post['traprazovigencia']	   = $post['traprazovigencia'] 		? Obras::MoedaToBd( $post['traprazovigencia'] ) 	 : 'NULL';
	$post['traprazoaditivadoexec'] = $post['traprazoaditivadoexec'] ? Obras::MoedaToBd( $post['traprazoaditivadoexec'] ) : 'NULL';

	foreach($post as $k => $v) $post[$k] = ($v ? $v : 'NULL');
	
	$sql = "UPDATE obras.obrainfraestrutura SET 
				obrprazovigenciaaditivo    = {$post['traprazovigencia']}, 
				obrterminocontratoaditivo  = {$post['traterminovigencia']}, 
            	obrprazoexecaditivodias    = {$post['traprazoaditivadoexec']}, 
            	obrterminoaditivo 		   = {$post['traterminoexec']}, 
            	obrcustocontratoaditivo    = {$post['travlraditivo']}, 
            	obrtotcustocontratoaditivo = {$post['travlrfinalobra']}, 
            	obrqtdconstruidaaditivo    = " . ($post['travlrqtdareaacresc'] != 'NULL' ? $post['travlrqtdareaacresc'] : $post['travlrqtdareaalterada']) . ", 
            	obrtotqtdconstruidaaditivo = {$post['travlrqtdareafinal']} 
			WHERE obrid = {$obrid};";
	
	$db->executar( $sql );
}

function duplicaObra( $obrid, Array $arrParam  ){
	
	global $db;

	$obrprazovigencia  = $arrParam['traprazovigencia'] ? "(" . $arrParam['traprazovigencia'] . " + obrprazovigencia)" : "obrprazovigencia";
	$dtterminocontrato = $arrParam['traterminovigencia'] ? "'" . formata_data_sql($arrParam['traterminovigencia']) . "'" : "dtterminocontrato";
	
	$obrprazoexec  	   = $arrParam['traprazoaditivadoexec'] ? "(" . $arrParam['traprazoaditivadoexec'] . " + obrprazoexec)" : "obrprazoexec";
	$obrdttermino      = $arrParam['traterminoexec'] ? "'" . formata_data_sql($arrParam['traterminoexec']) . "'" : "obrdttermino";
	
	$obrcustocontrato  = $arrParam['travlrfinalobra'] ? Obras::MoedaToBd( $arrParam['travlrfinalobra'] ) : "obrcustocontrato";
	$obrqtdconstruida  = $arrParam['travlrqtdareafinal'] ? Obras::MoedaToBd( $arrParam['travlrqtdareafinal'] ) : "obrqtdconstruida";
	
	// cria a nova obra
	$sql = "INSERT INTO obras.obrainfraestrutura( 
				tobraid, tpcoid, orgid, mdaid, endid, entidunidade, stoid, 
	            umdidobraconstruida, umdidareaserconstruida, umdidareaserreformada, 
	            umdidareaserampliada, obrdesc, obrdescundimplantada, obrdtinicio, 
	            obrdttermino, obrpercexec, obrcustocontrato, obrqtdconstruida, 
	            obrcustounitqtdconstruida, obrreaconstruida, obsobra, obsstatus, 
	            obsdtinclusao, entidempresaconstrutora, iexid, obrpercbdi, usucpf, 
	            entidcampus, obrdescfontefin, obrcomposicao, cloid, tpoid, prfid, 
	            obrdtinauguracao, obrdtprevinauguracao, obrstatusinauguracao, 
	            obrdtvistoria, sbaid, obrlincambiental, obraprovpatrhist, obrdtprevprojetos, 
	            obridorigem, numconvenio, ptpid, obrvalorprevisto, 
	            dtiniciocontrato, dtterminocontrato, obrprazoexec, obrdtordemservico, 
	            obrdtassinaturacontrato, molid, dtiniciolicitacao, dtfinallicitacao, 
	            licitacaouasg, numlicitacao, obrprazovigencia, obridaditivo, obridrelacionada 
            )( 
            	SELECT 
					tobraid, tpcoid, orgid, mdaid, endid, entidunidade, stoid, 
		            umdidobraconstruida, umdidareaserconstruida, umdidareaserreformada, 
		            umdidareaserampliada, obrdesc, obrdescundimplantada, obrdtinicio, 
		            $obrdttermino, obrpercexec, $obrcustocontrato, $obrqtdconstruida, 
		            obrcustounitqtdconstruida, obrreaconstruida, obsobra, obsstatus, 
		            obsdtinclusao, entidempresaconstrutora, iexid, obrpercbdi, usucpf, 
		            entidcampus, obrdescfontefin, obrcomposicao, cloid, tpoid, prfid, 
		            obrdtinauguracao, obrdtprevinauguracao, obrstatusinauguracao, 
		            null, sbaid, obrlincambiental, obraprovpatrhist, obrdtprevprojetos, 
		            obridorigem, numconvenio, ptpid, obrvalorprevisto, 
		            dtiniciocontrato, $dtterminocontrato, $obrprazoexec, obrdtordemservico, 
		            obrdtassinaturacontrato, molid, dtiniciolicitacao, dtfinallicitacao, 
		            licitacaouasg, numlicitacao, $obrprazovigencia, ".$obrid.", obridrelacionada 
            	FROM 
            		obras.obrainfraestrutura 
            	WHERE obrid = ".$obrid." 
            ) returning obrid";
		            
	$obridnova = $db->pegaUm( $sql );
	
	// inativa a obra antiga
	$sql = "UPDATE obras.obrainfraestrutura SET obsstatus = 'I' WHERE obrid = {$obrid}";
	$db->executar( $sql );
	
	return $obridnova;
					
}

function duplicaContatosObra( $obrid, $obridnova ){
	
	global $db;
	
	$sql = "SELECT recoid FROM obras.responsavelobra WHERE obrid = {$obrid}";
	
	$contatos = $db->carregarColuna( $sql );
	
	if( is_array( $contatos ) ){
		
		foreach( $contatos as $valor ){

	  		// Cria o relacionamento entre o responsï¿½vel e a obra
	  		$sql = "INSERT INTO obras.responsavelobra (recoid, obrid)
					VALUES ({$valor}, {$obridnova})";

		  	$db->executar( $sql );

	 	}
		
	}
	
}

function duplicaLicitacao( $obrid, $obridnova ){
	
	global $db;
			
	// duplica a licitaï¿½ï¿½o
	
	$sql = "SELECT tflid, flcpubleditaldtprev, flcaberpropdtprev, 
            flcrecintermotivo, flchomlicdtprev, flcordservdt, flcordservnum, 
           	flcstatus, flcdtinclusao, flcdtrecintermotivo, flcdata FROM obras.faselicitacao WHERE obrid = ".$obrid;
	
		foreach( $db->carregar( $sql ) as $dados ){
			foreach( $dados as $campo=>$valor ){
				if( !is_array( $dados[$campo] ) ){
					if( $valor == "" ){
						$dados[$campo] = 'NULL';
					} else {
						$dados[$campo] = "'" . pg_escape_string(trim($valor))  .  "'";
					}
				}
			}
			$sql = "INSERT INTO
						obras.faselicitacao( tflid, obrid, flcpubleditaldtprev, flcaberpropdtprev, 
		           	flcrecintermotivo, flchomlicdtprev, flcordservdt, flcordservnum, 
	    	       	flcstatus, flcdtinclusao, flcdtrecintermotivo, flcdata)
	    			
	           		VALUES
					
		           	( ".$dados['tflid'].", ".$obridnova.", ".$dados['flcpubleditaldtprev'].", ".$dados['flcaberpropdtprev'].", 
	           	".$dados['flcrecintermotivo'].", ".$dados['flchomlicdtprev'].", ".$dados['flcordservdt'].", ".$dados['flcordservnum'].", 
    	       	".$dados['flcstatus'].", ".$dados['flcdtinclusao'].", ".$dados['flcdtrecintermotivo'].", ".$dados['flcdata']." )";
	
			$db->executar( $sql );
		}
}

function duplicaCronograma($obrid, $traid, $traidRef = null, Array $arrParam){
	global $db;
	
//	1 => Prazo
//	3 => Prazo/Valor  
//	$arrTtaid = array(1, 3);
//    if ( in_array($arrParam['ttaid'], $arrTtaid) ){
//		$campos = ", icopercprojperiodo, icopercsobreobra, icovlritem, icopercexecutado, icodtinicioitem, icodterminoitem";  	
//    } 

	$travlrfinalobra  = pegaObMaiorVlrAditivo();
	if ( $traid != $traidRef ):
		$icopercsobreobra = ($travlrfinalobra ? "(icovlritem::numeric / $travlrfinalobra * 100)" : "icopercsobreobra::numeric"); 
		
		$traidWhere = "AND traid" . ($traidRef ? " = " . $traidRef : " IS NULL ");
		
		$campos = ", icopercprojperiodo, $icopercsobreobra, icovlritem, round(COALESCE((sup.supvlrinfsupervisor::numeric * ($icopercsobreobra / 100)), 0),2), icodtinicioitem, icodterminoitem";  	
		$sql = "INSERT INTO obras.itenscomposicaoobra(
		            itcid, obrid, icostatus, icodtinclusao, icoordem, traid, 
		            icopercprojperiodo, icopercsobreobra, icovlritem, icopercexecutado, icodtinicioitem, icodterminoitem
	            )(
	            	SELECT 
	            		ico.itcid, obrid, icostatus, now(), icoordem, {$traid}
	            		$campos
				  	FROM obras.itenscomposicaoobra ico
				  	LEFT JOIN (SELECT 
							  	supvlrinfsupervisor,
							  	ito.itcid  
							  FROM
							  	obras.supervisao s 
							  JOIN obras.supervisaoitenscomposicao sup ON sup.supvid = s.supvid
							  JOIN obras.itenscomposicaoobra ito ON ito.icoid = sup.icoid
							  WHERE	
							  	s.supvid IN (SELECT MAX(supvid) FROM obras.supervisao WHERE supstatus = 'A' AND obrid = {$obrid})) sup ON sup.itcid = ico.itcid
				  	
				  	WHERE
				  		obrid = {$obrid}
				  		{$traidWhere}
	            );";

		$db->executar( $sql );
			  			
		$sqlItens = "UPDATE obras.itenscomposicaoobra
					 SET 
					 	icovigente = 'I'
					 WHERE
					 	obrid = {$obrid}
						AND traid " . ($traidRef ? " = " . $traidRef : " IS NULL ") . ";";
		$db->executar( $sqlItens );	  			
	else:
		$travlrfinalobra  = $travlrfinalobra ? $travlrfinalobra : Obras::MoedaToBd( $arrParam['obrcustocontrato'] );
		$icopercsobreobra = "(icovlritem / $travlrfinalobra * 100)";
		
//		$sql = "UPDATE obras.itenscomposicaoobra it
//				SET 
//					icopercsobreobra = $icopercsobreobra,
//					icopercexecutado = (a.supvlrinfsupervisor * ($icopercsobreobra / 100))
//				FROM (
//					SELECT 
//					 ito.itcid,
//					 supvlrinfsupervisor  
//					FROM
//						obras.supervisao s 
//					JOIN obras.supervisaoitenscomposicao sup ON sup.supvid = s.supvid
//					JOIN obras.itenscomposicaoobra ito ON ito.icoid = sup.icoid
//					WHERE	
//						s.supvid IN (SELECT MAX(supvid) FROM obras.supervisao WHERE supstatus = 'A' AND obrid = {$obrid})
//				) AS a
//				WHERE 
//					it.itcid = a.itcid
//					AND obrid = {$obrid} 
//					AND traid = {$traid};";
		$sql = "UPDATE obras.itenscomposicaoobra it
				SET 
					icopercsobreobra = $icopercsobreobra,
					icopercexecutado = (( 	SELECT 
											 supvlrinfsupervisor  
											FROM
												obras.supervisao s 
											JOIN obras.supervisaoitenscomposicao sup ON sup.supvid = s.supvid
											JOIN obras.itenscomposicaoobra ito ON ito.icoid = sup.icoid
											WHERE	
												s.supvid IN (SELECT MAX(supvid) FROM obras.supervisao WHERE supstatus = 'A' AND obrid = 11477) 
												AND ito.itcid = it.itcid) * ($icopercsobreobra / 100))
				WHERE 
					obrid = {$obrid} 
					AND traid = {$traid};";
		$db->executar( $sql );
	endif;
}

function duplicaConvenio( $obrid, $obridnova){
	
	global $db;
			
	// duplica o convenio
	$sql = "SELECT frpid, obrid, frrconventbenef, frrconvnum, frrconvobjeto, 
            frrconvvlr, frrconvvlrconcedente, frrconvvlrconcenente, frrdescinstituicao, 
            frrdescnumport, frrdescobjeto, frrdescvlr, frrdescdtviginicio, 
            frrdescdtvigfinal, frrstatus, frrdtinclusao, frrobsrecproprio, 
            covid FROM obras.formarepasserecursos WHERE obrid = ".$obrid;
	
		foreach( $db->carregar( $sql ) as $dados ){
			foreach( $dados as $campo=>$valor ){
				if( !is_array( $dados[$campo] ) ){
					if( $valor == "" ){
						$dados[$campo] = 'NULL';
					} else {
						$dados[$campo] = "'" . pg_escape_string(trim($valor))  .  "'";
					}
				}
			}

			$sql = "INSERT INTO obras.formarepasserecursos(
		            frpid, obrid, frrconventbenef, frrconvnum, frrconvobjeto, 
		            frrconvvlr, frrconvvlrconcedente, frrconvvlrconcenente, frrdescinstituicao, 
		            frrdescnumport, frrdescobjeto, frrdescvlr, frrdescdtviginicio, 
		            frrdescdtvigfinal, frrstatus, frrdtinclusao, frrobsrecproprio, 
		            covid)
		            		
	           		VALUES
					
		           	( ".$dados['frpid'].", ".$obridnova.", ".$dados['frrconventbenef'].", ".$dados['frrconvnum'].", ".$dados['frrconvobjeto'].", 
	           	".$dados['frrconvvlr'].", ".$dados['frrconvvlrconcedente'].", ".$dados['frrconvvlrconcenente'].", ".$dados['frrdescinstituicao'].", 
    	       	".$dados['frrdescnumport'].", ".$dados['frrdescobjeto'].", ".$dados['frrdescvlr'].", ".$dados['frrdescdtviginicio'].",
	           	".$dados['frrdescdtvigfinal'].", ".$dados['frrstatus'].", ".$dados['frrdtinclusao'].", ".$dados['frrobsrecproprio'].", ".$dados['covid']." )"; 
	
			$db->executar( $sql );
		}
}

function duplicaProjeto( $obrid, $obridnova ){
	
	global $db;
			
	// duplica o projeto
	
	
	$sql = "SELECT fprid, tfpid, obrid, tpaid, felid, fprvlrformaelabrecproprio, 
            fprvlrformaelabrrecrepassado, fprdtiniciofaseprojeto, fprdtconclusaofaseprojeto, 
            fprobsprojcontrapartida, fprvlrprojcontratadorecrepassad, fprvlrprojcontratadorecproprio, 
            fprobsexecdireta, fprstatus, fprdtinclusao, fprdtprevterminoprojeto
            FROM obras.faseprojeto WHERE obrid = ".$obrid." AND fprstatus = 'A'";
	
	$dadosProjeto = $db->carregar( $sql );
	
	if( is_array( $dadosProjeto ) ){
		foreach( $db->carregar( $sql ) as $dados ){
			foreach( $dados as $campo=>$valor ){
				if( !is_array( $dados[$campo] ) ){
					if( $valor == "" ){
						$dados[$campo] = 'NULL';
					} else {
						$dados[$campo] = "'" . pg_escape_string(trim($valor))  .  "'";
					}
				}
			}
			$sql = "INSERT INTO obras.faseprojeto(
		            tfpid, obrid, tpaid, felid, fprvlrformaelabrecproprio, 
		            fprvlrformaelabrrecrepassado, fprdtiniciofaseprojeto, fprdtconclusaofaseprojeto, 
		            fprobsprojcontrapartida, fprvlrprojcontratadorecrepassad, fprvlrprojcontratadorecproprio, 
		            fprobsexecdireta, fprstatus, fprdtinclusao, fprdtprevterminoprojeto)
		            		
	           		VALUES
					
		           	( ".$dados['tfpid'].", ".$obridnova.", ".$dados['tpaid'].", ".$dados['felid'].", ".$dados['fprvlrformaelabrecproprio'].", 
	           	".$dados['fprvlrformaelabrrecrepassado'].", ".$dados['fprdtiniciofaseprojeto'].", ".$dados['fprdtconclusaofaseprojeto'].",
	           	".$dados['fprobsprojcontrapartida'].", ".$dados['fprvlrprojcontratadorecrepassad'].", ".$dados['fprvlrprojcontratadorecproprio'].",
	           	".$dados['fprobsexecdireta'].", ".$dados['fprstatus'].", ".$dados['fprdtinclusao'].", ".$dados['fprdtprevterminoprojeto']." ) returning fprid"; 
	
			$fprid = $db->pegaUm( $sql );
			
			$sql = "SELECT tflid, fprid, tfpdtfase, tfpnumos, tfpobsmotivo, tfpstatus, tfpdtinclusao
					FROM obras.faselicitacaoprojetos WHERE fprid = ".$dados['fprid']." AND tfpstatus = 'A'";
			
			if( is_array($novo = $db->carregar( $sql ) ) ){
				foreach( $novo as $dados2 ){
					foreach( $dados2 as $campo=>$valor ){
						if( !is_array( $dados2[$campo] ) ){
							if( $valor == "" ){
								$dados2[$campo] = 'NULL';
							} else {
								$dados2[$campo] = "'" . pg_escape_string(trim($valor))  .  "'";
							}
						}
					}
					
					$sql = "INSERT INTO obras.faselicitacaoprojetos(
							tflid, fprid, tfpdtfase, tfpnumos, tfpobsmotivo, tfpstatus, tfpdtinclusao)
							
							VALUES
							
							(".$dados2['tflid'].", ".$fprid.", ".$dados2['tfpdtfase'].", ".$dados2['tfpnumos'].", ".$dados2['tfpobsmotivo'].", 
		           			".$dados2['tfpstatus'].", ".$dados2['tfpdtinclusao'].")";
				}
			}
		}
	}
}

function duplicaExecucao( $obrid, $obridnova ){
	
	global $db;
			
	// duplica a Execuï¿½ï¿½o Orcamentaria
		
	$sql = "SELECT eorid, obrid, usucpf, teoid, eocvlrcusteio, eocvlrcapital, eocstatus, eocdtinclusao
  			FROM obras.execucaoorcamentaria WHERE obrid = ".$obrid." AND eocstatus = 'A'";
	
	$dadosExecucao = $db->carregar( $sql );
	
	if ( is_array($dadosExecucao) ){
		
		foreach( $dadosExecucao as $dados ){
			foreach( $dados as $campo=>$valor ){
				if( !is_array( $dados[$campo] ) ){
					if( $valor == "" ){
						$dados[$campo] = 'NULL';
					} else {
						$dados[$campo] = "'" . pg_escape_string(trim($valor))  .  "'";
					}
				}
			}
			$sql = "INSERT INTO obras.execucaoorcamentaria(
		            obrid, usucpf, teoid, eocvlrcusteio, eocvlrcapital, eocstatus, eocdtinclusao)
		            		
	           		VALUES
					
		           	( ".$obridnova.", ".$dados['usucpf'].", ".$dados['teoid'].", ".$dados['eocvlrcusteio'].", 
	           		".$dados['eocvlrcapital'].", ".$dados['eocstatus'].", ".$dados['eocdtinclusao']." ) returning eorid"; 
	
			$eorid = $db->pegaUm( $sql );
			
			$sql = "SELECT ideid, eorid, usucpf, eocvlrempenhado, eocvlrliquidado, eocdtposicao, eocdtinclusao
  					FROM obras.itensexecucaoorcamentaria WHERE eorid = ".$dados['eorid'];

			
			
			if( is_array($novo = $db->carregar( $sql ) ) ){
				foreach( $novo as $dados2 ){
					foreach( $dados2 as $campo=>$valor ){
						if( !is_array( $dados2[$campo] ) ){
							if( $valor == "" ){
								$dados2[$campo] = 'NULL';
							} else {
								$dados2[$campo] = "'" . pg_escape_string(trim($valor))  .  "'";
							}
						}
					}
					
					$sql = "INSERT INTO obras.itensexecucaoorcamentaria(
							eorid, usucpf, eocvlrempenhado, eocvlrliquidado, eocdtposicao, eocdtinclusao)
							
							VALUES
							
							( ".$eorid.", ".$dados2['usucpf'].", ".$dados2['eocvlrempenhado'].", ".$dados2['eocvlrliquidado'].", 
		           			".$dados2['eocdtposicao'].", ".$dados2['eocdtinclusao'].")";

					$db->executar( $sql );
				}
			}
		}
	}
}

function duplicaRestricao( $obrid, $obridnova ){
	
	global $db;
			
	// duplica a Restricao orcamentaria
	
	$sql = "SELECT rstoid, obrid, trtid, rstdesc, rstdtprevisaoregularizacao, rstdescprovidencia, 
       			rstdtsuperacao, rstsituacao, rststatus, rstdtinclusao, usucpf, 
       			fsrid
  			FROM obras.restricaoobra WHERE obrid = ".$obrid." AND rststatus = 'A'";
	
	$dadosRestricao = $db->carregar( $sql ); 
	
	if( is_array( $dadosRestricao ) ){
		foreach( $dadosRestricao as $dados ){
			foreach( $dados as $campo=>$valor ){
				if( !is_array( $dados[$campo] ) ){
					if( $valor == "" ){
						$dados[$campo] = 'NULL';
					} else {
						$dados[$campo] = "'" . pg_escape_string(trim($valor))  .  "'";
					}
				}
			}
			$sql = "INSERT INTO obras.restricaoobra(
		            obrid, trtid, rstdesc, rstdtprevisaoregularizacao, rstdescprovidencia, 
	       			rstdtsuperacao, rstsituacao, rststatus, rstdtinclusao, usucpf, 
	       			fsrid)
		            		
	           		VALUES
					
		           	( ".$obridnova.", ".$dados['trtid'].", ".$dados['rstdesc'].", ".$dados['rstdtprevisaoregularizacao'].", ".$dados['rstdescprovidencia'].", 
	      	     	".$dados['rstdtsuperacao'].", ".$dados['rstsituacao'].", ".$dados['rststatus'].", ".$dados['rstdtinclusao'].", 
	   	   	    	".$dados['usucpf'].", ".$dados['fsrid']." )"; 
	
			$db->executar( $sql );
		}
	}
	
}


function duplicaArquivos( $obrid, $obridnova ){
	
	global $db;
			
	// duplica os Arquivos
	
	$sql = "SELECT aqoid, obrid, tpaid, arqid, usucpf, aqodtinclusao, aqostatus
  			FROM obras.arquivosobra WHERE obrid = ".$obrid." AND aqostatus = 'A'";
	
	$dadosArquivo = $db->carregar( $sql );
	
	if( is_array($dadosArquivo) ){
		foreach( $dadosArquivo as $dados ){
			foreach( $dados as $campo=>$valor ){
				if( !is_array( $dados[$campo] ) ){
					if( $valor == "" ){
						$dados[$campo] = 'NULL';
					} else {
						$dados[$campo] = "'" . pg_escape_string(trim($valor))  .  "'";
					}
				}
			}
			$sql = "INSERT INTO obras.arquivosobra(
		            obrid, tpaid, arqid, usucpf, aqodtinclusao, aqostatus)
		            		
	           		VALUES
					
		           	( ".$obridnova.", ".$dados['tpaid'].", ".$dados['arqid'].", ".$dados['usucpf'].",
		           	".$dados['aqodtinclusao'].", ".$dados['aqostatus']." )"; 
	
			$db->executar( $sql );
		}
	}
}

function atualizaRelacionados( $obrid, $obridnova ){
	
	global $db;
			
	// atualiza as obras relacionadas 
	// OBS: Como conversado com o Mario, as obras relacionadas serï¿½o atualizadas para a nova obra, e nï¿½o duplicadas. Sendo assim
	// as obras antigas nï¿½o terï¿½o mais obras relacionadas a elas. (05/04/2010)
	
//	$sql = "SELECT obrid FROM obras.obrainfraestrutura WHERE obridrelacionada = ".$obrid." AND obsstatus = 'A'";
//	foreach( $db->carregar( $sql ) as $dados ){
	$sql = "UPDATE obras.obrainfraestrutura SET 
				obridrelacionada = ".$obridnova."
			WHERE obrid IN (SELECT obrid 
			 				 FROM obras.obrainfraestrutura 
			 				 WHERE obridrelacionada = ".$obrid." AND obsstatus = 'A') "; 

	$db->executar( $sql );
//	}
}

function duplicaAditivos( $obrid, $obridnova ){
	
	global $db;
			
	// duplica aditivos
	
	$sql = "SELECT ttaid, usucpf, umdidareaacresc, umdidareafinal, umdidareaalterada, 
		       obrid, tradsc, traseq, tradtassinatura, traprazovigencia, traterminovigencia, 
		       traprazoaditivadoexec, traterminoexec, travlraditivo, travlrfinalobra, 
		       travlrqtdareaacresc, travlrqtdareafinal, travlrqtdareaalterada, 
		       trajustificativa, tradtinclusao, trastatus
		  FROM obras.termoaditivo WHERE obrid = ".$obrid." AND trastatus = 'A'";
	
		
		$arrDados = $db->carregar( $sql );
		$arrDados = $arrDados ? $arrDados : array();
		
		foreach( $arrDados as $dados ){
			foreach( $dados as $campo=>$valor ){
				if( !is_array( $dados[$campo] ) ){
					if( $valor == "" ){
						$dados[$campo] = 'NULL';
					} else {
						$dados[$campo] = "'" . pg_escape_string(trim($valor))  .  "'";
					}
				}
			}
			$sql = "INSERT INTO obras.termoaditivo( 
					   ttaid, usucpf, umdidareaacresc, umdidareafinal, umdidareaalterada, 
				       obrid, tradsc, traseq, tradtassinatura, traprazovigencia, traterminovigencia, 
				       traprazoaditivadoexec, traterminoexec, travlraditivo, travlrfinalobra, 
				       travlrqtdareaacresc, travlrqtdareafinal, travlrqtdareaalterada, 
				       trajustificativa, tradtinclusao, trastatus
			        )VALUES( 
			           	".$dados['ttaid'].", ".$dados['usucpf'].", ".$dados['umdidareaacresc'].", ".$dados['umdidareafinal'].", ".$dados['umdidareaalterada'].", 
			           	".$obridnova.", ".$dados['tradsc'].", ".$dados['traseq'].", ".$dados['tradtassinatura'].", ".$dados['traprazovigencia'].", ".$dados['traterminovigencia'].", 
		    	       	".$dados['traprazoaditivadoexec'].", ".$dados['traterminoexec'].", ".$dados['travlraditivo'].", ".$dados['travlrfinalobra'].",
		    	       	".$dados['travlrqtdareaacresc'].", ".$dados['travlrqtdareafinal'].", ".$dados['travlrqtdareaalterada'].", ".$dados['trajustificativa'].",
		    	       	".$dados['tradtinclusao'].", ".$dados['trastatus']." 
		    	    )";
			
			$db->executar( $sql );
		}
}

function cadastraAditivo($obridnova, $post){
	global $db;
	
	$post['umdidareaacresc']    = $post['umdidareaacresc'] ? $post['umdidareaacresc'] : 'NULL';
	$post['umdidareaalterada']  = $post['umdidareaalterada'] ? $post['umdidareaalterada'] : 'NULL';
	$post['umdidareafinal']  	= $post['umdidareafinal'] ? $post['umdidareafinal'] : 'NULL';
	
	$post['tradtassinatura']    = $post['tradtassinatura'] ? "'" . formata_data_sql($post['tradtassinatura']) . "'" : 'NULL';
	$post['traterminovigencia'] = $post['traterminovigencia'] ? "'" . formata_data_sql($post['traterminovigencia']) . "'" : 'NULL';
	$post['traterminoexec']     = $post['traterminoexec'] ? "'" . formata_data_sql($post['traterminoexec']) . "'" : 'NULL';
	
	$post['travlraditivo'] 		   = $post['travlraditivo'] ? Obras::MoedaToBd( $post['travlraditivo'] ) : 'NULL';
	$post['travlrfinalobra'] 	   = $post['travlrfinalobra'] ? Obras::MoedaToBd( $post['travlrfinalobra'] ) : 'NULL';
	$post['travlrqtdareaacresc']   = $post['travlrqtdareaacresc'] ? Obras::MoedaToBd( $post['travlrqtdareaacresc'] ) : 'NULL';
	$post['travlrqtdareafinal']    = $post['travlrqtdareafinal'] ? Obras::MoedaToBd( $post['travlrqtdareafinal'] ) : 'NULL';
	$post['travlrqtdareaalterada'] = $post['travlrqtdareaalterada'] ? Obras::MoedaToBd( $post['travlrqtdareaalterada'] ) : 'NULL';
	$post['traprazovigencia']	   = $post['traprazovigencia'] ? Obras::MoedaToBd( $post['traprazovigencia'] ) : 'NULL';
	$post['traprazoaditivadoexec'] = $post['traprazoaditivadoexec'] ? Obras::MoedaToBd( $post['traprazoaditivadoexec'] ) : 'NULL';

	$post['trajustificativa'] = $post['trajustificativa'] ? "'" . htmlspecialchars( $post['trajustificativa'] ) . "'" : 'NULL';
	$post['tradsc'] 		  = $post['tradsc'] ? "'" . htmlspecialchars( $post['tradsc'] ) . "'" : 'NULL';
	$post['trasupressao'] 	  = $post['trasupressao'] == 'S' ? "'S'" : "'N'";
	
	foreach($post as $k => $v) $post[$k] = ($v ? $v : 'NULL');
	
	if ( !is_numeric($post['traid']) ){
		$sql = "INSERT INTO obras.termoaditivo(
		            ttaid, usucpf, umdidareaacresc, umdidareafinal, umdidareaalterada, 
		            obrid, tradsc, traseq, tradtassinatura, traprazovigencia, traterminovigencia, 
		            traprazoaditivadoexec, traterminoexec, travlraditivo, travlrfinalobra, 
		            travlrqtdareaacresc, travlrqtdareafinal, travlrqtdareaalterada, 
		            trajustificativa, trasupressao, tradtinclusao, trastatus
	            ) VALUES (
		    		{$post['ttaid']}, '" . $_SESSION['usucpf'] . "', {$post['umdidareaacresc']}, {$post['umdidareafinal']}, {$post['umdidareaalterada']}, 
		            {$obridnova}, {$post['tradsc']}, {$post['traseq']}, {$post['tradtassinatura']}, {$post['traprazovigencia']}, {$post['traterminovigencia']}, 
		            {$post['traprazoaditivadoexec']}, {$post['traterminoexec']}, {$post['travlraditivo']}, {$post['travlrfinalobra']}, 
		            {$post['travlrqtdareaacresc']}, {$post['travlrqtdareafinal']}, {$post['travlrqtdareaalterada']}, 
		            {$post['trajustificativa']}, {$post['trasupressao']}, now(), 'A'
		        ) RETURNING traid;";
		            
		$traid = $db->pegaUm( $sql );
	}else{
		// Limpa os valores atuais para atualizar com os novos
		$sql = "UPDATE obras.termoaditivo
			    SET umdidareaacresc=null, umdidareafinal=null, 
			        umdidareaalterada=null, traprazovigencia=null, traterminovigencia=null, 
			        traprazoaditivadoexec=null, travlraditivo=null, travlrfinalobra=null, 
			        travlrqtdareaacresc=null, travlrqtdareafinal=null, travlrqtdareaalterada=null, 
			        trajustificativa=null, traterminoexec=null
			 	WHERE traid={$post['traid']};";
		
		$db->executar( $sql );
		
		// Atualiza para valores novos
		$sql = "UPDATE obras.termoaditivo
			    SET ttaid={$post['ttaid']}, usucpf='" . $_SESSION['usucpf'] . "', umdidareaacresc={$post['umdidareaacresc']}, umdidareafinal={$post['umdidareafinal']}, 
			       umdidareaalterada={$post['umdidareaalterada']}, tradsc={$post['tradsc']}, traseq={$post['traseq']}, tradtassinatura={$post['tradtassinatura']}, 
			       traprazovigencia={$post['traprazovigencia']}, traterminovigencia={$post['traterminovigencia']}, traprazoaditivadoexec={$post['traprazoaditivadoexec']}, 
			       travlraditivo={$post['travlraditivo']}, travlrfinalobra={$post['travlrfinalobra']}, travlrqtdareaacresc={$post['travlrqtdareaacresc']}, travlrqtdareafinal={$post['travlrqtdareafinal']}, 
			       travlrqtdareaalterada={$post['travlrqtdareaalterada']}, trajustificativa={$post['trajustificativa']}, traterminoexec={$post['traterminoexec']}, trasupressao={$post['trasupressao']}
			 	WHERE traid={$post['traid']};";
		
		$db->executar( $sql );
		$traid = $post['traid'];
	}
	
	return $traid;
}

function obras_busca_campus( $entid, $orgid ){
	
	global $db;
	
	switch( $orgid ) {
		case ORGAO_SESU:
        	$funid = $db->pegaUm("SELECT funid FROM entidade.entidade e 
        						  INNER JOIN entidade.funcaoentidade ef ON e.entid = ef.entid 
        						  WHERE	e.entid = '".$entid."' AND ef.funid = '".ID_UNIVERSIDADE."'");
        break;
		case ORGAO_SETEC:
        	$funid = $db->pegaUm("SELECT funid FROM entidade.entidade e 
        						  INNER JOIN entidade.funcaoentidade ef ON e.entid = ef.entid 
        						  WHERE	e.entid = '".$entid."' AND ef.funid IN('".ID_ESCOLAS_TECNICAS."','".ID_ESCOLAS_AGROTECNICAS."')");
    	break;
	}
	
	switch($funid) {
		case ID_UNIVERSIDADE:
			$sql = "SELECT e.entid as codigo, entnome as descricao 
					FROM entidade.entidade e 
					INNER JOIN entidade.funcaoentidade ef ON ef.entid = e.entid 
					INNER JOIN entidade.funentassoc ea ON ea.fueid = ef.fueid
					WHERE ea.entid = {$entid} AND funid = '".ID_CAMPUS."' OR ea.entid = {$entid} AND funid = '".ID_REITORIA."' AND entstatus = 'A' ORDER BY e.entnome ASC";
		break;
		case ID_ESCOLAS_TECNICAS:
		case ID_ESCOLAS_AGROTECNICAS:
			$sql = "SELECT e.entid as codigo, entnome as descricao 
					FROM entidade.entidade e 
					INNER JOIN entidade.funcaoentidade ef ON ef.entid = e.entid 
					INNER JOIN entidade.funentassoc ea ON ea.fueid = ef.fueid
					WHERE ea.entid = {$entid} AND funid IN('".ID_UNED."') OR ea.entid = {$entid} AND funid = '".ID_REITORIA."' AND e.entstatus = 'A' ORDER BY e.entnome ASC";
			
		break;
	}
	
	if($sql) {
		
		$cm = $db->carregar($sql);
		
		if($cm[0]) {
			
			$combo = '<select class="CampoEstilo" id="entidcampus" name="entidcampus">';
			
			foreach($cm as $en) {
				$combo .= '<option value="' . $en['codigo'] . '">' . $en['descricao'] . '</option>';
			}
			
			$combo .= '</select>';
			
		}
	}

	return $combo;
	
}

function importarObras(){
	
	global $db;
	
	$sql   = "SELECT * FROM obras.importacaoobras WHERE status = 'N'";
	$obras = $db->carregar( $sql );
		
	for ( $i = 0; $i < count($obras); $i++ ){
		
		$sql = "SELECT muncod, mundescricao FROM territorios.municipio 
				WHERE mundescricao ilike '{$obras[$i]["municipio"]}'";
		
		$muncod = $db->pegaUm( $sql );

		$sql = "INSERT INTO entidade.endereco (estuf, muncod) 
				VALUES ('{$obras[$i]["uf"]}', '{$muncod}') returning endid";
		
		$endid = $db->pegaUm( $sql );

		$db->commit();
		
		$obrdesc = $obras[$i]["numconvenio"] . ' - ' . 
				   $obras[$i]["escola"] . ' - ' . 
				   $obras[$i]["programa"] . ' - ' . 
				   $obras[$i]["acao"] . ' - ' .
				   $obras[$i]["municipio"] . '/' . 
				   $obras[$i]["uf"];

		if ( !empty($obras[$i]["entidunidade"]) ){
			$sql = "INSERT INTO obras.obrainfraestrutura (orgid,
														  entidunidade,
														  obrdesc,
														  endid,
														  obsstatus,
														  obsdtinclusao,
														  usucpf,
														  tobraid,
														  numconvenio,
														  prfid,
														  cloid,
														  obrnumprocessoconv,
														  obranoconvenio 
														  )
					VALUES (3, 
							{$obras[$i]["entidunidade"]},
							'{$obrdesc}',
							{$endid},
							'A',
							'now',
							'{$_SESSION["usucpf"]}',
							{$obras[$i]["tipoobra"]},
							'{$obras[$i]["numconvenio"]}',
							{$obras[$i]["programafonte"]},
							{$obras[$i]["classificacao"]},
							'".trim($obras[$i]["numprocesso"])."',
							'".trim($obras[$i]["anoconvenio"])."')";
	
			$db->executar( $sql );
			
		}
	}
	
	$sql = "UPDATE obras.importacaoobras SET status = 'I' WHERE status = 'N'";
	$db->executar( $sql );
	
	$db->commit();
	$db->sucesso("principal/importacao");
	
}


function obrasSqlExecFinanceira(){
	
	$where = array();
	
	extract($_REQUEST);
	
	// tipo de ensino
	if( $orgid ){
		array_push($where, " oi.orgid in (" . implode( ',', $orgid ) . ") ");
	}
	
	// regiï¿½o
	if( $regiao[0] && $regiao_campo_flag ){
		array_push($where, " re.regcod " . (!$regiao_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $regiao ) . "') ");
	}
	
	// mesoregiï¿½o
	if( $mesoregiao[0] && $mesoregiao_campo_flag ){
		array_push($where, " me.mescod " . (!$mesoregiao_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $mesoregiao ) . "') ");
	}
	
	// UF
	if( $uf[0] && $uf_campo_flag ){
		array_push($where, " ed.estuf " . (!$uf_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( ',', $uf ) . "') ");
	}
	
	// grupo municipio
	if( $grupomun[0]  && $grupomun_campo_flag ){
		array_push($where, " gt.gtmid " . (!$grupomun_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $grupomun ) . "') ");
	}
	
	// tipo municipio
	if( $tipomun[0]  && $tipomun_campo_flag ){
		array_push($where, " tpm.tpmid " . (!$tipomun_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $tipomun ) . "') ");
	}
	
	// municipio
	if( $municipio[0]  && $municipio_campo_flag ){
		array_push($where, " ed.muncod " . (!$municipio_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ");
	}
	
	// unidade
	if( $unidade[0] && $unidade_campo_flag ){
		array_push($where, " oi.entidunidade " . (!$unidade_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $unidade ) . ") ");
	}
	
	// entidcampus
	if( $entidcampus[0] && $entidcampus_campo_flag ){
		array_push($where, " oi.entidcampus " . (!$entidcampus_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $entidcampus ) . ") ");
	}
	
	// programa fonte
	if( $prfid[0] && $prfid_campo_flag ){
		if ( !$prfid_campo_excludente ){
			array_push($where, " oi.prfid  IN (" . implode( ',', $prfid ) . ") ");	
		}else{
			array_push($where, " ( oi.prfid  NOT IN (" . implode( ',', $prfid ) . ") OR oi.prfid is null ) ");
		}
		
	}
	
	// tipologia da obra
	if( $tpoid[0] && $tpoid_campo_flag ){
		array_push($where, " oi.tpoid " . (!$tpoid_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $tpoid ) . ") ");
	}
	
	// classificaï¿½ï¿½o da obra
	if( $cloid[0] && $cloid_campo_flag ){
		array_push($where, " oi.cloid " . (!$cloid_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $cloid ) . ") ");
	}
	
	// situaï¿½ï¿½o da obra
	if( $stoid[0] && $stoid_campo_flag ){
		array_push($where, " oi.stoid " . (!$stoid_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $stoid ) . ") ");
	}
	
	// percentual da obra
	if( $percentualinicial ){
		array_push($where, " oi.obrpercexec BETWEEN {$percentualinicial} AND {$percentualfinal}");
	}
	
	// percentual da obra
	if( $latitudeElongitude ){
		array_push($where, " (TRIM(ed.medlatitude)<>'' AND TRIM(ed.medlongitude)<>'')");
	}
	
	// possui foto
	switch ( $foto ) {
		case 'sim' : $stFiltro .= " and (ao.obrid is not null and ao.aqostatus = 'A') "; break;
		case 'nao' : $stFiltro .= " and ao.obrid is null  "; break;
	}

	// Filtro de vistoria
	switch ( $_REQUEST["vistoria"] ) {
		case 'sim' : $stFiltro .= " and oi.obrdtvistoria is not null "; break;
		case 'nao' : $stFiltro .= " and oi.obrdtvistoria is null "; break;
		//default    : $stFiltro .= " and ( ( s.obrid is null ) OR ( s.obrid is not null AND s.supstatus <> 'I' ) ) "; break;
	}
	
	// Filtro de restricao
	switch ( $restricao ) {
		case 'sim' : $stFiltro .= " and (r.obrid is not null and r.rststatus = 'A')"; break;
		case 'nao' : $stFiltro .= " and r.obrid is null "; break;
	}

	// monta o sql 
	$sql = "SELECT
				CASE WHEN oi.entidcampus is not null THEN ee2.entnome ELSE 'Não informado' END as campus,
				CASE WHEN ee3.entnome is not null THEN ee3.entnome ELSE 'Não informado' END as empresa,
				me.mesdsc as mesoregiao,
				re.regdescricao as regiao,
				pa.paidescricao as pais,
				ee.entnome as unidade,
				CASE WHEN ed.estuf <> '' THEN ed.estuf ELSE 'Não Informado' END as uf,
				tm.mundescricao as municipio,
				CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END as nivelpreenchimento,
				CASE WHEN oi.cloid is not null THEN cl.clodsc  ELSE 'Não informado' END as classificacao,
				CASE WHEN oi.stoid is not null THEN st.stodesc ELSE 'Não Informado' END as situacao,
				oi.stoid as codigo_situacao,
				CASE WHEN oi.tpoid is not null THEN tp.tpodsc  ELSE 'Não informado' END as tipologia,
				CASE WHEN oi.prfid is not null THEN pf.prfdesc ELSE 'Não informado' END as programa,
				oi.obrdesc as nomedaobraxls,
				'<a style=\"cursor:pointer;\" onclick=\"parent.opener.window.location.href=\'/obras/obras.php?modulo=principal/cadastro&acao=A&obrid=' || oi.obrid || '\'; parent.opener.window.focus();\">' || oi.obrdesc || '</a>' as nomedaobra,
				co.covnumero as convenio,
				count(fr1.covid) as numconvenio,
				count(frr.covid) as numobras,
				(SELECT replace(coalesce(round(SUM(icopercexecutado), 2), '0') || '', '.', ',') as total FROM obras.itenscomposicaoobra WHERE obrid = oi.obrid) as percexec,
				sum( coalesce(co.covvalor,0) ) as valor,
				(SELECT coalesce(sum(mcvvalorlancamento),0) FROM obras.movimentacaoconvenio WHERE covid = fr1.covid AND mcvtipolancamento = 'C') as valorrepassado,
				(SELECT coalesce(sum(mcvvalorlancamento),0) FROM obras.movimentacaoconvenio WHERE covid = fr1.covid AND mcvtipolancamento = 'C') - (SELECT coalesce(sum(mcvvalorlancamento),0) FROM obras.movimentacaoconvenio WHERE covid = fr1.covid AND mcvtipolancamento = 'D') as saldo,
				0 as covexec
			FROM
				obras.obrainfraestrutura oi
			LEFT JOIN
				obras.formarepasserecursos frr ON frr.obrid = oi.obrid and frr.frrstatus = 'A' and frr.frpid = 2 AND frr.covid is not null 
			INNER JOIN 
				( SELECT covid, max(obrid) as obrid from obras.formarepasserecursos where frrstatus = 'A' and frpid = 2 AND covid is not null group by covid ) fr1 ON fr1.obrid = oi.obrid 
			LEFT JOIN
				( SELECT 
					c.covid, c.covnumero, covvalor 
				  FROM 
					obras.obrainfraestrutura oi 
				  INNER JOIN 
					obras.formarepasserecursos fr ON fr.obrid = oi.obrid and fr.frrstatus = 'A'
				  INNER JOIN 
				  	obras.conveniosobra c ON c.covid = fr.covid 
				  WHERE 
				  	c.covtipo = 'C' AND fr.frpid = 2 AND fr.covid IS NOT NULL
				  GROUP BY 
				  	c.covid, c.covnumero, covvalor ) co ON co.covid = fr1.covid
			INNER JOIN 
				entidade.endereco ed ON oi.endid = ed.endid	
			LEFT JOIN 
				territorios.estado et ON ed.estuf = et.estuf
			LEFT JOIN 
				territorios.regiao re ON re.regcod = et.regcod
			LEFT JOIN 
				territorios.municipio tm2 ON tm2.muncod = ed.muncod
			LEFT JOIN 
				territorios.mesoregiao me ON me.mescod = tm2.mescod
			LEFT JOIN 
				territorios.municipio tm ON tm.muncod = ed.muncod
			INNER JOIN 
				entidade.entidade ee ON oi.entidunidade = ee.entid
			LEFT JOIN 
				territorios.pais pa ON pa.paiid = re.paiid
			LEFT JOIN 
				entidade.entidade ee2 ON oi.entidcampus = ee2.entid
			LEFT JOIN 
				entidade.funcaoentidade ef ON ee2.entid = ef.entid AND ef.funid IN( 17 )
			LEFT JOIN
				entidade.entidade ee3 ON oi.entidempresaconstrutora = ee3.entid
			LEFT JOIN 
				obras.programafonte pf ON oi.prfid = pf.prfid
			LEFT JOIN
				obras.classificacaoobra cl ON oi.cloid = cl.cloid
			LEFT JOIN
				obras.situacaoobra st ON oi.stoid = st.stoid
			LEFT JOIN
				obras.tipologiaobra tp ON oi.tpoid = tp.tpoid
			LEFT JOIN 
				( SELECT DISTINCT obrid, aqostatus FROM obras.arquivosobra WHERE tpaid = 21 AND aqostatus = 'A' ) as ao ON ao.obrid = oi.obrid 
			LEFT JOIN 
				( SELECT DISTINCT obrid, rststatus FROM obras.restricaoobra WHERE rststatus = 'A' ) as r ON r.obrid = oi.obrid
			WHERE
				oi.obsstatus = 'A' " . ( is_array($where) ? ' AND' . implode(' AND ', $where) : '' ) 
			. $stFiltro . "   
			GROUP BY 
				oi.orgid, ed.estuf, tm.mundescricao, co.covid,
				ee.entnome, ee2.entnome, ee3.entnome, me.mesdsc,
				re.regdescricao, pa.paidescricao, cl.clodsc,
				st.stodesc, oi.stoid, tp.tpodsc, pf.prfdesc, oi.obrdesc,
				oi.prfid, oi.entidcampus, oi.cloid, oi.stoid, 
				oi.tpoid, oi.prfid, oi.obrid, oi.obrdtvistoria, oi.obsdtinclusao, oi.obrqtdconstruida,
				co.covvalor, fr1.covid, co.covnumero
		ORDER BY
			" . (is_array( $agrupador ) ?  implode(",", $agrupador) : "pais") ;
	
	return $sql;
	
}

function obrasAgpExecFinanceira(){
	
	$agrupador = $_REQUEST['agrupador'];
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array("numconvenio",
										  "numobras",
										  "percexec",
										  "valor",
										  "valorrepassado",
										  "saldo",
										  "covexec")	  
				);
	
	foreach ( $agrupador as $val ){
		switch( $val ){
			case "campus":
				array_push($agp['agrupador'], array(
													"campo" => "campus",
											  		"label" => "Campus")										
									   				);
			break;
			case "mesoregiao":
				array_push($agp['agrupador'], array(
													"campo" => "mesoregiao",
											  		"label" => "Mesoregião")										
									   				);
			break;
			case "municipio":
				array_push($agp['agrupador'], array(
													"campo" => "municipio",
											  		"label" => "Município")										
									   				);
			break;
			case "pais":
				array_push($agp['agrupador'], array(
													"campo" => "pais",
											  		"label" => "País")										
									   				);
			break;
			case "regiao":
				array_push($agp['agrupador'], array(
													"campo" => "regiao",
											  		"label" => "Região")										
									   				);
			break;
			case "uf":
				array_push($agp['agrupador'], array(
													"campo" => "uf",
											  		"label" => "UF")										
									   				);
			break;
			case "unidade":
				array_push($agp['agrupador'], array(
													"campo" => "unidade",
											  		"label" => "Unidade")										
									   				);
			break;
			case "programa":
				array_push($agp['agrupador'], array(
													"campo" => "programa",
											  		"label" => "Programa Fonte")										
									   				);
			break;
			case "situacao":
				array_push($agp['agrupador'], array(
													"campo" => "situacao",
											  		"label" => "Situação da Obra")										
									   				);
			break;
			case "tipologia":
				array_push($agp['agrupador'], array(
													"campo" => "tipologia",
											  		"label" => "Tipologia da Obra")										
									   				);
			break;
			case "classificacao":
				array_push($agp['agrupador'], array(
													"campo" => "classificacao",
											  		"label" => "Classificação da Obra")										
									   				);
			break;
			case "nomedaobra":
				array_push($agp['agrupador'], array(
													"campo" => "nomedaobra",
											  		"label" => "Nome da Obra")										
									   				);
			break;
			case "nomedaobra2":
				array_push($agp['agrupador'], array(
													"campo" => "nomedaobra2",
											  		"label" => "Nome da Obra")										
									   				);
			break;
			case "nomedaobraxls":
				array_push($agp['agrupador'], array(
													"campo" => "nomedaobraxls",
											  		"label" => "Nome da Obra")										
									   				);
			break;
			case "convenio":
				array_push($agp['agrupador'], array(
													"campo" => "convenio",
											  		"label" => "Convênio")										
									   				);
			break;
			case "empresa":
				array_push($agp['agrupador'], array(
													"campo" => "empresa",
											  		"label" => "Empresa Contratada")										
									   				);
			break;
			
		}	
	}
	
	return $agp;
	
}

function obrasColunaExecFinanceira(){
	
	$coluna = array();
	
	array_push( $coluna, array("campo" 	  => "numconvenio",
					   		   "label" 	  => "Nã de Convênios",
					   		   "blockAgp" => "convenio",
					   		   "type"	  => "numeric") );
	
	array_push( $coluna, array("campo" 	  => "numobras",
					   		   "label" 	  => "Nã de Obras",
					   		   "blockAgp" => "",
					   		   "type"	  => "numeric") );
	
	array_push( $coluna, array("campo" 	  => "percexec",
					   		   "label" 	  => "% Execução Física da Obras",
					   		   "blockAgp" => array( "campus", "mesoregiao", "municipio", "pais", "regiao", "uf", 
					   		   						"unidade", "programa", "situacao", "tipologia", "classificacao", "convenio" ),
					   		   "type"	  => "string") );
	
	array_push( $coluna, array("campo" 	  => "valor",
					   		   "label" 	  => "Valor do Convênio (R$) <br/> (A)",
					   		   "blockAgp" => "",
					   		   "type"	  => "") );
	
	array_push( $coluna, array("campo" 	  => "valorrepassado",
					   		   "label" 	  => "Valor Repassado (R$) <br/> (B)",
					   		   "blockAgp" => "",
					   		   "type"	  => "") );
	
	array_push( $coluna, array("campo" 	  => "saldo",
					   		   "label" 	  => "Saldo em Conta (R$) <br/> (C)",
					   		   "blockAgp" => "",
					   		   "type"	  => "") );
	
	array_push( $coluna, array("campo" 	  => "covexec",
					   		   "label" 	  => "(%) do Convênio Executado <br/> (D) = (C * 100) / (B)",
					   		   "blockAgp" => "",
					   		   "type"	  => "",
							   "php" 	  => array(
												"expressao" => "('{valorrepassado}' != '0')",
												"var" => 'convenioexecutado',
												"true" => "number_format((str_replace(array('.',','), array('', '.'), '{saldo}') * 100) / str_replace(array('.',','), array('', '.'), '{valorrepassado}'), 2, ',', '.')",
												"false" => "0,00",
												"type" => "numeric",
												"html" => "<div style='color:#0066CC'>{convenioexecutado}</div>"
												  )
	
	) );
	
	return $coluna;
	
}


function pesquisaResponsavelObra( $dados ){
	
	$dados["usucpf"] = str_replace(".", "", $dados["usucpf"]);
	$dados["usucpf"] = str_replace("-", "", $dados["usucpf"]);
	
	$stFiltro .= !empty( $dados["usucpf"] ) ? " AND ur.usucpf = '{$dados["usucpf"]}' " : "";
	$stFiltro .= !empty( $dados["pflcod"] ) ? " AND ur.pflcod = '{$dados["pflcod"]}' " : "";
	$stFiltro .= !empty( $dados["entid"] )  ? " AND ur.entid  = '{$dados["entid"]}'  " : "";
	
	return $stFiltro;
	
}

function associaResponsavelObra( $dados ){
	
	$i = 0;
	
	if ( $dados["rpuid"] ){
		
		foreach( $dados["rpuid"] as $chave=>$valor ){
			
			$divisao = strpos($valor, "_");
			$nome 	= substr($valor, 0, $divisao);
			$cpf  	= substr($valor, $divisao + 1 );
			$cpfcod = $cpf;
			$cpf 	= formatar_cpf($cpf);
			$botoes = '<img src="/imagens/excluir.gif" style="cursor: pointer"  border="0" title="Excluir" onclick="excluirResponsavel('.$chave.');"/>';
			$input  = '<input type="hidden" id="rpuid" name="rpuid[]" value="'.$cpfcod.'"/>';
			
			$cor = ( $i % 2 ) ? "#e0e0e0" : "#F7F7F7";
			
			echo '<script type="text/javascript">
					var tabela = window.opener.document.getElementById("responsaveisobra");
					var tamanho = tabela.rows.length;
					var tr = tabela.insertRow(tamanho);
					tr.style.backgroundColor = \''.$cor.'\';
					tr.id = \'rpuid_'.$chave.'\';
					var colAcao = tr.insertCell(0);
					var colCPF  = tr.insertCell(1);
					var colNome = tr.insertCell(2);
					colAcao.style.textAlign = "center";
					colCPF.style.textAlign  = "center";
					colAcao.innerHTML = \'' .$botoes . $input . '\';
					colCPF.innerHTML  = \''.$cpf.'\';
					colNome.innerHTML = \''.$nome.'\';
				</script>';
			
			$i++;
			
		}
	}
	
	echo '<script>window.close();</script>';
	
}

function litaResponsavelObra(){

	global $db;
	
	$hab = (possuiPerfil( PERFIL_ADMINISTRADOR ) || possuiPerfil( PERFIL_SUPERUSUARIO )) ? true : false;
	
	if( isset($_SESSION["obra"]["obrid"]) ){
		
		$acao = $hab ? "<img src=\"/imagens/excluir.gif\" style=\"cursor: pointer\"  border=\"0\" title=\"Excluir\" onclick=\"excluirResponsavel(\'' || su.usucpf || '\');\"/>" :
					   "<img src=\"/imagens/excluir_01.gif\" border=\"0\"/>"; 
		
		if ( $_SESSION['obras']['orgid'] == ORGAO_FNDE ){
			$filtro = " AND ur.pflcod IN (" . PERFIL_SUPERVISORUNIDADE . ", " . PERFIL_GESTORUNIDADE . ", " . PERFIL_EMPRESA . ")";
		}else{
			$filtro = " AND ur.pflcod IN (" . PERFIL_EMPRESA . ")";
		}
			
		$sql = "SELECT DISTINCT
					'{$acao}<input type=\"hidden\" id=\"rpuid\" name=\"rpuid[]\" value=\"' || su.usucpf || '\"/>' as acao,
					--ur.rpuid as id,
					su.usucpf as cpf, 
					su.usunome as nome
				FROM 
					seguranca.usuario su 
				JOIN obras.usuarioresponsabilidade ur ON ur.usucpf = su.usucpf
														 AND ur.rpustatus = 'A' 
				WHERE 
					ur.obrid = {$_SESSION["obra"]["obrid"]}
					{$filtro}
				ORDER BY 
					su.usunome";
		$dados = $db->carregar($sql);
		
		if($dados){
			for( $i = 0; $i < count($dados); $i++ ){
				
				$cor = ( $i % 2 ) ? "#e0e0e0" : "#F7F7F7";
				
				echo "<tr id='rpuid_{$dados[$i]["cpf"]}' bgColor='{$cor}'>"
			  	   . "	<td align='center'>{$dados[$i]["acao"]}</td>"
			  	   . "	<td align='center'>" . formatar_cpf($dados[$i]["cpf"]) . "</td>"
			  	   . "	<td>{$dados[$i]["nome"]}</td>"
			  	   . "</tr>";
				
			}
		}
		
	}
	
}

function verificaPermissaoObra( $usucpf, $obrid, $orgid = 'null' ){
	
	global $db;
	
	if ( possuiPerfil( Array( PERFIL_CONSULTAGERAL,  
					   		  PERFIL_SUPERVISORMEC,
					   		  PERFIL_SUPERVISORORGAO,
					   		  PERFIL_SAMPR,
					   		  PERFIL_SAA, 
					   		  PERFIL_CONSULTAUNIDADE, 
					   		  PERFIL_GESTORMEC,
					   		  PERFIL_ADMINISTRADOR,
					   		  PERFIL_CONSULTAESTADUAL, 
					   		  PERFIL_CONSULTATIPOENSINO) ) ) {
		
		return true;
	
	}else{
		
		if( !empty($orgid) && $orgid != 3 && possuiPerfil(PERFIL_SUPERVISORUNIDADE) ){
			
			return true;
			
		}else{
			$sql = "SELECT
						obrid 
					FROM 
						obras.usuarioresponsabilidade 
					WHERE 
						usucpf = '{$usucpf}'
						AND obrid IS NOT NULL
						AND rpustatus = 'A'";
			$obridResp = $db->carregarColuna( $sql );
			
			if ($obridResp){
				foreach( $obridResp as $chave=>$valor ){
					if ( $valor == $obrid ){
						return true;
						break;
					}else{
						continue;
					}
				}
				
			} 
			// Caso nï¿½o exista o "obrid" passa como true essa validaï¿½ï¿½o
			return ($obrid ? false : true);
			
		}
	}

}

function obrasCalculaDias( $termino, $inicio ){
	
	$termino = formata_data_sql($termino);
	$inicio  = formata_data_sql($inicio);
	
	$dtTermino 	 = explode( "-", $termino );
	$dataTermino = mktime( 0, 0, 0, $dtTermino[1], $dtTermino[2], $dtTermino[0] );
	
	$dtInicio 	 = explode( "-", $inicio );
	$dataInicio  = mktime( 0, 0, 0, $dtInicio[1], $dtInicio[2], $dtInicio[0] );
	
	$dias = ($dataTermino - $dataInicio) / 86400;
	$dias = ceil($dias);
	
	$dados["obrprazoexec"] = $dias;
	
	echo json_encode($dados);
	
}

function obrBuscaUfEmpresa( $usucpf ){
	
	global $db;
	
	if( $db->testa_superuser() ){
		return true;
	}else{
		
		$sql = "SELECT
					estuf
				FROM 
					obras.usuarioresponsabilidade
				
				WHERE
					usucpf = '{$usucpf}' AND
					rpustatus = 'A' AND
					pflcod = " . PERFIL_EMPRESA;
		
		return $db->carregarColuna( $sql );
		
	}
	
}

// ------ FUNï¿½ï¿½ES WORKFLOW ------


function obrVerificaEstado( $esdid ){
	
	global $db;
	
	$sql = "SELECT esdid FROM workflow.estadodocumento WHERE esdid = {$esdid}";
	
	return $db->pegaUm( $sql );
	
}

/*
 * FUNÇÕES DO WORKFLOW (QUESTIONÁRIO)
 * 
 */
function pegarDocidSupervisao( $supvid )
{
    global $db;
    
    if (!$supvid) return false;
    
    $sql = "SELECT 
    			docid 
    		FROM 
    			obras.supervisao 
    		WHERE 
    			supvid = '" . $supvid . "'";
    $docid = $db->pegaUm( $sql );
    if(!$docid){
    	/*
    	 * Criei essa constante "_constantes.php", corresponde ao workflow "workflow.tipodocumento" cadastrado pelo Vitor no sistema.
    	 */
        $tpdid = OBR_TIPO_DOCUMENTO_SUPERVISAO;

		// MONTA NOME DO DOC
        $docdsc = sprintf( "Supervisão (%s)", $supvid);
        // cria documento
        $docid = wf_cadastrarDocumento( $tpdid, $docdsc );   
        $sql = "UPDATE obras.supervisao SET docid=" . $docid . " WHERE supvid = " . $supvid;   
        $db->executar( $sql );       
        $db->commit();
    }
    return ($docid);
}


function obrCriarDocumento( $gpdid ) {
	
	global $db;
	
	$docid = obrPegarDocid( $gpdid );
	
	if( !$docid ) {
		
		// recupera o tipo do documento
		$tpdid = OBR_TIPO_DOCUMENTO;
		
		// descriï¿½ï¿½o do documento
		$docdsc = "Fluxo da Supervisão (obras)" . $gpdid;
		
		// cria documento do WORKFLOW
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );

		// atualiza o grupo de supervisï¿½o
		$sql = "UPDATE
					obras.grupodistribuicao
				SET 
					docid = {$docid} 
				WHERE
					gpdid = {$gpdid}";

		$db->executar( $sql );
		$db->commit();
	}
	
	return $docid;
	
}

function obrPegarDocid( $gpdid ) {
	
	global $db;
	
	$sql = "SELECT
				docid
			FROM
				obras.grupodistribuicao
			WHERE
			 	gpdid = " . (integer) $gpdid;
	
	return (integer) $db->pegaUm( $sql );
	
}

function obrCriarDocumentoObra( $obrid ){
	global $db;
	
	$docid = obrPegaDocidObra( $obrid );
	
	if ( !$docid ){

		// recupera o tipo do documento
		$tpdid = OBR_TIPO_DOCUMENTO_OBRA;
		
		// descriï¿½ï¿½o do documento
		$docdsc = "Fluxo da Obra" . $obrid;
		
		// cria documento do WORKFLOW
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );
		
		if($docid){
		// atualiza o grupo de supervisï¿½o
		$sql = "UPDATE
					obras.obrainfraestrutura
				SET 
					docid = {$docid} 
				WHERE
					obrid = {$obrid}";

		$db->executar( $sql );
		$db->commit();
			
		}
		
	}
	
	return $docid;
}

function obrPegaDocidObra( $obrid ){
	global $db;
	
	$sql = "SELECT
				docid
			FROM
				obras.obrainfraestrutura
			WHERE
			 	obrid = " . (integer) $obrid; //dbg($sql,1);
	
	return (integer) $db->pegaUm( $sql );
}

function obrPegarEstadoAtual( $gpdid ) {
	
	global $db; 
	
	$docid = obrPegarDocid( $gpdid );
	 
	$sql = "select
				ed.esdid
			from 
				workflow.documento d
			inner join 
				workflow.estadodocumento ed on ed.esdid = d.esdid
			where
				d.docid = " . $docid;
	
	$estado = (integer) $db->pegaUm( $sql );
	 
	return $estado;
	
}

function obrPegarNomeEstado( $esdid ){
	
	global $db;
	
	$sql = "SELECT esddsc FROM workflow.estadodocumento WHERE esdid = {$esdid}";
	
	return $db->pegaUm( $sql );
	
}

function obrVerEmpresaGrupo( $gpdid ){
	
	global $db;
	
	$sql = "SELECT epcid FROM obras.grupodistribuicao WHERE gpdid = {$gpdid}";
	$epcid = $db->pegaUm( $sql );
	
	if( $epcid ){
		return true;
	}else{
		return false;
	}
	
}

function obrAlteraSituacaoObraRepositorio( $gpdid ){
	
	global $db;
	
	$sql    = "SELECT repid FROM obras.itemgrupo oi where gpdid = {$gpdid}";
	$repids = $db->carregarColuna( $sql );
	
	$sql = "UPDATE obras.repositorio set stsid = 4 WHERE repid in (" . implode(", ", $repids) . ")";
	$db->executar( $sql );
	
	$db->commit();
	
}

function obrEnviaEmailEmpresa( $gpdid, $epcid ){
	
	global $db;
	
	// remetente
	$remetente = array("nome" => $GLOBALS['parametros_sistema_tela']['sigla-nome_completo'], "email" => "monitoramento.obras@presidencia.gov.br");
	
	// destinatï¿½rio
	$sql = "SELECT 
				entemail 
			FROM 
				entidade.entidade ee
			INNER JOIN 
				obras.empresacontratada ec ON ec.entid = ee.entid
			WHERE 
				epcid = {$epcid}";
	
	$destinatario = $db->pegaUm( $sql );
	
	// Com cï¿½pia, para SAA./
	$cc = array("email" => "monitoramento.obras@presidencia.gov.br");
	
	// assunto
	$assunto = "Solicitação de Definição de Rotas (Monitoramento de Obras - ".$GLOBALS['parametros_sistema_tela']['sigla'].")";
	
	// conteudo
	$conteudo  = "<b>Comunicado</b> <br/><br/>";
	$conteudo .= "Foi disponibilizado um grupo contendo a relação de obras para que a sua empresa possa propor a(s) possível(eis) rotas de supervisão.<br/><br/>";
	$conteudo .= "A empresa terá <b>48 horas (dias úteis)</b>, após o envio deste, para apresentar via sistema, a proposta de rotas para análise.<br/><br/>";
	$conteudo .= "O prazo para execução do(s) serviço(s) se dará após 30 (trinta) dias da emissão da Ordem de Serviço.<br/><br/>";
	$conteudo .= "<b>Dados do Grupo:</b> <br/><br/>";
	
	// nome de quem criou o grupo
	$sql = "SELECT
				usunome as nome,
				to_char(gpddtcriacao, 'DD/MM/YYYY') as dtcriacao
			FROM
				seguranca.usuario su
			INNER JOIN
				obras.grupodistribuicao gd ON su.usucpf = gd.usucpf
			WHERE
				gpdid = {$gpdid}";
	
	$dadosGrupo = $db->pegaLinha( $sql );
		
	// tabela informativa do grupo
	$conteudo .= "<table width='95%' align='center' border='0' cellspacing='0' cellpadding='2' class='listagem'>"
			    . "    <thead><tr>"
			   . "        <td align='center'><b>Cï¿½digo do Grupo</b></td>"
			   . "        <td align='center'><b>Grupo Criado Por</b></td>"
			   . "        <td align='center'><b>Data de Criaï¿½ï¿½o</b></td>"
			   . "    </tr></thead>"
			   . "    <tr bgcolor='#F7F7F7'>"
			   . "        <td>{$gpdid}</td>"
			   . "        <td>{$dadosGrupo["nome"]}</td>"
			   . "        <td>{$dadosGrupo["dtcriacao"]}</td>"
			   . "    </tr>"
			   . "</table>"
			   . "<br/><br/>";
	
	// tabela informativa das obras do grupo
	$conteudo .= "<b>Dado(s) da(s) Obra(s):</b> <br/><br/>";
	
	$conteudo .= "<table width='95%' align='center' border='0' cellspacing='0' cellpadding='2' class='listagem'>"
			  . "    <thead><tr>"
			   . "        <td align='center'><b>Origem MEC</b></td>"
			   . "        <td align='center'><b>Unidade</b></td>"
			   . "        <td align='center'><b>Campus</b></td>"
			   . "        <td align='center'><b>Nome da Obra</b></td>"
			   . "        <td align='center'><b>Município</b></td>"
			   . "        <td align='center'><b>% de Execução</b></td>"
			   . "    </tr></thead>";
	
	// busca as obras do grupo
	$sql = "SELECT
				orgdesc as orgao,
				ee.entnome as unidade,
				ee2.entnome as campus,
				obrdesc as obra,
				mundescricao as municipio,
				coalesce(round(obrpercexec, 2), '0') as percentual
			FROM
				obras.obrainfraestrutura oi
			INNER JOIN
				entidade.endereco ed ON ed.endid = oi.endid
			LEFT JOIN
				territorios.municipio tm ON tm.muncod = ed.muncod
			INNER JOIN
				obras.orgao oo ON oi.orgid = oo.orgid
			INNER JOIN
				entidade.entidade ee ON ee.entid = oi.entidunidade
			INNER JOIN
				entidade.entidade ee2 ON ee2.entid = oi.entidcampus
			INNER JOIN
				obras.repositorio ore ON ore.obrid = oi.obrid
			INNER JOIN
				obras.itemgrupo ig ON ig.repid = ore.repid
			WHERE
				gpdid = {$gpdid}";

	$dadosObras = $db->carregar( $sql );
	
	if( is_array( $dadosObras ) ){
		
		for( $i = 0; $i < count( $dadosObras ); $i++ ){
			
			$cor = ($i % 2) ? "" : "#F7F7F7";
			
			$conteudo .= "<tr bgcolor='{$cor}'>"
					   . "    <td>{$dadosObras[$i]["orgao"]}</td>"
					   . "    <td>{$dadosObras[$i]["unidade"]}</td>"
					   . "    <td>{$dadosObras[$i]["campus"]}</td>"
					   . "    <td>{$dadosObras[$i]["obra"]}</td>"
					   . "    <td>{$dadosObras[$i]["municipio"]}</td>"
					   . "    <td align='right'>{$dadosObras[$i]["percentual"]}</td>"
					   . "</tr>";
			
		}
		
	}
			   
	$conteudo .= "</table>"
			  . "<br/><br/>";
	
	$conteudo .= "Ministério da Educação";
	
	enviar_email( $remetente, $destinatario, $assunto, $conteudo,$cc );
	
	obrAlteraSituacaoObraRepositorio( $gpdid );
	
	return true;
	
}

function obrEnviaEmailMonitoramento( $gpdid ){
	
	global $db;
	
	// remetente
	$remetente = array("nome" => $GLOBALS['parametros_sistema_tela']['silga']." - Monitoramento de Obras", "email" => "monitoramento.obras@presidencia.gov.br");
	
	// destinatï¿½rio
	$destinatario = array("email" => "monitoramento.obras@presidencia.gov.br");
		
	// assunto
	$assunto = "Solicitação de Avaliação da Rota (MEC) (Monitoramento de Obras - ".$GLOBALS['parametros_sistema_tela']['silga'].")";
	
	// conteudo
	$conteudo  = "<b>Comunicado</b> <br/><br/>";
	$conteudo .= "Foi disponibilizado  pela Empresa as definições de Rotas para Avaliação.<br/><br/>";
	$conteudo .= "<b>Dados do Grupo:</b> <br/><br/>";
	
	// nome de quem criou o grupo
	$sql = "SELECT
				usunome as nome,
				to_char(gpddtcriacao, 'DD/MM/YYYY') as dtcriacao
			FROM
				seguranca.usuario su
			INNER JOIN
				obras.grupodistribuicao gd ON su.usucpf = gd.usucpf
			WHERE
				gpdid = {$gpdid}";
	
	$dadosGrupo = $db->pegaLinha( $sql );
		
	// tabela informativa do grupo
	$conteudo .= "<table width='95%' align='center' border='0' cellspacing='0' cellpadding='2' class='listagem'>"
			    . "    <thead><tr>"
			   . "        <td align='center'><b>Código do Grupo</b></td>"
			   . "        <td align='center'><b>Grupo Criado Por</b></td>"
			   . "        <td align='center'><b>Data de Criação</b></td>"
			   . "    </tr></thead>"
			   . "    <tr bgcolor='#F7F7F7'>"
			   . "        <td>{$gpdid}</td>"
			   . "        <td>{$dadosGrupo["nome"]}</td>"
			   . "        <td>{$dadosGrupo["dtcriacao"]}</td>"
			   . "    </tr>"
			   . "</table>"
			   . "<br/><br/>";
	
	// tabela informativa das obras do grupo
	$conteudo .= "<b>Dado(s) da(s) Obra(s):</b> <br/><br/>";
	
	$conteudo .= "<table width='95%' align='center' border='0' cellspacing='0' cellpadding='2' class='listagem'>"
			  . "    <thead><tr>"
			   . "        <td align='center'><b>Origem MEC</b></td>"
			   . "        <td align='center'><b>Unidade</b></td>"
			   . "        <td align='center'><b>Campus</b></td>"
			   . "        <td align='center'><b>Nome da Obra</b></td>"
			   . "        <td align='center'><b>Município</b></td>"
			   . "        <td align='center'><b>% de Execução</b></td>"
			   . "    </tr></thead>";
	
	// busca as obras do grupo
	$sql = "SELECT
				orgdesc as orgao,
				ee.entnome as unidade,
				ee2.entnome as campus,
				obrdesc as obra,
				mundescricao as municipio,
				coalesce(round(obrpercexec, 2), '0') as percentual
			FROM
				obras.obrainfraestrutura oi
			INNER JOIN
				entidade.endereco ed ON ed.endid = oi.endid
			LEFT JOIN
				territorios.municipio tm ON tm.muncod = ed.muncod
			INNER JOIN
				obras.orgao oo ON oi.orgid = oo.orgid
			INNER JOIN
				entidade.entidade ee ON ee.entid = oi.entidunidade
			INNER JOIN
				entidade.entidade ee2 ON ee2.entid = oi.entidcampus
			INNER JOIN
				obras.repositorio ore ON ore.obrid = oi.obrid
			INNER JOIN
				obras.itemgrupo ig ON ig.repid = ore.repid
			WHERE
				gpdid = {$gpdid}";

	$dadosObras = $db->carregar( $sql );
	
	if( is_array( $dadosObras ) ){
		
		for( $i = 0; $i < count( $dadosObras ); $i++ ){
			
			$cor = ($i % 2) ? "" : "#F7F7F7";
			
			$conteudo .= "<tr bgcolor='{$cor}'>"
					   . "    <td>{$dadosObras[$i]["orgao"]}</td>"
					   . "    <td>{$dadosObras[$i]["unidade"]}</td>"
					   . "    <td>{$dadosObras[$i]["campus"]}</td>"
					   . "    <td>{$dadosObras[$i]["obra"]}</td>"
					   . "    <td>{$dadosObras[$i]["municipio"]}</td>"
					   . "    <td align='right'>{$dadosObras[$i]["percentual"]}</td>"
					   . "</tr>";
			
		}
		
	}
			   
	$conteudo .= "</table>"
			  . "<br/><br/>";
	
	$conteudo .= "Ministério da Educação";
	
	enviar_email( $remetente, $destinatario, $assunto, $conteudo );
	
	obrAlteraSituacaoObraRepositorio( $gpdid );
	
	return true;
	
}
function obrEnviaEmailSupervisaoFinalizada( $gpdid ){
	
	global $db;
		
	
	// busca as obras do grupo
	$sql = "SELECT
				orsid,
				e.entnome,	
				e.entemail
			FROM
				obras.obrainfraestrutura oi
			INNER JOIN
				entidade.endereco ed ON ed.endid = oi.endid
			LEFT JOIN
				territorios.municipio tm ON tm.muncod = ed.muncod
			INNER JOIN
				obras.orgao oo ON oi.orgid = oo.orgid
			INNER JOIN
				entidade.entidade ee ON ee.entid = oi.entidunidade
			INNER JOIN
				entidade.entidade ee2 ON ee2.entid = oi.entidcampus
			INNER JOIN
				obras.repositorio ore ON ore.obrid = oi.obrid
			INNER JOIN
				obras.itemgrupo ig ON ig.repid = ore.repid
			INNER JOIN
				obras.ordemservico os ON os.gpdid = ig.gpdid
						              AND os.orsstatus ='A'
		    INNER JOIN 
				obras.grupodistribuicao gd ON gd.gpdid = os.gpdid 
							   			   AND gpdstatus = 'A' 
			INNER JOIN 
				obras.empresacontratada ec ON ec.epcid = gd.epcid 
			INNER JOIN 
				entidade.entidade e ON ec.entid = e.entid  	
			WHERE
				ig.gpdid = {$gpdid}";

	$dadosObras = $db->carregar( $sql );

	
	// remetente
	$remetente = array("nome" => $GLOBALS['parametros_sistema_tela']['silga']." - Monitoramento de Obras", "email" => "monitoramento.obras@presidencia.gov.br");
	
	
	// destinatï¿½rio
	$destinatario = $dadosObras[0]['entemail'];
	
	
	//Com cï¿½pia
	$cc = 'monitoramento.obras@presidencia.gov.br';
	
	
	// assunto
	if($dadosObras){
		$ordemServico = " OS n°: ".$dadosObras[0]['orsid']; 
	}else{
		$ordemServico = " Não foi encontrada a OS.";
	}
	$assunto = "Finalização de Supervisão - Grupo n°: ". $gpdid . $ordemServico ;
	
	
	// conteudo
	$conteudo  = "<html>
						<head>
						</head>
						<body>
							<center>	
									<h3>Finalização de Supervisão - Grupo n°: ". $gpdid . $ordemServico."</h3>
							</center>
							
							<p align=\"justify\"> 
								&nbsp;&nbsp;&nbsp;Comunicamos que foi finalizada a avaliação dos serviços de monitoramento e supervisão de obras referentes a esta Ordem de 
								Serviço. A empresa deve apresentar a Nota fiscal com a discriminação das vistorias efetuadas e documentos que comprovem a 
								realização dos serviços, para andamento do processo de pagamento, conforme estabelecido no Termo de Referência. Para a emissão 
								da Nota Fiscal, esclarecemos que o MEC efetuará o recolhimento de Impostos Federais por DARF, com alíquota de 9,45% sobre o 
								valor total da Nota Fiscal. Os impostos Federais serão assim recolhidos:
						   	</p>
						  							   
					   		<ul>
								<li> <b>IR		-	4,8%</b>
								<li> <b>CONFINS -	3%</b>
								<li> <b>CSSL	-	1%</b>
								<li> <b>PIS		-	0,65%</b>
							</ul>
						   					   			
							<p align=\"justify\"> 
					   			&nbsp;&nbsp;&nbsp;Comunicamos que o MEC efetuará o recolhimento de Impostos sobre Serviços por DAR, com alíquota de 2% (dois por cento) 
					   			sobre o	valor dos serviços prestados em cada município. Os impostos serï¿½o recolhidos pelo MEC e será feita a retenção de 
					   			2% (dois por cento) ISS e 9,45% de impostos federais.
						   	</p>
		
						   	<center>
									<br><b>Silvio Luis Santos da Silva</b><br> Coordenador de Documentação e Gestão de  Processos<br> Fiscal dos Contratos de Monitoramento e Supervisão de Obras
							</center>	          
						</body>
					</html>";	
	
	
		//ver( $remetente, $destinatario, $assunto, $conteudo, $cc, d );
		enviar_email( $remetente, $destinatario, $assunto, $conteudo, $cc );
	
	
	return true;
	
}







function obrVerRotaGrupo( $gpdid ){
	
	global $db;
	
	$sql = "SELECT count(rotid) FROM obras.rotas WHERE gpdid = {$gpdid}";
	$existe = $db->pegaUm( $sql );
	
	if( $existe ){
		return true;
	}else{
		return false;
	}
	
}

function obrEnviaEmailMEC( $gpdid ){

	global $db;
	
	$sql = "UPDATE obras.rotas SET strid = 3 WHERE gpdid = {$gpdid}";
	$db->executar( $sql );
	
	$db->commit();
	
	return true;
	
}

function obrVerificaAntesLiberar( $gpdid ){
	
	global $db;
	
	//Verifica se o Grupo de Obras, possui uma Ordem de Serviï¿½o.
	$sqlOrsid = "SELECT orsid FROM obras.ordemservico WHERE gpdid = {$gpdid} AND orsstatus = 'A'";
	$orsid = $db->pegaUm($sqlOrsid);
	
	//Verifica se o Grupo de Obras, possui uma Rota Aprovada.
	$sqlRotid = "SELECT rotid FROM obras.rotas WHERE gpdid = {$gpdid} AND strid = 1 AND rotstatus = 'A'";
	$rotid = $db->pegaUm($sqlRotid);
		
	if($orsid != '' && $rotid != '' ){
		return true;
	}else{
		return false;
	}
	
}

function obrPosLiberar( $gpdid ){
	global $db;
	
	$sql = "SELECT
				DISTINCT
				oi.obrid,
				ur.usucpf,
				r.repid
			FROM
				obras.itemgrupo i
			JOIN obras.repositorio r USING(repid)
			JOIN obras.obrainfraestrutura oi USING(obrid)
			JOIN entidade.endereco e USING(endid)
			JOIN obras.usuarioresponsabilidade ur ON ur.estuf = e.estuf 
													 AND ur.pflcod = " . PERFIL_EMPRESA . "
													 AND ur.rpustatus = 'A'
			WHERE 
				gpdid = {$gpdid}
				AND ur.usucpf || oi.obrid NOT IN (SELECT 
													usucpf || obrid 
												  FROM 
													obras.usuarioresponsabilidade ur1 
												  WHERE 
													ur1.pflcod = " . PERFIL_EMPRESA . "
													AND ur1.rpustatus = 'A' 
													AND ur1.obrid IS NOT NULL)";
	
	$arDado = $db->carregar( $sql );

	if ( $arDado ){
		$arRepid = array();
		foreach ($arDado as $dado){
			$sql = "INSERT INTO obras.usuarioresponsabilidade( 
						usucpf, rpustatus, rpudata_inc,pflcod, obrid 
					)VALUES( 
						'{$dado["usucpf"]}', 
						'A', 
						now(),
						" . PERFIL_EMPRESA . ", 
						{$dado["obrid"]}
					);";
			$db->executar( $sql );
			// Atualiza status da supervisï¿½o no repositï¿½rio, para "Em Vistoria"
			if ( !in_array($dado['repid'], $arRepid) ){
				$sql = "UPDATE obras.repositorio
						SET stsid=" . OBRSITSUPVISTORIA . "
						WHERE repid = {$dado['repid']};";
				$db->executar( $sql );
				
				$arRepid[] = $dado['repid'];
			}
		}
		$db->commit();
	}
	return true;
}



function obrVerificaRotaAprovada( $gpdid ){
	global $db;
	
	$sql = "SELECT
				strid
			FROM
				obras.rotas rt
			WHERE
				gpdid = {$gpdid}";
	
	$statusAprovacao = $db->pegaUm( $sql );
	return $statusAprovacao != 1;
}

function obrVerificaGrupoVinculoOS( $gpdid ){
	$supervisao = new supervisao();
	return $supervisao->obrGrupoVinculoOS( $gpdid ) > 0;
}

function obrVerificacaoRetornarDefinicaoRota( $gpdid ){
	return (obrVerificaGrupoVinculoOS( $gpdid ) && obrVerificaRotaAprovada( $gpdid ));
}
function pegaPerfilArray( $usucpf ){
	global $db;
	$sql = "SELECT pu.pflcod
			FROM seguranca.perfil AS p LEFT JOIN seguranca.perfilusuario AS pu 
			  ON pu.pflcod = p.pflcod
			WHERE 
			  p.sisid = '{$_SESSION['sisid']}'
			  AND pu.usucpf = '$usucpf'
			--ORDER BY p.pflnivel";	
	
	$pflcod = $db->carregarColuna( $sql );
	return $pflcod;
}

/**Funï¿½ï¿½o que cria o "docid" para cada Obra do Grupo, quando a aï¿½ï¿½o "Iniciar a Supervisï¿½o" for acionada.
 * @author Rodrigo Pereira de Souza Silva
 * @param unknown_type $gpdid
 */
function gerarExtratoGrupoObras($gpdid){
	
	global $db;
	$sql = "SELECT DISTINCT
				ore.obrid
			FROM
				obras.itemgrupo ig
			INNER JOIN
				obras.repositorio ore ON ore.repid = ig.repid
			INNER JOIN
				obras.obrainfraestrutura oi ON oi.obrid = ore.obrid
			INNER JOIN
				obras.orgao oo ON oo.orgid = oi.orgid
			WHERE
				gpdid = ".$gpdid." AND
				repstatus = 'A'
			--ORDER BY
				--itgid;";
	
	$obrids = $db->carregar($sql);
	
	foreach($obrids as $key => $value){
		gerarExtratoObras($value['obrid'], $gpdid); 
		obrCriarDocumentoObra( $value['obrid'] );
	}
	obrPosLiberar( $gpdid );
	
	return true;
}

/**
 * @author Rodrigo Pereira de Souza Silva
 * @param unknown_type $gpdid
 */
function gerarExtratoGrupoObrasSupervisao($gpdid){
	
	global $db;
	$sql = "SELECT DISTINCT
				ore.obrid
			FROM
				obras.itemgrupo ig
			INNER JOIN
				obras.repositorio ore ON ore.repid = ig.repid
			INNER JOIN
				obras.obrainfraestrutura oi ON oi.obrid = ore.obrid
			INNER JOIN
				obras.orgao oo ON oo.orgid = oi.orgid
			WHERE
				gpdid = ".$gpdid." AND
				repstatus = 'A'
			--ORDER BY
				--itgid;";
	
	$obrids = $db->carregar($sql);
	
	foreach($obrids as $key => $value){
		gerarExtratoObras($value['obrid'], $gpdid);
		
	}
	
	return true;
}

/**
 * Funï¿½ï¿½o que gera o extrato da obra
 * @author Rodrigo Pereira de Souza Silva
 * @param integer $obrid
 * @param integer $gpdid
 */
function gerarExtratoObras($obrid, $gpdid){
		global $db;
	// abre conexï¿½o com o servidor de banco de dados
		
	if ( $obrid ){
		$_REQUEST = Array(
							"requisicao" => "visualizar",
							"fotoselecionadas" => "",
							"supvids" => "",
							"buscanaoAgrupador" => "Pesquisar campo...",
							"agrupador" => Array(
												  "0" => "contatos",
												  "1" => "contratacao",
												  "2" => "etapasobra",
												  "3" => "licitacao",
												  "4" => "localobra",
												  "5" => "projetos",
												 ),
							"coordenada" => "1",
							"mapa" => "1",
							"foto" => "1",
							"numfotos" => "",
							"fotoseleciona" => "",
							"vistoria" => "0"											 											 											 											 
	
						 );
							
//		$obrid = $_GET['obrid'];
		
		extract( $_REQUEST );
		
		$supvids = str_replace("}{",",",$supvids);
		$supvids = str_replace("{","",$supvids);
		$supvids = str_replace("}","",$supvids);
		
		$fotoselecionadas = str_replace("}{",",",$fotoselecionadas);
		$fotoselecionadas = str_replace("{","",$fotoselecionadas);
		$fotoselecionadas = str_replace("}","",$fotoselecionadas);
		
		$obras  = new Obras();
		$dados  = $obras->Dados($obrid, '', 'A');
		$dobras = new DadosObra($dados);
	}else{
		extract( $_REQUEST );
		
		$supvids = str_replace("}{",",",$supvids);
		$supvids = str_replace("{","",$supvids);
		$supvids = str_replace("}","",$supvids);
		
		$fotoselecionadas = str_replace("}{",",",$fotoselecionadas);
		$fotoselecionadas = str_replace("{","",$fotoselecionadas);
		$fotoselecionadas = str_replace("}","",$fotoselecionadas);
		
		$obras  = new Obras();
		$dados  = $obras->Dados($obrid);
		$dobras = new DadosObra($dados);
	}
	
	$arrSerialize = array();
	
	foreach( $agrupador as $valor ){
		
		switch ( $valor ){
			
			case "localobra":
				
				$arrSerialize += array(
										"Local da Obra" => array(
																	"endcep" 		=> array("label" => "CEP", "valor" => $dobras->getEndCep()),
																	"endlog" 		=> array("label" => "Logradouro", "valor" => $dobras->getEndLog()),
																	"endnum" 		=> array("label" => "Nï¿½mero", "valor" => ($dobras->getEndNum() ? $dobras->getEndNum() : "Nï¿½o Informado")),
																	"endcom" 		=> array("label" => "Complemento", "valor" => ($dobras->getEndCom() ? $dobras->getEndCom() : "Nï¿½o Informado")),
																	"endbai" 		=> array("label" => "Bairro", "valor" => $dobras->getEndBai()),
																	"mundescricao" 	=> array("label" => "Municï¿½pio/UF", "valor" => $dobras->getMunDescricao() ."/". $dobras->getEstUf())
																 )
									 );
			
			break;
			
			case "contatos":
				
				$arrauxiliar = array();
			
				$sql = "SELECT
							et.entnumcpfcnpj as cpf,
							et.entnome as nome,
							et.entemail as email,
							et.entnumdddcomercial,
							et.entnumcomercial as telefone,
							tr.tprcdesc as tipo_desc
						FROM 
							obras.responsavelobra r 
						INNER JOIN 
							obras.responsavelcontatos rc ON r.recoid = rc.recoid 
						INNER JOIN 
							entidade.entidade et ON rc.entid = et.entid 
						LEFT JOIN 
							obras.tiporespcontato tr ON rc.tprcid = tr.tprcid
						WHERE 
							r.obrid = '". $obrid . "'  AND 
							rc.recostatus = 'A'";
				
				$dadosContatos = $db->carregar( $sql );
				
				if ( $dadosContatos ){
	
					for( $i = 0; $i < count($dadosContatos); $i++ ){
	
						array_push($arrauxiliar, array(
												"entnumcpfcnpj" 	=> array("label" => "CPF", "valor" 						=> $dadosContatos[$i]["cpf"]),
												"entnome" 			=> array("label" => "Nome do Responsï¿½vel", "valor" 		=> $dadosContatos[$i]["nome"]),
												"entemail" 			=> array("label" => "E-mail", "valor" 					=> ($dadosContatos[$i]["email"] ? $dadosContatos[$i]["email"] : "Nï¿½o informado" )),
												"entnumdddcomercial"=> array("label" => "DDD", "valor" 						=> $dadosContatos[$i]["entnumdddcomercial"]),
												"entnumcomercial"	=> array("label" => "telefone", "valor" 				=> $dadosContatos[$i]["telefone"]),
												"endbai" 			=> array("label" => "Tipo de Responsabilidade", "valor" => $dadosContatos[$i]["tipo_desc"])
											  )
									);
						
					}
									
				}else{
					
					$arrauxiliar = array(
											"ERRO" 	=> array("label" => "ERRO", "valor" => "Nï¿½o existem contatos cadastrados para a obra.")
										 );
					
				}
				
				#concatenando os arrays
				foreach($arrauxiliar as $key => $valor){
					$arrSerialize['Contatos'][$key] = $valor;
				}
				
				// pesquisando os responsï¿½veis
				$arrauxiliar = array();
			
				$sql = "SELECT DISTINCT
						su.usucpf,
						su.usunome,
						su.usuemail,
						su.usufoneddd,
						su.usufonenum
					FROM 
						seguranca.usuario su 
					JOIN obras.usuarioresponsabilidade ur ON ur.usucpf = su.usucpf
									      AND ur.rpustatus = 'A' 
					WHERE 
						ur.obrid = {$obrid}
					AND ur.pflcod IN (" . PERFIL_SUPERVISORUNIDADE . ", " . PERFIL_GESTORUNIDADE . ", " . PERFIL_EMPRESA . ")
					ORDER BY 
						su.usunome";
				
				$dadosResponsaveis = $db->carregar( $sql );
				
				if ( $dadosResponsaveis ){
	
					for( $i = 0; $i < count($dadosResponsaveis); $i++ ){
	
						array_push($arrauxiliar, array(
												"usucpf"	=> array("label" => "CPF", "valor" 						=> $dadosResponsaveis[$i]["usucpf"]),
												"usunome" 	=> array("label" => "Nome do Responsï¿½vel", "valor" 		=> $dadosResponsaveis[$i]["usunome"]),
												"usuemail" 	=> array("label" => "E-mail", "valor" 					=> ($dadosResponsaveis[$i]["usuemail"] ? $dadosContatos[$i]["usuemail"] : "Nï¿½o informado" )),
												"usufoneddd"=> array("label" => "DDD", "valor" 						=> $dadosResponsaveis[$i]["usufoneddd"]),
												"usufonenum"=> array("label" => "telefone", "valor" 				=> $dadosResponsaveis[$i]["usufonenum"])
											  )
									);
					}
									
				}else{
					
					$arrauxiliar = array(
											"ERRO" 	=> array("label" => "ERRO", "valor" => "Nï¿½o existem Responsï¿½veis cadastrados para a obra.")
										 );
					
				}// fim da pesquisa pelos responsï¿½veis
				
				#concatenando os arrays
				foreach($arrauxiliar as $key => $valor){
					$arrSerialize['Responsaveis'][$key] = $valor;
				}
								   
			break;
			
			case "contratacao":
				
				$obra = $obras->ViewObra($obrid);
				
				if ( $tobaid = $dobras->getTobraId() ){
						
					$sql = "SELECT 
								tobadesc AS descricao 
							FROM 
								obras.tipoobra
							WHERE
								tobaid = ".$tobaid;
					
					$tpContratacao = $db->pegaUm($sql) ? $db->pegaUm($sql) : "Nï¿½o informado";
					
				}
				
				$empresa = new Entidade($dobras->getEntIdEmpresaConstrutora());
				$entnomeempresa = $empresa->entnome;
				
				if( $dobras->getEntIdEmpresaConstrutora() ){
					
					$sql = "SELECT
								entemail as email,
								entnumdddcomercial as ddd,
								entnumcomercial as telefone,
								ed.endlog || ' nï¿½ ' || ed.endnum || ', ' || ed.endbai || ' - ' || tm.mundescricao || ', ' || ed.estuf as endereco
							FROM 
								entidade.entidade e
							LEFT JOIN
								entidade.endereco ed ON e.entid = ed.entid 
							LEFT JOIN
								territorios.municipio tm ON ed.muncod = tm.muncod 
							WHERE 
								e.entid = " . $dobras->getEntIdEmpresaConstrutora();
					
				}
				$dados = "";
				$dados = $db->carregar($sql);
				
				if ( is_array($dados) ){
					$emailempresa 	 = $dados[0]['email'] ? $dados[0]['email'] : "Nï¿½o informado";
					$enderecoempresa = $dados[0]['endereco'];	
					$dddempresa 	 = $dados[0]['ddd'];
					$telefoneempresa = $dados[0]['telefone'];
					$naturezaempresa = $dados[0]['natureza'] ? $dados[0]['natureza'] : "Nï¿½o informado";	
				}
				
				if( $stoid = $dobras->getStoId() ){
					
					$sql = "SELECT
								stodesc
							FROM
								obras.situacaoobra
							WHERE
								stoid = {$stoid}";
					
					$stContratacao = $db->pegaUm( $sql ) ? $db->pegaUm( $sql ) : "Nï¿½o informado";
					
				}
				
				$area = number_format( $dobras->getObrQtdConstruida(), 2, ',', '.' );
				if ( $umdid = $dobras->getUmdIdObraConstruida() ){
					
					$sql = "SELECT 
								umdeesc AS descricao 
							FROM 
								obras.unidademedida
							WHERE 
								umdid =".$umdid;
					
					$umContratacao = $db->pegaUm($sql);
					
				}
				
				
				switch( $dobras->getObrStatusInauguracao() ){
					case "S":
						$inContratacao = "Nï¿½o se Aplica";
					break;
					case "N":
						$inContratacao = "Nï¿½o Inaugurada";
					break;
					case "I":
						$inContratacao = "Inaugurada";
					break;
				}	
		
				if( $dobras->getFrpId() ){
					
					
					$sql = "SELECT 
								frpdesc AS descricao 
							FROM 
								obras.tipoformarepasserecursos
							WHERE 
								frpid =".$dobras->getFrpId();
					
					$tfrContratacao =  $db->pegaUm($sql);
					
				}
				
				$tfrContratacao = $tfrContratacao ? $tfrContratacao : "Nï¿½o informado";
				
				switch( $dobras->getFrpId() ){
					
					case 2:
				
						$covid = $dobras->getCovId();
							
						if( $covid ){
										
							$dados_convenio = $db->pegaLinha("SELECT
																*
															  FROM 
															  	obras.conveniosobra
															  WHERE
															  	covid = '{$covid}'");
							
						}
									
						$covnumero = $dados_convenio["covnumero"];
						
						$arrauxiliar	   = array(
													"covnumero" 		=> array("label" => "Nï¿½mero do Convï¿½nio" , "valor" => $covnumero),
													"covano" 			=> array("label" => "Ano" , "valor" => formata_data($dados_convenio["covano"])),
													"covobjeto" 		=> array("label" => "Objeto" , "valor" => $dados_convenio["covobjeto"]),
													"covdetalhamento" 	=> array("label" => "Detalhamento" , "valor" => $dados_convenio["covdetalhamento"]),
													"covprocesso" 		=> array("label" => "Processo" , "valor" => $dados_convenio["covprocesso"]),
													"covvlrconcedente" 	=> array("label" => "Concedente" , "valor" => number_format($dados_convenio["covvlrconcedente"],2,',','.')),
													"covvlrconvenente" 	=> array("label" => "Convenente" , "valor" => number_format($dados_convenio["covvlrconvenente"],2,',','.')),
													"covvalor" 			=> array("label" => "Valor (R$)" , "valor" => number_format($dados_convenio["covvalor"],2,',','.')),
													"covdtinicio" 		=> array("label" => "Inï¿½cio" , "valor" => formata_data($dados_convenio["covdtinicio"])),
													"covdtfinal" 		=> array("label" => "Fim" , "valor" => formata_data($dados_convenio["covdtfinal"])),
												   );
	
					break;
					
					case 3:
						
						$frrdescinstituicao = $dobras->getFrrDescInstituicao();
						if( $frrdescinstituicao == "" ){
							$dados2 = $obras->ViewObra($obrid);
							$dobras->setFrrDescInstituicao($dados2["entidade"]);
						}
						
						$frrdescinstituicao = $dobras->getFrrDescInstituicao();
						$frrdescnumport     = $dobras->getFrrDescNumPort();
						$frrdescobjeto 		= $dobras->getFrrDescObjeto();
						$frrdescvlr 		= number_format( $dobras->getFrrDescVlr(), 2, ',', '.' );
						
						$arrauxiliar	   = array(
													"frrdescinstituicao" 	=> array("label" => "Instituiï¿½ï¿½o" , "valor" => $frrdescinstituicao),
													"frrdescnumport" 		=> array("label" => "Nï¿½mero da Portaria de Descentralizaï¿½ï¿½o" , "valor" => $frrdescnumport),
													"frrdescobjeto" 		=> array("label" => "Objeto" , "valor" => $frrdescobjeto),
													"frrdescvlr" 			=> array("label" => "Valor (R$)" , "valor" => $frrdescvlr)
												   );
						
					break;
					
					default:
						
						$arrauxiliar	   = array(
													"frrobsrecproprio" 	=> array("label" => "Observaï¿½ï¿½o" , "valor" => ($dobras->getFrrObsRecProprio() ? $dobras->getFrrObsRecProprio() : "Nï¿½o informado"))
												   );
						
					break;
				}
				
				// Situaï¿½ï¿½o do Imï¿½vel
				$infraestrutura = new DadosInfraEstrutura();
				$infra = $infraestrutura->busca($obrid);
				
				$sql = "SELECT
							aqidsc 
						FROM 
							obras.tipoaquisicaoimovel
						WHERE
							aqiid = ".(int)$infra['aqiid'];
				
				// Descriï¿½ï¿½o da Situaï¿½ï¿½o do Imï¿½vel 
				$aqidsc = $db->pegaUm($sql);
				
				// Situaï¿½ï¿½o Dominial jï¿½ Regularizada? 
				$iexsitdominialimovelregulariza = $infra['iexsitdominialimovelregulariza'];
				
				$arrSerialize += array(
										"Contrataï¿½ï¿½o" => array(
																"entnome" 						=> array("label" => "Empresa Contratada" , "valor" => $entnomeempresa),
										    					"entemail" 						=> array("label" => "E-mail" , "valor" => $emailempresa),
																
																"endereï¿½o" 						=> array("label" => "Endereï¿½o" , "valor" => $enderecoempresa),				
										    					"telefone"						=> array("label" => "Telefone" , "valor" => "({$dddempresa}) {$telefoneempresa}"),
																
										    					"natureza"						=> array("label" => "Natureza Jurï¿½dica" , "valor" => $naturezaempresa),
										    					"obrdtassinaturacontrato"		=> array("label" => "Data de Assinatura do Contrato" , "valor" => ( $dobras->getObrDtAssinaturaContrato() ? formata_data( $dobras->getObrDtAssinaturaContrato() ) : "Nï¿½o informado" )),
										    					"stodesc"						=> array("label" => "Situaï¿½ï¿½o da Obra" , "valor" => $stContratacao),
																"obrdtordemservico"				=> array("label" => "Data da Ordem de Serviï¿½o" , "valor" => ( $dobras->getObrDtOrdemServico() ? formata_data( $dobras->getObrDtOrdemServico() ) : "Nï¿½o informado" )),
										    					"obrdtinicio"					=> array("label" => "Inï¿½cio de Execuï¿½ï¿½o da Obra" , "valor" => ( $dobras->getObrDtInicio() ? formata_data( $dobras->getObrDtInicio() ) : "Nï¿½o informado" )),
										    					"obrdttermino"					=> array("label" => "Tï¿½rmino de Execuï¿½ï¿½o da Obra" , "valor" => ( $dobras->getObrDtTermino() ? formata_data( $dobras->getObrDtTermino() ) : "Nï¿½o informado" )),
										    					"obrcustocontrato" 				=> array("label" => "Valor do Contrato (R$)" , "valor" => number_format( $dobras->getObrCustoContrato(), 2, ',' , '.' )),
										    					"obrqtdconstruida" 				=> array("label" => "ï¿½rea/Quantidade a ser Construï¿½da" , "valor" => $area. " ".$umContratacao),
										    					"obrcustounitqtdconstruida"		=> array("label" => "Custo Unitï¿½rio (R$)" , "valor" => number_format( $dobras->getObrCustoUnitQtdConstruida(), 2, ',', '.')),
										    					"tobadesc"						=> array("label" => "Tipo de Obra" , "valor" => $tpContratacao),
				
																"aqidsc"						=> array("label" => "Tipo de Aquisiï¿½ï¿½o do Terreno" , "valor" => $aqidsc),
																"iexsitdominialimovelregulariza"=> array("label" => "Situaï¿½ï¿½o Dominial jï¿½ Regularizada?" , "valor" => ( $iexsitdominialimovelregulariza ? "Sim" : "Nï¿½o") ),
				
										    					"obrstatusinauguracao"			=> array("label" => "Inaugurada" , "valor" => $inContratacao),
										    					"obrdtinauguracao"				=> array("label" => "Data da Inauguraï¿½ï¿½o" , "valor" => ( $dobras->getObrDtInauguracao() ? formata_data( $dobras->getObrDtInauguracao() ) : "Nï¿½o informado" )),
										    					"obrdtprevinauguracao"			=> array("label" => "Data de Previsï¿½o da Inauguraï¿½ï¿½o" , "valor" => ( $dobras->getObrDtPrevInauguracao() ? formata_data( $dobras->getObrDtPrevInauguracao() ) : "Nï¿½o informado" )),
										    					"frpdesc"						=> array("label" => "Tipo" , "valor" => $tfrContratacao)
										       				   )
									  );
				
				#concatenando os arrays
				foreach($arrauxiliar as $key => $valor){
					$arrSerialize['Contrataï¿½ï¿½o'][$key] = $valor;
				}
								   
			break;
			
			case "etapasobra":
				
				$arrauxiliar = array();
				
				$sql = "SELECT 
							i.itcid,
							i.icovlritem,
							i.icopercsobreobra,
							i.icopercexecutado,
							ic.itcdesc,
							ic.itcdescservico
						FROM 
							obras.itenscomposicaoobra i,
							obras.itenscomposicao ic 
						WHERE 
							i.obrid = " . $obrid . " 
							and i.itcid = ic.itcid 
							and i.icovigente = 'A'
						ORDER BY 
							i.icoordem";
				
				$dadosEtapas = $db->carregar( $sql );
				
				$tabelaEtapas = "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>";
				
				if ( $dadosEtapas ){
				
					$tabelaEtapas .= "    <tr>"
								  . "        <td class='subtitulocentro'>Descriï¿½ï¿½o</td>"
								  . "        <td class='subtitulocentro'>Valor do Item</td>"
								  . "        <td class='subtitulocentro'>% Referente a Obra</td>"
								  . "    </tr>";
	
					for( $i = 0; $i < count($dadosEtapas); $i++ ){
						
						$itcid 		= $dadosEtapas[$i]['itcid'];
						$icovlritem = $dadosEtapas[$i]['icovlritem'];
						$itcdesc 	= $dadosEtapas[$i]['itcdesc'];
						$icopercsobreobra = $dadosEtapas[$i]['icopercsobreobra'];
						$icopercexecutado = $dadosEtapas[$i]['icopercexecutado'];
						$itcdescservico   = $dadosEtapas[$i]['itcdescservico'];
						
						$somav 		= bcadd($somav, $icovlritem, 2);
						$icovlritem = number_format($icovlritem,2,',','.'); 
						$soma 		= round( $soma, 2 ) + round( $icopercsobreobra, 2 );
						
						$icopercsobreobra = number_format( $icopercsobreobra, 2);
						$icopercsobreobra = str_replace( '.', ',', $icopercsobreobra );
											
						$cor = $i % 2 ? "#e0e0e0" : '#ffffff';
						
						array_push($arrauxiliar,
	                                       array(
												"itcdesc" 			=> array("label" => "Descriï¿½ï¿½o", "valor" => $itcdesc),
												"icovlritem" 		=> array("label" => "Valor do Item", "valor" => $icovlritem),
												"icopercsobreobra" 	=> array("label" => "% Referente a Obra", "valor" => $icopercsobreobra)
											)
	                         );
	
					}
				
				}else{
					
					$arrauxiliar = array(
											"ERRO" 	=> array("label" => "ERRO", "valor" => "Nï¿½o existem etapas cadastradas para a obra.")
										 );
					
				}
				
				$soma = ($soma > 100.00) ? 100.00 : $soma;
				
				$arrauxiliar += array(
										"icovlritem" 		=> array("label" => "Total", "valor" => number_format( $somav, 2, ',', '.' )),
										"icopercsobreobra" 	=> array("label" => "Total", "valor" => number_format( $soma, 2, ',', '.' ))
									 );
				
				#concatenando os arrays
				foreach($arrauxiliar as $key => $valor){
					$arrSerialize['Cronograma Fï¿½sico-Financeiro'][$key] = $valor;
				}
				  			  
			break;
			
			case "execucao":
			
				/*$execOrc = new execOrcamentaria();
				$dadosExecOrc = $execOrc->buscaExecOrcamentaria( $obrid );
	
				$nEocvlrtotal = $dadosExecOrc["eocvlrcapital"] + $dadosExecOrc["eocvlrcusteio"];
				$eocvlrtotal  = number_format($nEocvlrtotal, 2, ",", ".");
	
				if ( $dadosExecOrc["eorid"] ){
						
						$sql = "SELECT
									*
								FROM
									 obras.itensexecucaoorcamentaria
								WHERE
									eorid = {$dadosExecOrc["eorid"]}
								ORDER BY
									eocdtposicao";
						
						$dadosItensExec =  $db->carregar( $sql );
					
					}
					
					$tabItensExec = "<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>";
					
					if( $dadosItensExec ){
						
						$tabItensExec .= "<tr>
											<td class='subtitulocentro'>Data</td>
											<td class='subtitulocentro'>Valor Empenhado (R$)</td>
											<td class='subtitulocentro'>Valor Liquidado (R$)</td>
											<td class='subtitulocentro'>% Empenhado</td>
											<td class='subtitulocentro'>% Liquidado</td>
										</tr>";
						
						for( $i = 0; $i < count($dadosItensExec); $i++ ){
							
							$cor = ( $i % 2 ) ? "#e0e0e0" : "#f4f4f4";
							
							$perEmpenhado = $dadosItensExec[$i]["eocvlrempenhado"] ? ( $dadosItensExec[$i]["eocvlrempenhado"] / $eocvlrtotal ) * 100 : 0.00;
							$perLiquidado = $dadosItensExec[$i]["eocvlrliquidado"] ? ( $dadosItensExec[$i]["eocvlrliquidado"] / $eocvlrtotal ) * 100 : 0.00;
							
							$tabItensExec .=  "<tr bgcolor='{$cor}' align='right' id='item_{$dadosItensExec[$i]["ideid"]}'>"
										  . "    <td width='15%' align='center'>"
										  . "        <input type='hidden' name='eocdtposicao[]' id='eocdtposicao[]' value='" . formata_data($dadosItensExec[$i]["eocdtposicao"]) . "'/>"
										  .	       formata_data($dadosItensExec[$i]["eocdtposicao"])
										  . "    </td>"
										  . "    <td width='20%'>"
										  . "        <input type='hidden' name='eocvlrempenhado[]' id='eocvlrempenhado[]' value='{$dadosItensExec[$i]["eocvlrempenhado"]}'/>"
										  . 	       number_format($dadosItensExec[$i]["eocvlrempenhado"], 2, ",", ".") 
										  . "    </td>"
										  . "    <td width='20%'>"
										  . "        <input type='hidden' name='eocvlrliquidado[]' id='eocvlrliquidado[]' value='{$dadosItensExec[$i]["eocvlrliquidado"]}'/>"
										  .          number_format($dadosItensExec[$i]["eocvlrliquidado"], 2, ",", ".")
										  . "    </td>"
										  . "    <td width='15%'>" . number_format($perEmpenhado, 2, ",", ".") . " %</td>"
										  . "    <td width='15%'>" . number_format($perLiquidado, 2, ",", ".") . " %</td>"
										  . "</tr>";
							
						}
						
					}else{
						
						$tabItensExec .= "<tr><td style='color:#ee0000;'>Nï¿½o existem itens cadastrados para este orï¿½amento.</td></tr>";
						
					}
				
				$tabItensExec .= "</table>";
					
				$dadosExtratoObras .= "<tr>"
								   .  "    <td class='subtitulocentro' colspan='2' height='25px;'>Execuï¿½ï¿½o Orï¿½amentï¿½ria</td>"
								   .  "</tr>"
								   .  "<tr>"
								   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Capital (R$)</td>"
								   .  "    <td bgcolor='#f5f5f5'>" .  number_format($dadosExecOrc["eocvlrcapital"], 2, ",", ".") . "</td>"
								   .  "</tr>"
								   .  "<tr>"
								   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Custeio (R$)</td>"
								   .  "    <td>" . number_format($dadosExecOrc["eocvlrcusteio"], 2, ",", ".") . "</td>"
								   .  "</tr>"
								   .  "<tr>"
								   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Total (R$)</td>"
								   .  "    <td bgcolor='#f5f5f5'>{$eocvlrtotal}</td>"
								   .  "</tr>"
								   .  "<tr>"
								   .  "    <td style='font-weight: bold;'>Detalhamento Orï¿½amentï¿½rio</td>"
								   .  "</tr>"
								   .  "<tr>"
								   .  "    <td colspan='2'>"
								   .           $tabItensExec
								   .  "    </td>"
								   .  "</tr>";*/
								  
				
			break;
			
			case "licitacao":
				
				$arrauxiliar = array();
			
				$licitacao = new licitacao();
				$resultado = $licitacao->busca($obrid);
				$dadosLic  = $licitacao->dados($resultado);
				
				if( $licitacao->molid ){
					
					$sql = "SELECT 
						   		moldsc AS descricao 
						   FROM 
								obras.modalidadelicitacao
						   ORDER BY 
								moldsc";
				
					$moldsc = $db->pegaUm( $sql );
					
				}
				
				$moldsc = $moldsc ? $moldsc : "Nï¿½o informado";
				
				$sql = "SELECT 
							fl.*,
							tfl.tfldesc, tfl.tflordem   
						FROM 
							obras.faselicitacao fl 
						INNER JOIN 
							obras.tiposfaseslicitacao tfl ON fl.tflid = tfl.tflid
						WHERE 
							fl.obrid = '". $obrid . "' AND fl.flcstatus = 'A' ORDER BY tfl.tflordem";
				
				$flLicitacao = $db->carregar( $sql );
				
				$tabelaFlCont = "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>";
				
				if ( $flLicitacao ){
				
					for( $i = 0; $i < count($flLicitacao); $i++ ){
						
						$tflid   = $flLicitacao[$i]['tflid'];
						$tfldesc = $flLicitacao[$i]['tfldesc'];
						$flcpubleditaldtprev = formata_data($flLicitacao[$i]['flcpubleditaldtprev']);
						$flcdtrecintermotivo = formata_data($flLicitacao[$i]['flcdtrecintermotivo']);
						$flcordservdt		 = formata_data($flLicitacao[$i]['flcordservdt']);
						$flchomlicdtprev     = formata_data($flLicitacao[$i]['flchomlicdtprev']);
						$flcaberpropdtprev	 = formata_data($flLicitacao[$i]['flcaberpropdtprev']);
	
						switch( $tflid ){
							
							case 2:
								$flcdata = $flcpubleditaldtprev;
							break;
							case 5:
								$flcdata = $flcdtrecintermotivo;
							break;
							case 6:
								$flcdata = $flcordservdt;
							break;
							case 7:
								$flcdata = $flcaberpropdtprev;
							break;
							case 9:
								$flcdata = $flchomlicdtprev;
							break;
							
						}
						
						array_push($arrauxiliar, array(
														"tfldesc" 	=> array("label" => "Descriï¿½ï¿½o" , "valor" => $tfldesc),
														"tflid" 	=> array("label" => "Data" , "valor" => $flcdata)
											 		   )
								    );
						
					}
									
				}else{
					
					$arrauxiliar = array(
											"ERRO" 	=> array("label" => "ERRO", "valor" => "Nï¿½o existem fases de licitaï¿½ï¿½o cadastradas para a obra.")
										 );
					
				}
				
				$tabelaFlCont .= "</table>";
				
				$arrSerialize += array(
										"Licitaï¿½ï¿½o" => array(
																	"moldsc" 			=> array("label" => "Modalidade de Licitaï¿½ï¿½o", "valor" => $moldsc),
																	"dtiniciolicitacao" => array("label" => "Inï¿½cio Programado", "valor" => ($licitacao->dtiniciolicitacao ? formata_data($licitacao->dtiniciolicitacao) : "Nï¿½o Informado")),
																	"dtfinallicitacao" 	=> array("label" => "Tï¿½rmino Programado", "valor" => ($licitacao->dtfinallicitacao ? formata_data($licitacao->dtfinallicitacao) : "Nï¿½o Informado")),
																	"licitacaouasg" 	=> array("label" => "Nï¿½mero da UASG", "valor" => ($licitacao->licitacaouasg ? $licitacao->licitacaouasg : "Nï¿½o Informado")),
																	"numlicitacao" 		=> array("label" => "Nï¿½mero da Licitaï¿½ï¿½o", "valor" => ($licitacao->numlicitacao ? $licitacao->numlicitacao : "Nï¿½o Informado"))
																 )
									 );
									 
				#concatenando os arrays
				foreach($arrauxiliar as $key => $valor){
					$arrSerialize['Licitaï¿½ï¿½o'][$key] = $valor;
				}
				
				
			break;
			
			case "projetos":
				
				$arrauxiliar = array();
				
				$faseprojeto = new DadosFasesProjeto();
				$resultado   = $faseprojeto->busca($obrid);	
				$dados 		 = $faseprojeto->dados($resultado);
				
				if ($tpaid = $faseprojeto->tpaid){
					
					$sql = "SELECT 
								tpadesc as descricao 
							FROM 
								obras.tipoprojetoarquitetonico
							WHERE 
								tpaid=".$tpaid;
					
					$tpProjeto = $db->pegaUm($sql) ? $db->pegaUm($sql) : "Nï¿½o informado";
				}
				
				if ($felid = $faseprojeto->felid){
					
					$sql = "SELECT 
								feldesc as descricao 
							FROM 
								obras.formaelaboracao
							WHERE 
								felid=".$felid;
					
					$feProjeto = $db->pegaUm($sql) ? $db->pegaUm($sql) : "Nï¿½o informado";
				}
				
				if( $faseprojeto->felid == 1 || $faseprojeto->felid == 3 || $faseprojeto->felid == 4 ){
					
					$obProjeto = $faseprojeto->fprobsexecdireta ? $faseprojeto->fprobsexecdireta : "Nï¿½o Informado";
					
				}else if( $faseprojeto->felid == 2 ){
					
					$arrauxiliar = array(
											"fprvlrformaelabrecproprio" 	=> array("label" => "Recurso Prï¿½prio (R$)", "valor" => number_format( $faseprojeto->fprvlrformaelabrecproprio, 2, ',', '.' )),
											"fprvlrformaelabrrecrepassado" 	=> array("label" => "Recurso Repassado (R$)", "valor" => number_format( $faseprojeto->fprvlrformaelabrrecrepassado, 2, ',', '.' ))
										 );
							
				}
				
				if ($tfpid = $faseprojeto->tfpid){
					
					$sql = "SELECT 
								tfpdesc as descricao 
							FROM 
								obras.tipofaseprojeto
							WHERE 
								tfpid=".$tfpid;
					
					$fpProjeto = $db->pegaUm($sql) ? $db->pegaUm($sql) : "Nï¿½o informado";
					
				}
	
				switch( $tfpid ){
					case 1:
						$dtProjeto = $faseprojeto->fprdtiniciofaseprojeto;
					break;
					case 2:
						$dtProjeto = $faseprojeto->fprdtprevterminoprojeto;
					break;
					case 3:
						$dtProjeto = $faseprojeto->fprdtconclusaofaseprojeto;
					break;
				}
				
				$arrSerialize += array(
										"Projetos" => array(
																	"tpadesc" 		=> array("label" => "Tipo de Projeto", "valor" => $tpProjeto),
																	"feldesc" 		=> array("label" => "Forma de Elaboraï¿½ï¿½o do projeto", "valor" => $feProjeto),
																	"obProjeto" 	=> array("label" => "Observaï¿½ï¿½es", "valor" => (!empty($arrauxiliar) ? $arrauxiliar : "NULL")),
																	"tfpdesc" 		=> array("label" => "Fases do Projeto", "valor" => $fpProjeto),
																	"dtProjeto"		=> array("label" => "Previsï¿½o / Conclusï¿½o", "valor" => formata_data($dtProjeto))
																 )
									 ); 
				
			break;
			
		}
		
	}
	
	if( $coordenada == 1 ){
		
		$arrauxiliar = array();
		
		$longitude = $dobras->getMedLongitude();
		$longitude = explode(".", $dobras->getMedLongitude());
		$graulongitude = $longitude[0];
		$minlongitude = $longitude[1];
		$seglongitude = $longitude[2];
		
		$latitude = $dobras->getMedLatitude();
		$latitude = explode(".", $dobras->getMedLatitude());
		$graulatitude = $latitude[0];
		$minlatitude = $latitude[1];
		$seglatitude = $latitude[2];
		$pololatitude = $latitude[3];
		
		//colocando # para que nï¿½o haja erro na hora em que os dados forem salvos no banco
		// Latitude
		$dadosLatitude =  $graulatitude."ï¿½ ".$minlatitude."# ".$seglatitude."## "."  ".$pololatitude;
		
		// Longitude
		$dadosLongitude = $graulongitude."ï¿½ ".$minlongitude."# ".$seglongitude."##";
		
		//valores para o usuï¿½rio
		$arrauxiliar = array(
								"medlatitude" 		=> array("label" => "Latitude", "valor" => $dadosLatitude),
								"medlongitude" 		=> array("label" => "Longitude", "valor" => $dadosLongitude)
							 );
		
		if( $mapa == 1 ){
			
			$latitude  = ((( $seglatitude / 60 ) + $minlatitude) / 60 ) + $graulatitude;
			$longitude = ((( $seglongitude / 60 ) + $minlongitude) / 60 ) + $graulongitude;
			
			if ( $latitude && $longitude ){	
				
				#$arrSerialize += array();
	
				$posicao = ( $pololatitude == "N" ) ? "-" : "";
				
				//valores para o Google
				$arrauxiliar += array(
										"posicao" => array("label" => "posicao", "valor" => $posicao),
										"latitude" => array("label" => "latitude", "valor" => $latitude),
										"longitude" => array("label" => "longitude", "valor" => $longitude),
									   );
				
			}else{
				$arrauxiliar += array(
										"ERRO" => array("label" => "ERRO", "valor" => "Dados de localizaï¿½ï¿½o nï¿½o cadastrados.")
									   );
			}
			
		}
		
		#concatenando os arrays
		foreach($arrauxiliar as $key => $valor){
			$arrSerialize['Coordenadas Geogrï¿½ficas'][$key] = $valor;
		}
						   
	}
	
	if( $foto == 1 ){
		
		$arrauxiliar = array();
		
		$sql = "SELECT 
					arqnome, 
					arq.arqid, 
					arq.arqextensao, 
					arq.arqtipo, 
					arq.arqdescricao 
				FROM 
					public.arquivo arq
				INNER JOIN 
					obras.arquivosobra oar ON arq.arqid = oar.arqid
				INNER JOIN 
					obras.obrainfraestrutura obr ON obr.obrid = oar.obrid 
				INNER JOIN 
					seguranca.usuario seg ON seg.usucpf = oar.usucpf 
				WHERE 
					" . ( $fotoseleciona ? " arq.arqid IN ({$fotoseleciona}) AND " : "" ) . "  
					obr.obrid = {$obrid} AND
					aqostatus = 'A' AND
					(arqtipo = 'image/jpeg' OR 
					 arqtipo = 'image/gif' OR 
					 arqtipo = 'image/png') 
				ORDER BY 
					arq.arqid ";
	
		$fotos = ($db->carregar($sql));
	
		if( $fotos ){
			
			$tabelaFotos = "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>";
			
			for( $i = 0; $i < count( $fotos ); $i++ ){
				
				if( $fotos[$i]["arqid"] ){
					
					array_push($arrauxiliar,
	                                       array(
													"arqid" 		=> array("label" => "arqid" , "valor" => $fotos[$i]["arqid"]),
													"arqdescricao" 	=> array("label" => "arqdescricao" , "valor" => $fotos[$i]["arqdescricao"])
											     )
	                           );
					
				}
				
				$i = $i+1;
				
				if( $fotos[$i]["arqid"] ){
					
					array_push($arrauxiliar,
	                                       array(
													"arqid" 		=> array("label" => "arqid" , "valor" => $fotos[$i]["arqid"]),
													"arqdescricao" 	=> array("label" => "arqdescricao" , "valor" => $fotos[$i]["arqdescricao"])
											     )
	                           );
					
				}
				
				$i = $i+1;
				
				if( $fotos[$i]["arqid"] ){
					
					array_push($arrauxiliar,
	                                       array(
													"arqid" 		=> array("label" => "arqid" , "valor" => $fotos[$i]["arqid"]),
													"arqdescricao" 	=> array("label" => "arqdescricao" , "valor" => $fotos[$i]["arqdescricao"])
											     )
	                           );
					
				}
				
			}
			
		}else{
			
			$arrauxiliar = array(
									"ERRO" 	=> array("label" => "ERRO", "valor" => "Nï¿½o existem fases de licitaï¿½ï¿½o cadastradas para a obra.")
								 );
			
		}			
		
		$tabelaFotos .= "</table>";
		
		#concatenando os arrays
		foreach($arrauxiliar as $key => $valor){
			$arrSerialize['Galeria de Fotos'][$key] = $valor;
		}
		
	}
	
	// Orï¿½amento para a Obra
	$execOrc = new execOrcamentaria();
	$dadosExecOrc = $execOrc->buscaExecOrcamentaria( $obrid );
	$arrauxiliar = array();
	
	if ( $dadosExecOrc ){
		
		$nEocvlrtotal = $dadosExecOrc["eocvlrcapital"] + $dadosExecOrc["eocvlrcusteio"];
		$eocvlrtotal  = number_format($nEocvlrtotal, 2, ",", ".");
		
		$arrauxiliar = array(
								"eocvlrcapital" => array("label" => "Capital (R$)" , "valor" => number_format($dadosExecOrc["eocvlrcapital"], 2, ",", ".") ),
								"eocvlrcusteio" => array("label" => "Custeio (R$)" , "valor" => number_format($dadosExecOrc["eocvlrcusteio"], 2, ",", ".") ),
								"eocvlrtotal" 	=> array("label" => "Total (R$)" , "valor"   => $eocvlrtotal)
						     );
		
	}else{
		
		$arrauxiliar = array(
								"ERRO" 	=> array("label" => "ERRO", "valor" => "Nï¿½o existem Orï¿½amentos cadastrados para a obra.")
							 );
		
	}
	#concatenando os arrays
	foreach($arrauxiliar as $key => $valor){
		$arrSerialize['Orï¿½amento para a Obra'][$key] = $valor;
	}
	// Fim do Orï¿½amento para a Obra
	
	// Detalhamento Orï¿½amentï¿½rio
	$arrauxiliar = array();
	
	if( $dadosExecOrc ){
		
		$sql = "SELECT
				eocvlrempenhado,
				eocvlrliquidado,
				eocdtposicao				
			FROM
				 obras.itensexecucaoorcamentaria
			WHERE
				eorid = {$dadosExecOrc["eorid"]}
			ORDER BY
				eocdtposicao";
		
		$dadosItensExec =  $db->carregar( $sql );
		
		for( $i = 0; $i < count($dadosItensExec); $i++ ){
			
			$perEmpenhado = ($nEocvlrtotal > 0) ? ( $dadosItensExec[$i]["eocvlrempenhado"] / $nEocvlrtotal ) * 100 : 0.00;
			$perLiquidado = ($nEocvlrtotal > 0) ? ( $dadosItensExec[$i]["eocvlrliquidado"] / $nEocvlrtotal ) * 100 : 0.00;
			
			$totEmpenhado = $totEmpenhado + $dadosItensExec[$i]["eocvlrempenhado"];
			$totLiquidado = $totLiquidado + $dadosItensExec[$i]["eocvlrliquidado"];
				
			$totPerEmpenhado = $totPerEmpenhado + $perEmpenhado;
			$totPerLiquidado = $totPerLiquidado + $perLiquidado;
			
			array_push($arrauxiliar,
										array(
												"eocdtposicao" 		=> array("label" => "Data" , "valor" => formata_data($dadosItensExec[$i]["eocdtposicao"]) ),
												"eocvlrempenhado" 	=> array("label" => "Valor Empenhado (R$)" , "valor" => number_format($dadosItensExec[$i]["eocvlrempenhado"], 2, ",", ".") ),
												"eocvlrliquidado" 	=> array("label" => "Valor Liquidado (R$)" , "valor" => number_format($dadosItensExec[$i]["eocvlrliquidado"], 2, ",", ".") ),
												"perEmpenhado" 		=> array("label" => "% Empenhado" , "valor" => number_format($perEmpenhado, 2, ",", ".") ),
												"perLiquidado" 		=> array("label" => "% Liquidado" , "valor" => number_format($perLiquidado, 2, ",", ".") )
										      )
	                   );
			
		}// fim do for
		
		//total
		array_push($arrauxiliar,
										array(
												"Total" 			=> array("label" => "Total" , "valor" => "Total" ),
												"totEmpenhado" 		=> array("label" => "Valor Empenhado (R$)" , "valor" => number_format($totEmpenhado, 2, ",", "." ) ),
												"totLiquidado" 		=> array("label" => "Valor Liquidado (R$)" , "valor" => number_format($totLiquidado, 2, ",", "." ) ),
												"totPerEmpenhado" 	=> array("label" => "% Empenhado" , "valor" => number_format($totPerEmpenhado, 2, ",", ".") ),
												"totPerLiquidado" 	=> array("label" => "% Liquidado" , "valor" => number_format($totPerLiquidado, 2, ",", ".") )
										      )
	                   );
		
	}else{
		
		$arrauxiliar = array(
								"ERRO" 	=> array("label" => "ERRO", "valor" => "Nï¿½o existem Detalhamentos Orï¿½amentï¿½rios cadastrados para a obra.")
							 );
	}// fim do if
	#concatenando os arrays
	foreach($arrauxiliar as $key => $valor){
		$arrSerialize['Detalhamento Orï¿½amentï¿½rio'][$key] = $valor;
	}
	// Fim Detalhamento Orï¿½amentï¿½rio
	
	// Restriï¿½ï¿½es e Providï¿½ncias
	$sql = "SELECT
				CASE WHEN fsrid is not null THEN fsrdsc ELSE 'Nï¿½o Informada' END as fase,
				rstdesc,
				trtdesc,
				rstdescprovidencia,
				to_char(rstdtprevisaoregularizacao,'DD/MM/YYYY') as rstdtprevisaoregularizacao,
				CASE WHEN rstsituacao = true THEN to_char(rstdtsuperacao,'DD/MM/YYYY') ELSE 'Nï¿½o' END AS rstdtsuperacao
			FROM
				obras.restricaoobra 
			INNER JOIN 
				obras.tiporestricao USING (trtid)
			LEFT JOIN
				obras.faserestricao USING (fsrid) 
			WHERE
				rststatus = 'A' AND
				obrid = " . $obrid;
	
	$arrauxiliar = array();	
	$dadosRestricoes =  $db->carregar( $sql );
	
	if($dadosRestricoes){
		$tabelaRestricoes .= "    <tr>"
						  . "        <td class='subtitulocentro'>Fase da Restriï¿½ï¿½o</td>"
						  . "        <td class='subtitulocentro'>Tipo de Restriï¿½ï¿½o</td>"
						  . "        <td class='subtitulocentro'>Restriï¿½ï¿½o</td>"
						  . "        <td class='subtitulocentro'>Providï¿½ncia</td>"
						  . "        <td class='subtitulocentro'>Previsï¿½o da Providï¿½ncia</td>"
						  . "        <td class='subtitulocentro'>Superaï¿½ï¿½o</td>"
						  . "    </tr>";

		for( $i = 0; $i < count($dadosRestricoes); $i++ ){
			
			array_push($arrauxiliar,
										array(
												"eocdtposicao" 		=> array("label" => "Fase da Restriï¿½ï¿½o" , "valor" => $dadosRestricoes[$i]["fase"] ),
												"eocvlrempenhado" 	=> array("label" => "Tipo de Restriï¿½ï¿½o" , "valor" => $dadosRestricoes[$i]["trtdesc"] ),
												"eocvlrliquidado" 	=> array("label" => "Restriï¿½ï¿½o" , "valor" => $dadosRestricoes[$i]["rstdesc"] ),
												"perEmpenhado" 		=> array("label" => "Providï¿½ncia" , "valor" => $dadosRestricoes[$i]["rstdescprovidencia"] ),
												"perLiquidado" 		=> array("label" => "Previsï¿½o da Providï¿½ncia" , "valor" => $dadosRestricoes[$i]["rstdtprevisaoregularizacao"] ),
												"perLiquidado" 		=> array("label" => "Superaï¿½ï¿½o" , "valor" => $dadosRestricoes[$i]["rstdtsuperacao"] )
										      )
	                   );
			
		}// fim do for
						  
	}else{
		$arrauxiliar = array(
								"ERRO" 	=> array("label" => "ERRO", "valor" => "Nï¿½o existem Restriï¿½ï¿½es e Providï¿½ncias cadastradas para a obra.")
							 );
	}// fim do if
	#concatenando os arrays
	foreach($arrauxiliar as $key => $valor){
		$arrSerialize['Restriï¿½ï¿½es e Providï¿½ncias'][$key] = $valor;
	}
	// Fim do Restriï¿½ï¿½es e Providï¿½ncias
	
	if( $vistoria ){
		
		$arrauxiliar = array();
		
		$WhereVistorias = $supvids != '' ? "s.supvid IN (" . $supvids . ") AND " : '';
		
		$sql = "SELECT
					supvid as vistoria,
					s.suprealizacao as responsavel,
					u.usunome as inseridopor,
					to_char(s.supvdt,'DD/MM/YYYY') as dtvistoria,
					CASE WHEN s.supvistoriador is not null THEN ev.entnome ELSE 'Nï¿½o informado' END as vistoriador,
					si.stodesc as situacao,
					CASE WHEN supprojespecificacoes = 't' THEN 'Sim' ELSE 'Nï¿½o' END as projetoespecificacoes,
					CASE WHEN supplacaobra = 't' THEN 'Sim' ELSE 'Nï¿½o' END as placaobra,
					CASE WHEN supdiarioobra = 't' THEN 'Sim' ELSE 'Nï¿½o' END as diarioobra,
					CASE WHEN supplacalocalterreno = 't' THEN 'Sim' ELSE 'Nï¿½o' END as placalocalterreno,
					CASE WHEN supvalidadealvara = 't' THEN 'Sim' ELSE 'Nï¿½o' END as validadealvara,
					qlbdesc as qualidadeobra,
					dcndesc as desempenho,
					CASE WHEN supobs != '' THEN supobs ELSE 'Nï¿½o informado' END as observacao
				FROM
					obras.supervisao s
				INNER JOIN 
					obras.situacaoobra si ON si.stoid = s.stoid
				INNER JOIN
					seguranca.usuario u ON u.usucpf = s.usucpf
				LEFT JOIN
					entidade.entidade ev ON ev.entid = s.supvistoriador
				LEFT JOIN
					obras.qualidadeobra oq ON oq.qlbid = s.qlbid
				LEFT JOIN
					 obras.desempenhoconstrutora od ON od.dcnid = s.dcnid
				WHERE
					" . $WhereVistorias . " 
					s.obrid = '{$obrid}' AND
					s.supstatus = 'A'
				ORDER BY 
					s.supdtinclusao ASC";
		
		$totVistorias = $db->carregar( $sql );
	
		if ($totVistorias){
		
			for( $i = 0; $i < count($totVistorias); $i++ ){
				
				$sql = "SELECT
							itco.icoid,
							itc.itcdesc,
							itco.icovlritem,
							itco.icopercsobreobra, 
							to_char(itco.icodtinicioitem, 'DD/MM/YYYY') as inicio,
							to_char(itco.icodterminoitem, 'DD/MM/YYYY') as termino,
							itco.icopercprojperiodo,
							itco.icopercexecutado,
							sup.supvlrinfsupervisor,
							sup.supvlritemexecanterior,
							sup.supvlritemsobreobraexecanterior,
							sup.supvid
						FROM 
							obras.itenscomposicao itc,
							obras.itenscomposicaoobra itco
						LEFT JOIN
							obras.supervisaoitenscomposicao sup ON sup.icoid = itco.icoid AND
																   sup.supvid = {$totVistorias[$i]["vistoria"]}
						WHERE								
							--itc.itcid = itco.itcid AND
							itco.obrid = {$obrid}
						ORDER BY 
							icoordem";
						
				$dadosItensVistoria = $db->carregar( $sql );
				
				$tabelaItensVistoria = "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>";
				$arrayItensVistoria = array();
				
				if( $dadosItensVistoria ){
					
					$totalValor 		= 0;
					$totalPercObra 		= 0;
					$totalPercObraAnt   = 0;
					$totalPercObraAtual = 0;
					
					for( $k = 0; $k < count($dadosItensVistoria); $k++ ){
						
						$supervisao_exec_sobre_obra = ( ((float)$dadosItensVistoria[$k]["supvlrinfsupervisor"] * (float)$dadosItensVistoria[$k]["icopercsobreobra"]) / 100 );
						
						$totalValor 		= $totalValor 		  + $dadosItensVistoria[$k]["icovlritem"];
						$totalPercObra 		= $totalPercObra 	  + $dadosItensVistoria[$k]["icopercsobreobra"];
						$totalPercObraAnt   = $totalPercObraAnt   + $dadosItensVistoria[$k]["supvlritemsobreobraexecanterior"];
						$totalPercObraAtual = $totalPercObraAtual + $supervisao_exec_sobre_obra;
						
						array_push($arrayItensVistoria,
	                                       array(
													"itcdesc" 			=> array("label" => "Item da Obra" , "valor" => $dadosItensVistoria[$k]["itcdesc"]),
	                                       			"icovlritem" 		=> array("label" => "Valor (R$)" , "valor" => number_format( $dadosItensVistoria[$k]["icovlritem"], 2, ",", "." )),
	                                       			"icopercsobreobra" 	=> array("label" => "(%) Sobre a Obra" , "valor" => number_format( $dadosItensVistoria[$k]["icopercsobreobra"], 2, ",", "." )),
	                                       			"inicio" 			=> array("label" => "Data de Inï¿½cio" , "valor" => $dadosItensVistoria[$k]["inicio"]),
	                                       			"termino" 			=> array("label" => "Data de Tï¿½rmino" , "valor" => $dadosItensVistoria[$k]["termino"]),
	                                       			"ï¿½ltima Vistoria" 	=> array("label" => "ï¿½ltima Vistoria" , array(
	                                       																"supvlritemexecanterior" => array("label" => "(%) do Item jï¿½ Executado" , "valor" => number_format( $dadosItensVistoria[$k]["supvlritemexecanterior"], 2, ",", "." )),
	                                       																"supvlritemsobreobraexecanterior" => array("label" => "(%) do Item jï¿½ Executado <br/> sobre a Obra" , "valor" => number_format( $dadosItensVistoria[$k]["supvlritemsobreobraexecanterior"], 2, ",", "." ))
	                                       															   )),
	                                       			"Vistoria Atual" 	=> array("label" => "Vistoria Atual" , array(
	                                       															   "supvlrinfsupervisor" => array("label" => "(%) Supervisï¿½o" , "valor" => number_format( $dadosItensVistoria[$k]["supvlrinfsupervisor"], 2, ",", "." )),
	                                       															   "icopercsobreobra" => array("label" => "(%) do Item jï¿½ Executado sobre a  <br/> Obra apï¿½s Supervisï¿½o" , "valor" => number_format( $supervisao_exec_sobre_obra, 2, ",", "." ))
	                                       															  )),
	                                       			
											     )
	                           	   );
						
					}
	
					$totalPercObra = ($totalPercObra > 100.00) ? 100.00 : $totalPercObra;
					
					$arrauxiliar = array(
											"icovlritem" 						=> array("label" => "Valor (R$)", "valor" => number_format( $totalValor, 2, ",", "." )),
											"icopercsobreobra" 					=> array("label" => "(%) Sobre a Obra", "valor" => number_format( $totalPercObra, 2, ",", "." )),
											"supvlritemsobreobraexecanterior" 	=> array("label" => "(%) do Item jï¿½ Executado <br/> sobre a Obra", "valor" => number_format( $totalPercObraAnt, 2, ",", "." )),
											"supvlrinfsupervisor" 				=> array("label" => "(%) do Item jï¿½ Executado sobre a  <br/> Obra apï¿½s Supervisï¿½o", "valor" => number_format( $totalPercObraAtual, 2, ",", "." ))
								 		 );
					
					#concatenando os arrays
					foreach($arrauxiliar as $key => $valor){
						$arrayItensVistoria['Total'][$key] = $valor;
					}
					
				}else{
					
					$arrayItensVistoria = array(
											"ERRO" 	=> array("label" => "ERRO", "valor" => "Nï¿½o existem itens cadastradas para esta vistoria.")
								 		 );
					
				}
				
				$tabelaItensVistoria .= "</table>";
				
				
				// fotos
				
				$WhereFotos = $fotoselecionadas != '' ? "fo.arqid IN (" . $fotoselecionadas . ") AND " : '';
				
				$sql = "SELECT 
							arqnome, 
							arq.arqid, 
							arq.arqextensao, 
							arq.arqtipo, 
							arq.arqdescricao 
						FROM 
							public.arquivo arq
						INNER JOIN 
							obras.fotos fo ON arq.arqid = fo.arqid
						WHERE
							{$WhereFotos}
							supvid = {$totVistorias[$i]["vistoria"]}
						ORDER BY 
							fotordem";
			
				$fotosVistoria = $db->carregar($sql);				 
								 
				if( $fotosVistoria ){
					
					$tabelaFotosVistoria = "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>";
					
					for( $a = 0; $a < count( $fotosVistoria ); $a++ ){
						
						if( $fotosVistoria[$a]["arqid"] ){
													
							array_push($arrayFotosVistoria,
	                                       array(
													"arqid" 		=> array("label" => "arqid" , "valor" => $fotosVistoria[$a]["arqid"]),
													"arqdescricao" 	=> array("label" => "arqdescricao" , "valor" => $fotosVistoria[$a]["arqdescricao"])
											     )
	                           		    );
							
						}
						
						$a = $a+1;
						
						if( $fotosVistoria[$a]["arqid"] ){
							
							array_push($arrayFotosVistoria,
	                                       array(
													"arqid" 		=> array("label" => "arqid" , "valor" => $fotosVistoria[$a]["arqid"]),
													"arqdescricao" 	=> array("label" => "arqdescricao" , "valor" => $fotosVistoria[$a]["arqdescricao"])
											     )
	                           		    );
							
						}
						
						$a = $a+1;
						
						if( $fotosVistoria[$a]["arqid"] ){
							
							array_push($arrayFotosVistoria,
	                                       array(
													"arqid" 		=> array("label" => "arqid" , "valor" => $fotosVistoria[$a]["arqid"]),
													"arqdescricao" 	=> array("label" => "arqdescricao" , "valor" => $fotosVistoria[$a]["arqdescricao"])
											     )
	                           		    );
							
						}
						
					}
			
					
					
				}else{
					
					$arrayFotosVistoria = array(
											"ERRO" 	=> array("label" => "ERRO", "valor" => "Nï¿½o existem fotos cadastradas para esta vistoria.")
								 		 );
								 
				}			
				
				$tabelaFotosVistoria .= "</table>";
	
				$arrayVistorias["Vistoria nï¿½ ". ($i + 1)] = array();
				
				array_push($arrayVistorias["Vistoria nï¿½ ". ($i + 1)], array(
												"responsavel" 			=> array("label" => "Responsï¿½vel", "valor" => $totVistorias[$i]["responsavel"]),
												"inseridopor" 			=> array("label" => "Inserido Por", "valor" => $totVistorias[$i]["inseridopor"]),
												"dtvistoria" 			=> array("label" => "Data da Vistoria", "valor" => $totVistorias[$i]["dtvistoria"]),
												"vistoriador" 			=> array("label" => "Nome do Vistoriador", "valor" => $totVistorias[$i]["vistoriador"]),
												"situacao" 				=> array("label" => "Situaï¿½ï¿½o atual", "valor" => $totVistorias[$i]["situacao"]),
												"projetoespecificacoes" => array("label" => "Projeto/Especificaï¿½ï¿½es", "valor" => $totVistorias[$i]["projetoespecificacoes"]),
												"placaobra" 			=> array("label" => "Placa da Obra", "valor" => $totVistorias[$i]["placaobra"]),
												"diarioobra" 			=> array("label" => "Diï¿½rio da Obra Atualizado", "valor" => $totVistorias[$i]["diarioobra"]),
												"placalocalterreno" 	=> array("label" => "Placa Indicativa do Programa/Dados da obra", "valor" => $totVistorias[$i]["placalocalterreno"]),
												"validadealvara" 		=> array("label" => "Validade do Alvarï¿½ da Obra", "valor" => $totVistorias[$i]["validadealvara"]),
												"qualidadeobra" 		=> array("label" => "Qualidade de Execuï¿½ï¿½o da Obra/Projeto", "valor" => $totVistorias[$i]["qualidadeobra"]),
												"desempenho" 			=> array("label" => "Desempenho da Construtora/Projetista", "valor" => $totVistorias[$i]["desempenho"]),
												"arrayItensVistoria" 	=> array("label" => "Detalhamento de Vistoria e Acompanhamento", "valor" => $arrayItensVistoria),
												"observacao" 			=> array("label" => "Observaï¿½ï¿½es da Vistora", "valor" => $totVistorias[$i]["observacao"]),
												"arrayFotosVistoria" 	=> array("label" => "Fotos", "valor" => $arrayFotosVistoria)
											  )
						   );
	
			}
			
		}else{
			
			$arrauxiliar += array(
									"ERRO" 	=> array("label" => "ERRO", "valor" => "Nï¿½o existem vistorias cadastradas para a obra.")
								 );
			
		}
		
		#concatenando os arrays
		foreach($arrayVistorias as $key => $valor){
			$arrSerialize[$key] = $valor;
		}
		
	}

		if ( $obrid && $_GET['traid'] ):
			$sql = "SELECT 
					   t.ttaid, ttadsc, usunome, umdidareaacresc, umdidareafinal, umdidareaalterada, 
				       obrid, tradsc, traseq, to_char(tradtassinatura, 'dd-mm-yyyy') as tradtassinatura, traprazovigencia, to_char(traterminovigencia, 'dd-mm-yyyy') as traterminovigencia, 
				       traprazoaditivadoexec, travlraditivo, travlrfinalobra, travlrqtdareaacresc, 
				       travlrqtdareafinal, travlrqtdareaalterada, trajustificativa, 
				       to_char(tradtinclusao, 'dd-mm-yyyy') as tradtinclusao, trastatus, to_char(traterminoexec, 'dd-mm-yyyy') as traterminoexec
				    FROM 
				    	obras.termoaditivo t
				    JOIN seguranca.usuario u USING(usucpf)
				    JOIN obras.tipotermoaditivo tt USING(ttaid)
				    WHERE
						traid = " . $_GET['traid'];
			$arrDadoAditivo = $db->pegaLinha( $sql );
	
			$arrSerialize += array( 
						"Dados do Aditivo" => array( 
													"traseq" => array("label" => "Nï¿½ do Aditivo", "valor" => $arrDadoAditivo['traseq']),
													"tradsc" => array("label" => "Denominaï¿½ï¿½o", "valor" => $arrDadoAditivo['tradsc']),
													"ttadsc" => array("label" => "Tipo de Aditivo", "valor" => $arrDadoAditivo['ttadsc']),
													"tradtassinatura" => array("label" => "Data de Assinatura do Aditivo", "valor" => $arrDadoAditivo['tradtassinatura'])
											     )
						  );
			
		if ( $arrDadoAditivo['ttaid'] == 1 || $arrDadoAditivo['ttaid'] == 3 ):
			$arrSerialize["Dados do Aditivo"] += array(
														"traprazovigencia" => array("label" => "Prazo de Vigï¿½ncia do Aditivo", "valor" => $arrDadoAditivo['traprazovigencia']),
														"traterminovigencia" => array("label" => "Tï¿½rmino da Vigï¿½ncia do Aditivo", "valor" => $arrDadoAditivo['traterminovigencia']),
														"traprazoaditivadoexec" => array("label" => "Prazo aditivado para Execuï¿½ï¿½o", "valor" => $arrDadoAditivo['traprazoaditivadoexec']),
														"traterminoexec" => array("label" => "Tï¿½rmino da Execuï¿½ï¿½o do Aditivo", "valor" => $arrDadoAditivo['traterminoexec'])
													  );
		endif;
		
		if ( $arrDadoAditivo['ttaid'] == 2 || $arrDadoAditivo['ttaid'] == 3 ):
			$arrSerialize["Dados do Aditivo"] += array(
														"travlraditivo" => array("label" => "Valor do Aditivo(R$)", "valor" => $arrDadoAditivo['travlraditivo']),
														"travlrfinalobra" => array("label" => "Valor Final da Obra Incluindo Aditivo(R$)", "valor" => $arrDadoAditivo['travlrfinalobra'])
													  );
	
			if ( $arrDadoAditivo['ttaid'] == 2 ):
				$arrSerialize["Dados do Aditivo"] += array(
														"travlrqtdareaacresc" => array("label" => "Acrï¿½scimo ou Supressï¿½o de ï¿½rea/Quantidade", "valor" => $arrDadoAditivo['travlrqtdareaacresc']),
														"umdidareaacresc" => array("label" => "Unidade de medida", "valor" => $arrDadoAditivo['umdidareaacresc'])
													  		);
			else:
				$arrSerialize["Dados do Aditivo"] += array(
														"travlrqtdareaalterada" => array("label" => "Alteraï¿½ï¿½o da ï¿½rea/Quantidade", "valor" => $arrDadoAditivo['travlrqtdareaalterada']),
														"umdidareaalterada" => array("label" => "Unidade de medida", "valor" => $arrDadoAditivo['umdidareaalterada'])
													  		);
			endif;
			
			$arrSerialize["Dados do Aditivo"] += array(
														"travlrqtdareafinal" => array("label" => "ï¿½rea/Quantidade Final Incluindo Aditivo(R$)", "valor" => $arrDadoAditivo['travlrqtdareafinal']),
														"umdidareafinal" => array("label" => "Unidade de medida", "valor" => $arrDadoAditivo['umdidareafinal'])
													  		);
		endif;
		
		$arrSerialize["Dados do Aditivo"] += array(
													"trajustificativa" => array("label" => "Justificativa", "valor" => $arrDadoAditivo['trajustificativa'])
												   );
		endif;
	
	#tipo de ensino
	$sql = "SELECT orgdesc as descricao FROM obras.orgao WHERE orgid = " . (int)$dobras->getOrgId();					
	$orgid = $db->pegaUm( $sql );
	
	#Unidade Implementadora
	$entidade = new Entidade( $dobras->getEntIdUnidade() );
	$entnome = $entidade->entnome;
	
	#Campus / Unidade
//	$campus = new Entidade($dobras->getEntIdCampus());
//	$campusnome = $campus->entnome;
	
	#Nome da Obra
	$obranome = $dobras->getObrDesc();
	
	#Subaï¿½ï¿½o
	if($dobras->getPrfId()){
		$sql = "SELECT prfdesc as descricao FROM obras.programafonte WHERE prfid= ".$dobras->getPrfId();
		$subacao = $db->pegaUm( $sql );
	}else{
		$subacao = "Nï¿½o informado";
	}
	
	#Classificaï¿½ï¿½o da Obra
	if( $dobras->getCloId() ){
		$sql = "SELECT clodsc as descricao FROM obras.classificacaoobra WHERE cloid = ".$dobras->getCloId();
		$classificacao = $db->pegaUm( $sql );
	}else{
		$classificacao = "Nï¿½o informado";
	}
	
	#Tipologia da Obra
	if ( $dobras->getTpoId() ){
		$sql = "SELECT tpodsc as descricao FROM obras.tipologiaobra WHERE tpoid = ".$dobras->getTpoId();
		$tipologia = $db->pegaUm( $sql );
	}else{
		$tipologia = "Nï¿½o informado";
	}
	
	#Descriï¿½ï¿½o / Composiï¿½ï¿½o da Obra
	if($dobras->getObrComposicao()){
		$descricao = $dobras->getObrComposicao();
	}else{
		$descricao = "Nï¿½o Informado";
	}
	
	#Observaï¿½ï¿½o sobre a Obra
	if($dobras->getObsObra()){
		$observacao = $dobras->getObsObra();
	}else{
		$observacao = "Nï¿½o Informado";
	}
	
	#Valor Previsto (R$)
	$valorPrevisto = $dobras->getObrValorPrevisto() ? number_format($dobras->getObrValorPrevisto(), 2, ",", ".") : "Nï¿½o Informado";
	
	#(%) Concluï¿½do
	/*$percentualExecutado = $obras->ViewPercentualExecutado($obrid);
					
	if( !$percentualExecutado ){
		$percentualExecutado = 0;
	}
	$percentualExecutado = ( $percentualExecutado > 100.00 ) ? 100.00 : $percentualExecutado;
	$percentualExecutado = number_format($percentualExecutado, 2, ',', '.');*/
	
	$arrSerialize += array( 
						"Dados da Obra" => array( 
													"orgdesc"		=> array("label" => "Tipo de Estabelecimento", "valor" 					=> $orgid),
													"entidunidade"	=> array("label" => "Unidade ResponsÃ¡vel pela Obra", "valor" 			=> $entnome),
													//"entidcampus"	=> array("label" => "Campus / Unidade", "valor" 				=> $campusnome),
													"obrdesc"		=> array("label" => "Nome da Obra", "valor" 					=> $obranome),
	
													"tobadesc"		=> array("label" => "Tipo de Obra", "valor" 					=> $tpContratacao),
	
													"prfdesc"		=> array("label" => "Subaï¿½ï¿½o", "valor" 							=> $subacao),
													"clodsc"		=> array("label" => "Classificaï¿½ï¿½o da Obra", "valor" 			=> $classificacao),
													"tpodsc"		=> array("label" => "Tipologia da Obra", "valor" 				=> $tipologia),
													"obrcomposicao"	=> array("label" => "Descriï¿½ï¿½o / Composiï¿½ï¿½o da Obra", "valor" 	=> $descricao),
													"obsobra"		=> array("label" => "Observaï¿½ï¿½o sobre a Obra", "valor" 			=> $observacao),
													"valorPrevisto"	=> array("label" => "Valor Previsto (R$)", "valor" 				=> $valorPrevisto)														
													//"obrpercexec" 	=> array("label" => "(%) Concluï¿½do", "valor" 					=> $percentualExecutado)
											     )
						  );
	
	#serializando o array resultante
	$arrayString = pg_escape_string(serialize($arrSerialize));
	
	#Inserindo no banco de dados
	$sql = "INSERT INTO
				seguranca.historicoalteracao
				(sisid, usucpf, haltxt, haldata)
			VALUES
				(".$_SESSION['sisid'].", '".$_SESSION['usucpf']."', '".$arrayString."', 'now()') 
			RETURNING halid;";
	
	
	$halid = $db->pegaUm($sql);
	
	$sql = "SELECT
				orsid
			FROM 
				obras.ordemservico 
			WHERE 
				gpdid = " . $gpdid;
	
	$orsid = $db->pegaUm($sql);
	
	$sql = "INSERT INTO
				obras.historicosupervisao
				(orsid, obrid, halid)
			VALUES
				(". $orsid . ", " . $obrid . ", " . $halid . ");";
	
	$db->executar($sql);

	$db->commit();


}

/**
 * Funï¿½ï¿½o que gera o HTML do extrato salvo na tabela seguranca.historicoalteracao;
 * $halid ï¿½ obrigatï¿½rio
 * Se $arrayTmp for um array, entï¿½o ele escreve o array que estï¿½ nesta variï¿½vel
 * @author Rodrigo Pereira de Souza Silva
 * @param integer $halid
 * @param unknown_type $arrayTmp
 */
function gerarExtratoObrasHTML($halid, $arrayTmp = ''){
	
	global $db;
	
	$sql = "SELECT
				ha.halid,
				ha.haltxt,
				u.usunome,
				to_char(ha.haldata, 'DD/MM/YYYY') as data,
				to_char(ha.haldata, 'HH24:MI') as hora
			FROM
				seguranca.historicoalteracao ha,
				seguranca.usuario u
			WHERE
				ha.halid = " . $halid . "
				AND u.usucpf = ha.usucpf;";
	
	$dados = $db->carregar($sql);

	if(is_array($arrayTmp)){
		$arrayExtrato = $arrayTmp;
		unset($arrayTmp);
	}else{
		$arrayExtrato = unserialize($dados[0]['haltxt']);
	}
	
	$html = '<center>
				<table width="95%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug"  style="border-bottom: 1px solid;">
					<tr bgcolor="#ffffff">
						<td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
							<H1>Histï¿½rico da Supervisï¿½o</H1><br />
						</td>
						<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">
							Gerado por: <b>' . $dados[0]['usunome'] . '</b><br/>
							Data: <b>' . $dados[0]['data'] . '</b> Hora: <b>' . $dados[0]['hora'] . '</b><br/>
							Nï¿½mero do Histï¿½rico: <b>' . $dados[0]['halid'] . '</b><br />
						</td>				
					</tr>
					<tr>
						<td colspan="2" align="center" valign="top" style="padding:0 0 5px 0;">
							<b><font style="font-size:14px;">' . $_REQUEST["titulo"] . '</font></b>
						</td>
					</tr>				
				</table>
			</center>';
				
	$html .= '<html>
				<head>
					<title>'.$GLOBALS['parametros_sistema_tela']['sigla-nome_completo'].'</title>
					<script type="text/javascript" src="../includes/funcoes.js"></script>
				    <script type="text/javascript" src="../includes/prototype.js"></script>
				    <script type="text/javascript" src="../includes/entidades.js"></script>
				    <script type="text/javascript" src="/includes/estouvivo.js"></script>
					<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
					<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
					<style>
						 @media print {
						 	.notprint { display: none }
						 }
					</style>
				</head>
				<body>';
	
	if(isset($arrayExtrato['Dados do Aditivo'])){
		$html .= '<table class="tabela" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="subtitulocentro" colspan="4" height="25px;">Dados do Aditivo</td>
					</tr>
					<tr>
						<td class="subtitulodireita" style="font-weight: bold;" width="190px;">Nï¿½ do Aditivo</td>
						<td colspan="3" bgcolor="#f5f5f5">
							'.$arrayExtrato['Dados do Aditivo']['traseq']["label"]["valor"].'
						</td>
					</tr>	
					<tr>
						<td class="subtitulodireita" style="font-weight: bold;" width="190px;">Denominaï¿½ï¿½o</td>
						<td colspan="3" bgcolor="#f5f5f5">
							'.$arrayExtrato['Dados do Aditivo']['tradsc']["label"]["valor"].'
						</td>
					</tr>	
					<tr>
						<td class="subtitulodireita" style="font-weight: bold;" width="190px;">Tipo de Aditivo</td>
						<td colspan="3" bgcolor="#f5f5f5">
							'.$arrayExtrato['Dados do Aditivo']['ttadsc']["label"]["valor"].'
						</td>
					</tr>	
					<tr>
						<td class="subtitulodireita" style="font-weight: bold;" width="190px;">Data de Assinatura do Aditivo</td>
						<td colspan="3" bgcolor="#f5f5f5">
							'.$arrayExtrato['Dados do Aditivo']['tradtassinatura']["label"]["valor"].'
						</td>
					</tr>';
	}
	
	if(isset($arrayExtrato['Dados da Obra'])){
		$html .= '<table class="tabela" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="subtitulocentro" colspan="2" height="25px;">Dados da Obra</td>
					</tr>
					<tr>
						<td class="subtitulodireita" style="font-weight: bold;" width="190px;">Tipo de estabelecimento</td>
						<td bgcolor="#f5f5f5">
							'.$arrayExtrato['Dados da Obra']['orgdesc']['valor'].'
						</td>
			    	</tr>	
			    	<tr>
						<td class="subtitulodireita" style="font-weight: bold;">Unidade ResponsÃ¡vel pela Obra</td>
						<td>
							'.$arrayExtrato['Dados da Obra']['entidunidade']['valor'].'
						</td>
			    	</tr>
			    <!--<tr>
						<td class="subtitulodireita" style="font-weight: bold;">Campus / Unidade</td>
						<td bgcolor="#f5f5f5">
							'.$arrayExtrato['Dados da Obra']['entidcampus']['valor'].'
						</td>
			    	</tr>-->
			    	<tr>
						<td class="subtitulodireita" style="font-weight: bold;">Nome da Obra</td>
						<td>
							'.$arrayExtrato['Dados da Obra']['obrdesc']['valor'].'
						</td>
			    	</tr>
			    	
			    	<tr>
						<td class="subtitulodireita" style="font-weight: bold;">Tipo de Obra</td>
						<td>
							'.$arrayExtrato['Dados da Obra']['tobadesc']['valor'].'
						</td>
			    	</tr>
			    	
			    	<tr>
						<td class="subtitulodireita" style="font-weight: bold;">Subaï¿½ï¿½o</td>
						<td>
							'.$arrayExtrato['Dados da Obra']['prfdesc']['valor'].'
						</td>
			    	</tr>
			    	<tr>
						<td class="subtitulodireita" style="font-weight: bold;">Classificaï¿½ï¿½o da Obra</td>
						<td bgcolor="#f5f5f5">
							'.$arrayExtrato['Dados da Obra']['clodsc']['valor'].'
						</td>
			    	</tr>
			    	<tr>
						<td class="subtitulodireita" style="font-weight: bold;">Tipologia da Obra</td>
						<td>
							'.$arrayExtrato['Dados da Obra']['tpodsc']['valor'].'
						</td>
			    	</tr>						
			    	<tr>
						<td class="subtitulodireita" style="font-weight: bold;">Descriï¿½ï¿½o / Composiï¿½ï¿½o da Obra</td>
						<td bgcolor="#f5f5f5" align="justify">
							'.$arrayExtrato['Dados da Obra']['obrcomposicao']['valor'].'
						</td>
			    	</tr>	
					<tr>
						<td class="subtitulodireita" style="font-weight: bold;">Observaï¿½ï¿½o sobre a Obra</td>
						<td align="justify">
							'.$arrayExtrato['Dados da Obra']['obsobra']['valor'].'
						</td>
			    	</tr>	    		
			    <!--<tr>
						<td class="subtitulodireita" style="font-weight: bold;">(%) Concluï¿½do</td>
						<td>
						'.$arrayExtrato['Dados da Obra']['obrpercexec']['valor'].'
						</td>
			    	</tr>-->
			    	<tr>
						<td class="subtitulodireita" style="font-weight: bold;">Valor Previsto (R$)</td>
						<td>
						'.$arrayExtrato['Dados da Obra']['valorPrevisto']['valor'].'
						</td>
			    	</tr>';
	}
	
	if(isset($arrayExtrato['Contatos'])){
		$tabelaContatos = "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>";
			
			if ( !$arrayExtrato['Contatos']['ERRO'] ){
			
				$tabelaContatos .= "    <tr>"
								. "        <td class='subtitulocentro'>CPF</td>"
								. "        <td class='subtitulocentro'>Nome do Responsï¿½vel</td>"
								. "        <td class='subtitulocentro'>E-mail</td>"
								. "        <td class='subtitulocentro'>Telefone</td>"
								. "        <td class='subtitulocentro'>Tipo de Responsabilidade</td>"
								. "    </tr>";
				
				$x = count($arrayExtrato['Contatos']);
				for( $i = 0; $i < $x; $i++ ){
					
					$cor = $i % 2 ? "#e0e0e0" : '#ffffff'; 
					
					$telefone = ($arrayExtrato['Contatos'][$i]['entnumdddcomercial']['valor'] ? "(".$arrayExtrato['Contatos'][$i]['entnumdddcomercial']['valor'].") ".$arrayExtrato['Contatos'][$i]['entnumcomercial']['valor'] : "Nï¿½o Informado");
					
					$tabelaContatos .= "    <tr bgcolor='{$cor}'>"
									. "        <td align='center'>{$arrayExtrato['Contatos'][$i]['entnumcpfcnpj']['valor']}</td>"
									. "        <td align='center'>{$arrayExtrato['Contatos'][$i]['entnome']['valor']}</td>"
									. "        <td align='center'>{$arrayExtrato['Contatos'][$i]['entemail']['valor']}</td>"
									. "        <td align='center'>{$telefone}</td>"
									. "        <td align='center'>{$arrayExtrato['Contatos'][$i]['endbai']['valor']}</td>"
									. "    </tr>";	
					
				}
								
			}else{
				
				$tabelaContatos .= "    <tr>"
								. "        <td align='center' style='color:#ee0000'>Nï¿½o existem contatos cadastrados para a obra.</td>"
								. "    </tr>";
				
			}
			
			$tabelaContatos .= "</table>";
			
			$html .=   "<tr>"
					.  "    <td class='subtitulocentro' colspan='2' height='25px;'>Contatos</td>"
					.  "</tr>"
					.  "<tr>"
					.  "    <td colspan='2'>"
					.           $tabelaContatos         
					.  "    </td>"
					.  "</tr>";
	}
	
	if(isset($arrayExtrato['Responsaveis'])){
		
		$tabelaResponsaveis = "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>";
			
		if ( !$arrayExtrato['Responsaveis']['ERRO'] ){
		
			$tabelaResponsaveis .= "<tr>"
							. "         <td class='subtitulocentro'>CPF</td>"
							. "         <td class='subtitulocentro'>Nome do Responsï¿½vel</td>"
							. "         <td class='subtitulocentro'>E-mail</td>"
							. "         <td class='subtitulocentro'>Telefone</td>"
							. "     </tr>";
			
			$x = count($arrayExtrato['Responsaveis']);
			for( $i = 0; $i < $x; $i++ ){
				
				$cor = $i % 2 ? "#e0e0e0" : '#ffffff'; 
				
				$telefone = ($arrayExtrato['Responsaveis'][$i]['usufonenum']['valor'] ? "(".$arrayExtrato['Responsaveis'][$i]['usufoneddd']['valor'].") ".$arrayExtrato['Responsaveis'][$i]['usufonenum']['valor'] : "Nï¿½o Informado");
				$email = ($arrayExtrato['Responsaveis'][$i]['usuemail']['valor'] ? $arrayExtrato['Responsaveis'][$i]['usuemail']['valor'] : "Nï¿½o Informado"); 
				
				$tabelaResponsaveis .= "<tr bgcolor='{$cor}'>"
								. "         <td align='center'>{$arrayExtrato['Responsaveis'][$i]['usucpf']['valor']}</td>"
								. "         <td align='center'>{$arrayExtrato['Responsaveis'][$i]['usunome']['valor']}</td>"
								. "         <td align='center'>{$email}</td>"
								. "         <td align='center'>{$telefone}</td>"
								. "     </tr>";	
				
			}
							
		}else{
			
			$tabelaResponsaveis .= "<tr>"
							. "         <td align='center' style='color:#ee0000'>Nï¿½o existem Responsï¿½veis cadastrados para a obra.</td>"
							. "     </tr>";
			
		}
		
		$tabelaResponsaveis .= "</table>";
		
		$html .=   "<tr>"
				.  "    <td class='subtitulocentro' colspan='2' height='25px;'>Responsï¿½veis</td>"
				.  "</tr>"
				.  "<tr>"
				.  "    <td colspan='2'>"
				.           $tabelaResponsaveis         
				.  "    </td>"
				.  "</tr>";
		
	}
	
	if(isset($arrayExtrato['Contrataï¿½ï¿½o'])){
		$i = 0;
		foreach($arrayExtrato['Contrataï¿½ï¿½o'] as $key => $value){
			$cor = $i % 2 ? "#f5f5f5" : '#ffffff';
			$dadosDetalheForma .=  "<tr>"
									."<td class='subtitulodireita' style='font-weight: bold;'>". $arrayExtrato['Contrataï¿½ï¿½o'][$key]['label'] ."</td>"
									."<td bgcolor='{$cor}'>" . $arrayExtrato['Contrataï¿½ï¿½o'][$key]['valor'] . "</td>"
									."</tr>";
			$i++;
		}
		
		$html .= "<tr>"
					."<td class='subtitulocentro' colspan='2' height='25px;'>Contrataï¿½ï¿½o</td>"
				."</tr>"
				.$dadosDetalheForma;
	}
	
	if(isset($arrayExtrato['Cronograma Fï¿½sico-Financeiro'])){
		
		$tabelaEtapas = "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>";
		
		if($arrayExtrato['Cronograma Fï¿½sico-Financeiro']['ERRO']){
			$tabelaEtapas .= "<tr>"
							  ."<td align='center' style='color:#ee0000'>Nï¿½o existem etapas cadastradas para a obra.</td>"
							."</tr>";
		}else{
			$tabelaEtapas .= "<tr>"
							  ."<td class='subtitulocentro'>Descriï¿½ï¿½o</td>"
							  ."<td class='subtitulocentro'>Valor do Item</td>"
							  ."<td class='subtitulocentro'>% Referente a Obra</td>"
							."</tr>";
			
			// Escrevendo a tabela com os valores
			foreach($arrayExtrato['Cronograma Fï¿½sico-Financeiro'] as $chave => $array){
				if(!ereg('[^0-9]',$chave)){
					$cor = $i % 2 ? "#e0e0e0" : '#ffffff';
					
					$tabelaEtapas .= "    <tr bgcolor='{$cor}'>"
								  . "        <td>{$array['itcdesc']['valor']}</td>"
								  . "        <td align='right'>{$array['icovlritem']['valor']}</td>"
								  . "        <td align='right'>{$array['icopercsobreobra']['valor']}</td>"
								  . "    </tr>";
					$i++;
				}
			}// fim do foreach
				
		}// fim do if
							
		//escrevendo o total
		$tabelaEtapas .= "<tr bgcolor='#C0C0C0'>"
							."<td align='right'><b>Total</b></td>"
						  	."<td align=right><b>{$arrayExtrato['Cronograma Fï¿½sico-Financeiro']['icovlritem']['valor']}</b></td>"
						  	."<td align=right><b>{$arrayExtrato['Cronograma Fï¿½sico-Financeiro']['icopercsobreobra']['valor']}</b></td>"
						  ."</tr>"
			  			."</table>";
			  			 
		$html .= "<tr>"
					."<td class='subtitulocentro' colspan='2' height='25px;'>Cronograma Fï¿½sico-Financeiro</td>"
					."</tr>"
					."<tr>"
					."	<td colspan='2'>"
					.     $tabelaEtapas
					."  </td>"
					."</tr>";
		
	}
	
	if(isset($arrayExtrato['Licitaï¿½ï¿½o'])){
		$i = 0;
		foreach($arrayExtrato['Licitaï¿½ï¿½o'] as $key => $value){
			$cor = $i % 2 ? "#f5f5f5" : '#ffffff';
			if($key == "ERRO" && $key != 0){
				$tabelaFlCont = "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>"
									."<tr>"
										."<td align='center' style='color:#ee0000'>Nï¿½o existem fases de licitaï¿½ï¿½o cadastradas para a obra.</td>"
							  		."</tr>"
				 				."</table>";
			}else{
				if(is_numeric($key)){
					foreach($arrayExtrato['Licitaï¿½ï¿½o'][$key] as $chave => $valor){
						$cor = $i % 2 ? "#f5f5f5" : '#ffffff';
						$tabelaLicitacoes .="<tr>"
									."<td class='subtitulodireita' style='font-weight: bold;'>". $arrayExtrato['Licitaï¿½ï¿½o'][$key][$chave]['label'] ."</td>"
									."<td bgcolor='{$cor}'>" . $arrayExtrato['Licitaï¿½ï¿½o'][$key][$chave]['valor'] . "</td>"
								   ."</tr>";
					}
					
				}else{
					$tabelaLicitacoes .="<tr>"
									."<td class='subtitulodireita' style='font-weight: bold;'>". $arrayExtrato['Licitaï¿½ï¿½o'][$key]['label'] ."</td>"
									."<td bgcolor='{$cor}'>" . $arrayExtrato['Licitaï¿½ï¿½o'][$key]['valor'] . "</td>"
								   ."</tr>";
					
				}
				
			}// fim do primeiro if
			$i++;
		}// fim do foreach
		$html .= "<tr>"
					."<td class='subtitulocentro' colspan='2' height='25px;'>Licitaï¿½ï¿½o</td>"
				 ."</tr>"
				 .$tabelaLicitacoes
				 ."<tr>"
				 ."	<td style='font-weight: bold;'>Fases de Licitaï¿½ï¿½o</td>"
				 ."</tr>"
				 ."<tr>"
				   ."<td colspan='2'>"
				   	  .$tabelaFlCont
				   ."</td>"
				 ."</tr>";
	}
	
	if(isset($arrayExtrato['Local da Obra'])){
		$html .= "<tr>"
							   . "    <td class='subtitulocentro' colspan='2' height='25px;'>Local da Obra</td>"
							   . "</tr>"
							   . "<tr>"
							   . "    <td class='subtitulodireita' style='font-weight: bold;'>CEP</td>"
							   . "    <td bgcolor='#f5f5f5'>".$arrayExtrato['Local da Obra']['endcep']['valor']."</td>"
							   . "</tr>"
							   . "<tr>"
							   . "    <td class='subtitulodireita' style='font-weight: bold;'>Logradouro</td>"
							   . "    <td>".$arrayExtrato['Local da Obra']['endlog']['valor']."</td>"
							   . "</tr>"
							   . "<tr>"
							   . "    <td class='subtitulodireita' style='font-weight: bold;'>Nï¿½mero</td>"
							   . "    <td bgcolor='#f5f5f5'>".$arrayExtrato['Local da Obra']['endnum']['valor']."</td>"
							   . "</tr>"
							   . "<tr>"
							   . "    <td class='subtitulodireita' style='font-weight: bold;'>Complemento</td>"
							   . "    <td>".$arrayExtrato['Local da Obra']['endcom']['valor']."</td>"
							   . "</tr>"
							   . "<tr>"
							   . "    <td class='subtitulodireita' style='font-weight: bold;'>Bairro</td>"
							   . "    <td bgcolor='#f5f5f5'>".$arrayExtrato['Local da Obra']['endbai']['valor']."</td>"
							   . "</tr>"
							   . "<tr>"
							   . "    <td class='subtitulodireita' style='font-weight: bold;'>Municï¿½pio/UF</td>"
							   . "    <td>".$arrayExtrato['Local da Obra']['mundescricao']['valor']."</td>"
							   . "</tr>";
	}
	
	if(isset($arrayExtrato['Projetos'])){
		$i = 0;
		foreach($arrayExtrato['Projetos'] as $key => $value){
			$cor = $i % 2 ? "#f5f5f5" : '#ffffff';
			
			if(($key == "obProjeto") && ($arrayExtrato['Projetos'][$key]['valor'] != 'NULL')){
				$tabelaProjetos    .= "<tr>"
								   	  	."<td class='subtitulodireita' style='font-weight: bold;'>Observaï¿½ï¿½es</td>"
								   		."<td bgcolor='#f5f5f5'>"
										."<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>"
							   		  ."<tr>"
							   				."<td>Recurso Prï¿½prio (R$)</td>"
							   				."<td>" . $arrayExtrato['Projetos'][$key]['valor']['fprvlrformaelabrecproprio']['valor'] . "</td>"
							   		  ."</tr>"
							   		  ."<tr>"
							   				."<td>Recurso Repassado (R$)</td>"
							   				."<td>" . $arrayExtrato['Projetos'][$key]['valor']['fprvlrformaelabrrecrepassado']['valor'] . "</td>"
							   		  ."</tr>"
							   		 ."</table>"
							   		 ."</td>"
								   	."</tr>";
			}else{
				$tabelaProjetos .= "<tr>"
									."<td class='subtitulodireita' style='font-weight: bold;'>". $arrayExtrato['Projetos'][$key]['label'] ."</td>"
									."<td bgcolor='{$cor}'>" . $arrayExtrato['Projetos'][$key]['valor'] . "</td>"
								."</tr>";
			}
			
			$i++;
			}
			
			$html .= "<tr>"
						."<td class='subtitulocentro' colspan='2' height='25px;'>Projetos </td>"
					."</tr>"
					.$tabelaProjetos;
	}
	
	if(isset($arrayExtrato['Coordenadas Geogrï¿½ficas'])){
		
		if( $arrayExtrato['Coordenadas Geogrï¿½ficas']['latitude']['valor'] && $arrayExtrato['Coordenadas Geogrï¿½ficas']['longitude']['valor'] ){
			
			$tabelaCoordenadas .= "<tr>"
								."<td class='subtitulodireita' style='font-weight: bold;'>Latitude</td>"
					   			."<td bgcolor='#f5f5f5'>".str_replace("#","'",$arrayExtrato['Coordenadas Geogrï¿½ficas']['medlatitude']['valor'])."</td>"
					   		 ."</tr>"
					   		 ."<tr>"
					   		 	."<td class='subtitulodireita' style='font-weight: bold;'>Longitude</td>"
					   			."<td>".str_replace("#","'",$arrayExtrato['Coordenadas Geogrï¿½ficas']['medlongitude']['valor'])."</td>"
					   		 ."</tr>";

		}else{
			$tabelaCoordenadas .= "<tr><td colspan='2' align='center'>Dados de localizaï¿½ï¿½o nï¿½o cadastrados.</td></tr>";
		}

		$html .= "<tr>"
					."<td class='subtitulocentro' colspan='2' height='25px;'>Coordenadas Geogrï¿½ficas</td>"
					."</tr>"
					.$tabelaCoordenadas;
		
	}
	
	if(isset($arrayExtrato['Galeria de Fotos'])){
		$tabelaFotos = "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>";
			if($arrayExtrato['Galeria de Fotos']['ERRO']){
				$tabelaFotos .= "<tr>"
									."<td align='center' colspan='2' style='color:#ee0000'>Nï¿½o existem fotos cadastradas para a obra.</td>"
					 			."</tr>";
			}else{

				$x = count($arrayExtrato['Galeria de Fotos']);
				for( $i = 0; $i < $x; $i++ ){
				
					if( $arrayExtrato['Galeria de Fotos'][$i]['arqid']['valor'] ){
						
						$tabelaFotos .= "<tr>"
									 . "    <td align='center'>"
									 . "        <img src='../slideshow/slideshow/verimagem.php?newwidth=225&newheight=225&arqid={$arrayExtrato['Galeria de Fotos'][$i]['arqid']['valor']}' 
												hspace='3' vspace='3' style='width:225px; height:225px;' /> <br>{$arrayExtrato['Galeria de Fotos'][$i]['arqdescricao']['valor']}"
									 . "    </td>";
									 
					}
					
					$i = $i+1;
					
					if( $arrayExtrato['Galeria de Fotos'][$i]['arqid']['valor'] ){
						
						$tabelaFotos .= "    <td align='center'>"
									 .  "       <img src='../slideshow/slideshow/verimagem.php?newwidth=225&newheight=225&arqid={$arrayExtrato['Galeria de Fotos'][$i]['arqid']['valor']}' 
												hspace='3' vspace='3' style='width:225px; height:225px;' /> <br>{$arrayExtrato['Galeria de Fotos'][$i]['arqdescricao']['valor']}"
									 .  "    </td>";
									 
					}
					
					$i = $i+1;
					
					if( $arrayExtrato['Galeria de Fotos'][$i]['arqid']['valor'] ){
						
						$tabelaFotos .= "    <td align='center'>"
									 .  "        <img src='../slideshow/slideshow/verimagem.php?newwidth=225&newheight=225&arqid={$arrayExtrato['Galeria de Fotos'][$i]['arqid']['valor']}' 
												hspace='3' vspace='3' style='width:225px; height:225px;' /> <br>{$arrayExtrato['Galeria de Fotos'][$i]['arqdescricao']['valor']}"
									 .  "    </td>"
									 . "</tr>";
									 
					}
				
				}// fim do for
			}// fim do primeiro if
		
		$tabelaFotos .= "</table>";
		$html .= "<tr>"
					."<td class='subtitulocentro' colspan='2' height='25px;'>Galeria de Fotos</td>"
				."</tr>"
				."<tr>"
					."<td colspan='2'>{$tabelaFotos}</td>"
				."</tr>";
	}
	
	if(isset($arrayExtrato['Orï¿½amento para a Obra'])){
		
		$tabelaOrcamento = "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>";
			
		if ( !$arrayExtrato['Orï¿½amento para a Obra']['ERRO'] ){
		
			$tabelaOrcamento .= "<tr>"
							. "         <td class='subtitulocentro'>Capital (R$)</td>"
							. "         <td class='subtitulocentro'>Custeio (R$)</td>"
							. "         <td class='subtitulocentro'>Total (R$)</td>"
							. "     </tr>";
			
			$x = count($arrayExtrato['Orï¿½amento para a Obra']);
				$cor = '#ffffff'; 
				
				$tabelaOrcamento .= "<tr bgcolor='{$cor}'>"
								. "         <td align='center'>{$arrayExtrato['Orï¿½amento para a Obra']['eocvlrcapital']['valor']}</td>"
								. "         <td align='center'>{$arrayExtrato['Orï¿½amento para a Obra']['eocvlrcusteio']['valor']}</td>"
								. "         <td align='center'>{$arrayExtrato['Orï¿½amento para a Obra']['eocvlrtotal']['valor']}</td>"
								. "     </tr>";	
				
		}else{
			
			$tabelaOrcamento .= "<tr>"
							. "         <td align='center' style='color:#ee0000'>Nï¿½o existem Orï¿½amentos cadastrados para a obra.</td>"
							. "     </tr>";
			
		}
		
		$tabelaOrcamento .= "</table>";
		
		$html .=   "<tr>"
				.  "    <td class='subtitulocentro' colspan='2' height='25px;'>Orï¿½amento para a Obra</td>"
				.  "</tr>"
				.  "<tr>"
				.  "    <td colspan='2'>"
				.           $tabelaOrcamento         
				.  "    </td>"
				.  "</tr>";
		
	}
	
	if(isset($arrayExtrato['Detalhamento Orï¿½amentï¿½rio'])){ 
		
		$tabelaDetalhamento = "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>";
			
		if ( !$arrayExtrato['Detalhamento Orï¿½amentï¿½rio']['ERRO'] ){
		
			$tabelaDetalhamento .= "<tr>"
							. "         <td class='subtitulocentro'>Data</td>"
							. "         <td class='subtitulocentro'>Valor Empenhado (R$)</td>"
							. "         <td class='subtitulocentro'>Valor Liquidado (R$)</td>"
							. "         <td class='subtitulocentro'>% Empenhado</td>"
							. "         <td class='subtitulocentro'>% Liquidado</td>"
							. "     </tr>";
			
			$x = count($arrayExtrato['Detalhamento Orï¿½amentï¿½rio']) - 1;
			for( $i = 0; $i < $x; $i++ ){
				
				$cor = $i % 2 ? "#e0e0e0" : '#ffffff'; 
				
				$tabelaDetalhamento .= "<tr bgcolor='{$cor}'>"
								. "         <td align='right'>{$arrayExtrato['Detalhamento Orï¿½amentï¿½rio'][$i]['eocdtposicao']['valor']}</td>"
								. "         <td align='right'>{$arrayExtrato['Detalhamento Orï¿½amentï¿½rio'][$i]['eocvlrempenhado']['valor']}</td>"
								. "         <td align='right'>{$arrayExtrato['Detalhamento Orï¿½amentï¿½rio'][$i]['eocvlrliquidado']['valor']}</td>"
								. "         <td align='right'>{$arrayExtrato['Detalhamento Orï¿½amentï¿½rio'][$i]['perEmpenhado']['valor']}</td>"
								. "         <td align='right'>{$arrayExtrato['Detalhamento Orï¿½amentï¿½rio'][$i]['perLiquidado']['valor']}</td>"
								. "     </tr>";	
				
			}
			
			//total
			$tabelaDetalhamento .= "<tr bgcolor='#c0c0c0'>"
								. "         <td align='right'>{$arrayExtrato['Detalhamento Orï¿½amentï¿½rio'][$i]['Total']['valor']}</td>"
								. "         <td align='right'>{$arrayExtrato['Detalhamento Orï¿½amentï¿½rio'][$i]['totEmpenhado']['valor']}</td>"
								. "         <td align='right'>{$arrayExtrato['Detalhamento Orï¿½amentï¿½rio'][$i]['totLiquidado']['valor']}</td>"
								. "         <td align='right'>{$arrayExtrato['Detalhamento Orï¿½amentï¿½rio'][$i]['totPerEmpenhado']['valor']}</td>"
								. "         <td align='right'>{$arrayExtrato['Detalhamento Orï¿½amentï¿½rio'][$i]['totPerLiquidado']['valor']}</td>"
								. "     </tr>";	
							
		}else{
			
			$tabelaDetalhamento .= "<tr>"
							. "         <td align='center' style='color:#ee0000'>Nï¿½o existem Detalhamentos Orï¿½amentï¿½rios cadastrados para a obra.</td>"
							. "     </tr>";
			
		}
		
		$tabelaDetalhamento .= "</table>";
		
		$html .=   "<tr>"
				.  "    <td class='subtitulocentro' colspan='2' height='25px;'>Detalhamento Orï¿½amentï¿½rio</td>"
				.  "</tr>"
				.  "<tr>"
				.  "    <td colspan='2'>"
				.           $tabelaDetalhamento         
				.  "    </td>"
				.  "</tr>";
		
	}
	
	if(isset($arrayExtrato['Restriï¿½ï¿½es e Providï¿½ncias'])){
		
		$tabelaRestricoes = "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>";
			
		if ( !$arrayExtrato['Restriï¿½ï¿½es e Providï¿½ncias']['ERRO'] ){
		
			$tabelaRestricoes .= "<tr>"
							. "         <td class='subtitulocentro'>Fase da Restriï¿½ï¿½o</td>"
							. "         <td class='subtitulocentro'>Tipo de Restriï¿½ï¿½o</td>"
							. "         <td class='subtitulocentro'>Restriï¿½ï¿½o</td>"
							. "         <td class='subtitulocentro'>Providï¿½ncia</td>"
							. "         <td class='subtitulocentro'>Previsï¿½o da Providï¿½ncia</td>"
							. "         <td class='subtitulocentro'>Superaï¿½ï¿½o</td>"
							. "     </tr>";
			
			$x = count($arrayExtrato['Restriï¿½ï¿½es e Providï¿½ncias']);
			for( $i = 0; $i < $x; $i++ ){
				
				$cor = $i % 2 ? "#e0e0e0" : '#ffffff'; 
				
				$tabelaRestricoes .= "<tr bgcolor='{$cor}'>"
								. "         <td align='center'>{$arrayExtrato['Restriï¿½ï¿½es e Providï¿½ncias'][$i]['eocdtposicao']['valor']}</td>"
								. "         <td align='center'>{$arrayExtrato['Restriï¿½ï¿½es e Providï¿½ncias'][$i]['eocvlrempenhado']['valor']}</td>"
								. "         <td align='center'>{$arrayExtrato['Restriï¿½ï¿½es e Providï¿½ncias'][$i]['eocvlrliquidado']['valor']}</td>"
								. "         <td align='center'>{$arrayExtrato['Restriï¿½ï¿½es e Providï¿½ncias'][$i]['perEmpenhado']['valor']}</td>"
								. "         <td align='center'>{$arrayExtrato['Restriï¿½ï¿½es e Providï¿½ncias'][$i]['perLiquidado']['valor']}</td>" //errado
								. "         <td align='center'>{$arrayExtrato['Restriï¿½ï¿½es e Providï¿½ncias'][$i]['perLiquidado']['valor']}</td>"
								. "     </tr>";	
				
			}
							
		}else{
			
			$tabelaRestricoes .= "<tr>"
							. "         <td align='center' style='color:#ee0000'>Nï¿½o existem Restriï¿½ï¿½es e Providï¿½ncias cadastrados para a obra.</td>"
							. "     </tr>";
			
		}
		
		$tabelaRestricoes .= "</table>";
		
		$html .=   "<tr>"
				.  "    <td class='subtitulocentro' colspan='2' height='25px;'>Restriï¿½ï¿½es e Providï¿½ncias</td>"
				.  "</tr>"
				.  "<tr>"
				.  "    <td colspan='2'>"
				.           $tabelaRestricoes         
				.  "    </td>"
				.  "</tr>";
		
	}	
	
	//fim
	$html .= '</table>
			    <table class="tabela" cellSpacing="1" cellPadding="3"	align="center">	
					<tr bgcolor="#D0D0D0">
						<td>
							<input type="button" value="Imprimir" onclick="self.print();" style="cursor: pointer;"/>
							<input type="button" value="Fechar" onclick="self.close();" style="cursor: pointer;"/>
						</td>
					</tr>
				</table>
				</body>
			</html>';
	
	return $html;
}

/**
 * Funï¿½ï¿½o que compara dos extratos de Obras
 * @author Rodrigo Pereira de Souza Silva
 */
function compararExtratoObras($halid1, $halid2){
	
	global $db;
	
	$sql = "SELECT
				haltxt 
			from 
				seguranca.historicoalteracao 
			WHERE halid = " . $halid1;
		
	$array1 = unserialize($db->pegaUm($sql));
	
	$sql = "SELECT
				haltxt 
			from 
				seguranca.historicoalteracao 
			WHERE halid = " . $halid2;
	
	$array2 = unserialize($db->pegaUm($sql)); //ver($array1,$array2);
	
	foreach($array1 as $grupo => $valor){
		//verificando os grupos que possuem ï¿½ndices numï¿½ricos
		if( (($grupo == "Cronograma Fï¿½sico-Financeiro") || ($grupo == "Contatos") || ($grupo == "Licitaï¿½ï¿½o") || ($grupo == "Galeria de Fotos")) ) {
			$x = count($array2[$grupo]);
			for($i = 0; $i < $x; $i++){
				
				if(is_array($array2[$grupo][$i])){
					// se for um array
					foreach($array2[$grupo][$i] as $chave => $valores){
						if($array1[$grupo][$i][$chave]['valor'] != $valores['valor']){
							
							//correï¿½ï¿½o para que a galeria de fotos seja colorida corretamente
							if($grupo == "Galeria de Fotos"){
								if($array1[$grupo][$i]['arqid']['valor'] != $array2[$grupo][$i]['arqid']['valor']){
									// se o arqid for diferente, entï¿½o eu destaco o arqdescricao.
									$array2[$grupo][$i]['arqdescricao']['valor'] = pintaPalavra($array1[$grupo][$i]['arqdescricao']['valor'], $array2[$grupo][$i]['arqdescricao']['valor']);
								}else{
									//senï¿½o eu verifico se o arqdescricao estï¿½ diferente e destaco tbm
									$array2[$grupo][$i]['arqdescricao']['valor'] = pintaPalavra($array1[$grupo][$i]['arqdescricao']['valor'], $array2[$grupo][$i]['arqdescricao']['valor']);
								}
							}else{
								$array2[$grupo][$i][$chave]['valor'] = pintaPalavra($array1[$grupo][$i][$chave]['valor'], $valores['valor']);
							}// fim do if da galeria de fotos
						}
					}
				}else{
					// senï¿½o for array
					foreach($array2[$grupo] as $key => $values){
						if($array1[$grupo][$key]['valor'] != $values['valor']){
							$array2[$grupo][$key]['valor'] = pintaPalavra($array1[$grupo][$key]['valor'], $values['valor']);
						}				
					}
				}
				
			}// fim do for
		}else{
			foreach($array2[$grupo] as $key => $values){ 
				if($array1[$grupo][$key]['valor'] != $values['valor']){
					$array2[$grupo][$key]['valor'] = pintaPalavra($array1[$grupo][$key]['valor'], $values['valor']);
				}

			}
		}// fim do if

	}// fim do foreach
	
	$html = "<center>
				<table border='1'>
					<tr>
						<td width='50%' valign='top'>".gerarExtratoObrasHTML($halid1)."</td>
						<td width='50%' valign='top'>".gerarExtratoObrasHTML($halid2, $array2)."</td>
					</tr>
			 	</table>
			 </center>";
	
	echo $html;

}

/**
 * Funï¿½ï¿½o que marca as diferenï¿½as entre as strings retornando sempre a string2 com as tags
 * <span style='background-color: #B0E2FF'></span>
 * @param string $string1
 * @param string $string2
 */
function pintaPalavra($string1, $string2) {
	
	$x = strlen($string2); //ver($string1,$string2); exit();
	$result = '';
	
	if($string1 == $string2){
		return $string2;
	}else{
		
		$result = "<span style='background-color: #B0E2FF'>" . $string2 . "</span>";
		
//		for($i = 0; $i < $x; $i++){
//			if($string2[$i] != $string1[$i]){
//				$result .= "<span style='background-color: #B0E2FF'>" . $string2[$i] . "</span>";
//			}else{
//				$result .= $string2[$i];
//			}// fim do if
//		}// fim do for
	    return $result;
	}
	
}

/**
 * Funï¿½ï¿½o que monta o formulï¿½rio para o checklist
 * @author Rodrigo Pereira de Souza Silva
 */
function formChecklist($orsid, $gpdid){
	
	$html = '<form id="formulario" name="formulario" method="post" onSubmit="" action="obras.php?modulo=principal/supervisao/check_list_visita&acao=A&requisicao=questionario">
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
					<tr>
						<td colspan="2">I. Referï¿½ncia da Visita</td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="260px">Profissional Responsï¿½vel pela vistoria:</td>
						<td>
							<span id="entnomerespvistoria"></span>
							<input type="hidden" name="entidrespvistoria" id="entidrespvistoria" value="">
							<input type="button" value="Inserir" id="respvistoria">
							<img src="../imagens/obrig.gif" title="Indica campo obrigatï¿½rio." border="0">
						</td>
					</tr>
					<tr>
						<td colspan="2">II. Coleta dos dados do Responsï¿½vel Tï¿½cnico pela obra</td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="260px">Responsï¿½vel Tï¿½cnico pela obra:</td>
						<td>
							<span id="entnomeresptecnico"></span>
							<input type="hidden" name="entidresptecnico" id="entidresptecnico" value="">
							<input type="button" value="Inserir" id="resptecnico">
							<img src="../imagens/obrig.gif" title="Indica campo obrigatï¿½rio." border="0">
						</td>
					<tr>
						<td class="SubTituloDireita" width="260px">Nï¿½ da Ordem de Serviï¿½o</td>
						<td>' . $orsid . '</td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="260px">Nï¿½ do Grupo</td>
						<td>' . $gpdid . '</td>
					</tr>
					<tr>
						<td class="SubTituloEsquerda" colspan="2">
							<input type="submit" value="Salvar" id="cadRespTecRespVist" />
							&nbsp;
							<input type="button" value="Voltar" onclick="window.location=\'obras.php?modulo=principal/supervisao/check_list_visita&acao=A\'" />
						</td>
					</tr>
				</table>
			 </form>';
	
	return $html;
	
}

function verificaPreenchQuest( $gpdid ){
	global $db;
	
	
	
	$sql = "SELECT
				f.nome_obra,
				f.situacao,
				f.orgdesc,
				f.entnome,
				( SELECT
					count(gp.grptitulo) as perguntas_respondidas -- 0 significa que todas foram respondidas
					
				FROM
					questionario.grupopergunta gp
				INNER JOIN questionario.pergunta p USING (grpid)
				INNER JOIN questionario.questionario q ON q.queid = gp.queid
				WHERE
					gp.queid = 42 --id do questionï¿½rio
					AND p.perid NOT IN (
			
				(
					-- Perguntas vinculadas a grupos filhos de questionarios e de resposta textual
					SELECT
						p.perid as idpergunta
			
					FROM
						obras.checklistvistoria cv
			
					JOIN questionario.questionarioresposta qr USING (qrpid)
					JOIN questionario.questionario q ON q.queid = qr.queid
					JOIN questionario.grupopergunta gp ON gp.queid = q.queid
					JOIN questionario.pergunta p ON p.grpid = gp.grpid
					JOIN questionario.resposta r ON r.perid = p.perid
									AND r.qrpid = qr.qrpid
									AND r.itpid IS NULL
					WHERE
						chkstatus = 'A'
						AND qr.qrpid = f.qrpid
			
				)UNION ALL(
					-- Perguntas vinculadas a grupos filhos de questionarios e que possuem item como resposta
					SELECT
						p.perid as idpergunta
						
					FROM
						obras.checklistvistoria cv
			
					JOIN questionario.questionarioresposta qr USING (qrpid)
					JOIN questionario.questionario q ON q.queid = qr.queid
					JOIN questionario.grupopergunta gp ON gp.queid = q.queid
					JOIN questionario.pergunta p ON p.grpid = gp.grpid
					JOIN questionario.itempergunta ip ON ip.perid = p.perid
					JOIN questionario.resposta r ON r.perid = p.perid
									AND r.qrpid = qr.qrpid
									AND r.itpid = ip.itpid
					WHERE
						chkstatus = 'A'
						AND qr.qrpid = f.qrpid
			
				)UNION ALL(
					-- Perguntas de resposta textual filhas de itens de perguntas vinculadas a grupos filhos de questionarios
					SELECT
						p.perid as idpergunta
			
					FROM
						obras.checklistvistoria cv
			
					JOIN questionario.questionarioresposta qr USING (qrpid)
					JOIN questionario.questionario q ON q.queid = qr.queid
					JOIN questionario.grupopergunta gp ON gp.queid = q.queid
					JOIN questionario.pergunta p ON p.grpid = gp.grpid
					JOIN questionario.itempergunta ip ON ip.perid = p.perid
					JOIN questionario.pergunta p1 ON p1.itpid = ip.itpid
					JOIN questionario.resposta r ON r.perid = p1.perid
									AND r.qrpid = qr.qrpid
									AND r.itpid IS NULL
					WHERE
						chkstatus = 'A'
						AND qr.qrpid = f.qrpid
			
				)UNION ALL(
				-- Perguntas de resposta textual filhas de grupos filhos de itens de perguntas vinculadas a grupos filhos de questionarios
					SELECT
			
						p.perid as idpergunta
			
						
					FROM
						obras.checklistvistoria cv
			
					JOIN questionario.questionarioresposta qr USING (qrpid)
					JOIN questionario.questionario q ON q.queid = qr.queid
					JOIN questionario.grupopergunta gp ON gp.queid = q.queid
					JOIN questionario.pergunta p ON p.grpid = gp.grpid
					JOIN questionario.itempergunta ip ON ip.perid = p.perid
					JOIN questionario.grupopergunta gp1 ON gp1.itpid = ip.itpid
					JOIN questionario.pergunta p1 ON p1.grpid = gp1.grpid
					JOIN questionario.resposta r ON r.perid = p1.perid
									AND r.qrpid = qr.qrpid
									AND r.itpid IS NULL
					WHERE
						chkstatus = 'A'
						AND qr.qrpid = f.qrpid
			
				)UNION ALL(
					-- Perguntas de resposta textual filhas de grupos vinculadas a grupos filhos de questionarios
					SELECT
			
						p.perid as idpergunta
			
						
					FROM
						obras.checklistvistoria cv
			
					JOIN questionario.questionarioresposta qr USING (qrpid)
					JOIN questionario.questionario q ON q.queid = qr.queid
					JOIN questionario.grupopergunta gp ON gp.queid = q.queid
					JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
					JOIN questionario.pergunta p ON p.grpid = gp1.grpid
					JOIN questionario.resposta r ON r.perid = p.perid
									AND r.qrpid = qr.qrpid
									AND r.itpid IS NULL
					WHERE
						chkstatus = 'A'
						AND qr.qrpid = f.qrpid
			
				)UNION ALL(
					-- Perguntas de resposta por itens filhas de grupos vinculadas a grupos filhos de questionarios
					SELECT
						p.perid as idpergunta
			
					FROM
						obras.checklistvistoria cv
			
					JOIN questionario.questionarioresposta qr USING (qrpid)
					JOIN questionario.questionario q ON q.queid = qr.queid
					JOIN questionario.grupopergunta gp ON gp.queid = q.queid
					JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
					JOIN questionario.pergunta p ON p.grpid = gp1.grpid
					JOIN questionario.itempergunta ip ON ip.perid = p.perid
					JOIN questionario.resposta r ON r.perid = p.perid
									AND r.itpid = ip.itpid
									AND r.qrpid = qr.qrpid
									AND r.itpid IS NOT NULL
					WHERE
						chkstatus = 'A'
						AND qr.qrpid = f.qrpid
			
				)UNION ALL(
					-- Perguntas de resposta textual filhas de itens de perguntas filhas de grupos vinculadas a grupos filhos de questionarios
					SELECT
						p.perid as idpergunta
			
					FROM
						obras.checklistvistoria cv
			
					JOIN questionario.questionarioresposta qr USING (qrpid)
					JOIN questionario.questionario q ON q.queid = qr.queid
					JOIN questionario.grupopergunta gp ON gp.queid = q.queid
					JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
					JOIN questionario.pergunta p ON p.grpid = gp1.grpid
					JOIN questionario.itempergunta ip ON ip.perid = p.perid
					JOIN questionario.pergunta p1 ON p1.itpid = ip.itpid
					JOIN questionario.resposta r ON r.perid = p1.perid
									AND r.qrpid = qr.qrpid
									AND r.itpid IS NULL
					WHERE
						chkstatus = 'A'
						AND qr.qrpid = f.qrpid
			
				)UNION ALL(
				-- Perguntas de resposta textual filhas de grupos filhos de itens de perguntas filhas de grupos vinculadas a grupos filhos de questionarios
					SELECT
						p.perid as idpergunta
						
					FROM
						obras.checklistvistoria cv
					JOIN questionario.questionarioresposta qr USING (qrpid)
					JOIN questionario.questionario q ON q.queid = qr.queid
					JOIN questionario.grupopergunta gp ON gp.queid = q.queid
					JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
					JOIN questionario.pergunta p ON p.grpid = gp1.grpid
					JOIN questionario.itempergunta ip ON ip.perid = p.perid
					JOIN questionario.grupopergunta gp2 ON gp2.itpid = ip.itpid
					JOIN questionario.pergunta p1 ON p1.grpid = gp2.grpid
					JOIN questionario.resposta r ON r.perid = p1.perid
									AND r.qrpid = qr.qrpid
									AND r.itpid IS NULL
					WHERE
						chkstatus = 'A'
						AND qr.qrpid = f.qrpid
				)
			
			 
			)) AS questionario
			FROM
			
			((
					SELECT
						DISTINCT
						'('|| oi.obrid ||') '|| oi.obrdesc ||'' as nome_obra,
						CASE WHEN cv.chksituacao = TRUE 
							THEN 'Favorï¿½vel' 
							WHEN cv.chksituacao = FALSE 
								THEN 'Nï¿½o Favorï¿½vel' 
							ELSE 
								'Nï¿½o Informado' END AS situacao,
						og.orgdesc,
						ee.entnome,
						cv.qrpid
						
					FROM
						obras.grupodistribuicao gd
					JOIN obras.itemgrupo ig USING(gpdid)
					JOIN obras.repositorio r ON r.repid = ig.repid
													  AND r.repstatus = 'A'
					LEFT JOIN obras.checklistvistoria cv ON cv.obrid = r.obrid
														    AND cv.chkstatus = 'A'
					JOIN obras.obrainfraestrutura oi ON oi.obrid = r.obrid
					JOIN obras.orgao og ON oi.orgid = og.orgid
					JOIN entidade.entidade ee ON oi.entidunidade = ee.entid										    	
					WHERE
						gd.gpdid = {$gpdid}
						AND cv.chkid IS NULL
			)UNION ALL(
					
					SELECT
						DISTINCT
						'('|| oi.obrid ||') '|| oi.obrdesc ||'' as nome_obra,
						CASE WHEN chksituacao = TRUE 
							THEN 'Favorï¿½vel' 
							WHEN cv.chksituacao = FALSE 
								THEN 'Nï¿½o Favorï¿½vel' 
							ELSE 
								'Nï¿½o Informado' 
						END AS situacao,
						og.orgdesc,
						ee.entnome,
						cv.qrpid
					FROM
						obras.grupodistribuicao gd
					JOIN obras.itemgrupo ig USING(gpdid)
					JOIN obras.repositorio r USING(repid)
					JOIN obras.checklistvistoria cv USING(obrid)
					JOIN questionario.questionarioresposta qr USING(qrpid)
					JOIN questionario.questionario q USING(queid)
					JOIN questionario.grupopergunta gp USING(queid)
					JOIN questionario.pergunta p USING (grpid)
					JOIN obras.obrainfraestrutura oi ON oi.obrid = r.obrid	
					JOIN obras.orgao og ON oi.orgid = og.orgid
					JOIN entidade.entidade ee ON oi.entidunidade = ee.entid
					WHERE
						gd.gpdid = {$gpdid}
						AND cv.chkstatus = 'A'
			)) as f";
	
	$dados = (array) $db->carregar( $sql );
	
	$erro = true;
	
	if( is_array($dados[0]) ){
		foreach( $dados as $dado ){
			if( !$dado[questionario] == 0 ){
				$erro = false;
				return $erro;
			}
		}
	}
	
	return $erro;
	
}

function tabelaObrasChecklistaNaoPreenchido($gpdid){
	global $db;
	
	if ( !$gpdid ) return true;
	
	$sql = "SELECT DISTINCT
				f.obra,
				f.nome_obra,
				f.situacao,
				f.dataultminclusao,
				f.orgdesc,
				f.entnome,
				f.itemgrupo,
				f.idchecklist,
				f.id_parecer,
				( SELECT
					count(gp.grptitulo) as perguntas_respondidas -- 0 significa que todas foram respondidas
					
				FROM
					questionario.grupopergunta gp
				INNER JOIN questionario.pergunta p USING (grpid)
				INNER JOIN questionario.questionario q ON q.queid = gp.queid
				WHERE
					gp.queid = 42 --id do questionï¿½rio
					AND p.perid NOT IN (
			
				(
					-- Perguntas vinculadas a grupos filhos de questionarios e de resposta textual
					SELECT
						p.perid as idpergunta
			
					FROM
						obras.checklistvistoria cv
			
					JOIN questionario.questionarioresposta qr USING (qrpid)
					JOIN questionario.questionario q ON q.queid = qr.queid
					JOIN questionario.grupopergunta gp ON gp.queid = q.queid
					JOIN questionario.pergunta p ON p.grpid = gp.grpid
					JOIN questionario.resposta r ON r.perid = p.perid
									AND r.qrpid = qr.qrpid
									AND r.itpid IS NULL
					WHERE
						chkstatus = 'A'
						AND qr.qrpid = f.qrpid
			
				)UNION ALL(
					-- Perguntas vinculadas a grupos filhos de questionarios e que possuem item como resposta
					SELECT
						p.perid as idpergunta
						
					FROM
						obras.checklistvistoria cv
			
					JOIN questionario.questionarioresposta qr USING (qrpid)
					JOIN questionario.questionario q ON q.queid = qr.queid
					JOIN questionario.grupopergunta gp ON gp.queid = q.queid
					JOIN questionario.pergunta p ON p.grpid = gp.grpid
					JOIN questionario.itempergunta ip ON ip.perid = p.perid
					JOIN questionario.resposta r ON r.perid = p.perid
									AND r.qrpid = qr.qrpid
									AND r.itpid = ip.itpid
					WHERE
						chkstatus = 'A'
						AND qr.qrpid = f.qrpid
			
				)UNION ALL(
					-- Perguntas de resposta textual filhas de itens de perguntas vinculadas a grupos filhos de questionarios
					SELECT
						p.perid as idpergunta
			
					FROM
						obras.checklistvistoria cv
			
					JOIN questionario.questionarioresposta qr USING (qrpid)
					JOIN questionario.questionario q ON q.queid = qr.queid
					JOIN questionario.grupopergunta gp ON gp.queid = q.queid
					JOIN questionario.pergunta p ON p.grpid = gp.grpid
					JOIN questionario.itempergunta ip ON ip.perid = p.perid
					JOIN questionario.pergunta p1 ON p1.itpid = ip.itpid
					JOIN questionario.resposta r ON r.perid = p1.perid
									AND r.qrpid = qr.qrpid
									AND r.itpid IS NULL
					WHERE
						chkstatus = 'A'
						AND qr.qrpid = f.qrpid
			
				)UNION ALL(
				-- Perguntas de resposta textual filhas de grupos filhos de itens de perguntas vinculadas a grupos filhos de questionarios
					SELECT
			
						p.perid as idpergunta
			
						
					FROM
						obras.checklistvistoria cv
			
					JOIN questionario.questionarioresposta qr USING (qrpid)
					JOIN questionario.questionario q ON q.queid = qr.queid
					JOIN questionario.grupopergunta gp ON gp.queid = q.queid
					JOIN questionario.pergunta p ON p.grpid = gp.grpid
					JOIN questionario.itempergunta ip ON ip.perid = p.perid
					JOIN questionario.grupopergunta gp1 ON gp1.itpid = ip.itpid
					JOIN questionario.pergunta p1 ON p1.grpid = gp1.grpid
					JOIN questionario.resposta r ON r.perid = p1.perid
									AND r.qrpid = qr.qrpid
									AND r.itpid IS NULL
					WHERE
						chkstatus = 'A'
						AND qr.qrpid = f.qrpid
			
				)UNION ALL(
					-- Perguntas de resposta textual filhas de grupos vinculadas a grupos filhos de questionarios
					SELECT
			
						p.perid as idpergunta
			
						
					FROM
						obras.checklistvistoria cv
			
					JOIN questionario.questionarioresposta qr USING (qrpid)
					JOIN questionario.questionario q ON q.queid = qr.queid
					JOIN questionario.grupopergunta gp ON gp.queid = q.queid
					JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
					JOIN questionario.pergunta p ON p.grpid = gp1.grpid
					JOIN questionario.resposta r ON r.perid = p.perid
									AND r.qrpid = qr.qrpid
									AND r.itpid IS NULL
					WHERE
						chkstatus = 'A'
						AND qr.qrpid = f.qrpid
			
				)UNION ALL(
					-- Perguntas de resposta por itens filhas de grupos vinculadas a grupos filhos de questionarios
					SELECT
						p.perid as idpergunta
			
					FROM
						obras.checklistvistoria cv
			
					JOIN questionario.questionarioresposta qr USING (qrpid)
					JOIN questionario.questionario q ON q.queid = qr.queid
					JOIN questionario.grupopergunta gp ON gp.queid = q.queid
					JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
					JOIN questionario.pergunta p ON p.grpid = gp1.grpid
					JOIN questionario.itempergunta ip ON ip.perid = p.perid
					JOIN questionario.resposta r ON r.perid = p.perid
									AND r.itpid = ip.itpid
									AND r.qrpid = qr.qrpid
									AND r.itpid IS NOT NULL
					WHERE
						chkstatus = 'A'
						AND qr.qrpid = f.qrpid
			
				)UNION ALL(
					-- Perguntas de resposta textual filhas de itens de perguntas filhas de grupos vinculadas a grupos filhos de questionarios
					SELECT
						p.perid as idpergunta
			
					FROM
						obras.checklistvistoria cv
			
					JOIN questionario.questionarioresposta qr USING (qrpid)
					JOIN questionario.questionario q ON q.queid = qr.queid
					JOIN questionario.grupopergunta gp ON gp.queid = q.queid
					JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
					JOIN questionario.pergunta p ON p.grpid = gp1.grpid
					JOIN questionario.itempergunta ip ON ip.perid = p.perid
					JOIN questionario.pergunta p1 ON p1.itpid = ip.itpid
					JOIN questionario.resposta r ON r.perid = p1.perid
									AND r.qrpid = qr.qrpid
									AND r.itpid IS NULL
					WHERE
						chkstatus = 'A'
						AND qr.qrpid = f.qrpid
			
				)UNION ALL(
				-- Perguntas de resposta textual filhas de grupos filhos de itens de perguntas filhas de grupos vinculadas a grupos filhos de questionarios
					SELECT
						p.perid as idpergunta
						
					FROM
						obras.checklistvistoria cv
					JOIN questionario.questionarioresposta qr USING (qrpid)
					JOIN questionario.questionario q ON q.queid = qr.queid
					JOIN questionario.grupopergunta gp ON gp.queid = q.queid
					JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
					JOIN questionario.pergunta p ON p.grpid = gp1.grpid
					JOIN questionario.itempergunta ip ON ip.perid = p.perid
					JOIN questionario.grupopergunta gp2 ON gp2.itpid = ip.itpid
					JOIN questionario.pergunta p1 ON p1.grpid = gp2.grpid
					JOIN questionario.resposta r ON r.perid = p1.perid
									AND r.qrpid = qr.qrpid
									AND r.itpid IS NULL
					WHERE
						chkstatus = 'A'
						AND qr.qrpid = f.qrpid
				)
			
			 
			)) AS questionario
			FROM
			
			((
					SELECT
						DISTINCT
						oi.obrid AS obra,
						'('|| oi.obrid ||') '|| oi.obrdesc ||'' as nome_obra,
						/*CASE WHEN cv.chksituacao = TRUE*/ 
						CASE WHEN mpcckl.mpcsituacao = TRUE 	
							THEN 'Favorï¿½vel' 
							/*WHEN cv.chksituacao = FALSE*/
							WHEN mpcckl.mpcsituacao = FALSE 
								THEN 'Nï¿½o Favorï¿½vel' 
							ELSE 
								'Nï¿½o Informado' END AS situacao,
						to_char(mpcckl.mpcdtinclusao, 'DD/MM/YYYY' ) AS dataultminclusao,	
						og.orgdesc,
						ee.entnome,
						cv.qrpid,
						ig.itgid AS itemgrupo,
						cv.chkid AS idchecklist,
						(
						SELECT
							COUNT(mpc_id)
						FROM 
							obras.itemgrupo ig2 
						INNER JOIN 
							obras.repositorio r2 ON ig2.repid = r2.repid
						INNER JOIN 
							obras.checklistvistoria cv2 ON cv2.obrid = r2.obrid 
										AND cv2.gpdid = gd.gpdid 
						LEFT JOIN 
							obras.movparecercklist mpcckl2 ON mpcckl2.chkid = cv2.chkid	
						WHERE 
							r2.obrid = r.obrid
							AND ig2.gpdid = {$gpdid} 
						) AS id_parecer
					FROM
						obras.grupodistribuicao gd
					JOIN obras.itemgrupo ig USING(gpdid)
					JOIN obras.repositorio r ON r.repid = ig.repid
											 AND r.repstatus = 'A'
					LEFT JOIN obras.checklistvistoria cv ON cv.obrid = r.obrid 
														 AND cv.gpdid = gd.gpdid 
														 AND cv.chkstatus = 'A'
					LEFT JOIN obras.movparecercklist mpcckl ON mpcckl.chkid = cv.chkid
															AND mpcckl.mpcstatus = 'A'
					JOIN obras.obrainfraestrutura oi ON oi.obrid = r.obrid
													 AND oi.obsstatus = 'A'
					JOIN obras.orgao og ON oi.orgid = og.orgid
					JOIN entidade.entidade ee ON oi.entidunidade = ee.entid										    	
					WHERE
						gd.gpdid = {$gpdid}
						AND cv.chkid IS NULL
			)UNION ALL(
					
					SELECT
						DISTINCT
						oi.obrid AS obra,
						'('|| oi.obrid ||') '|| oi.obrdesc ||'' as nome_obra,
						/*CASE WHEN chksituacao = TRUE*/
						CASE WHEN mpcckl.mpcsituacao = TRUE 
							THEN 'Favorï¿½vel' 
							/*WHEN cv.chksituacao = FALSE*/
							WHEN mpcckl.mpcsituacao = FALSE 
								THEN 'Nï¿½o Favorï¿½vel' 
							ELSE 
								'Nï¿½o Informado' 
						END AS situacao,
						to_char(mpcckl.mpcdtinclusao, 'DD/MM/YYYY' ) AS dataultminclusao,
						og.orgdesc,
						ee.entnome,
						cv.qrpid,
						ig.itgid AS itemgrupo,
						cv.chkid AS idchecklist,
						(
						SELECT
							COUNT(mpc_id)
						FROM 
							obras.itemgrupo ig2 
						INNER JOIN 
							obras.repositorio r2 ON ig2.repid = r2.repid
						INNER JOIN 
							obras.checklistvistoria cv2 ON cv2.obrid = r2.obrid 
														AND cv2.gpdid = gd.gpdid 
						LEFT JOIN 
							obras.movparecercklist mpcckl2 ON mpcckl2.chkid = cv2.chkid	
						WHERE 
							r2.obrid = r.obrid 
							AND ig2.gpdid = {$gpdid}
						) AS id_parecer
					FROM
						obras.grupodistribuicao gd
					JOIN obras.itemgrupo ig USING(gpdid)
					JOIN obras.repositorio r USING(repid)
					JOIN obras.checklistvistoria cv ON cv.obrid = r.obrid 
												    AND cv.gpdid = gd.gpdid 
					LEFT JOIN obras.movparecercklist mpcckl ON mpcckl.chkid = cv.chkid
					JOIN questionario.questionarioresposta qr USING(qrpid)
					JOIN questionario.questionario q USING(queid)
					JOIN questionario.grupopergunta gp USING(queid)
					JOIN questionario.pergunta p USING (grpid)
					JOIN obras.obrainfraestrutura oi ON oi.obrid = r.obrid
													 AND oi.obsstatus = 'A' 	
					JOIN obras.orgao og ON oi.orgid = og.orgid
					JOIN entidade.entidade ee ON oi.entidunidade = ee.entid
				  	LEFT JOIN ( SELECT 
									chkid, 
								    MAX(mpcdtinclusao) as mpcdtinclusao
								FROM 
									obras.movparecercklist mc1 
								WHERE 
									mc1.mpcstatus = 'A' 
								GROUP BY
									chkid
							   ) maiordata ON maiordata.chkid = cv.chkid AND maiordata.mpcdtinclusao = mpcckl.mpcdtinclusao
					WHERE
						gd.gpdid = {$gpdid} 
						AND gd.gpdstatus = 'A'
						AND cv.chkstatus = 'A'
						AND ( mpcckl.mpcdtinclusao =( SELECT 
														MAX(mpcdtinclusao) 
												    FROM 
														obras.movparecercklist mc1
													INNER JOIN
														obras.checklistvistoria cv1 ON cv1.chkid = mc1.chkid 
												    WHERE 
														mc1.chkid = cv.chkid
														AND mc1.mpcstatus = 'A'
												    ) 
						      OR mpcckl.mpcdtinclusao IS NULL 
							)
						AND ( mpcckl.mpc_id =( 
												SELECT
													MAX(mpc_id)
												FROM 
													obras.itemgrupo ig3 
												INNER JOIN 
													obras.repositorio r3 ON ig3.repid = r3.repid
												INNER JOIN 
													obras.checklistvistoria cv3 ON cv3.obrid = r3.obrid 
																				AND cv3.gpdid = gd.gpdid 
												LEFT JOIN 
													obras.movparecercklist mpcckl3 ON mpcckl3.chkid = cv3.chkid	
												WHERE 
													r3.obrid = r.obrid
													AND mpcckl3.mpcstatus = 'A' 
													AND ig3.gpdid = {$gpdid}
											  )
						     )	
			)) AS f ORDER BY f.itemgrupo";
		
	$dados = (array) $db->carregar( $sql );
	 
	$sql = "SELECT
				DISTINCT
				'<!--<center>-->'||to_char(MAX(wh.htddata), 'DD/MM/YYYY')||'<!--</center>-->' as datramitacao
			FROM
				obras.grupodistribuicao gd
			INNER JOIN
				workflow.documento wd ON wd.docid = gd.docid
			INNER JOIN
				workflow.historicodocumento wh ON wh.docid = gd.docid
			INNER JOIN
				workflow.estadodocumento we ON we.esdid = wd.esdid
			WHERE
				gpdstatus = 'A' 
				AND gd.gpdid  = {$gpdid}";
	
	$tramitacao = $db->pegaUm($sql);
	
	if ( is_array($dados[0]) ){
		$htm  = "<style type='text/css'>				
					body{
						font-size: 1em;
						font-family: Arial;
					}
					
					table{
						font-size: 0.8em;
					}
				
					div.scrollTable{
						background: #fff;
						/*border: 1px solid #888;*/
					}
				
					div.scrollTable table.header, div.scrollTable div.scroller table{
						width: 100%;
						border-collapse: collapse;
					}
					
					div.scrollTable table.header th, div.scrollTable div.scroller table td{
						/*border: 1px solid #444;*/
						padding: 3px 5px;
					}
					
					div.scrollTable table.header th{
						background: #ddd;
					}
				
					div.scrollTable div.scroller{
						height: 200px;
						overflow: scroll;
					}
				
					div.scrollTable .coluna75px{
						width: 75px;
					}
				
					div.scrollTable .coluna100px{
						width: 100px;
					}
				
					div.scrollTable .coluna150px{
						width: 150px;
					}
				</style>
				 <div class='scrollTable'><center><h2>Situaï¿½ï¿½o do Checklist/Parecer</h2>Data da Tramitaï¿½ï¿½o: {$tramitacao}</center>
					<table class='tabela header' cellpadding=\"1\" cellspacing=\"1\">
						<tbody>
							<tr bgcolor=\"#e7e7e7\">
								<th class='coluna150px'>Nome da obra</th>
								<th class='coluna150px'>Questionï¿½rio</th>
								<th class='coluna150px'>Parecer MEC( ï¿½ltima )</th>
								<th class='coluna150px'>Data ï¿½ltimo Parecer</th>
								<th class='coluna150px'>Orgï¿½o</th>
								<th class='coluna150px'>Unidade ResponsÃ¡vel pela Obra</th>
								<th width='2%'></th>
							</tr>
						</tbody>
					</table>
					<div class='scroller'>
						<table class='tabela'>
							<tbody>";
		
		foreach($dados as $dado){
			$cor = ( ($i % 2 == 0) ? "bgcolor=\"\" onmouseout=\"this.bgColor='';\" onmouseover=\"this.bgColor='#ffffcc';\"" : "bgcolor=\"#f7f7f7\" onmouseout=\"this.bgColor='#F7F7F7';\" onmouseover=\"this.bgColor='#ffffcc';\"" );
			$htm .= "<tr {$cor}>
						<td class='coluna150px' align=\"left\">{$dado['nome_obra']}</td>
						<td class='coluna150px' align=\"center\">".( ($dado['questionario'] > 0) ? "<font color=\"#FF0000\">Nï¿½o Preenchido</font>" : "Preenchido" )."</td>
						<td class='coluna150px' align=\"center\">{$dado['situacao']}</td>
						<td class='coluna150px' align=\"center\">{$dado['dataultminclusao']}</td>
						<td class='coluna150px'>{$dado['orgdesc']}</td>
						<td class='coluna150px'>{$dado['entnome']}</td>
					 </tr>";
			$i++;
		}
		$htm  .= "</tbody>
				</table>
			</div>
		</div>";
	}
	
	/*Alterado dia 13/04/2011 as 18:38h
	 * Obs.: Alteraï¿½ï¿½o feita por nï¿½o haver, pelo menos por enquanto a necessidade do Checklist completo(HTML), 
	 * por isso serï¿½o enviadas apenas as Informaï¿½ï¿½es do Questionï¿½rio.   
	 *return $htm;			
	 */ 
	return $dados;			
}

function dadosTramitacaoObraIndividual($gpdid){
	
	global $db;
	
	if ( !$gpdid ) return true;
	
	$sql = "SELECT DISTINCT
					it.itgid,
					oi.obrid,
					'<FONT ' ||
					/* Situaï¿½ï¿½o: Em Supervisï¿½o */
					CASE WHEN ed.esdid = ".OBREMSUPERVISAOIND." AND DATE_PART('days', NOW() - (to_char(MAX(whd.htddata), 'YYYY-mm-dd'))::timestamp) <= 20
							THEN 'COLOR=\"#008000\" />'||ed.esddsc
					 WHEN ed.esdid = ".OBREMSUPERVISAOIND." AND DATE_PART('days', NOW() - (to_char(MAX(whd.htddata), 'YYYY-mm-dd'))::timestamp) > 20 AND DATE_PART('days', NOW() - (to_char(MAX(whd.htddata), 'YYYY-mm-dd'))::timestamp) <= 30
							THEN 'COLOR=\"#BB9900\" />'||ed.esddsc
					     WHEN ed.esdid = ".OBREMSUPERVISAOIND." AND DATE_PART('days', NOW() - (to_char(MAX(whd.htddata), 'YYYY-mm-dd'))::timestamp) > 30
							THEN 'COLOR=\"#DD0000\" />'||ed.esddsc
						ELSE /* Situaï¿½ï¿½o: Em Avaliaï¿½ï¿½o de Supervisï¿½o(MEC) */
						     CASE WHEN ed.esdid = ".OBRAAVALIACAOSUPERVISAO_MEC." AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) <= 15
									THEN 'COLOR=\"#008000\" />'||ed.esddsc
							     WHEN ed.esdid = ".OBRAAVALIACAOSUPERVISAO_MEC." AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) > 15 AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) <= 20
									THEN 'COLOR=\"#BB9900\" />'||ed.esddsc
							     WHEN ed.esdid = ".OBRAAVALIACAOSUPERVISAO_MEC." AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) > 20
									THEN 'COLOR=\"#DD0000\" />'||ed.esddsc 
							ELSE /* Situaï¿½ï¿½o: Ajuste de Supervisï¿½o(Empresa) */
							     CASE WHEN ed.esdid = ".OBRAAJUSTESUPERVISAO_EMPRESA." AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) <= 5
										THEN 'COLOR=\"#008000\" />'||ed.esddsc
								     WHEN ed.esdid = ".OBRAAJUSTESUPERVISAO_EMPRESA." AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) > 5 AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) <= 10
										THEN 'COLOR=\"#BB9900\" />'||ed.esddsc
								     WHEN ed.esdid = ".OBRAAJUSTESUPERVISAO_EMPRESA." AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) > 10
										THEN 'COLOR=\"#DD0000\" />'||ed.esddsc 
								ELSE /* Situaï¿½ï¿½o: Reavaliaï¿½ï¿½o da Supervisï¿½o(MEC) */
								     CASE WHEN ed.esdid = ".OBRAREAVALIACAOSUPERVISAO_MEC." AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) <= 7
											THEN 'COLOR=\"#008000\" />'||ed.esddsc
									     WHEN ed.esdid = ".OBRAREAVALIACAOSUPERVISAO_MEC." AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) > 7 AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) <= 10
											THEN 'COLOR=\"#BB9900\" />'||ed.esddsc
									     WHEN ed.esdid = ".OBRAREAVALIACAOSUPERVISAO_MEC." AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) > 10
											THEN 'COLOR=\"#DD0000\" />'||ed.esddsc
									 ELSE /* Situaï¿½ï¿½o: Reajuste da supervisï¿½o (Empresa) */
									     CASE WHEN ed.esdid = ".OBRAREAJUSTESUPERVISAO_EMPRESA." AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) <= 5
												THEN 'COLOR=\"#008000\" />'||ed.esddsc
										     WHEN ed.esdid = ".OBRAREAJUSTESUPERVISAO_EMPRESA." AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) > 5 AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) <= 10
												THEN 'COLOR=\"#BB9900\" />'||ed.esddsc
										     WHEN ed.esdid = ".OBRAREAJUSTESUPERVISAO_EMPRESA." AND DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) > 10
												THEN 'COLOR=\"#DD0000\" />'||ed.esddsc
												ELSE /* As demais Situaï¿½ï¿½es do Grupo */
												     CASE WHEN ed.esddsc IS NOT NULL
														THEN 'COLOR=\"#000000\" />'||ed.esddsc
												END
									 END			 
								END			
							END 
						END
					END ||'</FONT>'AS situacao_tramitacao,
					--ed.esddsc As situacao_tramitacao,
					to_char(MAX(hd.htddata), 'DD/MM/YYYY') as datramitacao,
					DATE_PART('days', NOW() - (to_char(MAX(hd.htddata), 'YYYY-mm-dd'))::timestamp) AS diasapostramitacao,
					(	 SELECT 	
								DATE_PART('days', NOW() - (to_char(MIN(h.htddata), 'YYYY-mm-dd'))::timestamp) AS dias_ultima_tramitaï¿½ï¿½o_ate_atual
						 FROM 
						 		obras.obrainfraestrutura o
						 LEFT JOIN 
						 		workflow.documento d ON d.docid = o.docid
						 LEFT JOIN 
						 		workflow.estadodocumento e ON e.esdid = d.esdid AND e.esdstatus = 'A'
						 LEFT JOIN 
						 		workflow.historicodocumento h ON h.docid = o.docid
						 LEFT JOIN 
						 		workflow.tipodocumento t ON t.tpdid = e.tpdid
						 LEFT JOIN 
						 		obras.repositorio r ON r.obrid = o.obrid AND r.repstatus = 'A'
						 LEFT JOIN 
						 		obras.itemgrupo i ON i.repid = r.repid 
						 LEFT JOIN 
						 		obras.grupodistribuicao g ON g.gpdid = i.gpdid AND g.gpdstatus = 'A'
						 WHERE 
								g.gpdid = ".$gpdid."
								AND o.obsstatus = 'A'
					)AS diasatetramitacao
			FROM
				obras.obrainfraestrutura oi 
			LEFT JOIN
				obras.repositorio re ON oi.obrid = re.obrid AND re.repstatus = 'A'
			LEFT JOIN 
				workflow.documento dc ON dc.docid = oi.docid 
			LEFT JOIN 
				workflow.estadodocumento ed ON ed.esdid = dc.esdid AND ed.esdstatus = 'A'
			LEFT JOIN 
				workflow.historicodocumento hd ON hd.docid = dc.docid  
			LEFT JOIN
				obras.itemgrupo it ON it.repid = re.repid 
			LEFT JOIN
				obras.grupodistribuicao gd ON gd.gpdid = it.gpdid AND gd.gpdstatus = 'A'	
			LEFT JOIN 
				obras.checklistvistoria ch ON ch.obrid = oi.obrid AND ch.chkstatus = 'A'
			LEFT JOIN
				workflow.acaoestadodoc aed ON  aed.aedid = hd.aedid
			LEFT JOIN 
					workflow.documento wdc ON wdc.docid = gd.docid
			LEFT JOIN 
					workflow.estadodocumento wed ON wed.esdid = wdc.esdid AND wed.esdstatus = 'A'
			LEFT JOIN 
					workflow.historicodocumento whd ON whd.docid = gd.docid AND whd.aedid = ". GRUPOLIBERADOPARASUPERVISAO ." /* Junï¿½ï¿½o para recuperar a Data do Grupo quando Liberado para Supervisï¿½o.*/
			WHERE
				it.gpdid = ".$gpdid."
				AND oi.obsstatus = 'A'
			GROUP BY
				ed.esdid,
				ed.esddsc,
				it.itgid,
				oi.obrid
			ORDER BY
				it.itgid";
	
	$dados = (array) $db->carregar( $sql );
	
	return $dados;
}	
/**
 * VERIFICAR SE ESTA FUNï¿½ï¿½O PODE SER OU Nï¿½O REMOVIDA PQ, APARENTEMENTE, Nï¿½O ESTï¿½ SENDO USADA NO SISTEMA
 *
 * @param unknown_type $gpdid
 * @return unknown
 */
function tabelaObrasChecklistNaoAprovado($gpdid){
	global $db;
	
	if ( !$gpdid ) return true;
	
	$sql = "SELECT 
				'('|| oi.obrid ||') '|| oi.obrdesc ||'' as nome_obra,
				chksituacao as parecer_mec,
				og.orgdesc,
				entnome
			FROM
				obras.grupodistribuicao gd
			JOIN obras.itemgrupo ig ON ig.gpdid = gd.gpdid
			JOIN obras.repositorio r ON r.repid = ig.repid 
			JOIN obras.checklistvistoria cv ON cv.obrid = r.obrid
							AND cv.chkstatus = 'A'
			JOIN obras.obrainfraestrutura oi ON oi.obrid = cv.obrid
							AND obsstatus = 'A'
			JOIN obras.orgao og ON oi.orgid = og.orgid
			JOIN entidade.entidade ee ON oi.entidunidade = ee.entid
			WHERE
				gd.gpdid IN ({$gpdid})
				AND (cv.chksituacao = false OR cv.chksituacao IS NULL)
			ORDER BY nome_obra;";
	
	$dados = (array) $db->carregar( $sql );
	
	if ( is_array($dados[0]) ){
		$htm  = "<style type='text/css'>				
					body{
						font-size: 1em;
						font-family: Arial;
					}
					
					table{
						font-size: 0.8em;
					}
				
					div.scrollTable{
						background: #fff;
						/*border: 1px solid #888;*/
					}
				
					div.scrollTable table.header, div.scrollTable div.scroller table{
						width: 100%;
						border-collapse: collapse;
					}
					
					div.scrollTable table.header th, div.scrollTable div.scroller table td{
						/*border: 1px solid #444;*/
						padding: 3px 5px;
					}
					
					div.scrollTable table.header th{
						background: #ddd;
					}
				
					div.scrollTable div.scroller{
						height: 200px;
						overflow: scroll;
					}
				
					div.scrollTable .coluna75px{
						width: 75px;
					}
				
					div.scrollTable .coluna100px{
						width: 100px;
					}
				
					div.scrollTable .coluna150px{
						width: 150px;
					}
				</style>
				 <div class='scrollTable'>
					<table class='tabela header' cellpadding=\"1\" cellspacing=\"1\">
						<tbody>
							<tr bgcolor=\"#e7e7e7\">
								<th class='coluna150px'>Nome da obra</th>
								<th class='coluna150px'>Parecer MEC</th>
								<th class='coluna150px'>Orgï¿½o</th>
								<th class='coluna150px'>Unidade ResponsÃ¡vel pela Obra</th>
								<th width='2%'></th>
							</tr>
						</tbody>
					</table>
					<div class='scroller'>
						<table>
							<tbody>";
		
		foreach($dados as $dado){
		$cor = ( ($i % 2 == 0) ? "bgcolor=\"\" onmouseout=\"this.bgColor='';\" onmouseover=\"this.bgColor='#ffffcc';\"" : "bgcolor=\"#f7f7f7\" onmouseout=\"this.bgColor='#F7F7F7';\" onmouseover=\"this.bgColor='#ffffcc';\"" );
			$htm .= "<tr {$cor}>
						<td class='coluna150px' align=\"left\">{$dado['nome_obra']}</td>
						<td class='coluna150px' align=\"center\">".( ($dado['parecer_mec'] == "f") ? "Nï¿½o" : ( ($dado['parecer_mec']) ? "Sim" : "Nï¿½o Preenchido" ) )."</td>
						<td class='coluna150px' align=\"center\">{$dado['orgdesc']}</td>
						<td class='coluna150px' align=\"center\">{$dado['entnome']}</td>
					 </tr>";
			$i++;
		}
			$htm  .= "</tbody>
				</table>
			</div>
		</div>";
	}
	
	return $htm;			
}

/**
 * Essa funï¿½ï¿½o era pegaQrpid, agora ï¿½ pegaQrpidObras
 *
 * @param unknown_type $obrid
 * @param unknown_type $queid
 * @param unknown_type $orsid
 * @param unknown_type $dados
 * @return unknown
 * @author Rodrigo Pereira de Souza Silva
 */
function pegaQrpidObras( $obrid, $queid, $orsid, $dados, $gpdid ){
    global $db;
   
    $sql = "SELECT
                    ck.qrpid
            FROM
                    obras.checklistvistoria ck
            INNER JOIN
                    questionario.questionarioresposta q ON q.qrpid = ck.qrpid
            WHERE
                    ck.obrid = {$obrid} 
                    AND q.queid = {$queid} 
                    AND ck.entidresptecnico = {$_SESSION['obra']['entidresptecnico']} 
                    AND ck.entidrespvistoria = {$_SESSION['obra']['entidrespvistoria']}
                    AND ck.chkstatus = 'A'";
    $qrpid = $db->pegaUm( $sql );
    
    //if(!$qrpid){
        $arParam = array ( "queid" => $queid, "titulo" => "OBRAS (".$obrid." - ".$dados['nome'].")" );
        $qrpid = GerenciaQuestionario::insereQuestionario( $arParam );
        $chkid = salvaResponsaveisObras($_SESSION['obra']['entidrespvistoria'], $_SESSION['obra']['entidresptecnico'], $qrpid, $gpdid);

    //}
    return $qrpid.','.$chkid;
}

/**
 * Funï¿½ï¿½o que pega o nï¿½mero da Ordem de Serviï¿½o da Obra
 * @return integer
 * @author Rodrigo Pereira de Souza Silva
 *  
 */
function pegaOrdem(){
    global $db;
   
    $sql = "SELECT
    			rp.obrid, 
    			ig.repid, 
    			gd.gpdid, 
    			os.orsid as orsid, 
    			gd.docid
			FROM
				obras.repositorio as rp
			INNER JOIN obras.itemgrupo as ig on ig.repid = rp.repid
			INNER JOIN obras.grupodistribuicao as gd on gd.gpdid = ig.gpdid
			INNER JOIN obras.ordemservico as os on os.gpdid = gd.gpdid
			INNER JOIN workflow.documento as wd on wd.docid = gd.docid
			INNER JOIN workflow.estadodocumento as ed on ed.esdid = wd.esdid
			WHERE
				rp.obrid = {$_SESSION['obra']['obrid']} AND
				--ed.esdid =  159 AND
				rp.repstatus = 'A' AND
				os.orsstatus = 'A'";
    $orsid = $db->carregar( $sql );

    return $orsid;
}

/**
 * Funï¿½ï¿½o que salva o id do Profissional Responsï¿½vel pela vistoria e do Responsï¿½vel Tï¿½cnico pela obra, respectivamente 
 */
function salvaResponsaveisObras($entidrespvistoria, $entidresptecnico, $qrpid, $gpdid){
    global $db;
	
	$dados = pegaOrdem();
	
	$sql = "INSERT INTO 
				obras.checklistvistoria
					(entidresptecnico, 
					orsid, 
					obrid, 
					qrpid,
					usucpf, 
					entidrespvistoria,
					gpdid)
    		VALUES 
    			({$entidresptecnico}, {$dados[0]['orsid']}, {$_SESSION['obra']['obrid']}, {$qrpid}, '{$_SESSION['usucpf']}', {$entidrespvistoria}, {$gpdid})
			RETURNING chkid";
	
	$chkid = $db->carregar( $sql );
	
	$db->commit();
	return $chkid[0]['chkid'];
	
}

/**
 * Funï¿½ï¿½o que verifica se a obra jï¿½ possui um checklist
 * @return chkid caso exista checklist e FALSE caso nï¿½o exista
 * @author Rodrigo Pereira de Souza Silva
 */
function verificaQrpid($chkid){
	global $db;
	
	$sql = "SELECT 
				qrpid
			FROM 
				obras.checklistvistoria
			WHERE
				chkid = {$chkid}";
	
	return $db->pegaUm($sql);
	
}

/**
 * Funï¿½ï¿½o que atualiza o status do checklist da obra para I - Inativo
 * @param integer $chkid
 */
function excluirChecklistObras($chkid){
	global $db;
	
	//Verificando se o usuï¿½rio tem permissï¿½o para excluir o checklist
	$sql = "SELECT 
				usucpf
			FROM 
				obras.checklistvistoria
			WHERE 
				chkid = {$chkid}";
	
	$cpf = $db->pegaUm($sql);
	
	//se for superusuï¿½rio ou empresa ou cpf que cadastrou o checklist for igual ao do usuï¿½rio entï¿½o pode excluir
	if( possuiPerfil(PERFIL_SUPERUSUARIO) || (possuiPerfil(PERFIL_EMPRESA) && $_SESSION['usucpf'] == $cpf) ){
		// setando status do questionï¿½rio como inativo
		$sql = "UPDATE
					obras.checklistvistoria
				SET
					chkstatus = 'I'
				WHERE
					chkid = {$chkid}";
		
		$db->executar( $sql );
		
		$db->commit();
		return true;
	}else{
		return false;
	}

}
	
/**
 * Funï¿½ï¿½o que verifica se hï¿½ checklist cadastrada para a obra
 * @author Rodrigo Pereira de Souza Silva
 * @return chkid
 */
function verificaChecklist(){
	
	global $db;
	$dados = pegaOrdem();
	
	$sql = "SELECT 
				cv.chkid
				
			FROM
				obras.checklistvistoria cv
			INNER JOIN obras.ordemservico os ON os.orsid = cv.orsid 
			WHERE
				cv.chkstatus = 'A' AND
				cv.orsid = {$dados[0]['orsid']} AND
				cv.obrid = {$_SESSION['obra']['obrid']} AND
				os.gpdid = {$dados[0]['gpdid']} AND
				os.orsstatus = 'A' AND
      			os.orssupervisaostatus <> 'F'";
	$chkid = $db->pegaUm($sql);
	
	return $chkid;
	
}

function valEnviarParaAvaliacaoMec($gpdid, $obrid = null){
	global $db;
	ini_set("memory_limit","250M");
	set_time_limit(0);
	if ( !$gpdid ) return true;
	
	if ( $obrid ) $whereObra = " AND re.obrid = {$obrid} ";
	
	$sql = "SELECT 
				SUM(nrespondida) as nrespondida 
			FROM(
				(
					SELECT
						COUNT(ig.gpdid) as nrespondida
					FROM
						obras.grupodistribuicao gd
					INNER JOIN obras.itemgrupo ig USING(gpdid)
					INNER JOIN obras.repositorio re ON re.repid = ig.repid
													  AND re.repstatus = 'A'
													  {$whereObra}	
					LEFT JOIN obras.checklistvistoria cv ON cv.obrid = re.obrid
										    			    AND cv.chkstatus = 'A'	
					WHERE
						gd.gpdid = {$gpdid}
						AND cv.chkid IS NULL
				)UNION ALL(
					
					SELECT
						COUNT(p.perid) as nrespondida
					FROM
						obras.grupodistribuicao gd
					JOIN obras.itemgrupo ig USING(gpdid)
					JOIN obras.repositorio re USING(repid)
					JOIN obras.checklistvistoria cv USING(obrid)
					JOIN questionario.questionarioresposta qr USING(qrpid)
					JOIN questionario.questionario q USING(queid)
					JOIN questionario.grupopergunta gp USING(queid)
					JOIN questionario.pergunta p USING (grpid)	
					WHERE
						gd.gpdid = {$gpdid}
						{$whereObra}
						AND cv.chkstatus = 'A'
						AND p.perid::text || cv.obrid::text NOT IN (
								SELECT 
									f.idpergunta::text || cv.obrid::text
								FROM
									obras.grupodistribuicao gd
									JOIN obras.itemgrupo ig USING(gpdid)
									JOIN obras.repositorio re USING(repid)
									JOIN (
										(
											-- Perguntas vinculadas a grupos filhos de questionarios e de resposta textual
											SELECT
												p.perid as idpergunta,
												cv.obrid
											FROM
												obras.checklistvistoria cv
											JOIN obras.repositorio re ON re.obrid = cv.obrid
																		  AND re.repstatus = 'A'
																		  {$whereObra}
											JOIN obras.itemgrupo ig ON ig.repid = re.repid
														  AND ig.gpdid = {$gpdid}
											JOIN questionario.questionarioresposta qr USING (qrpid)
											JOIN questionario.questionario q ON q.queid = qr.queid
											JOIN questionario.grupopergunta gp ON gp.queid = q.queid
											JOIN questionario.pergunta p ON p.grpid = gp.grpid
											JOIN questionario.resposta r ON r.perid = p.perid
															AND r.qrpid = qr.qrpid
															AND r.itpid IS NULL
											WHERE
												chkstatus = 'A'
												--AND qr.qrpid = 117
										)UNION ALL(
											-- Perguntas vinculadas a grupos filhos de questionarios e que possuem item como resposta
											SELECT
												p.perid as idpergunta,
												cv.obrid		
											FROM
												obras.checklistvistoria cv
											JOIN obras.repositorio re ON re.obrid = cv.obrid
																		  AND re.repstatus = 'A'
																		  {$whereObra}
											JOIN obras.itemgrupo ig ON ig.repid = re.repid
														  AND ig.gpdid = {$gpdid}
											JOIN questionario.questionarioresposta qr USING (qrpid)
											JOIN questionario.questionario q ON q.queid = qr.queid
											JOIN questionario.grupopergunta gp ON gp.queid = q.queid
											JOIN questionario.pergunta p ON p.grpid = gp.grpid
											JOIN questionario.itempergunta ip ON ip.perid = p.perid
											JOIN questionario.resposta r ON r.perid = p.perid
															AND r.qrpid = qr.qrpid
															AND r.itpid = ip.itpid
											WHERE
												chkstatus = 'A'
												--AND qr.qrpid = 117
										)UNION ALL(
											-- Perguntas de resposta textual filhas de itens de perguntas vinculadas a grupos filhos de questionarios
											SELECT
												p.perid as idpergunta,
												cv.obrid		
											FROM
												obras.checklistvistoria cv
											JOIN obras.repositorio re ON re.obrid = cv.obrid
																		  AND re.repstatus = 'A'
																		  {$whereObra}
											JOIN obras.itemgrupo ig ON ig.repid = re.repid
														  AND ig.gpdid = {$gpdid}
											JOIN questionario.questionarioresposta qr USING (qrpid)
											JOIN questionario.questionario q ON q.queid = qr.queid
											JOIN questionario.grupopergunta gp ON gp.queid = q.queid
											JOIN questionario.pergunta p ON p.grpid = gp.grpid
											JOIN questionario.itempergunta ip ON ip.perid = p.perid
											JOIN questionario.pergunta p1 ON p1.itpid = ip.itpid
											JOIN questionario.resposta r ON r.perid = p1.perid
															AND r.qrpid = qr.qrpid
															AND r.itpid IS NULL
											WHERE
												chkstatus = 'A'
												--AND qr.qrpid = 117
										)UNION ALL(
										-- Perguntas de resposta textual filhas de grupos filhos de itens de perguntas vinculadas a grupos filhos de questionarios
											SELECT
												p.perid as idpergunta,
												cv.obrid		
											FROM
												obras.checklistvistoria cv
											JOIN obras.repositorio re ON re.obrid = cv.obrid
																		  AND re.repstatus = 'A'
																		  {$whereObra}
											JOIN obras.itemgrupo ig ON ig.repid = re.repid
														  AND ig.gpdid = {$gpdid}
											JOIN questionario.questionarioresposta qr USING (qrpid)
											JOIN questionario.questionario q ON q.queid = qr.queid
											JOIN questionario.grupopergunta gp ON gp.queid = q.queid
											JOIN questionario.pergunta p ON p.grpid = gp.grpid
											JOIN questionario.itempergunta ip ON ip.perid = p.perid
											JOIN questionario.grupopergunta gp1 ON gp1.itpid = ip.itpid
											JOIN questionario.pergunta p1 ON p1.grpid = gp1.grpid
											JOIN questionario.resposta r ON r.perid = p1.perid
															AND r.qrpid = qr.qrpid
															AND r.itpid IS NULL
											WHERE
												chkstatus = 'A'
												--AND qr.qrpid = 117
										)UNION ALL(
											-- Perguntas de resposta textual filhas de grupos vinculadas a grupos filhos de questionarios
											SELECT
												p.perid as idpergunta,
												cv.obrid		
											FROM
												obras.checklistvistoria cv
											JOIN obras.repositorio re ON re.obrid = cv.obrid
																		  AND re.repstatus = 'A'
																		  {$whereObra}
											JOIN obras.itemgrupo ig ON ig.repid = re.repid
														  AND ig.gpdid = {$gpdid}
											JOIN questionario.questionarioresposta qr USING (qrpid)
											JOIN questionario.questionario q ON q.queid = qr.queid
											JOIN questionario.grupopergunta gp ON gp.queid = q.queid
											JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
											JOIN questionario.pergunta p ON p.grpid = gp1.grpid
											JOIN questionario.resposta r ON r.perid = p.perid
															AND r.qrpid = qr.qrpid
															AND r.itpid IS NULL
											WHERE
												chkstatus = 'A'
												--AND qr.qrpid = 117
										)UNION ALL(
											-- Perguntas de resposta por itens filhas de grupos vinculadas a grupos filhos de questionarios
											SELECT
												p.perid as idpergunta,
												cv.obrid
											FROM
												obras.checklistvistoria cv
											JOIN obras.repositorio re ON re.obrid = cv.obrid
																		 AND re.repstatus = 'A'
																		 {$whereObra}
											JOIN obras.itemgrupo ig ON ig.repid = re.repid
														  AND ig.gpdid = {$gpdid}
											JOIN questionario.questionarioresposta qr USING (qrpid)
											JOIN questionario.questionario q ON q.queid = qr.queid
											JOIN questionario.grupopergunta gp ON gp.queid = q.queid
											JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
											JOIN questionario.pergunta p ON p.grpid = gp1.grpid
											JOIN questionario.itempergunta ip ON ip.perid = p.perid
											JOIN questionario.resposta r ON r.perid = p.perid
															AND r.itpid = ip.itpid
															AND r.qrpid = qr.qrpid
															AND r.itpid IS NOT NULL
											WHERE
												chkstatus = 'A'
												--AND qr.qrpid = 117
										)UNION ALL(
											-- Perguntas de resposta textual filhas de itens de perguntas filhas de grupos vinculadas a grupos filhos de questionarios
											SELECT
												p.perid as idpergunta,
												cv.obrid
											FROM
												obras.checklistvistoria cv
											JOIN obras.repositorio re ON re.obrid = cv.obrid
																		  AND re.repstatus = 'A'
																		  {$whereObra}
											JOIN obras.itemgrupo ig ON ig.repid = re.repid
														  AND ig.gpdid = {$gpdid}
											JOIN questionario.questionarioresposta qr USING (qrpid)
											JOIN questionario.questionario q ON q.queid = qr.queid
											JOIN questionario.grupopergunta gp ON gp.queid = q.queid
											JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
											JOIN questionario.pergunta p ON p.grpid = gp1.grpid
											JOIN questionario.itempergunta ip ON ip.perid = p.perid
											JOIN questionario.pergunta p1 ON p1.itpid = ip.itpid
											JOIN questionario.resposta r ON r.perid = p1.perid
															AND r.qrpid = qr.qrpid
															AND r.itpid IS NULL
											WHERE
												chkstatus = 'A'
												--AND qr.qrpid = 117
										)UNION ALL(
										-- Perguntas de resposta textual filhas de grupos filhos de itens de perguntas filhas de grupos vinculadas a grupos filhos de questionarios
											SELECT
												p.perid as idpergunta,
												cv.obrid		
											FROM
												obras.checklistvistoria cv
											JOIN obras.repositorio re ON re.obrid = cv.obrid
																		  AND re.repstatus = 'A'
																		  {$whereObra}
											JOIN obras.itemgrupo ig ON ig.repid = re.repid
														  AND ig.gpdid = {$gpdid}
											JOIN questionario.questionarioresposta qr USING (qrpid)
											JOIN questionario.questionario q ON q.queid = qr.queid
											JOIN questionario.grupopergunta gp ON gp.queid = q.queid
											JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
											JOIN questionario.pergunta p ON p.grpid = gp1.grpid
											JOIN questionario.itempergunta ip ON ip.perid = p.perid
											JOIN questionario.grupopergunta gp2 ON gp2.itpid = ip.itpid
											JOIN questionario.pergunta p1 ON p1.grpid = gp2.grpid
											JOIN questionario.resposta r ON r.perid = p1.perid
															AND r.qrpid = qr.qrpid
															AND r.itpid IS NULL
											WHERE
												chkstatus = 'A'
												--AND qr.qrpid = 117
										)
									) AS f ON f.obrid = re.obrid
								WHERE
									gd.gpdid = {$gpdid}
									{$whereObra}
									
						)
				)	
			) as f";

	return ($db->pegaUm( $sql ) == 0 ? true : false);			
}

function valEnviarParaSAA($gpdid, $obrid=null){
	global $db;
	
	if ( !$gpdid ) return true;
	
	if ( $obrid ) $whereObra = " AND r.obrid = {$obrid}";
	
	$sql = "SELECT 
				count(*) AS total
			FROM
				obras.grupodistribuicao gd
			JOIN obras.itemgrupo ig ON ig.gpdid = gd.gpdid
			JOIN obras.repositorio r ON r.repid = ig.repid 
										AND r.repstatus = 'A'
										{$whereObra}
			JOIN obras.checklistvistoria cv ON cv.obrid = r.obrid
							   				   AND cv.chkstatus = 'A'
			WHERE
				gd.gpdid IN ({$gpdid})
				AND (cv.chksituacao = false OR cv.chksituacao IS NULL);";	
										
	return ($db->pegaUm( $sql ) == 0 ? true : false);
}

function buscaGrupoPelaObra( $obrid ){
	global $db;
	
	$sql = "SELECT
				i.gpdid
			FROM
				obras.repositorio r
			JOIN obras.itemgrupo i ON i.repid = r.repid 
			WHERE
				r.repstatus = 'A'
				AND obrid = {$obrid}";
	
	return $db->pegaUm( $sql );
}

function valGrupoPreenchidoAprovado( $obrid ){
	if ( !$obrid ) return false;

	$gpdid 	 = buscaGrupoPelaObra( $obrid );
	if ( !$gpdid ) return false;
	/*
	 * valEnviarParaSAA()			=> verifica se os "checklist" da OBRA estï¿½o aprovados.
	 * valEnviarParaAvaliacaoMec()	=> verifica se todas as perguntas do "checklist" da OBRA foram preenchidas
	 * 
	 * Foi acrescentado o paramentro "obrid" nas duas funï¿½ï¿½es, com o objetivo de fazer a validaï¿½ï¿½o por OBRA.
	 * Pois para tramitar para o estado "Supervisï¿½o Aprovada" a obra, somente os checklist's daquela obra necessita
	 * estï¿½ aprovado e preenchido.
	 */
	return (valEnviarParaSAA( $gpdid, $obrid ) && valEnviarParaAvaliacaoMec( $gpdid, $obrid ));	
}

function posGrupoPreenchidoAprovado( $obrid ){

	/*
	 * Na tramitaï¿½ï¿½o de todas as obras para o estado "Supervisï¿½o Aprovada", serï¿½ feita essa verifiï¿½ï¿½o e quando
	 * todas as obras do GRUPO estiverem preenchidas e aprovadas, serï¿½ feito automaticamente a tramitaï¿½ï¿½o do worflow
	 * do GRUPO para o estado "Avaliaï¿½ï¿½o Final (SAA)".
	 * 377 ï¿½ a aï¿½ï¿½o "Enviar para SAA" do estado "Em Avaliaï¿½ï¿½o da Supervisï¿½o (MEC)" que resultarï¿½ na tramitaï¿½ï¿½o do worflow
	 * do GRUPO para "Avaliaï¿½ï¿½o Final (SAA)".
	 * 
	 */
	$gpdid = buscaGrupoPelaObra( $obrid );
	if ( valEnviarParaSAA( $gpdid ) && valEnviarParaAvaliacaoMec( $gpdid ) ){
//		$docid = obrPegarDocid( $gpdid );
		$docid = obrPegarDocid( $obrid );
		// checar se o gpdid necessita ser passado como paramentro para as funï¿½ï¿½es pertinentes a aï¿½ï¿½o 377 (id em produï¿½ï¿½o)
//		$arDado = array("gpdid" => $gpdid);
		$arDado = array("obrid" => $obrid);
//		wf_alterarEstado( $docid, 377, "", $arDado );
		wf_alterarEstado( $docid, 649, "", $arDado );
	}
	return true;
}

/**
 * Funï¿½ï¿½o que desabilita os inputs para o PERFIL_EMPRESA
 * @author Rodrigo Pereira de Souza Silva
 *
 */
function chkSituacaoObra(){
	global $db;
	
	if(isset( $_SESSION['obras']['obrid'] )){
		$obrid = $_SESSION['obras']['obrid'];
	} else{
		$obrid = $_SESSION['obra']['obrid'];
	}
	
	// pegando os dados da supervisï¿½o
	if($obrid){
		$sql = "SELECT 
					cv.chksituacao
				FROM 
					obras.checklistvistoria cv
				WHERE 
					cv.obrid = {$obrid};";
		$chksituacao = $db->pegaUm($sql);		
	}
	
	// Caso o parecer do MEC esteja aprovado, o sistema deverï¿½ desabilitar tanto o preenchimento do checklist quanto todos os dados da obra, 
	// somente para o perfil Empresa
	if( ($chksituacao == 't') && ((possuiPerfil(PERFIL_EMPRESA)) && !$db->testa_superuser() ) ){
		echo "<script type='text/javascript' src='http://{$_SERVER['HTTP_HOST']}/includes/JQuery/jquery-1.4.2.js'></script>
			  <script type='text/javascript'>
			  	function desabilitandoEmpresa(){
					$('tbody :input').attr('disabled', true);
				}
			  </script>";
	}
}

/**
 * Funï¿½ï¿½o que remove a responsabilidade da empresa sobre as obras de um grupo
 * Atualmente estï¿½ sendo utilizada no WorkFlow, quando o usuï¿½rio clica em Finalizar Supervisï¿½o
 * @author Rodrigo Pereira de Souza Silva
 * @param integer $gpdid
 * @return bolean
 */
function removeResponsabilidadeEmpresa($gpdid){
	
	global $db;
	$gpdid = (int)$gpdid;
	
	//verificando o(s) usuï¿½rio(s) responsï¿½vel(is) pela(s) obra(s) do grupo
	$sql = "SELECT
				DISTINCT
				oi.obrid,
				ur.usucpf
				--r.repid
			FROM
				obras.itemgrupo i
			JOIN obras.repositorio r USING(repid)
			JOIN obras.obrainfraestrutura oi USING(obrid)
			JOIN entidade.endereco e USING(endid)
			JOIN obras.usuarioresponsabilidade ur ON ur.estuf = e.estuf 
													 AND ur.pflcod = " . PERFIL_EMPRESA . "
													 AND ur.rpustatus = 'A'
			WHERE 
				gpdid = {$gpdid}";
	
	$responsaveis = $db->carregar($sql);
	
	foreach ($responsaveis as $valores) {
		$sql = "UPDATE
					obras.usuarioresponsabilidade
				SET
					rpustatus = 'I'
				WHERE
					obrid = {$valores['obrid']}
					AND usucpf = '{$valores['usucpf']}'";
		$db->executar( $sql );
		
	}
	
	//Inserido  o percentual Executado e a Situaï¿½ï¿½o da Obra da ï¿½ltima Supervisï¿½o.
	//Pegando todos os repid's, os obrpercexec's e os stoid's do grupo.
	$sql = "SELECT 
				oi.obrid,
				oi.obrpercexec,
				oi.stoid
			FROM
				obras.grupodistribuicao gd
			INNER JOIN
				obras.itemgrupo ig ON ig.gpdid = gd.gpdid
			INNER JOIN
				obras.repositorio ore ON ore.repid = ig.repid
			INNER JOIN
				obras.obrainfraestrutura oi ON oi.obrid = ore.obrid
							    AND oi.obsstatus = 'A'        
			WHERE
				gd.gpdstatus = 'A'
				AND ore.repstatus = 'A'
				AND ig.gpdid = {$gpdid} ";
	 
	$obrRep = $db->carregar($sql);
	
	if (is_array($obrRep)) {
		foreach ($obrRep as $obrValores ){
			$sql = " UPDATE
						   obras.obrainfraestrutura
					 SET
					 	   stoidsupemp = {$obrValores['stoid']} , 
					 	   obrsuppercexec = ". $obrValores['obrpercexec'] = $obrValores['obrpercexec'] == '' ? '0.00' : $obrValores['obrpercexec'] ."
					 WHERE
					 	   obrid = {$obrValores['obrid']} ";
				
			$db->executar($sql);
		}
	}
	
	// atualizando o campo obras.repositorio -> repsitsupervisao para F
	// pegando todas os repids do grupo
	$sql = "SELECT 
				ore.repid
			FROM
			        obras.grupodistribuicao gd
			INNER JOIN
			        obras.itemgrupo ig ON ig.gpdid = gd.gpdid
			INNER JOIN
			        obras.repositorio ore ON ore.repid = ig.repid
			WHERE
			        gd.gpdstatus = 'A'
			        AND ore.repstatus = 'A'
			        AND ig.gpdid = {$gpdid}";
	
	$repids = $db->carregarColuna($sql);
	
	if (is_array($repids)) {
		foreach ($repids as $repid) {
			$sql = "UPDATE 
						obras.repositorio
					SET 
						repsitsupervisao='F',
						repstatus='I'
					WHERE 
						repid={$repid}";
			
			$db->executar($sql);
		}					
	}
	
	$db->commit();
	
	
	obrEnviaEmailSupervisaoFinalizada($gpdid);
	
	obrGeraNovaDeclaracao($gpdid);
	
	return true;
	
	
}

/**
 * Funï¿½ï¿½o para geraï¿½ï¿½o de nova declaraï¿½ï¿½o ï¿½ partir
 * do grupo informado. Usado na pï¿½s-aï¿½ï¿½o da aï¿½ï¿½o de workflow: "Finalizar Supervisï¿½o".
 * @author Felipe de Oliveira Carvalho
 * @param integer $gpdid
 * @return void
 */
function obrGeraNovaDeclaracao($gpdid)
{
	ini_set("memory_limit", "256M");
	set_time_limit(0);
	
	global $db;
	$gpdid = (int)$gpdid;
	
	include_once APPRAIZ . "includes/classes/Modelo.class.inc";
	include_once APPRAIZ . "includes/classes/controller/Controller.class.inc";
	include_once APPRAIZ . "includes/classes/modelo/public/Arquivo.class.inc";
	include_once APPRAIZ . "obras/classe/modelo/OrdemServico.class.inc";
	include_once APPRAIZ . "obras/classe/modelo/GrupoDistribuicao.class.inc";
	include_once APPRAIZ . "obras/classe/modelo/EmpresaContratada.class.inc";
	include_once APPRAIZ . "obras/classe/modelo/Declaracao.class.inc";
	include_once APPRAIZ . "obras/classe/controller/DeclaracaoController.class.inc";
	
	/*** Cancela, se necessï¿½rio, a ï¿½ltima declaraï¿½ï¿½o ***/ 
	$sql = "SELECT
				dclid
			FROM
				obras.declaracao
			WHERE
				dclstatus = 'A'
				AND stdid = ".Declaracao::SITUACAO_DECLARACAO_GERADA."
				AND gpdid = ".$gpdid;
	$dclid = $db->pegaUm($sql);
	
	if( $dclid )
	{
		$sql = "UPDATE 
					obras.declaracao
				SET 
					stdid = ".Declaracao::SITUACAO_DECLARACAO_CANCELADA."
				WHERE 
					dclid = {$dclid}";
		$db->executar($sql);
		$db->commit();
	}
	
	$obDeclaracao = new DeclaracaoController();
	$obDeclaracao->salvarDeclaracao($gpdid);
}

/**
 * Funï¿½ï¿½o que verifica se a Supervisï¿½o da obra, possui Itens Vigentes.
 * @author Felipe Evangelista dos Santos  
 * @param $obrid (String) - Id da Obra
 * @param $supvid (String) - Id da Supervisï¿½o
 * @return $icovigente "A" ou "Vazio"
 */
function verificaItenVigente($obrid, $supvid){

		global $db;
		
		$sql="SELECT 		
					itco.icovigente 
			  FROM 
					obras.itenscomposicaoobra itco  
			  INNER JOIN
					obras.supervisaoitenscomposicao sup 
					ON sup.icoid = itco.icoid 
		      WHERE 
		      		itco.obrid = '".$obrid."' 
		      		AND 
		      		itco.icovigente = 'A' 
		      		AND 
		      		sup.supvid = '".$supvid."'";
			
		 $icovigente = $db->carregar( $sql );

    	 return $icovigente;
}


/**
 * Funï¿½ï¿½o responsï¿½vel por exibir o percentual da ï¿½ltima vistoria cadastrada
 * @author Rodrigo Pereira de Souza Silva
 */
function mostraPercentualUltimaVistoria($obrid){
	global $db;
	
// Mï¿½todo antigo de calcular o percentual da ï¿½ltima vistoria	
//	$sql = "SELECT
//				coalesce((SELECT 
//							sum(( icopercsobreobra * supvlrinfsupervisor ) / 100)
//						  FROM 
//							obras.itenscomposicaoobra i
//						  INNER JOIN 
//							obras.supervisaoitenscomposicao si ON i.icoid = si.icoid 
//						  WHERE si.supvid = s.supvid 
//						  	AND obrid = {$obrid} 
//						  	/*AND i.icovigente = 'A'*/ ),'0') as percentual
//			FROM
//				obras.supervisao s
//			LEFT JOIN(SELECT 
//						DISTINCT
//						t.traseq,
//						t.tradsc,
//						sic.supvid
//					  FROM 
//						obras.termoaditivo t
//						JOIN obras.itenscomposicaoobra ico ON t.traid = ico.traid
//						JOIN obras.supervisaoitenscomposicao sic ON sic.icoid = ico.icoid 
//					  WHERE
//						t.obrid = {$obrid} 
//					 ) t ON t.supvid = s.supvid	
//			INNER JOIN 
//				obras.situacaoobra si ON si.stoid = s.stoid
//			INNER JOIN
//				seguranca.usuario u ON u.usucpf = s.usucpf
//			LEFT JOIN
//				entidade.entidade e ON e.entid = s.supvistoriador
//			LEFT JOIN
//				obras.realizacaosupervisao rs ON rs.rsuid = s.rsuid 
//				
//			inner join
//				obras.itenscomposicaoobra i on i.obrid = {$obrid}
//			inner join
//				obras.itenscomposicao ic on i.itcid = ic.itcid
//				
//			WHERE
//				s.obrid = '{$obrid}' AND
//				s.supstatus = 'A'
//				AND i.icovigente = 'A'
//			/*ORDER BY 
//				s.supdtinclusao DESC LIMIT 1*/
//			ORDER BY
//				s.supdtinclusao DESC LIMIT 1";
//	
//	$dados = $db->carregar( $sql );
//	
//	if( is_array($dados) ){
//		
//		// pegando o percentual da ï¿½ltima vistoria
//		$percentual = $dados[0]["percentual"];
//		$percentual = $percentual > 100.00 ? 100.00 : $percentual;
//		return number_format($percentual,2,',','.')." %";
//	}else{
//		return "0 %";
//	}
	
// Mï¿½todo novo de calcular o percentual da ï¿½ltima vistoria
	$sql = "SELECT
				obrpercexec
			FROM
				obras.obrainfraestrutura
			WHERE
				obrid = {$obrid}";
	
	$valor = $db->pegaUm($sql);
	
	return number_format($valor,2,',','.')." %";
	
}

/**
 * Funï¿½ï¿½o que verifica se as obras do grupo estï¿½o na situaï¿½ï¿½o Supervisï¿½o Aprovada
 * Esta funï¿½ï¿½o retornarï¿½ true caso todas as obras do grupo estiverem com a situaï¿½ï¿½o Supervisï¿½o Aprovada
 * @param gpdid integer
 * @return boolean
 * @author Rodrigo Pereira de Souza Silva
 */
function verificaSituacaoObras( $gpdid ) {
	global $db;
	
	$gpdid = (int)$gpdid;
	
	// select com todas as obras do grupo
	$sql = "SELECT
				d.esdid
			FROM
				obras.itemgrupo ig
			INNER JOIN
				obras.repositorio ore ON ore.repid = ig.repid
			INNER JOIN
				obras.obrainfraestrutura oi ON oi.obrid = ore.obrid
			INNER JOIN
				obras.situacaoobra so ON so.stoid = oi.stoid
			INNER JOIN
				workflow.documento d ON d.docid = oi.docid
			WHERE
				gpdid = {$gpdid}
				AND repstatus = 'A'";
	
	$estadoObras = $db->carregar($sql);
	$estadoDifetente = 0;
	if( is_array($estadoObras) ){
		
		foreach ($estadoObras as $chave => $estadoObra) {
			if( $estadoObra != OBRAVISTORIAAPROVADA ){
				$estadoDifetente = 1;
			}
		}// fim do foreach
		
		if(!$estadoDifetente){
			return true;
		}
		
	}// fim do if
	
	return "Favor preencher o checklist de todas as obras do grupo {$gpdid}.";
}

/**
 * 
 * Funï¿½ï¿½o que envia e-mail para a empresa durante a tramitaï¿½ï¿½o da obra no workflow
 * @param integer $obrid
 * @author Rodrigo Pereira de Souza Silva
 */
function enviaEmailMECEmpresa($obrid, $gpdid){
	global $db;
	
	$obrid = (int)$obrid;
	$gpdid = (int)$gpdid;
	if($obrid && $gpdid){
		
		$sql = "SELECT 
					orgid
				FROM 
					obras.obrainfraestrutura 
				WHERE 
					obrid = {$obrid}
					AND obsstatus = 'A'";
		
		$orgid = $db->pegaUm($sql);
		
		if($orgid == 1){
			//superior
			$remetente = "monitoramentodeobras.sesucg.po@mec.gov.br";
		}elseif($orgid == 2){
			//profissional
			$remetente = "monitoramentodeobras@mec.gov.br";
		}else{
			//bï¿½sico
			$remetente = "monitoramentoobras@mec.gov.br";
		}
		
		//pegando o e-mail da empresa
		$sql = "SELECT DISTINCT 
					ee.entemail,
					oi.obrdesc
				FROM
					obras.grupodistribuicao gd
				INNER JOIN	
					obras.itemgrupo itg ON itg.gpdid = gd.gpdid
				INNER JOIN
					obras.repositorio ore ON ore.repid = itg.repid
				INNER JOIN
					obras.obrainfraestrutura oi ON oi.obrid = ore.obrid
				INNER JOIN
					entidade.endereco ed ON ed.endid = oi.endid
				INNER JOIN
					obras.orgao AS o ON o.orgid = oi.orgid 
				LEFT JOIN
					obras.empresacontratada ec ON ec.epcid = gd.epcid 
				LEFT JOIN
					entidade.entidade ee ON ee.entid = ec.entid
				WHERE
					gd.gpdstatus = 'A' 
					AND oi.obrid = {$obrid}
					AND repstatus = 'A'
					AND ee.entstatus = 'A'";
		
		$dados = $db->pegaLinha($sql);
		
		$destinatario = $dados['entemail'];
		
		$nome_obra = $dados['obrdesc'];
		
		//$assunto = "{$gpdid} - Grupo";
		$assunto = "Supervisï¿½o Empresa: {$obrid} - {$nome_obra}";
		
		//Funï¿½ï¿½o que recupera o ID da Situaï¿½ï¿½o da Obra.
		$esdidObra = recuperaSituacaoObra($obrid);
		
		switch ((integer)$esdidObra){ 
			
			//Caso a Obra esteja na Situaï¿½ï¿½o: Ajuste da Supervisï¿½o (Empresa). 
			case $esdidObra == OBRAAJUSTESUPERVISAO_EMPRESA:
			$conteudo = "<br><br>Atenï¿½ï¿½o:<br><br>A supervisï¿½o realizada na Obra: {$obrid} - {$nome_obra} foi encaminhada para \"Ajuste de supervisï¿½o (Empresa)\". Vocï¿½ deve atender ï¿½s alteraï¿½ï¿½es solicitadas e tramitar a obra para \"Reavaliaï¿½ï¿½o de supervisï¿½o MEC\", no prazo mï¿½ximo de 10 dias, a partir da data de hoje.<br><br>Atenciosamente,<br><br>Equipe de Monitoramento de Obras.";
			break;
			//Caso a Obra esteja na Situaï¿½ï¿½o: Reajuste de supervisï¿½o (Empresa).
			case $esdidObra == OBRAREAJUSTESUPERVISAO_EMPRESA:
			$conteudo ="<br><br>Atenï¿½ï¿½o:<br><br>A supervisï¿½o realizada na Obra: {$obrid} - {$nome_obra} foi encaminhada para \"Reajuste de supervisï¿½o (Empresa)\". Vocï¿½ deve atender ï¿½s alteraï¿½ï¿½es solicitadas e tramitar a obra para \"Reavaliaï¿½ï¿½o de supervisï¿½o MEC\", no prazo mï¿½ximo de 10 dias, a partir da data de hoje.<br><br>Atenciosamente,<br><br>Equipe de Monitoramento de Obras."; 
			break;
			default:
			$conteudo = "Obra: {$obrid} - {$nome_obra} - Grupo {$gpdid} - Tramitado para Anï¿½lise.";
			break;
					
		}
		
		enviar_email( $remetente, $destinatario, $assunto, $conteudo );
		
		return true;
	}else{
		return false;
	}// fim do primeiro if
	
}

/**
 * 
 * Funï¿½ï¿½o que envia e-mail para o MEC durante a tramitaï¿½ï¿½o da obra no workflow
 * @param integer $obrid
 * @author Rodrigo Pereira de Souza Silva
 */
function enviaEmailEmpresaMEC($obrid, $gpdid){
	global $db;
	
	$obrid = (int)$obrid;
	$gpdid = (int)$gpdid;
	if($obrid && $gpdid){
		
		// Alterando o campo chksituacao da tabela obras.checklistvistoria as obras que questï¿½o com chksituacao=false prara chksituacao=null
		$sql = "UPDATE
					obras.checklistvistoria ocv
				SET
					chksituacao = null
				WHERE
					ocv.obrid IN (SELECT
									oi.obrid
										
									FROM
										obras.itemgrupo ig
									INNER JOIN
										obras.repositorio ore ON ore.repid = ig.repid
									INNER JOIN
										obras.obrainfraestrutura oi ON oi.obrid = ore.obrid
									INNER JOIN
										obras.situacaoobra so ON so.stoid = oi.stoid
									INNER JOIN
										obras.checklistvistoria cv ON cv.obrid = oi.obrid
									WHERE
										ig.gpdid = {$gpdid}
										AND ore.repsitsupervisao <> ''
										AND oi.obsstatus = 'A'
										AND cv.chksituacao = false)";
		
		$db->carregar( $sql );
		
		$sql = "SELECT 
					orgid
				FROM 
					obras.obrainfraestrutura 
				WHERE 
					obrid = {$obrid}
					AND obsstatus = 'A'";
		
		$orgid = $db->pegaUm($sql);
		
		if($orgid == 1){
			//superior
			$destinatario = "monitoramentodeobras.SESUCGPO@mec.gov.br";
		}elseif($orgid == 2){
			//profissional
			$destinatario = "monitoramentodeobras@mec.gov.br";
		}else{
			//bï¿½sico
			/* Email do Ensino Bï¿½sico alterado dia 12/04/2011 as 11:44 h.
			 *$destinatario = "monitoramentoobras@mec.gov.br";
			 */ 
			$destinatario = "monitoraobrascgimp@fnde.gov.br";
		}
		
		//pegando o e-mail da empresa
		$sql = "SELECT DISTINCT 
					ee.entemail,
					oi.obrdesc
				FROM
					obras.grupodistribuicao gd
				INNER JOIN	
					obras.itemgrupo itg ON itg.gpdid = gd.gpdid
				INNER JOIN
					obras.repositorio ore ON ore.repid = itg.repid
				INNER JOIN
					obras.obrainfraestrutura oi ON oi.obrid = ore.obrid
				INNER JOIN
					entidade.endereco ed ON ed.endid = oi.endid
				INNER JOIN
					obras.orgao AS o ON o.orgid = oi.orgid 
				LEFT JOIN
					obras.empresacontratada ec ON ec.epcid = gd.epcid 
				LEFT JOIN
					entidade.entidade ee ON ee.entid = ec.entid
				WHERE
					gd.gpdstatus = 'A' 
					AND oi.obrid = {$obrid}
					AND repstatus = 'A'
					AND ee.entstatus = 'A'";
		
		$dados = $db->pegaLinha($sql);
		
		$remetente = $dados['entemail'];
		
		$nome_obra = $dados['obrdesc'];
		
		$assunto = "{$gpdid} - Grupo";
		
		$conteudo = "Obra: {$obrid} - {$nome_obra} - Grupo {$gpdid} - Tramitado para Anï¿½lise.";
		
		enviar_email( $remetente, $destinatario, $assunto, $conteudo );
		
		return true;
	}else{
		return false;
	}// fim do primeiro if
	
}

function atualizarFotosVistoria($supvid = null)
{
	global $db;
	if($_POST['hdn_fotos_galeria'] || $_POST['hdn_fotos_supervisao']){
		$_POST['hdn_fotos_galeria'] = str_replace(array("s_foto_","[]="),array("","_"),$_POST['hdn_fotos_galeria']);
		$_POST['hdn_fotos_supervisao'] = str_replace(array("foto_","[]="),array("","_"),$_POST['hdn_fotos_supervisao']);
		$_REQUEST['fotosGaleria']    = explode("&",$_POST['hdn_fotos_galeria']);
		$_REQUEST['fotosSupervisao'] = explode("&",$_POST['hdn_fotos_supervisao']);
	}
		
	$obrid  = $_SESSION["obra"]['obrid'];
	$supvid = !$supvid ? $_REQUEST['supvid'] : $supvid;
	
	if($_REQUEST['fotosSupervisao'][0] && $obrid && $supvid){
		$n = 0;
		foreach($_REQUEST['fotosSupervisao'] as $fotoSupervisao){
			
			$fotoSupervisao = trim($fotoSupervisao);
			
			if(!is_numeric($fotoSupervisao)){
				if(file_exists("../../arquivos/obras/imgs_tmp/".$fotoSupervisao)){
					$imagem = $fotoSupervisao;
					$imagem = str_replace("___","/",$imagem);
		 			$part1file = explode("__temp__", $imagem);
		 			$part2file = explode("__extension__", $part1file[0]);
		 			$part2file[0] = md5_decrypt($part2file[0]);
		 			$part2file[1] = md5_decrypt($part2file[1]);
		 			$nomearquivo = explode(".", $part2file[0]);
		 			if(is_readable("../../arquivos/obras/imgs_tmp/".$imagem.".d")) {
		 				$descricao = file_get_contents("../../arquivos/obras/imgs_tmp/".$imagem.".d");
		 			}
					//Insere o registro da imagem na tabela public.arquivo
		 			$sql = "INSERT INTO public.arquivo(arqnome,arqdescricao,arqextensao,arqtipo,arqdata,arqhora,usucpf,sisid)
					values('". substr($nomearquivo[0],0,255) ."','". substr($descricao,0,255) ."','".$nomearquivo[(count($nomearquivo)-1)]."','". $part2file[1] ."','".date('Y-m-d')."','".date('H:i:s')."','".$_SESSION["usucpf"]."',15) RETURNING arqid;";
		 			$arqid = $db->pegaUm($sql);
		 			if(!is_dir('../../arquivos/obras/'.floor($arqid/1000))) {
		 				mkdir(APPRAIZ.'/arquivos/obras/'.floor($arqid/1000), 0777);
		 			}
		 			if(@copy("../../arquivos/obras/imgs_tmp/".$imagem,"../../arquivos/obras/".floor($arqid/1000)."/".$arqid)){
		 				unlink("../../arquivos/obras/imgs_tmp/".$imagem);
		 				$_sql = "INSERT INTO obras.fotos(arqid,obrid,supvid,fotdsc,fotbox,fotordem)
						values({$arqid},{$_SESSION['obra']["obrid"]},{$supvid},'{$imagem}','imageBox{$n}',{$n});";
		 				$db->executar($_sql);
		 			}
		 		$fotoSupervisao = $arqid;
				$sqlFotos.= "update obras.fotos set fotordem = $n where arqid = $fotoSupervisao and obrid = $obrid and supvid = $supvid;";
				$n++;
				}
			}else{
				$sqlFotos.= "update obras.fotos set fotordem = $n where arqid = $fotoSupervisao and obrid = $obrid and supvid = $supvid;";
				$n++;	
			}
			$arrFotoid[] = $fotoSupervisao;
			
		}
	}
	
	if($arrFotoid){
		$sqlFotos.= "update obras.arquivosobra set aqostatus = 'I' where arqid in (".implode(",",str_replace("foto_","",$arrFotoid)).") and obrid = $obrid and tpaid = ".TIPO_ARQUIVO_FOTO_VISTORIA.";";
	}
	
	if($_REQUEST['fotosGaleria'][0] && $obrid && $supvid){
		foreach($_REQUEST['fotosGaleria'] as $fotoGaleria){
			
			$fotoGaleria = str_replace("s_foto_","",$fotoGaleria);
			
			if(is_numeric($fotoGaleria)){
				
				$sqlFotos.= "INSERT INTO 
						obras.arquivosobra ( 
									 obrid,
									 tpaid,
									 arqid,
									 usucpf,
									 aqodtinclusao,
									 aqostatus )
							VALUES ( 
									$obrid,
									 ".TIPO_ARQUIVO_FOTO_VISTORIA.",
									 $fotoGaleria,
									 '{$_SESSION["usucpf"]}',
									 'now',
									 'A' );";
			}
		}
	}
	if($sqlFotos){
		$db->executar($sqlFotos);
	}
	if($db->commit()){
		return  true;
	}else{
		return  false;
	}
}

function verificaParecerFavoravelSAA($gpdid){
	
	global $db;
	
	if ( !$gpdid ) return true;
	
	$sql = "SELECT DISTINCT
					f.situacao,
					f.obra,
					f.itemgrupo,
					MAX(f.dataultminclusao)
			FROM
			((
					SELECT
						DISTINCT
						oi.obrid AS obra,
						CASE WHEN mpcckl.mpcsituacao = TRUE 	
							THEN 1 
						     WHEN mpcckl.mpcsituacao = FALSE 
							THEN 0 
						END AS situacao,
						ig.itgid AS itemgrupo,
						mpcckl.mpcdtinclusao AS dataultminclusao
						
					FROM
						obras.grupodistribuicao gd
					JOIN obras.itemgrupo ig USING(gpdid)
					JOIN obras.repositorio r ON r.repid = ig.repid
								 AND r.repstatus = 'A'
					LEFT JOIN obras.checklistvistoria cv ON cv.obrid = r.obrid
									     AND cv.chkstatus = 'A'
					LEFT JOIN obras.movparecercklist mpcckl ON mpcckl.chkid = cv.chkid
										AND mpcckl.mpcstatus = 'A'
					JOIN obras.obrainfraestrutura oi ON oi.obrid = r.obrid
					JOIN obras.orgao og ON oi.orgid = og.orgid
					JOIN entidade.entidade ee ON oi.entidunidade = ee.entid										    	
					WHERE
						gd.gpdid = {$gpdid}
						
			)UNION ALL(
					
					SELECT	DISTINCT
						oi.obrid AS obra,
						CASE WHEN mpcckl.mpcsituacao = TRUE 	
							THEN 1 
						     WHEN mpcckl.mpcsituacao = FALSE 
							THEN 0 
						END AS situacao,
						ig.itgid AS itemgrupo,
						mpcckl.mpcdtinclusao AS dataultminclusao
					FROM
					     obras.grupodistribuicao gd
					JOIN obras.itemgrupo ig USING(gpdid)
					JOIN obras.repositorio r USING(repid)
					JOIN obras.checklistvistoria cv USING(obrid)
					JOIN obras.movparecercklist mpcckl ON mpcckl.chkid = cv.chkid
					JOIN questionario.questionarioresposta qr USING(qrpid)
					JOIN questionario.questionario q USING(queid)
					JOIN questionario.grupopergunta gp USING(queid)
					JOIN questionario.pergunta p USING (grpid)
					JOIN obras.obrainfraestrutura oi ON oi.obrid = r.obrid	
					JOIN obras.orgao og ON oi.orgid = og.orgid
					JOIN entidade.entidade ee ON oi.entidunidade = ee.entid
					WHERE
						gd.gpdid = {$gpdid}
						AND cv.chkstatus = 'A'
			)) AS f GROUP BY f.situacao, f.obra, f.itemgrupo ORDER BY f.itemgrupo";
	
	$dadosParecer =$db->carregarColuna( $sql );
	
	$parecer = in_array( 0 ,$dadosParecer) ? false : true;
	
	return $parecer;
}

/**
 * Funï¿½ï¿½o que verifica se o Checklist da Obra do grupo foi completamente preenchido.
 * Esta funï¿½ï¿½o retornarï¿½ true caso a Obras do grupo estiver com o Checklist completamente preenchido.
 * @param gpdid integer
 * @param obrid integer
 * @return boolean
 */
function verificaChecklistObrasIndividual( $gpdid , $obrid ) {
	
	global $db;
	
	$gpdid = (int)$gpdid;
	$obrid = (int)$obrid;
	
	if ( !$gpdid && !$obrid ) return true;
	
	$sql = "SELECT 
				SUM(nrespondida) as nrespondida 
			FROM(
				(
					SELECT
						COUNT(ig.gpdid) as nrespondida
					FROM
						obras.grupodistribuicao gd
					INNER JOIN obras.itemgrupo ig USING(gpdid)
					INNER JOIN obras.repositorio re ON re.repid = ig.repid
													  AND re.repstatus = 'A'
													  AND re.obrid = {$obrid}	
					LEFT JOIN obras.checklistvistoria cv ON cv.obrid = re.obrid
										    			    AND cv.chkstatus = 'A'	
					WHERE
						gd.gpdid = {$gpdid}
						AND cv.chkid IS NULL
				)UNION ALL(
					
					SELECT
						COUNT(p.perid) as nrespondida
					FROM
						obras.grupodistribuicao gd
					JOIN obras.itemgrupo ig USING(gpdid)
					JOIN obras.repositorio re USING(repid)
					JOIN obras.checklistvistoria cv USING(obrid)
					JOIN questionario.questionarioresposta qr USING(qrpid)
					JOIN questionario.questionario q USING(queid)
					JOIN questionario.grupopergunta gp USING(queid)
					JOIN questionario.pergunta p USING (grpid)	
					WHERE
						gd.gpdid = {$gpdid}
						AND re.obrid = {$obrid}
						AND cv.chkstatus = 'A'
						AND p.perid::text || cv.obrid::text NOT IN (
								SELECT 
									f.idpergunta::text || cv.obrid::text
								FROM
									obras.grupodistribuicao gd
									JOIN obras.itemgrupo ig USING(gpdid)
									JOIN obras.repositorio re USING(repid)
									JOIN (
										(
											-- Perguntas vinculadas a grupos filhos de questionarios e de resposta textual
											SELECT
												p.perid as idpergunta,
												cv.obrid
											FROM
												obras.checklistvistoria cv
											JOIN obras.repositorio re ON re.obrid = cv.obrid
																	  AND re.repstatus = 'A'
																	  AND re.obrid = {$obrid}
											JOIN obras.itemgrupo ig ON ig.repid = re.repid
														  			AND ig.gpdid = {$gpdid}
											JOIN questionario.questionarioresposta qr USING (qrpid)
											JOIN questionario.questionario q ON q.queid = qr.queid
											JOIN questionario.grupopergunta gp ON gp.queid = q.queid
											JOIN questionario.pergunta p ON p.grpid = gp.grpid
											JOIN questionario.resposta r ON r.perid = p.perid
																		 AND r.qrpid = qr.qrpid
																		 AND r.itpid IS NULL
											WHERE
												chkstatus = 'A'
												--AND qr.qrpid = 117
										)UNION ALL(
											-- Perguntas vinculadas a grupos filhos de questionarios e que possuem item como resposta
											SELECT
												p.perid as idpergunta,
												cv.obrid		
											FROM
												obras.checklistvistoria cv
											JOIN obras.repositorio re ON re.obrid = cv.obrid
																	  AND re.repstatus = 'A'
																	  AND re.obrid = {$obrid}
											JOIN obras.itemgrupo ig ON ig.repid = re.repid
														 			AND ig.gpdid = {$gpdid}
											JOIN questionario.questionarioresposta qr USING (qrpid)
											JOIN questionario.questionario q ON q.queid = qr.queid
											JOIN questionario.grupopergunta gp ON gp.queid = q.queid
											JOIN questionario.pergunta p ON p.grpid = gp.grpid
											JOIN questionario.itempergunta ip ON ip.perid = p.perid
											JOIN questionario.resposta r ON r.perid = p.perid
																		 AND r.qrpid = qr.qrpid
																		 AND r.itpid = ip.itpid
											WHERE
												chkstatus = 'A'
												--AND qr.qrpid = 117
										)UNION ALL(
											-- Perguntas de resposta textual filhas de itens de perguntas vinculadas a grupos filhos de questionarios
											SELECT
												p.perid as idpergunta,
												cv.obrid		
											FROM
												obras.checklistvistoria cv
											JOIN obras.repositorio re ON re.obrid = cv.obrid
																	  AND re.repstatus = 'A'
																	  AND re.obrid = {$obrid}
											JOIN obras.itemgrupo ig ON ig.repid = re.repid
														 			AND ig.gpdid = {$gpdid}
											JOIN questionario.questionarioresposta qr USING (qrpid)
											JOIN questionario.questionario q ON q.queid = qr.queid
											JOIN questionario.grupopergunta gp ON gp.queid = q.queid
											JOIN questionario.pergunta p ON p.grpid = gp.grpid
											JOIN questionario.itempergunta ip ON ip.perid = p.perid
											JOIN questionario.pergunta p1 ON p1.itpid = ip.itpid
											JOIN questionario.resposta r ON r.perid = p1.perid
																		 AND r.qrpid = qr.qrpid
																		 AND r.itpid IS NULL
											WHERE
												chkstatus = 'A'
												--AND qr.qrpid = 117
										)UNION ALL(
										-- Perguntas de resposta textual filhas de grupos filhos de itens de perguntas vinculadas a grupos filhos de questionarios
											SELECT
												p.perid as idpergunta,
												cv.obrid		
											FROM
												obras.checklistvistoria cv
											JOIN obras.repositorio re ON re.obrid = cv.obrid
																	  AND re.repstatus = 'A'
																	  AND re.obrid = {$obrid}
											JOIN obras.itemgrupo ig ON ig.repid = re.repid
														 			AND ig.gpdid = {$gpdid}
											JOIN questionario.questionarioresposta qr USING (qrpid)
											JOIN questionario.questionario q ON q.queid = qr.queid
											JOIN questionario.grupopergunta gp ON gp.queid = q.queid
											JOIN questionario.pergunta p ON p.grpid = gp.grpid
											JOIN questionario.itempergunta ip ON ip.perid = p.perid
											JOIN questionario.grupopergunta gp1 ON gp1.itpid = ip.itpid
											JOIN questionario.pergunta p1 ON p1.grpid = gp1.grpid
											JOIN questionario.resposta r ON r.perid = p1.perid
																		 AND r.qrpid = qr.qrpid
																		 AND r.itpid IS NULL
											WHERE
												chkstatus = 'A'
												--AND qr.qrpid = 117
										)UNION ALL(
											-- Perguntas de resposta textual filhas de grupos vinculadas a grupos filhos de questionarios
											SELECT
												p.perid as idpergunta,
												cv.obrid		
											FROM
												obras.checklistvistoria cv
											JOIN obras.repositorio re ON re.obrid = cv.obrid
																	  AND re.repstatus = 'A'
																	  AND re.obrid = {$obrid}
											JOIN obras.itemgrupo ig ON ig.repid = re.repid
														  			AND ig.gpdid = {$gpdid}
											JOIN questionario.questionarioresposta qr USING (qrpid)
											JOIN questionario.questionario q ON q.queid = qr.queid
											JOIN questionario.grupopergunta gp ON gp.queid = q.queid
											JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
											JOIN questionario.pergunta p ON p.grpid = gp1.grpid
											JOIN questionario.resposta r ON r.perid = p.perid
																		 AND r.qrpid = qr.qrpid
																		 AND r.itpid IS NULL
											WHERE
												chkstatus = 'A'
												--AND qr.qrpid = 117
										)UNION ALL(
											-- Perguntas de resposta por itens filhas de grupos vinculadas a grupos filhos de questionarios
											SELECT
												p.perid as idpergunta,
												cv.obrid
											FROM
												obras.checklistvistoria cv
											JOIN obras.repositorio re ON re.obrid = cv.obrid
																	  AND re.repstatus = 'A'
																	  AND re.obrid = {$obrid}
											JOIN obras.itemgrupo ig ON ig.repid = re.repid
														  			AND ig.gpdid = {$gpdid}
											JOIN questionario.questionarioresposta qr USING (qrpid)
											JOIN questionario.questionario q ON q.queid = qr.queid
											JOIN questionario.grupopergunta gp ON gp.queid = q.queid
											JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
											JOIN questionario.pergunta p ON p.grpid = gp1.grpid
											JOIN questionario.itempergunta ip ON ip.perid = p.perid
											JOIN questionario.resposta r ON r.perid = p.perid
																		 AND r.itpid = ip.itpid
																		 AND r.qrpid = qr.qrpid
																		 AND r.itpid IS NOT NULL
											WHERE
												chkstatus = 'A'
												--AND qr.qrpid = 117
										)UNION ALL(
											-- Perguntas de resposta textual filhas de itens de perguntas filhas de grupos vinculadas a grupos filhos de questionarios
											SELECT
												p.perid as idpergunta,
												cv.obrid
											FROM
												obras.checklistvistoria cv
											JOIN obras.repositorio re ON re.obrid = cv.obrid
																	  AND re.repstatus = 'A'
																	  AND re.obrid = {$obrid}
											JOIN obras.itemgrupo ig ON ig.repid = re.repid
														  			AND ig.gpdid = {$gpdid}
											JOIN questionario.questionarioresposta qr USING (qrpid)
											JOIN questionario.questionario q ON q.queid = qr.queid
											JOIN questionario.grupopergunta gp ON gp.queid = q.queid
											JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
											JOIN questionario.pergunta p ON p.grpid = gp1.grpid
											JOIN questionario.itempergunta ip ON ip.perid = p.perid
											JOIN questionario.pergunta p1 ON p1.itpid = ip.itpid
											JOIN questionario.resposta r ON r.perid = p1.perid
																		 AND r.qrpid = qr.qrpid
																		 AND r.itpid IS NULL
											WHERE
												chkstatus = 'A'
												--AND qr.qrpid = 117
										)UNION ALL(
										-- Perguntas de resposta textual filhas de grupos filhos de itens de perguntas filhas de grupos vinculadas a grupos filhos de questionarios
											SELECT
												p.perid as idpergunta,
												cv.obrid		
											FROM
												obras.checklistvistoria cv
											JOIN obras.repositorio re ON re.obrid = cv.obrid
																	  AND re.repstatus = 'A'
																	  AND re.obrid = {$obrid}
											JOIN obras.itemgrupo ig ON ig.repid = re.repid
														  			AND ig.gpdid = {$gpdid}
											JOIN questionario.questionarioresposta qr USING (qrpid)
											JOIN questionario.questionario q ON q.queid = qr.queid
											JOIN questionario.grupopergunta gp ON gp.queid = q.queid
											JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
											JOIN questionario.pergunta p ON p.grpid = gp1.grpid
											JOIN questionario.itempergunta ip ON ip.perid = p.perid
											JOIN questionario.grupopergunta gp2 ON gp2.itpid = ip.itpid
											JOIN questionario.pergunta p1 ON p1.grpid = gp2.grpid
											JOIN questionario.resposta r ON r.perid = p1.perid
																		 AND r.qrpid = qr.qrpid
																		 AND r.itpid IS NULL
											WHERE
												chkstatus = 'A'
												--AND qr.qrpid = 117
										)
									) AS f ON f.obrid = re.obrid
								WHERE
									gd.gpdid = {$gpdid}
									AND re.obrid = {$obrid}
									
						)
				)	
			) as f";

	return ($db->pegaUm( $sql ) == 0 ? true : false);
	
}

/**
 * Funï¿½ï¿½o que verifica se as Obras do grupo estï¿½o em Situaï¿½ï¿½es diferentes de: "Em Supervisï¿½o(OBREMSUPERVISAOIND-240)" , 
 * "Em Ajuste de supervisï¿½o(Empresa)(OBRAAJUSTESUPERVISAO_EMPRESA-242)" e "Em Reajuste da supervisï¿½o(Empresa)(OBRAREAJUSTESUPERVISAO_EMPRESA-279)". 
 * @param gpdid integer
 * @return boolean
 */
function verificaSituacaoObrasGrupo( $gpdid ) {
	
	global $db;
	
	if ( !$gpdid ) return true;
	
	$gpdid = (int)$gpdid;
	
	// Seleciona todas as Situaï¿½ï¿½es das Obras do grupo.
	$sql = "SELECT
				d.esdid
			FROM
				obras.itemgrupo ig
			INNER JOIN
				obras.repositorio ore ON ore.repid = ig.repid
			INNER JOIN
				obras.obrainfraestrutura oi ON oi.obrid = ore.obrid
			INNER JOIN
				obras.situacaoobra so ON so.stoid = oi.stoid
			INNER JOIN
				workflow.documento d ON d.docid = oi.docid
			WHERE
				gpdid = {$gpdid}
				AND repstatus = 'A'";
	
	$estadoObras = $db->carregarColuna($sql);
	
	if( !empty($estadoObras) ){
		
		//Recupera o Documneto(docid) do Grupo.
			$docid = obrPegarDocid( $gpdid );
			//Recupera o Estado atual do Grupo.
			$estado = wf_pegarEstadoAtual( $docid );
			//Verifica o Estado atual do Grupo.
			switch ((integer)$estado['esdid']){
				
				case $estado['esdid'] == OBREMSUPERVISAO :
				
					$estadoObrasNoGrupo = array( OBREMSUPERVISAOIND, OBRAAJUSTESUPERVISAO_EMPRESA, OBRAREAJUSTESUPERVISAO_EMPRESA );
		
					if( count(array_intersect($estadoObras, $estadoObrasNoGrupo)) == 0){
									
						return true;
						
					}
				break;
				
				case (integer)$estado['esdid'] == OBREMAVALIASUPERVMEC:
					
					$estadoObrasNoGrupoEmAvlSupMEC = array( OBRAAVALIACAOSUPERVISAO_MEC, OBRAREAVALIACAOSUPERVISAO_MEC, OBRASUPERVISAOAPROVADAOBRA );	
			
					if( count(array_intersect($estadoObras, $estadoObrasNoGrupoEmAvlSupMEC)) == 0 ){		
					
						return true;
				}
				break;
				case (integer)$estado['esdid'] == OBRAVALIAFINALSAA:
					
					$estadoObrasSupAprovada = array( OBRASUPERVISAOAPROVADAOBRA );	
			
					if( count(array_intersect($estadoObras, $estadoObrasSupAprovada)) == 0 ){		
					
						return true;
				}
				break;
			}

	return false;
	
	}
}
/**
 * Funï¿½ï¿½o que verifica se as Obras do grupo estï¿½o em Situaï¿½ï¿½es diferentes de: "Em Supervisï¿½o(Empresa)(OBREMSUPERVISAOIND-240)" , 
 * "Em Ajuste de supervisï¿½o(Empresa)(OBRAAJUSTESUPERVISAO_EMPRESA-242)" e "Em Reajuste da supervisï¿½o(Empresa)(OBRAREAJUSTESUPERVISAO_EMPRESA-279)". 
 * Caso as Obras estjam diferentes o Grupo terï¿½ o seu Estado alterado para "Em Avaliaï¿½ï¿½o da Supervisï¿½o(MEC)(OBREMAVALIASUPERVMEC-171)" no workflow.  
* @param gpdid integer
 * 
 */
function tramitaGrupo($gpdid){
	
	global $db; 
	
	if ( !$gpdid ) return true;
	
	// Seleciona todas as Situaï¿½ï¿½es das Obras do Grupo.
	$sql = "SELECT
				d.esdid
			FROM
				obras.itemgrupo ig
			INNER JOIN
				obras.repositorio ore ON ore.repid = ig.repid
			INNER JOIN
				obras.obrainfraestrutura oi ON oi.obrid = ore.obrid
			INNER JOIN
				obras.situacaoobra so ON so.stoid = oi.stoid
			INNER JOIN
				workflow.documento d ON d.docid = oi.docid
			WHERE
				gpdid = {$gpdid}
				AND repstatus = 'A'";
	
	$estadoObras = $db->carregarColuna($sql);
	 
	if( !empty($estadoObras) ){
		/** A Tramitaï¿½ï¿½o do Grupo de Obras atual.**/
			
			//O Estado em que as Obras do Grupo devem estar para a Tramitaï¿½ï¿½o do Grupo.
			$estadoObrasNoGrupoSupAprovada = array( OBRASUPERVISAOAPROVADAOBRA );	
			//Verificaï¿½ï¿½o da Intersecï¿½ï¿½o entre as situaï¿½ï¿½es em que as Obras devem estar.
			if( count(array_intersect($estadoObras, $estadoObrasNoGrupoSupAprovada)) == count($estadoObras) ){		
				//Recupera o Documneto(docid) do Grupo.
				$docid = obrPegarDocid( $gpdid );
				//Recupera o Estado atual do Grupo.
				$estado = wf_pegarEstadoAtual( $docid );
				//Verifica o Estado atual do Grupo.
				switch ((integer)$estado['esdid']){
					//Caso o Estado atual do Grupo seja "Grupo em Supervisï¿½o".
					case (integer)$estado['esdid'] == GRUPOEMSUPERVISAO: 
					   echo '<script> alert("As Obras do Grupo nï¿½:'.$gpdid.' atendem as condiï¿½ï¿½es necessï¿½rias para a Tramitaï¿½ï¿½o do Grupo.\nEstado atual do Grupo: Grupo em Supervisï¿½o (MEC)\nO Grupo serï¿½ Tramitado para o Estado:  Em Avaliaï¿½ï¿½o Final(SAA) ")</script>';
					   $arDado = array("gpdid" => $gpdid );
					    //Se as Obras estiverem com as situaï¿½ï¿½es iguais ao Estado de:"Supervisï¿½o Aprovada(OBRASUPERVISAOAPROVADAOBRA-244)". 
					    //Entï¿½o o Estado serï¿½ alterado para "Em Avaliaï¿½ï¿½o Final(SAA)(OBRAVALIAFINALSAA-172)"
					   wf_alterarEstado( $docid, 742, "", $arDado );
					break;	
				}
			}
			
		/**---Fim da Tramitaï¿½ï¿½o do Grupo atual---**/
			
		/** A Tramitaï¿½ï¿½o do Grupo de Obras antiga.
		 * 
		 *  
		//Os Estados em que as Obras nï¿½o podem estar.
		$estadoObrasNoGrupo = array( OBREMSUPERVISAOIND, OBRAAJUSTESUPERVISAO_EMPRESA, OBRAREAJUSTESUPERVISAO_EMPRESA );
		//Verificaï¿½ï¿½o da Intersecï¿½ï¿½o entre as situaï¿½ï¿½es em que as Obras nï¿½o podem estar.
		if( count(array_intersect($estadoObras, $estadoObrasNoGrupo)) == 0 ){
			//Recupera o Documneto(docid) do Grupo.
			$docid = obrPegarDocid( $gpdid );
			//Recupera o Estado atual do Grupo.
			$estado = wf_pegarEstadoAtual( $docid );
			//Verifica o Estado atual do Grupo.
			switch ((integer)$estado['esdid']){
				//Caso o Estado atual do Grupo seja "Em Supervisï¿½o(Empresa)".
				case (integer)$estado['esdid'] == OBREMSUPERVISAO:
				   echo '<script> alert("As Obras do Grupo nï¿½:'.$gpdid.' atendem as condiï¿½ï¿½es necessï¿½rias para a Tramitaï¿½ï¿½o do Grupo.\nEstado atual do Grupo: Em Supervisï¿½o(Empresa)\nO Grupo serï¿½ Tramitado para o Estado: Em Avaliaï¿½ï¿½o da Supervisï¿½o (MEC)")</script>';
				   $arDado = array("gpdid" => $gpdid );
				    //Se as Obras estiverem em situaï¿½ï¿½es diferentes de:"Em Supervisï¿½o(OBREMSUPERVISAOIND-240)" , 
			 		// "Em Ajuste de supervisï¿½o(Empresa)(OBRAAJUSTESUPERVISAO_EMPRESA-242)" e
			 		// "Em Reajuste da supervisï¿½o(Empresa)(OBRAREAJUSTESUPERVISAO_EMPRESA-279)". Entï¿½o o Estado 
			 		// serï¿½ alterado para "Em Avaliaï¿½ï¿½o da Supervisï¿½o(MEC)(OBREMAVALIASUPERVMEC-171)"
				   wf_alterarEstado( $docid, 375, "", $arDado );
				break;	
			}
		}else{
			//Os Estados em que as Obras do Grupo nï¿½o podem estar para a Tramitaï¿½ï¿½o do Grupo.
			$estadoObrasNoGrupoEmAvlSupMEC = array( OBRAAVALIACAOSUPERVISAO_MEC, OBRAREAVALIACAOSUPERVISAO_MEC, OBRASUPERVISAOAPROVADAOBRA );	
			//Verificaï¿½ï¿½o da Intersecï¿½ï¿½o entre as situaï¿½ï¿½es em que as Obras nï¿½o podem estar.
			if( count(array_intersect($estadoObras, $estadoObrasNoGrupoEmAvlSupMEC)) == 0 ){		
				//Recupera o Documneto(docid) do Grupo.		
				$docid = obrPegarDocid( $gpdid );
				//Recupera o Estado atual do Grupo.
				$estado = wf_pegarEstadoAtual( $docid );
				//Verifica o Estado atual do Grupo.
				switch ((integer)$estado['esdid']){
					//Caso o Estado atual do Grupo seja "Em Avaliaï¿½ï¿½o da Supervisï¿½o(MEC)".
					case (integer)$estado['esdid'] == OBREMAVALIASUPERVMEC: 
					   echo '<script> alert("As Obras do Grupo nï¿½:'.$gpdid.' atendem as condiï¿½ï¿½es necessï¿½rias para a Tramitaï¿½ï¿½o do Grupo.\nEstado atual do Grupo: Em Avaliaï¿½ï¿½o da Supervisï¿½o (MEC)\nO Grupo serï¿½ Tramitado para o Estado:  Em Supervisï¿½o(Empresa) ")</script>';
					   $arDado = array("gpdid" => $gpdid );
					    //Se as Obras estiverem em situaï¿½ï¿½es diferentes de:"Em Avaliaï¿½ï¿½o da Supervisï¿½o(MEC)(OBRAAVALIACAOSUPERVISAO_MEC-241)" , 
				 		// "Em reavaliaï¿½ï¿½o da Supervisï¿½o(MEC)(OBRAREAVALIACAOSUPERVISAO_MEC-243)" e
				 		// "Supervisï¿½o Aprovada(OBRASUPERVISAOAPROVADA-244)". Entï¿½o o Estado 
				 		// serï¿½ alterado para "Em Supervisï¿½o(OBREMSUPERVISAO-159)"
				 	   wf_alterarEstado( $docid, 376, "", $arDado );
			 	    break;	
				}
			}
		}
			//O Estado em que as Obras do Grupo devem estar para a Tramitaï¿½ï¿½o do Grupo.
			$estadoObrasNoGrupoSupAprovada = array( OBRASUPERVISAOAPROVADAOBRA );	
			//Verificaï¿½ï¿½o da Intersecï¿½ï¿½o entre as situaï¿½ï¿½es em que as Obras devem estar.
			if( count(array_intersect($estadoObras, $estadoObrasNoGrupoSupAprovada)) == count($estadoObras) ){		
				//Recupera o Documneto(docid) do Grupo.
				$docid = obrPegarDocid( $gpdid );
				//Recupera o Estado atual do Grupo.
				$estado = wf_pegarEstadoAtual( $docid );
				//Verifica o Estado atual do Grupo.
				switch ((integer)$estado['esdid']){
					//Caso o Estado atual do Grupo seja "Em Avaliaï¿½ï¿½o Final(SAA)".
					case (integer)$estado['esdid'] == OBREMAVALIASUPERVMEC: 
					   echo '<script> alert("As Obras do Grupo nï¿½:'.$gpdid.' atendem as condiï¿½ï¿½es necessï¿½rias para a Tramitaï¿½ï¿½o do Grupo.\nEstado atual do Grupo: Em Avaliaï¿½ï¿½o da Supervisï¿½o (MEC)\nO Grupo serï¿½ Tramitado para o Estado:  Em Avaliaï¿½ï¿½o Final(SAA) ")</script>';
					   $arDado = array("gpdid" => $gpdid );
					    //Se as Obras estiverem com as situaï¿½ï¿½es iguais ao Estado de:"Supervisï¿½o Aprovada(OBRASUPERVISAOAPROVADAOBRA-244)". 
					    //Entï¿½o o Estado serï¿½ alterado para "Em Avaliaï¿½ï¿½o Final(SAA)(OBRAVALIAFINALSAA-172)"
					   wf_alterarEstado( $docid, 377, "", $arDado );
					break;	
				}
			}
	*/		
	}
}

/**Funï¿½ï¿½o que verifica se o Checklist foi preenchido e se o ï¿½ltimo Parecer estï¿½ Aprovado.
 * 
 */
function verficaChecklistParecerAprovado( $gpdid , $obrid ) {
	
	global $db;
	
	$gpdid  = (int)$gpdid;
	$obrid  = (int)$obrid;
		
	if ( $gpdid == 0  || $obrid == 0 ) return true;
	
	$sql="SELECT 
				mc.mpcsituacao,
				MAX(mpcdtinclusao) 
			FROM
				obras.movparecercklist mc
			LEFT JOIN 
				obras.checklistvistoria chck ON chck.chkid = mc.chkid
			WHERE
				mpcstatus = 'A'
				AND chck.obrid ={$obrid}
				AND mc.mpcsituacao = 't'
				AND chck.chkstatus = 'A'
				AND mc.mpcdtinclusao =( SELECT 
											MAX(mpcdtinclusao) 
										FROM 
											obras.movparecercklist mc1 
										WHERE 
											mc1.chkid = chck.chkid
											AND mc1.mpcstatus = 'A'
											AND mc.mpcsituacao = 't' )
			GROUP BY	
				mpcsituacao	";
	
	$mpcsituacao = $db->pegaLinha($sql);
	
	if(verificaChecklistObrasIndividual( $gpdid , $obrid ) && $mpcsituacao['mpcsituacao'] == 't'){
		return true;
	}else{
		return false;
	}
}

function verificaQuestoesRespondidas($obrid){
	
	global $db;

	$obrid  = (int)$obrid;
	
	if ( !$obrid ) return true;
	
	$sql = "SELECT
				--f.obrid,
				( SELECT
					count(gp.grptitulo) as perguntas_respondidas -- 0 significa que todas foram respondidas
				  FROM
					questionario.grupopergunta gp
				  INNER JOIN questionario.pergunta p USING (grpid)
				  INNER JOIN questionario.questionario q ON q.queid = gp.queid
				  WHERE
					gp.queid = 42 --id do questionï¿½rio
					AND p.perid NOT IN (
			
				(
					-- Perguntas vinculadas a grupos filhos de questionarios e de resposta textual
					SELECT
						p.perid as idpergunta
					FROM
						obras.checklistvistoria cv
					JOIN questionario.questionarioresposta qr USING (qrpid)
					JOIN questionario.questionario q ON q.queid = qr.queid
					JOIN questionario.grupopergunta gp ON gp.queid = q.queid
					JOIN questionario.pergunta p ON p.grpid = gp.grpid
					JOIN questionario.resposta r ON r.perid = p.perid AND r.qrpid = qr.qrpid AND r.itpid IS NULL
					WHERE
						chkstatus = 'A' AND qr.qrpid = f.qrpid
				)UNION ALL(
					-- Perguntas vinculadas a grupos filhos de questionarios e que possuem item como resposta
					SELECT
						p.perid as idpergunta
					FROM
						obras.checklistvistoria cv
					JOIN questionario.questionarioresposta qr USING (qrpid)
					JOIN questionario.questionario q ON q.queid = qr.queid
					JOIN questionario.grupopergunta gp ON gp.queid = q.queid
					JOIN questionario.pergunta p ON p.grpid = gp.grpid
					JOIN questionario.itempergunta ip ON ip.perid = p.perid
					JOIN questionario.resposta r ON r.perid = p.perid AND r.qrpid = qr.qrpid AND r.itpid = ip.itpid
					WHERE
						chkstatus = 'A' AND qr.qrpid = f.qrpid
				)UNION ALL(
					-- Perguntas de resposta textual filhas de itens de perguntas vinculadas a grupos filhos de questionarios
					SELECT
						p.perid as idpergunta
					FROM
						obras.checklistvistoria cv
					JOIN questionario.questionarioresposta qr USING (qrpid)
					JOIN questionario.questionario q ON q.queid = qr.queid
					JOIN questionario.grupopergunta gp ON gp.queid = q.queid
					JOIN questionario.pergunta p ON p.grpid = gp.grpid
					JOIN questionario.itempergunta ip ON ip.perid = p.perid
					JOIN questionario.pergunta p1 ON p1.itpid = ip.itpid
					JOIN questionario.resposta r ON r.perid = p1.perid AND r.qrpid = qr.qrpid AND r.itpid IS NULL
					WHERE
						chkstatus = 'A' AND qr.qrpid = f.qrpid
				)UNION ALL(
					-- Perguntas de resposta textual filhas de grupos filhos de itens de perguntas vinculadas a grupos filhos de questionarios
					SELECT
						p.perid as idpergunta
					FROM
						obras.checklistvistoria cv
					JOIN questionario.questionarioresposta qr USING (qrpid)
					JOIN questionario.questionario q ON q.queid = qr.queid
					JOIN questionario.grupopergunta gp ON gp.queid = q.queid
					JOIN questionario.pergunta p ON p.grpid = gp.grpid
					JOIN questionario.itempergunta ip ON ip.perid = p.perid
					JOIN questionario.grupopergunta gp1 ON gp1.itpid = ip.itpid
					JOIN questionario.pergunta p1 ON p1.grpid = gp1.grpid
					JOIN questionario.resposta r ON r.perid = p1.perid AND r.qrpid = qr.qrpid AND r.itpid IS NULL
					WHERE
						chkstatus = 'A' AND qr.qrpid = f.qrpid
				)UNION ALL(
					-- Perguntas de resposta textual filhas de grupos vinculadas a grupos filhos de questionarios
					SELECT
						p.perid as idpergunta
					FROM
						obras.checklistvistoria cv
					JOIN questionario.questionarioresposta qr USING (qrpid)
					JOIN questionario.questionario q ON q.queid = qr.queid
					JOIN questionario.grupopergunta gp ON gp.queid = q.queid
					JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
					JOIN questionario.pergunta p ON p.grpid = gp1.grpid
					JOIN questionario.resposta r ON r.perid = p.perid AND r.qrpid = qr.qrpid AND r.itpid IS NULL
					WHERE
						chkstatus = 'A' AND qr.qrpid = f.qrpid
				)UNION ALL(
					-- Perguntas de resposta por itens filhas de grupos vinculadas a grupos filhos de questionarios
					SELECT
						p.perid as idpergunta
					FROM
						obras.checklistvistoria cv
					JOIN questionario.questionarioresposta qr USING (qrpid)
					JOIN questionario.questionario q ON q.queid = qr.queid
					JOIN questionario.grupopergunta gp ON gp.queid = q.queid
					JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
					JOIN questionario.pergunta p ON p.grpid = gp1.grpid
					JOIN questionario.itempergunta ip ON ip.perid = p.perid
					JOIN questionario.resposta r ON r.perid = p.perid AND r.itpid = ip.itpid AND r.qrpid = qr.qrpid AND r.itpid IS NOT NULL
					WHERE
						chkstatus = 'A' AND qr.qrpid = f.qrpid
				)UNION ALL(
					-- Perguntas de resposta textual filhas de itens de perguntas filhas de grupos vinculadas a grupos filhos de questionarios
					SELECT
						p.perid as idpergunta
					FROM
						obras.checklistvistoria cv
					JOIN questionario.questionarioresposta qr USING (qrpid)
					JOIN questionario.questionario q ON q.queid = qr.queid
					JOIN questionario.grupopergunta gp ON gp.queid = q.queid
					JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
					JOIN questionario.pergunta p ON p.grpid = gp1.grpid
					JOIN questionario.itempergunta ip ON ip.perid = p.perid
					JOIN questionario.pergunta p1 ON p1.itpid = ip.itpid
					JOIN questionario.resposta r ON r.perid = p1.perid AND r.qrpid = qr.qrpid AND r.itpid IS NULL
					WHERE
						chkstatus = 'A' AND qr.qrpid = f.qrpid
				)UNION ALL(
					-- Perguntas de resposta textual filhas de grupos filhos de itens de perguntas filhas de grupos vinculadas a grupos filhos de questionarios
					SELECT
						p.perid as idpergunta
					FROM
						obras.checklistvistoria cv
					JOIN questionario.questionarioresposta qr USING (qrpid)
					JOIN questionario.questionario q ON q.queid = qr.queid
					JOIN questionario.grupopergunta gp ON gp.queid = q.queid
					JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
					JOIN questionario.pergunta p ON p.grpid = gp1.grpid
					JOIN questionario.itempergunta ip ON ip.perid = p.perid
					JOIN questionario.grupopergunta gp2 ON gp2.itpid = ip.itpid
					JOIN questionario.pergunta p1 ON p1.grpid = gp2.grpid
					JOIN questionario.resposta r ON r.perid = p1.perid AND r.qrpid = qr.qrpid AND r.itpid IS NULL
					WHERE
						chkstatus = 'A' AND qr.qrpid = f.qrpid
				)
			)) AS questionario -------------------------------ï¿½ltimo Campo------------------------------------
			FROM
				((
				SELECT DISTINCT
					oi.obrid, 
					cv.qrpid
				FROM
					obras.grupodistribuicao gd
				JOIN obras.itemgrupo ig USING(gpdid)
				JOIN obras.repositorio r ON r.repid = ig.repid AND r.repstatus = 'A'
				LEFT JOIN obras.checklistvistoria cv ON cv.obrid = r.obrid AND cv.chkstatus = 'A'
				JOIN obras.obrainfraestrutura oi ON oi.obrid = r.obrid
				JOIN obras.orgao og ON oi.orgid = og.orgid
				JOIN entidade.entidade ee ON oi.entidunidade = ee.entid										    	
				WHERE
					r.obrid = {$obrid} AND cv.chkid IS NULL
				)UNION ALL(
				SELECT DISTINCT
					oi.obrid, 
					cv.qrpid
				FROM
					obras.grupodistribuicao gd
				JOIN obras.itemgrupo ig USING(gpdid)
				JOIN obras.repositorio r USING(repid)
				JOIN obras.checklistvistoria cv USING(obrid)
				JOIN questionario.questionarioresposta qr USING(qrpid)
				JOIN questionario.questionario q USING(queid)
				JOIN questionario.grupopergunta gp USING(queid)
				JOIN questionario.pergunta p USING (grpid)
				JOIN obras.obrainfraestrutura oi ON oi.obrid = r.obrid	
				JOIN obras.orgao og ON oi.orgid = og.orgid
				JOIN entidade.entidade ee ON oi.entidunidade = ee.entid
				WHERE
					r.obrid = {$obrid} AND cv.chkstatus = 'A'
			)) AS f";
	
	return ($db->pegaUm( $sql ) == 0 ? true : false);
	
}

/**Funï¿½ï¿½o que Recupera o ID da Situaï¿½ï¿½o do Grupo.
 * @param $gpdid
 */
function recuperaSituacaoGrupo($gpdid){
	
	global $db;

	if(!$gpdid)return true;
	
	$sql = " SELECT 
				wd.esdid
			 FROM 
			 	obras.grupodistribuicao gd
			 INNER JOIN 
			 	workflow.documento wd ON wd.docid = gd.docid
			 INNER JOIN 
			 	workflow.estadodocumento we ON wd.esdid = we.esdid 
			 								AND we.esdstatus = 'A'
			 INNER JOIN 
			 	workflow.historicodocumento wh ON wh.docid = gd.docid
			 WHERE 
				gd.gpdstatus = 'A'
				AND gd.gpdid = {$gpdid}
				AND wh.htddata = ( SELECT 
										MAX(whs.htddata) AS datramitacao 
								   FROM 
								   		workflow.historicodocumento whs 
								   WHERE 
								   		whs.docid = gd.docid
						   		 )";
	
	$situacaoGrupo = $db->pegaUm($sql);
	
	return $situacaoGrupo;
								   
}


/**Funï¿½ï¿½o que Recupera o ID da Situaï¿½ï¿½o da Obra.
 * @param $obrid
 */
function recuperaSituacaoObra($obrid){
	
	global $db;

	if(!$obrid)return true;
	
	$sql = " SELECT 
					we.esdid 
			 FROM 
					obras.obrainfraestrutura oi
			INNER JOIN 
					workflow.documento wd ON wd.docid = oi.docid
			INNER JOIN 
					workflow.estadodocumento we ON wd.esdid = we.esdid 
											    AND we.esdstatus = 'A'
			INNER JOIN 
					workflow.historicodocumento wh ON wh.docid = oi.docid
			WHERE 
				oi.obrid = {$obrid}
				AND oi.obsstatus = 'A'
				AND wh.htddata = ( SELECT 
										MAX(whs.htddata) AS ultima_data_tramitacao 
								   FROM 
										workflow.historicodocumento whs 
								   WHERE 
										whs.docid = oi.docid
						 		 )";
	
	$situacaoObra = $db->pegaUm($sql);
	
	return $situacaoObra;
								   
}

/**Funï¿½ï¿½o que retorna a soma total dos Valores dos Serviï¿½os do Cronograma Fï¿½sico-Financeiro. 
 * @param  $obrid
 * @return $valorTotalCronograma
 */
function recuperaValorTotalCronograma($obrid, $arrParam = null){
	
	global $db;
	
	if(!$obrid)return true;
	
	if ( $arrParam['traid'] != '' ){
		$whereTraid = "AND traid = ".$arrParam['traid'];	
	}
	
	$sql = "SELECT 
				SUM(i.icovlritem)
			FROM 
				obras.itenscomposicaoobra i,
				obras.itenscomposicao ic 
			WHERE 
				i.obrid = {$obrid} 
				and i.itcid = ic.itcid 
				and i.icostatus='A' 
				AND i.icovigente = 'A'
				" . $whereTraid . "
			GROUP BY
				i.obrid";
	
	$valorTotalCronograma = $db->pegaUm($sql);
	
	return $valorTotalCronograma;
	
}

/**Funï¿½ï¿½o que retorna o Valor do Contrato da Obra.  
 * @param  $obrid
 * @return $valorTotalContrato
 */
function recuperaValorTotalContrato($obrid, $arrParam = null){
	
	global $db;
	
	if(!$obrid)return true;
	
	$sql = " SELECT DISTINCT 
					obrcustocontrato 
			 FROM 
					obras.obrainfraestrutura oi 
			 WHERE 
					oi.obrid = {$obrid}
					AND oi.obsstatus = 'A' ";
	
	$valorTotalContrato = $db->pegaUm($sql);
	//Se a Obra possuir Aditivo, retornarï¿½ o Valor Total do Contrato com o Aditivo,
	//senï¿½o retornarï¿½ apenas o Valor Total do Contrato.
	if($arrParam["traid"]  != null  || $arrParam["traseq"] != null ){
		$valorTotalContratoComAditivo = pegaObMaiorVlrAditivo( $arrParam );
		if($valorTotalContratoComAditivo != 0 ){
			//Retorna o Valor Total do Contrato com Aditivo. Tipo:"Valor" ou "Prazo/Valor"
			return $valorTotalContratoComAditivo;
		}else{
			//Retorna o Valor Total do Contrato com Aditivo. Tipo:"Prazo"
			return $valorTotalContrato;
		}
	}else{
		//Retorna o Valor Total do Contrato.	
		return $valorTotalContrato;
	}
}

function atualizaPorcentagemObra($obrid, $supvid){
	global $db; 
	//busca o percentual
	$sql = "SELECT 
				COALESCE(sup.supvlrinfsupervisor, sup2.supvlrinfsupervisor) AS supvlrinfsupervisor,
				ico.icopercsobreobra
			FROM 
				obras.supervisaoitenscomposicao sup
			INNER JOIN
				obras.itenscomposicaoobra ico ON ico.icoid = sup.icoid
			LEFT JOIN (
				SELECT
					itcid,
					sic.supvlrinfsupervisor
				FROM
					obras.supervisaoitenscomposicao sic
				JOIN obras.itenscomposicaoobra ico ON ico.icoid = sic.icoid
													  AND ico.obrid = {$obrid}	
				WHERE
					supvid = (SELECT MAX(supvid) FROM obras.supervisao WHERE supstatus = 'A' AND obrid = {$obrid})
			) sup2 ON sup2.itcid = ico.itcid 
			WHERE 
				supvid = {$supvid}";
	
	$rs = $db->carregar($sql);
	$total_execsobreobra = 0;
	foreach ($rs as $linha){
		// Pega os valores reais
		$supervisao                 = ( isset($linha["supvlrinfsupervisor"]) ) 			   ? $linha['supvlrinfsupervisor']			   : 0;
		$perc_sobre_obra            = ( isset($linha["icopercsobreobra"]) ) 			   ? $linha['icopercsobreobra']				   : 0;
		
		// Valores do % do item executado sobre a obra
		$supervisao_exec_sobre_obra = ( ((float)$supervisao * (float)$perc_sobre_obra) / 100 );
		$total = $total + (float)$supervisao_exec_sobre_obra;
	}
	
	//insere o percentual
	$sql = "UPDATE
				obras.obrainfraestrutura
			SET
				obrpercexec    = {$total}
			   ,obrsuppercexec = {$total}	
			WHERE
				obrid = {$obrid}";
						
 	$db->executar($sql);
 	
 	return true;
}

function atualizaPorcentagemValidaObra($obrid){
	global $db; 
	//busca o percentual
	$sql = "SELECT 
				COALESCE(sup.supvlrinfsupervisor, sup2.supvlrinfsupervisor) AS supvlrinfsupervisor,
				ico.icopercsobreobra,
				sup.supvid
			FROM 
				obras.supervisaoitenscomposicao sup
			INNER JOIN
				obras.itenscomposicaoobra ico ON ico.icoid = sup.icoid
			LEFT JOIN (
				SELECT
					itcid,
					sic.supvlrinfsupervisor
				FROM
					obras.supervisaoitenscomposicao sic
				JOIN obras.itenscomposicaoobra ico ON ico.icoid = sic.icoid
													  AND ico.obrid = {$obrid}	
				WHERE
					supvid = (SELECT MAX(supvid) FROM obras.supervisao WHERE supstatus = 'A' AND obrid = {$obrid})
			) sup2 ON sup2.itcid = ico.itcid 
			WHERE 
				sup.supvid = (
						SELECT 
							supv.supvid
						FROM
							obras.supervisao supv
						INNER JOIN
							workflow.documento doc ON supv.docid = doc.docid AND doc.esdid = ".WF_ESTADO_VALIDADO."
						WHERE 
							supv.obrid = {$obrid}
						ORDER BY
							supv.supvdt DESC
						LIMIT 
							1)";
	$rs = $db->carregar($sql);
	$total = 0;
	if($rs)
		foreach ($rs as $linha){
			// Pega os valores reais
			$supervisao                 = ( isset($linha["supvlrinfsupervisor"]) ) 			   ? $linha['supvlrinfsupervisor']			   : 0;
			$perc_sobre_obra            = ( isset($linha["icopercsobreobra"]) ) 			   ? $linha['icopercsobreobra']				   : 0;
			
			// Valores do % do item executado sobre a obra
			$supervisao_exec_sobre_obra = ( ((float)$supervisao * (float)$perc_sobre_obra) / 100 );
			$total = $total + (float)$supervisao_exec_sobre_obra;
		}
	else
		$total = 0.00;
	
	//insere o percentual
	$sql = "UPDATE
				obras.obrainfraestrutura
			SET
				obrpercexec    = {$total}
			   ,obrsuppercexec = {$total}	
			WHERE
				obrid = {$obrid}";
						
 	$db->executar($sql);
 	
 	return true;
}

function pegaResponsabilidades($usucpf = ''){
	global $db;
	$arrResponsabilidades = array();
	$usucpf = $usucpf ? $usucpf : $_SESSION['usucpf'];
	$perfis = implode(" ',' ", pegaPerfilArray($usucpf));
	
	$sql = "SELECT
				entid,
				orgid,
				obrid
			FROM 
				obras.usuarioresponsabilidade
			WHERE
				usucpf = '{$usucpf}' 
			    AND pflcod IN ('{$perfis}')
			    AND rpustatus = 'A';";
	$arrResponsabilidade = $db->carregar($sql);
	if($arrResponsabilidade)
		foreach ($arrResponsabilidade as $index=>$responsabilidade) 
			foreach ($responsabilidade as $index2=>$value2)
				if(!is_null($value2))
					$arrResponsabilidades[$index2][] = $value2;
	
	return $arrResponsabilidades;
}

/*
 * function pegaResponsabilidaPorPerfil()
 * Tipos de retorno:
 * TRUE  - Usuário pode ver tudo;
 * ARRAY - Quais as responsabilidades do usuário;
 * FALSE - Usuário possui restrição de responsabilidade, porém não possui nenhuma. 
 */
function pegaResponsabilidadePorPerfil($usucpf = ''){
	global $db;
	$usucpf = $usucpf ? $usucpf : $_SESSION['usucpf'];
	$perfis = pegaPerfilArray($usucpf);
	
	if(in_array(PERFIL_SAMPR       , $perfis) 
	|| in_array(PERFIL_CONSULTAPR  , $perfis)
	|| in_array(PERFIL_SUPERUSUARIO, $perfis)){
		$responsabilidades = true;
	}else{
		$responsabilidades = pegaResponsabilidades($usucpf);
		$responsabilidades = $responsabilidades ? $responsabilidades : false;
	}
	
	return $responsabilidades;
}
?>
