function validar(rstoid){
	
	var mensagem           = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
	var validacao          = true;
	var trtid              = document.getElementById("trtid").value;
	var rstdesc	           = document.getElementById("rstdesc");
	var fsrid	           = document.getElementById("fsrid");
	var rstdescprovidencia = document.getElementById("rstdescprovidencia");
	
	
	if (rstoid){
		var rstdtsuperacao = document.formulario.rstdtsuperacao.value;
		var rstsituacao	   = document.getElementById("rstsituacao");
	}
	
	rstdesc.value = tinyMCE.getContent("rstdesc");
	var rstdesc_l = rstdesc.value.length;
	
	rstdescprovidencia.value = tinyMCE.getContent("rstdescprovidencia");
	var rstdescprovidencia_l = rstdescprovidencia.value.length;

	if( fsrid.value == "" ){
		mensagem += 'Situa��o da Obra na Restri��o \n';
		validacao = false;
	}

	if (trtid == ""){
		mensagem += 'Tipo de Restri��o \n';
		validacao = false;
	}
	
	if (rstdesc.value == ""){
		mensagem += 'Restri��o \n';
		validacao = false;
	}
	
	if (rstdesc_l > 500){
		alert("O limite de 500 caracteres foi ultrapassado");
		return false;
	}
	
	if ( rstdescprovidencia_l > 500 ){
		alert('O campo Provid�ncia deve ter no m�ximo 500 caracteres!');
		return false;
	}
	
	if (rstoid){
		if (rstsituacao.checked){
			if (rstdtsuperacao == "" && rstsituacao.value == true){
				alert("� necess�rio informar a data da supera��o.");
				return false;
			}
		}
	}	
	
	if (document.formulario.rstdtprevisaoregularizacao.value != ""){
		if (!validaData(document.formulario.rstdtprevisaoregularizacao)){
			alert("A data de previs�o de regulariza��o informada � inv�lida");
			document.formulario.rstdtprevisaoregularizacao.focus();
			return false;
		}
	}
	
	if (rstoid){
		if (rstdtsuperacao != "" && rstsituacao.value == true){
			if (!validaData(document.formulario.rstdtsuperacao)){
				alert("A data de supera��o informada � inv�lida");
				document.formulario.rstdtsuperacao.focus();
				return false;
			}
		}
	}
	
	if (!validacao){
		alert(mensagem);
	}
	
	return validacao;
	
}
