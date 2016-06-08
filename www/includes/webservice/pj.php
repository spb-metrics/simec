<?php 
if (!$_POST['ajaxPJ']):
?>
	<script type="text/javascript" src="/includes/prototype.js"></script>
	<script src="/includes/webservice/pj.js"></script>
<?php
endif;

/**
 * Classe para acesso ao webservice de pessoa jur�dica.
 * 
 * PS: N�o esque�a de ler o leiame.txt
 *
 */
final class PessoaJuridicaClient
{
	/**
	 * Coloca o objeto do cliente do webservice.
	 *
	 * @var SoapClient
	 */
	private $soapClient;

	
	/**
	 * Construtor da classe.
	 *
	 * @param string $wsdl
	 */
	public function __construct($wsdl)
	{

		try{
			$this->soapClient = new SoapClient( $wsdl );
		} catch (Exception $e){
			exit("N�o est� conectado!");
		}
		
	}
	
	/**
	 * Retorna dados de pessoa jur�dica pelo CNPJ.
	 *
	 * @param string $cnpj
	 * @return string
	 */
	public function solicitarDadosResumidoPessoaJuridicaPorCnpj( $cnpj )
	{
		return (  $this->soapClient->solicitarDadosResumidoPessoaJuridicaPorCnpj( $cnpj ) );
	}
	
	/**
	 * Retorna dados completo de pessoa jur�dica por CNPJ.
	 *
	 * @param string $cnpj
	 * @return string
	 */
	public function solicitarDadosPessoaJuridicaPorCnpj( $cnpj )
	{
		return (  $this->soapClient->solicitarDadosPessoaJuridicaPorCnpj( $cnpj ) );
	}
	
	/**
	 * Retorna dados de Endere�o da pessoa jur�dica por CNPJ.
	 *
	 * @param string $cnpj
	 * @return string
	 */
	public function solicitarDadosEnderecoPessoaJuridicaPorCnpj( $cnpj )
	{
		return (  $this->soapClient->solicitarDadosEnderecoPessoaJuridicaPorCnpj( $cnpj ) );
	}
	
	/**
	 * Retorna dados de Contato da pessoa jur�dica por CNPJ.
	 *
	 * @param string $cnpj
	 * @return string
	 */
	public function solicitarDadosContatoPessoaJuridicaPorCnpj( $cnpj )
	{
		return (  $this->soapClient->solicitarDadosContatoPessoaJuridicaPorCnpj( $cnpj ) );
	}
	
	/**
	 * Retorna as informa��es do s�cio da pessoa jur�dica.
	 *
	 * @param string $cnpj
	 * @return string
	 */
	public function solicitarDadosSocioPessoaJuridicaPorCnpj( $cnpj )
	{
		return (  $this->soapClient->solicitarDadosSocioPessoaJuridicaPorCnpj( $cnpj ) );
	}
	
}

if ($_POST['ajaxPJ']):

	$pj = str_replace(array('/', '.', '-'), '', $_POST['ajaxPJ']);
	
	/**
	 * Aqui � feita a chamada do m�todo da classe cliente do webservice.
	 */
	$objPessoaJuridica = new PessoaJuridicaClient("http://ws.mec.gov.br/PessoaJuridica/wsdl");
	//$xml = $objPessoaJuridica->solicitarDadosPessoaJuridicaPorCnpj("00394445053213");
	//$xml = $objPessoaJuridica->solicitarDadosPessoaJuridicaPorCnpj("05605468000123");
	//$xml = $objPessoaJuridica->solicitarDadosPessoaJuridicaPorCnpj("00720144000112");
	//$xml = $objPessoaJuridica->solicitarDadosPessoaJuridicaPorCnpj("01109184000438");
	//$xml = $objPessoaJuridica->solicitarDadosPessoaJuridicaPorCnpj("00394445013939");
	$xml = $objPessoaJuridica->solicitarDadosPessoaJuridicaPorCnpj($pj);
	
	$obj = (array) simplexml_load_string($xml);
	$xml = simplexml_load_string($xml);
	
	if (!$obj['PESSOA']) {
		die();
	}
	
	$empresa  = (array) $obj['PESSOA'];
	$endereco = (array) $obj['PESSOA']->ENDERECOS->ENDERECO;
	$contato  = (array) $obj['PESSOA']->CONTATOS->CONTATO;	
	
	foreach($empresa as $k =>$val):
		if (ctype_upper($k)){continue;}
		$return[] = "$k#{$val}";
//		echo "{$k} <br>";
	endforeach;
	
	foreach($endereco as $k =>$val):
		if (ctype_upper($k)){continue;}
		$return[] = "$k#{$val}";
//		echo "{$k} <br>";
	endforeach;	
	
	foreach($contato as $k =>$val):
		if (ctype_upper($k)){continue;}
		$return[] = "$k#{$val}";
//		echo "{$k} <br>";
	endforeach;	
	
	for ($i=0; $i < count($xml->PESSOA->SOCIOS->SOCIO); $i++ ):
		foreach ($xml->PESSOA->SOCIOS->SOCIO[$i] as $k=>$val){
			$socio[] = "$k#{$val}";
//			echo "$k<br>";	
		}
	endfor;
	
	die(implode('|', $return)."$$".implode('|', $socio));	
endif;