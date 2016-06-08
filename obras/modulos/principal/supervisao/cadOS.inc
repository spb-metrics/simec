<?php
/* REGRAS DEFINIDAS PELO MÁRIO - 04-05-2010
 * 
 * Somente um grupo por OS
 * Somente grupos onde NÃO há OS atribuída
 * Somente grupos que possuam uma rota aprovada (pela regra só poderá ter uma rota aprovada)
 * 
 * REGRAS DEFINIDAS PELO MÁRIO - 08-06-2010
 * 
 * Retirar o campo "Nº Protec"
 */

function montaListaOS($gpdid){
	global $db;
	
	// cabeçalho
	$html = '<div class="scrollTable">
				<table cellspacing="0" cellpadding="2" border="0" bgcolor="" align="center" width="95%" style="width: 100%;" class="listagem">
					<tbody>
						<tr bgcolor="#e9e9e9">
							<td align="center" valign="middle" rowspan="2" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255); font-weight: bold;" class="coluna75px">Ação</td>
							<td align="center" valign="middle" rowspan="2" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255); font-weight: bold;" class="coluna75px">OS (HTML)</td>
							<td align="center" valign="middle" rowspan="2" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255); font-weight: bold;" class="coluna75px">Nº OS</td>
							<td align="center" valign="middle" rowspan="2" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255); font-weight: bold;" class="coluna75px">Data Emissão</td>
							<td align="center" valign="middle" rowspan="2" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255); font-weight: bold;" class="coluna75px">UF</td>
							<td align="center" valign="middle" rowspan="2" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255); font-weight: bold;" class="coluna150px">Grupo / Empresa</td>
							<td align="center" valign="middle" colspan="2" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255); font-weight: bold;" class="coluna75px">Execução</td>
							<td align="center" valign="middle" rowspan="2" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255); font-weight: bold;" class="coluna75px">Data de Finalização da Supervisão</td>
							<td align="center" valign="middle" rowspan="2" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255); font-weight: bold;" class="coluna75px">Situação</td>
							<td align="center" valign="middle" rowspan="2" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255); font-weight: bold;" class="coluna75px">Valor (R$)</td>
							<td align="center" valign="middle" rowspan="2" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255); font-weight: bold;" class="coluna150px">Cadastrante</td>
						</tr>
						<tr bgcolor="#e9e9e9">
							<td align="center" valign="middle" rowspan="1" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255); font-weight: bold;" class="coluna75px">Início</td>
							<td align="center" valign="middle" rowspan="1" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255); font-weight: bold;" class="coluna75px">Término</td>
						</tr>
					</tbody>
				</table>';
	
	// consulta
	// <img onclick=\"location.href=\'?modulo=principal/supervisao/cadOS&acao=A&orsid='||orsid||'\'\" style=\"cursor: pointer; margin-left: 3px;\" title=\"Editar OS\" src=\"../imagens/alterar.gif\">
	$sql = "SELECT 
				
				'<center>
					<img onclick=\"excluir('||orsid||', this);\" style=\"cursor: pointer; margin-left: 3px;\" title=\"Excluir OS\" src=\"../imagens/excluir.gif\">
				</center>' as acao,
				'<img width=\"19\" height=\"19\" onclick=\"visualizarOS('||orsid||');\" style=\"cursor: pointer; margin-left: 3px;\" title=\"Visualizar OS\" src=\"../imagens/ico_html.gif\">' as visualizar,
				orsid,
				to_char(orsdtemissao, 'DD/MM/YYYY' ) AS orsdtemissao,
				gd.estuf,
				gd.gpdid ||' / '|| e.entnome AS empresa,
				to_char(orsdtinicioexecucao, 'DD/MM/YYYY' ) AS orsdtinicioexecucao,
				to_char(orsdtfinalexecucao, 'DD/MM/YYYY' ) AS orsdtfinalexecucao,
				COALESCE(to_char(htddata, 'DD/MM/YYYY' ), '-') AS htddata,
				so.stodsc,
				os.orsvalor AS valor,
				u.usunome,
				gd.gpdid,
				oe.empvlrsetec,
				oe.empvlrfnde,
				oe.empvlrsesu
			FROM 
			   obras.ordemservico os   
			JOIN obras.situacaoos so ON (so.stoid = os.stoid) 
			JOIN obras.grupodistribuicao gd ON (gd.gpdid = os.gpdid AND gpdstatus = 'A') 
			LEFT JOIN (SELECT
					MAX(hstid),
					docid, 
					htddata 
					FROM 
						workflow.historicodocumento
					WHERE
						aedid = 378
					GROUP BY
						docid, htddata
					) hd ON hd.docid = gd.docid
			JOIN obras.empresacontratada ec ON (ec.epcid = gd.epcid)
			JOIN entidade.entidade e ON (ec.entid = e.entid)
			JOIN obras.rotas r ON (r.gpdid  = gd.gpdid
							       AND r.rotstatus = 'A'
							       AND strid = 1)
			JOIN seguranca.usuario u ON (u.usucpf  = os.usucpf)
			JOIN obras.empenho oe ON oe.estuf = gd.estuf
			WHERE (os.orsstatus = 'A') AND (gd.gpdid = $gpdid) AND so.stoid IN (1, 2, 3)
			ORDER BY os.orsid DESC";
	
	$dados = $db->carregar($sql);
	
	// pegando os códigos do sesu, setec e fnde
	$sql = "SELECT 
				oe.empvlrsetec,
				oe.empvlrfnde,
				oe.empvlrsesu
			FROM 
				obras.grupodistribuicao ogd
			INNER JOIN
				obras.empenho oe ON oe.estuf = ogd.estuf
			WHERE 
				ogd.gpdid = {$gpdid} 
				AND ogd.gpdstatus = 'A'";
	
	$codigos = $db->pegaLinha($sql);
	$hidden = '';
	
	if(is_array($codigos)){
		$hidden = '<input type="hidden" id="setec" value="'.$codigos['empvlrsetec'].'">
				   <input type="hidden" id="fnde" value="'.$codigos['empvlrfnde'].'">
				   <input type="hidden" id="sesu" value="'.$codigos['empvlrsesu'].'">';
	}
	if ($gpdid > 0 && is_array($dados)) {
		$html .= '<div class="scroller">
						<table class="tabela">
							<tbody>';
		foreach ($dados as $chave => $dado) {
			
			if ($chave%2) {
				$linha = '<tr bgcolor="#FFFFFF" onmouseout="this.bgColor=\'#FFFFFF\'" onmouseover="this.bgColor=\'#ffffcc\'">';
			}else{
				$linha = '<tr bgcolor="#f7f7f7" onmouseout="this.bgColor=\'#f7f7f7\'" onmouseover="this.bgColor=\'#ffffcc\'">';
			}
			
			$html .= $linha.'
						<td class="coluna75px"><center>'.$dado['acao'].'</center></td>
						<td class="coluna75px"><center>'.$dado['visualizar'].'</center></td>
						<td class="coluna75px"><center>'.$dado['orsid'].'</center></td>
						<td class="coluna75px"><center>'.$dado['orsdtemissao'].'</center></td>
						<td class="coluna75px"><center>'.$dado['estuf'].'</center></td>
						<td class="coluna150px">'.$dado['empresa'].'</td>
						<td class="coluna75px"><center>'.$dado['orsdtinicioexecucao'].'</center></td>
						<td class="coluna75px"><center>'.$dado['orsdtfinalexecucao'].'</center></td>
						<td class="coluna75px"><center>'.$dado['htddata'].'</center></td>
						<td class="coluna75px"><center>'.$dado['stodsc'].'</center></td>
						<td class="coluna75px"><center>'.number_format($dado['valor'] ,2,',','.').'</center></td>
						<td class="coluna150px">'.$dado['usunome'].'</td>
					  </tr>';
		}
		
		return $html.'</table></div></div>'.$hidden;
		
	}else{
		return '<div style="color: rgb(204, 0, 0);">Não foram encontrados Registros.</div>'.$hidden;
	}
	
}

