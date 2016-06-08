<?php

// ----- VERIFICA SE PROJETO ESTÁ SELECIONADO
projeto_verifica_selecionado();

// ----- CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
montar_titulo_projeto();

/*
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba($abacod_tela,$url,'');
monta_titulo( 'Pesquisar Atividades', '&nbsp;' );
*/

// define campos que serão utilizados na busca
$campo_busca = array( 'da.numero', 'a.atidescricao' );
if ( $_REQUEST['buscar_por_descricao'] ) {
	array_push( $campo_busca, 'a.atidetalhamento' );
}
foreach ( $campo_busca as &$item_campo_busca ) {
	$item_campo_busca = " coalesce( " . $item_campo_busca . ", '' ) ";
}
$campo_busca = implode( " || ' ' || ", $campo_busca );

// cria condição a ser utilizada na busca
$condicaoOpcional = array();
$condicaoObrigatoria = array();
$condicaoExcludente = array();

$realce = array();

$arrExpressoes = explode( '\"' , $_REQUEST['filtro'] );
$arrAspas = array();
$arrForaAspas = array();
$strFiltro =  trim( $_REQUEST['filtro'] );

if( sizeof( $arrExpressoes ) > 0 )
{
	for( $i = 0; $i < sizeof( $arrExpressoes ) ; ++$i )
	{
		if( ( $i % 2) == 1 )
		{
			if( strlen( $arrExpressoes[ $i - 1 ] ) > 0 )
			{
				$strUltimaLetraDoAnterior = $arrExpressoes[ $i - 1 ][ strlen( $arrExpressoes[ $i - 1 ] ) - 1  ];
				switch( $strUltimaLetraDoAnterior )
				{
					case '-':
					case '+':
					{
						$arrExpressoes[ $i ] = $strUltimaLetraDoAnterior . $arrExpressoes[ $i ];
						$arrExpressoes[ $i - 1 ] = substr( $arrExpressoes[ $i - 1 ] , 0 , strlen(  $arrExpressoes[ $i - 1 ] ) - 2 ); 
						break;
					}
					default:
					{
						break;
					}
				}
			}
			$arrAspas[] = $arrExpressoes[ $i ];
		}
		else
		{
			$arrForaAspas[] = &$arrExpressoes[ $i ];
		}
	}
}
$strFiltro =  implode( ' ' , $arrForaAspas );

$filtro = array_unique( array_merge( $arrAspas , split( '[[:space:]]', $strFiltro ) ) );

foreach ( $filtro as $indice => $valor ) {
	if ( empty( $valor ) || strlen( $valor ) < 2 ) {
		unset( $filtro[$indice] );
		continue;
	}
	switch( $valor[0] )
	{
		case '+':
		{
			$valor = substr( $valor , 1 );
			array_push( $condicaoObrigatoria, " lower( " . $campo_busca . " ) like '%" . strtolower( $valor ) . "%' " );
			$realce[] = $valor;
			break;
		}
		case '-':
		{
			$valor = substr( $valor , 1 );
			array_push( $condicaoExcludente, " lower( " . $campo_busca . " ) like '%" . strtolower( $valor ) . "%' " );
			$realce[] = $valor;
			break;
		}
		default:
		{
			array_push( $condicaoOpcional, " lower( " . $campo_busca . " ) like '%" . strtolower( $valor ) . "%' " );
			$realce[] = $valor;
			break;
		}
	}
	if( strpos( $valor , " " ) !== false ) $valor = '"' . $valor . '"';
	$filtro[ $indice ] = htmlentities( $valor );
}


