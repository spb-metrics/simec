<?

// carrega a página solicitada pelo usuário
$sql = sprintf( "select u.usuchaveativacao from seguranca.usuario u where u.usucpf = '%s'", $_SESSION['usucpf'] );
$chave = $db->pegaUm( $sql );

if ( $chave == 'f' )
{
	// leva o usuário para o formulário de troca de senha
	include APPRAIZ . $_SESSION['sisdiretorio'] . "/modulos/sistema/usuario/altsenha.inc";
	include APPRAIZ . "includes/rodape.inc";
}
else if ( $_REQUEST['modulo'] )
{
	// leva o usuário para a página solicitada
	include APPRAIZ . $_SESSION['sisdiretorio'] . "/modulos/" . $_REQUEST['modulo'] . ".inc";
	
	include APPRAIZ . "includes/rodape.inc";
}
else
{
	// leva o usuário para o formulário de login
	header( "Location: ../login.php" );
}


?>