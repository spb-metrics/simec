<?

if ($_REQUEST["acao"] == "D"){
	
	$obras = new Obras();
	$obras->EnviarArquivoInfraEstrutura($_FILES,$_POST["anedescricao"]);
	
}

?>