<?php

function obras_monta_agp_arquivos_corrompidos_supervisao(){

	$agrupador = $_REQUEST['colunas'];
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array( "descricao_arquivo", "tamanho_arquivo", "datahora_arquivo", "qtd", "qtdrec" )
				);

				
	foreach ( $agrupador as $val ){
		switch( $val ){
			case "nome_usuario_inserir":
				array_push($agp['agrupador'], array(
													"campo" => "nome_usuario_inserir",
											  		"label" => "Nome do usuário que inseriu")										
									   				);
			break;
			case "unidade_orcamentaria":
				array_push($agp['agrupador'], array(
													"campo" => "unidade_orcamentaria",
											  		"label" => "Unidade ResponsÃ¡vel pela Obra")										
									   				);
			break;
			case "nome_obra":
				array_push($agp['agrupador'], array(
													"campo" => "nome_obra",
											  		"label" => "Nome da obra")										
									   				);
			break;
			case "municipio":
				array_push($agp['agrupador'], array(
													"campo" => "municipio",
											  		"label" => "Município")										
									   				);
			break;
			case "nome_arquivo":
				array_push($agp['agrupador'], array(
													"campo" => "nome_arquivo",
											  		"label" => "Nome do arquivo")										
									   				);
			break;
			case "uf":
				array_push($agp['agrupador'], array(
													"campo" => "uf",
											  		"label" => "UF")										
									   				);
			break;
			case "convenio":
				array_push($agp['agrupador'], array(
													"campo" => "convenio",
											  		"label" => "Convênio")										
									   				);
			break;
			case "data_supervisao":
				array_push($agp['agrupador'], array(
													"campo" => "data_supervisao",
											  		"label" => "Data da supervisão")										
									   				);
			break;
			case "data_inclusao_supervisao":
				array_push($agp['agrupador'], array(
													"campo" => "data_inclusao_supervisao",
											  		"label" => "Extensão do Arquivo")										
									   				);
			break;
			case "extensao_arquivo":
				array_push($agp['agrupador'], array(
													"campo" => "extensao_arquivo",
											  		"label" => "Extensão do arquivo")										
									   				);
			break;
			case "data_arquivo":
				array_push($agp['agrupador'], array(
													"campo" => "data_arquivo",
											  		"label" => "Data da inclusão (arquivo)")										
									   				);
			break;
			case "situacao_obra":
				array_push($agp['agrupador'], array(
													"campo" => "situacao_obra",
											  		"label" => "Situação da obra")										
									   				);
			break;
			case "orgao_obra":
				array_push($agp['agrupador'], array(
													"campo" => "orgao_obra",
											  		"label" => "Orgão da obra")										
									   				);
			break;
			case "tp_supervisao":
				array_push($agp['agrupador'], array(
													"campo" => "tp_supervisao",
											  		"label" => "Responsável pela supervisão")										
									   				);
			break;
			
		}	
	}
	
	
	return $agp;
	
}

