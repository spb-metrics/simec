<?php

if( $_POST['ajaxid'] != '' ){ 
	if( !salva($_POST['ajaxid'], $_POST['idPessoa'])){
		echo "f";
		die();
	}
	$lbChefe = getLabelChefe($_POST['ajaxid']);
	echo "<img src=\"/imagens/alterar.gif\" border=0 >&nbsp;".$lbChefe;
	die();
}
function getLabelChefe($cpf){
	global $db;
	$sql = "SELECT sernome FROM gestaopessoa.servidor WHERE sercpf = '$cpf'";
	$lbChefe = $db->pegaUm( $sql );
	if( $lbChefe ){
		return $lbChefe;
	}
}
function salva($chefe, $id){
	if( existeNotaAvaliado( $id ) ){
		return false;
	}
	global $db;
	$sql = "UPDATE gestaopessoa.servidor SET sercpfchefe = '".$chefe."' WHERE sercpf = '".$id."'"; 
	$up  = $db->executar( $sql ); 
	$db->commit();
	return true;
}
monta_titulo( 'Gestão de Pessoas', 'Avaliação' ); 
?>
<script src="../includes/prototype.js"></script>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<form name="formulario" id="formulario" method="post">
<input type="hidden" name="salvar" value="0"> 
 
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
    <tr>
    <input type="hidden" name="idPessoa" id="idPessoa" value = "<?=$_GET['idPessoa'];?>" >
        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Nome:
		 </td>
        <td width='50%'>
		<?php echo campo_texto( 'filtro_nome', 'N', 'S', '', 50, 200, '', '' ); ?>
        </td>      
    </tr>
    <tr>
    	<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">&nbsp;</td>
    	<td>
    		<input type="button" name="" value="Pesquisar" onclick="return validaForm();"/>
    	</td>
    </tr>
</table>
</form>
<?
echo "<div align=\"center\" id='msg' style='display:none; position: relative; top: 50px;'></div>";
if( $_POST['filtro_nome']){
	$where = " AND s.sernome ILIKE '%".$_POST['filtro_nome']."%' ";
}
$sql = "SELECT 
			'<input type=\"radio\" name=\"chefe\" id=\"chefe\" value=\"'||u.usucpf||'\" onchange=\"alteraValue(this.value);\"> &nbsp;'|| u.usucpf as cpf, 
			s.sernome, 
			tl.tlsdescricao 
			from gestaopessoa.servidor as s
	    INNER JOIN  seguranca.usuario as u on u.usucpf = s.sercpf
	    INNER JOIN  gestaopessoa.tipolotacaoservidor AS tl ON tl.tlsid = s.tlsid
	    INNER JOIN  gestaopessoa.tiposituacaoservidor AS ts ON ts.tssid = s.tssid 
	    WHERE s.sercpf IS NOT NULL
	    AND s.seranoreferencia = {$_SESSION['exercicio']}
	    $where
	    ";
$cabecalho = array( "CPF&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;","Nome", "Lotação"  );  
$db->monta_lista( $sql, $cabecalho, 10, 10, 'N', '', '');
?> 
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
    <tr>
        <td align='right' class="SubTituloEsquerda" style="vertical-align:top; width:25%"><input type="button" value="Selecionar" onclick="salvarAjax();" id="bt_select" name="bt_select" ></td>
    </tr>
</table>
<script type="text/javascript">
function salvarAjax(){
	 var chefe 	  = document.getElementById('chefe'); 
	 var campo_idPessoa = document.getElementById('idPessoa'); 
	 if( !chefe.value ){
	 	alert('Selecione um Chefe');
	 	return false;
	 }
	 var chefeid = chefe.value; 
	 var idPessoa = campo_idPessoa.value;
	 var msg = document.getElementById('msg');
	 var req = new Ajax.Request('gestaopessoa.php?modulo=principal/popupChefes&acao=A', {
					        method:     'post',
					        parameters: '&ajaxid=' + chefeid +'&idPessoa='+idPessoa,		
					        onLoading: carregando(),					         
					        onComplete: function (res) {	 
								var lbChefe = res.responseText; 
						 		if( res.responseText == 'f'){
									alert('Não é possivel a mudança de chefia para este servidor, o mesmo ja sencontra-se com nota de avaliação superior cadastrada'); 
									window.close();	
									return false;
								}
		
								window.opener.document.getElementById('chefe['+idPessoa+']').innerHTML = lbChefe;
								msg.style.display ='none';
								alert('Operação realizada com sucesso!');
								window.close();								
							}
	 });
}
function alteraValue(value){
	var chefe = document.getElementById('chefe');
	chefe.value = value;
	return true;
}
function carregando(){	
	var msg = document.getElementById('msg');
	msg.style.display="block";
	msg.innerHTML="<img src=\"../imagens/wait.gif\"'>";
}
function validaForm(){
	document.formulario.submit();
}
</script>