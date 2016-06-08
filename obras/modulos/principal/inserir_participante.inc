<?php

// controle o cache do navegador
header( "Cache-Control: no-store, no-cache, must-revalidate" );
header( "Cache-Control: post-check=0, pre-check=0", false );
header( "Cache-control: private, no-cache" );   
header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
header( "Pragma: no-cache" );

require_once APPRAIZ . "includes/classes/entidades.class.inc";

if ($_REQUEST['opt'] == 'salvarRegistro') {
	
	$entidade = new Entidades();
	$entidade->carregarEntidade($_REQUEST);
	$entidade->adicionarFuncoesEntidade($_REQUEST['funcoes']);
	$entidade->salvar();
	
	// cria o objeto da classe termoDeAjuste
	$termodeajuste = new termoDeAjuste();
	$termodeajuste->CadastraParticipante( $entidade->getEntId() );
	
	exit;
	
}

?>
<html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
    <title><?php echo $GLOBALS['parametros_sistema_tela']['nome_e_orgao']; ?></title>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/prototype.js"></script>
    <script type="text/javascript" src="../includes/entidades.js"></script>
    <script type="text/javascript" src="/includes/estouvivo.js"></script>
    <script src="/obras/js/obras.js"></script>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <script type="text/javascript">
      this._closeWindows = false;
    </script>
  </head>
  <body style="margin:10px; padding:0; background-color: #fff; background-image: url(../imagens/fundo.gif); background-repeat: repeat-y;">
  	<div>
<?php

$entidade = new Entidades();
if($_REQUEST['entid'])
	$entidade->carregarPorEntid($_REQUEST['entid']);
	echo $entidade->formEntidade("obras.php?modulo=principal/inserir_participante&acao=A&opt=salvarRegistro",
								 array("funid" => 54, "entidassociado" => null),
								 array("enderecos"=>array(1))
								 );
								 
if( $_REQUEST['tipo'] == "consulta" ){
	echo "<script>document.getElementById('btngravar').disabled=true</script>";	
}
								 
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
