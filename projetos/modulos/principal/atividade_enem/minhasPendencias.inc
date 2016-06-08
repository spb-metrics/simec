<?php

$conf = array(ENEM_EST_EM_VALIDACAO 	=> array('tpvid' => '2', 'descricao' => 'validar', 'vldsituacao_TRUE' => ENEM_AEDID_VALIDAR, 'vldsituacao_FALSE' => ENEM_AEDID_INVALIDAR, 'vldsituacao_FINALIZAR' => ENEM_AEDID_VLFINALIZAR),
			  ENEM_EST_EM_EXECUCAO  	=> array('tpvid' => '1', 'descricao' => 'executar','vldsituacao_TRUE' => ENEM_AEDID_EXECUTAR, 'vldsituacao_FALSE' => false, 'vldsituacao_FINALIZAR' => ENEM_AEDID_EXFINALIZAR),
			  ENEM_EST_EM_CERTIFICACAO  => array('tpvid' => '3', 'descricao' => 'certificar','vldsituacao_TRUE' => ENEM_AEDID_CERTIFICAR, 'vldsituacao_FALSE' => ENEM_AEDID_NAOCERTIFICAR));


			  
			  

function downloadArquivo($dados) {
	global $db;
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec("anexochecklist", $arrCampos = array(), "projetos");
	$file->getDownloadArquivo($dados['arqid']);
}
			  
