<?php
if( $_POST['ajaxsiape'] ){	 
	header('content-type: text/html; charset=ISO-8859-1'); 
	$sql = "SELECT  
			ds_email,
			nu_telefone 
		    FROM gestaopessoa.vw_simec_consulta_servidor_siape 
		    WHERE 
		    replace( replace(nu_cpf,'.', ''), '-','') = replace( replace('".$_POST['ajaxsiape']."','.', ''), '-','') ";
	$rs  = $db->carregar( $sql );
	if( $rs ){
		$res = 
		$rs[0]['ds_email'].'_'.
		$rs[0]['nu_telefone'];
		die( $res );
	} 
	die();
}
include  APPRAIZ."includes/cabecalho.inc";
echo '<br>'; 
include_once( APPRAIZ. "gestaopessoa/classes/FtDadoFuncional.class.inc" );

$bloquearEdicao = bloqueiaEdicaoFT();

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Força de Trabalho', 'Dados Funcionais' ); 

$tipo 				= getSituacaoMEC($_SESSION['fdpcpf']); 
$arCamposFuncionais = controlaDadoFuncional( $tipo ); 
echo cabecalhoPessoa($_SESSION['fdpcpf']);

$df = new FtDadoFuncional();

if( $_REQUEST['del'] != ''){
	$df->excluir( $_REQUEST['del'] );
	$df->commit();
	$df->sucesso("principal/cadDadosFuncionais");
}
$id = $df->pegaUm( "select fdfid from gestaopessoa.ftdadofuncional where fdpcpf = '". $_SESSION["fdpcpf"]."'" );
 
if( $id ){ 	
	
	$df->carregarPorId( $id );
	
	$sql = "SELECT * FROM gestaopessoa.ftdadofuncional WHERE fdfid = $id";
	$dados = $db->carregar( $sql );
 
}
if( $_POST ){
 
  	$arDados = controlaDadoFuncional( $tipo );  
  	$df->fdpcpf = $_SESSION['fdpcpf'];
  	$df->popularObjeto($arDados);
  	$df->salvar();
	$df->commit();
    $df->sucesso( "principal/cadDadosFuncionais" );
}
?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<center> Preencha os Dados Funcionais  </center>
 <form name = "formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    		<? if( in_array('fcmid',$arCamposFuncionais ) ){ ?>
         	<tr>
                <td class ="SubTituloDireita" align="right">
<?php

If (($_SESSION['sit']==5) || ($_SESSION['sit']==6) || ($_SESSION['sit']==10) || ($_SESSION['sit']==11)){
	echo "Cargo Efetivo:";
}

