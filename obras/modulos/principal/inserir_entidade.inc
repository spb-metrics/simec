<?php
header("Cache-Control: no-cache, must-revalidate");
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");

require_once APPRAIZ . "includes/classes/entidades.class.inc";

if ($_REQUEST['opt'] == 'salvarRegistro') {
	global $db;
	$entidade = new Entidades();
	$entidade->carregarEntidade($_REQUEST);
	$entidade->adicionarFuncoesEntidade($_REQUEST['funcoes']);
	$entidade->salvar();
	/*
	 * Validando os dados
	 */
	switch($_REQUEST['orgid']) {
		case ORGAO_REHUF:
        	$funid = $db->pegaUm("SELECT ef.funid FROM entidade.entidade e 
								  INNER JOIN entidade.funcaoentidade ef ON e.entid = ef.entid 
								  WHERE e.entid = '".$entidade->getEntid()."' AND ef.funid='".ID_HOSPITAL."'");
	
			if(!$funid) {
				echo '<script type="text/javascript">
						alert("A entidade '.$_REQUEST['nome'].' não possui hospitais universitarios!");
						window.close();
					  </script>';
				exit;
			}
			break;
		case ORGAO_SESU:
        	$funid = $db->pegaUm("SELECT funid FROM entidade.entidade e 
        						  INNER JOIN entidade.funcaoentidade ef ON e.entid = ef.entid 
        						  WHERE	e.entid = '".$entidade->getEntid()."' AND ef.funid = '".ID_UNIVERSIDADE."'");
        	if(!$funid) {
        		echo '<script type="text/javascript">
        				alert("A entidade '.$_REQUEST['nome'].' não é uma universidade!");
        				window.close();
	        		  </script>';
        		exit;
        	}
			break;
		case ORGAO_SETEC:
        	$funid = $db->pegaUm("SELECT funid FROM entidade.entidade e 
        						  INNER JOIN entidade.funcaoentidade ef ON e.entid = ef.entid 
        						  WHERE	e.entid = '".$entidade->getEntid()."' AND ef.funid IN('".ID_SSP."', '".ID_ESCOLAS_TECNICAS."','".ID_ESCOLAS_AGROTECNICAS."','".ID_UNED."','".ID_SUPERVISIONADA."','".ID_REITORIA."')");

        	if(!$funid) {
        		echo '<script type="text/javascript">
        				alert("A entidade '.$_REQUEST['nome'].' não é uma escola técnica!");
        				window.close();
	        		  </script>';
        		exit;
        	}
			break;
	}
	switch($funid) {
		case ID_UNIVERSIDADE:
			$sql = "SELECT e.entid as codigo, entnome as descricao 
					FROM entidade.entidade e 
					INNER JOIN entidade.funcaoentidade ef ON ef.entid = e.entid 
					INNER JOIN entidade.funentassoc ea ON ea.fueid = ef.fueid
					WHERE ea.entid = {$entidade->getEntid()} AND funid = '".ID_CAMPUS."' OR ea.entid = {$entidade->getEntid()} AND funid = ".ID_REITORIA." AND e.entstatus = 'A' ORDER BY e.entnome ASC";
			break;
		case ID_ESCOLAS_TECNICAS:
		case ID_ESCOLAS_AGROTECNICAS:
			$sql = "SELECT e.entid as codigo, entnome as descricao 
					FROM entidade.entidade e 
					INNER JOIN entidade.funcaoentidade ef ON ef.entid = e.entid 
					INNER JOIN entidade.funentassoc ea ON ea.fueid = ef.fueid
					WHERE ea.entid = {$entidade->getEntid()} AND funid = '".ID_UNED."' OR ea.entid = {$entidade->getEntid()} AND funid = ".ID_REITORIA." AND e.entstatus = 'A' ORDER BY e.entnome ASC";
			break;
	}
	if($sql) {
		$cm = $db->carregar($sql);
		if($cm[0]) {
			$combo = '<select class="CampoEstilo" id="entidcampus" name="entidcampus">';
			foreach($cm as $en) {
				$combo .='<option value="' . $en['codigo'] . '">' . $en['descricao'] . '</option>';
			}
			$combo .= '</select>';
		    echo '<script type="text/javascript">
		        	if (document.selection){
		        		window.opener.document.getElementById("campus").style.display = "block";
		        	}else{
			        	window.opener.document.getElementById("campus").style.display = "table-row";
			        }
				    window.opener.document.getElementById("mostracampus").innerHTML = \'' . $combo . '\';	
		          </script>';
		}
	}
	/*
	 * FIM
	 * Validando os dados
	 */
    echo '<script type="text/javascript">
	        window.opener.document.getElementById("entnome").innerHTML = \''.$_REQUEST['entnome'].'\';
	        window.opener.document.getElementById("entid").value       = \''.$entidade->getEntid().'\';
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
if(INTEGRA_RECEITA) :
	switch($_REQUEST['orgid']) {
		case ORGAO_ADM:
		case ORGAO_REHUF:
		case ORGAO_SESU:
			$javascript = "<script>
							document.getElementById('frmEntidade').onsubmit  = function(e) {
								if (document.getElementById('entnome').value == '') {
									alert('O nome da entidade é obrigatório.');
									return false;
								}
								return true;
							}
			
							document.getElementById('tr_entnumcpfcnpj').style.display = 'none';
							document.getElementById('tr_entcodent').style.display = 'none';
							document.getElementById('tr_entnuninsest').style.display = 'none';
							document.getElementById('tr_entungcod').style.display = 'none';
							document.getElementById('tr_tpctgid').style.display = 'none';
							/*
							 * DESABILITANDO A SIGLA DA ENTIDADE
							 */
							document.getElementById('entnome').readOnly = false;
							document.getElementById('entnome').className = 'normal';
							document.getElementById('entrazaosocial').readOnly = false;
							document.getElementById('entrazaosocial').className = 'normal';
							document.getElementById('entemail').readOnly = false;
							document.getElementById('entemail').className = 'normal';
							document.getElementById('entsig').onfocus = \"\";
							document.getElementById('entsig').onmouseout = \"\";
							document.getElementById('entsig').onblur = \"\";
							document.getElementById('entsig').onkeyup = \"\";
						   </script>";
			break;
		case ORGAO_SETEC:
	
	$javascript = "<script>
							document.getElementById('frmEntidade').onsubmit  = function(e) {
								if (document.getElementById('entnome').value == '') {
									alert('O nome da entidade é obrigatório.');
									return false;
								}
								return true;
							}
			
							document.getElementById('tr_entnumcpfcnpj').style.display = 'none';
							document.getElementById('tr_entunicod').style.display = 'none';
							document.getElementById('tr_entnuninsest').style.display = 'none';
							document.getElementById('tr_entungcod').style.display = 'none';
							document.getElementById('tr_tpctgid').style.display = 'none';
							/*
							 * DESABILITANDO A SIGLA DA ENTIDADE
							 */
							document.getElementById('entsig').readOnly = true;
							document.getElementById('entsig').className = 'disabled';
							document.getElementById('entsig').onfocus = \"\";
							document.getElementById('entsig').onmouseout = \"\";
							document.getElementById('entsig').onblur = \"\";
							document.getElementById('entsig').onkeyup = \"\";
							
							document.getElementById('tr_entnumcpfcnpj').style.display = '';
							break;
	
						   </script>";
					  
		break;
	
		case ORGAO_FNDE:
			$html = "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">
						<tr>
						<td class=\"SubTituloCentro\" colspan=\"3\">TIPO DE BUSCA</td>
						</tr>
						<tr>
						<td class=\"SubTituloEsquerda\" width=\"33%\"><input type='radio' name='tipobusca' onclick='tipobusca();' value='inep' checked> Código da escola (INEP)</td>
						<td class=\"SubTituloEsquerda\" width=\"33%\"><input type='radio' name='tipobusca' onclick='tipobusca();' value='cnpj'> CNPJ</td>
						</tr>
					</table>";
			
			$javascript = "<script>
							document.getElementById('frmEntidade').onsubmit  = function(e) {
								if (document.getElementById('entnome').value == '') {
									alert('O nome da entidade é obrigatório.');
									return false;
								}
								return true;
							}
			
							document.getElementById('tr_entnumcpfcnpj').style.display = 'none';
							document.getElementById('tr_entunicod').style.display = 'none';
							document.getElementById('tr_entnuninsest').style.display = 'none';
							document.getElementById('tr_entungcod').style.display = 'none';
							document.getElementById('tr_tpctgid').style.display = 'none';
							/*
							 * DESABILITANDO A SIGLA DA ENTIDADE
							 */
							document.getElementById('entsig').readOnly = true;
							document.getElementById('entsig').className = 'disabled';
							document.getElementById('entsig').onfocus = \"\";
							document.getElementById('entsig').onmouseout = \"\";
							document.getElementById('entsig').onblur = \"\";
							document.getElementById('entsig').onkeyup = \"\";
							
							function tipobusca() {
								document.getElementById('tr_entnumcpfcnpj').style.display = 'none';
								document.getElementById('tr_entunicod').style.display = 'none';
								document.getElementById('tr_entcodent').style.display = 'none';
	        					var tpi = document.getElementsByName('tipobusca');
								for(i=0;i<tpi.length;i++) {
									if(tpi[i].checked) {
										switch(tpi[i].value) {
											case 'inep':
												document.getElementById('tr_entcodent').style.display = '';
												break;
											case 'cnpj':
												document.getElementById('tr_entnumcpfcnpj').style.display = '';
												break;
											case 'unicod':
												document.getElementById('tr_entunicod').style.display = '';
												break;
										}
									}
								}
							}
						   </script>";
					  
		break;
	
	}
endif;
	
$entidade = new Entidades();
if($_REQUEST['entid'])
	$entidade->carregarPorEntid($_REQUEST['entid']);

echo $html;

$entidade->setBuscarEntidadePorNome('juridica');

echo $entidade->formEntidade("obras.php?modulo=principal/inserir_entidade&acao=A&opt=salvarRegistro&orgid=".$_REQUEST['orgid'],
							 array("funid" => ID_SSP, "entidassociado" => null),
							 array("funidEspecifico" => Array(ID_SSP,ID_UNIDADEIMPLANTADORA), "entidassociado" => null),
							 array("enderecos"=>array(1))
							 );

echo $javascript;

?>
</div>
<div id=test></div>
</body>
</html>
