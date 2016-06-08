<?

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Relatório Módulo Financeiro', 'Relatório da Junta 2009' );

?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="3"	align="center">
	<tr><td>&nbsp;</td></tr>
	<tr>
		<td align="center"><font color="red">Clique para gerar o Relatório da Junta 2009</font></td>
	</tr>
	<tr>
		<td align="center">
			<input type="button" id="bt_gerar_relatorio" onclick="abrePopupRelatorio();" value="Gerar Relatório" />
		</td>
	</tr>
	<tr><td>&nbsp;</td></tr>
</table>

<script type="text/javascript">

function abrePopupRelatorio() {
	var janela = window.open( 'financeiro.php?modulo=relatorio/resultado_execucao2008&acao=A', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	janela.focus();
}

</script>