<?php
header("Content-Type: text/html; charset=ISO-8859-1");
// Verificando se o obrid está salvo na sessão
if( !$_SESSION["obra"]['obrid'] ){
       header( "location:obras.php?modulo=inicio&acao=A" );
       exit;
}


// Condição que verificar a se o Questionário foi completamente preenchido. 
if ($_REQUEST['requisicao'] == 'analizarChecklist' && $_REQUEST['obrid']) {
	
	$questoesRespondidas = verificaQuestoesRespondidas($_REQUEST['obrid']);
	
	$sqlRespondidos = "SELECT DISTINCT
							p.perid
						FROM
							obras.checklistvistoria cv
						JOIN questionario.questionarioresposta qr USING (qrpid)
						JOIN questionario.questionario q ON q.queid = qr.queid
						JOIN questionario.grupopergunta gp ON gp.queid = q.queid
						JOIN questionario.pergunta p ON p.grpid = gp.grpid
						JOIN questionario.itempergunta ip ON ip.perid = p.perid
						JOIN questionario.pergunta p1 ON p1.itpid = ip.itpid
						JOIN questionario.resposta r ON r.perid = p1.perid AND r.qrpid = qr.qrpid 
						WHERE
							chkstatus = 'A' AND cv.obrid = ".$_REQUEST['obrid'];  
	
	$arrPeridRespondidos = $db->carregarColuna( $sqlRespondidos );
	
	if($arrPeridRespondidos){
		$sqlNaoRespondidos = " SELECT DISTINCT
								p.perid as idpergunta,
								p.pertitulo
						   FROM
								obras.checklistvistoria cv
						   JOIN questionario.questionarioresposta qr USING (qrpid)
						   JOIN questionario.questionario q ON q.queid = qr.queid
						   JOIN questionario.grupopergunta gp ON gp.queid = q.queid
						   JOIN questionario.pergunta p ON p.grpid = gp.grpid
						   JOIN questionario.itempergunta ip ON ip.perid = p.perid
						   JOIN questionario.pergunta p1 ON p1.itpid = ip.itpid
						   WHERE
								chkstatus = 'A' AND cv.obrid = ".$_REQUEST['obrid']." 
								AND p.perid NOT IN (". implode(",",$arrPeridRespondidos).") 
							ORDER BY
								p.pertitulo";
	
	$arrPeridNaoRespondidos = $db->carregar( $sqlNaoRespondidos );
	}

	if($questoesRespondidas == true){
		
		echo 'O Questionário do Checklist de Visita Técnica foi preenchido corretamente!';
		exit();
		
	}else if($questoesRespondidas == false){
		if(count($arrPeridNaoRespondidos)>1){
			$plural1 = 's';
			$plural2 = 'ões';
		}else{
			$plural1 = '';
			$plural2 = 'ão';
		}
		if($arrPeridNaoRespondidos){	 
		$str = 'Há pendência na'.$plural1.' segunte'.$plural1.' Quest'.$plural2.':' . "\n";
		foreach ($arrPeridNaoRespondidos as $arrNaoRespondido ){
			$str.= $arrNaoRespondido['pertitulo'] . "\n";		
		}
		echo $str;
		}else{
			echo 'Nenhuma Questão foi respondida!';
		}
		exit();
	}	

}

// formulário para inserir novo parecer
if ($_REQUEST['requisicao'] == 'inserirParecer' && $_REQUEST['chkid']) {
	
	echo parecerForm($_REQUEST['chkid']);
	exit();
}

// exibindo a descrição do parecer com o textarea desbloqueado
if ($_REQUEST['requisicao'] == 'descricaoParecer' && $_REQUEST['mpc_id']) {
	echo parecerDesc($_REQUEST['mpc_id']);
	exit();
}

// atualizando situacao do parecer
if ($_REQUEST['requisicao'] == 'atualizarSituacaoParecer' && $_REQUEST['mpc_id']) {

	$sql = "UPDATE obras.movparecercklist SET mpcsituacao=".(($_REQUEST['opcao'])?"TRUE":"FALSE")." WHERE mpc_id='".$_REQUEST['mpc_id']."'";
	$db->executar($sql);
	$db->commit();
	echo "Gravado com sucesso";
	exit();
}


// exibindo formulário para alterar o parecer
if ($_REQUEST['requisicao'] == 'alterarParecer' && $_REQUEST['mpc_id']) {
	echo alterarParecer($_REQUEST['mpc_id']);
	exit();	
}

// Recebendo AJAX que exclui o parecer
if ($_REQUEST['requisicao'] == 'excluirParecer' && $_REQUEST['mpc_id']) {
	
	$sql = "UPDATE obras.movparecercklist set mpcstatus = 'I' where mpc_id = {$_REQUEST['mpc_id']}";
	$db->executar($sql);
	
	//Inativando a Análise do Checklist de Visita Técnica
	$sqlVisTec = "UPDATE 
				  		obras.analisechecklist
   				  SET 
   				  		acvstatus = 'I'
				  WHERE 
				  		mpc_id = {$_REQUEST['mpc_id']}";
	$db->executar($sqlVisTec);
	
	//Inativando a Análise do Cadastro Básico
	$sqlCadBasic = "UPDATE 
							obras.analisecadastrobasico
				    SET 
				    		acbstatus = 'I'
				    WHERE 
				    		mpc_id = {$_REQUEST['mpc_id']}";
	$db->executar($sqlCadBasic);
		
	echo "<script>alert('Parecer excluído com sucesso.');window.opener.location=window.opener.location;window.close();</script>";
	$db->commit();
	exit();
}

// Recebendo AJAX que atualiza o parecer
if ($_REQUEST['requisicao'] == 'atualizarParecer' && $_POST['mpc_id'] ) {
	
	if($_POST['chksituacao'] == ''){
		$updateMpcsituacao = '';
	}else{
		$updateMpcsituacao = ", mpcsituacao='{$_POST['chksituacao']}'";
	}
	
	//Limitando o Campo do Parecer da Supervisão em até 5000 caracteres
	$_POST['chkobsmec']   =	substr(	$_POST['chkobsmec'], 0,	5000  );
	
	$sql = "UPDATE obras.movparecercklist set mpcdetalhamento = '".utf8_decode($_POST['chkobsmec'])."', nisid='{$_POST['nisid']}' {$updateMpcsituacao} where mpc_id = {$_POST['mpc_id']}";
	
	$db->carregar($sql);
	
	//Limitando os Campos de Observações em até 5000 caracteres
	$_POST['chkobsdoc']   =	substr(	$_POST['chkobsdoc'], 0,	5000  );
	$_POST['chkobsinst']  =	substr(	$_POST['chkobsinst'], 0, 5000 );
	$_POST['chkobspess']  =	substr(	$_POST['chkobspess'], 0, 5000 );
	$_POST['chkobsserv']  =	substr(	$_POST['chkobsserv'], 0, 5000 );
	$_POST['chkobspag']   =	substr(	$_POST['chkobspag'], 0,	5000  );
	$_POST['chkobsgeral'] =	substr(	$_POST['chkobsgeral'], 0, 5000);
	
	//Verifica se na tabela "obras.analisechecklist" possui algum "mpc_id" registrado com referência ao Checklist Cadastrado.
	$sqlAVT_mpc_id = " SELECT 
							mpc_id 
					   FROM 
						 	obras.analisechecklist
					   WHERE 
						   	mpc_id = {$_POST['mpc_id']}";
	$avt_mpc_id = $db->pegaUm($sqlAVT_mpc_id);
	  
	/*Caso haja algum "mpc_id" registrado com referência ao Checklist Cadastrado o mesmo deverá ser atualizado senão deverá ser
	 * inserido um novo, mas referente ao Checklista já Cadastrado.
	 */
	if($avt_mpc_id){
		//Atualizando a Análise do Checklist de Visita Técnica
		$sqlVisTec = "UPDATE 
							obras.analisechecklist
			    	  SET 
				    		usucpf = '{$_SESSION['usucpf']}', 
				    		acvdocumentacao = '{$_POST['documentacao']}', acvdocumentacaoobs = '".utf8_decode($_POST['chkobsdoc'])."', 
					        acvinstcanteiro = '{$_POST['instalacoes']}', acvinstcanteiroobs = '".utf8_decode($_POST['chkobsinst'])."', 
					        acvpessoal = '{$_POST['pessoal']}', acvpessoalobs = '".utf8_decode($_POST['chkobspess'])."', 
					        acvservicos = '{$_POST['servicos']}', acvservicosobs = '".utf8_decode($_POST['chkobsserv'])."', 
					        acvpagamento = '{$_POST['pagamentos']}', acvpagamentoobs = '".utf8_decode($_POST['chkobspag'])."', 
					        acvobsgeral = '".utf8_decode($_POST['chkobsgeral'])."',
					        acvdtinclusao = NOW()
			    	  WHERE 
			    			mpc_id = {$_POST['mpc_id']}";
		$db->executar($sqlVisTec);
	}else{
		//Inserindo a Análise do Checklist de Visita Técnica
		$sqlVisTec = "INSERT INTO 
							obras.analisechecklist
								(mpc_id, usucpf, 
								 acvdocumentacao, acvdocumentacaoobs, 
								 acvinstcanteiro, acvinstcanteiroobs, 
								 acvpessoal, acvpessoalobs, 
								 acvservicos, acvservicosobs, 
				            	 acvpagamento, acvpagamentoobs, 
				            	 acvobsgeral, 
				            	 acvdtinclusao, acvstatus)
			    		VALUES 
			    				({$_POST['mpc_id']}, '{$_SESSION['usucpf']}', 
			    				 '{$_POST['documentacao']}', '".utf8_decode($_POST['chkobsdoc'])."', 
			    				 '{$_POST['instalacoes']}','".utf8_decode($_POST['chkobsinst'])."',
			    		 		 '{$_POST['pessoal']}', '".utf8_decode($_POST['chkobspess'])."', 
			    		 		 '{$_POST['servicos']}', '".utf8_decode($_POST['chkobsserv'])."',
			    		 		 '{$_POST['pagamentos']}', '".utf8_decode($_POST['chkobspag'])."', 
			    		 		 '".utf8_decode($_POST['chkobsgeral'])."', 
			    		 		 NOW(),'A')";
		$db->executar($sqlVisTec);	
	}
	
	//Limitando os Campos de Observações em até 5000 caracteres
	$_POST['chkobsdadosobr']      =	substr(	$_POST['chkobsdadosobr'], 0, 5000);
	$_POST['chkobsproj'] 	      =	substr(	$_POST['chkobsproj'], 0, 5000);
	$_POST['chkobslic'] 	 	  =	substr(	$_POST['chkobslic'], 0,	5000);
	$_POST['chkobscontr'] 	 	  =	substr(	$_POST['chkobscontr'], 0, 5000);
	$_POST['chkobscron']	 	  =	substr(	$_POST['chkobscron'], 0, 5000);
	$_POST['chkobsvist'] 	  	  =	substr(	$_POST['chkobsvist'], 0, 5000);
	$_POST['chkobsrestr']	 	  =	substr(	$_POST['chkobsrestr'], 0, 5000);
	$_POST['chkobsdocument'] 	  =	substr(	$_POST['chkobsdocument'], 0, 5000);
	$_POST['chkobsgeralcadbasic'] =	substr(	$_POST['chkobsgeralcadbasic'], 0, 5000);
	
	//Verifica se na tabela "obras.analisecadastrobasico" possui algum "mpc_id" registrado com referência ao Checklist Cadastrado.
	$sqlACB_mpc_id = " SELECT 
							mpc_id
  					   FROM 
  					   		obras.analisecadastrobasico
  				   	   WHERE 
			    			mpc_id = {$_POST['mpc_id']}";
	$acb_mpc_id = $db->pegaUm($sqlACB_mpc_id);
	
	/*Caso haja algum "mpc_id" registrado com referência ao Checklist Cadastrado o mesmo deverá ser atualizado senão deverá ser
	 * inserido um novo, mas referente ao Checklista já Cadastrado.
	 */
	if($acb_mpc_id){
		//Atualizando a Análise do Cadastro Básico
		$sqlCadBasic = "UPDATE 
							obras.analisecadastrobasico
					    SET 
					    	usucpf='{$_SESSION['usucpf']}', 
					    	acbdadosobra='{$_POST['dadosobra']}', acbdadosobraobs='".utf8_decode($_POST['chkobsdadosobr'])."', acbdadosobrasitaba='{$_POST['dadosobrachekc']}', 
					        acbprojetos='{$_POST['projetos']}', acbprojetosobs='".utf8_decode($_POST['chkobsproj'])."', acbprojetossitaba='{$_POST['projetoschekc']}', 
					        acblicitacao='{$_POST['licitacao']}', acblicitacaoobs='".utf8_decode($_POST['chkobslic'])."', acblicitacaositaba='{$_POST['licitacaochekc']}', 
					        acbcontratacao='{$_POST['contratacao']}', acbcontratacaoobs='".utf8_decode($_POST['chkobscontr'])."', acbcontratacaositaba='{$_POST['contratacaochekc']}', 
					        acbcronograma='{$_POST['cronograma']}', acbcronogramaobs='".utf8_decode($_POST['chkobscron'])."', acbcronogramasitaba='{$_POST['cronogramachekc']}', 
					        acbvistoria='{$_POST['vistoria']}', acbvistoriaobs='".utf8_decode($_POST['chkobsvist'])."', acbvistoriasitaba='{$_POST['vistoriachekc']}', 
					        acbrestricao='{$_POST['restricoes']}', acbrestricaoobs='".utf8_decode($_POST['chkobsrestr'])."', acbrestricaositaba='{$_POST['restricoeschekc']}', 
					        acbdocumento='{$_POST['documentos']}', acbdocumentoobs='".utf8_decode($_POST['chkobsdocument'])."', acbdocumentositaba='{$_POST['documentoschekc']}', 
					        acbobsgeral  = '".utf8_decode($_POST['chkobsgeralcadbasic'])."',
					        acbdtinclusao =  NOW()
				        WHERE 
					    	mpc_id = {$_POST['mpc_id']}";
		$db->executar($sqlCadBasic);
	}else{
		//Inserindo a Análise do Cadastro Básico
		$sqlCadBasic = "INSERT INTO 
							obras.analisecadastrobasico
								(mpc_id, usucpf, 
								 acbdadosobra, acbdadosobraobs, acbdadosobrasitaba, 
					             acbprojetos, acbprojetosobs, acbprojetossitaba, 
					             acblicitacao, acblicitacaoobs, acblicitacaositaba, 
					             acbcontratacao, acbcontratacaoobs, acbcontratacaositaba, 
					             acbcronograma, acbcronogramaobs, acbcronogramasitaba, 
					             acbvistoria, acbvistoriaobs, acbvistoriasitaba, 
					             acbrestricao, acbrestricaoobs, acbrestricaositaba, 
					             acbdocumento, acbdocumentoobs, acbdocumentositaba,
					             acbobsgeral, 
					             acbdtinclusao, acbstatus)
			    		VALUES 
			    				({$_POST['mpc_id']}, '{$_SESSION['usucpf']}', 
			    				 '{$_POST['dadosobra']}', '".utf8_decode($_POST['chkobsdadosobr'])."', '{$_POST['dadosobrachekc']}', 
			    				 '{$_POST['projetos']}','".utf8_decode($_POST['chkobsproj'])."', '{$_POST['projetoschekc']}', 
			    				 '{$_POST['licitacao']}', '".utf8_decode($_POST['chkobslic'])."', '{$_POST['licitacaochekc']}', 
			    				 '{$_POST['contratacao']}', '".utf8_decode($_POST['chkobscontr'])."', '{$_POST['contratacaochekc']}',
					             '{$_POST['cronograma']}', '".utf8_decode($_POST['chkobscron'])."', '{$_POST['cronogramachekc']}', 
					             '{$_POST['vistoria']}','".utf8_decode($_POST['chkobsvist'])."', '{$_POST['vistoriachekc']}', 
					             '{$_POST['restricoes']}', '".utf8_decode($_POST['chkobsrestr'])."', '{$_POST['restricoeschekc']}', 
					             '{$_POST['documentos']}', '".utf8_decode($_POST['chkobsdocument'])."', '{$_POST['documentoschekc']}', 
					             '".utf8_decode($_POST['chkobsgeralcadbasic'])."',
					             NOW(), 'A')";
		$db->executar($sqlCadBasic);
	}
	
	echo "Parecer atualizado com sucesso.";
	$db->commit();
	exit();
}

