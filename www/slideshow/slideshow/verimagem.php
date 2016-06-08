<?
// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . 'includes/workflow.php';
function ImgSize($imgend,$img_max_dimX,$img_max_dimY){
	$imginfo = getimagesize($imgend);
	$width = $imginfo[0];
	$height = $imginfo[1];
	if (($width >$img_max_dimX) or ($height>$img_max_dimY)){
		  if ($width > $height){
			  $w = $width * 0.9;
			  while ($w > $img_max_dimX){
				  $w = $w * 0.9;
			  }
			  $w = round($w);
			  $h = ($w * $height)/$width;
		  }else{
			  $h = $height * 0.9;
			  while ($h > $img_max_dimY){
				  $h = $h * 0.9;
			  }
			  $h = round($h);
			  $w = ($h * $width)/$height;
		  }
	}else{
		  $w = $width;
		  $h = $height;
	}
	$detalhes_foto['width'] = $w;
	$detalhes_foto['height'] = $h;
	return $detalhes_foto;
}
// abre conexão com o servidor de banco de dados
$db = new cls_banco();

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */


$sql = "SELECT arqtipo, arqid  FROM public.arquivo 
		WHERE arqid = '". $_REQUEST['arqid'] ."'";
$dados = $db->pegaLinha($sql);

if($dados) {
	
	// verifica se o arquivo existe antes de carrega-lo
	if(!is_file('../../../arquivos/'.(($_REQUEST["_sisarquivo"])?$_REQUEST["_sisarquivo"]:$_SESSION["sisarquivo"]).'/'. floor($dados['arqid']/1000) .'/'.$dados['arqid'])) {
		return false;
		exit;		
	}
	
	$expires = 3600;
	$cache_time = mktime(0,0,0,date('m'),date('d')+1,date('Y'));
	
	header("Expires: " . date("D, d M Y H:i:s",$cache_time) . " GMT");
	header("Cache-Control: max-age=$expires, must-revalidate");
	header('Content-type:'.$dados['arqtipo']);
	
	list($width, $height) = getimagesize('../../../arquivos/'.(($_REQUEST["_sisarquivo"])?$_REQUEST["_sisarquivo"]:$_SESSION["sisarquivo"]).'/'. floor($dados['arqid']/1000) .'/'.$dados['arqid']);
	
	if($_REQUEST['newwidth'] || $_REQUEST['newheight']) {
		$d = ImgSize('../../../arquivos/'.(($_REQUEST["_sisarquivo"])?$_REQUEST["_sisarquivo"]:$_SESSION["sisarquivo"]).'/'. floor($dados['arqid']/1000) .'/'.$dados['arqid'],$_REQUEST['newwidth'],$_REQUEST['newheight']);
		$thumb = imagecreatetruecolor($d['width'], $d['height']);
		switch($dados['arqtipo']) {
			
		case 'image/jpeg':
		$source = imagecreatefromjpeg('../../../arquivos/'.(($_REQUEST["_sisarquivo"])?$_REQUEST["_sisarquivo"]:$_SESSION["sisarquivo"]).'/'. floor($dados['arqid']/1000) .'/'.$dados['arqid']);
		// 	Resize
		imagecopyresized($thumb, $source, 0, 0, 0, 0, $d['width'], $d['height'], $width, $height);
		imagejpeg($thumb);	
		break;
		case 'image/gif':
		$source = imagecreatefromgif('../../../arquivos/'.(($_REQUEST["_sisarquivo"])?$_REQUEST["_sisarquivo"]:$_SESSION["sisarquivo"]).'/'. floor($dados['arqid']/1000) .'/'.$dados['arqid']);
		// 	Resize
		imagecopyresized($thumb, $source, 0, 0, 0, 0, $d['width'], $d['height'], $width, $height);
		imagegif($thumb);
		break;
		case 'image/png':
		$source = imagecreatefrompng('../../../arquivos/'.(($_REQUEST["_sisarquivo"])?$_REQUEST["_sisarquivo"]:$_SESSION["sisarquivo"]).'/'. floor($dados['arqid']/1000) .'/'.$dados['arqid']);
		// 	Resize
		imagecopyresized($thumb, $source, 0, 0, 0, 0, $d['width'], $d['height'], $width, $height);
		imagepng($thumb);
		break;
		default:
		readfile('../../../arquivos/'.(($_REQUEST["_sisarquivo"])?$_REQUEST["_sisarquivo"]:$_SESSION["sisarquivo"]).'/'. floor($dados['arqid']/1000) .'/'.$dados['arqid']);
		
		}
		//Clean-up memory
		ImageDestroy($thumb);
	} else {
		readfile('../../../arquivos/'.(($_REQUEST["_sisarquivo"])?$_REQUEST["_sisarquivo"]:$_SESSION["sisarquivo"]).'/'. floor($dados['arqid']/1000) .'/'.$dados['arqid']);
	}
}
?>