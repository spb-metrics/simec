<?php

function calculaDataFim() {
	global $db;
	
	if($_POST['atidatainicio']) {
		
		$sql = "SELECT date '".formata_data_sql($_POST['atidatainicio'])."' + integer '".(($_POST['atiduracao'])?$_POST['atiduracao']:'0')."' as atidatafim";
		echo formata_data($db->pegaUm($sql));
		
	}
	
	exit;
}


if(!$_REQUEST['atiid'])
{
	header("Location: enem.php?modulo=principal/atividade_enem/arvore&acao=A");
}

if($_POST['requisicao'])
{
	$arrResposta = $_POST['requisicao']();
}

if( $_POST['ajaxResponsavelExcluir'] )
{
	if( $_POST['tipo'] == 'responsavel' )
		$funid = FUNID_RESPONSAVEL_ENEM;
	elseif( $_POST['tipo'] == 'executor' )
		$funid = FUNID_EXECUTOR_ENEM;
	elseif( $_POST['tipo'] == 'validador' )
		$funid = FUNID_VALIDADOR_ENEM;
	elseif( $_POST['tipo'] == 'certificador' )
		$funid = FUNID_CERTIFICADOR_ENEM;
		
	$sql = "UPDATE entidade.funcaoentidade SET fuestatus = 'I' WHERE entid = ".$_POST['entid']." AND funid = ".$funid;
	$db->executar($sql);
	if( !$db->commit() ) die('erro');
	
	$sql = "SELECT count(1) FROM entidade.funcaoentidade WHERE entid = ".$_POST['entid']." AND fuestatus = 'A'";
	$existeFuncaoEntidade = $db->pegaUm($sql);
	
	if( !$existeFuncaoEntidade )
	{
		$sql = "UPDATE projetos.responsavelatividade SET rpastatus = 'I' WHERE atiid = ".$_POST['atiid']." AND entid = ".$_POST['entid'];
		$db->executar($sql);
		if( !$db->commit() ) die('erro');
	}
	
	die('ok');
}

include APPRAIZ . 'includes/cabecalho.inc';
echo '<br/>';

$db->cria_aba( $abacod_tela, $url, '&atiid=' . $_REQUEST['atiid'] );
montar_titulo_projeto( $atividade['atidescricao'] );

//Carregar dados da Atividade
if( $_REQUEST['atiid'] )
{
	$sql = "select 
				atiid,
				atidescricao,
				atidetalhamento,
				atidatainicio,
				atiduracao,
				nvcid
			from
				projetos.atividade 
			where
				atiid = {$_REQUEST['atiid']}
			and
				atistatus = 'A'";
	
	$arrDados = $db->pegaLinha($sql);
}

//Regras de liberação de edição / inclusão
$somenteLeitura = "S";

if( temPerfilSomenteConsulta() || temPerfilExecValidCertif() ) $somenteLeitura = "N";

?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script>
<!--

jQuery.noConflict();

function calculaDataFim() {

	if(document.getElementById('atidatainicio').value.length > 0 && document.getElementById('atiduracao').value.length > 0) {
		jQuery.ajax({
	   		type: "POST",
	   		url: "enem.php?modulo=principal/atividade_enem/dados_atividade&acao=A&atiid=<? echo $_REQUEST['atiid']; ?>",
	   		data: "requisicao=calculaDataFim&atidatainicio="+document.getElementById('atidatainicio').value+"&atiduracao="+document.getElementById('atiduracao').value,
	   		success: function(msg){
	   			jQuery("#atidatafim").val(msg);
	   		}
	 		});
	 		
 	}
}


function salvarAtv()
{
	var erro = 0;
	jQuery("[class~=obrigatorio]").each(function() { 
		if(this.value.length == 0){
			erro = 1;
			alert('Favor preencher todos os campos obrigatórios!');
			this.focus();
			return false;
		}
	});
	if(erro == 0){
		jQuery("#formulario_atividade").submit();
	}	
}

function inserirResponsavel()
{
	janela("enem.php?modulo=principal/atividade_enem/inserir_responsavel_processo&acao=A&atiid=<?=$_REQUEST['atiid']?>",600,580,"att_resp");
}

function inserirExecutor()
{
	janela("enem.php?modulo=principal/atividade_enem/inserir_executor_processo&acao=A&atiid=<?=$_REQUEST['atiid']?>",600,580,"att_exec");
}

