<?
   	require_once "config.inc";
   	$tot=$_REQUEST['tot'];
   	header( "Cache-Control: no-store, no-cache, must-revalidate" );
	header( "Cache-Control: post-check=0, pre-check=0", false );
	header( "Cache-control: private, no-cache" );
	header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
	header( "Pragma: no-cache" );
	//ader( "Content-type: text/plain; charset=iso-8859-1" );
   	
   	
   	?>
<html>

<head>
<title>Pop-up informa Mensagens</title>
</head>

<body bgcolor="#FFCC00">

<p><b><font face="Verdana">Sr. Usu�rio</font></b></p>
<p><b><font face="Verdana">Informamos que voc� possui <?=$tot?> mensagens n�o lidas em 
sua caixa de mensagens.</font></b></p>
<p><b><font face="Verdana">Para ter acesso � caixa, clique sobre o �cone
<img border="0" src="/imagens/email.gif" width="14" height="11"> 
que fica localizado na parte superior direita da tela.</font></b></p>
<p><b><font face="Verdana">N�o deixe de ler suas mensagens.</font></b></p>
<p><b><font face="Verdana">Elas podem estar associadas a eventos gerenciais dentro do sistema.</font></b></p>

</body>

</html>