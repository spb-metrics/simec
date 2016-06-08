<?php

//function redirecionar( $modulo, $acao, $parametros = array() )
//{
//	$url = parse_url( $_SERVER['HTTP_REFERER'] );
//	$location = sprintf(
//		'Location: %s://%s%s?modulo=%s&acao=%s&%s',
//		$url['scheme'],
//		$url['host'],
//		$url['path'],
//		$modulo,
//		$acao,
//		http_build_query( (array) $parametros, '', '&' )
//	);
//	header( $location );
//	exit();
//}

function recarregar_popup(){
	?>
	<script type="text/javascript">
		window.opener.filtrarListagem();
		location.href = '?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&atiid=<?= $_REQUEST['atiid'] ?>&visao=<?= $_REQUEST['visao'] ?>';
	</script>
	<?php
	exit();
}

function fechar_popup( $mensagem = '' ){
	?>
	<script type="text/javascript">
		window.opener.filtrarListagem();
		window.close();
	</script>
	<?php
	exit();
}

function excluir_registro( $atiid ) {
	global $db;
	// captura as informações da atividade a ser excluída
	$sql = sprintf( "select * from projetos.atividade a where a.atiid = %s and a.atistatus = 'A'", $atiid );
	$atividade = $db->pegaLinha( $sql );
	if ( !$atividade ) {
		return false;
	}
	// exclui a atividade
	$sql = sprintf( "update projetos.atividade set atistatus = 'I' where atiid = %s", $atividade['atiid'] );
	if ( !$db->executar( $sql ) ) {
		return false;
	}
	// reordena as atividades que tem o mesmo pai
	$sql = sprintf(
		"update projetos.atividade set atiordem = atiordem - 1 where atiidpai = %s and atiordem > %s and atistatus = 'A'",
		$atividade['atiidpai'],
		$atividade['atiordem']
	);
	if ( !$db->executar( $sql ) ) {
		return false;
	}
	return true;
}

function pegar_filhos( $registro = null ){
	global $db;
	$sql = sprintf(
		"select * from projetos.atividade where atistatus='A' and atiidpai %s order by atiordem",
		$registro ? " = ". $registro['atiid'] : " is null "
	);
	$filhos = $db->carregar( $sql );
	if ( !$filhos ) {
		return array();
	}
	return $filhos;
}

function pegar_anexo( $id ){
	global $db;
	$sql = sprintf(
		"select distinct ta.taadescricao from projetos.anexoatividade at inner join projetos.tipoanexoatividade ta on at.taaid=ta.taaid where anestatus='A' and atiid %s",
		$id ? " = ". $id : " is null "
	);
	$linhas = $db->carregar( $sql );
	if ( !$linhas ) {
		//$anexos.="<ul>";
	} else {
		
		foreach ( $linhas as $linha ) 
		{			
			$anexos.="- ".$linha['taadescricao']."<br>";
		}
		//$anexos.="</ul>";
		return $anexos;
	}
	
}

function pegar_controle( $id ){
	global $db;
	$sql = sprintf(
		"select ob.obsdescricao from projetos.observacaoatividade ob where ob.obsstatus='A' and ob.atiid %s order by obsid desc limit 1",
		$id ? " = ". $id : " is null "
	);
	$linhas = $db->carregar( $sql );
	if ( !$linhas ) {
		return "-";
	} else {
		foreach ( $linhas as $linha ) 
		{			
			$anexos.=$linha['obsdescricao'];
		}
		return $anexos;
	}
	
}

function pegar_orcamento2( $lista, $inicio, $fim ){
	global $db;
	return;
	$lista = (array) $lista;
	if ( empty( $lista ) ) {
		return null;
	}
	$orcamento = 0;
	foreach ( $lista as $atividade ) {
		$sql = sprintf( "select atiid from projetos.atividade where atiidpai = %d", $atividade );
		$atiid = $db->pegaUm( $sql );
		if ( $atiid ) {
			$orcamento += pegar_orcamento2( $atiid, $inicio, $fim );
		}
	}
	
	
	$sql = sprintf(
		"select sum( orcvalor ) as valor from projetos.orcamentoatividade where orcano >= '%d' and orcano <= '%d' and atiid in ( %s ) ",
		$inicio,
		$fim,
		implode( ',', $atividade )
	);
	dbg( $sql, 1 );
	$linha = $db->pegaLinha( $sql );
	
	if ( !$linha ) {
		return "-";
	} else {
			if ($linha['valor']>0) 
					{
						return number_format($linha['valor'],0,',','.');
					} 
				else 
					{
						return "-";
					}
	}
	
}

