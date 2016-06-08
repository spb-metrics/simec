<?php

define("AD"   ,26101);
define("CAPES",26291);
define("INEP" ,26290);
define("FNDE" ,26298);
define("FIES" ,74902);


$atiid = $_POST['atiid'] ? $_POST['atiid'] : $_GET['atiid'];

$prgcod = $_POST['prgcod'];
$acacod = $_POST['acacod'];
$prgano = date("Y");

if($_POST['atualizar'] == '1'){
	unset($_POST['prgcod']);
	unset($_POST['acacod']);
}

if (!$atiid) die("<script>window.close();</script>");
?>
<html>
<head>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
</head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<? monta_titulo( 'Selecione uma unidade', 'Buscar ações'  ); ?>
<form action="" method="post" name="formulario">

<table width="100%" class="tabela" bgcolor="#f5f5f5" border="0" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" align="right" style="width:100px">Programa:</td>
		<td>
		<?
		$sql = "select
				 p.prgcod as codigo,
		 		 (p.prgcod || ' - ' || p.prgdsc) as descricao
				from
					monitora.programa p
				where 				 
				 p.prgano = '".$prgano."'
				order by prgdsc
				 ";
		$db->monta_combo( 'prgcod', $sql, 'S', 'Selecione','','','',500); 
		?>
		</td>
	</tr>	
	<tr>
		<td class="SubTituloDireita" align="right" style="width:100px">Ação:</td>
		<td>
		<?
		$sql = "SELECT
				 ac.acacod as codigo,
		 		 (ac.acacod || ' - ' || ac.acadsc) as descricao
				 FROM monitora.acao ac 
				 WHERE 
				 ac.prgano = '".$prgano."' and ac.unicod IN('".AD."','".CAPES."','".INEP."','".FNDE."') 
				 GROUP BY ac.acacod, ac.acadsc 
				 ORDER BY ac.acadsc
				 ";
		$db->monta_combo( 'acacod', $sql, 'S', 'Selecione','','','',500); 
		?>
		</td>
	</tr>	
	<tr>
		<td class="SubTituloDireita" align="right" style="width:100px">Busca livre:</td>
		<td>
		<? echo campo_texto('buscalivre', "N", "S", "Nome do grupo", 67, 150, "", "", '', '', 0, '' ); ?>
		</td>
	</tr>
	<tr style="background-color: #cccccc">
		<td align='right' style="vertical-align:top;">&nbsp;</td>
		<td>
		<input type="submit" name="botao" value="Pesquisar"/>
		<input type="button" name="botao" value="Todos" onclick="alert('sadasdasd');window.location='?modulo=principal/planotrabalho/listarPrograma&acao=A';"/>
		<input type="button" name="close" value="Fechar" onclick="window.close();">	
		</td>
	</tr>		
</table>
</form>
<br>
<?php	
$where .= $_POST['prgcod'] ? " AND upper(dtl.prgcod) LIKE('%".strtoupper($_POST[prgcod])."%')" : '';
$where .= $_POST['acacod'] ? " AND upper(dtl.acacod) LIKE('%".strtoupper($_POST[acacod])."%')" : '';
$where .= $_POST['buscalivre'] ? "AND (trim(dtl.prgcod||'.'||dtl.acacod||'.'||dtl.loccod||' - '||dtl.acadsc) ilike('%".$_POST['buscalivre']."%') OR dtl.ptres ilike '%".$_POST['buscalivre']."%')" : '';


$sql_lista = "SELECT 
				'<input type=\"checkbox\" id=\"chk_'||dtl.ptres||'\" onclick=\"resultado(this,\''||dtl.acaid||'\',\''||dtl.ptres||'\');\">' as checkbox,
				dtl.ptres,
				trim(dtl.prgcod||'.'||dtl.acacod||'.'||dtl.loccod||' - '||dtl.acadsc) as descricao,
				ent.entnome,
				SUM(dtl.ptrdotacao) as dotacaoinicial,
				coalesce(cast(SUM(dtl.valorpi) as varchar),'0.00') as detalhamento,
				(SUM(dtl.ptrdotacao)-coalesce(SUM(dtl.valorpi),0)) as diferenca 
				FROM monitora.v_detalhepiptres dtl 
				LEFT JOIN entidade.entidade ent ON ent.entunicod = dtl.unicod AND ent.entungcod IS NULL
				WHERE
					  dtl.unicod IN ('".AD."','".CAPES."','".INEP."','".FNDE."','".FIES."') AND
					  dtl.prgano = '".$prgano."' ";


