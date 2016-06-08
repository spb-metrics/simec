<?php

ini_set("memory_limit", "1024M");

// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

/* Adaptando para retirar código HTML, pois o agrupador 
 * nomedaobra contem código HTML (<a ...>)
 * */
//$chave = array_search('nomedapessoa', $_REQUEST['agrupador']);
//$_REQUEST['agrupador'][$chave]="nomedapessoaxls";
/* FIM Adaptando para retirar código HTML, pois o agrupador 
 * nomedaobra contem código HTML (<a ...>)
 * */

$sql       = ft_monta_sql_relatorio();
$agrupador = ft_monta_agp_relatorio();
$coluna    = ft_monta_coluna_relatorio();

$dados = $db->carregar( $sql );

$rel = new montaRelatorio();
$rel->setAgrupador($agrupador, $dados); 
$rel->setColuna($coluna);
$rel->setTotNivel(true);

$nomeDoArquivoXls = "SIMEC_Relat".date("YmdHis");
echo $rel->getRelatorioXls();
 
?>
