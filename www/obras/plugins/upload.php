<?php
include "config.inc";
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
require_once("class/ClassImage.php");

$upload = new ClassImage();

$storage = "../../../arquivos/obras/imgs_tmp";

switch(strtolower((end(explode(".", $_FILES['Filedata']['name']))))) {
	case 'gif':
		$_FILES['Filedata']['type'] = 'image/gif';
		break;
	case 'jpg':
	case 'jpeg':
		$_FILES['Filedata']['type'] = 'image/jpeg';
		break;
	case 'png':
		$_FILES['Filedata']['type'] = 'image/png';
		break;
	case 'bmp':
		$_FILES['Filedata']['type'] = 'image/bmp';
		break;
	
}
$foto_name = str_replace("/","",substr(md5_encrypt(tirar_acentos($_FILES['Filedata']['name'][$i]))."__extension__".md5_encrypt($_FILES['Filedata']['type'][$i])."__temp__".date('YmdHis').rand(1,10000).md5_encrypt(tirar_acentos($_FILES['Filedata']['name'][$i])),0,150));

$uploadfile = "$storage/$foto_name";

$uploaded = $upload->reduz_imagem($_FILES['Filedata']['tmp_name'],640,480,$uploadfile,strtolower((end(explode(".", $_FILES['Filedata']['name'])))));

if($uploaded)
	echo($foto_name);
?>