<?php

/**
 * Tela que monta um Repositorio de obras p/ vistoria de empresas
 * @author Fernando Bagno <fernandosilva@mec.gov.br>
 * @since 16/03/2010
 * @version 1.0
 *  
 */
$supervisao   = new supervisao();
$obrRespOrgid = $supervisao->obrBuscaTipoEnsinoResp();

switch( $_REQUEST["requisicao"] ){
	
	case "pesquisa":
		$filtros = $supervisao->obrFiltraListaRepositorio( );
	break;
	case "excluir":
		
		if( $supervisao->obrVerficaDadoRequisicao( $_REQUEST["obrid"], "repositorio", "obrid", "repstatus = 'A'" ) ){
			$supervisao->obrExcluiObraRepositorio( $_REQUEST["obrid"] );	
		}else{
			$supervisao->obrExibeMsgErro( "Esta obra nao existe no repositorio!" );
		}
		
	break;
	
}

//cria a sessao do tipo de ensino

$_SESSION["obras"]["orgidRepositorio"] = '';

if( $_REQUEST["orgid"] ){
	$_SESSION["obras"]["orgidRepositorio"] = $_REQUEST["orgid"];	
}else{
	if( is_array($obrRespOrgid) ){
		foreach($obrRespOrgid as $dado){
			$_SESSION["obras"]["orgidRepositorio"] .= $_SESSION["obras"]["orgidRepositorio"] != '' ? ','.$dado : $dado;	
		}
	}
}

// cabecalho padr„o do sistema
include APPRAIZ . "includes/cabecalho.inc";

// Monta as abas
print "<br/>";
$db->cria_aba( $abacod_tela, $url, $parametros );
monta_titulo( "Repositorio de Obras", "" );


