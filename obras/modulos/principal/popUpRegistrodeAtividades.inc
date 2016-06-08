<html>
<head>
	<script language="javascript"> 
	tmt_Move_WindowX = (screen.width - 850 ) / 2; 
	tmt_Move_WindowY = (screen.height - 500 ) / 2; 
	self.moveTo(tmt_Move_WindowX,tmt_Move_WindowY); 
	</script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../../includes/listagem.css"/>
	<script type="text/javascript" src="../includes/funcoes.js"></script>
</head>
<?php 
//Condição para atualizar a pagina de origem
if ($_POST['submeter'] == 'ok'){
		$onunload = 'onunload="window.opener.location=\'?modulo=principal/registroAtividade&acao=A\';"';
		echo "<script>
				window.close;
			  </script>";
}
?>
<body <?=$onunload ?>>
<?php
//Altera as inforamções do Registro de Atividades
if( $_POST['registro'] == 'alterar'){
	$sql="UPDATE 
				obras.registroatividade
		  SET 
	  			rtvdscsimplificada = '".trim($_POST['rtvdscsimplificada'])."', 
	  			rtvdsccompleta     = '".trim($_POST['rtvdsccompleta'])."', 
		       	rtvdtinclusao      = '".date("Y-m-d H:i:s")."'
		 WHERE 	
		  		rtvid ='".$_REQUEST['rtvid']."'
		       	AND
				rtvstatus= 'A'"; 
	$db->executar($sql);
	
	echo("<script>
			alert('Operação realizada com sucesso!');
			window.close(); 
		</script>");
}
//Insere as inforamções do Registro de Atividades
else if( $_POST['registro'] == 'salvar'){
	
		//Insere a descrição simplificada e a descrição completa
	$sql = "INSERT INTO 
				obras.registroatividade
			 	(obrid, 
			 	usucpf,
			  	rtvdscsimplificada, 
			  	rtvdsccompleta, 
			  	rtvstatus,
			  	rtvdtinclusao)
		    VALUES 
		    	('{$_SESSION["obra"]["obrid"]}',
				'{$_SESSION["usucpf"]}',
				'".trim($_POST['rtvdscsimplificada'])."', 
				'".trim($_POST['rtvdsccompleta'])."',
				'A',
				'".date("Y-m-d H:i:s")."')";
		$db->executar($sql);
	
		echo("<script>
				alert('Operação realizada com sucesso!');
				window.close(); 
			</script>");
}
	//Recupera as informações do Registro de Atividade, quando solicitada a alteração
	if($_REQUEST['rtvid']){
		// Monta Título	de apresentação da tela
		monta_titulo( "Alterar Registro de Atividade", "" );
		//Recupera a descrição simplificada e a descrição completa
		$sql = "SELECT 
					rtvdscsimplificada, 
					rtvdsccompleta
				FROM 
					obras.registroatividade
				WHERE
					rtvstatus = 'A'
					AND
					rtvid = '".$_REQUEST['rtvid']."'";	
		
		$registroAtividade = $db->pegaLinha($sql);
		//Variável que indica a opção para alterar informações
		$registro = 'alterar'; 
		$submeter = 'ok';
	}
	//Senão recuperar as informações do Registro de Atividade, será apresentada a opção para inserir
	else{
		// Monta Título	de apresentação da tela
		monta_titulo( "Registro de Atividade", "" );
		//Variável que indica a opção para salvar informações
		$registro = 'salvar';
		$submeter = 'ok'; 
	}
?>
<!--Formulário do Popup-->
<form id="formulario" name="formulario" method="post" action="">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td class="SubTituloDireita" width="190px">Descrição simplificada:</td>
			<td>
			<?php
				 $rtvdscsimplificada = trim($registroAtividade['rtvdscsimplificada']); 
			echo campo_texto( 'rtvdscsimplificada', 'N', 'S', '', 47 , 60, '', '');	
			?> 
			</td>
		</tr>
		<tr>	
			<td class="SubTituloDireita" width="190px">Descrição detalhada: </td>
			<td>
			<?php
				 $rtvdsccompleta = trim($registroAtividade['rtvdsccompleta']);	
			echo campo_textarea( 'rtvdsccompleta', 'N', 'S', '', 100, 20, 5000 );  
			?>
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td></td>
			<td>
			<div style="float: left;"> 
				<input type="hidden" name="registro" value="<?php echo $registro; ?>"/>
				<input type="hidden" name="submeter" value="<?php echo $submeter; ?>"/>
				<input type="submit" value="Salvar" style="cursor: pointer" ondblclick="numRegistro();" style="cursor: pointer;"/>
				<input type="button" value="Fechar" style="cursor: pointer" onclick="window.close();">
			</div>
			</td>
		</tr>
	</table>
</form>
<script>
function numRegistro(obj) {
}

</script>
		
</body>
</html>