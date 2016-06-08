<?

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

set_time_limit(0);
ini_set("memory_limit","256M");


/*** Mês de Referência ***/
$mesRef = $_REQUEST["mesReferencia"];


/*** Unidade Orçamentária ***/
if($_REQUEST["unidadeOrcamentaria"] != null) {
	if($_REQUEST["unidadeOrcamentaria_campo_excludente"] != "1")
		$inUnidadeOrcamentaria = "and unicod in ('" . implode('\',\'',$_REQUEST["unidadeOrcamentaria"]) . "')";
	else
		$inUnidadeOrcamentaria = "and unicod not in ('" . implode('\',\'',$_REQUEST["unidadeOrcamentaria"]) . "')";
} 
else {
	$inUnidadeOrcamentaria = "";
}


/*** Grupo de Despesa ***/
if($_REQUEST["grupoDespesa"] != null) {
	if($_REQUEST["grupoDespesa_campo_excludente"] != "1")
		$inGrupoDespesa = "and gndcod in ('" . implode('\',\'',$_REQUEST["grupoDespesa"]) . "')";
	else
		$inGrupoDespesa = "and gndcod not in ('" . implode('\',\'',$_REQUEST["grupoDespesa"]) . "')";
} 
else {
	$inGrupoDespesa = "";
}


/*** Fonte de Recursos ***/
$inFonteRecursos = "";


/*** Projeto/Atividade ***/
if($_REQUEST["projetoAtividade"] != null) {
	if($_REQUEST["projetoAtividade_campo_excludente"] != "1")
		$inProjetoAtividade = "and acacod in ('" . implode('\',\'',$_REQUEST["projetoAtividade"]) . "')";
	else
		$inProjetoAtividade = "and acacod not in ('" . implode('\',\'',$_REQUEST["projetoAtividade"]) . "')";
} 
else {
	$inProjetoAtividade = "";
}


