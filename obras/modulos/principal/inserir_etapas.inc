<?php 

function podeExcluir($itcid) {
	$sql_excluir = pg_query("SELECT 
								count(*) as num 
							 FROM 
							 	obras.itenscomposicaoobra itco 
							 WHERE 
							 	itco.obrid = ".$_SESSION["obra"]["obrid"]." 
							 	AND itco.itcid = ".$itcid."
							  	AND (itco.icovlritem is not null
							  	OR itco.icopercprojperiodo is not null) AND
							  	icostatus = 'A'");
	$dados = pg_fetch_array($sql_excluir);
		
	return $dados["num"];
}

function montaPopupEtapas(){
	$sql = pg_query("SELECT itcid, itcdesc, itcdescservico FROM obras.itenscomposicao order by itcordem");
	$count = "1";
	while (($dados = pg_fetch_array($sql)) != false){
		$itcid = $dados['itcid'];
		$itcdesc = $dados['itcdesc'];
		$itcdescservico = $dados['itcdescservico'];
		$cor = "#f4f4f4";
		$count++;
		$nome = "etapa_".$itcid;
		if ($count % 2){
			$cor = "#e0e0e0";
		}
		
		if (trim($itcdescservico)!='')
		$title = "onmouseover=\"return escape('$itcdescservico');\"";
		else
		$title = "";
		
		if(podeExcluir($itcid)) 
			$key = "accesskey=\"x\"";
		else 
			$key = "";
		
		echo "
			<script type=\"text/javascript\"> id_etapas.push('$nome'); </script>
			<tr bgcolor=\"$cor\"  onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='$cor';\">
				<td $title>
				<input id=\"".$nome."\" name=\"".$itcdesc."\" type=\"checkbox\" value=\"" . $itcid . "\" $key onclick=\"marcaItem('".$itcdesc."', ".$itcid.", '".$nome."', ".podeExcluir($itcid).");\">" . $itcdesc . "
				</td>
			</tr>
		";	
	};
}

?>
<html>
	<head>
		<title>Inserir Serviços</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<script type="text/javascript">	
		var id_etapas = new Array();
		
		function selecionaTodos() {
			var i, etapa, descricao, id, nome;
						
			for(i=0; i<id_etapas.length; i++) {
				etapa = document.getElementById(id_etapas[i]);
								
				if((document.getElementById("selecionar_todos").checked == true)&&(etapa.checked == false)) {
					etapa.checked = true;
					descricao = etapa.name;
					id = etapa.value;
					nome = etapa.id;
					if(etapa.accessKey == "x")
						podeExcluir = 1;
					else
						podeExcluir = 0;
										
					marcaItem(descricao, id, nome, podeExcluir);
				} else if((document.getElementById("selecionar_todos").checked == false)&&(etapa.checked == true)) {
					etapa.checked = false;
					descricao = etapa.name;
					id = etapa.value;
					nome = etapa.id;
					if(etapa.accessKey == "x")
						podeExcluir = 1;
					else
						podeExcluir = 0;	
								
					marcaItem(descricao, id, nome, podeExcluir);
				}			
			}
		}
		
		function marcaItem(descricao, id, nome, podeExcluir) {				
			var tabela = window.opener.document.getElementById("tabela_etapas");
			
			//alert(descricao+' - '+id+' - '+nome+' - '+podeExcluir);
			//alert(tabela.rows.length);
			if(tabela.rows.length > 2) {
				//window.opener.document.getElementById("desce_dis").src = '/imagens/seta_baixo.gif';
				//window.opener.document.getElementById("desce_dis").style.cursor = 'pointer';
				//window.opener.document.getElementById("desce_dis").id = 'sobe_dis';
			}
			
			if(document.getElementById(nome).checked == true) {				
				var tamanho = tabela.rows.length;
							
				if(tamanho == 1) {			
					var linha = tabela.insertRow(tamanho);
								
					linha.style.backgroundColor = "#f4f4f4";
				} 
				else {
					var linha = tabela.insertRow(Number(tamanho)-3);
					
					if(tabela.rows[tamanho-2].style.backgroundColor == "rgb(224, 224, 224)") {
						linha.style.backgroundColor = "#f4f4f4";					
					} else {
						linha.style.backgroundColor = "#e0e0e0";					
					}
				}
												
				linha.id = "linha_"+id;
				
									
				var colOrdem 	 = linha.insertCell(0);
				var colAcao 	 = linha.insertCell(1);
				var colDescricao = linha.insertCell(2);
				var colInicio    = linha.insertCell(3);
				var colTermino   = linha.insertCell(4);
				var colValor     = linha.insertCell(5);
				var colReferente = linha.insertCell(6);
				var colSobreObra = linha.insertCell(7);
				var colExecutado = linha.insertCell(8);		
				
				colOrdem.style.textAlign 	 = "center";
				colAcao.style.textAlign 	 = "center";
				colValor.style.textAlign     = "center";
				colReferente.style.textAlign = "center";
				colInicio.style.textAlign    = "center";
				colTermino.style.textAlign   = "center";
				colSobreObra.style.textAlign = "right";
				colExecutado.style.textAlign = "right";

				if(tabela.rows.length == 2) {
					colOrdem.innerHTML = "<span><a onclick=\"troca_linhas('tabela_etapas','cima',this.parentNode.parentNode.parentNode.rowIndex,4);\"><img src='/imagens/seta_cimad.gif' border='0' title='Subir'></a></span> <span><a><img src='/imagens/seta_baixod.gif' id='desce_dis' border='0' title='Descer'></a></span>";
				}else {
					colOrdem.innerHTML = "<span><a onclick=\"troca_linhas('tabela_etapas','cima',this.parentNode.parentNode.parentNode.rowIndex,4);\"><img src='/imagens/seta_cima.gif' style='cursor:pointer;' border='0' title='Subir'></a></span> <span><a><img src='/imagens/seta_baixod.gif' id='desce_dis' border='0' title='Descer'></a></span>";
					window.opener.document.getElementById("desce_dis").src = '/imagens/seta_baixo.gif';
					window.opener.document.getElementById("desce_dis").style.cursor = 'pointer';
					window.opener.document.getElementById("desce_dis").id = 'removido';
				}
				
				colAcao.innerHTML 	   = "<span onclick='excluiItem(this.parentNode.parentNode.rowIndex);'><img src='/imagens/excluir.gif' style='cursor:pointer;' border='0' title='Excluir'></span>";
				colDescricao.innerHTML = descricao;
				colValor.innerHTML 	   = "<input class='CampoEstilo' type='text' value='0,00' name='valoritem_"+id+"' id='valoritem_"+id+"' size='15' maxlength='14' onblur='preencheRef(" + id + "); calculaTotal();' onkeyup='this.value=mascaraglobal(\"###.###.###,##\", this.value);' onfocus='this.select();' onkeypress='reais(this,event); preencheRef("+id+");'>";
				colReferente.innerHTML = "<input class='CampoEstilo' type='text' value='0,00' name='mostraref_"+id+"' id='mostraref_"+id+"' size='6' maxlength='6' onkeypress='reais(this,event)' onkeydown='backspace(this,event);' onblur='preencheVal(" + id + ");calculaTotal();' disabled='disabled'> % <input type='hidden' name='referente_"+id+"' id='referente_"+id+"' value='0,00'/>";
				colInicio.innerHTML    = "<input type='text' onblur='MouseBlur(this), VerificaData(this, this.value);' onmouseout='MouseOut(this);' onfocus='MouseClick(this); this.select();' onmouseover='MouseOver(this);' class='normal' onkeyup=\"this.value=mascaraglobal('##/##/####',this.value);\" value='' maxlength='10' style='text-align: right;' size='12' name='dtinicial_" + id + "' id='dtinicial_" + id + "' onchange=\"obrValidaDataEtapa(this, 'obrdtinicio');\"/> <a href=\"javascript:show_calendar('formulario.dtinicial_" + id + "');\"><img border='0' align='absmiddle' onclick='displayCalendar(document.getElementById(\"dtinicial_" + id + "\"),\"dd/mm/yyyy\",this)' title='Escolha uma Data' style='cursor: pointer;' src='../includes/JsLibrary/date/displaycalendar/images/calendario.gif'>";
				colTermino.innerHTML   = "<input type='text' onblur='MouseBlur(this), VerificaData(this, this.value);' onmouseout='MouseOut(this);' onfocus='MouseClick(this); this.select();' onmouseover='MouseOver(this);' class='normal' onkeyup=\"this.value=mascaraglobal('##/##/####',this.value);\" value='' maxlength='10' style='text-align: right;' size='12' name='datafinal_" + id + "' id='datafinal_" + id + "' onchange=\"obrValidaDataEtapa(this, '','obrdttermino');obrValidaData(document.getElementById('dtinicial_" + id + "'), this);\"/> <a href=\"javascript:show_calendar('formulario.datafinal_" + id + "');\"><img border='0' align='absmiddle' onclick='displayCalendar(document.getElementById(\"datafinal_" + id + "\"),\"dd/mm/yyyy\",this)' title='Escolha uma Data' style='cursor: pointer;' src='../includes/JsLibrary/date/displaycalendar/images/calendario.gif'>";
				colSobreObra.innerHTML = "0,00";
				colExecutado.innerHTML = "0,00";
														
				if(tamanho == 1) {
					//linha total
					var linhaTotal = tabela.insertRow(Number(tamanho)+1);
					linhaTotal.id = "tr_total";
					linhaTotal.style.backgroundColor = "#FFFFFF";
					
					var zeroColTotal = linhaTotal.insertCell(0);
					var primeiraColTotal = linhaTotal.insertCell(1);
					var segundaColTotal = linhaTotal.insertCell(2);
					var terceiraColTotal = linhaTotal.insertCell(3);
					var quartaColTotal = linhaTotal.insertCell(4);
					var quintaColTotal = linhaTotal.insertCell(5);
					var sextaColTotal = linhaTotal.insertCell(6);
					var setimaColTotal = linhaTotal.insertCell(7);
					var oitavaColTotal = linhaTotal.insertCell(8);
					
					quartaColTotal.style.textAlign = "right";
					quintaColTotal.style.textAlign = "center";
					sextaColTotal.style.textAlign = "center";
					
					zeroColTotal.innerHTML = "";
					primeiraColTotal.innerHTML = "";
					segundaColTotal.innerHTML = "";
					terceiraColTotal.innerHTML = "";
					quartaColTotal.innerHTML = "<strong>Total</strong>";
					quintaColTotal.innerHTML = "<input class='CampoEstilo' type='text' id='totalv' size='15' maxlength='14' value='0,00' disabled='disabled'>";
					sextaColTotal.innerHTML = "<input class='CampoEstilo' type='text' id='total' size='6' maxlength='6' value='0,00' disabled='disabled'> %";


					//linha valor do contrato
					var custo = window.opener.document.getElementById("obrcustocontrato");
					var custoaux = mascaraglobal('###.###.###.###,##', custo.value);
					linhaTotal = tabela.insertRow(Number(tamanho)+2);
					linhaTotal.id = "tr_vlcontrato";
					linhaTotal.style.backgroundColor = "#FFFFFF";
					
					zeroColTotal = linhaTotal.insertCell(0);
					primeiraColTotal = linhaTotal.insertCell(1);
					segundaColTotal = linhaTotal.insertCell(2);
					terceiraColTotal = linhaTotal.insertCell(3);
					quartaColTotal = linhaTotal.insertCell(4);
					quintaColTotal = linhaTotal.insertCell(5);
					sextaColTotal = linhaTotal.insertCell(6);
					setimaColTotal = linhaTotal.insertCell(7);
					oitavaColTotal = linhaTotal.insertCell(8);
					
					quartaColTotal.style.textAlign = "right";
					quintaColTotal.style.textAlign = "center";
					sextaColTotal.style.textAlign = "center";
					
					zeroColTotal.innerHTML = "";
					primeiraColTotal.innerHTML = "";
					segundaColTotal.innerHTML = "";
					terceiraColTotal.innerHTML = "";
					quartaColTotal.innerHTML = "<strong>Valor do Contrato</strong>";
					quintaColTotal.innerHTML = "<input class='CampoEstilo' type='text' id='vl_contrato' size='15' maxlength='14' value='"+custoaux+"' disabled='disabled'>";
					sextaColTotal.innerHTML = "<input class='CampoEstilo' type='text' id='vl_porcento' size='6' maxlength='6' value='100,00' disabled='disabled'> %";


					//linha valor restante
					linhaTotal = tabela.insertRow(Number(tamanho)+3);
					linhaTotal.id = "tr_vlrestante";
					linhaTotal.style.backgroundColor = "#FFFFFF";
					
					var zeroColTotal = linhaTotal.insertCell(0);
					var primeiraColTotal = linhaTotal.insertCell(1);
					var segundaColTotal = linhaTotal.insertCell(2);
					var terceiraColTotal = linhaTotal.insertCell(3);
					var quartaColTotal = linhaTotal.insertCell(4);
					quintaColTotal = linhaTotal.insertCell(5);
					sextaColTotal = linhaTotal.insertCell(6);
					setimaColTotal = linhaTotal.insertCell(7);
					oitavaColTotal = linhaTotal.insertCell(8);
					
					quartaColTotal.style.textAlign = "right";
					quintaColTotal.style.textAlign = "center";
					sextaColTotal.style.textAlign = "center";
					
					zeroColTotal.innerHTML = "";
					primeiraColTotal.innerHTML = "";
					segundaColTotal.innerHTML = "";
					terceiraColTotal.innerHTML = "";
					quartaColTotal.innerHTML = "<strong>Valor Restante</strong>";
					quintaColTotal.innerHTML = "<input class='CampoEstilo' type='text' id='rest_totalv' size='15' maxlength='14' value='"+custoaux+"' disabled='disabled'>";
					sextaColTotal.innerHTML = "<input class='CampoEstilo' type='text' id='rest_total' size='6' maxlength='6' value='100,00' disabled='disabled'> %";

				}				
			}
			else {	
				if(podeExcluir) {					
					document.getElementById(nome).checked = true;
					alert("Existe supervisão cadastrada para esta etapa.");					
				}
				else {
					//alert(id);
					var linha = window.opener.document.getElementById("linha_"+id).rowIndex;
					//alert(linha);
					tabela.deleteRow(linha);
					
					if(tabela.rows.length == 4) {
						tabela.deleteRow(3);
						tabela.deleteRow(2);
						tabela.deleteRow(1);
					}
				}
			}	
		}
		
		/*
		function verificaNovaEtapa(){
		
			var etapa = document.formEtapas.itcdesc;
			
			if (etapa.value == ""){
				alert("É necessário preencher o nome da nova etapa");
				return false;
			}
		
		}
		*/
		
	</script>
	<body>
		<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none; width:100%;">
			<tr>
				<td width="100%" align="center">
					<label class="TituloTela" style="color: #000000;"> 
						Inserir Serviços 
					</label>
				</td>
			</tr>
		</table>
		
		<br/>
		
		<!--
		 
		<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<tr bgcolor="#cdcdcd">
				<td colspan="2" valign="top">
					<form method="post" name="formEtapas" onsubmit="return verificaNovaEtapa();" action="?modulo=principal/inserir_etapas&acao=Y">
						Nova: 
						
						<input type="submit" name="inserir_etapa" value="Inserir Etapa" onclick=""> 
					</form>
				</td>
			</tr>
		</table>
		
		-->
		
		<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<tr bgcolor="#cdcdcd">
				<td colspan="2" valign="top">
					<strong>Selecione a(s) Serviço(s)</strong>
				</td>
			</tr>
			<tr>
				<td>
					<input type="checkbox" value="todos" name="selecionar_todos" id="selecionar_todos" onclick="selecionaTodos();"> <strong>Selecionar Todos</strong>
				</td>
			</tr>
			<?php 
				montaPopupEtapas();
			?>
			<tr bgcolor="#C0C0C0">
				<td>
					<input type="button" name="ok" value="Ok" onclick="self.close();">
				</td>
			</tr>
		</table>
		
		<script type="text/javascript">
			var tabela = window.opener.document.getElementById("tabela_etapas");
			var i, id_linha, check;	
			
			for(i=1; i<tabela.rows.length; i++) {
				id_linha = tabela.rows[i].id;
				id_linha = id_linha.substr(6);
				
				if(document.getElementById("etapa_"+id_linha)) {
					check = document.getElementById("etapa_"+id_linha);
					check.checked = true;
				}					
			}
		</script>
		
	<script type="text/javascript" src="../includes/remedial.js"></script>
	<script language="JavaScript" src="../includes/wz_tooltip.js"></script>  
	</body>
</html>