?>
<script src="../includes/calendario.js"></script>
<form action="" method="post" name="formulario" id="obrFormPesRepositorio">
	<input type="hidden" name="requisicao" id="requisicao" value="pesquisa"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td class="SubTituloCentro" colspan="2">Argumentos de Pesquisa</td>
		</tr>
		<!--
		<tr>
			<td class="SubTituloDireita" width="190px">Unidade Respons√°vel pela Obra:</td>
			<td>
				<?php 
					/* 
					$entidunidade = $_REQUEST["entidunidade"];
				
					$sql = "SELECT DISTINCT
								entid as codigo,
								entnome as descricao
							FROM
								entidade.entidade ee
							INNER JOIN
								obras.obrainfraestrutura oi ON oi.entidunidade = ee.entid
							INNER JOIN
								obras.repositorio ore ON ore.obrid = oi.obrid
							WHERE
								obsstatus = 'A' AND 
								orgid in ({$_SESSION["obras"]["orgidRepositorio"]}) AND
								repstatus = 'A'";
					
					$db->monta_combo("entidunidade", $sql, "S", "Todas", '', '', '', '', 'N','entidunidade');
					*/
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nome:</td>
			<td>
				<?php
					/*
					$obrdesc = $_REQUEST["obrdesc"]; 
					print campo_texto( "obrdesc", "N", "S", "", 65, 60, "", "", "left", "", 0, "obrdesc");
					*/ 
				?>
			</td>
		</tr>
		-->
		<tr>
			<td class="SubTituloDireita">UF:</td>
			<td>
				<?php 
				
					$estuf = $_REQUEST["estuf"];
					
					$sql = "SELECT
								estuf as codigo,
								estdescricao as descricao
							FROM
								territorios.estado
							ORDER BY
								estuf";
					
					$db->monta_combo("estuf", $sql, "S", "Todos", '', '', '', '', 'N','estado');
					
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Situacao Supervisao do Grupo (Empresa):</td>
			<td>
				<?php 
				
				 	$esdid = $_REQUEST["esdid"];
					
					$sql = "SELECT DISTINCT 
								esdid AS codigo, 
								esddsc AS descricao
  							FROM 
  								workflow.estadodocumento
  							WHERE
  								esdstatus = 'A'
  								AND
  								tpdid = ". OBR_TIPO_DOCUMENTO ."
  								AND esdid <> ".OBRENVREAVALSUPMEC."
								AND esdid <> ".OBREMAVALIASUPERVMEC."
								AND esdid <> ".OBREMSUPERVISAO." 
								AND esdid <> ".OBRREAJSUPVISAOEMP." 
								AND esdid <> ".OBRREAVSUPVISAO." 
								AND esdid <> ".OBRREDISTRIBUIDO." 
								AND esdstatus = 'A'
							ORDER BY
								esddsc";
					
					$db->monta_combo("esdid", $sql, "S", "Todos", '', '', '', '', 'N','esdid');
					
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="190px">Situacao Supervisao da Obra (Empresa):</td>
			<td>
				<?php 

					$esdidobra = $_REQUEST["esdidobra"];
				
					$sql = "SELECT DISTINCT
								esdid as codigo,
								esddsc as descricao
							FROM
								workflow.estadodocumento ed
							WHERE
								tpdid = " . OBR_TIPO_DOCUMENTO_OBRA . "
							ORDER BY
								esddsc";
					
					$situacoes = $db->carregar($sql);
	
					array_push( $situacoes, array("codigo" => OBRSITSUPREPOSITORIO, "descricao" => 'Em RepositÛrio'),array("codigo" => OBRSITSUPDISTRIBUIDA, "descricao"=>'DistribuÌda') );
					
					$db->monta_combo("esdidobra", $situacoes, "S", "Todas", '', '', '', '', 'N','esdidobra');
				
				?>
			</td>
		</tr>
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
			<td class="SubTituloDireita" style="width: 190px;">Situacao da Obra:</td>
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
			<td class="SubTituloDireita" style="width: 190px;">Classificacao da Obra:</td>
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
						  		orgid IN ({$_SESSION["obras"]["orgidRepositorio"]})
						  	ORDER BY
						  		prfdesc";
				
					$db->monta_combo( "prfid", $sql, "S", "Todos", "", "", "", "", "N", "prfid" );
					
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="width: 190px;">Unidade:</td>
			<td>
				<?php 
				
					$entidunidade = $_SESSION["obras"]["filtros"]["entidunidade"];
					
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
				
					$db->monta_combo( "entidunidade", $sql, "S", "Todos", "", "", "", "", "N", "entidunidade" );
					
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="width: 190px;">Nome da Obra / Numero do Convenio / Numero do PI / ID:</td>
			<td>
				<?php $obrtextobusca = $_SESSION["obras"]["filtros"]["obrtextobusca"]; ?>
				<?php print campo_texto( 'obrtextobusca', 'N', 'S', '', 47, 60, '', '', 'left', '', 0, ''); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="width: 190px;">Possui foto:</td>
			<td>
				<input type="radio" name="foto" id="" value="S" <?php if( $_SESSION["obras"]["filtros"]["foto"] == "S" ){ print "checked='checked'"; } ?>/> Sim
				<input type="radio" name="foto" id="" value="N" <?php if( $_SESSION["obras"]["filtros"]["foto"] == "N" ){ print "checked='checked'"; } ?>/> Nao
				<input type="radio" name="foto" id="" value="" <?php if( $_SESSION["obras"]["filtros"]["foto"] == "" ){ print "checked='checked'"; } ?> /> Todas
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="width: 190px;">Possui vistoria:</td>
			<td>
				<input type="radio" name="vistoria" id="" value="S" <?php if( $_SESSION["obras"]["filtros"]["vistoria"] == "S" ){ print "checked='checked'"; } ?>/> Sim
				<input type="radio" name="vistoria" id="" value="N" <?php if( $_SESSION["obras"]["filtros"]["vistoria"] == "N" ){ print "checked='checked'"; } ?>/> Nao
				<input type="radio" name="vistoria" id="" value="" <?php if( $_SESSION["obras"]["filtros"]["vistoria"] == "" ){ print "checked='checked'"; } ?>/> Todas
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="width: 190px;">Possui restricao:</td>
			<td>
				<input type="radio" name="restricao" id="" value="S" <?php if( $_SESSION["obras"]["filtros"]["restricao"] == "S" ){ print "checked='checked'"; } ?>/> Sim
				<input type="radio" name="restricao" id="" value="N" <?php if( $_SESSION["obras"]["filtros"]["restricao"] == "N" ){ print "checked='checked'"; } ?>/> Nao
				<input type="radio" name="restricao" id="" value="" <?php if( $_SESSION["obras"]["filtros"]["restricao"] == "" ){ print "checked='checked'"; } ?>/> Todas
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="width: 190px;">Possui PI:</td>
			<td>
				<input type="radio" name="planointerno" id="" value="S" <?php if( $_SESSION["obras"]["filtros"]["planointerno"] == "S" ){ print "checked='checked'"; } ?>/> Sim
				<input type="radio" name="planointerno" id="" value="N" <?php if( $_SESSION["obras"]["filtros"]["planointerno"] == "N" ){ print "checked='checked'"; } ?>/> Nao
				<input type="radio" name="planointerno" id="" value="" <?php if( $_SESSION["obras"]["filtros"]["planointerno"] == "" ){ print "checked='checked'"; } ?>/> Todas
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="width: 190px;">Possui Aditivo:</td>
			<td>
				<input type="radio" name="aditivo" id="" value="S" <?php if( $_SESSION["obras"]["filtros"]["aditivo"] == "S" ){ print "checked='checked'"; } ?>/> Sim
				<input type="radio" name="aditivo" id="" value="N" <?php if( $_SESSION["obras"]["filtros"]["aditivo"] == "N" ){ print "checked='checked'"; } ?>/> Nao
				<input type="radio" name="aditivo" id="" value="" <?php if( $_SESSION["obras"]["filtros"]["aditivo"] == "" ){ print "checked='checked'"; } ?>/> Todas
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="width: 190px;">% Executado da Obra:</td>
			<td>
				<table>
					<tr>
						<th>Minimo</th>
						<th>Maximo</th>
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
				<input type="button" name="obrBtFiltraRepositorio" value="Pesquisar" onclick="document.getElementById('obrFormPesRepositorio').submit();" style="cursor: pointer;"/>
			</td>
		</tr>
		<tr bgcolor="#f5f5f5">
			<td></td>
			<td>
			</td>
		</tr>
		<?php if( possuiPerfil( PERFIL_SUPERVISORMEC ) && !$db->testa_superuser() ):?>
		<tr bgcolor="#D0D0D0">
			<td colspan="2">
				<input type="button" name="obrBtInserirObra" value="Inserir Obras" onclick="" style="cursor: pointer;" disabled="disabled" />
				<input type="button" name="obrBtVisualizarObrasMapa" value="Visualizar no Mapa" onclick="" style="cursor: pointer;" disabled="disabled" />
			</td>
		</tr>
		<?php else: ?>
		<tr bgcolor="#D0D0D0">
			<td colspan="2">
				<input type="button" name="obrBtInserirObra" value="Inserir Obras" onclick="obrSelecionaObras();" style="cursor: pointer;"/>
				<input type="button" name="obrBtVisualizarObrasMapa" value="Visualizar no Mapa" onclick="janela('?modulo=principal/supervisao/mapaSupervisao&acao=A', 600, 585, 'mapaSupervisao');" style="cursor: pointer;"/>
			</td>
		</tr>
		<?php endif; ?>
	</table>
