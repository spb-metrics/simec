<?

ini_set( "memory_limit", "1024M" );
set_time_limit(0);

if ( $_REQUEST['salvar'] )
{
	$existe_rel = 0;
	$sql = sprintf(
		"SELECT prtid FROM public.parametros_tela WHERE prtdsc = '%s'",
		$_REQUEST['titulo']
	);
	$existe_rel = $db->pegaUm( $sql );
	
	if ($existe_rel > 0)
	{
		$sql = sprintf(
			"UPDATE public.parametros_tela SET prtdsc = '%s', prtobj = '%s', prtpublico = 'FALSE', usucpf = '%s', mnuid = %d WHERE prtid = %d",
			$_REQUEST['titulo'],
			addslashes( addslashes( serialize( $_REQUEST ) ) ),
			$_SESSION['usucpf'],
			$_SESSION['mnuid'],
			$existe_rel
		);
		$db->executar( $sql );
		$db->commit();
	}
	else 
	{
		$sql = sprintf(
			"INSERT INTO public.parametros_tela ( prtdsc, prtobj, prtpublico, usucpf, mnuid ) VALUES ( '%s', '%s', %s, '%s', %d )",
			$_REQUEST['titulo'],
			addslashes( addslashes( serialize( $_REQUEST ) ) ),
			'FALSE',
			$_SESSION['usucpf'],
			$_SESSION['mnuid']
		);
		$db->executar( $sql );
		$db->commit();
	}
	?>
	<script type="text/javascript">
		alert("Operação realizada com sucesso!");
		location.href = '?modulo=relatorio/geral_novo&acao=R';
	</script>
	<?
}


// ***********************
// Montagem do SQL *******
// ***********************


// NOME DA TABELA USADA 
$tabela = "financeiro.execucao1 orc";


$arrayTituloColunas = array();
$arrayTituloAgrupadores = array();
$tituloAgrupadores = "";
$arraySelectCod = array();
$selectCod = "";
$arraySelectDsc = array();
$selectDsc = "";
$arraySum = array();
$sum = "";
$arrayJoin = array();
$join = "";
$arrayWhere = array();
$where = "";
$arrayGroupBy = array();
$groupBy = "";
$arrayOrderBy = array();
$orderBy = "";

$arrayAuxiliar = array();

