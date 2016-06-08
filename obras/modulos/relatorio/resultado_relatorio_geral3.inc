<?php
	
	if( $_POST['estuf'][0] != "" ){
		$estados = "AND te.estuf IN ( '". implode("' , '", $_POST['estuf']) ."' )";
		$estadosBusca = implode(", ", $_POST['estuf']);
	}else{
		$estados = "";
		$estadosBusca = "Todos";
	}
	
	if( $_POST['dataini'] != "" && $_POST['datafim'] != "" ){
		$periodo = "AND orsdtemissao BETWEEN '".formata_data_sql($_POST['dataini'])."' AND '".formata_data_sql($_POST['datafim'])."'";
		$periodoBusca = "De ".$_POST['dataini']." até ".$_POST['datafim'];
	}else{
		$periodo = "";
		$periodoBusca = "Tudo";
	}
	
	$sql = "SELECT 
				gd.estuf,
				gd.gpdid,
				tr.regdescricao,
				'<a onmouseout=\"SuperTitleOff( this );\" onmousemove=\"SuperTitleAjax(\'obras.php?modulo=relatorio/relatorio_geral3&acao=A&gpdid=' || gd.gpdid ||'\', this);\">'|| gd.gpdid || '/' || os.orsid || '</a>' AS os
			FROM
				obras.ordemservico os
			JOIN obras.grupodistribuicao gd ON gd.gpdid = os.gpdid
							   				   AND gd.gpdstatus = 'A'
			
			JOIN territorios.estado te ON te.estuf = gd.estuf
			
			JOIN territorios.regiao tr ON tr.regcod = te.regcod
							   
			WHERE
				os.orsstatus = 'A'
				$estados
				$periodo
			ORDER BY gd.estuf";
	
	$dados = $db->carregar( $sql );
	
	if( is_array($dados) ){
	
		$arDados = array();
		// array que serve para salvar os ufs já cadastrados
		$ufs = array();
		foreach ( $dados as $dado ){
			$obObra  = new ObraInfraestrutura();
			$obGrupo = new GrupoDistribuicao();
			
			$qtdOrg = 0;
			// TOTAL DO PROCEDIMENTO
			// Superior
			$arObridSup			  = $obObra->listaIdObraPorGrupo( $dado['gpdid'], ORGAO_SESU );
			$totalProcedimentoSup = (float) $obGrupo->pegaTotalValorProcedimento( $arObridSup, array("orgid" => ORGAO_SESU) );
			$qtdOrg += $arObridSup ? 1 : 0;
			// Profissional
			$arObridPro			  = $obObra->listaIdObraPorGrupo( $dado['gpdid'], ORGAO_SETEC );
			$totalProcedimentoPro = (float) $obGrupo->pegaTotalValorProcedimento( $arObridPro, array("orgid" => ORGAO_SETEC) );
			$qtdOrg += $arObridPro ? 1 : 0;
			// Básico
			$arObridBas			  = $obObra->listaIdObraPorGrupo( $dado['gpdid'], ORGAO_FNDE );
			$totalProcedimentoBas = (float) $obGrupo->pegaTotalValorProcedimento( $arObridBas, array("orgid" => ORGAO_FNDE) );
			$qtdOrg += $arObridBas ? 1 : 0;
			
			// TOTAL DO DESLOCAMENTO
			$obModel 		   = new DeslocamentoController();
			$totalDeslocamento = $obModel->totalTrajetos( $dado['gpdid'] );
			$deslocamento = $totalDeslocamento['valorTotal'];
			
			switch( $qtdOrg ){
				case 1:
					$totalDeslocamento = $totalDeslocamento['valorTotal'];			
					break;
				case 2: 
					$totalDeslocamento = $totalDeslocamento['valorTotal'] / 2;			
					break;
				case 3:
					$totalDeslocamento = $totalDeslocamento['valorTotal'] / 3;			
					break;
			}
			
			$totSup = (float) $arObridSup ? $totalDeslocamento + $totalProcedimentoSup : 0;
			$totPro = (float) $arObridPro ? $totalDeslocamento + $totalProcedimentoPro : 0;
			$totBas = (float) $arObridBas ? $totalDeslocamento + $totalProcedimentoBas : 0;
			
			$sql = "SELECT 
						COALESCE(psusesu, 0) AS psusesu, 
						COALESCE(psusetec, 0) AS psusetec, 
						COALESCE(psufnde, 0) AS psufnde 
					FROM 
						obras.previsaosupervisao
					WHERE
						estuf = '{$dado['estuf']}'";
			
			$obrPrevistas = $db->pegaLinha($sql);
			
			if( !in_array($dado['estuf'], $ufs) ){
				
				$arLinha['regdescricao']  = $dado['regdescricao'];
				$arLinha['estuf']		  = $dado['estuf'];
				$arLinha['os']		 	  = $dado['os'];
			 
				$arLinha['valortotalsup'] = $totSup; 
				$arLinha['qtdsup'] 		  = count($arObridSup);
				$arLinha['psusesu'] 	  = $obrPrevistas['psusesu'];
				$arLinha['sesudivisao']	  = ( $arLinha['psusesu'] ? ($arLinha['qtdsup'] / $arLinha['psusesu']) : 0 );  
				
				$arLinha['valortotalpro'] = $totPro; 
				$arLinha['qtdpro'] 		  = count($arObridPro);
				$arLinha['psusetec'] 	  = $obrPrevistas['psusetec'];
				$arLinha['setecdivisao']  = ( $arLinha['psusetec'] ? ($arLinha['qtdpro'] / $arLinha['psusetec']) : 0 ); 
				
				$arLinha['valortotalbas'] = $totBas; 
				$arLinha['qtdbas'] 		  = count($arObridBas);
				$arLinha['psufnde'] 	  = $obrPrevistas['psufnde'];
				$arLinha['fndedivisao']	  = ( $arLinha['psufnde'] ? ($arLinha['qtdbas'] / $arLinha['psufnde']) : 0 );
				
				// salvando o estado para fazer a próxima verificação
				$ufs[] = $dado['estuf'];
				
			}else{
				$arLinha['regdescricao']  = $dado['regdescricao'];
				$arLinha['estuf']		  = $dado['estuf'];
				$arLinha['os']		 	  = $dado['os'];
			 
				$arLinha['valortotalsup'] = $totSup; 
				$arLinha['qtdsup'] 		  = count($arObridSup);
				$arLinha['psusesu'] 	  = 0;  
				$arLinha['sesudivisao']	  = ( $arLinha['psusesu'] ? ($arLinha['qtdsup'] / $arLinha['psusesu']) : 0 );
				
				$arLinha['valortotalpro'] = $totPro; 
				$arLinha['qtdpro'] 		  = count($arObridPro);
				$arLinha['psusetec'] 	  = 0;
				$arLinha['setecdivisao']  = ( $arLinha['psusetec'] ? ($arLinha['qtdpro'] / $arLinha['psusetec']) : 0 );
				
				$arLinha['valortotalbas'] = $totBas; 
				$arLinha['qtdbas'] 		  = count($arObridBas);
				$arLinha['psufnde'] 	  = 0;
				$arLinha['fndedivisao']	  = ( $arLinha['psufnde'] ? ($arLinha['qtdbas'] / $arLinha['psufnde']) : 0 );
				
			}
			
			// somatório
			$arLinha['deslocamento'] 		 = $deslocamento; // apenas um teste
			$arLinha['valorDaOs']	 		 = (float) $arLinha['valortotalsup'] + $arLinha['valortotalpro'] + $arLinha['valortotalbas'];
			$arLinha['totalObrasPrevistas']	 = (int)$arLinha['psusesu'] + (int)$arLinha['psusetec'] + (int)$arLinha['psufnde'];
			$arLinha['totalObrasDaOs']	 	 = (float) $arLinha['qtdsup'] + $arLinha['qtdpro'] + $arLinha['qtdbas'];
			$arLinha['R']					 = (float) ( $arLinha['totalObrasPrevistas'] ? $arLinha['totalObrasDaOs'] / $arLinha['totalObrasPrevistas'] : 0 );   
//			$arLinha['S']					 = (float) $arLinha['deslocamento'] / $arLinha['valorDaOs'];
			$arLinha['S']					 = ( $arLinha['valorDaOs'] ? (float) $arLinha['deslocamento'] / $arLinha['valorDaOs'] : 0 );

			array_push($arDados, $arLinha);
			unset($arLinha);
			
		}// fim do foreach
	}// fim do if

	// Inclui componente de relatórios
	include APPRAIZ. 'includes/classes/relatorio.class.inc';
	
	// instancia a classe de relatório
	$rel = new montaRelatorio();
	
	// monta o sql, agrupador e coluna do relatório
	$agrupador = obras_monta_agp_relatorio3();
	$coluna    = obras_monta_coluna_relatorio3();
	
	$rel->setAgrupador($agrupador, $arDados); 
	$rel->setColuna($coluna);
	$rel->setTolizadorLinha(true);
	$rel->setTotNivel(true);
	$rel->setEspandir(true);
	?>
		<html>
			<head>
				<title><?php echo $GLOBALS['parametros_sistema_tela']['nome_e_orgao']; ?></title>
				<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
				<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
				<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
				
				<!-- Includes do SuperTitleAjax -->
				<link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
				<script type="text/javascript" src="../includes/remedial.js"></script>
				<script type="text/javascript" src="../includes/superTitle.js"></script>
				
				<script type="text/javascript"><!--
					$(document).ready(function() {
						// colorindo o cabeçalho das tabelas
						var i = 0;
						var arCor = new Array();
						arCor[0] = "FFF5EE"; 
						arCor[1] = "EEE5DE";
						arCor[2] = "CDC5BF";
						
						var idCor = 0;
						var cor   = arCor[0];
						
						var idxSelect = <?php echo(count($_POST['orgid']) * 4) + 1; ?>;
						
						$(".tabela > tbody > tr").each(function() {
							$("td",this).slice(1, idxSelect).each(function (){
							
								if ((i % 4) == 0){
									cor   = arCor[idCor];
									if ( idCor < <?=count($_POST['orgid']) - 1; ?> ){
										idCor++;
									}else{
										idCor = 0;
									}
								}
								$(this).css('background-color', cor);
								i++;
								
							});
						});
						
						// Highlighting
						$(".tabela").find('tbody tr:not(:first:last)').mouseover(function() {
							$('td', this).addClass('highlight');
						});
						
						$(".tabela").find('tbody tr:not(:first:last)').mouseout(function() {
							$('td', this).removeClass('highlight');
						});
						// end Highlighting
						
					});

				--></script>
				<style type="text/css">
					.highlight{
						background-color: #ffffcc !important;
					}
					
					.Table
					{
					    FONT-SIZE: xx-small;
					    BORDER-RIGHT: #cccccc 1px solid;
					    BORDER-TOP: #cccccc 1px solid;
					    BORDER-LEFT: #cccccc 1px solid;
					    BORDER-BOTTOM: #cccccc 1px solid;
						TEXT-DECORATION: none;
						WIDTH: 95%;
						TEXT-COLOR: #000000;
					}
					
					.Table a
					{
					    color: #133368;
						TEXT-DECORATION: none;
					}
					
					.Table a:hover
					{
					    color: #E47100;
						TEXT-DECORATION: underline;
					}
				</style>
	
			</head>
			<body>
			
				<a onclick="window.print();" style="cursor: pointer; float: right; margin-top: 50px; margin-right: 20px; position: absolute; margin-left: 92.2%;" class="notprint">
					<img border="0" src="../imagens/ico_print.jpg">
				</a>
				<center>
					<?php echo monta_cabecalho_relatorio( '95' ); ?>
				</center>
				