function cancelaUltimaOS($gpdid){
	global $db;
	
	$sql = "SELECT 
				orsid
			FROM
			   obras.ordemservico os   
			JOIN obras.situacaoos so ON (so.stoid = os.stoid) 
			JOIN obras.grupodistribuicao gd ON (gd.gpdid = os.gpdid AND gpdstatus = 'A') 
			LEFT JOIN (SELECT
					MAX(hstid),
					docid, 
					htddata 
					FROM 
						workflow.historicodocumento
					WHERE
						aedid = 378
					GROUP BY
						docid, htddata
					) hd ON hd.docid = gd.docid
			JOIN obras.empresacontratada ec ON (ec.epcid = gd.epcid)
			JOIN entidade.entidade e ON (ec.entid = e.entid)
			JOIN obras.rotas r ON (r.gpdid  = gd.gpdid
							       AND r.rotstatus = 'A'
							       AND strid = 1)
			JOIN seguranca.usuario u ON (u.usucpf  = os.usucpf)
			JOIN obras.empenho oe ON oe.estuf = gd.estuf
			WHERE (os.orsstatus = 'A') AND (gd.gpdid = {$gpdid}) AND (so.stoid = 1)
			ORDER BY os.orsdtemissao ASC limit 1";
	
	$orsid = $db->pegaUm($sql);
	
	if($orsid){
		$sql = "UPDATE 
					obras.ordemservico
				SET 
					stoid = 3
				WHERE 
					orsid = {$orsid}";
		$db->carregar($sql);
		$db->commit();
	}
	
}

