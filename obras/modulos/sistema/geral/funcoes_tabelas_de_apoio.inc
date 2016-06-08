<?php

function retornaSelectMontaTabela($nomeTabela) {
	global $db;
	if($nomeTabela == "itenscomposicao")
		return $db->executar("SELECT itcid as codigo,itcdesc as descricao,itcdescservico as descservico FROM obras.itenscomposicao WHERE itcstatus = 'A' ORDER BY itcdesc");
	if($nomeTabela == "tiporestricao")
		return $db->executar("SELECT trtid as codigo,trtdesc as descricao FROM obras.tiporestricao WHERE tdtdtstatus = 'A' ORDER BY trtdesc");
	if($nomeTabela == "tiporespcontato")
		return $db->executar("SELECT tprcid as codigo,tprcdesc as descricao FROM obras.tiporespcontato WHERE tprstatus = 'A' ORDER BY tprcdesc");
	if($nomeTabela == "tipoobra")
		return $db->executar("SELECT tobaid as codigo,tobadesc as descricao FROM obras.tipoobra WHERE tobstatus = 'A' ORDER BY tobadesc");
	if($nomeTabela == "unidademedida")
		return $db->executar("SELECT umdid as codigo,umdeesc as descricao FROM obras.unidademedida WHERE umdstatus = 'A' ORDER BY umdeesc");
	if($nomeTabela == "tipoarquivo")
		return $db->executar("SELECT tpaid as codigo,tpadesc as descricao FROM obras.tipoarquivo WHERE tpastatus = 'A' ORDER BY tpadesc");
	if($nomeTabela == "itensdetalhamento")
		return $db->executar("SELECT itdid as codigo,itddesc as descricao FROM obras.itensdetalhamento WHERE itdstatus = 'A' ORDER BY itddesc");
	if($nomeTabela == "situacaoobra")
		return $db->executar("SELECT stoid as codigo,stodesc as descricao FROM obras.situacaoobra WHERE stostatus = 'A' ORDER BY stodesc");
	if($nomeTabela == "desempenhoconstrutora")
		return $db->executar("SELECT dcnid as codigo,dcndesc as descricao FROM obras.desempenhoconstrutora WHERE dcnstatus = 'A' ORDER BY dcndesc");
	if($nomeTabela == "qualidadeobra")
		return $db->executar("SELECT qlbid as codigo,qlbdesc as descricao FROM obras.qualidadeobra WHERE qlbstatus = 'A' ORDER BY qlbdesc");
	if($nomeTabela == "programafonte")
		return $db->executar("SELECT prfid as codigo, prfdesc as descricao, orgid as idorgao FROM obras.programafonte ORDER BY prfid");
	if($nomeTabela == "tipologiaobra")
		return $db->executar("SELECT tpoid as codigo, tpodsc as descricao, tpodetalhe as detalhe, cloid as idclasse,tpomedida as medida FROM obras.tipologiaobra ORDER BY tpoid");
	if($nomeTabela == "programatipologia")
		return $db->executar("SELECT distinct prfid as codigo, tpoid as codigotipologia, ptpid FROM obras.programatipologia ORDER BY prfid");
}

