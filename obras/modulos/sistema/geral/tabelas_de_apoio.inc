<?php
$arrTbl = array("tipoorigemobra","modalidadeensino","tipoprojeto");
if(in_array($_REQUEST['tabelas'],$arrTbl) || $_POST['combo_sch'] || $_POST['combo_tbl'] || $_POST['inp_hdn_acao']){
		$titulo = "Monitoramento de Obras/Infraestrutura";
		$html = '<tr>
					<td class="SubTituloDireita">Selecione a tabela:</td>
					<td>
						<select class="CampoEstilo" id="tabelas" name="tabelas"	onChange="submeteTabelasApoio(this.value);">
							<option '.($_REQUEST['tabelas'] == "" ? " selected='selected' " : "").' value="">-- Selecione uma Tabela --</option>
							<option '.($_REQUEST['tabelas'] == "itenscomposicao" ? " selected='selected' " : "").' value="itenscomposicao">Etapas da Obra</option>
							<option '.($_REQUEST['tabelas'] == "tiporestricao" ? " selected='selected' " : "").' value="tiporestricao">Tipo de Restrição</option>
							<option '.($_REQUEST['tabelas'] == "tiporespcontato" ? " selected='selected' " : "").' value="tiporespcontato">Tipo de Responsável</option>
							<option '.($_REQUEST['tabelas'] == "tipoobra" ? " selected='selected' " : "").' value="tipoobra">Tipo de Obra</option>
							<option '.($_REQUEST['tabelas'] == "unidademedida" ? " selected='selected' " : "").' value="unidademedida">Unidade de Medida</option>
							<option '.($_REQUEST['tabelas'] == "tipoarquivo" ? " selected='selected' " : "").' value="tipoarquivo">Tipo de Arquivo</option>
							<option '.($_REQUEST['tabelas'] == "itensdetalhamento" ? " selected='selected' " : "").' value="itensdetalhamento">Módulo de Ampliação</option>
							<option '.($_REQUEST['tabelas'] == "situacaoobra" ? " selected='selected' " : "").' value="situacaoobra">Situação da Obra</option>
							<option '.($_REQUEST['tabelas'] == "desempenhoconstrutora" ? " selected='selected' " : "").' value="desempenhoconstrutora">Desempenho da Construtora</option>
							<option '.($_REQUEST['tabelas'] == "qualidadeobra" ? " selected='selected' " : "").' value="qualidadeobra">Qualidade da Obra</option>
							<option '.($_REQUEST['tabelas'] == "programafonte" ? " selected='selected' " : "").' value="programafonte">Programa Fonte</option>
							<option '.($_REQUEST['tabelas'] == "tipologiaobra" ? " selected='selected' " : "").' value="tipologiaobra">Tipologia da Obra</option>
							<option '.($_REQUEST['tabelas'] == "programatipologia" ? " selected='selected' " : "").' value="programatipologia">Programa / Tipologia</option>
							<option '.($_REQUEST['tabelas'] == "precocombustivel" ? " selected='selected' " : "").' value="precocombustivel">Tabela de Preço do Combustível</option>
							<option '.($_REQUEST['tabelas'] == "tipoorigemobra" ? " selected='selected' " : "").'value="tipoorigemobra">Origem</option>
							<option '.($_REQUEST['tabelas'] == "modalidadeensino" ? " selected='selected' " : "").'value="modalidadeensino">Modalidade de Ensino</option>
							<option '.($_REQUEST['tabelas'] == "tipoprojeto" ? " selected='selected' " : "").'value="tipoprojeto">Tipo de Projeto</option>
						</select>
						<script>function submeteTabelasApoio(tbl){ if(tbl){window.location.href=window.location+\'&tabelas=\' + tbl} }</script>
					<input type="hidden" id="descricao_tabela" name="descricao_tabela"></td>
				</tr>';
		$schema = $_REQUEST['inp_hdn_schema'] ? $_REQUEST['inp_hdn_schema'] : "obras"; //informa o schema
		$table = $_REQUEST['inp_hdn_tabela'] ? $_REQUEST['inp_hdn_tabela'] : $_REQUEST["tabelas"]; //informa a tabela
				
		$arrPermission = array(
									"inserir", //Permite inserir registros na tabela
									"alterar", //Permite alterar registros da tabela
									"excluir", //Permite excluir registros da tabela
									);
		
		include APPRAIZ.'/seguranca/modulos/sistema/tabelasApoio.inc';
		exit;	
}

header('Content-type: text/html; charset="iso-8859-1"',true);
include APPRAIZ . 'includes/cabecalho.inc';
include 'funcoes_tabelas_de_apoio.inc';

echo "<br>";
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo = "Monitoramento de Obras/Infraestrutura";
monta_titulo( $titulo_modulo, 'Tabelas de Apoio' );

switch ($_REQUEST['tabelas']){
	case 'programatipologia':
		include("programaTipologia.inc");
		break;
	case 'precocombustivel':
		include("tabelaCombustivel.inc");
		break;
	default:
		$codigoTabela = "";
				//ver($_REQUEST);
		if(isset($_REQUEST["tabelas"]) || isset($_REQUEST["nomeTabelaDados"])) {
			$tabela = $_REQUEST["nomeTabelaDados"];
				//dump($_REQUEST["enviado"]).die;
			if(isset($_REQUEST["nomeTabelaDados"])) {
				$descricao = $_REQUEST["descricaoTabelaDados"];
				//primeiro a ser enviado para as funções de inserção
				$retornoTabelaID = $_REQUEST["retornoTabelaDadosID"];
				//primeiro a ser enviado para as funções de inserção
				$retornoTabelaDescricao = $_REQUEST["retornoTabelaDadosDescricao"];
				$retornoTabelaID = explode(';', $retornoTabelaID);
				$retornoTabelaDescricao = explode(';', $retornoTabelaDescricao);

				if($tabela == "itenscomposicao") {
					//segundo campo a ser enviado para as funções de inserção
					$retornoTabelaDescricaoServicos = $_REQUEST["retornoTabelaDadosDescricaoServicos"];
					$retornoTabelaDescricaoServicos = explode(';', $retornoTabelaDescricaoServicos);
					insereDadosItensComposicao($retornoTabelaID, $retornoTabelaDescricao, $retornoTabelaDescricaoServicos);
				}
				if($tabela == "tiporestricao")
				insereDadosTipoRestricao($retornoTabelaID, $retornoTabelaDescricao);
				if($tabela == "tiporespcontato")
				insereDadosTipoRespContato($retornoTabelaID, $retornoTabelaDescricao);
				if($tabela == "tipoobra")
				insereDadosTipoObra($retornoTabelaID, $retornoTabelaDescricao);
				if($tabela == "unidademedida")
				insereDadosUnidadeMedida($retornoTabelaID, $retornoTabelaDescricao);
				if($tabela == "tipoarquivo")
				insereDadosTipoArquivo($retornoTabelaID, $retornoTabelaDescricao);
				if($tabela == "itensdetalhamento")
				insereDadosItensDetalhamento($retornoTabelaID, $retornoTabelaDescricao);
				if($tabela == "situacaoobra")
				insereDadosSituacaoObra($retornoTabelaID, $retornoTabelaDescricao);
				if($tabela == "desempenhoconstrutora")
				insereDadosDesempenhoConstrutora($retornoTabelaID, $retornoTabelaDescricao);
				if($tabela == "qualidadeobra")
				insereDadosQualidadeObra($retornoTabelaID, $retornoTabelaDescricao);
				if($tabela == "programafonte"){
					//segundo campo a ser enviado para as funções de inserção
					$retornoTabelaDescricaoServicos = $_REQUEST["retornoTabelaDadosDescricaoServicos"];
					$retornoTabelaDescricaoServicos = explode(';', $retornoTabelaDescricaoServicos);
					insereDadosProgramaFonte($retornoTabelaID, $retornoTabelaDescricao, $retornoTabelaDescricaoServicos);
				}
				if($tabela == "tipologiaobra"){
					//segundo campo a ser enviado para as funções de inserção
					$retornoTabelaDescricaoServicos = $_REQUEST["retornoTabelaDadosDescricaoServicos"];
					$retornoTabelaDescricaoServicos = explode('|||', $retornoTabelaDescricaoServicos);
					//terceiro campo a ser enviado para as funções de inserção
					$retornoTabelaMedidaDescricao = $_REQUEST["retornoTabelaDadosMedidaServicos"];
					$retornoTabelaMedidaDescricao = explode('|||', $retornoTabelaMedidaDescricao);
					insereDadosTipologiaObra($retornoTabelaID, $retornoTabelaDescricao, $retornoTabelaDescricaoServicos, $retornoTabelaMedidaDescricao);
					die("<script> 
							alert('Operação realizada com sucesso!');
							location.href = '?modulo=sistema/geral/tabelas_de_apoio&acao=A&tabelas=tipologiaobra'; 
						</script>");
				}
				if($tabela == "programatipologia"){
					$dados = $_REQUEST["dados"];
					$dados = explode('|', $dados);
					insereDadosProgramaTipologia($dados);
				}
				echo("<script type=\"text/javascript\">
						alert('Operação realizada com sucesso!');	
						location.href = window.location;
					</script>");		
				$_REQUEST["enviado"] = '0';
			}
			else if(isset($_REQUEST["tabelas"])) {
				$tabela = $_REQUEST["tabelas"];
				$descricao = $_REQUEST["descricao_tabela"];
				echo("<script type=\"text/javascript\">
						//submeteTabelas();
						//location.href = window.location+'&descricaoTabelaDados=".$_REQUEST[$tabela]."';			
					</script>");		
			}
			$resultado = retornaSelectMontaTabela($tabela);
			$count = 0;
			while($dados = pg_fetch_array($resultado)) {

				$count++;
				if($count % 2)
				$cor = "#f4f4f4";
				else
				$cor = "#e0e0e0";
					
				if(podeExcluir($tabela, $dados['codigo']))
				$excluir = "excluiItem(this.parentNode.parentNode.rowIndex);";
				else
				$excluir = "alert('Este registro não pode ser excluído pois está sendo usado em outra tabela.');";

				if($tabela == "itenscomposicao") {
					$codigoTabela .= "<tr style=\"background-color:".$cor."\">
										<td align=\"center\">
											<img src=\"/imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"alteraItem(this.parentNode.parentNode.rowIndex);\" title=\"Altera o item\">
											<img src=\"/imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"".$excluir."\" title=\"Exclui o item\">
											<input type=\"hidden\" id=\"item\" name=\"item\" value=\"valoritem_".$dados['codigo']."\">
										</td>
										<td align=\"left\">".$dados['descricao']."</td>
										<td align=\"center\"><textarea cols=\"70\" rows=\"2\" name=\"descricaoServicos\" id=\"descricaoServicos\">".$dados['descservico']."</textarea></td>
									 </tr>";
				}
				else {
					if ($tabela == "programafonte"){
						$codigoTabela .= "<tr style=\"background-color:".$cor."\">
											<td align=\"center\">
												<img src=\"/imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"alteraItem(this.parentNode.parentNode.rowIndex);\" title=\"Altera o item\">
												<img src=\"/imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"".$excluir."\" title=\"Exclui o item\">
												<input type=\"hidden\" id=\"item\" name=\"item\" value=\"valoritem_".$dados['codigo']."\">
											</td>
											<td align=\"left\">".$dados['descricao']."</td>
											<td align=\"center\">
												<select class='CampoEstilo' id='descricaoServicos_".$dados['codigo']."' name='descricaoServicos'>
													<option value='1' ". ($dados['idorgao'] == 1 ? 'selected=selected' : '').">GECopa</option>
													<option value='2' ". ($dados['idorgao'] == 2 ? 'selected=selected' : '').">Penitenciárias</option>
													<option value='3' ". ($dados['idorgao'] == 3 ? 'selected=selected' : '').">Gestão Aeroportuária</option>	
												</select>
											</td>
										 </tr>";
					}else if ($tabela == "tipologiaobra"){
						
						if($dados['medida'] == '' ){
							$medida = $dados['medida'];
						}else{
							$medida = number_format($dados['medida'], 2, ',', '.');
						}

						$codigoTabela .= "<tr style=\"background-color:".$cor."\">
											<td align=\"center\">
												<img src=\"/imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"alteraItem(this.parentNode.parentNode.rowIndex);\" title=\"Altera o item\">
												<img src=\"/imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"".$excluir."\" title=\"Exclui o item\">
												<input type=\"hidden\" id=\"item\" name=\"item\" value=\"valoritem_".$dados['codigo']."\">
											</td>
											<td align=\"left\">".$dados['descricao']."</td>
											<td align=\"left\">
													" . campo_textarea('tpodetalhe', 'N', 'N', '', 80, 4, 1000, '', '', '', '', '', $dados['detalhe']) . "	
											</td>
											<td align=\"left\">" . $medida . "</td>
										</tr>";					
							
						/*
						 $sql = "SELECT
						 cloid as codigo,
						 clodsc as descricao
							FROM
							obras.classificacaoobra
							ORDER BY cloid
							";
							$hidden_class = "";
							$dados_class =  $db->carregar($sql);

							foreach ($dados_class as $dado_class) {
							$hidden_class .=  $dado_class['codigo'].";".$dado_class['descricao']."|";
							}
							echo("<input type='hidden' id='hidden_class' name='hidden_class' value='".$hidden_class."'>");

							$aux = "";

							foreach ( $dados_class as $dado_class)	{
							$aux .="<option value='".$dado_class['codigo']."' ". ($dados['idclasse'] == $dado_class['codigo'] ? 'selected=selected' : '').">".$dado_class['descricao']."</option>";
							}

							$codigoTabela .= "<tr style=\"background-color:".$cor."\">
							<td align=\"center\">
							<img src=\"/imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"alteraItem(this.parentNode.parentNode.rowIndex);\" title=\"Altera o item\">
							<img src=\"/imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"".$excluir."\" title=\"Exclui o item\">
							<input type=\"hidden\" id=\"item\" name=\"item\" value=\"valoritem_".$dados['codigo']."\">
							</td>
							<td align=\"left\">".$dados['descricao']."</td>
							<td align=\"center\">
							<select class='CampoEstilo' id='descricaoServicos_".$dados['codigo']."' name='descricaoServicos'>
							".$aux."
							</select>
							</td>
							</tr>";
							*/
					}else if ($tabela == "programatipologia"){
						// preparando e alimentando os campos hidden com os dados necessários para montar as combos de novo item em javascrip
						$sql_programa = "SELECT
								prfid as codigo,
								prfdesc as descricao 
							FROM 
								obras.programafonte
							ORDER BY prfid
							";	
						$hidden_programa = "";
						$dados_programa =  $db->carregar($sql_programa);

						$sql_tipologia = "SELECT
								tpoid as codigo,
								tpodsc as descricao 
							FROM 
								obras.tipologiaobra
							ORDER BY tpoid
							";	
						$hidden_tipologia = "";
						$dados_tipologia =  $db->carregar($sql_tipologia);
						foreach ($dados_programa as $dado_programa) {
							$hidden_programa .=  $dado_programa['codigo'].";".$dado_programa['descricao']."|";
						}

						foreach ($dados_tipologia as $dado_tipologia) {
							$hidden_tipologia .=  $dado_tipologia['codigo'].";".$dado_tipologia['descricao']."|";
						}

						echo("<input type='hidden' id='hidden_programa' name='hidden_programa' value='".$hidden_programa."'>");
						echo("<input type='hidden' id='hidden_tipologia' name='hidden_tipologia' value='".$hidden_tipologia."'>");
						// fim da preparação e alimentação dos js

						$select_programa = "";
						$select_tipologia = "";

						foreach ( $dados_programa as $dado_programa)	{
							$select_programa .="<option value='".$dado_programa['codigo']."' ". ($dados['codigo'] == $dado_programa['codigo'] ? 'selected=selected' : '').">".$dado_programa['descricao']."</option>";
						}
						foreach ( $dados_tipologia as $dado_tipologia)	{
							$select_tipologia .="<option value='".$dado_tipologia['codigo']."' ". ($dados['codigotipologia'] == $dado_tipologia['codigo'] ? 'selected=selected' : '').">".$dado_tipologia['descricao']."</option>";
						}

						$codigoTabela .= "<tr style=\"background-color:".$cor."\">
											<td align=\"center\">										
												<img src=\"/imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"".$excluir."\" title=\"Exclui o item\">
												<input type=\"hidden\" id=\"item\" name=\"item\" value=\"valoritem_".$dados['codigo']."\">
											</td>
											<td align=\"center\">
												<select class='CampoEstilo' id='programaItem_".$dados['ptpid']."' name='programaItem_".$dados['ptpid']."'>
													".$select_programa."
												</select>
											</td>
											<td align=\"center\">
												<select class='CampoEstilo' id='tipologiaItem_".$dados['ptpid']."' name='tipologiaItem_".$dados['ptpid']."'>
													".$select_tipologia."
												</select>
											</td>
										 </tr>";

					}else{
						$codigoTabela .= "<tr style=\"background-color:".$cor."\">
											<td align=\"center\">
												<img src=\"/imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"alteraItem(this.parentNode.parentNode.rowIndex);\" title=\"Altera o item\">
												<img src=\"/imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"".$excluir."\" title=\"Exclui o item\">
												<input type=\"hidden\" id=\"item\" name=\"item\" value=\"valoritem_".$dados['codigo']."\">
											</td>
											<td align=\"left\">".$dados['descricao']."</td>
										 </tr>";
					}
				}
			}
		}

		?>

<script type="text/javascript"><!--
			var ItemEmAlteracao = 0;
			var idItemEmAlteracao = '';
			var limite = 200;
			
			function submeteTabelasApoio() {
				var select = document.getElementById('tabelas');
						
				if(select.value != "") {
					document.getElementById('descricao_tabela').value = select.options[select.selectedIndex].innerHTML;
					document.getElementById('formTabelasApoio').submit();
				}
			}
		
			function submeteTabelas() {
				var select = document.getElementById('nomeTabelaDados');
						
				if(select.value != "") {
					document.getElementById('descricao_tabela').value = select.options[select.selectedIndex].innerHTML;
					document.getElementById('formTabelasApoio').submit();
				}
			}
			
			function insereItem() {
			   if(ItemEmAlteracao == 0) {
				   
				  var descricao = document.getElementById('descricaoNovoItem');		  
				  
				  if(descricao.value != "") {		
					var tabela = document.getElementById('tabelaDados');
					
					var linha = tabela.insertRow(2);
					
					if(tabela.rows[3].style.backgroundColor == "rgb(224, 224, 224)") {
						linha.style.backgroundColor = "#f4f4f4";					
					} else {
						linha.style.backgroundColor = "#e0e0e0";					
					}
					
					var colAcao = linha.insertCell(0);
					var colDescricao = linha.insertCell(1);
					
					colAcao.style.textAlign = "center";
					colDescricao.style.textAlign = "left";
									
					colAcao.innerHTML = "<img src='/imagens/alterar.gif' style='cursor:pointer;' onclick='alteraItem(this.parentNode.parentNode.rowIndex);' title='Altera o item'>&nbsp;<img src='/imagens/excluir.gif' style='cursor:pointer;' onclick='excluiItem(this.parentNode.parentNode.rowIndex);' title='Exclui o item'><input type='hidden' id='item' name='item' value='valoritem_xx'>";								
					colDescricao.innerHTML = descricao.value;
					
					<? if($tabela == "itenscomposicao") { ?>
					var colDescricaoServicos = linha.insertCell(2);
					colDescricaoServicos.style.textAlign = "center";
					colDescricaoServicos.innerHTML = "<textarea class='txareanormal' cols='70' rows='2' name='descricaoServicos' id='descricaoServicos'></textarea>";
					<? } ?>
					
					<? if($tabela == "programafonte") {?>
		
					var orgaoNovoItem = document.getElementById('orgaoNovoItem');			
					var colDescricaoServicos = linha.insertCell(2);
					colDescricaoServicos.style.textAlign = "center";		
					colDescricaoServicos.innerHTML =   '<select class="CampoEstilo" id="descricaoServicos" name="descricaoServicos">' 
													 + '<option value="1" '+( orgaoNovoItem.value == 1 ? 'selected="selected"' : '')+' >GECopa</option>'
													 + '<option value="2" '+( orgaoNovoItem.value == 2 ? 'selected="selected"' : '')+' >Penitenciárias</option>'
													 + '<option value="3" '+( orgaoNovoItem.value == 3 ? 'selected="selected"' : '')+' >Gestão Aeroportuária</option>'
													 + '</select>';
					<? } ?>
		
					<? if($tabela == "tipologiaobra") {?>
					
					var orgaoNovoItem = '';
					var colDescricaoServicos = linha.insertCell(2);
					var colMedidaServicos = linha.insertCell(3);
					var detalhe = document.getElementById('detalheNovoItem');
					var medida = document.getElementById('descricaoNovaMedida');
					var detalheContador = document.getElementsByName('no_detalheNovoItem')[0];	
					var text = '';
					
					text = '<textarea ' + 
						   '	style="border-style: none none none solid; border-color: -moz-use-text-color -moz-use-text-color -moz-use-text-color rgb(136, 136, 136); border-width: medium medium medium 3px; color: rgb(64, 64, 64); width: 80ex;" ' + 
						   '	disabled="disabled"' +  
						   ' 	readonly="readonly"' +
						   '	rows="4" cols="80" name="tpodetalhe" id="tpodetalhe">' + detalhe.value + '</textarea>';
						   
					colDescricaoServicos.innerHTML = text;	
					colMedidaServicos.innerHTML = medida.value;	
					detalhe.value = '';
					medida.value = '';
					detalheContador.value = 1000;   
					
		/*
					var orgaoNovoItem = document.getElementById('classeNovoItem');			
					var colDescricaoServicos = linha.insertCell(2);
					var registro = document.getElementById('hidden_class').value;	
					var select = '';		
					registro = registro.split('|');
		
					colDescricaoServicos.style.textAlign = "center";	
					select =   '<select class="CampoEstilo" id="descricaoServicos" name="descricaoServicos">';
						
					for ( i = 0; i < (registro.length - 1); i++){
		
						var dado = registro[i].split(';');				
						
						select = select + '<option value="'+dado[0]+'" '+( orgaoNovoItem.value == dado[0] ? 'selected="selected"' : '')+' >'+dado[1]+'</option>';
					}
		
					select = select +  '</select>';
					
					colDescricaoServicos.innerHTML = select;
		*/		
					<? } ?>
								
					descricao.value = "";
					orgaoNovoItem.value = 1;
				  }
				  else {
				  	alert("A 'Descrição' deve ser informada.");
				  }
			   }
			   else {
			   	 alert("Você deve concluir a alteração do item.");
			   	 document.getElementById(idItemEmAlteracao).focus();
			   }
			}
		
			function insereItemProgramaTipologia(){		 	
					 	
					var tabela = document.getElementById('tabelaDados');
					
					var linha = tabela.insertRow(2);
					
					if(tabela.rows[3].style.backgroundColor == "rgb(224, 224, 224)") {
						linha.style.backgroundColor = "#f4f4f4";					
					} else {
						linha.style.backgroundColor = "#e0e0e0";					
					}
					
					var colAcao = linha.insertCell(0);
								
					colAcao.style.textAlign = "center";										
					colAcao.innerHTML = "<img src='/imagens/excluir.gif' style='cursor:pointer;' onclick='excluiItem(this.parentNode.parentNode.rowIndex);' title='Exclui o item'><input type='hidden' id='item' name='item' value='valoritem_xx'>";								
										
					var colPrograma = linha.insertCell(1);
					var colTipologia = linha.insertCell(2);
		
					colPrograma.style.textAlign = "center";
					colTipologia.style.textAlign = "center";				
					
					var programaNovoItem = document.getElementById('programaNovoItem');
					var tipologiaNovoItem = document.getElementById('tipologiaNovoItem');				
					
					var registroPrograma = document.getElementById('hidden_programa').value;
					var registroTipologia = document.getElementById('hidden_tipologia').value;				
						
					var selectPrograma = '';
					var selectTipologia = '';	
					registroPrograma = registroPrograma.split('|');
					registroTipologia = registroTipologia.split('|');			
					
					selectPrograma =   '<select class="CampoEstilo" id="programaNovoItem" name="programaNovoItem">';
					for ( i = 0; i < (registroPrograma.length - 1); i++){
		
						var dado = registroPrograma[i].split(';');	
						
						selectPrograma = selectPrograma + '<option value="'+dado[0]+'" '+( programaNovoItem.value == dado[0] ? 'selected="selected"' : '')+' >'+dado[1]+'</option>';
					}
		
					selectPrograma = selectPrograma +  '</select>';
		
					selectTipologia =  '<select class="CampoEstilo" id="tipologiaNovoItem" name="tipologiaNovoItem">';
					for ( i = 0; i < (registroTipologia.length - 1); i++){
		
						var dado = registroTipologia[i].split(';');				
						
						selectTipologia = selectTipologia + '<option value="'+dado[0]+'" '+( tipologiaNovoItem.value == dado[0] ? 'selected="selected"' : '')+' >'+dado[1]+'</option>';
					}
		
					selectTipologia = selectTipologia +  '</select>';
					
					colPrograma.innerHTML = selectPrograma;
					colTipologia .innerHTML = selectTipologia;
										
					programaNovoItem.value = 1;
					tipologiaNovoItem.value = 1;
					
			}
			
			function alteraItem(linha) {
			  if(ItemEmAlteracao == 0) {
				var tabela = document.getElementById("tabelaDados");		
				var cels = tabela.rows[linha].cells;

				var descItem = cels[1].innerHTML;
				cels[1].innerHTML = "&nbsp;&nbsp;&nbsp;<input class='normal' type='text' id='ok_" + linha + "' size='60' maxlength='256' value='" + descItem + "'>&nbsp;<input type='button' value='OK' onclick='concluiAlteracaoItem(" + linha + ");'>";
				
				idItemEmAlteracao = 'ok_' + linha;
				
				<? if($tabela == "tipologiaobra") {?>
				var valTextArea = cels[2].getElementsByTagName('textarea')[0].value;
				
				text = '<textarea ' + 
			   '	onkeyup="textCounter( this.form.ed_tpodetalhe, this.form.no_ed_tpodetalhe, 1000);"' + 
			   '	onkeydown="textCounter( this.form.ed_tpodetalhe, this.form.no_ed_tpodetalhe, 1000 );"' +  
			   ' 	style="width: 80ex;"' +
			   '    onblur="MouseBlur( this ); textCounter( this.form.ed_tpodetalhe, this.form.no_ed_tpodetalhe, 1000);"' + 
			   '	onmouseout="MouseOut( this );" onfocus="MouseClick( this );"' +
			   '	onmouseover="MouseOver( this );" rows="4" cols="80" name="ed_tpodetalhe" id="tpodetalhe" class="txareanormal">' + valTextArea + '</textarea>' +
			   '	<br/>' +
			   '	<input type="text" class="CampoEstilo" value="' + (1000 - valTextArea.length) + '" maxlength="6" size="6" name="no_ed_tpodetalhe" style="border-left: 3px solid rgb(136, 136, 136); text-align: right; color: rgb(128, 128, 128);" readonly=""/>' +
			   '	<font face="Verdana" color="red" size="1"> máximo de caracteres</font>';
				
				cels[2].innerHTML = text;
 
				var descMedida = cels[3].innerHTML;
				cels[3].innerHTML = "&nbsp;&nbsp;&nbsp;<input class='normal' maxlength='9' type='text' id='" + linha + "' size='9' value='" + descMedida + "' onkeypress='return somenteNumero(this)'>";
				<? } ?>
				
				ItemEmAlteracao++;
			  }
			  else {
			  	alert("Você deve concluir a alteração do item.");
			  	document.getElementById(idItemEmAlteracao).focus();
			  }
			}
			
			function concluiAlteracaoItem(linha) {
				
				var tabela = document.getElementById("tabelaDados");		
				var cels = tabela.rows[linha].cells;
				var valor = cels[1].getElementsByTagName("input")[0].value;
				//var valor2 = cels[3].getElementsByTagName("input")[0].value;
				var valor2 = linha;
				
				if(valor == "") {
					alert("A 'Descrição' deve ser informada.");
					document.getElementById("ok_"+linha).focus();
				}
				else {
					cels[1].innerHTML = valor;
					ItemEmAlteracao--;
					idItemEmAlteracao = '';
					
				<? if($tabela == "tipologiaobra") {?>
				var valTextArea = cels[2].getElementsByTagName('textarea')[0].value;
				
				text = '<textarea ' + 
					   '	style="border-style: none none none solid; border-color: -moz-use-text-color -moz-use-text-color -moz-use-text-color rgb(136, 136, 136); border-width: medium medium medium 3px; color: rgb(64, 64, 64); width: 80ex;" ' + 
					   '	disabled="disabled"' +  
			   		   ' 	readonly="readonly"' +
			   		   '	rows="4" cols="80" name="tpodetalhe" id="tpodetalhe">' + valTextArea + '</textarea>';
		   		
				cels[2].innerHTML = text;
				cels[3].innerHTML = valor2;
					
				<? } ?>			
		
				}
			}
			
			function excluiItem(linha) {		
				if(ItemEmAlteracao == 0) {
					var tabela = document.getElementById("tabelaDados");
					tabela.deleteRow(linha);
				}
				else {
					alert("Você deve concluir a alteração do item.");
			  		document.getElementById(idItemEmAlteracao).focus();
				}
			}
			
			function submeteTabelaDados() {
				if(ItemEmAlteracao == 0) {
					var tabela = document.getElementById("tabelaDados");
					var medida = document.getElementById("descricaoNovaMedida");
					var cels, sub, valor, retornoID = '', retornoDesc = '', retornoDescServicos = '', retornoMedidaServicos ='';
					
					for(var i=2; i < ((tabela.rows.length) - 1); i++) {
						valor = '';
						cels = tabela.rows[i].cells;
						
						sub = cels[0].innerHTML;				
						sub = sub.substr(sub.search(/valoritem_/), 13);
						//itens novos
						if((sub.charAt(10) == 'x') && (sub.charAt(11) == 'x')) {
							valor = 'xx';
						}
						else {
							for(var z=0; z < sub.length; z++) {
								if(!isNaN(sub.charAt(z)))
									valor = valor + sub.charAt(z);
							}
						}				
						if(i == 2) {
							retornoID = valor;
							
							//primeiro campo a ser enviado para as funções de inserção
							retornoDesc = cels[1].innerHTML;
							
							<? if($tabela == "itenscomposicao") { ?>
							//segundo campo a ser enviado para as funções de inserção
							retornoDescServicos = cels[2].getElementsByTagName("textarea")[0].value;
							<? } ?>
							
							<? if($tabela == "tipologiaobra") { ?>
							//segundo campo a ser enviado para as funções de inserção
							retornoDescServicos = cels[2].getElementsByTagName("textarea")[0].value;
							//terceiro campo a ser enviado para as funções de inserção
							retornoMedidaServicos = cels[3].innerHTML;
							
							<? } ?>
							
							<? if($tabela == "programafonte" /*|| $tabela == "tipologiaobra"*/) { ?>
							//segundo campo a ser enviado para as funções de inserção
							retornoDescServicos = cels[2].getElementsByTagName("select")[0].value;
							<? } ?>
						}
						else {
							retornoID = retornoID + ";" + valor;
							//primeiro campo a ser enviado para as funções de inserção
							retornoDesc = retornoDesc + ";" + cels[1].innerHTML;
							<? if($tabela == "itenscomposicao") { ?>
							//segundo campo a ser enviado para as funções de inserção
							retornoDescServicos = retornoDescServicos + ";" + cels[2].getElementsByTagName("textarea")[0].value;
							<? } ?>
							
							<? if($tabela == "tipologiaobra") { ?>
							//segundo campo a ser enviado para as funções de inserção
							retornoDescServicos = retornoDescServicos + "|||" + cels[2].getElementsByTagName("textarea")[0].value;
							//terceiro campo a ser enviado para as funções de inserção
							retornoMedidaServicos = retornoMedidaServicos + "|||" + cels[3].innerHTML;
							<? } ?>
							
							<? if($tabela == "programafonte" /*|| $tabela == "tipologiaobra"*/) { ?>
							//segundo campo a ser enviado para as funções de inserção
							retornoDescServicos = retornoDescServicos + ";" + cels[2].getElementsByTagName("select")[0].value;
							<? } ?>
						}
					}			
					document.getElementById('retornoTabelaDadosID').value = retornoID;
					document.getElementById('retornoTabelaDadosDescricao').value = retornoDesc;
					<? if($tabela == "itenscomposicao" || $tabela == "programafonte" || $tabela == "tipologiaobra") { ?>
					document.getElementById('retornoTabelaDadosDescricaoServicos').value = retornoDescServicos;
					document.getElementById('retornoTabelaDadosMedidaServicos').value = retornoMedidaServicos;
					
					
					<? } ?>
					//document.formTabelasApoio.enviado.value = '1';
					//document.formTabelasApoioDados.enviado.value = '1';
					document.getElementById('formTabelasApoioDados').submit();
				}
				else {
					alert("Você deve concluir a alteração do item.");
			  		document.getElementById(idItemEmAlteracao).focus();
				}
			}
			
			function submeteObraTipologia(){
				var tabela = document.getElementById("tabelaDados");
				var cels, sub, valor, dados = '';
			
			}
			
			
			function submeteProgramaTipologia() {
				
				var tabela = document.getElementById("tabelaDados");
				var cels, sub, valor, dados = '';
				
				for(var i=2; i < ((tabela.rows.length) - 1); i++) {
					valor = '';
					cels = tabela.rows[i].cells;
					var nome = cels[1].getElementsByTagName("select")[0].name;		
					//itens novos			
					if(nome == 'programaNovoItem') {
						valor = 'n';
					}
					else {
						nome = nome.substr(13);
						valor = nome;
					}					
					dados = dados + valor;
					dados = dados + ";" + cels[1].getElementsByTagName("select")[0].value;
					dados = dados + ";" + cels[2].getElementsByTagName("select")[0].value + "|";		
					
				}		
				document.getElementById('dados').value = dados;	
				//document.formTabelasApoio.enviado.value = '1';
				//document.formTabelasApoioDados.enviado.value = '1';
				document.getElementById('formTabelasApoioDados').submit();		
			}
			
			//Valida campo Área (m²) para inserir somente números
					
			function somenteNumero(sn){
				 
				v = sn.value; 
				v=v.replace(/\D/g,""); // permite digitar apenas numero 
				v=v.replace(/(\d{1})(\d{14})$/,"$1.$2") ;// coloca ponto antes dos ultimos digitos 
				v=v.replace(/(\d{1})(\d{11})$/,"$1.$2"); // coloca ponto antes dos ultimos 11 digitos 
				v=v.replace(/(\d{1})(\d{8})$/,"$1.$2"); // coloca ponto antes dos ultimos 8 digitos 
				v=v.replace(/(\d{1})(\d{5})$/,"$1.$2"); // coloca ponto antes dos ultimos 5 digitos 
				v=v.replace(/(\d{1})(\d{1,2})$/,"$1,$2"); // coloca virgula antes dos ultimos 2 digitos 
				sn.value = v; 

			}

			
		--></script>
<form method="post" name="formTabelasApoio" id="formTabelasApoio">
	<input type="hidden" id="enviado" name="enviado" value="0">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita">Selecione a tabela:</td>
		<td><select class="CampoEstilo" id="tabelas" name="tabelas"	onChange="submeteTabelasApoio();">
			<option value="">-- Selecione uma Tabela --</option>
			<option value="itenscomposicao">Etapas da Obra</option>
			<option value="tiporestricao">Tipo de Restrição</option>
			<option value="tiporespcontato">Tipo de Responsável</option>
			<option value="tipoobra">Tipo de Obra</option>
			<option value="unidademedida">Unidade de Medida</option>
			<option value="tipoarquivo">Tipo de Arquivo</option>
			<option value="itensdetalhamento">Módulo de Ampliação</option>
			<option value="situacaoobra">Situação da Obra</option>
			<option value="desempenhoconstrutora">Desempenho da Construtora</option>
			<option value="qualidadeobra">Qualidade da Obra</option>
			<option value="programafonte">Programa Fonte</option>
			<option value="tipologiaobra">Tipologia da Obra</option>
			<option value="programatipologia">Programa / Tipologia</option>
			<option value="precocombustivel">Tabela de Preço do Combustível</option>
			<option value="tipoorigemobra">Origem</option>
			<option value="modalidadeensino">Modalidade de Ensino</option>
			<option value="tipoprojeto">Tipo de Projeto</option>
		</select> <input type="hidden" id="descricao_tabela" name="descricao_tabela"></td>
	</tr>
</table>
</form>

					<?php if(isset($_REQUEST["tabelas"]) || isset($_REQUEST["nomeTabelaDados"])) { ?>
<form method="post" name="formTabelasApoioDados" id="formTabelasApoioDados">
	<input type="hidden" value="<?= $tabela ?>" id="nomeTabelaDados" name="nomeTabelaDados"> 
	<input type="hidden" value="<?= $descricao ?>" id="descricaoTabelaDados" name="descricaoTabelaDados"> 
	<input type="hidden" id="retornoTabelaDadosDescricao" name="retornoTabelaDadosDescricao"> 
	<input type="hidden" id="retornoTabelaDadosID" name="retornoTabelaDadosID"> 
	<input type="hidden" id="dados" name="dados"> 
	<input type="hidden" id="enviado" name="enviado" value="0"> 
	<? if($tabela == "itenscomposicao" || $tabela == "programafonte" || $tabela == "tipologiaobra" || $tabela == "programatipologia" ) { ?>
			<? if($tabela == "tipologiaobra"){$descricao = 'Tipologia da Obra';	}?>
	<input type="hidden" id="retornoTabelaDadosDescricaoServicos" name="retornoTabelaDadosDescricaoServicos"> 
	<input type="hidden"id="retornoTabelaDadosMedidaServicos" name="retornoTabelaDadosMedidaServicos"> 
	
	<? } ?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<tr>
		<td><?= $descricao ?>  </td>
	</tr>
	<tr>
		<td>
		<table id="tabelaDados" width="90%" align="center" border="0"
			cellspacing="2" cellpadding="2" class="listagem">
			<thead>
			<? if($tabela == "programafonte") { ?>
				<tr>
					<td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
					<td width="45%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Descrição</strong></td>
					<td width="45%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Eixo Temático</strong></td>
				</tr>
				<tr>
					<td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						<img src="/imagens/gif_inclui.gif" style="cursor: pointer;" onclick="insereItem();" title="Inclui um novo item">
					</td>
					<td width="45%" valign="top" align="left" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						&nbsp;&nbsp;&nbsp;<input type="text" size="60" id="descricaoNovoItem" class="normal">
					</td>
					<td width="45%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
					<select class='CampoEstilo' id='orgaoNovoItem' name='orgaoNovoItem'>
						<option value='1'>GECopa</option>
						<option value='2'>Penitenciárias</option>
						<option value='3'>Gestão Aeroportuária</option>
					</select></td>
				</tr>
				<? }else if($tabela == "tipologiaobra") { ?>

				<tr>
					<td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
					<td width="45%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Nome</strong></td>
					<td width="45%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Descrição</strong></td>
					<td width="45%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Área (m²)</strong></td>
				</tr>
				<tr>
					<td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						<img src="/imagens/gif_inclui.gif" style="cursor: pointer;" onclick="insereItem();" title="Inclui um novo item">
					</td>
					<td width="45%" valign="top" align="left" class="title"	style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						&nbsp;&nbsp;&nbsp;<input type="text" size="60" maxlength='256' id="descricaoNovoItem" class="normal">
					</td>
					<td width="45%" valign="top" align="left" class="title"style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						<?=campo_textarea('detalheNovoItem', 'N', 'S', '', 80, 4, 1000) ?>
					</td>
					<td width="45%" valign="top" align="left" class="title"	style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						<?= campo_texto( 'descricaoNovaMedida', 'N', 'S', '', 10, 9, '[###.]###,##', '','','','','id="descricaoNovaMedida"','','','this.value=mascaraglobal(\'[###.]###,##\',this.value);'  ); ?>
						<!--<input type="text" size="9" maxlength="5" id="descricaoNovaMedida" class="normal" onkeypress='return SomenteNumero(event)'>-->
					</td>
				</tr>

				<!--
									<tr>
										<td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
										<td width="45%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Descrição</strong></td>
										<td width="45%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Classe</strong></td>
									</tr>
									<tr>
										<td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><img src="/imagens/gif_inclui.gif" style="cursor:pointer;" onclick="insereItem();" title="Inclui um novo item"></td>
										<td width="45%" valign="top" align="left" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">&nbsp;&nbsp;&nbsp;<input type="text" size="60" id="descricaoNovoItem" class="normal"></td>
										<td width="45%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">								
											<select class='CampoEstilo' id='classeNovoItem' name='classeNovoItem'>
												<?php 
									/*			$sql = "SELECT 
															cloid as codigo,
															clodsc as descricao 
														FROM 
															obras.classificacaoobra
															";											
												$dados_class =  $db->carregar($sql);
												foreach ( $dados_class as $dado_class)	{					
													echo("<option value='".$dado_class['codigo']."' ". ($dados['idclasse'] == $dado_class['codigo'] ? 'selected=selected' : '').">".$dado_class['descricao']."</option>");					
												}
										*/			
												?>	
											</select>
										</td>
									</tr>
							-->
							<? }else if($tabela == "programatipologia") { ?>
				<tr>
					<td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';"onmouseout="this.bgColor='';"><strong>Ação</strong></td>
					<td width="45%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Programa</strong></td>
					<td width="45%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Tipologia</strong></td>
				</tr>
				<tr>
					<td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						<img src="/imagens/gif_inclui.gif" style="cursor: pointer;"	onclick="insereItemProgramaTipologia();" title="Inclui um novo item">
					</td>
					<td width="45%" valign="top" align="center" class="title"style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						<select class='CampoEstilo' id='programaNovoItem' name='programaNovoItem'>
						<?php
						$sql = "SELECT
															prfid as codigo,
															prfdesc as descricao 
														FROM 
															obras.programafonte
														ORDER BY prfid	
															";											
						$dados_programa =  $db->carregar($sql);
						foreach ( $dados_programa as $dado_programa)	{
							echo("<option value='".$dado_programa['codigo']."' ". ($dados['idclasse'] == $dado_programa['codigo'] ? 'selected=selected' : '').">".$dado_programa['descricao']."</option>");
						}
						?>
						</select></td>

					<td width="45%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						<select class='CampoEstilo' id='tipologiaNovoItem' name='tipologiaNovoItem'>
						<?php
						$sql = "SELECT
															tpoid as codigo,
															tpodsc as descricao 
														FROM 
															obras.tipologiaobra
														ORDER BY tpoid
															";											
						$dados_tipologiaobra =  $db->carregar($sql);
						foreach ( $dados_tipologiaobra as $dado_tipologiaobra)	{
							echo("<option value='".$dado_tipologiaobra['codigo']."' ". ($dados['idclasse'] == $dado_tipologiaobra['codigo'] ? 'selected=selected' : '').">".$dado_tipologiaobra['descricao']."</option>");
						}
						?>
						</select>
					</td>
				</tr>
				<? } else if($tabela == "itenscomposicao") { ?>
				<tr>
					<td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
					<td width="45%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Descrição</strong></td>
					<td width="45%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Descrição dos Serviços</strong></td>
				</tr>
				<tr>
					<td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						<img src="/imagens/gif_inclui.gif" style="cursor: pointer;" onclick="insereItem();" title="Inclui um novo item">
					</td>
					<td colspan="2" width="90%" valign="top" align="left" class="title"	style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						&nbsp;&nbsp;&nbsp;<input type="text" size="60" id="descricaoNovoItem" class="normal">
					</td>
				</tr>
				<? } else { ?>
				<tr>
					<td width="5%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
					<td width="95%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Descrição</strong></td>
				</tr>
				<tr>
					<td width="5%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						<img src="/imagens/gif_inclui.gif" style="cursor: pointer;" onclick="insereItem();" title="Inclui um novo item">
					</td>
					<td width="95%" valign="top" align="left" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
						&nbsp;&nbsp;&nbsp;<input type="text" size="60" id="descricaoNovoItem" class="normal">
					</td>
				</tr>
				<? } ?>
			</thead>
			<?= $codigoTabela ?>
			<tr bgcolor="#C0C0C0">
				<td></td>
				<td colspan="2">
					<div style="float: left;">
						<?php
						$submeter = "";
						if($tabela == "programatipologia"){
							$submeter = "onclick='submeteProgramaTipologia();'";
							//								}elseif ($tabela == "tipologiaobra"){
							//									$submeter = "onclick='submeteObraTipologia();'";
						}else{
							$submeter = "onclick='submeteTabelaDados();'";
						}
						?> 
						<input type="button" value="Salvar" style="cursor: pointer"<?=$submeter; ?>">
						<input type="button" value="Voltar" style="cursor: pointer"	onclick="history.back(-1);" >
					</div>
				</td>
				<td></td>
			</tr>
		</table>
		</td>
	</tr>
</table>
</form>
				<?
					}
}
?>
