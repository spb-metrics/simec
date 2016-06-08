<?php  
direcionaFT();
if( $_POST['ajaxestuf'] ){	 
	header('content-type: text/html; charset=ISO-8859-1'); 
	$sql = "select
			 muncod as codigo, mundescricao as descricao 
			from
			 territorios.municipio 
			where
			 estuf = '".$_POST['ajaxestuf']."' 
			order by
			 mundescricao asc";
	echo "Município: <br>"; 
	echo $db->monta_combo( "filtro_muncod", $sql, 'S', 'Selecione...', 'filtraEstado(this.value);', '', '', '', '', 'filtro_muncod' );
	die();
}
if( $_POST['ajaxmuncod'] ){	 
	header('content-type: text/html; charset=ISO-8859-1'); 
	$sql = "select
			 estuf  
			from
			 territorios.municipio 
			where
			 muncod = '".$_POST['ajaxmuncod']."' 
			 "; 
	die($db->pegaUm($sql));
}
if( $_POST['ajaxcpf'] != '' ){
	$_SESSION['fdpcpf'] = $_POST['ajaxcpf']; 
	die();
}
if( $_POST['novaPessoa'] == 't' ){
	unset( $_SESSION['fdpcpf'] );
	die();
}
include  APPRAIZ."includes/cabecalho.inc";
monta_titulo( 'Força de Trabalho', 'Lista de Pessoas' ); 
 
?>
<br>

<head>
   <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script src="../includes/prototype.js"></script> 
    </head> 
<script type="text/javascript" src="/includes/prototype.js"></script>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
				<form action="" method="POST" name="formulario">
					<input type="hidden" name="acao" value="<?= $_REQUEST['acao'] ?>"/>
					<div style="float: left;">
						
						<table border="0" cellpadding="3" cellspacing="0">
							<tr>
								<td valign="bottom" colspan="2">
									Nome da Pessoa:
									<br/> 
									<?= campo_texto( 'filtro_fdpnome', 'N', 'S', '', 80, 200, '', '' ); ?>
								</td>
								<td>
									CPF:
									<br>
									<?php  
									echo campo_texto( 'filtro_fdpcpf', 'N', 'S', '', 17, 100, '', '', '', '', '', '', 'this.value=mascaraglobal(\'###.###.###-##\',this.value);' ); ?> 
								</td> 
								 <td valign="bottom" align="right">
									SIAPE
									<br/>
									<?php  
								   echo campo_texto( 'filtro_estuf', 'N', 'S', '', 20, 100, '', '' ); ?> 
								</td> 
							</tr>
							<tr> 
								<td valign="bottom" >
									Lotação
									<br/>
									<?  
									$sql = "SELECT 
					                            fulid AS codigo, 
					                            fuldescricao AS descricao
					                        FROM
					                            gestaopessoa.ftunidadelotacao
					                       ";  
									$db->monta_combo( "filtro_fulid", $sql, 'S', 'Selecione...', '', '' ); 
									?>	
								</td>
								<td>
									Sexo: 
									<br>
									<?php  
									$sql = "SELECT
											 'm' as codigo,
											 'Masculino' as descricao
											UNION 
											SELECT
											 'f' AS codigo,
											 'Feminino' AS descricao";
									$db->monta_combo( "filtro_fdpsexo", $sql, 'S', 'Selecione...', '', '', '', '', '', 'filtro_fdpsexo' );
									?>
								</td>
								<td id="td_estado" >
									Estado:
									<br>
									<?
					                	$sql = "SELECT 
					                            estuf  AS codigo, 
					                            estdescricao AS descricao
					                        FROM
					                            territorios.estado
					                       "; 
					                 
					                $db->monta_combo('filtro_estuf', $sql, 'S', "Selecione...", 'filtraMunicipio(this.value)', '', '', '200', 'N', 'filtro_estuf');
					                ?>	
								</td>
								<td id="td_municipio">
									Município:
									<br>
									<?
					                	$sql = "SELECT 
					                            muncod AS codigo, 
					                            mundescricao AS descricao
					                        FROM
					                            territorios.municipio
					                       "; 
					                $db->monta_combo('filtro_muncod', $sql, 'S', "Selecione...", 'filtraEstado(this.value);', '', '', '200', 'N', 'filtro_muncod');
					                ?>			
								</td>	 
							</tr>
							<tr>
								<td >
									Grau de Escolaridade:<br/>
									<?
									$sql = "SELECT 
					                			tgeid as codigo, 
					                			tgedescricao as descricao
					                			from 
					                           public.tipograuescolaridade
					                       ";  
					                $db->monta_combo('filtro_tgeid', $sql, 'S', "Selecione...", '', '', '', '200', 'N', 'filtro_tgeid');
									?>
								</td> 
								<td valign="bottom">
									Idade:
									<br/>
									<select name="filtro_idade" id="filtro_idade">
									<option value="">Selecione...</option>
									<?php 
									for( $i = 18; $i<= 70; $i++ ){?>
										 <option value="<?=$i?>"> <?=$i ?></option>
									<?}?>
									</select>
								</td>
								<td valign="bottom"  align="right" >
									  &nbsp;
								</td>	
								<td valign="bottom"  align="right">
									Vínculo com o MEC:
									<br/>
									<?php
									$perfil = arrayPerfil();
									$sql = "SELECT 
					                            fstid AS codigo, 
					                            fstdescricao AS descricao
					                        FROM
					                            gestaopessoa.ftsituacaotrabalhador 
					                       ";  
									if(   in_array(  PERFIL_SERVIDOR , $perfil ) || 
					                  	  in_array(  PERFIL_TERCEIRIZADO, $perfil ) || 
					                  	  in_array(  PERFIL_CONSULTOR , $perfil ) ){
				                        $sql.= "WHERE fstid in ( ".implode(",",controlaPefilFT('vinculosPermitidos') ).")";
					                } else{
					                	$sql.= "WHERE fstid <> 4";
					                }
					                $db->monta_combo('filtro_fstid', $sql, 'S', "Selecione...", '', '', '', '200', 'N', 'filtro_fstid');
									?>
								</td> 
							</tr>
							<tr>
								<td colspan="4"> 
								<input type="button" name="" value="Pesquisar" onclick="return validaForm();"/>		
								</td>
							</tr>
						</table> 
					</div>	
				</form>
			</td>
		</tr>
	</tbody>