// realiza busca
//$_REQUEST['filtro'] = implode( ' ', $filtro );
$_REQUEST['filtro'] = htmlentities( stripslashes( $_REQUEST[ 'filtro' ] ) );
if ( !empty( $condicaoOpcional ) || !empty( $condicaoObrigatoria ) || !empty( $condicaoExcludente )  ) {
	$condicao = '';
	if( sizeof( $condicaoOpcional ) > 0 ) $condicao .= ' and ( ' . implode( ' or ', $condicaoOpcional ) . ' ) ' . "\n";
	if( sizeof( $condicaoObrigatoria ) > 0 ) $condicao .= ' and ( ' . implode( ' and ', $condicaoObrigatoria ) . ' ) ' . "\n";
	if( sizeof( $condicaoExcludente ) > 0 ) $condicao .= ' and not ( ' . implode( ' or ', $condicaoExcludente	 ) . ' )  '. "\n";
	$sql = sprintf(
		"select
			da.numero,
			da.profundidade,
			da.irmaos,
			da.filhos,
			a.atidescricao,
			a.atiid,
			a.atiidpai,
			a.atidatainicio,
			a.atidatafim,
			a.atiordem,
			a.atiporcentoexec,
			a.esaid,
			ea.esadescricao,
			u.usunome,
			u.usucpf,
			uni.unidsc,
			ug.ungdsc
		from projetos.atividade a
			inner join projetos.f_dadostodasatividades() as da on
				da.atiid = a.atiid
			left join projetos.estadoatividade ea on
				ea.esaid = a.esaid
			left join projetos.usuarioresponsabilidade ur on
				ur.atiid = da.atiid and
				ur.pflcod = %d and
				ur.rpustatus = 'A'
			left join seguranca.usuario u on
				u.usucpf = ur.usucpf and
				u.usustatus = 'A'
			left join public.unidade uni on
				uni.unicod = u.unicod and
				uni.unitpocod = 'U' and
				uni.unistatus = 'A'
			left join public.unidadegestora ug on
				ug.ungcod = u.ungcod and
				ug.ungstatus = 'A'
		where
			atistatus = 'A' and
			a.atistatus = 'A' and
			da.projeto = %d
			%s
		order by da.ordem",
		PERFIL_GERENTE,
		PROJETO,
		$condicao
	);
	
	$lista = $db->carregar( $sql );
	$lista = $lista ? $lista : array();
	$atividades = $lista;
	/*
	if ( is_array( $lista ) ) {
		foreach ( $lista as $item ) {
			if ( array_key_exists( $item['numero'], $atividades ) ) {
				continue;
			}
			// adiciona pais (caso o pai não esteja na lista)
			$numero_pai = substr( $item['numero'], 0, strrpos( $item['numero'], '.' ) );
			if ( $numero_pai && !array_key_exists( $numero_pai, $atividades ) ) {
				$rastro_pai = atividade_pegar_rastro( $item['numero'] );
				foreach ( $rastro_pai as $item_pai ) {
					if ( !array_key_exists( $item_pai['numero'], $atividades ) ) {
						$atividades[$item_pai['numero']] = $item_pai;
					}
				}
			}
			// adiciona item à lista
			$atividades[$item['numero']] = $item;
		}
		$atividades = array_values( $atividades );
	}
*/
} else {
	$atividades = array();
}

?>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="0" cellpadding="10" align="center">
	<tr>
		<td>
			<?= montar_formulario_pesquisa( true ); ?>
			<p style="text-align:center; padding-bottom:10px;">
				<?php if ( !$atividades ) : ?>
					<?php if( $condicao ): ?>
						Não há resultados para esta pesquisa.
					<?php else: ?>
						Preencha o formulário para realizar a pesquisa.
					<?php endif; ?>
				<?php elseif( count( $atividades ) == 1 ): ?>
					Esta pesquisa obteve <b>uma</b> atividade.
				<?php elseif ( count( $atividades ) > 1 ) : ?>
					Esta pesquisa obteve <b><?= count( $atividades ) ?></b> atividades.
				<?php endif; ?>
			</p>
			<table align="center" class="tabela" cellpadding="3">
				<colgroup>
					<col/>
					<col width="80"/>
					<col width="70"/>
					<col width="70"/>
				</colgroup>
				<thead>
					<tr style="background-color: #e0e0e0">
						<td style="font-weight:bold; text-align:center;">Título</td>
						<td style="font-weight:bold; text-align:center;">Situação</td>
						<td style="font-weight:bold; text-align:center;">Início</td>
						<td style="font-weight:bold; text-align:center;">Término</td>
					</tr>
				</thead>
				<tbody>
					<?php
						$profundidade_inicial = $atividades[0]['profundidade'];
						$pilha_permissao = array();
					?>
					<?php foreach( $atividades as $indice => $atividade ): ?>
					<?php
						
						$descricao = str_replace( "'", "", str_replace( '"', "", $atividade['atidescricao'] ) );
						$numero = $atividade['numero'];
						foreach ( $realce as $palavra )
						{
							if ( strlen( $palavra ) < 3 ) {
								continue;
							}
							$numero = str_ireplace(
								$palavra,
								'<span style="background-color: #ffffcc; border: 1px solid #eeee66;">' . $palavra . '</span>',
								$numero
							);
							$descricao = str_ireplace(
								$palavra,
								'<span style="background-color: #ffffcc; border: 1px solid #eeee66;">' . $palavra . '</span>',
								$descricao
							);
						}
						
						// identifica posição em relação aos irmãos
						$primeiro = true;
						$ultimo   = true;
						// verifica se tem filhos
						$profundidade         = 0;
						$tem_filhos           = false;
						$filho_esta_na_tabela = false;
						// verifica se deve ser exibido
						$visivel = true;
						// verifica permissão em alguns níveis
						$permissao     = atividade_verificar_responsabilidade( $atividade['atiid'] );
						$permissao_pai = false;
						$permissao_avo = false;
						
						// seleciona a cor da linha
						$cor = $cor == '#fafafa' ? '#f0f0f0' : '#fafafa';
					?>
					<tr style="background-color: <?= $cor ?>;<?= $visivel ? '' : 'display:none' ?>;">
						
						<!-- COLUNA TÍTULO -->
						<td>
							<div style="overflow:hidden; width:100%; height:14px;" scroll="no">
								<span style="color:#909090"><?= $numero ?></span>
								<a
									onmouseover="window.SuperTitleOn( this , this.innerHTML )"
									onmouseout="window.SuperTitleOff( this )"
									href="?modulo=principal/atividade_/atividade&acao=A&atiid=<?= $atividade['atiid'] ?>"
									<?= $permissao ? 'style="color:#133368"' : 'style="color:#909090"' ?>
								><?= $descricao ?></a>
							</div>
							<?php if ( $atividade['usunome'] ) : ?>
								<a href="#" onclick="enviar_email( '<?= $atividade['usucpf'] ?>' ); return false;" style="text-decoration: none; margin-left: 18px;">
									<span style="font-size: 9px; font-weight: normal; color: #333333;">
										<?= $atividade['usunome'] ?> / <?= $atividade['unidsc'] ?> / <?= $atividade['ungdsc'] ?>
									</span>
								</a> 
							<?php endif; ?>
						</td>
						
						<!-- COLUNA SITUAÇÃO -->
						<td style="text-align:center;">
							<?= montar_barra_execucao( $atividade, $permissao ) ?>
						</td>
						
						<!-- COLUNAS DATA -->
						<td style="text-align:center; color: <?= $permissao ? '#008000' : '#909090' ?>;">
							<?= formata_data( $atividade['atidatainicio'] ) ?>
						</td>
						<?php
							$cor_data = $permissao ? '#008000' : '#909090';
							if( $atividade['atidatafim'] && $atividade['esaid'] && $permissao ) {
								// captura o timestamp da data fim
								$datafim = explode( '-', $atividade['atidatafim'] );
								$datafim = mktime( 0, 0, 0, $datafim[1], $datafim[2], $datafim[0] );
								// seleciona a cor
								if ( $atividade['atiporcentoexec'] != 100 && $datafim < mktime() ) {
									$cor_data = '#901010';
								}
								// remove as variáveis do escopo
								unset( $datafim );
							}
						?>
						<td style="text-align:center; color: <?= $cor_data ?>;">
							<?= formata_data( $atividade['atidatafim'] ) ?>
						</td>
						
					</tr>
					<?php endforeach; ?>
				</tbody>
			</table>
		</td>
	</tr>
