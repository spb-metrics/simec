<?php

/**
 * Componente para um popup com uma lista ordenada.
 * @param $params array
 * 
 * O par�metros do componentes s�o passados por um array, sendo
 * que cada chave do array representa uma funcionalidade espec�fica.
 * Se o array for vazio n�o imprime nenhum conte�do.
 * 
 * Abaixo a lista com as chaves do array que podem ser utilizadas:
 * 		-> 'nome': 
 * 		-> 'valueButton': o texto(value) do bot�o que abre a popup. Se n�o for informado o padr�o do texto � "Abrir".
 * 		-> 'sql': a query para os dados da lista. Deve ser informado os alias "codigo" e "descricao", para defini��o do 
 * 				  valor(value) do checkbox e do texto que se segue, respectivamente.
 * 		-> 'titulo': t�tulo usado na popup para descrever os itens da lista. Se n�o for informado o padr�o � "Selecione o(s) item(ns) desejado(s):".
 */
function popLista( $params = array() )
{
	/*** Se algum par�metro foi criado... ***/
	if( !empty($params) )
	{
		/*** ***/
		if( $params['nome'] )
		{
			/*** Se o SQL foi definido... ***/
			if( $params['sql'] )
			{
				/*** Joga o SQL na sess�o para uso no arquivo popLista.php ***/
				$_SESSION['sql_pop_lista'][$params['nome']] = $params['sql'];
			}
			
			/*** Cria a vari�vel com o value do bot�o. Se n�o houver sido informada, usa-se o padr�o ***/
			$valueButton = ( $params['valueButton'] ) ? $params['valueButton'] : 'Abrir';
			/*** Cria vari�vel com o t�tulo da popup. Se n�o houver sido informada, usa-se o padr�o ***/
			$titulo = ( $params['titulo'] ) ? $params['titulo'] : 'Selecione o(s) item(ns) desejado(s):';
			/*** Imprime o bot�o que abrir a pop-up. (Arquivo: www/geral/popLista.php) ***/
			echo '<input type="button" value="'.$valueButton.'" onclick="abrePopLista(\''.urlencode($params['nome']).'\', \''.urlencode($titulo).'\');" />';
		}
	}
}

/*
 Sistema Sistema Simec
 Setor respons�vel: SPO/MEC
 Desenvolvedor: Desenvolvedores Simec
 Analista: Gilberto Arruda Cerqueira Xavier
 Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
 M�dulo: funcoes.inc
 Finalidade: reuni�o de v�rias funcoes que n�o dependem de banco
 Data de cria��o: 24/06/2005
 */

function emailPara($cpf){
	$html = "<img src='../imagens/email.gif' title='Enviar e-mail' border='0' onclick='emailPara(\"{$cpf}\");' style='cursor:pointer;'>&nbsp;";
	
	return $html;
}

/**
 * Converte Inteiros em Algarismos Romanos
 * por: Werter Dias Almeida em 11/03/2009 
 * @param integer valor
 * @return string
 */
function decimal2romano($decimal)
{ 
	$romano = "";
	while ($decimal/1000 >= 1) {$romano .= "M"; $decimal = decimal-1000;}
		if ($decimal/900 >= 1) {$romano .= "CM"; $decimal=$decimal-900;}
		if ($decimal/500 >= 1) {$romano .= "D"; $decimal=$decimal-500;}
		if ($decimal/400 >= 1) {$romano .= "CD"; $decimal=$decimal-400;}
	while ($decimal/100 >= 1) {$romano .= "C"; $decimal = $decimal-100;}
		if ($decimal/90 >= 1) {$romano .= "XC"; $decimal=$decimal-90;}
		if ($decimal/50 >= 1) {$romano .= "L"; $decimal=$decimal-50;}
		if ($decimal/40 >= 1) {$romano .= "XL"; $decimal=$decimal-40;}
	while ($decimal/10 >= 1) {$romano .= "X"; $decimal = $decimal-10;}
		if ($decimal/9 >= 1) {$romano .= "IX"; $decimal=$decimal-9;}
		if ($decimal/5 >= 1) {$romano .= "V"; $decimal=$decimal-5;}
		if ($decimal/4 >= 1) {$romano .= "IV"; $decimal=$decimal-4;}
	while ($decimal >= 1) {$romano .= "I"; $decimal = $decimal-1;}
	
	return $romano;
} 

/*
 * fun��o que faz a verifica��o do CPF
 * 
 * @param (string) $cpf 
 * @return (true) caso o CPF seja v�lido
 * @return (false) caso inv�lido.
 * @author: FelipeChiavicatti;
 */
function validaCPF($cpf) {
	/*
	*/
	$nulos = array("12345678909","11111111111","22222222222","33333333333",
	               "44444444444","55555555555","66666666666","77777777777",
	               "88888888888","99999999999","00000000000");
	
	/* Retira todos os caracteres que nao sejam 0-9 */
	$cpf = ereg_replace("[^0-9]", "", $cpf);
	
	/*Retorna falso se houver letras no cpf */
	if (!(ereg("[0-9]",$cpf)))
	    return 0;
	
	/* Retorna falso se o cpf for nulo */
	if( in_array($cpf, $nulos) )
	    return 0;
	
	if (strlen($cpf) > 11)
		return 0;	    
	    
	/*Calcula o pen�ltimo d�gito verificador*/
	$acum=0;
	for($i=0; $i<9; $i++) {
	  $acum+= $cpf[$i]*(10-$i);
	}
	
	$x=$acum % 11;
	$acum = ($x>1) ? (11 - $x) : 0;
	
	/* Retorna falso se o digito calculado eh diferente do passado na string */
	if ($acum != $cpf[9]){
	  return 0;
	}
	
	/*Calcula o �ltimo d�gito verificador*/
	$acum=0;
	for ($i=0; $i<10; $i++){
	  $acum+= $cpf[$i]*(11-$i);
	}  
	
	$x=$acum % 11;
	$acum = ($x > 1) ? (11-$x) : 0;
	
	/* Retorna falso se o digito calculado eh diferente do passado na string */
	if ( $acum != $cpf[10]){
	  return 0;
	}  
	
	/* Retorna verdadeiro se o cpf eh valido */
	return 1;
}


function get_rnd_iv($iv_len)
{
	// gera um caracter aleat�rio em fun��o de um n�mero aleat�rio gerado pela fun��o mt_rand
	$iv = '';
	while ($iv_len-- > 0) {
		$iv .= chr(mt_rand() & 0xff);
	}
	return $iv;
}

//Fun��o que retorna true caso contenha a valor procurado em um array ou false caso n�o encontre
function in($valor, $vetor)
{
	$key = array_search($valor, $vetor);
	if ($key==null and $vetor[0]<>$valor) return false; else return true;
}

function formata_numero($nm) {
	for ($done=strlen($nm); $done > 3;$done -= 3) {
		$returnNum = ".".substr($nm,$done-3,3).$returnNum;
	}
	return substr($nm,0,$done).$returnNum;
}

function aleatorio() {
	$aleatorio = '';
	while (strlen($aleatorio) < 15)
	$aleatorio = $aleatorio.mt_rand(2000,40000);
	Return $aleatorio;
}
function verifica_datas($dtmaior,$dtmenor,$dif) {
	if (
	mktime (0,0,0,substr($dtmaior,3,2),substr($dtmaior,0,2),substr($dtmaior,6,4)) -
	mktime (0,0,0,substr($dtmenor,3,2), substr($dtmenor,0,2), substr($dtmenor,6,4))
	> $dif
	)
	{
		return false;
	}
	else {
		return true;
	}
}

function ajusta_data($data)
{
	// retorna a data no formato yyyy-mm-dd
	return substr($data,6,4).'-'.substr($data,3,2).'-'.substr($data,0,2);
}

function updateArquivo($nome,$unicod,$codResposta){

	global  $db;
	$sql = "update reuni.resposta set rspdsc = $nome where unicod = '$unicod' and rspcod = $codResposta ";
	$db->executar($sql);
	$db->commit();
	return;
}

function formata_data($data)
{
	// retorna a data no formato yyyy-mm-dd
	//if ($data) return substr($data,0,2).'/'.substr($data,3,2).'/'.substr($data,6,4);

	if ($data) return strftime("%d/%m/%Y",strtotime($data));

	else return '';
}

function formata_data_sql($data)
{
	if ($data) return substr($data,6,4).'-'.substr($data,3,2).'-'.substr($data,0,2);
	else return '';
}


function controla_navegacao($nl)
{
	if ($_POST['navega'] == 0) $_SESSION['registro'] = 0;
	else if ($_POST['navega'] == -2) $_SESSION['registro'] = $nl;
	else
	{
		$_SESSION['registro'] = $_SESSION['registro'] + $_POST['navega'];
		if ($_SESSION['registro'] < 0) $_SESSION['registro'] = 0;
		if ($_SESSION['registro'] > $nl) $_SESSION['registro'] = $nl;
	}
	if (! $_SESSION['registro'] ) $_SESSION['registro'] = 0;
}
function md5_encrypt_senha($plain_text, $password, $iv_len = 16)
{
	$plain_text .= "\x13";
	$n = strlen($plain_text);
	if ($n % 16) $plain_text .= str_repeat("\0", 16 - ($n % 16));
	$i = 0;
	$enc_text = get_rnd_iv($iv_len);
	$iv = substr($password ^ $enc_text, 0, 512);
	while ($i < $n) {
		$block = substr($plain_text, $i, 16) ^ pack('H*', md5($iv));
		$enc_text .= $block;
		$iv = substr($block . $iv, 0, 512) ^ $password;
		$i += 16;
	}
	return base64_encode($enc_text);
	// return base64_encode(trim($plain_text));
}
function md5_encrypt($plain_text, $password='', $iv_len = 16)
{
	//$plain_text .= "\x13";
	$n = strlen($plain_text);
	if ($n % 16) $plain_text .= str_repeat("\0", 16 - ($n % 16));
	$i = 0;
	$enc_text = get_rnd_iv($iv_len);
	$iv = substr($password ^ $enc_text, 0, 512);
	while ($i < $n) {
		$block = substr($plain_text, $i, 16) ^ pack('H*', md5($iv));
		$enc_text .= $block;
		$iv = substr($block . $iv, 0, 512) ^ $password;
		$i += 16;
	}
	//return base64_encode($enc_text);
	return base64_encode(trim($plain_text));
}
Function obrigatorio()
{
	$obrigatorio = " <img border='0' src='../imagens/obrig.gif' title='Indica campo obrigat�rio.' />";
	return $obrigatorio;
}
function md5_decrypt($enc_text, $password='', $iv_len = 16)
{
	$enc_text = base64_decode($enc_text);
	$n = strlen($enc_text);
	$i = $iv_len;
	$plain_text = '';
	$iv = substr($password ^ substr($enc_text, 0, $iv_len), 0, 512);
	while ($i < $n) {
		$block = substr($enc_text, $i, 16);
		$plain_text .= $block ^ pack('H*', md5($iv));
		$iv = substr($block . $iv, 0, 512) ^ $password;
		$i += 16;
	}
	return $enc_text;
	//return preg_replace('/\\x13\\x00*$/', '', $plain_text);
}

function md5_decrypt_senha($enc_text, $password, $iv_len = 16)
{
	$enc_text = base64_decode($enc_text);
	$n = strlen($enc_text);
	$i = $iv_len;
	$plain_text = '';
	$iv = substr($password ^ substr($enc_text, 0, $iv_len), 0, 512);
	while ($i < $n) {
		$block = substr($enc_text, $i, 16);
		$plain_text .= $block ^ pack('H*', md5($iv));
		$iv = substr($block . $iv, 0, 512) ^ $password;
		$i += 16;
	}
	return preg_replace('/\\x13\\x00*$/', '', $plain_text);

}

function monta_titulo($linha1,$linha2)
{

	//print '<center><div id="cabecalho" align="center" style="background-color:#DCDCDC;padding-top:3px;padding-bottom:3px;width:95%;" class="TituloTela">'.$linha1.'</div></center>';
	print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">';
	print '<tr><td width="100%" align="center"><label class="TituloTela" style="color:#000000;">'.$linha1.'</label></td></tr><tr>';
	print '<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >'.$linha2.'</td></tr></table>';
}

function sql_vincula_arquivo ($campo,$item,$chave=0)
{
	//esta funcao tem por finalidade montar a sql que permitir� listar os arquivos que
	// poder�o ser listados para download
	$usucpf=$_SESSION['usucpf'];
	$sql = "select d.usucpf as donoarquivo, d.tpdcod, d.docid,d.docdsc as descricao, d.docnomefisico, case when d.docsigilo='O' then 'Ostensivo' when d.docsigilo='R' then 'Reservado' else 'Confidencial' end as sigilo from public.documento d inner join public.documento_processo dp on d.docid=dp.docid and dp.tabela_processo='$campo' inner join $campo cc on cc.$chave=$item and cc.$chave=dp.proid order by d.docsigilo,d.docdsc asc";
	return $sql;


}


function formatar_cpf($cpf) {
	if ( empty($cpf) ) return ;
	else return substr($cpf,0,3).".".substr($cpf,3,3).".".substr($cpf,6,3)."-".substr($cpf,9,2);
}
function formatar_cnpj($cnpj) {
	if ( empty($cnpj) ) return ;
	return substr($cnpj,0,2).".".substr($cnpj,2,3).".".substr($cnpj,5,3)."/".substr($cnpj,8,4)."-".substr($cnpj,12,2);
}
function pega_numero($cnpjcpf) {
	if ( empty($cnpjcpf) ) return ;
	return preg_replace( "/[^\d]/", "", $cnpjcpf );
}

function formatar_cpf_cnpj($cpfcnpj) {
	if ( strlen($cpfcnpj) == 11 )
	return substr($cpfcnpj,0,3).".".substr($cpfcnpj,3,3).".".substr($cpfcnpj,6,3)."-".substr($cpfcnpj,9,2);
	else if ( strlen($cpfcnpj) == 14 )
	return substr($cpfcnpj,0,2).".".substr($cpfcnpj,2,3).".".substr($cpfcnpj,5,3)."/".substr($cpfcnpj,8,4)."-".substr($cpfcnpj,12,2);
	else return;
}
function make_seed() {
	list($usec, $sec) = explode(' ', microtime());
	return (float) $sec + ((float) $usec * 100000);
}
function senha() {
	mt_srand(make_seed());
	for($i=0;$i<8;$i++) {
		$n = (integer)mt_rand(0,35);
		$c = ($n<10) ? ($n+48) : ($n+55);
		$senha.= pack ("c",$c);
	}
	return $senha;
}
function email($paraquem, $paraonde, $assunto, $message, $cc='',$cco='')
{
	global $db;
	require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
	require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
	
	//trata mensagem
	if($message) $message = str_replace(chr(13), '<br>', $message); 
	
	$mensagem = new PHPMailer();
	$mensagem->persistencia = $db;
	$mensagem->Host         = "localhost";
	$mensagem->Mailer       = "smtp";
	$mensagem->FromName		= $GLOBALS['parametros_sistema_tela']['sigla']." - ".strtoupper($_SESSION['sisdiretorio'])." - " . $_SESSION['usunome'] . " - " . $_SESSION['usuorgao'];
	
	// Altera��o feita por Felipe Carvalho - 19/02/2010
	//$mensagem->AddReplyTo("cristiano.cabral@mec.gov.br");
	$mensagem->AddReplyTo($_SESSION["usuemail"]);
	//$mensagem->From = "simec@mec.gov.br";
	$mensagem->From = $_SESSION["usuemail"];
	
	$mensagem->AddAddress( $paraonde, $paraquem );
	$mensagem->Subject = $assunto;
	$mensagem->Body = $message;
	$mensagem->IsHTML( true );
	foreach ( explode( ";", $cc ) as $end )
	{
		$end = trim( $end );
		if( $end )
		{
			$mensagem->AddCC( $end );
		}
	}
	foreach ( explode( ";", $cco ) as $end )
	{
		$end = trim( $end );
		if( $end )
		{
			$mensagem->AddBCC( $end );
		}
	}

	return $mensagem->Send();


	/*

	global $db;
	// esta fun��o est� sendo alterada para incluir sempre a origem e para enviar um email para a mesma origem
	$sql = "select u.usunome, u.usuemail, un.unidsc,o.orgdsc from seguranca.usuario u left join unidade un on un.unicod=u.unicod left join orgao o on o.orgcod=u.orgcod where u.usucpf='".$_SESSION['usucpf']."' limit 1";
	$res=$db->pegalinha($sql);
	if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	//$message=strip_tags($message);
	$dequem = "SIMEC - $usunome - $unidsc - $orgdsc"; //trim($_SESSION['sigla']);
	$deonde = "$usuemail;simec@mec.gov.br";//$_SESSION['ittemail'];
	$headers = "Return-Path: simec@mec.gov.br\n";
	$headers .= "Reply-To: simec@mec.gov.br\n";
	$headers .= "X-Sender: simec@mec.gov.br\n";
	$headers .= "Content-type: text/html;\n";
	$headers .= "From: \"".$dequem."\" <".$deonde.">\n";
	$headers .= "X-Mailer: PHP 5.1\n";
	//$headers .= "Reply-To:gilbertoxavier@gmail.com\n";
	if ($cc) $headers .= "Cc:$cc\n";
	if ($cco) $headers .= "Bcc:$cco\n";
	$headers .= "MIME-Version: 1.0\n";
	//dbg($headers,1);
	return mail($paraonde, $assunto, $message, $headers);
	*/
}

