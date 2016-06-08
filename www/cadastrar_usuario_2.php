<?
$theme= 'presidencia';
if(isset($_POST["theme_simec"])){
	$theme = $_POST["theme_simec"];
	setcookie("theme_simec", $_POST["theme_simec"] , time()+60*60*24*30, "/");
} else {

	if(isset($_COOKIE["theme_simec"])){
		$theme = $_COOKIE["theme_simec"];
	}
}


// Id da permissão
define("PER_SIMEC", 154);


/**
 * Sistema Integrado de Monitoramento do Ministério da Educação

/**
 * Setor responsvel: SPO/MEC
 * Desenvolvedor: Desenvolvedores Simec
 * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>, Alexandre Soares Diniz
 * Programadores: Renê de Lima Barbosa <renedelima@gmail.com>, Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
 * Módulo: Segurança
 * Finalidade: Solicitação de cadastro de contas de usuário.
 * Data de criação:
 * Última modificação: 31/08/2006
 */

// força o uso da base espelho produção
// $_REQUEST['baselogin'] = 'simec_espelho_producao';

// carrega as bibliotecas internas do sistema
include "config.inc";
require APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

if(!$theme) {
	$theme = $_SESSION['theme_temp'];
}

// Carrega a combo com os municípios
if( $_REQUEST["ajaxRegcod"] ){
	header('content-type: text/html; charset=ISO-8859-1');

	$sql = "SELECT
				muncod AS codigo,
				mundescricao AS descricao
			FROM
				territorios.municipio
			WHERE
				estuf = '{$_POST['ajaxRegcod']}'
			ORDER BY
				mundescricao ASC";
	die($db->monta_combo("muncod", $sql, 'S', 'Selecione um município', '', '', '', '200', 'S', 'muncod'));
}

// Carrega a combo com os orgãos do tipo selecionado
if( $_REQUEST["ajax"] == 1 ){
	// Se for estadual verifica se existe estado selecionado
	if ( $_REQUEST["tpocod"] == 2 && !$_REQUEST["regcod"]  ){
		echo '<font style="color:#909090;">
					Favor selecionar um Estado.
				  </font>';
		die;
	}
	// Se for municipal verifica se existe estado selecionado
	if ( $_REQUEST["tpocod"] == 3 && !$_REQUEST["muncod"]  ){
		echo '<font style="color:#909090;">
					Favor selecionar um município.
				  </font>';
		die;
	}
	$tpocod = $_REQUEST["tpocod"];
	$muncod = $_REQUEST["muncod"];
	$regcod = $_REQUEST["regcod"];

	carrega_orgao($editavel, $usucpf);
	die;
}

// Carrega a combo com os orgãos do tipo selecionado
if( $_REQUEST["ajax"] == 2 ){
	carrega_unidade($_REQUEST["entid"], $editavel, $usuario->usucpf);
	die;
}

if( $_REQUEST["ajax"] == 3 ){
	carrega_unidade_gestora($_REQUEST["unicod"], $editavel, $usuario->usucpf);
	die;
}

$_SESSION['mnuid'] = 10;
$_SESSION['sisid'] = 4;

// captura os dados informados no primeiro passo
$sisid  = $_REQUEST['sisid'];
$usucpf = $_REQUEST['usucpf'];