</table>
<?php

/*

<table class="tabela" bgcolor="#f5f5f5" cellspacing="0" cellpadding="10" align="center">
	<tr>
		<td>
			<?= montar_formulario_pesquisa( true ); ?>
			<p style="text-align:center; padding-bottom:10px;">
				<?php if ( !$atividades ) : ?>
					<?php if( $condicao ): ?>
						Não há resultados para esta pesquisa.
					<?php else: ?>
						Preencha o formulário para realizar a pesquisa.
					<?php endif; ?>
				<?php elseif( count( $atividades ) == 1 ): ?>
					Esta pesquisa obteve <b>uma</b> atividade.
				<?php elseif ( count( $atividades ) > 1 ) : ?>
					Esta pesquisa obteve <b><?= count( $atividades ) ?></b> atividades.
				<?php endif; ?>
			</p>
			<?php if( !empty( $atividades ) ): ?>
				<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width:100%;">
					<thead>
						<tr style="background-color: #e0e0e0">
							<td style="font-weight:bold; text-align:center;" colspan="2">Título</td>
							<td style="font-weight:bold; text-align:center; width:100px">Situação</td>
							<td style="font-weight:bold; text-align:center; width:100px">Data Início</td>
							<td style="font-weight:bold; text-align:center; width:100px">Data Conclusão</td>
						</tr>
					</thead>
					<tbody>
					<?php foreach( $atividades as $atividade ): ?>
						<?php $cor = $cor == '#fafafa' ? '#f0f0f0' : '#fafafa'; ?>
						<?php
							$numero    = $atividade['numero'];
							$descricao = $atividade['atidescricao'];
							
							$texto_resultado = $atividade['numero'] . ' ' . $atividade['atidescricao'];
							reset( $filtro );
							foreach ( $filtro as $palavra )
							{
								if ( strlen( $palavra ) < 3 ) {
									continue;
								}
								$numero = str_ireplace(
									$palavra,
									'<span style="background-color: #ffffcc; border: 1px solid #eeee66;">' . $palavra . '</span>',
									$numero
								);
								$descricao = str_ireplace(
									$palavra,
									'<span style="background-color: #ffffcc; border: 1px solid #eeee66;">' . $palavra . '</span>',
									$descricao
								);
							}
						?>
						<tr style="background-color: <?= $cor ?>;" onmouseover="this.style.backgroundColor='#ffffcc';" onmouseout="this.style.backgroundColor='<?= $cor ?>';">
							<td align="right" width="60">
								<a href="?modulo=principal/atividade_/atividade&acao=A&atiid=<?= $atividade['atiid'] ?>">
									<?= $numero ?> -
								</a>
							</td>
							<td>
								<a href="?modulo=principal/atividade_/atividade&acao=A&atiid=<?= $atividade['atiid'] ?>">
									<?= $descricao ?>
								</a>
							</td>
							<td style="text-align:center;"><?= $atividade['esadescricao'] ?></td>
							<td style="text-align:center;"><?= formata_data( $atividade['atidatainicio'] ) ?></td>
							<td style="text-align:center;"><?= formata_data( $atividade['atidatafim'] ) ?></td>
						</tr>
					<?php endforeach; ?>
					</tbody>
				</table>
			<?php endif; ?>
		</td>
	</tr>
</table>

*/

?>