function podeExcluir($nomeTabela, $codigoRegistro) {
	global $db;
	
	if($nomeTabela == "itenscomposicao")
		$sqlPodeExcluir = "SELECT count(*) FROM obras.itenscomposicaoobra WHERE itcid = ".$codigoRegistro;
	if($nomeTabela == "tiporestricao")
		$sqlPodeExcluir = "SELECT count(*) FROM obras.restricaoobra WHERE trtid = ".$codigoRegistro;
	if($nomeTabela == "tiporespcontato")
		$sqlPodeExcluir = "SELECT count(*) FROM obras.responsavelcontatos WHERE tprcid = ".$codigoRegistro;
	if($nomeTabela == "tipoobra")
		$sqlPodeExcluir = "SELECT count(*) FROM obras.obrainfraestrutura WHERE tobraid = ".$codigoRegistro;
	if($nomeTabela == "tipoarquivo")
		$sqlPodeExcluir = "SELECT count(*) FROM obras.arquivosobra WHERE tpaid = ".$codigoRegistro;
	if($nomeTabela == "itensdetalhamento")
		$sqlPodeExcluir = "SELECT count(*) FROM obras.itensdetalhamentoobra WHERE itdid = ".$codigoRegistro;
	if($nomeTabela == "desempenhoconstrutora")
		$sqlPodeExcluir = "SELECT count(*) FROM obras.supervisao WHERE dcnid = ".$codigoRegistro;
	if($nomeTabela == "qualidadeobra")
		$sqlPodeExcluir = "SELECT count(*) FROM obras.supervisao WHERE qlbid = ".$codigoRegistro;
	if($nomeTabela == "unidademedida") {
		$sqlPodeExcluir = "SELECT sum(valor)
						FROM
						(
						SELECT count(*) as valor
						FROM obras.infraestrutura i
						WHERE i.umdidareaconstruida = ".$codigoRegistro." OR 
						      i.umdidareareforma = ".$codigoRegistro." OR
						      i.umdidareaampliada = ".$codigoRegistro."							
						UNION							
						SELECT count(*) as valor
						FROM obras.obrainfraestrutura o
						WHERE o.umdidobraconstruida = ".$codigoRegistro." OR
						      o.umdidareaserconstruida = ".$codigoRegistro." OR
						      o.umdidareaserreformada = ".$codigoRegistro." OR
						      o.umdidareaserampliada = ".$codigoRegistro."
						) as foo";
	}
	if($nomeTabela == "situacaoobra") {
		$sqlPodeExcluir = "SELECT sum(valor)
						FROM
						(
						SELECT count(*) as valor
						FROM obras.obrainfraestrutura oi
						WHERE oi.stoid = ".$codigoRegistro."						
						UNION						
						SELECT count(*) as valor
						FROM obras.supervisao sv
						WHERE sv.stoid = ".$codigoRegistro."
						) as foo";
	}
	
	if($nomeTabela == "programafonte")
		$sqlPodeExcluir = "SELECT count(*) FROM obras.programafonte WHERE prfid = ".$codigoRegistro;
	if($nomeTabela == "tipologiaobra")
		$sqlPodeExcluir = "SELECT count(*) FROM obras.tipologiaobra WHERE tpoid = ".$codigoRegistro;
	if($nomeTabela == "programatipologia")
		$sqlPodeExcluir = "SELECT count(*) FROM obras.programafonte WHERE prfid = ".$codigoRegistro;
		
	if($db->pegaUm($sqlPodeExcluir) == 0)
		return true;
	else
		return false;
}

function insereDadosItensComposicao($retornoTabelaID, $retornoTabelaDescricao, $retornoTabelaDescricaoServicos) {
	global $db;
	
	//Deleta registros se necessário.
	$sql = $db->executar("SELECT itcid FROM obras.itenscomposicao WHERE itcstatus = 'A'");
	while(($dados = pg_fetch_array($sql)) != false) {
		$itcid = $dados['itcid'];
		$cont = 0;
		
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if($itcid == trim($retornoTabelaID[$i]))
				$cont++;
		}
		
		if($cont == 0) {
			$sql_delete = "DELETE FROM obras.itenscomposicao WHERE itcid = ".$itcid;	
			$db->executar($sql_delete);
			$db->commit();
		}
	}
	
	if(count($retornoTabelaID) > 0) {
		// Executa INSERT's e/ou UPDATE's na tabela. 
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if(trim($retornoTabelaID[$i]) == 'xx') {
				$sql = "INSERT INTO 
							obras.itenscomposicao(itcdesc,itcstatus,itcdtinclusao,itcdescservico) 
						VALUES
							('".trim($retornoTabelaDescricao[$i])."', 'A', now(), '".trim($retornoTabelaDescricaoServicos[$i])."');";	
				$db->executar($sql);
				$db->commit();
			} else {
				$sql = "UPDATE 
							obras.itenscomposicao 
						SET 
							itcdesc = '".trim($retornoTabelaDescricao[$i])."',
							itcdescservico = '".trim($retornoTabelaDescricaoServicos[$i])."'						 
						WHERE 
							itcid = ".trim($retornoTabelaID[$i]);
				$db->executar($sql);
				$db->commit();
			}
		}
	}
}

