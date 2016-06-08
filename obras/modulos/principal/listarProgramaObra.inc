<?php

if($_GET['unicod']){
	// verificando a unidade
	$unidade = $db->pegaLinha("SELECT * FROM public.unidade WHERE unicod='".$_GET['unicod']."'");
} else {
	die("<script>alert('Unidade não encontrada');window.close();</script>");
}

// verificando se é undidade ou unidade gestora

if ($_REQUEST['sbaid']){
	$verificaSubacaoUnidade = $db->pegaUm("SELECT count(1) as contador FROM monitora.pi_subacao a inner join monitora.pi_subacaounidade b ON b.sbaid = a.sbaid WHERE a.sbaid ={$_REQUEST['sbaid']} and b.unicod = '".$_GET['unicod']."'");
}

/* Filtro para a unidade indireta */	
if ($verificaSubacaoUnidade) {
$where .= $_REQUEST['sbaid'] ? "AND sad.sbaid = '".$_REQUEST['sbaid']."'" : '';
$clausula1 .= $_REQUEST['sbaid'] ? "where sbaid = '".$_REQUEST['sbaid']."'" : '';
$clausula2 .= $_REQUEST['sbaid'] ? "AND sbaid = '".$_REQUEST['sbaid']."'" : '';
}

/* Filtro para a unidade direta */	
if ($_REQUEST['sbaid'] && !$verificaSubacaoUnidade){
$where_direta .= $_REQUEST['sbaid'] ? "AND sad.sbaid = '".$_REQUEST['sbaid']."'" : '';
$clausula1_direta .= $_REQUEST['sbaid'] ? "and a.sbaid = '".$_REQUEST['sbaid']."'" : '';
$clausula2_direta .= $_REQUEST['sbaid'] ? "AND sbaid = '".$_REQUEST['sbaid']."'" : '';	
}

/* Filtro para a unidade indireta */	
$where .= $_POST['prgcod'] ? "AND UPPER(aca.prgcod) LIKE('%".strtoupper($_POST['prgcod'])."%')" : '';
$where .= $_POST['acacod'] ? "AND UPPER(aca.acacod) LIKE('%".strtoupper($_POST['acacod'])."%')" : '';
$where .= $_POST['buscalivre'] ? "AND (trim(aca.prgcod||'.'||aca.acacod||'.'||aca.loccod||' - '||aca.acadsc) ilike('%".$_POST['buscalivre']."%') OR dtl.ptres ilike '%".$_POST['buscalivre']."%')" : '';

/* Filtro para a unidade direta */	
$where_direta .= $_POST['prgcod'] ? "AND UPPER(aca.prgcod) LIKE('%".strtoupper($_POST['prgcod'])."%')" : '';
$where_direta .= $_POST['acacod'] ? "AND UPPER(aca.acacod) LIKE('%".strtoupper($_POST['acacod'])."%')" : '';
$where_direta .= $_POST['buscalivre'] ? "AND (trim(aca.prgcod||'.'||aca.acacod||'.'||aca.loccod||' - '||aca.acadsc) ilike('%".$_POST['buscalivre']."%') OR dtl.ptres ilike '%".$_POST['buscalivre']."%')" : '';

$unidsc    = $unidade['entunicod']." - ".$unidade['unidsc'];
$unicod    = $unidade['entunicod'];

if($_SESSION['obra']['orgid'] == 1 && possuiPerfil(array(PERFIL_EMPRESA,PERFIL_SUPERVISORMEC,PERFIL_ADMINISTRADOR,PERFIL_GESTORMEC))){
	$st_where = " or aca.unicod = '26101' ";
}

