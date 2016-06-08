<?php

function cfDesenhaResultado( $itens )
{
	global $cfAgrupadores;
	$soma = array(
		'credito_recebido' => 0,
		'empenhos_emitidos' => 0,
		'empenhos_liquidados' => 0,
		'repasse_recebido' => 0,
		'valores_pagos' => 0,
		'liquidado_pagar' => 0,
		'saldo_financeiro' => 0,
		'limite_saque' => 0,
		'valor_recompor' => 0,
		'valor_detalhar' => 0,
		'valore_desdetalhar' => 0,
		'repassado_excesso' => 0,
		'valor_repassar' => 0,
		'valor_proposto' => 0
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

		$soma['credito_recebido'] += $valores['credito_recebido'];
		$soma['empenhos_emitidos'] += $valores['empenhos_emitidos'];
		$soma['empenhos_liquidados'] += $valores['empenhos_liquidados'];
		$soma['repasse_recebido'] += $valores['repasse_recebido'];
		$soma['valores_pagos'] += $valores['valores_pagos'];
		$soma['liquidado_pagar'] += $valores['liquidado_pagar'];
		$soma['saldo_financeiro'] += $valores['saldo_financeiro'];
		$soma['limite_saque'] += $valores['limite_saque'];
		$soma['valor_recompor'] += $valores['valor_recompor'];
		$soma['valor_detalhar'] += $valores['valor_detalhar'];
		$soma['valore_desdetalhar'] += $valores['valore_desdetalhar'];
		$soma['repassado_excesso'] += $valores['repassado_excesso'];
		$soma['valor_repassar'] += $valores['valor_repassar'];
		$soma['valor_proposto'] += $valores['valor_proposto'];

	}
	cfDesenhaTotal( 0, 'Geral', '', $soma['credito_recebido'], $soma['empenhos_emitidos'], $soma['empenhos_liquidados'], $soma['repasse_recebido'], $soma['valores_pagos'], $soma['liquidado_pagar'], $soma['saldo_financeiro'], $soma['limite_saque'], $soma['valor_recompor'], $soma['valor_detalhar'], $soma['valore_desdetalhar'], $soma['repassado_excesso'], $soma['valor_repassar'], $soma['valor_proposto'] );
}

