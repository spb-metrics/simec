<?php

$supervisao = new supervisao();

switch( $_REQUEST["requisicao"] ){
	
	case "novo":
		$_SESSION["obras"]["epcid"] = null;
	break;
	
	case "salvar":
		$supervisao->obrCadastraEmpresaContratada( $_REQUEST );
	break;
	
}

if( $_SESSION["obras"]["epcid"] || $_REQUEST["epcid"] ){
	
	$_SESSION["obras"]["epcid"] = $_REQUEST["epcid"] ? $_REQUEST["epcid"] : $_SESSION["obras"]["epcid"]; 
	$dadosEmpresa = $supervisao->obrBuscaDadosEmpresa( $_SESSION["obras"]["epcid"] );
	extract($dadosEmpresa);
}

// cabecalho padrão do simec
include APPRAIZ . "includes/cabecalho.inc";

// Monta as abas
print "<br/>";
$db->cria_aba( $abacod_tela, $url, $parametros );
monta_titulo( "Inserir Empresa Contratada", obrigatorio() . " Indica os campos obrigatórios" );


?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<form action="" method="post" name="formulario" id="obrFormPesquisaEmpresa">
	<input type="hidden" name="requisicao" id="requisicao" value="salvar"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td class="SubTituloDireita" width="190px">Empresa:</td>
			<td>
				<input type="hidden" name="entid" id="entid" value="<?php print $entid; ?>"/>
				<input type="hidden" name="epcid" id="epcid" value="<?php print $_SESSION["obras"]["epcid"]; ?>"/>
				<?php print campo_texto( "entnome", "S", "N", "", 60, 60, "", "", "left", "", 0, "id='entnome'"); ?>
				
				<img src="../imagens/gif_inclui.gif" onclick="inserirEmpresa(document.getElementById('entid').value, 'supervisao');" style="border: 0px; vertical-align: middle; cursor: pointer;" title="Inserir Empresa" />
				
			</td>			
		</tr>
		<tr>
			<td class="SubTituloDireita">UF's de Atendimento:</td>
			<td>
				<select id="estuf" name="estuf[]" multiple="multiple" class="CampoEstilo" size="10" style="width:316px; cursor:pointer;" ondblclick="obrSelecionaUfEmpresa();">
					<?php 
					
					if( !empty( $_SESSION["obras"]["epcid"] ) ){
							$sql = "SELECT
										oe.estuf || '|' || oe.muncod as codigo,
										oe.estuf || ' - ' || mundescricao as descricao
									FROM
										obras.empresaufatuacao oe
									INNER JOIN
										territorios.municipio tm ON tm.muncod = oe.muncod
									WHERE
										epcid = {$_SESSION["obras"]["epcid"]}";

							$ufsAtendimento = $db->carregar( $sql );
							
							if( is_array( $ufsAtendimento ) ){
								for( $i = 0; $i < count($ufsAtendimento); $i++ ){
									print "<option value='{$ufsAtendimento[$i]["codigo"]}'>{$ufsAtendimento[$i]["descricao"]}</option>";
								}
							}else{
								print "<option value=''>Duplo clique para selecionar da lista...</option>";
							}
							
						}else{
							print "<option value=''>Duplo clique para selecionar da lista...</option>";
						}
					
					
					?>
				</select>
				<?php print obrigatorio(); ?>
			</td>			
		</tr>
		<tr>
			<td class="SubTituloDireita">Nº do Processo de Concessão:</td>
			<td>
				<?php print campo_texto( "epcnumproceconc", "N", $somenteLeitura, "", 20, 20, "", "", "left", "", 0, "id='epcnumproceconc'"); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">N° do Contrato:</td>
			<td>
				<?php print campo_texto( "epcnumcontrato", "N", $somenteLeitura, "", 20, 20, "", "", "left", "", 0, "id='epcnumcontrato'"); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de Início do Contrato:</td>
			<td>
				<?php print campo_data2( 'epcdtiniciocontrato', 'N', $somenteLeitura, '', 'S' ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de Término do Contrato:</td>
			<td>
				<?php print campo_data2( 'epcdtfinalcontrato', 'N', $somenteLeitura, '', 'S' ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2">Responsáveis pela Empresa</td>
		</tr>
		<tr>
			<td colspan="2">
			
				<br/>
				
				<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="respEmpresas">
					<thead>
						<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">
							<b>Ação</b>
						</td>
						<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">
							<b>CPF</b>
						</td>
						<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">
							<b>Nome</b>
						</td>
						<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">
							<b>Telefone</b>
						</td>
						<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">
							<b>Celular</b>
						</td>
						<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">
							<b>E-mail</b>
						</td>
					</thead>
					
					<?php $supervisao->obrMontaListaRespEmpresa( $_SESSION["obras"]["epcid"] );?>
					
				</table>
				
				<br/>
				
				<a style="border: 0px; cursor: pointer;" title="Inserir Responsáveis" onclick="inserirResponsavel( '', 'supervisao' );">
					<img src="../imagens/gif_inclui.gif" style="vertical-align: middle; "/>
					Inserir Responsáveis
				</a>
			
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td></td>
			<td>
				<input type="button" value="Salvar" onclick="obrValidaEmpresa();" style="cursor: pointer;"/>
				<input type="button" value="Voltar" onclick="history.back(-1);" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>
