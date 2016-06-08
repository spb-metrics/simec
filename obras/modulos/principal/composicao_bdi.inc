<?
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php'; 


$obras = new Obras();
echo "<br />";
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo = "Monitoramento de Obras/Infraestrutura";
monta_titulo( $titulo_modulo, 'Composição do BDI' );
echo $obras->CabecalhoObras();
/* FIM - Cabeçalho padrão */
?>
<script language="JavaScript" src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="JavaScript">
/* Função carrega as tabelas em valores para serem enviados */
function submeterFomulario() {
	var tabela1 = document.getElementById("tabela1");

	var	form = document.getElementById("form");
	// Verifica se a tabela1 possui algum registro e se o registro é o texto indicando que não existem itens 
	if(tabela1.rows.length > 2 && tabela1.rows[2].cells.length != 1) {
		for(i = 2; i < tabela1.rows.length; i++) {
		// Criando Array para ser enviado no formulário
		var newField1 = document.createElement("input");
		newField1.setAttribute('type', "hidden");
		newField1.setAttribute('name', "tabela1_itensbdi["+i+"][bdidesc]");
		newField1.setAttribute('value', tabela1.rows[i].cells[1].innerHTML);
		form.appendChild(newField1);
		var newField2 = document.createElement("input");
		newField2.setAttribute('type', "hidden");
		newField2.setAttribute('name', "tabela1_itensbdi["+i+"][bdivlritem]");
		newField2.setAttribute('value', tabela1.rows[i].cells[2].innerHTML);
		form.appendChild(newField2);
		var newField3 = document.createElement("input");
		newField3.setAttribute('type', "hidden");
		newField3.setAttribute('name', "tabela1_itensbdi["+i+"][bdipercitem]");
		newField3.setAttribute('value', tabela1.rows[i].cells[3].innerHTML);
		formulario.appendChild(newField3);
		}
	}

	form.submit();

}
/* Função para deletar uma linha da tabela */
function deletarLinha(linha,tabelaid) {
	var tabela = document.getElementById(tabelaid);
	tabela.deleteRow(linha);
	if(tabela.rows.length == 2) {
		var linha = tabela.insertRow(2);
		var mensagem = linha.insertCell(0);
		mensagem.colSpan = 5;
		mensagem.innerHTML = "Não existem itens cadastrados.";
	}
	calculaValor();
}

/* Função para editar linha da tabela */
function editarLinha(linha,tabelaid) {
	
	var tabela = document.getElementById(tabelaid);
	
	if(document.formulario.bdidesc_tabela1.value != "" && document.formulario.bdivlritem_tabela1.value != "" && document.formulario.bdipercitem_tabela1.value != "") {
		alert("Já existe um item em edição.");
	}else{
	document.getElementById("bdidesc_"+tabelaid).value = tabela.rows[linha].cells[1].innerHTML;
	document.getElementById("bdivlritem_"+tabelaid).value = tabela.rows[linha].cells[2].innerHTML;
	document.getElementById("bdipercitem_"+tabelaid).value = tabela.rows[linha].cells[3].innerHTML;
	tabela.deleteRow(linha);
	}

	if(tabela.rows.length == 2) {
		var linha = tabela.insertRow(2);
		var mensagem = linha.insertCell(0);
		mensagem.colSpan = 5;
		mensagem.innerHTML = "Não existem itens cadastrados.";
	}
}

/* Função varre a tabela recalculando as porcentagens */
function recalculaPorcentagem(tabelaid) {
	var tabela = document.getElementById(tabelaid);
	//vt = replaceAll(document.getElementById("vlrtotal_"+tabelaid).value, ".", "");
	//vt = parseFloat(replaceAll(vt,",","."));
	/*
	for(i = 2; i < tabela.rows.length; i++) {
		empenhado = tabela.rows[i].cells[2].innerHTML;
		empenhado = replaceAll(empenhado,".","");
		empenhado = parseFloat(replaceAll(empenhado,",","."));
		liquidado = tabela.rows[i].cells[3].innerHTML;
		liquidado = replaceAll(liquidado,".","");
		liquidado = parseFloat(replaceAll(liquidado,",","."));
		if(vt != 0) {
			percempenhado = (empenhado/vt*100);
			percempenhado = percempenhado.toFixed(1);
			tabela.rows[i].cells[5].innerHTML = replaceAll(percempenhado,".",",")+'%';
			percliquidado = (liquidado/vt*100);
			percliquidado = percliquidado.toFixed(1);
			tabela.rows[i].cells[6].innerHTML = replaceAll(percliquidado,".",",")+'%';
		}
	}
	*/
}

