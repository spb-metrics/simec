<?php 
// carrega as funções gerais
include_once "config.inc";
include ("../../includes/funcoes.inc");
include ("../../includes/classes_simec.inc");
include "_constantes.php";
// abre conexão com o servidor de banco de dados
$db = new cls_banco();

if($_REQUEST['evento']){
	header('content-type: text/html; charset=ISO-8859-1');
	if($_REQUEST['eoiid'] && $_REQUEST['eoiid'] != ""){
		
		$sql = "update 
					projetos.enemobsinstituicao
				set
					eoidescricao = '".utf8_decode(trim($_REQUEST['eoidescricao']))."',
					eoidata = '".date("Y-m-d H:m:s")."',
					usucpf = '{$_SESSION['usucpf']}'
				where
					eoiid = ".trim($_REQUEST['eoiid'])."";
		
	}else{
		$sql = "insert into projetos.enemobsinstituicao
					(eoidescricao,usucpf,eoitipo,eniid,eoidata)
				values
					('".utf8_decode(trim($_REQUEST['eoidescricao']))."','".$_SESSION['usucpf']."','R','".trim($_REQUEST['eniid'])."','".date("Y-m-d H:m:s")."')";
	}
	$db->executar($sql);
	$db->commit($sql);
	exit;
}

if($_REQUEST['eoiid'] && $_REQUEST['eoiid'] != "" && !$_REQUEST['evento']){
	$sqlObs = "select
					eoidescricao
				from 
					projetos.enemobsinstituicao
				where
					eoiid = '{$_REQUEST['eoiid']}' ";
	$eoidescricao = $db->pegaUm($sqlObs);
}

?>
<html>
<head>
<meta http-equiv="Cache-Control" content="no-cache">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="Connection" content="Keep-Alive">
<meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
<title>Observação / Comentário</title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script language="JavaScript" src="../includes/prototype.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
</head>
<body style="margin:10px; padding:0; background-color: #fff; background-image: url(../imagens/fundo.gif); background-repeat: repeat-y;">
<form method="POST"  id="formulario" name="formulario">
	<input type="hidden" id="evento" name="evento" value="" />
	<input type="hidden" id="eoiid" name="eoiid" value="<?php echo $_REQUEST['eoiid'] ?>" />
	<input type="hidden" id="eniid" name="eniid" value="<?php echo $_REQUEST['eniid'] ?>" />
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td bgcolor="#cccccc" ><b>Comentário / Observação:</b></td>
		</tr>
		<tr>
			<td>
				<?php 
				$eoidescricao = trim($eoidescricao);
				echo campo_textarea( 'eoidescricao', 'S', 'S', '', 60, 3, 500); ?>
			</td>
		</tr>
		<tr>
			<td bgcolor="#cccccc" >
				<input type="button" class="botao" name="btassociar" value="Salvar" onclick="salvarObservacao();">
				<input type="button" class="botao" name="btassociar" value="Cancelar" onclick="window.close();">
			</td>
		</tr>
</table>
</form>
<div id="erro"></div>
</body>
<script>
	function salvarObservacao(){
		var erro = 0;
		if(!document.getElementById('eoidescricao').value){
			alert('Favor informar o Comentário / Observação');
			erro = 1;
			return false;
		}
		if(erro == 0){
			var eniid = document.getElementById('eniid').value;
			var eoiid = document.getElementById('eoiid').value;
			var eoidescricao = document.getElementById('eoidescricao').value;
			var myAjax = new Ajax.Request(
				window.location.href,
				{
						method: 'post',
						parameters: 'evento=salvaObservacao&eniid=' + eniid + '&eoiid=' + eoiid + '&eoidescricao=' + eoidescricao,
						asynchronous: false,
						onComplete: function(resp){
							//document.getElementById('erro').innerHTML = (resp.responseText);
							window.opener.location.href = 'projetos.php?modulo=principal/atividade_/cadastroEnem&acao=A&eniid=' + eniid;
							window.close();
						}
				});
		}
	}
</script>
</html>