for($i=0; $i<count($_REQUEST["agrupador"]); $i++) {
	switch($_REQUEST["agrupador"][$i]) {
		case "acacod":
			array_push($arrayTituloAgrupadores, "Ação");
			
			array_push($arraySelectCod, "orc.acacod AS cod_agrupador".($i+1));
			array_push($arraySelectDsc, "orc.acadsc AS dsc_agrupador".($i+1));
			
			array_push($arrayGroupBy, "orc.acacod");
			array_push($arrayGroupBy, "orc.acadsc");
			
			array_push($arrayOrderBy, "orc.acacod");
			array_push($arrayOrderBy, "orc.acadsc");
			
			break;
			
		case "catecon":
			array_push($arrayTituloAgrupadores, "Categoria Econômica");
			
			array_push($arraySelectCod, "cte.ctecod AS cod_agrupador".($i+1));
			array_push($arraySelectDsc, "cte.ctedsc AS dsc_agrupador".($i+1));
			
			array_push($arrayJoin, "INNER JOIN public.categoriaeconomica cte ON cte.ctecod = orc.ctecod");
			
			array_push($arrayGroupBy, "cte.ctecod");
			array_push($arrayGroupBy, "cte.ctedsc");
			
			array_push($arrayOrderBy, "cte.ctecod");
			array_push($arrayOrderBy, "cte.ctedsc");
			
			break;
			
		case "elemento":
			array_push($arrayTituloAgrupadores, "Elemento de Despesa");
			
			array_push($arraySelectCod, "ele.edpcod AS cod_agrupador".($i+1));
			array_push($arraySelectDsc, "ele.edpdsc AS dsc_agrupador".($i+1));
			
			array_push($arrayJoin, "INNER JOIN public.elementodespesa ele ON ele.edpcod = orc.edpcod");
			
			array_push($arrayGroupBy, "ele.edpcod");
			array_push($arrayGroupBy, "ele.edpdsc");
			
			array_push($arrayOrderBy, "ele.edpcod");
			array_push($arrayOrderBy, "ele.edpdsc");
			
			break;
			
		case "esfera": 
			array_push($arrayTituloAgrupadores, "Esfera");
			
			array_push($arraySelectCod, "esf.esfcod AS cod_agrupador".($i+1));
			array_push($arraySelectDsc, "esf.esfdsc AS dsc_agrupador".($i+1));
			
			array_push($arrayJoin, "INNER JOIN public.esfera esf ON esf.esfcod = orc.esfcod");
			
			array_push($arrayGroupBy, "esf.esfcod");
			array_push($arrayGroupBy, "esf.esfdsc");
			
			array_push($arrayOrderBy, "esf.esfcod");
			array_push($arrayOrderBy, "esf.esfdsc");
			
			break;
			
		case "fonte":
			array_push($arrayTituloAgrupadores, "Fonte");
			
			array_push($arraySelectCod, "fr.foncod AS cod_agrupador".($i+1));
			array_push($arraySelectDsc, "fr.fondsc AS dsc_agrupador".($i+1));
			
			array_push($arrayJoin, "INNER JOIN public.fonterecurso fr ON fr.foncod = orc.foncod");
			
			array_push($arrayGroupBy, "fr.foncod");
			array_push($arrayGroupBy, "fr.fondsc");
			
			array_push($arrayOrderBy, "fr.foncod");
			array_push($arrayOrderBy, "fr.fondsc");
			
			break;
			
		case "funcao":
			array_push($arrayTituloAgrupadores, "Função");
			
			array_push($arraySelectCod, "f.it_co_funcao AS cod_agrupador".($i+1));
			array_push($arraySelectDsc, "f.it_no_funcao AS dsc_agrupador".($i+1));
			
			array_push($arrayJoin, "INNER JOIN siafi.funcao f ON f.it_co_funcao = orc.funcod");
			
			array_push($arrayGroupBy, "f.it_co_funcao");
			array_push($arrayGroupBy, "f.it_no_funcao");
			
			array_push($arrayOrderBy, "f.it_co_funcao");
			array_push($arrayOrderBy, "f.it_no_funcao");
			
			break;
			
		case "gnd":
			array_push($arrayTituloAgrupadores, "GND");
			
			array_push($arraySelectCod, "gnd.gndcod AS cod_agrupador".($i+1));
			array_push($arraySelectDsc, "gnd.gnddsc AS dsc_agrupador".($i+1));
			
			array_push($arrayJoin, "INNER JOIN public.gnd ON gnd.gndcod = orc.gndcod");
			
			array_push($arrayGroupBy, "gnd.gndcod");
			array_push($arrayGroupBy, "gnd.gnddsc");
			
			array_push($arrayOrderBy, "gnd.gndcod");
			array_push($arrayOrderBy, "gnd.gnddsc");
			
			break;

		case "gestao":
			array_push($arrayTituloAgrupadores, "Gestão");
			
			array_push($arraySelectCod, "orc.gescod AS cod_agrupador".($i+1));
			array_push($arraySelectDsc, "orc.gesdsc AS dsc_agrupador".($i+1));
			
			array_push($arrayGroupBy, "orc.gescod");
			array_push($arrayGroupBy, "orc.gesdsc");
			
			array_push($arrayOrderBy, "orc.gescod");
			array_push($arrayOrderBy, "orc.gesdsc");
			
			break;
			
		case "grf":
			array_push($arrayTituloAgrupadores, "Grupo de Fonte");
			
			array_push($arraySelectCod, "gf.grfid AS cod_agrupador".($i+1));
			array_push($arraySelectDsc, "gf.grfdsc AS dsc_agrupador".($i+1));
			
			array_push($arrayJoin, "INNER JOIN public.grupofonte gf ON gf.grfid = cast( substring( orc.foncod, 1, 1 ) as integer )");
			
			array_push($arrayGroupBy, "gf.grfid");
			array_push($arrayGroupBy, "gf.grfdsc");
			
			array_push($arrayOrderBy, "gf.grfid");
			array_push($arrayOrderBy, "gf.grfdsc");
			
			break;
			
		case "grupouo":
			array_push($arrayTituloAgrupadores, "Grupo de Unidade Orçamentária");
			
			array_push($arraySelectCod, "gu.gunid AS cod_agrupador".($i+1));
			array_push($arraySelectDsc, "gu.gundsc AS dsc_agrupador".($i+1));
			
			if(!in_array("INNER JOIN public.unidade u ON u.unicod = orc.unicod", $arrayJoin)) {
				array_push($arrayJoin, "INNER JOIN public.unidade u ON u.unicod = orc.unicod");
			}
			
			array_push($arrayJoin, "LEFT JOIN public.grupounidade gu ON gu.gunid = u.gunid");
			
			array_push($arrayGroupBy, "gu.gunid");
			array_push($arrayGroupBy, "gu.gundsc");
			
			array_push($arrayOrderBy, "gu.gunid");
			array_push($arrayOrderBy, "gu.gundsc");
			
			break;
			
		case "usopri":
			array_push($arrayTituloAgrupadores, "Ident. de Result. Primário");
			
			array_push($arraySelectCod, "irp.irpcod AS cod_agrupador".($i+1));
			array_push($arraySelectDsc, "irp.irpdsc AS dsc_agrupador".($i+1));
			
			array_push($arrayJoin, "INNER JOIN public.identresultprimario irp ON irp.irpcod = orc.irpcod");
			
			array_push($arrayGroupBy, "irp.irpcod");
			array_push($arrayGroupBy, "irp.irpdsc");
			
			array_push($arrayOrderBy, "irp.irpcod");
			array_push($arrayOrderBy, "irp.irpdsc");
			
			break;
			
		case "iducod":
			array_push($arrayTituloAgrupadores, "Identificador de Uso");
			
			array_push($arraySelectCod, "iu.iducod AS cod_agrupador".($i+1));
			array_push($arraySelectDsc, "iu.idudsc AS dsc_agrupador".($i+1));
			
			array_push($arrayJoin, "INNER JOIN public.identifuso iu ON iu.iducod = orc.iducod");
			
			array_push($arrayGroupBy, "iu.iducod");
			array_push($arrayGroupBy, "iu.idudsc");
			
			array_push($arrayOrderBy, "iu.iducod");
			array_push($arrayOrderBy, "iu.idudsc");
			
			break;
			
		case "localizador":
			array_push($arrayTituloAgrupadores, "Localizador");
			
			array_push($arraySelectCod, "loc.prgcod || '.' || loc.acacod || '.' || loc.unicod || '.' || loc.loccod AS cod_agrupador".($i+1));
			array_push($arraySelectDsc, "loc.acadsc || ' ' || loc.sacdsc AS dsc_agrupador".($i+1));
			
			array_push($arrayJoin, "INNER JOIN monitora.acao loc ON loc.acacod = orc.acacod AND loc.prgcod = orc.prgcod AND loc.unicod = orc.unicod AND loc.loccod = orc.loccod AND loc.prgano = orc.rofano AND loc.acasnrap = 'f'");
			
			array_push($arrayGroupBy, "loc.prgcod || '.' || loc.acacod || '.' || loc.unicod || '.' || loc.loccod");
			array_push($arrayGroupBy, "loc.acadsc || ' ' || loc.sacdsc");
			
			array_push($arrayOrderBy, "loc.prgcod || '.' || loc.acacod || '.' || loc.unicod || '.' || loc.loccod");
			array_push($arrayOrderBy, "loc.acadsc || ' ' || loc.sacdsc");
			
			break;
			
		case "mapcod":
			array_push($arrayTituloAgrupadores, "Modalidade de Aplicação");
			
			array_push($arraySelectCod, "ma.mapcod AS cod_agrupador".($i+1));
			array_push($arraySelectDsc, "ma.mapdsc AS dsc_agrupador".($i+1));
			
			array_push($arrayJoin, "INNER JOIN public.modalidadeaplicacao ma ON ma.mapcod = orc.mapcod");
			
			array_push($arrayGroupBy, "ma.mapcod");
			array_push($arrayGroupBy, "ma.mapdsc");
			
			array_push($arrayOrderBy, "ma.mapcod");
			array_push($arrayOrderBy, "ma.mapdsc");
			
			break;

		case "mes":
			array_push($arrayTituloAgrupadores, "Mês");
			
			array_push($arraySelectCod, "orc.rofmes AS cod_agrupador".($i+1));
			array_push($arraySelectDsc, "orc.rofmesdsc AS dsc_agrupador".($i+1));
			
			array_push($arrayGroupBy, "orc.rofmes");
			array_push($arrayGroupBy, "orc.rofmesdsc");
			
			array_push($arrayOrderBy, "orc.rofmes");
			array_push($arrayOrderBy, "orc.rofmesdsc");
			
			break;
			
		case "natureza":
			array_push($arrayTituloAgrupadores, "Natureza de Despesa");
			
			array_push($arraySelectCod, "cast(nd.ctecod as varchar) || nd.gndcod || nd.mapcod || nd.edpcod AS cod_agrupador".($i+1));
			array_push($arraySelectDsc, "nd.ndpdsc AS dsc_agrupador".($i+1));
			
			array_push($arrayJoin, "INNER JOIN public.naturezadespesa nd ON cast(nd.ctecod as varchar) || nd.gndcod || nd.mapcod || nd.edpcod = cast(orc.ctecod as varchar) || cast(orc.gndcod as varchar) || orc.mapcod || orc.edpcod AND nd.sbecod = '00'");
			
			array_push($arrayGroupBy, "cast(nd.ctecod as varchar) || nd.gndcod || nd.mapcod || nd.edpcod");
			array_push($arrayGroupBy, "nd.ndpdsc");
			
			array_push($arrayOrderBy, "cast(nd.ctecod as varchar) || nd.gndcod || nd.mapcod || nd.edpcod");
			array_push($arrayOrderBy, "nd.ndpdsc");
			
			break;
			
		case "planointerno":
			array_push($arrayTituloAgrupadores, "Plano Interno");
			
			array_push($arraySelectCod, "orc.plicod AS cod_agrupador".($i+1));
			array_push($arraySelectDsc, "orc.plidsc AS dsc_agrupador".($i+1));
			
			array_push($arrayGroupBy, "orc.plicod");
			array_push($arrayGroupBy, "orc.plidsc");
			
			array_push($arrayOrderBy, "orc.plicod");
			array_push($arrayOrderBy, "orc.plidsc");
			
			break;
			
		case "programa":
			array_push($arrayTituloAgrupadores, "Programa");
			
			array_push($arraySelectCod, "orc.prgcod AS cod_agrupador".($i+1));
			array_push($arraySelectDsc, "orc.prgdsc AS dsc_agrupador".($i+1));
			
			array_push($arrayGroupBy, "orc.prgcod");
			array_push($arrayGroupBy, "orc.prgdsc");
			
			array_push($arrayOrderBy, "orc.prgcod");
			array_push($arrayOrderBy, "orc.prgdsc");
			
			break;
			
		case "sub-elemento":
			array_push($arrayTituloAgrupadores, "Sub-Elemento de Despesa");
			
			array_push($arraySelectCod, "orc.sbecod AS cod_agrupador".($i+1));
			array_push($arraySelectDsc, "orc.sbecod AS dsc_agrupador".($i+1));
			
			array_push($arrayGroupBy, "orc.sbecod");
			array_push($arrayGroupBy, "orc.sbecod");
			
			array_push($arrayOrderBy, "orc.sbecod");
			array_push($arrayOrderBy, "orc.sbecod");
			
			break;
			
		case "subfuncao":
			array_push($arrayTituloAgrupadores, "Sub-função");
			
			array_push($arraySelectCod, "sf.sfucod AS cod_agrupador".($i+1));
			array_push($arraySelectDsc, "sf.sfudsc AS dsc_agrupador".($i+1));
			
			array_push($arrayJoin, "INNER JOIN public.ppasubfuncao sf ON sf.sfucod = orc.sfucod");
			
			array_push($arrayGroupBy, "sf.sfucod");
			array_push($arrayGroupBy, "sf.sfudsc");
			
			array_push($arrayOrderBy, "sf.sfucod");
			array_push($arrayOrderBy, "sf.sfudsc");
			
			break;

		case "ug":
			array_push($arrayTituloAgrupadores, "Unidade Gestora");
			
			array_push($arraySelectCod, "orc.ungcod AS cod_agrupador".($i+1));
			array_push($arraySelectDsc, "orc.ungdsc AS dsc_agrupador".($i+1));
			
			array_push($arrayGroupBy, "orc.ungcod");
			array_push($arrayGroupBy, "orc.ungdsc");
			
			array_push($arrayOrderBy, "orc.ungcod");
			array_push($arrayOrderBy, "orc.ungdsc");
			
			break;
			
		case "uo":
			array_push($arrayTituloAgrupadores, "Unidade Orçamentária");
			
			array_push($arraySelectCod, "u.unicod AS cod_agrupador".($i+1));
			array_push($arraySelectDsc, "u.unidsc AS dsc_agrupador".($i+1));
			
			if(!in_array("INNER JOIN public.unidade u ON u.unicod = orc.unicod", $arrayJoin)) {
				array_push($arrayJoin, "INNER JOIN public.unidade u ON u.unicod = orc.unicod");
			}
			
			array_push($arrayGroupBy, "u.unicod");
			array_push($arrayGroupBy, "u.unidsc");
			
			array_push($arrayOrderBy, "u.unicod");
			array_push($arrayOrderBy, "u.unidsc");
			
			break;
		
		case "orgao":
			array_push($arrayTituloAgrupadores, "Órgão");
			
			array_push($arraySelectCod, "o.orgcod AS cod_agrupador".($i+1));
			array_push($arraySelectDsc, "o.orgdsc AS dsc_agrupador".($i+1));
			
			array_push($arrayJoin, "INNER JOIN public.orgao o ON o.orgcod = orc.orgcod AND o.organo = orc.rofano");
			
			array_push($arrayGroupBy, "o.orgcod");
			array_push($arrayGroupBy, "o.orgdsc");
			
			array_push($arrayOrderBy, "o.orgcod");
			array_push($arrayOrderBy, "o.orgdsc");
			
			break;
	}
}



