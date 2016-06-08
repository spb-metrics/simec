<?php

$estadoGrupo = Array(OBREMSUPERVISAO=>240,
					 OBREMAVALIASUPERVMEC=>241,
					 OBRENVREAVALSUPMEC=>242,
					 OBRREAVSUPVISAO=>243,
					 OBRREAJSUPVISAOEMP=>279);
$acaoEstado  = Array(241=>711,
					 242=>712,
					 243=>713,
					 279=>714,
					 240=>715);
$erro = false;
$y=0;
foreach( $estadoGrupo as $grupo => $obr )
{
	echo "<br />Grupo {$grupo} <br />";
	// Pegando o gpdid e o obrid dos grupos que estejam Em Supervisão, ou seja, esdid = 159
	$sql = "SELECT DISTINCT
				ore.obrid,
				hwd.htddata
			FROM
				obras.itemgrupo ig
			INNER JOIN
				obras.repositorio ore ON ore.repid = ig.repid
			INNER JOIN
				obras.obrainfraestrutura oi ON oi.obrid = ore.obrid
			INNER JOIN
				obras.grupodistribuicao ogd ON ogd.gpdid = ig.gpdid
			INNER JOIN
				workflow.documento wed ON wed.docid = ogd.docid 
			INNER JOIN
				workflow.estadodocumento we ON we.esdid = wed.esdid
			INNER JOIN
				(SELECT DISTINCT max(htddata) as htddata, docid FROM workflow.historicodocumento GROUP BY docid ) hwd ON hwd.docid = ogd.docid
			WHERE
				wed.esdid = {$grupo} AND
				we.esdstatus = 'A' AND
				ogd.gpdstatus = 'A' AND
				repstatus = 'A';";
//	ver($sql, d);
	$obras = $db->carregar($sql);
	$x = 1;
	if( is_array($obras) )
	{
		$y = 0;
		$sql = '';
		foreach ($obras as $obra) 
		{
			$y++;
			// recupera o tipo do documento
			$tpdid = OBR_TIPO_DOCUMENTO_OBRA;
			
			// descrição do documento
			$docdsc = "Fluxo da Obra - n°" . $obra['obrid'];
			
			// cria documento do WORKFLOW
			$docid = wf_cadastrarDocumento( $tpdid, $docdsc );
			
			// atualiza o grupo de supervisão
			$sql .= "UPDATE
						obras.obrainfraestrutura
					SET 
						docid = {$docid} 
					WHERE
						obrid = {$obra['obrid']};";
	
//			$db->executar( $sql );
			
			echo "Obra {$obra['obrid']} com docid {$docid}.<br>";
			$sql .= "UPDATE workflow.documento
					SET
						esdid = {$obr}
					WHERE
						docid = {$docid};";
//			$db->executar($sql);
			
			$sql .= "INSERT INTO workflow.historicodocumento
					(
						aedid,
						docid,
						htddata
					)
					VALUES 
					(
						{$acaoEstado[$obr]},
						{$docid},
						'{$obra['htddata']}'
					);";
		}
		if(!$db->executar($sql))
		{
			$erro = true;
		}
		$x++;
		echo $y."obras.";
		
	}
}
if(!$erro){
	$db->commit();
}

?>