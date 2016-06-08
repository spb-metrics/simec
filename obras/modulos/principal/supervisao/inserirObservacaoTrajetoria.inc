<?php

monta_titulo( "Observação da Trajetória", "" );

?>


<html>
	<head>
		<title><?php echo $GLOBALS['parametros_sistema_tela']['nome_e_orgao']; ?></title>
		<script language="javascript"> 
		tmt_Move_WindowX = (screen.width - 510 ) / 2; 
		tmt_Move_WindowY = (screen.height - 500 ) / 2; 
		self.moveTo(tmt_Move_WindowX,tmt_Move_WindowY); 
		</script>
		
		<script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script type="text/javascript" src="../includes/prototype.js"></script>
	    <script type="text/javascript" src="../includes/entidades.js"></script>
	    <script type="text/javascript" src="/includes/estouvivo.js"></script>
	    <script type="text/javascript">
	    	
	    	function buscaObsTrajeto( entid ){
	    		document.getElementById('trjobservacao').value = window.opener.document.getElementById( 'trjobservacao_' + entid ).value;
	    	}
	    	
	    	function obrIncluiObs( entid ){
	    		window.opener.document.getElementById( 'trjobservacao_' + entid ).value = document.getElementById('trjobservacao').value;
	    		self.close();
	    	}
	    	
	    </script>
	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body topmargin="0">
		<form id="formulario" name="formulario" method="post" action="">
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
				<tr>
					<td>
						<?//= campo_textarea( 'trjobservacao', 'N', 'S', '', '94', '10', '1000', '' , 0, ''); ?>
						<?= campo_textarea( 'trjobservacao', 'N', 'S', '', 99, 16, 1000); ?>
					</td>
				</tr>
				<tr bgcolor="#D0D0D0">
					<td>
						<input type="button" value="Ok" onclick="obrIncluiObs( <?php print $_REQUEST["entid"]; ?> );" style="cursor: pointer;"/>
						<input type="button" value="Fechar" onclick="self.close();" style="cursor: pointer;"/>
					</td>
				</tr>
			</table>
		</form>
		<script>buscaObsTrajeto( <?php print $_REQUEST["entid"]; ?> );</script>
	</body>
</html>
