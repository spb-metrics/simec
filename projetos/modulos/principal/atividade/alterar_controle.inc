<?php

// utilizado para redirecionar 
$parametros = array(
	'visao' => $_REQUEST['visao'],
	'atiid' => $_REQUEST['atiid'],
);

switch( $_REQUEST['evento'] ){

	case 'cadastrar_controle':
		$sql = sprintf(
			"insert into projetos.observacaoatividade ( obsdescricao, atiid, usucpf ) values ( '%s', %d, '%s' )",
			$_REQUEST['obsdescricao'],
			$_REQUEST['atiid'],
			$_SESSION['usucpforigem']
		);
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
		} else {
			$db->commit();
		}
		recarregar_popup();
		break;

	case 'excluir_controle':
		$sql = sprintf(
			"update projetos.observacaoatividade set obsstatus = 'I' where obsid = %d and usucpf = '%s'",
			$_REQUEST['obsid'],
			$_SESSION['usucpforigem'] # impede que o usuário remova um controle de outro autor
		);
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

			function cadastrar_controle(){
				if ( validar_formulario_controle() ) {
					document.controle.submit();
				}
			}

			function validar_formulario_controle(){
				var validacao = true;
				var mensagem = 'Os seguintes campos não foram preenchidos:';
				document.controle.obsdescricao.value = trim( document.controle.obsdescricao.value );
				if ( document.controle.obsdescricao.value == '' ) {
					mensagem += '\nConteúdo';
					validacao = false;
				}
				if ( !validacao ) {
					alert( mensagem );
				}
				return validacao;
			}
			
			function excluirControle( controle ){
				if ( confirm( 'Deseja excluir o controle?' ) ) {
					window.location = '?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&atiid=<?= $_REQUEST['atiid'] ?>&visao=<?= $_REQUEST['visao'] ?>&evento=excluir_controle&obsid='+ controle;
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
		
		<!-- NOVO CONTROLE -->
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="margin-top:20px">
			<form method="post" name="controle" enctype="multipart/form-data">
				<input type="hidden" name="visao" value="<?= $_REQUEST['visao'] ?>"/>
				<input type="hidden" name="evento" value="cadastrar_controle"/>
				<input type="hidden" name="atiid" value="<?= $_REQUEST['atiid'] ?>"/>
				<tr>
					<td colspan="2" bgcolor="#cdcdcd"><b>Nova Observação</b></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top">Conteúdo:</td>
					<td><?= campo_textarea( 'obsdescricao', 'S', 'S', '', 70, 2, 250 ); ?></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">&nbsp;</td>
					<td>
						<input type="button" name="botao" value="Enviar" onclick="cadastrar_controle();"/>
						<input type="button" name="fechar" value="Fechar" onclick="self.close();">
					</td>
				</tr>
			</form>
		</table>
		
		<!-- LISTA DE CONTROLES -->
		<?php
			$sql = sprintf(
				"select o.obsid, o.obsdata, o.obsdescricao, u.usunome, u.usucpf
				from projetos.observacaoatividade o
				inner join seguranca.usuario u on u.usucpf = o.usucpf
				where o.atiid = %d and o.obsstatus = 'A'
				order by o.obsdata desc",
				$_REQUEST['atiid']
			);
			$controles = $db->carregar( $sql );
		?>
		<?php if( is_array( $controles ) ): ?>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="margin-top:20px">
			<tr>
				<td colspan="2" bgcolor="#cdcdcd"><b>Observações</b></td>
			</tr>
			<tr>
				<td colspan="2" style="padding:10px">
					<table class='listagem' style="width:100%">
						<thead>
							<tr>
								<td style="width:10%">&nbsp;</td>
								<td style="width:10%">Data</td>
								<td style="width:50%">Descrição</td>
								<td style="width:30%">Usuário</td>
							</tr>
						</thead>
						<tbody>
						<?php foreach( $controles as $indice => $controle ): ?>
							<tr style="vertical-align:top; background-color: <?= $indice % 2 ? '#e0e0e0' : '#f4f4f4' ?>">
								<td style="padding:5px; text-align:center" nowrap="nowrap">
								<?php if( $controle['usucpf'] == $_SESSION['usucpforigem'] ): ?>
									<img src="../imagens/excluir.gif" onmouseover="this.style.cursor='pointer'" onclick="excluirControle( <?= $controle['obsid'] ?> );"/>
								<?php else: ?>
									&nbsp;
								<?php endif; ?>
								</td>
								<td style="padding:5px; text-align:center"><?= formata_data( $controle['obsdata'] ) ?></td>
								<td style="padding:5px;"><?= $controle['obsdescricao'] ?></td>
								<td style="padding:5px; <?php if( $controle['usucpf'] == $_SESSION['usucpforigem'] ): ?>font-weight:bold<?php endif; ?>"><?= $controle['usunome'] ?></td>
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