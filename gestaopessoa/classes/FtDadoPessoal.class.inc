<?php
	
class FtDadoPessoal extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "gestaopessoa.ftdadopessoal";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "fdpcpf" );
    

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'fdpcpf' => null, 
									  	'fdpnome' => null, 
									  	'fdpsiape' => null, 
									  	'eciid' => null, 
									  	'fdpconjuge' => null, 
									  	'fdpnomepai' => null, 
									  	'fdpnomemae' => null, 
									  	'fdpsexo' => null, 
									  	'fdpdatanascimento' => null, 
									  	'fdpnaturalidade' => null, 
									  	'fdpenderecoresidencial' => null, 
									  	'fdpbairro' => null, 
									  	'muncod' => null, 
									  	'estuf' => null, 
									  	'fdpcep' => null, 
									  	'fdpdddtelefoneres' => null, 
									  	'fdptelefoneres' => null, 
									  	'fdpdddcelular' => null, 
									  	'fdpcelular' => null, 
									  	'fdpgruposanguineo' => null, 
									  	'fdpfatorrh' => null, 
									  	'fdpcor' => null, 
									  	'fdpdeficiente' => null, 
									  	'fdpdeficiencia' => null, 
									  	'fdpidentidade' => null, 
									  	'fdpidexpeditor' => null, 
									  	'fdpufidentidade' => null, 
									  	'fdpiddataexpedicao' => null, 
									  	'fdpregistroprofissional' => null, 
									  	'fdpdataregistroprofissional' => null, 
									  	'fdptituloeleitor' => null, 
									  	'fdpzonaeleitoral' => null, 
									  	'fdpsecaoeleitoral' => null, 
									  	'fdplocaleleitoral' => null, 
									  	'fdpserieeleitoral' => null, 
									  	'fdpdataprimeiroemprego' => null, 
									  	'fdpbanco' => null, 
									  	'fdpnomebanco' => null, 
									  	'fdpagencia' => null, 
									  	'fdpcontacorrente' => null, 
									  	'fstid' => null, 
									  );
	public function inserir(){
	        
		if( count( $this->arChavePrimaria ) > 1 ) trigger_error( "Favor sobreescrever método na classe filha!" );
		
		$arCampos  = array();
		$arValores = array();
		$arSimbolos = array();
		
		foreach( $this->arAtributos as $campo => $valor ){ 
			if( $valor !== null ){
				$arCampos[]  = $campo;
				$arValores[] = trim($valor);
			}
		}

		if( count( $arValores ) ){
			
			$sql = " insert into $this->stNomeTabela ( ". implode( ', ', $arCampos   ) ." ) 
											  values ( '". implode( "', '", $arValores ) ."' ) 
					 returning {$this->arChavePrimaria[0]}";
	
			$stChavePrimaria = $this->arChavePrimaria[0];
			return $this->$stChavePrimaria = $this->pegaUm( $sql );
		}
	}
}