<?php
// INDICA O PROJETO PDE
define( 'PROJETO_PDE', 3 ); 
define( 'PROJETOREVISAO', 68216 );
define( 'PROJETO_PDE2011', 115284 );

define( 'PROJETOENEM', 114098 );	

// INDICA OS PERFIS EM RELAÇÃO AO MÓDULO
switch( $_SESSION['sisid'] ) {
	case 10: # PDE
		
		define( 'PERFIL_GESTOR',  82 );
		
		define( 'PERFIL_GERENTE', 90 );
		
		define( 'PERFIL_EQUIPE_APOIO_GESTOR',  85 );
		
		define( 'PERFIL_EQUIPE_APOIO_GERENTE', 91 );
		define( 'PERFIL_ASSESSOR', 159 );
		
		$_SESSION['pde']['exercicio'] = $_SESSION['pde']['exercicio'] ? $_SESSION['pde']['exercicio'] : date('Y');
		$_SESSION['pde']['exercicio'] = ( $_REQUEST['exercicio'] != '' && $_REQUEST['exercicio'] != $_SESSION['pde']['exercicio'] ) ? $_REQUEST['exercicio'] : $_SESSION['pde']['exercicio'];
		
		switch($_SESSION['pde']['exercicio']){
			case '2009':
				$_SESSION['projeto'] = PROJETO_PDE;
//				ver($_SESSION['projeto']);
				break;
			case '2010':
				$_SESSION['projeto'] = PROJETOREVISAO;
//				ver($_SESSION['projeto']);
				break;
			case '2011':
				$_SESSION['projeto'] = PROJETO_PDE2011;
//				ver($_SESSION['projeto']);
				break;
			
		}
		
		break;
	case 24: # ENEM
		define( 'PERFIL_EXECUTOR',  518 );
		define( 'PERFIL_VALIDADOR',  519 );
		define( 'PERFIL_CERTIFICADOR',  520 );
		define( 'PERFIL_SUPERUSUARIO',  521 );

		define( 'PERFIL_ALTAGESTAO',  530 );
		define( 'PERFIL_SOMENTE_CONSULTA',  531 );
		define( 'PERFIL_ADMINISTRADOR',  532 );
		
		define( 'PERFIL_GESTOR',  82 );
		define( 'PERFIL_GERENTE', 90 );
		define( 'PERFIL_EQUIPE_APOIO_GESTOR',  85 );
		define( 'PERFIL_EQUIPE_APOIO_GERENTE', 91 );
		define( 'PERFIL_ASSESSOR', 159 );
		
		// workflow
		define( 'TPDID_ENEM', 39 );
		
		define( 'ENEM_EST_EM_EXECUCAO', 281 );
		define( 'ENEM_EST_EM_VALIDACAO', 282 );
		define( 'ENEM_EST_EM_CERTIFICACAO', 283 );
		define( 'ENEM_EST_EM_FINALIZADO', 284 );
		
		define( 'ENEM_AEDID_EXECUTAR', 719 );
		define( 'ENEM_AEDID_VALIDAR', 720 );
		define( 'ENEM_AEDID_INVALIDAR', 721 );
		define( 'ENEM_AEDID_CERTIFICAR', 722 );
		define( 'ENEM_AEDID_NAOCERTIFICAR', 723 );
		define( 'ENEM_AEDID_EXFINALIZAR', 729 );
		define( 'ENEM_AEDID_VLFINALIZAR', 730 );
		
		// funções
		define( 'FUNID_EXECUTOR_ENEM', 83 );
		define( 'FUNID_VALIDADOR_ENEM', 84 );
		define( 'FUNID_CERTIFICADOR_ENEM', 85 );
		define( 'FUNID_RESPONSAVEL_ENEM', 86 );

		define( 'FUNID_EXECUTOR_ENEM', 83);
		define( 'FUNID_VALIDADOR_ENEM', 84);
		
		define( 'FUNID_VALIDADORJUR_ENEM', 90);
		define( 'FUNID_EXECUTORJUR_ENEM', 92);
		define( 'FUNID_CERTIFICADORJUR_ENEM', 91);
		//define( 'FUNID_CERTIFICADOR_ENEM', 86);
			
		$_SESSION['pde']['exercicio'] = $_SESSION['pde']['exercicio'] ? $_SESSION['pde']['exercicio'] : '2010';
		$_SESSION['pde']['exercicio'] = ( $_REQUEST['exercicio'] != '' && $_REQUEST['exercicio'] != $_SESSION['pde']['exercicio'] ) ? $_REQUEST['exercicio'] : $_SESSION['pde']['exercicio'];
		
		$_SESSION['projeto'] = PROJETOENEM;
		
		break;
	case 11: # PROJETOS
		
		// usado na tela de listagem de projetos
		define( 'PERFIL_ADMINISTRADOR', 103 ); //Aba
		define( 'PERFIL_CONSULTA', 104 );
		
		define( 'PERFIL_GESTOR',  98 ); //Aba
		define( 'PERFIL_GERENTE', 101 );
		define( 'PERFIL_EQUIPE_APOIO_GESTOR',  99 ); //Aba
		define( 'PERFIL_EQUIPE_APOIO_GERENTE', 102 );
		
		define( 'PERFIL_ALOCACAO_SALAS', 391 ); //Aba
				
		if ( $_SESSION['projeto'] == PROJETO_PDE ) {
			$_SESSION['projeto'] = null;
		}
		break;
}

// INDICA O PROJETO A SER TRABALHADO
define( 'PROJETO', $_SESSION['projeto'] ? (int)$_SESSION['projeto'] : null );

// INDICA OS ESTADOS DAS ATIVIDADES
define( 'STATUS_NAO_INICIADO', 1 );
define( 'STATUS_EM_ANDAMENTO', 2 );
define( 'STATUS_SUSPENSO',     3 );
define( 'STATUS_CANCELADO',    4 );
define( 'STATUS_CONCLUIDO',    5 );

?>