<?php
ini_set("memory_limit","250M");
set_time_limit(0);
// gerando o extrato do histórico
if( isset($_GET['halid']) ){
	$html = gerarExtratoObrasHTML($_GET['halid']);
	echo $html;
	exit();
}

// fazendo a comparação entre os dois históricos
if( isset($_GET['halid1']) && isset($_GET['halid2']) ){
	$html = compararExtratoObras($_GET['halid1'], $_GET['halid2']);
	echo $html;
	exit();
}

// Verificando se o obrid está salvo na sessão
if( !$_SESSION["obra"]['obrid'] ){
       header( "location:obras.php?modulo=inicio&acao=A" );
       exit;
}

// cabecalho padrão do simec
include APPRAIZ . "includes/cabecalho.inc";
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

function VisualizarHistorico(url, halid){
	var janela = window.open( url+'&halid='+halid, 'visualizarExtrato', 'width=780, height=465, status=1, menubar=0, toolbar=0, scrollbars=1, resizable=1' );
	janela.focus();
}

function ValidaCheckBox(id){

	//contador de checkboxes
	var checkbox = 0;

	//verificando a quantidade de checkboxes marcadas
	$('input[type=checkbox]').each(function () {
        if(this.checked) {
        	checkbox++;
		}
    });

    //Não é permitido selecionar mais de duas opções, então desabilite o checkbox clicado
	if(checkbox > 2){
		alert('Não é permitido selecionar mais de 2 opções.');
		$('#halid_'+id).attr('checked', false);
	}

}

function ComparaHistorico() {

	//array que irá armazenar o valor dos checkboxes marcados
	var halids = new Array();
	//contador
	var i = 0;

	//Pegando o value dos checkboxes marcados
	$('input[type=checkbox]').each(function () {
        if(this.checked) {
        	halids[i] = this.value;
        	i++;
		}
    });

	if(i > 1){
		var janela = window.open( '?modulo=principal/supervisao/historico_obras&acao=A'+'&halid1='+halids[0]+'&halid2='+halids[1], 'Comparar', 'width=780, height=465, status=1, menubar=0, toolbar=0, scrollbars=1, resizable=1' );
		janela.focus();
	}else{
		alert('Por favor, selecione duas opções.');
	}
	
}

</script>
<?php
echo "<br>";
// Cria o título da tela
$titulo_modulo = "Histórico de Obras";
$db->cria_aba( $abacod_tela, $url, $parametros );
monta_titulo( $titulo_modulo, "&nbsp");

// Cabeçalho
$obras = new Obras();
echo $obras->CabecalhoObras();

//'<center> Isto servirá para a comparação dos históricos
//	<input type=\"checkbox\" id=\"halid_'|| hs.halid ||'\" value=\"'|| hs.halid ||'\" /> 
//</center>' as acao,

$sql = "SELECT
			'<center>
				<img src=\"/imagens/alterar.gif\" border=\"0\" title=\"Visualizar\" onclick=\"javascript:VisualizarHistorico(\'?modulo=principal/supervisao/historico_obras&acao=A\', \''|| hs.halid ||'\');\">
			 </center>'	as extrato,
			'<center>
				<input type=\"checkbox\" id=\"halid_'|| hs.halid ||'\" value=\"'|| hs.halid ||'\" onclick=\"ValidaCheckBox(\''|| hs.halid ||'\');\" /> 
			</center>' as comparacao,
			to_char(ha.haldata, 'DD/MM/YYYY') as data, 
			u.usunome as nome, 
			hs.orsid
		FROM
			seguranca.historicoalteracao ha
		INNER JOIN obras.historicosupervisao hs ON hs.halid = ha.halid
												AND hs.obrid = {$_SESSION['obra']['obrid']}
        INNER JOIN
            seguranca.usuario u ON u.usucpf = ha.usucpf";

$db->monta_lista($sql, array( "Extrato", "Comparar", "Data de Geração do Histórico", "Usuário", "Nº da O.S"  ), 50, 20, '', 'center', '');

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
	<tr>
		<td class="SubTituloEsquerda"><input type="button" value="Comparar" onclick="javascript:ComparaHistorico();" /></td>
	</tr>
</table>
