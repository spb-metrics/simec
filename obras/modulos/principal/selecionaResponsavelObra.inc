<?php
/*
 * Dúvida Tirada com o Mário - 16-06-2010
 * 
 * Nota-se que no filtro por perfil há apenas os perfis "Consulta Geral", "Gestor Unidade", "Supervisor Unidade", "Empresa Contratada", porém
 * na listagem mostra-se por padrão o resultado vinculado aos perfis "Supervisor Unidade", "Gestor Unidade", "Consulta Tipo de Ensino", 
 * "Consulta Unidade".
 * 
 * Mário afirmou que está correto.
 * 
 * Dúvida tirada com o Mário - 18-06-2010
 * 
 * Retirar o perfil "Consulta Geral" do filtro de pesquisa
 * Retirar o perfil "Consulta Unidade" da lista padrão
 * 
 */

$obrid = $_SESSION['obra']['obrid'];
$orgid = $_SESSION['obra']['orgid'];

$obObra 	= new Obras();
$estuf 		= $obObra->buscaUF( $obrid );

if ( $orgid == ORGAO_FNDE ){
	$filtro = " AND ur.pflcod IN (" . PERFIL_SUPERVISORUNIDADE . ", " . PERFIL_GESTORUNIDADE . ", " . PERFIL_EMPRESA . ")";
	$disabledPerfil = 'S';
}else{
	$filtro = " AND ur.pflcod IN (" . PERFIL_EMPRESA . ")";
	$disabledPerfil = 'N';
}	

switch ($_REQUEST["requisicao"]) {

	case "pesquisa":
		if ( $_POST['pflcod'] )
			$filtro = " AND ur.pflcod = '" . $_POST['pflcod'] . "'";
		if ( $_POST['usucpf'] )
			$filtro = " AND ur.usucpf = '" . $_POST['usucpf'] . "'";
//		$filtro = pesquisaResponsavelObra( $_REQUEST );
	break;
	case "associa":
		 associaResponsavelObra( $_REQUEST );
	break;
	
}

if ($_REQUEST["entid"] == 'undefined') {
		echo "<script>
				alert('A Obra não possui Unidade vinculada!');
				window.close();
			  </script>";
		die;
}

print '<br/>';
monta_titulo( 'Usuário cadastrados no Módulo', '');

?>

<html>
	<head>
		<title><?php echo $GLOBALS['parametros_sistema_tela']['nome_e_orgao']; ?></title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script type="text/javascript" src="../includes/prototype.js"></script>
	    <script type="text/javascript" src="../includes/entidades.js"></script>
	    <script type="text/javascript" src="/includes/estouvivo.js"></script>
	    <style>
			#div_rolagem table {
				width: 100%;                
			}
       </style>
	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
		<form id="pesquisa" name="pesquisa" method="post" action="">
			<input type="hidden" name="requisicao" value="pesquisa"/>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
				<tr>
					<td class="subtitulocentro" colspan="2">Filtros de Pesquisa</td>
				</tr>
				<tr>
					<td class="subtitulodireita" width="170px">CPF</td>
					<td>
						<?php $usucpf = $_REQUEST["usucpf"]; ?>
						<?= campo_texto( 'usucpf', 'N', $somenteLeitura, '', 18, 15, '###.###.###-##', '', 'left', '', 0, 'id="usucpf"'); ?>
					</td>
				</tr>
				<tr>
					<td class="subtitulodireita">Perfil</td>
					<td>
						<?php
							$pflcod = $orgid == ORGAO_FNDE ? $_REQUEST["pflcod"] : PERFIL_EMPRESA;
							
							$sql = "SELECT
										pflcod as codigo,
										pfldsc as descricao
									FROM
										seguranca.perfil
									WHERE
										sisid = {$_SESSION['sisid']} AND
										pflstatus = 'A' AND
										pflcod in (" . PERFIL_GESTORUNIDADE . ", " . PERFIL_SUPERVISORUNIDADE . ", " . PERFIL_EMPRESA . ")";
							
							$db->monta_combo("pflcod", $sql, $disabledPerfil, "Selecione...", '', '', '', '', 'N', 'pflcod');
						
						?>
					</td>
				</tr>
				<tr bgcolor="#C0C0C0">
					<td></td>
					<td>
						<input type="button" style="cursor: pointer;" onclick="document.getElementById('pesquisa').submit();" value="Pesquisar"/>
					</td>
				</tr>
			</table>
		</form>
		<form id="formulario" name="formulario" method="post" action="">
			<input type="hidden" name="requisicao" value="associa"/>
			<center>
				<div id="div_rolagem" style="overflow-x: auto; overflow-y: auto; width:95%; height:180px;">
				<?php
					$sql = "SELECT DISTINCT
								'<center><input type=\"checkbox\" name=\"rpuid[' || rpuid || ']\" id=\"rpuid\" value=\"' || su.usunome || '_' || su.usucpf || '\" /></center>',
								su.usucpf, 
								su.usunome 
							FROM 
								seguranca.usuario su 
							INNER JOIN 
								obras.usuarioresponsabilidade ur ON ur.usucpf = su.usucpf 
							WHERE 
								rpustatus = 'A' AND 
								(ur.entid = {$_REQUEST["entid"]} OR ur.orgid = {$_SESSION['obras']['orgid']} OR ur.estuf = '{$estuf}')
								{$filtro}
							ORDER BY 
								su.usunome";
								
					$cabecalho = array( "Ação", "CPF", "Nome");
					$db->monta_lista_simples( $sql, $cabecalho, 10000, 30, 'N', '95%');
				?>
				</div>
			</center>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
				<tr bgcolor="#C0C0C0">
					<td>
						<input type="button" style="cursor: pointer;" onclick="document.getElementById('formulario').submit();" value="Fechar"/>
					</td>
				</tr>
			</table>
		</form>
	</body>
</html>
