<?php
	
class GrupoDistribuicao extends Modelo{
	
//	const ROTAAPROVADA		= 1;
//	const TIPO_DESLOCAMENTO = array(1 => "Transporte Rodoviário - Trajeto Único", 
//									2 => "Transporte Rodoviário - Trajeto por Roteiro",
//									3 => "Transporte não Rodoviário - Trajeto Alternativo");
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras.grupodistribuicao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "gpdid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'gpdid' => null, 
									  	'estuf' => null, 
									  	'usucpf' => null, 
									  	'docid' => null, 
									  	'epcid' => null, 
									  	'gpddtcriacao' => null, 
									  	'gpdstatus' => null, 
									  );

	function buscaEmpresa( Array $param = null ){
		$where  = array();
		$join   = array();
		
		$coluna = $param['coluna'] ? $param['coluna'] : "*";  
		 
		if ( $param['vinculoGrupo'] ){
			$join[] = "JOIN obras.grupodistribuicao og ON og.epcid = oe.epcid";
		}
		
		$sql = "SELECT DISTINCT
					{$coluna}
				FROM
					obras.empresacontratada oe
				JOIN entidade.entidade ee ON oe.entid = ee.entid
				" . (count($join) ? implode(" ", $join) : "") . "
				ORDER BY
					entnome;";
		
		return $this->carregar( $sql );
	}								  
									  
	function dadosSelecaoGrupoOS($coluna = null, Array $arWhere = null){
		$coluna = implode(', ', (is_null($coluna) ? ((array) '*') : ((array) $coluna)) );
		$onWhereOS = $arWhere['gpdidEdicao'] ? "OR (gp.gpdid = " . $arWhere['gpdidEdicao'] . ")" : ''; 
		
		if ($arWhere['gpdidEdicao']){
			$coluna .= ", CASE 
							WHEN {$arWhere['gpdidEdicao']} = gp.gpdid THEN 'checked=\"checked\"'
							ELSE ''
						  END AS checked"; 
		}
		
		
		$sql = "SELECT
				    {$coluna} 				
				FROM
					obras.grupodistribuicao gp
				JOIN obras.empresacontratada ec ON ec.epcid = gp.epcid
				JOIN entidade.entidade e ON e.entid = ec.entid	
				JOIN entidade.endereco ed ON e.entid = ed.entid
				JOIN obras.rotas rt ON rt.gpdid = gp.gpdid AND strid = " . Rotas::ROTAAPROVADA . "
				WHERE
					(gp.gpdid NOT IN (SELECT gpdid FROM obras.ordemservico WHERE orsstatus='A' AND stoid != " . OrdemServico::SITUACAOOS_CANCELADA . ") {$onWhereOS})";
				    
		return $this->carregar( $sql );				    
	}
	
	function listaProcedimentoValor( $obrid ){
		/*
		 * 31/06/2010
		 * A pedido do Mário, estou alterando o valor de "vhtvlrhora" para "vhtmaxima".
		 * A pedido do Mário, quando "obrqtdconstruida" > 4500 e "tppsigla" = 'RMO' multiplica por 2.
		 */
		$sql = "SELECT
					tp.tppsigla,
					v.vhtmaxima,
					CASE WHEN TRIM(tp.tppsigla) = 'RMO'  
						THEN 
							CASE WHEN o.obrqtdconstruida > 4500
								THEN
									ROUND(v.vhtmaxima * tp.tppvlrhrtecnica * 2, 2)
								ELSE
									ROUND(v.vhtmaxima * tp.tppvlrhrtecnica, 2)
							END 
						ELSE ROUND(v.vhtmaxima * tp.tppvlrhrtecnica, 2)
					END as total
				FROM
					obras.obrainfraestrutura o
				JOIN entidade.endereco e ON e.endid = o.endid	
				JOIN obras.valorhoratecnica v ON v.estuf = e.estuf
				JOIN obras.repositorio r ON r.obrid = o.obrid
				JOIN obras.itemgrupo i ON i.repid = r.repid
				JOIN obras.procedimentotecnico pt ON pt.itgid = i.itgid
				JOIN obras.tipoprocedimento tp ON tp.tppid = pt.tppid
				WHERE
					/*r.repstatus = 'A'
					AND*/ o.obrid = {$obrid}";
		
		return $this->carregar( $sql );
	}
	
	function pegaTotalValorProcedimento( $obrid, Array $param = null ){
		if ( $obrid ){
			$where = array();
			$obrid = (array) $obrid;
			
			if ($param['orgid']){
				$where[] = "o.orgid = {$param['orgid']}";
			}
			
			$sql = "SELECT
						ROUND(SUM( f.total ), 2)
					FROM (
						SELECT
									CASE WHEN TRIM(tp.tppsigla) = 'RMO'  
										THEN 
											CASE WHEN o.obrqtdconstruida > 4500
												THEN
													ROUND(SUM(v.vhtmaxima * tp.tppvlrhrtecnica * 2), 2)
												ELSE
													ROUND(SUM(v.vhtmaxima * tp.tppvlrhrtecnica), 2)
											END 
										ELSE ROUND(SUM(v.vhtmaxima * tp.tppvlrhrtecnica), 2)
									END as total			
								FROM
									obras.obrainfraestrutura o
								JOIN entidade.endereco e ON e.endid = o.endid	
								JOIN obras.valorhoratecnica v ON v.estuf = e.estuf
								JOIN obras.repositorio r ON r.obrid = o.obrid
								JOIN obras.itemgrupo i ON i.repid = r.repid
								JOIN obras.procedimentotecnico pt ON pt.itgid = i.itgid
								JOIN obras.tipoprocedimento tp ON tp.tppid = pt.tppid
								WHERE
									/*r.repstatus = 'A'
									AND*/ o.obrid IN (" . implode(',', $obrid) . ")
									" . (count($where) ? " AND " . implode(' AND ', $where) : "") . "
								GROUP BY
									tp.tppsigla,
									o.obrid,
									o.obrqtdconstruida
							) AS f";
			
			$dado = $this->pegaUm( $sql );
		}
		return $dado;
	}
	
}