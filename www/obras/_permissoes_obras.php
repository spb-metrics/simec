<?php

include_once APPRAIZ . "www/obras/_funcoes.php";

$existe_obra = obras_verificaobras($_REQUEST["obrid"]);

if(!$existe_obra){
	echo "<script>
			alert('Esta obra não existe!');
			history.back(-1);
		  </script>";
	die;
}

$possui_permisao = obras_verificapermissao($_REQUEST["obrid"]);

if(!$possui_permisao){
	echo "<script>
			alert('Você não possui permissão para ver esta obra!');
			history.back(-1);
		  </script>";
	die;
}

?>