<?php

function cancelaUltimaDeclaracao( $gpdid )
{
	global $db;
	
	$sql = "SELECT
				dclid
			FROM
				obras.declaracao
			WHERE
				dclstatus = 'A'
				AND stdid = ".Declaracao::SITUACAO_DECLARACAO_GERADA."
				AND gpdid = ".$gpdid;
	$dclid = $db->pegaUm($sql);
	
	if( $dclid )
	{
		$sql = "UPDATE 
					obras.declaracao
				SET 
					stdid = ".Declaracao::SITUACAO_DECLARACAO_CANCELADA."
				WHERE 
					dclid = {$dclid}";
		$db->executar($sql);
		$db->commit();
	}
}

if( $_POST['ajaxCancelarDeclaracao'] )
{
	$sql = "UPDATE obras.declaracao SET stdid = ".Declaracao::SITUACAO_DECLARACAO_CANCELADA." WHERE dclid = ".$_POST['dclid'];
	$db->executar($sql);
	
	if( $db->commit() )
		die('ok');
	else
		die('erro');
}

if( $_POST['ajaxExisteDeclaracao'] )
{
	$sql = "SELECT
				dclid
			FROM
				obras.declaracao
			WHERE
				dclstatus = 'A'
				AND stdid = ".Declaracao::SITUACAO_DECLARACAO_GERADA."
				AND gpdid = ".$_POST['gpdid'];
	$dclid = $db->pegaUm($sql);
	
	if( $dclid )
		die('existe');
	else
		die('nao_existe');
}

if( $_POST['ajaxListaDeclaracoes'] )
{
	$dclidNotIn = "";
	$existeDeclaracaoGerada = $db->pegaUm("SELECT count(1) FROM obras.declaracao WHERE gpdid = ".$_POST['gpdid']." AND dclstatus = 'A' AND stdid = ".Declaracao::SITUACAO_DECLARACAO_GERADA."");
	
	if( $existeDeclaracaoGerada == 0 )
	{
		$ultimaDeclaracao = $db->pegaUm("SELECT max(dclid) FROM obras.declaracao WHERE gpdid = ".$_POST['gpdid']." AND dclstatus = 'A'");
		
		if($ultimaDeclaracao)
		{
			$dclidNotIn = " AND dcl.dclid not in (".$ultimaDeclaracao.") ";
		}
	}
	
	$sql = "SELECT
				'<center><img src=\'../imagens/ico_html.gif\' width=\'19\' height=\'19\' title=\'Visualizar Declara��o\' style=\'cursor:pointer; margin-left:3px;\' onclick=\'visualizarDeclaracao('||dcl.dclid||');\'><center>' as html,
				dcl.dclid,
				dcl.orsid,
				'<center>' || to_char(dcl.dcldtemissao, 'DD/MM/YYYY' ) || '</center>' AS dcldtemissao,
				dcl.dclvalor,
				'<center>' || usu.usunome || '</center>' AS usunome,
				'<center>' || std.stddsc || '</center>' AS stddsc
			FROM
				obras.declaracao dcl
			INNER JOIN
				seguranca.usuario usu ON usu.usucpf = dcl.usucpf
			INNER JOIN
				obras.situacaodeclaracao std ON std.stdid = dcl.stdid
			WHERE
				dcl.stdid = ".Declaracao::SITUACAO_DECLARACAO_CANCELADA."
				AND dcl.gpdid = ".$_POST['gpdid']."
				AND dcl.dclstatus = 'A'
				".$dclidNotIn."
			ORDER BY
				dcl.dclid DESC";
	
	$cabecalho = array("HTML", "N� da Declara��o", "N� da OS", "Data da Emiss�o", "Valor R$", "Criado Por", "Situa��o");
	
	die( $db->monta_lista_simples($sql, $cabecalho, 20, 10, 'N', '90%', 'N', false, false, false, false) );
}

if( $_POST['ajaxNovaDeclaracao'] )
{
	cancelaUltimaDeclaracao($_POST['gpdid']);
	$obDeclaracao = new DeclaracaoController();
	$dclid = $obDeclaracao->salvarDeclaracao();
	
	if( $dclid )
		die('ok');
	else
		die('erro');
}

include APPRAIZ . "includes/classes/PaginacaoAjax.class.inc";

$obOS = new OSController();

