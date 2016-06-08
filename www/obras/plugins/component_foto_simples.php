<? 
include "conf_fotos.php";
include "config.inc";
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
include APPRAIZ."www/obras/_funcoes.php";
include APPRAIZ."www/obras/_constantes.php";
$db = new cls_banco();

function deletarfotos($dados) {
	session_start();
	global $db;
	if($_SESSION['supvid']) {
		$fotid = $db->pegaUm("SELECT fotid FROM obras.fotos WHERE obrid='".$_SESSION['obra']['obrid']."' AND supvid='".$_SESSION['supvid']."' AND fotordem='".($dados['fotordem']-1)."'");
		
		if($fotid) {
			$db->executar("DELETE FROM obras.fotos WHERE fotid='".$fotid."'");
			$db->executar("UPDATE obras.fotos SET fotordem=(fotordem-1),fotbox='imageBox'||(fotordem-1) WHERE fotordem > ".($dados['fotordem']-1)." AND obrid='".$_SESSION['obra']['obrid']."' AND supvid='".$_SESSION['supvid']."'");
			$db->commit();
		}

	}
	exit;
}


function ordenarfotos($dados) {
	session_start();
	global $db;
	if($_SESSION['supvid']) {
		$fotatual = $db->pegaUm("SELECT fotid FROM obras.fotos WHERE obrid='".$_SESSION['obra']['obrid']."' AND supvid='".$_SESSION['supvid']."' AND fotordem='".($dados['ordematual']-1)."'");
		$fotir = $db->pegaUm("SELECT fotid FROM obras.fotos WHERE obrid='".$_SESSION['obra']['obrid']."' AND supvid='".$_SESSION['supvid']."' AND fotordem='".($dados['ordemir']-1)."'");
		if($fotatual && $fotir) {
			$db->executar("UPDATE obras.fotos SET fotordem='".($dados['ordemir']-1)."', fotbox='imageBox".($dados['ordemir']-1)."' WHERE fotid='".$fotatual."'");
			$db->executar("UPDATE obras.fotos SET fotordem='".($dados['ordematual']-1)."', fotbox='imageBox".($dados['ordematual']-1)."' WHERE fotid='".$fotir."'");
			$db->commit();
		}
	}
	exit;
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
}

header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); // Date in the past


header('Content-Type: text/html; charset=iso-8859-1');

if ( possuiPerfil(array(PERFIL_SUPERVISORMEC, PERFIL_ADMINISTRADOR, PERFIL_SUPERVISORUNIDADE, PERFIL_EMPRESA)) ) {
	$html = ""; // botão de enviar arquivos
	$adicionar_novos = '<a href="javascript:void(0);" onclick="inserirNovosArquivos();">Clique aqui para adicionar novos arquivos</a><br />';
}else{
	$html = " disabled=\"disabled\" ";
	$adicionar_novos = '<br />';
}

?>
<html>
<head>
	<meta http-equiv="Pragma" content="no-cache">
	<title>Inserir fotos</title>
	<link href="css/default.css" rel="stylesheet" type="text/css" />
	<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
	<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
	<script type="text/javascript" src="js/utils.js"></script>
	<script type="text/javascript" src="../js/prototype.js"></script>
	<script language="JavaScript" src="../../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<script>
jQuery.noConflict();

var MSIE = navigator.userAgent.indexOf('MSIE')>=0?true:false;

function ajaxatualizar(params,iddestinatario) {
	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: params,
			asynchronous: false,
			onComplete: function(resp) {
				if(iddestinatario) {
					document.getElementById(iddestinatario).innerHTML = resp.responseText;
				} 
			},
			onLoading: function(){
				if(iddestinatario) {
					document.getElementById(iddestinatario).innerHTML = 'Carregando...';
				}
			}
		});
}

