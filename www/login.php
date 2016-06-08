<?php
/**
 * Sistema Integrado de Monitoramento, Execução e Controle
 * Setor responsvel: DTI/SE/MEC
 * Autor: Cristiano Cabral <cristiano.cabral@gmail.com>
 * Módulo: Segurança
 * Finalidade: Tela de apresentação. Permite que o usuário entre no sistema.
 * Data de criação: 24/06/2005
 * Última modificação: 24/08/2008
 */

//Verifica Temas
if(isset($_COOKIE["theme_simec"])){
	$theme = $_COOKIE["theme_simec"];
}

 if(isset($_POST["theme_simec"])){
	$theme = $_POST["theme_simec"];
	setcookie("theme_simec", $_POST["theme_simec"] , time()+60*60*24*30, "/");
}else
{
	$theme = "versao antiga";
}

// carrega as bibliotecas internas do sistema
include "config.inc";
require APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

// Valida o CPF, vindo do post
if($_POST['usucpf'] && !validaCPF($_POST['usucpf'])) {
	die('<script>
			alert(\'CPF inválido!\');
			history.go(-1);
		 </script>');
}

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

// executa a rotina de autenticação quando o formulário for submetido
if ( $_POST['formulario'] ) {
	include APPRAIZ . "includes/autenticar.inc";
}

if ( $_REQUEST['expirou'] ) {
	$_SESSION['MSG_AVISO'][] = "Sua conexão expirou por tempo de inatividade. Para entrar no sistema efetue login novamente.";
}


//Define um tema existente (padrão), caso nenhum tenha sido escolhido

if(!$theme) {

	$diretorio = APPRAIZ."www/includes/layout";
	if(is_dir($diretorio)){
		if ($handle = opendir($diretorio)) {
		   while (false !== ($file = readdir($handle))) {
			  if ($file != "." && $file != ".." && $file != ".svn" && is_dir($diretorio."/".$file)) {
				  $dirs[] = $file;
			  }
		   }
		   closedir($handle);
		}
	}

	if($dirs) {
		// sorteia um tema para exibição
		$theme = $dirs[rand(0, (count($dirs)-1))];
		$_SESSION['theme_temp'] = $theme;
	}

}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<!--
/**
*SIMEC - Sistema Integrado de Monitoramento, Execução e Controle
*Setor responsvel: DTI/SE/MEC
*Finalidade: Tela de apresentação do sistema. Permite abrir uma sessão no sistema.
*Autor: Cristiano Cabral <cristiano.cabral@gmail.com>
**/
-->

<meta http-equiv="X-UA-Compatible" content="IE=7" />
<meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1"/>


<title>SIMEC - Sistema Integrado de Monitoramento, Execução e Controle da Presidência da República</title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<?php if(is_file( "includes/layout/".$theme."/include_login.inc" )) include "includes/layout/".$theme."/include_login.inc"; ?>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery.accordion.source.js"></script>
<script src="../includes/BeautyTips/excanvas.js" type="text/javascript"></script>
<script type="text/javascript" src="../includes/BeautyTips/jquery.bt.min.js"></script>
</head>

<body>
	<div id="tutorial_theme" style="display:none"><span style="color:red;font-weight:bold;">Novidade!</span><br/>Agora você pode escolher o VISUAL do seu sistema, clique no ícone ao lado e experimente!</div>

    <?php include "barragoverno.php"; ?>

<table width="100%" cellpadding="0" cellspacing="0" id="main">
<tr>
	<td width="50%" ><br><br><img src="/includes/layout/<? echo $theme ?>/img/logo.png" border="0" /><br><br></td>
	<td align="right" style="padding-right: 30px;padding-left:250px;" >
		<img src="/includes/layout/<? echo $theme ?>/img/bt_temas.png" style="cursor:pointer" id="img_change_theme" alt="Alterar Tema" title="Alterar Tema" border="0" />
		<div style="display:none" id="menu_theme">
		<script>

			$(document).ready(function() {
			        $().click(function () {
			        	$('#menu_theme').hide();
			        });
			        $("#img_change_theme").click(function () {
						$('#img_change_theme').btOff()
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
<form id="formulario" name="formulario" method="post">

<input type="hidden" name="formulario" value="1"/>

<input type="hidden" id="arquivo_login" name="arquivo_login" value="" />

<tr>
  <td width="55%" valign="top">
  <!-- Lista de Módulos-->
  <table width="98%" border="0" cellpadding="0" cellspacing="0" class="tabela_modulos">
  <tr>
  	<td class="td_bg">&nbsp;Módulos - <small> lista de módulos</small></td>
  </tr>
  <tr>
	<td valign="middle" class="td_table_inicio">
    <div id="pageWrap" class="pageWrap">
	    <ul class="accordion">
		<?
		// buscando a lista de sistemas
		$sql = "SELECT sisid, sisabrev, sisdsc, sisfinalidade, sispublico, sisrelacionado
				FROM seguranca.sistema
				WHERE sisstatus='A' AND sismostra=true
				ORDER BY sisid";
		?>
		<? foreach ( $db->carregar( $sql ) as $sistema ) : ?>
		<? extract( $sistema ); ?>
		<li>
			<a href="javascript:void(0)" class="link"><span class="txt_azul_bold"><?= $sisabrev ?></span> - <?= $sisdsc ?></a>
			<div style="width:95%">
            <table width="100%" border="0" style="cursor: default" align="center" cellpadding="2" cellspacing="0">
				<tr>
					<td valign="top" width="24%" align="right" class="txt_laranja txt_padrao">Finalidade:</td>
					<td style="text-align: justify;" class="txt_padrao" valign="top" width="76%"><?= $sisfinalidade ?></td>
					<td rowspan="3" valign="top" align="right">
		               	<?if (montaLinkManual2($sisid)){?>
						<div class="botao1"><?= montaLinkManual2($sisid) ?></div>
						<?}?>
		               	<a href="javascript:janela('/geral/fale_conosco.php?sisid=<?= $sisid; ?>',550,600)" class="botao1">Dúvidas</a>
		               	<a href="cadastrar_usuario.php?sisid=<? echo $sisid; ?>" class="botao2">Solicitar Cadastro</a>
	               </td>
				</tr>
				<tr>
					<td valign="top" align="right" class="txt_laranja txt_padrao">P&uacute;blico-Alvo:</td>
					<td valign="top" class="txt_padrao" ><?= $sispublico ?></td>
				</tr>
				<tr>
					<td valign="top" align="right" class="txt_laranja txt_padrao">Sistemas Relacionados:</td>
					<td valign="top" class="txt_padrao" ><?= $sisrelacionado ?></td>
				</tr>
            </table>
		</li>
		<?php endforeach; ?>
		</ul>
	</div>
    </td>

  </tr>
  </table>
  </td>

      <td width="30%" align="center" valign="top">
      <table width="92%" border="0" align="center" cellpadding="0" cellspacing="0" class="tabela_modulos">
        <tr>
          <td class="td_bg">&nbsp;Acesse o Sistema</td>
        </tr>
        <tr>
          <td height="106" align="center">
		  <? if ( $_SESSION['MSG_AVISO'] ): ?>
		  <div class="error_msg">
		  <ul><li><?= implode( '</li><li>', (array) $_SESSION['MSG_AVISO'] ); ?></li></ul>
		  </div>
		  <? endif; ?>
		  <? $_SESSION['MSG_AVISO'] = array(); ?>
          <!--Caixa de Login-->
          <table class="tbl_login" width="95%" border="0" cellspacing="0" cellpadding="3">
            <tr>
              <td style="font-weight: bold;" valign="middle"  width="13%" align="right">
              	<div style="margin-bottom:15px" >CPF:</div>
              	<div>SENHA:</div>
              </td>
              <td valign="middle" width="51%">
              	<input style="margin-bottom:10px"  type="text" name="usucpf" value="" size="20" class="login_input" onkeypress="return controlar_foco_cpf( event );" onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);" /> <br />
              	<input type="password" name="ususenha" class="login_input" autocomplete="off" size="20" onkeypress="return controlar_foco_senha( event );" />
              </td>
              <td valign="middle" width="36%">
              	<a class="botao2" href="javascript:enviar_formulario()" >Entrar</a>
              	<a href="./cadastrar_usuario.php" class="botao1">Solicitar Cadastro</a>
              </td>
            </tr>
            <tr>
              <td colspan="3" align="left" class="txt_laranja" ><a class="link_laranja" href="recupera_senha.php" >Esqueceu a senha?</a></td>
            </tr>
          </table>
          <!--fim Caixa de Login -->

          </td>

        </tr>
        <tr>
          <td class="td_bg">&nbsp;Sobre</td>
        </tr>
        <tr>
          <td height="115" align="center"><div id="premios">
			<p>O <?php echo $GLOBALS['parametros_sistema_tela']['sigla']; ?> é desenvolvido sobre a platoforma SIMEC cedida pela Secretaria Executiva do Ministério da Educação DTI/SE/MEC.</p>		
  <p>O sistema SIMEC é vencedor dos principais prêmios de sistemas de gestão pública do Brasil</p>
          	<a target="_blank" href="http://inovacao.enap.gov.br/index.php?option=com_content&task=blogcategory&id=51&Itemid=57" ><img style="cursor:pointer" src="/imagens/logo/selo-inovacao.gif" border="0" /></a>
          	<a target="_blank" href="http://www.premio-e.gov.br/vencedores2009.asp" ><img style="cursor:pointer" src="/imagens/logo/premioe-gov.png" border="0" /></a>
			<a target="_blank" href="http://www.conip.com.br" ><img style="cursor:pointer" src="/imagens/logo/conip.gif" border="0" /></a>
          </td>
        </tr>
<!--
        <tr>
          <td class="td_bg">&nbsp;Informes</td>
        </tr>
		<tr>
          <td height="115">
			<div id="informes">

					  Acrescentar textos de informes


			</div>
          </td>
        </tr>
 -->

      </table>
      </td>
  </tr>

	<tr>
	  <td colspan="2" class="rodape"><table width="100%" border="0" cellpadding="2" cellsapcing="0">
	  	<tr>
	  		<td>Data do Sistema: <? echo date("d/m/Y - H:i:s") ?></td>
	  		<td align="right">
	  		<!--<a href="http://www.websis.com.br" target="_blank"><img src="imagens/logo_inv2.png" alt="Versão Desenvolvida e distribuída por Websis Tecnologia & Sistemas" title="Versão Desenvolvida e distribuída por Websis Tecnologia & Sistemas" border="0" align="absmiddle"/></a>--></td>
	  			</tr></table>
  </tr>
</table>

</form>

</body>
</html>


<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>

<script language="javascript">

	$('#img_change_theme').bt({
  		trigger: 'none',
  		contentSelector: "$('#tutorial_theme')",
  		width: 200,
  		shadow: true,
	    shadowColor: 'rgba(0,0,0,.5)',
	    shadowBlur: 8,
	    shadowOffsetX: 4,
	    shadowOffsetY: 4
	});

$(document).ready(function () {
	$('#img_change_theme').btOn();
	window.setTimeout("$('#img_change_theme').btOff()", 10000);
});

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

		//limpa variavel de download
		var arquivo = document.getElementById("arquivo_login");
		arquivo.value = "";

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

	function abreArquivo(arq)
	{
		var form	= document.getElementById("formulario");
		var arquivo = document.getElementById("arquivo_login");

		arquivo.value = arq;
		form.submit();
	}

	/*** INICIO SHOW MODAL ***/
	function montaShowModal() {
		var alert='';
		alert += '<p align=center style=font-size:15;><font size=4 color=red><b>Atenção!</b></font><br>Seu navegador de internet está ultrapassado.<br/><br/>Em breve vamos descontinuar o suporte para Internet Explorer 6 e versões anteriores.<strong><br/><br/> Atualize seu navegador para obter uma experiência on-line mais rica, sugerimos algumas opções para download nos links abaixo:</strong></p>';
		alert += '<p><a target=_blank href=http://www.google.com/chrome/index.html?brand=CHNY&amp;utm_campaign=en&amp;utm_source=en-et-youtube&amp;utm_medium=et><img src=../imagens/browsers_chrome.png border=0></a> <a target=_blank href=http://www.microsoft.com/windows/internet-explorer/default.aspx><img src=../imagens/browsers_ie.png border=0></a> <a target=_blank href=http://www.mozilla.com/?from=sfx&amp;uid=267821&amp;t=449><img src=../imagens/browsers_firefox.png border=0></a></p>';
		alert += '<p align=center><input type=button value=Fechar onclick=closeMessage();></p>';
		displayStaticMessage(alert,false,'280');
		return false;
	}

	function displayStaticMessage(messageContent,cssClass,height) {
		messageObj = new DHTML_modalMessage();	// We only create one object of this class
		messageObj.setShadowOffset(5);	// Large shadow
		messageObj.setHtmlContent(messageContent);
		messageObj.setSize(570,height);
		messageObj.setCssClassMessageBox(cssClass);
		messageObj.setSource(false);	// no html source since we want to use a static message here.
		messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes
		messageObj.display();
	}

	function closeMessage() {
		messageObj.close();
	}
	/*** FIM SHOW MODAL ***/

</script>

<?php
// verificando se o browser é IE6 ou inferior
require APPRAIZ . "includes/classes/browser.class.inc";
$browser = new Browser();
if( $browser->getBrowser() == Browser::BROWSER_IE && $browser->getVersion() <= 6 ) {
	?>
		<script>montaShowModal();</script>
	<?
}
?>
