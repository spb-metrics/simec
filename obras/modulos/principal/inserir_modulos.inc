<?php 

/**
 * Função que lista os módulos 
 *
 */
function montaPopupModulos(){
	$sql = pg_query("SELECT tmaid, tmadesc FROM obras.tipomoduloampliacao");
	$count = "1";
	while (($dados = pg_fetch_array($sql)) != false){
		$tmaid = $dados['tmaid'];
		$tmadesc = $dados['tmadesc'];
		$cor = "#f4f4f4";
		$count++;
		$nome = "modulo_".$tmaid;
		if ($count % 2){
			$cor = "#e0e0e0";
		}
		
		echo "
			<script type=\"text/javascript\"> id_modulos.push('$nome'); </script>
			<tr bgcolor=\"$cor\">
				<td>
					<input id=\"".$nome."\" name=\"".$tmadesc."\" type=\"checkbox\" value=\"" . $tmaid . "\" onclick=\"marcaItem('".$tmadesc."', ".$tmaid.", '".$nome."');\">" . $tmadesc . "
				</td>
			</tr>
		";	
	};
}


?>
<html>
	<head>
		<title>Inserir Módulos</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<script type="text/javascript">	
		var id_modulos = new Array();
		
		function selecionaTodos() {
			var i, modulo, descricao, id, nome;
						
			for(i=0; i<id_modulos.length; i++) {
				modulo = document.getElementById(id_modulos[i]);
								
				if((document.getElementById("selecionar_todos").checked == true)&&(modulo.checked == false)) {
					modulo.checked = true;
					descricao = modulo.name;
					id = modulo.value;
					nome = modulo.id;
										
					marcaItem(descricao, id, nome);
					
				} else if((document.getElementById("selecionar_todos").checked == false)&&(modulo.checked == true)) {
					modulo.checked = false;
					descricao = modulo.name;
					id = modulo.value;
					nome = modulo.id;
													
					marcaItem(descricao, id, nome);
					
				}			
			}
		}
		
		function marcaItem(descricao, id, nome) {				
			var tabela = window.opener.document.getElementById("tabela_modulos");			
				
			if(document.getElementById(nome).checked == true) {				
				var tamanho = tabela.rows.length;
							
				if(tamanho == 1) {			
					var linha = tabela.insertRow(tamanho);
					linha.style.backgroundColor = "#f4f4f4";
				} else {
					var linha = tabela.insertRow(Number(tamanho)-1);
					if(tabela.rows[tamanho-2].style.backgroundColor == "rgb(224, 224, 224)") {
						linha.style.backgroundColor = "#f4f4f4";					
					} else {
						linha.style.backgroundColor = "#e0e0e0";					
					}
				}
												
				linha.id = "linha_"+id;
									
				var colAcao = linha.insertCell(0);
				var colDescricao = linha.insertCell(1);
				
				colAcao.style.textAlign = "center";
				
				colAcao.innerHTML = "<span onclick='excluiItem("+id+");'><img src='/imagens/excluir.gif' style='cursor:pointer;' border='0' title='Excluir'></span>";
				colDescricao.innerHTML = descricao + '<input type="hidden" id="tmaid['+id+']" name="tmaid[]" value="'+id+'">';
			
			}else{
				var linha = window.opener.document.getElementById("linha_"+id).rowIndex;
				tabela.deleteRow(linha);
					
				if(tabela.rows.length == 2) {
					tabela.deleteRow(1);
				}
			}					
		}
		/*
		function verificaNovoModulo(){
			var modulo = document.formModulos.tmadesc;
			
			if (modulo.value == ""){
				alert("É necessário preencher o nome do novo módulo");
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
						Inserir Módulos 
					</label>
				</td>
			</tr>
		</table>
		
		<br/>
		<!--
		<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<tr bgcolor="#cdcdcd">
				<td colspan="2" valign="top">
					<form method="post" name="formModulos" onsubmit="return verificaNovoModulo();" action="?modulo=principal/inserir_modulos&acao=Y">
						Novo: 
						
						<input type="submit" name="inserir_modulo" value="Inserir Módulo"> 
					</form>
				</td>
			</tr>
		</table>
		-->
		
		<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<tr bgcolor="#cdcdcd">
				<td colspan="2" valign="top">
					<strong>Selecione a(s) Módulo(s)</strong>
				</td>
			</tr>
			<?php 
				montaPopupModulos();
			?>
			<tr>
				<td>
					<input type="checkbox" value="todos" name="selecionar_todos" id="selecionar_todos" onclick="selecionaTodos();"> <strong>Selecionar Todos</strong>
				</td>
			</tr>
			 <tr bgcolor="#C0C0C0">
				<td>
					<input type="button" name="ok" value="Ok" onclick="self.close();">
				</td>
			</tr>
		</table>
		
		<script type="text/javascript">
			var tabela = window.opener.document.getElementById("tabela_modulos");
			var i, id_linha, check;	
			
			for(i = 1; i < tabela.rows.length; i++) {
				id_linha = tabela.rows[i].id;
				id_linha = id_linha.substr(6);
				
				check = document.getElementById("modulo_"+id_linha);
				check.checked = true;					
			}
		</script>
	</body>
</html>