function insereDadosTipoRestricao($retornoTabelaID, $retornoTabelaDescricao) {
	global $db;
	
	// Deleta registros se necessário.
	$sql = $db->executar("SELECT trtid FROM obras.tiporestricao WHERE tdtdtstatus = 'A'");
	while(($dados = pg_fetch_array($sql)) != false) {
		$trtid = $dados['trtid'];
		$cont = 0;
		
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if($trtid == trim($retornoTabelaID[$i]))
				$cont++;
		}
		
		if($cont == 0) {
			$sql_delete = "DELETE FROM obras.tiporestricao WHERE trtid = ".$trtid;	
			$db->executar($sql_delete);
			$db->commit();
		}
	}
	
	if(count($retornoTabelaID) > 0) {
		// Executa INSERT's e/ou UPDATE's na tabela. 
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if(trim($retornoTabelaID[$i]) == 'xx') {
				$sql = "INSERT INTO 
							obras.tiporestricao(trtdesc,tdtdtstatus,trtdtinclusao) 
						VALUES
							('".trim($retornoTabelaDescricao[$i])."', 'A', now());";	
				$db->executar($sql);
				$db->commit();
			} else {
				$sql = "UPDATE 
							obras.tiporestricao 
						SET 
							trtdesc = '".trim($retornoTabelaDescricao[$i])."'							 
						WHERE 
							trtid = ".trim($retornoTabelaID[$i]);
				$db->executar($sql);
				$db->commit();
			}
		}
	}
}

function insereDadosTipoRespContato($retornoTabelaID, $retornoTabelaDescricao) {
	global $db;
	
	// Deleta registros se necessário.
	$sql = $db->executar("SELECT tprcid FROM obras.tiporespcontato WHERE tprstatus = 'A'");
	while(($dados = pg_fetch_array($sql)) != false) {
		$tprcid = $dados['tprcid'];
		$cont = 0;
		
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if($tprcid == trim($retornoTabelaID[$i]))
				$cont++;
		}
		
		if($cont == 0) {
			$sql_delete = "DELETE FROM obras.tiporespcontato WHERE tprcid = ".$tprcid;	
			$db->executar($sql_delete);
			$db->commit();
		}
	}
	
	if(count($retornoTabelaID) > 0) {
		// Executa INSERT's e/ou UPDATE's na tabela. 
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if(trim($retornoTabelaID[$i]) == 'xx') {
				$sql = "INSERT INTO 
							obras.tiporespcontato(tprcdesc,tprstatus,tprdtinclusao) 
						VALUES
							('".trim($retornoTabelaDescricao[$i])."', 'A', now());";	
				$db->executar($sql);
				$db->commit();
			} else {
				$sql = "UPDATE 
							obras.tiporespcontato 
						SET 
							tprcdesc = '".trim($retornoTabelaDescricao[$i])."'							 
						WHERE 
							tprcid = ".trim($retornoTabelaID[$i]);
				$db->executar($sql);
				$db->commit();
			}
		}
	}	
}

function insereDadosTipoObra($retornoTabelaID, $retornoTabelaDescricao) {
	global $db;
	
	// Deleta registros se necessário.
	$sql = $db->executar("SELECT tobaid FROM obras.tipoobra WHERE tobstatus = 'A'");
	while(($dados = pg_fetch_array($sql)) != false) {
		$tobaid = $dados['tobaid'];
		$cont = 0;
		
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if($tobaid == trim($retornoTabelaID[$i]))
				$cont++;
		}
		
		if($cont == 0) {
			$sql_delete = "DELETE FROM obras.tipoobra WHERE tobaid = ".$tobaid;	
			$db->executar($sql_delete);
			$db->commit();
		}
	}
	
	if(count($retornoTabelaID) > 0) {
		// Executa INSERT's e/ou UPDATE's na tabela. 
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if(trim($retornoTabelaID[$i]) == 'xx') {
				$sql = "INSERT INTO 
							obras.tipoobra(tobadesc,tobstatus,tobdtinclusao) 
						VALUES
							('".trim($retornoTabelaDescricao[$i])."', 'A', now());";	
				$db->executar($sql);
				$db->commit();
			} else {
				$sql = "UPDATE 
							obras.tipoobra 
						SET 
							tobadesc = '".trim($retornoTabelaDescricao[$i])."'							 
						WHERE 
							tobaid = ".trim($retornoTabelaID[$i]);
				$db->executar($sql);
				$db->commit();
			}
		}
	}
}

