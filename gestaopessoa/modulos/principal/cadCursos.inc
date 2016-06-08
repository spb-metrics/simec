<?php
if( $_POST['alteraAjax'] ){
    die( dadosAjax($_POST['alteraAjax']) );
}

include  APPRAIZ."includes/cabecalho.inc";
echo '<br>';

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Força de Trabalho', 'Cursos' );


echo cabecalhoPessoa($_SESSION['fdpcpf']);

function dadosAjax($id){
    global $db;
    $sql = "SELECT 
    			ffccurso,
                ffcnomeinstituicao,
                ffccargahoraria,
                ffcanoconclusao
            FROM gestaopessoa.ftformacaocurso
            
            WHERE ffcid = $id";
    $dados = $db->carregar( $sql );
    if( $dados ){
        $res =         $dados[0]['ffccurso'].'_'.$dados[0]['ffcnomeinstituicao'].'_'.$dados[0]['ffccargahoraria'].'_'.$dados[0]['ffcanoconclusao'].'_'.$dados[0]['ffcid'];
        return $res;
    }
}


//////////////////////////////ALTERAR//////////////////////////////

if( $_POST['ffcid'] == '' ){
    if( $_POST['alterar'] != ''){


$ffccurso			= $_REQUEST['ffccurso'];
$ffcnomeinstituicao	= $_REQUEST['ffcnomeinstituicao'];
$ffccargahoraria	= $_REQUEST['ffccargahoraria'];
$ffcanoconclusao	= $_REQUEST['ffcanoconclusao'];

$sql = "UPDATE
			gestaopessoa.ftformacaocurso SET
			
				    ffccurso			= '".$ffccurso."',
				    ffcnomeinstituicao	= '".$ffcnomeinstituicao."',
				    ffccargahoraria		= ".$ffccargahoraria.",
				    ffcanoconclusao		= ".$ffcanoconclusao."
				    
			WHERE
				ffcid = ".$_POST["alterar"]."
	";
		$db->executar($sql);
		$db->commit();

		echo("<script>alert('Alterado com Sucesso.')\n</script>");				
		echo("<script>window.location.href = 'gestaopessoa.php?modulo=principal/cadCursos&acao=A'</script>");
		exit();	
	}
}
////////////////SALVAR//////////////////////////////

$fdpcpf	= $_SESSION['fdpcpf'];

if($_REQUEST["submetido"]) {

$ffcanoconclusao	= $_REQUEST['ffcanoconclusao'];
$ffcordem			= 1;
$ffccargahoraria	= $_REQUEST['ffccargahoraria'];
$ffcnomeinstituicao	= $_REQUEST['ffcnomeinstituicao'];
$ffccurso			= $_REQUEST['ffccurso'];

if( $_POST['ffcid'] == '' ){
	
$sql = "INSERT INTO
		gestaopessoa.ftformacaocurso(

	fdpcpf,
	ffcanoconclusao,
	ffcordem,
	ffccargahoraria,
	ffcnomeinstituicao,
	ffccurso
)
VALUES(
	'".$fdpcpf."',
	".$ffcanoconclusao.",
	'".$ffcordem."',
	".$ffccargahoraria.",
	'".$ffcnomeinstituicao."',
	'".$ffccurso."'
)";

	$db->executar($sql);
	$db->commit();
				echo("<script>alert('Cadastrado com Sucesso!')\n</script>");
				echo("<script>window.location.href = 'gestaopessoa.php?modulo=principal/cadCursos&acao=A'</script>");
				exit();
}

}

//////////////////////////////EXCLUIR//////////////////////////////

$ffcidDel = $_REQUEST['ffcid'];

if ($ffcidDel){

$sql = "DELETE FROM 
   					gestaopessoa.ftformacaocurso
   				WHERE
		   			ffcid =	'".$ffcidDel."'";
		$db->executar($sql);
		$db->commit();
		
		echo("<script>alert('Excluído com Sucesso.')\n</script>");				
		echo("<script>window.location.href = 'gestaopessoa.php?modulo=principal/cadCursos&acao=A'</script>");
		exit();	
	}

