var MSIE = navigator.userAgent.indexOf('MSIE')>=0?true:false;

Total = function(valor){
	var total = valor;
	this.total = total;
}
ImageComponent = function(){
		window.open("upload.html","upload","height=630,width=630");
}
function fileQueueError(file, errorCode, message) {
	try {
		var errorName = "";
		
		switch (errorCode) {
		case SWFUpload.errorCode_QUEUE_LIMIT_EXCEEDED:
			errorName = "Limite ultrapassado";	
			break;
		case SWFUpload.QUEUE_ERROR.FILE_EXCEEDS_SIZE_LIMIT:
			errorName = "tamanho do arquivo execido";
			break;
		case SWFUpload.QUEUE_ERROR.ZERO_BYTE_FILE:
			errorName = "Imagem com tamanho inválido";
		break
		case SWFUpload.QUEUE_ERROR.INVALID_FILETYPE:
			errorName = "Tipo de imagem inválido";
		break
		case 20:
			errorName = "máximo de 21 fotos ";
		break
		default:
			//alert(message);
			break;
		}
		
		if (errorName !== "") {
			alert(errorName);
			return;
		}
		
	} catch (ex) {
		this.debug(ex);
	}

}
Containers = function(containers){
	
	this.lista = containers;
	
	return this.lista;
	
	}
function fileDialogComplete(numFilesSelected, numFilesQueued) {
	try {
		if (numFilesQueued > 0) {
			
		var thumbs = window.document.getElementById("thumbnails");
		var qtdfotos = thumbs.childNodes.length;	
			
			if((qtdfotos+numFilesQueued) <= 21){
				this.startUpload();
				
			}else{
				
				alert("Limite de fotos ultrapassado, máximo de 21 fotos");
				while(this.getStats().files_queued > 0){
					this.cancelUpload();
				}
				return false;
			}
			
		}
	} catch (ex) {
		this.debug(ex);
	}
}
function uploadProgress(file, bytesLoaded) {
	
	try {
		
		var percent = Math.ceil((bytesLoaded / file.size) * 100);
		var progress = new FileProgress(file,  this.customSettings.upload_target);
		progress.setProgress(percent);
		
		if (percent == 100) {
			progress.setStatus("Redimencionando imagem...");
			progress.toggleCancel(false, this);
		} else {
			progress.setStatus("Aguarde...");
			progress.toggleCancel(true, this);
		}
	} catch (ex) {
		this.debug(ex);
	}
}

function uploadSuccess(file, serverData) {
	
	var wurl = window.location.href;
	var url_array = wurl.split("/");
	var url = url_array[0]+"//"+url_array[2]+"/";
	
	var Divs = document.getElementsByTagName("DIV");
	var containers = new Array();
	var container_i = new Array();
	var qtdfotos = this.getStats().files_queued;
		
	var n = 0;
	
	for(var k = 0; k< Divs.length;k++){		
		if(Divs[k].className == "imageBox" && Divs[k].id != "")
			containers.push(Divs[k].id);
	}
	for(var k = 0; k < 20;k++){		
		var div = "imageBox" + k;
			
		if(Find(containers,div) == -1){
			container_i = div;
			break;
		}
	}	
	try {
		var containerPai = window.document.getElementById("thumbnails");
		
		var attributes = new Array();
		attributes["class"] = "imageBox";
		attributes["id"] = container_i;
				
		var containerAtual = CreateElement(true,"div",containerPai,attributes);
		
		if(MSIE){
		
		containerAtual.id = container_i;
		containerAtual.className = "imageBox";
		}

		var src =url+"obras/plugins/resize.php?img=../../../arquivos/obras/imgs_tmp/" + serverData+"&w=68&h=68";
		
		addImage(src,containerAtual);
				 
		var progress = new FileProgress(file,  this.customSettings.upload_target);
		progress.setStatus("Imagem gravada com sucesso.");
		progress.toggleCancel(false);


	} catch (ex) {
		this.debug(ex);
	}
}