for($i=0; $i<count($_REQUEST["agrupadorColunas"]); $i++) {
	switch($_REQUEST["agrupadorColunas"][$i]) {
		case "rofpl":
			array_push($arrayTituloColunas, "PL");
			array_push($arraySum, "sum( coalesce( rofpl, 0 ) ) / ".$_REQUEST["escala"]." AS coluna".($i+1));
			break;
			
		case "rofdot_ini":
			array_push($arrayTituloColunas, "Dotação Inicial");
			array_push($arraySum, "sum( coalesce( rofdot_ini, 0 ) ) / ".$_REQUEST["escala"]." AS coluna".($i+1));
			break;
			
		case "rofautorizado":
			array_push($arrayTituloColunas, "Autorizado");
			array_push($arraySum, "sum( coalesce( rofautorizado, 0 ) ) / ".$_REQUEST["escala"]." AS coluna".($i+1));
			break;
			
		case "rofpreempenho":
			array_push($arrayTituloColunas, "Pré Empenho");
			array_push($arraySum, "sum( coalesce( rofpreempenho, 0 ) ) / ".$_REQUEST["escala"]." AS coluna".($i+1));
			break;
			
		case "rofempenhado":
			array_push($arrayTituloColunas, "Empenhado");
			array_push($arraySum, "sum( coalesce( rofempenhado, 0 ) ) / ".$_REQUEST["escala"]." AS coluna".($i+1));
			break;
			
		case "rofaliquidar":
			array_push($arrayTituloColunas, "à Liquidar");
			array_push($arraySum, "sum( coalesce( rofaliquidar, 0 ) ) / ".$_REQUEST["escala"]." AS coluna".($i+1));
			break;
			
		case "rofliquidado_favorecido":
			array_push($arrayTituloColunas, "Liquidado Favorecido");
			array_push($arraySum, "sum( coalesce( rofliquidado_favorecido, 0 ) ) / ".$_REQUEST["escala"]." AS coluna".($i+1));
			break;
			
		case "rofpago":
			array_push($arrayTituloColunas, "Pago");
			array_push($arraySum, "sum( coalesce( rofpago, 0 ) ) / ".$_REQUEST["escala"]." AS coluna".($i+1));
			break;
			
		case "rof_creditodisponivel":
			array_push($arrayTituloColunas, "Crédito Disponível");
			array_push($arraySum, "sum( coalesce( rof_creditodisponivel, 0 ) ) / ".$_REQUEST["escala"]." AS coluna".($i+1));
			break;
			
		case "rof_creditodindisponivel":
			array_push($arrayTituloColunas, "Crédito Indisponível");
			array_push($arraySum, "sum( coalesce( rof_creditodindisponivel, 0 ) ) / ".$_REQUEST["escala"]." AS coluna".($i+1));
			break;
			
		case "rofpl_credadicionais":
			array_push($arrayTituloColunas, "PL Créditos Adicionais");
			array_push($arraySum, "sum( coalesce( rofpl_credadicionais, 0 ) ) / ".$_REQUEST["escala"]." AS coluna".($i+1));
			break;
			
		case "rofpl_credsuplementares":
			array_push($arrayTituloColunas, "PL Créditos Suplementares");
			array_push($arraySum, "sum( coalesce( rofpl_credsuplementares, 0 ) ) / ".$_REQUEST["escala"]." AS coluna".($i+1));
			break;
			
		case "rofpl_credespeciais":
			array_push($arrayTituloColunas, "PL Créditos Especiais");
			array_push($arraySum, "sum( coalesce( rofpl_credespeciais, 0 ) ) / ".$_REQUEST["escala"]." AS coluna".($i+1));
			break;
			
		case "rofpl_credextraordinarios":
			array_push($arrayTituloColunas, "PL Créditos Extraordinários");
			array_push($arraySum, "sum( coalesce( rofpl_credextraordinarios, 0 ) ) / ".$_REQUEST["escala"]." AS coluna".($i+1));
			break;
		
		case "rofpl_canc":
			array_push($arrayTituloColunas, "PL Cancelamento");
			array_push($arraySum, "sum( coalesce( rofpl_canc, 0 ) ) / ".$_REQUEST["escala"]." AS coluna".($i+1));
			break;
			
		case "rofpl_cred":
			array_push($arrayTituloColunas, "PL Crédito");
			array_push($arraySum, "sum( coalesce( rofpl_cred, 0 ) ) / ".$_REQUEST["escala"]." AS coluna".($i+1));
			break;
			
		case "rofdotori":
			array_push($arrayTituloColunas, "Dotação Original");
			array_push($arraySum, "sum( coalesce( rofdotori, 0 ) ) / ".$_REQUEST["escala"]." AS coluna".($i+1));
			break;
	}
}



