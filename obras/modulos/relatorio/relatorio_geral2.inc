<?php

function obras_monta_coluna_relatorio2(){
	
	$coluna = array();
	
	foreach ( $_REQUEST['colunas'] as $valor ){

		switch( $valor ){
			case 'tipoaquisicao':
				array_push( $coluna, array("campo" 	  => "tipoaquisicao",
								   		   "label" 	  => "Tipo de aquisição",
								   		   "blockAgp" => "",
								   		   "type"	  => "string") );
			break;
			case 'metragem':
				array_push( $coluna, array("campo" 	  => "metragem",
								   		   "label" 	  => "Metragem",
								   		   "blockAgp" => "",
								   		   "type"	  => "string") );
			break;
			case 'situacaoobra':
				array_push( $coluna, array("campo" 	  => "situacaoobra",
								   		   "label" 	  => "Situação da obra",
								   		   "blockAgp" => "",
								   		   "type"	  => "string") );
			break;
			case 'unidade':
				array_push( $coluna, array("campo" 	  => "unidade",
								   		   "label" 	  => "Unidade",
								   		   "blockAgp" => "",
								   		   "type"	  => "string") );
			break;
			case 'campus':
				array_push( $coluna, array("campo" 	  => "campus",
								   		   "label" 	  => "Estabelecimento",
								   		   "blockAgp" => "",
								   		   "type"	  => "string") );
			break;
			
			
			case 'municipio':
				array_push( $coluna, array("campo" 	  => "municipio",
								   		   "label" 	  => "Municipio",
								   		   "blockAgp" => "",
								   		   "type"	  => "string") );
			break;
			case 'estado':
				array_push( $coluna, array("campo" 	  => "estado",
								   		   "label" 	  => "UF",
								   		   "blockAgp" => "",
								   		   "type"	  => "string") );
			break;
			
	//		case 'datainicio':
	//			array_push( $coluna, array("campo" 	  => "datainicio",
	//							   		   "label" 	  => "Início da Execução da Obra",
	//							   		   "blockAgp" => "",
	//							   		   "type"	  => "") );
	//		break;
	//		case 'datafim':
	//			array_push( $coluna, array("campo" 	  => "datafim",
	//							   		   "label" 	  => "Término da Execução da Obra",
	//							   		   "blockAgp" => "",
	//							   		   "type"	  => "") );
	//		break;
			
			case 'ultatualizacao':
				array_push( $coluna, array("campo" 	  => "ultatualizacao",
								   		   "label" 	  => "Última atualização",
								   		   "blockAgp" => "",
								   		   "type"	  => "") );
			break;			
			case 'qtdvistorias':
				array_push( $coluna, array("campo" 	  => "qtdvistorias",
								   		   "label" 	  => "Qtd de vistorias",
								   		   "blockAgp" => "",
								   		   "type"	  => "numeric") );
			break;			
			case 'porcexecucao':
				array_push( $coluna, array("campo" 	  => "porcexecucao",
								   		   "label" 	  => "% de execução",
								   		   "blockAgp" => "",
								   		   "type"	  => "string") );
			break;		

			case 'qtdrestricoes':
				array_push( $coluna, array("campo" 	  => "qtdrestricoes",
								   		   "label" 	  => "Restrições",
								   		   "blockAgp" => "",
								   		   "type"	  => "") );
			break;		
			
			case 'cnpjempresacontratada':
				array_push( $coluna, array("campo" 	  => "cnpjempresacontratada",
								   		   "label" 	  => "CNPJ Empresa contratada",
								   		   "blockAgp" => "",
								   		   "type"	  => "string") );
			break;		
			
			case 'nomeempresacontratada':
				array_push( $coluna, array("campo" 	  => "nomeempresacontratada",
								   		   "label" 	  => "Nome Empresa contratada",
								   		   "blockAgp" => "",
								   		   "type"	  => "") );
			break;
			case 'tipoobra':
				array_push( $coluna, array("campo" 	  => "tipoobra",
								   		   "label" 	  => "Tipo da obra",
								   		   "blockAgp" => "",
								   		   "type"	  => "") );
			break;
			
			case 'subacao':
				array_push( $coluna, array("campo" 	  => "subacao",
								   		   "label" 	  => "Programa",
								   		   "blockAgp" => "",
								   		   "type"	  => "") );
			break;
			
			case 'orgao':
				array_push( $coluna, array("campo" 	  => "orgao",
								   		   "label" 	  => "Orgão",
								   		   "blockAgp" => "",
								   		   "type"	  => "") );
			break;
			
			case 'obrvalorprevisto':
				if( $_REQUEST['pesquisa'] == '1' ){
					array_push( $coluna, array("campo" 	  => "obrvalorprevisto",
									   		   "label" 	  => "Valor Previsto (R$):",
									   		   "blockAgp" => "",
									   		   "html"     => "<div style='color:#0066CC; width:100%; text-align:right;'>{obrvalorprevisto}</div>") );
				}else{
					array_push( $coluna, array("campo" 	  => "obrvalorprevisto",
									   		   "label" 	  => "Valor Previsto (R$):",
									   		   "blockAgp" => "",
									   		   "type"	  => "") );
				}
			break;
			
			case 'obrcustocontrato':
				if( $_REQUEST['pesquisa'] == '1' ){
					array_push( $coluna, array("campo" 	  => "obrcustocontrato",
									   		   "label" 	  => "Valor Contratado da Obra (R$):",
									   		   "html"     => "<div style='color:#0066CC; width:100%; text-align:right;'>{obrcustocontrato}</div>") );
				}else{
					array_push( $coluna, array("campo" 	  => "obrcustocontrato",
									   		   "label" 	  => "Valor Contratado da Obra (R$):",
									   		   "type" 	  => "") );
				}
			break;
			
			case 'moldsc':
				array_push( $coluna, array("campo" 	  => "moldsc",
								   		   "label" 	  => "Modalidade de Licitação",
								   		   "blockAgp" => "",
								   		   "type"	  => "") );
			break;
			
			case 'licitacaouasg':
				array_push( $coluna, array("campo" 	  => "licitacaouasg",
								   		   "label" 	  => "Número da UASG",
								   		   "blockAgp" => "",
								   		   "type"	  => "") );
			break;
			
			case 'numlicitacao':
				array_push( $coluna, array("campo" 	  => "numlicitacao",
								   		   "label" 	  => "Número da Licitação",
								   		   "blockAgp" => "",
								   		   "type"	  => "") );
			break;
			
			case 'obrdtassinaturacontrato':
				array_push( $coluna, array("campo" 	  => "obrdtassinaturacontrato",
								   		   "label" 	  => "Data de Assinatura do Contrato",
								   		   "blockAgp" => "",
								   		   "type"	  => "") );
			break;
			
			case 'dtterminocontrato':
				array_push( $coluna, array("campo" 	  => "dtterminocontrato",
								   		   "label" 	  => "Data de Término do Contrato",
								   		   "blockAgp" => "",
								   		   "type"	  => "") );
			break;
			
			case 'obrdtordemservico':
				array_push( $coluna, array("campo" 	  => "obrdtordemservico",
								   		   "label" 	  => "Data de Ordem de Serviço",
								   		   "blockAgp" => "",
								   		   "type"	  => "") );
			break;
			
			case 'obrdtinicio':
				array_push( $coluna, array("campo" 	  => "obrdtinicio",
								   		   "label" 	  => "Início de Execução da Obra",
								   		   "blockAgp" => "",
								   		   "type"	  => "") );
			break;
			
			case 'obrdttermino':
				array_push( $coluna, array("campo" 	  => "obrdttermino",
								   		   "label" 	  => "Término de Execução da Obra",
								   		   "blockAgp" => "",
								   		   "type"	  => "") );
			break;
			
			case 'frpdesc':
				array_push( $coluna, array("campo" 	  => "frpdesc",
								   		   "label" 	  => "Tipo (Origem dos Recursos)",
								   		   "blockAgp" => "",
								   		   "type"	  => "") );
			break;
			
			case 'obridid':
				array_push( $coluna, array("campo" 	  => "obridid",
								   		   "label" 	  => "ID",
								   		   "blockAgp" => "",
								   		   "type"	  => "string") );
			break;
			
			case 'dtpublicacao':
				array_push( $coluna, array("campo" 	  => "dtpublicacao",
								   		   "label" 	  => "Publicação do Edital",
								   		   "blockAgp" => "",
								   		   "type"	  => "") );
			break;
			
			case 'dthomologacao':
				array_push( $coluna, array("campo" 	  => "dthomologacao",
								   		   "label" 	  => "Homologação da Licitação",
								   		   "blockAgp" => "",
								   		   "type"	  => "") );
			break;	
			
			case 'nr_convenio':
				array_push( $coluna, array("campo" 	  => "nr_convenio",
								   		   "label" 	  => "Número do convênio",
								   		   "blockAgp" => "",
								   		   "type"	  => "string") );
				
			break;
			case 'dt_inicio_conv':
				array_push( $coluna, array("campo" 	  => "dt_inicio_conv",
								   		   "label" 	  => "Data de inicio do convênio",
								   		   "blockAgp" => "",
								   		   "type"	  => "") );
				
			break;
			case 'dt_final_conv':
				array_push( $coluna, array("campo" 	  => "dt_final_conv",
								   		   "label" 	  => "Data de fim do convênio",
								   		   "blockAgp" => "",
								   		   "type"	  => "") );
				
			break;
			case 'vlrliquidado':
				if( $_REQUEST['pesquisa'] == '1' ){
					array_push( $coluna, array("campo" 	  => "vlrliquidado",
									   		   "label" 	  => "Execução Orçamentária - Valor Liquidado",
									   		   "blockAgp" => "",
											   "html"     => "<div style='color:#0066CC; width:100%; text-align:right;'>{vlrliquidado}</div>") );
				}else{
					array_push( $coluna, array("campo" 	  => "vlrliquidado",
									   		   "label" 	  => "Execução Orçamentária - Valor Liquidado",
									   		   "blockAgp" => "",
									   		   "type"	  => "") );
				}
				
			break;
			case 'coordenadas_geograficas':
				array_push( $coluna, array("campo" 	  => "coordenadas_geograficas",
								   		   "label" 	  => "Coordenadas Geográficas",
								   		   "blockAgp" => "",
								   		   "type"	  => "") );
				
			break;
			case 'muncod':
				array_push( $coluna, array("campo" 	  => "muncod",
								   		   "label" 	  => "Código do IBGE",
								   		   "blockAgp" => "",
								   		   "type"	  => "string") );
				
			break;
			case 'cep':
				array_push( $coluna, array("campo" 	  => "cep",
								   		   "label" 	  => "CEP",
								   		   "blockAgp" => "",
								   		   "type"	  => "string") );
				
			break;
			case 'endlog':
				array_push( $coluna, array("campo" 	  => "endlog",
								   		   "label" 	  => "Endereço - Logradouro",
								   		   "blockAgp" => "",
								   		   "type"	  => "") );
				
			break;
			case 'endcom':
				array_push( $coluna, array("campo" 	  => "endcom",
								   		   "label" 	  => "Endereço - Complemento",
								   		   "blockAgp" => "",
								   		   "type"	  => "") );
				
			break;
			case 'endnum':
				array_push( $coluna, array("campo" 	  => "endnum",
								   		   "label" 	  => "Endereço - Número",
								   		   "blockAgp" => "",
								   		   "type"	  => "string") );
				
			break;
			case 'endcomunidade':
				array_push( $coluna, array("campo" 	  => "endcomunidade",
								   		   "label" 	  => "Endereço - Comunidade",
								   		   "blockAgp" => "",
								   		   "type"	  => "") );
				
			break;
			case 'obrsupervisoes':
				array_push( $coluna, array("campo" 	  => "obrsupervisoes",
								   		   "label" 	  => "N° de Supervisões",
								   		   "blockAgp" => "",
								   		   "type"	  => "string") );
				
			break;
			
		}
		
	}
	return $coluna;
	
}


