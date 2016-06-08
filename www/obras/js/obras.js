
function obrFiltraLista(){
	document.getElementById( 'obrFormPesquisa' ).submit();
}

function obrVerTodas(){
	document.getElementById( 'requisicao' ).value = "limpar";
	document.getElementById( 'obrFormPesquisa' ).submit();
}

function listaDeObraOrdem( ordem ){
	
//	alert(ordem);
	jQuery.ajax({
		url: "obras.php?modulo=inicio&acao=A",
		type: "POST",
		async: true,
		cache: false,
		data: "&requisicao=ordem&ordem=" + ordem,
		dataType: "html",
		success: function(msg){ 
	}
	})
	
}

function obrMostraFilho( id, tipo, acao ){

	switch( tipo ){
		
		case "campus":
		
			divCarregando( "imgFilhoLista" + id );
		
			jQuery.ajax({
			    url: "obras.php?modulo=inicio&acao=A",
			    type: "POST",
			    async: true,
			    cache: false,
			    data: "&requisicao=listafilho&entid=" + id,
			    dataType: "html",
			    success: function(msg){ 
			    	jQuery( "#divFilhoLista" + id ).html(msg);
			    	divCarregado();
			    }
		    })
			
			if( acao == 'abre' ){
				
				document.getElementById( "imgFilhoLista" + id ).src     = "../imagens/menos.gif";
				document.getElementById( "imgFilhoLista" + id ).setAttribute( 'onclick', "obrMostraFilho( " + id + ", 'campus', 'fecha' )" );
				obrArvoreSessao( id, 'entidade', 'incluir' );
				document.getElementById( "trFilhoLista" + id ).style.display = "table-row";
				
			}else if( acao == 'fecha' ){
				
				document.getElementById( "imgFilhoLista" + id ).src = "../imagens/mais.gif";
				document.getElementById( "imgFilhoLista" + id ).setAttribute( 'onclick', "obrMostraFilho( " + id + ", 'campus', 'abre' )" );
				obrArvoreSessao( id, 'entidade', 'excluir' );
				document.getElementById( "trFilhoLista" + id ).style.display = "none";
				
			}
		
		break;
		
		case "obras":
		
			divCarregando( "imgFilhoObra" + id );
		
			jQuery.ajax({
			    url: "obras.php?modulo=inicio&acao=A",
			    type: "POST",
			    async: true,
			    cache: false,
			    data: "&requisicao=listaobra&entid=" + id,
			    dataType: "html",
			    success: function(msg){ 
			    	jQuery( "#divFilhoObra" + id ).html(msg);
			    	divCarregado();
			    }
		    })
		
			if( acao == 'abre' ){
				
				document.getElementById( "imgFilhoObra" + id ).src = "../imagens/menos.gif";
				document.getElementById( "imgFilhoObra" + id ).setAttribute( 'onclick', "obrMostraFilho( " + id + ", 'obras', 'fecha' )" );
				obrArvoreSessao( id, 'campus', 'incluir' );
				document.getElementById( "trFilhoObra" + id ).style.display = "table-row";
				
			}else if( acao == 'fecha' ){
				
				document.getElementById( "imgFilhoObra" + id ).src = "../imagens/mais.gif";
				document.getElementById( "imgFilhoObra" + id ).setAttribute( 'onclick', "obrMostraFilho( " + id + ", 'obras', 'abre' )" );
				obrArvoreSessao( id, 'campus', 'exluir' );
				document.getElementById( "trFilhoObra" + id ).style.display = "none";
				
			}
		
		break;
		
		case "obrasSC":
			
			divCarregando( "imgFilhoObra" + id );
			
			jQuery.ajax({
				url: "obras.php?modulo=inicio&acao=A",
				type: "POST",
				async: true,
				cache: false,
				data: "&requisicao=listaobraSC&entid=" + id,
				dataType: "html",
				success: function(msg){ 
				jQuery( "#divFilhoObra" + id ).html(msg);
				divCarregado();
			}
			})
			
			if( acao == 'abre' ){
				
				document.getElementById( "imgFilhoObra" + id ).src = "../imagens/menos.gif";
				document.getElementById( "imgFilhoObra" + id ).setAttribute( 'onclick', "obrMostraFilho( " + id + ", 'obras', 'fecha' )" );
				obrArvoreSessao( id, 'campus', 'incluir' );
				document.getElementById( "trFilhoObra" + id ).style.display = "table-row";
				
			}else if( acao == 'fecha' ){
				
				document.getElementById( "imgFilhoObra" + id ).src = "../imagens/mais.gif";
				document.getElementById( "imgFilhoObra" + id ).setAttribute( 'onclick', "obrMostraFilho( " + id + ", 'obras', 'abre' )" );
				obrArvoreSessao( id, 'campus', 'excluir' );
				document.getElementById( "trFilhoObra" + id ).style.display = "none";
				
			}
			
			break;
		
	}

}

function obrIrParaCaminho( obrid, tipo, orgid, arqid ){

	switch( tipo ){
	
		case "cadastro":
			location.href = "obras.php?modulo=principal/cadastro&acao=A&obrid=" + obrid; 
		break;
		
		case "anexos":
			location.href = "obras.php?modulo=principal/documentos&acao=A&obrid=" + obrid; 
		break;
		
		case "fotos":
			location.href = "obras.php?modulo=principal/album&acao=A&obrid=" + obrid; 
		break;
		
		case "slideFotos":
			javascript:window.open("../slideshow/slideshow/ajustarimgparam3.php?pagina=&arqid="+arqid+"&obrid="+obrid,"imagem","width=850,height=600,resizable=yes")
			break;
		
		case "restricao":
			location.href = "obras.php?modulo=principal/restricao&acao=A&obrid=" + obrid; 
		break;
		
		case "pi":
			location.href = "obras.php?modulo=principal/cadastro_pi&acao=A&obrid=" + obrid; 
		break;
		
		case "aditivo":
			location.href = "obras.php?modulo=principal/cadastroAditivo&acao=A&obrid=" + obrid; 
		break;
	
		case "novaobra":
			location.href = "obras.php?modulo=principal/cadastro&acao=A&subAcao=novaObra&org=" + orgid; 
		break;
	
		case "excluir":
			if( confirm( "Deseja realmente excluir esta obra?" ) ){
				location.href = "obras.php?modulo=inicio&acao=A&requisicao=excluir&obrid=" + obrid;
			} 
		break;
		case "vistoria":
				location.href = "obras.php?modulo=principal/vistoria&acao=A&obrid=" + obrid;
			break;
	
	}
}

function confirmExcluir(msg, url){
	if ( confirm(msg) )
		location.href = url;
	return;
}

function obrArvoreSessao( entid, tipo, acao ){
	
	switch( tipo ){
	
		case "entidade":
			if( acao == "incluir"){
				document.getElementById('arEntid').value += "{"+entid+"}"; 
			}
			if( acao == "excluir"){
				document.getElementById('arEntid').value = document.getElementById('arEntid').value.replace('{'+entid+'}',' '); 
			}
			break;
			
		case "campus":
			if( acao == "incluir"){
				document.getElementById('arEntidCampus').value += "{"+entid+"}"; 
			}
			if( acao == "excluir"){
				document.getElementById('arEntidCampus').value = document.getElementById('arEntidCampus').value.replace('{'+entid+'}',' '); 
			}
			break;
		
	}
	
	var arEntid       = document.getElementById('arEntid').value;
	var arEntidCampus = document.getElementById('arEntidCampus').value;

	jQuery.ajax({
		url: "obras.php?modulo=inicio&acao=A",
		type: "POST",
		async: true,
		cache: false,
		data: "&requisicao=atualizaarvore&arEntid="+arEntid+"&arEntidCampus="+arEntidCampus,
		dataType: "html",
		success: function(msg){ 
		divCarregado();
	}
	})
	
}

// ---------- FIM FUNÇÕES NOVA TELA INICIAO ----------

function obrValidaData( dtInicio, dtTermino ){
	
	if( dtInicio.value != "" && dtTermino.value != "" ){
		if( !validaDataMaior( dtInicio, dtTermino ) ){
			alert( "A data de Término deve ser maior que a de Início!" );
			 dtTermino.focus();
			 dtTermino.value = '';
			return false;
		}else{
			return true;
		}
	}else{
		alert( "Favor Preencher as Datas de Início e Término!" );
		return false;
	}

}

/* Função que permite somente a digitação de números. */
function somenteNumeros(e) {	
	if(window.event) {
    	/* Para o IE, 'e.keyCode' ou 'window.event.keyCode' podem ser usados. */
        key = e.keyCode;
    }
    else if(e.which) {
    	/* Netscape */
        key = e.which;
    }
    if(key!=8 || key < 48 || key > 57) return (((key > 47) && (key < 58)) || (key==8));
    {
    	return true;
    }
} 

function somenteLetras(e) {	
	if(window.event) {
    	/* Para o IE, 'e.keyCode' ou 'window.event.keyCode' podem ser usados. */
        key = e.keyCode;
    }
    else if(e.which) {
    	/* Netscape */
        key = e.which;
    }
    if(key!=8 || key < 65 || key > 122) return (((key > 65) && (key < 122)) || (key==8));
    {
    	return true;
    }
}

VertodasObras = function(){
	var form = window.document.getElementById("pesquisar");
	for(var i=0;i < form.elements.length;i++){
		var campo = form.elements[i];
		switch(campo.type){
			case "select-one":
				if(campo.name == 'percentualfinal'){
					campo.options[20].selected = true	
				}else{
				campo.options[0].selected = true;
				}
			break;
			case "text":
				campo.value = "";
			break;
			case "radio":
				campo.checked = false;
			break;
			default:
				
			break;
			
		}
	}
	form.submit();
}

VerificaOrgaoSelecionado = function (orgao){
	if(orgao.value != "")
		AtualizaComboUnidadeOrgaoObra(orgao.value);
}

