<?
ini_set('post_max_size', '300M');
ini_set('upload_max_filesize', '300M');
ini_set("memory_limit", "2048M");
set_time_limit(0);



function visualizarRelatorioDownload($dados) {
	global $db;
	
	$agrup = array("pasta"   => array("campo" => "pasta","label" => "Pasta"),
				   "arquivo" => array("campo" => "arquivo","label" => "Arquivo"),
				   "usuario" => array("campo" => "usuario","label" => "Usuário"),
				   "data"    => array("campo" => "data","label" => "Data")
				  );
	
	ini_set("memory_limit", "1024M");
	include APPRAIZ. 'includes/classes/relatorio.class.inc';
	
	$sql = "SELECT 
			dps.dpsdsc as pasta,
			arq.arqnome||'.'||arq.arqextensao as arquivo,
			usu.usunome as usuario,
			to_char(dho.dhodatadownload,'dd/mm/YYYY HH24:MI') as data 
			FROM seguranca.downloadhistorico dho 
			INNER JOIN seguranca.downloadarquivos das ON das.dasid = dho.dasid 
			INNER JOIN seguranca.downloadpastas dps ON dps.dpsid = das.dpsid 
			INNER JOIN public.arquivo arq ON arq.arqid = das.arqid 
			INNER JOIN seguranca.usuario usu ON usu.usucpf = dho.usucpf 
			WHERE dps.sisid='".$_SESSION['sisid']."' AND dps.dpsstatus='A' AND das.dasstatus='A'";
											
	$agp = array("agrupador" 			=> array(),
				"agrupadoColuna" 		=> array("pasta",
										  		 "arquivo",
										  		 "usuario",
										  		 "data"),
	
 
				);
	
	if($_REQUEST['agrupador']) {
		foreach($_REQUEST['agrupador'] as $ag) {
			array_push($agp['agrupador'], $agrup[$ag]);
		}
	} else {
		foreach($agrup as $ag) {
			array_push($agp['agrupador'], $ag);
		}
	}
	
	$dados = $db->carregar( $sql );

	$rel = new montaRelatorio();
	$rel->setTolizadorLinha(false);
	$rel->setTotalizador(false);
	$rel->setMonstrarTolizadorNivel(true);
	$rel->setAgrupador($agp, $dados);
			
	echo "<script language=\"JavaScript\" src=\"../includes/funcoes.js\"></script>";
	echo "<script type=\"text/javascript\" language=\"javascript\" src=\"../includes/agrupador.js\"></script>";
	echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>";
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	echo "<script>";
	echo "function filtrarDownloads() {selectAllOptions(document.getElementById('agrupador')); document.getElementById('formrel').submit();}";
	echo "</script>";
	echo "<body>";
	echo $rel->getRelatorio();
	echo "<br/>";
	echo "<form id=formrel method=post>";
	echo "<table align=center class=listagem>";
	echo "<tr>";
	echo "<td>";
	echo "<select id=\"agrupador\" name=\"agrupador[]\" multiple=\"multiple\" size=\"4\" style=\"width:160px;height:70px;\" class=\"combo campoEstilo\">";
	echo "<option value=\"pasta\">Pasta</option>";
	echo "<option value=\"arquivo\">Arquivo</option>";
	echo "<option value=\"usuario\">Usuário</option>";
	echo "<option value=\"data\">Data</option>";
	echo "</select>";
	echo "</td>";
	echo "<td>";
	echo "<img src=\"../imagens/uarrow.gif\" style=\"padding: 5px\" onClick=\"subir( document.getElementById( 'agrupador' ) );\"/><br/>";
	echo "<img src=\"../imagens/darrow.gif\" style=\"padding: 5px\" onClick=\"descer( document.getElementById( 'agrupador' ) );\"/><br/>";
	echo "</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td colspan=2 align=center>";
	echo "<input type=button name=filtrar value=Agrupar onclick=filtrarDownloads();>";
	echo "</td>";
	echo "</tr>";
	echo "</table>";
	
	
	echo "</body>";
	
	
}


function removerArquivo($dados) {
	global $db;
	
	if($dados['dasid']) {
		$db->executar("UPDATE seguranca.downloadarquivos SET dasstatus='I' WHERE dasid='".$dados['dasid']."'");
		$db->commit();
	}
	
	$argus = explode("&",$_SERVER['REQUEST_URI']);
	echo "<script>
			alert('Arquivo removido com sucesso');
			window.location = '".$argus[0]."&".$argus[1]."';
		  </script>";
	
}

