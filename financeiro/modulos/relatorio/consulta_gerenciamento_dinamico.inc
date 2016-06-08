<?php

//header("Cache-Control: no-store, no-cache, must-revalidate");
//header("Cache-Control: post-check=0, pre-check=0", false);
//header("Pragma: no-cache");



//if ($_REQUEST['evento'] == ''){ 
//if ($_GET['icbcod']){
//	session_start();
//	$_SESSION['icbcod'] = $_GET['icbcod'];
//	echo "<script>
//			window.location.href = 'financeiro.php?modulo=relatorio/consulta_gerenciamento_dinamico&acao=A';
//		  </script>";
//	//header( "Location: financeiro.php?modulo=relatorio/consulta_gerenciamento_dinamico&acao=A" );
//	exit();
//	}
//}

include  APPRAIZ . 'includes/cabecalho.inc';

echo '<br />';
monta_titulo('Relatório Módulo Financeiro', 'Gerenciamento Dinâmico de Contas Contábeis - Consulta');

?>

<?php

$agrupadorHtml =
<<<EOF
    <table>
        <tr valign="middle">
            <td>
                <select id="{NOME_ORIGEM}" name="{NOME_ORIGEM}[]" multiple="multiple" size="4" style="width: 140px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );" class="combo campoEstilo"></select>
            </td>
            <td>
                <img src="../imagens/rarrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>
                <!--
                <img src="../imagens/rarrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>
                <img src="../imagens/larrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, ''); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>
                -->
                <img src="../imagens/larrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>
            </td>
            <td>
                <select id="{NOME_DESTINO}" name="{NOME_DESTINO}[]" multiple="multiple" size="4" style="width: 140px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );" class="combo campoEstilo"></select>
            </td>
            <td>
                <img src="../imagens/uarrow.gif" style="padding: 5px" onClick="subir( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
                <img src="../imagens/darrow.gif" style="padding: 5px" onClick="descer( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
            </td>
        </tr>
    </table>
    <script type="text/javascript" language="javascript">
        limitarQuantidade( document.getElementById( '{NOME_DESTINO}' ), {QUANTIDADE_DESTINO} );
        limitarQuantidade( document.getElementById( '{NOME_ORIGEM}' ), {QUANTIDADE_ORIGEM} );
        {POVOAR_ORIGEM}
        {POVOAR_DESTINO}
        sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );
    </script>
EOF;

include APPRAIZ . 'includes/Agrupador.php';
        
?>

<head>
	<!-- <script type="text/javascript" src="../includes/calendario.js"></script> -->
	<script type="text/javascript"></script>
</head>
<form  onSubmit="return validaForm(this);" action="" method="post" name="formulario" id="formulario">
		
	<!-- ----------------------------------------------------------------------- -->

</form>

<?php
/**
 * Redireciona o navegador para a tela indicada.
 * 
 * @return void
 */
function redirecionar( $modulo, $acao, $parametros = array() )
{
    $parametros = http_build_query( (array) $parametros, '', '&' );
    header( "Location: ?modulo=$modulo&acao=$acao&$parametros" );
    exit();
}

switch( $_REQUEST['evento'] ){


    case 'excluir_anexo':
        $sql     ="SELECT 
        				* 
        		   FROM 
        		        financeiro.informacaocontabil 
        		   WHERE 
        		   		icbcod = ".$_REQUEST['icbcod'];
        
        $arquivo = $db->pegaLinha($sql);

        $sql = sprintf( " DELETE FROM
									financeiro.informacaoconta
						  WHERE 
						        	icbcod = %d", 
        							$_REQUEST['icbcod'] );
        							
        if( !$db->executar( $sql ) ){
            $_SESSION['MSG_AVISO'][] = "Não foi possível remover o documento.";
            $db->rollback();
        } else {
            // enquanto nao pode ser removido de verdade... fica comentáda a linha
            //if(unlink($this->arquivo_caminho))
           // {

            $db->commit();

            if (is_file(APPRAIZ . 'arquivos/elabrev/'.$arquivo['doccaminho'])) {
                if (unlink(APPRAIZ . 'arquivos/elabrev/'.$arquivo['doccaminho'])) {
                    $_SESSION['MSG_AVISO'][] = "";
                } else {
                    $_SESSION['MSG_AVISO'][] = "";
                }
            } else {
                $_SESSION['MSG_AVISO'][] = "";
            }
        }

        //redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
        break;

        default:
        break;

}

?>

