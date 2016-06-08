<?php
// VERIFICA SE PROJETO ESTÁ SELECIONADO
projeto_verifica_selecionado( $_REQUEST["atiidraiz"] );

// CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
montar_titulo_projeto();

?>
<style>
			body { font-size: 6pt; }
			.span_detalhamento { color: #aa5500; }
			.span_responsavel { color: #606060; }
			.span_datafim_ultrapassada { color: #dd2020; font-weight: bold; }
			.coluna_desativada { color: #909090; border-top: 1px solid #ababab; border-left: 1px solid #ababab; }
			.coluna_atidatainicio, .coluna_atidatafim { color: #404040; text-align: center; border-top: 1px solid #ababab; border-left: 1px solid #ababab; width: 80px; }
			.coluna_atimeta { color: #008000; text-align: center; border-top: 1px solid #ababab; border-left: 1px solid #ababab; width: 200px; }
			.coluna_atiinterface { color: #2277bb; text-align: center; border-top: 1px solid #ababab; border-left: 1px solid #ababab; width: 200px; }
			.coluna_atiporcentoexec { color: #aa5500; text-align: center; border-top: 1px solid #ababab; border-left: 1px solid #ababab; width: 30px; }
			.coluna_esadescricao { color: #ff0066; text-align: center; border-top: 1px solid #ababab; border-left: 1px solid #ababab; width: 30px; }
			.coluna_atidescricao { color: #000000; text-align: left; border-top: 1px solid #ababab; border-left: 1px solid #ababab; }
			.coluna_orcamento { color: #0000dd; text-align: right; border-top: 1px solid #ababab; border-left: 1px solid #ababab; width: 50px; }
		</style>

					
					
<?php

function capturar_orcamento( $atiid, $ano_inicio, $ano_fim, $agrupar )
{
	global $db;
	$sql = sprintf(
		"
		select
			orcano,
			orcvalor
		from projetos.orcamentoatividade
		where
			orcano >= '%d' and
			orcano <= '%d' and
			atiid = %d
		",
		$ano_inicio,
		$ano_fim,
		$atiid
	);
	$linhas = $db->carregar( $sql );
	$linhas = $linhas ? $linhas : array();
	$orcamento = array();
	foreach ( $linhas as $linha )
	{
		$orcamento[( (integer) $linha['orcano'])] = $linha['orcvalor'];
	}
	if ( $agrupar )
	{
		return array( array_sum( $orcamento ) );
	}
	for ( $ano = $ano_inicio; $ano <= $ano_fim; $ano++ )
	{
		if ( !array_key_exists( $ano, $orcamento ) )
		{
			$orcamento[$ano] = 0;
		}
	}
	ksort( $orcamento );
	return $orcamento;
}

function capturar_arquivos( $atiid )
{
	global $db;
	$atiid = (integer) $atiid;
	$sql = sprintf(
		"select
			v.verid,
			v.vernome,
			v.verdata,
			u.usunome
		from projetos.anexoatividade a
			inner join projetos.versaoanexoatividade v on
				a.verid = v.verid
			inner join seguranca.usuario u on
				u.usucpf = v.usucpf
		where
			anestatus = 'A' and
			atiid = %d
		order by
			verdata desc",
		$atiid
	);
	$arquivos = $db->carregar( $sql );
	$arquivos = $arquivos ? $arquivos : array();
	return $arquivos;
}

// ----- captura dados do request
$_REQUEST['atiidraiz'] = "";
$_REQUEST['niveisvisiveis'] = "-1";
$_REQUEST['submetido'] = "1";
$_REQUEST['colunas'] = array("data","conclusao","estado","rofautorizado","rofempenhado","rofliquidado","rofpago");
$_REQUEST['execucao'] = "-1";
$_REQUEST['restricao'] = "";


$orcamento_inicio  = (integer) $_REQUEST['orcamento_inicio'];
$orcamento_fim     = (integer) $_REQUEST['orcamento_fim'];
$orcamento_agrupar = (boolean) $_REQUEST['orcamento_agrupar'];
$responsabilidade  = (boolean) $_REQUEST['responsabilidade'];
$atiidraiz         = (integer) $_REQUEST['atiidraiz'];
$execucao          = (integer) $_REQUEST['execucao'];
$situacao          = (integer) $_REQUEST['situacao'];
$inicio0           = (string) $_REQUEST['data_inicio_0'];
$inicio1           = (string) $_REQUEST['data_inicio_1'];
$termino0          = (string) $_REQUEST['data_termino_0'];
$termino1          = (string) $_REQUEST['data_termino_1'];
$niveisvisiveis    = (integer) $_REQUEST['niveisvisiveis'];
$restricao         = null;
$colunas           = array();
$projeto           = PROJETO;
if ( $_REQUEST['restricao'] !== '' )
{
	$restricao = $_REQUEST['restricao'] === '1' ? true : false;
}
if ( is_array( $_REQUEST['colunas'] ) )
{
	$colunas = array_merge( $colunas, $_REQUEST['colunas'] );
}
if ( $orcamento_fim < $orcamento_inicio )
{
	$orcamento_fim = $orcamento_inicio;
}

array_unshift( $colunas, 'descricao' );

// ----- monta parametros para a requisição

$campos = array();
$join   = array();
$where  = array();
array_push( $where, ' da.projeto = ' . $projeto );
array_push( $where, ' a.atiidpai is not null ' );

$where_data = array();
if ( $inicio0 && $inicio1 )
{
	array_push(
		$where_data,
		" ( a.atidatainicio >= '" . formata_data_sql( $inicio0 ) . "' and a.atidatainicio <= '" . formata_data_sql( $inicio1 ) . "' ) "
	);
}
if ( $termino0 && $termino1 )
{
	array_push(
		$where_data,
		"
		(
			(
				a.esaid != " . STATUS_CONCLUIDO . " and
				a.atidatafim >= '" . formata_data_sql( $termino0 ) . "' and
				a.atidatafim <= '" . formata_data_sql( $termino1 ) . "'
			)
			or
			(
				a.esaid = " . STATUS_CONCLUIDO . " and
				a.atidataconclusao >= '" . formata_data_sql( $termino0 ) . "' and
				a.atidataconclusao <= '" . formata_data_sql( $termino1 ) . "'
			)
		)
		"
	);
}
if ( count( $where_data ) )
{
	array_push( $where, " ( " . implode( " or ", $where_data ) . " ) " );
}

if ( $execucao > -1 )
{
	array_push( $where, " a.atiporcentoexec = " . $execucao . " " );
}
if ( $situacao )
{
	array_push( $where, " a.esaid = " . $situacao . " " );
}
if ( $restricao !== null )
{
	array_push( $join,  ' inner join projetos.observacaoatividade o on o.atiid = a.atiid ' );
	array_push( $where, ' o.obssolucao = ' . ( $restricao ? 'true' : 'false' ) . ' ' );
}
if ( $atiidraiz )
{
	$sql = "select numero, profundidade from projetos.f_dadosatividade( " . $atiidraiz . " )";
	$dados_raiz = $db->recuperar( $sql );
	$numero_raiz = $dados_raiz['numero'];
	array_push( $where, " ( da.numero like '" . $numero_raiz . ".%' or da.numero = '" . $numero_raiz . "' ) " );
	// profundidade é relativa ao item raiz
	if ( $niveisvisiveis > -1 )
	{
		$niveisvisiveis += $dados_raiz['profundidade'];
	}
}
if ( $niveisvisiveis > -1 )
{
	array_push( $where, " a._atiprofundidade <= " . $niveisvisiveis . " " );
}
if ( in_array( 'descricao', $colunas ) )
{
	$campo = "coalesce( a.atidescricao, '' )";
	if ( in_array( 'detalhamento', $colunas ) )
	{
		$campo .= " || '<br/><span class=\"span_detalhamento\">'  || coalesce( a.atidetalhamento, '' ) || '</span>'";
	}
	if ( in_array( 'responsavel', $colunas ) )
	{
		// jeito antigo de pegar o gerente
		//array_push( $join, 'left join projetos.usuarioresponsabilidade ur on ur.atiid = a.atiid' );
		//array_push( $join, 'left join seguranca.usuario u on u.usucpf = ur.usucpf and u.usustatus = \'A\' ' );
		
		// jeito novo de se verificar quem é o gerente
		array_push(
			$join,
				"
				left join projetos.usuarioresponsabilidade ur on
					ur.atiid = a.atiid and
					ur.rpustatus = 'A' and
					ur.pflcod = " . PERFIL_GERENTE . "
				"
		);
		array_push(
			$join,
				"
				left join seguranca.perfilusuario pu on
					pu.pflcod = ur.pflcod and
					pu.usucpf = ur.usucpf
				"
		);
		array_push(
			$join,
				"
				left join seguranca.usuario u on
					u.usucpf = pu.usucpf and
					u.suscod = 'A'
				"
		);
		
		array_push( $join, 'left join public.unidade uni on uni.unicod = u.unicod and uni.unitpocod = \'U\' and uni.unistatus = \'A\'' );
		array_push( $join, 'left join public.unidadegestora ug on ug.ungcod = u.ungcod and ug.ungstatus = \'A\'' );
		array_push( $campos, 'u.usucpf' );
		array_push( $campos, 'u.usunome' );
		array_push( $campos, 'u.usufoneddd' );
		array_push( $campos, 'u.usufonenum' );
		array_push( $campos, 'uni.unidsc' );
		array_push( $campos, 'ug.ungdsc' );
		$campo .=
			" || '<br/><span class=\"span_responsavel\">' " .
			" || coalesce( u.usunome, '' ) " .
			" || ' - ' " .
			" || coalesce( ug.ungdsc, '' ) " .
			" || ' - ' " .
			" || coalesce( uni.unidsc, '' ) " .
			" || ' - ' " .
			" || '(' || coalesce( u.usufoneddd, '' ) || ')' " .
			" || ' ' " .
			" || coalesce( u.usufonenum, '' ) " .
			" || '</span>'";
	}
	$campo .= " as atidescricao";
	array_push( $campos, $campo );
}
else if ( in_array( 'detalhamento', $colunas ) )
{
	array_push( $campos, "a.atidetalhamento as atidescricao" );
}
if ( in_array( 'meta', $colunas ) )
{
	array_push( $campos, 'a.atimeta' );
}
if ( in_array( 'interface', $colunas ) )
{
	array_push( $campos, 'a.atiinterface' );
}
if ( in_array( 'conclusao', $colunas ) )
{
	array_push( $campos, 'a.atiporcentoexec' );
}
if ( in_array( 'data', $colunas ) )
{
	array_push( $campos, 'a.atidatainicio' );
	array_push( $campos, 'a.atidatafim' );
	array_push( $campos, 'a.atiporcentoexec' );
}
if ( in_array( 'estado', $colunas ) )
{
	array_push( $join,   'left join projetos.estadoatividade ea on ea.esaid = a.esaid' );
	array_push( $campos, 'ea.esadescricao' );
	array_push( $campos, 'a.esaid' );
	array_push( $campos, 'a.atidataconclusao' );
}

$campos = array_unique( $campos );
if ( $responsabilidade || $_REQUEST['responsavel'] )
{
	$sql_atiid_resp_base = <<<EOT
		select
			da2.atiid
		from projetos.usuarioresponsabilidade ur
			inner join projetos.f_dadostodasatividades() as da1 on
				da1.atiid = ur.atiid
			inner join projetos.f_dadostodasatividades() as da2 on
				da2.atiid = da1.atiid or
				da2.numero like da1.numero || '.%%'
		where
			ur.rpustatus = 'A' and
			ur.usucpf = '%s' and
			da2.projeto = {$projeto}
EOT;
	if ( $_REQUEST['responsavel'] )
	{
		$sql = sprintf(
			$sql_atiid_resp_base,
			$_REQUEST['responsavel']
		);
		$linhas = $db->carregar( $sql );
		$linhas = $linhas ? $linhas : array();
		$atiid_resp = array();
		foreach ( $linhas as $linha )
		{
			array_push( $atiid_resp, $linha['atiid'] );
		}
		$atiid_resp = count( $atiid_resp ) ? implode( ', ', $atiid_resp ) : 'null';
		array_push( $where, 'da.atiid in ( ' . $atiid_resp . ' )' );
	}
	if ( $responsabilidade )
	{
		$sql = sprintf(
			$sql_atiid_resp_base,
			$_SESSION['usucpf']
		);
		$linhas = $db->carregar( $sql );
		$linhas = $linhas ? $linhas : array();
		$atiid_resp = array();
		foreach ( $linhas as $linha )
		{
			array_push( $atiid_resp, $linha['atiid'] );
		}
		$atiid_resp = count( $atiid_resp ) ? implode( ', ', $atiid_resp ) : 'null';
		array_push( $where, 'da.atiid in ( ' . $atiid_resp . ' )' );
	}
}



//estratégicas
$estrategica =true;
if ( $estrategica )
{
	array_push( $join,  ' inner join projetos.atividadegrupoatividade aga on aga.atiid = a.atiid ' );
	array_push( $where, ' aga.graid = 2 ' );
}




// ----- monta requisição

if ( count( $campos ) )
{
	$campos = ',' . implode( ',', $campos );
}
else
{
	$campos = '';
}
$join = implode( ' ', $join );
if ( count( $where ) )
{
	$where = 'where ' . implode( ' and ', $where );
}
else
{
	$where = '';
}
$sql = <<<EOT
	select
		da.numero,
		da.profundidade,
		da.atiid,
		max(rofdata) as rofdata,
		sum( coalesce( rofautorizado, 0 ) ) / 1 as rofautorizado,
		sum( coalesce( rofempenhado, 0 ) ) / 1 as rofempenhado,
		sum( coalesce( rofliquidado_favorecido, 0 ) ) / 1 as rofliquidado,
		sum( coalesce( rofpago, 0 ) ) / 1 as rofpago
		$campos
	from projetos.f_dadostodasatividades() da
		 inner join projetos.atividade a on a.atiid = da.atiid
		 left JOIN projetos.planointernoatividade pla ON pla.atiid = a.atiid
		 left JOIN financeiro.planointerno pi ON  pi.pliid = pla.pliid
		 left JOIN financeiro.execucao exe ON exe.ptres = pi.pliptres and exe.plicod = pi.plicod

		$join
	$where
	group by
	 	da.numero,
		da.profundidade,
		da.atiid,
		a.atidescricao,
		a.atiporcentoexec,
		a.atidatainicio,
		a.atidatafim,
		ea.esadescricao,
		a.esaid,
		a.atidataconclusao,
		da.ordem
	order by
		da.ordem
EOT;


// ----- realiza requisição

$lista = $db->carregar( $sql );

$total = count($lista)-1;
$datArr = explode("-",$lista[0]['rofdata']);

if(!empty($datArr[0]))
	$data   = "{$datArr[2]}-{$datArr[1]}-{$datArr[0]}"; 

$lista = $lista ? $lista : array();
$lista_final = array();

foreach ( $lista as $item )
{
	$item['detalhar'] = true;
	if ( array_key_exists( $item['numero'], $lista_final ) )
	{
		continue;
	}
	// adiciona pais (caso o pai não esteja na lista)
	$numero_pai = substr( $item['numero'], 0, strrpos( $item['numero'], '.' ) );
	if ( $numero_pai && !array_key_exists( $numero_pai, $lista_final ) && $item['atiid'] != $atiidraiz )
	{
		$rastro_pai = atividade_pegar_rastro( $item['numero'] );
		foreach ( $rastro_pai as $item_pai )
		{
			if ( !array_key_exists( $item_pai['numero'], $lista_final ) )
			{
				$item_pai['detalhar'] = false;
				$item_pai['atidescricao'] = $item_pai['numero'] . ' - ' .  $item_pai['atidescricao'];
				$lista_final[$item_pai['numero']] = $item_pai;
			}
		}
	}
	// adiciona item à lista
	$lista_final[$item['numero']] = $item;
}

// ----- monta dados para exibição
$ordem_coluna = array();
$label_coluna = array();
foreach ( $colunas as $coluna )
{
	switch ( $coluna )
	{
		case 'descricao':
		case 'detalhamento':
		case 'responsavel':
			array_push( $ordem_coluna, 'atidescricao' );
			array_push( $label_coluna, 'Descrição' );
			break;
		case 'meta':
			array_push( $ordem_coluna, 'atimeta' );
			array_push( $label_coluna, 'Meta' );
			break;
		case 'interface':
			array_push( $ordem_coluna, 'atiinterface' );
			array_push( $label_coluna, 'Órgãos Participantes' );
			break;
		case 'conclusao':
			array_push( $ordem_coluna, 'atiporcentoexec' );
			array_push( $label_coluna, '%' );
			break;
		case 'data':
			array_push( $ordem_coluna, 'atidatainicio' );
			array_push( $label_coluna, 'Início' );
			array_push( $ordem_coluna, 'atidatafim' );
			array_push( $label_coluna, 'Término' );
			break;
		case 'estado':
			array_push( $ordem_coluna, 'esadescricao' );
			array_push( $label_coluna, 'Estado' );
			break;
		case 'arquivo':
			array_push( $ordem_coluna, 'arquivo' );
			array_push( $label_coluna, 'Instrumento' );
			break;
		case 'orcamento':
			if ( !$orcamento_fim || !$orcamento_inicio )
			{
				break;;
			}
			array_push( $ordem_coluna, 'orcamento' );
			if ( $orcamento_agrupar )
			{
				array_push( $label_coluna, 'Orçamento ' . $orcamento_inicio . '-' . $orcamento_fim );
			}
			else for ( $ano = $orcamento_inicio; $ano <= $orcamento_fim; $ano++ )
			{
				array_push( $label_coluna, 'Orçamento ' . $ano  );
			}
			break;
		case 'rofautorizado':
			array_push( $ordem_coluna, 'rofautorizado' );
			array_push( $label_coluna, 'Lei + Créditos <BR>(Autorizado)' );
			break;
		case 'rofempenhado':
			array_push( $ordem_coluna, 'rofempenhado' );
			array_push( $label_coluna, 'Empenhado' );
			break;	
		case 'rofliquidado':
			array_push( $ordem_coluna, 'rofliquidado' );
			array_push( $label_coluna, 'Liquidado' );
			break;	
		case 'rofpago':
			array_push( $ordem_coluna, 'rofpago' );
			array_push( $label_coluna, 'Pago' );
			break;								
	}
}
$label_coluna = array_unique( $label_coluna );
$ordem_coluna = array_unique( $ordem_coluna );

?>
	<body>
		<?php
			$primeiro_item = reset( $lista_final );
			$profundidade_inicial = $primeiro_item['profundidade'];
		?>
		<?php if ( count( $lista_final ) ) :  ?>
			<table bgcolor="#fefefe" border="0" class="tabela" width="90%" align="center" cellpadding="2" cellspacing="0" width="100%" style="border: 1px solid #ababab; border-left: none;">
				<thead>
					<tr bgcolor="#e0e0e0">
						<?php foreach ( $label_coluna as $coluna ) : ?>
							<td align="center" style="border-left: 1px solid #ababab;">
								<b><?php echo $coluna; ?></b>
							</td>
						<?php endforeach; ?>
					</tr>
				</thead>
				<tbody>
					<?php foreach ( $lista_final as $item ) : ?>
						<tr>
							<?php if ( $item['detalhar'] ) : ?>
								<?php reset( $ordem_coluna ); ?>
								<?php foreach ( $ordem_coluna as $coluna ) : ?>
									<?php
										$padding  = 0;
										$conteudo = $item[$coluna];
										switch ( $coluna )
										{
											case 'atidescricao':
												$padding = ( $item['profundidade'] - $profundidade_inicial ) * 15;
												$conteudo = '<img src="../imagens/star.gif" border="0" align="top" title="Estratégica"/> <a href="/projetos/projetos.php?modulo=principal/atividade_/planoInterno&acao=A&atiid=' .$item['atiid']. '" >' . $item['numero'] . ' - ' . $item['atidescricao'] . '</a>';
												break;
											case 'atidatafim':
												$dados = explode( '-', $item[$coluna] );
												if ( count( $dados ) == 3 )
												{
													$conteudo = $dados[2] . '/' . $dados[1] . '/' . $dados[0];
													$tempo = mktime( 0, 0, 0, $dados[1], $dados[2], $dados[0] );
													if ( $item['atiporcentoexec'] < 100 && $tempo < mktime() )
													{
														$conteudo = '<span class="span_datafim_ultrapassada">' . $conteudo . '</span>';
													}
												}
												break;
											case 'atidatainicio':
												$dados = explode( '-', $item[$coluna] );
												$conteudo = count( $dados ) != 3 ? '-' : $dados[2] . '/' . $dados[1] . '/' . $dados[0];
												break;
											case 'atiporcentoexec':
												$conteudo = ( (integer) $item[$coluna] ) . '%';
												break;
											case 'esadescricao':
												$conclusao = (integer) $item['atiporcentoexec'];
												if ( $item['esaid'] == 5 )
												{
													$dados = explode( '-', $item['atidataconclusao'] );
													if ( count( $dados ) == 3 )
													{
														$conteudo .= " em " . $dados[2] . '/' . $dados[1] . '/' . $dados[0];
													}
												}
												switch ( $item['esaid'] )
												{
													case 1: // não iniciado
													default:
														$cor = '#707070';
														break;
													case 2: // em andamento
														$cor = '#209020';
														break;
													case 3: // suspenso
														$cor = '#aa9020';
														break;
													case 4: // cancelado
														$cor = '#aa2020';
														break;
													case 5: // concluído
														$cor = '#2020aa';
														break;
												}
												$conteudo = $conteudo ? $conteudo : '-';
												$conteudo = '<span style="color: ' . $cor . ';">' . $conteudo . '</span>';
												break;
											case 'orcamento':
												$orcamento = capturar_orcamento(
													$item['atiid'],
													$orcamento_inicio,
													$orcamento_fim,
													$orcamento_agrupar
												);
												foreach ( $orcamento as $ano => $item_orcamento )
												{
													?>
													<td class="coluna_orcamento">
														<?php echo number_format( $item_orcamento, 2, ',', '.' ); ?>
													</td>
													<?php
												}
												continue 2;
											case 'arquivo':
												$arquivos = capturar_arquivos( $item['atiid'] );
												?><td class="coluna_orcamento"><?php
												if ( count( $arquivos ) == 0 )
												{
													echo '-';
												}
												else foreach ( $arquivos as $arquivo )
												{
													?>
													<a href="?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&verid=<?= $arquivo['verid'] ?>" title="arquivo anexado por <?= $arquivo['usunome'] ?> no dia <?= formata_data( $arquivo['verdata'] ) ?>">
														<?= $arquivo['vernome'] ?>
													</a><br/>
													<?php
												}
												?></td><?php
												continue 2;
												break;
											case 'rofautorizado':
												$tRofautorizado += $conteudo;
												?>
												<td class='coluna_orcamento'><?=$conteudo > 0 ? number_format($conteudo,2,',','.') : '-'; ?></td>
												<?
												continue 2;
												break;	
											case 'rofliquidado':
												$tRofliquidado += $conteudo;
												?>
												<td class='coluna_orcamento'><?=$conteudo > 0 ? number_format($conteudo,2,',','.') : '-'; ?></td>
												<?
												continue 2;
												break;
											case 'rofempenhado':
												$tRofempenhado += $conteudo;
												?>
												<td class='coluna_orcamento'><?=$conteudo > 0 ? number_format($conteudo,2,',','.') : '-'; ?></td>
												<?
												continue 2;
												break;	
											case 'rofpago':
												$tRofpago += $conteudo;
												?>
												<td class='coluna_orcamento' ><?=$conteudo > 0 ? number_format($conteudo,2,',','.') : '-'; ?></td>
												<?
												continue 2;
												break;	
										}
									?>
									<td
										style="
											padding-left: <?php echo $padding; ?>px;
											text-align: <?php echo $alinhamento; ?>;
										"
										class="coluna_<?php echo $coluna; ?>"
									>
										<?php echo $conteudo ? $conteudo : '-'; ?>
									</td>
								<?php endforeach; ?>
							<?php else : ?>
								<?php $padding = ( $item['profundidade'] - $profundidade_inicial ) * 15; ?>
								<td
									style="padding-left: <?php echo $padding; ?>px;"
									class="coluna_desativada"
								>
									<?php echo $item['atidescricao']; ?>
								</td>
								<?php for ( $qtd_coluna = count( $label_coluna ) - 1; $qtd_coluna > 0; $qtd_coluna-- ) : ?>
									<td class="coluna_desativada">&nbsp;</td>
								<?php endfor ; ?>
							<?php endif; ?>
						</tr>
					<?php endforeach; ?>
						<TR style="background:#DFDFDF;">
							<TD colspan="5">&nbsp;&nbsp;&nbsp;&nbsp;Totais:</TD>
							<td align="right" style="color: rgb(0, 102, 204);"><?=number_format($tRofautorizado,2,',','.') ?></td>
							<td align="right" style="color: rgb(0, 102, 204);"><?=number_format($tRofempenhado,2,',','.')?></td>	
							<td align="right" style="color: rgb(0, 102, 204);"><?=number_format($tRofliquidado,2,',','.')?></td>
							<td align="right" style="color: rgb(0, 102, 204);"><?=number_format($tRofpago,2,',','.')?></td>	
						</TR>					
						<TR style="background:#FFFFFF;">
							<TD colspan="10" align="right" style="font-weight:bold; font-size:9px; border-top:2px solid black; border-bottom:2px solid black;"><div style="float:left; font-size:11px;">Total de registros: <?=$total?></div><?= $data ? "* Dados financeiros atualizados até: $data" : "";?></TD>
 						</tr>
				</tbody>
			</table>
		<?php else : ?>
			<div style="color:#902020;font-size:12pt;text-align:center;">
				<br/><br/><br/>
				Não foram encontradas atividades com o filtro desejado.
			</div>
		<?php endif; ?>
					
					
