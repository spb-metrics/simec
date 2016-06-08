<?php

ini_set( "memory_limit", "2048M" );
set_time_limit(0);


/*
 * DEFINIÇÕES DE CONSTANTES
 */
define('PRF_CONSULTAUNIDADE', 173);

/*
 * FEITO POR ALEXANDRE DOURADO 25/06/2009
 * SOLICITADO POR HENRIQUE XAVIER
 * DESCRIÇÃO DA SOLICITAÇÃO: USUÁRIOS COM PERFIL CONSULTA UNIDADE SOMENTE DEVERÃO ACESSAR INFORMAÇÕES DAS UNIDADES DESTINADAS A ELE
 * DESCRIÇÃO DA ROTINA ABAIXO: PEGANDO AS UNIDADES DO PERFIL CONSULTA UNIDADE (QUANDO FOR CONSULTA UNIDADE)
 */
$pflcod = $db->pegaUm("SELECT pru.pflcod FROM seguranca.perfilusuario pru  
					   INNER JOIN seguranca.perfil prf ON prf.pflcod = pru.pflcod
					   WHERE prf.sisid='".$_SESSION['sisid']."' AND pru.usucpf='".$_SESSION['usucpf']."' 
					   ORDER BY pru.pflcod desc LIMIT 1"); 

switch($pflcod) {
	case PRF_CONSULTAUNIDADE:
		$unicods = $db->carregar("SELECT unicod FROM financeiro.usuarioresponsabilidade WHERE pflcod='".PRF_CONSULTAUNIDADE."' AND usucpf='".$_SESSION['usucpf']."' AND rpustatus='A'");
		if($unicods[0]) {
			foreach($unicods as $uni) {
				$unicods_CONSULTAUNIDADE[] = $uni['unicod'];
			}
		} else {
			echo "<script>
					alert('O perfil não esta associado a nenhuma unidade. Entre em contato com o administrador e solicite a autorização na unidade.');
					window.location='?modulo=relatorio/geral&acao=R';
				  </script>";
			exit;
		}
		break;
}
/*
 * FIM
 * DESCRIÇÃO DA ROTINA ABAIXO: PEGANDO AS UNIDADES DO PERFIL CONSULTA UNIDADE (QUANDO FOR CONSULTA UNIDADE)
 */

// Parâmetros para a nova conexão com o banco do SIAFI
$servidor_bd = $servidor_bd_siafi;
$porta_bd    = $porta_bd_siafi;
$nome_bd     = $nome_bd_siafi;
$usuario_db  = $usuario_db_siafi;
$senha_bd    = $senha_bd_siafi;

// Cria o novo objeto de conexão
$db2 = new cls_banco();

// Parâmetros da nova conexão com o banco do SIAFI para o componente 'combo_popup'.
$dados_conexao = array(
					'servidor_bd' => $servidor_bd_siafi,
					'porta_bd' => $porta_bd_siafi,
					'nome_bd' => $nome_bd_siafi,
					'usuario_db' => $usuario_db_siafi,
					'senha_bd' => $senha_bd_siafi
				);
//Ajax para carregar os detalhes do Agrupador
if($_POST['AjaxDetalhaAgrupador'] == 'true' && $_POST['hidden_agrupadores'] && $_POST['hidden_agrupadorColunas']){

	header('content-type: text/html; charset=ISO-8859-1');
	include APPRAIZ . '/includes/Agrupador.php';
	
	$agrupadores = explode(";",$_POST['hidden_agrupadores']);
	
	$agAntigo = explode(";",$_POST['agrupadorAntigo']);
	
	foreach($agAntigo as $aA){
		if($aA != "undefined")
			array_unshift($agrupadores,$aA);
	}
	
	
	array_unique($agrupadores);
			
	echo "<div id=\"div_exibicao\" class=\"exibe_simples\">";
	
	echo "<div><div class=\"aba_s\" id=\"aba_simples\" onclick=\"exibeAba('simples')\" >Simples</div><div class=\"aba\" id=\"aba_avancado\" onclick=\"exibeAba('avancado')\" >Avançado</div>
	
	<div class=\"aba\" id=\"aba_cor\" style=\"background-color:#dedede\" onclick=\"carregaCores()\" >Cor <input type=\"hidden\" id=\"cor\" name=\"cor\" value=\"#dedede\" />
	</div>
	<div id=\"exibe_cores\" style=\"left:120px;top:-30px;display:none;padding:3px;background-color:#FFFFFF;border:#000000 solid 1px;width:108px;height:40px;position:absolute;z-index:99999999\" >
	
		<div style=\"cursor:pointer;margin:2px;border:#000000 solid 1px;float:left;background-color:#FAEBD7;width:20px;height:12px;\" onclick=\"selecionaCor('#FAEBD7')\" ></div>
		<div style=\"cursor:pointer;margin:2px;border:#000000 solid 1px;float:left;background-color:#FFFACD;width:20px;height:12px;\" onclick=\"selecionaCor('#FFFACD')\" ></div>
		<div style=\"cursor:pointer;margin:2px;border:#000000 solid 1px;float:left;background-color:#e0ffff;width:20px;height:12px;\" onclick=\"selecionaCor('#e0ffff')\" ></div>
		<div style=\"cursor:pointer;margin:2px;border:#000000 solid 1px;float:left;background-color:#E6E6FA;width:20px;height:12px;\" onclick=\"selecionaCor('#E6E6FA')\" ></div>
		<div style=\"cursor:pointer;margin:2px;border:#000000 solid 1px;float:left;background-color:#90EE90;width:20px;height:12px;\" onclick=\"selecionaCor('#90EE90')\" ></div>
		<div style=\"cursor:pointer;margin:2px;border:#000000 solid 1px;float:left;background-color:#CD5C5C;width:20px;height:12px;\" onclick=\"selecionaCor('#CD5C5C')\" ></div>
		<div style=\"cursor:pointer;margin:2px;border:#000000 solid 1px;float:left;background-color:#AFEEEE;width:20px;height:12px;\" onclick=\"selecionaCor('#AFEEEE')\" ></div>
		<div style=\"cursor:pointer;margin:2px;border:#000000 solid 1px;float:left;background-color:#CFCFCF;width:20px;height:12px;\" onclick=\"selecionaCor('#CFCFCF')\" ></div>
	</div>
	
	</div>";
	
	echo "<div style=\"clear:both;margin-top:3px;\"  id=\"div_simples\" >";
	echo "<form name=\"formulario1\" id=\"formulario1\" style=\"width:220px;\" >";
		echo "<select style=\"margin-bottom:3px;\" id=\"agrupador_simples\" name=\"agrupador_simples\" size=\"7\">";
		
		$arrAgrupadores = recuperaAgrupadores();
		asort($arrAgrupadores);
		
		foreach($arrAgrupadores as $m){
				echo "<option ondblclick=\"enviaDetalhe('{$_POST['codigoAgrupador']}','{$_POST['agrupadorCorrente']}','{$_POST['agrupadorAntigo']}','{$_POST['valorAgrupadorAntigo']}');\"  value=\"{$m['codigo']}\" >{$m['descricao']}</option>";
		}
		echo "</select>";
	echo "<div><input type=\"button\" onclick=\"enviaDetalhe('{$_POST['codigoAgrupador']}','{$_POST['agrupadorCorrente']}','{$_POST['agrupadorAntigo']}','{$_POST['valorAgrupadorAntigo']}');\" value=\"Detalhar\" /> <input type=\"button\" onclick=\"fechaDetalhe('{$_POST['codigoAgrupador']}','{$_POST['agrupadorCorrente']}',true)\" value=\"Cancelar\" /></div>";
	echo "</form>";
	echo "<script>
			
					var elSel = document.getElementById('agrupador_simples');
					var i;
					";
			
			foreach($agrupadores as $agrupadorUnset){
				echo "
					  for (i = elSel.length - 1; i>=0; i--) {
					    if (elSel.options[i].value == '$agrupadorUnset') {
					      elSel.remove(i);
					    }
					  }
								";
			}
			
	echo "</script>";
	echo "</div>";
	
	echo "<div style=\"clear:both;margin-top:3px;display:none\" id=\"div_avancado\">";
	echo "<form name=\"formulario\" id=\"formulario\" style=\"width:550px\" >";
				$sql = "SELECT icbcod as codigo,icbdscresumida as descricao FROM financeiro.informacaocontabil ORDER BY descricao";
				$matriz = $db2->carregar($sql);
				$campoAgrupador = new Agrupador( 'formulario' );
				$campoAgrupador->setOrigem( 'agrupadorOrigemColunas', null, recuperaAgrupadores() );
				$campoAgrupador->setDestino( 'agrupadorColunas', null );
				$campoAgrupador->exibir();
			
			echo "<div><input type=\"button\" onclick=\"enviaDetalhe('{$_POST['codigoAgrupador']}','{$_POST['agrupadorCorrente']}','{$_POST['agrupadorAntigo']}','{$_POST['valorAgrupadorAntigo']}');\" value=\"Detalhar\" /> <input type=\"button\" onclick=\"fechaDetalhe('{$_POST['codigoAgrupador']}','{$_POST['agrupadorCorrente']}',true)\" value=\"Cancelar\" /></div>";
			echo "</form>";

			echo "<script>
			
					var elSel = document.getElementById('agrupadorOrigemColunas');
					var i;
					";
			
			foreach($agrupadores as $agrupadorUnset){
				echo "
					  for (i = elSel.length - 1; i>=0; i--) {
					    if (elSel.options[i].value == '$agrupadorUnset') {
					      elSel.remove(i);
					    }
					  }
								";
			}
			
			echo "</script>";
				
			echo "</div>";
			echo "</div>";
	exit;
}

//Ajax para carregar os detalhes do Agrupador
if($_POST['AjaxExibeDadosAgrupados'] == 'true'){
	
	header('content-type: text/html; charset=ISO-8859-1');
	
	$_POST['agrupador'] = $_REQUEST['agrupadorColunas']; //Agrupador escolhido [ + ]
	unset($_REQUEST['agrupadorColunas']);
	
	$_POST['agrupadorColunas'] =  explode(";",$_POST['hidden_agrupadorColunas']); //Código das colunas selecionadas
	
	$hidden_agrupadores = explode(";",$_POST['hidden_agrupadores']); //Agrupadores selecionados na montagem do relatorio
			
	krsort($hidden_agrupadores); //Ordena por chave em ordem decrescente
	
	//Se não for array, transforma em um array de uma posição
	if(!is_array($_POST['agrupador'])){
		$agrp = $_POST['agrupador'];
		unset($_POST['agrupador']);
		$_POST['agrupador'] = array($agrp); 
	}
	
	$arrAgrupador = $_POST['agrupador']; // $arrAgrupador recebe o valor do(s) agrupador(es) escolhido(s)
	
	$agrupadorAntigo = explode(";",$_POST['agrupadorAntigo']); //Agrupadores selecionados com o componentes
	$valorAgrupadorAntigo = explode(";",$_POST['valorAgrupadorAntigo']); //Valores dos agrupadores selecionados com o componentes
	
	krsort($agrupadorAntigo); //Ordena por chave em ordem decrescente
	krsort($valorAgrupadorAntigo); //Ordena por chave em ordem decrescente
	
	//dbg($agrupadorAntigo);
	//dbg($valorAgrupadorAntigo);
	
	foreach($agrupadorAntigo as $agAnt){
		if($agAnt != "undefined")
			array_unshift($hidden_agrupadores,$agAnt);
	}
	
	foreach($hidden_agrupadores as $hiddenAgrupador){
		if(!in_array($hiddenAgrupador,$_POST['agrupador']))	
			array_unshift($_POST['agrupador'],$hiddenAgrupador);
	}
	
	$arrayWhere = $_SESSION['arrayWhere'];
	$arrayTitulo = array();
	$arrayCase = array();//$_SESSION['arrayCase'];
	$arrayJoin = array();//$_SESSION['arrayJoin'];
	$arrayUnionSelects = array();//$_SESSION['arrayUnionSelects'];
	$arrayGroupBy =  array();//$_SESSION['arrayGroupBy'];
	$arraySelectCod =  array();//$_SESSION['arraySelectCod'];
	$arraySelectDsc =  array();//$_SESSION['arraySelectDsc'];
	/*
	 * FEITO POR ALEXANDRE DOURADO 25/06/2009
	 * SOLICITADO POR HENRIQUE XAVIER
	 * DESCRIÇÃO DA ROTINA ABAIXO: ADICIONANDO UM FILTRO POR UNIDADE (SOMENTE AS UNIDADES DESTINADAS NO PERFIL) 
	 */
	if($unicods_CONSULTAUNIDADE) {
		array_push($arrayWhere, "sld.unicod in ('".implode("','",$unicods_CONSULTAUNIDADE)."')");
	}
	/*
	 * FIM DA ROTINA: ADICIONANDO UM FILTRO POR UNIDADE (SOMENTE AS UNIDADES DESTINADAS NO PERFIL)
	 */
	//ver($_POST);
	//ver($hidden_agrupadores);
	//ver($valorAgrupadorAntigo);
	
	//$ag = 0;
	//dbg($agrupadorAntigo);
	//dbg($_POST['agrupador']);
	//foreach($agrupadorAntigo as $agAnt){
	
	for($ag=0; $ag<count($agrupadorAntigo); $ag++){
		if($agrupadorAntigo[$ag] != "undefined"){
			/*** [Ação] ***/
			if($agrupadorAntigo[$ag] == "acacod") {
				array_push($arrayWhere, "sld.acacod in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/Ação] ***/
			
			/*** [Programa] ***/
			if($agrupadorAntigo[$ag] == "programa") {
				array_push($arrayWhere, "sld.prgcod in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/Programa] ***/
			
			/*** [Unidade Gestora] ***/
			if($agrupadorAntigo[$ag] == "ug") {
				array_push($arrayWhere, "sld.ungcod in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/Unidade Gestora] ***/
					
			/*** [Unidade Gestora Responsável] ***/
			if($agrupadorAntigo[$ag] == "ugr") {
				array_push($arrayWhere, "sld.ungcodresp in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/Unidade Gestora Responsável] ***/
			
			/*** [Unidade Orçamentária] ***/
			if($agrupadorAntigo[$ag] == "uo") {
				array_push($arrayWhere, "sld.unicod in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/Unidade Orçamentária] ***/
					
			/*** [Órgão] ***/
			if($agrupadorAntigo[$ag] == "orgao") {
				array_push($arrayWhere, "substr(sld.orgcod, 1, 2) in (substr('".$valorAgrupadorAntigo[$ag]."', 1, 2))");
			}
			/*** [/Órgão] ***/
			
			/*** [Órgão da UG Executora] ***/
			if($agrupadorAntigo[$ag] == "orgaougexecutora") {
				array_push($arrayWhere, "sld.orgcodug in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/Órgão da UG Executora] ***/
			
			/*** [Gestão Executora] ***/
			if($agrupadorAntigo[$ag] == "gestaoexecutora") {
				array_push($arrayWhere, "sld.gescod in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/Gestão Executora] ***/
			
			/*** [Ptres] ***/
			if($agrupadorAntigo[$ag] == "ptres") {
				array_push($arrayWhere, "sld.ptres in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/Ptres] ***/
				
			
			/*** [Funcional] ***/
			if($agrupadorAntigo[$ag] == "funcional") {
				array_push($arrayWhere, "sld.esfcod in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/Funcional] ***/
					
			/*** [Plano Interno] ***/
			if($agrupadorAntigo[$ag] == "planointerno") {
				array_push($arrayWhere, "sld.plicod in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/Plano Interno] ***/
			
			/*** [Grupo UO/UG] ***/
			if($agrupadorAntigo[$ag] == "grupouo") {
				array_push($arrayWhere, "sld.unicod in (select unicod from dw.uguo where guoid in (".$valorAgrupadorAntigo[$ag]."))");
			}
			/*** [/Grupo UO/UG] ***/
					
			/*** [Órgão da UO] ***/
			if($agrupadorAntigo[$ag] == "orgaouo") {
				array_push($arrayWhere, "sld.unicod in (select unicod from dw.uguo where guoid in (".$valorAgrupadorAntigo[$ag]."))");
			}
			/*** [/Órgão da UO] ***/
					
			/*** [Categoria Econômica] ***/
			if($agrupadorAntigo[$ag] == "catecon") {
				array_push($arrayWhere, "sld.ctecod in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/Categoria Econômica] ***/
					
			/*** [Elemento de Despesa] ***/
			if($agrupadorAntigo[$ag] == "elemento") {
				array_push($arrayWhere, "sld.edpcod in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/Elemento de Despesa] ***/
					
			/*** [Sub-Elemento de Despesa] ***/
			if($agrupadorAntigo[$ag] == "subelemento") {
				array_push($arrayWhere, "sld.esfcod in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/Sub-Elemento de Despesa] ***/
					
			/*** [Esfera] ***/
			if($agrupadorAntigo[$ag] == "esfera") {
				array_push($arrayWhere, "sld.esfcod in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/Esfera] ***/
					
			/*** [Fonte SOF] ***/
			if($agrupadorAntigo[$ag] == "fonte") {
				array_push($arrayWhere, "substr(sld.foncod, 2, 3) in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/Fonte SOF] ***/
						
			/*** [Função] ***/
			if($agrupadorAntigo[$ag]== "funcao") {
				array_push($arrayWhere, "sld.funcod in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/Função] ***/
					
			/*** [GND] ***/
			if($agrupadorAntigo[$ag] == "gnd") {
				array_push($arrayWhere, "sld.gndcod in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/GND] ***/
					
			/*** [Grupo de Fonte] ***/
			if($agrupadorAntigo[$ag] == "grf") {
				array_push($arrayWhere, "sld.foncod in (select foscod from dw.fontesiafi where grfid in (".$valorAgrupadorAntigo[$ag]."))");
			}
			/*** [/Grupo de Fonte] ***/
					
			/*** [Modalidade de Aplicação] ***/
			if($agrupadorAntigo[$ag] == "mapcod") {
				array_push($arrayWhere, "sld.mapcod in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/Modalidade de Aplicação] ***/
			
			/*** [Natureza de Despesa] ***/
			if($agrupadorAntigo[$ag] == "natureza") {
				array_push($arrayWhere, "sld.ctecod || sld.gndcod || sld.mapcod || sld.edpcod in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/Natureza de Despesa] ***/
			
			/*** [Natureza de Despesa Detalhada] ***/
			if($agrupadorAntigo[$ag] == "naturezadet") {
				array_push($arrayWhere, "sld.ctecod || sld.gndcod || sld.mapcod || sld.edpcod || sld.sbecod in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/Natureza de Despesa Detalhada] ***/
			
			/*** [Sub-função] ***/
			if($agrupadorAntigo[$ag] == "subfuncao") {
				array_push($arrayWhere, "sld.sfucod in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/Sub-função] ***/
			
			/*** [Fonte Detalhada] ***/
			if($agrupadorAntigo[$ag]== "fontesiafi") {
				array_push($arrayWhere, "sld.foncod in ('".$valorAgrupadorAntigo[$ag]."')");	
			}
			/*** [/Fonte Detalhada] ***/
			
			/*** [Conta Corrente] ***/
			if($agrupadorAntigo[$ag] == "sldcontacorrente") {
				array_push($arrayWhere, "trim(sld.sldcontacorrente) in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/Conta Corrente] ***/
			
			/*** [Recurso] ***/
			if($agrupadorAntigo[$ag] == "recurso") {
				array_push($arrayWhere, "sld.trrcod in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/Recurso] ***/		
			
			/*** [Vinculação de Pagamento] ***/
			if($agrupadorAntigo[$ag] == "vincod") {
				array_push($arrayWhere, "sld.vincod in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/Vinculação de Pagamento] ***/
					
			/*** [Categoria de Gasto] ***/
			if($agrupadorAntigo[$ag] == "cagcod") {
				array_push($arrayWhere, "sld.cagcod in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/Categoria de Gasto] ***/
					
			/*** [Subtítulo] ***/
			if($agrupadorAntigo[$ag] == "loccod") {
				array_push($arrayWhere, "sld.loccod in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/Subtítulo] ***/
					
			/*** [Enquadramento da Despesa] ***/
			if($agrupadorAntigo[$ag] == "enquadramento") {
				array_push($arrayWhere, "substr(sld.plicod, 1, 1) in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/Enquadramento da Despesa] ***/
					
			/*** [Executor Orçamentário e Financeiro] ***/
			if($agrupadorAntigo[$ag] == "executor") {
				array_push($arrayWhere, "substr(sld.plicod, 2, 1) in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/Executor Orçamentário e Financeiro] ***/
			
			/*** [Gestor da Subação] ***/
			if($agrupadorAntigo[$ag] == "gestor") {
				array_push($arrayWhere, "substr(sld.plicod, 3, 1) in ('".$_POST["codigoAgrupador"]."')");
			}
			/*** [/Gestor da Subação] ***/
					
			/*** [Nível/Etapa de Ensino] ***/
			if($agrupadorAntigo[$ag] == "nivel") {
				array_push($arrayWhere, "substr(sld.plicod, 6, 1) in ('".$_POST["codigoAgrupador"]."')");
			}
			/*** [/Nível/Etapa de Ensino] ***/
					
			/*** [Categoria de Apropriação] ***/
			if($agrupadorAntigo[$ag] == "apropriacao") {
				array_push($arrayWhere, "substr(sld.plicod, 7, 2) in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/Modalidade de Ensino] ***/
					
			/*** [Modalidade de Ensino] ***/
			if($agrupadorAntigo[$ag] == "modalidade") {
				array_push($arrayWhere, "substr(sld.plicod, 11, 1) in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/Modalidade de Ensino] ***/
					
			/*** [Sub-Ação] ***/
			if($agrupadorAntigo[$ag] == "subacao") {
				array_push($arrayWhere, "substr(sld.plicod, 2, 4) in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/Sub-Ação] ***/
			
			/*** [Fonte de Recurso] ***/
			if($agrupadorAntigo[$ag] == "fonterecurso") {
				array_push($arrayWhere, "substr(sld.foncod, 3, 2) in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/Fonte de Recurso] ***/
			
			/*** [IDUSO] ***/
			if($agrupadorAntigo[$ag] == "iduso") {
				array_push($arrayWhere, "substr(sld.foncod,1,1) in ('".$valorAgrupadorAntigo[$ag]."')");
			}
			/*** [/IDUSO] ***/
		}
	//$ag++;
	}
	
	//dbg($arrayWhere);
	//dbg($valorAgrupadorAntigo);
	//die;
	
	//Retirar CodigoAgrupador e agrupadorCorrente
	
	/*** [Ação] ***/
	if($_POST['agrupadorCorrente'] == "acacod") {
		array_push($arrayWhere, "sld.acacod in ('".$_POST['codigoAgrupador']."')");
	}
	/*** [/Ação] ***/
	
	/*** [Programa] ***/
	if($_POST['agrupadorCorrente'] == "programa") {
		array_push($arrayWhere, "sld.prgcod in ('".$_POST['codigoAgrupador']."')");
	}
	/*** [/Programa] ***/
	
	/*** [Unidade Gestora] ***/
	if($_POST['agrupadorCorrente'] == "ug") {
		array_push($arrayWhere, "sld.ungcod in ('".$_POST['codigoAgrupador']."')");
	}
	/*** [/Unidade Gestora] ***/
	
	/*** [Unidade Gestora Responsável] ***/
	if($agrupadorAntigo[$ag] == "ugr") {
		array_push($arrayWhere, "sld.ungcodresp in ('".$valorAgrupadorAntigo[$ag]."')");
	}
	/*** [/Unidade Gestora Responsável] ***/
			
	/*** [Unidade Orçamentária] ***/
	if($_POST['agrupadorCorrente'] == "uo") {
		array_push($arrayWhere, "sld.unicod in ('".$_POST['codigoAgrupador']."')");
	}
	/*** [/Unidade Orçamentária] ***/
			
	/*** [Órgão] ***/
	if($_POST['agrupadorCorrente'] == "orgao") {
		array_push($arrayWhere, "substr(sld.orgcod, 1, 2) in (substr('".$_POST['codigoAgrupador']."', 1, 2))");
	}
	/*** [/Órgão] ***/
	
	/*** [Ptres] ***/
	if($_POST['agrupadorCorrente'] == "ptres") {
		array_push($arrayWhere, "sld.ptres in ('".$_POST['codigoAgrupador']."')");
	}
	/*** [/Ptres] ***/
			
	/*** [Funcional] ***/
	if($_POST['agrupadorCorrente'] == "funcional") {
		array_push($arrayWhere, "sld.esfcod in ('".$_POST['codigoAgrupador']."')");
	}
	/*** [/Funcional] ***/
			
	/*** [Plano Interno] ***/
	if($_POST['agrupadorCorrente'] == "planointerno") {
		array_push($arrayWhere, "sld.plicod in ('".$_POST['codigoAgrupador']."')");
	}
	/*** [/Plano Interno] ***/
	
	/*** [Grupo UO/UG] ***/
	if($_POST['agrupadorCorrente'] == "grupouo") {
		array_push($arrayWhere, "sld.unicod in (select unicod from dw.uguo where guoid in ('".$_POST['codigoAgrupador']."'))");
	}
	/*** [/Grupo UO/UG] ***/
			
	/*** [Órgão da UO] ***/
	if($_POST['agrupadorCorrente'] == "orgaouo") {
		array_push($arrayWhere, "sld.unicod in (select unicod from dw.uguo where guoid in (".$_POST['codigoAgrupador']."))");
	}
	/*** [/Órgão da UO] ***/
			
	/*** [Categoria Econômica] ***/
	if($_POST['agrupadorCorrente'] == "catecon") {
		array_push($arrayWhere, "sld.ctecod in ('".$_POST['codigoAgrupador']."')");
	}
	/*** [/Categoria Econômica] ***/
			
	/*** [Elemento de Despesa] ***/
	if($_POST['agrupadorCorrente'] == "elemento") {
		array_push($arrayWhere, "sld.edpcod in ('".$_POST['codigoAgrupador']."')");
	}
	/*** [/Elemento de Despesa] ***/
			
	/*** [Sub-Elemento de Despesa] ***/
	if($_POST['agrupadorCorrente'] == "subelemento") {
		array_push($arrayWhere, "sld.esfcod in ('".$_POST['codigoAgrupador']."')");
	}
	/*** [/Sub-Elemento de Despesa] ***/
			
	/*** [Esfera] ***/
	if($_POST['agrupadorCorrente'] == "esfera") {
		array_push($arrayWhere, "sld.esfcod in ('".$_POST['codigoAgrupador']."')");
	}
	/*** [/Esfera] ***/
			
	/*** [Fonte SOF] ***/
	if($_POST['agrupadorCorrente'] == "fonte") {
		array_push($arrayWhere, "substr(sld.foncod, 2, 3) in ('".$_POST['codigoAgrupador']."')");
	}
	/*** [/Fonte SOF] ***/
				
	/*** [Função] ***/
	if($_POST['agrupadorCorrente'] == "funcao") {
		array_push($arrayWhere, "sld.funcod in ('".$_POST['codigoAgrupador']."')");
	}
	/*** [/Função] ***/
			
	/*** [GND] ***/
	if($_POST['agrupadorCorrente'] == "gnd") {
		array_push($arrayWhere, "sld.gndcod in ('".$_POST['codigoAgrupador']."')");
	}
	/*** [/GND] ***/
			
	/*** [Grupo de Fonte] ***/
	if($_POST['agrupadorCorrente'] == "grf") {
		array_push($arrayWhere, "sld.foncod in (select foscod from dw.fontesiafi where grfid in (".$_POST['codigoAgrupador']."))");
	}
	/*** [/Grupo de Fonte] ***/
			
	/*** [Modalidade de Aplicação] ***/
	if($_POST['agrupadorCorrente'] == "mapcod") {
		array_push($arrayWhere, "sld.mapcod in ('".$_POST['codigoAgrupador']."')");
	}
	/*** [/Modalidade de Aplicação] ***/
	
	/*** [Natureza de Despesa] ***/
	if($_POST['agrupadorCorrente'] == "natureza") {
		array_push($arrayWhere, "sld.ctecod || sld.gndcod || sld.mapcod || sld.edpcod in ('".$_POST['codigoAgrupador']."')");
	}
	/*** [/Natureza de Despesa] ***/
	
	/*** [Natureza de Despesa Detalhada] ***/
	if($_POST['agrupadorCorrente'] == "naturezadet") {
		array_push($arrayWhere, "sld.ctecod || sld.gndcod || sld.mapcod || sld.edpcod || sld.sbecod in ('".$_POST['codigoAgrupador']."')");
	}
	/*** [/Natureza de Despesa Detalhada] ***/

	/*** [Sub-função] ***/
	if($_POST['agrupadorCorrente'] == "subfuncao") {
		array_push($arrayWhere, "sld.sfucod in ('".$_POST['codigoAgrupador']."')");
	}
	/*** [/Sub-função] ***/
	
	/*** [Fonte SIAFI] ***/
	if($_POST['agrupadorCorrente'] == "fontesiafi") {
		array_push($arrayWhere, "sld.foncod in ('".$_POST['codigoAgrupador']."')");	
	}
	/*** [/Fonte SIAFI] ***/
	
	/*** [Conta Corrente] ***/
	if($_POST['agrupadorCorrente'] == "sldcontacorrente") {
		array_push($arrayWhere, "trim(sld.sldcontacorrente) in ('".$_POST['codigoAgrupador']."')");
	}
	/*** [/Conta Corrente] ***/
	
	/*** [Modalidade de Aplicação] ***/
	if($_POST['agrupadorCorrente'] == "modlic") {
		array_push($arrayWhere, "trim(sld.modlic) in ('".$_POST['codigoAgrupador']."')");
	}
	/*** [/Modalidade de Aplicação] ***/
	
	
	/*** [Recurso] ***/
	if($_POST['agrupadorCorrente'] == "recurso") {
		array_push($arrayWhere, "sld.trrcod in ('".$_POST['codigoAgrupador']."')");
	}
	/*** [/Recurso] ***/		
	
	/*** [Vinculação de Pagamento] ***/
	if($_POST['agrupadorCorrente'] == "vincod") {
		array_push($arrayWhere, "sld.vincod in ('".$_POST['codigoAgrupador']."')");
	}
	/*** [/Vinculação de Pagamento] ***/
			
	/*** [Categoria de Gasto] ***/
	if($_POST['agrupadorCorrente'] == "cagcod") {
		array_push($arrayWhere, "sld.cagcod in ('".$_POST['codigoAgrupador']."')");
	}
	/*** [/Categoria de Gasto] ***/
			
	/*** [Subtítulo] ***/
	if($_POST['agrupadorCorrente'] == "loccod") {
		array_push($arrayWhere, "sld.loccod in ('".$_POST['codigoAgrupador']."')");
	}
	/*** [/Subtítulo] ***/
			
	/*** [Enquadramento da Despesa] ***/
	if($_POST['agrupadorCorrente'] == "enquadramento") {
		array_push($arrayWhere, "substr(sld.plicod, 1, 1) in ('".$_POST['codigoAgrupador']."')");
	}
	/*** [/Enquadramento da Despesa] ***/
			
	/*** [Executor Orçamentário e Financeiro] ***/
	if($_POST['agrupadorCorrente'] == "executor") {
		array_push($arrayWhere, "substr(sld.plicod, 2, 1) in ('".$_POST['codigoAgrupador']."')");
	}
	/*** [/Executor Orçamentário e Financeiro] ***/
	
	/*** [Gestor da Subação] ***/
	if($_POST['agrupadorCorrente'] == "gestor") {
		array_push($arrayWhere, "substr(sld.plicod, 3, 1) in ('".$_POST["codigoAgrupador"]."')");
	}
	/*** [/Gestor da Subação] ***/
			
	/*** [Nível/Etapa de Ensino] ***/
	if($_POST['agrupadorCorrente'] == "nivel") {
		array_push($arrayWhere, "substr(sld.plicod, 6, 1) in ('".$_POST["codigoAgrupador"]."')");
	}
	/*** [/Nível/Etapa de Ensino] ***/
			
	/*** [Categoria de Apropriação] ***/
	if($_POST['agrupadorCorrente'] == "apropriacao") {
		array_push($arrayWhere, "substr(sld.plicod, 7, 2) in ('".$_POST['codigoAgrupador']."')");
	}
	/*** [/Modalidade de Ensino] ***/
			
	/*** [Modalidade de Ensino] ***/
	if($_POST['agrupadorCorrente'] == "modalidade") {
		array_push($arrayWhere, "substr(sld.plicod, 11, 1) in ('".$_POST['codigoAgrupador']."')");
	}
	/*** [/Modalidade de Ensino] ***/
			
	/*** [Sub-Ação] ***/
	if($_POST['agrupadorCorrente'] == "subacao") {
		array_push($arrayWhere, "substr(sld.plicod, 2, 4) in ('".$_POST['codigoAgrupador']."')");
	}
	/*** [/Sub-Ação] ***/
	
	/*** [Fonte de Recurso] ***/
	if($_POST['agrupadorCorrente'] == "fonterecurso") {
		array_push($arrayWhere, "substr(sld.foncod, 3, 2) in ('".$_POST['codigoAgrupador']."')");
	}
	/*** [/Fonte de Recurso] ***/
	
	/*** [IDUSO] ***/
	if($_POST['agrupadorCorrente'] == "iduso") {
		array_push($arrayWhere, "substr(sld.foncod,1,1) in ('".$_POST['codigoAgrupador']."')");
	}
	/*** [/IDUSO] ***/

	//$_POST["agrupador"] = array_unique($_POST["agrupador"]);
	//dbg($_POST["agrupador"]);
	//die;
			
	for($i=0; $i<count($_POST["agrupador"]); $i++) {
		switch($_POST["agrupador"][$i]) {
			case "acacod":
				
				array_push($arrayTitulo, "Ação");
				
				array_push($arraySelectCod, "sld.acacod");
				array_push($arraySelectDsc, "aca.acadsc");
				
				array_push($arrayJoin, "LEFT JOIN dw.acao aca ON aca.acacod = sld.acacod");
				
				array_push($arrayGroupBy, "sld.acacod");
				array_push($arrayGroupBy, "aca.acadsc");
				
				break;
				
			case "catecon":

				array_push($arrayTitulo, "Categoria Econômica");
				
				array_push($arraySelectCod, "sld.ctecod");
				array_push($arraySelectDsc, "cte.ctedsc");
				
				array_push($arrayJoin, "LEFT JOIN dw.categoriaeconomica cte ON CAST(cte.ctecod AS text) = sld.ctecod");
				
				array_push($arrayGroupBy, "sld.ctecod");
				array_push($arrayGroupBy, "cte.ctedsc");
				
				break;
				
			case "elemento":
				
				array_push($arrayTitulo, "Elemento de Despesa");
								
				array_push($arraySelectCod, "sld.edpcod");
				array_push($arraySelectDsc, "edp.edpdsc");
				
				array_push($arrayJoin, "LEFT JOIN dw.elementodespesa edp ON edp.edpcod = sld.edpcod");
				
				array_push($arrayGroupBy, "sld.edpcod");
				array_push($arrayGroupBy, "edp.edpdsc");
				
				break;
				
			case "subelemento":
				
				array_push($arrayTitulo, "Sub-Elemento de Despesa");
	
				array_push($arraySelectCod, "sbe.sbecod");
				array_push($arraySelectDsc, "sbe.ndpdsc");
				
				array_push($arrayJoin, "LEFT JOIN ( select distinct ctecod || '.' || gndcod || '.' || mapcod || '.' || edpcod || '.' || sbecod as sbecod, ndpdsc from dw.naturezadespesa ) as sbe ON sbe.sbecod = sld.ctecod || '.' || sld.gndcod || '.' || coalesce(sld.mapcod,'00') || '.' || coalesce(sld.edpcod,'00') || '.' || coalesce(sld.sbecod,'00')");
				
				array_push($arrayGroupBy, "sbe.sbecod");
				array_push($arrayGroupBy, "sbe.ndpdsc");
	
				break;
	
			
			case "esfera": 
				
				array_push($arrayTitulo, "Esfera");
				
				array_push($arraySelectCod, "sld.esfcod");
				array_push($arraySelectDsc, "esf.esfdsc");
				
				array_push($arrayJoin, "LEFT JOIN dw.esfera esf ON substr(CAST(esf.esfcod AS text),1,1) = sld.esfcod");
				
				array_push($arrayGroupBy, "sld.esfcod");
				array_push($arrayGroupBy, "esf.esfdsc");
				
				break;
				
			case "fontesiafi":
				
				array_push($arrayTitulo, "Fonte Detalhada");
				
				array_push($arraySelectCod, "fsf.foscod");
				array_push($arraySelectDsc, "fsf.fosdsc");
				
				array_push($arrayJoin, "LEFT JOIN dw.fontesiafi fsf ON fsf.foscod = sld.foncod");
				
				array_push($arrayGroupBy, "fsf.foscod");
				array_push($arrayGroupBy, "fsf.fosdsc");
				
				break;
	
			case "fonte":
				
				array_push($arrayTitulo, "Fonte");
				
				array_push($arraySelectCod, "fon.foncod");
				array_push($arraySelectDsc, "fon.fondsc");
				
				array_push($arrayJoin, "LEFT JOIN dw.fontesof fon ON fon.foncod = substr(sld.foncod,2,3)");
				
				array_push($arrayGroupBy, "sld.foncod");
				array_push($arrayGroupBy, "fon.fondsc");
				
				break;
				
			case "funcao":
				
				array_push($arrayTitulo, "Função");
				
				array_push($arraySelectCod, "sld.funcod");
				array_push($arraySelectDsc, "fun.fundsc");
				
				array_push($arrayJoin, "LEFT JOIN dw.funcao fun ON fun.funcod = sld.funcod");
				
				array_push($arrayGroupBy, "sld.funcod");
				array_push($arrayGroupBy, "fun.fundsc");
				
				break;
				
			case "gnd":
				
				array_push($arrayTitulo, "GND");
				
				array_push($arraySelectCod, "sld.gndcod");
				array_push($arraySelectDsc, "gnd.gnddsc");
				
				array_push($arrayJoin, "LEFT JOIN dw.gnd gnd ON cast(gnd.gndcod AS text) = sld.gndcod");
				
				array_push($arrayGroupBy, "sld.gndcod");
				array_push($arrayGroupBy, "gnd.gnddsc");
				
				break;
				
			case "grf":
				
				array_push($arrayTitulo, "Grupo de Fonte");
				
				array_push($arraySelectCod, "grf.grfid");
				array_push($arraySelectDsc, "grf.grfdsc");
				
				array_push($arrayJoin, "LEFT JOIN (select fs.foscod, gf.grfid, gf.grfdsc from dw.fontesiafi fs inner join dw.grupofonte gf on gf.grfid = fs.grfid) as grf ON grf.foscod = sld.foncod");
				
				array_push($arrayGroupBy, "grf.grfid");
				array_push($arrayGroupBy, "grf.grfdsc");
				
				break;
				
			case "grupouo":
				
				array_push($arrayTitulo, "Grupo de Unidade Orçamentária");
				
				array_push($arraySelectCod, "gun.guoid");
				array_push($arraySelectDsc, "gun.guodsc");
				
				array_push($arrayJoin, "LEFT JOIN (select uo.unicod, gu.guoid, gu.guodsc from dw.uguo uo inner join dw.grupouo gu on gu.guoid = uo.guoid group by uo.unicod, gu.guoid, gu.guodsc) as gun ON gun.unicod = sld.unicod");
				
				array_push($arrayGroupBy, "gun.guoid");
				array_push($arrayGroupBy, "gun.guodsc");
				
				break;
	
	
			case "orgaouo":
				
				array_push($arrayTitulo, "Orgão da UO");
				
				array_push($arraySelectCod, "gun.unicod");
				array_push($arraySelectDsc, "gun.unidsc");
				
				array_push($arrayJoin, "LEFT JOIN (select uo.orgcoduo as unicod, uo.ugonome||' ('||uo.ugonomeabrev||')' as unidsc from dw.uguo uo group by uo.orgcoduo, uo.ugonome, uo.ugonomeabrev) as gun ON gun.unicod = sld.unicod");
				
				array_push($arrayGroupBy, "gun.unicod");
				array_push($arrayGroupBy, "gun.unidsc");
				
				break;
				
			case "mapcod":
				
				array_push($arrayTitulo, "Modalidade da Aplicação");
				
				array_push($arraySelectCod, "sld.mapcod");
				array_push($arraySelectDsc, "map.mapdsc");
				
				array_push($arrayJoin, "LEFT JOIN dw.modalidadeaplicacao map ON map.mapcod = sld.mapcod");
				
				array_push($arrayGroupBy, "sld.mapcod");
				array_push($arrayGroupBy, "map.mapdsc");
				
				break;
				
			case "natureza":
				
				array_push($arrayTitulo, "Natureza de Despesa");
				
				array_push($arraySelectCod, "sld.ctecod || '.' || sld.gndcod || '.' || sld.mapcod || '.' || sld.edpcod AS natureza");
				array_push($arraySelectDsc, "ndp.ndpdsc AS natureza_desc");
				
				array_push($arrayJoin, "LEFT JOIN ( select distinct ctecod, gndcod, mapcod, edpcod, ndpdsc from dw.naturezadespesa where sbecod = '00' ) as ndp ON cast(ndp.ctecod AS text) = sld.ctecod AND cast(ndp.gndcod AS text) = sld.gndcod AND cast(ndp.mapcod AS text) = sld.mapcod AND cast(ndp.edpcod AS text) = sld.edpcod");
				
				array_push($arrayGroupBy, "natureza");
				array_push($arrayGroupBy, "natureza_desc");
				
				break;
				
			case "naturezadet":
				
				array_push($arrayTitulo, "Natureza de Despesa Detalhada");
				
				array_push($arraySelectCod, "sld.ctecod || '.' || sld.gndcod || '.' || sld.mapcod || '.' || sld.edpcod || '.' || sld.sbecod AS naturezadet");
				array_push($arraySelectDsc, "ndp.ndpdsc AS naturezadet_desc");
				
				array_push($arrayJoin, "LEFT JOIN ( select distinct ctecod, gndcod, mapcod, edpcod, sbecod, ndpdsc from dw.naturezadespesa where sbecod <> '00' ) as ndp ON cast(ndp.ctecod AS text) = sld.ctecod AND cast(ndp.gndcod AS text) = sld.gndcod AND cast(ndp.mapcod AS text) = sld.mapcod AND cast(ndp.edpcod AS text) = sld.edpcod AND cast(ndp.sbecod AS text) = sld.sbecod");
				
				array_push($arrayGroupBy, "naturezadet");
				array_push($arrayGroupBy, "naturezadet_desc");
				
				break;

			case "programa":
				
				array_push($arrayTitulo, "Programa");
				
				array_push($arraySelectCod, "sld.prgcod");
				array_push($arraySelectDsc, "prg.prgdsc");
				
				array_push($arrayJoin, "LEFT JOIN dw.programa prg ON prg.prgcod = sld.prgcod");
				
				array_push($arrayGroupBy, "sld.prgcod");
				array_push($arrayGroupBy, "prg.prgdsc");
				
				break;
				
			case "subfuncao":
				
				array_push($arrayTitulo, "Sub-Função");
				
				array_push($arraySelectCod, "sld.sfucod");
				array_push($arraySelectDsc, "sfu.sfudsc");
				
				array_push($arrayJoin, "LEFT JOIN dw.subfuncao sfu ON sfu.sfucod = sld.sfucod");
				
				array_push($arrayGroupBy, "sld.sfucod");
				array_push($arrayGroupBy, "sfu.sfudsc");
				
				break;
				
			case "ug":
				
				array_push($arrayTitulo, "Unidade Gestora");
				
				array_push($arraySelectCod, "sld.ungcod");
				array_push($arraySelectDsc, "ung.ungdsc");
				
				array_push($arrayJoin, "LEFT JOIN dw.ug ung ON ung.ungcod = sld.ungcod");
				
				array_push($arrayGroupBy, "sld.ungcod");
				array_push($arrayGroupBy, "ung.ungdsc");
				
				break;
			
			case "ugr":
				
				array_push($arrayTitulo, "Unidade Gestora Responsável");
			
				array_push($arraySelectCod, "sld.ungcodresp");
				array_push($arraySelectDsc, "ung2.ungdsc as ungdsc2");
			
				array_push($arrayJoin, "LEFT JOIN dw.ug ung2 ON ung2.ungcod = sld.ungcodresp");
			
				array_push($arrayGroupBy, "sld.ungcodresp");
				array_push($arrayGroupBy, "ungdsc2");
				
				break;
				
			case "uo":
				
				array_push($arrayTitulo, "Unidade Orçamentária");
				
				array_push($arraySelectCod, "sld.unicod");
				array_push($arraySelectDsc, "uni.unidsc");
				
				array_push($arrayJoin, "LEFT JOIN dw.uo uni ON uni.unicod = sld.unicod");
				
				array_push($arrayGroupBy, "sld.unicod");
				array_push($arrayGroupBy, "uni.unidsc");
				
				break;
			
			case "orgao":
				
				array_push($arrayTitulo, "Orgão");
				
				array_push($arraySelectCod, "ors.orscod");
				array_push($arraySelectDsc, "ors.orsdsc");
				
				array_push($arrayJoin, "LEFT JOIN dw.orgaosuperior ors ON substr(ors.orscod, 1, 2) = substr(sld.orgcod, 1, 2)");
				
				array_push($arrayGroupBy, "ors.orscod");
				array_push($arrayGroupBy, "ors.orsdsc");
				
				break;
				
			case "ptres":
				
				array_push($arrayTitulo, "Ptres");
				
				array_push($arraySelectCod, "sld.ptres AS ptres");
				array_push($arraySelectDsc, "sld.ptres AS ptres_desc");
				
				array_push($arrayGroupBy, "ptres");
				array_push($arrayGroupBy, "ptres_desc");
				
				break;
				
			case "funcional":
				
				array_push($arrayTitulo, "Funcional");
				
				array_push($arraySelectCod, "sld.prgcod || '.' || sld.acacod || '.' || sld.unicod || '.' || sld.loccod || ' - ' || aca2.acadsc || '(' || sld.loccod || ')' AS funcional");
				array_push($arraySelectDsc, "sld.prgcod || '.' || sld.acacod || '.' || sld.unicod || '.' || sld.loccod || ' - ' || aca2.acadsc || '(' || sld.loccod || ')' AS funcional_desc");
				
				array_push($arrayJoin, "LEFT JOIN dw.acao aca2 ON sld.acacod = aca2.acacod");
				
				array_push($arrayGroupBy, "funcional");
				array_push($arrayGroupBy, "funcional_desc");
				
				break;
				
			case "planointerno":
				
				array_push($arrayTitulo, "Plano Interno");
				
				array_push($arraySelectCod, "sld.plicod");
				array_push($arraySelectDsc, "pli.plidsc");
				
				array_push($arrayJoin, "LEFT JOIN dw.planointerno pli ON pli.plicod = sld.plicod and pli.unicod = sld.unicod ");
				array_push($arrayWhere, "length(sld.plicod) = 11");
				
				array_push($arrayGroupBy, "sld.plicod");
				array_push($arrayGroupBy, "pli.plidsc");
				
				break;
				
			case "sldcontacorrente":
				
				array_push($arrayTitulo, "Conta Corrente");
				
				array_push($arraySelectCod, "sld.sldcontacorrente AS conta_corrente");
				array_push($arraySelectDsc, "sld.sldcontacorrente AS conta_corrente_desc");
				
				array_push($arrayGroupBy, "conta_corrente");
				array_push($arrayGroupBy, "conta_corrente_desc");
				
				break;
			
			case "recurso":
				
				array_push($arrayTitulo, "Recurso");
				
				array_push($arraySelectCod, "sld.trrcod");
				array_push($arraySelectDsc, "trr.trrdsc");
				
				array_push($arrayJoin, "LEFT JOIN dw.tiporecurso trr ON cast(trr.trrcod AS text) = sld.trrcod");
				
				array_push($arrayGroupBy, "sld.trrcod");
				array_push($arrayGroupBy, "trr.trrdsc");
				
				break;
				
			case "vincod":
				
				array_push($arrayTitulo, "Vinculação de Pagamento");
				
				array_push($arraySelectCod, "sld.vincod AS vincod");
				array_push($arraySelectDsc, "vp.it_no_vinculacao_pagamento AS vincod_desc");

				array_push($arrayJoin, "LEFT JOIN dw.vinculacaopagamento vp ON vp.it_co_vinculacao_pagamento = sld.vincod");
				
				array_push($arrayGroupBy, "sld.vincod");
				array_push($arrayGroupBy, "vp.it_no_vinculacao_pagamento");
				
				break;
				
			case "cagcod":
				
				array_push($arrayTitulo, "Categoria de Gasto");
				
				array_push($arraySelectCod, "sld.cagcod");
				array_push($arraySelectDsc, "cag.cagdsc");
				
				array_push($arrayJoin, "LEFT JOIN dw.categoriagasto cag ON cag.cagcod = sld.cagcod");
				
				array_push($arrayGroupBy, "sld.cagcod");
				array_push($arrayGroupBy, "cag.cagdsc");
				
				break;
				
			case "loccod":
				
				array_push($arrayTitulo, "Subtítulo");
				
				array_push($arraySelectCod, "sld.loccod AS loccod");
				array_push($arraySelectDsc, "sld.loccod AS loccod_desc");
				
				array_push($arrayGroupBy, "loccod");
				array_push($arrayGroupBy, "loccod_desc");
				
				break;
				
			case "enquadramento":
				
				array_push($arrayTitulo, "Enquadramento de Despesa");
				
				array_push($arraySelectCod, "ct1.cdtcod AS enquadramento_cod");
				array_push($arraySelectDsc, "ct1.cdtdsc AS enquadramento_dsc");
				
				array_push($arrayJoin, "LEFT JOIN public.combodadostabela ct1 ON ct1.ctbid = 5 AND ct1.cdtstatus = 'A' AND ct1.cdtcod = substr(sld.plicod, 1, 1)");
				
				array_push($arrayGroupBy, "enquadramento_cod");
				array_push($arrayGroupBy, "enquadramento_dsc");
				
				break;
				
			case "executor":
				
				array_push($arrayTitulo, "Executor Orçamentário e Financeiro");
				
				array_push($arraySelectCod, "ct2.cdtcod AS executor_cod");
				array_push($arraySelectDsc, "ct2.cdtdsc AS executor_dsc");
				
				array_push($arrayJoin, "LEFT JOIN public.combodadostabela ct2 ON ct2.ctbid = 3 AND ct2.cdtstatus = 'A' AND ct2.cdtcod = substr(sld.plicod, 2, 1)");
				
				array_push($arrayGroupBy, "executor_cod");
				array_push($arrayGroupBy, "executor_dsc");
				
				break;
				
			case "gestor":
				
				array_push($arrayTitulo, "Gestor da Subação");
				
				array_push($arraySelectCod, "ct3.cdtcod AS gestor_cod");
				array_push($arraySelectDsc, "ct3.cdtdsc AS gestor_dsc");
				
				array_push($arrayJoin, "LEFT JOIN public.combodadostabela ct3 ON ct3.ctbid = 4 AND ct3.cdtstatus = 'A' AND ct3.cdtcod = substr(sld.plicod, 3, 1)");
				
				array_push($arrayGroupBy, "gestor_cod");
				array_push($arrayGroupBy, "gestor_dsc");
				
				break;
				
			case "nivel":
				
				array_push($arrayTitulo, "Nivel/Etapa de Ensino");
				
				array_push($arraySelectCod, "ct4.cdtcod AS nivel_cod");
				array_push($arraySelectDsc, "ct4.cdtdsc AS nivel_dsc");
				
				array_push($arrayJoin, "LEFT JOIN public.combodadostabela ct4 ON ct4.ctbid = 6 AND ct4.cdtstatus = 'A' AND ct4.cdtcod = substr(sld.plicod, 6, 1)");
				
				array_push($arrayGroupBy, "nivel_cod");
				array_push($arrayGroupBy, "nivel_dsc");
				
				break;
				
			case "apropriacao":
				
				array_push($arrayTitulo, "Categoria de Apropriação");
				
				array_push($arraySelectCod, "ct5.cdtcod AS apropriacao_cod");
				array_push($arraySelectDsc, "ct5.cdtdsc AS apropriacao_dsc");
				
				array_push($arrayJoin, "LEFT JOIN public.combodadostabela ct5 ON ct5.ctbid = 7 AND ct5.cdtstatus = 'A' AND ct5.cdtcod = substr(sld.plicod, 7, 2)");
				
				array_push($arrayGroupBy, "apropriacao_cod");
				array_push($arrayGroupBy, "apropriacao_dsc");
				
				break;
				
			case "modalidade":
				
				array_push($arrayTitulo, "Modalidade de Ensino");
				
				array_push($arraySelectCod, "ct6.cdtcod AS modalidade_cod");
				array_push($arraySelectDsc, "ct6.cdtdsc AS modalidade_dsc");
				
				array_push($arrayJoin, "LEFT JOIN public.combodadostabela ct6 ON ct6.ctbid = 8 AND ct6.cdtstatus = 'A' AND ct6.cdtcod = substr(sld.plicod, 11, 1)");
				
				array_push($arrayGroupBy, "modalidade_cod");
				array_push($arrayGroupBy, "modalidade_dsc");
				
				break;
				
			case "subacao":
				
				array_push($arrayTitulo, "Subação");
				
				array_push($arraySelectCod, "sac.sbacod");
				array_push($arraySelectDsc, "sac.sbatitulo");
				
				array_push($arrayJoin, "INNER JOIN financeiro.subacao sac ON sac.sbastatus = 'A' AND sac.sbacod = substr(sld.plicod, 2, 4)");
				
				array_push($arrayGroupBy, "sac.sbacod");
				array_push($arrayGroupBy, "sac.sbatitulo");
				
				break;
				
			case "orgaougexecutora":
			
				array_push($arrayTitulo, "Orgão da UG Executora");
				
				array_push($arraySelectCod, "oge.orgcod");
				array_push($arraySelectDsc, "oge.orgdsc");
				
				array_push($arrayJoin, "LEFT JOIN dw.orgao oge ON oge.orgcod = sld.orgcodug");
				
				array_push($arrayGroupBy, "oge.orgcod");
				array_push($arrayGroupBy, "oge.orgdsc");
				
				break;
				
			case "gestaoexecutora":
			
				array_push($arrayTitulo, "Gestão Executora");
				
				array_push($arraySelectCod, "gse.gstcod");
				array_push($arraySelectDsc, "gse.gstdsc");
				
				array_push($arrayJoin, "LEFT JOIN dw.gestao gse ON gse.gstcod = sld.gescod");
				
				array_push($arrayGroupBy, "gse.gstcod");
				array_push($arrayGroupBy, "gse.gstdsc");
				
				break;
				
			case "modlic":
			
				array_push($arrayTitulo, "Modalidade de Licitação");
				
				array_push($arraySelectCod, "ml.mdlcod");
				array_push($arraySelectDsc, "ml.mdldsc");
				
				array_push($arrayJoin, "LEFT JOIN dw.modalidadelicitacao ml ON ml.mdlcod = sld.modlic");
				
				array_push($arrayGroupBy, "ml.mdlcod");
				array_push($arrayGroupBy, "ml.mdldsc");
				
				break;
								
			case 'fonterecurso':
			
				array_push($arrayTituloAgrupadores, "Fonte de Recurso");
					
				array_push($arraySelectCod, "ftr.codigo");
				array_push($arraySelectDsc, "ftr.descricao");
				
				array_push($arrayJoin, "LEFT JOIN dw.fonterecursos ftr ON ftr.codigo = substr(sld.foncod,3,2)");
				
				array_push($arrayGroupBy, "ftr.codigo");
				array_push($arrayGroupBy, "ftr.descricao");
				
				break;	
		
			case 'iduso':
			
				array_push($arrayTituloAgrupadores, "IDUSO");
					
				array_push($arraySelectCod, "idf.iducod");
				array_push($arraySelectDsc, "idf.idudsc");
				
				array_push($arrayJoin, "LEFT JOIN dw.identifuso idf ON idf.iducod = substr(sld.foncod,1,1)");
				
				array_push($arrayGroupBy, "idf.iducod");
				array_push($arrayGroupBy, "idf.idudsc");
				
				break;
		}
	}
		
		
	$arraySum = array();
	$arraySumWhere = array();
	$inContasTodos = array();

	// Pega valores referente ao mês de filtro
	$camposMes = "sld.sldvalor";
	if( count($_POST['mes']) && $_POST['mes'][0] ){
		$camposMes .= sprintf("%02s", array_shift($_POST['mes']) );
		while( current($_POST['mes']) ){
			$camposMes .= " + sld.sldvalor" . sprintf("%02d", current($_POST['mes']));
			next($_POST['mes']);
		}
	}	
	
	for($i=0; $i<count($_POST["agrupadorColunas"]); $i++) {
		$inContas = array();
		
		$arrayContas = $db2->carregar("SELECT conconta FROM financeiro.informacaoconta WHERE icbcod = ".$_POST["agrupadorColunas"][$i]);
		
		for($j=0; $j<count($arrayContas); $j++) {
			array_push($inContas, "'".$arrayContas[$j]["conconta"]."'");
			array_push($inContasTodos, "'".$arrayContas[$j]["conconta"]."'");
		}
		
		$case = "CASE WHEN sld.sldcontacontabil in (".implode(',', $inContas).") THEN ($camposMes) ELSE 0 END AS valor".($i+1);
		array_push($arrayCase, $case);
		array_push($arraySum, 'sum(valor'.($i+1).') AS coluna'.($i+1));
		array_push($arraySumWhere, 'valor'.($i+1).' <> 0');
	}
	
	array_push($arrayWhere, "sld.sldcontacontabil in (".implode(',', $inContasTodos).")");
	
	$arrayWhere = array_unique($arrayWhere);
	
	/*** Variável $where vai conter todos os filtros incluídos ***/
	if(!empty($arrayWhere))
		$where = "WHERE ".implode(" AND ",$arrayWhere);
		
		
	$case = implode(",", $arrayCase);
	$join = implode(" ", $arrayJoin);
	$groupBy = "GROUP BY " . implode(",", $arrayGroupBy) . ",sldcontacontabil, sld.sldvalor ";
	
	$arrayUnionSelects = array();
	$arrayAuxCod = array();
	$arrayAuxDsc = array();
		
	for($i=0; $i<count($_POST["agrupador"]); $i++) {
		$arrayAuxCod = $arraySelectCod;
		$arrayAuxDsc = $arraySelectDsc;
		
		$cont = (count($arrayAuxCod) - 1);
		
		for($k=1; $k<=$i; $k++) {
			$arrayAuxCod[$cont] = 'null';
			$arrayAuxDsc[$cont] = 'null';
			
			$cont--;
		}
		
		$selectCod = implode(",", $arrayAuxCod);
		$selectDsc = implode(",", $arrayAuxDsc);
		
		$sqlInterno = "SELECT 
					   ".$selectCod.",
					   ".$selectDsc.",
					   ".$case."
				       FROM
					   dw.saldo".$_REQUEST['ajaxAno']." sld
					   ".$join."
					   ".$where;
					   //".$groupBy;
		
		array_push($arrayUnionSelects, $sqlInterno);
	}
	
	$unionSelects = implode(" UNION ALL	", $arrayUnionSelects);
	
	$sum =implode(",", $arraySum);
	$sumWhere = implode(" OR ", $arraySumWhere);
	
	for($i=0; $i<count($arraySelectCod); $i++) {
		if($arraySelectCod[$i] == "sld.ptres AS ptres") {
			$arraySelectCod[$i] = "ptres";
			$arraySelectDsc[$i] = "ptres_desc";
		}
		else if($arraySelectCod[$i] == "sld.prgcod || '.' || sld.acacod || '.' || sld.unicod || '.' || sld.loccod || ' - ' || aca2.acadsc || '(' || sld.loccod || ')' AS funcional") {
			$arraySelectCod[$i] = "funcional";
			$arraySelectDsc[$i] = "funcional_desc";
		}
		else if($arraySelectCod[$i] == "sld.ctecod || '.' || sld.gndcod || '.' || sld.mapcod || '.' || sld.edpcod AS natureza") {
			$arraySelectCod[$i] = "natureza";
			$arraySelectDsc[$i] = "natureza_desc";
		}
		else if($arraySelectCod[$i] == "sld.ctecod || '.' || sld.gndcod || '.' || sld.mapcod || '.' || sld.edpcod || '.' || sld.sbecod AS naturezadet") {
			$arraySelectCod[$i] = "naturezadet";
			$arraySelectDsc[$i] = "naturezadet_desc";
		}
		else if($arraySelectCod[$i] == "sld.sldcontacorrente AS conta_corrente") {
			$arraySelectCod[$i] = "conta_corrente";
			$arraySelectDsc[$i] = "conta_corrente_desc";
		}
		else if($arraySelectCod[$i] == "sld.vincod AS vincod") {
			$arraySelectCod[$i] = "vincod";
			$arraySelectDsc[$i] = "vincod_desc";
		}
		else if($arraySelectCod[$i] == "sld.loccod AS loccod") {
			$arraySelectCod[$i] = "loccod";
			$arraySelectDsc[$i] = "loccod_desc";
		}
		else if($arraySelectCod[$i] == "ct1.cdtcod AS enquadramento_cod") {
			$arraySelectCod[$i] = "enquadramento_cod";
			$arraySelectDsc[$i] = "enquadramento_dsc";
		}
		else if($arraySelectCod[$i] == "ct2.cdtcod AS executor_cod") {
			$arraySelectCod[$i] = "executor_cod";
			$arraySelectDsc[$i] = "executor_dsc";
		}
		else if($arraySelectCod[$i] == "ct3.cdtcod AS gestor_cod") {
			$arraySelectCod[$i] = "gestor_cod";
			$arraySelectDsc[$i] = "gestor_dsc";
		}
		else if($arraySelectCod[$i] == "ct4.cdtcod AS nivel_cod") {
			$arraySelectCod[$i] = "nivel_cod";
			$arraySelectDsc[$i] = "nivel_dsc";
		}
		else if($arraySelectCod[$i] == "ct5.cdtcod AS apropriacao_cod") {
			$arraySelectCod[$i] = "apropriacao_cod";
			$arraySelectDsc[$i] = "apropriacao_dsc";
		}
		else if($arraySelectCod[$i] == "ct6.cdtcod AS modalidade_cod") {
			$arraySelectCod[$i] = "modalidade_cod";
			$arraySelectDsc[$i] = "modalidade_dsc";
		}
		elseif($arraySelectCod[$i] == "sld.ungcodresp") {
			$arraySelectCod[$i] = "ungcodresp";
			$arraySelectDsc[$i] = "ungdsc2";
		}
		else {
			$arraySelectCod[$i] = substr($arraySelectCod[$i], 4);
			$arraySelectDsc[$i] = substr($arraySelectDsc[$i], 4);
		}
	}
	
		
	$orderByCodExterno = $groupByCodExterno = implode(",", $arraySelectCod);
	$orderByDscExterno = $groupByDscExterno = implode(",", $arraySelectDsc);
	
	
	for($i=0; $i<count($arraySelectCod); $i++) {
		
			if($arraySelectCod[$i] == 'ptres' || $arraySelectCod[$i] == 'funcional' || $arraySelectCod[$i] == 'conta_corrente' || $arraySelectCod[$i] == 'vincod' || $arraySelectCod[$i] == 'loccod' ) {
				$arraySelectCod[$i] .= " AS cod_agrupador".($i+1);
				$arraySelectDsc[$i] = " '' AS dsc_agrupador".($i+1);
			} else {
				$arraySelectCod[$i] .= " AS cod_agrupador".($i+1);
				//$arraySelectCod[$i] = "coalesce(".$arraySelectCod[$i].", 'Não Aplicável') AS cod_agrupador".($i+1);
				$arraySelectDsc[$i] .= " AS dsc_agrupador".($i+1);
				//$arraySelectDsc[$i] = "coalesce(".$arraySelectDsc[$i].", 'Não Aplicável') AS dsc_agrupador".($i+1);
			}
	}
	
	$selectCodExterno = implode(",", $arraySelectCod);
	$selectDscExterno = implode(",", $arraySelectDsc);
	
	$sqlCompleto = "SELECT
					".$selectCodExterno.",
					".$selectDscExterno.",
					".$sum."
					FROM
					(".$unionSelects.") as foo
					WHERE
					".$sumWhere."
					GROUP BY
					".$groupByCodExterno.",
					".$groupByDscExterno."
					ORDER BY
					".$orderByCodExterno.",
					".$orderByDscExterno;

	
	$resultado = $db2->carregar($sqlCompleto);

		
	if(!$resultado || $resultado == ""){
		echo "<div style=\"color:#990000;padding-left:220px;\" >Não existem registros.</div>";
		return false;
	}
	
	//echo "<div onDblclick=\"document.getElementById('dbg_{$_POST['agrupadorCorrente']}').style.display='none'\"  id=\"dbg_{$_POST['agrupadorCorrente']}\" >";
	//dbg($sqlCompleto);
	//echo "</div>";

	echo "<div onDblclick=\"document.getElementById('dbg_{$_POST['agrupadorCorrente']}').style.display=''\" style=\"background-color:{$_POST['cor']};padding-left:".((count($_POST['agrupador']) - count($arrAgrupador)) * 10)."px;font-weight:bold;\" >".implode(" / ",$arrayTitulo)."</div>";
	//echo "<div style=\"background-color:{$_POST['cor']};padding-left:".((count($_POST['agrupador']) - count($arrAgrupador)) * 10)."px;font-weight:bold;\" >".implode(" / ",$arrayTitulo)."</div>";
	
	for($i=0; $i<count($resultado); $i++) {
		// Percorre cada agrupador selecionado.
		// Começa em 1, pois o padrão é cod_agrupador1, cod_agrupador2, cod_agrupador3...
		for($j=(count($_POST['agrupador']) - count($arrAgrupador)); $j<=count($_POST['agrupador']); $j++) {
			// Alterna a cor das linhas.
			$cor = ($i%2) ? "#F8F8F8" : substr($_POST['cor'], 0, -2)."FF";
			
			//Array de Agrupadores
			if($_POST["agrupador"][$j]){
				$arrAgrupadoresAntigos[$i][] = $_POST["agrupador"][$j];
				$arrValoresAgrupadoresAntigos[$i][] = $resultado[$i]["cod_agrupador".($j + 1)];
			}
			
			if($j == count($_POST['agrupador'])){
				//dbg($agrupadorAntigo);
				//dbg($valorAgrupadorAntigo);
				
				for($x=0;$x < count($agrupadorAntigo);$x++){
					if($agrupadorAntigo[$x] != "undefined"){
						
						array_push($arrAgrupadoresAntigos[$i],$agrupadorAntigo[$x]);
						array_push($arrValoresAgrupadoresAntigos[$i],$valorAgrupadorAntigo[$x]);
					}
				}
############## Esta parte do código resultou em muita dor de cabeça! NUNCA TIRE ESTE COMENTÁRIO !!!!!!!
//				foreach($agrupadorAntigo as $agAnt){
//					if($agAnt != "undefined"){
//						
//						array_push($arrAgrupadoresAntigos[$i],$agAnt);
//						array_push($arrValoresAgrupadoresAntigos[$i],$_POST['valorAgrupadorAntigo']);
//					}
//				}
				
				$exibeArrAg = implode(";",$arrAgrupadoresAntigos[$i]);
				$exibeValoresArrAg = implode(";",array_unique($arrValoresAgrupadoresAntigos[$i]));
			}

			//Inclui a opção de detalhar o último nível
			if($j == count($_POST['agrupador'])){ 
				$maisDetalheAjax = "<a id=\"img_detalhe_agrupador_mais_{$_POST["agrupador"][($j-1)]}_{$resultado[$i]["cod_agrupador".$j]}\" title=\"Detalhar\" style=\"cursor:pointer;\" onclick=\"detalheAgrupador('{$resultado[$i]["cod_agrupador".$j]}','{$_POST["agrupador"][($j-1)]}','$exibeArrAg','$exibeValoresArrAg');\" >[+]</a> <a id=\"img_detalhe_agrupador_menos_{$_POST["agrupador"][($j-1)]}_{$resultado[$i]["cod_agrupador".$j]}\" title=\"Fechar Detalhe\" style=\"display:none;cursor:pointer;\" onclick=\"fechaDetalhe('{$resultado[$i]["cod_agrupador".$j]}','{$_POST["agrupador"][($j-1)]}');\" >[ - ]</a> <a id=\"img_detalhe_agrupador_reset_{$_POST["agrupador"][($j-1)]}_{$resultado[$i]["cod_agrupador".$j]}\" title=\"Limpar Detalhe\" style=\"display:none;cursor:pointer;\" onclick=\"resetarDetalhe('{$resultado[$i]["cod_agrupador".$j]}','{$_POST["agrupador"][($j-1)]}');\" >[ x ]</a>";
			}else{
				$maisDetalheAjax = "";
			}
			
			// Se não for o primeiro agrupador, coloca a seta para representar a hierarquia.
			$seta = ($j==1) ? "" : "<img src='../imagens/seta_filho.gif' align='absmiddle' /> $maisDetalheAjax";
						
			
			// Testa se é o último agrupador.
			if($j == count($_POST['agrupador'])) {
				if($resultado[$i]["cod_agrupador".$j] != NULL) {
					
					$arrValorAgrupadores[$contAgrupador]["codigo"] 		= $resultado[$i]["cod_agrupador".$j];
					$arrValorAgrupadores[$contAgrupador]["descricao"] 	= $resultado[$i]["dsc_agrupador".$j];
					$arrValorAgrupadores[$contAgrupador]["cor"] 		= $cor;
					$arrValorAgrupadores[$contAgrupador]["padding"] 	= (($j-1) * 10);
					$arrValorAgrupadores[$contAgrupador]["seta"] 		= $seta;
					$arrValorAgrupadores[$contAgrupador]["filhos"] 		= 'N';
					$arrValorAgrupadores[$contAgrupador]["nivel"] 		= $j;
					$arrValorAgrupadores[$contAgrupador]["agrupador"]	= $_POST["agrupador"][($j-1)];
					
					for($k=1; $k<=count($_POST["agrupadorColunas"]); $k++) {
						if($resultado[$i]["coluna".$k] != NULL) {
							$valor = number_format($resultado[$i]["coluna".$k], 0, ',', '.' );
							$totalGeral[$k] += $resultado[$i]["coluna".$k];
						}
						else {
							$valor = " - ";
						}
							
						$arrValorAgrupadores[$contAgrupador]["colunas"][$k] = $valor;
					}
					$contAgrupador++;
				}
			}
			// Testa se o próximo agrupador tem valor não nulo.
			else if($resultado[$i]["cod_agrupador".($j+1)] != NULL) {
				if($arrValorAgrupadores[$arrayAuxAgrupadores[$j]]["codigo"] == $resultado[$i]["cod_agrupador".$j]) {
					continue;
				}
				
				$arrValorAgrupadores[$contAgrupador]["codigo"] 		= $resultado[$i]["cod_agrupador".$j];
				$arrValorAgrupadores[$contAgrupador]["descricao"] 	= $resultado[$i]["dsc_agrupador".$j];
				$arrValorAgrupadores[$contAgrupador]["cor"] 		= $cor;
				$arrValorAgrupadores[$contAgrupador]["padding"] 	= (($j-1) * 10);
				$arrValorAgrupadores[$contAgrupador]["seta"] 		= $seta;
				$arrValorAgrupadores[$contAgrupador]["filhos"] 		= 'S';
				$arrValorAgrupadores[$contAgrupador]["nivel"] 		= $j;
				$arrValorAgrupadores[$contAgrupador]["agrupador"]	= $_POST["agrupador"][($j-1)];
				
				$arrayAuxAgrupadores[$j] = $contAgrupador;
				$contAgrupador++;
			}
			// Se o próximo agrupador tiver valor nulo.
			else {
				if($resultado[$i]["cod_agrupador".$j] != NULL) {
					if($arrValorAgrupadores[$arrayAuxAgrupadores[$j]]["codigo"] == $resultado[$i]["cod_agrupador".$j]) {
						for($k=1; $k<=count($_POST["agrupadorColunas"]); $k++) {
							if($resultado[$i]["coluna".$k] != NULL) {
								$valor = number_format($resultado[$i]["coluna".$k], 0, ',', '.' );
							}
							else {
								$valor = " - ";
							}
								
							$arrValorAgrupadores[$arrayAuxAgrupadores[$j]]["colunas"][$k] = $valor;
						}
					}
					else {
						$arrValorAgrupadores[$contAgrupador]["codigo"] 		= $resultado[$i]["cod_agrupador".$j];
						$arrValorAgrupadores[$contAgrupador]["descricao"] 	= $resultado[$i]["dsc_agrupador".$j];
						$arrValorAgrupadores[$contAgrupador]["cor"] 		= $cor;
						$arrValorAgrupadores[$contAgrupador]["padding"] 	= (($j-1) * 10);
						$arrValorAgrupadores[$contAgrupador]["seta"] 		= $seta;
						$arrValorAgrupadores[$contAgrupador]["filhos"] 		= 'N';
						$arrValorAgrupadores[$contAgrupador]["nivel"] 		= $j;
						$arrValorAgrupadores[$contAgrupador]["agrupador"]	= $_POST["agrupador"][($j-1)];
						
						for($k=1; $k<=count($_POST["agrupadorColunas"]); $k++) {
							if($resultado[$i]["coluna".$k] != NULL) {
								$valor = number_format($resultado[$i]["coluna".$k], 0, ',', '.' );
								$totalGeral[$k] += $resultado[$i]["coluna".$k];
							}
							else {
								$valor = " - ";
							}
								
							$arrValorAgrupadores[$contAgrupador]["colunas"][$k] = $valor;
						}
						$arrayAuxAgrupadores[$j] = $contAgrupador;
						$contAgrupador++;
					}
				}
			}
		}
	}
	
	$arrNivel 	 = array();
	$arrNivel[1] = false;
	$arrNivel[2] = false;
	$arrNivel[3] = false;
	$arrNivel[4] = false;
	
		
	for($i=0; $i<count($arrValorAgrupadores); $i++) {
		if($arrNivel[$arrValorAgrupadores[$i]["nivel"]] == true) {
			for($t=(count($arrAgrupador) - 1); $t>$arrValorAgrupadores[$i]["nivel"]; $t--) {
				if($arrNivel[$t] == true) {
					echo "</div>";
					$arrNivel[$t] = false;
				}
			}
			
			echo "</div>";
			$arrNivel[$arrValorAgrupadores[$i]["nivel"]] = false;
		}
			
		if($arrValorAgrupadores[$i]["filhos"] == 'S') {
			$img = '';
		} else {
			$img = '';
		}
		
		if($arrValorAgrupadores[1]["codigo"] == "." || $arrValorAgrupadores[1]["codigo"] == "" || !$arrValorAgrupadores[1]["codigo"]){
			echo "<div style=\"color:#990000;padding-left:220px;\" >Não existem registros.</div>";
			return false;
		}
		
		//Inclui a opção de detalhar o último nível
		if($arrValorAgrupadores[$i]["nivel"] == count($_POST['agrupador'])){
			$tabelaDetalheAgrupador = "<div style=\"display:\" id=\"div_detalhe_agrupador_{$arrValorAgrupadores[$i]["agrupador"]}_{$arrValorAgrupadores[$i]["codigo"]}\" >";
			$tabelaDetalheAgrupador .= "</div>";
		}else{
			$tabelaDetalheAgrupador = "";
		}

		switch($_POST['cor']){
			case "#FAEBD7": //laranja
				$cor2 = "#fffbe0";
				$cor1 = "#fffefa";
				break;
			case "#FFFACD": //amarelo
				$cor2 = "#fffde0";
				$cor1 = "#fffffa";
				break;
			case "#e0ffff": //azul claro
				$cor2 = "#F0FFFF";
				$cor1 = "#faffff";
				break;
			case "#E6E6FA": //roxo
				$cor2 = "#fcfaff";
				$cor1 = "#fdf5ff";
				break;
			case "#90EE90": //verde
				$cor2 = "#efffe0";
				$cor1 = "#f5fff6";
				break;
			case "#CD5C5C": //vermelho
				$cor2 = "#ffe2e0";
				$cor1 = "#fff7f5";
				break;
			case "#AFEEEE": //azul
				$cor2 = "#e0fcff";
				$cor1 = "#fafcff";
				break;
			case "#CFCFCF": //cinza
				$cor2 = "#f2f2f2";
				$cor1 = "#fbfaf9";
				break;
			default:
				$cor2 = "#FFFFFF";
				$cor1 = "#fbfaf9";
		}
		
		// 	Alterna a cor das linhas.
		$cor = ($i%2) ? $cor1 : $cor2;
				
		echo '<table cellspacing="0" border="0">';
		echo '<tr bgcolor="'.$cor.'" onmouseover="this.style.backgroundColor = \'#ffffcc\';" onmouseout="this.style.backgroundColor = \''.$cor.'\';">';
		echo '<td style="text-align:left; padding: 2px 3px 2px '.$arrValorAgrupadores[$i]["padding"].'px;border-right: 1px solid #CCCCCC;">'.$arrValorAgrupadores[$i]["seta"].' '.$img.' '.$arrValorAgrupadores[$i]["codigo"].' '.$arrValorAgrupadores[$i]["descricao"].'</td>';
		
		for($k=1; $k<=count($arrValorAgrupadores[$i]["colunas"]); $k++) {
			echo '<td class="colunaValor">'.$arrValorAgrupadores[$i]["colunas"][$k].'</td>';
		}
		
		echo '</tr></table>';
		
		//Adiciona a Tabela para exibição do Detalhe do Agurpador de Úlimo Nível
		echo $tabelaDetalheAgrupador;
		
		
		if($arrValorAgrupadores[$i]["filhos"] == 'S') {
			echo '<div id="div'.$i.'">';
			$arrNivel[$arrValorAgrupadores[$i]["nivel"]] = true;
		}
	}
	
	echo "</div>";
	?>
	</td>
	</tr>
	<tr>
	<?php 
	echo '</tr></thead></table></td></tr>';
	exit;
}

				
// Busca os perfis do usuário para o módulo 'Orçamentário e Financeiro'
$possuiPerfilConsultaUnidades = false;
$sql = "SELECT 
			pu.pflcod as perfil
		FROM 
			seguranca.perfilusuario pu 
		INNER JOIN 
			seguranca.perfil p ON p.pflcod = pu.pflcod AND p.sisid = ".$_SESSION["sisid"]." 
		WHERE 
			pu.usucpf = '".$_SESSION["usucpf"]."'";

$perfilUsuario = $db->carregar($sql);

// Verifica se o usuário possui somente perfil de "Consulta Unidade"
if((count($perfilUsuario) == 1) && ($perfilUsuario[0]["perfil"] == 175)) {
	$possuiPerfilConsultaUnidades = true;
}

// Função que retorna a descrição da coluna a partir do código(icbdsc)
function recuperaColunas($cod) {
	global $db2;
	
	$sql = "SELECT 
				icbdscresumida 
			FROM 
				financeiro.informacaocontabil 
			WHERE 
				icbcod = ".$cod;
	
	return $db2->pegaUm($sql);
}

/* Função que retorna um array com todos os agrupadores (código e descrição). 
   Se um parâmetro for passado (código) ela retorna somente a descrição do agrupador. */
function recuperaAgrupadores($cod = null) {

$matriz = array(
				array('codigo' => 'acacod',
					  'descricao' => 'Ação'),
				array('codigo' => 'programa',
					  'descricao' => 'Programa'),
				array('codigo' => 'ug',
					  'descricao' => 'Unidade Gestora'),
				array('codigo' => 'ugr',
					  'descricao' => 'Unidade Gestora Responsável'),
				array('codigo' => 'uo',
					  'descricao' => 'Unidade Orçamentária'),
				array('codigo' => 'orgao',
					  'descricao' => 'Órgão'),
				array('codigo' => 'ptres',
					  'descricao' => 'Ptres'),
				array('codigo' => 'funcional',
					  'descricao' => 'Funcional'),
				array('codigo' => 'planointerno',
	    			  'descricao' => 'Plano Interno'),
				array('codigo' => 'grupouo',
	    			  'descricao' => 'Grupo UO/UG'),
				array('codigo' => 'orgaouo',
	    			  'descricao' => 'Órgão da UO'),
				array('codigo' => 'catecon',
					  'descricao' => 'Categoria Econômica'),
				array('codigo' => 'elemento',
					  'descricao' => 'Elemento de Despesa'),
				array('codigo' => 'subelemento',
					  'descricao' => 'Sub-Elemento de Despesa'),
				array('codigo' => 'esfera',
					  'descricao' => 'Esfera'),
				array('codigo' => 'fonte',
					  'descricao' => 'Fonte SOF'),
				array('codigo' => 'fontesiafi',
					  'descricao' => 'Fonte Detalhada'),
				array('codigo' => 'funcao',
					  'descricao' => 'Função'),
				array('codigo' => 'gnd',
					  'descricao' => 'GND'),
				array('codigo' => 'grf',
					  'descricao' => 'Grupo de Fonte'),
				array('codigo' => 'mapcod',
					  'descricao' => 'Modalidade de Aplicação'),
				array('codigo' => 'modlic',
					  'descricao' => 'Modalidade de Licitação'),
				array('codigo' => 'natureza',
					  'descricao' => 'Natureza de Despesa'),
				array('codigo' => 'naturezadet',
					  'descricao' => 'Natureza de Despesa Detalhada'),
				array('codigo' => 'subfuncao',
					  'descricao' => 'Sub-função'),
				array('codigo' => 'sldcontacorrente',
					  'descricao' => 'Conta Corrente'),
				array('codigo' => 'recurso',
					  'descricao' => 'Recurso'),
				array('codigo' => 'vincod',
					  'descricao' => 'Vinculação de Pagamento'),
				array('codigo' => 'cagcod',
					  'descricao' => 'Categoria de Gasto'),
				array('codigo' => 'loccod',
					  'descricao' => 'Subtítulo'),
				array('codigo' => 'enquadramento',
					  'descricao' => 'Enquadramento da Despesa'),
				array('codigo' => 'executor',
					  'descricao' => 'Executor Orçamentário e Financeiro'),
				array('codigo' => 'gestor',
					  'descricao' => 'Gestor da Subação'),
				array('codigo' => 'nivel',
					  'descricao' => 'Nível/Etapa de Ensino'),
				array('codigo' => 'apropriacao',
					  'descricao' => 'Categoria de Apropriação'),
				array('codigo' => 'modalidade',
					  'descricao' => 'Modalidade de Ensino'),
				array('codigo' => 'subacao',
					  'descricao' => 'Subação'),
				array('codigo' => 'orgaougexecutora',
					  'descricao' => 'Orgão da UG Executora'),
				array('codigo' => 'gestaoexecutora',
					  'descricao' => 'Gestão Executora'),
				array('codigo' => 'fonterecurso',
					  'descricao' => 'Fonte de Recurso'),
				array('codigo' => 'iduso',
					  'descricao' => 'IDUSO')
			);
			
	if($cod == null) {
		return $matriz;
	}
	else {
		for($i=0; $i<=count($matriz); $i++) {
			if($matriz[$i]['codigo'] == $cod) {
				$desc = $matriz[$i]['descricao'];
				break;
			}
		}
		
		return $desc;
	}
}
				
// Testa se o formulário foi submetido
//ver($_POST);
if($_REQUEST["submetido"]) {

	/*** Início - Transforma consulta em pública ***/
	if ( $_REQUEST['prtid'] && $_REQUEST['publico'] )
	{
		$sql = sprintf(
			"UPDATE public.parametros_tela SET prtpublico = case when prtpublico = true then false else true end WHERE prtid = %d",
			$_REQUEST['prtid']
		);
		$db2->executar( $sql );
		$db2->commit();
		?>
		<script type="text/javascript">
			alert("Operação realizada com sucesso!");
			location.href = '?modulo=relatorio/geral_teste&acao=R';
		</script>
		<?
	}
	/*** Fim - Transforma consulta em pública ***/

	
	/*** Início - Realiza exclusão de consultas/relatórios ***/
	if ( $_REQUEST['prtid'] && $_REQUEST['excluir'] ) 
	{
		$sql = sprintf(
			"DELETE FROM public.parametros_tela WHERE prtid = %d",
			$_REQUEST['prtid']
		);
		$db2->executar( $sql );
		$db2->commit();
		?>
			<script type="text/javascript">
				alert("Operação realizada com sucesso!");
				location.href = '?modulo=relatorio/geral_teste&acao=R';
			</script>
		<?
		exit;
	}
	/*** Fim - Realiza exclusão de consultas/relatórios ***/

	
	/*** Remove flag de submissão de formulário para carregá-lo ***/
	if ( $_REQUEST['prtid'] && $_REQUEST['carregar'] ) 
	{
		unset( $_REQUEST['submetido'] );
	}
	/*** Exibe a consulta ***/
	if ( isset( $_REQUEST['submetido'] ) == true && $_REQUEST['alterar_ano'] == '0' )
	{
		if ( $_REQUEST['prtid'] )
		{
			$sql = sprintf(	"select prtobj from public.parametros_tela where prtid = " . $_REQUEST['prtid'] );
			$itens = $db2->pegaUm( $sql );
			$dados = unserialize( stripslashes( stripslashes( $itens ) ) );
			$_REQUEST = $dados;
			unset( $_REQUEST['salvar'] );
		}
		include 'resultadoRelatorioGeral.inc';
		exit();
	}
	
	/*** Variáveis que alimentam o formulário ***/
	/*** Variáveis dos filtros gerais ***/
	$titulo = '';
	$ano = '';
	$nao_mostra_filtro_impressao = false;
	$agrupadores_escondidos = false;
	$agrupador = array();
	$agrupadorColunas = array();
	$publico = '';
	$prtid = '';
	$excluir = '';
	$escala = '1';
	
	
	/*** Início - Verifica se o usuário alterou o ano ***/
	$ano = $_SESSION['exercicio'];
	if ( $_REQUEST['alterar_ano'] == '1' )
	{
		$titulo = $_REQUEST['titulo'];
		$ano = $_REQUEST['ano'];
		if ( $_REQUEST['agrupador'] )
		{
			foreach ( $_REQUEST['agrupador'] as $valorAgrupador )
			{
				array_push( $agrupador, array( 'codigo' => $valorAgrupador, 'descricao' => recuperaAgrupadores($valorAgrupador) ) );
			}
		}
		
		if ( $_REQUEST['agrupadorColunas'] )
		{
			foreach ( $_REQUEST['agrupadorColunas'] as $valorAgrupador )
			{
				array_push( $agrupadorColunas, array( 'codigo' => $valorAgrupador, 'descricao' => recuperaColunas($valorAgrupador) ) );
			}
		}
	}
	/*** Fim - Verifica se o usuário alterou o ano ***/
	
	
	/*** Início - Carrega uma consulta gravada anteriormente ***/
	if ( $_REQUEST['prtid'] && $_REQUEST['carregar'] ) {
		$sql = sprintf(	"SELECT prtobj FROM public.parametros_tela WHERE prtid = ".$_REQUEST['prtid'] );
		$itens = $db2->pegaUm( $sql );
		
		$dados = unserialize( stripslashes( stripslashes( $itens ) ) );
		extract( $dados );
		$_REQUEST = $dados;
		unset( $_REQUEST['submetido'] );
		
		$titulo = $_REQUEST['titulo'];
		$ano = $_REQUEST['ano'];
		$escala = $_REQUEST['escala'];
		$nao_mostra_filtro_impressao = $_REQUEST['nao_mostra_filtro_impressao'];
		$agrupadores_escondidos = $_REQUEST['agrupadores_escondidos'];
		
		$agrupador = array();
		if ( $_REQUEST['agrupador'] )
		{
			foreach ( $_REQUEST['agrupador'] as $valorAgrupador )
			{
				array_push( $agrupador, array( 'codigo' => $valorAgrupador, 'descricao' => recuperaAgrupadores($valorAgrupador) ) );
			}
		}
		
		$agrupadorColunas = array();
		if ( $_REQUEST['agrupadorColunas'] )
		{
			foreach ( $_REQUEST['agrupadorColunas'] as $valorAgrupador )
			{
				array_push( $agrupadorColunas, array( 'codigo' => $valorAgrupador, 'descricao' => recuperaColunas($valorAgrupador) ) );
			}
		}
	}
	/*** Fim - Carrega uma consulta gravada anteriormente ***/
	
	
	/*** variáveis que alimentam o formulário ***/
	/*** variáveis dos combos ***/
	/*** variáveis devem ser alimentadas pelos querys antecedentes aos combos ***/
	$orgao	 			= array();
	$ug 				= array();
	$ugr 				= array();
	$uo 				= array();
	$grupouo 			= array();
	$funcao 			= array();
	$subfuncao			= array();
	$programa 			= array();
	$acacod		 		= array();
	$ptres 				= array();
	$planointerno		= array();
	$grf				= array();
	$fonte				= array();
	$catecon 			= array();
	$gnd 				= array();
	$mapcod		 		= array();
	$elemento 			= array();
	$natureza 			= array();
	$naturezadet		= array();
	$fontesiafi 		= array();
	$sldcontacorrente	= array();
	$recurso			= array();
	$vincod				= array();
	$cagcod				= array();
	$loccod				= array();
	$enquadramento		= array();
	$executor			= array();
	$gestor				= array();
	$nivel				= array();
	$apropriacao		= array();
	$modalidade			= array();
	$subacao			= array();
	$modlic				= array();
	$orgaougexecutora	= array();
	$gestaoexecutora    = array();
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . '/includes/Agrupador.php';
print '<br/>';
monta_titulo( 'Relatório Módulo Financeiro', 'Relatório Geral' );

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../../includes/remedial.js"></script>
<script type="text/javascript" src="../../includes/superTitle.js"></script>
<link rel="stylesheet" type="text/css" href="../../includes/superTitle.css"/>
<script type="text/javascript">
	/**
	 * Alterar visibilidade de um bloco.
	 * 
	 * @param string indica o bloco a ser mostrado/escondido
	 * @return void
	 */
	function onOffBloco( bloco )
	{
		var div_on = document.getElementById( bloco + '_div_filtros_on' );
		var div_off = document.getElementById( bloco + '_div_filtros_off' );
		var img = document.getElementById( bloco + '_img' );
		
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			img.src = '/imagens/menos.gif';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			img.src = '/imagens/mais.gif';
		}
	}
	
	/**
	 * Alterar visibilidade de um campo.
	 * 
	 * @param string indica o campo a ser mostrado/escondido
	 * @return void
	 */
	function onOffCampo( campo )
	{
		var div_on = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
		}
	}
	
</script>

<form action="" method="post" name="formulario">

<input type="hidden" name="submetido" value="1" />
<input type="hidden" name="alterar_ano" value="0" />
<input type="hidden" name="publico" value="" />
<input type="hidden" name="prtid" value="" />
<input type="hidden" name="carregar" value="" />
<input type="hidden" name="excluir" value="" />

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td class="SubTituloCentro" align="center" colspan="2"><b>O MODULO ORÇAMENTÁRIO E  FINANCEIRO  ENCONTRA-SE  EM  FASE  FINAL  DE  TESTES, POSSÍVEIS  DIVERGÊNCIAS  DE  VALORES  DEVERÃO  SER  COMUNICADOS À CCONT/MEC UG 150003.</b></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Título</td>
		<td>
			<?= campo_texto('titulo', 'N', 'S', '', 78, 100, '', ''); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Exercício</td>
		<td>
			<select class="campoEstilo" name="ano">
				<option value="2009">2009</option>
				<option value="2010">2010</option>
				<option value="2011" selected>2011</option>
			</select>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Mês Referência</td>
		<td>
			<select name="mes" class="campoEstilo">
				<option value="">Todos / Não se aplica</option>
				<option value="1">Janeiro</option>
				<option value="2">Fevereiro</option>
				<option value="3">Março</option>
				<option value="4">Abril</option>
				<option value="5">Maio</option>
				<option value="6">Junho</option>
				<option value="7">Julho</option>
				<option value="8">Agosto</option>
				<option value="9">Setembro</option>
				<option value="10">Outubro</option>
				<option value="11">Novembro</option>
				<option value="12">Dezembro</option>
			</select>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Escala</td>
		<td>
			<select name="escala" class="CampoEstilo">
				<option value="1" <?= $escala == '1' ? 'selected="selected"' : '' ; ?>>R$ 1 (Reais)</option>
				<option value="1000" <?= $escala == '1000' ? 'selected="selected"' : '' ; ?>>R$ 1.000 (Milhares de Reais)</option>
			</select>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Filtro</td>
		<td>
			<input type="checkbox" name="nao_mostra_filtro_impressao" id="nao_mostra_filtro_impressao" value="1" <?= $nao_mostra_filtro_impressao ? 'checked="checked"' : '' ; ?> />
			<label for="nao_mostra_filtro_impressao">Não mostrar filtros</label>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Exibição</td>
		<td>
			<input type="checkbox" name="agrupadores_escondidos" id="agrupadores_escondidos" value="1" <?= $agrupadores_escondidos ? 'checked="checked"' : '' ; ?> />
			<label for="agrupadores_escondidos">Agrupadores Fechados</label>
		</td>
	</tr>
	
	<tr>
		<td width="195" class="SubTituloDireita" valign="top">Agrupadores</td>
		<td>
			<?php
				$campoAgrupador = new Agrupador( 'formulario' );
				$campoAgrupador->setOrigem( 'agrupadorOrigem', null, recuperaAgrupadores() );
				$campoAgrupador->setDestino( 'agrupador', 5 );
				$campoAgrupador->exibir();
			?>
		</td>
	</tr>
	<tr>
		<td width="195" class="SubTituloDireita" valign="top">Colunas</td>
		<td>
			<?php
				$sql = "SELECT icbcod as codigo,icbdscresumida as descricao, icbdsc as title FROM financeiro.informacaocontabil WHERE icbvisualizaconta = 't' ORDER BY icbdscresumida";
				$matriz = $db2->carregar($sql);
				
				$campoAgrupador = new Agrupador( 'formulario' );
				$campoAgrupador->setOrigem( 'agrupadorOrigemColunas', null, $matriz );
				$campoAgrupador->setDestino( 'agrupadorColunas', null );
				$campoAgrupador->exibir();
				
				if ( $matriz ) {
				foreach( $matriz as $ma ){
					$sql = "SELECT DISTINCT		
								pc.conconta
							FROM 
								financeiro.informacaocontabil icb
							INNER JOIN 
								financeiro.informacaoconta ic ON ic.icbcod = icb.icbcod 
							INNER JOIN 
								dw.planoconta pc ON pc.conconta = ic.conconta
							WHERE 
								icb.icbcod = ".$ma['codigo'];
					$dados = $db2->carregarColuna($sql);

					$sql = "SELECT 
								conconta || ' - ' || condsc as descricao
							FROM 
								dw.planoconta 
							WHERE
								contipocontacorrente in ('16','17','31','80','26','45','50','77','00','02','06','12','52','64','37','72','28','18','76') and
								conconta in ('".implode("','", $dados)."')
							GROUP BY
								conconta, condsc 
							ORDER BY 
								conconta";
					$dados = $db2->carregarColuna( $sql );
					
					$title = $ma['title']."<br>";
					
					foreach( $dados as $dado ){
						$title.= "<br>".$dado;
					}

					$tas[$ma['codigo']] = utf8_encode($title);	
				}
				$ma = json_encode( $tas );
			}

			?>
			<script>
			document.getElementById('agrupadorOrigemColunas').style.width = '330';
			jQuery(document).ready(function(){
				var matriz = <?=$ma?>;
				jQuery("#agrupadorOrigemColunas option").mousemove(function() {
	               		texto = matriz[jQuery(this).val()];	               		
	                	SuperTitleOff( this );	               		
                    	SuperTitleOn( this, texto);
	            	})
	            	.mouseout(function(){
                		SuperTitleOff( this );
                });
                
            });
			
			document.getElementById('agrupadorColunas').style.width = '330';
			</script>
		</td>
	</tr>
</table>

<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
	<tr>
		<td onclick="javascript:onOffBloco( 'outros' );">
			<img border="0" src="/imagens/mais.gif" id="outros_img"/>&nbsp;
			Relatórios Gerenciais
		</td>
	</tr>
</table>

<div id="outros_div_filtros_off"></div>
<div id="outros_div_filtros_on" style="display:none;">
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
		<tr>
			<td width="195" class="SubTituloDireita" valign="top">Relatórios:</td>
			<td>
			<?php
				$sql = sprintf(
					"SELECT CASE WHEN prtpublico = true AND usucpf = '%s' THEN '<img border=\"0\" src=\"../imagens/usuario.gif\" title=\" Despublicar \" onclick=\"tornar_publico(' || prtid || ');\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' ELSE '<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">' END AS acao, '<a href=\"javascript: carregar_consulta(' || prtid || ');\">' || prtdsc || '</a>' AS descricao FROM public.parametros_tela WHERE mnuid = %d AND prtpublico = TRUE",
					$_SESSION['usucpf'],
					$_SESSION['mnuid'],
					$_SESSION['usucpf']);
					 
				$db2->monta_lista_simples( $sql, null, 50, 50, null, null, null ); 
			?>
			</td>
		</tr>
	</table>
</div>

<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
	<tr>
		<td onclick="javascript:onOffBloco( 'minhasconsultas' );">
			<img border="0" src="/imagens/mais.gif" id="minhasconsultas_img"/>&nbsp;
			Minhas Consultas
		</td>
	</tr>
</table>

<div id="minhasconsultas_div_filtros_off"></div>
<div id="minhasconsultas_div_filtros_on" style="display:none;">
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
		<tr>
			<td width="195" class="SubTituloDireita" valign="top">Consultas:</td>
			<td>
			<?php
				$sql = sprintf(
					"SELECT CASE WHEN prtpublico = false THEN '".(($db->testa_superuser())?"<img border=\"0\" src=\"../imagens/grupo.gif\" title=\" Publicar \" onclick=\"tornar_publico(' || prtid || ')\">&nbsp;&nbsp;":"")."<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' ELSE '<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' END AS acao, '<a href=\"javascript: carregar_consulta(' || prtid || ')\">' || prtdsc || '</a>' AS descricao FROM public.parametros_tela WHERE mnuid = %d AND usucpf = '%s'",
					$_SESSION['mnuid'],
					$_SESSION['usucpf']);
					
				$db2->monta_lista_simples( $sql, null, 50, 50, null, null, null );
				
			?>
			</td>
		</tr>
	</table>
</div>

<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
	<tr>
		<td onclick="javascript:onOffBloco( 'inst' );">
			<img border="0" src="/imagens/mais.gif" id="inst_img"/>&nbsp;
			Institucional
		</td>
	</tr>
</table>
	
<div id="inst_div_filtros_off"></div>
<div id="inst_div_filtros_on" style="display:none;">
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
		<tr>
			<td width="195" class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'orgao' );">
				Órgão
			</td>
			<td>
				<div id="orgao_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'orgao' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="orgao_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT
										os.orscod as codigo,
										os.orscod || ' - ' || os.orsdsc as descricao
									FROM 
										dw.orgaosuperior os
									".( ($unicods_CONSULTAUNIDADE) ? " INNER JOIN dw.uguo uo ON uo.orgcodsup = os.orscod WHERE uo.unicod in ('".implode("','", $unicods_CONSULTAUNIDADE)."') " : "" )."
									ORDER BY
										os.orsdsc";

						if ( $_REQUEST['orgao'] && $_REQUEST['orgao'][0] != '' )
						{
							$sql_carregados = "SELECT
											 		os.orscod as codigo,
													os.orscod || ' - ' || os.orsdsc as descricao 
											   FROM
											   		dw.orgaosuperior os
											   WHERE
											   		os.orscod in ('".implode("','", $_REQUEST['orgao'])."')";
							$orgao = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'orgao', $sql_combo, 'Selecione o(s) Órgão(s)', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true);
					?>
				</div>
			</td>
		</tr>
		<? if ( $orgao )  { ?> <script type="text/javascript"> onOffCampo( 'orgao' ); </script> <? } ?>
		<tr>
			<td width="195" class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'orgaougexecutora' );">
				Órgão da UG Executora
			</td>
			<td>
				<div id="orgaougexecutora_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'orgaougexecutora' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="orgaougexecutora_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT
										codigo as codigo,
										codigo || ' - ' || descricao as descricao
									FROM 
										dw.orgaoug u
									".( ($unicods_CONSULTAUNIDADE) ? " inner join dw.uguo ug ON ug.orgcoduo = u.codigo WHERE unicod in ('".implode("','", $unicods_CONSULTAUNIDADE)."') " : "" )."
									ORDER BY
										codigo";

						if ( $_REQUEST['orgaougexecutora'] && $_REQUEST['orgaougexecutora'][0] != '' )
						{
							$sql_carregados = "SELECT
											 		codigo as codigo,
													codigo || ' - ' || descricao as descricao 
											   FROM
											   		dw.orgaoug
											   WHERE
											   		codigo in ('".implode("','", $_REQUEST['orgaougexecutora'])."')";
							$orgaougexecutora = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'orgaougexecutora', $sql_combo, 'Selecione o(s) Órgão(s) da UG Executora', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true);
					?>
				</div>
			</td>
		</tr>
		<? if ( $orgaougexecutora )  { ?> <script type="text/javascript"> onOffCampo( 'orgaougexecutora' ); </script> <? } ?>
		<tr>
			<td width="195" class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'gestaoexecutora' );">
				Gestão Executora
			</td>
			<td>
				<div id="gestaoexecutora_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'gestaoexecutora' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="gestaoexecutora_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT
										gst.gstcod as codigo,
										gst.gstcod || ' - ' || gst.gstdsc as descricao
									FROM 
										dw.gestao gst
									".( ($unicods_CONSULTAUNIDADE) ? " INNER JOIN dw.uguo ug ON ug.orgcodgestao = gst.gstcod WHERE ug.unicod in ('".implode("','", $unicods_CONSULTAUNIDADE)."') " : "" )."
									ORDER BY
										gst.gstdsc";
						
						if ( $_REQUEST['gestaoexecutora'] && $_REQUEST['gestaoexecutora'][0] != '' )
						{
							$sql_carregados = "SELECT
											 		gstcod as codigo,
													gstcod || ' - ' || gstdsc as descricao 
											   FROM
											   		dw.gestao 
											   WHERE
											   		gstcod in ('".implode("','", $_REQUEST['gestaoexecutora'])."')";
							$gestaoexecutora = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'gestaoexecutora', $sql_combo, 'Selecione a(s) Gestão Executora', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true);
					?>
				</div>
			</td>
		</tr>
		<? if ( $gestaoexecutora )  { ?> <script type="text/javascript"> onOffCampo( 'gestaoexecutora' ); </script> <? } ?>
		<tr>
			<td width="195" class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'ug' );">
				Unidades Gestoras
			</td>
			<td>
				<div id="ug_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'ug' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="ug_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT distinct 
										u.ungcod as codigo,
										u.ungcod || ' - ' || u.ungdsc as descricao
										FROM 
										dw.ug u
										LEFT JOIN
										dw.uguo o ON o.ungcod = u.ungcod --AND o.unicod in ('73107', '74902')
										WHERE
										u.ungcod between '150000' and '159999'
										".( ($unicods_CONSULTAUNIDADE) ? " AND o.unicod in ('".implode("','", $unicods_CONSULTAUNIDADE)."') " : "" )."
										ORDER BY
										u.ungcod, u.ungcod || ' - ' || u.ungdsc";

						if ( $_REQUEST['ug'] && $_REQUEST['ug'][0] != '' )
						{
							$sql_carregados = "SELECT
											 		u.ungcod as codigo,
													u.ungcod || ' - ' || u.ungdsc as descricao 
											   FROM
											   		dw.ug u
											   WHERE
											   		u.ungcod in ('".implode("','", $_REQUEST['ug'])."')";
							$ug = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'ug', $sql_combo, 'Selecione a(s) Unidade(s) Gestora(s)', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true);
					?>
				</div>
			</td>
		</tr>
		<? if ( $ugr )  { ?> <script type="text/javascript"> onOffCampo( 'ugr' ); </script> <? } ?>
		
		<tr>
			<td width="195" class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'ugr' );">
				Unidades Gestoras Responsáveis
			</td>
			<td>
				<div id="ugr_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'ugr' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="ugr_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT distinct 
										u.ungcod as codigo,
										u.ungcod || ' - ' || u.ungdsc as descricao
										FROM 
										dw.ug u
										LEFT JOIN
										dw.uguo o ON o.ungcod = u.ungcod --AND o.unicod in ('73107', '74902')
										WHERE
										u.ungcod between '150000' and '159999'
										".( ($unicods_CONSULTAUNIDADE) ? " AND o.unicod in ('".implode("','", $unicods_CONSULTAUNIDADE)."') " : "" )."
										ORDER BY
										u.ungcod, u.ungcod || ' - ' || u.ungdsc";

						if ( $_REQUEST['ugr'] && $_REQUEST['ugr'][0] != '' )
						{
							$sql_carregados = "SELECT
											 		u.ungcod as codigo,
													u.ungcod || ' - ' || u.ungdsc as descricao 
											   FROM
											   		dw.ug u
											   WHERE
											   		u.ungcod in ('".implode("','", $_REQUEST['ugr'])."')";
							$ugr = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'ugr', $sql_combo, 'Selecione a(s) Unidade(s) Gestora(s) Responsáveis', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true);
					?>
				</div>
			</td>
		</tr>
		<? if ( $ugr )  { ?> <script type="text/javascript"> onOffCampo( 'ugr' ); </script> <? } ?>
		
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'uo' );">
				Unidades Orçamentárias
			</td>
			<td>
				<div id="uo_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'uo' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="uo_campo_on" style="display:none;">
					<?
					/*
					 * FEITO POR ALEXANDRE DOURADO 25/06/2009
					 * SOLICITADO POR HENRIQUE XAVIER
					 * DESCRIÇÃO DA ROTINA: OS USUARIOS COM O PERFIL CONSULTA UNIDADE, SOMENTE PODERÃO FILTRAR AS UNIDADES DESTINADAS AO PERFIL
					 */
				 		$sql_combo = "SELECT
				 						unicod as codigo, 
				 						unicod || ' - ' || unidsc as descricao 
				 					 FROM 
				 					 	dw.uo
				 					 WHERE 
				 					 	(unicod like '26%' OR unicod in ('73107', '74902')) ".(($unicods_CONSULTAUNIDADE)?"AND unicod in('".implode("','",$unicods_CONSULTAUNIDADE)."')":"")."
				 					 ORDER BY
				 					 	unicod";
				 		/*
				 		 * FIM DA ROTINA: OS USUARIOS COM O PERFIL CONSULTA UNIDADE, SOMENTE PODERÃO FILTRAR AS UNIDADES DESTINADAS AO PERFIL
				 		 */
						if ( $_REQUEST['uo'] && $_REQUEST['uo'][0] != '' )
						{
							$sql_carregados = "SELECT
											 		unicod as codigo, 
				 									unicod || ' - ' || unidsc as descricao 
											   FROM
											   		dw.uo
											   WHERE
											   		unicod in ('".implode("','", $_REQUEST['uo'])."')";
							$uo = $db2->carregar( $sql_carregados );
						}
						
					 	combo_popup( 'uo', $sql_combo, 'Selecione a(s) Unidade(s)', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true);
					?>
				</div>
			</td>
		</tr>
		<? if ( $uo )  { ?> <script type="text/javascript"> onOffCampo( 'uo' ); </script> <? } ?>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'grupouo' );">
				Grupo UO
			</td>
			<td>
				<div id="grupouo_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'grupouo' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="grupouo_campo_on" style="display:none;">
					<?
				 		$sql_combo = "SELECT
				 						guo.guoid as codigo, 
				 						guo.guoid || ' - ' || guo.guodsc as descricao 
				 					 FROM 
				 					 	dw.grupouo guo
				 					 ".( ($unicods_CONSULTAUNIDADE) ? " INNER JOIN dw.uguo uo ON uo.guoid = guo.guoid WHERE uo.unicod in ('".implode("','", $unicods_CONSULTAUNIDADE)."') " : "" )."
				 					 ORDER BY
				 					 	guo.guoid";

				 		if ( $_REQUEST['grupouo'] && $_REQUEST['grupouo'][0] != '' )
						{
							$sql_carregados = "SELECT
											 		guoid as codigo, 
				 									guoid || ' - ' || guodsc as descricao 
											   FROM
											   		dw.grupouo
											   WHERE
											   		guoid in (".implode(",", $_REQUEST['grupouo']).")";
							$grupouo = $db2->carregar( $sql_carregados );
						}
						
					 	combo_popup( 'grupouo', $sql_combo, 'Selecione o(s) Grupo(s) UO', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true);
					?>
				</div>
			</td>
		</tr>
		<? if ( $grupouo )  { ?> <script type="text/javascript"> onOffCampo( 'grupouo' ); </script> <? } ?>
	</table>
</div>

<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
	<tr>
		<td onclick="javascript:onOffBloco( 'func' );">
			<img border="0" src="/imagens/mais.gif" id="func_img"/>&nbsp;
			Funcional
		</td>
	</tr>
</table>
<div id="func_div_filtros_off"></div>
<div id="func_div_filtros_on" style="display:none;">
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
		<!--<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'esfera' );">
				Esfera
			</td>
			<td>
				<div id="esfera_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'esfera' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="esfera_campo_on" style="display:none;">
					<? 
						/*$sql_combo = "SELECT 
										esfcod AS codigo, 
										esfcod || ' - ' || esfdsc AS descricao 
								      FROM
								      	dw.esfera
								      ORDER BY
								      	esfcod";
					    
						if ( $_REQUEST['esfera'] && $_REQUEST['esfera'][0] != '' )
						{
							$sql_carregados = "SELECT
											 		esfcod AS codigo, 
													esfcod || ' - ' || esfdsc AS descricao 
											   FROM
											   		dw.esfera
											   WHERE
											   		esfcod in (".implode(",", $_REQUEST['esfera']).")";
							$esfera = $db->carregar( $sql_carregados );
						}
						
						combo_popup( 'esfera', $sql_combo, 'Selecione a(s) Esfera(s)', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao );*/ 
					?>
				</div>
			</td>
		</tr>
		--><? if ( $esfera )  { ?> <script type="text/javascript"> onOffCampo( 'esfera' ); </script> <? } ?>
		<tr>
			<td width="195" class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'funcao' );">
				Função
			</td>
			<td>
				<div id="funcao_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'funcao' );" ><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="funcao_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT
										funcod AS codigo, 
										funcod || ' - ' || fundsc AS descricao 
									  FROM 
									  	dw.funcao
									  ORDER BY
									  	funcod";

						if ( $_REQUEST['funcao'] && $_REQUEST['funcao'][0] != '' )
						{
							$sql_carregados = "SELECT
											 		funcod AS codigo, 
													funcod || ' - ' || fundsc AS descricao 
											   FROM
											   		dw.funcao
											   WHERE
											   		funcod in ('".implode("','", $_REQUEST['funcao'])."')";
							$funcao = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'funcao', $sql_combo, 'Selecione a(s) Função(ões)', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true ); 
					?>
				</div>
			</td>
		</tr>
		<? if ( $funcao )  { ?> <script type="text/javascript"> onOffCampo( 'funcao' ); </script> <? } ?>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'subfuncao' );">
				Sub-Função
			</td>
			<td>
				<div id="subfuncao_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'subfuncao' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="subfuncao_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT
										sfucod as codigo, 
										sfucod || ' - ' || sfudsc as descricao 
									  FROM 
									  	dw.subfuncao
									  ORDER BY
									  	sfucod"; 
						
						if ( $_REQUEST['subfuncao'] && $_REQUEST['subfuncao'][0] != '' )
						{
							$sql_carregados = "SELECT
											 		sfucod as codigo, 
													sfucod || ' - ' || sfudsc as descricao 
											   FROM
											   		dw.subfuncao
											   WHERE
											   		sfucod in ('".implode("','", $_REQUEST['subfuncao'])."')";
							$subfuncao = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'subfuncao', $sql_combo, 'Selecione a(s) Sub-Função(ões)', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true ); 
					?>
				</div>
			</td>
		</tr>
		<? if ( $subfuncao )  { ?> <script type="text/javascript"> onOffCampo( 'subfuncao' ); </script> <? } ?>
		<tr>
			<td width="195" class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'programa' );">
				Programa
			</td>
			<td>
				<div id="programa_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'programa' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="programa_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT DISTINCT
										prgcod AS codigo, 
										prgcod || ' - ' || prgdsc AS descricao 
									  FROM
									  	dw.programa
									  ORDER BY 
									  	prgcod";
						 
						if ( $_REQUEST['programa'] && $_REQUEST['programa'][0] != '' )
						{
							$sql_carregados = "SELECT
											 		prgcod AS codigo, 
													prgcod || ' - ' || prgdsc AS descricao 
											   FROM
											   		dw.programa
											   WHERE
											   		prgcod in ('".implode("','", $_REQUEST['programa'])."')";
							$programa = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'programa', $sql_combo, 'Selecione o(s) Programa(s)', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true ); 
					?>
				</div>
			</td>
		</tr>
		<? if ( $programa )  { ?> <script type="text/javascript"> onOffCampo( 'programa' ); </script> <? } ?>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'acacod' );">
				Ação
			</td>
			<td>
				<div id="acacod_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'acacod' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="acacod_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT DISTINCT
									 	acacod AS codigo, 
									 	acacod || ' - ' || acadsc AS descricao 
									  FROM
									  	dw.acao
									  ORDER BY
									  	acacod"; 
						
						if ( $_REQUEST['acacod'] && $_REQUEST['acacod'][0] != '' )
						{
							$sql_carregados = "SELECT DISTINCT
											 		acacod AS codigo, 
									 				acacod || ' - ' || acadsc AS descricao 
											   FROM
											   		dw.acao
											   WHERE
											   		acacod in ('".implode("','", $_REQUEST['acacod'])."')";
							$acacod = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'acacod', $sql_combo, 'Selecione a(s) Ação(ões)', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, true ); 
					?>
				</div>
			</td>
		</tr>
		<? if ( $acacod )  { ?> <script type="text/javascript"> onOffCampo( 'acacod' ); </script> <? } ?>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'ptres' );">
				Ptres
			</td>
			<td>
				<div id="ptres_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'ptres' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="ptres_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT
									 	pt.ptres AS codigo, 
									 	pt.ptres AS descricao 
									  FROM
									  	dw.ptres pt
									  ORDER BY
									  	pt.ptres"; 
						
						if ( $_REQUEST['ptres'] && $_REQUEST['ptres'][0] != '' )
						{
							$sql_carregados = "SELECT
											 		pt.ptres AS codigo, 
									 				pt.ptres AS descricao 
											   FROM
											   		dw.ptres pt
											   WHERE
											   		pt.ptres in ('".implode("','", $_REQUEST['ptres'])."')";
							$ptres = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'ptres', $sql_combo, 'Selecione o(s) Ptres', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true ); 
					?>
				</div>
			</td>
		</tr>
		<? if ( $ptres )  { ?> <script type="text/javascript"> onOffCampo( 'ptres' ); </script> <? } ?>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'planointerno' );">
				Plano Interno
			</td>
			<td>
				<div id="planointerno_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'planointerno' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="planointerno_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT
									 	plicod AS codigo, 
									 	plicod || ' - ' || plidsc AS descricao 
									  FROM
									  	dw.planointerno
									  WHERE
									  	length(plicod) = 11
									  ORDER BY
									  	plicod"; 
						
						if ( $_REQUEST['planointerno'] && $_REQUEST['planointerno'][0] != '' )
						{
							$sql_carregados = "SELECT
											 		plicod AS codigo, 
									 				plicod || ' - ' || plidsc AS descricao 
											   FROM
											   		dw.planointerno
											   WHERE
											   		length(plicod) = 11 AND 
											   		plicod in ('".implode("','", $_REQUEST['planointerno'])."')";
							$planointerno = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'planointerno', $sql_combo, 'Selecione o(s) Plano(s) Interno(s)', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true ); 
					?>
				</div>
			</td>
		</tr>
		<? if ( $planointerno )  { ?> <script type="text/javascript"> onOffCampo( 'planointerno' ); </script> <? } ?>
		<?
		// Mês
//		$stSql = array(
//						array(
//							"codigo" 	=> "1",
//							"descricao" => "Janeiro"
//							 ),
//						array(
//							"codigo" 	=> "2",
//							"descricao" => "Fevereiro"
//							 ),
//						array(
//							"codigo" 	=> "3",
//							"descricao" => "Março"
//							 ),
//						array(
//							"codigo" 	=> "4",
//							"descricao" => "Abril"
//							 ),
//						array(
//							"codigo" 	=> "5",
//							"descricao" => "Maio"
//							 ),
//						array(
//							"codigo" 	=> "6",
//							"descricao" => "Junho"
//							 ),
//						array(
//							"codigo" 	=> "7",
//							"descricao" => "Julho"
//							 ),
//						array(
//							"codigo" 	=> "8",
//							"descricao" => "Agosto"
//							 ),
//						array(
//							"codigo" 	=> "9",
//							"descricao" => "Setembro"
//							 ),
//						array(
//							"codigo" 	=> "10",
//							"descricao" => "Outubro"
//							 ),
//						array(
//							"codigo" 	=> "11",
//							"descricao" => "Novembro"
//							 ),
//						array(
//							"codigo" 	=> "12",
//							"descricao" => "Dezembro"
//							 )
//					   );
		/*$stSql = "		SELECT
							1 AS codigo,
							'Janeiro' AS descricao
					UNION ALL
						SELECT
							2 AS codigo,
							'Fevereiro' AS descricao
					UNION ALL
						SELECT
							3 AS codigo,
							'Março' AS descricao
					UNION ALL
						SELECT
							4 AS codigo,
							'Abril' AS descricao
					UNION ALL
						SELECT
							5 AS codigo,
							'Maio' AS descricao
					UNION ALL
						SELECT
							6 AS codigo,
							'Junho' AS descricao
					UNION ALL
						SELECT
							7 AS codigo,
							'Julho' AS descricao
					UNION ALL
						SELECT
							8 AS codigo,
							'Agosto' AS descricao
					UNION ALL
						SELECT
							9 AS codigo,
							'Setembro' AS descricao
					UNION ALL
						SELECT
							10 AS codigo,
							'Outubro' AS descricao
					UNION ALL
						SELECT
							11 AS codigo,
							'Novembro' AS descricao
					UNION ALL
						SELECT
							12 AS codigo,
							'Dezembro' AS descricao";
		$stSqlCarregados = "";
		mostrarComboPopup( 'Mês', 'mes',  $stSql, $stSqlCarregados, 'Selecione o(s) Meses(s)' ); */
		?>		
	</table>
</div>

<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
	<tr>
		<td onclick="javascript:onOffBloco( 'econ' );">
			<img border="0" src="/imagens/mais.gif" id="econ_img"/>&nbsp;
			Classificação Econômica
		</td>
	</tr>
</table>
<div id="econ_div_filtros_off"></div>
<div id="econ_div_filtros_on" style="display:none;">
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
		<tr>
			<td width="195" class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'grf' );">
				Grupo Fonte
			</td>
			<td>
				<div id="grf_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'grf' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="grf_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT 
										grfid AS codigo, 
										grfid || ' - ' || grfdsc AS descricao 
									  FROM 
									  	dw.grupofonte
									  ORDER BY
									  	grfid"; 
						
						if ( $_REQUEST['grf'] && $_REQUEST['grf'][0] != '' )
						{
							$sql_carregados = "SELECT
											 		grfid AS codigo, 
													grfid || ' - ' || grfdsc AS descricao 
											   FROM
											   		dw.grupofonte
											   WHERE
											   		grfid in (".implode(",", $_REQUEST['grf']).")";
							$grf = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'grf', $sql_combo, 'Selecione o(s) Grupo(s) de Fonte', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true );
					?>
				</div>
			</td>
		</tr>
		<? if ( $grf )  { ?> <script type="text/javascript"> onOffCampo( 'grf' ); </script> <? } ?>
		<tr>
			<td width="195" class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'fonte' );">
				Fonte SOF
			</td>
			<td>
				<div id="fonte_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'fonte' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="fonte_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT 
										foncod AS codigo, 
										foncod || ' - ' || fondsc AS descricao 
									  FROM 
									  	dw.fontesof
									  ORDER BY 
									  	foncod";
					
						if ( $_REQUEST['fonte'] && $_REQUEST['fonte'][0] != '' )
						{
							$sql_carregados = "SELECT
											 		foncod AS codigo, 
													foncod || ' - ' || fondsc AS descricao 
											   FROM
											   		dw.fontesof
											   WHERE
											   		foncod in ('".implode("','", $_REQUEST['fonte'])."')";
							$fonte = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'fonte', $sql_combo, 'Selecione a(s) Fonte(s) SOF', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true );
					?>
				</div>
			</td>
		</tr>
		<? if ( $fonte )  { ?> <script type="text/javascript"> onOffCampo( 'fonte' ); </script> <? } ?>
		<tr>
			<td width="195" class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'fonterecurso' );">
				Fonte de Recurso
			</td>
			<td>
				<div id="fonterecurso_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'fonterecurso' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="fonterecurso_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT 
										codigo AS codigo, 
										codigo || ' - ' || descricao AS descricao 
									  FROM 
									  	dw.fonterecursos
									  ORDER BY 
									  	codigo";
					
						if ( $_REQUEST['fonterecurso'] && $_REQUEST['fonterecurso'][0] != '' )
						{
							$sql_carregados = "SELECT
											 		codigo AS codigo, 
													codigo || ' - ' || descricao AS descricao 
											   FROM
											   		dw.fonterecursos
											   WHERE
											   		codigo in ('".implode("','", $_REQUEST['fonterecurso'])."')";
							$fonterecurso = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'fonterecurso', $sql_combo, 'Selecione a(s) Fonte(s) de Recurso', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true );
					?>
				</div>
			</td>
		</tr>
		<? if ( $fonterecurso )  { ?> <script type="text/javascript"> onOffCampo( 'fonterecurso' ); </script> <? } ?>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'fontesiafi' );">
				Fonte Detalhada
			</td>
			<td>
				<div id="fontesiafi_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'fontesiafi' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="fontesiafi_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT
										foscod AS codigo,
										foscod || ' - ' || fosdsc AS descricao 
									  FROM
									  	dw.fontesiafi
									  ORDER BY
									  	foscod"; 

						if ( $_REQUEST['fontesiafi'] && $_REQUEST['fontesiafi'][0] != '' )
						{
							$sql_carregados = "SELECT
												foscod AS codigo,
												foscod || ' - ' || fosdsc AS descricao
											   FROM
												dw.fontesiafi
											   WHERE
											   	foscod in ('".implode("','", $_REQUEST['fontesiafi'])."')";
							$fontesiafi = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'fontesiafi', $sql_combo, 'Selecione a(s) Fonte(s)', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true );
					?>
				</div>
			</td>
		</tr>
		<? if ( $fontesiafi )  { ?> <script type="text/javascript"> onOffCampo( 'fontesiafi' ); </script> <? } ?>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'catecon' );">
				Categoria Econômica
			</td>
			<td>
				<div id="catecon_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'catecon' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="catecon_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT
										ctecod AS codigo, 
										ctecod || ' - ' || ctedsc AS descricao 
									  FROM 
									  	dw.categoriaeconomica
									  ORDER BY
									  	ctecod";
						
						if ( $_REQUEST['catecon'] && $_REQUEST['catecon'][0] != '' )
						{
							$sql_carregados = "SELECT
											 		ctecod AS codigo, 
													ctecod || ' - ' || ctedsc AS descricao 
											   FROM
											   		dw.categoriaeconomica
											   WHERE
											   		ctecod in (".implode(",", $_REQUEST['catecon']).")";
							$catecon = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'catecon', $sql_combo, 'Selecione o(s) GND(s)', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true );
					?>
				</div>
			</td>
		</tr>
		<? if ( $catecon )  { ?> <script type="text/javascript"> onOffCampo( 'catecon' ); </script> <? } ?>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'gnd' );">
				GND
			</td>
			<td>
				<div id="gnd_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'gnd' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="gnd_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT
										gndcod AS codigo, 
										gndcod || ' - ' || gnddsc AS descricao 
									  FROM
									  	dw.gnd
									  ORDER BY
									  	gndcod"; 
						
						if ( $_REQUEST['gnd'] && $_REQUEST['gnd'][0] != '' )
						{
							$sql_carregados = "SELECT
											 		gndcod AS codigo, 
													gndcod || ' - ' || gnddsc AS descricao 
											   FROM
											   		dw.gnd
											   WHERE
											   		gndcod in (".implode(",", $_REQUEST['gnd']).")";
							$gnd = $db2->carregar( $sql_carregados );
							//dbg($sql_carregados);
							//$gnd = array();
						}
						//dbg($sql_combo);
						combo_popup( 'gnd', $sql_combo, 'Selecione o(s) GND(s)', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true );
					?>
				</div>
			</td>
		</tr>
		<? if ( $gnd )  { ?> <script type="text/javascript"> onOffCampo( 'gnd' ); </script> <? } ?>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'mapcod' );">
				Modalidade de Aplicação
			</td>
			<td>
				<div id="mapcod_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'mapcod' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="mapcod_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT 
										mapcod AS codigo, 
										mapcod || ' - ' || mapdsc AS descricao 
									  FROM
									  	dw.modalidadeaplicacao
									  ORDER BY
									  	mapcod";
						 
						if ( $_REQUEST['mapcod'] && $_REQUEST['mapcod'][0] != '' )
						{
							$sql_carregados = "SELECT
											 		mapcod AS codigo, 
													mapcod || ' - ' || mapdsc AS descricao 
											   FROM
											   		dw.modalidadeaplicacao
											   WHERE
											   		mapcod in ('".implode("','", $_REQUEST['mapcod'])."')";
							$mapcod = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'mapcod', $sql_combo, 'Selecione a(s) Modalidade(s) de Aplicação', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true );
					?>
				</div>
			</td>
		</tr>
		<? if ( $mapcod )  { ?> <script type="text/javascript"> onOffCampo( 'mapcod' ); </script> <? } ?>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'elemento' );">
				Elemento de Despesa
			</td>
			<td>
				<div id="elemento_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'elemento' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="elemento_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT
										edpcod AS codigo, 
										edpcod || ' - ' || edpdsc AS descricao 
									  FROM
									  	dw.elementodespesa
									  ORDER BY
									  	edpcod";
						
						if ( $_REQUEST['elemento'] && $_REQUEST['elemento'][0] != '' )
						{
							$sql_carregados = "SELECT
											 		edpcod AS codigo, 
													edpcod || ' - ' || edpdsc AS descricao 
											   FROM
											   		dw.elementodespesa
											   WHERE
											   		edpcod in ('".implode("','", $_REQUEST['elemento'])."')";
							$elemento = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'elemento', $sql_combo, 'Selecione o(s) Elemento(s)', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true );
					?>
				</div>
			</td>
		</tr>
		<? if ( $elemento )  { ?> <script type="text/javascript"> onOffCampo( 'elemento' ); </script> <? } ?>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'natureza' );">
				Natureza de Despesa
			</td>
			<td>
				<div id="natureza_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'natureza' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="natureza_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT
										cast(ctecod AS varchar) || cast(gndcod AS varchar) || mapcod || edpcod AS codigo,
										ctecod || '.' || gndcod || '.' || mapcod || '.' || edpcod || ' - ' || ndpdsc AS descricao 
									  FROM
									  	dw.naturezadespesa
									  ORDER BY
									  	codigo"; 
					
						if ( $_REQUEST['natureza'] && $_REQUEST['natureza'][0] != '' )
						{
							$sql_carregados = "SELECT
											 		cast(ctecod AS varchar) || cast(gndcod AS varchar) || mapcod || edpcod AS codigo,
													ctecod || '.' || gndcod || '.' || mapcod || '.' || edpcod || ' - ' || ndpdsc AS descricao 
											   FROM
											   		dw.naturezadespesa
											   WHERE
											   		cast(ctecod AS varchar) || cast(gndcod AS varchar) || mapcod || edpcod in ('".implode("','", $_REQUEST['natureza'])."')";
							$natureza = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'natureza', $sql_combo, 'Selecione a(s) Natureza(s)', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true );
					?>
				</div>
			</td>
		</tr>
		<? if ( $natureza )  { ?> <script type="text/javascript"> onOffCampo( 'natureza' ); </script> <? } ?>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'naturezadet' );">
				Natureza de Despesa Detalhada
			</td>
			<td>
				<div id="naturezadet_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'naturezadet' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="naturezadet_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT
										cast(ctecod AS varchar) || cast(gndcod AS varchar) || mapcod || edpcod || sbecod AS codigo,
										ctecod || '.' || gndcod || '.' || mapcod || '.' || edpcod || '.' || sbecod || ' - ' || ndpdsc AS descricao 
									  FROM
									  	dw.naturezadespesa where sbecod <> '00'
									  ORDER BY
									  	codigo"; 
					
						if ( $_REQUEST['naturezadet'] && $_REQUEST['naturezadet'][0] != '' )
						{
							$sql_carregados = "SELECT
											 		cast(ctecod AS varchar) || cast(gndcod AS varchar) || mapcod || edpcod || sbecod AS codigo,
													ctecod || '.' || gndcod || '.' || mapcod || '.' || edpcod || '.' || sbecod || ' - ' || ndpdsc AS descricao 
											   FROM
											   		dw.naturezadespesa
											   WHERE
											   		cast(ctecod AS varchar) || cast(gndcod AS varchar) || mapcod || edpcod || sbecod in ('".implode("','", $_REQUEST['naturezadet'])."')";
							$naturezadet = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'naturezadet', $sql_combo, 'Selecione a(s) Natureza(s) de Despesa Detalhadas', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true );
					?>
				</div>
			</td>
		</tr>
		<? if ( $naturezadet )  { ?> <script type="text/javascript"> onOffCampo( 'naturezadet' ); </script> <? } ?>

		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'iduso' );">
				IDUSO
			</td>
			<td>
				<div id="iduso_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'iduso' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="iduso_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT
										iducod AS codigo,
										iducod || ' - ' || idudsc AS descricao 
									  FROM
									  	dw.identifuso
									  ORDER BY
									  	iducod"; 
					
						if ( $_REQUEST['iduso'] && $_REQUEST['iduso'][0] != '' )
						{
							$sql_carregados = "SELECT
											 		iducod AS codigo,
													iducod || ' - ' || idudsc AS descricao 
											   FROM
											   		dw.identifuso
											   WHERE
											   		iducod in ('".implode("','", $_REQUEST['iduso'])."')";
							$iduso = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'iduso', $sql_combo, 'Selecione o(s) IDUSO(s)', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true );
					?>
				</div>
			</td>
		</tr>
		<? if ( $iduso )  { ?> <script type="text/javascript"> onOffCampo( 'iduso' ); </script> <? } ?>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'sldcontacorrente' );">
				Conta Corrente
			</td>
			<td>
				<div id="sldcontacorrente_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'sldcontacorrente' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="sldcontacorrente_campo_on" style="display:none;">
					<table cellspacing="0" cellpadding="0" border="0" width="400">
						<tbody>
							<tr>
								<td align="left">
									<input id="busca_conta_corrente" class="normal" type="text" style="margin: 2px 0pt;" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);" onmouseover="MouseOver( this );" maxlength="43" size="40" />
									<img align="absmiddle" onclick="adicionar_conta_corrente();" src="/imagens/check_p.gif" title="adicionar"/>
									<img align="absmiddle" onclick="remover_conta_corrente();" src="/imagens/exclui_p.gif" title="remover"/>
								</td>
							</tr>
						</tbody>
					</table>
					<select id="sldcontacorrente" class="CampoEstilo" style="width: 400px;" ondblclick="javascript:remover_conta_corrente();" name="sldcontacorrente[]" size="10" multiple="multiple"></select>
				</div>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'modlic' );">
				Modalidade de Licitação
			</td>
			<td>
				<div id="modlic_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'modlic' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="modlic_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT
										mdlcod AS codigo,
										mdlcod || ' - ' || mdldsc AS descricao 
									  FROM
									  	dw.modalidadelicitacao
									  ORDER BY
									  	mdlcod"; 
					
						if ( $_REQUEST['modlic'] && $_REQUEST['modlic'][0] != '' )
						{
							$sql_carregados = "SELECT
											 		mdlcod AS codigo,
													mdlcod || ' - ' || mdldsc AS descricao 
											   FROM
											   		dw.modalidadelicitacao
											   WHERE
											   		mdlcod in ('".implode("','", $_REQUEST['modlic'])."')";
							$modlic = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'modlic', $sql_combo, 'Selecione a(s) Modalidade(s) de Aplicação(ões)', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true );
					?>
				</div>
			</td>
		</tr>
		<? if ( $modlic )  { ?> <script type="text/javascript"> onOffCampo( 'modlic' ); </script> <? } ?>
	</table>
</div>
<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
	<tr>
		<td onclick="javascript:onOffBloco( 'financeiro' );">
			<img border="0" src="/imagens/mais.gif" id="financeiro_img"/>&nbsp;
			Financeiro
		</td>
	</tr>
</table>
<div id="financeiro_div_filtros_off"></div>
<div id="financeiro_div_filtros_on" style="display:none;">
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
		<tr>
			<td class="SubTituloDireita" width="15%" valign="top" onclick="javascript:onOffCampo( 'recurso' );">
				Recurso
			</td>
			<td>
				<div id="recurso_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'recurso' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="recurso_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT
										trrcod AS codigo,
										trrcod || ' - ' || trrdsc AS descricao 
									  FROM
									  	dw.tiporecurso
									  ORDER BY
									  	trrcod"; 
					
						if ( $_REQUEST['recurso'] && $_REQUEST['recurso'][0] != '' )
						{
							$sql_carregados = "SELECT
											 	trrcod AS codigo,
												trrcod || ' - ' || trrdsc AS descricao	 
											   FROM
											   	dw.tiporecurso
											   WHERE
											   	trrcod in (".implode(",", $_REQUEST['recurso']).")";
							$recurso = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'recurso', $sql_combo, 'Selecione o(s) Recurso(s)', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true );
					?>
				</div>
			</td>
		</tr>
		<? if ( $recurso )  { ?> <script type="text/javascript"> onOffCampo( 'recurso' ); </script> <? } ?>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'vincod' );">
				Vinculação de Pagamento
			</td>
			<td>
				<div id="vincod_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'vincod' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="vincod_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT DISTINCT
										it_co_vinculacao_pagamento AS codigo,
										it_co_vinculacao_pagamento || ' - ' || it_no_vinculacao_pagamento AS descricao 
									  FROM
									  	dw.vinculacaopagamento
									  ORDER BY
									  	it_co_vinculacao_pagamento"; 
					
						if ( $_REQUEST['vincod'] && $_REQUEST['vincod'][0] != '' )
						{
							$sql_carregados = "SELECT DISTINCT
													it_co_vinculacao_pagamento AS codigo,
													it_co_vinculacao_pagamento || ' - ' || it_no_vinculacao_pagamento AS descricao  
											   FROM
											   	dw.vinculacaopagamento
											   WHERE
											   	it_co_vinculacao_pagamento in ('".implode("','", $_REQUEST['vincod'])."')";
							$vincod = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'vincod', $sql_combo, 'Selecione a(s) Vinculação(ões) de Pagamento', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true );
					?>
				</div>
			</td>
		</tr>
		<? if ( $vincod )  { ?> <script type="text/javascript"> onOffCampo( 'vincod' ); </script> <? } ?>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'cagcod' );">
				Categoria de Gasto
			</td>
			<td>
				<div id="cagcod_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'cagcod' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="cagcod_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT
										cagcod AS codigo,
										cagcod || ' - ' || cagdsc AS descricao 
									  FROM
									  	dw.categoriagasto
									  ORDER BY
									  	cagcod"; 
					
						if ( $_REQUEST['cagcod'] && $_REQUEST['cagcod'][0] != '' )
						{
							$sql_carregados = "SELECT
											 	cagcod AS codigo,
												cagcod || ' - ' || cagdsc AS descricao	 
											   FROM
											   	dw.categoriagasto
											   WHERE
											   	cagcod in ('".implode("','", $_REQUEST['cagcod'])."')";
							$cagcod = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'cagcod', $sql_combo, 'Selecione a(s) Categoria(s) de Gasto', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true );
					?>
				</div>
			</td>
		</tr>
		<? if ( $cagcod )  { ?> <script type="text/javascript"> onOffCampo( 'cagcod' ); </script> <? } ?>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'loccod' );">
				Subtítulo
			</td>
			<td>
				<div id="loccod_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'loccod' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="loccod_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT DISTINCT
										loccod AS codigo,
										loccod AS descricao 
									  FROM
									  	dw.saldo
									  ORDER BY
									  	loccod"; 
					
						if ( $_REQUEST['loccod'] && $_REQUEST['loccod'][0] != '' )
						{
							$sql_carregados = "SELECT DISTINCT
											 	loccod AS codigo,
												loccod AS descricao	 
											   FROM
											   	dw.saldo
											   WHERE
											   	loccod in ('".implode("','", $_REQUEST['loccod'])."')";
							$loccod = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'loccod', $sql_combo, 'Selecione o(s) Subtítulo(s)', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true );
					?>
				</div>
			</td>
		</tr>
		<? if ( $loccod )  { ?> <script type="text/javascript"> onOffCampo( 'loccod' ); </script> <? } ?>
	</table>
</div>
<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
	<tr>
		<td onclick="javascript:onOffBloco( 'planointerno' );">
			<img border="0" src="/imagens/mais.gif" id="planointerno_img"/>&nbsp;
			Plano Interno
		</td>
	</tr>
</table>
<div id="planointerno_div_filtros_off"></div>
<div id="planointerno_div_filtros_on" style="display:none;">
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
		<tr>
			<td class="SubTituloDireita" width="15%" valign="top" onclick="javascript:onOffCampo( 'enquadramento' );">
				Enquadramento da Despesa
			</td>
			<td>
				<div id="enquadramento_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'enquadramento' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="enquadramento_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT 
										cdtcod AS codigo, 
										cdtcod || ' - ' || cdtdsc AS descricao 
									  FROM 
									  	public.combodadostabela 
									  WHERE 
									  	ctbid = 5 AND 
					   		 			cdtstatus = 'A'
					   		 		ORDER BY
					   		 			cdtcod";
					
						if ( $_REQUEST['enquadramento'] && $_REQUEST['enquadramento'][0] != '' )
						{
							$sql_carregados = "SELECT 
												cdtcod AS codigo, 
												cdtcod || ' - ' || cdtdsc AS descricao 
											  FROM 
											  	public.combodadostabela 
											  WHERE 
											  	ctbid = 5 AND
											  	cdtstatus = 'A' AND
											   	cdtid in (".implode(",", $_REQUEST['enquadramento']).")";
							$enquadramento = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'enquadramento', $sql_combo, 'Selecione o(s) Enquadramento(s) da Despesa', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, true, null, true );
					?>
				</div>
			</td>
		</tr>
		<? if ( $enquadramento )  { ?> <script type="text/javascript"> onOffCampo( 'enquadramento' ); </script> <? } ?>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'executor' );">
				Executor Orçamentário e Financeiro
			</td>
			<td>
				<div id="executor_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'executor' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="executor_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT 
										cdtcod AS codigo, 
										cdtcod || ' - ' || cdtdsc AS descricao 
									FROM 
										public.combodadostabela 
									WHERE 
										ctbid = 3 AND 
					   		 			cdtstatus = 'A'
					   		 		ORDER BY
					   		 			cdtcod"; 
					
						if ( $_REQUEST['executor'] && $_REQUEST['executor'][0] != '' )
						{
							$sql_carregados = "SELECT 
												cdtcod AS codigo, 
												cdtcod || ' - ' || cdtdsc AS descricao 
											FROM 
												public.combodadostabela 
											WHERE 
												ctbid = 3 AND
												cdtstatus = 'A' AND 
											   	cdtid in (".implode(",", $_REQUEST['executor']).")";
							$executor = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'executor', $sql_combo, 'Selecione o(s) Executor(es)', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, true, null, true );
					?>
				</div>
			</td>
		</tr>
		<? if ( $executor )  { ?> <script type="text/javascript"> onOffCampo( 'executor' ); </script> <? } ?>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'gestor' );">
				Gestor da Subação
			</td>
			<td>
				<div id="gestor_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'gestor' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="gestor_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT 
										cdtcod AS codigo, 
										cdtcod || ' - ' || cdtdsc AS descricao 
									FROM 
										public.combodadostabela 
									WHERE 
										ctbid = 4 AND 
					   		 			cdtstatus = 'A'
					   		 		ORDER BY
					   		 			cdtcod"; 
					
						if ( $_REQUEST['gestor'] && $_REQUEST['gestor'][0] != '' )
						{
							$sql_carregados = "SELECT 
												cdtcod AS codigo, 
												cdtcod || ' - ' || cdtdsc AS descricao 
											FROM 
												public.combodadostabela 
											WHERE 
												ctbid = 4 AND
												cdtstatus = 'A' AND 
											   	cdtid in (".implode(",", $_REQUEST['gestor']).")";
							$gestor = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'gestor', $sql_combo, 'Selecione o(s) Gestor(es) da Subação', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, true, null, true );
					?>
				</div>
			</td>
		</tr>
		<? if ( $gestor )  { ?> <script type="text/javascript"> onOffCampo( 'gestor' ); </script> <? } ?>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'nivel' );">
				Nível/Etapa de Ensino
			</td>
			<td>
				<div id="nivel_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'nivel' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="nivel_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT 
										cdtcod AS codigo, 
										cdtcod || ' - ' || cdtdsc AS descricao
					   		 		FROM 
					   		 			public.combodadostabela 
					   		 		WHERE 
					   		 			ctbid = 6 AND 
					   		 			cdtstatus = 'A'
					   		 		ORDER BY
					   		 			cdtcod"; 
					
						if ( $_REQUEST['nivel'] && $_REQUEST['nivel'][0] != '' )
						{
							$sql_carregados = "SELECT 
												cdtcod AS codigo, 
												cdtcod || ' - ' || cdtdsc AS descricao
							   		 		FROM 
							   		 			public.combodadostabela 
							   		 		WHERE 
							   		 			ctbid = 6 AND 
							   		 			cdtstatus = 'A' AND
											   	cdtid in (".implode(",", $_REQUEST['nivel']).")";
							$nivel = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'nivel', $sql_combo, 'Selecione o(s) Nível(eis)/Etapa(s) de Ensino', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, true, null, true );
					?>
				</div>
			</td>
		</tr>
		<? if ( $nivel )  { ?> <script type="text/javascript"> onOffCampo( 'nivel' ); </script> <? } ?>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'apropriacao' );">
				Categoria de Apropriação
			</td>
			<td>
				<div id="apropriacao_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'apropriacao' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="apropriacao_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT 
										cdtcod AS codigo, 
										cdtcod || ' - ' || cdtdsc AS descricao
					    			FROM 
					    				public.combodadostabela 
					    			WHERE 
					    				ctbid = 7 AND 
					    				cdtstatus = 'A' 
					    			ORDER BY
					    				cdtcod"; 
					
						if ( $_REQUEST['apropriacao'] && $_REQUEST['apropriacao'][0] != '' )
						{
							$sql_carregados = "SELECT 
												cdtcod AS codigo, 
												cdtcod || ' - ' || cdtdsc AS descricao
							    			FROM 
							    				public.combodadostabela 
							    			WHERE 
							    				ctbid = 7 AND 
							    				cdtstatus = 'A' AND
											   	cdtid in (".implode(",", $_REQUEST['apropriacao']).")";
							$apropriacao = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'apropriacao', $sql_combo, 'Selecione a(s) Categoria(s) de Apropriação', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true );
					?>
				</div>
			</td>
		</tr>
		<? if ( $apropriacao )  { ?> <script type="text/javascript"> onOffCampo( 'apropriacao' ); </script> <? } ?>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'modalidade' );">
				Modalidade de Ensino
			</td>
			<td>
				<div id="modalidade_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'modalidade' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="modalidade_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT 
										cdtcod AS codigo, 
										cdtcod || ' - ' || cdtdsc AS descricao
						    		FROM 
						    			public.combodadostabela 
						    		WHERE 
						    			ctbid = 8 AND 
						    			cdtstatus = 'A' 
						    		ORDER BY 
						    			cdtcod"; 
					
						if ( $_REQUEST['modalidade'] && $_REQUEST['modalidade'][0] != '' )
						{
							$sql_carregados = "SELECT 
												cdtcod AS codigo, 
												cdtcod || ' - ' || cdtdsc AS descricao
								    		FROM 
								    			public.combodadostabela 
								    		WHERE 
								    			ctbid = 8 AND 
								    			cdtstatus = 'A' AND
											   	cdtid in (".implode(",", $_REQUEST['modalidade']).")";
							$modalidade = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'modalidade', $sql_combo, 'Selecione a(s) Modalidade(s) de Ensino', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, true, null, true );
					?>
				</div>
			</td>
		</tr>
		<? if ( $modalidade )  { ?> <script type="text/javascript"> onOffCampo( 'modalidade' ); </script> <? } ?>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'subacao' );">
				Subação
			</td>
			<td>
				<div id="subacao_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'subacao' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="subacao_campo_on" style="display:none;">
					<? 
						$sql_combo = "SELECT 
										sbacod AS codigo, 
										sbacod || ' - ' || sbadsc AS descricao
						    		FROM 
						    			financeiro.subacao
						    		WHERE
						    			".(($unicods_CONSULTAUNIDADE)?" unicod in('".implode("','",$unicods_CONSULTAUNIDADE)."') AND ":"")." sbastatus = 'A' 
						    		ORDER BY 
						    			sbacod"; 
					
						if ( $_REQUEST['subacao'] && $_REQUEST['subacao'][0] != '' )
						{
							$sql_carregados = "SELECT 
												sbacod AS codigo, 
												sbacod || ' - ' || sbadsc AS descricao
								    		FROM 
								    			financeiro.subacao
								    		WHERE 
								    			".(($unicods_CONSULTAUNIDADE)?" unicod in('".implode("','",$unicods_CONSULTAUNIDADE)."') AND ":"")." sbastatus = 'A' 
								    		AND
											   	sbacod in ('".implode("','", $_REQUEST['subacao'])."')";
							$subacao = $db2->carregar( $sql_carregados );
						}
						
						combo_popup( 'subacao', $sql_combo, 'Selecione a(s) Subação(ões)', '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, $dados_conexao, null, null, true, false, null, true );
					?>
				</div>
			</td>
		</tr>
		<? if ( $subacao )  { ?> <script type="text/javascript"> onOffCampo( 'subacao' ); </script> <? } ?>
	</table>
</div>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
	<tr>
		<td align="center">
			<input type="button" name="relatorio" value="Gerar Relatório" onclick="javascript:submeterFormulario('relatorio');" />
			&nbsp;
			<input type="button" name="planilha" value="Exportar Planilha" onclick="javascript:submeterFormulario('planilha');" />
			&nbsp;
			<input type="button" name="consulta" value="Salvar Consulta" onclick="javascript:submeterFormulario('salvar');" />
		</td>
	</tr>
</table>

</form>

<script type="text/javascript">
onOffBloco( 'inst' );
onOffBloco( 'func' );
onOffBloco( 'econ' );
onOffBloco( 'outros' );
onOffBloco( 'minhasconsultas' );
onOffBloco( 'financeiro' );
onOffBloco( 'planointerno' );
	
function adicionar_conta_corrente() {
	var campoText 	= document.getElementById("busca_conta_corrente");
	var campoSelect	= document.getElementById("sldcontacorrente");
	var option 		= document.createElement('option');
	
	if(campoText.value == "") {
		alert("Você deve informar algum valor.");
		return;
	}
	
	for(var i=0; i<campoSelect.length; i++) {
		if(campoSelect.options[i].value == campoText.value) {
			alert("Já existe um item na lista com este valor.");
			return;
		}
	}
	
	option.text = campoText.value;
	option.value = campoText.value;
	
	try {
		campoSelect.add(option,null); // standards compliant
	}
	catch(ex) {
	  campoSelect.add(option); // IE only
	}
	campoText.value = "";
	campoText.focus();
}

function remover_conta_corrente() {
	var campoSelect	= document.getElementById("sldcontacorrente");
	
	campoSelect.remove(campoSelect.selectedIndex);
}

function submeterFormulario(tipo) {
	if(tipo == 'relatorio') {
		if ( formulario.agrupador.options.length == 0 ) {
			alert( 'Escolha pelo menos um agrupador.' );
		}
		else if ( formulario.agrupadorColunas.options.length == 0 ) {
			alert( 'Escolha pelo menos uma coluna.' );
		}
		else {
			selecionaCombos();
		
			formulario.action = 'financeiro.php?modulo=relatorio/geral_teste&acao=R';
			var janela = window.open( '', 'relatorio', 'width=800,height=700,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			janela.focus();
			formulario.target = 'relatorio';
			formulario.submit();
		}
	}
	
	if(tipo == 'salvar') {
		if(formulario.titulo.value == "") {
			alert('É necessário informar a descrição do relatório.');
			formulario.titulo.focus();
		}
		else if ( formulario.agrupador.options.length == 0 ) {
			alert( 'Escolha pelo menos um agrupador.' );
		}
		else if ( formulario.agrupadorColunas.options.length == 0 ) {
			alert( 'Escolha pelo menos uma coluna.' );
		}
		else {
			var nomesExistentes = new Array();
			<?php
				$sqlNomesConsulta = "SELECT prtdsc FROM public.parametros_tela where usucpf = '{$_SESSION['usucpf']}'";
				$nomesExistentes = $db2->carregar( $sqlNomesConsulta );
				if ( $nomesExistentes )
				{
					foreach ( $nomesExistentes as $linhaNome )
					{
						print "nomesExistentes[nomesExistentes.length] = '" . str_replace( "'", "\'", $linhaNome['prtdsc'] ) . "';";
					}
				}
			?>
			var confirma = true;
			var i, j = nomesExistentes.length;
			for ( i = 0; i < j; i++ )
			{
				if ( nomesExistentes[i].toUpperCase() == document.formulario.titulo.value.toUpperCase() )
				{
					confirma = confirm( 'Deseja alterar a consulta já existente?' );
					break;
				}
			}
			
			if ( confirma )
			{
				selecionaCombos();
				
				formulario.action = 'financeiro.php?modulo=relatorio/geral_teste&acao=R&salvar=1';
				formulario.target = '_top';
				formulario.submit();
			}
		}
	}
	
	if(tipo == 'planilha') {
		if ( formulario.agrupador.options.length == 0 ) {
			alert( 'Escolha pelo menos um agrupador.' );
		}
		else if ( formulario.agrupadorColunas.options.length == 0 ) {
			alert( 'Escolha pelo menos uma coluna.' );
		}
		else {
			selecionaCombos();
		
			formulario.action = 'financeiro.php?modulo=relatorio/geral_teste&acao=R&tipo=xls';
			var janela = window.open( '', 'relatorio', 'width=800,height=700,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			janela.focus();
			formulario.target = 'relatorio';
			formulario.submit();
		}
	}
}

function selecionaCombos() {
	// Agrupadores
	selectAllOptions( formulario.agrupador );
	selectAllOptions( formulario.agrupadorColunas );
	// Institucional 
	selectAllOptions( formulario.orgao );
	selectAllOptions( formulario.orgaougexecutora );
	selectAllOptions( formulario.gestaoexecutora );
	selectAllOptions( formulario.ug );
	selectAllOptions( formulario.ugr );
	selectAllOptions( formulario.uo );
	selectAllOptions( formulario.grupouo );
	// Funcional 
	//selectAllOptions( formulario.esfera );
	selectAllOptions( formulario.funcao );
	selectAllOptions( formulario.subfuncao );
	selectAllOptions( formulario.programa );
	selectAllOptions( formulario.acacod );
	selectAllOptions( formulario.ptres );
	selectAllOptions( formulario.planointerno );
	//selectAllOptions( formulario.mes );
	// Classificação Econômica 
	selectAllOptions( formulario.grf );
	selectAllOptions( formulario.fonte );
	selectAllOptions( formulario.fonterecurso );
	selectAllOptions( formulario.catecon );
	selectAllOptions( formulario.gnd );
	selectAllOptions( formulario.mapcod );
	selectAllOptions( formulario.elemento );
	selectAllOptions( formulario.natureza );
	selectAllOptions( formulario.naturezadet );
	selectAllOptions( formulario.fontesiafi );
	selectAllOptions( formulario.iduso );
	selectAllOptions( formulario.sldcontacorrente );
	selectAllOptions( formulario.modlic );
	// Financeiro
	selectAllOptions( formulario.recurso );
	selectAllOptions( formulario.vincod );
	selectAllOptions( formulario.cagcod );
	selectAllOptions( formulario.loccod );
	// Plano Interno
	selectAllOptions( formulario.enquadramento );
	selectAllOptions( formulario.executor );
	selectAllOptions( formulario.gestor );
	selectAllOptions( formulario.nivel );
	selectAllOptions( formulario.apropriacao );
	selectAllOptions( formulario.modalidade );
	selectAllOptions( formulario.subacao );
}

function tornar_publico( prtid )
{
	document.formulario.publico.value = '1';
	document.formulario.prtid.value = prtid;
	document.formulario.target = '_self';
	document.formulario.submit();
}
	
function excluir_relatorio( prtid )
{
	document.formulario.excluir.value = '1';
	document.formulario.prtid.value = prtid;
	document.formulario.target = '_self';
	document.formulario.submit();
}
	
function carregar_consulta( prtid )
{
	document.formulario.carregar.value = '1';
	document.formulario.prtid.value = prtid;
	document.formulario.target = '_self';
	document.formulario.submit();
}
	
function carregar_relatorio( prtid )
{
	document.formulario.alterar_ano.value = '0';
	document.formulario.prtid.value = prtid;
	//submeterFormulario( 'relatorio' );
	
	selecionaCombos();
		
	formulario.action = 'financeiro.php?modulo=relatorio/geral_teste&acao=R';
	var janela = window.open( '', 'relatorio', 'width=800,height=700,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	janela.focus();
	formulario.target = 'relatorio';
	formulario.submit();
}

function alterarAno()
{
	document.formulario.alterar_ano.value = '1';
	submeterFormulario('relatorio');
}

</script>

