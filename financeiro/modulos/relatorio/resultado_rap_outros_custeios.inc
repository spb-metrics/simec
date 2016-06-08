<?

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

set_time_limit(0);
ini_set("memory_limit","256M");


/*** Mês de Referência ***/
$mesRef = $_REQUEST["mesReferencia"];


/*** Fonte de Recursos ***/
$inFonteRecursos = "";


/*** Unidade Orçamentária ***/
if($_REQUEST["unidadeOrcamentaria"] != null) {
	if($_REQUEST["unidadeOrcamentaria_campo_excludente"] != "1")
		$inUnidadeOrcamentaria = "and unicod in ('" . implode('\',\'',$_REQUEST["unidadeOrcamentaria"]) . "')";
	else
		$inUnidadeOrcamentaria = "and unicod not in ('" . implode('\',\'',$_REQUEST["unidadeOrcamentaria"]) . "')";
} 
else {
	$inUnidadeOrcamentaria = "";
}


/*** UG Executora ***/
if($_REQUEST["ugExecutora"] != null) {
	if($_REQUEST["ugExecutora_campo_excludente"] != "1")
		$inUgExecutora = "and ungcod in ('" . implode('\',\'',$_REQUEST["ugExecutora"]) . "')";
	else
		$inUgExecutora = "and ungcod not in ('" . implode('\',\'',$_REQUEST["ugExecutora"]) . "')";
} 
else {
	$inUgExecutora = "";
}


/*** Órgão da UG Executora ***/
if($_REQUEST["orgaoUgExecutora"] != null) {
	if($_REQUEST["orgaoUgExecutora_campo_excludente"] != "1")
		$inOrgaoUgExecutora = "and orgcod in ('" . implode('\',\'',$_REQUEST["orgaoUgExecutora"]) . "')";
	else
		$inOrgaoUgExecutora = "and orgcod not in ('" . implode('\',\'',$_REQUEST["orgaoUgExecutora"]) . "')";
} 
else {
	$inOrgaoUgExecutora = "";
}


/*** Vinculação de Pagamento ***/
if($_REQUEST["vinculacaoPagamento"] != null) {
	if($_REQUEST["vinculacaoPagamento_campo_excludente"] != "1")
		$inVinculacaoPagamento = "and vincod in ('" . implode('\',\'',$_REQUEST["vinculacaoPagamento"]) . "')";
	else
		$inVinculacaoPagamento = "and vincod not in ('" . implode('\',\'',$_REQUEST["vinculacaoPagamento"]) . "')";
} 
else {
	$inVinculacaoPagamento = "";
}


/*** Grupo de Despesa ***/
if($_REQUEST["grupoDespesa"] != null) {
	if($_REQUEST["grupoDespesa_campo_excludente"] != "1")
		$inGrupoDespesa = "and gndcod in ('" . implode('\',\'',$_REQUEST["grupoDespesa"]) . "')";
	else
		$inGrupoDespesa = "and gndcod not in ('" . implode('\',\'',$_REQUEST["grupoDespesa"]) . "')";
} 
else {
	$inGrupoDespesa = "";
}


/*** Projeto/Atividade ***/
if($_REQUEST["projetoAtividade"] != null) {
	if($_REQUEST["projetoAtividade_campo_excludente"] != "1")
		$inProjetoAtividade = "and acacod in ('" . implode('\',\'',$_REQUEST["projetoAtividade"]) . "')";
	else
		$inProjetoAtividade = "and acacod not in ('" . implode('\',\'',$_REQUEST["projetoAtividade"]) . "')";
} 
else {
	$inProjetoAtividade = "";
}


/*** Recurso ***/
$inRecurso = "";


/*** Categoria de Gasto ***/
if($_REQUEST["categoriaGasto"] != null) {
	if($_REQUEST["categoriaGasto_campo_excludente"] != "1")
		$inCategoriaGasto = "and cagcod in ('" . implode('\',\'',$_REQUEST["categoriaGasto"]) . "')";
	else
		$inCategoriaGasto = "and cagcod not in ('" . implode('\',\'',$_REQUEST["categoriaGasto"]) . "')";
} 
else {
	$inCategoriaGasto = "";
}


