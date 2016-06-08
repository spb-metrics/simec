<?

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

set_time_limit(0);
ini_set("memory_limit","256M");

if($_REQUEST["submetido"]) {
	include 'resultado_rap_outros_custeios.inc';
	exit;
}

include APPRAIZ . 'includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
monta_titulo('Relatório Módulo Financeiro', 'RAP Outros Custeios e Capital (OCC)');
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
		<td class="SubTituloDireita">UG Executora:</td>
		<td>
			<?
				/*** Adiciona ao combo popup as UG's que já devem vir selecionadas ***/
				$sql_carregados = "SELECT trim(it_co_unidade_gestora) as codigo, trim(it_no_unidade_gestora) as descricao FROM siafi.ug WHERE it_co_unidade_gestora in ('153004', '153006', '153008', '153009', '153011', '153012', '153014', '153016', '153017', '153018', '153020', '153021', '153022', '153174', '153024', '153025', '153026', '153027', '153235', '158191', '153010', '153015', '153013', '153230', '153195', '153217', '153200', '153203', '153222', '153223', '153224', '153208', '153213', '153226', '154421', '153037', '153038', '153045', '153046', '153052', '153056', '153061', '153062', '153063', '153065', '153079', '153080', '153103', '153114', '153115', '153163', '153164', '153165', '153079', '153080', '153103', '153114', '153115', '153163', '153164', '153165', '153166', '154080', '154419', '158195', '153034', '153035', '153036', '153019', '153028', '153030', '153031', '153032', '153033', '154055', '154034', '154039', '154040', '154041', '154042', '154043', '154044', '154045', '154046', '154047', '154048', '154049', '154050', '154051', '154054', '154032', '154069', '154215', '154502', '158092', '154503', '153231', '153193', '153194', '153196', '153218', '153197', '153219', '153232', '153220', '153221', '153198', '153199', '153201', '153202', '153204', '153192', '153205', '153206', '153233', '153216', '153207', '153225', '153211', '153210', '153212', '153214', '153215', '153227', '153228', '153234', '153229', '153239', '153237', '153238', '153240', '153241', '153167', '153978', '154003', '344002', '155001', '153173', '152004', '152005')";
				$ugExecutora = $db->carregar( $sql_carregados );
				
				/*** Monta combo popup ***/
				// O combo não está sendo preenchido com este SQL. Tive que colocar um LIMIT pra dar certo. Ver melhor solução depois...
				$sql_combo = "SELECT trim(it_co_unidade_gestora) as codigo, trim(it_no_unidade_gestora) as descricao FROM siafi.ug LIMIT 500";
				combo_popup( 'ugExecutora', $sql_combo, 'Selecione a(s) UG(s) Executora(s)', '400x400', 0, array(), '', 'S', true, true );
				
				
				/*** Forma antiga de representar o filtro ***/
				/*$matrizOrigem = $db->carregar("SELECT trim(it_co_unidade_gestora) as codigo, trim(it_no_unidade_gestora) as descricao FROM siafi.ug ORDER BY descricao LIMIT 1000");
				$matrizDestino = $db->carregar("SELECT trim(it_co_unidade_gestora) as codigo, trim(it_no_unidade_gestora) as descricao FROM siafi.ug WHERE it_co_unidade_gestora in ('153004', '153006', '153008', '153009', '153011', '153012', '153014', '153016', '153017', '153018', '153020', '153021', '153022', '153174', '153024', '153025', '153026', '153027', '153235', '158191', '153010', '153015', '153013', '153230', '153195', '153217', '153200', '153203', '153222', '153223', '153224', '153208', '153213', '153226', '154421', '153037', '153038', '153045', '153046', '153052', '153056', '153061', '153062', '153063', '153065', '153079', '153080', '153103', '153114', '153115', '153163', '153164', '153165', '153079', '153080', '153103', '153114', '153115', '153163', '153164', '153165', '153166', '154080', '154419', '158195', '153034', '153035', '153036', '153019', '153028', '153030', '153031', '153032', '153033', '154055', '154034', '154039', '154040', '154041', '154042', '154043', '154044', '154045', '154046', '154047', '154048', '154049', '154050', '154051', '154054', '154032', '154069', '154215', '154502', '158092', '154503', '153231', '153193', '153194', '153196', '153218', '153197', '153219', '153232', '153220', '153221', '153198', '153199', '153201', '153202', '153204', '153192', '153205', '153206', '153233', '153216', '153207', '153225', '153211', '153210', '153212', '153214', '153215', '153227', '153228', '153234', '153229', '153239', '153237', '153238', '153240', '153241', '153167', '153978', '154003', '344002', '155001', '153173', '152004', '152005') ORDER BY descricao");
				
				$agrupador = new Agrupador('formulario');
				$agrupador->setOrigem('agrupadorOrigemUgExecutora', null, $matrizOrigem);
				$agrupador->setDestino('ugExecutora', null, $matrizDestino);
				$agrupador->exibir();*/
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Órgão da UG Executora:</td>
		<td>
			<?
				/*** Adiciona ao combo popup os órgãos que já devem vir selecionados ***/
				//$sql_carregados = "";
				//$orgaoUgExecutora = $db->carregar( $sql_carregados );
				
				/*** Monta combo popup ***/
				$sql_combo = "SELECT trim(gr_orgao) as codigo, trim(it_no_orgao) as descricao FROM siafi.orgao";
				combo_popup( 'orgaoUgExecutora', $sql_combo, 'Selecione o(s) Órgão(s) da(s) UG(s) Executora(s)', '400x400', 0, array(), '', 'S', true, true );
				
				
				/*** Forma antiga de representar o filtro ***/
				/*$matrizOrigem = $db->carregar("SELECT trim(gr_orgao) as codigo, trim(it_no_orgao) as descricao FROM siafi.orgao ORDER BY descricao");
				
				$agrupador = new Agrupador('formulario');
				$agrupador->setOrigem('agrupadorOrigemOrgaoUgExecutora', null, $matrizOrigem);
				$agrupador->setDestino('orgaoUgExecutora', null);
				$agrupador->exibir();*/
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Vinculação de Pagamento:</td>
		<td>
			<?
				/*** Adiciona ao combo popup as vinculações que já devem vir selecionadas ***/
				$sql_carregados = "SELECT trim(gr_orgao) as codigo, trim(it_no_orgao) as descricao FROM siafi.orgao WHERE gr_orgao in ('400','412','500')";
				$vinculacaoPagamento = $db->carregar( $sql_carregados );
				
				/*** Monta combo popup ***/
				$sql_combo = "SELECT trim(it_co_vinculacao_pagamento) as codigo, trim(it_no_vinculacao_pagamento) as descricao FROM siafi.vinculapagamento";
				combo_popup( 'vinculacaoPagamento', $sql_combo, 'Selecione a(s) Vinculação(ões) de Pagamento', '400x400', 0, array(), '', 'S', true, true );
				
				
				/*** Forma antiga de representar o filtro ***/
				/*$matrizOrigem = $db->carregar("SELECT trim(it_co_vinculacao_pagamento) as codigo, trim(it_no_vinculacao_pagamento) as descricao FROM siafi.vinculapagamento ORDER BY descricao");
				$matrizDestino = $db->carregar("SELECT trim(gr_orgao) as codigo, trim(it_no_orgao) as descricao FROM siafi.orgao WHERE gr_orgao in ('400','412','500') ORDER BY descricao");
				
				$agrupador = new Agrupador('formulario');
				$agrupador->setOrigem('agrupadorOrigemVinculacaoPagamento', null, $matrizOrigem);
				$agrupador->setDestino('vinculacaoPagamento', null, $matrizDestino);
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
	<tr>
		<td class="SubTituloDireita">Recurso:</td>
		<td>
			<?
				/*** Adiciona ao combo popup os recursos que já devem vir selecionados ***/
				//$sql_carregados = "";
				//$recurso = $db->carregar( $sql_carregados );
				
				/*** Monta combo popup ***/
				//$sql_combo = "";
				//combo_popup( 'recurso', $sql_combo, 'Selecione o(s) Recurso(s)', '400x400', 0, array(), '', 'S', true, true );
				
				
				/*** Forma antiga de representar o filtro ***/
				/*$agrupador = new Agrupador('formulario');
				$agrupador->setOrigem('agrupadorOrigemRecurso', null);
				$agrupador->setDestino('recurso', null);
				$agrupador->exibir();*/
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Categoria de Gasto:</td>
		<td>
			<?
				/*** Adiciona ao combo popup as categorias que já devem vir selecionadas ***/
				$sql_carregados = "SELECT trim(gr_categoria_gasto) as codigo, trim(it_no_categoria_gasto) as descricao FROM siafi.categoriagasto WHERE gr_categoria_gasto in ('C','D')";
				$categoriaGasto = $db->carregar( $sql_carregados );
				
				/*** Monta combo popup ***/
				$sql_combo = "SELECT trim(gr_categoria_gasto) as codigo, trim(it_no_categoria_gasto) as descricao FROM siafi.categoriagasto";
				combo_popup( 'categoriaGasto', $sql_combo, 'Selecione a(s) Categoria(s) de Gasto', '400x400', 0, array(), '', 'S', true, true );
				
				
				/*** Forma antiga de representar o filtro ***/
				/*$matrizOrigem = $db->carregar("SELECT trim(gr_categoria_gasto) as codigo, trim(it_no_categoria_gasto) as descricao FROM siafi.categoriagasto ORDER BY descricao");
				$matrizDestino = $db->carregar("SELECT trim(gr_categoria_gasto) as codigo, trim(it_no_categoria_gasto) as descricao FROM siafi.categoriagasto WHERE gr_categoria_gasto in ('C','D') ORDER BY descricao");
				
				$agrupador = new Agrupador('formulario');
				$agrupador->setOrigem('agrupadorOrigemCategoriaGasto', null, $matrizOrigem);
				$agrupador->setDestino('categoriaGasto', null, $matrizDestino);
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
		
		selectAllOptions(formulario.fonteRecurso);
		selectAllOptions(formulario.unidadeOrcamentaria);
		selectAllOptions(formulario.ugExecutora);
		selectAllOptions(formulario.orgaoUgExecutora);
		selectAllOptions(formulario.vinculacaoPagamento);
		selectAllOptions(formulario.grupoDespesa);
		selectAllOptions(formulario.projetoAtividade);
		//selectAllOptions(formulario.recurso);
		selectAllOptions(formulario.categoriaGasto);
		
		formulario.action = 'financeiro.php?modulo=relatorio/rel_rap_outros_custeios&acao=A';
		window.open('', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
		formulario.target = 'relatorio';
		
		formulario.submit();
	}
}
</script>