function email_pessoal($dequem,$deonde, $paraonde, $assunto, $message, $cc='',$cco='')
{

	global $db;
	require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
	require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
	$mensagem = new PHPMailer();
	$mensagem->persistencia = $db;
	$mensagem->Host         = "localhost";
	$mensagem->Mailer       = "smtp";
	$mensagem->FromName		= $GLOBALS['parametros_sistema_tela']['sigla']." - " . $dequem;
	$mensagem->AddReplyTo("noreply@presidencia.gov.br");
	$mensagem->From = $deonde;
	$mensagem->AddAddress( $paraonde );
	$mensagem->Subject = $assunto;
	$mensagem->Body = $message;
	$mensagem->IsHTML( true );
	foreach ( explode( ";", $cc ) as $end )
	{
		$end = trim( $end );
		if( $end )
		{
			$mensagem->AddCC( $end );
		}
	}
	foreach ( explode( ";", $cco ) as $end )
	{
		$end = trim( $end );
		if( $end )
		{
			$mensagem->AddBCC( $end );
		}
	}
	return $mensagem->Send();
	/*
	 // $dequem = 'SIMEC'; //trim($_SESSION['sigla']);
	 // $deonde = 'simec@mec.gov.br';//$_SESSION['ittemail'];
	 $headers = "Return-Path: simec@mec.gov.br\n";
	 $headers .= "X-Sender: simec@mec.gov.br\n";
	 $headers .= "Reply-To: simec@mec.gov.br\n";
	 $headers .= "Content-type: text/html;\n";
	 $headers .= "From: \"".'SIMEC - '.$dequem."\" <".$deonde.">\n";
	 $headers .= "X-Mailer: PHP 5.1\n";
	 if ($cc) $headers .= "Cc:$cc\n";
	 if ($cco) $headers .= "Bcc:$cco\n";
	 $headers .= "MIME-Version: 1.0\n";
	 return mail($paraonde, $assunto, $message, $headers);
	 */
}

function enviar_email( $remetente, $destinatario, $assunto, $conteudo, $cc='',$cco='', $arquivos = array()){
	require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
	require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
	$mensagem = new PHPMailer();
	global $db;
	$mensagem->persistencia = $db;
	$mensagem->Host         = "localhost";
	$mensagem->Mailer       = "smtp";
	
	/* Regras definidas pelo Cristiano (11/11/2008)*/
	//$mensagem->From = $remetente['email'];
	
	/* Regras redefinidas pelo Cristiano (08/10/2009)*/
	$mensagem->From = "sgi@presidencia.gov.br";
	
	if(count($remetente)==2) {
		$mensagem->AddReplyTo($remetente['email'],$remetente['nome']);
		$mensagem->FromName = $remetente['nome'];
	} elseif($remetente != '') {
		$mensagem->FromName = $remetente['nome'];
	} else {
		$mensagem->FromName = "Administrador do Sistema";
	}
	/* FIM regras*/
	if( is_array( $destinatario ) ){
		foreach( $destinatario as $email ){
			$mensagem->AddAddress( $email );
		}
	}
	else{
		$mensagem->AddAddress( $destinatario );
	}	
	/*
	 * Atualizado pelo Alexandre Dourado
	 * recebe um array de email
	 */
	if(count($cc) > 1) {
		foreach($cc as $email) {
			$mensagem->AddCC( $email );
		}
	} else {
		$mensagem->AddCC( $cc );
	}
	/*
	 * FIM Atualizado pelo Alexandre Dourado
	 */
	
	# Atualizado pelo Wesley Romualdo
	//anexa os arquivos
	foreach ( $arquivos as $arquivo ){
		if ( !file_exists( $arquivo['arquivo'] ) ) {
			continue;
		}
		$mensagem->AddAttachment( $arquivo['arquivo'], basename( $arquivo['nome'] ) );
	}
	# Fim Atualizado pelo Wesley Romualdo 
	
	$mensagem->Subject = $assunto;
	$mensagem->Body = $conteudo;
	$mensagem->IsHTML( true );
	return $mensagem->Send();
}

function enviar_email_usuario( $remetente, $destinatario, $assunto, $conteudo, $cc='',$cco='' ){	
	require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
	require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
	$mensagem = new PHPMailer();
	global $db;
	$mensagem->persistencia = $db;	
	$mensagem->Host         = "localhost";
	$mensagem->Mailer       = "smtp";		
	$mensagem->From 		= $remetente;
	$mensagem->FromName 	= $GLOBALS['parametros_sistema_tela']['sigla']." - ".strtoupper($_SESSION['sisdiretorio'])." - " . $_SESSION['usunome'];
	$mensagem->AddAddress( $destinatario );
	$mensagem->Subject 		= $assunto;
	$mensagem->Body 		= $conteudo;
	$mensagem->IsHTML( true );	
		
	//ver($mensagem);
	foreach ( explode( ";", $cc ) as $end )
	{
		$end = trim( $end );
		if( $end )
		{
			$mensagem->AddCC( $end );
		}
	}
	foreach ( explode( ";", $cco ) as $end )
	{
		$end = trim( $end );
		if( $end )
		{
			$mensagem->AddBCC( $end );
		}
	}

	return $mensagem->Send();
}


function ulcase($variavel) {
	// altera o primeiro caracter de cada palavra de uma expresa�o, transformando em mai�sculo, com exce��es de artigos, pronomes, etc.
	setlocale("LC_CTYPE","iso_8859_1");
	$nom=explode(" ",$variavel);
	$nom[0]=ucfirst(strtolower($nom[0]));
	for ($j=1;$nom[$j];$j++) {
		$nom[$j]=strtolower($nom[$j]);
		$nom[$j]=ucfirst($nom[$j]);
		if ( ereg("^(Com|Sem|Para|Por|Dos|Das|Ou)$",$nom[$j],$temp ) ) {
			$nom[$j]=strtolower($nom[$j]);
		}
		if ( ereg("^[DN].$",$nom[$j],$temp ) ) {
			$nom[$j]=strtolower($nom[$j]);
		}
		if ( ereg("^[AEO]$",$nom[$j],$temp ) ) {
			$nom[$j]=strtolower($nom[$j]);
		}
	}
	$nom_int = implode($nom," ");
	if ( substr($nom_int,-3,1) == "/" )
	$nom_int = substr($nom_int,0,strlen($nom_int)-3)." - "
	.strtoupper(substr($nom_int,-2));
	return $nom_int;
}
function tirar_acentos($variavel) {
	$busca= array("'�'","'�'","'�'","'�'","'�'","'�'","'�'","'�'","'�'",
	"'�'","'�'","'�'","'�'","'�'",
	"'�'","'�'","'�'","'�'","'�'","'�'","'�'","'�'","'�'",
	"'�'","'�'",
	"'�'","'�'","'�'","'�'","'�'","'�'","'�'","'�'","'�'",
	"'�'","'�'","'�'","'�'","'�'",
	"'�'","'�'","'�'","'�'","'�'","'�'","'�'","'�'","'�'",
	"'�'","'�'");
	$subst= array("A","A","A","A","A","E","E","E","E","I","I","I","I","N",
	"O","O","O","O","O","U","U","U","U","Y","C",
	"a","a","a","a","a","e","e","e","e","i","i","i","i","n",
	"o","o","o","o","o","u","u","u","u","y","c");
	$result = preg_replace($busca,$subst,$variavel);
	return $result;
}
function dv_cpf_cnpj_ok($cpfcnpj) {
	$dv = false;
	$cpfcnpj = ereg_replace("[^0-9]","",$cpfcnpj);
	if ( strlen($cpfcnpj) == 14 ) {
		$cnpj_dv = substr($cpfcnpj,-2);
		for ( $i = 0; $i < 2; $i++ ) {
			$soma = 0;
			for ( $j = 0; $j < 12; $j++ )
			$soma += substr($cpfcnpj,$j,1)*((11+$i-$j)%8+2);
			if ( $i == 1 ) $soma += $digito * 2;
			$digito = 11 - $soma  % 11;
			if ( $digito > 9 ) $digito = 0;
			$controle .= $digito;
		}
		if ( $controle == $cnpj_dv )
		$dv = true;
	}
	if ( strlen($cpfcnpj) == 11 ) {
		$cpf_dv = substr($cpfcnpj,-2);
		for ( $i = 0; $i < 2; $i++ ) {
			$soma = 0;
			for ( $j = 0; $j < 9; $j++ )
			$soma += substr($cpfcnpj,$j,1)*(10+$i-$j);
			if ( $i == 1 ) $soma += $digito * 2;
			$digito = ($soma * 10) % 11;
			if ( $digito == 10 ) $digito = 0;
			$controle .= $digito;
		}
		if ( $controle == $cpf_dv )
		$dv = true;
	}
	return $dv;
}
function formata_fone_fax($numero) {
	$numero = ereg_replace("[^0-9]","",$numero);
	$numero = ereg_replace("^0+","",$numero);
	if ( strlen($numero) == 14 or strlen($numero) == 12 )
	$numero = substr($numero,-10);
	if ( strlen($numero) == 13 or strlen($numero) == 11 )
	$numero = substr($numero,-9);
	$res = substr($numero,-4);
	if ( strlen($numero) > 4  and strlen($numero) < 9 )
	$res = substr($numero,0,strlen($numero)-4)."-".$res;
	if ( strlen($numero) > 8  )
	$res = "(0XX".substr($numero,0,2).") ".
	substr($numero,2,strlen($numero)-6)."-".$res;
	return $res;
}
function formata_cep($num) {
	$num = ereg_replace("[^0-9]","",$num);
	$res = substr($num,0,5);
	if ( strlen($num) > 5 ) $res.="-".substr($num,5,3);
	return $res;
}
function verifica_email ($email) {
	return(preg_match("'^[a-z0-9_.=-]+@(?:[a-z0-9-]+\.)+([a-z]{2,3})\$'i",
	$email));
}
function verifica_cep ($cep) {
	$res=false;
	$cep = ereg_replace("[^0-9]","",$cep);
	if ( strlen($cep) == 8 ) $res=true;
	return $res;
}
function verifica_data($data) {
	$dv = false;
	$data = ereg_replace("[^0-9]","",$data);
	$tam = strlen($data);
	if ( $tam == 8 ) {
		$dia = substr($data,0,2);
		$mes = substr($data,2,2);
		$ano = substr($data,4,4);
		if ( $dia > 0 && $dia < 32 &&
		$mes > 0 && $mes < 13 &&
		$ano > 1900 && $ano < 2100 ) $dv=true;
	}
	return $dv;
}

function verificaPerfil( $pflcods )
{		
	global $db;
	if ( is_array( $pflcods ) )
	{
		$pflcods = array_map( "intval", $pflcods );
		$pflcods = array_unique( $pflcods );
	}
	else
	{
		$pflcods = array( (integer) $pflcods );
	}
	if ( count( $pflcods ) == 0 )
	{
		return false;
	}
	$sql = "
		select
			count(*)
		from seguranca.perfilusuario pu
		inner join seguranca.usuario_sistema as us on us.usucpf = pu.usucpf 		
		where
			pu.usucpf = '" . $_SESSION['usucpf'] . "' and
			pu.pflcod in ( " . implode( ",", $pflcods ) . " ) and
			us.sisid = ".$_SESSION['sisid']."			
		";			
	return $db->pegaUm( $sql ) > 0;
}

/*
 function dv_banco_11_cb($numero) {
 $numero = ereg_replace("[^0-9]","",$numero);
 $len = strlen($numero);
 for ( $i = 0; $i<$len; $i++ )
 $soma+= (($len-$i-1)%8+2) * intval(substr($numero,$i,1));
 $dv = 11 - $soma%11;
 if ( $dv > 9 ) {
 $dv="1";
 }
 return $dv;
 }
 function dv_banco_11($numero,$x=false) {
 $numero = ereg_replace("[^0-9]","",$numero);
 $len = strlen($numero);
 for ( $i = 0; $i<$len; $i++ )
 $soma+= (9-($len-$i-1)%8) * intval(substr($numero,$i,1));
 $dv = $soma%11;
 if ( $dv == 10 ) {
 if ( $x ) $dv="X";
 else $dv="0";
 }
 return $dv;
 }
 function dv_banco_10($numero) {
 $numero = ereg_replace("[^0-9]","",$numero);
 $len = strlen($numero);
 for ( $i = 0; $i<$len; $i++ ) {
 $s = (2-($len-$i-1)%2) * intval(substr($numero,$i,1));
 if ( $s > 9 ) $s = intval(substr($s,0,1)) + intval(substr($s,1,1));
 $soma+=$s;
 }
 $dv = 10-$soma%10;
 if ( $dv == 10 ) $dv = 0;
 return $dv;
 }
 */
function corrige_cpf($cpf)
{
	return ereg_replace("[^0-9]", "", $cpf);

}

function exibir_erros($erros) {
	$num = count($erros);
	if(!$num) return 0;
	echo "Foram encontrado(s) o(s) seguinte(s) erro(s) no seu formul�rio:<br>\n";
	for($x=0;$x<$num;$x++) {
		echo "<br>&nbsp;&nbsp;&nbsp;&nbsp;"."<font color=red><b>".$erros[$x]."</b></font>"."\n";
	}
	return 1;
}
function fator_vencimento($data) {
	$dat = explode("/",$data);
	return intval((mktime(0,0,0,$dat[1],$dat[0],$dat[2])-mktime(0,0,0,10,7,1997))/86400);
}
function limpar_numero($num) {

	for($x=0;$x<strlen($num);$x++) {
		if(ereg("[0-9]",$num[$x])) $saida .= $num[$x];
	}
	return $saida;
}
function md5_senha($senha) {
	return "{md5}".base64_encode(pack("H*",md5($senha)));
}
function campo_data($dt,$obrig,$habil,$label,$formata,$txtdica='',$txtOnChange='', $value = null)
{
	global ${$dt};
	$txtdica = $txtdica ? "return escape('". $txtdica ."');" : null;
	
	$title = "";
	
	if($label)
		$title = ' title="'.$label.'" ';
	
	if ($habil=='S') {
		//dbg(${$dt},0,'37423428787');
		//dbg('rrr'.formata_data(${$dt}),0,'37423428787');
	
		$data = '<input '.$title.' type="text" id="'.$dt.'" name="'.$dt;
		if ($formata=='S') $data.='" value="'.formata_data(($value) ? $value : ${$dt}); else $data.='" value="'.${$dt};
		$data.='" size="12" style="text-align: right;" maxlength="10" value="" class="normal" 
				  onKeyUp="this.value=mascaraglobal('."'##/##/####',this.value);".'" 
				  onchange="' . $txtOnChange . '" onmouseover="MouseOver(this);'. $txtdica .'" 
				  onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" 
				  onblur="MouseBlur(this);this.value=mascaraglobal('."'##/##/####',this.value);".'VerificaData(this, this.value);"> 
				  <a href="javascript:show_calendar('."'formulario.".$dt."');\">".
		'<img src="../imagens/calendario.gif" width="16" height="15" border="0" align="absmiddle" alt=""></a>';
	}
	else
	{
		$data = '<input '.$title.' type="text" id="'.$dt.'" name="'.$dt.
		'" readonly class="normal"value="'.formata_data(($value) ? $value : ${$dt}).
		'" size="12" style="text-align:right;BORDER-LEFT:#888888 3px solid; COLOR:#808080;" maxlength="10">';
	}
	if ($obrig == 'S')
	$data = $data . obrigatorio();

	return $data;
}

/**
 * Cria um campo data2 no formul�rio
 *
 * @param string  $dt - data
 * @param string  $obrig - se � obrigat�rio
 * @param string  $habil - se est� habilitado
 * @param string  $label 
 * @param string  $formata - mascara
 * @param string  $txtdica - Title
 * @param string  $txtOnBlur - fun��es do onblur
 * @param string  $value - valor inicial
 * @param string  $txtOnChange - fun��es do onchange
 * @param string  $classe - classe do input
 * @param string  $id - id do input
 * @return mixed
 */

function campo_data2($dt,$obrig,$habil,$label,$formata,$txtdica='',$txtOnBlur='', $value = null,$txtOnChange='', $classe='', $id='' )
{
	global ${$dt};
	$txtdica = $txtdica ? "return escape('". $txtdica ."');" : null;
	
	$title = "";
	
	if(!$id){
		$id = $dt;
	}
	
	if ($obrig == 'S'){
		$clsObrig = " obrigatorio ";
	}
	
	if($label)
		$title = ' title="'.$label.'" ';
	
	if ($habil=='S') {
		$data = '<input '.$title.' type="text" id="'.$id.'" name="'.$dt;
		if ($formata=='S') 
			$data.='" value="'.formata_data(($value) ? $value : ${$dt});
		else 
			$data.='" value="'.( ($value) ? $value : ${$dt} );
		
		$data.='" size="12" style="text-align: right;" maxlength="10" value="" class="normal '.$clsObrig.$classe.'" 
				  onKeyUp="this.value=mascaraglobal('."'##/##/####',this.value);".'" 
				  onchange="' . $txtOnChange . '" onmouseover="MouseOver(this);'. $txtdica .'" 
				  onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" 
				  onblur="validando_data(this);' . $txtOnBlur . 'MouseBlur(this);this.value=mascaraglobal(\'##/##/####\',this.value)">'; 
		$data.= "&nbsp;<img src=\"../includes/JsLibrary/date/displaycalendar/images/calendario.gif\" align=\"absmiddle\" border=\"0\" style=\"cursor:pointer\" title=\"Escolha uma Data\" onclick=\"displayCalendar(document.getElementById('$id'),'dd/mm/yyyy',this)\">";
	} else {
		$data = '<input '.$title.' type="text" id="'.$dt.'" name="'.$dt.
		'" readonly class="normal '.$classe.'" value="'.formata_data(($value) ? $value : ${$dt}).
		'" size="12" style="text-align:right;BORDER-LEFT:#888888 3px solid; COLOR:#808080;" maxlength="10">';
	}
	
	if ($obrig == 'S')
		$data = $data . obrigatorio();
		
	return $data;
}

