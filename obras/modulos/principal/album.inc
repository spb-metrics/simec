<?php

if( !$_SESSION["obra"]['obrid'] ){
	header( "location:obras.php?modulo=inicio&acao=A" );
}

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
		$dir = 'album&acao=A';
		$obras->EnviarArquivo($_FILES, $_POST, $dir);
		exit;
	break;
	case "excluir":
		$obras->DeletarDocumento( $_REQUEST, 'principal/album' );
		exit;
	break;
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
//include APPRAIZ . 'www/obras/_funcoes.php'; 

echo "<br>";
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo = "Galeria de Fotos";
monta_titulo( $titulo_modulo, 'Clique na imagem para vê-la em tamanho original' );

echo $obras->CabecalhoObras();

?>
<script src="/obras/js/documentos.js" ></script>
<form method="post" name="anexo" enctype="multipart/form-data" onsubmit="return ValidarFormulario(this);" action="<?php echo $caminho_atual;?>acao=A">
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
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
			<td><?= campo_textarea( 'arqdescricao', 'S', $somenteLeitura, '', 60, 2, 250 ); ?></td>
		</tr>
		<tr style="background-color: #cccccc">
			<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
			<td>
				<input type="hidden" name="tpaid" id="tpaid" value="21"/>
				<?php if($habilitado){ ?>
					<input type="submit" name="botao" value="Salvar"/>
				<?php } ?>
			</td>
		</tr>
	</table>
	<br/>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr><td><div>
		<?
		$sql = "SELECT 
					arqnome, arq.arqid, 
					arq.arqextensao, arq.arqtipo, 
					arq.arqdescricao,
				 	to_char(arq.arqdata, 'DD/MM/YYYY') as data,
				 	'<img style=\"cursor:pointer; position:relative; z-index:10; top:-87px; left:-9px; float:right;\" src=\"../obras/plugins/imgs/delete.png\" border=0 title=\"Excluir\" onclick=\"javascript:ExcluirDocumento(\'" . $caminho_atual . "acao=A&requisicao=excluir" . "\',' || arq.arqid || ',' || oar.aqoid || ');\">' as acao
				FROM 
					public.arquivo arq
				INNER JOIN 
					obras.arquivosobra oar ON arq.arqid = oar.arqid
				INNER JOIN 
					obras.obrainfraestrutura obr ON obr.obrid = oar.obrid 
				INNER JOIN 
					seguranca.usuario seg ON seg.usucpf = oar.usucpf 
				WHERE 
					obr.obrid = {$_SESSION["obra"]["obrid"]} AND
					aqostatus = 'A' AND
					(arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png') 
				ORDER BY 
					arq.arqid
				LIMIT 16 OFFSET ".($_REQUEST['pagina']*16);
		$fotos = ($db->carregar($sql));
		$_SESSION['downloadfiles']['pasta'] = array("origem" => "obras","destino" => "obras");
		
		if( $fotos ){
			$_SESSION['imgparams'] = array("filtro" => "cnt.obrid=".$_SESSION["obra"]["obrid"]." AND 
														aqostatus = 'A'", 
										   "tabela" => "obras.arquivosobra");
			
			for( $k=0; $k < count($fotos); $k++ ){
				echo "<div style=\"float:left; width:90px; height:100px; text-align:center; margin:3px;\" >
						<img border='1px' id='".$fotos[$k]["arqid"]."' src='../slideshow/slideshow/verimagem.php?newwidth=64&newheight=48&arqid=".$fotos[$k]["arqid"]."' hspace='10' vspace='3' style='position:relative; z-index:5; float:left; width:70px; height:70px;' onmouseover=\"return escape( '". $fotos[$k]["arqdescricao"] ."' );\" onclick='javascript:window.open(\"../slideshow/slideshow/index.php?pagina=". $_REQUEST['pagina'] ."&arqid=\"+this.id+\"\",\"imagem\",\"width=850,height=600,resizable=yes\")'/><br>
						" . $fotos[$k]["data"] . " <br/>
						" . $fotos[$k]["acao"] . "
					  </div>";
				
			}
			
		}else {
			echo "Não existem fotos cadastradas";
		}
		?>
		
		</div>
		</td></tr>
		<tr>
			<td align="center">
				<?
					if(!$_REQUEST['pagina']) $_REQUEST['pagina'] = 0;
					$sql = "SELECT COUNT(arq.arqid) AS totalregistros FROM obras.arquivosobra AS cnt 
							LEFT JOIN public.arquivo AS arq ON arq.arqid = cnt.arqid 
							WHERE obrid = {$_SESSION["obra"]["obrid"]} AND 
							aqostatus = 'A' AND 
							(arqtipo = 'image/jpeg' OR
							 arqtipo = 'image/gif' OR
							 arqtipo = 'image/png')";
					$paginacao = current($db->carregar($sql));
					if($paginacao) {
						for($i = 0; $i < ceil(current($paginacao)/16); $i++ ) {
							$page[] = "<a href=?modulo=principal/album&acao=A&pagina=". $i .">".(($i==$_REQUEST['pagina'])?"<b>".($i+1)."</b>":($i+1))."</a>";
						}
						if(count($page) > 1) {
							echo implode(" | ", $page);
						}
					}
				?>
			</td>
		</tr>
	</table>	
</form>
		