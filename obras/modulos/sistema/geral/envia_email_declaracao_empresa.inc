<?

include_once "config.inc";
include_once APPRAIZ . "includes/classes/controller/Controller.class.inc";
include_once APPRAIZ . "obras/classe/controller/DeclaracaoController.class.inc";
include_once APPRAIZ . "obras/classe/modelo/Declaracao.class.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";

$modulo=$_REQUEST['modulo'] ;
	
if ($_REQUEST['submeter'] == 'ok'){

	// Envia email
	  $sql="SELECT 
				entnome,
				entemail
			FROM
				obras.declaracao dcl
			LEFT JOIN 
				obras.grupodistribuicao gd ON gd.gpdid = dcl.gpdid AND gpdstatus = 'A' 
			LEFT JOIN 
				obras.empresacontratada ec ON  ec.epcid = gd.epcid 
			LEFT JOIN 
				entidade.entidade e ON ec.entid = e.entid
			WHERE 
				dcl.dclid =".$_REQUEST['dclid']."";
	  $RSu = $db->record_set($sql);
	  $resu =  $db->carrega_registro($RSu,0);
		  if(is_array($resu)) foreach($resu as $k=>$v) ${$k}=$v;
			  $entnome = $resu['entnome'];
			  $entemail = $resu['entemail']; 
			  $assunto = $_REQUEST['assunto'];
			  $cc=$_REQUEST['cc'];
			  $cco=$_REQUEST['cco'];
			
			  $obDeclaracao = new DeclaracaoController();
			  $obDeclaracao->ativaDadosDeclaracao( array("dclid", "arqid"), $_REQUEST['dclid'] );
			  $obDeclaracao->ativaDadosArquivo(array("arqextensao", "arqnome"));
			  $caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($obDeclaracao->dclid/1000) .'/';
			  $name    = $obDeclaracao->arqnome . '.' . $obDeclaracao->arqextensao;
			
				if ( file_exists($caminho . $name) ){
					
					$mensagem = file_get_contents( $caminho . $name );
					
					$mensagem = str_replace("<table ", "<table style=\"width:100%\" ", $mensagem);
					$mensagem = str_replace("<td class=\"SubTituloCentro\" ", "<td style='background-color: #F0F0F0; color: black; font-family: Arial,Verdana; font-size: 8pt; font-weight: bold; text-align: center;' ", $mensagem);
					//$mensagem = str_replace("../imagens/brasao.gif","http://simec.mec.gov.br/imagens/brasao.gif", $mensagem);
					$mensagem = str_replace("<a class=\"notprint\" style=\"cursor:pointer; float:right; margin-top: 50px; margin-right: 20px;\" onclick=\"window.print();\"><img src=\"../imagens/ico_print.jpg\" border=\"0\"></a>"," ", $mensagem);
					$mensagem = str_replace("../includes/Estilo.css","http://simec.mec.gov.br/includes/Estilo.css", $mensagem);
					$mensagem = str_replace("../includes/listagem.css","http://simec.mec.gov.br/includes/listagem.css", $mensagem);
					
					$textoInicio = ' <b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Segue a nova Declaração que foi gerada nesta data. O documento original será enviado via SEDEX.</b>&nbsp;<br>&nbsp;<br>&nbsp;<br>&nbsp;<br> '; 
					$textoFim = '<br><br><b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Atenciosamente,<br><p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Equipe de Monitoramento de Obras</b>';
					
					$mensagem = $textoInicio.$mensagem.$textoFim;
					
									
				}else{
					$mensagem = "Não foi encontrada a Declaração.";
				}
	  email($entnome, $entemail, $assunto,$mensagem,$cc,$cco);
	  ?>
	      <script>
	         alert('Email enviado com sucesso. Esta janela será fechada.');
	         window.close();
	      </script>
	  <?
	  exit();
}

	$sql="SELECT	
    			entnome,
				entemail
		  FROM
				obras.declaracao dcl
		  LEFT JOIN 
				obras.grupodistribuicao gd ON gd.gpdid = dcl.gpdid AND gpdstatus = 'A' 
		  LEFT JOIN 
				obras.empresacontratada ec ON  ec.epcid = gd.epcid 
		  LEFT JOIN 
				entidade.entidade e ON ec.entid = e.entid
		WHERE 
			dcl.dclid =".$_REQUEST['dclid'];
    $RSu = $db->record_set($sql);
    $resu =  $db->carrega_registro($RSu,0);
    if(is_array($resu)) foreach($resu as $k=>$v) ${$k}=$v;
 
