<?php
$remetente = array('nome'=>$_SESSION['usunome'], 'email'=>'simec@mec.gov.br');

/**
 * Centraliza as requisi��es ajax do m�dulo.  
 *
 * @author Ren� de Lima Barbosa <renebarbosa@mec.gov.br> 
 * @since 25/05/2007 
 */

function erro( $codigo, $mensagem, $arquivo, $linha ){
	echo "Ocorreu um erro. Por favor tente mais tarde.";
	exit();
}

function excecao( Exception $excecao ){
	echo "Ocorreu um erro. Por favor tente mais tarde.";
	exit();
}


set_error_handler( 'erro', E_USER_ERROR );
set_exception_handler( 'excecao' );

// carrega as fun��es gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

// carrega as fun��es do m�dulo
include '_constantes.php';
include '_funcoes_enem.php';
include '_componentes_enem.php';

// abre conex�o com o servidor de banco de dados
$db = new cls_banco();

// indica ao navegador o tipo de sa�da
header( 'Content-type: text/plain' );
header( 'Cache-Control: no-store, no-cache' );


if($_REQUEST['dataInicio'] && $_REQUEST['idAtividade'] && $_REQUEST['desc']){
	$sql = "SELECT usucpf FROM projetos.usuarioresponsabilidade WHERE atiid = {$_REQUEST['idAtividade']}";
	$usucpf = $db->pegaUm($sql);
	if(!($usucpf)){
		$sql = "SELECT usucpf FROM projetos.usuarioresponsabilidade WHERE atiid = (SELECT x.atiidpai FROM projetos.atividade x WHERE x.atiid = {$_REQUEST['idAtividade']})";
		$usucpf = $db->pegaUm($sql);
	}
	//SE N�O TIVER, BUSCAR NO NIVEL ACIMA!! SE N�O TIVER, PACIENCIA!!
	if($usucpf){
		$sql = "select usuemail from seguranca.usuario where usucpf = '{$usucpf}'";
		$email = $db->pegaUm($sql);
		if($email){
			$assunto  = "[SIMEC] PDE - Plano de Desenvolvimento da Educa��o";
			$conteudo = "
				<font size='2'><b>Atividade:</b> <u>".utf8_decode($_REQUEST['desc'])."</u><font>
				<br><br>
				<b><font size='2'>E-mail para envio: {$email}.<font></b>
				<br><br>
				<b><font size='2'>A data inicial da atividade ".utf8_decode($_REQUEST['desc'])." foi alterada para uma data anterior a data da atividade pai pelo usu�rio {$_SESSION['usunome']}. Por favor verifique.<font></b>
				<br><br>
				<br>	
				<a href=\"http://simec.mec.gov.br/projetos/enem.php?modulo=principal/atividade_enem/atividade&acao=A&atiid={$_REQUEST['idAtividade']}\">Clique Aqui para acessar a atividade.</a>
				<br>	
				<br>		
				Atenciosamente,
				<br>	
				$assunto";
	
				enviar_email($remetente, 'victor.benzi@mec.gov.br', $assunto, $conteudo );
		}
	}
	
	//echo $_REQUEST;
	//ver('aqui');
	//echo 'aqui55';
	die;
}

if($_REQUEST['dataFim'] && $_REQUEST['idAtividade'] && $_REQUEST['desc']){
	$sql = "SELECT usucpf FROM projetos.usuarioresponsabilidade WHERE atiid = {$_REQUEST['idAtividade']}";
	$usucpf = $db->pegaUm($sql);
	if(!($usucpf)){
		$sql = "SELECT usucpf FROM projetos.usuarioresponsabilidade WHERE atiid = (SELECT x.atiidpai FROM projetos.atividade x WHERE x.atiid = {$_REQUEST['idAtividade']})";
		$usucpf = $db->pegaUm($sql);
	}
	//SE N�O TIVER, BUSCAR NO NIVEL ACIMA!! SE N�O TIVER, PACIENCIA!!
	if($usucpf){
		$sql = "select usuemail from seguranca.usuario where usucpf = '{$usucpf}'";
		$email = $db->pegaUm($sql);
		if($email){
			$assunto  = "[SIMEC] PDE - Plano de Desenvolvimento da Educa��o";
			$conteudo = "
				<font size='2'><b>Atividade:</b> <u>".utf8_decode($_REQUEST['desc'])."</u><font>
				<br><br>
				<b><font size='2'>E-mail para envio: {$email}.<font></b>
				<br><br>
				<b><font size='2'>A data final da atividade ".utf8_decode($_REQUEST['desc'])." foi alterada para uma data posterior a data da atividade pai pelo usu�rio {$_SESSION['usunome']}. Por favor verifique.<font></b>
				<br><br>
				<br>	
				<a href=\"http://simec.mec.gov.br/projetos/enem.php?modulo=principal/atividade_enem/atividade&acao=A&atiid={$_REQUEST['idAtividade']}\">Clique Aqui para acessar a atividade.</a>
				<br>	
				<br>		
				Atenciosamente,
				<br>	
				$assunto";
	
				enviar_email($remetente, 'victor.benzi@mec.gov.br', $assunto, $conteudo );
		}
	}
	
	//echo $_REQUEST;
	//ver('aqui');
	//echo 'aqui55';
	die;
}



