<?php
/************************* FUNÇÕES AJAX **********************************/
/*
//CARREGA COMBOS DE ACORDO COM O ANO
if($_REQUEST['requisicao'] == 'enviaano'){
	$ano = $_REQUEST['ano'];
	// Atividades
	$stSql = "SELECT 
		mtaid AS codigo,
		mtadescricao || ' - ' || mtaanoreferencia AS descricao
	FROM pdeescola.metipoatividade 
	WHERE mtasituacao = 'A'
		--AND mtaatividadepst = true
		AND mtaanoreferencia = ".$ano." 
	ORDER BY descricao ASC";					
	
	mostrarComboPopup( 'Atividades', 'atividade',  $stSql, '', 'Selecione a(s) Atividades(s)' );	
	
	// Macro Campo					
	$stSql = "SELECT 
		mtmid AS codigo, 
		mtmdescricao AS descricao 
	FROM pdeescola.metipomacrocampo
	WHERE mtmsituacao = 'A'
		--AND mtmanoreferencia = ".$ano." 
	ORDER BY mtmdescricao ASC";
	
	mostrarComboPopup( 'Macro Campo', 'macrocampo',  $stSql, '', 'Selecione o(s) Macro Campo(s)' );	
	die();
}
*/
/************************* FIM FUNÇÕES AJAX **********************************/

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
			//location.href = '?modulo=<?= $modulo ?>&acao=A';
			location.href = 'pdeescola.php?modulo=relatorio/relMaisEducacao&acao=A';
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
$titulo_modulo = "Relatório Geral Força de Trabalho";
monta_titulo( $titulo_modulo, 'Selecione os filtros e agrupadores desejados' );

?>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<!-- CARREGANDO DADOS AJAX -->
<div id="loader-container" style="display:none;">
    <div id="loader">
    	<img src="../imagens/wait.gif" border="0" align="middle">
   		<span>Aguarde! Carregando Dados...</span>
	</div>
