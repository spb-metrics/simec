<?php

ini_set("memory_limit", "2048M");

switch ($_REQUEST["requisicao"]) {
	case "importar":
		importarObras();
	break;
	default:
	break;
}


require_once APPRAIZ . 'includes/cabecalho.inc';
require_once APPRAIZ . "www/obras/permissoes.php";

echo '<br/>';
monta_titulo( 'Importação de Obras', '');

?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
	<tr>
		<td align="center">
			<br/>
			<a style="cursor:pointer;" onclick="importaObras();">Clique aqui para importar as obras</a>
			<br/><br/>
		</td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td>
			<input style="cursor:pointer;" type="button" value="Voltar" onclick="history.back(-1);"/>
		</td>
	</tr>
</table>	
