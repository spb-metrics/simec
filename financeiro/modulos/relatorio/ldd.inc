<?php

//ini_set( 'display_errors', 1 );

include 'Agrupador.php';
include 'ConsultaFinanceiro.inc';
include 'ConsultaLiquidacao.inc';

$titulo    = '';
$ano       = $_SESSION['exercicio'];
$agrupador = array();
$prtid     = '';
$escala    = '1';
$nao_mostra_filtro_impressao = false;

if ( isset( $_REQUEST['form'] ) == true )
{
	$titulo    = $_REQUEST['titulo'];
	$ano       = $_REQUEST['ano'];
	$agrupador = (array) $_REQUEST['agrupador'];
	$prtid     = $_REQUEST['prtid'];
	$escala    = $_REQUEST['escala'];
	$nao_mostra_filtro_impressao = (boolean) $_REQUEST['nao_mostra_filtro_impressao'];
	$rap 	= (boolean) $_REQUEST['rap'];

	if ( $_REQUEST['alterar_ano'] == '1' )
	{
		if ( $_REQUEST['agrupador'] )
		{
			foreach ( $_REQUEST['agrupador'] as $valorAgrupador )
			{
				array_push( $agrupador, array( 'codigo' => $valorAgrupador, 'descricao' => ConsultaFinanceiroTraducao::pegarTitulo( $valorAgrupador ) ) );
			}
		}
	} else {
		if ( $_REQUEST['prtid'] )
		{
			$sql = sprintf(	"select prtobj from public.parametros_tela where prtid = " . $_REQUEST['prtid'] );
			$itens = $db->pegaUm( $sql );
			$dados = unserialize( stripslashes( stripslashes( $itens ) ) );
			$_REQUEST = $dados;
			unset( $_REQUEST['salvar'] );
		}
		include 'resultado_ldd.inc'; 
		exit();
	}
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Liquidação das Dotações Descentralizadas', 'Relatório Financeiro' );

?>
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
	
	/**
	 * Realiza submissão de formulário. Caso o exercício (ano) tenha sido
	 * alterado a submissão é realizada para a própria página, caso contrário
	 * para uma nova janela.
	 * 
	 * @return void
	 */
	function submeterFormulario( tipo )
	{
		var formulario = document.formulario;
		var nomerel = '';
		var qtd = 0;
		<? $qtdrel = 0; ?>
		prepara_formulario();
		selectAllOptions( formulario.agrupador );
		
		// verifica se foi escolhido algum agrupador
		if ( formulario.alterar_ano.value == '0' )
		{
			if ( formulario.agrupador.options.length == 0 )
			{
				alert( 'Escolha pelo menos um agrupador.' );
				return;
			}
		}

		if ( formulario.alterar_ano.value == '0' && tipo == 'relatorio' )
		{
			
			formulario.action = 'financeiro.php?modulo=relatorio/ldd&acao=R';
			window.open( '', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			formulario.target = 'relatorio';
		}
		else
		{
			
			
			
			if ( tipo == 'planilha' )
			{
				formulario.action = 'financeiro.php?modulo=relatorio/ldd&acao=R&tipoRelatorio=xls';
			}
			else if ( tipo == 'salvar' ) 
			{
			
			
				if ( document.formulario.titulo.value == '' ) 
				{
					alert( 'É necessário informar a descrição do relatório!' );
					document.formulario.titulo.focus();
					return;
				}
				var nomesExistentes = new Array();
				<?php
					$sqlNomesConsulta = "select prtdsc from public.parametros_tela";
					$nomesExistentes = $db->carregar( $sqlNomesConsulta );
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
					if ( nomesExistentes[i] == document.formulario.titulo.value )
					{
						confirma = confirm( 'Deseja alterar a consulta já existente?' );
						break;
					}
				}
				if ( !confirma )
				{
					return;
				}
				formulario.action = 'financeiro.php?modulo=relatorio/ldd&acao=R&salvar=1';
			}
			else
			{
				formulario.action = '';
			}
			formulario.target = '_top';
		}
		formulario.submit();
	}
	
	function alterarAno()
	{
		var formulario = document.formulario;
		formulario.alterar_ano.value = '1';
		submeterFormulario('relatorio');
	}
	
</script>
<form action="" method="post" name="formulario">
	
	<input type="hidden" name="form" value="1"/> <!-- indica envio de formulário -->
	<input type="hidden" name="alterar_ano" value="0"/> <!-- indica se há mudança de ano no formulário -->
	<input type="hidden" name="publico" value="<?= $publico ?>"/> <!-- indica se foi clicado para tornar o relatório público ou privado -->
	<input type="hidden" name="prtid" value="<?= $prtid ?>"/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
	<input type="hidden" name="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
	<input type="hidden" name="excluir" value="<?= $excluir ?>"/> <!-- indica se foi clicado para excluir o relatório já gravado -->
	
	<!-- OPÇÕES -->
	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
		<!--
		<tr>
			<td class="SubTituloDireita" valign="top">Título</td>
			<td>
				<?= campo_texto( 'titulo', 'N', 'S', '', 78, 100, '', '' ); ?>
			</td>
		</tr>
		-->
		<tr>
			<td class="SubTituloDireita" valign="top">Exercício</td>
			<td>
				<select name="ano" onchange="alterarAno();">
					<option value="2006" <?= $ano == '2006' ? 'selected="selected"' : '' ; ?>>2006</option>
					<option value="2007" <?= $ano == '2007' ? 'selected="selected"' : '' ; ?>>2007</option>
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
			<td class="SubTituloDireita" valign="top">RAP</td>
			<td>
				<input type="checkbox" name="rap" id="rap" value="1" <?= $rap ? 'checked="checked"' : '' ; ?>/>
				<label for="rap">mostrar relatório RAP</label>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Filtro</td>
			<td>
				<input type="checkbox" name="nao_mostra_filtro_impressao" id="nao_mostra_filtro_impressao" value="1" <?= $nao_mostra_filtro_impressao ? 'checked="checked"' : '' ; ?>/>
				<label for="nao_mostra_filtro_impressao">Não mostrar filtros</label>
			</td>
		</tr>
		<tr>
			<td width="195" class="SubTituloDireita" valign="top">Agrupadores</td>
			<td>
				<?php
					$matriz = array(
						array(
							'codigo' => 'gestaoExecucao',
							'descricao' => ConsultaFinanceiroTraducao::pegarTitulo( 'gestaoExecucao' )
						),
						array(
							'codigo' => 'ugExecucao',
							'descricao' => ConsultaFinanceiroTraducao::pegarTitulo( 'ugExecucao' )
						),
						array(
							'codigo' => 'acacodExecucao',
							'descricao' => ConsultaFinanceiroTraducao::pegarTitulo( 'acacodExecucao' )
						),
						array(
							'codigo' => 'localizadorExecucao',
							'descricao' => ConsultaFinanceiroTraducao::pegarTitulo( 'localizadorExecucao' )
						),
						array(
							'codigo' => 'programaExecucao',
							'descricao' => ConsultaFinanceiroTraducao::pegarTitulo( 'programaExecucao' )
						),
						array(
							'codigo' => 'uoExecucao',
							'descricao' => ConsultaFinanceiroTraducao::pegarTitulo( 'uoExecucao' )
						),
					);
					$agrupador = new Agrupador( 'formulario' );
					$agrupador->setOrigem( 'agrupadorOrigem', null, $matriz );
					$agrupador->setDestino( 'agrupador', 4 );
					$agrupador->exibir();
				?>
			</td>
		</tr>
	</table>

	<!-- FILTROS -->

	<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
		<tr>
			<td onclick="javascript:onOffBloco( 'filtros' );">
				<img border="0" src="/imagens/mais.gif" id="func_img"/>&nbsp;
				Filtros
				<input type="hidden" id="func_flag" name="inst_flag" value="0" />
			</td>
		</tr>
	</table>
	<div id="filtros_div_filtros_off"></div>
	<div id="filtros_div_filtros_on" style="display:none;">
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
			<tr>
				<td width="195" class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'acacodExecucao' );">
					Ação<input type="hidden" id="acacodExecucao_campo_flag" name="acacodExecucao_campo_flag" value="0"/>
				</td>
				<td>
					<div id="acacodExecucao_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'acacodExecucao' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="acacodExecucao_campo_on" style="display:none;">
						<?php
							//$sql_combo = "select acao.acacod as codigo, acao.acacod || ' - ' || acao.acadsc as descricao from financeiro.execucaomec mec inner join ( select distinct acacod, acadsc, prgano from monitora.acao where prgano = '2007' and acasnrap = 'f' group by acacod, acadsc, prgano ) acao on acao.acacod = mec.acacod and acao.prgano = mec.exeano where mec.exeano = '" . $ano . "' group by acao.acacod, acao.acadsc order by acao.acacod, acao.acadsc"; 
							$sql_combo = "select mec.acacod as codigo, mec.acacod as descricao from financeiro.execucaomec mec where mec.exeano = '" . $ano . "' group by mec.acacod order by mec.acacod "; 
							if ( $_REQUEST['acacod'] && $_REQUEST['acacod'][0] != '' ) {
								//$sql_carregados = "select acao.acacod as codigo, acao.acacod || ' - ' || acao.acadsc as descricao from financeiro.reporcfin r inner join ( select distinct acacod, acadsc, prgano from monitora.acao where prgano = '2007' and acasnrap = 'f' group by acacod, acadsc, prgano ) acao on acao.acacod = r.acacod and acao.prgano = r.rofano where r.rofano = '" . $ano . "' and acao.acacod in ('".implode("','",$_REQUEST['acacod'])."') group by acao.acacod, acao.acadsc ";
								$sql_carregados = "select mec.acacod as codigo, mec.acacod as descricao from financeiro.execucaomec mec where mec.exeano = '" . $ano . "' and mec.acacod in ('".implode("','",$_REQUEST['acacod'])."') group by mec.acacod ";
								$acacod = $db->carregar( $sql_carregados );
							}
							combo_popup( 'acacodExecucao', $sql_combo, 'Selecione a(s) Ação(ões)', '400x400', 0, array(), '', 'S', true, true );
						?>
					</div>
				</td>
			</tr>
			<? if ( $acacod ): ?>
				<script type="text/javascript"> onOffCampo( 'acacodExecucao' ); </script>
			<? endif; ?>
			<!--
			<tr>
				<td width="195" class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'fonte' );">
					Fonte Detalhada<input type="hidden" id="fonte_campo_flag" name="fonte_campo_flag" value="0"/>
				</td>
				<td>
					<div id="fonte_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'fonte' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="fonte_campo_on" style="display:none;">
						&nbsp;
					</div>
				</td>
			</tr>
			-->
			<tr>
				<td width="195" class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'ugExecucao' );">
					Unidade Gestora<input type="hidden" id="ugExecucao_campo_flag" name="ugExecucao_campo_flag" value="0"/>
				</td>
				<td>
					<div id="ugExecucao_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'ugExecucao' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="ugExecucao_campo_on" style="display:none;">
						<?php
							$sql_combo = "select ug.ungcod as codigo, ug.ungcod || ' - ' || ug.ungdsc as descricao from financeiro.execucaomec mec inner join public.unidadegestora ug on ug.ungcod = mec.ungcod where mec.exeano = '". $ano ."' group by ug.ungcod, ug.ungdsc"; 
							if ( $_REQUEST['ug'] && $_REQUEST['ug'][0] != '' ) {
								$sql_carregados = "select ug.ungcod as codigo, ug.ungcod || ' - ' || ug.ungdsc as descricao from financeiro.execucaomec mec inner join public.unidadegestora ug on ug.ungcod = uo.ungcod where mec.exeano = '". $ano ."' and ug.ungcod in ('". implode( "','", $_REQUEST['ug'] ) ."') group by ug.ungcod, ug.ungdsc";
								$ug = $db->carregar( $sql_carregados );
							}
							combo_popup( 'ugExecucao', $sql_combo, 'Selecione a(s) Unidade(s) Gestora(s)', '400x400', 0, array(), '', 'S', true, true );
						?>
					</div>
				</td>
			</tr>
			<? if ( $ug ) : ?>
				<script type="text/javascript"> onOffCampo( 'ugExecucao' ); </script>
			<? endif; ?>
			<tr>
				<td width="195" class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'uoExecucao' );">
					Unidade Orçamentária<input type="hidden" id="uoExecucao_campo_flag" name="uoExecucao_campo_flag" value="0"/>
				</td>
				<td>
					<div id="uoExecucao_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'uoExecucao' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="uoExecucao_campo_on" style="display:none;">
						<?php
							$sql_combo = "select u.unicod as codigo, u.unicod || ' - ' || u.unidsc as descricao from financeiro.execucaomec mec inner join public.unidade u on mec.unicod = u.unicod where mec.exeano = '" . $ano . "' group by u.unicod, u.unidsc order by u.unicod, u.unidsc"; 
							if ( $_REQUEST['uoExecucao'] && $_REQUEST['uoExecucao'][0] != '' ) {
								$sql_carregados = "select u.unicod as codigo, u.unicod || ' - ' || u.unidsc as descricao from financeiro.execucaomec mec inner join public.unidade u on mec.unicod = u.unicod where mec.exeano = '" . $ano . "' and mec.unicod in ('".implode("','",$_REQUEST['uoExecucao'])."') group by u.unicod, u.unidsc ";
								$uo = $db->carregar( $sql_carregados );
							}
							combo_popup( 'uoExecucao', $sql_combo, 'Selecione a(s) Unidade(s) Orçamentária(s)', '400x400', 0, array(), '', 'S', true, true );
						?>
					</div>
				</td>
			</tr>
			<? if ( $uoExecucao ) : ?>
				<script type="text/javascript"> onOffCampo( 'uoExecucao' ); </script>
			<? endif; ?>
			<tr>
				<td width="195" class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'gestaoExecucao' );">
					Gestão<input type="hidden" id="gestaoExecucao_campo_flag" name="gestaoExecucao_campo_flag" value="0"/>
				</td>
				<td>
					<div id="gestaoExecucao_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'gestaoExecucao' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="gestaoExecucao_campo_on" style="display:none;">
						<?php
							$sql_combo = "select mec.gstcod as codigo, mec.gstcod as descricao from financeiro.execucaomec mec where mec.exeano = '". $ano ."' group by mec.gstcod "; 
							if ( $_REQUEST['gestaoExecucao'] && $_REQUEST['gestaoExecucao'][0] != '' ) {
								$sql_carregados = "select mec.gstcod as codigo, mec.gstcod as descricao from financeiro.execucaomec mec where mec.exeano = '". $ano ."' and mec.gstcod in ('". implode( "','", $_REQUEST['gestaoExecucao'] ) ."') group by mec.gstcod";
								$ug = $db->carregar( $sql_carregados );
							}
							combo_popup( 'gestaoExecucao', $sql_combo, 'Selecione as Gestões', '400x400', 0, array(), '', 'S', true, true );
						?>
					</div>
				</td>
			</tr>
			<? if ( $gestaoExecucao ) : ?>
				<script type="text/javascript"> onOffCampo( 'gestaoExecucao' ); </script>
			<? endif; ?>
			<tr>
				<td width="195" class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'fonte' );">
					Fonte de Recurso
					<input type="hidden" id="fonte_campo_flag" name="fonte_campo_flag" value="0"/>
				</td>
				<td>
					<div id="fonte_campo_off" style="color:#a0a0a0;" onclick="javascript:onOffCampo( 'fonte' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
					<div id="fonte_campo_on" style="display:none;">
						<? $sql_combo = "select fr.foncod as codigo, fr.foncod || ' - ' || fr.fondsc as descricao from financeiro.execucaomec r inner join public.fonterecurso fr on r.foncod = fr.foncod where r.exeano = '" . $ano . "' group by fr.foncod, fr.fondsc order by fr.foncod, fr.fondsc"; 
						if ( $_REQUEST['fonte'] && $_REQUEST['fonte'][0] != '' )
						{
							$sql_carregados = "select fr.foncod as codigo, fr.foncod || ' - ' || fr.fondsc as descricao from financeiro.execucaomec r inner join public.fonterecurso fr on r.foncod = fr.foncod where r.exeano = '" . $ano . "' and fr.foncod in ('".implode("','",$_REQUEST['fonte'])."') group by fr.foncod, fr.fondsc order by fr.foncod, fr.fondsc ";
							$fonte=$db->carregar( $sql_carregados );
						}
						?>
						<? combo_popup( 'fonte', $sql_combo, 'Selecione a(s) Fonte(s)', '400x400', 0, array(), '', 'S', true, true ); ?>
					</div>
				</td>
			</tr>
			<? if ( $fonte )  { ?>	<script type="text/javascript"> onOffCampo( 'fonte' ); </script> <? } ?>
			<tr>
				<td class="subtitulodireita" style="text-align: center" colspan="2">&nbsp;</td>
			</tr>
		</table>
	</div>

	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
			<tr>
				<td align="center">
					<input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:submeterFormulario( 'relatorio' );"/>
					&nbsp;
					<input type="button" name="Exportar Planilha" value="Exportar Planilha" onclick="javascript:submeterFormulario( 'planilha' );"/>
					&nbsp;
					<input type="button" name="Salvar Consulta" value="Salvar Consulta" onclick="javascript:submeterFormulario( 'salvar' );"/>
				</td>
			</tr>
	</table>

</form>
<script type="text/javascript">
	javascript:onOffBloco( 'filtros' );
</script>