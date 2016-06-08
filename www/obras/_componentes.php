<?php
include_once(APPRAIZ.'includes/classes/MontaListaAjax.class.inc');
require_once APPRAIZ . "includes/classes/dateTime.inc";

class obrasNovo extends Data{
	
	public $db;
	
	/**
	 * Funï¿½ï¿½o construtora das classes que cria os sets
	 * 
	 * @param array $dados
	 * @author Orion Teles de Mesquita
 	 * @since 18/08/2009
 	 * 
	 */
	function __construct( $dados = array() ){

		global $db;
		$this->db = $db;
		
		if( is_array( $dados ) ){
			foreach( $dados as $stAtributo => $mxValor ){
				if( property_exists($this, $stAtributo) ){
					$this->$stAtributo = $mxValor;
				}
			}
		}
		
	}
	
}

class inicio extends obrasNovo{

	function filtraListaDeObras( $dados ){
		
		$_SESSION["obras"]["filtros"] = null;
		
		
		if( !empty( $dados["agrupamento"] ) ){
			$_SESSION["obras"]["filtros"]["agrupamento"] = $dados["agrupamento"];
		}
		
		if ( !empty( $dados["supervisao"] ) ) {
			$_SESSION["obras"]["filtros"]["supervisao"] = $dados["supervisao"];
		}
		
		if ( !empty( $dados['vlrmaior'] ) && !empty( $dados['vlrmenor'] ) ) {
			$_SESSION["obras"]["filtros"]["vlrmaior"] = $dados['vlrmaior'];
			$_SESSION["obras"]["filtros"]["vlrmenor"] = $dados['vlrmenor'];
		}
		
		if( !empty( $dados["tobaid"] ) ){
			
			$_SESSION["obras"]["filtros"]["tobaid"] = $dados["tobaid"];
			$filtro .= " AND oi.tobraid = {$dados["tobaid"]} ";
			
		}
		 
		if( !empty( $dados["stoid"] ) ){
			
			$_SESSION["obras"]["filtros"]["stoid"] = $dados["stoid"];
			$filtro .= " AND oi.stoid = {$dados["stoid"]} ";
			
		}
		 
		if( !empty( $dados["cloid"] ) ){
			
			$_SESSION["obras"]["filtros"]["cloid"] = $dados["cloid"];
			$filtro .= " AND oi.cloid = {$dados["cloid"]} ";
			
		}
		 
		if( !empty( $dados["prfid"] ) ){
			
			$_SESSION["obras"]["filtros"]["prfid"] = $dados["prfid"];
			$filtro .= " AND oi.prfid = {$dados["prfid"]} ";
			
		}
		 
		if( !empty( $dados["entidunidade"] ) ){
			
			$_SESSION["obras"]["filtros"]["entidunidade"] = $dados["entidunidade"];
			$filtro .= " AND oi.entidunidade = {$dados["entidunidade"]} ";
			
		}
		
		if( !empty( $dados["ultatualizacao"] ) ){
			$_SESSION["obras"]["filtros"]["ultatualizacao"] = $dados["ultatualizacao"];

			switch( $_SESSION["obras"]["filtros"]["ultatualizacao"] ){
				case 1:
					$filtro .= " AND DATE_PART('days', NOW() - obrdtvistoria) <= 45";
					break;
				case 2:
					$filtro .= " AND DATE_PART('days', NOW() - obrdtvistoria) BETWEEN 46 AND 60";
					break;
				case 3:
					$filtro .= " AND DATE_PART('days', NOW() - obrdtvistoria) > 60";
					break;
			}
		}
		 
		if( !empty( $dados["obrtextobusca"] ) ){
			
			$_SESSION["obras"]["filtros"]["obrtextobusca"] = $dados["obrtextobusca"];
			
			$filtro .= " AND ( upper(oi.obrdesc) ilike upper('%{$dados["obrtextobusca"]}%') OR ";
			$filtro .= " upper(ee.entnome) ilike upper('%{$dados["obrtextobusca"]}%') OR ";
			$filtro .= " upper(tm.mundescricao) ilike upper('%{$dados["obrtextobusca"]}%') OR ";
			$filtro .= " upper(mpi.plicod) ilike upper('%{$dados["obrtextobusca"]}%') OR ";
			$filtro .= " upper(oi.obrdesc) ilike upper('%{$dados["obrtextobusca"]}%') OR ";
			$filtro .= " oi.obrid =".(int)$dados["obrtextobusca"]." ) "; // busca pelo campo ID
			
		}
		 
		if( !empty( $dados["estuf"] ) ){
			
			$_SESSION["obras"]["filtros"]["estuf"] = $dados["estuf"];
			$filtro .= " AND ed.estuf = '{$dados["estuf"]}'";
			
		}
		 
		$_SESSION["obras"]["filtros"]["foto"] = $dados["foto"];
		
		switch( $dados["foto"] ){
			
			case "S":
				$filtro .= " AND af.obrid IS NOT NULL ";
			break;
				
			case "N":
				$filtro .= " AND af.obrid IS NULL ";
			break;
			
		}
		
		$_SESSION["obras"]["filtros"]["vistoria"] = $dados["vistoria"];
	
		switch( $dados["vistoria"] ){
			
			case "S":
				$filtro .= " AND ov.obrid IS NOT NULL ";
			break;
				
			case "N":
				$filtro .= " AND ov.obrid IS NULL ";
			break;
			
		}
		
		$_SESSION["obras"]["filtros"]["restricao"] = $dados["restricao"];
	
		switch( $dados["restricao"] ){
			
			case "S":
				$filtro .= " AND re.obrid IS NOT NULL ";
			break;
				
			case "N":
				$filtro .= " AND re.obrid IS NULL ";
			break;
			
		}
		
		$_SESSION["obras"]["filtros"]["aditivo"] = $dados["aditivo"];
		
		switch( $dados["aditivo"] ){
			
			case "S":
				$filtro .= " AND ta.traid IS NOT NULL";
			break;
				
			case "N":
				$filtro .= " AND ta.traid IS NULL";
			break;
			
		}
		
		switch( $dados["supervisao"] ){
			
			case "S":
				$filtro .= " AND oi.obrsupemp = TRUE ";
			break;
				
			case "N":
				$filtro .= " AND oi.obrsupemp = FALSE ";
			break;
			
		}
		
		if ( $dados["vlrmenor"] && $dados["vlrmaior"] ) {
			$filtro .= " AND oi.obrvlrrealobra BETWEEN ".$dados["vlrmenor"]." AND ".$dados["vlrmaior"]." ";
		}
		
		if ( (int)$dados["percentualinicial"] > 0 ) {
			
			$_SESSION["obras"]["filtros"]["percentualinicial"] = $dados["percentualinicial"];
			$_SESSION["obras"]["filtros"]["percentualfinal"]   = $dados["percentualfinal"];
			
			$perc = (int)$dados["percentualfinal"] == 100 ? 110 : $dados["percentualfinal"];
			$filtro .= " AND ( (pe.total_exec BETWEEN {$dados["percentualinicial"]} AND {$perc}))";
			
		}elseif ((int)$dados["percentualinicial"] == '0') {
			if ( (int)$dados["percentualfinal"] > 0 ) {
				if( !((int)$dados["percentualfinal"] == 100) ){
					$_SESSION["obras"]["filtros"]["percentualinicial"] = $dados["percentualinicial"];
					$_SESSION["obras"]["filtros"]["percentualfinal"]   = $dados["percentualfinal"];
	
					$perc = (int)$dados["percentualfinal"] == 100 ? 110 : $dados["percentualfinal"];
					$filtro .= " AND ( (pe.total_exec IS NULL OR pe.total_exec BETWEEN {$dados["percentualinicial"]} AND {$perc}))";
				}
			}elseif ( (int)$dados["percentualfinal"] == 0 ) {
				
				$_SESSION["obras"]["filtros"]["percentualinicial"] = $dados["percentualinicial"];
				$_SESSION["obras"]["filtros"]["percentualfinal"]   = $dados["percentualfinal"];
				
				$filtro .= " AND ( (pe.total_exec = 0 OR pe.total_exec IS NULL))";
				
			}
		}
		
		return $filtro;
		
	}
	
	function listaDeObras( $filtros ){

		$arEntid = str_replace("}{",",",$_SESSION["obrasarvore"]["arEntid"]);
		$arEntid = str_replace("{","",$arEntid);
		$arEntid = str_replace("}","",$arEntid);
		$arEntid = explode(",",$arEntid);
		
		if( $_SESSION["obras"]["filtros"]["agrupamento"] == "" || $_SESSION["obras"]["filtros"]["agrupamento"] == "U" ){
//			$entidadesPermitidas = obras_pegarUnidadesPermitidas();
//			
//			if( count($entidadesPermitidas) > 0 && is_array( $entidadesPermitidas ) ){
//				$whereEntidades = "ee.entid in (" . implode( ", ", $entidadesPermitidas ) . ") AND ";
//			}

			$arFiltroPermissao = obras_permissaoPerfil();
			if ( is_array( $arFiltroPermissao ) ){
				if ($arFiltroPermissao['unidade'])
					$where[] = "ee.entid IN (" . implode(', ', $arFiltroPermissao['unidade']) . ")";
				if ($arFiltroPermissao['orgao'])
					$where[] = "oi.orgid IN (" . implode(', ', $arFiltroPermissao['orgao']) . ")";
				if ($arFiltroPermissao['estado'])
					$where[] = "ed.estuf IN ('" . implode("', '", $arFiltroPermissao['estado']) . "')";
				if ($arFiltroPermissao['campus'])
					$where[] = "oi.entidcampus IN ('" . implode("', '", $arFiltroPermissao['campus']) . "')";
				if ($arFiltroPermissao['obra'])
					$where[] = "oi.obrid IN ('" . implode("', '", $arFiltroPermissao['obra']) . "')";
									
				if (count($where) > 0){	
					$where = "(" . implode(" OR ", $where) . ") AND ";
				}else{
					$where = "(1=2) AND ";
				}	
			}
			
			$sql = "SELECT DISTINCT
						ee.entid as id,
						CASE WHEN ee.entnome is not null AND ee.entsig NOT LIKE '--'
							THEN upper(ee.entsig) || ' - ' || upper(ee.entnome) 
							ELSE upper(ee.entnome) 
						END as unidade,
						count(oi.obrid) as total
					FROM 
						obras.obrainfraestrutura oi
					INNER JOIN
						entidade.entidade ee ON ee.entid = oi.entidunidade
					INNER JOIN
						entidade.endereco ed ON ed.endid = oi.endid
					INNER JOIN
						territorios.municipio tm ON tm.muncod = ed.muncod
					INNER JOIN
						obras.situacaoobra so ON so.stoid = oi.stoid
					LEFT JOIN
						(SELECT 
							SUM(icopercexecutado) as total_exec, 
							obrid 
						FROM 
							obras.itenscomposicaoobra itco
						WHERE 
							icostatus = 'A'
							AND icovigente = 'A' 
						GROUP BY obrid ) pe ON pe.obrid = oi.obrid		
					LEFT JOIN
						( SELECT max(aqoid) as arquivo, obrid FROM obras.arquivosobra WHERE aqostatus = 'A' and tpaid <> 21 GROUP BY obrid ) ao ON ao.obrid = oi.obrid
					LEFT JOIN
						( SELECT max(aqoid) as foto, obrid FROM obras.arquivosobra WHERE aqostatus = 'A' and tpaid = 21 GROUP BY obrid ) af ON af.obrid = oi.obrid
					LEFT JOIN
						( SELECT max(rstoid) as restricao, obrid FROM obras.restricaoobra WHERE rststatus = 'A' GROUP BY obrid ) re ON re.obrid = oi.obrid
					LEFT JOIN
						( SELECT supvid as supervisao, obrid FROM obras.supervisao WHERE supstatus = 'A' ORDER BY supvdt DESC LIMIT 1 ) ov ON ov.obrid = oi.obrid
					WHERE
						--{$whereEntidades} 
						{$where} 
						obsstatus = 'A' AND 
						oi.orgid = {$_SESSION["obras"]["orgid"]} 
						{$filtros}
					GROUP BY
						ee.entnome,
						ee.entid,
						ee.entsig
					ORDER BY
						unidade";
			//dbg(sql);
						
			//paginaï¿½ï¿½o				
			$perpage = 50;
			$pages = 10;
			$numero = $_REQUEST["numero"];
			if(!$numero) $numero = 1;
						
			$sqlCount = "select 
							count(1)
						 from (" . $sql . ") rs";
			
			$totalRegistro = $this->db->pegaUm($sqlCount);
			
			$sql = $sql . " LIMIT {$perpage} offset ".($numero-1);	
				
			$unidades = $this->db->carregar( $sql );
	    
			$nlinhas = count($unidades);
			if (!$unidades) $nl = 0; else $nl=$nlinhas;
			$reg_fim = $nlinhas;
			if ($nl>0) $total_reg = $totalRegistro;
			//fim paginaï¿½ï¿½o		
			
			if( is_array( $unidades ) ){
				
				print "<table class='tabela' bgcolor='#ffffff' cellspacing='1' cellpadding='3' align='center'>"
					. "    <tr>"
					. "        <td class='SubTituloCentro'>Ação</td>"
					. "        <td class='SubTituloCentro'>Unidade Responsável pela Obra</td>"
					. "        <td class='SubTituloCentro'>Quantidade de Obras</td>"
					. "    </tr>";
				
				$ar = 0;	
					
				for( $i = 0; $i < count($unidades); $i++ ){
					
					$totalObras = $totalObras + $unidades[$i]["total"];
					
					$cor = ($i % 2) ? "" : "#F7F7F7";
					
					if(in_array(strval($unidades[$i]["id"]), $arEntid)){
						print "<tr bgcolor='{$cor}'>"
							. "    <td style='text-align:center;'>"
							. "        <img src='../imagens/menos.gif' id='imgFilhoLista{$unidades[$i]["id"]}' style='cursor:pointer;' onclick='obrMostraFilho( {$unidades[$i]["id"]}, \"campus\", \"fecha\" );'/>"
							. "    </td>"
							. "    <td>{$unidades[$i]["unidade"]}</td>"
							. "    <td style='text-align:right; color:#0066cc;'>{$unidades[$i]["total"]}</td>"
							. "</tr>"
							. "<tr id='trFilhoLista{$unidades[$i]["id"]}' style='display:table-row;'>"
							. "    <td style='text-align:center;'>"
							. "        <img src='../imagens/seta_filho.gif'/>"
							. "    </td>"
							. "    <td colspan='2'>"
							.			"<div id='divFilhoLista{$unidades[$i]["id"]}'>";

						$this->listaFilhos( $unidades[$i]["id"], $filtros);
						
						print 			"</div></td>"
							. "</tr>";
					}else{
						print "<tr bgcolor='{$cor}'>"
							. "    <td style='text-align:center;'>"
							. "        <img src='../imagens/mais.gif' id='imgFilhoLista{$unidades[$i]["id"]}' style='cursor:pointer;' onclick='obrMostraFilho( {$unidades[$i]["id"]}, \"campus\", \"abre\" );'/>"
							. "    </td>"
							. "    <td>{$unidades[$i]["unidade"]}</td>"
							. "    <td style='text-align:right; color:#0066cc;'>{$unidades[$i]["total"]}</td>"
							. "</tr>"
							. "<tr id='trFilhoLista{$unidades[$i]["id"]}' style='display:none;'>"
							. "    <td style='text-align:center;'>"
							. "        <img src='../imagens/seta_filho.gif'/>"
							. "    </td>"
							. "    <td colspan='2'>"
							.			"<div id='divFilhoLista{$unidades[$i]["id"]}'></div></td>"
							. "</tr>";
					}	
					
				
				}
				
				print "<tr bgcolor='#D0D0D0' style='text-align:right;'>"
					. "    <td></td>"
					. "    <td><b>Total de obras</b></td>"
					. "    <td style='color:#0066cc;'><b>{$totalObras}</b></td>"
					. "</tr>" 
					. "</table>";

				if ($nl>0){
					print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem"><tr bgcolor="#ffffff"><td><b>Total de Registros: ' . $totalRegistro . '</b></td><td>';
					include APPRAIZ."includes/paginacao.inc";
					print '</td></tr></table>';
					print '<script language="JavaScript">function pagina(numero) {document.formulario.numero.value=numero;document.formulario.submit();}</script>';
				}

				
			}else{
				
				print "<table class='tabela' bgcolor='#ffffff' cellspacing='1' cellpadding='3' align='center'>"
					. "    <tr>"
					. "    <td style='text-align:center; color:ee0000;'>Não foram encontradas obras.</td>"
					. "    </tr>"
					. "</table>";
				
			}
			
		} else {
			
			if(!$_REQUEST['pagina']) {
				$_REQUEST['pagina'] = 0;
			}
			
			$this->listaObras( "", $filtros, "total", 100, $_REQUEST['pagina']);
			
		}
		
	}
	
	function listaFilhos( $entid, $filtros ){
		
		$arEntid = str_replace("}{",",",$_SESSION["obrasarvore"]["arEntidCampus"]);
		$arEntid = str_replace("{","",$arEntid);
		$arEntid = str_replace("}","",$arEntid);
		$arEntid = explode(",",$arEntid);
		
		if( ( $_SESSION["obras"]["orgid"] == ORGAO_FNDE || 
			  $_SESSION["obras"]["orgid"] == ORGAO_REHUF || 
			  $_SESSION["obras"]["orgid"] == ORGAO_ADM ) ){
			$this->listaObras( $entid, $filtros, '', '', '', 'simples' );
		}else{
			$arFiltroPermissao = obras_permissaoPerfil();
			if ( is_array( $arFiltroPermissao ) ){
				if ($arFiltroPermissao['unidade'])
					$where[] = "ea.entid IN (" . implode(', ', $arFiltroPermissao['unidade']) . ")";
				if ($arFiltroPermissao['orgao'])
					$where[] = "oi.orgid IN (" . implode(', ', $arFiltroPermissao['orgao']) . ")";
				if ($arFiltroPermissao['estado'])
					$where[] = "ed.estuf IN ('" . implode("', '", $arFiltroPermissao['estado']) . "')";
				if ($arFiltroPermissao['campus'])
					$where[] = "oi.entidcampus IN ('" . implode("', '", $arFiltroPermissao['campus']) . "')";
				if ($arFiltroPermissao['obra'])
					$where[] = "oi.obrid IN ('" . implode("', '", $arFiltroPermissao['obra']) . "')";
				
				if (count($where) > 0){	
					$where = "(" . implode(" OR ", $where) . ") AND ";
				}else{
					$where = "(1=2) AND ";
				}
			}
			
						
			$sql = "(SELECT 
						ee.entid as id,
						upper(ee.entnome) as nome,
					 	count(oi.obrid) as total
					FROM 
						entidade.entidade ee
					LEFT JOIN
						obras.obrainfraestrutura oi ON oi.entidcampus = ee.entid
					LEFT JOIN 
						entidade.funcaoentidade ef on ee.entid = ef.entid
					LEFT JOIN 
						entidade.funentassoc ea on ea.fueid = ef.fueid
					INNER JOIN
						entidade.endereco ed ON ed.endid = oi.endid
					INNER JOIN
						territorios.municipio tm ON tm.muncod = ed.muncod
					INNER JOIN
						obras.situacaoobra so ON so.stoid = oi.stoid
					LEFT JOIN
						(SELECT 
							SUM(icopercexecutado) as total_exec, 
							obrid 
						FROM 
							obras.itenscomposicaoobra itco
						WHERE 
							icostatus = 'A'
							AND icovigente = 'A' 
						GROUP BY obrid ) pe ON pe.obrid = oi.obrid		
					LEFT JOIN
						( SELECT max(aqoid) as arquivo, obrid FROM obras.arquivosobra WHERE aqostatus = 'A' and tpaid <> 21 GROUP BY obrid ) ao ON ao.obrid = oi.obrid
					LEFT JOIN
						( SELECT max(aqoid) as foto, obrid FROM obras.arquivosobra WHERE aqostatus = 'A' and tpaid = 21 GROUP BY obrid ) af ON af.obrid = oi.obrid
					LEFT JOIN
						( SELECT max(rstoid) as restricao, obrid FROM obras.restricaoobra WHERE rststatus = 'A' GROUP BY obrid ) re ON re.obrid = oi.obrid
					LEFT JOIN
						( SELECT supvid as supervisao, obrid FROM obras.supervisao WHERE supstatus = 'A' ORDER BY supvdt DESC LIMIT 1 ) ov ON ov.obrid = oi.obrid
					WHERE 
						{$where}
						ef.funid in (17,18,75) AND 
						ea.entid = {$entid} AND
						oi.obsstatus = 'A' {$filtros}
					GROUP BY
						ee.entnome,
						ee.entid
					ORDER BY
						nome)
						
					UNION ALL --
					
					(SELECT 
						0 as id,
						'Sem campus informado' as nome,
					 	count(oi.obrid) as total
					FROM 
						obras.obrainfraestrutura oi
					INNER JOIN
						entidade.entidade ee ON ee.entid = oi.entidunidade
					INNER JOIN
						entidade.endereco ed ON ed.endid = oi.endid
					LEFT JOIN
						(SELECT 
							SUM(icopercexecutado) as total_exec, 
							obrid 
						FROM 
							obras.itenscomposicaoobra itco
						WHERE 
							icostatus = 'A'
							AND icovigente = 'A' 
						GROUP BY obrid ) pe ON pe.obrid = oi.obrid		
					LEFT JOIN
						territorios.municipio tm ON tm.muncod = ed.muncod
					LEFT JOIN
						obras.situacaoobra so ON so.stoid = oi.stoid
					LEFT JOIN
						( SELECT max(aqoid) as arquivo, obrid FROM obras.arquivosobra WHERE aqostatus = 'A' and tpaid <> 21 GROUP BY obrid ) ao ON ao.obrid = oi.obrid
					LEFT JOIN
						( SELECT max(aqoid) as foto, obrid FROM obras.arquivosobra WHERE aqostatus = 'A' and tpaid = 21 GROUP BY obrid ) af ON af.obrid = oi.obrid
					LEFT JOIN
						( SELECT max(rstoid) as restricao, obrid FROM obras.restricaoobra WHERE rststatus = 'A' GROUP BY obrid ) re ON re.obrid = oi.obrid
					LEFT JOIN
						( SELECT supvid as supervisao, obrid FROM obras.supervisao WHERE supstatus = 'A' ORDER BY supvdt DESC LIMIT 1 ) ov ON ov.obrid = oi.obrid
					WHERE 
						oi.entidcampus is null AND
						oi.entidunidade = {$entid} AND
						oi.obsstatus = 'A' {$filtros})";

			$campus = $this->db->carregar( $sql ); 
			
			if( is_array( $campus ) ){
				
				print "<table class='tabela' bgcolor='#ffffff' cellspacing='1' cellpadding='3' align='center'>"
					. "    <tr>"
					. "        <td class='SubTituloCentro'>Ação</td>"
					. "        <td class='SubTituloCentro'>Campus</td>"
					. "        <td class='SubTituloCentro'>Quantidade de Obras</td>"
					. "    </tr>";
				
				for( $i = 0; $i < count($campus); $i++ ){
					
					$cor = ($i % 2) ? "" : "#F7F7F7";
					
					if ( $campus[$i]["id"] == 0 ){
						if( $campus[$i]["total"] != 0 ){
							print "<tr bgcolor='{$cor}'>"
								. "    <td style='text-align:center;'>"
								. "        <img src='../imagens/mais.gif' id='imgFilhoObra{$entid}' style='cursor:pointer;' onclick='obrMostraFilho( {$entid}, \"obrasSC\", \"abre\" );'/>"
								. "    </td>"
								. "    <td>{$campus[$i]["nome"]}</td>"
								. "    <td style='text-align:right; color:#0066cc;'>{$campus[$i]["total"]}</td>"
								. "</tr>"
								. "<tr id='trFilhoObra{$entid}' style='display:none;'>"
								. "    <td style='text-align:center;'>"
								. "        <img src='../imagens/seta_filho.gif'/>"
								. "    </td>"
								. "    <td colspan='2'><div id='divFilhoObra{$entid}'></div></td>"
								. "</tr>";
						}
					}else{
						
						if(in_array(strval($campus[$i]["id"]), $arEntid)){
							print "<tr bgcolor='{$cor}'>"
								. "    <td style='text-align:center;'>"
								. "        <img src='../imagens/menos.gif' id='imgFilhoObra{$campus[$i]["id"]}' style='cursor:pointer;' onclick='obrMostraFilho( {$campus[$i]["id"]}, \"obras\", \"fecha\" );'/>"
								. "    </td>"
								. "    <td>{$campus[$i]["nome"]}</td>"
								. "    <td style='text-align:right; color:#0066cc;'>{$campus[$i]["total"]}</td>"
								. "</tr>"
								. "<tr id='trFilhoObra{$campus[$i]["id"]}' style='display:table-row;'>"
								. "    <td style='text-align:center;'>"
								. "        <img src='../imagens/seta_filho.gif'/>"
								. "    </td>"
								. "    <td colspan='2'><div id='divFilhoObra{$campus[$i]["id"]}'>";
//							dbg($arEntid,1);	
							$this->listaObras( $campus[$i]["id"], $filtros, '', '', '', 'simples' );
							
							print "</div></td>"
								. "</tr>";
							
						}else{
							print "<tr bgcolor='{$cor}'>"
								. "    <td style='text-align:center;'>"
								. "        <img src='../imagens/mais.gif' id='imgFilhoObra{$campus[$i]["id"]}' style='cursor:pointer;' onclick='obrMostraFilho( {$campus[$i]["id"]}, \"obras\", \"abre\" );'/>"
								. "    </td>"
								. "    <td>{$campus[$i]["nome"]}</td>"
								. "    <td style='text-align:right; color:#0066cc;'>{$campus[$i]["total"]}</td>"
								. "</tr>"
								. "<tr id='trFilhoObra{$campus[$i]["id"]}' style='display:none;'>"
								. "    <td style='text-align:center;'>"
								. "        <img src='../imagens/seta_filho.gif'/>"
								. "    </td>"
								. "    <td colspan='2'><div id='divFilhoObra{$campus[$i]["id"]}'></div></td>"
								. "</tr>";
						}
					}
					
				}
				
				print "</table>";
				
			}else{
//				ver($entid,$filtros);
				//$this->listaObras( $entid, $filtros );
				$this->listaObras( $entid, $filtros, '', '', '', 'simples' );
				
			}
			
		}
			
	}
	
	function listaObras( $entid, $filtros, $tipo = "normal", $limit = false, $offSet = false, $lista = null ){
		
		global $db;
		
		if( $tipo == "normal" ){
			$filtroObra = ( ( $_SESSION["obras"]["orgid"] == ORGAO_FNDE ) || !is_array( obrBuscaCampusObras($entid) ) ) ? "entidunidade = {$entid}" : "entidcampus = {$entid}";
		}else{
			$filtroObra = $_SESSION["obras"]["orgid"] ? "oi.orgid = {$_SESSION["obras"]["orgid"]}" : "(1=1)"; 
		}
		
		switch( $_SESSION["obras"]["filtros"]["ultatualizacao"] ){
			case 1:
				$filtroObra .= " AND DATE_PART('days', NOW() - obrdtvistoria) <= 45";
				break;
			case 2:
				$filtroObra .= " AND DATE_PART('days', NOW() - obrdtvistoria) BETWEEN 46 AND 60";
				break;
			case 3:
				$filtroObra .= " AND DATE_PART('days', NOW() - obrdtvistoria) > 60";
				break;
			
		}
		
		if( $_SESSION["obras"]["filtros"]["agrupamento"] == "O" ){
			$entidadesPermitidas = obras_pegarUnidadesPermitidas();
			if( count($entidadesPermitidas) > 0 && is_array( $entidadesPermitidas ) ){
				$whereEntidades = "ee.entid in (" . implode( ", ", $entidadesPermitidas ) . ") AND ";
			}
		}else{
			if( $tipo == 'abresc' ){
				$whereEntidades	= "oi.entidunidade = {$entid} AND oi.entidcampus is null AND";		
			}else{
				$whereEntidades	= "oi.entidcampus = {$entid} AND";		
			}
		}

		
		if( $_SESSION["obras"]["orgid"] == 3 || 
			$_SESSION["obras"]["orgid"] == ORGAO_REHUF || 
			$_SESSION["obras"]["orgid"] == ORGAO_ADM ){
			if( $_SESSION["obras"]["filtros"]["agrupamento"] == "O" ){
				$entidadesPermitidas = obras_pegarUnidadesPermitidas();
				if( count($entidadesPermitidas) > 0 && is_array( $entidadesPermitidas ) ){
					$whereEntidades = "ee.entid in (" . implode( ", ", $entidadesPermitidas ) . ") AND ";
				}
			}else{
				$whereEntidades	= "ee.entid = {$entid} AND";		
			}
		}
		
		$arFiltroPermissao = obras_permissaoPerfil();
		
		if ( is_array( $arFiltroPermissao ) ){
			if ($arFiltroPermissao['unidade'])
				$where[] = "ee.entid IN (" . implode(', ', $arFiltroPermissao['unidade']) . ")";
			if ($arFiltroPermissao['orgao'])
				$where[] = "oi.orgid IN (" . implode(', ', $arFiltroPermissao['orgao']) . ")";
			if ($arFiltroPermissao['estado'])
				$where[] = "ed.estuf IN ('" . implode("', '", $arFiltroPermissao['estado']) . "')";
			if ($arFiltroPermissao['campus'] && $entid != 'null')
				$where[] = "oi.entidcampus IN ('" . implode("', '", $arFiltroPermissao['campus']) . "')";
			if ($arFiltroPermissao['obra'])
				$where[] = "oi.obrid IN ('" . implode("', '", $arFiltroPermissao['obra']) . "')";
			
			if (count($where) > 0){	
				if( possuiPerfil(PERFIL_SUPERVISORUNIDADE) && !$db->testa_superuser() ){
					$where = "(" . implode(" AND ", $where) . ") AND ";
				}else{
					$where = "(" . implode(" OR ", $where) . ") AND ";
				}
			}else{
				$where = "(1=2) AND ";
			}
		}		
		
//		dbg($whereEntidades,1);
		$OrgaoFnde = $_SESSION["obras"]["orgid"] == ORGAO_FNDE;
		$AcaoOrgaoFnde = "false";
		if($OrgaoFnde == true) $AcaoOrgaoFnde = "true";
		
		/* Permissï¿½o para excluir obras do Perfil "PERFIL_SUPERVISORMEC", removida dia 27/10/2010 as 10:28 H. */
//		if ( $db->testa_superuser() || possuiPerfil( PERFIL_SUPERVISORMEC) || possuiPerfil(PERFIL_ADMINISTRADOR) ) {
		if ( $db->testa_superuser() || possuiPerfil(PERFIL_ADMINISTRADOR) ) {
			
			$btnExcluir = "CASE WHEN ov.supervisao IS NOT NULL OR gpd.gpdstatus = 'A' OR pe.total_servicos > 0 THEN 
						   		'<img src=\"/imagens/exclui_p2.gif\" title=\"Esta obra não pode ser excluída!\">'	
						   ELSE
						   		'<img src=\"/imagens/exclui_p.gif\" style=\"cursor:pointer;\" onclick=\"obrIrParaCaminho(\'' || oi.obrid || '\',\'excluir\');\" title=\"Excluir Obra\">'
						   END AS acao";
			
		
		}elseif ( possuiPerfil( PERFIL_SUPERVISORUNIDADE) ){
			
			$btnExcluir = "CASE WHEN ov.supervisao IS NOT NULL OR {$AcaoOrgaoFnde} OR gpd.gpdstatus = 'A' OR pe.total_servicos > 0 THEN
								'<img src=\"/imagens/exclui_p2.gif\" title=\"Esta obra não pode ser excluída!\">'
							ELSE
							 	'<img src=\"/imagens/exclui_p.gif\" style=\"cursor:pointer;\" onclick=\"obrIrParaCaminho(\'' || oi.obrid || '\',\'excluir\');\" title=\"Excluir Obra\">'
							END as acao";
			
		}else{
			
			$btnExcluir = " '<img src=\"/imagens/exclui_p2.gif\" title=\"Esta obra não pode ser excluída\">'";
			
		}
		
		//$btAcaoExcluir = ( !empty( $existeSupervisao ) || $_SESSION["obras"]["orgid"] == ORGAO_FNDE ) ? "<img src='/imagens/exclui_p2.gif' title='Esta obra nï¿½o pode ser excluï¿½da'/>" : "<img src='/imagens/exclui_p.gif' style='cursor:pointer;' onclick='obrIrParaCaminho( {$obras[$i]["id"]}, \"excluir\" );' title='Excluir Obra'/>";
		if($_SESSION["obras"]["orgid"] == "3" || 
		   $_SESSION["obras"]["orgid"] == ORGAO_REHUF || 
		   $_SESSION["obras"]["orgid"] == ORGAO_ADM){
			$numConvenioEnsBasico = 'oi.numconvenio,';
		}
	
		if ( empty($_SESSION["obras"]["ordem"]) ) {
			$_SESSION["obras"]["ordem"] = 'nome';
		}
		
		$sql = "SELECT DISTINCT
					'<img src=\"/imagens/check_p.gif\" style=\"cursor:pointer;\" onclick=\"obrIrParaCaminho(\'' || oi.obrid || '\',\'cadastro\');\" title=\"Visualizar Obra\">&nbsp;'
					|| 
					".$btnExcluir.",
					CASE WHEN arquivo IS NOT NULL THEN
							'<img src=\"/imagens/obras/anexo.png\" onclick=\"obrIrParaCaminho(\'' || oi.obrid || '\',\'anexos\');\" style=\"cursor:pointer; width:15px;\" title=\"Anexos da Obra\">'
					END as anexo,
					CASE WHEN foto IS NOT NULL THEN
							'<img src=\"/imagens/cam_foto.gif\" onclick=\"obrIrParaCaminho(\'' || oi.obrid || '\',\'slideFotos\', \'\', \'' || arqfoto  || '\');\" style=\"cursor:pointer; width:15px;\" title=\"Fotos da Obra\">'
					END as fotos,					
					CASE WHEN restricao IS NOT NULL THEN
							'<img src=\"/imagens/obras/atencao.png\" onclick=\"obrIrParaCaminho(\'' || oi.obrid || '\',\'restricao\');\" style=\"cursor:pointer; width:15px;\" title=\"Restrições da Obra\">'
					END as restricoes,					
					CASE WHEN max(ta.traid) IS NOT NULL THEN
							'<img src=\"/imagens/obras/check.png\" onclick=\"obrIrParaCaminho(\'' || oi.obrid || '\',\'aditivo\');\" style=\"cursor:pointer; width:15px;\" title=\"Aditivos da Obra\">'
					END as aditivo,	
					
					CASE WHEN 
						(
						-- porcentagem supervisor --
						SELECT
							count(*)
						FROM
							obras.supervisaoitenscomposicao sic
						JOIN obras.itenscomposicaoobra ico ON ico.icoid = sic.icoid
															  AND ico.obrid = oi.obrid 
						WHERE
							supvid = (  SELECT 
											s.supvid 
										FROM 
											obras.supervisao s
										JOIN
											workflow.documento doc ON doc.docid = s.docid 
																	AND doc.esdid = ".WF_ESTADO_VALIDADO."
										WHERE
											s.supstatus = 'A'
											AND s.obrid = oi.obrid 
										ORDER BY 
											s.supvdt DESC LIMIT 1
									  ) --PEGA A SUPERVISAO MAIS RECENTE
						AND ico.icodterminoitem < now()
						AND sic.supvlrinfsupervisor < 100
						-- porcentagem supervisor --
						)
					> 0 THEN 
							'<center><img src=\"/imagens/atencao.png\" style=\"width:15px; cursor:pointer;\" onclick=\"obrIrParaCaminho(\'' || oi.obrid || '\',\'vistoria\');\" title=\"Esta obra está atrasada\"></center>'
					END AS aviso,
					
					oi.obrid as id,
					{$numConvenioEnsBasico}
					--CASE when (CASE WHEN of.covid is not null THEN oc.covnumero ELSE oi.numconvenio END) is not null 
					--THEN 
					--	'<a onclick=\"obrIrParaCaminho(\'' || oi.obrid || '\',\'cadastro\');\">' || upper(oi.obrdesc) || ' (' || (CASE WHEN of.covid is not null THEN oc.covnumero ELSE oi.numconvenio END) || ') </a>' 
					--ELSE 
						'<a onclick=\"obrIrParaCaminho(\'' || oi.obrid || '\',\'cadastro\');\">' || upper(oi.obrdesc) || '</a>' 
					--END 
					AS nome,
					
					ee.entnome as entdescricao,
					tm.mundescricao || ' / ' || ed.estuf as municipiouf,
					'<div style=\"display:none\">'||obrdtinicio||'</div>' || to_char(obrdtinicio, 'DD/MM/YYYY') as inicio,
					CASE WHEN max(ta.tradtinclusao) IS NOT NULL
						THEN '<div style=\"display:none\">'||max(ta.traterminoexec)||'</div>' || to_char(max(ta.traterminoexec), 'DD/MM/YYYY')
						ELSE '<div style=\"display:none\">'||obrdttermino||'</div>' || to_char(obrdttermino, 'DD/MM/YYYY') 
					END as termino,
					stodesc as situacao,
					-- pog ordena data
					CASE WHEN obrdtvistoria IS NOT NULL THEN 
							'<div style=\"display:none\">'||obrdtvistoria||'</div>'  
						 ELSE 
						 	'<div style=\"display:none\">'||obsdtinclusao||'</div>' 
					END
					|| '<FONT ' ||
					CASE WHEN oi.stoid IN (1, 2) THEN
							CASE WHEN obrdtvistoria IS NOT NULL THEN 
									CASE WHEN DATE_PART('days', NOW() - obrdtvistoria) <= 45 THEN
											'COLOR=\"#00AA00\" TITLE=\"Esta obra foi atualizada em até 45 dias\">' 
										 WHEN DATE_PART('days', NOW() - obrdtvistoria) > 45 AND DATE_PART('days', NOW() - obrdtvistoria) <= 60 THEN
											'COLOR=\"#BB9900\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">' 
										 WHEN DATE_PART('days', NOW() - obrdtvistoria) > 60 THEN
											'COLOR=\"#DD0000\" TITLE=\"Esta obra está desatualizada\">' 
									END
									|| to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )'
						 		 ELSE 
						 			CASE WHEN DATE_PART('days', NOW() - obsdtinclusao) <= 45 THEN
											'COLOR=\"#00AA00\" TITLE=\"Esta obra foi atualizada em até 45 dias\">' 
										 WHEN DATE_PART('days', NOW() - obsdtinclusao) > 45 AND DATE_PART('days', NOW() - obsdtinclusao) <= 60 THEN
											'COLOR=\"#BB9900\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">' 
										 WHEN DATE_PART('days', NOW() - obsdtinclusao) > 60 THEN
											'COLOR=\"#DD0000\" TITLE=\"Esta obra está desatualizada\">' 
									END
									|| to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obsdtinclusao)||' dia(s) )' 
							END
						 WHEN oi.stoid IN (3) THEN
						  	'COLOR=\"#0066CC\" TITLE=\"Esta obra foi concluída\">' || COALESCE(to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS'), to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS'))||'</br>( '||DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )'
						 ELSE
						 	'COLOR=\"#000000\" TITLE=\" \">' ||
							CASE WHEN obrdtvistoria IS NOT NULL THEN 
									to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obrdtvistoria)||' dia(s) )' 
						 		 ELSE 
						 			to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS')||'</br>( '||DATE_PART('days', NOW() - obsdtinclusao)||' dia(s) )' 
							END 
					END 
					|| '</FONT>' as atualizacao,
					--fim pog
					/*CASE WHEN pe.total_exec is null 
						THEN '0.00'
						ELSE
							CASE WHEN pe.total_exec > 100
								THEN '100.00'
								ELSE
									--início da pesquisa da última vistoria
									(
										SELECT  
											coalesce((SELECT 
													sum(( icopercsobreobra * supvlrinfsupervisor ) / 100)
												  FROM 
													obras.itenscomposicaoobra i
												  INNER JOIN 
													obras.supervisaoitenscomposicao si ON i.icoid = si.icoid WHERE si.supvid = MAX(s.supvid) AND obrid = oi.obrid AND i.icovigente = 'A' ),'0') as percentual
										FROM
											obras.supervisao s
										INNER JOIN 
											obras.situacaoobra si ON si.stoid = s.stoid
										LEFT JOIN
											obras.realizacaosupervisao rs ON rs.rsuid = s.rsuid 
										WHERE
											s.obrid = oi.obrid AND
											s.supstatus = 'A'
									 ) --fim da pesquisa da última vistoria
							 END
					END as executado,*/	
					
					
					(
						coalesce((
						SELECT				
						CASE WHEN vic.vicexecutado is null 
							THEN '0.00'
							ELSE
								CASE WHEN vic.vicexecutado > 100
										THEN '100.00'
									ELSE
										vic.vicexecutado
								 END
						END as vicexecutado1
						FROM 
							obras.vistoriacaixa vic
						WHERE
							vic.obrid = oi.obrid
							AND vic.vicstatus = 'A'
						ORDER BY 
							vicdata DESC LIMIT 1
					 ),'0.00') ) AS vicexecutado,

					 
					 
					oi.obrpercexec as executado,
					/*oi.obrcustocontrato as vlr*/
					oi.obrvalorprevisto as vlr
				FROM 
					obras.obrainfraestrutura oi
				LEFT JOIN
					obras.vistoriacaixa vic ON vic.obrid = oi.obrid
				LEFT JOIN
					obras.termoaditivo ta ON ta.obrid = oi.obrid
				left JOIN
					entidade.entidade ee ON ee.entid = oi.entidunidade
				
				LEFT JOIN
				    obras.formarepasserecursos of ON of.obrid = oi.obrid
				LEFT JOIN
				    obras.conveniosobra  oc ON oc.covid = of.covid	
				
				INNER JOIN
					entidade.endereco ed ON ed.endid = oi.endid
				LEFT JOIN
					territorios.municipio tm ON tm.muncod = ed.muncod
				LEFT JOIN
					obras.situacaoobra so ON so.stoid = oi.stoid
				LEFT JOIN
					(SELECT 
						SUM(icopercexecutado) as total_exec,
						COUNT(itcid) AS total_servicos, 
						obrid 
					FROM 
						obras.itenscomposicaoobra itco
					WHERE 
						icostatus = 'A'
						AND icovigente = 'A' 
					GROUP BY obrid ) pe ON pe.obrid = oi.obrid		
				LEFT JOIN
					( SELECT max(aqoid) as arquivo, obrid FROM obras.arquivosobra WHERE aqostatus = 'A' and tpaid <> 21 GROUP BY obrid ) ao ON ao.obrid = oi.obrid
				LEFT JOIN
					( 
					 SELECT
					 	max(aqoid) as foto,
                        max(arq.arqid) as  arqfoto,
                        oar.obrid
                     FROM
                     	obras.arquivosobra oar
                     INNER JOIN
                     	public.arquivo arq ON arq.arqid = oar.arqid
                     WHERE
                     	aqostatus = 'A'
                        and tpaid = 21
                        and (arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png')
                     GROUP BY oar.obrid
					) af ON af.obrid = oi.obrid
				LEFT JOIN
					( SELECT max(rstoid) as restricao, obrid FROM obras.restricaoobra WHERE rststatus = 'A' GROUP BY obrid ) re ON re.obrid = oi.obrid
				LEFT JOIN
					( SELECT supvid as supervisao, obrid FROM obras.supervisao WHERE supstatus = 'A' ORDER BY supvdt DESC LIMIT 1 ) ov ON ov.obrid = oi.obrid
				LEFT JOIN
					obras.repositorio rep on rep.obrid = oi.obrid AND rep.repstatus = 'A'
				LEFT JOIN 
					obras.itemgrupo itg ON itg.repid = rep.repid 	
				LEFT JOIN
					obras.grupodistribuicao gpd ON gpd.gpdid = itg.gpdid AND gpd.gpdstatus = 'A'
				WHERE
					{$whereEntidades}
					{$where}
					{$filtroObra} AND
					oi.obsstatus = 'A' 
					{$filtros}
				GROUP BY
					ao.arquivo,
					af.foto,af.arqfoto,
					pe.total_exec,
					re.restricao,
					oi.obridaditivo,oi.obrid,oi.obrdesc,oi.obrdtinicio,oi.obrdttermino,oi.obrdtvistoria,oi.obsdtinclusao,oi.stoid,
					ee.entnome,tm.mundescricao,ed.estuf,
					so.stodesc,
					oi.numconvenio,
					oi.obrvalorprevisto,
					ov.supervisao,
					oi.obrcustocontrato,
					oi.obrvalorprevisto,
					oi.obrpercexec,
					gpd.gpdstatus,
					pe.total_servicos
				ORDER BY
					{$_SESSION["obras"]["ordem"]}";

		$c1  = $_SESSION["obras"]["ordem"] == "anexo" 		   && $_SESSION["obras"]["lista"] != "lista" ? "<img height='13' align='middle' width='11' src='../imagens/seta_ordemASC.gif'>" : "" ;
		$c2  = $_SESSION["obras"]["ordem"] == "foto"  		   && $_SESSION["obras"]["lista"] != "lista" ? "<img height='13' align='middle' width='11' src='../imagens/seta_ordemASC.gif'>" : "" ;
		$c3  = $_SESSION["obras"]["ordem"] == "restricoes" 	   && $_SESSION["obras"]["lista"] != "lista" ? "<img height='13' align='middle' width='11' src='../imagens/seta_ordemASC.gif'>" : "" ;
		$c4  = $_SESSION["obras"]["ordem"] == "pi" 			   && $_SESSION["obras"]["lista"] != "lista" ? "<img height='13' align='middle' width='11' src='../imagens/seta_ordemASC.gif'>" : "" ;
		$c5  = $_SESSION["obras"]["ordem"] == "aditivo" 	   && $_SESSION["obras"]["lista"] != "lista" ? "<img height='13' align='middle' width='11' src='../imagens/seta_ordemASC.gif'>" : "" ;
		$c6  = $_SESSION["obras"]["ordem"] == "id" 			   && $_SESSION["obras"]["lista"] != "lista" ? "<img height='13' align='middle' width='11' src='../imagens/seta_ordemASC.gif'>" : "" ;
		$c7  = $_SESSION["obras"]["ordem"] == "oi.numconvenio" && $_SESSION["obras"]["lista"] != "lista" ? "<img height='13' align='middle' width='11' src='../imagens/seta_ordemASC.gif'>" : "" ;
		$c8  = $_SESSION["obras"]["ordem"] == "nome" 		   && $_SESSION["obras"]["lista"] != "lista" ? "<img height='13' align='middle' width='11' src='../imagens/seta_ordemASC.gif'>" : "" ;
		$c9  = $_SESSION["obras"]["ordem"] == "entdescricao"   && $_SESSION["obras"]["lista"] != "lista" ? "<img height='13' align='middle' width='11' src='../imagens/seta_ordemASC.gif'>" : "" ;
		$c10 = $_SESSION["obras"]["ordem"] == "municipiouf"    && $_SESSION["obras"]["lista"] != "lista" ? "<img height='13' align='middle' width='11' src='../imagens/seta_ordemASC.gif'>" : "" ;
		$c11 = $_SESSION["obras"]["ordem"] == "inicio" 	  	   && $_SESSION["obras"]["lista"] != "lista" ? "<img height='13' align='middle' width='11' src='../imagens/seta_ordemASC.gif'>" : "" ;
		$c12 = $_SESSION["obras"]["ordem"] == "termino" 	   && $_SESSION["obras"]["lista"] != "lista" ? "<img height='13' align='middle' width='11' src='../imagens/seta_ordemASC.gif'>" : "" ;
		$c13 = $_SESSION["obras"]["ordem"] == "situacao" 	   && $_SESSION["obras"]["lista"] != "lista" ? "<img height='13' align='middle' width='11' src='../imagens/seta_ordemASC.gif'>" : "" ;
		$c14 = $_SESSION["obras"]["ordem"] == "atualizacao"    && $_SESSION["obras"]["lista"] != "lista" ? "<img height='13' align='middle' width='11' src='../imagens/seta_ordemASC.gif'>" : "" ;
		$c15 = $_SESSION["obras"]["ordem"] == "executado" 	   && $_SESSION["obras"]["lista"] != "lista" ? "<img height='13' align='middle' width='11' src='../imagens/seta_ordemASC.gif'>" : "" ;
		$c16 = $_SESSION["obras"]["ordem"] == "valorobra" 	   && $_SESSION["obras"]["lista"] != "lista" ? "<img height='13' align='middle' width='11' src='../imagens/seta_ordemASC.gif'>" : "" ;
		
		$_SESSION["obras"]["lista"] = "";
		
		if($_SESSION["obras"]["orgid"] == "1" || $_SESSION["obras"]["orgid"] == "2" ){
			$cabecalho = array("<div title='Ação' style='text-align:center'>Ação</div>",
							   "<div onclick='listaDeObraOrdem(1);' title='Anexo' style='text-align:center'>{$c1}A</div>",
							   "<div onclick='listaDeObraOrdem(2);' title='Foto' style='text-align:center'>{$c2} F</div>",
							   "<div onclick='listaDeObraOrdem(3);' title='Restrições' style='text-align:center'>{$c3} R</div>",
							   "<div onclick='listaDeObraOrdem(5);' title='Aditivos' style='text-align:center'>{$c5} AD</div>",
							   "Atraso",
							   "<div onclick='listaDeObraOrdem(6);'> {$c6} ID</div>",
							   "<div onclick='listaDeObraOrdem(8);'> {$c8} Nome da Obra</div>",
							   "<div onclick='listaDeObraOrdem(9);'> {$c9} Unidade Responsável pela Obra</div>",
							   "<div onclick='listaDeObraOrdem(10);'>{$c10} Município/UF</div>",
							   "<div onclick='listaDeObraOrdem(11);'>{$c11} Data de Início</div>",
							   "<div onclick='listaDeObraOrdem(12);'>{$c12} Data de Término</div>",
							   "<div onclick='listaDeObraOrdem(13);'>{$c13} Situação da Obra</div>",
							   "<div onclick='listaDeObraOrdem(14);'>{$c14} Última Atualização</div>",
							   " % Caixa",
							   "<div onclick='listaDeObraOrdem(15);'>{$c15} % Executado</div>",
							   "<div onclick='listaDeObraOrdem(16);'>{$c16} Valor da Obra</div>");
		}else{
			$cabecalho = array("<div title='Ação' style='text-align:center'>Ação</div>",
							   "<div onclick='listaDeObraOrdem(1);' title='Anexo' style='text-align:center'>{$c1} A</div>",
							   "<div onclick='listaDeObraOrdem(2);' title='Foto' style='text-align:center'>{$c2} F</div>",
							   "<div onclick='listaDeObraOrdem(3);' title='Restrições' style='text-align:center'>{$c3} R</div>",
							   "<div onclick='listaDeObraOrdem(5);' title='Aditivos' style='text-align:center'>{$c5} AD</div>",
							   "<div onclick='listaDeObraOrdem(6);' >{$c6} ID</div>",
							   "<div onclick='listaDeObraOrdem(7);' >{$c7} Convênio</div>",
							   "<div onclick='listaDeObraOrdem(8);' >{$c8} Nome da Obra</div>",
							   "<div onclick='listaDeObraOrdem(9);' >{$c9} Unidade Responsável pela Obra</div>",
							   "<div onclick='listaDeObraOrdem(10);'>{$c10} Município/UF</div>",
							   "<div onclick='listaDeObraOrdem(11);'>{$c11} Data de Início</div>",
							   "<div onclick='listaDeObraOrdem(12);'>{$c12} Data de Término</div>",
							   "<div onclick='listaDeObraOrdem(13);'>{$c13} Situação da Obra</div>",
							   "<div onclick='listaDeObraOrdem(14);'>{$c14} Última Atualização</div>",
							   "<div onclick='listaDeObraOrdem(15);'>{$c15} % Executado</div>",
							   "<div onclick='listaDeObraOrdem(16);'>{$c16} Valor da Obra</div>");
		}

		
		if($lista == 'simples')$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '', '' );	
		else $db->monta_lista_ordenaGROUPBY( $sql, $cabecalho, 50, 10, 'N', '', '' );
				
	}
	
}


// ---------- Fim Nova Tela inicial ---------- 

Class DadosObra{

	// Declaraï¿½ï¿½o dos campos da tabela obrainfraestrutura

	public $obrid 						= null;
	public $tobraid 					= null;
	public $tpcoid 						= null;
	public $recoid 						= null;
	public $orgid 						= null;
	public $mdaid 						= null;
	public $tpaid 						= null;
	public $felid 						= null;
	public $endid 						= null;
	public $entidunidade 				= null;
	public $stoid 						= null;
	public $umdidobraconstruida 		= null;
	public $umdidareaserconstruida  	= null;
	public $umdidareaserreformada   	= null;
	public $umdidareaserampliada    	= null;
	public $entidempresaconstrutora 	= null;
	public $obrdesc 					= null;
	public $obrdescundimplantada 		= null;
	public $obrdtinicio 				= null;
	public $obrdttermino 				= null;
	public $obrpercexec 				= null;
	public $obrpercbdi 					= null;
	public $obrcustocontrato 			= null;
	public $obrqtdconstruida 			= null;
	public $obrcustounitqtdconstruida   = null;
	public $obrinfexistedimovel 		= null;
	public $obrreaconstruida 			= null;
	public $obrdescsumariaedificacao	= null;
	public $obredificacaoreforma 		= null;
	public $obrqtdareapreforma 			= null;
	public $obrampliacao 				= null;
	public $obrqtdaraampliada 			= null;
	public $obrvlraraampliada 			= null;
	public $obsobra 					= null;
	public $obsstatus 					= null;
	public $obsdtinclusao 				= null;
	public $entidcampus					= null;
	public $tpoid 						= null;
	public $cloid 						= null;
	public $prfid 						= null;
	public $fntid 						= null;
	public $obrdescfontefin 			= null;
	public $obrcomposicao	 			= null;
	public $obrstatusinauguracao   	    = null;
	public $obrdtinauguracao			= null;
	public $obrdtprevinauguracao 		= null;
	public $obrlincambiental			= null;
	public $obraprovpatrhist			= null;
	public $obrdtprevprojetos			= null;
	public $sbaid 						= null;
	public $obrvalorprevisto			= null;
	public $obrdtassinaturacontrato		= null;
	public $obrdtordemservico			= null;
	public $dtterminocontrato			= null;
	public $obrprazoexec				= null;
	public $obrprazovigencia			= null;
	public $dtiniciocontrato			= null;
	public $terid						= null;
	public $povid						= null;
	
	
	// Declaraï¿½ï¿½o dos campos da tabela faselicitacao

	public $flcid 					= null;
	public $tflid 					= null;
	public $flcpubleditaldtprev 	= null;
	public $flcaberpropdtprev 		= null;
	public $flcrecintermotivo 		= null;
	public $flcdtrecintermotivo 	= null;
	public $flchomlicdtprev 		= null;
	public $flcordservdt 			= null;
	public $flcordservnum 			= null;
	public $flcstatus 				= null;
	public $flcdtinclusao 			= null;

	// Declaraï¿½ï¿½o dos campos da tabela formarepasserecursos

	public $frrid 					= null;
	public $frpid 					= null;
	public $frrconventbenef 		= null;
	public $frrconvnum 				= null;
	public $frrconvobjeto 			= null;
	public $frrconvvlr 				= null;
	public $frrconvvlrconcedente 	= null;
	public $frrconvvltconcenente 	= null;
	public $frrdescinstituicao 		= null;
	public $frrdescnumport 			= null;
	public $frrdescobjeto 			= null;
	public $frrdescvlr 				= null;
	public $frrdescdtviginicio 		= null;
	public $frrdescdtvigfinal 		= null;
	public $frrobs 					= null;
	public $frrstatus 				= null;
	public $frrdtinclusao 			= null;
	public $frrobsrecproprio 		= null;
	public $covid		 	 		= null;

	// Declaraï¿½ï¿½o dos campos da tabela itenscomposicaoobra

	public $icovlritem 			= null;
	public $icodtinicioitem 	= null;
	public $icodterminoitem 	= null;
	public $icopercprojperiodo 	= null;
	public $icopercexecutado 	= null;

	// Declaraï¿½ï¿½o dos campos da tabela supervisao

	public $supvdt 					= null;
	public $supvistoriador			= null;
	public $supprojespecificacoes   = null;
	public $supplacaobra 			= null;
	public $supplacalocalterreno 	= null;
	public $supvalidadealvara 		= null;
	public $supobs 					= null;
	public $supvlrinfsupervisor 	= null;
	public $supparecerorgao 		= null;
	public $supdiarioobra 			= null;
	public $tpsid		 			= null;

	// Campos tabela endereco

	public $endlog 			= null;
	public $endnum			= null;
	public $endcom			= null;
	public $endbai			= null;
	public $estuf			= null;
	public $endcep 			= null;
	public $medlatitude  	= null;
	public $medlongitude 	= null;
	public $endzoom 		= null;
	public $mundescricao 	= null;
	public $endcomunidade 	= null;

	// historico
	public $tplid 	= null;
	public $hprobs 	= null;
	
	public $flcdata  = null;
	public $qlbid 	 = null;
	public $dcnid 	 = null;
	public $numConvenio = null;

	function __construct($dados){

		// Tabela obrainfraestrutura
		$this->setObrId($dados["obrid"]);
		$this->setTobraId($dados["tobraid"]);
		$this->setTpcoId($dados["tpcoid"]);
		$this->setRecoId($dados["recoid"]);
		$this->setOrgId($dados["orgid"]);
		$this->setMdaId($dados["mdaid"]);
		$this->setTpaId($dados["tpaid"]);
		$this->setFelId($dados["felid"]);
		$this->setEndId($dados["endid"]);
		$this->setEntidUnidade($dados["entidunidade"]);
		$this->setStoId($dados["stoid"]);
		$this->setUmdIdObraConstruida($dados["umdidobraconstruida"]);
		$this->setUmdIdAreaSerConstruida($dados["umdidareaserconstruida"]);
		$this->setUmdIdAreaSerReformada($dados["umdidareaserreformada"]);
		$this->setUmdIdAreaSerAmpliada($dados["umdidareaserampliada"]);
		$this->setEntidEmpresaConstrutora($dados["entidempresaconstrutora"]);
		$this->setObrDesc($dados["obrdesc"]);
		$this->setObrDescundImplantada($dados["obrdescundimplantada"]);
		$this->setObrDtInicio($dados["obrdtinicio"]);
		$this->setObrDtTermino($dados["obrdttermino"]);
		$this->setObrPercExec($dados["obrpercexec"]);
		$this->setObrPercBdi($dados["obrpercbdi"]);
		$this->setObrCustoContrato($dados["obrcustocontrato"]);
		$this->setObrQtdConstruida($dados["obrqtdconstruida"]);
		$this->setObrCustoUnitQtdConstruida($dados["obrcustounitqtdconstruida"]);
		$this->setObrInfExistedImovel($dados["obrinfexistedimovel"]);
		$this->setObrReaConstruida($dados["obrreaconstruida"]);
		$this->setObrDescSumariaEdificacao($dados["obrdescsumariaedificacao"]);
		$this->setObrEdificacaoReforma($dados["obredificacaoreforma"]);
		$this->setObrQtdAreaPreforma($dados["obrqtdareapreforma"]);
		$this->setObrAmpliacao($dados["obrampliacao"]);
		$this->setObrQtdAraAmpliada($dados["obrqtdaraampliada"]);
		$this->setObrVlrAraAmpliada($dados["obrvlraraampliada"]);
		$this->setObsObra($dados["obsobra"]);
		$this->setObsStatus($dados["obsstatus"]);
		$this->setObsDtInclusao($dados["obsdtinclusao"]);
		$this->setEntidCampus($dados["entidcampus"]);
		$this->setTpoId($dados["tpoid"]);
		$this->setCloId($dados["cloid"]);
		$this->setPrfId($dados["prfid"]);
		$this->setFntId($dados["fntid"]);
		$this->setObrDescFonteFin($dados["obrdescfontefin"]);
		$this->setObrComposicao($dados["obrcomposicao"]);
		$this->setObrStatusInauguracao($dados["obrstatusinauguracao"]);
		$this->setObrDtInauguracao($dados["obrdtinauguracao"]);
		$this->setObrDtPrevInauguracao($dados["obrdtprevinauguracao"]);
		$this->setObrLincAmbiental($dados["obrlincambiental"]);
		$this->setObrAprovPatrHist($dados["obraprovpatrhist"]);
		$this->setObrDtPrevProjetos($dados["obrdtprevprojetos"]);
		$this->setSbaid($dados["sbaid"]);
		$this->setObrValorPrevisto( $dados["obrvalorprevisto"] );
		$this->setObrDtAssinaturaContrato($dados["obrdtassinaturacontrato"]);
		$this->setObrDtOrdemServico($dados["obrdtordemservico"]);
		$this->setDtTerminoContrato($dados["dtterminocontrato"]);
		$this->setObrPrazoExec($dados["obrprazoexec"]);
		$this->setObrPrazoVigencia($dados["obrprazovigencia"]);
		$this->setDtInicioContrato($dados["dtiniciocontrato"]);
		$this->setTerid($dados["terid"]);
		$this->setPovid($dados["povid"]);
		$this->numConvenio = $dados["numconvenio"];

		// Tabela faselicitacao

		$this->setFlcId($dados["flcid"]);
		$this->setTflId($dados["tflid"]);
		$this->setFlcPublEditalDtPrev($dados["flcpubleditaldtprev"]);
		$this->setFlcAberPropDtPrev($dados["flcaberpropdtprev"]);
		$this->setFlcRecInterMotivo($dados["flcrecintermotivo"]);
		$this->setFlcDtRecInterMotivo($dados["flcdtrecintermotivo"]);
		$this->setFlcHomlicDtPrev($dados["flchomlicdtprev"]);
		$this->setFlcOrdServDt($dados["flcordservdt"]);
		$this->setFlcOrdServNum($dados["flcordservnum"]);
		$this->setFlcStatus($dados["flcstatus"]);
		$this->setFlcDtInclusao($dados["flcdtinclusao"]);

		// Tabela formarepasserecursos

		$this->setFrrId($dados["frrid"]);
		$this->setFrpId($dados["frpid"]);
		$this->setFrrConventBenef($dados["frrconventbenef"]);
		$this->setFrrConvNum($dados["frrconvnum"]);
		$this->setFrrConvObjeto($dados["frrconvobjeto"]);
		$this->setFrrConvVlr($dados["frrconvvlr"]);
		$this->setFrrConvVlrConcedente($dados["frrconvvlrconcedente"]);
		$this->setFrrConvVlrConcenente($dados["frrconvvlrconcenente"]);
		$this->setFrrDescInstituicao($dados["frrdescinstituicao"]);
		$this->setFrrDescNumPort($dados["frrdescnumport"]);
		$this->setFrrDescObjeto($dados["frrdescobjeto"]);
		$this->setFrrDescVlr($dados["frrdescvlr"]);
		$this->setFrrDescDtVigInicio($dados["frrdescdtviginicio"]);
		$this->setFrrDescDtVigFinal($dados["frrdescdtvigfinal"]);
		$this->setFrrObs($dados["frrobs"]);
		$this->setFrrStatus($dados["frrstatus"]);
		$this->setFrrDtInclusao($dados["frrdtinclusao"]);
		$this->setFrrObsRecProprio($dados["frrobsrecproprio"]);
		$this->setCovId($dados["covid"]);

		// Tabela itenscomposicaoobra

		$this->setIcoVlrItem($dados["icovlritem"]);
		$this->setIcoDtInicioItem($dados["icodtinicioitem"]);
		$this->setIcoDterminoItem($dados["icodterminoitem"]);
		$this->setIcoPercProjPeriodo($dados["icopercprojperiodo"]);
		$this->setIcoPercExecutado($dados["icopercexecutado"]);

		// Tabela supervisao

		$this->setSupvDt($dados["supvdt"]);
		$this->setSupVistoriador($dados["supvistoriador"]);
		$this->setSupProjEspecificacoes($dados["supprojespecificacoes"]);
		$this->setSupPlacaObra($dados["supplacaobra"]);
		$this->setSupPlacaLocalTerreno($dados["supplacalocalterreno"]);
		$this->setSupValidadeAlvara($dados["supvalidadealvara"]);
		$this->setSupObs($dados["supobs"]);
		$this->setSupVlrInfSupervisor($dados["supvlrinfsupervisor"]);
		$this->setSupParecerOrgao($dados["supparecerorgao"]);
		$this->setSupDiarioObra($dados["supdiarioobra"]);
		$this->setTplId($dados["tplid"]);
		$this->setHprObs($dados["hprobs"]);
		$this->setTpsid($dados["tpsid"]);

		// Tabela endereco

		$this->setEndLog($dados["endlog"]);
		$this->setEndNum($dados["endnum"]);
		$this->setEndCom($dados["endcom"]);
		$this->setEndBai($dados["endbai"]);
		$this->setEstUf($dados["estuf"]);
		$this->setEndCep($dados["endcep"]);
		$this->setMedLatitude($dados["medlatitude"]);
		$this->setMedLongitude($dados["medlongitude"]);
		$this->setEndZoom($dados["endzoom"]);
		$this->setMunDescricao($dados["mundescricao"]);
		$this->setEndcomunidade($dados["endcomunidade"]);

		$this->setFlcData($dados["flcdata"]);
		$this->setQlbId($dados["qlbid"]);
		$this->setDcnId($dados["dcnid"]);

	}

	// Funï¿½ï¿½es SET tabela obrainfraestrutura
	public function setObrId($vlr){
		$this->obrid = $vlr;
	}
	public function setTobraId($vlr){
		$this->tobraid = $vlr;
	}
	public function setTpcoid($vlr){
		$this->tpcoid = $vlr;
	}
	public function setRecoId($vlr){
		$this->recoid = $vlr;
	}
	public function setOrgId($vlr){
		$this->orgid = $vlr;
	}
	public function setMdaId($vlr){
		$this->mdaid = $vlr;
	}
	public function setTpaId($vlr){
		$this->tpaid = $vlr;
	}
	public function setFelId($vlr){
		$this->felid = $vlr;
	}
	public function setEndId($vlr){
		$this->endid = $vlr;
	}
	public function setEntIdUnidade($vlr){
		$this->entidunidade = $vlr;
	}
	public function setStoId($vlr){
		$this->stoid = $vlr;
	}
	public function setUmdIdObraConstruida($vlr){
		$this->umdidobraconstruida = $vlr;
	}
	public function setUmdIdAreaSerConstruida($vlr){
		$this->umdidareaserconstruida = $vlr;
	}
	public function setUmdIdAreaSerReformada($vlr){
		$this->umdidareaserreformada = $vlr;
	}
	public function setUmdIdAreaSerAmpliada($vlr){
		$this->umdidareaserampliada = $vlr;
	}
	public function setEntIdEmpresaConstrutora($vlr){
		$this->entidempresaconstrutora = $vlr;
	}
	public function setObrDesc($vlr){
		$this->obrdesc = $vlr;
	}
	public function setObrDescUndImplantada($vlr){
		$this->obrdescundimplantada = $vlr;
	}
	public function setObrDtInicio($vlr){
		$this->obrdtinicio = $vlr;
	}
	public function setObrDtTermino($vlr){
		$this->obrdttermino = $vlr;
	}
	public function setObrPercExec($vlr){
		$this->obrpercexec = $vlr;
	}
	public function setObrPercBdi($vlr){
		$this->obrpercbdi = $vlr;
	}
	public function setObrCustoContrato($vlr){
		$this->obrcustocontrato = $vlr;
	}
	public function setObrQtdConstruida($vlr){
		$this->obrqtdconstruida = $vlr;
	}
	public function setObrCustoUnitQtdConstruida($vlr){
		$this->obrcustounitqtdconstruida = $vlr;
	}
	public function setObrInfExistedImovel($vlr){
		$this->obrinfexistedimovel = $vlr;
	}
	public function setObrReaConstruida($vlr){
		$this->obrreaconstruida = $vlr;
	}
	public function setObrDescSumariaEdificacao($vlr){
		$this->obrdescsumariaedificacao = $vlr;
	}
	public function setObrEdificacaoReforma($vlr){
		$this->obredificacaoreforma = $vlr;
	}
	public function setObrQtdAreaPreforma($vlr){
		$this->obrqtdareapreforma = $vlr;
	}
	public function setObrAmpliacao($vlr){
		$this->obrampliacao = $vlr;
	}
	public function setObrQtdAraAmpliada($vlr){
		$this->obrqtdaraampliada = $vlr;
	}
	public function setObrVlrAraAmpliada($vlr){
		$this->obrvlraraampliada = $vlr;
	}
	public function setObsObra($vlr){
		$this->obsobra = $vlr;
	}
	public function setObsStatus($vlr){
		$this->obsstatus = $vlr;
	}
	public function setObsDtInclusao($vlr){
		$this->obsdtinclusao = $vlr;
	}
	public function setEntIdCampus($vlr){
		$this->entidcampus = $vlr;
	}
	public function setTpoId($vlr){
		$this->tpoid = $vlr;
	}
	public function setCloId($vlr){
		$this->cloid = $vlr;
	}
	public function setPrfId($vlr){
		$this->prfid = $vlr;
	}
	public function setFntId($vlr){
		$this->fntid = $vlr;
	}
	public function setObrDescFonteFin($vlr){
		$this->obrdescfontefin = $vlr;
	}
	public function setObrComposicao($vlr){
		$this->obrcomposicao = $vlr;
	}
	public function setObrStatusInauguracao($vlr){
		$this->obrstatusinauguracao = $vlr;
	}
	public function setObrDtInauguracao($vlr){
		$this->obrdtinauguracao = $vlr;
	}
	public function setObrDtPrevInauguracao($vlr){
		$this->obrdtprevinauguracao = $vlr;
	}
	public function setSbaid($vlr){
		$this->sbaid = $vlr;
	}
	public function setObrLincAmbiental($vlr){
		$this->obrlincambiental = $vlr;
	}
	public function setObrAprovPatrHist($vlr){
		$this->obraprovpatrhist = $vlr;
	}
	public function setObrDtPrevProjetos($vlr){
		$this->obrdtprevprojetos = $vlr;
	}
	public function setObrDtAssinaturaContrato($vlr){
		$this->obrdtassinaturacontrato = $vlr;
	}
	public function setObrDtOrdemServico($vlr){
		$this->obrdtordemservico = $vlr;
	}
	public function setDtTerminoContrato($vlr){
		$this->dtterminocontrato = $vlr;
	}
	public function setObrPrazoExec($vlr){
		$this->obrprazoexec = $vlr;
	}
	public function setObrPrazoVigencia($vlr){
		$this->obrprazovigencia = $vlr;
	}
	public function setDtInicioContrato($vlr){
		$this->dtiniciocontrato = $vlr;
	}
	public function setTerid($vlr){
		$this->terid = $vlr;
	}
	public function setPovid($vlr){
		$this->povid = $vlr;
	}

	// Funï¿½ï¿½es SET tabela faselicitacao

	public function setFlcId($vlr){
		$this->flcid = $vlr;
	}
	public function setTflId($vlr){
		$this->tflid = $vlr;
	}
	public function setFlcPublEditalDtPrev($vlr){
		$this->flcpubleditaldtprev = $vlr;
	}
	public function setFlcAberPropDtPrev($vlr){
		$this->flcaberpropdtprev = $vlr;
	}
	public function setFlcRecInterMotivo($vlr){
		$this->flcrecintermotivo = $vlr;
	}
	public function setFlcDtRecInterMotivo($vlr){
		$this->flcrecintermotivo = $vlr;
	}
	public function setFlcHomlicDtPrev($vlr){
		$this->flchomlicdtprev = $vlr;
	}
	public function setFlcOrdServDt($vlr){
		$this->flcordservdt = $vlr;
	}
	public function setFlcOrdServNum($vlr){
		$this->flcordservnum = $vlr;
	}
	public function setFlcStatus($vlr){
		$this->flcstatus = $vlr;
	}
	public function setFlcDtInclusao($vlr){
		$this->flcdtinclusao = $vlr;
	}

	// Funï¿½ï¿½es SET tabela formarepasserecursos

	public function setFrrId($vlr){
		$this->frrid = $vlr;
	}
	public function setFrpId($vlr){
		$this->frpid = $vlr;
	}
	public function setFrrConventBenef($vlr){
		$this->frrconventbenef = $vlr;
	}
	public function setFrrConvNum($vlr){
		$this->frrconvnum = $vlr;
	}
	public function setFrrConvObjeto($vlr){
		$this->frrconvobjeto = $vlr;
	}
	public function setFrrConvVlr($vlr){
		$this->frrconvvlr = $vlr;
	}
	public function setFrrConvVlrConcedente($vlr){
		$this->frrconvvlrconcedente = $vlr;
	}
	public function setFrrConvVlrConcenente($vlr){
		$this->frrconvvlrconcenente = $vlr;
	}
	public function setFrrDescInstituicao($vlr){
		$this->frrdescinstituicao = $vlr;
	}
	public function setFrrDescNumPort($vlr){
		$this->frrdescnumport = $vlr;
	}
	public function setFrrDescObjeto($vlr){
		$this->frrdescobjeto = $vlr;
	}
	public function setFrrDescVlr($vlr){
		$this->frrdescvlr = $vlr;
	}
	public function setFrrDescDtVigInicio($vlr){
		$this->frrdescdtviginicio = $vlr;
	}
	public function setFrrDescDtVigFinal($vlr){
		$this->frrdescdtvigfinal = $vlr;
	}
	public function setFrrObs($vlr){
		$this->frrobs = $vlr;
	}
	public function setFrrStatus($vlr){
		$this->frrstatus = $vlr;
	}
	public function setFrrDtInclusao($vlr){
		$this->frrdtinclusao = $vlr;
	}
	public function setFrrObsRecProprio($vlr){
		$this->frrobsrecproprio = $vlr;
	}
	public function setCovId($vlr){
		$this->covid = $vlr;
	}
	public function setObrValorPrevisto($vlr){
		$this->obrvalorprevisto = $vlr;
	}


	// Funï¿½ï¿½es SET tabela itenscomposicaoobra

	public function setIcoVlrItem($vlr){
		$this->icovlritem = $vlr;
	}
	public function setIcoDtInicioItem($vlr){
		$this->icodtinicioitem = $vlr;
	}
	public function setIcoDterminoItem($vlr){
		$this->icodterminoitem = $vlr;
	}
	public function setIcoPercProjPeriodo($vlr){
		$this->icopercprojperiodo = $vlr;
	}
	public function setIcoPercExecutado($vlr){
		$this->icopercexecutado = $vlr;
	}

	// Funï¿½ï¿½es SET tabela supervisao

	public function setSupvDt($vlr){
		$this->supvdt = $vlr;
	}
	public function setSupVistoriador($vlr){
		$this->supvistoriador = $vlr;
	}
	public function setSupProjEspecificacoes($vlr){
		$this->supprojespecificacoes = $vlr;
	}
	public function setSupPlacaObra($vlr){
		$this->supplacaobra = $vlr;
	}
	public function setSupPlacaLocalTerreno($vlr){
		$this->supplacalocalterreno = $vlr;
	}
	public function setSupValidadeAlvara($vlr){
		$this->supvalidadealvara = $vlr;
	}
	public function setSupObs($vlr){
		$this->supobs = $vlr;
	}
	public function setSupVlrInfSupervisor($vlr){
		$this->supvlrinfsupervisor = $vlr;
	}
	public function setSupParecerOrgao($vlr){
		$this->supparecerorgao = $vlr;
	}
	public function setSupDiarioObra($vlr){
		$this->supdiarioobra = $vlr;
	}
	public function setFlcData($vlr){
		$this->flcdata = $vlr;
	}
	public function setQlbId($vlr){
		$this->qlbid = $vlr;
	}
	public function setDcnId($vlr){
		$this->dcnid = $vlr;
	}
	public function setTplId($vlr){
		$this->tplid = $vlr;
	}
	public function setHprObs($vlr){
		$this->hprobs = $vlr;
	}
	public function setTpsid($vlr){
		$this->tpsid = $vlr;
	}

	// Funï¿½ï¿½es SET tabela endereco

	public function setEndLog($vlr){
		$this->endlog = $vlr;
	}
	public function setEndNum($vlr){
		$this->endnum = $vlr;
	}
	public function setEndCom($vlr){
		$this->endcom = $vlr;
	}
	public function setEndBai($vlr){
		$this->endbai = $vlr;
	}
	public function setEstUf($vlr){
		$this->estuf = $vlr;
	}
	public function setEndCep($vlr){
		$this->endcep = $vlr;
	}
	public function setMedLatitude($vlr){
		$this->medlatitude = $vlr;
	}

	public function setMedLongitude($vlr){
		$this->medlongitude = $vlr;
	}
	public function setEndZoom($vlr){
		$this->endzoom = $vlr;
	}
	public function setMunDescricao($vlr){
		$this->mundescricao = $vlr;
	}
	public function setEndcomunidade($vlr){ $this->endcomunidade = $vlr; }

	// Funï¿½ï¿½es GET tabela obrainfraestrutura

	public function getObrId(){
		return $this->obrid;
	}
	public function getTobraId(){
		return $this->tobraid;
	}
	public function getTpcoid(){
		return $this->tpcoid;
	}
	public function getRecoId(){
		return $this->recoid;
	}
	public function getOrgId(){
		return $this->orgid;
	}
	public function getMdaId(){
		return $this->mdaid;
	}
	public function getTpaId(){
		return $this->tpaid;
	}
	public function getFelId(){
		return $this->felid;
	}
	public function getEndId(){
		return $this->endid;
	}
	public function getEntIdUnidade(){
		return $this->entidunidade;
	}
	public function getStoId(){
		return $this->stoid;
	}
	public function getUmdIdObraConstruida(){
		return $this->umdidobraconstruida;
	}
	public function getUmdIdAreaSerConstruida(){
		return $this->umdidareaserconstruida;
	}
	public function getUmdIdAreaSerReformada(){
		return $this->umdidareaserreformada;
	}
	public function getUmdIdAreaSerAmpliada(){
		return $this->umdidareaserampliada;
	}
	public function getEntIdEmpresaConstrutora(){
		return $this->entidempresaconstrutora;
	}
	public function getObrDesc(){
		return $this->obrdesc;
	}
	public function getObrDescUndImplantada(){
		return $this->obrdescundimplantada;
	}
	public function getObrDtInicio(){
		return $this->obrdtinicio;
	}
	public function getObrDtTermino(){
		return $this->obrdttermino;
	}
	public function getObrPercExec(){
		return $this->obrpercexec;
	}
	public function getObrPercBdi(){
		return $this->obrpercbdi;
	}
	public function getObrCustoContrato(){
		return $this->obrcustocontrato;
	}
	public function getObrQtdConstruida(){
		return $this->obrqtdconstruida;
	}
	public function getObrCustoUnitQtdConstruida(){
		return $this->obrcustounitqtdconstruida;
	}
	public function getObrInfExistedImovel(){
		return $this->obrinfexistedimovel;
	}
	public function getObrReaConstruida(){
		return $this->obrreaconstruida;
	}
	public function getObrDescSumariaEdificacao(){
		return $this->obrdescsumariaedificacao;
	}
	public function getObrEdificacaoReforma(){
		return $this->obredificacaoreforma;
	}
	public function getObrQtdAreaPreforma(){
		return $this->obrqtdareapreforma;
	}
	public function getObrAmpliacao(){
		return $this->obrampliacao;
	}
	public function getObrQtdAraAmpliada(){
		return $this->obrqtdaraampliada;
	}
	public function getObrVlrAraAmpliada(){
		return $this->obrvlraraampliada;
	}
	public function getObsObra(){
		return $this->obsobra;
	}
	public function getObsStatus(){
		return $this->obsstatus;
	}
	public function getObsDtInclusao(){
		return $this->obsdtinclusao;
	}
	public function getEntIdCampus(){
		return $this->entidcampus;
	}
	public function getTpoId(){
		return $this->tpoid;
	}
	public function getCloId(){
		return $this->cloid;
	}
	public function getPrfId(){
		return $this->prfid;
	}
	public function getFntId(){
		return $this->fntid;
	}
	public function getObrDescFonteFin(){
		return $this->obrdescfontefin;
	}
	public function getObrComposicao(){
		return $this->obrcomposicao;
	}
	public function getObrStatusInauguracao(){
		return $this->obrstatusinauguracao;
	}
	public function getObrDtInauguracao(){
		return $this->obrdtinauguracao;
	}
	public function getObrDtPrevInauguracao(){
		return $this->obrdtprevinauguracao;
	}
	public function getSbaid(){
		return $this->sbaid;
	}
	public function getObrLincAmbiental(){
		return $this->obrlincambiental;
	}
	public function getObrAprovPatrHist(){
		return $this->obraprovpatrhist;
	}
	public function getObrDtPrevProjetos(){
		return $this->obrdtprevprojetos;
	}
	public function getObrValorPrevisto(){
		return $this->obrvalorprevisto;
	}
	public function getObrDtAssinaturaContrato(){
		return $this->obrdtassinaturacontrato;
	}
	public function getObrDtOrdemServico(){
		return $this->obrdtordemservico;
	}
	public function getDtTerminoContrato(){
		return $this->dtterminocontrato;
	}
	public function getObrPrazoExec(){
		return $this->obrprazoexec;
	}
	public function getObrPrazoVigencia(){
		return $this->obrprazovigencia;
	}
	public function getDtInicioContrato(){
		return $this->dtiniciocontrato;
	}
	public function getTerid(){
		return $this->terid;
	}
	public function getPovid(){
		return $this->povid;
	}
	
	// Funï¿½ï¿½es GET tabela faselicitacao

	public function getFlcId(){
		return $this->flcid;
	}
	public function getTflId(){
		return $this->tflid;
	}
	public function getFlcPublEditalDtPrev(){
		return $this->flcpubleditaldtprev;
	}
	public function getFlcAberPropDtPrev(){
		return $this->flcaberpropdtprev;
	}
	public function getFlcRecInterMotivo(){
		return $this->flcrecintermotivo;
	}
	public function getFlcDtRecInterMotivo(){
		return $this->flcdtrecintermotivo;
	}
	public function getFlcHomlicDtPrev(){
		return $this->flchomlicdtprev;
	}
	public function getFlcOrdServDt(){
		return $this->flcordservdt;
	}
	public function getFlcOrdServNum(){
		return $this->flcordservnum;
	}
	public function getFlcStatus(){
		return $this->flcstatus;
	}
	public function getFlcDtInclusao(){
		return $this->flcdtinclusao;
	}

	// Funï¿½ï¿½es GET tabela formarepasserecursos

	public function getFrrId(){
		return $this->frrid;
	}
	public function getFrpId(){
		return $this->frpid;
	}
	public function getFrrConventBenef(){
		return $this->frrconventbenef;
	}
	public function getFrrConvNum(){
		return $this->frrconvnum;
	}
	public function getFrrConvObjeto(){
		return $this->frrconvobjeto;
	}
	public function getFrrConvVlr(){
		return $this->frrconvvlr;
	}
	public function getfrrConvVlrConcedente(){
		return $this->frrconvvlrconcedente;
	}
	public function getFrrConvVlrConcenente(){
		return $this->frrconvvlrconcenente;
	}
	public function getFrrDescInstituicao(){
		return $this->frrdescinstituicao;
	}
	public function getFrrDescNumPort(){
		return $this->frrdescnumport;
	}
	public function getFrrDescObjeto(){
		return $this->frrdescobjeto;
	}
	public function getFrrDescVlr(){
		return $this->frrdescvlr;
	}
	public function getFrrDescDtVigInicio(){
		return $this->frrdescdtviginicio;
	}
	public function getFrrDescDtVigFinal(){
		return $this->frrdescdtvigfinal;
	}
	public function getFrrObs(){
		return $this->frrobs;
	}
	public function getFrrStatus(){
		return $this->frrstatus;
	}
	public function getFrrDtInclusao(){
		return $this->frrdtinclusao;
	}
	public function getFrrObsRecProprio(){
		return $this->frrobsrecproprio;
	}
	public function getCovId(){
		return $this->covid;
	}

	// Funï¿½ï¿½es GET tabela itenscomposicaoobras

	public function getIcoVlrItem(){
		return $this->icovlritem;
	}
	public function getIcoDtInicioItem(){
		return $this->icodtinicioitem;
	}
	public function getIcoDterminoItem(){
		return $this->icodterminoitem;
	}
	public function getIcoPercProjPeriodo(){
		return $this->icopercprojperiodo;
	}
	public function getIcoPercExecutado(){
		return $this->icopercexecutado;
	}

	// Funï¿½ï¿½es GET tabela supervisao

	public function getSupvDt(){
		return $this->supvdt;
	}
	public function getSupVistoriador(){
		return $this->supvistoriador;
	}
	public function getSupProjEspecificacoes(){
		return $this->supprojespecificacoes;
	}
	public function getSupPlacaObra(){
		return $this->supplacaobra;
	}
	public function getSupPlacaLocalTerreno(){
		return $this->supplacalocalterreno;
	}
	public function getSupValidadeAlvara(){
		return $this->supvalidadealvara;
	}
	public function getSupObs(){
		return $this->supobs;
	}
	public function getSupVlrInfSupervisor(){
		return $this->supvlrinfsupervisor;
	}
	public function getSupParecerOrgao(){
		return $this->supparecerorgao;
	}
	public function getSupDiarioObra(){
		return $this->supdiarioobra;
	}
	public function getTplId(){
		return $this->tplid;
	}
	public function getHprObs(){
		return $this->hprobs;
	}
	public function getTpsid(){
		return $this->tpsid;
	}
	
	// Funï¿½ï¿½es GET tabela endereco

	public function getEndLog(){
		return $this->endlog;
	}
	public function getEndNum(){
		return $this->endnum;
	}
	public function getEndCom(){
		return $this->endcom;
	}
	public function getEndBai(){
		return $this->endbai;
	}
	public function getEstUf(){
		return $this->estuf;
	}
	public function getEndCep(){
		return $this->endcep;
	}
	public function getMedLatitude(){
		return $this->medlatitude;
	}
	public function getMedLongitude(){
		return $this->medlongitude;
	}
	public function getEndZoom(){
		return $this->endzoom;
	}
	public function getMunDescricao(){
		return $this->mundescricao;
	}
	public function getEndcomunidade(){ return $this->endcomunidade; }

	public function getFlcData(){
		return $this->flcdata;
	}
	public function getDcnId(){
		return $this->dcnid;
	}
	public function getQlbId(){
		return $this->qlbid;
	}
	public function getNumConvenio()
	{
		return $this->numConvenio;
	}
}

class ControllerData{

	public $simec;
	public $acao;

	public function __construct(){
		global $db;
		$this->simec  = $db;
	}

	public function setAcao($vlr){
		$this->acao = $vlr;
	}
	public function getAcao(){
		return $this->acao;
	}
}



class DadosFasesProjeto extends ControllerData{

	public $fprid  							 = null;
	public $tfpid 							 = null;
	public $obrid 							 = null;
	public $tpaid 							 = null;
	public $felid 							 = null;
	public $fprvlrformaelabrecproprio 		 = null;
	public $fprvlrformaelabrrecrepassado 	 = null;
	public $fpdtiniciofaseprojeto 			 = null;
	public $fprdtconclusaofaseprojeto 		 = null;
	public $fprobsprojcontrapartida 		 = null;
	public $fprvlrprojcontratadorecrepassad  = null;
	public $fprvlrprojcontratadorecproprio   = null;
	public $fprdtprevterminoprojeto 		 = null;
	public $fprobsexecdireta 				 = null;

	function __construct(){

		parent::__construct();

	}

	public function dados($dados){

		$this->felid=$dados['felid'];
		$this->tfpid=$dados['tfpid'];
		$this->tpaid=$dados['tpaid'];
		$this->obrid=$dados['obrid'];
		$this->fprid=$dados['fprid'];
		$this->fprobsexecdireta=$dados['fprobsexecdireta'];
		$this->fprdtconclusaofaseprojeto=$dados['fprdtconclusaofaseprojeto'];
		$this->fprdtiniciofaseprojeto=$dados['fprdtiniciofaseprojeto'];
		$this->fprobsprojcontrapartida=$dados['fprobsprojcontrapartida'];
		$this->fprvlrformaelabrecproprio=$dados['fprvlrformaelabrecproprio'];
		$this->fprvlrformaelabrrecrepassado=$dados['fprvlrformaelabrrecrepassado'];
		$this->fprvlrprojcontratadorecproprio=$dados['fprvlrprojcontratadorecproprio'];
		$this->fprvlrprojcontratadorecrepassad=$dados['fprvlrprojcontratadorecrepassad'];
		$this->fprdtprevterminoprojeto=$dados['fprdtprevterminoprojeto'];

	}

	public function busca($id){

		$result = $this->simec->executar("SELECT * FROM obras.faseprojeto WHERE obrid=".$id);
		return pg_fetch_assoc($result);

	}

}

Class DadosInfraEstrutura extends ControllerData{

	public $umdidareaconstruida 			= null;
	public $umdidareareforma 				= null;
	public $umdidareaampliada 				= null;
	public $iexsitdominialimovelregulariza  = null;
	public $iexinfexistedimovel 			= null;
	public $iexareaconstruida 				= null;
	public $iexdescsumariaedificacao 		= null;
	public $iexedificacaoreforma 			= null;
	public $iexqtdareapreforma 				= null;
	public $iexvlrareapreforma 				= null;
	public $iexampliacao 					= null;
	public $iexqtdareaampliada 				= null;
	public $iexvlrareaampliada 				= null;
	public $aqiid			 				= null;

	function __construct(){

		parent::__construct();

	}

	public function dados($dados){

		$this->umdidareaconstruida			  = $dados['umdidareaconstruida'];
		$this->umdidareareforma				  = $dados['umdidareareforma'];
		$this->umdidareaampliada			  = $dados['umdidareaampliada'];
		$this->iexsitdominialimovelregulariza = $dados['iexsitdominialimovelregulariza'];
		$this->iexinfexistedimovel			  = $dados['iexinfexistedimovel'];
		$this->iexareaconstruida			  = $dados['iexareaconstruida'];
		$this->iexdescsumariaedificacao		  = $dados['iexdescsumariaedificacao'];
		$this->iexedificacaoreforma			  = $dados['iexedificacaoreforma'];
		$this->iexqtdareapreforma			  = $dados['iexqtdareapreforma'];
		$this->iexvlrareapreforma			  = $dados['iexvlrareapreforma'];
		$this->iexampliacao					  = $dados['iexampliacao'];
		$this->iexqtdareaampliada			  = $dados['iexqtdareaampliada'];
		$this->iexvlrareaampliada			  = $dados['iexvlrareaampliada'];
		$this->iexvlrareaampliada			  = $dados['iexvlrareaampliada'];
		$this->aqiid						  = $dados['aqiid'];

	}

	public function busca($id){
		$result = $this->simec->executar("
									SELECT 
										* 
									FROM 
										obras.infraestrutura AS inf
									INNER JOIN
										obras.obrainfraestrutura AS obr
									ON
										inf.iexid = obr.iexid
									WHERE
										obr.obrid = ".$id);
		return pg_fetch_assoc($result);
	}

}

Class AquisicaoEquipamentos extends ControllerData{

	public $aeqid 				  = null;
	public $faeid 				  = null;
	public $aeqdtpubledital 	  = null;
	public $aeqdtpublreslicitacao = null;
	public $aeqobs 				  = null;

	public function __construct(){

		parent::__construct();

	}

	public function dados($dados){

		$this->aeqid=$dados['aeqid'];
		$this->faeid=$dados['faeid'];
		$this->aeqdtpubledital=$dados['aeqdtpubledital'];
		$this->aeqdtpublreslicitacao=$dados['aeqdtpublreslicitacao'];
		$this->aeqobs=$dados['aeqobs'];

	}

	public function busca($id){
		$result = $this->simec->executar("
									SELECT 
										* 
									FROM 
										obras.aquisicaoequipamentos
									WHERE
										obrid = ".$id);
		$this->simec->commit();
		return pg_fetch_assoc($result);

	}

}

Class ComposicaoBdi extends ControllerData{

	public $bdiid 		= null;
	public $bdidesc 	= null;
	public $bdivlritem  = null;
	public $bdipercitem = null;

	public function __construct(){
		parent::__construct();
	}

	public function dados($dados){
		$this->bdiid=$dados['bdiid'];
		$this->bdidesc=$dados['bdidesc'];
		$this->bdivlritem=$dados['bdivlritem'];
		$this->bdipercitem=$dados['bdipercitem'];
	}

	public function busca($id){
		$result = $this->simec->executar("
									SELECT 
										* 
									FROM 
										obras.itensbdi
									WHERE
										obrid = ".$id);
		$this->simec->commit();
		return pg_fetch_assoc($result);
	}

}

Class DadosRestricao extends ControllerData{

	public $trtid 					   = null;
	public $rstdesc 				   = null;
	public $rstdescprovidencia 		   = null;
	public $rstdtprevisaoregularizacao = null;
	public $rstdtsuperacao 			   = null;
	public $rstsituacao 			   = null;
	public $fsrid					   = null;

	function __construct(){

		parent::__construct();

	}
	public function dados($dados){

		$this->trtid=$dados['trtid'];
		$this->rstdesc=$dados['rstdesc'];
		$this->rstdescprovidencia=$dados['rstdescprovidencia'];
		$this->rstdtprevisaoregularizacao=$dados['rstdtprevisaoregularizacao'];
		$this->rstdtsuperacao=$dados['rstdtsuperacao'];
		$this->rstsituacao=$dados['rstsituacao'];
		$this->fsrid=$dados['fsrid'];

	}

	public function busca($id){
		$result = $this->simec->pegaLinha("
									SELECT 
										* 
									FROM 
										obras.restricaoobra
									WHERE
										rststatus = 'A' AND
										rstoid = ".$id);

		return $result;
	}

}

Class Obras{

	public $simec;
	public $acao;

	public function __construct(){
		global $db;
		$this->simec  = $db;
	}

	public function setAcao($vlr){
		$this->acao = $vlr;
	}

	public function getAcao(){
		return $this->acao;
	}

	private function Strip_Str($str){
		$string = str_replace("-","",$str);
		$string = str_replace(".","",$string);
		return $string;
	}

	public static function MoedaToBd($vlr){
		$string = str_replace(".","",$vlr);
		$string = str_replace(",",".",$string);

		return $string;
	}

	/**
	 * Funï¿½ï¿½o que cadastra uma obra
	 * @author Fernando A. Bagno da Silva
	 * @param array $obra
	 *
	 */
	public function CadastrarObras($obra){
	
		if(!$obra["entidcampus"]){
			$entidcampus = 'NULL';
		}else{
			$entidcampus = $obra['entidcampus'];
		}

		if(!$obra["prfid"]){
			$prfid = 'NULL';
		}else{
			$prfid = $obra["prfid"];
		}
		
		if(!$obra["fntid"]){
			$fntid = 'NULL';
		}else{
			$fntid = $obra["fntid"];
		}

		if(!$obra["obrcomposicao"]){
			$obrcomposicao = 'NULL';
		}else{
			$obrcomposicao = "'".$obra['obrcomposicao']."'";
		}

		if(!$obra["tpoid"]){
			$tpoid = 'NULL';
		}else{
			$tpoid = "'".$obra['tpoid']."'";
		}

		if(!$obra["terid"]){
			$terid = 'NULL';
		}else{
			$terid = $obra['terid'];
		}
		
		if(!$obra["povid"]){
			$povid = 'NULL';
		}else{
			$povid = "'".$obra['povid']."'";
		}
		
		if(!$obra["endcomunidade"]){
			$endcomunidade = 'NULL';
		}else{
			$endcomunidade = "'".$obra["endcomunidade"]."'";
		}

		if(!$obra["obrstatusinauguracao"]){
			$obrstatusinauguracao = 'NULL';
		}else{
			$obrstatusinauguracao = "'".$obra["obrstatusinauguracao"]."'";
		}

		if(!$obra["obrdtinauguracao"]){
			$obrdtinauguracao = 'NULL';
		}else{
			$obrdtinauguracao = "'".$obra["obrdtinauguracao"]."'";
		}

		if(!$obra["obrdtprevinauguracao"]){
			$obrdtprevinauguracao = 'NULL';
		}else{
			$obrdtprevinauguracao = "'".$obra["obrdtprevinauguracao"]."'";
		}

		if(!$obra["endzoom"]){
			$endzoom  = 'NULL';
		}else{
			$endzoom  = $obra["endzoom"];
		}
		// Concatena os dados de coordenadas geogrï¿½ficas
		if(trim($obra["graulatitude"]) != ""){
			$latitude = $obra["graulatitude"] . "." .  $obra["minlatitude"] . "." . $obra["seglatitude"]. "." . $obra["pololatitude"];
			$longitude = $obra["graulongitude"] . "." . $obra["minlongitude"] . "." . $obra["seglongitude"];
				
		}

		// Atribui valores nulos aos campos em branco e coloca aspas
		$ids = Array("cloid", "orgid", "obrid", "entid", "entidcampus", "tobaid", "prfid", "fntid", "tpoid", "terid", "povid", "aqiid");
		
		foreach($obra as $campo=>$valor){
			if(!is_array($obra[$campo])){
				if($valor == "" ){
					$obra[$campo] = 'NULL';
				} else {
					if(!in_array($campo, $ids)){
						$obra[$campo] = "'" . pg_escape_string(trim($valor))  .  "'";
					}
				}
			}
		}

		// Insere os dados na tabela endereco
		$__obra = $obra;
		$obra = array_map('x', $_REQUEST['endereco']);

		$sql = "INSERT INTO entidade.endereco (endcep,
											   endlog,
											   endcom,
											   endbai,
											   muncod,
											   estuf,
											   endnum,
											   medlatitude,
											   medlongitude,
											   endzoom,
											   endstatus,
											   endcomunidade) ";	

//		$sql .="VALUES (".trim($this->Strip_Str($obra["endcep"])).",

		$obra["endcep"] = $this->Strip_Str($obra["endcep"]);
		$obra["endcep"] = trim($obra["endcep"]);
		$obra["endcep"] = str_replace("'","",$obra["endcep"]);
		$obra["endcep"] = substr($obra["endcep"], 0, 8);
		$obra["endcep"] = "'".$obra["endcep"]."'";
		
		$sql .="VALUES ({$obra["endcep"]},
		{$obra["endlog"]},{$obra["endcom"]},
		{$obra["endbai"]},{$obra["muncod"]},
		{$obra["estuf"]},{$obra["endnum"]},
	    '{$latitude}',
	    '{$longitude}',
	    {$obra["endzoom"]} ,
	    'A',
	    {$endcomunidade}) returning endid";

	    $endid = $this->simec->pegaUm($sql);
	    $obra = $__obra;
	    $entid = $obra["entidunidade"];

	    // Parte nova sobre a infraestrutura
	    if( $obra["aqiid"] ){
			$sql = "INSERT INTO obras.infraestrutura( aqiid, iexsitdominialimovelregulariza)
										  VALUES( {$obra["aqiid"]}, {$obra["iexsitdominialimovelregulariza"]}) 
										  RETURNING iexid";
	    }else{
	    	$sql = "INSERT INTO obras.infraestrutura( aqiid, iexsitdominialimovelregulariza)
										  VALUES( null, {$obra["iexsitdominialimovelregulariza"]}) 
										  RETURNING iexid";
	    }
		$iexid = $this->simec->pegaUm( $sql );
	    
		$obra["obrvalorprevisto"] = str_replace( ".", "", $obra["obrvalorprevisto"] );
		$obra["obrvalorprevisto"] = str_replace( ",", ".", $obra["obrvalorprevisto"] );
		if( strlen($obra["obsobra"]) > 1000){
			$obra["obsobra"] = substr($obra["obsobra"],0,999)."'";
		}
		
	    // Insere os dados na tabela obrainfraestrutura
	    $sql = "INSERT INTO obras.obrainfraestrutura (orgid,
									  entidunidade,
									  obrdesc,
									  endid,
									  obrpercexec,
									  obsstatus,
									  obsobra, 
									  usucpf, 
									  entidcampus, 
									  cloid, 
									  tpoid, 
									  prfid,
									  fntid, 
									  obrcomposicao,
									  tobraid,
									  iexid,
									  obrvalorprevisto,
									  terid,
									  povid)";
	    $sql .= " VALUES ({$obra["orgid"]},
	    {$obra["entid"]},
	    {$obra["obrdesc"]},
	    {$endid},
		0.00,
		'A',
		{$obra["obsobra"]},
		'{$_SESSION["usucpf"]}', 
		{$entidcampus},
		{$obra["cloid"]},
		{$tpoid},
		{$prfid},
		{$fntid},
		{$obrcomposicao},
		{$obra["tobraid"]},
		{$iexid},
		{$obra["obrvalorprevisto"]},
		{$terid},
		{$povid}) returning obrid";

		$obrid = ($this->simec->pegaUm($sql));

		$_SESSION["obra"]["obrid"] = $obrid;

		// Se for obra indï¿½gena
		if ($_SESSION["sisid"] == ID_PARINDIGENA){

			// Cria o monitoramento
			$sql = "INSERT INTO parindigena.itemmonitoramento (usucpf, estuf, esaid, obrid, itmnome)
					VALUES ('{$_SESSION["usucpf"]}','{$obra["estuf"]}','2', '{$obrid}', '{$obra["obrdescunidimplantada"]}') returning itmid";
			  		
			$itmid = $this->simec->pegaUm($sql);
			  		
			// Insere o convï¿½nio que a obra foi cadastrada
			$sql = "";
			$sql = "INSERT INTO obras.formarepasserecursos (covid)
 					VALUES ('{$obra["covid"]}')";
			$this->simec->executar($sql);
		
		}
		
		// Insere os editores da obra
		if ( is_array($obra["rpuid"]) ){
			foreach ($obra["rpuid"] as $chave=>$valor){
				//$sql = "SELECT usucpf, pflcod FROM obras.usuarioresponsabilidade WHERE rpuid = {$valor}";
				$sql = "SELECT usucpf, pflcod FROM obras.usuarioresponsabilidade WHERE usucpf = '{$valor}'";
				$dadosUsuario = $this->simec->carregar($sql);
				
				$sql = "INSERT INTO obras.usuarioresponsabilidade ( usucpf, rpustatus, rpudata_inc, 
																	pflcod, obrid )
														   VALUES ( '{$dadosUsuario[0]["usucpf"]}', 'A', 'now', 
																	{$dadosUsuario[0]["pflcod"]}, {$_SESSION["obra"]["obrid"]})";
				
				$this->simec->executar($sql);
				
			}
		}
		
		// Insere os dados na tabela de responsï¿½veis
		if (is_array($obra["tprcid"])){

		  	foreach ($obra["tprcid"] as $chave=>$valor){
		  		$sql = "";
		  		$sql = "INSERT INTO
						obras.responsavelcontatos (entid, 
												   tprcid,
			 									   recostatus, 
			 						   			   recodtinclusao)
						VALUES 
							({$chave}, {$valor}, 'A', 'now()') 
						RETURNING 
							recoid";

		  		$recoid = $this->simec->pegaUm($sql);
	
		  		// Cria o relacionamento entre o responsï¿½vel e a obra
		  		$sql = "";
		  		$sql = "
				INSERT INTO obras.responsavelobra (recoid, obrid)
				VALUES ({$recoid}, {$_SESSION["obra"]["obrid"]})";

			  	$this->simec->executar($sql);

		 	}
		}

		$this->simec->commit();
			  	
		$_REQUEST["acao"] = "A";
		$this->simec->sucesso("principal/cadastro","&obrid={$_SESSION["obra"]["obrid"]}");

	}

	public function AtualizaComboUnidadeObra($orgid){

		$where = "where obra.orgid = {$orgid} and obra.obsstatus = 'A'";

		if($orgid == "")
		$where = " where obra.obsstatus = 'A'";
			
		$sql = "SELECT obra.entid as codigo, obra.entnome as descricao
				FROM (
				(
				obras.obrainfraestrutura oi 
				INNER JOIN
				entidade.entidade et ON oi.entidunidade = et.entid 
				) obr
				INNER JOIN 
				obras.situacaoobra sto ON obr.stoid = sto.stoid
		     	) obra {$where} group by obra.entnome,obra.entid";
		$res = $this->simec->carregar($sql);



		if(is_array($res)){

			$n = count($res);
			$k = 0;
			$lista = "";

			foreach($res as $campo){
				$lista .= $campo["codigo"]."-".$campo["descricao"];
				if($k < $n -1)
				$lista .= "|";
				$k++;
			}
			print_r($lista);
		}else{
			print("");
		}
	}

	public function CadastrarCronogramaObras($obra){

		// verifica a sessï¿½o da obra
		obras_verifica_sessao();

		foreach($obra as $campo=>$valor){
			if($valor == "" ){
				$obra[$campo] = 'NULL';
			} else {
				$obra[$campo] = "'" . pg_escape_string(trim($valor))  .  "'";
			}
		}

		$i = 0;

		foreach($obra as $nome=>$valor){

			$pos = strpos($nome,"_");
				
			if($pos > 0){
					
				if(($i % 3) >= 0){

					$pos = strpos($nome,"_");
					$nome = substr($nome,0,$pos);
						
					if ($nome != "item"){
						$valor = str_replace(".","",$valor);
						$valor = str_replace(",",".",$valor);
						$query .= $nome." = ".$valor;
					}
						
				}

				if(($i % 3) > 1){
					$codigo = $valor;

				}elseif(($i % 3) < 1){
						
					$query .= ", ";
						
				}

				if(($i % 3) == 1){
						
					$query .= "  WHERE itcid =" . $codigo . " AND obrid = '{$_SESSION["obra"]["obrid"]}'";

					$sql = "UPDATE obras.itenscomposicaoobra SET " . $query;
						
					$query = "";
						
					$this->simec->executar($sql);
					$this->simec->commit();
						
				}
			}
			$i++;
		}

		$_REQUEST["acao"] = "A";
		$this->simec->sucesso("principal/cronograma");
			
	}

	/**
	 * Faz as verificaï¿½ï¿½es antes de excluir uma obra, o mï¿½todo faz o tratamento das regras de negï¿½cio
	 *
	 * @param integer $obrid
	 * @return bool
	 * @author Cristiano Teles
	 */
	public function antesDeletarObra( $obrid ){

		// Verifica se existe vistoria/supervisï¿½o da obra
		$stSql = ' select count(*) from obras.supervisao WHERE obrid = ' . $obrid . ' AND supstatus = \'A\' ';
		$arVistoria = $this->simec->carregar( $stSql );
		
		if ( $arVistoria[0]['count'] > 0 ){
			return 'Esta obra não pode ser excluir porque possui vistorias cadastradas!';
		}
		
		return true;
		
	}

	public function DeletarObras($obrid){
		/*Rotina "Testa se ï¿½ um aditivo" inativada dia 26/11/2010 as 10:00 H. */ 
		//Testa se ï¿½ um aditivo
		/*$sql = "SELECT
					CASE WHEN obridaditivo IS NOT NULL
						THEN TRUE
						ELSE FALSE
					END as testaaditivo
				FROM
					obras.obrainfraestrutura
				WHERE
					obrid = ".$obrid;
		
		$testaAditivo = $this->simec->pegaUm($sql);
		
		if ( $testaAditivo == 't' ){
			$sql = "UPDATE 
						obras.obrainfraestrutura 
					SET 
						obsstatus = 'I' 
					WHERE 
						obrid = {$obrid}
					RETURNING 
						obridaditivo";
			$obridAditivo = $this->simec->pegaUm($sql);
			
			$sql = "UPDATE 
						obras.obrainfraestrutura 
					SET 
						obsstatus = 'A' 
					WHERE 
						obrid = {$obridAditivo}";
			$this->simec->executar($sql);
			$this->simec->commit();
			$_REQUEST["acao"] = "A";
			$this->simec->sucesso("inicio");
		}else{*/
			//Verifica se pode excluir a obra
			if ( true !== ( $stMensagem = $this->antesDeletarObra( $obrid ) ) ) {
				return $stMensagem;
			}
			
			$this->simec->executar("UPDATE obras.obrainfraestrutura SET obsstatus = 'I' WHERE obrid = ".$obrid);
			$this->simec->commit();
			$_REQUEST["acao"] = "A";
			$this->simec->sucesso("inicio");
		/*}*/
	}

	public function Dados($obrid, $supvid = null, $status = 'A'){
		$where = "";
		if($obrid)
		$where .= "AND obr.obrid=".$obrid;

		if($supvid)
		$where .= " AND sup.supvid=".$supvid . " AND sup.supstatus = 'A'";
			
		$res = $this->simec->pegaLinha("
								SELECT 
									obr.*, 
									flc.*, 
									frr.*, 
									sup.*,
									adr.*,
									mun.*,
									hrp.* 
								FROM 
									((obras.obrainfraestrutura obr 
								LEFT JOIN 
									obras.faselicitacao flc ON obr.obrid = flc.obrid) 
								LEFT JOIN 
									obras.formarepasserecursos frr ON obr.obrid = frr.obrid)
								LEFT JOIN
									obras.supervisao sup ON obr.obrid = sup.obrid
								LEFT JOIN
									obras.historicoparalisacao hrp ON hrp.supvidparalisacao = sup.supvid 
								LEFT JOIN 
									entidade.endereco adr ON obr.endid = adr.endid
								LEFT JOIN 
									territorios.municipio mun ON adr.muncod = mun.muncod
								WHERE 
									obr.obsstatus = '{$status}' ".$where);

		return $res;

	}

	public function buscaUF( $obrid ){
		if ( !$obrid ){
			return "";
		}
		
		$sql = "SELECT
					estuf
				FROM
					obras.obrainfraestrutura
				JOIN entidade.endereco USING(endid)
				WHERE
					obrid = {$obrid}";
		
		return $this->simec->pegaUm( $sql );
	}
	
	public function getDadosConvenio($numConvenio)
	{
		$sql = "select
					dcoano
				from
					painel.dadosconvenios dc
				where
					dcoconvenio = '$numConvenio'";
		
		return $this->simec->pegaLinha($sql);
		
	}
	
	public function ViewObra($obrid){

		$res = $this->simec->pegaLinha("
			select 
			ent.entnome as entidade,
			oie.obrdescundimplantada as unidade,
			oie.obrdesc as nome,
			oie.obrcustocontrato,
			org.orgdesc as orgao,
			ROUND(oie.obrpercexec,0)||'%' as executado
			from 
			(obras.obrainfraestrutura oie INNER JOIN obras.orgao org ON oie.orgid=org.orgid)
			LEFT JOIN 
			entidade.entidade ent ON oie.entidunidade = ent.entid  
			where oie.obsstatus = 'A' and oie.obrid=".$obrid);
		return $res;

	}

	public function ViewPercentualExecutado($obrid){

//		$percentual = $this->simec->carregar("select SUM(icopercexecutado) as total from obras.itenscomposicaoobra WHERE obrid = ".$obrid);
//		$percentual = $this->simec->carregar("SELECT 
//												SUM(icopercexecutado) as total
//											  FROM 
//												obras.itenscomposicaoobra ico
//											  JOIN obras.supervisaoitenscomposicao sic ON sic.icoid = ico.icoid
//											  WHERE 
//												sic.supvid IN (SELECT MAX(supvid) FROM obras.supervisao WHERE supstatus = 'A' AND obrid = {$obrid})
//												AND ico.icostatus = 'A';");

		$traid = pegaObUltimoAditivo('traid', NULL, $obrid);
		if ($traid){
			$percentual = $this->simec->carregar("SELECT 
													SUM(icopercexecutado) as total
												  FROM 
													obras.itenscomposicaoobra ico
												  WHERE 
												  	obrid = {$obrid}
												  	AND traid = {$traid}
												  	AND ico.icovigente = 'A'
													AND ico.icostatus = 'A';");
		}else{
			$percentual = $this->simec->carregar("SELECT 
													SUM(icopercexecutado) as total
												  FROM 
													obras.itenscomposicaoobra ico
												  JOIN obras.supervisaoitenscomposicao sic ON sic.icoid = ico.icoid
												  WHERE 
													sic.supvid IN (SELECT supvid FROM obras.supervisao WHERE supstatus = 'A' AND obrid = {$obrid} ORDER BY supvdt DESC LIMIT 1) AND ico.icostatus = 'A' AND ico.icovigente = 'A';");
		}
		return $percentual[0]["total"];

	}

	public function CadastrarComposicaoBdi($dados){

		$sql_del = "DELETE FROM obras.itensbdi WHERE obrid='". $_SESSION['obra']["obrid"] ."'";
		$res = $this->simec->executar($sql_del);
		$this->simec->commit();

		if(count($_REQUEST['tabela1_itensbdi']) > 0) {
			foreach($_REQUEST['tabela1_itensbdi'] as $itens) {
				$itens['bdidesc'] = $itens['bdidesc'];
				$itens['bdivlritem'] = str_replace(array(".",","),array("","."),($itens['bdivlritem'])?$itens['bdivlritem']:'0,00');
				$itens['bdipercitem'] = str_replace(array(".",","),array("","."),($itens['bdipercitem'])?$itens['bdipercitem']:'0,00');
				$sql_insert = "INSERT INTO obras.itensbdi(OBRID, bdidesc, bdivlritem, bdipercitem)
	   					   		   VALUES ('". $_SESSION['obra']["obrid"] ."', '". $itens['bdidesc'] ."', '". $itens['bdivlritem'] ."', '". $itens['bdipercitem'] ."')";
				$res = $this->simec->executar($sql_insert);
			}
		}
			
		$this->simec->commit();

		$_REQUEST["acao"] = "A";
		$this->simec->sucesso("principal/composicao_bdi");
			
	}

	public function busca($sql){
		$res = $this->simec->executar($sql);
		while($dados = pg_fetch_assoc($res)){
				
			$result[] = $dados;
		}
		return $result;
			
	}

	/**
	 * Funï¿½ï¿½o que atualiza uma obra
	 * @author Fernando A. Bagno da Silva
	 * @param array $obra
	 *
	 */
	public function AtualizarObras($obra){
		
		// verifica a sessï¿½o da obra
		obras_verifica_sessao();
		
		$this->simec->executar("DELETE FROM obras.usuarioresponsabilidade 
								WHERE obrid = {$_SESSION["obra"]["obrid"]}");
		
		// Atualiza os editores da obra
		if ( is_array($obra["rpuid"]) ){
			foreach ($obra["rpuid"] as $chave=>$valor){
				$sql = "SELECT 
							usucpf, pflcod 
						FROM 
							obras.usuarioresponsabilidade ur
						WHERE 
							usucpf = '{$valor}'
							AND ur.rpustatus = 'A'
							AND (ur.entid = {$obra['entid']} OR ur.orgid = {$_SESSION["obra"]["obrid"]} OR ur.estuf = '{$obra['endereco']['estuf']}')
							AND ur.pflcod IN (" . PERFIL_SUPERVISORUNIDADE . ", 
											  " . PERFIL_GESTORUNIDADE . ", 
											  " . PERFIL_CONSULTATIPOENSINO . ",
											  " . PERFIL_EMPRESA . ")";
				$arDadosUsuario = $this->simec->carregar($sql);
				
				if ( $arDadosUsuario ){
					foreach ($arDadosUsuario as $dadosUsuario){
						$sql = "INSERT INTO obras.usuarioresponsabilidade 
								( usucpf, rpustatus, rpudata_inc,pflcod, obrid )
								VALUES 
								( '{$dadosUsuario["usucpf"]}', 'A', 'now',{$dadosUsuario["pflcod"]}, {$_SESSION["obra"]["obrid"]})";
						$this->simec->executar($sql);
					}
				}
				
			}
		}
		
		// Atualiza os responsï¿½veis da obra
		if (is_array($obra["tprcid"])){

			$this->simec->executar("
					UPDATE 
						obras.responsavelcontatos oc 
					SET
						recostatus = 'I'
					FROM
						obras.responsavelobra oo
					WHERE
						oc.recoid = oo.recoid AND
						oo.obrid = {$_SESSION["obra"]["obrid"]}");
				
			$this->simec->executar("DELETE FROM obras.responsavelobra WHERE obrid = {$_SESSION["obra"]["obrid"]}");

			foreach ($obra["tprcid"] as $chave=>$valor){
				$sql = "";
				$sql = "
					INSERT INTO
						obras.responsavelcontatos (entid, tprcid,
						 						   recostatus, recodtinclusao)
					VALUES 
						({$chave}, {$valor}, 'A', 'now()') 
					RETURNING 
						recoid";

				$recoid = $this->simec->pegaUm($sql);

				// Cria o relacionamento entre o responsï¿½vel e a obra
				$sql = "";
				$sql = "
					INSERT INTO obras.responsavelobra (recoid, obrid)
					VALUES ({$recoid}, {$_SESSION["obra"]["obrid"]})";

				$this->simec->executar($sql);
					
			}
		}

		if(!$obra["entidcampus"]){
			$entidcampus = 'NULL';
		}else{
			$entidcampus = $obra['entidcampus'];
		}

		/*
		 * Inclusï¿½o da subaï¿½ï¿½o (sbaid) e Categoria de Apropriaï¿½ï¿½o (cpiid)
		 * Solicitada por Cristiano Cabral (28/04/2009)
		 * Feita por Alexandre Dourado
		 */
		if(!$obra["sbaid"]){
			$sbaid = 'NULL';
		}else{
			$sbaid = $obra["sbaid"];
		}

		if(!$obra["obrcomposicao"]){
			$obrcomposicao = 'NULL';
		}else{
			$obrcomposicao = "'". substr($obra["obrcomposicao"], 0, 1500) ."'"; //limitando o campo a 1500 caracteres "'".$obra["obrcomposicao"]."'";
		}

		if(!$obra["terid"]){
			$terid = 'NULL';
		}else{
			$terid = $obra['terid'];
		}
		
		if(!$obra["povid"]){
			$povid = 'NULL';
		}else{
			$povid = $obra['povid'];
		}
		
		$tpoid = 'NULL';
		if ( empty( $obra["tpoid"] ) ) {
			if ( !empty( $obra["tpoid_disable"] ) ) {
				$tpoid = $obra["tpoid_disable"];
			}
		}
		else {
			$tpoid = $obra["tpoid"]; //. "'";
		}

		if(!$obra["endcomunidade"]){
			$endcomunidade = 'NULL';
		}else{
			$endcomunidade = "'".$obra["endcomunidade"]."'";
		}

		$obrstatusinauguracao = $obra["obrstatusinauguracao"] ? "'" . $obra["obrstatusinauguracao"] . "'" : 'NULL';
		$obrdtinauguracao 	  = $obra["obrdtinauguracao"] 	  ? "'" . formata_data_sql($obra["obrdtinauguracao"]) . "'" 	  : 'NULL';
		$obrdtprevinauguracao = $obra["obrdtprevinauguracao"] ? "'" . formata_data_sql($obra["obrdtprevinauguracao"]) . "'" : 'NULL';
		
		if(trim($obra["graulatitude"]) != ""){
			$latitude = $obra["graulatitude"] . "." . $obra["minlatitude"] . "." . $obra["seglatitude"] . "." . $obra["pololatitude"];
			$longitude = $obra["graulongitude"] . "." . $obra["minlongitude"] . "." . $obra["seglongitude"];
		}

		$iexid = ($this->simec->pegaUm("SELECT iexid FROM obras.obrainfraestrutura	WHERE obrid = {$_SESSION["obra"]["obrid"]}"));
		
		  
		
		if( $iexid ){

			if( $obra["aqiid"] ){
				$sql = "UPDATE
						obras.infraestrutura
					SET
						aqiid = {$obra["aqiid"]}, 
						iexsitdominialimovelregulariza = '{$obra["iexsitdominialimovelregulariza"]}'
					WHERE
						iexid = {$iexid}";
			}else{
				$sql = "UPDATE
						obras.infraestrutura
					SET
						aqiid = null, 
						iexsitdominialimovelregulariza = '{$obra["iexsitdominialimovelregulariza"]}'
					WHERE
						iexid = {$iexid}";
				
			}
			$this->simec->executar( $sql );
			
		}else{
			
			// Parte nova sobre a infraestrutura
			if( $obra["aqiid"] ){
				$sql = "INSERT INTO obras.infraestrutura( aqiid, iexsitdominialimovelregulariza)
											  VALUES( {$obra["aqiid"]}, '{$obra["iexsitdominialimovelregulariza"]}' ) 
										  	RETURNING iexid";
			}else{
				$sql = "INSERT INTO obras.infraestrutura( aqiid, iexsitdominialimovelregulariza)
											  VALUES( null, '{$obra["iexsitdominialimovelregulariza"]}' ) 
										  	RETURNING iexid";
				
			}
			$attInfra = $this->simec->pegaUm( $sql );
			
			
		}
		
		// Atribui valores nulos aos campos em branco e coloca aspas
		$ids = Array("cloid", "orgid", "obrid", "entid", "entidcampus", "tobaid", "prfid", "fntid", "tpoid", "terid", "povid", "aqiid");
		
		foreach($obra as $campo=>$valor){
			if($valor == "" ){
				$obra[$campo] = 'NULL';
			} else {
				if(!in_array($campo, $ids)){
					$obra[$campo] = "'" . pg_escape_string(trim($valor))  .  "'";
				}
			}
		}
		
		$obra["obrvalorprevisto"] = str_replace( ".", "", $obra["obrvalorprevisto"] );
		$obra["obrvalorprevisto"] = str_replace( ",", ".", $obra["obrvalorprevisto"] );
		if( strlen($obra["obsobra"]) > 1000){
			$obra["obsobra"] = substr($obra["obsobra"],0,999)."'";
		}

		$sql = "
			UPDATE 
				obras.obrainfraestrutura 
			SET 
			  --orgid 					  = {$obra["orgid"]},
				entidunidade 			  = {$obra["entid"]},
				obrdesc 				  = {$obra["obrdesc"]},
				obsobra 				  = {$obra["obsobra"]},
				entidcampus 			  = {$entidcampus},
				cloid 					  = {$obra["cloid"]},
				tpoid 					  = {$tpoid},
				prfid 					  = {$obra["prfid"]},
				fntid 					  = ". (($obra["fntid"])?$obra["fntid"]:"NULL") .",
				obrcomposicao 			  = {$obrcomposicao},
				tobraid					  = {$obra["tobraid"]},
				obrstatusinauguracao 	  = {$obrstatusinauguracao},
				obrdtinauguracao 		  = {$obrdtinauguracao},
				obrdtprevinauguracao 	  = {$obrdtprevinauguracao},
				obrvalorprevisto		  = {$obra["obrvalorprevisto"]},
				terid					  = {$terid},
				povid					  = {$povid}
				" . ( $attInfra ? ", iexid = {$attInfra}" : "" ) . "
			WHERE
				obrid = ".$obra["obrid"];

		//Atualiza a obra
		$this->simec->executar( $sql );

		$obra = array_map('x', $_REQUEST['endereco']);
		$obra['endcomunidade'] = pg_escape_string(trim( $_REQUEST['endcomunidade']));
		$endzoom = !$obra["endzoom"] ? $obra["endzoom"] : 'NULL';
			
		// Atualiza a tabela entidade.endereco
		$sql = "
			UPDATE 
				entidade.endereco en
			SET
				endcep       = {$this->Strip_Str($obra["endcep"])},
				endlog       = {$obra["endlog"]},
				endcom       = {$obra["endcom"]},
				endbai       = {$obra["endbai"]},
				muncod       = {$obra["muncod"]},
				estuf        = {$obra["estuf"]},
				endnum       = {$obra["endnum"]},
				medlatitude  = '{$latitude}',
				medlongitude = '{$longitude}',
				endzoom		 = {$obra["endzoom"]},
				endcomunidade = {$endcomunidade}
			FROM
				obras.obrainfraestrutura o
			WHERE
				o.endid = en.endid AND
				o.obrid = {$_SESSION["obra"]["obrid"]}";
		$this->simec->executar( $sql );
		$this->simec->commit();

		$this->simec->sucesso("principal/cadastro", "&obrid={$_SESSION["obra"]["obrid"]}");
		
	}

	public function CabecalhoObras(){
		session_start();

		if( session_is_registered("obra") && ($_SESSION["obra"]["obrid"] != '') ){
			$obrid = $_SESSION["obra"]["obrid"];
			$obra = $this->ViewObra($obrid);
				
			/* Mï¿½todo antigo de calcular o percentual executado
			$percentualExecutado = $this->ViewPercentualExecutado($obrid);
			if(!$percentualExecutado) { $percentualExecutado = 0; }*/
				
			/*$percentualExecutado = ( $percentualExecutado > 100.00 ) ? 100.00 : $percentualExecutado;
			$percentualExecutado = number_format($percentualExecutado, 2, ',', '.');
			$percentualExecutado.=" %";*/
			
			// mï¿½todo novo de calcular o percentual executado
			$percentualExecutado = mostraPercentualUltimaVistoria($obrid);
				
			$vlrAditivo = pegaObMaiorVlrAditivo();
			
			$titulos = array("Unidade Responsável","Nome da Obra ",($vlrAditivo ? "Valor contratado da obra após o aditivo (R$)" : "Valor contratado da obra (R$)"), ( $vlrAditivo ? "(%) Concluído (Físico) após aditivo" : "(%) Concluído (Físico)") );
			$obra_list = array($obra["entidade"],"<b>({$obrid})</b> - " . $obra['nome'],number_format(($vlrAditivo ? $vlrAditivo : $obra['obrcustocontrato']),2,',','.'),$percentualExecutado);
				
			$cabecalho = "<table class=Tabela align=center>";
				
			for($i=0;$i<count($titulos);$i++){
				$cabecalho .= "<tr>";
				$cabecalho .= "<td width=100px class=SubTituloEsquerda style='text-align:right;' >";
				$cabecalho .= $titulos[$i];
				$cabecalho .= "</td>";
				$cabecalho .= "<td width=80% class=SubTituloDireita style='text-align:left;background:#EEE;' >";
				$cabecalho .= $obra_list[$i];
				$cabecalho .= "</td>";
				$cabecalho .= "</tr>";
			}

			$obraAditivoVistoria   = obraAditivoPossuiVistoria();	
			$obraAditivoCronograma = obraAditivoPossuiCronograma();
			
			if ( !$obraAditivoCronograma || !$obraAditivoVistoria ){
				$cabecalho .= "<tr>";
				$cabecalho .= "<td colspan='2' bgcolor='#E9E9E9' style='text-align:center;' >";
				
				if ( !$obraAditivoCronograma )
					$cabecalho .= "<b><font color='red'><blink>*</blink> É necessário o preenchimento do cronograma físico-financeiro para liberar a edição dos dados</font></b><br>";
					
				if ( !$obraAditivoVistoria )
					$cabecalho .= "<b><font color='red'><blink>*</blink> É necessário a inserção da vistoria para liberar a edição dos dados</font></b>";

				$cabecalho .= "</td>";
				$cabecalho .= "</tr>";
			}	
			
			$cabecalho .= "</table>";

			return $cabecalho;
				
		}else{

			return "<br /><br /><hr /><center>Não existe nenhuma obra escolhida ...</center><br /><br /><hr />";

		}


	}

	public function getPercentualdoItem($icoid){
		$sql="select sum(supvlrinfsupervisor) as total from obras.supervisaoitenscomposicao where icoid =".$icoid;
		return $this->simec->pegaUm($sql);

	}
	
	public function atualizaConv($obra){
		
		$obra['covnumero']		  = $obra['covnumero'] 		  ? "'".$obra['covnumero']."'" 		  			 : 'null';
		$obra['covano'] 		  = $obra['covano'] 		  ? "'".$obra['covano']."'" 		  			 : 'null';
		$obra['covobjeto'] 		  = $obra['covobjeto'] 		  ? "'".$obra['covobjeto']."'" 		  			 : 'null';
		$obra['covdetalhamento']  = $obra['covdetalhamento']  ? "'".$obra['covdetalhamento']."'"  			 : 'null';
		$obra['covprocesso'] 	  = $obra['covdetalhamento']  ? "'".$obra['covdetalhamento']."'"  			 : 'null';
		$obra['covvlrconcedente'] = $obra['covvlrconcedente'] ? $this->MoedaToBd($obra['covvlrconcedente'])  : 'null';
		$obra['covvlrconvenente'] = $obra['covvlrconvenente'] ? $this->MoedaToBd($obra['covvlrconvenente'])  : 'null';
		$obra['covvalor'] 		  = $obra['covvalor'] 		  ? $this->MoedaToBd($obra['covvalor']) 		 : 'null';
		$obra['covdtinicio'] 	  = $obra['covdtinicio'] 	  ? "'".$obra['covdtinicio']."'" 	  			 : 'null';
		$obra['covdtfinal'] 	  = $obra['covdtfinal'] 	  ? "'".$obra['covdtfinal']."'" 	  			 : 'null';
		
		if($obra['covid']!=''){
			
			$sql = "UPDATE obras.conveniosobra SET
						covnumero 		   = ".$obra['covnumero'].",
						covano  		   = ".$obra['covano'].",
						covobjeto   	   = ".$obra['covobjeto'].",
						covdetalhamento    = ".$obra['covdetalhamento'].",
						covprocesso  	   = ".$obra['covdetalhamento'].",
						covvlrconcedente   = ".$obra['covvlrconcedente'].",
						covvlrconvenente   = ".$obra['covvlrconvenente'].",
						covvalor  		   = ".$obra['covvalor'].",
						covdtinicio  	   = ".$obra['covdtinicio'].",
						covdtfinal         = ".$obra['covdtfinal']."
					WHERE
						covid = ".$obra['covid']."
					RETURNING
						covid";
		}else{
			$sql = "INSERT INTO obras.conveniosobra (
						covnumero , covano, covobjeto, covdetalhamento, covprocesso, covvlrconcedente,
						covvlrconvenente, covvalor, covdtinicio, covdtfinal, covstatus)
					VALUES(
						".$obra['covnumero'].", ".$obra['covano'].", ".$obra['covobjeto'].", ".$obra['covdetalhamento'].", ".$obra['covdetalhamento'].",
						".$obra['covvlrconcedente'].", ".$obra['covvlrconvenente'].", ".$obra['covvalor'].", ".$obra['covdtinicio'].", ".$obra['covdtfinal'].",
						'A'
						)
					RETURNING
						covid";
		}
		
		return $this->simec->pegaUm($sql);
	}

	public function CadastrarContratacaoObras($obra){
		
		// limitando o campo a 300 caracteres
		$obra['frrobsrecproprio'] = substr($obra['frrobsrecproprio'], 0, 300);
		
		// verifica a sessï¿½o da obra
		obras_verifica_sessao();

		$insert_dados = array();
		$flcid_tela = array();
		$flcid_banco = array();
		$sql_insert=array();


		// Insere os dados da tabela formarepasserecursos
		$arrayformarepasserecursos = array('frpid',
										   'obrid',
										   'covid',
										   'frrconventbenef',
										   'frrconvnum',
										   'frrconvobjeto',
										   'frrconvvlr',
										   'frrconvvlrconcedente',
										   'frrconvvlrconcenente',
										   'frrdescinstituicao',
										   'frrdescnumport',
										   'frrdescobjeto',
										   'frrdescvlr',
										   'frrdescdtviginicio',
										   'frrdescdtvigfinal',
										   'frrobsrecproprio');
		$campos = "";
		$valores = "";
		$camposSet = "";

		$obra['frrdescvlr'] = $this->MoedaToBd($obra['frrdescvlr']);
		$obra['frrconvvlr'] = $this->MoedaToBd($obra['frrconvvlr']);
		$obra['frrconvvlrconcedente'] = $this->MoedaToBd($obra['frrconvvlrconcedente']);
		$obra['frrconvvlrconcenente'] = $this->MoedaToBd($obra['frrconvvlrconcenente']);

		if($obra["frpid"] == "2"){ //Convï¿½nio		
			$obra['covid'] = $this->atualizaConv($obra);
//			ver($obra['covid'],d);
		}
	
		if($obra["frpid"] == "7"){
			$obra["covnumero"]			= $obra["covnumero2"];
			$obra["covano"]				= $obra["covano2"];
			$obra["covobjeto"]			= $obra["covobjeto2"];
			$obra["no_covobjeto"]		= $obra["no_covobjeto2"];
			$obra["covdetalhamento"]	= $obra["covdetalhamento2"];
			$obra["no_covdetalhamento"] = $obra["no_covdetalhamento2"];
			$obra["covprocesso"]		= $obra["covprocesso2"];
			$obra["covvlrconcedente"]	= $obra["covvlrconcedente2"];
			$obra["covvlrconvenente"]	= $obra["covvlrconvenente2"];
			$obra["covvalor"]			= $obra["covvalor2"];
			$obra["covdtinicio"] 		= $obra["covdtinicio2"];
			$obra["covdtfinal"] 		= $obra["covdtfinal2"];
			
			$obra['covid'] = $this->atualizaConv($obra);
		}

		if($obra["frpid"] == "3"){ //Descentralizaï¿½ï¿½o
			$obra['frrconventbenef'] = "";
			$obra['frrconvnum'] = "";
			$obra['frrconvobjeto'] = "";
			$obra['frrconvvlr'] = "";
			$obra['frrconvvlrconcedente'] = "";
			$obra['frrconvvlrconcenente'] = "";
			$obra['total'] = "";
			$obra['frrdescdtviginicio'] = "";
			$obra['frrdescdtvigfinal'] = "";
			$obra['frrobsrecproprio'] = "";
		}

		if($obra["frpid"] == "4"){//Recurso Prï¿½prio
			$obra['frrconventbenef'] = "";
			$obra['frrconvnum'] = "";
			$obra['frrconvobjeto'] = "";
			$obra['frrconvvlr'] = "";
			$obra['frrconvvlrconcedente'] = "";
			$obra['frrconvvlrconcenente'] = "";
			$obra['total'] = "";
			$obra['frrdescdtviginicio'] = "";
			$obra['frrdescdtvigfinal'] = "";
			$obra['frrdescinstituicao'] = "";
			$obra['frrdescnumport'] = "";
			$obra['frrdescobjeto'] = "";
			$obra['frrdescvlr'] = "";
		}

		foreach($arrayformarepasserecursos as $key){
			//if($obra[$key]){
			if($obra[$key] == ""){
				$camposSet .= " ".$key." = NULL, ";
				$campos  .="".$key.",";
				$valores .=" NULL,";
			}else{
				$camposSet .= " ".$key." = '".$obra[$key]."', ";
				$campos  .="".$key.",";
				$valores .="'".$obra[$key]."',";
			}
			//}
		}

		$campos  .= "frrstatus, frrdtinclusao";
		$valores .= "'A', now()";
		$camposSet .= " frrstatus = 'A', frrdtinclusao = now() ";

		if($obra['frrid']){
			$sql = "UPDATE obras.formarepasserecursos SET ".$camposSet." WHERE frrid = ".$obra['frrid']."";
		}else{
			$sql = "INSERT INTO obras.formarepasserecursos (".$campos.") VALUES (".$valores.")";
		}
		
		$this->simec->executar($sql);
	  
		// Atribui valores nulos aos campos em branco e coloca aspas
		//$tobraid			  = !empty($obra["tobraid"])      ? $obra["tobraid"] 									: 'NULL';
		$obrdtinicio 		  = !empty($obra["obrdtinicio"])  ? "'" . formata_data_sql($obra["obrdtinicio"])  . "'" : 'NULL';
		$obrdttermino 		  = !empty($obra["obrdttermino"]) ? "'" . formata_data_sql($obra["obrdttermino"]) . "'" : 'NULL';

		$obrpercbdi 		  	   = $obra["obrpercbdi"] 		  		? $this->MoedaToBd($obra["obrpercbdi"])		  		   : 'NULL';
		$obrcustocontrato 		   = $obra["obrcustocontrato"] 	 		? $this->MoedaToBd($obra["obrcustocontrato"]) 		   : 'NULL';
		$obrqtdconstruida 		   = $obra["obrqtdconstruida"]  		? $this->MoedaToBd($obra["obrqtdconstruida"])  		   : 'NULL';
		$obrcustounitqtdconstruida = $obra["obrcustounitqtdconstruida"] ? $this->MoedaToBd($obra["obrcustounitqtdconstruida"]) : 'NULL';
			
		$entidempresa = $obra["entidempresa"] ? "'" . $obra["entidempresa"]. "'" : 'NULL';
	  
		$dtterminocontrato		 = $obra["dtterminocontrato"]		? "'" . formata_data_sql($obra["dtterminocontrato"]) . "'" 		 : 'NULL';
		$obrdtassinaturacontrato = $obra["obrdtassinaturacontrato"] ? "'" . formata_data_sql($obra["obrdtassinaturacontrato"]) . "'" : 'NULL';
		$obrdtordemservico 	 	 = $obra["obrdtordemservico"] 	  	? "'" . formata_data_sql($obra["obrdtordemservico"]) . "'" 	  	 : 'NULL';
		
		$obrprazovigencia = $obra["obrprazovigencia"] ? $obra["obrprazovigencia"] : 'NULL';
		$obrprazoexec 	  = $obra["obrprazoexec"] 	  ? $obra["obrprazoexec"] 	  : 'NULL';
		
		// Insere os dados na tabela obrainfraestrutura
		$sql = "UPDATE
					obras.obrainfraestrutura 
				SET
					obrdtinicio 			  = {$obrdtinicio},
					obrdttermino 			  = {$obrdttermino},
					obrpercbdi 				  = {$obrpercbdi},
					obrcustocontrato 		  = {$obrcustocontrato},
					obrqtdconstruida 		  = {$obrqtdconstruida},
					umdidobraconstruida 	  = {$this->MoedaToBd($obra["umdidobraconstruida"])},
					obrcustounitqtdconstruida = {$obrcustounitqtdconstruida},
					entidempresaconstrutora   = {$entidempresa},
					dtterminocontrato		  = {$dtterminocontrato},
					obrdtassinaturacontrato	  = {$obrdtassinaturacontrato},
					obrdtordemservico		  = {$obrdtordemservico},
					obrprazovigencia		  = {$obrprazovigencia},
					obrprazoexec			  = {$obrprazoexec}		
				WHERE
					obrid = {$_SESSION["obra"]["obrid"]}";
		
		$this->simec->executar($sql);

		$this->simec->commit();

		$_REQUEST["acao"] = "A";
		$this->simec->sucesso("principal/contratacao_da_obra");

	}

	public function CadastrarVistoria($obra){

		if ( $obra["stoid"] != 5 && $obra["stoid"] != 4 ){
			
			$boFoto = false;
			
			foreach( $obra as $chave=>$valor ){
				$pos = strpos(str_to_upper($chave), "IMAGEBOX");
				if ( $pos === false ){
					continue;
				}else{
					$boFoto = true;
				}	
			} 
			
			if(!$boFoto && !possuiPerfil( array(PERFIL_SUPERVISORMEC, PERFIL_ADMINISTRADOR, PERFIL_SUPERVISORUNIDADE, PERFIL_EMPRESA) ) ){
				echo '<script>'
					.'	alert("Para cadastrar uma vistoria, é necessário anexar ao menos uma foto!");'
					.'	history.back(-1);'
					.'</script>';
				die;
			}
			
		}
		
		if(strlen($obra["supobs"]) > 5000){
			
			echo '<script>'
				.'	alert("O Campo Observação da Vistoria deve ter no máximo 5000 caracteres!");'
				.'	history.back(-1);'
				.'</script>';
			die;
			
		}
		
		$obra["supobs"] = substr($obra["supobs"], 0,5000);
		
		if($obra["percsupatual"] < 95.00 && $obra["stoid"] == 3 && !possuiPerfil( array(PERFIL_SUPERVISORMEC, PERFIL_ADMINISTRADOR, PERFIL_SUPERUSUARIO) ) ){
		//if($obra["percsupatual"] < 100.00 && $obra["stoid"] == 3){	
			echo '<script>'
				.'	alert("A situação Concluída pode ser inserida apenas com 100% da obra executada!");'
				.'	history.back(-1);'
				.'</script>';
			die;
			
		}
		
		// verifica a sessï¿½o da obra
		obras_verifica_sessao();

		foreach($obra as $campo=>$valor){
			if(!is_array($valor)) {
				if($valor == "") {
					$obra[$campo] = NULL;
				} else {
					$obra[$campo] = pg_escape_string(trim($valor));
				}
			}
		}

		foreach($obra as $nome=>$valor) {
			if(is_array($valor)) {
				if(preg_match("/^item/",$nome)){
					$item = $valor;
				}
			}
		}

		$obra["qtdsupervisao"] = (int)$obra["qtdsupervisao"];

		if($obra["qtdsupervisao"] > 0) {
			$valor_antigo = array();
			for($k = 0;$k < count($item);$k++){
				$sql = "SELECT
							sup.supvlrinfsupervisor,
							ico.icopercsobreobra
						FROM
							obras.supervisaoitenscomposicao sup	
						INNER JOIN
							obras.supervisao s ON s.obrid = ".$obra["obrid"]." AND 
							s.supdtinclusao = (SELECT max(ss.supdtinclusao) from obras.supervisao ss where ss.obrid = ".$obra["obrid"].")
						INNER JOIN
							obras.itenscomposicaoobra ico ON ico.icoid = ".$item[$k]."
						WHERE
							sup.supvid = s.supvid and sup.icoid = ".$item[$k];

				$valor_antigo[$k] = $this->simec->carregar($sql);
				$valor_antigo[$k] = (((int)$valor_antigo[$k][0]["supvlrinfsupervisor"] * (int)$valor_antigo[$k][0]["icopercsobreobra"]) / 100);

				if($valor_antigo[$k] == NULL)
				$valor_antigo[$k] = 0;
			}
		}

		$entidvistoriador 	   = !empty( $obra['entidvistoriador'] ) ? $obra['entidvistoriador'] : 'null';
		$supprojespecificacoes = !empty( $obra['supprojespecificacoes'] ) ? "'" . $obra['supprojespecificacoes'] . "'" : 'null';
		$supplacaobra 		   = !empty( $obra['supplacaobra'] ) ? "'" . $obra['supplacaobra'] . "'" : 'null';
		$supplacalocalterreno  = !empty( $obra['supplacalocalterreno'] ) ? "'" . $obra['supplacalocalterreno'] . "'" : 'null';
		$supvalidadealvara     = !empty( $obra['supvalidadealvara'] ) ? "'" . $obra['supvalidadealvara'] . "'" : 'null';
		$qlbid     			   = !empty( $obra['qlbid'] ) ? "'" . $obra['qlbid'] . "'" : 'null';
		$dcnid     			   = !empty( $obra['dcnid'] ) ? "'" . $obra['dcnid'] . "'" : 'null';
		$supdiarioobra     	   = !empty( $obra['supdiarioobra'] ) ? "'" . $obra['supdiarioobra'] . "'" : 'null';
//		$tpsid 		    	   = !empty( $obra['tpsid'] ) ? "'" . $obra['tpsid'] . "'" : 'null';
		
		$rsuid 		   = '4';
		if( possuiPerfil(PERFIL_SUPERVISORUNIDADE) ){
			$suprealizacao = 'Instituição';
			$rsuid 		   = '1';
		}elseif( possuiPerfil(PERFIL_EMPRESA) ){
			$suprealizacao = 'Empresa';
			$rsuid 		   = '3';
		}else{
			$suprealizacao = 'MEC';
			$rsuid 		   = '2';
		}
		
		// Insere os dados da supervisï¿½o
		$sql = "INSERT INTO
					obras.supervisao ( usucpf, 
					 				   supvdt,
					 				   supvistoriador, 
					 				   stoid, 
					 				   supprojespecificacoes, 
					 				   supplacaobra, 
					 				   supplacalocalterreno, 
					 				   supvalidadealvara, 
					 				   qlbid, 
					 				   dcnid, 
					 				   supobs, 
					 				   supparecerorgao, 
					 				   obrid, 
					 				   supstatus, 
					 				   supdiarioobra,
					 				   suprealizacao,
					 				   rsuid
					 				   ) 
				VALUES ( '{$_SESSION["usucpf"]}', 
						 '".formata_data_sql($obra["supvdt"])."',
						 {$entidvistoriador},
						 '{$obra["stoid"]}', 
						 {$supprojespecificacoes},
						 {$supplacaobra},
						 {$supplacalocalterreno},
						 {$supvalidadealvara},
						 {$qlbid},
						 {$dcnid},
						 '{$obra["supobs"]}', 
						 '{$obra["supparecerorgao"]}', 
						 '{$_SESSION["obra"]["obrid"]}', 
						 'A', 
						 {$supdiarioobra},
						 '{$suprealizacao}',
						 {$rsuid}
						 ) returning supvid"; 
						 
						 $supvid = $this->simec->pegaUm($sql);

						 /*
						  $ultimo_registro = $this->simec->executar("select last_value from obras.supervisao_supvid_seq");
						  $ultimo_registro = pg_fetch_assoc($ultimo_registro);
						  $supvid = $ultimo_registro["last_value"];
						  */

						 for($k = 0;$k < count($item);$k++){
						 	$percSupervisao = $this->MoedaToBd($obra["supvlrinfsuperivisor_".$item[$k]]);
						 	if ($percSupervisao == ""){
						 		$percSupervisao = 0.00;
						 	}

						 	$supItemExec = $this->MoedaToBd($obra["execanterior_".$item[$k]]);
						 	$percSobreObra = $this->simec->pegaUm("SELECT
															icopercsobreobra
													   FROM 
													   		obras.itenscomposicaoobra 
													   WHERE 
													   		icoid = " . $item[$k]);

						 	$supItemExecSobreObra = number_format($obra["execanteriorsobreobra_" . $item[$k]], 6); // favor nï¿½o mudar o nï¿½mero de casas decimais!
						 	$valorPercExecutado = (( (float)$percSupervisao * $percSobreObra ) / 100);

						 	$query = $percSupervisao . "," . "{$supvid}" . "," . "{$item[$k]}" . "," . "{$supItemExec}" . "," . $supItemExecSobreObra;
						 	$sql = "INSERT INTO
							obras.supervisaoitenscomposicao (supvlrinfsupervisor, supvid, icoid, supvlritemexecanterior, supvlritemsobreobraexecanterior) 
						VALUES (" . $query. ");";

						 	$this->simec->executar($sql);

						 	$sql = "UPDATE
							obras.itenscomposicaoobra
						SET
							icopercexecutado = {$valorPercExecutado}
						WHERE icoid = {$item[$k]}";

						 	$this->simec->executar($sql);

						 	$total += (float) $valorPercExecutado;

						 }

//						 if ( !empty($total) ){
//								if($rsuid == '3'){
//									$atualizaPercentualParaEmpresa = ",obrsuppercexec = {$total}";
//								}
//						 	$sql = "";
//						 	$sql = "UPDATE
//										obras.obrainfraestrutura
//									SET
//										obrpercexec = {$total}
//						 				{$atualizaPercentualParaEmpresa}	
//									WHERE
//										obrid = {$_SESSION["obra"]["obrid"]}";
//												
//						 	$this->simec->executar($sql);
//						 		
//						 }

						 $obra["stoid"] = !empty($obra["stoid"]) ? $obra["stoid"] : obras_pega_situacao_vistoria($_SESSION["obra"]["obrid"]);
						 
						 if ( !empty($obra["stoid"]) ){
							 if($rsuid == '3'){
								$atualizaSituacaoParaEmpresa = ",stoidsupemp = {$obra["stoid"]}";
							 }
						  	$sql = "";
						 	$sql = "UPDATE
										obras.obrainfraestrutura
									SET
										stoid = {$obra["stoid"]},
										obrdtvistoria = now()
										{$atualizaSituacaoParaEmpresa}
									WHERE
										obrid = {$_SESSION["obra"]["obrid"]}";
										 		
						 	$this->simec->executar($sql);
						 }

						 //Insere as fotos da supervisï¿½o e galeria
						 atualizarFotosVistoria($supvid);
						 
						 // Insere o dado da tabela supervisao
						 $ordem = 0;
						 foreach($obra as $nome=>$valor){
						 	$imagens = strpos($nome,"imageBox");
						 	if($imagens === 0){
						 		$imagem = str_replace("'","",$valor);
						 		$imagem = trim($imagem);
						 		$container = $nome;
						 		if(file_exists("../../arquivos/obras/imgs_tmp/".$imagem)){
						 			$imagem = str_replace("___","/",$imagem);
						 			$part1file = explode("__temp__", $imagem);
						 			$part2file = explode("__extension__", $part1file[0]);
						 			$part2file[0] = md5_decrypt($part2file[0]);
						 			$part2file[1] = md5_decrypt($part2file[1]);
						 			$nomearquivo = explode(".", $part2file[0]);
						 			if(is_readable("../../arquivos/obras/imgs_tmp/".$imagem.".d")) {
						 				$descricao = file_get_contents("../../arquivos/obras/imgs_tmp/".$imagem.".d");
						 			}
						 			//Insere o registro da imagem na tabela public.arquivo
						 			$sql = "INSERT INTO public.arquivo(arqnome,arqdescricao,arqextensao,arqtipo,arqdata,arqhora,usucpf,sisid)
						values('". substr($nomearquivo[0],0,255) ."','". substr($descricao,0,255) ."','".$nomearquivo[(count($nomearquivo)-1)]."','". $part2file[1] ."','".date('Y-m-d')."','".date('H:i:s')."','".$_SESSION["usucpf"]."',15) RETURNING arqid;";
						 			$arqid = $this->simec->pegaUm($sql);
						 			if(!is_dir('../../arquivos/obras/'.floor($arqid/1000))) {
						 				mkdir(APPRAIZ.'/arquivos/obras/'.floor($arqid/1000), 0777);
						 			}
						 			if(@copy("../../arquivos/obras/imgs_tmp/".$imagem,"../../arquivos/obras/".floor($arqid/1000)."/".$arqid)){
						 				unlink("../../arquivos/obras/imgs_tmp/".$imagem);
						 				$_sql = "INSERT INTO obras.fotos(arqid,obrid,supvid,fotdsc,fotbox,fotordem)
							values({$arqid},{$_SESSION['obra']["obrid"]},{$supvid},'{$imagem}','imageBox{$ordem}',{$ordem});";
						 				$this->simec->executar($_sql);
						 			}else{
						 				echo "Falha ao copiar o arquivo";
						 			}
						 				
						 		}else{
						 			echo "Arquivo não existe na pasta.";
						 		}
						 		$ordem++;
						 	}
						 }

						 // se for elaboraï¿½ï¿½o de projetos
						 if ($obra['stoid'] == '4'){
						 
						 	$sql = "";
						 	$sql = "UPDATE 
						 				obras.obrainfraestrutura
						 			SET
						 				obrlincambiental  = '{$obra['obrlincambiental']}',
						 				obraprovpatrhist  = '{$obra['obraprovpatrhist']}',
						 				obrdtprevprojetos = ".(($obra["obrdtprevprojetos"] && $obra["obrdtprevprojetos"] != "null")?"'".formata_data_sql($obra["obrdtprevprojetos"])."'":"NULL")."
						 			WHERE
						 				obrid = {$_SESSION["obra"]["obrid"]}";
						 	
						 	$this->simec->executar($sql);
						 	
						 // se a situaï¿½ï¿½o for paralizada, cria o histï¿½rico
						 }else if ($obra['stoid'] == '2'){
						 	/*
						 	$sql = "";
						 	$sql = "SELECT hprid 
						 			FROM obras.historicoparalisacao 
						 			WHERE obrid = {$_SESSION['obra']['obrid']} AND
						 				  hprdtstatus = 'A'";
						 	
						 	$existe_historico = $this->simec->pegaUm( $sql );
						 	
						 	if ( $existe_historico ){

						 		echo "<script>
						 				alert('Jï¿½ existe uma paralisaï¿½ï¿½o em aberto para esta obra!');
						 				history.back(-1);
						 			  </script>";
						 		die;
						 		
						 	}
						 	*/
						 	$sql = "";
						 	$sql = "INSERT INTO 
						 				obras.historicoparalisacao (tplid, obrid, supvidparalisacao, hprobs, 
						 											hprdtstatus, hprdtinclusao)
						 			VALUES
						 				({$obra['tplid']}, {$_SESSION['obra']['obrid']}, {$supvid}, 
						 				 '{$obra['hprobs1']}', 'A', now())";
						 	
							$this->simec->executar($sql);
						 				 
						 }else{
						 	
						 	$sql = "";
						 	$sql = "SELECT 
						 				hprid 
						 			FROM 
						 				obras.historicoparalisacao
						 			WHERE 
						 				obrid = {$_SESSION['obra']['obrid']} AND
						 				supvidparalisacao is not null AND
						 				supvidliberacao is null AND
						 				hprdtstatus = 'A'";
						 	
						 	$hprid = $this->simec->pegaUm($sql);
						 	
						 	if ( !empty($hprid) ){
						 		
						 		$sql = "";
						 		$sql = "UPDATE
						 					obras.historicoparalisacao
						 				SET
						 					supvidliberacao = {$supvid},
						 					hprdtliberacao = now(),
						 					hprdtstatus = 'I'
						 				WHERE
						 					hprid = {$hprid}";
						 		
						 		$this->simec->executar($sql);
						 		
						 	}
						 	
						 }

						 $this->simec->commit();
						 $this->simec->close();
						 $_REQUEST["acao"] = "A";
						 $this->simec->sucesso("principal/inserir_vistoria","&supvid=".$supvid);
	}

	/**
	 * Verifica se existe vistorias com data maior do que a da que esta tentando excluir
	 *
	 * @param integer $supvid
	 * @return bool Existe ou nï¿½o vistoria com data posterior
	 */
	public function VerificaExistenciaVistorias($supvid){

		$sql = "SELECT
					supdtinclusao 
				FROM 
					obras.supervisao 
				WHERE 
					supvid = {$supvid} AND 
					supstatus = 'A'";

		$datavistoriaatual = $this->simec->pegaUm($sql);

		if (!empty($datavistoriaatual)){
			$sql = "SELECT
					count(supvid) as total 
				FROM 
					obras.supervisao 
				WHERE 
					supdtinclusao > '{$datavistoriaatual}' AND 
					supstatus = 'A' AND 
					obrid = {$_SESSION['obra']['obrid']}";

			$vistoria = $this->simec->pegaUm($sql);
		}
		if( $vistoria > 0 ){
			return false;
		}else{
			return true;
		}

	}

	/**
	 * Verifica se existe vistorias para a obra
	 *
	 * @param integer $obraid Obra a ser pesquisada
	 * @return bool Existe ou nï¿½o vistoria para a obra
	 */
	public function existenciaVistoriaParaObra( $obraid = null ){
		if ( empty( $obraid ) ){
			$obraid = $this->getObraId();
		}

		$traid = pegaObUltimoAditivo('traid');
		
		$sql = "SELECT
					count(s.supvid) as total 
				FROM 
					obras.supervisao s
				JOIN obras.supervisaoitenscomposicao si ON si.supvid = s.supvid	
				JOIN obras.itenscomposicaoobra ic ON ic.icoid = si.icoid
								     				 AND ic.icostatus = 'A'
								     				 AND ic.traid " . ($traid ? " = {$traid} " : " IS NULL ") . "
				WHERE 
					supstatus = 'A' AND 
					s.obrid = " . $obraid;
					
		return (bool) $this->simec->pegaUm($sql);
	}


	/**
	 * Regras de negï¿½cio para ver se pode excluir a vistï¿½ria
	 *
	 * @param integer $supvid - id da vistoria
	 * @return bool || string Mensagem de erro caso exista
	 */
	public function antesExcluirVistoria( $supvid ){

		////// VERIFICA SE A VISTï¿½RIA FOI FEITA PELO USUï¿½RIO LOGADO /////
		$sql = "SELECT count(usucpf) as total FROM obras.supervisao
			WHERE 
				usucpf = '" . $_SESSION['usucpf'] . "' AND 
				supstatus = 'A' AND supvid = " . $supvid;

		$boVistoriaDoUsuario = (bool) $this->simec->pegaUm( $sql );
		if ( !$boVistoriaDoUsuario && !possuiPerfil(PERFIL_ADMINISTRADOR) ) return 'Você não pode excluir essa vistoria porque ela não foi feita por você';

		return true;
	}

	public function AtualizarVistoria($obra){

		// verifica a sessï¿½o da obra
		obras_verifica_sessao();
		
	if ( $obra["stoid"] != 5 && $obra["stoid"] != 4 ){
		$boFoto = false;
			foreach( $obra as $chave=>$valor ){
				$pos = strpos(str_to_upper($chave), "IMAGEBOX");
				if ( $pos === false ){
					continue;
				}else{
					$boFoto = true;
				}	
			}
						
			if( !$boFoto && !possuiPerfil( array(PERFIL_SUPERVISORMEC, PERFIL_ADMINISTRADOR, PERFIL_SUPERVISORUNIDADE, PERFIL_EMPRESA) ) ){
				echo '<script>'
					.'	alert("Para cadastrar uma vistoria, é necessário anexar ao menos uma foto!");'
					.'	history.back(-1);'
					.'</script>';
				die;
			}
			
		}
		if($obra["percsupatual"] < 95.00 && $obra["stoid"] == 3 && !possuiPerfil( array(PERFIL_SUPERVISORMEC, PERFIL_ADMINISTRADOR, PERFIL_SUPERUSUARIO) ) ){
//		//if($obra["percsupatual"] < 100.00 && $obra["stoid"] == 3){	
			echo '<script>'
				.'	alert("A situação Concluída pode ser inserida apenas com 100% da obra executada!");'
				.'	history.back(-1);'
				.'</script>';
			die;
		}
		
		
		foreach($obra as $campo=>$valor){
			if(!is_array($valor)){
				if($valor == "" ){
					$obra[$campo] = NULL;
				} else {
					$obra[$campo] = trim($valor);
				}
			}
		}

		$entidvistoriador 	   = !empty( $obra['entidvistoriador'] ) ? $obra['entidvistoriador'] : 'null';
		$supprojespecificacoes = !empty( $obra['supprojespecificacoes'] ) ? "'" . $obra['supprojespecificacoes'] . "'" : 'null';
		$supplacaobra 		   = !empty( $obra['supplacaobra'] ) ? "'" . $obra['supplacaobra'] . "'" : 'null';
		$supplacalocalterreno  = !empty( $obra['supplacalocalterreno'] ) ? "'" . $obra['supplacalocalterreno'] . "'" : 'null';
		$supvalidadealvara     = !empty( $obra['supvalidadealvara'] ) ? "'" . $obra['supvalidadealvara'] . "'" : 'null';
		$qlbid     			   = !empty( $obra['qlbid'] ) ? "'" . $obra['qlbid'] . "'" : 'null';
		$dcnid     			   = !empty( $obra['dcnid'] ) ? "'" . $obra['dcnid'] . "'" : 'null';
		$supdiarioobra     	   = !empty( $obra['supdiarioobra'] ) ? "'" . $obra['supdiarioobra'] . "'" : 'null';
//		$tpsid 		    	   = !empty( $obra['tpsid'] ) ? "'" . $obra['tpsid'] . "'" : 'null';

		$rsuid         = '4';
		if( possuiPerfil(PERFIL_SUPERVISORUNIDADE) ){
			$suprealizacao = 'Instituição';
			$rsuid         = '1';
		}elseif( possuiPerfil(PERFIL_EMPRESA) ){
			$suprealizacao = 'Empresa';
			$rsuid         = '3';
		}else{
			$suprealizacao = 'MEC';
			$rsuid         = '2';
		}
		
		$obra["supobs"] = substr($obra["supobs"], 0,5000);
		
		$sql = ("
			UPDATE 
				obras.supervisao
			SET 
				supvdt = '" . formata_data_sql($obra["supvdt"]) . "',
				supvistoriador = " . $entidvistoriador . ", 
				stoid = '{$obra["stoid"]}', 
				supprojespecificacoes = {$supprojespecificacoes}, 
				supplacaobra = {$supplacaobra}, 
				supplacalocalterreno = {$supplacalocalterreno},
				supvalidadealvara = {$supvalidadealvara}, 
				qlbid = {$qlbid}, 
				dcnid = {$dcnid}, 
				supobs = '{$obra["supobs"]}', 
				supparecerorgao = '{$obra["supparecerorgao"]}',
				supdiarioobra = {$supdiarioobra},
				suprealizacao = '{$suprealizacao}',
				rsuid = {$rsuid}
			WHERE 
				obrid = '{$_SESSION["obra"]["obrid"]}' AND 
				supvid = '{$_SESSION["supvid"]}'");
	
		$this->simec->executar($sql);

		foreach($obra as $nome=>$valor){
			if(is_array($valor)){
				if(preg_match("/^item/",$nome))
				$item = $valor;
			}
		}
			
		for($k = 0;$k < count($item);$k++){

			$percSupervisao = $this->MoedaToBd($obra["supvlrinfsuperivisor_".$item[$k]]);
			$supItemExec = $this->MoedaToBd($obra["execanterior_".$item[$k]]);
			$supItemExecSobreObra = number_format($obra["execanteriorsobreobra_".$item[$k]], 6); // favor nï¿½o mudar o nï¿½mero de casas decimais!

			$percSobreObra = $this->simec->pegaUm("SELECT icopercsobreobra FROM obras.itenscomposicaoobra where icoid=".$item[$k]);

			if($percSupervisao <= 100.00 ) {
					
				$sql = "SELECT
								sup.supvlrinfsupervisor,
								ico.icopercsobreobra 
							FROM 
								obras.supervisaoitenscomposicao sup
							INNER JOIN
								obras.itenscomposicaoobra ico ON ico.icoid = ".$item[$k]."
							WHERE 
								sup.supvid = ".$obra["supvid"]." AND sup.icoid = ".$item[$k];
					
				$valor_antigo = $this->simec->carregar($sql);
				$valor_antigo = (((int)$valor_antigo[0]["supvlrinfsupervisor"] * (int)$valor_antigo[0]["icopercsobreobra"]) / 100);

				if($valor_antigo == NULL)
				$valor_antigo = 0;

				$itens_supervisao = $this->simec->carregar("SELECT * FROM obras.supervisaoitenscomposicao WHERE icoid = ".$item[$k]." AND supvid = " . $obra["supvid"]);
					
				if ($itens_supervisao){
					$sql = "UPDATE
									obras.supervisaoitenscomposicao 
								SET 
									supvlrinfsupervisor = ".$percSupervisao.",
									supvlritemexecanterior = ".$supItemExec.",
									supvlritemsobreobraexecanterior = ".$supItemExecSobreObra."
								WHERE 
									supvid = ".$obra["supvid"]." AND icoid = ".$item[$k];
				} else{
					$sql = "INSERT INTO
									obras.supervisaoitenscomposicao (supvlrinfsupervisor, 
																	 supvlritemexecanterior, 
																	 supvlritemsobreobraexecanterior,
																	 supvid,
																	 icoid)
								VALUES
									(".$percSupervisao.", 
									 ".$supItemExec.", 
									 ".$supItemExecSobreObra.", 
									 ".$obra["supvid"].", 
									 ".$item[$k].")";
				}

				$this->simec->executar($sql);
					 
				$valorPercExecutado = (( (float)$percSupervisao * $percSobreObra ) / 100);
			
				$sql = "
						UPDATE 
							obras.itenscomposicaoobra
						SET
							icopercexecutado = $valorPercExecutado 
						
						WHERE icoid = {$item[$k]}";

				$this->simec->executar($sql);
					
				$total += (float) $valorPercExecutado;
					
			}else{
				echo "
						<script>
							alert('Valor não pode ultrapassar 100 % do item de composição');
						</script>";	
			}

		}

//		$percsupatual = str_replace(",",".",$_REQUEST['percsupatual']);
//		if ( !empty($percsupatual) ){
//			if($rsuid == '3'){
//				$atualizaPercentualParaEmpresa = ",obrsuppercexec = {$percsupatual}";
//			}	
//			$sql = "";
//			$sql = "UPDATE
//						obras.obrainfraestrutura
//					SET
//						obrpercexec = {$percsupatual}
//						{$atualizaPercentualParaEmpresa}
//					WHERE
//						obrid = {$_SESSION["obra"]["obrid"]}";
//			
//			$this->simec->executar($sql);
//		}
//		dbg($sql,1);

		$ultimadata = $this->simec->pegaUm("SELECT to_char(max(supdtinclusao), 'YYYY-MM-DD')
											FROM obras.supervisao 
											WHERE obrid = {$_SESSION["obra"]["obrid"]} AND supstatus = 'A'");

		$ultimadata = !empty($ultimadata) ? $ultimadata : 'now()';

		$obra["stoid"] = !empty($obra["stoid"]) ? $obra["stoid"] : obras_pega_situacao_vistoria($_SESSION["obra"]["obrid"]);
		
		if ( !empty($obra["stoid"]) ){
			if($rsuid == '3'){
				$atualizaSituacaoParaEmpresa = ",stoidsupemp = {$obra["stoid"]}";
			 }
			$sql = "";
			$sql = "UPDATE
						obras.obrainfraestrutura
					SET
						stoid = {$obra["stoid"]},
						obrdtvistoria = now()
						{$atualizaSituacaoParaEmpresa}
						-- Segundo o fernando quando atualizar a vistoria também atualiza a data para data atual
						--obrdtvistoria = '{$ultimadata}'
					WHERE
						obrid = {$_SESSION["obra"]["obrid"]}";
			
			$this->simec->executar($sql);
		}

		 // se for elaboraï¿½ï¿½o de projetos
		 if ($obra['stoid'] == '4'){
		 
		 	$sql = "";
		 	$sql = "UPDATE 
		 				obras.obrainfraestrutura
		 			SET
		 				obrlincambiental  = '{$obra['obrlincambiental']}',
		 				obraprovpatrhist  = '{$obra['obraprovpatrhist']}',
		 				obrdtprevprojetos = '".formata_data_sql($obra["obrdtprevprojetos"])."'
		 			WHERE
		 				obrid = {$_SESSION["obra"]["obrid"]}";
		 	
		 	$this->simec->executar($sql);

		 // se a situaï¿½ï¿½o for paralizada, cria o histï¿½rico
		 }else if ($obra['stoid'] == '2'){
		 	
		 	$hprdtliberacao = $this->simec->pegaUm("SELECT 
										 				hprdtliberacao 
										 			from 
										 				obras.historicoparalisacao
										 			WHERE
										 				supvidparalisacao = {$_SESSION["supvid"]}");
		 	
		 	$tplid = $obra['tplid'] ? $obra['tplid'] : 'null';
		 	
		 	$sql = "UPDATE 
		 				obras.historicoparalisacao 
		 			SET
		 				tplid = {$tplid}, 
		 				hprobs = '{$obra['hprobs1']}'
					WHERE
						". (!empty($hprdtliberacao) ? 'supvidliberacao' : 'supvidparalisacao') . " = {$_SESSION["supvid"]}";
		 	
			$this->simec->executar($sql);
		 				 
		 }else{
		 	
		 	$sql = "";
		 	$sql = "UPDATE obras.historicoparalisacao SET hprdtstatus = 'I'
		 	 		WHERE obrid = {$_SESSION['obra']['obrid']}";
		 		
		 	$this->simec->executar($sql);
		 
		 }
		
		//Atualiza fotos
		 atualizarFotosVistoria($_SESSION["supvid"]);
		
		$this->simec->commit();
		$this->simec->close();
		$_REQUEST["acao"] = "A";
		$this->simec->sucesso("principal/inserir_vistoria", "&supvid=" . $_SESSION["supvid"]);

	}


	public function DeletarVistoria($supvid){

		// verifica a sessï¿½o da obra
		obras_verifica_sessao();

		// Executa verificaï¿½ï¿½o para ver se pode excluir a vistï¿½ria de acordo com as regras de negï¿½cio
		$boPodeExcluir = $this->antesExcluirVistoria( $supvid );
		if ( true !== $boPodeExcluir ){
			return $boPodeExcluir;
		}

		$retorno = $this->simec->carregar("SELECT
										   		sic.icoid as id, 
										   		ico.icopercexecutado as executado,
										   		sic.supvlritemsobreobraexecanterior as executadoanterior 
										   FROM 
												obras.supervisaoitenscomposicao sic
										   INNER JOIN
										   		obras.itenscomposicaoobra ico ON ico.icoid = sic.icoid
										   WHERE 
												sic.supvid = ".$supvid);

		for($i=0; $i<count($retorno); $i++) {
			if(!isset($retorno[$i]['executadoanterior'])) {
				$retorno[$i]['executadoanterior'] = 0;
			}
			if(!isset($retorno[$i]['executado'])) {
				$retorno[$i]['executado'] = 0;
			}
			
			if ($retorno[$i]["id"]){
				$sql = $this->simec->executar("
								UPDATE
									obras.itenscomposicaoobra
								SET
									icopercexecutado = " . $retorno[$i]['executadoanterior'] . "
								WHERE
									icoid = ".$retorno[$i]['id']);
			}

			$total += $retorno[$i]['executadoanterior'];
				
		}
				
		if ( !empty($total)){
			$sql = "";
			$sql = "UPDATE
						obras.obrainfraestrutura
					SET
						obrpercexec = {$total}	
					WHERE
						obrid = {$_SESSION["obra"]["obrid"]}";
				
			$this->simec->executar($sql);
		}
		
		// exclui os itens da supervisao
		$this->simec->executar("DELETE FROM obras.supervisaoitenscomposicao WHERE supvid = ".$supvid);

		// deleta as fotos da vistoria
		$this->simec->executar("DELETE FROM obras.fotos WHERE supvid = ".$supvid);

		// desativa a vistoria
		 $desativaVistoria = $this->simec->executar("UPDATE obras.supervisao SET supstatus = 'I' WHERE supvid = ".$supvid);
		 	 
		//Quando desativada a vistoria, a data da ï¿½ltima vistoria serï¿½ inserida na tabela "obras.obrainfraestrutura", no campo "obrdtvistoria". 
		if($desativaVistoria){
			//Seleciona a ï¿½ltima Vistoria, quando desativada a Vistoria mais recente.
		 	$sqlDtinclusao = " SELECT 
		 							supdtinclusao 
	 						   FROM 
	 						   		obras.supervisao 
 						   	   WHERE 
 						   	   		obrid = ".$_SESSION["obra"]["obrid"]." 
 						   	   		AND supstatus = 'A' 
 						   	   		ORDER BY supvid DESC " ;
		 	$dtInclusao[0]['supdtinclusao'] = $this->simec->pegaUm($sqlDtinclusao);
		 	//Atualiza a data da Lista de Obras, quando a vistoria for desativada, com a data da penï¿½ltima vistoria inserida.
		 	if($dtInclusao[0]['supdtinclusao'] != ''){
		 		$sqlAtualizaDtInclusao = " UPDATE 
		 									obras.obrainfraestrutura 
	 								   SET 
	 								   		obrdtvistoria ='".$dtInclusao[0]['supdtinclusao']."' 
 							   		   WHERE 
 							   		   		obrid = {$_SESSION["obra"]["obrid"]} ";
		  	}else{
		 		$sqlAtualizaDtInclusao = " UPDATE 
		 									obras.obrainfraestrutura 
	 								   SET 
	 								   		obrdtvistoria = NULL 
 							   		   WHERE 
 							   		   		obrid = {$_SESSION["obra"]["obrid"]} ";
		 	}
		 	$this->simec->executar($sqlAtualizaDtInclusao);
//		 	ver($desativaVistoria,$supvid,$_SESSION["obra"]["obrid"],$dtInclusao[0]['supdtinclusao'],d);

		 }
		 
		// atualiza a situaï¿½ï¿½o da obra
		$stoid = obras_pega_situacao_vistoria($_SESSION["obra"]["obrid"]);
		
		if($stoid) {
			$sql = "UPDATE 
						obras.obrainfraestrutura SET stoid = {$stoid}
					WHERE 
						obrid = {$_SESSION["obra"]["obrid"]}";	
			
			$this->simec->pegaUm($sql);
		}

		// Verifica se a Vistoria foi Cadastrada por uma Empresa.
		$empresa = verifica_realizado_por_empresa($_SESSION["obra"]["obrid"]);
		
		if($empresa || $stoid){
			
			//Caso a Vistoria for Cadastrada por uma Empresa serï¿½ atribuï¿½da a Situaï¿½ï¿½o da Vistoria. 
			$stoidEmpresa = pega_situacao_obra_empresa($_SESSION["obra"]["obrid"]);
			
			//Caso a Vistoria for Cadastrada por uma Empresa serï¿½ atribuï¿½do o Percentual  Executado da Supervisï¿½o.
			$obrsuppercexecEmpresa = pega_percentual_obra_empresa($_SESSION["obra"]["obrid"]);
				
			if( $obrsuppercexecEmpresa && $stoidEmpresa ){
				$empresaPercentualSituacao = " obrsuppercexec = {$obrsuppercexecEmpresa} ,
												stoidsupemp = {$stoidEmpresa} ";
			}else{
				$empresaPercentualSituacao = " obrsuppercexec = NULL ,
												stoidsupemp = NULL ";	
			}
			$sql = "UPDATE 
						obras.obrainfraestrutura 
						SET {$empresaPercentualSituacao} 
					WHERE 
						obrid = {$_SESSION["obra"]["obrid"]}";	
			
			$this->simec->pegaUm($sql);
		}
		
		
		// verifica se existe histï¿½rico de paralisaï¿½ï¿½o da obra
		$sql = "SELECT hprid 
				FROM obras.historicoparalisacao 
				WHERE obrid = {$_SESSION['obra']['obrid']} AND hprdtstatus = 'A'";
		
		$hprid = $this->simec->pegaUm($sql);
		
		// inavita o histï¿½rico
		if ( !empty( $hprid ) ){
			
			$this->simec->executar("UPDATE obras.historicoparalisacao 
									SET hprdtstatus = 'I' WHERE hprid = {$hprid}");
						
		}
		
		$this->simec->commit();

		$this->simec->sucesso('principal/vistoria');
		return true;
	}

	public function AtualizarFotosVistoria($fotos){
		session_start();
		switch($fotos['tipoatualizacao']) {
			case 'somentedescricao':
				if(is_numeric($fotos['arqid'])) {
					$sql = "UPDATE public.arquivo SET arqdescricao = '". $fotos['arqdescricao'] ."' WHERE arqid = '". $fotos['arqid'] ."'";
					$this->simec->executar($sql);
					$this->simec->commit();
				} else {
					$fp = fopen("../../arquivos/obras/imgs_tmp/".$fotos['arqid'].".d", 'w');
					fwrite($fp, $fotos['arqdescricao']);
					fclose($fp);
				}
				echo "Descrição atualizada com sucesso!";
				break;
					
			default:
				for($k=0; $k < count($fotos["foto"]);$k++){
					if(is_numeric($fotos["foto"][$k])) {
						$csql = "arqid = '". $fotos["foto"][$k] ."'";
					} else {
						$csql = "fotdsc = '". $fotos["foto"][$k] ."'";
					}
					$_sql = "SELECT fotid,fotordem FROM obras.fotos WHERE supvid={$_SESSION["supvid"]} and obrid={$_SESSION["obra"]["obrid"]} AND {$csql}";
					$resultado = $this->simec->pegaLinha($_sql);
					$valor = $resultado;
					if(is_array($valor)){
						if($valor["fotordem"] != $fotos["ordem"][$k]){
							$_sql = "UPDATE obras.fotos SET fotordem = {$fotos["ordem"][$k]}, fotbox='imageBox{$fotos["ordem"][$k]}' WHERE fotid = '{$valor["fotid"]}'";
							$this->simec->executar($_sql);
							$this->simec->commit();
						}
					}else{
						$imagem = $fotos["foto"][$k];
						if(file_exists("../../arquivos/obras/imgs_tmp/".$imagem)){
							$part1file = explode("__temp__", $imagem);
							$part2file = base64_decode($part1file[0]);
							$part2file = explode("__extension__",$part2file);
							$nomearquivo = explode(".", $part2file[0]);
							//Insere o registro da imagem na tabela public.arquivo
							$sql = "INSERT INTO public.arquivo(arqnome,arqextensao,arqtipo,arqdata,arqhora,usucpf,sisid)
								values('". substr($nomearquivo[0],0,255) ."','".$nomearquivo[(count($nomearquivo)-1)]."','". $part2file[1] ."','".date('d/m/Y')."','".date('H:i:s')."','".$_SESSION["usucpf"]."',15) RETURNING arqid;";
							$arqid = $this->simec->pegaUm($sql);
							if(!is_dir('../../arquivos/obras/'.floor($arqid/1000))) {
								mkdir(APPRAIZ.'/arquivos/obras/'.floor($arqid/1000), 0777);
							}
							if(@copy("../../arquivos/obras/imgs_tmp/".$imagem,"../../arquivos/obras/".floor($arqid/1000)."/".$arqid)){
								unlink("../../arquivos/obras/imgs_tmp/".$imagem);
								$ordem = $fotos["ordem"][$k]++;
								$container = $fotos["box"][$k];
								$_sql = "INSERT INTO obras.fotos(arqid,obrid,supvid,fotdsc,fotbox,fotordem)
									values({$arqid},{$_SESSION['obra']["obrid"]},{$_SESSION['supvid']},'{$imagem}','imageBox{$ordem}',{$ordem});";
								$this->simec->executar($_sql);
								$this->simec->commit();
							}else{
								echo "Falha ao copiar o arquivo";
							}
						}else{
							echo "Arquivo não existe na pasta.";
						}
					}
				}
		}
	}

	public function DeletarFotoVistoria($foto){
			
		if(is_numeric($foto["img"])){
			$sql = "SELECT arqid,fotid,fotdsc FROM obras.fotos WHERE arqid='{$foto["img"]}'";
			$res = $this->simec->pegaLinha($sql);
			$result = $res;
				
			if(is_array($result)){
					
				$fotid = $result["fotid"];
				$arqid = $result["arqid"];

				$sql = "DELETE FROM obras.fotos WHERE fotid={$fotid}";
				$this->simec->executar($sql);
				//$this->simec->commit();
				
				$sql = "DELETE FROM obras.arquivosobra WHERE arqid={$arqid}";
				$this->simec->executar($sql);
				
				$sql = "DELETE FROM public.arquivo WHERE arqid={$arqid}";
				$this->simec->executar($sql);
				$this->simec->commit();

				if(file_exists("../../arquivos/obras/".floor($arqid/1000)."/".$arqid)) {
					unlink("../../arquivos/obras/".floor($arqid/1000)."/".$arqid);
					print_r("Foto deletada com sucesso !");
				}else{
					print_r("imagem nao encontrada.");
				}
			}
		}
	}

	public function CadastrarProjetoArquitetonico($dados){

		// verifica a sessï¿½o da obra
		obras_verifica_sessao();
		$dados["fprdtiniciofaseprojeto"]    = formata_data_sql($dados["fprdtiniciofaseprojeto"]);
		$dados["fprdtconclusaofaseprojeto"] = formata_data_sql($dados["fprdtconclusaofaseprojeto"]);
		
		//limpa campos
		if(!$dados['tpaid'] && !$dados['felid'] && !$dados['tfpid']){
			if($dados['fprid']){

				$SQL = "DELETE FROM obras.faselicitacaoprojetos WHERE fprid = ".$dados['fprid'];
				$this->simec->executar($SQL);

				$SQL = "DELETE FROM obras.faseprojeto WHERE fprid = ".$dados['fprid'];
				$this->simec->executar($SQL);
					
				$this->simec->commit();
			}
			$_REQUEST["acao"] = "A";
			$this->simec->sucesso("principal/projeto_arquitetonico");
			exit();
		}
			
		//insere e altera dados
		$campos = array();
		$_where = "";
			
		foreach($dados as $campo=>$valor){

			$search  = preg_match("/^ftp|^fpr|^fel|^tpa|^tfp/",$campo);
			if($search){
					
				if($valor){
					$tem_ponto = preg_match("/,/",$valor);
					if($tem_ponto){
						$valor = str_replace(".","",$valor);
						$valor = str_replace(",",".",$valor);
							
					}
				}else{
					$valor = "null";
				}
				array_push($campos,array($campo=>$valor));
			}

		}

//		dbg($campos,1);
			
		$total = count($campos);
		if($dados['fprid']){


			$sql = "UPDATE obras.faseprojeto SET ";
			$j=0;
			foreach($campos as $campo=>$valor){
					
				foreach($valor as $c=>$v)
				if($v == "null"){
					$sql .= $c."=".$v;
				}else{
					$sql .= $c."="."'".$v."'";
				}
					
				if($j >= 0 && $j < ($total-1) )
				$sql .= ",";

				$j++;
					
			}
			$sql .= " WHERE obrid=".$_SESSION['obra']['obrid']." AND fprid=".$dados['fprid'];
			$query = $sql;

		}else{

			$sql = "INSERT INTO obras.faseprojeto (";
			$campo = "";
			$valor = "";

			for($k = 0;$k < $total ;$k++){

				$y = 0;
				//$campos[$k]['fprdtiniciofaseprojeto'] = $campos[$k]['fprdtiniciofaseprojeto'] ? formata_data_sql($campos[$k]['fprdtiniciofaseprojeto']) : "null";
				
					
				if( (current($campos[$k]) == 'null') ||  (current($campos[$k]) == '0.00') ||  (current($campos[$k]) == '') ){
//					$valor .= current($campos[$k]);
						$y = 0;
				}else{
					if( (current($campos[$k]) != '') && (key($campos[$k]) != '') ){
						$campo .= key($campos[$k]);
						$valor .= "'".current($campos[$k])."'";
						$y = 1;
						if( ($k >= 0) && ($k < ($total-1)) && ($y == 1) ){
							$campo .= ",";
							$valor .= ",";
							$y = 0;
						}
					}
				}

					
			}
			$query = $sql . $campo . ",obrid,fprstatus,fprdtinclusao) values (".$valor.",{$_SESSION['obra']['obrid']},'A','".Date('Y-m-d H:i:s')."');";
			$query = str_replace(",,",",",$query);
		}
//		dbg($query,1);	
		$this->simec->executar($query);
		
		$this->simec->commit();
			
		//insere dados na fase de licitaï¿½ï¿½o de projeto
		//pega o codigo da faseprojeto
		$fprid = ($this->simec->pegaUm("SELECT fprid FROM obras.faseprojeto	WHERE obrid = {$_SESSION["obra"]["obrid"]}"));
			
		$SQL = "DELETE FROM obras.faselicitacaoprojetos WHERE fprid = ".$fprid;
		$this->simec->executar($SQL);
			
		if(is_array($dados['tflid'])){
			foreach($dados['tflid'] as $key=>$item){

				$tflid = $dados['tflid'][$key];
				$flcpubleditaldtprev = $dados['flcpubleditaldtprev'][$key];
				$flcdtrecintermotivo = $dados['flcdtrecintermotivo'][$key];
				$flcrecintermotivo = $dados['flcrecintermotivo'][$key];
				$flcordservdt = $dados['flcordservdt'][$key];
				$flcordservnum = $dados['flcordservnum'][$key];
				$flchomlicdtprev = $dados['flchomlicdtprev'][$key];
				$flcaberpropdtprev = $dados['flcaberpropdtprev'][$key];
				$_sql ="";
					
				if($tflid ==2){
					$flcdata = $flcpubleditaldtprev;
					$flcrecintermotivo = "";
					$flcordservnum = "";
					$flcdtrecintermotivo = "";
					$flcordservdt = "";
					$flchomlicdtprev = "";
					$flcaberpropdtprev = "";
				}
				if($tflid ==5){
					$flcdata = $flcdtrecintermotivo;
					$flcpubleditaldtprev = "";
					$flcordservnum = "";
					$flcordservdt = "";
					$flchomlicdtprev = "";
					$flcaberpropdtprev = "";
				}
				if($tflid ==6){
					$flcdata = $flcordservdt;
					$flcrecintermotivo = "";
					$flcdtrecintermotivo = "";
					$flchomlicdtprev = "";
					$flcaberpropdtprev = "";
					$flcdtrecintermotivo = "";
				}
				if($tflid ==9){
					$flcdata = $flchomlicdtprev;
					$flcrecintermotivo = "";
					$flcordservnum = "";
					$flcdtrecintermotivo = "";
					$flcordservdt = "";
					$flcpubleditaldtprev = "";
					$flcaberpropdtprev = "";
				}
				if($tflid ==7){
					$flcdata = $flcaberpropdtprev;
					$flcrecintermotivo = "";
					$flcordservnum = "";
					$flcdtrecintermotivo = "";
					$flcordservdt = "";
					$flcpubleditaldtprev = "";
					$flchomlicdtprev = "";
				}
					
				$_sql .= "INSERT INTO obras.faselicitacaoprojetos(tflid,fprid,tfpstatus,tfpdtfase,tfpnumos,tfpobsmotivo) ";
				$_sql .= "VALUES";
				$_sql .= "(".$item.",".$fprid.",'A',";
				$_sql .= "'".formata_data_sql($flcdata)."',";
				if($flcordservnum!="") $_sql .= "'".$flcordservnum."',";
				else $_sql .= "null,";
				if($flcrecintermotivo!="") $_sql .= "'".$flcrecintermotivo."'";
				else $_sql .= "null";
				$_sql .= ")";
					
				$this->simec->executar($_sql);
			}
		}
			
		$this->simec->commit();

		$_REQUEST["acao"] = "A";
		$this->simec->sucesso("principal/projeto_arquitetonico");

	}

	public function ShowImage($img,$dir = ""){

		if($dir){
			$diretorio = $dir;
		}else{
			$diretorio = "../../arquivos/obras/imgs/";
		}
			
		if(file_exists($diretorio.$img)){
				
			list($w,$h) = getimagesize($diretorio.$img);
			print($w."-".$h."-".$img);

		}else{
			echo "0";
		}


	}

	public function FlipImage($request){
		session_start();
		session_unregister("img_atual");
		$foto = explode("documentos/",$request["img"]);
		$sql = "SELECT
				arqnome
			FROM
				public.arquivo arq
			INNER JOIN
				obras.arquivosobra oar
			ON 
				arq.arqid = oar.arqid
			INNER JOIN 
				obras.obrainfraestrutura obr
			ON
				obr.obrid = oar.obrid
			INNER JOIN
				seguranca.usuario seg
			ON
				seg.usucpf = oar.usucpf
			WHERE
				obr.obrid = {$_SESSION["obra"]["obrid"]} AND
				aqostatus = 'A' AND 
				arqnome like '%.jpg'";
		$matriz = $this->simec->carregar($sql);
		$new_matriz = Array();
		$i = 0;
		foreach($matriz as $ch =>$vl){
			foreach($vl as $c => $v){
				array_push($new_matriz,$v);
			}
		}
		$index = array_search($foto[1],$new_matriz);
			
		$_SESSION["img_atual"] = $new_matriz[$index+$request["direcao"]];
		$this->ShowImage($new_matriz[$index+$request["direcao"]],$request["dir"]);
			
	}

	public function CadastrarEtapas($etapa){

		$sql = $this->simec->executar("SELECT * FROM obras.itenscomposicao");

		while (($dados = pg_fetch_array($sql)) != false){
			if ($dados["itcdesc"] == $etapa){
				$acao = "A";
				echo "
					<script>
						alert('Etapa já cadastrada!');
						window.close;
					</script>";

			}
		}

		if ($acao != "A"){
				
			$sql = "
				INSERT INTO obras.itenscomposicao (itcdesc, itcstatus, itcdtinclusao)
					VALUES ('{$etapa}', 'A', 'now()')";
				
			$this->simec->executar($sql);
			$this->simec->commit();
				
			$_REQUEST["acao"] = "A";
			$this->simec->sucesso("principal/inserir_etapas");
				
		}

	}

	public function EnviarArquivo($arquivo,$dados,$dir = 'documentos'){

		// verifica a sessï¿½o da obra
		obras_verifica_sessao();

		// obtï¿½m o arquivo
		$arquivo = $_FILES['arquivo'];
		if ( !is_uploaded_file( $arquivo['tmp_name'] ) ) {
			redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
		}
		// BUG DO IE
		// O type do arquivo vem como image/pjpeg
		if($arquivo["type"] == 'image/pjpeg') {
			$arquivo["type"] = 'image/jpeg';
		}
		//Insere o registro do arquivo na tabela public.arquivo
		$sql = "INSERT INTO public.arquivo 	(arqnome,arqextensao,arqdescricao,arqtipo,arqtamanho,arqdata,arqhora,usucpf,sisid)
		values('".current(explode(".", $arquivo["name"]))."','".end(explode(".", $arquivo["name"]))."','".substr($dados["arqdescricao"],0,255)."','".$arquivo["type"]."','".$arquivo["size"]."','".date('Y-m-d')."','".date('H:i:s')."','".$_SESSION["usucpf"]."',". $_SESSION["sisid"] .") RETURNING arqid;";
		$arqid = $this->simec->pegaUm($sql);

		//Insere o registro na tabela obras.arquivosobra
		$sql = "INSERT INTO obras.arquivosobra (obrid,tpaid,arqid,usucpf,aqodtinclusao,aqostatus)
		values(".$_SESSION["obra"]["obrid"].",".$dados["tpaid"].",". $arqid .",'".$_SESSION["usucpf"]."','". date("Y-m-d H:i:s") ."','A');";
		$this->simec->executar($sql);

		if(!is_dir('../../arquivos/obras/'.floor($arqid/1000))) {
			mkdir(APPRAIZ.'/arquivos/obras/'.floor($arqid/1000), 0777);
		}
		$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arqid/1000) .'/'. $arqid;
		switch($arquivo["type"]) {
			case 'image/jpeg':
				
				try {
				
					ini_set("memory_limit", "128M");
					list($width, $height) = getimagesize($arquivo['tmp_name']);
					$original_x = $width;
					$original_y = $height;
					// se a largura for maior que altura
					if($original_x > $original_y) {
						$porcentagem = (100 * 640) / $original_x;
					}else {
						$porcentagem = (100 * 480) / $original_y;
					}
					$tamanho_x = $original_x * ($porcentagem / 100);
					$tamanho_y = $original_y * ($porcentagem / 100);
					$image_p = imagecreatetruecolor($tamanho_x, $tamanho_y);
					$image   = imagecreatefromjpeg($arquivo['tmp_name']);
					imagecopyresampled($image_p, $image, 0, 0, 0, 0, $tamanho_x, $tamanho_y, $width, $height);
					imagejpeg($image_p, $caminho, 100);
					//Clean-up memory
					ImageDestroy($image_p);
					//Clean-up memory
					ImageDestroy($image);
				
				} catch (Exception $e) {
					
					if ( !move_uploaded_file( $arquivo['tmp_name'], $caminho ) ) {
						$this->simec->rollback();
						echo "<script>alert(\"Problemas no envio do arquivo.\");</script>";
						exit;
					}
				}
				break;
			default:
				if ( !move_uploaded_file( $arquivo['tmp_name'], $caminho ) ) {
					$this->simec->rollback();
					echo "<script>alert(\"Problemas no envio do arquivo.\");</script>";
					exit;
				}
		}


		$this->simec->commit();
		$this->simec->sucesso("principal/".$dir);

	}

	public function DownloadArquivo($param){
		$sql ="SELECT * FROM public.arquivo WHERE arqid = ".$param['arqid'];
		$arquivo = current($this->simec->carregar($sql));
		$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arquivo['arqid']/1000) .'/'.$arquivo['arqid'];
		if ( !is_file( $caminho ) ) {
			$_SESSION['MSG_AVISO'][] = "Arquivo não encontrado.";
		}
		if ( is_file( $caminho ) ) {
			$filename = str_replace(" ", "_", $arquivo['arqnome'].'.'.$arquivo['arqextensao']);
			header( 'Content-type: '. $arquivo['arqtipo'] );
			header( 'Content-Disposition: attachment; filename='.$filename);
			readfile( $caminho );
			exit();
		} else {
			die("<script>alert('Arquivo não encontrado.');window.location='obras.php?modulo=principal/documentos&acao=A';</script>");
			
		}
	}

	public function DeletarDocumento($documento, $caminho = 'principal/documentos'){

		$sql = "UPDATE obras.arquivosobra SET aqostatus = 'I' where aqoid=".$documento["aqoid"];
		$this->simec->executar($sql);

		$sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$documento["arqid"];
		$this->simec->executar($sql);

		$this->simec->commit();
		$_REQUEST["acao"] = "A";
		$this->simec->sucesso($caminho);

	}

	public function CadastrarInfraEstrutura($obra){

		// verifica a sessï¿½o da obra
		obras_verifica_sessao();

		$iexid = ($this->simec->pegaUm("SELECT iexid FROM obras.obrainfraestrutura	WHERE obrid = {$_SESSION["obra"]["obrid"]}"));

		foreach($obra as $campo=>$valor){
			if (!is_array($valor)){
				if(!trim($valor)){
					$obra[$campo] = "NULL";
				} else {
					$obra[$campo] = "'" . pg_escape_string(trim($valor))  .  "'";
				}
			}
		}

		foreach($obra as $nome=>$valor){
			if(is_array($valor)){
				if(preg_match("/^tmaid/",$nome)){
					$tmaid = $valor;
				}
			}
		}

		// Insere os dados na tabela infraestrutura
		// se nï¿½o houver infraestrutura cadastrada para a obra selecionada
		if (!$iexid){
			$obra["umdidareaampliada"]    = empty( $obra["umdidareaampliada"] )    ? 'NULL' : $obra["umdidareaampliada"];
			$obra["umdidareareforma"]     = empty( $obra["umdidareareforma"] )     ? 'NULL' : $obra["umdidareareforma"];
			$obra["umdidareaconstruida"]  = empty( $obra["umdidareaconstruida"] )  ? 'NULL' : $obra["umdidareaconstruida"];
				
			$sql = "
				INSERT INTO 
					obras.infraestrutura 
						(iexsitdominialimovelregulariza, 
						 iexinfexistedimovel,
						 iexampliacao,
					 	 iexedificacaoreforma,
					 	 iexareaconstruida,
					 	 umdidareaconstruida,
					 	 iexdescsumariaedificacao,
					 	 iexqtdareapreforma,
					 	 umdidareareforma,
					 	 iexvlrareapreforma,
					 	 iexqtdareaampliada,
					 	 umdidareaampliada,
					 	 iexvlrareaampliada,
					 	 aqiid) 
				VALUES
					({$obra["iexsitdominialimovelregulariza"]},
					{$obra["iexinfexistedimovel"]},
					{$obra["iexedificacaoreforma"]},
					{$obra["iexampliacao"]},
					{$this->MoedaToBd($obra["iexareaconstruida"])},
					{$obra["umdidareaconstruida"]},
					{$obra["iexdescsumariaedificacao"]},
					{$this->MoedaToBd($obra["iexqtdareapreforma"])},
					{$obra["umdidareareforma"]},
					{$this->MoedaToBd($obra["iexvlrareapreforma"])},
					{$this->MoedaToBd($obra["iexqtdareaampliada"])},
					{$obra["umdidareaampliada"]},
					{$this->MoedaToBd($obra["iexvlrareaampliada"])},
					{$obra["aqiid"]}) returning iexid ";
						
					$retorno = ($this->simec->pegaUm($sql));
						
					// Atualiza a tabela obrainfraestrutura setando o ID
					// da ï¿½ltima infraestrutura cadastra
					$sql ="	UPDATE obras.obrainfraestrutura SET	iexid = '{$retorno}' WHERE obrid = ".$_SESSION["obra"]["obrid"];
						
					$this->simec->executar($sql);
						
					// Insere os dados na tabela tipomoduloampliacao

					for($k = 0;$k < count($tmaid);$k++){

						$sql = "
					INSERT INTO 
						obras.modulosampliacao (tmaid,iexid) 
					VALUES 
						(" . $tmaid[$k] . ", 
						" . $retorno . ")";
							
						$this->simec->executar($sql);
							
					}
						
		}
			
		if ($iexid){
			$obra["umdidareaampliada"]    = empty( $obra["umdidareaampliada"] )   ? 'NULL' : $obra["umdidareaampliada"];
			$obra["umdidareareforma"]     = empty( $obra["umdidareareforma"] )    ? 'NULL' : $obra["umdidareareforma"];
			$obra["umdidareaconstruida"]  = empty( $obra["umdidareaconstruida"] ) ? 'NULL' : $obra["umdidareaconstruida"];
				
			$sql = "
				UPDATE
					obras.infraestrutura 
				SET
					iexsitdominialimovelregulariza = {$obra["iexsitdominialimovelregulariza"]},
					iexinfexistedimovel = {$obra["iexinfexistedimovel"]},
					iexedificacaoreforma = {$obra["iexedificacaoreforma"]},
					iexampliacao = {$obra["iexampliacao"]},
					iexareaconstruida = {$this->MoedaToBd($obra["iexareaconstruida"])},
					umdidareaconstruida = {$obra["umdidareaconstruida"]},
					iexdescsumariaedificacao = {$obra["iexdescsumariaedificacao"]},
					iexqtdareapreforma = {$this->MoedaToBd($obra["iexqtdareapreforma"])},
					umdidareareforma = {$obra["umdidareareforma"]},
					iexvlrareapreforma = {$this->MoedaToBd($obra["iexvlrareapreforma"])},
					iexqtdareaampliada = {$this->MoedaToBd($obra["iexqtdareaampliada"])},
					umdidareaampliada = {$obra["umdidareaampliada"]},
					iexvlrareaampliada = {$this->MoedaToBd($obra["iexvlrareaampliada"])},
					aqiid = {$obra["aqiid"]}
				WHERE
					iexid = {$iexid}";
				
			$this->simec->executar($sql);
				
			// Insere os dados na tabela tipomoduloampliacao
				
			$sql = "DELETE FROM obras.modulosampliacao WHERE iexid = {$iexid}";
			$this->simec->executar($sql);
				
			for($k = 0;$k < count($tmaid);$k++){

				$sql = "
					INSERT INTO 
						obras.modulosampliacao (tmaid,iexid) 
					VALUES 
						(" . $tmaid[$k] . ", 
						" . $iexid . ")";

				$this->simec->executar($sql);
					
			}
				
		}

		$this->simec->commit();

		$_REQUEST["acao"] = "A";
		$this->simec->sucesso("principal/infraestrutura");
	}

	public function CadastrarModulos($modulo){
		$sql = $this->simec->executar("SELECT * FROM obras.tipomoduloampliacao");

		while (($dados = pg_fetch_array($sql)) != false){
			if ($dados["tmadesc"] == $modulo){
				$acao = "A";
				echo "
					<script>
						alert('Módulo já cadastrado!');
						window.close;
					</script>";
			}
		}

		if ($acao != "A"){
				
			$sql = "
				INSERT INTO obras.tipomoduloampliacao (tmadesc, tmastatus, tmadtinclusao)
					VALUES ('{$modulo}', 'A', 'now()')";
				
			$this->simec->executar($sql);
			$this->simec->commit();
				
			$_REQUEST["acao"] = "A";
			$this->simec->sucesso("principal/inserir_modulos");
				
		}
	}

	public function UpdateListFoto(){
		if($_SESSION["supvid"]) {
			$sql = "select fot.*, arq.arqdescricao from obras.fotos AS fot
					left join public.arquivo AS arq ON arq.arqid = fot.arqid 
					where obrid = '".$_SESSION["obra"]["obrid"]."' AND supvid = '".$_SESSION["supvid"]."' ORDER BY fotordem ASC;";
			$fotos = ($this->simec->carregar($sql));
			if(is_array($fotos)){
				for($k=0;$k<20;$k++){
					$pagina = floor($k/16);
					echo '<div class="imageBox" id="imageBox' . $k . '">';
					if($fotos[$k]["fotdsc"] != ""){
						echo "<img id='".$fotos[$k]["arqid"]."' src='../slideshow/slideshow/verimagem.php?newwidth=64&newheight=48&arqid=". $fotos[$k]["arqid"] ."'  style=\"margin:0px;opacity:1\" class=\"imageBox_theImage\" title='". $fotos[$k]["arqdescricao"] ."' title='". $fotos[$k]["arqdescricao"] ."' onClick='javascript:window.open(\"../slideshow/slideshow/index.php?pagina=". $pagina ."&arqid=\"+this.id+\"\",\"imagem\",\"width=850,height=600,resizable=yes\")' />\n";
						echo "<input type='hidden' value='".$fotos[$k]["arqid"]."' id='".$fotos[$k]["fotbox"]."_".$fotos[$k]["arqid"]."' name='".$fotos[$k]["fotbox"]."'  />\n";
						echo "<input type='checkbox' onclick=repositorioGaleria( 'imageBox".$k."', ".$fotos[$k]["arqid"].", '../slideshow/slideshow/verimagem.php?arqid=".$fotos[$k]["arqid"]."' ); value='".$fotos[$k]["arqid"]."' id='galeria_".$fotos[$k]["arqid"]."'> Incluir na Galeria";
					}

					echo "</div>\n";
				}
			}else{
				for($k=0;$k<20;$k++){
					echo "<div class='imageBox' id='imageBox{$k}' ></div>";
				}
			}
		}
	}

	public function CadastrarRestricao($obra){

		// verifica a sessï¿½o da obra
		obras_verifica_sessao();

		$rstdtprevisaoregularizacao = !empty($obra['rstdtprevisaoregularizacao']) ? "'" . formata_data_sql($obra["rstdtprevisaoregularizacao"]) . "'" : 'null';

		$sql = "
			INSERT INTO 
				obras.restricaoobra 
				(obrid, trtid, rstdesc, 
				 rstdtprevisaoregularizacao, rstdescprovidencia, 
				 usucpf, rststatus, rstdtinclusao, fsrid)
			VALUES
				({$_SESSION["obra"]["obrid"]}, {$obra["trtid"]}, 
				'{$obra["rstdesc"]}', 
				{$rstdtprevisaoregularizacao},
				'{$obra["rstdescprovidencia"]}', 
				'{$_SESSION["usucpf"]}', 'A', 'now()', {$obra["fsrid"]})";


				$this->simec->executar($sql);
				$this->simec->commit();

				die("
			<script>
				alert('Operação realizada com sucesso!');
				window.opener.location.replace(window.opener.location);
				window.close();
			</script>
			");

	}

	public function AtualizarRestricaoObra($obra){

		// verifica a sessï¿½o da obra
		obras_verifica_sessao();

		$rstdtprevisaoregularizacao = !empty($obra['rstdtprevisaoregularizacao']) ? "'" . formata_data_sql($obra["rstdtprevisaoregularizacao"]) . "'" : 'null';
		$rstdtsuperacao				= !empty($obra['rstdtsuperacao']) 			  ? "'" . formata_data_sql($obra["rstdtsuperacao"]) . "'" 			  : 'null';

		$sql = "
			UPDATE 
				obras.restricaoobra
			SET
				trtid = " . $obra["trtid"] . ",
				rstdescprovidencia = '{$obra["rstdescprovidencia"]}',
				rstsituacao = {$obra["rstsituacao"]},
				rstdtprevisaoregularizacao = " . $rstdtprevisaoregularizacao . ",
				rstdesc = '{$obra["rstdesc"]}',
				rstdtsuperacao = " . $rstdtsuperacao . ",
				fsrid = {$obra["fsrid"]}
			WHERE
				rstoid = {$obra["rstoid"]} AND
				obrid = {$_SESSION["obra"]["obrid"]}
		";

		$this->simec->executar($sql);
		$this->simec->commit();

		die("
			<script>
				alert('Operação realizada com sucesso!');
				window.opener.location.replace(window.opener.location);
				window.close();
			</script>
			");

	}

	public function DeletarRestricao($rstoid){

		// verifica a sessï¿½o da obra
		obras_verifica_sessao();

		$sql = "
			UPDATE 
				obras.restricaoobra 
			SET 
				rststatus = 'I' 
			WHERE 
				obrid  = " . $_SESSION["obra"]["obrid"] . " AND
				rstoid = {$rstoid}";

		$this->simec->executar($sql);
		$this->simec->commit();

		$_REQUEST["acao"] = "A";
		$this->simec->sucesso("principal/restricao");

	}

	public function CadastrarAquisicao($obra){

		// verifica a sessï¿½o da obra
		obras_verifica_sessao();

		$aeqid = ($this->simec->pegaUm("SELECT aeqid FROM obras.aquisicaoequipamentos WHERE obrid = {$_SESSION["obra"]["obrid"]}"));

		foreach($obra as $campo=>$valor){
			if (!is_array($valor)){
				if($valor == "" ){
					$obra[$campo] = 'NULL';
				} else {
					$obra[$campo] = "'" . pg_escape_string(trim($valor))  .  "'";
				}
			}
		}

		foreach($obra as $nome=>$valor){
			if(is_array($valor)){
				if(preg_match("/^tmaid/",$nome)){
					$tmaid = $valor;
				}
			}
		}




		// Insere os dados na tabela aquisicaoequipamentos
		// se nï¿½o houver aquisicaoequipamentos cadastrada para a obra selecionada
		if (!$aeqid){
			$sql = "
			INSERT INTO 
				obras.aquisicaoequipamentos 
					(faeid,
					 obrid,
				 	 aeqdtpubledital,
				 	 aeqdtpublreslicitacao,
				 	 aeqobs,
				 	 aeqdtinclusao) 
			VALUES
				({$obra["faeid"]},
				{$obra["obrid"]},
				{$obra["aeqdtpubledital"]},
				{$obra["aeqdtpublreslicitacao"]},
				{$obra["aeqobs"]}, now()) returning aeqid ";

				$retorno = ($this->simec->pegaUm($sql));

				// Atualiza a tabela aquisicaoequipamentos setando o ID
				// da ï¿½ltima aquisicaoequipamentos cadastra
				$sql ="	UPDATE obras.aquisicaoequipamentos SET	aeqid = '{$retorno}' WHERE obrid = ".$_SESSION["obra"]["obrid"];

				$this->simec->executar($sql);

					
		}

		if ($aeqid){

			$sql = "
			UPDATE
				obras.aquisicaoequipamentos 
			SET
				faeid = {$obra["faeid"]},
				aeqdtpubledital = {$obra["aeqdtpubledital"]},
				aeqdtpublreslicitacao = {$obra["aeqdtpublreslicitacao"]},
				aeqobs = {$obra["aeqobs"]},
				aeqdtinclusao = now()
			WHERE
				aeqid = {$aeqid}";


			$this->simec->executar($sql);


		}

		$this->simec->commit();

		$_REQUEST["acao"] = "A";
		$this->simec->sucesso("principal/aquisicao_equipamentos");

	}

	function EnviarGaleria( $imagens ){
		
		$arrArqid = explode(",", $imagens);
		
		if( is_array($arrArqid) ){

			foreach( $arrArqid as $valor ){
				if(!$valor){
					echo "Não há foto(s) selecionada(s)!";
					return false;
				} else {
						$sql = "INSERT INTO 
											obras.arquivosobra ( 
														 obrid,
														 tpaid,
														 arqid,
														 usucpf,
														 aqodtinclusao,
														 aqostatus )
												VALUES ( 
														{$_SESSION["obra"]["obrid"]},
														 21,
														 {$valor},
														 '{$_SESSION["usucpf"]}',
														 'now',
														 'A' );";
						
						$this->simec->executar($sql);
				
				}
			}
			if( $this->simec->commit() ){
				print "Operação realizada com sucesso!";	
			}else{
				print "Ocorreu um erro no envio das imagens!";
			}
			
		}
		
	}
	
}

class UploadArquivo{


	public $arquivo = "";
	public $destino = "";
	public $name_arquivo = "";

	private $erro = array ( "0" => "upload execultado com sucesso!",
	                        "1" => "O arquivo é maior que o permitido pelo Servidor",
	                        "2" => "O arquivo é maior que o permitido pelo formulario",
	                        "3" => "O upload do arquivo foi feito parcialmente",    
	                        "4" => "Não foi feito o upload do arquivo",
	                        "5" => "Arquivo não possui o formato válido (.pdf|.doc|.xls)."
	                        );

	                        public function __construct($dado){
	                        	$this->arquivo = isset($dado["arquivo"]) ? $dado["arquivo"] : FALSE;
	                        }

	                        public function VerificaUpload(){

	                        	if(!is_uploaded_file($this->arquivo['tmp_name'])){

	                        		return false;
	                        	}
	                        	 
	                        	if($this->arquivo["type"] == "application/exe" || $this->arquivo["type"] == "application/bat" || $this->arquivo["type"] == "application/x-php" || $this->arquivo["type"] == "application/javascript"){
	                        		$this->arquivo['error'] = $this->erro[5];
	                        		return false;
	                        	}
	                        	 
	                        	return true;
	                        }

	                        public function EnviarArquivo(){

	                        	if($this->VerificaUpload()) {

	                        		$this->Upload();
	                        		return true;

	                        	} else {

	                        		$this->erro[$this->arquivo['error']];
	                        	}
	                        }

	                        public function Upload(){


	                        	$this->name_arquivo = $this->arquivo['name'];
	                        	$caminho = "../../arquivos/obras/documentos/";
	                        	$this->destino = $caminho.$this->name_arquivo;
	                        	move_uploaded_file($this->arquivo["tmp_name"],$this->destino);
	                        	$this->erro[$this->arquivo['error']];
	                        	 
	                        }

}


/**
 * POG Temporaria (Douglas)
 */
function x(&$a){return "'" . $a . "'";}


/**
 * Classe pai que possue o construtor e o mï¿½todo call a serem utilizados nas 
 * classes do mï¿½dulo
 * 
 * @author Orion Teles de Mesquita
 * @since 18/08/2009
 * 
 */
class obraPai{
	
	public $db;
	
	/**
	 * Funï¿½ï¿½o construtora das classes que cria os sets
	 * 
	 * @param array $dados
	 * @author Orion Teles de Mesquita
 	 * @since 18/08/2009
 	 * 
	 */
	function __construct( $dados = array() ){

		global $db;
		$this->db = $db;
		
		if( is_array( $dados ) ){
			foreach( $dados as $stAtributo => $mxValor ){
				if( property_exists($this, $stAtributo) ){
					$this->$stAtributo = $mxValor;
				}
			}
		}
		
	}

	/**
	 * Mï¿½todo call que verifica os atributos chamados pelos objetos no mï¿½dulo e
	 * evita o erro de programaï¿½ï¿½o
	 * 
	 * @param string $stMetodo
	 * @param array $arParametros
	 * @author Orion Teles de Mesquita
 	 * @since 18/08/2009
 	 *  
	 */
	final public function __call( $stMetodo, $arParametros ){
		
		$stMetodo = strtolower( $stMetodo );
		$stPrefixo  = substr( $stMetodo, 0 , 3 );
		$stAtributo = substr( $stMetodo, 3 );
		
		if ( method_exists( $this->db, $stMetodo ) ){
			return $this->db->$stMetodo( $arParametros[0] );
		}else if( $stPrefixo == 'get' ){
			if( property_exists($this, $stAtributo) ){
				return $this->$stAtributo;
			}	
		}else if( $stPrefixo == 'set' ){
			if( property_exists($this, $stAtributo) ){
				$this->$stAtributo = $arParametros[0];
			}	
		}
		
		
		
	}
	
	/**
	 * Funï¿½ï¿½o que trata as variï¿½veis antes de serem inseridas nas querys
	 *
	 * @param mixed $dados
	 * @return mixed
	 * @author Fernando Araï¿½jo Bagno da Silva
 	 * @since 18/08/2009
	 */
	public function trataDados( $dados = array() ){
		
		if ( is_array( $dados ) ){
			foreach ( $dados as $campo=>$valor ){
				if ( !is_numeric( $valor ) ){
					$dados[$campo] = !empty( $valor ) ? "'" . pg_escape_string( trim( $valor ) ) . "'" : "''";
				}else{
					$dados[$campo] = !empty( $valor ) ? $valor : "NULL";
				}
			}
		}else{
			if ( !is_numeric( $dados ) ){
				$dados = !empty( $dados ) ? "'" . pg_escape_string( trim( $dados ) ) . "'" : "''";
			}else{
				$dados = !empty( $dados ) ? $dados : "NULL";
			}
		}
		
		return $dados;
		
	}
	
	/**
	 * Funï¿½ï¿½o que trata as string que possuem alguma mï¿½scara que nï¿½o ï¿½ inserida 
	 * no banco
	 *
	 * @param string $string
	 * @return string
	 * @author Fernando Araï¿½jo Bagno da Silva
 	 * @since 18/08/2009
	 */
	public function trataString( $string ){
		
		$string = str_replace( "-", "", $string );
		$string = str_replace( ".", "", $string );
		$string = str_replace( "/", "", $string );
		
		return $string;
	}
	
}

/**
 * Classe que controla a tela de execuï¿½ï¿½o orï¿½amentï¿½ria da obra
 * @author FernandoSilva
 *
 */
class execOrcamentaria extends obraPai{
	
	public function buscaExecOrcamentaria( $obrid ){
		
		$sql = "SELECT 
					* 
				FROM
					obras.execucaoorcamentaria
				WHERE
					obrid = {$obrid} AND
					teoid = " . OBRAS_TIPO_EXECORC_OBRAS . " AND
					eocstatus = 'A'";
		
		return $this->db->pegaLinha( $sql );
		
	}
	
	public function buscaDetalheExecOrcamentaria( $eorid, $eocvlrtotal ){

		if ( $eorid ){
			
			$sql = "SELECT
						*
					FROM
						 obras.itensexecucaoorcamentaria
					WHERE
						eorid = {$eorid}
					ORDER BY
						eocdtposicao";
			
			$dadosItensExec =  $this->db->carregar( $sql );
		
		}

   		$perfis = obras_arrayPerfil();
		   if ($perfis[0] == PERFIL_CONSULTATIPOENSINO){
		   		$botaoExcluirDetalhamento = "<img src='../imagens/excluir_01.gif' style='cursor:pointer;' title='Este Detalhamento, não pode ser excluído!'/>";
			}else{
		   		$botaoExcluirDetalhamento = "<img src='../imagens/excluir.gif' style='cursor:pointer;' onclick='excluirItemExec(this.parentNode.parentNode.rowIndex);'/>";
		   }

		if( $dadosItensExec ){
			for( $i = 0; $i < count($dadosItensExec); $i++ ){
				
				$cor = ( $i % 2 ) ? "#e0e0e0" : "#f4f4f4";
				
//				ver($dadosItensExec[$i]["eocvlrempenhado"],$dadosItensExec[$i]["eocvlrliquidado"]);
//				die();
				
				$perEmpenhado = ($eocvlrtotal > 0) ? ( $dadosItensExec[$i]["eocvlrempenhado"] / $eocvlrtotal ) * 100 : 0.00;
				$perLiquidado = ($eocvlrtotal > 0) ? ( $dadosItensExec[$i]["eocvlrliquidado"] / $eocvlrtotal ) * 100 : 0.00;
				
				$totEmpenhado = $totEmpenhado + $dadosItensExec[$i]["eocvlrempenhado"];
				$totLiquidado = $totLiquidado + $dadosItensExec[$i]["eocvlrliquidado"];
				
				$totPerEmpenhado = $totPerEmpenhado + $perEmpenhado;
				$totPerLiquidado = $totPerLiquidado + $perLiquidado; 
				
				print "<tr bgcolor='{$cor}' align='right' id='item_{$dadosItensExec[$i]["ideid"]}'>"
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
					. "    <td width='14%' align='center'>"
					. "		$botaoExcluirDetalhamento	"
					. "    </td>";
				
			}
		}
		
		//$totPerEmpenhado = $totPerEmpenhado / $i;
		//$totPerLiquidado = $totPerLiquidado / $i;
		
		$totDetalhamentoOrc = array( "totalempenho"   	  => $totEmpenhado, 
									 "totalliquidado"     => $totLiquidado, 
									 "totalpercempenhado" => $totPerEmpenhado, 
									 "totalpercliquidado" => $totPerLiquidado );
		
		return $totDetalhamentoOrc;
		
	}
	
	/**
	 * 
	 * @param array $dados
	 */
	public function registraExecOrcamentaria( $dados ){
		
		$dados["eocvlrcapital"] = str_replace( array(".",","," "), array("",".",""),  $dados["eocvlrcapital"] );
		$dados["eocvlrcusteio"] = str_replace( array(".",","," "), array("",".",""),  $dados["eocvlrcusteio"] );
		
		$dados["eocvlrcapital"] = !empty( $dados["eocvlrcapital"] ) ? $dados["eocvlrcapital"] : "NULL";
		$dados["eocvlrcusteio"] = !empty( $dados["eocvlrcusteio"] ) ? $dados["eocvlrcusteio"] : "NULL";
		
		if ( $dados["eorid"] ){
			
			$sql = "UPDATE
						obras.execucaoorcamentaria
					SET
						eocvlrcapital = {$dados["eocvlrcapital"]},
						eocvlrcusteio = {$dados["eocvlrcusteio"]},
						usucpf		  = '{$_SESSION["usucpf"]}'
					WHERE
						eorid = {$dados["eorid"]}";
			
			$this->db->executar( $sql );
			
			$sql = "DELETE FROM obras.itensexecucaoorcamentaria WHERE eorid = {$dados["eorid"]}";
			$this->db->executar( $sql );
			
		}else{
			
			$sql = "INSERT INTO obras.execucaoorcamentaria ( obrid, 
													 		 eocvlrcapital, 
													 		 eocvlrcusteio, 
													 		 usucpf, 
													 		 teoid, 
													 		 eocstatus, 
													 		 eocdtinclusao ) 
													VALUES ( {$_SESSION["obra"]["obrid"]},
															 {$dados["eocvlrcapital"]},
															 {$dados["eocvlrcusteio"]},
															 '{$_SESSION["usucpf"]}',
															 " . OBRAS_TIPO_EXECORC_OBRAS . ",
															 'A',
															 'now' ) 
												 RETURNING eorid";
			
			$dados["eorid"] = $this->db->pegaUm( $sql );
															 
		}

		for( $i = 0; $i < count($dados["eocdtposicao"]); $i++ ){
			
			if ( $dados["eocvlrliquidado"][$i] != "" ){
				
				$sql = "INSERT INTO obras.itensexecucaoorcamentaria ( eorid, 
																	  usucpf, 
																	  eocvlrempenhado, 
																	  eocvlrliquidado, 
																	  eocdtposicao, 
																	  eocdtinclusao )
															 VALUES ( {$dados["eorid"]},
															 		  '{$_SESSION["usucpf"]}',
															 		  {$dados["eocvlrempenhado"][$i]},
															 		  {$dados["eocvlrliquidado"][$i]},
															 		  '" . formata_data_sql( $dados["eocdtposicao"][$i] ) . "',
															 		  'now' )";
				
				$this->db->executar( $sql );
			}			
		}
		
		$this->db->commit( $sql );
		$this->db->sucesso( "principal/exec_orcamentaria" );
		
	}
	
}

/**
 * Classe que controla os Termos de Ajuste das obras.
 * @author FernandoSilva
 *
 */
class termoDeAjuste extends obraPai{
	
	// Declaraï¿½ï¿½o dos atributos da tabela obras.termoajuste
	public $traid 		  = null;
	public $traassunto 	  = null;
	public $tralocal 	  = null;
	public $tradtcriacao  = null;
	public $tratextoata   = null;
	public $tradtinclusao = null;
	public $trastatus 	  = null;
	
	// Declaraï¿½ï¿½o dos atributos da tabela obras.anexotermoajuste
	public $ataid 		  = null;
	public $arqid 		  = null;
	public $atadsc  	  = null;
	public $atadtinclusao = null;
	public $atastatus	  = null;
	
	public function verificaDados( $traid, $orgid ){
		
		$existeTermo = $this->pegaUm("SELECT traid FROM obras.termoajuste WHERE traid = {$traid}");
		
		if ( !empty($existeTermo) ){
			$_SESSION["obra"]["traid"] = $traid;
		}else{
			print '<script>
						alert("O Termo de Ajuste enviado via parâmetro não existe!");
						history.back(-1);
				   </script>';
		}
		
		$existeOrgao = $this->pegaUm("SELECT orgid FROM obras.orgao WHERE orgid = {$orgid}");
		
		if ( !empty($existeOrgao) ){
			$_SESSION['obra']['traid_orgid'] = $orgid;
		}else{
			print '<script>
						alert("O Tipo de Estabelecimento via parâmetro não existe!");
						history.back(-1);
				   </script>';
		}
		
	}
	
	public function verificaSessao(){
		
		if ( $_SESSION["obra"]["traid"] == null ){
			print "<script>"
				. "		alert('A sessão do termo foi perdida!');"
				. "		location.href='obras.php?modulo=principal/lista_de_termos&acao=A'"
				. "</script>";
			die;
		}
		
	}
	
	public function cabecalho( $traid ){
		
		$sql = "SELECT 
					traassunto as assunto,
					tralocal as local, 
					to_char(tradtcriacao, 'DD/MM/YYYY') as data, 
					orgdesc as ensino
				FROM
					obras.termoajuste ta
				INNER JOIN
					obras.orgao oo ON oo.orgid = ta.orgid
				WHERE
					traid = {$traid}";
		
		$dados = $this->pegaLinha( $sql );
		
		if ( $dados ){

			print "<table class='tabela' bgcolor='#f5f5f5' cellspacing='1' cellpadding='3' align='center'>"
				. "		<tr>"
				. "		<td class='subtitulodireita' width='180px'>Tipo de Estabelecimento:</td><td>{$dados["ensino"]}</td>"
				. "		</tr>" 
				. "		<tr>"
				. "		<td class='subtitulodireita'>Assunto:</td><td>{$dados["assunto"]}</td>"
				. "		</tr>"
				. "		<tr>"
				. "		<td class='subtitulodireita'>Local:</td><td>{$dados["local"]}</td>"
				. "		</tr>"
				. "		<tr>"
				. "		<td class='subtitulodireita'>Data:</td><td>{$dados["data"]}</td>"
				. "		</tr>"
				. "</table>";
				
		}
		
	}
	
	/**
	 * Busca os dados do termo de ajuste
	 * 
	 * @param integer $traid
	 * @author Fernando Araï¿½jo Bagno da Silva
 	 * @since 18/08/2009
	 */
	public function buscaTermoAjuste( $traid ){
		
		$sql = "SELECT * FROM obras.termoajuste WHERE traid = {$traid}";
		
		$dados = $this->pegaLinha( $sql );
		
		foreach( $dados as $chave=>$valor ){
			$dados[$chave] = trim($valor);
		}
		
		return $dados;
	
	}
	
	public function PesquisaTermoAjuste( $dados ){
		
		global $orgidRes;
		
		$filtro = !empty($dados["orgid"]) 		  ? " AND ta.orgid = {$dados["orgid"]} " : "AND ta.orgid in (" . ( implode(", ", $orgidRes ) ) . ") ";
		$filtro .= !empty($dados["traassunto"])   ? " AND ta.traassunto ilike '%{$dados["traassunto"]}%' " : "";
		$filtro .= !empty($dados["tradtcriacao"]) ? " AND ta.tradtcriacao = '". formata_data_sql($dados["tradtcriacao"]) . "' " : "";
		$filtro .= !empty($dados["usucpf"]) ? " AND su.usucpf = '{$dados["usucpf"]}' " : "";
		
		return $filtro;
		
	}
	
	/**
	 * Cadastra o termo de ajuste de obras
	 *
	 * @param array $dados
	 * @author Fernando Araï¿½jo Bagno da Silva
 	 * @since 18/08/2009
	 */
	function CadastraTermoAjuste( $dados = array() ){
		
		$dados["tradtcriacao"] = formata_data_sql($dados["tradtcriacao"]);
		$dados = self::trataDados( $dados );
		
		// cria a query de inserï¿½ï¿½o do termo de ajuste
		$sql = "INSERT INTO obras.termoajuste( traassunto, tralocal, tradtcriacao,
											   tratextoata, orgid, usucpf, 
											   trastatus, tradtinclusao )
									  VALUES ( {$dados["traassunto"]}, {$dados["tralocal"]}, {$dados["tradtcriacao"]},
									  		   {$dados["tratextoata"]}, {$dados["orgid"]}, '{$_SESSION["usucpf"]}', 
									  		   'A', 'now' )
								   RETURNING traid";

		$_SESSION['obra']['traid'] = $this->pegaUm( $sql );
		$_SESSION['obra']['traid_orgid'] = $dados["orgid"];

		$this->commit();
		$this->sucesso("principal/termodeajuste", "");
		
	}
	
	/**
	 * Atualiza o termo de ajuste de obras
	 *
	 * @param array $dados
	 * @author Fernando Araï¿½jo Bagno da Silva
 	 * @since 18/08/2009
	 */
	function AtualizaTermoAjuste( $dados = array() ){
		
		$dados["tradtcriacao"] = formata_data_sql($dados["tradtcriacao"]);
		$dados = self::trataDados( $dados );
		
		$_SESSION['obra']['traid_orgid'] = $dados["orgid"];
		
		// cria a query de inserï¿½ï¿½o do termo de ajuste
		$sql = "UPDATE 
					obras.termoajuste
				SET 
					traassunto 	 = {$dados["traassunto"]}, 
					tralocal     = {$dados["tralocal"]}, 
					tradtcriacao = {$dados["tradtcriacao"]},
					tratextoata  = {$dados["tratextoata"]},
					orgid  		 = {$dados["orgid"]} 
				WHERE
					traid = {$dados["traid"]}";

		$this->executar( $sql );
		
		$this->commit();
		$this->sucesso("principal/termodeajuste", "");
		
	}
	
	function DeletaTermoAjuste( $traid ){
		
		$otaid = $this->carregar( "SELECT otaid FROM obras.obratermoajuste WHERE traid = {$traid}" );
		
		if( $otaid ){
			print "<script>
					alert('Este Termo de Ajuste possui obras encaminhadas e não pode ser excluido!');
					window.location.href = '/obras/obras.php?modulo=principal/lista_de_termos&acao=A';
				  </script>";
			die;
		} else{
			
			$ptaid = $this->carregar( "SELECT ptaid FROM obras.participantetermoajuste WHERE traid = {$traid}" );
			if ( $ptaid ){
				
				foreach( $ptaid as $chave=>$valor ){
					foreach( $valor as $c=>$v ){
						$arr_ptaid[] = $v;
					}
				}
			
				$sql = "DELETE FROM obras.participantetermoajuste where ptaid in (" . implode(",", $arr_ptaid) . ")";
				$this->executar( $sql );
				
			}
			
			$ataid = $this->carregar( "SELECT ataid FROM obras.anexotermoajuste WHERE traid = {$traid}" );
			if ( $ataid ){
				
				foreach( $ataid as $chave=>$valor ){
					foreach( $valor as $c=>$v ){
						$arr_ataid[] = $v;
					}
				}
				
				$sql = "UPDATE obras.anexotermoajuste SET atastatus = 'I' WHERE ataid in (" . implode(",", $arr_ataid) . ")";
				$this->executar( $sql );
			
			}
			
			$arqid = $this->carregar( "SELECT arqid FROM obras.anexotermoajuste WHERE traid = {$traid}" );
			if ( $arqid ){
				
				foreach( $arqid as $chave=>$valor ){
					foreach( $valor as $c=>$v ){
						$arr_arqid[] = $v;
					}
				}
				
				$sql = "UPDATE public.arquivo SET arqstatus = 'I' WHERE arqid in (" . implode(",", $arr_arqid) . ")";
				$this->executar( $sql );
				
			}
			
			if ( $arr_arqid ){
				foreach( $arr_arqid as $chave=>$valor ){
					$caminho = '../../arquivos/obras/' . floor($valor/1000) . '/' . $valor;
					unlink( $caminho );	
				}
			}
			
			$sql = "UPDATE obras.termoajuste SET trastatus = 'I' WHERE traid = {$traid}";
			$this->executar( $sql );
	
			$this->commit();
			$this->sucesso("principal/lista_de_termos", "");
			
			
		}
		
	}
		
	function CadastraParticipante( $entid ){
		
		$sql = "INSERT INTO obras.participantetermoajuste( traid, entidparticipante )
				VALUES ( {$_SESSION["obra"]["traid"]}, {$entid} )";
	
		$this->executar( $sql );
		$this->commit();
		
		print "<script>"
			. "		alert('Operação Realizada com sucesso');"
			. "		window.parent.opener.location.reload();"
			. "		self.close();"
			. "</script>";
	
	}
	
	function CadastraUnidadeParticipante( $entid, $ptaid ){
		
	$sql = "SELECT entidparticipante FROM obras.participantetermoajuste 
				WHERE ptaid = {$ptaid}";
		
		$participanteatual = $this->pegaUm( $sql );
		
		$sql = "SELECT ptaid as id FROM obras.participantetermoajuste 
				WHERE entidunidade = {$entid} AND entidparticipante = {$participanteatual} AND traid = {$_SESSION["obra"]["traid"]}";
		
		$existe = $this->pegaUm( $sql );
		
		if ( $existe ){
			print "<script>
						alert('Este participante já esta inserido no termo com esta unidade!');
						window.parent.opener.location.reload();
						self.close();
				   </script>";
		}else{
			$sql = "UPDATE obras.participantetermoajuste
					SET entidunidade = {$entid}
					WHERE ptaid = {$ptaid}";
			
			$this->executar( $sql );
			$this->commit();
			print "<script>
						alert('Operação realizada com sucesso!');
						window.parent.opener.location.reload();
						self.close();
					</script>";
		}
			
	}
	
	function ExcluiParticipante( $ptaid ){
		
		$sql = "DELETE FROM obras.participantetermoajuste WHERE ptaid = {$ptaid}";
	
		$this->executar( $sql );
		$this->commit();
		$this->sucesso("principal/termodeajuste", "");
		
	}
	
	function ExcluiUnidadeParticipante( $ptaid ){
		
		$sql = "UPDATE obras.participantetermoajuste SET entidunidade = null 
				WHERE ptaid = {$ptaid}";
		
		$this->executar( $sql );
		$this->commit();
		$this->sucesso("principal/termodeajuste", "");
		
	}
	
	function CadastraAnexo( $dados, $arquivo ){
		
		
		$arquivo = $arquivo["arquivo"];
		
		if( $arquivo["type"] == "application/exe"   || $arquivo["type"] == "application/bat" || 
			$arquivo["type"] == "application/x-php" || $arquivo["type"] == "application/x-javascript" ){
            
			print "<script>alert('Não é possível enviar este tipo de arquivo!');</script>";
			return false;
			
		}
		
		//Insere o registro do arquivo na tabela public.arquivo
		$sql = "INSERT INTO public.arquivo (arqnome,arqextensao,arqdescricao,arqtipo,arqtamanho,arqdata,arqhora,usucpf,sisid)
				VALUES('".current(explode(".", $arquivo["name"]))."','".end(explode(".", $arquivo["name"]))."','".$dados["arqdescricao"]."','".$arquivo["type"]."','".$arquivo["size"]."','".date('Y-m-d')."','".date('H:i:s')."','".$_SESSION["usucpf"]."',". $_SESSION["sisid"] .") RETURNING arqid;";
		
		$arqid = $this->pegaUm($sql);

		//Insere o registro na tabela obras.arquivosobra
		$sql = "INSERT INTO obras.anexotermoajuste (arqid, traid, tpaid, usucpf, atastatus, atadtinclusao)
				VALUES(". $arqid .",{$dados["traid"]},{$dados["tpaid"]}, '{$_SESSION["usucpf"]}', 'A','now');";
		$this->executar($sql);

		$caminho = '../../arquivos/obras/' . floor($arqid/1000) . '/';
		
		if( !is_dir($caminho) ) {
			mkdir($caminho, 0777);
		}
		
		move_uploaded_file( $arquivo["tmp_name"], $caminho.$arqid );
				
		$this->commit();
		$this->sucesso("principal/doctermodeajuste", "");

	}

	function DownloadArquivo( $arqid ){
		
		
		$sql ="SELECT * FROM public.arquivo WHERE arqid = {$arqid}";
		$arquivo = current($this->carregar($sql));
		$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arquivo['arqid']/1000) .'/'.$arquivo['arqid'];
		if ( !is_file( $caminho ) ) {
			$_SESSION['MSG_AVISO'][] = "Arquivo não encontrado.";
		}
		$filename = str_replace(" ", "_", $arquivo['arqnome'].'.'.$arquivo['arqextensao']);
		header( 'Content-type: '. $arquivo['arqtipo'] );
		header( 'Content-Disposition: attachment; filename='.$filename);
		readfile( $caminho );
		exit();
		
	}
	
	function DeletaAnexo( $ataid ){
		
		$sql   = "SELECT arqid FROM obras.anexotermoajuste WHERE ataid = {$ataid}";
		$arqid = $this->pegaUm( $sql );
		
		$sql = "UPDATE obras.anexotermoajuste SET atastatus = 'I' WHERE ataid = {$ataid}";
		$this->executar( $sql );
		
		$sql = "UPDATE public.arquivo SET arqstatus = 'I' WHERE arqid = {$arqid}";
		$this->executar( $sql );
		
		$caminho = '../../arquivos/obras/' . floor($arqid/1000) . '/' . $arqid;
		unlink( $caminho );
		
		$this->commit();
		$this->sucesso("principal/doctermodeajuste", "");
		
	}
	
	function CadastraObrasTermo( $dados ){

		foreach( $dados["sel"] as $chave=>$valor ){
			
			$sql   		= "SELECT obrid FROM obras.obratermoajuste WHERE obrid = {$valor} AND traid = {$_SESSION["obra"]["traid"]}";
			$existeobra = $this->pegaUm( $sql );
			
			if ( !$existeobra ){
				$sql = "INSERT INTO obras.obratermoajuste ( traid, obrid ) VALUES ( {$dados["traid"]}, {$valor} )";
				$this->executar( $sql );	
			}
			
		}
		
		$this->commit();
		
		print "<script>"
			. "		alert('Operação realizada com sucesso');"
			. "		window.parent.opener.location.reload();"
			. "		self.close();"
			. "</script>";
		
	}
	
	function listaObras( $entid ){
		
		$sql = "SELECT
					ot.otaid as acao,
					oi.obrdesc as nome,
					to_char(oi.obrdtinicio,'DD/MM/YYYY') as inicio,
			        to_char(oi.obrdttermino,'DD/MM/YYYY') as final,
			        CASE WHEN oi.stoid is not null THEN sto.stodesc ELSE 'Não Informado' END as situacao,
			        '' as prazo,
			        '' as situacaotermo,
			        '' as obs
				FROM
					obras.obrainfraestrutura oi
				INNER JOIN
					obras.obratermoajuste ot ON ot.obrid = oi.obrid
				LEFT JOIN
					obras.situacaoobra sto ON sto.stoid = oi.stoid
				WHERE
					oi.entidunidade = {$entid} AND
					ot.traid = {$_SESSION["obra"]["traid"]}";
		
		$cabecalho = array( "Ação", "Nome da Obra" );
		$this->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%');
		
	}
	
	function PegaSituacaoTermo( $entid ){
		
		$sql = "SELECT DISTINCT
					staid as codigo,
					stadsc as descricao
				FROM
					obras.situacaotermoajuste";
		
		$combo = $this->carregar( $sql );
		
		$combo_st .= "<option value=\'\'>Selecione...</option>";
		
		for( $i = 0; $i < count($combo); $i++ ){
			$combo_st .= "<option value=\'" . $combo[$i]['codigo'] . "\' " . ( $combo[$i]["obra"] != null ? "selected=selected" : "" ) . " >" . $combo[$i]['descricao'] . "</option>";
		}

		return $combo_st;
		
	}
	
	function AtualizaObrasTermo( $dados ){

		
		foreach( $dados as $chave=>$valor ){
			if ( strpos($chave, '_') ){
				
				$posicao = strpos($chave, '_');
				$otaid   = substr($chave, $posicao + 1);
				
				$valor = !empty($valor) ? "'" . formata_data_sql( trim($valor) ) . "'" : 'null';
			
				$sql .= "UPDATE obras.obratermoajuste 
						 SET otadtprazo = {$valor} 
						 WHERE otaid = {$otaid}; \n";
				
			}
		}
		
		$this->executar( $sql );
		
		foreach( $dados["staid"] as $chave=>$valor ){
			
			$valor = !empty($valor) ? trim($valor) : "null";
			
			$sql .= "UPDATE obras.obratermoajuste 
					 SET staid = " . $valor . "
					 WHERE otaid = {$chave}; \n";	
			
		}
		
		$this->executar( $sql );
		
		$this->commit();
		$this->sucesso("principal/obratermodeajuste", "");
		
	}
	
	function CadastraObsObrasTermo( $dados ){
		
		$dados["otaobs"] = self::trataDados( $dados["otaobs"] );
		
		$sql = "UPDATE obras.obratermoajuste SET otaobs = {$dados["otaobs"]} 
			   WHERE otaid = {$dados["otaid"]};";
		
		$this->executar( $sql );
		$this->commit();

		print "<script>"
			. "		alert('Operação realizada com sucesso');"
			. "		window.parent.opener.location.reload();"
			. "		self.close();"
			. "</script>";
		
	}
	
	function ExcluiObrasTermo( $otaid ){
		
		$sql = "DELETE FROM obras.obratermoajuste WHERE otaid = {$otaid}";
		$this->executar( $sql );
		
		$this->commit();
		$this->sucesso("principal/obratermodeajuste", "");
		
	}
	
	function MontaDetalhesTermo( $traid ){
		
		$sql = "SELECT *, u.usunome as nome
				FROM obras.termoajuste ta
				INNER JOIN seguranca.usuario u ON u.usucpf = ta.usucpf 
			    WHERE ta.traid = {$traid}";
		
		$termo = $this->pegaLinha( $sql );
		
		foreach( $termo as $chave=>$valor ){
			if ( $valor == null ){
				$termo[$chave] = "Não Informado";
			}
		}
		
		$sql = "SELECT ee2.entnome as unidade, ee.entnome as participante
				FROM obras.participantetermoajuste pt
				INNER JOIN entidade.entidade ee ON pt.entidparticipante = ee.entid
				LEFT JOIN entidade.entidade ee2 ON pt.entidunidade = ee2.entid
				WHERE traid = {$traid}";
		
		$participante = $this->carregar( $sql );
		
		if( $participante ){
			for ( $i = 0; $i < count($participante); $i++ ){
				if ( $participante[$i]["unidade"] == '' ){
					$participante[$i]["unidade"] = 'Não Informado';
				}
			}
			
		}

		$sql = "SELECT 
					CASE WHEN ot.staid is not null THEN stadsc ELSE 'Não Informado' END as situacao, 
					CASE WHEN ot.otadtprazo is not null THEN to_char(otadtprazo, 'DD/MM/YYYY') ELSE 'Não Informado' END as data, 
					CASE WHEN ot.otaobs <> '' THEN otaobs ELSE 'Não Informado' END as obs
				FROM obras.obratermoajuste ot
				LEFT JOIN obras.situacaotermoajuste st ON st.staid = ot.staid
				WHERE traid = {$traid} and obrid = {$_SESSION["obra"]["obrid"]}";
		
		$obra = $this->pegaLinha( $sql );
		
		print '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">'
			. '		<tr>'
			. '			<td class="subtitulocentro" colspan="2">Dados do Termo de Ajuste</td>'
			. '		</tr>'
			. '		<tr>'
			. '			<td class="subtitulodireita" width="130px">Assunto</td>'
			. '			<td>'. $termo["traassunto"] .'</td>'
			. '		</tr>'
			. '		<tr>'
			. '			<td class="subtitulodireita">Local</td>'
			. '			<td>'. $termo["tralocal"] .'</td>'
			. '		</tr>'
			. '		<tr>'
			. '			<td class="subtitulodireita">Data de Criação</td>'
			. '			<td>'. formata_data($termo["tradtcriacao"]) .'</td>'
			. '		</tr>'
			. '		<tr>'
			. '			<td class="subtitulodireita">Texto da Ata</td>'
			. '			<td align="justify">'. $termo["tratextoata"] .'</td>'
			. '		</tr>'
			. '		<tr>'
			. '			<td class="subtitulodireita">Inserido Por</td>'
			. '			<td>'. $termo["nome"] .'</td>'
			. '		</tr>'
			. '		<tr>'
			. '			<td class="subtitulocentro" colspan="2">Participantes</td>'
			. '		</tr>';
		
		if ( !$participante ){
			print '<tr><td colspan="2"> Não Existem Participantes Associados </td></tr>';
		}else{
			for ( $i = 0; $i < count($participante); $i++ ){
				print '<tr><td colspan="2">'
					. '	<table width="98%" cellSpacing="1" cellPadding="3" align="center" style="border:1px solid #ccc; background-color:#f5f5f5;">'
				    . ' 	<tr>'
					. '			<td class="subtitulodireita" width="130px">Nome</td>'
					. '			<td>'. $participante[$i]["participante"] .'</td>'
					. '		</tr>'
					. '		<tr>'
					. '			<td class="subtitulodireita" width="130px">Unidade</td>'
					. '			<td>'. $participante[$i]["unidade"] .'</td>'
					. '		</tr>'
					. ' </table>'
					. '</td></tr>';
			}
		}
		
		print '		<tr>'
			. '			<td class="subtitulocentro" colspan="2">Informações Sobre a Obra no Termo</td>'
			. '		</tr>'
			. '		<tr>'
			. '			<td class="subtitulodireita" width="130px">Situação Atual</td>'
			. '			<td>'. $obra["situacao"] .'</td>'
			. '		</tr>'
			. '		<tr>'
			. '			<td class="subtitulodireita" width="130px">Prazo</td>'
			. '			<td>'. $obra["data"] .'</td>'
			. '		</tr>'
			. '		<tr>'
			. '			<td class="subtitulodireita" width="130px">Observação</td>'
			. '			<td alig="justify">'. $obra["obs"] .'</td>'
			. '		</tr>'	
			. '</table>';
		
	}
	
}


Class licitacao extends ControllerData{

	public $molid			  = null;
	public $moldias           = null;
	public $dtiniciolicitacao = null;
	public $dtfinallicitacao  = null;
	public $licitacaouasg 	  = null;
	public $numlicitacao 	  = null;
	
	function __construct(){
		parent::__construct();
	}

	public function dados($dados){
		$this->molid 			 = $dados['molid'];
		$this->moldias 			 = $dados['moldias'];
		$this->dtiniciolicitacao = $dados['dtiniciolicitacao'];
		$this->dtfinallicitacao  = $dados['dtfinallicitacao'];
		$this->licitacaouasg 	 = $dados['licitacaouasg'];
		$this->numlicitacao 	 = $dados['numlicitacao'];
	}

	public function busca( $id ){
		$result = $this->simec->executar("SELECT 
											oi.molid,
											moldias, 
											dtiniciolicitacao, 
											dtfinallicitacao, 
											licitacaouasg, 
											numlicitacao 
										  FROM 
										    obras.obrainfraestrutura oi
										  INNER JOIN
										  	obras.modalidadelicitacao ml ON ml.molid = oi.molid 
										  WHERE 
										    obrid = {$id}");
		return pg_fetch_assoc($result);
	}

	public function cadastraLicitacao( $obra ){
		
		// verifica a sessï¿½o da obra
		obras_verifica_sessao();

		$insert_dados = array();
		$flcid_tela   = array();
		$flcid_banco  = array();
		$sql_insert   = array();

		if( is_array($obra['tflid']) ){
			
			foreach( $obra['tflid'] as $key => $item ){
				
				$flcpubleditaldtprev = $obra['flcpubleditaldtprev'][$key] ? "'" . formata_data_sql($obra['flcpubleditaldtprev'][$key]) . "'" : 'null';
				$flcdtrecintermotivo = $obra['flcdtrecintermotivo'][$key] ? "'" . formata_data_sql($obra['flcdtrecintermotivo'][$key]) . "'" : 'null';
				$flcrecintermotivo 	 = $obra['flcrecintermotivo'][$key]   ? "'" . $obra['flcrecintermotivo'][$key] . "'" 					 : 'null';
				$flcordservdt 		 = $obra['flcordservdt'][$key] 		  ? "'" . formata_data_sql($obra['flcordservdt'][$key])	   . "'" 					 : 'null';
				$flcordservnum 		 = $obra['flcordservnum'][$key] 	  ? "'" . $obra['flcordservnum'][$key]	   . "'" 					 : 'null';
				$flchomlicdtprev 	 = $obra['flchomlicdtprev'][$key] 	  ? "'" . formata_data_sql($obra['flchomlicdtprev'][$key]) . "'"	 : 'null';
				$flcaberpropdtprev 	 = $obra['flcaberpropdtprev'][$key]   ? "'" . formata_data_sql($obra['flcaberpropdtprev'][$key]) . "'"	 : 'null';
				
				$_sql = "";

				if( !is_numeric($obra['flcid'][$key]) ){
					
					$_sql = "INSERT INTO obras.faselicitacao ( tflid, 
															   obrid, 
															   flcstatus,
															   flcpubleditaldtprev,
															   flcdtrecintermotivo,
															   flcrecintermotivo,
															   flcordservdt,
															   flcordservnum,
															   flchomlicdtprev,
															   flcaberpropdtprev)
													  VALUES ( {$item}, 
															   {$_SESSION["obra"]["obrid"]}, 
															   'A',
															   {$flcpubleditaldtprev},
															   {$flcdtrecintermotivo},
															   {$flcrecintermotivo},
															   {$flcordservdt},
															   {$flcordservnum},
															   {$flchomlicdtprev},
															   {$flcaberpropdtprev} )";
 					
					$this->simec->executar($_sql);
					
				}
				
			}
			
		}

		if( is_array($obra['acaoFases']) ){
			
			foreach( $obra['acaoFases'] as $key => $item ){
				
				if( is_numeric($item) ){
					
					$_sql  = "";
					$_sql .= "UPDATE obras.faselicitacao SET flcstatus ='I' WHERE flcid = {$item}";
					$this->simec->executar($_sql);
					
				}
				
			}
			
		}

		$obra["dtiniciolicitacao"] = $obra["dtiniciolicitacao"] != "NULL" ? formata_data_sql($obra["dtiniciolicitacao"]) : $obra["dtiniciolicitacao"];
		$obra["dtfinallicitacao"]  = $obra["dtfinallicitacao"] != "NULL" ? formata_data_sql($obra["dtfinallicitacao"]) : $obra["dtfinallicitacao"];
	
		foreach( $obra as $campo=>$valor ){
			if ( !is_array($valor) ){
				if( !trim($valor) ){
					$obra[$campo] = "NULL";
				} else {
					$obra[$campo] = "'" . pg_escape_string(trim($valor))  .  "'";
				}
			}
		}
		
		$sql = "UPDATE 
					obras.obrainfraestrutura 
				SET 
					molid 			  = {$obra["molid"]},
					dtiniciolicitacao = {$obra["dtiniciolicitacao"]},
					dtfinallicitacao  = {$obra["dtfinallicitacao"]},
					licitacaouasg 	  = {$obra["licitacaouasg"]},
					numlicitacao 	  = {$obra["numlicitacao"]} 
				WHERE 
					obrid = {$_SESSION["obra"]["obrid"]}";
		
		$this->simec->executar($sql);
		
		$this->simec->commit();
		$this->simec->sucesso("principal/licitacao");
		
	}
	
}


//-------- SUPERVISï¿½O -------- //

/**
 * Classe que controla as aï¿½ï¿½os das Supervisï¿½es (vistorias) das obras pelas 
 * empresas contratadas p/ este fim
 * @author Fernando Bagno <fernandosilva@mec.gov.br>
 * @since 16/03/2010
 * @version 1.0
 *
 */
class supervisao extends obraPai{
		
	/**
	 * Busca quais os tipo de ensino o usuï¿½rio possui permissï¿½o para visualizar
	 * @author Fernando Bagno <fernandosilva@mec.gov.br>
	 * @since 16/03/2010 
	 * @return array
	 * 
	 */
	function obrBuscaTipoEnsinoResp( ){
		
		global $db;
		
		$arTipoEnsino = array( ORGAO_SESU, 
							   ORGAO_SETEC, 
							   ORGAO_FNDE );
		
		if( !$db->testa_superuser() && !possuiPerfil(PERFIL_SAA) ){
			
			$sql = "SELECT DISTINCT
						o.orgid
					FROM
						obras.usuarioresponsabilidade ur 
					LEFT JOIN 
						obras.orgao o ON ur.orgid = o.orgid
					WHERE
						ur.usucpf = '{$_SESSION["usucpf"]}' AND
						ur.rpustatus = 'A'";
			
			
			$arTipoEnsino = $db->carregarColuna( $sql ); 
			
		}
							   
		return $arTipoEnsino;
		
	}
	
	/**
	 * Monta as abas com os tipo de ensino de responsabilidade do usuï¿½rio na pï¿½gina
	 * inicial do mï¿½dulo
	 * @author Fernando Bagno <fernandosilva@mec.gov.br>
	 * @since 16/03/2010 
	 * @param array $tipos
	 * @return mixed
	 * 
	 */
	function obrMontaAbasTipoEnsino( $tipos, $orgid = "" ){
		
		$arItensMenu = array();
		
		$orgidPrincipal = $orgid ? $orgid : 1;
		
		if( is_array( $tipos ) ){
			
			// cria o array com os as abas de menu
			foreach( $tipos as $tipoensino ){
				
				// atribui as descriï¿½ï¿½es dos menus 
				switch( $tipoensino ){
					case 1:
						$descricao = "Educação Superior";
					break;
					case 2:
						$descricao = "Educação Profissional";
					break;
					case 3:
						$descricao = "Educação Básica";
					break;
					
				}
	
				// insere os dados de menu no array
				array_push( $arItensMenu, array( "descricao" => $descricao,
												 "link"      => "obras.php?modulo=principal/supervisao/repositorio&acao=A&orgid={$tipoensino}" ) );
				
			}
			
		}
		
		return montarAbasArray( $arItensMenu, "obras.php?modulo=principal/supervisao/repositorio&acao=A&orgid={$orgidPrincipal}" );
		
	}
	
	/**
	 * Monta as abas com os tipo de ensino de responsabilidade do usuï¿½rio na pï¿½gina
	 * inicial do mï¿½dulo
	 * @author Rodrigo Pereira de Souza Silva <rodrigossilva@mec.gov.br>
	 * @since 23/09/2010 
	 * @param array $tipos
	 * @return mixed
	 * 
	 */
	function obrMontaAbasTipoEnsino2( $tipos, $orgid = "" ){
		
		$arItensMenu = array();
		
		$orgidPrincipal = $orgid ? $orgid : 1;
		
		if( is_array( $tipos ) ){
			
			// cria o array com os as abas de menu
			foreach( $tipos as $tipoensino ){
				
				// atribui as descriï¿½ï¿½es dos menus 
				switch( $tipoensino ){
					case 1:
						$descricao = "Educação Superior";
					break;
					case 2:
						$descricao = "Educação Profissional";
					break;
					case 3:
						$descricao = "Educação Básica";
					break;
					
				}
	
				// insere os dados de menu no array
				array_push( $arItensMenu, array( "descricao" => $descricao,
												 "link"      => "obras.php?modulo=principal/supervisao/supervisoesFinalizadas&acao=A&orgid={$tipoensino}" ) );
				
			}
			
		}
		
		return montarAbasArray( $arItensMenu, "obras.php?modulo=principal/supervisao/supervisoesFinalizadas&acao=A&orgid={$orgidPrincipal}" );
		
	}	
	
	function obrExibeMsgErro( $msg ){
		
		print "<script>"
			. "    alert( '{$msg}' );"
			. "    history.back(-1);"
			. "</script>";

		die;
			
	}
	
	function obrVerficaDadoRequisicao( $dado, $nomeTabela, $campoWhere, $status = ''){
		
		$sql = "SELECT {$campoWhere} FROM obras.{$nomeTabela} WHERE {$campoWhere} = {$dado}" . ( !empty($status) ? " AND " . $status : "" );
		return $this->db->pegaUm( $sql );
		
	}
	
	/**
	 * Cria o filtro da lista de obras de acordo com as informaï¿½ï¿½es passadas
	 * @author Fernando Bagno <fernandosilva@mec.gov.br>
	 * @since 16/03/2010 
	 * @return string
	 * 
	 */
	function obrFiltraListaRepositorio(){
		
//		$filtro  = $_REQUEST["entidunidade"] 	   ? " AND entidunidade = {$_REQUEST["entidunidade"]}" : "";
//		$filtro .= $_REQUEST["obrdesc"] 	 	   ? " AND obrdesc ilike '%{$_REQUEST["obrdesc"]}%'"   : "";
		$filtro .= $_REQUEST["estuf"] 		 	   ? " AND ed.estuf = '{$_REQUEST["estuf"]}'" 		   : "";
//		$filtro .= $_REQUEST["stsid"] 		 	   ? " AND ore.stsid = {$_REQUEST["stsid"]}" 		   : "";
		
		if($_REQUEST["esdid"]){
			
			if($_REQUEST["esdid"] != OBRSITSUPREPOSITORIO){
				$filtro .= " AND we.esdid = {$_REQUEST["esdid"]}";
			}else{
				$filtro .= " AND we.esdid is null";
			}
			
		}
		
		//if(!empty($_REQUEST["esdidobra"])){
		//	$filtro .= " AND west.esdid = {$_REQUEST["esdidobra"]}";
		//}
		
		if(!empty($_REQUEST["esdidobra"])){
			switch ($_REQUEST["esdidobra"]){
						
				//Situaï¿½ï¿½o da Obra "Em Repositï¿½rio".
				case $_REQUEST["esdidobra"] == OBRSITSUPREPOSITORIO :
					$filtro .= " AND we.esdid IS NULL AND west.esdid IS NULL";  
				break;
				//Situaï¿½ï¿½o da Obra "Distribuï¿½da".
				case $_REQUEST["esdidobra"] == OBRSITSUPDISTRIBUIDA :
					$filtro .= " AND we.esdid IS NOT NULL  AND west.esdid IS NULL ";
				break;
				//Demais Situaï¿½ï¿½es.
				case (!empty($_REQUEST["esdidobra"])):
					$filtro .= " AND west.esdid = {$_REQUEST["esdidobra"]}";
				break;		
			}
		}
		
//		$filtro .= $_POST["esdid"] 		 	   	   ? " AND we.esdid = {$_POST["esdid"]}" 		   	   : " AND we.esdid is null";
		
		$filtro .= $_REQUEST["repdtlimiteinicial"] ? " AND ore.repdtlimiteinicial = '" . formata_data_sql($_REQUEST["repdtlimiteinicial"]) . "'" : "";
		$filtro .= $_REQUEST["repdtlimitefinal"]   ? " AND ore.repdtlimitefinal = '" . formata_data_sql($_REQUEST["repdtlimitefinal"]) . "'" 	 : "";
		$filtro .= $_REQUEST["repdtlimitefinal"]   ? " AND ore.repdtlimitefinal = '" . formata_data_sql($_REQUEST["repdtlimitefinal"]) . "'" 	 : "";
		
		$_SESSION["obras"]["filtros"] = null;
		
		if( !empty( $_REQUEST["tobaid"] ) ){
			
			$_SESSION["obras"]["filtros"]["tobaid"] = $_REQUEST["tobaid"];
			$filtro .= " AND oi.tobraid = {$_REQUEST["tobaid"]} ";
			
		}
		 
		if( !empty( $_REQUEST["stoid"] ) ){
			
			$_SESSION["obras"]["filtros"]["stoid"] = $_REQUEST["stoid"];
			$filtro .= " AND oi.stoid = {$_REQUEST["stoid"]} ";
			
		}
		 
		if( !empty( $_REQUEST["cloid"] ) ){
			
			$_SESSION["obras"]["filtros"]["cloid"] = $_REQUEST["cloid"];
			$filtro .= " AND oi.cloid = {$_REQUEST["cloid"]} ";
			
		}
		 
		if( !empty( $_REQUEST["prfid"] ) ){
			
			$_SESSION["obras"]["filtros"]["prfid"] = $_REQUEST["prfid"];
			$filtro .= " AND oi.prfid = {$_REQUEST["prfid"]} ";
			
		}
		/* 
		if( !empty( $_REQUEST["entidunidade"] ) ){
			
			$_SESSION["obras"]["filtros"]["entidunidade"] = $_REQUEST["entidunidade"];
			$filtro .= " AND oi.entidunidade = {$_REQUEST["entidunidade"]} ";
			
		}
		 */
		if( !empty( $_REQUEST["obrtextobusca"] ) ){
			
			$_SESSION["obras"]["filtros"]["obrtextobusca"] = $_REQUEST["obrtextobusca"];
			
			$filtro .= " AND ( upper(oi.obrdesc) ilike upper('%{$_REQUEST["obrtextobusca"]}%') OR ";
			$filtro .= " upper(ee.entnome) ilike upper('%{$_REQUEST["obrtextobusca"]}%') OR ";
			$filtro .= " upper(tm.mundescricao) ilike upper('%{$_REQUEST["obrtextobusca"]}%') OR ";
			$filtro .= " upper(oi.obrdesc) ilike upper('%{$_REQUEST["obrtextobusca"]}%') OR ";
			$filtro .= " oi.obrid =".(int)$_REQUEST["obrtextobusca"]." ) "; // busca pelo campo ID
			
		}
		
		$_SESSION["obras"]["filtros"]["foto"] = $_REQUEST["foto"];
		
		switch( $_REQUEST["foto"] ){
			
			case "S":
				$filtro .= " AND af.obrid IS NOT NULL ";
			break;
				
			case "N":
				$filtro .= " AND af.obrid IS NULL ";
			break;
			
		}
		
		$_SESSION["obras"]["filtros"]["vistoria"] = $_REQUEST["vistoria"];
	
		switch( $_REQUEST["vistoria"] ){
			
			case "S":
				$filtro .= " AND ov.obrid IS NOT NULL ";
			break;
				
			case "N":
				$filtro .= " AND ov.obrid IS NULL ";
			break;
			
		}
		
		$_SESSION["obras"]["filtros"]["restricao"] = $_REQUEST["restricao"];
	
		switch( $_REQUEST["restricao"] ){
			
			case "S":
				$filtro .= " AND re.obrid IS NOT NULL ";
			break;
				
			case "N":
				$filtro .= " AND re.obrid IS NULL ";
			break;
			
		}
		
		$_SESSION["obras"]["filtros"]["aditivo"] = $_REQUEST["aditivo"];
		
		switch( $_REQUEST["aditivo"] ){
			
			case "S":
				$filtro .= " AND obridaditivo IS NOT NULL";
			break;
				
			case "N":
				$filtro .= " AND obridaditivo IS NULL";
			break;
			
		}
		
		if ( $_REQUEST["percentualinicial"] > '0' ) {
			
			$_SESSION["obras"]["filtros"]["percentualinicial"] = $_REQUEST["percentualinicial"];
			$_SESSION["obras"]["filtros"]["percentualfinal"]   = $_REQUEST["percentualfinal"];
			
			$perc = $_REQUEST["percentualfinal"] == 100 ? 110 : $_REQUEST["percentualfinal"];
			$filtro .= " AND ( total_exec BETWEEN {$_REQUEST["percentualinicial"]} AND {$perc})";
			
		}elseif ($_REQUEST["percentualinicial"] == '0') {
			if ( $_REQUEST["percentualfinal"] > '0' ) {
				
				$_SESSION["obras"]["filtros"]["percentualinicial"] = $_REQUEST["percentualinicial"];
				$_SESSION["obras"]["filtros"]["percentualfinal"]   = $_REQUEST["percentualfinal"];

				$perc = $_REQUEST["percentualfinal"] == 100 ? 110 : $_REQUEST["percentualfinal"];
				$filtro .= " AND ( total_exec IS NULL OR total_exec BETWEEN {$_REQUEST["percentualinicial"]} AND {$perc})";
			
			}elseif ( $_REQUEST["percentualfinal"] == '0' ) {
				
				$_SESSION["obras"]["filtros"]["percentualinicial"] = $_REQUEST["percentualinicial"];
				$_SESSION["obras"]["filtros"]["percentualfinal"]   = $_REQUEST["percentualfinal"];
				
				$filtro .= " AND ( total_exec = 0 OR total_exec IS NULL )";
				
			}
		}
		
		return $filtro;
		
	}
	
	/**
	 * Monta a lista de obras que estï¿½o inseridas no repositï¿½rio
	 * @author Fernando Bagno <fernandosilva@mec.gov.br>
	 * @since 16/03/2010 
	 * @param integer $orgid
	 * @return mixed
	 * 
	 */
	function obrListaObrasRepositorio( $orgid, $filtros, $tipo = "repositorio" ){
		
		global $db;
		
		switch( $tipo ){
			
			case "repositorio":
				if($orgid == '3' ){
					$obrid = 'oi.obrid,';
					$numconvenio = 'oi.numconvenio,';
				}else{
					$obrid = 'oi.obrid,';
				}
				
				if( possuiPerfil( PERFIL_SUPERVISORMEC ) && !$db->testa_superuser() ):
					$btExcluir    = "'<center><img src=\"../imagens/excluir_01.gif\" onclick=\"\" style=\"cursor:pointer;\" title=\"Excluir obra\"/></center>'";
				else:
					$btExcluir    = "'<center><img src=\"../imagens/excluir.gif\" onclick=\"obrExcluiObraRepositorio( ' || oi.obrid || ' );\" style=\"cursor:pointer;\" title=\"Excluir obra\"/></center>'";
				endif;
					
				$btNaoExcluir = "'<center><img src=\"../imagens/excluir_01.gif\" title=\"Esta obra não pode excluída do repositório!\"/></center>'";
				
				$select = " CASE WHEN ore.stsid = " . OBRSITSUPREPOSITORIO . "  THEN {$btExcluir} ELSE {$btNaoExcluir} END as acao,
							{$obrid}
							--obrdesc 
							'<a onclick=\"obrIrParaCaminho(\'' || oi.obrid || '\',\'cadastro\');\">' || upper(oi.obrdesc) || '</a>'/* Nome da Obra com Link para a tela Dados da Obra */ 
							as nome,
							{$numconvenio}
							ee.entnome as unidade,
							tm.mundescricao || ' / ' || ed.estuf as mun,
							CASE WHEN obrdtinicio is not null THEN to_char(obrdtinicio, 'DD/MM/YYYY') ELSE 'Não Informado' END as inicio,
							CASE WHEN obrdttermino is not null THEN to_char(obrdttermino, 'DD/MM/YYYY') ELSE 'Não Informado' END as termino,
							stodesc as situacao,
							--(SELECT coalesce(SUM(icopercexecutado), 0.00 ) as total FROM obras.itenscomposicaoobra WHERE obrid = oi.obrid) as percentual,
							CASE WHEN 
								(
									SELECT  
										MAX(coalesce((SELECT 
													sum(( icopercsobreobra * supvlrinfsupervisor ) / 100)
												  FROM 
												obras.itenscomposicaoobra i
												  INNER JOIN 
													obras.supervisaoitenscomposicao si ON i.icoid = si.icoid WHERE si.supvid = s.supvid AND obrid = oi.obrid AND i.icovigente = 'A' ),'0') ) as percentual
									FROM
										obras.supervisao s
									INNER JOIN 
										obras.situacaoobra si ON si.stoid = s.stoid
									LEFT JOIN
										obras.realizacaosupervisao rs ON rs.rsuid = s.rsuid 
									WHERE
										s.obrid = oi.obrid AND
										s.supstatus = 'A'
								) > 100 THEN 100
								 ELSE
									CASE WHEN
										(
											SELECT  
												MAX( coalesce((SELECT 
															sum(( icopercsobreobra * supvlrinfsupervisor ) / 100)
														  FROM 
														obras.itenscomposicaoobra i
														  INNER JOIN 
															obras.supervisaoitenscomposicao si ON i.icoid = si.icoid WHERE si.supvid = s.supvid AND obrid = oi.obrid AND i.icovigente = 'A' ),'0') ) as percentual
											FROM
												obras.supervisao s
											INNER JOIN 
												obras.situacaoobra si ON si.stoid = s.stoid
											LEFT JOIN
												obras.realizacaosupervisao rs ON rs.rsuid = s.rsuid 
											WHERE
												s.obrid = oi.obrid AND
												s.supstatus = 'A'
										 ) is null THEN 0
									ELSE
										(
											SELECT  
												MAX( coalesce((SELECT 
															sum(( icopercsobreobra * supvlrinfsupervisor ) / 100)
														  FROM 
														obras.itenscomposicaoobra i
														  INNER JOIN 
															obras.supervisaoitenscomposicao si ON i.icoid = si.icoid WHERE si.supvid = s.supvid AND obrid = oi.obrid AND i.icovigente = 'A' ),'0') ) as percentual
											FROM
												obras.supervisao s
											INNER JOIN 
												obras.situacaoobra si ON si.stoid = s.stoid
											LEFT JOIN
												obras.realizacaosupervisao rs ON rs.rsuid = s.rsuid 
											WHERE
												s.obrid = oi.obrid AND
												s.supstatus = 'A'
										 )
									END
							END as percentual,
							CASE WHEN repdtlimiteinicial is not null THEN to_char(repdtlimiteinicial, 'DD/MM/YYYY') ELSE 'Não Informado' END as inicial,
							CASE WHEN repdtlimitefinal is not null THEN to_char(repdtlimitefinal, 'DD/MM/YYYY') ELSE 'Não Informado' END as final,
							CASE WHEN gd.gpdid is not null THEN we.esddsc ELSE '<center> - </center>' END as supervisao,
							CASE WHEN west.esdid is not null THEN west.esddsc 
							     WHEN we.esdid is not null  AND west.esdid is null THEN 'Distribuída'	
							ELSE 'Em Repositório' END as supervisao_obra,
							CASE WHEN obrdtvistoria IS NOT NULL THEN 
									'<div style=\"display:none\">'||obrdtvistoria||'</div>'  
								 ELSE 
								 	'<div style=\"display:none\">'||obsdtinclusao||'</div>' 
							END
							|| '<FONT ' ||
							CASE WHEN oi.stoid IN (1, 2) THEN
									CASE WHEN obrdtvistoria IS NOT NULL THEN 
											CASE WHEN DATE_PART('days', NOW() - obrdtvistoria) <= 45 THEN
													'COLOR=\"#00AA00\" TITLE=\"Esta obra foi atualizada em até 45 dias\">' 
												 WHEN DATE_PART('days', NOW() - obrdtvistoria) > 45 AND DATE_PART('days', NOW() - obrdtvistoria) <= 60 THEN
													'COLOR=\"#BB9900\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">' 
												 WHEN DATE_PART('days', NOW() - obrdtvistoria) > 60 THEN
													'COLOR=\"#DD0000\" TITLE=\"Esta obra está desatualizada\">' 
											END
											|| to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS')
								 		 ELSE 
								 			CASE WHEN DATE_PART('days', NOW() - obsdtinclusao) <= 45 THEN
													'COLOR=\"#00AA00\" TITLE=\"Esta obra foi atualizada em até 45 dias\">' 
												 WHEN DATE_PART('days', NOW() - obsdtinclusao) > 45 AND DATE_PART('days', NOW() - obsdtinclusao) <= 60 THEN
													'COLOR=\"#BB9900\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">' 
												 WHEN DATE_PART('days', NOW() - obsdtinclusao) > 60 THEN
													'COLOR=\"#DD0000\" TITLE=\"Esta obra está desatualizada\">' 
											END
											|| to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS') 
									END
								 WHEN oi.stoid IN (3) THEN
								  	'COLOR=\"#0066CC\" TITLE=\"Esta obra foi concluída\">' || COALESCE(to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS'), to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS'))
								 ELSE
								 	'COLOR=\"#000000\" TITLE=\" \">' ||
									CASE WHEN obrdtvistoria IS NOT NULL THEN 
											to_char(obrdtvistoria, 'DD/MM/YYYY HH24:MI:SS') 
								 		 ELSE 
								 			to_char(obsdtinclusao, 'DD/MM/YYYY HH24:MI:SS') 
									END 
							END 
							|| '</FONT>' as atualizacao";

							
				if($orgid == '3' ){
				$cabecalho = array( "Ação","Id","Nome da Obra","Convênio", "Unidade Responsável pela Obra", "Município / UF", "Data de Início", "Data de Término", "Situação da Obra","% Executado", "Data Limite <br/> Inical da Supervisão", "Data Limite <br/> Final da Supervisão", "Situação da Supervisão (Grupo)", "Situação da Supervisão da Obra (Empresa)","Última Atualização" );
				} else{
				$cabecalho = array( "Ação","Id","Nome da Obra","Unidade Responsável pela Obra", "Município / UF", "Data de Início", "Data de Término", "Situação da Obra","% Executado", "Data Limite <br/> Inical da Supervisão", "Data Limite <br/> Final da Supervisão", "Situação da Supervisão (Grupo)", "Situação da Supervisão da Obra (Empresa)","Última Atualização" );
				}
			break;
			
			case "lote":
				
				$select = "'<center><input type=\"checkbox\" id=\"repid_' || ore.repid || '\" name=\"repid\" value=\"' || ore.repid || '\" onclick=\"obrIncluiObraNoLote( ' || oi.obrid || ', ' || ore.repid || ', \'' || replace(obrdesc,'\"', '') || '\', \'' ||  replace(tm.mundescricao,'''', '') || '\', \'' || obrqtdconstruida || '\', \'' || umdeesc || '\', \'' || ee.entnome || '\', \'' || stodesc || '\', \'' || ed.estuf || '\', \'' || oo.orgdesc || '\',  \'' || CASE WHEN oc.covid IS NOT NULL THEN covnumero ELSE (CASE WHEN numconvenio is not null THEN numconvenio ELSE 'Não Informado' END) END || '\', \'' || coalesce(obrpercexec,0.00) || '\' );\" /></center>' as acao,
							'<img src=\"../imagens/consultar.gif\" style=\"vertical-align:middle; cursor: pointer;\" title=\"Dados da Obra\" onclick=\"obrVerDados(' || oi.obrid || ', \'obra\');\"/> 
							 <img src=\"../imagens/globo_terrestre.png\" style=\"vertical-align:middle; cursor: pointer;\" title=\"Visualizar Mapa\" onclick=\"janela(\'?modulo=principal/supervisao/mapaObra&acao=A&obrid=' || oi.obrid || '\', 600, 585, \'mapaGrupo\');\"/> (' || oi.obrid || ') ' || obrdesc || ' (nº do convênio: ' || CASE WHEN oc.covid IS NOT NULL THEN covnumero ELSE (CASE WHEN numconvenio is not null THEN numconvenio ELSE 'Não Informado' END) END ||  ') - ' || tm.mundescricao as obrdesc,
							orgdesc || ' - ' || ee.entnome as unidade";
				
				$cabecalho = array( "Ação", 
									"Nome da Obra" );
				
				if( !$_SESSION["obras"]["gpdid"] ){
					$where = "ore.repid not in ( SELECT repid FROM obras.itemgrupo ) AND ";
				}else{
					$where = "ore.repid not in ( SELECT repid FROM obras.itemgrupo WHERE gpdid <> {$_SESSION["obras"]["gpdid"]} ) AND ";
				}
				
			break;
			
		}
		
		$sql = "SELECT DISTINCT
					 {$select}
				FROM
					obras.obrainfraestrutura oi
				INNER JOIN
						obras.repositorio ore ON ore.obrid = oi.obrid
				LEFT JOIN
				    	obras.itemgrupo ig ON ig.repid = ore.repid
				LEFT JOIN
				    	obras.grupodistribuicao gd ON gd.gpdid = ig.gpdid
				LEFT JOIN
				    	workflow.documento wd ON wd.docid = gd.docid
				LEFT JOIN
				    	workflow.estadodocumento we ON we.esdid = wd.esdid
				INNER JOIN
				    	entidade.entidade ee ON ee.entid = oi.entidunidade
				INNER JOIN
				    	obras.situacaoobra so ON so.stoid = oi.stoid
				INNER JOIN
				    	entidade.endereco ed ON ed.endid = oi.endid
				INNER JOIN
				    	territorios.municipio tm ON tm.muncod = ed.muncod
				LEFT JOIN
				    	obras.unidademedida ou ON ou.umdid = oi.umdidobraconstruida
				INNER JOIN
				    	obras.orgao oo ON oo.orgid = oi.orgid
				LEFT JOIN 
				    	obras.formarepasserecursos of ON of.obrid = oi.obrid
				LEFT JOIN
				    	obras.conveniosobra oc ON oc.covid = of.covid 
				LEFT JOIN
					(SELECT 
						SUM(icopercexecutado) as total_exec, 
						obrid 
					FROM 
						obras.itenscomposicaoobra itco
					WHERE 
						icostatus = 'A'
						AND icovigente = 'A' 
					GROUP BY obrid ) pe ON pe.obrid = oi.obrid		
				LEFT JOIN
					( 
					SELECT 
						max(aqoid) as foto, 
						max(arq.arqid) as  arqfoto,
						obr.obrid 
					FROM 
						public.arquivo arq
					INNER JOIN 
						obras.arquivosobra oar ON arq.arqid = oar.arqid
					INNER JOIN 
						obras.obrainfraestrutura obr ON obr.obrid = oar.obrid 
					INNER JOIN 
						seguranca.usuario seg ON seg.usucpf = oar.usucpf 
					WHERE 
						aqostatus = 'A' 
						and tpaid = 21 
						and (arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png') 
					GROUP BY obr.obrid 
					) af ON af.obrid = oi.obrid
				LEFT JOIN
					( SELECT max(rstoid) as restricao, obrid FROM obras.restricaoobra WHERE rststatus = 'A' GROUP BY obrid ) re ON re.obrid = oi.obrid
				LEFT JOIN
					( SELECT supvid as supervisao, obrid FROM obras.supervisao WHERE supstatus = 'A' ORDER BY supvdt DESC LIMIT 1 ) ov ON ov.obrid = oi.obrid
				LEFT JOIN
				    	workflow.documento wdoc ON wdoc.docid = oi.docid
				LEFT JOIN
				    	workflow.estadodocumento west ON west.esdid = wdoc.esdid	
				WHERE
					{$where}
					obsstatus = 'A' AND
					" . ($orgid ? "oi.orgid in ({$orgid}) AND " : "" ) . "
					repstatus = 'A' {$filtros}
					AND (we.esdid <> ". OBRSUPFINALIZADA ." --para Não exibir grupos finalizados
					OR we.esdid is null)
				ORDER BY
					unidade";

		switch( $tipo ){
			
			case "repositorio":
				$this->db->monta_lista( $sql, $cabecalho, 50, 10, "N", "center", "" );
			break;
			
			case "lote":
				$obMontaListaAjax = new MontaListaAjax($db, false);   
				$registrosPorPagina = 50;            
                $obMontaListaAjax->montaLista($sql, $cabecalho,$registrosPorPagina, 50, 'S', '', '', '', '', '', '', '' );
			break;
			
		}
		
	}
	
	/**
	 * Monta a lista de obras que estï¿½o inseridas no repositï¿½rio
	 * @author Rodrigo Pereira de Souza Silva <rodrigossilva@mec.gov.br>
	 * @since 23/09/2010 
	 * @param integer $orgid
	 * @return mixed
	 * 
	 */
	function obrListaObrasSupervisoesFinalizadas( $orgid, $filtros, $tipo = "repositorio" ){
		
		switch( $tipo ){
			
			case "repositorio":
				if($orgid == '3' ){
					$obrid = 'oi.obrid';
					$numconvenio = 'oi.numconvenio,';
				}else{
					$obrid = 'oi.obrid';
				}
				$btExcluir    = "'<center><img src=\"../imagens/excluir.gif\" onclick=\"obrExcluiObraRepositorio( ' || oi.obrid || ' );\" style=\"cursor:pointer;\" title=\"Excluir obra\"/></center>'";
				$btNaoExcluir = "'<center><img src=\"../imagens/excluir_01.gif\" title=\"Esta obra não pode excluída do repositório!\"/></center>'";
				
				$select = " CASE WHEN ore.stsid = " . OBRSITSUPREPOSITORIO . "  THEN {$btExcluir} ELSE {$btNaoExcluir} END as acao,
							ig.gpdid as grupo,
							os.orsid as ordem,
							'(' || {$obrid} || ') ' || obrdesc as nome,
							{$numconvenio}
							ee.entnome as unidade,
							tm.mundescricao || ' / ' || ed.estuf as mun,
							CASE WHEN obrdtinicio is not null THEN to_char(obrdtinicio, 'DD/MM/YYYY') ELSE 'Não Informado' END as inicio,
							CASE WHEN obrdttermino is not null THEN to_char(obrdttermino, 'DD/MM/YYYY') ELSE 'Não Informado' END as termino,
							stodesc as situacao,
							--------Cï¿½lculo Percentual Executado---------------------------------------------------------------------------------------------
							/*(SELECT coalesce(SUM(icopercexecutado), 0.00 ) as total FROM obras.itenscomposicaoobra WHERE obrid = oi.obrid) as percentual,*/
							CASE WHEN pe.total_exec IS NULL 
									THEN '0.00'
								 ELSE
									CASE WHEN pe.total_exec > 100
											THEN '100.00'
										 ELSE
											(SELECT  
											 		COALESCE((SELECT 
															 		SUM(( icopercsobreobra * supvlrinfsupervisor ) / 100)
															  FROM 
																	obras.itenscomposicaoobra itc
															  INNER JOIN 
																	obras.supervisaoitenscomposicao sitc ON itc.icoid = sitc.icoid 
															  WHERE 
															  		sitc.supvid = MAX(s.supvid) 
															  		AND obrid = oi.obrid 
															  		AND itc.icovigente = 'A' 
														  	 ),'0') as percentual
											 FROM
													obras.supervisao s
											 INNER JOIN 
													obras.situacaoobra sit ON sit.stoid = s.stoid
											 LEFT JOIN
													obras.realizacaosupervisao rs ON rs.rsuid = s.rsuid 
											 WHERE
													s.obrid = oi.obrid 
													AND	s.supstatus = 'A'
											)
									END
							END	AS percentual,			
							CASE WHEN repdtlimiteinicial is not null THEN to_char(repdtlimiteinicial, 'DD/MM/YYYY') ELSE 'Não Informado' END as inicial,
							CASE WHEN repdtlimitefinal is not null THEN to_char(repdtlimitefinal, 'DD/MM/YYYY') ELSE 'Não Informado' END as final,
							CASE WHEN gd.gpdid is not null THEN we.esddsc ELSE 'Distribuição' END as supervisao";

							
				if($orgid == '3' ){
				$cabecalho = array( "Ação","Grupo","Ordem de Serviço","Nome da Obra","Convênio", "Unidade Responsável pela Obra", "Município / UF", "Data de Início", "Data de Término", "Situação da Obra","% Executado", "Data Limite <br/> Inical da Supervisão", "Data Limite <br/> Final da Supervisão", "Situação da Supervisão" );
				} else{
				$cabecalho = array( "Ação","Grupo","Ordem de Serviço","Nome da Obra","Unidade Responsável pela Obra", "Município / UF", "Data de Início", "Data de Término", "Situação da Obra","% Executado", "Data Limite <br/> Inical da Supervisão", "Data Limite <br/> Final da Supervisão", "Situação da Supervisão" );
				}
			break;
			
			case "lote":
				
				$select = "'<center><input type=\"checkbox\" id=\"repid_' || ore.repid || '\" name=\"repid\" value=\"' || ore.repid || '\" onclick=\"obrIncluiObraNoLote( ' || oi.obrid || ', ' || ore.repid || ', \'' || replace(obrdesc,'\"', '') || '\', \'' || tm.mundescricao || '\', \'' || obrqtdconstruida || '\', \'' || umdeesc || '\', \'' || ee.entnome || '\', \'' || stodesc || '\', \'' || ed.estuf || '\', \'' || oo.orgdesc || '\',  \'' || CASE WHEN oc.covid IS NOT NULL THEN covnumero ELSE (CASE WHEN numconvenio is not null THEN numconvenio ELSE 'Não Informado' END) END || '\', \'' || coalesce(obrpercexec,0.00) || '\' );\" /></center>' as acao,
							'<img src=\"../imagens/consultar.gif\" style=\"vertical-align:middle; cursor: pointer;\" title=\"Dados da Obra\" onclick=\"obrVerDados(' || oi.obrid || ', \'obra\');\"/> 
							 <img src=\"../imagens/globo_terrestre.png\" style=\"vertical-align:middle; cursor: pointer;\" title=\"Visualizar Mapa\" onclick=\"janela(\'?modulo=principal/supervisao/mapaObra&acao=A&obrid=' || oi.obrid || '\', 600, 585, \'mapaGrupo\');\"/> (' || oi.obrid || ') ' || obrdesc || ' (nº do convênio: ' || CASE WHEN oc.covid IS NOT NULL THEN covnumero ELSE (CASE WHEN numconvenio is not null THEN numconvenio ELSE 'Não Informado' END) END ||  ') - ' || tm.mundescricao as obrdesc,
							orgdesc || ' - ' || ee.entnome as unidade";
				
				$cabecalho = array( "Ação", 
									"Nome da Obra" );
				
				if( !$_SESSION["obras"]["gpdid"] ){
					$where = "ore.repid not in ( SELECT repid FROM obras.itemgrupo ) AND ";
				}else{
					$where = "ore.repid not in ( SELECT repid FROM obras.itemgrupo WHERE gpdid <> {$_SESSION["obras"]["gpdid"]} ) AND ";
				}
				
			break;
			
		}
		
		$sql = "SELECT DISTINCT
					 {$select}
				FROM
					obras.obrainfraestrutura oi
				INNER JOIN
					obras.repositorio ore ON ore.obrid = oi.obrid
				LEFT JOIN
					obras.itemgrupo ig ON ig.repid = ore.repid
				LEFT JOIN
					obras.grupodistribuicao gd ON gd.gpdid = ig.gpdid 
				INNER JOIN
					obras.ordemservico os ON os.gpdid = gd.gpdid --ordem de serviï¿½o
				LEFT JOIN
					workflow.documento wd ON wd.docid = gd.docid
				LEFT JOIN
					workflow.estadodocumento we ON we.esdid = wd.esdid
				INNER JOIN
					entidade.entidade ee ON ee.entid = oi.entidunidade
				INNER JOIN
					obras.situacaoobra so ON so.stoid = oi.stoid
				INNER JOIN
					entidade.endereco ed ON ed.endid = oi.endid
				INNER JOIN
					territorios.municipio tm ON tm.muncod = ed.muncod
				LEFT JOIN
					obras.unidademedida ou ON ou.umdid = oi.umdidobraconstruida
				INNER JOIN
					obras.orgao oo ON oo.orgid = oi.orgid
				LEFT JOIN 
					obras.formarepasserecursos of ON of.obrid = oi.obrid
				LEFT JOIN
					obras.conveniosobra oc ON oc.covid = of.covid 
				LEFT JOIN
					(SELECT 
						SUM(icopercexecutado) as total_exec, 
						obrid 
					FROM 
						obras.itenscomposicaoobra itco
					WHERE 
						icostatus = 'A'
						AND icovigente = 'A' 
					GROUP BY obrid ) pe ON pe.obrid = oi.obrid
				LEFT JOIN
					( 
					SELECT 
						max(aqoid) as foto, 
						max(arq.arqid) as  arqfoto,
						obr.obrid 
					FROM 
						public.arquivo arq
					INNER JOIN 
						obras.arquivosobra oar ON arq.arqid = oar.arqid
					INNER JOIN 
						obras.obrainfraestrutura obr ON obr.obrid = oar.obrid 
					INNER JOIN 
						seguranca.usuario seg ON seg.usucpf = oar.usucpf 
					WHERE 
						aqostatus = 'A' 
						and tpaid = 21 
						and (arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png') 
					GROUP BY obr.obrid 
					) af ON af.obrid = oi.obrid
				LEFT JOIN
					( SELECT max(rstoid) as restricao, obrid FROM obras.restricaoobra WHERE rststatus = 'A' GROUP BY obrid ) re ON re.obrid = oi.obrid
				LEFT JOIN
					(SELECT supvid as supervisao, obrid FROM obras.supervisao WHERE supstatus = 'A' ORDER BY supvdt DESC LIMIT 1) ov ON ov.obrid = oi.obrid
				WHERE
					{$where}
					obsstatus = 'A' AND
					" . ($orgid ? "oi.orgid in ({$orgid}) AND " : "" ) . "
					--repstatus = 'A'
					repsitsupervisao = 'F' -- Status para Supervisï¿½o Finalizada 
					{$filtros}
					AND we.esdid = ". OBRSUPFINALIZADA ." --para exibir apenas grupos finalizados
				ORDER BY
					unidade";

		switch( $tipo ){
			
			case "repositorio":
				$this->db->monta_lista( $sql, $cabecalho, 50, 10, "N", "center", "" );
			break;
			
			case "lote":
				$this->db->monta_lista_grupo( $sql, $cabecalho, 50, 10, "N", "center", "", "", "unidade" );
			break;
			
		}
		
	}
	
	/**
	 * Insere as obras e as datas de inï¿½cio e fim da vistoria no repositï¿½rio
	 * @author Fernando Bagno <fernandosilva@mec.gov.br>
	 * @since 17/03/2010 
	 * @param array $dados
	 * 
	 */
	function insereObrasRepositorio( $dados ){
		
		
		if( is_array($dados["obrid"]) ){
			
			$obras = $dados["obrid"];
			
			// insere as obras no banco
			foreach( $obras as $chave=>$valor ){
				
				$sql = "INSERT INTO obras.repositorio( obrid, 
													   stsid, 
													   repstatus, 
													   repdtinclusao,
													   usucpf ) 
											   VALUES( {$valor}, 
											   		   " . OBRSITSUPREPOSITORIO . ",
											   		   'A', 
											   		   'now',
											   		   '{$_SESSION["usucpf"]}' )";
							   		   
				$this->db->executar( $sql );
				
			}
			
			if( $this->db->commit() ){
				
				print "<script type='text/javascript'>"
					. "    alert('Operação realizada sucesso!');"
					. "    window.opener.location.href = '?modulo=principal/supervisao/repositorio&acao=A';"
					. "    self.close();"
					. "</script>";
				
				die;
					
			}else{
				$this->db->rollback();
			}
			
		}
		
	}
	
	/**
	 * Exclui uma obra do repositï¿½rio de vistorias
	 * @author Fernando Bagno <fernandosilva@mec.gov.br>
	 * @since 16/03/2010 
	 * @param integer $obrid
	 * 
	 */
	function obrExcluiObraRepositorio( $obrid ){
		
		$sql = "UPDATE obras.repositorio SET repstatus = 'I' WHERE obrid = {$obrid}";
		$this->db->executar( $sql );
		
		$this->db->commit( );
		$this->db->sucesso( "principal/supervisao/repositorio", "" );
		
	}
	
	/**
	 * Cria o filtro da lista de obras de acordo com as informaï¿½ï¿½es passadas
	 * @author Fernando Bagno <fernandosilva@mec.gov.br>
	 * @since 16/03/2010 
	 * @return string
	 * 
	 */
	function obrFiltraListaGrupos(){
		$filtro  = $_REQUEST["gpdid"] ? " AND gd.gpdid  = {$_REQUEST["gpdid"]}" : "";
		$filtro .= $_REQUEST["epcid"] ? " AND gd.epcid  = {$_REQUEST["epcid"]}" : "";
		$filtro .= $_REQUEST["esdid"] ? " AND we.esdid  = {$_REQUEST["esdid"]}" : "";
		$filtro .= $_REQUEST["esdidobra"] ? " AND wdobr.esdid  = {$_REQUEST["esdidobra"]}" : "";
		$filtro .= $_REQUEST["obrid"] ? " AND oi.obrid  = {$_REQUEST["obrid"]}" : "";
		$filtro .= $_REQUEST["orgid"] ? " AND o.orgid  = {$_REQUEST["orgid"]}" : "";
		
		$filtro .= $_REQUEST["entid"]  ? " AND oi.entidunidade  = {$_REQUEST["entid"]}" : "";
		$filtro .= $_REQUEST["estcod"] ? " AND gd.estuf = '{$_REQUEST["estcod"]}'" : "";
		$filtro .= $_REQUEST["munid"]  ? " AND ed.muncod = '{$_REQUEST["munid"]}'" : "";
		$filtro .= $_REQUEST["rotas"]  ? " AND we.esdid = ".OBREMAVALIAMEC : "";

		//grupo sem OS
		$filtro .= $_REQUEST["semos"]  ? " AND gd.gpdid NOT IN ( SELECT 
																		oos.gpdid
																	FROM 
																		obras.ordemservico oos
																	WHERE
																		oos.orsstatus = 'A' )" : "";
		
		return $filtro;
	}
	
	/**
	 * Lista os grupos de supervisï¿½o
	 * @author Fernando Bagno <fernandosilva@mec.gov.br>
	 * @since 18/03/2010 
	 * 
	 */
	function obrListaGrupos( $filtros ){
		
		$arPermissao = obras_permissaoPerfil();
		
		$stWhere = ( is_array($arPermissao['obra']) && count($arPermissao['obra']) > 0 ) ? " AND ore.obrid IN(" . ( implode(",", $arPermissao['obra']) ) . ") " : '';	
		
		$btExcluir = "<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"obrExcluirGrupo( ' || gd.gpdid || ' );\"/>";
		$btNaoExcluir = "<img src=\"../imagens/excluir_01.gif\" />";
		
		$sql = "SELECT
					DISTINCT
					'<center>
					     <img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"location.href=\'?modulo=principal/supervisao/criarLote&acao=A&gpdid='|| gd.gpdid ||'\'\"/> &nbsp;'
					     || CASE WHEN ( wd.esdid = " . OBRDISTRIBUIDO . " OR wd.esdid = " . OBREMAVALIAMEC . " ) THEN '{$btExcluir}' ELSE '{$btNaoExcluir}' END ||
					'</center>' as acao,
					gd.estuf as uf,
					gd.gpdid as n_controle,
					CASE WHEN gd.epcid is not null THEN entnome ELSE 'Não Informada' END as empresa,
					(SELECT 
						count(ig.itgid)
					FROM 
						obras.itemgrupo ig 
					INNER JOIN 
						obras.repositorio ore ON ore.repid = ig.repid
					INNER JOIN
						obras.obrainfraestrutura oi ON oi.obrid = ore.obrid
					WHERE 
						ore.repsitsupervisao <> ''
						AND oi.obsstatus = 'A'
						AND ig.gpdid = gd.gpdid) as totalobras,
					upper(usunome) as resp,
					to_char(gpddtcriacao, 'DD/MM/YYYY') as dtinclusao,
					'<FONT '||
					/*Situaï¿½ï¿½o: Grupo em Supervisï¿½o*/
					CASE WHEN wd.esdid = ".GRUPOEMSUPERVISAO."  AND DATE_PART('days', NOW() - (to_char(MAX(whd.htddata), 'YYYY-mm-dd'))::timestamp) + DATE_PART('days', NOW() - (to_char(MAX(wh.htddata), 'YYYY-mm-dd'))::timestamp) > 90
							  THEN 'COLOR=\"#DD0000\" />'||we.esddsc
						 /* Situaï¿½ï¿½o: Aguardando Inï¿½cio de Supervisï¿½o pela Empresa */ 
						 WHEN wd.esdid = ".GRUPOAGUARDANDOINICIOSUPERVISAO." AND DATE_PART('days', NOW() - (to_char(MAX(whd.htddata), 'YYYY-mm-dd'))::timestamp) + DATE_PART('days', NOW() - (to_char(MAX(wh.htddata), 'YYYY-mm-dd'))::timestamp) <= 20
							  THEN 'COLOR=\"#008000\" />'||we.esddsc
						 WHEN wd.esdid = ".GRUPOAGUARDANDOINICIOSUPERVISAO." AND DATE_PART('days', NOW() - (to_char(MAX(whd.htddata), 'YYYY-mm-dd'))::timestamp) + DATE_PART('days', NOW() - (to_char(MAX(wh.htddata), 'YYYY-mm-dd'))::timestamp) > 20 AND DATE_PART('days', NOW() - (to_char(MAX(whd.htddata), 'YYYY-mm-dd'))::timestamp) + DATE_PART('days', NOW() - (to_char(MAX(wh.htddata), 'YYYY-mm-dd'))::timestamp) <= 24
							  THEN 'COLOR=\"#BB9900\" />'||we.esddsc
						 WHEN wd.esdid = ".GRUPOAGUARDANDOINICIOSUPERVISAO." AND DATE_PART('days', NOW() - (to_char(MAX(whd.htddata), 'YYYY-mm-dd'))::timestamp) + DATE_PART('days', NOW() - (to_char(MAX(wh.htddata), 'YYYY-mm-dd'))::timestamp) > 24
							  THEN 'COLOR=\"#DD0000\" />'||we.esddsc
						 ELSE/* As demais Situaï¿½ï¿½es do Grupo */
								CASE WHEN we.esddsc IS NOT NULL
									 	  THEN 'COLOR=\"#000000\" />'||we.esddsc
								END 
					END ||'</FONT>'AS situacao,
					--esddsc as situacao,
					'<center>'||to_char(MAX(wh.htddata), 'DD/MM/YYYY')||'</center>' as datramitacao,
					'<center>'||DATE_PART('days', NOW() - (to_char(MAX(wh.htddata), 'YYYY-mm-dd'))::timestamp)||' dia(s)</center>' as qtddias,
					-- nï¿½mero de dias atï¿½ a ultima tramitaï¿½ï¿½o
					'<center>'||
					(SELECT 
						DATE_PART('days', MAX(hd.htddata) - MIN(hd.htddata)) AS qtd
					 FROM 
					 	workflow.historicodocumento hd
					 WHERE
						hd.docid = gd.docid
					)||' dia(s)</center>' AS datatotal
					
				FROM
					obras.grupodistribuicao gd
				INNER JOIN
					workflow.documento wd ON wd.docid = gd.docid
				LEFT JOIN
					workflow.historicodocumento wh ON wh.docid = gd.docid
				INNER JOIN
					workflow.estadodocumento we ON we.esdid = wd.esdid
				INNER JOIN	
						obras.itemgrupo itg ON itg.gpdid = gd.gpdid
				INNER JOIN
					obras.repositorio ore ON ore.repid = itg.repid --AND ore.repstatus = 'A'
				INNER JOIN
					obras.obrainfraestrutura oi ON oi.obrid = ore.obrid
				INNER JOIN
					entidade.endereco ed ON ed.endid = oi.endid
				INNER JOIN
					obras.orgao AS o ON o.orgid = oi.orgid 
				LEFT JOIN
					obras.empresacontratada ec ON ec.epcid = gd.epcid 
				LEFT JOIN
					entidade.entidade ee ON ee.entid = ec.entid
				LEFT JOIN
					seguranca.usuario su ON su.usucpf = gd.usucpf
				LEFT JOIN
					workflow.documento wdobr ON wdobr.docid = oi.docid
				LEFT JOIN 
					workflow.documento wdc ON wdc.docid = gd.docid
				LEFT JOIN 
					workflow.estadodocumento wed ON wed.esdid = wdc.esdid AND wed.esdstatus = 'A'
				LEFT JOIN 
					workflow.historicodocumento whd ON whd.docid = gd.docid AND whd.aedid = ". GRUPOLIBERADOPARASUPERVISAO ."				
				WHERE
					gpdstatus = 'A' {$filtros}
					$stWhere
				GROUP BY
					gd.gpdid,
					wd.esdid,
					gd.estuf,
					gd.epcid,
					ee.entnome,
					su.usunome,
					gd.gpddtcriacao,
					we.esddsc,
					datatotal
				ORDER BY
					gd.gpdid";
//		dbg($sql,1);
//		$cabecalho = array( "Aï¿½ï¿½o", "UF", "Nï¿½ de Controle", "Empresa Contratada", "Total de Obras", "Criado Por", "Data de Criaï¿½ï¿½o", "Situaï¿½ï¿½o do Grupo" );
		$cabecalho = array( "Ação", "UF", "Nº de Controle", "Empresa Contratada", "Total de Obras", "Criado Por", "Data de Criação", "Situação do Grupo", "Data da Tramitação", "QTD dia(s)após a Tramitação", "QTD dias(s) até a última tramitação" );
		$tamanho   = array('4%');
		$this->db->monta_lista( $sql, $cabecalho, 50, 10, "N", "center", "", "", $tamanho);
		
	}
	
	/**
	 * Salva os dados dos grupos de supervisï¿½o
	 * @author Fernando Bagno <fernandosilva@mec.gov.br>
	 * @since 18/03/2010 
	 * @param array $dados
	 * 
	 */
	function obrSalvaGrupoSupervisao( $dados ){
		
		
		if( !$dados["gpdid"] ){
			// seleciona a empresa responsï¿½vel pelo estado
			$sql   = "SELECT epcid FROM obras.empresaufatuacao WHERE estuf = '{$dados["estuf"]}'";
			$epcid = $this->db->pegaUm($sql);
			
			$epcid = $epcid ? $epcid : "NULL";
			
			// valida as datas
			$gpddtinicio = !empty( $dados["gpddtinicio"] ) ? "'" . formata_data_sql( $dados["gpddtinicio"] ) . "'" : "NULL";   
			$gpdtermino  = !empty( $dados["gpdtermino"] )  ? "'" . formata_data_sql( $dados["gpdtermino"] ) . "'"  : "NULL";
			
			// cria o grupo
			$sql = "INSERT INTO obras.grupodistribuicao( epcid, 
														 estuf, 
														 gpddtinicio,
														 gpdtermino,
														 usucpf, 
														 gpdstatus, 
														 gpddtcriacao)
												 VALUES( {$epcid}, 
												 		 '{$dados["estuf"]}',
												 		 {$gpddtinicio},
												 		 {$gpdtermino}, 
												 		 '{$_SESSION["usucpf"]}',
												 		 'A', 
												 		 'now' )
												 RETURNING gpdid";
			
		
												 		 
			$gpdid = $this->db->pegaUm( $sql );
	
			// cria a sessï¿½o com ID do grupo
			$_SESSION["obras"]["gpdid"] = $gpdid;
	
			//cria os itens do grupo
			if( is_array( $dados["repid"] ) ){
				
				foreach( $dados["repid"] as $chave=>$valor ){
					
					$sql = "INSERT INTO obras.itemgrupo( gpdid, repid, itgdtinclusao )
												 VALUES( {$gpdid}, {$valor}, 'now' )";
					$this->db->executar( $sql );
	
				}
				
			}				
			
		}
			
		$gpdid = $dados["gpdid"] != '' ? $dados["gpdid"] : $gpdid;
		
		// busca a empresa contratada para a UF do grupo (se existir)
		$sql   = "SELECT epcid FROM obras.empresaufatuacao WHERE estuf = '{$dados["estuf"]}'";
		$epcid = $this->db->pegaUm($sql);
		
		$epcid = $epcid ? $epcid : "NULL";
		
		// valida as datas
		$gpddtinicio = !empty( $dados["gpddtinicio"] ) ? "'" . formata_data_sql( $dados["gpddtinicio"] ) . "'" : "NULL";   
		$gpdtermino  = !empty( $dados["gpdtermino"] )  ? "'" . formata_data_sql( $dados["gpdtermino"] ) . "'"  : "NULL";
					
		$sql = "UPDATE 
					obras.grupodistribuicao 
				SET 
					epcid = {$epcid},
					gpddtinicio = {$gpddtinicio},
					gpdtermino = {$gpdtermino}
				WHERE 
					gpdid = {$gpdid}";
		
		$this->db->executar( $sql );
		
		// busca as obras do repositï¿½rio que estï¿½o no grupo
		$sql = "SELECT repid FROM obras.itemgrupo WHERE gpdid = {$gpdid}";
		$arRepid = $this->db->carregarColuna( $sql );
		
		// atualizando o campo repsitsupervisao para S
		if (is_array($arRepid)) {
			foreach ($arRepid as $repid) {
				$sql = "UPDATE 
							obras.repositorio
						SET 
							repsitsupervisao='S'
						WHERE 
							repid={$repid}
							AND repstatus='A'";
				
				$this->db->carregar($sql);
			}					
		}
		
		if( is_array( $dados["repid"] ) ){
			
			if( is_array( $arRepid ) ){
				
				foreach( $arRepid as $valor ){
					if( !in_array( $valor, $dados["repid"] ) ){
						
						$sql = "UPDATE obras.repositorio SET stsid = " . OBRSITSUPREPOSITORIO . " WHERE repid = {$valor}";
						$this->db->executar( $sql );
						
						$itemNaoGrupo = $this->db->pegaUm( "SELECT itgid FROM obras.itemgrupo WHERE repid = {$valor}" );
						
						$sql = "DELETE FROM obras.procedimentotecnico WHERE itgid = {$itemNaoGrupo}";
						$this->db->executar( $sql );
						
						$sql = "DELETE FROM obras.itemgrupo WHERE itgid = {$itemNaoGrupo}";
						$this->db->executar( $sql );
						
					}
				}
			}
			
			foreach( $dados["repid"] as $valor ){
				
				if( in_array( $valor, $arRepid ) ){
					continue;
				}else{
					
					$sql = "INSERT INTO obras.itemgrupo( gpdid, repid, itgdtinclusao )
											 VALUES( {$gpdid}, {$valor}, 'now' )";
					
					$this->db->executar( $sql );
				
				}
				
			}
			
		}
		
		$this->obrInserirProcedimentoTecnico( $dados["tppid"], $gpdid );
			
		$this->db->commit();
		$this->db->sucesso( "principal/supervisao/criarLote", "" );
		
	}
	
	function obrInserirProcedimentoTecnico( $dados, $gpdid ){

		$sql = "SELECT itgid FROM obras.itemgrupo WHERE gpdid = {$gpdid}";
		$itgid = $this->db->carregarColuna( $sql );
		
		if($itgid){
			$sql = "DELETE FROM obras.procedimentotecnico WHERE itgid in (" . implode( ",", $itgid ) . ")";
			$this->db->executar( $sql );
		}
		
		if( is_array( $dados ) ){
			
			foreach( $dados as $chave=>$valor ){
				
				for( $i = 0; $i < count($valor); $i++ ){
					
					$sql = "INSERT INTO obras.procedimentotecnico( itgid, tppid )
													       VALUES( {$chave}, {$valor[$i]} )";
				
					$this->db->executar( $sql );
					
				}
				
			}
			
		}
		
	}
	
	
	/**
	 * Busca os dados do grupo
	 * @author Fernando Bagno <fernandosilva@mec.gov.br>
	 * @since 18/03/2010 
	 * @param integer $gpdid
	 * @return array
	 * 
	 */
	function obrBuscaDadosGrupo( $gpdid ){
		
		$sql = "SELECT 
					estuf, entnome, ec.epcid, gpddtinicio, gpdtermino
				FROM 
					obras.grupodistribuicao og
				LEFT JOIN
					obras.empresacontratada ec ON ec.epcid = og.epcid
				LEFT JOIN
					entidade.entidade ee ON ee.entid = ec.entid 
				WHERE 
					gpdid = {$gpdid}";
		
		return  $this->db->pegaLinha( $sql );
		
	}
	
	/**
	 * Monta a tabela com as obras inseridas no grupo
	 * @author Fernando Bagno <fernandosilva@mec.gov.br>
	 * @since 18/03/2010 
	 * @param integer $gpdid
	 * @return mixed
	 * 
	 */
	function obrMotaListaGrupo( $gpdid, $obrSupSoLeitura, $esdid ){
		
		print "<script>var obrItensLote = new Array();</script>";
		
		if( $gpdid ){
			// pegando o estado atual do grupo
			$docid = obrCriarDocumento( $_SESSION["obras"]["gpdid"] );
			//$estadoAtual = wf_pegarEstadoAtual( $docid );
			//Pegando a Situaï¿½ï¿½o do Checklist/Parecer. 
			$dadosChecklist = tabelaObrasChecklistaNaoPreenchido($gpdid);
			//Pegando as informaï¿½ï¿½es da Tramitaï¿½ï¿½o da Obra(Individual).
			$dadosTramitacao = dadosTramitacaoObraIndividual($gpdid);
			
			$sql = "SELECT
						oi.obrid as obra, 
						ig.itgid as grupo,
						ore.repid as id,
						obrdesc,
						obrqtdconstruida,
						tm.mundescricao as municipio,
						umdeesc as unidademedida,
						entnome,
						orgdesc,
						stodesc,
						oi.orgid,
						CASE WHEN oc.covid IS NOT NULL THEN covnumero ELSE (CASE WHEN numconvenio is not null THEN numconvenio ELSE 'Não Informado' END) END as convenio,
						--obrpercexec as percentual
						CASE WHEN
							(
								SELECT  
									MAX( coalesce((SELECT 
												sum(( icopercsobreobra * supvlrinfsupervisor ) / 100)
											  FROM 
												obras.itenscomposicaoobra i
											  INNER JOIN 
												obras.supervisaoitenscomposicao si ON i.icoid = si.icoid WHERE si.supvid = s.supvid AND obrid = oi.obrid AND i.icovigente = 'A' ),'0') ) as percentual
								FROM
									obras.supervisao s
								INNER JOIN 
									obras.situacaoobra si ON si.stoid = s.stoid
								LEFT JOIN
									obras.realizacaosupervisao rs ON rs.rsuid = s.rsuid 
								WHERE
									s.obrid = oi.obrid AND
									s.supstatus = 'A'
							 ) > 100 THEN 100
						ELSE
							CASE WHEN
								(
									SELECT  
										MAX( coalesce((SELECT 
													sum(( icopercsobreobra * supvlrinfsupervisor ) / 100)
												  FROM 
													obras.itenscomposicaoobra i
												  INNER JOIN 
													obras.supervisaoitenscomposicao si ON i.icoid = si.icoid WHERE si.supvid = s.supvid AND obrid = oi.obrid AND i.icovigente = 'A' ),'0') ) as percentual
									FROM
										obras.supervisao s
									INNER JOIN 
										obras.situacaoobra si ON si.stoid = s.stoid
									LEFT JOIN
										obras.realizacaosupervisao rs ON rs.rsuid = s.rsuid 
									WHERE
										s.obrid = oi.obrid AND
										s.supstatus = 'A'
								 ) IS NULL THEN 0
							ELSE
								(
									SELECT  
										MAX( coalesce((SELECT 
													sum(( icopercsobreobra * supvlrinfsupervisor ) / 100)
												  FROM 
													obras.itenscomposicaoobra i
												  INNER JOIN 
													obras.supervisaoitenscomposicao si ON i.icoid = si.icoid WHERE si.supvid = s.supvid AND obrid = oi.obrid AND i.icovigente = 'A' ),'0') ) as percentual
									FROM
										obras.supervisao s
									INNER JOIN 
										obras.situacaoobra si ON si.stoid = s.stoid
									LEFT JOIN
										obras.realizacaosupervisao rs ON rs.rsuid = s.rsuid 
									WHERE
										s.obrid = oi.obrid AND
										s.supstatus = 'A'
								 )
							END
						END as percentual
						
					FROM
						obras.itemgrupo ig
					INNER JOIN
						obras.repositorio ore ON ore.repid = ig.repid
					INNER JOIN
						obras.obrainfraestrutura oi ON oi.obrid = ore.obrid
					INNER JOIN
						obras.unidademedida ou ON ou.umdid = oi.umdidobraconstruida
					INNER JOIN
						entidade.entidade ee ON ee.entid = oi.entidunidade
					INNER JOIN
						entidade.endereco ed ON ed.endid = oi.endid
					INNER JOIN
						territorios.municipio tm ON tm.muncod = ed.muncod
					INNER JOIN
						obras.orgao oo ON oo.orgid = oi.orgid
					INNER JOIN
						obras.situacaoobra so ON so.stoid = oi.stoid
					LEFT JOIN 
						( SELECT max(frrid), obrid, covid FROM obras.formarepasserecursos GROUP BY obrid, covid ) of ON of.obrid = oi.obrid
					LEFT JOIN
						obras.conveniosobra oc ON oc.covid = of.covid 	
					WHERE
						gpdid = {$gpdid} 
						AND ore.repsitsupervisao <> ''
						AND oi.obsstatus = 'A'
					ORDER BY
						itgid";
			
			$itens = $this->db->carregar( $sql );

			if( is_array($itens) ){
				
				$edSuperior = 0;
				$edProfi    = 0;
				$edBasica   = 0;
				
//				$disabledCheckbox = ( $esdid == OBRDISTRIBUIDO ) ? "" : 'disabled=\"disabled\"';
				
				#verificando se os campos estarï¿½o ou nï¿½o desabilitados
				$sql = "select gpdid from obras.ordemservico where gpdid = ".$_SESSION["obras"]["gpdid"] . " and orsstatus = 'A'";
				$gpdid = $this->db->carregar($sql);
				
//				if( (possuiPerfil(PERFIL_ADMINISTRADOR) || possuiPerfil(PERFIL_SAA)) && (!$gpdid[0]['gpdid']) ){
//					$disabledCheckbox = "";
//				}
				if( possuiPerfil(PERFIL_ADMINISTRADOR) || possuiPerfil(PERFIL_SAA) ){
					$disabledCheckbox = "";
				}else{
					$disabledCheckbox = 'disabled=\"disabled\"';
				}
				
				for( $i = 0; $i < count($itens); $i++ ){
					
					$sql = "SELECT
							'<div style=\"white-space: nowrap;\"><input type=\"checkbox\" name=\"tppid[{$itens[$i]["grupo"]}][]\" value=\"' || tp.tppid || '\" id=\"tppid_{$itens[$i]["grupo"]}_' || tp.tppid || '\" ' || CASE WHEN pt.tppid is not null THEN 'checked=\"checked\"' ELSE '' END || ' {$disabledCheckbox}/>' || tppsigla || '</div>' as acao
						FROM
							obras.tipoprocedimento tp
						LEFT JOIN
							obras.procedimentotecnico pt ON tp.tppid = pt.tppid AND itgid = {$itens[$i]["grupo"]}";
					
					$procedimentos = $this->db->carregarColuna( $sql );
					
					if( is_array( $procedimentos ) ){
						
						//array_push( $procedimentos, "&nbsp;&nbsp;&nbsp;<a style='cursor:pointer;'>Todos</a>" );
						array_push( $procedimentos, "<input type='checkbox' id='selecionaTodos_{$itens[$i]["grupo"]}' onclick='obrSelecionaTodosProcedimentos({$itens[$i]["grupo"]});' {$disabledCheckbox}/>Todos" );
						 
						$tabelaProcedimentos .= "<table widht='100%' style='color: #888888;'>";
						
						$count = 0;
						
						foreach( $procedimentos as $valor ){
							
							$abreTr  = ( $count % 2 == 1) ? ""  : "<tr>";
							$fechaTr = ( $count % 2 == 0) ? ""  : "</tr>";
							
							$tabelaProcedimentos .= "{$abreTr}<td>{$valor}</td>{$fechaTr}";
							
							$count++;
							
						}
						
						$tabelaProcedimentos .= "</table>";
						
					}
					
					$edSuperior = ($itens[$i]["orgid"] == 1) ? $edSuperior + 1 : $edSuperior;
					$edProfi    = ($itens[$i]["orgid"] == 2) ? $edProfi + 1   : $edProfi;
					$edBasica   = ($itens[$i]["orgid"] == 3) ? $edBasica + 1  : $edBasica; 
					
					$cor = ($i % 2) ? "#f4f4f4" : "#e0e0e0";
					
					$atencao = $itens[$i]["obrqtdconstruida"] == 0 ? "<img src='../imagens/restricao.png' style='vertical-align:middle;' title='Não existe área construída informada para esta obra!'/> " : "";
										  
					print "<tr bgcolor='{$cor}' id='obralote_{$itens[$i]["id"]}'>"
						. "    <td>"
						. "        <img src=\"../imagens/consultar.gif\" style=\"vertical-align:middle; cursor: pointer;\" title=\"Dados da Obra\" onclick=\"obrVerDados( {$itens[$i]["obra"]}, 'obra');\"/>"
						. "        <img src=\"../imagens/globo_terrestre.png\" style=\"vertical-align:middle; cursor: pointer;\" title=\"Ver Obra no mapa\" onclick=\"janela('?modulo=principal/supervisao/mapaObra&acao=A&obrid={$itens[$i]["obra"]}', 600, 585, 'mapaGrupo');\"/>"
						. "        <input type='hidden' name='repid[]' id='repid_{$itens[$i]["id"]}' value='{$itens[$i]["id"]}'/>"
						. "        ({$itens[$i]["obra"]}) {$itens[$i]["obrdesc"]} (nº do convênio: {$itens[$i]["convenio"]}) <br/>"
						. "    </td>"
						. "    <td>"
						. "        <font style='color:#888888; font-size:8pt;'>" 
						.              $tabelaProcedimentos 
						. "        </font>"         
						. "    </td>"
						. "    <td style='text-align:right;'>" . $atencao . number_format( $itens[$i]["obrqtdconstruida"], 2, ",", "." ) . " {$itens[$i]["unidademedida"]}</td>"
						. "    <td>{$itens[$i]["municipio"]}</td>"
						. "    <td>{$itens[$i]["entnome"]}</td>"
						. "    <td>{$itens[$i]["orgdesc"]}</td>"
						. "    <td>{$itens[$i]["stodesc"]}</td>"
						. "    <td style='text-align:right; color:#0066cc;'>" . number_format( $itens[$i]["percentual"], 2, ",", "." ) . "</td>"
					  /*. "    <td style='text-align:center;'>" . $estadoAtual['esddsc'] . "</td>"*/
						. "    <td style='text-align:center;'>" . ( ($dadosChecklist[$i]['questionario'] > 0) ? "<font color=\"#FF0000\">Não Preenchido</font>" : "Preenchido" ) . "</td>"
						. "    <td style='text-align:center;'>" . (($dadosChecklist[$i]['situacao'])? $dadosChecklist[$i]['situacao'] : ' - ') . "</td>"
						. "    <td style='text-align:center;'>" . (($dadosChecklist[$i]['dataultminclusao'])? $dadosChecklist[$i]['dataultminclusao'] : ' - ') . "</td>"
						. "    <td style='text-align:center;'>" . ($dadosChecklist[$i]['id_parecer']) . "</td>"
						. "    <td style='text-align:center;'>" . (($dadosTramitacao[$i]['situacao_tramitacao'])? $dadosTramitacao[$i]['situacao_tramitacao'] : ' - ') . "</td>"
						. "    <td style='text-align:center;'>" . (($dadosTramitacao[$i]['datramitacao'])? $dadosTramitacao[$i]['datramitacao'] : ' - ') . "</td>"
						. "    <td style='text-align:center;'>" . (($dadosTramitacao[$i]['diasapostramitacao'] == '' ) ? ' - ' : $dadosTramitacao[$i]['diasapostramitacao']) . "</td>"
						. "    <td style='text-align:center;'>" . (($dadosTramitacao[$i]['diasatetramitacao']  == '' ) ? ' - ' : $dadosTramitacao[$i]['diasatetramitacao'] ). "</td>"
						. "</tr>";
		
					print "<script>obrItensLote.push( {$itens[$i]["id"]} );obrVerificaTodosProcedimentos({$itens[$i]["grupo"]});</script>";	

					$tabelaProcedimentos = "";
					
				}
				
				print "<tr bgcolor='#ffffff' id='totalGrupo'>"
					. "    <td colspan='16' style='border-top: 2px solid #404040; border-bottom: 3px solid #dfdfdf;'><b>"
					. "        Total de Registros: <span id='nTotalObrasGrupo'>{$i}</span> &nbsp;"
					. "        ( Educação Superior: <span id='nTotalObrasSuperior'>{$edSuperior}</span> | "
					. "          Educação Profissional: <span id='nTotalObrasProfissional'>{$edProfi}</span> | "
					. "          Educação Básica: <span id='nTotalObrasBasica'>{$edBasica}</span> )"
					. "    </b></td>"
					. "</tr>";
				
			}
		}
	}
		
	/**
	 * Inativa um grupo de supervisï¿½o
	 * @author Fernando Bagno <fernandosilva@mec.gov.br>
	 * @since 19/03/2010 
	 * @param integer $gpdid
	 * 
	 */
	function obrExcluirGrupo( $gpdid ){
		
		// busca as obras do repositï¿½rio que estï¿½o no grupo
		$sql = "SELECT repid FROM obras.itemgrupo WHERE gpdid = {$gpdid}";
		$arRepid = $this->db->pegaLinha( $sql );
		
		// atualiza a situaï¿½ï¿½o de supervisï¿½o das obras do repositï¿½rio
		if( is_array( $arRepid ) ){
			$sql = "UPDATE obras.repositorio SET stsid = " . OBRSITSUPREPOSITORIO . " WHERE repid in ( ". implode( ", ", $arRepid ). " )";
			$this->db->executar( $sql );
		} 
		
		// deleta os procedimentos
		$sql = "SELECT itgid FROM obras.itemgrupo WHERE gpdid = {$gpdid}";
		$itgid = $this->db->carregarColuna( $sql );
		
		if( $itgid ){
			
			$sql = "DELETE FROM obras.procedimentotecnico WHERE itgid in (" . implode( ",", $itgid ) . ")";
			$this->db->executar( $sql );
			
			$sql = "SELECT trjid FROM obras.trajetoria WHERE itgid in (" . implode( ",", $itgid ) . ") AND trjstatus = 'A'";
			$trjid = $this->db->carregarColuna( $sql );
			
			if( $trjid ){
				$this->obrExibeMsgErro( "Este grupo possui rota(s) cadastrada(s) e não pode ser excluído!" );
				die;
			}
			
		}
		
		// deleta as obras do grupo
		$sql = "DELETE FROM obras.itemgrupo WHERE gpdid = {$gpdid}";
		$this->db->executar( $sql );
		
		// inativa o grupo
		$sql = "UPDATE obras.grupodistribuicao SET gpdstatus = 'I' WHERE gpdid = {$gpdid}";
		$this->db->executar( $sql );
			
		$this->db->commit();
		$this->db->sucesso( "principal/supervisao/distribuicao", "" );
		
	}
	

	/**
	 * Cria o filtro da lista de obras de acordo com as informaï¿½ï¿½es passadas
	 * @author Fernando Bagno <fernandosilva@mec.gov.br>
	 * @since 16/03/2010 
	 * @return string
	 * 
	 */
	function obrFiltraListaEmpresas(){
		
		$_REQUEST["entnumcpfcnpj"] = $_REQUEST["entnumcpfcnpj"] ? $this->trataString($_REQUEST["entnumcpfcnpj"]) : "";
		
		$filtro  = $_REQUEST["entnumcpfcnpj"] ? " AND entnumcpfcnpj = '{$_REQUEST["entnumcpfcnpj"]}'" : "";
		$filtro .= $_REQUEST["epcid"] 		  ? " AND ec.epcid = {$_REQUEST["epcid"]}" 				  : "";
		
		return $filtro;
		
	}
	
	
	/**
	 * Monta uma lista com as empresas cadastradas
	 * @author Fernando Bagno <fernandosilva@mec.gov.br>
	 * @since 25/03/2010 
	 * @param string $filtros
	 * 
	 */
	function obrMontaListaEmpresas( $filtros = "" ){
		
		$sql = "SELECT
					ec.epcid as id,
					entnumcpfcnpj as cnpj,
					entnome as nome,
					entemail as email,
					entnumdddcomercial as ddd,
					entnumcomercial as tel
				FROM
					obras.empresacontratada ec
				INNER JOIN
					(SELECT max(epaid), epcid FROM obras.empresaufatuacao GROUP BY epcid ) ef ON ef.epcid = ec.epcid
				INNER JOIN
					entidade.entidade ee ON ee.entid = ec.entid
				WHERE
					entstatus = 'A' {$filtros}";
				
		$dadosEmpresa = $this->db->carregar( $sql );

		print "<table width='95%' align='center' border='0' cellspacing='0' cellpadding='2' class='listagem'>";
		
		if( is_array($dadosEmpresa) ){
			
			print "<thead><tr>"
				. "    <td align='center' valign='top' class='title' style='border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;' onmouseover='this.bgColor=\"#c0c0c0\";' onmouseout='this.bgColor=\"\";'>"
				. "        <strong>Ação</strong>"
				. "    </td>"
				. "    <td align='center' valign='top' class='title' style='border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;' onmouseover='this.bgColor=\"#c0c0c0\";' onmouseout='this.bgColor=\"\";'>"
				. "        <strong>CNPJ</strong>"
				. "    </td>"
				. "    <td align='center' valign='top' class='title' style='border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;' onmouseover='this.bgColor=\"#c0c0c0\";' onmouseout='this.bgColor=\"\";'>"
				. "        <strong>Nome</strong>"
				. "    </td>"
				. "    <td align='center' valign='top' class='title' style='border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;' onmouseover='this.bgColor=\"#c0c0c0\";' onmouseout='this.bgColor=\"\";'>"
				. "        <strong>E-mail</strong>"
				. "    </td>"
				. "    <td align='center' valign='top' class='title' style='border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;' onmouseover='this.bgColor=\"#c0c0c0\";' onmouseout='this.bgColor=\"\";'>"
				. "        <strong>Telefone Comercial</strong>"
				. "    </td>"
				. "    <td align='center' valign='top' class='title' style='border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;' onmouseover='this.bgColor=\"#c0c0c0\";' onmouseout='this.bgColor=\"\";'>"
				. "        <strong>UF's de Atendimento</strong>"
				. "    </td>"
				. "</tr></thead>";
			
				for( $i = 0; $i < count($dadosEmpresa); $i++ ){
					
					$sql = "SELECT
								estuf
							FROM
								obras.empresaufatuacao
							WHERE
								epcid = {$dadosEmpresa[$i]["id"]}";
					
					$ufsEmpresa = $this->db->carregarColuna( $sql ); 
					
					$sql = "SELECT DISTINCT 
								gpdid 
							FROM 
								obras.grupodistribuicao 
							WHERE 
								epcid = {$dadosEmpresa[$i]["id"]} AND
								gpdstatus = 'A'";
					
					$btExcluir = $this->db->carregarColuna( $sql ) ? "<img src='../imagens/excluir_01.gif' title='Esta empresa está associada a um grupo!'/>" : "<img src='../imagens/excluir.gif' style='cursor:pointer;' title='Excluir Empresa' onclick='obrExcluirEmpresa({$dadosEmpresa[$i]["id"]});'/>";
					
					print "<tr>"
						. "    <td align='center'>"
						. "       <img src='../imagens/alterar.gif' style='cursor:pointer;' title='Editar Empresa' onclick='obrAlterarEmpresa({$dadosEmpresa[$i]["id"]});'/>"
						. "       {$btExcluir}"
						. "    </td>"
						. "    <td>"
						. 	       formatar_cnpj($dadosEmpresa[$i]["cnpj"])
						. "    </td>"
						. "    <td>"
						. "       {$dadosEmpresa[$i]["nome"]}"
						. "    </td>"
						. "    <td>"
						.          ( $dadosEmpresa[$i]["email"] ? $dadosEmpresa[$i]["email"] : "Não Informado" )
						. "    </td>"
						. "    <td>"
						. "       ({$dadosEmpresa[$i]["ddd"]}) {$dadosEmpresa[$i]["tel"]}"
						. "    </td>"
						. "    <td>"
						. 	       implode(", ", $ufsEmpresa)
						. "    </td>"
						. "</tr>";
					
				}
				
		}else{
			
			print "<tr><td align='center' style='color:#cc0000;'>Não foram encontrados Registros.</td></tr>";
			
		}
		
		print "</table>";
		
	}
	
	/**
	 * Insere no banco a empresa e os seus dados
	 * @author Fernando Bagno <fernandosilva@mec.gov.br>
	 * @since 25/03/2010 
	 * @param array $dados
	 * 
	 */
	function obrCadastraEmpresaContratada( $dados ){
		
		$dados["epcnumproceconc"] = !empty($dados["epcnumproceconc"]) ? "'" . $dados["epcnumproceconc"] . "'" : "NULL";
		$dados["epcnumcontrato"]  = !empty($dados["epcnumcontrato"])  ? "'" . $dados["epcnumcontrato"] . "'"  : "NULL";
		
		$dados["epcdtiniciocontrato"]  = !empty($dados["epcdtiniciocontrato"])  ? "'" . formata_data_sql( $dados["epcdtiniciocontrato"] ) . "'" : "NULL";
		$dados["epcdtfinalcontrato"]   = !empty($dados["epcdtfinalcontrato"])   ? "'" . formata_data_sql( $dados["epcdtfinalcontrato"] ) . "'"  : "NULL";
		
		if( !$dados["epcid"] ){
			
			$sql = "SELECT epcid FROM obras.empresacontratada WHERE entid = {$dados["entid"]}";
			$jaExiste = $this->db->pegaUm( $sql );
	
			if( !$jaExiste ){
				
				$sql   = "INSERT INTO obras.empresacontratada( entid, 
															   epcnumproceconc, 
															   epcnumcontrato, 
															   epcdtiniciocontrato, 
															   epcdtfinalcontrato ) 
													  VALUES ( {$dados["entid"]},
													  		   {$dados["epcnumproceconc"]},
													  		   {$dados["epcnumcontrato"]},
													  		   {$dados["epcdtiniciocontrato"]},
													  		   {$dados["epcdtfinalcontrato"]} ) RETURNING epcid";
													  		   
				$epcid = $this->db->pegaUm( $sql );
				
				$_SESSION["obras"]["epcid"] = $epcid;
				
				if( is_array($dados["estuf"]) ){
					
					foreach( $dados["estuf"] as $valor ){
						
						$posicao = strpos( $valor , "|" );
						
						$estuf  = substr( $valor, 0, $posicao );
						$muncod = substr( $valor, $posicao + 1 ); 
						
						$sql = "INSERT INTO obras.empresaufatuacao(estuf, epcid, muncod) VALUES ('{$estuf}', {$epcid}, '{$muncod}')";
						$this->db->executar( $sql );
							
					}
					
				}
				
				if( is_array($dados["entidresp"]) ){
					
					foreach( $dados["entidresp"] as $valor ){
						
						$sql = "INSERT INTO obras.respempresacontratada(entid, epcid) VALUES ('{$valor}', {$epcid})";
						$this->db->executar( $sql );
							
					}
					
				}
				
			}else{
				$this->obrExibeMsgErro( "Esta empresa já está cadastrada!" );
				die;	
			}
			
		}else{
			
			$epcid = $dados["epcid"];
			
			$sql = "UPDATE 
						obras.empresacontratada
					SET
						epcnumproceconc = {$dados["epcnumproceconc"]}, 
						epcnumcontrato  = {$dados["epcnumcontrato"]}, 
						epcdtiniciocontrato = {$dados["epcdtiniciocontrato"]}, 
						epcdtfinalcontrato  = {$dados["epcdtfinalcontrato"]}
					WHERE
						epcid = {$epcid}";
			$this->db->executar( $sql );
			
			$sql = "DELETE FROM obras.empresaufatuacao WHERE epcid = {$epcid}";
			$this->db->executar( $sql );
			
			$sql = "DELETE FROM obras.respempresacontratada WHERE epcid = {$epcid}";
			$this->db->executar( $sql );
			
			if( is_array($dados["estuf"]) ){
				
				foreach( $dados["estuf"] as $valor ){

					$posicao = strpos( $valor , "|" );
					
					$estuf  = substr( $valor, 0, $posicao );
					$muncod = substr( $valor, $posicao + 1 ); 
					
					$sql = "INSERT INTO obras.empresaufatuacao(estuf, epcid, muncod) VALUES ('{$estuf}', {$epcid}, '{$muncod}')";
					$this->db->executar( $sql );
					
				}
				
			}
			
			if( is_array($dados["entidresp"]) ){
				
				foreach( $dados["entidresp"] as $valor ){
					
					$sql = "INSERT INTO obras.respempresacontratada(entid, epcid) VALUES ('{$valor}', {$epcid})";
					$this->db->executar( $sql );
					
				}
				
			}
			
		}
		
		$this->db->commit();
		$this->db->sucesso( "principal/supervisao/inserirEmpresaContratada", "" );
	
		
	}
	
	/**
	 * Busca as dados da empresa
	 * @author Fernando Bagno <fernandosilva@mec.gov.br>
	 * @since 25/03/2010 
	 * @param integer $epcid
	 * 
	 */
	function obrBuscaDadosEmpresa( $epcid ){
		
		$sql = "SELECT
					ee.entid,
					entnome,
					trim(epcnumproceconc) as epcnumproceconc, 
					trim(epcnumcontrato) as epcnumcontrato, 
					epcdtiniciocontrato, 
					epcdtfinalcontrato
				FROM
					entidade.entidade ee 
				INNER JOIN
					obras.empresacontratada ec ON ee.entid = ec.entid
				WHERE
					epcid = {$epcid}";
		
		return $this->db->pegaLinha( $sql );
		
	}

	/**
	 * Monta uma lista com os responsï¿½veis de uma empresa
	 * @author Fernando Bagno <fernandosilva@mec.gov.br>
	 * @since 25/03/2010 
	 * @param integer $epcid
	 * 
	 */
	function obrMontaListaRespEmpresa( $epcid ){
		
		if( $epcid ){
			
			$sql = "SELECT
						ee.entid,
						entnome,
						entnumcpfcnpj,
						entnumdddcomercial,
						entnumcomercial,
						entnumdddcelular,
						entnumcelular,
						entemail
					FROM
						obras.respempresacontratada rc
					INNER JOIN
						entidade.entidade ee ON ee.entid = rc.entid 
					WHERE
						epcid = {$epcid}";
				
			$responsaveis = $this->db->carregar( $sql );
			
			if( is_array($responsaveis) ){

				for( $i = 0; $i < count($responsaveis); $i++ ){
					
					print "<tr id='linha_{$responsaveis[$i]["entid"]}'>"
						. "<td align='center'>"
						. "    <img src='/imagens/excluir.gif' style='cursor: pointer'  border='0' title='Excluir' onclick='obrExcluiRespEmpresa({$responsaveis[$i]["entid"]});'/>"
						. "    <input type='hidden' name='entidresp[]' id='entidresp_{$responsaveis[$i]["entid"]}' value='{$responsaveis[$i]["entid"]}'/>"
						. "</td>"
						. "<td>" . formatar_cpf($responsaveis[$i]["entnumcpfcnpj"]) . "</td>"
						. "<td>{$responsaveis[$i]["entnome"]}</td>"
						. "<td>({$responsaveis[$i]["entnumdddcomercial"]}) {$responsaveis[$i]["entnumcomercial"]}</td>"
						. "<td>({$responsaveis[$i]["entnumdddcelular"]}) {$responsaveis[$i]["entnumcelular"]}</td>"
						. "<td>{$responsaveis[$i]["entemail"]}</td>"
						. "</tr>";
					
				}
				
			}
			
		}
		
	}
	
	/**
	 * Exclui uma empresa do banco (exclusï¿½o lï¿½gica)
	 * @author Fernando Bagno <fernandosilva@mec.gov.br>
	 * @since 25/03/2010 
	 * @param integer $epcid
	 * 
	 */
	function obrExcluiEmpresa( $epcid ){

		$sql = "DELETE FROM obras.empresaufatuacao WHERE epcid = {$epcid}";
		$this->db->executar( $sql );
		
		$sql = "DELETE FROM obras.respempresacontratada WHERE epcid = {$epcid}";
		$this->db->executar( $sql );
		
		$sql = "DELETE FROM obras.empresacontratada WHERE epcid = {$epcid}";
		$this->db->executar( $sql );

		$this->db->commit();
		$this->db->sucesso( "principal/supervisao/listaEmpresas", "" );
		
	}
	
	/**
	 * Monta uma lista com os grupos que podem definir rota
	 * @author Fernando Bagno <fernandosilva@mec.gov.br>
	 * @since 25/03/2010 
	 * @param string $filtros
	 * 
	 */
	function obrMontaListaGruposRotas( $filtros = "" ){
		
		global $obrMEC;
		
		//if( $obrMEC ):
		
			$selectEmpresa = ", ee.entnome as empresa";
			$goupByEmpresa = ", ee.entnome"; 
			$joinEmpresa   = "INNER JOIN obras.empresacontratada ec ON ec.epcid = og.epcid
							  INNER JOIN entidade.entidade ee ON ee.entid = ec.entid";
		//endif;
		
		if( !$this->db->testa_superuser() ){
			
			$ufsPermitidas = obrBuscaUfEmpresa( $_SESSION["usucpf"] );
			
			if( count($ufsPermitidas) > 0 && is_array( $ufsPermitidas ) ){
				$whereEmpresa = "estuf in ( '" . implode( "','", $ufsPermitidas ) . "' ) AND ";
			}
			
		}
		
		$btAlterar = "<img src=\"../imagens/alterar.gif\" style=\"vertical-align: middle; cursor:pointer;\" onclick=\"obrAbreListaRota( ' || og.gpdid || ' );\"/>";
		
		$icAprovado    = "<center><img src=\"../imagens/check_p.gif\" style=\"vertical-align: middle;\" title=\"Aprovada\"/></center>";
		$icEmDefinicao = "<center><img src=\"../imagens/restricao.png\" style=\"vertical-align: middle;\" title=\"Em Definição de Rotas\"/></center>";
		$icProposta    = "<center><img src=\"../imagens/report.gif\" style=\"vertical-align: middle;\" title=\"Proposta\"/></center>";
		
		//filtros
		$filtro .= ( $_REQUEST['gpdid']  != '' ) ? " AND og.gpdid  = '{$_REQUEST['gpdid']}'   " : "";
		$filtro .= ( $_REQUEST['esdid']  != '' ) ? " AND we.esdid  = '{$_REQUEST['esdid']}'   " : "";
		$filtro .= ( $_REQUEST['epcid']  != '' ) ? " AND ee.entid  = '{$_REQUEST['epcid']}'   " : "";
		$filtro .= ( $_REQUEST['estcod'] != '' ) ? " AND og.estuf  = '{$_REQUEST['estcod']}'  " : "";
		$filtro .= ( $_REQUEST['munid']  != '' ) ? " AND ed.muncod = '{$_REQUEST['munid']}'   " : "";
		$filtro .= ( $_REQUEST['rotas']  != '' ) ? " AND we.esdid  = '".OBREMAVALIAMEC."'     " : "";
		//fim dos filtros
		
		$sql = "SELECT DISTINCT
					'<center>{$btAlterar}</center>' as acao,
					CASE
						WHEN ra.strid = 1 THEN '$icAprovado'
						WHEN rp.strid = 4 THEN '$icProposta'
						ELSE '$icEmDefinicao' 
					END as situacao,
					og.gpdid as numgrupo,
					to_char(gpddtcriacao, 'DD/MM/YYYY') as dtinclusao,
					usunome as responsavel,
					og.estuf as uf {$selectEmpresa},
					we.esddsc,
					'<center>'||to_char(MAX(htddata), 'DD/MM/YYYY')||'</center>' as datramitacao,
					'<center>'||DATE_PART('days', NOW() - (to_char(MAX(htddata), 'YYYY-mm-dd'))::timestamp)||' dia(s)</center>' as qtddias
					--'<center>'|| DATE_PART('days', NOW() - (SELECT (to_char(MAX(htddata), 'YYYY-mm-dd'))::timestamp FROM workflow.historicodocumento wh WHERE wh.docid = og.docid ))||' dia(s) </center>' as qtddias
					
				FROM
					obras.grupodistribuicao og
				INNER JOIN
					seguranca.usuario su ON og.usucpf = su.usucpf
				INNER JOIN
					workflow.documento wd On og.docid = wd.docid
				INNER JOIN
					workflow.historicodocumento wh ON wh.docid = og.docid	
				{$joinEmpresa}
				INNER JOIN workflow.estadodocumento we ON we.esdid = wd.esdid
				LEFT JOIN obras.rotas ra ON ra.gpdid = og.gpdid
							    			AND ra.strid = 1
							    			AND ra.rotstatus = 'A'	
				LEFT JOIN obras.rotas rp ON rp.gpdid = og.gpdid
							    			AND rp.strid = 4	
							    			AND rp.rotstatus = 'A'
				".( ($_REQUEST['munid']) ? 
					"--join para os municï¿½pios
						INNER JOIN entidade.endereco ed ON ed.estuf = og.estuf
						LEFT JOIN obras.obrainfraestrutura oi ON oi.endid = ed.endid
						INNER JOIN obras.repositorio ore ON ore.obrid = oi.obrid" : "" 
				)."
						    			
				WHERE
					{$whereEmpresa} og.gpdstatus = 'A' 
					{$filtro}
				GROUP BY
					og.gpdid,
					gpddtcriacao,
					usunome,
					og.estuf,
					we.esddsc,
					ra.strid,
					rp.strid,
					og.estuf
					{$goupByEmpresa}	
				ORDER BY
					og.gpdid";

//		$cabecalho = array( "Aï¿½ï¿½o", "Situaï¿½ï¿½o", "Nï¿½ do Grupo", "Data de Inclusï¿½o", "Inserido Por", "UF", "Empresa Contratada", "Situaï¿½ï¿½o da Supervisï¿½o" );
		$cabecalho = array( " Ação ", "Situação", "Nº do Grupo", "Data de Inclusão", "Inserido Por", "UF", "Empresa Contratada", "Situação da Supervisão", "Data da Tramitação", "QTD dia(s) após a Tramitação" );
		
		//$obrMEC ? array_push( $cabecalho, "Empresa Contratada" ) : "";
//		ver($sql, d);
		$this->db->monta_lista( $sql, $cabecalho, 50, 10, "N", "center", "" );
		
	}

	function obrMontaCabecalhoGrupo( $gpdid ){
		
		$sqlRota ="SELECT 
						COUNT(trjid) 
				   FROM 
				   		obras.trajetoria ot 
			   	   WHERE  
			   	   		trjstatus = 'A' 
			   	   		AND epcid = {$gpdid}";		
		$possuiRota = $this->db->carregar( $sqlRota );
		
		if($possuiRota[0]['count'] > 0){
			$innerJoinRota = " INNER JOIN
									obras.trajetoria ot ON ot.itgid = it.itgid";
			$wereRotaStatus = " AND 
									ot.trjstatus = 'A'";
		}

		$sql = "SELECT
					og.gpdid as controle,
					estuf as ufatuacao,
					to_char(gpddtcriacao, 'DD/MM/YYYY') as dtinclusao,
					count(r.repid) as numobras,					e.entnome
				FROM
					obras.grupodistribuicao og
				INNER JOIN
					obras.itemgrupo it ON it.gpdid = og.gpdid
				LEFT JOIN 
					obras.repositorio r ON r.repid = it.repid
										   AND r.repstatus = 'A'	
				INNER JOIN 
					obras.empresacontratada ec ON ec.epcid = og.epcid
				INNER JOIN
					entidade.entidade e ON e.entid = ec.entid
				$innerJoinRota 
				WHERE
					og.gpdid = {$gpdid}
					$wereRotaStatus	
				GROUP BY
					og.gpdid, estuf, dtinclusao, e.entnome";
		
		$dados = $this->db->pegaLinha( $sql );
		
		if( $dados ){
			
			print "<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>"
				. "    <tr>"
				. "        <td colspan='2' class='SubTituloCentro'>Dados do Grupo</td>"
				. "    </tr>"
				. "    <tr>"
				. "        <td class='SubTituloDireita' width='190px'>Nº de Controle:</td>"
				. "        <td><b>{$dados["controle"]}</b></td>"
				. "    </tr>"
				. "    <tr>"
				. "        <td class='SubTituloDireita'>Empresa:</td>"
				. "        <td>{$dados["entnome"]}</td>"
				. "    </tr>"
				. "    <tr>"
				. "        <td class='SubTituloDireita'>UF:</td>"
				. "        <td>{$dados["ufatuacao"]}</td>"
				. "    </tr>"
				. "    <tr>"
				. "        <td class='SubTituloDireita'>Data de Criação:</td>"
				. "        <td>{$dados["dtinclusao"]}</td>"
				. "    </tr>"
				. "    <tr>"
				. "        <td class='SubTituloDireita'>Nº de Obras:</td>"
				. "        <td>{$dados["numobras"]}</td>"
				. "    </tr>"
				. "</table>";
			
		}
		
	}
	
	function obrFiltraListaObrasRota(){
		
		$filtro  = $_REQUEST["orgid"] 		 ? " AND oi.orgid = {$_REQUEST["orgid"]}" 		   	 : "";
		$filtro .= $_REQUEST["obrdesc"] 	 ? " AND obrdesc ilike '%{$_REQUEST["obrdesc"]}%'"   : "";
		$filtro .= $_REQUEST["entidunidade"] ? " AND entidunidade = {$_REQUEST["entidunidade"]}" : "";
		$filtro .= $_REQUEST["stoid"] 		 ? " AND oi.stoid = {$_REQUEST["stoid"]}" 			 : "";
		
		return $filtro; 
		
	}
	
	function obrMontaListaObrasRota( $gpdid, $filtros = "" ){
		
		if( $gpdid ){
			
			$sql = "SELECT
						'<center>
						     <input type=\"checkbox\" id=\"itgid_'|| ig.itgid ||'\" value=\"'|| ig.itgid ||'\" onclick=\"obrIncluiObraRota( ' || ig.itgid || ', \'' || obrdesc || '\' );\"/> 
						 </center>' as acao,
						orgdesc as tipoensino,
						upper(obrdesc) as nome,
						entnome as unidade,
						stodesc as situacaoobra
					FROM
						obras.obrainfraestrutura oi
					INNER JOIN
						obras.repositorio ore ON ore.obrid = oi.obrid
					INNER JOIN
						obras.itemgrupo ig ON ig.repid = ore.repid
					INNER JOIN
						obras.situacaoobra so ON oi.stoid = so.stoid
					INNER JOIN
						entidade.entidade ee ON ee.entid = oi.entidunidade
					INNER JOIN
						obras.orgao oo ON oo.orgid = oi.orgid 
					WHERE
						ig.gpdid = {$gpdid} {$filtros}
					ORDER BY
						itgordem";

			$cabecalho = array( "Ação", "Tipo de Estabelecimento", "Nome da Obra", "Unidade Responsável pela Obra", "Situação da Obra" );
			
			$this->db->monta_lista( $sql, $cabecalho, 100, 10, "N", "center", "", "obrFormObras" );
			
		}		
	}
	
	function obrSalvaRota( $dados ){
		
		
		if( !$dados["rotid"] ){
			
			$sql = "SELECT count(rotid) 
					FROM obras.rotas 
					WHERE rotstatus = 'A' AND gpdid = {$dados["gpdid"]}";
			
			$rotnumero = $this->pegaUm( $sql ); 
			$rotnumero = !$rotnumero ? 1 : $rotnumero + 1; 
			
			$sql = "INSERT INTO obras.rotas( gpdid, strid, 
											 rotnumero, rotdtinclusao, 
											 rotstatus, rotkmdistanciatotal, usucpf, prpid )
									 VALUES( {$dados["gpdid"]}, " . OBRSITROTADEFINIDA . ",
									 		 {$rotnumero}, 'now', 
									 		 'A', null, '{$_SESSION["usucpf"]}', 1 ) 
								  RETURNING rotid";
			
			$rotid = $this->db->pegaUm( $sql );
			
			$_SESSION["obras"]["rotid"] = $rotid;
			
			$seq = 1; 
			
			if( is_array( $dados["itgid"] ) ){
				
				foreach( $dados["itgid"] as $valor ){

					$seq++;
					
					$trjkm    	  = $dados["trjkm"][$valor]    	 ? str_replace( ".", "", $dados["trjkm"][$valor] )  : "0";
					$trjkm    	  = $trjkm    				   	 ? str_replace( ",", ".", $trjkm ) 				  	: "0";
					$trjtempo 	  = $dados["trjtempo"][$valor] 	 ? "'{$dados["trjtempo"][$valor]}'" 				: "''";
					$tdeid    	  = $dados["tdeid"][$valor]    	 ? $dados["tdeid"][$valor] 				  	      	: "NULL";
					
					if( $dados["aprovacao_{$valor}"] == "0" ){
						$trjaprovacao = "'false'";
					}else{
						if( $dados["aprovacao_{$valor}"] == "1" ){
							$trjaprovacao = "'true'";
						}else{
							$trjaprovacao = "NULL";
						}
					}
					
					$trjvlrpedagio = $dados["trjvlrpedagio"][$valor] ? str_replace( ".", "", $dados["trjvlrpedagio"][$valor] ) : "NULL";
					$trjvlrpedagio = $trjvlrpedagio ? str_replace( ",", ".", $trjvlrpedagio ) 								   : "NULL";

					$trjobservacao = $dados["trjobservacao"][$valor] ? "'" . pg_escape_string( $dados["trjobservacao"][$valor] ) . "'" : "''";
					
					// soma o total percorrido
					$rotkmdistanciatotal = $rotkmdistanciatotal + $trjkm;
					
					$sql = "INSERT INTO obras.trajetoria( rotid, itgid, 
														  trjseq, trjkm, 
														  trjtempo, trjstatus, trjdtinclusao, tdeid, trjvlrpedagio, trjobservacao, trjaprovacao )
												  VALUES( {$rotid}, {$valor}, 
														  {$seq}, {$trjkm}, 
														  {$trjtempo}, 'A', 'now', {$tdeid}, {$trjvlrpedagio}, {$trjobservacao}, {$trjaprovacao} )";

					$this->db->executar( $sql );
				}
				
			}
						
		}else{
			
			/*
			$sql = "UPDATE obras.trajetoria SET trjstatus = 'I' WHERE rotid = '{$dados["rotid"]}'";
			$this->db->executar( $sql );
			*/
			
			$seq = 1; 
			
			if( is_array($dados["trjid"])) {
				
				foreach($dados["trjid"] as $entid => $trjid) {
					
					$seq++;
					
					$valor = $this->db->pegaUm("SELECT itgid FROM obras.trajetoria WHERE trjid='".$trjid."'");
					
					$trjkm    = $dados["trjkm"][$valor]    ? str_replace( ".", "", $dados["trjkm"][$valor] )  : "0";
					$trjkm    = $trjkm    				   ? str_replace( ",", ".", $trjkm ) 				  : "0";
					$trjtempo = $dados["trjtempo"][$valor] ? "'{$dados["trjtempo"][$valor]}'" 				  : "NULL";
					$tdeid    = $dados["tdeid"][$valor]    ? $dados["tdeid"][$valor] 				  	      : "NULL";
					
					$trjvlrpedagio = $dados["trjvlrpedagio"][$valor] ? str_replace( ".", "", $dados["trjvlrpedagio"][$valor] ) : "NULL";
					$trjvlrpedagio = $trjvlrpedagio ? str_replace( ",", ".", $trjvlrpedagio ) 								   : "NULL";
					
					$trjobservacao = $dados["trjobservacao"][$valor] ? "'" . pg_escape_string( $dados["trjobservacao"][$valor] ) . "'" : "''";
					
					// soma o total percorrido
					$rotkmdistanciatotal = $rotkmdistanciatotal + $trjkm;
					
					$sql = "UPDATE obras.trajetoria SET trjseq        = ".$seq.", 
														trjkm         = ".$trjkm.", 
														trjtempo      = ".$trjtempo.", 
														tdeid         = ".$tdeid.", 
														trjvlrpedagio = ".$trjvlrpedagio.",
														trjobservacao = ".$trjobservacao."
							WHERE trjid='".$trjid."'";
					
					$this->db->executar($sql);
					
					
				}
				
			}
			
			// Atualizando a coluna Aprovaï¿½ï¿½o
			if( is_array( $dados["itgid"] ) ){
				
				foreach( $dados["itgid"] as $itgid ){
					
					if( $dados["aprovacao_{$itgid}"] == '1' ){
						$valor = "'t'";
					}else{
						
						if( $dados["aprovacao_{$itgid}"] == '0' ){
							$valor = "'f'";
						}else{
							$valor = "NULL";
						}
						
					}
					
					$sql = "UPDATE obras.trajetoria SET trjaprovacao = {$valor}
							WHERE itgid = {$itgid}";
					
					$this->db->executar($sql);
				}// fim do foreach
				
			}
			// Fim da atualizaï¿½ï¿½o na coluna Aprovaï¿½ï¿½o
			
		}
		
		if( $dados["epcid"] ){

			$seq++;
			
			$trjkmempresa    = $dados["trjkmempresa_{$dados["epcid"]}"]    ? str_replace( ".", "", $dados["trjkmempresa_{$dados["epcid"]}"] )  : "0";
			$trjkmempresa    = $trjkmempresa    ? str_replace( ",", ".", $trjkmempresa ) : "0";
			$trjtempoempresa = $dados["trjtempoempresa_{$dados["epcid"]}"] ? "'{$dados["trjtempoempresa_{$dados["epcid"]}"]}'" 				   : "''";
			$tdeidempresa 	 = $dados["tdeidempresa_{$dados["epcid"]}"]    ? $dados["tdeidempresa_{$dados["epcid"]}"] 				  	       : "NULL";
			
			$trjvlrpedagioempresa = $dados["trjvlrpedagioempresa_{$dados["epcid"]}"]    ? str_replace( ".", "", $dados["trjvlrpedagioempresa_{$dados["epcid"]}"] )  : "NULL";
			$trjvlrpedagioempresa = $trjvlrpedagioempresa							    ? str_replace( ",", ".", $trjvlrpedagioempresa )  							: "NULL";
			
			// soma o total percorrido
			$rotkmdistanciatotal = $rotkmdistanciatotal + $trjkmempresa;
			
			if( $dados["aprovacao_{$dados["epcid"]}"] == "0" ){
				$trjaprovacao = "'false'";
			}else{
				if( $dados["aprovacao_{$dados["epcid"]}"] == "1" ){
					$trjaprovacao = "'true'";
				}else{
					$trjaprovacao = "NULL";
				}
			}
		
			if($dados["rotid"]) {
				
				$sql = "UPDATE obras.trajetoria SET trjseq={$seq}, 
													trjkm={$trjkmempresa}, 
													trjtempo={$trjtempoempresa}, 
													tdeid={$tdeidempresa}, 
													trjvlrpedagio={$trjvlrpedagioempresa},
													trjaprovacao={$trjaprovacao} 
						WHERE rotid={$_SESSION["obras"]["rotid"]} AND epcid={$dados["epcid"]}";
				
			} else {
				
				$sql = "INSERT INTO obras.trajetoria( rotid, epcid, 
													  trjseq, trjkm, 
													  trjtempo, trjstatus, trjdtinclusao, tdeid, trjvlrpedagio, trjaprovacao )
											  VALUES( {$_SESSION["obras"]["rotid"]}, {$dados["epcid"]}, 
													  {$seq}, {$trjkmempresa}, 
													  {$trjtempoempresa}, 'A', 'now', {$tdeidempresa}, {$trjvlrpedagioempresa}, {$trjaprovacao} )";
													  
			}
			
			$this->db->executar( $sql );
			
		} elseif($dados["rotid"]) { // corrigindo bugs devido a alteraï¿½ï¿½es do sistema
			
			/*
			 * ESSA CONDIï¿½ï¿½O RARAMENTE SERï¿½ UTILIZADA, ESTE TRECHO CORRIGI CASO A ROTA Nï¿½O POSSUA O ULTIMO TRECHO DE 
			 * VOLTA A EMPRESA CONTRATADA
			 */
			
			$dados["epcid"] = $this->db->pegaUm("SELECT epcid FROM obras.grupodistribuicao WHERE gpdid={$dados["gpdid"]} AND gpdstatus='A'");
			
			$seq++;
			
			$trjkmempresa    = $dados["trjkmempresa_"]    ? str_replace( ".", "", $dados["trjkmempresa_"] )  : "0";
			$trjkmempresa    = $trjkmempresa    ? str_replace( ",", ".", $trjkmempresa ) : "0";
			$trjtempoempresa = $dados["trjtempoempresa_"] ? "'{$dados["trjtempoempresa_"]}'" 				   : "''";
			$tdeidempresa 	 = $dados["tdeidempresa_"]    ? $dados["tdeidempresa_"] 				  	       : "NULL";
			
			$trjvlrpedagioempresa = $dados["trjvlrpedagioempresa_"]    ? str_replace( ".", "", $dados["trjvlrpedagioempresa_"] )  : "NULL";
			$trjvlrpedagioempresa = $trjvlrpedagioempresa							    ? str_replace( ",", ".", $trjvlrpedagioempresa )  							: "NULL";
			
			// soma o total percorrido
			$rotkmdistanciatotal = $rotkmdistanciatotal + $trjkmempresa;
			
			$sql = "INSERT INTO obras.trajetoria( rotid, epcid, 
												  trjseq, trjkm, 
												  trjtempo, trjstatus, trjdtinclusao, tdeid, trjvlrpedagio, trjaprovacao )
										  VALUES( {$_SESSION["obras"]["rotid"]}, {$dados["epcid"]}, 
												  {$seq}, {$trjkmempresa}, 
												  {$trjtempoempresa}, 'A', 'now', {$tdeidempresa}, {$trjvlrpedagioempresa}, {$trjaprovacao} )";
			$this->db->executar($sql);
			
			
		}
		
		/*$sql = "SELECT trjtempo::time FROM obras.trajetoria WHERE rotid = {$_SESSION["obras"]["rotid"]}";
		$rottotaltempo = $this->db->pegaUm( $sql );*/
		
		$sql = "UPDATE obras.rotas SET rotkmdistanciatotal = {$rotkmdistanciatotal} WHERE rotid = {$_SESSION["obras"]["rotid"]}";
		$this->db->executar( $sql );
		
		$this->db->commit();
		$this->db->sucesso( "principal/supervisao/criarRota", "" );
		
	}
	
	function obrListaRotasGrupo( $gpdid, $esdid = "" ){
		
		global $obrSupDisabled;

		$btExcluir = ($esdid == OBREMAVALIAMEC) && ( $this->db->testa_superuser() ) || 
					 ($esdid == OBRDISTRIBUIDO) && ( $this->db->testa_superuser() ) || 
					 ($esdid == OBREMDEFINROTA) && ( $this->db->testa_superuser() ) ||
					 ($esdid == OBREMAVALIAMEC) && (  possuiPerfil( PERFIL_SAA )  ) ||
					 ($esdid == OBRDISTRIBUIDO) && (  possuiPerfil( PERFIL_SAA )  ) ||
					 ($esdid == OBREMDEFINROTA) && (  possuiPerfil( PERFIL_SAA )  ) ? "<img src=\"../imagens/excluir.gif\" style=\"cursor: pointer;\" onclick=\"obrExcluiRota( ' || rotid || ' );\"/>" 
					 																: "<img src=\"../imagens/excluir_01.gif\" />";
		
		
		$btExcluirSim = "<img src=\"../imagens/excluir.gif\" style=\"cursor: pointer;\" onclick=\"obrExcluiRota( ' || rotid || ' );\"/>";
						 			  		    
		$imgSim = "'<center><img src=\"../imagens/check_p.gif\" title=\"Sim\"/></center>'";
		$imgNao = "'<center><img src=\"../imagens/exclui_p.gif\" title=\"Não\"/></center>'";
						 			  
		$sql = "SELECT
					'<center>
					     <img src=\"../imagens/alterar.gif\" style=\"cursor: pointer;\" onclick=\"obrBuscaRota( ' || rotid || ' );\"/>
					     ' || CASE WHEN rt.prpid = 1 THEN ' {$btExcluir} ' ELSE ' {$btExcluirSim} ' END || '
					 </center>' as acao,
					CASE WHEN strid = 1 THEN {$imgSim} ELSE {$imgNao} END as aprovada,
					rotnumero,
					prpdsc as proponente,
					to_char(rotdtinclusao, 'DD/MM/YYYY') as dtinclusao,
					upper(usunome) as usuario,
					rotkmdistanciatotal as total
				FROM
					obras.rotas rt
				INNER JOIN
					obras.proponente op ON op.prpid = rt.prpid
				INNER JOIN
					seguranca.usuario su ON su.usucpf = rt.usucpf
				WHERE
					gpdid = {$gpdid} AND
					rotstatus = 'A'
				ORDER BY
					rotnumero";
		
		$cabecalho = array( "Ação", "Aprovada", "Nº da Rota", "Proponente", "Data de Inclusão", "Criada Por", "Distancia Total (Km)" );
			
		$this->db->monta_lista( $sql, $cabecalho, 100, 10, "N", "center", "" );
			
	}
	
	function obrExcluiRota( $rotid ){
		
		$sql = "DELETE FROM obras.trajetoria WHERE rotid = {$rotid}";
		$this->db->executar( $sql );
		
		$sql = "UPDATE obras.rotas SET rotstatus = 'I' WHERE rotid = {$rotid}";
		$this->db->executar( $sql );
		
		$this->db->commit();
		$this->db->sucesso( "principal/supervisao/listaDeRotas", "" );
		
	}
	
	
	function obrBuscaProponente( $rotid ){
		
		$sql = "SELECT prpid FROM obras.rotas WHERE rotid = {$rotid}";
		return $this->db->pegaUm( $sql );
		
	}
	
	function obrMontaListaTrajetorias( $gpdid, $rotid = "", $esdid = "", $obrProponente ){
		if( $rotid ){
			
			$select = ", trjkm as km,
					   trjtempo as tempo,
					   tdeid as deslocamento,
					   trjvlrpedagio as pedagio,
					   trjobservacao as obs,
					   trjid,
					   ot.trjaprovacao";
			
			$join = "INNER JOIN
						 obras.trajetoria ot ON ot.itgid = it.itgid";
					 
			$where = "AND trjstatus = 'A' AND ot.rotid = {$rotid}";
			
			$order = "ORDER BY
					      trjseq";
			
		}
		
		$rotAprovada = $this->obrVerRotaAprovada($gpdid);
		
		$disabled = ( ($esdid == OBREMAVALIAMEC && $obrProponente == 1) || $rotAprovada ) ? "disabled=disabled" : "";
		//$disabled = ( $esdid == OBREMAVALIAMEC && $obrProponente == 1 && $rotAprovada ) ? "readOnly='readOnly'" : "";
		//$disabled = $this->db->testa_superuser() ? "" : $disabled; era assim
		$disabled = ( ( $esdid == OBREMDEFINROTA ) ? "" : "disabled=disabled" );
		$disabled = possuiPerfil( array( PERFIL_SUPERUSUARIO, PERFIL_SAA, PERFIL_ADMINISTRADOR ) ) ? "" : $disabled; // agora ï¿½ assim
		
		// verificando os perfis para a coluna Aprovaï¿½ï¿½o
		if( possuiPerfil( array( PERFIL_SAA ) ) ){
			$desabilita_aprovacao = "";
		}else{
			$desabilita_aprovacao = " disabled=disabled";
		}
		
		$sql = "SELECT
					oi.obrid as obra,
					it.itgid as id, 
					'('|| oi.obrid ||') '|| upper(obrdesc) ||'' as obrdesc,	
					oi.endid as endereco,
					oi.endid as entid,
					ed.endcep,
					ed.endlog,
					ed.endcom,
					ed.endbai,
					ed.estuf,
					ed.medlatitude,
					ed.medlongitude,
					ed.endzoom,
					mun.muncod,
					mun.mundescricao,					
						 --############### LATITUDE ###################### --
					CASE WHEN (SPLIT_PART(munmedlat, '.', 1) <>'' AND SPLIT_PART(munmedlat, '.', 2) <>'' AND split_part(munmedlat, '.', 3) <>'') THEN
		               CASE WHEN split_part(munmedlat, '.', 4) <>'N' THEN
		                   (((split_part(munmedlat, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlat, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlat, '.', 1)::int)))*(-1)
		                ELSE
		                   ((SPLIT_PART(munmedlat, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlat, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlat, '.', 1)::int))
		               END
		            ELSE
		            -- Valores do IBGE convertidos em  decimal
		            CASE WHEN (length (munmedlat)=8) THEN
		                CASE WHEN length(REPLACE('0' || munmedlat,'S','')) = 8 THEN
		                    ((SUBSTR(REPLACE('0' || munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || munmedlat,'S',''),1,2)::double precision))*(-1)
		                ELSE
		                    (SUBSTR(REPLACE('0' || munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || munmedlat,'N',''),1,2)::double precision)
		                END
		            ELSE
		                CASE WHEN length(REPLACE(munmedlat,'S','')) = 8 THEN
		                   ((SUBSTR(REPLACE(munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(munmedlat,'S',''),1,2)::double precision))*(-1)
		                ELSE
		                  0--((SUBSTR(REPLACE(munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE(munmedlat,'N',''),1,2)::double precision))
		                END
		            END
		            END as latitude,
		            --############### FIM LATITUDE ###################### --

		            --############### LONGITUDE ###################### --
		            CASE WHEN (SPLIT_PART(munmedlog, '.', 1) <>'' AND SPLIT_PART(munmedlog, '.', 2) <>'' AND split_part(munmedlog, '.', 3) <>'') THEN
		               ((split_part(munmedlog, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlog, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlog, '.', 1)::int))*(-1)
		            ELSE
		                -- Valores do IBGE convertidos em  decimal
		               (SUBSTR(REPLACE(munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(munmedlog,'W',''),3,2)::double precision/60)) *(-1)
		            END as longitude,
		            --############### FIM LONGITUDE ###################### --
					ee.entnome
					{$select}
				FROM 
					obras.obrainfraestrutura oi
				INNER JOIN
					entidade.entidade ee ON ee.entid = oi.entidunidade
				INNER JOIN
					entidade.endereco ed ON oi.endid = ed.endid
				INNER JOIN
					territorios.municipio mun ON mun.muncod = ed.muncod
				INNER JOIN
					obras.repositorio ore ON ore.obrid = oi.obrid
											 --AND ore.repstatus = 'A'	
				INNER JOIN
					obras.itemgrupo it ON it.repid = ore.repid
				{$join}
				WHERE 
					oi.obsstatus = 'A'
					AND it.gpdid = {$gpdid}
					{$where}
				{$order}";
			 
		$obras = $this->db->carregar( $sql );
		
		if( is_array($obras) ){
			
			print  "<input type='hidden' name='entid' id='entid' value='{$obras[0]["entid"]}'/>";
			
			for( $i = 0; $i < count($obras); $i++ ){
				
				$icondettrjalter = (($obras[$i]["deslocamento"] == 3)?"src='../imagens/alterar.gif' style=cursor:pointer;":"src='../imagens/alterar_01.gif'");

				if( count($obras) == 1 ){
					
					$comboTrajetoObra = "<select id='tdeid_{$obras[$i]["entid"]}' name='tdeid[{$obras[$i]["id"]}]' class='campoestilo' onchange=verificarTipoDeslocamento(this); {$disabled}>"
									  . "	<option value=''>Selecione...</option>"
									  . "	<option value='1' " . ( $obras[$i]["deslocamento"] == 1 ? "selected='selected'" : "" ) . ">Transporte Rodoviário - Trajeto único</option>"
									  . "	<option value='3' " . ( $obras[$i]["deslocamento"] == 3 ? "selected='selected'" : "" ) . ">Transporte não Rodoviário - Trajeto Alternativo</option>"
									  . "</select>";
					
					
									  
				}else{
					
					$comboTrajetoObra = "<select id='tdeid_{$obras[$i]["entid"]}' name='tdeid[{$obras[$i]["id"]}]' class='campoestilo' onchange=verificarTipoDeslocamento(this); {$disabled}>"
									  . "	<option value=''>Selecione...</option>"
									  . "	<option value='2' " . ( $obras[$i]["deslocamento"] == 2 ? "selected='selected'" : "" ) . ">Transporte Rodoviário - Trajeto por Roteiro</option>"
									  . "	<option value='3' " . ( $obras[$i]["deslocamento"] == 3 ? "selected='selected'" : "" ) . ">Transporte não Rodoviário - Trajeto Alternativo</option>"
									  . "</select>";
									  
					
									    
				}
				
				$cor = ( $i % 2 ) ? "#e0e0e0" : "#f4f4f4";
				
				// condiï¿½ï¿½es das setas para ordenaï¿½ï¿½o
				$setaCima = ($i < 1) ? "<img src='/imagens/seta_cimad.gif' border='0' title='Sobe'/>&nbsp;" : 
				 					   "<img src='/imagens/seta_cima.gif' title='Sobe' style='cursor:pointer;' border='0' onclick='obrTrocaPrioridade(\"sobe\", this);'/>&nbsp;";

				$setaBaixo = ( ($i + 1) == count($obras)) ? "<img src='/imagens/seta_baixod.gif' border='0' title='Desce'/>&nbsp;" : 
				 						  				 	"<img src='/imagens/seta_baixo.gif' title='Desce' style='cursor:pointer;' border='0' onclick='obrTrocaPrioridade(\"desce\", this);'/>&nbsp;";
				
				$setaCima  = ($esdid == OBREMAVALIAMEC && $obrProponente == 1 ) ? "<img src='/imagens/seta_cimad.gif' border='0'/>"  : $setaCima;
				$setaBaixo = ($esdid == OBREMAVALIAMEC && $obrProponente == 1 ) ? "<img src='/imagens/seta_baixod.gif' border='0'/>" : $setaBaixo;
				
				// formata a km
				$obras[$i]["km"] = number_format( $obras[$i]["km"], 1, ",", "." );
				
				// formata o vlr do pedagio
				$obras[$i]["pedagio"] = number_format( $obras[$i]["pedagio"], 2, ",", "." );

				//Longitude
				if($obras[$i]['medlongitude']){
					$lon[$i] = explode(".",$obras[$i]['medlongitude']);
					$graulongitude[$i] 	= $lon[$i][0];
					$minlongitude[$i] 	= $lon[$i][1];
					$seglongitude[$i]	= $lon[$i][2];
					$pololongitude[$i] 	= $lon[$i][3];
				}
				
				//Latitude
				if($obras[$i]['medlatitude']){
					$lat[$i] = explode(".",$obras[$i]['medlatitude']);
					$graulatitude[$i] 	=  $lat[$i][0];
					$minlatitude[$i]	=  $lat[$i][1];
					$seglatitude[$i] 	=  $lat[$i][2];
					$pololatitude[$i] 	=  $lat[$i][3];
				}
				
				$arrTrajetos[] = $obras[$i]["entid"];
				
				if($rotid)
					$valorcomposicaotrajetoria = $this->db->pegaUm("SELECT SUM(ctjvalor) FROM obras.composicaotrajetoria WHERE trjid='".$obras[$i]["trjid"]."'");

					
				// Coluna Aprovaï¿½ï¿½o
				$aprovacao = "<label><input id='aprovacao_{$obras[$i]["entid"]}' type='radio' name='aprovacao_{$obras[$i]["id"]}' value='1'{$desabilita_aprovacao}>&nbsp;Sim</label>
				    		  <label><input id='aprovacao_{$obras[$i]["entid"]}' type='radio' name='aprovacao_{$obras[$i]["id"]}' value='0'{$desabilita_aprovacao}>&nbsp;Não</label>";
					
				if( isset($obras[$i]["trjaprovacao"]) ){
					
					if( $obras[$i]["trjaprovacao"] == 't' ){
						$aprovacao = "<label><input id='aprovacao_{$obras[$i]["entid"]}' type='radio' name='aprovacao_{$obras[$i]["id"]}' value='1' checked='checked'{$desabilita_aprovacao}>&nbsp;Sim</label>
				    		  	  	  <label><input id='aprovacao_{$obras[$i]["entid"]}' type='radio' name='aprovacao_{$obras[$i]["id"]}' value='0'{$desabilita_aprovacao}>&nbsp;Não</label>";
					}elseif( $obras[$i]["trjaprovacao"] == 'f' ){
						$aprovacao = "<label><input id='aprovacao_{$obras[$i]["entid"]}' type='radio' name='aprovacao_{$obras[$i]["id"]}' value='1'{$desabilita_aprovacao}>&nbsp;Sim</label>
				    		  	  	  <label><input id='aprovacao_{$obras[$i]["entid"]}' type='radio' name='aprovacao_{$obras[$i]["id"]}' value='0' checked='checked'{$desabilita_aprovacao}>&nbsp;Não</label>";
					}
				}// fim do if Coluna Aprovaï¿½ï¿½o
				
				print "<tr bgcolor='{$cor}'>"
					. "    <td style='text-align:center;'>" . ( $i + 2 ) . "</td>"
					. "	   <td style='text-align: center;'>"
					. 		  $aprovacao
					. "	   </td>"
					. "    <td>"
					. "        <input type='hidden' name='trjid[{$obras[$i]["entid"]}]' id='trjid_{$obras[$i]["entid"]}' value='{$obras[$i]["trjid"]}'/>"
					. "        <input type='hidden' name='itgid[{$obras[$i]["id"]}]' id='itgid_{$obras[$i]["id"]}' value='{$obras[$i]["id"]}'/>"
					. "        <input type='hidden' name='entid[{$obras[$i]["entid"]}]' id='entid{$obras[$i]["entid"]}' value='{$obras[$i]["entid"]}'/>"
					. "        <input type='hidden' name='endcep[{$obras[$i]["entid"]}]' id='endcep{$obras[$i]["entid"]}' value='{$obras[$i]["endcep"]}'/>"
					. "        <input type='hidden' name='mundescricao[{$obras[$i]["entid"]}]' id='mundescricao{$obras[$i]["entid"]}' value='{$obras[$i]["mundescricao"]}'/>"
					. "        <input type='hidden' name='estuf[{$obras[$i]["entid"]}]' id='estuf{$obras[$i]["entid"]}' value='{$obras[$i]["estuf"]}'/>"
					. "        <input type='hidden' name='endbai[{$obras[$i]["entid"]}]' id='endbai{$obras[$i]["entid"]}' value='{$obras[$i]["endbai"]}'/>"
					. "        <input type='hidden' name='endzoom[{$obras[$i]["entid"]}]' id='endzoom{$obras[$i]["entid"]}' value='{$obras[$i]["endzoom"]}'/>"
					. "        <input type='hidden' name='graulongitude[{$obras[$i]["entid"]}]' id='graulongitude{$obras[$i]["entid"]}' value='{$graulongitude[$i]}'/>"
					. "        <input type='hidden' name='minlongitude[{$obras[$i]["entid"]}]' id='minlongitude{$obras[$i]["entid"]}' value='{$minlongitude[$i]}'/>"
					. "        <input type='hidden' name='seglongitude[{$obras[$i]["entid"]}]' id='seglongitude{$obras[$i]["entid"]}' value='{$seglongitude[$i]}'/>"
					. "        <input type='hidden' name='pololongitude[{$obras[$i]["entid"]}]' id='pololongitude{$obras[$i]["entid"]}' value='{$pololongitude[$i]}'/>"
					. "        <input type='hidden' name='longitude[{$obras[$i]["entid"]}]' id='longitude{$obras[$i]["entid"]}' value='{$obras[$i]["longitude"]}'/>"
					. "        <input type='hidden' name='graulatitude[{$obras[$i]["entid"]}]' id='graulatitude{$obras[$i]["entid"]}' value='{$graulatitude[$i]}'/>"
					. "        <input type='hidden' name='minlatitude[{$obras[$i]["entid"]}]' id='minlatitude{$obras[$i]["entid"]}' value='{$minlatitude[$i]}'/>"
					. "        <input type='hidden' name='seglatitude[{$obras[$i]["entid"]}]' id='seglatitude{$obras[$i]["entid"]}' value='{$seglatitude[$i]}'/>"
					. "        <input type='hidden' name='pololatitude[{$obras[$i]["entid"]}]' id='pololatitude{$obras[$i]["entid"]}' value='{$pololatitude[$i]}'/>"
					. "        <input type='hidden' name='latitude[{$obras[$i]["entid"]}]' id='latitude{$obras[$i]["entid"]}' value='{$obras[$i]["latitude"]}'/>"
					. "        <img src='../imagens/globo_terrestre.png' onclick=\"abreRotaObras(".($i + 1).")\" style='vertical-align:middle; cursor: pointer;' title='Visualizar Mapa' />"
					. "        <img src='../imagens/consultar.gif' style='vertical-align:middle; cursor: pointer;' title='Dados da Obra' onclick='obrVerDados({$obras[$i]["obra"]}, \"obra\");'/>"
					. "        <img src='".( $obras[$i]["obs"] ? "../imagens/editar_nome_vermelho.gif" : "../imagens/editar_nome.gif" )."' style='vertical-align:middle; cursor: pointer;' title='Inserir Observação' onclick='obrInserirObs({$obras[$i]["entid"]});'/>"
					. "        {$obras[$i]["obrdesc"]}"
					. "        <input type='hidden' value='{$obras[$i]["obs"]}' id='trjobservacao_{$obras[$i]["entid"]}' name='trjobservacao[{$obras[$i]["id"]}]'/>"
					. "    </td>"
					. "    <td style='text-align: left;'>"
					. "    {$obras[$i]["mundescricao"]}"
					. "    </td>"
					. "    <td style='text-align: center;'>"
					.          $comboTrajetoObra
					. "   </td>"
					. "    <td style='text-align: center;'>".(($rotid)?"<img ".$icondettrjalter." onclick='detalharTrajAlternativa(".$obras[$i]["trjid"].", this);' align=absmiddle> <input type='text' class='disabled' size=10 id=imgtrj_".$obras[$i]["trjid"]." value='".(($obras[$i]["deslocamento"] == 3 && $valorcomposicaotrajetoria)?number_format($valorcomposicaotrajetoria,2,",","."):"")."'>":"-")."</td>"
					. "    <td style='text-align: center;'>"
					. "        <input type='text' onchange=\"contabilizaKM()\" onkeyup='this.value=mascaraglobal(\"[.###],#\",this.value);' onblur='MouseBlur(this);contabilizaKM()' 
							   onmouseout='MouseOut(this);' onfocus='MouseClick(this);this.select();' id='trjkm_{$obras[$i]["entid"]}' 
							   name='trjkm[{$obras[$i]["id"]}]' size='12' maxlength='8' value='{$obras[$i]["km"]}' class='normal' 
							   style='width: 15ex; text-align: left;' title='' {$disabled}/>"
					. "    </td>"
					. "    <td style='text-align: center;'>"
					. "        <input type='text' onkeyup='this.value=mascaraglobal(\"#.###,##\",this.value);' onblur='MouseBlur(this);contabilizaPedagio();' 
							   onmouseout='MouseOut(this);' onfocus='MouseClick(this);this.select();' id='trjvlrpedagio_{$obras[$i]["entid"]}' 
							   name='trjvlrpedagio[{$obras[$i]["id"]}]' size='11' maxlength='8' value='{$obras[$i]["pedagio"]}' class='normal' 
							   style='width: 12ex; text-align: left;' title='' {$disabled}/>"
					. "    </td>"
					. "    <td style='text-align: center;'>"
					. "        <input type='text' onkeyup='this.value=mascaraglobal(\"##:##\",this.value);' onblur='MouseBlur(this);contabilizaTempo()' 
							   onmouseout='MouseOut(this);' onfocus='MouseClick(this);this.select();' id='trjtempo_{$obras[$i]["entid"]}' 
							   name='trjtempo[{$obras[$i]["id"]}]' size='7' maxlength='5' value='{$obras[$i]["tempo"]}' class='normal' 
							   style='width: 10ex; text-align: left;' title='' {$disabled}/>" 
					. "    </td>"
					. "    <td style='text-align: center;'>"
					. "        {$setaCima}{$setaBaixo}"
					. "    </td>"
					. "</tr>";
					
			}
			
			if($rotid){
				
				$select = ",
							trjkm as km,
							trjtempo as tempo,
							tdeid as deslocamento,
							trjvlrpedagio as pedagio,
					   		trjobservacao as obs,
					   		ot.trjaprovacao";
				
				$join = "LEFT JOIN
							obras.trajetoria ot ON ot.epcid = ec.epcid";
				
				$where = " AND ot.rotid = {$rotid} AND trjstatus = 'A'";
				
			}
			
			$sql = "SELECT
						ec.epcid as id,
						ee.entid as empresa,
						entnome as nome,
						ed.endcep,
						ed.endlog,
						ed.endcom,
						ed.endbai,
						ed.estuf,
						ed.medlatitude,
						ed.medlongitude,
						ed.endzoom,
						mun.muncod,
						mun.mundescricao,
						".(($rotid)?"trjid,":"")."
							 --############### LATITUDE ###################### --
						CASE WHEN (SPLIT_PART(munmedlat, '.', 1) <>'' AND SPLIT_PART(munmedlat, '.', 2) <>'' AND split_part(munmedlat, '.', 3) <>'') THEN
			               CASE WHEN split_part(munmedlat, '.', 4) <>'N' THEN
			                   (((split_part(munmedlat, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlat, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlat, '.', 1)::int)))*(-1)
			                ELSE
			                   ((SPLIT_PART(munmedlat, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlat, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlat, '.', 1)::int))
			               END
			            ELSE
			            -- Valores do IBGE convertidos em  decimal
			            CASE WHEN (length (munmedlat)=8) THEN
			                CASE WHEN length(REPLACE('0' || munmedlat,'S','')) = 8 THEN
			                    ((SUBSTR(REPLACE('0' || munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || munmedlat,'S',''),1,2)::double precision))*(-1)
			                ELSE
			                    (SUBSTR(REPLACE('0' || munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || munmedlat,'N',''),1,2)::double precision)
			                END
			            ELSE
			                CASE WHEN length(REPLACE(munmedlat,'S','')) = 8 THEN
			                   ((SUBSTR(REPLACE(munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(munmedlat,'S',''),1,2)::double precision))*(-1)
			                ELSE
			                  0--((SUBSTR(REPLACE(munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE(munmedlat,'N',''),1,2)::double precision))
			                END
			            END
			            END as latitude,
			            --############### FIM LATITUDE ###################### --
	
			            --############### LONGITUDE ###################### --
			            CASE WHEN (SPLIT_PART(munmedlog, '.', 1) <>'' AND SPLIT_PART(munmedlog, '.', 2) <>'' AND split_part(munmedlog, '.', 3) <>'') THEN
			               ((split_part(munmedlog, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlog, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlog, '.', 1)::int))*(-1)
			            ELSE
			                -- Valores do IBGE convertidos em  decimal
			               (SUBSTR(REPLACE(munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(munmedlog,'W',''),3,2)::double precision/60)) *(-1)
			            END as longitude
			            --############### FIM LONGITUDE ###################### --
						{$select}
					FROM
						entidade.entidade ee
					INNER JOIN
						entidade.endereco ed ON ee.entid = ed.entid
					INNER JOIN
						obras.empresacontratada ec ON ec.entid = ee.entid
					INNER JOIN
						obras.empresaufatuacao oe ON oe.epcid = ec.epcid
					INNER JOIN
						territorios.municipio mun ON mun.muncod = oe.muncod
					INNER JOIN
						obras.grupodistribuicao og ON og.epcid = ec.epcid 
													  AND og.estuf = oe.estuf
													  AND og.gpdstatus = 'A'
					{$join}
					WHERE
						og.gpdid = {$_SESSION["obras"]["gpdid"]}
						{$where}";
		
			$empresa = $this->db->pegaLinha( $sql );
			
			$icondettrjalter = (($empresa["deslocamento"] == 3)?"src='../imagens/alterar.gif' style=cursor:pointer;":"src='../imagens/alterar_01.gif'");
			
			if( count($obras) == 1 ){
				
				$comboTrajetoEmpresa = "<select id='tdeid_{$empresa["id"]}' name='tdeidempresa_{$empresa["id"]}' class='campoestilo' onchange=verificarTipoDeslocamento(this); {$disabled}>"
									    . "	   <option value=''>Selecione...</option>"
									    . "	   <option value='1' " . ( $empresa["deslocamento"] == 1 ? "selected='selected'" : "" ) . ">Transporte Rodoviário - Trajeto único</option>"
									    . "	   <option value='3' " . ( $empresa["deslocamento"] == 3 ? "selected='selected'" : "" ) . ">Transporte não Rodoviário - Trajeto Alternativo</option>"
									    . "</select>";
				
			}else{
									    
				$comboTrajetoEmpresa = "<select id='tdeid_{$empresa["id"]}' name='tdeidempresa_{$empresa["id"]}' class='campoestilo' onchange=verificarTipoDeslocamento(this); {$disabled}>"
									    . "	   <option value=''>Selecione...</option>"
									    . "	   <option value='2' " . ( $empresa["deslocamento"] == 2 ? "selected='selected'" : "" ) . ">Transporte Rodoviário - Trajeto por Roteiro</option>"
									    . "	   <option value='3' " . ( $empresa["deslocamento"] == 3 ? "selected='selected'" : "" ) . ">Transporte não Rodoviário - Trajeto Alternativo</option>"
									    . "</select>";
				
			}
			
			$arrTrajetos[] = $empresa["empresa"];
			
			// formata a km
			$empresa["km"] = number_format( $empresa["km"], 1, ",", "." );

			$empresa["pedagio"] = number_format( $empresa["pedagio"], 2, ",", "." );
			
			$cor = ($cor == "#e0e0e0") ? "#f4f4f4" : "#e0e0e0";
			
			if($rotid && $empresa["trjid"])
				$valorcomposicaotrajetoria = $this->db->pegaUm("SELECT SUM(ctjvalor) FROM obras.composicaotrajetoria WHERE trjid='".$empresa["trjid"]."'");
			
			// Coluna Aprovaï¿½ï¿½o
			$aprovacao = "<label><input id='aprovacao_{$empresa["empresa"]}' type='radio' name='aprovacao_{$empresa["id"]}' value='1'{$desabilita_aprovacao}>&nbsp;Sim</label>
			    		  <label><input id='aprovacao_{$empresa["empresa"]}' type='radio' name='aprovacao_{$empresa["id"]}' value='0'{$desabilita_aprovacao}>&nbsp;Não</label>";
				
			if( isset($empresa["trjaprovacao"]) ){
				
				if( $empresa["trjaprovacao"] == 't' ){
					$aprovacao = "<label><input id='aprovacao_{$empresa["empresa"]}' type='radio' name='aprovacao_{$empresa["id"]}' value='1' checked='checked'{$desabilita_aprovacao}>&nbsp;Sim</label>
			    		  	  	  <label><input id='aprovacao_{$empresa["empresa"]}' type='radio' name='aprovacao_{$empresa["id"]}' value='0'{$desabilita_aprovacao}>&nbsp;Não</label>";
				}elseif( $empresa["trjaprovacao"] == 'f' ){
					$aprovacao = "<label><input id='aprovacao_{$empresa["empresa"]}' type='radio' name='aprovacao_{$empresa["id"]}' value='1'{$desabilita_aprovacao}>&nbsp;Sim</label>
			    		  	  	  <label><input id='aprovacao_{$empresa["empresa"]}' type='radio' name='aprovacao_{$empresa["id"]}' value='0' checked='checked'{$desabilita_aprovacao}>&nbsp;Não</label>";
				}
			}// fim do if Coluna Aprovaï¿½ï¿½o
				
			print "<tr bgcolor='{$cor}'>"
				. "    <td style='text-align:center;'>" . ( $i + 2 ) . "</td>"
				. "	   <td style='text-align: center;'>"
				.		   $aprovacao
				. "	   </td>"
				. "    <td>"
				. "        <input type='hidden' name='epcid' id='epcid' value='{$empresa["id"]}'/>"
				. "        <input type='hidden' name='entid[{$empresa["empresa"]}]' id='entid{$empresa["empresa"]}' value='{$empresa["empresa"]}'/>"
				. "        <input type='hidden' name='endcep[{$empresa["empresa"]}]' id='endcep{$empresa["empresa"]}' value='{$empresa["endcep"]}'/>"
				. "        <input type='hidden' name='mundescricao[{$empresa["empresa"]}]' id='mundescricao{$empresa["empresa"]}' value='{$empresa["mundescricao"]}'/>"
				. "        <input type='hidden' name='estuf[{$empresa["empresa"]}]' id='estuf{$empresa["empresa"]}' value='{$empresa["estuf"]}'/>"
				. "        <input type='hidden' name='endbai[{$empresa["empresa"]}]' id='endbai{$empresa["empresa"]}' value='{$empresa["endbai"]}'/>"
				. "        <input type='hidden' name='endzoom[{$empresa["empresa"]}]' id='endzoom{$empresa["empresa"]}' value='{$empresa["endzoom"]}'/>"
				. "        <input type='hidden' name='graulongitude[{$empresa["empresa"]}]' id='graulongitude{$empresa["empresa"]}' value='{$graulongitude[$empresa["empresa"]]}'/>"
				. "        <input type='hidden' name='minlongitude[{$empresa["empresa"]}]' id='minlongitude{$empresa["empresa"]}' value='{$minlongitude[$empresa["empresa"]]}'/>"
				. "        <input type='hidden' name='seglongitude[{$empresa["empresa"]}]' id='seglongitude{$empresa["empresa"]}' value='{$seglongitude[$empresa["empresa"]]}'/>"
				. "        <input type='hidden' name='pololongitude[{$empresa["empresa"]}]' id='pololongitude{$empresa["empresa"]}' value='{$pololongitude[$empresa["empresa"]]}'/>"
				. "        <input type='hidden' name='longitude[{$empresa["empresa"]}]' id='longitude{$empresa["empresa"]}' value='{$empresa["longitude"]}'/>"
				. "        <input type='hidden' name='graulatitude[{$empresa["empresa"]}]' id='graulatitude{$empresa["empresa"]}' value='{$graulatitude[$empresa["empresa"]]}'/>"
				. "        <input type='hidden' name='minlatitude[{$empresa["empresa"]}]' id='minlatitude{$empresa["empresa"]}' value='{$minlatitude[$empresa["empresa"]]}'/>"
				. "        <input type='hidden' name='seglatitude[{$empresa["empresa"]}]' id='seglatitude{$empresa["empresa"]}' value='{$seglatitude[$empresa["empresa"]]}'/>"
				. "        <input type='hidden' name='pololatitude[{$empresa["empresa"]}]' id='pololatitude{$empresa["empresa"]}' value='{$pololatitude[$empresa["empresa"]]}'/>"
				. "        <input type='hidden' name='latitude[{$empresa["empresa"]}]' id='latitude{$empresa["empresa"]}' value='{$empresa["latitude"]}'/>"
				. "        <img src='../imagens/globo_terrestre.png' onclick=\"abreRotaObras(".($i + 1).")\" style='vertical-align:middle; cursor: pointer; margin-right: 34px;' title='Visualizar Mapa'/>"
				. "        {$empresa["nome"]}"
				. "    </td>"
				. "    <td style='text-align: left;'>"
				. "    {$empresa["mundescricao"]}"
				. "    </td>"
				. "    <td style='text-align: center;'>"
				.          $comboTrajetoEmpresa 
				. " 	</td>"
				. "    <td style='text-align: center;'>".(($rotid)?"<img ".$icondettrjalter." onclick='detalharTrajAlternativa(".$empresa["trjid"].", this);' align=absmiddle> <input type='text' class='disabled' id=imgtrj_".$empresa["trjid"]." size=10 value='".(($empresa["deslocamento"] == 3 && $valorcomposicaotrajetoria)?number_format($valorcomposicaotrajetoria,2,",","."):"")."'>":"")."</td>"
				. "    <td style='text-align: center;'>"
				. "        <input type='text' onchange=\"contabilizaKM()\" onkeyup='this.value=mascaraglobal(\"[.###],#\",this.value);' onblur='MouseBlur(this);contabilizaKM()' 
						   onmouseout='MouseOut(this);' onfocus='MouseClick(this);this.select();' id='trjkm_{$empresa["empresa"]}' 
						   name='trjkmempresa_{$empresa["id"]}' size='12' maxlength='8' value='{$empresa["km"]}' class='normal' 
						   style='width: 15ex; text-align: left;' title='' {$disabled}/>"
				. "    </td>"
				. "    <td style='text-align: center;'>"
				. "        <input type='text' onkeyup='this.value=mascaraglobal(\"#.###,##\",this.value);' onblur='MouseBlur(this);contabilizaPedagio();' 
						   onmouseout='MouseOut(this);' onfocus='MouseClick(this);this.select();' id='trjvlrpedagio_{$empresa["empresa"]}' 
						   name='trjvlrpedagioempresa_{$empresa["id"]}' size='11' maxlength='8' value='{$empresa["pedagio"]}' class='normal' 
						   style='width: 12ex; text-align: left;' title='' {$disabled}/>"
				. "    </td>"
				. "    <td style='text-align: center;'>"
				. "        <input type='text' onkeyup='this.value=mascaraglobal(\"##:##\",this.value);' onblur='MouseBlur(this);contabilizaTempo()' 
						   onmouseout='MouseOut(this);' onfocus='MouseClick(this);this.select();' id='trjtempo_{$empresa["empresa"]}' 
						   name='trjtempoempresa_{$empresa["id"]}' size='7' maxlength='5' value='{$empresa["tempo"]}' class='normal' 
						   style='width: 10ex; text-align: left;' title='' {$disabled}/>" 
				. "    </td>"
				. "    <td style='text-align: center;'> - </td>"
				. "</tr>";
				
				array_unshift($arrTrajetos, $empresa["empresa"] );
				
				print "<tr bgcolor=\"#c9c9c9\" >"
				. "    <td style='text-align:center;font-weight:bold'>Total</td>"
				. "	   <td style='text-align: center;'>-</td>"
				. "    <td style=\"text-align:left\" >"
				. "    <img src='../imagens/globo_terrestre.png' onclick=\"abreRotaObras('total')\" style='vertical-align:middle; cursor: pointer;' title='Visualizar Mapa'/>" 
				. " 	Rota Completa</td>"
				. "    <td style=\"text-align:center\" >-</td>"
				. "    <td style=\"text-align:center\" >-</td>"
				. "    <td style=\"text-align:center\" >-</td>"
				. "    <td style='text-align: center;'>"
				. "        <input type='text' onchange='this.value=mascaraglobal(\"[.###],#\",this.value);' onblur='MouseBlur(this);' 
						   onmouseout='MouseOut(this);' onfocus='MouseClick(this);this.select();' id='trjkm_total' 
						   name='trjkm_total' size='12' maxlength='8' value='0' class='normal' 
						   style='width: 15ex; text-align: left;' readonly=\"readonly\" />"
				. "    </td>"
				. "    <td style='text-align: center;'>"
				. "        <input type='text' onkeyup='this.value=mascaraglobal(\"#.###,##\",this.value);' onblur='MouseBlur(this);' 
						   onmouseout='MouseOut(this);' onfocus='MouseClick(this);this.select();' id='trjvlrpedagio_total' 
						   name='trjvlrpedagio_total' size='11' maxlength='8' value='0,00' class='normal' 
						   style='width: 12ex; text-align: left;' title='' readonly=\"readonly\"/>"
				. "    </td>"
				. "    <td style='text-align: center;'>"
				. "        <input type='text' onchange='this.value=mascaraglobal(\"####:##\",this.value);' onblur='MouseBlur(this);' 
						   onmouseout='MouseOut(this);' onfocus='MouseClick(this);this.select();' id='trjtempo_total' 
						   name='trjtempo_total' size='7' maxlength='6' value='0' class='normal' 
						   style='width: 10ex; text-align: left;' readonly=\"readonly\" />"
				. "    </td>"
				. "    <td style=\"text-align:center\" >-</td>"
				. "</tr>"
				." <input type=\"hidden\" name=\"trajetos_id\" id=\"trajetos_id\" value=\"".implode(",",$arrTrajetos)."\" />";
			
		}
		// div que irï¿½ armazenar os valores bloqueados
		echo "<div id='bloqueados'></div>";
	}
	
	function obrVerRotaAprovada( $gpdid ){
		
		$sql = "SELECT rotid FROM obras.rotas WHERE gpdid = {$gpdid} AND strid = 1 AND rotstatus = 'A'";
		return $this->db->pegaUm( $sql );
		
	}
	
	function obrGrupoVinculoOS( $gpdid ){
		
		if ( !$gpdid ){
			return false;
		}
		
		$sql = "SELECT
					count(o.orsid) AS total
				FROM
					obras.grupodistribuicao g
				JOIN obras.ordemservico o ON o.gpdid = g.gpdid
											 AND o.orsstatus = 'A'
				WHERE
					g.gpdid = {$gpdid}";
		
		return $this->db->pegaUm( $sql );
	}
	
	function obrAprovaRota( $rotid ){
		
		
		$sql = "SELECT gpdid FROM obras.rotas WHERE rotid = {$rotid}";
		$gpdid = $this->db->pegaUm( $sql );
		
		$sql = "UPDATE obras.rotas SET strid = 2 WHERE gpdid = {$gpdid}";
		$this->db->executar( $sql );
		
		$sql = "UPDATE obras.rotas SET strid = 1 WHERE rotid = {$rotid}";
		$this->db->executar( $sql );
		
		$this->db->commit( );
		$this->db->sucesso( "principal/supervisao/criarRota", "" );
		
	}
	
	function obrCancelaAprovacaoRota( $gpdid ){
		
		$sql = "UPDATE obras.rotas SET strid = 3 WHERE gpdid = {$gpdid}";
		$this->db->executar( $sql );
		
		$this->db->commit( );
		$this->db->sucesso( "principal/supervisao/criarRota", "" );
		
	}
	
	function obrProporRota( $rotid ){
		
		$sql = "SELECT * FROM obras.rotas WHERE rotid = {$rotid}";
		$dadosRota = $this->db->pegaLinha( $sql );

		$sql = "SELECT * FROM obras.trajetoria WHERE rotid = {$rotid} AND trjstatus = 'A'";
		$dadosTrajetosRota = $this->db->carregar( $sql );
		
		// cria a nova rota
		if( $dadosRota ){

			$dadosRota["rotnumero"] = $dadosRota["rotnumero"] + 1;
			
			$sql = "INSERT INTO obras.rotas( gpdid, 
											 strid, 
											 rotnumero, 
											 rotdtinclusao, 
											 rotstatus, 
											 rotkmdistanciatotal, 
											 usucpf,
											 prpid ) 
									 VALUES( {$dadosRota["gpdid"]},
									 		 {$dadosRota["strid"]},
									 		 {$dadosRota["rotnumero"]},
									 		 'now',
									 		 'A',
									 		 {$dadosRota["rotkmdistanciatotal"]},
									 		 '{$_SESSION["usucpf"]}',
									 		 2 ) 
								  RETURNING rotid";
									 		 
			$rotidNovo = $this->db->pegaUm( $sql );

			$_SESSION["obras"]["rotid"] = $rotidNovo;
			
			if( $rotidNovo && $dadosTrajetosRota ){
				
				for( $i = 0; $i < count($dadosTrajetosRota); $i++ ){
					
					if( !$dadosTrajetosRota[$i]["epcid"] ){
						
						$sql = "INSERT INTO obras.trajetoria( rotid, itgid, 
															  trjseq, trjkm, 
															  trjtempo, trjstatus, trjdtinclusao, tdeid )
													  VALUES( {$rotidNovo}, {$dadosTrajetosRota[$i]["itgid"]}, 
															  {$dadosTrajetosRota[$i]["trjseq"]}, {$dadosTrajetosRota[$i]["trjkm"]}, 
															  '{$dadosTrajetosRota[$i]["trjtempo"]}', 'A', 'now', {$dadosTrajetosRota[$i]["tdeid"]} )";
						
						
						$this->db->executar( $sql );
						
					}else{
						
						$sql = "INSERT INTO obras.trajetoria( rotid, epcid, 
															  trjseq, trjkm, 
															  trjtempo, trjstatus, trjdtinclusao, tdeid )
													  VALUES( {$rotidNovo}, {$dadosTrajetosRota[$i]["epcid"]}, 
															  {$dadosTrajetosRota[$i]["trjseq"]}, {$dadosTrajetosRota[$i]["trjkm"]}, 
															  '{$dadosTrajetosRota[$i]["trjtempo"]}', 'A', 'now', {$dadosTrajetosRota[$i]["tdeid"]} )";
						
						
						$this->db->executar( $sql );
						
					}
											  
				}
				
			}
			
		}
		
		$this->db->commit( );
		$this->db->sucesso( "principal/supervisao/criarRota", "" );
		
	}

	
}

class obrasRelatorioResumoTotal extends ControllerData{
	
	public $db;
	
	/**
	 * Funï¿½ï¿½o construtora das classes que cria os sets
	 * 
	 * @param array $dados
	 * @author Orion Teles de Mesquita
 	 * @since 18/08/2009
 	 * 
	 */
	function __construct(){
		parent::__construct();
	}
	
	function monta_cabecalho_relatorio_painel( $largura  = 95 ){
	
		$cabecalho = '<table width="'.$largura.'%" border="0" cellpadding="0" cellspacing="0" align="center" style="border-bottom: 1px solid;">'
					.'	<tr bgcolor="#ffffff">' 	
					.'		<td valign="top" width="50" rowspan="2"><img src="../imagens/brasao.gif" width="45" height="45" border="0"></td>'			
					.'		<td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">'				
					.'			'.$GLOBALS['parametros_sistema_tela']['sigla-nome_completo'].'<br/>'				
					.'			Resumo Total de Obras<br/>'					
//					.'			MEC / SE - Secretaria Executiva <br />'
					.'		</td>'
					.'		<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">'										
					.'			Data do Relatório:' . date( 'd/m/Y - H:i:s' ) . '<br />'					
					.'		</td>'					
					.'	</tr><tr bgcolor="#ffffff">'
					.'		<td colspan="2" align="center" valign="top" style="padding:0 0 5px 0;">'
					.'			<b><font style="font-size:14px;">' . $_REQUEST["titulo"] . '</font></b>'
					.'		</td>'
					.'	</tr>'					
					.'</table>';					
								
		echo $cabecalho;						
						
	}
	
	public function resumoObrasBrasil( $orgid ){
		
		if( $orgid != 0 ){
			$filtroOrgid = "AND obr.orgid = ".$orgid;
			$filtroOrgid2 = "AND orgid = ".$orgid;
		}
		
		$sql = "SELECT 
					sto.stodesc as descricao,
					count(DISTINCT obr.obrid) as obras,
					to_char(count(DISTINCT obr.obrid)::numeric/(SELECT count(DISTINCT obrid) FROM obras.obrainfraestrutura)::numeric*100, '9G999D9999') as perc,
					SUM(CASE WHEN (SELECT count(traid) FROM obras.termoaditivo ta WHERE ta.obrid = obr.obrid) > 0 
							THEN (SELECT travlrfinalobra FROM obras.termoaditivo ta WHERE ta.obrid = obr.obrid ORDER BY traid DESC LIMIT 1) 
							ELSE obr.obrvalorprevisto END) as vlr,
					CASE WHEN obr.stoid IN (1, 2) 
						THEN (SELECT count(DISTINCT obrid) FROM obras.obrainfraestrutura WHERE stoid IN (1, 2) AND stoid = obr.stoid AND DATE_PART('days', NOW() - (CASE WHEN obrdtvistoria IS NOT NULL THEN obrdtvistoria ELSE obsdtinclusao END) ) < 45 AND obsstatus = 'A' $filtroOrgid2)
						ELSE 0
					END as verde,
					CASE WHEN obr.stoid IN (1, 2) 
						THEN (SELECT count(DISTINCT obrid) FROM obras.obrainfraestrutura WHERE stoid IN (1, 2) AND stoid = obr.stoid AND DATE_PART('days', NOW() - (CASE WHEN obrdtvistoria IS NOT NULL THEN obrdtvistoria ELSE obsdtinclusao END) ) >= 45 AND DATE_PART('days', NOW() - obrdtvistoria ) <= 60 AND obsstatus = 'A' $filtroOrgid2) 
						ELSE 0
					END as amarelo,
					CASE WHEN obr.stoid IN (1, 2) 
						THEN (SELECT count(DISTINCT obrid) FROM obras.obrainfraestrutura WHERE stoid IN (1, 2) AND stoid = obr.stoid AND DATE_PART('days', NOW() - (CASE WHEN obrdtvistoria IS NOT NULL THEN obrdtvistoria ELSE obsdtinclusao END) ) > 60 AND obsstatus = 'A' $filtroOrgid2)
						ELSE 0
					END as vermelho
				FROM 
					obras.obrainfraestrutura obr
				INNER JOIN obras.situacaoobra sto ON sto.stoid = obr.stoid AND sto.stostatus = 'A'
				WHERE
					obr.obsstatus = 'A' $filtroOrgid
				GROUP BY
					sto.stodesc,obr.stoid";
		$arDados = $this->simec->carregar($sql);
		
		switch( $orgid ){
			case 1:
				$titulo = "GECopa";
			break;
			case 2:
				$titulo = "Penitenciárias";
			break;
			case 3:
				$titulo = "Gestão Aeroportuária";
			break;
			case 0:
				$titulo = "Total Brasil";
			break;
		}
		
		$tabela = " <table class=\"Listagem\" align=\"center\" width=\"95%\" bgcolor=\"#f5f5f5\" cellspacing=\"1\" cellpadding=\"3\">
				    <tr style=\"background-color: rgb(230,230,230);\">
					   	<td colspan=\"4\" align=\"center\" style=\"font-size:14px;\"><strong> $titulo </strong></td>
						<td colspan=\"3\" align=\"center\"><strong> Situação de Preenchimento </strong></td>
					</tr>
					<tr style=\"background-color: rgb(230,230,230);\">
						<td><strong>Sitação da Obra</strong></td>
						<td align=\"center\"><strong>Obras (ID)</strong></td>
						<td align=\"center\"><strong>%</strong></td>
						<td align=\"center\"><strong>Valor contratado<br>(R$ milhões)</br></strong></td>
						<td align=\"center\"><strong>Verde</strong></td>
						<td align=\"center\"><strong>Amarelo</strong></td>
						<td align=\"center\"><strong>Vermelho</strong></td>
					</tr>";
		if( is_array($arDados) ){
			$i=0;
			$cor = $i%2==0 ? 'rgb(240,240,240)' : 'rgb(250,250,250)' ;
			foreach( $arDados as $Dados ){
				$Soma['obras']    += $Dados['obras']; 
				$Soma['perc'] 	  += str_replace(",",".",$Dados['perc']); 
				$Soma['vlr'] 	  += $Dados['vlr']; 
				$Soma['verde']    += $Dados['verde']; 
				$Soma['amarelo']  += $Dados['amarelo']; 
				$Soma['vermelho'] += $Dados['vermelho']; 
				$verde    		  =  $Dados['verde'] != 0 ? $Dados['verde'] : ' - '; 
				$amarelo  		  =  $Dados['amarelo'] != 0 ? $Dados['amarelo'] : ' - '; 
				$vermelho 		  =  $Dados['vermelho'] != 0 ? $Dados['vermelho'] : ' - '; 
				$tabela .= "<tr style=\"background-color: {$cor};\">
									<td align=\"left\">{$Dados['descricao']}</td>
									<td align=\"center\">{$Dados['obras']}</td>
									<td align=\"center\">".number_format(str_replace(",",".",$Dados['perc']),2,",",".")."</td>
									<td align=\"center\">".number_format($Dados['vlr'],2,",",".")."</td>
									<td align=\"center\">".$verde."</td>
									<td align=\"center\">".$amarelo."</td>
									<td align=\"center\">".$vermelho."</td>
								</tr>";
			}
			$tabela .= "<tr style=\"background-color: rgb(230,230,230);\">
								<td align=\"right\"><strong>Total</strong></td>
								<td align=\"center\">{$Soma['obras']}</td>
								<td align=\"center\">".number_format($Soma['perc'],2,",",".")."</td>
								<td align=\"center\">".number_format($Soma['vlr'],2,",",".")."</td>
								<td align=\"center\"><FONT COLOR=\"#00AA00\">{$Soma['verde']}</FONT></td>
								<td align=\"center\"><FONT COLOR=\"#BB9900\">{$Soma['amarelo']}</FONT></td>
								<td align=\"center\"><FONT COLOR=\"#DD0000\">{$Soma['vermelho']}</FONT></td>
							</tr>
						</table>";
		}else{
			$tabela .= "<tr><td colspan=\"7\" color=\"red\">Sem dados encontrados</td></tr>";
		}
		echo $tabela;
	}
	
}

?>
