<?php
// Inclusão do arquivo de permissões (somente no módulo de obras)
if ($_SESSION["sisid"] == ID_OBRAS){
	require_once APPRAIZ . "www/obras/permissoes.php";
}

// Inclusão de arquivos do componente de Entidade 
require_once APPRAIZ . "adodb/adodb.inc.php";

// Pega o caminho atual do usuário (em qual módulo se encontra)
$caminho_atual = $_SERVER["REQUEST_URI"];
$posicao_caminho = strpos($caminho_atual, 'acao');
$caminho_atual = substr($caminho_atual, 0 , $posicao_caminho);
?>
<html>
	<head>
		<title>Extrato da Obra</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<script type="text/javascript">	
		
		function salvar(origem,idvistoria){

			if (origem=="galeria"){
				var objForm = document.getElementsByName("galeria[]"); 
			    ids="0";
			    for (var i=0; i < objForm.length; i++)
			    {
			    	if (objForm[i].checked)
			    		ids+=","+objForm[i].id;
				}
				window.opener.document.getElementById("selecao_fotos_galeria").value=ids;
			}

			if (origem=="vistoria"){
				var objForm = document.getElementsByName("vistoria[]"); 
			    ids="0";
			    for (var i=0; i < objForm.length; i++)
			    {
			    	if (objForm[i].checked)
			    		ids+=","+objForm[i].id;
				}
				window.opener.document.getElementById("selecao_fotos_vistoria_"+idvistoria).value=ids;
			}
		}
	</script>
	<?php if($_REQUEST["selecionado"]=="galeria"){ ?>
		<body onUnload="salvar('<?php echo $_REQUEST["selecionado"];?>','0');">
	<?php } else { ?>
		<body onUnload="salvar('<?php echo substr($_REQUEST["selecionado"],0,8);?>','<?php echo substr($_REQUEST["selecionado"],9,strlen($_REQUEST["selecionado"]));?>');">
	<?php }  ?>
		<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none; width:100%;">
			<tr>
				<td width="100%" align="center">
					<label class="TituloTela" style="color: #000000;"> 
						Selecionar Fotos
					</label>
				</td>
			</tr>
		</table>
		<br/>
		<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<tr bgcolor="#cdcdcd">
				<td colspan="2" valign="top">
					<strong>Selecione a(s) foto(s)</strong>
				</td>
			</tr>
			<tr bgcolor=#e0e0e0>
				<td>
				<?
				// galeria de fotos
				if($_REQUEST["selecionado"]=="galeria"){
					$sql = "SELECT arq.arqid, arq.arqdescricao FROM public.arquivo arq
							INNER JOIN obras.arquivosobra oar ON arq.arqid = oar.arqid
							INNER JOIN obras.obrainfraestrutura obr ON obr.obrid = oar.obrid 
							INNER JOIN seguranca.usuario seg ON seg.usucpf = oar.usucpf 
							WHERE obr.obrid = {$_SESSION["obra"]["obrid"]} AND
								  aqostatus = 'A' AND
								  (arqtipo = 'image/jpeg' OR 
								   arqtipo = 'image/gif' OR 
								   arqtipo = 'image/png') 
							ORDER BY arq.arqid";
					
					$fotos = ($db->carregar($sql));
					if($fotos){
							$_SESSION['imgparams'] = array("filtro" => "cnt.obrid=".$_SESSION["obra"]["obrid"]." AND aqostatus = 'A'", "tabela" => "obras.arquivosobra");
							echo "<table>";
							for($i=0;$i < count($fotos);$i++){
								$marcado=false;
								if ($fotos[$i]["arqid"]){
									foreach(explode(",",$_REQUEST["marcadas"]) as $valor)
									{ if ($fotos[$i]["arqid"]== $valor) $marcado=true;	}
									echo "<tr>
											<td>"; 
										echo "<img src='../slideshow/slideshow/verimagem.php?newwidth=100&newheight=100&arqid=".$fotos[$i]["arqid"]."' hspace='3' vspace='3' style='width:100px; height:100px; '\n>";										
										echo "<br><input ";
										if ($marcado) echo " checked=checked ";		
											echo " type=checkbox name=galeria[] id=".$fotos[$i]["arqid"]." \">"."
											".$fotos[$i]["arqdescricao"]."
											</td>";
									}
								$i=$i+1;
								$marcado=false;	
								if ($fotos[$i]["arqid"]){
									foreach(explode(",",$_REQUEST["marcadas"]) as $valor)
									{ if ($fotos[$i]["arqid"]== $valor) $marcado=true;	}
									echo "
											<td>"; 
										echo "<img src='../slideshow/slideshow/verimagem.php?newwidth=100&newheight=100&arqid=".$fotos[$i]["arqid"]."' hspace='3' vspace='3' style='width:100px; height:100px; '\n>";										
										echo "<br><input ";
										if ($marcado) echo " checked=checked ";		
											echo " type=checkbox name=galeria[] id=".$fotos[$i]["arqid"]." \">"."
											".$fotos[$i]["arqdescricao"]."
											</td>";
									}
								$i=$i+1;
								$marcado=false;
								if ($fotos[$i]["arqid"]){
									foreach(explode(",",$_REQUEST["marcadas"]) as $valor)
									{ if ($fotos[$i]["arqid"]== $valor) $marcado=true;	}
									echo "
										<td>"; 
										echo "<img src='../slideshow/slideshow/verimagem.php?newwidth=100&newheight=100&arqid=".$fotos[$i]["arqid"]."' hspace='3' vspace='3' style='width:100px; height:100px; '\n>";										
										echo "<br><input ";
										if ($marcado) echo " checked=checked ";		
											echo " type=checkbox name=galeria[] id=".$fotos[$i]["arqid"]." \">"."
											".$fotos[$i]["arqdescricao"]."
											</td></tr>";
									}
								}
								echo "</table>";
						} else {
							echo "Não existem fotos cadastradas";
						}			
					}
					#### fotos das vistorias ####
					$selecionado=substr($_REQUEST["selecionado"],0,8);
					$supvid=substr($_REQUEST["selecionado"],9,strlen($_REQUEST["selecionado"]));
					if($selecionado=="vistoria"){
							// FOTOS da vistoria
								$sql = "SELECT fot.*, arq.arqdescricao FROM obras.fotos AS fot
										LEFT JOIN public.arquivo AS arq ON arq.arqid = fot.arqid
										WHERE obrid =".$_SESSION["obra"]["obrid"]." AND supvid=".$supvid." 
										ORDER BY fotordem ";
								$fotos = ($db->carregar($sql));
								if($fotos){
									$_SESSION['imgparams'] = array("filtro" => "cnt.obrid=".$_SESSION["obra"]["obrid"]." AND aqostatus = 'A'", "tabela" => "obras.arquivosobra");
									echo "<table>";
									for($i=0;$i < count($fotos);$i++){
										$marcado=false;
										if ($fotos[$i]["arqid"]){
											foreach(explode(",",$_REQUEST["marcadas"]) as $valor)
											{ if ($fotos[$i]["arqid"]== $valor) $marcado=true;	}
											echo "<tr>
													<td>"; 
												echo "<img src='../slideshow/slideshow/verimagem.php?newwidth=100&newheight=100&arqid=".$fotos[$i]["arqid"]."' hspace='3' vspace='3' style='width:100px; height:100px; '\n>";										
												echo "<br><input ";
												if ($marcado) echo " checked=checked ";		
													echo " type=checkbox name=vistoria[] id=".$fotos[$i]["arqid"]." \">"."
													".$fotos[$i]["arqdescricao"]."
													</td>";
											}
										$i=$i+1;
										$marcado=false;	
										if ($fotos[$i]["arqid"]){
											foreach(explode(",",$_REQUEST["marcadas"]) as $valor)
											{ if ($fotos[$i]["arqid"]== $valor) $marcado=true;	}
											echo "
													<td>"; 
												echo "<img src='../slideshow/slideshow/verimagem.php?newwidth=100&newheight=100&arqid=".$fotos[$i]["arqid"]."' hspace='3' vspace='3' style='width:100px; height:100px; '\n>";										
												echo "<br><input ";
												if ($marcado) echo " checked=checked ";		
													echo " type=checkbox name=vistoria[] id=".$fotos[$i]["arqid"]." \">"."
													".$fotos[$i]["arqdescricao"]."
													</td>";
											}
										$i=$i+1;
										$marcado=false;
										if ($fotos[$i]["arqid"]){
											foreach(explode(",",$_REQUEST["marcadas"]) as $valor)
											{ if ($fotos[$i]["arqid"]== $valor) $marcado=true;	}
											echo "
												<td>"; 
												echo "<img src='../slideshow/slideshow/verimagem.php?newwidth=100&newheight=100&arqid=".$fotos[$i]["arqid"]."' hspace='3' vspace='3' style='width:100px; height:100px; '\n>";										
												echo "<br><input ";
												if ($marcado) echo " checked=checked ";		
													echo " type=checkbox name=vistoria[] id=".$fotos[$i]["arqid"]." \">"."
													".$fotos[$i]["arqdescricao"]."
													</td></tr>";
											}
										}
										echo "</table>";
								} else {
									echo "Não existem fotos cadastradas";
								}
							}	?>
				</td>
			</tr>
			<tr><td> <input type=button value="Fechar" onClick='javascript: window.close();'></td></tr>
		</table>
	</body>
</html>