/************************************************************/
/*** INÍCIO - Monta a cláusula WHERE à partir dos filtros ***/
/************************************************************/

/*** [Grupo de UO] ***/
if($_REQUEST["grupouo"][0] != "") {
	
	if(!in_array("INNER JOIN public.unidade u ON u.unicod = orc.unicod", $arrayJoin)) {
		array_push($arrayJoin, "INNER JOIN public.unidade u ON u.unicod = orc.unicod");
	}
	if(!in_array("LEFT JOIN public.grupounidade gu ON gu.gunid = u.gunid", $arrayJoin)) {
		array_push($arrayJoin, "LEFT JOIN public.grupounidade gu ON gu.gunid = u.gunid");
	}
	
	for($i=0; $i<count($_REQUEST["grupouo"]); $i++) {
		array_push($arrayAuxiliar, "'".$_REQUEST["grupouo"][$i]."'");
	}
	
	if($_REQUEST["grupouo_campo_excludente"] != "1")
		array_push($arrayWhere, "gu.gunid in (".implode(",", $arrayAuxiliar).")");
	else
		array_push($arrayWhere, "gu.gunid not in (".implode(",", $arrayAuxiliar).")");
		
	$arrayAuxiliar = array();
}
/*** [/Grupo de UO] ***/


/*** [Unidades Orçamentárias] ***/
if($_REQUEST["uo"][0] != "") {
	
	if(!in_array("INNER JOIN public.unidade u ON u.unicod = orc.unicod", $arrayJoin)) {
		array_push($arrayJoin, "INNER JOIN public.unidade u ON u.unicod = orc.unicod");
	}
	
	for($i=0; $i<count($_REQUEST["uo"]); $i++) {
		array_push($arrayAuxiliar, "'".$_REQUEST["uo"][$i]."'");
	}
	
	if($_REQUEST["uo_campo_excludente"] != "1")
		array_push($arrayWhere, "u.unicod in (".implode(",", $arrayAuxiliar).")");
	else
		array_push($arrayWhere, "u.unicod not in (".implode(",", $arrayAuxiliar).")");
		
	$arrayAuxiliar = array();
}
/*** [/Unidades Orçamentárias] ***/


