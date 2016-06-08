<?php
/**
 * @author Renan de Lima Barbosa <renandelima@gmail.com>
 * @author Renê de Lima Barbosa <renedelima@gmail.com>
 */

$_SESSION['desenvolvedores'] = array();
if ( $ini_array['erros']['desenvolvedores'] )
	$_SESSION['desenvolvedores'] = (array)explode( ",", $ini_array['erros']['desenvolvedores']);


class ErrorHandler
{

	/**
	 * @var array
	 */
	static $aErrorType = array(
		E_ERROR           => 'Error',
		E_WARNING         => 'Warning',
		E_PARSE           => 'Parsing Error',
		E_NOTICE          => 'Notice',
		E_CORE_ERROR      => 'Core Error',
		E_CORE_WARNING    => 'Core Warning',
		E_COMPILE_ERROR   => 'Compile Error',
		E_COMPILE_WARNING => 'Compile Warning',
		E_USER_ERROR      => 'User Error',
		E_USER_WARNING    => 'User Warning',
		E_USER_NOTICE     => 'User Notice',
		E_STRICT          => 'Runtime Notice'
	);

	/**
	 * @var ErrorHandler
	 */
	static $oHandler = null;

	/**
	 * @ignore
	 */
	private function __construct(){}

	/**
	 * @param integer $iCode
	 * @param string $sMessage
	 * @param string $sFile
	 * @param integer $iLine
	 * @param array $aContext
	 * @return void
	 */
	public function display( $iCode, $sMessage, $sFile, $iLine, array $aContext = array() )
	{
		$oTrace = new BackTrace();
		$oTrace->levelDown( 2 );
		$sTrace = $oTrace->explain();
		ob_clean();

		ob_start();
        print_r($_REQUEST);
        $v_requ = ob_get_contents();
        ob_clean();
               
        ob_start();
        print_r($_SESSION);
        $v_session = ob_get_contents();
        ob_clean();
               
        ob_start();
         print_r($_SERVER);
         $v_server = ob_get_contents();
         ob_clean();
               
               
	   $msg_erro = "
	   <fieldset>
			   <legend>" . self::$aErrorType[$iCode] . " - " . $_SESSION['ambiente'] . "</legend>
			   <pre><b>" . $sMessage . "</b></pre>" . $sTrace . "
	   </fieldset>
	   <fieldset>
			   <legend><b>Variaveis de _REQUEST</b></legend>
			   <pre>".$v_requ."</pre>
	   </fieldset>
	   <fieldset>
			   <legend><b>Variaveis de _SESSION</b></legend>
			   <pre>".$v_session."</pre>
	   </fieldset>
	   <fieldset>
			   <legend><b>Variaveis de _SERVER</b></legend>
			   <pre>".$v_server."</pre>
	   </fieldset>        ";
 

		return $msg_erro;
	}

	
	
	/**
	 * Grava Log de erro
	 * 
	 * @param Text $msgLog
	 * @return void
	 */
	function grava($msgLog)
	{
			$conerr = pg_connect("host=".$GLOBALS["servidor_bd"]." port=".$GLOBALS["porta_bd"]." dbname=".$GLOBALS['nome_bd']."  user=".$GLOBALS["usuario_db"] ." password=".$GLOBALS["senha_bd"] ."");
		    $msgLog = str_replace("<q>","\n",$msgLog);
			$msgLog = str_replace("'","''",$msgLog);
			$sql = "insert into seguranca.auditoria (auddata, audsql,usucpf,audmsg,audip,mnuid, audtipo) values ('".date("d-m-Y H:i:s")."','".str_replace("'","''",$_SESSION['sql'])."','".$_SESSION['usucpf']."','".$msgLog."','".$_SESSION['ip']."','".$_SESSION['mnuid']."','X')";
			pg_query($conerr, $sql);
		    pg_close($conerr);
   	}
	
    /**
	 * Envia email de erro para administrador
	 * 
	 * @param Text $msgLog
	 * @return void
	 */
    function enviaEmail($msgLog)
	{
		global $ini_array;
	    $msgLog = str_replace("<q>","<br>",$msgLog);
	    $paraquem = 'Administrador do Sistema';
	    $paraonde = $ini_array['email']['email_from'];
	    $assunto = 'Erro no Simec ' . date("d-m-Y H:i:s") . " - " . $_SESSION['ambiente'];
	    email($paraquem,$paraonde,$assunto,$msgLog,'',implode(";",$_SESSION['desenvolvedores']));
   	}

    
    /**
	 * Mostra mensagem para o usuário
	 * 
	 * @param Text $msgLog
	 * @return void
	 */	
    function msgErro($msgLog)
	{
        //$msgLog = str_replace("<q>",'\n',$msgLog);
		//$msgLog = str_replace("'","\'",$msgLog);
        ?><script>alert('Ocorreu uma falha inesperada e sua operação não foi executada.\nO problema foi enviado automaticamente aos administradores do sistema para análise,\nse necessário entre em contato com o setor responsável.\n');history.back();</script><?
		exit();
   	}
	
	/**
	 * Manipula erros disparados durante a execução.
	 * 
	 * Captura todos os erros disparados durante a execução.
	 * 
	 * @param integer $iCode
	 * @param string $sMessage
	 * @param string $sFile
	 * @param integer $iLine
	 * @param array $aContext
	 * @return void
	 * @see set_error_handler
	 */
	public function handle( $iCode, $sMessage, $sFile, $iLine, array $aContext = array() )
	{
		global $ini_array;
		
		if( ! isset($_SESSION['usucpf']) ) 
		{
				session_unset();
				$_SESSION['MSG_AVISO'] = 'Sua sessão expirou. Efetue login novamente.';
				header('Location: login.php');
		        exit();
		}
       	 	
       	 	
		// cancela a transacao, caso ela exista;
        if (isset($_SESSION['transacao'])) 
		{
        		pg_query ('rollback;');
        		unset($_SESSION['transacao']);
   	 	}
        	
        	
		$msgLog = $this->display( $iCode, $sMessage, $sFile, $iLine, $aContext );
		/*
		 * Trecho que limpa o código HTML com o número excessivo de <span> 
		 */
		$msgLog = str_replace(array("<span style=\"color: #007700\"></span>",
									"<span style=\"color: #0000BB\"></span>",
									"<span style=\"color: #DD0000\"></span>",
									"<span style=\"color: #FF8000\"></span>"),
							  array("",
							  		"",
							  		"",
							  		""),
							  $msgLog);
		/*
		 * FIM -  Trecho que limpa o código HTML com o número excessivo de <span> 
		 */		
		
		// gravando no arquivo de log
		if ( $ini_array['erros']['grava_erro'] )
	    	$this->grava($msgLog);
    	// enviar um e-mail de aviso quando acontecer algum erro
		if ( $ini_array['erros']['email_erro'] )
	    	$this->enviaEmail($msgLog);
	 	// Mostra Mensagem de erro amigavel e encerra o programa
		if ( $ini_array['erros']['alerta_erro'] )
			$this->msgErro($msgLog);

		print $msgLog;
		exit();
	}

	/**
	 * @return void
	 */
	public static function start()
	{
		if ( self::$oHandler === null )
		{
			self::$oHandler = new ErrorHandler();
			$cError = array( self::$oHandler, 'handle' );
			set_error_handler( $cError, E_ALL & ~E_NOTICE & ~E_STRICT);
		}
	}

}
