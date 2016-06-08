<?php
ini_set("memory_limit", "3000M");
set_time_limit(30000);

include_once "config.inc";
include_once "_constantes.php";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

include APPRAIZ . 'includes/classes/EmailAgendado.class.inc';

$e = new EmailAgendado();
$e->setTitle("Nota do Sistema - ".$GLOBALS['parametros_sistema_tela']['sigla']);
$html = '<div style="font: 12pt Arial,verdana" ><center><b><span style="color:red" >NOTA do SISTEMA - '.$GLOBALS['parametros_sistema_tela']['sigla'].'!</span></b><br /><br />
Alguns arquivos que você anexou foram corrompidos, para corrigir o problema você poderá enviá-los novamente seguindo os passos abaixo:<br /><br />
<b>1 - Entre no sistema com seu login e sua senha acessando: <a href="'.$GLOBALS['sgi_url_sistema']['home'].'">'.$GLOBALS['sgi_url_sistema']['home'].'</a>.<br /><br />
2 - Ao entrar no sistema, aparecerá uma mensagem que irá direcioná-lo para a página de Upload de Arquivos, outra opção é acessar o Menu: Principal >> Upload de Arquivos.<br /><br />
3 - Uma lista de arquivos corrompidos será apresentada, você poderá selecionar os arquivos e clicar em SALVAR no final da página para efetuar a correção.</b><br /><br />
</center>
Caso não apareça a relação de arquivos ou a mensagem ao entrar no '.$GLOBALS['parametros_sistema_tela']['sigla'].', desconsidere esta mensagem.<br /><br />
Contamos com sua colaboração.<br /><br />
Atenciosamente,<br /><br />
Equipe '.$GLOBALS['parametros_sistema_tela']['sigla'].'<br /><br />
Obs.: Este é um email automático enviado pelo sistema, favor não responder.</div>';
echo $html;
$e->setText($html);
$e->setName($GLOBALS['parametros_sistema_tela']['sigla']." - ".$GLOBALS['parametros_sistema_tela']['orgao']);
$e->setEmailOrigem("no-reply@presidencia.gov.br");
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
			(a.arqstatus != 'I'::bpchar or a.arqstatus != '0'::bpchar)";
$arrEmails = $db->carregar($sql);
//foreach($arrEmails as $email){
//	$arrEmail[] = $email['usuemail'];
//}

$arrEmail[] = "julianosouza@mec.gov.br";
//$arrEmail[] = "cristianocabral@mec.gov.br";
$e->setEmailsDestino($arrEmail);
$e->enviarEmails();
