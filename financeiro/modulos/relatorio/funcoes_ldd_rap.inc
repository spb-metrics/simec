<?php

function cfDesenhaResultado( $itens )
{
	global $cfAgrupadores;
	$soma = array(
		'processado_mes0' => 0,
		'nao_processado_mes0' => 0,
		'total_mes0' => 0,
		'processado_atual' => 0,
		'nao_processado_atual' => 0,
		'total_atual' => 0,
		'pago' => 0,
		'atual_total' => 0,
		'processado_apagar' => 0,
		'nao_processado_apagar' => 0,
		'total_apagar' => 0,
		'valora_repassar' => 0
	);
	
	if ( count( $itens ) < 1 )
	{
		print '<br/><br/><p style="color: #ff2020;">Nenhum resultado para os parâmetros indicados.</p>';
		return;
	}
	
	
	foreach ( $itens as $item )
	{
		$rastro = array( $item['cod'] => $item['dsc'] );
		$rastroCodigo = array();
		$rastroCodigo[$cfAgrupadores[0]] = $item['cod'];
		//dbg( $rastro, '00677864132' );
		$valores = cfDesenha( $item, 0, $rastro, $rastroCodigo );

		$soma['processado_mes0'] += $valores['processado_mes0'];
		$soma['nao_processado_mes0'] += $valores['nao_processado_mes0'];
		$soma['total_mes0'] += $valores['total_mes0'];
		$soma['processado_atual'] += $valores['processado_atual'];
		$soma['nao_processado_atual'] += $valores['nao_processado_atual'];
		$soma['total_atual'] += $valores['total_atual'];
		$soma['pago'] += $valores['pago'];
		$soma['atual_total'] += $valores['atual_total'];
		$soma['processado_apagar'] += $valores['processado_apagar'];
		$soma['nao_processado_apagar'] += $valores['nao_processado_apagar'];
		$soma['total_apagar'] += $valores['total_apagar'];
		$soma['valora_repassar'] += $valores['valora_repassar'];
		
	}

	cfDesenhaTotal( 0, 'Geral', '', $soma['processado_mes0'], $soma['nao_processado_mes0'], $soma['total_mes0'], $soma['processado_atual'], $soma['nao_processado_atual'], $soma['total_atual'], $soma['pago'], $soma['atual_total'], $soma['processado_apagar'], $soma['nao_processado_apagar'], $soma['total_apagar'], $soma['valora_repassar'] );

}

function cfDesenha( $dados, $nivel, $rastro, $rastroCodigo )
{
	global $cfAgrupadores;
	$soma = array(
		'processado_mes0' => 0,
		'nao_processado_mes0' => 0,
		'total_mes0' => 0,
		'processado_atual' => 0,
		'nao_processado_atual' => 0,
		'total_atual' => 0,
		'pago' => 0,
		'atual_total' => 0,
		'processado_apagar' => 0,
		'nao_processado_apagar' => 0,
		'total_apagar' => 0,
		'valora_repassar' => 0
	);
	
	$rastro[$dados['cod']] = $dados['dsc'];
	if ( count( $dados ) == 3 )
	{
		cfDesenhaAgrupador( $dados['cod'], $dados['dsc'], $nivel, $rastro, $rastroCodigo );
		foreach ( $dados['itens'] as $item )
		{
			$subRastroCodigo = $rastroCodigo;
			$subRastroCodigo[$cfAgrupadores[$nivel+1]] = $item['cod'];
			$rastroItem = $rastro;
			$rastroItem[$item['cod']] = $item['dsc'];
			$valores = cfDesenha( $item, $nivel + 1, $rastroItem, $subRastroCodigo );

			$soma['processado_mes0'] += $valores['processado_mes0'];
			$soma['nao_processado_mes0'] += $valores['nao_processado_mes0'];
			$soma['total_mes0'] += $valores['total_mes0'];
			$soma['processado_atual'] += $valores['processado_atual'];
			$soma['nao_processado_atual'] += $valores['nao_processado_atual'];
			$soma['total_atual'] += $valores['total_atual'];
			$soma['pago'] += $valores['pago'];
			$soma['atual_total'] += $valores['atual_total'];
			$soma['processado_apagar'] += $valores['processado_apagar'];
			$soma['nao_processado_apagar'] += $valores['nao_processado_apagar'];
			$soma['total_apagar'] += $valores['total_apagar'];
			$soma['valora_repassar'] += $valores['valora_repassar'];
		
		}

		global $consulta;
		$titulo = $consulta->pegarTituloAgrupador( $nivel );
		cfDesenhaTotal( $nivel, $titulo, $dados['cod'], $soma['processado_mes0'], $soma['nao_processado_mes0'], $soma['total_mes0'], $soma['processado_atual'], $soma['nao_processado_atual'], $soma['total_atual'], $soma['pago'], $soma['atual_total'], $soma['processado_apagar'], $soma['nao_processado_apagar'], $soma['total_apagar'], $soma['valora_repassar'] );

	
	}
	else
	{
		cfDesenhaItem( $dados, $nivel, $rastro, $rastroCodigo );

		$soma['processado_mes0'] += $dados['processado_mes0'];
		$soma['nao_processado_mes0'] += $dados['nao_processado_mes0'];
		$soma['total_mes0'] += $dados['total_mes0'];
		$soma['processado_atual'] += $dados['processado_atual'];
		$soma['nao_processado_atual'] += $dados['nao_processado_atual'];
		$soma['total_atual'] += $dados['total_atual'];
		$soma['pago'] += $dados['pago'];
		$soma['atual_total'] += $dados['atual_total'];
		$soma['processado_apagar'] += $dados['processado_apagar'];
		$soma['nao_processado_apagar'] += $dados['nao_processado_apagar'];
		$soma['total_apagar'] += $dados['total_apagar'];
		$soma['valora_repassar'] += $dados['valora_repassar'];
		
	}
	return $soma;
}

