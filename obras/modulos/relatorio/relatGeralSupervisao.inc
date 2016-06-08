<?php

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
print '<br/>';
monta_titulo( 'Relatório - Monitoramento de Obras', 'Relatório Geral de Supervisão' );

?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script type="text/javascript"><!--

	function exibeRelatorio()
	{
		var cont = 0;
		$('[name=orgid[]]').each(function()
		{
			if( $(this).attr('checked') ) cont++;
		});

		if( cont == 0 )
		{
			alert('Pelo menos 1(um) "Tipo de Estabelecimento" deve ser selecionado.');
			return false;
		}

		if( $('#agrupador option').length == 0 )
		{
			alert('Pelo menos 1(um) "Agrupador" deve ser escolhido.');
			return false;
		}

		if( ($('#dt_tramitacao_grupo_ini').val() != '' && $('#dt_tramitacao_grupo_fim').val() == '') || ($('#dt_tramitacao_grupo_ini').val() == '' && $('#dt_tramitacao_grupo_fim').val() != '') )
		{
			alert('Deve-se informar as duas datas no filtro "Intervalo de Data da Última Tramitação do Grupo"');
			return false;
		}

		if( ($('#dt_tramitacao_obra_ini').val() != '' && $('#dt_tramitacao_obra_fim').val() == '') || ($('#dt_tramitacao_obra_ini').val() == '' && $('#dt_tramitacao_obra_fim').val() != '') )
		{
			alert('Deve-se informar as duas datas no filtro "Intervalo de Data da Última Tramitação da Obra"');
			return false;
		}
		
		var formulario 		= document.getElementById('formRelatorio');
		formulario.action	= 'obras.php?modulo=relatorio/popGeralSupervisao&acao=A';
		
		var janela = window.open( '', 'relatorio', 'width=800,height=700,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		janela.focus();

		var agrupador  = document.getElementById( 'agrupador' );

		selectAllOptions( agrupador );
		selectAllOptions( document.getElementById( 'esdid_grupo' ) );
		selectAllOptions( document.getElementById( 'esdid_obra' ) );
		selectAllOptions( document.getElementById( 'entid_unidade' ) );
		selectAllOptions( document.getElementById( 'entid_campus' ) );
		selectAllOptions( document.getElementById( 'epcid_empresa' ) );
		selectAllOptions( document.getElementById( 'uf' ) );
		selectAllOptions( document.getElementById( 'muncod' ) );
		selectAllOptions( document.getElementById( 'regcod' ) );
		selectAllOptions( document.getElementById( 'mescod' ) );
		selectAllOptions( document.getElementById( 'orgid_tipoensino' ) );
		selectAllOptions( document.getElementById( 'gpdid' ) );
		
		formulario.target = 'relatorio';
		formulario.submit();
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

//
--></script>

<form id="formRelatorio" method="post" action="">
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="SubTituloDireita" style="width:250px;">Tipo de Estabelecimento:</td>
			<td>
			<?php 
			$orgaos = $db->carregar("SELECT orgid,orgdesc FROM obras.orgao");
			for($i=0; $i<count($orgaos); $i++)
			{
				echo '<input type="checkbox" id="orgid" name="orgid[]" value="'.$orgaos[$i]['orgid'].'" />'.$orgaos[$i]["orgdesc"].'&nbsp;';
			}
			?>	
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Agrupadores:</td>
			<td>
			<?php
				// Início dos agrupadores
				$agrupador = new Agrupador('formRelatorio','');
				
				// Dados padrão de destino (nulo)
				if(!$agrupador2){
					/*$destino = array('nuprocesso' => array(
										'codigo'    => 'nuprocesso',
										'descricao' => 'Nº do Processo'
									)
								);*/
				}
				else{
					$destino = $agrupador2;
				}
				
				// Dados padrão de origem
				$origem = array(
					'situacaogrupo' => array(
						'codigo'    => 'situacaogrupo',
						'descricao' => 'Situação do Grupo'
					),
					'situacaoobra' => array(
						'codigo'    => 'situacaoobra',
						'descricao' => 'Situação da Obra'
					),
					'empresa' => array(
						'codigo'    => 'empresa',
						'descricao' => 'Empresa'
					),
					'uf' => array(
						'codigo'    => 'uf',
						'descricao' => 'UF'
					),
					'municipio' => array(
						'codigo'    => 'municipio',
						'descricao' => 'Município'
					),	
					'regiao' => array(
						'codigo'    => 'regiao',
						'descricao' => 'Região'
					),
					'mesorregiao' => array(
						'codigo'    => 'mesorregiao',
						'descricao' => 'Mesorregião'
					),
					'tipoensino' => array(
						'codigo'    => 'tipoensino',
						'descricao' => 'Tipo de Estabecimento'
					),
					'brasil' => array(
						'codigo'    => 'brasil',
						'descricao' => 'Brasil'
					),
					'unidade' => array(
						'codigo'    => 'unidade',
						'descricao' => 'Unidade'
					),
					'campus' => array(
						'codigo'    => 'campus',
						'descricao' => 'Estabelecimento'
					),
					'nomeobra' => array(
						'codigo'    => 'nomeobra',
						'descricao' => 'Nome da Obra'
					),
					'grupo' => array(
						'codigo'    => 'grupo',
						'descricao' => 'Grupo'
					)
				);
				
				// exibe agrupador
				$agrupador->setOrigem( 'naoAgrupador', null, $origem );
				$agrupador->setDestino( 'agrupador', null, $destino );
				$agrupador->exibir();
			?>
			</td>
		</tr>
		
		<!-- Intervalo de Data da Última Tramitação do Grupo -->
		<tr>
			<td class="SubTituloDireita" style="width:300px;">Intervalo de Data da Última Tramitação do Grupo:</td>
			<td>
			<?=campo_data2('dt_tramitacao_grupo_ini','N','S','Intervalo de Data da Última Tramitação do Grupo','','','', null,'', '', 'dt_tramitacao_grupo_ini' );?>
			&nbsp;até&nbsp;
			<?=campo_data2('dt_tramitacao_grupo_fim','N','S','Intervalo de Data da Última Tramitação do Grupo','','','', null,'', '', 'dt_tramitacao_grupo_fim' );?>
			</td>
		</tr>
		
		<!-- Intervalo de Data da Última Tramitação da Obra -->
		<tr>
			<td class="SubTituloDireita">Intervalo de Data da Última Tramitação da Obra:</td>
			<td>
			<?=campo_data2('dt_tramitacao_obra_ini','N','S','Intervalo de Data da Última Tramitação da Obra','','','', null,'', '', 'dt_tramitacao_obra_ini' );?>
			&nbsp;até&nbsp;
			<?=campo_data2('dt_tramitacao_obra_fim','N','S','Intervalo de Data da Última Tramitação da Obra','','','', null,'', '', 'dt_tramitacao_obra_fim' );?>
			</td>
		</tr>
		
		<!-- Situação do Parecer -->
		<tr>
			<td class="SubTituloDireita" style="width:250px;">Situação do Parecer:</td>
			<td>
				<input type="radio" value="T" name="parecer" checked="checked" />Todos
				&nbsp;
				<input type="radio" value="S" name="parecer" />Aprovado
				&nbsp;
				<input type="radio" value="N" name="parecer" />Não Aprovado
			</td>
		</tr>
		
		<!-- Preenchimento do Checklist -->
		<tr>
			<td class="SubTituloDireita">Preenchimento do Checklist:</td>
			<td>
				<input type="radio" value="T" name="checklist" checked="checked" />Todos
				&nbsp;
				<input type="radio" value="S" name="checklist" />Sim
				&nbsp;
				<input type="radio" value="N" name="checklist" />Não
			</td>
		</tr>
		
		<!-- Situação do Grupo -->
		<?php
		$sql = "SELECT DISTINCT
					esdid as codigo,
					esddsc as descricao
				FROM
					workflow.estadodocumento
				WHERE
					tpdid = " . OBR_TIPO_DOCUMENTO . "
					AND esdid <> ".OBRENVREAVALSUPMEC."
					AND esdid <> ".OBREMAVALIASUPERVMEC."
					AND esdid <> ".OBREMSUPERVISAO." 
					AND esdid <> ".OBRREAJSUPVISAOEMP." 
					AND esdid <> ".OBRREAVSUPVISAO." 
					AND esdstatus = 'A'
				ORDER BY
					esddsc";
		$sqlCarregados 	= "";
		$arrVisivel 	= array("descricao");
		$arrOrdem		= array("descricao");
		mostrarComboPopup('Situação do Grupo:', 'esdid_grupo', $sql, $sqlCarregados, 'Selecione a(s) Situação(ões) do(s) Grupo(s)', null, null, null, null, $arrVisivel, $arrOrdem);
		?>
		
		<!-- Situação da Obra -->
		<?php
		$sql = "SELECT DISTINCT
					esdid as codigo,
					esddsc as descricao
				FROM
					workflow.estadodocumento ed
				WHERE
					tpdid = " . OBR_TIPO_DOCUMENTO_OBRA . "
				ORDER BY
					esddsc";
		$sqlCarregados 	= "";
		$arrVisivel 	= array("descricao");
		$arrOrdem		= array("descricao");
		mostrarComboPopup('Situação da Obra:', 'esdid_obra', $sql, $sqlCarregados, 'Selecione a(s) Situação(ões) da(s) Obra(s)', null, null, null, null, $arrVisivel, $arrOrdem);
		?>
		
		<!-- Unidade -->
		<?php
		$sql = "SELECT DISTINCT
					ent.entid AS codigo,
					ent.entnome AS descricao
				FROM 
					entidade.entidade ent
				INNER JOIN
					obras.obrainfraestrutura obr ON obr.entidunidade = ent.entid
												AND obr.obsstatus = 'A'
				WHERE 
					ent.entstatus = 'A'
				ORDER BY
					ent.entnome ASC";
		$sqlCarregados 	= "";
		$arrVisivel 	= array("descricao");
		$arrOrdem		= array("descricao");
		mostrarComboPopup('Unidade:', 'entid_unidade', $sql, $sqlCarregados, 'Selecione a(s) Unidade(s)', null, null, null, null, $arrVisivel, $arrOrdem);
		?>
		
		<!-- Campus -->
		<?php
		$sql = "SELECT DISTINCT
					ent.entid AS codigo,
					ent.entnome AS descricao
				FROM 
					entidade.entidade ent
				INNER JOIN
					obras.obrainfraestrutura obr ON obr.entidcampus = ent.entid
												AND obr.obsstatus = 'A'
				WHERE 
					ent.entstatus = 'A'
				ORDER BY
					ent.entnome ASC";
		$sqlCarregados 	= "";
		$arrVisivel 	= array("descricao");
		$arrOrdem		= array("descricao");
		mostrarComboPopup('Estabelecimento:', 'entid_campus', $sql, $sqlCarregados, 'Selecione o(s) Estabelecimento', null, null, null, null, $arrVisivel, $arrOrdem);
		
		?>
		
		<!-- Empresa -->
		<?php
		$sql = "SELECT DISTINCT
					ent.entid AS codigo,
					ent.entnome AS descricao
				FROM 
					entidade.entidade ent
				INNER JOIN
					obras.empresacontratada epc ON epc.entid = ent.entid
				WHERE 
					ent.entstatus = 'A'
				ORDER BY
					ent.entnome ASC";
		$sqlCarregados 	= "";
		$arrVisivel 	= array("descricao");
		$arrOrdem		= array("descricao");
		mostrarComboPopup('Empresa:', 'epcid_empresa', $sql, $sqlCarregados, 'Selecione a(s) Empresa(s)', null, null, null, null, $arrVisivel, $arrOrdem);
		
		?>
		
		<!-- UF -->
		<?php
		$sql = "SELECT DISTINCT
					estuf AS codigo,
					estdescricao AS descricao
				FROM 
					territorios.estado
				ORDER BY
					estdescricao ASC";
		$sqlCarregados 	= "";
		$arrVisivel 	= array("descricao");
		$arrOrdem		= array("descricao");
		mostrarComboPopup('UF:', 'uf', $sql, $sqlCarregados, 'Selecione a(s) UF(s)', null, null, null, null, $arrVisivel, $arrOrdem);
		
		?>
		
		<!-- Município -->
		<?php
		$sql = "SELECT DISTINCT
					muncod AS codigo,
					mundescricao AS descricao
				FROM 
					territorios.municipio
				ORDER BY
					mundescricao ASC";
		$sqlCarregados 	= "";
		$arrVisivel 	= array("descricao");
		$arrOrdem		= array("descricao");
		mostrarComboPopup('Município:', 'muncod', $sql, $sqlCarregados, 'Selecione o(s) Município(s)', null, null, null, null, $arrVisivel, $arrOrdem);
		
		?>
		
		<!-- Região -->
		<?php
		$sql = "SELECT DISTINCT
					regcod AS codigo,
					regdescricao AS descricao
				FROM 
					territorios.regiao
				ORDER BY
					regdescricao ASC";
		$sqlCarregados 	= "";
		$arrVisivel 	= array("descricao");
		$arrOrdem		= array("descricao");
		mostrarComboPopup('Região:', 'regcod', $sql, $sqlCarregados, 'Selecione a(s) Região(ões)', null, null, null, null, $arrVisivel, $arrOrdem);
		
		?>
		
		<!-- Mesorregião -->
		<?php
		$sql = "SELECT DISTINCT
					mescod AS codigo,
					mesdsc AS descricao
				FROM 
					territorios.mesoregiao
				ORDER BY
					mesdsc ASC";
		$sqlCarregados 	= "";
		$arrVisivel 	= array("descricao");
		$arrOrdem		= array("descricao");
		mostrarComboPopup('Mesorregião:', 'mescod', $sql, $sqlCarregados, 'Selecione a(s) Mesorregião(ões)', null, null, null, null, $arrVisivel, $arrOrdem);
		
		?>
		
		<!-- Tipo de Estabelecimento -->
		<?php
		$sql = "SELECT DISTINCT
					orgid AS codigo,
					orgdesc AS descricao
				FROM 
					obras.orgao
				WHERE
					orgstatus = 'A'
				ORDER BY
					orgdesc ASC";
		$sqlCarregados 	= "";
		$arrVisivel 	= array("descricao");
		$arrOrdem		= array("descricao");
		mostrarComboPopup('Tipo de Estabelecimento:', 'orgid_tipoensino', $sql, $sqlCarregados, 'Selecione o(s) Tipo de Estabelecimento', null, null, null, null, $arrVisivel, $arrOrdem);
		
		?>
		
		<!-- Grupo -->
		<?php
		$sql = "SELECT DISTINCT
					gpdid AS codigo,
					gpdid AS descricao
				FROM 
					obras.grupodistribuicao
				WHERE
					gpdstatus = 'A'
				ORDER BY
					gpdid ASC";
		$sqlCarregados 	= "";
		$arrVisivel 	= array("descricao");
		$arrOrdem		= array("descricao");
		mostrarComboPopup('Grupo:', 'gpdid', $sql, $sqlCarregados, 'Selecione o(s) Grupo(s)', null, null, null, null, $arrVisivel, $arrOrdem);
		
		?>
		
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" value="Visualizar" onclick="exibeRelatorio();" style="cursor:pointer;" />
				<input type="button" value="Salvar Consulta" onclick="exibeRelatorioProcesso('salvar');" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>
