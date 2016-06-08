<?php
$obras  = new Obras();
$dobras = new DadosObra(null);

if($_SESSION["obra"]["obrid"]) {
	$dados = $obras->Dados($_SESSION["obra"]["obrid"]);
	$dobras = new DadosObra($dados);	
}

if ($_POST['ajax'] == "buscaCronogramaAditivo"){
	header('Content-Type: text/html; charset=iso-8859-1');
	montaTabela2(array("traid" => $_POST['traid']));
	exit;
}

if(isset($_REQUEST["listaetapas"])) {
	if($_POST["listaetapas"] != "") { 
		$lista = explode(";", $_POST["listaetapas"]);
	}
	
	// DELETE
//	$sql = pg_query("SELECT itcid FROM obras.itenscomposicaoobra WHERE obrid = ".$_SESSION["obra"]["obrid"]."");
//	while (($dados = pg_fetch_array($sql)) != false) {
	$traidWhere = "AND traid" . ($_POST['traid'] ? " = " . $_POST['traid'] : " IS NULL ");
	$sql = "SELECT itcid FROM obras.itenscomposicaoobra WHERE obrid = ".$_SESSION["obra"]["obrid"]." {$traidWhere} AND icostatus='A'";
	
	$dado = $db->carregar($sql);
	$dado = $dado ? $dado : array(); 
	
	$flagExcluirSupervisao = false;
	$msgSupervisao = "Os seguintes itens não foram excluídos pois ainda estão em supervisão:\\n";
	
	foreach($dado as $dados) {
		
		$itcid = $dados['itcid'];
		$cont = 0;
			
		for($i = 0; $i < count($lista); $i++) {
				
			if(($i % 10) == 0) {
				
				if($itcid == $lista[$i+1]) {
					$cont++;
				}
			}
		}
		
		$existe_ico = $db->pegaUm("SELECT 
									s.icoid 
								   FROM 
								   	obras.supervisaoitenscomposicao s 
								   JOIN obras.itenscomposicaoobra i ON i.icoid = s.icoid 
								   WHERE 
								   	obrid = ".$_SESSION["obra"]["obrid"]."
								   	{$traidWhere} 
								   	AND itcid = ".$itcid);
		
		if( $cont == 0 && $existe_ico )
		{
			$flagExcluirSupervisao = true;
			
			$itcdesc = $db->pegaUm('SELECT itcdesc FROM obras.itenscomposicao WHERE itcid = '.$itcid);
			$msgSupervisao .= "\\t\\t- ".$itcdesc."\\n";
		}
		if($cont == 0 && !$existe_ico) {
			$sql_delete = "DELETE FROM obras.itenscomposicaoobra WHERE obrid = ".$_SESSION["obra"]["obrid"]." {$traidWhere} AND itcid = ".$itcid;
			//$sql_delete = "UPDATE obras.itenscomposicaoobra SET icostatus='I' WHERE obrid = ".$_SESSION["obra"]["obrid"]." AND itcid = ".$itcid;
			$db->executar($sql_delete);
		}
	}
	
	if(count($lista) > 0) {
		
		// INSERT-UPDATE
		for($i=0; $i<count($lista); $i++) {
			if(($i % 10) == 0) {
				
				$sql = "SELECT * FROM obras.itenscomposicaoobra WHERE icostatus='A' AND obrid = ".$_SESSION["obra"]["obrid"]." {$traidWhere} AND itcid = ".$lista[$i+1];

				if( $lista[$i] != 'null' ){
					$array     = explode("/",$lista[$i]);
					$lista[$i] = "'".$array[2]."-".$array[1]."-".$array[0]."'";
				}
				if( $lista[$i+2] != 'null' ){
					$array2      = explode("/",$lista[$i+2]);
					$lista[$i+2] = "'".$array2[2]."-".$array2[1]."-".$array2[0]."'";
				}	
				
				if($db->pegaUm($sql) == false) {
					
					$sql = "INSERT INTO obras.itenscomposicaoobra ( obrid, 
																	itcid,
																	icopercsobreobra,
																	icovlritem,
																	icodtinicioitem,
																	icodterminoitem,
																	icostatus,
																	icodtinclusao,
																	icoordem,
																	traid 
																  ) VALUES ( " . $_SESSION["obra"]["obrid"] .",
														   			" . $lista[$i+1] . ",
														   			" . str_replace(',', '.', $lista[$i+8] ) . ",
														   			" . $obras->MoedaToBd( $lista[$i+4] ) . ",
														   			" . $lista[$i]  . ",
														   			" . $lista[$i+2]  . ",
														   			'A',
														   			now(),
														   			".$lista[$i+7].", 
														   			" . ($_POST['traid'] ? $_POST['traid'] : 'NULL') . "
														   		  );";
					
					$db->executar($sql);
					
				} else {
					$sql = "UPDATE 
								obras.itenscomposicaoobra 
							SET 
								icodtinicioitem  = " .  $lista[$i] . ",
								icodterminoitem  = " .  $lista[$i+2]  . ",
								icopercsobreobra = " . str_replace(',', '.', $lista[$i+8]) . ",
								icoordem         = '".$lista[$i+7]."',
								icovlritem       = " . $obras->MoedaToBd($lista[$i+4]) . " 
							WHERE 
								obrid = ".$_SESSION["obra"]["obrid"]." {$traidWhere} AND itcid = ".$lista[$i+1];
					$db->executar($sql);
					
				}
			}
		}
	}

	$db->commit();
	if($flagExcluirSupervisao)
	{
		echo "<script>
				alert('".$msgSupervisao."');
				window.location.href = 'obras.php?modulo=principal/etapas_da_obra&acao=A';
		      </script>";
		die;
	}
	else
	{
		$db->sucesso("principal/etapas_da_obra");
	}
}



// Inclusão de arquivos padrão do sistema
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

// Pega o caminho atual do usuário (em qual módulo se encontra)
$caminho_atual   = $_SERVER["REQUEST_URI"];
$posicao_caminho = strpos($caminho_atual, 'acao');
$caminho_atual   = substr($caminho_atual, 0 , $posicao_caminho);