// verifica se existe declaração gerada para a OS
if( $_POST['ajaxExisteDeclaracao'] )
{
	$sql = "SELECT 
				count(1) 
			FROM 
				obras.declaracao 
			WHERE 
				dclstatus = 'A' 
				AND stdid = ".Declaracao::SITUACAO_DECLARACAO_GERADA." 
				AND orsid = ".$_POST['orsid'];
	$existeDecl = $db->pegaUm($sql);
	
	if( $existeDecl > 0 )
		die('existe');
	else
		die('nao_existe');
}

// cancela a declaração associada a OS
if( $_POST['ajaxCancelarDeclaracao'] )
{
	$sql = "UPDATE 
				obras.declaracao 
			SET 
				stdid = ".Declaracao::SITUACAO_DECLARACAO_CANCELADA." 
			WHERE 
				dclstatus = 'A' 
				AND stdid = ".Declaracao::SITUACAO_DECLARACAO_GERADA." 
				AND orsid = ".$_POST['orsid'];
	$db->executar($sql);
	
	if( $db->commit() )
		die('ok');
	else
		die('erro');
}

// A deleção é feita por ajax, por isso não é feito o refresh na página.
if ( $_POST['operacao'] == 'excluir' && $_POST['orsid'] ){
	$obOS->deletaOS( $_POST['orsid'] );	
//	$obOS->listaOS();
	die;
}

// ajax para listar as ordens de serviço cadastradas
// executado quando o usuário clica no botão Gerar Nova OS
if ($_REQUEST['requisicao'] == 'ajaxos' && $_REQUEST['gpdid']) {
	$grupo = (int)$_REQUEST['gpdid'];
	if($grupo > 0)
		echo montaListaOS($grupo);
	exit();
}

if($_POST['requisicao'] == 'preencheNotasEmpenho'){
	if($_POST['gpdid']){
		$sql = "select empvlrsetec, empvlrfnde, empvlrsesu from obras.empenho where estuf in (select estuf from obras.grupodistribuicao where gpdid = {$_POST['gpdid']}) limit 1";
		echo json_encode($db->pegaLinha($sql));
	}
	die;
}

