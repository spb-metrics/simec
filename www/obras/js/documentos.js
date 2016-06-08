ValidarFormulario = function(form){
	
	if(form.arquivo.value == ""){
		alert("Campo Arquivo obrigatório.");
		form.arquivo.focus();
		return false;
	}
	if(form.tpaid.value == ""){
		alert("Campo Tipo obrigatório.");
		form.tpaid.focus();
		return false;
	}
	if(form.arqdescricao.value == ""){
		alert("Campo Descrição obrigatório.");
		form.arqdescricao.focus();
		return false;
	}
	
	return true;

}
FlipPhoto = function(img,direcao){

	/*var wurl = window.location.href;
	var url_array = wurl.split("/");
	var url = url_array[0]+"//"+url_array[2]+"/";*/
	
	var url = caminho_atual + '?modulo=inicio&acao=N&AJAX=1';
	var parametros = "img="+img+"&dir="+"../../arquivos/obras/documentos/"+"&direcao="+direcao;
		
	var myAjax = new Ajax.Request(
		url,
		{
			method: 'post',
			parameters: parametros,
			asynchronous: false,
			onComplete: function(resp) {
				alert(resp.responseText);
				size = resp.responseText.split("-");
				window.innerWidth  = size[0];  
				window.innerHeight = size[1];
				
				var frame = window.document.getElementById("main_image");
				var linker = window.document.getElementById("tete");
				window.location.reload();
				frame.src = "resize.php?img=../../../arquivos/obras/documentos/"+size[2]+"&w="+size[0]+"&h="+size[1]
			}
		});
}