function insereDadosUnidadeMedida($retornoTabelaID, $retornoTabelaDescricao) {
	global $db;
	
	// Deleta registros se necessário.
	$sql = $db->executar("SELECT umdid FROM obras.unidademedida WHERE umdstatus = 'A'");
	while(($dados = pg_fetch_array($sql)) != false) {
		$umdid = $dados['umdid'];
		$cont = 0;
		
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if($umdid == trim($retornoTabelaID[$i]))
				$cont++;
		}
		
		if($cont == 0) {
			$sql_delete = "DELETE FROM obras.unidademedida WHERE umdid = ".$umdid;	
			$db->executar($sql_delete);
			$db->commit();
		}
	}
	
	if(count($retornoTabelaID) > 0) {
		// Executa INSERT's e/ou UPDATE's na tabela. 
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if(trim($retornoTabelaID[$i]) == 'xx') {
				$sql = "INSERT INTO 
							obras.unidademedida(umdeesc,umdstatus,umddtinclusao) 
						VALUES
							('".trim($retornoTabelaDescricao[$i])."', 'A', now());";	
				$db->executar($sql);
				$db->commit();
			} else {
				$sql = "UPDATE 
							obras.unidademedida 
						SET 
							umdeesc = '".trim($retornoTabelaDescricao[$i])."'							 
						WHERE 
							umdid = ".trim($retornoTabelaID[$i]);
				$db->executar($sql);
				$db->commit();
			}
		}
	}
}

function insereDadosTipoArquivo($retornoTabelaID, $retornoTabelaDescricao) {
	global $db;
	
	// Deleta registros se necessário.
	$sql = $db->executar("SELECT tpaid FROM obras.tipoarquivo WHERE tpastatus = 'A'");
	while(($dados = pg_fetch_array($sql)) != false) {
		$tpaid = $dados['tpaid'];
		$cont = 0;
		
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if($tpaid == trim($retornoTabelaID[$i]))
				$cont++;
		}
		
		if($cont == 0) {
			$sql_delete = "DELETE FROM obras.tipoarquivo WHERE tpaid = ".$tpaid;	
			$db->executar($sql_delete);
			$db->commit();
		}
	}
	
	if(count($retornoTabelaID) > 0) {
		// Executa INSERT's e/ou UPDATE's na tabela. 
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if(trim($retornoTabelaID[$i]) == 'xx') {
				$sql = "INSERT INTO 
							obras.tipoarquivo(tpadesc,tpastatus,tpadtinclusao) 
						VALUES
							('".trim($retornoTabelaDescricao[$i])."', 'A', now());";	
				$db->executar($sql);
				$db->commit();
			} else {
				$sql = "UPDATE 
							obras.tipoarquivo 
						SET 
							tpadesc = '".trim($retornoTabelaDescricao[$i])."'							 
						WHERE 
							tpaid = ".trim($retornoTabelaID[$i]);
				$db->executar($sql);
				$db->commit();
			}
		}
	}
}

function insereDadosItensDetalhamento($retornoTabelaID, $retornoTabelaDescricao) {
	global $db;
	
	// Deleta registros se necessário.
	$sql = $db->executar("SELECT itdid FROM obras.itensdetalhamento WHERE itdstatus = 'A'");
	while(($dados = pg_fetch_array($sql)) != false) {
		$itdid = $dados['itdid'];
		$cont = 0;
		
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if($itdid == trim($retornoTabelaID[$i]))
				$cont++;
		}
		
		if($cont == 0) {
			$sql_delete = "DELETE FROM obras.itensdetalhamento WHERE itdid = ".$itdid;	
			$db->executar($sql_delete);
			$db->commit();
		}
	}
	
	if(count($retornoTabelaID) > 0) {
		// Executa INSERT's e/ou UPDATE's na tabela. 
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if(trim($retornoTabelaID[$i]) == 'xx') {
				$sql = "INSERT INTO 
							obras.itensdetalhamento(itddesc,itdstatus,idtdtinclusao) 
						VALUES
							('".trim($retornoTabelaDescricao[$i])."', 'A', now());";	
				$db->executar($sql);
				$db->commit();
			} else {
				$sql = "UPDATE 
							obras.itensdetalhamento 
						SET 
							itddesc = '".trim($retornoTabelaDescricao[$i])."'							 
						WHERE 
							itdid = ".trim($retornoTabelaID[$i]);
				$db->executar($sql);
				$db->commit();
			}
		}
	}
}