function pegar_orcamento( $id, $anoini, $anofim ){
	global $db;
	$sql = sprintf(
		"select sum(orcvalor) as valor from projetos.orcamentoatividade where orcano>='$anoini' and orcano<='$anofim' and atiid %s ",
		$id ? " = ". $id : " is null "
	);
	$linha = $db->pegaLinha( $sql );

	if ( !$linha ) {
		return "-";
	} else {
			if ($linha['valor']>0) 
					{
						return number_format($linha['valor'],0,',','.');
					} 
				else 
					{
						return "-";
					}
	}
	
}

function exibir_registro( $registro, $orc_agrupar, $orc_inicio, $orc_fim, $nivel = 0, $rastro = '', $total = 0, $nivel_max = 0, $col_ins = true, $col_met = true, $col_int = true, $col_orc = true, $col_con = true ){
	static $numero = 0;
	static $cor = '#f7f7f7';
	static $profundidade = -1; # nivel a partir do qual os itens serão ocultados
	if ( $profundidade == -1 )
	{
		$profundidade = $nivel_max;
	}
	
	$cor = ( $cor == '#f7f7f7' ) ? '#ffffff' : '#f7f7f7';
	if ( !$registro ) {
		return;
	}
	$numero++;
	$filhos = pegar_filhos( $registro );
	
	$somatorio = array();
	?>
	<tr id="tr<?=$registro['atiidpai']?>" bgcolor="<?= $cor ?>" onmouseout="this.bgColor='<?= $cor ?>';" onmouseover="this.bgColor='#ffffcc';" parent="<?= $registro['atiidpai'] ?>">
		<td align="right" nowrap style="width:15px; text-align:rigth"><?= $numero ?> </td>
		<td nowrap style="width:70px; text-align:center" class="notprint">
			<img border="0" src="../imagens/gif_inclui.gif" onclick="popup_cadastrar_atividade( <?= $registro['atiid'] ?> )" title="Cadastrar Atividade" onmouseover="this.style.cursor='pointer'"/>
			<img border="0" src="../imagens/alterar.gif" onclick="popup_alterar_atividade( <?= $registro['atiid'] ?> )" title="Editar Atividade" onmouseover="this.style.cursor='pointer'"/>
			<img border="0" src="../imagens/excluir.gif" onclick="excluir_atividade( <?= $registro['atiid'] ?> )" title="Excluir Atividade" onmouseover="this.style.cursor='pointer'"/>
		</td>
		<td nowrap style="width:50px; text-align:center"" class="notprint">
			<?php if( $nivel != 0 ): ?>
				<img title="Recuo Esquerda" src="../imagens/recuo_e.gif" style="border:0" onclick="setaEsquerda( <?= $registro['atiid'] ?> );" onmouseover="this.style.cursor='pointer'"/>
			<?php else: ?>
				<img title="" src="../imagens/recuo_e_d.gif" style="border:0"/>
			<?php endif; ?>
			<?php if( $registro['atiordem'] > 1 ): ?>
				<img title="Recuo Direita" src="../imagens/recuo_d.gif" style="border:0" onclick="setaDireita( <?= $registro['atiid'] ?> );" onmouseover="this.style.cursor='pointer'"/>
			<?php else: ?>
				<img title="" src="../imagens/recuo_d_d.gif" style="border:0"/>
			<?php endif; ?>
		</td>
		<td style="padding-left: <?= $nivel * 25 ?>px" class="coluna_medida">
			<?php if( $nivel > 0 ): ?>
				<img src="../imagens/seta_filho.gif" align="left"/>
			<?php endif; ?>
			<div style="float:left; width: 90%;<?= count( $filhos ) > 0 ? 'font-weight: bold' : '' ?>">
				<?if (count( $filhos ) > 0) {?>
					<img id="img<?= $registro['atiid'] ?>" atividade="<?= $registro['atiid'] ?>" onclick="exibirOcultarAtividadesFilhas( <?= $registro['atiid'] ?>, this, true );" id="imgTarefa<?= $registro['atiidpai'] ?>" src="../imagens/menos.gif"/>&nbsp;
				<?}?>
				<span onmouseover="this.style.cursor='pointer'" onclick="popup_alterar_atividade( <?= $registro['atiid'] ?> )"><?= $rastro . $registro['atiordem'] .' - '. $registro['atidescricao'] ?></span>
			</div>
		</td>
		<?php if ( $col_ins ) : ?>
			<td class="coluna_instrumento" onclick="popup_anexo( <?= $registro['atiid'] ?> )" onmouseover="this.style.cursor='pointer'"><?=pegar_anexo( $registro['atiid'] )?></td>
		<?php endif; ?>
		<?php if ( $col_met ) : ?>
			<td class="coluna_meta"><?= $registro['atimeta'] ?></td>
		<?php endif; ?>
		<?php if ( $col_int ) : ?>
			<td><?= $registro['atiinterface'] ?></td>
		<?php endif; ?>
		<?php if ( $col_orc ) : ?>
			<?php if ( !$orc_agrupar ) : ?>
				<?php foreach ( range( $orc_inicio, $orc_fim ) as $ano ) : ?>
					<td
						class="coluna_orcamento"
						onclick="popup_orcamento( <?= $registro['atiid'] ?> , <?= $ano ?> )"
						onmouseover="this.style.cursor='pointer'">
							<?php
								$valor = pegar_orcamento( $registro['atiid'], $ano, $ano );
								$somatorio[] = $valor != '-' ? (integer) str_replace( '.', '', $valor ) : 0;
							?>
							<?= $valor ?>
					</td>
				<?php endforeach ; ?>
			<?php else : ?>
				<td
					class="coluna_orcamento"
					onclick="popup_orcamento( <?= $registro['atiid'] ?>, <?= $orc_inicio ?>, <?= $orc_fim ?> )"
					onmouseover="this.style.cursor='pointer'">
						<?php
							$valor = pegar_orcamento( $registro['atiid'], $orc_inicio, $orc_fim );
							$somatorio[] = $valor != '-' ? (integer) str_replace( '.', '', $valor ) : 0;
						?>
						<?= $valor ?>
				</td>
			<?php endif; ?>
		<?php endif; ?>
		<?php if ( $col_con ) : ?>
			<td class="coluna_controle" onclick="popup_controle( <?= $registro['atiid'] ?> )" onmouseover="this.style.cursor='pointer'"><?=pegar_controle( $registro['atiid'] )?></td>
		<?php endif; ?>
		<td class="coluna_data"><?= formata_data( $registro['atidatainicio'] ) ?></td>
		<td class="coluna_data"><?= formata_data( $registro['atidatafim'] ) ?></td>
		<td nowrap style="width:50px; text-align:center"" class="notprint">
			<?php if( $registro['atiordem'] != 1 ): ?>
				<img title="Mover para cima" src="../imagens/seta_cima.gif" style="border:0" onclick="setaCima( <?= $registro['atiid'] ?> );" onmouseover="this.style.cursor='pointer'"/>
			<?php else: ?>
				<img title="" src="../imagens/seta_cimad.gif" style="border:0"/>
			<?php endif; ?>
			<?php if( $registro['atiordem'] != $total ): ?>
				<img title="Mover para baixo" src="../imagens/seta_baixo.gif" style="border:0" onclick="setaBaixo( <?= $registro['atiid'] ?> );" onmouseover="this.style.cursor='pointer'"/>
			<?php else: ?>
				<img title="" src="../imagens/seta_baixod.gif" style="border:0"/>
			<?php endif; ?>
		</td>
	</tr>
	<?php
	$rastro .= $registro['atiordem'] . '.';
	foreach ( $filhos as $filho ) {
		
		$somatorio_filho = exibir_registro( $filho, $orc_agrupar, $orc_inicio, $orc_fim, $nivel + 1, $rastro, count( $filhos ), $nivel_max, $col_ins, $col_met, $col_int, $col_orc, $col_con );
		foreach ( $somatorio_filho as $chave => $item_somatorio_filho )
		{
			$somatorio[$chave] += $item_somatorio_filho;
		}
	}
	?>
	<?php if( $nivel_max != -1 && $nivel >= $profundidade ): ?>
		<script type="text/javascript">
			var imagem = document.getElementById( "img<?= $registro['atiid'] ?>" );
			if ( imagem ) {
				imagem.onclick();
			}
		</script>
	<?php endif; ?>
	<?
	return $somatorio;
}

