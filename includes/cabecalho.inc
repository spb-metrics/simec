<?php
if(isset($_SESSION["theme_simec"])){
	$theme = $_SESSION["theme_simec"];
}else{
	$theme = recuperaThemaUsuario();
	if($theme) {
		$_SESSION["theme_simec"] = $theme;
	} else {
		$theme = $_SESSION['theme_temp'];
		if($theme){
				$_SESSION["theme_simec"] = $theme;
			} else {
				$theme = "verde";
				$_SESSION["theme_simec"] = $theme;
		}
	}
}

if(isset($_POST["theme_simec"])){
	$theme = $_POST["theme_simec"];
	setcookie("theme_simec", $_POST["theme_simec"] , time()+60*60*24*30, "/");
	$_SESSION["theme_simec"] = $_POST["theme_simec"];
	gravaThemaUsuario($_POST["theme_simec"]);
}


if($_REQUEST['carregarcolaboradores'] and ( $_SESSION['superuser'] OR $db->testa_uma() OR $_SESSION['usuuma'] )) {
	$sql = "select distinct
			u.usucpf as codigo,
			u.usunome as descricao
			from seguranca.usuario u
			inner join seguranca.perfilusuario pf on pf.usucpf = u.usucpf
			inner join perfil p on p.pflcod = pf.pflcod and	p.sisid = " . $_SESSION['sisid'] . "
			inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf and	us.sisid = p.sisid
			where us.suscod = 'A' and p.pflnivel >= (
			select max(pflnivel) from seguranca.perfil
			inner join seguranca.perfilusuario on perfil.pflcod = perfilusuario.pflcod
			where perfilusuario.usucpf = '" . $_SESSION['usucpforigem'] . "'
			and perfil.sisid = " . $_SESSION['sisid'] . " )
			order by 2 ";
	$opt = $db->carregar($sql);
	if($opt[0]) {
		foreach($opt as $op) {
			echo $op['codigo']."##".$op['descricao']."&&";
		}
	}
	exit;

}
	/**
	 * Sistema Integrado de Monitoramento do Ministério da Educação
	 * Setor responsvel: DTI / SE / MEC
	 * Gerente: Cristiano Cabral <cristiano.cabral@gmail.com>
	 * Analistas: Cristiano Cabral <cristiano.cabral@gmail.com>, Adonias Malosso <malosso@gmail.com>
	 * Desenvolvedor: Equipe de Desenvolvedores Simec
	 * Módulo: Cabeçalho do Sistema
	 * Finalidade: Exibir Cabeçalho com funções gerais do sistema.
	 * Data de criação:
	 * Última modificação: 28/08/2006
	 **/

	// ativa o flag de inclusão do cabeçalho
	$cabecalho_sistema = true;

	if($_SESSION['sisdiretorio'] && $_SESSION["sisexercicio"] == 't'){
		$sql = "select prsano as codigo, prsano as descricao,prsexerccorrente,prsexercicioaberto from ". $_SESSION['sisdiretorio'] .".programacaoexercicio order by 1";
		$arrAnoExercicio = $db->carregar($sql);
	}

   if (! $_SESSION['exercicioaberto'] && $_SESSION['sisexercicio'] == 't')
   {
	   $sql = sprintf(
			"SELECT prsexercicioaberto FROM %s.programacaoexercicio WHERE prsano = '%s'",
			$_SESSION['sisdiretorio'],
			$_SESSION['exercicio']
		);
		$_SESSION['exercicioaberto'] = $db->pegaUm( $sql );
   }
	// altera o ano de exercício (caso o usuário solicite)
	if ( $_REQUEST['exercicio'] AND $_SESSION['exercicio'] != $_REQUEST['exercicio'] ) {

		if($arrAnoExercicio){
			foreach($arrAnoExercicio as $anoExercicio){
				$arrAno[] = $anoExercicio['codigo'];
				if($anoExercicio['prsexerccorrente'] == "t"){
					$ano_corrente = $anoExercicio['codigo'] ;
				}
			}
		}
		if(is_array($arrAno)){
			if(in_array($_REQUEST['exercicio'],$arrAno)){
				$_SESSION['exercicio'] = $_REQUEST['exercicio'];
			}else{
				$_SESSION['exercicio'] = $ano_corrente;
			}
		}
		$exercicio = $_SESSION['exercicio'];

		$sql = sprintf(
			"SELECT prsexercicioaberto FROM %s.programacaoexercicio WHERE prsano = '%s'",
			$_SESSION['sisdiretorio'],
			$_REQUEST['exercicio']
		);
		$_SESSION['exercicioaberto'] = $db->pegaUm( $sql );
	}
	/*
	 * O contador de tempo online na tela deve ser atualizado toda vez que o
	 * usuário carregar uma tela do sistema. Ele é utilizado pelo "estou vivo"
	 * de acordo com a constante MAXONLINETIME, definido no config.inc.
	 */
	include APPRAIZ . "includes/registraracesso.php";