switch ( $_REQUEST['evento'] ) {

	case 'arvore_alterar_atividade':
		// verifica permiss�o
		if ( !atividade_verificar_responsabilidade( $_REQUEST['atiid'], $_REQUEST['usucpf'] ) ) {
			echo 'Usu�rio sem permiss�o para alterar a atividade.';
			exit();
		}
		if ( in_array( $_REQUEST['campo'], array( 'atidatainicio', 'atidatafim', 'atidataconclusao' ) ) ) {
			switch( $_REQUEST[ 'campo' ] ) {
				case 'atidatainicio':
					// a nova data de inicio nao pode ser posterior a data de termino
					if( ! atividade_calcular_possibilidade_mudar_data( $_REQUEST['atiid'] ,  $_REQUEST['valor'] , null, null ) ) {
						echo " A data de in�cio n�o pode ser posterior a data de t�rmino";
						exit();
					}
					break;
				case 'atidatafim':
					// a nova data de termino nao pode ser anterior a data de inicio
					if( ! atividade_calcular_possibilidade_mudar_data( $_REQUEST['atiid'] , null , $_REQUEST['valor'], null ) ) {
						echo " A data de t�rmino n�o pode ser anterior a data de in�cio";
						exit();
					}
					break;
				case 'atidataconclusao':
					// a nova data de conclusao nao pode ser anterior a data de inico
					if( ! atividade_calcular_possibilidade_mudar_data( $_REQUEST['atiid'] , null , null, $_REQUEST['valor'] ) ) {
						echo " A data de conclus�o n�o pode ser anterior a data de in�cio";
						exit();
					}
					break;
				default:
					break;

			}
			$valor = $_REQUEST['valor'];
			if ( !ereg( "^[0-9]{2}\/[0-9]{2}\/[0-9]{4}$", $valor, $regs ) )
			{
				echo "Data inv�lida";
				exit();
			} else {
				$valor = formata_data_sql($valor);
			}
		} else {
			$valor = $_REQUEST['valor'];
		}
		$sql = sprintf(
			"update projetos.atividade set %s = '%s' where atiid = %d",
		$_REQUEST['campo'],
		$valor,
		$_REQUEST['atiid']
		);
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
		} else {
			$db->commit();
		}
		exit();

	case 'arvore_ocultar':
		arvore_ocultar_item( $_REQUEST['atiid'] );
		exit();

	case 'arvore_exibir':
		arvore_exibir_item( $_REQUEST['atiid'] );
		exit();

	case 'arvore_excluir':
		// verifica permiss�o
		if ( !atividade_verificar_responsabilidade( $_REQUEST['atiid'], $_REQUEST['usucpf'] ) ) {
			echo 'Usu�rio sem permiss�o para excluir a atividade.';
			exit();
		}
		// efetiva a exclus�o
		if( !atividade_excluir( $_REQUEST['atiid'] ) ) {
			$db->rollback();
			echo 'Ocorreu um erro, por favor tente mais tarde.';
			exit();
		}
		// atualiza dados da �rvore
		$sql = "select atiidpai from projetos.atividade where atiid = " . $_REQUEST['atiid'];
		$atiidpai = $db->pegaUm( $sql );
		atividade_calcular_dados( $atiidpai );
		$db->commit();
		exit();

	case 'arvore_inserir':
		// verifica permiss�o no pai do item novo
		if ( !atividade_verificar_responsabilidade( $_REQUEST['atiidpai'], $_REQUEST['usucpf'] ) ) {
			echo 'Usu�rio sem permiss�o para cadastrar atividade.';
			exit();
		}
		// verifica se o t�tulo foi preenchido
		$titulo = trim( $_REQUEST['atidescricao'] );
		if ( empty( $titulo ) ){
			echo 'O t�tulo � obrigat�rio.';
			exit();
		}
		// efetiva a inser��o
		if ( !atividade_inserir( $_REQUEST['atiidpai'], $titulo, $_REQUEST['atitipoenem'] ) ) {
			echo 'Ocorreu um erro, por favor tente mais tarde.';
			exit();
		}
		// atualiza projeto da sess�o
		$sql = sprintf( "select _atiprojeto from projetos.atividade where atiid = %d", $_REQUEST['atiidpai'] );
		$_SESSION['projeto'] = (integer) $db->pegaUm( $sql );
		// atualiza dados da �rvore
		atividade_calcular_dados( $_REQUEST['atiidpai'] );
		$db->commit();
		exit();

	case 'arvore_mudar_ordem':
		// verifica permiss�o no pai do item a ser movido
		$sql = "select atiidpai from projetos.atividade where atiid = ". (integer) $_REQUEST['origem'];
		$atiidpai = $db->pegaUm( $sql );
		if ( !atividade_verificar_responsabilidade( $atiidpai, $_REQUEST['usucpf'] ) ) {
			echo 'Usu�rio sem permiss�o para alterar a atividade.';
			exit();
		}
		$ordem_origem = $db->pegaUm( "select atiordem from projetos.atividade where atiid = ". (integer) $_REQUEST['origem'] );
		$ordem_destino = $db->pegaUm( "select atiordem from projetos.atividade where atiid = ". (integer) $_REQUEST['destino'] );

		if ( !$ordem_origem || !$ordem_destino ) {
			exit();
		}
		$sql = sprintf(
"update projetos.atividade
set atiordem = %d
where atiid = %d",
		$ordem_destino,
		$_REQUEST['origem']
		);
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			echo 'Ocorreu um erro.';
			exit();
		}
		$sql = sprintf(
"update projetos.atividade
set atiordem = %d
where atiid = %d",
		$ordem_origem,
		$_REQUEST['destino']
		);
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			echo 'Ocorreu um erro.';
			exit();
		}

		// atualiza dados da �rvore
		atividade_calcular_dados( $atiidpai );
		$db->commit();
		exit();

	case 'arvore_mudar_nivel':
		// verifica permiss�o
		if ( !atividade_verificar_responsabilidade( $_REQUEST['atiid'], $_REQUEST['usucpf'] ) ) {
			echo 'Usu�rio sem permiss�o para alterar a atividade.';
			exit();
		}
		$sql = "select atiidpai from projetos.atividade where atiid = " . (integer) $_REQUEST['atiid'];
		$atiidpai_antigo = $db->pegaUm( $sql );
		// efetiva a mudan�a de n�vel
		$funcao = $_REQUEST['direcao'] == 'esquerda' ? 'atividade_profundidade_esquerda' : 'atividade_profundidade_direita';
		if ( !$funcao( $_REQUEST['atiid'] ) ) {
			echo 'Ocorreu um erro, por favor tente mais tarde.';
			exit();
		}
		// atualiza dados da �rvore
		$sql = "select atiidpai from projetos.atividade where atiid = " . (integer) $_REQUEST['atiid'];
		$atiidpai_novo = $db->pegaUm( $sql );
		atividade_calcular_dados( $atiidpai_antigo );
		atividade_calcular_dados( $atiidpai_novo );
		$db->commit();
		exit();

	case 'arvore_recarregar':
		// verifica se est� em subatividades
		$sql = "select _atiprojeto from projetos.atividade where atiid = ". (integer) $_REQUEST['atividade'];
		$projeto = $db->pegaUm( $sql );
		$subatividade = ( $_REQUEST['atividade'] == $projeto );
		// carrega a �rvore
		$lista = atividade_listar( $_REQUEST['atividade'], $_REQUEST['profundidade'], $_REQUEST['situacao'],  $_REQUEST['usuario'], $_REQUEST['perfil'] );
		echo arvore_corpo( $lista, null, $subatividade, $_REQUEST['numeracao_relativa'] );
		exit();

	case 'arvore_mudar_pai':
		// verifica permiss�o no pai do item a ser movido
		if ( !atividade_verificar_responsabilidade( $_REQUEST['pai'], $_REQUEST['usucpf'] ) ) {
			echo 'Usu�rio sem permiss�o para alterar a atividade.';
			exit();
		}
		// pega os dados da atividade
		$sql = "select atiid, atiidpai, atiordem from projetos.atividade where atiid = " . (integer) $_REQUEST["atiid"];
		$atividade = $db->pegaLinha( $sql );
		$avo = $atividade["atiidpai"];
		// move para tr�s os antigos irm�os
		$sql = "update projetos.atividade set atiordem = atiordem - 1 where atiidpai = " . (integer) $atividade["atiidpai"] . " and atiordem > " . (integer) $atividade["atiordem"];
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			echo 'Ocorreu um erro, por favor tente mais tarde.';
			exit();
		}
		
		// muda o pai
		$sql = "update projetos.atividade set atiidpai = " . (integer) $_REQUEST['pai'] . ", atiordem = ( select count(atiid) + 1 from projetos.atividade where atistatus = 'A' and atiidpai = ". (integer) $_REQUEST['pai'] ." ) where atiid = " . (integer) $atividade["atiid"];
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			echo 'Ocorreu um erro, por favor tente mais tarde.';
			exit();
		}
		atividade_calcular_dados( $avo );
		$db->commit();
		exit();

	case 'arvore_mudar_irma':
		// verifica permiss�o no pai do item a ser movido
		$sql = "select atiidpai from projetos.atividade where atiid = " . (integer) $_REQUEST['irma'];
		$pai = $db->pegaUm( $sql );
		if ( !atividade_verificar_responsabilidade( $pai, $_REQUEST['usucpf'] ) ) {
			echo 'Usu�rio sem permiss�o para alterar a atividade.';
			exit();
		}
		// pega os dados da atividade
		$sql = "select atiid, atiidpai, atiordem from projetos.atividade where atiid = " . (integer) $_REQUEST["atiid"];
		$atividade = $db->pegaLinha( $sql );
		// move para tr�s os antigos irm�os
		$sql = "update projetos.atividade set atiordem = atiordem - 1 where atiidpai = " . (integer) $atividade["atiidpai"] . " and atiordem > " . (integer) $atividade["atiordem"];
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			echo 'Ocorreu um erro, por favor tente mais tarde.';
			exit();
		}
		// captura os dados do novo irm�o
		$sql = "select atiid, atiidpai, atiordem from projetos.atividade where atiid = " . (integer) $_REQUEST['irma'];
		$irma = $db->pegaLinha( $sql );
		// move os novos irm�os pra frente
		$sql = "update projetos.atividade set atiordem = atiordem + 1 where atiidpai = " . (integer) $irma["atiidpai"] . " and atiordem > " . (integer) $irma["atiordem"];
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			echo 'Ocorreu um erro, por favor tente mais tarde.';
			exit();
		}
		// coloca a atividade na nova posi��o
		$sql = "update projetos.atividade set atiordem = " . ( $irma["atiordem"] + 1 ) . ", atiidpai = ". $irma["atiidpai"] ." where atiid = " . $_REQUEST["atiid"];
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			echo 'Ocorreu um erro, por favor tente mais tarde.';
			exit();
		}
		// organiza a �rvore
		atividade_calcular_dados( $pai );
		$db->commit();
		exit();

	/*
	 * copiar informa��es gerais
	 * copiar restri��es
	 * copiar documentos
	 * copiar responsabilidades
	 * copiar observa��es
	 * copiar atividades filhas
	 */
	case 'copiar':
		
		$_REQUEST['origem'] = (integer) $_REQUEST['origem'];
		$_REQUEST['destino'] = (integer) $_REQUEST['destino'];
		
		if ( !atividade_verificar_responsabilidade( $_REQUEST['origem'], $_REQUEST['usucpf'] ) ) {
			echo 'Usu�rio sem permiss�o para alterar a atividade.';
			exit();
		}
		$origem = array();
		switch ( $_REQUEST['copiar'] ) {
			case '':
				# quando usu�rio quer copiar a atividade e suas filhas
				array_push( $origem, $_REQUEST['origem'] );
				break;
			case '':
				# quando o usu�rio quer copiar as filhas apenas
				$sql = "select atiid from projetos.atividade where atiidpai = " . $_REQUEST['origem'];
				foreach ( (array) $db->carregar( $sql ) as $atividade ) {
					if ( !$atividade ) {
						continue;
					}
					array_push( $origem, (integer) $atividade['atiid'] );
				}
				break;
			default:
				break;
		}
		
		switch ( $_REQUEST['colar'] ) {
			case '':
				if ( !atividade_verificar_responsabilidade( $_REQUEST['destino'], $_REQUEST['usucpf'] ) ) {
					echo 'Usu�rio sem permiss�o para alterar a atividade.';
					exit();
				}
				# quando o usu�rio quer colar dentro de uma atividade
				break;
			case '':
				$atiid = $db->pegaUm( "select atiidpai = from projetos.atividade where atiid = ". (integer) $_REQUEST['destino'] );
				if ( !atividade_verificar_responsabilidade( $atiid, $_REQUEST['usucpf'] ) ) {
					echo 'Usu�rio sem permiss�o para alterar a atividade.';
					exit();
				}
				# quando o usu�rio quer colar ap�s uma atividade
				break;
			default:
				break;
		}
		exit();

	default:
		echo '';
		exit();

}

?>