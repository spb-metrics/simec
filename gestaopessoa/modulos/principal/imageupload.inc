<?php 
	if($_POST['id']) { 
		$dirName=APPRAIZ.'arquivos/gestaopessoa/uploads'; 
		if(!is_dir($dirName)) { 
			mkdir($dirName,0777);
		}
		$uploadFile="$dirName/".md5($_FILES['Arquivo']['name'].".demo");
		$_SESSION['sessionFILES'] = $_FILES['Arquivo'];  
		$_SESSION['session_temp_name'] = $uploadFile;
		if(!is_dir($_GET['dirname'])) {
			echo '<script> alert("Failed to find the final upload directory: $dirName);</script>';
		}
		if (!copy($_FILES['Arquivo']['tmp_name'], $dirName.'/'.md5($_FILES['Arquivo']['name'].".demo"))) {	
			echo '<script> alert("Failed to upload file");</script>';
		} 
		$_SESSION['gestaopessoa_vat']['arqmd5'] = md5($_FILES['Arquivo']['name'].".demo");
	} 
	else {
		$dirName=APPRAIZ.'arquivos/gestaopessoa/uploads'; 
		$uploadFile="$dirName/".md5($_GET['filename'].".demo");	 
		if (file_exists($_SESSION['session_temp_name'])) { 
			$content = '<img style="width: 3cm; height: 4cm;" src="verimagemupload.php?arqmd5='.$_SESSION['gestaopessoa_vat']['arqmd5'].'">'; 
			include_once( APPRAIZ . "gestaopessoa/classes/AjaxFileUploader.inc.php" );
			$ajaxFileUploader = new AjaxFileuploader($uploadDirectory="");	 
			echo $content; 
			echo "#"; 
			echo $ajaxFileUploader->showFileUploader('Arquivo'); 
			die();
		}
		else { 
			echo "&nbsp;<img src='/imagens/wait.gif' alt='loading...' />";
			die();
		}
	}   
?>