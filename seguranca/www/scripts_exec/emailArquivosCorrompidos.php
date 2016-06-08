<?php
ini_set("memory_limit", "3000M");
set_time_limit(30000);

$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as fun��es gerais
include_once "/var/www/simec/global/config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . 'includes/classes/EmailAgendado.class.inc';

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conex�o com o servidor de banco de dados
$db = new cls_banco();

$e = new EmailAgendado();
$e->setTitle("Nota do Sistema - SIMEC");
$html = '<div style="font: 12pt Arial,verdana" ><center><b><span style="color:red" >NOTA do SISTEMA - SIMEC!</span></b><br /><br />
Alguns arquivos anexados por voc� no SIMEC foram corrompidos, para corrigir o problema voc� poder� envi�-los novamente seguindo os passos abaixo:<br /><br />
<b>1 � Entre no SIMEC com seu login e sua senha acessando: <a href="http://simec.mec.gov.br">http://simec.mec.gov.br</a>.<br /><br />
2 � Ao entrar no sistema, aparecer� uma mensagem que ir� direcion�-lo para a p�gina de Upload de Arquivos, outra op��o � acessar o Menu: Principal >> Upload de Arquivos.<br /><br />
3 � Uma lista de arquivos corrompidos ser� apresentada, voc� poder� selecionar os arquivos e clicar em SALVAR no final da p�gina para efetuar a corre��o.</b><br /><br />
</center>
Caso n�o apare�a a rela��o de arquivos ou a mensagem ao entrar no SIMEC, desconsidere esta mensagem.<br /><br />
Contamos com sua colabora��o.<br /><br />
Atenciosamente,<br /><br />
Equipe SIMEC<br /><br />
Obs.: Este � um email autom�tico enviado pelo sistema, favor n�o responder.</div>';
echo $html;
$e->setText($html);
$e->setName("SIMEC - Minist�rio da Educa��o");
$e->setEmailOrigem("no-reply@mec.gov.br");
//$e->setEmailsDestinoPorArquivo(APPRAIZ."www/painel/emailsDaniel.txt");

$sql = "select distinct 
			u.usuemail
		from
			public.arquivo a
		inner join 
			seguranca.usuario u ON u.usucpf = a.usucpf
		inner join
			seguranca.perfilusuario pu ON pu.usucpf = a.usucpf
		inner join
			seguranca.perfil pe ON pe.pflcod = pu.pflcod
		where 
			pe.sisid = 15
		and 
			(a.arqid / 1000) between 647 and 725
		and 
			a.arqid not in(select arqid from public.arquivo_recuperado)
		and
			(a.arqstatus is null or a.arqstatus != 'I'::bpchar or a.arqstatus != '0'::bpchar)";
$arrEmails = $db->carregar($sql);
foreach($arrEmails as $email){
	$arrEmail[] = $email['usuemail'];
}

$arrEmail[] = "julianosouza@mec.gov.br";
$arrEmail[] = "cristianocabral@mec.gov.br";
$e->setEmailsDestino($arrEmail);
$e->enviarEmails();