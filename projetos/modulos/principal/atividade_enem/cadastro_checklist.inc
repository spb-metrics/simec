<?php

include_once APPRAIZ . 'includes/workflow.php';

$atividade = atividade_pegar( $_REQUEST['atiid'] );
if ( !$atividade ) {
	redirecionar( 'principal/atividade_enem/arvore', 'A' );
}

$parametros = array(
	'aba' => $_REQUEST['aba'], # mantém a aba ativada
	'atiid' => $_REQUEST['atiid']
);

if( $_POST['ajaxOrdem'] )
{
	$db->executar("UPDATE projetos.itemchecklist SET iclordem = ".$_POST['ordem']." WHERE iclid = ".$_POST['iclid']);
	$db->executar("UPDATE projetos.itemchecklist SET iclordem = ".$_POST['ordem2']." WHERE iclid = ".$_POST['iclid2']);
	
	if( $db->commit() )
		die('ok');
	else
		die('erro');
}

if( $_REQUEST['evento'] )
{
	switch( $_REQUEST['evento'] )
	{
		case 'cadastrar_checklist':
			
			$iclprazo = explode('/', $_REQUEST['iclprazo']);
			$iclprazo = $iclprazo[2].'-'.$iclprazo[1].'-'.$iclprazo[0];
			
			/*** UPDATE ***/
			if( $_REQUEST['iclid'] )
			{
				$iclid = $_REQUEST['iclid'];
				
				/* Gatilho no workflow solicitado pelo Henrique Xavier - 19/05/2011 - Regras para alteração do item */
				$sql = "SELECT d.docid, d.esdid FROM projetos.itemchecklist i 
						INNER JOIN workflow.documento d ON i.docid = d.docid 
						WHERE i.iclid='".$_REQUEST['iclid']."'";
				$wfdados = $db->pegaLinha($sql);
				
				// REGRA : (Henrique) Caso o usuário vai acrescentar uma nova fase de validação que não exista para o item, caso o documento já tiver sido finalizado, o sistema deverá voltar o estado do documento para o que foi criado, para continuar a tramitação;
				if($wfdados['esdid'] == ENEM_EST_EM_FINALIZADO && $_REQUEST["etapa_controle_validacao"] && !$db->pegaUm("SELECT etcid FROM projetos.etapascontrole WHERE tpvid = 2 AND iclid = ".$_REQUEST['iclid'])) {
					$db->executar("UPDATE workflow.documento SET esdid='".ENEM_EST_EM_VALIDACAO."' WHERE docid='".$wfdados['docid']."'");
				}
				if($wfdados['esdid'] == ENEM_EST_EM_FINALIZADO && $_REQUEST["etapa_controle_certificacao"] && !$db->pegaUm("SELECT etcid FROM projetos.etapascontrole WHERE tpvid = 3 AND iclid = ".$_REQUEST['iclid'])) {
					$db->executar("UPDATE workflow.documento SET esdid='".ENEM_EST_EM_CERTIFICACAO."' WHERE docid='".$wfdados['docid']."'");
				}
				if(($wfdados['esdid'] != ENEM_EST_EM_EXECUCAO && $db->pegaUm("SELECT etcid FROM projetos.etapascontrole WHERE tpvid = 2 AND iclid = ".$_REQUEST['iclid']) && !$_REQUEST["etapa_controle_validacao"]) ||
				   ($wfdados['esdid'] != ENEM_EST_EM_EXECUCAO && $db->pegaUm("SELECT etcid FROM projetos.etapascontrole WHERE tpvid = 3 AND iclid = ".$_REQUEST['iclid']) && !$_REQUEST["etapa_controle_certificacao"]) || 
				   ($wfdados['esdid'] != ENEM_EST_EM_EXECUCAO && $_REQUEST["entid_executor"] != $db->pegaUm("SELECT entid FROM projetos.checklistentidade WHERE tpvid = 1 AND iclid = ".$_REQUEST['iclid'])) || 
				   ($wfdados['esdid'] != ENEM_EST_EM_EXECUCAO && $_REQUEST["entid_validador"] != $db->pegaUm("SELECT entid FROM projetos.checklistentidade WHERE tpvid = 2 AND iclid = ".$_REQUEST['iclid'])) || 
				   ($wfdados['esdid'] != ENEM_EST_EM_EXECUCAO && $_REQUEST["entid_certificador"] != $db->pegaUm("SELECT entid FROM projetos.checklistentidade WHERE tpvid = 3 AND iclid = ".$_REQUEST['iclid']))) {
					
				   	die("<script>
				   			alert('O item finalizado, não é possivel modificar a estrutura dele. Remova-o e insira novamente.');
				   			window.location='enem.php?modulo=principal/atividade_enem/cadastro_checklist&acao=A&atiid=".$atividade["atiid"]."';
				   		 </script>");
				}
				
				
				/* FIM - Gatilho no workflow solicitado pelo Henrique Xavier - 19/05/2011 */
				
				
				
				$sql = "UPDATE projetos.itemchecklist SET icldsc = '".$_REQUEST['icldsc']."',iclprazo = '".$iclprazo."',iclcritico = '".$_REQUEST['iclcritico']."' WHERE iclid = ".$_REQUEST['iclid'];
				$db->executar($sql);
				
				$docdsc = "<p>".$iclid." - ".$_REQUEST["icldsc"]."</p><p>".$atividade['_atinumero']." - ".$atividade['atidescricao']."</p>";
				$db->executar("UPDATE workflow.documento SET docdsc='".$docdsc."' WHERE docid='".$wfdados['docid']."'");
				
				
				
				$sql = "DELETE FROM projetos.etapascontrole WHERE iclid = ".$_REQUEST['iclid'];
				$db->executar($sql);
				
				$sql = "DELETE FROM projetos.checklistentidade WHERE iclid = ".$_REQUEST['iclid'];
				$db->executar($sql);
				
				
			}
			/*** INSERT ***/
			else
			{
				$iclordem = $db->pegaUm("SELECT max(iclordem) FROM projetos.itemchecklist WHERE atiid = ".$atividade["atiid"]);
				$iclordem = ($iclordem) ? ((integer)$iclordem + 1) : 1;
				
				$sql = "INSERT INTO projetos.itemchecklist(icldsc, atiid, iclprazo, iclcritico, iclordem)
						VALUES ('".$_REQUEST["icldsc"]."', ".$atividade["atiid"].", '".$iclprazo."', '".$_REQUEST['iclcritico']."', ".$iclordem.")
						RETURNING iclid";
				$iclid = $db->pegaUm($sql);
				
				
				$tpdid 	= TPDID_ENEM;
				$docdsc = "<p>".$iclid." - ".$_REQUEST["icldsc"]."</p><p>".$atividade['_atinumero']." - ".$atividade['atidescricao']."</p>";
				$docid = wf_cadastrarDocumento( $tpdid, str_replace("\'","'",$docdsc) );
				
				$db->executar("UPDATE projetos.itemchecklist SET docid='".$docid."' WHERE iclid='".$iclid."'");
				
				// prdid,
				//, ".$_REQUEST["prdid"]."
			}
			
			/*** Execução ***/
			if( $_REQUEST["etapa_controle_execucao"] )
			{
				if( $_REQUEST["opcao_evidencia_execucao"] == 'S' )
				{
					$etcopcaoevidencia = 't';
					$etcevidencia = "'".$_REQUEST["etcevidenciaexecucao"]."'";
				}
				else
				{
					$etcopcaoevidencia = 'f';
					$etcevidencia = 'null';
				}
				
				$sql = "INSERT INTO projetos.etapascontrole (tpvid,iclid,etcopcaoevidencia,etcevidencia)
						VALUES (1, ".$iclid.", '".$etcopcaoevidencia."', ".$etcevidencia.")";
				$db->executar($sql);
				
				if( $_REQUEST["entid_executor"] )
				{
					$sql = "INSERT INTO projetos.checklistentidade(iclid,entid,tpvid) VALUES(".$iclid.",".$_REQUEST["entid_executor"].",1)";
					$db->executar($sql);
				}
			}
			/*** Validação ***/
			if( $_REQUEST["etapa_controle_validacao"] )
			{
				if( $_REQUEST["opcao_evidencia_validacao"] == 'S' )
				{
					$etcopcaoevidencia = 't';
					$etcevidencia = "'".$_REQUEST["etcevidenciavalidacao"]."'";
				}
				else
				{
					$etcopcaoevidencia = 'f';
					$etcevidencia = 'null';
				}
				
				$sql = "INSERT INTO projetos.etapascontrole (tpvid,iclid,etcopcaoevidencia,etcevidencia)
						VALUES (2, ".$iclid.", '".$etcopcaoevidencia."', ".$etcevidencia.")";
				$db->executar($sql);
				
				if( $_REQUEST["entid_validador"] )
				{
					$sql = "INSERT INTO projetos.checklistentidade(iclid,entid,tpvid) VALUES(".$iclid.",".$_REQUEST["entid_validador"].",2)";
					$db->executar($sql);
				}
			}
			/*** Certificação ***/
			if( $_REQUEST["etapa_controle_certificacao"] )
			{
				if( $_REQUEST["opcao_evidencia_certificacao"] == 'S' )
				{
					$etcopcaoevidencia = 't';
					$etcevidencia = "'".$_REQUEST["etcevidenciacertificacao"]."'";
				}
				else
				{
					$etcopcaoevidencia = 'f';
					$etcevidencia = 'null';
				}
				
				$sql = "INSERT INTO projetos.etapascontrole (tpvid,iclid,etcopcaoevidencia,etcevidencia)
						VALUES (3, ".$iclid.", '".$etcopcaoevidencia."', ".$etcevidencia.")";
				$db->executar($sql);
				
				if( $_REQUEST["entid_certificador"] )
				{
					$sql = "INSERT INTO projetos.checklistentidade(iclid,entid,tpvid) VALUES(".$iclid.",".$_REQUEST["entid_certificador"].",3)";
					$db->executar($sql);
				}
			}
			
			$db->commit();
			echo "<script>
					alert('Dados Gravados com sucesso');
					window.location='enem.php?modulo=principal/atividade_enem/cadastro_checklist&acao=A&atiid=".$atividade["atiid"]."';
				  </script>";
			exit;
			break;
	
		case 'carregar_checklist':
			//prdid,
			$sql = "SELECT icldsc,to_char(iclprazo, 'DD/MM/YYYY') as iclprazo,iclcritico FROM projetos.itemchecklist WHERE iclid = ".$_REQUEST['iclid'];
			$dados = $db->carregar($sql);
			
			$iclid 	= $_REQUEST['iclid'];
			$icldsc = $dados[0]["icldsc"];
			//$prdid 	= $dados[0]["prdid"];
			$iclprazo = $dados[0]["iclprazo"];
			$iclcritico = $dados[0]["iclcritico"];
			
			// etapas de controle
			$sql = "SELECT etcopcaoevidencia,etcevidencia as etcevidenciaexecucao FROM projetos.etapascontrole WHERE iclid = ".$iclid." AND tpvid = 1";
			$dadosExecucao = $db->carregar($sql);
			
			$sql = "SELECT etcopcaoevidencia,etcevidencia as etcevidenciavalidacao FROM projetos.etapascontrole WHERE iclid = ".$iclid." AND tpvid = 2";
			$dadosValidacao = $db->carregar($sql);
			
			$sql = "SELECT etcopcaoevidencia,etcevidencia as etcevidenciacertificacao FROM projetos.etapascontrole WHERE iclid = ".$iclid." AND tpvid = 3";
			$dadosCertificacao = $db->carregar($sql);
			
			// entidades
			$sql = "SELECT e.entid, e.entnome FROM projetos.checklistentidade c 
					INNER JOIN entidade.entidade e ON e.entid = c.entid 
					WHERE c.iclid = ".$_REQUEST['iclid']." AND c.tpvid = 1";
			$entExecucao = $db->pegaLinha($sql);
			
			$sql = "SELECT e.entid, e.entnome FROM projetos.checklistentidade c 
					INNER JOIN entidade.entidade e ON e.entid = c.entid 
					WHERE c.iclid = ".$_REQUEST['iclid']." AND c.tpvid = 2";
			$entValidacao = $db->pegaLinha($sql);
			
			$sql = "SELECT e.entid, e.entnome FROM projetos.checklistentidade c
					INNER JOIN entidade.entidade e ON e.entid = c.entid 
					WHERE c.iclid = ".$_REQUEST['iclid']." AND c.tpvid = 3";
			$entCertificacao = $db->pegaLinha($sql);
			
			break;
			
		case 'excluir_checklist':
			$sql = "DELETE FROM workflow.historicodocumento WHERE docid IN(SELECT docid FROM projetos.itemchecklist WHERE iclid='".$_REQUEST['iclid']."')";
			$db->executar($sql);
			
			$sql = "DELETE FROM projetos.anexochecklist WHERE vldid IN(SELECT vldid FROM projetos.validacao WHERE iclid = ".$_REQUEST['iclid'].")";
			$db->executar($sql);
			
			$sql = "DELETE FROM projetos.validacao WHERE iclid = ".$_REQUEST['iclid'];
			$db->executar($sql);

			$sql = "DELETE FROM projetos.etapascontrole WHERE iclid = ".$_REQUEST['iclid'];
			$db->executar($sql);
			
			$sql = "DELETE FROM projetos.checklistentidade WHERE iclid = ".$_REQUEST['iclid'];
			$db->executar($sql);
				
			$sql = "DELETE FROM projetos.itemchecklist WHERE iclid = ".$_REQUEST['iclid'];
			$db->executar($sql);
			
			$db->commit();
			echo "<script>
					alert('Dados Gravados com sucesso');
					window.location='enem.php?modulo=principal/atividade_enem/cadastro_checklist&acao=A&atiid=".$atividade["atiid"]."';
				  </script>";
			exit;
			break;
			
		default:
			break;
	}
}

$sql = "select
			to_char(atidatainicio, 'YYYYMMDD') as data_inicio,
			to_char((atidatainicio + atiduracao::integer), 'YYYYMMDD') as data_fim
		from
			projetos.atividade
		where
			atiid = {$atividade['atiid']}
		and
			atistatus = 'A'";
$dataAtividade = $db->pegaLinha($sql);

$permissao = atividade_verificar_responsabilidade( $atividade['atiid'], $_SESSION['usucpf'] );
$permissao_formulario = $permissao ? 'S' : 'N'; # S habilita e N desabilita o formulário

if( temPerfilSomenteConsulta() || temPerfilExecValidCertif() )
{
	$permissao = false;
	$permissao_formulario = 'N';
}
elseif( temPerfilAdministrador() )
{
	$permissao = true;
	$permissao_formulario = 'S';
}

// ----- VERIFICA SE PROJETO ESTÁ SELECIONADO
projeto_verifica_selecionado();

// ----- CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '&atiid=' . $atividade['atiid']  );
montar_titulo_projeto( $atividade['atidescricao'] );

extract( $atividade ); # mantém o formulário preenchido
?>

<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script language="javascript" type="text/javascript">

jQuery.noConflict();

jQuery(document).ready(function($)
{
	// ao escolher uma das opções de etapa de controle...
	$('.etapa_controle').click(function()
	{
		// execução
		if( $(this).val() == 'E' )
		{
			if( $(this).is(':checked') )
			{
				jQuery("#linha_evidencia_execucao").show();
				jQuery("#linha_executores").show();
			}
			else
			{
				jQuery("#linha_evidencia_execucao").hide();
				jQuery("#linha_executores").hide();
			}
		}
		// validação
		if( $(this).val() == 'V' )
		{
			if( $(this).is(':checked') )
			{
				jQuery("#linha_evidencia_validacao").show();
				jQuery("#linha_validadores").show();
			}
			else
			{
				jQuery("#linha_evidencia_validacao").hide();
				jQuery("#linha_validadores").hide();
			}
		}
		// certificação
		if( $(this).val() == 'C' )
		{
			if( $(this).is(':checked') )
			{
				jQuery("#linha_evidencia_certificacao").show();
				jQuery("#linha_certificadores").show();
			}
			else
			{
				jQuery("#linha_evidencia_certificacao").hide();
				jQuery("#linha_certificadores").hide();
			}
		}
	});

	// ao optar por uma opção de evidência ou não...
	$('.opcao_evidencia').click(function()
	{
		// execução
		if( $(this).attr('name') == 'opcao_evidencia_execucao' )
		{
			if( $(this).val() == 'S' )
				$('#evidencia_execucao').show();
			else
				$('#evidencia_execucao').hide();
		}
		// validação
		if( $(this).attr('name') == 'opcao_evidencia_validacao' )
		{
			if( $(this).val() == 'S' )
				$('#evidencia_validacao').show();
			else
				$('#evidencia_validacao').hide();
		}
		// certificação
		if( $(this).attr('name') == 'opcao_evidencia_certificacao' )
		{
			if( $(this).val() == 'S' )
				$('#evidencia_certificacao').show();
			else
				$('#evidencia_certificacao').hide();
		}
	});

	// ao submeter o formulário
	$('#btSalvar').click(function()
	{
		
		if( $('#icldsc').val() == '' )
		{
			alert("O campo 'Descriçao do item de Checklist' deve ser preenchido.");
			$('#icldsc').focus();
			return;
		}
		/*if( $('#prdid').val() == '' )
		{
			alert("O campo 'Peridiocidade' deve ser preenchido.");
			$('#prdid').focus();
			return;
		}*/
		if( $('#iclprazo').val() == '' )
		{
			alert("O campo 'Prazo' deve ser preenchido.");
			$('#iclprazo').focus();
			return;
		}

		var arrData = $('#iclprazo').val().split('/');
		var data 	= arrData[2] + arrData[1] + arrData[0];

		<?php if($dataAtividade['data_inicio']): ?>
		if( data < <?=$dataAtividade['data_inicio']?> )
		{
			alert("O campo 'Prazo' deve ser maior que data de início da Atividade.");
			$('#iclprazo').focus();
			return;
		}
		<?php endif;?>

		<?php if($dataAtividade['data_fim']): ?>
		if( data > <?=$dataAtividade['data_fim']?> )
		{
			alert("O campo 'Prazo' deve ser menor que data final(duração) da Atividade.");
			$('#iclprazo').focus();
			return;
		}
		<?php endif;?>
		
		var str = '';
		
		$('.etapa_controle').each(function()
		{
			if( $(this).is(':checked') )
			{
				if( $(this).val() == 'E' )
				{
					if( $('#opcao_evidencia_execucao_sim').is(':checked') )
					{
						if( $('#iclevidenciaexecucao').val() == '' )
							str += "O campo 'Evidência' da Execução deve ser preenchido.\n";
					}

					if( $('#entid_executor').val() == '' )
					{
						str += "Selecione o executor.\n";
					}
				}
				if( $(this).val() == 'V' )
				{
					if( $('#opcao_evidencia_validacao_sim').is(':checked') )
					{
						if( $('#iclevidenciavalidacao').val() == '' )
							str += "O campo 'Evidência' da Validação deve ser preenchido.\n";
					}

					if( $('#entid_validador').val() == '' )
					{
						str += "Selecione o validador.\n";
					}
				}
				if( $(this).val() == 'C' )
				{
					if( $('#opcao_evidencia_execucao_sim').is(':checked') == false )
					{
						str += "Com 'Certificação' selecionado, o campo 'Evidência' da Execução deve ser preenchido.\n";
					}
				
					if( $('#opcao_evidencia_certificacao_sim').is(':checked') )
					{
						if( $('#iclevidenciacertificacao').val() == '' )
							str += "O campo 'Evidência' da Certificação deve ser preenchido.";
					}

					if( $('#entid_certificador').val() == '' )
					{
						str += "Selecione o certificador.\n";
					}
				}
			}
		});

		if( str != '' )
		{
			alert(str);
			return;
		}
		
		$('#checklist').submit();
	});
});

function alterarItemChecklist(id)
{
	var form 	= document.getElementById('checklist');
	var evento 	= document.getElementById('evento');
	var iclid 	= document.getElementById('iclid');
	iclid.value = id;
	evento.value = 'carregar_checklist';
	form.submit();
}

function excluirItemChecklist(id)
{
	if( confirm("Deseja realmente excluir este item?") )
	{
		var form 	= document.getElementById('checklist');
		var evento 	= document.getElementById('evento');
		var iclid 	= document.getElementById('iclid');
	
		iclid.value = id;
		evento.value = 'excluir_checklist';
		form.submit();
	}
}

function inserirExecutor()
{
	janela("enem.php?modulo=principal/atividade_enem/inserir_executor_processo&acao=A&atiid=<?=$_REQUEST['atiid']?>&entid_executor="+document.getElementById('entid_executor').value,800,580,"att_exec");
}

function inserirValidador()
{
	janela("enem.php?modulo=principal/atividade_enem/inserir_validador_processo&acao=A&atiid=<?=$_REQUEST['atiid']?>&entid_validador="+document.getElementById('entid_validador').value,800,580,"att_valid");
}

function inserirCertificador()
{
	janela("enem.php?modulo=principal/atividade_enem/inserir_certificador_processo&acao=A&atiid=<?=$_REQUEST['atiid']?>&entid_certificador="+document.getElementById('entid_certificador').value,800,580,"att_cert");
}

function alterarOrdem(index, sentido)
{
	var index2;
	index2 = (sentido == 'cima') ? (index - 1) : (index + 1);

	var iclid  = jQuery('#tb_itens_checklist').find('tr').eq(index).attr('id');
	var iclid2 = jQuery('#tb_itens_checklist').find('tr').eq(index2).attr('id');

	jQuery.ajax({
	      url: "enem.php?modulo=principal/atividade_enem/cadastro_checklist&acao=A&atiid=<?=$atividade['atiid']?>",
	      type: "POST",
	      data: "ajaxOrdem=1&iclid="+iclid+"&ordem="+index2+"&iclid2="+iclid2+"&ordem2="+index+"",
	      success: function(msg)
	      {
	      	if(msg == 'ok')
	      	{
	      		jQuery('#tb_itens_checklist').find('tr').eq(index).attr('id', iclid2);
	      		jQuery('#tb_itens_checklist').find('tr').eq(index2).attr('id', iclid);	
	      		
	      		var html  = jQuery('#tb_itens_checklist').find('tr').eq(index2).html();
	      		var html2 = jQuery('#tb_itens_checklist').find('tr').eq(index).html();

	      		jQuery('#tb_itens_checklist').find('tr').eq(index).html(html);
	      		jQuery('#tb_itens_checklist').find('tr').eq(index2).html(html2);

	      		var col  = jQuery('#tb_itens_checklist').find('tr').eq(index).find('td').eq(0).html();
	      		var col2 = jQuery('#tb_itens_checklist').find('tr').eq(index2).find('td').eq(0).html();

	      		jQuery('#tb_itens_checklist').find('tr').eq(index).find('td').eq(0).html(col2);
	      		jQuery('#tb_itens_checklist').find('tr').eq(index2).find('td').eq(0).html(col);
	      	}
	      	else
	      	{
		      	alert('Ocorreu um erro. Entre em contato com o administrador do sistema.');
	      	}
	      }
	});
}

</script>

<table class="tabela" bgcolor="#fbfbfb" cellspacing="0" cellpadding="10" align="center">
	<tr>
		<td>
			<?= montar_resumo_atividade( $atividade ) ?>
			<?php if( $permissao ): ?>
				<form method="post" name="checklist" id="checklist" enctype="multipart/form-data">
					<input type="hidden" id="evento" name="evento" value="cadastrar_checklist"/>
					<input type="hidden" id="iclid" name="iclid" value="<?php echo $iclid; ?>" />
					<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="5" style="width: 100%;">
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descriçao do item de Checklist:</td>
							<td>
								<?php echo campo_texto('icldsc','S','S','',60, 500,'','',null,null,null,'id="icldsc"'); ?>
							</td>
						</tr>
						<!--<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Peridiocidade:</td>
							<td>
								<?
								/*$sql = "SELECT
											prdid as codigo,
											prddsc as descricao
									    FROM
									    	projetos.periodicidade
									    WHERE
									    	prdstatus = 'A' 
									    ORDER BY 
									    	prddsc";
								$db->monta_combo('prdid', $sql, 'S', 'Selecione', '', '', '', '250', 'S', 'prdid');*/
								?>
							</td>
						</tr>-->
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Prazo:</td>
							<td>
								<?php echo campo_data2('iclprazo', 'S', 'S', 'Prazo', '##/##/####') ?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Item crítico:</td>
							<td>
								<input type="radio" name="iclcritico" value="f" <?= ((!$iclcritico || $iclcritico=='f') ? 'checked="checked"' : '') ?> /> Não
								<input type="radio" name="iclcritico" value="t" <?= (($iclcritico=='t') ? 'checked="checked"' : '') ?> /> Sim
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Etapas de Controle:</td>
							<td>
								<?php
									/*** Execução ***/
									$checkExecucao = '';
									$displayLinhaExecucao = 'none';
									$displayEvidenciaExecucao = 'none';
									
									if( $dadosExecucao )
									{
										$checkExecucao = 'checked="checked"';
										$displayLinhaExecucao = 'table-row';
										$opcaoEvidenciaExecucao = $dadosExecucao[0]["etcopcaoevidencia"];
										
										if( $opcaoEvidenciaExecucao == 't' )
										{
											$displayEvidenciaExecucao = 'inline';
											$etcevidenciaexecucao = $dadosExecucao[0]["etcevidenciaexecucao"];
										}
									}
									/*** Validação ***/
									$checkValidacao = '';
									$displayLinhaValidacao = 'none';
									$displayEvidenciaValidacao = 'none';
									
									if( $dadosValidacao )
									{
										$checkValidacao = 'checked="checked"';
										$displayLinhaValidacao = 'table-row';
										$opcaoEvidenciaValidacao = $dadosValidacao[0]["etcopcaoevidencia"];
										
										if( $opcaoEvidenciaValidacao == 't' )
										{
											$displayEvidenciaValidacao = 'inline';
											$etcevidenciavalidacao = $dadosValidacao[0]["etcevidenciavalidacao"];
										}
									}
									/*** Certificação ***/
									$checkCertificacao = '';
									$displayLinhaCertificacao = 'none';
									$displayEvidenciaCertificacao = 'none';
									
									if( $dadosCertificacao )
									{
										$checkCertificacao = 'checked="checked"';
										$displayLinhaCertificacao = 'table-row';
										$opcaoEvidenciaCertificacao = $dadosCertificacao[0]["etcopcaoevidencia"];
										
										if( $opcaoEvidenciaCertificacao == 't' )
										{
											$displayEvidenciaCertificacao = 'inline';
											$etcevidenciacertificacao = $dadosCertificacao[0]["etcevidenciacertificacao"];
										}
									}
								?>
								<input type="checkbox" value="E" class="etapa_controle" id="etapa_controle_execucao" name="etapa_controle_execucao" <?=$checkExecucao?> onclick="if(this.checked==true){document.getElementById('etapa_controle_validacao').disabled=false;}else{document.getElementById('etapa_controle_validacao').checked=true;document.getElementById('etapa_controle_validacao').click();document.getElementById('etapa_controle_validacao').disabled=true;}" /> Execução
								&nbsp;&nbsp;&nbsp;
								<input type="checkbox" value="V" class="etapa_controle" id="etapa_controle_validacao" name="etapa_controle_validacao" <?=$checkValidacao?> <? echo ((!$checkExecucao)?"disabled=disabled":""); ?> onclick="if(this.checked==true){document.getElementById('etapa_controle_certificacao').disabled=false;}else{document.getElementById('etapa_controle_certificacao').checked=true;document.getElementById('etapa_controle_certificacao').click();document.getElementById('etapa_controle_certificacao').disabled=true;}" /> Validação
								&nbsp;&nbsp;&nbsp;
								<input type="checkbox" value="C" class="etapa_controle" id="etapa_controle_certificacao" name="etapa_controle_certificacao" <?=$checkCertificacao?> <? echo ((!$checkValidacao)?"disabled=disabled":""); ?> /> Certificação
							</td>
						</tr>
						<tr id="linha_evidencia_execucao" style="display:<?=$displayLinhaExecucao?>;">
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
								<img src="/imagens/seta_filho.gif" />
								<b>Coletar evidência na Execução?</b>
							</td>
							<td>
								<input type="radio" value="S" class="opcao_evidencia" id="opcao_evidencia_execucao_sim" name="opcao_evidencia_execucao" <?php if($opcaoEvidenciaExecucao=='t') echo 'checked="checked"'; ?> /> Sim
								&nbsp;&nbsp;
								<input type="radio" value="N" class="opcao_evidencia" id="opcao_evidencia_execucao_nao" name="opcao_evidencia_execucao" <?php if(!$opcaoEvidenciaExecucao || $opcaoEvidenciaExecucao=='f') echo 'checked="checked"'; ?> /> Não
								&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
								<span id="evidencia_execucao" style="display:<?=$displayEvidenciaExecucao?>;">
									<b>Evidência</b>: <?php echo campo_texto('etcevidenciaexecucao','S','S','',30, 500,'','',null,null,null,'id="iclevidenciaexecucao"'); ?>
								</span>
							</td>
						</tr>
						<tr id="linha_executores" style="display:<?=$displayLinhaExecucao?>;">
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
								<img src="/imagens/seta_filho.gif" />
								<b>Executores</b>
							</td>
							<td>
							<span id="nome_executor"><? echo $entExecucao['entnome']; ?></span> 
							<a style="cursor:pointer;" onclick="inserirExecutor(); return false;"><img src="../imagens/gif_inclui.gif" align="absmiddle"> Executor</a>
							<input type="hidden" name="entid_executor" id="entid_executor" value="<? echo $entExecucao['entid']; ?>">
							</td>
						</tr>
						
						<tr id="linha_evidencia_validacao" style="display:<?=$displayLinhaValidacao?>;">
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
								<img src="/imagens/seta_filho.gif" />
								<b>Coletar evidência na Validação?</b>
							</td>
							<td>
								<input type="radio" value="S" class="opcao_evidencia" id="opcao_evidencia_validacao_sim" name="opcao_evidencia_validacao" <?php if($opcaoEvidenciaValidacao=='t') echo 'checked="checked"'; ?> /> Sim
								&nbsp;&nbsp;
								<input type="radio" value="N" class="opcao_evidencia" id="opcao_evidencia_validacao_nao" name="opcao_evidencia_validacao" <?php if(!$opcaoEvidenciaValidacao || $opcaoEvidenciaValidacao=='f') echo 'checked="checked"'; ?> /> Não
								&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
								<span id="evidencia_validacao" style="display:<?=$displayEvidenciaValidacao?>;">
									<b>Evidência</b>: <?php echo campo_texto('etcevidenciavalidacao','S','S','',30, 500,'','',null,null,null,'id="iclevidenciavalidacao"'); ?>
								</span>
							</td>
						</tr>
						<tr id="linha_validadores" style="display:<?=$displayLinhaValidacao?>;">
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
								<img src="/imagens/seta_filho.gif" />
								<b>Validadores</b>
							</td>
							<td>
							<span id="nome_validador"><? echo $entValidacao['entnome']; ?></span> 
							<a style="cursor:pointer;" onclick="inserirValidador(); return false;"><img src="../imagens/gif_inclui.gif" align="absmiddle"> Validador</a>
							<input type="hidden" name="entid_validador" id="entid_validador" value="<? echo $entValidacao['entid']; ?>">
							</td>
						</tr>
						
						<tr id="linha_evidencia_certificacao" style="display:<?=$displayLinhaCertificacao?>;">
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
								<img src="/imagens/seta_filho.gif" />
								<b>Coletar evidência na Certificação?</b>
							</td>
							<td>
								<input type="radio" value="S" class="opcao_evidencia" id="opcao_evidencia_certificacao_sim" name="opcao_evidencia_certificacao" <?php if($opcaoEvidenciaCertificacao=='t') echo 'checked="checked"'; ?> /> Sim
								&nbsp;&nbsp;
								<input type="radio" value="N" class="opcao_evidencia" id="opcao_evidencia_certificacao_nao" name="opcao_evidencia_certificacao" <?php if(!$opcaoEvidenciaCertificacao || $opcaoEvidenciaCertificacao=='f') echo 'checked="checked"'; ?> /> Não
								&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
								<span id="evidencia_certificacao" style="display:<?=$displayEvidenciaCertificacao?>;">
									<b>Evidência</b>: <?php echo campo_texto('etcevidenciacertificacao','S','S','',30, 500,'','',null,null,null,'id="iclevidenciacertificacao"'); ?>
								</span>
							</td>
						</tr>
						<tr id="linha_certificadores" style="display:<?=$displayLinhaCertificacao?>;">
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
								<img src="/imagens/seta_filho.gif" />
								<b>Certificadores</b>
							</td>
							<td>
							<span id="nome_certificador"><? echo $entCertificacao['entnome']; ?></span> 
							<a style="cursor:pointer;" onclick="inserirCertificador(); return false;"><img src="../imagens/gif_inclui.gif" align="absmiddle"> Certificador</a>
							<input type="hidden" name="entid_certificador" id="entid_certificador" value="<? echo $entCertificacao['entid']; ?>">
							</td>
						</tr>
						<tr bgcolor="#cccccc">
							<td colspan="2" align="center">
								<input type="button" class="botao" name="btSalvar" id="btSalvar" value="Salvar Item do Checklist"">
								<? if($_REQUEST['iclid']) : ?><input type="button" class="botao" name="btNovo" id="btNovo" value="Novo" onclick="window.location='enem.php?modulo=principal/atividade_enem/cadastro_checklist&acao=A&atiid=<? echo $atividade['atiid']; ?>';"><? endif; ?>	
							</td>
						</tr>
					</table>
			<?php endif; ?>
		</td>
	</tr>
	<?php
	$sql = "SELECT
				icl.iclid,
				icl.icldsc,
				--prd.prddsc,
				to_char(icl.iclprazo, 'DD/MM/YYYY') as iclprazo,
				CASE WHEN iclcritico='t' THEN 'Sim' ELSE 'Não' END as iclcritico
			FROM
				projetos.itemchecklist icl
			--INNER JOIN
				--projetos.periodicidade prd ON prd.prdid = icl.prdid
			WHERE
				icl.atiid = ".$atividade['atiid']." 
			ORDER BY 
				icl.iclordem ASC";
	$dados = $db->carregar($sql);
	
	if($dados){
	?>
	<tr>
		<td>
			<table id="tb_itens_checklist" width="100%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem">
				<thead>
					<tr>
					<?php if( !temPerfilSomenteConsulta() ): ?>
						<td valign="top" width="5%" align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" class="title"><strong>Ordem</strong></td>
						<td valign="top" width="5%" align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" class="title"><strong>Alterar/Excluir</strong></td>
					<?php endif; ?>
						<td valign="top" width="2%" align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" class="title"><strong>ID</strong></td>
						<td valign="top" width="10%" align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" class="title"><strong>Descrição</strong></td>
						<!--<td valign="top" width="5%" align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" class="title"><strong>Peridiocidade</strong></td>-->
						<td valign="top" width="2.5%" align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" class="title"><strong>Prazo</strong></td>
						<td valign="top" width="2.5%" align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" class="title"><strong>Crítico</strong></td>
						<td valign="top" width="10%" align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" class="title"><strong>Execução</strong></td>
						<td valign="top" width="15%" align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" class="title"><strong>Executor(es)</strong></td>
						<td valign="top" width="10%" align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" class="title"><strong>Validação</strong></td>
						<td valign="top" width="15%" align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" class="title"><strong>Validador(es)</strong></td>
						<td valign="top" width="10%" align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" class="title"><strong>Certificação</strong></td>
						<td valign="top" width="15%" align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);" class="title"><strong>Certificador(es)</strong></td>
					</tr>
				</thead>
				<tbody>
					<?php 
					for($i=0; $i<count($dados); $i++)
					{
						$cor = ($i%2) ? '#e0e0e0' : '#fbfbfb';
					?>
					<tr id="<?php echo $dados[$i]["iclid"]; ?>" bgcolor="<?=$cor?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
					<?php if( !temPerfilSomenteConsulta() ): ?>
						<td align="center">
						<?php if( count($dados) > 1 ): ?>
							<?php if( $i == 0 ): ?>
							<img src="/imagens/seta_baixo.gif" border="0" style="cursor:pointer;" onclick="alterarOrdem(<?=($i+1)?>, 'baixo');" />
							<img src="/imagens/seta_cimad.gif" border="0" style="cursor:pointer;" /> 
							<?php elseif( $i == (count($dados) - 1) ): ?>
							<img src="/imagens/seta_baixod.gif" border="0" style="cursor:pointer;" />
							<img src="/imagens/seta_cima.gif" border="0" style="cursor:pointer;" onclick="alterarOrdem(<?=($i+1)?>, 'cima');" />
							<?php else: ?>
							<img src="/imagens/seta_baixo.gif" border="0" style="cursor:pointer;" onclick="alterarOrdem(<?=($i+1)?>, 'baixo');" />
							<img src="/imagens/seta_cima.gif" border="0" style="cursor:pointer;" onclick="alterarOrdem(<?=($i+1)?>, 'cima');" />
							<?php endif; ?>
						<?php else: ?>
							<img src="/imagens/seta_baixod.gif" border="0" style="cursor:pointer;" />
							<img src="/imagens/seta_cimad.gif" border="0" style="cursor:pointer;" />
						<?php endif; ?>
						</td>
						<td align="center">
							<? if($permissao) : ?>
							<img src="/imagens/alterar.gif" border="0" style="cursor:pointer;" onclick="alterarItemChecklist(<?php echo $dados[$i]["iclid"]; ?>);" />
							<img src="/imagens/excluir.gif" border="0" style="cursor:pointer;" onclick="excluirItemChecklist(<?php echo $dados[$i]["iclid"]; ?>);" />
							<? else : ?>
							&nbsp;
							<? endif; ?>
						</td>
					<?php endif; ?>
						<td align="center">
							<?php echo $dados[$i]["iclid"]; ?>
						</td>
						<td align="center">
							<?php echo $dados[$i]["icldsc"]; ?>
						</td>
						<!--<td align="center">
							<?php //echo $dados[$i]["prddsc"]; ?>
						</td>-->
						<td align="center">
							<?php echo $dados[$i]["iclprazo"]; ?>
						</td>
						<td align="center">
							<?php echo $dados[$i]["iclcritico"]; ?>
						</td>
						<td align="center">
						<?php
						$sql = "SELECT etcopcaoevidencia,etcevidencia FROM projetos.etapascontrole WHERE iclid = ".$dados[$i]["iclid"]." AND tpvid = 1";
						$execucao = $db->carregar($sql);
						
						if($execucao)
						{
							$str = "<b>Sim</b>";
							
							if( $execucao[0]['etcopcaoevidencia'] == 't' )
							{
								$str .= "<br />Com evidências " . "(" . $execucao[0]['etcevidencia'] . ")";
							}
							else
							{
								$str .= "<br />Sem evidências";
							}
							
							echo $str;
						}
						else
						{
							echo "<b>Não</b>";
						}
						?>
						</td>
						<td align="center">
						<?php
						$sql = "SELECT ent.entnome FROM projetos.checklistentidade cle INNER JOIN entidade.entidade ent ON ent.entid = cle.entid WHERE cle.iclid = ".$dados[$i]["iclid"]." AND cle.tpvid = 1";
						$entidadeExecucao = $db->carregarColuna($sql);
						
						$entnomes = array();
						for($k=0; $k<count($entidadeExecucao); $k++)
						{
							$entnomes[] = $entidadeExecucao[$k];
						}
						
						echo implode('<br />',$entnomes);
						?>
						</td>
						<td align="center">
						<?php
						$sql = "SELECT etcopcaoevidencia,etcevidencia FROM projetos.etapascontrole WHERE iclid = ".$dados[$i]["iclid"]." AND tpvid = 2";
						$validacao = $db->carregar($sql);
						
						if($validacao)
						{
							$str = "<b>Sim</b>";
							
							if( $validacao[0]['etcopcaoevidencia'] == 't' )
							{
								$str .= "<br />Com evidências " . "(" . $validacao[0]['etcevidencia'] . ")";
							}
							else
							{
								$str .= "<br />Sem evidências";
							}
							
							echo $str;
						}
						else
						{
							echo "<b>Não</b>";
						}
						?>
						</td>
						<td align="center">
						<?php
						$sql = "SELECT ent.entnome FROM projetos.checklistentidade cle INNER JOIN entidade.entidade ent ON ent.entid = cle.entid WHERE cle.iclid = ".$dados[$i]["iclid"]." AND cle.tpvid = 2";
						$entidadeValidacao = $db->carregarColuna($sql);
						
						$entnomes = array();
						for($k=0; $k<count($entidadeValidacao); $k++)
						{
							$entnomes[] = $entidadeValidacao[$k];
						}
						
						echo implode('<br />',$entnomes);
						?>
						</td>
						<td align="center">
						<?php
						$sql = "SELECT etcopcaoevidencia,etcevidencia FROM projetos.etapascontrole WHERE iclid = ".$dados[$i]["iclid"]." AND tpvid = 3";
						$certificacao = $db->carregar($sql);
						
						if($certificacao)
						{
							$str = "<b>Sim</b>";
							
							if( $certificacao[0]['etcopcaoevidencia'] == 't' )
							{
								$str .= "<br />Com evidências " . "(" . $certificacao[0]['etcevidencia'] . ")";
							}
							else
							{
								$str .= "<br />Sem evidências";
							}
							
							echo $str;
						}
						else
						{
							echo "<b>Não</b>";
						}
						?>
						</td>
						<td align="center">
						<?php
						$sql = "SELECT ent.entnome FROM projetos.checklistentidade cle INNER JOIN entidade.entidade ent ON ent.entid = cle.entid WHERE cle.iclid = ".$dados[$i]["iclid"]." AND cle.tpvid = 3";
						$entidadeCertificacao = $db->carregarColuna($sql);
						
						$entnomes = array();
						for($k=0; $k<count($entidadeCertificacao); $k++)
						{
							$entnomes[] = $entidadeCertificacao[$k];
						}
						
						echo implode('<br />',$entnomes);
						?>
						</td>
					</tr>
					<?php } ?>
				</tbody>
			</table>
			</form>
		</td>
	</tr>
	<?php } ?>
</table>