/**
 * Vari�vel do tipo objeto, instanciada como global
 * 
 * @access public
 * @name objs
 * @var object
 */
var objs = new Object;

/**
 * dCPF classe que estrutura a busca/tratamento dos dados vindo do WebService da Receita Federal;
 * 
 * @author FelipeChiavicatti
 */
function dCPF (){

    /**
     * Atributo que receber� o �ndice e valor retornado do WebService
     * 
     * @see dCPF()
     * @access public
     * @name dadosAjax
     * @var atribute
     * @attribute array indice
     * @attribute array valor
     */
	this.dadosAjax = {
						indice : new Array(), 
						valor  : new Array()
				  	 };	

    /**
     * Atributo que receber� o �ndice e valor retornado do WebService
     * 
     * @see dCPF()
     * @access public
     * @name dados
     * @var atribute
     * @attribute string no_pessoa_rf "Nome"
	 * @attribute string nu_cpf_rf "N�mero do CPF"
	 * @attribute string no_mae_rf "Nome da m�e"
	 * @attribute string dt_nascimento_rf "Data de nascimento"
	 * @attribute string sg_sexo_rf "Sexo" M:Masculino | F:Feminino | X:N�o informado
	 * @attribute string nu_titulo_eleitor_rf "N�mero do t�tulo de eleitor"
	 * @attribute string st_indicador_residente_ext_rf "Indicativo de residente no exterior" 1:Residente | 2:N�o residente
	 * @attribute string co_pais_residente_exterior_rf "Caso seja residente no exterior, o c�digo do pa�s onde reside"
	 * @attribute string no_pais_residente_exterior_rf "Caso seja residente no exterior, o nome do pa�s onde reside"
	 * @attribute string st_cadastro_rf "Indicativo da situa��o cadastral do CPF" 0:Regular | 1:Cancelada por encerramento | 2:Suspensa | 3:Cancelada por �bito sem esp�lio | 4:Pendente de regulariza��o | 5:Cancelada por multiplicidade | 8:Nula | 9:Cancelada de of�cio 
	 * @attribute string nu_ano_obito_rf "Ano do �bito no formato AAAA"
	 * @attribute string co_natureza_ocupacao_rf "C�digo da natureza ocupa��o da pessoa f�sica"
	 * @attribute string co_ocupacao_principal_rf "C�digo da natureza ocupa��o da pessoa f�sica" 
	 * @attribute string nu_ano_exercicio_ocupacao_rf "Ano do exerc�cio da informa��o de natureza de ocupa��o principal"
	 * @attribute string co_unidade_administrativa_rf "C�digo da unidade administrativa"
	 * @attribute string dt_inscricao_atualizacao_cpf_rf "Data de inscri��o do CPF ou da �ltima opera��o de atualiza��o no formato AAAAMMDD"
	 * @attribute string nu_rg "N�mero do documento de identidade da pessoa f�sica"
	 * @attribute string dt_emissao_rg "Data de emiss�o do RG da pessoa f�sica no formato AAAAMMDD"
	 * @attribute string ds_orgao_expedidor_rg "Nome do �rg�o emissor do RG da pessoa f�sica"
	 * @attribute string st_indicador_estrangeiro_rf "Indicativo de estrangeiro" 0:N�o � estrangeiro | 1:� estrangeiro
	 * @attribute string dt_cadastro "Data e hora em que foi feito o cadastramento da pessoa f�sica no formato AAAAMMDDHHMMSS"
	 * @attribute string co_cidade "C�digo IBGE do munic�pio onde o endere�o se encontra"
	 * @attribute string co_tipo_endereco_pessoa "C�digo do tipo de endere�o" 1:Endere�o Receita | 2:Endere�o Residencial | 3:Endere�o Comercial 
	 * @attribute string sg_uf "Sigla da unidade federativa do endere�o"
	 * @attribute string ds_localidade "Localidade geogr�fica dos correios"
	 * @attribute string ds_bairro "Bairro do endere�o"
	 * @attribute string ds_logradouro "Logradouro do endere�o"
	 * @attribute string ds_logradouro_comp "Complemento do logradouro"
	 * @attribute string ds_numero "N�mero do endere�o"
	 * @attribute string nu_cep "C�digo de endere�amento postal sem m�scara"
	 * @attribute string ds_ponto_referencia "Ponto de refer�ncia que sirva de aux�lio no endere�o"
	 * @attribute string ds_tipo_logradouro "Descri��o do tipo de logradouro"
	 * @attribute string co_tipo_contato_pessoa "Tipo de contato" 1:Telefone receita | 2:Telefone Residencial | 3:Telefone comercial | 4:Correio eletr�nico | 5:Fax | 6:Telefone celular
	 * @attribute string ds_contato_pessoa "Cont�m a descri��o do contato, como o n�mero de telefone ou e-mail da pessoa f�sica"
     */	
	this.dados = { 
					no_pessoa_rf:					'',
					nu_cpf_rf:						'',
					no_mae_rf:						'',
					dt_nascimento_rf:				'',
					sg_sexo_rf:						'',
					nu_titulo_eleitor_rf:			'',
					st_indicador_estrangeiro_rf:	'',
					co_pais_residente_exterior_rf:	'',
					st_indicador_residente_ext_rf:	'',
					no_pais_residente_exterior_rf:	'',
					st_cadastro_rf:					'',
					nu_ano_obito_rf:				'',
					co_natureza_ocupacao_rf:		'',
					co_ocupacao_principal_rf:		'',
					co_unidade_administrativa_rf:	'',
					nu_ano_exercicio_ocupacao_rf:	'',
					dt_inscricao_atualizacao_cpf_rf:'',
					nu_rg:							'',
					dt_emissao_rg:					'',
					ds_orgao_expedidor_rg:			'',
					dt_cadastro:					'',
					nu_cpf_rf:						'',
					co_cidade:						'',
					co_tipo_endereco_pessoa:		'',
					sg_uf:							'',
					ds_localidade:					'',
					ds_bairro:						'',
					ds_logradouro:					'',
					ds_logradouro_comp:				'',
					ds_numero:						'',
					nu_cep:							'',
					ds_ponto_referencia:			'',
					ds_tipo_logradouro:				'',
					nu_cpf_rf:						'',
					co_tipo_contato_pessoa:			'',
					ds_contato_pessoa:				''				
		 		};
	/**
	 * Faz requisi��o AJAX ao arquivo PHP que faz a requisi��o do WebService
	 * 
	 * @param string cpf com m�scara ou n�o;
	 */
	this.buscarDados = function (cpf){
		
										// Faz uma requisi��o ajax							
										var req = new Ajax.Request('/includes/webservice/cpf.php', {
																	method:      'post',
																	parameters:  '&ajaxCPF=' + cpf,
																	asynchronous: false,
																	onComplete:   function (res)
																	{	
		
										        							if (res.responseText) {
										        								objs.formatarDados(res.responseText);
										        							}	
										        								  
																	  }
																});
											
									  }




	


	/**
	 * Formata os dados passados no parametro e seta nos atributos da classe "dadosAjax.indice e dadosAjax.valor"
	 * 
	 * @param string text no formato " indice#valor| ";
	 */
	this.formatarDados = function (text){
		
											var linhaItem  = [];
										    var colunaItem = []; 
											var indice 	   = [];
											var val		   = [];
										
											linhaItem = text.split('|');		
											for (i=0; i < linhaItem.length; i++){
												colunaItem = linhaItem[i].split('#');											
												indice[i]  = colunaItem[0];
												val[i]     = colunaItem[1];												
											}

											objs.dadosAjax.indice = indice.join('#').split('#');
											objs.dadosAjax.valor  = val.join('#').split('#');

											objs.carregarDados();
											
										}



	/**
	 * Seta nos atributos da classe os valores, relacionando �ndices e valores;
	 */
	this.carregarDados = function () {
										var ajaxCampoName  = objs.dadosAjax.indice;
										var ajaxCampoValue = objs.dadosAjax.valor;
										
										for (i=0; i < ajaxCampoName.length; i++){
											
											switch (ajaxCampoName[i]) {
												case 'no_pessoa_rf': 
													objs.dados.no_pessoa_rf = ajaxCampoValue[i]; 
													continue; 
													break;
												case 'nu_cpf_rf': 
													objs.dados.nu_cpf_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'no_mae_rf':
													objs.dados.no_mae_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'dt_nascimento_rf':
													objs.dados.dt_nascimento_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'sg_sexo_rf':
													objs.dados.sg_sexo_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'nu_titulo_eleitor_rf':
													objs.dados.nu_titulo_eleitor_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'st_indicador_estrangeiro_rf':
													objs.dados.st_indicador_estrangeiro_rf = ajaxCampoValue[i];
													continue;
													break;	
												case 'co_pais_residente_exterior_rf':
													objs.dados.co_pais_residente_exterior_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'st_indicador_residente_ext_rf':
													objs.dados.st_indicador_residente_ext_rf = ajaxCampoValue[i];
													continue;
													break;												
												case 'st_cadastro_rf':
													objs.dados.st_cadastro_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'nu_ano_obito_rf':
													objs.dados.nu_ano_obito_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'co_natureza_ocupacao_rf':
													objs.dados.co_natureza_ocupacao_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'co_ocupacao_principal_rf':
													objs.dados.co_ocupacao_principal_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'co_unidade_administrativa_rf':
													objs.dados.co_unidade_administrativa_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'nu_ano_exercicio_ocupacao_rf':
													objs.dados.nu_ano_exercicio_ocupacao_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'dt_inscricao_atualizacao_cpf_rf':
													objs.dados.dt_inscricao_atualizacao_cpf_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'nu_rg':
													objs.dados.nu_rg = ajaxCampoValue[i];
													continue;
													break;
												case 'dt_emissao_rg':
													objs.dados.dt_emissao_rg = ajaxCampoValue[i];
													continue;
													break;
												case 'ds_orgao_expedidor_rg':
													objs.dados.ds_orgao_expedidor_rg = ajaxCampoValue[i];
													continue;
													break;
												case 'dt_cadastro':
													objs.dados.dt_cadastro = ajaxCampoValue[i];
													continue;
													break;
												case 'co_cidade':
													objs.dados.co_cidade = ajaxCampoValue[i];
													continue;
													break;
												case 'co_tipo_endereco_pessoa':
													objs.dados.co_tipo_endereco_pessoa = ajaxCampoValue[i];
													continue;
													break;
												case 'sg_uf':
													objs.dados.sg_uf = ajaxCampoValue[i];
													continue;
													break;
												case 'ds_localidade':
													objs.dados.ds_localidade = ajaxCampoValue[i];
													continue;
													break;													
												case 'ds_bairro':
													objs.dados.ds_bairro = ajaxCampoValue[i];
													continue;
													break;
												case 'ds_logradouro':
													objs.dados.ds_logradouro = ajaxCampoValue[i];
													continue;
													break;													
												case 'ds_logradouro_comp':
													objs.dados.ds_logradouro_comp = ajaxCampoValue[i];
													continue;
													break;
												case 'ds_numero':
													objs.dados.ds_numero = ajaxCampoValue[i];
													continue;
													break;													
												case 'nu_cep':
													objs.dados.nu_cep = ajaxCampoValue[i];
													continue;
													break;
												case 'ds_ponto_referencia':
													objs.dados.ds_ponto_referencia = ajaxCampoValue[i];
													continue;
													break;													
												case 'ds_tipo_logradouro':
													objs.dados.ds_tipo_logradouro = ajaxCampoValue[i];
													continue;
													break;													
												case 'co_tipo_contato_pessoa':
													objs.dados.co_tipo_contato_pessoa = ajaxCampoValue[i];
													continue;
													break;													
												case 'ds_contato_pessoa':
													objs.dados.ds_contato_pessoa = ajaxCampoValue[i];
													continue;
													break;													
											}
																
												
										}
		
									}
    /**
     * Atributo que receber� a inst�ncia do objeto "this"
     * 
     * @see dCPF()
     * @access public
     * @name objs
     * @var object
     */
	objs = this;
	
}