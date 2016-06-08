<?php

function EnviarArquivo(){
	global $db;

	// obtém o arquivo
	$arquivo = $_FILES['arquivo'];

	if ( !is_uploaded_file( $arquivo['tmp_name'] ) ) {
		?>
			<script>
				location.href='obras.php?modulo=<?=$_REQUEST['modulo']?>&acao=<?= $_REQUEST['acao']?>';
			</script>
		<?
	}
	// BUG DO IE
	// O type do arquivo vem como image/pjpeg
	if($arquivo["type"] == 'image/pjpeg') {
		$arquivo["type"] = 'image/jpeg';
	}
	//Insere o registro do arquivo na tabela public.arquivo
	$sql = "INSERT INTO public.arquivo 	(arqnome,arqextensao,arqdescricao,arqtipo,arqtamanho,arqdata,arqhora,usucpf,sisid,arqstatus)
	values('".current(explode(".", $arquivo["name"]))."','".end(explode(".", $arquivo["name"]))."','".$_REQUEST["arqdescricao"]."','".$arquivo["type"]."','".$arquivo["size"]."','".date('d/m/Y')."','".date('H:i:s')."','".$_SESSION["usucpf"]."',". $_SESSION["sisid"] .",'A') RETURNING arqid;";
	$arqid = $db->pegaUm($sql);

	//Insere o registro na tabela obras.anexosprojeto
	$sql = "INSERT INTO obras.anexosprojeto (arqid,prjid,anpdsc,anpstatus,anpdtinclusao) VALUES ($arqid,{$_SESSION['prjid']},'{$_REQUEST['arqdescricao']}','A','now()')";
	$db->executar($sql);
	
	$ordem = 1;
	
	//Insere o registro na tabela obras.fotosprojeto
	$sql = "INSERT INTO obras.fotosprojeto(prjid,arqid,ftpdesc,ftpordem) VALUES ({$_SESSION['prjid']},$arqid,'{$_REQUEST['arqdescricao']}',$ordem)";
	$db->executar($sql);


	if(!is_dir('../../arquivos/obras/'.floor($arqid/1000))) {
		mkdir(APPRAIZ.'/arquivos/obras/'.floor($arqid/1000), 0777);
	}
	$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arqid/1000) .'/'. $arqid;
	switch($arquivo["type"]) {
		case 'image/jpeg':
			ini_set("memory_limit", "128M");
			list($width, $height) = getimagesize($arquivo['tmp_name']);
			$original_x = $width;
			$original_y = $height;
			// se a largura for maior que altura
			if($original_x > $original_y) {
  	 			$porcentagem = (100 * 640) / $original_x;      
			}else {
   				$porcentagem = (100 * 480) / $original_y;  
			}
			$tamanho_x = $original_x * ($porcentagem / 100);
			$tamanho_y = $original_y * ($porcentagem / 100);
			$image_p = imagecreatetruecolor($tamanho_x, $tamanho_y);
			$image   = imagecreatefromjpeg($arquivo['tmp_name']);
			imagecopyresampled($image_p, $image, 0, 0, 0, 0, $tamanho_x, $tamanho_y, $width, $height);
			imagejpeg($image_p, $caminho, 100);
			//Clean-up memory
			ImageDestroy($image_p);
			//Clean-up memory
			ImageDestroy($image);
			break;
		default:
			if ( !move_uploaded_file( $arquivo['tmp_name'], $caminho ) ) {
				$db->rollback();
				echo "<script>alert(\"Problemas no envio do arquivo.\");</script>";
				exit;
			}
	}

	$db->commit();
}

if($_REQUEST['requisicao'] == 'inserirarquivo' && $_FILES['arquivo'] && $_REQUEST['arqdescricao']) {
		EnviarArquivo();
		print "<script>alert('Operação Realizada com Sucesso!');</script>";
}

if(!$_SESSION['prjid'] && !$_REQUEST['verFotos']){
	header( "Location: obras.php?modulo=principal/biblioteca_projeto&acao=A" );
	exit();
}

