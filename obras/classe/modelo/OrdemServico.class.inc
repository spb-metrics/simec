<?php
	
class OrdemServico extends Modelo{
	
	const SITUACAOOS_GERADA    = 1;
	const SITUACAOOS_CANCELADA = 3;
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras.ordemservico";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "orsid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'orsid' => null, 
									  	'gpdid' => null, 
									  	'stoid' => null, 
									  	'arqid' => null, 
									  	'orsdtemissao' => null, 
									  	'orstotalhoratecserv' => null, 
									  	'orsremdeslocamento' => null, 
									  	'orsvlrremhoratecnica' => null, 
									  	'orsvlrremdeslocamento' => null, 
									  	'orsvlrestimadoos' => null, 
									  	'orsnumprotec' => null, 
									  	'orsdtinicioexecucao' => null, 
									  	'orsdtfinalexecucao' => null, 
									  	'orsmemoriacalculo' => null, 
									  	'orsdtreuniaonucleo' => null, 
									  	'orsnumreuniaonucleo' => null, 
									  	'orsobs' => null, 
									  	'orsnotaempeds' => null, 
									  	'orsnotaempedp' => null, 
									  	'orsnotaempedb' => null, 
									  	'orsvalor' => null, 
									  	'orsstatus' => null, 
									  	'usucpf' => null, 
									  	'orsidderivada' => null 
									  );
	
	function buscaSituacao( Array $param = null){
		
		$coluna = $param['coluna'] ? $param['coluna'] : "*";  
		
		$sql = "SELECT 
					{$coluna}
 				FROM 
 					obras.situacaoos;";
		
		return $this->carregar( $sql );
	}									
	
	function contaOsGrupoAtivo( $gpdid, $orsid ){
		
		$sql = "SELECT
					count(orsid) AS total
				FROM
					$this->stNomeTabela	
				WHERE
					stoid = " . self::SITUACAOOS_GERADA . "
					AND orsid != {$orsid}
					AND gpdid = {$gpdid}";
					
		return $this->pegaUm( $sql );			
	}

	function listaOS( Array $arParam = null, $retorno = 'array' ){
		/*
		 * COLUNAS - INÍCIO
		 */
		$coluna = ($arParam['coluna'] ? implode(",", (array) $arParam['coluna']) : "*");
		/*
		 * COLUNAS - FIM
		 */
		/*
		 * Cláusulas de parametro - INÍCIO
		 */
		$whereP = $arParam['filtro'];
		// Filtra padrão de status = ativo
		$arWhere[] = "os.orsstatus = 'A'";		
		// Filtra pelo ID da OS.
		if ( is_numeric($whereP['orsid']) ){
			$arWhere[] = "os.orsid = {$whereP['orsid']}";
		}
		// Filtra pelo ID do GRUPODISTRIBUICAO.
		if ( is_numeric($whereP['gpdid']) ){
			$arWhere[] = "gd.gpdid = {$whereP['gpdid']}";
		}
		// Filtra pelo ID da EMPRESACONTRATADA.
		if ( is_numeric($whereP['epcid']) ){
			$arWhere[] = "ec.epcid = {$whereP['epcid']}";
		}
		// Filtra pelo ID da SITUACAOOS.
		if ( is_numeric($whereP['stoid']) ){
			$arWhere[] = "so.stoid = {$whereP['stoid']}";
		}
		// Filtra a partir da data da EMISSÃO DA OS.
		if ( $whereP['orsdtemissaoini'] ){
			$arWhere[] = "os.orsdtemissao >= '" . formata_data_sql( $whereP['orsdtemissaoini'] ) . "'";
		}
		// Filtra a até a data da EMISSÃO DA OS.
		if ( $whereP['orsdtemissaofim'] ){
			$arWhere[] = "os.orsdtemissao <= '" . formata_data_sql( $whereP['orsdtemissaofim'] ) . "'";
		}
		// Filtra a UF.
		if ( $whereP['estcod'] ){
			$arWhere[] = "estuf = '" . ( $whereP['estcod'] ) . "'";
		}
		// OS associada.
		if ( $whereP['orsidderivada'] ){
			$arWhere[] = "orsidderivada = '" . ( $whereP['orsidderivada'] ) . "'";
		}
		
		// Monta string WHERE
		$where = (count( $arWhere ) > 0 ? "WHERE (" . implode(') AND (', $arWhere) . ")" : "");
		/*
		 * Cláusulas de parametro - FIM
		 */
		
		$sql = "SELECT 
				   {$coluna}
				FROM 
				   obras.ordemservico os   
				JOIN obras.situacaoos so ON (so.stoid = os.stoid) 
				JOIN obras.grupodistribuicao gd ON (gd.gpdid = os.gpdid 
								    AND gpdstatus = 'A') 
				LEFT JOIN (SELECT
							MAX(hstid),	
							docid, 
							htddata 
						   FROM 
							workflow.historicodocumento
						   WHERE
							aedid = 378
						   GROUP BY
							docid, htddata
							) hd ON hd.docid = gd.docid				    
				JOIN obras.empresacontratada ec ON (ec.epcid = gd.epcid) 
				JOIN entidade.entidade e ON (ec.entid = e.entid) 
				JOIN obras.rotas r ON (r.gpdid  = gd.gpdid 
								       AND r.rotstatus = 'A' 
								       AND strid = " . Rotas::ROTAAPROVADA . ") 
				JOIN seguranca.usuario u ON (u.usucpf  = os.usucpf)
				{$where}
				ORDER BY os.orsid DESC";
				
		if($retorno == 'array'){
			return $this->carregar( $sql );
		} elseif ($retorno == 'string') {
			return $sql;		
		}

	}
	
	function antesSalvar(){
		$this->stoid     = $this->stoid ? $this->stoid : self::SITUACAOOS_GERADA;
		$this->usucpf 	 = $this->usucpf ? $this->usucpf : $_SESSION['usucpf'];
		$this->orsstatus = $this->orsstatus ? $this->orsstatus : 'A';
		
		// Calcula Valor total da OS
		// Busca obras vinculadas ao grupo escolhido
		$obModel = new ObraInfraestrutura();
		$arObrid = $obModel->listaIdObraPorGrupo( $this->gpdid );
		
		if ( !$this->orsvalor ){
			// calcula total dos procedimentos
			$obModel 		   = new GrupoDistribuicao();
			$totalProcedimento = $obModel->pegaTotalValorProcedimento( $arObrid );
			// calcula total do Deslocamento (Rota)
			$obModel 		   = new DeslocamentoController();
			
			$totalDeslocamento = $obModel->totalTrajetos( $this->gpdid );
			# $totalDeslocamento = $obModel->totalRemuneracaoDeslocamento( $this->gpdid );	
			$this->orsvalor = ($totalProcedimento + $totalDeslocamento['valorTotal']);	
		}
		
		return true;
	}

	function depoisSalvar(){
		// Caso não tenha o atributo
		if ( !$this->arqid ){
//			É necessário comitar antes de gerar o arquivo HTML, pois quando se faz uma nova instância da classe modelo
//			perde-se a transação aberta. 
			$this->commit();
			// Gera arquivo de OS
			$arqid  = OSController::gerarArquivoHTML( $this->orsid );
			if ( $arqid ){
				$this->arqid = $arqid;
				// Atualiza o atributo "arqid"
				parent::salvar(false, false);
			}
		}
		return true;
	}
	
	function buscaValorHoraTecnicaPorUf( $estuf ){
		$sql = "select vhtmaxima from obras.valorhoratecnica where estuf = '{$estuf}'";
		return $this->pegaUm( $sql );				
	}
	
	function buscaTempoTrajetoPorObrid( $obrid ){
		$sql = "select trj.trjtempo from obras.repositorio ore 
					INNER JOIN obras.itemgrupo it ON it.repid = ore.repid
					INNER JOIN obras.trajetoria trj ON trj.itgid = it.itgid AND trj.trjstatus = 'A' 
				where ore.repstatus = 'A' and ore.obrid = '{$obrid}'";
		return $this->pegaUm( $sql );				
	}
	
	function buscaValorCombustivelPorUf( $estuf ){
		$sql = "select cbtvalor from obras.combustivel where estuf = '{$estuf}'";
		return $this->pegaUm( $sql );				
	}
	
}