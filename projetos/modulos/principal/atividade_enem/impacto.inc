<?php 
$titulo = "Lista de Processos";
monta_titulo( $titulo, '&nbsp');
?>
<script language="JavaScript" src="../../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
function cadastraProcesso(atiid,atidescricao){

	if($("#ckb_" + atiid + "").attr("checked") == true){
		var tbl = $("#processoatividade",window.opener.document);
		var col1 = "<td><input type=\"hidden\" value=\"" + atiid + "\"  name=\"atiidprocesso[]\"  />" + atidescricao + "</td>";
		var col2 = '<td><input type="text" name="ipcdsc[]" size="110" maxlength="110" value="" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" class="normal" /></td>';
		$("<tr id=\"tr_processo_" + atiid + "\" >" + col1 + col2 + "</tr>").appendTo( tbl );
	}else{
		$("#tr_processo_" + atiid,window.opener.document).remove();
	}
}

function verificaProcesso(atiid){

	if($("#tr_processo_" + atiid,window.opener.document).html()){
		$("#ckb_" + atiid + "").attr("checked","checked");
	}
}
</script>
<?php 
if( temPerfilSomenteConsulta() || temPerfilExecValidCertif() )
	$acao = "'<input type=\"checkbox\" id=\"ckb_' || atiid || '\" name=\"ckb_' || atiid || '\" /><script>verificaProcesso(\'' || atiid || '\')</script>'";
else
	$acao = "'<input type=\"checkbox\" onclick=\"cadastraProcesso(\'' || atiid || '\',\'' || atidescricao || '\')\" id=\"ckb_' || atiid || '\" name=\"ckb_' || atiid || '\" /><script>verificaProcesso(\'' || atiid || '\')</script>'";

$sql = "select 
			".$acao.",
			atidescricao
		from
			projetos.atividade
		where
			atistatus = 'A'
		and
			atitipoenem = 'P'";
$cabecalho = array("Ação","Descrição");

$db->monta_lista($sql,$cabecalho,30,30,'N','center');
?>

	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubtituloDireita" width="100%" >
			<input type="button" value="Fechar" onclick="javascript:window.close()">
		    </td>
		</tr>
	</table>