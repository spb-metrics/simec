<?php
	
class FtAtividadeDesenvolvida extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "gestaopessoa.ftatividadedesenvolvida";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "fadid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'fadid' => null, 
									  	'fdpcpf' => null, 
									  	'ftaid' => null, 
									  	'fnaid' => null, 
									  	'faddescricao' => null, 
									  	'fadordem' => null, 
									  );
}