function inserirValidador()
{
	janela("enem.php?modulo=principal/atividade_enem/inserir_validador_processo&acao=A&atiid=<?=$_REQUEST['atiid']?>",600,580,"att_valid");
}

function inserirCertificador()
{
	janela("enem.php?modulo=principal/atividade_enem/inserir_certificador_processo&acao=A&atiid=<?=$_REQUEST['atiid']?>",600,580,"att_cert");
}

function RemoveLinha( idContato, tipo )
{
	if( confirm('Deseja realmente excluir?') )
	{
		var req = new Ajax.Request('enem.php?modulo=principal/atividade_enem/dados_atividade&acao=A',
		{
	        method:     'post',
	        parameters: '&ajaxResponsavelExcluir=1&atiid=<?=$_REQUEST['atiid']?>&entid='+idContato+'&tipo='+tipo,
	        onComplete: function (res)
	        {
				if( res.responseText == 'ok' )
				{
					//var index = document.getElementById('linha_'+tipo+'_'+idContato).rowIndex;
					//var table = document.getElementById(tipo+'contato');
					//table.deleteRow(index);
					alert('Excluído com sucesso.');
					window.location.href = window.location.href;
				}
				else
				{
					alert("Ocorreu um erro.");
				}
	        }
	  	});
	}
}

-->
</script>

<style>
	.SubtituloTabela{background-color:#cccccc}
	.negrito{font-weight:bold}
	.normal{font-weight:normal}
	.center{text-align: center;}
	.direita{text-align: right;}
</style>

<form name="formulario_atividade" id="formulario_atividade"  method="post" action="" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubtituloDireita" width="25%" >Nome</td>
			<td><?php echo campo_texto( 'atidescricao', 'S', $somenteLeitura, '', 66, '', '', '', 'left', '', 0,'','',$arrDados['atidescricao']); ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" width="25%" >Descrição(Normas / Procedimentos)</td>
			<td><?php echo campo_textarea( 'atidetalhamento', 'S', $somenteLeitura, '', 70, 1, '', '', '', '', 0,'',$arrDados['atidetalhamento']); ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Data Início</td>
			<td><?php echo campo_data2( 'atidatainicio', 'S', $somenteLeitura, '', 'S' ,'','',$arrDados['atidatainicio'],'calculaDataFim();'); ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Nível de Criticidade</td>
			<td>
				<?php $sql = "	select
									nvcid as codigo,
									nvcdsc as descricao
								from
									projetos.nivelcriticidade
								where
									nvcstatus = 'A'
								order by
									nvcdsc" ?>
				<?php 
				$db->monta_combo("nvcid", $sql, $somenteLeitura , "Selecione...", '', '', '', '', 'N','','',$arrDados['nvcid']); ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Duração</td>
			<td><?php echo campo_texto( 'atiduracao', 'S', $somenteLeitura, '', 4, '', '', '', '', '', 0,'id="atiduracao"','calculaDataFim();',$arrDados['atiduracao']); ?></td>
		</tr>
		<tr>
			<?

			if($arrDados['atidatainicio'] && $arrDados['atiduracao']) {
				$sql = "SELECT date '".$arrDados['atidatainicio']."' + integer '".$arrDados['atiduracao']."' as atidatafim";
				$atidatafim = $db->pegaUm($sql);
			}
			?>
			<td class="SubtituloDireita" >Data Fim</td>
			<td><?php echo campo_data2( 'atidatafim', 'N', 'N', '', 'S' ,'','',$atidatafim); ?></td>
		</tr>

		<tr>
			<td colspan="2" class="SubtituloTabela center negrito" >
				<input type="hidden" name="atiid" value="<?php echo $arrDados['atiid'] ?>" />
				<input type="hidden" name="requisicao" value="salvarDadosAtividade" />
				<?php if( !temPerfilSomenteConsulta() && !temPerfilExecValidCertif() ): ?>
				<input type="button" value="Salvar" onclick="salvarAtv()" />
				<?php else: ?>
				<input type="button" value="Salvar" disabled="disabled" />
				<?php endif; ?>
				<input type="button" value="Voltar" onclick="javascript:history.back(-1);" />
			</td>
		</tr>
	</table>
</form>

<?php if($arrResposta['msg']):?>
	<script>
		alert('<?php echo $arrResposta['msg']?>')
	</script>
<?php endif;?>