</form>
<?php 
//CondiÁ„o para apresentar inicialmente apenas as Obras de um Tipo de Ensino.
if(empty($_REQUEST["orgid"])){
	$_SESSION["obras"]["orgidRepositorio"] = $_SESSION["obras"]["orgid"];
}else{
	$_SESSION["obras"]["orgidRepositorio"] = $_SESSION["obras"]["orgidRepositorio"];
}

// monta as abas com os tipo de ensino
print "<br/>";
print $supervisao->obrMontaAbasTipoEnsino( $obrRespOrgid, $_SESSION["obras"]["orgidRepositorio"] );
monta_titulo( "Lista de Obras", "" );
// lista de obras
$supervisao->obrListaObrasRepositorio($_SESSION["obras"]["orgidRepositorio"], $filtros); ?>
	
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
	<tr bgcolor="#D0D0D0">
		<td>
			<input type="button" name="obrBtInserirObra" value="Inserir Obras" onclick="obrSelecionaObras();" style="cursor: pointer;"/>
			<input type="button" name="obrBtVisualizarObrasMapa" value="Visualizar no Mapa" onclick="janela('?modulo=principal/supervisao/mapaSupervisao&acao=A', 600, 585, 'mapaSupervisao');" style="cursor: pointer;"/>
		</td>
	</tr>
</table>
