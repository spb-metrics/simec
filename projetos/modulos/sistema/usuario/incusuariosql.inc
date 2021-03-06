<?php

	// exclui todas os programas e a��es propostos anteriormente
	$db->executar( "DELETE FROM monitora.progacaoproposto WHERE usucpf = '$cpf'" );

	// relaciona o usu�rio com os programas indicados por ele
	$programas = (array) $_REQUEST["proposto"]['P'];
	foreach ( $programas as $programa ) {
		if ( !$programa ) {
			continue;
		}
		$sql = sprintf(
			"insert into monitora.progacaoproposto ( usucpf, prgid ) values ( '%s', '%s' )",
			$cpf,
			$programa
		);
		$db->executar( $sql );
	}

	// relaciona o usu�rio com as a��es indicadas por ele
	$acoes = (array) $_REQUEST["proposto"]['A'];
	foreach ( $acoes as $acao ) {
		if ( !$acao ) {
			continue;
		}
		$acao = explode( '.', $acao );
		$sql = sprintf(
			"insert into monitora.progacaoproposto ( usucpf, prgid, acacod, unicod ) values ( '%s', '%s', '%s', '%s' )",
			$cpf,
			$acao[0],
			$acao[1],
			$acao[2]
		);
		$db->executar( $sql );
	}

?>