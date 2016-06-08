<?php

include_once APPRAIZ . 'includes/workflow.php';

if($_REQUEST['verDownload']) {
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec("anexochecklist", $arrCampos = null, "projetos");
	$file->getDownloadArquivo($_REQUEST['verDownload']);
}


if($_REQUEST['lerResponsavelItem']) {
	
	$dadosItem = $db->pegaLinha("SELECT ent.entnome, to_char(itm.iclprazo,'dd/mm/YYYY') as iclprazo FROM projetos.itemchecklist itm 
								 INNER JOIN projetos.checklistentidade cle ON cle.iclid = itm.iclid AND tpvid = ".$_REQUEST['fase']."
								 INNER JOIN entidade.entidade ent ON ent.entid = cle.entid 
								 WHERE itm.iclid = '".$_REQUEST['iclid']."'");
	
	$strex .= "<div style=width:700;height:400;overflow:auto;>";
	if($dadosItem) {
		$strex = "<table class=listagem  cellspacing=0 cellpadding=3 align=center style=width:650px;>
					<tr><td class=SubTituloDireita>Executor:</td><td>".$dadosItem['entnome']."</td></tr>
					<tr><td class=SubTituloDireita>Prazo:</td><td>".$dadosItem['iclprazo']."</td></tr>
				  </table>";
	}
	$strex .= "<p align=center><input type=button value=Fechar onclick=messageObj.close();></p>";
	$strex .= "</div>";
	die($strex);
	
}

if($_REQUEST['lerHistoricoItem']) {
	
	$itemchecklist = $db->pegaLinha("SELECT docid, iclid FROM projetos.itemchecklist WHERE iclid='".$_REQUEST['iclid']."'");
	
	$docid = (integer) $itemchecklist['docid'];
	$documento = wf_pegarDocumento( $docid );
	$atual = wf_pegarEstadoAtual( $docid );
	$historico = wf_pegarHistorico( $docid );
	$strHistorico .= "<div style=width:700;height:400;overflow:auto;>";
	$strHistorico .= "<table class=listagem cellspacing=0 cellpadding=3 align=center style=width:650px;>";
	$strHistorico .= "<thead><tr><td style=text-align:center;background-color: #e0e0e0; colspan=6><b style=font-size:10pt;>Histórico de Tramitações<br/></b>";
	$strHistorico .= "<div>".$documento['docdsc']."</div></td></tr>";

	if ( count( $historico ) ) :
		$strHistorico .= "<tr><td style=width:20px;><b>Seq.</b></td><td style=width:200px;><b>Onde Estava</b></td><td style=width:200px;><b>O que aconteceu</b></td><td style=width:90px;><b>Quem fez</b></td><td style=width:120px;><b>Quando fez</b></td><td style=width:17px;>&nbsp;</td></tr>";
	endif;
		
	$strHistorico .= "</thead>";
	$j = 1;
	
	foreach ( $historico as $item ) : 
		
		$marcado = $j % 2 == 0 ? "" : "#f7f7f7";
		$strHistorico .= "<tr bgcolor=".$marcado."><td align=right>".$j."</td><td style=color:#008000;>".$item['esddsc']."</td><td valign=middle style=color:#133368>".$item['aeddscrealizada']."</td>";
		$strHistorico .= "<td style=font-size: 6pt;>".$item['usunome']."</td><td style=color:#133368>".$item['htddata']."</td><td style=color:#133368; text-align:center;>";
		if( $item['cmddsc'] ) :
			$strHistorico .= "<img align=middle style=cursor:pointer; src=../imagens/restricao.png	onclick=exebirOcultarComentario( \'".$j."\' ); />";
		endif;
		$strHistorico .= "</td></tr><tr id=comentario".$j." style=display:none; bgcolor=".$marcado.">";
		$strHistorico .= "<td colspan=6><div >".htmlentities( $item['cmddsc'] )."</div></td></tr>";
		$j++;
			
	endforeach;
	$marcado = $j++ % 2 == 0 ? "" : "#f7f7f7";
	$strHistorico .= "<tr bgcolor=".$marcado."><td style=text-align: right; colspan=6>Estado atual: <span style=color:#008000;>".$atual['esddsc']."</span></td></tr></table>";
		
	
	$sql = "SELECT val1.vldid as existe_execucao,
				   tpv1.tpvdsc as execucao,
				   ent1.entnome as responsavel_execucao,
				   anl1.arqid as arquivo_execucao,
				   arq1.arqnome||'.'||arq1.arqextensao as descricao_arquivo_execucao,
				   val1.vldobservacao as obs_execucao,
				   val2.vldid as existe_validacao,
				   tpv2.tpvdsc as validacao,
				   ent2.entnome as responsavel_validacao,
				   anl2.arqid as arquivo_validacao,
				   arq2.arqnome||'.'||arq2.arqextensao as descricao_arquivo_validacao,
				   val2.vldobservacao as obs_validacao,
				   val3.vldid as existe_certificacao,
				   tpv3.tpvdsc as certificacao,
				   ent3.entnome as responsavel_certificacao,
				   anl3.arqid as arquivo_certificacao,
				   arq3.arqnome||'.'||arq3.arqextensao as descricao_arquivo_certificacao,
				   val3.vldobservacao as obs_certificacao
			FROM projetos.itemchecklist icl 
			LEFT JOIN projetos.validacao val1 ON icl.iclid = val1.iclid AND val1.tpvid=1 
			LEFT JOIN projetos.anexochecklist anl1 ON anl1.vldid = val1.vldid
			LEFT JOIN public.arquivo arq1 ON arq1.arqid = anl1.arqid 
			LEFT JOIN entidade.entidade ent1 ON ent1.entid = val1.entid 
			LEFT JOIN projetos.tipovalidacao tpv1 ON tpv1.tpvid = val1.tpvid 
			LEFT JOIN projetos.validacao val2 ON icl.iclid = val2.iclid AND val2.tpvid=2
			LEFT JOIN projetos.anexochecklist anl2 ON anl2.vldid = val2.vldid 
			LEFT JOIN public.arquivo arq2 ON arq2.arqid = anl2.arqid
			LEFT JOIN entidade.entidade ent2 ON ent2.entid = val2.entid 
			LEFT JOIN projetos.tipovalidacao tpv2 ON tpv2.tpvid = val2.tpvid										
			LEFT JOIN projetos.validacao val3 ON icl.iclid = val3.iclid AND val3.tpvid=3 
			LEFT JOIN entidade.entidade ent3 ON ent3.entid = val3.entid 
			LEFT JOIN projetos.tipovalidacao tpv3 ON tpv3.tpvid = val3.tpvid	
			LEFT JOIN projetos.anexochecklist anl3 ON anl3.vldid = val3.vldid 
			LEFT JOIN public.arquivo arq3 ON arq3.arqid = anl3.arqid									
			WHERE icl.iclid='".$itemchecklist['iclid']."'";
	
	
	
	$dadositm = $db->pegaLinha($sql);
	if($dadositm) {
		$strHistorico .= "<table class=listagem cellspacing=0 cellpadding=3 align=center style=width:650px;>";
		
		if($dadositm['existe_execucao']) {
			
			$strHistorico .= "<tr>
								<td class=SubTituloDireita>Fase realizada:</td><td>".$dadositm['execucao']."</td>
								<td class=SubTituloDireita>Executor:</td><td>".$dadositm['responsavel_execucao']."</td>
								<td class=SubTituloDireita>Anexo:</td><td><a href=enem.php?modulo=principal/atividade_enem/listar_checklist&acao=A&atiid=".$_REQUEST['atiid']."&verDownload=".$dadositm['arquivo_execucao'].">".$dadositm['descricao_arquivo_execucao']."</td>
							  </tr>
							  <tr>
							  	<td class=SubTituloDireita>Descrição:</td><td colspan=5>".$dadositm['obs_execucao']."</td>
							  </tr>";
			
		}
		
		if($dadositm['existe_validacao']) {
			
			$strHistorico .= "<tr>
								<td class=SubTituloDireita>Fase realizada:</td><td>".$dadositm['validacao']."</td>
								<td class=SubTituloDireita>Validador:</td><td>".$dadositm['responsavel_validacao']."</td>
								<td class=SubTituloDireita>Anexo:</td><td><a href=enem.php?modulo=principal/atividade_enem/listar_checklist&acao=A&atiid=".$_REQUEST['atiid']."&verDownload=".$dadositm['arquivo_validacao'].">".$dadositm['descricao_arquivo_validacao']."</td>
							  </tr>
							  <tr>
							  	<td class=SubTituloDireita>Descrição:</td><td colspan=5>".$dadositm['obs_validacao']."</td>
							  </tr>";
			
		}
		
		if($dadositm['existe_certificacao']) {
			
			$strHistorico .= "<tr>
								<td class=SubTituloDireita>Fase realizada:</td><td>".$dadositm['certificacao']."</td>
								<td class=SubTituloDireita>Certificador:</td><td>".$dadositm['responsavel_certificacao']."</td>
								<td class=SubTituloDireita>Anexo:</td><td><a href=enem.php?modulo=principal/atividade_enem/listar_checklist&acao=A&atiid=".$_REQUEST['atiid']."&verDownload=".$dadositm['arquivo_certificacao'].">".$dadositm['descricao_arquivo_certificacao']."</td>
							  </tr>
							  <tr>
							  	<td class=SubTituloDireita>Descrição:</td><td colspan=5>".$dadositm['obs_certificacao']."</td>
							  </tr>";
			
		}
		
		$strHistorico .= "</table>";
	}
	
	$strHistorico .= "<p align=center><input type=button value=Fechar onclick=messageObj.close();></p>";
	$strHistorico .= "</div>";
	die($strHistorico);
}

$atividade = atividade_pegar( $_REQUEST['atiid'] );
if ( !$atividade ) {
	redirecionar( 'principal/atividade_enem/arvore', 'A' );
}

$parametros = array(
	'aba' => $_REQUEST['aba'], # mantém a aba ativada
	'atiid' => $_REQUEST['atiid']
);

$permissao = atividade_verificar_responsabilidade( $atividade['atiid'], $_SESSION['usucpf'] );
$permissao_formulario = $permissao ? 'S' : 'N'; # S habilita e N desabilita o formulário

if( temPerfilSomenteConsulta() || temPerfilExecValidCertif() ) {
	$permissao = false;
	$permissao_formulario = 'N';
}
elseif( temPerfilAdministrador() ) {
	$permissao = true;
	$permissao_formulario = 'S';
}

// ----- VERIFICA SE PROJETO ESTÁ SELECIONADO
projeto_verifica_selecionado();

// ----- Recupera o entid do usuário
$entidUsuario = $db->pegaUm("SELECT entid FROM projetos.usuarioresponsabilidade WHERE rpustatus = 'A' AND usucpf = '".$_SESSION['usucpf']."' AND entid is not null");
if( !$entidUsuario ) {
	$entidUsuario = $db->pegaUm("SELECT entid FROM entidade.entidade WHERE entnumcpfcnpj = '".$_SESSION['usucpf']."' AND entstatus = 'A'");
}

// ----- CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '&atiid=' . $atividade['atiid']  );
montar_titulo_projeto( $atividade['atidescricao'] );

extract( $atividade ); # mantém o formulário preenchido
?>
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />


<script>

function lerResponsavelItem(iclid,fase) {
	displayMessage(window.location.href+'&lerResponsavelItem=1&iclid='+iclid+'&fase='+fase);
}

function lerHistoricoItem(iclid) {
	displayMessage(window.location.href+'&lerHistoricoItem=1&iclid='+iclid);
}

</script>

<table class="tabela" bgcolor="#fbfbfb" cellspacing="0" cellpadding="10" align="center">
	<tr>
		<td>
			<?= montar_resumo_atividade( $atividade ) ?>
			<form method="post" name="checklist" id="checklist" enctype="multipart/form-data"></form>
				<table align="center">
				<tr>
					<td align="center">
						<span>
							<table style="border:1px solid black;" cellpadding="5">
								<thead>
									<tr>
										<th colspan="6"><b>Legenda</b></th>
									</tr>
								</thead>
								<tbody>
									<tr>
										<td><img src="/imagens/exclamacao_checklist.png" border="0" /></td>
										<td>Não informado</td>
										<td><img src="/imagens/check_checklist.png" border="0" /></td>
										<td>Item Validado</td>
										<td><img src="/imagens/erro_checklist.png" border="0" /></td>
										<td>Item invalidado</td>
									</tr>
								</tbody>
							</table>
						</span>
					</td>
				</tr>
				</table>
				<br />
				<table id="tbExecucao" align="center" class="" bgcolor="#fcfcfc" cellspacing="1" cellpadding="3" style="width: 90%;border:1px solid black;">
					<thead>
						<tr>
							<th style="width:10%;">ID</th>
							<th style="width:45%;">Item</th>
							<th style="width:5%;">Ordem</th>
							<th style="width:10%;">Prazo</th>
							<th style="width:10%;">Execução</th>
							<th style="width:10%;">Validação</th>
							<th style="width:10%;">Certificação</th>
						</tr>
					</thead>
					<tbody>
					<?php
						
					$sql = "SELECT DISTINCT
								icl.iclid,
								icl.icldsc,
								icl.iclordem,
								icl.docid,
								to_char(icl.iclprazo,'dd/mm/YYYY') as iclprazo 
							FROM 
								projetos.itemchecklist icl
							WHERE
								icl.atiid = ".$atividade["atiid"]."
							ORDER BY
								icl.iclordem";
					
					$listaChecklist = $db->carregar($sql);
					
					if( $listaChecklist ) {
						
						for($i=0; $i<count($listaChecklist); $i++) {
							
							$cor = ($i%2) ? "#e0e0e0" : "#f4f4f4";
							 
							echo '<tr>
									<td bgcolor="'.$cor.'" align=center>
										'.$listaChecklist[$i]['iclid'].'
									</td>
									<td bgcolor="'.$cor.'">
										'.$listaChecklist[$i]['icldsc'].'
									</td>
									<td bgcolor="'.$cor.'" align=center>
										'.$listaChecklist[$i]['iclordem'].'
									</td>
									<td bgcolor="'.$cor.'">
										'.$listaChecklist[$i]['iclprazo'].'
									</td>';
							
							$sql = "SELECT * FROM projetos.etapascontrole WHERE iclid = ".$listaChecklist[$i]['iclid']." AND tpvid = 1";
							$etapasControleExecucao = $db->carregar($sql);
							
							if($entidUsuario) {
								$sql = "SELECT count(1) FROM projetos.checklistentidade WHERE iclid = ".$listaChecklist[$i]['iclid']." AND entid = ".$entidUsuario." AND tpvid = 1";
								$entidadeExecucao = $db->pegaUm($sql);
							}
							
							if( $etapasControleExecucao && ($entidadeExecucao > 0 || (temPerfilSomenteConsulta() || $db->testa_superuser() || temPerfilAdministrador()) ) ) {
								$sql = "SELECT * FROM projetos.validacao WHERE iclid = ".$listaChecklist[$i]['iclid']." AND tpvid = 1";
								$situacaoExecucao = $db->carregar($sql);
								
								if( $situacaoExecucao ) {
									if( !$situacaoExecucao[0]["vldsituacao"] ) {
										$img = '<img src="/imagens/exclamacao_checklist.png" border="0" />';
									}
									if( $situacaoExecucao[0]["vldsituacao"] == 't' ) {
										$img = '<img src="/imagens/check_checklist.png" border="0" style=cursor:pointer; onclick=lerHistoricoItem(\''.$listaChecklist[$i]['iclid'].'\') />';
									}
									if( $situacaoExecucao[0]["vldsituacao"] == 'f' ) {
										$img = '<img src="/imagens/erro_checklist.png" border="0" onclick=lerResponsavelItem(\''.$listaChecklist[$i]['iclid'].'\',\'1\'); />';
									}
								} else {
									$img = '<img src="/imagens/exclamacao_checklist.png" style=cursor:pointer; onclick=lerResponsavelItem(\''.$listaChecklist[$i]['iclid'].'\',\'1\'); border="0" />';
								}
							} else {
								$img = '-';
							}
							
							echo '<td bgcolor="'.$cor.'" align="center" id="item_'.$listaChecklist[$i]['iclid'].'_E">'.$img.'</td>';
							
							$sql = "SELECT * FROM projetos.etapascontrole WHERE iclid = ".$listaChecklist[$i]['iclid']." AND tpvid = 2";
							$etapasControleValidacao = $db->carregar($sql);
							
							if($entidUsuario) {
								$sql = "SELECT count(1) FROM projetos.checklistentidade WHERE iclid = ".$listaChecklist[$i]['iclid']." AND entid = ".$entidUsuario." AND tpvid = 2";
								$entidadeValidacao = $db->pegaUm($sql);
							}
							
							if( $etapasControleValidacao && ($entidadeValidacao > 0 || (temPerfilSomenteConsulta() || $db->testa_superuser() || temPerfilAdministrador())) ) {
								
								$sql = "SELECT * FROM projetos.validacao WHERE iclid = ".$listaChecklist[$i]['iclid']." AND tpvid = 2";
								$situacaoValidacao = $db->carregar($sql);

								if( $situacaoValidacao ) {
									if( !$situacaoValidacao[0]["vldsituacao"] ) {
										$img = '<img src="/imagens/exclamacao_checklist.png" border="0" />';
									}
									if( $situacaoValidacao[0]["vldsituacao"] == 't' ) {
										$img = '<img '.$class.' src="/imagens/check_checklist.png" border="0" onclick=lerResponsavelItem(\''.$listaChecklist[$i]['iclid'].'\',\'2\'); />';
									}
									if( $situacaoValidacao[0]["vldsituacao"] == 'f' ) {
										$img = '<img '.$class.' src="/imagens/erro_checklist.png" border="0" onclick=lerResponsavelItem(\''.$listaChecklist[$i]['iclid'].'\',\'2\'); />';
									}
								} else {
									$img = '<img src="/imagens/exclamacao_checklist.png" border="0" style="cursor:pointer;" onclick=lerResponsavelItem(\''.$listaChecklist[$i]['iclid'].'\',\'2\'); />';
								}
							}
							else
							{
								$img = '-';
							}
								
							echo '<td bgcolor="'.$cor.'" align="center" id="item_'.$listaChecklist[$i]['iclid'].'_V">
										'.$img.'
									</td>';

							$sql = "SELECT * FROM projetos.etapascontrole WHERE iclid = ".$listaChecklist[$i]['iclid']." AND tpvid = 3";
							$etapasControleCertificacao = $db->carregar($sql);
							
							if($entidUsuario) {
								$sql = "SELECT count(1) FROM projetos.checklistentidade WHERE iclid = ".$listaChecklist[$i]['iclid']." AND entid = ".$entidUsuario." AND tpvid = 3";
								$entidadeCertificacao = $db->pegaUm($sql);
							}
							
							if( $etapasControleCertificacao && ($entidadeCertificacao > 0 || (temPerfilSomenteConsulta() || $db->testa_superuser() || temPerfilAdministrador())) ) {
								
								$sql = "SELECT * FROM projetos.validacao WHERE iclid = ".$listaChecklist[$i]['iclid']." AND tpvid = 3";
									
								$situacaoCertificacao = $db->carregar($sql);
								
								if( $situacaoCertificacao ) {
									if( !$situacaoCertificacao[0]["vldsituacao"] ) {
										$img = '<img src="/imagens/exclamacao_checklist.png" border="0" />';
									}
									if( $situacaoCertificacao[0]["vldsituacao"] == 't' ) {
										$img = '<img '.$class.' src="/imagens/check_checklist.png" border="0" onclick=lerResponsavelItem(\''.$listaChecklist[$i]['iclid'].'\',\'3\'); />';
									}
									if( $situacaoCertificacao[0]["vldsituacao"] == 'f' ) {
										$img = '<img '.$class.' src="/imagens/erro_checklist.png" border="0" onclick=lerResponsavelItem(\''.$listaChecklist[$i]['iclid'].'\',\'3\'); />';
									}
									
								} else {
									$img = '<img src="/imagens/exclamacao_checklist.png" border="0" style="cursor:pointer;" onclick=lerResponsavelItem(\''.$listaChecklist[$i]['iclid'].'\',\'3\'); />';

								}
							}
							else
							{
								$img = '-';
							}
								
							echo '<td bgcolor="'.$cor.'" align="center" id="item_'.$listaChecklist[$i]['iclid'].'_C">'.$img.'</td></tr>';
						}
					}
					else
					{
						echo '<tr><td bgcolor="#f4f4f4" style="color:red;text-align:center;" colspan="4">Não existe item(ns) de checklist cadastrado(s).</td></tr>';
					}

					?>
					</tbody>
				</table>
		</td>
	</tr>
</table>
	<script type="text/javascript">
	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	
	function displayMessage(url) {
		messageObj.setSource(url);
		messageObj.setCssClassMessageBox(false);
		messageObj.setSize(690,400);
		messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
		messageObj.display();
	}
	function displayStaticMessage(messageContent,cssClass) {
		messageObj.setHtmlContent(messageContent);
		messageObj.setSize(600,150);
		messageObj.setCssClassMessageBox(cssClass);
		messageObj.setSource(false);	// no html source since we want to use a static message here.
		messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
		messageObj.display();
	}
	function closeMessage() {
		messageObj.close();	
	}
	</script>