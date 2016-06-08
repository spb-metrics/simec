<?php

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
$db = new cls_banco();

include APPRAIZ . 'includes/workflow.php';

$docid = (integer) $_REQUEST['docid'];
$documento = wf_pegarDocumento( $docid );
$atual = wf_pegarEstadoAtual( $docid );
$historico = wf_pegarHistorico( $docid );

?>
<html>
	<head>
		<title>SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação</title>
		<script language="JavaScript" src="../../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../../includes/listagem.css">
		<!-- biblioteca javascript local -->
		<script type="text/javascript">
			
			IE = !!document.all;
			
			function exebirOcultarComentario( docid )
			{
				id = 'comentario' + docid;
				div = document.getElementById( id );
				if ( !div )
				{
					return;
				}
				var display = div.style.display != 'none' ? 'none' : 'table-row';
				if ( display == 'table-row' && IE == true )
				{
					display = 'block';
				}
				div.style.display = display;
			}
			
		</script>
	</head>
	<body topmargin="0" leftmargin="0">
		<form action="" method="post" name="formulario">
			<table class="listagem" cellspacing="0" cellpadding="3" align="center" style="width: 650px;">
				<thead>
					<tr>
						<td style="text-align: center; background-color: #e0e0e0;" colspan="6">
							<b style="font-size: 10pt;">Histórico de Tramitações<br/></b>
							<div><?php echo $documento['docdsc']; ?></div>
						</td>
					</tr>
					<?php if ( count( $historico ) ) : ?>
						<tr>
							<td style="width: 20px;"><b>Seq.</b></td>
							<td style="width: 200px;"><b>Onde Estava</b></td>
							<td style="width: 200px;"><b>O que aconteceu</b></td>
							<td style="width: 90px;"><b>Quem fez</b></td>
							<td style="width: 120px;"><b>Quando fez</b></td>
							<td style="width: 17px;">&nbsp;</td>
						</tr>
					<?php endif; ?>
				</thead>
				<?php $i = 1; ?>
				<?php foreach ( $historico as $item ) : ?>
					<?php $marcado = $i % 2 == 0 ? "" : "#f7f7f7";?>
					<tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
						<td align="right"><?=$i?>.</td>
						<td style="color:#008000;">
							<?php echo $item['esddsc']; ?>
						</td>
						<td valign="middle" style="color:#133368">
							<?php echo $item['aeddscrealizada']; ?>
						</td>
						<td style="font-size: 6pt;">
							<?php echo $item['usunome']; ?>
						</td>
						<td style="color:#133368">
							<?php echo $item['htddata']; ?>
						</td>
						<td style="color:#133368; text-align: center;">
							<?php if( $item['cmddsc'] ) : ?>
								<img
									align="middle"
									style="cursor: pointer;"
									src="http://<?php echo $_SERVER['SERVER_NAME'] ?>/imagens/restricao.png"
									onclick="exebirOcultarComentario( '<?php echo $i; ?>' );"
								/>
							<?php endif; ?>
						</td>
					</tr>
					<tr id="comentario<?php echo $i; ?>" style="display: none;" bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
						<td colspan="6">
							<div >
								<?php echo htmlentities( $item['cmddsc'] ); ?>
							</div>
						</td>
					</tr>
					<?php $i++; ?>
				<?php endforeach; ?>
				<?php $marcado = $i++ % 2 == 0 ? "" : "#f7f7f7";?>
				<tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
					<td style="text-align: right;" colspan="6">
						Estado atual: <span style="color:#008000;"><?php echo $atual['esddsc']; ?></span>
					</td>
				</tr>
			</table>
			<br/>
			<div style="text-align: center;">
				<input class="botao" type="button" value="Fechar" onclick="window.close();">
			</div>
		</form>
	</body>
</html>