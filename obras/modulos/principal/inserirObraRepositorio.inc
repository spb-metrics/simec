<?php

$supervisao = new supervisao();

switch( $_REQUEST["requisicao"] ){
	
	case "lista":
		
		$where = $_REQUEST["estuf"] ? " AND estuf = '{$_REQUEST["estuf"]}'" : "";
		$where .= $_REQUEST["entidunidade"] ? " AND entidunidade = {$_REQUEST["entidunidade"]}" : "";  
		
		$sql = "SELECT
					'<center><input type=\"checkbox\" name=\"obrid[]\" value=\" ' || oi.obrid || ' \" id=\"obrid\" /></center>' as acao,
					obrdesc as nome,
					stodesc as situacao
				FROM
					obras.obrainfraestrutura oi
				INNER JOIN
					obras.situacaoobra st ON st.stoid = oi.stoid
				INNER JOIN
					entidade.endereco ed ON ed.endid = oi.endid 
				WHERE
					obsstatus = 'A' AND orgid = {$_SESSION["obras"]["orgidRepositorio"]} {$where}
				ORDER BY
					obrdesc, stodesc";
		
		$cabecalho = array( "AÁ„o", "Nome da Obra", "SituaÁ„o da Obra", "Data Inicial", "Data Final" );
		
		$db->monta_lista( $sql, $cabecalho, 100, 10, "N", "center", "" );
			
		die;
		
	break;
	
	case "salvar":
		$supervisao->insereObrasRepositorio( $_REQUEST["obrid"] );	
	break;
	
}

// monta o titulo da tela
monta_titulo( "Selecionar Obras Para o RepositÛrio", "" );

?>

<html>
	<head>
		<title><?php echo $GLOBALS['parametros_sistema_tela']['nome_e_orgao']; ?></title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script src="/includes/prototype.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
		<form action="" method="post" id="obrFormInsereObras" name="formulario">
			<input type="hidden" value="salvar" name="requisicao" id="requisicao"/>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
				<tr>
					<td class="SubTituloDireita" width="190px">Unidade Respons√°vel pela Obra:</td>
					<td>
						<?php 
				
							$sql = "SELECT DISTINCT
										entid as codigo,
										entnome as descricao
									FROM
										entidade.entidade ee
									INNER JOIN
										obras.obrainfraestrutura oi ON oi.entidunidade = ee.entid
									WHERE
										obsstatus = 'A' AND orgid = {$_SESSION["obras"]["orgidRepositorio"]}
									ORDER BY
										entnome";
							
							$db->monta_combo("entidunidade", $sql, "S", "Todas", '', '', '', '350', 'N','entidunidade');
							
						?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" width="190px">UF:</td>
					<td>
						<?php 
				
							$sql = "SELECT
										estuf as codigo,
										estdescricao as descricao
									FROM
										territorios.estado";
							
							$db->monta_combo("estuf", $sql, "S", "Todos", '', '', '', '', 'N','estado');
							
						?>
					</td>
				</tr>
				<tr bgcolor="#D0D0D0">
					<td></td>
					<td>
						<input type="button" value="Pesquisar" onclick="obrPesquisaListaObras();" style="cursor: pointer;"/>
					</td>
				</tr>
				<tr>
					<td colspan="2" class="SubTituloCentro">Lista de Obras</td>
				</tr>
			</table>
			<span id="listaObrasRepositorio">
				<font style="font-size: 8pt; color: #dd0000;">
					<center>Utilize os filtros para visualizar as obras.</center>
				</font>
			</span>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
				<tr bgcolor="#D0D0D0">
					<td>
						<input type="button" value="Salvar" onclick="document.getElementById('obrFormInsereObras').submit();" style="cursor: pointer;"/>
						<input type="button" value="Fechar" onclick="self.close();" style="cursor: pointer;"/>
					</td>
				</tr>
			</table>
		</form>
	</body>
</html>