function salvarItem($dados) {
	global $db, $conf;
	
	include_once APPRAIZ . 'includes/workflow.php';
	
	$entidUsuario = $db->pegaUm("SELECT entid FROM projetos.usuarioresponsabilidade WHERE rpustatus = 'A' AND usucpf = '".$_SESSION['usucpf']."' AND entid is not null");
	if( !$entidUsuario ) {
		$entidUsuario = $db->pegaUm("SELECT entid FROM entidade.entidade WHERE entnumcpfcnpj = '".$_SESSION['usucpf']."' AND entstatus = 'A'");
	}
	
	$arrwf = $db->pegaLinha("SELECT d.esdid, d.docid FROM projetos.itemchecklist i 
						     INNER JOIN workflow.documento d ON i.docid = d.docid 
						     WHERE iclid='".$dados['iclid']."'");
				  
	$sql = "SELECT vldid FROM projetos.validacao 
			WHERE entid='".$entidUsuario."' AND tpvid='".$conf[$arrwf['esdid']]['tpvid']."' AND iclid='".$dados['iclid']."'";
	
	$vldid = $db->pegaUm($sql);
	
	if($dados['vldsituacao'] == "TRUE") {
		$tpviddestino = $conf[$arrwf['esdid']]['tpvid']+1;
	} else {
		$tpviddestino = $conf[$arrwf['esdid']]['tpvid']-1;
	}
	
	$sql = "SELECT usu.usuemail, usu.usunome FROM projetos.checklistentidade cle 
			INNER JOIN entidade.entidade ent ON ent.entid = cle.entid 
			LEFT JOIN seguranca.usuario usu ON usu.usucpf = ent.entnumcpfcnpj 
			WHERE cle.iclid = ".$dados['iclid']." AND cle.tpvid = ".$tpviddestino;
	
	$dadosusu = $db->pegaLinha($sql);
	
	if($vldid) {
		
		$db->executar("UPDATE projetos.validacao 
					   SET vldsituacao = ".$dados['vldsituacao'].", vlddata = now(), vldobservacao = '".pg_escape_string($dados['vldobservacao'])."'
					   WHERE iclid = ".$dados['iclid']." AND tpvid = ".$conf[$arrwf['esdid']]['tpvid']." AND entid = ".$entidUsuario);
		
	} else {
		
		$vldid = $db->pegaUm("INSERT INTO projetos.validacao(
					            iclid, tpvid, entid, vldsituacao, vlddata, vldobservacao)
    						  VALUES ('".$dados['iclid']."', '".$conf[$arrwf['esdid']]['tpvid']."', '".$entidUsuario."', ".$dados['vldsituacao'].", now(), '".pg_escape_string($dados['vldobservacao'])."') RETURNING vldid;");
		
	}
	
	$db->commit();
	
	if( $_FILES['arquivo']['name'] ) {
		
		if( $vldid ) {
			
			$sql = "SELECT ancid FROM projetos.anexochecklist WHERE vldid = ".$vldid." AND ancstatus = 'A'";
			$ancid = $db->pegaUm($sql);
			
			if( $ancid ) {
				$sql = "UPDATE projetos.anexochecklist SET ancstatus = 'I' WHERE ancid = ".$ancid;
				$db->executar($sql);
				$db->commit();
			}
			include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
			$arrCampos = array("vldid" => $vldid);
			$file = new FilesSimec("anexochecklist", $arrCampos, "projetos");
				
			$arqdescricao = 'arquivo_checklist_enem_'.$vldid.'_'.date('Ymdhis');
			$file->setUpload($arqdescricao, "arquivo");
				
		}
	}
	
	$atiid = $db->pegaUm("SELECT atiid FROM projetos.itemchecklist WHERE iclid='".$dados['iclid']."'");
	
	// codigo especifico para atender a regra de finalização
	if($dados['vldsituacao'] == "TRUE" && ($conf[$arrwf['esdid']]['tpvid']+1) < 4) {
		$sql = "SELECT etcid FROM projetos.etapascontrole WHERE iclid='".$dados['iclid']."' AND tpvid='".($conf[$arrwf['esdid']]['tpvid']+1)."'";
		$etcid = $db->pegaUm($sql);
		if(!$etcid) $dados['vldsituacao'] = "FINALIZAR";
		
	}
	
	
	$aedid = $conf[$arrwf['esdid']]['vldsituacao_'.$dados['vldsituacao']];
			
	if( $aedid ) {
		if($dados['vldsituacao'] == "FALSE") {
			$comentario = $dados['vldobservacao'];
		}
		wf_alterarEstado( $arrwf['docid'], $aedid, $comentario, $dados );
	}
	
	// verificando se todos os item estão finalizados
	// verificando se existir algum item que tenha sido tramitado do estado inicial, atualizar andamento da atividade para 10%
	if($atiid) {
		
		$sql = "SELECT d.esdid FROM projetos.itemchecklist i
				INNER JOIN workflow.documento d ON d.docid = i.docid 
				WHERE atiid = '".$atiid."'";
		
		$estados = $db->carregarColuna($sql);
		
		$todositensfinalizados = true;
		$itensemandamento = 0;
		if($estados) {
			foreach($estados as $estado) {
				if($estado != ENEM_EST_EM_EXECUCAO) $itensemandamento++;
				if($estado != ENEM_EST_EM_FINALIZADO) $todositensfinalizados = false;
			}
			if($todositensfinalizados) $db->executar("UPDATE projetos.atividade SET esaid=5, atiporcentoexec='100',atidataconclusao=NOW() WHERE atiid='".$atiid."'");
			elseif($itensemandamento > 0) $db->executar("UPDATE projetos.atividade SET esaid=2, atiporcentoexec='10' WHERE atiid='".$atiid."'");
		}
	}
	
	$db->commit();
	
	
	
	if( $dadosusu['usuemail'] ) {
		
		// Email para o validador ou certificador informando que a fase anterior do checklist foi concluída:
		if(($tpviddestino == '2' || $tpviddestino == '3') && $dados['vldsituacao'] == 'TRUE') {
			
			$html .= "<p>Prezado (a) ".$dadosusu['usunome'].",</p>";
	 		$html .= "<p>Informamos que o item do checklist abaixo foi concluído e está pendente de sua validação no módulo ENEM do sistema SIMEC:</p>";
			$html .= "<p>";
			$html .= "<table>";
			$html .= "<tr><td>Item</td><td>Descrição</td><td>Prazo</td></tr>";
			
			$sql = "SELECT * FROM projetos.itemchecklist WHERE iclid='".$dados['iclid']."'";
			$itemchecklist = $db->pegaLinha($sql);
			
			$html .= "<tr><td>".$itemchecklist['iclid']."</td><td>".$itemchecklist['icldsc']."</td><td>".(($itemchecklist['iclprazo']>=date("Y-m-d"))?$itemchecklist['iclprazo']:"<font color=red>".$itemchecklist['iclprazo']."</font>")."</td></tr>";
			$html .= "</table>"; 
			$html .= "</p>";
			$html .= "<p>* Os itens cujo o prazo está em vermelho estão em atraso, necessitando de ação urgente.</p>";
			$html .= "<p>Para concluí-los, acesse o módulo ENEM no endereço <a href=http://simec.mec.gov.br>http://simec.mec.gov.br</a>, no menu \"Principal > Minhas Pendências\", e clique no ícone <img align=absmiddle src=../../imagens/valida2.gif></p>";
		}
		
			// Email para o executor, validador informando que o item foi invalidado
		if(($tpviddestino == '1' || $tpviddestino == '2') && $dados['vldsituacao'] == 'FALSE') {
			
			$html .= "<p>Prezado (a) ".$dadosusu['usunome'].",</p>";
	 		$html .= "<p>Informamos que o item do checklist abaixo foi invalidado.<br/>Favor acessar o módulo ENEM do sistema SIMEC para realizar nova execução:</p>";
			$html .= "<p>";
			$html .= "<table>";
			$html .= "<tr><td>Item</td><td>Descrição</td><td>Prazo</td><td>Justificativa</td></tr>";
			
			$sql = "SELECT * FROM projetos.itemchecklist WHERE iclid='".$dados['iclid']."'";
			$itemchecklist = $db->pegaLinha($sql);
			
			$html .= "<tr><td>".$itemchecklist['iclid']."</td><td>".$itemchecklist['icldsc']."</td><td>".(($itemchecklist['iclprazo']>=date("Y-m-d"))?$itemchecklist['iclprazo']:"<font color=red>".$itemchecklist['iclprazo']."</font>")."</td><td>".pg_escape_string($dados['vldobservacao'])."</td></tr>";
			$html .= "</table>"; 
			$html .= "</p>";
			$html .= "<p>* Os itens cujo o prazo está em vermelho estão em atraso, necessitando de ação urgente.</p>";
			$html .= "<p>Para concluí-los, acesse o módulo ENEM no endereço <a href=http://simec.mec.gov.br>http://simec.mec.gov.br</a>, no menu \"Principal > Minhas Pendências\", e clique no ícone <img align=absmiddle src=../../imagens/valida2.gif></p>";
		}

		$arrEmail 	= array($usuemail);
		$titulo		= "SIMEC - ENEM - Aviso de validação/invalidação de item de checklist";
				
		require_once(APPRAIZ . 'includes/classes/EmailAgendado.class.inc');
				
		if($html) {
			$e = new EmailAgendado();
			$e->setTitle($titulo);
			$e->setText($html);
			$e->setName("SIMEC");
			$e->setEmailOrigem("simec@mec.gov.br");
			$e->setEmailsDestino($arrEmail);
			$e->enviarEmails();
		}

	}

	die("<script>
			alert('Dados registrados com sucesso');
			window.location='enem.php?modulo=principal/atividade_enem/minhasPendencias&acao=A';
		 </script>");
	
}


function telaFluxoEnem($dados) {
	global $db, $conf;
	
	$sql = "SELECT atv._atinumero, doc.esdid, atv.atidescricao, icl.icldsc, to_char(icl.iclprazo, 'dd/mm/YYYY') as iclprazo, icl.iclid FROM projetos.itemchecklist icl
			INNER JOIN workflow.documento doc ON doc.docid = icl.docid 
			INNER JOIN projetos.atividade atv ON icl.atiid = atv.atiid 
			WHERE icl.docid = '".$dados['docid']."'";
	
	
	$arrItem = $db->pegaLinha($sql);
	
	echo "<form method=post id=formulario enctype=multipart/form-data>";
	echo "<input type=hidden name=requisicao value=salvarItem>";
	echo "<input type=hidden name=iclid value=".$arrItem['iclid'].">";
	echo "<table class=tabela width=100% cellspacing=2 cellpadding=3 align=center>";
	
	echo "<tr>";
	echo "<td class=SubTituloDireita width=30%>Atividade:</td>";
	echo "<td>".$arrItem['_atinumero']." - ".$arrItem['atidescricao']."</td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td class=SubTituloDireita width=30%>Item:</td>";
	echo "<td>".$arrItem['iclid']." - ".$arrItem['icldsc']."</td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td class=SubTituloDireita width=30%>Prazo:</td>";
	echo "<td>".$arrItem['iclprazo']."</td>";
	echo "</tr>";
	
	
	echo "<tr>";
	echo "<td class=SubTituloDireita width=30%>Deseja ".$conf[$arrItem['esdid']]['descricao']." este item?</td>";
	echo "<td><input type=radio name=vldsituacao id=vldsituacao_TRUE value=TRUE checked> Sim <input type=radio name=vldsituacao id=vldsituacao_FALSE value=FALSE> Não</td>";	
	echo "</tr>";
	
	$arrEtcopcaoevidencia = $db->pegaLinha("SELECT etcopcaoevidencia, etcevidencia FROM projetos.etapascontrole WHERE iclid='".$arrItem['iclid']."' AND tpvid = '".$conf[$arrItem['esdid']]['tpvid']."'");
	
	echo "<tr>";
	echo "<td class=SubTituloDireita width=30%>".(($arrEtcopcaoevidencia['etcevidencia'])?$arrEtcopcaoevidencia['etcevidencia']:"Anexo").":</td>";
	echo "<td><input type=file id=arquivo name=arquivo />".(($arrEtcopcaoevidencia['etcopcaoevidencia']=='t')?"<input type=hidden name=etcopcaoevidencia id=etcopcaoevidencia value=sim>":"")."</td>";
	echo "</tr>";
	
	$sql = "SELECT to_char(v.vlddata,'dd/mm/YYYY HH24:MI') as vlddata, v.vldobservacao, t.tpvdsc, ar.arqid, usu.usunome FROM projetos.validacao v 
			LEFT JOIN projetos.anexochecklist a ON a.vldid = v.vldid 
			LEFT JOIN public.arquivo ar ON ar.arqid = a.arqid 
			LEFT JOIN seguranca.usuario usu ON usu.usucpf = ar.usucpf 
			LEFT JOIN projetos.tipovalidacao t ON t.tpvid = v.tpvid 
			WHERE v.iclid='".$arrItem['iclid']."' AND a.ancstatus = 'A'";
	
	$validacao = $db->carregar($sql);
	
	if($validacao[0]) {
	
		echo "<tr>";
		echo "<td colspan=2><table class=listagem width=100%>";
		echo "<thead>";
		echo "<tr><td align=center><b>Data</b></td><td align=center><b>Usuário</b></td><td align=center><b>Observação</b></td><td align=center><b>Fase</b></td><td align=center><b>Download</b></td></tr>";
		echo "</thead>";
					
		foreach($validacao as $val) {
			echo "<tr><td>".$val['vlddata']."</td><td>".$val['usunome']."</td><td>".$val['vldobservacao']."</td><td>".$val['tpvdsc']."</td><td align=center><img src=../imagens/salvar.png align=absmiddle border=0 style=cursor:pointer; onclick=\"window.location='enem.php?modulo=principal/atividade_enem/minhasPendencias&acao=A&requisicao=downloadArquivo&arqid=".$val['arqid']."';\"></td></tr>";
		}
			
		echo "</table></td>";
		echo "</tr>";
		
		
	}
	
	echo "<tr>";
	echo "<td class=SubTituloDireita width=30%>Observação:</td>";
	echo "<td><textarea name=vldobservacao id=vldobservacao cols=50 rows=7></textarea></td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td class=SubTituloDireita colspan=2><input type=button value=Salvar onclick=enviarform();></td>";
	echo "</tr>";
	
	echo "</table>";
 
}

function pegarResponsavelItemCheckList($docid) {
	global $db, $conf;
	
	$esdid = $db->pegaUm("SELECT esdid FROM workflow.documento WHERE docid='".$docid."'");
	
	$responsavel = $db->pegaUm("SELECT ent.entnome FROM projetos.itemchecklist icl 
				 				INNER JOIN projetos.checklistentidade cle ON cle.iclid = icl.iclid AND cle.tpvid = ".$conf[$esdid]['tpvid']."
				 				INNER JOIN entidade.entidade ent ON ent.entid = cle.entid 
				 				WHERE icl.docid='".$docid."'");
	
	return (($responsavel)?$responsavel:'Sem responsável');
}

if($_REQUEST['requisicao']) {
	
	$_REQUEST['requisicao']($_REQUEST);
	exit;
	
}


include APPRAIZ . 'includes/workflow.php';
include APPRAIZ . 'includes/cabecalho.inc';

print '<br/>';

monta_titulo("ENEM", "Minhas Pendências");

// ----- Recupera o entid do usuário
$entidUsuario = $db->pegaUm("SELECT entid FROM projetos.usuarioresponsabilidade WHERE rpustatus = 'A' AND usucpf = '".$_SESSION['usucpf']."' AND entid is not null");
if( !$entidUsuario ) {
	$entidUsuario = $db->pegaUm("SELECT entid FROM entidade.entidade WHERE entnumcpfcnpj = '".$_SESSION['usucpf']."' AND entstatus = 'A'");
}

if($entidUsuario) {
	
	$parametros = array("capturar_responsavel" => "pegarResponsavelItemCheckList");

	$sql = "SELECT p.pflcod FROM seguranca.perfilusuario p 
			INNER JOIN seguranca.perfil pp ON p.pflcod = pp.pflcod 
			WHERE p.usucpf = '".$_SESSION['usucpf']."' AND pp.sisid = '".$_SESSION['sisid']."'";
	
	$perfis = $db->carregarColuna($sql);
	
	$sql = "SELECT icl.docid, doc.esdid, cle.tpvid FROM projetos.itemchecklist icl
			INNER JOIN projetos.checklistentidade cle ON cle.iclid = icl.iclid AND cle.entid = ".$entidUsuario."
			INNER JOIN workflow.documento doc ON doc.docid = icl.docid 
			INNER JOIN projetos.atividade ati ON ati.atiid = icl.atiid
			WHERE doc.docid is not null AND ati.atistatus='A' ORDER BY icl.iclprazo";
	
	$dados = $db->carregar($sql);
	
	if($dados[0]) {
		foreach($dados as $d) {
			
			if($d['esdid'] == ENEM_EST_EM_EXECUCAO     && $d['tpvid'] == 1 && in_array(PERFIL_EXECUTOR,$perfis)) {
				$docs['pendencias'][] = $d['docid'];
			}
			if($d['esdid'] == ENEM_EST_EM_VALIDACAO    && $d['tpvid'] == 2 && in_array(PERFIL_VALIDADOR,$perfis)) {
				$docs['pendencias'][] = $d['docid'];
			}
			if($d['esdid'] == ENEM_EST_EM_CERTIFICACAO && $d['tpvid'] == 3 && in_array(PERFIL_CERTIFICADOR,$perfis)) {
				$docs['pendencias'][] = $d['docid'];
			}
				
		}
	}
	
	if(in_array(PERFIL_EXECUTOR,$perfis)) {

		$sql = "SELECT icl.docid, doc.esdid, cle.tpvid FROM projetos.itemchecklist icl
				INNER JOIN projetos.checklistentidade cle ON cle.iclid = icl.iclid AND cle.entid = ".$entidUsuario."
				INNER JOIN workflow.documento doc ON doc.docid = icl.docid 
				INNER JOIN projetos.atividade ati ON ati.atiid = icl.atiid
				WHERE doc.docid is not null AND 
					  ati.atistatus='A' AND 
					  doc.esdid IN( SELECT esdidorigem FROM workflow.acaoestadodoc WHERE esdiddestino='".ENEM_EST_EM_EXECUCAO."' )
			    ORDER BY icl.iclprazo";
		
		$dados = $db->carregar($sql);
		
		if($dados[0]) {
			foreach($dados as $d) {
				$docs['futuras'][$d['docid']] = $d['docid'];
			}
		}
		
	}
	
	if(in_array(PERFIL_VALIDADOR,$perfis)) {

		$sql = "SELECT icl.docid, doc.esdid, cle.tpvid FROM projetos.itemchecklist icl
				INNER JOIN projetos.checklistentidade cle ON cle.iclid = icl.iclid AND cle.entid = ".$entidUsuario."
				INNER JOIN workflow.documento doc ON doc.docid = icl.docid 
				INNER JOIN projetos.atividade ati ON ati.atiid = icl.atiid
				WHERE doc.docid is not null AND ati.atistatus='A' AND doc.esdid IN( SELECT esdidorigem FROM workflow.acaoestadodoc WHERE esdiddestino='".ENEM_EST_EM_VALIDACAO."' )
				ORDER BY icl.iclprazo";
		
		$dados = $db->carregar($sql);
		
		if($dados[0]) {
			foreach($dados as $d) {
				$docs['futuras'][$d['docid']] = $d['docid'];
			}
		}
		
	}
	
	if(in_array(PERFIL_CERTIFICADOR,$perfis)) {

		$sql = "SELECT icl.docid, doc.esdid, cle.tpvid FROM projetos.itemchecklist icl
				INNER JOIN projetos.checklistentidade cle ON cle.iclid = icl.iclid AND cle.entid = ".$entidUsuario."
				INNER JOIN workflow.documento doc ON doc.docid = icl.docid 
				INNER JOIN projetos.atividade ati ON ati.atiid = icl.atiid
				WHERE doc.docid is not null AND ati.atistatus='A' AND doc.esdid IN( SELECT esdidorigem FROM workflow.acaoestadodoc WHERE esdiddestino='".ENEM_EST_EM_CERTIFICACAO."' ) 
				ORDER BY icl.iclprazo";
		
		$dados = $db->carregar($sql);
		
		if($dados[0]) {
			foreach($dados as $d) {
				$docs['futuras'][$d['docid']] = $d['docid'];
			}
		}
		
	}

}

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.min.js"></script>
<script>
			function enviarform() {
			if(document.getElementById('vldsituacao_FALSE').checked){
				if(document.getElementById('vldobservacao').value == '') {
					alert('Observação é obrigatória');
					return false;
				}
			}
			if(document.getElementById('etcopcaoevidencia')){
				if(document.getElementById('arquivo').value == '') {
					alert('Anexe um arquivo');
					return false;
				}
			}
			document.getElementById('formulario').submit();
		  }
</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr><td><? wf_gerencimentoFluxo(TPDID_ENEM, $docs, (($_REQUEST['cxentrada'])?$_REQUEST['cxentrada']:'pendencias'),$parametros); ?></td></tr>
</table>