function insereDadosSituacaoObra($retornoTabelaID, $retornoTabelaDescricao) {
	global $db;
	
	// Deleta registros se necessário.
	$sql = $db->executar("SELECT stoid FROM obras.situacaoobra WHERE stostatus = 'A'");
	while(($dados = pg_fetch_array($sql)) != false) {
		$stoid = $dados['stoid'];
		$cont = 0;
		
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if($stoid == trim($retornoTabelaID[$i]))
				$cont++;
		}
		
		if($cont == 0) {
			$sql_delete = "DELETE FROM obras.situacaoobra WHERE stoid = ".$stoid;	
			$db->executar($sql_delete);
			$db->commit();
		}
	}
	
	if(count($retornoTabelaID) > 0) {
		// Executa INSERT's e/ou UPDATE's na tabela. 
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if(trim($retornoTabelaID[$i]) == 'xx') {
				$sql = "INSERT INTO 
							obras.situacaoobra(stodesc,stostatus,stodtinclusao) 
						VALUES
							('".trim($retornoTabelaDescricao[$i])."', 'A', now());";	
				$db->executar($sql);
				$db->commit();
			} else {
				$sql = "UPDATE 
							obras.situacaoobra 
						SET 
							stodesc = '".trim($retornoTabelaDescricao[$i])."'							 
						WHERE 
							stoid = ".trim($retornoTabelaID[$i]);
				$db->executar($sql);
				$db->commit();
			}
		}
	}
}

function insereDadosDesempenhoConstrutora($retornoTabelaID, $retornoTabelaDescricao) {
	global $db;
	
	// Deleta registros se necessário.
	$sql = $db->executar("SELECT dcnid FROM obras.desempenhoconstrutora WHERE dcnstatus = 'A'");
	while(($dados = pg_fetch_array($sql)) != false) {
		$dcnid = $dados['dcnid'];
		$cont = 0;
		
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if($dcnid == trim($retornoTabelaID[$i]))
				$cont++;
		}
		
		if($cont == 0) {
			$sql_delete = "DELETE FROM obras.desempenhoconstrutora WHERE dcnid = ".$dcnid;	
			$db->executar($sql_delete);
			$db->commit();
		}
	}
	
	if(count($retornoTabelaID) > 0) {
		// Executa INSERT's e/ou UPDATE's na tabela. 
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if(trim($retornoTabelaID[$i]) == 'xx') {
				$sql = "INSERT INTO 
							obras.desempenhoconstrutora(dcndesc,dcnstatus,dcbdtinclusao) 
						VALUES
							('".trim($retornoTabelaDescricao[$i])."', 'A', now());";	
				$db->executar($sql);
				$db->commit();
			} else {
				$sql = "UPDATE 
							obras.desempenhoconstrutora 
						SET 
							dcndesc = '".trim($retornoTabelaDescricao[$i])."'							 
						WHERE 
							dcnid = ".trim($retornoTabelaID[$i]);
				$db->executar($sql);
				$db->commit();
			}
		}
	}
}

