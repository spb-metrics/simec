<?php

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
	switch($_REQUEST['pesquisa']) {
		case '1':
			include "geral_resultado.inc";
			exit;
		case '2':
			include "geralxls_resultado.inc";
			exit;
	}
	
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
$titulo_modulo = "Relatório Quantitativo";
monta_titulo( $titulo_modulo, 'Selecione os filtros e agrupadores desejados' );

?>
<script type="text/javascript">
	function obras_exibeRelatorioGeralXLS(){
		
		var formulario = document.filtro;
		var agrupador  = document.getElementById( 'agrupador' );
		
		// Tipo de relatorio
		formulario.pesquisa.value='2';
		
		prepara_formulario();
		selectAllOptions( formulario.agrupador );
		
		 
		if ( !document.getElementsByName('orgid[]').item(0).checked &&
			 !document.getElementsByName('orgid[]').item(1).checked &&
			 !document.getElementsByName('orgid[]').item(2).checked ){
			alert( 'Favor selecionar ao menos um tipo de ensino!' );
			return false;
		}
		
		if ( !agrupador.options.length ){
			alert( 'Favor selecionar ao menos um item para agrupar o resultado!' );
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
		
		formulario.submit();
		
	}
	
	function obras_exibeRelatorioGeral(tipo){
		
		var formulario = document.filtro;
		var agrupador  = document.getElementById( 'agrupador' );

		// Tipo de relatorio
		formulario.pesquisa.value='1';

		
		prepara_formulario();
		selectAllOptions( formulario.agrupador );
		
		if ( tipo == 'relatorio' ){
			
			formulario.action = 'obras.php?modulo=relatorio/relatorio_geral&acao=A';
			window.open( '', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			formulario.target = 'relatorio';
			
		}else {
		
			if ( tipo == 'planilha' ){
				
				if ( !document.getElementsByName('orgid[]').item(0).checked &&
					 !document.getElementsByName('orgid[]').item(1).checked &&
					 !document.getElementsByName('orgid[]').item(2).checked ){
					alert( 'Favor selecionar ao menos um tipo de ensino!' );
					return false;
				}
				
				if ( !agrupador.options.length ){
					alert( 'Favor selecionar ao menos um item para agrupar o resultado!' );
					return false;
				}
				
				formulario.action = 'obras.php?modulo=relatorio/relatorio_geral&acao=A&tipoRelatorio=xls';
				
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
				formulario.action = 'obras.php?modulo=relatorio/relatorio_geral&acao=A&salvar=1';
				formulario.target = '_self';
		
			}else if( tipo == 'exibir' ){
			 
				if ( !document.getElementsByName('orgid[]').item(0).checked &&
					 !document.getElementsByName('orgid[]').item(1).checked &&
					 !document.getElementsByName('orgid[]').item(2).checked ){
					alert( 'Favor selecionar ao menos um tipo de ensino!' );
					return false;
				}
				
				if ( !agrupador.options.length ){
					alert( 'Favor selecionar ao menos um item para agrupar o resultado!' );
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
						echo '<input type="checkbox" id="orgid" name="orgid[]" value="'.$orgaos[0]["id"].'" checked=\"checked\"/>' . $orgaos[0]["descricao"] . '&nbsp;';
						
					}
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Agrupadores</td>
			<td>
				<?php

					// Início dos agrupadores
					$agrupador = new Agrupador('filtro','');
					
					// Dados padrão de destino (nulo)
					$destino = isset( $agrupador2 ) ? $agrupador2 : array();
					
					// Dados padrão de origem
					$origem = array(
						'pais' => array(
							'codigo'    => 'pais',
							'descricao' => 'Brasil'
						),
						'regiao' => array(
							'codigo'    => 'regiao',
							'descricao' => 'Região'
						),
						'mesoregiao' => array(
							'codigo'    => 'mesoregiao',
							'descricao' => 'Mesoregião'
						),
						'unidade' => array(
							'codigo'    => 'unidade',
							'descricao' => 'Unidades'
						),
						'campus' => array(
							'codigo'    => 'campus',
							'descricao' => 'Campus'
						),	
						'uf' => array(
							'codigo'    => 'uf',
							'descricao' => 'UF'
						),
						'municipio' => array(
							'codigo'    => 'municipio',
							'descricao' => 'Município'
						),
						'programa' => array(
							'codigo'    => 'programa',
							'descricao' => 'Programa Fonte'
						),
						'situacao' => array(
							'codigo'    => 'situacao',
							'descricao' => 'Situação da Obra'
						),
						'tipologia' => array(
							'codigo'    => 'tipologia',
							'descricao' => 'Tipologia da Obra'
						),
						'classificacao' => array(
							'codigo'    => 'classificacao',
							'descricao' => 'Classificação da Obra'
						),
						'nomedaobra' => array(
							'codigo'    => 'nomedaobra',
							'descricao' => 'Nome da Obra'
						),
						'nivelpreenchimento' => array(
							'codigo'    => 'nivelpreenchimento',
							'descricao' => 'Nível de Preenchimento'
						),
						'empresa' => array(
							'codigo'    => 'empresa',
							'descricao' => 'Empresa Contratada'
						),
						'metragem' => array(
							'codigo'    => 'metragem',
							'descricao' => 'Metragem da Obra'
						),
						'tipomun' => array(
							'codigo'    => 'tipomun',
							'descricao' => 'Território da Cidadania'
						)
												
					);
					
					// exibe agrupador
					$agrupador->setOrigem( 'naoAgrupador', null, $origem );
					$agrupador->setDestino( 'agrupador', null, $destino );
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
				<input type='radio' id='foto' name='foto' value='nao' /> Não
				<input type='radio' id='foto' name='foto' value='todos' checked="checked" /> Todas
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Possui vistoria</td>
			<td>
				<input type='radio' id='vistoria' name='vistoria' value='sim'   onclick="mostraResponsavel(this.value);" /> Sim
				<input type='radio' id='vistoria' name='vistoria' value='nao'   onclick="mostraResponsavel(this.value);" /> Não
				<input type='radio' id='vistoria' name='vistoria' value='todos' onclick="mostraResponsavel(this.value);" checked="checked" /> Todas
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
				<input type='radio' id='restricao' name='restricao' value='nao' /> Não
				<input type='radio' id='restricao' name='restricao' value='todos' checked="checked"/> Todas
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Com retorno de (concluída) para (em execução)</td>
			<td>
				<input type='radio' name='concluidaexec' value='sim' /> Sim
				<input type='radio' name='concluidaexec' value='nao' /> Não
				<input type='radio' name='concluidaexec' value='todos' checked="checked" /> Todas
			</td>
		</tr>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" value="Visualizar" 	 onclick="obras_exibeRelatorioGeral('exibir');" style="cursor: pointer;"/>
				<input type="button" value="Visualizar XLS"  onclick="obras_exibeRelatorioGeralXLS();" 		style="cursor: pointer;"/>
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