</table>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:none;"  >
		<tr>
			<td style="padding: 10px;">
				<span
					style="cursor: pointer"
					onclick="cadastrarPessoa();"
					title="Nova"
				>
					<img
						align="absmiddle"
						src="/imagens/gif_inclui.gif"
					/>
					Cadastrar Pessoa
				</span>
			</td>
		</tr>
	</table>
<? 
$and = " "; 

if( $_REQUEST['filtro_ftu_fulid']){
	$and .= "AND ftu.fulid = ".$_REQUEST['filtro_ftu_fulid']." "; 
}
if( $_REQUEST['filtro_fst_fstid']){
	$and .= "AND fst.fstid = ".$_REQUEST['filtro_fst_fstid']." "; 
}
if( $_REQUEST['filtro_tfo_tfoid']){
	$and .= "AND formacao.tfoid = ".$_REQUEST['filtro_tfo_tfoid']." "; 
}
if( $_REQUEST['filtro_fti_ftiid']){
	$and .= "AND idioma.ftiid = ".$_REQUEST['filtro_fti_ftiid']." "; 
}
if( $_REQUEST['filtro_fta_ftaid']){
	$and .= "AND atividadedesenv.ftaid = ".$_REQUEST['filtro_fta_ftaid']." "; 
}
if( $_REQUEST['filtro_fta_fnaid']){
	$and .= "AND atividadedesenv.fnaid = ".$_REQUEST['filtro_fta_fnaid']." "; 
}
if( $_REQUEST['filtro_fte_fteid']){
	$and .= "AND fte.fteid = ".$_REQUEST['filtro_fte_fteid']." "; 
}
if( $_REQUEST['filtro_fte_fneid']){
	$and .= "AND fte.fneid = ".$_REQUEST['filtro_fte_fneid']." "; 
}

if( $_REQUEST['filtro_fdpnome']){
	$and .= "AND fdp.fdpnome ILIKE '%".$_REQUEST['filtro_fdpnome']."%' "; 
}
if( $_REQUEST['filtro_fdpcpf']){
	$and .= "AND fdp.fdpcpf = '".str_replace( ".", "",str_replace("-","", $_REQUEST['filtro_fdpcpf'] ) )."' "; 
}
if( $_REQUEST['filtro_estuf']){
	$and .= "AND fdp.estuf = '".$_REQUEST['filtro_estuf']."' "; 
}
if( $_REQUEST['filtro_muncod']){
	$and .= "AND fdp.muncod = '".$_REQUEST['filtro_muncod']."' "; 
}
if( $_REQUEST['filtro_fdpsexo']){
	$and .= "AND fdp.fdpsexo = '".$_REQUEST['filtro_fdpsexo']."' "; 
}
if( $_REQUEST['filtro_tgeid']){
	$and .= "AND ".$_REQUEST['filtro_tgeid']." 
			IN ( SELECT tfoid FROM gestaopessoa.ftformacaoacademica 
				 WHERE fdpcpf = fdp.fdpcpf ) "; 
}
if( $_REQUEST['filtro_idade']){
	$and .= "AND (( ".date("Y")." - to_number( to_char( fdp.fdpdatanascimento, 'yyyy') , '9999') ) = ".$_REQUEST['filtro_idade']." ) "; 
}
if( $_REQUEST['filtro_fstid']){
	$and .= "AND fdp.fstid = ".$_REQUEST['filtro_fstid']." "; 
}
if(   in_array(  PERFIL_SERVIDOR , $perfil ) || 
	in_array(  PERFIL_TERCEIRIZADO, $perfil ) || 
	in_array(  PERFIL_CONSULTOR , $perfil ) ){
		 $and_= "AND fdp.fstid in ( ".implode(",",controlaPefilFT('vinculosPermitidos') ).")";
	} 
