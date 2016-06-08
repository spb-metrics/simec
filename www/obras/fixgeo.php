<?php

/* configurações */
ini_set("memory_limit", "3000M");
set_time_limit(30000);
/* FIM configurações */

$_REQUEST['baselogin'] = "simec_espelho_producao";
//$_REQUEST['baselogin'] = "simec_desenvolvimento";

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once '_constantes.php';


// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "select obrid,mundescricao, estuf, munmedraio, medlatitude, medlongitude, latm, logm, lato, endid, logo, distanciaPontosGPS(latm,logm,lato,logo) as distancia,munmedlat,munmedlog
from (
SELECT mun.mundescricao,mun.munmedlat,mun.munmedlog, mun.estuf, ed.endid, CASE WHEN (length (mun.munmedlat)=8) THEN 
											CASE WHEN length(REPLACE('0' || mun.munmedlat,'S','')) = 8 THEN
												((SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),1,2)::double precision))*(-1)
											ELSE
												(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),1,2)::double precision)
											END
										ELSE
											CASE WHEN length(REPLACE(mun.munmedlat,'S','')) = 8 THEN
												((SUBSTR(REPLACE(mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),1,2)::double precision))*(-1)
											ELSE
												0--((SUBSTR(REPLACE(mun.munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(mun.munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE(mun.munmedlat,'N',''),1,2)::double precision))
											END
										END as latm, 
										(SUBSTR(REPLACE(mun.munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(mun.munmedlog,'W',''),3,2)::double precision/60)) *(-1) as logm,
										CASE 
										WHEN (medlatitude is not null AND medlatitude <> '...S') THEN 
											CASE WHEN (SPLIT_PART(medlatitude, '.', 1) <>'' AND SPLIT_PART(medlatitude, '.', 2) <>'' AND split_part(medlatitude, '.', 3) <>'') THEN 
											       CASE WHEN split_part(medlatitude, '.', 4) <>'N' THEN
												   (((split_part(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int)))*(-1)
												ELSE
												   ((SPLIT_PART(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int))
											       END
											END
										ELSE
											CASE WHEN (length (medlatitude)=8) THEN 
												CASE WHEN length(REPLACE('0' || medlatitude,'S','')) = 8 THEN
												    ((SUBSTR(REPLACE('0' || medlatitude,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || medlatitude,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || medlatitude,'S',''),1,2)::double precision))*(-1)
												ELSE
												    (SUBSTR(REPLACE('0' || medlatitude,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || medlatitude,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || medlatitude,'N',''),1,2)::double precision)
												END
											ELSE
												CASE WHEN length(REPLACE(medlatitude,'S','')) = 8 THEN
												   ((SUBSTR(REPLACE(medlatitude,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(medlatitude,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(medlatitude,'S',''),1,2)::double precision))*(-1)
												ELSE
												  0--((SUBSTR(REPLACE(medlatitude,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(medlatitude,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE(medlatitude,'N',''),1,2)::double precision))
												END
											END 

										END as lato,
										split_part(medlongitude, '.', 3) as xx,
										SPLIT_PART(medlongitude, '.', 2) as xxx,
										SPLIT_PART(medlongitude, '.', 1) as xxxx,
										CASE WHEN (medlongitude is not null AND medlongitude <> '...W' AND medlongitude <> '..' ) THEN 
											((split_part(medlongitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlongitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlongitude, '.', 1)::int))*(-1)  END as logo,

										
				CAST(munmedraio as integer)*1000 as munmedraio,
				medlatitude,
				CASE 
					WHEN (medlatitude is not null AND medlatitude <> '...S') THEN 
						CASE WHEN (SPLIT_PART(medlatitude, '.', 1) <>'' AND SPLIT_PART(medlatitude, '.', 2) <>'' AND split_part(medlatitude, '.', 3) <>'') THEN 
					               CASE WHEN split_part(medlatitude, '.', 4) <>'N' THEN
					                   (((split_part(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int)))*(-1)
					                ELSE
					                   ((SPLIT_PART(medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(medlatitude, '.', 1)::int))
					               END
						END
					ELSE
						CASE WHEN (length (medlatitude)=8) THEN 
					                CASE WHEN length(REPLACE('0' || medlatitude,'S','')) = 8 THEN
					                    ((SUBSTR(REPLACE('0' || medlatitude,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || medlatitude,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || medlatitude,'S',''),1,2)::double precision))*(-1)
					                ELSE
					                    (SUBSTR(REPLACE('0' || medlatitude,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || medlatitude,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || medlatitude,'N',''),1,2)::double precision)
					                END
					        ELSE
					                CASE WHEN length(REPLACE(medlatitude,'S','')) = 8 THEN
					                   ((SUBSTR(REPLACE(medlatitude,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(medlatitude,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(medlatitude,'S',''),1,2)::double precision))*(-1)
					                ELSE
					                  0--((SUBSTR(REPLACE(medlatitude,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(medlatitude,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE(medlatitude,'N',''),1,2)::double precision))
					                END
					        END 

				END as la,
				medlongitude, 
				(SUBSTR(REPLACE(medlongitude,'W',''),1,2)::double precision + (SUBSTR(REPLACE(medlongitude,'W',''),3,2)::double precision/60))*(-1) as lo,
				(SUBSTR(REPLACE(mun.munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(mun.munmedlog,'W',''),3,2)::double precision/60)) *(-1) as lol,
				obrid

			
				FROM obras.obrainfraestrutura ob
			INNER JOIN 
				entidade.endereco ed ON ob.endid = ed.endid
			INNER JOIN 
				territorios.municipio mun ON ed.muncod = mun.muncod
			WHERE ob.obsstatus = 'A' AND medlatitude!='' AND medlongitude!='' AND medlongitude not ilike '%NaN%' and split_part(medlongitude, '.', 3)!='') as foo 
			where distanciaPontosGPS(latm,logm,lato,logo)>munmedraio";

$dados = $db->carregar($sql);

if($dados[0]) {
	echo "<pre>";
	print_r($dados);
	foreach($dados as $key => $d) {
		if(substr($d['latm'],0,1)=="-") {
			
			$grauslat = ceil($d['latm'])*-1;
			$pololat = "S";
			$resto = ($d['latm']+$grauslat)*-1;
			$ml = $resto*60;
			$minlat = floor($ml);
			$resto2 = $ml-$minlat;
			$seglat = ceil($resto2*60);
			
			
		} else {
			
			$grauslat = floor($d['latm']);
			$pololat = "N";
			$resto = $d['latm']-$grauslat;
			$ml = $resto*60;
			$minlat = floor($ml);
			$resto2 = $ml-$minlat;
			$seglat = ceil($resto2*60); 
			
		}
		
		$grauslog = ceil($d['logm'])*-1;
		$pololog = "W";
		$resto = ($d['logm']+$grauslog)*-1;
		
		$ml = $resto*60;
		$minlog = floor($ml);
		if(substr($minlog,0,1)=="-") $minlog = ($minlog*-1);
		$resto2 = $ml-$minlog;
		$seglog = ceil($resto2*60); 
		
		/*
		echo "G".$grauslat."<br>";
		echo "P".$pololat."<br>";
		echo "R".$resto."<br>";
		echo "M".$minlat."<br>";
		echo "MM".$ml."<br>";
		echo "RR".$resto2."<br>";
		echo "S".$seglat."<br>";
		*/
		echo "-- C".($key+1)."<br>";
		echo "UPDATE entidade.endereco SET medlatitude='".$grauslat.".".$minlat.".".$seglat.".".$pololat."', medlongitude='".$grauslog.".".$minlog.".".$seglog.".".$pololog."' where endid='".$d['endid']."';<br>";
 
	}
	exit;
}


?>