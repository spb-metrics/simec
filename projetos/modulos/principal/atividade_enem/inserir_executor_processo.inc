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
	
	$sql = "SELECT count(1) FROM projetos.responsavelatividade WHERE entid = ".$entidade->getEntid()." AND atiid = ".$_REQUEST['atiid']." AND rpastatus = 'A' AND tpvid = 1";
	$existeResponsavel = $db->pegaUm($sql);
	
	if( !$existeResponsavel )
	{
		$sql = "insert into 
				projetos.responsavelatividade
			(entid,atiid,rpastatus,rpadtinclusao,tpvid)
				values
			(".$entidade->getEntid().", ".$_REQUEST['atiid'].", 'A', now(), 1);";
		
		$db->executar($sql);
		$db->commit();
	}
	
	if( $_REQUEST["funcoes"]["funid"] == FUNID_EXECUTOR_ENEM )
	{
		$sql = "SELECT count(1) FROM seguranca.usuario WHERE usucpf = '".$entidade->getEntNumCpfCnpj()."'";
		$existeUsuario = $db->pegaUm($sql);
		
		if( $existeUsuario == 0 )
		{
			$nome  = ( $entidade->getEntnome() ) ? $entidade->getEntnome() : 'NOVO USUARIO ENEM';
			$email = ( $entidade->getEntEmail() ) ? $entidade->getEntEmail() : 'novousuarioenem@mec.gov.br';
			
			$sql = "INSERT INTO 
						seguranca.usuario (usucpf,usunome,usuemail,ususenha,usuchaveativacao,usustatus)
					VALUES
						('".$entidade->getEntNumCpfCnpj()."', '".$nome."', '".$email."', '".md5_encrypt_senha('simecdti', '')."', 't', 'A')";
			$db->executar($sql);
			
			$sql = "INSERT INTO seguranca.usuario_sistema (sisid, usucpf) VALUES ( 24, '".$entidade->getEntNumCpfCnpj()."')";
			$db->executar($sql);
			
			$sql = "INSERT INTO seguranca.perfilusuario ( usucpf, pflcod ) VALUES ( '".$entidade->getEntNumCpfCnpj()."', 518 )";
			$db->executar($sql);
			
			$db->commit();
		}
		else
		{
			$sql = "SELECT count(1) FROM seguranca.usuario_sistema WHERE sisid = 24 AND usucpf = '".$entidade->getEntNumCpfCnpj()."'";
			$existeSistema = $db->pegaUm($sql);
			
			if( $existeSistema == 0 )
			{
				$sql = "INSERT INTO seguranca.usuario_sistema (sisid, usucpf) VALUES ( 24, '".$entidade->getEntNumCpfCnpj()."')";
				$db->executar($sql);
			}
			
			$sql = "SELECT count(1) FROM seguranca.perfilusuario WHERE pflcod = 518 AND usucpf = '".$entidade->getEntNumCpfCnpj()."'";
			$existePerfilSistema = $db->pegaUm($sql);
			
			if( $existePerfilSistema == 0 )
			{
				$sql = "INSERT INTO seguranca.perfilusuario ( usucpf, pflcod ) VALUES ( '".$entidade->getEntNumCpfCnpj()."', 518 )";
				$db->executar($sql);
			}
			
			if( $existeSistema == 0 || $existePerfilSistema == 0 ) $db->commit();
		}
	}
	
	echo "<script type=text/javascript>
			alert('Dados gravados com sucesso.');
			window.opener.document.getElementById('nome_executor').innerHTML='".$_REQUEST['entnome']."';
			window.opener.document.getElementById('entid_executor').value='".$entidade->getEntid()."';
			window.close();
		</script>";
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

if( $_REQUEST['entid_executor'] ) $entidade->carregarPorEntid($_REQUEST['entid_executor']);

$funcoes = $entidade->getEntFuncoes();

$comp = false;
if($funcoes[0]) {
	foreach($funcoes as $funcao) {
		if(!$_REQUEST['funcao_executor']) {
			if($funcao['funid'] == FUNID_EXECUTOR_ENEM) {
				$_REQUEST['funcao_executor'] = FUNID_EXECUTOR_ENEM;
			}
			if($funcao['funid'] == FUNID_EXECUTORJUR_ENEM) {
				$_REQUEST['funcao_executor'] = FUNID_EXECUTORJUR_ENEM;
			}
		}
		if($funcao['funid'] == $_REQUEST['funcao_executor']) {
			$comp = true;
		}
		
	}
}

if(!$comp) $entidade = new Entidades();

?>
    <script type="text/javascript">
      this._closeWindows = false;
      
     <? if($_REQUEST['funcao_executor'] == FUNID_EXECUTORJUR_ENEM): ?>
     
	document.observe("dom:loaded", function() {
		document.getElementById('tr_tpctgid').style.display = 'none';
		document.getElementById('tr_tpcid').style.display = 'none';
		document.getElementById('tr_tplid').style.display = 'none';
		document.getElementById('tr_tpsid').style.display = 'none';
		
		document.getElementById('tr_entungcod').style.display = 'none';
		document.getElementById('tr_entunicod').style.display = 'none';
		document.getElementById('tr_entnuninsest').style.display = 'none';
		document.getElementById('tr_entcodent').style.display = 'none';
		
	});
	
	<? endif; ?>
      
	function selecionaFuncao(funid) {
		window.location = window.location.href+'&funcao_executor='+funid;
	}
	
	function selecionaExecutor(entid) {
		window.location = window.location.href+'&funcao_executor=<? echo $_REQUEST['funcao_executor']; ?>&entidexecutor='+entid;
	}
	
    </script>

    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
	    <td class="SubtituloDireita" width="50%"><b>Selecione a função:</b></td>
	    <td>
	    <?
	    $funcao_executor = $_REQUEST['funcao_executor'];
		$sql = "SELECT funid as codigo, fundsc as descricao FROM entidade.funcao WHERE funid IN(".FUNID_EXECUTOR_ENEM.",".FUNID_EXECUTORJUR_ENEM.") ORDER BY fundsc";
		$db->monta_combo('funcao_executor', $sql, 'S', 'Selecione', 'selecionaFuncao', '', '', '300', 'S', 'funcao_executor','',$funcao_executor);
	    ?>
	    </td>
    </tr>
    </table>
<?

if($_REQUEST['funcao_executor']) {
	
?>
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
	    <td class="SubtituloDireita" width="50%"><b>Executores:</b></td>
	    <td>
	    <?
	    $executor_ = $_REQUEST['entidexecutor'];
	    $sql = "SELECT ent.entid as codigo, ent.entnome as descricao 
	    		FROM entidade.entidade ent 
	    		INNER JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
	    		WHERE fen.funid='".$_REQUEST['funcao_executor']."'";
		$db->monta_combo('executor_', $sql, 'S', 'Selecione', 'selecionaExecutor', '', '', '300', 'S', 'executor_','',$executor_);
	    ?>
	    </td>
    </tr>
    </table>
<?

if($_REQUEST['entidexecutor'] && $_REQUEST['funcao_executor']) {
	$entidade->carregarPorEntid($_REQUEST['entidexecutor']);
}	
echo $entidade->formEntidade("enem.php?modulo=principal/atividade_enem/inserir_executor_processo&acao=A&opt=salvarRegistro&atiid=".$_REQUEST['atiid'],
							 array("funid" => $_REQUEST['funcao_executor'])
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
<?
}
?>
  </body>
</html>