function uploadComplete(file) {
	try {
		/*  I want the next upload to continue automatically so I'll call startUpload here */
		if (this.getStats().files_queued > 0) {
			this.startUpload();
		} else {
			initGallery();
			var progress = new FileProgress(file,  this.customSettings.upload_target);
			progress.setComplete();
			progress.setStatus("Todas as Imagens foram enviadas ");
			progress.toggleCancel(false);
			if(Criterio.atualizar) {
				window.opener.AtualizaFotos();
			}
			
		}
	} catch (ex) {
		this.debug(ex);
	}
}

function uploadError(file, errorCode, message) {
	var imageName =  "error.gif";
	var progress;
	try {
		switch (errorCode) {
		case SWFUpload.UPLOAD_ERROR.FILE_CANCELLED:
			try {
				progress = new FileProgress(file,this.customSettings.upload_target);
				progress.setCancelled();
				progress.setStatus("Cancelled");
				progress.toggleCancel(false);
			}
			catch (ex1) {
				this.debug(ex1);
			}
			break;
		case SWFUpload.UPLOAD_ERROR.UPLOAD_STOPPED:
			try {
				progress = new FileProgress(file,  this.customSettings.upload_target);
				progress.setCancelled();
				progress.setStatus("Stopped");
				progress.toggleCancel(true);
			}
			catch (ex2) {
				this.debug(ex2);
			}
		case SWFUpload.UPLOAD_ERROR.UPLOAD_LIMIT_EXCEEDED:
			imageName = "uploadlimit.gif";
			break;
		default:
			alert(message);
			break;
		}

		//addImage("" + imageName);

	} catch (ex3) {
		this.debug(ex3);
	}

}