if(possuiPerfil(array(PERFIL_EMPRESA,PERFIL_SUPERVISORMEC,PERFIL_ADMINISTRADOR,PERFIL_GESTORMEC))){

$sql_lista .= " 
			SELECT
					'<input type=\"checkbox\" id=\"chk_'||dtl.ptres||'\" onclick=\"resultado(this,\''||dtl.ptrid||'\',\''||dtl.ptres||'\');\">' as checkbox, 
					dtl.ptres, 
					trim(aca.prgcod||'.'||aca.acacod||'.'||aca.loccod||' - '||aca.acadsc) as descricao, 
					uni.unidsc, 
					dtl.ptrdotacao as dotacaoinicial, 
					coalesce(dt.valor,0.00) as dotacaosubacao, 
					coalesce(sum(dt2.valorpi),0.00) as detalhamento, 
					Case when coalesce(dt.valor,0.00) > 0 then coalesce(dt.valor,0.00)-coalesce(sum(dt2.valorpi),0.00) else coalesce(dtl.ptrdotacao,0.00)-coalesce(sum(dt2.valorpi),0.00) end as valorcadastrado 
					--sum(sad.sadvalor) as valorcadastrado 
				FROM monitora.acao aca
					INNER JOIN monitora.ptres dtl ON aca.acaid = dtl.acaid
					INNER JOIN public.unidade uni ON uni.unicod = dtl.unicod
					LEFT JOIN monitora.pi_subacaodotacao sad ON dtl.ptrid = sad.ptrid 
					LEFT JOIN ( select a.sbaid, a.ptrid,
								sum(sadvalor) as valor
								from  monitora.pi_subacaodotacao a
								inner join monitora.pi_subacao b ON b.sbaid = a.sbaid
								where b.pieid is not null 
								". $clausula1_direta ."
								group by a.sbaid, a.ptrid ) dt ON dtl.ptrid = dt.ptrid and dt.sbaid = sad.sbaid
					LEFT JOIN ( select pi.sbaid, ptres.ptrid, ptrano, coalesce(sum(pipvalor),0) as valorpi 
							from monitora.pi_planointerno pi 
							inner join monitora.pi_planointernoptres pip ON pip.pliid = pi.pliid 
							inner join monitora.ptres ptres ON ptres.ptrid = pip.ptrid 
							where pi.plistatus = 'A' 
								". $clausula2_direta ."								
							group by pi.sbaid, ptres.ptrid, ptrano  ) dt2 ON dtl.ptrid = dt2.ptrid 
					WHERE aca.prgano = '".date("Y")."'
					      and aca.acasnrap = false ";
						  
$sql_lista .= $where_direta ? $where_direta : '';
$sql_lista .= "GROUP BY dtl.ptrid, dtl.ptres,dtl.ptrdotacao, 
						trim(aca.prgcod||'.'||aca.acacod||'.'||aca.loccod||' - '||aca.acadsc) ,
						uni.unidsc, dt.valor";
				} else {
					
	$sql_lista = "SELECT
					'<input type=\"checkbox\" id=\"chk_'||dtl.ptres||'\" onclick=\"resultado(this,\''||dtl.ptrid||'\',\''||dtl.ptres||'\');\">' as checkbox, 
					dtl.ptres, 
					trim(aca.prgcod||'.'||aca.acacod||'.'||aca.loccod||' - '||aca.acadsc) as descricao, 
					uni.unidsc, 
					dtl.ptrdotacao as dotacaoinicial, 
					coalesce(sum(dt.valor),0.00) as dotacaosubacao, 
					coalesce(sum(dt2.valorpi),0.00) as detalhamento, 
					Case when coalesce(sum(dt.valor),0.00) > 0 then coalesce(sum(dt.valor),0.00)-coalesce(sum(dt2.valorpi),0.00) else coalesce(dtl.ptrdotacao,0.00)-coalesce(sum(dt2.valorpi),0.00) end as valorcadastrado 
					--sum(sad.sadvalor) as valorcadastrado 
				FROM monitora.acao aca
					INNER JOIN monitora.ptres dtl ON aca.acaid = dtl.acaid
					INNER JOIN public.unidade uni ON uni.unicod = dtl.unicod
					LEFT JOIN monitora.pi_subacaodotacao sad ON dtl.ptrid = sad.ptrid 
					LEFT JOIN ( select sbaid, ptrid,
								sum( sadvalor ) as valor
								from  monitora.pi_subacaodotacao
								". $clausula1 ."
								group by sbaid, ptrid ) dt ON dtl.ptrid = dt.ptrid and dt.sbaid = sad.sbaid
					LEFT JOIN ( select pi.sbaid, ptres.ptrid, ptrano, coalesce(sum(pipvalor),0) as valorpi 
							from monitora.pi_planointerno pi 
							inner join monitora.pi_planointernoptres pip ON pip.pliid = pi.pliid 
							inner join monitora.ptres ptres ON ptres.ptrid = pip.ptrid 
							where pi.plistatus = 'A' 
								". $clausula2 ."
							group by pi.sbaid, ptres.ptrid, ptrano  ) dt2 ON dtl.ptrid = dt2.ptrid --and dt2.sbaid = sad.sbaid 
					WHERE --aca.prgano = '".date("Y")."'
						  aca.prgano = '".$_GET['anoexercicio']."'
						  and ( aca.unicod = '".$_GET['unicod']."' )
					      and aca.acasnrap = false ";

$sql_lista .= $where ? $where : ''; 
$sql_lista .= "GROUP BY dtl.ptrid, dtl.ptres,dtl.ptrdotacao, 
						trim(aca.prgcod||'.'||aca.acacod||'.'||aca.loccod||' - '||aca.acadsc) ,
						uni.unidsc ";
				}

