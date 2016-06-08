<?php

//// FUNÇÕES ///////////////////////////////////////////////////////////////////

function adicionarFiltro( $campo )
{
	global $consulta;
	if ( $_REQUEST[$campo . '_campo_flag'] && $_REQUEST[$campo][0] != '' )
	{
		$excludente = (boolean) $_REQUEST[$campo . '_campo_excludente'];
		$consulta->adicionarFiltro( $campo, $_REQUEST[$campo], $excludente );
	}
}

function cabecalhoBrasao()
{
	global $db;
	global $consulta;
	?>
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
				Orçamento Fiscal e Seg.Social - EM R$ <?= number_format( $consulta->pegarEscala(), 2, ',', '.' ) ?><br />
				<? $whereAcumuladoAte = ''; ?>
				<? $valoresFiltroAno = $consulta->pegarValoresFiltro( 'ano' ); ?>
				<? if ( count( $valoresFiltroAno ) > 0 ) : ?>
					<? $whereAcumuladoAte = " where rofano in ( '" . implode( "','", $valoresFiltroAno ) . "' ) "; ?>
				<? endif; ?>
				<? $sqlAcumuladoAte = "select max( rofdata ) as data from financeiro.reporcfin " . $whereAcumuladoAte; ?>
				Acumulado até: <?= formata_data( $db->pegaUm( $sqlAcumuladoAte ) ) ?>
			</td>
		</tr>
		<tr>
			<td colspan="2" align="center" valign="top" style="padding:0 0 5px 0;">
				<b><font style="font-size:14px;"><?= $consulta->pegarTitulo() ?></font></b>
			</td>
		</tr>
	</table>
	<?
}

//// PROCESSA ENTRADA //////////////////////////////////////////////////////////


if ( $_REQUEST['rap'] == null ) 
include 'funcoes_ldd.inc';
else
include 'funcoes_ldd_rap.inc';

$consulta = new ConsultaFinanceiroOrcamento(); 

// carrega as opções
$consulta->alterarTitulo( $_REQUEST['titulo'] );
$consulta->alterarEscala( $_REQUEST['escala'] );

if ( is_array( $_REQUEST['agrupador'] ) == true )
{
	foreach ( $_REQUEST['agrupador'] as $agrupador )
	{
		$consulta->adicionarAgrupador( $agrupador );
	}
}
$consulta->adicionarFiltro( 'anoExecucao', $_REQUEST['ano'] );

// carrega os filtros
adicionarFiltro( 'acacodExecucao' );
adicionarFiltro( 'uoExecucao' );
adicionarFiltro( 'ugExecucao' );
adicionarFiltro( 'gestaoExecucao' );