// transforma consulta em pública
if ( $_REQUEST['prtid'] && $_REQUEST['publico'] ){
	$sql = sprintf(
		"UPDATE public.parametros_tela SET prtpublico = case when prtpublico = true then false else true end WHERE prtid = %d",
		$_REQUEST['prtid']
	);
	$db->executar( $sql );
	$db->commit();
	?>
	<script type="text/javascript">
		location.href = '?modulo=<?= $modulo ?>&acao=A';
	</script>
	<?
	die;
}
// FIM transforma consulta em pública

// remove consulta
if ( $_REQUEST['prtid'] && $_REQUEST['excluir'] == 1 ) {
	$sql = sprintf(
		"DELETE from public.parametros_tela WHERE prtid = %d",
		$_REQUEST['prtid']
	);
	$db->executar( $sql );
	$db->commit();
	?>
		<script type="text/javascript">
			location.href = '?modulo=<?= $modulo ?>&acao=A';
		</script>
	<?
	die;
}
// FIM remove consulta

// remove flag de submissão de formulário
if ( $_REQUEST['prtid'] && $_REQUEST['carregar'] ){
	unset( $_REQUEST['form'] );
}
// FIM remove flag de submissão de formulário

// exibe consulta
if ( isset( $_REQUEST['form'] ) == true ){
	if ( $_REQUEST['prtid'] ){
		$sql = sprintf(	"select prtobj from public.parametros_tela where prtid = " . $_REQUEST['prtid'] );
		$itens = $db->pegaUm( $sql );
		$dados = unserialize( stripslashes( stripslashes( $itens ) ) );
		$_REQUEST = $dados;//array_merge( $_REQUEST, $dados );
		unset( $_REQUEST['salvar'] );
	}
	
	include "geral_resultado2.inc";
	exit;
	
}

