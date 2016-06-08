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

$db     = new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];
$acao   = $_REQUEST["acao"];
$gravar = $_REQUEST['gravar'];

if ($gravar == "1"){
	$estuf = $_REQUEST["estresp"];
	atribuiEstado( $usucpf, $pflcod, $estuf );
}

function recuperaEstado ($estuf = null){
	global $db;	
	
	$sql = "SELECT
				estdescricao
			FROM
				territorios.estado
			WHERE
				estuf = '{$estuf}'";
	
	return $dsc = $db->pegaUm($sql);
}
/**
 * Função que lista as uf's
 *
 */
function listaEstados(){
	
	global $usucpf, $pflcod, $db;	

	// SQL para buscar estados existentes
	$estadosExistentes = $db->carregar(
								"SELECT
									estuf, estdescricao
								 FROM 
								 	territorios.estado
								ORDER BY 
									estuf, estdescricao");
	
	$count = count($estadosExistentes);

	//$orgdsc = recuperaOrgao($orgid);	
	
	// Monta as TR e TD com as unidades
	for ($i = 0; $i < $count; $i++){
		
		$codigo    = $estadosExistentes[$i]["estuf"];
		$descricao = $estadosExistentes[$i]["estdescricao"];
		
		$sql = "SELECT 
					rpuid 
				FROM 
					obras.usuarioresponsabilidade 
				WHERE 
					estuf = '{$codigo}' AND 
					pflcod = {$pflcod} AND 
					usucpf = '{$usucpf}' AND 
					rpustatus = 'A'";
	
		$checked = $db->pegaUm( $sql );
		$checado = $checked ? "checked" : "";
		
		$cor = ( fmod($i,2) == 0 ) ? '#f4f4f4' : '#e0e0e0'; 
				
		echo "
			<tr bgcolor=\"".$cor."\">
				<td align=\"right\" width=\"10%\">
					<input type=\"Checkbox\" name=\"estuf\" id=\"".$codigo."\" value=\"".$codigo."\" onclick=\"retorna('".$i."');\" {$checado}>
					<input type=\"hidden\" name=\"estdescricao\" value=\"$codigo - $descricao\">
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
 * Função que atribui a responsabilidade de uma uf ao usuário
 *
 * @param string $usucpf
 * @param int $pflcod
 * @param string $estuf
 */
function atribuiEstado( $usucpf, $pflcod, $estuf ){	
	
	global $db;
	
	$data = date("Y-m-d H:i:s");
	
	// Altera o status para I
	$sql_limpa = $db->executar("UPDATE 
								 	obras.usuarioresponsabilidade 
								SET 
									rpustatus = 'I' 
								WHERE 
									usucpf = '{$usucpf}' AND 
									pflcod = '{$pflcod}' AND 
									estuf IS NOT NULL");
	
	
	if (is_array($estuf) && $estuf[0]){
	
		$count = count($estuf);
		
		for ($i = 0; $i < $count; $i++){
			
			$sql_insere = $db->carregar("INSERT INTO obras.usuarioresponsabilidade (
										  estuf,
										  usucpf, 
										  rpustatus, 
										  rpudata_inc, 
										  pflcod
									    )VALUES(
									      '{$estuf[$i]}',
									      '{$usucpf}', 
									      'A', 
									      '{$data}', 
									      '{$pflcod}' 
									    )");
					
		}
	
	}

	$db->commit();
	
	echo "
		<script>
			alert('Operação realizada com sucesso!');
			window.parent.opener.location.href = window.opener.location;
			self.close();
		</script>";
	
}

function buscaEstadoAtribuido($usucpf, $pflcod){
	
	global $db;

	$sql = "SELECT DISTINCT 
				u.estuf AS codigo, 
				u.estuf||' - '||u.estdescricao AS descricao
			FROM
				obras.usuarioresponsabilidade ur  	  
			INNER JOIN 
				territorios.estado u ON ur.estuf = u.estuf
			WHERE 
				ur.rpustatus = 'A' AND
				ur.usucpf = '$usucpf' AND 
				ur.pflcod = $pflcod;";

	$RS = $db->carregar($sql);

	if(is_array($RS)) {
		$nlinhas = count($RS)-1;
		if ($nlinhas>=0) {
			for ($i=0; $i<=$nlinhas;$i++) {
				
				foreach($RS[$i] as $k => $v) ${$k} = $v;
				$option .= " <option value=\"$codigo\">$descricao</option>";
	    				
			}
		}
	}
	
	return $option;
	
}

?>


<html>
	<head>
		<meta http-equiv="Pragma" content="no-cache">

		<title>Estados</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
		<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
	</head>
	<body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff">
		
	<form name="formulario">
		
		<!-- Lista de Estados -->
		<div id="tabela" style="overflow:auto; width:496px; height:300px; border:2px solid #ececec; background-color: #ffffff;">
				<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
					<thead>
						<tr>
							<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3"><strong>Selecione o Estado</strong></td>		
						</tr>
					</thead>
					
					<?php listaEstados(); ?>
					
				</table>
		</div>		
		<!-- Estados Selecionadas -->
			<input type="hidden" name="usucpf" value="<?=$usucpf?>">
			<input type="hidden" name="pflcod" value="<?=$pflcod?>">
		    <select multiple size="8" name="estresp[]" id="estresp" style="width:500px;" class="CampoEstilo" onchange="//moveto(this);">		
					<?=buscaEstadoAtribuido($usucpf, $pflcod);?>
			</select>		
		<!-- Submit do Formulário -->
		<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<tr bgcolor="#c0c0c0">
				<td align="right" style="padding:3px;" colspan="3">
					<input type="hidden" name="gravar" id="gravar" value="">
					<input type="Button" name="ok" value="OK" onclick="document.getElementById('gravar').value=1; enviaForm();" id="ok">
				</td>
			</tr>
		</table>
	</form>		
	</body>
</html>

<script language="JavaScript">

var campoSelect = document.getElementById("estresp");

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
	
	if (document.formulario.estuf[objeto].checked == true){
		campoSelect.options[tamanho] = new Option(document.formulario.estdescricao[objeto].value, document.formulario.estuf[objeto].value, false, false);
		sortSelect(campoSelect);
	}
	else {
		for(var i=0; i<=campoSelect.length-1; i++){
			if (document.formulario.estuf[objeto].value == campoSelect.options[i].value)
				{campoSelect.options[i] = null;}
			}
			//if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Clique no Estado.', '', false, false);}
			//sortSelect(campoSelect);
	}
}

function moveto(obj) {
	if (obj.options[0].value != '') {
		if(document.getElementById('img'+obj.value.slice(0,obj.value.indexOf('.'))).name=='+'){
			abreconteudo(obj.value.slice(0,obj.value.indexOf('.')));
		}
		document.getElementById(obj.value).focus();}
}

function enviaForm(){
	selectAllOptions(campoSelect);
	document.formulario.submit();
}

</script>