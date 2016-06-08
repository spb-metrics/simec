<?php

$obras = new Obras();
require_once APPRAIZ . "www/obras/permissoes.php";
//$dobras = new DadosObra(null);

include_once APPRAIZ."includes/classes/fileSimec.class.inc";

//if( $_REQUEST["obrid"] ){
//	
//	include_once APPRAIZ . "www/obras/_permissoes_obras.php";
//	
//	session_unregister("obra");
//	$_SESSION["obra"]["obrid"] = $_REQUEST["obrid"];
//	
//}

// Realiza as rotinas da tela 
switch($_REQUEST['requisicao']) {
	case 'carregar':
		$vicid  = $_POST['vicid'];
		$sql = "SELECT * FROM obras.vistoriacaixa WHERE vicid = '{$vicid}'";
		extract($db->pegaLinha($sql));
	break;
	case "gravar":
	    $arquivo = $_FILES["arquivo"];
	    $_POST['vicexecutado'] = str_replace(',', '.', $_POST['vicexecutado']);
	    $arqid = "";
	    
	    if ( $_FILES["arquivo"] && $arquivo["name"] && $arquivo["type"] && $arquivo["size"] ){
				
			$file 		= new FilesSimec();
			$file->setUpload(null,null,false);
			$arqid = $file->getIdArquivo();
	    }    	

		if( empty($_POST['vicid'])){
			$sql = "INSERT INTO 
						obras.vistoriacaixa (obrid, usucpf, vicdata, vicexecutado, vicobs, arqid, entid)
					VALUES
						('".$_SESSION["obra"]["obrid"]."','".$_SESSION["usucpf"]."','".$_POST["vicdata"]."','".$_POST["vicexecutado"]."','".$_POST["vicobs"]."',".(empty($arqid)? 'null' : $arqid).",'".$_POST["entidvistoriador"]."')";

			$db->executar($sql);
		}else{
			if ( $arqid ){
				if (verificaExistenciaArquivo($_POST["arqid"])){
					$sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$_POST["arqid"];
					$db->executar($sql);
				}
			}
			
			$_POST['arqid'] = !empty($_POST['arqid']) ? $_POST['arqid']: 'null';
			$arqid = $arqid ? $arqid : $_POST['arqid'];
			
			$sql = "UPDATE
						obras.vistoriacaixa 
					SET 
						entid = '{$_POST["entidvistoriador"]}',
						vicdata = '{$_POST["vicdata"]}',
						vicexecutado = '{$_POST["vicexecutado"]}',
						vicobs = '{$_POST["vicobs"]}',
						arqid = {$arqid}
					WHERE
						vicid = '{$_POST['vicid']}'";
			$db->executar($sql);
		}
		$db->commit();
		die("<script>
				alert('Operação realizada com sucesso!');
				window.location = '?modulo=principal/vistoria_caixa&acao=A';
		 	</script>");
		exit;
	break;
	case "alterar":
		$obras->DownloadArquivo( $_REQUEST );
	break;
		case 'excluir':
			$sql = "UPDATE obras.vistoriacaixa SET vicstatus = 'I' where vicid=".$_POST["vicid"];

			$db->executar($sql);
			$db->commit();
		die("<script>
				alert('Operação realizada com sucesso!');
				window.location = '?modulo=principal/vistoria_caixa&acao=A';
		 	</script>");
		break;
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";

$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo = "Vistoria Caixa";
monta_titulo( $titulo_modulo, '' );

echo $obras->CabecalhoObras();

if(!$_SESSION["obra"]["obrid"]) {
	die("<script>
			alert('Variavel de obra não encontrada');
		 </script>");
}
//			window.location='obras.php?modulo=inicio&acao=A';

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>


<form method="post" id="formulario" name="formulario" enctype="multipart/form-data">
	<input type="hidden" name="requisicao" id="requisicao" value="gravar"/>
	<input type="hidden" name="vicid" id="vicid" value="<?= $vicid ?>"/>
	<input type="hidden" name="arqid" id="arqid" value="<?= $arqid ?>"/>
		<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
			<td class="SubTituloDireita">Vistoriador</td>
			<td>
				<?php
					if ( !empty($entid) ){
						$vistoriador = new Entidade($entid);
						$entnomevistoriador = $vistoriador->entnome;
					}
				?>
				<span id="entnomevistoriador"><?php echo $entnomevistoriador; ?></span>
			  	<input type="hidden" name="entidvistoriador" id="entidvistoriador" value="<? if( isset($_SESSION["obra"]["obrid"]) ) echo $entid; ?>">
				<input type="button" <?php if($somenteLeitura=="N") echo "disabled"; ?> name="pesquisar_entidade" value="Pesquisar" style="cursor: pointer;" onclick="inserirVistoriador(document.getElementById('entidvistoriador').value);"/>
				<?php print obrigatorio(); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data</td>
			<td>
				<?= campo_data2( 'vicdata', 'S', $somenteLeitura, '', 'S' ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">% Executado</td>
			<td>
				<?
					$vicexecutado = !empty($vicexecutado) ? str_replace('.', ',', $vicexecutado) : '' ;
				 	echo campo_texto( 'vicexecutado', 'S', $somenteLeitura, '', 11, 6, '###,##', '', 'left', '', 0, 'id="vicexecutado" onchange="validaExecutado();"');
				 ?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top;">Comentários:</td>
			<td><?= campo_textarea( 'vicobs', 'N',$somenteLeitura, '', '60', '2', '250','' , 0, '');  ?></td>
		</tr>		
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
			<td>
				<?php if($habilitado){ ?>
					<input type="file" name="arquivo"/>
				<?php } ?>
			</td>
		</tr>
		<tr style="background-color: #cccccc">
			<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
			<td>
				<input type="button"  <?php if($somenteLeitura=="N") echo "disabled"; ?>  name="botao" value="Salvar" onclick="validaForm();";/>
				<input type="button"  <?php if($somenteLeitura=="N") echo "disabled"; ?>  name="botao" value="Novo" onclick="novo();";/>
			</td>
		</tr>
	</table>
</form>
<table border="0" cellspacing="0" cellpadding="3" align="center" class="Tabela">
	<?	

			$acao = "'<center><img onclick=\"javascript:alterar('''|| vic.vicid ||''')\" src=\"/imagens/alterar.gif\" style=\"cursor:pointer;\" border=0 title=\"Alterar Vistoria\">&nbsp;
					  <img onclick=\"javascript:excluir('''|| vic.vicid ||''');\" src=\"/imagens/excluir.gif\" style=\"cursor:pointer;\" border=0 title=\"Excluir Vistoria\"></center>' as acao,";	

		$sql = "SELECT
						{$acao}
						ent.entnome,
						to_char(vic.vicdata,'DD/MM/YYYY'),
						vic.vicexecutado,
						vic.vicobs,
						'<a style=\"cursor: pointer; color: blue;\" onclick=\"DownloadArquivo(' || vic.arqid || ');\" />' || arq.arqnome ||'</a>' as arquivo				
					FROM
						obras.vistoriacaixa vic
					INNER JOIN
						seguranca.usuario usu ON vic.usucpf = usu.usucpf
					INNER JOIN
						entidade.entidade ent ON ent.entid = vic.entid
					LEFT JOIN
						arquivo arq ON arq.arqid = vic.arqid
					WHERE
						vic.vicstatus = 'A' AND	vic.obrid = '" . $_SESSION["obra"]["obrid"] . "'
					ORDER BY
						3 DESC";
						
		
		$cabecalho = array( "Ação", 
							"Nome",
							"Data",
							"% Executado",
							"Comentários",
							"Arquivo em Anexo");
		
		$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '' );
	?>
</table>
<script type="text/javascript">

	function validaExecutado(){
		var campo = document.getElementById('vicexecutado');
		var valor = campo.value;

		valor = valor.replace(",", ".");

		if ( Number(valor) > 100 ){
			alert('O valor do campo \'% Executado\' não pode ser maior que 100,00!');
			campo.value = '';
			campo.focus();
		}
	}
		
	function validaForm(){

		var msg = '';
		
		if ( document.getElementById('entidvistoriador').value == ''){
			msg += '\tVistoriador\n';
		} 
		
		if ( document.getElementById('vicdata').value == ''){
			msg += '\tData\n';
		}
		
		if ( document.getElementById('vicexecutado').value == ''){
			msg += '\t% Executado\n';
		}

		if (msg == ''){
			document.getElementById('requisicao').value = 'gravar';
			document.getElementById('formulario').submit();
		}else{
			alert('Os campos são obrigatórios:\n'+msg);	
			return false;
		}
	}

	function novo()
	{
		document.getElementById('requisicao').value = '';
		document.getElementById('formulario').submit();
	}
	
	function alterar(vicid){
		if ( vicid ){
			document.getElementById('requisicao').value = 'carregar';
			document.getElementById('vicid').value = vicid;
			document.getElementById('formulario').submit();
		}
	}

	function excluir(vicid){
		if (vicid){
			if (confirm('Deseja excluir este registro?')){
				document.getElementById('requisicao').value = 'excluir';
				document.getElementById('vicid').value = vicid;
				document.getElementById('formulario').submit();
			}
		}	
	}
</script>