function exibir_registro_consulta( $registro, $orc_agrupar, $orc_inicio, $orc_fim, $nivel = 0, $rastro = '', $total = 0, $nivel_max = 0, $col_ins = true, $col_met = true, $col_int = true, $col_orc = true, $col_con = true ){
	static $numero = 0;
	static $cor = '#f7f7f7';
	static $profundidade = -1; # nivel a partir do qual os itens serão ocultados
	if ( $profundidade == -1 )
	{
		$profundidade = $nivel_max;
	}
	
	$cor = ( $cor == '#f7f7f7' ) ? '#ffffff' : '#f7f7f7';
	if ( !$registro ) {
		return;
	}
	$numero++;
	$filhos = pegar_filhos( $registro );
	
	$somatorio = array();
	?>
	<tr id="tr<?=$registro['atiidpai']?>" bgcolor="<?= $cor ?>" onmouseout="this.bgColor='<?= $cor ?>';" onmouseover="this.bgColor='#ffffcc';" parent="<?= $registro['atiidpai'] ?>">
		<td align="right" nowrap style="width:15px; text-align:rigth"><?= $numero ?> </td>
		<td style="padding-left: <?= $nivel * 25 ?>px" class="coluna_medida">
			<?php if( $nivel > 0 ): ?>
				<img src="../imagens/seta_filho.gif" align="left"/>
			<?php endif; ?>
			<div style="float:left; width: 90%;<?= count( $filhos ) > 0 ? 'font-weight: bold' : '' ?>">
				<?if (count( $filhos ) > 0) {?>
					<img id="img<?= $registro['atiid'] ?>" atividade="<?= $registro['atiid'] ?>" onclick="exibirOcultarAtividadesFilhas( <?= $registro['atiid'] ?>, this, true );" id="imgTarefa<?= $registro['atiidpai'] ?>" src="../imagens/menos.gif"/>&nbsp;
				<?}?>
				<span><?= $rastro . $registro['atiordem'] .' - '. $registro['atidescricao'] ?></span>
			</div>
		</td>
		<?php if ( $col_ins ) : ?>
			<td class="coluna_instrumento"><?=pegar_anexo( $registro['atiid'] )?></td>
		<?php endif; ?>
		<?php if ( $col_met ) : ?>
			<td class="coluna_meta"><?= $registro['atimeta'] ?></td>
		<?php endif; ?>
		<?php if ( $col_int ) : ?>
			<td><?= $registro['atiinterface'] ?></td>
		<?php endif; ?>
		<?php if ( $col_orc ) : ?>
			<?php if ( !$orc_agrupar ) : ?>
				<?php foreach ( range( $orc_inicio, $orc_fim ) as $ano ) : ?>
					<td class="coluna_orcamento">
							<?php
								$valor = pegar_orcamento( $registro['atiid'], $ano, $ano );
								$somatorio[] = $valor != '-' ? (integer) str_replace( '.', '', $valor ) : 0;
							?>
							<?= $valor ?>
					</td>
				<?php endforeach ; ?>
			<?php else : ?>
				<td class="coluna_orcamento">
						<?php
							$valor = pegar_orcamento( $registro['atiid'], $orc_inicio, $orc_fim );
							$somatorio[] = $valor != '-' ? (integer) str_replace( '.', '', $valor ) : 0;
						?>
						<?= $valor ?>
				</td>
			<?php endif; ?>
		<?php endif; ?>
		<?php if ( $col_con ) : ?>
			<td class="coluna_controle"><?=pegar_controle( $registro['atiid'] )?></td>
		<?php endif; ?>
		<td class="coluna_data"><?= formata_data( $registro['atidatainicio'] ) ?></td>
		<td class="coluna_data"><?= formata_data( $registro['atidatafim'] ) ?></td>
	</tr>
	<?php
	$rastro .= $registro['atiordem'] . '.';
	foreach ( $filhos as $filho ) {
		
		$somatorio_filho = exibir_registro_consulta( $filho, $orc_agrupar, $orc_inicio, $orc_fim, $nivel + 1, $rastro, count( $filhos ), $nivel_max, $col_ins, $col_met, $col_int, $col_orc, $col_con );
		foreach ( $somatorio_filho as $chave => $item_somatorio_filho )
		{
			$somatorio[$chave] += $item_somatorio_filho;
		}
	}
	?>
	<?php if( $nivel >= $profundidade ): ?>
		<script type="text/javascript">
			var imagem = document.getElementById( "img<?= $registro['atiid'] ?>" );
			if ( imagem ) {
				imagem.onclick();
			}
		</script>
	<?php endif; ?>
	<?
	return $somatorio;
}

?>