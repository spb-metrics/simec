<?php 
include_once APPRAIZ . "includes/classes/modelo/seguranca/Sistema.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QQuestionario.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QGrupo.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QPergunta.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QItemPergunta.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QResposta.class.inc";
include_once APPRAIZ . "includes/classes/questionario/QArvore.class.inc";

if($_POST['submeter'] == 'salvar'){	
	
	$itpidP = $_POST['itpid'];
	$peridP = $_POST['perid'];
	$justP = $_POST['itcjustificativa'];
	
	$sql = "SELECT * FROM questionario.itemcondicao WHERE itpid = {$itpidP} AND perid = {$peridP} AND itcstatus='A'";
	$arr = $db->carregar( $sql );
	
	if( is_array( $arr ) ){
		$sql = "UPDATE questionario.itemcondicao SET itcstatus='I' WHERE itpid = {$itpidP} AND perid = {$peridP} AND itcstatus='A'";
		$db->executar( $sql );
	}
	
	if( is_array($_POST['condicoes']) ){
		foreach( $_POST['condicoes'] as $k => $cond ){
			$sql = "INSERT INTO questionario.itemcondicao ( perid, itpid, itcperid, itcjustificativa, itcstatus ) VALUES ( {$peridP}, {$itpidP}, {$k}, '{$justP}', 'A' )";
			$db->executar( $sql );
		}
	}
	if($db->commit()){
		echo "<script>
				alert('Operação realizada com sucesso'); 
				window.close();
				window.opener.location.replace(window.opener.location);
			  </script>";
	} else {
		$db->rollback();
		$db->insucesso('Falha na operação');
	}
}

$itpid = $_REQUEST['itpid'];
$obItem = new QItemPergunta( $itpid );
$obPergunta = new QPergunta( $obItem->perid );

$queid = $obPergunta->pegaQuestionario( $obPergunta->perid );

$obItem->listaObjeto( array( "itpid" => $itpid) );
$perid = $obItem->perid;

$obArvore = new QArvore( array("questionarioC" => new QQuestionario($queid)) );

if( $itpid ){
	$sql = "SELECT * FROM questionario.itemcondicao WHERE itcstatus = 'A' AND itpid = ".$itpid;
	$arrDados = $db->carregar( $sql );
}

?>


<html>
	<head>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
		<script type="text/javascript" src="/includes/funcoes.js"></script>
		<script language="javascript" type="text/javascript" src="../includes/dtree/dtree.js"></script>
		<script type="text/javascript">
			var d = document;
			
			function salvarCondicao(){
				var camposObri 		= "#condicao#itcjustificativa";
				var tiposCamposObri	= '#combo#textarea';
				if(!validaForm('formulario',camposObri,tiposCamposObri,false))
						return false;
				
				d.getElementById('submeter').value = 'salvar';
				d.getElementById('formulario').submit();
			}
			
			function addItem(perid, pertitulo){
				var table, totRow, metadeRow, color, img;
				
				if( perid == <?=$perid ?> ){
					alert( 'Você não pode colocar uma condição na própria pergunta!' );
					return;
				}
				name = "descricao[]";
				img	 = '<img src="/imagens/excluir.gif" onclick="$(\'#tableItem\')[0].deleteRow(this.parentNode.parentNode.parentNode.rowIndex);">';			
				
				table = $('#tableItem');
				var campo1 = '<input type="text" name="condicoes[' + perid + ']" value="' + pertitulo + '" style="width:360px;">';
				
				table.append("<tr><td><center>" + img + "</center></td><td>" + campo1 + "</td></tr>");
	
				
				
			}
		</script>

	</head>
<body topmargin="0" marginwidth="0">
<form action="" method="POST" id="formulario" name="formulario" enctype="multipart/form-data">
<input type="hidden" id="submeter" name="submeter" value="">
<input type="hidden" id="itpid" name="itpid" value="<?=$itpid; ?>">
<input type="hidden" id="perid" name="perid" value="<?=$perid; ?>">
<input type="hidden" id="permultipla" name="permultipla" value="f">
<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="subtitulodireita" style="width: 20%;"><b>Condição:</b></td>
		<td><?
			$array = array(
						array("codigo" => "01", "descricao" => "Desabilita Perguntas")
						);
			$db->monta_combo("condicao", $array, 'S', '', '', '', '', '265', 'S', 'condicao', '', '', 'Condições' );
		 ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita" style="width: 20%;"><b>Justificativa:</b></td>
		<td><?
			echo campo_textarea('itcjustificativa', 'S', 'S', '', 55, 5, 800, '', '', '', '', 'Justificativa', $arrDados[0]['itcjustificativa']);
		 ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita" width="20px;"><b>Perguntas:</b></td>
		<td>
			<div id="bloco" style="overflow: hidden;">
				<div id="_arvore"></div>
			</div>
		</td>
	</tr>
	<?=$obArvore->montaArvore();?>	
	
	<tr id="itens">
		<td colspan="2" align="center">
			<fieldset style="background: #FFF;">
				<legend>Itens</legend>
				<div style="overflow: auto; height: 100px;">
				<table class="tabela" id="tableItem">
					<tr>
						<th width="10%">Ação</th>
						<th>Descrição</th>
					</tr>
				</table>
				</div>
			</fieldset>
		</td>
	</tr>
	
	<tr>
		<th colspan="2">
			<input type="button" value="Salvar" name="btnSalvar" id="btnSalvar" onclick="salvarCondicao();">&nbsp;
			<input type="button" value="Voltar" name="btnVoltar" id="btnVoltar" onclick="window.close();">
		</th>
	</tr>
</table>
</form>
<script type="text/javascript">
<?php 
if( is_array( $arrDados ) ){
	foreach( $arrDados as $item ):
		$sql = "SELECT pertitulo FROM questionario.pergunta WHERE perid = ".$item['itcperid'];
		$pertitulo = $db->pegaUm( $sql );
		echo "addItem({$item['itcperid']}, '{$pertitulo}');";
	endforeach;
}
?>
</script>
</body>
</html>