function cfDesenhaTotal( $nivel, $titulo, $cod, $processado_mes0, $nao_processado_mes0, $total_mes0, $processado_atual, $nao_processado_atual, $total_atual, $pago, $atual_total, $processado_apagar, $nao_processado_apagar, $total_apagar, $valora_repassar )
{
	$autorizado = $dotacaoInicial + $creditoAdicional;
	$porcentagemAutorizado = $autorizado ? ( ( $pago * 100 ) / $autorizado ) : 0 ;
	$cod = $cod != '' ? ' ( ' . $cod . ' ) ' : '';
	?>
	<table class="tabelaDados" style="border-bottom:1px solid black; background-color: #f0f0f0;">
		<tr>
			<td class="alignLeft bold titulolinha" style="padding: 0 0 0 <?= $nivel * 10 ?>px;">
				Total <?= $titulo . $cod ?>
			</td>
			<td class="alignRight titulo">&nbsp;</td>
			<td class="alignRight titulo">&nbsp;</td>

			<td class="alignRight titulo bold"><? cfDesenhaValor( $processado_mes0 ); ?></td>
			<td class="alignRight titulo bold"><? cfDesenhaValor( $nao_processado_mes0 ); ?></td>
			<td class="alignRight titulo bold"><? cfDesenhaValor( $total_mes0 ); ?></td>
			<td class="alignRight titulo bold"><? cfDesenhaValor( $processado_atual ); ?></td>
			<td class="alignRight titulo bold"><? cfDesenhaValor( $nao_processado_atual ); ?></td>
			<td class="alignRight titulo bold"><? cfDesenhaValor( $total_atual ); ?></td>
			<td class="alignRight titulo bold"><? cfDesenhaValor( $pago ); ?></td>
			<td class="alignRight titulo bold"><? cfDesenhaValor( $atual_total ); ?></td>
			<td class="alignRight titulo bold"><? cfDesenhaValor( $processado_apagar ); ?></td>
			<td class="alignRight titulo bold"><? cfDesenhaValor( $nao_processado_apagar ); ?></td>
			<td class="alignRight titulo bold"><? cfDesenhaValor( $total_apagar ); ?></td>
			<td class="alignRight titulo bold"><? cfDesenhaValor( $valora_repassar ); ?></td>
		</tr>
	</table>
	<?
}