function cfDesenha( $dados, $nivel, $rastro, $rastroCodigo )
{
	global $cfAgrupadores;
	$soma = array(
		'credito_recebido' => 0,
		'empenhos_emitidos' => 0,
		'empenhos_liquidados' => 0,
		'repasse_recebido' => 0,
		'valores_pagos' => 0,
		'liquidado_pagar' => 0,
		'saldo_financeiro' => 0,
		'limite_saque' => 0,
		'valor_recompor' => 0,
		'valor_detalhar' => 0,
		'valore_desdetalhar' => 0,
		'repassado_excesso' => 0,
		'valor_repassar' => 0,
		'valor_proposto' => 0
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

			$soma['credito_recebido'] += $valores['credito_recebido'];
			$soma['empenhos_emitidos'] += $valores['empenhos_emitidos'];
			$soma['empenhos_liquidados'] += $valores['empenhos_liquidados'];
			$soma['repasse_recebido'] += $valores['repasse_recebido'];
			$soma['valores_pagos'] += $valores['valores_pagos'];
			$soma['liquidado_pagar'] += $valores['liquidado_pagar'];
			$soma['saldo_financeiro'] += $valores['saldo_financeiro'];
			$soma['limite_saque'] += $valores['limite_saque'];
			$soma['valor_recompor'] += $valores['valor_recompor'];
			$soma['valor_detalhar'] += $valores['valor_detalhar'];
			$soma['valore_desdetalhar'] += $valores['valore_desdetalhar'];
			$soma['repassado_excesso'] += $valores['repassado_excesso'];
			$soma['valor_repassar'] += $valores['valor_repassar'];
			$soma['valor_proposto'] += $valores['valor_proposto'];
			
		}
		global $consulta;
		$titulo = $consulta->pegarTituloAgrupador( $nivel );
		cfDesenhaTotal( $nivel, $titulo, $dados['cod'], $soma['credito_recebido'], $soma['empenhos_emitidos'], $soma['empenhos_liquidados'], $soma['repasse_recebido'], $soma['valores_pagos'], $soma['liquidado_pagar'], $soma['saldo_financeiro'], $soma['limite_saque'], $soma['valor_recompor'], $soma['valor_detalhar'], $soma['valore_desdetalhar'], $soma['repassado_excesso'], $soma['valor_repassar'], $soma['valor_proposto'] );
	}
	else
	{
		cfDesenhaItem( $dados, $nivel, $rastro, $rastroCodigo );

		$soma['credito_recebido'] += $dados['credito_recebido'];
		$soma['empenhos_emitidos'] += $dados['empenhos_emitidos'];
		$soma['empenhos_liquidados'] += $dados['empenhos_liquidados'];
		$soma['repasse_recebido'] += $dados['repasse_recebido'];
		$soma['valores_pagos'] += $dados['valores_pagos'];
		$soma['liquidado_pagar'] += $dados['liquidado_pagar'];
		$soma['saldo_financeiro'] += $dados['saldo_financeiro'];
		$soma['limite_saque'] += $dados['limite_saque'];
		$soma['valor_recompor'] += $dados['valor_recompor'];
		$soma['valor_detalhar'] += $dados['valor_detalhar'];
		$soma['valore_desdetalhar'] += $dados['valore_desdetalhar'];
		$soma['repassado_excesso'] += $dados['repassado_excesso'];
		$soma['valor_repassar'] += $dados['valor_repassar'];
		$soma['valor_proposto'] += $dados['valor_proposto'];
	
	}
	return $soma;
}

