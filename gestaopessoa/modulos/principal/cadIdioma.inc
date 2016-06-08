<?php
if( $_POST['alteraAjax'] ){
	die( dadosIdiomaAjax($_POST['alteraAjax']) );
}
if( $_POST['novoIdioma'] != ''){
	$sql = "INSERT INTO gestaopessoa.ftitipoidioma ( ftidescricao, ftistatus ) VALUES ( '".$_POST['novoIdioma']."', 'A' ) RETURNING ftiid";
	$ins = $db->pegaUm( $sql );
	$db->commit();
	if( $ins ){
		die( $ins.'_'.$_POST['novoIdioma']);
	}
	die();
}

$bloquearEdicao = bloqueiaEdicaoFT();

include  APPRAIZ."includes/cabecalho.inc";
echo '<br>'; 
include_once( APPRAIZ. "gestaopessoa/classes/Idioma.class.inc" ); 
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Força de Trabalho', 'Idiomas' ); 
$idioma   	= new Idioma(); 
if( $_REQUEST['del'] != ''){
	$idioma->excluir( $_REQUEST['del'] );
	$idioma->commit();
	$idioma->sucesso("principal/cadIdioma");
}
echo cabecalhoPessoa($_SESSION['fdpcpf']);

function dadosIdiomaAjax($id){
	global $db;
	$sql = "SELECT i.ftiid,i.ftcidleitura,i.ftcidfala,i.ftcidescrita, ti.ftidescricao FROM gestaopessoa.idioma as i
			inner join gestaopessoa.ftitipoidioma as ti on ti.ftiid = i.ftiid WHERE i.fidif = $id";
	$dados = $db->carregar( $sql );
	if( $dados ){
		$res = $dados[0]['ftiid'].'_'.$dados[0]['ftcidleitura'].'_'.$dados[0]['ftcidfala'].'_'.$dados[0]['ftcidescrita'].'_'.$dados[0]['ftidescricao'];
		return $res;
	}
}

if( $_POST['ftiid'] ){
	if( $_POST['alterar'] != ''){
 		$id = $idioma->pegaUm( "select fidif from gestaopessoa.idioma where fidif = ". $_POST["alterar"] );
	} 
	if( $id ){ 
		$idioma->carregarPorId( $id );
	}
  	$arDados = array( 	 
					  	'ftiid',
					  	'ftcidleitura',
					  	'ftcidfala',
					  	'ftcidescrita');
  	$idioma->fdpcpf = $_SESSION['fdpcpf'];
  	$idioma->popularObjeto($arDados);
  	$idioma->salvar();
	$idioma->commit();
    $idioma->sucesso( "principal/cadIdioma" );
}
?>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script src="../includes/prototype.js"></script>
<center> Preencha os Idiomas  </center>
 <form name = "formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <input type="hidden" value="" name="alterar" id="alterar" >
         	<tr>
                <td class ="SubTituloDireita" align="right">Idioma: </td>
                <td>
                <?
                /*
                	$sql = "SELECT 
                            f.ftiid AS codigo, 
                            f.ftidescricao AS descricao
                        FROM
                            gestaopessoa.ftitipoidioma as f
                        
                        WHERE f.ftiid NOT IN (
                        			SELECT ftiid FROM gestaopessoa.idioma WHERE fdpcpf = '".$_SESSION['fdpcpf']."' AND ftiid = f.ftiid
                        			)
                        			union all select 0 as codigo , 'Outros' as descricao
                       ";  
                $db->monta_combo('ftiid', $sql, 'S', "Selecione...", 'abreOutros(this.value);', '', '', '200', 'S', 'ftiid');
                */
                	$sql = "SELECT 
                            f.ftiid AS codigo, 
                            f.ftidescricao AS descricao
                        FROM
                            gestaopessoa.ftitipoidioma as f
                       ";  
                
                $db->monta_combo('ftiid', $sql, 'S', "Selecione...", '', '', '', '200', 'S', 'ftiid');
                ?>
                </td>
            </tr>
 
            <tr>
                <td class ="SubTituloDireita" align="right">Leitura: </td>
                <td>
                <?
                	$sql = "SELECT 
                            ftcid AS codigo, 
                            ftcdescricao AS descricao
                        FROM
                            gestaopessoa.ftconceitoidioma
                       "; 
                 
                $db->monta_combo('ftcidleitura', $sql, 'S', "Selecione...", '', '', '', '200', 'S', 'ftcidleitura');
                ?>
                </td>
            </tr>
 
            <tr>
                <td class ="SubTituloDireita" align="right">Fala: </td>
                <td>
                <?
                	$sql = "SELECT 
                            ftcid AS codigo, 
                            ftcdescricao AS descricao
                        FROM
                            gestaopessoa.ftconceitoidioma
                       ";  
                $db->monta_combo('ftcidfala', $sql, 'S', "Selecione...", '', '', '', '200', 'S', 'ftcidfala');
                ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Escrita: </td>
                <td>
                <?
                	$sql = "SELECT 
                            ftcid AS codigo, 
                            ftcdescricao AS descricao
                        FROM
                            gestaopessoa.ftconceitoidioma
                       ";  
                $db->monta_combo('ftcidescrita', $sql, 'S', "Selecione...", '', '', '', '200', 'S', 'ftcidescrita');
                ?>
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
						onclick=\"javascript: alterar('|| i.fidif ||' );\"
						title=\"Alterar Idioma\"
					 > &nbsp;
					 <img
						align=\"absmiddle\"
						src=\"/imagens/excluir.gif\"
						style=\"cursor: pointer\"
						onclick=\"javascript: excluir('|| i.fidif ||' );\"
						title=\"Excluir Idioma\"
					 > ' as acao, 
    				fti.ftidescricao, 
    				fcleitura.ftcdescricao as leitura, 
    				fcfala.ftcdescricao as fala, 
    				fcescrita.ftcdescricao as escrita
    		FROM gestaopessoa.idioma as i 
    		INNER JOIN gestaopessoa.ftitipoidioma 	 as fti 	  on fti.ftiid 	  	 = i.ftiid
    		INNER JOIN gestaopessoa.ftconceitoidioma as fcleitura on fcleitura.ftcid = i.ftcidleitura
    		INNER JOIN gestaopessoa.ftconceitoidioma as fcfala    on fcfala.ftcid 	 = i.ftcidfala
    		INNER JOIN gestaopessoa.ftconceitoidioma as fcescrita on fcescrita.ftcid = i.ftcidescrita
    		WHERE i.fdpcpf = '".$_SESSION['fdpcpf']."'
    ";
	$cabecalho = array("&nbsp;&nbsp;&nbsp;&nbsp;Ação", "Idioma","Leitura", "Fala","Escrita" ); 
	$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', '');
	
	?>
