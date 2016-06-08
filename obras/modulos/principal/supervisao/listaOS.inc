<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<?php 
header('Content-Type: text/html; charset=ISO-8859-1');
include APPRAIZ . "includes/classes/PaginacaoAjax.class.inc";

$obOS = new OSController();

if ( $_GET['operacao'] == 'listaGrupoTotal' ){
	$arParam['filtro'] = array("gpdid" => $_GET['gpdid']);
	echo "<b>Todas as OS vinculadas ao grupo ({$_GET['gpdid']})</b>";
	$obOS->listaOSTodos( $arParam );
	echo "<table width=\"100%\"  class=\"listagem\" bgcolor=\"#f5f5f5\" cellSpacing=\"0\" cellPadding=\"2\" align=\"center\">"
		."<tr colspan='3' bgcolor=\"#d0d0d0\">"
		.	"<td>"
		.		"<input type=\"button\" name=\"btFechar\" value=\"Fechar\" onclick=\"window.close();\" style=\"cursor: pointer;\">"
		.	"</td>"
		."</tr>"
		."</table>";
	die();
}

$obOS->ativaDadosOrdemServico( array('orsid'), $_POST['orsid'] );

// A deleção é feita por ajax, por isso não é feito o refresh na página.
if ( $_POST['operacao'] == 'excluir' && $_POST['orsid'] ){
	$obOS->deletaOS( $_POST['orsid'] );	
	$obOS->listaOS();
	die;
}

// cabecalho padrão do sistema
include APPRAIZ . "includes/cabecalho.inc";

// Monta as abas
print "<br/>";
$db->cria_aba( $abacod_tela, $url, $parametros );
monta_titulo( "Lista de Ordem de Serviço", "" );

// Extrai dados para carregar os campos da pesquisa
extract($_POST);
?>
<link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
<script type="text/javascript" src="../includes/remedial.js"></script>
<script type="text/javascript" src="../includes/superTitle.js"></script>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<form method="post">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td align='right' class="SubTituloDireita">Nº da O.S.:</td>
		<td>
		<?=campo_texto('orsid','N','S','',10,14,'[#]','');?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Nº do Grupo:</td>
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
	<!--<tr>
		<td align='right' class="SubTituloDireita">Situação da OS:</td>
		<td>
		<?php 
		//$db->monta_combo("stoid", $obOS->buscaDadosSituacaoOS(), "S", "Todas", '', '', '', '', 'N','stoid');
		?>
		</td>
	</tr>-->
	<tr>
		<td align='right' class="SubTituloDireita" width="30%">Data de Emissão:</td>
	    <td>
		<?= campo_data2( 'orsdtemissaoini','N', 'S', '', 'N' ); ?>
		&nbsp;até&nbsp;
		<?= campo_data2( 'orsdtemissaofim','N', 'S', '', 'N' ); ?>
	    </td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita" width="30%">UF:</td>
	    <td>
	    	<?php 

				$estcod = $_REQUEST["estcod"];
			
				$sql = "SELECT 
							estuf as descricao,
							estuf as codigo
						FROM
							territorios.estado
						ORDER BY
							descricao;";
				
				$db->monta_combo("estcod", $sql, "S", "Todas", '', '', '', '', 'N','estcod');
				
			?>
	    </td>
	</tr>
	<tr bgcolor="#CCCCCC">
		<td>&nbsp;</td>
	    <td>
	    	<input type="submit" name="btalterar" value="Pesquisar">
	    </td>
	</tr>      
</table>
</form>
<center>
<div style="width:95%; text-align: left;">
	<!--<input type="button" value="Gerar O.S." style="margin-top: 2px;" onclick="location.href='?modulo=principal/supervisao/cadOS&acao=A';">-->
	<input type="button" value="Manter O.S." style="margin-top: 2px;" onclick="location.href='?modulo=principal/supervisao/cadOS&acao=A';">
	<!--<span id="listaOS"><?php //$obOS->listaOS( array("filtro" => $_POST) );?></span>	-->
	<div id="listaOS"><?php OSController::listaOS( array("filtro" => $_POST, 'nrRegPorPagina' => 20 ) );?></div>
	<!--<input type="button" value="Gerar O.S." style="margin-top: 2px;" onclick="location.href='?modulo=principal/supervisao/cadOS&acao=A';">-->
	<input type="button" value="Manter O.S." style="margin-top: 2px;" onclick="location.href='?modulo=principal/supervisao/cadOS&acao=A';">
</div>
</center>
<script>
function excluir( orsid, objClick ){
	divCarregando( objClick );
	if (confirm('Deseja deletar a O.S. Nº'+ orsid + '?')){
		var url  = '?modulo=principal/supervisao/listaOS&acao=A';
		var dado = {"operacao" : "excluir", "orsid" : orsid} 
		jQuery('#listaOS').load(url, dado);
	}
	divCarregado();
	return;
}

function visualizarOS( orsid ){
	janela('?modulo=principal/supervisao/emitirOS&acao=A&orsid=' + orsid , 900, 600, 'OS');
	return;
}
</script>