/*** [Esfera] ***/
if($_REQUEST["esfera"][0] != "") {
	
	if(!in_array("INNER JOIN public.esfera esf ON esf.esfcod = orc.esfcod", $arrayJoin)) {
		array_push($arrayJoin, "INNER JOIN public.esfera esf ON esf.esfcod = orc.esfcod");
	}
	
	for($i=0; $i<count($_REQUEST["esfera"]); $i++) {
		array_push($arrayAuxiliar, "'".$_REQUEST["esfera"][$i]."'");
	}
	
	if($_REQUEST["esfera_campo_excludente"] != "1")
		array_push($arrayWhere, "esf.esfcod in (".implode(",", $arrayAuxiliar).")");
	else
		array_push($arrayWhere, "esf.esfcod not in (".implode(",", $arrayAuxiliar).")");
	
	$arrayAuxiliar = array();
}
/*** [/Esfera] ***/


/*** [Função] ***/
if($_REQUEST["funcao"][0] != "") {
	
	if(!in_array("INNER JOIN siafi.funcao f ON f.it_co_funcao = orc.funcod", $arrayJoin)) {
		array_push($arrayJoin, "INNER JOIN siafi.funcao f ON f.it_co_funcao = orc.funcod");
	}
	
	for($i=0; $i<count($_REQUEST["funcao"]); $i++) {
		array_push($arrayAuxiliar, "'".$_REQUEST["funcao"][$i]."'");
	}
	
	if($_REQUEST["funcao_campo_excludente"] != "1")
		array_push($arrayWhere, "f.funcod in (".implode(",", $arrayAuxiliar).")");
	else
		array_push($arrayWhere, "f.funcod not in (".implode(",", $arrayAuxiliar).")");
	
	$arrayAuxiliar = array();
}
/*** [/Função] ***/


/*** [Sub-Função] ***/
if($_REQUEST["subfuncao"][0] != "") {
	
	if(!in_array("INNER JOIN public.ppasubfuncao sf ON sf.sfucod = orc.sfucod", $arrayJoin)) {
		array_push($arrayJoin, "INNER JOIN public.ppasubfuncao sf ON sf.sfucod = orc.sfucod");
	}
	
	for($i=0; $i<count($_REQUEST["subfuncao"]); $i++) {
		array_push($arrayAuxiliar, "'".$_REQUEST["subfuncao"][$i]."'");
	}
	
	if($_REQUEST["subfuncao_campo_excludente"] != "1")
		array_push($arrayWhere, "sf.sfucod in (".implode(",", $arrayAuxiliar).")");
	else
		array_push($arrayWhere, "sf.sfucod not in (".implode(",", $arrayAuxiliar).")");
	
	$arrayAuxiliar = array();
}
/*** [/Sub-Função] ***/


/*** [Programa] ***/
if($_REQUEST["programa"][0] != "") {
	for($i=0; $i<count($_REQUEST["programa"]); $i++) {
		array_push($arrayAuxiliar, "'".$_REQUEST["programa"][$i]."'");
	}
	
	if($_REQUEST["programa_campo_excludente"] != "1")
		array_push($arrayWhere, "orc.prgcod in (".implode(",", $arrayAuxiliar).")");
	else
		array_push($arrayWhere, "orc.prgcod not in (".implode(",", $arrayAuxiliar).")");
	
	$arrayAuxiliar = array();
}
/*** [/Programa] ***/


/*** [Ação] ***/
if($_REQUEST["acacod"][0] != "") {
	for($i=0; $i<count($_REQUEST["acacod"]); $i++) {
		array_push($arrayAuxiliar, "'".$_REQUEST["acacod"][$i]."'");
	}
	
	if($_REQUEST["acacod_campo_excludente"] != "1")
		array_push($arrayWhere, "orc.acacod in (".implode(",", $arrayAuxiliar).")");
	else
		array_push($arrayWhere, "orc.acacod not in (".implode(",", $arrayAuxiliar).")");
	
	$arrayAuxiliar = array();
}
/*** [/Ação] ***/


/*** [Grupo Fonte] ***/
if($_REQUEST["grf"][0] != "") {
	
	if(!in_array("INNER JOIN public.grupofonte gf ON gf.grfid = cast( substring( orc.foncod, 1, 1 ) as integer )", $arrayJoin)) {
		array_push($arrayJoin, "INNER JOIN public.grupofonte gf ON gf.grfid = cast( substring( orc.foncod, 1, 1 ) as integer )");
	}
	
	for($i=0; $i<count($_REQUEST["grf"]); $i++) {
		array_push($arrayAuxiliar, "'".$_REQUEST["grf"][$i]."'");
	}
	
	if($_REQUEST["grf_campo_excludente"] != "1")
		array_push($arrayWhere, "gf.grfid in (".implode(",", $arrayAuxiliar).")");
	else
		array_push($arrayWhere, "gf.grfid not in (".implode(",", $arrayAuxiliar).")");
	
	$arrayAuxiliar = array();
}
/*** [/Grupo Fonte] ***/


/*** [Identificador de Uso] ***/
if($_REQUEST["iducod"][0] != "") {
	
	if(!in_array("INNER JOIN public.identifuso iu ON iu.iducod = orc.iducod", $arrayJoin)) {
		array_push($arrayJoin, "INNER JOIN public.identifuso iu ON iu.iducod = orc.iducod");
	}
	
	for($i=0; $i<count($_REQUEST["iducod"]); $i++) {
		array_push($arrayAuxiliar, "'".$_REQUEST["iducod"][$i]."'");
	}
	
	if($_REQUEST["iducod_campo_excludente"] != "1")
		array_push($arrayWhere, "iu.iducod in (".implode(",", $arrayAuxiliar).")");
	else
		array_push($arrayWhere, "iu.iducod not in (".implode(",", $arrayAuxiliar).")");
	
	$arrayAuxiliar = array();
}
/*** [/Identificador de Uso] ***/


