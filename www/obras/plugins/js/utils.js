var MSIE = navigator.userAgent.indexOf('MSIE')>=0?true:false;

Acao = function(bool){
		this.atualizar = bool;
}

DeletarFoto = function(target){
		
		/*var wurl = window.location.href;
		var url_array = wurl.split("/");
		var url = url_array[0]+"//"+url_array[2]+"/";*/
		var url = caminho_atual + '?modulo=principal/inserir_vistoria&acao=A&subacao=deletar&AJAX=1';
		var parametros = "img="+target;
		var myAjax = new Ajax.Request(
			url,
			{
				method: 'post',
				parameters: parametros,
				onComplete: function(resp) {
				}
	});
}

EditarFoto = function(target, descricao){
		window.opener.document.getElementById(target).title = descricao;
		/*var wurl = window.location.href;
		var url_array = wurl.split("/");
		var url = url_array[0]+"//"+url_array[2]+"/";*/

		var url = caminho_atual + '?modulo=principal/inserir_vistoria&acao=A&subacao=editar&AJAX=1';
		var parametros = "arqid="+target+"&arqdescricao="+descricao+"&tipoatualizacao=somentedescricao";
		var myAjax = new Ajax.Request(
			url,
			{
				method: 'post',
				parameters: parametros,
				onComplete: function(resp) {
					//alert(resp.responseText);
					UpdateListFoto();
				}
		});
		
}

BuscarFoto = function(arqid){
		var wurl = window.location.href;
		var url_array = wurl.split("/");
		var url = url_array[0]+"//"+url_array[2]+"/";

		var url = url + 'obras/plugins/buscardescricaofoto.php';
		var parametros = "arqid="+arqid;
		var myAjax = new Ajax.Request(
			url,
			{
				method: 'post',
				asynchronous: false,
				parameters: parametros,
				onComplete: function(resp) {
					//alert(resp.responseText);
					document.getElementById("varauxiliar").value = resp.responseText;
				}
		});
}

/*
UpdateListFoto = function(){
	
	var url = caminho_atual + '?modulo=principal/inserir_vistoria&acao=A&subacao=UpdateListFoto&AJAX=1';
		
	var myAjax = new Ajax.Request(
		url,
		{
			method: 'post',
			asynchronous: false,
			onComplete: function(resp) {
				if(resp.responseText) {
					window.opener.document.getElementById("thumbnails").innerHTML = resp.responseText;
				}
			}
		});
	
}*/

FadeIn = function(element, opacity){

	var reduceOpacityBy = 5;
	var rate = 30;	// 15 fps

	if (opacity < 100) {
		opacity += reduceOpacityBy;
		if (opacity > 100) {
			opacity = 100;
		}

		if (element.filters) {
			try {
				element.filters.item("DXImageTransform.Microsoft.Alpha").opacity = opacity;
			} catch (e) {
				element.style.filter = 'progid:DXImageTransform.Microsoft.Alpha(opacity=' + opacity + ')';
			}
		} else {
			element.style.opacity = opacity / 100;
		}
	}
	if (opacity < 100) {
		setTimeout(function () {FadeIn(element, opacity);}, rate);
	}
}

CreateElement = function(location, type, container, atributos)
{	
	if(location){
		var newElement = window.document.createElement(type);	
	}else{
		var newElement = window.opener.document.createElement(type);
	}
	
	for(var k in atributos) {
        if (/name|style|class|src|id|value|type|readonly/i.test(k))
            newElement.setAttribute(k, atributos[k]);
    }
	    
	container.appendChild(newElement);
	
	if(type == "img"){
		
		if (newElement.filters) {
			try {
				newElement.filters.item("DXImageTransform.Microsoft.Alpha").opacity = 0;
			} catch (e) {
				newElement.style.filter = 'progid:DXImageTransform.Microsoft.Alpha(opacity=' + 0 + ')';
			}
		} else {
			newElement.style.opacity = 0;
		}
		
		newElement.onload = function () {
			FadeIn(newElement, 0);
		}
	}
	
	return newElement;
}

Find = function(ary, element){
    for(var i=0; i<ary.length; i++){
        if(ary[i] == element){
            return i;
        }
    }
    return -1;
}