$obOS = new OSController();
$obOS->ativaDadosOrdemServico(null, $_GET['orsid']);
$obOS->ativaDadosGrupo( array('gpdid', 'epcid', 'gpddtinicio', 'gpdtermino') );

if ($_POST['operacao'] == 'salvar'){
	cancelaUltimaOS($_POST['gpdid']);
	$orsid = $obOS->salvarOS();
	$msg = $orsid ? 'Operação realizada com sucesso!' : 'Falha na operação!';
	die("<script>alert('{$msg}'); location.href='?modulo=principal/supervisao/cadOS&acao=A&gpdid={$_POST['gpdid']}';</script>");
}

// cabecalho padrão do sistema
include APPRAIZ . "includes/cabecalho.inc";

// Monta as abas
print "<br/>";
$db->cria_aba( $abacod_tela, $url, $parametros );
monta_titulo( "Cadastro de Ordem de Serviço",  obrigatorio() . " Campos Obrigatórios" );

?>
<style type='text/css'>				

	div.scrollTable{
		background: #fff;
		/*border: 1px solid #888;*/
	}

	div.scrollTable table.header, div.scrollTable div.scroller table{
		width: 100%;
		border-collapse: collapse;
	}
	
	div.scrollTable table.header th, div.scrollTable div.scroller table td{
		/*border: 1px solid #444;*/
		padding: 3px 5px;
	}
	
	div.scrollTable table.header th{
		background: #ddd;
	}

	div.scrollTable div.scroller{
		height: 100px;
		overflow: scroll;
	}

	div.scrollTable .coluna75px{
		width: 75px;
	}

	div.scrollTable .coluna100px{
		width: 100px;
	}

	div.scrollTable .coluna150px{
		width: 150px;
	}
</style>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript"><!--

function existeDeclaracao(orsid)
{
	var retorno = false;
	
	$.ajax({
	   type: "POST",
	   url: "obras.php?modulo=principal/supervisao/cadOS&acao=A",
	   data: "ajaxExisteDeclaracao=1&orsid="+orsid,
	   async: false,
	   success: function(msg)
	   {
		   if( msg == 'existe' )
		   {
			   retorno = true;
		   }
	   }
	 });

	return retorno;
}

function verificaDeclaracao(orsid)
{
	var retorno = false;
	
	if( existeDeclaracao(orsid) )
	{
		if( confirm("Esta ação irá cancelar um declaração gerada, que está associada à esta OS.\nDeseja prosseguir?") )
		{
			$.ajax({
			   type: "POST",
			   url: "obras.php?modulo=principal/supervisao/cadOS&acao=A",
			   data: "ajaxCancelarDeclaracao=1&orsid="+orsid,
			   async: false,
			   success: function(msg)
			   {
				   if( msg == 'ok' )
				   {
					   retorno = true;
				   }
				   else
				   {
					   alert('Ocorreu um erro ao cancelar a Declaração.');
				   }
			   }
			 });
		}
	}
	else
	{
		retorno = true;
	}

	return retorno;
}

function excluir( orsid, objClick ){
	divCarregando( objClick );
	if (confirm('Deseja deletar a O.S. Nº'+ orsid + '?'))
	{
		// se houver declaração com a OS, cancela-a ou não, dependendo da decisão do usuário
		if( verificaDeclaracao(orsid) )
		{
			var operacao = "excluir";
			//enviando o post
			$.post('obras.php?modulo=principal/supervisao/listaOS&acao=A&requisicao=ajaxos', { operacao : operacao, orsid : orsid  },
				function(){
					$('#geraros').trigger('click');
					divCarregado();
				});
		}
	}
	divCarregado();
	return;
}

function visualizarOS( orsid ){
	janela('?modulo=principal/supervisao/emitirOS&acao=A&orsid=' + orsid , 900, 600, 'OS');
	return;
}

