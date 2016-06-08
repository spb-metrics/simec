<?php

	$sql = "SELECT
				upper(ee.entnome) as ifes,
				upper(oi.obrdesc) as nomeobra,
				tm.mundescricao || ' / ' || ed.estuf as municipio,
				CASE WHEN ee2.entnome is not null
					 THEN upper(ee2.entnome)
					 ELSE 'Não Informado'
				END as empresa,
				CASE WHEN obrdtinicio is not null
					 THEN oi.obrcustocontrato  
					 ELSE 0.00 
				END as valor,
				CASE WHEN obrdtinicio is not null
					 THEN to_char(obrdtinicio, 'DD/MM/YYYY') 
					 ELSE 'Não Informado' 
				END as inicio,
				CASE WHEN obrdttermino is not null
					 THEN to_char(obrdttermino, 'DD/MM/YYYY') 
					 ELSE 'Não Informado' 
				END as termino,
				CASE WHEN oi.stoid is not null
					 THEN so.stodesc 
					 ELSE 'Não Informado'
				END as situacao,
				CASE WHEN obrdtvistoria is not null
					 THEN to_char(obrdtvistoria, 'DD/MM/YYYY')
					 ELSE to_char(oi.obsdtinclusao, 'DD/MM/YYYY') 
				END as atualizacao,
				(SELECT 
					replace(coalesce(round(SUM(icopercexecutado), 2), '0') || ' %', '.', ',') as total 
				 FROM
				 	obras.itenscomposicaoobra WHERE obrid = oi.obrid) as percentual
			FROM
				obras.obrainfraestrutura oi
			INNER JOIN
				entidade.entidade ee ON oi.entidunidade = ee.entid
			INNER JOIN
				entidade.endereco ed ON ed.endid = oi.endid
			INNER JOIN
				territorios.municipio tm ON ed.muncod = tm.muncod
			LEFT JOIN
				entidade.entidade ee2 ON oi.entidempresaconstrutora = ee2.entid
			LEFT JOIN
				obras.situacaoobra so ON so.stoid = oi.stoid
			WHERE
				oi.obsstatus = 'A' AND
				prfid = 2 AND
				orgid = " . ORGAO_SESU . "
			ORDER BY
				ee.entnome";

?>
<html>
	<head>
		<title><?php echo $GLOBALS['parametros_sistema_tela']['nome_e_orgao']; ?></title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
	</head>
	<body>
		<center>
			<?php echo monta_cabecalho_relatorio( '95' ); ?>
		</center>
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
			<thead>
				<tr>
					<th>
						SESu - Secretaria de Educação Superior <br/>
						Obras do Programa REUNI
					</th>
				</tr>	
			</thead>
		</table>
		<?php
							$cabecalho = array('IFES', 'Nome da Obra', 'Município/UF', 'Empresa Contratada', 
											   'Valor do Contrato', 'Data Início', 'Data Término', 'Situação da Obra', 
											   'Última Atualização', '(%) Executado');
							$db->monta_lista( $sql, $cabecalho, 2000, 30, 'N', 'center', '' );
						?>		
	</body>
</html>
