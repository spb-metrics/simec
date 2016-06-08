<?php

// exibe consulta
if ( isset( $_REQUEST['form'] ) == true ) {
	
	include "checklist_resultado2.inc";
	exit;
	
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
$titulo_modulo = "Relatório Checklist - Qtde";
monta_titulo( $titulo_modulo, 'Selecione os filtros e agrupadores desejados' );

?>
<script type="text/javascript">
	
	function checklist_exibeRelatorioGeral(tipo){
		
		var formulario = document.filtro;
		var agrupador  = document.getElementById( 'agrupadores' );

		// Tipo de relatorio
		formulario.pesquisa.value='1';

		
		prepara_formulario();
		selectAllOptions( formulario.agrupadores );
		
		if( tipo == 'exibir' ){
			 
			if ( !agrupador.options.length ){
				alert( 'Favor selecionar ao menos um agrupador!' );
				return false;
			}
			
			selectAllOptions( agrupador );
			selectAllOptions( document.getElementById( 'atividade' ) );
			
			formulario.target = 'resultadoChecklistGeral';
			var janela = window.open( '', 'resultadoChecklistGeral', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			janela.focus();

		}
		
		if( tipo == 'exibirxls' ){
		
			formulario.pesquisa.value='2';
			 
			if ( !agrupador.options.length ){
				alert( 'Favor selecionar ao menos um agrupador!' );
				return false;
			}
			
			selectAllOptions( agrupador );
			selectAllOptions( document.getElementById( 'atividade' ) );

		}
		
		formulario.submit();
		
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
	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
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
						'etapas' => array(
													'codigo'    => 'etapas',
													'descricao' => 'Etapas'
						),
						'processos' => array(
													'codigo'    => 'processos',
													'descricao' => 'Processos'
						),
						'subprocessos' => array(
													'codigo'    => 'subprocessos',
													'descricao' => 'Sub-processos'
						),
						'atividades' => array(
													'codigo'    => 'atividades',
													'descricao' => 'Atividades'
						)
						
					);
					
					// exibe agrupador
					$agrupador->setOrigem( 'naoAgrupadores', null, $origem );
					$agrupador->setDestino( 'agrupadores', null, $destino );
					$agrupador->exibir();
				?>
			</td>
		</tr>
		</table>
		
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<tr><td colspan="2" class="SubTituloEsquerda">Filtros</td></tr>
		<?php
				// Atividades
				$stSql = " SELECT
								ati.atiid AS codigo,
								ati._atinumero||' - '||ati.atidescricao AS descricao
							FROM 
								projetos.atividade ati 
							INNER JOIN 
								projetos.itemchecklist icl ON icl.atiid = ati.atiid 
							GROUP BY
								ati.atiid, ati.atidescricao, ati._atinumero, ati._atiordem  
							ORDER BY
								ati._atiordem";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Atividades', 'atividade',  $stSql, $stSqlCarregados, 'Selecione a(s) Atividade(s)' );
		?>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" value="Visualizar" onclick="checklist_exibeRelatorioGeral('exibir');" style="cursor: pointer;"/>
				<input type="button" value="Visualizar XLS" onclick="checklist_exibeRelatorioGeral('exibirxls');" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>