/**
 * Cria um campo texto no formul�rio
 *
 * @param string $var
 * @param string $obrig
 * @param string $habil
 * @param string $label
 * @param integer $size
 * @param integer $max
 * @param string $masc
 * @param string $hid
 * @param string $align
 * @param string $txtdica
 * @param integer $acao
 * @param string $complemento
 * @param string $evtkeyup
 * @param string $value
 * @param string $evtblur
 * @param array  $arrStyle: Variavel onde podem ser passados parametros de estilo, sendo o �ndice a propriedade. ex: array('width' =>'100%')
 * @return mixed
 */
function campo_texto($var,$obrig,$habil,$label,$size,$max,$masc,$hid, $align='left', $txtdica='',$acao=0, $complemento='',$evtkeyup='', $value = null, $evtblur='', $arrStyle = null )
{
	
	global ${$var};
	$value = $value != '' ? $value : ${$var};
	  
	if ( $obrig == 'S' ) $class = 'obrigatorio';	
	
	$arrStyle['text-align'] = $align;
	
	if(is_array($arrStyle)){
		$sty = 'style="';
		foreach($arrStyle as $chaves=>$dados){
			$sty .= $chaves.':'.$dados.';';
		}
		$sty .= '"';
	}
	
	if ($hid=='S') $dif = '1';
	$texto = '<input type="text" '.$sty.' name="'.$var.$dif.'" size="'.($size+1).'" maxlength="'.$max. '" value="'.$value.'"';
	if ($masc !="")
	{
		if($evtkeyup != "")
		{
			$texto = $texto. ' onKeyUp= "this.value=mascaraglobal(\'' . $masc . '\',this.value);' . $evtkeyup . '"';
		}
		else
		{
			$texto = $texto. ' onKeyUp= "this.value=mascaraglobal(\'' . $masc . '\',this.value);"';
		}
	}
	else
	{
		if($evtkeyup != "")
		$texto = $texto. ' onKeyUp= "' . $evtkeyup . '"';
	}
	$habil != 'N' ? $class.=" normal"  :   $class="disabled";

	if ( $habil == 'N' )
	{
		if ( !$complemento )
		$texto = $texto.' readonly="readonly" style="width:'.($size+3).'ex;text-align : '.$align.';" ';
		else
		$texto = $texto.' ' . $complemento .' readonly="readonly" style="width:'.($size+3).'ex;text-align : '.$align.';" ';
		//if ($habil == 'N') $texto = $texto.' readonly ';
	}
	else
	{
		$texto .= ' onmouseover="MouseOver(this);';
		if ( $txtdica ){
			$texto .= 'return escape(\''.$txtdica.'\');';
		}
		if ( !$complemento ){
			if( $arrStyle ){
				$style = 'style="text-align : '.$align.'; width:'.($size+3).'ex;"';
			} else {
				$style = $sty;				
			}
			$texto .= '" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);' . $evtblur . '" '.$style.' ';
		} else {
			$texto .= '" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);' . $evtblur . '" ' . $complemento . ' '.$style.' ';
		}
	}


	$texto = $texto . " title='$label' class='$class' />";

	if ($obrig == 'S')
	$texto = $texto . obrigatorio();
	if ($hid == 'S'){
		$texto = $texto."<input type='hidden' name='".$var."' value ='".$value."'>";
	}
	if ($acao)
	{
		//  	unidade,unicod,uni,combo,ppaacao,Unidade Or�ament�ria Respons�vel, and substr(unicod,1,2) in '26'
		$partes = explode(";", $acao);
		$alvo=$partes[0];
		$campo=$partes[1];
		$padrao=$partes[2];
		$tipo=$partes[3];
		$origem=$partes[4];
		$nome_campo=$partes[5];
		$especial=$partes[6];
		$especial2=$partes[7];

		$texto .= '&nbsp;<img border="0" src="../imagens/alterar.gif" title="Editar o campo." onclick='.'"edita_campo('."'$alvo','$campo','$padrao','$tipo','$origem','$nome_campo'";
		//if ($especial)
		$texto .= ",'$especial'";
		//if ($especial2)
		$texto .= ",'$especial2'";
		$texto .= ')">';


	}
	return $texto;
}

function campo_bool($var,$obrig,$habil,$label,$size,$max,$masc,$hid, $align='left', $txtdica='',$acao=0)
{
	global ${$var};
	if (${$var}=='t') ${$var}=$txtdica.'-> Sim'; else ${$var}=$txtdica.'-> N�o';
	if ($hid=='S') $dif = '1';
	$texto = '<input type="text" name="'.$var.$dif.
	'" size="'.($size+1).'" maxlength="'.$max. '" value="'.${$var}.'" class="normal"';
	if ($masc <>"")
	$texto = $texto. ' onKeyUp= "this.value=mascaraglobal('."'".$masc."',this.value);".'"';

	if ($habil == 'N') $texto = $texto.' readonly style="BORDER:none;BORDER-LEFT:#888888 3px solid;COLOR:#404040;width:'.$size.'ex;text-align : '.$align.';" ';
	else {$texto .= ' onmouseover="MouseOver(this);';

	if($txtdica) $texto.= 'return escape(\''.$txtdica.'\');'; $texto.= '" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="text-align : '.$align.'; width:'.($size+3).'ex;"';}

	$texto = $texto . " title='$label'>";

	if ($obrig == 'S')
	$texto = $texto . obrigatorio();
	if ($hid == 'S'){
		$texto = $texto."<input type='hidden' name='".$var."' value ='".${$var}."'>";
	}
	if ($acao)
	{
		//  	unidade,unicod,uni,combo,ppaacao,Unidade Or�ament�ria Respons�vel, and substr(unicod,1,2) in '26'
		$partes = explode(";", $acao);
		$alvo=$partes[0];
		$campo=$partes[1];
		$padrao=$partes[2];
		$tipo=$partes[3];
		$origem=$partes[4];
		$nome_campo=$partes[5];
		$especial=$partes[6];
		$especial2=$partes[7];

		$texto .= '&nbsp;<img border="0" src="../imagens/alterar.gif" title="Editar o campo." onclick='.'"edita_campo('."'$alvo','$campo','$padrao','$tipo','$origem','$nome_campo'";
		//if ($especial)
		$texto .= ",'$especial'";
		//if ($especial2)
		$texto .= ",'$especial2'";
		$texto .= ')">';


	}
	return $texto;
}


/**
 * Faz um input de texto com busca de dados no servidor.
 * A consulta sql precisa ter um %s com o valor que ser� buscado
 *
 * @param string $nome
 * @param string $label
 * @param mixed $valor
 * @param integer $size
 * @param integer $maxlength
 * @param string $mascara
 * @param string $align
 * @param boolean $obrigatorio
 * @param boolean $habilitado
 * @param boolean $hidden
 * @param string $complemento conte�do que vai dentro da tag input, os eventos onfocus, onkeyup e onkeydown n�o podem ser sobreescritos
 * @param string[] $parametros_extra envia na requisi��o ajax os valores dos campos com os nomes indicados
 * @param boolean $valida
 * @return string
 *
 * @author Adonias Malosso <malosso@gmail.com>
 * @version 1.0
 */
function campo_texto_ajax( $sql, $id, $nome, $label='', $valor='', $size='', $maxlength='', $mascara='', $align='left', $obrigatorio=false, $habilitado=true, $hidden=false, $label_externo = false, $complemento = '', $parametros_extra = array(), $valor_label = '', $valida = false )
{
	// � preciso passar o comando SQL a ser executado com '%s' no lugar do valor do cmapo
	if(strpos($sql, "%s") === false ) {
		return false;
	}

	$jsCssCampo = $extraCampo = '';

	$_SESSION["SQLAJAX"][$id] = $sql;

	$size += $habilitado ? 1 : 0;
	// se usar o campo hidden, altera o nome do campo normal
	$idCampo = $id;
	$nomeCampo	 = $hidden ? $nome . "1" :  $nome;
	// se campo obrigatorio, apresentar indicador
	if ( $label_externo )
	{
		$extraCampo .= '<span id="LSLabel_' . $idCampo . '">' . $valor_label . '</span>';
	}
	$extraCampo .= $obrigatorio ? "&nbsp;<img src=\"/imagens/obrig.gif\" title=\"Campo $label obrigat�rio.\" border=\"0\">" : "";
	// colocar campo hidden se necessario
	$extraCampo .= $hidden ? "<input type=\"hidden\" name=\"" . $nome . "\" value=\"" . $valor ."\">" : "";
	// adiciona label ao campo


	// acrescenta javascript para a mascara do campo
	if ($mascara <> "")
	$jsCssCampo .= " onKeyUp=\"this.value=mascaraglobal('" . $mascara . "',this.value);\"";

	if(!$habilitado)
	$jsCssCampo .= " readonly style=\"border: none; border-left: #888888 3px solid; color: #404040; width: " . $size . "ex; text-align: ". $align . ";\"";
	else
	$jsCssCampo .= " style=\"text-align: " . $align . "; width:" . $size . "ex;\"";
	//		$jsCssCampo .= " onmouseover=\"MouseOver(this)\" onfocus=\"MouseClick(this);alert(123);liveSearchStart(this);\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" style=\"text-align: " . $align . "; width:" . $size . "ex;\"";

	$campos_extra = '';
	if ( count( $parametros_extra ) )
	{
		$campos_extra .= " '" . implode( "','", $parametros_extra ) . "' ";
	}
	$extra .= 'new Array(' . $campos_extra . ')';
	$valida_valor = $valida ? 'true' : 'false' ;
	$jsImg = <<<EOS
	var campo = document.getElementById( '$idCampo' );
	 if ( liveSearchStart( campo, $extra ) )
	 {
	 	campo.value = '';
	 	lsFazerBuscar( campo );
	 }
	 else
	 {
	 	MouseBlur( this );
	 }
EOS;
	return <<<EOS
	<div class="divcampoajax">
		<input type="text" name="$nomeCampo" id="$idCampo" size="$size" maxlength="$maxlength" value="$valor" class="normal" title="$label" $jsCssCampo onmouseover="MouseOver(this);" onmouseout="MouseOut(this);" onfocus="if ( liveSearchStart( this, $extra ) ) MouseClick( this ); else MouseBlur( this );" $complemento /><img id="lsImagem_$idCampo" src="/imagens/seta_combo.gif" width="14" height="16" style="margin:0;padding:0;" align="absmiddle" onclick="$jsImg" />
		$extraCampo
	</div>
	<div id="LSResult_$idCampo" class="LSResult" style="display: none;"></div>
	<input type="hidden" id="hidden_$nome" name="hidden_$nome" value="$valor_label"/>
	<script language="javascript">
		document.getElementById( '$idCampo' ).valida = $valida_valor;
	</script>
EOS;
}

/**
 * Monta um campo texto ajax em cascata. Caso o campo que ele �
 * dependente seja edita o valor deste � apagado. As dependencias
 * devem ser passadas na ordem de modo que somente o �ltimo � verificado.
 *
 * @param string $sql
 * @param string $nome
 * @param string $descricao
 * @param string[] $dependencias
 * @param integer $size
 * @param integer $maxlength
 * @param string $mascara
 * @param string $align
 * @param boolean $obrigatorio
 * @return string
 */
function campo_texto_ajax_cascata( $sql, $nome, $descricao, $dependencias, $size = '', $maxlength = '', $mascara = '', $align = 'left', $obrigatorio = false, $valor = '', $valor_label = '' )
{
	$ultima_dependencia = end( $dependencias );
	// adiciona valor do campo caso ele j� tenha iniciado com algum valor
	$jsLabel = $valor ? " lsLabel[$nome] = '$valor_label'; " : '' ;
	$jsLabel = '';
	$javascript = <<<EOS
		<script type="text/javascript">
			var tamanho = lista_campo_cascata.length;
			lista_campo_cascata[tamanho] = new Array();
			lista_campo_cascata[tamanho]['nome'] = '$nome';
			lista_campo_cascata[tamanho]['dependencia'] = '$ultima_dependencia';
			lista_campo_cascata[tamanho]['valor'] = '${$nome}';
			$jsLabel
		</script>
EOS;
			return $javascript . campo_texto_ajax( $sql, $nome, $nome, $descricao, $valor, $size, $maxlength, $mascara, $align, $obrigatorio, true, false, true, '', $dependencias, $valor_label, true );
}


/**
 * Monta campo textarea.
 *
 * @param string $var nome do campo
 * @param string $obrig obrigat�rio ou n�o S|N
 * @param string $habil habilitado ou n�o S|N
 * @param string $label ???
 * @param string $cols quantidade de coluna
 * @param string $rows quantidade de linhas
 * @param string $max tamanho m�ximo de caracteres
 * @param string $funcao ???
 * @param string $acao ???
 * @param string $txtdica texto de dica, exibido quando o mouse fica sob o textarea
 * @param string $tab habilita digita��o do tab ou n�o true|false
 * @return string
 */
function campo_textarea( $var, $obrig, $habil, $label, $cols, $rows, $max, $funcao = '', $acao = 0, $txtdica = '', $tab = false, $title = NULL, $value = null)
{
	global ${$var};
	$value = $value ? $value : ${$var};
	
	$texto = '';
	
	if ( $obrig == 'S' ) $class = 'obrigatorio ';	
	
	if($title){
		$title = 'title="'.$title.'"';
	}
	// verifica se h� texto de dica e cria span onde o texto ficar� vis�vel 
	// ao final da fun��o a mesma verifica��o � realizada para fechar o tag
	if ( $txtdica )
	{
		$texto .= '<span onmouseover="return escape( \'' . $txtdica . '\' );">';
	}
	// inicia o campo textarea
	$texto .= '<textarea  id="' . $var . '" name="' . $var . '" cols="' . $cols . '" rows="' . $rows . '" ' . $funcao . ' '  . $title;
	$style = ''; // cont�m os estilos a serem aplicados ao textarea
	// monta atributos do textarea de acordo com a habilita��o ou n�o do campo
	if ( $habil == 'N' )
	{
		$texto .= ' readonly="readonly" disabled="disabled" ';
		$style .= 'border:none;border-left:#888888 3px solid;color:#404040;width:' . $cols . 'ex;text-align:' . $align . ';';
	}
	else
	{
		$txtdica = $txtdica ? 'return escape( \'' . htmlentities( $txtdica ) . '\' );' : '' ;
		$texto .= ' onmouseover="MouseOver( this );' . $txtdica . '"';
		$texto .= ' onfocus="MouseClick( this );" ';
		$texto .= ' onmouseout="MouseOut( this );" ';
		if ($max){
			$texto .= ' onblur="MouseBlur( this ); textCounter( this.form.' . $var . ', this.form.no_' . $var . ', ' . $max . ');" ';		
		}else {
			$texto .= ' onblur="MouseBlur( this );" ';
		}
	
		$style .= 'width:' . $cols . 'ex;';
	}
	// habilita tab, quando o tab � habilitado o estilo � alterado para
	// uma classe onde a fonte � monoespe�ada
	$keydown = '';
	if ( $tab == true )
	{
		//$keydown = 'return readTabChar( event, this );';
		$style .= 'font-family:monospace;font-size:8pt;';
	}
	// aplica estilos
	$texto .= ' style="' . $style . '" ';
	// verifica se � para habilitar checagem de tamanho do campo
	if ( $max && $habil == 'S' )
	{
		$texto .= ' onkeydown="textCounter( this.form.' . $var . ', this.form.no_' . $var . ', ' . $max . ' );' . $keydown . '" ';
		$texto .= ' onkeyup="textCounter( this.form.' . $var . ', this.form.no_' . $var . ', ' . $max . ');" ';
	}
	// finaliza cria��o do campo textarea
	$texto .= ' class="'.$class.'txareanormal" >' . str_replace( '&nbsp;', '&amp;nbsp;', ($value) ? $value : ${$var} ) . '</textarea>';
	// mostra indicador de obrigatoriedade do campo
	
	if ( $obrig == 'S' )$texto = $texto . obrigatorio();
	// mostra tamanho m�ximo do conte�do caso haja tamanho m�ximo
	if ( $max && $habil=='S' )
	{
		$texto .= "<br><input readonly style=\"text-align:right;border-left:#888888 3px solid;color:#808080;\" type=\"text\" name=\"no_" . $var . "\" size=\"6\" maxlength=\"6\" value=\"". ( $max - strlen( $value ) ) . "\" class=\"CampoEstilo\"><font color=\"red\" size=\"1\" face=\"Verdana\"> m�ximo de caracteres</font>";
	}
	// ???
	if ( $acao )
	{
		$partes = explode( ';', $acao);
		$alvo       = $partes[0];
		$campo      = $partes[1];
		$padrao     = $partes[2];
		$tipo       = $partes[3];
		$origem     = $partes[4];
		$nome_campo = $partes[5];
		$especial   = $partes[6];
		$texto = $texto . '&nbsp;<img border="0" src="../imagens/alterar.gif" title="Editar o campo." onclick="edita_campo( ' . "'$alvo', '$campo', '$padrao', '$tipo', '$origem', '$nome_campo', '$especial'" . ' )">';
	}
	// fecha tag span do texto dica
	// olhar coment�rio no in�cio da fun��o que iniciar o tag
	if ( $txtdica )
	{
		$texto .= "</span>";
	}
	return
		'<div class="notprint">' .
	$texto .
		'</div>' .
		'<div id="print_' . $var . '" class="notscreen" style="text-align: left;">' .
	${$var} .
		'</div>';
}

function procura_array($search_value, $the_array)
{
	if (is_array($the_array))
	{
		foreach ($the_array as $key => $value)
		{
			$result = procura_array($search_value, $value);
			if (is_array($result))
			{
				$return = $result;
				array_unshift($return, $key);
				return $return;
			}
			elseif ($result == true)
			{
				$return[] = $key;
				return $return;
			}
		}
		return false;
	}
	else
	{
		if ($search_value == $the_array)
		{
			return true;
		}
		else return false;
	}
}

