<?php

if ( $_GET['AJAX'] ) {
	switch ( $_GET['subAcao'] ){
		case 'ajaxCarregaTipologia' : carregaTipologia( $_GET['cloid'], $_GET['prfid'] ); break;
		case 'ajaxCarregaTipologia_Prog' : carregaTipologiaProg( $_GET['prfid'] ); break;
		case 'ajaxCarregaTipologia_Class' : carregaTipologiaClass( $_GET['cloid'] ); break;
	}
	die;
}

// Inclusão do arquivo de permissões (somente no módulo de obras)
if ($_SESSION["sisid"] == ID_OBRAS){
	require_once APPRAIZ . 'includes/cabecalho.inc';
	require_once APPRAIZ . "www/obras/permissoes.php";
}

// Inclusão de arquivos padrão do sistema
require_once APPRAIZ . 'includes/Agrupador.php';

// Inclusão de arquivos do componente de Entidade 
require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

// Pega o caminho atual do usuário (em qual módulo se encontra)
$caminho_atual = $_SERVER["REQUEST_URI"];
$posicao_caminho = strpos($caminho_atual, 'acao');
$caminho_atual = substr($caminho_atual, 0 , $posicao_caminho);

// Pega url para os js
$posicao_caminho_js = strpos($caminho_atual, '?');
$caminho_atual_js = substr($caminho_atual, 0 , $posicao_caminho_js);

$obras = new Obras();

// Executa as funções da tela de acordo com suas ações
switch ($_REQUEST["requisicao"]){
	case "cadastro": $obras->CadastrarObras( $_REQUEST ); break;
	case "atualiza": $obras->AtualizarObras( $_REQUEST ); break;
}

$dobras = new DadosObra(null);
$requisicao = "cadastro";

echo "<br/>";
if ( $_REQUEST['subAcao'] == 'novaObra' ) {
	unset( $_SESSION['obra'] );
	unset( $_REQUEST['obrid'] );
}
if( $_REQUEST["obrid"] ){
	
	// Verifica se existe a obra e se o usuário possui permissão
	include_once APPRAIZ . "www/obras/_permissoes_obras.php";
	
	// Cria a sessão com a nova obra
	session_unregister("obra");
	$_SESSION["obra"]["obrid"] = $_REQUEST["obrid"];
	
}else {
	$tr_campus = 'display:none;';
}

if ($_SESSION["obra"]["obrid"]){
	
	$tr_campus = '';
	$requisicao = "atualiza";
	
	// Carrega os dados da obra 
	$obrid = $_SESSION["obra"]["obrid"];
	$dados = $obras->Dados($obrid);
	$dobras = new DadosObra($dados);
	
	// For uma obra do FNDE não possui campus
	if ($dobras->orgid == ORGAO_FNDE){
		$tr_campus = 'display:none;';
	}
	
	// Monta as abas
	$db->cria_aba($abacod_tela,$url,$parametros);
	
}


// Cria o título da tela
$titulo_modulo = "Extrato da Obra";
monta_titulo( $titulo_modulo, "");