function obras_monta_sql_arquivos_corrompidos_supervisao(){
	
	global $db;
	$where = array();
	
	$filtro="";
	
	// Filtros
	if ( $_REQUEST["estuf"][0] ){
		$notestuf = $_REQUEST['estuf_campo_excludente'] == '1' ? ' NOT ' : '';
		$filtro .= " AND e.estuf ".$notestuf."IN ('".implode("','",$_REQUEST['estuf'])."') ";
	}
	
	if ( $_REQUEST["muncod"][0] ){
		$notmuncod = $_REQUEST['muncod_campo_excludente'] == '1' ? ' NOT ' : '';
		$filtro .= " AND e.muncod ".$notmuncod."IN ('".implode("','",$_REQUEST['muncod'])."') ";
	}
	
	if ( $_REQUEST["idimplantadora"][0] ){
		$notidimplantadora = $_REQUEST['idimplantadora_campo_excludente'] == '1' ? ' NOT ' : '';
		$filtro .= " AND o.entidunidade ".$notidimplantadora."IN ('".implode("','",$_REQUEST['idimplantadora'])."') ";
	}
	
	if ( $_REQUEST["esdid"][0] ){
		$notesdid = $_REQUEST['esdid_campo_excludente'] == '1' ? ' NOT ' : '';
		$filtroESDID .= " AND esd.esdid ".$notesdid."IN ('".implode("','",$_REQUEST['esdid'])."') ";
		$filtro .= " AND esd.esdid ".$notesdid."IN ('".implode("','",$_REQUEST['esdid'])."') ";
	}else{
		$filtroESDID .= " and esd.esdid IN (159, 173, 174, 172, 171) ";
	}
		
	if ( $_REQUEST["obrid"][0] ){
		$notobrid = $_REQUEST['obrid_campo_excludente'] == '1' ? ' NOT ' : '';
		$filtro .= " AND o.obrid ".$notobrid."IN ('".implode("','",$_REQUEST['obrid'])."') ";
	}
	
	if ( $_REQUEST["orgid"][0] ){
		$notorgid = $_REQUEST['orgid_campo_excludente'] == '1' ? ' NOT ' : '';
		$filtro .= " AND o.orgid ".$notorgid."IN ('".implode("','",$_REQUEST['orgid'])."') ";
	}
	if ( $_REQUEST["stoid"][0] ){
		$notstoid = $_REQUEST['stoid_campo_excludente'] == '1' ? ' NOT ' : '';
		$filtro .= " AND o.stoid ".$notstoid."IN ('".implode("','",$_REQUEST['stoid'])."') ";
	}
	if ( $_REQUEST["rsuid"][0] ){
		$notrsuid = $_REQUEST['rsuid_campo_excludente'] == '1' ? ' NOT ' : '';
		$filtro .= " AND rsu.rsuid ".$notrsuid."IN ('".implode("','",$_REQUEST['rsuid'])."') ";
	}	
	
	
	
	$sql = "select distinct
				   a.usucpf as cpf_usuario_inserir, 
				   u.usunome as nome_usuario_inserir, 
				   f.obrid, 
				   ent.entnome as unidade_orcamentaria, 
				   o.obrdesc || ' (Cód.:' || o.obrid || ')' as nome_obra, 
				   m.mundescricao as municipio,
				   m.estuf as uf, 
				   o.numconvenio as convenio, 
				   to_char(s.supvdt, 'dd/mm/YYYY') as data_supervisao, 
				   to_char(s.supdtinclusao, 'dd/mm/YYYY HH24:MI') as data_inclusao_supervisao, 
				   a.arqid, 
				   a.arqnome||'.'||a.arqextensao as nome_arquivo, 
				   a.arqextensao as extensao_arquivo,
				   a.arqdescricao as descricao_arquivo, 
				   a.arqtamanho as tamanho_arquivo,
				   to_char(a.arqdata,'dd/mm/YYYY') as data_arquivo, 
				   COALESCE(to_char(a.arqdata,'dd/mm/YYYY'), '00/00/0000')||' '||COALESCE(a.arqhora, '00:00') as datahora_arquivo,
				   so.stodesc as situacao_obra, 
				   org.orgdesc as orgao_obra,
				   rsu.rsudsc as tp_supervisao,
				   1 as qtd,
				   CASE WHEN arr.arcid IS NULL THEN 0 ELSE 1 END as qtdrec
			from obras.fotos f 
			inner join obras.supervisao s on s.supvid=f.supvid 
			inner join public.arquivo a on a.arqid=f.arqid 
			inner join seguranca.usuario u ON u.usucpf = a.usucpf 
			inner join obras.obrainfraestrutura o ON o.obrid = s.obrid 
			
			left join 
			    obras.repositorio rep ON rep.obrid = o.obrid and repstatus = 'A'
			left join 
			    obras.itemgrupo ig ON ig.repid = rep.repid
			left join 
			    obras.grupodistribuicao gd ON gd.gpdid = ig.gpdid and gd.gpdstatus = 'A'
			left join 
			    workflow.documento doc ON doc.docid = gd.docid
			left join 
			   workflow.estadodocumento esd ON esd.esdid = doc.esdid
							   				   {$filtroESDID}
			
			left join entidade.entidade ent ON ent.entid = o.entidunidade 
			inner join obras.situacaoobra so ON so.stoid = o.stoid 
			inner join entidade.endereco e ON e.endid = o.endid 
			inner join territorios.municipio m ON m.muncod = e.muncod 
			left join obras.tiposupervisao ts ON ts.tpsid = s.tpsid 
			left join obras.orgao org ON org.orgid = o.orgid 
			left join obras.realizacaosupervisao rsu ON rsu.rsuid = s.rsuid 
			left join public.arquivo_recuperado arr ON arr.arqid = a.arqid
			where a.arqid/1000 between 647 and 725 
			and supstatus='A' and sisid=15 and obsstatus = 'A' 
			".$filtro." 
			order by ".(($_REQUEST['colunas'])?implode(",",$_REQUEST['colunas']):"1");
							   				   
	return $sql;
	
}

