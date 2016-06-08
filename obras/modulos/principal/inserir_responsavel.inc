<?php
// controle o cache do navegador
header( "Cache-Control: no-store, no-cache, must-revalidate" );
header( "Cache-Control: post-check=0, pre-check=0", false );
header( "Cache-control: private, no-cache" );   
header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
header( "Pragma: no-cache" );

require_once APPRAIZ . "includes/classes/entidades.class.inc";

if( $_REQUEST["tipo"] == "supervisao" ){
	$tipo = "&tipo=supervisao";
}

define("ID_RESPONSAVELOBRAS",45);

if ($_REQUEST['opt'] == 'salvarRegistro') {
	
	$entidade = new Entidades();
	$entidade->carregarEntidade($_REQUEST);
	$entidade->adicionarFuncoesEntidade($_REQUEST['funcoes']);
	$entidade->salvar();
	
	if( $_REQUEST["tipo"] == "supervisao" ){
	
		$botoes = '<img src="/imagens/excluir.gif" style="cursor: pointer"  border="0" title="Excluir" onclick="obrExcluiRespEmpresa('.$entidade->getEntid().');"/>';
		echo '<script type="text/javascript">
					var tabela = window.opener.document.getElementById("respEmpresas");
					var tamanho = tabela.rows.length;
					var tr = tabela.insertRow(tamanho);	
					tr.id = "linha_'.$entidade->getEntid().'";
					var colAcao  = tr.insertCell(0);
					var colNome  = tr.insertCell(1);
					var colCpf   = tr.insertCell(2);
					var colTel   = tr.insertCell(3);
					var colCel   = tr.insertCell(4);
					var colEmail = tr.insertCell(5);
					colNome.id = "'.$entidade->getEntid().'";
					colAcao.style.textAlign = "center";
					colAcao.innerHTML  = \''.$botoes.'<input type="hidden" name="entidresp[]" id="entidresp_'.$entidade->getEntid().'" value="'.$entidade->getEntid().'"/>\';
					colNome.innerHTML  = "'.$entidade->getEntnome().'";
					colCpf.innerHTML   = "' . formatar_cpf($entidade->getEntNumCpfCnpj()) . '";
					colTel.innerHTML   = "(' . $entidade->getEntNumDddComercial() . ') ' . $entidade->getEntNumComercial() . '";
					colCel.innerHTML   = "(' . $entidade->getEntNumDddCelular() . ') ' . $entidade->getEntNumCelular() . '";
					colEmail.innerHTML = "' . $entidade->getEntEmail() . '";
					window.close();
				</script>';
		
	}else{
		
		// Verifica se é atualização de responsáveis
		if($_REQUEST["tr"]){
			echo '<script>
					var coluna = window.opener.document.getElementById("nomeEntidade_'.$entidade->getEntid().'");
					coluna.innerHTML = "'.$_REQUEST['entnome'].'";
					window.close();
				  </script>';
			
		} else {
			// Valida se tentar inserir um responsável já adiocionado
			echo '<script type="text/javascript" src="../includes/prototype.js"></script>
		          <script type="text/javascript">
		          	resp = window.opener.document.getElementById(\'linha_'.$entidade->getEntid().'\');
		        	if (resp){
		        		alert("Este responsável já foi adicionado!");
		        		window.close();
		        	}
		          </script>';
			
	        $dadoscontato = $db->carregar("SELECT tprcid, tprcdesc FROM obras.tiporespcontato ORDER BY tprcdesc");
	        if($dadoscontato[0]) {
		        $combo_responsabilidade = '<select style="width:170px" class="CampoEstilo" name="tprcid['.$entidade->getEntid().']" id="tprcid['.$entidade->getEntid().']">';
				foreach($dadoscontato as $tc) {
					$combo_responsabilidade .= '<option value="'.$tc['tprcid'].'">'.$tc['tprcdesc'].'</option>';
				}
				$combo_responsabilidade .= '</select>';
	        }
	        $botoes = '<img src="/imagens/alterar.gif" style="cursor: pointer;" border="0" title="Editar" onclick="atualizaResponsavel(' . $entidade->getEntid(). ');"/>&nbsp&nbsp&nbsp<img src="/imagens/excluir.gif" style="cursor: pointer"  border="0" title="Excluir" onclick="RemoveLinha('.$entidade->getEntid().');"/>';
			echo '<script type="text/javascript">
						var tabela = window.opener.document.getElementById("responsaveiscontato");
						var tamanho = tabela.rows.length;
						var tr = tabela.insertRow(tamanho);	
						tr.id = "linha_'.$entidade->getEntid().'";
						var colAcao = tr.insertCell(0);
						var colNome = tr.insertCell(1);
						var colResp = tr.insertCell(2);
						colNome.id = "nomeEntidade_'.$entidade->getEntid().'";
						colAcao.style.textAlign = "center";
						colAcao.innerHTML = \''.$botoes.'\';
						colNome.innerHTML = \''.$_REQUEST['entnome'].'\';
						colResp.innerHTML = \''.$combo_responsabilidade.'\'; 
						window.close();
					</script>';
		}
		
	}
	
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
if($_REQUEST['entid'])
	$entidade->carregarPorEntid($_REQUEST['entid']);
	echo $entidade->formEntidade("obras.php?modulo=principal/inserir_responsavel&acao=A&opt=salvarRegistro{$tipo}&tr=".$_REQUEST['tr'],
								 array("funid" => ID_RESPONSAVELOBRAS, "entidassociado" => null),
								 array("enderecos"=>array(1))
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
