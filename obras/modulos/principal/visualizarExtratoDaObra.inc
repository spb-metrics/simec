<?php

ini_set("memory_limit","250M");
set_time_limit(0);
/**
 * Arquivo que gera o Extrato da obra
 * 
 * @author Fernando Araújo Bagno da SIlva
 * @since 11/02/2010
 * 
 */

// Inclusão de arquivos do componente de Entidade 
require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

if ( $_GET['obrid'] ){
	$_REQUEST = Array(
						    "requisicao" => "visualizar",
						    "buscanaoAgrupador" => "Pesquisar campo...",
						    "agrupador" => Array(
										            "0" => "contatos",
										            "1" => "localobra",
										            "2" => "projetos",
										            "3" => "contratacao",
										            "4" => "etapasobra",
											        ),
						
						    "coordenada" => 1,
						    "mapa" => 1,
						    "foto" => 1,
						    "numfotos" => "", 
						    "fotoseleciona" => "", 
						    "vistoria" => 1,
						);
	$obrid = $_GET['obrid'];
	
	extract( $_REQUEST );
	
	$supvids = str_replace("}{",",",$supvids);
	$supvids = str_replace("{","",$supvids);
	$supvids = str_replace("}","",$supvids);
	
	$fotoselecionadas = str_replace("}{",",",$fotoselecionadas);
	$fotoselecionadas = str_replace("{","",$fotoselecionadas);
	$fotoselecionadas = str_replace("}","",$fotoselecionadas);
	
	$obras  = new Obras();
	$dados  = $obras->Dados($obrid, '', 'I');
	$dobras = new DadosObra($dados);
}else{
	extract( $_REQUEST );
	
	$supvids = str_replace("}{",",",$supvids);
	$supvids = str_replace("{","",$supvids);
	$supvids = str_replace("}","",$supvids);
	
	$fotoselecionadas = str_replace("}{",",",$fotoselecionadas);
	$fotoselecionadas = str_replace("{","",$fotoselecionadas);
	$fotoselecionadas = str_replace("}","",$fotoselecionadas);
	
	$obras  = new Obras();
	$dados  = $obras->Dados($obrid);
	$dobras = new DadosObra($dados);
}

