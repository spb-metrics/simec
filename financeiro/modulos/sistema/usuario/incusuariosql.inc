<?php

	// exclui todas as unidades propostas anteriormente
	$db->executar( "DELETE FROM financeiro.unidproposto WHERE usucpf = '$cpf'" );

	// relaciona o usuário com os programas indicados por ele
	$unidades = (array) $_REQUEST["proposto"]['U'];
	foreach ( $unidades as $unidade ) {
		if ( !$unidade ) {
			continue;
		}
		$sql = sprintf(
			"INSERT INTO financeiro.unidproposto ( usucpf, unicod ) VALUES ( '%s', '%s' )",
			$cpf,
			$unidade
		);
		$db->executar( $sql );
	}
	
?>