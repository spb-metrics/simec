<?php

// cria o objeto da classe termoDeAjuste
$termodeajuste = new termoDeAjuste();

// limpa a sessão do termo
$_SESSION["obra"]["traid"] = null;

// pega os orgão (tipo de ensino) permitidos para o usuário
$res   = obras_pegarOrgaoPermitido();
$orgidRes = array();

if( is_array($res) ){
	for( $i = 0; $i < count( $res ); $i++ ){
		foreach( $res[$i] as $chave=>$valor ){
			if ( $chave == "id" ){
				if($valor){
					$orgidRes[] = $valor;
				}
			}
		}
	}
}

// realiza a pesquisa com os filtros, caso sejam informados
if ( $_REQUEST["pesquisa"] == "pesquisa" ){
	$filtroTermo = $termodeajuste->PesquisaTermoAjuste( $_REQUEST );
}

// deleta o termo de ajuste
if ( $_REQUEST["requisicao"] == "excluir" ){
	$termodeajuste->DeletaTermoAjuste( $_REQUEST["traid"] );
}

// cabeçalho padrão do sistema
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

// monta o título da tela
print '<br/>';

echo montarAbasArray( array(0 => array ( "id" => 1, 
										 "descricao" => "Lista de Termos de Ajuste",  
										 "link" => "/obras/obras.php?modulo=principal/lista_de_termos&acao=A" )
							), '' );
monta_titulo( 'Lista de Termos de Ajuste', '' );


$orgid = $_REQUEST["orgid"];
 
$habil_org = ( count($orgidRes) > 1 ) ? "S" : "N";

if( sizeof($orgidRes) > 0 ){
	$where = "orgid in ( " . implode(", ", $orgidRes) . " )";
	$habil = "S";
	$texto = "";
}else{
	$where = "1=2";
	$texto = "<label style=\"color: red; \">Você não possui as atribuições necessárias para operar esta página.</label>";
	$disabled = "disabled=\"disabled\"";
	$habil = "N";
}
?>
<script src="../includes/calendario.js"></script>
<form action="" method="post" name="formulario" id="formulario">
	<input type="hidden" name="pesquisa" id="pesquisa" value="pesquisa"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td class="subtitulocentro" colspan="2">Argumentos de Pesquisa<br><?=$texto ?></br></td>
		</tr>
		<tr>
			<td class="subtitulodireita" width="180px;">Tipo de Estabelecimento</td>
			<td>
				<?php
					
					
					$sql = "SELECT
								orgid as codigo,
								orgdesc as descricao
							FROM
								obras.orgao
							WHERE
								{$where}";
					
					$db->monta_combo("orgid", $sql, $habil_org, "Selecione...", '', '', '', '', 'S','orgid');
					
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita" width="180px;">Assunto</td>
			<td>
				<?php
					$traassunto = $_REQUEST["traassunto"];
					echo campo_texto( 'traassunto', 'N', $habil, '', 47, 60, '', '', 'left', '', 0, 'id="traassunto"');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita" width="180px;">Inserido Por</td>
			<td>
				<?php
					
					$usucpf = $_REQUEST["usucpf"];
				
					$sql = "SELECT DISTINCT
								us.usucpf as codigo,
								us.usunome as descricao
							FROM
								seguranca.usuario us
							INNER JOIN
								obras.termoajuste ot ON ot.usucpf = us.usucpf
							WHERE
								trastatus = 'A'
							ORDER BY
								descricao";
					
					$db->monta_combo("usucpf", $sql, $habil, "Selecione...", '', '', '', '250', 'N','usucpf');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita" width="180px;">Data de Criação</td>
			<td>
				<?php
					
					echo campo_data( 'tradtcriacao', 'N', $habil, '', 'S' );
				?>
			</td>
		</tr>
		<tr bgcolor="#DCDCDC">
			<td></td>
			<td>
				<input type="button" value="Pesquisar" style="cursor:pointer;" onclick="validaForm();" <?=$disabled ?>/>
			</td>
		</tr>
	</table>
</form>
<?php

// monta a lista com os termos cadastrados, filtrado por tipo de ensino
$sql = "SELECT
			'<center>
				<img src=\"/imagens/check_p.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"atualizaTermo(' || ta.traid || ', ' || ta.orgid || ');\">
				<img src=\"/imagens/exclui_p.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"excluiTermo(' || ta.traid || ');\">
			 </center>' as acao,
			orgdesc as tipoensino, 
			'<a style=\"cursor:pointer;\" onclick=\"atualizaTermo(' || ta.traid || ', ' || ta.orgid || ');\">' || traassunto || '<a/>' as assunto,
			tralocal as local,
			to_char(tradtcriacao, 'DD/MM/YYYY') as data,
			usunome as nome
		FROM
			obras.termoajuste ta
		INNER JOIN
			seguranca.usuario su ON ta.usucpf = su.usucpf
		INNER JOIN
			obras.orgao oo ON oo.orgid = ta.orgid
		WHERE
			trastatus = 'A' {$filtroTermo}";

$cabecalho = array( "Ação", "Tipo de Estabelecimento", "Assunto", "Local", "Data de Criação", "Inserido Por" );
$db->monta_lista( $sql, $cabecalho, 100, 30, 'N', 'center', '' );

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
	<tr bgcolor="#DCDCDC">
		<td>
			<input type="button" value="Cadastrar Termo" style="cursor:pointer;" onclick="cadastraTermo();"/>
		</td>
	</tr>
</table>
<script>
function validaForm(){

	var orgid = document.getElementById('orgid');
	if( orgid.value == '' ){
		alert('Campo "Tipo de Estabelecimento" obrigatório');
		orgid.focus();
		return false;
	}
	pesquisarTermo();
}
</script>
