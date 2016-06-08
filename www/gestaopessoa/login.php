<?php

	/**
	 * Sistema Integrado de Planejamento, Orçamento e Finanças do Ministério da Educação
	 * Setor responsvel: DTI/MEC
	 * Desenvolvedor: Desenvolvedores Simec
	 * Autores: Cristiano Cabral <cristiano.cabral@gmail.com>, Adonias Malosso <malosso@gmail.com> 
	 * Módulo: Segurança
	 * Finalidade: Tela de apresentação. Permite que o usuário entre no sistema.
	 * Data de criação: 24/06/2005
	 * Última modificação: 24/08/2008
	 */

	// carrega as bibliotecas internas do sistema
	include "config.inc";
	require APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";

	// abre conexão com o servidor de banco de dados
	$db = new cls_banco();
	//erro
	// executa a rotina de autenticação quando o formulário for submetido
	
	if ( $_POST['formulario'] ) {
		$_SESSION['dmdidavaliacao'] = $_REQUEST['dmdidavaliacao'];	
		include "autenticar.inc";
	}

	if ( $_REQUEST['expirou'] ) {
		$_SESSION['MSG_AVISO'][] = "Sua conexão expirou por tempo de inatividade. Para entrar no sistema efetue login novamente.";
	}

?>
<!-- 
	Sistema Integrado de Planejamento, Orçamento e Finanças do Ministério da Educação
	Autores: Cristiano Cabral <cristiano.cabral@gmail.com>, Adonias Malosso <malosso@gmail.com> 
	Finalidade: Tela de apresentação do sistema. Permite abrir uma sessão no sistema.
-->
<html>
	<head>
		<meta name="description" content="SIMEC - Sistema Integrado de Monitoramento Execução e Controle do Ministério da Educação, Permite o Monitoramento Físico e Financeiro e a Avaliação das Ações e Programas do Ministério dentre outras atividades estratégicas">
		<meta name="keywords" content="SIMEC, MEC, PDE, Ministério da Educação, Analistas: ,Cristiano Cabral, Adonias Malosso, Gilberto Xavier">
		<META NAME="Author" CONTENT="Cristiano Cabral, cristiano.cabral@gmail.com">
		<meta name="audience" content="all">
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Expires" content="-1">
		<title>SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação</title>
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
	<body bgcolor="#ffffff" vlink="#666666" bottommargin="0" topmargin="0" marginheight="0" marginwidth="0" rightmargin="0" leftmargin="0">
		<form name="formulario" method="post">
			
			<?php include "cabecalho.php"; ?>

			<input type="hidden" name="formulario" value="1"/>
			<input type="hidden" name="dmdidavaliacao" value="<?=$_REQUEST['dmdidavaliacao']?>">
			
			<table border="0" height="100%" width='100%' cellspacing=0 cellpadding=0 background='../imagens/back-login.gif'>
				<tr>
					<td valign='top' width="60%">
						<table cellpadding='0' cellspacing='0' border='0' width='100%'>
							<tr><td>&nbsp;</td></tr>
							<tr><td><img src='../imagens/indent2.gif'></td></tr>
							<tr><td>&nbsp;</td></tr>
							<?php
								$sql = "select sisid, sisabrev, sisdsc, sisfinalidade, sispublico, sisrelacionado from seguranca.sistema where sisstatus='A' and sismostra=true and sisid=64 order by sisid";
							?>
							<?php foreach ( $db->carregar( $sql ) as $sistema ) : ?>
								<?php extract( $sistema ); ?>
								<tr>
									<td class="MenuTitulo">
										<img src="../imagens/ico_ajuda.gif" border="0" align="absmiddle"> <b><?= $sisabrev ?></b> - <?= $sisdsc ?><br>
										<font color="#555555" size="1" face="Verdana"><b>Finalidade:</b> <?= $sisfinalidade ?><br>
										<b>Público-Alvo:</b> <?= $sispublico ?><br>
										<b>Sistemas Relacionados:</b> <?= $sisrelacionado ?></font>
										<br/>&nbsp;
									</td>
								</tr>
							<?php endforeach; ?>
						</table>
					</td>
					<td valign='top' width="40%" style="padding-top:50px;">
						<table align="center" cellpadding=0 cellspacing="1" bgcolor=#f5f5f5 style="border:1px solid #888888;">
							<tr>
								<td bgcolor="#294054" style="font-size:8pt;font-family:Tahoma;color:#ffffff;padding:3px;">
									<img src="../imagens/ico_key.gif" border="0" align="absmiddle"> &nbsp;<b>Acesso ao Sistema</b>
								</td>
							</tr>

							<tr>
				            <td nowrap bgcolor="ececec" style="padding-left:10px;padding-right:10px;" width="350">
								<table cellpadding="2" cellspacing="0" align="center" border="0">
									<tr>
										<td colspan="2">
											<?php if ( $_SESSION['MSG_AVISO'] ): 
													if ($_SESSION['MSG_AVISO'][0] == "O cpf informado não está cadastrado." || 
															$_SESSION['MSG_AVISO'][0] == "Você não possui permissão de acesso ao módulo avaliação."){
														header( "Location: cadastrar_usuario_2.php?sisid=64&usucpf=".$_REQUEST['cpf'] );
													}
														?>
												<font color="red">
													<p>
														<ul>
															<li><?= implode( '</li><li>', (array) $_SESSION['MSG_AVISO'] ); ?></li>
														</ul>
													</p>
												</font>
												<hr size="1" color="#cccccc"/>
											<? endif; ?>
											<?php $_SESSION['MSG_AVISO'] = array(); ?>
										</td>
									</tr>
									<tr> 
										<td nowrap align="right" valign="top">CPF:</td>
										<td> 
											<input type="text" name="usucpf" value="" size="20" onkeypress="return controlar_foco_cpf( event );" onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);"  class="normal" onmouseover="MouseOver(this);" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);">
										</td>
									</tr>
									<tr> 
										<td colspan="2" align="center"></td>
									</tr>
									<tr> 
										<td align="right" valign="top">Senha:</td>
										<td> 
											<input type="password" name="ususenha" autocomplete="off" size="20" onkeypress="return controlar_foco_senha( event );" class="normal" onmouseover="MouseOver(this);" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"><br>
											<a href="./recupera_senha.php"><target="_top"><font size="1" face="Arial, sans-serif">Esqueceu sua Senha?</font></a>
										</td>
									</tr>
									<tr>
										<td colspan="2">
											<p style="padding-left: 118px"><input type="button" name="Autenticar" value="Entrar" onclick="enviar_formulario()"></p>
										</td>
									</tr>
									<tr>
										<td>&nbsp;</td>
									</tr>
									<tr>
										<td colspan="2">
											<p style="text-align: center">
												<font size="1" color="red" face="verdana, Arial, sans-serif">Primeiro acesso? Clique no botão abaixo.</font>
											</p>
										</td>
									</tr>
									<tr>
										<td colspan="2">
											<p style="text-align: center">
												<input type="button" value="Solicitar Cadastro" onclick="location.href='cadastrar_usuario.php'">
											</p>
										</td>
									</tr>

									<tr>
										<td colspan="2" align="center">&nbsp;</td>
									</tr>
								</table>
							</td>
						</tr>
					</table>
					<br><br>