/* Função varre a tabela recalculando as porcentagens */
function calculaValor(tipo) {

	
	var tabela = document.getElementById("tabela1");

	vl_bdi = replaceAll(document.formulario.vl_bdi.value, ".", "");
	vl_bdi = parseFloat(replaceAll(vl_bdi,",","."));

	perc_bdi = replaceAll(document.formulario.perc_bdi.value, ".", "");
	perc_bdi = parseFloat(replaceAll(perc_bdi,",","."));
	
	bdivlritem = replaceAll(document.formulario.bdivlritem_tabela1.value, ".", "");
	bdivlritem = parseFloat(replaceAll(bdivlritem,",","."));
	if(isNaN(bdivlritem)) bdivlritem = 0;
	
	bdipercitem = replaceAll(document.formulario.bdipercitem_tabela1.value, ".", "");
	bdipercitem = parseFloat(replaceAll(bdipercitem,",","."));
	if(isNaN(bdipercitem)) bdipercitem = 0;
	
	total_valor = 0;
	total_perc = 0;
	
	if(tabela.rows[2].cells[0].innerHTML != "Não existem itens cadastrados."){
		for(i = 2; i < tabela.rows.length; i++) {
			valor = tabela.rows[i].cells[2].innerHTML;
			valor = replaceAll(valor,".","");
			valor = parseFloat(replaceAll(valor,",","."));
			total_valor += valor;

			perc = tabela.rows[i].cells[3].innerHTML;
			perc = replaceAll(perc,".","");
			perc = parseFloat(replaceAll(perc,",","."));
			total_perc += perc;

		}
	}
	
	//onblur para o campo (%)Percentual
	if(tipo=="2"){
		bdivlritem = ((bdipercitem/100)*vl_bdi);
		document.formulario.bdivlritem_tabela1.value = float2moeda(bdivlritem);
	}
	
	if((bdivlritem + total_valor) > vl_bdi){
		alert("A soma dos campos 'Valor(R$)' não pode ser maior que o Valor do BDI");
		document.formulario.bdivlritem_tabela1.value = "";
		document.formulario.bdipercitem_tabela1.value = "";
		//calcula valor total
		document.formulario.t_vl.value = float2moeda(total_valor);
		//calcula percent total
		document.formulario.t_perc.value = float2moeda(total_perc);
		//calcula valor restante
		document.formulario.vl_rest.value = float2moeda(vl_bdi-total_valor);
		//calcula percent restante
		document.formulario.perc_rest.value = float2moeda(perc_bdi-total_perc);
	}
	else{
		//calcula percent item
		document.formulario.bdipercitem_tabela1.value = float2moeda((bdivlritem * 100) / vl_bdi);
		//calcula valor total
		document.formulario.t_vl.value = float2moeda(bdivlritem + total_valor);
		//calcula percent total
		var t_perc = ((bdivlritem * 100) / vl_bdi) + total_perc;
		document.formulario.t_perc.value = float2moeda(((bdivlritem * 100) / vl_bdi) + total_perc);
		//calcula valor restante
		document.formulario.vl_rest.value = float2moeda(vl_bdi-(bdivlritem + total_valor));
		//calcula precent restante
		document.formulario.perc_rest.value = float2moeda(perc_bdi-t_perc);
	}
	
	if(document.formulario.bdivlritem_tabela1.value == "0,00" || document.formulario.bdipercitem_tabela1.value == "0,00")
	{
		document.formulario.bdivlritem_tabela1.value = "";
		document.formulario.bdipercitem_tabela1.value = "";
	}
	
	
}


