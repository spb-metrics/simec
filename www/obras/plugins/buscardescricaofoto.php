<?
// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
// abre conexão com o servidor de banco de dados
$db = new cls_banco();
if(is_numeric($_REQUEST['arqid'])) {
	$sql = "SELECT arqdescricao FROM public.arquivo 
			WHERE arqid = '". $_REQUEST['arqid'] ."'";
	$dados = current($db->carregar($sql));
	echo $dados["arqdescricao"];
} elseif(is_readable("../../../arquivos/obras/imgs_tmp/".$_REQUEST['arqid'].".d")) {
	$descricao = file_get_contents("../../../arquivos/obras/imgs_tmp/".$_REQUEST['arqid'].".d");
	echo $descricao;
}
?>