function addImage(src,container) {
	var foto = src.split("imgs_tmp/");
	var foto_name_array = foto[1].split("&")
	var foto_name = foto_name_array[0];
	
	//adiciona imagem...	
	var attributes = new Array();
	
	attributes["class"] = "imageBox_theImage";
	attributes["style"] = "margin:2px;";
	attributes["src"] = src;
	attributes["id"] = foto_name;
	
	var img = CreateElement(true,"img",container,attributes);
	
	// se for IE criar os atributos da imagem 
	if(MSIE){
		img.className = "imageBox_theImage";
		img.style.margin = "2px";
		img.src = src;
		img.id = foto_name;
	}
	var opener_container = window.opener.document.getElementById("thumbnails");
		
	for(var k=0;k<opener_container.childNodes.length;k++){
		var child = opener_container.childNodes;
				
		if(child[k].className == "imageBox"){
			if(child[k].firstChild){
				continue;
			}else{
				var attributes = new Array();
				attributes["class"] = "imageBox_theImage";
				attributes["style"] = "margin:2px;";
				attributes["src"] = src;
				attributes["id"] = foto_name;
 
				
				var target_img = CreateElement(false,"img",child[k],attributes);
				
				// se for IE criar os atributos da imagem 
				if(MSIE){
					target_img.className = "imageBox_theImage";
					target_img.style.margin = "0px";
					target_img.src = src;
					target_img.id = foto_name;
				}				
				
				//adiciona o campo hidden	
				var attributes      = new Array();
				attributes["type"]  = "hidden"; 
				attributes["name"]  = child[k].id
				attributes["value"] = foto_name
				attributes["id"]    = child[k].id+"_"+foto_name;
				
				var hidden = CreateElement(false,"input",child[k],attributes);
				
				// se for IE criar os atributos do hidden
				if(MSIE){
					hidden.type  = "hidden"; 
					hidden.name  = child[k].id
					hidden.value = foto_name
					hidden.id    = child[k].id+"_"+foto_name;	
				}
				
				break;
			}
		}
	}


	//adiciona o botao de excluir...
	var attributes = new Array();
	attributes["id"] = "del_"+foto_name;
	attributes["style"] = "cursor:pointer;";
	attributes["src"] = "imgs/delete.png";
	
	var del = CreateElement(true,"img",container,attributes);
	
	if(MSIE){
	del.id = "del_"+foto_name;
	del.style.cursor = "pointer";
	del.src = "imgs/delete.png";
	
	var button_del = window.document.getElementById("del_"+foto_name);
	button_del.attachEvent('onclick',DeletarDiv);
		
	function DeletarDiv(e){
		
			var divAtual = img.parentNode;
			
			DeletarFoto(img.id);
			
			var opener_img = window.opener.document.getElementById(img.id);
			var opener_div = opener_img.parentNode;
			var opener_pai = opener_div.parentNode;
						
			divAtual.parentNode.removeChild(divAtual);
			
			opener_pai.removeChild(opener_div);
		
			var div_ = CreateElement(false,"div",opener_pai,attributes);
			
			if(MSIE){
				div_.id = opener_div.id;
				div_.className = "imageBox";
			}
		}
	}	
	
	//adiciona o campo hidden
	var attributes      = new Array();
	attributes["type"]  = "hidden"; 
	attributes["name"]  = container.id
	attributes["value"] = foto_name
	attributes["id"]    = container.id+"_"+foto_name;
	
	var hidden = CreateElement(true,"input",container,attributes);
	
	if(MSIE){
		hidden.type  = "hidden"; 
		hidden.name  = container.id
		hidden.value = foto_name
		hidden.id    = container.id+"_"+foto_name;
	}
			
	del.onclick = function(e){
		
		var divAtual = e.target.parentNode;
		var img = divAtual.firstChild.id;
		
		DeletarFoto(img);

		var opener_img = window.opener.document.getElementById(img);
		var opener_div = opener_img.parentNode;
		var opener_pai = opener_div.parentNode;
					
		divAtual.parentNode.removeChild(divAtual);
		
		opener_pai.removeChild(opener_div);
						
		var attributes = new Array();
		attributes["id"] = opener_div.id;
		attributes["class"] = "imageBox";
		
		var div_ = CreateElement(false,"div",opener_pai,attributes);
		
		if(MSIE){
			div_.id = opener_div.id;
			div_.className = "imageBox";
		}	
				
	}
	
	//adiciona o botao de editar...
	var attributes = new Array();
	attributes["id"] = "edi_"+foto_name;
	attributes["style"] = "cursor:pointer;margin: 0 0 2 5";
	attributes["src"] = "imgs/principal.gif";
	
	
	var edi = CreateElement(true,"img",container,attributes);
	
	if(MSIE){
		edi.id = "edi_"+foto_name;
		edi.style.cursor = "pointer";
		edi.src = "imgs/principal.gif";
	
		var button_edi = window.document.getElementById("edi_"+foto_name);
		button_edi.attachEvent('onclick',EditarDiv);
		
		function EditarDiv(e){
			var divAtual = e.target.parentNode;
			var img = divAtual.firstChild;
			BuscarFoto(img.id);
			var descricao = prompt("Descrição da imagem :",  document.getElementById("varauxiliar").value);
			if(descricao) {
				EditarFoto(img.id, descricao);
			}
		}
	}	
	edi.onclick = function(e){
		var divAtual = e.target.parentNode;
		var img = divAtual.firstChild.id;
		BuscarFoto(img);
		var descricao = prompt("Descrição da imagem :",  document.getElementById("varauxiliar").value);
		if(descricao) {
			EditarFoto(img, descricao);
		}
	}
}

