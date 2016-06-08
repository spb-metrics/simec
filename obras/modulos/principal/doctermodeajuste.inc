<?php

/**
 * Tela com a finalidade de cadastrar/excluir os anexos dos termos de ajuste das obras.
 * 
 * @author Fernando Araújo Bagno da Silva 
 * @since 19/08/2009
 * @version 1.0
 */

// cria o objeto da classe termoDeAjuste
$termodeajuste = new termoDeAjuste();

$termodeajuste->verificaSessao();

// cria a variável com o ID do termo, caso exista
$traid = !empty($_SESSION['obra']['traid']) ? $_SESSION["obra"]["traid"] : '';

// cadastra, exclui e efetua o download do arquivo 
switch( $_REQUEST["requisicao"] ){
	case "insere":
		$termodeajuste->CadastraAnexo( $_REQUEST, $_FILES );
	break;
	case "excluir":
		$termodeajuste->DeletaAnexo( $_REQUEST["ataid"] );
	break;
	case "download":
		$termodeajuste->DownloadArquivo( $_REQUEST["arqid"] );
	break;
}

// cabeçalho padrão do sistema
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

// cria o título e as abas da tela
echo '<br/>';
$db->cria_aba( $abacod_tela, $url, $parametros );
monta_titulo( 'Documentos do Termo de Ajuste', '' );

$termodeajuste->cabecalho( $traid );

?>

<form action="" method="post" enctype="multipart/form-data" name="formulario" id="formulario">
	<input type="hidden" name="requisicao" value="insere"/>
	<input type="hidden" name="traid" value="<?php echo $traid; ?>"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td width="180px" class="SubTituloDireita">Arquivo</td>
			<td>
				<?php if($habilitado){ ?>
					<input type="file" name="arquivo" id="file"/>
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
				<?php } ?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Tipo:</td>
			<td>
				<?php
			
					$sql = "SELECT tpaid AS codigo, tpadesc AS descricao 
							FROM obras.tipoarquivo 
							WHERE tpaid in (2,3,6,8,9,10,11,12,13,15,20,21,22)
							ORDER BY tpadesc";
					
					$db->monta_combo('tpaid', $sql, $somenteLeitura, "Selecione...", '', '', '', '', 'S','tpaid');
				?>
			</td>
		<tr>
			<td class="SubTituloDireita" >Descrição</td>
			<td><?= campo_textarea( 'arqdescricao', 'S', $somenteLeitura, '', 60, 5, 250 ); ?></td>
		</tr>
		<tr bgcolor="#DCDCDC">
			<td></td>
			<td>
				<input type="button" value="Salvar" onclick="obrasValidaAnexoTermoAjuste();"/>
				<input type="button" value="Voltar" onclick="history.back(-1)"/>
			</td>
		</tr>
	</table>
</form>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
	<tr>
		<td colspan="2" class="subtitulocentro">Lista de Documentos</td>
	</tr>
</table>
<?php

	$sql = "SELECT
				'<center>
					<img src=\"/imagens/exclui_p.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"deletaAnexo(' || ataid || ');\">
				 </center>' as acao,
				to_char(at.atadtinclusao, 'DD/MM/YYYY') as data,
				tpadesc as tipo,
				'<a style=\"cursor: pointer; color: blue;\" onclick=\"downloadAnexo(' || at.arqid || ');\" />' || pa.arqnome || '.' || pa.arqextensao || '</a>' as nome,
				pa.arqtamanho || ' kbs' as tamanho,
				pa.arqdescricao as descricao,
				usunome	as nomeresponsavel		
			FROM
				obras.anexotermoajuste at
			INNER JOIN
				public.arquivo pa ON pa.arqid = at.arqid
			INNER JOIN
				seguranca.usuario su ON su.usucpf = at.usucpf
			INNER JOIN
				obras.tipoarquivo ta ON ta.tpaid = at.tpaid
			WHERE
				atastatus = 'A' AND traid = {$traid}";
	
	$cabecalho = array( "Ação", "Data Inclusão", "Tipo", "Nome do Arquivo", "Tamanho (Mb)", "Descrição Arquivo", "Anexado Por" );
	$db->monta_lista( $sql, $cabecalho, 100, 30, 'N', 'center', '' );

?>