function removerPasta($dados) {
	global $db;
	
	
	$db->executar("UPDATE seguranca.downloadpastas SET dpsstatus='I' WHERE dpsid='".$dados['dpsid']."'");
	
	$sql = "SELECT dpsid FROM seguranca.downloadpastas WHERE dpsidpai='".$dados['dpsid']."'";
	$dpsids = $db->carregarColuna($sql);
	
	if($dpsids) {
		foreach($dpsids as $dpsid) {
			removerPasta(array("dpsid" => $dpsid));
		}
	}
	
	$sql = "SELECT dasid FROM seguranca.downloadarquivos WHERE dpsid='".$dados['dpsid']."'";
	$dasids = $db->carregarColuna($sql);
	
	if($dasids) {
		foreach($dasids as $dasid) {
			$db->executar("UPDATE seguranca.downloadarquivos SET dasstatus='I' WHERE dasid='".$dasid."'");
		}
	}
	
	if($dados['requisicao']) {
		$argus = explode("&",$_SERVER['REQUEST_URI']);
		$db->commit();
		echo "<script>
				alert('Pasta removida com sucesso');
				window.location = '".$argus[0]."&".$argus[1]."';
			  </script>";
	}
}

function downloadArquivo($dados) {
	global $db;
	
	$sql = "INSERT INTO seguranca.downloadhistorico(
            dasid, usucpf, dhodatadownload)
    		VALUES ((SELECT dasid FROM seguranca.downloadarquivos WHERE arqid='".$dados['arqid']."'), '".$_SESSION['usucpf']."', NOW());";
	
	$db->executar($sql);
	$db->commit();
	
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec("downloadarquivos", NULL ,"seguranca");
	$file->getDownloadArquivo($dados['arqid']);
}

function telaInserirArquivo($dados) {
	global $db;
	echo "<form name=forminserirarquivo method=post id=forminserirarquivo enctype=multipart/form-data>";
	echo "<input type=hidden name=requisicao value=inserirArquivo>";
	echo "<input type=hidden name=dpsid value=".$dados['dpsid'].">";
	echo "<table class=\"listagem\" width=\"100%\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
	echo "<tr>";
	echo "<td class=\"SubTituloCentro\" colspan=\"2\">Inserção de arquivos</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\">Arquivo:</td>";
	echo "<td><input type=file name=arquivo id=arquivo></td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloCentro\" colspan=\"2\"><input type=button name=inserirarquivo value=Salvar onclick=inserirArquivo();> <input type=button name=fechar value=Fechar onclick=closeMessage();></td>";
	echo "</tr>";
	echo "</table>";
	echo "</form>";
}

function telaInserirPasta($dados) {
	global $db;
	echo "<form name=forminserirpasta method='post' id=forminserirpasta>";
	echo "<input type=hidden name=requisicao value=inserirPasta>";
	echo "<input type=hidden name=dpsid value=".$dados['dpsid'].">";
	echo "<table class=\"listagem\" width=\"100%\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
	echo "<tr>";
	echo "<td class=\"SubTituloCentro\" colspan=\"2\">Inserção de pastas</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\">Pasta:</td>";
	echo "<td>".campo_texto('dpsdsc', "S", "S", "Descrição da pasta", 40, 100, "", "", '', '', 0, 'id=dpsdsc' )."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloCentro\" colspan=\"2\"><input type=button name=inserirpasta value=Salvar onclick=inserirPasta();> <input type=button name=fechar value=Fechar onclick=closeMessage();></td>";
	echo "</tr>";
	echo "</table>";
	echo "</form>";
}

function inserirPasta($dados) {
	global $db;
	
	$sql = "INSERT INTO seguranca.downloadpastas(
            sisid, usucpfcriacao, dpsdatacriacao, dpsdsc, dpsidpai, 
            dpsstatus)
    		VALUES ('".$_SESSION['sisid']."', '".$_SESSION['usucpf']."', NOW(), '".$dados['dpsdsc']."', ".(($dados['dpsid'])?"'".$dados['dpsid']."'":"NULL").", 'A');";
	
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Pasta inserida com sucesso');
			window.location = window.location;
		  </script>";
	
}

