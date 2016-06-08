<?php
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php'; 
include APPRAIZ . 'www/cte/_funcoes.php';

$obras = new Obras();
$dobras = new DadosObra(null);
$faseprojeto = new DadosFasesProjeto();

if($_SESSION['obra']['obrid']){
	$resultado = $faseprojeto->busca($_SESSION['obra']['obrid']);	
	$dados = $faseprojeto->dados($resultado);
}

if ( $_REQUEST['requisicao'] ){
	$obras->CadastrarProjetoArquitetonico( $_REQUEST );
}

?>

<br/>

<?php

$db->cria_aba($abacod_tela,$url,$parametros);

$titulo_modulo = "Projetos";
monta_titulo( $titulo_modulo, "<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /> Indica os campos obrigatórios");

echo $obras->CabecalhoObras();

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<form name="formulario" method="post" onsubmit="return validaProjetoArquitetonico(this, 1 <?=possuiPerfil(array(PERFIL_SUPERVISORMEC, PERFIL_ADMINISTRADOR));?>);" action="<?php echo $caminho_atual;?>acao=A"> 
	<input type="hidden" name="requisicao" value="executar"/> 
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td colspan="2">
				<div align="center">
					<font color="red">
						<br/>
						<b>O arquivo do projeto indicado nesta aba deve ser inserido na aba Documentos (pdf)</b>
						<br/><br/>
					</font>
				</div>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Tipo de Projeto</td>
			<td>
				<?
				if($_SESSION['obra']['obrid']){
				?>
					<input type="hidden" name="fprid" value="<? echo $faseprojeto->fprid; ?>">
				<?			
				}
				?>
				<?php
					$tpaid = $faseprojeto->tpaid;
					$sql = "SELECT 
								tpaid as codigo, 
								tpadesc as descricao 
							FROM 
								obras.tipoprojetoarquitetonico";
					 
					$db->monta_combo("tpaid", $sql, $somenteLeitura, "Selecione...", '', '', '', '', 'S', 'tpaid');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Forma de Elaboração do projeto</td>
			<td>
				<?php
					$felid = $faseprojeto->felid;
					$sql = "SELECT 
								felid as codigo, 
								feldesc as descricao 
							FROM 
								obras.formaelaboracao ORDER BY feldesc ASC";
					 
					$db->monta_combo("felid", $sql, $somenteLeitura, "Selecione...", 'abreFormaElaboracao', '', '', '', 'S', 'felid');
				?>
			</td>
		</tr>
		<tr id="recurso_proprio" style="display: none;">
			<td class="SubTituloDireita">Recurso Próprio R$</td>
			<td>
				<? $fprvlrformaelabrecproprio =  number_format($faseprojeto->fprvlrformaelabrecproprio,2,',','.'); ?>
				<?= campo_texto( 'fprvlrformaelabrecproprio', 'N', $somenteLeitura, '', 17, 14, '###.###.###,##',  '', 'left', '', 0); ?>
			</td>
		</tr>
		<tr id="recurso_repassado" style="display: none;">
			<td class="SubTituloDireita">Recurso Repassado R$</td>
			<td>
				<? $fprvlrformaelabrrecrepassado = number_format($faseprojeto->fprvlrformaelabrrecrepassado,2,',','.');	?>
				<?= campo_texto( 'fprvlrformaelabrrecrepassado', 'N', $somenteLeitura, '', 17, 14, '###.###.###,##',  '', 'left', '', 0); ?>
			</td>
		</tr>
		<tr id="observacoes_direta" style="display: none;">
			<td class="SubTituloDireita">Observações: </td>
			<td>
				<? $fprobsexecdireta = $faseprojeto->fprobsexecdireta; ?>
				<?= campo_textarea( 'fprobsexecdireta', 'N', $somenteLeitura, '', '70', '4', '500'); ?>
			</td>
		</tr>
		<tr id="observacoes_contrapartida" style="display: none;">
			<td class="SubTituloDireita">Observações: </td>
			<td>
				<? $fprobsprojcontrapartida = $faseprojeto->fprobsprojcontrapartida; ?>
				<?= campo_textarea( 'fprobsprojcontrapartida', 'N', $somenteLeitura, '', '70', '4', '500'); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Fases do Projeto</td>
			<td>
				<?php
					$tfpid = $faseprojeto->tfpid;
					$sql = "SELECT 
								tfpid as codigo, 
								tfpdesc as descricao 
							FROM 
								obras.tipofaseprojeto";
					 
					$db->monta_combo("tfpid", $sql, $somenteLeitura, "Selecione...", '', '', '', '', 'S', 'tfpid');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de Início: </td>
			<td>
			
				<? $fprdtiniciofaseprojeto = $faseprojeto->fprdtiniciofaseprojeto;?>
				<?= campo_data2( 'fprdtiniciofaseprojeto', 'S', $somenteLeitura, '', 'S' ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data da Término: </td>
			<td>
				<? $fprdtconclusaofaseprojeto = $faseprojeto->fprdtconclusaofaseprojeto; ?>
				<?= campo_data2( 'fprdtconclusaofaseprojeto', 'S', $somenteLeitura, '', 'S', '', "obrValidaData(document.getElementById('fprdtiniciofaseprojeto'), this);" ); ?>
			</td>
		</tr>
		
		<tr id="fasesprojeto" style="display: none;">
			<td colspan='2'>
			
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td>Fases de Licitação do Projeto</td>
					</tr>
					<tr>
						<td colspan="2">
							<table id="faseslicitacao" width="70%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
								<thead>
									<tr bgcolor="#e9e9e9">
										<td width="10%" valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
										<td width="70%" valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Descrição</strong></td>
										<td width="20%" valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Data</strong></td>						
									</tr>
								</thead>
								<?php 

									$fprid = $faseprojeto->fprid;
									if($fprid){
										
										$sql = pg_query("
											SELECT 
												fl.*,
												tfl.tfldesc  
											FROM 
												obras.faselicitacaoprojetos fl 
											INNER JOIN 
												obras.tiposfaseslicitacao tfl ON fl.tflid = tfl.tflid
											WHERE 
												fl.fprid = '". $fprid . "' AND fl.tfpstatus = 'A' ORDER BY tfl.tflordem");
										
										while (($dadosx = pg_fetch_assoc($sql))){
											$flcid = $dadosx['flpid'];
											$tflid = $dadosx['tflid'];
											$tfldesc = $dadosx['tfldesc'];
											$flcrecintermotivo = $dadosx['tfpobsmotivo'];
											$flcordservnum = $dadosx['tfpnumos'];
											$flcpubleditaldtprev = formata_data($dadosx['tfpdtfase']);
											$flcdtrecintermotivo = formata_data($dadosx['tfpdtfase']);
											$flcordservdt = formata_data($dadosx['tfpdtfase']);
											$flchomlicdtprev = formata_data($dadosx['tfpdtfase']);
											$flcaberpropdtprev = formata_data($dadosx['tfpdtfase']);
											
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
											
											
											
											if($somenteLeitura == "N")
												$botoes = "<img src=\"/imagens/excluir_01.gif\" style=\"cursor: pointer\"  border=0 title=\"Excluir\">";
											else
												$botoes = "
													<img src='/imagens/alterar.gif' style='cursor: pointer' border=0 title='Editar' onclick=\"atualizaFase(" . $tflid . ",".$flcid.",'update');\"/>&nbsp&nbsp&nbsp
													<img src=\"/imagens/excluir.gif\" style=\"cursor: pointer\"  border=0 title=\"Excluir\" onClick=\"RemoveLinha(window.document.getElementById('tr_".$flcid."').rowIndex,'".$flcid."');\">
												";
											
											echo "
												<tr id=\"tr_" . $flcid . "\">
													<td bgcolor=\"#F7F7F7\" align=\"center\">" . $botoes . "</td>
													<td><input type='hidden' name='flcid[]' id='flcid_".$flcid."'  value='".$flcid."'/>" . $tfldesc . "</td>
													<td>
													<input type='hidden' name='flcpubleditaldtprev[]' id='flcpubleditaldtprev_".$flcid."' value='".$flcpubleditaldtprev."'/>
													<input type='hidden' name='flcdtrecintermotivo[]' id='flcdtrecintermotivo_".$flcid."' value='".$flcdtrecintermotivo."'/>
													<input type='hidden' name='flcordservdt[]' id='flcordservdt_".$flcid."' value='".$flcordservdt."'/>
													<input type='hidden' name='flchomlicdtprev[]' id='flchomlicdtprev_".$flcid."' value='".$flchomlicdtprev."'/>
													<input type='hidden' name='flcaberpropdtprev[]' id='flcaberpropdtprev_".$flcid."' value='".$flcaberpropdtprev."'/>									
													" . $flcdata . "
													<input type='hidden' name='flcrecintermotivo[]' id='flcrecintermotivo_".$flcid."' value='".$flcrecintermotivo."'/>
													<input type='hidden' name='flcordservnum[]' id='flcordservnum_".$flcid."' value='".$flcordservnum."'/>
													<input type='hidden' name='tflid[]' id='tflid_".$flcid."' value='".$tflid."'/>		
													</td>
												</tr>
											";
										}
									}
								
								?>
							</table>
							<div id='conteinerAcao'>			
							</div>
						</td>
					</tr>
					<tr>
						<td>
							<?php if($habilitado) { ?>
								<a href="#" onclick="adicionarFases('projeto');"><img src="/imagens/gif_inclui.gif" style="cursor:pointer;" border="0" title="Inserir Fases"> Inserir Fases</a>
							<?php } ?>
						</td>
					</tr>
				</table>			
			
			</td>
		</tr>
		
		<tr bgcolor="#C0C0C0">
			<td></td>
			<td>
				<div style="float: left;">
					<?php if ($habilitado){ ?>
						<input type="submit" value="Salvar" style="cursor: pointer" <?php if($somenteLeitura=="N") echo "disabled"; ?>>
					<?php } ?> 
					<input type="button" value="Voltar" style="cursor: pointer" onclick="history.back(-1);">
				</div>
			</td>
		</tr>
	</table>
</form>	
<script>



//alert(superuser);

RemoveLinha = function(index,id){
	table = window.document.getElementById("faseslicitacao");
	var conteiner = window.document.getElementById("conteinerAcao");
	conteiner.innerHTML += "<input type='hidden' name='acaoFases[]' id='acaoFases"+id+"' value='"+id+"'/>";
	table.deleteRow(index);
}

function abreFormaElaboracao(id){
	
	var tr_recurso_proprio  		  = document.getElementById( 'recurso_proprio' );
	var tr_recurso_repassado  		  = document.getElementById( 'recurso_repassado' );
	var tr_observacoes_direta  		  = document.getElementById( 'observacoes_direta' );
	var tr_observacoes_contrapartida  = document.getElementById( 'observacoes_contrapartida' );
	var tr_fasesprojeto  			  = document.getElementById( 'fasesprojeto' );
					
	if(id == ''){
		if (document.selection){
			tr_recurso_proprio.style.display = 'none';
			tr_recurso_repassado.style.display = 'none';
			tr_observacoes_direta.style.display = 'none';
			tr_observacoes_contrapartida.style.display = 'none';
			tr_fasesprojeto.style.display = 'none';
		}else{
			tr_recurso_proprio.style.display = 'none';
			tr_recurso_repassado.style.display = 'none';
			tr_observacoes_direta.style.display = 'none';
			tr_observacoes_contrapartida.style.display = 'none';
			tr_fasesprojeto.style.display = 'none';
		}
	}
	if(id == 1 || id ==4 ){
		if (document.selection){
			tr_observacoes_direta.style.display = 'block';
			tr_recurso_proprio.style.display = 'none';
			tr_recurso_repassado.style.display = 'none';
			tr_observacoes_contrapartida.style.display = 'none';
			tr_fasesprojeto.style.display = 'none';			
			
		}else{
			tr_observacoes_direta.style.display = 'table-row';
			tr_recurso_proprio.style.display = 'none';
			tr_recurso_repassado.style.display = 'none';
			tr_observacoes_contrapartida.style.display = 'none';
			tr_fasesprojeto.style.display = 'none';
			
		}
	}
	if(id == 2){
		if (document.selection){
			tr_recurso_proprio.style.display = 'block';
			tr_recurso_repassado.style.display = 'block';
			tr_fasesprojeto.style.display = 'block';
			tr_observacoes_direta.style.display = 'none';
			tr_observacoes_contrapartida.style.display = 'none';
			
			
		}else{
			tr_recurso_proprio.style.display = 'table-row';
			tr_recurso_repassado.style.display = 'table-row';
			tr_fasesprojeto.style.display = 'table-row';
			tr_observacoes_direta.style.display = 'none';
			tr_observacoes_contrapartida.style.display = 'none';
			
		}
	}

	if(id == 3){
		if (document.selection){
			tr_observacoes_direta.style.display = 'none';
			tr_recurso_proprio.style.display = 'none';
			tr_recurso_repassado.style.display = 'none';
			tr_observacoes_contrapartida.style.display = 'block';
			tr_fasesprojeto.style.display = 'none';
		}else{
			tr_observacoes_direta.style.display = 'none';
			tr_recurso_proprio.style.display = 'none';
			tr_recurso_repassado.style.display = 'none';
			tr_observacoes_contrapartida.style.display = 'table-row';
			tr_fasesprojeto.style.display = 'none';
		}
	}
		
}
function abreFasesProjeto(id){

	//var tr_iniciado  = document.getElementById( 'iniciado' );
	var tr_execussao  = document.getElementById( 'execussao' );
	var tr_concluido  = document.getElementById( 'concluido' );

			
	if(id == ''){
		if (document.selection){
			//tr_iniciado.style.display = 'none';
			tr_execussao.style.display = 'none';
			tr_concluido.style.display = 'none';
			
		}else{
			//tr_iniciado.style.display = 'none';
			tr_execussao.style.display = 'none';
			tr_concluido.style.display = 'none';
		}
	}
	if(id == 1){
		if (document.selection){
			//tr_iniciado.style.display = 'block';
			tr_execussao.style.display = 'none';
			tr_concluido.style.display = 'none';			
			
		}else{
			//tr_iniciado.style.display = 'table-row';
			tr_execussao.style.display = 'none';
			tr_concluido.style.display = 'none';
			
		}
	}
	if(id == 2){
		if (document.selection){
			//tr_iniciado.style.display = 'none';
			tr_execussao.style.display = 'block';
			tr_concluido.style.display = 'none';
			
		}else{
			//tr_iniciado.style.display = 'none';
			tr_execussao.style.display = 'table-row';
			tr_concluido.style.display = 'none';
		}
	}

	if(id == 3){
		if (document.selection){
			//tr_iniciado.style.display = 'none';
			tr_execussao.style.display = 'none';
			tr_concluido.style.display = 'block';
		}else{
			//tr_iniciado.style.display = 'none';
			tr_execussao.style.display = 'none';
			tr_concluido.style.display = 'table-row';
		}
	}
	
}

</script>
<? 
if($_SESSION['obra']['obrid'] )
	echo "<script>
		 
		abreFormaElaboracao(window.document.getElementsByName('felid')[0].value);
		
		</script>";
?>