?>
	<script type="text/javascript">	
		function selecionar_fotos(origem){
			if (origem=="galeria"){
				selecionado="selecionado="+origem;
				document.getElementById("num_fotos_galeria").value="0";
				selecionado+="&marcadas="+document.getElementById("selecao_fotos_galeria").value;
			}
			
			// quebrando a strig para definir a vistoria
			id_vistoria= origem.substr(9,origem.length);
			origem = origem.substr(0,8);
			
			if (origem=="vistoria"){
				selecionado="selecionado="+origem+"_"+id_vistoria;
				document.getElementById("num_fotos_vistoria_"+id_vistoria).value="0";
				selecionado+="&marcadas="+document.getElementById("selecao_fotos_vistoria_"+id_vistoria).value;
			}

			var janela = window.open('?modulo=principal/extrato_selecionar_fotos&acao=A&'+selecionado, 'Fotos', 'width=380,height=380,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			janela.focus();
		}

		function mostrar(elemento_tela){
			
			try{
				valor = elemento_tela.id;
				valor = valor.substr(4,valor.length);
				valor=document.getElementById(valor).checked;
				if(valor)
					elemento_tela.style.display = "block";
				else
					elemento_tela.style.display = "none";
				} catch (e) {}
		}
		
		function selecionaTodos()
		{
			marcar_true=document.getElementById("selecionar_todos");

			    var objForm = document.getElementById("form_extrato"); 
			    for (var i=0; i < objForm.length; i++)
			    {
			        if(objForm.elements[i].type == "checkbox")
			        {
			            var check = objForm.elements[i];
			            if (marcar_true.checked == true){
			            	check.checked = true;
			            	// Mostrando
			            	mostrar(div_vistoria);
			            	mostrar(div_galeria);
			            	mostrar(div_coordenadas);
			            	var id_vistoria="div_idvistoria_"+check.id.substr(11,check.id.length);
			            	mostrar(document.getElementById(id_vistoria));
			            	
			         }else{
			            	check.checked = false;
			            	// Escondendo
			            	mostrar(div_vistoria);
			            	mostrar(div_galeria);
			            	mostrar(div_coordenadas);
			            	var id_vistoria="div_idvistoria_"+check.id.substr(11,check.id.length);
			            	mostrar(document.getElementById(id_vistoria));
			         }
			    }
			}
		}
		
		function gerar(){
			// Recupernado as partes a serem mostradas
			selecionado='&localobra='+document.getElementById("localobra").checked;
			selecionado+='&coordenadas='+document.getElementById("coordenadas").checked;
			selecionado+='&coordenadas_img='+document.getElementById("coordenadas_img").checked;
			selecionado+='&contatos='+document.getElementById("contatos").checked;
			selecionado+='&contratacao='+document.getElementById("contratacao").checked;
			selecionado+='&infra='+document.getElementById("infra").checked;
			selecionado+='&execucao_orcamentaria='+document.getElementById("execucao_orcamentaria").checked;
			selecionado+='&projetos='+document.getElementById("projetos").checked;
			selecionado+='&etapas='+document.getElementById("etapas").checked;
			selecionado+='&cronograma='+document.getElementById("cronograma").checked;
			selecionado+='&galeria='+document.getElementById("galeria").checked;
			selecionado+='&num_fotos_galeria='+document.getElementById("num_fotos_galeria").value;
			selecionado+='&vistoria='+document.getElementById("vistoria").checked;
			
			// lendo as vistorias e as respectivas quantidades de fotos
			var idvistoria = document.getElementsByName("idvistoria[]");
			vistorias="&vistorias=";
			for(i=0; i<idvistoria.length; i++) {
				if (idvistoria[i].checked == true){
					vistorias+=idvistoria[i].value;
					try{	
						n_fotos_vistoria="num_fotos_vistoria_"+idvistoria[i].value;
						n_fotos_vistoria=document.getElementById(n_fotos_vistoria);
						vistorias+="."+n_fotos_vistoria.value;
					} catch(e){ }
					vistorias+="_";
				}
			}
			selecionado+=vistorias;
			// Fotos da Galeria (ignorado quantidade)
			selecionado+="&sel_fotos_galeria="+document.getElementById("selecao_fotos_galeria").value;
			
			
			// Fotos da Vistoria (ignorado quantidade)
			fotos_vistoria=document.getElementsByName("selecao_fotos_vistoria[]");
			var idfotos_vistoria_todos="";
			for(i=0; i<fotos_vistoria.length; i++) {
				id_vistoria=fotos_vistoria[i].id;
				id_vistoria= id_vistoria.substr(23,id_vistoria.length);
				idfotos_vistoria=document.getElementById("selecao_fotos_vistoria_"+id_vistoria).value;
				if (idfotos_vistoria!="0"){
					idfotos_vistoria_todos += "_"+id_vistoria+"."+idfotos_vistoria;
				}
			}
			selecionado+="&selecao_fotos_vistoria="+idfotos_vistoria_todos;
			window.open('?modulo=principal/extrato_obra&acao=A&'+selecionado);//, 'Extrato da Obra','width=780,height=500,scrollbars=yes');
			//janela.focus();
		}
function onOffBloco( bloco )
	{
		var div_on = document.getElementById( bloco + '_div_filtros_on' );
		var div_off = document.getElementById( bloco + '_div_filtros_off' );
		var img = document.getElementById( bloco + '_img' );
		var input = document.getElementById( bloco + '_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '0';
			img.src = '/imagens/menos.gif';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '1';
			img.src = '/imagens/mais.gif';
		}
	}		
	</script>
	<body>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr bgcolor="#cdcdcd">
				<td colspan="2" valign="top">
					<strong>Selecione o(s) Ítens(s) para o Extrato</strong>
				</td>
			</tr>
			<tr bgcolor=#e0e0e0>
				<td>
					<input type="checkbox" value="todos" name="selecionar_todos" id="selecionar_todos" onclick="selecionaTodos();"> <strong>Selecionar Todos</strong>
				</td>
			</tr>
		</table>
		<form name=form_extrato id=form_extrato>
	
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td>
			<input type="checkbox" value="1" name="localobra" id="localobra"> Local da Obra 
				</td>
			</tr>
			<tr bgcolor=#e0e0e0>
				<td>
			<input type="checkbox" value="1" name="coordenadas" id="coordenadas" onClick="javascript: mostrar(div_coordenadas);"> Coordenadas Geográficas
				<div id=div_coordenadas style='display:none'>&nbsp &nbsp &nbsp &nbsp<input type="checkbox" value="1" name="coordenadas_img" id="coordenadas_img"> Imprimir mapa	</div>
				</td>
			</tr>
			<tr>
				<td>
				<input type="checkbox" value="1" name="contatos" id="contatos"> Contatos
				</td>
			</tr>
			<tr  bgcolor=#e0e0e0>
				<td>
				<input type="checkbox" value="1" name="contratacao" id="contratacao"> Contratação 
				</td>
			</tr>
			<tr>
				<td>
				<input type="checkbox" value="1" name="infra" id="infra"> Infra-Estrutura
				</td>
			</tr>
			<tr  bgcolor=#e0e0e0>
				<td>
				<input type="checkbox" value="1" name="execucao_orcamentaria" id="execucao_orcamentaria"> Execução Orçamentária
				</td>
			</tr>
			<tr>
				<td>
				<input type="checkbox" value="1" name="projetos" id="projetos"> Projetos  
				</td>
			</tr>
			<tr  bgcolor=#e0e0e0>
				<td>
				<input type="checkbox" value="1" name="etapas" id="etapas"> Etapas da Obra
				</td>
			</tr>
			<tr>
				<td>
				<input type="checkbox" value="1" name="cronograma" id="cronograma"> Cronograma Físico-Financeiro
				</td>
			</tr>
			<tr bgcolor=#e0e0e0>
				<td>
				<?php
				$sql = "SELECT arqnome 
					FROM public.arquivo arq
						INNER JOIN obras.arquivosobra oar ON arq.arqid = oar.arqid
						INNER JOIN obras.obrainfraestrutura obr ON obr.obrid = oar.obrid 
						INNER JOIN seguranca.usuario seg ON seg.usucpf = oar.usucpf 
					WHERE obr.obrid = {$_SESSION["obra"]["obrid"]} AND
						  aqostatus = 'A' AND
						  (arqtipo = 'image/jpeg' OR 
						   arqtipo = 'image/gif' OR 
						   arqtipo = 'image/png')";
				$fotos = (count($db->carregar($sql)));
				?>
				<input type="checkbox" value="1" name="galeria" id="galeria" onClick="javascript: mostrar(div_galeria);"> Galeria de Fotos
				<div id=div_galeria style='display:none'>&nbsp &nbsp &nbsp &nbsp Nº de fotos a ser exibido: <input type=text name=num_fotos_galeria id=num_fotos_galeria value="<?php echo $fotos; ?>" size=2 maxlength=4> de <?php echo $fotos; ?>
				ou <input type=button value='Selecionar Fotos' onClick="javascript:selecionar_fotos('galeria');">				
				<input type=hidden id="selecao_fotos_galeria" value="0">
				</div> 
				</td>
			</tr>
			<tr>
				<td>
				<input type="checkbox" value="1" name="vistoria" id="vistoria" onClick="javascript: mostrar(div_vistoria);"> Vistoria da Obra				
				<div id=div_vistoria style='display:none'>
				
				<?php
				$sql = "
					SELECT
						s.*,
						to_char(s.supvdt,'DD/MM/YYYY') as dtvistoria,
						to_char(s.supdtinclusao,'DD/MM/YYYY') as dtinclusao,						
						u.usunome,
						si.stodesc,
						s.suprealizacao as responsavel,
						s.supvid,
						s.usucpf
					FROM
						obras.supervisao s
					INNER JOIN 
						obras.situacaoobra si ON si.stoid = s.stoid
					INNER JOIN
						seguranca.usuario u ON u.usucpf = s.usucpf
					WHERE
						s.obrid = '" . $_SESSION["obra"]["obrid"] . "' AND
						s.supstatus = 'A'
					ORDER BY 
						s.supdtinclusao ASC";
				$dados = $db->carregar( $sql );			   

				if($dados){
					echo "<table border=0 cellspacing=0=3 align=left class=tabela style='position:relative; left:20px'>
					<tr>
						<td class=SubTituloEsquerda></td>
						<td class=SubTituloEsquerda></td>
						<td class=SubTituloEsquerda>Nº</td>
						<td class=SubTituloEsquerda>Data Vistoria</td>
						<td class=SubTituloEsquerda>Data Inclusão</td>
						<td class=SubTituloEsquerda>Responsável</td>
						<td class=SubTituloEsquerda>Situação da Obra</td>
						<td class=SubTituloEsquerda>Realizada Por</td>
					</tr>";
					for($i=0;$i < count($dados);$i++){
						$ordem=$i+1;
						$supvid=$dados[$i]["supvid"];

						$sql = "SELECT fot.*, arq.arqdescricao 
								FROM obras.fotos AS fot
									LEFT JOIN public.arquivo AS arq ON arq.arqid = fot.arqid
								WHERE obrid =".$_SESSION["obra"]["obrid"]." AND supvid=".$supvid;
						$fotos_vistoria = ($db->carregar($sql));
						if(is_array($fotos_vistoria)){						
							$num_fotos_vistoria=count($fotos_vistoria);
							$tem_foto="<img src=/imagens/cam_foto.gif border=0 title='Possui fotos'>";
						}
						echo "
								<tr>
									<td><input type=checkbox id=idvistoria_".$supvid." name=idvistoria[] value=".$supvid." onClick='javascript: mostrar(div_idvistoria_".$supvid.")'></td>
									<td>".$tem_foto."</td>
									<td>".$ordem."</td>
									<td>".$dados[$i]["dtvistoria"]."</td>
									<td>".$dados[$i]["dtinclusao"]."</td>
									<td>".$dados[$i]["responsavel"]."</td>
									<td>".$dados[$i]["stodesc"]."</td>
									<td>".$dados[$i]["usunome"]."</td>
								</tr>";
						
						if(is_array($fotos_vistoria)){						
							echo"
								<tr><td></td><td></td><td></td><td colspan=5 bgcolor=#e0e0e0>
								<div id=div_idvistoria_".$supvid." style='display:none'>
								Nº de fotos a ser exibido: <input type=text id=num_fotos_vistoria_".$supvid." name=num_fotos_vistoria[] value=".$num_fotos_vistoria." size=2 maxlength=4> de ".$num_fotos_vistoria."
								ou <input type=button value='Selecionar Fotos' onClick=\"javascript:selecionar_fotos('vistoria_".$supvid."');\">				
								<input type=hidden id='selecao_fotos_vistoria_".$supvid."' name='selecao_fotos_vistoria[]' value=0>
								</div></td></tr>
								";
						}
					}
					echo "</table>";
				}			
				?>
				</div>
				</td>
			</tr>
			<tr bgcolor="#C0C0C0">
				<td>
					<input type="button" name="ok" value="Imprimir" onclick="javascript: gerar();">
				</td>
			</tr>
		</table>
		</form>
