<?
// inicializa sistema
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
$db = new cls_banco();

$modulo=$_REQUEST['modulo'] ;//

if ($_REQUEST['email']){
if (ereg_replace("<[^>]*>","",$_REQUEST['email']) == '')
{
	   ?>
	      <script>
	         alert ('O texto do e-mail n�o pode estar vazio.');
	         history.back();
	      </script>
	   <?
	     exit();
}
else
{
  // envia email
  $sql="select usunome,usuemail from usuario where usucpf='".$_REQUEST['cpf']."'";
  $RSu = $db->record_set($sql);
  $resu =  $db->carrega_registro($RSu,0);
  if(is_array($resu)) foreach($resu as $k=>$v) ${$k}=$v;
  $assunto = $_REQUEST['assunto'];
  $mensagem = $_REQUEST['email'];
  $cc=$_REQUEST['cc'];
  $cco=$_REQUEST['cco'];
  email(str_to_upper($usunome), $usuemail, $assunto, $mensagem,$cc,$cco);
  $sql="select usunome,usuemail from usuario where usucpf='".$_SESSION['usucpf']."'";
  $RSu = $db->record_set($sql);
  $resu =  $db->carrega_registro($RSu,0);
  if(is_array($resu)) foreach($resu as $k=>$v) ${$k}=$v;
  email(str_to_upper($usunome), $usuemail, $assunto, $mensagem);
  ?>
      <script>
         alert('Email enviado com sucesso. Esta janela ser� fechada.')
         window.close();
      </script>
  <?
  exit();

}
}
    $sql="select u.usunome,o.orgdsc from usuario u left join orgao o on u.orgcod = o.orgcod where u.usucpf='".$_REQUEST['cpf']."'";
    $RSu = $db->record_set($sql);
    $resu =  $db->carrega_registro($RSu,0);
    if(is_array($resu)) foreach($resu as $k=>$v) ${$k}=$v;

?>
<html>
<head>
<title>Envio de Email</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<script language="JavaScript" src="../includes/funcoes.js"></script>


</head>
<body bgcolor="#ffffff" leftmargin="0" rightmargin="0" topmargin="0" bottommargin="0" marginheight="0" marginwidth="0">
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="cpf" value="<?=$_REQUEST['cpf']?>">

    <center>
    <table width='100%' align='center' border="0" cellspacing="1" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
     <tr>
	 <td colspan="2" align="Center" bgcolor="#dedede">Enviar Email</td>
	 </tr>
	  <tr>
        <td align="right" class="subtitulodireita">Para:</td> 
        <td><?=campo_texto('usunome','N','N','',70,100,'','');?></td>
     </tr>
	  <tr>
        <td align="right" class="subtitulodireita">�rg�o:</td> 
        <td><?=campo_texto('orgdsc','N','N','',70,100,'','');?></td>
     </tr>
	  <tr>
        <td align="right" class="subtitulodireita">Cc:</td> 
        <td><?=campo_texto('cc','N','S','',70,100,'','');?></td>
     </tr>
     <tr>
        <td align="right" class="subtitulodireita">Cco:</td> 
        <td><?=campo_texto('cco','N','S','',70,100,'','');?></td>
     </tr>     
	  <tr>
        <td align="right" class="subtitulodireita">Assunto:</td> 
        <td><?=campo_texto('assunto','S','S','',70,100,'','');?></td>
     </tr>
	  <tr>
        <td align="right" class="subtitulodireita">ATEN��O:</td> 
        <td><b><font color="red">EVITE COPIAR TEXTOS FORMATADOS DO WORD PORQUE SE O DESTINAT�RIO UTILIZAR O OUTLOOK, A MENSAGEM PODE FICAR CONFUSA E ININTELIG�VEL!</font></td>
     </tr>     
        <? $email= '';?>
        <td colspan=2><?=campo_textarea('email','N','S','','95%',22,'');?></td>
     </tr>
	 <tr>
	 <td colspan="2" align="right" class="subtitulodireita"><input type='button' class="botao" value='Enviar E-mail' onclick="envia_email()">&nbsp;&nbsp;&nbsp;<input type='button' class="botao" value='Fechar' onclick="fechar_janela()"></td>
	 </tr>
  </table>
</form> 
<script>
  function fechar_janela()
  {
    window.close();

  }
    function envia_email()
  {

  	if (!validaBranco(document.formulario.assunto, 'Assunto')) return;
	//verifica��o do campo corpo email
	//document.formulario.email.value = email.getContent('email');
	if (!validaBranco(document.formulario.email, 'Texto da Mensagem')) return tinyMCE.execCommand('mceFocus', true, 'email');
	
	document.formulario.submit();

  }

</script>
</body>
</html>
