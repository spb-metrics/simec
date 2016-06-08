<?php

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

if( $_REQUEST['arq_iclid'] && $_REQUEST['arq_tpvid'] && $_REQUEST['arq_entid'] )
{
	$sql = "SELECT 
				vldid 
			FROM 
				projetos.validacao 
			WHERE 
				iclid = ".$_REQUEST['arq_iclid']." 
				AND tpvid = ".$_REQUEST['arq_tpvid']."
				AND entid = ".$_REQUEST['arq_entid'];
	$vldid = $db->pegaUm($sql);
	
	if( $vldid )
	{
		$arrCampos = array("vldid" => $vldid);
		$file = new FilesSimec("anexochecklist", $arrCampos, "projetos");
			
		$sql = "SELECT arqid FROM projetos.anexochecklist WHERE vldid = ".$vldid." AND ancstatus = 'A'";
		$arqid = $db->pegaUm($sql);
		
		if( $arqid )
		{
			$sql = "UPDATE projetos.anexochecklist SET ancstatus = 'I' WHERE arqid = ".$arqid;
			$db->executar($sql);
			
			$sql = "UPDATE public.arquivo SET arqstatus = 'I' WHERE arqid = ".$arqid;
			$db->executar($sql);
			
			$db->commit();
			
			//$file->excluiArquivoFisico($arqid);
		}
		
		if( $_FILES['arquivo'] )
		{
			$arqdescricao = 'arquivo_checklist_enem_'.$vldid;
			
			if( $file->setUpload($arqdescricao, "arquivo") )
			{
				// ok...
			}
		}
	}
}

?>