// SQL
$sql = "SELECT
			gr_unidade_orcamentaria,
			
       		(select sum(sldvalor)
       		   from siafi.saldo sld1
              where sldcontacontabil in ('195100000', '195400000', '292410510', '292410508', '295200000', '292410510', '295400000', '292410508', '295100000')
                and foncod like '0100%'
                ".$inUnidadeOrcamentaria."
                ".$inGrupoDespesa."
                ".$inProjetoAtividade."
                and ctecod = '3'
       			and sld1.unicod = uo.gr_unidade_orcamentaria
       		  group by unicod) as coluna1,
       		
       		(select sum(sldvalor)
       		   from siafi.saldo sld2
        	  where sldcontacontabil in ('195100000', '195400000', '292410510', '292410508', '295200000', '292410510', '295400000', '292410508', '295100000')
        		and foncod like '0100%'
        		".$inUnidadeOrcamentaria."
                ".$inGrupoDespesa."
                ".$inProjetoAtividade."
       			and ctecod = '4'
       			and sld2.unicod = uo.gr_unidade_orcamentaria
       		  group by unicod) as coluna2,
       		
       		(select sum(sldvalor)
       		   from siafi.saldo sld3
        	  where sldcontacontabil in ('195100000', '195400000', '292410510', '292410508', '295200000', '292410510', '295400000', '292410508', '295100000')
        		and foncod like '0112%'
        		".$inUnidadeOrcamentaria."
                ".$inGrupoDespesa."
                ".$inProjetoAtividade."
       			and ctecod = '3'
       			and sld3.unicod = uo.gr_unidade_orcamentaria
       		  group by unicod) as coluna3,
       		
			(select sum(sldvalor)
       		   from siafi.saldo sld4
        	  where sldcontacontabil in ('195100000', '195400000', '292410510', '292410508', '295200000', '292410510', '295400000', '292410508', '295100000')
        		and foncod like '0112%'
        		".$inUnidadeOrcamentaria."
                ".$inGrupoDespesa."
                ".$inProjetoAtividade."
       			and ctecod = '4'
       			and sld4.unicod = uo.gr_unidade_orcamentaria
       		  group by unicod) as coluna4,
       		  
			(select sum(sldvalor)
       		   from siafi.saldo sld5
        	  where sldcontacontabil in ('292130201', '292130202', '292130209', '292130301', '292410403', '292410402')
        		and foncod like '0100%'
        		".$inUnidadeOrcamentaria."
                ".$inGrupoDespesa."
                ".$inProjetoAtividade."
       			and ctecod = '3'
       			and sld5.unicod = uo.gr_unidade_orcamentaria
       		  group by unicod) as coluna5,
       		  
			(select sum(sldvalor)
       		   from siafi.saldo sld6
        	  where sldcontacontabil in ('292130201', '292130202', '292130209', '292130301', '292410403', '292410402')
        		and foncod like '0100%'
        		".$inUnidadeOrcamentaria."
                ".$inGrupoDespesa."
                ".$inProjetoAtividade."
       			and ctecod = '4'
       			and sld6.unicod = uo.gr_unidade_orcamentaria
       		  group by unicod) as coluna6,
       		  
			(select sum(sldvalor)
       		   from siafi.saldo sld7
        	  where sldcontacontabil in ('292130201', '292130202', '292130209', '292130301', '292410403', '292410402')
        		and foncod like '0112%'
        		".$inUnidadeOrcamentaria."
                ".$inGrupoDespesa."
                ".$inProjetoAtividade."
       			and ctecod = '3'
       			and sld7.unicod = uo.gr_unidade_orcamentaria
       		  group by unicod) as coluna7,
       		  
			(select sum(sldvalor)
       		   from siafi.saldo sld8
        	  where sldcontacontabil in ('292130201', '292130202', '292130209', '292130301', '292410403', '292410402')
        		and foncod like '0112%'
        		".$inUnidadeOrcamentaria."
                ".$inGrupoDespesa."
                ".$inProjetoAtividade."
       			and ctecod = '4'
       			and sld8.unicod = uo.gr_unidade_orcamentaria
       		  group by unicod) as coluna8,

       		(select sum(sldvalor)
       		   from siafi.saldo sld9
        	  where sldcontacontabil in ('112160400')
        		and foncod like '0100%'
        		".$inUnidadeOrcamentaria."
                ".$inGrupoDespesa."
                ".$inProjetoAtividade."
       			and ctecod = '3'
       			and sld9.unicod = uo.gr_unidade_orcamentaria
       		  group by unicod) as coluna9,			
       		
       		(select sum(sldvalor)
       		   from siafi.saldo sld10
        	  where sldcontacontabil in ('112160400')
        		and foncod like '0100%'
        		".$inUnidadeOrcamentaria."
                ".$inGrupoDespesa."
                ".$inProjetoAtividade."
       			and ctecod = '4'
       			and sld10.unicod = uo.gr_unidade_orcamentaria
       		  group by unicod) as coluna10,
       		  
			(select sum(sldvalor)
       		   from siafi.saldo sld11
        	  where sldcontacontabil in ('112160400')
        		and foncod like '0112%'
        		".$inUnidadeOrcamentaria."
                ".$inGrupoDespesa."
                ".$inProjetoAtividade."
       			and ctecod = '3'
       			and sld11.unicod = uo.gr_unidade_orcamentaria
       		  group by unicod) as coluna11,
			
       		(select sum(sldvalor)
       		   from siafi.saldo sld12
        	  where sldcontacontabil in ('112160400')
        		and foncod like '0112%'
        		".$inUnidadeOrcamentaria."
                ".$inGrupoDespesa."
                ".$inProjetoAtividade."
       			and ctecod = '4'
       			and sld12.unicod = uo.gr_unidade_orcamentaria
       		  group by unicod) as coluna12,
			
       		(select sum(sldvalor)
       		   from siafi.saldo sld13
        	  where sldcontacontabil in ('112160400')
        		and foncod like '0100%'
        		".$inUnidadeOrcamentaria."
                ".$inGrupoDespesa."
                ".$inProjetoAtividade."
       			and ctecod = '3'
       			and sld13.unicod = uo.gr_unidade_orcamentaria
       		  group by unicod) as coluna13,
			
       		(select sum(sldvalor)
       		   from siafi.saldo sld14
        	  where sldcontacontabil in ('112160400')
        		and foncod like '0100%'
        		".$inUnidadeOrcamentaria."
                ".$inGrupoDespesa."
                ".$inProjetoAtividade."
       			and ctecod = '4'
       			and sld14.unicod = uo.gr_unidade_orcamentaria
       		  group by unicod) as coluna14,
       		  
			(select sum(sldvalor)
       		   from siafi.saldo sld15
        	  where sldcontacontabil in ('112160400')
        		and foncod like '0112%'
        		".$inUnidadeOrcamentaria."
                ".$inGrupoDespesa."
                ".$inProjetoAtividade."
       			and ctecod = '3'
       			and sld15.unicod = uo.gr_unidade_orcamentaria
       		  group by unicod) as coluna15,
			
       		(select sum(sldvalor)
       		   from siafi.saldo sld16
        	  where sldcontacontabil in ('112160400')
        		and foncod like '0112%'
        		".$inUnidadeOrcamentaria."
                ".$inGrupoDespesa."
                ".$inProjetoAtividade."
       			and ctecod = '4'
       			and sld16.unicod = uo.gr_unidade_orcamentaria
       		  group by unicod) as coluna16,
       		
			(select sum(sldvalor)
       		   from siafi.saldo sld17
        	  where sldcontacontabil in ('112160400')
        		and foncod like '0100%'
        		".$inUnidadeOrcamentaria."
                ".$inGrupoDespesa."
                ".$inProjetoAtividade."
       			and ctecod = 'C'
       			and sld17.unicod = uo.gr_unidade_orcamentaria
       		  group by unicod) as coluna17,
			
			(select sum(sldvalor)
       		   from siafi.saldo sld18
        	  where sldcontacontabil in ('112160400')
        		and foncod like '0112%'
        		".$inUnidadeOrcamentaria."
                ".$inGrupoDespesa."
                ".$inProjetoAtividade."
       			and ctecod = 'C'
       			and sld18.unicod = uo.gr_unidade_orcamentaria
       		  group by unicod) as coluna18
			
		FROM
			siafi.uo uo";

