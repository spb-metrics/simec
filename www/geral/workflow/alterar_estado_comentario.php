<?php

// inicializa sistema
require_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/workflow.php";
if ( !$db )
{
	$db = new cls_banco();
}

$docid = (integer) $_REQUEST['docid'];
$esdid = (integer) $_REQUEST['esdid'];
$aedid = (integer) $_REQUEST['aedid'];
$verificacao = stripcslashes( (string) $_REQUEST['verificacao'] );

$documento = wf_pegarDocumento( $docid );
$acao = wf_pegarAcao( $documento['esdid'], $esdid );
if ( !$documento || !$acao )
{
	?>
	<script type="text/javascript">
		alert( 'Documento ou ação inválida.' );
		window.close();
	</script>
	<?php
	exit();
}

?>
<html>
	<head>
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Connection" content="Keep-Alive">
		<meta http-equiv="Expires" content="-1">
		<title><?= $titulo ?><?= $maximo != 0 ? ' - Ecolha no máximo ' . $maximo . ' itens' : '' ; ?></title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="/../includes/Estilo.css" />
		<link rel="stylesheet" type="text/css" href="/../includes/listagem.css" />
	</head>
	<body style="font-size:7pt;" leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0" marginheight="0" bgcolor="#ffffff">
		<form method="post" action="http://<?php echo $_SERVER['SERVER_NAME'] ?>/geral/workflow/alterar_estado.php" name="combo_alterar_estado_comentario">
				
			<input type="hidden" name="docid" value="<?php echo $docid ?>"/>
			<input type="hidden" name="esdid" value="<?php echo $esdid ?>"/>
			<input type="hidden" name="aedid" value="<?php echo $aedid ?>"/>
			<input type="hidden" name="verificacao" value="<?php echo htmlentities( $verificacao ) ?>"/>
			
			<table width="100%" align="center" border="0" cellspacing="1" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5; font-size: 8pt;">
				
				<!-- NOME DO DOCUMENTO -->
				<tr>
					<td style="background-color: #d0d0d0; text-align: center;" colspan="2">
						<b><?php echo $documento['docdsc']; ?></b>
					</td>
				</tr>
				
				<!-- ESTADO ATUAL -->
				<tr>
					<td width="20%" align="right" class="subtitulodireita" style="background-color: #dfdfdf;">
						Estado atual
					</td> 
					<td width="80%">
						<?php echo $acao['esddscorigem']; ?>
					</td>
				</tr>
				
				<!-- AÇÂO -->
				<tr>
					<td align="right" class="subtitulodireita" style="background-color: #dfdfdf;">
						Ação
					</td> 
					<td>
						<?php echo $acao['aeddscrealizar']; ?>
					</td>
				</tr>
				
				<!-- COMENTÁRIO -->
				<tr>
					<td align="right" class="subtitulodireita" style="background-color: #dfdfdf;">
						Comentário
					</td> 
					<td>
						<?php
						echo campo_textarea(
							"cmddsc",		// nome do campo
							"N",			// obrigatorio
							"S",			// habilitado
							"Comentário",	// label
							61,				// colunas
							17,				// linhas
							6000			// quantidade maximo de caracteres
						)
						?>
					</td>
				</tr>
				
				<tr>
					<td colspan="2" style="text-align: center; background-color: #d0d0d0;">
						<input type="submit" name="alterar_estado" value="Tramitar" style="font-size: 8pt;"/>
					</td>
				</tr>
				
			</table>
		</form>
	</body>
</html>