foreach( $agrupador as $valor ){
	
	switch ( $valor ){
		case "localobra":
		
			$dadosExtratoObras .= "<tr>"
							   . "    <td class='subtitulocentro' colspan='2' height='25px;'>Local da Obra</td>"
							   . "</tr>"
							   . "<tr>"
							   . "    <td class='subtitulodireita' style='font-weight: bold;'>CEP</td>"
							   . "    <td bgcolor='#f5f5f5'>{$dobras->getEndCep()}</td>"
							   . "</tr>"
							   . "<tr>"
							   . "    <td class='subtitulodireita' style='font-weight: bold;'>Logradouro</td>"
							   . "    <td>{$dobras->getEndLog()}</td>"
							   . "</tr>"
							   . "<tr>"
							   . "    <td class='subtitulodireita' style='font-weight: bold;'>Número</td>"
							   . "    <td bgcolor='#f5f5f5'>" . ($dobras->getEndNum() ? $dobras->getEndNum() : "Não Informado") . "</td>"
							   . "</tr>"
							   . "<tr>"
							   . "    <td class='subtitulodireita' style='font-weight: bold;'>Complemento</td>"
							   . "    <td>" . ($dobras->getEndCom() ? $dobras->getEndCom() : "Não Informado") . "</td>"
							   . "</tr>"
							   . "<tr>"
							   . "    <td class='subtitulodireita' style='font-weight: bold;'>Bairro</td>"
							   . "    <td bgcolor='#f5f5f5'>{$dobras->getEndBai()}</td>"
							   . "</tr>"
							   . "<tr>"
							   . "    <td class='subtitulodireita' style='font-weight: bold;'>Município/UF</td>"
							   . "    <td>{$dobras->getMunDescricao()} / {$dobras->getEstUf()}</td>"
							   . "</tr>";
		
		break;
		
		case "contatos":
		
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
						r.obrid = '". $obrid . "'  AND 
						rc.recostatus = 'A'";
			
			$dadosContatos = $db->carregar( $sql );
			
			$tabelaContatos = "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>";
			
			if ( $dadosContatos ){
			
				$tabelaContatos .= "    <tr>"
								. "        <td class='subtitulocentro'>CPF</td>"
								. "        <td class='subtitulocentro'>Nome do Responsável</td>"
								. "        <td class='subtitulocentro'>E-mail</td>"
								. "        <td class='subtitulocentro'>Telefone</td>"
								. "        <td class='subtitulocentro'>Tipo de Responsabilidade</td>"
								. "    </tr>";

				for( $i = 0; $i < count($dadosContatos); $i++ ){
					
					$cor = $i % 2 ? "#e0e0e0" : '#ffffff'; 
					
					$tabelaContatos .= "    <tr bgcolor='{$cor}'>"
									. "        <td align='center'>{$dadosContatos[$i]["cpf"]}</td>"
									. "        <td align='center'>{$dadosContatos[$i]["nome"]}</td>"
									. "        <td align='center'>" . ( $dadosContatos[$i]["email"] ? $dadosContatos[$i]["email"] : "Não informado" ). "</td>"
									. "        <td align='center'>{$dadosContatos[$i]["telefone"]}</td>"
									. "        <td align='center'>{$dadosContatos[$i]["tipo_desc"]}</td>"
									. "    </tr>";	
					
				}
								
			}else{
				
				$tabelaContatos .= "    <tr>"
								. "        <td align='center' style='color:#ee0000'>Não existem contatos cadastrados para a obra.</td>"
								. "    </tr>";
				
			}
			
			$tabelaContatos .= "</table>"; // fim da pesquisa da tabela contatos
			
			$sql = "SELECT DISTINCT
						su.usucpf as cpf,
						su.usunome as nome,
						su.usuemail as email,
						'(' || su.usufoneddd || ') ' || su.usufonenum as telefone
					FROM 
						seguranca.usuario su 
					JOIN obras.usuarioresponsabilidade ur ON ur.usucpf = su.usucpf
									      AND ur.rpustatus = 'A' 
					WHERE 
						ur.obrid = {$obrid}
					AND ur.pflcod IN (" . PERFIL_SUPERVISORUNIDADE . ", " . PERFIL_GESTORUNIDADE . ", " . PERFIL_EMPRESA . ")
					ORDER BY 
						su.usunome";
			
			$dadosResponsaveis = $db->carregar( $sql );
			
			$tabelaResponsaveis = "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>";
			
			if ( $dadosResponsaveis ){
			
				$tabelaResponsaveis .= "<tr>"
									. " 	<td class='subtitulocentro'>CPF</td>"
									. "     <td class='subtitulocentro'>Nome do Responsável</td>"
									. "     <td class='subtitulocentro'>E-mail</td>"
									. "     <td class='subtitulocentro'>Telefone</td>"
									. "</tr>";

				for( $i = 0; $i < count($dadosResponsaveis); $i++ ){
					
					$cor = $i % 2 ? "#e0e0e0" : '#ffffff'; 
					
					$tabelaResponsaveis .= "<tr bgcolor='{$cor}'>"
										. " 	<td align='center'>{$dadosResponsaveis[$i]["cpf"]}</td>"
										. "     <td align='center'>{$dadosResponsaveis[$i]["nome"]}</td>"
										. "     <td align='center'>" . ( $dadosResponsaveis[$i]["email"] ? $dadosResponsaveis[$i]["email"] : "Não informado" ). "</td>"
										. "     <td align='center'>{$dadosResponsaveis[$i]["telefone"]}</td>"
										. "</tr>";	
					
				}
								
			}else{
				
				$tabelaResponsaveis .= "    <tr>"
								. "        <td align='center' style='color:#ee0000'>Não existem responsáveis cadastrados para a obra.</td>"
								. "    </tr>";
				
			}
			
			$tabelaResponsaveis .= "</table>"; // fim da pesquisa da tabela responsáveis
			
			$dadosExtratoObras .= "<tr>"
							   .  "    <td class='subtitulocentro' colspan='2' height='25px;'>Contatos</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td colspan='2'>"
							   .           $tabelaContatos         
							   .  "    </td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulocentro' colspan='2' height='25px;'>Responsáveis pela Obra</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td colspan='2'>"
							   .           $tabelaResponsaveis         
							   .  "    </td>"
							   .  "</tr>";
							   
		break;
		
		case "contratacao":
			
			$obra = $obras->ViewObra($obrid);
			
			if ( $tobaid = $dobras->getTobraId() ){
					
				$sql = "SELECT 
							tobadesc AS descricao 
						FROM 
							obras.tipoobra
						WHERE
							tobaid = ".$tobaid;
				
				$tpContratacao = $db->pegaUm($sql) ? $db->pegaUm($sql) : "Não informado";
				
			}
			
//			$empresa = new Entidade($dobras->getEntIdEmpresaConstrutora());
//			$entnomeempresa = $empresa->entnome;
			$sql = "SELECT
						entnome
					FROM
						entidade.entidade
					WHERE
						entid = ".(int)$dobras->getEntIdEmpresaConstrutora();
						
			$entnomeempresa = $db->pegaUm( $sql );
			
			if( $dobras->getEntIdEmpresaConstrutora() ){
				
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
				
			}
			$dados = "";
			$dados = $db->carregar($sql);
			
			
			if ( is_array($dados) ){
				$emailempresa 	 = $dados[0]['email'] ? $dados[0]['email'] : "Não informado";
				$enderecoempresa = $dados[0]['endereco'];
				$dddempresa 	 = $dados[0]['ddd'];
				$telefoneempresa = $dados[0]['telefone'];
				$naturezaempresa = $dados[0]['natureza'] ? $dados[0]['natureza'] : "Não informado";	
			}
			
			if( $stoid = $dobras->getStoId() ){
				
				$sql = "SELECT
							stodesc
						FROM
							obras.situacaoobra
						WHERE
							stoid = {$stoid}";
				
				$stContratacao = $db->pegaUm( $sql ) ? $db->pegaUm( $sql ) : "Não informado";
				
			}
			
			if ( $umdid = $dobras->getUmdIdObraConstruida() ){
				
				$sql = "SELECT 
							umdeesc AS descricao 
						FROM 
							obras.unidademedida
						WHERE 
							umdid =".$umdid;
				
				$umContratacao = $db->pegaUm($sql);
				
			}
			
			
			switch( $dobras->getObrStatusInauguracao() ){
				case "S":
					$inContratacao = "Não se Aplica";
				break;
				case "N":
					$inContratacao = "Não Inaugurada";
				break;
				case "I":
					$inContratacao = "Inaugurada";
				break;
			}	
	
			if( $dobras->getFrpId() ){
				
				
				$sql = "SELECT 
							frpdesc AS descricao 
						FROM 
							obras.tipoformarepasserecursos
						WHERE 
							frpid =".$dobras->getFrpId();
				
				$tfrContratacao =  $db->pegaUm($sql);
				
			}
			
			$tfrContratacao = $tfrContratacao ? $tfrContratacao : "Não informado";
			
			switch( $dobras->getFrpId() ){
				
				case 2:
			
					$covid = $dobras->getCovId();
						
					if( $covid ){
									
						$dados_convenio = $db->pegaLinha("SELECT
															*
														  FROM 
														  	obras.conveniosobra
														  WHERE
														  	covid = '{$covid}'");
						
					}
								
					$covnumero = $dados_convenio["covnumero"];

					$dadosDetalheForma = "<tr><td>"
										. "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>"
										. "    <tr>"
										. "        <td class='subtitulodireita'>Número do Convênio</td>"
										. "        <td bgcolor='#f5f5f5'>{$covnumero}</td>"
										. "    </tr>"
										. "    <tr>"
										. "        <td class='subtitulodireita'>Ano</td>"
										. "        <td>".formata_data($dados_convenio["covano"])."</td>"
										. "    </tr>"
										. "    <tr>"
										. "        <td class='subtitulodireita'>Objeto</td>"
										. "        <td bgcolor='#f5f5f5'>".$dados_convenio["covobjeto"]."</td>"
										. "    </tr>"
										. "    <tr>"
										. "        <td class='subtitulodireita'>Detalhamento</td>"
										. "        <td>".$dados_convenio["covdetalhamento"]."</td>"
										. "    </tr>"
										. "    <tr>"
										. "        <td class='subtitulodireita'>Processo</td>"
										. "        <td bgcolor='#f5f5f5'>".$dados_convenio["covprocesso"]."</td>"
										. "    </tr>"
										. "    <tr>"
										. "        <td class='subtitulodireita'>Concedente</td>"
										. "        <td>".number_format($dados_convenio["covvlrconcedente"],2,',','.')."</td>"
										. "    </tr>"
										. "    <tr>"
										. "        <td class='subtitulodireita'>Convenente</td>"
										. "        <td bgcolor='#f5f5f5'>".number_format($dados_convenio["covvlrconvenente"],2,',','.')."</td>"
										. "    </tr>"
										. "    <tr>"
										. "        <td class='subtitulodireita'>Valor (R$)</td>"
										. "        <td>".number_format($dados_convenio["covvalor"],2,',','.')."</td>"
										. "    </tr>"
										. "    <tr>"
										. "        <td class='subtitulodireita'>Início</td>"
										. "        <td bgcolor='#f5f5f5'>".formata_data($dados_convenio["covdtinicio"])."</td>"
										. "    </tr>"
										. "    <tr>"
										. "        <td class='subtitulodireita'>Fim</td>"
										. "        <td>".formata_data($dados_convenio["covdtfinal"])."</td>"
										. "    </tr>"
										. "</table>"
										. "</tr></td>";
				
				break;
				
				case 3:
					
					$frrdescinstituicao = $dobras->getFrrDescInstituicao();
					if( $frrdescinstituicao == "" ){
						$dados2 = $obras->ViewObra($obrid);
						$dobras->setFrrDescInstituicao($dados2["entidade"]);
					}
					
					$frrdescinstituicao = $dobras->getFrrDescInstituicao();
					$frrdescnumport     = $dobras->getFrrDescNumPort();
					$frrdescobjeto 		= $dobras->getFrrDescObjeto();
					$frrdescvlr 		= number_format( $dobras->getFrrDescVlr(), 2, ',', '.' );
					
					$dadosDetalheForma = "<tr><td>"
									   . "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>"
									   . "    <tr>"
									   . "        <td class='subtitulodireita' style='font-weight: bold;'>Instituição</td>"
									   . "        <td bgcolor='#f5f5f5'>{$frrdescinstituicao}</td>"
									   . "    </tr>"
									   . "    <tr>"
									   . "        <td class='subtitulodireita' style='font-weight: bold;'>Número da Portaria de Descentralização</td>"
									   . "        <td bgcolor='#f5f5f5'>{$frrdescnumport}</td>"
									   . "    </tr>"
									   . "    <tr>"
									   . "        <td class='subtitulodireita' style='font-weight: bold;'>Objeto</td>"
									   . "        <td bgcolor='#f5f5f5'>{$frrdescobjeto}</td>"
									   . "    </tr>"
									   . "    <tr>"
									   . "        <td class='subtitulodireita' style='font-weight: bold;'>Valor (R$)</td>"
									   . "        <td bgcolor='#f5f5f5'>{$frrdescvlr}</td>"
									   . "    </tr>"
									   . "</table>"
									   . "</tr></td>";
					
				break;
				
				default:
					
					$dadosDetalheForma =  "<tr>"
									   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Observação</td>"
									   .  "    <td bgcolor='#f5f5f5'>" . ($dobras->getFrrObsRecProprio() ? $dobras->getFrrObsRecProprio() : "Não informado") . "</td>"
									   .  "</tr>";
					
				break;
			}
			
			
			// Situação do Imóvel
			$infraestrutura = new DadosInfraEstrutura();
			$infra = $infraestrutura->busca($_SESSION['obra']['obrid']);
			
			$sql = "SELECT
						aqidsc 
					FROM 
						obras.tipoaquisicaoimovel
					WHERE
						aqiid = ".(int)$infra['aqiid'];
			
			// Descrição da Situação do Imóvel 
			$aqidsc = $db->pegaUm($sql);
			
			// Situação Dominial já Regularizada? 
			$iexsitdominialimovelregulariza = $infra['iexsitdominialimovelregulariza'];
			
			$dadosExtratoObras .= "<tr>"
							   .  "    <td class='subtitulocentro' colspan='2' height='25px;'>Contratação</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td colspan='2' style='font-weight: bold;'>Contratação da Obra</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Empresa Contratada</td>"
							   .  "    <td bgcolor='#f5f5f5'>{$entnomeempresa}</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>E-mail</td>"
							   .  "    <td>{$emailempresa}</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Endereço</td>"
							   .  "    <td bgcolor='#f5f5f5'>{$enderecoempresa}</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Telefone</td>"
							   .  "    <td>({$dddempresa}) {$telefoneempresa}</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Natureza Jurídica</td>"
							   .  "    <td bgcolor='#f5f5f5'>{$naturezaempresa}</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Data de Assinatura do Contrato</td>"
							   .  "    <td>" . ( $dobras->getObrDtAssinaturaContrato() ? formata_data( $dobras->getObrDtAssinaturaContrato() ) : "Não informado" ) . "</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td colspan='2' style='font-weight: bold;'>Sobre a Obra</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Situação da Obra</td>"
							   .  "    <td bgcolor='#f5f5f5'>{$stContratacao}</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Data da Ordem de Serviço</td>"
							   .  "    <td>" . ( $dobras->getObrDtOrdemServico() ? formata_data( $dobras->getObrDtOrdemServico() ) : "Não informado" ) . "</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Início de Execução da Obra</td>"
							   .  "    <td bgcolor='#f5f5f5'>" . ( $dobras->getObrDtInicio() ? formata_data( $dobras->getObrDtInicio() ) : "Não informado" ) . "</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Término de Execução da Obra</td>"
							   .  "    <td>" . ( $dobras->getObrDtTermino() ? formata_data( $dobras->getObrDtTermino() ) : "Não informado" ) . "</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Valor do Contrato (R$)</td>"
							   .  "    <td bgcolor='#f5f5f5'>" . number_format( $dobras->getObrCustoContrato(), 2, ',' , '.' ) . "</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Área/Quantidade a ser Construída</td>"
							   .  "    <td>" . number_format( $dobras->getObrQtdConstruida(), 2, ',', '.' ) . " " . $umContratacao ."</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Custo Unitário (R$)</td>"
							   .  "    <td bgcolor='#f5f5f5'>" . number_format( $dobras->getObrCustoUnitQtdConstruida(), 2, ',', '.') . "</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Custo Unitário (R$)</td>"
							   .  "    <td>" . number_format( $dobras->getObrPercBdi(), 2, ',', '.') . "</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Tipo de Obra</td>"
							   .  "    <td bgcolor='#f5f5f5'>{$tpContratacao}</td>"
							   .  "</tr>"
							   
							   
							   .  "<tr>"
							   .  "    <td colspan='2' style='font-weight: bold;'>Situação do Imóvel</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Tipo de Aquisição do Terreno</td>"
							   .  "    <td bgcolor='#f5f5f5'>{$aqidsc}</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Situação Dominial já Regularizada?</td>"
							   .  "    <td bgcolor='#f5f5f5'>".( $iexsitdominialimovelregulariza ? "Sim" : "Não" )."</td>"
							   .  "</tr>"
							   
							   
							   .  "<tr>"
							   .  "    <td colspan='2' style='font-weight: bold;'>Sobre a Inauguração</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Inaugurada</td>"
							   .  "    <td bgcolor='#f5f5f5'>{$inContratacao}</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Data da Inauguração</td>"
							   .  "    <td>" . ( $dobras->getObrDtInauguracao() ? formata_data( $dobras->getObrDtInauguracao() ) : "Não informado" ) . "</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Data de Previsão da Inauguração</td>"
							   .  "    <td bgcolor='#f5f5f5'>" . ( $dobras->getObrDtPrevInauguracao() ? formata_data( $dobras->getObrDtPrevInauguracao() ) : "Não informado" ) . "</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td colspan='2' style='font-weight: bold;'>Forma de Repasse de Recursos</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Tipo</td>"
							   .  "    <td bgcolor='#f5f5f5'>{$tfrContratacao}</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td colspan='2' style='font-weight: bold;'>Detalhes</td>"
							   .  "</tr>"
							   .  $dadosDetalheForma;

							   
		break;
		
		case "etapasobra":
			
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
						i.obrid = " . $obrid . " 
						and i.itcid = ic.itcid 
						and i.icovigente = 'A'
					ORDER BY 
						i.icoordem";
			
			$dadosEtapas = $db->carregar( $sql );
			
			$tabelaEtapas = "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>";
			
			if ( $dadosEtapas ){
			
				$tabelaEtapas .= "    <tr>"
							  . "        <td class='subtitulocentro'>Descrição</td>"
							  . "        <td class='subtitulocentro'>Valor do Item</td>"
							  . "        <td class='subtitulocentro'>% Referente a Obra</td>"
							  . "    </tr>";

				for( $i = 0; $i < count($dadosEtapas); $i++ ){
					
					$itcid 		= $dadosEtapas[$i]['itcid'];
					$icovlritem = $dadosEtapas[$i]['icovlritem'];
					$itcdesc 	= $dadosEtapas[$i]['itcdesc'];
					$icopercsobreobra = $dadosEtapas[$i]['icopercsobreobra'];
					$icopercexecutado = $dadosEtapas[$i]['icopercexecutado'];
					$itcdescservico   = $dadosEtapas[$i]['itcdescservico'];
					
					$somav 		= bcadd($somav, $icovlritem, 2);
					$icovlritem = number_format($icovlritem,2,',','.'); 
					$soma 		= round( $soma, 2 ) + round( $icopercsobreobra, 2 );
					
					$icopercsobreobra = number_format( $icopercsobreobra, 2);
					$icopercsobreobra = str_replace( '.', ',', $icopercsobreobra );
										
					$cor = $i % 2 ? "#e0e0e0" : '#ffffff';
					
					$tabelaEtapas .= "    <tr bgcolor='{$cor}'>"
								  . "        <td>{$itcdesc}</td>"
								  . "        <td align='right'>{$icovlritem}</td>"
								  . "        <td align='right'>{$icopercsobreobra}</td>"
								  . "    </tr>";	
					
				}
								
			}else{
				
				$tabelaEtapas .= "    <tr>"
							  . "        <td align='center' style='color:#ee0000'>Não existem etapas cadastradas para a obra.</td>"
							  . "    </tr>";
				
			}
			
			$soma = ($soma > 100.00) ? 100.00 : $soma;
			
			$tabelaEtapas .= "    <tr bgcolor='#C0C0C0'>"
						  .  "        <td align='right'><b>Total</b></td>"
						  .  "        <td align=right><b>" . number_format( $somav, 2, ',', '.' ) . "</b></td>"
						  .  "        <td align=right><b>" . number_format( $soma, 2, ',', '.' ) . "</b></td>"
						  .  "    </tr>"
			  			  .  "</table>";
			
			$dadosExtratoObras .= "<tr>"
							   .  "    <td class='subtitulocentro' colspan='2' height='25px;'>Cronograma Físico-Financeiro</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td colspan='2'>"
							   .           $tabelaEtapas
							   .  "    </td>"
							   .  "</tr>";
							   
		break;
		
		case "execucao":
		
			/*$execOrc = new execOrcamentaria();
			$dadosExecOrc = $execOrc->buscaExecOrcamentaria( $obrid );

			$nEocvlrtotal = $dadosExecOrc["eocvlrcapital"] + $dadosExecOrc["eocvlrcusteio"];
			$eocvlrtotal  = number_format($nEocvlrtotal, 2, ",", ".");

			if ( $dadosExecOrc["eorid"] ){
					
					$sql = "SELECT
								*
							FROM
								 obras.itensexecucaoorcamentaria
							WHERE
								eorid = {$dadosExecOrc["eorid"]}
							ORDER BY
								eocdtposicao";
					
					$dadosItensExec =  $db->carregar( $sql );
				
				}
				
				$tabItensExec = "<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>";
				
				if( $dadosItensExec ){
					
					$tabItensExec .= "<tr>
										<td class='subtitulocentro'>Data</td>
										<td class='subtitulocentro'>Valor Empenhado (R$)</td>
										<td class='subtitulocentro'>Valor Liquidado (R$)</td>
										<td class='subtitulocentro'>% Empenhado</td>
										<td class='subtitulocentro'>% Liquidado</td>
									</tr>";
					
					for( $i = 0; $i < count($dadosItensExec); $i++ ){
						
						$cor = ( $i % 2 ) ? "#e0e0e0" : "#f4f4f4";
						
						$perEmpenhado = $dadosItensExec[$i]["eocvlrempenhado"] ? ( $dadosItensExec[$i]["eocvlrempenhado"] / $eocvlrtotal ) * 100 : 0.00;
						$perLiquidado = $dadosItensExec[$i]["eocvlrliquidado"] ? ( $dadosItensExec[$i]["eocvlrliquidado"] / $eocvlrtotal ) * 100 : 0.00;
						
						$tabItensExec .=  "<tr bgcolor='{$cor}' align='right' id='item_{$dadosItensExec[$i]["ideid"]}'>"
									  . "    <td width='15%' align='center'>"
									  . "        <input type='hidden' name='eocdtposicao[]' id='eocdtposicao[]' value='" . formata_data($dadosItensExec[$i]["eocdtposicao"]) . "'/>"
									  .	       formata_data($dadosItensExec[$i]["eocdtposicao"])
									  . "    </td>"
									  . "    <td width='20%'>"
									  . "        <input type='hidden' name='eocvlrempenhado[]' id='eocvlrempenhado[]' value='{$dadosItensExec[$i]["eocvlrempenhado"]}'/>"
									  . 	       number_format($dadosItensExec[$i]["eocvlrempenhado"], 2, ",", ".") 
									  . "    </td>"
									  . "    <td width='20%'>"
									  . "        <input type='hidden' name='eocvlrliquidado[]' id='eocvlrliquidado[]' value='{$dadosItensExec[$i]["eocvlrliquidado"]}'/>"
									  .          number_format($dadosItensExec[$i]["eocvlrliquidado"], 2, ",", ".")
									  . "    </td>"
									  . "    <td width='15%'>" . number_format($perEmpenhado, 2, ",", ".") . " %</td>"
									  . "    <td width='15%'>" . number_format($perLiquidado, 2, ",", ".") . " %</td>"
									  . "</tr>";
						
					}
					
				}else{
					
					$tabItensExec .= "<tr><td style='color:#ee0000;'>Não existem itens cadastrados para este orçamento.</td></tr>";
					
				}
			
			$tabItensExec .= "</table>";
				
			$dadosExtratoObras .= "<tr>"
							   .  "    <td class='subtitulocentro' colspan='2' height='25px;'>Execução Orçamentária</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Capital (R$)</td>"
							   .  "    <td bgcolor='#f5f5f5'>" .  number_format($dadosExecOrc["eocvlrcapital"], 2, ",", ".") . "</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Custeio (R$)</td>"
							   .  "    <td>" . number_format($dadosExecOrc["eocvlrcusteio"], 2, ",", ".") . "</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Total (R$)</td>"
							   .  "    <td bgcolor='#f5f5f5'>{$eocvlrtotal}</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td style='font-weight: bold;'>Detalhamento Orçamentário</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td colspan='2'>"
							   .           $tabItensExec
							   .  "    </td>"
							   .  "</tr>";*/
							  
			
		break;
		
		case "licitacao":
		
			$licitacao = new licitacao();
			$resultado = $licitacao->busca($obrid);
			$dadosLic  = $licitacao->dados($resultado);
			
			if( $licitacao->molid ){
				
				$sql = "SELECT 
					   		moldsc AS descricao 
					   FROM 
							obras.modalidadelicitacao
					   ORDER BY 
							moldsc";
			
				$moldsc = $db->pegaUm( $sql );
				
			}
			
			$moldsc = $moldsc ? $moldsc : "Não informado";
			
			$sql = "SELECT 
						fl.*,
						tfl.tfldesc, tfl.tflordem   
					FROM 
						obras.faselicitacao fl 
					INNER JOIN 
						obras.tiposfaseslicitacao tfl ON fl.tflid = tfl.tflid
					WHERE 
						fl.obrid = '". $obrid . "' AND fl.flcstatus = 'A' ORDER BY tfl.tflordem";
			
			$flLicitacao = $db->carregar( $sql );
			
			$tabelaFlCont = "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>";
			
			if ( $flLicitacao ){
			
				$tabelaFlCont .= "    <tr>"
							  . "        <td class='subtitulocentro'>Descrição</td>"
							  . "        <td class='subtitulocentro'>Data</td>"
							  . "    </tr>";

				for( $i = 0; $i < count($flLicitacao); $i++ ){
					
					$tflid   = $flLicitacao[$i]['tflid'];
					$tfldesc = $flLicitacao[$i]['tfldesc'];
					$flcpubleditaldtprev = formata_data($flLicitacao[$i]['flcpubleditaldtprev']);
					$flcdtrecintermotivo = formata_data($flLicitacao[$i]['flcdtrecintermotivo']);
					$flcordservdt		 = formata_data($flLicitacao[$i]['flcordservdt']);
					$flchomlicdtprev     = formata_data($flLicitacao[$i]['flchomlicdtprev']);
					$flcaberpropdtprev	 = formata_data($flLicitacao[$i]['flcaberpropdtprev']);

					switch( $tflid ){
						
						case 2:
							$flcdata = $flcpubleditaldtprev;
						break;
						case 5:
							$flcdata = $flcdtrecintermotivo;
						break;
						case 6:
							$flcdata = $flcordservdt;
						break;
						case 7:
							$flcdata = $flcaberpropdtprev;
						break;
						case 9:
							$flcdata = $flchomlicdtprev;
						break;
						
					}
					
					
					$cor = $i % 2 ? "#e0e0e0" : '#ffffff'; 
					
					$tabelaFlCont .= "    <tr bgcolor='{$cor}'>"
								   . "        <td>{$tfldesc}</td>"
								   . "        <td align='center'>{$flcdata}</td>"
								   . "    </tr>";	
					
				}
								
			}else{
				
				$tabelaFlCont .= "    <tr>"
							  . "        <td align='center' style='color:#ee0000'>Não existem fases de licitação cadastradas para a obra.</td>"
							  . "    </tr>";
				
			}
			
			$tabelaFlCont .= "</table>";
			
			$dadosExtratoObras .= "<tr>"
							   .  "    <td class='subtitulocentro' colspan='2' height='25px;'>Licitação</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Modalidade de Licitação</td>"
							   .  "    <td bgcolor='#f5f5f5'>{$moldsc}</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Início Programado</td>"
							   .  "    <td >" . ($licitacao->dtiniciolicitacao ? formata_data($licitacao->dtiniciolicitacao) : "Não Informado") . "</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Término Programado</td>"
							   .  "    <td bgcolor='#f5f5f5'>" . ($licitacao->dtfinallicitacao ? formata_data($licitacao->dtfinallicitacao) : "Não Informado") . "</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Número da UASG</td>"
							   .  "    <td>" . ($licitacao->licitacaouasg ? $licitacao->licitacaouasg : "Não Informado") . "</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Número da Licitação</td>"
							   .  "    <td bgcolor='#f5f5f5'>" . ($licitacao->numlicitacao ? $licitacao->numlicitacao : "Não Informado") . "</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td style='font-weight: bold;'>Fases de Licitação</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td colspan='2'>"
							   .           $tabelaFlCont
							   .  "    </td>"
							   .  "</tr>";
			
		break;
		
		case "projetos":
			
			$faseprojeto = new DadosFasesProjeto();
			$resultado   = $faseprojeto->busca($obrid);	
			$dados 		 = $faseprojeto->dados($resultado);
			
			if ($tpaid = $faseprojeto->tpaid){
				
				$sql = "SELECT 
							tpadesc as descricao 
						FROM 
							obras.tipoprojetoarquitetonico
						WHERE 
							tpaid=".$tpaid;
				
				$tpProjeto = $db->pegaUm($sql) ? $db->pegaUm($sql) : "Não informado";
			}
			
			if ($felid = $faseprojeto->felid){
				
				$sql = "SELECT 
							feldesc as descricao 
						FROM 
							obras.formaelaboracao
						WHERE 
							felid=".$felid;
				
				$feProjeto = $db->pegaUm($sql) ? $db->pegaUm($sql) : "Não informado";
			}
			
			if( $faseprojeto->felid == 1 || $faseprojeto->felid == 3 || $faseprojeto->felid == 4 ){
				
				$obProjeto = $faseprojeto->fprobsexecdireta ? $faseprojeto->fprobsexecdireta : "Não Informado";
				
			}else if( $faseprojeto->felid == 2 ){
				
				$obProjeto = "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>"
						   . "    <tr>"
						   . "        <td>Recurso Próprio (R$)</td>"
						   . "        <td>" . number_format( $faseprojeto->fprvlrformaelabrecproprio, 2, ',', '.' ) . "</td>"
						   . "    </tr>"
						   . "    <tr>"
						   . "        <td>Recurso Repassado (R$)</td>"
						   . "        <td>" . number_format( $faseprojeto->fprvlrformaelabrrecrepassado, 2, ',', '.' ) . "</td>"
						   . "    </tr>"
						   . "</table>";
						
			}
			
			if ($tfpid = $faseprojeto->tfpid){
				
				$sql = "SELECT 
							tfpdesc as descricao 
						FROM 
							obras.tipofaseprojeto
						WHERE 
							tfpid=".$tfpid;
				
				$fpProjeto = $db->pegaUm($sql) ? $db->pegaUm($sql) : "Não informado";
				
			}

			switch( $tfpid ){
				case 1:
					$dtProjeto = $faseprojeto->fprdtiniciofaseprojeto;
				break;
				case 2:
					$dtProjeto = $faseprojeto->fprdtprevterminoprojeto;
				break;
				case 3:
					$dtProjeto = $faseprojeto->fprdtconclusaofaseprojeto;
				break;
			}
			
			$dadosExtratoObras .= "<tr>"
							   .  "    <td class='subtitulocentro' colspan='2' height='25px;'>Projetos </td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Tipo de Projeto</td>"
							   .  "    <td bgcolor='#f5f5f5'>{$tpProjeto}</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Forma de Elaboração do projeto</td>"
							   .  "    <td>{$feProjeto}</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Observações</td>"
							   .  "    <td bgcolor='#f5f5f5'>{$obProjeto}</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Fases do Projeto</td>"
							   .  "    <td>{$fpProjeto}</td>"
							   .  "</tr>"
							   .  "<tr>"
							   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Previsão / Conclusão</td>"
							   .  "    <td bgcolor='#f5f5f5'>" . formata_data($dtProjeto) . "</td>"
							   .  "</tr>";
				   
		break;
		
	}
	
}

if( $coordenada == 1 ){
	
	$longitude = explode(".", $dobras->getMedLongitude());
	$graulongitude = $longitude[0];
	$minlongitude = $longitude[1];
	$seglongitude = $longitude[2];
	
	$latitude = explode(".", $dobras->getMedLatitude());
	$graulatitude = $latitude[0];
	$minlatitude = $latitude[1];
	$seglatitude = $latitude[2];
	$pololatitude = $latitude[3];

	// Latitude
	$dadosLatitude =  $graulatitude."° ".$minlatitude."' ".$seglatitude."'' "."  ".$pololatitude;
	
	// Longitude
	$dadosLongitude = $graulongitude."° ".$minlongitude."' ".$seglongitude."''";
	
	$dadosExtratoObras .= "<tr>"
					   .  "    <td class='subtitulocentro' colspan='2' height='25px;'>Coordenadas Geográficas</td>"
					   .  "</tr>"
					   .  "<tr>"
					   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Latitude</td>"
					   .  "    <td bgcolor='#f5f5f5'>{$dadosLatitude}</td>"
					   .  "</tr>"
					   .  "<tr>"
					   .  "    <td class='subtitulodireita' style='font-weight: bold;'>Longitude</td>"
					   .  "    <td>{$dadosLongitude}</td>"
					   .  "</tr>";
	
	if( $mapa == 1 ){
		
		$latitude  = ((( $seglatitude / 60 ) + $minlatitude) / 60 ) + $graulatitude;
		$longitude = ((( $seglongitude / 60 ) + $minlongitude) / 60 ) + $graulongitude;
		
		if ( $latitude && $longitude ){	

			$posicao = ( $pololatitude == "N" ) ? "-" : "";
			
			$dadosExtratoObras .= "<tr><td colspan='2' align='center'><iframe width='400' height='400' frameborder='0' scrolling='no' marginheight='0' marginwidth='0' 
									src='http://maps.google.com/?ie=UTF8&amp;t=k&amp;ll={$posicao}{$latitude},-{$longitude}&amp;spn=0.009417,0.013304&amp;z=17&amp;output=embed&amp;s=AARTsJqzARj-Z8VnW5pkPMLMmZbqrJcYpw'>
									</iframe></td></tr>";
			
		}else{
			$dadosExtratoObras .= "<tr><td colspan='2' align='center'>Dados de localização não cadastrados.</td></tr>";
		}
		
	}					   
					   
}

if( $foto == 1 ){
	
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
				" . ( $fotoseleciona ? " arq.arqid IN ({$fotoseleciona}) AND " : "" ) . "  
				obr.obrid = {$obrid} AND
				aqostatus = 'A' AND
				(arqtipo = 'image/jpeg' OR 
				 arqtipo = 'image/gif' OR 
				 arqtipo = 'image/png') 
			ORDER BY 
				arq.arqid ";

	$fotos = ($db->carregar($sql));

	if( $fotos ){
		
		$tabelaFotos = "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>";
		
		for( $i = 0; $i < count( $fotos ); $i++ ){
			
			if( $fotos[$i]["arqid"] ){
				
				$tabelaFotos .= "<tr>"
							 . "    <td align='center'>"
							 . "        <img src='../slideshow/slideshow/verimagem.php?newwidth=225&newheight=225&arqid={$fotos[$i]["arqid"]}' 
										hspace='3' vspace='3' style='width:225px; height:225px;' /> <br>{$fotos[$i]["arqdescricao"]}"
							 . "    </td>";
							 
			}
			
			$i = $i+1;
			
			if( $fotos[$i]["arqid"] ){
				
				$tabelaFotos .= "    <td align='center'>"
							 .  "       <img src='../slideshow/slideshow/verimagem.php?newwidth=225&newheight=225&arqid={$fotos[$i]["arqid"]}' 
										hspace='3' vspace='3' style='width:225px; height:225px;' /> <br>{$fotos[$i]["arqdescricao"]}"
							 .  "    </td>";
							 
			}
			
			$i = $i+1;
			
			if( $fotos[$i]["arqid"] ){
				
				$tabelaFotos .= "    <td align='center'>"
							 .  "        <img src='../slideshow/slideshow/verimagem.php?newwidth=225&newheight=225&arqid={$fotos[$i]["arqid"]}' 
										hspace='3' vspace='3' style='width:225px; height:225px;' /> <br>{$fotos[$i]["arqdescricao"]}"
							 .  "    </td>"
							 . "</tr>";
							 
			}
			
		}
		
	}else{
		
		$tabelaFotos .= "    <tr>"
					 .  "        <td align='center' colspan='2' style='color:#ee0000'>Não existem fotos cadastradas para a obra.</td>"
					 .  "    </tr>";
		
	}			
	
	$tabelaFotos .= "</table>";
	
	$dadosExtratoObras .= "<tr>"
					   .  "    <td class='subtitulocentro' colspan='2' height='25px;'>Galeria de Fotos</td>"
					   .  "</tr>"
					   .  "<tr>"
					   .  "<td colspan='2'>{$tabelaFotos}</td>"
					   .  "</tr>";
					   
	// Execução Orçamentária
	// Orçamento para a Obra
	$execOrc = new execOrcamentaria();
	$dadosExecOrc = $execOrc->buscaExecOrcamentaria( $_SESSION["obra"]["obrid"] );
	
	$tabelaOrcamento = "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>";
	
	if ( $dadosExecOrc ){
	
		$nEocvlrtotal = $dadosExecOrc["eocvlrcapital"] + $dadosExecOrc["eocvlrcusteio"];
		$eocvlrtotal  = number_format($nEocvlrtotal, 2, ",", ".");
		
		$tabelaOrcamento .= "    <tr>"
						 . "        <td class='subtitulocentro'>Capital (R$)</td>"
						 . "        <td class='subtitulocentro'>Custeio (R$)</td>"
						 . "        <td class='subtitulocentro'>Total (R$)</td>"
						 . "    </tr>"
						 . "    <tr bgcolor='#ffffff'>"
						 . "        <td align='center'>".number_format($dadosExecOrc["eocvlrcapital"], 2, ",", ".")."</td>"
						 . "        <td align='center'>".number_format($dadosExecOrc["eocvlrcusteio"], 2, ",", ".")."</td>"
						 . "        <td align='center'>{$eocvlrtotal}</td>"
						 . "    </tr>";	
			
	}else{
		
		$tabelaOrcamento .= "    <tr>"
						 . "        <td align='center' style='color:#ee0000'>Não existem Orçamentos cadastrados para a obra.</td>"
						 . "    </tr>";
		
	}
	
	$tabelaOrcamento .= "</table>"; // fim da pesquisa da tabela Orçamento para a Obra
	
	if( $dadosExecOrc["eorid"] ){
		// Detalhamento Orçamentário
		$sql = "SELECT
					eocvlrempenhado,
					eocvlrliquidado,
					eocdtposicao				
				FROM
					 obras.itensexecucaoorcamentaria
				WHERE
					eorid = {$dadosExecOrc["eorid"]}
				ORDER BY
					eocdtposicao";
		
		$dadosItensExec =  $db->carregar( $sql );
	}
	$tabelaDetalhamento = "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>";
	
	if( $dadosItensExec ){
		
		$tabelaDetalhamento .= "    <tr>"
						 	. "        <td class='subtitulocentro'>Data</td>"
						 	. "        <td class='subtitulocentro'>Valor Empenhado (R$)</td>"
						 	. "        <td class='subtitulocentro'>Valor Liquidado (R$)</td>"
						 	. "        <td class='subtitulocentro'>% Empenhado</td>"
						 	. "        <td class='subtitulocentro'>% Liquidado</td>"
						 	. "    </tr>";
						 	
		for( $i = 0; $i < count($dadosItensExec); $i++ ){
			
			$cor = $i % 2 ? "#e0e0e0" : '#ffffff';
			
			$perEmpenhado = ($nEocvlrtotal > 0) ? ( $dadosItensExec[$i]["eocvlrempenhado"] / $nEocvlrtotal ) * 100 : 0.00;
			$perLiquidado = ($nEocvlrtotal > 0) ? ( $dadosItensExec[$i]["eocvlrliquidado"] / $nEocvlrtotal ) * 100 : 0.00;
			
			$totEmpenhado = $totEmpenhado + $dadosItensExec[$i]["eocvlrempenhado"];
			$totLiquidado = $totLiquidado + $dadosItensExec[$i]["eocvlrliquidado"];
				
			$totPerEmpenhado = $totPerEmpenhado + $perEmpenhado;
			$totPerLiquidado = $totPerLiquidado + $perLiquidado;
			
			$tabelaDetalhamento .= "<tr bgcolor='{$cor}'>"
								. "    <td align='center'>"
								.	       formata_data($dadosItensExec[$i]["eocdtposicao"])
								. "    </td>"
								. "    <td align='right'>"
								. 	       number_format($dadosItensExec[$i]["eocvlrempenhado"], 2, ",", ".") 
								. "    </td>"
								. "    <td align='right'>"
								.          number_format($dadosItensExec[$i]["eocvlrliquidado"], 2, ",", ".")
								. "    </td>"
								. "    <td align='right'>" . number_format($perEmpenhado, 2, ",", ".") . "</td>"
								. "    <td align='right'>" . number_format($perLiquidado, 2, ",", ".") . "</td>"
								. "</tr>";
				
		}// fim do for
		
		//total
		$tabelaDetalhamento .= "    <tr bgcolor='#c0c0c0'>"
	 						. "     	<td align='right'><b>Total</b></td>"
				 			. "		    <td align='right'>".number_format($totEmpenhado, 2, ",", "." )."</td>"
				 			. "         <td align='right'>".number_format($totLiquidado, 2, ",", "." )."</td>"
				 			. "         <td align='right'>".number_format($totPerEmpenhado, 2, ",", ".")."</td>"
				 			. "         <td align='right'>".number_format($totPerLiquidado, 2, ",", ".")."</td>"
				 			. "    </tr>";
		
	}else{
		$tabelaDetalhamento .= "    <tr>"
						 	. "        <td align='center' style='color:#ee0000'>Não existem Detalhamentos Orçamentários cadastrados para a obra.</td>"
						 	. "    </tr>";
	}// fim do if
	
	$tabelaDetalhamento .= "</table>"; // fim da pesquisa da tabela Detalhamento Orçamentário
	
	$dadosExtratoObras .= "<tr>"
					   .  "    <td class='subtitulocentro' colspan='2' height='25px;'>Orçamento para a Obra</td>"
					   .  "</tr>"
					   .  "<tr>"
					   .  "    <td colspan='2'>"
					   .           $tabelaOrcamento      
					   .  "    </td>"
					   .  "</tr>"
					   .  "<tr>"
					   .  "    <td class='subtitulocentro' colspan='2' height='25px;'>Detalhamento Orçamentário</td>"
					   .  "</tr>"
					   .  "<tr>"
					   .  "    <td colspan='2'>"
					   .           $tabelaDetalhamento         
					   .  "    </td>"
					   .  "</tr>";
	// Fim da Execução Orçamentária
	
	// Restrições e Providências						   
	$sql = "SELECT
				CASE WHEN fsrid is not null THEN fsrdsc ELSE 'Não Informada' END as fase,
				rstdesc,
				trtdesc,
				rstdescprovidencia,
				to_char(rstdtprevisaoregularizacao,'DD/MM/YYYY') as rstdtprevisaoregularizacao,
				CASE WHEN rstsituacao = true THEN to_char(rstdtsuperacao,'DD/MM/YYYY') ELSE 'Não' END AS rstdtsuperacao
			FROM
				obras.restricaoobra 
			INNER JOIN 
				obras.tiporestricao USING (trtid)
			LEFT JOIN
				obras.faserestricao USING (fsrid) 
			WHERE
				rststatus = 'A' AND
				obrid = " . $_SESSION["obra"]["obrid"];
	
	$dadosRestricoes =  $db->carregar( $sql );
	
	$tabelaRestricoes = "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>";
	
	if($dadosRestricoes){
		$tabelaRestricoes .= "    <tr>"
						  . "        <td class='subtitulocentro'>Fase da Restrição</td>"
						  . "        <td class='subtitulocentro'>Tipo de Restrição</td>"
						  . "        <td class='subtitulocentro'>Restrição</td>"
						  . "        <td class='subtitulocentro'>Providência</td>"
						  . "        <td class='subtitulocentro'>Previsão da Providência</td>"
						  . "        <td class='subtitulocentro'>Superação</td>"
						  . "    </tr>";

		for( $i = 0; $i < count($dadosRestricoes); $i++ ){
			
			$cor = $i % 2 ? "#e0e0e0" : '#ffffff';
			
			$tabelaRestricoes .= "<tr bgcolor=\"" . $cor . "\">
									<td align='center'>".$dadosRestricoes[$i]["fase"]."</td>
									<td align='center'>".$dadosRestricoes[$i]["trtdesc"]."</td>
									<td>".$dadosRestricoes[$i]["rstdesc"]."</td>
									<td>".$dadosRestricoes[$i]["rstdescprovidencia"]."</td>
									<td align='center'>".$dadosRestricoes[$i]["rstdtprevisaoregularizacao"]."</td>
									<td align='center'>".$dadosRestricoes[$i]["rstdtsuperacao"]."</td>
								  </tr>";
				
		}// fim do for
						  
	}else{
		$tabelaRestricoes .= "    <tr>"
						  . "        <td align='center' style='color:#ee0000'>Não existem Restrições e Providências cadastradas para a obra.</td>"
						  . "    </tr>";
	}// fim do if
	
	$tabelaRestricoes .= "</table>"; // fim da pesquisa da tabela Restrições e Providências
	
	$dadosExtratoObras .= "<tr>"
					   .  "    <td class='subtitulocentro' colspan='2' height='25px;'>Restrições e Providências</td>"
					   .  "</tr>"
					   .  "<tr>"
					   .  "    <td colspan='2'>"
					   .           $tabelaRestricoes      
					   .  "    </td>"
					   .  "</tr>";
	// Fim das Restrições e Providências
					   
}

if( $vistoria ){
	
	$WhereVistorias = $supvids != '' ? "s.supvid IN (" . $supvids . ") AND " : '';
	
	$sql = "SELECT
				supvid as vistoria,
				s.suprealizacao as responsavel,
				u.usunome as inseridopor,
				to_char(s.supvdt,'DD/MM/YYYY') as dtvistoria,
				CASE WHEN s.supvistoriador is not null THEN ev.entnome ELSE 'Não informado' END as vistoriador,
				si.stodesc as situacao,
				CASE WHEN supprojespecificacoes = 't' THEN 'Sim' ELSE 'Não' END as projetoespecificacoes,
				CASE WHEN supplacaobra = 't' THEN 'Sim' ELSE 'Não' END as placaobra,
				CASE WHEN supdiarioobra = 't' THEN 'Sim' ELSE 'Não' END as diarioobra,
				CASE WHEN supplacalocalterreno = 't' THEN 'Sim' ELSE 'Não' END as placalocalterreno,
				CASE WHEN supvalidadealvara = 't' THEN 'Sim' ELSE 'Não' END as validadealvara,
				qlbdesc as qualidadeobra,
				dcndesc as desempenho,
				CASE WHEN supobs != '' THEN supobs ELSE 'Não informado' END as observacao
			FROM
				obras.supervisao s
			INNER JOIN 
				obras.situacaoobra si ON si.stoid = s.stoid
			INNER JOIN
				seguranca.usuario u ON u.usucpf = s.usucpf
			LEFT JOIN
				entidade.entidade ev ON ev.entid = s.supvistoriador
			LEFT JOIN
				obras.qualidadeobra oq ON oq.qlbid = s.qlbid
			LEFT JOIN
				 obras.desempenhoconstrutora od ON od.dcnid = s.dcnid
			WHERE
				" . $WhereVistorias . " 
				s.obrid = '{$obrid}' AND
				s.supstatus = 'A'
			ORDER BY 
				s.supdtinclusao ASC";
	
	$totVistorias = $db->carregar( $sql );

	if ($totVistorias){
	
		for( $i = 0; $i < count($totVistorias); $i++ ){
			
			$sql = "SELECT
						itco.icoid,
						itc.itcdesc,
						itco.icovlritem,
						itco.icopercsobreobra, 
						to_char(itco.icodtinicioitem, 'DD/MM/YYYY') as inicio,
						to_char(itco.icodterminoitem, 'DD/MM/YYYY') as termino,
						itco.icopercprojperiodo,
						itco.icopercexecutado,
						sup.supvlrinfsupervisor,
						sup.supvlritemexecanterior,
						sup.supvlritemsobreobraexecanterior,
						sup.supvid
					FROM 
						obras.itenscomposicao itc,
						obras.itenscomposicaoobra itco
					LEFT JOIN
						obras.supervisaoitenscomposicao sup ON sup.icoid = itco.icoid AND
															   sup.supvid = {$totVistorias[$i]["vistoria"]}
					WHERE								
						itc.itcid = itco.itcid AND
						itco.obrid = {$obrid}
					ORDER BY 
						icoordem";
					
			$dadosItensVistoria = $db->carregar( $sql );
			
			$tabelaItensVistoria = "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>";
			
			if( $dadosItensVistoria ){
				
				$totalValor 		= 0;
				$totalPercObra 		= 0;
				$totalPercObraAnt   = 0;
				$totalPercObraAtual = 0;
				
				$tabelaItensVistoria .= "<tr>"
									 .  "    <td colspan='1' rowspan='2' class='subtitulocentro'>Item da Obra</td>"
									 .  "    <td colspan='1' rowspan='2' class='subtitulocentro'>Valor (R$)</td>"
									 .  "    <td colspan='1' rowspan='2' class='subtitulocentro'>(%) Sobre a Obra</td>"
									 .  "    <td colspan='1' rowspan='2' class='subtitulocentro'>Data de Início</td>"
									 .  "    <td colspan='1' rowspan='2' class='subtitulocentro'>Data de Término</td>"
									 .  "    <td colspan='2' rowspan='1' class='subtitulocentro'>Última Vistoria</td>"
									 .  "    <td colspan='2' rowspan='1' class='subtitulocentro'>Vistoria Atual</td>"
									 .  "</tr>"
									 .  "<tr>"
									 .  "    <td class='subtitulocentro'>(%) do Item já Executado</td>"
									 .  "    <td class='subtitulocentro'>(%) do Item já Executado <br/> sobre a Obra</td>"
									 .  "    <td class='subtitulocentro'>(%) Supervisão</td>"
									 .  "    <td class='subtitulocentro'>(%) do Item já Executado sobre a  <br/> Obra após Supervisão</td>"
									 .  "</tr>";
				
				for( $k = 0; $k < count($dadosItensVistoria); $k++ ){
					
					$cor = $k % 2 ? "#e0e0e0" : '#ffffff'; 
					
					$supervisao_exec_sobre_obra = ( ((float)$dadosItensVistoria[$k]["supvlrinfsupervisor"] * (float)$dadosItensVistoria[$k]["icopercsobreobra"]) / 100 );
					
					$totalValor 		= $totalValor 		  + $dadosItensVistoria[$k]["icovlritem"];
					$totalPercObra 		= $totalPercObra 	  + $dadosItensVistoria[$k]["icopercsobreobra"];
					$totalPercObraAnt   = $totalPercObraAnt   + $dadosItensVistoria[$k]["supvlritemsobreobraexecanterior"];
					$totalPercObraAtual = $totalPercObraAtual + $supervisao_exec_sobre_obra;
					
					$tabelaItensVistoria .= "<tr bgcolor='{$cor}'>"
										 .  "    <td>{$dadosItensVistoria[$k]["itcdesc"]}</td>"
										 .  "    <td align='right'>" . number_format( $dadosItensVistoria[$k]["icovlritem"], 2, ",", "." ) . "</td>"
										 .  "    <td align='right'>" . number_format( $dadosItensVistoria[$k]["icopercsobreobra"], 2, ",", "." ) . "</td>"
										 .  "    <td align='center'>{$dadosItensVistoria[$k]["inicio"]}</td>"
										 .  "    <td align='center'>{$dadosItensVistoria[$k]["termino"]}</td>"
										 .  "    <td align='right'>" . number_format( $dadosItensVistoria[$k]["supvlritemexecanterior"], 2, ",", "." ) . "</td>"
										 .  "    <td align='right'>" . number_format( $dadosItensVistoria[$k]["supvlritemsobreobraexecanterior"], 2, ",", "." ) . "</td>"
										 .  "    <td align='right'>" . number_format( $dadosItensVistoria[$k]["supvlrinfsupervisor"], 2, ",", "." ) . "</td>"
										 .  "    <td align='right'>" . number_format( $supervisao_exec_sobre_obra, 2, ",", "." ) . "</td>"
										 .  "</tr>";
					
				}

				$totalPercObra = ($totalPercObra > 100.00) ? 100.00 : $totalPercObra;
				
				$tabelaItensVistoria .= "<tr bgcolor='#d0d0d0'>"
									 .  "    <td align='right'><b>Total</b></td>"
									 .  "    <td align='right'><b>" . number_format( $totalValor, 2, ",", "." ) . "</b></td>"
									 .  "    <td align='right'><b>" . number_format( $totalPercObra, 2, ",", "." ) . "</b></td>"
									 .  "    <td align='right'><b></b></td>"
									 .  "    <td align='right'><b></b></td>"
									 .  "    <td align='right'><b></b></td>"
									 .  "    <td align='right'><b>" . number_format( $totalPercObraAnt, 2, ",", "." ) . "</b></td>"
									 .  "    <td align='right'><b></b></td>"
									 .  "    <td align='right'><b>" . number_format( $totalPercObraAtual, 2, ",", "." ) . "</b></td>"
									 .  "</tr>";
				
			}else{
				
				$tabelaItensVistoria .= "    <tr>"
									 .  "        <td align='center' style='color:#ee0000'>Não existem itens cadastradas para esta vistoria.</td>"
									 .  "    </tr>";
						
				
			}
			
			$tabelaItensVistoria .= "</table>";
			
			
			// fotos
			
			$WhereFotos = $fotoselecionadas != '' ? "fo.arqid IN (" . $fotoselecionadas . ") AND " : '';
			
			$sql = "SELECT 
						arqnome, 
						arq.arqid, 
						arq.arqextensao, 
						arq.arqtipo, 
						arq.arqdescricao 
					FROM 
						public.arquivo arq
					INNER JOIN 
						obras.fotos fo ON arq.arqid = fo.arqid
					WHERE
						{$WhereFotos}
						supvid = {$totVistorias[$i]["vistoria"]}
					ORDER BY 
						fotordem";
		
			$fotosVistoria = $db->carregar($sql);				 
							 
			if( $fotosVistoria ){
				
				$tabelaFotosVistoria = "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>";
				
				for( $a = 0; $a < count( $fotosVistoria ); $a++ ){
					
					if( $fotosVistoria[$a]["arqid"] ){
						
						$tabelaFotosVistoria .= "<tr>"
											 . "    <td align='center'>"
											 . "        <img src='../slideshow/slideshow/verimagem.php?newwidth=225&newheight=225&arqid={$fotosVistoria[$a]["arqid"]}' 
														hspace='3' vspace='3' style='width:125px; height:125px;' /> <br>{$fotosVistoria[$a]["arqdescricao"]}"
											 . "    </td>";
									 
					}
					
					$a = $a+1;
					
					if( $fotosVistoria[$a]["arqid"] ){
						
						$tabelaFotosVistoria .= "    <td align='center'>"
											 .  "       <img src='../slideshow/slideshow/verimagem.php?newwidth=225&newheight=225&arqid={$fotosVistoria[$a]["arqid"]}' 
														hspace='3' vspace='3' style='width:125px; height:125px;' /> <br>{$fotosVistoria[$a]["arqdescricao"]}"
											 .  "    </td>";
									 
					}
					
					$a = $a+1;
					
					if( $fotosVistoria[$a]["arqid"] ){
						
						$tabelaFotosVistoria .= "    <td align='center'>"
											 .  "        <img src='../slideshow/slideshow/verimagem.php?newwidth=225&newheight=225&arqid={$fotosVistoria[$a]["arqid"]}' 
														hspace='3' vspace='3' style='width:125px; height:125px;' /> <br>{$fotosVistoria[$a]["arqdescricao"]}"
											 .  "    </td>"
											 . "</tr>";
									 
					}
					
				}
		
				
				
			}else{
				$tabelaFotosVistoria = "    <table class='tabela' cellSpacing='1' cellPadding='3' align='center'><tr>"
									 .  "        <td align='center' style='color:#ee0000' colspan=\"2\">Não existem fotos cadastradas para esta vistoria.</td>"
									 .  "    </tr>";
			}			
			
			$tabelaFotosVistoria .= "</table>";
			
			
			$tabelaVistorias .= "<tr>"
							 .  "    <td align='center' colspan='2'>"
							 .  "        <table class='tabela' cellSpacing='1' cellPadding='3' align='center'>"
							 .  "            <tr>"
							 .  "                <td class='subtitulocentro' colspan='2'>Vistoria nº ". ($i + 1) . "</td>"
							 .  "            </tr>"
							 .  "            <tr>"
							 .  "                <td class='subtitulodireita' width='190px'>Responsável</td>"
							 .  "                <td bgcolor='#f5f5f5'>"
							 .  "                    {$totVistorias[$i]["responsavel"]}"
							 .  "                </td>"
							 .  "            </tr>"
							 .  "            <tr>"
							 .  "                <td class='subtitulodireita' width='190px'>Inserido Por</td>"
							 .  "                <td>"
							 .  "                    {$totVistorias[$i]["inseridopor"]}"
							 .  "                </td>"
							 .  "            </tr>"
							 .  "            <tr>"
							 .  "                <td class='subtitulodireita' width='190px'>Data da Vistoria</td>"
							 .  "                <td bgcolor='#f5f5f5'>"
							 .  "                    {$totVistorias[$i]["dtvistoria"]}"
							 .  "                </td>"
							 .  "            </tr>"
							 .  "            <tr>"
							 .  "                <td class='subtitulodireita'>Nome do Vistoriador</td>"
							 .  "                <td>"
							 .  "                    {$totVistorias[$i]["vistoriador"]}"
							 .  "                </td>"
							 .  "            </tr>"
							 .  "            <tr>"
							 .  "                <td class='subtitulodireita'>Situação atual</td>"
							 .  "                <td bgcolor='#f5f5f5'>"
							 .  "                     {$totVistorias[$i]["situacao"]}"
							 .  "                </td>"
							 .  "            </tr>"
							 .  "            <tr>"
							 .  "                <td class='subtitulodireita'>Projeto/Especificações</td>"
							 .  "                <td>"
							 .  "                    {$totVistorias[$i]["projetoespecificacoes"]}"
							 .  "                </td>"
							 .  "            </tr>"
							 .  "            <tr>"
							 .  "                <td class='subtitulodireita'>Placa da Obra</td>"
							 .  "                <td bgcolor='#f5f5f5'>"
							 .  "                    {$totVistorias[$i]["placaobra"]}"
							 .  "                </td>"
							 .  "            </tr>"
							 .  "            <tr>"
							 .  "                <td class='subtitulodireita'>Diário da Obra Atualizado</td>"
							 .  "                <td>"
							 .  "                    {$totVistorias[$i]["diarioobra"]}"
							 .  "                </td>"
							 .  "            </tr>"
							 .  "            <tr>"
							 .  "                <td class='subtitulodireita'>Placa Indicativa do Programa/Dados da obra</td>"
							 .  "                <td bgcolor='#f5f5f5'>"
							 .  "                    {$totVistorias[$i]["placalocalterreno"]}"
							 .  "                </td>"
							 .  "            </tr>"
							 .  "            <tr>"
							 .  "                <td class='subtitulodireita'>Validade do Alvará da Obra</td>"
							 .  "                <td>"
							 .  "                    {$totVistorias[$i]["validadealvara"]}"
							 .  "                </td>"
							 .  "            </tr>"
							 .  "            <tr>"
							 .  "                <td class='subtitulodireita'>Qualidade de Execução da Obra/Projeto</td>"
							 .  "                <td bgcolor='#f5f5f5'>"
							 .  "                    {$totVistorias[$i]["qualidadeobra"]}"
							 .  "                </td>"
							 .  "            </tr>"
							 .  "            <tr>"
							 .  "                <td class='subtitulodireita'>Desempenho da Construtora/Projetista</td>"
							 .  "                <td>"
							 .  "                    {$totVistorias[$i]["desempenho"]}"
							 .  "                </td>"
							 .  "            </tr>"
							 .  "            <tr>"
							 .  "                <td colspan='2'><b>Detalhamento de Vistoria e Acompanhamento</b></td>"
							 .  "            </tr>"
							 .  "            <tr>"
							 .  "                <td colspan='2'>{$tabelaItensVistoria}</td>"
							 .  "            </tr>"
							 .  "            <tr>"
							 .  "                <td class='subtitulodireita'>Observações da Vistora</td>"
							 .  "                <td bgcolor='#f5f5f5' align='justify'>"
							 .  "                    {$totVistorias[$i]["observacao"]}"
							 .  "                </td>"
							 .  "            </tr>"
							 .  "			<tr><td colspan='2'>{$tabelaFotosVistoria}</td></tr>"
							 .  "		 </table>"
							 .  "    </td>"
							 .  "</tr>";
		}
		
	}else{
		
		$tabelaVistorias .= "    <table align=\"center\"><tr>"
					     .  "        <td colspan='2' align='center' style='color:#ee0000'>Não existem vistorias cadastradas para a obra.</td>"
					     .  "    </tr></table>";
		
	}
	$dadosExtratoObras .= "<tr>"
					   .  "    <td colspan='2'>{$tabelaVistorias}</td>"
					   .  "</tr>";
	
}

// Inclusão de arquivos do componente de Entidade 
require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

print "<center>" . monta_cabecalho_relatorio( 95 ) . "</center>";

?>
<html>
	<head>
		<title><?php echo $GLOBALS['parametros_sistema_tela']['nome_e_orgao'];?></title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script type="text/javascript" src="../includes/prototype.js"></script>
	    <script type="text/javascript" src="../includes/entidades.js"></script>
	    <script type="text/javascript" src="/includes/estouvivo.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
		<style>
			 @media print {
			 	.notprint { display: none }
			 }
		</style>
	</head>
	<body>
<? 
	if ( $_GET['obrid'] && $_GET['traid'] ):
		$sql = "SELECT 
				   t.ttaid, ttadsc, usunome, umdidareaacresc, umdidareafinal, umdidareaalterada, 
			       obrid, tradsc, traseq, to_char(tradtassinatura, 'dd-mm-yyyy') as tradtassinatura, traprazovigencia, to_char(traterminovigencia, 'dd-mm-yyyy') as traterminovigencia, 
			       traprazoaditivadoexec, travlraditivo, travlrfinalobra, travlrqtdareaacresc, 
			       travlrqtdareafinal, travlrqtdareaalterada, trajustificativa, 
			       to_char(tradtinclusao, 'dd-mm-yyyy') as tradtinclusao, trastatus, to_char(traterminoexec, 'dd-mm-yyyy') as traterminoexec
			    FROM 
			    	obras.termoaditivo t
			    JOIN seguranca.usuario u USING(usucpf)
			    JOIN obras.tipotermoaditivo tt USING(ttaid)
			    WHERE
					traid = " . $_GET['traid'];
		$arrDadoAditivo = $db->pegaLinha( $sql );

//		$arrDadoAditivo['ttaid'] = 2;
?>	
	<table class="tabela" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="subtitulocentro" colspan="4" height="25px;">Dados do Aditivo</td>
		</tr>
		<tr>
			<td class="subtitulodireita" style="font-weight: bold;" width="190px;">Nº do Aditivo</td>
			<td colspan="3" bgcolor="#f5f5f5">
				<?=$arrDadoAditivo['traseq']?>
			</td>
		</tr>	
		<tr>
			<td class="subtitulodireita" style="font-weight: bold;" width="190px;">Denominação</td>
			<td colspan="3" bgcolor="#f5f5f5">
				<?=$arrDadoAditivo['tradsc']?>
			</td>
		</tr>	
		<tr>
			<td class="subtitulodireita" style="font-weight: bold;" width="190px;">Tipo de Aditivo</td>
			<td colspan="3" bgcolor="#f5f5f5">
				<?=$arrDadoAditivo['ttadsc']?>
			</td>
		</tr>	
		<tr>
			<td class="subtitulodireita" style="font-weight: bold;" width="190px;">Data de Assinatura do Aditivo</td>
			<td colspan="3" bgcolor="#f5f5f5">
				<?=$arrDadoAditivo['tradtassinatura']?>
			</td>
		</tr>	
<?		
	if ( $arrDadoAditivo['ttaid'] == 1 || $arrDadoAditivo['ttaid'] == 3 ):
?>
		<tr>
			<td class="subtitulodireita" style="font-weight: bold;" width="190px;">Prazo de Vigência do Aditivo</td>
			<td colspan="3" bgcolor="#f5f5f5">
				<?=$arrDadoAditivo['traprazovigencia']?>
			</td>
    	</tr>
		<tr>
			<td class="subtitulodireita" style="font-weight: bold;" width="190px;">Término da Vigência do Aditivo</td>
			<td colspan="3" bgcolor="#f5f5f5">
				<?=$arrDadoAditivo['traterminovigencia']?>
			</td>
    	</tr>
		<tr>
			<td class="subtitulodireita" style="font-weight: bold;" width="190px;">Prazo aditivado para Execução</td>
			<td colspan="3" bgcolor="#f5f5f5">
				<?=$arrDadoAditivo['traprazoaditivadoexec']?>
			</td>
    	</tr>
		<tr>
			<td class="subtitulodireita" style="font-weight: bold;" width="190px;">Término da Execução do Aditivo</td>
			<td colspan="3" bgcolor="#f5f5f5">
				<?=$arrDadoAditivo['traterminoexec']?>
			</td>
    	</tr>
<? 
	endif;
	if ( $arrDadoAditivo['ttaid'] == 2 || $arrDadoAditivo['ttaid'] == 3 ):
?>
		<tr>
			<td class="subtitulodireita" style="font-weight: bold;" width="190px;">Valor do Aditivo(R$)</td>
			<td colspan="3" bgcolor="#f5f5f5">
				<?=$arrDadoAditivo['travlraditivo']?>
			</td>
		</tr>	
		<tr>
			<td class="subtitulodireita" style="font-weight: bold;" width="190px;">Valor Final da Obra Incluindo Aditivo(R$)</td>
			<td colspan="3" bgcolor="#f5f5f5">
				<?=$arrDadoAditivo['travlrfinalobra']?>
			</td>
		</tr>		
<? 
		if ( $arrDadoAditivo['ttaid'] == 2 ):
?>		
		<tr>
			<td class="subtitulodireita" style="font-weight: bold;" width="190px;">Acréscimo ou Supressão de Área/Quantidade</td>
			<td bgcolor="#f5f5f5">
				<?=$arrDadoAditivo['travlrqtdareaacresc']?>
			</td>
			<td class="subtitulodireita" style="font-weight: bold;" width="190px;">Unidade de medida</td>
			<td bgcolor="#f5f5f5">
				<?=$arrDadoAditivo['umdidareaacresc']?>
			</td>
		</tr>	
<? 
		else:
?>		
		<tr>
			<td class="subtitulodireita" style="font-weight: bold;" width="190px;">Alteração da Área/Quantidade</td>
			<td bgcolor="#f5f5f5">
				<?=$arrDadoAditivo['travlrqtdareaalterada']?>
			</td>
			<td class="subtitulodireita" style="font-weight: bold;" width="190px;">Unidade de medida</td>
			<td bgcolor="#f5f5f5">
				<?=$arrDadoAditivo['umdidareaalterada']?>
			</td>
		</tr>	
<? 
		endif;
?>		
		<tr>
			<td class="subtitulodireita" style="font-weight: bold;" width="190px;">Área/Quantidade Final Incluindo Aditivo(R$)</td>
			<td bgcolor="#f5f5f5">
				<?=$arrDadoAditivo['travlrqtdareafinal']?>
			</td>
			<td class="subtitulodireita" style="font-weight: bold;" width="190px;">Unidade de medida</td>
			<td bgcolor="#f5f5f5">
				<?=$arrDadoAditivo['umdidareafinal']?>
			</td>
		</tr>	
<? 
	endif;
?>	
		<tr>
			<td class="subtitulodireita" style="font-weight: bold;" width="190px;">Justificativa</td>
			<td colspan="3" bgcolor="#f5f5f5">
				<?=$arrDadoAditivo['trajustificativa']?>
			</td>
    	</tr>
	</table>	
	<br>
<? 
	endif;
?>    		
	<table class="tabela" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="subtitulocentro" colspan="2" height="25px;">Dados da Obra</td>
		</tr>
		<tr>
			<td class="subtitulodireita" style="font-weight: bold;" width="190px;">Tipo de estabelecimento</td>
			<td bgcolor="#f5f5f5">
				<?php
					
					if( $orgid = $dobras->getOrgId() ){
						
						$sql = "SELECT 
									orgdesc as descricao 
								FROM 
									obras.orgao 
								WHERE 
									orgid = {$orgid}";
						
						print $db->pegaUm( $sql );
						
					}
				?>
			</td>
    	</tr>	
    	<tr>
			<td class="subtitulodireita" style="font-weight: bold;">Unidade ResponsÃ¡vel pela Obra</td>
			<td>
				<?php
//					$entidade = new Entidade( $dobras->getEntIdUnidade() );
//					print $entnome = $entidade->entnome;

					$sql = "SELECT
								entnome
							FROM
								entidade.entidade
							WHERE
								entid = {$dobras->getEntIdUnidade()}";
								
					print $db->pegaUm( $sql );
				
				?>
			</td>
    	</tr>
    	<!-- <tr>
			<td class="subtitulodireita" style="font-weight: bold;">Campus / Reitoria</td>
			<td bgcolor="#f5f5f5">
				<?php
//					$sql = "SELECT
//								entnome
//							FROM
//								entidade.entidade
//							WHERE
//								entid = ".(int)$dobras->getEntIdCampus();
								
					//print $db->pegaUm( $sql );
				?>
			</td>
    	</tr> -->
    	<tr>
			<td class="subtitulodireita" style="font-weight: bold;">Nome da Obra</td>
			<td>
				<?php
					print $dobras->getObrDesc();
				?>
			</td>
    	</tr>
    	<tr>
    	<tr>
			<td class="subtitulodireita" style="font-weight: bold;">Tipo de Obra</td>
			<td>
				<?php
					$tobraid = $dobras->getTobraId();
					$sql = "SELECT 
								tobadesc 
							FROM 
								obras.tipoobra
							WHERE
								tobaid = ".(int)$dobras->getTobraId();
					
					print $db->pegaUm( $sql );
				?>
			</td>
    	</tr>
    	<tr>
			<td class="subtitulodireita" style="font-weight: bold;">Subação</td>
			<td bgcolor="#f5f5f5">
				<?php
					if( $prfid = $dobras->getPrfId() ){
						
						$sql = "SELECT 
									prfdesc as descricao
								FROM 
									obras.programafonte
								WHERE
									prfid= ".$prfid;
						
						print $db->pegaUm( $sql );
						
					}else{
						print "Não informado";
					}
					
				?>
			</td>
    	</tr>
    	<tr>
			<td class="subtitulodireita" style="font-weight: bold;">Classificação da Obra</td>
			<td>
				<?php
					
					if( $cloid = $dobras->getCloId() ){
						
						$sql = "SELECT 
									clodsc as descricao
								FROM
									obras.classificacaoobra
								WHERE
									cloid = ".$cloid;
						
						print $db->pegaUm( $sql );
						
					}else{
						print "Não informado";
					}
					
				?>
			</td>
    	</tr>
    	<tr>
			<td class="subtitulodireita" style="font-weight: bold;">Tipologia da Obra</td>
			<td bgcolor="#f5f5f5">
				<?php
					
					if ( $tpoid = $dobras->getTpoId() ){
						
						$sql = "SELECT 
									tpodsc as descricao 
								FROM 
									obras.tipologiaobra
								WHERE
									tpoid = ".$tpoid;
						
						print $db->pegaUm( $sql );
						
					}else{
						print "Não informado";
					}
					
				?>
			</td>
    	</tr>						
    	<tr>
			<td class="subtitulodireita" style="font-weight: bold;">Descrição / Composição da Obra</td>
			<td align="justify">
				<?php
					print $dobras->getObrComposicao() ? $dobras->getObrComposicao() : "Não Informado";
				?>
			</td>
    	</tr>	
		<tr>
			<td class="subtitulodireita" style="font-weight: bold;">Observação sobre a Obra</td>
			<td bgcolor="#f5f5f5" align="justify">
				<?php
					print $dobras->getObsObra() ? $dobras->getObsObra() : "Não Informado";
				?>
			</td>
    	</tr>	    		
		<tr>
			<td class="subtitulodireita" style="font-weight: bold;">Valor Previsto (R$):</td>
			<td bgcolor="#f5f5f5" align="justify">
				<?php
					print $dobras->getObrValorPrevisto() ? number_format($dobras->getObrValorPrevisto(), 2, ",", ".") : "Não Informado";
				?>
			</td>
    	</tr>	    		
    	<!-- <tr>
			<td class="subtitulodireita" style="font-weight: bold;">(%) Concluído</td>
			<td>
			<?php
				
//				$percentualExecutado = $obras->ViewPercentualExecutado($obrid);
//				
//				if( !$percentualExecutado ){ 
//					$percentualExecutado = 0; 
//				}
//				
//				$percentualExecutado = ( $percentualExecutado > 100.00 ) ? 100.00 : $percentualExecutado;
//				$percentualExecutado = number_format($percentualExecutado, 2, ',', '.');
//				
//				print $percentualExecutado .= " %";
				
			?>
			</td>
    	</tr> -->
    	
    	<?php print $dadosExtratoObras; ?>
    </table>
    <table class="tabela" cellSpacing="1" cellPadding="3"	align="center">	
		<tr bgcolor="#D0D0D0">
			<td>
				<input type="button" value="Imprimir" onclick="self.print();" style="cursor: pointer;"/>
				<input type="button" value="Fechar" onclick="self.close();" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
	</body>
</html>
