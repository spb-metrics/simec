<?php


	/**
	 * Sistema Integrado de Monitoramento do Minist�rio da Educa��o
	 * Setor responsvel: SPO/MEC
	 * Desenvolvedor: Desenvolvedores Simec
	 * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>, Marcelo Freire
	 * Programadores: Ren� de Lima Barbosa <renedelima@gmail.com>, Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
	 * M�dulo: Seguran�a
	 * Finalidade: Permitir que o usu�rio nevege entre os sistemas.
	 * Data de cria��o:
	 * �ltima modifica��o: 29/08/2006
	 */

	include "config.inc";
	require APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";
	//echo(ob_get_level());
	
	// verifica se a sess�o n�o expirou
	if ( !$_SESSION['usucpf'] ) {
		header( "Location: ../login.php" );
		exit();
	}
	

	// abre conex�o com o servidor de banco de dados
	$db = new cls_banco();
	
	$sisid = $_REQUEST['sisid'];
	$cpf = $_SESSION['usucpf'];
	$_SESSION['usunome'] = $db->pegaUm( "SELECT usunome FROM seguranca.usuario WHERe usucpf = '". $cpf ."'" );
	
	
	// obt�m os dados do m�dulo
	$sql = sprintf(
		"SELECT
			s.sisid, s.sisdiretorio, s.sisarquivo, s.sisdsc, s.sisurl, s.sisabrev, s.sisexercicio, s.paginainicial, p.pflnivel AS usunivel, us.susdataultacesso, us.suscod, s.sisarquivo
			FROM seguranca.usuario u
			INNER JOIN seguranca.perfilusuario pu USING ( usucpf )
			INNER JOIN seguranca.perfil p ON pu.pflcod = p.pflcod
			INNER JOIN seguranca.sistema s ON p.sisid = s.sisid
			INNER JOIN seguranca.usuario_sistema us ON s.sisid = us.sisid AND u.usucpf = us.usucpf
		WHERE
			s.sisid = %d AND
			u.usucpf = '%s' AND
			us.suscod = 'A' AND
			p.pflstatus = 'A' AND
			u.suscod = 'A'
		ORDER BY p.pflnivel
		LIMIT 1",
		$sisid,
		$cpf
	);
	$sistema = (object) $db->pegaLinha( $sql );

	if ( !$sistema ) {
		$_SESSION['MSG_AVISO'][] = "Sua sess�o expirou.";
		header( "Location: login.php" );
		exit();
	}
	
	if ( !$sistema->sisid ) {
		//dbg( $sql );
		//dbg( $sistema, 1 );
		$db->insucesso( 'Voc� n�o tem permiss�o de acesso neste m�dulo.' );
	}

	// carrega os dados do m�dulo para sess�o
	foreach ( $sistema as $attribute => $value ) {
		$_SESSION[$attribute] = $value;
	}

    //dbg($_SESSION, 1);
	// atualiza a data de �ltimo acesso no m�dulo selecionado
	$_SESSION['usuacesso'] = date( 'Y/m/d H:i:s' );
	$sql = sprintf(
		$sql = "UPDATE seguranca.usuario_sistema SET susdataultacesso = '%s' where usucpf = '%s' and sisid = %d",
		$_SESSION['usuacesso'],
		$cpf,
		$sistema->sisid
	);
	$db->executar( $sql );
	$db->commit();

	unset($_SESSION['superuser']);
	unset($_SESSION['usuuma']);
	$_SESSION['superuser'] = $db->testa_superuser();
	unset($_SESSION['exercicio']);
	unset($_SESSION['exercicio_atual']);
	// atribui o ano atual para o exerc�cio das tarefas
	$_SESSION['exercicio_atual'] = $db->pega_ano_atual();
	$_SESSION['exercicio']       = $db->pega_ano_atual();


	// leva o usu�rio para a tela inicial do m�dulo selecionado
	$header = sprintf(
		"../%s/%s.php?modulo=%s",
		$sistema->sisdiretorio,
		$sistema->sisarquivo,
		$sistema->paginainicial
	); 
	/* script pra verificar e exibir eventuais mensagens de alerta
    */
    //include APPRAIZ . "seguranca/modulos/sistema/geral/alertarUsuario.inc";
	//header( $header );
	echo '<script type="text/javascript">window.location.href=\'' , $header , '\'</script>';
	exit();
?>

