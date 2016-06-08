<?php

function obras_monta_agp_relatorio2(){
	
	$agrupador = array('nomedaobra');
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array()
				);
	
	foreach ( $_REQUEST['colunas'] as $valor ) {
		$agp['agrupadoColuna'][] = $valor;				
	}
				
	foreach ( $agrupador as $val ){
		switch( $val ){
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
		}	
	}
	
	return $agp;
	
}

/**
 * Funçao que monta o sql para trazer o relatório geral de obras
 *
 * @author Fernando A. Bagno da Silva
 * @since 20/02/2009
 * @return string
 */
function obras_monta_sql_relatorio2(){
	
	$where = array();
	
	extract($_REQUEST);
	
	$selectTerritorios = "territorios.municipio ";
	
	// tipo de ensino
	if( $orgid ){
		array_push($where, " oi.orgid in (" . implode( ',', $orgid ) . ") ");
	}
	
	// região
	if( $regiao[0] && $regiao_campo_flag ){
		array_push($where, " re.regcod " . (!$regiao_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $regiao ) . "') ");
	}
	
	// mesoregião
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
		array_push($where, " tm.gtmid " . (!$grupomun_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $grupomun ) . "') ");
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
	
	// classificação da obra
	if( $cloid[0] && $cloid_campo_flag ){
		array_push($where, " oi.cloid " . (!$cloid_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $cloid ) . ") ");
	}
	
	// situação da obra
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
	}
	
	// Filtro de responsável pela vistoria
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

	
	// monta o sql 
	$sql = "SELECT DISTINCT
				oi.obrid || ' ' as obridid,
				'(' || oi.obrid || ') ' || oi.obrdesc as nomedaobra,
				oi.obrqtdconstruida || ' ' || COALESCE(und.umdeesc,'') as metragem,
				CASE WHEN oi.obrvalorprevisto is null OR oi.obrvalorprevisto = 0
					THEN ' - '
					ELSE to_char(oi.obrvalorprevisto, '999G999G999G999D99')
				END as obrvalorprevisto,
				CASE WHEN oi.obrcustocontrato is null OR oi.obrcustocontrato = 0
					THEN ' - '
					ELSE to_char(oi.obrcustocontrato, '999G999G999G999D99')
				END as obrcustocontrato,
				oi.obrpercexec||'%' as porcexecucao,
				to_char(date(oi.obrdtassinaturacontrato), 'DD/MM/YYYY') as obrdtassinaturacontrato,
				to_char(date(oi.dtterminocontrato), 'DD/MM/YYYY') as dtterminocontrato,
				to_char(date(oi.obrdtordemservico), 'DD/MM/YYYY') as obrdtordemservico,
				to_char(date(oi.obrdtinicio), 'DD/MM/YYYY') as obrdtinicio,
				to_char(date(oi.obrdttermino), 'DD/MM/YYYY') as obrdttermino,
				to_char(oi.obrdtinicio, 'DD/MM/YYYY') as datainicio,
				to_char(oi.obrdttermino, 'DD/MM/YYYY') as datafim,
				to_char(oi.obrdtvistoria, 'DD/MM/YYYY HH24:MI') as ultatualizacao,
				CASE WHEN ( SELECT COUNT(rstoid) FROM obras.restricaoobra WHERE obrid = oi.obrid ) = 0 
					THEN 'Não' 
					ELSE 'Sim' 
				END as qtdrestricoes,
				( SELECT COUNT(DISTINCT supv.supvid) FROM obras.supervisao supv WHERE supv.obrid = oi.obrid AND supstatus = 'A') as qtdvistorias,
				tai.aqidsc as tipoaquisicao,
				st.stodesc as situacaoobra,
				tm2.muncod as muncod,
				tm2.mundescricao as municipio,
				et.estuf as estado,
				ee.entnome as unidade,
				ee2.entnome as campus,
				ee3.entnumcpfcnpj || ' ' as cnpjempresacontratada,
				ee3.entnome as nomeempresacontratada,
				tpo.tobadesc as tipoobra,
				pf.prfdesc as subacao,
				org.orgdesc as orgao,
				CASE WHEN mol.moldsc is null
					THEN 'Não informado'
					ELSE moldsc
				END as moldsc,
				CASE WHEN licitacaouasg is null
					THEN 'Não informado'
					ELSE licitacaouasg
				END as licitacaouasg,
				CASE WHEN numlicitacao is null
					THEN 'Não informado'
					ELSE numlicitacao
				END as numlicitacao,
				CASE WHEN frpdesc is null
					THEN 'Não informado'
					ELSE frpdesc
				END  as frpdesc,
				to_char((SELECT 
							date(fl.flcpubleditaldtprev)
						 FROM 
						 	obras.faselicitacao fl
						 LEFT JOIN obras.tiposfaseslicitacao tfl ON tfl.tflid = fl.tflid
						 WHERE 
						 	-- fl.tflid = '2'AND fl.obrid = oi.obrid AND fl.flcstatus='A' ORDER BY flcid DESC LIMIT 1
						 	fl.obrid = oi.obrid AND fl.flcstatus = 'A' AND tfl.tflordem = 1 ORDER BY flcid DESC LIMIT 1
				), 'DD/MM/YYYY') as dtpublicacao,
				to_char((SELECT 
							date(fl.flchomlicdtprev)
						 FROM obras.faselicitacao fl
						 LEFT JOIN obras.tiposfaseslicitacao tfl ON tfl.tflid = fl.tflid
						 WHERE --fl.tflid = '9'AND fl.obrid = oi.obrid AND fl.flcstatus='A' ORDER BY flcid DESC LIMIT 1
						 	fl.obrid = oi.obrid AND fl.flcstatus = 'A' AND tfl.tflordem = 4 ORDER BY flcid DESC LIMIT 1	
				), 'DD/MM/YYYY') as dthomologacao,
				CASE WHEN of.covid is not null 
					THEN oc.covnumero || ' ' 
					ELSE oi.numconvenio || ' ' 
				END as nr_convenio,
	            to_char(oc.covdtinicio, 'DD/MM/YYYY') as dt_inicio_conv,
	            to_char(oc.covdtfinal, 'DD/MM/YYYY') as dt_final_conv,
	            SUM(CASE WHEN ieo.eocvlrliquidado is null OR ieo.eocvlrliquidado = 0
					THEN 0.00
					ELSE ieo.eocvlrliquidado
				END) as vlrliquidado,
				CASE WHEN ed.medlatitude = '' and ed.medlongitude = '' 
					THEN ''
					ELSE 'Lat.: ' || ed.medlatitude || '<br>Long.: ' || ed.medlongitude 
				END as coordenadas_geograficas,
				SUBSTRING(ed.endcep, 1, 5) || '-' || SUBSTRING(ed.endcep, 6, 8) as cep, 
				ed.endlog, 
				ed.endcom, 
				ed.endbai, 
				ed.endnum, 
				ed.endcomunidade,
				(SELECT DISTINCT
					count(os.orsid) as qtd
				FROM
					obras.obrainfraestrutura obr
				INNER JOIN
					obras.repositorio ore ON ore.obrid = obr.obrid
				INNER JOIN
					obras.itemgrupo ig ON ig.repid = ore.repid
				INNER JOIN
					obras.grupodistribuicao gd ON gd.gpdid = ig.gpdid
				INNER JOIN
					obras.ordemservico os ON os.gpdid = gd.gpdid --ordem de serviço
				INNER JOIN
					workflow.documento wd ON wd.docid = gd.docid
				INNER JOIN
					workflow.estadodocumento we ON we.esdid = wd.esdid
				WHERE
					os.orsstatus = 'A' 
					AND obr.obsstatus = 'A' 
					AND obr.obrid = oi.obrid 
					AND ore.repstatus = 'A'
					AND we.esdid = ". OBRSUPFINALIZADA .") as obrsupervisoes    
			FROM
				obras.obrainfraestrutura oi
			INNER JOIN 
				entidade.endereco ed 				ON oi.endid = ed.endid
			LEFT JOIN 
				obras.faselicitacao fl 				ON fl.obrid = oi.obrid
			LEFT JOIN 
				obras.tiposfaseslicitacao tfl 		ON tfl.tflid = fl.tflid
			LEFT JOIN 
				obras.infraestrutura inf 			ON inf.iexid = oi.iexid
			LEFT JOIN 
				obras.formarepasserecursos frr 		ON frr.obrid = oi.obrid
			LEFT JOIN 
				obras.tipoformarepasserecursos tfrr ON tfrr.frpid = frr.frpid
			LEFT JOIN 
				obras.modalidadelicitacao mol 		ON mol.molid = oi.molid
			LEFT JOIN 
				obras.tipoaquisicaoimovel tai 		ON tai.aqiid = inf.aqiid
			LEFT JOIN
				obras.situacaoobra st 	   			ON oi.stoid = st.stoid
			LEFT JOIN
				obras.tipoobra tpo 		   			ON tpo.tobaid = oi.tobraid
			LEFT JOIN
				obras.orgao org 		   			ON org.orgid = oi.orgid 
			LEFT JOIN 
				obras.unidademedida und     		ON und.umdid = oi.umdidobraconstruida
			LEFT JOIN 
				obras.execucaoorcamentaria eo       ON eo.obrid = oi.obrid
			LEFT JOIN 
				obras.itensexecucaoorcamentaria ieo ON ieo.eorid = eo.eorid
			LEFT JOIN 
				territorios.estado et 	   			ON ed.estuf = et.estuf
			LEFT JOIN 
				territorios.regiao re 	   			ON re.regcod = et.regcod
			LEFT JOIN 
				territorios.municipio tm2  			ON tm2.muncod = ed.muncod
			LEFT JOIN 
				territorios.mesoregiao me  			ON me.mescod = tm2.mescod
			LEFT JOIN 
				{$selectTerritorios} tm    			ON tm.muncod = ed.muncod
			INNER JOIN 
				entidade.entidade ee 	   			ON oi.entidunidade = ee.entid
			LEFT JOIN 
				territorios.pais pa 	   			ON pa.paiid = re.paiid
			LEFT JOIN 
				entidade.entidade ee2 	   			ON oi.entidcampus = ee2.entid
			LEFT JOIN 
				entidade.funcaoentidade ef 			ON ee2.entid = ef.entid AND ef.funid IN( 17,18 )
			LEFT JOIN
				entidade.entidade ee3 	   			ON oi.entidempresaconstrutora = ee3.entid
			LEFT JOIN 
				obras.programafonte pf     			ON oi.prfid = pf.prfid
			LEFT JOIN
				obras.classificacaoobra cl 			ON oi.cloid = cl.cloid
			LEFT JOIN
				obras.tipologiaobra tp    			ON oi.tpoid = tp.tpoid
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
			LEFT JOIN
	            obras.formarepasserecursos of ON of.obrid = oi.obrid
			LEFT JOIN
	            obras.conveniosobra  oc ON oc.covid = of.covid
			WHERE
			--	oi.obrid = 1484 AND 
				oi.obsstatus = 'A' " . ( is_array($where) ? ' AND' . implode(' AND ', $where) : '' ) 
				. $stFiltro."
			GROUP BY
				oi.obrid,oi.obrdesc,oi.obrqtdconstruida,und.umdeesc,oi.obrvalorprevisto,oi.obrcustocontrato,oi.obrpercexec,
				oi.obrdtassinaturacontrato,oi.dtterminocontrato,oi.obrdtordemservico,oi.obrdtinicio,oi.obrdttermino,oi.obrdtvistoria,
				tai.aqidsc,st.stodesc,tm2.muncod,tm2.mundescricao,et.estuf,ee.entnome,ee2.entnome,ee3.entnumcpfcnpj,ee3.entnome,
				tpo.tobadesc,pf.prfdesc,org.orgdesc,mol.moldsc,oi.licitacaouasg,oi.numlicitacao,tfrr.frpdesc,of.covid,oc.covnumero,
				oi.numconvenio,oc.covdtinicio,oc.covdtfinal,ed.medlatitude,ed.medlongitude,ed.endcep,ed.endlog,ed.endcom,ed.endbai,
				ed.endnum,ed.endcomunidade";
