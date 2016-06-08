<html>
  <head>
    <meta http-equiv="content-type" content="text/html; charset=iso-8859-1"/>
    
    <title>Mapa de Obras</title>
    <?
    	if(!$_REQUEST["obrid"]) {
    		die("<script>
    				alert('Obra não identificada');
    				window.close();
    			 </script>");
    	}
		// Carrega os dados das obras
   		$IdObra = $_REQUEST["obrid"];
		
	if ( empty( $IdObra ) ){
		print "<script>"
			. "		alert('A sessão da obra escolhida expirou!');"
			. "		window.location='/obras/obras.php?modulo=inicio&acao=A';"
			. "</script>";
	}
   		
    	// Parâmetros para o Sildeshow selecionar as imagens da obra.
		$_SESSION['imgparams'] = array("filtro" => "cnt.obrid=".$IdObra." AND aqostatus = 'A'", "tabela" => "obras.arquivosobra");
		$sql="
			SELECT 
				oi.obrid as id_obra,    
				UPPER( oi.obrdesc ) as nome_obra,
				sto.stodesc as situacao,
	            obrcomposicao, 
				REPLACE(oi.obrpercexec||'','.',',') || ' %'  as percentual,
				oi.obrcustocontrato as valor,
				obrdtinicio as dtinicio,
				obrdttermino as dtfim,
				ed.estuf,
				umdeesc as unidademedida,
				obrqtdconstruida,
				mun.mundescricao, 
                et.entnome as unidade
			FROM 
				obras.obrainfraestrutura oi 
			INNER JOIN 
				entidade.entidade et ON oi.entidunidade = et.entid 
			LEFT JOIN 
				-- entidade.endereco ed ON ed.entid = et.entid
				entidade.endereco ed ON oi.endid = ed.endid
			LEFT JOIN 
				territorios.municipio mun ON mun.muncod = ed.muncod
			LEFT JOIN 
				obras.unidademedida umd ON oi.umdidobraconstruida = umd.umdid
			LEFT JOIN 
				obras.situacaoobra sto ON oi.stoid = sto.stoid
			INNER JOIN 
				obras.orgao org ON oi.orgid = org.orgid
			 WHERE  oi.obrid = $IdObra
			GROUP BY 
                oi.obrid,oi.obrdesc, sto.stodesc, obrcomposicao,oi.obrpercexec, oi.obrcustocontrato,  et.entnome,obrdtinicio,obrdttermino,ed.estuf,mun.mundescricao,umdeesc,obrqtdconstruida";
//			dbg ($sql,1);
			$dados = $db->carregar($sql);			   
	
			if($dados){
				for($i=0;$i < count($dados);$i++){
					$id_obra=$dados[$i]["id_obra"];
					// Fotos
					$imagens="";
						$sql = "SELECT arqnome, arq.arqid, arq.arqdescricao, to_char(arq.arqdata, 'dd/mm/yyyy') as arqdata 
									FROM public.arquivo arq
									INNER JOIN obras.arquivosobra oar ON arq.arqid = oar.arqid
									INNER JOIN obras.obrainfraestrutura obr ON obr.obrid = oar.obrid 
									INNER JOIN seguranca.usuario seg ON seg.usucpf = oar.usucpf 
								WHERE obr.obrid = $IdObra AND
									  aqostatus = 'A' AND
									  (arqtipo = 'image/jpeg' OR 
									   arqtipo = 'image/gif' OR 
									   arqtipo = 'image/png') 
								ORDER BY arq.arqid DESC LIMIT 3";
					$fotos = ($db->carregar($sql));
					if ($fotos){
						for($i=0;$i < count($fotos);$i++){
							$imagens_fotos.="<img src='../../slideshow/slideshow/verimagem.php?newwidth=80&newheight=60&arqid=".$fotos[$i]["arqid"]."' hspace='0' vspace='2' style='width:80px; height:60px;' onClick='abrir_fotos(".$fotos[$i]["arqid"].")'> <br> ".$fotos[$i]["arqdata"]." <br>".$fotos[$i]["arqdescricao"]." <br>";										
						}
					}
				}			
			}
			?>
<style>
	body {
	BORDER-BOTTOM: 0px; BORDER-LEFT: 0px; BORDER-RIGHT: 0px; BORDER-TOP: 0px}
</style>
<script type="text/javascript">
function abrir(){
	janela=parent.window.location.href='/obras/obras.php?modulo=principal/cadastro&acao=A&obrid=<?= $IdObra;?>';
	janela.focus();
}
function abrir_fotos(foto){
	window.open("../slideshow/slideshow/index.php?pagina=&amp;arqid="+foto,"imagem","width=850,height=600,resizable=yes")
}

</script>
  </head>
  <body>
	<table class=tabela width=300>
	<tr>
		<td valign="top">
		<b><font size=1 face=arial><?= $dados[0]["unidade"] ?></font></b>
		<br><b><font color=blue size=1 face=arial><a href=# onClick="javascript: abrir();"><?=$dados[0]["nome_obra"]; ?></a></font></b>
		<br><font size=1 face=arial color=#660000><?= "Localização: ".$dados[0]["mundescricao"]."/".$dados[0]["estuf"]; ?></font>
		<br><font size=1 face=arial color=#660000><?= "Situação: ".$dados[0]["situacao"]; ?></font>
		<br><font size=1 face=arial color=#660000> Área Construída: <?= number_format($dados[0]["obrqtdconstruida"],2,",",".")." ".$dados[0]["unidademedida"] ?></font>
		<br><font size=1 face=arial color=#660000> Data de Início: <?= $dados[0]["dtinicio"] ? date("d/m/Y",strtotime($dados[0]["dtinicio"])) : "N/A" ?></font>
		<br><font size=1 face=arial color=#660000> Data de Término: <?= $dados[0]["dtfim"] ? date("d/m/Y",strtotime($dados[0]["dtfim"])) : "N/A" ?></font>
		<br><font size=1 face=arial color=#FFCC33> Executado: <?= $dados[0]["percentual"] ?></font>
		<br><font size=1 face=arial color=#3399FF> Valor: R$ <?= number_format($dados[0]["valor"] ,2,',','.');?></font>
		<br><br><font size=1 face=arial><?=$dados[0]["obrcomposicao"]; ?></font></td>
		<td colspan=2 valign="top"><font size=1 face=arial><?=$imagens_fotos; ?></font></td>
		</tr>
	</table>	
  </body>
</html>
