<html>
	<head>
		<title><?php echo $GLOBALS['parametros_sistema_tela']['nome_e_orgao']; ?></title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
<?php
$obOS = new OSController();
$obOS->ativaDadosGrupo( array('gpdid'), $_GET['gpdid'] );
?>
	<body marginwidth="0" marginheight="0">
		<table class="tabela" bgcolor="#ffffff" cellSpacing="1" cellPadding=3 align="center" style="width:100%">
			<tr>
				<td colspan="2">
					<? $obOS->listaObrasGrupo(); ?>
					<? $obOS->listaRotasGrupo(); ?>
				</td>
			</tr>
			<tr bgcolor="#D0D0D0">
				<td colspan="2">
					<input type="button" value="Fechar" style="cursor: pointer;" onclick="self.close();"/>
				</td>
			</tr>
		</table>
	</body>
</html>
