<?php

switch ($_REQUEST["requisicao"]) {
	
	case "listamunicipios":
		
		if( !empty( $_REQUEST["estuf"] ) ){
			
			$sql = "SELECT
						'<center><input type=\"checkbox\" id=\"estuf[' || muncod || ']\" value=\"' || estuf || '|' || muncod || '\" onclick=\"selecionaMunicipio(\'' || muncod || '\', this.value, \'' || mundescricao || '\');\"/></center>' as acao,
						mundescricao
					FROM
						territorios.municipio
					WHERE
						estuf = '{$_REQUEST["estuf"]}'
					ORDER BY
						mundescricao";
			
			$cabecalho = array( "Selecione", "Município Sede do Escritório" );
			$db->monta_lista_simples( $sql, $cabecalho, 1000, 30, 'N', '100%');
			
		}else{
			print "Selecione um estado...";
		}
		
		die;
		
	break;
	
}

// titulo da página
monta_titulo( "UF's de Atendimento", "" );

?>

<html>
	<head>
		<title><?php echo $GLOBALS['parametros_sistema_tela']['nome_e_orgao']; ?></title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script type="text/javascript" src="/includes/JQuery/jquery.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script>
		
			var obrEstufEmpresa    = new Array();
			var obrMuncipioEmpresa = new Array();
			
			function obrListaEmpresaMunicipios( estuf ){
				
				divCarregando();
				
				$.ajax({
			      url: "?modulo=principal/supervisao/inserirUfEmpresa&acao=A",
			      type: "POST",
			      async: true,
			      cache: false,
			      data: '&requisicao=listamunicipios&estuf=' + estuf,
			      dataType: "html",
			      success: function(msg){
			         
			         $('#divListaMunicipios').html(msg);
			         obrCheckaMunicipioEmpresa();
			       	 divCarregado();
			       
			      }
			     
			   })
						
				
			}
			
			function selecionaMunicipio( muncod, valor, mundescricao ){
			
				var campoSelect = document.getElementById('muncod');
				var tamanho = campoSelect.options.length;

				var estuf = valor.slice(0,2);
	
				if (document.getElementById('estuf[' + muncod + ']').checked == true){
					
					for( i = 0; i < obrEstufEmpresa.length; i++ ){
						if( obrEstufEmpresa[i] == estuf ){
							alert( "É permitido inserir apenas um município por estado!" );
							document.getElementById('estuf[' + muncod + ']').checked = false;
							return false;
						}
					}
					
					campoSelect.options[tamanho] = new Option( estuf + ' - ' + mundescricao,  valor, false, false);
					sortSelect(campoSelect);
				
					obrEstufEmpresa.push( estuf );
					obrMuncipioEmpresa.push( valor );
				
				}else {
					
					for( i=0; i <= campoSelect.length-1; i++ ){
						if ( valor == campoSelect.options[i].value ){
							campoSelect.options[i] = null;
						}
					}
					
					for( i = 0; i < obrMuncipioEmpresa.length; i++ ){
						if( obrMuncipioEmpresa[i] == muncod ){
							obrMuncipioEmpresa.splice(i, 1);
						}
					}
					
					for( i = 0; i < obrEstufEmpresa.length; i++ ){
						if( obrEstufEmpresa[i] == estuf ){
							obrEstufEmpresa.splice(i, 1);
						}
					}
						
				}
		
			}
			
			function obrCheckaMunicipioEmpresa(){
				
				var form = document.getElementById( "obrFormMunEmpresa" );
				
				for( i = 0; i < form.length; i++ ){
					if ( form.elements[i].type == "checkbox" ){
						if( form.elements[i].id.substr(0,6) == "estuf[" ){
							for( k = 0; k < obrMuncipioEmpresa.length; k++ ){
								if( obrMuncipioEmpresa[k] == form.elements[i].value ){
									form.elements[i].checked = true;
								}
							}
						}
					}
				}
				
			}
			
			function obrAtribuiMunicipio(){
				
				var campoSelect = document.getElementById('muncod');
				var tamanho     = campoSelect.options.length;
				
				if( tamanho < 1 ){
					alert( "É necessário selecionar ao menos um município!" );
					return false;
				}else{
					
					var estuf = window.opener.document.getElementById('estuf');
					estuf.remove(0)
					
					for( i=0; i <= campoSelect.length-1; i++ ){
						estuf.options[i] = new Option( campoSelect.options[i].text, campoSelect.options[i].value, false, false);
						sortSelect(estuf);
					}

					self.close();
					
				}
				
			}
			
			function obrBuscaMunCadastrados(){
					
				var estuf 		= window.opener.document.getElementById('estuf');
				var campoSelect = document.getElementById('muncod');
				
				for( i=0; i <= estuf.length-1; i++ ){
					
					if( estuf.options[i].value != "" ){
						campoSelect.options[i] = new Option( estuf.options[i].text, estuf.options[i].value, false, false);
						sortSelect(campoSelect);
						
						obrEstufEmpresa.push( estuf.options[i].value.slice(0,2) );
						obrMuncipioEmpresa.push( estuf.options[i].value );
					}
					
				}

			}
			
		</script>
	</head>
	<body onload="obrBuscaMunCadastrados();">
		<form action="" method="post" id="obrFormMunEmpresa">
			<table class="tabela" bgcolor="#FFFFFF" cellSpacing="1" cellPadding=3 align="center">
				<tr>
					<td class="SubTituloDireita">Selecione o Estado:</td>
					<td>
						<?php 
						
							$sql = "SELECT 
										estuf as codigo,
										estdescricao as descricao
									FROM
										territorios.estado
									ORDER BY
										estuf";
							
							$db->monta_combo( "estuf", $sql, "S", "Selecione...", "obrListaEmpresaMunicipios(this.value);", "", "", "", "S", "estuf" );
						
						?>
					</td>
				</tr>
			</table>
			<center>
				<div style="width: 95%; height: 65%; color:#ee0000; overflow:auto; font-size: 8pt;" id="divListaMunicipios">
					Selecione um estado...
				</div>
				<select multiple="multiple" class="CampoEstilo" style="width: 95%;" size="8" name="muncod[]" id="muncod">
				</select>
			</center>
			<table class="tabela" bgcolor="#FFFFFF" cellSpacing="1" cellPadding=3 align="center">
				<tr bgcolor="#D0D0D0">
					<td>
						<input type="button" value="Ok" onclick="obrAtribuiMunicipio();" style="cursor: pointer;"/>
						<input type="button" value="Fechar" onclick="self.close();" style="cursor: pointer;"/>
					</td>
				</tr>
			</table>
		</form>
	</body>
</html>