function excluirfotosvistoria(indiceselecionado,id) {
	var tabela = document.getElementById('listaimagens');
//	var thumbs = window.opener.document.getElementById("thumbnails");
//	if((tabela.rows.length-2) > indiceselecionado) {
//		if(MSIE){
//			var thumb2 = thumbs.childNodes[indiceselecionado].innerHTML;
//			thumbs.childNodes[indiceselecionado-1].innerHTML = thumb2;
//			thumbs.childNodes[indiceselecionado].innerHTML = "";
//		} else {
//			var thumb2 = window.opener.document.getElementById("imageBox"+indiceselecionado).innerHTML;
//			window.opener.document.getElementById("imageBox"+(indiceselecionado-1)).innerHTML = thumb2;
//			window.opener.document.getElementById("imageBox"+indiceselecionado).innerHTML = "";
//		}
//	} else {
//		if(MSIE){
//			thumbs.childNodes[indiceselecionado-1].innerHTML = "";
//		} else {
//			window.opener.document.getElementById("imageBox"+(indiceselecionado-1)).innerHTML = "";
//		}
//	}
	tabela.deleteRow(indiceselecionado);
	if(tabela.rows.length == 2) {
		document.getElementById('submit1').disabled = true;
		document.getElementById('submit2').disabled = true;
	}
	jQuery('#foto_' + id,window.opener.document).remove();
	ajaxatualizar("fotordem="+indiceselecionado+"&requisicao=deletarfotos");
//	atualizaFotosAjax("atualizarFotosAjax=true",window.opener.document.getElementById("fotos_supervisao"))
//	window.opener.UpdateListFotoSimples(indiceselecionado-1);
}


function ordenar(indiceselecionado, acao) {
	switch(acao) {
		case 'subir':
			if(indiceselecionado > 1) {
				// alterando ordem na tabela
				var tabela = document.getElementById('listaimagens');
				var linha1 = tabela.rows[indiceselecionado];
				var desc1 = tabela.rows[indiceselecionado].cells[2].firstChild.value;
				var linha2 = tabela.rows[(indiceselecionado-1)];
				var desc2 = tabela.rows[(indiceselecionado-1)].cells[2].firstChild.value;
				var celula2_1 = linha2.cells[1].innerHTML;
				var celula2_2 = linha2.cells[2].innerHTML;
				var celula1_1 = linha1.cells[1].innerHTML;
				var celula1_2 = linha1.cells[2].innerHTML;
				linha1.cells[1].innerHTML = celula2_1;
				linha1.cells[2].innerHTML = celula2_2;
				linha2.cells[1].innerHTML = celula1_1;
				linha2.cells[2].innerHTML = celula1_2;
				linha1.cells[2].firstChild.value = desc2;
				linha2.cells[2].firstChild.value = desc1;
				// alterando ordem nos thumbnails
//				if(MSIE){
//					var thumbs = window.opener.document.getElementById("thumbnails");
//					var thumb2 = thumbs.childNodes[(indiceselecionado-2)].innerHTML;
//					var thumb1 = thumbs.childNodes[(indiceselecionado-1)].innerHTML;
//					thumbs.childNodes[(indiceselecionado-1)].innerHTML = thumb2;
//					thumbs.childNodes[(indiceselecionado-2)].innerHTML = thumb1;
//				} else {
//					var thumb2 = window.opener.document.getElementById("imageBox"+indiceselecionado).innerHTML;
//					var thumb1 = window.opener.document.getElementById("imageBox"+(indiceselecionado-1)).innerHTML;
//					window.opener.document.getElementById("imageBox"+(indiceselecionado-1)).innerHTML = thumb2;
//					window.opener.document.getElementById("imageBox"+indiceselecionado).innerHTML = thumb1;
//				}
				ajaxatualizar("ordematual="+indiceselecionado+"&ordemir="+(indiceselecionado-1)+"&requisicao=ordenarfotos");
				atualizaFotosAjax("atualizarFotosAjax=true",window.opener.document.getElementById("fotos_supervisao"))
			}
			break;
		case 'descer':
			var tabela = document.getElementById('listaimagens');
			if((tabela.rows.length-2) > indiceselecionado) {
				// alterando ordem na tabela
				var tabela = document.getElementById('listaimagens');
				var linha1 = tabela.rows[indiceselecionado];
				var desc1 = tabela.rows[indiceselecionado].cells[2].firstChild.value;
				var linha2 = tabela.rows[(indiceselecionado+1)];
				var desc2 = tabela.rows[(indiceselecionado+1)].cells[2].firstChild.value;
				var celula2_1 = linha2.cells[1].innerHTML;
				var celula2_2 = linha2.cells[2].innerHTML;
				var celula1_1 = linha1.cells[1].innerHTML;
				var celula1_2 = linha1.cells[2].innerHTML;
				linha1.cells[1].innerHTML = celula2_1;
				linha1.cells[2].innerHTML = celula2_2;
				linha2.cells[1].innerHTML = celula1_1;
				linha2.cells[2].innerHTML = celula1_2;
				linha1.cells[2].firstChild.value = desc2;
				linha2.cells[2].firstChild.value = desc1;
				// alterando ordem nos thumbnails
//				if(MSIE){
//					var thumbs = window.opener.document.getElementById("thumbnails");
//					var thumb1 = thumbs.childNodes[indiceselecionado].innerHTML;
//					var thumb2 = thumbs.childNodes[indiceselecionado-1].innerHTML;
//					thumbs.childNodes[indiceselecionado].innerHTML = thumb2;
//					thumbs.childNodes[indiceselecionado-1].innerHTML = thumb1;
//				} else {
//					var thumb2 = window.opener.document.getElementById("imageBox"+(indiceselecionado)).innerHTML;
//					var thumb1 = window.opener.document.getElementById("imageBox"+(indiceselecionado-1)).innerHTML;
//					window.opener.document.getElementById("imageBox"+(indiceselecionado-1)).innerHTML = thumb2;
//					window.opener.document.getElementById("imageBox"+(indiceselecionado)).innerHTML = thumb1;
//				}
				ajaxatualizar("ordematual="+indiceselecionado+"&ordemir="+(indiceselecionado+1)+"&requisicao=ordenarfotos");
				atualizaFotosAjax("atualizarFotosAjax=true",window.opener.document.getElementById("fotos_supervisao"))
			}
			break;
	} 	
}

