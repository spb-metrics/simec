<?php

// inicializa sistema
require_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/workflow.php";

switch ( $_SESSION['sisdiretorio'] ){

	case 'brasilpro':
		include_once APPRAIZ . "www/brasilpro/_funcoes.php"; 
	break;
	case 'par': 
		include_once APPRAIZ . "www/par/autoload.php";
		include_once APPRAIZ . "www/par/_funcoes.php";
		include_once APPRAIZ . "www/par/_constantes.php";
	break;
	case 'cte': 
		include_once APPRAIZ . "www/cte/_funcoes.php";
	break;
	case 'demandas': 
		include_once APPRAIZ . "www/demandas/_constantes.php";
		include_once APPRAIZ . "www/demandas/_funcoes.php";	
	break;
	case 'evento':
		include_once APPRAIZ . "www/evento/_constantes.php";
		include_once APPRAIZ . "www/evento/_funcoes.php"; 
	break;
	case 'pdeescola':
		include_once APPRAIZ . "www/pdeescola/_constantes.php";
		include_once APPRAIZ . "www/pdeescola/_funcoes.php";		
		include_once APPRAIZ . "www/pdeescola/_mefuncoes.php";
	break;
	case 'emenda':
		include_once APPRAIZ . "www/emenda/_constantes.php";
		include_once APPRAIZ . "www/emenda/_funcoes.php";	
	break;
	case 'ies':
		include_once APPRAIZ . "www/ies/_funcoes.php";
	break;
	case 'reuni':
		include_once APPRAIZ . "reuni/www/funcoes.php";
	break;
	case 'fabrica':
		include_once APPRAIZ . "www/fabrica/_constantes.php";
		include_once APPRAIZ . "www/fabrica/_funcoes.php";
		//include_once APPRAIZ . "www/fabrica/_funcoes_avaliacao_aprovacao.php";
		//include_once APPRAIZ . "www/fabrica/_funcoes_execucao.php";
	break;
	case 'academico':
		include_once APPRAIZ . "www/academico/_constantes.php";
		include_once APPRAIZ . "www/academico/_componentes.php";
		include_once APPRAIZ . "www/academico/_funcoes.php";
	break;
	case 'obras':
		include_once APPRAIZ . "www/obras/_constantes.php"; 
		include_once APPRAIZ . "www/obras/_funcoes.php"; 
		include_once APPRAIZ . "www/obras/_componentes.php";
		require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";
	break;
	
}

	
if ( !$db ) {
	$db = new cls_banco();
}

if(!$_REQUEST['docid']) {
	die("Documento não encontrado");
}

$docid = (integer) $_REQUEST['docid'];
$dados = $_REQUEST;

wf_desenhaBarraNavegacao( $docid, $dados );


?>