<?php	
	$ensinoBusca = "";
	$i = 0;
	foreach ($_POST['orgid'] as $tipo) {
		switch ($tipo) {
			case ORGAO_SESU:
				if($i == 0){
					$ensinoBusca .= " Educação Superior";
				}elseif($i == 1){
					$ensinoBusca .= ", Educação Superior";
				}else{
					$ensinoBusca .= " e Educação Superior";
				}
				$i++;
			break;
						
			case ORGAO_SETEC:
				if($i == 0){
					$ensinoBusca .= " Educação Profissional";
				}elseif($i == 1){
					$ensinoBusca .= ", Educação Profissional";
				}else{
					$ensinoBusca .= " e Educação Profissional";
				}
				$i++;
			break;
						
			case ORGAO_FNDE:
				if($i == 0){
					$ensinoBusca .= " Educação Básica";
				}elseif($i == 1){
					$ensinoBusca .= ", Educação Básica";
				}else{
					$ensinoBusca .= " e Educação Básica";
				}
				$i++;
			break;			
		}
	}
?>
		  <br>
		  <table class="table" cellspacing="1" cellpadding="5" border="0" align="center" width="100%">
		  	<tr colspan="2" align="center">
		  		<td colspan="2" class="subTituloCentro">Parâmetros de Pesquisa:</td>
		  	</tr>
		  	<tr bgcolor="#e5e5e5">
		  		<td align="right" class="SubTituloDireita">Tipo de Estabelecimento:</td>
		  		<td><?php echo $ensinoBusca; ?></td>
		  	</tr>
		  	<tr bgcolor="#e5e5e5">
		  		<td align="right" class="SubTituloDireita">Período:</td>
		  		<td><?php echo $periodoBusca ?></td>
		  	</tr>
		  	<tr bgcolor="#e5e5e5">
		  		<td align="right" class="SubTituloDireita">UF:</td>
		  		<td><?php echo $estadosBusca ?></td>
		  	</tr>
		  </table>
	
<?php
	// Monta o Relatório
	echo $rel->getRelatorio();
?>
			</body>
		  </html>
	  <script language="JavaScript" src="../includes/wz_tooltip.js"></script>
