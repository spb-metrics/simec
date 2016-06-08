<?php

// classes do módulo
$obras = new Obras();
$execOrc = new execOrcamentaria();


if( $_REQUEST["obrid"] ){
	
	include_once APPRAIZ . "www/obras/_permissoes_obras.php";
	
	session_unregister("obra");
	$_SESSION["obra"]["obrid"] = $_REQUEST["obrid"];
	
}


if( $_SESSION["obra"]["obrid"] ){
	$dadosExecOrc = $execOrc->buscaExecOrcamentaria( $_SESSION["obra"]["obrid"] );
}

switch ($_REQUEST["requisicao"]){
	case "salvar":
		$execOrc->registraExecOrcamentaria( $_REQUEST );
	break;
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php'; 

// Pega o caminho atual do usuário (em qual módulo se encontra)
$caminho_atual   = $_SERVER["REQUEST_URI"];
$posicao_caminho = strpos($caminho_atual, 'acao');
$caminho_atual   = substr($caminho_atual, 0 , $posicao_caminho);

echo "<br />";
$db->cria_aba($abacod_tela,$url,$parametros);

$titulo_modulo = "Execução Orçamentária";
monta_titulo( $titulo_modulo, '' );

echo $obras->CabecalhoObras();

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript">

	function somaOrcObra(){

		var capital = document.getElementById( "eocvlrcapital" ).value;
		var custeio = document.getElementById( "eocvlrcusteio" ).value;
		var total   = document.getElementById( "eocvlrtotal" );
		
		var novoTotal = "";
		
		capital = capital.replace(".", "");
		capital = capital.replace(".", "");
		capital = capital.replace(".", "");
		capital = capital.replace(",", ".");

		custeio = custeio.replace(".", "");
		custeio = custeio.replace(".", "");
		custeio = custeio.replace(".", "");
		custeio = custeio.replace(",", ".");

		novoTotal = Number(capital) + Number(custeio);
		novoTotal = novoTotal.toFixed(2);
		novoTotal = mascaraglobal('###.###.###,##', novoTotal); 

		total.value = novoTotal;
	}

	function incluiDetalhamento(){

		var tabela = document.getElementById( "tabelaDetalhamento" );
		var tamanho = tabela.rows.length;

		var data      = document.getElementById( "insdtposicao" ).value;
		var empenho   = document.getElementById( "insvlrempenhado" ).value;
		var liquidado = document.getElementById( "insvlrliquidado" ).value;
		var total     = document.getElementById( "eocvlrtotal" ).value;
		
		// regras da tela
		
		if( !data || !empenho || !liquidado || !total ){
			alert("Todos os campos devem ser preenchidos!");
			return false;
		}
		
		tsEmpenho = empenho.replace(".","");
		tsEmpenho = tsEmpenho.replace(".","");
		tsEmpenho = tsEmpenho.replace(".","");
		tsEmpenho = tsEmpenho.replace(",",".");

		tsLiquidado = liquidado.replace(".","");
		tsLiquidado = tsLiquidado.replace(".","");
		tsLiquidado = tsLiquidado.replace(".","");
		tsLiquidado = tsLiquidado.replace(",",".");

		tsTotal = total.replace(".","");
		tsTotal = tsTotal.replace(".","");
		tsTotal = tsTotal.replace(".","");
		tsTotal = tsTotal.replace(",",".");

		if( Number(tsLiquidado) > Number(tsEmpenho) ){
			alert("O valor empenhado deve ser maior/igual ao valor liquidado!");
			return false;
		}

		if( Number(tsEmpenho) > Number(tsTotal) ){
			alert("O valor empenhado não pode ser maior que o valor total!");
			return false;
		}

		// cria a tr
		var linha = tabela.insertRow(tamanho);

		// colore a linha
		if ( tamanho ){
			if( tabela.rows[tamanho-1].style.backgroundColor == "rgb(224, 224, 224)" ){
				linha.style.backgroundColor = "#f4f4f4";					
			}else{
				linha.style.backgroundColor = "#e0e0e0";					
			}
		}else{
			linha.style.backgroundColor = "#f4f4f4";
		}

		// cria as tds
		var colData   		= linha.insertCell(0);
		var colEmpenhado 	= linha.insertCell(1);
		var colLiquidado    = linha.insertCell(2);
		var colPerEmpenhado = linha.insertCell(3);
		var colPerLiquidado = linha.insertCell(4);
		var colAcao 		= linha.insertCell(5);

		colData.style.width   		= "15%";
		colEmpenhado.style.width    = "20%";
		colLiquidado.style.width    = "20%";
		colPerEmpenhado.style.width = "15%";
		colPerLiquidado.style.width = "15%";
		colAcao.style.width  	 	= "14%";

		colData.style.textAlign		    = "center";
		colEmpenhado.style.textAlign 	= "right";
		colLiquidado.style.textAlign 	= "right";
		colPerEmpenhado.style.textAlign = "right";
		colPerLiquidado.style.textAlign = "right";
		colAcao.style.textAlign 	  	= "center";

		colData.innerHTML 		  = "<input type='hidden' name='eocdtposicao[]' id='eocdtposicao[]' value='" + data + "'/>" + data;
		colEmpenhado.innerHTML 	  = "<input type='hidden' name='eocvlrempenhado[]' id='eocvlrempenhado[]' value='" + tsEmpenho + "'/>" + empenho;
		colLiquidado.innerHTML 	  = "<input type='hidden' name='eocvlrliquidado[]' id='eocvlrliquidado[]' value='" + tsLiquidado + "'/>" + liquidado;

		if( Number(total) == 0 || Number(total) == "" ) {

			colPerEmpenhado.innerHTML = "";
			colPerLiquidado.innerHTML = "";
			
		} else {

			porcempenhado = ( Number(tsEmpenho) / Number(tsTotal) * 100 );
			porcempenhado = porcempenhado.toFixed(2);
			colPerEmpenhado.innerHTML = porcempenhado.replace(".",",") + ' %';
		
			porcliquidado = ( Number(tsLiquidado) / Number(tsTotal) * 100 );
			porcliquidado = porcliquidado.toFixed(2);
			colPerLiquidado.innerHTML = porcliquidado.replace(".",",") + ' %';
			
		}

		colAcao.innerHTML = "<img src='../imagens/excluir.gif' style='cursor:pointer;' onclick='excluirItemExec(this.parentNode.parentNode.rowIndex);'/>";

		// total empenhado
		var vlrTotAtualEmpenho = document.getElementById( "totalEmpenhado" ).innerHTML;

		vlrTotAtualEmpenho = vlrTotAtualEmpenho.replace(".","");
		vlrTotAtualEmpenho = vlrTotAtualEmpenho.replace(".","");
		vlrTotAtualEmpenho = vlrTotAtualEmpenho.replace(".","");
		vlrTotAtualEmpenho = vlrTotAtualEmpenho.replace(",",".");

		vlrTotNovoEmpenho = Number(vlrTotAtualEmpenho) +  Number(tsEmpenho);
		vlrTotNovoEmpenho = vlrTotNovoEmpenho.toFixed(2);
		vlrTotNovoEmpenho = mascaraglobal('###.###.###.###,##', vlrTotNovoEmpenho);

		document.getElementById( "totalEmpenhado" ).innerHTML = vlrTotNovoEmpenho;

		// total liquidado
		var vlrTotAtualLiquidado = document.getElementById( "totalLiquidado" ).innerHTML;

		vlrTotAtualLiquidado = vlrTotAtualLiquidado.replace(".","");
		vlrTotAtualLiquidado = vlrTotAtualLiquidado.replace(".","");
		vlrTotAtualLiquidado = vlrTotAtualLiquidado.replace(".","");
		vlrTotAtualLiquidado = vlrTotAtualLiquidado.replace(",",".");

		vlrTotNovoLiquidado = Number(vlrTotAtualLiquidado) +  Number(tsLiquidado);
		vlrTotNovoLiquidado = vlrTotNovoLiquidado.toFixed(2);
		vlrTotNovoLiquidado = mascaraglobal('###.###.###.###,##', vlrTotNovoLiquidado);

		document.getElementById( "totalLiquidado" ).innerHTML = vlrTotNovoLiquidado;

		// total % empenhado
		var vlrTotAtualPerEmpenho = document.getElementById( "totalPerEmpenhado" ).innerHTML;

		vlrTotAtualPerEmpenho = vlrTotAtualPerEmpenho.replace(".","");
		vlrTotAtualPerEmpenho = vlrTotAtualPerEmpenho.replace(".","");
		vlrTotAtualPerEmpenho = vlrTotAtualPerEmpenho.replace(".","");
		vlrTotAtualPerEmpenho = vlrTotAtualPerEmpenho.replace(",",".");

		vlrTotNovoPerEmpenho = Number(vlrTotAtualPerEmpenho) +  Number(porcempenhado);
		vlrTotNovoPerEmpenho = vlrTotNovoPerEmpenho.toFixed(2);
		vlrTotNovoPerEmpenho = mascaraglobal('###,##', vlrTotNovoPerEmpenho);

		document.getElementById( "totalPerEmpenhado" ).innerHTML = vlrTotNovoPerEmpenho;

		// total % liquidado
		var vlrTotAtualPerLiquidado = document.getElementById( "totalPerLiquidado" ).innerHTML;

		vlrTotAtualPerLiquidado = vlrTotAtualPerLiquidado.replace(".","");
		vlrTotAtualPerLiquidado = vlrTotAtualPerLiquidado.replace(".","");
		vlrTotAtualPerLiquidado = vlrTotAtualPerLiquidado.replace(".","");
		vlrTotAtualPerLiquidado = vlrTotAtualPerLiquidado.replace(",",".");

		vlrTotNovoPerLiquidado = Number(vlrTotAtualPerLiquidado) +  Number(porcliquidado);
		vlrTotNovoPerLiquidado = vlrTotNovoPerLiquidado.toFixed(2);
		vlrTotNovoPerLiquidado = mascaraglobal('###,##', vlrTotNovoPerLiquidado);

		document.getElementById( "totalPerLiquidado" ).innerHTML = vlrTotNovoPerLiquidado; 

		document.getElementById( "insdtposicao" ).value  	= "";
		document.getElementById( "insvlrempenhado" ).value  = "";
		document.getElementById( "insvlrliquidado" ).value  = "";
		
		
	}

	function excluirItemExec( linha ) {

		if( confirm('Deseja realmente excluir este detalhamento?') ) {
			var tabela = document.getElementById( "tabelaDetalhamento" );
			tabela.deleteRow(linha);
		}
	}

	function registraExecOrc(){
		document.getElementById( "formulario" ).submit();
	}
	
</script>

<style>
	#divDetalhamento table {
		width: 100%;                
	}
