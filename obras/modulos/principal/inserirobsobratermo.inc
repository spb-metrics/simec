<?php

// controle o cache do navegador
header( "Cache-Control: no-store, no-cache, must-revalidate" );
header( "Cache-Control: post-check=0, pre-check=0", false );
header( "Cache-control: private, no-cache" );   
header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
header( "Pragma: no-cache" );

// cria o objeto da classe termoDeAjuste
$termodeajuste = new termoDeAjuste();

// cria a variável com o ID do termo e do registro de obra, caso exista
$traid = !empty($_SESSION['obra']['traid']) ? $_SESSION["obra"]["traid"] : '';
$otaid = $_REQUEST["otaid"];

if ( $_REQUEST["requisicao"] == "cadastra" ){
	$termodeajuste->CadastraObsObrasTermo( $_REQUEST );
}

monta_titulo( 'Observações da Obra', '' );

?>

<html>
	<head>
		<title><?php echo $GLOBALS['parametros_sistema_tela']['nome_e_orgao']; ?></title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script type="text/javascript" src="../includes/prototype.js"></script>
	    <script type="text/javascript" src="../includes/entidades.js"></script>
	    <script type="text/javascript" src="/includes/estouvivo.js"></script>
	    <script src="/obras/js/obras.js"></script>
	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	</head>
	<body>
		<form action="" method="post" name="formulario" id="formulario">
			<input type="hidden" id="requisicao" name="requisicao" value="cadastra"/>
			<input type="hidden" id="carga" name="carga" value="<?php echo $_REQUEST["carga"]; ?>"/>
			<input type="hidden" id="traid" name="otaid" value="<?php echo $otaid; ?>"/>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
				<tr>
					<td class="subtitulodireita">Observação</td>
					<td>
						<?php
							$otaobs = $db->pegaUm( "SELECT otaobs FROM obras.obratermoajuste WHERE otaid = {$otaid}" ); 
							echo campo_textarea("otaobs", 'S', $somenteLeitura, '',73, 6, '500');
						?>
					</td>
				</tr>
				<tr bgcolor="#DCDCDC">
					<td></td>
					<td>
						<input type="button" value="Salvar" onclick="document.getElementById('formulario').submit();"/>
						<input type="button" value="Fechar" onclick="self.close();"/>
					</td>
				</tr>
			</table>
		</form>
	</body>
</html>
