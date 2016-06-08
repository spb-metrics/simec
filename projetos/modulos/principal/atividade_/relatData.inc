<?php

$atividade = atividade_pegar( $_REQUEST['atiid'] );
if ( !$atividade ) {
	redirecionar( 'principal/atividade_/arvore', 'A' );
}

if($_POST['requisicao'] == 'montaPai'){

	$dataAtual = date('Y-m-d');
	$numero = trim($atividade['_atinumero']);
	$tamanho2 = strlen($numero)+1;
	$sql = "SELECT DISTINCT
					a.atiid,
					_atinumero as atinumero,
					a.atidescricao,
					to_char(a.atidatainicio, 'DD/MM/YYYY') as atidatainicio,
					to_char(a.atidatafim, 'DD/MM/YYYY') as atidatafim,
					a.atiidpai,
					(select to_char(x.atidatainicio, 'DD/MM/YYYY') from projetos.atividade x where x.atiid = a.atiidpai) as atidatainiciopai,
					(select to_char(x.atidatafim, 'DD/MM/YYYY') from projetos.atividade x where x.atiid = a.atiidpai) as atidatafimpai,
					a._atiprofundidade as atiprof, atistatus, a._atiordem,
					case when ((select x.atidatainicio from projetos.atividade x where x.atiid = a.atiidpai) > a.atidatainicio) then 'Data Inicial' else '' end as prob1,
					case when ((select x.atidatafim from projetos.atividade x where x.atiid = a.atiidpai) < a.atidatafim) then 'Data Final' else '' end as prob2,
					case when (a.atidatafim < '$dataAtual') then 'Vencida' else '' end as prob3,
					case when (a.esaid = 2 and (a.atiporcentoexec = 0 OR atiporcentoexec is null )) then 'Sem Porcentagem' else '' end as prob4,
					case when (a.esaid = 2 and a.atidatainicio is null) then 'Sem Data Inicial' else '' end as prob6,
					case when (a.esaid = 2 and a.atidatafim is null) then 'Sem Data Final' else '' end as prob7,
					case when (a.esaid = 1 AND (select count(x.atiid) from projetos.atividade x WHERE x.atiidpai = a.atiid AND x.esaid = 2) > 0 ) then 'Sem Iniciar com filhos iniciados' else '' end as prob5
				FROM
					projetos.atividade a
				WHERE
					(a.esaid = 1 OR a.esaid = 2) AND 
					a._atiprojeto = 3
					AND a.atistatus = 'A'
					AND a.esaid IN ('1','2')
					AND SUBSTR(a._atinumero,0,$tamanho2) = '$numero'						
					AND ( (select x.atidatainicio from projetos.atividade x where x.atiid = a.atiidpai) > a.atidatainicio
					OR  (select x.atidatafim from projetos.atividade x where x.atiid = a.atiidpai) < a.atidatafim
					OR  a.atidatafim < '$dataAtual'  OR  a.esaid = 2 and (a.atiporcentoexec = 0 OR atiporcentoexec is null)
					OR (a.esaid = 1 AND (a.esaid = 1 AND (select count(x.atiid) from projetos.atividade x WHERE x.atiidpai = a.atiid AND x.esaid = 2) > 0 ))
					OR  (a.esaid = 2 and a.atidatainicio is null)
					OR  (a.esaid = 2 and a.atidatafim is null))
				ORDER BY
					a._atiordem";
		
	$arDados = $db->carregar($sql);
	$arDados = ($arDados) ? $arDados : array();
	
	$count = 0;
	foreach($arDados as $dados){
		$arDados[$count]['atidescricao'] = utf8_encode($dados['atidescricao']);
		$count++;
	}
	
	echo json_encode($arDados);
	die;
}

//Chamada de programa
// ----- VERIFICA SE PROJETO ESTÁ SELECIONADO
projeto_verifica_selecionado();

// ----- CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '&atiid=' . $atividade['atiid'] );
montar_titulo_projeto( $atividade['atidescricao'] );

echo "<table class=\"tabela\" bgcolor=\"#fbfbfb\" cellspacing=\"0\" cellpadding=\"10\" align=\"center\" style=\"border-top: none;\">
	<tr>
		<td>";
			echo montar_resumo_atividade( $atividade );
				$profundidade = $_REQUEST["profundidade"] + $atividade["_atiprofundidade"];
				//echo arvore( $_REQUEST["atiid"], $profundidade, $situacao, $usuario, null, null, array() );
		echo "</td>
	</tr>
</table>";
echo "<div id=\"lista\">";
	echo "<table id=\"tabela_data\" class=\"tabela\" bgcolor=\"#f5f5f5\" cellpadding=\"3\" align=\"center\">";
	echo "<tr style=\"background-color: #e0e0e0\">
				<td style=\"font-weight:bold; text-align:center; width:30%;\">Descrição</td>
				<td style=\"font-weight:bold; text-align:center; width:15%;\">Data Inicial da Atividade</td>
				<td style=\"font-weight:bold; text-align:center; width:15%;\">Data Final da Atividade</td>
				<td style=\"font-weight:bold; text-align:center; width:15%;\">Data Inicial da Atividade Pai</td>
				<td style=\"font-weight:bold; text-align:center; width:15%;\">Data Final da Atividade Pai</td>
				<td style=\"font-weight:bold; text-align:center; width:10%;\">Problema Identificado</td>				
			</tr>
		  ";
	echo "</table>
		  </div>";