</div>

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
			<td class="SubTituloDireita">Agrupadores</td>
			<td>
				<?php

					// Início dos agrupadores
					$agrupador = new Agrupador('filtro','');
					
					// Dados padrão de destino (nulo)
					$destino = isset( $agrupador2 ) ? $agrupador2 : array();
					
					// Dados padrão de origem
					$origem = array(
						'fstid' => array(
							'codigo'    => 'fstid',
							'descricao' => 'Situação no MEC'
						),
						'estuf' => array(
							'codigo'    => 'estuf',
							'descricao' => 'UF'
						),						
						'fcmid' => array(
							'codigo'    => 'df.fcmid',
							'descricao' => 'Cargo Efetivo no MEC'
						),
						'fulid' => array(
							'codigo'    => 'fulid',
							'descricao' => 'Unidade de Lotação'
						),
						'tfoid' => array(
							'codigo'    => 'tfoid',
							'descricao' => 'Grau de Escolaridade'
						),
						'ftiid' => array(
							'codigo'    => 'ftiid',
							'descricao' => 'Idioma'
						),
						'ftaid' => array(
							'codigo'    => 'ftaid',
							'descricao' => 'Atividade Desenvolvida'
						),
						'fteid' => array(
							'codigo'    => 'fteid',
							'descricao' => 'Tipo de Experiência Anterior'
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
			
		<!-- MINHAS CONSULTAS -->		
		<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
			<tr>
				<td onclick="javascript:onOffBloco( 'minhasconsultas' );" >
					<!-- -->  
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
		
		<!-- MEUS FILTROS -->								
		<table class="tabela" style=" border-bottom:none;" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" >
						
				<?php
				
					$stSqlCarregados = "";
					
					/********************************************************************
					 * Situação	no MEC */				
					$stSql = "  SELECT 
										fstid as codigo,
										fstdescricao as descricao 
								FROM gestaopessoa.ftsituacaotrabalhador 
								WHERE fststatus = 'A'
								AND fstdescricao <> 'Consultor'
								ORDER BY fstdescricao ASC ";
										
					mostrarComboPopup( 'Situação no MEC', 'fstid',  $stSql, $stSqlCarregados, 'Selecione a(s) Situação(ões)' );
						
					/********************************************************************
					 * Estado Civil */
					$stSql = " 	SELECT 
									eciid as codigo, 
									ecidescricao as descricao 
								FROM public.estadocivil
								WHERE ecistatus = 'A'
								ORDER BY ecidescricao ASC ";
										
					mostrarComboPopup( 'Estado Civil', 'eciid',  $stSql, $stSqlCarregados, 'Selecione o(s) Estado(s) Civil(is)' );
					
					/********************************************************************
					 * Sexo */					
					$html = "<tr>
								<td width=\"195\" class=\"SubTituloDireita\" valign=\"top\">Sexo</td>
								<td>
									<ul style=\"margin: 0pt; padding: 0pt;\">					
									  <li style=\"margin: 0pt; width: 70px; list-style-type: none; float: left;\">
										<input type=\"checkbox\" id=\"fdpsexo\" name=\"fdpsexo[]\" value=\"M\">Masculino</ul>
									  </li>
									  <li style=\"margin: 0pt; width: 70px; list-style-type: none; float: left;\">
										<input type=\"checkbox\" id=\"fdpsexo\" name=\"fdpsexo[]\" value=\"F\">Feminino</ul>
									  </li>					
									</ul>
								</td>
							  </tr>";
					echo $html;
					
					
					/********************************************************************
					 * DATA DE NASCIMENTO */					
					$html = "<tr>
								<td width=\"195\" class=\"SubTituloDireita\" valign=\"top\">Data de Nascimento</td>
								<td>
								".campo_data2('dtnascinicio','N','S','Data de Nascimento','','','','')." 
								 &nbsp;&nbsp;a&nbsp;&nbsp; 
								".campo_data2('dtnascfim','N','S','Data de Nascimento','','','','')." 									
								</td>
							  </tr>";
					echo $html;
					
					/********************************************************************
					 * UF */
					$stSql = " SELECT
									estuf AS codigo,
									estdescricao AS descricao
								FROM 
									territorios.estado
								ORDER BY
									estdescricao ";
					
					mostrarComboPopup( 'UF', 'estuf',  $stSql, $stSqlCarregados, 'Selecione a(s) UF(s)' );
					
					/********************************************************************
					 * Grupo Sanguineo */
					$stSql = "	SELECT DISTINCT 
									fdpgruposanguineo AS codigo,
									fdpgruposanguineo AS descricao
								FROM gestaopessoa.ftdadopessoal
								WHERE fdpgruposanguineo <> ''
								ORDER BY fdpgruposanguineo ASC";
					
					$arGrupoSangue = $db->carregar($stSql);
					
					$html = "<tr>
								<td width=\"195\" class=\"SubTituloDireita\" valign=\"top\">Grupo Sanguíneo</td>
								<td>
								<ul style=\"margin: 0pt; padding: 0pt;\">";
					
					foreach($arGrupoSangue as $arDados){
						$html .= "	  <li style=\"margin: 0pt; width: 70px; list-style-type: none; float: left;\">
										<input type=\"checkbox\" id=\"fdpgruposanguineo\" name=\"fdpgruposanguineo[]\" value=\"{$arDados['descricao']}\">".$arDados['descricao']."</ul>
									  </li>
								 ";									
					}
					
					$html .= "	</ul>
								</td>
							  </tr>";
					echo $html;
					
					/********************************************************************
					 * Fator RH */
					$stSql = "	SELECT DISTINCT 
									fdpfatorrh AS codigo,
									fdpfatorrh AS descricao
								FROM gestaopessoa.ftdadopessoal
								WHERE fdpfatorrh <> ''
								ORDER BY fdpfatorrh ASC";
					
					$arGrupoSangue = $db->carregar($stSql);
					
					$html = "<tr>
								<td width=\"195\" class=\"SubTituloDireita\" valign=\"top\">Fator RH</td>
								<td>
								<ul style=\"margin: 0pt; padding: 0pt;\">";
					
					foreach($arGrupoSangue as $arDados){
						$html .= "<li style=\"margin: 0pt; width: 70px; list-style-type: none; float: left;\">
									<input type=\"checkbox\" id=\"fdpfatorrh\" name=\"fdpfatorrh[]\" value=\"{$arDados['descricao']}\">".$arDados['descricao']."</ul>
								  </li>
								 ";									
					}
					
					$html .= "	</ul>
								</td>
							  </tr>";
					echo $html;

					/********************************************************************
					 * Pessoa com Deficiência */					
					$html = "<tr>
								<td width=\"195\" class=\"SubTituloDireita\" valign=\"top\">Pessoa com deficiencia</td>
								<td>
									<ul style=\"margin: 0pt; padding: 0pt;\">					
									  <li style=\"margin: 0pt; width: 70px; list-style-type: none; float: left;\">
										<input type=\"checkbox\" id=\"fdpdeficiente\" name=\"fdpdeficiente[]\" value=\"true\" onclick=\"mostraTipoDeficiencia(this)\">Sim</ul>
									  </li>
									  <li style=\"margin: 0pt; width: 70px; list-style-type: none; float: left;\">
										<input type=\"checkbox\" id=\"fdpdeficiente\" name=\"fdpdeficiente[]\" value=\"false\">Não</ul>
									  </li>					
									</ul>
								</td>
							  </tr>";
					echo $html;				
					
					/********************************************************************
					 * Verificar na pesquisa pois existem nomes iguais com diferença entre letras maiusculas e minusculas
					 * Tipo de Deficiência */
					$stSql = "  SELECT DISTINCT 
									Upper(fdpdeficiencia) AS codigo,
									Upper(fdpdeficiencia) AS descricao
								FROM gestaopessoa.ftdadopessoal
								WHERE fdpdeficiencia <> ''
								AND fdpdeficiencia NOT IN ('-', 'NENHUMA', 'nenhuma', 'Nenhuma')
								ORDER BY descricao ASC ";					
					
					mostrarComboPopup( 'Tipo da Deficiência', 'fdpdeficiencia',  $stSql, $stSqlCarregados, 'Selecione o(s) Tipo(s) de Deficiência(s)' );
					
					/********************************************************************10.220.5.234
					 * Cargo efetivo no MEC */
					$stSql = "  SELECT 
									fcmid as codigo,
									fcmdescricao as descricao 
								FROM gestaopessoa.ftcargoefetivomec
								WHERE fcmstatus = 'A'
								ORDER BY fcmdescricao ASC ";					
					
					mostrarComboPopup( 'Cargo Efetivo no MEC', 'fcmid',  $stSql, $stSqlCarregados, 'Selecione o(s) Cargo(s) Efetivo(s) no MEC' );
					
					/********************************************************************
					 * Exerce Cargo ou Função */					
					$html = "<tr>
								<td width=\"195\" class=\"SubTituloDireita\" valign=\"top\">Exerce Cargo ou Função</td>
								<td>
									<ul style=\"margin: 0pt; padding: 0pt;\">					
									  <li style=\"margin: 0pt; width: 70px; list-style-type: none; float: left;\">
										<input type=\"checkbox\" id=\"fdpexercecargofuncao\" name=\"fdpexercecargofuncao[]\" value=\"true\">Sim</ul>
									  </li>
									  <li style=\"margin: 0pt; width: 70px; list-style-type: none; float: left;\">
										<input type=\"checkbox\" id=\"fdpexercecargofuncao\" name=\"fdpexercecargofuncao[]\" value=\"false\">Não</ul>
									  </li>					
									</ul>
								</td>
							  </tr>";
					echo $html;

					/********************************************************************
					 * Unidade de Lotação */
					$stSql = "  SELECT 
									fulid as codigo,
									fuldescricao as descricao
								FROM gestaopessoa.ftunidadelotacao
								WHERE fulstatus = 'A'
								ORDER BY fuldescricao ASC ";					
					
					mostrarComboPopup( 'Unidade de Lotação', 'fulid',  $stSql, $stSqlCarregados, 'Selecione a(s) Unidade(s) de Lotação' );
					
					/********************************************************************
					 * Grau de Escolaridade */
					$stSql = "  SELECT
									tfoid as codigo,
									tfodsc as descricao
								FROM public.tipoformacao
								WHERE tfostatus = 'A'
								ORDER BY tfodsc ASC ";					
					
					mostrarComboPopup( 'Grau de Escolaridade', 'tfoid',  $stSql, $stSqlCarregados, 'Selecione o(s) Grau(s) de Escolaridade' );
					?>					
					<tr>
						<td class="SubTituloDireita">Situação do Curso</td>
						<td>
						<?php
							/********************************************************************
							 * Situação do Curso c = concluido, e = em andamento, s = suspenso */
							$stSql = "  SELECT DISTINCT
											ffasituacao AS codigo,
											CASE WHEN ffasituacao = 'c' THEN 'Concluído'
												 WHEN ffasituacao = 'e' THEN 'Em andamento'
												 WHEN ffasituacao = 's' THEN 'Suspenso'
											END AS descricao
										FROM gestaopessoa.ftformacaoacademica
										ORDER BY descricao ASC ";												
							
							$db->monta_combo( 'ffasituacao', $stSql, 'S', 'Selecione...', '','','','244','','','','' );
						?>												
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Ano de Conclusão do Curso</td>
						<td>
						<?php
							/********************************************************************
							 * ANO DE CONCLUSÃO */
							$stSql = "  SELECT DISTINCT
											ffaanoconclusao AS codigo, 
											ffaanoconclusao AS descricao 
										FROM gestaopessoa.ftformacaoacademica
										WHERE ffaanoconclusao > '1900'
											AND ffaanoconclusao  < '2100'
										ORDER BY ffaanoconclusao DESC ";												
							
							$db->monta_combo( 'ffaanoconclusao', $stSql, 'S', 'Selecione...', '','','','244','','','','' );
						?>												
						</td>
					</tr>
					<?
					/********************************************************************
					 * Tipo de Idioma */
					$stSql = "  SELECT 
									ftiid as codigo,
									ftidescricao as descricao
								FROM gestaopessoa.ftitipoidioma
								WHERE ftistatus = 'A'
								AND ftidescricao NOT iLIKE 'Novo Idioma'
								ORDER BY ftidescricao ASC ";					
					
					mostrarComboPopup( 'Idioma', 'ftiid',  $stSql, $stSqlCarregados, 'Selecione o(s) Idioma(s)' );
					
					/********************************************************************
					 * Conceito Idioma */					
					$html = "<tr>
								<td width=\"195\" class=\"SubTituloDireita\" valign=\"middle\">Conceito Idioma</td>
								<td>
								<table width=\"500px\">									
									<tr>
										<td width=\"112px\">Leitura </td>
										<td>
											<ul style=\"margin: 0pt; padding: 0pt;\">					
											  <li style=\"margin: 0pt; width: 120px; list-style-type: none; float: left;\">
												<input type=\"checkbox\" id=\"ftcidleitura1\" name=\"ftcidleitura[]\" value=\"1\" onclick=\"verificaIdiomas(this)\">Básico</ul>
											  </li>
											  <li style=\"margin: 0pt; width: 120px; list-style-type: none; float: left;\">
												<input type=\"checkbox\" id=\"ftcidleitura2\" name=\"ftcidleitura[]\" value=\"2\" onclick=\"verificaIdiomas(this)\">Intermediário</ul>
											  </li>
											  <li style=\"margin: 0pt; width: 120px; list-style-type: none; float: left;\">
												<input type=\"checkbox\" id=\"ftcidleitura3\" name=\"ftcidleitura[]\" value=\"3\" onclick=\"verificaIdiomas(this)\">Avançado</ul>
											  </li>					
											</ul>
										</td>
									</tr>
									<tr>
										<td width=\"112px\">Fala </td>
										<td>
											<ul style=\"margin: 0pt; padding: 0pt;\">					
											  <li style=\"margin: 0pt; width: 120px; list-style-type: none; float: left;\">
												<input type=\"checkbox\" id=\"ftcidfala1\" name=\"ftcidfala[]\" value=\"1\" onclick=\"verificaIdiomas(this)\">Básico</ul>
											  </li>
											  <li style=\"margin: 0pt; width: 120px; list-style-type: none; float: left;\">
												<input type=\"checkbox\" id=\"ftcidfala2\" name=\"ftcidfala[]\" value=\"2\" onclick=\"verificaIdiomas(this)\">Intermediário</ul>
											  </li>
											  <li style=\"margin: 0pt; width: 120px; list-style-type: none; float: left;\">
												<input type=\"checkbox\" id=\"ftcidfala3\" name=\"ftcidfala[]\" value=\"3\" onclick=\"verificaIdiomas(this)\">Avançado</ul>
											  </li>					
											</ul>
										</td>
									</tr>
									<tr>
										<td width=\"112px\">Escrita </td>
										<td>
											<ul style=\"margin: 0pt; padding: 0pt;\">					
											  <li style=\"margin: 0pt; width: 120px; list-style-type: none; float: left;\">
												<input type=\"checkbox\" id=\"ftcidescrita1\" name=\"ftcidescrita[]\" value=\"1\" onclick=\"verificaIdiomas(this)\">Básico</ul>
											  </li>
											  <li style=\"margin: 0pt; width: 120px; list-style-type: none; float: left;\">
												<input type=\"checkbox\" id=\"ftcidescrita2\" name=\"ftcidescrita[]\" value=\"2\" onclick=\"verificaIdiomas(this)\">Intermediário</ul>
											  </li>
											  <li style=\"margin: 0pt; width: 120px; list-style-type: none; float: left;\">
												<input type=\"checkbox\" id=\"ftcidescrita3\" name=\"ftcidescrita[]\" value=\"3\" onclick=\"verificaIdiomas(this)\">Avançado</ul>
											  </li>					
											</ul>
										</td>
									</tr>
								</table>
								</td>
							  </tr>";
					echo $html;	
					
					/********************************************************************
					 * Atividade Desenvolvida */
					$stSql = "  SELECT 
									ftaid as codigo,
									ftadescricao as descricao
								FROM gestaopessoa.fttipoatividadedesenvolvida
								WHERE ftastatus = 'A'
								ORDER BY ftadescricao ASC ";					
					
					mostrarComboPopup( 'Atividade Desenvolvida', 'ftaid',  $stSql, $stSqlCarregados, 'Selecione a(s) Atividade(s) Desenvolvida(s)' );
					
					/********************************************************************
					 * Níve de atividade */
					
					$sql = "select * from gestaopessoa.fttiponivelatividadedesenvolvid";
					$pegaNivelAtividade = $db->carregar($sql);
										
					$html = "<tr>
								<td width=\"195\" class=\"SubTituloDireita\" valign=\"middle\">Nível Atividade</td>
								<td>
									<ul style=\"margin: 0pt; padding: 0pt;\">
							";					
					
						foreach ($pegaNivelAtividade as $dados){
							
							$html .= 	"
										<li style=\"margin: 0pt; width: 120px; list-style-type: none; float: left;\">
											<input type=\"checkbox\" id=\"fnaid{$dados['fnaid']}\" name=\"fnaid[]\" value=\"{$dados['fnaid']}\" onclick=\"verificaAtividades(this)\">{$dados['fnadescricao']}</ul>
										</li>
										";
						}									  
									  
					$html .=  "  					
									</ul>
								</td>
							  </tr>
							  ";
					
					echo $html;	
					
					/********************************************************************
					 * Tipo de Experiência Anterior */
					$stSql = "  SELECT 
									fteid as codigo,
									ftedescricao as descricao
								FROM gestaopessoa.fttipoexperienciaanterior
								WHERE ftestatus = 'A'
								ORDER BY ftedescricao ASC ";					
					
					mostrarComboPopup( 'Tipo de Experiência Anterior', 'fteid',  $stSql, $stSqlCarregados, 'Selecione a(s) Experiência(s) Anterior(es)' );
					
					
																							
				?>				
				
		</table>
		
		<table class="tabela" style="border-top:none; border-bottom:none;" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" id="tabelaAgrupa">			
		</table>
			
		<table class="tabela" style="border-top:none;" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" >			
			<tr>
				<td bgcolor="#CCCCCC" width="195"></td>
				<td bgcolor="#CCCCCC">
					<input type="button" value="Visualizar" onclick="pdeescola_exibeRelatorioGeral('exibir');" style="cursor: pointer;"/>					 
					<input type="button" value="Visualizar XLS" onclick="pdeescola_exibeRelatorioGeralXLS();" style="cursor: pointer;"/>					
					<input type="button" value="Salvar Consulta" onclick="pdeescola_exibeRelatorioGeral('salvar');" style="cursor: pointer;"/>
										
				</td>
			</tr>
		</table>
</form>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript">

	var tipoDeficiencia = document.getElementById('tr_fdpdeficiencia');	
	tipoDeficiencia.style.display = "none";	
	
	var inputIdioma = document.getElementById('ftiid');
	//if(inputIdioma.click()){
	//alert(inputIdioma+" - "+inputIdioma.length);
	//}	
	
	function verificaIdiomas(campo){

		var inputIdiomaName = document.getElementsByName('ftiid[]');
		var inputCheckBox	= document.getElementById(campo.id);
		
		if(inputIdiomaName[0][0].value == ''){
			alert("Escolha um idioma primeiro! ");
			inputCheckBox.checked = "";
			//inputIdiomaName.focus();
			return false;
		}
	}
	
	function verificaAtividades(campo){

		var inputIdiomaName = document.getElementsByName('ftaid[]');
		var inputCheckBox	= document.getElementById(campo.id);
		
		if(inputIdiomaName[0][0].value == ''){
			alert("Escolha uma atividade primeiro! ");
			inputCheckBox.checked = "";
			//inputIdiomaName.focus();
			return false;
		}
	}
	
	function mostraTipoDeficiencia(campo){
	
		var tipoDeficiencia = document.getElementById('tr_fdpdeficiencia');		
		
		if(campo.checked == true){
			tipoDeficiencia.style.display = "";
		} else {
			tipoDeficiencia.style.display = "none";	
		}
				
	}

	function pdeescola_exibeRelatorioGeralXLS(){		
		
		var formulario = document.filtro;
		var agrupador  = document.getElementById( 'agrupador' );
		
		// Tipo de relatorio
		formulario.pesquisa.value='2';
		
		prepara_formulario();
		selectAllOptions( formulario.agrupador );
		
		if ( !agrupador.options.length ){
			alert( 'Favor selecionar ao menos um item para agrupar o resultado!' );
			agrupador.focus();
			return false;
		}		
		
		
		if ( document.getElementById('fstid_campo_flag').value != 1 ){
			alert( 'Favor selecionar ao menos uma Situação no MEC!' );
			document.getElementById('fstid_campo_flag').focus();
			return false;
		}			
		
		selectAllOptions( agrupador );				
		selectAllOptions( document.getElementById( 'fstid' ) ); // Situação no MEC				
		selectAllOptions( document.getElementById( 'eciid' ) ); // Estado Civil		
		selectAllOptions( document.getElementById( 'estuf' ) ); // UF		
		selectAllOptions( document.getElementById( 'fdpdeficiencia' ) ); // Tipo Deficiência
		selectAllOptions( document.getElementById( 'fcmid' ) ); // Cargo Efetivo no MEC
		selectAllOptions( document.getElementById( 'fulid' ) ); // Unidade de Lotação
		selectAllOptions( document.getElementById( 'tfoid' ) ); // Grau de Escolaridade
		selectAllOptions( document.getElementById( 'ftiid' ) ); // Idioma
		selectAllOptions( document.getElementById( 'ftaid' ) ); // Atividade Desenvolvida
		selectAllOptions( document.getElementById( 'fteid' ) ); // Tipo de Experiência Anterior
		
		formulario.submit();
		
	}
	
	function pdeescola_exibeRelatorioGeral(tipo){
		
		var formulario = document.filtro;
		var agrupador  = document.getElementById( 'agrupador' );

		// Tipo de relatorio
		formulario.pesquisa.value='1';
		
		prepara_formulario();
		selectAllOptions( formulario.agrupador );
		
		if ( tipo == 'relatorio' ){
			
			formulario.action = 'gestaopessoa.php?modulo=relatorio/relGeral&acao=A';
			window.open( '', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			formulario.target = 'relatorio';
			
		}else {
		
			if ( tipo == 'planilha' ){
			
				if ( !agrupador.options.length ){
					alert( 'Favor selecionar ao menos um item para agrupar o resultado!' );
					agrupador.focus();
					return false;
				}				
				
				if ( document.getElementById('fstid_campo_flag').value != 1 ){
					alert( 'Favor selecionar ao menos uma Situação no MEC!' );
					document.getElementById('fstid_campo_flag').focus();
					return false;
				}		
				
				formulario.action = 'gestaopessoa.php?modulo=relatorio/relGeral&acao=A&tipoRelatorio=xls';
				
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
				formulario.action = 'gestaopessoa.php?modulo=relatorio/relGeral&acao=A&salvar=1';
				formulario.target = '_self';
		
			}else if( tipo == 'exibir' ){
			
				if ( !agrupador.options.length ){
					alert( 'Favor selecionar ao menos um item para agrupar o resultado!' );
					agrupador.focus();
					return false;
				}				
				
				if ( document.getElementById('fstid_campo_flag').value != 1 ){
					alert( 'Favor selecionar ao menos uma Situação no MEC!' );
					document.getElementById('fstid_campo_flag').focus();
					return false;
				}		
				
				selectAllOptions( agrupador );
				selectAllOptions( document.getElementById( 'fstid' ) ); // Situação no MEC				
				selectAllOptions( document.getElementById( 'eciid' ) ); // Estado Civil		
				selectAllOptions( document.getElementById( 'estuf' ) ); // UF		
				selectAllOptions( document.getElementById( 'fdpdeficiencia' ) ); // Tipo Deficiência
				selectAllOptions( document.getElementById( 'fcmid' ) ); // Cargo Efetivo no MEC
				selectAllOptions( document.getElementById( 'fulid' ) ); // Unidade de Lotação
				selectAllOptions( document.getElementById( 'tfoid' ) ); // Grau de Escolaridade
				selectAllOptions( document.getElementById( 'ftiid' ) ); // Idioma
				selectAllOptions( document.getElementById( 'ftaid' ) ); // Atividade Desenvolvida
				selectAllOptions( document.getElementById( 'fteid' ) ); // Tipo de Experiência Anterior				
				
				formulario.target = 'resultadoFtGeral';
				var janela = window.open( '', 'resultadoFtGeral', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
				janela.focus();
			}
		}
		
		formulario.submit();
		
	}	
	
	function tornar_publico( prtid ){
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
		pdeescola_exibeRelatorioGeral( 'relatorio' );
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
</script>