<script>

function validaForm(){
	var ftiid = document.getElementById('ftiid');
	var ftcidleitura = document.getElementById('ftcidleitura');
	var ftcidfala = document.getElementById('ftcidfala');
	var ftcidescrita = document.getElementById('ftcidescrita');
 
 	if( ftiid.value == '' ){
 		alert( 'O Campo Idioma é obrigatório' );
 		return false;
 	}
 	if( ftcidleitura.value == '' ){
 		alert( 'O Campo Leitura é obrigatório' );
 		return false;
 	}
 	if( ftcidfala.value == '' ){
 		alert( 'O Campo Fala é obrigatório' );
 		return false;
 	}
 	if( ftcidescrita.value == '' ){
 		alert( 'O Campo Escrita é obrigatório' );
 		return false;
 	} 
	document.formulario.submit();
}
function excluir( id ){
	if( confirm( 'Deseja realmente excluir as informações do idioma?') ){
		window.location.href = 'gestaopessoa.php?modulo=principal/cadIdioma&acao=A&del='+id;
	}
}
function alterar(id){
	var ftiid 		 = document.getElementById('ftiid');
	var ftcidleitura = document.getElementById('ftcidleitura');
	var ftcidfala	 = document.getElementById('ftcidfala');
	var ftcidescrita = document.getElementById('ftcidescrita');
	var alterar = document.getElementById('alterar');
	var req = new Ajax.Request('gestaopessoa.php?modulo=principal/cadIdioma&acao=A', {
							        method:     'post',
							        parameters: '&alteraAjax='+id,
							        onComplete: function (res)
							        {	  
							        	 var arRes 	 = res.responseText.split("_"); 
							        	 var idioma  = arRes[0]; 
							        	 var leitura = arRes[1];
							        	 var fala 	 = arRes[2];
							        	 var escrita = arRes[3];
							        	 var labelid = arRes[4];  
							        	 
							        	 ftiid.value 		= idioma; 
							        	 ftcidleitura.value = leitura;
							        	 ftcidfala.value 	= fala;
							        	 ftcidescrita.value = escrita; 
							        	 alterar.value 		= id; 
							        	 
							        	 ftiid.options[0].value = idioma;
										 ftiid.options[0].text  = labelid;
							        }
							  });
}
function abreOutros(value){
 	if( value != 0 ){
 		return false;
 	}
	var ftiid 		 = document.getElementById('ftiid');
	var novoIdioma = prompt( 'Descrição do Idioma:', 'Novo Idioma' );
	if( novoIdioma ){
		var req = new Ajax.Request('gestaopessoa.php?modulo=principal/cadIdioma&acao=A', { 
								        method:     'post',
								        parameters: '&novoIdioma='+novoIdioma,							         
								        onComplete: function (res)
								        {							        	
											resp = res.responseText.split("_"); 
											var codigo = resp[0]; 
											var descricao = resp[1];
											ftiid.options[0].value = codigo;
											ftiid.options[0].text  = descricao;
											ftiid.value = codigo;
								        }
	   		}); 
	}	
}
</script>