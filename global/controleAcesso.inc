<?

// carrega a p�gina solicitada pelo usu�rio
$sql = sprintf( "select u.usuchaveativacao from seguranca.usuario u where u.usucpf = '%s'", $_SESSION['usucpf'] );
$chave = $db->pegaUm( $sql );

if ( $chave == 'f' )
{
	// leva o usu�rio para o formul�rio de troca de senha
	include APPRAIZ . $_SESSION['sisdiretorio'] . "/modulos/sistema/usuario/altsenha.inc";
	include APPRAIZ . "includes/rodape.inc";
}
else if ( $_REQUEST['modulo'] )
{
	// leva o usu�rio para a p�gina solicitada
	include APPRAIZ . $_SESSION['sisdiretorio'] . "/modulos/" . $_REQUEST['modulo'] . ".inc";
	
	include APPRAIZ . "includes/rodape.inc";
}
else
{
	// leva o usu�rio para o formul�rio de login
	header( "Location: ../login.php" );
}


?>