<?php 

// precisa de consertos... enquanto isso redireciona para o básico
header("location:component_foto_simples.php".(($_REQUEST['funcao'])?"?funcao=".$_REQUEST['funcao']:""));
exit;

include "conf_fotos.php";

// Pega o caminho atual do usuário (em qual módulo se encontra)
$caminho_atual = $_SERVER["REQUEST_URI"];
$posicao_caminho = strpos($caminho_atual, 'acao');
$caminho_atual = substr($caminho_atual, 0 , $posicao_caminho); 

// Pega url para os js
$posicao_caminho_js = strpos($caminho_atual, 'plugins');
$caminho_atual_js = substr($caminho_atual, 0 , $posicao_caminho_js);

?>

<html>
<head>
<script type="text/javascript"> var caminho_atual = '/obras/obras.php'; </script>
<script type="text/javascript" src="js/swfupload.graceful_degradation.js"></script>
<script type="text/javascript" src="js/swfupload.swfobject.js"></script>
<script type="text/javascript" src="js/swfupload.cookies.js"></script>
<script type="text/javascript" src="js/swfupload.queue.js"></script>
<script type="text/javascript" src="js/swfupload.js"></script>
<script type="text/javascript" src="js/utils.js"></script>
<? 
if($_REQUEST['funcao']){
	echo "<script> Criterio = new Acao(true);</script>";		
}else{
	echo "<script> Criterio = new Acao(false);</script>";
}
?>
<script type="text/javascript" src="js/dragdrop.js"></script>
<script type="text/javascript" src="../js/prototype.js"></script>
<script type="text/javascript" src="js/handlers.js"></script>
<link href="css/default.css" rel="stylesheet" type="text/css" />
<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>

<script type="text/javascript">
		var swfu;
		window.onload = function () {
			swfu = new SWFUpload({
			
				upload_url: "upload.php",
				post_params: {"PHPSESSID": ""},
				file_size_limit : "2048",	// 1MB
				file_types : "*.jpg",
				file_types_description : "JPG Images",
				file_upload_limit : 0,
				file_queue_limit : "21",
				file_post_name : "Filedata",
				// Button Settings
				button_image_url : "images/XPButtonUploadText_61x22.png",
				button_placeholder_id : "btnBrowse",
				button_width: 61,
				button_height: 22,

				file_queue_error_handler : fileQueueError,
				file_dialog_complete_handler : fileDialogComplete,
				upload_progress_handler : uploadProgress,
				upload_error_handler : uploadError,
				upload_success_handler : uploadSuccess,
				upload_complete_handler : uploadComplete,


				flash_url : "swfupload.swf",
				custom_settings : {
				upload_target : "divFileProgressContainer"
				
				},
				debug: false
			});
		};
		
	</script>
</head>
<body>
<br />
<? echo montarAbasArray_($abas, $_SERVER['REQUEST_URI']); ?>
<table class="listagem" align="center" width="100%">
<tr>
<td>
<input type="hidden" id="varauxiliar">
<div id="content">
	<br />
	<form>
		<span id="btnBrowse"></span> 
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="component_foto_simples.php<? echo (($_REQUEST['funcao'])?"?funcao=AtualizaFotos":""); ?>"><b>Problemas para inserir fotos? clique aqui</b></a>
	</form>
		<div id="divFileProgressContainer" style="height: 75px;"></div>
		
	<div id="thumbnails"></div>
</div>
<div id="dragDropContent"></div>
<div id="debug" style="clear:both"></div>
<div id="insertionMarker">
	<img src="imgs/marker_top.gif">
	<img src="imgs/marker_middle.gif" id="insertionMarkerLine">
	<img src="imgs/marker_bottom.gif">
</div>
<script>
	VerifyOpenerChildren();
</script>

</td>
</tr>
</table>


</body>

</html>
