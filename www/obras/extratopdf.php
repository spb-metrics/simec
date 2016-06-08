<?php

ini_set("memory_limit", "1024M");

//Chamando a classe de FPDF
require('../../includes/fpdf/fpdf.inc');
require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

$obras = new Obras();
$dobras = new DadosObra(null);

if ( !empty($_SESSION["extratoObrid"]) ){
	
	$dadoObrid = $_SESSION["extratoObrid"];
	
	// Carrega os dados da obra 
	$dados = $obras->Dados($dadoObrid);
	$dobras = new DadosObra($dados);
	
	
}

$PDF = new PDF_Table( 'P','cm','A4' ); //documento em formato de retrato, medido em cm e no tamanho A4
$PDF->SetMargins(1, 1, 1); // margem esquerda = 3 , superior = 3 e direita = 2.
$PDF->SetAuthor($GLOBALS['parametros_sistema_tela']['sigla']); //informando o autor do documento.
$PDF->SetTitle($GLOBALS['parametros_sistema_tela']['sigla']); //informando o título do documento.
$PDF->AddPage(); //adicionando um nova página



/*
 * CRIANDO CABEÇALHO DO PDF
 */
$PDF->Image("../imagens/brasao.JPG",1,0.8,1.2,1.2); //endereco da imagem,posicao X(horizontal),posicao Y(vertical), tamanho altura, tamanho largura
$PDF->SetFont('Arial', '', 6); //informando a fonte, estilo (B = negrito) e tamanho da fonte
$PDF->SetX(2.5);
$PDF->Cell(6,0,$GLOBALS['parametros_sistema_tela']['unidade_pai'],0,'P');
$PDF->SetX(2.5);
$PDF->Cell(6,0.5,'Palácio do Planalto',0,'P');
$PDF->SetX(2.5);
$PDF->Cell(6,1,'CEP:  - Brasília - DF - Brasil',0,'P');

/*
 * FIM
 * CRIANDO CABEÇALHO DO PDF
 */


$PDF->SetY( 2.5 );
$PDF->SetX( 1 );
$PDF->SetFont( 'Arial', 'B', 8 ); 
$PDF ->SetFillColor( 234, 234, 234 ); 
$PDF->Cell( 19, 0.5, 'Extrato da Obra', 1, 0, 'C', 1 );
$PDF->ln();

$PDF->SetFont('Arial', '', 6);