</style>
<form name="formulario" method="post" id="formulario" action="<?php echo $caminho_atual; ?>acao=A">
	<input type="hidden" name="requisicao" value="salvar"/>
	<input type="hidden" name="eorid" value="<?php echo $dadosExecOrc["eorid"]; ?>"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="subtitulocentro" colspan="2">Orçamento para a Obra</td>
		</tr>
		<tr>
			<td class="subtitulodireita" width="190px">Capital (R$):</td>
			<td>
				<?php
					$eocvlrcapital = number_format($dadosExecOrc["eocvlrcapital"], 2, ",", ".");
					echo campo_texto( 'eocvlrcapital', 'N', $somenteLeitura, '', 17, 15, '###.###.###,##', '', 'left', '', 0, 'id="eocvlrcapital"', '', null, 'somaOrcObra();'); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Custeio (R$):</td>
			<td>
				<?php
					$eocvlrcusteio = number_format($dadosExecOrc["eocvlrcusteio"], 2, ",", ".");
					echo campo_texto( 'eocvlrcusteio', 'N', $somenteLeitura, '', 17, 15, '###.###.###,##', '', 'left', '', 0, 'id="eocvlrcusteio"', '', null, 'somaOrcObra();'); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Total (R$):</td>
			<td>
				<?php
					$nEocvlrtotal = $dadosExecOrc["eocvlrcapital"] + $dadosExecOrc["eocvlrcusteio"];
					$eocvlrtotal  = number_format($nEocvlrtotal, 2, ",", "."); 
					echo campo_texto( 'eocvlrtotal', 'N', 'N', '', 17, 15, '', '', 'left', '', 0, 'id="eocvlrtotal"',''); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulocentro" colspan="2">Detalhamento Orçamentário</td>
		</tr>
		<tr>
			<td colspan="2">
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr>
						<td class="subtitulocentro">Data</td>
						<td class="subtitulocentro">Valor Empenhado (R$)</td>
						<td class="subtitulocentro">Valor Liquidado (R$)</td>
						<td class="subtitulocentro">% Empenhado</td>
						<td class="subtitulocentro">% Liquidado</td>
						<td class="subtitulocentro">Ação</td>
						<td class="subtitulocentro"></td>
					</tr>
					<tr align="center" bgcolor="#e0e0e0">
						<td width="15%">
							<?php echo campo_data2( 'insdtposicao', 'N', $somenteLeitura, '', 'S' ); ?>
						</td>
						<td width="20%">
							<?php echo campo_texto( 'insvlrempenhado', 'N', $somenteLeitura, '', 17, 15, '###.###.###,##', '', 'left', '', 0, 'id="insvlrempenhado"', '', null, ''); ?>
						</td>
						<td width="20%">
							<?php echo campo_texto( 'insvlrliquidado', 'N', $somenteLeitura, '', 17, 15, '###.###.###,##', '', 'left', '', 0, 'id="insvlrliquidado"', '', null, ''); ?>
						</td>
						<td width="15%"></td>
						<td width="15%"></td>
						<td width="14%">
						<?php if($habilitado){?>						
							<img src="../imagens/gif_inclui.gif" style="cursor: pointer;" align="absmidle" onclick="incluiDetalhamento();"/>
						<?php }else{?>	
							<img src="../imagens/gif_inclui_d.gif" style="cursor: pointer;" align="absmidle" title="Detalhamento não pode ser inserido!"/>
						<?php }?>	
						</td>
						<td width="1%"></td>
					</tr>
				</table>
				<center>
					<div class="div_rolagem" style="width: 95%; height: 150px;" id="divDetalhamento">
						<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" id="tabelaDetalhamento">
							<?php $dadosDetOrc = $execOrc->buscaDetalheExecOrcamentaria( $dadosExecOrc["eorid"], $nEocvlrtotal );?>
						</table>
					</div>
				</center>
				
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" id="tabelaTotal">
					<tr>
						<td class="subtitulodireita" width="15%" style="font-weight: bold;">Total</td>
						<td class="subtitulodireita" width="20%" style="font-weight: bold;">
							<div id="totalEmpenhado">
								<?php echo number_format($dadosDetOrc["totalempenho"], 2, ",", "." );?>
							</div>
						</td>
						<td class="subtitulodireita" width="20%" style="font-weight: bold;">
							<div id="totalLiquidado">
								<?php echo number_format($dadosDetOrc["totalliquidado"], 2, ",", "." );?>
							</div>
						</td>
						<td class="subtitulodireita" width="15%" style="font-weight: bold;">
							<div id="totalPerEmpenhado">
								<?php echo number_format($dadosDetOrc["totalpercempenhado"], 2, ",", "." );?>
							</div>
						</td>
						<td class="subtitulodireita" width="15%" style="font-weight: bold;">
							<div id="totalPerLiquidado">
								<?php echo number_format($dadosDetOrc["totalpercliquidado"], 2, ",", "." );?>
							</div>
						</td>
						<td class="subtitulodireita" width="14%"></td>
						<td class="subtitulodireita" width="1%"></td>
					</tr>
				</table>
			</td>
		</tr>
		<tr bgcolor="#c0c0c0">
			<td colspan="2">
			<?php if($habilitado){?>
				<input type="button" value="Salvar" onclick="registraExecOrc();" style="cursor:pointer;"/>
				<?php }?>
				<input type="button" value="Voltar" onclick="history.back(-1);" style="cursor:pointer;"/>
			</td>
		</tr>
	</table>
</form>
<?php chkSituacaoObra(); ?>