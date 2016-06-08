<html>
<head></head>
<body bgcolor=#ffffff vlink=#666666 bottommargin="0" topmargin="0" marginheight="0" marginwidth="0" rightmargin="0" leftmargin="0">
<?php
/*
   Sistema Simec
   Setor respons�vel: SPO-MEC
   Desenvolvedor: Equipe Simec
   Analista: Alexandre Soares Diniz
   Programador: Alexandre Soares Diniz
   M�dulo:solicitar.inc
   Finalidade: permite que alguem solicite usu�rios do simec
 */

 	include "config.inc";
 	include APPRAIZ."includes/classes_simec.inc";
 	include APPRAIZ."includes/funcoes.inc";
 	$db = new cls_banco();
 	include "cabecalho.php";
?>

<?

print '<br>';

//print_r($_REQUEST);
//exit();
?>
<br>
<?
$titulo_modulo='Ficha de Solicita��o de Cadastro de Usu�rios';
$subtitulo_modulo='Preencha os Dados Abaixo e clique no bot�o: "Enviar Solicita��o".<br>'.obrigatorio().' Indica Campo Obrigat�rio.';
monta_titulo($titulo_modulo,$subtitulo_modulo);

$codigo = $_POST['codigo'];	
$usucpf = $_POST['usucpf'];	

?>

<table width='95%' align='center' border="0" cellspacing="1" cellpadding="3" style="border: 1px Solid Silver; background-color:#f5f5f5;">
<form method="POST" name="formulario">
<input type=hidden name="modulo" value="./inclusao_usuario.php">
	<tr bgcolor="#F2F2F2">
   	 	<td align = 'right' class="subtitulodireita">Sistema:</td>
    	<td>
    <?	  
        $sql = "select s.sisid as codigo, s.sisabrev as descricao from seguranca.sistema s where s.sisstatus='A' and sismostra='t'";
        $db->monta_combo("codigo",$sql,'S',"Selecione o sistema desejado",'exibe_dsc','');
    ?>
  &nbsp;<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigat&oacute;rio.'> </td>
 	</tr>
 	<? if($codigo!=""){ ?>
	<tr id = "desc">
		<td align='right' class="subtitulodireita">Descri��o:</td>
		<td>
		<?
			if ($codigo !="")
			{
				$sql = "select sisdsc,sisfinalidade from sistema where sisid = ".$codigo;
				$sisdsc = $db->pegaUm($sql,0);
				$sisfinalidade_selc = $sisdsc." - ".$db->pegaUm($sql,1);
				print $sisfinalidade_selc;
			}else{
				print "-";
			}
		?>
		</td>
	</tr>
	<?}?>
	<input type=hidden name="sisfinalidade_selc" value=<?=$sisfinalidade_selc?>>
 	<tr>
    	<td align='right' class="subtitulodireita">CPF:</td>
		<td><input type="text" name="usucpf" size="20" maxlength="14" value=<? print '"'.$usucpf.'"'; ?> class="normal" onKeyUp= "this.value=mascaraglobal('###.###.###-##',this.value);" onmouseover="MouseOver(this);" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="text-align : left; width:22ex;" title=''>&nbsp;<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigat�rio.'></td>
	</tr>
	<tr bgcolor="#C0C0C0">
 		<td></td>
   		<td><input type="button" name="btinserir" value="Enviar Solicita&ccedil;&atilde;o"  onclick="validar_solicitacao()">&nbsp;&nbsp;&nbsp;<input type="Button" value="Voltar" onclick="history.back();"></td>
	</tr>
	
</form>
</table>
<br>
</body>
</html>
<?php
 	include "rodape.php";
?>
<script>

//document.getElementById("desc").style.visibility = "hidden";
//document.getElementById("desc").style.display = "none";

function exibe_dsc() {

	//document.formulario.modulo.value = "sistema/usuario/solicitar";
	document.formulario.submit();

}


function validar_solicitacao() {
    e = document.formulario.usucpf;
    s = FiltraCampo(e.value);
   if (document.formulario.codigo.value == "") {
		alert('Selecione um Sistema para ter acesso.');
		document.formulario.codigo.focus();
		return;
	}
    if (document.formulario.usucpf.value == '') {
		alert('O CPF precisa ser preenchido corretamente.');
		e.focus();
		return;
	}
	if (! DvCpfOk(document.formulario.usucpf))
	{
		e.focus();
	    return;
	}	
	
	document.formulario.action = document.formulario.modulo.value;
	//alert (document.formulario.modulo.value);
	document.formulario.submit();
	
}

</script>

