<?php

function EnviarArquivo(){
	global $db;

	// obtém o arquivo
	$arquivo = $_FILES['arquivo'];

	if ( !is_uploaded_file( $arquivo['tmp_name'] ) ) {
		?>
			<script>
				location.href='obras.php?modulo=<?=$_REQUEST['modulo']?>&acao=<?= $_REQUEST['acao']?>';
			</script>
		<?
	}
	// BUG DO IE
	// O type do arquivo vem como image/pjpeg
	if($arquivo["type"] == 'image/pjpeg') {
		$arquivo["type"] = 'image/jpeg';
	}
	//Insere o registro do arquivo na tabela public.arquivo
	$sql = "INSERT INTO public.arquivo 	(arqnome,arqextensao,arqdescricao,arqtipo,arqtamanho,arqdata,arqhora,usucpf,sisid,arqstatus)
	values('".current(explode(".", $arquivo["name"]))."','".end(explode(".", $arquivo["name"]))."','".$_REQUEST["arqdescricao"]."','".$arquivo["type"]."','".$arquivo["size"]."','".date('d/m/Y')."','".date('H:i:s')."','".$_SESSION["usucpf"]."',". $_SESSION["sisid"] .",'A') RETURNING arqid;";
	$arqid = $db->pegaUm($sql);

	//Insere o registro na tabela obras.anexosprojeto
	$sql = "INSERT INTO obras.anexosprojeto (arqid,prjid,anpdsc,anpstatus,anpdtinclusao) VALUES ($arqid,{$_SESSION['prjid']},'{$_REQUEST['arqdescricao']}','A','now()')";
	$db->executar($sql);


	if(!is_dir('../../arquivos/obras/'.floor($arqid/1000))) {
		mkdir(APPRAIZ.'/arquivos/obras/'.floor($arqid/1000), 0777);
	}
	$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arqid/1000) .'/'. $arqid;
	switch($arquivo["type"]) {
		case 'image/jpeg':
			ini_set("memory_limit", "128M");
			list($width, $height) = getimagesize($arquivo['tmp_name']);
			$original_x = $width;
			$original_y = $height;
			// se a largura for maior que altura
			if($original_x > $original_y) {
  	 			$porcentagem = (100 * 640) / $original_x;      
			}else {
   				$porcentagem = (100 * 480) / $original_y;  
			}
			$tamanho_x = $original_x * ($porcentagem / 100);
			$tamanho_y = $original_y * ($porcentagem / 100);
			$image_p = imagecreatetruecolor($tamanho_x, $tamanho_y);
			$image   = imagecreatefromjpeg($arquivo['tmp_name']);
			imagecopyresampled($image_p, $image, 0, 0, 0, 0, $tamanho_x, $tamanho_y, $width, $height);
			imagejpeg($image_p, $caminho, 100);
			//Clean-up memory
			ImageDestroy($image_p);
			//Clean-up memory
			ImageDestroy($image);
			break;
		default:
			if ( !move_uploaded_file( $arquivo['tmp_name'], $caminho ) ) {
				$db->rollback();
				echo "<script>alert(\"Problemas no envio do arquivo.\");</script>";
				exit;
			}
	}

	$db->commit();

}

function DownloadArquivo($arqid){
		global $db;
		
		$sql = "SELECT * FROM public.arquivo WHERE arqid = ".$arqid;
		$arquivo = $db->carregar($sql);
        $caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arquivo[0]['arqid']/1000) .'/'.$arquivo[0]['arqid'];
		if ( !is_file( $caminho ) ) {
            $_SESSION['MSG_AVISO'][] = "Arquivo não encontrado.";
        }
        $filename = str_replace(" ", "_", $arquivo[0]['arqnome'].'.'.$arquivo[0]['arqextensao']);
        header( 'Content-type: '. $arquivo[0]['arqtipo'] );
        header( 'Content-Disposition: attachment; filename='.$filename);
        readfile( $caminho );
        exit();
}

function DeletarDocumento($arqid){
	global $db;

	$sql = "UPDATE obras.anexosprojeto SET anpstatus = 'I' where arqid = $arqid";
	$db->executar($sql);
	$sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid = $arqid";
	$db->executar($sql);
	$db->commit();

}

if($_REQUEST['requisicao'] == 'inserirarquivo' && $_REQUEST['tpaid'] && $_REQUEST['arqdescricao']){
	EnviarArquivo();
	print "<script>alert('Operação Realizada com Sucesso!');</script>";
}

