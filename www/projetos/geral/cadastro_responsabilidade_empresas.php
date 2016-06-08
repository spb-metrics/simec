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

if ($_REQUEST["empresasresp"]){
	$empresasresp = $_REQUEST["empresasresp"];
	atribuiEmpresas($usucpf, $pflcod, $empresasresp);
}

/**
 * Função que lista os hospitais
 *
 */
function listaEmpresas(){
	$db = new cls_banco();
	
	// SQL para buscar estados existentes
	$empresasExistentes = $db->carregar("SELECT distinct
											ent.entid,
											ent.entnome
										  FROM entidade.entidade ent 
										  INNER JOIN projetos.checklistentidade chk ON chk.entid = ent.entid
										  INNER JOIN entidade.funcaoentidade ef ON ef.entid = ent.entid 
										  WHERE ef.funid IN(90,91,92)
										  ORDER BY ent.entnome");
	$count = count($empresasExistentes);

	// Monta as TR e TD com as unidades
	for ($i = 0; $i < $count; $i++){
		$codigo    = $empresasExistentes[$i]["entid"];
		$descricao = $empresasExistentes[$i]["entnome"];
		
		if (fmod($i,2) == 0){ 
			$cor = '#f4f4f4';
		} else {
			$cor='#e0e0e0';
		}
		
		echo "
			<tr bgcolor=\"".$cor."\">
				<td align=\"right\" width=\"10%\">
					<input type=\"Checkbox\" name=\"entid\" id=\"".$codigo."\" value=\"".$codigo."\" onclick=\"retorna('".$i."');\">
					<input type=\"hidden\" name=\"entnome\" value=\"".$codigo." - ".$descricao."\">
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

function atribuiEmpresas($usucpf, $pflcod, $empresa){
	$db = new cls_banco();
	
	$data = date("Y-m-d H:i:s");
	
	$db->executar("UPDATE projetos.usuarioresponsabilidade SET rpustatus = 'I' WHERE usucpf = '". $usucpf ."' AND pflcod = '". $pflcod ."'");
	if ($empresa[0]){
		foreach($empresa as $tipo) {
			$dadosur = $db->carregar("SELECT * FROM projetos.usuarioresponsabilidade WHERE usucpf = '". $usucpf ."' AND pflcod = '". $pflcod ."' AND entid = '". $tipo ."'");
			if($dadosur) {
				// Se existir registro atualizar para ativo
				$db->carregar("UPDATE projetos.usuarioresponsabilidade
   							   SET rpustatus = 'A', rpudata_inc= NOW()
 							   WHERE usucpf = '". $usucpf ."' AND pflcod = '". $pflcod ."' AND entid = '". $tipo ."'");
			} else {
				// Se não existir, inserir novo
				$db->executar("INSERT INTO projetos.usuarioresponsabilidade(
            				   pflcod, usucpf, entid, rpustatus, rpudata_inc)
    						   VALUES ('". $pflcod ."', '". $usucpf ."', '". $tipo ."', 'A', NOW());");
			}
		}
	}
	$db->commit();
	
	echo '<script>
			alert(\'Operação realizada com sucesso!\');
			window.parent.opener.location.reload();
			self.close();
		  </script>';
	
}

function buscaEmpresasAtribuido($usucpf, $pflcod){
	
	$db = new cls_banco();
	
	$sql = "SELECT DISTINCT 
			ent.entid AS codigo, 
			ent.entnome AS descricao
			FROM projetos.usuarioresponsabilidade ur 
			INNER JOIN entidade.entidade ent ON ur.entid = ent.entid 
			LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
			WHERE ur.rpustatus = 'A' AND ur.usucpf = '$usucpf' AND ur.pflcod = $pflcod";
	
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
		print '<option value="">Clique no estado selecionar.</option>';
	}
}

?>

<?flush();?>
<html>
	<head>
		<meta http-equiv="Pragma" content="no-cache">
		<title>Empresas</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
		<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
	</head>
	<body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff">
		<!-- Lista de Estados -->
		<div style="overflow:auto; width:496px; height:350px; border:2px solid #ececec; background-color: #ffffff;">
			<form name="formulario">
				<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="tabela">
					<thead>
						<tr>
							<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="4"><strong>Selecione o tipo de ensino</strong></td>		
						</tr>
					</thead>
					<?php listaEmpresas(); ?>
				</table>
			</form>
		</div>
		
		<!-- Estados Selecionadas -->
		<form name="formassocia" action="cadastro_responsabilidade_empresas.php" method="post">
			<input type="hidden" name="usucpf" value="<?=$usucpf?>">
			<input type="hidden" name="pflcod" value="<?=$pflcod?>">
			<select multiple size="8" name="empresasresp[]" id="empresp" style="width:500px;" class="CampoEstilo">
				<?php 
					buscaEmpresasAtribuido($usucpf, $pflcod);
				?>
			</select>
		</form>
		
		<!-- Submit do Formulário -->
		<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<tr bgcolor="#c0c0c0">
				<td align="right" style="padding:3px;" colspan="3">
					<input type="Button" name="ok" value="OK" onclick="selectAllOptions(campoSelect);document.formassocia.submit();" id="ok">
				</td>
			</tr>
		</table>
	</body>
</html>

<script language="JavaScript">

var campoSelect = document.getElementById("empresp");


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
	var entid;
	var entnome;
	if(!document.formulario.entid[objeto]) {
		entid = document.formulario.entid; 
		entnome = document.formulario.entnome;
	} else {
		entid = document.formulario.entid[objeto];
		entnome = document.formulario.entnome[objeto];
	}
	if (entid.checked == true){
		campoSelect.options[tamanho] = new Option(entnome.value, entid.value, false, false);
		sortSelect(campoSelect);
	}
	else {
		for(var i=0; i<=campoSelect.length-1; i++){
			if (entid.value == campoSelect.options[i].value)
				{campoSelect.options[i] = null;}
			}
			if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Clique na Empresa.', '', false, false);}
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

</script>