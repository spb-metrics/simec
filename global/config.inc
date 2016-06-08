<?php

	function fechar_conexoes()
	{
		while(  pg_ping())
		{
			pg_close();
		}
	}

	register_shutdown_function( 'fechar_conexoes' );

	//colocar o sistema em manutenção
	//header("Location: http://simec.mec.gov.br/manutencao.htm");

	global $servidor_bd, $porta_bd, $nome_bd, $usuario_db, $senha_bd, $email_sistema;

	// Configuração
    $ini_array      = parse_ini_file("config.ini", true);

    // Geral
    define(APPRAIZ, $ini_array['geral']['APPRAIZ']);

    // DB
    $servidor_bd        = $ini_array['db']['servidor_bd'];
    $porta_bd           = $ini_array['db']['porta_bd'];
    $nome_bd            = $ini_array['db']['nome_bd'];
    $usuario_db         = $ini_array['db']['usuario_db'];
    $senha_bd           = $ini_array['db']['senha_bd'];
    $email_sistema      = $ini_array['db']['email_sistema'];

    // DB SIAFI
    $servidor_bd_siafi  = $ini_array['db']['servidor_bd_siafi'];
    $porta_bd_siafi     = $ini_array['db']['porta_bd_siafi'];
    $nome_bd_siafi      = $ini_array['db']['nome_bd_siafi'];
    $usuario_db_siafi   = $ini_array['db']['usuario_db_siafi'];
    $senha_bd_siafi     = $ini_array['db']['senha_bd_siafi'];

    // DB APOIO
    $servidor_bd_apoio  = $ini_array['db']['servidor_bd_apoio'];
    $porta_bd_apoio     = $ini_array['db']['porta_bd_apoio'];
    $nome_bd_apoio      = $ini_array['db']['nome_bd_apoio'];
    $usuario_db_apoio   = $ini_array['db']['usuario_db_apoio'];
    $senha_bd_apoio     = $ini_array['db']['senha_bd_apoio'];

    // EMAIL
    $email_from         = $ini_array['email']['email_from'];
    $email_host         = $ini_array['email']['email_host'];
    $email_mailer       = $ini_array['email']['email_mailer'];
    $email_auth         = $ini_array['email']['email_auth'];
    $email_login        = $ini_array['email']['email_login'];
    $email_pass         = $ini_array['email']['email_pass'];
    $email_port         = $ini_array['email']['email_port'];


	define( 'AUTHSSD', false );

	/**
	 * Tempo máximo que o usuário pode ficar conectado ao sistema sem que sua
	 * sessão expire. Essa funcionalidade é controlada no arquivo estouvivo.php,
	 * no diretório www.
	 */
	 define( 'MAXONLINETIME', 3600);


	//Persistência de Sessão no Banco
	//include_once APPRAIZ . "adodb/session/adodb-session2.php";
	//ADOdb_Session::config("pgsql", $servidor_bd, $usuario_db, $senha_bd, $nome_bd,array('table'=>'seguranca.phpsession'));


	session_start();

	//Controle de erros do sistema
	if ( $ini_array['erros']['ativado'] )
		include_once( 'config.dev.php' );

	//Emula outro usuário
	if ( $_POST['usucpf_simu'] && ( $_SESSION['superuser'] || $_SESSION['usuuma'] ) )
	{
		$_SESSION['usucpf'] = $_POST['usucpf_simu'];
	}

	date_default_timezone_set('America/Sao_Paulo');

	$_SESSION['ambiente'] = 'Presidência da República';
	$email_sistema = 'cristiano.cabral@mec.gov.br; henrique.couto@mec.gov.br';

	/**
	 * Solução paleativa para o problema de navegação entre sistemas. Esta
	 * rotina tenta adivinhar qual módulo o usuário teve a intenção de acessar.
	 * A decisão é tomada a partir da url solicitada pelo usuário no qual ele
	 * indica o diretório e a ação pretendida.
	 */
	preg_match( '/\/([a-zA-Z]*)\//', $_SERVER['REQUEST_URI'], $sisdiretorio );
	$sisdiretorio = $sisdiretorio[1];

	preg_match( '/\/([a-zA-Z]*)\.php/', $_SERVER['REQUEST_URI'], $sisarquivo );
	$sisarquivo = $sisarquivo[1];

	define( 'SISRAIZ', APPRAIZ . $_SESSION['sisdiretorio'] . '/' );



	global $parametros_sistema_tela;
	
	$parametros_sistema_tela = array(
			'nome_completo'=> $ini_array['sistema']['nome_completo'],
			'sigla'=> $ini_array['sistema']['sigla'],
			'unidade'=> $ini_array['sistema']['unidade'],
			'unidade_pai'=>$ini_array['sistema']['unidade_pai'],
			'email'=>$ini_array['email']['email_from'],
	);
		
	$parametros_sistema_tela['sigla-nome_completo'] = $parametros_sistema_tela['sigla'].' - '.$parametros_sistema_tela['nome_completo'];
	$parametros_sistema_tela['nome_e_orgao'] = $parametros_sistema_tela['sigla-nome_completo'].' / '.$parametros_sistema_tela['unidade'].' / '.$parametros_sistema_tela['unidade_pai'];
	$parametros_sistema_tela['orgao_e_pai'] = $parametros_sistema_tela['unidade'].' / '.$parametros_sistema_tela['unidade_pai'];


	$parametros_chave_googlemaps = array(
		'simec'=>   $ini_array['chave_googlemaps']['producao'],
		'simec-local'=> $ini_array['chave_googlemaps']['desenvolvimento'],
		'simec-homologacao'=>$ini_array['chave_googlemaps']['homologacao']
	)

?>
