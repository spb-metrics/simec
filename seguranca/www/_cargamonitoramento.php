<?php
function getmicrotime()
{list($usec, $sec) = explode(" ", microtime());
 return ((float)$usec + (float)$sec);} 


date_default_timezone_set ('America/Sao_Paulo');

$_REQUEST['baselogin'] = "simec_espelho_producao";
//$_REQUEST['baselogin'] = "simec_desenvolvimento";


// controle o cache do navegador
header( "Cache-Control: no-store, no-cache, must-revalidate" );
header( "Cache-Control: post-check=0, pre-check=0", false );
header( "Cache-control: private, no-cache" );   
header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
header( "Pragma: no-cache" );

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações */


// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once "./_funcoesmonitoramento.php";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

if($_REQUEST['ano'])$ano=$_REQUEST['ano'];
else $ano=date("Y");

if($_REQUEST['mes'])$mes=$_REQUEST['mes'];
else $mes=date("m");


$sql = "SELECT * FROM seguranca.sistema WHERE sisstatus='A'";
$sistemas = $db->carregar($sql);

if($sistemas[0]) {
	foreach($sistemas as $sis) {
		
		$Tinicio = getmicrotime();
		
		/*
		 * INICIO CARGA NO TIPO MONITORAMENTO NÚMERO DE USUÁRIOS DISTINTOS(NU)
		 */
		
		unset($dadosnu);
		
		$sql = "SELECT COUNT(DISTINCT usucpf) as num, to_char(estdata, 'DD') as dia FROM seguranca.estatistica WHERE sisid='".$sis['sisid']."' AND to_char(estdata, 'YYYY-MM')='".sprintf("%04d-%02d", $ano, $mes)."' GROUP BY to_char(estdata, 'DD') ORDER BY dia";
		$dados = $db->carregar($sql);
		if($dados[0]) {
			foreach($dados as $d) {
				$dadosnu[$d['dia']] = $d['num'];
			}
			$sql = "SELECT COUNT(DISTINCT usucpf) as num FROM seguranca.estatistica WHERE sisid='".$sis['sisid']."' AND to_char(estdata, 'YYYY-MM')='".sprintf("%04d-%02d", $ano, $mes)."'";
			$dadosnu['NULL'] = $db->pegaUm($sql);
		}
		
		$sql = "DELETE FROM seguranca.monitoramento WHERE monano='".$ano."' AND monmes='".$mes."' AND sisid='".$sis['sisid']."' AND tmoid='".NU."'";
		$db->executar($sql);
		if($dadosnu) {
			foreach($dadosnu as $dia => $valor) {
				$sql = "INSERT INTO seguranca.monitoramento(
		            	tmoid, sisid, monano, monmes, mondia, mondatacarga, monvalor)
		    			VALUES ('".NU."', '".$sis['sisid']."', '".$ano."', '".$mes."', ".$dia.", NOW(), '".$valor."');";
				$db->executar($sql);
			}
		}
		
		/*
		 * FIM CARGA NO TIPO MONITORAMENTO NÚMERO DE USUÁRIOS DISTINTOS(NU)
		 */
		
		/*
		 * INICIO CARGA NO TIPO MONITORAMENTO NÚMERO DE ERROS(NE)
		 */
		
		unset($dadosne);
		
		$sql = "SELECT COUNT(au.oid) as num, to_char(auddata, 'DD') as dia FROM seguranca.auditoria au 
			    INNER JOIN seguranca.menu me ON au.mnuid=me.mnuid 
				WHERE me.sisid='".$sis['sisid']."' AND au.audtipo='X' AND to_char(auddata, 'YYYY-MM')='".sprintf("%04d-%02d", $ano, $mes)."' 
				GROUP BY to_char(auddata, 'DD') ORDER BY dia";
		
		$dados = $db->carregar($sql);
		if($dados[0]) {
			foreach($dados as $d) {
				$dadosne[$d['dia']] = $d['num'];
			}
			$sql = "SELECT COUNT(au.oid) as num FROM seguranca.auditoria au 
			    INNER JOIN seguranca.menu me ON au.mnuid=me.mnuid 
				WHERE me.sisid='".$sis['sisid']."' AND au.audtipo='X' AND to_char(auddata, 'YYYY-MM')='".sprintf("%04d-%02d", $ano, $mes)."'";
			
			$dadosne['NULL'] = $db->pegaUm($sql);
		}
		
		$sql = "DELETE FROM seguranca.monitoramento WHERE monano='".$ano."' AND monmes='".$mes."' AND sisid='".$sis['sisid']."' AND tmoid='".NE."'";
		$db->executar($sql);
		if($dadosne) {
			foreach($dadosne as $dia => $valor) {
				$sql = "INSERT INTO seguranca.monitoramento(
		            	tmoid, sisid, monano, monmes, mondia, mondatacarga, monvalor)
		    			VALUES ('".NE."', '".$sis['sisid']."', '".$ano."', '".$mes."', ".$dia.", NOW(), '".$valor."');";
				$db->executar($sql);
			}
		}
		
		/*
		 * FIM CARGA NO TIPO MONITORAMENTO NÚMERO DE ERROS(NE)
		 */

		
		/*
		 * INICIO CARGA NO TIPO MONITORAMENTO TEMPO MÉDIO DE PROCESSAMENTO POR PÁGINA (TM)
		 */
		
		unset($dadostm);
		
		$sql = "SELECT ROUND(CAST(AVG(esttempoexec)as numeric),2) as num, to_char(estdata, 'DD') as dia FROM seguranca.estatistica WHERE sisid='".$sis['sisid']."' AND to_char(estdata, 'YYYY-MM')='".sprintf("%04d-%02d", $ano, $mes)."' GROUP BY to_char(estdata, 'DD') ORDER BY dia";		
		$dados = $db->carregar($sql);
		if($dados[0]) {
			foreach($dados as $d) {
				$dadostm[$d['dia']] = $d['num'];
			}
			$sql = "SELECT ROUND(CAST(AVG(esttempoexec)as numeric),2) as num FROM seguranca.estatistica WHERE sisid='".$sis['sisid']."' AND to_char(estdata, 'YYYY-MM')='".sprintf("%04d-%02d", $ano, $mes)."'";
			
			$dadostm['NULL'] = $db->pegaUm($sql);
		}
		
		$sql = "DELETE FROM seguranca.monitoramento WHERE monano='".$ano."' AND monmes='".$mes."' AND sisid='".$sis['sisid']."' AND tmoid='".TM."'";
		$db->executar($sql);
		if($dadostm) {
			foreach($dadostm as $dia => $valor) {
				$sql = "INSERT INTO seguranca.monitoramento(
		            	tmoid, sisid, monano, monmes, mondia, mondatacarga, monvalor)
		    			VALUES ('".TM."', '".$sis['sisid']."', '".$ano."', '".$mes."', ".$dia.", NOW(), '".$valor."');";
				$db->executar($sql);
			}
		}
		
		/*
		 * FIM CARGA NO TIPO MONITORAMENTO TEMPO MÉDIO DE PROCESSAMENTO POR PÁGINA (TM)
		 */
		

		/*
		 * INICIO CARGA NO TIPO MONITORAMENTO NÚMERO DE REQUISIÇÕES (NR)
		 */
		
		unset($dadosnr);
		
		$sql = "SELECT COUNT(oid) as num, to_char(estdata, 'DD') as dia FROM seguranca.estatistica WHERE sisid='".$sis['sisid']."' AND to_char(estdata, 'YYYY-MM')='".sprintf("%04d-%02d", $ano, $mes)."' GROUP BY to_char(estdata, 'DD') ORDER BY dia";		
		$dados = $db->carregar($sql);
		if($dados[0]) {
			foreach($dados as $d) {
				$dadosnr[$d['dia']] = $d['num'];
			}
			$sql = "SELECT COUNT(oid) as num FROM seguranca.estatistica WHERE sisid='".$sis['sisid']."' AND to_char(estdata, 'YYYY-MM')='".sprintf("%04d-%02d", $ano, $mes)."'";
			$dadosnr['NULL'] = $db->pegaUm($sql);
		}
		
		$sql = "DELETE FROM seguranca.monitoramento WHERE monano='".$ano."' AND monmes='".$mes."' AND sisid='".$sis['sisid']."' AND tmoid='".NR."'";
		$db->executar($sql);
		if($dadosnr) {
			foreach($dadosnr as $dia => $valor) {
				$sql = "INSERT INTO seguranca.monitoramento(
		            	tmoid, sisid, monano, monmes, mondia, mondatacarga, monvalor)
		    			VALUES ('".NR."', '".$sis['sisid']."', '".$ano."', '".$mes."', ".$dia.", NOW(), '".$valor."');";
				$db->executar($sql);
			}
		}
		
		/*
		 * FIM CARGA NO TIPO MONITORAMENTO NÚMERO DE REQUISIÇÕES (NR)
		 */
		
		
		/*
		 * INICIO CARGA NO TIPO MONITORAMENTO RELAÇÃO ERROS POR REQUISIÇÕES (PE)
		 */
		
		unset($dadospe);
		
		if($dadosne) {
			foreach($dadosne as $da => $d) {
				if($dadosnr[$da]) { 
					$dadospe[$da] = round(($d/$dadosnr[$da])*100,4);
				}
			}
		}
		
		$sql = "DELETE FROM seguranca.monitoramento WHERE monano='".$ano."' AND monmes='".$mes."' AND sisid='".$sis['sisid']."' AND tmoid='".PE."'";
		$db->executar($sql);
		if($dadospe) {
			foreach($dadospe as $dia => $valor) {
				$sql = "INSERT INTO seguranca.monitoramento(
		            	tmoid, sisid, monano, monmes, mondia, mondatacarga, monvalor)
		    			VALUES ('".PE."', '".$sis['sisid']."', '".$ano."', '".$mes."', ".$dia.", NOW(), '".$valor."');";
				$db->executar($sql);
			}
		}

		/*
		 * FIM CARGA NO TIPO MONITORAMENTO RELAÇÃO ERROS POR REQUISIÇÕES (PE)
		 */
		
		/*
		 * INICIO CARGA NO TIPO MONITORAMENTO MEMORIA UTILIZADA (MU)
		 */
		
		unset($dadosmu);
		
		$sql = "SELECT ROUND(CAST(AVG(estmemusa)as numeric),2) as num, to_char(estdata, 'DD') as dia FROM seguranca.estatistica WHERE sisid='".$sis['sisid']."' AND to_char(estdata, 'YYYY-MM')='".sprintf("%04d-%02d", $ano, $mes)."' GROUP BY to_char(estdata, 'DD') ORDER BY dia";		
		$dados = $db->carregar($sql);
		if($dados[0]) {
			foreach($dados as $d) {
				$dadosmu[$d['dia']] = $d['num'];
			}
			$sql = "SELECT ROUND(CAST(AVG(estmemusa)as numeric),2) as num FROM seguranca.estatistica WHERE sisid='".$sis['sisid']."' AND to_char(estdata, 'YYYY-MM')='".sprintf("%04d-%02d", $ano, $mes)."'";
			$dadosmu['NULL'] = $db->pegaUm($sql);
		}
		
		$sql = "DELETE FROM seguranca.monitoramento WHERE monano='".$ano."' AND monmes='".$mes."' AND sisid='".$sis['sisid']."' AND tmoid='".MU."'";
		$db->executar($sql);
		if($dadosmu) {
			foreach($dadosmu as $dia => $valor) {
				$sql = "INSERT INTO seguranca.monitoramento(
		            	tmoid, sisid, monano, monmes, mondia, mondatacarga, monvalor)
		    			VALUES ('".MU."', '".$sis['sisid']."', '".$ano."', '".$mes."', ".$dia.", NOW(), '".(($valor)?$valor:"0")."');";
				$db->executar($sql);
			}
		}
		
		/*
		 * FIM CARGA NO TIPO MONITORAMENTO MEMORIA UTILIZADA (MU)
		 */
		
		/*
		 * INICIO CARGA NO TIPO MONITORAMENTO USUARIOS ATIVOS (UA)
		 */
		
		unset($dadosua);
		
		$sql = "SELECT COUNT(DISTINCT usucpf) as num FROM seguranca.usuario_sistema WHERE sisid='".$sis['sisid']."' AND suscod='A'";		
		$dados = $db->carregar($sql);
		if($dados[0]) {
			foreach($dados as $d) {
				$dadosua[date("d")] = $d['num'];
			}
			$sql = "SELECT COUNT(DISTINCT usucpf) as num FROM seguranca.usuario_sistema WHERE sisid='".$sis['sisid']."' AND suscod='A'";
			$dadosua['NULL'] = $db->pegaUm($sql);
		}
		
		$sql = "DELETE FROM seguranca.monitoramento WHERE monano='".$ano."' AND monmes='".$mes."' AND sisid='".$sis['sisid']."' AND (mondia IS NULL OR mondia=".date("d").") AND tmoid='".UA."'";
		$db->executar($sql);
		if($dadosua) {
			foreach($dadosua as $dia => $valor) {
				$sql = "INSERT INTO seguranca.monitoramento(
		            	tmoid, sisid, monano, monmes, mondia, mondatacarga, monvalor)
		    			VALUES ('".UA."', '".$sis['sisid']."', '".$ano."', '".$mes."', ".$dia.", NOW(), '".(($valor)?$valor:"0")."');";
				$db->executar($sql);
			}
		}
		
		/*
		 * FIM CARGA NO TIPO MONITORAMENTO USUARIOS ATIVOS (UA)
		 */		
		
		/*
		 * INICIO CARGA NO TIPO MONITORAMENTO USUARIOS ATIVOS (UP)
		 */
		
		unset($dadosup);
		
		$sql = "SELECT COUNT(DISTINCT usucpf) as num FROM seguranca.usuario_sistema WHERE sisid='".$sis['sisid']."' AND suscod='P'";		
		$dados = $db->carregar($sql);
		if($dados[0]) {
			foreach($dados as $d) {
				$dadosup[date("d")] = $d['num'];
			}
			$sql = "SELECT COUNT(DISTINCT usucpf) as num FROM seguranca.usuario_sistema WHERE sisid='".$sis['sisid']."' AND suscod='P'";
			$dadosup['NULL'] = $db->pegaUm($sql);
		}
		
		$sql = "DELETE FROM seguranca.monitoramento WHERE monano='".$ano."' AND monmes='".$mes."' AND sisid='".$sis['sisid']."' AND (mondia IS NULL OR mondia=".date("d").") AND tmoid='".UP."'";
		$db->executar($sql);
		if($dadosup) {
			foreach($dadosup as $dia => $valor) {
				$sql = "INSERT INTO seguranca.monitoramento(
		            	tmoid, sisid, monano, monmes, mondia, mondatacarga, monvalor)
		    			VALUES ('".UP."', '".$sis['sisid']."', '".$ano."', '".$mes."', ".$dia.", NOW(), '".(($valor)?$valor:"0")."');";
				$db->executar($sql);
			}
		}
		
		/*
		 * FIM CARGA NO TIPO MONITORAMENTO USUARIOS ATIVOS (UP)
		 */		
		
		$db->commit();
		
		$HTML .= $sis['sisdsc']." foi carregado... ".number_format( ( getmicrotime() - $Tinicio ), 4, ',', '.' )."s<br>";
		$_TOTTEMPO[] = getmicrotime() - $Tinicio;
		
	}
	if($_TOTTEMPO) {
		foreach($_TOTTEMPO as $t) {
			$tf += $t; 
		}
		$HTML .= "Tempo total de carga... ".$tf."s<br>";
	}
}

require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= "SISTEMA DE CARGA NO MONITORAMENTO";
$mensagem->From 		= "simec@mec.gov.br";
$mensagem->AddAddress( "alexandre.dourado@mec.gov.br", "Alexandre Dourado" );
$mensagem->Subject = "Carga no monitoramento";
$mensagem->Body = $HTML;
$mensagem->IsHTML( true );
$mensagem->Send();

?>