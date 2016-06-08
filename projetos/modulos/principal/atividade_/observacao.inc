<?php

$atividade = atividade_pegar( $_REQUEST['atiid'] );
if ( !$atividade ) {
	redirecionar( 'principal/atividade_/arvore', 'A' );
}

$parametros = array(
	'aba' => $_REQUEST['aba'], # mantém a aba ativada
	'atiid' => $_REQUEST['atiid'] 
);

// impede que usuário sem permissão acione eventos
if ( $_REQUEST['evento'] && !atividade_verificar_responsabilidade( $_REQUEST['atiid'], $_SESSION['usucpf'] ) ) {
	redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
}

switch( $_REQUEST['evento'] ){

	case 'cadastrar_nota':
		$sql = sprintf(
			"insert into projetos.notaatividade ( notdescricao, atiid, usucpf ) values ( '%s', %d, '%s' )",
			$_REQUEST['notdescricao'],
			$_REQUEST['atiid'],
			$_SESSION['usucpf']
		);
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
		} else {
			$db->commit();
		}
		redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
		break;

	case 'excluir_nota':
		$sql = sprintf(
			"update projetos.notaatividade set notstatus = 'I' where notid = %d",
			$_REQUEST['notid']
		);
		if( !$db->executar( $sql ) ){
			$db->rollback();
		} else {
			$db->commit();
		}
		redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
		break;

	default:
		break;

}

$permissao = atividade_verificar_responsabilidade( $atividade['atiid'], $_SESSION['usucpf'] );
$permissao_formulario = $permissao ? 'S' : 'N'; # S habilita e N desabilita o formulário

// VERIFICA SE PROJETO ESTÁ SELECIONADO
projeto_verifica_selecionado();

// CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '&atiid=' . $atividade['atiid']  );
montar_titulo_projeto( $atividade['atidescricao'] );

extract( $atividade ); # mantém o formulário preenchido
?>
<script language="JavaScript" src="../includes/calendario.js"></script>
<script language="javascript" type="text/javascript">
	
	function cadastrar_nota(){
		if ( validar_formulario_nota() ) {
			document.nota.submit();
		}
	}
	
	function validar_formulario_nota(){
		var validacao = true;
		var mensagem = 'Os seguintes campos não foram preenchidos:';
		document.nota.notdescricao.value = trim( document.nota.notdescricao.value );
		if ( document.nota.notdescricao.value == '' ) {
			mensagem += '\nConteúdo';
			validacao = false;
		}
		if ( !validacao ) {
			alert( mensagem );
		}
		return validacao;
	}
	
	function excluirNota( nota ){
		if ( confirm( 'Deseja excluir a observação?' ) ) {
			window.location = '?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&atiid=<?= $_REQUEST['atiid'] ?>&aba=<?= $_REQUEST['aba'] ?>&evento=excluir_nota&notid='+ nota;
		}
	}

	function filtrar_restricoes( filtro ){
		if ( filtro ) {
			window.location = '?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&atiid=<?= $_REQUEST['atiid'] ?>&aba=<?= $_REQUEST['aba'] ?>';
		} else {
			window.location = '?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&atiid=<?= $_REQUEST['atiid'] ?>&aba=<?= $_REQUEST['aba'] ?>&filtro=1';
		}
	}

	function enviar_email( cpf ){
		var nome_janela = 'janela_enviar_emai_' + cpf;
		window.open(
			'/geral/envia_email.php?cpf=' + cpf,
			nome_janela,
			'width=650,height=557,scrollbars=yes,scrolling=yes,resizebled=yes'
		);
	}

</script>

<script language="javascript" type="text/javascript">
	
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
			<!-- NOVO CONTROLE -->
			<?php if( $permissao ): ?>
				<form method="post" name="nota">
					<input type="hidden" name="evento" value="cadastrar_nota"/>
					<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 100%;">
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Descrição:</td>
							<td><?= campo_textarea( 'notdescricao', 'S', 'S', '', 70, 2, 250 ); ?></td>
						</tr>
						<tr style="background-color: #cccccc">
							<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
							<td><input type="button" name="botao" value="Salvar" onclick="cadastrar_nota();"/></td>
						</tr>
					</table>
				</form>
			<?php endif; ?>
			
			<!-- LISTA DE RESTRIÇÕES -->
			<?php
				$sql = sprintf(
					"select
						n.notid, n.notdata, n.notdescricao,
						usu.usucpf, usu.usunome, usu.usufoneddd, usu.usufonenum,
						uni.unidsc
					from projetos.notaatividade n
					left join seguranca.usuario usu on
						usu.usucpf = n.usucpf
					left join public.unidade uni on
						uni.unicod = usu.unicod
					where
						n.atiid = %d and n.notstatus = 'A'
					order by n.notdata desc",
					$_REQUEST['atiid']
				);
				$lista = $db->carregar( $sql );
			?>
			<?php if( is_array( $lista ) ): ?>
				<?php foreach( $lista as $item ): ?>
				<form method="post" id="formulario_<?= $item['obsid'] ?>">
					<input type="hidden" name="evento" value="controlar_solucao"/>
					<input type="hidden" name="obsid" value="<?= $item['obsid'] ?>"/>
					<table id="<?= $item['obsid'] ?>" class='tabela' bgcolor="#f5f5f5" style="width:100%; margin-top: 15px;<?= $item['obssolucao'] == 't' ? 'color:#454545;' : '' ?>" cellpadding="3">
						<tbody>
							<tr style="background-color: #cccccc">
								<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
								<td align='left' style="vertical-align:top;"><b>Observação</b></td>
							</tr>
							<tr>
								<td class="SubTituloDireita" style="vertical-align:top; width:25%;">Descrição:</td>
								<td style="font-weight:bold;">
									<?= $item['notdescricao'] ?>
								</td>
							</tr>
							<tr>
								<td class="SubTituloDireita" style="vertical-align:top; width:25%;">Data:</td>
								<td><?= formata_data( $item['notdata'] ); ?></td>
							</tr>
							<tr>
								<td class="SubTituloDireita" style="vertical-align:top; width:25%;">Autor:</td>
								<td>
									<div>
										<img onclick="enviar_email( '<?= $item['usucpf'] ?>' );" title="enviar e-mail" src="../imagens/email.gif" align="absmiddle" style="border:0; cursor:pointer;"/>
										<?= $item['usunome'] ?>
									</div>
									<div style="color:#959595;"><?= $item['usunome'] ?> - Tel: (<?= $item['usufoneddd'] ?>) <?= $item['usufonenum'] ?></div>
								</td>
							</tr>
							<?php if( $permissao ): ?>
								<tr style="background-color: #cccccc">
									<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
									<td>
										<input type="button" name="botao" value="Excluir" onclick="excluirNota( <?= $item['notid'] ?> );" />
									</td>
								</tr>
							<?php endif; ?>
						</tbody>
					</table>
				</form>
				<?php endforeach; ?>
			<?php else: ?>
				<table class='tabela' style="width:100%;" cellpadding="3">
					<tbody>
						<td style="text-align:center;padding:15px;background-color:#f5f5f5;">
							A atividade não possui observações.
						</td>
					</tbody>
				</table>
			<?php endif; ?>
		</td>
	</tr>
</table>