/*** [Fonte de Recurso] ***/
if($_REQUEST["fonte"][0] != "") {
	
	if(!in_array("INNER JOIN public.fonterecurso fr ON fr.foncod = orc.foncod", $arrayJoin)) {
		array_push($arrayJoin, "INNER JOIN public.fonterecurso fr ON fr.foncod = orc.foncod");
	}
	
	for($i=0; $i<count($_REQUEST["fonte"]); $i++) {
		array_push($arrayAuxiliar, "'".$_REQUEST["fonte"][$i]."'");
	}
	
	if($_REQUEST["fonte_campo_excludente"] != "1")
		array_push($arrayWhere, "fr.foncod in (".implode(",", $arrayAuxiliar).")");
	else
		array_push($arrayWhere, "fr.foncod not in (".implode(",", $arrayAuxiliar).")");
	
	$arrayAuxiliar = array();
}
/*** [/Fonte de Recurso] ***/


/*** [Fonte SIAFI] ***/
// FAZER DEPOIS QUE A CARGA ESTIVER PRONTA.
/*** [/Fonte SIAFI] ***/


/*** [Categoria Econômica] ***/
if($_REQUEST["catecon"][0] != "") {
	
	if(!in_array("INNER JOIN public.categoriaeconomica cte ON cte.ctecod = orc.ctecod", $arrayJoin)) {
		array_push($arrayJoin, "INNER JOIN public.categoriaeconomica cte ON cte.ctecod = orc.ctecod");
	}
	
	for($i=0; $i<count($_REQUEST["catecon"]); $i++) {
		array_push($arrayAuxiliar, "'".$_REQUEST["catecon"][$i]."'");
	}
	
	if($_REQUEST["catecon_campo_excludente"] != "1")
		array_push($arrayWhere, "cte.ctecod in (".implode(",", $arrayAuxiliar).")");
	else
		array_push($arrayWhere, "cte.ctecod not in (".implode(",", $arrayAuxiliar).")");
	
	$arrayAuxiliar = array();
}
/*** [/Categoria Econômica] ***/


/*** [GND] ***/
if($_REQUEST["gnd"][0] != "") {
	
	if(!in_array("INNER JOIN public.gnd ON gnd.gndcod = orc.gndcod", $arrayJoin)) {
		array_push($arrayJoin, "INNER JOIN public.gnd ON gnd.gndcod = orc.gndcod");
	}
	
	for($i=0; $i<count($_REQUEST["gnd"]); $i++) {
		array_push($arrayAuxiliar, "'".$_REQUEST["gnd"][$i]."'");
	}
	
	if($_REQUEST["gnd_campo_excludente"] != "1")
		array_push($arrayWhere, "gnd.gndcod in (".implode(",", $arrayAuxiliar).")");
	else
		array_push($arrayWhere, "gnd.gndcod not in (".implode(",", $arrayAuxiliar).")");
	
	$arrayAuxiliar = array();
}
/*** [/GND] ***/


/*** [Modalidade de Aplicação] ***/
if($_REQUEST["mapcod"][0] != "") {
	
	if(!in_array("INNER JOIN public.modalidadeaplicacao ma ON ma.mapcod = orc.mapcod", $arrayJoin)) {
		array_push($arrayJoin, "INNER JOIN public.modalidadeaplicacao ma ON ma.mapcod = orc.mapcod");
	}
	
	for($i=0; $i<count($_REQUEST["mapcod"]); $i++) {
		array_push($arrayAuxiliar, "'".$_REQUEST["mapcod"][$i]."'");
	}
	
	if($_REQUEST["mapcod_campo_excludente"] != "1")
		array_push($arrayWhere, "ma.mapcod in (".implode(",", $arrayAuxiliar).")");
	else
		array_push($arrayWhere, "ma.mapcod not in (".implode(",", $arrayAuxiliar).")");
	
	$arrayAuxiliar = array();
}
/*** [/Modalidade de Aplicação] ***/


/*** [Elemento de Despesa] ***/
if($_REQUEST["elemento"][0] != "") {
	
	if(!in_array("INNER JOIN public.elementodespesa ele ON ele.edpcod = orc.edpcod", $arrayJoin)) {
		array_push($arrayJoin, "INNER JOIN public.elementodespesa ele ON ele.edpcod = orc.edpcod");
	}
	
	for($i=0; $i<count($_REQUEST["elemento"]); $i++) {
		array_push($arrayAuxiliar, "'".$_REQUEST["elemento"][$i]."'");
	}
	
	if($_REQUEST["elemento_campo_excludente"] != "1")
		array_push($arrayWhere, "ele.edpcod in (".implode(",", $arrayAuxiliar).")");
	else
		array_push($arrayWhere, "ele.edpcod not in (".implode(",", $arrayAuxiliar).")");
	
	$arrayAuxiliar = array();
}
/*** [/Elemento de Despesa] ***/


/*** [Natureza de Despesa] ***/
if($_REQUEST["natureza"][0] != "") {
	
	if(!in_array("INNER JOIN public.naturezadespesa nd ON cast(nd.ctecod as varchar) || nd.gndcod || nd.mapcod || nd.edpcod = cast(orc.ctecod as varchar) || cast(orc.gndcod as varchar) || orc.mapcod || orc.edpcod AND nd.sbecod = '00'", $arrayJoin)) {
		array_push($arrayJoin, "INNER JOIN public.naturezadespesa nd ON cast(nd.ctecod as varchar) || nd.gndcod || nd.mapcod || nd.edpcod = cast(orc.ctecod as varchar) || cast(orc.gndcod as varchar) || orc.mapcod || orc.edpcod AND nd.sbecod = '00'");
	}
	
	for($i=0; $i<count($_REQUEST["natureza"]); $i++) {
		array_push($arrayAuxiliar, "'".$_REQUEST["natureza"][$i]."'");
	}
	
	if($_REQUEST["natureza_campo_excludente"] != "1")
		array_push($arrayWhere, "cast(nd.ctecod as varchar) || nd.gndcod || nd.mapcod || nd.edpcod in (".implode(",", $arrayAuxiliar).")");
	else
		array_push($arrayWhere, "cast(nd.ctecod as varchar) || nd.gndcod || nd.mapcod || nd.edpcod not in (".implode(",", $arrayAuxiliar).")");
	
	$arrayAuxiliar = array();
}
/*** [/Natureza de Despesa] ***/


