<?php

if($_POST['requisicao'] == 'montaPai'){
	extract($_POST);
	//ver($_POST,trim($atiidraiz));
	
	$where = array();
	if(is_array($relatorio)){
		if(isset($relatorio['dataIni']) && $relatorio['dataIni']){
			array_push($where, " (select x.atidatainicio from projetos.atividade x where x.atiid = a.atiidpai) > a.atidatainicio ");
		}
		if(isset($relatorio['dataFim']) && $relatorio['dataFim']){
			array_push($where, " (select x.atidatafim from projetos.atividade x where x.atiid = a.atiidpai) < a.atidatafim ");
		}
		if(isset($relatorio['venc']) && $relatorio['venc']){
			$dataAtual = date('Y-m-d');
			array_push($where, " a.esaid = 2 and a.atidatafim < '$dataAtual' ");
		}
		if(isset($relatorio['percen']) && $relatorio['percen']){
			array_push($where, " a.esaid = 2 and a.atiporcentoexec = 0 ");
		}
		if(isset($relatorio['semData']) && $relatorio['semData']){
			array_push($where, " a.esaid = 2 AND a.atidatainicio is null ");
		}
		if(isset($relatorio['semDataFim']) && $relatorio['semDataFim']){
			array_push($where, " a.esaid = 2 AND a.atidatafim is null ");
		}
		
		$atiidraiz = trim($atiidraiz);
		
		if(isset($atiidraiz) && $atiidraiz){
			$tamanho = strlen($atiidraiz)+1;
			$where2 = " AND SUBSTR(a._atinumero,0,$tamanho) = '$atiidraiz' ";
		}
		
		$sql = "SELECT DISTINCT
					a.atiid,
					_atinumero as atinumero,
					a.atidescricao,
					to_char(a.atidatainicio, 'DD/MM/YYYY') as atidatainicio,
					to_char(a.atidatafim, 'DD/MM/YYYY') as atidatafim,
					a.atiidpai, 
					(select to_char(x.atidatainicio, 'DD/MM/YYYY') from projetos.atividade x where x.atiid = a.atiidpai) as atidatainiciopai,
					(select to_char(x.atidatafim, 'DD/MM/YYYY') from projetos.atividade x where x.atiid = a.atiidpai) as atidatafimpai,
					a._atiprofundidade as atiprof, atistatus, a._atiordem
				FROM
					projetos.atividade a
				WHERE
					(a.atiidpai = 3 AND a.atistatus = 'A')OR 
					a._atiprojeto = 3
					AND a.atistatus = 'A'
					$where2
					AND (" . ( is_array($where) && count($where) ? '' . implode(' OR ', $where) : '' ) .")
				ORDER BY
					a._atiordem
				";
	}
	
	//ver($sql,d);
	

		
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
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '' );

echo "
<form action=\"\" method=\"POST\" name=\"formulario\" id=\"formulario\">
<input type=\"hidden\" name=\"requisicao\" id=\"requisicao\" value=\"montaPai\" >
<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">
<tr>
	<td class=\"SubTituloEsquerda\" style='text-align: right' width=\"100\" colspan=\"2\">
		Filtros
	</td>
	<td>
		<label style=\"margin: 0 10px 0 0;\"><input type=\"checkbox\" name=\"relatorio[dataIni]\" value=\"1\" >Data Início</label>
		<label style=\"margin: 0 10px 0 0;\"><input type=\"checkbox\" name=\"relatorio[dataFim]\" value=\"2\" >Data Fim</label>
		<label style=\"margin: 0 10px 0 0;\"><input type=\"checkbox\" name=\"relatorio[venc]\" value=\"3\" >Vencidas</label>
		<label style=\"margin: 0 10px 0 0;\"><input type=\"checkbox\" name=\"relatorio[percen]\" value=\"4\" >Sem Percentual</label>
		<label style=\"margin: 0 10px 0 0;\"><input type=\"checkbox\" name=\"relatorio[semData]\" value=\"5\" >Sem Data Inicial</label>
		<label style=\"margin: 0 10px 0 0;\"><input type=\"checkbox\" name=\"relatorio[semDataFim]\" value=\"6\" >Sem Data Final</label>
	</td>
</tr>
<tr>
	<td class=\"SubTituloEsquerda\" style='text-align: right' width=\"100\" colspan=\"2\">
		Atividade
	</td>
	<td>
		<select id=\"atiidraiz\" name=\"atiidraiz\" class=\"CampoEstilo\" style=\"width: 250px;\">
			<option value=\"\">";
				$sql = "select atidescricao from projetos.atividade where atiid = 3";
				echo $db->pegaUm( $sql );
			echo"</option>";
			$sql = "
				select
					a.atiid,
					a.atidescricao,
					a._atiprofundidade as profundidade,
					a._atinumero as numero
				from projetos.atividade a
				where
					a.atistatus = 'A'
					and a._atiprofundidade < 3
					and a._atiprojeto = 3 
				order by
					a._atiordem
			";
			$lista = $db->carregar( $sql );
			$lista = $lista ? $lista : array();
			
			foreach ( $lista as $item ) {
				echo "<option value=\" {$item['numero']} \">";
					echo str_repeat( '&nbsp;', $item['profundidade'] * 5 );
					echo $item['numero'];
					echo "&nbsp;";
					echo $item['atidescricao'];
				echo"</option>";
			}
		echo"</select>
	</td>
</tr>
<tr>
	<td class=\"SubTituloEsquerda\" style='text-align: center' width=\"100\" colspan=\"2\"></td>
	<td class=\"SubTituloEsquerda\">
		<input type=\"button\" name=\"gera_relatorio\" value=\"Gerar Relatório\" onclick=\"montaPai();\"/>
	</td>
</tr>
</table>
</form>";

echo "<div id=\"lista\">";
	echo "<table id=\"tabela_data\" class=\"tabela\" bgcolor=\"#f5f5f5\" cellpadding=\"3\" align=\"center\">";
	echo "<tr style=\"background-color: #e0e0e0\">
				<td style=\"font-weight:bold; text-align:center; width:40%;\">Descrição</td>	
				<td style=\"font-weight:bold; text-align:center; width:15%;\">Data Inicial da Atividade</td>
				<td style=\"font-weight:bold; text-align:center; width:15%;\">Data Final da Atividade</td>
				<td style=\"font-weight:bold; text-align:center; width:15%;\">Data Inicial da Atividade Pai</td>
				<td style=\"font-weight:bold; text-align:center; width:15%;\">Data Final da Atividade Pai</td>			
			</tr>
		  ";
	echo "</table>
		  </div>";


?>
<script type="text/javascript" src="../includes/prototype.js"></script>

<div id="divCarregando" style="width:100%;text-align:center;margin-top:15%; display: none;" ><img src="../imagens/carregando.gif" border="0" align="middle"><br />Carregando...</div>
<div id="teste"></div>

<script type="text/javascript"><!--

function montaPai(filtro) {
	var maxRows = $('tabela_data').rows.length;
	if(maxRows > 1){
		for (var i = 1; i < maxRows; i++) {
			$('tabela_data').deleteRow(1);
		}
	}
//	var data = 'requisicao=montaPai&filtro='+filtro;
	var data = $('formulario').serialize(true);
//	var data = 'requisicao=montaPai&'+$('formulario').serialize(true);
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
	        		td1.innerHTML = espacoTemp+"<b>"+atinumero+' - '+atidescricao+"</b>";

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
					
				}
			} else {
			
			}
		}
		
	});
	$('divCarregando').hide();
}

--></script>