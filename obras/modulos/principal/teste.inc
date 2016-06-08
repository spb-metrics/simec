<?php

header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
header ( "Pragma: no-cache" );
header ( "Content-type: application/xls; name=teste.xls");
header ( "Content-Disposition: attachment; filename=teste.xls");
header ( "Content-Description: MID Gera excel" );

ini_set("memory_limit", "1024M");

$sql = "SELECT 
			oo.orgdesc as Tipo_Ensino,
			upper(ee.entnome) as Nome_Instituicao,
			ed.endcep as CEP_Da_Obra,
			upper(ed.endlog) as Logradouro_Da_Obra,
			upper(ed.endbai) as Bairro_Da_Obra,
			ed.muncod as Cod_IBGE,
			CASE WHEN ed.muncod <> '' THEN tm.mundescricao ELSE 'Não Informado' END as Nome_Cidade,
			ed.estuf as uf,
			case when trim(ed.medlatitude) <> '' then upper(ed.medlatitude) else 'Não Informado' end as latidude,
			case when trim(ed.medlongitude) <> '' then upper(ed.medlongitude) else 'Não Informado' end as longitude,
			upper(oi.obrdesc) as Nome_Da_Obra 
		
		FROM
			obras.obrainfraestrutura AS oi 
		INNER JOIN
			entidade.entidade AS ee ON oi.entidunidade = ee.entid
		LEFT JOIN
			entidade.endereco AS ed ON oi.endid = ed.endid
		LEFT JOIN
			territorios.municipio AS tm ON ed.muncod = tm.muncod
		INNER JOIN
			obras.orgao AS oo ON oi.orgid = oo.orgid
		WHERE
			oi.obsstatus = 'A' AND
			ee.entstatus = 'A' AND
			--ed.endstatus = 'A' AND
			ed.endcep != '' AND
			ed.endlog != '' AND
			ed.endbai != '' AND
			oi.stoid = 1 AND
			( tm.mundescricao ilike '%Rio de Janeiro%' OR 
			  tm.mundescricao ilike '%São Paulo%' OR
			  tm.mundescricao ilike '%Salvador%' OR
			  tm.mundescricao ilike '%Porto Alegre%' OR
			  tm.mundescricao ilike '%Belo Horizonte%') AND
			oi.tobraid <> 4 
		ORDER BY
			oo.orgdesc, ee.entnome";

	$dados = $db->carregar($sql);

	print '<table border="1" style="font-size: 11px;">'
		. '<tr>'
		. '<td>Tipo de Ensino</td>'
		. '<td>Nome da Unidade</td>'
		. '<td>CEP</td>'
		. '<td>Logradouro</td>'
		. '<td>Bairro</td>'
		. '<td>Cód. IBGE</td>'
		. '<td>Cidade</td>'
		. '<td>Estado</td>'
		. '<td>Latitude</td>'
		. '<td>Longitude</td>'
		. '<td>Nome da Obra</td>'
		. '</tr>';
	
	for ( $i = 0; $i < count($dados); $i++ ){
		
		if ($dados[$i]["latidude"] != "Não Informado" && $dados[$i]["longitude"] != "Não Informado"){
			
			$latitude = explode(".", $dados[$i]["latidude"]);
			
			$graulatitude = $latitude[0];
			$minlatitude  = $latitude[1];
			$seglatitude  = $latitude[2];
			$pololatitude = $latitude[3];
			
			$polo 		 = $pololatitude == "S" ? "-" : ""; 
			$resLatitude = ($seglatitude / 60) + ($minlatitude / 60 ) + $graulatitude;
			
			$dadoLatitude = $polo . $resLatitude;
			
			$longitude = explode(".", $dados[$i]["longitude"]);
			
			$graulongitude = $longitude[0];
			$minlongitude  = $longitude[1];
			$seglongitude  = $longitude[2];
			
			$resLongitude = ($seglongitude / 60) + ($minlongitude / 60 ) + $graulongitude;
			
			$dadoLongitude = "-" . $resLongitude;
			
			print "<tr>"
				. "<td>{$dados[$i]["tipo_ensino"]}</td>"
				. "<td>{$dados[$i]["nome_instituicao"]}</td>"
				. "<td>{$dados[$i]["cep_da_obra"]}</td>"
				. "<td>{$dados[$i]["logradouro_da_obra"]}</td>"
				. "<td>{$dados[$i]["bairro_da_obra"]}</td>"
				. "<td>{$dados[$i]["cod_ibge"]}</td>"
				. "<td>{$dados[$i]["nome_cidade"]}</td>"
				. "<td>{$dados[$i]["uf"]}</td>"
				. "<td>{$dadoLatitude}</td>"
				. "<td>{$dadoLongitude}</td>"
				. "<td>{$dados[$i]["nome_da_obra"]}</td>"
				. "</tr>";
		
			
		}else{
			$dadoLatitude = "Não Informado";
			$dadoLongitude = "Não Informado";
		}
		
		
	}
	
	print "</table>";
	
?>