if ( $dobras->getOrgId() ){
	$tipoEnsino = $db->pegaUm("SELECT orgdesc as descricao 
							   FROM obras.orgao 
							   WHERE orgid=" . $dobras->getOrgId());
}

 
$PDF->MultiCell( 19, 0.5, 'Tipo de estabelecimento: ' . $tipoEnsino, 1, 'J', 0);

$entidade = new Entidade($dobras->getEntIdUnidade());
$entnome = $entidade->entnome;

$PDF->MultiCell( 19, 0.5, 'Unidade ResponsÃ¡vel pela Obra: ' . $entnome , 1, 'J', 0);

$campus = new Entidade($dobras->getEntIdCampus());
$campusnome = $campus->entnome;

$PDF->MultiCell( 19, 0.5, 'Campus / Unidade: ' . $campusnome, 1, 'J', 0);
$PDF->MultiCell( 19, 0.5, 'Nome da Obra: ' . $dobras->getObrDesc(), 1, 'J', 0);

if ( $dobras->getPrfId() ){
	$progFonte = $db->pegaUm("SELECT 
								prfdesc as descricao
							  FROM 
								obras.programafonte
							  WHERE
								prfid= ".$dobras->getPrfId());
}

$PDF->MultiCell( 19, 0.5, 'Programa/Fonte: ' . $progFonte, 1, 'J', 0);

if ( $dobras->getCloId() ){
	$classObra = $db->pegaUm("SELECT 
								clodsc as descricao
							 FROM
								obras.classificacaoobra
							 WHERE
								cloid = ".$dobras->getCloId());
}

$PDF->MultiCell( 19, 0.5, 'Classificação da Obra: ' . $classObra, 1, 'J', 0);

if ( $dobras->getTpoId() ){
	$tipoObra = $db->pegaUm("SELECT tpodsc as descricao 
							 FROM obras.tipologiaobra
							 WHERE tpoid = ".$dobras->getTpoId());
}

$PDF->MultiCell( 19, 0.5, 'Tipologia da Obra: ' . $tipoObra, 1, 'J', 0);
$PDF->MultiCell( 19, 0.5, 'Descrição / Composição da Obra: ' . $dobras->getObrComposicao(), 1, 'J', 0);
$PDF->MultiCell( 19, 0.5, 'Observação sobre a Obra: ' . $dobras->getObsObra(), 1, 'J', 0);

$percentualExecutado = $obras->ViewPercentualExecutado($dadoObrid);
if(!$percentualExecutado) { $percentualExecutado = 0; }
	$percentualExecutado = ( $percentualExecutado > 100.00 ) ? 100.00 : $percentualExecutado;

$percentualExecutado = number_format($percentualExecutado, 2, ',', '.');
$percentualExecutado.=" %";

$PDF->MultiCell( 19, 0.5, '(%) Concluído: ' . $percentualExecutado, 1, 'J', 0);

if ( $_REQUEST["localobra"] == "true" ){
	
	$PDF->SetFont( 'Arial', 'B', 8 ); 
	$PDF ->SetFillColor( 234, 234, 234 ); 
	$PDF->Cell( 19, 0.5, 'Local da Obra', 1, 0, 'C', 1 );
	$PDF->ln();
	
	$PDF->SetFont('Arial', '', 6);
	$PDF->MultiCell( 19, 0.5, 'CEP: ' 		   . $dobras->getEndCep(), 1, 'J', 0);
	$PDF->MultiCell( 19, 0.5, 'Logradouro: '   . $dobras->getEndLog(), 1, 'J', 0);
	$PDF->MultiCell( 19, 0.5, 'Número: ' 	   . $dobras->getEndNum(), 1, 'J', 0);
	$PDF->MultiCell( 19, 0.5, 'Bairro: ' 	   . $dobras->getEndBai(), 1, 'J', 0);
	$PDF->MultiCell( 19, 0.5, 'Município/UF: ' . $dobras->getMunDescricao()." / ".$dobras->getEstUf(), 1, 'J', 0);
		
}

if ( $_REQUEST["coordenadas"] == "true" ){
	
	$PDF->SetFont( 'Arial', 'B', 8 ); 
	$PDF ->SetFillColor( 234, 234, 234 ); 
	$PDF->Cell( 19, 0.5, 'Coordenadas Geográficas', 1, 0, 'C', 1 );
	$PDF->ln();
	
	$PDF->SetFont('Arial', '', 6);
	
	
	$latitude = explode(".", $dobras->getMedLatitude());
	$graulatitude = $latitude[0];
	$minlatitude = $latitude[1];
	$seglatitude = $latitude[2];
	$pololatitude = $latitude[3];
	
	$dadosLatitude = trim($graulatitude) != '' ? $graulatitude."° ".$minlatitude."' ".$seglatitude."'' "."  ".$pololatitude : '';
	
	$PDF->MultiCell( 19, 0.5, 'Latitude: ' . $dadosLatitude, 1, 'J', 0);
	
	$longitude = explode(".", $dobras->getMedLongitude());
	$graulongitude = $longitude[0];
	$minlongitude = $longitude[1];
	$seglongitude = $longitude[2];
	
	$dadosLatitude = trim($graulongitude) != '' ?  $graulongitude."° ".$minlongitude."' ".$seglongitude."''" : '';
	
	$PDF->MultiCell( 19, 0.5, 'Longitude: ' . $dadosLatitude, 1, 'J', 0);
	
}

if ( $_REQUEST["contatos"] == "true" ){
	
	$PDF->SetFont( 'Arial', 'B', 8 ); 
	$PDF ->SetFillColor( 234, 234, 234 ); 
	$PDF->Cell( 19, 0.5, 'Contatos', 1, 0, 'C', 1 );
	$PDF->ln();
	
	$PDF->SetFont('Arial', '', 6);
	
	$sql = "SELECT
				et.entnumcpfcnpj as cpf,
				et.entnome as nome,
				et.entemail as email,
				'(' || et.entnumdddcomercial || ') ' || et.entnumcomercial as telefone,
				tr.tprcdesc as tipo_desc
			FROM 
				obras.responsavelobra r 
			INNER JOIN 
				obras.responsavelcontatos rc ON r.recoid = rc.recoid 
			INNER JOIN 
				entidade.entidade et ON rc.entid = et.entid 
			LEFT JOIN 
				obras.tiporespcontato tr ON rc.tprcid = tr.tprcid
			WHERE 
				r.obrid = '". $dadoObrid . "'  AND 
				rc.recostatus = 'A'";
	
	$dadosContatosObra = $db->carregar( $sql );
	
	if ( $dadosContatosObra ){

		$PDF->Cell( 3, 0.5, 'CPF', 1, 0, 'C', 1 );
		$PDF->Cell( 5, 0.5, 'Nome do Responsável', 1, 0, 'C', 1 );
		$PDF->Cell( 5, 0.5, 'E-mail', 1, 0, 'C', 1 );
		$PDF->Cell( 3, 0.5, 'Telefone', 1, 0, 'C', 1 );
		$PDF->Cell( 3, 0.5, 'Tipo de Responsabilidade', 1, 0, 'C', 1 );
		$PDF->ln();
		
		$PDF->SetFont('Arial', '', 6); 
		$cor_linha = 1;
		
		foreach($dadosContatosObra as $contato) {
			
			($cor_linha % 2)? $cor_linha2 = '255,255,255' : $cor_linha2 = '245,245,235';
			
			$PDF ->SetFillColor($cor_linha2);
			$PDF->Cell( 3, 0.5, $contato['cpf'], 1, 0, 'C', 1 );
			$PDF->Cell( 5, 0.5, $contato['nome'], 1, 0, 'C', 1 );
			$PDF->Cell( 5, 0.5, $contato['email'], 1, 0, 'C', 1 );
			$PDF->Cell( 3, 0.5, $contato['telefone'], 1, 0, 'C', 1 );
			$PDF->Cell( 3, 0.5, $contato['tipo_desc'], 1, 0, 'C', 1 );
			$PDF->ln(); 
			
			$cor_linha ++;
			
		}
		
	}else{
		
		$PDF->MultiCell( 19, 0.5, 'Não existem contatos cadastrados para esta obra ', 1, 'C', 0);
		
	}
	
}

if ( $_REQUEST["contratacao"] == "true" ){
	
	
	$PDF->SetFont( 'Arial', 'B', 8 ); 
	$PDF ->SetFillColor( 234, 234, 234 ); 
	$PDF->Cell( 19, 0.5, 'Contratação', 1, 0, 'C', 1 );
	$PDF->ln();
	
	$PDF->SetFont('Arial', '', 6);
	
	$obra = $obras->ViewObra($dadoObrid);
	
	$PDF->MultiCell( 19, 0.5, 'Nome da Obra: ' . $obra['nome'], 1, 'J', 0);
	
	$obrcustocontrato = number_format($obra['obrcustocontrato'],2,',','.');
	$PDF->MultiCell( 19, 0.5, 'Valor do Contrato (R$): ' . $obrcustocontrato, 1, 'J', 0);
	
	$PDF->MultiCell( 19, 0.5, '(%) Concluído: ' . $percentualExecutado, 1, 'J', 0);
	
	$PDF->SetFont('Arial', 'B', 6);
	$PDF->MultiCell( 19, 0.5, 'Sobre a Obra', 1, 'J', 0);
	
	$PDF->SetFont('Arial', '', 6);
	
	if( $dobras->getTobraId() ){
		
		$tobadesc = $db->pegaUm("SELECT tobadesc AS descricao 
							     FROM obras.tipoobra
							     WHERE tobaid =".$dobras->getTobraId());
		
	}
	
	$PDF->MultiCell( 19, 0.5, 'Tipo de Obra: ' . $tobadesc, 1, 'J', 0);
	
	if( $dobras->getStoId() ){
		
		$stodesc = $db->pegaUm("SELECT stodesc AS descricao 
							   FROM obras.situacaoobra
							   WHERE stoid =".$dobras->getStoId());
		
	}
	
	$PDF->MultiCell( 19, 0.5, 'Situação da Obra: ' . $stodesc, 1, 'J', 0);
	
	$dtinicio = formata_data($dobras->getObrDtInicio(), "d/m/Y");
	$PDF->MultiCell( 19, 0.5, 'Início programado para: ' . $dtinicio, 1, 'J', 0);
	
	$dttermino = formata_data($dobras->getObrDtTermino(), "d/m/Y");
	$PDF->MultiCell( 19, 0.5, 'Término programado para: ' . $dttermino, 1, 'J', 0);
	
	if ( $dobras->getUmdIdObraConstruida() ){
		
		$umdeesc = $db->pegaUm("SELECT umdeesc AS descricao 
							  FROM obras.unidademedida
							  WHERE umdid =".$dobras->getUmdIdObraConstruida());
		
		
		$obrqtdconstruida = $dobras->getObrQtdConstruida();
		$obrqtdconstruida = number_format($obrqtdconstruida,2,',','.') ." ".$umdid;
		
		$dadosArea = !empty( $umdeesc ) && $obrqtdconstruida != '' ? $obrqtdconstruida . '/' . $umdeesc : '';
	}
	
	$PDF->MultiCell( 19, 0.5, 'Área/Quantidade a ser Construída: ' . $dadosArea, 1, 'J', 0);
	
	$custoUnidade = number_format($dobras->getObrCustoUnitQtdConstruida(),2,',','.');
	$PDF->MultiCell( 19, 0.5, 'Custo Unitário (R$): ' . $custoUnidade, 1, 'J', 0);
	
	$PDF->SetFont('Arial', 'B', 6);
	$PDF->MultiCell( 19, 0.5, 'Sobre a Inauguração', 1, 'J', 0);
	
	$PDF->SetFont('Arial', '', 6);
	
	$obrstatusinauguracao = $dobras->getObrStatusInauguracao();	
	if( $obrstatusinauguracao == "S" ) $inaugurada = "Não se Aplica";
	if( $obrstatusinauguracao == "N" ) $inaugurada = "Não Inaugurada";
	if( $obrstatusinauguracao == "I" ) $inaugurada = "Inaugurada";
	
	if( $dobras->getObrDtInauguracao() )				
		$inaugurada = " Data de Inauguração: " . formata_data($dobras->getObrDtInauguracao());
	if($dobras->getObrDtPrevInauguracao())				
		$inaugurada = "Data de Previsão da Inauguração: " . formata_data($dobras->getObrDtPrevInauguracao());
				
	$PDF->MultiCell( 19, 0.5, 'Inaugurada: ' . $inaugurada, 1, 'J', 0);
	
	$PDF->SetFont('Arial', 'B', 6);
	$PDF->MultiCell( 19, 0.5, 'Contratação da Obra', 1, 'J', 0);
	
	$PDF->SetFont('Arial', '', 6);
	
	$empresa = new Entidade($dobras->getEntIdEmpresaConstrutora());
	$entnomeempresa = $empresa->entnome;
	
	if ($dobras->getEntIdEmpresaConstrutora()){
	
		$sql = "SELECT
					entemail as email,
					entnumdddcomercial as ddd,
					entnumcomercial as telefone,
					ed.endlog || ' nº ' || ed.endnum || ', ' || ed.endbai || ' - ' || tm.mundescricao || ', ' || ed.estuf as endereco
				FROM 
					entidade.entidade e
				LEFT JOIN
					entidade.endereco ed ON e.entid = ed.entid
				LEFT JOIN
					territorios.municipio tm ON ed.muncod = tm.muncod 
				WHERE 
					e.entid = " . $dobras->getEntIdEmpresaConstrutora();
	
		$dados = $db->carregar($sql);
		
	}
	
	if ( is_array($dados) ){
		$emailempresa 	 = $dados[0]['email'];
		$enderecoempresa = $dados[0]['endereco'];
		$dddempresa 	 = $dados[0]['ddd'];
		$telefoneempresa = $dados[0]['telefone'];
		$naturezaempresa = $dados[0]['natureza'];
		
	}
	
	$telefone = !empty($dddempresa) && $telefoneempresa ? '(' . $dddempresa . ') ' . $telefoneempresa : '';
	
	$PDF->MultiCell( 19, 0.5, 'Empresa Contratada: ' . $entnomeempresa, 1, 'J', 0);
	$PDF->MultiCell( 19, 0.5, 'Endereço: ' . $enderecoempresa, 1, 'J', 0);
	$PDF->MultiCell( 19, 0.5, 'Telefone: ' . $telefone, 1, 'J', 0);
	$PDF->MultiCell( 19, 0.5, 'E-mail: ' . $emailempresa, 1, 'J', 0);

	$PDF->SetFont('Arial', 'B', 6);
	$PDF->MultiCell( 19, 0.5, 'Fases de Licitação', 1, 'J', 0);
	
	$sql = "SELECT 
				fl.*,
				tfl.tfldesc, tfl.tflordem   
			FROM 
				obras.faselicitacao fl 
			INNER JOIN 
				obras.tiposfaseslicitacao tfl ON fl.tflid = tfl.tflid
			WHERE 
				fl.obrid = '". $dadoObrid . "' AND 
				fl.flcstatus = 'A' 
			ORDER BY 
				tfl.tflordem";
	
	$dadoFaseLicitacao = $db->carregar( $sql );
	
	if ( $dadoFaseLicitacao ){
		
		$PDF->ln();
		$PDF->Cell( 3, 0.5, 'Fase', 1, 0, 'C', 1 );
		$PDF->Cell( 3, 0.5, 'Data', 1, 0, 'C', 1 );
		$PDF->ln();
		
		for( $i = 0; $i < count( $dadoFaseLicitacao ); $i++ ){
			
			$cor = ($i % 2) ? '255,255,255' : '245,245,235';
			
			$flcid 				 = $dadoFaseLicitacao[$i]['flcid'];
			$tflid 				 = $dadoFaseLicitacao[$i]['tflid'];
			$tfldesc 			 = $dadoFaseLicitacao[$i]['tfldesc'];
			$flcrecintermotivo   = $dadoFaseLicitacao[$i]['flcrecintermotivo'];
			$flcordservnum 		 = $dadoFaseLicitacao[$i]['flcordservnum'];
			$flcpubleditaldtprev = formata_data($dadoFaseLicitacao[$i]['flcpubleditaldtprev']);
			$flcdtrecintermotivo = formata_data($dadoFaseLicitacao[$i]['flcdtrecintermotivo']);
			$flcordservdt 		 = formata_data($dadoFaseLicitacao[$i]['flcordservdt']);
			$flchomlicdtprev 	 = formata_data($dadoFaseLicitacao[$i]['flchomlicdtprev']);
			$flcaberpropdtprev   = formata_data($dadoFaseLicitacao[$i]['flcaberpropdtprev']);

			
			if($tflid ==2){
				$flcdata = $flcpubleditaldtprev;
			}
			if($tflid ==5){
				$flcdata = $flcdtrecintermotivo;
			}
			if($tflid ==6){
				$flcdata = $flcordservdt;
			}
			if($tflid ==9){
				$flcdata = $flchomlicdtprev;
			}
			if($tflid ==7){
				$flcdata = $flcaberpropdtprev;
			}
			
			$PDF->SetFont('Arial', '', 6);
			$PDF->SetFillColor( $cor );
			$PDF->Cell( 3, 0.5, $tfldesc, 1, 0, 'C', 1 );
			$PDF->Cell( 3, 0.5, $flcdata, 1, 0, 'C', 1 );
			$PDF->ln();
		}
		
		$PDF->ln();
		
	}else{
		$PDF->MultiCell( 19, 0.5, 'Não existem fases de licitação cadastradas para esta obra ', 1, 'C', 0);
	}
	
	$PDF->SetFont('Arial', 'B', 6);
	$PDF->MultiCell( 19, 0.5, 'Forma de Repasse de Recursos', 1, 'J', 0);	
	
	$PDF->SetFont('Arial', '', 6);
	
	if( $dobras->getFrpId() ){
		
		$frpdesc = $db->pegaUm("SELECT frpdesc AS descricao 
								FROM obras.tipoformarepasserecursos
								WHERE frpid =".$dobras->getFrpId());

		$PDF->MultiCell( 19, 0.5, 'Tipo: ' . $frpdesc, 1, 'J', 0);
		
		switch ( $dobras->getFrpId() ) {
			
			case 2:
	
				$dadoConvenio = $db->pegaLinha("SELECT
													*
											    FROM 
											  		obras.conveniosobra
											    WHERE
											  		covid = '{$dobras->getCovId()}'");
			
				$dadoConvenio["covdtinicio"] = formata_data( $dadoConvenio["covdtinicio"] );
				$dadoConvenio["covdtfinal"]  = formata_data( $dadoConvenio["covdtfinal"] );
				
				$dadoConvenio["covvlrconcedente"] = number_format( $dadoConvenio["covvlrconcedente"], 2, ',', '.' );
				$dadoConvenio["covvlrconvenente"] = number_format( $dadoConvenio["covvlrconvenente"], 2, ',', '.' );
				$dadoConvenio["covvalor"] 		  = number_format( $dadoConvenio["covvalor"], 2, ',', '.' );
				
				$PDF->MultiCell( 19, 0.5, 'Nº do Convênio: ' . $dadoConvenio["covnumero"], 1, 'J', 0);
				$PDF->MultiCell( 19, 0.5, 'Ano: ' . $dadoConvenio["covano"], 1, 'J', 0);
				$PDF->MultiCell( 19, 0.5, 'Objeto: ' . $dadoConvenio["covobjeto"], 1, 'J', 0);
				$PDF->MultiCell( 19, 0.5, 'Detalhamento: ' . $dadoConvenio["covdetalhamento"], 1, 'J', 0);
				$PDF->MultiCell( 19, 0.5, 'Processo: ' . $dadoConvenio["covprocesso"], 1, 'J', 0);
				$PDF->MultiCell( 19, 0.5, 'Concedente: ' . $dadoConvenio["covvlrconcedente"], 1, 'J', 0);
				$PDF->MultiCell( 19, 0.5, 'Convenente: ' . $dadoConvenio["covvlrconvenente"], 1, 'J', 0);
				$PDF->MultiCell( 19, 0.5, 'Valor (R$): ' . $dadoConvenio["covvalor"], 1, 'J', 0);
				$PDF->MultiCell( 19, 0.5, 'Início: ' . $dadoConvenio["covdtinicio"], 1, 'J', 0);
				$PDF->MultiCell( 19, 0.5, 'Fim: ' . $dadoConvenio["covdtfinal"], 1, 'J', 0);
				
			break;
			
			case 3:
				
				$PDF->MultiCell( 19, 0.5, 'Instituição: ' . $dobras->getFrrDescInstituicao(), 1, 'J', 0);
				$PDF->MultiCell( 19, 0.5, 'Nº da Portaria de Descentralização: ' . $dobras->getFrrDescNumPort(), 1, 'J', 0);
				$PDF->MultiCell( 19, 0.5, 'Objeto: ' . $dobras->getFrrDescObjeto(), 1, 'J', 0);
				
				$frrdescvlr = number_format($dobras->getFrrDescVlr(), 2, ',', '.');
				$PDF->MultiCell( 19, 0.5, 'Valor (R$): ' . $frrdescvlr, 1, 'J', 0);
				
			break;
			
			default:
				$PDF->MultiCell( 19, 0.5, 'Recurso Próprio: ' . 	$dobras->getFrrObsRecProprio(), 1, 'J', 0);
			break;
			
		}
		
	}else{
		$PDF->MultiCell( 19, 0.5, 'Não existem forma de repasse de recursos cadastradas para esta obra ', 1, 'C', 0);	
	}
	
	
	
}

if ( $_REQUEST["infra"] == "true" ){
	
	$infraestrutura = new DadosInfraEstrutura();
	$resultado = $infraestrutura->busca($dadoObrid);	
	$dados = $infraestrutura->dados($resultado);
	
	$PDF->SetFont( 'Arial', 'B', 8 ); 
	$PDF ->SetFillColor( 234, 234, 234 ); 
	$PDF->Cell( 19, 0.5, 'Infra-Estrutura', 1, 0, 'C', 1 );
	$PDF->ln();
	
	$PDF->SetFont('Arial', '', 6);
	
	$iexsitdominialimovelregulariza = $infraestrutura->iexsitdominialimovelregulariza == "t" ? 'Sim' : 'Não';
	$PDF->MultiCell( 19, 0.5, 'Situação Dominial já Regularizada? ' . $iexsitdominialimovelregulariza, 1, 'J', 0);
	
	$iexinfexistedimovel = $infraestrutura->iexinfexistedimovel == "t" ? 'Sim' : 'Não';
	$PDF->MultiCell( 19, 0.5, 'Existem Edificações no Local da Obra? '. $iexinfexistedimovel, 1, 'J', 0);
	
	if ($infraestrutura->umdidareaconstruida){
		$umdidareaconstruida = $db->pegaUm("SELECT 
												umdid AS codigo, 
												umdeesc AS descricao 
											FROM 
												obras.unidademedida
											WHERE umdid=".$infraestrutura->umdidareaconstruida);
		
		$iexareaconstruida = number_format($infraestrutura->iexareaconstruida,2,',','.') . " " . $umdidareaconstruida;
	}
	
	$PDF->MultiCell( 19, 0.5, 'Área Construída: '. $iexareaconstruida, 1, 'J', 0);
	
	$PDF->MultiCell( 19, 0.5, 'Descrição Sumária da Edificação: ' . $infraestrutura->iexdescsumariaedificacao, 1, 'J', 0);
	
	$iexqtdareapreforma = $infraestrutura->iexqtdareapreforma == "t" ? 'Sim' : 'Não';
	$PDF->MultiCell( 19, 0.5, 'A(s) Edificaçõe(s) Necessita(m) de Reforma(s)? ' . $iexqtdareapreforma, 1, 'J', 0);
	
	$iexqtdareaampliada = $infraestrutura->iexqtdareaampliada == "t" ? 'Sim' : 'Não';
	$PDF->MultiCell( 19, 0.5, 'Há Necessidade de Ampliação? ' . $iexqtdareaampliada, 1, 'J', 0);
	
}

if ( $_REQUEST["projetos"] == "true" ){
	
	$faseprojeto = new DadosFasesProjeto();
	$resultado = $faseprojeto->busca($dadoObrid);	
	$dados = $faseprojeto->dados($resultado);
	
	
	$PDF->SetFont( 'Arial', 'B', 8 ); 
	$PDF ->SetFillColor( 234, 234, 234 ); 
	$PDF->Cell( 19, 0.5, 'Projetos', 1, 0, 'C', 1 );
	$PDF->ln();
	
	$PDF->SetFont('Arial', '', 6);

	if ( $faseprojeto->tpaid ){
		$tpadesc = $db->pegaUm("SELECT 
									tpadesc as descricao 
								FROM 
									obras.tipoprojetoarquitetonico
								WHERE tpaid=" . $faseprojeto->tpaid);
		
	}
	
	$PDF->MultiCell( 19, 0.5, 'Tipo de Projeto: ' . $tpadesc, 1, 'J', 0);
	
	if ( $faseprojeto->felid ){
		$feldesc = $db->pegaUm("SELECT 
									feldesc as descricao 
								FROM 
									obras.formaelaboracao
								WHERE felid = " . $faseprojeto->felid);
	}
	
	$PDF->MultiCell( 19, 0.5, 'Forma de Elaboração do Projeto: ' . $feldesc, 1, 'J', 0);
	
	switch ( $faseprojeto->felid ) {
		
		case 3:
			
			$fprvlrformaelabrecproprio    = number_format( $faseprojeto->fprvlrformaelabrecproprio, 2, ',', '.' );
			$fprvlrformaelabrrecrepassado = number_format( $faseprojeto->fprvlrformaelabrrecrepassado, 2, ',', '.' );
			
			$PDF ->SetFillColor( 255, 255, 255 );
			$PDF->Cell( 9.5, 0.5, 'Recurso Próprio (R$): ' . $fprvlrformaelabrecproprio, 1, 0, 'J', 1 );
			$PDF->Cell( 9.5, 0.5, 'Recurso Repassado (R$): '. $fprvlrformaelabrrecrepassado, 1, 0, 'J', 1 );
			$PDF->ln(); 
			
		break;
		default:
			$PDF->MultiCell( 19, 0.5, 'Observações: ' . $faseprojeto->fprobsexecdireta, 1, 'J', 0);
		break;
		
	}
	
	if ( $faseprojeto->tfpid ){
		$tfpdesc = $db->pegaUm("SELECT 
									tfpdesc as descricao 
								FROM 
									obras.tipofaseprojeto
								WHERE tfpid = " . $faseprojeto->tfpid);
	}
	
	$PDF->MultiCell( 19, 0.5, 'Fases do Projeto: ' . $tfpdesc, 1, 'J', 0);
	
	switch ( $faseprojeto->tfpid ){
		
		case 1:
			$dtprevisao = formata_data($faseprojeto->fprdtiniciofaseprojeto);
		break;
		case 2:
			$dtprevisao = formata_data($faseprojeto->fprdtprevterminoprojeto);
		break;
		case 3:
			$dtprevisao = formata_data($faseprojeto->fprdtconclusaofaseprojeto);
		break;
		
	}

	
	$PDF->MultiCell( 19, 0.5, 'Previsão/Conclusão: ' . $dtprevisao, 1, 'J', 0);
	
}

if ( $_REQUEST["etapas"] == "true" ){
	
	$PDF->SetFont( 'Arial', 'B', 8 ); 
	$PDF ->SetFillColor( 234, 234, 234 ); 
	$PDF->Cell( 19, 0.5, 'Etapas da Obra', 1, 0, 'C', 1 );
	$PDF->ln();
	
	$PDF->SetFont('Arial', '', 6);
	
	$sql = "SELECT 
				i.itcid,
				i.icovlritem,
				i.icopercsobreobra,
				i.icopercexecutado,
				ic.itcdesc,
				ic.itcdescservico
			FROM 
				obras.itenscomposicaoobra i,
				obras.itenscomposicao ic 
			WHERE 
				i.obrid = " . $dadoObrid . " 
				and i.itcid = ic.itcid 
			ORDER BY 
				i.icoordem";
	
	$dadoEtapas = $db->carregar($sql);
	
	$count = 1;
	$soma  = 0;
	$somav = 0;
	
	$controleLinha = 1;

	if ( $dadoEtapas ){

		$PDF->Cell( 8, 0.5, 'Descrição', 1, 0, 'C', 1 );
		$PDF->Cell( 6, 0.5, 'Valor do Item (R$)', 1, 0, 'C', 1 );
		$PDF->Cell( 5, 0.5, '(%) Referente a Obra', 1, 0, 'C', 1 );
		$PDF->ln();
		
		$PDF->SetFont('Arial', '', 6);
		
		foreach( $dadoEtapas as $dado ){

			$icovlritem = $dado['icovlritem'];
			$itcdesc 	= $dado['itcdesc'];
			$icopercsobreobra = $dado['icopercsobreobra'];
			$icopercexecutado = $dado['icopercexecutado'];
			$itcdescservico   = $dado['itcdescservico'];
			
			$somav 		= bcadd( $somav, $icovlritem, 2 );
			$icovlritem = number_format( $icovlritem, 2, ',', '.' ); 
			$soma 		= round( $soma, 2 ) + round( $icopercsobreobra, 2 );
			
			$icopercsobreobra = number_format( $icopercsobreobra, 2 );
			$icopercsobreobra = str_replace( '.', ',', $icopercsobreobra );
			
			($controleLinha % 2)? $cor_linha = '255,255,255' : $cor_linha = '245,245,235';
			
			$PDF->SetFillColor( $cor_linha );
			
			$PDF->Cell( 8, 0.5, $itcdesc, 1, 0, 'C', 1 );
			$PDF->Cell( 6, 0.5, $icovlritem, 1, 0, 'C', 1 );
			$PDF->Cell( 5, 0.5, $icopercsobreobra, 1, 0, 'C', 1 );
			$PDF->ln();
			
			$controleLinha++;
			
		}
		
		$somav = number_format( $somav, 2, ',', '.' );
		$soma  = number_format( $soma, 2, ',', '.' );
		
		$PDF->SetFont('Arial', 'B', 6);
		$PDF->SetFillColor( 255, 255, 255 );
		
		$PDF->Cell( 8, 0.5, 'Total', 1, 0, 'C', 1 );
		$PDF->Cell( 6, 0.5, $somav, 1, 0, 'C', 1 );
		$PDF->Cell( 5, 0.5, $soma, 1, 0, 'C', 1 );
		$PDF->ln();
		
	}else{
		
		$PDF->MultiCell( 19, 0.5, 'Não existem etapas cadastradas para esta obra ', 1, 'C', 0);
		
	}
	
}

if ( $_REQUEST["cronograma"] == "true" ){
	
	$PDF->SetFont( 'Arial', 'B', 8 ); 
	$PDF ->SetFillColor( 234, 234, 234 ); 
	$PDF->Cell( 19, 0.5, 'Cronograma Físico-Financeiro', 1, 0, 'C', 1 );
	$PDF->ln();
	
	$sql = "SELECT
				oi.obrpercexec,
				itco.icoid,
				itc.itcid,
				itc.itcdesc,
				itc.itcdescservico,
				itco.icopercsobreobra,
				itco.icovlritem,
				itco.icodtinicioitem,
				itco.icodterminoitem,
				itco.icopercexecutado
			FROM 
				obras.itenscomposicao itc
			INNER JOIN 
				obras.itenscomposicaoobra itco ON itc.itcid = itco.itcid
			INNER JOIN
				obras.obrainfraestrutura oi ON oi.obrid = itco.obrid
			WHERE 
				itco.obrid = ".$dadoObrid."
			ORDER BY 
				itco.icoordem";
	
	$dadoCronograma = $db->carregar( $sql );
	
	$controleLinha = 1;
	
	if( $dadoCronograma ){

		$PDF->SetFont('Arial', '', 6);
		
		$PDF->Cell( 3.5, 0.5, 'Item da Obra', 1, 0, 'C', 1 );
		$PDF->Cell( 2.5, 0.5, '% Sobre a Obra (A)', 1, 0, 'C', 1 );
		$PDF->Cell( 2, 0.5, 'Valor (R$)', 1, 0, 'C', 1 );
		$PDF->Cell( 2, 0.5, 'Data de Início', 1, 0, 'C', 1 );
		$PDF->Cell( 2, 0.5, 'Data de Término', 1, 0, 'C', 1 );
		$PDF->Cell( 3.5, 0.5, '% Executado Sobre a Obra (B)', 1, 0, 'C', 1 );
		$PDF->Cell( 3.5, 0.5, '% do Item Executado (B x 100 / A)', 1, 0, 'C', 1 );
		$PDF->ln();
		
		$PDF->SetWidths( array( 3.5, 2.5, 2, 2, 2, 3.5, 3.5 ) );
		$PDF->SetAligns( array( 'L', 'C', 'C', 'C', 'C', 'C', 'C' ) );
		$PDF->SetHeight( 1 );
		
		foreach( $dadoCronograma as $dado ){
			
			$icopercexecutado   = $icopercexecutado + $dado['icopercsobreobra'];
			$icovlritem 	    = $icovlritem + $dado['icovlritem'];
			$porcento_executado = $dado['obrpercexec'];	
			$icopercsobreobra   = $icopercsobreobra + $dado['icopercsobreobra'];
			
			if ( $dado['icopercsobreobra'] > 0.00 ){
				$execItem = ( $dado['icopercsobreobra'] * 100 ) / $dado['icopercsobreobra'];
			}else{
				$execItem = 0.00;
			}
			
			$sobreObra = number_format( $dado['icopercsobreobra'], 2, ',', '.' );
			$valorItem = number_format( $dado['icovlritem'], 2, ',', '.' );
			$dtInicio  = formata_data( $dado['icodtinicioitem'] );
			$dtTermino = formata_data( $dado['icodterminoitem'] );
			$execObra  = number_format( $dado['icopercsobreobra'], 2, ',', '.' );
			
			$execItem  = number_format( $execItem, 2, ',', '.' );
			
			($controleLinha % 2)? $cor_linha = '255,255,255' : $cor_linha = '245,245,235';
			
			$PDF->SetFillColor( $cor_linha );
			
			$PDF->Row( array($dado['itcdesc'], $sobreObra, $valorItem, $dtInicio, $dtTermino, $execObra, $execItem ), true, true );
			
			$controleLinha++;
			
		}
		
		$PDF->SetFont('Arial', 'B', 6);
		$PDF->SetFillColor( 255, 255, 255 );
		
		$icopercexecutado 	= number_format( $icopercexecutado ,2, ',', '.' );
		$icovlritem 		= number_format( $icovlritem, 2, ',', '.' );
		$porcento_executado = number_format( $porcento_executado, 2, ',', '.' );
		
		$PDF->Cell( 3.5, 0.5, 'Total', 1, 0, 'C', 1 );
		$PDF->Cell( 2.5, 0.5, $icopercexecutado, 1, 0, 'C', 1 );
		$PDF->Cell( 2, 0.5, $icovlritem, 1, 0, 'C', 1 );
		$PDF->Cell( 2, 0.5, '', 1, 0, 'C', 1 );
		$PDF->Cell( 2, 0.5, '', 1, 0, 'C', 1 );
		$PDF->Cell( 3.5, 0.5, '', 1, 0, 'C', 1 );
		$PDF->Cell( 3.5, 0.5, $porcento_executado, 1, 0, 'C', 1 );
		$PDF->ln();
		
	}else{
		
		$PDF->SetFont('Arial', '', 6);
		$PDF->MultiCell( 19, 0.5, 'Não existem cronogramas cadastrados para esta obra ', 1, 'C', 0);
		
	}
	
}

if( $_REQUEST["galeria"] == "true" ){
	
	$PDF->SetFont( 'Arial', 'B', 8 ); 
	$PDF ->SetFillColor( 234, 234, 234 ); 
	$PDF->Cell( 19, 0.5, 'Galeria de Fotos', 1, 0, 'C', 1 );
	$PDF->ln();

	if ( $_REQUEST["sel_fotos_galeria"] != "0" ){
			
		$sql = "SELECT 
					arqnome, 
					arq.arqid, 
					arq.arqextensao, 
					arq.arqtipo, 
					arq.arqdescricao 
				FROM 
					public.arquivo arq
				INNER JOIN 
					obras.arquivosobra oar ON arq.arqid = oar.arqid
				INNER JOIN 
					obras.obrainfraestrutura obr ON obr.obrid = oar.obrid 
				INNER JOIN 
					seguranca.usuario seg ON seg.usucpf = oar.usucpf 
				WHERE 
					arq.arqid IN (".$_REQUEST["sel_fotos_galeria"].") AND 
					obr.obrid = {$dadoObrid} AND
				  	aqostatus = 'A' AND
				  	(arqtipo = 'image/jpeg' OR 
				  	arqtipo = 'image/gif' OR 
				  	arqtipo = 'image/png') 
				ORDER BY 
					arq.arqid";
		
	} else {
		
		$sql = "SELECT 
					arqnome, 
					arq.arqid, 
					arq.arqextensao, 
					arq.arqtipo, 
					arq.arqdescricao 
				FROM 
					public.arquivo arq
				INNER JOIN 
					obras.arquivosobra oar ON arq.arqid = oar.arqid
				INNER JOIN 
					obras.obrainfraestrutura obr ON obr.obrid = oar.obrid 
				INNER JOIN 
					seguranca.usuario seg ON seg.usucpf = oar.usucpf 
				WHERE 
					obr.obrid = {$dadoObrid} AND
				  	aqostatus = 'A' AND
				  	(arqtipo = 'image/jpeg' OR 
				   	arqtipo = 'image/gif' OR 
				   	arqtipo = 'image/png') 
				ORDER BY 
					arq.arqid DESC LIMIT " . $_REQUEST["num_fotos_galeria"];
		
	}
	
	$fotos = ($db->carregar($sql));

	if( $fotos ){
		
		$total_fotos = count($fotos);
		$PDF->ln(0.5);
		$PDF->ln(0.5); 
		$y = $PDF->GetY();
		$coluna = 2;
		$largura_max = 2.0;
		$altura_max = 2.0;
		$linha = 1;
		
		for( $i=0; $i < count($fotos); $i++ ){

			$caminho = APPRAIZ . 'arquivos/obras/'. floor((int)$fotos[$i]['arqid']/1000) .'/'. $fotos[$i]['arqid'];
			
			if( file_exists($caminho) ){
				
				$novo_arquivo = APPRAIZ . 'arquivos/obras/'. floor((int)$fotos[$i]['arqid']/1000) .'/'.$fotos[$i]['arqid'].'_01.'.$fotos[$i]['arqextensao'];
				copy( $caminho, $novo_arquivo );
			
				if($coluna <= 19 ){
					( ($largura/100) > $largura_max ) ? $larg = $largura_max : $larg = ($largura/100);
					$coluna += $larg + 0.5;
				}
				if( $coluna > 19 ){
					$coluna = 1;
					$y = $y + 3.5;
					$linha++;
				}
				if( $y > 22 ){
					$PDF->AddPage();
					$y = $PDF->GetY();
					$linha = 1;
				}
				
				list( $largura, $altura ) = getimagesize( $novo_arquivo );
				
				( ($largura/100) > $largura_max ) ? $larg = $largura_max : $larg = ( $largura/100 );
				( ($altura/100)  > $altura_max )  ? $alt  = $altura_max  : $alt  = ( $altura/100 );
				
				$PDF->Image( $novo_arquivo, $coluna, $y, $larg, $alt );
				
				unlink($novo_arquivo);
		
			}
				
		}
		
		$PDF->ln($linha + 4);
			
	} else {
		
		$PDF->SetFont('Arial', '', 6);
		$PDF->MultiCell( 19, 0.5, 'Não existem fotos anexadas a esta obra ', 1, 'C', 0);
		
	}	
	
}

if ( $_REQUEST["vistoria"] == "true" ){
	
	$PDF->SetFont( 'Arial', 'B', 8 ); 
	$PDF ->SetFillColor( 234, 234, 234 ); 
	$PDF->Cell( 19, 0.5, 'Vistorias', 1, 0, 'C', 1 );
	$PDF->ln();
	
	$id_vistoria = explode( "_", $_REQUEST["vistorias"], -1 );
	
	foreach( $id_vistoria as $valor ){
		
		$valor2 = explode( ".", $valor );
		$fotos_vistoria[$valor2[0]] = $valor2[1];
		$id_vistorias .= $valor2[0] . ",";
		
	}
	
	$id_vistorias .= "0";
	
	if ($id_vistorias!=",0"){	
	
		$sql = "SELECT
					s.*,
					to_char(s.supvdt,'DD/MM/YYYY') as dtvistoria,
					to_char(s.supdtinclusao,'DD/MM/YYYY') as dtinclusao,						
					u.usunome,
					si.stodesc,
					s.suprealizacao as responsavel,
					s.supvid,
					s.usucpf
				FROM
					obras.supervisao s
				INNER JOIN 
					obras.situacaoobra si ON si.stoid = s.stoid
				INNER JOIN
					seguranca.usuario u ON u.usucpf = s.usucpf
				WHERE
					s.supvid IN (" . $id_vistorias . ") AND
					s.obrid = '" . $dadoObrid . "' AND
					s.supstatus = 'A'
				ORDER BY 
					s.supdtinclusao ASC";
	
		$dadosVistoria = $db->carregar($sql);
		
		$selecao_fotos_vistoria = explode( "_", $_REQUEST["selecao_fotos_vistoria"] );
					
		foreach ( $selecao_fotos_vistoria as $valor ){
			
			$valor2 = explode( ".", $valor );
			
			if ( $valor2[0] != "" ){
				$selecao_fotos_vistoria_array[$valor2[0]] = $valor2[1];
			}
			
		}
	
		if ( $dadosVistoria ){
			
			for( $i = 0; $i < count($dadosVistoria); $i++){

				$ordem = $i + 1;
				$supvid = $dadosVistoria[$i]["supvid"];
				
				$PDF->SetFont( 'Arial', 'B', 7 ); 
				$PDF->Cell( 19, 0.5, 'Vistoria n°' . $ordem, 1, 0, 'J', 1 );
				$PDF->ln();
				
				$PDF->SetFont( 'Arial', '', 6 ); 
				$PDF->MultiCell( 19, 0.5, 'Data da Vistoria: ' . $dadosVistoria[$i]["dtvistoria"], 1, 'J', 0);
				$PDF->MultiCell( 19, 0.5, 'Data  de Inclusão: ' . $dadosVistoria[$i]["dtinclusao"], 1, 'J', 0);
				$PDF->MultiCell( 19, 0.5, 'Responsável: ' . $dadosVistoria[$i]["responsavel"], 1, 'J', 0);
				$PDF->MultiCell( 19, 0.5, 'Situação da Obra: ' . $dadosVistoria[$i]["stodesc"], 1, 'J', 0);
				$PDF->MultiCell( 19, 0.5, 'Realizada Por: ' . $dadosVistoria[$i]["usunome"], 1, 'J', 0);
				
				$supprojespecificacoes = $dadosVistoria[$i]["supprojespecificacoes"] == 't' ? 'Sim' : 'Não';
				$PDF->MultiCell( 19, 0.5, 'Projeto/Especificações: ' . $supprojespecificacoes, 1, 'J', 0);
				
				$supplacaobra = $dadosVistoria[$i]["supplacaobra"] == 't' ? 'Sim' : 'Não';
				$PDF->MultiCell( 19, 0.5, 'Placa da Obra: ' . $supplacaobra, 1, 'J', 0);
				
				$supdiarioobra = $dadosVistoria[$i]["supdiarioobra"] == 't' ? 'Sim' : 'Não';
				$PDF->MultiCell( 19, 0.5, 'Diário da Obra Atualizado: ' . $supdiarioobra, 1, 'J', 0);
				
				$supplacalocalterreno = $dadosVistoria[$i]["supplacalocalterreno"] == 't' ? 'Sim' : 'Não';
				$PDF->MultiCell( 19, 0.5, 'Placa Indicativa do Programa/Dados da obra: ' . $supplacalocalterreno, 1, 'J', 0);
				
				$supvalidadealvara = $dadosVistoria[$i]["supvalidadealvara"] == 't' ? 'Sim' : 'Não';
				$PDF->MultiCell( 19, 0.5, 'Validade do Alvará da Obra: ' . $supvalidadealvara, 1, 'J', 0);
				
				if ( $dadosVistoria[$i]["qlbid"] ){
					
					$qlbdesc = $db->pegaUm("SELECT 
												qlbdesc as descricao 
											FROM 
												obras.qualidadeobra
											WHERE 
												qlbid = " . $dadosVistoria[$i]["qlbid"]);  
					
				}
				
				$PDF->MultiCell( 19, 0.5, 'Qualidade de Execução da Obra/Projeto: ' . $qlbdesc, 1, 'J', 0);
				
				if ( $dadosVistoria[$i]["dcnid"] ){
					
					$dcndesc = $db->pegaUm("SELECT 
												dcndesc as descricao 
											FROM 
												obras.desempenhoconstrutora
											WHERE 
												dcnid = " . $dadosVistoria[$i]["dcnid"]);   
					
				}
				$PDF->MultiCell( 19, 0.5, 'Desempenho da Construtora/Projetista: ' . $dcndesc, 1, 'J', 0);
				
				$PDF->SetFont( 'Arial', 'B', 5 ); 
				$PDF->SetFillColor( 234, 234, 234 ); 
				

				$sql = "SELECT
						itco.icoid,
						itc.itcdesc,
						itco.icovlritem,
						itco.icopercsobreobra, 
						itco.icodtinicioitem,
						itco.icodterminoitem,
						itco.icopercexecutado,
						sup.supvlrinfsupervisor,
						sup.supvlritemexecanterior,
						sup.supvlritemsobreobraexecanterior,
						sup.supvid								
					FROM 
						obras.itenscomposicao itc
					INNER JOIN 
						obras.itenscomposicaoobra itco ON itc.itcid = itco.itcid
					INNER JOIN
						obras.supervisao s ON s.obrid = itco.obrid AND s.supdtinclusao = ( SELECT 
																								max(ss.supdtinclusao) 
																						   FROM 
																						   		obras.supervisao ss 
																						   WHERE 
																						   		ss.obrid = itco.obrid AND 
																						   		ss.supstatus = 'A' )
					LEFT JOIN
						obras.supervisaoitenscomposicao sup ON sup.supvid = s.supvid AND sup.icoid = itco.icoid
					WHERE
						itco.obrid = ".$dadoObrid."
					GROUP BY
						itco.icoordem,
						itco.icoid,
						itc.itcdesc,
						itco.icovlritem,
						itco.icopercsobreobra, 
						itco.icodtinicioitem,
						itco.icodterminoitem,
						itco.icopercexecutado,
						sup.supvlrinfsupervisor,
						sup.supvlritemexecanterior,
						sup.supvlritemsobreobraexecanterior,
						sup.supvid
					ORDER BY 
						itco.icoordem";
				
				$dadosItensVistoria = $db->carregar( $sql );
				
				imprimirCabecalhoTabelaVistoria( $PDF );
				
				$PDF->SetWidths( array( 2, 2, 2, 2, 2, 2.25, 2.25, 2.25, 2.25 ) );
				$PDF->SetAligns( array( 'L', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C' ) );
				$PDF->SetHeight( 1.5 );
				
//				ver( $dadosItensVistoria );
				
				for( $k = 0; $k < count($dadosItensVistoria); $k++  ){

					$supervisao                 = ( isset($dadosItensVistoria[$k]["supvlrinfsupervisor"]) ) 		    ? $dadosItensVistoria[$k]['supvlrinfsupervisor']			   : 0;
					$exec_anterior              = ( isset($dadosItensVistoria[$k]["supvlritemexecanterior"]) ) 		    ? $dadosItensVistoria[$k]['supvlritemexecanterior']		   : 0;
					$exec_anterior_sobre_obra   = ( isset($dadosItensVistoria[$k]["supvlritemsobreobraexecanterior"]) ) ? $dadosItensVistoria[$k]['supvlritemsobreobraexecanterior'] : 0;
					$perc_sobre_obra            = ( isset($dadosItensVistoria[$k]["icopercsobreobra"]) ) 			    ? $dadosItensVistoria[$k]['icopercsobreobra']				   : 0;
					
					// Valores do % do item executado sobre a obra
					$supervisao_exec_sobre_obra = ( ((float)$supervisao * (float)$perc_sobre_obra) / 100 );
					$supervisao_exec_sobre_obra2 = number_format($supervisao_exec_sobre_obra, 2, ',', '.');

					$vlrSupervisao = !isset($supvid) ? $exec_anterior : $supervisao;
					
					($k % 2)? $cor_linha = '245,245,235' : $cor_linha = '255,255,255';
			
					$PDF->SetFont( 'Arial', '', 5 ); 
					$PDF->SetFillColor( $cor_linha );
					
					$icovlritem 			    = number_format( $dadosItensVistoria[$k]["icovlritem"], 2, ',', '.' );
					$perc_sobre_obra		    = number_format( $perc_sobre_obra, 2, ',', '.' );
					$exec_anterior 			    = number_format( $exec_anterior, 2, ',', '.' );
					$exec_anterior_sobre_obra   = number_format( $exec_anterior_sobre_obra, 2, ',', '.' );
					$vlrSupervisao 			    = number_format( $vlrSupervisao, 2, ',', '.' );
					$supervisao_exec_sobre_obra = number_format( $supervisao_exec_sobre_obra, 2, ',', '.' );
					
					$PDF->Row(array( $dadosItensVistoria[$k]["itcdesc"], $icovlritem, $perc_sobre_obra, $dadosItensVistoria[$k]["icodtinicioitem"], $dadosItensVistoria[$k]["icodterminoitem"], $exec_anterior, $exec_anterior_sobre_obra, $vlrSupervisao, $supervisao_exec_sobre_obra), true, true );
				}
				$PDF->ln(1);
				
			}
		}
		
	}
	
}

$PDF->Output();

function imprimirCabecalhoTabelaVistoria( $PDF ){
	
	if( $PDF->GetY() > 20 ){
		$PDF->AddPage();
	}
	
	$PDF->Cell( 2, 0.5, 'Item da Obra', 1, 0, 'C', 1 );
	$PDF->Cell( 2, 0.5, 'Valor (R$)', 1, 0, 'C', 1 );
	$PDF->Cell( 2, 0.5, '% Sobre a Obra', 1, 0, 'C', 1 );
	$PDF->Cell( 2, 0.5, 'Data de Início', 1, 0, 'C', 1 );
	$PDF->Cell( 2, 0.5, 'Data de Término', 1, 0, 'C', 1 );
	$PDF->Cell( 4.5, 0.5, 'Última Supervisão', 1, 0, 'C', 1 );
	$PDF->Cell( 4.5, 0.5, 'Supervisão Atual', 1, 0, 'C', 1 );
	$PDF->ln();
	
	$PDF->Cell( 2, 0.5, '', 1, 0, 'C', 1 );
	$PDF->Cell( 2, 0.5, '', 1, 0, 'C', 1 );
	$PDF->Cell( 2, 0.5, '', 1, 0, 'C', 1 );
	$PDF->Cell( 2, 0.5, '', 1, 0, 'C', 1 );
	$PDF->Cell( 2, 0.5, '', 1, 0, 'C', 1 );
	$PDF->Cell( 2.25, 0.5, '% do Item Executado', 1, 0, 'C', 1 );
	$PDF->Cell( 2.25, 0.5, '% Exec. Sobre a Obra', 1, 0, 'C', 1 );
	$PDF->Cell( 2.25, 0.5, '% Supervisão', 1, 0, 'C', 1 );
	$PDF->Cell( 2.25, 0.5, '% Após Supervisão', 1, 1, 'C', 1 );	
	
}

?>