/*** [Ident. Result. Primário] ***/
// FAZER DEPOIS QUE A CARGA ESTIVER PRONTA.
/*** [/Ident. Result. Primário] ***/


array_push($arrayWhere, "orc.rofano = '".$_REQUEST["ano"]."'");

/*********************************************************/
/*** FIM - Monta a cláusula WHERE à partir dos filtros ***/
/*********************************************************/

$tituloAgrupadores = implode("/", $arrayTituloAgrupadores);

$selectCod = implode(",", $arraySelectCod);
$selectDsc = implode(",", $arraySelectDsc);
$sum = implode(",", $arraySum);

if(!empty($arrayJoin)) {
	$join = implode(" ", $arrayJoin);
}	
if(!empty($arrayWhere)) {
	$where = implode(" AND ",$arrayWhere);
}	
	
$groupBy = implode(",", $arrayGroupBy);
$orderBy = implode(",", $arrayOrderBy);

$sql = "SELECT 
			".$selectCod.", 
			".$selectDsc.", 
			".$sum." 
		FROM 
			".$tabela." 
		$join 
		WHERE 
			".$where." 
		GROUP BY 
			".$groupBy." 
		ORDER BY 
			".$orderBy;

//dbg($sql,1);

		
// mostra relatório em XLS
if ( $_REQUEST['tipoRelatorio'] == 'xls' )
{
	$qtdAgrupadores = count($_REQUEST["agrupador"]) * 2;
	$tipo_campo = array();
	while( $qtdAgrupadores > 0 )
	{
		array_push( $tipo_campo, 's' );
		$qtdAgrupadores--;
	}
	for( $i = 0; $i < 10; $i++ )
	{
		array_push( $tipo_campo, 'n' );
	}
	header( 'Content-type: application/xls' );
	header( 'Content-Disposition: attachment; filename="planilha_simec.xls"' );
	$db->sql_to_excel( $sql, 'relorc', '', $tipo_campo );
	exit();
}
// FIM mostra relatório em XLS


$resultado = $db->carregar($sql);

