<?php

ini_set("memory_limit", "1024M");

// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

/* Adaptando para retirar código HTML, pois o agrupador 
 * nomedaobra contem código HTML (<a ...>)
 * */

$chave = array_search('nomedaobra', $_REQUEST['agrupador']);
if($chave) $_REQUEST['agrupador'][$chave]="nomedaobraxls";
$chave = array_search('metragem', $_REQUEST['agrupador']);
if($chave) $_REQUEST['agrupador'][$chave]="metragemxls";
$chave = array_search('nivelpreenchimento', $_REQUEST['agrupador']);
if($chave) $_REQUEST['agrupador'][$chave]="nivelpreenchimentoxls";

/* FIM Adaptando para retirar código HTML, pois o agrupador 
 * nomedaobra contem código HTML (<a ...>)
 * */

$sql       = obras_monta_sql_relatio();
$agrupador = obras_monta_agp_relatorio();
$coluna    = obras_monta_coluna_relatorio();

$dados = $db->carregar( $sql );

$rel = new montaRelatorio();
$rel->setAgrupador($agrupador, $dados); 
$rel->setColuna($coluna);
$rel->setTotNivel(true);

$nomeDoArquivoXls = "SIMEC_Relat".date("YmdHis");
echo $rel->getRelatorioXls();
 
?>