?>
<script type="text/javascript" src="../includes/prototype.js"></script>

<div id="divCarregando" style="width:100%;text-align:center;margin-top:15%; display: none;" ><img src="../imagens/carregando.gif" border="0" align="middle"><br />Carregando...</div>
<div id="teste"></div>

<script type="text/javascript"><!--

function montaPai() {
	var maxRows = $('tabela_data').rows.length;
	if(maxRows > 1){
		for (var i = 1; i < maxRows; i++) {
			$('tabela_data').deleteRow(1);
		}
	}
//	var data = 'requisicao=montaPai&filtro='+filtro;
	//var data = $('formulario').serialize(true);
	var data = 'requisicao=montaPai';
	var cor = "#f0f0f0";
	var aj = new Ajax.Request(window.location.href,  
	{  
		method: 'post',
		asynchronous: false,
		parameters: data,
		onLoading: $('divCarregando').show(),
		onComplete: function(r)
		{
			if(r.responseText != ""){
			//	$('teste').update(r.responseText);
			//	return false;
				var arDados = eval('(' + r.responseText + ')');
				for (var j = 0; j < arDados.length; j++) {
					var atiid = arDados[j].atiid;
					var atinumero = arDados[j].atinumero;
					var atidescricao = arDados[j].atidescricao;
					var atidatainicio = arDados[j].atidatainicio;
					var atidatafim = arDados[j].atidatafim;
					var atidatainiciopai = arDados[j].atidatainiciopai;
					var atidatafimpai = arDados[j].atidatafimpai;
					var atiprof = arDados[j].atiprof;
					var prob1 = arDados[j].prob1;
					var prob2 = arDados[j].prob2;
					var prob3 = arDados[j].prob3;
					var prob4 = arDados[j].prob4;
					var prob5 = arDados[j].prob5;
					var prob6 = arDados[j].prob6;
					var prob7 = arDados[j].prob7;


					var tr = $('tabela_data').insertRow($('tabela_data').rows.length);
					tr.id = atiid;
					
		       		if(cor == "#fafafa") {
						tr.style.backgroundColor = "#f0f0f0";
						cor = "#f0f0f0";
					} else {
						tr.style.backgroundColor = "#fafafa";
						cor = "#fafafa";
					}
					if(atinumero == 1 || atinumero == 2 || atinumero == 3 || atinumero == 4 || atinumero == 5){
						tr.style.backgroundColor = "#E0E0E0";
						cor = "#E0E0E0";
					}

		       		var espaco     = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
		       		var espacoTemp = espaco;

	       			if(atiprof > 1){
			       		for (var y = 1; y < atiprof; y++) {
		            		espacoTemp = espacoTemp + espaco;
		            	}	       			
	       			} 
	        		
	        		td1 = tr.insertCell(0);
	        		td1.innerHTML = espacoTemp+"<a href='projetos.php?modulo=principal/atividade_/atividade&acao=A&atiid="+atiid+"'><b>"+atinumero+' - '+atidescricao+"</b></a>";

	        		td2 = tr.insertCell(1);
					if(atidatainicio){
	        			td2.innerHTML = "<center>"+atidatainicio+"</center>";
					}

					td3 = tr.insertCell(2);
					if(atidatafim){
	        			td3.innerHTML = "<center>"+atidatafim+"</center>";
					}
					
					td4 = tr.insertCell(3);
	        		if(atidatainiciopai){
		        		td4.innerHTML = "<center>"+atidatainiciopai+"</center>";
	        		}

	        		td5 = tr.insertCell(4);
	        		if(atidatafimpai){
		        		td5.innerHTML = "<center>"+atidatafimpai+"</center>";
	        		}

	        		td6 = tr.insertCell(5);
	        		var stProblema = '';
	        		if(prob1){
	        			stProblema += prob1+",<br>";
	        		}
	        		if(prob2){
	        			stProblema += prob2+",<br>";
	        		}
	        		if(prob3){
	        			stProblema += prob3+",<br>";
	        		}
	        		if(prob4){
	        			stProblema += prob4+",<br>";
	        		}
	        		if(prob5){
	        			stProblema += prob5+",<br>";
	        		}
	        		if(prob6){
	        			stProblema += prob6+",<br>";
	        		}
	        		if(prob7){
	        			stProblema += prob7+",<br>";
	        		}
	        		if(stProblema){
	        			stProblema = stProblema.substr(0,(stProblema.length - 5));
		        		td6.innerHTML = "<center>"+stProblema+"</center>";
		        	}
				}
			} else {
			
			}
		}
		
	});
	$('divCarregando').hide();
}

montaPai();

--></script>