<?php  
include  APPRAIZ."includes/cabecalho.inc"; 
echo '<br>';
monta_titulo( 'Avaliação', 'Administrar Chefia' );  
?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script src="../includes/prototype.js"></script>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
				<form action="" method="POST" name="formulario">
					<input type="hidden" name="acao" value="<?= $_REQUEST['acao'] ?>"/>
					<div style="float: left;">
						
						<table border="0" cellpadding="3" cellspacing="0">
							<tr>
								<td valign="bottom" >
									Nome da Pessoa:
									<br/> 
									<?= campo_texto( 'filtro_nome', 'N', 'S', '', 50, 200, '', '' ); ?>
								</td>
								<td  valign="bottom" >
								 	CPF:
									<br/> 
									<?= campo_texto( 'filtro_cpf', 'N', 'S', '', 50, 200, '', '' ); ?>
								</td>
								<td  valign="bottom" >
								 	SIAPE:
									<br/> 
									<?= campo_texto( 'filtro_siape', 'N', 'S', '', 50, 200, '', '' ); ?>
								</td>
								<td valign="bottom" >
									<input type="button" name="" value="Pesquisar" onclick="return validaForm();"/>
								</td>
							</tr> 
							<tr>
								<td>Situação</br><? $sql = "select
											 tssid as codigo, tssdescricao as descricao 
											from
											 gestaopessoa.tiposituacaoservidor
											order by
											 tssdescricao asc";
									$db->monta_combo( "filtro_situacao", $sql, 'S', 'Selecione...', '', '' );
									?>
								</td>
								<td colspan="2">Lotação:</br>
								<? $sql = "SELECT 
												tlsid as codigo,
												tlsdescricao as descricao
											FROM 
												gestaopessoa.tipolotacaoservidor 
											order by
												descricao ASC";
									$db->monta_combo( "filtro_lotacao", $sql, 'S', 'Selecione...', '', '', '', 269 );
									?>
								</td>
							</tr>
						</table>
					</div>
				</form>
			</td>
		</tr>
	</tbody>
</table>	
<?

