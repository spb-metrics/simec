<?php
//echo "<pre>";
//print_r($_REQUEST);


switch($_REQUEST["requisicao"]){
	case "deletarcomposicaotrajetoria":
		
		$sql = "DELETE FROM obras.composicaotrajetoria WHERE ctjid='".$_GET['ctjid']."';";
		
		$db->executar($sql);
		$db->commit();
		
		$valor = $db->pegaUm("SELECT SUM(ctjvalor) FROM obras.composicaotrajetoria WHERE trjid='".$_REQUEST['trjid']."'");
		
		die("<script>
				".(($valor)?"window.opener.document.getElementById('imgtrj_".$_REQUEST['trjid']."').value='".number_format($valor,2,",",".")."';":"window.opener.document.getElementById('imgtrj_".$_REQUEST['trjid']."').value='';")."
				alert('Detalhamento removido com sucesso');
				window.location = 'obras.php?modulo=principal/supervisao/detalharTrajAlternativa&acao=A&trjid=".$_REQUEST['trjid']."';
			 </script>");
	
	
	case "atualizarcomposicaotrajetoria":
		
		$sql = "UPDATE obras.composicaotrajetoria
				SET ctjdsc='".$_POST['ctjdsc']."', ctjvalor='".str_replace(array(".",","),array("","."),$_POST['ctjvalor'])."'
				WHERE ctjid='".$_POST['ctjid']."';";
		
		$db->executar($sql);
		$db->commit();
		
		$valor = $db->pegaUm("SELECT SUM(ctjvalor) FROM obras.composicaotrajetoria WHERE trjid='".$_REQUEST['trjid']."'");
		
		die("<script>
				".(($valor)?"window.opener.document.getElementById('imgtrj_".$_REQUEST['trjid']."').value='".number_format($valor,2,",",".")."';":"")."
				alert('Detalhamento atualizado com sucesso');
				window.location = 'obras.php?modulo=principal/supervisao/detalharTrajAlternativa&acao=A&trjid=".$_REQUEST['trjid']."';
			 </script>");
	
	
	case "inserircomposicaotrajetoria":
		
		$sql = "INSERT INTO obras.composicaotrajetoria(
            	trjid, ctjdsc, ctjvalor)
    			VALUES ('".$_POST['trjid']."', '".$_POST['ctjdsc']."', '".str_replace(array(".",","),array("","."),$_POST['ctjvalor'])."');";
		
		$db->executar($sql);
		$db->commit();
		
		$valor = $db->pegaUm("SELECT SUM(ctjvalor) FROM obras.composicaotrajetoria WHERE trjid='".$_REQUEST['trjid']."'");
		
		die("<script>
				".(($valor)?"window.opener.document.getElementById('imgtrj_".$_REQUEST['trjid']."').value='".number_format($valor,2,",",".")."';":"")."
				alert('Detalhamento inserido com sucesso');
				window.location = 'obras.php?modulo=principal/supervisao/detalharTrajAlternativa&acao=A&trjid=".$_REQUEST['trjid']."';
			 </script>");

}




?>

<html>
	<head>
		<title><?php echo $GLOBALS['parametros_sistema_tela']['nome_e_orgao']; ?></title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script src="/includes/prototype.js"></script>
		<script src="../includes/calendario.js"></script>
		<script src="../obras/js/obras.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
		<?
		if($_REQUEST['ctjid']) {
			$requisicao = "atualizarcomposicaotrajetoria";
			$sql = "SELECT * FROM obras.composicaotrajetoria WHERE ctjid='".$_REQUEST['ctjid']."'";
			$composicaotrajetoria = $db->pegaLinha($sql);
			$ctjdsc = $composicaotrajetoria['ctjdsc'];
			$ctjvalor = number_format($composicaotrajetoria['ctjvalor'],2,",",".");
		} else {
			$requisicao = "inserircomposicaotrajetoria";
		}
		?>
		<script>
		function deletarcomposicaotrajetoria(ctjid) {
			var conf = confirm("Deseja realmente excluir o detalhamento?");
			if(conf) {
				window.location='obras.php?modulo=principal/supervisao/detalharTrajAlternativa&acao=A&requisicao=deletarcomposicaotrajetoria&ctjid='+ctjid+'&trjid=<? echo $_REQUEST['trjid']; ?>';
			}
		}
		
		function validardetalhamento(frm) {
			if(frm.ctjdsc.value == "") {
				alert("Descrição é obrigatória");
				return false;
			}
			if(frm.ctjvalor.value == "") {
				alert("Valor é obrigatório");
				return false;
			}
			
			return true;

		}
		</script>
		<form method="post" action="obras.php?modulo=principal/supervisao/detalharTrajAlternativa&acao=A" onsubmit="return validardetalhamento(this);">
		<input type="hidden" name="trjid" value="<? echo $_REQUEST['trjid']; ?>">
		<input type="hidden" name="ctjid" value="<? echo $_REQUEST['ctjid']; ?>">
		<input type="hidden" name="requisicao" value="<? echo $requisicao; ?>">
		<table class="tabela" bgcolor="#ffffff" cellSpacing="1" cellPadding=3 align="center">
			<tr>
				<td class="SubTituloCentro" colspan="4">Detalhamento da trajetória alternativa</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Descrição:</td>
				<td><? echo campo_texto('ctjdsc', "S", "S", "Descrição", 30, 100, "", "", '', '', 0, '' ); ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Valor:</td>
				<td><? echo campo_texto('ctjvalor', "S", "S", "Valor", 20, 50, "###.###.###,##", "", '', '', 0, '' ); ?></td>
			</tr>
			<tr bgcolor="#D0D0D0">
				<td colspan="4">
					<input type="submit" value="Salvar"/>
					<input type="button" value="Novo" onclick="window.location='obras.php?modulo=principal/supervisao/detalharTrajAlternativa&acao=A&trjid=<? echo $_REQUEST['trjid']; ?>'" />
					<input type="button" value="Fechar" style="cursor: pointer;" onclick="self.close();"/>
				</td>
			</tr>
		</table>
		</form>
		<?
		$sql = "SELECT '<center><img src=../imagens/alterar.gif style=cursor:pointer; onclick=window.location=\'obras.php?modulo=principal/supervisao/detalharTrajAlternativa&acao=A&trjid='|| trjid ||'&ctjid='|| ctjid ||'\';> <img src=../imagens/excluir.gif style=cursor:pointer; onclick=deletarcomposicaotrajetoria('|| ctjid ||');></center>' as acao, ctjdsc, ctjvalor FROM obras.composicaotrajetoria WHERE trjid='".$_REQUEST['trjid']."'";
		$cabecalho = array("&nbsp;", "Descrição", "Valor"); 
		$db->monta_lista($sql,$cabecalho,50,5,'S','95%',$par2);
		?>
	</body>
</html>
