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

	
if ( !$db )
{
	$db = new cls_banco();
}

if(!$_REQUEST['docid'] || !$_REQUEST['esdid'] || !$_REQUEST['aedid']) {
	echo "<script>
			alert('Informações não foram passadas corretamente. Refaça o procedimento.');
			window.opener.location='?modulo=inicio&acao=C';
			window.close();
		  </script>";
	exit;
}

$docid = (integer) $_REQUEST['docid'];
$esdid = (integer) $_REQUEST['esdid'];
$aedid = (integer) $_REQUEST['aedid'];
$cmddsc = trim( $_REQUEST['cmddsc'] );
$verificacao = (string) $_REQUEST['verificacao'];
 
// verifica se precisa de comentário e se comentário está preenchido
if ( wf_acaoNecessitaComentario2( $aedid ) && !$cmddsc )
{
	include "alterar_estado_comentario.php";
	exit();
}

// trata dado para verificacao externa
$dadosVerificacao = unserialize( stripcslashes( $verificacao ) );
if ( !is_array( $dadosVerificacao ) )
{
	$dadosVerificacao = array();
}

// realiza alteracao de estado
if ( wf_alterarEstado( $docid, $aedid, $cmddsc, $dadosVerificacao ) )
{
    //var_dump($a);
    //die();
	$mensagem = "Estado alterado com sucesso!";
}
else
{
	$mensagem = wf_pegarMensagem();
	$mensagem = $mensagem ? $mensagem : "Não foi possível alterar estado do documento.";
}

?>
<script type="text/javascript">
	window.opener.wf_atualizarTela( '<?php echo $mensagem ?>', self );
</script>