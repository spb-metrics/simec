<?php
$atiid = $_POST[atiid] ? $_POST[atiid] : $_GET[atiid]; 

define("AD"   ,26101);
define("CAPES",26291);
define("INEP" ,26290);
define("FNDE" ,26298);
define("FIES" ,74902);

if (!$atiid)
	die("<script>window.close();</script>");

//if ($_POST[pliidDel] || $_POST[pliid]):	
if ($_POST[pliidDel] || $_POST[pliidIns]):
	if ($_POST[pliidDel]):
		$sql 	  = "DELETE FROM projetos.planointernoatividade WHERE pliid IN (";
		$pliidDel = explode(";",$_POST[pliidDel]);
		
		do {
			$sql .= current($pliidDel).",";		
		} while (next($pliidDel) != false);
		
		$sql .= ") AND atiid = {$atiid}";
		$sql = str_replace(",)",")",$sql);

		$db->executar($sql);
	endif;
		
	if ($_POST[pliidIns]):
		unset($sql);
		
		$_POST[pliidIns] = explode(";", substr($_POST[pliidIns], 0, (strlen($_POST[pliidIns]) - 1)));
		
		foreach ($_POST[pliidIns] as $pliid):
		$sql .= "INSERT INTO projetos.planointernoatividade (atiid, pliid) ";
		$sql .= sprintf("SELECT %d, %d 
						 WHERE 
  							NOT EXISTS 
  								(SELECT
  								   pliid,
  								   atiid 
  								  FROM
  								   projetos.planointernoatividade
  								  WHERE
  								   atiid = %d AND
  								   pliid = %d);",$atiid, $pliid, $atiid, $pliid);
		endforeach;
		$db->executar($sql);
	endif;
	
	$db->commit();
	die ("<script>
			alert('Operação executada com sucesso!');
			window.opener.location.replace(window.opener.location);
			window.close();
		  </script>");
endif;


$sbacod = $_POST[sbacod];
/*
if (strlen($_POST[prgcod]) == 1)
	$prgcod = "000{$_POST[prgcod]}";
elseif (strlen($_POST[prgcod]) == 2)
	$prgcod = "00{$_POST[prgcod]}";
elseif (strlen($_POST[prgcod]) == 3)
	$prgcod = "0{$_POST[prgcod]}";
else
*/
	$prgcod = $_POST[prgcod];
/*
	if (strlen($_POST[acacod]) == 1)
	$acacod = "000{$_POST[acacod]}";
elseif (strlen($_POST[acacod]) == 2)
	$acacod = "00{$_POST[acacod]}";
elseif (strlen($_POST[acacod]) == 3)
	$acacod = "0{$_POST[acacod]}";
else
*/
	$acacod = $_POST[acacod];

/*	
if (strlen($_POST[unicod]) == 1)
	$unicod = "0000{$_POST[unicod]}";
elseif (strlen($_POST[unicod]) == 2)
	$unicod = "000{$_POST[unicod]}";
elseif (strlen($_POST[unicod]) == 3)
	$unicod = "00{$_POST[unicod]}";
elseif (strlen($_POST[unicod]) == 4)
	$unicod = "0{$_POST[unicod]}";
else */
	$unicod = $_POST[unicod];
	
	$plicod = $_POST[plicod];

if (strlen($_POST[loccod]) == 1)
	$loccod = "000{$_POST[loccod]}";
elseif (strlen($_POST[loccod]) == 2)
	$loccod = "00{$_POST[loccod]}";
elseif (strlen($_POST[loccod]) == 3)
	$loccod = "0{$_POST[loccod]}";
else
	$loccod = $_POST[loccod];
?>

<html>
<head>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script>
function allSelect(obj) {
	element = document.getElementsByName('pliid[]');

	param = obj.checked ? obj.checked : obj.checked;
	
	for (i = 0; i < element.length; i++) {
		element[i].checked = param;
		
		if (element[i].id == 'pliidDelete')
			delSelected(element[i]);
		else
			insertSelected(element[i]);
		//element[i].click();
	} 
}

function checado() {
	element  = document.getElementsByName('pliid[]');
    element2 = document.getElementsByName('allElements')[0];
    
	for (i = 0; i < element.length; i++) {
		if (element[i].checked == false) {
			element2.checked = false;
			return false;
			break;
		}
	}
	element2.checked = true;	
}

function delSelected(obj) {
	d 	  = document;
	//input = d.formulario2.pliidDel;
	input = d.getElementById("pliidDel");
	
	if (obj.checked)
		input.value = input.value.replace(obj.value+';','');
	else
		input.value += obj.value+';';
}

function insertSelected(obj) {
	input = document.getElementById("pliidIns");
	
	if ( ! obj.checked )
		input.value = input.value.replace(obj.value+';','');
	else
		input.value += obj.value+';';
}
</script>
<!-- AUTO COMPLEMENTO -->
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/_start.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/keys/keyEvents.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/tags/suggest.js"></script>
<style>
	.suggestMarked
	{
		background-color: #AAAAFF;
		width:500px; 
		border-style: solid; 
		border-width: 1px; 
		border-color: #DDDDFF;
		border-top-width: 0px;
		position: relative;
		z-index: 100;
	}
	.suggestUnmarked
	{
		background-color: #EEEEFF;
		width:500px; 
		border-style: solid; 
		border-width: 1px; 
		border-color: #DDDDFF;
		border-top-width: 0px;
		z-index: 100;
		position: relative;
	}
	
</style>
</head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<?php
monta_titulo( 'Pesquisa Plano Interno', '&nbsp'  );
?>
<form action="" method="post" name="formulario">
<table width="100%" class="tabela" bgcolor="#f5f5f5" border="0" cellSpacing="1" cellPadding="3" align="center">
  <tr>
	<td class="SubTituloDireita" align="right">Programa:</td>
    <td>	
    	<?php
    	$sql = "SELECT
    			 DISTINCT pr.prgcod as codigo, 
    			 prgdsc as descricao 
    			FROM
    			 monitora.programa pr JOIN monitora.acao ac ON (pr.prgid = ac.prgid)
    			 JOIN financeiro.planointerno pl on(pl.acaid = ac.acaid)
    			WHERE
    			 pr.prgano = '" . $_SESSION['exercicio'] . "' AND
    			 prgstatus = 'A' 
    			ORDER BY
    			 pr.prgcod";
    	 //$complemento = ' onkeypress="item_financeiro_alterar_campo_focado( event, \'asdf\' );" ';
		 texto_popup( 'prgcod', $sql, 'Programa', 200, 45, '', '', $complemento ); 
		?>
    </td>
  </tr>
  <tr>
	<td class="SubTituloDireita" align="right">Ação:</td>
    <td>	
    	<?php
    	unset($sql);
    	$sql = "SELECT
				 DISTINCT acacod AS codigo,
				 acadsc AS descricao	 
    			FROM
    			 monitora.acao ac JOIN financeiro.planointerno pl USING(acaid)
    			WHERE
    			 prgano = '" . $_SESSION['exercicio'] . "' AND
    			 acastatus = 'A' 
    			ORDER BY
    			 acacod";
    	
    	 //$complemento = ' onkeypress="item_financeiro_alterar_campo_focado( event, \'asdf\' );" ';
		 texto_popup( 'acacod', $sql, 'Ação', 200, 45, '', '', $complemento ); 
		?>
    </td>
  </tr>  
  <tr>
		<td class="SubTituloDireita" align="right">Subação:</td>
		<td>
		<?
		$sql = "SELECT
					sbacod as codigo,
					sbatitulo as descricao
				FROM
					monitora.subacao
				WHERE
					sbastatus = 'A'
				ORDER BY
					sbatitulo";
		//$db->monta_combo( 'sbacod', $sql, 'S', 'Selecione','','');
		 texto_popup( 'sbacod', $sql, 'Subação', 200, 45, '', '', $complemento );  
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" align="right">Unidade:</td>
		<td>
		<?
		$sql = "SELECT
			   --ent.entid as codigo,
			   entunicod as codigo,
			   CASE fen.funid
			   	when 34 then 
			   		entungcod ||' - '|| entnome 
			   	else 
			   		entunicod ||' - '|| entnome 
			   	end as descricao
			   FROM entidade.entidade ent 
			   LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
			   WHERE entunicod IN('".AD."','".CAPES."','".INEP."','".FNDE."','".FIES."') and 
			   		 fen.funid <> 33 and ent.entid not in(391293,391294) 
			   ORDER BY entunicod";
		$db->monta_combo( 'unicod', $sql, 'S', 'Selecione','',''); 
		?>
		</td>
	</tr>	
	<tr>
		<td class="SubTituloDireita" align="right">Localizador:</td>
		<td><?= campo_texto( 'loccod', 'N', 'S', '', 10, 4, '', '' ); ?></td>
	</tr>
	<tr style="background-color: #cccccc">
		<td align='right' style="vertical-align:top; width:25%;">&nbsp;</td>
		<td>
			<input type="submit" name="botao" value="Pesquisar"/>
			<!--<input type="button" name="close" value="Fechar" onclick="window.close();">  -->
		</td>
	</tr>
</table>
</form>
<br>
<form action="" method="post" name="formulario2">
<table width="95%" border="0" cellSpacing="1" cellPadding="3" align="center">
	<tr style="background-color: #cccccc">
		<td align='right' style="vertical-align:top; width:25%;">&nbsp;</td>
		<td>
			<input type="submit" name="botao" value="Atualizar"/>
			<input type="button" name="close" value="Fechar" onclick="window.close();">
			<input type="hidden" name="atiid" value="<?php echo $atiid; ?>">
			<input type="hidden" id="pliidIns" name="pliidIns">
			<input type="hidden" id="pliidDel" name="pliidDel">
			<input type="hidden" id="ano" name="ano" value="<?=$_REQUEST["ano"]?>" />
		</td>
	</tr>
	<tr style="background-color: #cccccc">
		<td colspan="2">
			<input type='checkbox' onclick='allSelect(this);' name='allElements' /> <b>Selecionar todos os PI's</b>
		</td>
	</tr>		
</table>
<?php
	
$where .= $_POST[prgcod] ? "AND upper(ac.prgcod ||' '|| prgdsc) LIKE('%".strtoupper($prgcod)."%')" : '';
$where .= $_POST[acacod] ? "AND upper(acacod ||' '|| acadsc) LIKE('%".strtoupper($acacod)."%')" : '';
$where .= $_POST[unicod] ? "AND ac.unicod ='{$unicod}'" : '';
$where .= $_POST[loccod] ? "AND loccod ='{$loccod}'" : '';
$where .= $_POST[sbacod] ? "AND sba.sbacod = '{$sbacod}'" : '';
/*
$sql = "SELECT
		 '<input type=\"checkbox\" name=\"pliid[]\" value=\"' || pliid || '\" />',
		 pl.plicod || ' - ' || pl.plidsc as planoInterno,
		 prgcod || '.' || acacod || '.' || unicod || '.' || loccod || ' - ' || acadsc as acao 
		FROM 
		 projetos.planointerno pl JOIN monitora.acao USING(acaid)";
*/
$sql = sprintf("SELECT
				 CASE WHEN
				  pa.pliid IS NULL
				 THEN
				  '<input type=\"checkbox\" name=\"pliid[]\" value=\"' || pl.pliid || '\" onclick=\"insertSelected(this); if(this.checked==false){document.getElementsByName(\'allElements\')[0].checked=false;}\" />'
				 ELSE
				  '<input type=\"checkbox\" name=\"pliid[]\" value=\"' || pl.pliid || '\" id=\"pliidDelete\" checked=\"checked\" onclick=\"delSelected(this); if(this.checked==false){document.getElementsByName(\'allElements\')[0].checked=false;}\" />'
				 END,
				 pl.plicod || ' - ' || pl.plidsc as planoInterno,
				 unidsc,
				 ac.prgcod || '.' || acacod || '.' || ac.unicod || '.' || loccod || ' - ' || acadsc as acao 
				FROM
				 monitora.acao ac 
				 JOIN projetos.planointerno pl USING(acaid)
				 JOIN public.unidade un ON un.unicod = ac.unicod 
				 LEFT JOIN projetos.planointernoatividade pa ON pa.pliid = pl.pliid AND pa.atiid = %d and pa.pliorigem = 'm' 
				 JOIN monitora.programa pr ON pr.prgid = ac.prgid 
				 INNER JOIN monitora.subacao sba ON sba.sbacod =  substr(pl.plicod, 2, 4)
				WHERE
				  pl.pliid NOT IN (SELECT
				 					pliid
				 				   FROM
				 				  	projetos.planointernoatividade
				 				   WHERE
				 				  	atiid != %d and pliorigem = 'm' ) AND
				  acastatus = 'A' and pr.prgano = '".$_REQUEST["ano"]."'",
				$atiid, $atiid);

$sql .= $where ? $where : ''; 
$sql .= " ORDER BY plidsc;";

//$cabecalho = array ("<input type='checkbox' onclick='allSelect(this);' name='allElements' />", "Cod. PI - Descrição", "Unidade Orçamentária", "Ação");
$cabecalho = array ("Marcar", "Cod. PI - Descrição", "Unidade Orçamentária", "Ação");
//$db->monta_lista_simples($sql,$cabecalho,999999,'','','','');

?>
</form>

<?
$db->monta_lista($sql,$cabecalho,50,5,'S','','');
?>
<script type="text/javascript">
	checado();
</script>
</body>
</html>