/**
** RAP OUTROS CUSTEIOS E CAPITAL(OCC) 
**/
$sql = "SELECT
			uo.gr_unidade_orcamentaria
	--RAP INSCRITO PROC. E NÃO PROCESSADO
	(
		select	sum(sldvalor) as valor
		from 	siafi.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('195100000', '195400000', '292410510', '292410508')
		---------------------------------------------------------
		and 	si.foncod like '0112%'
		and 	si.gndcod = '3'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna1,
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('195100000', '195400000', '292410510', '292410508')
		---------------------------------------------------------
		and 	si.foncod like '0112%'
		and 	si.gndcod = '4'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna2,
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('195100000', '195400000', '292410510', '292410508')
		---------------------------------------------------------
		and 	si.foncod like '0112%'
		and 	si.gndcod = '3'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna3,
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('195100000', '195400000', '292410510', '292410508')		
		---------------------------------------------------------
		and 	si.foncod like '0112%'
		and 	si.gndcod = '4'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna4,
	--SALDO   LIMITE SAQUE
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('112160400')
		---------------------------------------------------------
		and 	si.foncod = '0100000000'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna5,
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('112160400')
		---------------------------------------------------------
		and 	si.foncod = '0112000000'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna6,
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('112160400')
		---------------------------------------------------------
		and 	si.foncod in ('0100000000', '0112000000')
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna7,
	--FINANCEIRO  RAP  LIBERADO EM  2008			
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('293110307', '293110308', '293110316', '293110306')
		---------------------------------------------------------		
		and 	si.foncod like '0100%'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna8,
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('293110307', '293110308', '293110316', '293110306')
		---------------------------------------------------------
		and 	si.foncod like '0100%'
		and 	si.cagcod = 'D'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna9,
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('293110307', '293110308', '293110316', '293110306')
		---------------------------------------------------------
		and 	si.foncod like '0112%'
		and 	si.cagcod = 'C'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna10,
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('293110307', '293110308', '293110316', '293110306')
		---------------------------------------------------------
		and 	si.foncod like '0112%'
		and 	si.cagcod = 'D'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna11,
	--TOTAL  LIMITE SAQUE  MÊS 0 + FINANCEIRO RAP  LIBERADO 2008			
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('112160400')
		---------------------------------------------------------
		and 	si.foncod = '0100000000'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)+(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('293110307', '293110308', '293110316', '293110306')		
		---------------------------------------------------------		
		and 	si.foncod like '0100%'
		and 	si.cagcod = 'C'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna12,
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('293110307', '293110308', '293110316', '293110306')		
		---------------------------------------------------------		
		and 	si.foncod like '0100%'
		and 	si.cagcod = 'D'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna13,
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('112160400')		
		---------------------------------------------------------
		and 	si.foncod = '0112000000'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)+(
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('293110307', '293110308', '293110316', '293110306')		
		---------------------------------------------------------		
		and 	si.foncod like '0112%'
		and 	si.cagcod = 'C'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna14,
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('293110307', '293110308', '293110316', '293110306')		
		---------------------------------------------------------		
		and 	si.foncod like '0112%'
		and 	si.cagcod = 'D'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna15,
	--RAP PROCESSADO PAGO			
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('292410510')		
		---------------------------------------------------------
		and 	si.foncod like '0100%'
		and	si.gndcod = '3'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna16,
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('292410510')		
		---------------------------------------------------------
		and 	si.foncod like '0100%'
		and	si.gndcod = '4'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna17,
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('292410510')		
		---------------------------------------------------------
		and 	si.foncod like '0112%'
		and	si.gndcod = '3'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna18,
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('292410510')		
		---------------------------------------------------------
		and 	si.foncod like '0112%'
		and	si.gndcod = '4'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna19,
	--RAP PROCESSADO PAGO			
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('295200000')		
		---------------------------------------------------------
		and 	si.foncod like '0100%'
		and	si.gndcod = '3'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna20,
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('295200000')		
		---------------------------------------------------------
		and 	si.foncod like '0100%'
		and	si.gndcod = '4'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna21,
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('295200000')		
		---------------------------------------------------------
		and 	si.foncod like '0112%'
		and	si.gndcod = '3'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna22,
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('295200000')		
		---------------------------------------------------------
		and 	si.foncod like '0112%'
		and	si.gndcod = '4'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna23,
	--TOTAL  DE RAP PAGO PROC. E NÃO PROCESSADO			
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('292410510', '295200000')		
		---------------------------------------------------------
		and 	si.foncod like '0100%'
		and	si.gndcod = '3'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna24,
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('292410510', '295200000')		
		---------------------------------------------------------
		and 	si.foncod like '0100%'
		and	si.gndcod = '4'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna25,
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('292410510', '295200000')		
		---------------------------------------------------------
		and 	si.foncod like '0112%'
		and	si.gndcod = '3'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna26,
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('292410510', '295200000')		
		---------------------------------------------------------
		and 	si.foncod like '0112%'
		and	si.gndcod = '4'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna27,
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('112160400')		
		---------------------------------------------------------
		and 	si.foncod = '0100000000'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)+(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('293110307', '293110308', '293110316', '293110306')		
		---------------------------------------------------------		
		and 	si.foncod like '0100%'
		and 	si.cagcod = 'C'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)+(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('292410510', '295200000')		
		---------------------------------------------------------
		and 	si.foncod like '0100%'
		and	si.gndcod = '3'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna28,
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('293110307', '293110308', '293110316', '293110306')		
		---------------------------------------------------------		
		and 	si.foncod like '0100%'
		and 	si.cagcod = 'D'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)+(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('292410510', '295200000')		
		---------------------------------------------------------
		and 	si.foncod like '0100%'
		and	si.gndcod = '4'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna29,
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('112160400')		
		---------------------------------------------------------
		and 	si.foncod = '0112000000'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)+(
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('293110307', '293110308', '293110316', '293110306')		
		---------------------------------------------------------		
		and 	si.foncod like '0112%'
		and 	si.cagcod = 'C'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)+(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('292410510', '295200000')		
		---------------------------------------------------------
		and 	si.foncod like '0112%'
		and	si.gndcod = '3'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna30,
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('293110307', '293110308', '293110316', '293110306')		
		---------------------------------------------------------		
		and 	si.foncod like '0112%'
		and 	si.cagcod = 'D'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)+(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('292410510', '295200000')		
		---------------------------------------------------------
		and 	si.foncod like '0112%'
		and	si.gndcod = '4'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as aoluna31,
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('195100000', '195400000', '292410510', '292410508')		
		---------------------------------------------------------
		and 	si.foncod like '0112%'
		and 	si.gndcod = '3'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)+(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('112160400')		
		---------------------------------------------------------
		and 	si.foncod = '0100000000'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)+(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('293110307', '293110308', '293110316', '293110306')		
		---------------------------------------------------------		
		and 	si.foncod like '0100%'
		and 	si.cagcod = 'C'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna32,
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('195100000', '195400000', '292410510', '292410508')		
		---------------------------------------------------------
		and 	si.foncod like '0112%'
		and 	si.gndcod = '4'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)+(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('293110307', '293110308', '293110316', '293110306')		
		---------------------------------------------------------		
		and 	si.foncod like '0100%'
		and 	si.cagcod = 'D'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."', '500')		
		---------------------------------------------------------
	)as coluna33,
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('195100000', '195400000', '292410510', '292410508')		
		---------------------------------------------------------
		and 	si.foncod like '0112%'
		and 	si.gndcod = '3'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)+(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('112160400')		
		---------------------------------------------------------
		and 	si.foncod = '0112000000'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)+(
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('293110307', '293110308', '293110316', '293110306')		
		---------------------------------------------------------		
		and 	si.foncod like '0112%'
		and 	si.cagcod = 'C'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna34,
	(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('195100000', '195400000', '292410510', '292410508')		
		---------------------------------------------------------
		and 	si.foncod like '0112%'
		and 	si.gndcod = '4'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)+(
		select	sum(sldvalor) as valor
		from 	dw.saldo si
		where 	si.unicod = uo.gr_unidade_orcamentaria
		---------------------------------------------------------
		and 	si.sldcontacontabil in ('293110307', '293110308', '293110316', '293110306')		
		---------------------------------------------------------		
		and 	si.foncod like '0112%'
		and 	si.cagcod = 'D'
		".$inUnidadeOrcamentaria."
		".$inUgExecutora."
		".$inOrgaoUgExecutora."
		".$inVinculacaoPagamento."
		".$inGrupoDespesa."
		".$inProjetoAtividade."
		".$inCategoriaGasto."
		---------------------------------------------------------
	)as coluna35

from 	siafi.uo uo
where	uo.gr_unidade_orcamentaria between '26101' and '26907'
order by uo.gr_unidade_orcamentaria";

//dbg($sql,1);
//$result = $db->carregar($sql);

$result = array(
			array(
				"gr_unidade_orcamentaria" => "Testandoo",
				"coluna1" => "1111",
				"coluna2" => "2222",
				"coluna3" => "3333",
				"coluna4" => "4444",
			 	"coluna5" => "5555",
			 	"coluna6" => "6666",
				"coluna7" => "7777",
			 	"coluna8" => "8888",
				"coluna9" => "9999",
				"coluna10" => "1010",
				"coluna11" => "1111",
				"coluna12" => "1212",
				"coluna13" => "1313",
				"coluna14" => "1414",
				"coluna15" => "1515",
				"coluna16" => "1616",
				"coluna17" => "1717",
				"coluna18" => "1818"
			),
			array("coluna1" => "1111"),
			array("coluna2" => "2222"),
			array("coluna3" => "3333"),
			array("coluna4" => "4444"),
			array("coluna5" => "5555"),
			array("coluna6" => "6666"),
			array("coluna7" => "7777"),
			array("coluna8" => "8888"),
			array("coluna9" => "9999"),
			array("coluna10" => "1010"),
			array("coluna11" => "1111"),
			array("coluna12" => "1212"),
			array("coluna13" => "1313"),
			array("coluna14" => "1414"),
			array("coluna15" => "1515"),
			array("coluna16" => "1616"),
			array("coluna17" => "1717"),
			array("coluna18" => "1818")
			);
//dbg($result,1);
?>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Expires" content="-1">
		<title>RAP Outros Custeios e Capital (OCC)</title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
		<div id="aguarde" style="background-color:#ffffff;position:absolute;color:#000033;top:50%;left:30%;border:2px solid #cccccc; width:300px;">
			<center style="font-size:12px;"><br><img src="../imagens/wait.gif" border="0" align="absmiddle"> Aguarde! Gerando Relatório...<br><br></center>
		</div>
		<table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug"  style="border-bottom: 1px solid;">
			<tr bgcolor="#ffffff">
				<td valign="top" width="50" rowspan="2"><img src="../imagens/brasao.gif" width="45" height="45" border="0"></td>
				<td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
					SIMEC- Sistema Integrado do Ministério da Educação<br/>
					Acompanhamento da Execução Orçamentária<br/>
					MEC / SE - Secretaria Executiva <br />
					SPO - Subsecretaria de Planejamento e Orçamento
				</td>
				<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">
					Impresso por: <b><?= $_SESSION['usunome'] ?></b><br/>
					Hora da Impressão: <?= date( 'd/m/Y - H:i:s' ) ?><br />
					Orçamento Fiscal e Seg.Social - EM R$<br />
					Acumulado até:
				</td>
			</tr>
		</table>
		<br /><br />
		<center>ORÇAMENTO 2005/2006/2007/2008<br />RAP OUTROS CUSTEIOS E CAPITAL(OCC)</center>
		<br />
		<table class="tabela" border="1">
			<thead>
				<tr>
					<th rowspan="2">UNIDADE</th>
					<th colspan="4">RAP INSCRITO PROC. E NÃO PROCESSADO</th>
					<th colspan="4">SALDO LIMITE SAQUE<br />NO MÊS (0)</th>
					<th colspan="3">FINANCEIRO RAP<br />LIBERADO EM 2008</th>
					<th colspan="4">TOTAL LIMITE SAQUE MÊS 0 +<br />FINANCEIRO RAP LIBERADO 2008</th>
					<th colspan="4">RAP PROCESSADO PAGO</th>
					<th colspan="4">RAP NÃO PROCESSADO PAGO</th>
					<th colspan="4">TOTAL DE RAP PAGO PROC. E<br />NÃO PROCESSADO</th>
					<th colspan="4">SALDO FINANCEIRO<br />(TOTAL FINANCEIRO - RAP PAGO)</th>
					<th colspan="4">SALDO A LIBERAR<br />(RAP INSCRITO - FINANCEIRO LIBERADO)</th>
				</tr>
				<tr>
					<th>100-3 C (a)</th>
					<th>100-4 D (b)</th>
					<th>112-3 C (c)</th>
					<th>112-4 D (d)</th>
					
					<th>100-3 C (e)</th>
					<th>100-4 D (f)</th>
					<th>112-3 C (g)</th>
					<th>112-4 D (h)</th>
					
					<th>100-3 C (i)</th>
					<th>112-3 C (j)</th>
					<th>TOTAL</th>
					
					<th>100-3 C (l)</th>
					<th>100-4 D (m)</th>
					<th>112-3 C (n)</th>
					<th>112-4 D (o)</th>
					
					<th>100-3 C (p)</th>
					<th>100-4 D (q)</th>
					<th>112-3 C (r)</th>
					<th>112-4 D (s)</th>
					
					<th>100-3 C (t)</th>
					<th>100-4 D (u)</th>
					<th>112-3 C (v)</th>
					<th>112-4 D (w)</th>
					
					<th>100-3 C (x)</th>
					<th>100-4 D (y)</th>
					<th>112-3 C (z)</th>
					<th>112-4 D (a1)</th>
					
					<th>100-3 C (b1)</th>
					<th>100-4 D (c1)</th>
					<th>112-3 C (d1)</th>
					<th>112-4 D (e1)</th>
					
					<th>100-3 C (f1)</th>
					<th>100-4 D (g1)</th>
					<th>112-3 C (h1)</th>
					<th>112-4 D (i1)</th>
				</tr>
			</thead>
			<tbody>
				<?
					for($i=0; $i<count($result); $i++) {
						echo "<tr>
								<td>".$result[$i]["gr_unidade_orcamentaria"]."</td>
								<td>".$result[$i]["coluna1"]."</td>
								<td>".$result[$i]["coluna2"]."</td>
								<td>".$result[$i]["coluna3"]."</td>
								<td>".$result[$i]["coluna4"]."</td>
								<td>".$result[$i]["coluna5"]."</td>
								<td>".$result[$i]["coluna6"]."</td>
								<td>".$result[$i]["coluna7"]."</td>
								<td>".$result[$i]["coluna8"]."</td>
								<td>".$result[$i]["coluna9"]."</td>
								<td>".$result[$i]["coluna10"]."</td>
								<td>".$result[$i]["coluna11"]."</td>
								<td>".$result[$i]["coluna12"]."</td>
								<td>".$result[$i]["coluna13"]."</td>
								<td>".$result[$i]["coluna14"]."</td>
								<td>".$result[$i]["coluna15"]."</td>
								<td>".$result[$i]["coluna16"]."</td>
								<td>".$result[$i]["coluna17"]."</td>
								<td>".$result[$i]["coluna18"]."</td>
								<td>".$result[$i]["coluna19"]."</td>
								<td>".$result[$i]["coluna20"]."</td>
								<td>".$result[$i]["coluna21"]."</td>
								<td>".$result[$i]["coluna22"]."</td>
								<td>".$result[$i]["coluna23"]."</td>
								<td>".$result[$i]["coluna24"]."</td>
								<td>".$result[$i]["coluna25"]."</td>
								<td>".$result[$i]["coluna26"]."</td>
								<td>".$result[$i]["coluna27"]."</td>
								<td>".$result[$i]["coluna28"]."</td>
								<td>".$result[$i]["coluna29"]."</td>
								<td>".$result[$i]["coluna30"]."</td>
								<td>".$result[$i]["coluna31"]."</td>
								<td>".$result[$i]["coluna32"]."</td>
								<td>".$result[$i]["coluna33"]."</td>
								<td>".$result[$i]["coluna34"]."</td>
								<td>".$result[$i]["coluna35"]."</td>
							</tr>";
					}
				?>
			</tbody>
		</table>
		<script type="text/javascript" language="javascript">
			document.getElementById('aguarde').style.visibility = 'hidden';
			document.getElementById('aguarde').style.display = 'none';
		</script>
		</body>
	</html>