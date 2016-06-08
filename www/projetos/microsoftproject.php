<?php

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Cache-control: private, no-cache");
header("Last-Modified: " . gmdate( "D, d M Y H:i:s" ) . " GMT");
header("Pragma: no-cache");

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

// carrega as funções do módulo
include '_constantes.php';
include '_funcoes.php';
include '_componentes.php';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

include APPRAIZ . 'includes/TarefaProject.php';

$atiid = (integer) $_REQUEST['atiid'];
$acao_msproject = isset( $_REQUEST['microsoftprojectacao'] ) ? $_REQUEST['microsoftprojectacao'] : null; 

//error_reporting( E_ALL );

switch ( $acao_msproject ) {
	case 'importar':
		//header( 'Content-Type: text/plain;' );
		$caminho = $_FILES['arquivo']['tmp_name'];
		if ( $caminho ) {
			$arquivo = fopen( $caminho, 'r' );
			$tarefas = TarefaProject::lerCSV( $arquivo );
			foreach ( $tarefas as $tarefa ) {
				if( $tarefa->nivel == 1 ){
					$tarefa->importarParaAtividade( $atiid );
				}
			}
		}
		atividade_calcular_dados( $atiid );
		//$db->rollback();
		$db->commit();
		header( 'Content-Type: text/html;' );
		?>
		<script type="text/javascript">
			alert( 'Importação realizada com sucesso.' );
			top.microsoft_project_limpar_formulario();
			top.bloquear_arvore();
			top.recarregar_arvore();
		</script>
		<?php
		exit();
	case 'exportar':
		$sql = sprintf(
			"select a.atidescricao from projetos.atividade a where atiid = %s",
			$_REQUEST['atiid']
		);
		$incluiRaiz = (boolean) $_REQUEST['microsoftprojectincluipai'];
		$atidescricao = $db->pegaUm( $sql );
		$a = array( "\n", "\r", "\t", " " );
		$nome = str_replace( $a, '_', $atidescricao ) . '_' . date( 'd-m-Y-h-i-s' );
		//header( 'Content-Type: text/x-csv;' );
		header( 'Content-type: text/comma-separated-values; charset=iso-8859-1' );
		header( 'Content-Disposition: attachment; filename=' . $nome . '.csv' );
		echo TarefaProject::exportarDeAtividade( $atiid, $incluiRaiz );
		exit();
	default:
		?>
		<script type="text/javascript">
			alert( 'Erro na aplicação. Operação inexistente.' );
			top.microsoft_project_limpar_formulario();
			//top.bloquear_arvore();
			//top.recarregar_arvore();
		</script>
		<?php
		exit();
}

?>