AtualizaComboUnidadeOrgaoObra = function(orgao){
	
	var wurl = window.location.href;
	var url_array = wurl.split("/");
	var url = url_array[0]+"//"+url_array[2]+"/";
		
	var url = url + 'obras/obras.php?modulo=inicio&acao=A&lista=1&AJAX=1';
	var parametros = "orgao="+orgao;
		
	var myAjax = new Ajax.Request(
		url,
		{
			method: 'post',
			parameters: parametros,
			asynchronous: true,
			onComplete: function(resp) {
				var unidade = window.document.createElement("option");
						
				if(resp.responseText){
					
					Hide("loading");
					var campos = resp.responseText.split("|");
					var n = campos.length;
					window.document.getElementById("unidade").options.length = 1;
									
					if(n >1){
						for(var k=0;k < n;k++){
							var j = k+1;
							
							var valores = campos[k].split("-");
							window.document.getElementById("unidade").options[j] = new Option(valores[1],
                                                                                      valores[0],
                                                                                      false,
                                                                                      false);
						}
					}else{
						var valores = campos[0].split("-");
						window.document.getElementById("unidade").options[1] = new Option(valores[1],valores[0],false,false);	
					} 
																				
				}else{
					
					// Clear("loading");
					// Add("loading","Nenhuma Unidade encontrada para o orgão
					// selecionado");
					// Show("loading");
					window.document.getElementById("unidade").options.length = 1;
					window.setTimeout(function(){
						Hide("loading");
					},2000)
				}
			},
			onLoading: function(){
				// Clear("loading");
				// Add("loading","Aguarde... <br/> Carregando lista de
				// Unidades...");
				// Show("loading");
			}
		});
}  
Clear = function(id){
	window.document.getElementById(id).innerHTML = "";
}
Add =  function(id,data){
	window.document.getElementById(id).innerHTML += data;
}
Show = function(id){
	window.document.getElementById(id).style.display = "block";
}

Hide = function(id){
	window.document.getElementById(id).style.display = "none";
}

ViewBigImage = function(img,dir){
	
	/*
	 * var wurl = window.location.href; var url_array = wurl.split("/"); var url =
	 * url_array[0]+"//"+url_array[2]+"/";
	 */
		
	var url = caminho_atual + '?modulo=principal/inserir_vistoria&acao=A&subacao=ShowImage&AJAX=1';
	var parametros = "img="+img+"&dir="+dir;
		
	var myAjax = new Ajax.Request(
		url,
		{
			method: 'post',
			parameters: parametros,
			asynchronous: false,
			onComplete: function(resp) {
				
				if(resp.responseText){
					size = resp.responseText.split("-");
					ShowImage(img,size[0],size[1],"../" + dir);
				}else{
					alert("Imagem não encontrada.");
				}
			}
		});
	
}

ShowImage = function(img,w,h,dir){
	window.open('/obras/plugins/view_image.php?img=' + dir + img +'&w='+w+'&h='+h,'teste','width='+w+',height='+h)
}
UpdateListFoto = function(){
	var url = caminho_atual + '?modulo=principal/inserir_vistoria&acao=A&subacao=UpdateListFoto&AJAX=1';
	var myAjax = new Ajax.Updater(
		"thumbnails",
		url,
		{
			method: 'post',
			asynchronous: false
		});
}	

function UpdateListFotoSimples(indice) {
	for(i=indice;i<20;i++) {
		if(document.getElementById('imageBox'+(i+1)))
			document.getElementById('imageBox'+i).innerHTML = document.getElementById('imageBox'+(i+1)).innerHTML; 
	}
}


Cadastrar = function(url){
		 window.location = url;
		return; 
}

Excluir = function(url,obrid){
		if(confirm("Deseja realmente excluir esta obra ?"))
			window.location = url+'&obrid='+obrid;
}

AtualizarVistoria = function(url,supvid){
	window.location = url+'&supvid='+supvid;
}

function ExcluirVistoria(url, supvid){
	if(confirm("Deseja realmente excluir esta vistoria?")){
		window.location = url + '&subacao=VerificaVistoria&supvid=' + supvid;	
	}
}
		
function VerificaVistoria(caminho, supervisao){
	
	/*
	 * var wurl = window.location.href; var url_array = wurl.split("/"); var url =
	 * url_array[0]+"//"+url_array[2]+"/";
	 */
	
	var url = caminho_atual + '?modulo=principal/vistoria&acao=A&subacao=VerificaVistoria&AJAX=1';
	var parametros = "?supvid=" + supervisao;
	var myAjax = new Ajax.Request(
		url,
		{
			method: 'post',
			parameters: parametros,
			asynchronous: true,
			onComplete: function(resp) {
											
				if(!(resp.responseText == '1')){
					window.location = caminho+'&supvid='+supervisao;
					
				}else{
					alert("Não é possível alterar/deletar esta vistoria, pois existe(m) outra(s) após a mesma.")
				}	
			}
		});	
}
function ExcluirDocumento(url, arqid, aqoid){
	if(confirm("Deseja realmente excluir este documento ?")){
		window.location = url+'&aqoid='+aqoid+'&arqid='+arqid;
	}
}
DownloadArquivo = function(arqid){
	window.location = caminho_atual + '?modulo=principal/documentos&acao=A&requisicao=download&arqid='+arqid;
}

Atualizar = function(url,obrid){
			window.location = url+'&obrid='+obrid;
}

AbrirPopUp = function(url,nome,param){
	window.open(url,nome,param);
}
Ordem = function(valor){
	var index = valor;
	this.index = index;
}
AtualizaFotos = function(){
	
	/*
	 * var wurl = window.location.href; var url_array = wurl.split("/"); var url =
	 * url_array[0]+"//"+url_array[2]+"/";
	 */
	
	var inputs = window.document.getElementsByTagName("input");
	var ordem  = 0;
	var params = ""; 
	for (var k in inputs){
		var elemento = inputs[k];
		var t = ""+elemento.id+"";
		if (elemento.type == "hidden" && t.substr(0,8) == "imageBox") {
			if (elemento.id != "") {
				params += "ordem[]="+ ordem +"&box[]=imageBox"+ordem+"&foto[]="+elemento.value+"&";				
				ordem++;
				}
		}
				
	}
	
	var url = caminho_atual + '?modulo=principal/inserir_vistoria&acao=A&subacao=FotosVistoria&AJAX=1';
	var parametros = params;
	var myAjax = new Ajax.Request(
		url,
		{
			method: 'post',
			parameters: parametros,
			asynchronous: false,
			onComplete: function(resp) {
				UpdateListFoto();
			}
		});
}
ImageComponent = function(params){
	var a = window.open("plugins/component_foto.php" + params,"inserir_fotos","scrollbars=yes,height=540,width=630");
	a.focus();
}
ImageComponentSimple = function(params){
	var a = window.open("plugins/component_foto_simples.php" + params,"inserir_fotos","scrollbars=yes,height=540,width=630");
	a.focus();
}


function abreMapa(){
	var graulatitude = window.document.getElementById("graulatitude").value;
	var minlatitude  = window.document.getElementById("minlatitude").value;
	var seglatitude  = window.document.getElementById("seglatitude").value;
	var pololatitude = window.document.getElementById("pololatitude").value;
	
	var graulongitude = window.document.getElementById("graulongitude").value;
	var minlongitude  = window.document.getElementById("minlongitude").value;
	var seglongitude  = window.document.getElementById("seglongitude").value;
	
	var latitude  = ((( Number(seglatitude) / 60 ) + Number(minlatitude)) / 60 ) + Number(graulatitude);
	var longitude = ((( Number(seglongitude) / 60 ) + Number(minlongitude)) / 60 ) + Number(graulongitude);
	var obrid = document.getElementById("obrid").value;
	var janela=window.open(caminho_atual + '?modulo=principal/mapa&acao=A&longitude='+longitude+'&latitude='+latitude+'&polo='+pololatitude+'&obrid'+obrid, 'mapa','height=620,width=570,status=no,toolbar=no,menubar=no,scrollbars=no,location=no,resizable=no').focus();

}

