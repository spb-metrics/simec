<?php
header("Cache-Control: no-cache, must-revalidate");
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");

require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/entidades.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Funcao.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/EntidadeEndereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/TipoClassificacao.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/TipoLocalizacao.php";



if ($_REQUEST['opt'] && ($_REQUEST['entnumcpfcnpj'] || $_REQUEST['entcodent'])) {
    global $db;

    // Busca CNPJ cadastrado
    if ($_REQUEST['opt'] == 'buscarCnpj') {
        $entidade = Entidade::carregarEntidadePorCnpjCpf($_REQUEST['entnumcpfcnpj'], $db->testa_superuser());

        if ($entidade->getPrimaryKey() !== null) {
            die($entidade->getPrimaryKey());
        } else {
            die('0');
        }
    } elseif ($_REQUEST['opt'] == 'buscarEscola') {
        $entidade = Entidade::carregarEntidadePorEntcodent($_REQUEST['entcodent'], $db->testa_superuser());

        if ($entidade->getPrimaryKey() !== null) {
            die($entidade->getPrimaryKey());
        } else {
            die('0');
        }
    } elseif ($_REQUEST['opt'] == 'salvarRegistro') {
        $entidade = new Entidade();

        if ($_REQUEST['entid'] != '')
    		$entidade->carregar($_REQUEST['entid']);

    		if ($_REQUEST['entdatanasc'] == ""){
    			$_REQUEST['entdatanasc'] = null;
    		}
    		
    	$entidade->BeginTransaction();

    	$entidade->entidassociado       = null;
        $entidade->entnumcpfcnpj        = str_replace(array(".", "/", "-"), "", $_REQUEST['entnumcpfcnpj']);
        $entidade->entnome              = $_REQUEST['entnome'];
        $entidade->entemail             = $_REQUEST['entemail'];
        $entidade->entnuninsest         = $_REQUEST['entnuninsest'];
        $entidade->entobs               = $_REQUEST['entobs'];
        $entidade->entstatus            = $_REQUEST['entstatus'] != '' ? $_REQUEST['entstatus'] : 'A';
        $entidade->entnumdddresidencial = $_REQUEST['entnumdddresidencial'];
        $entidade->entnumresidencial    = $_REQUEST['entnumresidencial'];
        $entidade->entnumdddcomercial   = $_REQUEST['entnumdddcomercial'];
        $entidade->entnumramalcomercial = $_REQUEST['entnumramalcomercial'];
        $entidade->entnumcomercial      = $_REQUEST['entnumcomercial'];
        $entidade->entnumdddfax         = $_REQUEST['entnumdddfax'];
        $entidade->entnumramalfax       = $_REQUEST['entnumramalfax'];
        $entidade->entnumfax            = $_REQUEST['entnumfax'];
        $entidade->entnumrg				= $_REQUEST['entnumrg'];
        $entidade->entorgaoexpedidor    = $_REQUEST['entorgaoexpedidor'];
        $entidade->entsexo   		    = $_REQUEST['entsexo'];
        $entidade->entdatanasc			= $_REQUEST['entdatanasc'];

        $entidade->njuid                = $_REQUEST['njuid'] != '' ? $_REQUEST['njuid'] : null;
        $entidade->funid                = $_REQUEST['funid'] != '' ? $_REQUEST['funid'] : null;
        $entidade->tpcid                = $_REQUEST['tpcid'] != '' ? $_REQUEST['tpcid'] : null;
        $entidade->tplid                = $_REQUEST['tplid'] != '' ? $_REQUEST['tplid'] : null;
        $entidade->tpsid                = $_REQUEST['tpsid'] != '' ? $_REQUEST['tpsid'] : null;

    	$entidade->save();
        $entidade->Commit();

        $endereco = new Endereco();
        $endereco->BeginTransaction();

        if ($_REQUEST['endid'] != '')
            $endereco->carregar($_REQUEST['endid']);

        $endereco->endcep = str_replace(array('.', '-'), '', $_REQUEST['endereco']['endcep']);
        $endereco->endnum = $_REQUEST['endereco']['endnum'];
        $endereco->endcom = $_REQUEST['endereco']['endcom'];
        $endereco->tpeid  = 1;
        $endereco->endlog = $_REQUEST['endereco']['endlog'];
        $endereco->endbai = $_REQUEST['endereco']['endbai'];
        $endereco->muncod = $_REQUEST['endereco']['muncod'];
        $endereco->estuf  = $_REQUEST['endereco']['estuf'];
        $endereco->entid  = null;//$entidade->getPrimaryKey();

        $endereco->save();
        $endereco->Commit();

        EntidadeEndereco::adicionar($entidade, $endereco);

            $codigo = '<input type="hidden" value="'.$entidade->getPrimaryKey().'" id="entid_'.$entidade->getPrimaryKey().'" name="praid[]"/>';
			$id     = '<input type="hidden" value="0" name="recoid_0" />';
            $botoes = '<img src="/imagens/alterar.gif" style="cursor: pointer" border="0" title="Editar" onclick="atualizaAutor(\\\'linha_' . $entidade->getPrimaryKey(). '\\\');"/>&nbsp<img src="/imagens/excluir.gif" style="cursor: pointer"  border="0" title="Excluir" onclick="RemoveLinha('.$entidade->getPrimaryKey().');"/>'; 

            if($_REQUEST['opt'] == 'salvarRegistro'){
            	 echo '
				<script type="text/javascript">
					
					var atual = window.opener.document.getElementById("linha_'.$entidade->getPrimaryKey().'"); 
					var tabela = window.opener.document.getElementById("tabela_autor");
					if (atual){
    					tabela.deleteRow(atual.rowIndex);
    				}
					var tamanho = tabela.rows.length;
					
					if(window.opener.document.getElementById("linha_'.$entidade->getPrimaryKey().'") == null){
						var tr = tabela.insertRow(tamanho);	
						tr.id = "linha_'.$entidade->getPrimaryKey().'";
					
						var colAcao = tr.insertCell(0);
						var colNome = tr.insertCell(1);
					
						colAcao.style.textAlign = "center";
					
						colAcao.innerHTML = \''.$botoes.'\';
						colNome.innerHTML = \'' . $codigo . $entidade->entnome . $id . '\'; 
					}
					window.close();

				</script>';
            }else{
            	echo '
				<script type="text/javascript">
					window.close();

				</script>';
            }
    }
}