function inserirArquivo($dados) {
	global $db;
	
	if($_FILES['arquivo']['error'] == 0) {
		
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		
		$campos = array("dpsid"         => (($dados['dpsid'])?"'".$dados['dpsid']."'":"NULL"),
						"usucpfcriacao" => $_SESSION['usucpf'],
						"dasdatacriacao"        => "NOW()",
						"dasstatus"     => "'A'");
		
		$file = new FilesSimec("downloadarquivos", $campos ,"seguranca");
		$file->setUpload( NULL, "arquivo" );
		
		
	}
	
	echo "<script>
			alert('Arquivo inserido com sucesso');
			window.location = window.location;
		  </script>";
	
}



function gerarEstruturaArvore($dpsid = null, $adm = false) {
	global $db;
	
	if(!$dpsid) {
		$html .= "<img src=\"../includes/jquery-treeview/images/base.gif\" align=\"top\" /> Raiz ( ".$db->pegaUm("SELECT sisabrev FROM seguranca.sistema WHERE sisid='".$_SESSION['sisid']."'")." )";
		$html .= "<ul id=\"tree\" class=\"filetree treeview-famfamfam\">";
		if($adm) {
			$html .= "<li><img src=../includes/jquery-treeview/images/question.gif align=absmiddle border=0> <a style=cursor:pointer; onclick=exibirRelatorio();>Relatório de downloads</a></li>";
		}
	} else {
		$html .= "<ul>";
	}
	
	// carregando estrutura de pastas
	if($dpsid) {
		$sql = "SELECT * FROM seguranca.downloadpastas WHERE dpsidpai='".$dpsid."' AND sisid='".$_SESSION['sisid']."' AND dpsstatus='A'";
	} else {
		$sql = "SELECT * FROM seguranca.downloadpastas WHERE dpsidpai IS NULL AND sisid='".$_SESSION['sisid']."' AND dpsstatus='A'";
	}
	$pastas = $db->carregar($sql);

	if($pastas[0]) {
		foreach($pastas as $pasta) {
			$html .= "<li><span class=\"folder\">&nbsp;".$pasta['dpsdsc']."".(($adm)?" ( <img src=../imagens/excluir.gif align=absmiddle style=width:13px;height:14px;cursor:pointer; onclick=removerPasta('".$pasta['dpsid']."');> )":"")."</span>";
			$html .= gerarEstruturaArvore($pasta['dpsid'], $adm);
			$html .= "</li>";
		}
		
	}

	// carregando estrutura de arquivos
	if($dpsid) {
		$sql = "SELECT d.dasid, a.arqid, 
					   a.arqnome||'.'||a.arqextensao as nomearquivo,
					   to_char(a.arqdata,'dd/mm/YYYY')||' '||a.arqhora as datacriacao,
					   CASE WHEN a.arqtamanho BETWEEN 0 AND 1023 THEN a.arqtamanho||' bytes' 
					   	    WHEN a.arqtamanho BETWEEN 1024 AND 1048575 THEN ROUND(a.arqtamanho/1024::numeric,1)||' kilobytes'
					   	    WHEN a.arqtamanho BETWEEN 1048576 AND 1073741823 THEN ROUND(a.arqtamanho/1048576::numeric,1)||'M'
					   	    ELSE ROUND(a.arqtamanho/1073741824::numeric,1)||' Gigabytes' END as tamanho
				FROM seguranca.downloadarquivos d 
				INNER JOIN public.arquivo a ON a.arqid = d.arqid  
				WHERE dpsid='".$dpsid."' AND d.dasstatus='A'";
	} else {
		$sql = "SELECT d.dasid, a.arqid, a.arqnome||'.'||a.arqextensao as nomearquivo,
			   to_char(a.arqdata,'dd/mm/YYYY')||' '||a.arqhora as datacriacao,
			   CASE WHEN a.arqtamanho BETWEEN 0 AND 1023 THEN a.arqtamanho||' bytes' 
			   	    WHEN a.arqtamanho BETWEEN 1024 AND 1048575 THEN ROUND(a.arqtamanho/1024::numeric,1)||' kilobytes'
			   	    WHEN a.arqtamanho BETWEEN 1048576 AND 1073741823 THEN ROUND(a.arqtamanho/1048576::numeric,1)||'M'
			   	    ELSE ROUND(a.arqtamanho/1073741824::numeric,1)||' Gigabytes' END as tamanho
				FROM seguranca.downloadarquivos d
				INNER JOIN public.arquivo a ON a.arqid = d.arqid 
				WHERE dpsid IS NULL AND d.dasstatus='A'";
	}
	$arquivos = $db->carregar($sql);

	if($arquivos[0]) {
		foreach($arquivos as $arquivo) {
			$html .= "<li><span class=\"file\"><a style=\"cursor:pointer;\" onclick=\"window.location=window.location.href+'&requisicao=downloadArquivo&arqid=".$arquivo['arqid']."'\">&nbsp;".$arquivo['nomearquivo']."</a> - <font size=1>".$arquivo['tamanho']." ".$arquivo['datacriacao']."</font>".(($adm)?"( <img src=../imagens/excluir.gif align=absmiddle style=width:13px;height:14px;cursor:pointer; onclick=removerArquivo('".$arquivo['dasid']."');> )":"")."</span></li>";
		}
	} else {
		if(!$adm) {
			$html .= "<li>&nbsp;Não existem arquivos na pasta</li>";			
		}
	}
	
	if($adm) {
		$html .= "<li><span class=\"folderadd\" style=\"cursor:pointer;\" onclick=\"displayMessage('telaInserirPasta','".(($dpsid)?$dpsid:"")."')\">&nbsp;<b>Inserir pasta</b></span></li>";
		$html .= "<li><span class=\"fileadd\" style=\"cursor:pointer;\" onclick=\"displayMessage('telaInserirArquivo','".(($dpsid)?$dpsid:"")."')\">&nbsp;<b>Inserir arquivo</b></span></li>";
	}
	
	
	$html .= "</ul>";
	
	return $html;
	
	
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}


