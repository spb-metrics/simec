<?php
	
class EmpresaContratada extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras.empresacontratada";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "epcid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'epcid' => null, 
									  	'entid' => null, 
									  	'epcnumproceconc' => null, 
									  	'epcnumcontrato' => null, 
									  	'epcdtiniciocontrato' => null, 
									  	'epcdtfinalcontrato' => null, 
									  );
									  
	function buscaNomeEmpresa( $epcid ){
		$sql = "SELECT
					e.entnome
				FROM
					{$this->stNomeTabela} ec
				JOIN entidade.entidade e ON e.entid = ec.entid
 				WHERE
 					ec.epcid = {$epcid}";
		
		return $this->pegaUm( $sql );
	}									  
	
	function buscaDadosEmpresa( $epcid ){
		$sql = "SELECT
					ec.epcnumproceconc,
					ec.epcnumcontrato,					 
					e.entnome as empresa, 
					e.entnumcpfcnpj as cnpj 
				FROM
					{$this->stNomeTabela} ec
				JOIN entidade.entidade e ON e.entid = ec.entid
 				WHERE
 					ec.epcid = {$epcid}";
		
		return $this->pegaLinha( $sql );
	}									  
}