<!--
					<table align="center" cellpadding="0" cellspacing="0" width="200">
			        	<tr>
							<td>Apoio:</td>
						</tr>
						<tr><td>&nbsp;</td></tr>
						<tr>
							<td><a href="http://www.abc.gov.br/" target="_blank">ABC - Agência Brasileira de Cooperação</a></td>
						</tr>
						<tr><td>&nbsp;</td></tr>
						<tr>
							<td><a href="http://www.pnud.org.br/" target="_blank">PNUD - Programa das Nações Unidas para o Desenvolvimento</td>
						</tr>
					</table>
-->
				</td>
			</tr>
		</table>
	</tr>

		<?php include "rodape.php"; ?>
		</form>
	</body>
</html>


<script language="javascript">

	if ( document.formulario.usucpf.value == '' ) {
		document.formulario.usucpf.focus();
	} else {
		document.formulario.ususenha.focus();
	}

	function enviar_formulario() {
		if ( validar_formulario() ) {
			document.formulario.submit();
		}
	}

	function validar_formulario() {
		var validacao = true;
		var mensagem = '';
		if ( !validar_cpf( document.formulario.usucpf.value ) ) {
			mensagem += '\nO cpf informado não é válido.';
			validacao = false;
		}
		if ( document.formulario.ususenha.value == "" ) {
			mensagem += '\nÉ necessário preencher a senha.';
			validacao = false;
		}
		if ( !validacao ) {
			alert( mensagem );
		}
		return validacao;
	}

	function controlar_foco_cpf( evento ) {
		if ( window.event || evento.which ) {
			if ( evento.keyCode == 13) {
				return document.formulario.ususenha.focus();
			};
		} else {
			return true;
		}
	}

	function controlar_foco_senha( evento ) {
		if ( window.event || evento.which ) {
			if ( evento.keyCode == 13) {
				return enviar_formulario();
			};
		} else {
			return true;
		}
	}

</script>
<script type="text/javascript">
var gaJsHost = (("https:" == document.location.protocol) ? "https://ssl." : "http://www.");
document.write(unescape("%3Cscript src='" + gaJsHost + "google-analytics.com/ga.js' type='text/javascript'%3E%3C/script%3E"));
</script>
<script type="text/javascript">
try {
var pageTracker = _gat._getTracker("UA-830397-2");
pageTracker._trackPageview();
} catch(err) {}</script>