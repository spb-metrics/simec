<?php
ini_set("memory_limit","128M");
$supervisao = new supervisao();

switch( $_REQUEST["requisicao"] ){
	
	case "lista":

		$where = $_REQUEST["estuf"] ? " AND ed.estuf = '{$_REQUEST["estuf"]}'" : "";
		$where .= $_REQUEST["entidunidade"] ? " AND entidunidade = {$_REQUEST["entidunidade"]}" : "";  
		
		
		if ( ($_REQUEST["vlrmenor"]) != '' && !empty($_REQUEST["vlrmaior"]) != '' ) {
			$where .= " AND (oi.obrvlrrealobra BETWEEN {$_REQUEST["vlrmenor"]} AND {$_REQUEST["vlrmaior"]}) ";
		}
		
		if ( $_REQUEST['supervisao'] == 'S' ) {
			$where .= " AND oi.obrsupemp = TRUE ";
			
		}elseif ( $_REQUEST['supervisao'] == 'N' ) {
			$where .= " AND oi.obrsupemp = FALSE ";
			
		}
		
		if( !empty( $_REQUEST["tobaid"] ) ){
			
			$where .= " AND oi.tobraid = {$_REQUEST["tobaid"]} ";
			
		}
		 
		if( !empty( $_REQUEST["stoid"] ) ){
			
			$where .= " AND oi.stoid = {$_REQUEST["stoid"]} ";
			
		}
		 
		if( !empty( $_REQUEST["cloid"] ) ){
			
			$where .= " AND oi.cloid = {$_REQUEST["cloid"]} ";
			
		}
		 
		if( !empty( $_REQUEST["prfid"] ) ){
			
			$where .= " AND oi.prfid = {$_REQUEST["prfid"]} ";
			
		}
		 
		/*if( !empty( $_REQUEST["entidunidade2"] ) ){
			
			$where .= " AND oi.entidunidade = {$_REQUEST["entidunidade2"]} ";
			
		}*/
		 
		if( !empty( $_REQUEST["obrtextobusca"] ) ){
			
			$where .= " AND ( upper(oi.obrdesc) ilike upper('%{$_REQUEST["obrtextobusca"]}%') OR ";
			$where .= " upper(ee.entnome) ilike upper('%{$_REQUEST["obrtextobusca"]}%') OR ";
			$where .= " upper(tm.mundescricao) ilike upper('%{$_REQUEST["obrtextobusca"]}%') OR ";
			$where .= " upper(mpi.plicod) ilike upper('%{$_REQUEST["obrtextobusca"]}%') OR ";
			$where .= " upper(oi.obrdesc) ilike upper('%{$_REQUEST["obrtextobusca"]}%') OR ";
			$where .= " oi.obrid =".(int)$_REQUEST["obrtextobusca"]." ) "; // busca pelo campo ID
			
		}
		 
		switch( $_REQUEST["foto"] ){
			
			case "S":
				$where .= " AND af.obrid IS NOT NULL ";
			break;
				
			case "N":
				$where .= " AND af.obrid IS NULL ";
			break;
			
		}
		
		switch( $_REQUEST["vistoria"] ){
			
			case "S":
				$where .= " AND ov.obrid IS NOT NULL ";
			break;
				
			case "N":
				$where .= " AND ov.obrid IS NULL ";
			break;
			
		}
		
		switch( $_REQUEST["restricao"] ){
			
			case "S":
				$where .= " AND re.obrid IS NOT NULL ";
			break;
				
			case "N":
				$where .= " AND re.obrid IS NULL ";
			break;
			
		}
		
		switch( $_REQUEST["planointerno"] ){
			
			case "S":
				$where .= " AND o.obrid IS NOT NULL ";
			break;
				
			case "N":
				$where .= " AND o.obrid IS NULL ";
			break;
			
		}
	
		switch( $_REQUEST["aditivo"] ){
			
			case "S":
				$where .= " AND obridaditivo IS NOT NULL";
			break;
				
			case "N":
				$where .= " AND obridaditivo IS NULL";
			break;
			
		}
		
		if ( $_REQUEST["percentualinicial"] > '0' ) {
			
			$perc = $_REQUEST["percentualfinal"] == 100 ? 110 : $_REQUEST["percentualfinal"];
			$where .= " AND ( total_exec BETWEEN {$_REQUEST["percentualinicial"]} AND {$perc})";
			
		}elseif ($_REQUEST["percentualinicial"] == '0') {
			if ( $_REQUEST["percentualfinal"] > '0' ) {
				
				$perc = $_REQUEST["percentualfinal"] == 100 ? 110 : $_REQUEST["percentualfinal"];
				$where .= " AND ( total_exec IS NULL OR total_exec BETWEEN {$_REQUEST["percentualinicial"]} AND {$perc})";
			
			}elseif ( $_REQUEST["percentualfinal"] == '0' ) {
				
				$where .= " AND ( total_exec = 0 OR total_exec IS NULL )";
				
			}
		}
		
		$sql = "SELECT DISTINCT
					'<center>
						<input type=\"checkbox\" name=\"obrid[' || oi.obrid || ']\" value=\" ' || oi.obrid || '\" id=\"obrid_' || oi.obrid || '\"' || CASE WHEN obrqtdconstruida is not null THEN '' ELSE 'disabled=\"disabled\"' END || ' /></center>' as acao,
					'('|| oi.obrid ||') '|| oi.obrdesc ||'' as nome,
					CASE WHEN oi.entidunidade is not null 
						THEN eu.entnome 
						ELSE 'Não Informado' 
					END AS unidade,
					CASE WHEN oi.entidcampus is not null 
						THEN ee.entnome 
						ELSE 'Não Informado' 
					END as campus,
					mundescricao || ' / ' || ed.estuf,
					CASE WHEN obrqtdconstruida is not null 
						THEN obrqtdconstruida 
						ELSE '0.00' 
					END as areaconstruida,
					stodesc as situacao,
					coalesce(obrpercexec,'0.00'),
					(SELECT 
			                COUNT(ooi.obrid)
			        FROM
			                obras.obrainfraestrutura ooi
			        left JOIN
			                obras.repositorio r ON r.obrid = ooi.obrid 
			        left JOIN
			                obras.itemgrupo oig ON oig.repid = r.repid
			        left JOIN
			                obras.grupodistribuicao ogd ON ogd.gpdid = oig.gpdid 
			        left JOIN
			                workflow.documento wd ON wd.docid = ogd.docid
			        left JOIN
			                workflow.estadodocumento we ON we.esdid = wd.esdid 
			        WHERE
			                ooi.obrid = oi.obrid
			                AND we.esdid = ".OBRSUPFINALIZADA."
			                AND r.repstatus = 'I'
			                AND ogd.gpdstatus = 'A'
			                ) as qtd_supervisao,
	                oi.obrsuppercexec,
	                (SELECT 
	                	stsup.stodesc 
                	 FROM 
                	 	obras.situacaoobra stsup   
                	 WHERE	
                	 	stsup.stoid = oi.stoidsupemp
                 	) AS situacao_ult_sup
				FROM
					obras.obrainfraestrutura oi
				LEFT JOIN
					entidade.entidade ee ON ee.entid = oi.entidcampus
				INNER JOIN
					obras.situacaoobra st ON st.stoid = oi.stoid
				INNER JOIN
					entidade.endereco ed ON ed.endid = oi.endid
				INNER JOIN
					territorios.municipio tm ON tm.muncod = ed.muncod
				INNER JOIN
				    entidade.entidade eu ON eu.entid = oi.entidunidade	 
				LEFT JOIN
					(SELECT 
						SUM(icopercexecutado) as total_exec, 
						obrid 
					FROM 
						obras.itenscomposicaoobra
					WHERE
						icovigente = 'A'	 
					GROUP BY obrid ) pe ON pe.obrid = oi.obrid
				LEFT JOIN
					( 
					SELECT 
						max(aqoid) as foto, 
						max(arq.arqid) as  arqfoto,
						obr.obrid 
					FROM 
						public.arquivo arq
					INNER JOIN 
						obras.arquivosobra oar ON arq.arqid = oar.arqid
					INNER JOIN 
						obras.obrainfraestrutura obr ON obr.obrid = oar.obrid 
					INNER JOIN 
						seguranca.usuario seg ON seg.usucpf = oar.usucpf 
					WHERE 
						aqostatus = 'A' 
						and tpaid = 21 
						and (arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png') 
					GROUP BY obr.obrid 
					) af ON af.obrid = oi.obrid
				LEFT JOIN
					( SELECT max(rstoid) as restricao, obrid FROM obras.restricaoobra WHERE rststatus = 'A' GROUP BY obrid ) re ON re.obrid = oi.obrid
				LEFT JOIN
					( SELECT max(supvid) as supervisao, obrid FROM obras.supervisao WHERE supstatus = 'A' GROUP BY obrid ) ov ON ov.obrid = oi.obrid
				LEFT JOIN
					( SELECT max(mpi.pliid) as pi, a.obrid FROM monitora.pi_obra a INNER JOIN monitora.pi_planointerno mpi ON mpi.pliid = a.pliid WHERE mpi.plistatus = 'A' GROUP BY a.obrid ) o ON o.obrid = oi.obrid 
				LEFT JOIN
					monitora.pi_planointerno mpi ON mpi.pliid = o.pi AND mpi.plistatus = 'A'
				/*LEFT JOIN
					(SELECT
						CASE WHEN (SELECT count(traid) FROM obras.termoaditivo ta WHERE ta.obrid = obra.obrid) > 0 
							THEN (SELECT 
									travlrfinalobra 
								FROM 
									obras.termoaditivo ta 
								WHERE 
									ta.obrid = obra.obrid 
								ORDER BY traid DESC LIMIT 1) 
							ELSE coalesce(obra.obrvalorprevisto,'0') 
						END as vlr,
						obra.obrid
					 FROM
						obras.obrainfraestrutura obra) as valor ON valor.obrid = oi.obrid*/	
				WHERE
					oi.obrid NOT IN (SELECT obrid FROM obras.repositorio WHERE repstatus = 'A') AND 
					obsstatus = 'A' AND 
					orgid in ({$_SESSION["obras"]["orgidRepositorio"]}) 
					{$where}
				ORDER BY
					nome, stodesc";
		$cabecalho = array( "Ação", "Nome da Obra", "Unidade ResponsÃ¡vel pela Obra", "Campus", "Municipio / UF","Área Construída","Situação da Obra", "% Executado da Obra", "N° de Supervisões", "% Executado - Última Supervisão", "Situação da Obra - Última Supervisão" );
		$db->monta_lista_simples($sql, $cabecalho, 1000, 10, 'N', '95%');
			
		die;
		
	break;
	
	case "salvar":
		$supervisao->insereObrasRepositorio( $_REQUEST );	
	break;
	
}

// monta o titulo da tela
monta_titulo( "Selecionar Obras Para o Repositório", "Para inserir uma obra no repositório é necessário que a mesma tenha área construída informada." );

?>

<html>
	<head>
		<title><?php echo $GLOBALS['parametros_sistema_tela']['nome_e_orgao']; ?></title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script type="text/javascript" src="/includes/JQuery/jquery.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
		<form action="" method="post" name="formulario" id="formulario">
			<input type="hidden" value="salvar" name="requisicao" id="requisicao"/>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
				<tr>
					<td class="SubTituloDireita" width="190px">Unidade ResponsÃ¡vel pela Obra:</td>
					<td>
						<?php 
				
							$sql = "SELECT DISTINCT
										entid as codigo,
										entnome as descricao
									FROM
										entidade.entidade ee
									INNER JOIN
										obras.obrainfraestrutura oi ON oi.entidunidade = ee.entid
									WHERE
										obsstatus = 'A' AND orgid in ( {$_SESSION["obras"]["orgidRepositorio"]} )
									ORDER BY
										entnome";
							
							$db->monta_combo("entidunidade", $sql, "S", "Todas", '', '', '', '350', 'N','entidunidade');
							
						?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" width="190px">UF:</td>
					<td>
						<?php 
				
							$sql = "SELECT
										estuf as codigo,
										estdescricao as descricao
									FROM
										territorios.estado";
							
							$db->monta_combo("estuf", $sql, "S", "Todos", '', '', '', '', 'N','estado');
							
						?>
					</td>
				</tr>
				<!--<tr>
					<td class="SubTituloDireita">Situação da Supervisão:</td>
					<td>
						<?php 
						
//							$stsid = $_REQUEST["stsid"];
//							
//							$sql = "SELECT
//										stsid as codigo,
//										stsdsc as descricao
//									FROM
//										obras.statussupervisao
//									ORDER BY
//										stsdsc";
//							
//							$db->monta_combo("stsid", $sql, "S", "Todos", '', '', '', '', 'N','stsid');
							
						?>
					</td>
				</tr>-->
				<tr>
					<td class="SubTituloDireita">Data Limite Incial:</td>
					<td>
						<?php print campo_data( 'repdtlimiteinicial', 'N', 'S', '', 'S' ); ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Data Limite Final:</td>
					<td>
						<?php print campo_data( 'repdtlimitefinal', 'N', 'S', '', 'S' ); ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" style="width: 190px;">Tipo de Obra:</td>
					<td>
						<?php 
						
							$tobaid = $_SESSION["obras"]["filtros"]["tobaid"];
							$sql = "SELECT 
										tobaid as codigo, 
										tobadesc as descricao 
									FROM 
										obras.tipoobra 
									ORDER BY 
										tobadesc";
						
							$db->monta_combo( "tobaid", $sql, "S", "Todos", "", "", "", "", "N", "tobaid" );
							
						?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" style="width: 190px;">Situação da Obra:</td>
					<td>
						<?php 
						
							$stoid = $_SESSION["obras"]["filtros"]["stoid"];
							
							$sql = "SELECT 
										stoid as codigo, 
										stodesc as descricao 
									FROM 
										obras.situacaoobra 
									ORDER BY 
										stodesc";
						
							$db->monta_combo( "stoid", $sql, "S", "Todas", "", "", "", "", "N", "stoid" );
							
						?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" style="width: 190px;">Classificação da Obra:</td>
					<td>
						<?php 
						
							$cloid = $_SESSION["obras"]["filtros"]["cloid"];
						
							$sql = "SELECT 
										cloid as codigo,
										clodsc as descricao
									FROM 
									  	obras.classificacaoobra
									ORDER BY
										clodsc";
						
							$db->monta_combo( "cloid", $sql, "S", "Todas", "", "", "", "", "N", "cloid" );
							
						?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" style="width: 190px;">Programa / Fonte:</td>
					<td>
						<?php 
						
							$prfid = $_SESSION["obras"]["filtros"]["prfid"];
							
							$sql = "SELECT 
										prfid as codigo,
										prfdesc as descricao
								  	FROM 
								  		obras.programafonte
								  	WHERE
								  		orgid = {$_SESSION["obras"]["orgid"]}
								  	ORDER BY
								  		prfdesc";
						
							$db->monta_combo( "prfid", $sql, "S", "Todos", "", "", "", "", "N", "prfid" );
							
						?>
					</td>
				</tr>
				<!--<tr>-->
					<!--<td class="SubTituloDireita" style="width: 190px;">Unidade:</td>-->
					<!--<td>-->
						<?php 
							/*
							$entidunidade2 = $_SESSION["obras"]["filtros"]["entidunidade"];
							
							$sql = "SELECT 
										ee.entid as codigo, 
										upper(ee.entnome) as descricao 
									FROM
										entidade.entidade ee
									INNER JOIN 
										obras.obrainfraestrutura oi ON oi.entidunidade = ee.entid 
									WHERE
										orgid = {$_SESSION["obras"]["orgid"]} AND
										obsstatus = 'A'
									GROUP BY 
										ee.entnome, 
										ee.entid 
									ORDER BY 
										ee.entnome";
						
							$db->monta_combo( "entidunidade2", $sql, "S", "Todos", "", "", "", "", "N", "entidunidade2" );
							*/
						?>
					<!--</td>-->
				<!--</tr>-->
				<tr>
					<td class="SubTituloDireita" style="width: 190px;">Nome da Obra / Nº do Convênio / Nº do PI / ID:</td>
					<td>
						<?php $obrtextobusca = $_SESSION["obras"]["filtros"]["obrtextobusca"]; ?>
						<?php print campo_texto( 'obrtextobusca', 'N', 'S', '', 47, 60, '', '', 'left', '', 0, 'id="obrtextobusca"'); ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" style="width: 190px;">Possui foto:</td>
					<td>
						<label><input type="radio" name="foto" id="foto_1" value="S" <?php if( $_SESSION["obras"]["filtros"]["foto"] == "S" ){ print "checked='checked'"; } ?>/> Sim</label>
						<label><input type="radio" name="foto" id="foto_2" value="N" <?php if( $_SESSION["obras"]["filtros"]["foto"] == "N" ){ print "checked='checked'"; } ?>/> Não</label>
						<label><input type="radio" name="foto" id="foto_3" value="" <?php if( $_SESSION["obras"]["filtros"]["foto"] == "" ){ print "checked='checked'"; } ?> /> Todas</label>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" style="width: 190px;">Possui vistoria:</td>
					<td>
						<label><input type="radio" name="vistoria" id="vistoria_1" value="S" <?php if( $_SESSION["obras"]["filtros"]["vistoria"] == "S" ){ print "checked='checked'"; } ?>/> Sim</label>
						<label><input type="radio" name="vistoria" id="vistoria_2" value="N" <?php if( $_SESSION["obras"]["filtros"]["vistoria"] == "N" ){ print "checked='checked'"; } ?>/> Não</label>
						<label><input type="radio" name="vistoria" id="vistoria_3" value="" <?php if( $_SESSION["obras"]["filtros"]["vistoria"] == "" ){ print "checked='checked'"; } ?>/> Todas</label>
					</td>
				</tr>
				
				<tr>
					<td class="SubTituloDireita" style="width: 190px;">Possui supervisão empresa:</td>
					<td>
						<label><input type="radio" name="supervisao" id="supervisao_1" value="S" <?php if( $_SESSION["obras"]["filtros"]["supervisao"] == "S" ){ print "checked='checked'"; } ?>/> Sim</label>
						<label><input type="radio" name="supervisao" id="supervisao_2" value="N" <?php if( $_SESSION["obras"]["filtros"]["supervisao"] == "N" ){ print "checked='checked'"; } ?>/> Não</label>
						<label><input type="radio" name="supervisao" id="supervisao_3" value="" <?php if( $_SESSION["obras"]["filtros"]["supervisao"] == "" ){ print "checked='checked'"; } ?>/> Todas</label>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" style="width: 190px;">Valor da obra:</td>
					<td>
						De:&nbsp;
						<input type="text" class=" normal" title="" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" onkeyup="this.value=mascaraglobal('[#].##',this.value);" value="" maxlength="30" size="11" name="vlrmenor" id="vlrmenor">
						Até:&nbsp;
						<input type="text" class=" normal" title="" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" onkeyup="this.value=mascaraglobal('[#].##',this.value);" value="" maxlength="30" size="11" name="vlrmaior" id="vlrmaior">
					</td>
				</tr>
				
				<tr>
					<td class="SubTituloDireita" style="width: 190px;">Possui restrição:</td>
					<td>
						<label><input type="radio" name="restricao" id="restricao_1" value="S" <?php if( $_SESSION["obras"]["filtros"]["restricao"] == "S" ){ print "checked='checked'"; } ?>/> Sim</label>
						<label><input type="radio" name="restricao" id="restricao_2" value="N" <?php if( $_SESSION["obras"]["filtros"]["restricao"] == "N" ){ print "checked='checked'"; } ?>/> Não</label>
						<label><input type="radio" name="restricao" id="restricao_3" value="" <?php if( $_SESSION["obras"]["filtros"]["restricao"] == "" ){ print "checked='checked'"; } ?>/> Todas</label>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" style="width: 190px;">Possui PI:</td>
					<td>
						<label><input type="radio" name="planointerno" id="planointerno_1" value="S" <?php if( $_SESSION["obras"]["filtros"]["planointerno"] == "S" ){ print "checked='checked'"; } ?>/> Sim</label>
						<label><input type="radio" name="planointerno" id="planointerno_2" value="N" <?php if( $_SESSION["obras"]["filtros"]["planointerno"] == "N" ){ print "checked='checked'"; } ?>/> Não</label>
						<label><input type="radio" name="planointerno" id="planointerno_3" value="" <?php if( $_SESSION["obras"]["filtros"]["planointerno"] == "" ){ print "checked='checked'"; } ?>/> Todas</label>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" style="width: 190px;">Possui Aditivo:</td>
					<td>
						<label><input type="radio" name="aditivo" id="aditivo_1" value="S" <?php if( $_SESSION["obras"]["filtros"]["aditivo"] == "S" ){ print "checked='checked'"; } ?>/> Sim</label>
						<label><input type="radio" name="aditivo" id="aditivo_2" value="N" <?php if( $_SESSION["obras"]["filtros"]["aditivo"] == "N" ){ print "checked='checked'"; } ?>/> Não</label>
						<label><input type="radio" name="aditivo" id="aditivo_3" value="" <?php if( $_SESSION["obras"]["filtros"]["aditivo"] == "" ){ print "checked='checked'"; } ?>/> Todas</label>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" style="width: 190px;">% Executado da Obra:</td>
					<td>
						<table>
							<tr>
								<th>Mínimo</th>
								<th>Máximo</th>
							</tr>
							<tr>
								<?php
									
									$arPercentual[] = array( 'codigo' =>  0 , 'descricao' => '0 %' );
									$arPercentual[] = array( 'codigo' =>  5 , 'descricao' => '5 %' );
									$arPercentual[] = array( 'codigo' => 10 , 'descricao' => '10 %' );
									$arPercentual[] = array( 'codigo' => 15 , 'descricao' => '15 %' );
									$arPercentual[] = array( 'codigo' => 20 , 'descricao' => '20 %' );
									$arPercentual[] = array( 'codigo' => 25 , 'descricao' => '25 %' );
									$arPercentual[] = array( 'codigo' => 30 , 'descricao' => '30 %' );
									$arPercentual[] = array( 'codigo' => 35 , 'descricao' => '35 %' );
									$arPercentual[] = array( 'codigo' => 40 , 'descricao' => '40 %' );
									$arPercentual[] = array( 'codigo' => 45 , 'descricao' => '45 %' );
									$arPercentual[] = array( 'codigo' => 50 , 'descricao' => '50 %' );
									$arPercentual[] = array( 'codigo' => 55 , 'descricao' => '55 %' );
									$arPercentual[] = array( 'codigo' => 60 , 'descricao' => '60 %' );
									$arPercentual[] = array( 'codigo' => 65 , 'descricao' => '65 %' );
									$arPercentual[] = array( 'codigo' => 70 , 'descricao' => '70 %' );
									$arPercentual[] = array( 'codigo' => 75 , 'descricao' => '75 %' );
									$arPercentual[] = array( 'codigo' => 80 , 'descricao' => '80 %' );
									$arPercentual[] = array( 'codigo' => 85 , 'descricao' => '85 %' );
									$arPercentual[] = array( 'codigo' => 90 , 'descricao' => '90 %' );
									$arPercentual[] = array( 'codigo' => 95 , 'descricao' => '95 %' );
									$arPercentual[] = array( 'codigo' => 100 , 'descricao' => '100 %' );
									
									$percentualinicial = $_SESSION["obras"]["filtros"]['percentualinicial'];
									$percentualfinal   = $_SESSION["obras"]["filtros"]['percentualfinal'];
									
									$percfinal = $percentualfinal == '' ? 100 : $percentualfinal; 
									
									print '<td>';
									$db->monta_combo("percentualinicial", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualinicial');
									print '</td><td>';
									$db->monta_combo("percentualfinal", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualfinal', false,$percfinal);
									print '</td>';
									
								?>
							</tr>
						</table>
					</td>
				</tr>
				<tr bgcolor="#D0D0D0">
					<td></td>
					<td>
						<input type="button" value="Pesquisar" onclick="obrPesquisaListaObras();" style="cursor: pointer;"/>
					</td>
				</tr>
				<tr>
					<td colspan="2" class="SubTituloCentro">Lista de Obras</td>
				</tr>
			</table>
			<div id="listaObrasRepositorio">
				<font style="font-size: 8pt; color: #dd0000;">
					<center>Utilize os filtros para visualizar as obras.</center>
				</font>
			</div>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
				<tr bgcolor="#D0D0D0">
					<td>
						<input type="button" value="Salvar" onclick="document.getElementById('formulario').submit();" style="cursor: pointer;"/>
						<input type="button" value="Fechar" onclick="self.close();" style="cursor: pointer;"/>
					</td>
				</tr>
			</table>
		</form>
	</body>
</html>
