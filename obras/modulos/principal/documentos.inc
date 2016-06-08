<?php

$obras = new Obras();
$dobras = new DadosObra(null);

if( $_REQUEST["obrid"] ){
	
	include_once APPRAIZ . "www/obras/_permissoes_obras.php";
	
	session_unregister("obra");
	$_SESSION["obra"]["obrid"] = $_REQUEST["obrid"];
	
}

// Realiza as rotinas da tela 
switch($_REQUEST['requisicao']) {
	case "inserirarquivo":
		$dir = 'documentos&acao=A';
		$obras->EnviarArquivo($_FILES, $_POST, $dir);
		exit;
	break;
	case "download":
		$obras->DownloadArquivo( $_REQUEST );
	break;
	case "excluir":
		$obras->DeletarDocumento( $_REQUEST );
	break;
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";

$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo = "Documentos da Obra";
monta_titulo( $titulo_modulo, '' );

echo $obras->CabecalhoObras();

if(!$_SESSION["obra"]["obrid"]) {
	die("<script>
			alert('Variavel de obra não encontrada');
			window.location='obras.php?modulo=inicio&acao=A';
		 </script>");
}

?>

<form method="post" id="anexo" name="anexo" enctype="multipart/form-data" onsubmit="return ValidarFormulario(this);" action="<? echo $caminho_atual; ?>acao=A">
	<input type="hidden" name="requisicao" value="inserirarquivo"/>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
			<td>
				<?php if($habilitado){ ?>
					<input type="file" name="arquivo"/>
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
				<?php } ?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Tipo:</td>
			<td><?php
			
			$sql = "
				SELECT tpaid AS codigo, tpadesc AS descricao 
					FROM obras.tipoarquivo
			";
			
			$db->monta_combo('tpaid', $sql, $somenteLeitura, "Selecione...", '', '', '', '100', 'S', 'tpaid');
		?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
			<td><?= campo_textarea( 'arqdescricao', 'S', $somenteLeitura, '', 60, 2, 250 ); ?></td>
		</tr>
		<tr style="background-color: #cccccc">
			<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
			<td>
				<?php if($habilitado){ ?>
					<input type="button" name="botao" value="Salvar" onclick="validaForm();";/>
				<?php } ?>
			</td>
		</tr>
	</table>
</form>
<table border="0" cellspacing="0" cellpadding="3" align="center" class="Tabela">
	<?	
		if($habilitado){
			$permissaoBotaoExcluir = "'<center><a href=\"#\" onclick=\"javascript:ExcluirDocumento(\'" . $caminho_atual . "acao=A&requisicao=excluir" . "\',' || arq.arqid || ',' || aqb.aqoid || ');\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a></center>' as acao,";	
		}else{
			$permissaoBotaoExcluir = "'<center><img src=\"/imagens/excluir_01.gif\" title=\"Este Documento não pode ser excluído!\"></center>' as acao,";
		}	
		$sql = "SELECT
						{$permissaoBotaoExcluir}
						to_char(aqb.aqodtinclusao,'DD/MM/YYYY'),
						tarq.tpadesc,
						'<a style=\"cursor: pointer; color: blue;\" onclick=\"DownloadArquivo(' || arq.arqid || ');\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a>',
						arq.arqtamanho || ' kbs' as tamanho ,
						arq.arqdescricao,								
						usu.usunome
					FROM
						((public.arquivo arq INNER JOIN obras.arquivosobra aqb
						ON arq.arqid = aqb.arqid) INNER JOIN obras.tipoarquivo tarq
						ON tarq.tpaid = aqb.tpaid) INNER JOIN seguranca.usuario usu
						ON usu.usucpf = aqb.usucpf
					WHERE
						aqb.aqostatus = 'A' AND	aqb.obrid = '" . $_SESSION["obra"]["obrid"] . "'
						AND (arqtipo <> 'image/jpeg' AND arqtipo <> 'image/png' AND arqtipo <> 'image/gif')";
		
		$cabecalho = array( "Ação", 
							"Data Inclusão",
							"Tipo Arquivo",
							"Nome Arquivo",
							"Tamanho (Mb)",
							"Descrição Arquivo",
							"Responsável");
		$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '' );
	?>
</table>
<script type="text/javascript">
	function validaForm (){
		
		var tpaid = document.getElementById('tpaid');

		if ( tpaid.value == "" ){
			alert("Campo obrigatório.");
			tpaid.focus();
			return false;
		}

		document.getElementById('anexo').submit();
	}
</script>
<?php chkSituacaoObra(); ?>