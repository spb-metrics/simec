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

$sql = "SELECT count(obrid) as num, 
			   ob.dirid,
			   ee.entid,
			   ee.entsig, 
			   UPPER(ee.entnome) as entnome,
			   ob.dirnome,
			   ob.dddcel||ob.telcel1 as telefone1,
			   ob.diremail1 
		FROM obras.obrainfraestrutura oi 
		INNER JOIN entidade.entidade ee ON ee.entid = oi.entidunidade 
		INNER JOIN obras.dirigentesunidades ob ON ob.unicod = ee.entunicod 
		WHERE obrdtvistoria < (CURRENT_DATE - integer '45') AND 
			  oi.stoid != '3' AND 
			  oi.stoid != '6' AND 
			  oi.stoid != '99'
		GROUP BY ee.entid, 
				 ee.entnome, 
				 ee.entsig, 
				 ob.dirid, 
				 ob.dirnome, 
				 ob.dddcel, 
				 ob.telcel1, 
				 ob.diremail1";

$dados = $db->carregar($sql);

if($dados[0]) {

	require_once('../webservice/painel/nusoap.php');
	
	$client = new soapcliente('https://webservice.cgi2sms.com.br/axis/services/VolaSDKSecure?wsdl', true);
	$err = $client->getError();
	if ($err) die('<h2>Constructor error</h2><pre>' . $err . '</pre>');

	require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
	require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
	
	$mensagem = new PHPMailer();
	$mensagem->persistencia = $db;
	$mensagem->Host         = "localhost";
	$mensagem->Mailer       = "smtp";
	$mensagem->FromName		= $GLOBALS['parametros_sistema_tela']['sigla']." - Lembrete automático";
	$mensagem->From 		= $GLOBALS['parametros_sistema_tela']['email'];
	$mensagem->Subject      = "Obras desatualizadas no ".$GLOBALS['parametros_sistema_tela']['sigla']." - Módulos de obras";
	$mensagem->IsHTML( true );
	
	$_LOG .= "Foram encontrados ".count($dados)." regitros para serem encaminhados <br /><br />";
	
	foreach($dados as $key => $d) {
		
		if(strlen($d['telefone1']) == 10) {
			$envio = $client->call('sendMessage', array('user' => 'inep', 'password' => 'tmmjee', 'testMode' => true, 'sender' => $GLOBALS['parametros_sistema_tela']['sigla'], 'target' => '55'.$d['telefone1'], 'body' => 'Existem '.$d['num'].' obras da '.$d['entsig'].' sob sua responsabilidade desatualizadas, acesse '.$GLOBALS['sgi_url_sistema']['home'].' e proceda a atualização.', 'ID' => substr($d['dirid'],0,6).date("Ymdhis")));
		}
		
		$_LOG .= $d['dirnome']." >> ";
		
		if(!$envio) {
			$_LOG .= "SMS Enviado para telefone ".$d['telefone1']." >> ";
		} else {
			$_LOG .= "Problemas para enviar SMS para telefone ".$d['telefone1']." >> ";
		}
		
		$sql = "SELECT DISTINCT
			    nome_obra,
			    descricao,
			    municipio,
			    inicio,
			    final,
			    situacao,
			    CASE WHEN codigo_situacao NOT IN (2,3,4,5) THEN
				(CASE WHEN ultimadata > (CURRENT_DATE - integer '30') THEN '<label style=\"color:#00AA00;\">' ||  to_char(ultimadata, 'DD/MM/YYYY') || '</label>'
				      WHEN (ultimadata <= (CURRENT_DATE - integer '30') AND ultimadata >= (CURRENT_DATE - integer '45')) THEN '<label style=\"color:#BB9900;\">' || to_char(ultimadata, 'DD/MM/YYYY') || '</label>'
				 ELSE '<label style=\"color:#DD0000;\">' || to_char(ultimadata, 'DD/MM/YYYY') || '</label>' END)
				 WHEN codigo_situacao = 2 THEN '<label style=\"color:#DD0000;\">' || to_char(ultimadata, 'DD/MM/YYYY') || '</label>'
				 WHEN codigo_situacao = 3 THEN '<label style=\"color:blue;\">' || to_char(ultimadata, 'DD/MM/YYYY') || '</label>'
			    ELSE to_char(ultimadata, 'DD/MM/YYYY') END as ultimadata,
			    percentual
			FROM (
			    SELECT DISTINCT
			        CASE WHEN fr.covnumero is not null THEN '' || fr.covnumero || ' - ' || UPPER( oi.obrdesc ) || '' ELSE '' || UPPER( oi.obrdesc ) || '' END as nome_obra,
			        upper(COALESCE(et.entnome, '')) as descricao,
			        mun.mundescricao || '/' || ed.estuf as municipio,
			        to_char(oi.obrdtinicio,'DD/MM/YYYY') as inicio,
			        to_char(oi.obrdttermino,'DD/MM/YYYY') as final,
			        sto.stodesc as situacao,
			        oi.stoid as codigo_situacao,
			        CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END as ultimadata,
			        (SELECT replace(coalesce(round(SUM(icopercexecutado), 2), '0') || ' %', '.', ',') as total FROM obras.itenscomposicaoobra WHERE obrid = oi.obrid) as percentual,
			        oi.obrid as id
			    FROM
			        obras.obrainfraestrutura oi
			    LEFT JOIN
			        entidade.entidade et ON oi.entidcampus = et.entid
			    LEFT JOIN
			        obras.situacaoobra sto ON oi.stoid = sto.stoid
			    INNER JOIN
			        entidade.endereco ed ON ed.endid = oi.endid
			    LEFT JOIN
			        territorios.municipio mun ON mun.muncod = ed.muncod
			    INNER JOIN
			        obras.orgao org ON oi.orgid = org.orgid
			    LEFT JOIN (
			        SELECT fri.obrid, covi.covid, covi.covnumero
			        FROM obras.formarepasserecursos fri
			        INNER JOIN obras.conveniosobra covi ON covi.covid = fri.covid  )fr ON fr.obrid = oi.obrid
			    LEFT JOIN
			        obras.arquivosobra aa ON aa.obrid = oi.obrid and aa.tpaid = 21
			    LEFT JOIN
			        obras.arquivosobra aa2 ON aa2.obrid = oi.obrid and aa2.tpaid <> 21
			    LEFT JOIN
					monitora.pi_obra o ON o.obrid = oi.obrid 
				LEFT JOIN
					monitora.pi_planointerno mpi ON mpi.pliid = o.pliid AND mpi.plistatus = 'A'
			    LEFT JOIN
			        public.arquivo pa ON pa.arqid = aa.arqid and aa.aqostatus = 'A'
			    LEFT JOIN
			        (SELECT distinct obrid, rststatus
			         FROM obras.restricaoobra
			         WHERE rststatus = 'A') r ON r.obrid = oi.obrid
				 WHERE  oi.obsstatus = 'A' AND 
				 		oi.entidunidade='".$d['entid']."' AND 
			  			oi.stoid != '3' AND 
			  			oi.stoid != '6' AND
			  			oi.stoid != '99'
			    GROUP BY
			        org.orgdesc, oi.obrid, oi.obrdesc, oi.obrdtinicio, oi.obrdttermino,
			        sto.stodesc, r.obrid, fr.covnumero, et.entnome,
			        mun.mundescricao, ed.estuf, aa.obrid, oi.obrpercexec,
			        oi.obsdtinclusao, oi.stoid, oi.obrdtvistoria, aa2.obrid, mpi.plistatus, o.obrid, mpi.plicod
			    ORDER BY
			        municipio) as foo 
			    WHERE ultimadata < (CURRENT_DATE - integer '45')";

		
		ob_start();
		$cabecalho = array("Nome da Obra", "Campus", "Município/UF", "Data de Início", "Data de Término", "Situação da Obra", "Ultima Atualização", "(%) Executado" );
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);
		$dadosserv = ob_get_contents();
		ob_end_clean();
		
		unset($mensagem->to);
		$mensagem->AddAddress($d['diremail1'], $d['dirnome']);
		$mensagem->IsHTML(true);
		$mensagem->Body = "<style>table.listagem  {border-bottom:3px solid #DFDFDF;border-collapse:collapse;border-top:2px solid #404040;font-size:11px;padding:3px;font:8pt Arial,verdana;}body {font:12px Arial,verdana;}</style>";
		$mensagem->Body .= "<p>Prezado <b>".$d['dirnome']."</b>,</p>";
		$mensagem->Body .= '<p>Existem <b>'.$d['num'].'</b> obras desatualizadas referentes a sua unidade <b>'.$d['entnome'].'</b>. Acesse o <a href="'.$GLOBALS['sgi_url_sistema']['home'].'" target=_blank>'.$GLOBALS['parametros_sistema_tela']['sigla'].' - Monitoramento de obras</a> ('.$GLOBALS['sgi_url_sistema']['home'].') e realize as atualizações.</p>';
		$mensagem->Body .= "<p>Segue abaixo a lista de obras desatualizadas:</p>";
		$mensagem->Body .= $dadosserv;
		$mensagem->Body .= "<p>Agradecemos a colaboração,<br/>".$GLOBALS['parametros_sistema_tela']['orgao']."</p>";
		
		$enviosmtp = $mensagem->Send();
		
		if($enviosmtp) {
			$_LOG .= "Email enviado para ".$d['diremail1']." <br /> ";
		} else {
			$_LOG .= "Problemas para enviar email ".$d['diremail1']." <br /> ";
		}

	}
	
}

$sql = "INSERT INTO obras.dirigentesunidadeslogs(
            dildata, dillog)
    	VALUES (NOW(), '".$_LOG."');";

$db->executar($sql);
$db->commit();

echo $_LOG;

?>
