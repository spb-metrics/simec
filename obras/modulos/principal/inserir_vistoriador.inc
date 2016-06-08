<?php
require_once APPRAIZ . "includes/classes/entidades.class.inc";

define("ID_VISTORIADOROBRAS", 47);
define("ID_RVISTORIA", 76);
define("ID_ROBRAS", 77);



if ($_REQUEST['opt'] == 'salvarRegistro') {
	$entidade = new Entidades();
	$entidade->carregarEntidade($_REQUEST);
	if($_REQUEST['funcoes']) {
		$entidade->adicionarFuncoesEntidade($_REQUEST['funcoes']);
		$entidade->salvar();
		switch($_REQUEST['funcoes']['funid']) {
			case ID_RVISTORIA:
				echo '<script type="text/javascript">
						window.opener.document.getElementById("entnomerespvistoria").innerHTML = \'' . $_REQUEST['entnome'] . '\';
						window.opener.document.getElementById("entidrespvistoria").value       = \'' . $entidade->getEntid() . '\';
						window.close();
					  </script>';
				
				break;
			case ID_ROBRAS:
				echo '<script type="text/javascript">
						window.opener.document.getElementById("entnomeresptecnico").innerHTML = \'' . $_REQUEST['entnome'] . '\';
						window.opener.document.getElementById("entidresptecnico").value       = \'' . $entidade->getEntid() . '\';
						window.close();
					  </script>';
				
				break;
			default:
			echo '<script type="text/javascript">
					window.opener.document.getElementById("entnomevistoriador").innerHTML = \'' . $_REQUEST['entnome'] . '\';
					window.opener.document.getElementById("entidvistoriador").value       = \'' . $entidade->getEntid() . '\';
					window.close();
				  </script>';
		}
		
	} else {
		echo '<script type="text/javascript">
				alert("Informações sobre entidade não enviadas corretamente. Refaça o procedimento.");
				window.close();
			  </script>';
	}
	
	exit;
}

if($_REQUEST['funid']) {
	$funid = $_REQUEST['funid'];
} else {
	$funid = ID_VISTORIADOROBRAS;
}

?>
<html>
<head>
<meta http-equiv="Cache-Control" content="no-cache">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="Connection" content="Keep-Alive">
<meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
<title><?= $titulo ?></title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<script type="text/javascript">
this._closeWindows = false;
</script>
</head>
<body style="margin:10px; padding:0; background-color: #fff; background-image: url(../imagens/fundo.gif); background-repeat: repeat-y;">
<?php

$entidade = new Entidades();
if($_REQUEST['entid'])
	$entidade->carregarPorEntid($_REQUEST['entid']);
echo $entidade->formEntidade("obras.php?modulo=principal/inserir_vistoriador&acao=A&opt=salvarRegistro",
							 array("funid" => $funid, "entidassociado" => null),
							 array("enderecos"=>array(1))
							 );

?>
<script type="text/javascript">
document.getElementById('frmEntidade').onsubmit  = function(e) {
	if (document.getElementById('entnumcpfcnpj').value == '') {
		alert('O CPF é obrigatório.');
		return false;
	}

	if (document.getElementById('entnome').value == '') {
		alert('O nome da entidade é obrigatório.');
		return false;
	}
	
	if ( document.getElementById('entdatanasc').value != '' && !validaData(document.getElementById('entdatanasc')) ) {
		alert('Formato de data inválido.');
		document.getElementById('entdatanasc').focus();
		document.getElementById('entdatanasc').value = '';
		return false;
	}
	return true;
}

</script>
</body>
</html>