function inserirEntidade(entid, orgid){
	if (entid){
			return windowOpen( caminho_atual + '?modulo=principal/inserir_entidade&acao=A&busca=entnumcpfcnpj&entid=' + entid + '&orgid=' + orgid,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		}else{
			return windowOpen( caminho_atual + '?modulo=principal/inserir_entidade&acao=A&orgid=' + orgid,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		}
}

function inserirEmpresa(entidempresa, tipo){
	if (entidempresa){ 
			return windowOpen( caminho_atual + '?modulo=principal/inserir_empresa&acao=A&busca=entnumcpfcnpj&tipo=' + tipo + '&entid=' + entidempresa,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		}else{
			return windowOpen( caminho_atual + '?modulo=principal/inserir_empresa&acao=A&tipo=' + tipo,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		}
}

function inserirResponsavel(entid, tipo){
	if (entid){ 
			return windowOpen( caminho_atual + '?modulo=principal/inserir_responsavel&acao=A&busca=entnumcpfcnpj&tipo=' + tipo + '&entid=' + entid,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		}else{
			return windowOpen( caminho_atual + '?modulo=principal/inserir_responsavel&acao=A&tipo=' + tipo,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		}
}

function inserirVistoriador(entid){
	if (entid){ 
		return windowOpen( caminho_atual + '?modulo=principal/inserir_vistoriador&acao=A&busca=entnumcpfcnpj&entid=' + entid,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
	}else{
		return windowOpen( caminho_atual + '?modulo=principal/inserir_vistoriador&acao=A','blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
	}
}


function atualizaResponsavel(entid){
	var index = window.document.getElementById('linha_'+entid).rowIndex;
	return windowOpen( caminho_atual + '?modulo=principal/inserir_responsavel&acao=A&busca=entnumcpfcnpj&entid=' + entid + '&tr=' + index,'blank','height=700,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' )	;
}

function inserirEtapas(){
	return windowOpen( caminho_atual + '?modulo=principal/inserir_etapas&acao=A','blank','height=450,width=400,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}

function adicionarFases( objeto ){
	return windowOpen( caminho_atual + '?modulo=principal/fases_licitacao&acao=A&objeto='+objeto,'blank','height=450,width=400,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}

function atualizaFase(id,flcid){
	return windowOpen( caminho_atual + '?modulo=principal/fases_licitacao&acao=A&tflid=' +id+'&flcid='+flcid,'blank','height=450,width=400,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}

function preencheValor()
{
	var custoTotal = document.getElementById("obrcustocontrato").value.replace('.', '');
	    custoTotal = custoTotal.replace('.','');
	    custoTotal = Number(custoTotal.replace(',', '.'));

	var area       = document.getElementById("obrqtdconstruida").value.replace('.', '');
	    area	   = area.replace('.','');
	    area       = Number(area.replace(',', '.'));

	var custo      = document.getElementById("obrcustounitqtdconstruida");
	var vlCusto = 0;
	
	if (custoTotal != "" && area != ""){ 
		vlCusto    = (new String((custoTotal / area).toFixed(2)).replace('.', ''));
		// alert( vlCusto + '<->' + (custoTotal / area).toFixed(2) );
		custo.value = mascaraglobal('###.###.###,##', vlCusto );
	}
}

function Validacao( superuser ){
	
	var mensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
	var validacao = true;
	
	var orgao = document.formulario.orgid.value;
	if ( orgao == '' && !superuser ){
		mensagem += 'Orgão \n';
		validacao = false;
	}
	
	var entidade = document.formulario.entid.value;
	if (entidade == '' && !superuser ){
		mensagem += 'Unidade ResponsÃ¡vel pela Obra \n';
		validacao = false;
	}
	
	var nomedaobra = document.formulario.obrdesc.value;
	if ( nomedaobra == '' ){
		alert('Nome da Obra é um campo obrigatório. \n');
		document.formulario.obrdesc.focus();
		return false;
	}
	
	var tipoobra = document.formulario.tobraid.value;
	if (tipoobra == '' && !superuser ){
		mensagem += 'Tipo de Obra \n';
		validacao = false;
	}
	
	var programa = document.formulario.prfid.value;
	if (programa == '' && !superuser ){
		mensagem += 'Programa / Fonte \n';
		validacao = false;
	}
	
	var obrcomposicao = document.formulario.obrcomposicao.value;
	if (obrcomposicao == '' && !superuser ){
		mensagem += 'Descrição / Composição da Obra \n';
		validacao = false;
	}
	
	if (obrcomposicao.length > 1500 ){
		alert( 'O campo Descrição / Composição da Obra não pode ter mais de 1500 caracteres.' );
		obrcomposicao.value = '';
		return false;
	}
	
	var classificacao = document.formulario.cloid.value;
	if (classificacao == '' && !superuser ){
		mensagem += 'Classificação da Obras \n';
		validacao = false;
	}else if( classificacao == 4 ){
		if( document.formulario.terid.value == '' || document.formulario.povid.value == '' ){
			alert("Esta é uma obra Indígena. Favor preencher os campos: \n\n Território Etno-Educacional;\n Povos.");
			return false;
		}
	}
	 
	var cep = document.formulario.endcep.value;
	if (cep == '' && !superuser ){
		mensagem += 'CEP \n';
		validacao = false;
	}
	
	var graulatitude = document.formulario.graulatitude.value;
	if(graulatitude > 90){
		alert("O grau de latitude informado não pode ser maior que 90°");
		document.formulario.graulatitude.focus();
		return false;
	}
	
	var minlatitude = document.formulario.minlatitude.value;
	if(minlatitude > 60){
		alert("O minuto de latitude informado não pode ser maior que 60");
		document.formulario.minlatitude.focus();
		return false;
	}

	var seglatitude = document.formulario.seglatitude.value;
	if(seglatitude > 60){
		alert("O segundo de latitude informado não pode ser maior que 60");
		document.formulario.seglatitude.focus();
		return false;
	}
	
	var graulongitude = document.formulario.graulongitude.value;
	if(graulongitude > 90){
		alert("O grau de longitude informado não pode ser maior que 90°");
		document.formulario.graulongitude.focus();
		return false;
	}
	
	var minlongitude = document.formulario.minlongitude.value;
	if(minlatitude > 60){
		alert("O minuto de longitude informado não pode ser maior que 60");
		document.formulario.minlongitude.focus();
		return false;
	}

	var seglongitude = document.formulario.seglongitude.value;
	if(seglatitude > 60){
		alert("O segundo de longitude informado não pode ser maior que 60");
		document.formulario.seglongitude.focus();
		return false;
	}
	
	if( !superuser ){
		
		var obrcustocontrato = document.formulario.obrvalorprevisto.value;
		if (obrcustocontrato == '0,00'){
			mensagem += 'Valor Previsto \n';
			validacao = false;
		}
		
		
		var aqiid = document.formulario.aqiid.value;
		if (aqiid == ''){
			mensagem += 'Situação do Imóvel \n';
			validacao = false;
		}
		
		var form = document.getElementById("formulario");
	
		var existeContato = false;
	
		for(i=0; i<form.length; i++) {
			
			campo = form.elements[i].id.substr(0,7);
			if(campo == "tprcid["){
				existeContato = true;
			}
			
		}

		if( graulatitude == ''){
			mensagem += 'Coordenadas Geográficas \n';
			validacao = false;
		}
		
		if( !existeContato ){
			mensagem += 'Contatos \n';
			validacao = false;
		}
		
	}
		
	if (!validacao){
		alert(mensagem);
	}
	
	if ( validacao ){
		window.document.getElementById("muncod").disabled = false;
		window.document.getElementById("endbai").disabled = false;
		window.document.getElementById("orgid").disabled = false;
	}
	
	return validacao;
	
}

function validaVistoria( formu, superuser )
{
	var form        = document.getElementById(formu);
	var numelements = form.elements.length;
		
	var mensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
	var validacao = true;

	var vistoriador = document.formulario.entidvistoriador;
	
	if( vistoriador.value == '' ){
		mensagem += 'Vistoriador \n';
		validacao = false;
	}
	
	var supobs = document.formulario.supobs;

	if ( supobs.value.length == 0 && ( superuser == 1 ) ){
		mensagem += 'Observação da Vistoria \n';
		validacao = false;
	}
	
	if ( supobs.value.length > 5000 ){
		alert( 'O Campo Observação da Vistoria deve ter no máximo 5000 caracteres!' );
		return false;
	}

// if (document.formulario.tpsid.value == ''){
// mensagem += 'Tipo da Vistoria \n';
// validacao = false;
// }
	
	var supvdt = document.formulario.supvdt.value;
	if ( supvdt == '' ){
		mensagem += 'Data da Vistoria \n';
		validacao = false;
	}

	var stoid = document.formulario.stoid.value;
	if (stoid == ''){
		mensagem += 'Situação Atual \n';
		validacao = false;
	}
	
	if ( stoid == 2 ){
	
		var tplid = document.formulario.tplid.value;	
		//if ( tplid == '' && ( superuser == 1 ) ){
			if ( tplid == '' ){
			mensagem += 'Tipo da Paralisação \n';
			validacao = false;
		}
		
		var hprobs = document.formulario.hprobs1.value;	
		if ( hprobs == '' && ( superuser == 1 ) ){
			mensagem += 'Observação da Paralisação \n';
			validacao = false;
		}
	
	}
		
	if ( stoid == 4 ){
		var obrdtprevprojetos = document.formulario.obrdtprevprojetos.value;
		//if ( obrdtprevprojetos == '' || ( superuser == 1 ) ){
		/*Verifica se o campo "Previsão de entrega do(s) projeto(s)" está vazio. Alteração feita dia 16/12/2010 as 12:22 H.*/
		if ( obrdtprevprojetos == '' ){
			mensagem += 'Previsão de entrega do(s) projeto(s)\n';
			validacao = false;
		}
	}

	if ( stoid ){
		if ( stoid != 4 && stoid != 5 && ( superuser == 1 ) ){
	
			var supprojespecificacoes = document.getElementsByName("supprojespecificacoes");
			if (supprojespecificacoes.checked == false){
				mensagem += 'Projeto/Especificações \n';
				validacao = false;
			}
		
			var supplacaobra = document.formulario.supplacaobra;
			if (supplacaobra.checked == false){
				mensagem += 'Placa da Obra \n';
				validacao = false;
			}
		
			/*
			 * 
			 * var supplacalocalterreno =
			 * document.formulario.supplacalocalterreno; if
			 * (supplacalocalterreno.checked == false){ mensagem += 'Placa
			 * Indicativa do Programa/Localização do Terreno \n'; validacao =
			 * false; }
			 * 
			 */
		
			var qlbid = document.formulario.qlbid.value;
			if (qlbid == ''){
				mensagem += 'Qualidade da Execução da Obra \n';
				validacao = false;
			}
		
			var dcnid = document.formulario.dcnid.value;
			if (dcnid == ''){
				mensagem += 'Desempenho da Construtora \n';
				validacao = false;
			}
			
		}
	}
	
	if ( document.formulario.supvdt.value != ""){
		if(!validaData(document.formulario.supvdt)){
			alert("A data informada é inválida");
			document.formulario.supvdt.focus();
			return false;
		}
	}

	if (!validacao){
		alert(mensagem);
	}

	return validacao;
}
function validaProjetoArquitetonico(form, superuser){

	//alert(superuser);
	
	var mensagem = "Os seguintes campos devem ser preenchidos: \n\n";
	var validacao = true;
	
	var tpaid = document.getElementById("tpaid");
	if (tpaid.value == ""){
		mensagem += "Tipo de Projeto \n";
		validacao = false;
	}
	
	var felid = document.getElementById("felid");
	if (felid.value == ""){
		mensagem += "Forma de Elaboração do projeto \n";
		validacao = false;
	}
	
	var tfpid = document.getElementById("tfpid");
	if (tfpid.value == ""){
		mensagem += "Fases do Projeto \n";
		validacao = false;
	}
	
	var fprdtiniciofaseprojeto = document.getElementById("fprdtiniciofaseprojeto");
	if (fprdtiniciofaseprojeto.value == ""){
		mensagem += "Data de Início \n";
		validacao = false;
	}else{
		if(!validaData(fprdtiniciofaseprojeto)){
			alert('Data inválida!');
			fprdtiniciofaseprojeto.focus();
			return false;
		}
	}
	
	var fprdtconclusaofaseprojeto = document.getElementById("fprdtconclusaofaseprojeto");
	if (fprdtconclusaofaseprojeto.value == ""){
		mensagem += "Data da Término \n";
		validacao = false;
	}else{
		if(!validaData(fprdtconclusaofaseprojeto)){
			alert('Data inválida!');
			fprdtconclusaofaseprojeto.focus();
			return false;
		}
	}
	
	
	var obData = new Data();
	
	if( (fprdtiniciofaseprojeto.value != '') && (fprdtconclusaofaseprojeto.value != '') ){
		if( !(obData.comparaData( fprdtiniciofaseprojeto.value, fprdtconclusaofaseprojeto.value, '<=' ) ) ){
			alert('Data final deve ser maior ou igual que a data inicial !');
			fprdtconclusaofaseprojeto.focus();
			return false;
		}
	}
	 
	if ( tfpid.value == "" ){
		alert("Fases do Projeto é um campo Indispensável\n");
		tfpid.focus();
		return false;
	}
	 
	if(superuser != 1){
		validacao = true;
	}
		
	if(!validacao){
		alert(mensagem);
		return validacao;
	}else{
		return validacao;
	}
	
}

function validaFases(){
	var mensagem = "Os seguintes campos devem ser preenchidos: \n\n";
	var validacao = true;
	
	var frpid = document.getElementById("frpid");
	if (frpid.value == ""){
		mensagem += "Tipo de Forma de Repasse de Recursos \n";
		validacao = false;
	}
	
	if (!validacao){
		alert(mensagem);
	}
	
	return validacao;
	
}

function validaInfraEstrutura(){
	
	var iexinfexistedimovel = document.formulario.iexinfexistedimovel;
	
	for (k = 0; k < iexinfexistedimovel.length; k++){	
		if (iexinfexistedimovel[k].checked == true){
			if (iexinfexistedimovel[k].value == 0){
				window.document.getElementById("iexareaconstruida").disabled = false;
				window.document.getElementById("umdid").disabled = false;
				window.document.getElementById("iexdescsumariaedificacao").disabled = false;
				window.document.getElementById("iexedificacaoreforma").disabled = false;
				window.document.getElementById("iexampliacao").disabled = false;				
			}
		}
	}
				
}
function EfetuarDownload(formid, caminho) {
	var formulario = document.getElementById(formid);
	formulario.method = 'post';
	formulario.action = caminho;
	formulario.submit();
}

function ExcluirRestricao(rstoid){
	if(confirm("Deseja realmente excluir esta restrição?")){
		window.location = caminho_atual + '?modulo=principal/restricao&acao=A&requisicao=excluir&rstoid=' + rstoid;
	}
		
}

function mostrarDtInauguracaoObra( stoid ){
	// Verifica se a situacao de obra é de inauguração
	if ( stoid == 7 ) {
		$('trDtInauguracao').show();
	}else{
		$('obrdtinauguracao').value = '';
		$('trDtInauguracao').hide();
	}
}


function ajaxCarregaTipologia( cloid ){
	if ( cloid ) {
		var url = caminho_atual + '?modulo=principal/cadastro&acao=A&subAcao=ajaxCarregaTipologia&AJAX=1&cloid=' + cloid;
			
		var myAjax = new Ajax.Updater(
			"spanComboTopologia",
			url,
			{
				method: 'get',
				asynchronous: false
			});
	}
	mostraDescricaoTipologia();
}

function ajaxCarregaTipologiaProg( prfid ){
	
	var cloid =  $('cloid');
	var tpoid =  $('tpoid');
	
	tpoid.disabled = true;
	cloid.disabled = true;
	$('obrcomposicao').readOnly = false;
	// $('obrcomposicao').value = '';

/*
 * if(cloid.value){ var url = caminho_atual +
 * '?modulo=principal/cadastro&acao=A&subAcao=ajaxCarregaTipologia&AJAX=1&cloid=' +
 * cloid.value + '&prfid=' + prfid;
 * 
 * var myAjax = new Ajax.Updater( "spanComboTopologia", url, { method: 'get',
 * asynchronous: false });
 * 
 * }else{
 */		
		if ( prfid ) {
			var url = caminho_atual + '?modulo=principal/cadastro&acao=A&subAcao=ajaxCarregaTipologia_Prog&AJAX=1&prfid=' + prfid;
				
			var myAjax = new Ajax.Updater(
				"spanComboTopologia",
				url,
				{
					method: 'get',
					asynchronous: false
				});
		}
// }
	tpoid.disabled = false;
	cloid.disabled = false;
	cloid.options[0].selected = true;
	mostraDescricaoTipologia();
}

function ajaxCarregaTipologiaClass( cloid ){
	
	var prfid =  $('prfid');
	var tpoid =  $('tpoid');
	
	tpoid.disabled = true;
	prfid.disabled = true;
	$('obrcomposicao').readOnly = false;
	// $('obrcomposicao').value = '';
	
	if(prfid.value){
	
		var url = caminho_atual + '?modulo=principal/cadastro&acao=A&subAcao=ajaxCarregaTipologia&AJAX=1&cloid=' + cloid + '&prfid=' + prfid.value;
			
		var myAjax = new Ajax.Updater(
			"spanComboTopologia",
			url,
			{
				method: 'get',
				asynchronous: false
			});
			
	}else if(prfid.value && cloid){
	
		var url = caminho_atual + '?modulo=principal/cadastro&acao=A&subAcao=ajaxCarregaTipologia_Prog&AJAX=1&cloid=' + cloid + '&prfid=' + prfid.value;
			
		var myAjax = new Ajax.Updater(
			"spanComboTopologia",
			url,
			{
				method: 'get',
				asynchronous: false
			});
			
	}
	else{
		
		var url = caminho_atual + '?modulo=principal/cadastro&acao=A&subAcao=ajaxCarregaTipologia_Class&AJAX=1&cloid=' + cloid;
			
		var myAjax = new Ajax.Updater(
			"spanComboTopologia",
			url,
			{
				method: 'get',
				asynchronous: false
			});
		
	}	
	tpoid.disabled = false;
	prfid.disabled = false;
	mostraDescricaoTipologia();
}

function mostraDescricaoTipologia(){
	if ( $('tpoid') ) {
		if( $('tpoid').value ) {
			$('obrcomposicao').value = arTipologias[$('tpoid').value];
			$('obrcomposicao').readOnly = true;
		}
	}
}

function buscaAditivo( covnumero ){

	var url = caminho_atual + '?modulo=principal/contratacao_da_obra&acao=A&ajax=2';
	var parametros = "&covnumero=" + covnumero;

	var myAjax = new Ajax.Request(
		url,
		{
			method: 'post',
			parameters: parametros,
			asynchronous: false,
			onComplete: function(resp) {
				window.opener.$('covaditivo').innerHTML = resp.responseText;
			}
		}
	);
}

function buscaApostilamento( covnumero ){

	var url = caminho_atual + '?modulo=principal/contratacao_da_obra&acao=A&ajax=3';
	var parametros = "&covnumero=" + covnumero;

	var myAjax = new Ajax.Request(
		url,
		{
			method: 'post',
			parameters: parametros,
			asynchronous: false,
			onComplete: function(resp) {
				window.opener.$('covapostilamento').innerHTML = resp.responseText;
			}
		}
	);
}

function buscaConvenio( id ){
	
	var url = caminho_atual + '?modulo=principal/contratacao_da_obra&acao=A&ajax=1';
	var parametros = "&convenio=" + id;

	var myAjax = new Ajax.Request(
		url,
		{
			method: 'post',
			parameters: parametros,
			asynchronous: false,
			onComplete: function(resp) {
				var json = resp.responseText.evalJSON();
				
				if ( id ){
				
					window.opener.$('covid').value            = json.covid;
					window.opener.$('covano').value           = json.covano;
					window.opener.$('covnumero').value        = json.covnumero;
					window.opener.$('covobjeto').value        = json.covobjeto;
					window.opener.$('covdetalhamento').value  = json.covdetalhamento;
					window.opener.$('covprocesso').value      = json.covprocesso;
					window.opener.$('covvlrconcedente').value = mascaraglobal('###.###.###,##', json.covvlrconcedente);
					window.opener.$('covvlrconvenente').value = mascaraglobal('###.###.###,##', json.covvlrconvenente);
					window.opener.$('covvalor').value         = mascaraglobal('###.###.###,##', json.covvalor);
					window.opener.$('covdtinicio').value   	  = json.covdtinicio;
					window.opener.$('covdtfinal').value       = json.covdtfinal;
					
				} else {
				
					window.opener.$('covid').value            = '';
					window.opener.$('covano').value           = '';
					window.opener.$('covnumero').value        = '';
					window.opener.$('covobjeto').value        = '';
					window.opener.$('covdetalhamento').value  = '';
					window.opener.$('covprocesso').value      = '';
					window.opener.$('covvlrconcedente').value = '';
					window.opener.$('covvlrconvenente').value = '';
					window.opener.$('covvalor').value         = '';
					window.opener.$('covdtinicio').value      = '';
					window.opener.$('covdtfinal').value       = '';
					
					alert('Este convênio não existe!');
					
				}
				buscaAditivo(json.covnumero);
				buscaApostilamento(json.covnumero);
			}
			
		}
	);
	
	window.close();
		
}

function removeConvenio(){

	$('covid').value            = '';
	$('covano').value           = '';
	$('covnumero').value        = '';
	$('covobjeto').value        = '';
	$('covdetalhamento').value  = '';
	$('covprocesso').value      = '';
	$('covvlrconcedente').value = '';
	$('covvlrconvenente').value = '';
	$('covvalor').value         = '';
	$('covdtinicio').value      = '';
	$('covdtfinal').value       = '';

	$('covaditivo').innerHTML       =   '<table width="90%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">'
									  + '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>'
									  + '</table>';
									  
	$('covapostilamento').innerHTML =   '<table width="90%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">'
									  + '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>'
									  + '</table>';

}

function validarPercentual( valor ){
	var inicio = new Number( document.getElementById( 'percentualinicial' ).value );
	var fim    = new Number( document.getElementById( 'percentualfinal' ).value );
	
	if ( inicio > fim ){
		alert('O valor percentual mínimo é maior do que o máximo');
		if ( fim > 5 ){
			document.getElementById( 'percentualinicial' ).value = fim - 5;
		}else{
			document.getElementById( 'percentualinicial' ).value = 0;
		}
	}
}

function envia_email( cpf ){
	e = "obras.php?modulo=sistema/geral/envia_email&acao=A&cpf=" + cpf;
	window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=590,height=490");
}


function envia_email_empresa( orsid ){
	e = "obras.php?modulo=sistema/geral/envia_email_empresa&acao=A&orsid=" + orsid;
	window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=590,height=490");
}


function criaObraNova( obrid ){
		
	if ( confirm('Esta obra será inativada, deseja realmente continuar?') ){
		
		var url = caminho_atual + '?modulo=principal/contratacao_da_obra&acao=A&requisicao=vincularobranova&obrid=' + obrid;
		window.location.href = url;
		
	}
	
}

function visualizarObraOrigem( obrid, tipo ){

	window.open('?modulo=principal/visualizarExtratoDaObra&acao=A&obrid=' + obrid,
				"extrato","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=700,height=500");

}

function abreCampus( entid, orgid ){
	
	if ( !entid ){
		
		document.getElementById("campus").style.display   = "none";
		document.getElementById("mostracampus").innerHTML = "";
		
	}else{
	
		var url = caminho_atual + '?modulo=principal/cadastro&acao=A';
		var parametros = "&ajax=4&entid=" + entid + "&orgid=" + orgid;
	
		var myAjax = new Ajax.Request(
			url,
			{
				method: 'post',
				parameters: parametros,
				asynchronous: false,
				onComplete: function(resp) {
					if (document.selection){
						document.getElementById("campus").style.display = "block";
		        	}else{
		        		document.getElementById("campus").style.display = "table-row";
			        }
			        
			        document.getElementById("mostracampus").innerHTML = resp.responseText;
				    
				}
			}
		);	
	}
	
}

function cadastraTermo(){
	window.location.href = "obras.php?modulo=principal/termodeajuste&acao=A";
}

function insereParticipante( entid, tipo ){
	if ( entid != '' ){
		window.open( 'obras.php?modulo=principal/inserir_participante&acao=A&tipo=' + tipo + '&entid=' + entid,
				 'Participante','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
	}else{
		window.open( 'obras.php?modulo=principal/inserir_participante&acao=A&tipo=' + tipo,
				 'Participante','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
	}
	
}

function excluiParticipante( ptaid ){
	if ( confirm("Deseja realmente excluir este participante?") ){
		window.location.href = "obras.php?modulo=principal/termodeajuste&acao=A&subacao=excluiparticipante&ptaid=" + ptaid;
	}
}

function insereUnidadeParticipante( ptaid ){
	window.open( 'obras.php?modulo=principal/inserir_unidade_participante&acao=A&subacao=cadastra&ptaid=' + ptaid,
				 'Participante','height=600,width=500,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}

function excluiUnidadeParticipante( ptaid ){
	if ( confirm("Deseja realmente excluir a unidade deste participante?") ){
		window.location.href = 'obras.php?modulo=principal/termodeajuste&acao=A&subacao=excluiunidade&ptaid=' + ptaid;
	}
}

function insereObra(){
	window.open( 'obras.php?modulo=principal/inserirobratermo&acao=A',
				 'Participante','height=700,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}

function atualizaTermo( traid, orgid ){
	window.location.href = "obras.php?modulo=principal/termodeajuste&acao=A&traid=" + traid + '&orgid=' + orgid;
}

function excluiTermo( traid ){
	if ( confirm("Deseja realmente excluir este termo?") ){
		window.location.href = "obras.php?modulo=principal/lista_de_termos&acao=A&requisicao=excluir&traid=" + traid;
	}
}

function deletaAnexo( ataid ){
	if ( confirm( "Deseja realmente excluir este anexo?" ) ){
		window.location.href = "obras.php?modulo=principal/doctermodeajuste&acao=A&requisicao=excluir&ataid=" + ataid;
	}
}

function downloadAnexo( arqid ){
	window.location.href = "obras.php?modulo=principal/doctermodeajuste&acao=A&requisicao=download&arqid=" + arqid;
}

function excluiObraTermo( otaid ){
	window.location.href = "obras.php?modulo=principal/obratermodeajuste&acao=A&requisicao=exclui&otaid=" + otaid;
}

function insereObsTermoObra( otaid, carga ){
	window.open( 'obras.php?modulo=principal/inserirobsobratermo&acao=A&otaid=' + otaid + '&carga=' + carga,
				 'Participante','height=300,width=500,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}

function pesquisarTermo(){

	var form = document.getElementById('formulario');
	form.submit();
	
}

function verTermoVinculado( traid ){
	window.open( 'obras.php?modulo=principal/vertermovinculado&acao=A&traid=' + traid,
				 'Termo','height=500,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}

/**
 * Valida os campos do cadastro/atualização de Termo de Ajuste
 * 
 * @author Fernando Araújo Bagno da Silva
 * @since 19/08/2009
 * 
 */
function obrasValidaTermoAjuste(){
	
	var form = document.getElementById( 'formulario' );
	
	var mensagem  = 'Os seguintes campos devem ser preenchidos: \n\n';
	var validacao = true;
	
	var orgid   	 = document.getElementById( 'orgid' );
	var traassunto   = document.getElementById( 'traassunto' );
	var tralocal	 = document.getElementById( 'tralocal' );
	var tradtcriacao = document.getElementById( 'tradtcriacao' );
	var tratextoata  = document.getElementById( 'tratextoata' );
		
	if ( orgid.value == '' ){
		mensagem += 'Órgão \n';
		validacao = false;
	}
	
	if ( traassunto.value == '' ){
		mensagem += 'Assunto \n';
		validacao = false;
	}
	 
	if ( tralocal.value == '' ){
		mensagem += 'Local \n';
		validacao = false;
	}
	
	if ( tradtcriacao.value == '' ){
		mensagem += 'Data \n';
		validacao = false;
	}
		
	if ( !validacao ){
		alert( mensagem );
		return validacao;
	}else{
		form.submit();
	}
	
}

function obrasValidaAnexoTermoAjuste(){
	
	var form = document.getElementById( 'formulario' );
	
	var mensagem  = 'Os seguintes campos devem ser preenchidos: \n\n';
	var validacao = true;
	
	var file		 = document.getElementById( 'file' );
	var arqdescricao = document.getElementById( 'arqdescricao' );
	var tpaid		 = document.getElementById( 'tpaid' );
	
	if ( file.value == '' ){
		mensagem += 'Arquivo \n';
		validacao = false;
	}
	
	if ( tpaid.value == '' ){
		mensagem += 'Tipo \n';
		validacao = false;
	}
	
	if ( arqdescricao.value == '' ){
		mensagem += 'Descrição \n';
		validacao = false;
	}
		
	if ( !validacao ){
		alert( mensagem );
		return validacao;
	}else{
		form.submit();
	}
	
}

function importaObras(){
	window.location.href = "obras.php?modulo=principal/importacao&acao=A&requisicao=importar";
}

function inserirEditorObras( entid ){
	window.open( caminho_atual + '?modulo=principal/selecionaResponsavelObra&acao=A&entid=' + entid, 'blank','height=400,width=600,status=no,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=no' );
}

function excluirResponsavel( rpuid ){
	if (confirm('Deseja realmente excluir este responsável?')){
		
		var index = window.document.getElementById('rpuid_' + rpuid).rowIndex;
		table = window.document.getElementById("responsaveisobra");
		table.deleteRow(index);
		
	}
}

function obrasValidaData( idIni, idFim ){
	
	var noIni, noFim;
	
	switch ( idIni.id ){
		case "obrdtassinaturacontrato":
			noIni = "Data de Assinatura do Contrato";
		break;
		case "dtiniciocontrato":
			noFim = "Data de Início do Contrato";
		break;
	}
	
	switch ( idFim.id ){
		case "obrdtordemservico":
			noFim = "Data da Ordem de Serviço";
		break;
		case "dtiniciocontrato":
			noFim = "Data de Início do Contrato";
		break;
		case "dtterminocontrato":
			noFim = "Data de Término do Contrato";
		break;
	}
	
	if( !validaDataMaior(idIni, idFim) ){
		alert( "A " + noFim + " não pode ser maior que a " + noIni + "!" );
		return false;
	}
	
}

// --- Funções JS da SUPERVISÂO--- //

function obrSelecionaUfEmpresa(){
	janela('?modulo=principal/supervisao/inserirUfEmpresa&acao=A', 500, 580, 'inserirObra');
}

function obrSelecionaObras(){
	janela('?modulo=principal/supervisao/inserirObraRepositorio&acao=A', 900, 480, 'inserirObra');
}

function obrPesquisaListaObras(){
	
	var entidunidade 	   = document.getElementById( 'entidunidade' );
	var estado       	   = document.getElementById( 'estado' );
	var repdtlimiteinicial = document.getElementById( 'repdtlimiteinicial' );
	var repdtlimitefinal   = document.getElementById( 'repdtlimitefinal' );
	var tobaid   		   = document.getElementById( 'tobaid' );
	var stoid   		   = document.getElementById( 'stoid' );
	var cloid   		   = document.getElementById( 'cloid' );
	var prfid   		   = document.getElementById( 'prfid' );
	/*var entidunidade2      = document.getElementById( 'entidunidade2' );*/
	var obrtextobusca      = document.getElementById( 'obrtextobusca' );
	/*var fotos     		   = document.getElementsByName( 'foto' );*/
	var vlrmenor		   = $("#vlrmenor").val();
	var vlrmaior		   = $("#vlrmaior").val();
	
	/*var foto;
	for(i=0; i<fotos.length; i++){
		if( fotos[i].checked ){
			foto = fotos[i].value;
		} 
	}*/
	
	var foto = "";
	
	var foto_1		   = $('#foto_1:checked').val();
	var foto_2		   = $('#foto_2:checked').val();
	var foto_3		   = $('#foto_3:checked').val();
	
	if( foto_1 != null ){
		foto = 'S';
	}else if(foto_2 != null){
		foto = 'N';
	}

	var supervisao = "";
	
	var supervisao_1		   = $('#supervisao_1:checked').val();
	var supervisao_2		   = $('#supervisao_2:checked').val();
	var supervisao_3		   = $('#supervisao_3:checked').val();
	
	if( supervisao_1 != null ){
		supervisao = 'S';
	}else if(supervisao_2 != null){
		supervisao = 'N';
	}
	
	/*
	var vistorias     	   = document.getElementsByName( 'vistoria' );
	var vistoria;
	for(i=0; i<vistorias.length; i++){
		if( vistorias[i].checked ){
			vistoria = vistorias[i].value;
		} 
	}
	*/
	
	var vistoria = "";
	
	var vistoria_1		   = $('#vistoria_1:checked').val();
	var vistoria_2		   = $('#vistoria_2:checked').val();
	var vistoria_3		   = $('#vistoria_3:checked').val();
	
	if( vistoria_1 != null ){
		vistoria = 'S';
	}else if(vistoria_2 != null){
		vistoria = 'N';
	}

	/*
	var restricaos     	   = document.getElementsByName( 'restricao' );
	var restricao;
	for(i=0; i<restricaos.length; i++){
		if( restricaos[i].checked ){
			restricao = restricaos[i].value;
		} 
	}
	*/
	
	var restricao = "";
	
	var restricao_1		   = $('#restricao_1:checked').val();
	var restricao_2		   = $('#restricao_2:checked').val();
	var restricao_3		   = $('#restricao_3:checked').val();
	
	if( restricao_1 != null ){
		restricao = 'S';
	}else if(restricao_2 != null){
		restricao = 'N';
	}
	
	/*
	var planointernos      = document.getElementsByName( 'planointerno' );
	var planointerno;
	for(i=0; i<planointernos.length; i++){
		if( planointernos[i].checked ){
			planointerno = planointernos[i].value;
		} 
	}
	*/
	var planointerno = "";
	
	var planointerno_1		   = $('#planointerno_1:checked').val();
	var planointerno_2		   = $('#planointerno_2:checked').val();
	var planointerno_3		   = $('#planointerno_3:checked').val();
	
	if( planointerno_1 != null ){
		planointerno = 'S';
	}else if(planointerno_2 != null){
		planointerno = 'N';
	}
	
	/*
	var aditivos       	   = document.getElementsByName( 'aditivo' );
	var aditivo;
	for(i=0; i<aditivos.length; i++){
		if( aditivos[i].checked ){
			aditivo = aditivos[i].value;
		} 
	}
	*/
	
	var aditivo = "";
	
	var aditivo_1		   = $('#aditivo_1:checked').val();
	var aditivo_2		   = $('#aditivo_2:checked').val();
	var aditivo_3		   = $('#aditivo_3:checked').val();
	
	if( aditivo_1 != null ){
		aditivo = 'S';
	}else if(aditivo_2 != null){
		aditivo = 'N';
	}
	
	var percentualinicial  = document.getElementById( 'percentualinicial' );
	var percentualfinal    = document.getElementById( 'percentualfinal' );
	
	if ( supervisao != '' || vlrmenor.value != '' || vlrmaior.value != '' || entidunidade.value != '' || estado.value != '' || repdtlimiteinicial.value != '' || repdtlimitefinal.value != '' || tobaid.value != '' || stoid.value != '' || cloid.value != '' || prfid.value != '' /*|| entidunidade2.value != ''*/ || obrtextobusca.value != '' || foto/*.value*/ != '' || vistoria/*.value*/ != '' || restricao/*.value*/ != '' || planointerno/*.value*/ != '' || aditivo/*.value*/ != '' || percentualinicial.value != '' || percentualfinal.value != '' ){
		divCarregando( this );
		$.ajax({
				url: "obras.php?modulo=principal/supervisao/inserirObraRepositorio&acao=A",
				type: "POST",
				async: true,
				cache: false,
				data: '&requisicao=lista&entidunidade=' + entidunidade.value + '&estuf=' + estado.value + '&repdtlimiteinicial=' + repdtlimiteinicial.value + '&repdtlimitefinal=' + repdtlimitefinal.value + '&tobaid=' + tobaid.value + '&stoid=' + stoid.value + '&cloid=' + cloid.value + '&prfid=' + prfid.value /*+ '&entidunidade2=' + entidunidade2.value*/ + '&obrtextobusca=' + obrtextobusca.value + '&foto=' + foto/*.value*/ + '&vistoria=' + vistoria/*.value*/ + '&restricao=' + restricao/*.value*/ + '&planointerno=' + planointerno/*.value*/ + '&aditivo=' + aditivo/*.value*/ + '&percentualinicial=' + percentualinicial.value + '&percentualfinal=' + percentualfinal.value + '&supervisao=' + supervisao + '&vlrmenor=' + vlrmenor + '&vlrmaior=' + vlrmaior,
				dataType: "html",
				success: function(msg){
					$('#listaObrasRepositorio').html(msg);
					divCarregado();
		      }
		   })
	}else{
		alert('Para visualizar as obras é necessário escolher pelo menos um filtro!');
		return false;
	}
	
}

function obrExcluiObraRepositorio( obrid ){
	
	if( confirm("Deseja realmente excluir esta obra do repositório?") ){
		window.location.href = "?modulo=principal/supervisao/repositorio&acao=A&requisicao=excluir&obrid=" + obrid;
	}
	
}

function obrListaUfObras( orgid ){

	var url = '?modulo=principal/supervisao/criarLote&acao=A&requisicao=listauf&orgid=' + orgid;
	
	var myAjax = new Ajax.Updater(
		"trUf",
		url,
		{
			method: 'post',
			asynchronous: false
		});
	
}

function obrValidaGrupo(){
	
	if( document.getElementById( 'estuf' ).value == '' ){
		alert( "Para criar um novo grupo é necessário incluir ao menos uma obra!" );
		return false;
	}else{
		document.getElementById( 'obrFormLote' ).submit();
	}
	
}

function obrCheckaItemLote(){
	
	var form = document.getElementById( "obrFormLote" );
	
	for( i = 0; i < form.length; i++ ){
		if ( form.elements[i].type == "checkbox" ){
			if( form.elements[i].id.substr(0,6) == "repid_" ){
				for( k = 0; k < obrItensLote.length; k++ ){
					if( obrItensLote[k] == form.elements[i].value ){
						form.elements[i].checked = true;
					}
				}
			}
		}
	}
	
}

function in_array(string, array) {
   for (i = 0; i < array.length; i++)
   {
      if(array[i] == string)
      {
         return true;
      }
   }
return false;
}

function obrListaMunicipios( estuf ){

	$.ajax({
	      url: "?modulo=principal/supervisao/criarLote&acao=A",
	      type: "POST",
	      async: true,
	      cache: false,
	      data: '&requisicao2=mostramunicipios&estuf=' + estuf,
	      dataType: "html",
	      success: function(msg){
	         
	         $('#listaMunicipios').html(msg);
	       
	      }
	     
	   })

}

function obrListaObras( estuf ){
	
	$.ajax({
	      url: "?modulo=principal/supervisao/criarLote&acao=A",
	      type: "POST",
	      async: true,
	      cache: false,
	      data: '&requisicao=lista&estuf=' + estuf,
	      dataType: "html",
	      success: function(msg){
	         
	         $('#listaObrasRepositorioLote').html(msg);
	       
	       	 obrCheckaItemLote();
			
			 if( estuf && obrItensLote.length == 0 ){
			 	document.getElementById("estuf").value = estuf;
			 }
	         
	      }
	     
	   })
		
	
}

function obrListaObrasMunicipio( estuf, muncod ){

	$.ajax({
	      url: "?modulo=principal/supervisao/criarLote&acao=A",
	      type: "POST",
	      async: true,
	      cache: false,
	      data: '&requisicao=listapormunicipio&muncod=' + muncod + '&estuf=' + estuf,
	      dataType: "html",
	      success: function(msg){
	         
	         $('#listaObrasRepositorioLote').html(msg);
	       
	       	 obrCheckaItemLote();
			
			 if( estuf && obrItensLote.length == 0 ){
			 	document.getElementById("estuf").value = estuf;
			 }
	         
	      }
	     
	   })
		
}

function atualizaSetaLinhaLote( tabela ){
	
	var tamanho = tabela.rows.length;
	
	if(tamanho > 2){
		tabela.rows[1].cells[0].innerHTML = "<img src='/imagens/seta_cimad.gif' border='0'/> <img src='/imagens/seta_baixo.gif' style='cursor:pointer;' border='0' onclick='obrTrocaPrioridade(\"desce\", this);'/>";
		tabela.rows[tamanho - 1].cells[0].innerHTML = "<img src='/imagens/seta_cima.gif' style='cursor:pointer;' border='0' onclick='obrTrocaPrioridade(\"sobe\", this);'/> <img src='/imagens/seta_baixod.gif' border='0'/>";
	}
	if(tamanho == 2){
		tabela.rows[1].cells[0].innerHTML = "<img src='/imagens/seta_cimad.gif' border='0'/> <img src='/imagens/seta_baixod.gif' border='0'/>";
	}
	
}

function obrTrocaPrioridade( acao, objeto ){
	
	var linha1, linha2;
	var tabela = window.document.getElementById( "tbListaRota" );
	var linha = objeto.parentNode.parentNode.rowIndex;
	
	switch(acao) {
		case 'sobe':
			linha1 = linha;
			linha2 = (linha-1);
		break;
		case 'desce':
			linha1 = linha;
			linha2 = (linha+1);
		break;
		
	}
	
	var trAjuda = tabela.rows[linha1].innerHTML;
	
	if( linha2 != 0 && linha2 < (tabela.rows.length-1) ){
		
		var seqUm   = tabela.rows[linha1].cells[0].innerHTML;
		var seqDois = tabela.rows[linha2].cells[0].innerHTML;
		
		var setaUm   = tabela.rows[linha1].cells[8].innerHTML;
		var setaDois = tabela.rows[linha2].cells[8].innerHTML;
		
		$(tabela.rows[linha1]).html( tabela.rows[linha2].innerHTML );
		$(tabela.rows[linha2]).html( trAjuda );
// tabela.rows[linha1].innerHTML = tabela.rows[linha2].innerHTML;
// tabela.rows[linha2].innerHTML = trAjuda;
		
		tabela.rows[linha1].cells[8].innerHTML = setaUm;
		tabela.rows[linha2].cells[8].innerHTML = setaDois;
		
		tabela.rows[linha1].cells[0].innerHTML = seqUm;
		tabela.rows[linha2].cells[0].innerHTML = seqDois;
		
	}

} 

function obrVerificaData( repid ){

	var dtInicio  = window.document.getElementById( "repdtlimiteinicial_" + repid );
	var dtTermino = window.document.getElementById( "repdtlimitefinal_" + repid );
	
	if( !validaDataMaior( dtInicio, dtTermino ) ){
		alert( "A data de Início deve ser menor que a de Término!" );
		dtTermino.focus();
		return false;
	}else{
		return true;
	}

}

function obrIncluiObraNoLote( obrid, repid, nomeObra, municipio, area, unidademedida, unidade, situacao, uf, tipoensino, convenio, percentual ){
	
	var tabela = window.document.getElementById( "tblLoteObras" );
	var trInfo = window.document.getElementById( "trRelecioneObras" );
	
	if( document.getElementById( "repid_" + repid ).checked == true ){
		
		if( document.getElementById("estuf").value != uf ){
			alert( "Não é pertido criar Grupos de Supervisão com obras de UF's diferentes!" );
			document.getElementById( "repid_" + repid ).checked = false
			return false;
		}
		
		obrItensLote.push( repid );
		
		var tamanho = tabela.rows.length;
		
		var existeTotal = document.getElementById('totalGrupo').rowIndex;
		
		if( existeTotal ){
			
			var linha = tabela.insertRow( existeTotal );
			
			var totalObras = document.getElementById('nTotalObrasGrupo').innerHTML;
			document.getElementById('nTotalObrasGrupo').innerHTML = Number(totalObras) + 1;
			
			var totalSuperior = document.getElementById('nTotalObrasSuperior').innerHTML;
			if( tipoensino == 'Educação Superior' ){
				document.getElementById('nTotalObrasSuperior').innerHTML = Number(totalSuperior) + 1;
			} 
			
			var totalProfissional = document.getElementById('nTotalObrasProfissional').innerHTML;
			if( tipoensino == 'Educação Profissional' ){
				document.getElementById('nTotalObrasProfissional').innerHTML = Number(totalProfissional) + 1;
			}
			
			var totalBasica = document.getElementById('nTotalObrasBasica').innerHTML;
			if( tipoensino == 'Educação Básica' ){
				document.getElementById('nTotalObrasBasica').innerHTML = Number(totalBasica) + 1;
			}
			
		}else{
			var linha = tabela.insertRow(tamanho);
		}
		
		if( tamanho % 2 ){
			linha.style.backgroundColor = "#f4f4f4"
		}else{
			linha.style.backgroundColor = "#e0e0e0";
		}
		
		linha.id = "obralote_" + repid;
		
		var colNome      = linha.insertCell(0);
		var colProce     = linha.insertCell(1);
		var colArea      = linha.insertCell(2);
		var colMunicipio = linha.insertCell(3);
		var colUnidade   = linha.insertCell(4);
		var colTpEnsino  = linha.insertCell(5);
		var colSituacao  = linha.insertCell(6);
		var colPercen    = linha.insertCell(7);
		
		// var colDtInicio = linha.insertCell(5);
		// var colDtFinal = linha.insertCell(6);
		
		colProce.style.textAlign   = "center";
		colArea.style.textAlign   = "right";
		
		colPercen.style.textAlign = "right";
		colPercen.style.color     = "#0066cc";
		
		var atencao = "";
		if( area == 0.00 ){
			atencao = "<img src='../imagens/restricao.png' style='vertical-align:middle;' title='Não existe área construída informada para esta obra!'/> ";
		}
		
		colNome.innerHTML      = "<img src='../imagens/consultar.gif' style='vertical-align:middle; cursor: pointer;' title='Dados da Obra' onclick='obrVerDados(" + obrid + ", \"obra\");'/> " +
								 "<img src='../imagens/globo_terrestre.png' style='vertical-align:middle; cursor: pointer;' title='Visualizar mapa' onclick=\"janela('?modulo=principal/supervisao/mapaObra&acao=A&obrid=" + obrid + "', 600, 585, 'mapaGrupo');\"/> (" +  obrid + ") " + nomeObra + " (nº do convênio: " + convenio + ") <input type='hidden' name='repid[]' id='repid_" + repid + "' value='" + repid + "'/>";
		colProce.innerHTML     = "-";
		colArea.innerHTML      = atencao + area + " " + unidademedida;
		colMunicipio.innerHTML = municipio;
		colUnidade.innerHTML   = unidade;
		colTpEnsino.innerHTML  = tipoensino;
		colSituacao.innerHTML  = situacao;
		colPercen.innerHTML    = percentual.replace(".", ",");
		
		/*
		 * 
		 * colDtInicio.innerHTML = "<input type='text' size='12'
		 * style='text-align: right;' name='repdtlimiteinicial_" + repid + "' " +
		 * "id='repdtlimiteinicial_" + repid + "' maxlength='10' value=''
		 * onKeyUp='this.value=mascaraglobal( \"##/##/####\", this.value );' " +
		 * "class='normal' onmouseover='MouseOver(this);'
		 * onfocus='MouseClick(this);this.select();'
		 * onmouseout='MouseOut(this);' " + "onblur='MouseBlur(this),
		 * VerificaData(this, this.value);'/> " + "<a
		 * href='javascript:show_calendar(\"formulario.repdtlimiteinicial_" +
		 * repid + "\");'> " + " <img src='../imagens/calendario.gif' width='16'
		 * height='15' border='0' align='absmiddle' alt=''> " + "</a> ";
		 * colDtFinal.innerHTML = "<input type='text' size='12'
		 * style='text-align: right;' name='repdtlimitefinal_" + repid + "' " +
		 * "id='repdtlimitefinal_" + repid + "' maxlength='10' value=''
		 * onKeyUp='this.value=mascaraglobal( \"##/##/####\", this.value );' " +
		 * "class='normal' onmouseover='MouseOver(this);'
		 * onfocus='MouseClick(this);this.select();'
		 * onmouseout='MouseOut(this);' " + "onblur='MouseBlur(this),
		 * VerificaData(this, this.value);' onchange='obrVerificaData(" + repid +
		 * ");'/> " + "<a
		 * href='javascript:show_calendar(\"formulario.repdtlimitefinal_" +
		 * repid + "\");'> " + " <img src='../imagens/calendario.gif' width='16'
		 * height='15' border='0' align='absmiddle' alt=''> " + "</a> ";
		 * 
		 */
		
	}else{
		
		for( i = 0; i < obrItensLote.length; i++ ){
			if( obrItensLote[i] == repid ){
				obrItensLote.splice(i, 1);
			}
		}
		
		if( obrItensLote.length == 0 ){
			document.getElementById("estuf").value = null;
		}
		
		var linha = document.getElementById( "obralote_" + repid );
		
		tabela.deleteRow(linha.rowIndex);
		
		var existeTotal = document.getElementById('totalGrupo').rowIndex;
		
		if( existeTotal ){
			
			var totalObras = document.getElementById('nTotalObrasGrupo').innerHTML;
			document.getElementById('nTotalObrasGrupo').innerHTML = Number(totalObras) - 1;
			
			var totalSuperior = document.getElementById('nTotalObrasSuperior').innerHTML;
			if( tipoensino == 'Educação Superior' ){
				document.getElementById('nTotalObrasSuperior').innerHTML = Number(totalSuperior) - 1;
			} 
			
			var totalProfissional = document.getElementById('nTotalObrasProfissional').innerHTML;
			if( tipoensino == 'Educação Profissional' ){
				document.getElementById('nTotalObrasProfissional').innerHTML = Number(totalProfissional) - 1;
			}
			
			var totalBasica = document.getElementById('nTotalObrasBasica').innerHTML;
			if( tipoensino == 'Educação Básica' ){
				document.getElementById('nTotalObrasBasica').innerHTML = Number(totalBasica) - 1;
			}
			
		}
		
	}
	
}

function obrExcluirGrupo( gpdid ){
	
	if( confirm("Deseja realmente excluir este grupo?") ){
		window.location.href = '?modulo=principal/supervisao/distribuicao&acao=A&requisicao=excluir&gpdid=' + gpdid;
	}
	
}

function obrAlterarEmpresa( epcid ){
	
	window.location.href = '?modulo=principal/supervisao/inserirEmpresaContratada&acao=A&epcid=' + epcid;
	
}

function obrExcluirEmpresa( epcid ){
	
	if( confirm("Deseja realmente excluir esta empresa?") ){
		window.location.href = '?modulo=principal/supervisao/listaEmpresas&acao=A&requisicao=excluir&epcid=' + epcid;
	}
	
}

function obrValidaEmpresa(){

	var obData = new Data();
	
	if( obData.comparaData( document.getElementById('epcdtiniciocontrato').value, document.getElementById('epcdtfinalcontrato').value, ">" ) ){
		alert( "Data de Término do Contrato deve ser maior que a Data de Início!" );
		return false;
	}
	

	var formulario = document.getElementById( 'obrFormPesquisaEmpresa' );
	var estuf      = document.getElementById( 'estuf' );
	var entid      = document.getElementById( 'entid' );
	var mensagem   = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
	var validacao  = true;
	
	if( !entid.value ){
		mensagem += 'Empresa \n';
		validacao = false;
	}
	
	if( estuf.options[0].value == '' ){
		mensagem += 'UF\'s Atendidas \n';
		validacao = false;
	}
	
	if( !validacao ){
		alert( mensagem );
		return validacao;
	}else{
		selectAllOptions( formulario.estuf );
		formulario.submit();
	}
	
}

function obrExcluiRespEmpresa( entid ){
	if (confirm('Deseja realmente excluir este Resposnável?')){
		var index = window.document.getElementById('linha_' + entid).rowIndex;
		table = window.document.getElementById("respEmpresas");
		table.deleteRow(index);
	}
}

function obrAbreListaRota( gpdid ){
	window.location.href = '?modulo=principal/supervisao/listaDeRotas&acao=A&gpdid=' + gpdid;
}

function obrCriarRota( gpdid ){
	window.location.href = '?modulo=principal/supervisao/criarRota&acao=A&requisicao=novarota&gpdid=' + gpdid;
}

function obrCheckaObraRota(){
	
	var form1 = window.opener.document.getElementById('obrFormRotas');
	var form2 = document.getElementById('obrFormObras');
	
	for( i = 0; i < form1.length; i++ ){
		if ( form1.elements[i].type == "hidden" ){
			if( form1.elements[i].id.substr(0,6) == "itgid_" ){
				for( k = 0; k < form2.length; k++ ){
					if ( form2.elements[k].type == "checkbox" ){
						if( form2.elements[k].id.substr(0,6) == "itgid_" ){
							if( form1.elements[i].value == form2.elements[k].value ){
								form2.elements[k].checked = true;
							}
						}
					}
				}
			}
		}
	}
	
}

function obrIncluiObraRota(itgid, obrdesc){

	var tabela = window.opener.document.getElementById( 'tbListaRota' );

	var tamanho = tabela.rows.length;
	
	if( document.getElementById( "itgid_" + itgid ).checked == true ){
	
		var linha   = tabela.insertRow(tamanho);
		
		if( tamanho % 2 ){
			linha.style.backgroundColor = "#f4f4f4";
		}else{
			linha.style.backgroundColor = "#e0e0e0";
		}
		
		linha.id = "obrarota_" + itgid;
			
		var colSequencia = linha.insertCell(0);
		var colNome      = linha.insertCell(1);
		var colDesloca   = linha.insertCell(2);
		var colDistancia = linha.insertCell(3);
		var colTempo     = linha.insertCell(4);
	
		colSequencia.style.textAlign = "center";
		colDesloca.style.textAlign   = "center";
		colDistancia.style.textAlign = "center";
		colTempo.style.textAlign 	 = "center";
	
		var campoHidden = '<input type="hidden" name="itgid[]" id="itgid_' + itgid + '" value="' + itgid + '"/>';
	
		colSequencia.innerHTML = tamanho - 1;
		colNome.innerHTML      = campoHidden
							   + '<input type="text" title="" style="width: 60ex; text-align: left;"' 
							   + 'readonly="readonly" class="disabled" value="' + obrdesc.toUpperCase() + '"'
							   + 'maxlength="" size="58" name="entnome"/>';
	
		colDesloca.innerHTML   = '<select id="tdeid_'+ itgid + '" name="tdeid['+ itgid + ']" class="campoestilo">'
							   + '    <option value="" selected="selected">Selecione...</option>'
							   + '    <option value="1">Rodoviário</option>'
							   + '    <option value="2">Não Rodoviário</option>'
							   + '</select>';
	
		colDistancia.innerHTML = '<input type="text" title="" style="width: 15ex; text-align: left;" ' 
							   + 'class="normal" value="" maxlength="8" size="12" name="trjkm['+ itgid + ']" id="trjkm_'+ itgid + '" '
							   + 'onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" '
							   + 'onKeyUp="this.value=mascaraglobal(\'#####,##\',this.value);"/>';
		
		colTempo.innerHTML 	   = '<input type="text" title="" style="width: 9ex; text-align: left;" ' 
							   + 'class="normal" value="" maxlength="5" size="6" name="trjtempo['+ itgid + ']" id="trjtempo_'+ itgid + '" '
							   + 'onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" '
							   + 'onKeyUp="this.value=mascaraglobal(\'##:##\',this.value);" />';
	
	}else{
		
		var linha = window.opener.document.getElementById( "obrarota_" + itgid );
		
		tabela.deleteRow(linha.rowIndex);
		
	}
	
}

function obrBuscaRota( rotid ){
	window.location.href = '?modulo=principal/supervisao/criarRota&acao=A&requisicao=buscatrajetoria&rotid=' + rotid;
}

function obrExcluiRota( rotid ){
	if( confirm("Deseja realmente excluir esta rota?") ){
		window.location.href = '?modulo=principal/supervisao/listaDeRotas&acao=A&requisicao=excluir&rotid=' + rotid;
	}
}

function obrAprovaRota( rotid ){
	if( confirm("Deseja realmente aprovar esta rota?") ){
		window.location.href = '?modulo=principal/supervisao/criarRota&acao=A&requisicao=aprovar&rotid=' + rotid;
	}
}

function obrCancelaAprovacao( gpdid ){
	if( confirm("Deseja realmente cancelar a aprovação da rota deste grupo?") ){
		window.location.href = '?modulo=principal/supervisao/criarRota&acao=A&requisicao=cancelaraprovacao&gpdid=' + gpdid;
	}
}

function obrProporRota( rotid ){
	if( confirm("Deseja realmente propor uma nova rota?") ){
		document.getElementById("requisicao").value = "proporrota";
		document.getElementById("obrFormRotas").submit();
	}
}

function obrEmiteOS(){
	janela('?modulo=principal/supervisao/emitirOS&acao=A', 600, 585, 'emitirOS');
}

function obrVerDados( id, tipo ){
	janela('?modulo=principal/supervisao/verDados&acao=A&id=' + id + '&tipo=' + tipo, 600, 550, 'verDados');
}

function abreMapaObras(tipoendereco){
	var graulatitude = window.document.getElementById("graulatitude"+tipoendereco).value;
	var minlatitude  = window.document.getElementById("minlatitude"+tipoendereco).value;
	var seglatitude  = window.document.getElementById("seglatitude"+tipoendereco).value;
	var pololatitude = window.document.getElementById("pololatitude"+tipoendereco).value;
	
	var graulongitude = window.document.getElementById("graulongitude"+tipoendereco).value;
	var minlongitude  = window.document.getElementById("minlongitude"+tipoendereco).value;
	var seglongitude  = window.document.getElementById("seglongitude"+tipoendereco).value;
	
	if(!graulatitude || !minlatitude || !seglatitude){
		var latitude = document.getElementById("latitude"+tipoendereco).value;
	}else{
		var latitude  = ((( Number(seglatitude) / 60 ) + Number(minlatitude)) / 60 ) + Number(graulatitude);
	}
		
	if(!graulongitude || !minlongitude || !seglongitude){
		var longitude = document.getElementById("longitude"+tipoendereco).value;
	}else{
		var longitude = ((( Number(seglongitude) / 60 ) + Number(minlongitude)) / 60 ) + Number(graulongitude);
	}
	
	var entid = window.document.getElementById("entid" + tipoendereco).value;
	
	janela('../apigoogle/php/mapa_padraon.php?redirectOFF=true&tipoendereco='+trim(tipoendereco)+'&longitude='+trim(longitude)+'&latitude='+trim(latitude)+'&polo='+trim(pololatitude)+'&entid='+trim(entid), 'mapa','height=620,width=570,status=no,toolbar=no,menubar=no,scrollbars=no,location=no,resizable=no');
	
}

function abreRotaObras(i){
	
	var trajetos = document.getElementById('trajetos_id').value;
	var arrTrj=trajetos.split(",");	
	var arrEnd = new Array();
	var rotas = "";
		
	if(i == "total" || i == 0){
		var y = i == 0;
		arrTrj.length - 1;
		var i = i == "total" ? arrTrj.length - 1 : 0;
	}else{
		var y = i - 1;
	}
	
	for (x=y;x<=i;x++){
		if(document.getElementById('graulatitude' + arrTrj[x])){
	    	rotas += retornaEndereco(arrTrj[x]);
	    	if(i > 0 && x < i)
	    		rotas += "::rota::"
	    }		
	}
	
	janela('../apigoogle/php/mapa_rotas.php?rotas=' + rotas, 'mapa','width=550,height=700,width=570,status=no,toolbar=no,menubar=no,scrollbars=no,location=no,resizable=no');
	
}

function retornaEndereco(tipoendereco){
	
	var graulatitude = document.getElementById("graulatitude"+tipoendereco).value;
	var minlatitude  = window.document.getElementById("minlatitude"+tipoendereco).value;
	var seglatitude  = window.document.getElementById("seglatitude"+tipoendereco).value;
	var pololatitude = window.document.getElementById("pololatitude"+tipoendereco).value;
	
	var graulongitude = window.document.getElementById("graulongitude"+tipoendereco).value;
	var minlongitude  = window.document.getElementById("minlongitude"+tipoendereco).value;
	var seglongitude  = window.document.getElementById("seglongitude"+tipoendereco).value;
	
	if(document.getElementById("latitude"+tipoendereco).value){
		var latitude = document.getElementById("latitude"+tipoendereco).value;
	}else{
		var latitude  = ((( Number(seglatitude) / 60 ) + Number(minlatitude)) / 60 ) + Number(graulatitude);
	}
		
	if(document.getElementById("longitude"+tipoendereco).value){
		var longitude = document.getElementById("longitude"+tipoendereco).value;
	}else{
		var longitude = ((( Number(seglongitude) / 60 ) + Number(minlongitude)) / 60 ) + Number(graulongitude);
	}
	
	return latitude + "," + longitude;
	
}

function obrInserirProcTecnico( itgid ){
	janela('?modulo=principal/supervisao/inserirProcedimentoTecnico&acao=A&itgid=' + itgid, 540, 380, 'inserirProcedimentoTecnico');
}

function obrInserirObs( entid ){
	janela('?modulo=principal/supervisao/inserirObservacaoTrajetoria&acao=A&entid=' + entid, 540, 380, 'inserirProcedimentoTecnico');
}

function obrSelecionaTodosProcedimentos( itgid ){
	
	var form = document.getElementById('obrFormLote');
	
	if( document.getElementById('selecionaTodos_' + itgid).checked == true ){

		for( i = 0; i < form.length; i++ ) {
		
			if( form.elements[i].id.search('tppid_' + itgid) > -1 ){
				form.elements[i].checked = true;
			}
		}
	
	}else{
	
		for( i = 0; i < form.length; i++ ) {
		
			if( form.elements[i].id.search('tppid_' + itgid) > -1 ){
				form.elements[i].checked = false;
			}
		}
	
	}
	
}

function obrVerificaTodosProcedimentos( itgid ){
	
	var form = document.getElementById('obrFormLote');
	var todos = true;
	
	for( i = 0; i < form.length; i++ ) {
		if( form.elements[i].id.search('tppid_' + itgid) > -1 ){
			if( form.elements[i].checked == false ){
				todos = false;
			}else{
				continue;
			}
		}
	}

	if( todos ){
		document.getElementById('selecionaTodos_' + itgid).checked = true; 
	}

}

function obrVerificaArea( id, area ){
	
	if( area == 0 ){
	
		alert("Para inserir uma obra no repositório é necessário que a mesma tenha área construída informada!");
		document.getElementById( "obrid_" + id ).checked = false;
		return false;
		
	}
	
}