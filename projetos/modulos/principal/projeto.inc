<?php

// captura dados do projeto da requisição
$atiid = $_REQUEST['atiid'] ? (integer) $_REQUEST['atiid'] : PROJETO;

// captura as inforamções da atividade
$atividade = atividade_pegar( $atiid );
if ( !$atividade ) {
	redirecionar( $_SESSION['paginainicial'], null );
}

$permissao = projeto_verificar_responsabilidade( $atividade['atiid'], $_SESSION['usucpf'] );
$permissao_formulario = $permissao ? 'S' : 'N'; # S habilita e N desabilita o formulário

// efetiva a alteração dos dados
if ( $_REQUEST['gravar'] ) {
	$sql = sprintf(
		"update projetos.atividade set atidescricao = '%s', atidetalhamento = '%s' where atiid = %d",
		$_REQUEST['atidescricao'],
		$_REQUEST['atidetalhamento'],
		$_REQUEST['atiid']
	);
	if ( $db->executar( $sql ) ) {
		$_REQUEST['apoio'] = array_diff( $_REQUEST['apoio'], $_REQUEST['gerente'] ); # impede que os usuários redebam atribuição dupla de responsabilidade
		if ( !empty( $_REQUEST['gerente'] ) ) {
			atividade_atribuir_responsavel( $_REQUEST['atiid'], PERFIL_GESTOR, $_REQUEST['gerente'] );
		}
		atividade_atribuir_responsavel( $_REQUEST['atiid'], PERFIL_EQUIPE_APOIO_GESTOR, $_REQUEST['apoio'] );
		$db->commit();
		$db->sucesso( $_REQUEST['modulo'], '&atiid='. $_REQUEST['atiid'] );
	} else {
		$db->rollback();
		$db->insucesso( $_REQUEST['modulo'], '&atiid='. $_REQUEST['atiid'] );
	}
	exit();
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '&nbsp;' );

extract( $atividade ); # mantém o formulário preenchido

?>
<script type="text/javascript">

	function cancelar() {
		window.location.href = '?modulo=<?= $_SESSION['paginainicial'] ?>';
	}
	
	function enviar(){
		if ( document.formulario.atidescricao.value == '' ) {
			alert( 'Preencha o campo Descrição' );
			document.formulario.atidescricao.focus();
			return;
		}
		selectAllOptions( formulario.gerente );
		selectAllOptions( formulario.apoio );
		document.formulario.submit();
	}

</script>
<form action="" method="post" name="formulario">
	<input type="hidden" name="atiid" value="<?= $atiid ?>"/>
	<input type="hidden" name="gravar" value="1"/>
	<table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Título:</td>
			<td><?= campo_textarea( 'atidescricao', 'S', $permissao_formulario, '', 70, 3, 500 ); ?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Descrição:</td>
			<td><?= campo_textarea( 'atidetalhamento', 'N', $permissao_formulario, '', 70, 10, '' ); ?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Gestor:</td>
			<td>
				<?php
					$sql_gerente = "
						select
							u.usucpf as codigo,
							u.usucpf || ' - ' || u.usunome as descricao
						from seguranca.usuario u
							inner join projetos.usuarioresponsabilidade ur on
								ur.usucpf = u.usucpf
							inner join seguranca.perfilusuario pu on pu.pflcod = ur.pflcod and pu.usucpf = ur.usucpf
						where
							ur.rpustatus = 'A' and
							ur.pflcod = '" . PERFIL_GESTOR . "' and
							atiid = " . $atividade['atiid'] . "
						order by u.usucpf
					";
					$gerente = $db->carregar( $sql_gerente );
					$gerente = $gerente ? $gerente : array();
					$sql_combo = "
						select
							u.usucpf as codigo,
							u.usucpf || ' - ' || u.usunome as descricao
						from seguranca.usuario u
						inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf  
						where
							( u.suscod = 'A' or u.suscod = 'P' ) and
							us.suscod = 'A' and
							us.sisid = ". $_SESSION['sisid'] ."
						group by u.usucpf, u.usunome"; 
					combo_popup(
						'gerente',
						$sql_combo,
						'Selecione o(s) Identificador(es) de Uso',
						'400x400',
						1,
						array(),
						'',
						$permissao_formulario,
						true,
						false,
						3
					);
				?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Equipe de Apoio:</td>
			<td>
				<?php
					$sql_apoio = "
						select
							u.usucpf as codigo,
							u.usucpf || ' - ' || u.usunome as descricao
						from seguranca.usuario u
							inner join projetos.usuarioresponsabilidade ur on
								ur.usucpf = u.usucpf
							inner join seguranca.perfilusuario pu on pu.pflcod = ur.pflcod and pu.usucpf = ur.usucpf
						where
							ur.rpustatus = 'A' and
							ur.pflcod = '" . PERFIL_EQUIPE_APOIO_GESTOR . "' and
							atiid = " . $atividade['atiid'] . "
						order by u.usucpf
					";
					$apoio = $db->carregar( $sql_apoio );
					$apoio = $apoio ? $apoio : array();
					$sql_combo2 = "
						select
							u.usucpf as codigo,
							u.usucpf || ' - ' || u.usunome as descricao
						from seguranca.usuario u
						inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf  
						where
							( u.suscod = 'A' or u.suscod = 'P' ) and
							( us.suscod = 'A' or us.suscod = 'P' ) and
							us.sisid = ". $_SESSION['sisid'] ."
						group by u.usucpf, u.usunome";
					combo_popup(
						'apoio',
						$sql_combo2,
						'Selecione o(s) Identificador(es) de Uso',
						'400x400',
						0,
						array(),
						'',
						$permissao_formulario,
						true
					);
				?>
			</td>
		</tr>
		<?php if( $permissao ): ?>
		<tr style="background-color: #cccccc">
			<td align='right' style="vertical-align:top; width:25%;">&nbsp;</td>
			<td>
				<input type="button" name="botao_gravar" value="Gravar" onclick="enviar();"/>
				<input type="button" name="botao_cancelar" value="Cancelar" onclick="cancelar();"/>
			</td>
		</tr>
		<?php endif; ?>
	</table>
</form>