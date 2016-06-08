<?php
include  APPRAIZ."includes/cabecalho.inc";
echo '<br>'; 

$bloquearEdicao = bloqueiaEdicaoFT();

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Força de Trabalho', 'Dados Funcionais Terceirizados' ); 

echo cabecalhoPessoa($_SESSION['fdpcpf']);

if( $_POST ){
	echo '<pre>';
	print_r( $_POST );
	die();
}
?>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<center> Preencha os Dados Funcionais  </center>
 <form name = "formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
         	<tr>
                <td class ="SubTituloDireita" align="right">Posto de Trabalho: </td>
                <td>
                <?
                	$sql = "SELECT 
                           ''
                       "; 
                
                $ungcod = $rsDadosEvento[0]["ungcod"];
                 
                $db->monta_combo('sercor', $sql, 'S', "Selecione...", '', '', '', '100', 'S', 'ungcod');
                ?>
                </td>
            </tr>
 			<tr>
                <td class ="SubTituloDireita" align="right">Nível: </td>
                <td>         
             		<?= campo_texto('nivel', 'S', $somenteLeitura, '', 20, 20, '#.###.###.###,##', '', 'left', '',  0, 'id="evecustoprevisto" onblur="MouseBlur(this);"' ); ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Unidade de Trabalho: </td>
                <td>
                <?
                	$sql = "SELECT 
                           ''
                       "; 
                
                $ungcod = $rsDadosEvento[0]["ungcod"];
                 
                $db->monta_combo('sercor', $sql, 'S', "Selecione...", '', '', '', '100', 'S', 'ungcod');
                ?>
                </td>
            </tr>
           <tr>
                <td class ="SubTituloDireita" align="right">Sala: </td>
                <td>         
             		<?= campo_texto('Sala', 'S', $somenteLeitura, '', 20, 20, '#.###.###.###,##', '', 'left', '',  0, 'id="evecustoprevisto" onblur="MouseBlur(this);"' ); ?>
                </td>
            </tr>
             <tr>
                <td class ="SubTituloDireita" align="right">Telefone: </td>
                <td>         
             		<?= campo_texto('Telefone', 'S', $somenteLeitura, '', 20, 20, '#.###.###.###,##', '', 'left', '',  0, 'id="evecustoprevisto" onblur="MouseBlur(this);"' ); ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Email: </td>
                <td>         
             		<?= campo_texto('Email', 'S', $somenteLeitura, '', 20, 20, '#.###.###.###,##', '', 'left', '',  0, 'id="evecustoprevisto" onblur="MouseBlur(this);"' ); ?>
                </td>
            </tr>
            <tr>
            	<td class ="SubTituloDireita" align="right">  </td>
            	<td><input type="button" name="btSalvar" id="btSalvar" onclick="validaForm();" value="Salvar" <?=$bloquearEdicao;?>></td>
            </tr>
     </table>
     </form>
 
<script>

function validaForm(){
	document.formulario.submit();
}
</script>