<?php
	
class Orgao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras.orgao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "orgid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'orgid' => null, 
									  	'orgdesc' => null, 
									  	'orgstatus' => null, 
									  	'orgdtinclusao' => null, 
									  	'orgtipo' => null, 
									  	'orgcodigo' => null, 
									  );
}