<?php
if( $_POST['alteraAjax'] ){
	die( dadosAjax($_POST['alteraAjax']) );
}
include  APPRAIZ."includes/cabecalho.inc";
echo '<br>'; 
include_once( APPRAIZ. "gestaopessoa/classes/FtAtividadeDesenvolvida.class.inc" );
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Força de Trabalho', 'Atividades Desenvolvidas' ); 

$bloquearEdicao = bloqueiaEdicaoFT();

echo cabecalhoPessoa($_SESSION['fdpcpf']);
function dadosAjax($id){
	global $db;
	$sql = "SELECT  at.ftaid,t.ftadescricao, at.fnaid, n.fnadescricao, at.faddescricao
    		FROM gestaopessoa.ftatividadedesenvolvida as at
    		INNER JOIN gestaopessoa.fttipoatividadedesenvolvida AS t ON t.ftaid = at.ftaid
    		INNER JOIN gestaopessoa.fttiponivelatividadedesenvolvid AS n ON n.fnaid = at.fnaid
			WHERE at.fadid = $id";
 
	$dados = $db->carregar( $sql );
	if( $dados ){
		$res = 		 
		$dados[0]['ftaid'].'_'.
		$dados[0]['ftadescricao'].'_'.
		$dados[0]['fnaid'].'_'.
		$dados[0]['fnadescricao'].'_'.
		$dados[0]['faddescricao'];
		return $res;
	}
}
$atividade = new FtAtividadeDesenvolvida();
if( $_REQUEST['del'] != ''){
	$atividade->excluir( $_REQUEST['del'] );
	$atividade->commit();
	$atividade->sucesso("principal/cadAtividades");
}
if( $_POST['ftaid'] ){
	if( $_POST['alterar'] != ''){
		
 		$id = $atividade->pegaUm( "select fadid from gestaopessoa.ftatividadedesenvolvida where fadid = ". $_POST["alterar"] );
	} 
	if( $id ){ 
		$atividade->carregarPorId( $id );
	}
  	$arDados = array( 	 
				  	'fdpcpf', 
				  	'ftaid', 
				  	'fnaid', 
				  	'faddescricao' );
  	
  	
  	$atividade->fdpcpf = $_SESSION['fdpcpf'];
  	$atividade->fadordem = 1;
  	$atividade->popularObjeto($arDados);
  	$atividade->salvar();
	$atividade->commit();
    $atividade->sucesso( "principal/cadAtividades" );
}

?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<center> Preencha as Atividades Desenvolvidas  </center>
 <form name = "formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <input type ="hidden" name="alterar" id="alterar" value="">
         	<tr>
                <td class ="SubTituloDireita" align="right">Tipo de Atividade: </td>
                <td>
                <?
                	$sql = "SELECT 
                           		ftaid as codigo,
                           		ftadescricao as descricao
                           	FROM gestaopessoa.fttipoatividadedesenvolvida
                           	WHERE ftastatus = 'A'
                       "; 
 
                $db->monta_combo('ftaid', $sql, 'S', "Selecione...", '', '', '', '200', 'S', 'ftaid');
                ?>
                </td>
            </tr>
 
            <tr>
                <td class ="SubTituloDireita" align="right">Nível: </td>
                <td>
                <?
                	$sql = "SELECT 
                           		fnaid as codigo,
                           		fnadescricao as descricao
                           	FROM gestaopessoa.fttiponivelatividadedesenvolvid
                           	WHERE fnastatus = 'A'
                       ";  
 
                $db->monta_combo('fnaid', $sql, 'S', "Selecione...", '', '', '', '200', 'S', 'fnaid');
                ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Descrição da Atividade: </td>
                <td>
                	<?= campo_textarea( 'faddescricao', 'S', 'S', '', 70, 2, 1000 ); ?>
                </td>
            </tr> 
             
            <tr>
            	<td class ="SubTituloDireita" align="right">  </td>
            	<td><input type="button" name="btSalvar" id="btSalvar" onclick="validaForm();" value="Salvar" <?=$bloquearEdicao;?>></td>
            </tr>
     </table>
     </form>
    <?php
    $sql = "SELECT '<img
						align=\"absmiddle\"
						src=\"/imagens/alterar.gif\"
						style=\"cursor: pointer\"
						onclick=\"javascript: alterar('||at.fadid ||' );\"
						title=\"Alterar Atividade\"
					 > &nbsp;
					 <img
						align=\"absmiddle\"
						src=\"/imagens/excluir.gif\"
						style=\"cursor: pointer\"
						onclick=\"javascript: excluir('||at.fadid ||' );\"
						title=\"Excluir Atividade\"
					 > ' as acao,  
					 t.ftadescricao, n.fnadescricao, 
					 REPLACE (at.faddescricao, chr(13)||chr(10), '<br>')
    		FROM gestaopessoa.ftatividadedesenvolvida as at
    		INNER JOIN gestaopessoa.fttipoatividadedesenvolvida AS t ON t.ftaid = at.ftaid
    		INNER JOIN gestaopessoa.fttiponivelatividadedesenvolvid AS n ON n.fnaid = at.fnaid 
    		WHERE at.fdpcpf = '".$_SESSION['fdpcpf']."'";
	$cabecalho = array("&nbsp;&nbsp;&nbsp;&nbsp;Ação", "Atividade","Nível", "Descrição da Atividade" ); 
	$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', '');
	
	?>
<script>

function validaForm(){
	var ftaid = document.getElementById('ftaid');
	var fnaid = document.getElementById('fnaid');
	var faddescricao = document.getElementById('faddescricao');
	if( ftaid.value == '' ){
		alert( 'O campo Tipo de Atividade é obrigatório' );
		return false;
	}
	if( fnaid.value == '' ){
		alert( 'O campo Nível é obrigatório' );
		return false;
	}
	if( faddescricao.value == '' ){
		alert( 'O campo Descrição da Atividade é obrigatório' );
		return false;
	}
	
	document.formulario.submit();
}
function excluir( id ){
	if( confirm( 'Deseja realmente excluir as informações de formação?') ){
		window.location.href = 'gestaopessoa.php?modulo=principal/cadAtividades&acao=A&del='+id;
	}
}
function alterar(id){
 
	var ftaid 	 	 = document.getElementById('ftaid'); 
	var fnaid	 	 = document.getElementById('fnaid');
	var faddescricao = document.getElementById('faddescricao'); 
	var alterar 	 = document.getElementById('alterar');
	var req = new Ajax.Request('gestaopessoa.php?modulo=principal/cadAtividades&acao=A', {
							        method:     'post',
							        parameters: '&alteraAjax='+id,
							        onComplete: function (res)
							        {  
							        	 var arRes 	  = res.responseText.split("_"); 
							        	 
							        	 var tipo  	   = arRes[0]; 
							        	 var lbTipo	   = arRes[1];
							        	 var nivel     = arRes[2]; 
							        	 var lbNivel   = arRes[3]; 
							        	 var descricao = arRes[4]; 
 	  
 	  								     ftaid.value = tipo;
 	  								     fnaid.value = nivel;
							        	 faddescricao.value = descricao; 
							        	 
							        	 alterar.value 		= id;
 
							        	 ftaid.options[0].value = tipo;
										 ftaid.options[0].text  = lbTipo;
										 
										 fnaid.options[0].value = nivel;
										 fnaid.options[0].text  = lbNivel;
							        }
							  });
}
</script>