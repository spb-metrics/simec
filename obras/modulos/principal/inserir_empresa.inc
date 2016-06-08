<?
require_once APPRAIZ . "includes/classes/entidades.class.inc";

if( $_REQUEST["tipo"] == "supervisao" ){
	$tipo = "&tipo=supervisao";
}

if($_REQUEST['opt'] == 'salvarRegistro') {
	$entidade = new Entidades();
	$entidade->carregarEntidade($_REQUEST);
	$entidade->salvar();
	
	if( $_REQUEST["tipo"] == "supervisao" ){
		
		echo '<script type="text/javascript">
			      window.opener.document.getElementById("entnome").value = \'' . $_REQUEST['entnome'] . '\';
			      window.opener.document.getElementById("entid").value       = \'' . $entidade->getEntid() . '\';
			      window.close();
			  </script>';
		
	}else{
		
		echo '<script type="text/javascript">
			      window.opener.document.getElementById("entnomeempresa").innerHTML = \'' . $_REQUEST['entnome'] . '\';
			      window.opener.document.getElementById("entidempresa").value       = \'' . $entidade->getEntid() . '\';
			      window.close();
			  </script>';
			
	}
	
    exit;
    
}

header("Cache-Control: no-cache, must-revalidate");
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");

define("ID_EMPRESACONTRATADA", 46);
?>
<html>
<head>
<meta http-equiv="Cache-Control" content="no-cache">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="Connection" content="Keep-Alive">
<meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
<title><?= $titulo ?></title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>

<script type="text/javascript" src="/includes/estouvivo.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<script type="text/javascript">
this._closeWindows = false;
</script>
</head>
<body style="margin:10px; padding:0; background-color: #fff; background-image: url(../imagens/fundo.gif); background-repeat: repeat-y;">
<div>
<?php
$entidade = new Entidades();
if($_REQUEST['entid'])
	$entidade->carregarPorEntid($_REQUEST['entid']);
echo $entidade->formEntidade("obras.php?modulo=principal/inserir_empresa&acao=A&opt=salvarRegistro{$tipo}",
							 array("funid" => ID_EMPRESACONTRATADA, "entidassociado" => null),
							 array("enderecos"=>array(1))
							 );
?>
    </div>

<script type="text/javascript">
if(document.getElementById('tr_entcodent')){
	document.getElementById('tr_entcodent').style.display = 'none';
}
document.getElementById('tr_entnuninsest').style.display = 'none';
document.getElementById('tr_entungcod').style.display = 'none';
document.getElementById('tr_tpctgid').style.display = 'none';
document.getElementById('tr_entunicod').style.display = 'none';
/*
 * DESABILITANDO O NOME DA ENTIDADE
 */
document.getElementById('entnome').readOnly = false;
document.getElementById('entnome').className = 'normal';

if(document.getElementById('tr_entcodent')){
	document.getElementById('entrazaosocial').readOnly = false;
	document.getElementById('entrazaosocial').className = 'normal';
}
document.getElementById('entemail').readOnly = false;
document.getElementById('entemail').className = 'normal';

document.getElementById('entnome').onfocus = "";
document.getElementById('entnome').onmouseout = "";
document.getElementById('entnome').onblur = "";
document.getElementById('entnome').onkeyup = "";

$('frmEntidade').onsubmit  = function(e) {
	if (trim($F('entnumcpfcnpj')) == '') {
		alert('CNPJ é obrigatório.');
    	return false;
	}
	if (trim($F('entnome')) == '') {
		alert('O nome da entidade é obrigatório.');
		return false;
	}

<?php if( $_REQUEST["tipo"] == "supervisao" ): ?>
	if (trim($F('entemail')) == '') {
		alert('O email da entidade é obrigatório.');
		return false;
	}
<?php endif;?>
	
	return true;
}

</script>
  </body>
</html>
