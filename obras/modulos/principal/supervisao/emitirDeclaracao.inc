<?php

$obDeclaracao = new DeclaracaoController();
$obDeclaracao->ativaDadosDeclaracao( array("dclid", "arqid"), $_GET['dclid'] );
$obDeclaracao->ativaDadosArquivo( array("arqextensao", "arqnome") );

$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($obDeclaracao->dclid/1000) .'/';
$name    = $obDeclaracao->arqnome . '.' . $obDeclaracao->arqextensao;

if( file_exists($caminho . $name) )
{
	echo file_get_contents( $caminho . $name );
}
else
{
	echo "<center><font style='color:red;'>Não foi encontrada a Declaração.</font></center>";
}

die;

?>