function validarInsercaoFiles() {
	var anexovazio = true;
	for(i=0;i<document.anexo.elements.length;i++) {
		if(document.anexo.elements[i].type == "file") {
			if(document.anexo.elements[i].value != "") {
				anexovazio = false;
			}
		}
	}
	if(anexovazio) {
		alert("Não existem arquivos anexados.");
		return false;
	} else {
		return true;
	}
}

function inserirNovosArquivos() {
	var tabela = document.getElementById('inserirarquivos');
	for(i=0;i<2;i++) {
		var line = tabela.insertRow((tabela.rows.length-2));
		var cell = line.insertCell(0);
		cell.innerHTML = "<input type=\"file\" name=\"Filedata[]\">";
	}
}

function isNumber(numero){
	var CaractereInvalido = false;
	for (i=0; i < numero.length; i++) {
		var Caractere = numero.charAt(i);
		if(Caractere != "." && Caractere != "," && Caractere != "-"){
			if (isNaN(parseInt(Caractere))) CaractereInvalido = true;
		}
	}
	return !CaractereInvalido;
}
function inserirobservacaothumnails(id, descricao) {
//	var opener_container = window.opener.document.getElementById("thumbnails");
//	for(var k=0;k<opener_container.childNodes.length;k++){
//		var child = opener_container.childNodes;
//		if(child[k].firstChild){
//			var imagem = child[k].firstChild;
//			if(!isNumber(imagem.id)) {
//				var img  = ""+imagem.id+"";
//				if(img == id) {
//					imagem.title = descricao
//				}
//			}
//		}
//
//	}
}

function inserirfotosthumbnailsPorNome(nome)
{
	var num = jQuery('#fotos_supervisao li',window.opener.document);
	
	jQuery('#fotos_supervisao',window.opener.document).append( '<li class="nodraggable" id="foto_'+ nome + '"><img width="96" height="76.65" src="./plugins/resize.php?img=../../../arquivos/obras/imgs_tmp/' + nome + '&w=85&h=100" class=\"img_foto\"></li>' );
}

function inserirfotosthumbnails(id) {
	
	var num = jQuery('#fotos_supervisao li',window.opener.document);
	
	var pag = parseInt(num.size() / 16);

	jQuery('#fotos_supervisao',window.opener.document).append( '<li class="draggable" id="foto_'+ id + '"><img width="96" height="76.65" src="../slideshow/slideshow/verimagem.php?arqid=' + id + '&newwidth=100&newheight=85" ondblclick="abrirGaleria(\'829800\',\'' + pag + '\')" class=\"img_foto\"></li>' );
	
//	atualizaFotosAjax("atualizarFotosAjax=true",window.opener.document.getElementById("fotos_supervisao"))
//	var tabela = document.getElementById('listaimagens');
//	var opener_container = window.opener.document.getElementById("thumbnails");
//	for(var k=0;k<opener_container.childNodes.length;k++){
//		var child = opener_container.childNodes;
//		if(child[k].className == "imageBox"){
//			if(child[k].firstChild){
//				continue;
//			} else {
//				if(isNumber(id)) {
//					var imagem = "<img id='"+id+"' class='imageBox_theImage' style='margin:2px;' src='../../slideshow/slideshow/verimagem.php?arqid="+id+"'><br><input type='checkbox' onclick=\"repositorioGaleria('"+child[k].id+"', "+id+", '../slideshow/slideshow/verimagem.php?arqid="+id+"');\" value='"+id+"' id=galeria_"+id+">  Incluir na Galeria";
//					child[k].innerHTML = imagem;
//				} else {
//					var imagem = "<img id='"+id+"' style='width:68px;height:68px;' src='./plugins/resize.php?img=../../../arquivos/obras/imgs_tmp/"+id+"&w=68&h=68'><input type='hidden' id='"+child[k].id+"_"+id+"' name='"+child[k].id+"' value='"+id+"'>";
//					child[k].innerHTML = imagem;
//				}
//				break;
//			}
//		}
//	}
}