//$result = $db->carregar($sql);

$result = array(
			array(
				"gr_unidade_orcamentaria" => "Testandoo",
				"coluna1" => "1111",
				"coluna2" => "2222",
				"coluna3" => "3333",
				"coluna4" => "4444",
			 	"coluna5" => "5555",
			 	"coluna6" => "6666",
				"coluna7" => "7777",
			 	"coluna8" => "8888",
				"coluna9" => "9999",
				"coluna10" => "1010",
				"coluna11" => "1111",
				"coluna12" => "1212",
				"coluna13" => "1313",
				"coluna14" => "1414",
				"coluna15" => "1515",
				"coluna16" => "1616",
				"coluna17" => "1717",
				"coluna18" => "1818"
			),
			array("coluna1" => "1111"),
			array("coluna2" => "2222"),
			array("coluna3" => "3333"),
			array("coluna4" => "4444"),
			array("coluna5" => "5555"),
			array("coluna6" => "6666"),
			array("coluna7" => "7777"),
			array("coluna8" => "8888"),
			array("coluna9" => "9999"),
			array("coluna10" => "1010"),
			array("coluna11" => "1111"),
			array("coluna12" => "1212"),
			array("coluna13" => "1313"),
			array("coluna14" => "1414"),
			array("coluna15" => "1515"),
			array("coluna16" => "1616"),
			array("coluna17" => "1717"),
			array("coluna18" => "1818")
			);
		//dbg($result);
