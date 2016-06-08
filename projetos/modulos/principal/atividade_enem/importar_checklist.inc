<?php

function importarcsv($dados) {
	global $db;
	include_once APPRAIZ . 'includes/workflow.php';
	
	$ar = file($_FILES['arquivo']['tmp_name']);
	
	if(count($ar) > 1) {
		foreach($ar as $key => $dados) {
			if($key) {
				
				$dadosimp = explode(";", $dados);
				
				if($dadosimp[0]) {
					
					$atividadeimp = $db->pegaLinha("SELECT * FROM projetos.atividade WHERE _atinumero = '".$dadosimp[0]."' AND atistatus='A' AND _atiprojeto='".$_SESSION['projeto']."'");
					
					if(!$atividadeimp) {

						$db->rollback();
								
						die("<script>
								alert('Linha ".($key+1).": Atividade não esta cadastrado');
								window.location='enem.php?modulo=principal/atividade_enem/importar_checklist&acao=A';
							 </script>");
					}

					if(strlen(trim($dadosimp[3]))!=10) {

						$db->rollback();
								
						die("<script>
								alert('Linha ".($key+1).": Data deve estar no padrão dd/mm/YYYY');
								window.location='enem.php?modulo=principal/atividade_enem/importar_checklist&acao=A';
							 </script>");
						
					}
										
					$iclordem = $db->pegaUm("SELECT max(iclordem) FROM projetos.itemchecklist WHERE atiid = ".$atividadeimp['atiid']);
					$iclordem = ($iclordem) ? ((integer)$iclordem + 1) : 1;
					
					$sql = "INSERT INTO projetos.itemchecklist(icldsc, atiid, iclprazo, iclcritico, iclordem)
							VALUES ('".addslashes($dadosimp[1])."', ".$atividadeimp['atiid'].", '".formata_data_sql(trim($dadosimp[3]))."', ".(($dadosimp[10]=="S")?"TRUE":"FALSE").", ".$iclordem.")
							RETURNING iclid";
					
					$iclid = $db->pegaUm($sql);
					
					$tpdid 	= TPDID_ENEM;
					$docdsc = "<p>".$iclid." - ".$dadosimp[1]."</p><p>".$atividadeimp['_atinumero']." - ".$atividadeimp['atidescricao']."</p>";
					$docid = wf_cadastrarDocumento( $tpdid, $docdsc );
					
					$db->executar("UPDATE projetos.itemchecklist SET docid='".$docid."' WHERE iclid='".$iclid."'");
					
					
					if($dadosimp[4]) {
						
						$sql = "INSERT INTO projetos.etapascontrole (tpvid,iclid,etcopcaoevidencia,etcevidencia)
								VALUES (1, ".$iclid.", ".(($dadosimp[5])?"TRUE":"FALSE").", ".(($dadosimp[5])?"'".$dadosimp[5]."'":"NULL").")";
						$db->executar($sql);
						
						$sql = "SELECT entid FROM entidade.entidade WHERE entnumcpfcnpj='".$dadosimp[4]."'";
						$entid = $db->pegaUm($sql);
						
						if(!$entid) {
							$sql = "SELECT * FROM seguranca.usuario WHERE usucpf='".$dadosimp[4]."'";
							$usuario = $db->pegaLinha($sql);
							
							if($usuario) {
								
								$sql = "INSERT INTO entidade.entidade(
									            entnumcpfcnpj, 
									            entnome, 
									            entemail, 
									            entstatus, 
									            entsexo, 
									            entnumdddcomercial, 
									            entnumcomercial, 
									            entdatainclusao)
									    VALUES ('".$usuario['usucpf']."', 
									    		'".$usuario['usunome']."', 
									    		'".$usuario['usuemail']."', 
									    		'A', 
									    		'".$usuario['ususexo']."', 
									    		'".$usuario['usufoneddd']."', 
									            '".str_replace("-","",$usuario['usufonenum'])."', 
									            NOW()) RETURNING entid;";
								
								$entid = $db->pegaUm($sql);
							}
						}
						
						if($entid) {
							$sql = "SELECT fueid FROM entidade.funcaoentidade WHERE entid='".$entid."' AND funid='".FUNID_EXECUTOR_ENEM."'";
							$fueid = $db->pegaUm($sql);
							if(!$fueid && $entid) {
								
								$sql = "INSERT INTO entidade.funcaoentidade(
									            funid, entid, fuedata, fuestatus)
									    VALUES ('".FUNID_EXECUTOR_ENEM."', '".$entid."', NOW(), 'A');";
								
								$db->executar($sql);
								
							}
							
							$sql = "INSERT INTO projetos.checklistentidade(iclid,entid,tpvid) VALUES(".$iclid.",".$entid.",1)";
							$db->executar($sql);
							
						} else {
							
							$db->rollback();
							
							die("<script>
									alert('Linha ".($key+1).": Executor não esta cadastrado');
									window.location='enem.php?modulo=principal/atividade_enem/importar_checklist&acao=A';
								 </script>");
							
						}
					}
					
					if($dadosimp[6]) {
						
						$sql = "INSERT INTO projetos.etapascontrole (tpvid,iclid,etcopcaoevidencia,etcevidencia)
								VALUES (2, ".$iclid.", ".((trim($dadosimp[7]))?"TRUE":"FALSE").", ".(($dadosimp[7])?"'".$dadosimp[7]."'":"NULL").")";
						$db->executar($sql);
						
						$sql = "SELECT entid FROM entidade.entidade WHERE entnumcpfcnpj='".trim($dadosimp[6])."'";
						$entid = $db->pegaUm($sql);
						
						if(!$entid) {
							$sql = "SELECT * FROM seguranca.usuario WHERE usucpf='".trim($dadosimp[6])."'";
							$usuario = $db->pegaLinha($sql);
							
							if($usuario) {
								
								$sql = "INSERT INTO entidade.entidade(
									            entnumcpfcnpj, 
									            entnome, 
									            entemail, 
									            entstatus, 
									            entsexo, 
									            entnumdddcomercial, 
									            entnumcomercial, 
									            entdatainclusao)
									    VALUES ('".$usuario['usucpf']."', 
									    		'".$usuario['usunome']."', 
									    		'".$usuario['usuemail']."', 
									    		'A', 
									    		'".$usuario['ususexo']."', 
									    		'".$usuario['usufoneddd']."', 
									            '".str_replace("-","",$usuario['usufonenum'])."', 
									            NOW()) RETURNING entid;";
								
								$entid = $db->pegaUm($sql);
							}
						}
						
						if($entid) {
							$sql = "SELECT fueid FROM entidade.funcaoentidade WHERE entid='".$entid."' AND funid='".FUNID_VALIDADOR_ENEM."'";
							$fueid = $db->pegaUm($sql);
							if(!$fueid && $entid) {
								
								$sql = "INSERT INTO entidade.funcaoentidade(
									            funid, entid, fuedata, fuestatus)
									    VALUES ('".FUNID_VALIDADOR_ENEM."', '".$entid."', NOW(), 'A');";
								
								$db->executar($sql);
								
							}
							$sql = "INSERT INTO projetos.checklistentidade(iclid,entid,tpvid) VALUES(".$iclid.",".$entid.",2)";
							$db->executar($sql);
						} else {
							
							$db->rollback();
							
							die("<script>
									alert('Linha ".($key+1).": Avaliador não esta cadastrado');
									window.location='enem.php?modulo=principal/atividade_enem/importar_checklist&acao=A';
								 </script>");
							
						}
					}
					
					if($dadosimp[8]) {
						
						$sql = "INSERT INTO projetos.etapascontrole (tpvid,iclid,etcopcaoevidencia,etcevidencia)
								VALUES (3, ".$iclid.", ".(($dadosimp[9])?"TRUE":"FALSE").", ".(($dadosimp[9])?"'".$dadosimp[9]."'":"NULL").")";
						$db->executar($sql);
						
						$sql = "SELECT entid FROM entidade.entidade WHERE entnumcpfcnpj='".$dadosimp[8]."'";
						$entid = $db->pegaUm($sql);
						
						if(!$entid) {
							
							$sql = "SELECT * FROM seguranca.usuario WHERE usucpf='".$dadosimp[8]."'";
							$usuario = $db->pegaLinha($sql);
							
							if($usuario) {
								
								$sql = "INSERT INTO entidade.entidade(
									            entnumcpfcnpj, 
									            entnome, 
									            entemail, 
									            entstatus, 
									            entsexo, 
									            entnumdddcomercial, 
									            entnumcomercial, 
									            entdatainclusao)
									    VALUES ('".$usuario['usucpf']."', 
									    		'".$usuario['usunome']."', 
									    		'".$usuario['usuemail']."', 
									    		'A', 
									    		'".$usuario['ususexo']."', 
									    		'".$usuario['usufoneddd']."', 
									            '".str_replace("-","",$usuario['usufonenum'])."', 
									            NOW()) RETURNING entid;";
								
								$entid = $db->pegaUm($sql);
							}
						}
						
						if($entid) {
							$sql = "SELECT fueid FROM entidade.funcaoentidade WHERE entid='".$entid."' AND funid='".FUNID_CERTIFICADOR_ENEM."'";
							$fueid = $db->pegaUm($sql);
							if(!$fueid && $entid) {
								
								$sql = "INSERT INTO entidade.funcaoentidade(
									            funid, entid, fuedata, fuestatus)
									    VALUES ('".FUNID_CERTIFICADOR_ENEM."', '".$entid."', NOW(), 'A');";
								
								$db->executar($sql);
								
							}
							$sql = "INSERT INTO projetos.checklistentidade(iclid,entid,tpvid) VALUES(".$iclid.",".$entid.",3)";
							$db->executar($sql);
							
						} else {
							
							$db->rollback();
							
							die("<script>
									alert('Linha ".($key+1).": Certificador não esta cadastrado');
									window.location='enem.php?modulo=principal/atividade_enem/importar_checklist&acao=A';
								 </script>");
							
							
						}
					}
					
					
				}
				
			}
		}
	}
	
	$db->commit();
	
	die("<script>
			alert('Carga efetuada com sucesso');
			window.location='enem.php?modulo=principal/atividade_enem/importar_checklist&acao=A';
		 </script>");
}


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}


// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$titulo = "Importar Item checklist";

monta_titulo( $titulo, '&nbsp;');

?>
<script>
function importaritemchecklist() {
	document.getElementById('checklist').submit();
}
</script>
<form method="post" name="checklist" id="checklist" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="importarcsv">
<table class="tabela" bgcolor="#fbfbfb" cellspacing="0" cellpadding="3" align="center">
	<tr>
		<td class="SubTituloDireita">Arquivo CSV:</td>
		<td><input type="file" name="arquivo"></td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2"><input type="button" name="btnimportarcsv" value="Importar" onclick="importaritemchecklist();"></td>
	</tr>
	<tr>
		<td colspan="2">
		<p><b>Instruções de preenchimento</b></p>
		
		<p>Deverá ser submetido um arquivo csv com a seguinte estrutura:</p>
		<p>Código da atividade;Descrição do item;Data de início;Data de fim;CPF executor;Evidência execução;CPF validador;Evidência validação;CPF certificador;Evidência certificador;Item crítico</p>
		<p>
		<table>
		<tr>
			<td class="SubTituloDireita">1. Código da atividade:</td>
			<td>Número referente a atiid, este número pode ser visualizado na url ao clicar na atividade.</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">2. Descrição do item:</td>
			<td>Descrição do item</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">3. Data de início:</td>
			<td>Data de início do item no formato (dd/mm/YYYY)</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">4. Data de fim:</td>
			<td>Data de fim do item no formato (dd/mm/YYYY). Esta data será o prazo do item.</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">5. CPF executor:</td>
			<td>Se tiver execução, deverá inserir o CPF do executor. Caso não tenha execução, deixar o CPF em branco.</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">6. Evidência execução:</td>
			<td>Se não tiver evidência na execução, deixar em branco.</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">7. CPF validador:</td>
			<td>Se tiver validação, deverá inserir o CPF do validador. Caso não tenha validação, deixar o CPF em branco.</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">8. Evidência validação:</td>
			<td>Se não tiver evidência na validação, deixar em branco.</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">9. CPF certificador:</td>
			<td>Se tiver validação, deverá inserir o CPF do validador. Caso não tenha validação, deixar o CPF em branco.</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">10. Evidência certificação:</td>
			<td>Se não tiver evidência na certificação, deixar em branco.</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">11. Item crítico:</td>
			<td>Se o item for crítico crítico preencher S, senão N</td>
		</tr>
		
		</table>
		</p>
		<p>
		<b>Exemplo de arquivo CSV</b><br/><br/>
		Número da atividade;Descrição do item;Data de início;Data de fim;CPF executor;Evidência execução;CPF validador;Evidência validação;CPF certificador;Evidência certificador;Item crítico<br/>
		1.1.1.1;Item de teste do Alexandre;06/06/2011;20/06/2011;91112796134;ev. execução;70183040163;ev. validação;;;S
		</p>

		</td>
	</tr>
</table>
</form>