<table class="tabela" bgcolor="#fbfbfb" cellspacing="0" cellpadding="10" align="center">
    <tr>
        <td>
        <!-- LISTA DE DOCUMENTOS -->
    <?php
    //	dbg( $_REQUEST, true  );
	$filtro = '';
	$filtro .= ( $_POST['icbdscresumida'] ) ? " icbdscresumida like '%" . $_POST['icbdscresumida'] . "%' AND " : '';
	$filtro .= ( $_POST['icbdsc'] ) ? " icbdsc like '%" . $_POST['icbdsc'] . "%' AND " : '';
	
	// Transforma a data para YYYY-MM-DD
	if ( $_POST['icbdatainiciovalidade'] ){
		$dataini = explode("/", $_POST['icbdatainiciovalidade'] );
		$dataini = "{$dataini[2]}-{$dataini[1]}-{$dataini[0]}";
		$filtro .= " icbdatainiciovalidade >= '" . $dataini . "' AND "; 
	}
	
	// Transforma a data para YYYY-MM-DD
	if ( $_POST['icbdatafimvalidade'] ){
		$datafim = explode("/", $_POST['icbdatafimvalidade'] );
		$datafim = "{$datafim[2]}-{$datafim[1]}-{$datafim[0]}";
		$filtro .= " icbdatafimvalidade <= '" . $datafim . "' AND "; 
	}
	
	
	// Verifica se foi escolhido conta contábil
	if ( $_POST['agrupadorMacro'] ) {
		// Verifca se veio em array
		if ( is_array( $_POST['agrupadorMacro'] ) ){
			// Navega pelo array para recuperar os valores
			$filtro .= " pc.gr_codigo_conta in ( '" . implode( $_POST['agrupadorMacro'], "', '") . "' ) AND ";
		}
	}

	
        $sql ="SELECT DISTINCT
					icb.icbcod,        		
        			'<img title=\"Alterar o Cadastro\" align=\"absmiddle\" border=\"0\" src=\"../imagens/alterar.gif\" onclick=\"altera_contas(' || icb.icbcod || ')\">
        			 <img title=\"Excluir o Cadastro\" align=\"absmiddle\" border=\"0\" src=\"../imagens/excluir.gif\" onclick=\"excluir_anexo(' || icb.icbcod || ')\">
        			' AS acao, 		
        			icbdscresumida, 
					icbdatainiciovalidade, 
					icbdatafimvalidade, 
					pc.gr_codigo_conta
				FROM 
					financeiro.informacaocontabil icb
					INNER JOIN financeiro.informacaoconta ic ON ic.icbcod = icb.icbcod 
					INNER JOIN siafi.planoconta pc ON pc.gr_codigo_conta = ic.gr_codigo_conta
              ";
        $sql .= ( $filtro ) ? ' WHERE ' . substr( $filtro, 0, -4 ): '';
        $dados = $db->carregar($sql);
        
//        dbg($dados);
//        die();

        if( count($dados) > 1 )
	    {
	        foreach($dados as $val){
	
	        	if ($val['icbcod'] != $icbcodAtual){
	        		$z += isset($z) ? 1 : 0;
	        		//echo $z;
					$dados1[$z] = array("acao" 	   => $val['acao'], 
										"resumido" => $val['icbdscresumida'], 
										"inicio"   => formata_data($val['icbdatainiciovalidade']), 
										"fim"      => formata_data($val['icbdatafimvalidade']) , 
										"contas"   => "<div style='color: rgb(0, 102, 204);'>".$val['gr_codigo_conta']."</div>");
					
	        		$icbcodAtual = $val['icbcod'];
	        	}else{
	        		echo("<p style=\"color: rgb(0, 102, 204);\" align=\"rigth\">");
	        		$dados1[$z]['contas'] .= "<div style='color: rgb(0, 102, 204);'>".$val['gr_codigo_conta']."</div>"; 
	        		echo("</p>");
	        	}
	        	
	        
	        }
	                
	        $cabecalho = array("Ações", "Descrição Resumida", "Início Validade", "Fim Validade", "Contas Contábeis");
	        
	        $db->monta_lista_array($dados1, $cabecalho, 10, 20, '', '', '');
	                
	        $dados = $db->carregar( $sql );
	        //
		
	    } 
	    
        else
        { 
        ?>
            <table class='tabela' style="width:100%;" cellpadding="3">
                <tbody>
                    <td style="text-align:center;padding:15px;background-color:#f5f5f5;">
                        Nenhum Documento Encontrado.
                    </td>
                </tbody>
            </table>
        <?php
        }
        ?>
            
        </td>
    </tr>
</table>

<script>
function altera_contas(id)
{
window.location = 'financeiro.php?modulo=relatorio/rel_gerenciamento_dinamico&acao=A&icbcod='+id;
	//alert(id);
}

function excluir_anexo( id )
{
    if ( confirm( 'Deseja excluir o cadastro?' ) ) {
    window.location.href = '?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&evento=excluir_anexo&icbcod='+ id;
    //alert('?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&evento=excluir_anexo&icbcod='+ id);
    }
}
</script>

