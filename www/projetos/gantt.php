<?php

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

$atiid = $_REQUEST['atiid'] == PROJETO ? null : (integer) $_REQUEST['atiid'];

?>
<html>
	<head>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<title>Gráfico de Gantt</title>
		<style type="text/css">
			span.imprimir { position: absolute; top: 3px; margin: 0; padding: 5px; position: fixed; background-color: #f0f0f0; border: 1px solid #909090; cursor:pointer; }
			span.imprimir:hover { background-color: #d0d0d0; }
		</style>
		<script type="text/javascript">
			self.focus();
		</script>
	</head>
	<body>
		<span class="imprimir notprint" onclick="window.print();"><img src="/imagens/print.gif"/></span>
		<img
			src="http://<?= $_SERVER['SERVER_NAME'] ?>/<?= $_SESSION['sisdiretorio'] ?>/ganttimagem.php?atiid=<?= $atiid ?>"
		/>
	</body>
</html>