function monta_arvore_menu($cod)
{
	$sql = 'select * from menu where mnucodpai = '.$cod;
	$saida = $db->recuperar($sql);
	if(is_array($saida)) foreach($saida as $k=>$v) ${$k}=$v;
	if ($mnusnsubmenu == 't') monta_arvore_menu($mnucod);
	else print "<td><input type='checkbox' name='cb'$mnucod</td><td>$mnucod</td><td>$mnudsc</td>";
}


function mes_extenso($mes)
{
	if (strval($mes) == 1) return 'JANEIRO';
	else   if (strval($mes) == 2) return 'FEVEREIRO';
	else   if (strval($mes) == 3) return 'MAR�O';
	else   if (strval($mes) == 4) return 'ABRIL';
	else   if (strval($mes) == 5) return 'MAIO';
	else   if (strval($mes) == 6) return 'JUNHO';
	else   if (strval($mes) == 7) return 'JULHO';
	else   if (strval($mes) == 8) return 'AGOSTO';
	else   if (strval($mes) == 9) return 'SETEMBRO';
	else   if (strval($mes) == 10) return 'OUTUBRO';
	else   if (strval($mes) == 11) return 'NOVEMBRO';
	else   if (strval($mes) == 12) return 'DEZEMBRO';
}

function str_to_upper($str){
	return strtr($str,
	"abcdefghijklmnopqrstuvwxyz".
	"\x9C\x9A\xE0\xE1\xE2\xE3".
	"\xE4\xE5\xE6\xE7\xE8\xE9".
	"\xEA\xEB\xEC\xED\xEE\xEF".
	"\xF0\xF1\xF2\xF3\xF4\xF5".
	"\xF6\xF8\xF9\xFA\xFB\xFC".
	"\xFD\xFE\xFF",
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ".
	"\x8C\x8A\xC0\xC1\xC2\xC3\xC4".
	"\xC5\xC6\xC7\xC8\xC9\xCA\xCB".
	"\xCC\xCD\xCE\xCF\xD0\xD1\xD2".
	"\xD3\xD4\xD5\xD6\xD8\xD9\xDA".
	"\xDB\xDC\xDD\xDE\x9F");
}

function grid($name, &$data){

	$row_count = @pg_num_rows($data);
	$column_count = @pg_numfields($data);


	$columns = "var ".$name."_columns = [\n";
	for ($i=0; $i < $column_count; $i++) {
		$columns .= "\"".@pg_field_name($data, $i)."\", ";
	}
	$columns .= "\n];\n";


	$rows = "var ".$name."_data = [\n";
	while ($result = @pg_fetch_array($data)) {
		$rows .= "[";
		for ($i=0; $i < $column_count; $i++) {
			$rows .= "\"".grid_html($result[$i])."\", ";
		}
		$rows .= "],\n";
	}
	$rows .= "];\n";

	$html = "<"."script".">\n";
	$html .= $columns;
	$html .= $rows;
	$html .= "try {\n";
	$html .= "  var $name = new Active.Controls.Grid;\n";
	$html .= "  $name.setId(\"grid1\");";
	$html .= "  $name.setRowCount($row_count);\n";
	$html .= "  $name.setColumnCount($column_count);\n";
	$html .= "  $name.setDataText(function(i, j){return ".$name."_data[i][j]});\n";
	$html .= "  $name.setColumnText(function(i){return ".$name."_columns[i]});\n";
	$html .= "  document.write($name);\n";
	$html .= "}\n";
	$html .= "catch (error){\n";
	$html .= "  document.write(error.description);\n";
	$html .= "}\n";
	$html .= "</"."script".">\n";

	print $html;
}

function grid_html($msg){

	$msg = addslashes($msg);
	$msg = str_replace("\n", "\\n", $msg);
	$msg = str_replace("\r", "\\r", $msg);
	$msg = htmlspecialchars($msg);

	return $msg;
}

function dbg($var=NULL, $morte=FALSE, $usucpf=''){
	if ( $usucpf AND $usucpf != $_SESSION['usucpforigem'] )	return;
	print "/*<center>-- ----- ----- ----- ----- ----- ----- ----- ----- -----\n ";
	print "<strong>DEBUG INICIO</strong>";
	print " ----- ----- ----- ----- ----- ----- ----- ----- ----- --</center>\n";
	$array = debug_backtrace();
	print("<center>linha '". $array[0]['line'] ."' do arquivo '". $array[0]['file'] ."'</center>\n");
	print("<pre><font size='3'>\n");
	var_dump($var);
	print("</font></pre>\n");
	print "<center>---- ----- ----- ----- ----- ----- ----- ----- ----- ----- \n";
	print "<strong>DEBUG FIM</strong>";
	print " ----- ----- ----- ----- ----- ----- ----- ----- ----- ----</center>\n";
	if( $morte )
	die("<center><font color=\"#ff0000\"><strong>D I E</strong></font></center>\n*/");
}

define( "d", "die" );

function ver(){
	
	$parar = false;

	$caminho = array_shift(debug_backtrace());
	echo '
		<div style="background-color: #ddd; color: #000; font-size: 14px;">
		<div>
			<label>
				Linha   --> <label class="valorCaminho">'.$caminho['line']	  .'</label><br />
				Caminho --> <label class="valorCaminho">'.$caminho['file']	  .'</label><br />
			</label>
		</div>';
	
	echo '<div><pre>';
	
	
	foreach( $caminho['args'] as $indice => $valor ){
	
		if(  $valor == 'die' ) $parar = true;
		echo '<label style="color:red; font-weight: bold;">Argumento '.( $indice + 1 ).'</label><br />';
		print_r( $valor );
		echo '<br /><br />';
		
	}
	
	echo '</div></pre></div>';
	
	
	// Verifica se � para dar o die no final
	if( $parar ){ die; }
}

function monta_grafico_validacao($vl0,$vl1,$vl2,$vl3,$vltotal)
{
	$vl0p = number_format($vl0*100/$vltotal,2,'.',',');
	$vl1p = number_format($vl1*100/$vltotal,2,'.',',');
	$vl2p = number_format($vl2*100/$vltotal,2,'.',',');
	$vl3p = number_format($vl3*100/$vltotal,2,'.',',');
	$txtgrafico = "$vltotal A��es no Total:<br>$vl1 <img src=\'../imagens/cor1.gif\' style=\'height:7;width:7;border:1px solid #888888;\'> Concord�ncia ($vl1p%)<br>$vl2 <img src=\'../imagens/cor2.gif\' style=\'height:7;width:7;border:1px solid #888888;\'> Pendente ($vl2p%)<br>$vl3 <img src=\'../imagens/cor3.gif\' style=\'height:7;width:7;border:1px solid #888888;\'> Discord�ncia ($vl3p%)<br>$vl0 <img src=\'../imagens/cor0.gif\' style=\'height:7;width:7;border:1px solid #888888;\'> N�o Validado ($vl0p%)";
	$imggrafico = "<span onmouseover=\"return escape('$txtgrafico')\"><img src='../imagens/cor1.gif' style='height:10;width:".($vl1p/2).";border:1px solid #888888;border-right:0'><img src='../imagens/cor2.gif' style='height:10;width:".($vl2p/2).";border:1px solid #888888;border-right:0;border-left:0;border-right:0;border-left:0'><img src='../imagens/cor3.gif' style='height:10;width:".($vl3p/2).";border:1px solid #888888;border-left:0;border-right:0'><img src='../imagens/cor0.gif' style='height:10;width:".($vl0p/2).";border:1px solid #888888;border-left:0'></span>";
	return $imggrafico;
}


function monta_grafico_avaliacao( $vl0, $vl1, $vl2, $vl3, $vltotal, $mes, $percent )
{
	$vl0p = number_format($vl0*100/$vltotal,0,'.',',');
	$vl1p = number_format($vl1*100/$vltotal,0,'.',',');
	$vl2p = number_format($vl2*100/$vltotal,0,'.',',');
	$vl3p = number_format($vl3*100/$vltotal,0,'.',',');
	/*
	 $txtgrafico = "<b>$mes</b> - Total: $vltotal A��es<br> ". ( $vl1+$vl2+$vl3 ) ." Avaliada(s) / Preenchida(s) $percent<br><table border=0><tr><td>$vl1</td><td><img src=\'../imagens/cor1.gif\' style=\'height:7;width:$vl1p;border:1px solid #888888;\'> Est�vel ($vl1p%)</td></tr><tr><td>$vl2</td><td><img src=\'../imagens/cor2.gif\' style=\'height:7;width:$vl2p;border:1px solid #888888;\'> Merece Aten��o ($vl2p%)</td></tr><tr><td>$vl3</td><td><img src=\'../imagens/cor3.gif\' style=\'height:7;width:$vl3p;border:1px solid #888888;\'> Cr�tico ($vl3p%)</td></tr><tr><td>$vl0</td><td><img src=\'../imagens/cor0.gif\' style=\'height:7;width:$vl0p;border:1px solid #888888;\'> N�o Avaliada ($vl0p%)</td></tr></table>";
	 $imggrafico = "<span onmouseover=\"return escape('$txtgrafico')\"><img src='../imagens/cor1.gif' style='height:10;width:".($vl1p/3).";border:1px solid #888888;border-right:0'><img src='../imagens/cor2.gif' style='height:10;width:".($vl2p/3).";border:1px solid #888888;border-right:0;border-left:0;border-right:0;border-left:0'><img src='../imagens/cor3.gif' style='height:10;width:".($vl3p/3).";border:1px solid #888888;border-left:0;border-right:0'><img src='../imagens/cor0.gif' style='height:10;width:".($vl0p/3).";border:1px solid #888888;border-left:0'></span>";
	 return $imggrafico;
	 */
	$url = '../' . $_SESSION['sisdiretorio'] . '/geral/infoMonitora.php' .
	'?vl0=' . $vl0 .
	'&vl1=' . $vl1 .
	'&vl2=' . $vl2 .
	'&vl3=' . $vl3 .
	'&vltotal=' . $vltotal .
	'&mes=' . $mes .
	'&percent=' . $percent;
	return "
		<span	onmousemove=\"SuperTitleAjax( '" . $url . "', this );\"
				onmouseout=\"SuperTitleOff( this );\">
			<img src='../imagens/cor1.gif' style='height:10;width:".($vl1p/3).";border:1px solid #888888;border-right:0'><img src='../imagens/cor2.gif' style='height:10;width:".($vl2p/3).";border:1px solid #888888;border-right:0;border-left:0;border-right:0;border-left:0'><img src='../imagens/cor3.gif' style='height:10;width:".($vl3p/3).";border:1px solid #888888;border-left:0;border-right:0'><img src='../imagens/cor0.gif' style='height:10;width:".($vl0p/3).";border:1px solid #888888;border-left:0'>
		</span>";
}

/*
 function monta_grafico_avaliacao_resultado( $vl0,$vl1,$vl2,$vl3,$vltotal,$mes,$percent )
 {
 $vl0p = number_format( $vl0 * 100 / $vltotal, 0, '.', ',' );
 $vl1p = number_format( $vl1 * 100 / $vltotal, 0, '.', ',' );
 $vl2p = number_format( $vl2 * 100 / $vltotal, 0, '.', ',' );
 $vl3p = number_format( $vl3 * 100 / $vltotal, 0, '.', ',' );
 return "<b>$mes</b> - Total: $vltotal A��es<br>$percent  Avaliada(s) / Preenchida(s)<br><table border=0><tr><td>$vl1</td><td><img src=\'../imagens/cor1.gif\' style=\'height:7;width:$vl1p;border:1px solid #888888;\'> Est�vel ($vl1p%)</td></tr><tr><td>$vl2</td><td><img src=\'../imagens/cor2.gif\' style=\'height:7;width:$vl2p;border:1px solid #888888;\'> Merece Aten��o ($vl2p%)</td></tr><tr><td>$vl3</td><td><img src=\'../imagens/cor3.gif\' style=\'height:7;width:$vl3p;border:1px solid #888888;\'> Cr�tico ($vl3p%)</td></tr><tr><td>$vl0</td><td><img src=\'../imagens/cor0.gif\' style=\'height:7;width:$vl0p;border:1px solid #888888;\'> N�o Avaliada ($vl0p%)</td></tr></table>";
 }

 function monta_grafico_avaliacao( $vl0, $vl1, $vl2, $vl3, $vltotal, $mes, $percent )
 {
 $vl0p = number_format( $vl0 * 100 / $vltotal, 0, '.', ',' );
 $vl1p = number_format( $vl1 * 100 / $vltotal, 0, '.', ',' );
 $vl2p = number_format( $vl2 * 100 / $vltotal, 0, '.', ',' );
 $vl3p = number_format( $vl3 * 100 / $vltotal, 0, '.', ',' );
 //$onmouseover = "monta_grafico_avaliacao( '$vl0', '$vl1', '$vl2', '$vl3', '$vltotal', '$mes', '$percent' );";
 //$onmouseover = "alert( 4 );";
 $imggrafico = "<span onmouseover=\"" . $onmouseover . "\"><img src='../imagens/cor1.gif' style='height:10;width:" . ( $vl1p / 3 ) . ";border:1px solid #888888;border-right:0'><img src='../imagens/cor2.gif' style='height:10;width:" . ( $vl2p / 3 ) . ";border:1px solid #888888;border-right:0;border-left:0;border-right:0;border-left:0'><img src='../imagens/cor3.gif' style='height:10;width:".($vl3p/3).";border:1px solid #888888;border-left:0;border-right:0'><img src='../imagens/cor0.gif' style='height:10;width:" . ( $vl0p / 3 ) . ";border:1px solid #888888;border-left:0'></span>";
 return $imggrafico;
 }
 */


/**
 * Monta um combo especial com sele��o em popups.
 *
 * Monta apenas o campo do formul�rio. Consulte as observa��es indicada
 * para cada par�metro. � necess�rio que o campo identificador seja
 * renomeado para 'codigo' e a descri��o para 'descricao'. Veja restante
 * dos coment�rio do arquivo www/geral/combopopup.php
 *
 * @param string $nome nome atribuido ao campo no formul�rio
 * @param string $sql consulta a ser realizada no banco
 * @param string $titulo titulo que dever� aparecer no popup
 * @param string $tamanho_janela tamanho da janela popup no formato HEIGHTxWIDTH
 * @param integer $maximo_itens m�ximo de itens que podem ser selecionados ( se igual a 0 quantidade � ilimitada )
 * @param string[] $codigos_fixos c�digos que o usu�rio n�o pode remover do combo
 * @param string $mensagem_fixo mensagem que ser� exibida caso o usu�rio tente remover um item fixo
 * @param string $habilitado indica se o campo est� aberto para edi��o ( S | N )
 * @param boolean $campo_busca_codigo
 * @param boolean $campo_flag_contem
 * @param integer $size
 * @param integer $width
 * @param string $funcaoJS Fun��o Javascript a ser chamada quando se escolhe uma op��o da popup(onclick). � passado por par�metro para esta fun��o js o objeto(this) (Adicionado por: Felipe Carvalho)
 * @return void
 */
