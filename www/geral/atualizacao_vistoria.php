<?php
include_once "config.inc";
include_once APPRAIZ."includes/funcoes.inc";
include_once APPRAIZ."www/obras/_constantes.php";
include_once APPRAIZ."www/obras/_funcoes.php";
include_once APPRAIZ."includes/workflow.php";
include_once APPRAIZ."includes/classes_simec.inc";

$db = new cls_banco();

$sql = "SELECT supvid FROM obras.supervisao WHERE docid IS NULL";
$dados = $db->carregarColuna($sql);

if(!empty($dados)){
	foreach($dados AS $i => $dado ){
		$docid = pegarDocidSupervisao( $dado );
		$obrid = $db->pegaUm("SELECT obrid FROM obras.supervisao WHERE docid = {$docid}");
		wf_alterarEstado( $docid, WF_ACAO_ENVIAR_PARA_VALIDADO , '', array('obrid' => $obrid));
	}
	
	$msg = "Registros atualizados com sucesso!";	

}else{
	$msg = "Não Existem Registros sem docid!";
}

die("<script>
	alert('{$msg}');
 </script>");