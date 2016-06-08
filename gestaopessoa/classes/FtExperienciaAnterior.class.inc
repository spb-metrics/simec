<?php
	
class FtExperienciaAnterior extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "gestaopessoa.ftexperienciaanterior";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "feaid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'feaid' => null, 
									  	'fdpcpf' => null, 
									  	'fneid' => null, 
									  	'fteid' => null, 
									  	'feadescricao' => null, 
									  	'feaordem' => null, 
    									'feaanoinicio' => null,
    									'feaanofim' => null,
									  );
}