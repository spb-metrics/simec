<?php
	
class QItemCondicao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "questionario.itemcondicao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "itcid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    
    protected $arAtributos     = array(
									  	'itcid' => null, 
									  	'queid' => null, 
									  	'perid' => null, 
									  	'itpid' => null, 
									  	'itcperid' => null, 
									  	'itcjustificativa' => null,
									  	'itcstatus' => null
									  );

	public function pegaUmObjeto(Array $param){
		$param  = (array) $param;
		
		foreach ($param as $k => $item){
			if ( $k === 'perid' ){
				$condicao[] = "itcperid" . ( $item ? " = " . $item : ' IS NULL' );
				continue;	
//			}elseif ( $k === 'queid' ){
//				$condicao[] = "queid" . ( $item ? " = " . $item : ' IS NULL' );
//				continue;	
			}
		}
		$condicao[] = "itcstatus = 'A'";
		
		$sql = "SELECT
					*
				FROM
					{$this->stNomeTabela}
				" . ( count($condicao) ? " WHERE " . implode(" AND ", $condicao) : "" );

		return $this->carregar($sql);
	}	

	
}