/* Função para subustituir todos */
function replaceAll(str, de, para){
    var pos = str.indexOf(de);
    while (pos > -1){
		str = str.replace(de, para);
		pos = str.indexOf(de);
	}
    return (str);
}
/* Função para adicionar linha nas tabelas */
function adicionarLinha(tabelaid) {
	var tabela = document.getElementById(tabelaid);
	// Verificando se todos os campos foram preenchidos
	if(document.getElementById("bdipercitem_"+tabelaid).value == "" ||
	   document.getElementById("bdivlritem_"+tabelaid).value == "" ||
	   document.getElementById("bdidesc_"+tabelaid).value == "") {
		alert("Todos os campos devem ser preenchidos!");
		return false;
	}
	/*
	// valor total disponivel
	var v2 = replaceAll(document.getElementById("vlrtotal_"+tabelaid).value,".", "");
	v2 = parseFloat(replaceAll(v2,",","."));
	// valor empenhado
	var v1 = replaceAll(document.getElementById("bdivlritem_"+tabelaid).value,".", "");
	v1 = parseFloat(replaceAll(v1,",","."));
	// valor liquidado
	var v3 = replaceAll(document.getElementById("insvlrliquidado_"+tabelaid).value,".", "");
	v3 = parseFloat(replaceAll(v3,",","."));
	// Validando regras 
	if(v1 < v3) {
		alert("Valor empenhado deve ser maior/igual ao valor liquidado.");
		return false;
	}
	if(v1 > v2) {
		alert("Valor empenhado não pode ser maior que o valor total.");
		return false;
	}
	*/
	// Verificando ser a primeira linha não é um texto informando que não existe registros
	if(tabela.rows.length == 3) {
		if(tabela.rows[2].cells.length == 1) {
			tabela.deleteRow(2);
		}
	}
	var tamanho = tabela.rows.length;
	var linha = tabela.insertRow(tamanho);
	if(tabela.rows[(linha.rowIndex-1)].style.backgroundColor == "") {
		linha.style.backgroundColor = "#ffffff";
	} else {
		linha.style.backgroundColor = "";
	}
	var acoes = linha.insertCell(0);
	acoes.innerHTML = "<a style=\"cursor: pointer;\" onclick=\"editarLinha(this.parentNode.parentNode.rowIndex,'"+tabelaid+"');\"><img src='../imagens/alterar.gif'></a> <a style=\"cursor: pointer;\" onclick=\"deletarLinha(this.parentNode.parentNode.rowIndex,'"+tabelaid+"');\"><img src='../imagens/excluir.gif'></a>";
	acoes.setAttribute("align","center");
	var bdidesc = linha.insertCell(1);
	bdidesc.innerHTML = document.getElementById("bdidesc_"+tabelaid).value;
	var bdivlritem = linha.insertCell(2);
	bdivlritem.innerHTML = document.getElementById("bdivlritem_"+tabelaid).value;
	bdivlritem.setAttribute("align","center");
	var bdipercitem = linha.insertCell(3);
	bdipercitem.innerHTML = document.getElementById("bdipercitem_"+tabelaid).value;
	bdipercitem.setAttribute("align","center");

	
	/*
	var percempenhado = linha.insertCell(5);
	var percliquidado = linha.insertCell(6);
	
	if(v2 == 0) {
		percliquidado.innerHTML = '0%';
		percempenhado.innerHTML = '0%';
	} else {
		porcempenhado = (v1/v2*100);
		porcempenhado = porcempenhado.toFixed(1);
		percempenhado.innerHTML = porcempenhado.replace(".",",")+'%';
	
		porcliquidado = (v3/v2*100);
		porcliquidado = porcliquidado.toFixed(1);
		percliquidado.innerHTML = porcliquidado.replace(".",",")+'%';
	}
	*/
	document.getElementById("bdidesc_"+tabelaid).value = "";
	document.getElementById("bdivlritem_"+tabelaid).value = "";
	document.getElementById("bdipercitem_"+tabelaid).value = "";
}
/* Função para transformar inteiro em formato de moeda (1002.54 -> 1.002,54) */
function float2moeda(num) {
   x = 0;
   if(num<0) {
      num = Math.abs(num);
      x = 1;
   }
   if(isNaN(num)) num = "0";
      cents = Math.floor((num*100+0.5)%100);
   num = Math.floor((num*100+0.5)/100).toString();
   if(cents < 10) cents = "0" + cents;
      for (var i = 0; i < Math.floor((num.length-(1+i))/3); i++)
         num = num.substring(0,num.length-(4*i+3))+'.'
               +num.substring(num.length-(4*i+3));
    ret = num + ',' + cents;
    if (x == 1) ret = ' - ' + ret;return ret;
}
/* Função usada para somar os orçamentos, somente para obras faz tratamento especial */
function somaOrcObras(tabelaid) {
	switch(tabelaid) {
	case 'tabela1':
		//valor1 = replaceAll(document.getElementById("eocvlrcapital_1").value,".","");
		//valor1 = parseFloat(replaceAll(valor1,",","."));
		//valor2 = replaceAll(document.getElementById("eocvlrcusteio").value,".", "");
		//valor2 = parseFloat(replaceAll(valor2,",", "."));
		//document.getElementById("vlrtotal_"+tabelaid).value = float2moeda(valor1 + valor2);
	default:
		recalculaPorcentagem(tabelaid);
	}
}
</script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<style type="text/css">
.colCabecalho_execorc {
	background-color: #dcdcdc;
	font-weight: bold;
	
}
.colTitulo_execorc {
	background-color: #dcdcdc;
	text-align: center;
	font-weight: bold;
	font-size: 14px;
}
</style>
<?

