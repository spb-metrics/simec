<?php
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); // Date in the past

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

// carrega as funções do módulo
include APPRAIZ."www/obras/_constantes.php";
include APPRAIZ."www/obras/_funcoes.php";
include APPRAIZ."www/obras/_componentes.php";

$db     = new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];
$acao   = $_REQUEST["acao"];
$orgid  = $_REQUEST['orgid'];
$estuf  = $_REQUEST['estuf'];
$muncod = $_REQUEST['muncod'];
$gravar = $_REQUEST['gravar'];
$unicod = $_REQUEST["uniresp"];

if ( !$db->testa_superuser() && !possuiPerfil(PERFIL_SAMPR) && !$orgid ){
	$sql = "	SELECT
				    oo.orgid
				FROM
					obras.orgao oo
				JOIN obras.usuarioresponsabilidade ur ON ur.orgid = oo.orgid
														 AND ur.rpustatus = 'A'
														 AND ur.estuf IS NULL 
														 AND ur.entid IS NULL 
														 AND ur.usucpf = '{$_SESSION["usucpf"]}'
				WHERE
					orgstatus = 'A'
			UNION
				SELECT
					DISTINCT
				    oi.orgid
				FROM
					obras.obrainfraestrutura oi
				JOIN obras.usuarioresponsabilidade ur ON ur.entid = oi.entidunidade
														 AND ur.orgid IS NULL
														 AND ur.rpustatus = 'A'
														 AND ur.usucpf = '{$_SESSION["usucpf"]}'
				WHERE
					obsstatus = 'A'";
	$orgid = $db->pegaUm($sql);
	
	if (!$orgid){
		$sql = "";
		
		
		die('<script type="text/javascript">
				alert(\'Seu perfil não permite liberar acesso ao sistema!\');
				window.close();
			 </script>');	
	}
}

//if ($orgid == 1){
//	$funid = '12';	
//}elseif ($orgid == 2){
//	$funid = '11,14';	
//} elseif($orgid == 5) {
//	$funid = '16';
//}elseif ($muncod){
//	$funid = '1,3,7';
//}else{
////	$funid = '1,6,43,3,44,42';
//	$funid = '1, 3, 6, 7, 11, 12, 14, 34, 43, 42, 44';	
//}

if ($_POST && $gravar == 1){
	atribuiUnidade($usucpf, $pflcod, $unicod);
}

function recuperaOrgao ($orgid = null){
	global $db;
	
	if ($db->testa_superuser() || possuiPerfil(PERFIL_SAMPR) || $orgid){
		$sql = "SELECT
					oo.orgdesc 
				FROM
					obras.orgao oo
				WHERE
					oo.orgstatus = 'A' AND
					oo.orgid = {$orgid}";
	}else{
		$sql = "	SELECT
						orgdesc 
					FROM
						obras.orgao oo
					JOIN obras.usuarioresponsabilidade ur ON oo.orgid = ur.orgid
															 AND rpustatus = 'A'
					WHERE
						orgstatus = 'A' AND 
						usucpf = '{$_SESSION["usucpf"]}'
				UNION
					SELECT
						DISTINCT
					    oo.orgdesc
					FROM
					obras.orgao oo 
					JOIN obras.obrainfraestrutura oi ON oi.orgid = oo.orgid
														AND oi.obsstatus = 'A'
					JOIN obras.usuarioresponsabilidade ur ON ur.entid = oi.entidunidade
															 AND ur.orgid IS NULL
															 AND ur.rpustatus = 'A'
															 AND ur.usucpf = '{$_SESSION["usucpf"]}'
					WHERE
						orgstatus = 'A'";
	}
	
	return $dsc = $db->pegaUm($sql);
}

/**
 * Função que lista as unidades
 *
 */
function listaUnidades(){
	global $db, $funid, $estuf, $muncod, $orgid;
		
	$where  = array();
	$campo  = array();
	$from   = array();
	 
	if (($db->testa_superuser() || possuiPerfil(PERFIL_SAMPR))  && !$orgid){
		echo "<tr>
				<td style='color:red;'>Faça sua busca...</td>
			  </tr>";
		return;
	}
//	if ($orgid == 3 && !$estuf){
//		echo "<tr>
//				<td style='color:red;'>Selecione uma unidade federativa para continuar a busca...</td>
//			  </tr>";
//		return;
//		
//	}
	
	/*if ($orgid == 3 && !$muncod){
		echo "<tr>
				<td style='color:red;'>Selecione um município para continuar a busca...</td>
			  </tr>";
		return;
		
	}*/
	
	if ($orgid == 3):
		
		###### Monta "from" filtro de Estado e Município para o SQL ######
		$campo[] = $estuf || $muncod ? 'ed.estuf,' : '';
		$campo[] = $estuf && $muncod ? 'ed.estuf, m.mundescricao, ' : '';
		$from[]  = $estuf || $muncod ? 'INNER JOIN entidade.endereco ed ON ed.entid = e.entid' : '';
		$from[]  = $estuf && $muncod ? ' INNER JOIN territorios.municipio m ON m.muncod IN (\'' . $muncod . '\')': '';
		 
		###### Monta filtro de Estado para o SQL ######
		if ($estuf)
			$where[] = "ed.estuf IN('{$estuf}')"; 
	
		###### Monta filtro de Município para o  SQL ######
		if ($muncod)
			$where[] = "ed.muncod IN ('{$muncod}')";
	endif;	
	// SQL para buscar unidades existentes
	if ($db->testa_superuser() || possuiPerfil(PERFIL_SAMPR)){		
		$unidadesExistentes = $db->carregar("SELECT
												DISTINCT
												e.entid,
												" . implode("", $campo) . "
												e.entnome,
												e.entcodent,
												oo.orgid,
												oo.orgdesc
											FROM 
												entidade.entidade e
											JOIN obras.obrainfraestrutura oi ON oi.entidunidade = e.entid
				    															AND oi.orgid = {$orgid}
				    															AND oi.obsstatus = 'A'	
				    						JOIN obras.orgao oo ON oo.orgid = oi.orgid
				    											   AND oo.orgstatus = 'A' 									
												" . implode("",$from) . "
												".($where ? " WHERE ".implode(" AND ", $where) : '')."
											ORDER BY 
												entnome");
	} else {
		$unidadesExistentes = $db->carregar("	(SELECT
													DISTINCT
													e.entid,
													" . implode("", $campo) . "
													e.entnome,
													e.entcodent
												FROM 
													entidade.entidade e
												JOIN obras.obrainfraestrutura oi ON oi.entidunidade = e.entid
					    															AND oi.orgid = {$orgid}		
					    															AND oi.obsstatus = 'A'	
													" . implode("",$from) . "
												JOIN obras.usuarioresponsabilidade ur ON ur.orgid = oi.orgid
												WHERE
													rpustatus = 'A' AND
													ur.usucpf = '{$_SESSION["usucpf"]}'
													".($where ? " AND ".implode(" AND ", $where) : '')."
												ORDER BY 
													entnome
											)UNION( 	
												SELECT
													DISTINCT
													e.entid,
													" . implode("", $campo) . "
													e.entnome,
													e.entcodent
												FROM 
													entidade.entidade e
													JOIN obras.obrainfraestrutura oi ON oi.entidunidade = e.entid
					    																AND oi.orgid = {$orgid}		
					    																AND oi.obsstatus = 'A'
					    								" . implode("",$from) . "								
													JOIN obras.usuarioresponsabilidade ur ON ur.entid = oi.entidunidade
																							 AND ur.orgid IS NULL
																							 AND ur.rpustatus = 'A'
																							 AND ur.usucpf = '{$_SESSION["usucpf"]}'
													WHERE
														obsstatus = 'A'
														".($where ? " AND ".implode(" AND ", $where) : '')."
													ORDER BY
														entnome)");

	}
	if (!$unidadesExistentes){
		echo "<tr>
				<td style='color:red;'>Busca não retornou registros...</td>
			  </tr>";
		return;
	}	
	
	$count = count($unidadesExistentes);
	$orgdsc = recuperaOrgao($orgid);
	// Monta as TR e TD com as unidades
	for ($i = 0; $i < $count; $i++){
		$codigo    = $unidadesExistentes[$i]["entid"];
		$descricao = $unidadesExistentes[$i]["entnome"];
		$municipio = $unidadesExistentes[$i]["mundescricao"];
		$codigoUf  = $unidadesExistentes[$i]["estuf"];
		$codinep   = $unidadesExistentes[$i]["entcodent"];
		//dbg($codinep, 1);
		if (fmod($i,2) == 0){ 
			$cor = '#f4f4f4';
		} else {
			$cor='#e0e0e0';
		}
		
		echo "
			<tr bgcolor=\"".$cor."\">
				<td align=\"right\" width=\"10%\">
					<input type=\"checkbox\" name=\"unicod\" id=\"".$codigo."\" value=\"$orgid|$codigo\" onclick=\"retorna('".$i."');\">
					<input type=\"hidden\" name=\"unidsc\" value=\"".($orgdsc.' - '.$descricao)."\">
				</td>
				<td>
					".( ( $orgid == 3 && !empty($codinep) ) ? $codinep . " - " . $descricao : $orgdsc.' - '.$descricao )."
				</td>
			</tr>";
	}
	
}

/**
 * Função que atribui a responsabilidade de uma unidade ao usuário
 *
 * @param string $usucpf
 * @param int $pflcod
 * @param string $unicod
 */
function atribuiUnidade($usucpf, $pflcod, $entid){
	
	global $db;
	
	$data = date("Y-m-d H:i:s");
	
	$sql_zera = $db->executar("UPDATE 
								obras.usuarioresponsabilidade 
							   SET 
								rpustatus = 'I' 
							   WHERE 
								usucpf = '{$usucpf}' AND 
								pflcod = '{$pflcod}' AND 
								-- prsano = '{$_SESSION["exercicio"]}' AND 
								entid IS NOT NULL");
	
	if (is_array($entid) && !empty($entid[0])){
		$count = count($entid);
		
		// Insere a nova unidade
		$sql_insert = "INSERT INTO obras.usuarioresponsabilidade (
							entid, 
							usucpf, 
							rpustatus, 
							rpudata_inc, 
							pflcod -- , 
							-- prsano
					   )VALUES";
		
		for ($i = 0; $i < $count; $i++){
			
			list(,$entidade) = explode("|", $entid[$i]);
			
			if ( $entidade != $entidade_antiga ){
				$arrSql[] = "(
								'{$entidade}',
								'{$usucpf}', 
								'A', 
								'{$data}', 
								'{$pflcod}' -- , 
							--	'{$_SESSION["exercicio"]}'
							 )";
			}
			
			$entidade_antiga = $entidade;
			
		}

		$sql_insert = (string) $sql_insert.implode(",",$arrSql);
		$db->executar($sql_insert);
	}
	$db->commit();
	die("<script>
			alert('Operação realizada com sucesso!');
			window.parent.opener.location.href = window.opener.location;
			self.close();
		 </script>");
	
}

function buscaUnidadesCadastradas($usucpf, $pflcod){
	
	global $db, $unicod;
	
//	if (!$_POST['gravar'] && $_REQUEST["uniresp"][0]){
//		foreach ($_REQUEST["uniresp"] as $v){
//			list(,$entid[]) = explode('|', $v );
//		}
//		$where = " e.entid IN (".implode(',',$entid).") -- AND ef.funid in (1, 3, 6, 7, 11, 12, 14, 34, 43, 42, 44)";
//	}else{
		$where = " (ur.usucpf = '{$usucpf}' AND 
			 	    ur.pflcod = {$pflcod})";	
//	}

	$sql = "SELECT DISTINCT 
				e.entid as codigo, 
				CASE WHEN (m.mundescricao is not null AND ed.estuf is not null) THEN
					e.entnome || ' - ' || m.mundescricao || ' - ' || ed.estuf 
				ELSE 
					e.entnome 
				END as descricao,
				oo.orgid,
				oo.orgdesc 
			FROM 
				obras.usuarioresponsabilidade ur
			JOIN entidade.entidade e ON e.entid = ur.entid
			JOIN obras.obrainfraestrutura oi ON oi.entidunidade = e.entid
												AND oi.obsstatus = 'A'
			JOIN obras.orgao oo ON oo.orgid = oi.orgid
								   AND oo.orgstatus = 'A'
		    LEFT JOIN 
				entidade.endereco ed ON ed.entid = e.entid
			LEFT JOIN 
				territorios.municipio m ON m.muncod = ed.muncod
			WHERE 
			 	ur.rpustatus = 'A' AND 	
			 ".$where;
	$RS = @$db->carregar($sql);
	
	if(is_array($RS)) {
		$arDescricao = array();
		foreach ($RS as $linha) {
			extract($linha);
			$arDescricao[]  = "{$orgdesc} - {$descricao}";
	    	print " <option title=\"{$descricao}\" value=\"$orgid|$codigo\">{$orgdesc} - $descricao</option>";
		}
	} else{
		print '<option value="">Faça o filtro para selecionar.</option>';
		
	}
}

?><html>
	<head>
		<meta http-equiv="Pragma" content="no-cache">
		<title>Unidades</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
		<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
	</head>
	<body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff">
		<div align=center id="aguarde"><img src="/imagens/icon-aguarde.gif" border="0" align="absmiddle">
			<font color=blue size="2">Aguarde! Carregando Dados...</font>
		</div>
		
		<form name="formulario" action="<?=$_SERVER['REQUEST_URI'] ?>" method="post">
		<table style="width:100%; display:none;" id="filtro" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td class="subtitulodireita">Tipo de Estabelecimento:</td>
				<td>
				<?php
					if ($db->testa_superuser() || possuiPerfil(PERFIL_SAMPR)){
						$sql = "SELECT
									DISTINCT
									oo.orgid AS codigo, 
									oo.orgdesc AS descricao
								FROM
									obras.orgao oo
								WHERE
									orgstatus = 'A'";

						$db->monta_combo('orgid',$sql,'S',"-- Selecione para filtrar --",'filtroFunid','');
					}else{
						$sql = "	SELECT
										oo.orgid AS codigo, 
										oo.orgdesc AS descricao
									FROM
										obras.orgao oo
									JOIN obras.usuarioresponsabilidade ur ON ur.orgid = oo.orgid
																			 AND ur.rpustatus = 'A'
									WHERE
										oo.orgstatus = 'A' AND
										ur.usucpf = '{$_SESSION["usucpf"]}'
								UNION
									SELECT
										DISTINCT
									    oo.orgid AS codigo, 
										oo.orgdesc AS descricao
									FROM
										obras.orgao oo 
									JOIN obras.obrainfraestrutura oi ON oi.orgid = oo.orgid
																	    AND oi.obsstatus = 'A'
									JOIN obras.usuarioresponsabilidade ur ON ur.entid = oi.entidunidade
																			 AND ur.orgid IS NULL
																			 AND ur.estuf IS NULL
																			 AND ur.obrid IS NULL
																			 AND ur.rpustatus = 'A'
																			 AND ur.usucpf = '{$_SESSION["usucpf"]}'
									WHERE
										oo.orgstatus = 'A'";
						
						$db->monta_combo('orgid',$sql,'S',"",'filtroFunid','');
					}
				
					echo '&nbsp;<img src="/imagens/obrig.gif" title="Indica campo obrigatório">';					
				?>
				</td>
			</tr>
<? if ($orgid == 3): ?>			
			<tr>
				<td class="subtitulodireita">Unidade Federativa:</td>
				<td>
				<?php
				$sql = "SELECT
						 estuf AS codigo,
						 estuf || ' - ' || estdescricao AS descricao
						FROM
						 territorios.estado
						ORDER BY
						 estuf";
				
				$db->monta_combo('estuf',$sql,'S',"-- Selecione para filtrar --",'limpaMuncod(); filtroFunid','');
				echo '&nbsp;<img src="/imagens/obrig.gif" title="Indica campo obrigatório">';					
				?>
				</td>
			</tr>
			<? if ($estuf): ?>			
			<tr>
				<td class="subtitulodireita">Município:</td>
				<td>
				<?php
				$sql = "SELECT
						 muncod AS codigo,
						 mundescricao AS descricao
						FROM
						 territorios.municipio
						WHERE
						 estuf = '{$estuf}'
						ORDER BY
						 mundescricao";
				
				$db->monta_combo('muncod',$sql,'S',"-- Selecione para filtrar --",'filtroFunid','');
				?>
				</td>
			</tr>	
			<? endif; ?>
<? endif; ?>								
		</table>		
		<!-- Lista de Unidades -->
		<div id="tabela" style="overflow:auto; width:496px; height:270px; border:2px solid #ececec; background-color: #ffffff;">	
				<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
					<script language="JavaScript">
						//document.getElementById('tabela').style.visibility = "hidden";
						document.getElementById('tabela').style.display  = "none";
					</script>
					<thead>
						<tr>
							<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3"><strong>Selecione a(s) Unidade(s)</strong></td>		
						</tr>
					</thead>
					<?php listaUnidades(); ?>
				</table>
		</div>
		<script language="JavaScript">
			document.getElementById('filtro').style.display = 'block';
		</script>
		<!-- Unidades Selecionadas -->
			<input type="hidden" name="usucpf" value="<?=$usucpf?>">
			<input type="hidden" name="pflcod" value="<?=$pflcod?>">
			<select multiple size="8" name="uniresp[]" id="uniresp" style="width:500px;" onkeydown="javascript:combo_popup_remove_selecionados( event, 'uniresp' );" class="CampoEstilo" onchange="//moveto(this);">				
				<?php 
					buscaUnidadesCadastradas($usucpf, $pflcod);
				?>
			</select>
		<!-- Submit do Formulário -->
		<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<tr bgcolor="#c0c0c0">
				<td align="right" style="padding:3px;" colspan="3">
					<input type="Button" name="ok" value="OK" onclick="selectAllOptions(campoSelect); document.getElementsByName('gravar')[0].value=1; document.formulario.submit();" id="ok">
					<input type="hidden" name="gravar" value="">
				</td>
			</tr>
		</table>
</form>
<script type="text/JavaScript">

document.getElementById('aguarde').style.visibility = "hidden";
document.getElementById('aguarde').style.display  = "none";
//document.getElementById('tabela').style.visibility = "visible";
document.getElementById('tabela').style.display  = 'block';


var campoSelect = document.getElementById("uniresp");

<?
//if ($funid):
?>
if (campoSelect.options[0].value != ''){
	for(var i=0; i<campoSelect.options.length; i++){
		var id = campoSelect.options[i].value.split('|');
		
		if (document.getElementById(id[1])){
			document.getElementById(id[1]).checked = true;
		}
	}
}
<?
//endif;
?>


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
	if (campoSelect.options[0].value=='')
		tamanho--;
	if(document.formulario.unicod[objeto]) {
		if (document.formulario.unicod[objeto].checked == true){
			campoSelect.options[tamanho] = new Option(document.formulario.unidsc[objeto].value, document.formulario.unicod[objeto].value, false, false);
			sortSelect(campoSelect);
		}else{
			for(var i=0; i<=campoSelect.length-1; i++){
				if (document.formulario.unicod[objeto].value == campoSelect.options[i].value){
					campoSelect.options[i] = null;
				}
			}
			if (!campoSelect.options[0]){
				campoSelect.options[0] = new Option('Clique na Unidade.', '', false, false);
			}
			sortSelect(campoSelect);
		}
	}else{
		// qunado possui apenas 1 registro
		if (document.formulario.unicod.checked == true){
			campoSelect.options[tamanho] = new Option(document.formulario.unidsc.value, document.formulario.unicod.value, false, false);
			sortSelect(campoSelect);
		}else{
			for(var i=0; i<=campoSelect.length-1; i++){
				if (document.formulario.unicod.value == campoSelect.options[i].value){
					campoSelect.options[i] = null;
				}
			}
			if (!campoSelect.options[0]){
				campoSelect.options[0] = new Option('Clique na Unidade.', '', false, false);
			}
			sortSelect(campoSelect);
		}
	}
}

function moveto(obj) {
	if (obj.options[0].value != '') {
		if(document.getElementById('img'+obj.value.slice(0,obj.value.indexOf('.'))).name=='+'){
			abreconteudo(obj.value.slice(0,obj.value.indexOf('.')));
		}
		document.getElementById(obj.value).focus();}
}

function filtroFunid (id) {

	var d 	   = document;
	var orgid  = d.getElementsByName('orgid')[0]  ? d.getElementsByName('orgid')[0].value : '';
	var estuf  = d.getElementsByName('estuf')[0]  ? d.getElementsByName('estuf')[0].value : '';;
	var muncod = d.getElementsByName('muncod')[0] ? d.getElementsByName('muncod')[0].value : '';

	if (!orgid){
		alert('Selecione um "tipo de ensino" afim de efetuar o filtro!');
		return false;
	}
	
	selectAllOptions(campoSelect);
	d.formulario.submit();
	//window.location.href = '?pflcod=<?=$_GET['pflcod']; ?>&usucpf=<?=$_GET['usucpf']; ?>&funid='+funid+'&estuf='+estuf+'&muncod='+muncod;
}

function limpaMuncod(){
	if (document.getElementsByName('muncod')[0]) {
		document.getElementsByName('muncod')[0].value='';
	}
}
</script>
	</body>
</html>