function insereDadosQualidadeObra($retornoTabelaID, $retornoTabelaDescricao) {
	global $db;
	
	// Deleta registros se necessário.
	$sql = $db->executar("SELECT qlbid FROM obras.qualidadeobra WHERE qlbstatus = 'A'");
	while(($dados = pg_fetch_array($sql)) != false) {
		$qlbid = $dados['qlbid'];
		$cont = 0;
		
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if($qlbid == trim($retornoTabelaID[$i]))
				$cont++;
		}
		
		if($cont == 0) {
			$sql_delete = "DELETE FROM obras.qualidadeobra WHERE qlbid = ".$qlbid;	
			$db->executar($sql_delete);
			$db->commit();
		}
	}
	
	if(count($retornoTabelaID) > 0) {
		// Executa INSERT's e/ou UPDATE's na tabela. 
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if(trim($retornoTabelaID[$i]) == 'xx') {
				$sql = "INSERT INTO 
							obras.qualidadeobra(qlbdesc,qlbstatus,qlbdtinclusao) 
						VALUES
							('".trim($retornoTabelaDescricao[$i])."', 'A', now());";	
				$db->executar($sql);
				$db->commit();
			} else {
				$sql = "UPDATE 
							obras.qualidadeobra 
						SET 
							qlbdesc = '".trim($retornoTabelaDescricao[$i])."'							 
						WHERE 
							qlbid = ".trim($retornoTabelaID[$i]);
				$db->executar($sql);
				$db->commit();
			}
		}
	}
}

function insereDadosProgramaFonte($retornoTabelaID, $retornoTabelaDescricao, $retornoTabelaDescricaoServicos) {
global $db;
	
	
	// Deleta registros se necessário.
	$sql = $db->executar("SELECT prfid FROM obras.programafonte");
	while($dados = pg_fetch_array($sql)) {
		$prfid = $dados['prfid'];
		$cont = 0;
		
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if($prfid == trim($retornoTabelaID[$i]))
				$cont++;
		}
		
		if($cont == 0) {
			$sql_delete = "DELETE FROM obras.programafonte WHERE prfid = ".$prfid;	
			$db->executar($sql_delete);
			$db->commit();
		}
	}
	
	if(count($retornoTabelaID) > 0) {
		// Executa INSERT's e/ou UPDATE's na tabela. 
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if(trim($retornoTabelaID[$i]) == 'xx') {
				$sql = "INSERT INTO 
							obras.programafonte(prfdesc,orgid) 
						VALUES
							('".trim($retornoTabelaDescricao[$i])."', ".trim($retornoTabelaDescricaoServicos[$i]).");";	
				$db->executar($sql);			
				$db->commit();
			} else {
				$sql = "UPDATE 
							obras.programafonte 
						SET 
							prfdesc = '".trim($retornoTabelaDescricao[$i])."'							 
						WHERE 
							prfid = ".trim($retornoTabelaID[$i]);
				$db->executar($sql);				
				$db->commit();
			}
		}
	}
}


