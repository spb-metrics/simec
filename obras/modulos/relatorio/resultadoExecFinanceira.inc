<?php

ini_set("memory_limit", "1024M");

// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

// instancia a classe de relatório
$rel = new montaRelatorio();

// monta o sql, agrupador e coluna do relatório
$sql       = obrasSqlExecFinanceira();
$agrupador = obrasAgpExecFinanceira();
$coluna    = obrasColunaExecFinanceira();
$dados 	   = $db->carregar( $sql );

$rel->setAgrupador($agrupador, $dados); 
$rel->setColuna($coluna);
$rel->setTotNivel(true);

// Gera o XLS do relatório
if ( $_REQUEST['tipoRelatorio'] == 'xls' ){
	ob_clean();
    $nomeDoArquivoXls = 'relatorio';
    echo $rel->getRelatorioXls();
    die;
}

?>

<html>
	<head>
		<title><?php echo $GLOBALS['parametros_sistema_tela']['nome_e_orgao']; ?></title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
	</head>
	<body>
		<center>
			<!--  Cabeçalho Brasão -->
			<?php echo monta_cabecalho_relatorio( '95' ); ?>
		</center>
		
		<!--  Monta o Relatório -->
		<? echo $rel->getRelatorio(); ?>
		
	</body>
</html>
