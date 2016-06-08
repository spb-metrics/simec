<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral
   Programador: Cristiano Cabral (e-mail: cristiano.cabral@gmail.com)
   Módulo:seleciona_unid_perfilresp.php
  
   */
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
include_once APPRAIZ . "www/obras/_funcoes.php";
include_once APPRAIZ . "www/obras/_constantes.php";

$db     = new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];
$acao   = $_REQUEST["acao"];
	
if ( !$db->testa_superuser() && !possuiPerfil(PERFIL_SAMPR) ){
	
	$sql = "SELECT
			    oo.orgid
			FROM
				obras.orgao oo
			INNER JOIN
				obras.usuarioresponsabilidade ur ON
				ur.orgid = oo.orgid
			WHERE
				oo.orgstatus = 'A' AND				
				ur.rpustatus = 'A' AND
				ur.estuf IS NULL AND
				ur.entid IS NULL AND
				ur.usucpf = '{$_SESSION["usucpf"]}'";
	$orgid = $db->pegaUm($sql);
	

	if (!$orgid){
		die('<script type="text/javascript">
				alert(\'Seu perfil não permite liberar acesso ao sistema!\');
				window.close();
			 </script>');	
	}
}

if ($acao == "A"){
	$orgid = $_REQUEST["orgresp"];
	atribuiOrgao($usucpf, $pflcod, $orgid);
}

/**
 * Função que lista os orgãos encontrados na tabela orgao.obras
 *
 */
function listaOrgao(){
	
	global $db,$pflcod;
	
	// SQL para buscar os orgãos existentes
	
	if ($db->testa_superuser() || possuiPerfil(PERFIL_SAMPR) ){		
		$orgaoExistentes = $db->carregar("
									SELECT
										orgid, orgdesc
									FROM
										obras.orgao
									WHERE
										orgstatus = 'A'");
		
	} else {
		
		$sql = "
			select
				count(*)
			from seguranca.perfilusuario
			where
				usucpf = '" . $_SESSION['usucpf'] . "' and
				pflcod = 166 ";
		
		//$disabled = 'disabled';	
		if( $db->pegaUm( $sql ) > 0 ){
			$orgaoExistentes = $db->carregar("
											SELECT DISTINCT
												orgid, 
												orgdesc
											FROM
												obras.orgao 
											WHERE
												orgstatus = 'A' 
											ORDER BY
												orgid");
		}else{
			$orgaoExistentes = $db->carregar("
											SELECT
											    DISTINCT
												o.orgid, 
												orgdesc
											FROM
												obras.orgao o
											INNER JOIN obras.usuarioresponsabilidade ur ON ur.orgid = o.orgid
											WHERE
												ur.usucpf = '{$_SESSION["usucpf"]}' AND
												-- ur.pflcod = '{$pflcod}' AND
												ur.rpustatus = 'A' AND
												orgstatus = 'A' AND
												ur.estuf IS NULL AND
												ur.entid IS NULL
											ORDER BY
												o.orgid");
		}	
		
	}
	$count = count($orgaoExistentes);
	
	// Monta as TR e TD com os orgãos
	for ($i = 0; $i < $count; $i++){
		$codigo    = $orgaoExistentes[$i]["orgid"];
		$descricao = $orgaoExistentes[$i]["orgdesc"];
		
		if (fmod($i,2) == 0){ 
			$cor = '#f4f4f4';
		} else {
			$cor='#e0e0e0';
		}
		
		echo "
			<tr bgcolor=\"".$cor."\">
				<td align=\"right\" width=\"10%\">
					<input type=\"checkbox\" {$disabled} name=\"orgid\" id=\"".$codigo."\" value=\"".$codigo."\" onclick=\"retorna('".$i."');\">
					<input type=\"hidden\" name=\"orgdsc\" value=\"".$codigo." - ".$descricao."\">
				</td>
				<td align=\"right\" style=\"color:blue;\" width=\"10%\">
					".$codigo."
				</td>
				<td>
					".$descricao."
				</td>
			</tr>";
	}
	
}

/**
 * Função que insere, atualiza ou deleta o orgão de responsabilidade do 
 * usuário no perfil informado
 *
 * @param string $usucpf
 * @param int $pflcod
 * @param string $orgid
 */
function atribuiOrgao($usucpf, $pflcod, $orgid){
	
	global $db;
	
	$data = date("Y-m-d H:i:s");
	
	// SQL que deleta a responsabilidade do usuário
	$sql = $db->executar("UPDATE 
							obras.usuarioresponsabilidade 
						  SET 
							rpustatus = 'I' 
						  WHERE 
							usucpf = '{$usucpf}' AND
							pflcod = '{$pflcod}' AND
							orgid is not null AND
							entid is null AND
							estuf is null");

	// Se existir $orgid, o sistema insere/atualiza o banco, senao, deleta o dado.
	if ( $orgid[0] ){
		foreach ($orgid as $valor){

			// SQL que insere o orgao no banco
			$sql = $db->executar("INSERT INTO
									 obras.usuarioresponsabilidade
									 (orgid, usucpf, rpudata_inc, pflcod)
								  VALUES
									 ('{$valor}', '{$usucpf}', '{$data}', '{$pflcod}')");
		}
	}
	
	
	$db->commit();
	
	echo "
		<script>
			alert('Operação realizada com sucesso!');
			window.parent.opener.location.reload();
			self.close();
		</script>";
	
}

/**
 * Função que busca o órgão que o usuário tem responsabilidade
 *
 * @param string $usucpf
 * @param int $pflcod
 */
function buscaOrgaoCadastrado($usucpf, $pflcod){
	
	global $db;
	
	$sql = "
			SELECT DISTINCT 
				o.orgid AS codigo, 
				o.orgid||' - '||o.orgdesc AS descricao 
			FROM 
				obras.usuarioresponsabilidade ur 
				INNER JOIN obras.orgao o ON ur.orgid = o.orgid 
			WHERE 
				ur.rpustatus='A' AND 
				ur.usucpf = '$usucpf' AND 
				ur.pflcod=$pflcod AND
				ur.estuf is null AND
				ur.entid is null";
	$RS = @$db->carregar($sql);
	
	if(is_array($RS)) {
		$nlinhas = count($RS)-1;
		if ($nlinhas>=0) {
			for ($i=0; $i<=$nlinhas;$i++) {
				foreach($RS[$i] as $k=>$v) ${$k}=$v;
	    		print " <option value=\"$codigo\">$descricao</option>";		
			}
		}
	}else{
		$sql = "SELECT DISTINCT 
					o.orgid AS codigo, 
					o.orgid||' - '||o.orgdesc AS descricao 
				FROM 
					obras.usuarioresponsabilidade ur 
					INNER JOIN obras.orgao o ON ur.orgid = o.orgid 
				WHERE 
					ur.rpustatus='A' AND 
					ur.usucpf = '$usucpf' AND 
					ur.pflcod=$pflcod AND
					ur.estuf is null AND
					ur.entid is null";
		
		$RS = @$db->carregar($sql);
		
		if(is_array($RS)) {
			$nlinhas = count($RS)-1;
			if ($nlinhas>=0) {
				for ($i=0; $i<=$nlinhas;$i++) {
					foreach($RS[$i] as $k=>$v) ${$k}=$v;
					print " <option value=\"$codigo\">$codigo - $descricao</option>";
				}
			}
		} else {
			print '<option value="">Clique no Orgão para selecionar.</option>';
		}
	}
}

?>

<html>
	<head>
		<meta http-equiv="Pragma" content="no-cache">
		<title>Tipo de Estabelecimento</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
		<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
	</head>
	<body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff">
		<div align=center id="aguarde"><img src="../imagens/icon-aguarde.gif" border="0" align="absmiddle">
			<font color=blue size="2">Aguarde! Carregando Dados...</font>
		</div>
		<?//flush();?>
		
		<!-- Lista de Orgãos -->
		<div style="overflow:auto; width:496px; height:350px; border:2px solid #ececec; background-color: #ffffff;">
			<form name="formulario">
				<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="tabela">
					<script language="JavaScript">
						document.getElementById('tabela').style.visibility = "hidden";
						document.getElementById('tabela').style.display  = "none";
					</script>
					<thead>
						<tr>
							<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3"><strong>Selecione o Tipo de estabelecimento</strong></td>		
						</tr>
					</thead>
					<?php listaOrgao(); ?>
				</table>
			</form>
		</div>
		
		<!-- Orgãos Selecionados -->
		<form name="formassocia" action="?acao=A" method="post">
			<input type="hidden" name="usucpf" value="<?=$usucpf?>">
			<input type="hidden" name="pflcod" value="<?=$pflcod?>"> 
			<select multiple size="8" name="orgresp[]" id="orgresp" style="width:500px;" class="CampoEstilo" onchange="//moveto(this);">			
				<?php buscaOrgaoCadastrado($usucpf, $pflcod); ?>
			</select>
		</form>
		
		<!-- Submit do Formulário -->
		<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<tr bgcolor="#c0c0c0">
				<td align="right" style="padding:3px;" colspan="3">
					<input type="Button" name="ok" value="OK" onclick="selectAllOptions(campoSelect);verificarQtd();" id="ok">
				</td>
			</tr>
		</table>
	</body>
</html>

<script language="JavaScript">
	document.getElementById('aguarde').style.visibility = "hidden";
	document.getElementById('aguarde').style.display  = "none";
	document.getElementById('tabela').style.visibility = "visible";
	document.getElementById('tabela').style.display  = "";
	
	
	var campoSelect = document.getElementById("orgresp");
	
	
	if (campoSelect.options[0].value != ''){
		for(var i=0; i<campoSelect.options.length; i++)
			{document.getElementById(campoSelect.options[i].value).checked = true;}
	}
	
	
	
	function abreconteudo(objeto)
	{
	if (document.getElementById('img'+objeto).name=='+')
		{
		document.getElementById('img'+objeto).name='-';
	    document.getElementById('img'+objeto).src = document.getElementById('img'+objeto).src.replace('mais.gif', 'menos.gif');
		document.getElementById(objeto).style.visibility = "visible";
		document.getElementById(objeto).style.display  = "";
		}
		else
		{
		document.getElementById('img'+objeto).name='+';
	    document.getElementById('img'+objeto).src = document.getElementById('img'+objeto).src.replace('menos.gif', 'mais.gif');
		document.getElementById(objeto).style.visibility = "hidden";
		document.getElementById(objeto).style.display  = "none";
		}
	}
	
	
	
	function retorna(objeto)
	{

		tamanho = campoSelect.options.length;
		
		if (campoSelect.options[0].value=='') {tamanho--;}
		if (document.getElementsByName('orgid')[objeto].checked == true){
			campoSelect.options[tamanho] = new Option(document.getElementsByName('orgdsc')[objeto].value, document.getElementsByName('orgid')[objeto].value, false, false);
			sortSelect(campoSelect);
		}
		else {
			for(var i=0; i<=campoSelect.length-1; i++){
				if (document.getElementsByName('orgid')[objeto].value == campoSelect.options[i].value)
					{campoSelect.options[i] = null;}
			}
			if (!campoSelect.options[0]){
				campoSelect.options[0] = new Option('Clique na Órgão', '', false, false);
			}
				sortSelect(campoSelect);
		}
	}
	
	function moveto(obj) {
		if (obj.options[0].value != '') {
			if(document.getElementById('img'+obj.value.slice(0,obj.value.indexOf('.'))).name=='+'){
				abreconteudo(obj.value.slice(0,obj.value.indexOf('.')));
			}
			document.getElementById(obj.value).focus();}
	}

function verificarQtd(){
	document.formassocia.submit();
}

</script>