function insereDadosTipologiaObra($retornoTabelaID, $retornoTabelaDescricao, $retornoTabelaDescricaoServicos, $retornoTabelaMedidaServicos) {
global $db;
//	/dump($retornoTabelaID).die;
	// Deleta registros se necessário.
	$sql = $db->executar("SELECT tpoid FROM obras.tipologiaobra");
	while($dados = pg_fetch_array($sql)) {
		$tpoid = $dados['tpoid'];
		$cont = 0;
		
		for($i=0; $i < count($retornoTabelaID); $i++) {
			if($tpoid== trim($retornoTabelaID[$i]))
				$cont++;
		}
		
		if($cont == 0) {
			$sql_delete = "DELETE FROM obras.tipologiaobra WHERE tpoid = ".$tpoid;	
			$db->executar($sql_delete);
			$db->commit();
		}
	}
	
	if(count($retornoTabelaID) > 0) {
		// Executa INSERT's e/ou UPDATE's na tabela. 
		for($i=0; $i < count($retornoTabelaID); $i++) {
			//intens novos
			if(trim($retornoTabelaID[$i]) == 'xx') {
				if( empty($retornoTabelaMedidaServicos[$i]) ){
					$valor = " NULL ";
				} else {
					$valor = " '" . str_replace( ".", "", pg_escape_string(trim($retornoTabelaMedidaServicos[$i])) ) . "' ";
					$valor = str_replace( ",", ".", $valor );
				}
				$sql = "INSERT INTO 
							obras.tipologiaobra(cloid, tpodsc,tpodetalhe,tpomedida) 
						VALUES
							(1, '" . pg_escape_string(trim($retornoTabelaDescricao[$i])) . "', '" . pg_escape_string(trim($retornoTabelaDescricaoServicos[$i])) . "', $valor );";	
				
				$db->executar($sql);			
//				$db->commit();
			} else {
				// itens editados	
				if( empty($retornoTabelaMedidaServicos[$i]) ){
					$valor = " NULL ";
				} else {
					
						$valor = " '" . str_replace( ".", "", pg_escape_string(trim($retornoTabelaMedidaServicos[$i])) ) . "' ";
						$valor = str_replace( ",", ".", $valor );
					
				}
				$sql = "UPDATE 
							obras.tipologiaobra 
						SET 
							cloid = 1,
							tpodsc = '" . pg_escape_string(trim($retornoTabelaDescricao[$i])) . "',	
							tpodetalhe = '" . pg_escape_string(trim($retornoTabelaDescricaoServicos[$i])) . "',	
							tpomedida =".$valor."					 
						WHERE 
							tpoid = ".trim($retornoTabelaID[$i]);

				$db->executar($sql);				
//				$db->commit();
			}
			$db->commit();
		}
//		// Executa INSERT's e/ou UPDATE's na tabela. 
//		for($i=0; $i < count($retornoTabelaID); $i++) {
//			//intens novos
//			if(trim($retornoTabelaID[$i]) == 'xx') {
//				$sql = "INSERT INTO 
//							obras.tipologiaobra(tpodsc,cloid) 
//						VALUES
//							('".trim($retornoTabelaDescricao[$i])."', ".trim($retornoTabelaDescricaoServicos[$i]).");";	
//				
//				$db->executar($sql);			
//				$db->commit();
//			// itens editados	
//			} else {
//				$sql = "UPDATE 
//							obras.tipologiaobra 
//						SET 
//							tpodsc = '".trim($retornoTabelaDescricao[$i])."',	
//							cloid = ".trim($retornoTabelaDescricaoServicos[$i])."						 
//						WHERE 
//							tpoid = ".trim($retornoTabelaID[$i]);
//				$db->executar($sql);				
//				$db->commit();
//			}
//		}
	}
}

//function insereDadosProgramaTipologia($dados_enviados) {
//global $db;
//	//$registro[0] corresponde ptpid dos registros alterados
//	//$registro[1] prfid
//	//$registro[1] tpoid
//	// Deleta registros se necessário.
//	
//	
//	$sql = $db->executar("SELECT prfid FROM obras.programatipologia");
//	while($dados = pg_fetch_array($sql)) {
//		$prfid = $dados['prfid'];
//		$cont = 0;	
//		
//		for($i=0; $i < (count($dados_enviados) - 1); $i++) {
//			$registro = explode(';', $dados_enviados[$i]);
//			if($prfid == trim($registro[1]))
//				$cont++;
//		}					
//		if($cont == 0) {
//			$sql_delete = "DELETE FROM obras.programatipologia WHERE prfid = ".$prfid;		
//			$db->executar($sql_delete);
//			$db->commit();
//		}
//	}
//	
//	if(count($dados_enviados) > 0){
//		for($i=0; $i < (count($dados_enviados) - 1); $i++) {			
//			$registro = explode( ';',$dados_enviados[$i]);		
//			
//			if($registro[0] == 'n'){
//				$sql = "INSERT INTO 
//							obras.programatipologia(prfid,tpoid) 
//						VALUES
//							('".trim($registro[1])."', ".trim($registro[2]).");";	
//			
//				$db->executar($sql);			
//				$db->commit();
//			}else{				
//				$sql = "UPDATE 
//							obras.programatipologia 
//						SET 
//							prfid = '".trim($registro[1])."',
//							tpoid = '".trim($registro[2])."'							 
//						WHERE 
//							ptpid = ".trim($registro[0]);
//			
//				$db->executar($sql);				
//				$db->commit();				
//			}		
//		}	
//	}	
//}

