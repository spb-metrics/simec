<?php
	
class ObraInfraestrutura extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras.obrainfraestrutura";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "obrid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'obrid' => null, 
									  	'tobraid' => null, 
									  	'tpcoid' => null, 
									  	'orgid' => null, 
									  	'mdaid' => null, 
									  	'endid' => null, 
									  	'entidunidade' => null, 
									  	'stoid' => null, 
									  	'umdidobraconstruida' => null, 
									  	'umdidareaserconstruida' => null, 
									  	'umdidareaserreformada' => null, 
									  	'umdidareaserampliada' => null, 
									  	'obrdesc' => null, 
									  	'obrdescundimplantada' => null, 
									  	'obrdtinicio' => null, 
									  	'obrdttermino' => null, 
									  	'obrpercexec' => null, 
									  	'obrcustocontrato' => null, 
									  	'obrqtdconstruida' => null, 
									  	'obrcustounitqtdconstruida' => null, 
									  	'obrreaconstruida' => null, 
									  	'obsobra' => null, 
									  	'obsstatus' => null, 
									  	'obsdtinclusao' => null, 
									  	'entidempresaconstrutora' => null, 
									  	'iexid' => null, 
									  	'obrpercbdi' => null, 
									  	'usucpf' => null, 
									  	'entidcampus' => null, 
									  	'obrdescfontefin' => null, 
									  	'obrcomposicao' => null, 
									  	'cloid' => null, 
									  	'tpoid' => null, 
									  	'prfid' => null, 
									  	'obrdtinauguracao' => null, 
									  	'obrdtprevinauguracao' => null, 
									  	'obrstatusinauguracao' => null, 
									  	'obrdtvistoria' => null, 
									  	'sbaid' => null, 
									  	'obrlincambiental' => null, 
									  	'obraprovpatrhist' => null, 
									  	'obrdtprevprojetos' => null, 
									  	'obridorigem' => null, 
									  	'numconvenio' => null, 
									  	'ptpid' => null, 
									  	'obrvalorprevisto' => null, 
									  	'dtiniciocontrato' => null, 
									  	'dtterminocontrato' => null, 
									  	'obrprazoexec' => null, 
									  	'obrdtordemservico' => null, 
									  	'obrdtassinaturacontrato' => null, 
									  	'molid' => null, 
									  	'dtiniciolicitacao' => null, 
									  	'dtfinallicitacao' => null, 
									  	'licitacaouasg' => null, 
									  	'numlicitacao' => null, 
									  	'obridrelacionada' => null, 
									  	'obridaditivo' => null, 
									  	'terid' => null, 
									  	'povid' => null, 
									  	'obrprazovigencia' => null, 
									  	'tpsid' => null, 
									  );
									  
	function pegaSituacaoObra( $stoid ){
		$sql = "SELECT
					stodesc
				FROM 
					obras.situacaoobra
				WHERE
					stoid = {$stoid}";
		
		return $this->pegaUm( $sql );
	}
	
	function pegaUnidadeMedida( $umdid ){
		$sql = "SELECT
					umdeesc
				FROM 
					obras.unidademedida
				WHERE
					umdid = {$umdid}";
		
		return $this->pegaUm( $sql );
	}

	function listaIdObraPorGrupo( $gpdid, $orgid = null ){
		if ($orgid){
			$orgid = (array) $orgid;	
			$join = "JOIN obras.obrainfraestrutura oi ON oi.obrid = r.obrid 
														 AND oi.obsstatus = 'A'
														 AND oi.orgid IN (" . implode(",", $orgid) . ")";
		}
		
		$sql = "SELECT 
					r.obrid
				FROM
					obras.itemgrupo i
					JOIN obras.repositorio r ON r.repid = i.repid
								    			--AND r.repstatus = 'A'
					$join
				WHERE
					i.gpdid = {$gpdid}";
		
		return $this->carregarColuna( $sql );
	}
	
	function listaDadosObraPorGrupo( $gpdid, Array $arParam = null ){
		$coluna = implode(', ', (is_null($arParam['coluna']) ? ((array) '*') : ((array) $arParam['coluna'])) );
		
		$sql = "SELECT 
					{$coluna}
				FROM
					obras.itemgrupo i
					JOIN obras.repositorio r USING ( repid )
					JOIN obras.obrainfraestrutura o USING ( obrid )
				WHERE
					i.gpdid = {$gpdid}";
		
		return $this->carregar( $sql );
	}
}