function combo_popup( $nome, $sql, $titulo, $tamanho_janela = '400x400', $maximo_itens = 0,
$codigos_fixos = array(), $mensagem_fixo = '', $habilitado = 'S', $campo_busca_codigo = false,
$campo_flag_contem = false, $size = 10, $width = 400 , $onpop = null, $onpush = null, $param_conexao = false, $where=null, $value = null, $mostraPesquisa = true, $campo_busca_descricao = false, $funcaoJS=null, $intervalo=false, $arrVisivel = null , $arrOrdem = null)
{
	global ${$nome};
	unset($dados_sessao);
	// prepara parametros
	$maximo_itens = abs( (integer) $maximo_itens );
	$codigos_fixos = $codigos_fixos ? $codigos_fixos : array();
	// prepara sess�o
	$dados_sessao = array(
	'sql' => (string) $sql, // o sql � armazenado para ser executado posteriormente pela janela popup
	'titulo' => $titulo,
	'indice' => $indice_visivel,
	'maximo' => $maximo_itens,
	'codigos_fixos' => $codigos_fixos,
	'mensagem_fixo' => $mensagem_fixo,
	'param_conexao' => $param_conexao,
	'where'			=> $where,
	'mostraPesquisa'=> $mostraPesquisa,
	'intervalo'     => $intervalo,
	'arrVisivel'    => $arrVisivel,
	'arrOrdem'     => $arrOrdem
	);
	
	if ( !isset( $_SESSION['indice_sessao_combo_popup'] ) )
	{
		$_SESSION['indice_sessao_combo_popup'] = array();
	}
	unset($_SESSION['indice_sessao_combo_popup'][$nome]);
	$_SESSION['indice_sessao_combo_popup'][$nome] = $dados_sessao;
	
	// monta html para formulario
	$tamanho    = explode( 'x', $tamanho_janela );
	$onclick    = ' onclick="javascript:combo_popup_alterar_campo_busca( this );" ';
	
	/*** Adiciona a fun��o Javascript ***/
	$funcaoJS = (is_null($funcaoJS)) ? 'false' : "'" . $funcaoJS . "'";
	
	$ondblclick = ' ondblclick="javascript:combo_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ', '.$funcaoJS.' );" ';
	$ondelete   = ' onkeydown="javascript:combo_popup_remove_selecionados( event, \'' . $nome . '\' );" ';
	$onpop		= ( $onpop == null ) ? $onpop = '' : ' onpop="' . $onpop . '"';
	$onpush		= ( $onpush == null ) ? $onpush = '' : ' onpush="' . $onpush . '"';
	$habilitado_select = $habilitado == 'S' ? '' : ' disabled="disabled" ' ;
	$select =
	'<select ' .
	'maximo="'. $maximo_itens .'" tipo="combo_popup" ' .
	'multiple="multiple" size="' . $size . '" ' .
	'name="' . $nome . '[]" id="' . $nome . '" '.
	$onclick . $ondblclick . $ondelete . $onpop . $onpush  .
	'class="CampoEstilo" style="width:' . $width . 'px;" ' .
	$habilitado_select .
	'>';
	
	if($value && count( $value ) > 0){
		$itens_criados = 0;
		foreach ( $value as $item )
		{
			$select .= '<option value="' . $item['codigo'] . '">' . htmlentities( $item['descricao'] ) . '</option>';
			$itens_criados++;
			if ( $maximo_itens != 0 && $itens_criados >= $maximo_itens )
			{
				break;
			}
		}
	} elseif ( ${$nome} && count( ${$nome} ) > 0 ) {
		$itens_criados = 0;
		foreach ( ${$nome} as $item )
		{
			$select .= '<option value="' . $item['codigo'] . '">' . htmlentities( $item['descricao'] ) . '</option>';
			$itens_criados++;
			if ( $maximo_itens != 0 && $itens_criados >= $maximo_itens )
			{
				break;
			}
		}
	}
	else if ( $habilitado == 'S' )
	{
		$select .= '<option value="">Duplo clique para selecionar da lista</option>';
	}
	else
	{
		$select .= '<option value="">Nenhum</option>';
	}
	$select .= '</select>';
	$buscaCodigo = '';
	
	#Altera��o feita por wesley romualdo
	#caso a consulta n�o seja por descri��o e sim por codigo, n�o permitir digitar string no campo de consulta.
	if($campo_busca_descricao == true ){
		$paramentro = "";
		$complOnblur = "";
	} else {
		$paramentro = "onkeyup=\"this.value=mascaraglobal('[#]',this.value);\"";
		$complOnblur = "this.value=mascaraglobal('[#]',this.value);";		
	}
	
	if ( $campo_busca_codigo == true && $habilitado == 'S' )
	{
		$buscaCodigo .= '<input type="text" id="combopopup_campo_busca_' . $nome . '" onkeypress="combo_popup_keypress_buscar_codigo( event, \'' . $nome . '\', this.value );" '.$paramentro.' onmouseover="MouseOver( this );" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this); '.$complOnblur.'" class="normal" style="margin: 2px 0;" />';
		$buscaCodigo .= '&nbsp;<img title="adicionar" align="absmiddle" src="/imagens/check_p.gif" onclick="combo_popup_buscar_codigo( \'' . $nome . '\', document.getElementById( \'combopopup_campo_busca_' . $nome . '\' ).value );"/>';
		$buscaCodigo .= '&nbsp;<img title="remover" align="absmiddle" src="/imagens/exclui_p.gif" onclick="combo_popup_remover_item( \'' . $nome . '\', document.getElementById( \'combopopup_campo_busca_' . $nome . '\' ).value, true );"/>';
		$buscaCodigo .= '&nbsp;<img title="abrir lista" align="absmiddle" src="/imagens/pop_p.gif" onclick="combo_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ' );"/>';
		$buscaCodigo .= '<br/>';
	}
	#Fim da altera��o realizada por wesley romualdo
	
	$flagContem = '';
	if ( $campo_flag_contem == true )
	{
		$nomeFlagContemGlobal = $nome . '_campo_excludente';
		global ${$nomeFlagContemGlobal};
		$flagContem .= '<input type="checkbox" id="' . $nome . '_campo_excludente" name="' . $nome . '_campo_excludente" value="1" ' . ( ${$nomeFlagContemGlobal} ? 'checked="checked"' : '' ) . ' style="margin:0;" />';
		$flagContem .= '&nbsp;<label for="' . $nome . '_campo_excludente">N�o cont�m</label>';
	}
	$cabecalho = '';
	if ( $buscaCodigo != '' || $flagContem != '' )
	{
		$cabecalho .= '<table width="400" border="0" cellspacing="0" cellpadding="0"><tr>';
		$cabecalho .= '<td align="left">' . $buscaCodigo . '</td>';
		$cabecalho .= '<td align="right">' . $flagContem . '</td>';
		$cabecalho .= '</tr></table>';
	}
	print $cabecalho . $select;
}

/**
 * Monta o HTTML do combo_popup
 *
 * @param string $stDescricao Descri��o do campo
 * @param string $stNomeCampo Nome do campo que ser� criado HTML/PHP
 * @param string $sql_combo SQL para carregar as op��es
 * @param string $sql_carregados SQL para mostrar campos selecionados
 * @param string $stTextoSelecao Texto para aparecer no popup
 * @param string $funcaoJS Fun��o Javascript a ser chamada quando se escolhe uma op��o da popup(onclick). � passado por par�metro para esta fun��o js o objeto(this) (Adicionado por: Felipe Carvalho)
 * @param boolean $semTR Se true, n�o coloca TR, o que deve ser feito manualmente. Usado para casos em que se deve setar o innerHTML ou outro atributo DOM da TR. (Adicionado por: Felipe Carvalho)
 * @param string $intervalo se "S" ser� adicionado um campo chamado Selecionar intervalo onde, depois que dois registros forem clicados e acionado o bot�o de intervalo, todos os registros entre esse intervalo ser�o selecionados. ( Adicionado por: Victor Benzi )
 * @author Cristiano Teles
 * @since 09/02/2009
 */
function mostrarComboPopup( $stDescricao, $stNomeCampo, $sql_combo, $sql_carregados, $stTextoSelecao, Array $where=null, $funcaoJS=null, $semTR=false, $intervalo=false , $arrVisivel = null , $arrOrdem = null ){
	global $db, $$stNomeCampo;
	
	if ( $_REQUEST[$stNomeCampo] && $_REQUEST[$stNomeCampo][0] != '' && !empty( $sql_carregados ) ) {
		$sql_carregados = sprintf( $sql_carregados, "'" . implode( "','", $_REQUEST[$stNomeCampo] ) . "'" );
		$$stNomeCampo = $db->carregar( sprintf( $sql_combo, $sql_carregados ) );
	}
	if( !empty($sql_carregados) ){
		$$stNomeCampo = $db->carregar($sql_carregados);
	}
	
	if(!$semTR)
	{
		echo '<tr id="tr_'.$stNomeCampo.'">';
	}
	
	echo '<td width="195" class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( \'' . $stNomeCampo . '\' );">
			' . $stDescricao . '
			<input type="hidden" id="' . $stNomeCampo . '_campo_flag" name="' . $stNomeCampo . '_campo_flag" value="' . ( empty( $$stNomeCampo ) ? '0' : '1' ) . '"/>
		</td>
		<td>';
	
	echo '<div id="' . $stNomeCampo . '_campo_off" style="color:#a0a0a0;';
	echo !empty( $$stNomeCampo ) ? 'display:none;' : '';
	echo '" onclick="javascript:onOffCampo( \'' . $stNomeCampo . '\' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>';
	echo '<div id="' . $stNomeCampo . '_campo_on" '; 
	echo empty( $$stNomeCampo ) ? 'style="display:none;"' : '';
	echo '>';
	combo_popup( $stNomeCampo, sprintf( $sql_combo, '' ), $stTextoSelecao, '400x400', 0, array(), '', 'S', true, true, 10, 400, null, null, '', $where, null, true, false, $funcaoJS, $intervalo , $arrVisivel, $arrOrdem);
	echo '</div>
			</td>';
	
	if(!$semTR)	echo '</tr>';
}

/**
 * Monta um combo especial com sele��o em popups.
 *
 * Monta apenas o campo do formul�rio. Consulte as observa��es indicada
 * para cada par�metro. � necess�rio que o campo identificador seja
 * renomeado para 'codigo' e a descri��o para 'descricao'. Veja restante
 * dos coment�rio do arquivo www/geral/combopopup.php
 *
 * @param string  $nome nome atribuido ao campo no formul�rio
 * @param string  $sql consulta a ser realizada no banco, sendo os �lias (codigo,
 * 																		  descricao,
 * 																		  value)
 * 				  OBS.: O "value" � opcional, caso n�o seja passado o return ser� o "codigo".
 * 						o return fica setado em um campo "hidden", portanto o campo visto denomina-se por: "nome do campo"_dsc.									
 * 	
 * @param string  $titulo titulo que dever� aparecer no popup
 * @param string  $tamanho_janela tamanho da janela popup no formato HEIGHTxWIDTH
 * @param integer $width
 * @param array   $where ex: array( array("codigo" 	  => nomeCampoBanco,
 * 								 		  "descricao" => labelCampoFormulario,
 * 										  "tipo" 	  => 1 = inteiro ou 0 = string),
 * 						   		    array("codigo" 	  => nomeCampoBanco2,
 * 								 		  "descricao" => labelCampoFormulario2
 * 										  "tipo" 	  => 1 = inteiro ou  0 = string),
 * 						 )
 * 
 * @param array   $complemento
 * 				  Como ativar o autocomplete(exemplo: Solicita��o de Di�rias => evento/modulos/principal/solicitacaoDiarias.inc):
 * 				  - $complemento['class'] = 'campo_popup_autocomplete' e $complemento['whereAuto'] com o campo a ser filtrado
 * 				  Ter incluido o modulo do jQuery + Autocomplete
 * 				  - <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
 * 				  - <script type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>
 * 				  - <link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css" />
 * 				  Colocar os seguintes script jQuery:
 * 				  - jQuery('.campo_popup_autocomplete').autocomplete("/geral/campopopup.php?nome=origem&autocomplete=1", {
 *					  	cacheLength:50,
 *						width: 440,
 *						scrollHeight: 220,
 *						delay: 1000,
 *						selectFirst: true,
 *						autoFill: false
 *					});
 *				
 *					jQuery('.campo_popup_autocomplete').result(function(event, data, formatted) {
 *					     if (data) {
 *				
 *					     	// Extract the data values
 *					     	var campoHidden = jQuery(this).attr('id').replace('_dsc','');
 *							
 *							var descricao = data[0];
 *							var id = data[1];
 *				
 *							if( id == '' ){
 *						    	jQuery(this).val('');
 *						    	return false;
 *							}
 *				
 *					     	jQuery('#'+campoHidden).val( id );
 *						  	jQuery('#'+campoHidden+'_retorno').val( descricao );
 *						}
 *					});
 *				
 *					jQuery('.campo_popup_autocomplete').blur(function (){
 *						jQuery(this).val(jQuery(this).val());
 *					  	var campoHidden = jQuery(this).attr('id').replace('_dsc','');
 *					  	if( jQuery(this).val() != '' && jQuery('#'+campoHidden+'_retorno').val() != '' ){
 *							if( jQuery(this).val() != jQuery('#'+campoHidden+'_retorno').val() ){
 *								jQuery(this).val('Selecione...')
 *								jQuery('#'+campoHidden).val('')
 *								jQuery('#'+campoHidden+'_retorno').val('')
 *							}
 *						}else{
 *							jQuery(this).val('Selecione...')
 *						}
 *					});
 * 
 * @return void
 */
function campo_popup( $nome, $sql, $titulo, $func=null, $tamanho_janela = '400x400', $width = 30, $where=null, $typeCampo=1, $limpar = true,$disabled = false, $return = false, $obrigatorio = "N", $complemento = Array() )
{
	global $db,${$nome};

	unset($dados_sessao);
	
	// prepara parametros
	$maximo_itens  = abs( (integer) $maximo_itens );
	$codigos_fixos = $codigos_fixos ? $codigos_fixos : array();
	$where = $where ? $where : "";
	
	$ClassObrigatorio = $obrigatorio == "S" ? " obrigatorio" : "";
	
	// prepara sess�o
	$dados_sessao = array(
							'sql'    => (string) $sql, // o sql � armazenado para ser executado posteriormente pela janela popup
							'titulo' => $titulo,
							'indice' => $indice_visivel,
							'func'	 => $func,
//							'maximo' => $maximo_itens,
//							'codigos_fixos' => $codigos_fixos,
//							'mensagem_fixo' => $mensagem_fixo,
							'where'			=> $where,
							'whereAuto'     => $complemento['whereAuto']
						  );
	if ( !isset( $_SESSION['indice_sessao_campo_popup'] ) )
	{
		$_SESSION['indice_sessao_campo_popup'] = array();
	}
	$_SESSION['indice_sessao_campo_popup'][$nome] = $dados_sessao;
	// monta html para formulario
	$tamanho    = explode( 'x', $tamanho_janela );
	$onclick    = $disabled ? ' ' : 'onclick="javascript:campo_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ' );" ';
//	$ondblclick = ' ondblclick="javascript:campo_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ' );" ';
    $onchange   = ' onchange="javascript:'.$func.'(this.value)" ';
	$habilitado = 'S' ;
	$buscaCodigo = '';
	$desabilitado = $disabled ? ' disabled="disabled" ' : '';
	$class = $complemento['class'];
	$readonly   = $complemento['class'] == 'campo_popup_autocomplete' ? '' : 'readonly="readonly"';
	$ondblclick = ($complemento['class'] == 'campo_popup_autocomplete') && ($onclick != '')  ? 'onclick="if(this.value==\'Selecione...\'){this.value=\'\'}"' : $onclick;
	if ($typeCampo == 1){
		$campo =
				'<input ' .
				'name="' . $nome.'_dsc' . '" ' . 
				'id="' . $nome.'_dsc' . '" ' .
				'type = "text" ' .
				'value = "' . ( ${$nome}['descricao'] ? ${$nome}['descricao'] : 'Selecione...') . '" ' .
				$ondblclick.
				'class="CampoEstilo'.$ClassObrigatorio.' '.$class.'" '.
				'size="' . $width . '" ' .
				$readonly . 
				$desabilitado .
				'/>';
		if( $complemento['class'] == 'campo_popup_autocomplete' ){
			$campo .=
					'<input '.
					'name="' . $nome .'_retorno' . '" ' . 
					'id="' . $nome .'_retorno' . '" ' .
					'type = "hidden"' .
					'value = "' . ${$nome}['value'] . '" ' .
					'class="CampoEstilo" '.
					'size="' . $width . '" ' .
					$habilitado_select .
					'/>';
		}

		$campo .=
				'<input '.
				'name="' . $nome . '" ' . 
				'id="' . $nome . '" ' .
				'type = "hidden"' .
				'value = "' . ${$nome}['value'] . '" ' .
				'class="CampoEstilo" '.
				'size="' . $width . '" ' .
				$habilitado_select .
				'/>';
		if(!$disabled)
			$buscaCodigo .= '<img title="abrir lista" id="' . $nome . '_img" align="absmiddle" src="/imagens/seta_combo.gif" onclick="campo_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ' );"/>'.($obrigatorio == "S" ? '&nbsp;'.obrigatorio() : "");
		if  ($limpar && !$disabled){			
			$buscaCodigo .= '&nbsp;<img title="remover" align="absmiddle" src="/imagens/exclui_p.gif" onclick="campo_popup_remover_item( \'' . $nome . '\');"/>'.($obrigatorio == "S" ? '&nbsp;'.obrigatorio() : "");
		}
	}else{
		$campo ='<select ' .
				'maximo="'. $maximo_itens .'" tipo="combo_popup" ' .
				'size="1" ' .
				'name="' . $nome . '" id="' . $nome . '" '.
				$onchange .
				'class="CampoEstilo'.$ClassObrigatorio.'" style="width:' . $width . 'px;" ' .
				$habilitado_select .
				'>';
		if ($sql){
			$dados = $db->carregar($sql);
			$itens_criados = 0;
			
			$campo .= '<option value="" >Selecione...</option>';
			
			foreach ( $dados as $item ){
				$select = ${$nome} ? ${$nome} : ''; 
				$campo .= '<option value="' . ($item['value'] ? $item['value'] : $item['codigo']) . '">' . htmlentities( $item['descricao'] ) . '</option>';
				$itens_criados++;
			}
		}else{
			$campo .= '<option value="">Nenhum</option>';
		}
		$campo .= '</select>';
		$buscaCodigo .= '&nbsp;<img title="abrir lista" align="absmiddle" src="/imagens/pop_p.gif" onclick="campo_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ' );"/> '.($obrigatorio == "S" ? '&nbsp;'.obrigatorio() : "");
		
	}

//	$cabecalho .= '<table width="100%" border="0" cellspacing="0" cellpadding="0"><tr>';
//	$cabecalho .= '<td align="left">' . $campo . $buscaCodigo . '</td>';
//	$cabecalho .= '<td align="right">' . $flagContem . '</td>';
//	$cabecalho .= '</tr></table>';
	$cabecalho = "<span style=\"white-space: no-wrap\">".$campo.$buscaCodigo.$flagContem."</span>";

	if($return){
		return $cabecalho;
	}else{
		print $cabecalho;
	}
}

/**
 * Mona um campo texto que pode ser preenchido com valores listados em um popup.
 *
 * Monta apenas o campo do formul�rio. Consulte as observa��es indicada
 * para cada par�metro. � necess�rio que o campo identificador seja
 * renomeado para 'codigo' e a dhttp://www.maujor.com/tutorial/haslayout.phpescri��o para 'descricao'. Veja restante
 * dos coment�rio do arquivo www/geral/textopopup.php
 *
 * @param string $nome nome atribuido ao campo no formul�rio
 * @param string $sql consulta a ser realizada no banco
 * @param string $titulo titulo que dever� aparecer no popup
 * @param string $maxlength quantidade m�xima de caracteres
 * @param string $tamanho_campo tamanho do campo texto
 * @param string $mascara m�scara do campo texto
 * @param string $tamanho_janela tamanho da janela popup no formato HEIGHTxWIDTH
 * @param string $complemento complemento para o input
 * @return void
 */
