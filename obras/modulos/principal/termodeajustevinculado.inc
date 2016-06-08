<?php

require_once APPRAIZ . 'includes/cabecalho.inc';
require_once APPRAIZ . "www/obras/permissoes.php";

echo "<br/>";
$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( "Termos de Ajuste", "");

$obras  = new Obras();
echo $obras->CabecalhoObras();

$sql = "SELECT
			'<center><img style=\"cursor:pointer;\" src=\"/imagens/consultar.gif\" border=\"0\" title=\"Visualizar Termo\" onclick=\"verTermoVinculado(' || ta.traid || ');\"></center>' as acao,
			ta.traassunto as assunto,
			ta.tralocal as local,
			to_char(tradtcriacao, 'DD/MM/YYYY') as data,
			usunome as nome
		FROM
			obras.termoajuste ta
		INNER JOIN
			obras.obratermoajuste ot ON ot.traid = ta.traid
		INNER JOIN
			seguranca.usuario su ON su.usucpf = ta.usucpf  
		WHERE
			ot.obrid = {$_SESSION["obra"]["obrid"]}";

$cabecalho = array( "Ação", "Assunto", "Local", "Data de Criação", "Inserido Por" );
$db->monta_lista( $sql, $cabecalho, 100, 30, 'N', 'center', '' );

?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr bgcolor="#C0C0C0">
		<td>
			<input type="button" value="Voltar" style="cursor:pointer;" onclick="history.back(-1);"/>
		</td>
	</tr>
</table>