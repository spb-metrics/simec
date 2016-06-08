<?php
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo '<br />';
monta_titulo('Relatório Módulo Financeiro', 'Gerenciamento Dinâmico de Contas Contábeis - Parâmetros');


if ($_GET['icbcod']){
	session_start();
	$_SESSION['icbcod'] = $_GET['icbcod'];

	
	
        $sql ="SELECT
				icbdscresumida, 
				icbdsc,
				icbdatainiciovalidade, 
				icbdatafimvalidade, 
				pc.gr_codigo_conta
			FROM 
				financeiro.informacaocontabil icb
				INNER JOIN financeiro.informacaoconta ic ON ic.icbcod = icb.icbcod 
				INNER JOIN siafi.planoconta pc ON pc.gr_codigo_conta = ic.gr_codigo_conta
			WHERE
			    ic.icbcod = ".$_GET['icbcod'].";
			  ";
			    
        $dados = $db->carregar($sql);
                
        $icbdscresumida 	   = $dados[0]['icbdscresumida'];
        $icbdsc                = $dados[0]['icbdsc'];
        $icbdatainiciovalidade = $dados[0]['icbdatainiciovalidade'];
        $icbdatafimvalidade    = $dados[0]['icbdatafimvalidade'];
        $gr_codigo_conta 	   = $dados[0]['gr_codigo_conta'];
        
        if($icbdscresumida == 'icbdscresumida'){
        	
        }
        
}

?>
<html>
	<head>
	<script type="text/javascript" src="../includes/calendario.js"></script>
	</head>
	
<body>
<form action="financeiro.php?modulo=relatorio/consulta_gerenciamento_dinamico&acao=A" method="POST" name="formulario">
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
			<tr>
				<td class="SubTituloDireita" valign="top">Descrição Resumida</td>
				<td>
					<?= campo_texto( 'icbdscresumida', 'N', 'S', '', 78, 100, '', '' ); ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="top">Descrição</td>
				<td>
					<?= campo_texto( 'icbdsc', 'N', 'S', '', 140, 100, '', '' ); ?>
				</td>
			</tr>
			<tr>
				<td width="195" id="" class="SubTituloDireita" valign="top">Conta Contábil</td>
				<td>
				
				<?php
				// inicia agrupador
				$origem = $db->carregar("SELECT 
												DISTINCT 
												gr_codigo_conta as codigo,  
												gr_codigo_conta as descricao
										  FROM 
										        siafi.planoconta 
										  WHERE
										  		it_in_conta_corrente_contabil in ('16','17','31','80','26','45','50','77')
  										  ORDER BY 
  										        gr_codigo_conta");
					
		        $agrupador = new Agrupador( 'formulario', $agrupadorHtml );
                $destino = array();
                           
                // exibe agrupador
	            $agrupador->setOrigem( 'naoAgrupadoMacro', null, $origem );
	            $agrupador->setDestino( 'agrupadorMacro', null, $destino );
	            $agrupador->exibir();
	              
 				?>
					
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="top">Data Inicio Validade </td>
				<td>
						<?=campo_data('icbdatainiciovalidade', 'N', 'S', '', 'S' );?>					
				</td>	
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="top">Data Fim Validade</td>
				<td>
						<?=campo_data('icbdatafimvalidade', 'N', 'S', '', 'S' );?>	
				</td>
			</tr>
		</table>
		
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
			<tr>
				<td align="center">
					<input type="submit" name="Consultar" value="Consultar" onclick="javascript:submeterFormulario();"/>
				</td>
			</tr>
	</table>


</form>
</body>
</html>

<script language="JavaScript">
function submeterFormulario(){
	 selectAllOptions( document.formulario.agrupadorMacro );	
	 document.formulario.submit();
}
	function Envia(id)
	{
		//document.formulario.submit();
		window.location = 'financeiro.php?modulo=relatorio/consulta_gerenciamento_dinamico&acao=A&icbcod='+id;
	}
</script>