function texto_popup( $nome, $sql, $titulo, $maxlength = '', $tamanho_campo = 20, $mascara = '', $tamanho_janela = '400x400', $complemento = '' , $onblur = '')
{
	global ${$nome};
	// prepara sess�o
	$dados_sessao = array(
	'sql' => (string) $sql, // o sql � armazenado para ser executado posteriormente pela janela popup
	'titulo' => $titulo,
	);
	$_SESSION['indice_sessao_texto_popup'][$nome] = $dados_sessao;
	if ( !$tamanho_janela )
	{
		$tamanho_janela = '400x400';
	}
	$tamanho = explode( 'x', $tamanho_janela );
	$onclick = ' onclick="javascript:texto_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ', \'&mostraCodigo=1&label=0\' );" ';
	$mascara = $mascara == '' ? '' : ' onKeyUp= "if(event.keyCode!=9&&event.keyCode!=13)this.value=mascaraglobal( \'' . $mascara . '\',this.value);" ' ;
	//$mascara = '';
	$maxlength = $maxlength == '' ? '' : ' maxlength=' . $maxlength . ' ';
	print '<input ' . $complemento . ' ' . $mascara . $maxlength . ' onmouseover="MouseOver(this);" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);' . $onblur . '" size="' . $tamanho_campo . '" type="text" name="' . $nome . '" id="' . $nome . '" class="normal" value="' . ${$nome} . '">';
	print '<img id="img_texto_popup_' . $nome . '" src="/imagens/seta_combo.gif" width="14" height="16" style="margin:0;padding:0;" align="absmiddle" ' . $onclick . ' />';
}
function hidden_popup( $nome, $sql, $titulo, $tamanho_janela = '400x400',$explica='' )
{
	global ${$nome};
	// prepara sess�o
	$dados_sessao = array(
	'sql' => (string) $sql, // o sql � armazenado para ser executado posteriormente pela janela popup
	'titulo' => $titulo,
	);
	$_SESSION['indice_sessao_texto_popup'][$nome] = $dados_sessao;
	$tamanho = explode( 'x', $tamanho_janela );
	$onclick = ' onclick="javascript:texto_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ', \'&mostraCodigo=0&label=1\' );" ';
	print '<input type="hidden" name="' . $nome . '" id="' . $nome . '" class="normal" value="' . ${$nome} . '">';
	print '<span id="label_popup_'.$nome.'"></span><img src="/imagens/preview.gif"  style="margin:0 0 0 5px;padding:0;cursor:pointer;" align="absmiddle" ' . $onclick . ' />'.$explica;
}

/**
 * Cria os bot�es de listagem e inclus�o de arquivos.
 *
 * @param string $nome Nome para controle dos dados da sessao
 * @param string $sqlListagem Se o valor for diferente de '', insere o bot�o para listar os arquivos de acordo com a query passada
 * @param string $campo_id_vinculo Nome do campo que ser� usado para a tabela de assicia��o documento_processo (pjeid, ptoid, acaid, etc.)
 * @param integer $inserir Vari�vel para verifica��o de inclus�o ou n�o do bot�o de inser��o de novos arquivos
 * @param integar $width largura da janela a ser aberta
 * @param integer $height altura da janela a ser aberta
 */
function popup_arquivo( $nome, $sqlListagem = '', $campo_id_vinculo, $valor_id_vinculo, $inserir = 1, $width, $height,$tabela_vinculo='' )
{

	$dados_sessao = array(
	'nome' => $nome,
	'campo_id_vinculo' => $campo_id_vinculo,
	'valor_id_vinculo' => $valor_id_vinculo,
	'tabela_vinculo' => $tabela_vinculo
	);
	if( $sqlListagem )
	{
		print '<input type="button" value="Arquivos associados" onclick="popup_arquivo( \'L\', \'' . $nome .'\', '. $width .', ' . $height . ' )" />&nbsp;&nbsp;';
		$dados_sessao[ 'sql' ] = $sqlListagem;
	}

	if( $inserir )
	{
		print '<input type="button" value="Inserir arquivo" onclick="popup_arquivo( \'I\', \'' . $nome .'\', '. $width .', ' . $height . ' )" />';
	}

	$_SESSION[ 'indice_sessao_popup_arquivo' ][ $nome ] = $dados_sessao;
}

/**
 * Formata um numero real com a mascara ###.###.###,##
 * O segundo par�metro indica a quantidade de casas decimais
 *
 * @param float $valor
 * @param integer $casas_decimais
 * @return string
 */
function formata_valor( $valor, $casas_decimais = 2, $adicionar_zeros_direita = true )
{
	$valor = sprintf( '%.' . $casas_decimais . 'f', (float) $valor );
	$positivo = $valor > 0;
	$valores = explode( '.', $valor );
	$parte_inteira = @$valores[0];
	$parte_decimal = @$valores[1];
	while ( $parte_inteira{0} == '0' )
	{
		$parte_inteira = substr( $parte_inteira, 1 );
	}
	if ( strlen( $parte_inteira ) == 0 )
	{
		$parte_inteira = '0';
	}
	$sobra = '';
	if ( strlen( $parte_inteira ) % 3 == 2 )
	{
		$sobra .= $parte_inteira{0};
		$parte_inteira = substr( $parte_inteira, 1 );
	}
	if ( strlen( $parte_inteira ) % 3 == 1 )
	{
		$sobra .= $parte_inteira{0};
		$parte_inteira = substr( $parte_inteira, 1 );
	}
	if ( strlen( $sobra ) && $positivo && strlen( $parte_inteira ) )
	{
		$sobra .= '.';
	}
	$parte_inteira = implode( '.', str_split( $parte_inteira, 3 ) );
	if ( $adicionar_zeros_direita == false )
	{
		while ( $parte_decimal[strlen($parte_decimal)-1] == '0' )
		{
			$parte_decimal = substr( $parte_decimal, 0, -1 );
		}
	}
	if ( strlen( $parte_decimal ) > 0 )
	{
		$parte_inteira .= ',';
	}
	$retorna = $sobra . $parte_inteira . $parte_decimal;
	return $retorna;
}

/**
 * Formata um string com a mascara ###.###.###,## para um
 * ponto flutuante.
 *
 * @param float $valor
 * @param integer $casas_decimais
 * @return string
 */
function desformata_valor( $valor )
{
	$valor = str_replace( '.', '', $valor );
	$valor = str_replace( ',', '.', $valor );
	return (float) $valor;
}

function eof()
{
	?>
<html>
<head>
<title>Registro n�o encontrado</title>
</head>
<body>
<script language="Javascript">
				alert('Erro: Registro n�o encontrado!');
				history.back();
				</script>
</body>
</html>
	<?
	exit();
}
function mostra_num($num,$dec=0,$simb='')
{
	if ($num==0) return '-' ; else
	return $simb.' '.number_format($num,$dec,',','.');
}

/**
 * Monta campo radio com as op��es determinadas pelo segundo par�metros.
 * O formato do segundo par�metro deve ser
 * $opcoes = array(
 * 		"label1" => array(
 * 			"valor" => "valor1",
 * 			"id" => "id1"
 * 			"callbak" => "nome_da_funcao"
 * );
 * Caso queira que algum valor seja por default, selecionado
 *
 * @param string $nome
 * @param array $opcoes
 * @param string $alinhamento vertical ou horizontal ( 'v'|'h' )
 * @param boolean $return
 */
function campo_radio( $nome, $opcoes, $alinhamento, $return = false )
{
	global ${$nome};
	$valor_selecionado = ${$nome};
	if ( !is_array( $opcoes ) || !count( $opcoes ) )
	{
		return;
	}

    $container  = '';
	$container .= '<ul style="margin: 0; padding: 0;">';
	$alinhamento = strtolower( $alinhamento ) == 'h' ? ' float:left; ' : '' ;
	$nome = htmlentities( $nome );
	foreach ( $opcoes as $label => $dados )
	{
		$valor = $dados['valor'];
		$id = $dados['id'] ? $dados['id'] : 'radio_opcao_' . $nome . '_' . $valor ;
		$checked = $valor_selecionado == $valor  ? ' checked="checked" ' : $dados[ 'default' ] ? ' checked="checked" ' : '' ;
		$valor = htmlentities( $valor );
		$onfocus = '';
		if ( $dados['callback'] )
		{
			$onfocus = ' onfocus="this.checked = true; ' . $dados['callback'] . '( this );" ';
		}
		$container .= '<li style="margin:0;width:80px; list-style-type: none;' . $alinhamento . '">';
		$container .= '<input type="radio" name="' . $nome . '" value="' . $valor . '" id="' . $id . '" ' . $checked . ' ' . $onfocus . '/>';
		$container .= '<label for="' . $id . '">' . $label . '</label>';
		$container .= '</li>';
	}
	$container .= '</ul>';

    if ($return)
       return $container;

    echo $container;
}

/**
 * Monta campo radio com op��es booleana. Os valores dos campos ser�o '0' ou '1'.
 *
 * @param string $nome
 * @param string $alinhamento vertical ou horizontal ( 'v'|'h' )
 */
function campo_radio_sim_nao( $nome, $alinhamento = 'v' )
{
	global ${$nome};
	$valor_antigo = ${$nome};
	${$nome} = ${$nome} ? '1' : '0' ; // altera de booleano para string '0' ou '1'
	$opcoes = array(
	'Sim' =>
	array(
	'valor' => '1',
	'id' => null
	),
	'N�o' =>
	array(
	'valor' => '0',
	'id' => null
	)
	);
	campo_radio( $nome, $opcoes, $alinhamento );
	${$nome} = $valor_antigo; // volta ao valor original
}
function tradutor_att($valor)
{
	// esta fun��o traduz um nome de um atributo para o seu nome fantasia
	if (trim($valor)=='acafinalidade') return 'Finalidade';
	if (trim($valor)=='funcod') return 'Fun��o';
	if (trim($valor)=='unicod') return 'Unidade';
	if (trim($valor)=='sfucod') return 'Subfun��o';
	if (trim($valor)=='acadescricao') return 'Descri��o';
	if (trim($valor)=='procod') return 'Produto';
	if (trim($valor)=='acadscproduto') return 'Descri��o do Produto';
	if (trim($valor)=='unmcod') return 'Unidade de Medida';
	if (trim($valor)=='taccod') return 'Tipo de A��o';
	if (trim($valor)=='esfcod') return 'Esfera';
	if (trim($valor)=='acadetalhamento') return 'Detalhamento';
	if (trim($valor)=='acabaselegal') return 'Base legal';
	// programas
	if (trim($valor)=='tprcod') return 'Tipo do Programa';
	if (trim($valor)=='mobcod') return 'Macro objetivo';
	if (trim($valor)=='prgdscobjetivo') return 'Descri��o do objetivo';
	if (trim($valor)=='prgdscpublicoalvo') return 'P�blico alvo';
	if (trim($valor)=='prgdscjustificativa') return 'Justificativa';
	if (trim($valor)=='prgdscestrategia') return 'Estrat�gia de implementa��o';
	if (trim($valor)=='prgdscproblema') return 'Problema';
	// indicador
	if (trim($valor)=='percod') return 'Periodicidade';
	if (trim($valor)=='bsgcod') return 'Base geogr�fica';
	if (trim($valor)=='inddscfonte') return 'Fonte';
	if (trim($valor)=='inddscformula') return 'F�rmula';
	if (trim($valor)=='indvlrapurado') return 'Valor apurado';
	if (trim($valor)=='inddataapuracao') return 'Data de apura��o';
	if (trim($valor)=='indvlrapurado') return 'Valor apurado';

	if (trim($valor)=='indvlrfinalppa') return 'Valor ao final do PPA';
	if (trim($valor)=='indvlrfinalprg') return 'Valor final do programa';
	if (trim($valor)=='indvlrreferencia') return 'Valor de refer�ncia';
	if (trim($valor)=='indvlrapurado') return 'Valor apurado';

}
function busca_filho($id)
{
	global $db;
	$sql = "select ptoid from monitora.planotrabalho where ptostatus='A'  and ptoid_pai=$id limit 1";
	//dbg($sql,1);
	$filho=$db->pegaUm($sql);
	if ($filho) return true;
	else return false;
}

function monta_link( $modulo, $acao, $parametros = '' )
{
	return '/' . $_SESSION['sisdiretorio'] . '/' . $_SESSION['sisdiretorio'] . '.php?modulo=' . $modulo . '&acao=' . urlencode( $acao ) . '&' . $parametros;
}

function mostracod($id)
{
	global $db;
	// monta um c�digo com pontos
	$sql = "select ptoid_pai, ptoordemprov from monitora.planotrabalho where ptoid=$id";
	$cod= $db->pegalinha($sql);
	if ($cod['ptoid_pai'])
	{
		// tem pai
		return mostracod($cod['ptoid_pai']).'.'.$cod['ptoordemprov'];
	}
	else
	return $cod['ptoordemprov'];

}

function nomeUnidade($cod){
	global  $db;
	$sql = "select unidsc from public.unidade where unicod = '$cod' and unitpocod = 'U'";	
	return $db->pegaUm($sql);

}

function verficaPerfil($cpf,$sisid){
	global $db;
	$sql = "select  trim(p.pflcod) as cod from seguranca.perfilusuario pu inner join seguranca.perfil p on pu.pflcod = p.pflcod where pu.usucpf = '$cpf' and p.pflstatus = 'A' and p.sisid = $sisid;";
	return $db->carregarColuna($sql);
}

function verficaSuperUsuario($sisid){
	global $db;
	$cpf = $_SESSION['usucpf'];
	$sql = "select count(*) from seguranca.perfilusuario pu inner join seguranca.perfil p on pu.pflcod = p.pflcod where pu.usucpf = '$cpf' and p.pflstatus = 'A' and p.sisid = $sisid and p.pflnivel = 1;";

	if($db->pegaUm($sql) > 0) return true;
	else
	return false;
}


function permissaoUsuario($cod){


	$perfil =  verficaPerfil($_SESSION['usucpf'],$_SESSION['sisid']);

	if(in_array(SESU_ADMINISTRADOR,$perfil)) return true;

	return in_array($cod,$perfil);
}

function permissaoReuni($arrayPerfil,$unicod){
	global $db;

	$sisid = $_SESSION['sisid'];

	if(verficaSuperUsuario($sisid)) return true;

	$fase = statusFase($unicod);


	switch ($fase){
		case 'N' :
			$permissao = array(IFES_APROVACAO,IFES_CADASTRO,IFES_CONSULTA);
			break;
		case 'E' :
			$permissao = array(IFES_APROVACAO,IFES_CADASTRO,IFES_CONSULTA);
			break;
		case 'G' :
			$permissao = array(SESU_APROVACAO,SESU_CONSULTAGERAL,SESU_PARECER);
			break;
		default:
			$permissao = array();
			break;

	}

	$arrayPerfil = array_intersect($permissao,$arrayPerfil);

	foreach ($arrayPerfil as $perfil){

		if(permissaoUsuario($perfil) === true){
			return true;
			break;
		}
	}

	return false;

}
function statusFase($unicod){
	global $db;
	$sql = "select coalesce(u.stscod,'N') as status  from reuni.unidadepropostastatus u inner join reuni.statusproposta s on s.stscod = u.stscod where unicod = '$unicod' order by u.upscod desc" ;
	if(($dado = $db->pegaUm($sql))!=''){
		return $dado;
	}else return 'N';
}
function mudaStatus($projeto,$codUnidade,$tipo,$status){
	global $db;
	$sql = "insert into reuni.unidadepropostastatus (prjcod,unicod,unitpocod,stscod) values ($projeto,'$codUnidade','$tipo','$status')";
	$db->executar($sql);
	$db->commit();
}

function montaLinkManual( $intSisId )
{
	global $db;
	require_once APPRAIZ . "includes/arquivo.inc";
	$strSql = "	 select
					 arqid
				 from
					 public.manual as manual
				 WHERE
					 manual.sisid = {$intSisId}
				";
	$arrDadosManual = $db->pegaLinha( $strSql );
	$booArquivoExiste = existeArquivoFisico( $arrDadosManual["arqid"] );
	if( $booArquivoExiste && sizeof( $arrDadosManual ) > 0 && $arrDadosManual["arqid"] > 0 )
	{
		$intArqId = $arrDadosManual["arqid"];
		
	$linkmanual = '<a href="../mostra_arquivo.php?id='.$intArqId.'"
	style="color: #ffffff; margin-right: 10px; margin-bottom: 5px;">Manual</a>';
		
}
else
{
	$linkmanual = '<script type="text/javascript">
				function alertMe( objTag )
				{
					alert( objTag.title );
					return false;
				}
			</script>
<a href="#"
	title="Manual n�o cadastrado para este m�dulo, encontra-se em elabora��o."
	onclick="return alertMe( this )"
	style="color: #ffffff; margin-right: 10px; margin-bottom: 5px;">Manual</a>';
	
}
	return $linkmanual;
}

function montaLinkManual2( $intSisId )
{
	global $db;
	require_once APPRAIZ . "includes/arquivo.inc";
	$strSql = "	 select
					 arqid
				 from
					 public.manual as manual
				 WHERE
					 manual.sisid = {$intSisId}
				";
	$arrDadosManual = $db->pegaLinha( $strSql );
	$booArquivoExiste = existeArquivoFisico( $arrDadosManual["arqid"] );
	if( $booArquivoExiste && sizeof( $arrDadosManual ) > 0 && $arrDadosManual["arqid"] > 0 )
	{
		$intArqId = $arrDadosManual["arqid"];
		
	$linkmanual = '<a href="../mostra_arquivo.php?id='.$intArqId.'&tela_login=1"
	style="color: #555555; margin-right: 10px; margin-bottom: 5px;">Manual do Sistema</a>';
		
}
else
{
	$linkmanual = false;
	
}
	return $linkmanual;
}

/**
 * Monta abas no padrao do sistema
 *
 * @param array Um array no seguinte formato:
 *  Array
 *  (
 *      [0] => Array
 *      (
 *          ['descricao'] => TITULO DA ABA 1
 *          ['link']      => LINK DA ABA 1
 *      )
 *
 *      [1] => Array (
 *          ['descricao'] => TITULO DA ABA 1
 *          ['link']      => LINK DA ABA 1
 *          )
 *      )
 *  )
 * 
 * @param $boOpenWin Boleano. Quando true esse parametro indica que as abas devem ser abertas em uma nova janela. (09-11-2010)
 */