if( $_POST['filtro_nome']){
	$where = " AND s.sernome ILIKE '%".$_POST['filtro_nome']."%' ";
}
if( $_POST['filtro_cpf']){
	$where .= "AND u.usucpf ILIKE '%".$_POST['filtro_cpf']."%' ";
}
if( $_POST['filtro_siape']){
	$where .= "AND s.sersiape ILIKE '%".$_POST['filtro_siape']."%' ";
}
if( $_POST['filtro_lotacao']){
	$where .= "AND s.tlsid =".$_POST['filtro_lotacao'];
}
if( $_POST['filtro_situacao']){
	$where .= "AND s.tssid =".$_POST['filtro_situacao'];
}
//$sql = "SELECT 
//			cast( s.sercpf as character varying(11) ), s.sernome, s.sersiape , 
//			tl.tlsdescricao,
//			ts.tssdescricao,
//			s2.sercpf,
//			CASE 
//				WHEN s.sercpfchefe IS NOT NULL 
//				THEN
//					'<a border=0 id=\"chefe['||s.sercpf||']\" href=\"javascript: abrirPopupChefes(\''||s.sercpf||'\');\"><img border=0 src=\"/imagens/alterar.gif\" onclick=\"abrirPopupChefes( \''||s.sercpf||'\' );\" style=\"cursor: pointer\">&nbsp;'||(SELECT sernome FROM gestaopessoa.servidor WHERE sercpf = s.sercpfchefe ) || '</img></a>'
//				WHEN s.sercpfchefe IS NULL 
//				THEN
//				   '<a border=0 id=\"chefe['||s.sercpf||']\" href=\"javascript: abrirPopupChefes(\''||s.sercpf||'\');\"><img src=\"/imagens/gif_inclui.gif\" border=0 onclick=\"abrirPopupChefes( \''||s.sercpf||'\' );\" style=\"cursor: pointer\">&nbsp;Adicionar Chefe</img> </a>'
//				WHEN s2.sercpf IS NULL
//				THEN
//				   '<a border=0 id=\"chefe['||s.sercpf||']\" href=\"javascript: abrirPopupChefes(\''||s.sercpf||'\');\"><img src=\"/imagens/gif_inclui.gif\" border=0 onclick=\"abrirPopupChefes( \''||s.sercpf||'\' );\" style=\"cursor: pointer\">&nbsp;Adicionar Chefe</img> </a>'
//			END as chefe
//				from gestaopessoa.servidor as s
//				LEFT JOIN gestaopessoa.servidor AS s2 ON s2.sercpf = s.sercpfchefe
//	    left JOIN  seguranca.usuario as u on u.usucpf = s.sercpf
//	    INNER JOIN  gestaopessoa.tipolotacaoservidor AS tl ON tl.tlsid = s.tlsid
//	    INNER JOIN  gestaopessoa.tiposituacaoservidor AS ts ON ts.tssid = s.tssid
//	    
//	    WHERE s.sercpf IS NOT NULL
//	    $where
//	    "; 
//	    ver( $sql, d);
$sql = "SELECT 
			sercpf, 
			sernome, 
			sersiape, 
			tlsdescricao,
			tssdescricao,  
			CASE 
				WHEN chefe IS NULL THEN '<a border=0 id=\"chefe['||sercpf||']\"><img src=\"/imagens/gif_inclui.gif\" border=0 onclick=\"abrirPopupChefes( \''||sercpf||'\' );\" style=\"cursor: pointer\">&nbsp;Adicionar Chefe</img> </a>'
				ELSE chefe 
			END AS chefe2 FROM (
					SELECT 
						cast( s.sercpf as character varying(11) ), 
						s.sernome, s.sersiape , 
						tl.tlsdescricao,
						ts.tssdescricao, 
						s.sercpfchefe,
						CASE 
							WHEN s.sercpfchefe IS NOT NULL 
							THEN 
									'<a border=0 id=\"chefe['||s.sercpf||']\"><img border=0 src=\"/imagens/alterar.gif\" onclick=\"abrirPopupChefes( \''||s.sercpf||'\' );\" style=\"cursor: pointer\">&nbsp;'||( 
									 SELECT DISTINCT sernome
									 FROM gestaopessoa.servidor 
									 WHERE sercpf = s.sercpfchefe
									 AND seranoreferencia = {$_SESSION['exercicio']} ) 
							ELSE
							   '<a border=0 id=\"chefe['||s.sercpf||']\"><img src=\"/imagens/gif_inclui.gif\" border=0 onclick=\"abrirPopupChefes( \''||s.sercpf||'\' );\" style=\"cursor: pointer\">&nbsp;Adicionar Chefe</img> </a>'
						END as chefe
							FROM gestaopessoa.servidor as s
							    LEFT JOIN gestaopessoa.servidor AS s2 ON s2.sercpf = s.sercpfchefe
							    									  AND s2.seranoreferencia = {$_SESSION['exercicio']}
							    LEFT JOIN  seguranca.usuario as u on u.usucpf = s.sercpf
							    INNER JOIN  gestaopessoa.tipolotacaoservidor AS tl ON tl.tlsid = s.tlsid
							    INNER JOIN  gestaopessoa.tiposituacaoservidor AS ts ON ts.tssid = s.tssid			    
				    		WHERE s.sercpf IS NOT NULL AND s.seranoreferencia = {$_SESSION['exercicio']}
				    		AND s.tssid IN (".SITUACAO_ATIVO_PERMANENTE.", 
					 	    				".SITUACAO_CEDIDO.", 
					 	    				".SITUACAO_EXCEDENTE.", 
					 	    				".SITUACAO_ATIVO_PERM_L.", 
					  	    				".SITUACAO_ANISTIADO.", 
					 	    				".SITUACAO_EXERC.") 
 	    				
					 		AND s.tssid NOT IN (".REQUISITADO.", 
					 	    					".NOMEADO_CARGO_COMIS.", 
					 	    					".REQ_DE_OUTROS_ORGAOS.", 
					 	    					".EXERC_DESCENT_CARREI.")
						     $where
					) AS foo";
$cabecalho = array( "CPF","Nome", "SIAPE", "Lotação", "Situação", "Chefe" ); 
$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', ''); 

?>
<script type="text/javascript">
function validaForm(){
	document.formulario.submit();
}

function abrirPopupChefes(id){
	var url = 'gestaopessoa.php?modulo=principal/popupChefes&acao=A&idPessoa='+id;
	var janela = window.open( url, 'PopupDeChefes', 'width=650,height=480,scrollbars=yes,scrolling=yes,resizebled=yes' );
	janela.focus();
//	var nome_janela = 'Popup de Chefes';
//	window.open('gestaopessoa.php?modulo=principal/popupChefes&acao=A&idPessoa='+id,nome_janela,'width=650,height=480,scrollbars=yes,scrolling=yes,resizebled=yes');
}
</script>