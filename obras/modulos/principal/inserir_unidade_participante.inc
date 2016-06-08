<?php

$termodeajuste = new termoDeAjuste();

if ( $_REQUEST["subacao"] == "cadastra" ){
	$_SESSION["obra"]["ptaid"] = $_REQUEST["ptaid"] ? $_REQUEST["ptaid"] : 'null';	
}
 
switch ( $_REQUEST["subacao"] ) {
	case "listamunicipio":
		
		$sql = "SELECT muncod as codigo, mundescricao as descricao 
				FROM territorios.municipio WHERE estuf = '{$_REQUEST["estuf"]}'
				ORDER BY mundescricao";
		
		$db->monta_combo("muncod", $sql, "S", "Selecione...", "pesquisaObra(this.value, '{$_REQUEST["estuf"]}');", '', '', '', 'N','muncod');
		die;
		
	break;
	case "salvaunidadeparticipante":
		$termodeajuste->CadastraUnidadeParticipante( $_REQUEST["entid"], $_REQUEST["ptaid"] );
	break;
	
	case "listadeunidade":
		
		$sql = "SELECT DISTINCT
					'<center>
						<img src=\"/imagens/incluir_p.gif\" style=\"padding-right: 1px; cursor: pointer; width:12px;\" 
					 	align=\"absmiddle\" vspace=\"1\" title=\"Inserir Unidade\" border=\"0\" onclick=\"salvaUnidadeParticipante(' || ee.entid || ', {$_SESSION["obra"]["ptaid"]});\">
					 </center>' as acao,
					ee.entnome as nome
				FROM
					entidade.entidade ee
				INNER JOIN
					entidade.endereco ed ON ee.entid = ed.entid
				INNER JOIN
					obras.obrainfraestrutura oi ON oi.entidunidade = ee.entid
				WHERE
					ed.estuf = '{$_REQUEST["estuf"]}' AND ed.muncod = '{$_REQUEST["muncod"]}' AND
					oi.obsstatus = 'A' AND oi.orgid = '{$_REQUEST["orgid"]}' 
				ORDER BY
					ee.entnome";

		$cabecalho = array( "Ação", "Unidade" );
		$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '95%');
	
		die;
		
	break;
	
}

?>

<html>
	<head>
		<title><?php echo $GLOBALS['parametros_sistema_tela']['nome_e_orgao']; ?></title>

		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<script type="text/javascript" src="../includes/prototype.js"></script>
		<script type="text/javascript" src="../includes/entidades.js"></script>
		<script src="/obras/js/obras.js"></script>
		<script>
			
			function listaMunicipio( estuf ){
			
				var url = 'obras.php?modulo=principal/inserir_unidade_participante&acao=A&subacao=listamunicipio&estuf=' + estuf;
		
				var myAjax = new Ajax.Updater(
					"municipio",
					url,
					{
						method: 'post',
						asynchronous: false
					});
				
			}
			
			function pesquisaObra( muncod, estuf ){
			
				var orgid = window.opener.document.getElementById('orgid').value;
				var url   = 'obras.php?modulo=principal/inserir_unidade_participante&acao=A&subacao=listadeunidade&muncod=' + muncod + '&estuf=' + estuf + '&orgid=' + orgid;
		
				var myAjax = new Ajax.Updater(
					"listadeunidade",
					url,
					{
						method: 'post',
						asynchronous: false
					});
				
			}
			
			function salvaUnidadeParticipante( entid, ptaid ){
				window.location.href = 'obras.php?modulo=principal/inserir_unidade_participante&acao=A&subacao=salvaunidadeparticipante&entid=' + entid + '&ptaid=' + ptaid;
			}
			
		</script>
	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	
	<form action="" method="post" name="formulario" id="formulario">
		<input type="hidden" name="pesquisa" value="1"/>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
			<tr>
				<td colspan="2" class="subtitulocentro">Unidade do Participante</td>
			</tr>
			<tr>
				<td colspan="2"><b>Filtros de Pesquisa</b></td>
			</tr>
			<tr>
				<td class="subtitulodireita">Estado</td>
				<td>
					<?php 
						$estuf = $_REQUEST["estuf"];
						$sql = "SELECT estuf as codigo, estdescricao as descricao 
								FROM territorios.estado
								ORDER BY estdescricao";
						$db->monta_combo("estuf", $sql, "S", "Selecione...", "listaMunicipio(this.value);", '', '', '150', 'N','estuf');
					?>
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita">Município</td>
				<td>
					<?php 
						if ( $_REQUEST["pesquisa"] == 1 ){
							
							$muncod = $_REQUEST["muncod"];
							$sql = "SELECT muncod as codigo, mundescricao as descricao 
									FROM territorios.municipio WHERE estuf = '{$_REQUEST["estuf"]}'";
							$db->monta_combo("muncod", $sql, "S", "Selecione...", '', '', '', '', 'N','muncod');
							
						}else{
							print '<div id="municipio" style="color: #909090">Selecione um Estado...</div>';
						}
					?>
					
				</td>
			</tr>
		</table>
	</form>
	<div id="listadeunidade"></div>
</html>