if(!$_SESSION['prjid'] && $_REQUEST['verFotos']){
	$_SESSION['prjid'] = $_REQUEST['verFotos'];
	header( "Location: obras.php?modulo=principal/album_projeto&acao=A" );
	exit();
}


include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo = "Galeria de Fotos";
monta_titulo( $titulo_modulo, 'Clique na imagem para vê-la em tamanho original' );
?>

<style type="text/css">
	table.tabela{
		background-color: #f5f5f5;
	}	
</style>

<script src="/obras/js/documentos.js" ></script>

<form method="post" name="anexo" enctype="multipart/form-data" onsubmit="return ValidarFormulario(this);" action="<?php echo $caminho_atual;?>acao=A">
	<input type="hidden" name="requisicao" value="inserirarquivo"/>		
	<table class="tabela" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
			<td>
					<input type="file" name="arquivo"/>
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
			<td><?= campo_textarea( 'arqdescricao', 'S', $somenteLeitura, '', 60, 2, 250 ); ?></td>
		</tr>
		<tr style="background-color: #cccccc">
			<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
			<td>
				<input type="hidden" name="tpaid" id="tpaid" value="21"/>
					<input type="submit" name="botao" value="Salvar"/>
			</td>
		</tr>
	</table>
	<br/>
	<table class="tabela" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td>
<?
$sql = "SELECT arq.arqnome, arq.arqid, arq.arqextensao, arq.arqtipo, arq.arqdescricao FROM public.arquivo arq
		INNER JOIN obras.fotosprojeto fp ON arq.arqid = fp.arqid  
		WHERE fp.prjid = {$_SESSION["prjid"]} AND
			  arq.arqstatus = 'A' AND
			  (arq.arqtipo = 'image/jpeg' OR 
			   arq.arqtipo = 'image/gif' OR 
			   arq.arqtipo = 'image/png') 
		ORDER BY arq.arqid
		LIMIT 16 OFFSET ".($_REQUEST['pagina']*16);
$fotos = ($db->carregar($sql));
$_SESSION['downloadfiles']['pasta'] = array("origem" => "obras","destino" => "obras");
if($fotos){
	$_SESSION['imgparams'] = array("filtro" => "cnt.prjid=".$_SESSION["prjid"]." AND arq.arqstatus = 'A'", "tabela" => "obras.fotosprojeto");
	for($k=0;$k < count($fotos);$k++){
		echo "<img id='".$fotos[$k]["arqid"]."' src='../slideshow/slideshow/verimagem.php?newwidth=64&newheight=48&arqid=".$fotos[$k]["arqid"]."' hspace='10' vspace='3' style='width:70px; height:70px;' onmouseover=\"return escape( '". $fotos[$k]["arqdescricao"] ."' );\" onclick='javascript:window.open(\"../slideshow/slideshow/index.php?pagina=". $_REQUEST['pagina'] ."&arqid=\"+this.id+\"\",\"imagem\",\"width=850,height=600,resizable=yes\")'/>\n";
	}
} else {
	echo "Não existem fotos cadastradas";
}
?>
			</td>
		</tr>
		<tr>
		<td align="center"><?
if(!$_REQUEST['pagina']) $_REQUEST['pagina'] = 0;
$sql = "SELECT COUNT(arq.arqid) AS totalregistros FROM obras.fotosprojeto AS cnt 
		LEFT JOIN public.arquivo AS arq ON arq.arqid = cnt.arqid 
		WHERE prjid = {$_SESSION["prjid"]} AND 
		arq.arqstatus = 'A' AND 
		(arq.arqtipo = 'image/jpeg' OR
		 arq.arqtipo = 'image/gif' OR
		 arq.arqtipo = 'image/png')";
$paginacao = current($db->carregar($sql));
if($paginacao) {
	for($i = 0; $i < ceil(current($paginacao)/16); $i++ ) {
		$page[] = "<a href=?modulo=principal/album_projeto&acao=A&pagina=". $i .">".(($i==$_REQUEST['pagina'])?"<b>".($i+1)."</b>":($i+1))."</a>";
	}
	if(count($page) > 1) {
		echo implode(" | ", $page);
	}
}
		?></td>
		</tr>
	</table>	
</form>
		