include APPRAIZ . "includes/cabecalho.inc";
print "<br/>";
$db->cria_aba( $abacod_tela, $url, $parametros );
monta_titulo( "Declara��o de Conclus�o de Supervis�o", "" );

extract($_POST);

?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<!--<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>-->
<!--<link rel="stylesheet" href="http://ajax.googleapis.com/ajax/libs/jqueryui/1.8.13/themes/base/jquery-ui.css" type="text/css" media="all" />-->
<link rel="stylesheet" href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" type="text/css" media="all" />
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script type="text/javascript" src="/includes/JQuery/jquery-ui-1.8.4.custom.min.js"></script>

<style>
.ui-widget
{
	font-size: 10px;
}
</style>

<script type="text/javascript">
$(document).ready(function()
{
	$("#dialog").dialog(
	{
    	autoOpen: false,
      	modal: true
    });
});
</script>

<!-- Alerta de nova declara��o -->
<div id="dialog" title="Gerar nova declara��o" style="display:none;">
	<center><img border="0" src="../imagens/atencao.png" /><b>Aten��o!</b></center>
	<br />
	Ao gerar uma nova declara��o, o sistema ir� cancelar a anterior.
	<br />
	Deseja prosseguir?
</div>

<!-- Alerta de exclus�o da declara��o -->
<div id="dialog_cancelar" title="Excluir declara��o" style="display:none;">
	<center><img border="0" src="../imagens/atencao.png" /><b>Aten��o!</b></center>
	<br />
	Ao cancelar esta declara��o, o grupo ficar� sem declara��o vigente.
	<br />
	Deseja prosseguir?
</div>

<form id="formPesquisaDeclaracao" method="post">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td align='right' class="SubTituloDireita">N� do Grupo:</td>
			<td>
			<?=campo_texto('gpdid','N','S','',10,14,'[#]','');?>
		    </td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Empresa:</td>
			<td>
			<?php 
			$db->monta_combo("epcid", $obOS->buscaDadosEmpresa(), "S", "Todas", '', '', '', '', 'N','epcid');
			?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" width="30%">Data de Emiss�o:</td>
		    <td>
			<?= campo_data2( 'dcldtemissaoini','N', 'S', '', 'N' ); ?>
			&nbsp;at�&nbsp;
			<?= campo_data2( 'dcldtemissaofim','N', 'S', '', 'N' ); ?>
		    </td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" width="30%">UF:</td>
		    <td>
	    	<?php
			$sql = "SELECT 
						estuf as descricao,
						estuf as codigo
					FROM
						territorios.estado
					ORDER BY
						descricao;";
			
			$db->monta_combo("estuf", $sql, "S", "Todas", '', '', '', '', 'N','estcod');
			?>
		    </td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" width="30%">Mostrar grupos que possuem declara��o:</td>
		    <td>
		    	<input type="radio" name="possui_declaracao" value="S" <?=(($_REQUEST['possui_declaracao']=='S') ? 'checked="checked"' : '')?> /> Sim
		    	<input type="radio" name="possui_declaracao" value="N" <?=(($_REQUEST['possui_declaracao']=='N') ? 'checked="checked"' : '')?> /> N�o
		    	<input type="radio" name="possui_declaracao" value="T" <?=((!$_REQUEST['possui_declaracao'] || $_REQUEST['possui_declaracao']=='T') ? 'checked="checked"' : '')?> /> Todos 
		    </td>
		</tr>
		<tr bgcolor="#CCCCCC">
			<td>&nbsp;</td>
		    <td>
		    	<input type="submit" name="btPesquisar" value="Pesquisar">
		    </td>
		</tr>      
	</table>
</form>

<center>
	<div style="width:95%; text-align: left;">
		<div id="listaDeclaracao">
			<?php DeclaracaoController::lista( array("filtro" => $_POST, 'nrRegPorPagina' => 20 ) ); ?>
		</div>
	</div>
</center>

<script>

function alterarDeclaracao(dclid)
{
	janela('?modulo=principal/supervisao/alterarDeclaracao&acao=A&dclid=' + dclid , 450, 350, 'Declaracao');
	return;
}