function obras_monta_coluna_arquivos_corrompidos_supervisao(){
	
	$coluna = array();

	
	if( in_array('nome_arquivo', $_REQUEST['colunas']) ){
		
		array_push( $coluna, array("campo" 	  => "descricao_arquivo",
						   		   "label" 	  => "Descrição do Arquivo",
								   "type"	  => "string") );
		array_push( $coluna, array("campo" 	  => "tamanho_arquivo",
						   		   "label" 	  => "Tamanho do arquivo",
								   "type"	  => "numeric") );
		array_push( $coluna, array("campo" 	  => "datahora_arquivo",
						   		   "label" 	  => "Data da Inclusão do Arquivo",
								   "type"	  => "string") );
	}

	array_push( $coluna, array("campo" 	  => "qtd",
					   		   "label" 	  => "Quantidade de Arquivos",
					   		   "blockAgp" => "nome_arquivo",
					   		   "type"	  => "numeric") );
	
	array_push( $coluna, array("campo" 	  => "qtdrec",
					   		   "label" 	  => "Quantidade de Arquivos (Recuperados)",
					   		   "blockAgp" => "nome_arquivo",
					   		   "type"	  => "numeric") );
	
	array_push( $coluna, array("campo" 	  => "qtd",
					   		   "label" 	  => "% Recuperada",
					   		   "blockAgp" => "nome_arquivo",
					   		   "type"	  => "numeric",
							   "php"      => array("expressao" => "{qtd}>0",
												   "var"       => "per",
												   "true"      => "round({qtdrec}/{qtd}*100,1)",
												   "false"     => "0",
					   		   					   "type"	   => "numeric",
												   "html"      => "{per} %")) );
	
	
	return $coluna;
	
}


/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

if(!$_REQUEST['colunas']) {
	
	die("<script>
			alert('Selecione os agrupadores');
			window.location='obras.php?modulo=relatorio/relatorioArquivosSupervisao&acao=A';
		 </script>");
	
}


// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

// instancia a classe de relatório
$rel = new montaRelatorio();

// monta o sql, agrupador e coluna do relatório
$sql       = obras_monta_sql_arquivos_corrompidos_supervisao(); 
$agrupador = obras_monta_agp_arquivos_corrompidos_supervisao();
$coluna    = obras_monta_coluna_arquivos_corrompidos_supervisao();
$dados 	   = $db->carregar( $sql );

$rel->setAgrupador($agrupador, $dados); 
$rel->setColuna($coluna);
$rel->setTolizadorLinha(true);
$rel->setTotNivel(true);

?>
<html>
	<head>
		<title><?php echo $GLOBALS['parametros_sistema_tela']['nome_e_orgao']; ?></title>
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
