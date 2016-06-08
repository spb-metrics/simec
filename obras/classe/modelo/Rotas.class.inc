<?php
	
class Rotas extends Modelo{
	const ROTAAPROVADA				= 1;
	
	const TRANS_ROD_TRAJUNICO		= 1;	
	const TRANS_ROD_TRAJROTEIRO		= 2;	
	const TRANS_NROD_TRAJALTERNATIO	= 3;		
	
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras.rotas";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "rotid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'rotid' => null, 
									  	'gpdid' => null, 
									  	'strid' => null, 
									  	'usucpf' => null, 
									  	'prpid' => null, 
									  	'rotnumero' => null, 
									  	'rotdtinclusao' => null, 
									  	'rotstatus' => null, 
									  	'rotkmdistanciatotal' => null, 
									  	'rottotaltempo' => null, 
									  	'rtovlrpedagio' => null, 
									  	'rotobs' => null, 
									  );

	function buscaRotaAprovada($gpdid, $coluna = null){
		$coluna = implode(', ', (is_null($coluna) ? ((array) '*') : ((array) $coluna)) );
		
		$sql = "SELECT
					{$coluna}
				FROM
					obras.rotas
				WHERE
					strid = " . self::ROTAAPROVADA;

		return $this->pegaLinha( $sql );			
	}

	function dadosRota(Array $arParam){		
		$gpdid  = $arParam['gpdid'];
		$strid  = $arParam['strid'] ? $arParam['strid'] : self::ROTAAPROVADA;
		$coluna = is_array( $arParam['addColuna'] ) ? implode(', ', $arParam['addColuna']) : $arParam['addColuna'];
		$coluna = ($coluna != "") ? ', ' . $arParam['addColuna'] : $arParam['addColuna'];
		
		$colunaOrder = is_array( $arParam['addColunaOrder'] ) ? implode(', ', $arParam['addColunaOrder']) : $arParam['addColunaOrder'];
		$colunaOrder = ($colunaOrder != "") ? ', ' . $arParam['addColunaOrder'] : $arParam['addColunaOrder'];
		
		if ($gpdid):
			/*$sql = "(
						SELECT
							trjseq,
							'(' || oi.obrid || ') ' || upper(obrdesc) as obrdesc,
							mun.mundescricao,
							td.tdedsc as deslocamento,
							ctjvalor,
							trjkm as km,
							trjvlrpedagio as pedagio,
							trjtempo as tempo
							{$coluna}
						FROM 
							obras.obrainfraestrutura oi
						INNER JOIN
							entidade.endereco ed ON oi.endid = ed.endid
						INNER JOIN
							territorios.municipio mun ON mun.muncod = ed.muncod
						INNER JOIN
							obras.repositorio ore ON ore.obrid = oi.obrid
										 			 AND ore.repstatus = 'A'
						INNER JOIN
							obras.itemgrupo it ON it.repid = ore.repid
						INNER JOIN
							obras.trajetoria ot ON ot.itgid = it.itgid
									       		   AND ot.trjstatus = 'A' 
						INNER JOIN 
							obras.tipodeslocamento td ON td.tdeid = ot.tdeid			       
						INNER JOIN 
							obras.rotas r ON r.rotid = ot.rotid
									 		 AND r.rotstatus = 'A'
									 		 AND strid = {$strid}
						LEFT JOIN 
							obras.composicaotrajetoria ct ON ot.trjid = ct.trjid						
						WHERE 
							it.gpdid = $gpdid
						ORDER BY
							      trjseq
					)UNION ALL(
						SELECT
							trjseq,
							upper(entnome) as obrdesc,
							mun.mundescricao,
							td.tdedsc as deslocamento,
							ctjvalor,
							trjkm as km,
							trjvlrpedagio as pedagio,
							trjtempo as tempo
							{$coluna}
						FROM
							entidade.entidade ee
						INNER JOIN
							obras.empresacontratada ec ON ec.entid = ee.entid						
						INNER JOIN
							obras.empresaufatuacao oe ON oe.epcid = ec.epcid
						INNER JOIN
							territorios.municipio mun ON mun.muncod = oe.muncod
						INNER JOIN
							obras.grupodistribuicao og ON og.epcid = ec.epcid
														  AND og.estuf = oe.estuf
						INNER JOIN
							obras.trajetoria ot ON ot.epcid = og.epcid
									       		   AND ot.trjstatus = 'A'	
						INNER JOIN 
							obras.tipodeslocamento td ON td.tdeid = ot.tdeid			       
						INNER JOIN 
							obras.rotas r ON r.rotid = ot.rotid
											 AND r.rotstatus = 'A'
											 AND r.gpdid = og.gpdid
											 AND strid = {$strid}
						LEFT JOIN 
							obras.composicaotrajetoria ct ON ot.trjid = ct.trjid
						WHERE
							og.gpdid = {$gpdid} 
						ORDER BY
							      trjseq	
					)";*/
		
			$sql = "select  trjseq,
                            obrdesc,
                            mundescricao,
                            deslocamento,
                            sum(ctjvalor) as ctjvalor,
                            km,
                            pedagio,
                            tempo
                            $colunaOrder
					from
					(
					(SELECT
                            trjseq,
                            '(' || oi.obrid || ') ' || upper(obrdesc) as obrdesc,
                            mun.mundescricao,
                            td.tdedsc as deslocamento,
                            ct.ctjvalor,
                            trjkm as km,
                            trjvlrpedagio as pedagio,
                            trjtempo as tempo
                            $coluna
                        FROM
                            obras.obrainfraestrutura oi
                        INNER JOIN
                            entidade.endereco ed ON oi.endid = ed.endid
                        INNER JOIN
                            territorios.municipio mun ON mun.muncod = ed.muncod
                        INNER JOIN
                            obras.repositorio ore ON ore.obrid = oi.obrid --AND ore.repstatus = 'A'
                        INNER JOIN
                            obras.itemgrupo it ON it.repid = ore.repid
                        INNER JOIN
                            obras.trajetoria ot ON ot.itgid = it.itgid AND ot.trjstatus = 'A'
                        INNER JOIN
                            obras.tipodeslocamento td ON td.tdeid = ot.tdeid
                        INNER JOIN
                            obras.rotas r ON r.rotid = ot.rotid AND r.rotstatus = 'A' AND strid = {$strid}
                        LEFT JOIN
                            obras.composicaotrajetoria ct ON ot.trjid = ct.trjid
                        WHERE
                            it.gpdid = {$gpdid}
                        ORDER BY
                                  trjseq
                    )UNION ALL(
                        SELECT
                            trjseq,
                            upper(entnome) as obrdesc,
                            mun.mundescricao,
                            td.tdedsc as deslocamento,
                            ctjvalor,
                            trjkm as km,
                            trjvlrpedagio as pedagio,
                            trjtempo as tempo
                            $coluna
                        FROM
                            entidade.entidade ee
                        INNER JOIN
                            obras.empresacontratada ec ON ec.entid = ee.entid
                        INNER JOIN
                            obras.empresaufatuacao oe ON oe.epcid = ec.epcid
                        INNER JOIN
                            territorios.municipio mun ON mun.muncod = oe.muncod
                        INNER JOIN
                            obras.grupodistribuicao og ON og.epcid = ec.epcid AND og.estuf = oe.estuf
                        INNER JOIN
                            obras.trajetoria ot ON ot.epcid = og.epcid AND ot.trjstatus = 'A'
                        INNER JOIN
                            obras.tipodeslocamento td ON td.tdeid = ot.tdeid
                        INNER JOIN
                            obras.rotas r ON r.rotid = ot.rotid
                                             AND r.rotstatus = 'A'
                                             AND r.gpdid = og.gpdid
                                             AND strid = {$strid}
                        LEFT JOIN
                            obras.composicaotrajetoria ct ON ot.trjid = ct.trjid
                        WHERE
                            og.gpdid = {$gpdid}
                        ORDER BY
                               trjseq
				) ) as foo
				group by
				trjseq,
				obrdesc,
                mundescricao,
                deslocamento,
                km,
                pedagio,
                tempo 
                $colunaOrder
                order by trjseq
                ";
		
			$arDados = $this->carregar( $sql );
		else:
			$arDados = false;
		endif;
		/*
		 * INÍCIO - ADD primeira linha
		 * 
		 * Essa primeira linha e adicionada na mão, conforme conversado com o Fernando ele também faz assim nas rotas, com
		 * objetivo de replicar o ponto de chegada que é a própria empresa, para o início, pois a empresa também é o ponto
		 * de partida.
		 */
		if ( is_array($arDados) ){
			//ADD Primeira linha
			end($arDados);
			$ultArDados = current($arDados);
			$arAdd = array();
			foreach ($ultArDados as $indice => $valor){
				switch ($indice){
					case 'trjseq':
						$arAdd[$indice] = 1;
					continue;
					case 'obrdesc':
						$arAdd[$indice] = $valor;
					continue;
					case 'mundescricao':
						$arAdd[$indice] = $valor;
					continue;
					default:
						$arAdd[$indice] = '-';
					continue;
				}
			}
			
			reset($arDados);
			array_unshift($arDados, $arAdd);
		}
		/*
		 * FIM - ADD primeira linha
		 */
		
		return $arDados;
	}		

	function dadosRotaCalculado( Array $arParam ){
		$arParam['addColuna'] = 'ot.trjid, td.tdeid, mun.muncod, mun.estuf';
		$arParam['addColunaOrder'] = 'trjid, tdeid, muncod, estuf';
		$arDado = $this->dadosRota( $arParam );
		
		if ( is_array($arDado) ):
			for($i=0; $i < count($arDado); $i++):
				$arDado[$i]['desctrajetoalternativo'] = '';
				switch ( $arDado[$i]['tdeid'] ){
					case self::TRANS_ROD_TRAJUNICO:
						$arDado[$i]['calculo'] = self::calculoRodoviarioTrajetoUnico( $arDado[$i] );
					break;
					case self::TRANS_ROD_TRAJROTEIRO:
						$arDado[$i]['calculo'] = self::calculoRodoviarioTrajetoRoteiro( $arDado[$i] );
					break;	
					case self::TRANS_NROD_TRAJALTERNATIO:
						$dadoTrajeto = self::calculoNRodoviarioTrajetoAlternativo( $arDado[$i] );
						
						$arDado[$i]['desctrajetoalternativo'] = implode('<br>', (array) $dadoTrajeto['trajeto']);
						$arDado[$i]['calculo'] = $dadoTrajeto['calculo'];
					break;	
					default:
						$arDado[$i]['calculo'] = '-';
					break;
				}
				unset($arDado[$i]['km'], $arDado[$i]['pedagio'], $arDado[$i]['tempo'], 
					  $arDado[$i]['tdeid'], $arDado[$i]['muncod'], $arDado[$i]['estuf'], $arDado[$i]['trjid']);				
			endfor;
		endif;
		return $arDado;
	}
	
	function pegaTotalRemuneracaoRota( Array $arParam ){
		$arParam['addColuna'] = 'ot.trjid, td.tdeid, mun.muncod, mun.estuf';
		$arParam['addColunaOrder'] = 'trjid, tdeid, muncod, estuf';
		$arDado = $this->dadosRota( $arParam );
		
		$total = 0;
		if ( is_array($arDado) ):
			for($i=0; $i < count($arDado); $i++):
				switch ( $arDado[$i]['tdeid'] ){
					case self::TRANS_ROD_TRAJUNICO:
						$total += self::calculoRodoviarioTrajetoUnico( $arDado[$i] );
					break;
					case self::TRANS_ROD_TRAJROTEIRO:
						$total += self::calculoRodoviarioTrajetoRoteiro( $arDado[$i] );
					break;	
					case self::TRANS_NROD_TRAJALTERNATIO:
						$dadoTrajeto = self::calculoNRodoviarioTrajetoAlternativo( $arDado[$i] );
						
						$total += $dadoTrajeto['calculo'];
					break;	
				}
			endfor;
		endif;
		return $total;
	}
	
	function pegaTotalKmRota(Array $arParam){
		$gpdid  = $arParam['gpdid'];
		$strid  = $arParam['strid'] ? $arParam['strid'] : self::ROTAAPROVADA;

		if ($arParam['tdeid']){
			$whereTrajetoria = "AND t.tdeid = " . $arParam['tdeid'];
		}
		
		$dado = false;
		if ($gpdid){
			$sql = "SELECT 
						SUM(trjkm)
					FROM
						obras.rotas r
					JOIN obras.trajetoria t ON t.rotid = r.rotid
								   AND t.trjstatus = 'A'
								   {$whereTrajetoria}	 		
					WHERE
						gpdid = $gpdid
						AND strid = $strid";
			
			$dado = $this->pegaUm($sql);
		}
		return $dado;
	}
	
	function pegaTotalTrajetos( Array $arParam ){
		$arParam['addColuna'] = 'ot.trjid, td.tdeid, mun.muncod, mun.estuf';
		$arParam['addColunaOrder'] = 'trjid, tdeid, muncod, estuf';
		$arDado = $this->dadosRota( $arParam );
		
		$arTotaisTrajeto = array();
		if ( is_array($arDado) ):
			for($i=0; $i < count($arDado); $i++):
				switch ( $arDado[$i]['tdeid'] ){
					case self::TRANS_ROD_TRAJUNICO:
						$arTotaisTrajeto['trajetoUnico'] += self::calculoRodoviarioTrajetoUnico( $arDado[$i] );
					break;
					case self::TRANS_ROD_TRAJROTEIRO:
						$arDados = self::calculoRodoviarioTrajetoRoteiro( $arDado[$i] );
						//ver($arDados,d);
						$arTotaisTrajeto['trajetoRoteiro']['valor'] += $arDados['valDesloc'];
						$arTotaisTrajeto['trajetoRoteiro']['prcCombustivel'] = $arDados['prcCombustivel'];
						$arTotaisTrajeto['trajetoRoteiro']['vlrHoraTecnica'] = $arDados['vlrHoraTecnica'];
						$arTotaisTrajeto['trajetoRoteiro']['pedagio'] += $arDados['pedagio'];
					break;	
					case self::TRANS_NROD_TRAJALTERNATIO:
						$dadoTrajeto = self::calculoNRodoviarioTrajetoAlternativo( $arDado[$i] );
						$arTotaisTrajeto['trajetoAlternativo'] += $dadoTrajeto['calculo'];
					break;	
				}
			endfor;
		endif;
		
		if($arTotaisTrajeto['trajetoRoteiro']){
			$arTotaisTrajeto['semPedagio'] = $arParam['semPedagio'] ? $arParam['semPedagio'] : false;
			$arTotaisTrajeto['trajetoRoteiro']['valor'] = self::calculoRodoviarioTrajetoRoteiroTotal( $arTotaisTrajeto );
		}
		
		$arTotaisTrajeto['valorTotal'] = $arTotaisTrajeto['trajetoAlternativo'] + $arTotaisTrajeto['trajetoUnico'] + $arTotaisTrajeto['trajetoRoteiro']['valor'];
		
		return $arTotaisTrajeto;
	}
				
	function buscaVlrHoraTecnica( $estuf ){		
		$sql = "SELECT
					vhtmaxima
				FROM
					obras.valorhoratecnica	
				WHERE
					estuf = '{$estuf}'";
		
		return $this->pegaUm( $sql );			
	}
	
	function buscaDetalheTrajetoAlternativo( $trjid ){
		$sql = "SELECT
					ctjdsc,
					ctjvalor
				FROM 
					obras.composicaotrajetoria
				WHERE
					trjid = {$trjid}";
		return $this->carregar( $sql );
	}
	
	private function calculoRodoviarioTrajetoUnico( Array $arDado ){
		if ( $arDado['muncod'] ){
			$obModelMun = new Municipio();
			$populacao  = $obModelMun->buscaPopulacao( array("muncod" => $arDado['muncod']) );
		}
		
		if ( $arDado['estuf'] ){
			$obModelComb = new Combustivel();
			$prcCombustivel  = $obModelComb->buscaPrecoCombustivelPorUF( $arDado['estuf'] );
			$vlrHoraTecnica  = $this->buscaVlrHoraTecnica( $arDado['estuf'] );
//			$obModelRot 	 = new Rotas();
//			$prcCombustivel  = $obModelRot->buscaPrecoCombustivel( $arDado['estuf'] );
//			$vlrHoraTecnica  = $obModelRot->buscaVlrHoraTecnica( $arDado['estuf'] );
		}
		
		$valDesloc = 0;
		if ( $arDado['km'] > 30 ):
			$valDesloc = (0.55 * $prcCombustivel * $arDado['km'] + $arDado['pedagio']);
			/*
			 * Se deslocamento superior a 200Km
			 * será acrescido das horas despendidas no percurso de viagem.
			 */
			if ( $arDado['km'] > 200 ):
				$valDesloc += ( ($arDado['km'] / 80) * 0.50 * $vlrHoraTecnica );
			endif;			
		elseif ( $arDado['km'] <= 30 ):
			if($populacao <= 50000){
				$valDesloc = ( 3 * $prcCombustivel );	
			}elseif($populacao <= 200000){
				$valDesloc = ( 6 * $prcCombustivel );	
			}elseif($populacao <= 500000){
				$valDesloc = ( 8 * $prcCombustivel );	
			}elseif($populacao <= 1000000){
				$valDesloc = ( 10 * $prcCombustivel );	
			}elseif($populacao > 1000000){
				$valDesloc = ( 13 * $prcCombustivel );	
			}
		endif;
		
		return $valDesloc;
//		return number_format($valDesloc, 2, ',', '.');
	}
	
	/**
	 * Código antigo...
	 */
	/*private function calculoRodoviarioTrajetoRoteiro( Array $arDado ){
		if ( $arDado['muncod'] ){
			$obModelMun = new Municipio();
			$populacao  = $obModelMun->buscaPopulacao( array("muncod" => $arDado['muncod']) );
		}
		
		if ( $arDado['estuf'] ){
			$obModelComb = new Combustivel();
			$prcCombustivel  = $obModelComb->buscaPrecoCombustivelPorUF( $arDado['estuf'] );
			$vlrHoraTecnica  = $this->buscaVlrHoraTecnica( $arDado['estuf'] );
//			$obModelRot 	 = new Rotas();
//			$prcCombustivel  = $obModelRot->buscaPrecoCombustivel( $arDado['estuf'] );
//			$vlrHoraTecnica  = $obModelRot->buscaVlrHoraTecnica( $arDado['estuf'] );
		}
		
		
		$valDesloc = 0;
		$valDesloc = (0.55 * $prcCombustivel * $arDado['km'] + $arDado['pedagio']);

		/*
		 * Se deslocamento superior a 200Km
		 * será acrescido das horas despendidas no percurso de viagem.
		 */
		/*if ( $arDado['km'] > 200 ){
			$valDesloc += ( ($arDado['km'] / 80) * 0.50 * $vlrHoraTecnica );
		}*/
		
		//return $valDesloc;
