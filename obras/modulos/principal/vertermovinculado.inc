<?php

// cria o objeto da classe termoDeAjuste
$termodeajuste = new termoDeAjuste();

$traid = $_REQUEST["traid"];

if ( !$traid ){
	print "<script>"
		. "		alert('Nenhum termo foi selecionado!');"
		. "		self.close();"
		. "</script>";
	die;
}

monta_titulo( "Termo de Ajuste Vinculado", "");

?>
<html>
	<head>
	    <meta http-equiv="Cache-Control" content="no-cache">
	    <meta http-equiv="Pragma" content="no-cache">
	    <meta http-equiv="Connection" content="Keep-Alive">
	    <meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
	    <title><?php echo $GLOBALS['parametros_sistema_tela']['nome_e_orgao'];?></title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script type="text/javascript" src="/includes/estouvivo.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	</head>
	<body>
		<?php

			$termodeajuste->MontaDetalhesTermo( $traid );
		
		?>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr bgcolor="#C0C0C0">
				<td>
					<input type="button" value="Fechar" style="cursor:pointer;" onclick="self.close();"/>
				</td>
			</tr>
		</table>
	</body>
</html>