if($_SESSION['obra']['obrid']){
		//$composicaobdi = new ComposicaoBdi();
		//$resultado = $composicaobdi->busca($_SESSION['obra']['obrid']);	
		//$dados2 = $composicaobdi->dados($resultado);
		
		$sql = "SELECT obrcustocontrato, obrpercbdi	FROM obras.obrainfraestrutura WHERE obrid = '".$_SESSION['obra']['obrid']."'";
		$res = $db->carregar($sql);
		foreach($res as $res2) {
			$obrcustocontrato = $res2['obrcustocontrato'];
			$obrpercbdi = $res2['obrpercbdi'];
		}
}

	$sql = pg_query("SELECT * FROM obras.itensbdi WHERE	obrid = '".$_SESSION['obra']['obrid']."'");
	
	$count = 1;
	
	while (($dados = pg_fetch_array($sql)) != false) {
		
		$cor = "#f4f4f4";
		$count++;
		$nome = "linha_".$itcid;
		if ($count % 2)	$cor = "#e0e0e0";
				
		$bdidesc = $dados['bdidesc'];
		$bdivlritem = $dados['bdivlritem'];
		$bdipercitem = $dados['bdipercitem'];
			
		$t_vl += $bdivlritem;
		$t_perc += $bdipercitem;
			
		$htmlitens .= "<tr bgcolor='".$cor."' onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='$cor';\">
						<td align=center><a style=\"cursor: pointer;\" onclick=\"editarLinha(this.parentNode.parentNode.rowIndex,'tabela1');\"><img src='../imagens/alterar.gif'></a> <a style=\"cursor: pointer;\" onclick=\"deletarLinha(this.parentNode.parentNode.rowIndex,'tabela1');\"><img src='../imagens/excluir.gif'></a></td>
						<td>". $bdidesc ."</td>
						<td align=center>". number_format($bdivlritem,2,',','.') ."</td>
						<td align=center>". number_format($bdipercitem,2,',','.') ."</td>
				  	   </tr>";
	}
		
	if($count == 1) {
		$htmlitens = "<tr>
						<td colspan='5'>Não existem itens cadastrados.</td>
					  </tr>";
	}

