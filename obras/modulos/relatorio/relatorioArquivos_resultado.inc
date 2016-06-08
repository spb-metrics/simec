<?php
function obras_monta_agp_painel_gerencial(){

	$agrupador = $_REQUEST["colunas"];
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array( "qtd","qtdrec" )
				);
	
	foreach ( $agrupador as $val ){
		switch( $val ){
			case "usunome":
				array_push($agp['agrupador'], array(
													"campo" => "usunome",
											  		"label" => "Nome do usuário que inseriu")										
									   				);
			break;
			case "obrid":
				array_push($agp['agrupador'], array(
													"campo" => "obrid",
											  		"label" => "ID da Obra")										
									   				);
			break;
			case "entnome":
				array_push($agp['agrupador'], array(
													"campo" => "entnome",
											  		"label" => "Unidade ResponsÃ¡vel pela Obra")										
									   				);
			break;
			case "obrdesc":
				array_push($agp['agrupador'], array(
													"campo" => "obrdesc",
											  		"label" => "Nome da Obra")										
									   				);
			break;
			case "estuf":
				array_push($agp['agrupador'], array(
													"campo" => "estuf",
											  		"label" => "UF")										
									   				);
			break;
			break;
			case "mundescricao":
				array_push($agp['agrupador'], array(
													"campo" => "mundescricao",
											  		"label" => "Município")										
									   				);
			break;
			case "numconvenio":
				array_push($agp['agrupador'], array(
													"campo" => "numconvenio",
											  		"label" => "Convênio")										
									   				);
			break;
			case "arqdata":
				array_push($agp['agrupador'], array(
													"campo" => "arqdata",
											  		"label" => "Data da inclusão (arquivo)")										
									   				);
			break;
			case "stodesc":
				array_push($agp['agrupador'], array(
													"campo" => "stodesc",
											  		"label" => "Situação da Obra")										
									   				);
			break;
			case "orgdesc":
				array_push($agp['agrupador'], array(
													"campo" => "orgdesc",
											  		"label" => "Orgão da obra")										
									   				);
			break;
			case "arqnome":
				array_push($agp['agrupador'], array(
													"campo" => "arqnome",
											  		"label" => "Dados do Arquivo")										
									   				);
				array_push($agp['agrupadoColuna'], 'arquivo' );
				array_push($agp['agrupadoColuna'], 'arqdescricao' );
				array_push($agp['agrupadoColuna'], 'arqtamanho' );
				array_push($agp['agrupadoColuna'], 'arqdata' );
				array_push($agp['agrupadoColuna'], 'arqhora' );
			break;
		}	
	}
	
	return $agp;
	
}

