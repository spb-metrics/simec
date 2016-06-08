<?php

$atividade = atividade_pegar( $_REQUEST['atiid'] );
if ( !$atividade ) {
	redirecionar( 'principal/atividade_/arvore', 'A' );
}

switch( $_REQUEST['evento'] ){

	case 'cadastrar_orcamento':
		$sql = sprintf(
			"delete from projetos.orcamentoatividade where orcano = '%d' and atiid = %d",
			$_REQUEST['orcano'],
			$_REQUEST['atiid']
		);
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			recarregar_popup();
		}
		$sql = sprintf(
			"insert into projetos.orcamentoatividade ( orcano, orcvalor, atiid ) values ( '%s', '%s', %d )",
			$_REQUEST['orcano'],
			str_replace( '.', '', $_REQUEST['orcvalor'] ), # retira os caracteres indicadores
			$_REQUEST['atiid']
		);
		if( !$db->executar( $sql ) ){
			$db->rollback();
		} else {
			$db->commit();
		}
		$parametros = array(
			'aba' => $_REQUEST['aba'], # mantém a aba ativada
			'atiid' => $_REQUEST['atiid']
		);
		redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
		break;

	case 'excluir_orcamento':
		$sql = sprintf( "delete from projetos.orcamentoatividade where orcid = %d", $_REQUEST['orcid'] );
		if( !$db->executar( $sql ) ){
			$db->rollback();
		} else {
			$db->commit();
		}
		$parametros = array(
			'aba' => $_REQUEST['aba'], # mantém a aba ativada
			'atiid' => $_REQUEST['atiid']
		);
		redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
		break;

	default:
		break;

}

$permissao = atividade_verificar_responsabilidade( $atividade['atiid'], $_SESSION['usucpf'] );
$permissao_formulario = $permissao ? 'S' : 'N'; # S habilita e N desabilita o formulário

// ----- VERIFICA SE PROJETO ESTÁ SELECIONADO
projeto_verifica_selecionado();

// ----- CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '&atiid=' . $atividade['atiid']  );
montar_titulo_projeto( $atividade['atidescricao'] );

/*
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba(
	$abacod_tela,
	$url,
	'&atiid='.$_REQUEST['atiid']
);
monta_titulo(
	$titulo_modulo,
	$permissao ? '&nbsp;' : '<img src="../imagens/preview.gif" align="absmiddle"/> Atividade disponível apenas para visualização'
);
*/

extract( $atividade ); # mantém o formulário preenchido
?>
<script language="javascript" type="text/javascript">
	function cadastrar_orcamento(){
		if ( validar_formulario_orcamento() ) {
			document.orcamento.submit();
		}
	}

	function validar_formulario_orcamento(){
		var validacao = true;
		var mensagem = 'Os seguintes campos não foram preenchidos:';
		document.orcamento.orcano.value = trim( document.orcamento.orcano.value );
		document.orcamento.orcvalor.value = trim( document.orcamento.orcvalor.value );
		if ( document.orcamento.orcano.value.length != 4 ) {
			mensagem += '\nAno';
			validacao = false;
		}
		if ( document.orcamento.orcvalor.value == '' ) {
			mensagem += '\nValor';
			validacao = false;
		}
		if ( !validacao ) {
			alert( mensagem );
		}
		return validacao;
	}
	
	function excluirOrcamento( orcamento ){
		if ( confirm( 'Deseja excluir o instrumento?' ) ) {
			window.location = '?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&atiid=<?= $_REQUEST['atiid'] ?>&aba=<?= $_REQUEST['aba'] ?>&evento=excluir_orcamento&orcid='+ orcamento;
		}
	}
	
	function ltrim( value ){
		var re = /\s*((\S+\s*)*)/;
		return value.replace(re, "$1");
	}
	
	function rtrim( value ){
		var re = /((\s*\S+)*)\s*/;
		return value.replace(re, "$1");
	}
	
	function trim( value ){
		return ltrim(rtrim(value));
	}
</script>
<table class="tabela" bgcolor="#fbfbfb" cellspacing="0" cellpadding="10" align="center">
	<tr>
		<td>
			<?= montar_resumo_atividade( $atividade ) ?>
			<!-- NOVO ORCAMENTO -->
			<?php if( $permissao ): ?>
				<form method="post" name="orcamento">
					<input type="hidden" name="evento" value="cadastrar_orcamento"/>
					<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 100%;">
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Ano:</td>
							<td><?= campo_texto( 'orcano', 'S', 'S', '', 5, 4, '####', '' ); ?></td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Valor:</td>
							<td><?= campo_texto( 'orcvalor', 'S', 'S', '', 30, 20, '###.###.###.###.###.###.###', '' ); ?></td>
						</tr>
						<tr style="background-color: #cccccc">
							<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
							<td><input type="button" name="botao" value="Salvar" onclick="cadastrar_orcamento();"/></td>
						</tr>
					</table>
				</form>
			<?php endif; ?>
			<!-- LISTA DE ORCAMENTOS -->
			<?php
				$sql = sprintf(
					"select o.orcid, o.orcano, o.orcvalor
					from projetos.orcamentoatividade o
					where o.atiid = %d
					order by o.orcano desc",
					$_REQUEST['atiid']
				);
				$orcamentos = $db->carregar( $sql );
				$total = 0;
			?>
			<?php if( is_array( $orcamentos ) ): ?>
				<table class='tabela' style="width:100%;"  cellpadding="3">
					<thead>
						<tr style="background-color: #e0e0e0">
							<td style="font-weight:bold; text-align:center; width:5%">&nbsp;</td>
							<td style="font-weight:bold; text-align:center; width:10%">Ano</td>
							<td style="font-weight:bold; text-align:center; width:85%">Valor</td>
						</tr>
					</thead>
					<tbody>
					<?php foreach( $orcamentos as $indice => $orcamento ): ?>
						<?php $cor = $cor == '#fafafa' ? '#f0f0f0' : '#fafafa'; ?>
						<tr style="vertical-align:top; background-color: <?= $cor ?>" onmouseover="this.style.backgroundColor='#ffffcc';" onmouseout="this.style.backgroundColor='<?= $cor ?>';">
							<td style="text-align:center" nowrap="nowrap">
								<?php if( $permissao ): ?>
									<img align="absmiddle" src="../imagens/excluir.gif" onmouseover="this.style.cursor='pointer'" onclick="excluirOrcamento( <?= $orcamento['orcid'] ?> );" title="excluir orçamento"/>
								<?php else: ?>
									<img align="absmiddle" src="../imagens/excluir_01.gif"/>
								<?php endif; ?>
							</td>
							<td style="text-align:center"><?= $orcamento['orcano'] ?></td>
							<td style="text-align:right">R$ <?= number_format( $orcamento['orcvalor'], 2, ',', '.' ) ?></td>
						</tr>
						<? $total += $orcamento['orcvalor'] ?>
					<?php endforeach; ?>
					<tr style="vertical-align:top; background-color: #ffffff">
						<td style="text-align:center" nowrap="nowrap">&nbsp;</td>
						<td style="text-align:center">&nbsp;</td>
						<td style="text-align:right"><b>R$ <?= number_format( $total, 2, ',', '.' ) ?></b></td>
					</tr>
					</tbody>
				</table>
			<?php else: ?>
				<table class='tabela' style="width:100%;" cellpadding="3">
					<tbody>
						<td style="text-align:center;padding:15px;background-color:#f5f5f5;">
							A atividade não possui orçamento
						</td>
					</tbody>
				</table>
			<?php endif; ?>
		</td>
	</tr>
</table>