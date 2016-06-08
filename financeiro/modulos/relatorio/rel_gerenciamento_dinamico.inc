<?php

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");
//dbg($_POST, 1);
//extract($_POST);

ini_set( "memory_limit", "256M" );
set_time_limit(0);

if ($dataini){
	$dataini = explode("/",$dataini);
	$dataini = "{$dataini[2]}/{$dataini[1]}/{$dataini[0]}"; 
}

if ($datafim){
	$datafim = explode("/",$datafim);
	$datafim = "{$datafim[2]}/{$datafim[1]}/{$datafim[0]}"; 
}
//unset($_SESSION['dmdid']);

if ($_GET['dmdid']){
	session_start();
	$_SESSION['dmdid'] = $_GET['dmdid'];
	header( "Location: financeiro.php?modulo=relatorio/rel_gerenciamento_dinamico&acao=A" );
	exit();
	}
	

if( $_POST['icbdscresumida'] && $_POST['icbdsc'] && $_POST['icbdatainiciovalidade'] && $_POST['agrupadorMacro'] && !$_POST['icbcod']){
	cadastrarGerenciamento();

	unset($_SESSION['icbcod']);
	print "<script>
				alert('Operação realizada com sucesso!');
				location.href = window.location;
			 </script>";
	exit();
	
}

/**************
 * Carrega os dados do formulário para serem editados na página Gerenciamento de Contas Contábeis
 * 
****************/
if ($_GET['icbcod']){
	session_start();
	$_SESSION['icbcod'] = $_GET['icbcod'];

        $sql ="SELECT
				icbdscresumida, 
				icbdsc,
				icbdatainiciovalidade, 
				icbdatafimvalidade, 
				pc.gr_codigo_conta
			FROM 
				financeiro.informacaocontabil icb
				INNER JOIN financeiro.informacaoconta ic ON ic.icbcod = icb.icbcod 
				INNER JOIN siafi.planoconta pc ON pc.gr_codigo_conta = ic.gr_codigo_conta
			WHERE
			    ic.icbcod = ".$_GET['icbcod'].";
			  ";
			    
        $dados = $db->carregar($sql);
                
        $icbdscresumida 	   = $dados[0]['icbdscresumida'];
        $icbdsc                = $dados[0]['icbdsc'];
        $icbdatainiciovalidade = $dados[0]['icbdatainiciovalidade'];
        $icbdatafimvalidade    = $dados[0]['icbdatafimvalidade'];
        $gr_codigo_conta 	   = $dados[0]['gr_codigo_conta'];
}

include APPRAIZ . 'includes/Agrupador.php';
include  APPRAIZ . 'includes/cabecalho.inc';

echo '<br />';
monta_titulo('Relatório Módulo Financeiro', 'Gerenciamento Dinâmico de Contas Contábeis - Incluir/ Alterar');

?>



<?php

   
$possuiPerfilConsultaUnidades = false;

$sql = "SELECT 
			pu.pflcod as perfil
		FROM 
			seguranca.perfilusuario pu 
		INNER JOIN 
			seguranca.perfil p ON p.pflcod = pu.pflcod AND p.sisid = ".$_SESSION["sisid"]." 
		WHERE 
			pu.usucpf = '".$_SESSION["usucpf"]."'";

$perfilUsuario = $db->carregar($sql);

?>

<head>
	<script type="text/javascript" src="../includes/calendario.js"></script>
	<script type="text/javascript"></script>