?>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Expires" content="-1">
		<title>Acompanhamento da Execução Orçamentária</title>
		<style type="text/css">
			
			@media print {.notprint { display: none }}
			
			@media screen {
			.notscreen { display: none;  }
			.div_rolagem{ overflow-x: auto; overflow-y: auto; width:19.5cm;height:350px;}
			.topo { position: absolute; top: 0px; margin: 0; padding: 5px; position: fixed; background-color: #ffffff;}
			}
			
			*{margin:0; padding:0; border:none; font-size:10px;font-family:Arial;}
			.alignRight{text-align:right !important;}
			.alignCenter{ text-align:center !important;}
			.alignLeft{text-align:left !important;}
			.bold{font-weight:bold !important;}
			.italic{font-style:italic !important;}
			.noPadding{padding:0;}
			.titulo{width:52px;}
			.tituloagrup{font-size:9px;}
			.titulolinha{font-size:9px;}
			
			#tabelaTitulos th{border:2px solid black;border-left:none; border-right:none;}
			
			#orgao{margin:3px 0 0 0;}
			#orgao tr td{border:1px solid black;border-left:none;border-right:none;font-size:11px;}
			
			div.filtro { page-break-after: always; text-align: center; }
			
			table{width:19cm;border-collapse:collapse;}
			th, td{font-weight:normal;padding:4px;vertical-align:top;}
			thead{display:table-header-group;}
			table, tr{page-break-inside:avoid;}
			a{text-decoration:none;color:#3030aa;}
			a:hover{text-decoration:underline;color:#aa3030;}
			span.topo { position: absolute; top: 3px; margin: 0; padding: 5px; position: fixed; background-color: #f0f0f0; border: 1px solid #909090; cursor:pointer; }
			span.topo:hover { background-color: #d0d0d0; }
		</style>
		<!-- 
			border-collapse:collapse;
			#tabelaTitulos tr td, #tabelaTitulos tr th{border:2px solid black;border-left:none; border-right:none;}			
		 -->
	</head>
	<body>
		<div id="aguarde" style="background-color:#ffffff;position:absolute;color:#000033;top:50%;left:30%;border:2px solid #cccccc; width:300px;">
			<center style="font-size:12px;"><br><img src="../imagens/wait.gif" border="0" align="absmiddle"> Aguarde! Gerando Relatório...<br><br></center>
		</div>
		<script type="text/javascript">
			self.focus();
		</script>
		<table>
			<thead>
				<tr>
					<th class="noPadding" align="left">
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
									Orçamento Fiscal e Seg.Social - EM R$ <?= ($_REQUEST["escala"] == 1) ? "1,00" : "1.000,00" ?><br />
									Acumulado até: <?= formata_data( $db->pegaUm("SELECT max(rofdata) FROM ".$tabela."") ) ?>
								</td>
								<tr>
									<td colspan="2" align="center" valign="top" style="padding:0 0 5px 0;">
										<b><font style="font-size:14px;"><?= $_REQUEST["titulo"] ?></font></b>
									</td>
								</tr>
							</tr>
						</table>
						<table id="tabelaTitulos" align="left">
							<thead>
								<tr>
									<th class="bold alignLeft"><?=$tituloAgrupadores?></th>
									<?
										for($i=0; $i<count($arrayTituloColunas); $i++) {
											echo "<th class=\"alignRight\">".$arrayTituloColunas[$i]."</th>\n";
										}
									?>
								</tr>
								<?
									if($resultado) {
										// array para gravar o último registro dos agrupadores.
										$arrayAuxAgrupadores = array();
										// totais
										$arrayTotalColunas = array();
										$arrayTotalGeral = array();
										
										for($i=0; $i<count($resultado); $i++) {
											
											for($j=1; $j<=count($_REQUEST["agrupador"]); $j++) {
												$cor = ($j%2) ? "#f8f8f8" : "";
												$seta = ($j==1) ? "" : "&rsaquo;";
												
												// testa se é o último.
												if($j == count($_REQUEST["agrupador"])) {
													echo '<tr bgcolor="'.$cor.'" onmouseover="this.style.backgroundColor = \'#ffffcc\';" onmouseout="this.style.backgroundColor = \''.$cor.'\';">';
				
													echo '<td style="text-align:left; padding: 0 0 0 '.(($j-1) * 10).'px;">'.$seta.' '.$resultado[$i]["cod_agrupador".$j].' '.$resultado[$i]["dsc_agrupador".$j].'</td>';
													
													for($k=1; $k<=count($_REQUEST["agrupadorColunas"]); $k++) {
														if($resultado[$i]["coluna".$k] != NULL) {
															$valor = number_format($resultado[$i]["coluna".$k], 0, ',', '.' );
															
															for($z=1; $z<count($_REQUEST["agrupador"]); $z++) {
																$arrayTotalColunas[$z][$k] += $resultado[$i]["coluna".$k];
															}
															$arrayTotalGeral[$k] += $resultado[$i]["coluna".$k];
															
														} else {
															$valor = " - ";
														}
															
														echo '<td style="text-align:right;">'.$valor.'</td>';
													}
													
													echo '</tr>';
													
													// Quando for o último registro, imprime os totais restantes e o total geral.
													if($i == (count($resultado) - 1)) {
														//totais
														$val = (count($_REQUEST["agrupador"]) - 1);
														for($z=$val; $z>=1; $z--) {
															echo '<tr style="border-bottom:1px solid black; background-color: #f0f0f0;">';
															echo '<td style="text-align:left;border-bottom:1px solid black;">TOTAL('.$arrayAuxAgrupadores[$z].')</td>';
															
															for($k=1; $k<=count($_REQUEST["agrupadorColunas"]); $k++) {
																if($arrayTotalColunas[$z][$k] != NULL) {
																	$valor = number_format($arrayTotalColunas[$z][$k], 0, ',', '.' );
																} else {
																	$valor = " - ";
																}
																
																echo '<td style="text-align:right;border-bottom:1px solid black;">'.$valor.'</td>';
																
																$arrayTotalColunas[$z][$k] = 0;
																$arrayAuxAgrupadores[$z] = NULL;
															}
															
															echo '</tr>';
														}
														
														//TOTAL GERAL
														echo '<tr style="border-bottom:1px solid black; background-color: #f0f0f0;">';
														echo '<td style="text-align:left;border-bottom:1px solid black;">TOTAL GERAL</td>';
														
														for($k=1; $k<=count($arrayTotalGeral); $k++) {
															if($arrayTotalGeral[$k] != NULL) {
																$valor = number_format($arrayTotalGeral[$k], 0, ',', '.' );
															} else {
																$valor = " - ";
															}
															
															echo '<td style="text-align:right;border-bottom:1px solid black;">'.$valor.'</td>';
														}
														
														echo '</tr>';
													}
												}
												// se não for o último, ou imprime sem valor, testa se ele já foi impresso ou imprime total.
												else {
													// já foi impresso.
													if($arrayAuxAgrupadores[$j] == $resultado[$i]["cod_agrupador".$j]) {
														continue;
													}
													else {
														// imprime o agrupador corrente sem valor.
														if($arrayAuxAgrupadores[$j] == NULL) {
															$arrayAuxAgrupadores[$j] = $resultado[$i]["cod_agrupador".$j];
															
															echo '<tr bgcolor="'.$cor.'" onmouseover="this.style.backgroundColor = \'#ffffcc\';" onmouseout="this.style.backgroundColor = \''.$cor.'\';">';
															echo '<td style="text-align:left; padding: 0 0 0 '.(($j-1) * 10).'px;">'.$seta.' '.$resultado[$i]["cod_agrupador".$j].' '.$resultado[$i]["dsc_agrupador".$j].'</td>';
			
															for($k=1; $k<=count($_REQUEST["agrupadorColunas"]); $k++) {
																echo '<td style="text-align:right;">&nbsp;</td>';
															}
															
															echo '</tr>';
														}
														// imprime o(s) total(is) e o próximo agrupador.
														else {
															
															//totais
															$val = (count($_REQUEST["agrupador"]) - 1);
															for($z=$val; $z>=$j; $z--) {
																echo '<tr style="border-bottom:1px solid black; background-color: #f0f0f0;">';
																echo '<td style="text-align:left;border-bottom:1px solid black;">TOTAL('.$arrayAuxAgrupadores[$z].')</td>';
																
																for($k=1; $k<=count($_REQUEST["agrupadorColunas"]); $k++) {
																	if($arrayTotalColunas[$z][$k] != NULL) {
																		$valor = number_format($arrayTotalColunas[$z][$k], 0, ',', '.' );
																	} else {
																		$valor = " - ";
																	}
																	
																	echo '<td style="text-align:right;border-bottom:1px solid black;">'.$valor.'</td>';
																	
																	$arrayTotalColunas[$z][$k] = 0;
																	$arrayAuxAgrupadores[$z] = NULL;
																}
																
																echo '</tr>';
															}															
															
															// próximo agrupador
															$arrayAuxAgrupadores[$j] = $resultado[$i]["cod_agrupador".$j];
															
															echo '<tr bgcolor="'.$cor.'" onmouseover="this.style.backgroundColor = \'#ffffcc\';" onmouseout="this.style.backgroundColor = \''.$cor.'\';">';
															echo '<td style="text-align:left; padding: 0 0 0 '.(($j-1) * 10).'px;">'.$seta.' '.$resultado[$i]["cod_agrupador".$j].' '.$resultado[$i]["dsc_agrupador".$j].'</td>';
			
															for($k=1; $k<=count($_REQUEST["agrupadorColunas"]); $k++) {
																echo '<td style="text-align:right;">&nbsp;</td>';
															}
															
															echo '</tr>';
														}
													}
												}
											}
										}
										
									} else { ?>
										<tr><td>
										<br/><br/><p style="color: #ff2020;">Nenhum resultado para os parâmetros indicados.</p>
										</td></tr>
									<? } 
								?>
							</thead>
						</table>
					</th>
				</tr>
			</thead>
		</table>
		<script type="text/javascript" language="javascript">
			document.getElementById('aguarde').style.visibility = 'hidden';
			document.getElementById('aguarde').style.display = 'none';
		</script>
	</body>
</html>