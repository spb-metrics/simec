<? 
/*
   Sistema Simec
   Setor respons�vel: SPO-MEC
   Desenvolvedor: Equipe Simec
   Analista: Alexandre Soares Diniz
   Programador: Alexandre Soares Diniz
   M�dulo:incusuariosql.inc
   Finalidade: include de dados especificos para cadastro de usuarios no sistema de seguran�a, para inclus�o de dados, usado no modulo de seguran�a
 */

		$sql = "insert into usuario_sistema (usucpf,sisid,pflcod,susstatus) values (".
    	"'".corrige_cpf($_REQUEST['usucpf'])."',".
    	"".$_REQUEST['codigo'].",".
    	"".($_REQUEST['pflcod']==''?'null':$_REQUEST['pflcod']).",".
    	"'X')";
    	//print $sql."<br>";
    	$db->executar($sql);
    	$db -> commit();
?>