</head>
<form  onSubmit="return validaForm(this);" action="" method="post" name="formulario" id="formulario">
	
	<!-- TABELA INCLUIR GERENCIAMENTO -->
	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
			<tr>
				<td class="SubTituloDireita" valign="top">Descrição Resumida</td>
				<td>
					<?=campo_texto('icbdscresumida', 'S', 'S' , '', 78, 100, '', '', 'left', '', 0, 'id="icbdscresumida"'); ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="top">Descrição</td>
				<td>
					<?=campo_texto('icbdsc', 'S', 'S' , '', 140, 100, '', '', 'left', '', 0, 'id="icbdsc"'); ?>
				</td>
			</tr>
			
			<tr>
				<td width="195" id="" class="SubTituloDireita" valign="top">Conta Contábil</td>
				<td>
				
				<?php
				// inicia agrupador
				$origem = $db->carregar("SELECT 
												DISTINCT 
												gr_codigo_conta as codigo,  
												gr_codigo_conta as descricao
										  FROM 
										        siafi.planoconta 
										  WHERE
										  		it_in_conta_corrente_contabil in ('16','17','31','80','26','45','50','77')
  										  ORDER BY 
  										        gr_codigo_conta");
				
				//fazendo o teste para carregar somente quando passar icbcod via GET
				if( $_GET['icbcod'] != '' )
				{	
					$destino = $db->carregar("SELECT
												DISTINCT
												pc.gr_codigo_conta as codigo,  
												pc.gr_codigo_conta as descricao
											  FROM
											    financeiro.informacaoconta ic
											  	INNER JOIN siafi.planoconta pc ON pc.gr_codigo_conta = ic.gr_codigo_conta 
											  WHERE
											    icbcod = ".$_GET['icbcod']."
											  ORDER BY
											   	pc.gr_codigo_conta ASC");
				}
		        $agrupador = new Agrupador( 'formulario', $agrupadorHtml );
                           
                // exibe agrupador
	            $agrupador->setOrigem( 'naoAgrupadoMacro', null, $origem );
	            $agrupador->setDestino( 'agrupadorMacro', null, $destino );
	            $agrupador->exibir();
	              
 				?>
					
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="top">Data Inicio Validade </td>
				<td>
						<?=campo_data('icbdatainiciovalidade', 'S', 'S', '', 'S' );?>					
				</td>	
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="top">Data Fim Validade</td>
				<td>
						<?=campo_data('icbdatafimvalidade', 'N', 'S', '', 'S' );?>	
				</td>
			</tr>
	</table>
	<!-- INSERINDO VALORES NO BANCO -->

<?php
function cadastrarGerenciamento(){
	global $db;
			
//	die();


	if( $_REQUEST['icbcod'] == ''):
		$sql = " INSERT INTO 
							financeiro.informacaocontabil
							( icbdscresumida, 
							  icbdsc, 
							  icbdatainiciovalidade, 
							  icbdatafimvalidade) 
				 VALUES
							( '".$_POST['icbdscresumida']."',
							  '".$_POST['icbdsc']."',
							  '".$_POST['icbdatainiciovalidade']."', ";
		
				if( $_POST['icbdatafimvalidade'] != '') {
					$sql .= "'".$_POST['icbdatafimvalidade']."' )";
				}elseif( $_POST['icbdatafimvalidade'] == ''){
					$sql .= "null)";
				} 			 
				
				$sql .= " RETURNING icbcod";
	    
		$icbcod = $db->pegaUm($sql);
	
		$sql = "INSERT INTO financeiro.informacaoconta ( 
						icbcod, 
					 	gr_codigo_conta
				)VALUES(
					 	".$icbcod.",
		 			 	".implode("),(".$icbcod."," , $_POST['agrupadorMacro'])."
		 		);";		
		
		$db->executar($sql);
		$db->commit();
	//dbg($_POST, 1);
	
	elseif($_REQUEST['icbcod'] != ''):
	
		// 1 update na tabela principal 
		$sql = " UPDATE 
					financeiro.informacaocontabil 
				 SET 
					icbdscresumida = '".$_POST['icbdscresumida']."',
					icbdsc = '".$_POST['icbdsc']."', 
					icbdatainiciovalidade = '".$_POST['icbdatainiciovalidade']."',";
		
         if( $_POST['icbdatafimvalidade'] != '') {
         	$sql .= "icbdatafimvalidade = '".$_POST['icbdatafimvalidade']."'"; 
		 }elseif( $_POST['icbdatafimvalidade'] == ''){
	     	$sql .= "icbdatafimvalidade = null";
			
		 }

		 $sql .= " WHERE
		 			icbcod =".$_REQUEST['icbcod'];	
		 
		//$sql .= " RETURNING icbcod";	
		$db->executar($sql);

		
		// 2 deletar a tabela auxiliar N:M
		$sql = "DELETE FROM
					financeiro.informacaoconta 
				WHERE 
					icbcod = ".$_REQUEST['icbcod'];
		
		$db->executar($sql);

		// 2.1 após ter sido deletado os registros, onde icbcod = $_request['icbcod'], insere-se os novos registros da tabela auxiliar. 
		for($i=0; $i<count($_POST["agrupadorMacro"]); $i++) {
			$sql = "INSERT INTO financeiro.informacaoconta (
						icbcod,
						gr_codigo_conta
					)VALUES(
						".$_REQUEST['icbcod'].",
					    '".$_REQUEST["agrupadorMacro"][$i]."'
					)";
				
			$db->executar($sql);
		}
		
		$db->commit();
				
	endif;	

	}
 ?>
	<!-- BOTÃO SALVAR -->		
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
			<tr>
				<td align="center">
					<input type="button" name="Salvar" value="Salvar" onclick="javascript:submeterFormulario();"/>
				</td>
			</tr>
	</table>
	
</form>
<script language="JavaScript" src="../includes/wz_tooltip.js">
</script>

<script type="text/javascript">

<!-- ACAO BOTAO SALVAR -->	
		
function submeterFormulario(){
	 selectAllOptions( document.formulario.agrupadorMacro );	
	 if(validaForm()){
	    document.formulario.submit();
	 }

}
	

 
 function validaForm(){
  	var datai = document.getElementById('icbdatainiciovalidade');
 	var dataf = document.getElementById('icbdatafimvalidade');
 	
	 if (!validaDataMaior(datai, dataf)){
             alert('Data inicial da etapa nao pode ser maior que a data final');
             datai.focus();
             return false;
         }
         
 	var icbdscresumida = document.getElementById('icbdscresumida');
 	var icbdsc = document.getElementById('icbdsc');
 	var agrupadorMacro = document.getElementById('agrupadorMacro');
 	var icbdatainiciovalidade = document.getElementById('icbdatainiciovalidade');
 	
 	
 	if(icbdscresumida.value == "" ) {
 		alert("Campo Descrição Resumida é obrigatório!");
 		icbdscresumida.focus();
 		return false;
 		}
 		
 	else if(icbdsc.value == "" ) {
 		alert("Campo Descrição é obrigatório!");
 		icbdsc.focus();
 		return false;
 		}

 	else if(agrupadorMacro.value == "" ) {
 		alert("Falta inserir valor para Conta Contábil!");
 		agrupadorMacro.focus();
 		return false;
 		}
 	else if(icbdatainiciovalidade.value == "" ) {
 		alert("Campo Data Início Validade é obrigatório!");
 		icbdatainiciovalidade.focus();
 		return false;
 		}
 	else {
 		return true;
 	}
 }
 

</script>