function montarAbasArray($itensMenu, $url = false, $boOpenWin = false)
{
    $url = $url ? $url : $_SERVER['REQUEST_URI'];

    if (is_array($itensMenu)) {
        $rs = $itensMenu;
    } else {
        global $db;
        $rs = $db->carregar($itensMenu);
    }

    $menu    = '<table width="95%" border="0" cellspacing="0" cellpadding="0" align="center" class="notprint">'
             . '<tr>'
             . '<td>'
             . '<table cellpadding="0" cellspacing="0" align="left">'
             . '<tr>';

    $nlinhas = count($rs) - 1;

    for ($j = 0; $j <= $nlinhas; $j++) {
        extract($rs[$j]);

        if ($url != $link && $j == 0)
            $gifaba = 'aba_nosel_ini.gif';
        elseif ($url == $link && $j == 0)
            $gifaba = 'aba_esq_sel_ini.gif';
        elseif ($gifaba == 'aba_esq_sel_ini.gif' || $gifaba == 'aba_esq_sel.gif')
            $gifaba = 'aba_dir_sel.gif';
        elseif ($url != $link)
            $gifaba = 'aba_nosel.gif';
        elseif ($url == $link)
            $gifaba = 'aba_esq_sel.gif';

        if ($url == $link) {
            $giffundo_aba = 'aba_fundo_sel.gif';
            $cor_fonteaba = '#000055';
        } else {
            $giffundo_aba = 'aba_fundo_nosel.gif';
            $cor_fonteaba = '#4488cc';
        }

        $menu .= '<td height="20" valign="top"><img src="../imagens/'.$gifaba.'" width="11" height="20" alt="" border="0"></td>'
               . '<td height="20" align="center" valign="middle" background="../imagens/'.$giffundo_aba.'" style="color:'.$cor_fonteaba.'; padding-left: 10px; padding-right: 10px;cursor:pointer;" onclick="'. ($boOpenWin ? 'janela(\''.$link.'\',780,600,\'newtab\')' : 'window.location=\''.$link.'\'' ) . '">';

        if ($link != $url) {
            $menu .= $descricao;
        } else {
            $menu .= $descricao . '</td>';
        }
    }

    if ($gifaba == 'aba_esq_sel_ini.gif' || $gifaba == 'aba_esq_sel.gif')
        $gifaba = 'aba_dir_sel_fim.gif';
    else
        $gifaba = 'aba_nosel_fim.gif';

    $menu .= '<td height="20" valign="top"><img src="../imagens/'.$gifaba.'" width="11" height="20" alt="" border="0"></td></tr></table></td></tr></table>';

    return $menu;
}


function alert($str, $close = false, $escape = true)
{
    if ($escape) {
        $str = str_replace(array("'", "\"", "\\"),
                           array("\\'", "\\\"", "\\\\"), $str);
    }

    $close = $close ? 'self.close();' : '';

    printf('<script type="text/javascript">alert("%s");%s</script>', $str, $close);
}

function removeAcentos($str){
	 $arAcentos = array(
				"/[�����]/"=>"A",
				"/[�����]/"=>"a",
				"/[����]/"=>"E",
				"/[����]/"=>"e",
				"/[����]/"=>"I",
				"/[����]/"=>"i",
				"/[�����]/"=>"O",
				"/[�����]/"=>"o",
				"/[����]/"=>"U",
				"/[����]/"=>"u",
				"/�/"=>"c",
				"/�/"=> "C");
	// Tira o acento pela chave do array
	return preg_replace(array_keys($arAcentos), array_values($arAcentos), $str);

}


/**
 * Fun��o que carrega os org�os de acordo com o tipo
 * e com o munic�pio informado.
 *
 * @author Fernando A. Bagno da Silva
 * @param string $editavel -> se os campos estao habilitados ou n�o
 * @param string $usucpf -> cpf do usu�rio cadastrado, caso exista
 * /
 */
function carrega_orgao($editavel, $usucpf){
	

	global $muncod, $regcod, $tpocod, $entid, $db;
	
	if ($editavel == null){
		$editavel = 'S';
	}
	
	if ($editavel == 'N'){

		// Se o usu�rio j� est� cadastrado no sistema, busca o org�o
		$sql = "SELECT
					u.entid as codigo,
					CASE WHEN ee.entorgcod is not null THEN ee.entorgcod ||' - '|| ee.entnome 
					ELSE ee.entnome END AS descricao 
				FROM
					seguranca.usuario u
				INNER JOIN
					entidade.entidade ee ON
					ee.entid = u.entid
				WHERE
					u.usucpf = '{$usucpf}' AND 
					ee.entorgcod <> '73000'";

		$db->monta_combo("entid",$sql,$editavel,"&nbsp;",'ajax_carrega_unidade','','','350','S', 'entid');

	}else {
		// Se for Estadual ou Outros, o usu�rio dever� informar o org�o
		if ( /*$tpocod == 2 ||*/ $tpocod == 4){
			echo campo_texto( 'orgao', '', $editavel, '', 50, 50, '', '', 'left', '', 0, 'id="orgao"' );
				
		}else{

			// Caso o tipo seja municipal, verifica o municipio escolhido
			$inner = ( $tpocod == 3 || $tpocod == 2 ) ? 'INNER JOIN
																entidade.endereco eed ON
																eed.entid = ee.entid 
															 ': '';
			
			$uniao = ( $tpocod == 3 || $tpocod == 2 ) ? " UNION ALL (		
																  SELECT 
																  	999999 AS codigo,
																  	'OUTROS' AS descricao
																)" : '';

			if ( $tpocod == 2 ){
				$regcod = ($_REQUEST['regcod']) ? $_REQUEST['regcod'] : $regcod;
				$clausula = "AND eed.estuf = '" . $regcod . "'";
			}elseif ( $tpocod == 3 ){
				$clausula = "AND eed.muncod = '" . $muncod . "'";
			}
//				$clausula = $tpocod == 3 ? "AND eed.muncod = '" . $muncod . "'" : '';
			
			$sql = "(SELECT
						ee.entid AS codigo, 
						CASE WHEN ee.entorgcod is not null THEN ee.entorgcod ||' - '|| ee.entnome 
						ELSE ee.entnome END AS descricao 
					FROM 
						entidade.entidade ee
					INNER JOIN entidade.funcaoentidade ef ON ef.entid = ee.entid
					INNER JOIN public.tipoorgaofuncao tpf ON ef.funid = tpf.funid
						" . $inner . "
					WHERE
						tpf.tpocod = '{$tpocod}'
						" . $clausula . " AND
						( ee.entorgcod is null or ee.entorgcod <> '73000' )
						
					ORDER BY
						ee.entnome)" . $uniao;
			
			$db->monta_combo("entid",$sql,$editavel,"&nbsp;",'ajax_carrega_unidade','','','350','S', 'entid');
				
		}

	}

}

/**
 * Fun��o que carrega as unidades daquele �rg�o (caso
 * existam)
 *
 * @author Fernando A. Bagno da Silva
 * @param integer $entid -> ID do org�o selecionado
 * @param string $editavel -> se os campos estao habilitados ou n�o
 * @param string $usucpf -> cpf do usu�rio cadastrado, caso exista
 */
