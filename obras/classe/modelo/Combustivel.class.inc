<?php
	
class Combustivel extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras.combustivel";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "cbtid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'cbtid' => null, 
									  	'estuf' => null, 
									  	'cbtvalor' => null, 
									  );
	function buscaPrecoCombustivelPorUF( $estuf ){		
		$sql = "SELECT
					cbtvalor
				FROM
					{$this->stNomeTabela}
				WHERE
					estuf = '{$estuf}'";
		
		return $this->pegaUm( $sql );			
	}
									  
	function listaCombustivel(){
		$sql = "SELECT 
					estuf, 
					cbtvalor,
					cbtid
 				FROM 
 					obras.combustivel
 				ORDER BY estuf;";
 					
 		return $this->carregar( $sql );			
	}
	
	function antesSalvar(){
		$return = true;
		if ( $this->estuf == '' || $this->cbtvalor == '' ){
			$return = false;
		}else{
			$this->cbtvalor = str_replace(array(".", ","), array("", "."), $this->cbtvalor);
		}
		
		return $return;
	}
	
}