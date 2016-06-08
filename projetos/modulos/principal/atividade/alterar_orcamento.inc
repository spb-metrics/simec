<?php

// utilizado para redirecionar 
$parametros = array(
	'visao' => $_REQUEST['visao'],
	'atiid' => $_REQUEST['atiid'],
);

switch( $_REQUEST['evento'] ){

	case 'cadastrar_orcamento':
		$sql = sprintf(
			"delete from projetos.orcamentoatividade where orcano = %d and atiid = %d",
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
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			recarregar_popup();
		}
		$db->commit();
		recarregar_popup();
		break;

	case 'excluir_orcamento':
		$sql = sprintf( "delete from projetos.orcamentoatividade where orcid = %d", $_REQUEST['orcid'] );
		if( !$db->executar( $sql ) ){
			$db->rollback();
		} else {
			$db->commit();
		}
		recarregar_popup();
		break;

	default:
		break;

}

?>
<html>
	<head>
		<title>SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação</title>
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Expires" content="-1">
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<style type="text/css">
			.combo { width: 200px; }
		</style>
		<script type="text/javascript">

			function cadastrar_orcamento(){
				if ( validar_formulario_orcamento() ) {
//					window.opener.location.reload();
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
//					window.opener.location.reload();
					window.location = '?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&atiid=<?= $_REQUEST['atiid'] ?>&visao=<?= $_REQUEST['visao'] ?>&evento=excluir_orcamento&orcid='+ orcamento;
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
			
			<?php if( $_REQUEST['recarregar'] ): ?>
				// recarrega a janela principal
				window.opener.document.filtro.reset();
				window.opener.filtrarListagem();
			<?php endif; ?>
			
		</script>
	</head>
	<body onload="window.focus();">
		<?php monta_titulo( $titulo_modulo, '&nbsp;' ); ?>
		
		<!-- ATIVIDADE -->
		<?php
			$sql = sprintf( "select atidescricao, atidatainicio, atidatafim from projetos.atividade where atiid = %d", $_REQUEST['atiid'] );
			$atividade = $db->pegaLinha( $sql );
		?>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td colspan="2" bgcolor="#cdcdcd"><a href="?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&atiid=<?= $_REQUEST['atiid'] ?>"><b>Atividade</b></a></td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
				<td><?= $atividade['atidescricao'] ?></td>
			</tr>
			<?php if( $atividade['atidatainicio'] && $atividade['atidatafim'] ): ?>
			<tr>
				<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Período:</td>
				<td><?= formata_data( $atividade['atidatainicio'] ) ?> à <?= formata_data( $atividade['atidatafim'] ) ?></td>
			</tr>
			<?php endif; ?>
		</table>
		
		<!-- NOVO ORCAMENTO -->
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="margin-top:20px">
			<form method="post" name="orcamento" enctype="multipart/form-data">
				<input type="hidden" name="visao" value="<?= $_REQUEST['visao'] ?>"/>
				<input type="hidden" name="evento" value="cadastrar_orcamento"/>
				<input type="hidden" name="atiid" value="<?= $_REQUEST['atiid'] ?>"/>
				<tr>
					<td colspan="2" bgcolor="#cdcdcd"><b>Novo Orçamento</b></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top">Ano:</td>
					<td><?= campo_texto( 'orcano', 'S', 'S', '', 5, 4, '####', '' ); ?></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top">Valor:</td>
					<td><?= campo_texto( 'orcvalor', 'S', 'S', '', 30, 20, '###.###.###.###.###.###.###', '' ); ?></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">&nbsp;</td>
					<td>
						<input type="button" name="botao" value="Enviar" onclick="cadastrar_orcamento();"/>
						<input type="button" name="fechar" value="Fechar" onclick="self.close();">
					</td>
				</tr>
			</form>
		</table>
		
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
		?>
		<?php if( is_array( $orcamentos ) ): ?>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="margin-top:20px">
			<tr>
				<td colspan="2" bgcolor="#cdcdcd"><b>Orçamentos</b></td>
			</tr>
			<tr>
				<td colspan="2" style="padding:10px">
					<table class='listagem' style="width:100%">
						<thead>
							<tr>
								<td style="width:10%">&nbsp;</td>
								<td style="width:30%">Ano</td>
								<td style="width:60%">Valor</td>
							</tr>
						</thead>
						<tbody>
						<?php foreach( $orcamentos as $indice => $orcamento ): ?>
							<tr style="vertical-align:top; background-color: <?= $indice % 2 ? '#e0e0e0' : '#f4f4f4' ?>">
								<td style="padding:5px; text-align:center" nowrap="nowrap">
									<img src="../imagens/excluir.gif" onmouseover="this.style.cursor='pointer'" onclick="excluirOrcamento( <?= $orcamento['orcid'] ?> );"/>
								</td>
								<td style="padding:5px; text-align:center"><?= $orcamento['orcano'] ?></td>
								<td style="padding:5px; text-align:right">R$ <?= number_format( $orcamento['orcvalor'], 2, ',', '.' ) ?></td>
							</tr>
						<?php endforeach; ?>
						</tbody>
					</table>
				</td>
			</tr>
		</table>
		<?php endif; ?>
	</body>
</html>