//		return number_format($valDesloc, 2, ',', '.');
	//}
	
	private function calculoRodoviarioTrajetoRoteiro( Array $arDado ){
		$arDados = array();
		
		if ( $arDado['estuf'] ){
			$obModelComb = new Combustivel();
			$prcCombustivel  = $obModelComb->buscaPrecoCombustivelPorUF( $arDado['estuf'] );
			$vlrHoraTecnica  = $this->buscaVlrHoraTecnica( $arDado['estuf'] );
		}
		
		$arDados['valDesloc'] = $arDado['km']; 
		$arDados['prcCombustivel'] = $prcCombustivel; 
		$arDados['vlrHoraTecnica'] = $vlrHoraTecnica; 
		$arDados['pedagio'] = $arDado['pedagio']; 

		return $arDados;
	}
	
	private function calculoRodoviarioTrajetoRoteiroTotal( Array $arDado ){

		//ver('0.55 * '.$arDado['trajetoRoteiro']['prcCombustivel'].' * '.$arDado['trajetoRoteiro']['valor'] .' + '.$arDado['trajetoRoteiro']['pedagio']);
		
		$valDesloc = (0.55 * $arDado['trajetoRoteiro']['prcCombustivel'] * $arDado['trajetoRoteiro']['valor']) + ($arDado['semPedagio'] == true ? 0 : $arDado['trajetoRoteiro']['pedagio']);
		
		if ( $arDado['trajetoRoteiro']['valor'] > 200 ){
			$valDesloc += ( ($arDado['trajetoRoteiro']['valor'] / 80) * 0.50 * $arDado['trajetoRoteiro']['vlrHoraTecnica'] );
		}
			
		return $valDesloc;
	}
	
	
	private function calculoNRodoviarioTrajetoAlternativo( Array $arDado ){
		if ( $arDado['trjid'] ):
			$dadosTrajeto = $this->buscaDetalheTrajetoAlternativo( $arDado['trjid'] );
			if ( $dadosTrajeto[0] ){
				foreach($dadosTrajeto as $traj){
					$arRetorno['calculo']   += $traj['ctjvalor']; 					
					$arRetorno['trajeto'][] = '* ' . $traj['ctjdsc'] . ' - R$' .  number_format($traj['ctjvalor'], 2, ',', '.'); 				 					
				}
			}
		endif;
		
		return $arRetorno;	
	}
}