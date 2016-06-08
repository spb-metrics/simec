<?
if ($cabecalho_painel==true){
include APPRAIZ . '/www/painel/novo/rodape_painel.php';
exit();
}
/*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral
   Módulo:rodape.inc
   Finalidade: permitir o fechamento da conexão
*/

//monta link manual
$linkmanual = montaLinkManual( $_SESSION['sisid'] );

//Loga estatistica
$sql = "Insert into seguranca.estatistica (mnuid,usucpf,esttempoexec,estsession,sisid,estmemusa) VALUES(" . $_SESSION['mnuid'] . ",'" . $_SESSION['usucpforigem'] . "'," . ( getmicrotime() - $Tinicio ) . ",'" . session_id() . "'," . $_SESSION['sisid'] . ", '".number_format(memory_get_usage()/(1024*1024),2,'.','')."')";
$db->executar($sql,false);
$db->commit();


//Fecha conexões DB
$db->close();


//Roda Estatística e conta users online
//$conn = pg_connect( "host=$servidor_bd port=$porta_bd dbname=$nome_bd user=$usuario_db password=$senha_bd" );
	//pg_query($conn, "begin");
		//$sql = "Insert into seguranca.estatistica (mnuid,usucpf,esttempoexec,estsession,sisid) VALUES(" . $_SESSION['mnuid'] . ",'" . $_SESSION['usucpforigem'] . "'," . ( getmicrotime() - $Tinicio ) . ",'" . session_id() . "'," . $_SESSION['sisid'] . ")";
//		pg_query($conn, $sql);
		//$sql_usuarios_online = "select qtdonline from seguranca.qtdusuariosonline where sisid = " . $_SESSION['sisid'];
		//$rs = pg_query($conn, $sql_usuarios_online);
		//$dados = pg_fetch_array($rs);
		//$_SESSION['qtdusuariosonline'][$_SESSION['sisid']] = $dados['qtdonline'];
	//pg_query($conn, "commit;");
//pg_close();

?>
<? if ($cabecalho_sistema<>"") : ?>
					</td>
				</tr>
			</table>
		</td>
		</tr>
			<tr>
				<td>

					<?php include_once APPRAIZ . "includes/estouvivo.php"; ?>

					<script type="text/javascript" language="javascript">
						function abrirUsuariosOnline()
						{
							window.open(
								'../geral/usuarios_online.php',
								'usuariosonline',
								'height=500,width=600,scrollbars=yes,top=50,left=200'
							);
						}
						</script>
					
				</td>
			</tr>
		</table>
		
			<table class="rodape" align='center' border="0" cellspacing="0" cellpadding="2" class="notprint" style="width:100%">
						<tr>
							<td colspan="2" height="2"></td>
						</tr>
						<tr>
							<td align="left" height="15">
								Data: <?= date("d/m/Y - H:i:s") ?> /
									Último acesso (<?= formata_data( $_SESSION['usuacesso'] ); ?>) -
									<a href="javascript:abrirUsuariosOnline();" >
										<span id="rdpUsuariosOnLine"><?= $_SESSION['qtdusuariosonline'][$_SESSION['sisid']];?></span>
										Usuários On-Line
									</a>
							</td>
							<td align="right" height="15">
								
									<?php echo $GLOBALS['parametros_sistema_tela']['sigla']; ?> - <a href="javascript:janela('/geral/fale_conosco.php?uc=<?= $_SESSION['usucpf']; ?>',550,600)">Fale Conosco</a>
									<?= $linkmanual; ?>
									|
									Tx.: <?= number_format( ( getmicrotime() - $Tinicio ), 4, ',', '.' ); ?>s / <?=number_format(memory_get_usage()/(1024*1024),2,',','.');?>
								
							</td>
						</tr>
					</table>
					
		<div id="avisochat">
			<div style="padding: 0px; width: 110px; padding: 0; margin: 0; height: 100px; overflow: none; position-y: absolute;display:none;" id="avisochat_lista">
				<table border="0" cellpadding="2" cellspacing="2">
				<tr>
					<td  align="left" width="110" bgcolor="#FFF7AF" style="border-top: 1px solid #808080; border-right: 1px solid #808080;border-bottom: 1px solid #808080;">
						<span onclick="avisoChatMostrarEsconder();"><img src="/imagens/balaochat.gif" border="0" align="absmiddle"><font color="#000000"><strong>&nbsp;Mensagens</strong></font><br></span>
						<div style="overflow: auto; position-y: absolute;border-top: 1px solid #808080; height: 100px;">
							<table id="avisochat_tabela" width="100%" cellpadding="0" cellspacing="0"></table>
						</div>
					</td>
				</tr>
				</table>
			</div>
		</div>
	
		<script type="text/javascript" language="javascript">
			document.getElementById( 'aguarde' ).style.visibility = 'hidden';
			document.getElementById('aguarde').style.display = 'none';
		</script>
	</body>
	</html>
	<script language="JavaScript" src="../includes/wz_tooltip.js"></script> 
	<? if ( $ajustaTela ) : ?>
		<script type="text/javascript" language="javascript">
			window.scrollBy( 0, 112 );
		</script>
	<? endif; ?>
<? endif; ?>
<!--
<script type="text/javascript">
//var gaJsHost = (("https:" == document.location.protocol) ? "https://ssl." : "http://www.");
//document.write(unescape("%3Cscript src='" + gaJsHost + "google-analytics.com/ga.js' type='text/javascript'%3E%3C/script%3E"));
//try {
//var pageTracker = _gat._getTracker("UA-830397-2");
//pageTracker._trackPageview();
//} catch(err) {}
</script>
-->
