<? 
switch ($_REQUEST['evento']){
	case 'salvar':
		insereDadosProgramaTipologia();
		die("<script>
				alert('Operação realizada com sucesso!');
				location.href = '?modulo=sistema/geral/tabelas_de_apoio&acao=A&tabelas=programatipologia';
			 </script>");
	break;
	case 'editar':
		extract(carregaProgramaTipologia());
	break;
	case 'excluir':
		ExcluirProgramaTipologia();
		die("<script>
				alert('Operação realizada com sucesso!');
				location.href = '?modulo=sistema/geral/tabelas_de_apoio&acao=A&tabelas=programatipologia';
			 </script>");
	break;
}

//if ($_REQUEST['evento'] == 'salvar'){
//	insereDadosProgramaTipologia();
//	die("<script>
//			alert('Operação realizada com sucesso!');
//			location.href = '?modulo=sistema/geral/tabelas_de_apoio&acao=A&tabelas=programatipologia';
//		 </script>");
//}

//extract();

// Busca Programa tipologia, já existente
$progTipExist = carregaListaProgramaTipologia();
$js = array();
if ( is_array($progTipExist) ):
	foreach($progTipExist as $arrProgTip){
		$jsArr = array();	
		if ( is_array($progTipExist) ):
			foreach($arrProgTip as $k => $val){
				$jsArr[] = "$k : '" . htmlentities($val, ENT_QUOTES) . "'";	
			}
			$js[] = implode(',', $jsArr);
		endif;	
	}
endif;
$js = "[{" . implode("}, {", $js) . "}]";
?>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script type="text/javascript">
function validaForm(){
	var msg 	= '';
	var retorno = true;
	var progTip = <?=$js; ?>;
	var prfid, cloid, tpoid;
	
	for (i=0; i < progTip.length; i++){
		prfid = progTip[i].prfid;
		cloid = progTip[i].cloid;
		tpoid = progTip[i].tpoid;
		
		if ( prfid == $('[name=prfid]').val() && cloid == $('[name=cloid]').val() && tpoid == $('[name=tpoid]').val() ){
			msg += 'Este Programa/Tipologia já se encontra cadastrado!\n';
		}
	}
	
	if ( $('[name=prfid]').val() == '' ){
		msg += 'O campo "Programa" é obrigatório \n';
	}
	
	if ( $('[name=cloid]').val() == '' ){
		msg += 'O campo "Classificação" é obrigatório \n';
	}
	
	if ( $('[name=tpoid]').val() == '' ){
		msg += 'O campo "Tipologia" é obrigatório';
	}
	
	if (msg != ''){
		alert(msg);
		retorno = false;
	}
	return retorno;
}

function submeteTabelasApoio() {
	var select = document.getElementById('tabelas');
			
	if(select.value != "") {
		document.getElementById('descricao_tabela').value = select.options[select.selectedIndex].innerHTML;
		document.getElementById('formTabelasApoio').submit();
	}
}

function redireciona(url){
	location.href = url;
}

function confirmExcluir(url, msg) {
	if(confirm(msg)) {
		window.location = url;
	}
} 

</script>

<form method="post" name="formTabelasApoio" id="formTabelasApoio" action="?modulo=sistema/geral/tabelas_de_apoio&acao=A">
<input type="hidden" id="enviado" name="enviado" value="0">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloDireita">Selecione a tabela:</td>
			<td>
				<select class="CampoEstilo" id="tabelas" name="tabelas" onChange="submeteTabelasApoio();">
					<option value="">-- Selecione uma Tabela --</option>
					<option value="itenscomposicao">Etapas da Obra</option>
					<option value="tiporestricao">Tipo de Restrição</option>
					<option value="tiporespcontato">Tipo de Responsável</option>
					<option value="tipoobra">Tipo de Obra</option>
					<option value="unidademedida">Unidade de Medida</option>
					<option value="tipoarquivo">Tipo de Arquivo</option>
					<option value="itensdetalhamento">Módulo de Ampliação</option>
					<option value="situacaoobra">Situação da Obra</option>
					<option value="desempenhoconstrutora">Desempenho da Construtora</option>
					<option value="qualidadeobra">Qualidade da Obra</option>
					<option value="programafonte">Programa Fonte</option>
					<option value="tipologiaobra">Tipologia da Obra</option>
					<option value="programatipologia">Programa / Tipologia</option>
					<option value="precocombustivel">Tabela de Preço do Combustível</option>
				</select>
				<input type="hidden" id="descricao_tabela" name="descricao_tabela">
			</td>
		</tr>
	</table>
</form>

<form method="POST"  name="telaFormulario" onsubmit="javascript: return validaForm()"> 
<input type="hidden" name="evento" value="salvar">
<input type="hidden" name="ptpid" value="<?=$ptpid ?>">
<input type="hidden" name="tabelas" value="<?=$_REQUEST['tabelas'] ?>">

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td colspan="2">Programa / Tipologia</td>
	</tr>
	<tr>
		<td align="right" valign="top" class="SubTituloDireita">Programa</td>
		<td>
			<!-- Lembre-se de configurar o SQL do campo -->         
			<?
			$sql="SELECT 
					prfid as codigo, 
					orgdesc || ' - ' || prfdesc as descricao 
				  FROM 
				  	obras.programafonte
				  INNER JOIN
				  	obras.orgao USING(orgid)
				  ORDER BY
				  	descricao ASC";
			$db->monta_combo('prfid', $sql, 'S', 'Selecione...', '', '', '', '', 'S');
		 	?>
		</td>
	</tr>
	<tr>
		<td align="right" valign="top" class="SubTituloDireita">Classificação</td>
		<td>
			<!-- Lembre-se de configurar o SQL do campo -->         
			<?
			$sql="SELECT 
					cloid as codigo, 
					clodsc as descricao 
				  FROM 
				  	obras.classificacaoobra";
			$db->monta_combo('cloid', $sql, 'S', 'Selecione...', '', '', '', '', 'S');
		 	?>
		</td>
	</tr>
	<tr>
		<td align="right" valign="top" class="SubTituloDireita">Tipologia</td>
		<td>
			<!-- Lembre-se de configurar o(s) SQL(s) do campo -->
				<?
					// SQL que montará a tela de opções do campo
					$sql_opcao = sprintf("SELECT
											 tpoid AS value,
											 tpodsc AS codigo, 
											 tpodetalhe AS descricao 
										   FROM
										     obras.tipologiaobra");	
	
					// Opção selecionada					   
					if ($tpoid){	
						$tpoid = array(
										"descricao" => $tpodsc . ' - ' . $tpodetalhe,
										"value" 	=> $tpoid
									  );						   
					}
					campo_popup(
	                               "tpoid",
	                               $sql_opcao,
	                               "Configure o campo",
	                               "",
	                               "400x400",
	                               "60",
	                               "",
	                               1,
	                               false
	                       		);          
				?>
		</td>
	</tr>
	<tr bgcolor="#CCCCCC">
	   <td>&nbsp;</td>
	   <td>
	   	<input type="submit" name="btalterar" value="Salvar" onclick="" class="botao">
	   </td>
	</tr> 	
</table>
</form>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td>
			<fieldset style="background: #ffffff;">
				<legend>Lista de Programa(s) / Tipologia(s)</legend>
				<div style="height: 320px; overflow: auto;"><? listaProgramaTipologia() ?></div>
			</fieldset>
		</td>
	</tr>
</table>