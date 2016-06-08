<script type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<?php
monta_titulo( 'Relacionamentos', 'Selecione os relacionamentos.'  );

$ocpid = $_REQUEST['ocpid'];
$fdpnome = $_REQUEST['fdpnome'];


$checkbox = "'<input type=\"checkbox\" id=\"chk_' || dap.fdpcpf || '\"  onclick=\"selecionaRelacionamento(\'' || dap.fdpcpf ||'\',\'' || dap.fdpnome || '\')\"  />'";


if($fdpnome){
	$arrwhere[] = "removeacento(upper(dap.fdpnome)) like removeacento(('%".strtoupper($fdpnome)."%'))";
}

$sql = "select
			$checkbox as acao,
			dap.fdpnome as nome
		from 
			gestaopessoa.ftdadopessoal dap
		where
			dap.fdpcpf != '{$_SESSION['fdpcpf']}'
		".($arrwhere ? " and ".implode(" and ",$arrwhere) : "" )."
		order by
			dap.fdpnome";
$arrCabecalho = array("Ação","Nome");		
?>
<script type="text/javascript">
$(function() {
	verificaRelacionamentos();
});

function verificaRelacionamentos()
{
	var rel = $('[name=arrFdpcpf[]]', window.opener.document);
	$.each(rel, function(key, obj) { 
		$('#chk_'+ obj.value).attr("checked","checked"); 
	});
}

function selecionaRelacionamento(fdpcpf,nome)
{
	if($("#chk_" + fdpcpf).is(':checked')){
		$("<tr id=\"tr_" + fdpcpf + "\" ><td><input type=\"hidden\" name=\"arrFdpcpf[]\" value=\"" + fdpcpf + "\"  /> <img onclick=\"excluirRelacionamento(\'" + fdpcpf + "\')\" class=\"link\" src=\"../imagens/excluir.gif\" /></td><td>" + nome + "</td><td><input type=\"text\" class=\" normal\" title=\"\" onblur=\"MouseBlur(this);\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" value=\"\" maxlength=\"\" size=\"61\" name=\"rloobs[]\"></td></tr>").appendTo( $('#tbl_relacionamento', window.opener.document) );
		alterarCoresTRTabela();
	}else{
		$('#tr_'+ fdpcpf, window.opener.document).remove();
		alterarCoresTRTabela();
	}
}

function alterarCoresTRTabela()
{
	var rel = $('[name=arrFdpcpf[]]', window.opener.document);
	var n = 0;
	var cor = "";
	$.each(rel, function(key, obj) { 
		if(n%2 == 1){
			cor = "#FFFFFF";
		}else{
			cor = "";
		}
		$('#tr_'+ obj.value, window.opener.document).attr("bgcolor","" + cor + "");
		$('#tr_'+ obj.value, window.opener.document).attr("onmouseover","this.bgColor='" + cor + "'");
		$('#tr_'+ obj.value, window.opener.document).attr("onmouseout","this.bgColor='" + cor + "'");
		n++; 
	});
}
</script>
<form name="formulario_popupocupacao" id="formulario_popupocupacao"  method="post" action="" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="25%" class="SubtituloDireita" >Nome</td>
			<td><?php echo campo_texto("fdpnome","S","S","Nome","60","","","",""); ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" ></td>
			<td><input type="submit" name="btn_pesquisar" value="Pesquisar" /></td>
		</tr>
	</table>
</form>
<?php $db->monta_lista($sql,$arrCabecalho,50,10,'N','center'); ?>