// exibe relatório em formato xls
if ( $_REQUEST['tipoRelatorio'] == 'xls' )
{
	$qtdAgrupadores = count( $consulta->pegarAgrupadores() ) * 2;
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
	$db->sql_to_excel( $consulta->montaRequisicao(), 'relorc', '', $tipo_campo );
	exit();
}

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
				.div_rolagem{ overflow-x: auto; overflow-y: auto; width:42.5cm;height:335px;}
				.topo { position: absolute; top: 0px; margin: 0; padding: 5px; position: fixed; background-color: #ffffff;}
			}

			*{margin:0; padding:0; border:none; font-size:8px;font-family:Arial;}
			.alignRight{text-align:right !important;}
			.alignCenter{ text-align:center !important;}
			.alignLeft{text-align:left !important;}
			.bold{font-weight:bold !important;}
			.italic{font-style:italic !important;}
			.noPadding{padding:0;}
			.titulo{width:52px;}
			.tituloagrup{font-size:9px;}
			.titulolinha{font-size:9px;}
			
			#tabelaTitulos tr td, #tabelaTitulos tr th{border:2px solid black;border-left:none; border-right:none;}
			#orgao{margin:3px 0 0 0;}
			#orgao tr td{border:1px solid black;border-left:none;border-right:none;font-size:11px;}
			
			div.filtro { page-break-after: always; text-align: center; }
			
			table{width:42cm;border-collapse:collapse;}
			th, td{font-weight:normal;padding:4px;vertical-align:top;}
			thead{display:table-header-group;}
			table, tr{page-break-inside:avoid;}
			a{text-decoration:none;color:#3030aa;}
			a:hover{text-decoration:underline;color:#aa3030;}
			span.topo { position: absolute; top: 3px; margin: 0; padding: 5px; position: fixed; background-color: #f0f0f0; border: 1px solid #909090; cursor:pointer; }
			span.topo:hover { background-color: #d0d0d0; }
			
		</style>
		<script type="text/javascript">
			
			function mostrarGrafico( rastro )
			{
				var url = '../geral/graficoSiof.php' +
					'?titulo=' + escape( '<?= $consulta->pegarTitulo() ?>' ) +
					'&' + rastro;
				window.open( url, 'relatorioFinanceiroGrafico', 'width=600,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			}
			
			/*
			function ativarDesativaFiltroImpressao()
			{
				div = document.getElementById( 'filtros' );
				classes = div.className.split( ' ' );
				div.className = '';
				var classe, j = classes.length;
				var notprintEncontrado = false;
				for ( i = 0; i < j; i++ )
				{
					classe = classes[i];
					if ( classe == 'notprint' )
					{
						notprintEncontrado = true;
					}
					else
					{
						div.className += ' ' + classe + ' ';
					}
				}
				if ( notprintEncontrado == false )
				{
					div.className += ' notprint';
				}
				span = document.getElementById( 'label_mostra_filtro_impressao' );
				if ( span.innerHTML == 'esconder filtro' )
				{
					span.innerHTML = 'mostrar filtro';
				}
				else
				{
					span.innerHTML = 'esconder filtro';
				}
			}
			*/
			
		</script>
	</head>
	<body>
		<div id="aguarde" style="background-color:#ffffff;position:absolute;color:#000033;top:50%;left:30%;border:2px solid #cccccc; width:300px;">
			<center style="font-size:12px;"><br><img src="../imagens/wait.gif" border="0" align="absmiddle"> Aguarde! Gerando Relatório...<br><br></center>
		</div>
		<script type="text/javascript">
			self.focus();
		</script>
		<?php ob_flush();flush(); ?>
		<?php
		// realiza consulta, mantem dados na sessão para as imagens
		$itens = $consulta->consultar();
		$_SESSION['consulta_financeira'] = array();
		$_SESSION['consulta_financeira']['itens'] = $itens['itens'];
		$_SESSION['consulta_financeira']['agrupadores'] = $consulta->pegarAgrupadores( true );
		// FIM realiza consulta, mantem dados na sessão para as imagens
		?>
		<? if ( !$_REQUEST['nao_mostra_filtro_impressao'] ) : ?>
			<div id="filtros" class="notscreen filtro">
				<? cabecalhoBrasao(); ?>
				<b><font style="font-size:12px;">Filtros</font></b>
				<?
				function mostraFiltro( $campo )
				{
					global $consulta;
					global $db;
					$nomeCampoCod = ConsultaFinanceiroTraducao::pegarAliasCodigo( $campo );
					$nomeCampoDsc = ConsultaFinanceiroTraducao::pegarAliasDescricao( $campo );
					// trata comportamento diferente da natureza de despesa
					if ( $campo == 'natureza' )
					{
						$nomeCampoCod = "ctecod || gndcod || mapcod || edpcod";
					}
					$titulo = ConsultaFinanceiroTraducao::pegarTitulo( $campo );
					$tabela = ConsultaFinanceiroTraducao::pegarTabela( $campo );
					$filtros = $consulta->pegarValoresFiltro( $campo );
					$excludente = $consulta->filtroExcludente( $campo ) ? ' (excludente) ' : '';
					if ( count( $filtros ) > 0 )
					{
						print "<br/><br/><b>" . $titulo . "</b>" . $excludente . "<br/>";
						$sqlFiltro = "select " . $nomeCampoCod . " as codigo, " . $nomeCampoDsc . " as descricao from " . $tabela . " where " . $nomeCampoCod . " in ( '" . implode( "','", $filtros ) . "' ) group by codigo, descricao order by codigo, descricao";
						foreach ( $db->carregar( $sqlFiltro ) as $itemFiltro )
						{
							print $itemFiltro['codigo'] . " - " . $itemFiltro['descricao'] . "<br/>";
						}
					}
				}
				mostraFiltro( 'uoExecucao' );
				mostraFiltro( 'ugExecucao' );
				mostraFiltro( 'acacodExecucao' );
				mostraFiltro( 'gestao' );
			
				?>
			</div>
		<? endif; ?>
		<table>
			<thead>
				<tr>
					<th class="noPadding" align="left">
						<? cabecalhoBrasao(); ?>
						<table id="tabelaTitulos" align="left">
							<thead>
								<? 
								if ( $_REQUEST['rap'] == null ) { ?>
								<tr>
									<th class="bold alignLeft"><?= $consulta->pegarTituloAgrupador() ?></th>
									<th class="titulo alignCenter">Gestão Executora<br/></th>
									<th class="titulo alignCenter">Fontes<br/></th>
									<th class="titulo alignCenter">Créditos Recebidos<br/>a</th>
									<th class="titulo alignCenter">Empenhos Emitidos<br/>b</th>
									<th class="titulo alignCenter">Empenhos Liquidados<br/>c</th>
									<th class="titulo alignCenter">Repasse Financeiro Recebido<br/>d</th>
									<th class="titulo alignCenter">Valores Pagos<br/>e</th>
									<th class="titulo alignCenter">Liquidado a Pagar<br/>f = c -e</th>
									<th class="titulo alignCenter">Saldo Financeiro<br/>g = d - e</th>
									<th class="titulo alignCenter">Limite de Saque<br/>h</th>
									<th class="titulo alignCenter">Valor a Recompor<br/>i</th>
									<th class="titulo alignCenter">Valor a Detalhar<br/>j</th>
									<th class="titulo alignCenter">Valor a Desdetalhar<br/>k</th>
									<th class="titulo alignCenter">Repassado em Excesso<br/>l = c - d + i se &lg; 0</th>
									<th class="titulo alignCenter">Valor a Repassar<br/>l = c - d + i se &gt; 0</th>
									<th class="titulo alignCenter">Valor a Repassar Proposto<br/></th>
								</tr>
								<? } else { ?>
								<tr>
									<th class="bold alignLeft" rowspan="2"><?= $consulta->pegarTituloAgrupador() ?></th>
									<th class="titulo alignCenter" rowspan="2">Gestão Executora</th>
									<th class="titulo alignCenter" rowspan="2">Fonte de<br> Recurso</th>
									<th class="titulo alignCenter" colspan="3">Inscritos (Mês 0)</th>
									<th class="titulo alignCenter" colspan="3">Inscritos (Atual)</th>
									<th class="titulo alignCenter">Pagos</th>
									<th class="titulo alignCenter">RAP Atual</th>
									<th class="titulo alignCenter" colspan="3">A Pagar</th>
									<th class="titulo alignCenter" rowspan="2">Valor a <br> Repassar</th>
								</tr>
								<tr>
									<th class="titulo alignCenter">Processado</th>
									<th class="titulo alignCenter">N. Processado</th>
									<th class="titulo alignCenter">Total</th>
									<th class="titulo alignCenter">Processado</th>
									<th class="titulo alignCenter">N. Processado</th>
									<th class="titulo alignCenter">Total a Pagar<br>a</th>
									<th class="titulo alignCenter">Total<br>b</th>
									<th class="titulo alignCenter">Total<br>c = a + b</th>
									<th class="titulo alignCenter">Processado</th>
									<th class="titulo alignCenter">N. Processado</th>
									<th class="titulo alignCenter">Total</th>
								</tr>
								<? } ?>
							</thead>
						</table>
					</th>
				</tr>
			</thead>
			<tbody>
				<tr>
					<td class="noPadding" align="left">
						<div class="div_rolagem">
							<?php
								$cfAgrupadores = $consulta->pegarAgrupadores();
								if ($itens['itens'] != '')	
									cfDesenhaResultado( $itens['itens'], array() );
								else
									print("Não existem dados para o filtro selecionado!");
								
							?>
						</div>
					</td>
				</tr>
			</tbody>
		</table>
		<script type="text/javascript" language="javascript">
			document.getElementById( 'aguarde' ).style.visibility = 'hidden';
			document.getElementById('aguarde').style.display = 'none';
		</script>
	</body>
</html>