?>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Expires" content="-1">
		<title>Apuração de Valores Financeiros a Repassar</title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
		<div id="aguarde" style="background-color:#ffffff;position:absolute;color:#000033;top:50%;left:30%;border:2px solid #cccccc; width:300px;">
			<center style="font-size:12px;"><br><img src="../imagens/wait.gif" border="0" align="absmiddle"> Aguarde! Gerando Relatório...<br><br></center>
		</div>
		<center>
		<table width="100%" align="center" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug"  style="border-bottom: 1px solid;">
			<tr bgcolor="#ffffff">
				<td valign="top" width="50" rowspan="2"><img src="../imagens/brasao.gif" width="45" height="45" border="0"></td>
				<td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
					SIMEC- Sistema Integrado do Ministério da Educação<br/>
					Acompanhamento da Execução Orçamentária<br/>
					MEC / SE - Secretaria Executiva <br />
					SPO - Subsecretaria de Planejamento e Orçamento
				</td>
				<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">
					Impresso por: <b><?= $_SESSION['usunome'] ?></b><br/>
					Hora da Impressão: <?= date( 'd/m/Y - H:i:s' ) ?><br />
					Orçamento Fiscal e Seg.Social - EM R$<br />
					Acumulado até:
				</td>
			</tr>
		</table>
		<br /><br />
		<center>ORÇAMENTO 2008<br />APURAÇÃO DE VALORES FINANCEIROS A REPASSAR</center>
		<br />
		<table class="tabela" border="1">
			<thead>
				<tr>
					<th rowspan="2">UNIDADE</th>
					<th colspan="4">RAP. PROCESSADOS<br />A PAGAR</th>
					<th colspan="4">EMPENHOS LIQUIDADOS<br />A PAGAR</th>
					<th colspan="4">LIMITE DE SAQUE<br />VINCULAÇÃO DE PAGAMENTO</th>
					<th colspan="4">VALORES A LIBERAR</th>
					<th colspan="2">TOTAL GERAL<br>A REPASSAR</th>
				</tr>
				<tr>
					<th>100-3-C</th>
					<th>100-4-D</th>
					<th>112-3-C</th>
					<th>112-4-D</th>
					
					<th>100-3-C</th>
					<th>100-4-D</th>
					<th>112-3-C</th>
					<th>112-4-D</th>
					
					<th>100-3-C</th>
					<th>100-4-D</th>
					<th>112-3-C</th>
					<th>112-4-D</th>
					
					<th>100-3-C</th>
					<th>100-4-D</th>
					<th>112-3-C</th>
					<th>112-4-D</th>
					
					<th>100-C-D</th>
					<th>112-C-D</th>
				</tr>
			</thead>
			<tbody>
				<?
					for($i=0; $i<count($result); $i++) {
						echo "<tr>
								<td>".$result[$i]["gr_unidade_orcamentaria"]."</td>
								<td>".$result[$i]["coluna1"]."</td>
								<td>".$result[$i]["coluna2"]."</td>
								<td>".$result[$i]["coluna3"]."</td>
								<td>".$result[$i]["coluna4"]."</td>
								<td>".$result[$i]["coluna5"]."</td>
								<td>".$result[$i]["coluna6"]."</td>
								<td>".$result[$i]["coluna7"]."</td>
								<td>".$result[$i]["coluna8"]."</td>
								<td>".$result[$i]["coluna9"]."</td>
								<td>".$result[$i]["coluna10"]."</td>
								<td>".$result[$i]["coluna11"]."</td>
								<td>".$result[$i]["coluna12"]."</td>
								<td>".$result[$i]["coluna13"]."</td>
								<td>".$result[$i]["coluna14"]."</td>
								<td>".$result[$i]["coluna15"]."</td>
								<td>".$result[$i]["coluna16"]."</td>
								<td>".$result[$i]["coluna17"]."</td>
								<td>".$result[$i]["coluna18"]."</td>
							</tr>";
					}
				?>
			</tbody>
		</table>
		</center>
		<script type="text/javascript" language="javascript">
			document.getElementById('aguarde').style.visibility = 'hidden';
			document.getElementById('aguarde').style.display = 'none';
		</script>
		</body>
	</html>