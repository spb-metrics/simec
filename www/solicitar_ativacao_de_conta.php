<?php

	/**
	 * Sistema Integrado de Monitoramento do Minist�rio da Educa��o
	 * Setor responsvel: SPO/MEC
	 * Desenvolvedor: Desenvolvedores Simec
	 * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>, Alexandre Soares Diniz
	 * Programadores: Ren� de Lima Barbosa <renedelima@gmail.com>
	 * M�dulo: Autentica��o
	 * Finalidade: Permite que o usu�rio solicite justificadamente a ativa��o da sua conta.
	 * Data de cria��o: 24/06/2005
	 * �ltima modifica��o: 29/08/2006
	 */

	// carrega as bibliotecas internas do sistema
	include "config.inc";
	require APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";

	// abre conex�o com o banco de dados
	$db = new cls_banco();

	$usucpf = $_REQUEST['usucpf'];
	$sisid = $_REQUEST['sisid'];

	if ( $_REQUEST['formulario'] ) {
		$cpf = corrige_cpf( $_POST['usucpf'] );
		$justificativa = trim( $_POST['htudsc'] );
		
		// carrega os dados do usu�rio
		$sql = sprintf(
			"SELECT u.usucpf, u.suscod FROM seguranca.usuario u WHERE u.usucpf = '%s'",
			$cpf
		);
		$usuario = (object) $db->recuperar( $sql );
		
		// atribui��es requeridas para que a auditoria do sistema funcione
		$_SESSION['sisid'] = 4; # seleciona o sistema de seguran�a
		$_SESSION['usucpf'] = $usuario->usucpf;
		$_SESSION['usucpforigem'] = $usuario->usucpf;
		$_SESSION['superuser'] = $db->testa_superuser( $usuario->usucpf );
		
		$descricao = "Usu�rio solicitou a ativa��o da conta e apresentou a seguinte justificativa: ". $justificativa;
		if ( $usuario->usucpf ) {
			if ( $sisid ) {
				$sql = sprintf(
					"SELECT us.* FROM seguranca.usuario_sistema us WHERE us.sisid = %d AND us.usucpf = '%s'",
					$sisid,
					$usuario->usucpf
				);
				$usuario_sistema = (object) $db->pegaLinha( $sql );
				if ( $usuario_sistema->suscod == 'B' ) {
					$db->alterar_status_usuario( $cpf, 'P', $descricao, $usuario_sistema->sisid );
				}
			} else if ( $usuario->suscod == 'B' ) {
				$db->alterar_status_usuario( $cpf, 'P', $descricao );
			}
		}
		$db->commit();
		$_SESSION['MSG_AVISO'] = array( "Seu pedido foi submetido e ser� avaliado em breve." );
		
		header( "Location: login.php" );
		exit();
	}

?>
<!-- 
	Sistema Integrado de Monitoramento do Minist�rio da Educa��o
	Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>, Alexandre Soares Diniz
	Programadores: Ren� de Lima Barbosa <renedelima@gmail.com>
	Finalidade: Permite que o usu�rio solicite justificadamente a ativa��o da sua conta.
-->
<html>
	<head>
		<title>Simec - Minist�rio da Educa��o</title>
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<style type=text/css>
			.MenuTitulo {
			    FONT-SIZE: 10pt;
			    VERTICAL-ALIGN: baseline;
			    COLOR: black;
			    FONT-FAMILY: Tahoma;
			    TEXT-ALIGN: justify;
			    TEXT-DECORATION: none;
			    PADDING-LEFT: 40px;
			    PADDING-BOTTOM: 10px;
			}
			form {
				margin: 0px;
			}
		</style>
	</head>
	<body bgcolor=#ffffff vlink=#666666 bottommargin="0" topmargin="0" marginheight="0" marginwidth="0" rightmargin="0" leftmargin="0">
		<?php include "cabecalho.php"; ?>
		<br/>
		<?php
			$mensagens = '<p style="align: center; color: red; font-size: 12px">'. implode( '<br/>', (array) $_SESSION['MSG_AVISO'] ) . '</p>';
			$_SESSION['MSG_AVISO'] = null;
			$titulo_modulo = 'Solicita��o de Ativa��o Conta';
			$subtitulo_modulo = 'Preencha os Dados Abaixo e clique no bot�o: "Enviar Solicita��o".<br/>'. obrigatorio() .' Indica Campo Obrigat�rio.'. $mensagens;
			monta_titulo( $titulo_modulo, $subtitulo_modulo );
		?>
		<form method="POST" name="formulario">
			<input type=hidden name="formulario" value="1"/>
			<table width='95%' align='center' border="0" cellspacing="1" cellpadding="3" style="border: 1px Solid Silver; background-color:#f5f5f5;">
				<?php if ( $_REQUEST['sisid'] ): ?>
			 	<tr>
			    	<td align='right' class="subtitulodireita">CPF:</td>
					<td>
						<?= campo_texto( 'usucpf', 'N', 'N', '', 19, 14, '###.###.###-##', '' ); ?>
					</td>
				</tr>
			 	<tr>
			    	<td align='right' class="subtitulodireita">Sistema:</td>
					<td>
						<?php
							$sql = "select s.sisid as codigo, s.sisabrev as descricao from seguranca.sistema s where s.sisstatus='A' and sismostra='t'";
							$db->monta_combo( 'sisid', $sql, 'N', '&nbsp;', '', '' );
						?>
						<?= obrigatorio(); ?>
					</td>
				</tr>
				<?php else: ?>
			 	<tr>
			    	<td align='right' class="subtitulodireita">CPF:</td>
					<td>
						<input type="text" name="usucpf" value="" size="20" onkeypress="return entra0(event);" onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);"  class="normal" onmouseover="MouseOver(this);" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" >
						<?= obrigatorio() ?>
					</td>
				</tr>
				<?php endif; ?>
			 	<tr>
			    	<td align='right' class="subtitulodireita">Justificativa:</td>
					<td>
						<textarea name="htudsc" cols="100" rows="3" onmouseover="MouseOver(this);" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="width: 100ex;"><?= $observacao ?></textarea>
						<?= obrigatorio() ?>
					</td>
				</tr>
				<tr bgcolor="#C0C0C0">
			 		<td>&nbsp;</td>
			   		<td>
			   			<input type="button" name="btinserir" value="Enviar Solicita&ccedil;&atilde;o"  onclick="enviar_formulario()"/>
			   			&nbsp;&nbsp;&nbsp;
			   			<input type="Button" value="Voltar" onclick="history.back();"/>
			   		</td>
				</tr>
			</table>
		</form>
		<br/>
		<?php include "./rodape.php"; ?>
	</body>
</html>
<script language="javascript">

	function enviar_formulario() {
		if ( validar_formulario() ) {
			document.formulario.submit();
		}
	}

	function validar_formulario() {
		var validacao = true;
		var mensagem = '';
		if ( document.formulario.usucpf.value == "" ) {
			mensagem += '\nInforme o cpf.';
			validacao = false;
		}
		if ( document.formulario.htudsc.value == "" ) {
			mensagem += '\nVoc� deve justificar o pedido.';
			validacao = false;
		}
		if ( !validacao ) {
			alert( mensagem );
		}
		return validacao;
	}

</script>