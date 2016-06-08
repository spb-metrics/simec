<?php
	
class Ftformacaoacademica extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "gestaopessoa.ftformacaoacademica";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ffaid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'ffaid' => null, 
									  	'fdpcpf' => null,
									  	'tfoid' => null, 
									  	'ffasituacao' => null, 
									  	'ffaanoconclusao' => null, 
									  	'ffaordem' => null, 
    								  	'ffacurso' => null, 
									  	'ffanomeinstituicao' => null, 
									  );
}