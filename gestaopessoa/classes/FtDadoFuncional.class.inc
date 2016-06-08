<?php
	
class FtDadoFuncional extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "gestaopessoa.ftdadofuncional";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "fdfid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'fdfid' => null, 
									  	'fdpcpf' => null, 
									  	'fdfpostotrabalho' => null, 
									  	'fdfcnpjempresa' => null, 
									  	'fdfnumerocontrato' => null, 
									  	'fdfempresa' => null, 
									  	'fdfnumeroprojeto' => null, 
									  	'fdfprojetodatainicio' => null, 
									  	'fdfprojetodatafim' => null, 
									  	'forid' => null, 
									  	'fooid' => null, 
									  	'fcmid' => null, 
									  	'fdfexercecargofuncao' => null, 
									  	'furid' => null, 
									  	'fulid' => null, 
									  	'futid' => null, 
									  	'fdfsala' => null, 
									  	'fdftelefone' => null, 
									  	'fdfemail' => null, 
									  );
	/*
	 * "Sobrescrita" do método popularObjeto() da classe Modelo.
	 * neste método o array de campos será dinamico, conforme a função controlaDadoFuncional() retornar.
	 * a função receberá um $arCampos
	 */
	public function popularObjetoFuncional( $arCampos, $arDados = null ){
		
		$arDados = $arDados ? $arDados : $_REQUEST;
		
		foreach( $arCampos as $campo ){
			
			if( key_exists( $campo, $arDados ) ){
				$this->$campo = $arDados[$campo];
			}
		}
	}
	 public $arCompleto = array(
						  	'fdfid' , 
						  	'fdpcpf',
						  	'fdfpostotrabalho',
						  	'fdfcnpjempresa' , 
						  	'fdfnumerocontrato' ,
						  	'fdfempresa',
						  	'fdfnumeroprojeto' ,
						  	'fdfprojetodatainicio',
						  	'fdfprojetodatafim',
						  	'forid' ,
						  	'fooid',
						  	'fcmid',
						  	'fdfexercecargofuncao',
						  	'furid',
						  	'fulid',
						  	'futid',
						  	'fdfsala',
						  	'fdftelefone' ,
						  	'fdfemail' 
						  );
	 public $arEfetivo     = array(
						  	'fdfid' , 
						  	'fdpcpf',  
						  	'fcmid',
						  	'fdfexercecargofuncao', 
						  	'fulid',
						  	'fdfsala',
						  	'fdftelefone' ,
						  	'fdfemail' 
						  );
	 public $arCedido      = array(
						  	'fdfid' , 
						  	'fdpcpf',  
						  	'fcmid',
						  	'fdfexercecargofuncao', 
						  	'fulid',
						  	'fdfsala',
						  	'fdftelefone' ,
						  	'fdfemail' 
						  );
	 public $arCTU     = array(
						  	'fdfid' , 
						  	'fdpcpf',
	 						'fulid',
						  	'fdfpostotrabalho',    
						  	'fdfsala',
						  	'fdftelefone' ,
						  	'fdfemail' 
						  );
	 public $arConsultor  = array(
						  	'fdfid' , 
						  	'fdpcpf',
						  	'fdfnumeroprojeto' ,
						  	'fdfprojetodatainicio',
						  	'fdfprojetodatafim',
						  	'furid',
						  	'fdfsala',
						  	'fdftelefone' ,
						  	'fdfemail' 
						  );
	 public $arExercicioDes = array(
						  	'fdfid' , 
						  	'fdpcpf',
						  	'fcmid',
						  	'fdfexercecargofuncao',
						  	'fulid',
						  	'fdfsala',
						  	'fdftelefone' ,
						  	'fdfemail' 
						  );
	 public $arExercicioPro = array(
						  	'fdfid' , 
						  	'fdpcpf',
						  	'forid' ,
						  	'fcmid',
						  	'fulid',
						  	'fdfsala',
						  	'fdftelefone' ,
						  	'fdfemail' 
						  );
	 public $arTerceirizado = array(
						  	'fdfid' , 
						  	'fdpcpf',
						  	'fdfpostotrabalho',
						  	'fdfcnpjempresa' , 
						  	'fdfempresa',
						  	'futid',
						  	'fdfsala',
						  	'fdftelefone' ,
						  	'fdfemail' 
						  );
	 public $arAnistiadoCLT = array(
						  	'fdfid' , 
						  	'fdpcpf',
						  	'fcmid',
						  	'fdfexercecargofuncao',
						  	'fulid',
						  	'fdfsala',
						  	'fdftelefone' ,
						  	'fdfemail' 
						  );
	 public $arCargoComissionado = array(
						  	'fdfid' , 
						  	'fdpcpf',
						  	'fulid',
						  	'fdfsala',
						  	'fdftelefone' ,
						  	'fdfemail' 
						  );
	 public $arRequisitados = array(
						  	'fdfid' , 
						  	'fdpcpf',
						  	'forid' ,
						  	'fooid',
						  	'fcmid',
						  	'fdfexercecargofuncao',
						  	'fulid',
						  	'fdfsala',
						  	'fdftelefone' ,
						  	'fdfemail' 
						  );
	 public $arColaboracaoTecnica = array(
						  	'fdfid' , 
						  	'fdpcpf',
						  	'forid' ,
						  	'fooid',
						  	'fcmid',
						  	'fdfexercecargofuncao',
						  	'fulid',
						  	'fdfsala',
						  	'fdftelefone' ,
						  	'fdfemail' 
						  );
}