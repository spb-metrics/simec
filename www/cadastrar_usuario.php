<?php

if(isset($_POST["theme_simec"])){
	$theme = $_POST["theme_simec"];
	setcookie("theme_simec", $_POST["theme_simec"] , time()+60*60*24*30, "/");
} else {
	if(isset($_COOKIE["theme_simec"])){
		$theme = $_COOKIE["theme_simec"];
	}else{
			$theme = "versao antiga";
	}
}

	/** 
	 * Sistema Integrado de Monitoramento do Ministério da Educação
	 * Setor responsvel: SPO/MEC
	 * Desenvolvedor: Desenvolvedores Simec
	 * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>, Alexandre Soares Diniz
	 * Programadores: Renê de Lima Barbosa <renedelima@gmail.com>, Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
	 * Módulo: Segurança
	 * Finalidade: Solicitação de cadastro de contas de usuário.
	 * Data de criação:
	 * Última modificação: 30/08/2006
	 */
	
	define("SIS_PDEESCOLA", 34);
	define("SIS_PSEESCOLA", 65);

	// carrega as bibliotecas internas do sistema
	include "config.inc";
	require APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";

	// abre conexão com o servidor de banco de dados
	$db = new cls_banco();
	
	if(!$theme) {
		$theme = $_SESSION['theme_temp'];
	}
	
	// Particularidade feita para o PDE Escola
	$selecionar_modulo_habilitado = 'S';
	if($_REQUEST['banner_pdeescola']=='acessodireto') {
		$selecionar_modulo_habilitado = 'N';
		$_REQUEST['sisid'] = SIS_PDEESCOLA;
	}
	if($_REQUEST['banner_pseescola']=='acessodireto') {
		$selecionar_modulo_habilitado = 'N';
		$_REQUEST['sisid'] = SIS_PSEESCOLA;
	}


	$sisid  		= $_REQUEST['sisid'];
	$usucpf 		= $_REQUEST['usucpf'];

	// leva o usuário para o passo seguinte do cadastro
	if ($_REQUEST['usucpf'] && $_REQUEST['modulo'] && $_REQUEST['varaux'] == '1') {
		$_SESSION = array();
		if($theme) $_SESSION['theme_temp'] = $theme;
		header("Location: cadastrar_usuario_2.php?sisid=$sisid&usucpf=$usucpf");
		exit();
	}

?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>

<meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1">

<title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>
<?php if(is_file( "includes/layout/".$theme."/include_login.inc" )) include "includes/layout/".$theme."/include_login.inc"; ?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script> 
function ImprimeStatus(texto){ 
    document.formul.numCaracteres.value = texto
} 
</script> 
</head>

<body>
	<div id="tutorial_theme" style="display:none"><span style="color:red;font-weight:bold;">Novidade!</span><br>Agora você pode escolher o VISUAL do seu SIMEC, clique no ícone ao lado e experimente!</div>
	<? include "barragoverno.php"; ?>

<?php
	$mensagens = implode( '<br/>', (array) $_SESSION['MSG_AVISO'] );
	$_SESSION['MSG_AVISO'] = null;
	$titulo_modulo = 'Solicitação de Cadastro de Usuários';
	$subtitulo_modulo = 'Preencha os Dados Abaixo e clique no botão: "Continuar".<br/>'. obrigatorio() .' Indica Campo Obrigatório.'. $mensagens;
//	monta_titulo( $titulo_modulo, $subtitulo_modulo );
?>
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
		     <?
				if($_POST) {
					foreach($_POST as $key => $var) {
						if($key != 'theme_simec') echo "<input type=hidden name='".$key."' value='".$var."'>";
					}
				}
		     ?>
		</form>
		</div>
		
	</td>
</tr>
<form method="post" name="formulario" id="formulario" onsubmit="return false;">
<input type=hidden name="modulo" value="./inclusao_usuario.php"/>
<input type=hidden name="varaux" value="">
<tr>
  <td colspan="2" width="100%" valign="top">
  
  <!-- Lista de Módulos-->
  <table width="98%" border="0" cellpadding="0" cellspacing="0" class="tabela_modulos">
  <tr>
  	<td class="td_bg">&nbsp;Solicitação de Cadastro de Usuários - <small>Preencha os Dados Abaixo e clique no botão: "Continuar"</small></td>
  </tr>
  <tr>
  	<td align="center">
  	<? if( strlen($mensagens) > 5 ){?>
	<div class="error_msg"><? echo (($mensagens)?$mensagens:""); ?></div>
	<? } ?>  	
	</td>
  </tr>
  <tr>
	<td valign="middle" class="td_table_inicio">
	<table width="95%">
	<tr>
		<td style="font-weight: bold;" align='right'>Módulos:</td>
		<td>
		<?php
		$sql = "select s.sisid as codigo, s.sisabrev as descricao from seguranca.sistema s where s.sisstatus='A' and sismostra='t' order by descricao ";
		$db->monta_combo( "sisid", $sql, $selecionar_modulo_habilitado, "&nbsp;", 'selecionar_modulo', '');
		?>
		<?= obrigatorio(); ?>
		</td>
	</tr>
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
							<li><span style="color: #000000">Público-Alvo:</span> <?= $sistema->sispublico ?><br></li>
							<li><span style="color: #000000">Sistemas Relacionados:</span> <?= $sistema->sisrelacionado ?></li>
						</ul>
					</font>
				<?php endif; ?>
			</td>
		</tr>
	<?php endif; ?>
	<input type="hidden" name="sisfinalidade_selc" value="<?=$sisfinalidade_selc?>"/>
	
	<tr>
		<td style="font-weight: bold;" align='right'>CPF:</td>
		<td>
			<input id="usucpf" type="text" name="usucpf" value=<? print '"'.$usucpf.'"'; ?> class="login_input" onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);" />
			<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.'>
		</td>
	</tr>
	<tr>
		<td>&nbsp;</td>
		<td>
			<a class="botao2" href="javascript:validar_formulario()" >Continuar</a>
			<a class="botao1" href="./login.php" >Voltar</a>
		</td>
	</tr>
	</table>  
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

	function selecionar_modulo()
    {
		document.formulario.submit();
	}

	function validar_formulario() 
    {
        var validacao = true;
        var mensagem  = '';

        if (document.formulario.sisid.value == "" ) {
            mensagem += '\nSelecione o módulo no qual você pretende ter acesso.';
            validacao = false;
        }
        
        if (document.formulario.usucpf.value == '' || !validar_cpf(document.formulario.usucpf.value)) {
            mensagem += '\nO cpf informado não é válido.';
            validacao = false;
        }

        document.formulario.varaux.value = '1'; 

        if ( !validacao ) {
            alert( mensagem );
        }else{
        	document.formulario.submit();
        }
	}
</script>