function carrega_unidade($entid, $editavel, $usucpf){
	
	global $db;
	
	if ($editavel == null){
		$editavel = 'S';
	}
	
	if ( $entid == 999999 ){
		
		echo campo_texto( 'orgao', '', $editavel, '', 50, 50, '', '', 'left', '', 0, 'id="orgao"' );
		
	} else {

		if ( $editavel == 'N' ){

			$sql = "SELECT
						u.unicod AS codigo, 
						u.unicod||' - '||u.unidsc AS descricao 
					FROM 
						unidade u
					INNER JOIN
						seguranca.usuario su ON 
						u.unicod = su.unicod
					WHERE
						usucpf = '{$usucpf}'";
	
			$db->monta_combo("unicod",$sql,$editavel,"",'selecionar_unidade_orcamentaria', '', '','','S','unicod');
	
		}else {
	
			if ( $entid ){
				
				$uo_total = $db->pegaUm( "SELECT
											count(u.unicod) 
										  FROM 
										  	unidade u
										  INNER JOIN
										  	entidade.entidade e ON
										  	u.orgcod = e.entorgcod
										  WHERE 
										  	unistatus='A' AND 
										  	unitpocod='U' AND 
										  	e.entid ='{$entid}'" );
					
			}
				
			if ( $uo_total > 0 ){
					
				$sql = "SELECT DISTINCT 
							u.unicod AS codigo, 
							u.unicod||' - '||unidsc AS descricao 
						FROM 
							unidade u
						INNER JOIN
							entidade.entidade e ON
							orgcod = entorgcod 
						WHERE 
							unistatus='A' AND 
							u.unitpocod='U' AND 
							entid = '{$entid}' 
						ORDER BY 
							u.unicod";
		
				$db->monta_combo("unicod",$sql,$editavel,"&nbsp;",'ajax_unidade_gestora', '', '','','S','unicod');
					
			} else {
				echo '<font style="color:#909090;">Este �rg�o n�o possui uma unidade.</font>';
			}
		}
	}
}

/**
 * Fun��o que carrega a unidade gestora de uma unidade or�ament�ria, 
 * quando existir
 *
 * @author Fernando A. Bagno da Silva
 * @param integer $unicod -> ID da unidade selecionada
 * @param string $editavel -> se os campos estao habilitados ou n�o
 * @param string $usucpf -> cpf do usu�rio cadastrado, caso exista
 */
function carrega_unidade_gestora($unicod, $editavel, $usucpf){
	
	global $db;
	
	if ($editavel == null){
		$editavel = 'S';
	}
	
	if ( $editavel == 'N' ){
		
		$sql = "SELECT 
					ug.ungcod AS codigo, 
					ug.ungcod||' - '||ug.ungdsc as descricao 
				FROM 
					unidadegestora ug
				INNER JOIN
					seguranca.usuario su ON
					ug.ungcod = su.ungcod
				WHERE 
					ungstatus = 'A' AND 
					unitpocod = 'U' AND 
					su.unicod = '".$unicod."' AND
					usucpf = '".$usucpf."'
				ORDER BY 
					ungdsc";
	
		$db->monta_combo("ungcod",$sql,$editavel,"",'','', '', '', 'S', 'ungcod');
		
	}else {
		
		if ( $unicod == '26101' || $unicod == '26000' ){

			$sql = "SELECT 
						ungcod AS codigo, 
						ungcod||' - '||ungdsc as descricao 
					FROM 
						unidadegestora 
					WHERE 
						ungstatus = 'A' AND 
						unitpocod = 'U' AND 
						unicod = '".$unicod."' 
					ORDER BY 
						ungdsc";
			
			$db->monta_combo("ungcod",$sql,$editavel,"&nbsp;",'','','', '', 'S', 'ungcod');
		
		}else {

			
			echo '<font style="color: #909090;">Esta unidade n�o possui uma Unidade Gestora.</font>';
			
		}
	}
}

/**
 * Fun��o que monta o cabecalho padr�o com o bras�o brasileiro
 * utilizado nos relat�rios do sistema
 *
 * @param string $largura
 * @author Fernando A. Bagno da Silva
 * @since 20/02/2009
 */
function monta_cabecalho_relatorio( $largura ){
	
	global $db;
	
	$cabecalho = '<table width="'.$largura.'%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug"  style="border-bottom: 1px solid;">'
				.'	<tr bgcolor="#ffffff">' 	
				.'		<td valign="top" width="50" rowspan="2"><img src="../imagens/brasao.gif" width="45" height="45" border="0"></td>'			
				.'		<td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">'				
				.'			'.$GLOBALS['parametros_sistema_tela']['sigla-nome_completo'].'<br/>'				
//				.'			Acompanhamento da Execu��o Or�ament�ria<br/>'					
				.'			'.$GLOBALS['parametros_sistema_tela']['orgao'].' <br />'
				.'		</td>'
				.'		<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">'					
				.'			Impresso por: <b>' . $_SESSION['usunome'] . '</b><br/>'					
				.'			Hora da Impress�o:' . date( 'd/m/Y - H:i:s' ) . '<br />'					
				.'		</td>'					
				.'	</tr><tr>'
				.'		<td colspan="2" align="center" valign="top" style="padding:0 0 5px 0;">'
				.'			<b><font style="font-size:14px;">' . $_REQUEST["titulo"] . '</font></b>'
				.'		</td>'
				.'	</tr>'					
				.'</table>';					
								
		return $cabecalho;						
						
}


/**
 * Fun��o que retorna o pr�ximo valor � partir do par�metro recebido.
 * O padr�o para os valores � o seguinte: '0' � '9', e depois de 'A' � 'Z', dependendo 
 * do n�mero de caracteres do par�metro.
 * 
 * Exemplos:
 * 			valor passado por par�metro: '99'   --->  valor retornado: 'AA'
 * 			valor passado por par�metro: 'ABA'  --->  valor retornado: 'ABC'
 * 			valor passado por par�metro: '677'  --->  valor retornado: '678'
 * 			valor passado por par�metro: 'AMZZ' --->  valor retornado: 'ANAA'
 *
 * @param string $num
 * @author Cristiano Cabral
 * @since 07/04/2009
 */
function retornaseq($num) {
	$qtddig = strlen($num);

    if(is_numeric($num)) {
    	$num = (integer) $num;
        $num = $num+1;
        
        if(strlen($num)>$qtddig) {
        	$num = str_pad('',$qtddig,'A');
            return $num;
        } else {
           	return str_pad($num,$qtddig,'0',0);
        }
	}
    else {
		//valida se � uma sequencia de caracteres permitida
        foreach (range(0,$qtddig-1) as $i){ validachar(substr($num,$i,1)); }
        
        $numnovo = '';
        //$num = strrev($numnovo);
        
        foreach (range(1,$qtddig) as $i) {
        	$ascii = ord(substr($num,$qtddig-$i,1));
        	
            if($ascii==90 && !$para) {
            	$numnovo .= 'A';
                $para=false;
           	} 
           	else {
            	if(!$para) {
                	$numnovo .= chr($ascii+1);
                    $para=true;
                } else {
                	$numnovo .= chr($ascii);
                }
            }

            try{if (strrev($numnovo)==str_pad('',$qtddig,'A')) throw new Exception('Fim da Sequencia.');} catch (Exception $e){die('Caught exception: '.  $e->getMessage() . "\n");}                 
		}
        
		return strrev($numnovo);
	}
}

/**
 * Fun��o que valida se o caracter � permitido.
 *
 * @param char $char
 * @author Cristiano Cabral
 * @since 07/04/2009
 */
function validachar($char) {
      if(in_array(ord($char),range(65,90))) {return true;} else {try{if (strrev($numnovo)==str_pad('',$qtddig,'A')) throw new Exception('Formato Inv�lido.');} catch (Exception $e){die('Caught exception: '.  $e->getMessage() . "\n");}}
}

/**
 * Fun��o que coloca em extenso valores monet�rios.
 * 
 * @param float $valor
 * @param boolean $maiusculas
 * @author Felipe de Oliveira Carvalho
 * @since 22/12/2009
 */
function valorMonetarioExtenso($valor=0, $maiusculas=false) { 
	
    $singular 	= array("centavo", "real", "mil", "milh�o", "bilh�o", "trilh�o", "quatrilh�o"); 
    $plural 	= array("centavos", "reais", "mil", "milh�es", "bilh�es", "trilh�es", "quatrilh�es"); 

    $c = array("", "cem", "duzentos", "trezentos", "quatrocentos", "quinhentos", "seiscentos", "setecentos", "oitocentos", "novecentos"); 
    $d = array("", "dez", "vinte", "trinta", "quarenta", "cinquenta", "sessenta", "setenta", "oitenta", "noventa"); 
    $d10 = array("dez", "onze", "doze", "treze", "quatorze", "quinze", "dezesseis", "dezesete", "dezoito", "dezenove"); 
    $u = array("", "um", "dois", "tr�s", "quatro", "cinco", "seis", "sete", "oito", "nove"); 

    $z=0; 

    $valor = number_format($valor, 2, ".", "."); 
    $inteiro = explode(".", $valor);
     
    for($i=0;$i<count($inteiro);$i++) 
        for($ii=strlen($inteiro[$i]);$ii<3;$ii++) 
            $inteiro[$i] = "0".$inteiro[$i]; 

    $fim = count($inteiro) - ($inteiro[count($inteiro)-1] > 0 ? 1 : 2); 
    
    for ($i=0;$i<count($inteiro);$i++) { 
        $valor = $inteiro[$i]; 
        $rc = (($valor > 100) && ($valor < 200)) ? "cento" : $c[$valor[0]]; 
        $rd = ($valor[1] < 2) ? "" : $d[$valor[1]]; 
        $ru = ($valor > 0) ? (($valor[1] == 1) ? $d10[$valor[2]] : $u[$valor[2]]) : ""; 

        $r = $rc.(($rc && ($rd || $ru)) ? " e " : "").$rd.(($rd && $ru) ? " e " : "").$ru; 
        $t = count($inteiro)-1-$i; 
        $r .= $r ? " ".($valor > 1 ? $plural[$t] : $singular[$t]) : ""; 
        if ($valor == "000")$z++; elseif ($z > 0) $z--; 
        if (($t==1) && ($z>0) && ($inteiro[0] > 0)) $r .= (($z>1) ? " de " : "").$plural[$t]; 
        if ($r) $rt = $rt . ((($i > 0) && ($i <= $fim) && ($inteiro[0] > 0) && ($z < 1)) ? ( ($i < $fim) ? ", " : " e ") : " ") . $r; 
    } 

    if(!$maiusculas) { 
    	return($rt ? trim($rt) : "zero"); 
    } else { 
    	return (ucwords($rt) ? ucwords(trim($rt)) : "Zero"); 
	} 

} 

/**
 * @description : Fun��o para gravar o tema escolhido pelo usu�rio
 * @category Function
 * @version 1.0
 * @param string $theme		tema do usu�rio 
 * @param string $usucpf	cpf do usu�rio
 * @author Juliano Meinen
 * @since 01/06/2010
 * @example : gravaThemaUsuario($_POST['theme]);
 */
function gravaThemaUsuario($theme = "verde",$usucpf = null){
	global $db;
	
	$usucpf = !$usucpf ? $_SESSION['usucpf'] : $usucpf;
	
	if(!$usucpf)
		return false;
	
	if(!$theme)
		return false;
	
	$sql = "SELECT
				pruvalor
			FROM
				seguranca.parametro_usuario
			WHERE
				usucpf = '$usucpf'
			AND
				prunome = 'tema'
			AND
				prustatus = 'A'";
	
	$tema = $db->pegaUm($sql);
	
	if(!$tema){
		
		$sql = "INSERT 
					INTO
				seguranca.parametro_usuario
					(usucpf,prunome,pruvalor,prudesc,prustatus)
				VALUES
					('$usucpf','tema','$theme','Tema do sistema selecionado pelo usu�rio.','A')";
		
	}else{
		
		$sql = "UPDATE 
					seguranca.parametro_usuario
				SET
					pruvalor = '$theme'
				WHERE
					usucpf = '$usucpf'
				AND
					prunome = 'tema'
				AND
					prustatus = 'A'";
	}
	
	if($db->executar($sql)){
		$db->commit($sql);
		return true;	
	}
	
	
}


/**
 * @description : Fun��o que recupera o tema escolhido pelo usu�rio
 * @category Function
 * @version 1.0 
 * @param string $usucpf	cpf do usu�rio
 * @author Juliano Meinen
 * @since 02/06/2010
 * @return string ( nome do tema)
 * @example : recuperaThemaUsuario();
 */
function recuperaThemaUsuario($usucpf = null){
	global $db;
	
	$usucpf = !$usucpf ? $_SESSION['usucpf'] : $usucpf;
	
	if(!$usucpf)
		return false;
	
	$sql = "SELECT
				pruvalor
			FROM
				seguranca.parametro_usuario
			WHERE
				usucpf = '$usucpf'
			AND
				prunome = 'tema'
			AND
				prustatus = 'A'";
	
	$tema = $db->pegaUm($sql);
	
	if($tema)
		return $tema;
	else
		return false;
	
}

/**
 * @description : Fun��o valida um xml com a referencia de um xsd
 * @category Function
 * @version 1.0 
 * @param string $arqXml	pode ser um arquivo ou um string xml
 * @author Wesley Romualdo da Silva
 * @since 10/06/2010
 * @return array com mensagem de erro, caso ocorra.
 * @example :
 	1 - Exemplo 1
 		$arqXml = <<<XML 
 		<?xml version="1.0" encoding="iso-8859-1"?>
		<catalogo>
		    <livro>
		        <isbn>8504006115</isbn>
		        <autor>George Orwell</autor>
		        <titulo>1984</titulo>
		        <paginas>302</paginas>
		    </livro>
		</catalogo>
		XML;
		$xsd = 'catalogo.xsd';
		validaXML($arqXml, $xsd);

	2 - Exemplo 2
		$arqXml = 'catalogo.xml';
		$xsd = 'catalogo.xsd';
		validaXML($arqXml, $xsd);
		
 */
function validaXML($arqXml, $xsd){
	$dom = new DOMDocument('1.0', 'iso-8859-1');
	
	if( is_file( $arqXml ) ){
		$dom->load($arqXml);
	} else {
		$dom->loadXML($arqXml);
	}
	
    //Enable user error handling
    libxml_use_internal_errors(true);
    $arMessage = array();
    if(!$dom->schemaValidate( $xsd )) {
    	$errors = libxml_get_errors();
        
        foreach ($errors as $error) {
        	$arMessage[] = array('code' => $error->code,
                                 'text' => $error->message );
        }
    }
    return $arMessage;
}


/**
 * @description : Fun��o para associar perfil ao usu�rio e caso esteja pendente, ativ�-lo. (Descontinuada)
 * @category Function
 * @version 1.0 
 * @param string/array $usucpf	cpf do usu�rio
 * @param string/array $perfil	perfil do usu�rio (normalmente � uma constante com valor num�rico)
 * @param string	$sisid	(opcional) identificador do sistema 
 * @param array	$arrUsuResp	(opcional) colunas e valores para inclus�o na tabela de usuarioresponsabilidade do schema 
 * @author Juliano Meinen
 * @since 28/07/2010
 * @return boolean ( true se tudo ocorrer normalmente / falso se houver erros )
 * @example : associaPerfilUsuarioPendente($_SESSION['usucpf'],PERFIL_COD_GESTOR,11,array("atiid" => 1);
 */
function associaPerfilUsuarioPendente($usucpf,$perfil,$sisid = null,$arrUsuResp = null){
	global $db;
	
	if(!$sisid)
		$sisid = $_SESSION['sisid'];

	if(!$usucpf)
		return false;
	
	if(!$perfil)
		return false;
	
	if(!is_array($usucpf))
		$arrUsucpf = array($usucpf);
	else
		$arrUsucpf = $usucpf;
	
	if(!is_array($perfil))
		$arrPerfil = array($perfil);
	else
		$arrPerfil = $perfil;
	
	foreach($arrUsucpf as $usucpf):
		if(strlen(limpar_numero($usucpf)) == 11)
			$arrUsucpfTratado[] = limpar_numero($usucpf);
		else
			return false;	
	endforeach;
	
	foreach($arrPerfil as $perfil2):
		$arrPerfilTratado[] = $perfil2;	
	endforeach;
	
	//Passo 1 - Selecionar os Usu�rios Pendentes (suscod = 'A')
	$sql = "select
				usunome,
				usucpf,
				usuemail,
				ususexo,
				ususenha
			from 
				seguranca.usuario 
			where 
				usucpf in ('".implode("','",$arrUsucpfTratado)."') 
			and 
				suscod = 'P'";
	$arrDadosUsuario = $db->carregar($sql);
	
	if(is_array($arrDadosUsuario)):
		foreach($arrDadosUsuario as $dadoUsuario):
			$remetente = array("nome" => $_SESSION['usunome'],"email" => $_SESSION['usuemail']);
			$destinatario = $dadoUsuario['usuemail'];
			$assunto = "Aprova��o do Cadastro no Sistema";
			$conteudo = "
				<br/>
				<span style='background-color: red;'><b>Esta � uma mensagem gerada automaticamente pelo sistema. </b></span>
				<br/>
				<span style='background-color: red;'><b>Por favor, n�o responda. Pois, neste caso, a mesma ser� descartada.</b></span>
				<br/>
				";
			$conteudo .= sprintf(
									"%s %s<p>Sua conta est� ativa.</p>",
									$dadoUsuario['ususexo'] == 'M' ? 'Prezado Sr.' : 'Prezada Sra.',
									$dadoUsuario['usunome']
								);
			//Passo 2 - Atualizar os status dos usu�rios Pendentes (suscod = 'A')
			alterar_status_usuario( $dadoUsuario['usucpf'], 'A', "Atribui��o de responsabilidade em atividade ou projeto.", $sisid );
			//Passo 3 - Enviar e-mails para usu�rios Pendentes de Ativa��o
			//enviar_email( $remetente, $destinatario, $assunto, $conteudo );
			$arrUsucpfPendentes[] = $dadoUsuario['usucpf'];
		endforeach;
	endif;
	
	//Passo 4 - Coletar dados do esquema por meio do sisid
	$sqlSchema = "select 
				sisdiretorio 
			from 
				seguranca.sistema 
			where 
				sisid = $sisid";
	$schema = $db->pegaUm($sqlSchema);
	
	if(is_array($arrUsuResp)):
		foreach($arrUsuResp as $campo => $valor):
			$arrAnd[] = "$campo = '$valor'";
			$arrInsert[] = $campo;
			$arrValorInsert[] = "'$valor'";
		endforeach;
	endif;
	
	//Passo 5 - Desativar todos os usu�rios que n�o fazem parte dos usu�rios passados por par�metro
	if( count($arrUsucpfTratado) == count($arrPerfilTratado) ):
		$n = 0;
		foreach($arrUsucpfTratado as $usuCpf):
			$sqlClear = "update
							$schema.usuarioresponsabilidade
						set
							rpustatus = 'I'
						where
							usucpf != '$usuCpf'
						and
							pflcod = {$arrPerfilTratado[$n]}
						".(count($arrAnd) ? " AND ".implode(" AND ", $arrAnd) : "")."";
			dbg($sqlClear);//$db->executar($sqlClear);
		$n++;
		endforeach;
	else:
		$sqlClear = "update
						$schema.usuarioresponsabilidade
					set
						rpustatus = 'I'
					where
						usucpf not in ('".implode("','", $arrUsucpfTratado)."') 
					and
						pflcod = {$arrPerfilTratado[0]}
					".(count($arrAnd) ? " AND ".implode(" AND ", $arrAnd) : "")."";
		dbg($sqlClear);//$db->executar($sqlClear);
	endif;
	
	
	$n = 0;
	foreach($arrUsucpfTratado as $usuCpf):
		
		$pflcod = count($arrUsucpfTratado) == count($arrPerfilTratado) ? $arrPerfilTratado[$n] : $arrPerfilTratado[0];
		
		$sqlConsulta = "select
							rpuid
						from
							$schema.usuarioresponsabilidade
						where
							usucpf = '$usuCpf'
						and
							rpustatus = 'I'
						and
							pflcod = $pflcod
						".(count($arrAnd) ? " AND ".implode(" AND ", $arrAnd) : "")."";
		$rpuid = $db->pegaUm($sqlConsulta);
		if($rpuid):
			$arrSql .= "update
							$schema.usuarioresponsabilidade
						set
							rpustatus = 'A'
						where
							rpuid = $rpuid;";
		else:
			$arrSql .= "insert into $schema.usuarioresponsabilidade
							(usucpf,rpustatus,rpudata_inc,pflcod".(count($arrInsert) ? ",".implode(",",$arrInsert) : "").")
						values
							('$usuCpf','A',now(),'$pflcod'".(count($arrValorInsert) ? ",".implode(",",$arrValorInsert) : "").");";
		endif;
		$n++;	
	endforeach;
}

function somar_dias_uteis($str_data, $int_qtd_dias_somar = 7) {

    // Caso seja informado uma data do tipo DATETIME - aaaa-mm-dd 00:00:00
    // Transforma para DATE - aaaa-mm-dd
    $str_data = substr($str_data,0,10);
    
    // Se a data estiver no formato brasileiro: dd/mm/aaaa
    // Converte-a para o padr�o americano: aaaa-mm-dd
    if ( preg_match("@/@",$str_data) == 1 ) {
        $str_data = implode("-", array_reverse(explode("/",$str_data)));
    }
    
    $array_data = explode('-', $str_data);
    $count_days = 0;
    $int_qtd_dias_uteis = 0;
    while ( $int_qtd_dias_uteis < $int_qtd_dias_somar ) {
        $count_days++;
                if ( ( $dias_da_semana = gmdate('w', strtotime('+'.$count_days.' day', mktime(0, 0, 0, $array_data[1], $array_data[2], $array_data[0]))) ) != '0' && $dias_da_semana != '6' ) {
            $int_qtd_dias_uteis++;
        }
    }
    return gmdate('Y-m-d',strtotime('+'.$count_days.' day',strtotime($str_data)));
}

function pegaPerfilGeral($usucpf = null,$sisid = null){
	global $db;
	
	$usucpf = !$usucpf ? $_SESSION['usucpf'] : $usucpf;
	$sisid  = !$sisid  ? $_SESSION['sisid']  : $sisid;
	
	$sql = "select 
				pu.pflcod
			from 
				seguranca.perfilusuario pu 
			inner join 
				seguranca.perfil p on p.pflcod = pu.pflcod
			and 
				pu.usucpf = '$usucpf' 
			and 
				p.sisid = $sisid
			and
				pflstatus = 'A'";
				
	$arrPflcod = $db->carregar($sql);
	
	!$arrPflcod? $arrPflcod = array() : $arrPflcod = $arrPflcod;
	
	foreach($arrPflcod as $pflcod){
		$arrPerfil[] = $pflcod['pflcod'];
	}
	
	return $arrPerfil ? $arrPerfil : false;
}

function popupAlertaGeral($texto = "Informe o texto.",$largura = "400px",$altura = "200px",$id = "id_popup_alerta", $classeCSS = null)
{ ?>
	<style>
		.popup_alerta{
				width:<?php echo $largura ?>;height:<?php echo $altura ?>;position:absolute;z-index:0;top:50%;left:50%;margin-top:-<?php echo $altura/2 ?>;margin-left:-<?php echo $largura/2 ?>;border:solid 2px black;background-color:#FFFFFF;}
	</style>
	<div id="<?php echo $id ?>" class="popup_alerta <?php echo $classeCSS ?>" >
		<div style="width:100%;text-align:right">
			<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" />
		</div>
		<div style="padding:5px;text-align:justify;">
			<?php echo $texto ?>
		</div>
	</div>
<?php }

function verificaExistenciaArquivo($arqid,$nomeEsquema = null)
{
	$nomeEsquema = !$nomeEsquema ? $_SESSION['sisdiretorio'] : $nomeEsquema;
	$caminho = APPRAIZ."arquivos/".$nomeEsquema.'/'.floor($arqid/1000).'/'.$arqid;
	if(is_file($caminho)){
		return true;
	}else{
		return false;
	}
}

function verificaArquivoUsuarioTabela($schema,$tbl,$usucpf = null)
{
	global $db;
	
	$usucpf = !$usucpf ? $_SESSION['usucpf'] : $usucpf;
	
	$arrWhere[] = "a.usucpf = '$usucpf'";
	$arrWhere[] = "(a.arqid / 1000) between 647 and 725";
	$arrWhere[] = "a.arqid not in(select arqid from public.arquivo_recuperado)";
	
	$sql = "select 
				a.arqid
			from
				public.arquivo a
			inner join
				$schema.$tbl tbl ON tbl.arqid = a.arqid
			and
				(a.arqstatus = 'A'::bpchar or a.arqstatus = '1'::bpchar)
			".($arrWhere ? " and ".implode(" and ",$arrWhere) : "");

	if($db->pegaUm($sql)){
		return true;
	}else{
		return false;	
	}
	
}

function extenso($valor = 0, $maiusculas = false, $masc = true) {

	$singular  = array("", "", "mil", "milh�o", "bilh�o", "trilh�o", "quatrilh�o");
	$plural    = array("", "", "mil", "milh�es", "bilh�es", "trilh�es", "quatrilh�es");
	
	$c = array("", "cem", "duzentos", "trezentos", "quatrocentos", "quinhentos", "seiscentos", "setecentos", "oitocentos", "novecentos");
	$d = array("", "dez", "vinte", "trinta", "quarenta", "cinquenta", "sessenta", "setenta", "oitenta", "noventa");
	$d10 = array("dez", "onze", "doze", "treze", "quatorze", "quinze", "dezesseis", "dezesete", "dezoito", "dezenove");
	if($masc){
		$u = array("", "um", "dois", "tr�s", "quatro", "cinco", "seis", "sete", "oito", "nove");
	}else{
		$u = array("", "uma", "duas", "tr�s", "quatro", "cinco", "seis", "sete", "oito", "nove");
	}
	
	$z = 0;
	$rt = "";
	
	$valor = number_format($valor, 2, ".", ".");
	$inteiro = explode(".", $valor);
	for($i=0;$i<count($inteiro);$i++)
		for($ii=strlen($inteiro[$i]);$ii<3;$ii++)
			$inteiro[$i] = "0".$inteiro[$i];
	
	$fim = count($inteiro) - ($inteiro[count($inteiro)-1] > 0 ? 1 : 2);
	for ($i=0;$i<count($inteiro);$i++) {
		$valor = $inteiro[$i];
		$rc = (($valor > 100) && ($valor < 200)) ? "cento" : $c[$valor[0]];
		$rd = ($valor[1] < 2) ? "" : $d[$valor[1]];
		$ru = ($valor > 0) ? (($valor[1] == 1) ? $d10[$valor[2]] : $u[$valor[2]]) : "";
		
		$r = $rc.(($rc && ($rd || $ru)) ? " e " : "").$rd.(($rd &&
		$ru) ? " e " : "").$ru;
		$t = count($inteiro)-1-$i;
		$r .= $r ? " ".($valor > 1 ? $plural[$t] : $singular[$t]) : "";
		if ($valor == "000")$z++; elseif ($z > 0) $z--;
		if (($t==1) && ($z>0) && ($inteiro[0] > 0)) $r .= (($z>1) ? " de " : "").$plural[$t];
		if ($r) $rt = $rt . ((($i > 0) && ($i <= $fim) && ($inteiro[0] > 0) && ($z < 1)) ? ( ($i < $fim) ? ", " : " e ") : " ") . $r;
	}
	
	if(!$maiusculas){
		return($rt ? $rt : "zero");
	} else {
		if ($rt) $rt=ereg_replace(" E "," e ",ucwords($rt));
		return (($rt) ? ($rt) : "Zero");
	}

}

function retornaPflcodFilhos($arrPerfil){
	global $db;
	$sql = "select 
				pflcodfilho 
			from 
				seguranca.perfilpermissao 
			where 
				pflcodpai in (".implode(",",$arrPerfil).")
			and
				pflcodfilho not in (".implode(",",$arrPerfil).")";
	$arrFilhos = $db->carregarColuna($sql);
	if($arrFilhos){
		foreach($arrFilhos as $f){
			$arrPerfil[] = $f;
		}
		$arrPerfil = retornaPflcodFilhos($arrPerfil);
	}
	return array_unique($arrPerfil);	
}