function cfDesenhaTotal( $nivel, $titulo, $cod, $credito_recebido, $empenhos_emitidos, $empenhos_liquidados, $repasse_recebido, $valores_pagos, $liquidado_pagar, $saldo_financeiro, $limite_saque, $valor_recompor, $valor_detalhar, $valore_desdetalhar, $repassado_excesso, $valor_repassar, $valor_proposto )
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

			<td class="alignRight titulo bold"><? cfDesenhaValor( $credito_recebido ); ?></td>
			<td class="alignRight titulo bold"><? cfDesenhaValor( $empenhos_emitidos ); ?></td>
			<td class="alignRight titulo bold"><? cfDesenhaValor( $empenhos_liquidados ); ?></td>
			<td class="alignRight titulo bold"><? cfDesenhaValor( $repasse_recebido ); ?></td>
			<td class="alignRight titulo bold"><? cfDesenhaValor( $valores_pagos ); ?></td>
			<td class="alignRight titulo bold"><? cfDesenhaValor( $liquidado_pagar ); ?></td>
			<td class="alignRight titulo bold"><? cfDesenhaValor( $saldo_financeiro ); ?></td>
			<td class="alignRight titulo bold"><? cfDesenhaValor( $limite_saque ); ?></td>
			<td class="alignRight titulo bold"><? cfDesenhaValor( $valor_recompor ); ?></td>
			<td class="alignRight titulo bold"><? cfDesenhaValor( $valor_detalhar ); ?></td>
			<td class="alignRight titulo bold"><? cfDesenhaValor( $valore_desdetalhar ); ?></td>
			<td class="alignRight titulo bold"><? cfDesenhaValor( $repassado_excesso ); ?></td>
			<td class="alignRight titulo bold"><? cfDesenhaValor( $valor_repassar ); ?></td>
			<td class="alignRight titulo bold"><? cfDesenhaValor( $valor_proposto ); ?></td>
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

			<td class="alignRight titulo"><? cfDesenhaValor( $item['credito_recebido'] ); ?></td>
			<td class="alignRight titulo"><? cfDesenhaValor( $item['empenhos_emitidos'] ); ?></td>
			<td class="alignRight titulo"><? cfDesenhaValor( $item['empenhos_liquidados'] ); ?></td>
			<td class="alignRight titulo"><? cfDesenhaValor( $item['repasse_recebido'] ); ?></td>
			<td class="alignRight titulo"><? cfDesenhaValor( $item['valores_pagos'] ); ?></td>
			<td class="alignRight titulo"><? cfDesenhaValor( $item['liquidado_pagar'] ); ?></td>
			<td class="alignRight titulo"><? cfDesenhaValor( $item['saldo_financeiro'] ); ?></td>
			<td class="alignRight titulo"><? cfDesenhaValor( $item['limite_saque'] ); ?></td>
			<td class="alignRight titulo"><? cfDesenhaValor( $item['valor_recompor'] ); ?></td>
			<td class="alignRight titulo"><? cfDesenhaValor( $item['valor_detalhar'] ); ?></td>
			<td class="alignRight titulo"><? cfDesenhaValor( $item['valore_desdetalhar'] ); ?></td>
			<td class="alignRight titulo"><? cfDesenhaValor( $item['repassado_excesso'] ); ?></td>
			<td class="alignRight titulo"><? cfDesenhaValor( $item['valor_repassar'] ); ?></td>
			<td class="alignRight titulo"><? cfDesenhaValor( $item['valor_proposto'] ); ?></td>
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
		'credito_recebido' => $itens['credito_recebido'],
		'empenhos_emitidos' => $itens['empenhos_emitidos'],
		'empenhos_liquidados' => $itens['empenhos_liquidados'],
		'repasse_recebido' => $itens['repasse_recebido'],
		'valores_pagos' => $itens['valores_pagos'],
		'liquidado_pagar' => $itens['liquidado_pagar'],
		'saldo_financeiro' => $itens['saldo_financeiro'],
		'limite_saque' => $itens['limite_saque'],
		'valor_recompor' => $itens['valor_recompor'],
		'valor_detalhar' => $itens['valor_detalhar'],
		'valore_desdetalhar' => $itens['valore_desdetalhar'],
		'repassado_excesso' => $itens['repassado_excesso'],
		'valor_repassar' => $itens['valor_repassar'],
		'valor_proposto' => $itens['valor_proposto']
		);
	}
	$soma = array(
		'credito_recebido' => 0,
		'empenhos_emitidos' => 0,
		'empenhos_liquidados' => 0,
		'repasse_recebido' => 0,
		'valores_pagos' => 0,
		'liquidado_pagar' => 0,
		'saldo_financeiro' => 0,
		'limite_saque' => 0,
		'valor_recompor' => 0,
		'valor_detalhar' => 0,
		'valore_desdetalhar' => 0,
		'repassado_excesso' => 0,
		'valor_repassar' => 0,
		'valor_proposto' => 0
	);
	foreach ( $itens['itens'] as $item )
	{
		$valores = cfCalcularValorTotal( $item );
		$soma['credito_recebido'] += $valores['credito_recebido'];
		$soma['empenhos_emitidos'] += $valores['empenhos_emitidos'];
		$soma['empenhos_liquidados'] += $valores['empenhos_liquidados'];
		$soma['repasse_recebido'] += $valores['repasse_recebido'];
		$soma['valores_pagos'] += $valores['valores_pagos'];
		$soma['liquidado_pagar'] += $valores['liquidado_pagar'];
		$soma['saldo_financeiro'] += $valores['saldo_financeiro'];
		$soma['limite_saque'] += $valores['limite_saque'];
		$soma['valor_recompor'] += $valores['valor_recompor'];
		$soma['valor_detalhar'] += $valores['valor_detalhar'];
		$soma['valore_desdetalhar'] += $valores['valore_desdetalhar'];
		$soma['repassado_excesso'] += $valores['repassado_excesso'];
		$soma['valor_repassar'] += $valores['valor_repassar'];
		$soma['valor_proposto'] += $valores['valor_proposto'];
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