function obras_monta_sql_painel_gerencial(){
	
	global $db,$agrupador;
	$where = array();
	
	$filtro="";

	// Filtros
	if ( $_REQUEST["usucpf"][0] ){
		$filtro .= " AND u.usucpf ".$notusucpf."IN ('".implode("','",$_REQUEST["usucpf"])."') ";
	}
	if ( $_REQUEST["obridid"] ){
		$filtro .= " AND o.obrid = ".$_REQUEST["obridid"];
	}
	if ( $_REQUEST["entid"][0] ){
		$filtro .= " AND ent.entid ".$notentid."IN (".implode(",",$_REQUEST["entid"]).") ";
	}
	if ( $_REQUEST["obrid"][0] ){
		$filtro .= " AND o.obrid ".$notobrid."IN (".implode(",",$_REQUEST["obrid"]).") ";
	}
	if ( $_REQUEST["estuf"][0] ){
		$filtro .= " AND m.estuf ".$notestuf."IN ('".implode("','",$_REQUEST["estuf"])."') ";
	}
	if ( $_REQUEST["muncod"][0] ){
		$filtro .= " AND m.muncod ".$notmuncod."IN ('".implode("','",$_REQUEST["muncod"])."') ";
	}
	if ( $_REQUEST["numconvenio"][0] ){
		$filtro .= " AND o.numconvenio ".$notnumconvenio."IN ('".implode("','",$_REQUEST["numconvenio"])."') ";
	}
	if ( $_REQUEST["stoid"][0] ){
		$filtro .= " AND o.stoid ".$notstoid."IN (".implode(",",$_REQUEST["stoid"]).") ";
	}
	if ( $_REQUEST["orgid"][0] ){
		$filtro .= " AND o.orgid ".$notorgid."IN (".implode(",",$_REQUEST["orgid"]).") ";
	}
	
	if ($_REQUEST["rsuid"][0]){
		$perfil = array();
		foreach($_REQUEST["rsuid"] as $rsuid){
			switch ($rsuid){
				case 1:
					array_push($perfil, 177, 231, 164, 163);
					break;
				case 2:
					array_push($perfil, 160, 166, 165, 162, 425, 230);
					break;
				case 3:
					array_push($perfil, 426);
					break;
			}
		}
		$notrsuid = $_REQUEST['rsuid_campo_excludente'] == '1' ? ' NOT ' : '';
		$inner .= "INNER JOIN 
						(SELECT DISTINCT 
								usucpf 
						 FROM 
						 	seguranca.perfilusuario WHERE pflcod {$notrsuid}IN(".implode(",", $perfil).")) pu ON pu.usucpf = u.usucpf";
	}
	
	$agrupador = $_REQUEST["colunas"];
	
	// ordenador
	array_push($agrupador, "qtd");
//	ver($_REQUEST);

	// monta o sql 
	$sql = "SELECT 
				u.usunome,
				f.obrid, 
				o.obrdesc || ' (Cod. ' || f.obrid || ')' as obrdesc , 
				ent.entid, 
				ent.entnome, 
				m.estuf, 
				m.mundescricao,
				o.numconvenio,
				org.orgdesc, 
				so.stodesc,
				a.arqid, 
				a.arqid || ' - ' || a.arqnome||'.'||a.arqextensao as arquivo, 
				a.arqdescricao, 
				a.arqtamanho, 
				to_char(a.arqdata,'dd/mm/YYYY') as arqdata,
				a.arqhora,
				1 as qtd,
				CASE WHEN arr.arcid IS NULL THEN 0 ELSE 1 END as qtdrec
			FROM 
				obras.arquivosobra f 
			INNER JOIN obras.tipoarquivo 	   ta ON ta.tpaid = f.tpaid 
			INNER JOIN public.arquivo 			a ON a.arqid=f.arqid 
			INNER JOIN seguranca.usuario 		u ON u.usucpf = a.usucpf 
			{$inner}
			INNER JOIN obras.obrainfraestrutura o ON o.obrid = f.obrid 
			LEFT  JOIN obras.orgao			  org ON org.orgid = o.orgid
			LEFT  JOIN entidade.entidade 	  ent ON ent.entid = o.entidunidade 
			INNER JOIN obras.situacaoobra 	   so ON so.stoid = o.stoid 
			INNER JOIN entidade.endereco 	    e ON e.endid = o.endid 
			INNER JOIN territorios.municipio 	m ON m.muncod = e.muncod 
			LEFT join public.arquivo_recuperado arr ON arr.arqid = a.arqid
			WHERE 
				a.arqid/1000 BETWEEN 647 
				AND 725 
				AND aqostatus='A' AND sisid=15  AND obsstatus = 'A' 
				{$filtro}
			ORDER BY 
				".implode(",",$agrupador) ;
//	ver($sql,d);
	return $sql;
	
}

function obras_monta_coluna_painel_gerencial(){
	
	$coluna = array();

	if( in_array( 'arqnome',$_REQUEST['colunas']) ){
		array_push( $coluna, array("campo" 	  => "arquivo",
						   		   "label" 	  => "Arquivo",
								   "type"	  => "string") );
		array_push( $coluna, array("campo" 	  => "arqdescricao",
						   		   "label" 	  => "Descrição do Arquivo",
								   "type"	  => "string") );
		array_push( $coluna, array("campo" 	  => "arqtamanho",
						   		   "label" 	  => "Extensão do Arquivo",
								   "type"	  => "string") );
		array_push( $coluna, array("campo" 	  => "arqdata",
						   		   "label" 	  => "Data da Inclusão do Arquivo",
								   "type"	  => "string") );
		array_push( $coluna, array("campo" 	  => "arqhora",
						   		   "label" 	  => "Hora da Inclusão do Arquivo",
								   "type"	  => "string") );
	}
	array_push( $coluna, array("campo" 	  => "qtd",
					   		   "label" 	  => "Quantidade de Arquivos",
					   		   "blockAgp" => "nomearquivo",
					   		   "type"	  => "numeric") );
	array_push( $coluna, array("campo" 	  => "qtdrec",
					   		   "label" 	  => "Quantidade de Arquivos Recuperados",
					   		   "blockAgp" => "nomearquivo",
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


// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

// instancia a classe de relatório
$rel = new montaRelatorio();

// monta o sql, agrupador e coluna do relatório
$sql       = obras_monta_sql_painel_gerencial(); 
//ver($sql,d);
$agrupador = obras_monta_agp_painel_gerencial();
$coluna    = obras_monta_coluna_painel_gerencial();
$dados 	   = $db->carregar( $sql );
//ver($sql,d);

$rel->setAgrupador($agrupador, $dados); 
$rel->setColuna($coluna);
$rel->setTolizadorLinha(true);
$rel->setEspandir(false);
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
