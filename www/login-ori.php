<?php


// Faz download de um dos arquivos solicitados
if( $_REQUEST["arquivo_login"] )
{
	// caminho do arquivo
	$path = "./";
	// recupera o nome e o tipo do arquivo
	switch($_REQUEST["arquivo_login"])
	{
		/*
		case 'lista':
			$file = "lista_de_escolas_pdde.pdf";
			$type = "application/pdf";
			break;
		case 'manual':
			$file = "manual_de_orientacao_pdde.pdf";
			$type = "application/pdf";
			break;
		*/
		case 'pesquisa':
			$file = "pesquisa_educacao_campo.pdf";
			$type = "application/pdf";
			break;

	}

	// caminho completo
	$file = $path . $file;
	// cabeçalho
	header("Content-type: $type");
	header("Content-Disposition: attachment;filename=$file");
	// mostra o download
	readfile($file);
	// destrói a variável do formulário
	unset($_REQUEST["formulario"]);
	exit;
}

/**
 * Sistema Integrado de Planejamento, Orçamento e Finanças do Ministério da Educação
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
	if(AUTHSSD) {
		include APPRAIZ . "includes/autenticarssd.inc";
	} else {
		include APPRAIZ . "includes/autenticar.inc";
	}
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
<!--
	Sistema Integrado de Monitoramento, Execução e Controle
	Setor responsvel: DTI/SE/MEC
	Finalidade: Tela de apresentação do sistema. Permite abrir uma sessão no sistema.
	Autor: Alexandre Dourado
-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="X-UA-Compatible" content="IE=7" />
<meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1">


<title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<?php if(is_file( "includes/layout/".$theme."/include_login.inc" )) include "includes/layout/".$theme."/include_login.inc"; ?>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery.accordion.source.js"></script>
<script src="../includes/BeautyTips/excanvas.js" type="text/javascript"></script>
<script type="text/javascript" src="../includes/BeautyTips/jquery.bt.min.js"></script>


</head>

<body>
	<div id="tutorial_theme" style="display:none"><span style="color:red;font-weight:bold;">Novidade!</span><br>Agora você pode escolher o VISUAL do seu SIMEC, clique no ícone ao lado e experimente!</div>
	<? include "barragoverno.php"; ?>

<table width="100%" cellpadding="0" cellspacing="0" id="main">
<tr>
	<td width="50%" ><img src="/includes/layout/<? echo $theme ?>/img/logo.png" border="0" /></td>
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
              <td style="font-weight: bold;"  width="13%" align="right">
              	CPF:
              </td>
              <td width="51%">
              	<input tabindex="1"  type="text" name="usucpf" value="" size="20" class="login_input" onkeypress="return controlar_foco_cpf( event );" onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);" />
              </td>
              <td width="36%">
              	<a tabindex="3" class="botao2" href="javascript:enviar_formulario()" >Entrar</a>
              </td>
            </tr>
            <tr>
              <td style="font-weight: bold;" valign="middle"  width="13%" align="right">
              	SENHA:
              </td>
              <td valign="middle" width="51%">
              	<input tabindex="2" type="password" name="ususenha" class="login_input" autocomplete="off" size="20" onkeypress="return controlar_foco_senha( event );" />
              </td>
              <td valign="middle" width="36%">
              	<a tabindex="4" href="./cadastrar_usuario.php" class="botao1">Solicitar Cadastro</a>
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
          <td class="td_bg">&nbsp;Prêmios</td>
        </tr>
        <tr>
          <td height="115" align="center"><div id="premios">&nbsp;
          	<a target="_blank" href="http://inovacao.enap.gov.br/index.php?option=com_content&task=blogcategory&id=51&Itemid=57" ><img style="cursor:pointer" src="/imagens/logo/selo-inovacao.gif" border="0" /></a>
          	<a target="_blank" href="http://www.premio-e.gov.br/vencedores2009.asp" ><img style="cursor:pointer" src="/imagens/logo/premioe-gov.png" border="0" /></a>
			<a target="_blank" href="http://www.conip.com.br" ><img style="cursor:pointer" src="/imagens/logo/conip.gif" border="0" /></a>
          </td>
        </tr>
        <tr>

          <td class="td_bg">&nbsp;Informes</td>
        </tr>
		<tr>
          <td height="115">
			<div id="informes">
						
					   <p align="left">
							<table border=0 cellspacing=0 cellpadding=0 width='100%' >
							 <tr >
							  <td align="center" width="90"  style='border:solid windowtext 1.0pt;padding:0cm 5.4pt 0cm 5.4pt;height:61.85pt'>
							  	<img border=0 width="96" height="79" src="/imagens/st.gif">
							  </td>
							  <td align="center" width=480 style='width:360.0pt;border:solid windowtext 1.0pt;border-left:none;padding:0cm 5.4pt 0cm 5.4pt;height:101.85pt;border-color:-moz-use-text-color'>
								  <b><span style='font-size:11.0pt;line-height:200%;font-family:"Arial","sans-serif";color:navy'>
								  	3ª&nbsp;&nbsp;VIDEOCONFERÊNCIA - 2011
								  </span></b>
								  <br>
								  <b><span style='font-size:11.0pt;font-family:"Arial","sans-serif";color:black'>
								  	PST / Mais Educação
									<br><br>
									<font color="navy">28/06/2011</font>
									<br>
									<font color="black">14h30 às 17h30</font>
								  </span></b>
								  <br>
								  <br>
								  <b><u><span style='font-family:"Arial","sans-serif";color:red'><!--<a href="javascript:void(0);" onclick="montaSegTempo();">Clique aqui</a>--><a href="http://www.esporte.gov.br/snee/segundotempo/maiseducacao/videoconferencia32011.jsp" target="_blank"><font color='red'>Clique aqui para mais informações</font></a></span></u></b>
							  </td>
							 </tr>
							</table>
					   </p>
					   

					    <p align="left">
							<table border=0 cellspacing=0 cellpadding=0 width='100%' >
							 <tr >
							  <td align="center" width=480 style='width:360.0pt;border:solid windowtext 1.0pt; padding:0cm 5.4pt 0cm 5.4pt;height:71.85pt;border-color:-moz-use-text-color'>
								    <br>
								    <p align="left"><div style="float:left"><img src="/imagens/seta_galeria1.gif" align="bottom">&nbsp;</div> <div style="float:left"> <b>ESCOLA - MAIS EDUCAÇÃO</b></div></p>
						           	<p align="left"><strong>
						           					<br>
						           					<br>
						           					<font color=red>
							           					Atenção: Informamos que o Relatório Geral Consolidado de cada Município/ Estado deverá ser impresso e encaminhado via sedex para o seguinte endereço:

														<br><br>
														Esplanada dos Ministérios Bloco L<br>
														Anexo II 3° andar, sala 300<br>
														Diretoria de Concepções e Orientações Curriculares para Educação Básica - DCOCEB   Brasília - DF<br>
														Cep: 70047-900<br>
														Programa Mais Educação<br>
														<br><br>

														OBS: Ressaltamos a importância da assinatura e carimbo nos Relatórios Consolidados.
														<br>
														Para Municípios assinatura e carimbo do Prefeito e para os Estados do Secretário Estadual de Educação.
						           					</font>
						           	</strong></p>
						           	<br>
							  </td>
							 </tr>
							</table>
					   </p>
						<!--
					    <p align="left">
							<table border=0 cellspacing=0 cellpadding=0 width='100%' >
							 <tr >
							  <td align="center" width=480 style='width:360.0pt;border:solid windowtext 1.0pt; padding:0cm 5.4pt 0cm 5.4pt;height:71.85pt;border-color:-moz-use-text-color'>
								   	<br>
								    <p align="left"><div style="float:left"><img src="/imagens/seta_galeria1.gif" align="bottom">&nbsp;</div> <div style="float:left"> <b>Senhor (a) Secretário (a) Municipal,</b></div></p>

						           	<p align="left"><strong>
						           					<br><br>
													Informamos a prorrogação do prazo para envio das respostas relativa à “PESQUISA DE DADOS SOBRE A EDUCAÇÃO INFANTIL DO CAMPO” para 31/03/2011 (instrumento anexo), disponível também na página do MEC/SECAD/Programas e Ações/Educação Infantil do Campo e  solicitamos a devolução para o seguinte endereço:
													<br><br>
													Profª Gilmara da Silva<br>
													MEC/SECAD/DEDI-CGEC<br>
													gilmara.dasilva@mec.gov.br / profgilmaradasilva@hotmail.com<br>
													(47) 9142-1102 - (47) 3348-4496
													<br><br>
													Maiores informações:<br>
													Coordenação Geral de Educação do Campo<br>
													Esplanada dos Ministérios - Bloco L, Anexo I, 4º andar, Sala 402, 70.047-900    Brasília-DF 		(61) 2022 9302/9011.<br>
													E-mail: coordenacaoeducampo@mec.gov.br ou mariajoselma@mec.gov.br

													<br><br>
						           					 Clique no arquivo abaixo para fazer o download:
						           					 <br>
						           					 <br>Arquivo: <a href="javascript:void(0);" onclick="abreArquivo('pesquisa');">Pesquisa</a>
					           		</strong></p>
					           		<br>
							  </td>
							 </tr>
							</table>
					   </p>
						 -->

			           <!--
					<br>

					   <p align="left"><b>SECRETARIA DE EDUCAÇÃO CONTINUADA, ALFABETIZAÇÃO E DIVERSIDADE</b></p>
			           <p align="left"><strong><font color=red>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			           					Senhor (a) Prefeito (a), est&aacute; dispon&iacute;vel na p&aacute;gina do FNDE e Anexo a rela&ccedil;&atilde;o das escolas pass&iacute;veis de atendimento em 2010 com os recursos do Programa Dinheiro Direto na Escola (PDDE - Campo), bem como o Manual de Orienta&ccedil;&atilde;o para auxiliar as Unidades Executoras (UEx) das escolas benefici&aacute;rias sobre os procedimentos de planejamento, execu&ccedil;&atilde;o e presta&ccedil;&atilde;o de contas dos recursos transferidos.
			           					<br><br>
			           					 Clique nos arquivos abaixo para fazer os downloads:
			           					 <br>
			           					 <br>Arquivo 1: <a href="javascript:void(0);" onclick="abreArquivo('comunicado');">Comunicado</a>
			           					 <br>Arquivo 2: <a href="javascript:void(0);" onclick="abreArquivo('lista');">Lista de Escolas</a>
			           					 <br>Arquivo 3: <a href="javascript:void(0);" onclick="abreArquivo('manual');">Manual de Orienta&ccedil;&atilde;o</a>
			           	</font></strong></p>
					 -->

			</div>
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

	/*
	function montaSegTempo() {
		var alert='';
		alert += '<center><font color=blue><b>2ª Videoconferência Nacional</b></font></center>';
		alert += '<br><br>Informamos que no dia <b>16 de Novembro</b>, às <b>15h</b> (horário de Brasília), teremos a <u><b>2ª videoconferência do PST no Mais Educação</b></u> com o intuito de darmos continuidade às orientações sobre o desenvolvimento pedagógico das atividades em sua escola.';
		alert += '<br><br>Esta 2ª videoconferência terá como principal objetivo orientar aos <font color=red><b>monitores</b></font> sobre o desenvolvimento das atividades a serem desenvolvidas pelo PST/Mais educação.';
		alert += '<br><br>Mobilize as Secretarias de Educação (Estadual e Municipal), gestores das escolas, professores e monitores. <font color=red><b>Todos estão convocados!</b></font>';
		alert += '<br><br>Link de transmissão: <a href="http://portal.mec.gov.br/secad/transmissao" target="_blank"><b>http://portal.mec.gov.br/secad/transmissao</b></a>';
		alert += '<br><br>Aproveitamos para agradecer a todos que participaram da 1ª Videoconferência realizada no dia 07 de Outubro de 2010. Todos puderam colaborar com perguntas e sugestões.';
		alert += '<br><br>A participação efetiva expressa o quanto cada professor e cidadão envolvido no PST/Mais Educação acredita na importância da coletividade, da coesão e da democracia para eficácia deste trabalho junto à sociedade.';
		alert += '<br><br>Em caso de dúvida entre em contato pelo telefone (61) 3217-9490, ou pelo <br> email: <a href="mailto:segundotempo_maisedu@esporte.gov.br">segundotempo_maisedu@esporte.gov.br</a>';
		alert += '<br><br>Contamos com a participação de todos!';
		alert += '<br><br>Atenciosamente,';
		alert += '<br><br>Equipe Gestora do Programa Segundo Tempo.';
		alert += '<p align=center><input type=button value=Fechar onclick=closeMessage();></p>';
		displayStaticMessage(alert,false,'490');
		return false;
	}
	*/
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
