<?

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

set_time_limit(0);
ini_set("memory_limit","256M");

if($_REQUEST["submetido"]) {
	include 'resultado_apuracao_valores_financeiros.inc';
	exit;
}

include APPRAIZ . 'includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
monta_titulo('Relatório Módulo Financeiro', 'Apuração de Valores Financeiros a Repassar');
?>

<form method="post" name="formulario" id="formulario">
<input type="hidden" name="submetido" value="1" />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tbody>
	<tr>
		<td class="SubTituloDireita">Mês de Referência:</td>
		<td width="75%" colspan="2">
			<?
				$sql = "SELECT trim(mescod) as codigo, trim(mesdsc) as descricao FROM public.meses ORDER BY mescod";
				echo $db->monta_combo('mesReferencia',$sql,'S','Selecione...','','','','','S','mesReferencia',true);
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Unidade Orçamentária:</td>
		<td>
			<?
				/*** Adiciona ao combo popup as unidades que já devem vir selecionadas ***/
				$sql_carregados = "SELECT trim(gr_unidade_orcamentaria) as codigo, trim(it_no_unidade_orcamentaria) as descricao FROM siafi.uo WHERE gr_unidade_orcamentaria BETWEEN '26101' and '26907'";
				$unidadeOrcamentaria = $db->carregar( $sql_carregados );
				
				/*** Monta combo popup ***/
				$sql_combo = "SELECT trim(gr_unidade_orcamentaria) as codigo, trim(it_no_unidade_orcamentaria) as descricao FROM siafi.uo";
				combo_popup( 'unidadeOrcamentaria', $sql_combo, 'Selecione a(s) Unidade(s) Orçamentária(s)', '400x400', 0, array(), '', 'S', true, true );
				
				
				/*** Forma antiga de representar o filtro ***/ 
				/*$matrizOrigem = $db->carregar("SELECT trim(gr_unidade_orcamentaria) as codigo, trim(it_no_unidade_orcamentaria) as descricao FROM siafi.uo ORDER BY descricao");
				$matrizDestino = $db->carregar("SELECT trim(gr_unidade_orcamentaria) as codigo, trim(it_no_unidade_orcamentaria) as descricao FROM siafi.uo WHERE gr_unidade_orcamentaria BETWEEN '26101' and '26907' ORDER BY descricao");
				
				$agrupador = new Agrupador('formulario');
				$agrupador->setOrigem('agrupadorOrigemUnidadeOrcamentaria', null, $matrizOrigem);
				$agrupador->setDestino('unidadeOrcamentaria', null, $matrizDestino);
				$agrupador->exibir();*/
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Grupo de Despesa:</td>
		<td>
			<?
				/*** Adiciona ao combo popup os grupos que já devem vir selecionados ***/
				$sql_carregados = "SELECT gndcod as codigo, trim(gnddsc) as descricao FROM siafi.gruponaturezadespesa WHERE gndcod in (4,3)";
				$grupoDespesa = $db->carregar( $sql_carregados );
				
				/*** Monta combo popup ***/
				$sql_combo = "SELECT gndcod as codigo, trim(gnddsc) as descricao FROM siafi.gruponaturezadespesa";
				combo_popup( 'grupoDespesa', $sql_combo, 'Selecione o(s) Grupo(s) de Despesa', '400x400', 0, array(), '', 'S', true, true );
				
				
				/*** Forma antiga de representar o filtro ***/
				/*$matrizOrigem = array(array('codigo' => 6, 'descricao' => 'Amortização da Dívida'),
									  array('codigo' => 5, 'descricao' => 'Inversões Financeiras'),
									  array('codigo' => 4, 'descricao' => 'Investimentos'),
									  array('codigo' => 2, 'descricao' => 'Juros e Encargos da Dívida'),
									  array('codigo' => 3, 'descricao' => 'Outras Despesas Correntes'),
									  array('codigo' => 1, 'descricao' => 'Pessoal e Encargos Sociais'),
									  array('codigo' => 9, 'descricao' => 'Reserva de Contingência'));
	
				$matrizDestino = array(array('codigo' => 4, 'descricao' => 'Investimentos'),
									   array('codigo' => 3, 'descricao' => 'Outras Despesas Correntes'));
									   
				$agrupador = new Agrupador('formulario');
				$agrupador->setOrigem('agrupadorOrigemGrupoDespesa', null, $matrizOrigem);
				$agrupador->setDestino('grupoDespesa', null, $matrizDestino);
				$agrupador->exibir();*/
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Fonte de Recursos:</td>
		<td>
			<?
				/*** Adiciona ao combo popup as fontes que já devem vir selecionadas ***/
				$sql_carregados = "SELECT trim(foncod) as codigo, trim(fondsc) as descricao FROM public.fonterecurso WHERE foncod in ('100','112')";
				$fonteRecurso = $db->carregar( $sql_carregados );
				
				/*** Monta combo popup ***/
				$sql_combo = "SELECT trim(foncod) as codigo, trim(fondsc) as descricao FROM public.fonterecurso";
				combo_popup( 'fonteRecurso', $sql_combo, 'Selecione a(s) Fonte(s) de Recursos', '400x400', 0, array(), '', 'S', true, true );
				
				
				/*** Forma antiga de representar o filtro ***/
				/*$matrizOrigem = $db->carregar("SELECT trim(foncod) as codigo, trim(fondsc) as descricao FROM public.fonterecurso ORDER BY descricao");
				$matrizDestino = $db->carregar("SELECT trim(foncod) as codigo, trim(fondsc) as descricao FROM public.fonterecurso WHERE foncod in ('100','112') ORDER BY descricao");
				
				$agrupador = new Agrupador('formulario');
				$agrupador->setOrigem('agrupadorOrigemFonteRecurso', null, $matrizOrigem);
				$agrupador->setDestino('fonteRecurso', null, $matrizDestino);
				$agrupador->exibir();*/
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Projeto/Atividade:</td>
		<td>
			<?
				/*** Adiciona ao combo popup os projetos/atividades que já devem vir selecionados ***/
				$sql_carregados = "SELECT trim(gr_orgao_acao) as codigo, trim(it_no_acao) as descricao FROM siafi.acao WHERE gr_orgao_acao not in ('0304', '2010', '2011', '2012', '0005', '0369')";
				$projetoAtividade = $db->carregar( $sql_carregados );
				
				/*** Monta combo popup ***/
				$sql_combo = "SELECT trim(gr_orgao_acao) as codigo, trim(it_no_acao) as descricao FROM siafi.acao";
				combo_popup( 'projetoAtividade', $sql_combo, 'Selecione o(s) Projeto(s)/Atividade(s)', '400x400', 0, array(), '', 'S', true, true );
				
				
				/*** Forma antiga de representar o filtro ***/
				/*$matrizOrigem = $db->carregar("SELECT trim(gr_orgao_acao) as codigo, trim(it_no_acao) as descricao FROM siafi.acao ORDER BY descricao");
				$matrizDestino = $db->carregar("SELECT trim(gr_orgao_acao) as codigo, trim(it_no_acao) as descricao FROM siafi.acao WHERE gr_orgao_acao not in ('0304', '2010', '2011', '2012', '0005', '0369') ORDER BY descricao");
				
				$agrupador = new Agrupador('formulario');
				$agrupador->setOrigem('agrupadorOrigemProjetoAtividade', null, $matrizOrigem);
				$agrupador->setDestino('projetoAtividade', null, $matrizDestino);
				$agrupador->exibir();*/
			?>
		</td>
	</tr>
</tbody>
<tfoot>
	<tr bgcolor="#C0C0C0">
		<td colspan="3" align="center" valign="top">
			<input type="button" class="botao" id="bt_gerar_relatorio" value="Gerar Relatório" title="Clique para visualizar o relatório em HTML" onclick="abreRelatorio();" />
		</td>
	</tr>
</tfoot>
</table>
</form>
<script type="text/javascript">
function abreRelatorio() {
	if(document.getElementById("mesReferencia").value == "") {
		alert("O 'Mês de Referência' deve ser selecionado.");
		document.getElementById("mesReferencia").focus();
	}
	else {
		var formulario = document.getElementById("formulario");
		
		selectAllOptions(formulario.unidadeOrcamentaria);
		selectAllOptions(formulario.grupoDespesa);
		selectAllOptions(formulario.fonteRecurso);
		selectAllOptions(formulario.projetoAtividade);
		
		formulario.action = 'financeiro.php?modulo=relatorio/rel_apuracao_valores_financeiros&acao=A';
		window.open('', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
		formulario.target = 'relatorio';
		
		formulario.submit();
	}
}
</script>