<?php

$obras     = new Obras();
$dobras    = new DadosObra(null);
$licitacao = new licitacao();

if( !$_SESSION["obra"]["obrid"] ){
	die('<script type="text/javascript">
				alert(\'Seu perfil não permite liberar acesso ao sistema!\');
				window.location.replace("obras.php?modulo=inicio&acao=A");
		 </script>');
}

switch( $_REQUEST["requisicao"] ){
	case "salvar":
		$licitacao->cadastraLicitacao( $_REQUEST );
	break;
}

if(isset($_SESSION['obra']['obrid']) && ($_SESSION['obra']['obrid'] != '')) {
	$resultado = $licitacao->busca($_SESSION['obra']['obrid']);
	$dados 	   = $licitacao->dados($resultado);
}

#apagando o obras da sessão para que não seja exibido datas "estranhas" na tela
unset($_SESSION['obras']);

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";

$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo = "Licitação";
monta_titulo( $titulo_modulo, '' );

echo $obras->CabecalhoObras();

?>
<script language="JavaScript" src="../includes/calendario.js"></script>
<script type="text/javascript">
	
	function salvaLicitacao( superuser ){

		var obData = new Data();

		var tflid = document.getElementsByName("tflid[]");
		var molid = document.getElementById( "molid" ).value;
		
		var flcdata = document.getElementsByName('flcdata[]');
		
		var dtiniciolicitacao = document.getElementById('dtiniciolicitacao');
		var dtfinallicitacao  = document.getElementById('dtfinallicitacao');
		
		if( obData.comparaData( dtfinallicitacao.value, dtiniciolicitacao.value, "<" ) ){
			alert( "O Término Programado deve ser maior do que o Início Programado!" );
			return false;
		}
		
		if( (tflid.length == 0) && (superuser == 1) ){
			alert( "Favor inserir ao menos uma das Fases de Licitação!" );
			return false;
		}else{

			if( superuser == 1 ){
				if( molid == 1 ){
					if( obData.comparaData( dtfinallicitacao.value, obData.dtAddDia( flcdata[0].value, 5 ), "<" ) ){
						alert( "O término Programado não está compatível com a Modalidade de Licitação selecionada!" );
						return false;
					}
				}
				
	//			if( molid == 2 ){
	//				if( obData.comparaData( dtfinallicitacao.value, obData.dtAddDia( flcdata[0].value, 15 ), "<" ) ){
	//					alert( "O término Programado não está compatível com a Modalidade de Licitação selecionada!" );
	//					return false;
	//				}
	//			}
				
				// Se amodalidade for Tomada de Preços e a diferença entre a Data de Publicação do Edital e Término Programado for inferior a 15 dias ERRO!
				if( molid == 2 ){
					if( obData.comparaData( dtfinallicitacao.value, obData.dtAddDia( dtiniciolicitacao.value, 15 ), "<" ) ){
						alert( "O término Programado não está compatível com a Modalidade de Licitação selecionada!" );
						return false;
					}
				}
				
				if( molid == 3 ){
					if( obData.comparaData( dtfinallicitacao.value, obData.dtAddDia( flcdata[0].value, 30 ), "<" ) ){
						alert( "O término Programado não está compatível com a Modalidade de Licitação selecionada!" );
						return false;
					}
				}
						
				if( molid == 4 ){
					if( obData.comparaData( dtfinallicitacao.value, obData.dtAddDia( flcdata[0].value, 10 ), "<" ) ){
						alert( "O término Programado não está compatível com a Modalidade de Licitação selecionada!" );
						return false;
					}
				}
			}
		
			var flag = false;
		
			for( i = 0; i < tflid.length; i++ ){
				
				if( Number(tflid[i].value) == 2 || Number(tflid[i].value) == 9 ){
					flag = true;
					break;
				}
				
			}
			
		}

		if( !flag && (superuser == 1) ){
			alert("Favor inserir a fase Publicação do Edital ou Homologação da Licitação!");
			return false;
		}

		if( dtfinallicitacao.value == "" && (superuser == 1) ){
			alert( "O campo Término Programado deve ser preenchido!" );
			return false;
		}

		if ( molid == '' && (superuser == 1) ){
			alert( "O campo Modalidade de Licitação deve ser preenchido!" );
			return false;
		}else{
			document.getElementById( "formulario" ).submit();
		}
		
	}

	function RemoveLinha(index,id){
		table = window.document.getElementById("faseslicitacao");
		var conteiner = window.document.getElementById("conteinerAcao");
		conteiner.innerHTML += "<input type='hidden' name='acaoFases[]' id='acaoFases"+id+"' value='"+id+"'/>";
		table.deleteRow(index);
	}	
			
	function abreNumLicUasg( id ){
		
		var tr_uasg    = document.getElementById( 'tr_uasg' );
		var tr_numlic  = document.getElementById( 'tr_numlic' );
	
		if( id == 1 || id == 2 || id == 3 || id == 4 ){
			
			if (document.selection){
				tr_uasg.style.display   = 'block';
				tr_numlic.style.display = 'block';
			}else{
				tr_uasg.style.display   = 'table-row';
				tr_numlic.style.display = 'table-row';
			}
			
		}else{
			
			if (document.selection){
				tr_uasg.style.display   = 'none';
				tr_numlic.style.display = 'none';
			}else{
				tr_uasg.style.display   = 'none';
				tr_numlic.style.display = 'none';
			}
			
		}
		
	}
		
</script>
<form method="post" id="formulario" name="formulario">
	<input type="hidden" name="requisicao" id="requisicao" value="salvar"/>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td width="265" class="subtitulodireita">Modalidade de Licitação:</td>
			<td>
				<?php 
				
					$molid = $licitacao->molid;
					
					$sql ="SELECT 
						   		molid AS codigo, moldsc AS descricao 
						   FROM 
								obras.modalidadelicitacao
						   ORDER BY 
								molid";
							
					$db->monta_combo('molid', $sql, $somenteLeitura, "Selecione...", 'abreNumLicUasg', '', '', '', 'S', 'molid'); 
							
				
				?>
			</td>
		</tr>
		<tr>
			<td>Fases de Licitação</td>
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
					
						if($_SESSION['obra']['obrid']){
							
							$sql = pg_query("
								SELECT 
									fl.*,
									tfl.tfldesc, tfl.tflordem   
								FROM 
									obras.faselicitacao fl 
								INNER JOIN 
									obras.tiposfaseslicitacao tfl ON fl.tflid = tfl.tflid
								WHERE 
									fl.obrid = '". $_SESSION['obra']['obrid'] . "' AND fl.flcstatus = 'A' ORDER BY tfl.tflordem");
							
							while (($dados = pg_fetch_assoc($sql))){
								$flcid = $dados['flcid'];
								$tflid = $dados['tflid'];
								$tfldesc = $dados['tfldesc'];
								$flcrecintermotivo = $dados['flcrecintermotivo'];
								$flcordservnum = $dados['flcordservnum'];
								$flcpubleditaldtprev = formata_data($dados['flcpubleditaldtprev']);
								$flcdtrecintermotivo = formata_data($dados['flcdtrecintermotivo']);
								$flcordservdt = formata_data($dados['flcordservdt']);
								$flchomlicdtprev = formata_data($dados['flchomlicdtprev']);
								$flcaberpropdtprev = formata_data($dados['flcaberpropdtprev']);
								$tflordem = $dados['tflordem'];
								
								if($tflid ==2){
									$_SESSION["obras"]["flcpubleditaldtprev"] = formata_data_sql($flcpubleditaldtprev);
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
									$_SESSION["obra"]["flchomlicdtprev"] = $flchomlicdtprev; 
								}
								if($tflid ==7){
									$flcdata = $flcaberpropdtprev;
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
										<input type='hidden' name='flcdata[]' id='flcdata_".$flcid."' value='".$flcdata."'/>
										" . $flcdata . "
										<input type='hidden' name='flcrecintermotivo[]' id='flcrecintermotivo_".$flcid."' value='".$flcrecintermotivo."'/>
										<input type='hidden' name='flcordservnum[]' id='flcordservnum_".$flcid."' value='".$flcordservnum."'/>
										<input type='hidden' name='tflordem[]' id='flordem_".$flcid."' value='".$tflordem."'/>		
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
					<a href="#" onclick="adicionarFases('licitacao');"><img src="/imagens/gif_inclui.gif" style="cursor:pointer;" border="0" title="Inserir Fases"> Inserir Fases de Licitação </a>
				<?php } ?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Início Programado:</td>
			<td>
				<?php
					
					$dtiniciolicitacao = !empty($_SESSION["obras"]["flcpubleditaldtprev"]) ? $_SESSION["obras"]["flcpubleditaldtprev"] : $licitacao->dtiniciolicitacao;
					echo campo_data( 'dtiniciolicitacao', 'N', 'N', '', 'S' ); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Término Programado:</td>
			<td>
				<?php
				
					$obData = new Data();
				
					/*if( $licitacao->dtfinallicitacao ){
						
						if( $obData->diferencaEntreDatas(Date("d/m/Y"), formata_data($licitacao->dtfinallicitacao), 'maiorDataBolean', 'DD/MM/YYYY', '') ){
							$msgErroData = "Término programado para o procedimento licitatório expirado.
											Favor registrar motivo do atraso em <b>Restrições e Providências</b>.";	
						}
						
					}*/
					
					$dtfinallicitacao = $licitacao->dtfinallicitacao;
					echo campo_data( 'dtfinallicitacao', 'S', $somenteLeitura, '', 'S' );
					
				?>
			<!--<span style="color: #ee0000;"><?php //print $msgErroData; ?></span>-->
			</td>
		</tr>
		<tr id="tr_uasg" style="display: none;">
			<td class="subtitulodireita">Número da UASG:</td>
			<td>
				<?php
					$licitacaouasg = $licitacao->licitacaouasg; 
					echo campo_texto( 'licitacaouasg', 'N', $somenteLeitura, '', 12, 10, '##########', '', 'left', '', 0, 'id="licitacaouasg"' ); 
				?>
			</td>
		</tr>
		<tr id="tr_numlic" style="display: none;">
			<td class="subtitulodireita">Número da Licitação:</td>
			<td>
				<?php 
					$numlicitacao = $licitacao->numlicitacao;
					echo campo_texto( 'numlicitacao', 'N', $somenteLeitura, '', 12, 10, '##########', '', 'left', '', 0, 'id="numlicitacao"' ); 
				?>
			</td>
		</tr>
		<tr bgcolor="#DEDEDE">
			<td colspan="2">
				<input <?=(($habilitado)? "" :"disabled=\"disabled\""); ?> type="button" name="botao" value="Salvar" onclick="salvaLicitacao('1<?=possuiPerfil(array(PERFIL_SUPERVISORMEC, PERFIL_ADMINISTRADOR));?>');"/>
				<input type="button" name="botao" value="Voltar" onclick="history.back(-1);"/>
			</td>
		</tr>
	</table>
</form>

<script type="text/javascript">
	abreNumLicUasg(<?php print $licitacao->molid ?>);
</script>
<?php chkSituacaoObra(); ?>