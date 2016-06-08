<?php
if( $_POST['alteraAjax'] ){
	die( dadosAjax($_POST['alteraAjax']) );
}
include  APPRAIZ."includes/cabecalho.inc";
echo '<br>'; 
include_once( APPRAIZ. "gestaopessoa/classes/FtExperienciaAnterior.class.inc" );
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Força de Trabalho', 'Experiências Anteriores' ); 

echo cabecalhoPessoa($_SESSION['fdpcpf']);

$bloquearEdicao = bloqueiaEdicaoFT();

function dadosAjax($id){
	global $db;
	$sql = "SELECT  e.fneid,t.ftedescricao, e.fteid, n.fnedescricao, e.feadescricao, feaanoinicio, feaanofim
    		FROM gestaopessoa.ftexperienciaanterior as e
    		INNER JOIN gestaopessoa.fttipoexperienciaanterior AS t ON t.fteid = e.fteid
    		INNER JOIN gestaopessoa.fttiponivelexperienciaanterior AS n ON n.fneid = e.fneid
			WHERE e.feaid = $id"; 
	$dados = $db->carregar( $sql );
	if( $dados ){
		$res = 		 
		$dados[0]['fneid'].'_'.
		$dados[0]['fnedescricao'].'_'.
		$dados[0]['fteid'].'_'.
		$dados[0]['ftedescricao'].'_'.
		$dados[0]['feadescricao'].'_'.
		$dados[0]['feaanoinicio'].'_'.
		$dados[0]['feaanofim'];
		return $res;
	}
}
$exp = new FtExperienciaAnterior();
if( $_REQUEST['del'] != ''){
	$exp->excluir( $_REQUEST['del'] );
	$exp->commit();
	$exp->sucesso("principal/cadExpAnterior");
}
if( $_POST['fteid']){
	if( $_POST['alterar'] != ''){
	
 		$id = $exp->pegaUm( "select feaid from gestaopessoa.ftexperienciaanterior where feaid = ". $_POST["alterar"] );
	} 
	if( $id ){ 
		$exp->carregarPorId( $id );
	}
  	$arDados = array( 	 
				  	'fdpcpf', 
				  	'fneid',
					'fteid',
					'feadescricao' ,
					'feaordem' ); 
  	
  	$exp->fdpcpf = $_SESSION['fdpcpf'];
  	$exp->feaordem = 1;
  	$exp->feaanoinicio = $_POST['feaanoinicio'];
  	$exp->feaanofim    = $_POST['feaanofim'];
  	$exp->popularObjeto($arDados);
  	$exp->salvar();
	$exp->commit();
    $exp->sucesso( "principal/cadExpAnterior" );
}

?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<center> Preencha as Experiências Anteriores  </center>
 <form name = "formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <input type ="hidden" name="alterar" id="alterar" value="">
         	<tr>
                <td class ="SubTituloDireita" align="right">Tipo de Atividade: </td>
                <td>
                <?
                	$sql = "SELECT 
                			fteid as codigo, 
                			ftedescricao as descricao
                			FROM gestaopessoa.fttipoexperienciaanterior
                			WHERE ftestatus = 'A'
                       ";  
                $db->monta_combo('fteid', $sql, 'S', "Selecione...", '', '', '', '200', 'S', 'fteid');
                ?>
                </td>
            </tr>
 
            <tr>
                <td class ="SubTituloDireita" align="right">Nível: </td>
                <td>
                <?
                	$sql = "SELECT 
                			fneid as codigo, 
                			fnedescricao as descricao
                			FROM gestaopessoa.fttiponivelexperienciaanterior
                			WHERE fnestatus = 'A'
                       ";  
                 
                $db->monta_combo('fneid', $sql, 'S', "Selecione...", '', '', '', '200', 'S', 'fneid');
                ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Descrição da Experiência Anterior: </td>
                <td>
                	<?= campo_textarea( 'feadescricao', 'S', 'S', '', 70, 2, 1000 ); ?>
                </td>
            </tr> 
            <tr>
                <td class ="SubTituloDireita" align="right">Período: </td>
                <td>  
                de                
                <select class="CampoEstilo" name="feaanoinicio" id="feaanoinicio">
	          		<option value="">Selecione...</option>
					<?
					for( $i = 1940; $i <= date("Y"); $i++ ){?> 
		                	<option value="<?=$i;?>"><?=$i;?></option>  
					<?}?>  
					 </select>	
				à
				<select class="CampoEstilo"  name="feaanofim" id="feaanofim">
	          		<option value="">Selecione...</option>
					<?
					for( $i = 1940; $i <= date("Y"); $i++ ){?> 
		                	<option value="<?=$i;?>"><?=$i;?></option>  
					<?}?>
					</select> 
                </td> 
            </tr>
            <tr>
            	<td class ="SubTituloDireita" align="right">  </td>
            	<td><input type="button" name="btSalvar" id="btSalvar" onclick="validaForm();" value="Salvar" <?=$bloquearEdicao;?>></td>
            </tr>
    </table>
    </form>
    <?php
    $sql = "SELECT 
    		'<img
						align=\"absmiddle\"
						src=\"/imagens/alterar.gif\"
						style=\"cursor: pointer\"
						onclick=\"javascript: alterar('||e.feaid ||' );\"
						title=\"Alterar Atividade\"
					 > &nbsp;
					 <img
						align=\"absmiddle\"
						src=\"/imagens/excluir.gif\"
						style=\"cursor: pointer\"
						onclick=\"javascript: excluir('||e.feaid ||' );\"
						title=\"Excluir Atividade\"
					 > ' as acao,  t.ftedescricao,  n.fnedescricao, 
					 REPLACE (e.feadescricao, chr(13)||chr(10), '<br>'), 
					 ( feaanoinicio  || ' à ' || feaanofim ) as data
    		FROM gestaopessoa.ftexperienciaanterior as e
    		INNER JOIN gestaopessoa.fttipoexperienciaanterior AS t ON t.fteid = e.fteid
    		INNER JOIN gestaopessoa.fttiponivelexperienciaanterior AS n ON n.fneid = e.fneid
    		WHERE e.fdpcpf = '".$_SESSION['fdpcpf']."'
    		"; 
    //dbg($sql);
	$cabecalho = array("&nbsp;&nbsp;&nbsp;&nbsp;Ação", "Atividade","Nível", "Descrição das Experiências Anteriores" , "Período"); 
	$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', '');
	
	?>