function insereDadosProgramaTipologia(){
	global $db;
	
	$dados = $_REQUEST;
	
	if ($dados['ptpid']){
		$sql = sprintf("UPDATE obras.programatipologia
						SET 
							prfid=%d, tpoid=%d, cloid=%d
						WHERE 
							ptpid=%d;"
						, $dados['prfid']
						, $dados['tpoid']
						, $dados['cloid']
						, $dados['ptpid']);
	}else{
		$sql = sprintf("INSERT INTO obras.programatipologia(
					    	prfid, tpoid, cloid)
					    VALUES (
					    	%d, %d, %d
					    );"
						, $dados['prfid']
						, $dados['tpoid']
						, $dados['cloid']);
	}
	
	$db->executar($sql);
	$db->commit();
}

function ExcluirProgramaTipologia($ptpid = null){
	global $db;

	$ptpid = $ptpid ? $ptpid : $_REQUEST['ptpid'];
	
	if ( !$ptpid ){return;}
	
	$sql = sprintf("DELETE FROM
						obras.programatipologia 
					WHERE
						ptpid = %d"
					, $ptpid);
					
	$db->executar($sql);
	$db->commit();	
}

function carregaProgramaTipologia($ptpid = null){
	global $db;
	
	$ptpid = $ptpid ? $ptpid : $_REQUEST['ptpid'];
	
	if ( !$ptpid ){return array();}
	
	$sql = sprintf("SELECT
						ptpid, prfid, pt.cloid, pt.tpoid, tpodsc, tpodetalhe
					FROM
						obras.programatipologia pt
					INNER JOIN 
						obras.tipologiaobra tpo ON tpo.tpoid = pt.tpoid 
					WHERE
						ptpid = %d"
					, $ptpid);
					
	return $db->pegaLinha($sql);				
	
}

function carregaListaProgramaTipologia(Array $filtro = null){
	global $db;
	$where = array();
	
	if ( !is_null($filtro) ){
		foreach( $filtro as $k => $val ):
			switch ($k){
				// O CASE é destinado para quando existir alguma implementação diferente da DEFAULT 
//				case 'ptpid':
//					array_push($where, "$k = '$val'");
//				break;
				default:
					array_push($where, "$k = '$val'");
				break;
			}	
		endforeach;
	}
	
	$sql = sprintf("SELECT
						ptpid, prfid, pt.cloid, pt.tpoid
					FROM
						obras.programatipologia pt
					WHERE
						1=1
						%s"
					, implode(" AND ", $where));
					
	return $db->carregar($sql);				
	
}

function listaProgramaTipologia(){
	global $db;
	
	$modulo = $param['modulo'] ? $param['modulo'] : $_REQUEST['modulo'];
	$acao   = $param['acao'] ? $param['modulo'] : $_REQUEST['acao'];
	
	$select = <<<ASDF
			'<img src="/imagens/alterar.gif" style="cursor:pointer;" border=0 title="Alterar Programa / Tipologia" onclick="redireciona(\'?modulo=$modulo&acao=$acao&tabelas=programatipologia&evento=editar&ptpid=' || prt.ptpid || '\');">&nbsp;		
			 <img src="/imagens/excluir.gif" style="cursor:pointer;" border=0 title="Alterar Programa / Tipologia" onclick="confirmExcluir(\'?modulo=$modulo&acao=$acao&tabelas=programatipologia&evento=excluir&ptpid=' || prt.ptpid || '\', \'Deseja Excluir o Programa Tipologia?\');">' AS op,
			orgdesc || ' - ' || prfdesc,
			clodsc,
			tpodsc || ' - ' || tpodetalhe AS tipologia		
ASDF;
	
	$sql = "SELECT
				$select
			FROM
				obras.programatipologia prt
			INNER JOIN 
				obras.classificacaoobra clo ON clo.cloid = prt.cloid
			INNER JOIN 
				obras.programafonte prf ON prf.prfid = prt.prfid
			INNER JOIN 
				obras.tipologiaobra tpo ON tpo.tpoid = prt.tpoid
			INNER JOIN
				obras.orgao oo ON oo.orgid = prf.orgid";

	$cabecalho = array("Opções", "Programa", "Classificação", "Tipologia");
	$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', '');				
//	dbg($db->carregar($sql),1);			
}
?>