if($_REQUEST['opt'] == 'Excluir' && $_REQUEST['arqid']){
	DeletarDocumento($_REQUEST['arqid']);
	print "<script>alert('Operação Realizada com Sucesso!');window.location = 'obras.php?modulo=principal/documentos_projeto&acao=A';</script>";
}

if(!$_SESSION['prjid']){
	header( "Location: obras.php?modulo=principal/biblioteca_projeto&acao=A" );
	exit();
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";

$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo = "Documentos do Projeto";
monta_titulo( $titulo_modulo, '' );

?>
<html>
	<head>
		<title>Anexar Documentos</title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
		<form method="post" name="anexo" enctype="multipart/form-data" >
			<input type="hidden" name="requisicao" value="inserirarquivo"/>
			<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
					<td>
							<input type="file" name="arquivo"/>
							<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
					</td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Tipo:</td>
					<td><?php
									
					$sql = "
						SELECT tpaid AS codigo, tpadesc AS descricao 
							FROM obras.tipoarquivo
					";
					$db->monta_combo('tpaid', $sql, 'S', "Selecione...", '', '', '', '100', 'S');
				?></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
					<td><?= campo_textarea( 'arqdescricao', 'S', 'S', '', 60, 2, 250 ); ?></td>
				</tr>
				<tr style="background-color: #cccccc">
					<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
					<td>
							<input type="button" name="botao" value="Salvar" onclick="enviar();" />
					</td>
				</tr>
			</table>
		</form>
		<table border="0" cellspacing="0" cellpadding="3" align="center" class="Tabela">
			<?
			if($_SESSION['prjid']){
				$sql = "SELECT
								'<center><a href=\"#\" onclick=\"javascript:DeletarDocumento(' || arq.arqid || ');\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a></center>' as acao,						
								to_char(anp.anpdtinclusao,'DD/MM/YYYY'),
								arq.arqtipo,
								'<a style=\"cursor: pointer; color: blue;\" onclick=\"DownloadArquivo(' || arq.arqid || ');\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a>',
								arq.arqtamanho || ' kbs' as tamanho ,
								arq.arqdescricao,								
								u.usunome
							FROM
								obras.anexosprojeto AS anp
								LEFT JOIN public.arquivo AS arq ON arq.arqid = anp.arqid 
								LEFT JOIN seguranca.usuario AS u ON u.usucpf = arq.usucpf
							WHERE
								anp.anpstatus = 'A' AND anp.prjid = {$_SESSION['prjid']}";
			}else{
				$sql = "SELECT
								'<center><a href=\"#\" onclick=\"javascript:DeletarDocumento(' || arq.arqid || ');\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a></center>' as acao,						
								to_char(anp.anpdtinclusao,'DD/MM/YYYY'),
								arq.arqtipo,
								'<a style=\"cursor: pointer; color: blue;\" onclick=\"DownloadArquivo(' || arq.arqid || ');\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a>',
								arq.arqtamanho || ' kbs' as tamanho ,
								arq.arqdescricao,								
								u.usunome
							FROM
								obras.anexosprojeto AS anp
								LEFT JOIN public.arquivo AS arq ON arq.arqid = anp.arqid 
								LEFT JOIN seguranca.usuario AS u ON u.usucpf = arq.usucpf
							WHERE
								anp.anpstatus = 'A'";
			}
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
	</body>
<script>
function validaForm(){
			if (!document.anexo.arquivo.value ) {
				alert( 'Selecione o Arquivo!' );
				document.anexo.arquivo.focus();
				return false;
			}
			if (!document.anexo.tpaid.value ) {
				alert( 'Selecione o Tipo do Arquivo!' );
				document.anexo.tpaid.focus();
				return false;
			}
			if (!document.anexo.arqdescricao.value ) {
				alert( 'Informe a Descrição do Arquivo!' );
				document.anexo.arqdescricao.focus();
				return false;
			}
			else{
				return true;
			}
		}
		function enviar(){
			if(validaForm() == true){
			anexo.submit();
			}
		}
		function DeletarDocumento(arqid){
			if(arqid){
				if(confirm('Deseja excluir o arquivo?')){
					window.location = 'obras.php?modulo=principal/documentos_projeto&acao=A&opt=Excluir&arqid=' + arqid;
				}
			}
		}
</script>
</html>