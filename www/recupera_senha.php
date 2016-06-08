<?php
if(isset($_POST["theme_simec"])){
	$theme = $_POST["theme_simec"];
	setcookie("theme_simec", $_POST["theme_simec"] , time()+60*60*24*30, "/");
} else {
	if(isset($_COOKIE["theme_simec"])){
		$theme = $_COOKIE["theme_simec"];
	}else{
		$theme = 'versao antiga';
	}
}


	/**
	 * Sistema Integrado de Monitoramento do Ministério da Educação
	 * Setor responsvel: SPO/MEC
	 * Desenvolvedor: Desenvolvedores Simec
	 * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
	 * Programadores: Renê de Lima Barbosa <renedelima@gmail.com>
	 * Módulo: Segurança
	 * Finalidade: Permite que o usuário solicite uma nova senha.
	 * Última modificação: 26/08/2006
	 */

	function erro(){
		global $db;
		$db->commit();
		$_SESSION = array();
		$_SESSION['MSG_AVISO'] = func_get_args();
		header( "Location: ". $_SERVER['PHP_SELF'] );
		exit();
	}

	// carrega as bibliotecas internas do sistema
	include "config.inc";
	require APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";

	// abre conexão com o servidor de banco de dados
	$db = new cls_banco();

	if(!$theme) {
		$theme = $_SESSION['theme_temp'];
	}

	// executa a rotina de recuperação de senha quando o formulário for submetido
	if ( $_POST['formulario'] ) {

		// verifica se a conta está ativa
		$sql = sprintf(
			"SELECT u.* FROM seguranca.usuario u WHERE u.usucpf = '%s'",
			corrige_cpf( $_REQUEST['usucpf'] )
		);
		$usuario = (object) $db->pegaLinha( $sql );
		if ( $usuario->suscod != 'A' ) {
			erro( "A conta não está ativa." );
		}

		$_SESSION['mnuid'] = 10;
		$_SESSION['sisid'] = 4;
		$_SESSION['exercicio_atual'] = $db->pega_ano_atual();
		$_SESSION['usucpf'] = $usuario->usucpf;
		$_SESSION['usucpforigem'] = $usuario->usucpf;

		// cria uma nova senha
	    //$senha = $db->gerar_senha();
	    $senha = strtoupper(senha());
		$sql = sprintf(
			"UPDATE seguranca.usuario SET ususenha = '%s', usuchaveativacao = 'f' WHERE usucpf = '%s'",
			md5_encrypt_senha( $senha, '' ),
			$usuario->usucpf
		);
		$db->executar( $sql );

		// envia email de confirmação
		$sql = "select ittemail from public.instituicao where ittstatus = 'A'";
		$remetente = $db->pegaUm( $sql );
		$destinatario = $usuario->usuemail;
		$assunto = "Simec - Recuperação de Senha";
	    $conteudo = sprintf(
	    	"%s %s<br/>Sua senha foi alterada para %s<br>Ao se conectar, altere esta senha para a sua senha preferida.",
	    	$usuario->ususexo == 'F' ?  'Prezada Sra.': 'Prezado Sr.',
	    	$usuario->usunome,
	    	$senha
	    );
		enviar_email( $remetente, $destinatario, $assunto, $conteudo );

		$db->commit();
		$_SESSION = array();
		$_SESSION['MSG_AVISO'][] = "Recuperação de senha concluída. Em breve você receberá uma nova senha por email.";
		header( "Location: /" );
		exit();
	}

	if ( $_REQUEST['expirou'] ) {
		$_SESSION['MSG_AVISO'][] = "Sua conexão expirou por tempo de inatividade. Para entrar no sistema efetue login novamente.";
	}

?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>

<meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1">

<title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>
<?php if(is_file( "includes/layout/".$theme."/include_login.inc" )) include "includes/layout/".$theme."/include_login.inc"; ?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
</head>

<body>
    <?php include "barragoverno.php"; ?>

<table width="100%" cellpadding="0" cellspacing="0" id="main">
<tr>
	<td width="80%" ><img src="/includes/layout/<? echo $theme ?>/img/logo.png" border="0" /></td>
	<td align="right" style="padding-right: 30px;padding-left:20px;" >
		<img src="/includes/layout/<? echo $theme ?>/img/bt_temas.png" style="cursor:pointer" id="img_change_theme" alt="Alterar Tema" title="Alterar Tema" border="0" />

		<div style="display:none" id="menu_theme">
		<script>

			$(document).ready(function() {
			        $().click(function () {
			        	$('#menu_theme').hide();
			        });
			        $("#img_change_theme").click(function () {
			        	$('#menu_theme').show();
			        	return false;
			        });
			        $("#menu_theme").click(function () {
			        	$('#menu_theme').show();
			        	return false;
			        });
			});

			function alteraTema(){
				document.getElementById('formTheme').submit();
			}
		</script>

		<form id="formTheme" action="" method="post" >
		Tema:
			<select class="select_ylw" name="theme_simec" title="Tema do SIMEC" onchange="alteraTema(this.value)" >
		            <?php include(APPRAIZ."www/listaTemas.php") ?>
	        </select>
		</form>
		</div>
	</td>
</tr>
<form method="post" name="formulario">
	<input type=hidden name="formulario" value="1"/>
	<input type=hidden name="modulo" value="./inclusao_usuario.php"/>
<tr>
      <td colspan="2" align="center" valign="top">
      <table width="98%"  border="0" align="center" cellpadding="0" cellspacing="0" class="tabela_modulos">
        <tr>
          <td class="td_bg">&nbsp;Ficha de Solicitação de Cadastro de Usuários</td>
        </tr>
        <tr>
          <td height="106" align="left">

          <!--Caixa de Login-->
          <table border="0" cellspacing="0" cellpadding="3">
				<tr>
					<td style="font-weight: bold;text-align: right;width:150px">CPF:</td>
					<td style="width:250px" >
						<input type="text" name="usucpf" value="" class="login_input" onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);" />
						<?= obrigatorio(); ?>
					</td>
			 	</tr>
				<tr>
					<td></td>
					<td align="left"  >
						<a class="botao2" href="javascript:enviar_formulario()" >Lembrar Senha</a>
						<a class="botao1" href="./login.php" >Voltar</a>
					</td>
				</tr>
          </table>
          <!--fim Caixa de Login -->

          </td>

        </tr>
      </table>
      </td>
  </tr>

	<tr>
	  <td colspan="2" class="rodape"> Data do Sistema: <? echo date("d/m/Y - H:i:s") ?></td>
  </tr>
</table>

</form>

</body>
</html>

<script language="javascript">

	document.formulario.usucpf.focus();

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
		if ( !validacao ) {
			alert( mensagem );
		}
		return validacao;
	}

</script>