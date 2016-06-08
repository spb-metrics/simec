<?php

include_once(APPRAIZ.'includes/classes/MontaListaAjax.class.inc');

if($_POST['salvar']) {
	
	if($_REQUEST['pliid']) {
		foreach($_REQUEST['pliid'] as $p) {
			if($p) $db->executar("DELETE FROM monitora.pi_obra WHERE pliid='".$p."' AND obrid='".$_REQUEST['obrid']."'");
		}
	}
	
	if($_REQUEST['plicod']) {
		foreach($_REQUEST['plicod'] as $plicod) {
			
			$sql = "SELECT pliid FROM monitora.pi_planointerno WHERE plicod='".$plicod."' and pliano = '".$_REQUEST['anoexercicio']."' ";
			$pliid = $db->pegaUm($sql);
			
			if(!$pliid) {
			
				$sql = "INSERT INTO monitora.pi_planointerno(
			            mdeid, eqdid, neeid, capid, sbaid, obrid, plisituacao, 
			            plititulo, plidata, plistatus, plicodsubacao, plicod, plilivre, 
			            plidsc, usucpf, unicod, ungcod, pliano)
					    VALUES (".(($_REQUEST['mdeid'][$plicod])?"'".$_REQUEST['mdeid'][$plicod]."'":"NULL").", 
					    		".(($_REQUEST['eqdid'][$plicod])?"'".$_REQUEST['eqdid'][$plicod]."'":"NULL").", 
					    		".(($_REQUEST['neeid'][$plicod])?"'".$_REQUEST['neeid'][$plicod]."'":"NULL").", 
					    		".(($_REQUEST['capid'][$plicod])?"'".$_REQUEST['capid'][$plicod]."'":"NULL").", 
					    		".(($_REQUEST['sbaid'][$plicod])?"'".$_REQUEST['sbaid'][$plicod]."'":"NULL").",
					    		'".$_REQUEST['obrid']."',
					    		'C', 
					            ".(($_REQUEST['plititulo'][$plicod])?"'".$_REQUEST['plititulo'][$plicod]."'":"NULL").", 
					            NOW(), 
					            'A', 
					            ".(($_REQUEST['sbacod'][$plicod])?"'".$_REQUEST['sbacod'][$plicod]."'":"NULL").", 
					            '".$plicod."', 
					            ".(($_REQUEST['plilivre'][$plicod])?"'".$_REQUEST['plilivre'][$plicod]."'":"NULL").", 
					            ".(($_REQUEST['plidsc'][$plicod])?"'".$_REQUEST['plidsc'][$plicod]."'":"NULL").", 
					            '".$_SESSION['usucpf']."', 
					            '".$_REQUEST['unicod']."', 
					            NULL, 
					            '".$_REQUEST['anoexercicio']."') RETURNING pliid;";

				$pliid = $db->pegaUm($sql);
				
					$sql = "select pt.ptrid, pi.plicod, pt.ptres, sum(rofempenhado) as pipvalor
								from monitora.pi_planointerno pi 
							inner join ( select plicod, rofano, ptres, rofempenhado from financeiro.execucao where plicod = '".$plicod."' and  rofano ='".$_REQUEST['anoexercicio']."' and ( trim(gescod) = '".$_REQUEST['unicod']."' or unicod = '".ADM_UNICOD."' ) ) e ON pi.plicod = e.plicod
							inner join monitora.ptres pt on pt.ptrano=e.rofano and pt.ptres=e.ptres
								group by pi.plicod, pt.ptres, pt.ptrid
							order by plicod";
				
				$ptres = $db->carregar($sql);
				if($ptres[0]) {
					foreach($ptres as $pt) {
						$sql = "INSERT INTO monitora.pi_planointernoptres(pliid, ptrid, pipvalor) VALUES ('".$pliid."', '".$pt['ptrid']."', '".$pt['pipvalor']."');";
						$db->executar($sql);
					}
				}
			
			}
			$db->executar("INSERT INTO monitora.pi_obra(pliid, obrid) VALUES ('".$pliid."', '".$_REQUEST['obrid']."');");
			
		}
		$db->commit();
	}
	
	die("<script>
			alert('PIs vinculado com sucesso');
			window.opener.location.replace(window.opener.location);
			window.close();
		 </script>");
	
	
}
$sql = "SELECT entunicod FROM obras.obrainfraestrutura obr
		LEFT JOIN entidade.entidade ent ON ent.entid = obr.entidunidade 
		WHERE obrid='".$_REQUEST['obrid']."'";

$unicod = $db->pegaUm($sql);