$(document).ready(function() {

	$('input[name=rdo_grupo]').click(function() {
		var param = new Array();
		param.push({name : 'requisicao', value : 'preencheNotasEmpenho'}, 
				   {name : 'gpdid', value : $(this).val()}
		);
		$.ajax({
			type	 : "POST",
			url		 : "obras.php?modulo=principal/supervisao/cadOS&acao=A",
			data	 : param,
			async    : false,
			dataType : 'json',
			success	 : function(data){
							$('#orsnotaempeds').val(data.empvlrsesu);
							$('#orsnotaempedp').val(data.empvlrsetec);
							$('#orsnotaempedb').val(data.empvlrfnde);
					   }
			 });

    });

	// função Listar OS
	$("#geraros").click(function () {
		var gpdid = $('#gpdid').val();

		$('#orsnotaempeds').val('');
		$('#orsnotaempedp').val('');
		$('#orsnotaempedb').val('');
		
		if(gpdid > 0){
			divCarregando();
			//enviando o post
			$.post('obras.php?modulo=principal/supervisao/cadOS&acao=A&requisicao=ajaxos', { gpdid : gpdid },
				function(data){
					$('#os').html(data);
					divCarregado();

					$('#orsnotaempeds').val( $('#sesu').val() );
					$('#orsnotaempedp').val( $('#setec').val() );
					$('#orsnotaempedb').val( $('#fnde').val() );
					
				});
		}
		
	})

	<?php if ($_REQUEST['gpdid']) { //caso exista o request então eu executo a trigger para mostrar as OSs do grupo ?>
	$('#geraros').trigger('click');
	<?php }?>
	
});

//
--></script>
<form method="POST"  name="formulario" onsubmit="javascript:return validacao();">
<input type="hidden" name="stoid" value="1">
<input type="hidden" name="operacao" value="salvar">
<input type="hidden" name="orsid" value="<?=$obOS->orsid ?>">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td align='right' style="width: 190px;" class="SubTituloDireita">Código do Grupo:</td>
		<td>
			<input type="text" name="gpdid" id="gpdid" maxlength="4" value="<?php echo $_REQUEST['gpdid']; ?>">
			&nbsp;
			<input type="button" id="geraros" value="Mostrar OS's do Grupo">
		</td>
	</tr>
	<tr>
		<td align='right' style="width: 190px;" class="SubTituloDireita">Lista de OS do Grupo</td>
		<td colspan="2" id="os"></td>
	</tr>
      <!-- <tr>
        <td align='right'  class="SubTituloDireita">Grupo(id) / Empresa:</td>
        <td> 
			<? //$empresa = !$obOS->empresa ? "Não existe grupo atribuído." : $obOS->gpdid . ' / ' . $obOS->empresa; ?>
        	<?php //echo campo_texto('empresa','N','N','',60,60,'','','','','',' id="gprdsc" onclick="exibeGrupo()" ');?>
        	<img style="vertical-align: sub;cursor:pointer" onclick="exibeGrupo()" src="../imagens/arrow_v.png" />
        	<img style="vertical-align: top;" src="../imagens/obrig.gif" />
        	<input type="hidden" id="gpdid" name="gpdid" value="<?php //echo $obOS->gpdid?>"  />
        </td>
      </tr> -->
      <tr id="tr_grupo" style="display:none" >
      	<td class="SubTituloDireita" >
      	</td>
      	<td align="left">
      	<?
        $obOS->listaSelecaoGrupo();
		?>
      	</td>
      </tr>
      <!-- <tr>
        <td align='right'  class="SubTituloDireita">Nº da O.S.:</td>
        <td>
        	<? //$orsid = $obOS->orsid; ?>
        	<?php //echo campo_texto('orsid','N','N','',10,14,'','');?>
        </td>
      </tr> -->
      <!-- <tr>
        <td align='right'  class="SubTituloDireita">Situaçao da OS:</td>
        <td>
		<? 
		
