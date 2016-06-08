<html>
<body>
<?

if(!$_REQUEST['prtid']) die("<script>
								alert('Relatório não encontrado');
								window.close();
							 </script>");

// Parâmetros para a nova conexão com o banco do SIAFI
$servidor_bd = $servidor_bd_siafi;
$porta_bd    = $porta_bd_siafi;
$nome_bd     = $nome_bd_siafi;
$usuario_db  = $usuario_db_siafi;
$senha_bd    = $senha_bd_siafi;

// Cria o novo objeto de conexão
$db2 = new cls_banco();

$_POST['ano']=date("Y");

$sql = sprintf(	"select prtobj from public.parametros_tela where prtid = ".$_REQUEST['prtid']);
$itens = $db2->pegaUm( $sql );
$dados = unserialize( stripslashes( stripslashes( $itens ) ) );
$dados['uo'][0] = $_REQUEST['unicod'];
$_REQUEST = $dados;
unset( $_REQUEST['salvar'] );

include_once APPRAIZ . "financeiro/modulos/relatorio/resultadoRelatorioGeral.inc";
?>
</body>
</html>