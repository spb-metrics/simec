<?

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

if($_POST['salvar']) {
	
	if($_POST['sbaid']) {
		
		$existe = $db->pegaUm("SELECT atiid FROM monitora.pi_subacaoatividade WHERE atiid='".$_POST['atiid']."' AND sbaid='".$_POST['sbaid']."'");
			
		if($existe) {
			$sql = "UPDATE monitora.pi_subacaoatividade SET sbaid='".$_POST['sbaid']."' WHERE atiid='".$_POST['atiid']."';";
		} else {
			$sql = "INSERT INTO monitora.pi_subacaoatividade(atiid, sbaid, sbaatividade)
		    		VALUES ('".$_POST['atiid']."', '".$_POST['sbaid']."', FALSE);";
		}
			
		$db->executar($sql);
			
		$sql = "SELECT atiid FROM projetos.atividade WHERE _atiprojeto='3' AND _atinumero ilike (SELECT _atinumero FROM projetos.atividade WHERE atiid=".$_REQUEST['atiid'].")||'.%'";
		$filhos = $db->carregar($sql);
			
		if($filhos[0]) {
			foreach($filhos as $f) {
				
				$existe = $db->pegaUm("SELECT atiid FROM monitora.pi_subacaoatividade WHERE atiid='".$f['atiid']."' AND sbaid='".$_POST['sbaid']."'");
				if($existe) {
					$sql = "UPDATE monitora.pi_subacaoatividade SET sbaid='".$_POST['sbaid']."' WHERE atiid='".$_POST['atiid']."';";
				} else {
					$sql = "INSERT INTO monitora.pi_subacaoatividade(sbaid, atiid, sbaatividade) VALUES ('".$_POST['sbaid']."', '".$f['atiid']."', false);";
				}
				$db->executar($sql);
			}
		}

		$db->executar("DELETE FROM monitora.pi_planointernoatividade WHERE atiid='".$_POST['atiid']."'");
		
		$planosinternos = $db->carregar("SELECT * FROM monitora.pi_planointerno WHERE sbaid='".$_POST['sbaid']."' AND plisituacao='C'");
				
		if($planosinternos[0]) {
					
			foreach($planosinternos as $pl) {
				 $db->executar("INSERT INTO monitora.pi_planointernoatividade(pliid, atiid) VALUES ('".$pl['pliid']."', '".$_POST['atiid']."')");
			}
					
		}
			
	}
		
	$db->commit();
	
	die("<script>
			window.opener.location.replace(window.opener.location);
			window.close();
		 </script>");
		
}

?>
<html>
<head>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />

<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<script language="JavaScript" src="../includes/prototype.js"></script>

</head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<?

monta_titulo('Vinculação da subação', '&nbsp;');

$sql = "SELECT distinct sba.sbaid, sba.sbacod, sba.sbatitulo, sub.atiid FROM monitora.pi_subacao sba 
		LEFT JOIN monitora.pi_subacaounidade sbu ON sbu.sbaid = sba.sbaid 
		LEFT JOIN monitora.pi_subacaoatividade sub ON sub.sbaid = sba.sbaid AND sub.atiid IN('".$_REQUEST['atiid']."')
		WHERE sbu.ungcod IS NOT NULL and pieid is not null AND sba.sbasituacao='A' AND sba.sbastatus='A' 
		ORDER BY sba.sbacod";

$subacoes = $db->carregar($sql);

//echo "<pre>";
//print_r($subacoes);

?>
<form method="post" name="formulario" id="formulario">
<input type="hidden" name="salvar" id="salvar" value="1">
<input type="hidden" name="atiid" value="<? echo $_REQUEST['atiid']; ?>">

<table class="tabela" bgcolor="#f5f5f5" border="0" cellSpacing="1" cellPadding="3" align="center">
<thead>
<tr>
	<td class="SubTituloCentro">&nbsp;</td>
	<td class="SubTituloCentro">Código</td>
	<td class="SubTituloCentro">Subação</td>
</tr>
</thead>
<tbody style="height:300px;overflow-y:scroll;overflow-x:hidden;">
<?
if($subacoes[0]) {
	foreach($subacoes as $subacao) {	
	?>
	<tr>
		<td><input type="radio" name="sbaid" value="<? echo $subacao['sbaid']; ?>" <? echo (($subacao['atiid'])?"checked":""); ?>></td>
		<td><? echo $subacao['sbacod']; ?></td>
		<td><? echo $subacao['sbatitulo']; ?></td>
	</tr>
	<?
	}
}
?>
</tbody>
<tfoot> 
<tr style="background-color: #cccccc">
	<td colspan="4">
	<input type="button" name="gravar" value="Salvar" onclick="enviar();" />
	<input type="button" name="close" value="Fechar" onclick="window.close();">
	</td>
</tr>
</tfoot>
</table>
</form>
<script type="text/javascript">

function enviar(){
	displayStaticMessage("<p align=center>Aguarde...</p>",false);
	document.formulario.submit();
}

	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	
	function displayMessage(url) {
		messageObj.setSource(url);
		messageObj.setCssClassMessageBox(false);
		messageObj.setSize(700,300);
		messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
		messageObj.display();
	}
	function displayStaticMessage(messageContent,cssClass) {
		messageObj.setHtmlContent(messageContent);
		messageObj.setSize(700,300);
		messageObj.setCssClassMessageBox(cssClass);
		messageObj.setSource(false);	// no html source since we want to use a static message here.
		messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
		messageObj.display();
	}
	function closeMessage() {
		messageObj.close();	
	}

</script>

</body>
</html>