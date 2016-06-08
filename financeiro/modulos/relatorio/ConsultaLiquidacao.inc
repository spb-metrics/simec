<?php

class ConsultaLiquidacao extends ConsultaFinanceiro
{

	static protected $selecionados = array(
//		'foncod' => 'gestao_executora',
		'foncod' => 'fonte',
//		'foncod' => 'credito_recebido',
		'rofempenhado' => 'empenho_emitido',
		'rofliquidado_favorecido' => 'empenho_liquidado',
//		'foncod' => 'repasse_financeiro_recebido',
		'rofpago' => 'valores_pagos',
//		'foncod' => 'liquidado_a_pagar',
//		'foncod' => 'saldo_financeiro',
//		'foncod' => 'limite_de_saque',
//		'foncod' => 'valor_a_recompor',
//		'foncod' => 'valor_a_detalhar',
//		'foncod' => 'valor_a_desdetalhar',
//		'foncod' => 'repassado_em_excesso',
//		'foncod' => 'valor_a_repassar',
//		'foncod' => 'valor_a_repassar_proposto'
	);

	protected function pegarSelecionados( $alias )
	{
		$selecionados = array();
		foreach ( ConsultaLiquidacao::$selecionados as $campo => $apelido )
		{
			if ( $alias )
			{
				$campo .= ' as '. $apelido;
			}
			array_push( $selecionados, $campo );
		}
		return array_merge( parent::pegarSelecionados( $alias ), $selecionados );
	}

	public function pegarFixos()
	{
		return '';
	}

}

?>