// carrega consulta do banco
if ( $_REQUEST['prtid'] && $_REQUEST['carregar'] == 1 ){
	
	$sql = sprintf(	"select prtobj from public.parametros_tela where prtid = ".$_REQUEST['prtid'] );
	$itens = $db->pegaUm( $sql );
	$dados = unserialize( stripslashes( stripslashes( $itens ) ) );
	extract( $dados );
	$_REQUEST = $dados;
	unset( $_REQUEST['form'] );
	unset( $_REQUEST['pesquisa'] );
	$titulo = $_REQUEST['titulo'];
	
	$agrupador2 = array();
	
	if ( $_REQUEST['agrupador'] ){
		
		foreach ( $_REQUEST['agrupador'] as $valorAgrupador ){
			array_push( $agrupador2, array( 'codigo' => $valorAgrupador, 'descricao' => $valorAgrupador ));
		}
		
	}
	
}


if ( isset( $_REQUEST['pesquisa'] ) || isset( $_REQUEST['tipoRelatorio'] ) ){
	switch($_REQUEST['pesquisa']) {
		case '1':
			include "geral_resultado.inc";
			exit;
		case '2':
			include "geralxls_resultado.inc";
			exit;
	}
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo = "Relatório Geral";
monta_titulo( $titulo_modulo, 'Selecione os filtros e agrupadores desejados' );

?>
<script type="text/javascript">
	function obras_exibeRelatorioGeralXLS(){
		
		var formulario = document.filtro;
		var colunas  = document.getElementById( 'colunas' );
		
		// Tipo de relatorio
		formulario.pesquisa.value='2';
		
		prepara_formulario();
		selectAllOptions( formulario.colunas );
		
		 
		if ( !document.getElementsByName('orgid[]').item(0).checked &&
			 !document.getElementsByName('orgid[]').item(1).checked &&
			 !document.getElementsByName('orgid[]').item(2).checked ){
			alert( 'Favor selecionar ao menos um tipo de ensino!' );
			return false;
		}
		
		if ( !colunas.options.length ){
			alert( 'Favor selecionar ao menos uma coluna!' );
			return false;
		}
		
		selectAllOptions( colunas );
		selectAllOptions( document.getElementById( 'regiao' ) );
		selectAllOptions( document.getElementById( 'mesoregiao' ) );
		selectAllOptions( document.getElementById( 'uf' ) );
		selectAllOptions( document.getElementById( 'municipio' ) );
		selectAllOptions( document.getElementById( 'unidade' ) );
		selectAllOptions( document.getElementById( 'campus' ) );
		selectAllOptions( document.getElementById( 'prfid' ) );
		selectAllOptions( document.getElementById( 'tpoid' ) );
		selectAllOptions( document.getElementById( 'cloid' ) );
		selectAllOptions( document.getElementById( 'stoid' ) );
		selectAllOptions( document.getElementById( 'grupomun' ) );
		selectAllOptions( document.getElementById( 'tipomun' ) );
		
		formulario.submit();
		
	}
	
	function obras_exibeRelatorioGeral(tipo){
		
		var formulario = document.filtro;
		var agrupador  = document.getElementById( 'colunas' );

		// Tipo de relatorio
		formulario.pesquisa.value='1';

		
		prepara_formulario();
		selectAllOptions( formulario.colunas );
		
		if ( tipo == 'relatorio' ){
			
			formulario.action = 'obras.php?modulo=relatorio/relatorio_geral2&acao=A';
			window.open( '', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			formulario.target = 'relatorio';
			
		}else {
		
			if ( tipo == 'planilha' ){
				
				if( document.getElementsByName('orgid[]') ) {
					if ( !document.getElementsByName('orgid[]').item(0).checked &&
						 !document.getElementsByName('orgid[]').item(1).checked &&
						 !document.getElementsByName('orgid[]').item(2).checked ){
						alert( 'Favor selecionar ao menos um tipo de ensino!' );
						return false;
					}
				} else {
					alert( 'Nenhum tipo de ensino associado a seu perfil!' );
					return false;
				}
				
				if ( !colunas.options.length ){
					alert( 'Favor selecionar ao menos uma coluna!' );
					return false;
				}
				
				formulario.action = 'obras.php?modulo=relatorio/relatorio_geral2&acao=A&tipoRelatorio=xls';
				
			}else if ( tipo == 'salvar' ){
				
				if ( formulario.titulo.value == '' ) {
					alert( 'É necessário informar a descrição do relatório!' );
					formulario.titulo.focus();
					return;
				}
				var nomesExistentes = new Array();
				<?php
					$sqlNomesConsulta = "SELECT prtdsc FROM public.parametros_tela";
					$nomesExistentes = $db->carregar( $sqlNomesConsulta );
					if ( $nomesExistentes ){
						foreach ( $nomesExistentes as $linhaNome )
						{
							print "nomesExistentes[nomesExistentes.length] = '" . str_replace( "'", "\'", $linhaNome['prtdsc'] ) . "';";
						}
					}
				?>
				var confirma = true;
				var i, j = nomesExistentes.length;
				for ( i = 0; i < j; i++ ){
					if ( nomesExistentes[i] == formulario.titulo.value ){
						confirma = confirm( 'Deseja alterar a consulta já existente?' );
						break;
					}
				}
				if ( !confirma ){
					return;
				}
				formulario.action = 'obras.php?modulo=relatorio/relatorio_geral2&acao=A&salvar=1';
				formulario.target = '_self';
		
			}else if( tipo == 'exibir' ){
			 
				if ( !document.getElementsByName('orgid[]').item(0).checked &&
					 !document.getElementsByName('orgid[]').item(1).checked &&
					 !document.getElementsByName('orgid[]').item(2).checked ){
					alert( 'Favor selecionar ao menos um tipo de ensino!' );
					return false;
				}
				
				if ( !agrupador.options.length ){
					alert( 'Favor selecionar ao menos uma coluna!' );
					return false;
				}
				
				selectAllOptions( agrupador );
				selectAllOptions( document.getElementById( 'regiao' ) );
				selectAllOptions( document.getElementById( 'mesoregiao' ) );
				selectAllOptions( document.getElementById( 'uf' ) );
				selectAllOptions( document.getElementById( 'municipio' ) );
				selectAllOptions( document.getElementById( 'unidade' ) );
				selectAllOptions( document.getElementById( 'campus' ) );
				selectAllOptions( document.getElementById( 'prfid' ) );
				selectAllOptions( document.getElementById( 'tpoid' ) );
				selectAllOptions( document.getElementById( 'cloid' ) );
				selectAllOptions( document.getElementById( 'stoid' ) );
				selectAllOptions( document.getElementById( 'grupomun' ) );
				selectAllOptions( document.getElementById( 'tipomun' ) );
				
				formulario.target = 'resultadoObrasGeral';
				var janela = window.open( '', 'resultadoObrasGeral', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
				janela.focus();
			}
		}
		
		formulario.submit();
		
	}
	
	
	function tornar_publico( prtid ){
//		location.href = '?modulo=<?//= $modulo ?>&acao=R&prtid='+ prtid + '&publico=1';
		document.filtro.publico.value = '1';
		document.filtro.prtid.value = prtid;
		document.filtro.target = '_self';
		document.filtro.submit();
	}
	
	function excluir_relatorio( prtid ){
		document.filtro.excluir.value = '1';
		document.filtro.prtid.value = prtid;
		document.filtro.target = '_self';
		document.filtro.submit();
	}
	
	function carregar_consulta( prtid ){
		document.filtro.carregar.value = '1';
		document.filtro.prtid.value = prtid;
		document.filtro.target = '_self';
		document.filtro.submit();
	}
	
	function carregar_relatorio( prtid ){
		document.filtro.prtid.value = prtid;
		obras_exibeRelatorioGeral( 'relatorio' );
	}
	
	/* Função para substituir todos */
	function replaceAll(str, de, para){
	    var pos = str.indexOf(de);
	    while (pos > -1){
			str = str.replace(de, para);
			pos = str.indexOf(de);
		}
	    return (str);
	}
	/* Função para adicionar linha nas tabelas */

	/* CRIANDO REQUISIÇÃO (IE OU FIREFOX) */
	function criarrequisicao() {
		return window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject( 'Msxml2.XMLHTTP' );
	}
	/* FIM - CRIANDO REQUISIÇÃO (IE OU FIREFOX) */
	
	/* FUNÇÃO QUE TRATA O RETORNO */
	var pegarretorno = function () {
		try {
				if ( evXmlHttp.readyState == 4 ) {
					if ( evXmlHttp.status == 200 && evXmlHttp.responseText != '' ) {
						// criando options
						var x = evXmlHttp.responseText.split("&&");
						for(i=1;i<(x.length-1);i++) {
							var dados = x[i].split("##");
							document.getElementById('usrs').options[i] = new Option(dados[1],dados[0]);
						}
						var dados = x[0].split("##");
						document.getElementById('usrs').options[0] = new Option(dados[1],dados[0]);
						document.getElementById('usrs').value = cpfselecionado;
					}
					if ( evXmlHttp.dispose ) {
						evXmlHttp.dispose();
					}
					evXmlHttp = null;
				}
			}
		catch(e) {}
	};
	/* FIM - FUNÇÃO QUE TRATA O RETORNO */
			
				
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
		var input = document.getElementById( campo + '_campo_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '0';
		}
	}

//-->
</script>
<form action="" method="post" name="filtro"> 
	<input type="hidden" name="form" value="1"/>
	<input type="hidden" name="pesquisa" value="1"/>
	<input type="hidden" name="publico" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado -->
	<input type="hidden" name="prtid" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
	<input type="hidden" name="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
	<input type="hidden" name="excluir" value=""/> <!-- indica se foi clicado para excluir o relatório já gravado -->
	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="SubTituloDireita">Título</td>
			<td>
				<?= campo_texto( 'titulo', 'N', 'S', '', 65, 60, '', '', 'left', '', 0, 'id="titulo"'); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Tipo de Estabelecimento</td>
			<td>
				<?php

					// Monta as opções de tipo de ensino de acordo com a responsabilidade do usuario
					if( ($db->testa_superuser()) || ( possuiPerfil( PERFIL_CONSULTAGERAL) || 
									   				  possuiPerfil( PERFIL_GESTORMEC ) || possuiPerfil(PERFIL_ADMINISTRADOR) ) ){
						
						$orgaos = $db->carregar("SELECT 
													orgid, orgdesc 
												 FROM 
													obras.orgao");
						
						$count = count($orgaos);
						for($i = 0; $i < $count; $i++){
							
							echo '<input type="checkbox" id="orgid" name="orgid[]" value="' . $orgaos[$i]['orgid'] . '"/> ' . $orgaos[$i]["orgdesc"] . '</label>&nbsp;';
							
						}
									   				  	
					}else{
						
						$orgaos = obras_pegarOrgaoPermitido();
						$count = count($orgaos);
						
						for($i = 0; $i < $count; $i++){
							
							echo '<input type="checkbox" id="orgid" name="orgid[]" value="' . $orgaos[$i]['id'] . '"/> ' . $orgaos[$i]["descricao"] . '</label>&nbsp;';
							
						}
//						if($orgaos[0])
//							echo '<input type="checkbox" id="orgid" name="orgid[]" value="'.$orgaos[0]["id"].'" checked=\"checked\"/>' . $orgaos[0]["descricao"] . '&nbsp;';
						
					}
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Colunas</td>
			<td>
				<?php
					// Início dos agrupadores
					$agrupador = new Agrupador('filtro','');
					
					// Dados padrão de destino (nulo)
					$destino = isset( $agrupador2 ) ? $agrupador2 : array();
					
					// Dados padrão de origem
					$origem = array(
						'tipoaquisicao' => array(
													'codigo'    => 'tipoaquisicao',
													'descricao' => 'Tipo de aquisição'
						),
						'metragem' => array(
													'codigo'    => 'metragem',
													'descricao' => 'Área/Quantidade a ser Construída'
						),
						'situacaoobra' => array(
													'codigo'    => 'situacaoobra',
													'descricao' => 'Situação da obra'
						),
						'unidade' => array(
													'codigo'    => 'unidade',
													'descricao' => 'Unidade'
						),
						'campus' => array(
													'codigo'    => 'campus',
													'descricao' => 'Estabelecimento'
						),
						
						'municipio' => array(
													'codigo'    => 'municipio',
													'descricao' => 'Município'
						),
						'estado' => array(
													'codigo'    => 'estado',
													'descricao' => 'UF'
						),
	//					'datainicio' => array(
	//												'codigo'    => 'datainicio',
	//												'descricao' => 'Início da Execução da Obra'
	//					),
	//					'datafim' => array(
	//												'codigo'    => 'datafim',
	//												'descricao' => 'Término da Execução da Obra'
	//					),
						'ultatualizacao' => array(
													'codigo'    => 'ultatualizacao',
													'descricao' => 'Última atualização'
						),
						'qtdvistorias' => array(
													'codigo'    => 'qtdvistorias',
													'descricao' => 'Qtd de vistorias'
						),
						'porcexecucao' => array(
													'codigo'    => 'porcexecucao',
													'descricao' => '% de execução'
						),
						'qtdrestricoes' => array(
													'codigo'    => 'qtdrestricoes',
													'descricao' => 'Restrições'
						),
						'cnpjempresacontratada' => array(
													'codigo'    => 'cnpjempresacontratada',
													'descricao' => 'CNPJ Empresa contratada'
						),
						'nomeempresacontratada' => array(
													'codigo'    => 'nomeempresacontratada',
													'descricao' => 'Nome Empresa contratada'
						),
						'tipoobra' => array(
													'codigo'    => 'tipoobra',
													'descricao' => 'Tipo da obra'
						),
						'subacao' => array(
													'codigo'    => 'subacao',
													'descricao' => 'Programa/Fonte'
						),
						'obrvalorprevisto' => array(
													'codigo'    => 'obrvalorprevisto',
													'descricao' => 'Valor Previsto (R$):'
						),
						'obrcustocontrato' => array(
													'codigo'    => 'obrcustocontrato',
													'descricao' => 'Valor Contratado da Obra (R$):',
													'type'	  	=> "string"
													
						),
						'moldsc' => array(
													'codigo'    => 'moldsc',
													'descricao' => 'Modalidade de Licitação'
						),
						'licitacaouasg' => array(
													'codigo'    => 'licitacaouasg',
													'descricao' => 'Número da UASG'
						),
						'numlicitacao' => array(
													'codigo'    => 'numlicitacao',
													'descricao' => 'Número da Licitação'
						),
						'obrdtassinaturacontrato' => array(
													'codigo'    => 'obrdtassinaturacontrato',
													'descricao' => 'Data de Assinatura do Contrato'
						),
						'dtterminocontrato' => array(
													'codigo'    => 'dtterminocontrato',
													'descricao' => 'Data de Término do Contrato'
						),
						'obrdtordemservico' => array(
													'codigo'    => 'obrdtordemservico',
													'descricao' => 'Data de Ordem de Serviço'
						),
						'obrdtinicio' => array(
													'codigo'    => 'obrdtinicio',
													'descricao' => 'Início de Execução da Obra'
						),
						'obrdttermino' => array(
													'codigo'    => 'obrdttermino',
													'descricao' => 'Término de Execução da Obra'
						),
						'frpdesc' => array(
													'codigo'    => 'frpdesc',
													'descricao' => 'Tipo (Origem dos Recursos)'
						),
						'obridid' => array(
													'codigo'    => 'obridid',
													'descricao' => 'ID'
						),
						'dtpublicacao' => array(
													'codigo'    => 'dtpublicacao',
													'descricao' => 'Publicação do Edital'
						),
						'dthomologacao' => array(
													'codigo'    => 'dthomologacao',
													'descricao' => 'Homologação do Edital'
						),
						'orgao' => array(
													'codigo'    => 'orgao',
													'descricao' => 'Orgão'
						),
						'nr_convenio' => array(
													'codigo'    => 'nr_convenio',
													'descricao' => 'Número do convênio'
						),
						'dt_inicio_conv' => array(
													'codigo'    => 'dt_inicio_conv',
													'descricao' => 'Data de inicio do convênio'
						),
						'dt_final_conv' => array(
													'codigo'    => 'dt_final_conv',
													'descricao' => 'Data de fim do convênio'
						),
						'vlrliquidado' => array(
													'codigo'    => 'vlrliquidado',
													'descricao' => 'Exec. Orc. - Valor Liquidado'
						),
						'coordenadas_geograficas' => array(
													'codigo'    => 'coordenadas_geograficas',
													'descricao' => 'Coordenadas Geográficas'
						),
						'muncod' => array(
													'codigo'    => 'muncod',
													'descricao' => 'Código do IBGE'
						),
						'cep' => array(
													'codigo'    => 'cep',
													'descricao' => 'CEP'
						),
						'endlog' => array(
													'codigo'    => 'endlog',
													'descricao' => 'Endereço - Logradouro'
						),
						'endcom' => array(
													'codigo'    => 'endcom',
													'descricao' => 'Endereço - Complemento'
						),
						'endbai' => array(
													'codigo'    => 'endbai',
													'descricao' => 'Endereço - Bairro'
						),
						'endnum' => array(
													'codigo'    => 'endnum',
													'descricao' => 'Endereço - Número'
						),
						'endcomunidade' => array(
													'codigo'    => 'endcomunidade',
													'descricao' => 'Endereço - Comunidade'
						),
						'obrsupervisoes' => array(
													'codigo'    => 'obrsupervisoes',
													'descricao' => 'N° de Supervisões'
						)
						
					);
					
					// exibe agrupador
					$agrupador->setOrigem( 'naoColunas', null, $origem );
					$agrupador->setDestino( 'colunas', null, $destino );
					$agrupador->exibir();
				?>
			</td>
		</tr>
		</table>
		
		<!-- OUTROS FILTROS -->
	
		<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
			<tr>
				<td onclick="javascript:onOffBloco( 'outros' );">
					<img border="0" src="/imagens/mais.gif" id="outros_img"/>&nbsp;
					Relatórios Gerenciais
					<input type="hidden" id="outros_flag" name="outros_flag" value="0" />
				</td>
			</tr>
		</table>
		<div id="outros_div_filtros_off">
			<!--
			<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
				<tr>
					<td><span style="color:#a0a0a0;padding:0 30px;">nenhum filtro</span></td>
				</tr>
			</table>
			-->
		</div>
	
		<div id="outros_div_filtros_on" style="display:none;">
			<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
					<tr>
						<td width="195" class="SubTituloDireita" valign="top">Relatórios:</td>
						<?php
						
							if( $db->testa_superuser() || possuiPerfil(PERFIL_SUPERVISORMEC) || 
								possuiPerfil(PERFIL_ADMINISTRADOR) ){
							 	$bt_publicar = "<img border=\"0\" src=\"../imagens/usuario.gif\" title=\" Despublicar \" onclick=\"tornar_publico(' || prtid || ');\">&nbsp;&nbsp;";
							} 
						
							$sql = sprintf(
								"SELECT Case when prtpublico = true and usucpf = '%s' then '{$bt_publicar}<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' else '<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' end as acao, '' || prtdsc || '' as descricao FROM public.parametros_tela WHERE mnuid = %d AND prtpublico = TRUE",
								$_SESSION['usucpf'],
								$_SESSION['mnuid'],
								$_SESSION['usucpf']
							);
							/*
							$sql = sprintf(
								"SELECT 'abc' as acao, prtdsc FROM public.parametros_tela WHERE mnuid = %d AND prtpublico = TRUE",
								$_SESSION['mnuid']
							);
							*/
							$cabecalho = array('Ação', 'Nome');
						?>
						<td><?php $db->monta_lista_simples( $sql, $cabecalho, 50, 50, null, null, null ); ?>
						</td>
					</tr>
			</table>
		</div>
		<script language="javascript">	//alert( document.formulario.agrupador_combo.value );	</script>

		<!-- FIM OUTROS FILTROS -->
		
		<!-- MINHAS CONSULTAS -->
		
		<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
			<tr>
				<td onclick="javascript:onOffBloco( 'minhasconsultas' );">
					<img border="0" src="/imagens/mais.gif" id="minhasconsultas_img"/>&nbsp;
					Minhas Consultas
					<input type="hidden" id="minhasconsultas_flag" name="minhasconsultas_flag" value="0" />
				</td>
			</tr>
		</table>
		<div id="minhasconsultas_div_filtros_off">
		</div>
		<div id="minhasconsultas_div_filtros_on" style="display:none;">
			<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
					<tr>
						<td width="195" class="SubTituloDireita" valign="top">Consultas</td>
						<?php
						
							$sql = sprintf(
								"SELECT 
									CASE WHEN prtpublico = false THEN '<img border=\"0\" src=\"../imagens/grupo.gif\" title=\" Publicar \" onclick=\"tornar_publico(' || prtid || ')\">&nbsp;&nbsp;
																	   <img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
																	   <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' 
																 ELSE '<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
																 	   <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' 
									END as acao, 
									'' || prtdsc || '' as descricao 
								 FROM 
								 	public.parametros_tela 
								 WHERE 
								 	mnuid = %d AND usucpf = '%s'",
								$_SESSION['mnuid'],
								$_SESSION['usucpf']
							);
							
							$cabecalho = array('Ação', 'Nome');
						?>
						<td>
							<?php $db->monta_lista_simples( $sql, $cabecalho, 50, 50, 'N', '80%', null ); ?>
						</td>
					</tr>
			</table>
		</div>
		<!-- FIM MINHAS CONSULTAS -->
		
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
			<?php
				// Região
				$stSql = " SELECT
								regcod AS codigo,
								regdescricao AS descricao
							FROM 
								territorios.regiao
							%s
							ORDER BY
								regdescricao ";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Regiões', 'regiao',  $stSql, $stSqlCarregados, 'Selecione a(s) Regiões(s)' );

				// Mesoregião
				$stSql = " SELECT
								mescod AS codigo,
								mesdsc AS descricao
							FROM 
								territorios.mesoregiao
							ORDER BY
								mesdsc ";
				mostrarComboPopup( 'Mesorregião', 'mesoregiao',  $stSql, $stSqlCarregados, 'Selecione a(s) Mesorregião(ões)' );
				
				// UF
				$stSql = " SELECT
								estuf AS codigo,
								estdescricao AS descricao
							FROM 
								territorios.estado
							ORDER BY
								estdescricao ";
				mostrarComboPopup( 'UF', 'uf',  $stSql, $stSqlCarregados, 'Selecione a(s) UF(s)' );

				// Grupo de Município
				$stSql = "  SELECT
								gtmid as codigo,
								gtmdsc as descricao
							FROM
								territorios.grupotipomunicipio
							ORDER BY
								gtmdsc";
				mostrarComboPopup( 'Grupo de Municipio', 'grupomun',  $stSql, $stSqlCarregados, 'Selecione o(s) Grupo(s) de Município(s)' );
				
				// Tipo de Município
				$stSql = "  SELECT
								tpmid AS codigo,
								tpmdsc AS descricao
							FROM 
								territorios.tipomunicipio
							ORDER BY
								tpmdsc";
				mostrarComboPopup( 'Tipo de Município', 'tipomun',  $stSql, $stSqlCarregados, 'Selecione o(s) tipo(s) de Município(s)' );
				
				// Município
				$stSql = "  SELECT
								tm.muncod AS codigo,
								tm.estuf || ' - ' || tm.mundescricao AS descricao
							FROM 
								territorios.municipio tm
							INNER JOIN
								entidade.endereco ee ON ee.muncod = tm.muncod
							INNER JOIN
								obras.obrainfraestrutura oi ON oi.endid = ee.endid
							WHERE
							 	oi.obsstatus = 'A'
							ORDER BY
								mundescricao ";
				mostrarComboPopup( 'Município', 'municipio',  $stSql, $stSqlCarregados, 'Selecione o(s) Município(s)' );

				// Unidades
				$stSql = "  SELECT
								ee.entid AS codigo,
								ee.entnome AS descricao
						 	FROM 
								entidade.entidade ee
						 	INNER JOIN
						 		obras.obrainfraestrutura oi ON ee.entid = oi.entidunidade
						 	WHERE
						 		oi.obsstatus = 'A'
						 	ORDER BY
						 	entnome";
				mostrarComboPopup( 'Unidades', 'unidade',  $stSql, $stSqlCarregados, 'Selecione o(s) Unidades(s)' );

				// Campus
				$stSql = " SELECT 
						   		ee.entid AS codigo, 
						   		ee.entnome AS descricao 
						   FROM 
						   		entidade.entidade ee
						   INNER JOIN
						   		obras.obrainfraestrutura oi ON ee.entid = oi.entidcampus
						   INNER JOIN
						   		entidade.funcaoentidade ef ON ef.entid = ee.entid 
						   WHERE 
						   		ef.funid in(17,18) AND
						   		oi.obsstatus = 'A'";
				mostrarComboPopup( 'Estabelecimento', 'campus',  $stSql, $stSqlCarregados, 'Selecione a(s) Unidade(s)' );

				// Programa Fonte
				$stSql = "SELECT
							prfid AS codigo,
							prfdesc AS descricao
						 FROM 
							obras.programafonte
						 ORDER BY
							prfdesc ";
				mostrarComboPopup( 'Programa Fonte', 'prfid',  $stSql, $stSqlCarregados, 'Selecione o(s) Programa(s) Fonte(s)' );

				// Tipologia da Obra
				$stSql = "SELECT
							tpoid AS codigo,
							tpodsc AS descricao
						  FROM 
							obras.tipologiaobra
						  ORDER BY
							tpodsc ";
				mostrarComboPopup( 'Tipologia da Obra', 'tpoid',  $stSql, $stSqlCarregados, 'Selecione a(s) Tipologia(a) da(s) Obra(s)' );

				// Classificação da Obra
				$stSql = "SELECT
							cloid AS codigo,
							clodsc AS descricao
						  FROM 
							obras.classificacaoobra
						  ORDER BY
							clodsc ";
				mostrarComboPopup( 'Classificação da Obra', 'cloid',  $stSql, $stSqlCarregados, 'Selecione a(s) Classificação(ões) da(s) Obra(s)' );

				// Situação da Obra
				$stSql = " SELECT
								stoid AS codigo,
								stodesc AS descricao
						   FROM 
								obras.situacaoobra
						   ORDER BY
								stodesc ";
				mostrarComboPopup( 'Situação da Obra', 'stoid',  $stSql, $stSqlCarregados, 'Selecione a(s) Situação(ões) da(s) Obra(s)' );
			?>
		<tr>
			<td class="SubTituloDireita">Percentual da Obra</td>
			<td>
				<table>
					<tr>
						<th>Mínimo</th>
						<th>Máximo</th>
					</tr>
					<tr>
				<?php
					$arPercentual[]  = array( 'codigo' =>  0 , 'descricao' => '0 %' );
					$arPercentual[]  = array( 'codigo' =>  5 , 'descricao' => '5 %' );
					$arPercentual[]  = array( 'codigo' => 10 , 'descricao' => '10 %' );
					$arPercentual[]  = array( 'codigo' => 15 , 'descricao' => '15 %' );
					$arPercentual[]  = array( 'codigo' => 20 , 'descricao' => '20 %' );
					$arPercentual[]  = array( 'codigo' => 25 , 'descricao' => '25 %' );
					$arPercentual[]  = array( 'codigo' => 30 , 'descricao' => '30 %' );
					$arPercentual[]  = array( 'codigo' => 35 , 'descricao' => '35 %' );
					$arPercentual[]  = array( 'codigo' => 40 , 'descricao' => '40 %' );
					$arPercentual[]  = array( 'codigo' => 45 , 'descricao' => '45 %' );
					$arPercentual[]  = array( 'codigo' => 50 , 'descricao' => '50 %' );
					$arPercentual[]  = array( 'codigo' => 55 , 'descricao' => '55 %' );
					$arPercentual[]  = array( 'codigo' => 60 , 'descricao' => '60 %' );
					$arPercentual[]  = array( 'codigo' => 65 , 'descricao' => '65 %' );
					$arPercentual[]  = array( 'codigo' => 70 , 'descricao' => '70 %' );
					$arPercentual[]  = array( 'codigo' => 75 , 'descricao' => '75 %' );
					$arPercentual[]  = array( 'codigo' => 80 , 'descricao' => '80 %' );
					$arPercentual[]  = array( 'codigo' => 85 , 'descricao' => '85 %' );
					$arPercentual[]  = array( 'codigo' => 90 , 'descricao' => '90 %' );
					$arPercentual[]  = array( 'codigo' => 95 , 'descricao' => '95 %' );
					$arPercentual[]  = array( 'codigo' => 100 , 'descricao' => '100 %' );
					
					$percentualinicial = 0;
					$percentualfinal   = 100;
					echo '<td>';
					$db->monta_combo("percentualinicial", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualinicial');
					echo '</td><td>';
					$db->monta_combo("percentualfinal", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualfinal');
					echo '</td>';
				?>
					</tr>
				</table>
			</td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita">Possui foto</td>
			<td>
				<input type='radio' id='foto' name='foto' value='sim' /> Sim
				<input type='radio' id='foto' name='foto' value='nao'/> Não
				<input type='radio' id='foto' name='foto' value='todos' checked="checked"/> Todas
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Possui vistoria</td>
			<td>
				<input type='radio' id='vistoria' name='vistoria' value='sim'   onclick="mostraResponsavel(this.value);" /> Sim
				<input type='radio' id='vistoria' name='vistoria' value='nao'   onclick="mostraResponsavel(this.value);" /> Não
				<input type='radio' id='vistoria' name='vistoria' value='todos' onclick="mostraResponsavel(this.value);"  checked="checked" /> Todas
			</td>
		</tr>
		<tr id="trResponsavel">
			<td class="SubTituloDireita">Responsável pela última vistoria</td>
			<td>
				<?php 
					
				$sql = "SELECT 
							rsuid as codigo, 
							rsudsc as descricao
						FROM 
							obras.realizacaosupervisao";
				
				$db->monta_combo( 'responsavel', $sql, 'S', 'Selecione', '', '','','200','S','responsavel' );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Possui restrição</td>
			<td>
				<input type='radio' id='restricao' name='restricao' value='sim' /> Sim
				<input type='radio' id='restricao' name='restricao' value='nao'/> Não
				<input type='radio' id='restricao' name='restricao' value='todos' checked="checked"/> Todas
			</td>
		</tr>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" value="Visualizar" onclick="obras_exibeRelatorioGeral('exibir');" style="cursor: pointer;"/>
				<input type="button" value="Visualizar XLS" onclick="obras_exibeRelatorioGeralXLS();" style="cursor: pointer;"/>
				<input type="button" value="Salvar Consulta" onclick="obras_exibeRelatorioGeral('salvar');" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>
<script>

function mostraResponsavel( valor ){

	var tr   = document.getElementById('trResponsavel');
	var resp = document.getElementById('responsavel');
	
	if( valor == 'nao' ){
		resp.value = '';
		tr.style.display = 'none';
	}else{
		tr.style.display = 'table-row';
	}
	
}

</script>