?>
<?if ($cabecalho_painel==true){
include APPRAIZ . 'www/painel/novo/cabecalho_painel.php';
}
else{
?>
<html>
	<head>
		<!--
		Baseado no SIMEC:
			Sistema Integrado de Monitoramento do Ministério da Educação
		Creditos SIMEC:
			Analistas: Cristiano Cabral <cristiano.cabral@gmail.com>, Adonias Malosso <malosso@gmail.com>
			Programadores: Cristiano Cabral <cristiano.cabral@gmail.com>, Adonias Malosso <malosso@gmail.com>
			Finalidade: Exibir Cabeçalho com funções gerais do sistema. Permitir o Monitoramento Físico e Financeiro e a Avaliação das Ações e Programas do Ministério
		-->
		<?php echo '<meta name="description" content= "'.$GLOBALS['parametros_sistema_tela']['sigla-nome_completo'].': Permite o Monitoramento Físico e Financeiro e a Avaliação das Ações e Programas do Ministério dentre outras atividades estratégicas">'; ?>
		<meta name="keywords" content="Casa Civil, Presidência da República, SIMEC, MEC, PDE, Ministério da Educação, Analistas: ,Cristiano Cabral, Adonias Malosso, Gilberto Xavier">
		<META NAME="Author" CONTENT="Cristiano Cabral, cristiano.cabral@gmail.com">
		<meta name="audience" content="all">
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Expires" content="-1">

		<center>
			<div id="aguarde" style="background-color:#ffffff;position:absolute;color:#000033;top:50%;left:30%;border:2px solid #cccccc; width:300;font-size:12px;z-index:0;">
				<br><img src="../imagens/wait.gif" border="0" align="middle"> Aguarde! Carregando Dados...<br><br>
			</div>
		</center>
		<?php //ob_flush(); flush(); ?>
		<title><?php echo $GLOBALS['parametros_sistema_tela']['sigla-nome_completo'] ?></title>

		<script language="JavaScript" src="../includes/funcoes.js"></script>

		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

		<!-- INCLUSÃO DO CSS PARA NOVO LAYOUT DO SIMEC - 15/05/10 -->
		<?php if(is_file( APPRAIZ."www/includes/layout/".$theme."/include_simec.inc" )) include APPRAIZ."www/includes/layout/".$theme."/include_simec.inc"; ?>

		<script language="JavaScript">
			function setpfl() {
				document.getElementById('setperfil2').submit();
			}
			function setpfl1() {
				document.getElementById('setperfil').submit();
			}

			function abrirsistema( sisid ) {
				location.href = "../muda_sistema.php?sisid=" + sisid;
			}

			function abrir_popup_mensagem()
			{
				w = window.open( '../geral/popup_mensagem.php', 'mensagens', 'width=780,height=400,scrollbars=yes,menubar=no,toolbar=no,statusbar=no' );
				w.focus();
			}

		</script>

		<!-- AGRUPADOR -->
		<script type="text/javascript" language="javascript" src="../includes/agrupador.js"></script>
		<style type="text/css">
			.combo {
				width: 200px;
			}
		</style>
		<?php include APPRAIZ . "includes/gera_menu.inc"; ?>

	</head>
	<?php
		// pega a página inicial do sistema
		$sql = sprintf(
			"SELECT TRIM( paginainicial ) AS paginic FROM seguranca.sistema WHERE sisid = %d",
			$_SESSION['sisid']
		);
		$paginic = $db->pegaUm( $sql );
	?>
	<body LEFTMARGIN="0" TOPMARGIN="0" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff">	<?
	// exibe mensagem detectada no autenticar.inc
	if ( $_SESSION['mostramsg'] )
	{
		?>
		<script>
		window.open("../geral/ctrlmensagens.php?tot=<?=$_SESSION['mostramsg']?>", "VtrlMensagem",
        "width=420,height=300,menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes");
		</script>
		<?
	}
	unset( $_SESSION['mostramsg'] );

	?>
	<?php include APPRAIZ . "www/barragoverno.php"; ?>

	<table border="0" width="98%" id="main" cellpadding="0" cellspacing="0" style="min-width: 780px;margin-left:7px;">
		<tr>
			<td align="left" valign="top" colspan="2" >
			<form id="setperfil" name="setperfil" action=<?= $_SESSION['sisarquivo'] ?>.php?modulo=<?= $paginic ?>&acao=C" method="post">
				<table width="100%" border="0" cellpadding="2" cellspacing="0" class="notprint" >
					<tr>
						<td align="center">
						</td>
					</tr>
					<tr>
						<td colspan=4 >
							<table width="100%" border=0 >
								<tr>
									<!-- Logo -->
									<td class="titleModulo" width="180" ><img style="cursor:pointer" onclick="window.location.href='<?=$_SESSION['sisarquivo']?>.php?modulo=<?=$paginic?>'" src="../includes/layout/<? echo $theme ?>/img/logo.png" ></td>
									<!-- Módulo -->
									<td class="titleModulo">
									<?php
									$sql = "select sisabrev, sisdsc from seguranca.sistema where sisid = {$_SESSION['sisid']}";
									$arrSis = $db->pegaLinha($sql);
									echo $arrSis['sisabrev'];
									//echo $arrSis['sisdsc'] != $arrSis['sisabrev'] ? $arrSis['sisdsc'] : "";
									?>
									</td>
									<!-- Link / Usuário -->
									<td width="40%" align="right" >
										<?php if( $_SESSION["sisexercicio"] == 't' ): ?>
												<?php
													if ( $_SESSION['exercicio'] ) {
														$exercicio = $_SESSION['exercicio'];
													}
													$exerc = $_SESSION['sisdiretorio'] == 'monitora' ? 'Exercício' : 'Exercício';
												?>

											<? endif; ?>

										<table>
											<tr>
												<td style="color:#FFFFFF" >
													<?= $exerc ? $exerc." :" : "" ?>
												</td>
												<td style="color:#FFFFFF" >
													<?php $exerc ? $db->monta_combo('exercicio',$arrAnoExercicio,'S','','setpfl1','') : ""; ?>
												</td>

												</form>

												<td align="right">
													<a href="javascript:exibeThemas()"><img src="../includes/layout/<? echo $theme ?>/img/bt_temas.png" style="vertical-align: middle" alt="Alterar Tema" title="Alterar Tema" border="0" /></a>
													<a href="javascript:self.print();"><img src="../includes/layout/<? echo $theme ?>/img/bt_print.png" style="vertical-align: middle" border="0" /></a>
											        <a href="javascript:DoPopup('<?= $_SESSION['sisarquivo'] ?>.php?modulo=ajuda/ajuda&acao=C&mnuid=<?= $modulo_atual ?>','helpsimec',440,550);"><img style="vertical-align: middle" src="../includes/layout/<? echo $theme ?>/img/bt_help.png" border="0" /></a>
											        <a href="javascript:DoPopup('<?= $_SESSION['sisarquivo'] ?>.php?modulo=sistema/favorito/cadfavorito&acao=A','favorito',440,400);"><img style="vertical-align: middle" src="../includes/layout/<? echo $theme ?>/img/bt_star.png" border="0" /></a>
											        <a href="javascript:abrir_popup_mensagem();"><img src="../includes/layout/<? echo $theme ?>/img/bt_mail.png" style="vertical-align: middle" border="0" /></a>
											        <a href="/logout.php"><img src="../includes/layout/<? echo $theme ?>/img/bt_logoff.png" border="0" style="vertical-align: middle" /></a>

											        <div style="display:none;margin-left:-150px" id="menu_theme">
													<script>

														function exibeThemas(){
														var div = document.getElementById('menu_theme');

														if(div.style.display == 'none')
															div.style.display = '';
														else
															div.style.display = 'none';

														}

														function alteraTema(){
															document.getElementById('formTheme').submit();
														}
													</script>

													<form id="formTheme" action="" method="post" >
													Tema:
														<select class="select_ylw" name="theme_simec" title="Tema do sistema" onchange="alteraTema(this.value)" >
													            <?php include(APPRAIZ."www/listaTemas.php") ?>
												        </select>
													</form>
													</div>

												</td>
											</tr>

											<form id="setperfil2" name="setperfil2" action="<?= $_SESSION['sisarquivo'] ?>.php?modulo=<?= $paginic ?>" method="post">

											<tr>
												<td style="color:#FFFFFF;white-space: no-wrap;font-weight:bold" >
													Usuário:
												</td>
												<td colspan="2" style="color:#FFFFFF;white-space: no-wrap;font-weight:bold" >

													<?php $_SESSION['usucpf'] = $_REQUEST['usucpf_simu'] ? $_REQUEST['usucpf_simu'] : $_SESSION['usucpf']; ?>
													<?php if ( $_SESSION['superuser'] OR $db->testa_uma() OR $_SESSION['usuuma'] ): ?>
														<?php
															if ( $_SESSION['superuser'] ) {
																//$sql = "select distinct u.usucpf as codigo, u.usunome as descricao from seguranca.usuario u inner join seguranca.perfilusuario pf on pf.usucpf = u.usucpf inner join perfil p on p.pflcod = pf.pflcod and p.sisid =".$_SESSION['sisid']." inner join seguranca.usuario_sistema us on us.usucpf=u.usucpf and us.sisid=p.sisid where us.suscod='A' and u.usustatus = 'A' order by 2";
																$sql = "
																	select distinct
																		u.usucpf as codigo,
																		u.usunome as descricao
																	from seguranca.usuario u
																		inner join seguranca.perfilusuario pf on
																			pf.usucpf = u.usucpf
																		inner join perfil p on
																			p.pflcod = pf.pflcod and
																			p.sisid = " . $_SESSION['sisid'] . "
																		inner join seguranca.usuario_sistema us on
																			us.usucpf = u.usucpf and
																			us.sisid = p.sisid
																	where
																		us.suscod = 'A' AND
																		u.usucpf='".$_SESSION['usucpf']."'
																	order by 2";
															} else {
																$sql = "select distinct u.usucpf as codigo, u.usunome as descricao from seguranca.usuario u inner join seguranca.perfilusuario pf on pf.usucpf = u.usucpf inner join perfil p on p.pflcod = pf.pflcod and p.sisid =".$_SESSION['sisid']." inner join seguranca.usuario_sistema us on us.usucpf=u.usucpf and us.sisid=p.sisid where us.suscod='A' and  u.usucpf not in (select pu.usucpf from seguranca.perfilusuario pu, seguranca.perfil p where pu.pflcod=p.pflcod and p.pflsuperuser = 't' and p.sisid=".$_SESSION['sisid'].") order by 2";
																$_SESSION['usuuma'] = 1;
															}
															$dadosusuario = $db->pegaLinha($sql);
															$usucpf_simu = $_SESSION['usucpf'];
														?>
															<? $db->monta_combo("usucpf_simu",$sql,'S',"Selecione o Usuário",'setpfl','','','300px','','usrs'); ?>

														<script>
														/* Função para substituir todos */
														function replaceAll(str, de, para){
														    var pos = str.indexOf(de);
														    while (pos > -1){
																str = str.replace(de, para);
																pos = str.indexOf(de);
															}
														    return (str);
														}
														/* Função para adicionar linha nas tabelas */

														/* CRIANDO REQUISIÇÃO (IE OU FIREFOX) */
														function criarrequisicao() {
															return window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject( 'Msxml2.XMLHTTP' );
														}
														/* FIM - CRIANDO REQUISIÇÃO (IE OU FIREFOX) */

														/* FUNÇÃO QUE TRATA O RETORNO */
														var pegarretorno = function () {
															try {
																	if ( evXmlHttp.readyState == 4 ) {
																		if ( evXmlHttp.status == 200 && evXmlHttp.responseText != '' ) {
																			// criando options
																			var x = evXmlHttp.responseText.split("&&");
																			for(i=1;i<(x.length-1);i++) {
																				var dados = x[i].split("##");
																				document.getElementById('usrs').options[i] = new Option(dados[1],dados[0]);
																			}
																			var dados = x[0].split("##");
																			document.getElementById('usrs').options[0] = new Option(dados[1],replaceAll(dados[0], ' ',''));
																			document.getElementById('usrs').value = cpfselecionado;
																		}
																		if ( evXmlHttp.dispose ) {
																			evXmlHttp.dispose();
																		}
																		evXmlHttp = null;
																	}
																}
															catch(e) {}
														};
														/* FIM - FUNÇÃO QUE TRATA O RETORNO */

														/* VARIAVEIS GLOBAIS */
														var cpfselecionado = document.getElementById('usrs').value;
														var carregar = false;
														var evXmlHttp = null;
														/* FIM - VARIAVEIS GLOBAIS */

														/* ATRIBUINDO FUNÇÃO DE CLICK AO COMBO DE USUARIOS */
														document.getElementById('usrs').onclick = function () {
															if(!carregar) {
																carregar = true;
																												// limpar o select
																var tot=document.getElementById('usrs').options.length
																for (i=0;i<tot;i++) {
																	document.getElementById('usrs').options[i]=null
																}
																document.getElementById('usrs').options.length=0;

																document.getElementById('usrs').options[0] = new Option("Aguarde, carregando dados...","aguarde");
																document.getElementById('usrs').value = "aguarde";
																if ( evXmlHttp != null ) {
																	return;
																}

																try {
																	evXmlHttp = criarrequisicao();
																	var enderecohtml = replaceAll(window.location.href,"#","");
																	evXmlHttp.open( 'GET', enderecohtml+'&carregarcolaboradores=true', true );
																	evXmlHttp.onreadystatechange = pegarretorno;
																	evXmlHttp.send( null );
																}
																catch (e) {}// tratar exceção


															}// fim do if(carregar)
														}
														/* FIM - ATRIBUINDO FUNÇÃO DE CLICK AO COMBO DE USUARIOS */
														</script>

													<? else: ?>
														<?= $_SESSION['usunome']; ?>
													<? endif; ?>
												</td>
											</tr>
											<tr>
											<td></td>
											<td colspan=2 style="color:#FFFFFF;white-space: no-wrap;">
											<div align="right" class="layBarraCronometroEstilo" id="cronometro_div">Sua sessão expira em:<span id="cronometro" style="font-weight:bold"></span></div>
											</td>
											</tr>
										</table>

									</td>
								</tr>
							</table>
						</td>
					</tr>
				</table>
			</form>
			<table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen"  style="border-bottom: 1px solid;">
				<tr bgcolor="#ffffff">
					<td><img src="../imagens/brasao.gif" width="50" height="50" border="0"></td>
					<td height="20" nowrap>
						<?php echo $GLOBALS['parametros_sistema_tela']['sigla-nome_completo']; ?><br/>
						<?php echo $GLOBALS['parametros_sistema_tela']['unidade_pai']; ?><br/>
						<?php echo $GLOBALS['parametros_sistema_tela']['unidade']; ?><br/>
					</td>
					<td height="20" align="right">
						Impresso por: <strong><?= $_SESSION['usunome']; ?></strong><br>
						Órgão: <?= $_SESSION['usuorgao']; ?><br>
						Hora da Impressão: <?= date("d/m/Y - H:i:s") ?>
					</td>
				</tr>
				<tr bgcolor="#ffffff">
					<td colspan="2">&nbsp;</td>
				</tr>
			</table>
			<table width="100%" border="0" cellpadding="0" cellspacing="0" class="notprint">
				<tr>
					<td valign="bottom">
						<?php

							/*
							 * Monta as abas de seleção de sistemas. Carrega
							 * todos os módulos no qual o usuário possui perfil
							 * ativo.
							 */

						if ( $_SESSION['usucpf'] != $_SESSION['usucpforigem'] ) {
							$sql = sprintf(
								"SELECT
									s.sisid, trim(s.sisabrev) as sisabrev, trim(s.sisdsc) as sisdsc
									FROM seguranca.usuario u
									INNER JOIN seguranca.perfilusuario pu USING ( usucpf )
									INNER JOIN seguranca.perfil p ON pu.pflcod = p.pflcod
									INNER JOIN seguranca.sistema s ON p.sisid = s.sisid
									INNER JOIN seguranca.usuario_sistema us ON s.sisid = us.sisid AND u.usucpf = us.usucpf
								WHERE
									u.usucpf = '%s' AND
									us.suscod = 'A' AND
									p.pflstatus = 'A' AND
									u.suscod = 'A' AND
									s.sisid = %d
								GROUP BY s.sisid, s.sisabrev, s.sisdsc
								ORDER BY s.sisabrev",
								$_SESSION['usucpf'],
								$_SESSION['sisid']
							);
						} else {
							$sql = sprintf(
								"SELECT
									s.sisid, trim(s.sisabrev) as sisabrev, trim(s.sisdsc) as sisdsc
									FROM seguranca.usuario u
									INNER JOIN seguranca.perfilusuario pu USING ( usucpf )
									INNER JOIN seguranca.perfil p ON pu.pflcod = p.pflcod
									INNER JOIN seguranca.sistema s ON p.sisid = s.sisid
									INNER JOIN seguranca.usuario_sistema us ON s.sisid = us.sisid AND u.usucpf = us.usucpf
								WHERE
									u.usucpf = '%s' AND
									us.suscod = 'A' AND
									p.pflstatus = 'A' AND
									u.suscod = 'A'
								GROUP BY s.sisid, s.sisabrev, s.sisdsc
								ORDER BY s.sisabrev",
								$_SESSION['usucpf']
							);
						}
							$sistemas = $db->carregar( $sql );

							// Lógica para retirar, caso exista, o caracter ascii da tecla "Enter".
							$vTeclaEnter = array(chr(10), chr(13));
							for($i=0; $i<count($sistemas); $i++) {
								$sistemas[$i]["sisabrev"] = str_replace($vTeclaEnter, "", $sistemas[$i]["sisabrev"]);
								$sistemas[$i]["sisdsc"] = str_replace($vTeclaEnter, "", $sistemas[$i]["sisdsc"]);
							}

						?>
						<?php if ( $sistemas ) : ?>

<script type="text/javascript">
/***********************************************
* Scrollable Menu Links- © Dynamic Drive DHTML code library (www.dynamicdrive.com)
* Visit http://www.dynamicDrive.com for hundreds of DHTML scripts
* This notice must stay intact for legal use
***********************************************/

//configure path for left and right arrows
var goleftimage='../imagens/back.gif'
var gorightimage='../imagens/foward.gif'
//configure menu width (in px):
var myWidth;
if( typeof( window.innerWidth ) == 'number' ) {
    //Non-IE
    myWidth = window.innerWidth - 70 ;
} else if( document.documentElement && ( document.documentElement.clientWidth || document.documentElement.clientHeight ) ) {
    //IE 6+ in 'standards compliant mode'
    myWidth = document.documentElement.clientWidth - 50;
} else if( document.body && ( document.body.clientWidth || document.body.clientHeight ) ) {
    //IE 4 compatible
    myWidth = document.body.clientWidth - 50 ;
}

var menuwidth = myWidth;
//configure menu height (in px):
var menuheight=18
//Specify scroll buttons directions ("normal" or "reverse"):
var scrolldir="normal"
//configure scroll speed (1-10), where larger is faster
var scrollspeed=10
//specify menu content
var menucontents='<table id="tb_menu" border="0" cellspacing="0" cellpadding="0"><tr>';
<? foreach ( $sistemas as $key => $sistema ): ?>
<? extract( $sistema ); ?>

	<? if ( $sisid == $_SESSION['sisid'] && $key == 0){ ?>
		menucontents = menucontents + '<td><img src="../includes/layout/<? echo $theme ?>/img/aba_left.png"></td>';
	<? }elseif ( $sisid != $_SESSION['sisid'] && $key == 0){ ?>
		menucontents = menucontents + '<td><img src="../includes/layout/<? echo $theme ?>/img/aba_left_off.png"></td>';
	<? }elseif ( $sisid == $_SESSION['sisid'] && $key != 0 ){ ?>
		menucontents = menucontents + '<td><img src="../includes/layout/<? echo $theme ?>/img/aba_mid_left_off.png"></td>';
	<? }elseif ( $sisid != $_SESSION['sisid'] && $key != 0 && $sistemas[$key - 1]['sisid'] == $_SESSION['sisid']){ ?>
		menucontents = menucontents + '<td><img src="../includes/layout/<? echo $theme ?>/img/aba_mid_right_off.png"></td>';
	<? }elseif ( $sisid != $_SESSION['sisid'] && $key != 0 ){ ?>
		menucontents = menucontents + '<td><img src="../includes/layout/<? echo $theme ?>/img/aba_mid_off.png"></td>';
	<? } ?>

	<? if ( $sisid == $_SESSION['sisid']){ ?>
		menucontents = menucontents + '<td nowrap class="td_on" onclick="abrirsistema(<?= $sisid ?>)" title="<?= $sisdsc ?>"><a href="javascript:abrirsistema(<?= $sisid ?>)" ><?= $sisabrev ?></a></td>';
	<? } else{ ?>
		menucontents = menucontents + '<td nowrap class="td_off" onclick="abrirsistema(<?= $sisid ?>)" title="<?= $sisdsc ?>"><a href="javascript:abrirsistema(<?= $sisid ?>)" ><?= $sisabrev ?></a></td>';
	<? } ?>

	<? if ( $sisid == $_SESSION['sisid'] && $key == (count($sistemas) - 1) ){ ?>
		menucontents = menucontents + '<td><img src="../includes/layout/<? echo $theme ?>/img/aba_right.png"></td>';
	<? } elseif ( $sisid != $_SESSION['sisid'] && $key == (count($sistemas) - 1) ){ ?>
		menucontents = menucontents + '<td><img src="../includes/layout/<? echo $theme ?>/img/aba_right_off.png"></td>';
	<? } ?>

<?php endforeach; ?>

menucontents = menucontents + '</tr></table>';

////NO NEED TO EDIT BELOW THIS LINE////////////

var iedom=document.all||document.getElementById
var leftdircode='onmousedown="moveleft(5)"'
var rightdircode='onmousedown="moveright(5)"'
if (scrolldir=="reverse"){
var tempswap=leftdircode
leftdircode=rightdircode
rightdircode=tempswap
}
if (iedom)
document.write('<span id="temp" style="visibility:hidden;position:absolute;top:-100;left:-5000">'+menucontents+'</span>')
var actualwidth='';
var cross_scroll, ns_scroll
var loadedyes=0
function fillup(){
if (iedom){
cross_scroll=document.getElementById? document.getElementById("test2") : document.all.test2
cross_scroll.innerHTML=menucontents;
actualwidth=document.all? parseInt(cross_scroll.offsetWidth) : parseInt(document.getElementById("temp").offsetWidth);
}
else if (document.layers){
ns_scroll=document.ns_scrollmenu.document.ns_scrollmenu2
ns_scroll.document.write(menucontents)
ns_scroll.document.close()
actualwidth=ns_scroll.document.width
}
loadedyes=1
}

function moveleft(i){
if (loadedyes){
if (iedom&&parseInt(cross_scroll.style.left)>(menuwidth-actualwidth)){
cross_scroll.style.left=parseInt(cross_scroll.style.left)-scrollspeed+"px"
}
else if (document.layers&&ns_scroll.left>(menuwidth-actualwidth)) {
ns_scroll.left-=scrollspeed
}
}
if(i > 0) {
	lefttime=setTimeout("moveleft("+(i-1)+")",10)
}

}

function moveright(i){
if (loadedyes){
if (iedom&&parseInt(cross_scroll.style.left)<0)
cross_scroll.style.left=parseInt(cross_scroll.style.left)+scrollspeed+"px"
else if (document.layers&&ns_scroll.left<0)
ns_scroll.left+=scrollspeed
}
if(i > 0) {
	righttime=setTimeout("moveright("+(i-1)+")",10);
}


}


if (iedom||document.layers){
with (document){
document.write('<table border="0" class="bg_linha_cinza" cellspacing="0 cellpadding="0" width="'+menuwidth+'" height="'+menuheight+'"><tr>')
document.write('<td valign="top" id="dirleft" width="10"><table border="0" cellspacing="0" cellpadding="0"><tr><td style="cursor: pointer;background-color:#e6ebed;color:#285e78; border: 1px solid #555555;" '+rightdircode+'>&nbsp;«&nbsp;</td></tr></table></td>')
document.write('<td width="'+menuwidth+'" height="'+menuheight+'" id="flutua1" class="bg_linha_cinza" valign="top">')
if (iedom){
document.write('<div id="flutua2" style="position:relative;width:'+menuwidth+';height:18;overflow:hidden;">')
document.write('<div id="test2" style="position:absolute;left:0;top:0">')
document.write('</div></div>')
}
else if (document.layers){
document.write('<ilayer width='+menuwidth+' height='+menuheight+' name="ns_scrollmenu">')
document.write('<layer name="ns_scrollmenu2" left=0 top=0></layer></ilayer>')
}
document.write('</td>')
document.write('<td valign="top" class="bg_linha_cinza"  id="dirright"><table class="bg_linha_cinza"  border="0" cellspacing="0" cellpadding="0"><tr><td style="cursor: pointer;background-color:#e6ebed;color:#285e78; border: 1px solid #555555;" '+leftdircode+'>&nbsp;')
document.write('»&nbsp;</td></tr></table>')
document.write('</td></tr></table>')
}
}

fillup();

// Procedimento para ajustar a aba selecionada selecionado
var tabela1 = document.getElementById("tb_menu");
var countchar = 0;

for(i = 1; i < tabela1.rows[0].cells.length; i=i+2) {
	var text = tabela1.rows[0].cells[i].innerHTML;
	countchar = countchar + text.length;

	if(tabela1.rows[0].cells[i].className == 'td_on') {
		var sel = countchar + (countchar/1.1); // padronização : 15 caracteres = 90px
	}
}
if(sel > menuwidth) {
	cross_scroll.style.left = (menuwidth + 10) - sel;
}
// Caso seja IE, utilizar o tamanho estimado
if(actualwidth == 0) {
	actualwidth = (countchar*6 - 30);
}
if((countchar*6) < menuwidth) {
	document.getElementById("dirleft").style.display = 'none';
	document.getElementById("dirright").style.display = 'none';
}


window.onresize = redimensionarMenu;

function redimensionarMenu() {
	var myWidth;
	if( typeof( window.innerWidth ) == 'number' ) {
    	//Non-IE
	    myWidth = window.innerWidth;
	} else if( document.documentElement && ( document.documentElement.clientWidth || document.documentElement.clientHeight ) ) {
    	//IE 6+ in 'standards compliant mode'
	    myWidth = document.documentElement.clientWidth;
	} else if( document.body && ( document.body.clientWidth || document.body.clientHeight ) ) {
	    //IE 4 compatible
    	myWidth = document.body.clientWidth;
	}
	if(parseInt(cross_scroll.style.left) < 0) {
		cross_scroll.style.left = (myWidth - 80 - menuwidth) + parseInt(cross_scroll.style.left);
	}
	menuwidth = myWidth - 80;

	if((countchar*6) < menuwidth) {
		document.getElementById("dirleft").style.display = 'none';
		document.getElementById("dirright").style.display = 'none';
	} else {
		document.getElementById("dirleft").style.display = '';
		document.getElementById("dirright").style.display = '';
	}

	document.getElementById('flutua1').style.width = myWidth - 80;
	document.getElementById('flutua2').style.width = myWidth - 80;
}
var minutos=<? echo floor((MAXONLINETIME/60)); ?>;
var seconds=<? echo floor(MAXONLINETIME%60); ?>;
var campo = document.getElementById("cronometro");
var campo_div = document.getElementById("cronometro_div");

		function startCountdown()
			{
			     	if (seconds<=0){
			            seconds=60;
					    minutos-=1;
					 }
					 if (minutos<=-1){
					    seconds=0;
					    seconds+=1;
					    campo.innerHTML="";
					    campo_div.innerHTML="Sessão expirada!";
					 } else{
						seconds-=1
						if(seconds < 10) {
							seconds = "0" + seconds;
						}
					    campo.innerHTML = " " + minutos+"min"+seconds+"s";
					    setTimeout("startCountdown()",1000);
					}
			}
			startCountdown();
</script>

						<?php
							endif;
						?>
					</td>
				</tr>
				<tr>
					<td height="31px" class="submenu" ></td>
				</tr>
			</table>
			<table class="tbl_conteudo" width="100%" border="0" cellpadding="0" cellspacing="0">
				<tr>
					<td align="right" height="17" valign="middle" class="migalha">
						<font color="#285e78" style="margin-right:10px;">
							<?php

								/*
								 * Exibe rastro de navegação. Mostra em qual
								 * local da hierarquia de telas o usuário está.
								 */

								if ( $modulo_atual != "" ) {
									$sql = sprintf(
										"select distinct mnu1.mnudsc  || '  ' || mnu2.mnudsc || ' »» ' || mnu3.mnudsc as rastro from seguranca.menu mnu1, seguranca.menu mnu2,  seguranca.menu mnu3 where mnu3.mnuidpai=mnu2.mnuid and mnu2.mnuidpai=mnu1.mnuid and mnu3.mnuid = %d",
										$modulo_atual
									);
									$rastro = $db->pegaUm( $sql );
									if ( $rastro ) {
										print "<b>Você está aqui:</b> ". $rastro;
									}
								}
							?>
						</font>
					</td>
				</tr>
				<tr>
					<td valign="top"  style="padding-bottom:15px;" >
<?}?>