if($_REQUEST['plititulo']) {
	$filtro[] = "e.plidsc ilike '%".$_REQUEST['plititulo']."%'";
}

if($_REQUEST['plicod']) {
	$filtro[] = "e.plicod ilike '%".$_REQUEST['plicod']."%'";
}

if($_REQUEST['enquadramento']) {
	$filtro[] = "eqd.eqdid = '".$_REQUEST['enquadramento']."'";
}

if($_REQUEST['nivel']) {
	$filtro[] = "n.neeid = '".$_REQUEST['nivel']."'";
}

if($_REQUEST['apropriacao']) {
	$filtro[] = "c.capid = '".$_REQUEST['apropriacao']."'";
}

if ( possuiPerfil( array( PERFIL_SUPERVISORMEC, PERFIL_GESTORMEC, PERFIL_ADMINISTRADOR )  ) ) {
	$filtro_direta = " ( trim(e.gescod) = '".$unicod."' or ( e.unicod = '".ADM_UNICOD."' and substr(trim(e.plicod),2,4) in ( 'PP02','PP03','PP09','SS10','SS14','SS16' ) ))";
}
elseif ( possuiPerfil( array( PERFIL_SUPERVISORUNIDADE, PERFIL_GESTORUNIDADE )  )) {
	$filtro_direta = " trim(e.gescod) = '".$unicod."'";
}


?>
<script language="JavaScript" src="../../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<form name="formulario2" id="formulario2" method="post">
<input type="hidden" name="obrid" value="<? echo $_REQUEST['obrid']; ?>">
<input type="hidden" name="anoexercicio" value="<? echo $_REQUEST['anoexercicio']; ?>">

<table class="listagem" width="100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"	align="center">
<tr>
	<td class="SubTituloDireita">Código do PI</td>
	<td>
		<?php 
			$plicod = $_REQUEST['plicod'];
			echo campo_texto( 'plicod', 'N', 'S', '', 20, 15, '', '', 'left', '', 0);
		?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita">Título</td>
	<td>
		<?php
			$plititulo = $_REQUEST['plititulo'];
			echo campo_texto( 'plititulo', 'N', 'S', '', 65, 60, '', '', 'left', '', 0);
		?>
	</td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita">Enquadramento da Despesa:</td>
    <td>
		<?php
			$enquadramento = $_REQUEST['enquadramento'];
			$sql = "SELECT eqdid as codigo, eqdcod ||' - '|| eqddsc as descricao FROM monitora.pi_enquadramentodespesa WHERE eqdano = '".$_REQUEST['anoexercicio']."' ORDER BY eqdcod";  
			$db->monta_combo('enquadramento', $sql, 'S',  'Selecione', '', '', '', '300', 'N'); 
		?>    	
    </td>
</tr>
<tr>
	<td class='SubTituloDireita'>Categoria de Apropriação:</td>
	<td>
		<?php
			$apropriacao = $_REQUEST['apropriacao'];
			$sql = "SELECT capid as codigo, capcod ||' - '|| capdsc as descricao FROM monitora.pi_categoriaapropriacao WHERE capano = '".$_REQUEST['anoexercicio']."' ORDER BY capcod";
			$db->monta_combo('apropriacao', $sql, 'S', 'Selecione', '', '', '', '300', 'N', '');
		?>
	</td>
</tr>	
<tr bgcolor="#C0C0C0">
	<td colspan="2">
		<div style="float: right;">
			<input type="submit" value="Pesquisar" style="cursor: pointer;"/>
			<input type="button" value="Ver Todos" style="cursor: pointer;" onclick="location.href='obras.php?modulo=principal/vincular_pi&acao=A&obrid=<? echo $_REQUEST['obrid']; ?>&anoexercicio=<? echo $_REQUEST['anoexercicio']; ?>';"/>
		</div>
	</td>
</tr>
</table>

</form>



<form name="formulario" id="formulario" method="post">
<input type="hidden" name="salvar" value="1">
<input type="hidden" name="obrid" value="<? echo $_REQUEST['obrid']; ?>">
<input type="hidden" name="unicod" value="<? echo $unicod; ?>">
<input type="hidden" name="anoexercicio" value="<? echo $_REQUEST['anoexercicio']; ?>">
<?php 