?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<center> Preencha o Curso  </center>
 <form name = "formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <input type="hidden" name="submetido" value="1">
    <input type ="hidden" name="alterar" id="alterar" value="">
            <tr>
                <td class ="SubTituloDireita" align="right">Curso: </td>
                <td>
                     <?= campo_texto('ffccurso', 'S', $somenteLeitura, '', 80, 600, '', '', 'left', '',  0, 'id="ffccurso" onblur="MouseBlur(this);"' ); ?> 
                </td>
            </tr>
           
            <tr>
                <td class ="SubTituloDireita" align="right">Nome da Instituição: </td>
                <td>
                     <?= campo_texto('ffcnomeinstituicao', 'S', $somenteLeitura, '', 80, 600, '', '', 'left', '',  0, 'id="ffcnomeinstituicao" onblur="MouseBlur(this);"' ); ?> 
                </td>
            </tr>

            <tr>
                <td class ="SubTituloDireita" align="right">Carga Horária: </td>
                <td>
                     <?= campo_texto('ffccargahoraria', 'S', $somenteLeitura, '', 4, 5, '####', '', 'left', '',  0, 'id="ffccargahoraria" onblur="MouseBlur(this);"' ); ?> 
                </td>
            </tr>
            
            <tr>
                <td class ="SubTituloDireita" align="right">Ano de Conclusão: </td>
                <td>
                     <?= campo_texto('ffcanoconclusao', 'S', $somenteLeitura, '', 4, 5, '####', '', 'left', '',  0, 'id="ffcanoconclusao" onblur="MouseBlur(this);"' ); ?> 
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">  </td>
                <td> <input type="button" name="btSalvar" id="btSalvar" onclick="incluirCurso();" value="Salvar">     
                </td>
            </tr>
     </table>
     </form>
          
    <?php
    $sql = "SELECT
                '<img
                        align=\"absmiddle\"
                        src=\"/imagens/alterar.gif\"
                        style=\"cursor: pointer\"
                        onclick=\"javascript: alterar('||ffcid ||' );\"
                        title=\"Alterar Formação\"
                     > &nbsp;
                     <img
                        align=\"absmiddle\"
                        src=\"/imagens/excluir.gif\"
                        style=\"cursor: pointer\"
                        onclick=\"javascript: excluir('||ffcid ||' );\"
                        title=\"Excluir Formação\"
                     > ' as acao, 
                ffccurso,
                ffcnomeinstituicao,
                ffccargahoraria,
                ffcanoconclusao
            FROM
                gestaopessoa.ftformacaocurso
            
            WHERE
                fdpcpf = '".$_SESSION['fdpcpf']."'
                
            ORDER BY ffccurso ASC
                ";
    $cabecalho = array("&nbsp;&nbsp;&nbsp;&nbsp;Ação", "Curso", "Nome da Instituição", "Carga Horária", "Ano de Conclusão" );
    $db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', '');
    
?>
<script>


function incluirCurso(){
    var ffcid				= document.getElementById('ffcid');
    var ffccurso			= document.getElementById('ffccurso');
    var ffcnomeinstituicao	= document.getElementById('ffcnomeinstituicao');
    var ffccargahoraria		= document.getElementById('ffccargahoraria');
    var ffcanoconclusao		= document.getElementById('ffcanoconclusao');

    if( ffccurso.value == ''){
        alert( 'Campo Curso é obrigatório.' );
        return false;
    }
    if( ffcnomeinstituicao.value == ''){
        alert( 'Campo Nome da Instituição é obrigatório.' );
        return false;
    }
    if( ffccargahoraria.value == ''){
        alert( 'Campo Carga Horária é obrigatório.' );
        return false;
    }
    if( ffcanoconclusao.value == ''){
        alert( 'Campo Ano de Conclusão é obrigatório.' );
        return false;
    }
    document.formulario.submit();
}
function excluir( ffcid ){
    if( confirm( 'Deseja realmente excluir?') ){
        window.location.href = 'gestaopessoa.php?modulo=principal/cadCursos&acao=A&ffcid='+ffcid;
    }
}
function alterar(id){
 
    var ffcid				= document.getElementById('ffcid');
    var ffccurso			= document.getElementById('ffccurso');
    var ffccargahoraria		= document.getElementById('ffccargahoraria');
    var ffcnomeinstituicao	= document.getElementById('ffcnomeinstituicao');
    var ffcanoconclusao		= document.getElementById('ffcanoconclusao');
    var alterar             = document.getElementById('alterar');
    var req = new Ajax.Request('gestaopessoa.php?modulo=principal/cadCursos&acao=A', {
                                    method:     'post',
                                    parameters: '&alteraAjax='+id,
                                    onComplete: function (res)
                                    {
                                         var arRes       = res.responseText.split("_");
                                                                                  
                                         var curso		= arRes[0];
                                         var nome		= arRes[1];
                                         var carga		= arRes[2];
                                         var ano		= arRes[3];
                                         
                                         ffccurso.value				= curso;
                                         ffccargahoraria.value		= carga;
                                         ffcnomeinstituicao.value	= nome;
                                         ffcanoconclusao.value		= ano;
                                         alterar.value				= id;

                                    }
                              });
}
</script>