<?php 
if( $_POST['ajaxcpf'] != '' ){
	$_SESSION['boautoavaliacao'] = false;
	$_SESSION['cpfavaliado'] = $_POST['ajaxcpf']; 
	$_SESSION['autoavalchefe'] = false;
	die();
}elseif( $_POST['ajaxcpfautoaval'] != '' ){
	$_SESSION['boautoavaliacao'] = true;
	$_SESSION['cpfavaliado'] = $_POST['ajaxcpfautoaval']; 
	$_SESSION['autoavalchefe'] = true;
	die();
}
 
$_SESSION['boautoavaliacao'] = false;
$_SESSION['autoavalchefe'] = false;
direcionaAvaliador($_SESSION['usucpf']);
include  APPRAIZ."includes/cabecalho.inc";
echo '<br>';
monta_titulo( 'Avaliação - GDPGPE', 'Lista de Pessoas' ); 

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
									<?= campo_texto( 'filtro_nome', 'N', 'S', '', 60, 200, '', '' ); ?>
								</td>
								<td  valign="bottom" >
								 	CPF:
									<br/> 
									<?= campo_texto( 'filtro_cpf', 'N', 'S', '', 20, 200, '', '' ); ?>
								</td>
								<td  valign="bottom" >
								 	SIAPE:
									<br/> 
									<?= campo_texto( 'filtro_siape', 'N', 'S', '', 30, 200, '', '' ); ?>
								</td>
								<td valign="bottom" >
									<input type="button" name="" value="Pesquisar" onclick="return validaForm();"/>
								</td>
							</tr> 
							<!-- <tr>
								<td colspan="4"> <input type="checkbox" name="filtro_auto_aval" value="<?=TIPO_AUTO_AVAL;?>"/> Auto-avaliação Finalizada</td>
								
							</tr> -->	
							<tr>
								<td colspan="4">
									<input type="checkbox" name="filtro_aval_superior" value="<?=TIPO_AVAL_SUPERIOR;?>"/>Avaliação Superior Finalizada
								</td>
							</tr>
							<!-- <tr> 
								<td colspan="4"> <input type="checkbox" name="filtro_consenso" value="<?=TIPO_CONSENSO;?>"/>Consenso (Avaliação Finalizada)</td>
							</tr> -->
						</table>
					</div>
				</form>
			</td>
		</tr>
	</tbody>
</table>	
<?
$perfis = arrayPerfil();
$where2 = array();
if( !controlaPermissao( 'lista_completa' ) ){
		$where =" AND ( s.sercpfchefe = '".$_SESSION['usucpf']."' OR s.sercpf = '".$_SESSION['usucpf']."' ) ";
}
if( $_POST['filtro_nome']){
	$where .= " AND s.sernome ILIKE '%".$_POST['filtro_nome']."%' ";
}
if( $_POST['filtro_cpf']){
	$where .= "AND u.usucpf ILIKE '%".$_POST['filtro_cpf']."%' ";
}
if( $_POST['filtro_siape']){
	$where .= "AND s.sersiape ILIKE '%".$_POST['filtro_siape']."%' ";
}
if( $_REQUEST['filtro_media'] == 't'){
	$where .= " AND s.sermediaconsenso = 't'";
}


if( $_REQUEST['filtro_auto_aval']){
	array_push($where2, "autoaval IS NOT NULL");
}
if( $_REQUEST['filtro_aval_superior']){
	array_push($where2, "avalsuperior IS NOT NULL");
}
if( $_REQUEST['filtro_consenso']){
	array_push($where2, "consenso IS NOT NULL");
}
$perfil = arrayPerfil();
$super = array(Array(PERFIL_ADMINISTRADOR),Array(PERFIL_SUPER_USER));

$acao = "CASE WHEN s.sercpf = '".$_SESSION['usucpf']."'
			THEN '<img
					align=\"absmiddle\"
					src=\"/imagens/alterar.gif\"
					style=\"cursor: pointer\"
					onclick=\"javascript: selecionarPessoaAutoAval(\''|| cast( s.sercpf as character varying(11) ) ||'\' );\"
					title=\"Selecionar Pessoa\"> ' 
			ELSE '<img
					align=\"absmiddle\"
					src=\"/imagens/alterar.gif\"
					style=\"cursor: pointer\"
					onclick=\"javascript: selecionarPessoa(\''|| cast( s.sercpf as character varying(11) ) ||'\' );\"
					title=\"Selecionar Pessoa\"> ' 
			END ";