//		$existeOsGrupoAtivo = $obOS->verificaOsGrupoAtivo( $obOS->gpdid, $obOS->orsid );
//		$statusOSAtivo = ($existeOsGrupoAtivo ? 'N' : 'S');
//		$stoid = $obOS->stoid;
//		if ( !possuiPerfil( array(PERFIL_SAA, PERFIL_ADMINISTRADOR, PERFIL_SUPERUSUARIO) ) ){
//			$statusOSAtivo = 'N';
//		}
//		$db->monta_combo( 'stoid', $obOS->buscaDadosSituacaoOS(), $statusOSAtivo, '', '', '', '', 100, 'S', 'stoid' );
//		$img    = "<img src='../imagens/IconeAjuda.gif' title='A situação não pode ser alterado, pois já existe OS ativa para este Grupo'>";
//		$button = '<input type="button" name="btgravarsit" value="Gravar Situação" class="botao" onclick="salvarSituacao(' . $obOS->orsid . ')">';
//		
//		if ( $existeOsGrupoAtivo ){
//			echo $img;			
//		}elseif ( $obOS->orsid && possuiPerfil( array(PERFIL_SAA, PERFIL_ADMINISTRADOR, PERFIL_SUPERUSUARIO) )){
//			echo $button;
//		}
			
		?>
        </td>
      </tr> -->
      <tr>
        <td align='right' style="width: 190px;" class="SubTituloDireita" width="30%">Data de Emissão:</td>
        <td>
        <? 
        $orsdtemissao = $obOS->orsdtemissao ? $obOS->orsdtemissao : date('d-m-Y');
        ?>
		<?= campo_data2( 'orsdtemissao','S', 'S', 'Data de Emissão', 'S' ); ?>
        </td>
      </tr>
      <tr>
        <td align='right' style="width: 190px;" class="SubTituloDireita" width="30%">Início da Execução:</td>
        <td>
        <? $orsdtinicioexecucao = $obOS->orsdtinicioexecucao; ?>
		<?= campo_data2( 'orsdtinicioexecucao','S', 'S', 'Início da execução', 'S' ); ?>
        </td>
      </tr>
      <tr>
        <td align='right' style="width: 190px;" class="SubTituloDireita" width="30%">Término da Execução:</td>
        <td>
        <? $orsdtfinalexecucao = $obOS->orsdtfinalexecucao; ?>
		<?= campo_data2( 'orsdtfinalexecucao','S', 'S', 'Término da execução', 'S' ); ?>
        </td>
      </tr>
      <!--
      <tr>
        <td align='right'  class="SubTituloDireita">Nº Protec:</td>
        <td>
        <? //$orsnumprotec = $obOS->orsnumprotec; ?>
        <?//=campo_texto('orsnumprotec','S','S','',15,15,'','');?>
        </td>
      </tr>
      -->
      <tr>
        <td colspan="2" align='right'  class="SubTituloCentro">Notas de Empenho</td>
      </tr>
      <tr>  
        <td align='right' style="width: 190px;" class="SubTituloDireita">Educação Superior:</td>
        <td>
        <? $orsnotaempeds = $obOS->orsnotaempeds; ?>
        <?=campo_texto('orsnotaempeds','S','S','',15,15, '', '', '', '','', "id='orsnotaempeds'");?>
        </td>
      </tr>
      <tr>  
        <td align='right' style="width: 190px;" class="SubTituloDireita">Educação Profissional:</td>
        <td>
        <? $orsnotaempedp = $obOS->orsnotaempedp; ?>
        <?=campo_texto('orsnotaempedp','S','S','',15,15, '', '', '', '','', "id='orsnotaempedp'");?>
        </td>
      </tr>
      <tr>  
        <td align='right' style="width: 190px;" class="SubTituloDireita">Educação Básica:</td>
        <td>
        <? $orsnotaempedb = $obOS->orsnotaempedb; ?>
        <?=campo_texto('orsnotaempedb','S','S','',15,15, '', '', '', '','', "id='orsnotaempedb'");?>
        </td>
      </tr>
	  <tr>
		<td style="width: 190px;" class="SubTituloDireita">Observação:</td>
		<td>
        <? $orsobs = $obOS->orsobs; ?>
		<?=campo_textarea('orsobs','N', 'S', '', 80, 05, 500,''); ?>
		</td>
	  </tr>      
	  <tr bgcolor="#CCCCCC">
	    <td>&nbsp;</td>
	    <td>
	    	<input type="submit" name="btalterar" value="Salvar">
	    	&nbsp;&nbsp;&nbsp;&nbsp;
	    	<input type="button" name="btcancelar" value="Voltar" onclick="history.go(-1)" class="botao">
	    </td>
	  </tr>      
