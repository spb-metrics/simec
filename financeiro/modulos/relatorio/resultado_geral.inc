<?php
ini_set( "memory_limit", "1024M" ); // ...
set_time_limit(0);

include 'funcoes_consulta_financeiro.inc'; 
$consulta = new ConsultaFinanceiro(); 


// parâmetros gerais
$consulta->alterarTitulo( $_REQUEST['titulo'] );
$consulta->alterarEscala( $_REQUEST['escala'] );
if ( is_array( $_REQUEST['agrupador'] ) == true )
{
	foreach ( $_REQUEST['agrupador'] as $agrupador )
	{
		$consulta->adicionarAgrupador( $agrupador );
	}
}
// FIM parâmetros gerais

// adiciona filtros
function adicionarFiltro( $campo )
{
	global $consulta;
	if ( $_REQUEST[$campo . '_campo_flag'] && $_REQUEST[$campo][0] != '' )
	{
		$excludente = (boolean) $_REQUEST[$campo . '_campo_excludente'];
		$consulta->adicionarFiltro( $campo, $_REQUEST[$campo], $excludente );
	}
}
adicionarFiltro( 'orgao' );
adicionarFiltro( 'grupouo' );
adicionarFiltro( 'uo' );
adicionarFiltro( 'ug' );
adicionarFiltro( 'funcao' );
adicionarFiltro( 'subfuncao' );
adicionarFiltro( 'programa' );
adicionarFiltro( 'acacod' );
adicionarFiltro( 'localizador' );
adicionarFiltro( 'fonte' );
adicionarFiltro( 'fontesiafi' );
adicionarFiltro( 'fontesof' );
adicionarFiltro( 'gnd' );
adicionarFiltro( 'natureza' );
adicionarFiltro( 'usopri' );
adicionarFiltro( 'mapcod' );
adicionarFiltro( 'grf' );
adicionarFiltro( 'iducod' );
adicionarFiltro( 'catecon' );
adicionarFiltro( 'elemento' );
adicionarFiltro( 'esfera' );
$consulta->adicionarFiltro( 'ano', $_REQUEST['ano'] );
// FIM adiciona filtros

// salva os POST na tabela
if ( $_REQUEST['salvar'] )
{
	$existe_rel = 0;
	$sql = sprintf(
		"select prtid from public.parametros_tela where prtdsc = '%s'",
		$_REQUEST['titulo']
	);
	$existe_rel = $db->pegaUm( $sql );
	if ($existe_rel > 0) 
	{
		/*
		?>
		<script language="text/javascript">
			if ( !confirm( 'Deseja alterar a consulta já existente?' ) )
			{
				history.back();
			}
		</script>
		<?
		*/
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
		location.href = '?modulo=<?= $modulo ?>&acao=R';
	</script>
	<?
}
// FIM salva os POST na tabela

// mostra relatório em XLS
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
// FIM mostra relatório em XLS



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
				<? $sqlAcumuladoAte = "select max( rofdata ) as data from financeiro.execucao " . $whereAcumuladoAte; ?>
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
			
			table{width:19cm;border-collapse:collapse;}
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
						$nomeCampoCod = "cast(ctecod as varchar) || cast(gndcod as varchar) || mapcod || edpcod";
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
				mostraFiltro( 'orgao' );
				mostraFiltro( 'grupouo' );
				mostraFiltro( 'uo' );
				//mostraFiltro( 'ug' );
				mostraFiltro( 'funcao' );
				mostraFiltro( 'subfuncao' );
				mostraFiltro( 'programa' );
				mostraFiltro( 'acacod' );
				mostraFiltro( 'esfera' );
				//mostraFiltro( 'localizador' );
				mostraFiltro( 'grf' );
				mostraFiltro( 'fonte' );
				mostraFiltro( 'fontesiafi' );
				mostraFiltro( 'fontesof' );
				mostraFiltro( 'catecon' );
				mostraFiltro( 'gnd' );
				mostraFiltro( 'mapcod' );
				mostraFiltro( 'elemento' );
				mostraFiltro( 'natureza' );
				mostraFiltro( 'iducod' );
				mostraFiltro( 'usopri' );
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
								<tr>
									<th class="bold alignLeft"><?= $consulta->pegarTituloAgrupador() ?></th>
									<th class="titulo alignCenter">PL<br/>(Projeto de Lei)<br/>(A)</th>
									<th class="titulo alignCenter">Lei Após Vetos<br/>(Dot. Inicial)<br/>(B)</th>
									<th class="titulo alignCenter">Créd.Adic. +- Remanejam.<br/>(C)</th>
									<th class="titulo alignCenter">Lei + Créditos<br/>(Autorizado)<br/>(D)=(B)+(C)</th>
									<th class="titulo alignCenter">Empenhado<br/><br/>(E)</th>
									<th class="titulo alignCenter">Liquidado<br/><br/>(F)</th>
									<th class="titulo alignCenter">Valores Pagos<br/><br/>(G)</th>
									<th class="titulo alignCenter">% do Pago s/ Autorizado<br/>(H)=(G)/(D)</th>
								</tr>
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
								//dbg( $itens['itens'] );
								cfDesenhaResultado( $itens['itens'] );
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