// Se tiver a sessão, está em atualização de obra
if( $_SESSION["obra"]["obrid"] ){
	
	$dados = $db->pegaLinha("
			SELECT 
				obrcustocontrato,
				obrdttermino,
				obrdtinicio
			FROM
				obras.obrainfraestrutura
			WHERE
				obrid = {$_SESSION["obra"]["obrid"]}");
	
	$_SESSION["obrcustocontrato"] 	   = $dados['obrcustocontrato'];
	$_SESSION["obras"]["obrdtinicio"]  = $dados['obrdtinicio'] ? formata_data($dados['obrdtinicio']) : "";
	
	$traterminoexec = pegaObUltimoDadosAditivo("traterminoexec");
	if ($traterminoexec){
		$_SESSION["obras"]["obrdttermino"] = $traterminoexec;
	}else{
		$_SESSION["obras"]["obrdttermino"] = $dados['obrdttermino'] ? formata_data($dados['obrdttermino']) : "";
	}
}

// Verifica se existe vistória já cadastrada
$boPossuiVistoria = $obras->existenciaVistoriaParaObra( $_SESSION['obra']['obrid'] );
// Cria as abas do módulo
echo '<br>';
$db->cria_aba($abacod_tela,$url,$parametros);

// Cria o título da tela
$titulo_modulo = "Cronograma Físico-Financeiro";
monta_titulo( $titulo_modulo, 'Items de composição da obra' );

echo $obras->CabecalhoObras();

function montaTabela($dis, Array $paramWhere = null, Array $paramConfig = null) {
	global $db;
	
	$paramConfig['tabela'] = $paramConfig['tabela'] ? $paramConfig['tabela'] : 'tabela_etapas'; 
	
	if ( is_array($paramWhere) ){
//		$traid = $paramWhere['traid'];
//		$obAditivo = pegaObAditivo( $traid );
		$traid 	   = $paramWhere['traid'];
		$obAditivo = pegaObAditivo( $traid );
		if ( $traid ){
			$vlrFinalObra = pegaObMaiorVlrAditivo( array("traseq" => $obAditivo->traseq) );
		}
		$obAditivo->travlrfinalobra = $obAditivo->travlrfinalobra ? $obAditivo->travlrfinalobra : $vlrFinalObra;
		
		do{
			switch(true){
				case current( $paramWhere ) == 'null':
					$where[] = key( $paramWhere ) . " IS " . current( $paramWhere );
					break;
				default:
					$where[] = key( $paramWhere ) . " = " . current( $paramWhere );
					
					
			}
		}while( next($paramWhere) );
	}
	
	$sql = "SELECT 
				i.itcid,
				i.icovlritem,
				i.icopercsobreobra,
				i.icopercexecutado,
				ic.itcdesc,
				ic.itcdescservico,
				icodtinicioitem,
				icodterminoitem
			FROM 
				obras.itenscomposicaoobra i,
				obras.itenscomposicao ic 
			WHERE 
				i.obrid = ".$_SESSION["obra"]["obrid"]." 
				and i.itcid = ic.itcid 
				and i.icostatus='A' 
				AND i.icovigente = 'A'
				" . (is_array($where) ? " AND " . implode(" AND ", $where) : "") . "
			ORDER BY 
				i.icoordem";
	
	$count = 1;
	$soma = 0;
	$somav = 0;
	
	$controleLinha = 1;
	
	$dado = $db->carregar($sql);
	$dado = $dado ? $dado : array(); 
	
//	1 => Prazo (Aditivo)	
//	2 => Valor (Aditivo)	
//	3 => Prazo/Valor (Aditivo)	

//	$ttaid = pegaObUltimoAditivo('ttaid');
	$ttaid = $obAditivo->ttaid;
	
	switch( $ttaid ){
		case 1:
				$habData  = 'S';			
				if(!$db->testa_superuser() && !possuiPerfil(PERFIL_ADMINISTRADOR) ){ $habValor = 'disabled'; }
				$obAditivo->travlrfinalobra = pegaObMaiorVlrAditivo();	
			break;
		case 2:
				$habData  = 'S';			
				$habValor = '';			
			
			break;
		case 3:
				$habData  = 'S';			
				$habValor = '';			
			break;
		default:
				$habData  = 'S';			
				$habValor = '';			
			break;
	}
	
//	if (empty($dis) && pegaObUltimoAditivo('ttaid') == 1 ){
//		$disabledAditivo = 'disabled';
//		$habilitado		 = true;
//	}else{
//		$disabledAditivo = $dis;
//	}
	$disabledAditivo = $dis;
	
	echo "<table id=\"{$paramConfig['tabela']}\" width=\"95%\" align=\"center\" border=\"0\" cellspacing=\"2\" cellpadding=\"2\" class=\"listagem\">
			<thead>
				<tr id=\"cabecalho\">
					<td width=\"5%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Ordem</strong></td>
					<td width=\"5%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Ação</strong></td>
					<td width=\"30%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Descrição</strong></td>
					<td width=\"10%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Data de Início</strong></td>
					<td width=\"10%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Data de Término</strong></td>
					<td width=\"10%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Valor do Item (R$)</strong></td>
					<td width=\"10%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>(%) Referente a Obra <br/> (A)</strong></td>
					<td width=\"10%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>(%) Executado do Item Sobre a Obra <br/> (B)</strong></td>
					<td width=\"10%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>(%) do Item Executado <br/> (B x 100 / A)</strong></td>
				</tr>
		</thead>";	
	
	
	
	foreach($dado as $dados) {
		
		$itcid 			  = $dados['itcid'];
		$icovlritem 	  = $dados['icovlritem'];
		$itcdesc 		  = $dados['itcdesc'];
		$icopercsobreobra = $dados['icopercsobreobra'];
		$icopercexecutado = $dados['icopercexecutado'];
		$itcdescservico   = $dados['itcdescservico'];
		$icodtinicioitem  = $dados['icodtinicioitem'];
		$icodterminoitem  = $dados['icodterminoitem'];
				
		if(number_format($dados['icopercsobreobra'],2,',','.') == "0,00"){
			$$icopercsobreobra = "";
		}else{
			$$icopercsobreobra = number_format($dados['icopercsobreobra'],2,',','.');
		}	
		
		if($$icopercsobreobra == "" || $$icopercsobreobra == "0" || $$icopercsobreobra == "0.00" || $$icopercsobreobra == "0,00"){
			$porcento_executado = 0;
		}else{
			$porcento_executado = ($dados['icopercexecutado']*100)/$dados['icopercsobreobra'];	
		}
		
		$somav = bcadd($somav, $icovlritem, 2);
		
		$icovlritem = number_format($icovlritem,2,',','.'); 
		
		//$soma = round( $soma, 10 ) + round( $icopercsobreobra, 10 );
//		$soma = bcadd(round( $soma, 10 ), round( $icopercsobreobra, 10 ), 2);
		$soma = bcadd($soma, $icopercsobreobra, 10);
		
		$cor = "#f4f4f4";
		
		$count++;
		
		$nome = "linha_".$itcid;
		
		if ($count % 2){
			$cor = "#e0e0e0";
		}
		
		if ($itcdescservico!='')
			$title = "onmouseover=\"return escape('$itcdescservico');\"";
		else
			$title = "";
		
		
		$sql_excluir = "SELECT 
							count(*) as num 
						FROM 
							obras.itenscomposicaoobra itco
						INNER JOIN obras.supervisaoitenscomposicao sup ON itco.icoid = sup.icoid
						INNER JOIN obras.supervisao s on s.supvid = sup.supvid
						INNER JOIN obras.obrainfraestrutura oi on oi.obrid = s.obrid  
						WHERE
							oi.stoid not in (4, 5) AND 
							itco.obrid = ".$_SESSION["obra"]["obrid"]." 
							AND itco.itcid = ".$itcid."
							AND s.supstatus = 'A'
							AND itco.icostatus='A'
							AND itco.icopercexecutado is not null";
		
		$dados_e = $db->pegaUm($sql_excluir);
		
		$botaoExcluir = "<span><img src='/imagens/excluir_01.gif' style='cursor:pointer;' border='0' title='Excluir'></span>";
		if( $disabledAditivo == "" ) {
			if($dados_e == 0 || ($db->testa_superuser() || possuiPerfil(PERFIL_ADMINISTRADOR))) {
				if ( $ttaid != 1 && is_numeric($ttaid) )
					$habValor = '';			
						
				$botaoExcluir = "<span onclick='excluiItem(this.parentNode.parentNode.rowIndex);'><img src='/imagens/excluir.gif' style='cursor:pointer;' border='0' title='Excluir'></span>";
			} else {
				if ( $ttaid != 1 && is_numeric($ttaid) )
					$habValor = '';			
				$botaoExcluir = "<span onclick='alert(\"Existe supervisão cadastrada para esta etapa.\");'><img src='/imagens/excluir_01.gif' style='cursor:pointer;' border='0' title='Excluir'></span>";
			}
		}
		if($habilitado){
			$detalhesSetaCima = "<span><a ><img src='/imagens/seta_cimad.gif' id='sobe_dis' border='0' title='Subir'></a></span>";
			$detalhesSetaBaixo = "<span><a><img src='/imagens/seta_baixod.gif' id='desce_dis' border='0' title='Descer'></a></span>";			
		}else{
			if($controleLinha == 1) {
				 $detalhesSetaCima = "<span><a onclick=\"troca_linhas('{$paramConfig['tabela']}','cima',this.parentNode.parentNode.parentNode.rowIndex,8);\"><img src='/imagens/seta_cimad.gif' id='sobe_dis' border='0' title='Subir'></a></span>";
				 if(count($dado) == 1) {
				 	$detalhesSetaBaixo = "<span><a><img src='/imagens/seta_baixod.gif' id='desce_dis' border='0' title='Descer'></a></span>";
				 }else {
				 	$detalhesSetaBaixo = "<span><a onclick=\"troca_linhas('{$paramConfig['tabela']}','baixo',this.parentNode.parentNode.parentNode.rowIndex,8);\"><img src='/imagens/seta_baixo.gif' style='cursor:pointer;' border='0' title='Descer'></a></span>";
				 }
			}elseif(count($dado) == $controleLinha) {
				 $detalhesSetaCima = "<span><a onclick=\"troca_linhas('{$paramConfig['tabela']}','cima',this.parentNode.parentNode.parentNode.rowIndex,8);\"><img src='/imagens/seta_cima.gif' style='cursor:pointer;' border='0' title='Subir'></a></span>";
				 $detalhesSetaBaixo = "<span><a onclick=\"\"><img src='/imagens/seta_baixod.gif' id='desce_dis' border='0' title='Descer'></a></span>";
			}else {
				 $detalhesSetaCima = "<span><a onclick=\"troca_linhas('{$paramConfig['tabela']}','cima',this.parentNode.parentNode.parentNode.rowIndex,8);\"><img src='/imagens/seta_cima.gif' style='cursor:pointer;' border='0' title='Subir'></a></span>";
				 $detalhesSetaBaixo = "<span><a onclick=\"troca_linhas('{$paramConfig['tabela']}','baixo',this.parentNode.parentNode.parentNode.rowIndex,8);\"><img src='/imagens/seta_baixo.gif' style='cursor:pointer;' border='0' title='Descer'></a></span>";
			}
		}
		
		echo "			
			<tr id=\"$nome\" bgcolor='$cor' onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='$cor';\">
				<td align=\"center\">
					$detalhesSetaCima
					$detalhesSetaBaixo
				</td>
				<td align=\"center\">
					$botaoExcluir
				</td>
				<td $title>
					$itcdesc	
				</td>
				<td align='center'>".campo_data2( "dtinicial_$itcid", 'N', $habData, '', 'S', '', "obrValidaDataEtapa(this, 'obrdtinicio');", $icodtinicioitem, "obrValidaDataEtapa(this, 'obrdtinicio');" )."</td>
				<td align='center'>".campo_data2( "datafinal_$itcid", 'N', $habData, '', 'S', '', "obrValidaData(document.getElementById('dtinicial_$itcid'), document.getElementById('datafinal_$itcid')); obrValidaDataEtapa(this, '','obrdttermino');", $icodterminoitem, "obrValidaDataEtapa(this, '','obrdttermino');" )."</td>
				<td align=\"center\">
					<input class='CampoEstilo' type='text' id='valoritem_$itcid' size='15' maxlength='14' value='" . $icovlritem . "' onfocus='this.select();' onkeypress='reais(this,event); preencheRef(\"" . $itcid . "\",\"".$icovlritem."\");' onkeydown='backspace(this,event);'  $disabledAditivo $habValor >					
				</td>
				<td align=\"center\">					
					<input class='CampoEstilo' disabled=\"disabled\" type='text' id='mostraref_$itcid' size='16' maxlength='16' value='". number_format( $icopercsobreobra, 2)."' onkeypress='reais(this,event)' onkeydown='backspace(this,event);'  onblur='verificaVist(this,\"" . str_replace('.',',',$icopercsobreobra) . "\",\"" . $icopercexecutado . "\"); preencheVal(\"" . $itcid . "\",\"".str_replace('.',',',$icopercsobreobra)."\"); calculaTotal();'  $disabledAditivo> %
					<input type='hidden' id='referente_$itcid' value='" . str_replace('.',',',$icopercsobreobra) . "'/>					
				</td>
				<td align=\"right\">".number_format($icopercexecutado,2,',','.')."</td>
				<td align=\"right\">".number_format($porcento_executado,2,',','.')."</td>
			</tr>
		";
		$controleLinha++;
		
	}
	
	if ( ( ( (int) $soma) >= 100 ) && ( 101 > (int) $soma) ){
		$soma = 100.00;
	}
	
	if($count != 1) {
		$custoContrato = ($obAditivo->travlrfinalobra ? $obAditivo->travlrfinalobra : $_SESSION["obrcustocontrato"]);
		echo "			
			<tr id=\"tr_total\" bgcolor=\"#FFFFFF\">
				<td></td>
				<td></td>
				<td></td>
				<td></td>
				<td align=\"right\"><strong>Total</strong></td>
				<td align=\"center\">					
					<input class='CampoEstilo' type='text' id='totalv' size='15' maxlength='14' value='" . number_format($somav,2,',','.')."' disabled=\"disabled\">					
				</td>
				<td align=\"center\">					
					<input class='CampoEstilo' type='text' id='total' size='6' maxlength='6' value='" . number_format($soma,2,',','.') . "' disabled=\"disabled\"> %					
				</td>
				<td align='right'> - </td>
				<td align='right'> - </td>
			</tr>
			<tr id=\"tr_vlcontrato\" bgcolor=\"#FFFFFF\">
				<td></td>
				<td></td>
				<td></td>
				<td></td>
				<td align=\"right\"><strong>Valor do Contrato</strong></td>
				<td align=\"center\">					
					<input class='CampoEstilo' type='text' id='vl_contrato' size='15' maxlength='14' value='".number_format($custoContrato,2,',','.')."' disabled=\"disabled\">					
				</td>
				<td align=\"center\">					
					<input class='CampoEstilo' type='text' id='vl_porcento' size='6' maxlength='6' value='100,00' disabled=\"disabled\"> %					
				</td>
				<td align='right'> - </td>
				<td align='right'> - </td>
			</tr>
			<tr id=\"tr_vlrestante\" bgcolor=\"#FFFFFF\">
				<td></td>
				<td></td>
				<td></td>
				<td></td>
				<td align=\"right\"><strong>Valor Restante</strong></td>
				<td align=\"center\">					
					<input class='CampoEstilo' type='text' id='rest_totalv' size='15' maxlength='14' value='".number_format($custoContrato-$somav,2,',','.')."' disabled=\"disabled\">					
				</td>
				<td align=\"center\">					
					<input class='CampoEstilo' type='text' id='rest_total' size='6' maxlength='6' value='".number_format(100-$soma,2,',','.')."' disabled=\"disabled\"> %					
				</td>
				<td align='right'> - </td>
				<td align='right'> - </td>
			</tr>
		";
	}
	echo "</table>";
}
function montaTabela2(Array $paramWhere = null) {
	global $db;
	
	$habilitado = true;
	
	if ( is_array($paramWhere) ){
		$traid 	   = $paramWhere['traid'];
		$obAditivo = pegaObAditivo( $traid );
		if ( is_numeric($traid) ){
			$vlrFinalObra = pegaObMaiorVlrAditivo( array("traseq" => $obAditivo->traseq) );
		}
			
		$obAditivo->travlrfinalobra = $obAditivo->travlrfinalobra ? $obAditivo->travlrfinalobra : $vlrFinalObra;
		
		do{
			switch(true){
				case current( $paramWhere ) == 'null':
					$where[] = key( $paramWhere ) . " IS " . current( $paramWhere );
					break;
				default:
					$where[] = key( $paramWhere ) . " = " . current( $paramWhere );
					
					
			}
		}while( next($paramWhere) );
	}
	
	$sql = "SELECT 
				i.itcid,
				i.icovlritem,
				i.icopercsobreobra,
				i.icopercexecutado,
				ic.itcdesc,
				ic.itcdescservico,
				icodtinicioitem,
				icodterminoitem
			FROM 
				obras.itenscomposicaoobra i,
				obras.itenscomposicao ic 
			WHERE 
				i.obrid = ".$_SESSION["obra"]["obrid"]." 
				and i.itcid = ic.itcid 
				and i.icostatus='A' 
				" . (is_array($where) ? " AND " . implode(" AND ", $where) : "") . "
			ORDER BY 
				i.icoordem";
	
	$count = 1;
	$soma = 0;
	$somav = 0;
	
	$controleLinha = 1;
	
	$dado = $db->carregar($sql);
	$dado = $dado ? $dado : array(); 
	
	echo "<table id=\"tabela_etapas_original\" width=\"95%\" align=\"center\" border=\"0\" cellspacing=\"2\" cellpadding=\"2\" class=\"listagem\">
		  	<thead>
				<tr id=\"disabled_cabecalho\">
					<td width=\"30%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Descrição</strong></td>
					<td width=\"10%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Data de Início</strong></td>
					<td width=\"10%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Data de Término</strong></td>
					<td width=\"10%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>Valor do Item (R$)</strong></td>
					<td width=\"10%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>(%) Referente a Obra <br/> (A)</strong></td>
					<td width=\"10%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>(%) Executado do Item Sobre a Obra <br/> (B)</strong></td>
					<td width=\"10%\" valign=\"top\" align=\"center\" class=\"title\" style=\"border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;\" onmouseover=\"this.bgColor='#c0c0c0';\" onmouseout=\"this.bgColor='';\"><strong>(%) do Item Executado <br/> (B x 100 / A)</strong></td>
				</tr>
			</thead>";
	
	foreach($dado as $dados) {
		
		$itcid 			  = $dados['itcid'];
		$icovlritem 	  = $dados['icovlritem'];
		$itcdesc 		  = $dados['itcdesc'];
		$icopercsobreobra = $dados['icopercsobreobra'];
		$icopercexecutado = $dados['icopercexecutado'];
		$itcdescservico   = $dados['itcdescservico'];
		$icodtinicioitem  = $dados['icodtinicioitem'];
		$icodterminoitem  = $dados['icodterminoitem'];
				
		if(number_format($dados['icopercsobreobra'],2,',','.') == "0,00"){
			$$icopercsobreobra = "";
		}else{
			$$icopercsobreobra = number_format($dados['icopercsobreobra'],2,',','.');
		}	
		
		if($$icopercsobreobra == "" || $$icopercsobreobra == "0" || $$icopercsobreobra == "0.00" || $$icopercsobreobra == "0,00"){
			$porcento_executado = 0;
		}else{
			$porcento_executado = ($dados['icopercexecutado']*100)/$dados['icopercsobreobra'];	
		}
		
		$somav = bcadd($somav, $icovlritem, 2);
		
		$icovlritem = number_format($icovlritem,2,',','.'); 
		
		$soma = round( $soma, 10 ) + round( $icopercsobreobra, 10 );
		
		
		$cor = "#f4f4f4";
		
		$count++;
		
		$nome = "disabled_linha_".$itcid;
		
		if ($count % 2){
			$cor = "#e0e0e0";
		}
		
		if ($itcdescservico!='')
			$title = "onmouseover=\"return escape('$itcdescservico');\"";
		else
			$title = "";
		
		echo "			
			<tr id=\"$nome\" bgcolor='$cor' onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='$cor';\">
				<td $title>
					$itcdesc	
				</td>
				<td align='center'>".campo_data2( "disabled_dtinicial_$itcid", 'N', ($habilitado ? 'N' : 'S'), '', 'S', '', "", $icodtinicioitem )."</td>
				<td align='center'>".campo_data2( "disabled_datafinal_$itcid", 'N', ($habilitado ? 'N' : 'S'), '', 'S', '', "", $icodterminoitem )."</td>
				<td align=\"center\">
					<input class='disabled' disabled type='text' id='disable_valoritem_$itcid' size='15' maxlength='14' value='" . $icovlritem . "' $disabledAditivo >					
				</td>
				<td align=\"center\">					
					<input class='disabled' disabled type='text' id='disable_mostraref_$itcid' size='16' maxlength='16' value='". number_format( $icopercsobreobra, 2)."' onkeypress='reais(this,event)' onkeydown='backspace(this,event);'  onblur='verificaVist(this,\"" . str_replace('.',',',$icopercsobreobra) . "\",\"" . $icopercexecutado . "\"); preencheVal(\"" . $itcid . "\",\"".str_replace('.',',',$icopercsobreobra)."\"); calculaTotal();'  $disabledAditivo> %
				</td>
				<td align=\"right\">".number_format($icopercexecutado,2,',','.')."</td>
				<td align=\"right\">".number_format($porcento_executado,2,',','.')."</td>
			</tr>
		";
		$controleLinha++;
		
	}
	if ( ( ( (int) $soma) >= 100 ) && ( 101 > (int) $soma) ){
		$soma = 100.00;
	}
	
	if($count != 1) {
		$custoContrato = ($obAditivo->travlrfinalobra ? $obAditivo->travlrfinalobra : $_SESSION["obrcustocontrato"]);
		echo "			
			<tr id=\"tr_total\" bgcolor=\"#FFFFFF\">
				<td></td>
				<td></td>
				<td align=\"right\"><strong>Total</strong></td>
				<td align=\"center\">					
					<input class='CampoEstilo' type='text' id='disable_totalv' size='15' maxlength='14' value='" . number_format($somav,2,',','.')."' disabled=\"disabled\">					
				</td>
				<td align=\"center\">					
					<input class='CampoEstilo' type='text' id='disable_total' size='6' maxlength='6' value='" . number_format($soma,2,',','.') . "' disabled=\"disabled\"> %					
				</td>
				<td align='right'> - </td>
				<td align='right'> - </td>
			</tr>
			<tr id=\"tr_vlcontrato\" bgcolor=\"#FFFFFF\">
				<td></td>
				<td></td>
				<td align=\"right\"><strong>Valor do Contrato</strong></td>
				<td align=\"center\">					
					<input class='CampoEstilo' type='text' id='disable_vl_contrato' size='15' maxlength='14' value='".number_format($custoContrato,2,',','.')."' disabled=\"disabled\">					
				</td>
				<td align=\"center\">					
					<input class='CampoEstilo' type='text' id='disable_vl_porcento' size='6' maxlength='6' value='100,00' disabled=\"disabled\"> %					
				</td>
				<td align='right'> - </td>
				<td align='right'> - </td>
			</tr>
			<tr id=\"tr_vlrestante\" bgcolor=\"#FFFFFF\">
				<td></td>
				<td></td>
				<td align=\"right\"><strong>Valor Restante</strong></td>
				<td align=\"center\">					
					<input class='CampoEstilo' type='text' id='disable_rest_totalv' size='15' maxlength='14' value='".number_format($custoContrato-$somav,2,',','.')."' disabled=\"disabled\">					
				</td>
				<td align=\"center\">					
					<input class='CampoEstilo' type='text' id='disable_rest_total' size='6' maxlength='6' value='".number_format(100-$soma,2,',','.')."' disabled=\"disabled\"> %					
				</td>
				<td align='right'> - </td>
				<td align='right'> - </td>
			</tr>
		";
	}
	echo "</table>";
}

?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript">

// dando um KeyPress no primeiro Valor do Item para forçar a correção dos valores
$(document).ready(function() {
	$("[id^='valoritem_']").click(function () {
		$(this).trigger('keypress');
	})
});

function obrValidaDataEtapa( objeto, nomeId, nomeId2 ){
	
	if( nomeId ){
		var campoDt  = document.getElementById(nomeId);
		
		if( !validaDataMaior( campoDt, objeto ) ){
			alert( "A data de Início deve ser maior ou igual a Data de Início da Execução da Obra!" );
			objeto.value = '';
			objeto.focus();
			return false;
		}
	
	}else if( nomeId2 ){
		var campoDt2 = document.getElementById(nomeId2);
		
		if( !validaDataMaior( objeto, campoDt2 ) ){
			alert( "A data de Término deve ser menor ou igual a Data de Término da Execução da Obra!" );
			objeto.value = '';
			objeto.focus();
			return false;
		}
	
	}
	
	return true;

}

function troca_linhas(idTabela,acao,linha,ncolAfetada) {
	switch(acao) {
		case 'cima':
		linha1 = linha;
		linha2 = (linha-1);
		break;
		case 'baixo':
		linha1 = linha;
		linha2 = (linha+1);
		break;
	}
	if(linha2 != 0 && linha2 < (document.getElementById(idTabela).rows.length-1)) {
		for(i = 1; i<= ncolAfetada; i++) {
			cel1 = document.getElementById(idTabela).rows[linha1].cells[i].innerHTML;
			cel2 = document.getElementById(idTabela).rows[linha2].cells[i].innerHTML;
			document.getElementById(idTabela).rows[linha1].cells[i].innerHTML = cel2;
			document.getElementById(idTabela).rows[linha2].cells[i].innerHTML = cel1;
		}
	}
}

/* Função que permite somente a digitação de números. */

function somenteNumeros(e) {	
	if(window.event) {
    	/* Para o IE, 'e.keyCode' ou 'window.event.keyCode' podem ser usados. */
        key = e.keyCode;
    }
    else if(e.which) {
    	/* Netscape */
        key = e.which;
    }
    if(key!=8 || key < 48 || key > 57) return (((key > 47) && (key < 58)) || (key==8));
    {
    	return true;
    }
}



function preencheRef(id2, valorant){
	valorant = (valorant == undefined || valorant == '' ? '0,00' : valorant );
	var valorItemClick	 = $("[id='valoritem_" + id2 + "']"); //window.document.getElementById("valoritem_"+id2);
	var mrefClick		 = $("[id='mostraref_" + id2 + "']"); //window.document.getElementById("mostraref_"+id2);
	var refClick 		 = $("[id='referente_" + id2 + "']"); //window.document.getElementById("referente_"+id2);
	var custo		  	 = $("[id='obrcustocontrato']"); 	  //window.document.getElementById("obrcustocontrato");
	var somaValorItem 	 = new Number();
	var percentValorItem = new Number();
	var obCalc			 = new Calculo();

	$(":input[id^='valoritem_']").each(function (){
		
		var id	 = $(this).attr('id');	
		id 	   	 = id.substr(10, id.length);
		var mref = $("[id='mostraref_" + id + "']");
		var ref  = $("[id='referente_" + id + "']");
		
		if (parseFloat(custo.val()) > 0){
			somaValorItem = mascaraglobal('###.###.###.###,##', somaValorItem.toFixed(2));
			somaValorItem = parseFloat( obCalc.operacao(somaValorItem, $(this).val(), "+") );

//			if(parseFloat( somaValorItem ) > parseFloat( custo.val() ) && ( id2 == id && obCalc.comparar( $(this).val(), valorant, '>' ) ) ){
//				alert("A soma dos campos 'Valor do Item (R$ " + mascaraglobal('###.###.###.###,##', somaValorItem.toFixed(2)) + ")' não deve ultrapassar o 'Valor do Contrato (R$ " + mascaraglobal('###.###.###.###,##', parseFloat(custo.val()).toFixed(2)) + ")'.");
//				valorItemClick.val( valorant );
//				percentValorItem = (obCalc.converteMonetario( valorant ) * 100) / parseFloat(custo.val());
//				mrefClick.val( percentValorItem.toFixed(2) );

//				refClick.val( percentValorItem.toFixed(10) );
//			}
			
			percentValorItem = (obCalc.converteMonetario( $(this).val() ) * 100) / parseFloat(custo.val());
		}else{
			$(this).val('0,00');
			percentValorItem = 0;
		}
		mref.val( mascaraglobal('###.###.###.###,##', percentValorItem.toFixed(2)) );
		ref.val( percentValorItem.toFixed(10) );
	});
	
	if(parseFloat( somaValorItem ) > parseFloat( custo.val() ) && ( obCalc.comparar( valorItemClick.val(), valorant, '>' ) )){
		alert("A soma dos campos 'Valor do Item (R$ " + mascaraglobal('###.###.###.###,##', somaValorItem.toFixed(2)) + ")' não deve ultrapassar o 'Valor do Contrato (R$ " + mascaraglobal('###.###.###.###,##', parseFloat(custo.val()).toFixed(2)) + ")'.");
		valorItemClick.val( valorant );
		percentValorItem = (obCalc.converteMonetario( valorant ) * 100) / parseFloat(custo.val());
		mrefClick.val( percentValorItem.toFixed(2) );
		refClick.val( percentValorItem.toFixed(10) );
	}
	calculaTotal( false );
	return true;
}	



//function preencheRef(id2, valorant){
//	
//	var valor = window.document.getElementById("valoritem_"+id2);
//	var mref  = window.document.getElementById("mostraref_"+id2);
//	var ref   = window.document.getElementById("referente_"+id2);
//	var custo = window.document.getElementById("obrcustocontrato");
//	var valoritem = 0;
//	var campo = "";
//	var somav = 0;
//	
//	if(custo.value == null || custo.value == "") custo.value = 0.00;
//	if (valorant == undefined)	valorant = "0,00";
//	
//	//verifica valor antigo
//	form = document.getElementById("formulario");
//	for(i=0; i<form.length; i++) {
//
//		if(form.elements[i].id == "salvar" || form.elements[i].id == "total" || form.elements[i].id == "totalv" || form.elements[i].id == "rest_totalv" || form.elements[i].id == "rest_total" || form.elements[i].id == "vl_contrato"  || form.elements[i].id == "vl_porcento") {
//			continue;
//		}
//		
//		if(form.elements[i].type == "text") {
//			campo = form.elements[i].id.substr(0,10);
//			if(campo == "valoritem_"){
//				valoritem = form.elements[i].value;
//				valoritem = valoritem.replace(".", "");
//				valoritem = valoritem.replace(".", "");
//				valoritem = valoritem.replace(".", "");
//				valoritem = valoritem.replace(".", "");
//				valoritem = valoritem.replace(",", ".");
//     			somav += parseFloat(valoritem);
//			}
//		}
//	}
//	
//	if(parseFloat(somav.toFixed(2)) > parseFloat(custo.value)){
//		alert("A soma dos campos 'Valor do Item (R$)' não deve ultrapassar o valor do contrato."+somav.toFixed(2)+" "+custo.value);
//		valor.value = valorant;
//		calculaTotal();
//	}
//	//fim
//
//
////	var valor_v = valor.value;
////	valor_v = mascaraglobal('###.###.###.###,##', valor_v);
////		
////	valor_v = valor_v.replace(".",""); 
////	valor_v = valor_v.replace(".",""); 
////	valor_v = valor_v.replace(".","");
////	valor_v = valor_v.replace(".","");
////	valor_v = valor_v.replace(",",".");
//	
//	if (parseFloat(custo.value)>0){
//		//ref.disabled = true;
//	
//		for(x=0; x<form.length; x++) {
//	
//			campo = form.elements[x].id.substr(0,10);
//			if(campo == "valoritem_"){
//				if ( x % 5 == 4 ){
//					
//					var id = form.elements[x].id.substr(10,12);
//					
//					var valor_calc = form.elements[x].value;
//					valor_calc = mascaraglobal('###.###.###.###,##', valor_calc);
//					valor_calc = valor_calc.replace(".",""); 
//					valor_calc = valor_calc.replace(".",""); 
//					valor_calc = valor_calc.replace(".","");
//					valor_calc = valor_calc.replace(".","");
//					valor_calc = valor_calc.replace(",",".");
//					valor_calc = (valor_calc * 100) / parseFloat(custo.value);
//
//					document.getElementById('mostraref_'+id).value = valor_calc.toFixed(2);
//					document.getElementById('referente_'+id).value = valor_calc.toFixed(10);
//					form.elements[x].value = mascaraglobal('###.###.###.###,##', form.elements[x].value);
//				}
//			}
//		}
////		var valor_calc = ((valor_v * 100) / parseFloat(custo.value));
////				
////		mref.value = valor_calc.toFixed(2);
////		ref.value = valor_calc.toFixed(10);
//
////		valor.value = mascaraglobal('###.###.###.###,##', valor.value);
////		mref.value = mascaraglobal('###,##', mref.value);
////		ref.value = mascaraglobal('###,##########', ref.value);
//		
//	}else{
//		for(x=0; x<form.length; x++) {
//	
//			campo = form.elements[x].id.substr(0,10);
//			if(campo == "valoritem_"){
//				if ( x % 5 == 4 ){
//					
//					var id = form.elements[x].id.substr(10,12);
//					
//					document.getElementById('mostraref_'+id).value = "0,00";
//					document.getElementById('referente_'+id).value = "0,00";
//				}
//			}
//		}
////		mref.value = "0,00";
////		ref.value  = "0,00";
//	}
//	
//	if(ref.value == "") ref.value = "0,00";
//	if(mref.value == "") mref.value = "0,00";
//	if(valor.value == "") valor.value = "0,00";
//	mref.disabled = 'disabled';
//}

function preencheVal(id2, valorant){
	return;
	var valor = window.document.getElementById("valoritem_"+id2);
	var mref  = window.document.getElementById("mostraref_"+id2);
	var ref   = window.document.getElementById("referente_"+id2);
	var custo = window.document.getElementById("obrcustocontrato");
	var refitem = 0;
	var campo = "";
	var somav = 0;
	
//	ref.value = mref.value;	
	
	if(custo.value == null || custo.value == "") custo.value = 0.00;
	if (valorant == undefined)	valorant = "0,00";
	
	
	//verifica valor antigo
	form = document.getElementById("formulario");
	for(i=0; i<form.length; i++) {
		if(form.elements[i].id == "total" || form.elements[i].id == "totalv" || form.elements[i].id == "rest_totalv" || form.elements[i].id == "rest_total" || form.elements[i].id == "vl_contrato"  || form.elements[i].id == "vl_porcento") {
			continue;
		}
		if(form.elements[i].type == "text") {
			campo = form.elements[i].id.substr(0,10);
			if(campo == "referente_"){
				refitem = form.elements[i].value;
				refitem = refitem.replace(",", ".");
     			somav += parseFloat(refitem);
     			alert(somav);
			}
		}
	}
	/*
	if(parseFloat(somav) > 100){
		alert("A soma dos campos '(%) Referente a Obra' não pode ultrapassar 100 %");
		ref.value = valorant;
	}*/
	//fim
	
		
	var valor_v = valor.value;
	valor_v = mascaraglobal('###.###.###.###,##', valor_v);
		
	valor_v = valor_v.replace(".","");
	valor_v = valor_v.replace(".",""); 
	valor_v = valor_v.replace(".","");  
	valor_v = valor_v.replace(",",".");
	
	//if (valor_v != "" && parseFloat(valor_v) != 0.00){
		var valor_calc2 = ((ref.value.replace(",",".")/100) * parseFloat(custo.value));
		valor.value = valor_calc2.toFixed(2);
		
		valor.value = mascaraglobal('###.###.###.###,##', valor.value);
		mref.value = mascaraglobal('###,##', mref.value);
		ref.value = mascaraglobal('###,##########', ref.value);
	//}
	
	if(mref.value == "") mref.value = "0,00";
	if(ref.value == "") ref.value = "0,00";
	if(valor.value == "") valor.value = "0,00";
	
		
}

function verificaVist(obj,valor,valorexec)
{
	var custo = window.document.getElementById("obrcustocontrato");
	var boExisteVistoria = "<?php echo (bool) $boPossuiVistoria; ?>";
	var perilsSuper      = <? echo ((possuiPerfil( array(PERFIL_ADMINISTRADOR,PERFIL_SUPERVISORMEC) ))?"true":"false"); ?>;

	if( boExisteVistoria && valorexec != "" && obj.value != valor && !perilsSuper ){
		alert("Este item não pode ser Alterado, pois já possui dados de Vistoria.");
		obj.value = valor;
		return true;
	}
	return false;
}


function calculaTotal( msg ) {
	msg = (msg == true ? msg : false);
	var i, soma = 0, somav = 0, valoritem = 0;
		
	form = document.getElementById("formulario");
			
	for(i=0; i<form.length; i++) {
		
		if(form.elements[i].id == "total" || form.elements[i].id == "totalv" || form.elements[i].id == "rest_totalv" || form.elements[i].id == "rest_total" || form.elements[i].id == "vl_contrato"  || form.elements[i].id == "vl_porcento") {
			continue;
		}
		
		if(form.elements[i].type == "text" || form.elements[i].type == "hidden" ) {

			var campo = form.elements[i].id.substr(0,10);
			if(campo == "referente_"){
				soma += Number(form.elements[i].value.replace(",", "."));
			}

			if(campo == "valoritem_"){
				valoritem = form.elements[i].value;
				valoritem = valoritem.replace(".", "");
				valoritem = valoritem.replace(".", "");
				valoritem = valoritem.replace(".", "");
				valoritem = valoritem.replace(",", ".");
     			somav += parseFloat(valoritem);
			}
			
		}
	}	
	
	var campo_total = document.getElementById("total");
	var campo_totalv = document.getElementById("totalv");
	
	var rest_total = document.getElementById("rest_total");
	var rest_totalv = document.getElementById("rest_totalv");
	/*
	if(soma > 100.00){
		alert("A soma dos campos '(%) Referente a Obra' não pode ultrapassar 100 %");
		campo_total.value = soma.toFixed(2).toString().replace(".", ",");
	}else{	
		campo_total.value = soma.toFixed(2).toString().replace(".", ",");
	}
	*/
	campo_total.value = soma.toFixed(2).toString().replace(".", ",");

	var custox = window.document.getElementById("obrcustocontrato");
	somav2 = somav;
	if(parseFloat(somav.toFixed(2)) > parseFloat(custox.value)){
		if ( msg ){
			alert("A soma dos campos 'Valor do Item (R$)' não deve ultrapassar o valor do contrato.");
		}
		somav = somav.toFixed(2).toString().replace(".", ",");
		campo_totalv.value = mascaraglobal('###.###.###.###,##', somav);
//		campo_totalv.value = mascaraglobal('###.###.###.###,##', custox.value);
		return false;
	}else{
		somav = somav.toFixed(2).toString().replace(".", ",");	
		campo_totalv.value = mascaraglobal('###.###.###.###,##', somav);
	}
	
	
	var tot_rest_total = (100-soma);
	tot_rest_total = tot_rest_total.toFixed(2).toString().replace(".", ",");
	
	rest_total.value = tot_rest_total.replace("-","");
	
//	alert(custox.value+' - '+somav2);
	var tot_rest_totalv = (custox.value-somav2);
	if(tot_rest_totalv>=-0.01){
		tot_rest_totalv = tot_rest_totalv.toFixed(2).toString().replace(".", ",");
		rest_totalv.value = mascaraglobal('###.###.###.###,##', tot_rest_totalv);
	}
	else{
		tot_rest_totalv = tot_rest_totalv.toFixed(2).toString().replace(".", ",");
		rest_totalv.value = "-" + mascaraglobal('###.###.###.###,##', tot_rest_totalv);
	}

}

/******************************************/
// INÍCIO DA LÓGICA PARA MÁSCARA DE REAIS //
/******************************************/
	documentall = document.all;
/*
* função para formatação de valores monetários retirada de
* http://jonasgalvez.com/br/blog/2003-08/egocentrismo
*/

function formatamoney(c) {
    var t = this; if(c == undefined) c = 2;		
    var p, d = (t=t.split("."))[1].substr(0, c);
    for(p = (t=t[0]).length; (p-=3) >= 1;) {
	        t = t.substr(0,p) + "." + t.substr(p);
    }
    return t+","+d+Array(c+1-d.length).join(0);
}

String.prototype.formatCurrency=formatamoney

function demaskvalue(valor, currency){
/*
* Se currency é false, retorna o valor sem apenas com os números. Se é true, os dois últimos caracteres são considerados as 
* casas decimais
*/
var val2 = '';
var strCheck = '0123456789';
var len = valor.length;
	if (len== 0){
		return 0.00;
	}

	if (currency ==true){	
		/* Elimina os zeros à esquerda 
		* a variável  <i> passa a ser a localização do primeiro caractere após os zeros e 
		* val2 contém os caracteres (descontando os zeros à esquerda)
		*/
		
		for(var i = 0; i < len; i++)
			if ((valor.charAt(i) != '0') && (valor.charAt(i) != ',')) break;
		
		for(; i < len; i++){
			if (strCheck.indexOf(valor.charAt(i))!=-1) val2+= valor.charAt(i);
		}

		if(val2.length==0) return "0.00";
		if (val2.length==1)return "0.0" + val2;
		if (val2.length==2)return "0." + val2;
		
		var parte1 = val2.substring(0,val2.length-2);
		var parte2 = val2.substring(val2.length-2);
		var returnvalue = parte1 + "." + parte2;
		return returnvalue;
		
	}
	else{
			/* currency é false: retornamos os valores COM os zeros à esquerda, 
			* sem considerar os últimos 2 algarismos como casas decimais 
			*/
			val3 ="";
			for(var k=0; k < len; k++){
				if (strCheck.indexOf(valor.charAt(k))!=-1) val3+= valor.charAt(k);
			}			
	return val3;
	}
}

function reais(obj,teclapres){

//var whichCode = (window.Event) ? event.which : event.keyCode;

        if(window.event) { // Internet Explorer
         var whichCode = teclapres.keyCode; }
        else if(teclapres.which) { // Nestcape / firefox
         var whichCode = teclapres.which;
        }
      

/*
Executa a formatação após o backspace nos navegadores !document.all
*/
if (whichCode == 8 && !documentall) {	
/*
Previne a ação padrão nos navegadores
*/
	if (teclapres.preventDefault){ //standart browsers
			teclapres.preventDefault();
		}else{ // internet explorer
			teclapres.returnValue = false;
	}
	var valor = obj.value;
	var x = valor.substring(0,valor.length-1);
	obj.value= demaskvalue(x,true).formatCurrency();
	return false;
}
/*
Executa o Formata Reais e faz o format currency novamente após o backspace
*/
FormataReais(obj,'.',',',teclapres);
} // end reais


function backspace(obj,teclapres){
/*
Essa função basicamente altera o  backspace nos input com máscara reais para os navegadores IE e opera.
O IE não detecta o keycode 8 no evento keypress, por isso, tratamos no keydown.
Como o opera suporta o infame document.all, tratamos dele na mesma parte do código.
*/

//var whichCode = (window.Event) ? event.which : event.keyCode;
        if(window.event) { // Internet Explorer
         var whichCode = teclapres.keyCode; }
        else if(teclapres.which) { // Nestcape / firefox
         var whichCode = teclapres.which;
        }
    

if (whichCode == 8 && documentall) {	
	var valor = obj.value;
	var x = valor.substring(0,valor.length-1);
	var y = demaskvalue(x,true).formatCurrency();

	obj.value =""; //necessário para o opera
	obj.value += y;
	
	if (teclapres.preventDefault){ //standart browsers
			teclapres.preventDefault();
		}else{ // internet explorer
			teclapres.returnValue = false;
	}
	return false;

	}// end if		
}// end backspace

function FormataReais(fld, milSep, decSep, teclapres) {
var sep = 0;
var key = '';
var i = j = 0;
var len = len2 = 0;
var strCheck = '0123456789';
var aux = aux2 = '';
//var whichCode = (window.Event) ? e.which : e.keyCode;
        if(window.event) { // Internet Explorer
         var whichCode = teclapres.keyCode; }
        else if(teclapres.which) { // Nestcape / firefox
         var whichCode = teclapres.which;
        }    

//if (whichCode == 8 ) return true; //backspace - estamos tratando disso em outra função no keydown
if (whichCode == 0 ) return true;
if (whichCode == 9 ) return true; //tecla tab
if (whichCode == 13) return true; //tecla enter
if (whichCode == 16) return true; //shift internet explorer
if (whichCode == 17) return true; //control no internet explorer
if (whichCode == 27 ) return true; //tecla esc
if (whichCode == 34 ) return true; //tecla end
if (whichCode == 35 ) return true;//tecla end
if (whichCode == 36 ) return true; //tecla home

/*
O trecho abaixo previne a ação padrão nos navegadores. Não estamos inserindo o caractere normalmente, mas via script
*/

if (teclapres.preventDefault){ //standart browsers
		teclapres.preventDefault()
	}else{ // internet explorer
		teclapres.returnValue = false
}

var key = String.fromCharCode(whichCode);  // Valor para o código da Chave
if (strCheck.indexOf(key) == -1) return false;  // Chave inválida

/*
Concatenamos ao value o keycode de key, se esse for um número
*/
fld.value += key;

var len = fld.value.length;
var bodeaux = demaskvalue(fld.value,true).formatCurrency();
fld.value=bodeaux;

/*
Essa parte da função tão somente move o cursor para o final no opera. Atualmente não existe como movê-lo no konqueror.
*/
  if (fld.createTextRange) {
    var range = fld.createTextRange();
    range.collapse(false);
    range.select();
  }
  else if (fld.setSelectionRange) {
    fld.focus();
    var length = fld.value.length;
    fld.setSelectionRange(length, length);
  }
  return false;

}
/****************************************/
// FIM DA LÓGICA PARA MÁSCARA DE REAIS  //
/****************************************/
	
	function excluiItem(linha) {
		var tabela = document.getElementById("tabela_etapas");
		tabela.deleteRow(linha);
		if(tabela.rows.length == 4) {
			tabela.deleteRow(3);
			tabela.deleteRow(2);
			tabela.deleteRow(1);
		}
		

		if(tabela.rows.length == 4) {
			tabela.rows[1].cells[0].innerHTML = "<span><a onclick=\"troca_linhas('tabela_etapas','cima',this.parentNode.parentNode.parentNode.rowIndex,4);\"><img src='/imagens/seta_cimad.gif' id='sobe_dis' border='0' title='Subir'></a></span> <span><a onclick=\"\"><img src='/imagens/seta_baixod.gif' id='desce_dis' border='0' title='Descer'></a></span>";
		} else {
			if(linha == 1) {
				tabela.rows[1].cells[0].innerHTML = "<span><a onclick=\"troca_linhas('tabela_etapas','cima',this.parentNode.parentNode.parentNode.rowIndex,4);\"><img src='/imagens/seta_cimad.gif' id='sobe_dis' border='0' title='Subir'></a></span> <span><a onclick=\"troca_linhas('tabela_etapas','baixo',this.parentNode.parentNode.parentNode.rowIndex,4);\"><img src='/imagens/seta_baixo.gif' border='0' title='Descer'></a></span>";		
			} 
			else {
				if(linha == (tabela.rows.length-1)) {
					tabela.rows[(tabela.rows.length-2)].cells[0].innerHTML = "<span><a onclick=\"troca_linhas('tabela_etapas','cima',this.parentNode.parentNode.parentNode.rowIndex,4);\"><img src='/imagens/seta_cima.gif' id='sobe_dis' border='0' title='Subir'></a></span> <span><a onclick=\"\"><img src='/imagens/seta_baixod.gif' id='desce_dis' border='0' title='Descer'></a></span>";
				}
			}
		
		}
		
		return calculaTotal();

	}
		
	function submeterListaEtapas( superuser ) {
		var retorno = ""; 
		var count = 0, soma = 0, somav = 0, valoritem =0;
		var commit = true;
		
		<?php if( !$db->testa_superuser() && !possuiPerfil(PERFIL_ADMINISTRADOR) ): ?>
		// validando as datas
		$('input[id*=dtinicial_]').each(function () {
			var datafinal = this.name.replace("dtinicial","datafinal");
		
			if( !validaData( this ) || !validaData( document.getElementById(datafinal)) ) {
	            alert('Data fim está no formato incorreto.');
	            commit = false;
	            return false;
		    }else if( !validaDataMaior( this, document.getElementById(datafinal) ) ){
//					alert( ( this, document.getElementById(datafinal).value) );
					var linha 	= this.parentNode.parentNode;
					var servico = jQuery.trim(linha.cells[2].innerHTML);
	            	alert("A data inicial não pode ser maior que data final, no serviço '"+servico+"'");
	            	this.focus();
		            commit = false;
		            return false;
		    }
		    
		});// fim da validação das datas
		<?php endif; ?>
		
		form = document.getElementById("formulario");	
		
		for (var i=0; i<form.length; i++) {
			
			if(form.elements[i].id.substr(0,10) == "valoritem_" || form.elements[i].id.substr(0,10) == "mostraref_" || form.elements[i].id.substr(0,10) == "referente_" || form.elements[i].id.substr(0,10) == "dtinicial_" || form.elements[i].id.substr(0,10) == "datafinal_") {			
				
				if(form.elements[i].id == "total" || form.elements[i].id == "totalv" || form.elements[i].id == "rest_totalv" || form.elements[i].id == "rest_total" || form.elements[i].id == "vl_contrato"  || form.elements[i].id == "vl_porcento"){
	     			continue;
				}
			
				id_objeto = form.elements[i].id;
	     		id_objeto = id_objeto.substr(10);

//	     		alert(id_objeto);

	     		if( (form.elements[i].value != "") || (superuser != 1) ) {
	     			if(form.elements[i].id.substr(0,10) != "mostraref_"){
//		     			if( form.elements[i].value == "" ){
//		     				form.elements[i].value = 'null';
//			     		}
	     				if(count == 0) {
		     				retorno = retorno + form.elements[i].value + ";" + id_objeto;
		     				count++;
		     			} else {
		     				retorno = retorno + ";" + form.elements[i].value + ";" + id_objeto;
		     			}
	     			}else{
//		     			alert(document.getElementById("referente_"+id_objeto).value);
//	     				retorno = retorno + (";" + form.elements[i].value + ";" + document.getElementById("referente_"+id_objeto).parentNode.parentNode.rowIndex);
	     				retorno = retorno + ";" + form.elements[i].value + ";" + document.getElementById("referente_"+id_objeto).parentNode.parentNode.rowIndex;
	     			}
     			} else {
				  	alert("Todos os campos devem ser preenchidos");
     				form.elements[i].style.backgroundColor = "#FFFACD"; 
     				form.elements[i].focus();
     				commit = false;
     				return false;
     			}
	     		
	     		var campo = form.elements[i].id.substr(0,10);
				if(campo == "referente_"){
	     			soma += Number(form.elements[i].value.replace(",", "."));
	     		}
	     		
				
				if(campo == "valoritem_"){
					valoritem = form.elements[i].value;
					valoritem = valoritem.replace(".", "");
					valoritem = valoritem.replace(".", "");
					valoritem = valoritem.replace(".", "");
					valoritem = valoritem.replace(",", ".");
	     			somav += parseFloat(valoritem);
	     		}
				
	     	}
//	     	retorno += '\n';
     	}
		var custoc = window.document.getElementById("obrcustocontrato");
		if(custoc.value == null || custoc.value == "") custoc.value = 0.00;
		
		var valores = new Number(somav.toFixed(2));

		<?php if( !$db->testa_superuser() && !possuiPerfil(PERFIL_ADMINISTRADOR) ): ?>
     	if(valores > custoc.value) {
     		alert("A soma dos campos 'Valor do Item (R$)' não deve ultrapassar o valor do contrato.");
     		return false;
     	}
     	<?php endif; ?>
		/*
     	if(soma > 100) {
     		alert("A soma dos campos '(%) Referente a Obra' não deve ultrapassar 100%.");
     		return false;
     	}*/
//		alert(retorno);
//		return false;
     	document.getElementById("listaetapas").value = retorno;    	     	
//		return true;
		if(commit){
			document.getElementById('formulario').submit();
		}
	}
	
function visualizarCronograma(obj, traid){
	if ( $(obj).attr('src').indexOf('mais') > -1 ){
		divCarregando();
		var dado = {"ajax" : "buscaCronogramaAditivo", "traid" : traid};
		$('#subLinha_' + traid).load("?modulo=principal/etapas_da_obra&acao=A", dado, function () { 
				$(obj).attr('src', '../imagens/menos.gif');
				$('#subLinha_' + traid).parent().show();	
				divCarregado();
			});
	}else{
		$('#subLinha_' + traid).parent().hide();	
		$(obj).attr('src', '../imagens/mais.gif');
	}	
}

function abreCronogramaOriginal(obj){
	if ( $(obj).attr('src').indexOf('mais') > -1 ){
		$('#cronogramaOriginal').show();	
		$(obj).attr('src', '../imagens/menos.gif');
	}else{
		$('#cronogramaOriginal').hide();	
		$(obj).attr('src', '../imagens/mais.gif');
	}	
}
</script>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="post" name="formulario" id="formulario" action="<?php echo $caminho_atual;?>acao=A">
<input type="hidden" id="obrdtinicio" value="<?php print $_SESSION["obras"]["obrdtinicio"]; ?>"/>
<input type="hidden" id="obrdttermino" value="<?php print $_SESSION["obras"]["obrdttermino"]; ?>"/>
<?
// DADOS DO ADITIVO
$obAditivo = pegaObUltimoAditivo();

$traid 	   = $obAditivo->traid;
$ttaid 	   = $obAditivo->ttaid;

$sql = "SELECT
			count(*) AS total
		FROM
			obras.termoaditivo
		WHERE
			trastatus = 'A'
			AND obrid = {$_SESSION['obra']['obrid']}";

$totAditivo = $db->pegaUm( $sql );
// DADOS DO ADITIVO - FIM
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>	
		<td>
			<!--<br/>-->
			<!--<div align="center"><font color=RED><B>Havendo a necessidade de inserção de uma nova etapa que não conste no sistema, <BR>favor entrar em contato com os administradores do sistema.</B></font></div>-->
			<!--<br/>-->
			<?
			if ($totAditivo == 0){
			?>	
				<center>
					<div style="width:95%; background:#C0C0C0;">
						<b>Cronograma Original</b>
					</div>
				</center>
			<?	
				montaTabela($disabled, array("traid" => "null")); 
			}
			?>
			</br>

			<? 
			if ($totAditivo > 0){
				// CABEÇALHO da lista
				$arCabecalho = array(
										"Ação",
										"Nº",
										"Aditivo",
										"Tipo",
										"Cadastrante"
								    );
								    
				// AÇÃO que será posta na primeira coluna de todas as linhas
				$acao = "<center>
						   <img onclick='visualizarCronograma(this, {traid})' src='../imagens/mais.gif' title='Visualizar cronograma do aditivo' style='cursor:pointer; margin-left:3px'>" . 
						"<center>";
				
				// ARRAY de parametros de configuração da tabela
				$arConfig = array("style" 	 => "width:95%; align:center;",
								  "subLinha" => "{traid}");
				
				$sql = "	SELECT
								'-' AS traseq,
								'<b>Cronograma Original</b>' AS tradsc,
								'-' AS ttadsc,
								'-' AS usunome,
								'null' AS traid
						UNION ALL
							SELECT
								traseq::text,
								tradsc::text,
								ttadsc::text,
								usunome::text,
								traid::text
							FROM
								obras.termoaditivo t
							JOIN seguranca.usuario u USING(usucpf)
							JOIN obras.tipotermoaditivo tt USING(ttaid)
							WHERE
								trastatus = 'A'
								AND obrid = {$_SESSION['obra']['obrid']}
								AND traseq NOT IN (SELECT 
													MAX(traseq) 
												  FROM 
												  	obras.termoaditivo 
												  WHERE 
												  	trastatus = 'A' 
												  	AND obrid = {$_SESSION['obra']['obrid']})
							ORDER BY
								traseq;";
				$arDados = $db->carregar( $sql );
				
				$a = new Lista($arConfig);
				$a->setCabecalho( $arCabecalho );
				$a->setCorpo( $arDados, $arParamCol );
				$a->setAcao( $acao );
				?>
				<center>
				<fieldset style="width:95%;">
					<legend>Histórico de cronograma por aditivo</legend>
			<? 
				$a->show();	
			?>	
				</fieldset>
				</center>
			<?
			}
			?>
			</br>
			<? 
			if ($totAditivo > 0){
			?>
				<center>
					<div style="width:95%; background:#C0C0C0;">
						<b>Cronograma Atual</b>
					</div>
				</center>
				<?
				montaTabela($disabled, array("traid" => $traid));
			}	 
				?>
		</td>
	</tr>
	<? 
	$vlrMaiorAditivo = pegaObMaiorVlrAditivo();
	?>
	<input type="hidden" id="obrcustocontrato" value="<?php echo ($vlrMaiorAditivo ? $vlrMaiorAditivo : $_SESSION["obrcustocontrato"]); ?>"/>
	<input type="hidden" id="listaetapas" name="listaetapas">
	<input type="hidden" id="traid" name="traid" value="<?=$traid; ?>">
	<tr>
		<td>
		<?php //if( !$disabled || (!obraAditivoPossuiCronograma() || !obraAditivoPossuiVistoria()) ) { ?>
		<?php if( $habilitado && $ttaid != 1 ) { ?>
			<a href="#" onclick="inserirEtapas();"><img src="/imagens/gif_inclui.gif" style="cursor:pointer;" border="0" title="Inserir Etapas">&nbsp;&nbsp;Inserir Serviços</a>
		<?php } ?>
		</td>		
	</tr>
	<tr bgcolor="#C0C0C0">
		<td>
			<div style="float: left;">
				<?php //if($habilitado || (!obraAditivoPossuiCronograma() || !obraAditivoPossuiVistoria()) ){ ?>
				<?php if( $habilitado ){ ?>
					<input type="button" id="salvar" value="Salvar" style="cursor: pointer" <?=$somenteLeitura?> onclick="submeterListaEtapas('1<?=possuiPerfil(array(PERFIL_SUPERVISORMEC, PERFIL_ADMINISTRADOR));?>');">
				<?php } ?> 
				<input type="button" value="Voltar" style="cursor: pointer" onclick="history.back(-1);">		
			</div>
		</td>
	</tr>
</table>
</form>
<?php chkSituacaoObra(); ?>
