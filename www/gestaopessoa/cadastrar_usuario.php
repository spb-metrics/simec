<?php
	/**
	 * Sistema Integrado de Monitoramento do Minist�rio da Educa��o
	 * Setor responsvel: SPO/MEC
	 * Desenvolvedor: Desenvolvedores Simec
	 * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>, Alexandre Soares Diniz
	 * Programadores: Ren� de Lima Barbosa <renedelima@gmail.com>, Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
	 * M�dulo: Seguran�a
	 * Finalidade: Solicita��o de cadastro de contas de usu�rio.
	 * Data de cria��o:
	 * �ltima modifica��o: 30/08/2006
	 */

	// carrega as bibliotecas internas do sistema
	include "config.inc";
	require APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";

	// abre conex�o com o servidor de banco de dados
	$db = new cls_banco();

	$sisid  			 = 64;
	$sisid_gestaopessoas = 64;
	$usucpf 		= $_REQUEST['usucpf'];

	// leva o usu�rio para o passo seguinte do cadastro
	if ($_REQUEST['usucpf'] && $_REQUEST['modulo']) {
		$_SESSION = array();
		header("Location: cadastrar_usuario_2.php?sisid=$sisid&usucpf=$usucpf");
		exit();
	}

?>
<script> 
function ImprimeStatus(texto){ 
    document.formul.numCaracteres.value = texto
} 
</script> 
<!-- 
	Sistema Integrado de Monitoramento do Minist�rio da Educa��o
	Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>, Alexandre Soares Diniz
	Programadores: Ren� de Lima Barbosa <renedelima@gmail.com>, Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
	Finalidade: Solicita��o de cadastro de contas de usu�rio.
-->
<html>
	<head>
		<title>Simec - Minist�rio da Educa��o</title>
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<style type=text/css>
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
			$titulo_modulo = 'Solicita��o de Cadastro de Usu�rios';
			$subtitulo_modulo = 'Preencha os Dados Abaixo e clique no bot�o: "Continuar".<br/>'. obrigatorio() .' Indica Campo Obrigat�rio.'. $mensagens;
			monta_titulo( $titulo_modulo, $subtitulo_modulo );
		?>
		<form method="POST" name="formulario" onsubmit="return validar_formulario(this);">
			<input type=hidden name="modulo" value="./inclusao_usuario.php"/>
			<table width='95%' align='center' border="0" cellspacing="1" cellpadding="3" style="border: 1px Solid Silver; background-color:#f5f5f5;">
				<?php if( $sisid ): ?>
					<tr>
						<td align='right' class="subtitulodireita">&nbsp;</td>
						<td>
							<?php
								$sql = sprintf( "select sisid, sisdsc, sisfinalidade, sispublico, sisrelacionado from sistema where sisid = %d", $sisid );
								$sistema = (object) $db->pegaLinha( $sql );
								if ( $sistema->sisid ) :
							?>
								<font color="#555555" face="Verdana">
									<b><?= $sistema->sisdsc ?></b><br/>
									<p><?= $sistema->sisfinalidade ?></p>
									<ul>
										<li><span style="color: #000000">P�blico-Alvo:</span> <?= $sistema->sispublico ?><br></li>
										<li><span style="color: #000000">Sistemas Relacionados:</span> <?= $sistema->sisrelacionado ?></li>
									</ul>
								</font>
							<?php endif; ?>
						</td>
					</tr>
				<?php endif; ?>
				<input type="hidden" name="sisfinalidade_selc" value="<?=$sisfinalidade_selc?>"/>
				<tr>
					<td align='right' class="subtitulodireita">CPF:</td>
					<td>
						<input id="usucpf" type="text" name="usucpf" size="20" maxlength="14" value=<? print '"'.$usucpf.'"'; ?> class="normal" onKeyUp= "this.value=mascaraglobal('###.###.###-##',this.value);" onmouseover="MouseOver(this);" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="text-align : left; width:22ex;" title=''>
						<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigat�rio.'>
					</td>
				</tr>
				<tr bgcolor="#C0C0C0">
					<td>&nbsp;</td>
					<td>
						<input type="submit" name="btinserir" value="Continuar" />
						&nbsp;&nbsp;&nbsp;
						<input type="Button" value="Cancelar" onclick="location.href='./login.php'"/>
					</td>
				</tr>
			</table>
		</form>
		<br/>
		<?php include "./rodape.php"; ?>
	</body>
</html>
<script language="javascript">

	function selecionar_modulo()
    {
		document.formulario.submit();
	}

	function validar_formulario()
    {
        var validacao = true;
        var mensagem  = '';

        if (document.getElementById('usucpf').value == '') {
            mensagem += '\nInforme o CPF.';
            validacao = false;
        }

        if (document.getElementById('usucpf').value != '' && !validar_cpf(document.getElementById('usucpf').value)) {
            mensagem += '\nO CPF informado n�o � v�lido.';
            validacao = false;
        }

        if ( !validacao ) {
            alert( mensagem );
            return false;
        }

        return true;
	}
</script>