<?php

ini_set("memory_limit", "1024M");
set_time_limit(0);


if($_FILES['arquivo']) {
	
	include APPRAIZ ."includes/funcoes_public_arquivo.php";
	
	$arrValidacao = array('extensao');
	
	$resp = atualizarPublicArquivo($arrValidacao);
	
	if($resp['TRUE']) $msg .= 'Foram processados '.count($resp['TRUE']).' arquivos.'.'\n';
	if($resp['FALSE']) {
		$msg .= 'Problemas encontrados:'.'\n';
		foreach($resp['FALSE'] as $k => $v) {
			$msg .= 'ARQID : '.$k.' | '.$v.'\n';
		}
	}
	
	die("<script>
			alert('".$msg."');
			window.location = window.location;
		 </script>");
}


include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

include "funcoes_obras_arquivo.php";

if(!$_REQUEST['tabela']) $_REQUEST['tabela']='fotos';

$menu = carregarMenuObras();

if(!$menu) {
	die("<script>
			alert('Não existem fotos para você fazer UPLOAD');
			windoe.location='obras.php?modulo=inicio&acao=A&acao=C';
		 </script>");
}

echo montarAbasArray($menu, '/obras/obras.php?modulo=sistema/public_arquivo/obras_arquivo&acao=A&tabela='.$_REQUEST['tabela']);

?>
<script type="text/javascript">

function limpaUpload(arqid)
{
	document.getElementById('arquivo_' + arqid).value = "";
}

function uploadArquivos()
{
	document.getElementById('btn_salvar').value="Carregando...";
	document.getElementById('btn_salvar').disabled = "true";
	document.getElementById('form_arquivo').submit();
}

</script>
<?

if($_REQUEST['tabela']) $_REQUEST['tabela']();

?>

<table cellspacing="0" cellpadding="3" border="0" bgcolor="#dcdcdc" align="center" class="tabela">
	<tr>
		<td style="text-align:center;"><input type="button" name="btn_salvar" id="btn_salvar" value="Salvar" onclick="uploadArquivos()"  /></td>
	</tr>
</table>