function cfDesenhaAgrupador( $codigo, $descricao, $nivel, $rastro, $rastroCodigo )
{
	$seta = $nivel > 0 ? '&rsaquo; ' : '' ;
	?>
	<table class="tabelaDados" style="margin-bottom: 2px; background-color: #f0f0f0;">
		<tr>
			<td class="tituloagrup bold alignLeft" style="padding: 0 0 0 <?= $nivel * 10 ?>px;">
				<?= $seta ?>
				<? cfDesenhaCodigo( $nivel, $codigo, $rastroCodigo ); ?>
				<a href="<? cfMontarLinkGrafico( $rastro ); ?>" class="tituloagrup bold"><?= $descricao ?></a>
			</td>
		</tr>
	</table>
	<?
}

function cfDesenhaCodigo( $nivel, $codigo, $rastroCodigo )
{
	global $cfAgrupadores;
	switch ( $cfAgrupadores[$nivel] )
	{
		case 'acacod':
			//dbg( $rastroCodigo );
			$unicod = trim( $rastroCodigo['uo'] );
			$acacod = trim( $rastroCodigo['acacod'] );
			$loccod = explode( '.', $rastroCodigo['localizador'] );
			$loccod = $loccod[3];
			$saida = sprintf(
				'<a href="#" onclick="window.open( \'%s\', \'Ação\', \'scrollbars=yes,top=50,left=200\' )">%s</a>',
				"http://simec-d/monitora/monitora.php?modulo=relatorio/acao/relatorio_evolucao&acao=C&acacod=$acacod&unicod=$unicod&loccod=$loccod",
				$codigo
			);
			break;
		case 'localizador':
			$a = explode( '.', $codigo );
			$acao = $a[1];
			$programa = $a[0];
			$saida = sprintf(
				'<a href="#" onclick="window.open( \'%s\', \'Localizador\', \'scrollbars=yes,top=50,left=200\' )">%s</a>',
				"http://simec-d/monitora/monitora.php?modulo=principal/acao/monitoraacao&acao=A&refcod=x&acaid=$acao&prgid=$programa",
				$codigo
			);
			break;
		default:
			$saida = $codigo;
			break;
	}
	//var_dump( $rastroCodigo );
	print $saida;
}

function cfDesenhaItem( $item, $nivel, $rastro, $rastroCodigo )
{
	static $cor = '';
	$cor = $cor == '' ? '#f8f8f8' : '';
	$seta = $nivel > 0 ? '&rsaquo; ' : '' ;
	?>
	<table class="tabelaDados">
		<tr bgcolor="<?= $cor ?>" onmouseover="this.style.backgroundColor = '#ffffcc';" onmouseout="this.style.backgroundColor = '<?= $cor ?>';">
			<td class="alignLeft titulolinha" style="padding: 0 0 0 <?= $nivel * 10 ?>px;">
				<?= $seta ?>
				<? cfDesenhaCodigo( $nivel, $item['cod'], $rastroCodigo ); ?>
				<a href="<? cfMontarLinkGrafico( $rastro ); ?>" class="alignLeft titulolinha"><?= $item['dsc'] ?></a>
			</td>
			<td class="alignRight titulo"><?=$item['gstcod']?></td>
			<td class="alignRight titulo"><?=$item['frscod']?></td>

			<td class="alignRight titulo"><? cfDesenhaValor( $item['processado_mes0'] ); ?></td>
			<td class="alignRight titulo"><? cfDesenhaValor( $item['nao_processado_mes0'] ); ?></td>
			<td class="alignRight titulo"><? cfDesenhaValor( $item['total_mes0'] ); ?></td>
			<td class="alignRight titulo"><? cfDesenhaValor( $item['processado_atual'] ); ?></td>
			<td class="alignRight titulo"><? cfDesenhaValor( $item['nao_processado_atual'] ); ?></td>
			<td class="alignRight titulo"><? cfDesenhaValor( $item['total_atual'] ); ?></td>
			<td class="alignRight titulo"><? cfDesenhaValor( $item['pago'] ); ?></td>
			<td class="alignRight titulo"><? cfDesenhaValor( $item['atual_total'] ); ?></td>
			<td class="alignRight titulo"><? cfDesenhaValor( $item['processado_apagar'] ); ?></td>
			<td class="alignRight titulo"><? cfDesenhaValor( $item['nao_processado_apagar'] ); ?></td>
			<td class="alignRight titulo"><? cfDesenhaValor( $item['total_apagar'] ); ?></td>
			<td class="alignRight titulo"><? cfDesenhaValor( $item['valora_repassar'] ); ?></td>
		</tr>
	</table>
	<?
}