function cancelarDeclaracao(dclid)
{
	$( "#dialog_cancelar" ).dialog(
	{
		resizable: false,
		height:170,
		modal: true,
		buttons: {
			"Sim": function()
			{
				$.ajax({
					   type: "POST",
					   url: "obras.php?modulo=principal/supervisao/listaDeclaracoes&acao=A",
					   data: "ajaxCancelarDeclaracao=1&dclid="+dclid,
					   success: function(msg)
					   {
						   if( msg == 'ok' )
						   {
							   alert('Declara��o cancelada com sucesso.');
							   window.location.href = window.location.href;
							   $( this ).dialog( "close" );
						   }
						   else
						   {
								alert('Ocorreu um erro ao cancelar a Declara��o.');
								$( this ).dialog( "close" );
						   }
					   }
					 });
			},
			"N�o": function()
			{
				$( this ).dialog( "close" );
			}
		}
	});

	$("#dialog_cancelar").dialog("open");
}

function visualizarDeclaracoes(gpdid)
{
	if( $('#linha_'+gpdid).css('display') == 'none' )
	{
		$('#img_'+gpdid).attr('src', '../imagens/menos.gif');
		$('#img_'+gpdid).attr('title', 'Fechar lista das declara��es n�o vigentes.');
		$('#linha_'+gpdid).show();
		
		if( $('#linha_'+gpdid).children().html() == '' )
		{
			$('#linha_'+gpdid).children().html('<img border="0" src="../imagens/carregando.gif" /><br />Carregando...');
			
			$.ajax({
			   type: "POST",
			   url: "obras.php?modulo=principal/supervisao/listaDeclaracoes&acao=A",
			   data: "ajaxListaDeclaracoes=1&gpdid="+gpdid,
			   success: function(msg)
			   {
				   //msg = msg + '<div style="width:90%;text-align:left;"><input type="button" value="Inserir nova declara��o" onclick="novaDeclaracao('+gpdid+');" /></div>';
			   	   $('#linha_'+gpdid).children().html(msg);
			   }
			 });
		}
	}
	else
	{
		$('#img_'+gpdid).attr('src', '../imagens/mais.gif');
		$('#img_'+gpdid).attr('title', 'Visualizar as declara��es n�o vigentes.');
		$('#linha_'+gpdid).hide();
	}
}

function novaDeclaracao(gpdid)
{
	var alerta = false;
	
	$.ajax({
	   type: "POST",
	   url: "obras.php?modulo=principal/supervisao/listaDeclaracoes&acao=A",
	   data: "ajaxExisteDeclaracao=1&gpdid="+gpdid,
	   async: false,
	   success: function(msg)
	   {
		   if( msg == 'existe' )
		   {
			   alerta = true;
		   }
	   }
	 });

	if( alerta )
	{
		$( "#dialog" ).dialog(
		{
			resizable: false,
			height:170,
			modal: true,
			buttons: {
				"Sim": function()
				{
					$.ajax({
						   type: "POST",
						   url: "obras.php?modulo=principal/supervisao/listaDeclaracoes&acao=A",
						   data: "ajaxNovaDeclaracao=1&gpdid="+gpdid,
						   success: function(msg)
						   {
							   if( msg == 'ok' )
							   {
								   alert('Nova Declara��o gerada com sucesso.');
								   window.location.href = window.location.href;
								   $( this ).dialog( "close" );
							   }
							   else
							   {
									alert('Ocorreu um erro ao gerar a nova Declara��o.');
									$( this ).dialog( "close" );
							   }
						   }
						 });
				},
				"N�o": function()
				{
					$( this ).dialog( "close" );
				}
			}
		});
	
		$("#dialog").dialog("open");
	}
	else
	{
		$.ajax({
		   type: "POST",
		   url: "obras.php?modulo=principal/supervisao/listaDeclaracoes&acao=A",
		   data: "ajaxNovaDeclaracao=1&gpdid="+gpdid,
		   success: function(msg)
		   {
			   if( msg == 'ok' )
			   {
				   alert('Nova Declara��o gerada com sucesso.');
				   window.location.href = window.location.href;
			   }
			   else
			   {
					alert('Ocorreu um erro ao gerar a nova Declara��o.');
			   }
		   }
		 });
	}
}

function visualizarDeclaracao(dclid)
{
	janela('?modulo=principal/supervisao/emitirDeclaracao&acao=A&dclid=' + dclid , 900, 600, 'Declaracao');
	return;
}

function envia_email_declaracao_empresa(dclid)
{
	e = "obras.php?modulo=sistema/geral/envia_email_declaracao_empresa&acao=A&dclid=" + dclid;
	window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=590,height=490");
}

</script>