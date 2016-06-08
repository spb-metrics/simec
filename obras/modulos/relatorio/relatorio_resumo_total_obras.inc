<?php

function apaga_files($name = null)
{
	define("CAMINHO",APPRAIZ."arquivos/obras/file/cache/");
	
	$dir = CAMINHO;
            
    if(is_dir($dir))
    {
        if($handle = opendir($dir))
        {
            while(($file = readdir($handle)) !== false)
            {
//                echo $file." <br />";
                if($file != '.' && $file != '..' && $file != '.svn')
                {
                    if( $file != $name)
                    {
                        unlink($dir.$file);
                    }
                }
            }
        }
    }
    else
    {
        die("Erro ao abrir dir: $dir");
    }
	return 0;
}

if( $_REQUEST['atualiza'] == true ){
	apaga_files(".html");
}

function cacheFile($arrREQUEST)
{
	unset($arrREQUEST['PHPSESSID']);
	
	define("CAMINHO",APPRAIZ."arquivos/obras/file/cache");
	
	if(is_array($arrREQUEST)){
		foreach($arrREQUEST as $keyCache => $arrRequest){
			if( $keyCache != 'atualiza'){
				$string .= $keyCache.$arrRequest;
			}
		}
		$cacheFile = md5($string.$_SERVER['REQUEST_URI']);
		
		if(!is_file(CAMINHO."/$cacheFile.html")){
			$cacheFileCriar = true;
		}else{
			$cacheFileCriar = false;
		}
	}
	if($cacheFileCriar && $cacheFile){
		if(is_dir(CAMINHO)){
			$cacheConteudo = ob_get_contents();
			$cacheArquivo = fopen(CAMINHO."/$cacheFile.html", "a");
			fwrite($cacheArquivo, $cacheConteudo);
			fclose($cacheArquivo);
		}else{
			mkdir(str_replace("/file/cache","/file",CAMINHO), 0777);
			mkdir(CAMINHO,0777);
			$cacheConteudo = ob_get_contents();
			$cacheArquivo = fopen(CAMINHO."/$cacheFile.html", "a");
			fwrite($cacheArquivo, $cacheConteudo);
			fclose($cacheArquivo);
		}
	}
}

$arrREQUEST = $_REQUEST;
unset($arrREQUEST['PHPSESSID']);
foreach($arrREQUEST as $keyCache => $arrRequest){
	$string .= $keyCache.$arrRequest;
}

$cacheFile = md5($string.$_SERVER['REQUEST_URI']);
if(is_file(APPRAIZ."arquivos/obras/file/cache/$cacheFile.html") && $_REQUEST['atualiza'] != true){
	$cacheFileCriar = false;
	include(APPRAIZ."arquivos/obras/file/cache/$cacheFile.html");
	exit;
}else{
	$cacheFileCriar = true;
}

if($cacheFileCriar){
	register_shutdown_function('cacheFile',$_REQUEST);
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
$db->cria_aba($abacod_tela,$url,'');
monta_titulo( $titulo_modulo, '' );

$rel = new obrasRelatorioResumoTotal();
?>
<form name="formResumo" id="formResumo" method="post" name="filtro"> 
	<br />
	<div align="right" style="margin-right:35px; float: right; cursor: pointer; width:110px;" onclick="atualizaRel()">
		<input type="hidden" name="atualiza" id="atualiza" value="false"/>
		Atualizar Relatório <img src="../../imagens/obras/atualizar.png" style="cursor: pointer"  border="0" title="Atualizar Relatório" />
	</div>
	<?php $rel->monta_cabecalho_relatorio_painel();?>
	<br />
	<?php $rel->resumoObrasBrasil( 0 );?>
	<br />
	<div align="center" width="95%" style="text-align: left; margin-left:35px; font-size:14px;">
		<strong> Detalhamento por tipo de estabelecimento prisional </strong>
	</div>
	<br />
	<?php $rel->resumoObrasBrasil( 1 );?>
	<br />
	<?php $rel->resumoObrasBrasil( 2 );?>
	<br />
	<?php $rel->resumoObrasBrasil( 3 );?>
</form>
<script>

function atualizaRel(){
	var atualiza = document.getElementById('atualiza').value;
	var form     = document.getElementById('formResumo');

	atualiza = true;
	form.submit();
}
</script>