function cfDesenhaValor( $valor )
{
	$valor = number_format( $valor, 0, ',', '.' );
	print $valor == '-0' ? '-' : $valor ;
}

function cfMontarParametroRastroGrafico( $rastro )
{
	$parametro = '';
	foreach ( $rastro as $cod => $dsc )
	{
		$parametro .= "&rastro[" . $cod . "]=" . urlencode( $dsc );
	}
	$parametro = substr( $parametro, 1 );
	return $parametro;
}

function cfMontarLinkGrafico( $rastro )
{
	print "javascript:mostrarGrafico( '" . cfMontarParametroRastroGrafico( $rastro ) . "' );";
}

// a função nativa do php zera as chaves numéricas
function cfArrayShift( &$array )
{
	reset( $array );
	unset( $array[key($array)] );
}

$cfRastroBusca = array();
function cfBuscarItem( $itens, $rastro )
{
	global $cfRastroBusca;
	// captura o primeiro elemento do array e seus dados
	reset( $rastro );
	$codAtual = key( $rastro );
	$dscAtual = current( $rastro );	
	foreach ( $itens as $item )
	{
		if ( $item['cod'] == $codAtual && $item['dsc'] == $dscAtual )
		{
			$cfRastroBusca[$item['cod']] = $item['dsc'];
			if ( count( $rastro ) > 1 )
			{
				cfArrayShift( $rastro );
				return cfBuscarItem( $item['itens'], $rastro );
			}
			return $item;
		}
	}
	return null;
}

function cfCalcularValorTotal( $itens )
{
	if ( array_key_exists( 'itens', $itens ) == false )
	{
		return array(
		'processado_mes0' => $itens['processado_mes0'],
		'nao_processado_mes0' => $itens['nao_processado_mes0'],
		'total_mes0' => $itens['total_mes0'],
		'processado_atual' => $itens['processado_atual'],
		'nao_processado_atual' => $itens['nao_processado_atual'],
		'total_atual' => $itens['total_atual'],
		'pago' => $itens['pago'],
		'atual_total' => $itens['atual_total'],
		'processado_apagar' => $itens['processado_apagar'],
		'nao_processado_apagar' => $itens['nao_processado_apagar'],
		'total_apagar' => $itens['total_apagar'],
		'valora_repassar' => $itens['valora_repassar']
		);
	}
	$soma = array(
		'processado_mes0' => 0,
		'nao_processado_mes0' => 0,
		'total_mes0' => 0,
		'processado_atual' => 0,
		'nao_processado_atual' => 0,
		'total_atual' => 0,
		'pago' => 0,
		'atual_total' => 0,
		'processado_apagar' => 0,
		'nao_processado_apagar' => 0,
		'total_apagar' => 0,
		'valora_repassar' => 0
	);
	foreach ( $itens['itens'] as $item )
	{
	
		$valores = cfCalcularValorTotal( $item );
		$soma['processado_mes0'] += $valores['processado_mes0'];
		$soma['nao_processado_mes0'] += $valores['nao_processado_mes0'];
		$soma['total_mes0'] += $valores['total_mes0'];
		$soma['processado_atual'] += $valores['processado_atual'];
		$soma['nao_processado_atual'] += $valores['nao_processado_atual'];
		$soma['total_atual'] += $valores['total_atual'];
		$soma['pago'] += $valores['pago'];
		$soma['atual_total'] += $valores['atual_total'];
		$soma['processado_apagar'] += $valores['processado_apagar'];
		$soma['nao_processado_apagar'] += $valores['nao_processado_apagar'];
		$soma['total_apagar'] += $valores['total_apagar'];
		$soma['valora_repassar'] += $valores['valora_repassar'];
	
	}
	return $soma;
}

function cfCalculaValorAgrupado( $itens )
{
	if ( array_key_exists( 'itens', $itens ) == false )
	{
		return array( $itens['cod'] => cfCalcularValorTotal( $itens ) );
	}
	$soma = array();
	foreach ( $itens['itens'] as $item )
	{
		$valores = cfCalcularValorTotal( $item );
		$valores['cod'] = $item['cod'];
		$valores['dsc'] = $item['dsc'];
		// o código não é utilizado como chave, pois há casos em que o cod se repete (exemplo: RAP)
		array_push( $soma, $valores );
	}
	return $soma;
}

$cfAgrupadores = array();

?>