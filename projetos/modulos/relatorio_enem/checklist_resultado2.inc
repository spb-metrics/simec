<?php
function checklist_monta_coluna_relatorio(){
	
	$coluna = array();
	
	array_push( $coluna, array("campo" 	  => "qtdexecucao_sim",
					   		   "label" 	  => "Qtd. Execução - Sim",
					   		   "blockAgp" => "",
					   		   "type"	  => "numeric") );
	
	array_push( $coluna, array("campo" 	  => "qtdvalidacao_sim",
					   		   "label" 	  => "Qtd. Validação - Sim",
					   		   "blockAgp" => "",
					   		   "type"	  => "numeric") );
	
	array_push( $coluna, array("campo" 	  => "qtdvalidacao_nao",
					   		   "label" 	  => "Qtd. Validação - Não",
					   		   "blockAgp" => "",
					   		   "type"	  => "numeric") );
	
	array_push( $coluna, array("campo" 	  => "qtdcertificacao_sim",
					   		   "label" 	  => "Qtd. Certificação - Sim",
					   		   "blockAgp" => "",
					   		   "type"	  => "numeric") );
	
	array_push( $coluna, array("campo" 	  => "qtdcertificacao_nao",
					   		   "label" 	  => "Qtd. Certificação - Não",
					   		   "blockAgp" => "",
					   		   "type"	  => "numeric") );
	
	return $coluna;
	
}


function checklist_monta_agp_relatorio(){
	
	$agrupador = $_REQUEST['agrupadores'];
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array("qtdexecucao_sim","qtdexecucao_nao","qtdvalidacao_sim","qtdvalidacao_nao","qtdcertificacao_sim","qtdcertificacao_nao")
				);
				
	foreach ( $agrupador as $val ) {
		
		switch( $val ){

			case 'etapas':
				array_push( $agp['agrupador'], array("campo" 	  => "etapas",
								   		   		     "label" 	  => "Etapas") );
			break;	
			
			case 'processos':
				array_push( $agp['agrupador'], array("campo" 	  => "processos",
								   		   		     "label" 	  => "Processos") );
			break;
			
			case 'subprocessos':
				array_push( $agp['agrupador'], array("campo" 	  => "subprocessos",
								   		   		     "label" 	  => "Sub-processos") );
			break;
			
			case 'atividades':
				array_push( $agp['agrupador'], array("campo" 	  => "atividades",
								   		   		     "label" 	  => "Atividades") );
			break;

		}	
	}
	
	return $agp;
	
}


function checklist_monta_sql_relatorio(){
	
	$where = array();
	
	extract($_REQUEST);
	
	// $atividade
	if( $atividade[0] && $atividade_campo_flag ){
		array_push($where, " ati.atiid " . (!$atividade_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $atividade ) . "') ");
	}
	
	if($_REQUEST['agrupadores']) {
		foreach($_REQUEST['agrupadores'] as $or) {
			$order[] = $or;
		}
	}
	
	// monta o sql 
	$sql = "SELECT 
				ati._atinumero ||' - '|| ati.atidescricao as atividades,
				CASE WHEN val1.vldsituacao = TRUE THEN '1' ELSE '0' END as qtdexecucao_sim,
				CASE WHEN val1.vldsituacao = FALSE THEN '1' ELSE '0' END as qtdexecucao_nao,
				CASE WHEN val2.vldsituacao = TRUE THEN '1' ELSE '0' END as qtdvalidacao_sim,
				CASE WHEN val2.vldsituacao = FALSE THEN '1' ELSE '0' END as qtdvalidacao_nao,
				CASE WHEN val3.vldsituacao = TRUE THEN '1' ELSE '0' END as qtdcertificacao_sim,
				CASE WHEN val3.vldsituacao = FALSE THEN '1' ELSE '0' END as qtdcertificacao_nao,
				CASE WHEN ati2.atitipoenem = 'S' THEN ati2._atinumero ||' - '|| ati2.atidescricao 
					 ELSE 'Não possuem sub-processos'
			    END as subprocessos,
				CASE WHEN ati2.atitipoenem = 'P' THEN ati2._atinumero ||' - '|| ati2.atidescricao 
					 WHEN ati3.atitipoenem = 'P' THEN ati3._atinumero ||' - '|| ati3.atidescricao 
					 ELSE 'Não existe'
				END as processos,
				CASE WHEN ati3.atitipoenem = 'E' THEN ati3._atinumero ||' - '|| ati3.atidescricao 
					 WHEN ati4.atitipoenem = 'E' THEN ati4._atinumero ||' - '|| ati4.atidescricao 
					 ELSE 'Não existe'
				END as etapas
				
			FROM 
				projetos.itemchecklist icl
			LEFT JOIN 
				projetos.atividade ati ON ati.atiid = icl.atiid 
			LEFT JOIN 
				projetos.atividade ati2 ON ati2.atiid = ati.atiidpai
			LEFT JOIN 
				projetos.atividade ati3 ON ati3.atiid = ati2.atiidpai
			LEFT JOIN 
				projetos.atividade ati4 ON ati4.atiid = ati4.atiidpai 
			LEFT JOIN 
				projetos.validacao val1 ON val1.iclid = icl.iclid AND val1.tpvid = 1 
			LEFT JOIN 
				projetos.validacao val2 ON val2.iclid = icl.iclid AND val2.tpvid = 2 
			LEFT JOIN 
				projetos.validacao val3 ON val3.iclid = icl.iclid AND val3.tpvid = 3 
				
			".(($where)?"WHERE ".implode(" AND ",$where):"")." 
			".(($order)?"ORDER BY ".implode(",",$order):"");
	
	return $sql;
	
}

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */


// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

// instancia a classe de relatório
$rel = new montaRelatorio();

// monta o sql, agrupador e coluna do relatório
$sql       = checklist_monta_sql_relatorio(); //dbg($sql,1);
$agrupador = checklist_monta_agp_relatorio();
$coluna    = checklist_monta_coluna_relatorio();
$dados 	   = $db->carregar( $sql );

$rel->setAgrupador($agrupador, $dados); 
$rel->setColuna($coluna);
$rel->setTolizadorLinha(true);
$rel->setTotNivel(true);



// Gera o XLS do relatório
if ( $_REQUEST['pesquisa'] == '2' ){
	ob_clean();
    $nomeDoArquivoXls = 'relatorio';
    echo $rel->getRelatorioXls();
    die;
}

?>
<html>
	<head>
		<title> Simec - Sistema Integrado de Monitoramento do Ministério da Educação </title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
	</head>
	<body>
		<center>
			<!--  Cabeçalho Brasão -->
			<?php echo monta_cabecalho_relatorio( '95' ); ?>
		</center>
		
		<!--  Monta o Relatório -->
		<? echo $rel->getRelatorio(); ?>
		
	</body>
</html>