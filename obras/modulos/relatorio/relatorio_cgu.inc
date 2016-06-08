<?php

include APPRAIZ . 'includes/cabecalho.inc';

echo "<br>";
$titulo_modulo = "Relatório de Obras do Programa REUNI";
monta_titulo( $titulo_modulo, '' );

?>

<script type="text/javascript">
	
	function abreRelatorio(  ){
		window.open('obras.php?modulo=relatorio/resultado_relatorio_cgu&acao=A', 'relatorio', 'width=900,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1')	
	}
	
</script>

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<tr>
		<td align="center">
			<br/>
			<a style="cursor:pointer;" onclick="abreRelatorio();">Clique aqui para gerar o Relatório de Obras do Programa REUNI</a>
			<br/><br/>
		</td>
	</tr>
</table>