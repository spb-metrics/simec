<?php

$supervisao = new supervisao();

switch( $_REQUEST["requisicao"] ){
	
	case "pesquisa":
		$filtros = $supervisao->obrFiltraListaObrasRota();
	break;
	
}

monta_titulo( "Inserir Obras na Rota", "" );

?>

<html>
	<head>
		<title><?php echo $GLOBALS['parametros_sistema_tela']['nome_e_orgao']; ?></title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script src="/includes/prototype.js"></script>
		<script src="../includes/calendario.js"></script>
		<script src="../obras/js/obras.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
		<form action="" method="post" name="formulario" id="obrFormObraRota">
			<input type="hidden" value="pesquisa" name="requisicao" id="requisicao"/>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
				<tr>
					<td class="subTituloCentro" colspan="2">Argumentos de Pesquisa</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" width="190px;">Tipo de Ensino:</td>
					<td>
						<?php 
							
							$orgid = $_REQUEST["orgid"];
						
							$sql = "SELECT
										orgid as codigo,
										orgdesc as descricao
									FROM
										obras.orgao";
							
							$db->monta_combo("orgid", $sql, "S", "Todos", '', '', '', '', 'N','orgid');
							
						?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Nome da Obra:</td>
					<td>
						<?php 
							$obrdesc = $_REQUEST["obrdesc"]; 
							print campo_texto( "obrdesc", "N", "S", "", 65, 60, "", "", "left", "", 0, "obrdesc"); 
						?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Unidade ResponsÃ¡vel pela Obra:</td>
					<td>
						<?php
							
							$entidunidade = $_REQUEST["entidunidade"];
					
							$sql = "SELECT DISTINCT
										entid as codigo,
										entnome as descricao
									FROM
										entidade.entidade ee
									INNER JOIN
										obras.obrainfraestrutura oi ON oi.entidunidade = ee.entid
									INNER JOIN
										obras.repositorio ore ON ore.obrid = oi.obrid
									INNER JOIN
										obras.itemgrupo it ON it.repid = ore.repid
									WHERE
										repstatus = 'A' AND
										it.gpdid = {$_SESSION["obras"]["gpdid"]}";
							
							$db->monta_combo("entidunidade", $sql, "S", "Todas", '', '', '', '', 'N','entidunidade');
							
						 ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Situação da Obra:</td>
					<td>
						<?php 
						
							$stoid = $_REQUEST["stoid"];
						
							$sql = "SELECT
										stoid as codigo,
										stodesc as descricao
									FROM
										obras.situacaoobra
									ORDER BY
										stodesc";
						
							$db->monta_combo("stoid", $sql, "S", "Todas", '', '', '', '', 'N','stoid');
							
						?>
					</td>
				</tr>
				<tr bgColor="#D0D0D0">
					<td></td>
					<td>
						<input type="button" value="Pesquisar" style="cursor: pointer;" onclick="document.getElementById('obrFormObraRota').submit();"/>
					</td>
				</tr>
				<tr>
					<td class="subTituloCentro" colspan="2">Lista de Obras</td>
				</tr>
			</table>
		</form>
		
		<?php $supervisao->obrMontaListaObrasRota( $_SESSION["obras"]["gpdid"], $filtros ); ?>
		
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
			<tr bgColor="#D0D0D0">
				<td>
					<input type="button" value="Fechar" style="cursor: pointer;" onclick="self.close();"/>
				</td>
			</tr>
		</table>
		
	</body>
	<script type="text/javascript">
		obrCheckaObraRota();
	</script>
</html>
