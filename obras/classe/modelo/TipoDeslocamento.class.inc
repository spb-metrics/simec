<?php
	
class TipoDeslocamento extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras.tipodeslocamento";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tdeid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tdeid' => null, 
									  	'tdedsc' => null, 
									  );

	public function buscaTipoDeslocamentoPorGrupo(Array $arParam){
		$gpdid = $arParam['gpdid'];
		$strid = $arParam['strid'] ? $arParam['strid'] : Rotas::ROTAAPROVADA;
		
		$sql = "SELECT 
					DISTINCT tdedsc
				FROM
					obras.itemgrupo it
				JOIN obras.trajetoria t USING(itgid)	
				JOIN {$this->stNomeTabela} USING(tdeid)	
				JOIN obras.rotas r ON r.gpdid = it.gpdid
						      		  AND strid = {$strid}
				WHERE
					it.gpdid = {$gpdid}";
		
		return $this->carregarColuna($sql);		
	}
	
	public function buscaTipoDeslocamentoPorObra($obrid){
		if($obrid){
			$sql = "SELECT t.tdeid FROM obras.repositorio r
						inner join obras.itemgrupo ig on r.repid = ig.repid
						inner join obras.trajetoria t on ig.itgid = t.itgid
					WHERE r.obrid = {$obrid}";
		}
		
		return $this->pegaUm($sql);		
	}									  
}