<?php

ini_set("memory_limit", "1024M");

// Inclui componente de relat�rios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

/* Adaptando para retirar c�digo HTML, pois o agrupador 
 * nomedaobra contem c�digo HTML (<a ...>)
 * */
//$chave = array_search('nomedapessoa', $_REQUEST['agrupador']);
//$_REQUEST['agrupador'][$chave]="nomedapessoaxls";
/* FIM Adaptando para retirar c�digo HTML, pois o agrupador 
 * nomedaobra contem c�digo HTML (<a ...>)
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
