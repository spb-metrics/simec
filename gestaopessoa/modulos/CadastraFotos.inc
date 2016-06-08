<?php

/*
 * Página que irá cadastrar todas as fotos no BD
 */
//include_once "../global/config.inc";
//include_once APPRAIZ . "/includes/classes_simec.inc";
//include_once APPRAIZ . "/includes/funcoes.inc";
include_once APPRAIZ . "/includes/classes/fileSimec.class.inc";
global $db;
function completaZerosEsquerda($string, $quantidade){
	
	return str_pad($string, $quantidade, "0", STR_PAD_LEFT);
	
}

// pega o endereço do diretório
$diretorio = "C:/Documents and Settings/rodrigossilva/Desktop/Fotos/";
 
// abre o diretório
$ponteiro  = opendir($diretorio);

// monta os vetores com os itens encontrados na pasta
while ($nome_itens = readdir($ponteiro)) {
	if( ($nome_itens != ".") && ($nome_itens != "..") && ($nome_itens != "Thumbs.db") ){
		// pegando o cpf da foto
		$string = str_replace(".JPG","",$nome_itens);
		$cpf = completaZerosEsquerda($string,11);
		
		// verificando se o usuário já possui uma foto cadastrada
		$sql = "SELECT 
					agpid
				FROM gestaopessoa.anexogp
				WHERE
					fdpcpf = '{$cpf}'
					AND agpstatus = 'A';";
		
		// se não existir foto cadastrada então eu movo a foto nova
		if( !$db->pegaUm($sql) ){
			echo $cpf."<br>";
			// gravando os anexos
			$campos = array("fdpcpf"         	=> "'".$cpf."'",
							"agpstatus"    		=> "'A'",
							"agpdatainclusao" 	=> "NOW()",
							"usucpf"     		=> "'".$cpf."'");
			
			$file = new FilesSimec("anexogp", $campos ,"gestaopessoa");
			$file->setMover( $diretorio.$nome_itens, "JPG" );
		}// fim do segundo if
		
	}// fim do primeiro if
}// fim do while

?>