$sql_lista .= $where ? $where : ''; 
$sql_lista .= " GROUP BY dtl.ptres,checkbox,descricao,ent.entnome  
				ORDER BY 1;";

$cabecalho = array ("", "PTRES", "Ação", "Unidade Orçamentária", "Dotação inicial", "Detalhamento por PI", "Disponível" );
$db->monta_lista($sql_lista,$cabecalho,60,20,'','','');

?>
<script type="text/javascript">
/* CARREGANDO OS DADOS DE PTRES */
var tabelaorigem = window.opener.document.getElementById('orcamento');
for(i=2;i<tabelaorigem.rows.length-2;i++) {
	document.getElementById("chk_"+tabelaorigem.rows[i].cells[0].innerHTML).checked=true;
}
/* FIM CARREGANDO OS DADOS DE PTRES */

function resultado(dados, acaid, ptres){

	if(!ptres) {
		alert('Não existe PTRES. Entre em contato com o administrador do sistema.');
		return false;
	}

	if(dados.checked) {


		
	var linhaTbl = dados.parentNode.parentNode;
	var tabelaorigem = window.opener.document.getElementById('orcamento');
	if(eval(tabelaorigem.rows.length%2)) {
		var cor = "";
	} else {
		var cor = "#DCDCDC";
	}
	var linha = tabelaorigem.insertRow(2);
	linha.id = "ptres_"+ptres;
	linha.style.backgroundColor = cor;
	linha.style.height = '30px';
	
	// setando o ptres
	var celula1 = tabelaorigem.rows[2].insertCell(0);
	celula1.style.textAlign = "center";
	celula1.innerHTML = ptres;
	
	var celula2 = tabelaorigem.rows[2].insertCell(1);
	celula2.style.textAlign = "center";
	celula2.innerHTML = linhaTbl.cells[2].innerHTML+"<input type='hidden' name='acaid["+ptres+"]' value='"+acaid+"'>";
	
	var celula3 = tabelaorigem.rows[2].insertCell(2);
	celula3.style.textAlign = "right";
	celula3.innerHTML = linhaTbl.cells[4].innerHTML;
	
	var celula4 = tabelaorigem.rows[2].insertCell(3);
	celula4.style.textAlign = "right";
	celula4.innerHTML = linhaTbl.cells[5].innerHTML;
	
	var celula5 = tabelaorigem.rows[2].insertCell(4);
	celula5.style.textAlign = "right";
	celula5.innerHTML = linhaTbl.cells[6].innerHTML;

	var celula6 = tabelaorigem.rows[2].insertCell(5);
	celula6.style.textAlign = "center";
	celula6.innerHTML = "<input type=\"text\" name=\"plivalor["+ptres+"]["+acaid+"]\" size=\"28\" maxlength=\"\" value=\"\" onKeyUp=\"this.value=mascaraglobal('###.###.###,##',this.value);calculovalorPI();\"  class=\"normal\"  onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" style=\"text-align : right; width:25ex;\" title='' />";

	switch(ptres) {
	case '001703':
		var unidgest = window.opener.document.getElementById('000000003_0');
		var unidresp = window.opener.document.getElementById('000000003_1');
		unidgest.parentNode.parentNode.style.display = 'none';
		unidresp.parentNode.parentNode.style.display = 'none';
		window.opener.document.getElementById('geradorcompleto').style.display = '';
		window.opener.document.getElementById('regraespecial1').options[1] = new Option("2773","2773");
		window.opener.document.getElementById('regraespecial1').options[2] = new Option("2774","2774");
		window.opener.document.getElementById('btn_selecionar_acaptres').disabled = true;
		window.close();
		break;
	case '001704':
		var unidgest = window.opener.document.getElementById('000000003_0');
		var unidresp = window.opener.document.getElementById('000000003_1');
		unidgest.parentNode.parentNode.style.display = 'none';
		unidresp.parentNode.parentNode.style.display = 'none';
		window.opener.document.getElementById('geradorcompleto').style.display = '';
		window.opener.document.getElementById('regraespecial1').options[1] = new Option("2339","2339");
		window.opener.document.getElementById('regraespecial1').options[2] = new Option("2783","2783");
		window.opener.document.getElementById('regraespecial1').options[3] = new Option("2778","2778");
		window.opener.document.getElementById('btn_selecionar_acaptres').disabled = true;
		window.close();
		break;
}

	} else {

	var tabelaorigem = window.opener.document.getElementById('orcamento');
	tabelaorigem.deleteRow(window.opener.document.getElementById('ptres_'+ptres).rowIndex)

	}

}

</script>
</body>
</html>