if( !in_array($perfil,$super) && (Date("c") > '2010-11-30T23:59:59-02:00') ){
	$acao = "' '";
}
$sql = "select 
		acao, 
		sercpf, 
		sernome, 
		sersiape, 
		avalsuperior from (
			SELECT 
			{$acao} AS acao ,  
			cast( s.sercpf as character varying(11) ), 
			s.sernome, 
			s.sersiape,
		    (SELECT 
				ROUND(AVG(resnota))					
				FROM gestaopessoa.respostaavaliacao 
				WHERE sercpf = u.usucpf 
				AND tavid = ".TIPO_AUTO_AVAL."
				AND resavaliacaopendente = 'f'
				AND resano = {$_SESSION['exercicio']} )as autoaval ,
			(SELECT 
				ROUND(AVG(resnota))
				FROM gestaopessoa.respostaavaliacao 
				WHERE sercpf = u.usucpf 
				AND tavid = ".TIPO_AVAL_SUPERIOR."
				AND resavaliacaopendente = 'f'
				AND resano = {$_SESSION['exercicio']} ) as avalsuperior,
			(SELECT 
				ROUND(AVG(resnota))
				FROM gestaopessoa.respostaavaliacao 
				WHERE sercpf = u.usucpf 
				AND tavid = ".TIPO_AVAL_CONSENSO." 
				AND resavaliacaopendente = 'f'
				AND resano = {$_SESSION['exercicio']} ) as consenso  
		FROM gestaopessoa.servidor as s
	    LEFT JOIN seguranca.usuario as u on u.usucpf = s.sercpf
	    								  AND s.seranoreferencia = '{$_SESSION['exercicio']}'
	    WHERE s.sercpf IS NOT NULL
 		--AND s.tssid IN (".SITUACAO_ATIVO_PERMANENTE.", 
 	    				--".SITUACAO_CEDIDO.", 
 	    				--".SITUACAO_EXCEDENTE.", 
 	    				--".SITUACAO_ATIVO_PERM_L.", 
  	    				--".SITUACAO_ANISTIADO.", 
 	    				--".SITUACAO_EXERC.") 
 	    				
 		AND s.tssid NOT IN (".REQUISITADO.", 
 	    					".NOMEADO_CARGO_COMIS.", 
 	    					".REQ_DE_OUTROS_ORGAOS.", 
 	    					".EXERC_DESCENT_CARREI.") 
 	    AND s.sercpf <> '".$_SESSION['usucpf']."'
 	    AND s.seranoreferencia = '{$_SESSION['exercicio']}'
	    $where
	    )AS tbl1
	    ".(!empty($where2) ? ' WHERE '.implode( ' and ', $where2 ) : '');
	  
//$cabecalho = array("Ação","CPF","Nome", "SIAPE", "Auto-avaliação", "Aval.Superior", "Consenso" );
$cabecalho = array("Ação","CPF","Nome", "SIAPE", "Aval.Superior", );
$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', ''); 
    
?>
<script type="text/javascript">
function validaForm(){
	document.formulario.submit();
}
function selecionarPessoa(cpf){
	var req = new Ajax.Request('gestaopessoa.php?modulo=principal/listaPessoalAvaliacao&acao=A', {
						        method:     'post',
						        parameters: '&ajaxcpf=' + cpf,							         
						        onComplete: function (res) {  
									window.location.href = '?modulo=principal/formularioAvaliacao&acao=A';
								}
			});
}
function selecionarPessoaAutoAval(cpf){
	var req = new Ajax.Request('gestaopessoa.php?modulo=principal/listaPessoalAvaliacao&acao=A', {
						        method:     'post',
						        parameters: '&ajaxcpfautoaval=' + cpf,							         
						        onComplete: function (res) {  
									window.location.href = '?modulo=principal/formularioAvaliacao&acao=A';
								}
			});
}
 
</script>