//			ver($sql, d);
	return $sql;
	
}


ini_set("memory_limit", "1024M");

// salva os POST na tabela
if ( $_REQUEST['salvar'] == 1 ){
	$existe_rel = 0;
	$sql = sprintf(
		"select prtid from public.parametros_tela where prtdsc = '%s'",
		$_REQUEST['titulo']
	);
	$existe_rel = $db->pegaUm( $sql );
	if ($existe_rel > 0) 
	{
		$sql = sprintf(
			"UPDATE public.parametros_tela SET prtdsc = '%s', prtobj = '%s', prtpublico = 'FALSE', usucpf = '%s', mnuid = %d WHERE prtid = %d",
			$_REQUEST['titulo'],
			addslashes( addslashes( serialize( $_REQUEST ) ) ),
			$_SESSION['usucpf'],
			$_SESSION['mnuid'],
			$existe_rel
		);
		$db->executar( $sql );
		$db->commit();
	}
	else 
	{
		$sql = sprintf(
			"INSERT INTO public.parametros_tela ( prtdsc, prtobj, prtpublico, usucpf, mnuid ) VALUES ( '%s', '%s', %s, '%s', %d )",
			$_REQUEST['titulo'],
			addslashes( addslashes( serialize( $_REQUEST ) ) ),
			'FALSE',
			$_SESSION['usucpf'],
			$_SESSION['mnuid']
		);
		$db->executar( $sql );
		$db->commit();
	}
	?>
	<script type="text/javascript">
		alert('Operação realizada com sucesso!');
		location.href = '?modulo=<?= $modulo ?>&acao=A';
	</script>
	<?
	die;
}

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */


// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

// instancia a classe de relatório
$rel = new montaRelatorio();

// monta o sql, agrupador e coluna do relatório
$sql       = obras_monta_sql_relatorio2(); //dbg($sql,1);
$agrupador = obras_monta_agp_relatorio2();
$coluna    = obras_monta_coluna_relatorio2();
$dados 	   = $db->carregar( $sql );

$rel->setAgrupador($agrupador, $dados); 
$rel->setColuna($coluna);
$rel->setTolizadorLinha(false);

// Gera o XLS do relatório
if ( $_REQUEST['pesquisa'] == '2' ){
	ob_clean();
    $nomeDoArquivoXls = 'relatorio';
    echo $rel->getRelatorioXls();
    die;
}

?>
<html>
	<head>
		<title><?php echo $GLOBALS['parametros_sistema_tela']['nome_e_orgao']; ?></title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
	</head>
	<body>
		<center>
			<!--  Cabeçalho Brasão -->
			<?php echo monta_cabecalho_relatorio( '95' ); ?>
		</center>
		
		<!--  Monta o Relatório -->
		<? echo $rel->getRelatorio(); ?>
		
	</body>
</html>