else {
	echo "Cargo Efetivo no MEC:";
}
?>	</td>
                <td>
                <?
                	$sql = "SELECT 
                            fcmid as codigo, 
                            fcmdescricao as descricao
                            FROM gestaopessoa.ftcargoefetivomec
                            ORDER BY fcmdescricao 
                       ";  
                $fcmid = $dados[0]['fcmid'];
                $db->monta_combo('fcmid', $sql, 'S', "Selecione...", '', '', '', '310', 'S', 'fcmid');
                ?>
                </td>
            </tr>
            <? } ?>
            <? if( in_array('fdfexercecargofuncao',$arCamposFuncionais ) ){ ?>
            <tr>
                <td class ="SubTituloDireita" align="right"> Exerce Cargo/Função: </td>
                <td>
                	<input type="radio" name="fdfexercecargofuncao" id="fdfexercecargofuncao" <? if( $dados[0]['fdfexercecargofuncao'] == 't') echo 'checked = checked'; ?> value="t" > Sim
                	<input type="radio" name="fdfexercecargofuncao" id="fdfexercecargofuncao" <? if( $dados[0]['fdfexercecargofuncao'] == 'f') echo 'checked = checked'; ?> value="f"> Não
                	&nbsp;&nbsp;&nbsp;&nbsp;  
                	<?
                	$fdespecificacaocargofuncao = $dados[0]['fdespecificacaocargofuncao'];
                	?>
                	</a>
                	<!-- 
                	<?= campo_texto('fdespecificacaocargofuncao', 'S', $somenteLeitura, '', 80, 600, '', '', 'left', '',  0, 'id="fdespecificacaocargofuncao" onblur="MouseBlur(this);"' ); ?>  
                	-->
                </td>
            </tr>  
            <? } ?>
            <? if( in_array('fulid',$arCamposFuncionais ) ){ ?>
           <tr>
                <td class ="SubTituloDireita" align="right">Unidade de Lotação: </td>
                <td>
                <?
                	$fulid = $dados[0]['fulid'];
          
                	$sql = "SELECT 
                            fulid AS codigo, 
                            fuldescricao AS descricao
                        FROM
                            gestaopessoa.ftunidadelotacao 
                       ";  
                $db->monta_combo('fulid', $sql, 'S', "Selecione...", '', '', '', '400', 'S', 'fulid');
                ?>
                </td>
            </tr>
            <? } ?>
            <? if( in_array('forid',$arCamposFuncionais ) ){ ?>
             <tr>
                <td class ="SubTituloDireita" align="right">Orgão Origem do Requisitado: </td>
                <td>
                <?
                	$sql = "SELECT 
                            forid AS codigo, 
                            fordescricao AS descricao
                     FROM
                            gestaopessoa.ftorigem
                       ";
               	$forid = $dados[0]['forid'];  
                $db->monta_combo('forid', $sql, 'S', "Selecione...", '', '', '', '400', 'S', 'forid');
                ?>
                </td>
            </tr>
            <? } ?>
            <? if( in_array('fooid',$arCamposFuncionais ) ){ ?>
            <tr>
                <td class ="SubTituloDireita" align="right">Orgão Origem do Exercicio Provisório: </td>
                <td>
                <?
                	$sql = "SELECT 
                            fooid AS codigo, 
                            foodescricao AS descricao
                     FROM
                            gestaopessoa.ftorgaoorigem
                       ";  
                $fooid = $dados[0]['fooid'];  
                $db->monta_combo('fooid', $sql, 'S', "Selecione...", '', '', '', '100', 'S', 'fooid');
                ?>
                </td>
            </tr>
            <? } ?>
            <? if( in_array('furid',$arCamposFuncionais ) ){ ?>
             <tr>
                <td class ="SubTituloDireita" align="right">Unidade Responsável: </td>
                <td>
                <?
                	$sql = "SELECT 
                            furid AS codigo, 
                            furdescricao AS descricao
                     FROM
                            gestaopessoa.ftunidaderesponsavel
                       ";  
                $furid = $dados[0]['furid'];  
                $db->monta_combo('furid', $sql, 'S', "Selecione...", '', '', '', '400', 'S', 'furid');
                ?>
                </td>
            </tr>
            <? } ?>
            <? if( in_array('fdfsala',$arCamposFuncionais ) ){ ?>
            <tr>
                <td class ="SubTituloDireita" align="right">Sala: </td>
                <td><? $fdfsala = $dados[0]['fdfsala'];   ?>
                	 <?= campo_texto('fdfsala', 'S', $somenteLeitura, '', 10, 50, '', '', 'left', '',  0, 'id="fdfsala" onblur="MouseBlur(this);"' ); ?>  
                </td>
            </tr> 
            <? } ?>
            <? if( in_array('fdfpostotrabalho',$arCamposFuncionais ) ){ ?>
            <tr>
                <td class ="SubTituloDireita" align="right">Posto de Trabalho: </td>
                <td><? $fdfpostotrabalho = $dados[0]['fdfpostotrabalho'];   ?>
                	 <?= campo_texto('fdfpostotrabalho', 'S', $somenteLeitura, '', 80, 200, '', '', 'left', '',  0, 'id="fdfpostotrabalho" onblur="MouseBlur(this);"' ); ?>  
                </td>
            </tr> 
            <? } ?>
            <? if( in_array('fdfempresa',$arCamposFuncionais ) ){ ?>
            <tr><? $fdfempresa = $dados[0]['fdfempresa'];   ?>
                <td class ="SubTituloDireita" align="right">Empresa do Terceirizado: </td>
                <td>
                	 <?= campo_texto('fdfempresa', 'S', $somenteLeitura, '', 80, 200, '', '', 'left', '',  0, 'id="fdfempresa" onblur="MouseBlur(this);"' ); ?>  
                </td>
            </tr> 
            <? } ?>
            <? if( in_array('fdfcnpjempresa',$arCamposFuncionais ) ){ ?>
            <tr><? $fdfcnpjempresa = $dados[0]['fdfcnpjempresa'];   ?>
                <td class ="SubTituloDireita" align="right">CNPJ da Empresa: </td>
                <td>
                	 <?= campo_texto('fdfcnpjempresa', 'S', $somenteLeitura, '', 20, 18, '', '', 'left', '',  0, 'id="fdfcnpjempresa" onblur="MouseBlur(this);"' ); ?>  
                </td>
            </tr> 
            <? } ?>
            <? if( ( in_array('fdfprojetodatainicio',$arCamposFuncionais ) && (in_array('fdfprojetodatafim',$arCamposFuncionais ) ) )){ ?>
            <tr><?
				 $fdfprojetodatainicio = formata_data( $dados[0]['fdfprojetodatainicio']) ; 
				 $fdfprojetodatafim    = formata_data( $dados[0]['fdfprojetodatafim'] ); 
            	?>
                <td class ="SubTituloDireita" align="right">Período do Projeto: </td>
                <td>   
                <?= campo_data2( 'fdfprojetodatainicio','S', 'S', '', 'S' ); ?> à  
                <?= campo_data2( 'fdfprojetodatafim','S', 'S', '', 'S' ); ?> 
                </td> 
            </tr>
            <? } ?>
            <? if( in_array('fdftelefone',$arCamposFuncionais ) ){ ?>
            <tr>
                <td class ="SubTituloDireita" align="right">Telefone: </td>
                <td><? $fdftelefone = $dados[0]['fdftelefone'];   ?>
                	 <?= campo_texto('fdftelefone', 'S', $somenteLeitura, '', 20, 20, '', '', 'left', '',  0, 'id="fdftelefone" onblur="MouseBlur(this);"' ); ?>  
                </td>
            </tr> 
            <? } ?>
            <? if( in_array('fdfemail',$arCamposFuncionais ) ){ ?>
            <tr>
                <td class ="SubTituloDireita" align="right">Email: </td>
                <td><? $fdfemail = $dados[0]['fdfemail'];   ?>
                	 <?= campo_texto('fdfemail', 'S', $somenteLeitura, '', 50, 100, '', '', 'left', '',  0, 'id="fdfemail" onblur="MouseBlur(this);"' ); ?>  
                </td>
            </tr>  
            <? } ?>
            <!-- 
             <tr>
                <td class ="SubTituloDireita" align="right">Dependentes: </td>
                <td>
                	<table class ="tabela"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="left">
                		<tr>
                			<th>Dependente</th>
                			<th>IRPF</th>
                			<th>Plano de Saúde</th>
                		</tr>
                		<tr>
                			<td>Dependente 1</td>
                			<td>--</td>
                			<td>--</td>
                		</tr>
                		<tr>
                			<td>Dependente 2</td>
                			<td>--</td>
                			<td>--</td>
                		</tr>
                		<tr>
                			<td>Dependente 3</td>
                			<td>--</td>
                			<td>--</td>
                		</tr>
                	</table>
                </td>
            </tr>
             --> 
            <tr>
            	<td class ="SubTituloDireita" align="right">   </td>
            	<td><input type="button" name="btSalvar" id="btSalvar" onclick="validaForm();" value="Salvar" <?=$bloquearEdicao;?>></td>
            </tr>
            
     </table>
     </form>
     