$sql = "SELECT distinct
			'<img
 					align=\"absmiddle\"
 					src=\"/imagens/alterar.gif\"
 					style=\"cursor: pointer\"
 					onclick=\"javascript: selecionarPessoa(\''||fdp.fdpcpf||'\');\"
 					title=\"Selecionar Pessoa\">  ',
			fdp.fdpcpf,
			fdp.fdpnome,
			fdp.fdpsiape,
			fst.fstdescricao,
			ftu.fuldescricao,
			'-- ' as fdpalteradopor
		FROM 
		gestaopessoa.ftdadopessoal as fdp
		INNER JOIN gestaopessoa.ftsituacaotrabalhador AS fst ON fst.fstid = fdp.fstid 
		LEFT  JOIN gestaopessoa.ftdadofuncional AS fdt ON fdt.fdpcpf = fdp.fdpcpf
		LEFT  JOIN gestaopessoa.ftunidadelotacao as ftu ON ftu.fulid = fdt.fulid
		LEFT  JOIN gestaopessoa.ftformacaoacademica as formacao ON fdp.fdpcpf = formacao.fdpcpf
		LEFT  JOIN gestaopessoa.idioma as idioma ON fdp.fdpcpf = idioma.fdpcpf
		LEFT  JOIN gestaopessoa.ftatividadedesenvolvida as atividadedesenv ON fdp.fdpcpf = atividadedesenv.fdpcpf 
		LEFT  JOIN gestaopessoa.ftexperienciaanterior AS fte ON fdp.fdpcpf = fte.fdpcpf
		
		WHERE fdp.fdpcpf IS NOT NULL
		AND fst.fstid <> 4
		$and_ 
		$and
		";  
		
$cabecalho = array("&nbsp;&nbsp;&nbsp;&nbsp;Ação", "CPF","NOME", "SIAPE","Vínculo com o MEC","Lotação","Alterado por" ); 

//ver( $sql );

$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', ''); ?>
	
	
<script>
function validaForm(){
	document.formulario.submit();
}
function selecionarPessoa(fdpcpf){
 
	var req = new Ajax.Request('gestaopessoa.php?modulo=principal/listaPessoa&acao=A', {
						        method:     'post',
						        parameters: '&ajaxcpf=' + fdpcpf,							         
						        onComplete: function (res) {  
									window.location.href = '?modulo=principal/cadDadosPessoais&acao=A';
								}
			});
}
function cadastrarPessoa(){
	var req = new Ajax.Request('gestaopessoa.php?modulo=principal/listaPessoa&acao=A', {
						        method:     'post',
						        parameters: '&novaPessoa=t',							         
						        onComplete: function (res) {  
									window.location.href = '?modulo=principal/cadDadosPessoais&acao=A';
								}
			});
}
function filtraMunicipio(estuf){
 
	td_municipio = document.getElementById('td_municipio');
	td_estado	 = document.getElementById('td_estado');
  
	var req = new Ajax.Request('gestaopessoa.php?modulo=principal/listaPessoa&acao=A', {
							        method:     'post',
							        parameters: '&ajaxestuf=' + estuf,							         
							        onComplete: function (res)
							        {			
										td_municipio.innerHTML = res.responseText;
							        }
							        
							  });
    
}
function filtraEstado(muncod){

	td_municipio = document.getElementById('td_municipio');
	td_estado	 = document.getElementById('td_estado');
	var estuf	 = document.getElementById('filtro_estuf');
  
	var req = new Ajax.Request('gestaopessoa.php?modulo=principal/listaPessoa&acao=A', {
							        method:     'post',
							        parameters: '&ajaxmuncod=' + muncod,							         
							        onComplete: function (res)
							        {		 
										estuf.value = res.responseText;
							        }
							        
							  });
    
}
</script>