// Recebendo AJAX que salva a observação do parecer
if ( isset($_POST['chkobsmec']) && isset($_POST['chkid']) && isset($_POST['mpcseqtramitacao']) ) {
	
	if($_POST['chksituacao']== ''){
		$insertMpcsituacao = ''; 
		$valuesMpcsituacao = '';
	}else{ 
		$insertMpcsituacao = ", mpcsituacao"; 
		$valuesMpcsituacao = ", '{$_POST ['chksituacao']}'";
	}
	
	//Limitando o Campo do Parecer da Supervisão em até 3000 caracteres
	$_POST['chkobsmec']   =	substr(	$_POST['chkobsmec'], 0,	3000  );
	
	$sql = "INSERT INTO
				obras.movparecercklist
			(chkid, usucpf, mpcseqtramitacao, mpcdetalhamento, mpcdtinclusao, mpcstatus,nisid {$insertMpcsituacao})
			VALUES
			({$_POST['chkid']}, '{$_SESSION['usucpf']}', {$_POST['mpcseqtramitacao']}, '".utf8_decode($_POST['chkobsmec'])."', NOW(), 'A', {$_POST['nisid']} {$valuesMpcsituacao})
			 RETURNING mpc_id";
	$mpc_id = $db->pegaUm($sql);
	
	//Limitando os Campos de Observações em até 5000 caracteres
	$_POST['chkobsdoc']   =	substr(	$_POST['chkobsdoc'], 0,	5000  );
	$_POST['chkobsinst']  =	substr(	$_POST['chkobsinst'], 0, 5000 );
	$_POST['chkobspess']  =	substr(	$_POST['chkobspess'], 0, 5000 );
	$_POST['chkobsserv']  =	substr(	$_POST['chkobsserv'], 0, 5000 );
	$_POST['chkobspag']   =	substr(	$_POST['chkobspag'], 0,	5000  );
	$_POST['chkobsgeral'] =	substr(	$_POST['chkobsgeral'], 0, 5000);
	
	
	//Inserindo a Análise do Checklist de Visita Técnica
	$sqlVisTec = "INSERT INTO 
						obras.analisechecklist
							(mpc_id, usucpf, 
							 acvdocumentacao, acvdocumentacaoobs, 
							 acvinstcanteiro, acvinstcanteiroobs, 
							 acvpessoal, acvpessoalobs, 
							 acvservicos, acvservicosobs, 
			            	 acvpagamento, acvpagamentoobs, 
			            	 acvobsgeral, 
			            	 acvdtinclusao, acvstatus)
		    		VALUES 
		    				({$mpc_id}, '{$_SESSION['usucpf']}', 
		    				 '{$_POST['documentacao']}', '".utf8_decode($_POST['chkobsdoc'])."', 
		    				 '{$_POST['instalacoes']}','".utf8_decode($_POST['chkobsinst'])."',
		    		 		 '{$_POST['pessoal']}', '".utf8_decode($_POST['chkobspess'])."', 
		    		 		 '{$_POST['servicos']}', '".utf8_decode($_POST['chkobsserv'])."',
		    		 		 '{$_POST['pagamentos']}', '".utf8_decode($_POST['chkobspag'])."', 
		    		 		 '".utf8_decode($_POST['chkobsgeral'])."', 
		    		 		 NOW(),'A')";
	$db->executar($sqlVisTec);

	//Limitando os Campos de Observações em até 5000 caracteres
	$_POST['chkobsdadosobr']      =	substr(	$_POST['chkobsdadosobr'], 0, 5000);
	$_POST['chkobsproj'] 	      =	substr(	$_POST['chkobsproj'], 0, 5000);
	$_POST['chkobslic'] 	      =	substr(	$_POST['chkobslic'], 0,	5000);
	$_POST['chkobscontr'] 	      =	substr(	$_POST['chkobscontr'], 0, 5000);
	$_POST['chkobscron']	      =	substr(	$_POST['chkobscron'], 0, 5000);
	$_POST['chkobsvist'] 	      =	substr(	$_POST['chkobsvist'], 0, 5000);
	$_POST['chkobsrestr']	      =	substr(	$_POST['chkobsrestr'], 0, 5000);
	$_POST['chkobsdocument'] 	  =	substr(	$_POST['chkobsdocument'], 0, 5000);
	$_POST['chkobsgeralcadbasic'] = substr(	$_POST['chkobsgeralcadbasic'], 0, 5000);
	
	//Inserindo a Análise do Cadastro Básico
	$sqlCadBasic = "INSERT INTO 
						obras.analisecadastrobasico
							(mpc_id, usucpf, 
							 acbdadosobra, acbdadosobraobs, acbdadosobrasitaba, 
				             acbprojetos, acbprojetosobs, acbprojetossitaba, 
				             acblicitacao, acblicitacaoobs, acblicitacaositaba, 
				             acbcontratacao, acbcontratacaoobs, acbcontratacaositaba, 
				             acbcronograma, acbcronogramaobs, acbcronogramasitaba, 
				             acbvistoria, acbvistoriaobs, acbvistoriasitaba, 
				             acbrestricao, acbrestricaoobs, acbrestricaositaba, 
				             acbdocumento, acbdocumentoobs, acbdocumentositaba,
				             acbobsgeral, 
				             acbdtinclusao, acbstatus)
		    		VALUES 
		    				({$mpc_id}, '{$_SESSION['usucpf']}', 
		    				 '{$_POST['dadosobra']}', '".utf8_decode($_POST['chkobsdadosobr'])."', '{$_POST['dadosobrachekc']}', 
		    				 '{$_POST['projetos']}','".utf8_decode($_POST['chkobsproj'])."', '{$_POST['projetoschekc']}', 
		    				 '{$_POST['licitacao']}', '".utf8_decode($_POST['chkobslic'])."', '{$_POST['licitacaochekc']}', 
		    				 '{$_POST['contratacao']}', '".utf8_decode($_POST['chkobscontr'])."', '{$_POST['contratacaochekc']}',
				             '{$_POST['cronograma']}', '".utf8_decode($_POST['chkobscron'])."', '{$_POST['cronogramachekc']}', 
				             '{$_POST['vistoria']}','".utf8_decode($_POST['chkobsvist'])."', '{$_POST['vistoriachekc']}', 
				             '{$_POST['restricoes']}', '".utf8_decode($_POST['chkobsrestr'])."', '{$_POST['restricoeschekc']}', 
				             '{$_POST['documentos']}', '".utf8_decode($_POST['chkobsdocument'])."', '{$_POST['documentoschekc']}', 
				             '".utf8_decode($_POST['chkobsgeralcadbasic'])."',
				             NOW(), 'A')";
	$db->executar($sqlCadBasic);

	
	echo "Parecer salvo com sucesso.";
	$db->commit();
	exit();
}

// Recebendo AJAX no momento em que a Observação está sendo salva
if( isset($_POST['chkobscompempresa']) && isset($_POST['chkid']) ){
	// limitando o chkobscompempresa a 3000 caracteres
	$_POST['chkobscompempresa'] = substr($_POST['chkobscompempresa'], 0, 3000);
	
	$sql = "UPDATE obras.checklistvistoria
			SET 
				chkobscompempresa='".utf8_decode($_POST['chkobscompempresa'])."'
			WHERE 
				chkid={$_POST['chkid']};";
	
	$db->executar($sql);
	echo "Observação salva com sucesso.";
	$db->commit();
	exit();
}

// Recebendo AJAX no momento em que o Parecer da Supervisão(MEC) está sendo salvo
if( isset($_POST['chkobsmec']) && isset($_POST['chksituacao']) && isset($_POST['chkid']) ){
										// esta parte da condição serve para os navegadores antigos
	if( ($_POST['chksituacao'] == 0) || $_POST['chksituacao'] == 'undefined'){
		$_POST['chksituacao'] = 'false';
	}else{
		$_POST['chksituacao'] = 'true';
	}
	
	// limitando o chkobsmec a 3000 caracteres
	$_POST['chkobsmec'] = substr($_POST['chkobsmec'], 0, 5000);
	
	$sql = "UPDATE obras.checklistvistoria
			SET 
				chkobsmec='".utf8_decode($_POST['chkobsmec'])."', 
				chksituacao={$_POST['chksituacao']}
			WHERE 
				chkid={$_POST['chkid']};";
	
	$db->executar($sql);
	echo "Parecer salvo com sucesso.";
	$db->commit();
	exit();
}

// cabecalho padrão do sistema
include APPRAIZ . "includes/cabecalho.inc";

echo "<br>";
// Cria o título da tela
$titulo_modulo = "Checklist de Visita Técnica";
$db->cria_aba( $abacod_tela, $url, $parametros );
//monta_titulo( $titulo_modulo, "&nbsp");

?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