function atualizaFotosAjax(params,iddestinatario)
{

	var myAjax = new Ajax.Request(
		window.opener.location.href,
		{
			method: 'post',
			parameters: params,
			asynchronous: false,
			onComplete: function(resp) {
				if(iddestinatario) {
					iddestinatario.innerHTML = resp.responseText;
				} 
			},
			onLoading: function(){
				if(iddestinatario) {
					iddestinatario.innerHTML = 'Carregando...';
				}
			}
		});
}
	
</script>
</head>
<iframe name="iframeUpload" style="display:none;"></iframe>
<body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff">
<br />
<form method="post" id="anexo" name="anexo" enctype="multipart/form-data" action="upload_simples.php?funcao=<? echo $_REQUEST['funcao']; ?>" onsubmit="return validarInsercaoFiles();" target="iframeUpload">
<? //echo montarAbasArray_($abas, $_SERVER['REQUEST_URI']); ?>
<table class="listagem" width="95%" align="center" id="inserirarquivos">
<tr>
<td class="SubTituloCentro">Anexar arquivos</td>
</tr>
<tr>
<td><input type="file" name="Filedata[]" <?php echo $html; ?>></td>
</tr>
<tr>
<td><input type="file" name="Filedata[]" <?php echo $html; ?>></td>
</tr>
<tr>
<td>
<?php echo $adicionar_novos; ?>
</td>
</tr>
<tr>
<td><input type="submit" name="sub" value="Enviar" <?php echo $html; ?>></td>
</tr>
</table>
</form>
<form method="post" name="descricao" action="gravar_obs.php?funcao=<? echo $_REQUEST['funcao']; ?>" target="iframeUpload">
<table class="listagem" width="95%" align="center" id="listaimagens">
<tr bgcolor="#C0C0C0">
<td align="right" colspan="4">
<input type="submit" value="Gravar observações" id="submit1" <?php echo $html; ?>>
</td>
</tr>
<?
if($_SESSION['supvid']) {
	
	$sql = "SELECT arq.arqid, arq.arqdescricao FROM obras.fotos fot
			LEFT JOIN public.arquivo arq ON arq.arqid = fot.arqid 
			WHERE supvid='".$_SESSION['supvid']."' AND obrid='".$_SESSION['obra']['obrid']."' 
			ORDER BY fotordem";
	
	$arqssup = $db->carregar($sql);
	if($arqssup[0]) {
		$habilitarsubmitsobs = true;
		$controletextarea=0;
		foreach($arqssup as $arq) {
			echo "<tr>";
			//echo "<td><img id=\"setasubir\" src=\"../../imagens/seta_cima.gif\"  style=\"cursor:pointer;\" onclick=\"ordenar(this.parentNode.parentNode.rowIndex,'subir');\"> <img id=\"setadescer\" src=\"../../imagens/seta_baixo.gif\" style=\"cursor:pointer;\" onclick=\"ordenar(this.parentNode.parentNode.rowIndex,'descer');\"></td>";
			echo "<td></td>";
			echo "<td>";
			echo "<img src='../../slideshow/slideshow/verimagem.php?arqid=".$arq['arqid']."' width=100 height=100>";
			echo "</td>"; 
			echo "<td>";
			echo "<textarea {$html}  id='supobs' name='supobs[".$arq['arqid']."]' cols='40' rows='5' onmouseover='MouseOver( this );' onfocus='MouseClick( this );'  onmouseout='MouseOut( this );'  onblur='MouseBlur( this );' onkeydown='textCounter( this.form.supobs[".$arq['arqid']."], this.form.no_supobs".$controletextarea.", 255 );'  onkeyup='textCounter( this.form.supobs[".$arq['arqid']."], this.form.no_supobs".$controletextarea.", 255);'>".$arq['arqdescricao']."</textarea><br /><input readonly style='text-align:right;border-left:#888888 3px solid;color:#808080;' type='text' name='no_supobs".$controletextarea."' size='6' maxlength='6' value='255' class='CampoEstilo'><font color='red' size='1' face='Verdana'> máximo de caracteres</font>";
			echo "</td>";
			
		if ( possuiPerfil(array(PERFIL_SUPERVISORMEC, PERFIL_ADMINISTRADOR, PERFIL_SUPERVISORUNIDADE, PERFIL_EMPRESA)) ) {
			$botao = "<img id=\"excluirfotovistoria\" src=\"../../imagens/excluir.gif\" style=\"cursor:pointer\" onclick=\"excluirfotosvistoria(this.parentNode.parentNode.rowIndex,'".$arq['arqid']."');\">";
		}else{
			$botao = "<img id=\"excluirfotovistoria\" src=\"../../imagens/excluir_01.gif\">";
		}
			
			echo "<td>
					{$botao}
				 </td>";
			echo "</tr>";
			$controletextarea++;
		}
	}
}
?>
<tr bgcolor="#C0C0C0">
<td align="right" colspan="4">
<input type="submit" value="Gravar observações" id="submit2" <?php echo $html; ?>>
</td>
</tr>
</table>
<? 
if($habilitarsubmitsobs) { 
?>
	<script>
	document.getElementById('submit1').disabled = false;
	document.getElementById('submit2').disabled = false;
	</script>
<? 
}
?>
</form>
</body>
</html>
<script>
//var opener_container = window.opener.document.getElementById("thumbnails");

