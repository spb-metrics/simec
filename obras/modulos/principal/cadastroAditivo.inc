<?php 

if( !$_SESSION["obra"]['obrid'] ){
	header( "location:obras.php?modulo=inicio&acao=A" );
	exit;
}

$obObras = new Obras();
$obData  = new Data();

// Carrega dados da obra
$dados = $obObras->Dados($_SESSION["obra"]['obrid']);

include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
$db->cria_aba($abacod_tela,$url,'');
monta_titulo( "Termos Aditivos Contratuais", '' );

echo $obObras->CabecalhoObras();

$srcImg  = "../imagens/gif_inclui.gif";
$onClick = "AbrirPopUp('obras.php?modulo=principal/popUpInserirAditivo&acao=A', 'popUpInsertAditivo', 'toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes,resizable=yes,copyhistory=yes,width=720,height=540')";

$arr = array("foo" => "bar", 12 => true);
$pflcods = array(
					1 => PERFIL_SUPERUSUARIO,
					2 => PERFIL_ADMINISTRADOR,
					3 => PERFIL_SUPERVISORMEC,
					4 => PERFIL_EMPRESA
					);

if ( ($_SESSION['obras']['orgid'] == 3 || $_SESSION['obra']['orgid'] == 3) && !(possuiPerfil($pflcods)) ){
	$msg	= '* O seu perfil não pode adicionar aditivos.\n';	
	$srcImg = "../imagens/gif_inclui_d.gif";
	$onClick = "";
}

if ( obras_pega_situacao( $_SESSION['obra']['obrid'] ) != 1 ){
	$msg	.= '\n* A situação da obra estiver em execução.\n';	
	$srcImg = "../imagens/gif_inclui_d.gif";
	$onClick = "";
}
// Calcula a diferença entre a data atual e a ultima vistoria
//list($dtUltVistoria) = explode(" ", $dados['obrdtvistoria']);
//$difUltVistoria 	 = $obData->quantidadeDeDiasEntreDuasDatas($dtUltVistoria, date("Y-m-d"), "YYYY-MM-DD");

/*if ( $difUltVistoria > 30 ){
	$msg	.= '* A vistoria estiver atualizada.\n';	
	$srcImg  = "../imagens/gif_inclui_d.gif";
	$onClick = "";
}*/

$msg = $msg ? 'O termo aditivo só pode ser incluído se:\n\n' . $msg : $msg;
?>
<table class="Tabela" align="center" style="background: #E9E9E9; border-top:0px;">
  <tr>
    <td>
		<center>
		<fieldset style="width: 95%; background: #fff; text-align: left;">
			<legend>Lista de Aditivos</legend>
			<div style="text-align: left; height: 250px;">
			<? 
			
			$sql = "SELECT DISTINCT
							o1.obrid
						FROM
							obras.obrainfraestrutura o
						INNER JOIN
							obras.obrainfraestrutura o1 ON o1.obrid = o.obridaditivo
						WHERE
							o.obrid = {$_SESSION['obra']['obrid']}";
			$obridAux = $db->pegaUm($sql);
			
			if($obridAux) {
					$i                  = 0;
					$obridAditivos 		= array();
					$obridsAditivos   	= $obridAux;
					$obridAditivos[$i]  = $obridAux;
					$flag				= true;
					
					while($flag)
					{
						$sql = "SELECT DISTINCT
									o1.obrid
								FROM
									obras.obrainfraestrutura o
								INNER JOIN
									obras.obrainfraestrutura o1 ON o1.obrid = o.obridaditivo
								WHERE
									o.obrid = {$obridAditivos[$i]}";
						$obridAux = $db->pegaUm($sql);
						
						if($obridAux)
						{
							$i++;
							$obridAditivos[$i] = $obridAux;
							$obridsAditivos .= ','.$obridAux;
						}
						else
						{
							$flag = false;
						}
					}
				
				$condicao = "obrid in ({$obridsAditivos})";	
					
			}else{
				$condicao = "obrid IS NULL";
			}		
//			ver($condicao);	
			$sql = "SELECT 
					   traseq || ' / ' || tradsc as seqdsc,
					   ttadsc,
					   to_char(tradtinclusao, 'dd/mm/yyyy'),
					   usunome
				    FROM obras.termoaditivo
				    JOIN obras.tipotermoaditivo USING(ttaid)
				    JOIN seguranca.usuario USING(usucpf)
				    WHERE ({$condicao})
				    ORDER BY traseq;";
			
			$cabecalho = array("Nº do Aditivo/Denominação", "Tipo de aditivo", "Data de Assinatura do Aditivo", "Inserido por");
			$db->monta_lista($sql, $cabecalho, 50, 20, '', 'center', '');	
			?>
			</div>
		</fieldset>
		</center>    
		<a href="#" style="text-align: left; color: #0F55A9; margin-left: 28px;" onclick="<?=($msg ? "alert('{$msg}');" : "") ?><?=$onClick?>"><img src="<?=$srcImg ?>" border="0"> Incluir Aditivo</a>
    </td>
  </tr>
</table>