$(document).ready(function() {

	$("#inserir").click(function () {
		window.location = 'obras.php?modulo=principal/supervisao/check_list_visita&acao=A&requisicao=inserir';
	})
	
	//botão Responsável pela Vistoria
	$("#respvistoria").click(function () {
		var entid = $("#entidrespvistoria").val(); 
		if (entid){ 
			return windowOpen( caminho_atual + '?modulo=principal/inserir_vistoriador&acao=A&busca=entnumcpfcnpj&entid=' + entid,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		}else{
			return windowOpen( caminho_atual + '?modulo=principal/inserir_vistoriador&acao=A&funid=76','blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		}
	})
	
	//botão Responsável Técnico
	$("#resptecnico").click(function () {
		var entid = $("#entidresptecnico").val(); 
		if (entid){ 
			return windowOpen( caminho_atual + '?modulo=principal/inserir_vistoriador&acao=A&busca=entnumcpfcnpj&entid=' + entid,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		}else{
			return windowOpen( caminho_atual + '?modulo=principal/inserir_vistoriador&acao=A&funid=77','blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		}
	})

	// botão salvar Responsáveis
	$("#cadRespTecRespVist").click(function () {

		if( $("#entidrespvistoria").val() == "" ){
			alert('Preencha o Profissional Responsável pela vistoria.');
			return false;
		}else if($("#entidresptecnico").val() == ""){
			alert('Preencha o Responsável Técnico pela obra.');
			return false;
		}
		return true;
	})

	// botão salvar Supervisão
	$("#salvarSupervisao").click(function () {
		
		var chksituacao = $("#chksituacao:checked").val();
		var chkobsmec = $("#chkobsmec").val();
		var chkid = <?php echo @$_GET['chkid']; ?>
		
		$.post(caminho_atual + '?modulo=principal/supervisao/check_list_visita&acao=A', { chkobsmec: chkobsmec, chksituacao: chksituacao, chkid : chkid },
			function(data){
				alert(data);
			});
	})

	// botão salvar Observação
	$("#salvarObservacao").click(function () {

		var chkobscompempresa = $("#chkobscompempresa").val();
		var chkid = <?php echo @$_GET['chkid']; ?>
		
		$.post(caminho_atual + '?modulo=principal/supervisao/check_list_visita&acao=A', { chkobscompempresa : chkobscompempresa, chkid : chkid },
			function(data){
				alert(data);
			});
	})

	//botão inserir parecer
	$("#inserirParecer").click(function () {

		// abrindo a popup para que o usuário digite a descrição do parecer
		windowOpen( caminho_atual + '?modulo=principal/supervisao/check_list_visita&acao=A&requisicao=inserirParecer&chkid=<?php echo $_REQUEST['chkid']; ?>','blank','height=800,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		
	})

	//botão descricao do parecer
	$("[id^='descricaoParecer_']").click(function () {

		var mpc_id = this.id.replace("descricaoParecer_","");
		
		// abrindo a popup para que o usuário digite a descrição do parecer
		windowOpen( caminho_atual + '?modulo=principal/supervisao/check_list_visita&acao=A&requisicao=descricaoParecer&mpc_id='+mpc_id,'blank','height=800,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		
	})

	//botão alterar parecer
	$("[id^='alterar_']").click(function () {

		var mpc_id = this.id.replace("alterar_","");
		// abrindo a popup para que o usuário digite a descrição do parecer
		windowOpen( caminho_atual + '?modulo=principal/supervisao/check_list_visita&acao=A&requisicao=alterarParecer&mpc_id='+mpc_id,'blank','height=800,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
	})

	//botão excluir parecer
	$("[id^='excluir_']").click(function () {

		if( confirm("Deseja realmente excluir este Parecer?") ){
			var mpc_id = this.id.replace("excluir_","");
			// abrindo a popup para que o usuário digite a descrição do parecer
			windowOpen( caminho_atual + '?modulo=principal/supervisao/check_list_visita&acao=A&requisicao=excluirParecer&mpc_id='+mpc_id,'blank','height=200,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		}
	})

});

	function desabilita(){
		<?php
			/*if( possuiPerfil(PERFIL_EMPRESA) && !$db->testa_superuser() ){
				echo "$('tbody :input').attr('disabled', true);";
			}else*/ if(possuiPerfil( array(PERFIL_SUPERUSUARIO, PERFIL_ADMINISTRADOR, PERFIL_SUPERVISORMEC) )){
				//questionario
				echo "$('#telacentral :input').attr('disabled', true);";
				//observacao
				echo "$('#observacao :input').attr('disabled', true);";
				//parecer

				// echo "$('#parecer :input').attr('disabled', false);";
			}else{
				//questionario
				echo "$('#telacentral :input').attr('disabled', false);";
				//observacao
				echo "$('#observacao :input').attr('disabled', false);";
				//parecer
				echo "$('#parecer :input').attr('disabled', true);";
			}?>
	}

<?php /*Habilitando para superusuario*/
	if( !possuiPerfil(PERFIL_SUPERUSUARIO) ){ ?>
	setInterval('desabilita()', 100);
<?php } ?>
	
</script>

<?php

$obras = new Obras();

switch ($_GET['requisicao']){
	case 'inserir':
		
		unset($_SESSION['obra']['disable']);
		
		if(verificaChecklist()){
			// REGRA: Não será possível cadastrar um novo Checklist com o mesmo obrid, orsid e gpdid
			header( "location:obras.php?modulo=principal/supervisao/check_list_visita&acao=A" );
       		exit();
		}
		
		$dados = pegaOrdem();
		
		// REGRA: Caso não exista nenhuma OS, ou Grupo contendo o id da obra, o questionário não pode ser criado
		if( $dados[0]['orsid'] && $dados[0]['gpdid'] ){

			monta_titulo( $titulo_modulo, "&nbsp;");
			// Cabeçalho
			echo $obras->CabecalhoObras();
			echo formChecklist($dados[0]['orsid'], $dados[0]['gpdid']);

		}else{
			header( "location:obras.php?modulo=principal/supervisao/check_list_visita&acao=A" );
       		exit();
		}
		
	break;
	
	case 'questionario':
		
		include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
		include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
		
		
		if($_REQUEST['chkid']){
			monta_titulo( $titulo_modulo, "&nbsp;");
			$cabecalho = cabecalho_tabela($_REQUEST['chkid']);
			echo $obras->CabecalhoObras();
			$qrpid = verificaQrpid( $_REQUEST['chkid'] );
			
			// pegando os dados da supervisão
			$sql = "SELECT 
						chkobsmec, 
						chksituacao,
						chkobscompempresa
					FROM 
						obras.checklistvistoria
					WHERE 
						chkid={$_REQUEST['chkid']};";
			$dados = $db->carregar($sql);
			
			// verificando se o usuário possui permissão para alterar o Parecer da Supervisão
			if(possuiPerfil( array(PERFIL_SUPERUSUARIO, PERFIL_ADMINISTRADOR, PERFIL_SUPERVISORMEC) )){
				$radio = '';
				$habil = 'S';
				$acao = "'<center>
							   <img src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar\" id=\"alterar_'|| mpc_id ||'\" title=\"alterar\" alt=\"alterar\" style=\"cursor:pointer;\">
							   &nbsp;
							   <img src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" id=\"excluir_'|| mpc_id ||'\" title=\"excluir\" alt=\"excluir\" style=\"cursor:pointer;\">
						   </center>' as acao,";
			}else{
				$radio = ' disabled="disabled"';
				$habil = 'N';
				$acao = "'<center>
							   <img src=\"/imagens/alterar_01.gif\" border=\"0\" title=\"Alterar\" style=\"cursor:pointer;\" title=\"alterar\" alt=\"alterar\">
							   &nbsp;
							   <img src=\"/imagens/excluir_01.gif\" border=\"0\" title=\"Excluir\" style=\"cursor:pointer;\" title=\"excluir\" alt=\"excluir\">
						   </center>' as acao,";
			}
			?>
			<script>
			function marcarSituacao(mpc_id, obj) {
				$.ajax({
			   		type: "POST",
			   		url: "obras.php?modulo=principal/supervisao/check_list_visita&acao=A",
			   		data: "requisicao=atualizarSituacaoParecer&mpc_id="+mpc_id+"&opcao="+obj.value,
			   		async: false,
			   		success: function(data){alert(data);}
		 		});
				
			}
			function verificaQestionario(analizar,obrid) {
				$.ajax({
			   		type: "POST",
			   		url: "obras.php?modulo=principal/supervisao/check_list_visita&acao=A",
			   		data: "requisicao=analizarChecklist&obj="+analizar+"&obrid="+obrid,
			   		async: false,
			   		success: function(data){alert(data);window.location.href=window.location.href;}
		 		});
			}
			</script>
				<form name="frm" id="frm">
					<table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem">
						<?php echo $cabecalho ?>
						<tr>
							<td colspan="6">
							<fieldset style="width: 94%; background: #fff;"  >
								<legend>Questionário</legend>
								<?php
									$tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25 ) );
								?>
							</fieldset>
							</td>
							<td rowspan="24" width="100" valign="top" align="center">
							<?php
								$obrid = ( ( $_SESSION["obra"]["obrid"] ) ? $_SESSION["obra"]["obrid"] : $_SESSION["obras"]["obrid"] );
								if ( $obrid ){
								$gpdid = buscaGrupoPelaObra( $obrid );
								$docid = obrPegaDocidObra( $obrid );
								$estado = wf_pegarEstadoAtual( $docid );
									if ( $estado ){
										wf_desenhaBarraNavegacao( $docid , array( 'obrid' => $obrid, 'gpdid' => $gpdid) );
									}
								}
							?>
							</td>
						</tr>
						<tr>
						<td colspan="5"></td>
						<? 
							if($obrid){
							$questoesRespondidas = verificaQuestoesRespondidas($obrid);
								if($questoesRespondidas == true){
									$desabilitaBotao ='disabled="disabled"';
								}
							} 
						?>
						<td><input <?=$desabilitaBotao;?>  type="button" value="Validar Questionário" id="avaliarQuestionario" onclick="verificaQestionario('avaliarQuestionario',<?=$obrid;?>);"/></td>
						</tr>
						<tr>
							<td colspan="6">
								<div id="observacao">
								<fieldset style="width: 94%; background: #fff;">
									<legend>Observações Complementares (EMPRESA)</legend>
										<table border="1" width="100%">
											<tr>
												<td bgcolor="#e7e7e7" width="15%"><b>Observação</b></td>
												<td><?php  echo campo_textarea( 'chkobscompempresa', 'N', 'S', '', '150', '10', '3000', '' , 0, '', false, NULL, $dados[0]['chkobscompempresa']); ?>
												</td>
											</tr>
											<tr>
												<td>
													<input type="button" value="Salvar Observação" id="salvarObservacao" />
												</td>
												<td>&nbsp;</td>
											</tr>
										</table>
								</fieldset>
								</div>
							</td>
						</tr>
						<tr>
							<td colspan="6">
								<div id="parecer">
									<fieldset style="width: 94%; background: #fff;">
										<legend>Parecer</legend>
										<?php
											$sql = "SELECT
													/*'<center>
														<img src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar\" id=\"alterar_'|| mpc_id ||'\" title=\"alterar\" alt=\"alterar\">
														&nbsp;
														<img src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" id=\"excluir_'|| mpc_id ||'\" title=\"excluir\" alt=\"excluir\">
													 </center>' as acao,*/
													 {$acao}
													 to_char(mpcdtinclusao, 'DD/MM/YYYY') as data_inclusao,
													 
													CASE WHEN
														mpcsituacao IS NULL THEN '<center> - </center>'
														/*'<label><input disabled=\"disabled\" type=\"radio\" name=\"chksituacao\" value=\"1\" onclick=marcarSituacao(\''||mpc_id||'\',this);>Aprovado</label>
														<label><input disabled=\"disabled\" type=\"radio\" name=\"chksituacao\" value=\"0\" onclick=marcarSituacao(\''||mpc_id||'\',this);>Não Aprovado</label>'*/
													ELSE
														CASE WHEN
															mpcsituacao = TRUE THEN '<center><b> Aprovado </b><center>'
															/*'<input disabled=\"disabled\" type=\"radio\" name=\"chksituacao\" value=\"1\" checked=\"checked\" onclick=marcarSituacao(\''||mpc_id||'\',this);>Aprovado
															<input disabled=\"disabled\" type=\"radio\" name=\"chksituacao\" value=\"0\" onclick=marcarSituacao(\''||mpc_id||'\',this);>Não Aprovado'*/
														ELSE  '<center><b> Não Aprovado </b><center>'
															/*'<input disabled=\"disabled\" type=\"radio\" name=\"chksituacao\" value=\"1\" onclick=marcarSituacao(\''||mpc_id||'\',this);>Aprovado
															<input disabled=\"disabled\" type=\"radio\" name=\"chksituacao\" value=\"0\" checked=\"checked\" onclick=marcarSituacao(\''||mpc_id||'\',this);>Não Aprovado'*/
														END
													END as situacao,
													 
													 mpcseqtramitacao as sequencia,
													 '<center><img src=\"/imagens/editar_nome.gif\" style=\"cursor:pointer;\" id=\"descricaoParecer_'||mpc_id||'\" title=\"Descrição do Parecer\"></center>' as desc_parecer,
													 --'<a href=\"#\" id=\"descricaoParecer_'||mpc_id||'\">descricao</a>' as desc_parecer
													 --mpc_id as desc_parecer
													 us.usunome AS nome_cadastrante
												FROM
													obras.movparecercklist mc
												LEFT JOIN 
													seguranca.usuario us ON us.usucpf = mc.usucpf
												WHERE
													mpcstatus = 'A'
													AND chkid = {$_REQUEST['chkid']}
												ORDER BY chkid";
											
											// solicitação solicitada pelo mario, foi informado sobre a carga de processamento gerada 
											$arrDados = $db->carregar($sql);
											$arrLista = array();
											if($arrDados[0]) {
												foreach($arrDados as $key => $arr) {
													$arrLista[$key]['acao'] = $arr['acao'];
													$arrLista[$key]['data_inclusao'] = $arr['data_inclusao'];
													$arrLista[$key]['situacao'] = $arr['situacao'];
													if($arr['sequencia']) {
														$ss = "SELECT h.hstid, e1.esddsc as desc1, e2.esddsc as desc2, to_char(h.htddata, 'dd/mm/YYYY HH24:MI') as htddata 
															   FROM workflow.historicodocumento h 
															   LEFT JOIN workflow.acaoestadodoc a ON a.aedid = h.aedid 
															   LEFT JOIN workflow.estadodocumento e1 ON a.esdidorigem=e1.esdid 
															   LEFT JOIN workflow.estadodocumento e2 ON a.esdiddestino=e2.esdid
															   WHERE docid IN ( SELECT docid FROM workflow.historicodocumento WHERE hstid='".$arr['sequencia']."') 
															   ORDER BY h.htddata";
														
														$historicodocumentos = $db->carregar($ss);
														
														if($historicodocumentos[0]) {
															foreach($historicodocumentos as $hd) {
																$num++;
																if($hd['hstid'] == $arr['sequencia']) {
																	$arrLista[$key]['sequencia'] = $num." . ".$hd['desc1']." >> ".$hd['desc2'];	
																	$arrLista[$key]['data'] = $hd['htddata'];
																}
															}
														}else {
															$arrLista[$key]['sequencia'] = "<center> - </center>";	
															$arrLista[$key]['data'] = "<center> - </center>";
														} 
													}else {
														$arrLista[$key]['sequencia'] = "<center> - </center>";	
														$arrLista[$key]['data'] = "<center> - </center>";
													}
													
													$arrLista[$key]['nome_cadastrante'] = $arr['nome_cadastrante'];
													$arrLista[$key]['desc_parecer'] = $arr['desc_parecer'];
												}
											}
											// fim solicitacao
											$cabecalho = array( "Ação", "Data de Inclusão", "Situação", "Seq. Tramitação", "Data Tramitação", "Realizado por", "Parecer" );
											$db->monta_lista($arrLista, $cabecalho, 50, 20, '', 'center', '');
										?>
										<input type="button" value="Voltar" onclick="window.location='obras.php?modulo=principal/supervisao/check_list_visita&acao=A'" />
										&nbsp;
										<?
										$dados = pegaOrdem();
										
										$sql = "SELECT
													DISTINCT MAX(hstid) as sequencia 
												FROM
													obras.checklistvistoria ch 
												INNER JOIN 
													obras.obrainfraestrutura ob ON ob.obrid = ch.obrid 
												INNER JOIN
													workflow.documento wd ON wd.docid = ob.docid
												INNER JOIN
													workflow.historicodocumento wh ON wh.docid = ob.docid
												INNER JOIN
													workflow.estadodocumento we ON we.esdid = wd.esdid
												WHERE
													obsstatus = 'A' 
													AND ch.chkid  = ".$_REQUEST['chkid'];
										
										$sequencia = $db->pegaUm($sql);
										
										if($sequencia) {
											$sql = "SELECT mpc_id FROM obras.movparecercklist WHERE mpcstatus='A' AND chkid='".$_REQUEST['chkid']."' AND mpcseqtramitacao='".$sequencia."' ";
											$mpc_id = $db->pegaUm($sql);
											
											if($mpc_id) {
												$disabledparecer = true;
											} else {
												$disabledparecer = false;
											}
											
										} else {
											$disabledparecer = true;	
										}

										?>
										<input type="button" value="Inserir Parecer" id="inserirParecer" <? echo (($disabledparecer)?"disabled":""); ?> />
										<!-- <table border="1" width="100%">
											<tr>
												<td bgcolor="#e7e7e7" width="15%"><b>Parecer da Supervisão(MEC)</b></td>
												<td><?php  //echo campo_textarea( 'chkobsmec', 'N', '', '', '150', '10', '3000', '' , 0, '', false, NULL, $dados[0]['chkobsmec']); ?>
												</td>
											</tr>
											<tr>
												<td bgcolor="#e7e7e7"><b>Supervisão Aprovada:</b></td>
												<td>
													<input<?php //echo $radio; ?> type="radio" name="chksituacao" id="chksituacao" value="1"<?php //echo ($dados[0]['chksituacao'] == 't' ? ' checked="checked"' : '' ) ?> /> Sim
													<input<?php //echo $radio; ?> type="radio" name="chksituacao" id="chksituacao" value="0"<?php //echo ($dados[0]['chksituacao'] == 'f' ? ' checked="checked"' : '' ) ?> /> Não
												</td>
											</tr>
											<tr>
												<td>
													<input type="button" value="Voltar" onclick="window.location='obras.php?modulo=principal/supervisao/check_list_visita&acao=A'" />
													&nbsp;
													<input type="button" value="Salvar Parecer" id="salvarSupervisao" />
												</td>
												<td>&nbsp;</td>
											</tr>
										</table> -->
									</fieldset>
								</div>
							</td>
						</tr>
					</table>
				</form>
				<?php
			
		}elseif( (isset($_REQUEST['entidrespvistoria'])) && (isset($_REQUEST['entidresptecnico'])) ){
			
			$_SESSION['obra']['entidrespvistoria'] = $_REQUEST['entidrespvistoria'];
			$_SESSION['obra']['entidresptecnico'] = $_REQUEST['entidresptecnico'];
			
			$obrid = $_SESSION["obra"]['obrid'];
			$orsid = pegaOrdem();
			$gpdid = buscaGrupoPelaObra( $obrid );
			
			if( $orsid ){
				$qrpid = explode( ',', pegaQrpidObras( $obrid, 42, $orsid[0]['orsid'], $obras->ViewObra( $obrid ) , $gpdid ) ); // 42 é o id do questionário!! queid
				$cabecalho = cabecalho_tabela($qrpid[1]); // chkid
				
				?>
				<script> window.location="obras.php?modulo=principal/supervisao/check_list_visita&acao=A&requisicao=questionario&chkid="+<?php echo $qrpid[1]; ?> </script>
				<!--<table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem">
					<?php //echo $cabecalho; // acho q esse html pode ser removido, pq estou fazendo um window.location com o js ?>
					<tr>
						<td colspan="6">
							<fieldset style="width: 94%; background: #fff;"  >
								<legend>Questionário</legend>
								<?php
									//$tela = new Tela( array("qrpid" => $qrpid[0], 'tamDivArvore' => 25 ) );
								?>
							</fieldset>
						</td>
					</tr>
					<tr>
						<td><input type="button" value="Voltar" onclick="window.location='obras.php?modulo=principal/supervisao/check_list_visita&acao=A'" /></td>
					</tr>
				</table>
				--><?php
				
			}// fim do segundo if
			
		}// fim do primeiro if
		
		
	break;
	
	case 'excluir':
		
		unset($_SESSION['obra']['disable']);
		monta_titulo( $titulo_modulo, "&nbsp");
		// Cabeçalho
		$obras = new Obras();
		echo $obras->CabecalhoObras();
		
		if( excluirChecklistObras($_REQUEST['chkid']) ){
			echo '<script>
					alert("Exclusão realizada com sucesso!");
					window.location = "obras.php?modulo=principal/supervisao/check_list_visita&acao=A";
				  </script>';
			
		}else{
			echo '<script>
					alert("Você não tem permissão para executar esta açao.");
					window.location = "obras.php?modulo=principal/supervisao/check_list_visita&acao=A";
				  </script>';
		}
		
	break;
	
	default:
		
		unset($_SESSION['obra']['disable']);
		monta_titulo( $titulo_modulo, "&nbsp");
		// Cabeçalho
		$obras = new Obras();
		echo $obras->CabecalhoObras();
		
		// se for superusuário então exibo todos os checklists, se for empresa exibo todos os checklists cadastrados pelo cpf do usuário
		if( possuiPerfil(PERFIL_EMPRESA) ){
			//$condicao = " AND cv.usucpf = '{$_SESSION['usucpf']}'";
			$condicao = '';
			$acao = "<center>
						<img src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar\" id=\"alterar\" title=\"alterar\" alt=\"alterar\" onclick=\"javascript:alterarChecklist(\''|| cv.chkid ||'\');\">
						&nbsp;
						<img src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" id=\"excluir\" title=\"excluir\" alt=\"excluir\" onclick=\"javascript:excluirChecklist(\''|| cv.chkid ||'\');\">
					 </center>";
			
			// Escrevendo as funções js
			echo "<script type='text/javascript'>
					function alterarChecklist(chkid) {
						window.location = caminho_atual + '?modulo=principal/supervisao/check_list_visita&acao=A&requisicao=questionario&chkid=' + chkid;
					}
					
					function excluirChecklist(chkid) {

						if(confirm('Deseja realmente excluir este Checklist?')){
							window.location = caminho_atual + '?modulo=principal/supervisao/check_list_visita&acao=A&requisicao=excluir&chkid=' + chkid;
						}else{
							return false;
						}
						return true;
					}
				  </script>";
			
			if( possuiPerfil(PERFIL_SUPERUSUARIO) ){
				// superusuario pode visualizar todos os checklists
				$condicao = '';
			}// fim do segundo if
			
			// botão enviar
			$botao = '<input type="button" value="Inserir Checklist" id="inserir" />';
			
		}else{
			//para os perfis PERFIL_SUPERVISORUNIDADE, PERFIL_GESTORUNIDADE, PERFIL_ADMINISTRADOR, PERFIL_SUPERVISORMEC e PERFIL_SAA somente leitura e visualizar todos os checklists
			$acao = "<center>
						<img src=\"/imagens/alterar.gif\" border=\"0\" title=\"Visualizar\" id=\"visualizar\" title=\"visualizar\" alt=\"visualizar\" onclick=\"javascript:visualizarChecklist(\''|| cv.chkid ||'\');\">
					 </center>";
			
			// Escrevendo as funções js
			echo "<script type='text/javascript'>
					function visualizarChecklist(chkid) {
						window.location = caminho_atual + '?modulo=principal/supervisao/check_list_visita&acao=A&requisicao=questionario&chkid=' + chkid + '&disable=1';
					}
				  </script>";
			
			// todos os checklists
			$condicao = '';
			// botão desabilitado
			$botao = '<input type="button" value="Inserir Checklist" id="inserir" disabled="disabled" />';
		}// fim do primeiro if
		
		
		// exibindo os checklists
		$sql = "SELECT DISTINCT
					'{$acao}' as acao,
					cv.chkid as sequencia,
					to_char(cv.chkdtinclusao, 'DD/MM/YYYY') as datachk,
					u.usunome as nome,
					cv.orsid as ordem,
					to_char(os.orsdtemissao, 'DD/MM/YYYY') as dataos,
					gd.gpdid as grupo
					
				FROM
					obras.checklistvistoria cv
				INNER JOIN obras.ordemservico os ON os.orsid = cv.orsid
				INNER JOIN obras.grupodistribuicao gd ON gd.gpdid = os.gpdid
				INNER JOIN obras.itemgrupo ig on gd.gpdid  = ig.gpdid
				INNER JOIN seguranca.usuario u ON u.usucpf = cv.usucpf
				INNER JOIN obras.repositorio r ON ig.repid = r.repid 
							       AND cv.obrid = {$_SESSION["obra"]['obrid']}
							       AND chkstatus = 'A'
							       AND gd.gpdstatus = 'A'
							       {$condicao}";
		
		$db->monta_lista($sql, array( "Ação", "Seqüência", "Data de Criação", "Inserido por", "Nº da O.S", "Data da O.S", "Nº do Grupo"  ), 50, 20, '', 'center', '');
		
		$dados = pegaOrdem();
		// se existir algum resultado, então eu f
		if($dados){
			// verificando se poderá ser inserida um novo checklist
			if(verificaChecklist()){
				$botao = '<input type="button" value="Inserir Checklist" id="inserir" disabled="disabled" />';
			}
			
		}else{
			$botao = '<input type="button" value="Inserir Checklist" id="inserir" disabled="disabled" />';
		}
		
		echo $msg . '
			  <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
				<tr>
					<td class="SubTituloEsquerda">' . $botao . '</td>
				</tr>
			  </table>';
	break;
}

//$obrid = $_SESSION["obra"]['obrid'];
//$gpdid = buscaGrupoPelaObra( $obrid );
//$verfifcaQuestionario = verificaChecklistObrasIndividual( $gpdid , $obrid );
//if ($verfifcaQuestionario == 1){
//	ver('Questionário Respondido!');
//}else{
//	ver('Questionário não foi Respondido ou não foi criado!');
//}


function alterarParecer($mpc_id){
	global $db;
	$sql = "SELECT
				mpcdetalhamento,
				nisid,
				mpcsituacao
			FROM
				obras.movparecercklist
			WHERE
				mpc_id = {$mpc_id}
				AND mpcstatus = 'A'";
	
	$desc = $db->pegaLinha($sql);
	
	$sqlVisTec = "SELECT 
						acvdocumentacao, acvdocumentacaoobs, 
						acvinstcanteiro, acvinstcanteiroobs, 
						acvpessoal, acvpessoalobs, 
						acvservicos, acvservicosobs, 
	       				acvpagamento, acvpagamentoobs, 
	       				acvobsgeral 
	  			FROM 
	  				    obras.analisechecklist 
			    WHERE
					mpc_id = {$mpc_id}
					AND acvstatus = 'A'";

	$analiseVisTec = $db->pegaLinha($sqlVisTec);

	$sqlCadBasic = "SELECT 
						acbdadosobra, acbdadosobraobs, acbdadosobrasitaba, 
				        acbprojetos, acbprojetosobs, acbprojetossitaba, 
				        acblicitacao, acblicitacaoobs, acblicitacaositaba, 
				        acbcontratacao, acbcontratacaoobs, acbcontratacaositaba, 
				        acbcronograma, acbcronogramaobs, acbcronogramasitaba, 
				        acbvistoria, acbvistoriaobs, acbvistoriasitaba, 
				        acbrestricao, acbrestricaoobs, acbrestricaositaba, 
				        acbdocumento, acbdocumentoobs, acbdocumentositaba,
				        acbobsgeral 
				  FROM 
				  		obras.analisecadastrobasico
				  WHERE  
				 		mpc_id = {$mpc_id}
				 		AND acbstatus = 'A' ";
	 $analiseCadBasic = $db->pegaLinha($sqlCadBasic);

	
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

$(document).ready(function() {
	//botão salvar descrição do parecer
	$("#salvarParecer").click(function () {
	
		var chkobsmec = $('#chkobsmec').val();
		var nisid 	  = $('#nisid').val();
		var mpc_id = <?php echo $mpc_id; ?>;

		var documentacao_1 = $('#documentacao_1:checked').val();
		var documentacao_2 = $('#documentacao_2:checked').val();
		var chkobsdoc = $('#chkobsdoc').val();

		var instalacoes_1 = $('#instalacoes_1:checked').val();
		var instalacoes_2 = $('#instalacoes_2:checked').val();
		var chkobsinst = $('#chkobsinst').val();  
		
		var pessoal_1 = $('#pessoal_1:checked').val();
		var pessoal_2 = $('#pessoal_2:checked').val();
		var chkobspess = $('#chkobspess').val(); 
			
		var servicos_1 = $('#servicos_1:checked').val();
		var servicos_2 = $('#servicos_2:checked').val();
		var chkobsserv = $('#chkobsserv').val(); 
			
		var pagamentos_1 = $('#pagamentos_1:checked').val();
		var pagamentos_2 = $('#pagamentos_2:checked').val();
		var chkobspag = $('#chkobspag').val(); 

		var chkobsgeral = $('#chkobsgeral').val(); 

		var dadosobra_1 = $('#dadosobra_1:checked').val();
		var dadosobra_2 = $('#dadosobra_2:checked').val();
		var dadosobra_3 = $('#dadosobra_3:checked').val();
		var chkobsdadosobr = $('#chkobsdadosobr').val(); 

		var projetos_1 = $('#projetos_1:checked').val();
		var projetos_2 = $('#projetos_2:checked').val();
		var projetos_3 = $('#projetos_3:checked').val();
		var chkobsproj = $('#chkobsproj').val(); 

		var licitacao_1 = $('#licitacao_1:checked').val();
		var licitacao_2 = $('#licitacao_2:checked').val();
		var licitacao_3 = $('#licitacao_3:checked').val();
		var chkobslic = $('#chkobslic').val(); 
			
		var contratacao_1 = $('#contratacao_1:checked').val();
		var contratacao_2 = $('#contratacao_2:checked').val();
		var contratacao_3 = $('#contratacao_3:checked').val();
		var chkobscontr = $('#chkobscontr').val(); 
			
		var cronograma_1 = $('#cronograma_1:checked').val();
		var cronograma_2 = $('#cronograma_2:checked').val();
		var cronograma_3 = $('#cronograma_3:checked').val();
		var chkobscron = $('#chkobscron').val(); 
			
		var vistoria_1 = $('#vistoria_1:checked').val();
		var vistoria_2 = $('#vistoria_2:checked').val();
		var vistoria_3 = $('#vistoria_3:checked').val();
		var chkobsvist = $('#chkobsvist').val(); 
			
		var restricoes_1 = $('#restricoes_1:checked').val();
		var restricoes_2 = $('#restricoes_2:checked').val();
		var restricoes_3 = $('#restricoes_3:checked').val();
		var chkobsrestr = $('#chkobsrestr').val(); 
			
		var documentos_1 = $('#documentos_1:checked').val();
		var documentos_2 = $('#documentos_2:checked').val();
		var documentos_3 = $('#documentos_3:checked').val();
		var chkobsdocument = $('#chkobsdocument').val(); 

		var chksituacao_1 = $('#chksituacao_1:checked').val();
		var chksituacao_2 = $('#chksituacao_2:checked').val();

		var chkobsgeralcadbasic = $('#chkobsgeralcadbasic').val();
			
		if( documentacao_1 != null ){
			documentacao = 'S';
		}else if(documentacao_2 != null){
			documentacao = 'N';
		}else{
			documentacao = '';
		}
		
		if( instalacoes_1 != null ){
			instalacoes = 'S';
		}else if(instalacoes_2 != null){
			instalacoes = 'N';
		}else{
			instalacoes = '';
		}
		
		if( pessoal_1 != null ){
			pessoal = 'S';
		}else if(pessoal_2 != null){
			pessoal = 'N';
		}else{
			pessoal = '';
		}
		
		if( servicos_1 != null ){
			servicos = 'S';
		}else if(servicos_2 != null){
			servicos = 'N';
		}else{
			servicos = '';
		}
		
		if( pagamentos_1 != null ){
			pagamentos = 'S';
		}else if(pagamentos_2 != null){
			pagamentos = 'N';
		}else {
			pagamentos = '';
		}
		
		if( dadosobra_1 != null ){
			dadosobra = 'S';
		}else if(dadosobra_2 != null){
			dadosobra = 'N';
		}else{
			dadosobra = '';
		}
		if(dadosobra_3 != null){
			dadosobrachekc = 'TRUE';
		}else if(dadosobra_3 == null){
			dadosobrachekc = 'FALSE';
		}
		
		if( projetos_1 != null ){
			projetos = 'S';
		}else if(projetos_2 != null){
			projetos = 'N';
		}else{
			projetos = '';
		}
		if(projetos_3 != null){
			projetoschekc = 'TRUE';
		}else if(projetos_3 == null){
			projetoschekc = 'FALSE';
		}
		
		if( licitacao_1 != null ){
			licitacao = 'S';
		}else if(licitacao_2 != null){
			licitacao = 'N';
		}else{
			licitacao = '';
		}
		if(licitacao_3 != null){
			licitacaochekc = 'TRUE';
		}else if(licitacao_3 == null){
			licitacaochekc = 'FALSE';
		}
		
		if( contratacao_1 != null ){
			contratacao = 'S';
		}else if(contratacao_2 != null){
			contratacao = 'N';
		}else{
			contratacao = '';
		}
		if(contratacao_3 != null){
			contratacaochekc = 'TRUE';
		}else if(contratacao_3 == null){
			contratacaochekc = 'FALSE';
		}
		
		if( cronograma_1 != null ){
			cronograma = 'S';
		}else if(cronograma_2 != null){
			cronograma = 'N';
		}else{
			cronograma = '';
		}
		if(cronograma_3 != null){
			cronogramachekc = 'TRUE';
		}else if(cronograma_3 == null){
			cronogramachekc = 'FALSE';
		}
		
		if( vistoria_1 != null ){
			vistoria = 'S';
		}else if(vistoria_2 != null){
			vistoria = 'N';
		}else{
			vistoria = '';
		}
		if(vistoria_3 != null){
			vistoriachekc = 'TRUE';
		}else if(vistoria_3 == null){
			vistoriachekc = 'FALSE';
		}
		
		if( restricoes_1 != null ){
			restricoes = 'S';
		}else if(restricoes_2 != null){
			restricoes = 'N';
		}else{
			restricoes = '';
		}
		if(restricoes_3 != null){
			restricoeschekc = 'TRUE';
		}else if(restricoes_3 == null){
			restricoeschekc = 'FALSE';
		}
		
		if( documentos_1 != null ){
			documentos = 'S';
		}else if(documentos_2 != null){
			documentos = 'N';
		}else{
			documentos = '';
		}
		if(documentos_3 != null){
			documentoschekc = 'TRUE';
		}else if(documentos_3 == null){
			documentoschekc = 'FALSE';
		}

		if(chksituacao_1 != null ){
			chksituacao = 'TRUE';
		}else if(chksituacao_2 != null){
			chksituacao = 'FALSE';
		}else{
			chksituacao = '';
		}
		
		if(nisid == ''){
			alert('O Nível de satisfação da Supervisão é de preenchimento obrigatório!');
			return false;
		}
		
		$.ajax({
	   		type: "POST",
	   		url: "obras.php?modulo=principal/supervisao/check_list_visita&acao=A",
	   		data: "requisicao=atualizarParecer&mpc_id="+mpc_id+"&chkobsmec="+chkobsmec+"&nisid="+nisid+"&documentacao="+documentacao+"&chkobsdoc="+chkobsdoc+"&instalacoes="+instalacoes+"&chkobsinst="+chkobsinst+"&pessoal="+pessoal+"&chkobspess="+chkobspess+"&servicos="+servicos+"&chkobsserv="+chkobsserv+"&pagamentos="+pagamentos+"&chkobspag="+chkobspag+"&dadosobra="+dadosobra+"&dadosobrachekc="+dadosobrachekc+"&chkobsdadosobr="+chkobsdadosobr+"&projetos="+projetos+"&projetoschekc="+projetoschekc+"&licitacao="+licitacao+"&licitacaochekc="+licitacaochekc+"&contratacao="+contratacao+"&contratacaochekc="+contratacaochekc+"&cronograma="+cronograma+"&cronogramachekc="+cronogramachekc+"&vistoria="+vistoria+"&vistoriachekc="+vistoriachekc+"&restricoes="+restricoes+"&restricoeschekc="+restricoeschekc+"&documentos="+documentos+"&documentoschekc="+documentoschekc+"&chkobsproj="+chkobsproj+"&chkobslic="+chkobslic+"&chkobscontr="+chkobscontr+"&chkobscron="+chkobscron+"&chkobsvist="+chkobsvist+"&chkobsrestr="+chkobsrestr+"&chkobsdocument="+chkobsdocument+"&chkobsgeral="+chkobsgeral+"&chksituacao="+chksituacao+"&chkobsgeralcadbasic="+chkobsgeralcadbasic,
	   		async: false,
	   		success: function(data){alert(data);}
 		});

		window.opener.location=window.opener.location;
		window.close();
		
	})

	$('#dadosobra_1').click(function()
			{
				$('#dadosobra_3').attr('checked', true);
			});
			$('#dadosobra_2').click(function()
			{
				$('#dadosobra_3').attr('checked', false);
			});
			
			$('#projetos_1').click(function()
			{
				$('#projetos_3').attr('checked', true);
			});
			$('#projetos_2').click(function()
			{
				$('#projetos_3').attr('checked', false);
			});
			
			$('#licitacao_1').click(function()
			{
				$('#licitacao_3').attr('checked', true);
			});
			$('#licitacao_2').click(function()
			{
				$('#licitacao_3').attr('checked', false);
			});
			
			$('#contratacao_1').click(function()
			{
				$('#contratacao_3').attr('checked', true);
			});
			$('#contratacao_2').click(function()
			{
				$('#contratacao_3').attr('checked', false);
			});
			
			$('#cronograma_1').click(function()
			{
				$('#cronograma_3').attr('checked', true);
			});
			
			$('#cronograma_2').click(function()
			{
				$('#cronograma_3').attr('checked', false);
			});
			
			$('#vistoria_1').click(function()
			{
				$('#vistoria_3').attr('checked', true);
			});
			
			$('#vistoria_2').click(function()
			{
				$('#vistoria_3').attr('checked', false);
			});
			
			$('#restricoes_1').click(function()
			{
				$('#restricoes_3').attr('checked', true);
			});
			$('#restricoes_2').click(function()
			{
				$('#restricoes_3').attr('checked', false);
			});
			
			$('#documentos_1').click(function()
			{
				$('#documentos_3').attr('checked', true);
			});
			$('#documentos_2').click(function()
			{
				$('#documentos_3').attr('checked', false);
			});
			
});

function abreObservacoes( observacao ){

	switch( observacao ){

		case "documentacao":
			 if( $('#trDocumentacao').css('display') == 'none' ){
				$('#trDocumentacao').show();
			}else{
				$('#trDocumentacao').hide();
			}
		break;

		case "instalacoes":
			 if( $('#trInstalacoes').css('display') == 'none' ){
				$('#trInstalacoes').show();
			}else{
				$('#trInstalacoes').hide();
			}
		break;

		case "pessoal":
			 if( $('#trPessoal').css('display') == 'none' ){
				$('#trPessoal').show();
			}else{
				$('#trPessoal').hide();
			}
		break;
		
		case "servicos":
			 if( $('#trServicos').css('display') == 'none' ){
				$('#trServicos').show();
			}else{
				$('#trServicos').hide();
			}
		break;

		case "pagamentos":
			 if( $('#trPagamentos').css('display') == 'none' ){
				$('#trPagamentos').show();
			}else{
				$('#trPagamentos').hide();
			}
		break;

		case "dadosobra":
			 if( $('#trDadosObra').css('display') == 'none' ){
				$('#trDadosObra').show();
			}else{
				$('#trDadosObra').hide();
			}
		break;
		
		case "projetos":
			 if( $('#trProjetos').css('display') == 'none' ){
				$('#trProjetos').show();
			}else{
				$('#trProjetos').hide();
			}
		break;
		
		case "licitacao":
			 if( $('#trLicitacao').css('display') == 'none' ){
				$('#trLicitacao').show();
			}else{
				$('#trLicitacao').hide();
			}
		break;
		
		case "contratacao":
			 if( $('#trContratacao').css('display') == 'none' ){
				$('#trContratacao').show();
			}else{
				$('#trContratacao').hide();
			}
		break;
		
		case "cronograma":
			 if( $('#trCronograma').css('display') == 'none' ){
				$('#trCronograma').show();
			}else{
				$('#trCronograma').hide();
			}
		break;
		
		case "vistoria":
			 if( $('#trVistoria').css('display') == 'none' ){
				$('#trVistoria').show();
			}else{
				$('#trVistoria').hide();
			}
		break;
		
		case "restricoes":
			 if( $('#trRestricoes').css('display') == 'none' ){
				$('#trRestricoes').show();
			}else{
				$('#trRestricoes').hide();
			}
		break;
		
		case "documentos":
			 if( $('#trDocumentos').css('display') == 'none' ){
				$('#trDocumentos').show();
			}else{
				$('#trDocumentos').hide();
			}
				
	}
	
}
</script>
<form id="formulario">
<table border="0" width="100%" cellspacing="0" cellpadding="3" bgcolor="#DCDCDC" style="border-top: none; border-bottom: none;">
	<tr>
		<td width="100%" align="center" ><label class="TituloTela" style="color:#000000;"> Parecer Checklist</label></td>
	</tr>
	<tr>
		<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /><b> Indica os campos obrigatórios</b></td>
	</tr>
</table>
<table border="0" width="100%" >
	<tr>
		<td bgcolor="#e7e7e7" width="15%">Parecer da Supervisão(MEC)</td>
		<td><?php  echo campo_textarea( 'chkobsmec', 'N', 'S', '', '90', '10', '5000', '' , 0, '', false, NULL, $desc['mpcdetalhamento']); ?></td>
	</tr>
	<tr>
	<td colspan="2"><b>Análise do Checklist de Visita Técnica</b></td>
	</tr>
	<?php 
	if($analiseVisTec['acvdocumentacaoobs']){
		$possuiObsDocumentacao = '<label>&nbsp;&nbsp;<img src="/imagens/check_p.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes(\'documentacao\');" title="Possui Observação"/></label>';
	}
	?>
	<tr>
		<td bgcolor="#e7e7e7" width="15%" >1 - Quanto à Documentação:</td>
		<td>
			<label><input type="radio" name="documentacao" id="documentacao_1" value="S" <?=(($analiseVisTec['acvdocumentacao']== 'S')? 'checked=\"checked\"' : '') ?>/>Atende</label>
			<label><input type="radio" name="documentacao" id="documentacao_2" value="N" <?=(($analiseVisTec['acvdocumentacao']== 'N')? 'checked=\"checked\"' : '') ?>/>Não atende</label>
			<label>&nbsp;&nbsp;<img src="/imagens/editar_nome.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes('documentacao');" title="Inserir Observação"> Observação</label>
			<?=$possuiObsDocumentacao; ?>
		</td>
	</tr>
	<tr id="trDocumentacao" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea  id="chkobsdoc" name="chkobsdoc" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ><?php echo $analiseVisTec['acvdocumentacaoobs'] ; ?></textarea></td>
	</tr>
	<?php 
	if($analiseVisTec['acvinstcanteiroobs']){
		$possuiObsInst = '<label>&nbsp;&nbsp;<img src="/imagens/check_p.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes(\'instalacoes\');" title="Possui Observação"/></label>';
	}
	?>
	<tr>	
		<td bgcolor="#e7e7e7" width="15%" >2 - Quanto às Instalações do Canteiro de Obras:</td>
		<td>
			<label><input type="radio" name="instalacoes" id="instalacoes_1" value="S" <?=(($analiseVisTec['acvinstcanteiro'] == 'S')? 'checked=\"checked\"' : '')?> />Atende</label>
			<label><input type="radio" name="instalacoes" id="instalacoes_2" value="N" <?=(($analiseVisTec['acvinstcanteiro'] == 'N')? 'checked=\"checked\"' : '')?>/>Não atende</label>
			<label>&nbsp;&nbsp;<img src="/imagens/editar_nome.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes('instalacoes');" title="Inserir Observação"> Observação</label>
			<?=$possuiObsInst; ?>
		</td>
	</tr>
	<tr id="trInstalacoes" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea  id="chkobsinst" name="chkobsinst" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ><?php echo $analiseVisTec['acvinstcanteiroobs'];  ?></textarea></td>
	</tr>
	<?php 
	if($analiseVisTec['acvpessoalobs']){			
		$possuiObsPess ='<label>&nbsp;&nbsp;<img src="/imagens/check_p.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes(\'pessoal\');" title="Possui Observação"/></label>';
    }			
	?>
	<tr>	
		<td bgcolor="#e7e7e7" width="15%" >3 - Quanto ao Pessoal:</td>
		<td>
			<label><input type="radio" name="pessoal" id="pessoal_1" value="S" <?=(($analiseVisTec['acvpessoal'] == 'S')? 'checked=\"checked\"' : '')?> />Atende</label>
			<label><input type="radio" name="pessoal" id="pessoal_2" value="N" <?=(($analiseVisTec['acvpessoal'] == 'N')? 'checked=\"checked\"' : '')?> />Não atende</label>
			<label>&nbsp;&nbsp;<img src="/imagens/editar_nome.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes('pessoal');" title="Inserir Observação"> Observação</label>
			<?=$possuiObsPess; ?>
		</td>
	</tr>
	<tr id="trPessoal" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea  id="chkobspess" name="chkobspess" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ><?php echo $analiseVisTec['acvpessoalobs'];?></textarea></td>
	</tr>
	<?php 
	if($analiseVisTec['acvservicosobs']){			
		$possuiObsServ ='<label>&nbsp;&nbsp;<img src="/imagens/check_p.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes(\'servicos\');" title="Possui Observação"/></label>';
	}			
	?>
	<tr>	
		<td bgcolor="#e7e7e7" width="15%" >4 - Quanto aos Serviços:</td>
		<td>
			<label><input type="radio" name="servicos" id="servicos_1" value="S" <?=(($analiseVisTec['acvservicos']  == 'S')? 'checked=\"checked\"' : '')?> />Atende</label>
			<label><input type="radio" name="servicos" id="servicos_2" value="N" <?=(($analiseVisTec['acvservicos']  == 'N')? 'checked=\"checked\"' : '')?> />Não atende</label>
			<label>&nbsp;&nbsp;<img src="/imagens/editar_nome.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes('servicos');" title="Inserir Observação"> Observação</label>
			<?=$possuiObsServ; ?>
		</td>
	</tr>
	<tr id="trServicos" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea  id="chkobsserv" name="chkobsserv" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ><?php echo $analiseVisTec['acvservicosobs'];?></textarea></td>
	</tr>
	<?php 
	if($analiseVisTec['acvpagamentoobs']){			
		$possuiObsPag ='<label>&nbsp;&nbsp;<img src="/imagens/check_p.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes(\'pagamentos\');" title="Possui Observação"/></label>';
	}			
	?>
	<tr>	
		<td bgcolor="#e7e7e7" width="15%" >5 - Quanto aos Pagamentos efetuados:</td>
		<td>
			<label><input type="radio" name="pagamentos" id="pagamentos_1" value="S" <?=(($analiseVisTec['acvpagamento']   == 'S')? 'checked=\"checked\"' : '')?> />Atende</label>
			<label><input type="radio" name="pagamentos" id="pagamentos_2" value="N" <?=(($analiseVisTec['acvpagamento']   == 'N')? 'checked=\"checked\"' : '')?> />Não atende</label>
			<label>&nbsp;&nbsp;<img src="/imagens/editar_nome.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes('pagamentos');" title="Inserir Observação"> Observação</label>
			<?=$possuiObsPag; ?>
		</td>
	</tr>
	<tr id="trPagamentos" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea  id="chkobspag" name="chkobspag" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ><?php echo $analiseVisTec['acvpagamentoobs'];?></textarea></td>
	</tr>
	<tr>
		<td bgcolor="#e7e7e7" width="15%">Observação - Análise do Checklist de Visita Técnica:</td>
		<td><?php  echo campo_textarea( 'chkobsgeral', 'N', 'S', '', '90', '10', '5000', '' , 0, '', false, NULL, $analiseVisTec['acvobsgeral']); ?></td>
	</tr>
	<tr>
		<td colspan="2"><b>Análise do Cadastro</b></td>
	</tr>
	<?php 
	if($analiseCadBasic['acbdadosobraobs']){			
		$possuiObsDadosObr ='<label>&nbsp;&nbsp;<img src="/imagens/check_p.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes(\'dadosobra\');" title="Possui Observação"/></label>';
	}			
	?>
	<tr>
		<td bgcolor="#e7e7e7" width="15%" >1 - Dados da Obra:</td>
		<td>
			<label><input type="radio" name="dadosobra" id="dadosobra_1" value="S" <?=(($analiseCadBasic['acbdadosobra'] == 'S')? 'checked=\"checked\"' : '')?> />Atende</label>
			<label><input type="radio" name="dadosobra" id="dadosobra_2" value="N" <?=(($analiseCadBasic['acbdadosobra'] == 'N')? 'checked=\"checked\"' : '')?> />Não atende</label>
			<label><input type="checkbox" name="dadosobra" id="dadosobra_3" value="TRUE" <?=(($analiseCadBasic['acbdadosobrasitaba'] == 't')? 'checked=\"checked\"' : '')?> />Bloquear Aba</label>
			<label>&nbsp;&nbsp;<img src="/imagens/editar_nome.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes('dadosobra');" title="Inserir Observação"> Observação</label>
			<?=$possuiObsDadosObr; ?>
		</td>
	</tr>
	<tr id="trDadosObra" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea  id="chkobsdadosobr" name="chkobsdadosobr" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ><?php echo $analiseCadBasic['acbdadosobraobs'];?></textarea></td>
	</tr>
	<?php 
	if($analiseCadBasic['acbprojetosobs']){			
		$possuiObsProj ='<label>&nbsp;&nbsp;<img src="/imagens/check_p.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes(\'projetos\');" title="Possui Observação"/></label>';
	}			
	?>
	<tr>
		<td bgcolor="#e7e7e7" width="15%" >2 - Projetos:</td>
		<td>
			<label><input type="radio" name="projetos" id="projetos_1" value="S" <?=(($analiseCadBasic['acbprojetos'] == 'S')? 'checked=\"checked\"' : '')?> />Atende</label>
			<label><input type="radio" name="projetos" id="projetos_2" value="N" <?=(($analiseCadBasic['acbprojetos'] == 'N')? 'checked=\"checked\"' : '')?> />Não atende</label>
			<label><input type="checkbox" name="projetos" id="projetos_3" value="TRUE" <?=(($analiseCadBasic['acbprojetossitaba'] == 't')? 'checked=\"checked\"' : '')?> />Bloquear Aba</label>
			<label>&nbsp;&nbsp;<img src="/imagens/editar_nome.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes('projetos');" title="Inserir Observação"> Observação</label>
			<?=$possuiObsProj; ?>
		</td>
	</tr>
	<tr id="trProjetos" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea  id="chkobsproj" name="chkobsproj" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ><?php echo $analiseCadBasic['acbprojetosobs'];?></textarea></td>
	</tr>
	<?php 
	if($analiseCadBasic['acblicitacaoobs']){			
		$possuiObsLict ='<label>&nbsp;&nbsp;<img src="/imagens/check_p.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes(\'licitacao\');" title="Possui Observação"/></label>';
	}			
	?>
	<tr>
		<td bgcolor="#e7e7e7" width="15%" >3 - Licitação:</td>
		<td>
			<label><input type="radio" name="licitacao" id="licitacao_1" value="S" <?=(($analiseCadBasic['acblicitacao'] == 'S')? 'checked=\"checked\"' : '')?> />Atende</label>
			<label><input type="radio" name="licitacao" id="licitacao_2" value="N" <?=(($analiseCadBasic['acblicitacao'] == 'N')? 'checked=\"checked\"' : '')?> />Não atende</label>
			<label><input type="checkbox" name="licitacao" id="licitacao_3" value="TRUE" <?=(($analiseCadBasic['acblicitacaositaba'] == 't')? 'checked=\"checked\"' : '')?> />Bloquear Aba</label>
			<label>&nbsp;&nbsp;<img src="/imagens/editar_nome.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes('licitacao');" title="Inserir Observação"> Observação</label>
			<?=$possuiObsLict; ?>
		</td>
	</tr>
	<tr id="trLicitacao" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea  id="chkobslic" name="chkobslic" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ><?php echo $analiseCadBasic['acblicitacaoobs'];?></textarea></td>
	</tr>
	<?php 
	if($analiseCadBasic['acbcontratacaoobs']){			
		$possuiObsContrat ='<label>&nbsp;&nbsp;<img src="/imagens/check_p.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes(\'contratacao\');" title="Possui Observação"/></label>';
	}			
	?>
	<tr>
		<td bgcolor="#e7e7e7" width="15%" >4 - Contratação:</td>
		<td>
			<label><input type="radio" name="contratacao" id="contratacao_1" value="S" <?=(($analiseCadBasic['acbcontratacao'] == 'S')? 'checked=\"checked\"' : '')?> />Atende</label>
			<label><input type="radio" name="contratacao" id="contratacao_2" value="N" <?=(($analiseCadBasic['acbcontratacao'] == 'N')? 'checked=\"checked\"' : '')?> />Não atende</label>
			<label><input type="checkbox" name="contratacao" id="contratacao_3" value="TRUE" <?=(($analiseCadBasic['acbcontratacaositaba'] == 't')? 'checked=\"checked\"' : '')?> />Bloquear Aba</label>
			<label>&nbsp;&nbsp;<img src="/imagens/editar_nome.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes('contratacao');" title="Inserir Observação"> Observação</label>
			<?=$possuiObsContrat; ?>
		</td>
	</tr>
	<tr id="trContratacao" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea  id="chkobscontr" name="chkobscontr" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ><?php echo $analiseCadBasic['acbcontratacaoobs'];?></textarea></td>
	</tr>
	<?php 
	if($analiseCadBasic['acbcronogramaobs']){			
		$possuiObsCronog ='<label>&nbsp;&nbsp;<img src="/imagens/check_p.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes(\'cronograma\');" title="Possui Observação"/></label>';
	}			
	?>
	<tr>
		<td bgcolor="#e7e7e7" width="15%" >5 - Cronograma F/F:</td>
		<td>
			<label><input type="radio" name="cronograma" id="cronograma_1" value="S" <?=(($analiseCadBasic['acbcronograma'] == 'S')? 'checked=\"checked\"' : '')?> />Atende</label>
			<label><input type="radio" name="cronograma" id="cronograma_2" value="N" <?=(($analiseCadBasic['acbcronograma'] == 'N')? 'checked=\"checked\"' : '')?> />Não atende</label>
			<label><input type="checkbox" name="cronograma" id="cronograma_3" value="TRUE" <?=(($analiseCadBasic['acbcronogramasitaba'] == 't')? 'checked=\"checked\"' : '')?> />Bloquear Aba</label>
			<label>&nbsp;&nbsp;<img src="/imagens/editar_nome.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes('cronograma');" title="Inserir Observação"> Observação</label>
			<?=$possuiObsCronog; ?>
		</td>
	</tr>
	<tr id="trCronograma" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea  id="chkobscron" name="chkobscron" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ><?php echo $analiseCadBasic['acbcronogramaobs'];?></textarea></td>
	</tr>
	<?php 
	if($analiseCadBasic['acbvistoriaobs']){			
		$possuiObsVist ='<label>&nbsp;&nbsp;<img src="/imagens/check_p.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes(\'vistoria\');" title="Possui Observação"/></label>';
	}			
	?>
	<tr>
		<td bgcolor="#e7e7e7" width="15%" >6 - Vistoria:</td>
		<td>
			<label><input type="radio" name="vistoria" id="vistoria_1" value="S" <?=(($analiseCadBasic['acbvistoria'] == 'S')? 'checked=\"checked\"' : '')?> />Atende</label>
			<label><input type="radio" name="vistoria" id="vistoria_2" value="N" <?=(($analiseCadBasic['acbvistoria']== 'N')? 'checked=\"checked\"' : '')?> />Não atende</label>
			<label><input type="checkbox" name="vistoria" id="vistoria_3" value="TRUE" <?=(($analiseCadBasic['acbvistoriasitaba'] == 't')? 'checked=\"checked\"' : '')?> />Bloquear Aba</label>
			<label>&nbsp;&nbsp;<img src="/imagens/editar_nome.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes('vistoria');" title="Inserir Observação"> Observação</label>
			<?=$possuiObsVist; ?>
		</td>
	</tr>
	<tr id="trVistoria" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea  id="chkobsvist" name="chkobsvist" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ><?php echo $analiseCadBasic['acbvistoriaobs'];?></textarea></td>
	</tr>
	<?php 
	if($analiseCadBasic['acbrestricaoobs']){			
		$possuiObsrestric ='<label>&nbsp;&nbsp;<img src="/imagens/check_p.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes(\'restricoes\');" title="Possui Observação"/></label>';
	}			
	?>
	<tr>
		<td bgcolor="#e7e7e7" width="15%" >7 - Restrições e Providências:</td>
		<td>
			<label><input type="radio" name="restricoes" id="restricoes_1" value="S" <?=(($analiseCadBasic['acbrestricao'] == 'S')? 'checked=\"checked\"' : '')?> />Atende</label>
			<label><input type="radio" name="restricoes" id="restricoes_2" value="N" <?=(($analiseCadBasic['acbrestricao'] == 'N')? 'checked=\"checked\"' : '')?> />Não atende</label>
			<label><input type="checkbox" name="restricoes" id="restricoes_3" value="TRUE" <?=(($analiseCadBasic['acbrestricaositaba'] == 't')? 'checked=\"checked\"' : '')?> />Bloquear Aba</label>
			<label>&nbsp;&nbsp;<img src="/imagens/editar_nome.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes('restricoes');" title="Inserir Observação"> Observação</label>
			<?=$possuiObsrestric; ?>
		</td>
	</tr>
	<tr id="trRestricoes" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea  id="chkobsrestr" name="chkobsrestr" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ><?php echo $analiseCadBasic['acbrestricaoobs'];?></textarea></td>
	</tr>
	<?php 
	if($analiseCadBasic['acbdocumentoobs']){			
		$possuiObsDocument ='<label>&nbsp;&nbsp;<img src="/imagens/check_p.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes(\'documentos\');" title="Possui Observação"/></label>';
	}			
	?>
	<tr>
		<td bgcolor="#e7e7e7" width="15%" >8 - Documentos:</td>
		<td>
			<label><input type="radio" name="documentos" id="documentos_1" value="S" <?=(($analiseCadBasic['acbdocumento'] == 'S')? 'checked=\"checked\"' : '')?> />Atende</label>
			<label><input type="radio" name="documentos" id="documentos_2" value="N" <?=(($analiseCadBasic['acbdocumento'] == 'N')? 'checked=\"checked\"' : '')?> />Não atende</label>
			<label><input type="checkbox" name="documentos" id="documentos_3" value="TRUE" <?=(($analiseCadBasic['acbdocumentositaba'] == 't')? 'checked=\"checked\"' : '')?> />Bloquear Aba</label>
			<label>&nbsp;&nbsp;<img src="/imagens/editar_nome.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes('documentos');" title="Inserir Observação"> Observação</label>
			<?=$possuiObsDocument; ?>
		</td>
	</tr>
	<tr id="trDocumentos" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea  id="chkobsdocument" name="chkobsdocument" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ><?php echo $analiseCadBasic['acbdocumentoobs'];?></textarea></td>
	</tr>
	<tr>
		<td bgcolor="#e7e7e7" width="15%">Observação - Análise do Cadastro:</td>
		<td><?php  echo campo_textarea( 'chkobsgeralcadbasic', 'N', 'S', '', '90', '10', '5000', '' , 0, '', false, NULL, $analiseCadBasic['acbobsgeral']); ?></td>
	</tr>
	<tr>
		<td bgcolor="#e7e7e7" width="30%" >Nível de satisfação da Supervisão</td>
		<td>
			<?php $nisid = $desc['nisid']; ?>
			<?php $sql = " SELECT nisid AS codigo, nisdsc AS descricao FROM	obras.nivelsatisfacao ORDER BY nisid"; ?>
			<?php echo $db->monta_combo( 'nisid', $sql, 'S', 'Selecione...', '', '', '', 100 ,'S', 'nisid','',$nisid); ?>
		</td>
	</tr>
	<tr>
		<td colspan="2"><b>Situação do Parecer</b></td>
	</tr>
	<tr>
		<td bgcolor="#e7e7e7" width="15%" >Situação:</td>
		<td>
			<label><input type="radio" name="chksituacao" id="chksituacao_1" value="S" <?=(($desc['mpcsituacao'] == 't')? 'checked=\"checked\"' : '')?> />Aprovado</label>
			<label><input type="radio" name="chksituacao" id="chksituacao_2" value="N" <?=(($desc['mpcsituacao'] == 'f')? 'checked=\"checked\"' : '')?>/>Não Aprovado</label>
		</td>
	</tr>
	<tr>
		<td></td>
	</tr>
	<tr>
		<td>
			<input type="button" value="Salvar Parecer" id="salvarParecer" />
		</td>
		<td>&nbsp;</td>
	</tr>
</table>
</form>
<?php	
}

function parecerDesc($mpc_id){
	global $db;
	
	$sql = "SELECT
				mpcdetalhamento,
				nisid,
				mpcsituacao
			FROM
				obras.movparecercklist
			WHERE
				mpc_id = {$mpc_id}";

	$desc = $db->pegaLinha($sql);
	
	//Selecionando os valores da Análise do Checklist de Visita Técnica
	$sqlVisTec = "SELECT 
						acvdocumentacao, acvdocumentacaoobs, 
						acvinstcanteiro, acvinstcanteiroobs, 
						acvpessoal, acvpessoalobs, 
						acvservicos, acvservicosobs, 
	       				acvpagamento, acvpagamentoobs, 
	       				acvobsgeral 
	  			FROM 
	  				    obras.analisechecklist 
			    WHERE
					mpc_id = {$mpc_id}
					AND acvstatus = 'A'";

	$analiseVisTec = $db->pegaLinha($sqlVisTec);

	//Selecionando os valores da Análise do Cadastro Básico
	$sqlCadBasic = "SELECT 
						acbdadosobra, acbdadosobraobs, acbdadosobrasitaba, 
				        acbprojetos, acbprojetosobs, acbprojetossitaba, 
				        acblicitacao, acblicitacaoobs, acblicitacaositaba, 
				        acbcontratacao, acbcontratacaoobs, acbcontratacaositaba, 
				        acbcronograma, acbcronogramaobs, acbcronogramasitaba, 
				        acbvistoria, acbvistoriaobs, acbvistoriasitaba, 
				        acbrestricao, acbrestricaoobs, acbrestricaositaba, 
				        acbdocumento, acbdocumentoobs, acbdocumentositaba,
				        acbobsgeral 
				  FROM 
				  		obras.analisecadastrobasico
				  WHERE  
				 		mpc_id = {$mpc_id}
				 		AND acbstatus = 'A' ";
	 $analiseCadBasic = $db->pegaLinha($sqlCadBasic);
	
	 if($mpc_id){
	 	$disabled = 'disabled=\"disabled\"';
	 }
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
function abreObservacoes( observacao ){

	switch( observacao ){

		case "documentacao":
			 if( $('#trDocumentacao').css('display') == 'none' ){
				$('#trDocumentacao').show();
			}else{
				$('#trDocumentacao').hide();
			}
		break;

		case "instalacoes":
			 if( $('#trInstalacoes').css('display') == 'none' ){
				$('#trInstalacoes').show();
			}else{
				$('#trInstalacoes').hide();
			}
		break;

		case "pessoal":
			 if( $('#trPessoal').css('display') == 'none' ){
				$('#trPessoal').show();
			}else{
				$('#trPessoal').hide();
			}
		break;
		
		case "servicos":
			 if( $('#trServicos').css('display') == 'none' ){
				$('#trServicos').show();
			}else{
				$('#trServicos').hide();
			}
		break;

		case "pagamentos":
			 if( $('#trPagamentos').css('display') == 'none' ){
				$('#trPagamentos').show();
			}else{
				$('#trPagamentos').hide();
			}
		break;

		case "dadosobra":
			 if( $('#trDadosObra').css('display') == 'none' ){
				$('#trDadosObra').show();
			}else{
				$('#trDadosObra').hide();
			}
		break;
		
		case "projetos":
			 if( $('#trProjetos').css('display') == 'none' ){
				$('#trProjetos').show();
			}else{
				$('#trProjetos').hide();
			}
		break;
		
		case "licitacao":
			 if( $('#trLicitacao').css('display') == 'none' ){
				$('#trLicitacao').show();
			}else{
				$('#trLicitacao').hide();
			}
		break;
		
		case "contratacao":
			 if( $('#trContratacao').css('display') == 'none' ){
				$('#trContratacao').show();
			}else{
				$('#trContratacao').hide();
			}
		break;
		
		case "cronograma":
			 if( $('#trCronograma').css('display') == 'none' ){
				$('#trCronograma').show();
			}else{
				$('#trCronograma').hide();
			}
		break;
		
		case "vistoria":
			 if( $('#trVistoria').css('display') == 'none' ){
				$('#trVistoria').show();
			}else{
				$('#trVistoria').hide();
			}
		break;
		
		case "restricoes":
			 if( $('#trRestricoes').css('display') == 'none' ){
				$('#trRestricoes').show();
			}else{
				$('#trRestricoes').hide();
			}
		break;
		
		case "documentos":
			 if( $('#trDocumentos').css('display') == 'none' ){
				$('#trDocumentos').show();
			}else{
				$('#trDocumentos').hide();
			}
				
	}
	
}
</script>
<table border="0" width="100%" cellspacing="0" cellpadding="3" bgcolor="#DCDCDC" style="border-top: none; border-bottom: none;">
	<tr>
		<td width="100%" align="center" ><label class="TituloTela" style="color:#000000;"> Parecer Checklist</label></td>
	</tr>
	<tr>
		<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /><b> Indica os campos obrigatórios</b></td>
	</tr>
</table>
<table border="0" width="100%">
	<tr>
		<td bgcolor="#e7e7e7" width="15%">Parecer da Supervisão(MEC)</td>
		<td><textarea <?=$disabled;?> id="chkobsmec" name="chkobsmec" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ><?php echo $desc['mpcdetalhamento']; ?></textarea></td>
	</tr>
	<tr>
	<td colspan="2"><b>Análise do Checklist de Visita Técnica</b></td>
	</tr>
	<?php 
	if($analiseVisTec['acvdocumentacaoobs']){
		$possuiObsDocumentacao = '<label>&nbsp;&nbsp;<img src="/imagens/check_p.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes(\'documentacao\');" title="Possui Observação"/></label>';
	}
	?>
	<tr>
		<td bgcolor="#e7e7e7" width="15%" >1 - Quanto à Documentação:</td>
		<td>
			<label><input <?=$disabled;?> type="radio" name="documentacao" id="documentacao_1" value="S" <?=(($analiseVisTec['acvdocumentacao']== 'S')? 'checked=\"checked\"' : '') ?>/>Atende</label>
			<label><input <?=$disabled;?> type="radio" name="documentacao" id="documentacao_2" value="N" <?=(($analiseVisTec['acvdocumentacao']== 'N')? 'checked=\"checked\"' : '') ?>/>Não atende</label>
			<?=$possuiObsDocumentacao; ?>
		</td>
	</tr>
	<tr id="trDocumentacao" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea <?=$disabled;?> id="chkobsdoc" name="chkobsdoc" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ><?php echo $analiseVisTec['acvdocumentacaoobs'] ; ?></textarea></td>
	</tr>
	<?php 
	if($analiseVisTec['acvinstcanteiroobs']){
		$possuiObsInst = '<label>&nbsp;&nbsp;<img src="/imagens/check_p.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes(\'instalacoes\');" title="Possui Observação"/></label>';
	}
	?>
	<tr>	
		<td bgcolor="#e7e7e7" width="15%" >2 - Quanto às Instalações do Canteiro de Obras:</td>
		<td>
			<label><input <?=$disabled;?> type="radio" name="instalacoes" id="instalacoes_1" value="S" <?=(($analiseVisTec['acvinstcanteiro'] == 'S')? 'checked=\"checked\"' : '')?> />Atende</label>
			<label><input <?=$disabled;?> type="radio" name="instalacoes" id="instalacoes_2" value="N" <?=(($analiseVisTec['acvinstcanteiro'] == 'N')? 'checked=\"checked\"' : '')?>/>Não atende</label>
			<?=$possuiObsInst; ?>
		</td>
	</tr>
	<tr id="trInstalacoes" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea <?=$disabled;?> id="chkobsinst" name="chkobsinst" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ><?php echo $analiseVisTec['acvinstcanteiroobs'];  ?></textarea></td>
	</tr>
	<?php 
	if($analiseVisTec['acvpessoalobs']){			
		$possuiObsPess ='<label>&nbsp;&nbsp;<img src="/imagens/check_p.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes(\'pessoal\');" title="Possui Observação"/></label>';
	}			
	?>
	<tr>	
		<td bgcolor="#e7e7e7" width="15%" >3 - Quanto ao Pessoal:</td>
		<td>
			<label><input <?=$disabled;?> type="radio" name="pessoal" id="pessoal_1" value="S" <?=(($analiseVisTec['acvpessoal'] == 'S')? 'checked=\"checked\"' : '')?> />Atende</label>
			<label><input <?=$disabled;?> type="radio" name="pessoal" id="pessoal_2" value="N" <?=(($analiseVisTec['acvpessoal'] == 'N')? 'checked=\"checked\"' : '')?> />Não atende</label>
			<?=$possuiObsPess; ?>			
		</td>
	</tr>
	<tr id="trPessoal" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea <?=$disabled;?> id="chkobspess" name="chkobspess" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ><?php echo $analiseVisTec['acvpessoalobs'];?></textarea></td>
	</tr>
	<?php 
	if($analiseVisTec['acvservicosobs']){			
		$possuiObsServ ='<label>&nbsp;&nbsp;<img src="/imagens/check_p.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes(\'servicos\');" title="Possui Observação"/></label>';
	}			
	?>
	<tr>	
		<td bgcolor="#e7e7e7" width="15%" >4 - Quanto aos Serviços:</td>
		<td>
			<label><input <?=$disabled;?> type="radio" name="servicos" id="servicos_1" value="S" <?=(($analiseVisTec['acvservicos']  == 'S')? 'checked=\"checked\"' : '')?> />Atende</label>
			<label><input <?=$disabled;?> type="radio" name="servicos" id="servicos_2" value="N" <?=(($analiseVisTec['acvservicos']  == 'N')? 'checked=\"checked\"' : '')?> />Não atende</label>
			<?=$possuiObsServ; ?>			
		</td>
	</tr>
	<tr id="trServicos" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea <?=$disabled;?> id="chkobsserv" name="chkobsserv" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ><?php echo $analiseVisTec['acvservicosobs'];?></textarea></td>
	</tr>
	<?php 
	if($analiseVisTec['acvpagamentoobs']){			
		$possuiObsPag ='<label>&nbsp;&nbsp;<img src="/imagens/check_p.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes(\'pagamentos\');" title="Possui Observação"/></label>';
	}			
	?>
	<tr>	
		<td bgcolor="#e7e7e7" width="15%" >5 - Quanto aos Pagamentos efetuados:</td>
		<td>
			<label><input <?=$disabled;?> type="radio" name="pagamentos" id="pagamentos_1" value="S" <?=(($analiseVisTec['acvpagamento']   == 'S')? 'checked=\"checked\"' : '')?> />Atende</label>
			<label><input <?=$disabled;?> type="radio" name="pagamentos" id="pagamentos_2" value="N" <?=(($analiseVisTec['acvpagamento']   == 'N')? 'checked=\"checked\"' : '')?> />Não atende</label>
			<?=$possuiObsPag; ?>			
		</td>
	</tr>
	<tr id="trPagamentos" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea <?=$disabled;?> id="chkobspag" name="chkobspag" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ><?php echo $analiseVisTec['acvpagamentoobs'];?></textarea></td>
	</tr>
	<tr>
		<td bgcolor="#e7e7e7" width="15%">Observação - Análise do Checklist de Visita Técnica:</td>
		<td><textarea <?=$disabled;?> id="chkobsgeral" name="chkobsgeral" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ><?php echo $analiseVisTec['acvobsgeral']; ?></textarea></td>
	</tr>
	<tr>
		<td colspan="2"><b>Análise do Cadastro</b></td>
	</tr>
	<?php 
	if($analiseCadBasic['acbdadosobraobs']){			
		$possuiObsDadosObr ='<label>&nbsp;&nbsp;<img src="/imagens/check_p.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes(\'dadosobra\');" title="Possui Observação"/></label>';
	}			
	?>
	<tr>
		<td bgcolor="#e7e7e7" width="15%" >1 - Dados da Obra:</td>
		<td>
			<label><input <?=$disabled;?> type="radio" name="dadosobra" id="dadosobra_1" value="S" <?=(($analiseCadBasic['acbdadosobra'] == 'S')? 'checked=\"checked\"' : '')?> />Atende</label>
			<label><input <?=$disabled;?> type="radio" name="dadosobra" id="dadosobra_2" value="N" <?=(($analiseCadBasic['acbdadosobra'] == 'N')? 'checked=\"checked\"' : '')?> />Não atende</label>
			<label><input <?=$disabled;?> type="checkbox" name="dadosobra" id="dadosobra_3" value="TRUE" <?=(($analiseCadBasic['acbdadosobrasitaba'] == 't')? 'checked=\"checked\"' : '')?> />Bloquear Aba</label>
			<?=$possuiObsDadosObr; ?>		
		</td>
	</tr>
	<tr id="trDadosObra" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea <?=$disabled;?> id="chkobsdadosobr" name="chkobsdadosobr" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ><?php echo $analiseCadBasic['acbdadosobraobs'];?></textarea></td>
	</tr>
	<?php 
	if($analiseCadBasic['acbprojetosobs']){			
			$possuiObsProj ='<label>&nbsp;&nbsp;<img src="/imagens/check_p.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes(\'projetos\');" title="Possui Observação"/></label>';
		}			
	?>
	<tr>
		<td bgcolor="#e7e7e7" width="15%" >2 - Projetos:</td>
		<td>
			<label><input <?=$disabled;?> type="radio" name="projetos" id="projetos_1" value="S" <?=(($analiseCadBasic['acbprojetos'] == 'S')? 'checked=\"checked\"' : '')?> />Atende</label>
			<label><input <?=$disabled;?> type="radio" name="projetos" id="projetos_2" value="N" <?=(($analiseCadBasic['acbprojetos'] == 'N')? 'checked=\"checked\"' : '')?> />Não atende</label>
			<label><input <?=$disabled;?> type="checkbox" name="projetos" id="projetos_3" value="TRUE" <?=(($analiseCadBasic['acbprojetossitaba'] == 't')? 'checked=\"checked\"' : '')?> />Bloquear Aba</label>
			<?=$possuiObsProj; ?>			
		</td>
	</tr>
	<tr id="trProjetos" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea <?=$disabled;?> id="chkobsproj" name="chkobsproj" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ><?php echo $analiseCadBasic['acbprojetosobs'];?></textarea></td>
	</tr>
	<?php 
	if($analiseCadBasic['acblicitacaoobs']){			
		$possuiObsLict ='<label>&nbsp;&nbsp;<img src="/imagens/check_p.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes(\'licitacao\');" title="Possui Observação"/></label>';
	}			
	?>
	<tr>
		<td bgcolor="#e7e7e7" width="15%" >3 - Licitação:</td>
		<td>
			<label><input <?=$disabled;?> type="radio" name="licitacao" id="licitacao_1" value="S" <?=(($analiseCadBasic['acblicitacao'] == 'S')? 'checked=\"checked\"' : '')?> />Atende</label>
			<label><input <?=$disabled;?> type="radio" name="licitacao" id="licitacao_2" value="N" <?=(($analiseCadBasic['acblicitacao'] == 'N')? 'checked=\"checked\"' : '')?> />Não atende</label>
			<label><input <?=$disabled;?> type="checkbox" name="licitacao" id="licitacao_3" value="TRUE" <?=(($analiseCadBasic['acblicitacaositaba'] == 't')? 'checked=\"checked\"' : '')?> />Bloquear Aba</label>
			<?=$possuiObsLict; ?>			
		</td>
	</tr>
	<tr id="trLicitacao" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea <?=$disabled;?> id="chkobslic" name="chkobslic" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ><?php echo $analiseCadBasic['acblicitacaoobs'];?></textarea></td>
	</tr>
	<?php 
	if($analiseCadBasic['acbcontratacaoobs']){			
		$possuiObsContrat ='<label>&nbsp;&nbsp;<img src="/imagens/check_p.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes(\'contratacao\');" title="Possui Observação"/></label>';
	}			
	?>
	<tr>
		<td bgcolor="#e7e7e7" width="15%" >4 - Contratação:</td>
		<td>
			<label><input <?=$disabled;?> type="radio" name="contratacao" id="contratacao_1" value="S" <?=(($analiseCadBasic['acbcontratacao'] == 'S')? 'checked=\"checked\"' : '')?> />Atende</label>
			<label><input <?=$disabled;?> type="radio" name="contratacao" id="contratacao_2" value="N" <?=(($analiseCadBasic['acbcontratacao'] == 'N')? 'checked=\"checked\"' : '')?> />Não atende</label>
			<label><input <?=$disabled;?> type="checkbox" name="contratacao" id="contratacao_3" value="TRUE" <?=(($analiseCadBasic['acbcontratacaositaba'] == 't')? 'checked=\"checked\"' : '')?> />Bloquear Aba</label>
			<?=$possuiObsContrat; ?>			
		</td>
	</tr>
		<tr id="trContratacao" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea <?=$disabled;?> id="chkobscontr" name="chkobscontr" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ><?php echo $analiseCadBasic['acbcontratacaoobs'];?></textarea></td>
	</tr>
	<?php 
	if($analiseCadBasic['acbcronogramaobs']){			
		$possuiObsCronog ='<label>&nbsp;&nbsp;<img src="/imagens/check_p.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes(\'cronograma\');" title="Possui Observação"/></label>';
	}			
	?>
	<tr>
		<td bgcolor="#e7e7e7" width="15%" >5 - Cronograma F/F:</td>
		<td>
			<label><input <?=$disabled;?> type="radio" name="cronograma" id="cronograma_1" value="S" <?=(($analiseCadBasic['acbcronograma'] == 'S')? 'checked=\"checked\"' : '')?> />Atende</label>
			<label><input <?=$disabled;?> type="radio" name="cronograma" id="cronograma_2" value="N" <?=(($analiseCadBasic['acbcronograma'] == 'N')? 'checked=\"checked\"' : '')?> />Não atende</label>
			<label><input <?=$disabled;?> type="checkbox" name="cronograma" id="cronograma_3" value="TRUE" <?=(($analiseCadBasic['acbcronogramasitaba'] == 't')? 'checked=\"checked\"' : '')?> />Bloquear Aba</label>
			<?=$possuiObsCronog; ?>			
		</td>
	</tr>
	<tr id="trCronograma" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea <?=$disabled;?> id="chkobscron" name="chkobscron" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ><?php echo $analiseCadBasic['acbcronogramaobs'];?></textarea></td>
	</tr>
	<?php 
	if($analiseCadBasic['acbvistoriaobs']){			
		$possuiObsVist ='<label>&nbsp;&nbsp;<img src="/imagens/check_p.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes(\'vistoria\');" title="Possui Observação"/></label>';
	}			
	?>
	<tr>
		<td bgcolor="#e7e7e7" width="15%" >6 - Vistoria:</td>
		<td>
			<label><input <?=$disabled;?> type="radio" name="vistoria" id="vistoria_1" value="S" <?=(($analiseCadBasic['acbvistoria'] == 'S')? 'checked=\"checked\"' : '')?> />Atende</label>
			<label><input <?=$disabled;?> type="radio" name="vistoria" id="vistoria_2" value="N" <?=(($analiseCadBasic['acbvistoria']== 'N')? 'checked=\"checked\"' : '')?> />Não atende</label>
			<label><input <?=$disabled;?> type="checkbox" name="vistoria" id="vistoria_3" value="TRUE" <?=(($analiseCadBasic['acbvistoriasitaba'] == 't')? 'checked=\"checked\"' : '')?> />Bloquear Aba</label>
			<?=$possuiObsVist; ?>			
		</td>
	</tr>
	<tr id="trVistoria" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea <?=$disabled;?> id="chkobsvist" name="chkobsvist" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ><?php echo $analiseCadBasic['acbvistoriaobs'];?></textarea></td>
	</tr>
	<?php 
	if($analiseCadBasic['acbrestricaoobs']){			
		$possuiObsrestric ='<label>&nbsp;&nbsp;<img src="/imagens/check_p.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes(\'restricoes\');" title="Possui Observação"/></label>';
	}			
	?>	
	<tr>
		<td bgcolor="#e7e7e7" width="15%" >7 - Restrições e Providências:</td>
		<td>
			<label><input <?=$disabled;?> type="radio" name="restricoes" id="restricoes_1" value="S" <?=(($analiseCadBasic['acbrestricao'] == 'S')? 'checked=\"checked\"' : '')?> />Atende</label>
			<label><input <?=$disabled;?> type="radio" name="restricoes" id="restricoes_2" value="N" <?=(($analiseCadBasic['acbrestricao'] == 'N')? 'checked=\"checked\"' : '')?> />Não atende</label>
			<label><input <?=$disabled;?> type="checkbox" name="restricoes" id="restricoes_3" value="TRUE" <?=(($analiseCadBasic['acbrestricaositaba'] == 't')? 'checked=\"checked\"' : '')?> />Bloquear Aba</label>
			<?=$possuiObsrestric; ?>			
		</td>
	</tr>
	<tr id="trRestricoes" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea <?=$disabled;?> id="chkobsrestr" name="chkobsrestr" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ><?php echo $analiseCadBasic['acbrestricaoobs'];?></textarea></td>
	</tr>
	<?php 
	if($analiseCadBasic['acbdocumentoobs']){			
		$possuiObsDocument ='<label>&nbsp;&nbsp;<img src="/imagens/check_p.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes(\'documentos\');" title="Possui Observação"/></label>';
	}			
	?>
	<tr>
		<td bgcolor="#e7e7e7" width="15%" >8 - Documentos:</td>
		<td>
			<label><input <?=$disabled;?> type="radio" name="documentos" id="documentos_1" value="S" <?=(($analiseCadBasic['acbdocumento'] == 'S')? 'checked=\"checked\"' : '')?> />Atende</label>
			<label><input <?=$disabled;?> type="radio" name="documentos" id="documentos_2" value="N" <?=(($analiseCadBasic['acbdocumento'] == 'N')? 'checked=\"checked\"' : '')?> />Não atende</label>
			<label><input <?=$disabled;?> type="checkbox" name="documentos" id="documentos_3" value="TRUE" <?=(($analiseCadBasic['acbdocumentositaba'] == 't')? 'checked=\"checked\"' : '')?> />Bloquear Aba</label>
			<?=$possuiObsDocument; ?>
		</td>
	</tr>
	<tr id="trDocumentos" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea <?=$disabled;?> disabled="disabled" id="chkobsdocument" name="chkobsdocument" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ><?php echo $analiseCadBasic['acbdocumentoobs'];?></textarea></td>
	</tr>
	<tr>
		<td bgcolor="#e7e7e7" width="15%">Observação - Análise do Cadastro:</td>
		<td><textarea <?=$disabled;?> id="chkobsgeralcadbasic" name="chkobsgeralcadbasic" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ><?php echo $analiseCadBasic['acbobsgeral']; ?></textarea></td>
	</tr>
	<tr>
		<td bgcolor="#e7e7e7" width="30%" >Nível de satisfação da Supervisão</td>
		<td>
			<?php $sql = " SELECT nisid AS codigo, nisdsc AS descricao FROM	obras.nivelsatisfacao WHERE nisid = {$desc['nisid']} ORDER BY nisid"; ?> 
			<?php echo $db->monta_combo( 'nisid', $sql, 'N', '', '', '', '', 100 ,'', '', ''); ?>
		</td>
	</tr>
	<tr>
		<td colspan="2"><b>Situação do Parecer</b></td>
	</tr>
	<tr>
		<td bgcolor="#e7e7e7" width="15%" >Situação:</td>
		<td>
			<label><input <?=$disabled;?> type="radio" name="chksituacao" id="chksituacao_1" value="S" <?=(($desc['mpcsituacao'] == 't')? 'checked=\"checked\"' : '')?>/>Aprovado</label>
			<label><input <?=$disabled;?> type="radio" name="chksituacao" id="chksituacao_2" value="N" <?=(($desc['mpcsituacao'] == 'f')? 'checked=\"checked\"' : '')?>/>Não Aprovado</label>
		</td>
	</tr>
	<tr>
		<td></td>
	</tr>
	<tr>
		<td>
			<input type="button" value="Fechar" name="Fechar" onclick="javascript:window.close()">
		</td>
		<td>&nbsp;</td>
	</tr>
</table>

<?php
}

function parecerForm($chkid) {
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

$(document).ready(function() {
	//botão salvar descrição do parecer
	$("#salvarParecer").click(function () {
		var chkobsmec = $('#chkobsmec').val();
		var nisid = $('#nisid').val();
		var chkid = <?php echo $chkid; ?>;
		var documentacao_1 = $('#documentacao_1:checked').val();
		var documentacao_2 = $('#documentacao_2:checked').val();
		var chkobsdoc = $('#chkobsdoc').val();

		var instalacoes_1 = $('#instalacoes_1:checked').val();
		var instalacoes_2 = $('#instalacoes_2:checked').val();
		var chkobsinst = $('#chkobsinst').val();  
		
		var pessoal_1 = $('#pessoal_1:checked').val();
		var pessoal_2 = $('#pessoal_2:checked').val();
		var chkobspess = $('#chkobspess').val(); 
			
		var servicos_1 = $('#servicos_1:checked').val();
		var servicos_2 = $('#servicos_2:checked').val();
		var chkobsserv = $('#chkobsserv').val(); 
			
		var pagamentos_1 = $('#pagamentos_1:checked').val();
		var pagamentos_2 = $('#pagamentos_2:checked').val();
		var chkobspag = $('#chkobspag').val(); 

		var chkobsgeral = $('#chkobsgeral').val(); 

		var dadosobra_1 = $('#dadosobra_1:checked').val();
		var dadosobra_2 = $('#dadosobra_2:checked').val();
		var dadosobra_3 = $('#dadosobra_3:checked').val();
		var chkobsdadosobr = $('#chkobsdadosobr').val(); 

		var projetos_1 = $('#projetos_1:checked').val();
		var projetos_2 = $('#projetos_2:checked').val();
		var projetos_3 = $('#projetos_3:checked').val();
		var chkobsproj = $('#chkobsproj').val(); 

		var licitacao_1 = $('#licitacao_1:checked').val();
		var licitacao_2 = $('#licitacao_2:checked').val();
		var licitacao_3 = $('#licitacao_3:checked').val();
		var chkobslic = $('#chkobslic').val(); 
			
		var contratacao_1 = $('#contratacao_1:checked').val();
		var contratacao_2 = $('#contratacao_2:checked').val();
		var contratacao_3 = $('#contratacao_3:checked').val();
		var chkobscontr = $('#chkobscontr').val(); 

			
		var cronograma_1 = $('#cronograma_1:checked').val();
		var cronograma_2 = $('#cronograma_2:checked').val();
		var cronograma_3 = $('#cronograma_3:checked').val();
		var chkobscron = $('#chkobscron').val(); 
			
		var vistoria_1 = $('#vistoria_1:checked').val();
		var vistoria_2 = $('#vistoria_2:checked').val();
		var vistoria_3 = $('#vistoria_3:checked').val();
		var chkobsvist = $('#chkobsvist').val(); 
			
		var restricoes_1 = $('#restricoes_1:checked').val();
		var restricoes_2 = $('#restricoes_2:checked').val();
		var restricoes_3 = $('#restricoes_3:checked').val();
		var chkobsrestr = $('#chkobsrestr').val(); 
			
		var documentos_1 = $('#documentos_1:checked').val();
		var documentos_2 = $('#documentos_2:checked').val();
		var documentos_3 = $('#documentos_3:checked').val();
		var chkobsdocument = $('#chkobsdocument').val(); 

		var chksituacao_1 = $('#chksituacao_1:checked').val();
		var chksituacao_2 = $('#chksituacao_2:checked').val();

		var chkobsgeralcadbasic = $('#chkobsgeralcadbasic').val();
			
		if( documentacao_1 != null ){
			documentacao = 'S';
		}else if(documentacao_2 != null){
			documentacao = 'N';
		}else{
			documentacao = '';
		}
		
		if( instalacoes_1 != null ){
			instalacoes = 'S';
		}else if(instalacoes_2 != null){
			instalacoes = 'N';
		}else{
			instalacoes = '';
		}
		
		if( pessoal_1 != null ){
			pessoal = 'S';
		}else if(pessoal_2 != null){
			pessoal = 'N';
		}else{
			pessoal = '';
		}
		
		if( servicos_1 != null ){
			servicos = 'S';
		}else if(servicos_2 != null){
			servicos = 'N';
		}else{
			servicos = '';
		}
		
		if( pagamentos_1 != null ){
			pagamentos = 'S';
		}else if(pagamentos_2 != null){
			pagamentos = 'N';
		}else {
			pagamentos = '';
		}
		
		if( dadosobra_1 != null ){
			dadosobra = 'S';
		}else if(dadosobra_2 != null){
			dadosobra = 'N';
		}else{
			dadosobra = '';
		}
		if(dadosobra_3 != null){
			dadosobrachekc = 'TRUE';
		}else if(dadosobra_3 == null){
			dadosobrachekc = 'FALSE';
		}
		
		if( projetos_1 != null ){
			projetos = 'S';
		}else if(projetos_2 != null){
			projetos = 'N';
		}else{
			projetos = '';
		}
		if(projetos_3 != null){
			projetoschekc = 'TRUE';
		}else if(projetos_3 == null){
			projetoschekc = 'FALSE';
		}
		
		if( licitacao_1 != null ){
			licitacao = 'S';
		}else if(licitacao_2 != null){
			licitacao = 'N';
		}else{
			licitacao = '';
		}
		if(licitacao_3 != null){
			licitacaochekc = 'TRUE';
		}else if(licitacao_3 == null){
			licitacaochekc = 'FALSE';
		}
		
		if( contratacao_1 != null ){
			contratacao = 'S';
		}else if(contratacao_2 != null){
			contratacao = 'N';
		}else{
			contratacao = '';
		}
		if(contratacao_3 != null){
			contratacaochekc = 'TRUE';
		}else if(contratacao_3 == null){
			contratacaochekc = 'FALSE';
		}
		
		if( cronograma_1 != null ){
			cronograma = 'S';
		}else if(cronograma_2 != null){
			cronograma = 'N';
		}else{
			cronograma = '';
		}
		if(cronograma_3 != null){
			cronogramachekc = 'TRUE';
		}else if(cronograma_3 == null){
			cronogramachekc = 'FALSE';
		}
		
		if( vistoria_1 != null ){
			vistoria = 'S';
		}else if(vistoria_2 != null){
			vistoria = 'N';
		}else{
			vistoria = '';
		}
		if(vistoria_3 != null){
			vistoriachekc = 'TRUE';
		}else if(vistoria_3 == null){
			vistoriachekc = 'FALSE';
		}
		
		if( restricoes_1 != null ){
			restricoes = 'S';
		}else if(restricoes_2 != null){
			restricoes = 'N';
		}else{
			restricoes = '';
		}
		if(restricoes_3 != null){
			restricoeschekc = 'TRUE';
		}else if(restricoes_3 == null){
			restricoeschekc = 'FALSE';
		}
		
		if( documentos_1 != null ){
			documentos = 'S';
		}else if(documentos_2 != null){
			documentos = 'N';
		}else{
			documentos = '';
		}
		if(documentos_3 != null){
			documentoschekc = 'TRUE';
		}else if(documentos_3 == null){
			documentoschekc = 'FALSE';
		}
		
		if(chksituacao_1 != null ){
			chksituacao = 'TRUE';
		}else if(chksituacao_2 != null){
			chksituacao = 'FALSE';
		}else{
			chksituacao = '';
		}

		if(nisid == ''){
			alert('O Nível de satisfação da Supervisão é de preenchimento obrigatório!');
			return false;
		}	

<?php
			global $db;
			$dados = pegaOrdem();
			$sql = "SELECT
						DISTINCT MAX(hstid) as sequencia,
						to_char(MAX(wh.htddata), 'DD/MM/YYYY') as datramitacao
					FROM
						obras.checklistvistoria ch 
					INNER JOIN 
						obras.obrainfraestrutura ob ON ob.obrid = ch.obrid 
					INNER JOIN
						workflow.documento wd ON wd.docid = ob.docid
					INNER JOIN
						workflow.historicodocumento wh ON wh.docid = ob.docid
					INNER JOIN
						workflow.estadodocumento we ON we.esdid = wd.esdid
					WHERE
						obsstatus = 'A' 
						AND ch.chkid  = ".$chkid;
			/*
			$sql = "SELECT
						DISTINCT MAX(hstid) as sequencia,
						to_char(MAX(wh.htddata), 'DD/MM/YYYY') as datramitacao
					FROM
						obras.grupodistribuicao gd
					INNER JOIN
						workflow.documento wd ON wd.docid = gd.docid
					INNER JOIN
						workflow.historicodocumento wh ON wh.docid = gd.docid
					INNER JOIN
						workflow.estadodocumento we ON we.esdid = wd.esdid
					WHERE
						gpdstatus = 'A' 
						AND gd.gpdid  = ".$dados[0]['gpdid'];
			*/
			$dados = $db->pegaLinha($sql);
		?>
	
		// sequencia
		var mpcseqtramitacao = <?php echo (($dados['sequencia'])?$dados['sequencia']:"null"); ?>;
		
		$.ajax({
	   		type: "POST",
	   		url: "obras.php?modulo=principal/supervisao/check_list_visita&acao=A",
	   		data: "chkobsmec="+chkobsmec+"&nisid="+nisid+"&chkid="+chkid+"&mpcseqtramitacao="+mpcseqtramitacao+"&documentacao="+documentacao+"&chkobsdoc="+chkobsdoc+"&instalacoes="+instalacoes+"&chkobsinst="+chkobsinst+"&pessoal="+pessoal+"&chkobspess="+chkobspess+"&servicos="+servicos+"&chkobsserv="+chkobsserv+"&pagamentos="+pagamentos+"&chkobspag="+chkobspag+"&dadosobra="+dadosobra+"&dadosobrachekc="+dadosobrachekc+"&chkobsdadosobr="+chkobsdadosobr+"&projetos="+projetos+"&projetoschekc="+projetoschekc+"&licitacao="+licitacao+"&licitacaochekc="+licitacaochekc+"&contratacao="+contratacao+"&contratacaochekc="+contratacaochekc+"&cronograma="+cronograma+"&cronogramachekc="+cronogramachekc+"&vistoria="+vistoria+"&vistoriachekc="+vistoriachekc+"&restricoes="+restricoes+"&restricoeschekc="+restricoeschekc+"&documentos="+documentos+"&documentoschekc="+documentoschekc+"&chkobsproj="+chkobsproj+"&chkobslic="+chkobslic+"&chkobscontr="+chkobscontr+"&chkobscron="+chkobscron+"&chkobsvist="+chkobsvist+"&chkobsrestr="+chkobsrestr+"&chkobsdocument="+chkobsdocument+"&chkobsgeral="+chkobsgeral+"&chksituacao="+chksituacao+"&chkobsgeralcadbasic="+chkobsgeralcadbasic,
	   		async: false,
	   		success: function(data){alert(data);}
	 		});
		window.opener.location=window.opener.location;
		window.close();
		
	})

	$('#dadosobra_1').click(function()
	{
		$('#dadosobra_3').attr('checked', true);
	});
	$('#dadosobra_2').click(function()
	{
		$('#dadosobra_3').attr('checked', false);
	});
	
	$('#projetos_1').click(function()
	{
		$('#projetos_3').attr('checked', true);
	});
	$('#projetos_2').click(function()
	{
		$('#projetos_3').attr('checked', false);
	});
	
	$('#licitacao_1').click(function()
	{
		$('#licitacao_3').attr('checked', true);
	});
	$('#licitacao_2').click(function()
	{
		$('#licitacao_3').attr('checked', false);
	});
	
	$('#contratacao_1').click(function()
	{
		$('#contratacao_3').attr('checked', true);
	});
	$('#contratacao_2').click(function()
	{
		$('#contratacao_3').attr('checked', false);
	});
	
	$('#cronograma_1').click(function()
	{
		$('#cronograma_3').attr('checked', true);
	});
	
	$('#cronograma_2').click(function()
	{
		$('#cronograma_3').attr('checked', false);
	});
	
	$('#vistoria_1').click(function()
	{
		$('#vistoria_3').attr('checked', true);
	});
	
	$('#vistoria_2').click(function()
	{
		$('#vistoria_3').attr('checked', false);
	});
	
	$('#restricoes_1').click(function()
	{
		$('#restricoes_3').attr('checked', true);
	});
	$('#restricoes_2').click(function()
	{
		$('#restricoes_3').attr('checked', false);
	});
	
	$('#documentos_1').click(function()
	{
		$('#documentos_3').attr('checked', true);
	});
	$('#documentos_2').click(function()
	{
		$('#documentos_3').attr('checked', false);
	});

});

function abreObservacoes( observacao ){

	switch( observacao ){

		case "documentacao":
			 if( $('#trDocumentacao').css('display') == 'none' ){
				$('#trDocumentacao').show();
			}else{
				$('#trDocumentacao').hide();
			}
		break;

		case "instalacoes":
			 if( $('#trInstalacoes').css('display') == 'none' ){
				$('#trInstalacoes').show();
			}else{
				$('#trInstalacoes').hide();
			}
		break;

		case "pessoal":
			 if( $('#trPessoal').css('display') == 'none' ){
				$('#trPessoal').show();
			}else{
				$('#trPessoal').hide();
			}
		break;
		
		case "servicos":
			 if( $('#trServicos').css('display') == 'none' ){
				$('#trServicos').show();
			}else{
				$('#trServicos').hide();
			}
		break;

		case "pagamentos":
			 if( $('#trPagamentos').css('display') == 'none' ){
				$('#trPagamentos').show();
			}else{
				$('#trPagamentos').hide();
			}
		break;

		case "dadosobra":
			 if( $('#trDadosObra').css('display') == 'none' ){
				$('#trDadosObra').show();
			}else{
				$('#trDadosObra').hide();
			}
		break;
		
		case "projetos":
			 if( $('#trProjetos').css('display') == 'none' ){
				$('#trProjetos').show();
			}else{
				$('#trProjetos').hide();
			}
		break;
		
		case "licitacao":
			 if( $('#trLicitacao').css('display') == 'none' ){
				$('#trLicitacao').show();
			}else{
				$('#trLicitacao').hide();
			}
		break;
		
		case "contratacao":
			 if( $('#trContratacao').css('display') == 'none' ){
				$('#trContratacao').show();
			}else{
				$('#trContratacao').hide();
			}
		break;
		
		case "cronograma":
			 if( $('#trCronograma').css('display') == 'none' ){
				$('#trCronograma').show();
			}else{
				$('#trCronograma').hide();
			}
		break;
		
		case "vistoria":
			 if( $('#trVistoria').css('display') == 'none' ){
				$('#trVistoria').show();
			}else{
				$('#trVistoria').hide();
			}
		break;
		
		case "restricoes":
			 if( $('#trRestricoes').css('display') == 'none' ){
				$('#trRestricoes').show();
			}else{
				$('#trRestricoes').hide();
			}
		break;
		
		case "documentos":
			 if( $('#trDocumentos').css('display') == 'none' ){
				$('#trDocumentos').show();
			}else{
				$('#trDocumentos').hide();
			}
				
	}
	
}

</script>
<form id="formulario">
<table border="0" width="100%" cellspacing="0" cellpadding="3" bgcolor="#DCDCDC" style="border-top: none; border-bottom: none;">
	<tr>
		<td width="100%" align="center" ><label class="TituloTela" style="color:#000000;"> Parecer Checklist</label></td>
	</tr>
	<tr>
		<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /><b> Indica os campos obrigatórios</b></td>
	</tr>
</table>
<table border="0" width="100%">
	<tr>
		<td bgcolor="#e7e7e7" width="15%">Parecer da Supervisão(MEC)</td>
		<td><?php  echo campo_textarea( 'chkobsmec', 'N', 'S', '', '90', '10', '5000', '' , 0, '', false, NULL, $chkobsmec); ?></td>
	</tr>
	<tr>
	<td colspan="2"><b>Análise do Checklist de Visita Técnica</b></td>
	</tr>
	<tr>
		<td bgcolor="#e7e7e7" width="15%" >1 - Quanto à Documentação:</td>
		<td>
			<label><input type="radio" name="documentacao" id="documentacao_1" value="S" />Atende</label>
			<label><input type="radio" name="documentacao" id="documentacao_2" value="N"/>Não atende</label>
			<label>&nbsp;&nbsp;<img src="/imagens/editar_nome.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes('documentacao');" title="Inserir Observação"> Observação</label>
		</td>
	</tr>
	<tr id="trDocumentacao" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea  id="chkobsdoc" name="chkobsdoc" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ></textarea></td>
	</tr>
	<tr>	
		<td bgcolor="#e7e7e7" width="15%" >2 - Quanto às Instalações do Canteiro de Obras:</td>
		<td>
			<label><input type="radio" name="instalacoes" id="instalacoes_1" value="S" />Atende</label>
			<label><input type="radio" name="instalacoes" id="instalacoes_2" value="N"/>Não atende</label>
			<label>&nbsp;&nbsp;<img src="/imagens/editar_nome.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes('instalacoes');" title="Inserir Observação"> Observação</label>
		</td>
	</tr>
	<tr id="trInstalacoes" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea  id="chkobsinst" name="chkobsinst" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ></textarea></td>
	</tr>
	<tr>	
		<td bgcolor="#e7e7e7" width="15%" >3 - Quanto ao Pessoal:</td>
		<td>
			<label><input type="radio" name="pessoal" id="pessoal_1" value="S" />Atende</label>
			<label><input type="radio" name="pessoal" id="pessoal_2" value="N"/>Não atende</label>
			<label>&nbsp;&nbsp;<img src="/imagens/editar_nome.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes('pessoal');" title="Inserir Observação"> Observação</label>
		</td>
	</tr>
	<tr id="trPessoal" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea  id="chkobspess" name="chkobspess" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ></textarea></td>
	</tr>
	<tr>	
		<td bgcolor="#e7e7e7" width="15%" >4 - Quanto aos Serviços:</td>
		<td>
			<label><input type="radio" name="servicos" id="servicos_1" value="S" />Atende</label>
			<label><input type="radio" name="servicos" id="servicos_2" value="N"/>Não atende</label>
			<label>&nbsp;&nbsp;<img src="/imagens/editar_nome.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes('servicos');" title="Inserir Observação"> Observação</label>
		</td>
	</tr>
	<tr id="trServicos" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea  id="chkobsserv" name="chkobsserv" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ></textarea></td>
	</tr>
	<tr>	
		<td bgcolor="#e7e7e7" width="15%" >5 - Quanto aos Pagamentos efetuados:</td>
		<td>
			<label><input type="radio" name="pagamentos" id="pagamentos_1" value="S" />Atende</label>
			<label><input type="radio" name="pagamentos" id="pagamentos_2" value="N"/>Não atende</label>
			<label>&nbsp;&nbsp;<img src="/imagens/editar_nome.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes('pagamentos');" title="Inserir Observação"> Observação</label>
		</td>
	</tr>
	<tr id="trPagamentos" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea  id="chkobspag" name="chkobspag" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ></textarea></td>
	</tr>
	<tr>
		<td bgcolor="#e7e7e7" width="15%">Observação - Análise do Checklist de Visita Técnica:</td>
		<td><?php  echo campo_textarea( 'chkobsgeral', 'N', 'S', '', '90', '10', '5000', '' , 0, '', false, NULL, $chkobsgeral); ?></td>
	</tr>
	<tr>
		<td colspan="2"><b>Análise do Cadastro</b></td>
	</tr>
	<tr>
		<td bgcolor="#e7e7e7" width="15%" >1 - Dados da Obra:</td>
		<td>
			<label><input type="radio" name="dadosobra" id="dadosobra_1" value="S"/>Atende</label>

			<label><input type="radio" name="dadosobra" id="dadosobra_2" value="N"/>Não atende</label>
			<label><input type="checkbox" name="dadosobra" id="dadosobra_3" value="TRUE"/>Bloquear Aba</label>
			<label>&nbsp;&nbsp;<img src="/imagens/editar_nome.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes('dadosobra');" title="Inserir Observação"> Observação</label>
		</td>
	</tr>
	<tr id="trDadosObra" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea  id="chkobsdadosobr" name="chkobsdadosobr" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ></textarea></td>
	</tr>
	<tr>
		<td bgcolor="#e7e7e7" width="15%" >2 - Projetos:</td>
		<td>
			<label><input type="radio" name="projetos" id="projetos_1" value="S"/>Atende</label>
			<label><input type="radio" name="projetos" id="projetos_2" value="N"/>Não atende</label>
			<label><input type="checkbox" name="projetos" id="projetos_3" value="TRUE"/>Bloquear Aba</label>
			<label>&nbsp;&nbsp;<img src="/imagens/editar_nome.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes('projetos');" title="Inserir Observação"> Observação</label>
		</td>
	</tr>
	<tr id="trProjetos" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea  id="chkobsproj" name="chkobsproj" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ></textarea></td>
	</tr>
	<tr>
		<td bgcolor="#e7e7e7" width="15%" >3 - Licitação:</td>
		<td>
			<label><input type="radio" name="licitacao" id="licitacao_1" value="S"/>Atende</label>
			<label><input type="radio" name="licitacao" id="licitacao_2" value="N"/>Não atende</label>
			<label><input type="checkbox" name="licitacao" id="licitacao_3" value="TRUE"/>Bloquear Aba</label>
			<label>&nbsp;&nbsp;<img src="/imagens/editar_nome.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes('licitacao');" title="Inserir Observação"> Observação</label>
		</td>
	</tr>
	<tr id="trLicitacao" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea  id="chkobslic" name="chkobslic" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ></textarea></td>
	</tr>
	<tr>
		<td bgcolor="#e7e7e7" width="15%" >4 - Contratação:</td>
		<td>
			<label><input type="radio" name="contratacao" id="contratacao_1" value="S"/>Atende</label>
			<label><input type="radio" name="contratacao" id="contratacao_2" value="N"/>Não atende</label>
			<label><input type="checkbox" name="contratacao" id="contratacao_3" value="TRUE"/>Bloquear Aba</label>
			<label>&nbsp;&nbsp;<img src="/imagens/editar_nome.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes('contratacao');" title="Inserir Observação"> Observação</label>
		</td>
	</tr>
	<tr id="trContratacao" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea  id="chkobscontr" name="chkobscontr" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ></textarea></td>
	</tr>
	<tr>
		<td bgcolor="#e7e7e7" width="15%" >5 - Cronograma F/F:</td>
		<td>
			<label><input type="radio" name="cronograma" id="cronograma_1" value="S"/>Atende</label>
			<label><input type="radio" name="cronograma" id="cronograma_2" value="N"/>Não atende</label>
			<label><input type="checkbox" name="cronograma" id="cronograma_3" value="TRUE"/>Bloquear Aba</label>
			<label>&nbsp;&nbsp;<img src="/imagens/editar_nome.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes('cronograma');" title="Inserir Observação"> Observação</label>
		</td>
	</tr>
	<tr id="trCronograma" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea  id="chkobscron" name="chkobscron" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ></textarea></td>
	</tr>
	<tr>
		<td bgcolor="#e7e7e7" width="15%" >6 - Vistoria:</td>
		<td>
			<label><input type="radio" name="vistoria" id="vistoria_1" value="S"/>Atende</label>
			<label><input type="radio" name="vistoria" id="vistoria_2" value="N"/>Não atende</label>
			<label><input type="checkbox" name="vistoria" id="vistoria_3" value="TRUE"/>Bloquear Aba</label>
			<label>&nbsp;&nbsp;<img src="/imagens/editar_nome.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes('vistoria');" title="Inserir Observação"> Observação</label>
		</td>
	</tr>
	<tr id="trVistoria" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea  id="chkobsvist" name="chkobsvist" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ></textarea></td>
	</tr>
	<tr>
		<td bgcolor="#e7e7e7" width="15%" >7 - Restrições e Providências:</td>
		<td>
			<label><input type="radio" name="restricoes" id="restricoes_1" value="S"/>Atende</label>
			<label><input type="radio" name="restricoes" id="restricoes_2" value="N"/>Não atende</label>
			<label><input type="checkbox" name="restricoes" id="restricoes_3" value="TRUE"/>Bloquear Aba</label>
			<label>&nbsp;&nbsp;<img src="/imagens/editar_nome.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes('restricoes');" title="Inserir Observação"> Observação</label>
		</td>
	</tr>
	<tr id="trRestricoes" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea  id="chkobsrestr" name="chkobsrestr" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ></textarea></td>
	</tr>
	<tr>
		<td bgcolor="#e7e7e7" width="15%" >8 - Documentos:</td>
		<td>
			<label><input type="radio" name="documentos" id="documentos_1" value="S" />Atende</label>
			<label><input type="radio" name="documentos" id="documentos_2" value="N"/>Não atende</label>
			<label><input type="checkbox" name="documentos" id="documentos_3" value="TRUE"/>Bloquear Aba</label>
			<label>&nbsp;&nbsp;<img src="/imagens/editar_nome.gif" style="cursor:pointer;"  id=""  onclick="abreObservacoes('documentos');" title="Inserir Observação"> Observação</label>
		</td>
	</tr>
	<tr id="trDocumentos" style="display: none;">
		<td bgcolor="#e7e7e7" width="15%">Observação</td>
		<td><textarea  id="chkobsdocument" name="chkobsdocument" cols="90" rows="10"   onmouseover="MouseOver( this );" onfocus="MouseClick( this );"  onmouseout="MouseOut( this );"  onblur="MouseBlur( this );"  style="width:90ex;"  class="txareanormal" ></textarea></td>
	</tr>
	<tr>
		<td bgcolor="#e7e7e7" width="15%">Observação - Análise do Cadastro:</td>
		<td><?php  echo campo_textarea( 'chkobsgeralcadbasic', 'N', 'S', '', '90', '10', '5000', '' , 0, '', false, NULL, $chkobsgeralcadbasic); ?></td>
	</tr>
	<tr>
		<td bgcolor="#e7e7e7" width="30%" >Nível de satisfação da Supervisão</td>
		<td>
			<?php $sql = " SELECT nisid AS codigo, nisdsc AS descricao FROM	obras.nivelsatisfacao ORDER BY nisid"; ?>
			<?php echo $db->monta_combo( 'nisid', $sql, 'S', 'Selecione...', '', '', '', 100 ,'S', 'nisid'); ?>
		</td>
	</tr>
	<tr>
		<td colspan="2"><b>Situação do Parecer</b></td>
	</tr>
	<tr>
		<td bgcolor="#e7e7e7" width="15%" >Situação:</td>
		<td>
			<label><input type="radio" name="chksituacao" id="chksituacao_1" value="S" />Aprovado</label>
			<label><input type="radio" name="chksituacao" id="chksituacao_2" value="N" />Não Aprovado</label>
		</td>
	</tr>
	<tr>
		<td></td>
	</tr>
	<tr>
		<td>
			<input type="button" value="Salvar Parecer" id="salvarParecer" />
		</td>
		<td>&nbsp;</td>
	</tr>
</table>
</form>
<?php	
}

function cabecalho_tabela($chkid = 0){
	
	global $db;
	$sql = "SELECT
			cv.chkid as sequencia,
			to_char(cv.chkdtinclusao, 'DD/MM/YYYY') as datachk,
			u.usunome as nome,
			cv.orsid as ordem,
			to_char(os.orsdtemissao, 'DD/MM/YYYY') as dataos,
			gd.gpdid as grupo,
			( SELECT entnome FROM entidade.entidade WHERE entid = cv.entidresptecnico ) as resptecnico,
			( SELECT entnome FROM entidade.entidade WHERE entid = cv.entidrespvistoria ) as respvistoria
			
		FROM
			obras.checklistvistoria cv
		INNER JOIN obras.ordemservico os ON os.orsid = cv.orsid
		INNER JOIN obras.grupodistribuicao gd ON gd.gpdid = os.gpdid
		INNER JOIN seguranca.usuario u ON u.usucpf = cv.usucpf
					       AND cv.obrid = {$_SESSION["obra"]['obrid']}
					       AND chkstatus = 'A'
					       AND cv.chkid = {$chkid}";
	
	$dados = $db->carregar($sql);
	$REQUEST['chkid'] = $dados[0]['sequencia'];
	$cabecalho = '<tr bgcolor="#e7e7e7">
						<td><b>Sequência</b></td>
						<td><b>Data de Criação</b></td>
						<td><b>Inserido por</b></td>
						<td><b>Nº da O.S</b></td>
						<td><b>Data da O.S</b></td>
						<td><b>Nº do Grupo</b></td>
					</tr>
					<tr>
						<td>'.$dados[0]['sequencia'].'</td>
						<td>'.$dados[0]['datachk'].'</td>
						<td>'.$dados[0]['nome'].'</td>
						<td>'.$dados[0]['ordem'].'</td>
						<td>'.$dados[0]['dataos'].'</td>
						<td>'.$dados[0]['grupo'].'</td>
					</tr>
					<tr bgcolor="#e7e7e7">
						<td colspan="3" width="50%"><b>Responsável pela Vistoria</b></td>
						<td colspan="3" width="50%"><b>Responsável Técnico pela obra</b></td>
					</tr>
					<tr>
						<td colspan="3" width="50%">'.$dados[0]['respvistoria'].'</td>
						<td colspan="3" width="50%">'.$dados[0]['resptecnico'].'</td>
					</tr>';
	
	return $cabecalho;	
	
}


if($obrid){ 
		$gpdid = buscaGrupoPelaObra( $obrid );
		tramitaGrupo($gpdid); 
}

?>
