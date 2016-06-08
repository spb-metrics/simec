<?php

function getmicrotime()
{list($usec, $sec) = explode(" ", microtime());
 return ((float)$usec + (float)$sec);} 

date_default_timezone_set ('America/Sao_Paulo');

$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações */

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "SELECT * FROM educacenso_2010.tb_escola_inep_2010 WHERE situacao='EM ATIVIDADE'";
$escolas = $db->carregar($sql);

echo "Começou:".date("d/mY h:i:s");
$atualizouent=0;
$atualizouend=0;
$inseriuent=0;
$inseriuend=0;

try {

	if($escolas[0]) {
		foreach($escolas as $esc) {
			
			$sql = "SELECT e.entid, en.endid FROM entidade.entidade e 
					LEFT JOIN entidade.endereco en ON en.entid = e.entid AND en.tpeid=1 
					WHERE entcodent='".$esc['pk_cod_entidade']."'";
			
			$escola_l = $db->pegaLinha($sql);
			
			if($escola_l) {
				
				$sql = "UPDATE entidade.entidade SET entnome='".$esc['no_escola']."', entstatus='A' WHERE entid='".$escola_l['entid']."'";
				$db->executar($sql);
				$atualizouent++;
				
				if($escola_l['endid']) {
					
					$sql = "UPDATE entidade.endereco
							   SET endcep='".$esc['num_cep']."', endlog='".$esc['desc_endereco']."', endcom='".$esc['desc_endereco_complemento']."', 
							   	   endbai='".$esc['desc_endereco_bairro']."', 
							       muncod='".$esc['co_municipio']."', estuf='".$esc['sg_uf']."', endnum='".$esc['num_endereco']."'
							 WHERE endid='".$escola_l['endid']."'";
					
					$db->executar($sql);
					$atualizouend++;
					
				} else {
					
					$sql = "INSERT INTO entidade.endereco(
	            			entid, tpeid, endcep, endlog, endcom, endbai, muncod, 
	            			estuf, endnum, endstatus, medlatitude, medlongitude, endcomunidade, 
	            			endzoom, endpc)
	    					VALUES ('".$escola_l['entid']."', '1', '".$esc['num_cep']."', '".$esc['desc_endereco']."', '".$esc['desc_endereco_complemento']."', 
	    					'".$esc['desc_endereco_bairro']."', '".$esc['co_municipio']."', 
	            			'".$esc['sg_uf']."', '".$esc['num_endereco']."', 'A');";
					
					$db->executar($sql);
					$inseriuend++;
				}
				
			} else {
				
				switch($esc['tp_dependencia']) {
					case 'ESTADUAL':
						$tpcid = '1';
						break;
					case 'FEDERAL':
						$tpcid = '2';
						break;
					case 'MUNICIPAL':
						$tpcid = '3';
						break;
					case 'PRIVADA':
						$tpcid = '4';
						break;
						
						
				}
				
				$sql = "INSERT INTO entidade.entidade(
				            entnome, entcodent, tpcid, tpsid, entstatus)
				    	VALUES ('".$esc['no_escola']."', '".$esc['pk_cod_entidade']."', $tpcid, '1', 'A') RETURNING entid;";
				
				$entid = $db->pegaUm($sql);
				$inseriuent++;
				
				$sql = "INSERT INTO entidade.endereco(
	            		entid, tpeid, endcep, endlog, endcom, endbai, muncod, 
	            		estuf, endnum, endstatus)
	    				VALUES ('".$entid."', '1', '".$esc['num_cep']."', '".$esc['desc_endereco']."', '".$esc['desc_endereco_complemento']."', 
	    				'".$esc['desc_endereco_bairro']."', '".$esc['co_municipio']."', 
	            		'".$esc['sg_uf']."', '".$esc['num_endereco']."', 'A');";
					
				$db->executar($sql);
				$inseriuend++;
				
			}
			
			$db->commit();
			
			
		}
	}


} catch (Exception $e) {
	
	echo "<pre>";
	print_r($e);
	echo "</pre>";
	
}

echo "<pre>";
echo "Numero entidades atualizadas: ".$atualizouent."<br>";
echo "Numero entidades inseridas: ".$inseriuent."<br>";
echo "Numero endereços atualizados: ".$atualizouend."<br>";
echo "Numero endereços inseridos: ".$inseriuend."<br>";
echo "</pre>";

echo "Fim:".date("d/mY h:i:s");

?>