function FileProgress(file, targetID) {
	this.fileProgressID = "divFileProgress";

	this.fileProgressWrapper = document.getElementById(this.fileProgressID);
	if (!this.fileProgressWrapper) {
		this.fileProgressWrapper = document.createElement("div");
		this.fileProgressWrapper.className = "progressWrapper";
		this.fileProgressWrapper.id = this.fileProgressID;

		this.fileProgressElement = document.createElement("div");
		this.fileProgressElement.className = "progressContainer";

		var progressCancel = document.createElement("a");
		progressCancel.className = "progressCancel";
		progressCancel.href = "#";
		progressCancel.style.visibility = "hidden";
		progressCancel.appendChild(document.createTextNode(" "));

		var progressText = document.createElement("div");
		progressText.className = "progressName";
		progressText.appendChild(document.createTextNode(file.name));

		var progressBar = document.createElement("div");
		progressBar.className = "progressBarInProgress";

		var progressStatus = document.createElement("div");
		progressStatus.className = "progressBarStatus";
		progressStatus.innerHTML = "&nbsp;";

		this.fileProgressElement.appendChild(progressCancel);
		this.fileProgressElement.appendChild(progressText);
		this.fileProgressElement.appendChild(progressStatus);
		this.fileProgressElement.appendChild(progressBar);

		this.fileProgressWrapper.appendChild(this.fileProgressElement);

		document.getElementById(targetID).appendChild(this.fileProgressWrapper);
		fadeIn(this.fileProgressWrapper, 0);

	} else {
		this.fileProgressElement = this.fileProgressWrapper.firstChild;
		this.fileProgressElement.childNodes[1].firstChild.nodeValue = file.name;
	}

	this.height = this.fileProgressWrapper.offsetHeight;

}
function VerifyOpenerChildren(){

	var objects = window.opener.document.getElementsByTagName('DIV');
	var self_objects = window.document.getElementsByTagName('DIV');
	
	for(var k=0;k<self_objects.length;k++){

		if(self_objects[k].id == "thumbnails"){
		
			var SelfPai = self_objects[k];
			this.selfpai = SelfPai;
		}
		
	}
	for(var k=0;k<objects.length;k++){
		
		if(objects[k].id == "thumbnails"){
		
			var ParentPai = objects[k];
			this.parentpai = ParentPai;
		}
		
	}
		
	for(var k=0;k<objects.length;k++){
		
		if(objects[k].className=='imageBox' || objects[k].className=='imageBoxHighlighted') {
			if(objects[k].firstChild) {
			
				var attributes = new Array();
				attributes["id"] = objects[k].id;
				attributes["class"] = "imageBox";
				
				var new_div = CreateElement(true,"div",this.selfpai,attributes);
				
				if(MSIE){
					new_div.id = objects[k].id;
					new_div.className = "imageBox";
				}
				
				var attributes = new Array();
				attributes["class"] = "imageBox_theImage";
				attributes["style"] = "margin:2px;";
				attributes["src"] = objects[k].firstChild.src;
				attributes["id"] = objects[k].firstChild.id;
				var img = CreateElement(true,"img",new_div,attributes);
				
				if(MSIE){
					img.id = objects[k].firstChild.id;
					img.className = "imageBox_theImage";
					img.style.margin = "2px";
					img.src = objects[k].firstChild.src;
				}
				
				//adiciona o botao de excluir...
				var attributes = new Array();
				attributes["id"] = "del_"+objects[k].firstChild.id;
				attributes["style"] = "cursor:pointer;";
				attributes["src"] = "imgs/delete.png";
				
				var del = CreateElement(true,"img",new_div,attributes);
				
				if(MSIE){
					del.id = "del_"+objects[k].firstChild.id;
					del.style.cursor = "pointer";
					del.src = "imgs/delete.png";
						
				var button_del = window.document.getElementById(del.id);
				button_del.attachEvent('onclick',DeletarDiv,false);
				
					function DeletarDiv(event){
						
						var divAtual = event.srcElement.parentNode;
						var img = divAtual.firstChild;
						
						DeletarFoto(img.id);
						var opener_img = window.opener.document.getElementById(img.id);
						var opener_div = opener_img.parentNode;
						var opener_pai = opener_div.parentNode;
						divAtual.parentNode.removeChild(divAtual);
						opener_pai.removeChild(opener_div);
						
						var attributes = new Array();
						attributes["id"] = opener_div.id;
						attributes["class"] = "imageBox";
						
						var div_ = CreateElement(false,"div",opener_pai,attributes);
						
						div_.id = opener_div.id;
						div_.className = "imageBox";
					}
				}
				
							
				del.onclick = function(e){
					
					var divAtual = e.target.parentNode;
					var img = divAtual.firstChild.id;
					DeletarFoto(img);
					var opener_img = window.opener.document.getElementById(img);
					var opener_div = opener_img.parentNode;
					var opener_pai = opener_div.parentNode;
					
					divAtual.parentNode.removeChild(divAtual);
					
					opener_div.parentNode.removeChild(opener_div);
					
					var attributes = new Array();
					attributes["id"] = opener_div.id;
					attributes["class"] = "imageBox";
					
					var div_ = CreateElement(false,"div",opener_pai,attributes);
					if(MSIE){
						div_.id = opener_div.id;
						div_.className = "imageBox";
					}
													
				}
				
				
				//adiciona o botao de editar...
				var attributes = new Array();
				attributes["id"] = "edi_"+objects[k].firstChild.id;
				attributes["style"] = "cursor:pointer;margin: 0 0 2 5";
				attributes["src"] = "imgs/principal.gif";
				
				var edi = CreateElement(true,"img",new_div,attributes);
				
				if(MSIE){
					edi.id = "edi_"+objects[k].firstChild.id;
					edi.style.cursor = "pointer";
					edi.src = "imgs/principal.gif";
						
					var button_edi = window.document.getElementById(edi.id);
					button_edi.attachEvent('onclick',EditarDiv,false);
				
					function EditarDiv(event){
						var divAtual = e.target.parentNode;
						var img = divAtual.firstChild;
						BuscarFoto(img.id);
						var descricao = prompt("Descrição da imagem :", document.getElementById("varauxiliar").value);
						if(descricao) {
							EditarFoto(img.id, descricao);
						}
					}
				}
				edi.onclick = function(e){
					var divAtual = e.target.parentNode;
					var img = divAtual.firstChild.id;
					BuscarFoto(img);
					var descricao = prompt("Descrição da imagem :", document.getElementById("varauxiliar").value);
					if(descricao) {
						EditarFoto(img, descricao);
					}
				}
			}
		}
	}
	initGallery();
}