?>
<html>
	<head>
		<title>Envio de Email</title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
		<script src="../includes/calendario.js"></script>
		<script src="../obras/js/obras.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body bgcolor="#ffffff" leftmargin="0" rightmargin="0" topmargin="0" bottommargin="0" marginheight="0" marginwidth="0">
		<form id="formulario" name="formulario" method="post" action="">
			<input type=hidden name="modulo" value="<?=$modulo?>">
			<input type=hidden name="dclid" value='<?=$_REQUEST['dclid']?>'>
			<input type=hidden name="submeter" value="ok">
			<table width='100%' align='center' border="0" cellspacing="1" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
			     <tr>
				 <td colspan="2" align="Center" bgcolor="#dedede">Enviar Email</td>
				 </tr>
				  <tr>
			        <td align="right" class="subtitulodireita">Para:</td> 
			        <td><?=campo_texto('entnome','N','N','',70,100,'','');?></td>
			     </tr>
				 <tr>
			        <td align="right" class="subtitulodireita">Cc:</td>
			        <? $cc = 'monitoramentoobras@presidencia.gov.br' ?> 
			        <td><?=campo_texto('cc','N','S','',70,100,'','');?></td>
			     </tr>
			     <tr>
			        <td align="right" class="subtitulodireita">Cco:</td> 
			        <td><?=campo_texto('cco','N','S','',70,100,'','');?></td>
			     </tr>     
				  <tr>
			        <td align="right" class="subtitulodireita">Assunto:</td>
			         <?$assunto = 'SAA - Subsecretaria de Assuntos Administrativos - Declaração n°:'.$_REQUEST['dclid'];?>
			        <td><?=campo_texto('assunto','S','S','',70,100,'','');?></td>
			     </tr>
			 </table>
			<?php
				
			
			$obDeclaracao = new DeclaracaoController();
			$obDeclaracao->ativaDadosDeclaracao( array("dclid", "arqid"), $_REQUEST['dclid'] );
			$obDeclaracao->ativaDadosArquivo(array("arqextensao", "arqnome"));
			
			$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($obDeclaracao->dclid/1000) .'/';
			$name    = $obDeclaracao->arqnome . '.' . $obDeclaracao->arqextensao;
			
			if ( file_exists($caminho . $name) ){
				$textoInicio = ' <br><b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Segue a nova Declaração que foi gerada nesta data. O documento original será enviado via SEDEX.</b>&nbsp;<br>&nbsp;<br>&nbsp;<br>&nbsp;<br> '; 
				$conteudo = file_get_contents( $caminho . $name );
				$textoFim = '<br><br><b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Atenciosamente,<br><p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Equipe de Monitoramento de Obras</b>';
//				echo file_get_contents( $caminho . $name );
				echo $textoInicio.$conteudo.$textoFim;	
			}else{
				echo "<table align=\"center\"><tr><td><font style='color:red;'>Não foi encontrada a Declaração.</font></td></tr></table>";
				$declaracaoNaoEmitida = '<font style=\'color:red;\'>Não foi encontrada a Declaração.</font>';	
			}	

?>	
			<table width='100%' align='center' border="0" cellspacing="1" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
			 	 <tr>
					 <td colspan="2" align="right" class="subtitulodireita">
					 	<input type="submit" class="botao" value='Enviar E-mail' onclick="envia_email_declaracao_empresa(<?=$_REQUEST['dclid']; ?>)">
					 	&nbsp;&nbsp;&nbsp;
					 	<input type='button' class="botao" value='Fechar' onclick="fechar_janela(<?=$_REQUEST['dclid']; ?>)">
					 </td>
				 </tr>
			</table>
			<input type=hidden name="declaracaoNaoEmitida" value="<?=$declaracaoNaoEmitida?>">

		</form>
		 
	</body>
</html>

<script>

  function fechar_janela()
  {
    window.close();

  }
    function envia_email_declaracao_empresa()
  {

  	if (!validaBranco(document.formulario.assunto, 'Assunto')) return;
	//verificação do campo corpo email
	//document.formulario.email.value = email.getContent('email');
	if (!validaBranco(document.formulario.email, 'Texto da Mensagem')) return tinyMCE.execCommand('mceFocus', true, 'declaracaoNaoEmitida');
	
	document.formulario.submit();

  }

</script>