<script>
<? if( !$id ){ ?>
	pegaDadosSIAPE('<?=$_SESSION['fdpcpf'];?>');
<? } ?>
function pegaDadosSIAPE(cpf){
	if( !cpf ){
		return false;
	}
	var fdfemail 	= document.getElementById('fdfemail');
	var fdftelefone = document.getElementById('fdftelefone');
	var req = new Ajax.Request('gestaopessoa.php?modulo=principal/cadDadosFuncionais&acao=A', {
							        method:     'post',
							        parameters: '&ajaxsiape=' + cpf,							         
							        onComplete: function (res)
							        {	 
										var arResp = res.responseText.split("_");
		 
										var ds_email = arResp[0];
										var nu_telefone = arResp[1]; 
 										 
 								 		fdfemail.value = ds_email;
 								 		if(nu_telefone)	fdftelefone.value = nu_telefone; 
							        } 
							  });
}

function validaForm(){
	
	<? if( in_array('fcmid',$arCamposFuncionais ) ){ ?>
		var fcmid = document.getElementById('fcmid');
		if( fcmid.value == '' ){
			alert( 'Campo Cargo Efetivo é obrigatório. ');
			return false;
		}
    <? } ?>
	<? if( in_array('fdfexercecargofuncao',$arCamposFuncionais ) ){ ?>
		var fdfexercecargofuncao = document.getElementById('fdfexercecargofuncao');
		if( fdfexercecargofuncao.value == '' ){
			alert( 'Campo "Exerce Cargo/Função é obrigatório. ');
			return false;
		}
	<? } ?>
	<? if( in_array('fulid',$arCamposFuncionais ) ){ ?>
          var fulid = document.getElementById('fulid');
		if( fulid.value == '' ){
			alert( 'Campo Unidade de Lotação é obrigatório. ');
			return false;
		}
    <? } ?>
    <? if( in_array('forid',$arCamposFuncionais ) ){ ?>
            var forid = document.getElementById('forid');
		if( forid.value == '' ){
			alert( 'Campo Orgão requisitado é obrigatório. ');
			return false;
		}
    <? } ?>
    <? if( in_array('fooid',$arCamposFuncionais ) ){ ?>
           var fooid = document.getElementById('fooid');
		if( fooid.value == '' ){
			alert( 'Campo Orgão de Origem é obrigatório. ');
			return false;
		}
    <? } ?>
    <? if( in_array('furid',$arCamposFuncionais ) ){ ?>
            var furid = document.getElementById('furid');
		if( furid.value == '' ){
			alert( 'Campo Unidade Responsável é obrigatório. ');
			return false;
		}
    <? } ?>
    <? if( in_array('fdfsala',$arCamposFuncionais ) ){ ?>
        var fdfsala = document.getElementById('fdfsala');
		if( fdfsala.value == '' ){
			alert( 'Campo Sala é obrigatório. ');
			return false;
		}
    <? } ?>
    <? if( in_array('fdfpostotrabalho',$arCamposFuncionais ) ){ ?>
        var fdfpostotrabalho = document.getElementById('fdfpostotrabalho');
		if( fdfpostotrabalho.value == '' ){
			alert( 'Campo Posto de Trabalho é obrigatório. ');
			return false;
		}
    <? } ?>
    <? if( in_array('fdfempresa',$arCamposFuncionais ) ){ ?>
            var fdfempresa = document.getElementById('fdfempresa');
		if( fdfempresa.value == '' ){
			alert( 'Campo Empresa é obrigatório. ');
			return false;
		}
    <? } ?>
    <? if( in_array('fdfcnpjempresa',$arCamposFuncionais ) ){ ?>
            var fdfcnpjempresa= document.getElementById('fdfcnpjempresa');
		if( fdfcnpjempresa.value == '' ){
			alert( 'Campo CNPJ da Empresa é obrigatório. ');
			return false;
		}
    <? } ?>
    <? if( ( in_array('fdfprojetodatainicio',$arCamposFuncionais ) && (in_array('fdfprojetodatafim',$arCamposFuncionais ) ) )){ ?>
            var fdfprojetodatainicio = document.getElementById('fdfprojetodatainicio');
             var fdfprojetodatafim = document.getElementById('fdfprojetodatafim');
		if( fdfprojetodatainicio.value == '' || fdfprojetodatafim.value == ''){
			alert( 'O período do projeto é obrigatório. ');
			return false;
		}
    <? } ?>
    <? if( in_array('fdftelefone',$arCamposFuncionais ) ){ ?> 
        var fdftelefone = document.getElementById('fdftelefone');
		if( fdftelefone.value == '' ){
			alert( 'Campo Telefone é obrigatório. ');
			return false;
		} 
    <? } ?>
    <? if( in_array('fdfemail',$arCamposFuncionais ) ){ ?>
            var fdfemail = document.getElementById('fdfemail');
		if( fdfemail.value == '' ){
			alert( 'Campo Email é obrigatório. ');
			return false;
		}
    <? } ?>
            
	document.formulario.submit();
}
</script>