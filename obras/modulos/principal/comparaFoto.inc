<?php
$obras = new Obras();
$dobras = new DadosObra(null);

if( $_REQUEST["obrid"] ){	
	require_once APPRAIZ . "www/obras/permissoes.php";
	include_once APPRAIZ . "www/obras/_permissoes_obras.php";
	session_unregister("obra");
	$_SESSION["obra"]["obrid"] = $_REQUEST["obrid"];
}

if($_REQUEST["requisicao"]){
	
	function salvarComparacaoFoto()
	{
		global $db;
		
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		
		if(!$_FILES['foto1'] || !$_FILES['foto2'] || !$_POST['arqdescricao']){
			return array("msg" => "Favor preencher todos os campos obrigatórios!");
		}
		$arrCampos = array("obrid" => $_SESSION["obra"]["obrid"]);
		$file = new FilesSimec("compararfoto",$arrCampos,"obras");
		
		if(!$file->setUpload($_POST['arqdescricao'],"foto1")){
			return array("msg" => "Não foi possível fazer o upload do arquivo!");
		}
		
		$arrCampos = array("obrid" => $_SESSION["obra"]["obrid"] ,"arqidpar" => $file->getIdArquivo());
		$file2 = new FilesSimec("compararfoto",$arrCampos,"obras");
		if(!$file2->setUpload($_POST['arqdescricao'],"foto2",false)){
			return array("msg" => "Não foi possível fazer o upload do arquivo!");
		}
		
		$sql = "update obras.compararfoto set arqidpar = {$file2->getIdArquivo()} where arqid = {$file->getIdArquivo()} and obrid = {$_SESSION["obra"]["obrid"]}";
		$db->executar($sql);
		if($db->commit($sql)){
			return array("msg" => "Operação realizada com sucesso!");
		}
	}
	
	function excluirComparacaoFoto()
	{
		global $db;
		
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		
		$cmfid = $_POST['cmfid'];
		if(!$cmfid){
			return array("msg" => "Não foi possível realizar a operação!");
		}
		$sql = "select arqid, arqidpar from obras.compararfoto where cmfid = $cmfid";
		$arrArq = $db->pegaLinha($sql);

		$arrCampos = array("obrid" => $_SESSION["obra"]["obrid"]);
		$file = new FilesSimec("compararfoto",$arrCampos,"obras");
		$file->excluiArquivoFisico($arrArq['arqid']);
		$file->excluiArquivoFisico($arrArq['arqidpar']);
		
		$sql = "update obras.compararfoto set cmfstatus = 'I' where cmfid = $cmfid";
		$db->executar($sql);
		
		if($db->commit($sql)){
			return array("msg" => "Operação realizada com sucesso!");
		}else{
			return array("msg" => "Não foi possível realizar a operação!");
		}
	}
	
	$arrMsg = $_REQUEST["requisicao"]();
}

include APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";

$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo = "Comparativo de Fotos";
monta_titulo( $titulo_modulo, '' );

echo $obras->CabecalhoObras();

if(!$_SESSION["obra"]["obrid"]) {
	die("<script>
			alert('Variável de obra não encontrada!');
			window.location='obras.php?modulo=inicio&acao=A';
		 </script>");
}
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<script>
function salvarComparacaoFoto()
{
	var erro = 0;
	$("[class~=obrigatorio]").each(function() { 
		if(!this.value){
			erro = 1;
			alert('Favor preencher todos os campos obrigatórios!');
			this.focus();
			return false;
		}
	});
	if(erro == 0){
		$("#btn_salvar").attr("disabled","disabled");
		$("#btn_salvar").val("Carregando...");
		$("#formulario").submit();
	}
}

function excluirComparacao(cmf)
{
	if(confirm("Deseja realmente excluir a comparação de fotos?")){
		$("[name=cmfid]").val(cmf);
		$("[name=requisicao]").val("excluirComparacaoFoto");
		$("#formulario").submit();	
	}
}

function exibirComparacao(cmf)
{
	janela("obras.php?modulo=principal/popupComparaFoto&acao=A&cmfid=" + cmf,800,600,"Comparação de Fotos");
}

</script>

<form method="post" id="formulario" name="formulario" enctype="multipart/form-data" action="" >
	
	<input type="hidden" name="requisicao" value="salvarComparacaoFoto"/>
	<input type="hidden" name="cmfid" value=""/>
	
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Foto 1:</td>
			<td>
				<?php if($habilitado){ ?>
					<input class="obrigatorio" type="file" name="foto1"/>
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
				<?php }?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Foto 2:</td>
			<td>
				<?php if($habilitado){ ?>
					<input class="obrigatorio" type="file" name="foto2"/>
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
				<?php }?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
			<td><?= campo_textarea( 'arqdescricao', 'S', $somenteLeitura, '', 60, 2, 250 ); ?></td>
		</tr>
		<tr style="background-color: #cccccc">
			<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
			<td>
				<input type="button" name="botao" id="btn_salvar" value="Salvar" <?php if($somenteLeitura=="N") echo "disabled"; ?> onclick="salvarComparacaoFoto();";/>
			</td>
		</tr>
	</table>
</form>
<table border="0" cellspacing="0" cellpadding="3" align="center" class="Tabela">
	<?php	
		$sql = "SELECT
						'<img style=\"cursor:pointer\" onclick=\"excluirComparacao(\'' || cmf.cmfid || '\')\" src=\"/imagens/excluir.gif\" title=\"Excluir Comparação\" /> ' ||
						'<img style=\"cursor:pointer\" onclick=\"exibirComparacao(\'' || cmf.cmfid || '\')\" src=\"/imagens/consultar.gif\" title=\"Exibir Comparação\" />' as acao,
						to_char(cmf.cmfdtinclusao,'DD/MM/YYYY'),
						'<a style=\"cursor: pointer; color: blue;\" onclick=\"exibirComparacao(\'' || cmf.cmfid || '\')\" />' || arq1.arqdescricao ||'</a>' as desc,
						'<a style=\"cursor: pointer; color: blue;\" onclick=\"DownloadArquivo(' || arq1.arqid || ');\" />' || arq1.arqnome || '.'|| arq1.arqextensao ||'</a>' as foto1,
						'<a style=\"cursor: pointer; color: blue;\" onclick=\"DownloadArquivo(' || arq2.arqid || ');\" />' || arq2.arqnome || '.'|| arq2.arqextensao ||'</a>' as foto2,
						usu.usunome
					FROM
						obras.compararfoto cmf
					INNER JOIN
						public.arquivo arq1 ON arq1.arqid = cmf.arqid 
					INNER JOIN
						public.arquivo arq2 ON arq2.arqid = cmf.arqidpar
					INNER JOIN
						seguranca.usuario usu ON arq2.usucpf = usu.usucpf
					WHERE
						cmf.cmfstatus = 'A'
					and
						cmf.obrid = {$_SESSION["obra"]["obrid"]}";
		
		$cabecalho = array( "Ação", 
							"Data Inclusão",
							"Descrição dos Arquivos",
							"Foto 1",
							"Foto 2",
							"Responsável");
		$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '' );
	?>
</table>
<?php if($arrMsg['msg']): ?>
<script>alert('<?php echo $arrMsg['msg'] ?>')</script>
<?php endif; ?>