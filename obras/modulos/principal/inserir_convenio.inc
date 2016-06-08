<?php

if (!$_SESSION["obra"]["obrid"]){
	die('<script type="text/javascript">
			alert(\'Escolha uma obra!\');
			window.close();
		 </script>');	
}


$titulo_modulo = "Inserir Convênio ou Contrato de Repasse";
monta_titulo( $titulo_modulo, "Clique no número do convênio ou contrato de repasse para adicioná-lo à obra");

if ( $_REQUEST["pesquisa"] == 1 ){
	
	$pesquisa = "";
	
	$pesquisa .= $_REQUEST["covnumero"]     ? " AND trim(co.covnumero) 		    = '{$_REQUEST["covnumero"]}' "   : "";
	$pesquisa .= $_REQUEST["covobjeto"]     ? " AND trim(co.covobjeto) ilike   '%{$_REQUEST["covobjeto"]}%' " : "";
	$pesquisa .= $_REQUEST["covdtinicio"]   ? " AND co.covdtinicio	 			= '" . formata_data_sql( $_REQUEST["covdtinicio"] ) . "'" : "";
	$pesquisa .= $_REQUEST["covdtfinal"]    ? " AND co.covdtfinal			 	= '" . formata_data_sql( $_REQUEST["covdtfinal"] ) . "'"    : "";
	
}else{
	$pesquisa = "";
}

?>

<html>
	<head>
		<title>Inserir Convênio ou Contrato de Repasse</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script src="../includes/prototype.js"></script>
		<script src="../includes/calendario.js"></script>
		<script type="text/javascript">
			function validaPesquisaConvenio(){

				var inicio = document.getElementById('covdtinicio');
				var final  = document.getElementById('covdtfinal');
				
				if( !validaData(inicio) ){
					alert("A data inicial informada é inválida!");
					inicio.focus();
					return false;
				}
				
				if( !validaData(final) ){
					alert("A data final informada é inválida!");
					inicio.focus();
					return false;
				}
				
				if ( !validaDataMaior(inicio, final) ){
					alert("A data inicial informada é maior do que a final!");
					inicio.focus();
					return false;
				}
				
				document.getElementById('busca_convenio').submit();				
							
			}
		</script>
	</head>
	<body>
		<form name="formulario" id="busca_convenio" method="post" action="">
			<input type="hidden" name="pesquisa" value="1"/>
			<table align="center" border="0" cellpadding="5" cellspacing="1" class="tabela" cellpadding="0" cellspacing="0">
				<tr>
					<td  bgcolor="#CCCCCC" colspan="2"><b>Filtros da Pesquisa</b></td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Número do Convênio/Contrato de Repasse</td>
					<td>
						<?= campo_texto( 'covnumero', 'N', 'S', '', 16, 20, '', '', 'left', '', 0); ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Objeto</td>
					<td>
						<?php $covobjeto = $dados_convenio["covobjeto"]; ?>
						<?= campo_textarea( 'covobjeto', 'N', 'S', '', '50', '4', '500'); ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Início</td>
					<td>
						<?= campo_data( 'covdtinicio', 'N', 'S', '', 'S' ); ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Fim</td>
					<td>
						<?= campo_data( 'covdtfinal', 'N', 'S', '', 'S' ); ?>
					</td>
				</tr>
				<tr>
					<td bgcolor="#CCCCCC"></td>
					<td bgcolor="#CCCCCC">
						<input type="button" value="Pesquisar" style="cursor: pointer;" onclick="validaPesquisaConvenio();"/>
						<input type="submit" value="Ver Todos" style="cursor: pointer;"/>
					</td>
				</tr>
			</table>
		</form>
		<?php
			
			$sql = "SELECT DISTINCT
						'<a style=\"cursor:pointer;\" onclick=\"buscaConvenio(' || co.covid || ')\">' || co.covnumero || '</a>',
						co.covano,
						co.covobjeto,
						to_char(co.covdtinicio, 'DD/MM/YYYY') as inicio,
						to_char(co.covdtfinal, 'DD/MM/YYYY') as fim,
						CASE WHEN co2.covtipo = 'A' THEN 'Sim' ELSE 'Não' END as aditivo,
						CASE WHEN co3.covtipo = 'P' THEN 'Sim' ELSE 'Não' END as apostilamento,
						co.covvalor
					FROM
						obras.conveniosobra co
					INNER JOIN
						obras.conveniosobra co2 ON co.covnumero = co2.covnumero
					INNER JOIN
						obras.conveniosobra co3 ON co.covnumero = co3.covnumero												   
					LEFT JOIN
						entidade.endereco ed ON co.covmuncod = ed.muncod
					INNER JOIN
						obras.obrainfraestrutura oi ON ed.endid = oi.endid AND
													   oi.obrid = {$_SESSION["obra"]["obrid"]}
					WHERE
						co.covstatus = 'A' AND
						co.covtipo = 'C' AND
						co.covnumero IS NOT NULL" . $pesquisa;
			$cabecalho = array( "Número do Convênio ou Contrato de Repasse", "Ano", "Objeto", "Data de Início", "Data de Término", "Aditivo","Apostilamento", "Valor" );
			$db->monta_lista( $sql, $cabecalho, 100, 30, 'N', 'center', '' );
		?>
	</body>
</html>
