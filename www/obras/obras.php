<?php
//Carrega parametros iniciais do sistema
include_once "controleInicio.inc";

	function __autoload($class_name) {
		$arCaminho = array(
							APPRAIZ . "includes/classes/modelo/public/",
							APPRAIZ . "includes/classes/modelo/territorios/",
							APPRAIZ . "includes/classes/modelo/entidade/",
							APPRAIZ . "includes/classes/controller/",
							APPRAIZ . "includes/classes/view/",
							APPRAIZ . "includes/classes/html/",
							APPRAIZ . "obras/classe/controller/",
							APPRAIZ . "obras/classe/modelo/"
						  );
						  
		foreach($arCaminho as $caminho){
			$arquivo = $caminho . $class_name . '.class.inc';
			if ( file_exists( $arquivo ) ){
				require_once( $arquivo );
				break;	
			}
		}				  
	}

	// inclui os objetos do sistema.
	include_once APPRAIZ . 'includes/workflow.php';
	include_once APPRAIZ . "includes/classes/Modelo.class.inc";



	// Pega o caminho atual do usuário (em qual módulo se encontra)
	$caminho_atual = $_SERVER["REQUEST_URI"];
	$posicao_caminho = strpos($caminho_atual, 'acao');
	$caminho_atual = substr($caminho_atual, 0 , $posicao_caminho);
	
	// Pega url para os js
	$posicao_caminho_js = strpos($caminho_atual, '?');
	$caminho_atual_js = substr($caminho_atual, 0 , $posicao_caminho_js);

	// carrega as funções do módulo
	include_once '_constantes.php';
	include_once '_funcoes.php';
	include_once '_componentes.php';
	
	

	// abre conexão com o servidor de banco de dados
	$somenteLeitura = "S";
	include "permissoes.php";
	
	// carrega os dados do módulo
	$modulo = $_REQUEST['modulo'];
	$sql= "select ittemail, orgcod, ittabrev from instituicao where ittstatus = 'A'";
	foreach( (array) $db->pegaLinha( $sql ) as $campo => $valor ) {
		$_SESSION[$campo]= trim( $valor );
	}

//Carrega as funções de controle de acesso
include_once "controleAcesso.inc";
?>
<script src="/obras/js/obras.js"></script>
<script type="text/javascript"> var caminho_atual = '<?php echo $caminho_atual_js; ?>'; </script>