$sql = "select '<input type=\'checkbox\' name=\'plicod[]\' value=\''||e.plicod||'\' '|| CASE WHEN ( SELECT pl.pliid FROM monitora.pi_planointerno pl 
																								  INNER JOIN monitora.pi_obra o ON pl.pliid = o.pliid  
																								  WHERE pl.plistatus='A' AND pl.pliano = '".$_REQUEST['anoexercicio']."' AND pl.plisituacao='C' AND pl.plicod=e.plicod AND o.obrid=".$_REQUEST['obrid'].")  IS NULL THEN '' ELSE 'checked' END ||'>
			    <input type=\'hidden\' name=\'mdeid['||e.plicod||']\' value=\''||COALESCE(m.mdeid, 0)||'\'>
			    <input type=\'hidden\' name=\'eqdid['||e.plicod||']\' value=\''||COALESCE(eqd.eqdid, 0)||'\'>
			    <input type=\'hidden\' name=\'neeid['||e.plicod||']\' value=\''||COALESCE(n.neeid, 0)||'\'>
			    <input type=\'hidden\' name=\'capid['||e.plicod||']\' value=\''||COALESCE(c.capid, 0)||'\'>
			    <input type=\'hidden\' name=\'sbaid['||e.plicod||']\' value=\''||COALESCE(s.sbaid, 0)||'\'>
			    <input type=\'hidden\' name=\'plilivre['||e.plicod||']\' value=\''|| COALESCE(substr(e.plicod,9,3),'') ||'\'>
			    <input type=\'hidden\' name=\'sbacod['||e.plicod||']\' value=\''||COALESCE(substr(trim(e.plicod),2,4),'')||'\'>
			    <input type=\'hidden\' name=\'plidsc['||e.plicod||']\' value=\''||COALESCE(e.plidsc,'')||'\'>
			    <input type=\'hidden\' name=\'plititulo['||e.plicod||']\' value=\''||COALESCE(e.plidsc,'')||'\'>
			    <input type=\'hidden\' name=\'pliid[]\' value=\''|| COALESCE( (SELECT CAST(pl.pliid AS varchar) as pliid FROM monitora.pi_planointerno pl 
																	INNER JOIN monitora.pi_obra o ON pl.pliid = o.pliid  
																	WHERE pl.plistatus='A' AND pl.pliano = '".$_REQUEST['anoexercicio']."' AND pl.plisituacao='C' AND pl.plicod=e.plicod AND o.obrid=".$_REQUEST['obrid'].") , '') ||'\'>' as chk,
		e.plicod, 
		e.plidsc, 
		sum(rofempenhado) 
		from financeiro.execucao e 
		left join monitora.pi_planointerno pl on pl.plicod = e.plicod 
		left join monitora.pi_obra ob on ob.pliid = pl.pliid 
		left join ( select eqdcod, eqdid from monitora.pi_enquadramentodespesa where eqdano = '".$_REQUEST['anoexercicio']."' ) eqd ON eqd.eqdcod = substr(trim(e.plicod),1,1) 
		left join ( select sbaid, sbacod from monitora.pi_subacao where pieid is not null ) s ON s.sbacod = substr(trim(e.plicod),2,4) 
		left join ( select neeid, neecod from monitora.pi_niveletapaensino where neeano = '".$_REQUEST['anoexercicio']."' ) n on n.neecod = substr(trim(e.plicod),6,1) 
		left join ( select capid, capcod from monitora.pi_categoriaapropriacao where capano = '".$_REQUEST['anoexercicio']."' ) c ON c.capcod = substr(trim(e.plicod),7,2) 
		left join ( select mdeid, mdecod from monitora.pi_modalidadeensino where mdeano = '".$_REQUEST['anoexercicio']."' ) m ON m.mdecod = substr(trim(e.plicod),11,1) 
		where e.plicod not in ( select plicod from monitora.pi_planointerno pl inner join monitora.pi_obra ob on ob.pliid = pl.pliid where pl.unicod = '".$unicod."' and pliano = '".$_REQUEST['anoexercicio']."' ) AND
		(ob.obrid IS NULL OR ob.obrid=".$_REQUEST['obrid'].") AND rofano ='".$_REQUEST['anoexercicio']."' and {$filtro_direta} and trim(e.plicod) <> '' 
		".(($filtro)?" AND ".implode(" AND ", $filtro):"")."
		group by m.mdeid, eqd.eqdid, n.neeid, c.capid, s.sbaid, 
				e.plidsc, e.plicod, substr(e.plicod,9,3) 
		order by e.plicod";

$cabecalho = array("","Código do PI","Título","Empenhado(R$)");
//$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);

$obMontaListaAjax = new MontaListaAjax($db, true);
$obMontaListaAjax->montaLista($sql, $cabecalho,50, 10, 'S', '', '', '', '', '', '', '' );

?>
<br/>
<input type="submit" name="vincular_pi" value="Salvar">
</form>