?>
<form name="formulario" method="post" id="form" action="?modulo=inicio&acao=CB">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="colTitulo_execorc" colspan="5">Composição do BDI</td>
	</tr>
	<tr>
		<td colspan='5'>
			<div style="height: 200px; overflow:auto">
			<table width="100%" id="tabela1">
			<tr>
				<td class="colCabecalho_execorc" align='center' width="10%">Ação</td>
				<td class="colCabecalho_execorc" align='center' width="50%">Descrição do Item</td>
				<td class="colCabecalho_execorc" align='center' width="20%">Valor (R$)</td>
				<td class="colCabecalho_execorc" align='center' width="20%">(%) Percentual</td>
			</tr>
			<tr>
				<td class="colCabecalho_execorc" align='center'><a style="cursor: pointer;" onclick="adicionarLinha('tabela1');"><img src="../imagens/gif_inclui.gif"></a></td>
				<td class="colCabecalho_execorc"><?= campo_texto( 'bdidesc_tabela1', 'N', $somenteLeitura, '', 80, 100, '', '', 'left', '', 0, 'id="bdidesc_tabela1"' ); ?></td>
				<td class="colCabecalho_execorc" align='center'><input class='CampoEstilo' type='text' name='bdivlritem_tabela1' id='bdivlritem_tabela1' size='17' maxlength='14' value='' onkeyup="this.value=mascaraglobal('###.###.###,##',this.value);" onblur="calculaValor('1');"></td>
				<td class="colCabecalho_execorc" align='center'><input class='CampoEstilo' type='text' name='bdipercitem_tabela1' id='bdipercitem_tabela1' size='8' maxlength='6' value='' onkeyup="this.value=mascaraglobal('###,##',this.value);" onblur="calculaValor('2');"></td>
			</tr>
			<? echo $htmlitens; unset($htmlitens); ?>
			</table>
			<table width="100%" border=0>
			<tr bgcolor="white">
				<td width="10%">&nbsp;</td>
				<td width="50%" align="right"><b>Total:</b></td>
				<td width="20%" align="center"><input class='CampoEstilo' type='text' name='t_vl' size='17' value='<?=number_format($t_vl,2,',','.')?>' disabled></td>
				<td width="20%" align="center"><input class='CampoEstilo' type='text' name='t_perc' size='8' value='<?=number_format($t_perc,2,',','.')?>' disabled></td>
			</tr>
			<tr bgcolor="white">
				<td width="10%">&nbsp;</td>
				<td width="50%" align="right"><b>(<font color=blue><?=number_format($obrpercbdi,2,',','.')?>% do Contrato</font>) Valor do BDI:</b></td>
				<td width="20%" align="center"><input class='CampoEstilo' type='text' name='vl_bdi' size='17' value='<?=number_format(($obrpercbdi/100)*$obrcustocontrato,2,',','.')?>' disabled></td>
				<td width="20%" align="center">
					<?if($obrpercbdi>0){?>
						<input class='CampoEstilo' type='text' name='perc_bdi' size='8' value='<?=number_format(100,2,',','.')?>' disabled>
					<?}else{?>
						<input class='CampoEstilo' type='text' name='perc_bdi' size='8' value='0,00' disabled>
					<?}?>
				</td>
			</tr>
			<tr bgcolor="white">
				<td width="10%">&nbsp;</td>
				<td width="50%" align="right"><b>Valor Restante:</b></td>
				<td width="20%" align="center"><input class='CampoEstilo' type='text' name='vl_rest' size='17' value='<?=number_format((($obrpercbdi/100)*$obrcustocontrato)-$t_vl,2,',','.')?>' disabled></td>
				<td width="20%" align="center">
					<?if($obrpercbdi>0){?>
						<input class='CampoEstilo' type='text' name='perc_rest' size='8' value='<?=number_format(100-$t_perc,2,',','.')?>' disabled>
					<?}else{?>
						<input class='CampoEstilo' type='text' name='perc_rest' size='8' value='0,00' disabled>
					<?}?>
				</td>
			</tr>
			</table>
			</div>
		</td>
	</tr>
</table>
<table align="center">
	<tr>
		<td colspan="2" align="center">
			<input type="button" value="Salvar" onclick="submeterFomulario();" style="cursor: pointer" <?php if($somenteLeitura=="N") echo "disabled"; ?>> 
			<input type="button" value="Voltar" style="cursor: pointer" onclick="history.back(-1);">
		</td>
	</tr>
</table>
<input type="hidden" name="obrcustocontrato" id="obrcustocontrato" value="<?=$obrcustocontrato?>">
<input type="hidden" name="obrpercbdi" id="obrpercbdi" value="<?=$obrpercbdi?>">
</form>