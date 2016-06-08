<?php

/**
 * Tela que monta um Repositório de obras p/ vistoria de empresas
 * @author Rodrigo Pereira de Souza Silva <rodrigossilva@mec.gov.br>
 * @since 22/09/2010
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
			$supervisao->obrExibeMsgErro( "Esta obra não existe no repositório!" );
		}
		
	break;
	
}

//cria a sessão do tipo de ensino

$_SESSION["obras"]["orgidRepositorio"] = '';

if( $_REQUEST["orgid"] ){
	$_SESSION["obras"]["orgidRepositorio"] = $_REQUEST["orgid"];	
}else{
	switch (is_array($obrRespOrgid)){
		case $obrRespOrgid[0] == 1 :
				$_SESSION["obras"]["orgidRepositorio"] = 1;
		break;	
		case $obrRespOrgid[0] == 2 :
				$_SESSION["obras"]["orgidRepositorio"] = 2;
		break;
		default:
				$_SESSION["obras"]["orgidRepositorio"] = 3;
		break;
	}
	/*	
	    if( is_array($obrRespOrgid) ){
			foreach($obrRespOrgid as $dado){
				$_SESSION["obras"]["orgidRepositorio"] .= $_SESSION["obras"]["orgidRepositorio"] != '' ? ','.$dado : $dado;
			}
		}
	*/	
}

// cabecalho padrão do sistema
include APPRAIZ . "includes/cabecalho.inc";

// Monta as abas
print "<br/>";
$db->cria_aba( $abacod_tela, $url, $parametros );
monta_titulo( "Supervisões Finalizadas", "" );


?>
<script src="../includes/calendario.js"></script>
<form action="" method="post" name="formulario" id="obrFormPesRepositorio">
	<input type="hidden" name="requisicao" id="requisicao" value="pesquisa"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td class="SubTituloCentro" colspan="2">Argumentos de Pesquisa</td>
		</tr>
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
			<td class="SubTituloDireita">Situação da Supervisão:</td>
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
							ORDER BY
								esddsc";

					$db->monta_combo("esdid", $sql, "S", "Todos", '', '', '', '', 'N','esdid');
					
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
			<td class="SubTituloDireita" style="width: 190px;">Nome da Obra / Nº do Convênio / Nº do PI / ID:</td>
			<td>
				<?php $obrtextobusca = $_SESSION["obras"]["filtros"]["obrtextobusca"]; ?>
				<?php print campo_texto( 'obrtextobusca', 'N', 'S', '', 47, 60, '', '', 'left', '', 0, ''); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="width: 190px;">Possui foto:</td>
			<td>
				<input type="radio" name="foto" id="" value="S" <?php if( $_SESSION["obras"]["filtros"]["foto"] == "S" ){ print "checked='checked'"; } ?>/> Sim
				<input type="radio" name="foto" id="" value="N" <?php if( $_SESSION["obras"]["filtros"]["foto"] == "N" ){ print "checked='checked'"; } ?>/> Não
				<input type="radio" name="foto" id="" value="" <?php if( $_SESSION["obras"]["filtros"]["foto"] == "" ){ print "checked='checked'"; } ?> /> Todas
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="width: 190px;">Possui vistoria:</td>
			<td>
				<input type="radio" name="vistoria" id="" value="S" <?php if( $_SESSION["obras"]["filtros"]["vistoria"] == "S" ){ print "checked='checked'"; } ?>/> Sim
				<input type="radio" name="vistoria" id="" value="N" <?php if( $_SESSION["obras"]["filtros"]["vistoria"] == "N" ){ print "checked='checked'"; } ?>/> Não
				<input type="radio" name="vistoria" id="" value="" <?php if( $_SESSION["obras"]["filtros"]["vistoria"] == "" ){ print "checked='checked'"; } ?>/> Todas
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="width: 190px;">Possui restrição:</td>
			<td>
				<input type="radio" name="restricao" id="" value="S" <?php if( $_SESSION["obras"]["filtros"]["restricao"] == "S" ){ print "checked='checked'"; } ?>/> Sim
				<input type="radio" name="restricao" id="" value="N" <?php if( $_SESSION["obras"]["filtros"]["restricao"] == "N" ){ print "checked='checked'"; } ?>/> Não
				<input type="radio" name="restricao" id="" value="" <?php if( $_SESSION["obras"]["filtros"]["restricao"] == "" ){ print "checked='checked'"; } ?>/> Todas
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="width: 190px;">Possui PI:</td>
			<td>
				<input type="radio" name="planointerno" id="" value="S" <?php if( $_SESSION["obras"]["filtros"]["planointerno"] == "S" ){ print "checked='checked'"; } ?>/> Sim
				<input type="radio" name="planointerno" id="" value="N" <?php if( $_SESSION["obras"]["filtros"]["planointerno"] == "N" ){ print "checked='checked'"; } ?>/> Não
				<input type="radio" name="planointerno" id="" value="" <?php if( $_SESSION["obras"]["filtros"]["planointerno"] == "" ){ print "checked='checked'"; } ?>/> Todas
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="width: 190px;">Possui Aditivo:</td>
			<td>
				<input type="radio" name="aditivo" id="" value="S" <?php if( $_SESSION["obras"]["filtros"]["aditivo"] == "S" ){ print "checked='checked'"; } ?>/> Sim
				<input type="radio" name="aditivo" id="" value="N" <?php if( $_SESSION["obras"]["filtros"]["aditivo"] == "N" ){ print "checked='checked'"; } ?>/> Não
				<input type="radio" name="aditivo" id="" value="" <?php if( $_SESSION["obras"]["filtros"]["aditivo"] == "" ){ print "checked='checked'"; } ?>/> Todas
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
				<input type="button" name="obrBtFiltraRepositorio" value="Pesquisar" onclick="document.getElementById('obrFormPesRepositorio').submit();" style="cursor: pointer;"/>
			</td>
		</tr>
		<tr bgcolor="#f5f5f5">
			<td></td>
			<td>
			</td>
		</tr>
	<!--<tr bgcolor="#D0D0D0">-->
	<!--	<td colspan="2">-->
	<!--		<input type="button" name="obrBtInserirObra" value="Inserir Obras" onclick="obrSelecionaObras();" style="cursor: pointer;"/>-->
	<!--		<input type="button" name="obrBtVisualizarObrasMapa" value="Visualizar no Mapa" onclick="janela('?modulo=principal/supervisao/mapaSupervisao&acao=A', 600, 585, 'mapaSupervisao');" style="cursor: pointer;"/>-->
	<!--	</td>-->
	<!--</tr>-->
	</table>
</form>
<?php 

// monta as abas com os tipo de ensino
print "<br/>";
print $supervisao->obrMontaAbasTipoEnsino2( $obrRespOrgid, $_SESSION["obras"]["orgidRepositorio"] );
monta_titulo( "Lista de Obras", "" );
// lista de obras
$supervisao->obrListaObrasSupervisoesFinalizadas($_SESSION["obras"]["orgidRepositorio"], $filtros); ?>
	
<!--<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">-->
<!--	<tr bgcolor="#D0D0D0">-->
<!--		<td>-->
<!--			<input type="button" name="obrBtInserirObra" value="Inserir Obras" onclick="obrSelecionaObras();" style="cursor: pointer;"/>-->
<!--			<input type="button" name="obrBtVisualizarObrasMapa" value="Visualizar no Mapa" onclick="janela('?modulo=principal/supervisao/mapaSupervisao&acao=A', 600, 585, 'mapaSupervisao');" style="cursor: pointer;"/>-->
<!--		</td>-->
<!--	</tr>-->
<!--</table>-->
