<?php

// controle o cache do navegador
header( "Cache-Control: no-store, no-cache, must-revalidate" );
header( "Cache-Control: post-check=0, pre-check=0", false );
header( "Cache-control: private, no-cache" );   
header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
header( "Pragma: no-cache" );

require_once APPRAIZ . "includes/classes/entidades.class.inc";

if( $_REQUEST['opt'] == 'salvarRegistro')
{
	$entidade = new Entidades();
	$entidade->carregarEntidade($_REQUEST);
	$entidade->adicionarFuncoesEntidade($_REQUEST['funcoes']);
	$entidade->salvar();
	
	$sql = "SELECT count(1) FROM projetos.responsavelatividade WHERE entid = ".$entidade->getEntid()." AND atiid = ".$_REQUEST['atiid']." AND rpastatus = 'A' AND tpvid is null";
	$existeResponsavel = $db->pegaUm($sql);
	
	if( !$existeResponsavel )
	{
		$sql = "insert into 
				projetos.responsavelatividade
			(entid,atiid,rpastatus,rpadtinclusao)
				values
			(".$entidade->getEntid().", ".$_REQUEST['atiid'].", 'A', now());";
		
		$db->executar($sql);
		$db->commit();
	}
	
	echo '<script type="text/javascript">
			alert("Dados gravados com sucesso.");
			window.opener.location.href = window.opener.location.href;
			window.close();
		</script>';
	exit;
}

?>

<html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
    <title><?= $titulo ?></title>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/prototype.js"></script>
    <script type="text/javascript" src="../includes/entidades.js"></script>
    <script type="text/javascript" src="/includes/estouvivo.js"></script>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <script type="text/javascript">
      this._closeWindows = false;
    </script>
  </head>
  <body style="margin:10px; padding:0; background-color: #fff; background-image: url(../imagens/fundo.gif); background-repeat: repeat-y;">
    <div>
<?php

$entidade = new Entidades();

if( $_REQUEST['entid'] ) $entidade->carregarPorEntid($_REQUEST['entid']);
	
echo $entidade->formEntidade("enem.php?modulo=principal/atividade_enem/inserir_responsavel_processo&acao=A&opt=salvarRegistro&atiid=".$_REQUEST['atiid'],
							 array("funid" => FUNID_RESPONSAVEL_ENEM)
							);
?>
    </div>
    <script type="text/javascript">
	document.getElementById('frmEntidade').onsubmit  = function(e) {
	if (document.getElementById('entnumcpfcnpj').value == '') {
		alert('O CPF é obrigatório.');
		return false;
	}

	if (document.getElementById('entnome').value == '') {
		alert('O nome da entidade é obrigatório.');
		return false;
	}
	return true;
	}
    </script>
  </body>
</html>