var tabela = window.document.getElementById("listaimagens");

var controletextarea=0;

for(var k=0;k<tabela.childNodes.length;k++){
	
	var child = tabela.childNodes;
	
	if(child[k].firstChild){
	
		var imagem = child[k].firstChild;
		
		if(!isNumber(imagem.id)) {
		
			document.getElementById('submit1').disabled = false;
			document.getElementById('submit2').disabled = false;
			var linha  = tabela.insertRow(tabela.rows.length-1);
			linha.id=imagem.id;
			var img  = ""+imagem.id+"";
			var celulaordenacao = linha.insertCell(0);
			celulaordenacao.vAlign="middle";
			celulaordenacao.innerHTML = "<img id=\"setasubir\" src=\"../../imagens/seta_cima.gif\" align=\"absmiddle\"  style=\"cursor:pointer;\" onclick=\"ordenar(this.parentNode.parentNode.rowIndex,'subir');\"> <img id=\"setadescer\" src=\"../../imagens/seta_baixo.gif\" style=\"cursor:pointer;\" onclick=\"ordenar(this.parentNode.parentNode.rowIndex,'descer');\">";
			var celulaimg = linha.insertCell(1);
			celulaimg.innerHTML = "<img src='./resize.php?img=../../../arquivos/obras/imgs_tmp/"+img+"&w=100&h=100'>";
			var celuladsc = linha.insertCell(2);
			celuladsc.innerHTML = "<textarea  id='supobs' name='supobs["+img+"]' cols='40' rows='5' onmouseover='MouseOver( this );' onfocus='MouseClick( this );'  onmouseout='MouseOut( this );'  onblur='MouseBlur( this );' onkeydown='textCounter( this.form.supobs["+controletextarea+"], this.form.no_supobs"+controletextarea+", 255 );'  onkeyup='textCounter( this.form.supobs["+controletextarea+"], this.form.no_supobs"+controletextarea+", 255);'>"+imagem.title+"</textarea><br><input readonly style='text-align:right;border-left:#888888 3px solid;color:#808080;' type='text' name='no_supobs"+controletextarea+"' size='6' maxlength='6' value='255' class='CampoEstilo'><font color='red' size='1' face='Verdana'> máximo de caracteres</font>";
			var celuladeleta = linha.insertCell(3);
			celuladeleta.innerHTML = "<img id=\"excluirfotovistoria\" src=\"../../imagens/excluir.gif\" style=\"cursor:pointer\" onclick=\"excluirfotosvistoria(this.parentNode.parentNode.rowIndex);\">";
			controletextarea++;
			
		}
	}

}
</script>