FileProgress.prototype.setProgress = function (percentage) {
	this.fileProgressElement.className = "progressContainer green";
	this.fileProgressElement.childNodes[3].className = "progressBarInProgress";
	this.fileProgressElement.childNodes[3].style.width = percentage + "%";
};
FileProgress.prototype.setComplete = function () {
	this.fileProgressElement.className = "progressContainer blue";
	this.fileProgressElement.childNodes[3].className = "progressBarComplete";
	this.fileProgressElement.childNodes[3].style.width = "";

};
FileProgress.prototype.setError = function () {
	this.fileProgressElement.className = "progressContainer red";
	this.fileProgressElement.childNodes[3].className = "progressBarError";
	this.fileProgressElement.childNodes[3].style.width = "";

};
FileProgress.prototype.setCancelled = function () {
	this.fileProgressElement.className = "progressContainer";
	this.fileProgressElement.childNodes[3].className = "progressBarError";
	this.fileProgressElement.childNodes[3].style.width = "";

};
FileProgress.prototype.setStatus = function (status) {
	this.fileProgressElement.childNodes[2].innerHTML = status;
};

FileProgress.prototype.toggleCancel = function (show, swfuploadInstance) {
	this.fileProgressElement.childNodes[0].style.visibility = show ? "visible" : "hidden";
	if (swfuploadInstance) {
		var fileID = this.fileProgressID;
		this.fileProgressElement.childNodes[0].onclick = function () {
			swfuploadInstance.cancelUpload(fileID);
			return false;
		};
	}
};
