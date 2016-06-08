<?
/*
session_start();
if($_SESSION["img_atual"]){
	echo "tem";
	if($_SESSION["img_atual"] == $_REQUEST["img"]){
		echo "igual";
		$_REQUEST["img"] = $_REQUEST["img"];		
	}else{
		$_REQUEST["img"] = "../../../arquivos/obras/documentos/".$_SESSION["img_atual"];	
	}
}else{
	echo "nao";
	$_REQUEST["img"] = $_REQUEST["img"];
}
*/
?>
<html>
<head>
<link rel="stylesheet" href="../css/obras.css">
<script type="text/javascript" src="../js/prototype.js"></script>
<script type="text/javascript" src="../js/documentos.js"></script>
</head>
<body>
<iframe name="main_image" id="main_image" height="100%" width="100%" src="resize.php?img=<? print($_REQUEST["img"]); ?>&w=800&h=600" scrolling="no"></iframe> 
<!-- 
<div id="image_left"><a href="#" onclick="FlipPhoto('<? // echo $_REQUEST["img"]; ?>',-1);"> ANTERIOR </a></div>
<div id="image_right"><a href="#" id="tete" onclick="FlipPhoto('<? //echo $_REQUEST["img"]; ?>',1);">PRÓXIMA</a></div>
 -->
</body>
</html>