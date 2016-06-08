<?php

// registros ///////////////////////////////////////////////////////////////////

/**
 * @param array $registro
 * @param string $tabela
 * @param string $esquema
 * @return array
 */
function pde_registro_atualizar( &$registro, $tabela, $esquema = 'pde' ){
	global $db;
	if ( !pde_tabela_existe( $tabela, $esquema ) ) {
		return null;
	}
	// monta atribuiчуo
	$atribuicao = array();
	foreach ( $registro as $campo => $valor ) {
		if ( empty( $valor ) ) {
			continue;
		}
		array_push( $atribuicao, sprintf( " %s = '%s' ", $campo, $valor ) );
	}
	$atribuicao = implode( ',', $atribuicao );
	// monta condiчуo
	$condicao = array();
	foreach ( pde_tabela_pegar_chaves( $tabela ) as $chave ) {
		if ( empty( $registro[$chave] ) ) {
			continue;
		}
		array_push( $condicao, sprintf( " %s = '%s' ", $chave, $registro[$chave] ) );
	}
	$condicao = implode( ',', $condicao );
	// executa
	$sql = sprintf(
		"update %s.%s set %s where %s",
		$esquema,
		$tabela,
		$atribuicao,
		$condicao
	);
	if ( !$db->executar( $sql ) ) {
		return null;
	}
	return $registro;
}

/**
 * @param array $registro
 * @param string $tabela
 * @param string $esquema
 * @return boolean
 */
function pde_registro_existe( $registro, $tabela, $esquema = 'pde' ){
	global $db;
	if ( !pde_tabela_existe( $tabela, $esquema ) ) {
		return null;
	}
	// monta condiчуo
	$condicao = array();
	foreach ( pde_tabela_pegar_chaves( $tabela ) as $chave ) {
		if ( empty( $registro[$chave] ) ) {
			return false;
		}
		array_push( $condicao, sprintf( " %s = '%s' ", $chave, $registro[$chave] ) );
	}
	$condicao = implode( ',', $condicao );
	// excecuta
	$sql = sprintf(
		"select count(*) from %s.%s where %s",
		$esquema,
		$tabela,
		$condicao
	);
	return $db->pegaUm( $sql ) == 1;
}

/**
 * @param array $registro
 * @param string $tabela
 * @param string $esquema
 * @return array
 */
function pde_registro_inserir( &$registro, $tabela, $esquema = 'pde' ){
	global $db;
	if ( !pde_tabela_existe( $tabela, $esquema ) ) {
		return null;
	}
	// executa
	$sql = sprintf(
		"insert into %s.%s ( %s ) values ( '%s' )",
		$esquema,
		$tabela,
		implode( ",", array_keys( (array) $registro ) ),
		implode( "','", $registro )
	);
	$resultado = $db->executar( $sql );
	if ( !$resultado ) {
		return null;
	}
	// captura as chaves primсrias
	$sql = sprintf(
		"select %s from %s.%s where oid = '%s' limit 1",
		implode( ",", pde_tabela_pegar_chaves( $tabela, $esquema ) ),
		$esquema,
		$tabela,
		pg_last_oid( $resultado )
	);
	foreach( $db->pegaLinha( $sql ) as $chave => $valor ){
		$registro[$chave] = $valor; # atualiza o registro
	}
	return $registro;
}

/**
 * @param array $registro
 * @param string $tabela
 * @param string $esquema
 * @return array
 */
function pde_registro_pegar( &$registro, $tabela, $esquema = 'pde' ){
}

/**
 * @param array $registro
 * @param string $tabela
 * @param string $esquema
 * @return array
 */
function pde_registro_salvar( &$registro, $tabela, $esquema = 'pde' ){
	if ( pde_registro_existe( $registro, $tabela, $esquema ) ) {
		return pde_registro_atualizar( $registro, $tabela, $esquema );
	} else {
		return pde_registro_inserir( $registro, $tabela, $esquema );
	}
}

// tabelas /////////////////////////////////////////////////////////////////////

/**
 * @param string $tabela
 * @param string $esquema
 * @return boolean
 */
function pde_tabela_existe( $tabela, $esquema = 'pde' ){
	global $db;
	static $tabelas = array();
	if ( !array_key_exists( $tabela, $tabelas ) ) {
		$sql = sprintf(
			"select count(*) from information_schema.tables t where t.table_name = '%s' and t.table_schema = '%s'",
			$tabela,
			$esquema
		);
		$tabelas[$tabela] = $db->pegaUm( $sql ) == 1;
	}
	return $tabelas[$tabela];
}

/**
 * @param string $tabela
 * @param string $esquema
 * @return array
 */
function pde_tabela_pegar_chaves( $tabela, $esquema = 'pde' ){
	global $db;
	static $chaves = array();
	if ( !isset( $chaves[$esquema][$tabela] ) ) {
		$sql = sprintf(
			"select k.column_name from information_schema.table_constraints t
			inner join information_schema.key_column_usage k on t.constraint_name = k.constraint_name  
			where t.constraint_type = 'PRIMARY KEY' and t.table_name = '%s' and t.table_schema = '%s'
			order by k.ordinal_position",
			$tabela,
			$esquema
		);
		$chaves[$esquema][$tabela] = array();
		foreach ( $db->carregar( $sql ) as $registro ) {
			array_push( $chaves[$esquema][$tabela], $registro['column_name'] );
		}
	}
	return $chaves[$esquema][$tabela];
}

?>