?>
<html>
<head>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
</head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<?php monta_titulo($entnome, '&nbsp'); ?>
<form action="" method="post" name="formulario">

<table width="100%" class="tabela" bgcolor="#f5f5f5" border="0" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita" align="right">Programa:</td>
	<td>
	<?
	$sql = "SELECT p.prgcod as codigo, (p.prgcod || ' - ' || p.prgdsc) as descricao
			FROM monitora.programa p
			WHERE p.prgano = '".$_SESSION['exercicio']."' ORDER BY prgcod";
	$db->monta_combo('prgcod', $sql, 'S','Selecione','','','',400); 
	?>
	</td>
</tr>	
<tr>
	<td class="SubTituloDireita" align="right">Ação:</td>
	<td>
	<?
	$sql = "SELECT ac.acacod as codigo, (ac.acacod || ' - ' || ac.acadsc) as descricao
 			FROM monitora.acao ac 
 			WHERE ac.unicod = '".$_SESSION['monitora_var']['unicod']."' AND ac.prgano = '".$_SESSION['exercicio']."' AND ac.acastatus = 'A' AND ac.acasnrap = false  
 			GROUP BY ac.acacod, ac.acadsc 
 			ORDER BY ac.acacod";
	$db->monta_combo('acacod', $sql, 'S', 'Selecione','','','',400); 
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" align="right">Buscar:</td>
	<td>
	<? echo campo_texto('buscalivre', "N", "S", "", 67, 150, "", "", '', '', 0, '' ); ?>
	</td>
</tr>
<tr style="background-color: #cccccc">
	<td align='right' style="vertical-align:top; width:25%;">&nbsp;</td>
	<td>
	<input type="submit" name="botao" value="Pesquisar"/>
	<input type="button" name="botao" value="Todos" onclick="window.location='?modulo=principal/planotrabalho/listarPrograma&acao=A';"/>
	<input type="button" name="close" value="Fechar" onclick="window.close();">	
	</td>
</tr>		
</table>

</form>
<br>
<?php

$cabecalho = array ("", "PTRES", "Ação", "Unidade", "Dotação Autorizada", "Dotação SubAção", "Detalhado no PI", "Saldo Disponível" );
$db->monta_lista($sql_lista,$cabecalho,60,20,'','','');

?>
<script type="text/javascript">
/* CARREGANDO OS DADOS DE PTRES */
var tabelaorigem = window.opener.document.getElementById('orcamento');
for(i=2;i<tabelaorigem.rows.length-2;i++) {
	if(document.getElementById("chk_"+tabelaorigem.rows[i].cells[0].innerHTML)) {
		document.getElementById("chk_"+tabelaorigem.rows[i].cells[0].innerHTML).checked=true;
	}
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
	celula2.style.textAlign = "left";
	celula2.innerHTML = linhaTbl.cells[2].innerHTML+"<input type='hidden' name='acaid["+ptres+"]' value='"+acaid+"'>";

	var celula3 = tabelaorigem.rows[2].insertCell(2);
	celula3.style.textAlign = "right";
	celula3.innerHTML = linhaTbl.cells[4].innerHTML;
	
	var celula4 = tabelaorigem.rows[2].insertCell(3);
	celula4.style.textAlign = "right";
	celula4.innerHTML = linhaTbl.cells[5].innerHTML;
	
	var celula5 = tabelaorigem.rows[2].insertCell(4);
	celula5.style.textAlign = "right";
	celula5.innerHTML = "<a href=javascript:detfin('"+ptres+"')>"+linhaTbl.cells[6].innerHTML+"</a>";
	
	var celula6 = tabelaorigem.rows[2].insertCell(5);
	celula6.style.textAlign = "right";
	celula6.innerHTML = linhaTbl.cells[7].innerHTML;

	var celula7 = tabelaorigem.rows[2].insertCell(6);
	celula7.style.textAlign = "center";
	celula7.innerHTML = "<input type=\"text\" name=\"plivalor["+ptres+"]["+acaid+"]\" size=\"28\" maxlength=\"\" value=\"\" onKeyUp=\"this.value=mascaraglobal('###.###.###.###,##',this.value);calculovalorPI();\" onBlur=\"return verificaDisponivel(this,'"+ptres+"','0');\"  class=\"normal\"  onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" style=\"text-align : right; width:25ex;\" title='' />";



	} else {

	var tabelaorigem = window.opener.document.getElementById('orcamento');
	tabelaorigem.deleteRow(window.opener.document.getElementById('ptres_'+ptres).rowIndex);
	window.opener.calculovalorPI();

	}

}


</script>
</body>
</html>

