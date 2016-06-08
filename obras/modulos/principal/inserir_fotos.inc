
<script language="JavaScript" src="../../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<link rel="stylesheet" href="css/obras.css" type="text/css">

<?php 

$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo = "Monitoramento de Obras/Infraestrutura";
monta_titulo( $titulo_modulo, 'Inserir Fotos' );

?>

<br/>

<input type="button" name="selecionar_fotos" value="Selecionar Fotos">