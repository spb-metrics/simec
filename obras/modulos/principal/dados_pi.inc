<?

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

// Cria o título da tela
$titulo_modulo = "Dados do Plano Interno";
monta_titulo( $titulo_modulo, "Segue abaixo os dados do Plano Interno Selecionado");
?>

<html>
	<head>
		<title><?php echo $GLOBALS['parametros_sistema_tela']['nome_e_orgao']; ?></title>
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<script src="/includes/prototype.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
	</head>
	<body>
		<?php 
		
			if ( empty($_REQUEST['pliid']) ){
				
				echo '<table width="95%;" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
						<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>
					  </table>';
				
			}else{

				$sql = "";
				$sql = "SELECT
							CASE WHEN 
								pl.plititulo IS NOT NULL
							THEN 
								pl.plititulo 
							ELSE 
								'Não Preenchido' END as titulo,
							trim(ac.prgcod||'.'||ac.acacod||'.'||ac.unicod||'.'||ac.loccod||' - '||ac.acadsc) as descricao,
							round(sum( coalesce(rofdot_ini,0) ),2) as dotacaoinicial,
							coalesce((SELECT 
										SUM(plivalor) as valor
									  FROM 
									  	monitora.planointerno 
									  WHERE 
									  	pl.pliptres = pliptres AND 
									  	plistatus='A'),0) as detalhamento,
							pl.pliptres, 
							pl.plilivre as codificacao,
							pl.plidsc,
							pl.plivalor,
							pc.eqdcod as enquadramento,
							pc.eqdcod ||' - '|| pc.eqddsc as enquadramentodsc,
							sub.sbacod ||' - '|| sub.sbatitulo as subacaodsc,
							pc1.cdtcod as nivel,
							pc1.cdtcod ||' - '|| pc1.cdtdsc as niveldsc,
							pc2.cdtcod as apropriacao,
							pc2.cdtcod ||' - '|| pc2.cdtdsc as apropriacaodsc, 
							pc3.cdtcod as modalidade,
							pc3.cdtcod ||' - '|| pc3.cdtdsc as modalidadedsc, 
							pc4.cdtcod ||' - '|| pc4.cdtdsc as executordsc,
							pc5.cdtcod ||' - '|| pc5.cdtdsc as gestordsc,
							
							-- status do PI
							pl.plisituacao as plisituacao,
							-- atividade
							pia.atiid as atividade,
							pl.obrid as obras, 
							UPPER(obr.obrdesc) as obradesc, 
							UPPER(uni.entnome) as nomeunidade,
							UPPER(cam.entnome) as nomecampus 
						FROM monitora.pi_planointerno pl 
						LEFT JOIN obras.obrainfraestrutura obr ON pl.obrid = obr.obrid 
						LEFT JOIN entidade.entidade uni ON uni.entid = obr.entidunidade 
						LEFT JOIN entidade.entidade cam ON cam.entid = obr.entidcampus
						INNER JOIN monitora.acao ac ON ac.acaid = pl.acaid and ac.prgano= pl.pliano
						LEFT JOIN financeiro.execucao e ON trim(pl.pliptres)=trim(e.ptres) and e.rofano = pl.pliano
						INNER JOIN monitora.pi_enquadramentodespesa pc ON pl.eqdid = pc.eqdid
						INNER JOIN monitora.pi_niveletapaensino pc1 ON pl.neeid = pc1.neeid
						INNER JOIN public.combodadostabela pc2 ON pl.cpiid = pc2.cdtid
						LEFT JOIN public.combodadostabela pc3 ON pl.modid = pc3.cdtid
						INNER JOIN monitora.subacao sub ON pl.sbaid = sub.sbaid
						INNER JOIN public.combodadostabela pc4 ON sub.exeid = pc4.cdtid
						INNER JOIN public.combodadostabela pc5 ON sub.gstid = pc5.cdtid
						LEFT JOIN monitora.planointernoatividade pia ON pia.pliid = pl.pliid
						WHERE pl.plicod = '".$_REQUEST['pliid']."' AND
							  ac.acastatus = 'A' AND
							  ac.acasnrap = false AND
							  pl.plistatus='A'
						GROUP BY 
							pl.pliid, pl.plivalor, ac.prgcod, pl.pliptres, ac.acacod, ac.acadsc, 
							ac.loccod, ac.unicod, pl.plititulo, pc.cdtcod, pc.cdtdsc, pl.plidsc, pl.plilivre, pl.pliacao, 
							pc1.cdtcod, pc1.cdtdsc, pc2.cdtcod, pc2.cdtdsc, pc3.cdtcod, pc3.cdtdsc,
							pc4.cdtcod, pc4.cdtdsc, pc5.cdtcod, pc5.cdtdsc,
							sub.sbacod, sub.sbatitulo, pl.plisituacao,pia.atiid, pl.obrid, obrdesc, nomeunidade, nomecampus
						ORDER BY 
							pl.pliid";
				
				$sql = "SELECT 
							pl.plititulo, pl.plilivre, pl.plidsc, pl.plicod, pl.plisituacao, pl.plicodsubacao,
							en.eqdcod, en.eqddsc,
							su.sbaid, su.sbadsc, su.sbacod,
							ex.piecod, ex.piedsc,
							gs.pigcod, gs.pigdsc,
							ni.neecod, ni.needsc,
							ca.capcod, ca.capdsc 
						FROM monitora.pi_planointerno pl 
						LEFT JOIN monitora.pi_enquadramentodespesa en ON pl.eqdid = en.eqdid 
						LEFT JOIN monitora.pi_subacao su ON su.sbaid = pl.sbaid 
						LEFT JOIN monitora.pi_executor ex ON ex.pieid = su.pieid
						LEFT JOIN monitora.pi_gestor gs ON gs.pigid = su.pigid 
						LEFT JOIN monitora.pi_niveletapaensino ni ON ni.neeid = pl.neeid 
						LEFT JOIN monitora.pi_categoriaapropriacao ca ON ca.capid = pl.capid
						WHERE pl.pliid='".$_REQUEST['pliid']."'";
				
				$dadoscomuns = $db->pegaLinha($sql);
				
				if($dadoscomuns) {
					
					$plicod = $dadoscomuns['plicod'];
					$plititulo = $dadoscomuns['plititulo'];
					$plilivre = $dadoscomuns['plilivre'];
					$plidsc = $dadoscomuns['plidsc'];
					$plisituacao = $dadoscomuns['plisituacao'];
					
					$enquadramentocod = $dadoscomuns['eqdcod'];
					$enquadramentodsc = $dadoscomuns['eqddsc'];
					
					$subacaoid = $dadoscomuns['sbaid'];
					$subacaocod = $dadoscomuns['sbacod'];
					if(!$subacaocod) $subacaocod = $dadoscomuns['plicodsubacao'];
					$subacaodsc = $dadoscomuns['sbadsc'];
					
					$executorcod = $dadoscomuns['piecod'];
					$executordsc = $dadoscomuns['piedsc'];

					$gestorcod = $dadoscomuns['pigcod'];
					$gestordsc = $dadoscomuns['pigdsc'];
					
					$nivelcod = $dadoscomuns['neecod'];
					$niveldsc = $dadoscomuns['needsc'];
					
					$categoriacod = $dadoscomuns['capcod'];
					$categoriadsc = $dadoscomuns['capdsc'];

				} 
			?>
		<form method="post" name="pesquisa" id="pesquisa">
			
			<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td colspan="2">
						<?
					if ($nao_mostra_cabecalho==false) {
						if($obras) {
							echo "<table cellpadding=\"3\" border=\"0\" width=\"98%\" align=\"center\"  style=\"BORDER-RIGHT: #C9C9C9 1px solid; BORDER-TOP: #C9C9C9 1px solid; BORDER-LEFT: #C9C9C9 1px solid; BORDER-BOTTOM: #C9C9C9 1px solid;\">
									<tr>
										<td class='SubTituloDireita'><strong>UNIDADE :</strong></td>
										<td>".$nomeunidade."</td>
									</tr>
									<tr>
										<td class='SubTituloDireita'><strong>CAMPUS :</strong></td>
										<td>".$nomecampus."</td>
									</tr>
									<tr>
										<td class='SubTituloDireita'><strong>OBRA :</strong></td>
										<td>".$obradesc."</td>
									</tr>
									
								  </table>";
						}
					}
						?>
					</td>
				</tr>
				<tr>
				    <td colspan="2">
				        <table cellpadding="0" border="0" width="98%" align="center" id="orcamento"  style="BORDER-RIGHT: #C9C9C9 1px solid; BORDER-TOP: #C9C9C9 1px solid; BORDER-LEFT: #C9C9C9 1px solid; BORDER-BOTTOM: #C9C9C9 1px solid;">
						<tr>
						<td style="background-color: #C9C9C9;" colspan="7" align="center"><b>Detalhamento Orçamentário<br/> Valores em Reais (R$)</b></td>
						</tr>
				        <tr>
							<td style="background-color: #C9C9C9;" align="center" nowrap><b>PTRES</b><input type="hidden" name="pliptres"></td>
							<td style="background-color: #C9C9C9; width:45%;" align="center" nowrap><b>Ação</b></td>
							<td style="background-color: #C9C9C9; width:100px;" align="center" nowrap><b>Dotação Inicial</b></td>
							<td style="background-color: #C9C9C9; width:100px;" align="center" nowrap><b>Dotação Subação</b></td>
							<td style="background-color: #C9C9C9; width:100px;" align="center" nowrap><b>Detalhado no PI</b></td>
							<td style="background-color: #C9C9C9; width:100px;" align="center"><b>Dotação Disponível</b></td>
							<td style="background-color: #C9C9C9;" align="center"><b>Valor Previsto(Anual)</b></td>

						</tr>
						<? 
						$sql = "SELECT
									pl.pliid,
									ptr.ptres,
									pt.ptrid,
									pt.pipvalor, 
									ptr.acaid,
									trim(ac.prgcod||'.'||ac.acacod||'.'||ac.unicod||'.'||ac.loccod||' - '||ac.acadsc) as descricao,
									sum(ptr.ptrdotacao) as dotacaoinicial,
									round(sum( coalesce(sad.sadvalor,0) ),2) as dotacaosubacao,
									coalesce((SELECT SUM(pipvalor) as valor FROM monitora.pi_planointernoptres WHERE pt.ptrid = ptrid AND plistatus='A'),0) as detalhamento
								FROM monitora.pi_planointerno pl
								INNER JOIN monitora.pi_planointernoptres pt ON pt.pliid = pl.pliid 
								LEFT JOIN monitora.ptres ptr ON ptr.ptrid = pt.ptrid 
								LEFT JOIN monitora.acao ac ON ac.acaid = ptr.acaid
								LEFT JOIN monitora.pi_subacaodotacao sad ON ptr.ptrid = sad.ptrid
								WHERE
										pl.pliid = '".$_REQUEST['pliid']."' AND
										pl.plistatus='A'
						    	GROUP BY pl.pliid, pt.ptrid, ptr.ptres, pl.plistatus, pt.pipvalor, ac.prgcod, ptr.acaid, ac.acacod, ac.unicod, ac.loccod, ac.acadsc
						    	ORDER BY ptr.ptres";
						
						$dadosPI = $db->carregar($sql);
						
						if($dadosPI[0]) {
							$valortotalpi = 0;
							$cor = 0;
							foreach($dadosPI as $acpl) { 
						?>
				        <tr style="height:30px;<? echo (($cor%2)?"":"background-color:#DCDCDC;"); ?>" id="ptres_<? echo $acpl['ptres']; ?>">
							<td align="center"><? echo $acpl['ptres']; ?></td>
							<td align="center"><? echo $acpl['descricao']; ?></td>
						    <td align="right"><? echo number_format($acpl['dotacaoinicial'],2,',','.'); ?></td>
						    <td align="right"><? echo number_format($acpl['dotacaosubacao'],2,',','.'); ?></td>
						    <td align="right"><? echo number_format($acpl['detalhamento'],2,',','.'); ?></td>
						    <td align="right"><? echo number_format(($acpl['dotacaosubacao']-$acpl['detalhamento']),2,',','.'); ?></td>
						    <td align="center"><input type="text" name="plivalored[<? echo $acpl['ptrid']; ?>]" size="28" maxlength="" value="<? echo number_format($acpl['pipvalor'],2,',','.'); ?>" onKeyUp="this.value=mascaraglobal('###.###.###.###,##',this.value);calculovalorPI();"  disabled class="disabled"  onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this); verificaDisponivel(this,'<? echo $acpl['ptres']; ?>','<? echo number_format($acpl['pipvalor'],2,',','.'); ?>');" style="text-align : right; width:25ex;" title='' /></td>
						</tr>
						<? 
								$cor++;
								$valortotalpi = $valortotalpi + $acpl['pipvalor']; 
							}
						} 
						?>
				        <tr style="height: 30px;">
							<td align="right" valign="top" colspan="6"><b>TOTAL :</b></td>
							<td align="center" valign="top"><input type="text" name="valortotalpi" id="valortotalpi" size="28" maxlength="" value="<? echo number_format($valortotalpi,2,',','.'); ?>" onKeyUp="this.value=mascaraglobal('###.###.###.###,##',this.value);" disabled  class="disabled"  onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="text-align : right; width:25ex;" title='' /></td>
						</tr>
				        </table>
				    </td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" nowrap>Enquadramento da Despesa:</td>
				    <td><? echo $enquadramentodsc; ?></td>
				</tr>
				<? if($subacaodsc) { ?>
				<tr>
					<td align='right' class="SubTituloDireita">Subação:</td>
				    <td><? echo $subacaocod." - ".$subacaodsc; ?></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita">Executor Orçamentário e Financeiro:</td>
					<td><? echo $executorcod." - ".$executordsc; ?></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita">Gestor da Subação:</td>
					<td><? echo $gestorcod." - ".$gestordsc; ?></td>
				</tr>
				<? } else { ?>
				<tr>
					<td align='right' class="SubTituloDireita">Código da subação:</td>
				    <td><? echo $subacaocod; ?></td>
				</tr>
				<? } ?>
				<tr>
					<td align='right' class="SubTituloDireita">Nível/Etapa de Ensino:</td>
				    <td><? echo $nivelcod." - ".$niveldsc; ?></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita">Categoria de Apropriação:</td>
				    <td><? echo $categoriacod." - ".$categoriadsc; ?></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita">Codifição da Unidade(livre):</td>
				    <td><? echo $plilivre; ?></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita">Título:</td>
				    <td><? echo $plititulo; ?></td>
				</tr>
				
				<tr>
				    <td align='right' class="SubTituloDireita">Descrição:</td>
				    <td><?=$plidsc?></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita">Situação:</td>
				    <td align="left">
				    <?
				    	switch($plisituacao) {
				    		case 'P':
				    			echo "<font color=\"red\">Pendente</font>";
				    			break;
				    		case 'C':
				    			echo "<font color=\"green\">Aprovado</font>";
				    			break;
				    		case 'H':
				    			echo "<font color=\"blue\">Homologado</font>";
				    			break;
				    		case 'V':
				    			echo "<font color=\"#3F85FF\">Revisado</font>";
				    			break;
				    		case 'S':
				    			echo "<font color=\"#AF7817\">Cadastrado no SIAFI</font>";
				    			break;
				    		case 'R':
				    			echo "<font color=\"#EAC117\">Enviado para Revisão</font>";
				    			break;
				    	}
				    ?>
				    </td>
				</tr>
				<tr>
				    <td align='right' class="SubTituloDireita">Previsão PI:</td>
				    <td align="left">
				    	<table style="background-color: #C9C9C9" cellpadding="" border="0" width="126px" >
			        		<tr>
				        		<td>
				        		<table cellpadding="0" border="0" width="98%" >
					        		<tr>
					        		<td style="width: 30px; height: 20px; background-color: #C9C9C9;" align="center"><b>Enquadramento</b></td>
					        		<td align="center" colspan="3"><b>Subação</b></td>
					        		<td style="width: 30px; height: 20px; background-color: #C9C9C9;" align="center"><b>Nível</b></td>
					        		<td style="width: 30px; height: 20px; background-color: #C9C9C9;" align="center"><b>Apropriação</b></td>
					        		<td style="width: 30px; height: 20px; background-color: #C9C9C9;" align="center"><b>Codificação</b></td>
					        		</tr>
					        		<tr>
					        			<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" ><? echo $enquadramentocod; ?></td>
						        		<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" colspan=3><? echo $subacaocod; ?></td>
										<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center"><? echo $nivelcod; ?></td>
										<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" ><? echo $categoriacod; ?></td>
										<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" ><? echo $plilivre; ?></td>		        			        			  
					        		</tr>
					        		<tr><td style="background-color: #C9C9C9;" align="center" colspan="8">&nbsp;</td></tr>
					        		<tr>
					        		<td style="background-color: #C9C9C9;" align="center" colspan="8"><b>Código do PI/SIAFI:</b> <? echo $plicod; ?></td>
					        		</tr>
					        	</table>	        		
				        		</td>
			        		</tr>
				        </table>
				    </td>
				</tr>
				<tr>
					<td align="center" colspan="6" style="background-color: rgb(201, 201, 201);">
						<b>Histórico do PI</b>
					</td>
				</tr>
			</table>
			<center>
			<div id="valida" style="overflow:auto; width:95%; height:150px; border-bottom:2px solid #cccccc;">
				<?php 
						$sql = "";
						$sql = "SELECT
									to_char(po.pihdata, 'DD/MM/YYYY HH24:MI:SS') as data,
									CASE WHEN
											po.pihsituacao = 'A' 
										THEN 
											' Aprovado '
										WHEN
											po.pihsituacao = 'C' 
										THEN 
											' Cadastrado no SIAFI '
										WHEN
											po.pihsituacao = 'S' 
										THEN 
											' Confirmado no SIAFI '
										WHEN
											po.pihsituacao = 'H' 
										THEN 
											' Homologado '
										WHEN
											po.pihsituacao = 'R' 
										THEN 
											' Revisado '
										WHEN
											po.pihsituacao = 'E' 
										THEN 
											' Enviado para Revisão '
										WHEN
											po.pihsituacao = 'P' 
										THEN 
											' Pendente de Aprovação ' END as situacao,
									po.pihobs,
									su.usunome
								FROM
									monitora.pi_planointernohistorico po
								INNER JOIN
									seguranca.usuario su ON po.usucpf = su.usucpf 
								WHERE
									po.pliid = '".$_REQUEST['pliid']."'";
						
						$cabecalho = array("Data/Hora", "Situação", "Observação", "Cadastrado Por");
						$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%' );
					?>
			</div>
			</center>
		<?php } ?>
		</form>
	</body>
</html>