<script>

function validaForm(){
	var fteid = document.getElementById('fteid');
	var fneid = document.getElementById('fneid');
	var feadescricao = document.getElementById('feadescricao');
	var feaanoinicio = document.getElementById('feaanoinicio');
	var feaanofim = document.getElementById('feaanofim');
	if( fteid.value == '' ){
		alert( 'O campo Tipo de Atividade é obrigatório' );
		return false;
	}
	if( fneid.value == '' ){
		alert( 'O campo Nível é obrigatório' );
		return false;
	}
	if( feadescricao.value == '' ){
		alert( 'O campo Descrição da Atividade é obrigatório' );
		return false;
	}
	if( feaanoinicio.value == '' ){
		alert( 'O campo Ano inicial é obrigatório' );
		return false;
	}
	if( feaanofim.value == '' ){
		alert( 'O campo Ano Final é obrigatório' );
		return false;
	}
	if( Number( feaanoinicio.value ) > Number( feaanofim.value ) ){
		alert( 'O ano inicial não pode ser maior que o ano final');
		return false;
	}
	document.formulario.submit();
}
function excluir( id ){
	if( confirm( 'Deseja realmente excluir as informações de formação?') ){
		window.location.href = 'gestaopessoa.php?modulo=principal/cadExpAnterior&acao=A&del='+id;
	}
}
function alterar(id){
 
	var fteid 	 	 = document.getElementById('fteid'); 
	var fneid	 	 = document.getElementById('fneid');
	var feadescricao = document.getElementById('feadescricao'); 
	var feaanoinicio = document.getElementById('feaanoinicio'); 
	var feaanofim= document.getElementById('feaanofim'); 
	var alterar 	 = document.getElementById('alterar');
	var req = new Ajax.Request('gestaopessoa.php?modulo=principal/cadExpAnterior&acao=A', {
							        method:     'post',
							        parameters: '&alteraAjax='+id,
							        onComplete: function (res)
							        {  
							        	 var arRes 	  = res.responseText.split("_"); 
							        	 
							        	 var nivel     = arRes[0]; 
							        	 var lbNivel   = arRes[1];
							        	 var tipo      = arRes[2]; 
							        	 var lbTipo    = arRes[3]; 
							        	 var descricao = arRes[4]; 
							        	 var dataI 	   = arRes[5]; 
							        	 var dataF 	   = arRes[6]; 
 	   
 	  								     fteid.value 		  = tipo;
 	  								     fneid.value 		  = nivel;
							        	 feadescricao.value   = descricao; 
							        	 feaanoinicio.value = dataI;
							        	 feaanofim.value   = dataF;
							        	 alterar.value 		  = id;
 
							        	 fteid.options[0].value = tipo;
										 fteid.options[0].text  = lbTipo;
										 
										 fneid.options[0].value = nivel;
										 fneid.options[0].text  = lbNivel;
							        }
							  });
}
</script>