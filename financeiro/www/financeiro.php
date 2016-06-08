<?
//Carrega parametros iniciais do simec
/*
 * Solicitado pelo Vitor Sad a retirada do controle de acesso 10/09/10
 */

//include_once "controleInicio.inc";

/**
 * Rotina que controla o acesso às páginas do módulo. Carrega as bibliotecas
 * padrões do sistema e executa tarefas de inicialização. 
 *
 * @author Cristiano Cabral (cristiano.cabral@gmail.com)
 * @since 25/08/2008
 */
date_default_timezone_set ('America/Sao_Paulo');

/**
 * Obtém o tempo com precisão de microsegundos. Essa informação é utilizada para
 * calcular o tempo de execução da página.  
 * 
 * @return float
 * @see /includes/rodape.inc
 */
function getmicrotime(){
	list( $usec, $sec ) = explode( ' ', microtime() );
	return (float) $usec + (float) $sec; 
}

// obtém o tempo inicial da execução
$Tinicio = getmicrotime();

// controle o cache do navegador
header( "Cache-Control: no-store, no-cache, must-revalidate" );
header( "Cache-Control: post-check=0, pre-check=0", false );
header( "Cache-control: private, no-cache" );   
header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
header( "Pragma: no-cache" );

// carrega as funções gerais
include_once "config.inc";
include "verificasistema.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

// abre conexão com o servidor de banco de dados
$db = new cls_banco();


$url = str_replace('/'.$_SESSION['sisdiretorio'].'/','',$_SERVER['REQUEST_URI']);
if (! strpos($url, 'favorito')) {  $_SESSION['favurl']=$url; }
$posicao = strpos($url, '&acao=');
$url = substr($url,0,$posicao+7);
$sql = "select mnu.mnuid  from seguranca.menu mnu where trim(mnu.mnulink) ='".$url."'";
$_SESSION['mnuid'] = $db->pegaUm($sql);


//Carrega as funções de controle de acesso
include_once "controleAcesso.inc";
?>