</table>
</form>
<script>
function validacao(){
	var retorno = true;
	var arCampos = new Array(
							 'gpdid',
							 'orsdtemissao',
							 'orsdtinicioexecucao',
							 'orsdtfinalexecucao',
							 'stoid',
//							 'orsnumprotec',
							 'orsnotaempeds',
							 'orsnotaempedp',
							 'orsnotaempedb'
							);
	if ( !valObrig(arCampos) ){
		retorno = false;
	}
	
	return retorno;
}

function valObrig(arCampo){
	for (var i=0; i < arCampo.length; i++){
		if ($.trim( $("[name='" + arCampo[i] + "']").val() ) == ""){
			alert('Preencha todos os campos obrigatórios!');
			$("[name='" + arCampo[i] + "']").focus();
			$("[name='" + arCampo[i] + "']").select();
			return false;
		}
	}
	return true;
}
function exibeGrupo(){
	var  gpdid = $('#gpdid').val();
	$('#rdo_grupo_' + gpdid ).attr("checked", "checked");
	if($("#tr_grupo").css("display") == "none")
		$("#tr_grupo").css("display","");
	else
		$("#tr_grupo").css("display","none");
}

function selecionarGrupo(gpddtinicio, gpdtermino, validacaoMetragem){
	if ( validacaoMetragem ){
		alert('Este grupo possui obras com metragens zeradas!\n' + replaceAll(validacaoMetragem, '||', '\n'));
		$('#gprdsc').val('Não existe grupo atribuído.');
		return false;
	}else if(!$('input:radio[name=rdo_grupo]').is(':checked')){
		alert('Favor selecionar um grupo!');
		return false;
	}else{
		var grpid = $('input:radio[name=rdo_grupo]:checked').val();
		var desc = $('#' + grpid + '_grp_desc').html();
		
		$('#gpdid').val(grpid);
		$('#gprdsc').val(grpid + ' / ' + desc);
		// Sujere datas
		if ($('#orsdtinicioexecucao').val() == ''){
			$('#orsdtinicioexecucao').val(gpddtinicio);
		}
		if ($('#orsdtfinalexecucao').val() == ''){
			$('#orsdtfinalexecucao').val(gpdtermino);
		}
		exibeGrupo();
	}
}
function exibeDadosEmpresa(gpdid){
	janela('?modulo=principal/supervisao/grupoObrasRotas&acao=A&gpdid=' + gpdid , 900, 480, 'Empresa');
}

function salvarSituacao(orsid){
	divCarregando();
	
	var endURL = '?modulo=principal/supervisao/cadOS&acao=A';
	var stoid  = jQuery("#stoid").val()
	var dado   = {"operacao" : "salvar", 
				  "orsid"    : orsid,
				  "stoid"    : stoid}; 
	jQuery.ajax({type: "POST",
   				 url: endURL,
   				 async: false,
   				 data: dado,
				 success: function(msg){
				   	  alert('Operação realizada com sucesso!');
				   }
				});
	
	divCarregado();
	return;
}

<? 
if ( $obOS->orsid ):
?>
$('[name=formulario] input,textarea').each(function (){ 
			if ( $(this).attr('name') != 'btcancelar' && $(this).attr('name') != 'btgravarsit' ){
				$(this).attr('disabled', 'disabled'); 
			}	
		});	
<? 
endif;
?>
</script>
