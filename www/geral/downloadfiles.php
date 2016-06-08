<?

session_start();

// carrega as fun��es gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
// abre conex�o com o servidor de banco de dados
$db = new cls_banco();

function deletararquivos($files) {
	foreach($files as $fl) {
		unlink($fl);
	}
}

function processararquivos($files,$orig,$dest) {
	if($files[0]) {
		if(!is_dir("../../arquivos/".$dest."/files_tmp/")) {
			mkdir("../../arquivos/".$dest."/files_tmp/");
		}
		foreach($files as $f) {
			$endorigem =  "../../arquivos/".$orig."/".floor($f['arqid']/1000)."/".$f['arqid'];
			$enddestino = "../../arquivos/".$dest."/files_tmp/".$f['arqid'].".".$f['arqextensao'];
			if(file_exists($endorigem)) {
				if(copy($endorigem, $enddestino)) {
					$fzip[] = $enddestino; 	
				}
			}
		}
	}
	return $fzip;
}


/*
 * $_REQUEST[fotosselecionadas] : Array de ids da tabela public.arquivo, 
 * esses arquivos passados ser�o compactados em um arquivo .ZIP 
 */
if(count($_REQUEST['fotosselecionadas']) > 0) {
	include('../../includes/pclzip-2-6/pclzip.lib.php');
	$files = $db->carregar("SELECT arqid, arqextensao FROM public.arquivo WHERE arqid IN('".implode("','",$_REQUEST['fotosselecionadas'])."')");
	$filezip = processararquivos($files,$_SESSION['downloadfiles']['pasta']['origem'],$_SESSION['downloadfiles']['pasta']['destino']);
	$nomearquivozip = $_SESSION['usucpf'].'_'.date('dmyhis').'.zip';
	$enderecozip = '../../arquivos/'.$_SESSION['downloadfiles']['pasta']['destino'].'/files_tmp/'.$nomearquivozip;
	$archive = new PclZip($enderecozip);
	$archive->create( $filezip,  PCLZIP_OPT_REMOVE_ALL_PATH);
	if($filezip) deletararquivos($filezip);
/*
 * $_REQUEST[enderecoabsolutoarquivo] : Se possui essa variavel, 
 * o programa vai pegar o arqid de apenas um arquivo, 
 * e fazer com que o usuario fa�a o download na extens�o original 
 */
} elseif($_REQUEST['enderecoabsolutoarquivo']) {
	if($_REQUEST['arqid']) {
		$files = $db->carregar("SELECT arqid, arqextensao FROM public.arquivo WHERE arqid IN('".$_REQUEST['arqid']."')");
		$filezip = processararquivos($files,$_SESSION['downloadfiles']['pasta']['origem'],$_SESSION['downloadfiles']['pasta']['destino']);
		if(count($filezip) > 0) {
			$files = $files[0];
			$nomearquivozip = $files['arqid'].'.'.$files['arqextensao'];
			$enderecozip = current($filezip);
		} else {
			echo "<script>alert('Erro no download. Entre em contato com a equipe t�cnica.');window.close();</script>";
			exit;
		}
	} else {
		echo "<script>alert('Erro no download. Entre em contato com a equipe t�cnica.');window.close();</script>";
		exit;
	}
} else {
	echo "<script>alert('N�o foi selecionado nenhuma foto.');window.close();</script>";
	exit;
}

if(is_file($enderecozip)) {

header("Content-Disposition: attachment; filename=".$nomearquivozip);
header("Content-Type: application/oct-stream");
header("Expires: 0");
header("Pragma: public");
header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
readfile($enderecozip);

} else {
	echo "<script>alert('Arquivo n�o encontrado');</script>";
}

echo "<script>window.close();</script>";

?>