?>
<html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
    <title><?= $titulo ?></title>

    <script type="text/javascript" src="/includes/funcoes.js"></script>
    <script type="text/javascript" src="/includes/prototype.js"></script>
    <script type="text/javascript" src="/includes/entidades.js"></script>

    <link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
    <script type="text/javascript">
      this._closeWindows = false;
    </script>
  </head>
  <body style="margin:10px; padding:0; background-color: #fff; background-image: url(../imagens/fundo.gif); background-repeat: repeat-y;">
    <div>
      <h3 class="TituloTela" style="color:#000000; text-align: center"><?php echo "Inserir Autor"; ?></h3>
<?php

if (!$_REQUEST['entid'] || $_REQUEST['entid'] == '') {
    $ent = new Entidade();
    $end = new Endereco();
} else {
    $ent = new Entidade((integer) $_REQUEST['entid']);
    $end = $ent->carregarEnderecos();

    if ($end[0] instanceof Endereco)
        $end = $end[0];
    else
        $end = new Endereco();
}
	echo formEntidade($ent, 'obras.php?modulo=principal/autores_projeto&acao=A&opt=salvarRegistro&tpPessoa=Fisica', PESSOA_FISICA);
?>
    </div>

    <script type="text/javascript">
        $('frmEntidade').onsubmit  = function(e)
        {
            if (Entidade.validateForm(this, ['entnumcpfcnpj', 'entnome', 'entnumdddcomercial', 'entnumcomercial', 'endcep'])) {
                /*!@
                 * SEMPRE deve-se remover os atributos 'disabled' ao submeter
                 * o formulário
                 */
                $('muncod').removeAttribute('disabled');
                $('endbai').removeAttribute('disabled');

                $('frmEntidade').submit();
	        	return true;
            } else {
                return false;
            }
        }
		if ($F('entnumcpfcnpj') == '')
        	$('entnumcpfcnpj').activate();
        else {
        	$('entnome').activate();
        	$('entnumcpfcnpj').setAttribute('readOnly', 'readOnly');
        }

    </script>
  </body>
</html>