// Verifica se o CPF digitado é válido.
if (!validaCPF($usucpf)){
	die('<script>
			alert(\'CPF inválido!\');
			history.go(-1);
		 </script>');
}

// atribui o ano atual para o exercício das tarefas
// ultima modificação: 05/01/2007
$_SESSION['exercicio_atual'] = $db->pega_ano_atual();
$_SESSION['exercicio'] = $db->pega_ano_atual();

// captura os dados do formulário
$usunome    = $_REQUEST['usunome'];
$usuemail   = $_REQUEST['usuemail'];
$usuemail_c = $_REQUEST['usuemail_c'];
$usufoneddd = $_REQUEST['usufoneddd'];
$usufonenum = $_REQUEST['usufonenum'];

if($sisid == '44'){ // demandas - login automatico
	$entid 		= "390376";

	//$orgcod     = $_REQUEST['orgcod'];
	$orgcod 	= "26000";
	$usufuncao  = $_REQUEST['usufuncao'];
	$carid		= $_REQUEST['carid'];
	//$unicod     = $_REQUEST['unicod'];
	$unicod     = "26101";
	$ungcod     = $_REQUEST['ungcod'];
	//$regcod     = $_REQUEST['regcod'];
	$regcod     = "DF";
	$ususexo    = $_REQUEST['ususexo'];
	$htudsc     = $_REQUEST['htudsc'];
	//$pflcod     = $_REQUEST['pflcod'];
	$pflcod     = 235; //perfil demandante
	//$orgao      = $_REQUEST['orgao'];
	$orgao      = "26000";
	//$muncod     = $_REQUEST['muncod'];
	$muncod     = "5300108";
	//$tpocod     = $_REQUEST['tpocod'];
	$tpocod     = "1";
}
else{
	// Verifica a entidade
	$entid      = isset($_REQUEST['entid']) ? $_REQUEST['entid'] : 'null';
	$entid 		= $entid == 999999 ? 'null' : $entid;

	// captura os dados do formulário
	$usufuncao  = $_REQUEST['usufuncao'];
	$carid		= $_REQUEST['carid'];
	$unicod     = $_REQUEST['unicod'];
	$ungcod     = $_REQUEST['ungcod'];
	$regcod     = $_REQUEST['regcod'];
	$ususexo    = $_REQUEST['ususexo'];
	$htudsc     = $_REQUEST['htudsc'];
	$pflcod     = $_REQUEST['pflcod'];
	$orgao      = $_REQUEST['orgao'];
	$muncod     = $_REQUEST['muncod'];
	$tpocod     = $_REQUEST['tpocod'];
}


// prepara o cpf para ser usado nos comandos sql
$cpf = corrige_cpf( $usucpf );

// verifica se o cpf já está cadastrado no sistema
$sql = sprintf(
		"SELECT
			u.ususexo,
			u.usucpf,
			u.regcod,
			u.usunome,
			u.usuemail,
			u.usustatus,
			u.usufoneddd,
			u.usufonenum,
			u.ususenha,
			u.usudataultacesso,
			u.usunivel,
			u.usufuncao,
			u.ususexo,
			u.entid,
			u.unicod,
			u.usuchaveativacao,
			u.usutentativas,
			u.usuobs,
			u.ungcod,
			u.usudatainc,
			u.usuconectado,
			u.suscod,
			u.muncod,
			u.carid
		FROM
			seguranca.usuario u
		WHERE
			u.usucpf = '%s'",
$cpf
);

$usuario = (object) $db->pegaLinha( $sql );

if ( $usuario->usucpf ) {
	foreach ( $usuario as $atributo => $valor ) {
		$$atributo = $valor;
	}
	$usucpf = formatar_cpf( $usuario->usucpf );
	$cpf_cadastrado = true;
	$editavel = 'N';
} else {
	$cpf_cadastrado = false;
	$editavel = 'S';
}
// verifica se o usuário já está cadastrado no módulo selecionado
$sql = sprintf("SELECT usucpf, sisid, suscod FROM usuario_sistema WHERE usucpf = '%s' AND sisid = %d",$cpf,$sisid);

$usuario_sistema = (object) $db->pegaLinha( $sql );
if ( $usuario_sistema->sisid ) {
	if ( $usuario_sistema->suscod == 'B' ) {
		$_SESSION['MSG_AVISO'] = array( "Sua conta está bloqueada neste sistema. Para solicitar a ativação da sua conta justifique o pedido no formulário abaixo." );
		header( "Location: solicitar_ativacao_de_conta.php?sisid=$sisid&usucpf=$usucpf");
		exit();
	}
	$_SESSION['MSG_AVISO'] = array( "Atenção. CPF já cadastrado no módulo solicitado." );
	header( "Location: cadastrar_usuario.php?sisid=$sisid&usucpf=$usucpf");
	exit();
}
$cpf_cadastrado_sistema = (boolean) $db->pegaUm( $sql );

$sql = sprintf( "select sisid, sisdsc, sisfinalidade, sispublico, sisrelacionado, sisdiretorio from sistema where sisid = %d", $sisid );
$sistema = (object) $db->pegaLinha( $sql );

// efetiva cadastro se o formulário for submetido
if ( $_POST['formulario'] ) {

	// Gerando a senha que poderá ser usada no SSD e no simec
	$senhageral = $db->gerar_senha();

	/*
	 *  Código feito para integrar a autenticação do SIMEC com o SSD
	 *  Inserir o usuário no BD do SSD e inserir a permissão
	 *  Desenvolvido por Alexandre Dourado
	 */
	if(AUTHSSD) {
		include_once("connector.php");
		/*
 		 *  Código feito para integrar a autenticação do SIMEC com o SSD
 		 *  Verifica se o cpf ja esta cadastrado no SSD
 		 *  Desenvolvido por Alexandre Dourado
 		 */

		// Instanciando Classe de conexão
		$SSDWs = new SSDWsUser($tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain);
		// Efetuando a conexão com o servidor (produção/homologação)
		if ($GLOBALS['USE_PRODUCTION_SERVICES']) {
			$SSDWs->useProductionSSDServices();
		} else {
			$SSDWs->useHomologationSSDServices();
		}
		$cpfOrCnpj = str_replace(array(".","-"),array("",""),$_REQUEST["usucpf"]);
		$resposta = $SSDWs->getUserInfoByCPFOrCNPJ($cpfOrCnpj);
		// 	Se retornar a classe padrão, o cpf esta cadastrado
		if($resposta instanceof stdClass) {
			$ssd_cpf_cadastrado= true;
		} else {
			$ssd_cpf_cadastrado = false;
		}
		/*
 		 *  FIM
 		 *  Código feito para integrar a autenticação do SIMEC com o SSD
 		 *  Verifica se o cpf ja esta cadastrado no SSD
 		 *  Desenvolvido por Alexandre Dourado
 	     */


		if(!$ssd_cpf_cadastrado) {
			header("Content-Type: text/html; charset=utf-8");
			ob_start();
			// Instanciando Classe de conexão
			$SSDWs = new SSDWsUser($tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain);
			// Efetuando a conexão com o servidor (produção/homologação)
			if ($GLOBALS['USE_PRODUCTION_SERVICES']) {
				$SSDWs->useProductionSSDServices();
			} else {
				$SSDWs->useHomologationSSDServices();
			}

			$SSD_senha = @utf8_encode(base64_encode($senhageral));
			$SSD_tipo_pessoa = @utf8_encode("F");
			$SSD_nome = @utf8_encode($_POST["usunome"]);
			$SSD_cpf = @utf8_encode(str_replace(array(".","-"),array("",""),$_POST["usucpf"]));
			$SSD_data_nascimento = @utf8_encode("0000-00-00");
			$SSD_email = @utf8_encode($_POST["usuemail"]);
			$SSD_ddd_telefone = @utf8_encode($_POST["usufoneddd"]);
			$SSD_telefone = @utf8_encode($_POST["usufonenum"]);

			// Variavel para inserir os dados no SSD
			$userInfo = "$SSD_senha||$SSD_tipo_pessoa||$SSD_nome||$nome_mae||$SSD_cpf||$rg||$sigla_orgao_expedidor||$orgao_expedidor||$nis||" .
							"$SSD_data_nascimento||$codigo_municipio_naturalidade||$codigo_nacionalidade||$SSD_email||$email_alternativo||" .
							"$cep||$endereco||$sigla_uf_cep||$localidade||$bairro||$complemento||$endereco||$SSD_ddd_telefone||$SSD_telefone||" .
							"$ddd_telefone_alternativo||$telefone_alternativo||$ddd_celular||$celular||$instituicao_trabalho||$lotacao||ssd";
			// Inserindo usuario no SSD
			$resposta = $SSDWs->signUpUser($userInfo);
			if($resposta != "true") {
				session_unset();
				$_SESSION['MSG_AVISO'] = $resposta["erro"];
				header('location: login.php');
    			exit;
			}
			// Incluindo a permissão
			$permissionId = PER_SIMEC;
			$cpfOrCnpj = str_replace(array(".","-"),array("",""),$_POST["usucpf"]);
			// $responsibleForChangeCpfOrCnpj deve ser vazio
			$resposta = $SSDWs->includeUserPermissionByCPFOrCNPJ($cpfOrCnpj, $permissionId, $responsibleForChangeCpfOrCnpj);
			if($resposta != "true") {
				session_unset();
				$_SESSION['MSG_AVISO'] = $resposta["erro"];
				header('location: login.php');
    			exit;
			}
		}

	}
	/*
	 *  FIM
	 *  Código feito para integrar a autenticação do SIMEC com o SSD
	 *  Inserir o usuário no BD do SSD e inserir a permissão
	 *  Desenvolvido por Alexandre Dourado
	 */



	// atribuições requeridas para que a auditoria do sistema funcione
	$_SESSION['sisid'] = 4; # seleciona o sistema de segurança
	$_SESSION['usucpf'] = $cpf;
	$_SESSION['usucpforigem'] = $cpf;

	$tpocod_banco = $tpocod ? (integer) $tpocod : "null";

	if ( !$cpf_cadastrado ) {

		// insere informações gerais do usuário
		$sql = sprintf(
				"INSERT INTO seguranca.usuario (
					usucpf, usunome, usuemail, usufoneddd, usufonenum,
					usufuncao, carid, entid, unicod, usuchaveativacao, regcod,
					ususexo, ungcod, ususenha, suscod, orgao,
					muncod, tpocod
				) values (
					'%s', '%s', '%s', '%s', '%s',
					'%s', '%s', %s, '%s', '%s',
					'%s', '%s', '%s', '%s', '%s',
					'%s', '%s', %s
				)",
		$cpf,
		str_to_upper( $usunome ),
		strtolower( $usuemail ),
		$usufoneddd,
		$usufonenum,
		$usufuncao,
		$carid,
		$entid,
		$unicod,
		'f',
		$regcod,
		$ususexo,
		$ungcod,
		md5_encrypt_senha( $senhageral, '' ),
		'P',
		$orgao,
		$muncod,
		$tpocod_banco
		);

		$db->executar( $sql );
	}


	// vincula o usuário com o módulo
	$sql = sprintf(
    		"INSERT INTO seguranca.usuario_sistema ( usucpf, sisid, pflcod ) values ( '%s', %d, %d )",
	$cpf,
	$sisid,
	$pflcod
	);
	$db->executar( $sql );

	// modifica o status do usuário (no módulo) para pendente
	$descricao = "Usuário solicitou cadastro e apresentou as seguintes observações: ". $htudsc;
	$db->alterar_status_usuario( $cpf, 'P', $descricao, $sisid );

	// executa rotina específica do módulo
	//$arquivo = APPRAIZ . $sistema->sisdiretorio ."/modulos/sistema/usuario/incusuariosql.inc";
	//if ( file_exists( $arquivo ) ) {
	//	include $arquivo;
	//}

	//----------- verificando se o sistema deve inserir dados de proposta e os insere caso necessario

	$sql = "SELECT
							 s.sisid, lower(s.sisdiretorio) as sisdiretorio
							FROM
							 seguranca.sistema s
							WHERE
							 sisid = ".$sisid."";


	$sistema = (object) $db->pegaLinha( $sql );

	$sistema->sisdiretorio = $sistema->sisid == 14 ? 'cte' : $sistema->sisdiretorio;

	$sql = sprintf("SELECT
						CASE WHEN (SELECT
				   						true
				  				   FROM
				   						pg_tables
				  				   WHERE
				   						schemaname = '%s' AND
				   						tablename  = 'tiporesponsabilidade')
						THEN
							true
						WHEN
							  (SELECT
							   		true
							   FROM
							   		pg_tables
							   WHERE
								   schemaname='%s' AND
								   tablename = 'tprperfil')
				THEN true
				ELSE false
				END;",
	$sistema->sisdiretorio,
	$sistema->sisdiretorio);
	//echo "<pre>".$sql;

	$existeTabela = $db->pegaUm($sql);

	if($existeTabela == 't'){

		// exclui todas os programas e ações e etc propostos anteriormente
		//$db->executar( "DELETE FROM ".$sistema->sisdiretorio.".usuariorespproposta WHERE usucpf = ".$cpf." and pflcod = ".$pflcod."" );

		$propostos = (array) $_REQUEST["proposto"];

		foreach ( $propostos as  $chave => $valores) {

			$sql_tpr = "select tprcampo from ".$sistema->sisdiretorio.".tiporesponsabilidade where tprsigla = '".$chave."'";

			$tprcampo = $db->pegaUm($sql_tpr);
			foreach ( $valores as $chave => $valor) {
				$sql_proposta = "insert into
						seguranca.usuariorespproposta
						( urpcampoid, urpcampo, pflcod, usucpf, sisid )
					 	values
					 	( '".$valor."', '".$tprcampo."', '".$pflcod."', '".$cpf."', ".$sisid." )";

				$db->executar( $sql_proposta, false );
			}
		}
	}

	//---------------------------- fim da verificação e inserção



	//**************VERIFICA SE PERFIL POSSUI ATIVAÇÃO AUTOMATICA DO CADASTRO DE USUÁRIO********************

	//$sql = sprintf("SELECT pflcod FROM seguranca.perfil WHERE sisid= and pfpadrao = %s",$pflcod);
	$sql = sprintf("SELECT pflcod FROM seguranca.perfil WHERE sisid=%s and pflpadrao='t'",$sisid);
	$pflcodpadrao = (array) $db->carregarColuna($sql);

	if($pflcodpadrao){ //ativação automatica

		// carrega os dados da conta do usuário
		$sql = sprintf("SELECT
							usucpf, usuemail, ususexo, usunome, ususenha
						FROM
							seguranca.usuario
						WHERE
							usucpf = '%s'",
		$cpf
		);
		$usuariod = (object) $db->pegaLinha( $sql );

		$justificativa = "Ativação automática de usuário pelo sistema";
		$suscod = "A";
		$db->alterar_status_usuario( $usuariod->usucpf, $suscod, $justificativa, $sisid );
		//$email_aprovacao = $usuariosistema->suscod == 'P' && $suscod == 'A' ? true : $email_aprovacao;

		// envia o email de confirmação da conta aprovada
		/*if ( !$cpf_cadastrado ) {
			$remetente = array("nome" => "SIMEC","email" => $usuariod->usuemail);
			$destinatario = $usuariod->usuemail;
			$assunto = "Aprovação do Cadastro no Simec";
			$conteudo = "
				<br/>
				<span style='background-color: red;'><b>Esta é uma mensagem gerada automaticamente pelo sistema. </b></span>
				<br/>
				<span style='background-color: red;'><b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b></span>
				<br/>
				";
			$conteudo .= sprintf(
			"%s %s<p>Sua conta está ativa. Sua Senha de acesso é: %s</p>",
			$usuariod->ususexo == 'M' ? 'Prezado Sr.' : 'Prezada Sra.',
			$usuariod->usunome,
			md5_decrypt_senha( $usuariod->ususenha, '' )
			);

			$conteudo .= "<br><br>* Caso você já alterou a senha acima, favor desconsiderar este e-mail.";
			enviar_email( $remetente, $destinatario, $assunto, $conteudo );

		}
		*/

		// cadastra o perfil demandante

			//deleta os perfis
			$sql = sprintf(
			"DELETE FROM seguranca.perfilusuario WHERE usucpf = '%s' AND pflcod IN ( SELECT p.pflcod FROM seguranca.perfil p WHERE p.sisid = %d )",
			$usuariod->usucpf,
			$sisid
			);
			$db->executar( $sql );

			// inclui o perfil
			foreach($pflcodpadrao as $p){
				$sql = sprintf(
				"INSERT INTO seguranca.perfilusuario ( usucpf, pflcod ) VALUES ( '%s', %d )",
				$usuariod->usucpf,
				$p
				);
				$db->executar( $sql );
			}

			$db->commit();


		$_REQUEST['usucpf'] = formatar_cpf($usuariod->usucpf);
		$_POST['ususenha'] = md5_decrypt_senha( $usuariod->ususenha, '' );
		$_SESSION['logincadastro'] = true;
		include APPRAIZ . "includes/autenticar.inc";

		exit();

	}
	else{ //solicitação de cadastro

		// obtém dados da instituição
		$sql = "select ittcod, ittemail_inclusao_usuario, ittemail, itttelefone1, itttelefone2, ittddd, ittfax, ittsistemasigla from public.instituicao where ittstatus = 'A'";
		$instituicao = (object) $db->pegaLinha( $sql );
		if ( $instituicao->ittcod ) {
			// captura email de cópia quando for módulo de monitoramento
			//if ( $sisid == 1 ) {
			$sqlPegaEmailSistema = "select sisemail	from seguranca.sistema where sisid = " . ( (integer) $sisid );
			$emailCopia = trim( $db->pegaUm( $sqlPegaEmailSistema ) );
			//} else {
			//	$emailCopia = "";
			//}
			$sql = "SELECT sisemail, sistel, sisfax from seguranca.sistema s where s.sisstatus='A' and sismostra='t' AND sisid = $sisid" ;
			//dbg($sql);
			$sistema = (object) $db->pegaLinha( $sql );
			// envia email de confirmação
			$remetente = array("nome" => $instituicao->ittsistemasigla,"email" => $emailCopia);
			$destinatario = $usuemail;
			$assunto = "Solicitação de Cadastro no Simec";
			$conteudo = sprintf("%s<p>%s %s ou no(s) telefone(s): %s Fax %s</p>%s",
				$ususexo == 'M' ? 'Prezado Sr.' : 'Prezada Sra.',
				$instituicao->ittemail_inclusao_usuario,
				" este mesmo endereço ",
				$sistema->sistel,
				$sistema->sisfax,
				$cpf_cadastrado ? '*Usuário já cadastrado' : '*Novo Usuário'
			);
			//dbg($remetente);
			//dbg($emailCopia,1);
			enviar_email( $remetente, $destinatario, $assunto, $conteudo, $emailCopia );
		}
		// leva o usuário para a página de login e exibe confirmação
		$db->commit();

		$sisabrev = $db->pegaUm( "SELECT sisabrev FROM seguranca.sistema WHERE sisid = ". $sisid );
		$mensagem = sprintf("Sua solicitação de cadastro para acesso ao módulo %s foi registrada e será analisada pelo setor responsável. Em breve você receberá maiores informações.",	$sisabrev);
		$_SESSION['MSG_AVISO'][] = $mensagem;
		header( "Location: login.php" );

		exit();

	}

	//**************FIM VERIFICA SE PERFIL POSSUI ATIVAÇÃO AUTOMATICA DO CADASTRO DE USUÁRIO********************
}

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>

<meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1" />

<title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="./includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<?php if(is_file( "includes/layout/".$theme."/include_login.inc" )) include "includes/layout/".$theme."/include_login.inc"; ?>

</head>

<body>
	<div id="tutorial_theme" style="display:none"><span style="color:red;font-weight:bold;">Novidade!</span><br>Agora você pode escolher o VISUAL do seu SIMEC, clique no ícone ao lado e experimente!</div>
	<? include "barragoverno.php"; ?>

<table width="100%" cellpadding="0" cellspacing="0" id="main">
<tr>
	<td width="80%" ><img src="/includes/layout/<? echo $theme ?>/img/logo.png" border="0" /></td>
	<td align="right" style="padding-right: 30px;padding-left:10px;" >
		<img src="/includes/layout/<? echo $theme ?>/img/bt_temas.png" onclick="exibeThemas()" style="cursor:pointer" id="img_change_theme" alt="Alterar Tema" title="Alterar Tema" border="0" />

		<div style="display:none" id="menu_theme">
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

<tr>
      <td colspan="2" width="98%" align="center" valign="top">

      <form method="post" name="formulario">
		<input type=hidden name="formulario" value="1" />

      <table width="98%" border="0" align="center" cellpadding="0" cellspacing="0" class="tabela_modulos">
        <tr>
          <td class="td_bg">&nbsp;Ficha de Solicitação de Cadastro de Usuários</td>
        </tr>
        <tr>
          <td height="106" align="left">

          <!--Caixa de Login-->
          <table class="tabela" width="100%" bgcolor="#f5f5f5">
            <tr>
              <td class="SubTituloDireita"><strong>Módulo:</strong></td>
			  <td><?php
					$sql = "select s.sisid as codigo, s.sisabrev as descricao from seguranca.sistema s where s.sisstatus='A' and sismostra='t'";
					$db->monta_combo( "sisid", $sql, 'N', "Selecione o sistema desejado", '', '' );
				?></td>
            </tr>

			<?php if( $sistema->sisid ): ?>
			<tr>
				<td class="SubTituloDireita">&nbsp;</td>
				<td><font color="#555555" face="Verdana"> <b><?= $sistema->sisdsc ?></b><br />
				<p><?= $sistema->sisfinalidade ?></p>
				<ul>
					<li><span style="color: #000000">Público-Alvo:</span> <?= $sistema->sispublico ?><br>
					</li>
					<li><span style="color: #000000">Sistemas Relacionados:</span> <?= $sistema->sisrelacionado ?></li>
				</ul>
				</font></td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">CPF:</td>
				<td>
					<?php $usucpf_disabled = $usucpf; ?>
					<?= campo_texto( 'usucpf_disabled', $obrig, 'N' ,'' , 19, 14, '###.###.###-##', '' ); ?>
					<?= obrigatorio(); ?>
					<input type="hidden" name="usucpf" value="<?=$usucpf?>" />
				</td>
			</tr>
			<?php endif; ?>


			<?php if ( $cpf_cadastrado_sistema ): ?>
			<tr bgcolor="#C0C0C0">
				<td>&nbsp;</td>
				<td><input type="button" value="Voltar"	onclick="location.href='./cadastrar_usuario.php?sisid=<?=$sisid?>&usucpf=<?=$usucpf?>'"></td>
			</tr>
			<?php else: ?>
			<tr>
				<td align='right' class="SubTituloDireita" width="150px">Nome:</td>
				<td><?= campo_texto( 'usunome', $obrig, $editavel, '', 50, 50, '', '' ); ?>
				<?= obrigatorio(); ?></td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Sexo:</td>
				<td><input id="sexo_masculino" type="radio" name="ususexo" value="M" <?=($ususexo=='M'?"CHECKED":"")?> <?= $cpf_cadastrado ? 'disabled="disabled"' : '' ?> />
					<label for="sexo_masculino">Masculino</label>
					<input id="sexo_feminino" type="radio" name="ususexo" value="F"	<?=($ususexo=='F'?"CHECKED":"")?>	<?= $cpf_cadastrado ? 'disabled="disabled"' : '' ?> />
					<label for="sexo_feminino">Feminino</label> <?= obrigatorio(); ?>
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">UF:</td>
				<td><?php
				$sql = "SELECT regcod AS codigo, regcod||' - '||descricaouf AS descricao FROM uf WHERE codigoibgeuf IS NOT NULL ORDER BY 2";
				$db->monta_combo("regcod",$sql,$editavel,"&nbsp;",'listar_municipios','', '', '', 'S', 'regcod');
				?></td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Município:</td>
				<td>
				<div id="muncod_on" style="display:<?=(($regcod && $muncod) ? 'block' : 'none')?>;">
				<?php
				if($regcod && $muncod)
				{
					$sql = "SELECT
								muncod AS codigo,
								mundescricao AS descricao
							FROM
								territorios.municipio
							WHERE
								estuf = '{$regcod}'
							ORDER BY
								mundescricao ASC";
					$db->monta_combo("muncod", $sql, 'S', 'Selecione um município', '', '', '', '200', 'S', 'muncod');
				}
				else
				{
					echo '<select name=\'muncod\' id=\'muncod\' class=\'CampoEstilo\' style=\'width:170px;\'>
							<option value="">Selecione um município</option>
						  </select>';
				}
				?>
				</div>
				<div id="muncod_off" style="color: #909090; display:<?=(($regcod && $muncod) ? 'none' : 'block')?>;">A Unidade Federal
				selecionada não possui municípios.</div>
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Tipo do Órgão / Instituição:</td>
				<td>
				<?php
				if( $usuario->usucpf )
				{
					$sql = "SELECT
								tp.tpocod as codigo,
								tp.tpodsc as descricao
							FROM
								public.tipoorgao tp
							INNER JOIN public.tipoorgaofuncao tpf ON tp.tpocod = tpf.tpocod
							INNER JOIN entidade.funcaoentidade e ON tpf.funid = e.funid
							INNER JOIN seguranca.usuario u ON u.entid = e.entid
							WHERE u.usucpf = '{$usuario->usucpf}' AND tp.tpostatus='A'";
					$descricao_tipo = "";

					/*** Se não retornar nenhum registro ***/
					if( ! $db->carregar($sql) )
					{
						/*** Recupera todas as instituições ***/
						$sql = "SELECT
								tpocod as codigo,
								tpodsc as descricao
							FROM
								public.tipoorgao
							WHERE tpostatus='A'";
						/*** Campo passa a ser editável ***/
						$editavelTipoOrgao = 'S';

						$descricao_tipo = "&nbsp;";
					}
				}
				else
				{
					$sql = "SELECT
								tpocod as codigo,
								tpodsc as descricao
							FROM
								public.tipoorgao
							WHERE tpostatus='A'";

					$descricao_tipo = "&nbsp;";
				}

				$editavelTipoOrgao = ($editavelTipoOrgao) ? $editavelTipoOrgao : $editavel;

				$db->monta_combo("tpocod",$sql,$editavelTipoOrgao,$descricao_tipo,'ajax_carrega_orgao','','','170','S');
				?>
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Órgão / Instituição:</td>
				<td>
					<span id="spanOrgao">
					 	<?php
					 		if ( $tpocod == 3 && !empty($usuario->orgao) ){
					 			$entid = 999999;
					 		}

					 		if( $usuario->usucpf )
							{
								$sql = "SELECT
											u.entid as codigo,
											CASE WHEN ee.entorgcod is not null THEN ee.entorgcod ||' - '|| ee.entnome
											ELSE ee.entnome END AS descricao
										FROM
											seguranca.usuario u
										INNER JOIN
											entidade.entidade ee ON
											ee.entid = u.entid
										WHERE
											u.usucpf = '{$usuario->usucpf}' AND
											ee.entorgcod <> '73000'";

								if( ! $db->carregar($sql) )
								{
									$editavelOrgao = 'S';
									$editavelUO = 'S';
									$editavelUG = 'S';
								}
							}

							$editavelOrgao = ($editavelOrgao) ? $editavelOrgao : $editavel;

					 		carrega_orgao($editavelOrgao, $usuario->usucpf);
					 	?>

					</span>
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Unidade Orçamentária:</td>
				<td>
					<span id="unidade">
						<?php
							if ( $entid == 'null' ){
								$entid = '';
							}

							$editavelUO = ($editavelUO) ? $editavelUO : $editavel;

							carrega_unidade($entid, $editavelUO, $usuario->usucpf);
						?>
					</span>
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Unidade Gestora:</td>
				<td>
					<span id="unidade_gestora">
						<?php
							$editavelUG = ($editavelUG) ? $editavelUG : $editavel;

							#verificando se há unidades gestoras
							$unidade_gestora = carrega_unidade_gestora($unicod, $editavelUG, $usuario->usucpf);
							if(!$unidade_gestora){
								//echo '<font style="color: #909090;">Esta unidade não possui uma Unidade Gestora.</font>';
							}else{
								echo $unidade_gestora;
							}
						?>
					</span>
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">(DDD) + Telefone:</td>
				<td><?= campo_texto('usufoneddd','',$editavel,'',3,2,'##',''); ?> <?= campo_texto('usufonenum','S',$editavel,'',18,15,'###-####|####-####',''); ?>
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Seu E-Mail:</td>
				<td><?= campo_texto('usuemail','S',$editavel,'',50,100,'','', 'left', '', 0, '' ); ?>
				</td>
			</tr>
			<?php if ( !$cpf_cadastrado ): ?>
			<tr>
				<td align='right' class="SubTituloDireita">Confirme o Seu E-Mail:</td>
				<td><?= campo_texto('usuemail_c','S','','',50,100,'',''); ?> <br />
				<font color="#202020" face="verdana">Este e-mail é para uso
				individual, <b>não utilize endereço coletivo</b>.</font></td>
			</tr>
			<?php endif; ?>
			<tr>
				<td align='right' class="SubTituloDireita">Função/Cargo:</td>
				<td>
					<?php
						if ( $editavel == 'N' && $usuario->carid == 9 ){
							echo campo_texto('usufuncao','S',$editavel,'',50,100,'','', '', '', '', 'id="usufuncao" style="display: none;"');
							echo '<script>document.getElementById(\'usufuncao\').style.display = "";</script>';
						}else{
							$sql = "select carid as codigo, cardsc as descricao from public.cargo order by cardsc";
							$db->monta_combo( "carid", $sql, 'S', 'Selecione', 'alternarExibicaoCargo', '', '', '', 'N', "carid", '' );
						}
					?>
					<?= campo_texto('usufuncao','N',$editavel,'',50,100,'','', '', '', '', 'id="usufuncao" style="display: none;"'); ?>
					<a href="javascript: alternarExibicaoCargo( 'exibirOpcoes' )" id="linkVoltar" style="display: none;" > Exibir Opções</a>
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita" colspan="2">&nbsp;</td>
			</tr>
			<? if($_REQUEST['sisid'] != 57) { ?>
			<tr>
				<td align='right' class="SubTituloDireita">Observações:</td>
				<td><?= campo_textarea('htudsc','N','S','',100,3,''); ?><br>
				</td>
			</tr>
			<?php
			}


			// inclui campos requeridos pelo módulo no qual o usuário pretende se cadastrar
			$arquivo = APPRAIZ . $sistema->sisdiretorio ."/modulos/sistema/usuario/incusuario.inc";
			if ( file_exists( $arquivo ) ){
				include $arquivo;
			}

			if($_REQUEST['sisid'] == 57) {
			?>
			<tr>
				<td align='right' class="SubTituloDireita"><span id="texto_observacao">Observações:</span></td>
				<td><?= campo_textarea('htudsc','N','S','',100,3,''); ?><br>
				</td>
			</tr>
			<? } ?>
			<tr bgcolor="#DCDCDC">
				<td>&nbsp;</td>
				<td>
					<?if($sisid == 44){ ?>
						<a class="botao2" style="float:left;" href="javascript:enviar_formulario()" >Cadastrar</a>
					<?}else{ ?>
						<a class="botao2" style="float:left;" href="javascript:enviar_formulario()" >Enviar Solicita&ccedil;&atilde;o</a>
					<?} ?>
					<a class="botao1" style="float:left" href="./cadastrar_usuario.php?sisid=<?= $sisid ?>&usucpf=<?= $usucpf ?>" >Voltar</a>
				</td>
			</tr>
			<?php endif; ?>


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
<script src="/includes/prototype.js"></script>
<script type="text/javascript">
<!--

	function listar_municipios( regcod )
	{
		var div_on = document.getElementById( 'muncod_on' );
		var div_off = document.getElementById( 'muncod_off' );

		var req = new Ajax.Request('cadastrar_usuario_2.php', {
	        method:     'post',
	        parameters: '&ajaxRegcod=' + regcod,
	        onComplete: function(res)
	        {
				div_on.style.display = 'block';
				div_off.style.display = 'none';

				div_on.innerHTML = res.responseText;
	        }
	  });
	}

	function trim( valor )
	{
		//return valor.replace( /^\s*|\s*$/g, '' );
		return valor.replace( /^\s+|\s+$/g,"" );
	}

	function selecionar_orgao( valor ) {
		document.formulario.formulario.value = "";
		document.formulario.submit();
	}

	function selecionar_unidade_orcamentaria() {
		document.formulario.formulario.value = "";
		document.formulario.submit();
	}

	function enviar_formulario() {
		if ( validar_formulario() ) {
			document.formulario.submit();
		}
	}


	function validar_formulario() {

		//alert('tamanho do nome'+ document.formulario.usunome.value.length);

		var validacao = true;
		var mensagem = 'Os seguintes campos não foram preenchidos corretamente:\n';
		if ( document.formulario.sisid.value == '' || !validar_cpf( document.getElementsByName("usucpf")[0].value ) ) {
			// TODO: voltar para o primeiro formulário
		}

		<?php if ( !$cpf_cadastrado ): ?>
			document.formulario.usunome.value = trim( document.formulario.usunome.value );
			if ( ( document.formulario.usunome.value == '')  || (document.formulario.usunome.value.length < 5 )) {
				mensagem += '\n\tNome';
				validacao = false;
			}
			if ( !validar_radio( document.formulario.ususexo, 'Sexo' ) ) {
				mensagem += '\n\tSexo';
				validacao = false;
			}
			if ( document.formulario.regcod.value == '' ) {
				mensagem += '\n\tUnidade Federal';
				validacao = false;
			} else if ( document.formulario.muncod.value == '' ) {
				mensagem += '\n\tMunicípio';
				validacao = false;
			}

			/*** Tipo do Órgão / Instituição ***/
			if( document.formulario.tpocod )
			{
				if( document.formulario.tpocod.value == '' )
				{
					mensagem += '\n\tTipo do Órgão / Instituição';
					validacao = false;
				}
			}
			/*** Órgão / Instituição ***/
			if( document.formulario.entid )
			{
				if ( document.formulario.tpocod.value != 4 && document.formulario.entid.value == '' )
				{
					mensagem += '\n\tÓrgão / Instituição';
					validacao = false;
				}
			}
			/*** Órgão / Instituição(Outros) ***/
			if( document.formulario.orgao )
			{
				if ( document.formulario.tpocod.value == 4 && document.formulario.orgao.value == '' )
				{
					mensagem += '\n\tÓrgão / Instituição';
					validacao = false;
				}
			}
			/*** Se for federal, valida o preenchimento da UO e UG ***/
			if( document.formulario.tpocod )
			{
				if( document.formulario.tpocod.value == 1 )
				{
					if( document.formulario.unicod )
					{
						if ( document.formulario.unicod.value == '' )
						{
							mensagem += '\n\tUnidade Orçamentária';
							validacao = false;
						}
					}
					if( document.formulario.ungcod )
					{
						if ( document.formulario.ungcod.value == '' )
						{
							mensagem += '\n\tUnidade Gestora';
							validacao = false;
						}
					}
				}
			}

			<?php if ( $uo_total > 0 ): ?>
				/*if ( document.formulario.unicod.value == '' ) {
					mensagem += '\n\tUnidade Orçamentária';
					validacao = false;
				}*/
			<?php endif; ?>
			/*
			if ( document.formulario.orgao ){
				document.formulario.orgao.value = trim( document.formulario.orgao.value );
				if (    document.formulario.orgao.value == '' ||
					document.formulario.orgao.value.length < 5
				    )
				{
					mensagem += '\n\tNome do Órgão';
					validacao = false;
				}
			}*/

			if ( document.formulario.entid ) {
				if ( document.formulario.entid.value == '390360' && document.formulario.unicod.value == '26101' ) {
					if ( document.formulario.ungcod.value == '' ) {
						mensagem += '\n\tUnidade Gestora';
						validacao = false;
					}
				}
			}

			document.formulario.usufoneddd.value = trim( document.formulario.usufoneddd.value );
			document.formulario.usufonenum.value = trim( document.formulario.usufonenum.value );
			if (
				document.formulario.usufoneddd.value == '' ||
				document.formulario.usufonenum.value == '' ||

				document.formulario.usufoneddd.value.length < 2 ||
				document.formulario.usufonenum.value.length < 7
			   )
			{
				mensagem += '\n\tTelefone';
				validacao = false;
			}
			document.formulario.usuemail.value = trim( document.formulario.usuemail.value );
			if ( !validaEmail( document.formulario.usuemail.value ) ) {
				mensagem += '\n\tEmail';
				validacao = false;
			}
			document.formulario.usuemail_c.value = trim( document.formulario.usuemail_c.value );
			if ( !validaEmail( document.formulario.usuemail_c.value ) ) {
				mensagem += '\n\tConfirmação do Email';
				validacao = false;
			}
			if ( validaEmail( document.formulario.usuemail.value ) && validaEmail( document.formulario.usuemail_c.value ) && document.formulario.usuemail.value != document.formulario.usuemail_c.value ) {
				mensagem += '\n\tOs campos Email e Confirmação do Email não coincidem.';
				validacao = false;
			}

			if ( document.formulario.carid ) {
				if ( document.formulario.carid.value == '' ) {
					mensagem += '\n\tFunção/Cargo';
					validacao = false;
				}
				else{
					if( document.formulario.carid.value == 9 ){
						document.formulario.usufuncao.value = trim( document.formulario.usufuncao.value );
						if (
							document.formulario.usufuncao.value == '' ||
							document.formulario.usufuncao.value.length < 5
						    )
						{
							mensagem += '\n\tFunção';
							validacao = false;
						}
					}
				}
			}
		<?php endif; ?>

		if ( document.formulario.pflcod )
		{
			if ( document.formulario.pflcod.value == '' ) {
				mensagem += '\n\tPerfil';
				validacao = false;
			}

			// seleciona todos as ações
			var acoes = document.getElementById( "proposto_A" );
			if ( acoes ) {
				if ( acoes.options.length == 1 && acoes.options[0].value == '' ) {
					mensagem += '\n\tAções';
					validacao = false;
				} else {
					for ( var i=0; i < acoes.options.length; i++ ) {
						acoes.options[i].selected = true;
					}
				}
			}

			// seleciona todos os programas
			var programas = document.getElementById( "proposto_P" );
			if ( programas ) {
				if ( programas.options.length == 1 && programas.options[0].value == '' ) {
					mensagem += '\n\tProgramas';
					validacao = false;
				} else {
					for ( var i=0; i < programas.options.length; i++ ) {
						programas.options[i].selected = true;
					}
				}
			}

			// seleciona todas as unidades
			var unidades = document.getElementById( "proposto_U" );
			if ( unidades ) {
				if ( unidades.options.length == 0 && unidades.options[0].value == '' ) {
					mensagem += '\n\tUnidades';
					validacao = false;
				} else {
					for ( var i=0; i < unidades.options.length; i++ ) {
						unidades.options[i].selected = true;
					}
				}
			}
		}

		if ( !validacao ) {
			alert( mensagem );
		}
		return validacao;
	}

	function alternarExibicaoCargo( tipo ){

		var carid = document.getElementById( 'carid' );
		var usufuncao = document.getElementById( 'usufuncao' );
		var link = document.getElementById( 'linkVoltar' );


		if( tipo != 'exibirOpcoes' ){
			if( carid.value == 9 || carid.value == ''){
				usufuncao.style.display = "";
				//usufuncao.className = "";
				link.style.display = "";
				carid.style.display = "none";
				//link.className = "";
			}
		}
		else{
			usufuncao.style.display = "none";
			usufuncao.value = "";
			link.style.display = "none";
			//link.className = "objetoOculto";
			carid.style.display = "";
			carid.value = "";
		}
	}

--></script>