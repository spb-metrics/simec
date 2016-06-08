<?
 /*
   Sistema Simec
   Setor responsvel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com), Fabrício Mendonça (fabriciomendonca@gmail.com)
   Mdulo:inicio.inc
   Finalidade: permitir abrir a página inicial do módulo de orçamento - financeiro
    */
//recupera todas as variaveis que veio pelo post ou get
foreach($_REQUEST as $k=>$v) ${$k}=$v;

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

?>
<br>

<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem2">
	<tr bgcolor="#e7e7e7">
	  <td><h1>Bem-vindo</h1></td>
	</tr>
</table>

<script>
	location.href = "financeiro.php?modulo=relatorio/geral_teste&acao=R";
    function envia_email(cpf)
    {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
          window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
    }
</script>