include APPRAIZ."includes/cabecalho.inc";

echo "<br />";

monta_titulo("Downloads","Estrutura de arquivos");

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<link rel="stylesheet" href="../includes/jquery-treeview/jquery.treeview.css" />
<script type="text/javascript" src="../includes/jquery-treeview/lib/jquery.cookie.js"></script>
<script type="text/javascript" src="../includes/jquery-treeview/jquery.treeview.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />

<script type="text/javascript">

jQuery(document).ready(function(){

	jQuery("#tree").treeview({
		collapsed: true,
		animated: "medium",
		control:"#sidetreecontrol",
		persist: "cookie"

	});
	
	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow


});

function exibirRelatorio() {
	var janela = window.open( window.location.href+'&requisicao=visualizarRelatorioDownload', 'relatoriodownload', 'width=900,height=645,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
}
	
function displayMessage(requisicao,pastaid) {
	messageObj.setSource(window.location.href+'&requisicao='+requisicao+'&dpsid='+pastaid);
	messageObj.setCssClassMessageBox(false);
	messageObj.setSize(690,400);
	messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
	messageObj.display();
}

function displayStaticMessage(messageContent,cssClass) {
	messageObj.setHtmlContent(messageContent);
	messageObj.setSize(600,150);
	messageObj.setCssClassMessageBox(cssClass);
	messageObj.setSource(false);	// no html source since we want to use a static message here.
	messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
	messageObj.display();
}

function closeMessage() {
	messageObj.close();	
}

function inserirPasta() {
	if(document.getElementById('dpsdsc').value == "") {
		alert('Nome da pasta é obrigatória');
		return false;
	}
	document.getElementById('forminserirpasta').submit();
	
}

function removerPasta(dpsid) {
	var conf = confirm('Deseja realemente remover a pasta?');
	
	if(conf) {
		window.location=window.location+'&requisicao=removerPasta&dpsid='+dpsid;
	}
}

function removerArquivo(dasid) {
	var conf = confirm('Deseja realemente remover o arquivo?');
	
	if(conf) {
		window.location=window.location+'&requisicao=removerArquivo&dasid='+dasid;
	}
}


function inserirArquivo() {
	if(document.getElementById('arquivo').value == "") {
		alert('Arquivo é obrigatório');
		return false;
	}
	document.getElementById('forminserirarquivo').submit();
	
}

</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
<td>
<?

echo gerarEstruturaArvore($dpsid = null, $adm);

?>
</td>
</tr>
</table>