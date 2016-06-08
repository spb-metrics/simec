<?php 

$atividade = atividade_pegar( $_REQUEST['atiid'] );
if ( !$atividade ) {
	redirecionar( 'principal/atividade_enem/arvore', 'A' );
}

if( $_REQUEST['download'] == 'S' )
{
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec();
	$arqid = $_REQUEST['arqid'];
    $arquivo = $file->getDownloadArquivo($arqid);
    echo"<script>window.location.href = 'enem.php?modulo=principal/atividade_enem/visualizar_evidencias&acao=A&atiid=".$_REQUEST['atiid']."';</script>";
    exit;
}

$parametros = array(
	'aba' => $_REQUEST['aba'], # mantém a aba ativada
	'atiid' => $_REQUEST['atiid']
);

$permissao = atividade_verificar_responsabilidade( $atividade['atiid'], $_SESSION['usucpf'] );
$permissao_formulario = $permissao ? 'S' : 'N'; # S habilita e N desabilita o formulário

if( temPerfilSomenteConsulta() || temPerfilExecValidCertif() )
{
	$permissao = false;
	$permissao_formulario = 'N';
}
elseif( temPerfilAdministrador() )
{
	$permissao = true;
	$permissao_formulario = 'S';
}

// ----- VERIFICA SE PROJETO ESTÁ SELECIONADO
projeto_verifica_selecionado();

// ----- CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '&atiid=' . $atividade['atiid']  );
montar_titulo_projeto( $atividade['atidescricao'] );

extract( $atividade ); # mantém o formulário preenchido

?>

<link rel="stylesheet" href="/includes/jquery-tooltip/jquery.tooltip.css" />


<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script type="text/javascript" src="/includes/jquery-tooltip/jquery.tooltip.min.js"></script>

<script type="text/javascript">

jQuery.noConflict();

jQuery(document).ready(function($)
{
	$("a").tooltip({ 
	    track: true, 
	    delay: 0, 
	    showURL: false, 
	    opacity: 1, 
	    fixPNG: true, 
	    showBody: " - ", 
	    extraClass: "pretty fancy", 
	    top: -15, 
	    left: 5 
	});
});

</script>

<table class="tabela" bgcolor="#fbfbfb" cellspacing="0" cellpadding="10" align="center">
	<tr>
		<td>
			<form method="post" name="evidencias" id="evidencias" action="" enctype="multipart/form-data">
			<input type="hidden" id="evento" name="evento" value="pesquisar_evidencias"/>
			<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="5" style="width: 100%;">
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Item de checklist</td>
					<td>
					<?php
					$iclid = $_REQUEST['iclid'];
					
					$sql = "SELECT
								iclid as codigo,
								icldsc as descricao
						    FROM
						    	projetos.itemchecklist
						    WHERE
						    	atiid = ".$atividade["atiid"]."
						    ORDER BY 
						    	icldsc";
					$db->monta_combo('iclid', $sql, 'S', 'Selecione', '', '', '', '250', 'N', 'iclid');
					?>
					</td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Usuário</td>
					<td>
					<?php
					$entid = $_REQUEST['entid'];
					
					$sql = "SELECT DISTINCT
								ent.entid as codigo,
								ent.entnome as descricao
						    FROM
						    	entidade.entidade ent
						    INNER JOIN
						    	projetos.validacao vld ON vld.entid = ent.entid
						    INNER JOIN
						    	projetos.itemchecklist icl ON icl.iclid = vld.iclid
						    WHERE
						    	icl.atiid = ".$atividade["atiid"]."
						    ORDER BY 
						    	ent.entnome";
					$db->monta_combo('entid', $sql, 'S', 'Selecione', '', '', '', '250', 'N', 'entid');
					?>
					</td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%"></td>
					<td>
						<input type="button" class="botao" value="Pesquisar" onclick="this.form.submit();">	
					</td>
				</tr>
			</table>
			</form>
			<br />
			<table align="center" class="" bgcolor="#fcfcfc" cellspacing="1" cellpadding="5" style="width: 90%;border:1px solid black;">
				<thead>
					<tr>
						<th>Item do checklist</th>
						<th>Evidências (execução)</th>
						<th>Evidências (validação)</th>
						<th>Evidências (certificação)</th>
					</tr>
				</thead>
				<tbody>
				<?php 
				
				$whItem = ( $_REQUEST['iclid'] ) ? " AND iclid = ".$_REQUEST['iclid'] : "";
				
				$sql = "SELECT iclid,icldsc FROM projetos.itemchecklist WHERE atiid = ".$atividade["atiid"].$whItem;
				$listaChecklist = $db->carregar($sql);
				
				if( $listaChecklist )
				{
					for($i=0; $i<count($listaChecklist); $i++)
					{
						$cor = ($i%2) ? "#e0e0e0" : "#f4f4f4";
						 
						echo '<tr>
								<td bgcolor="'.$cor.'">
									'.$listaChecklist[$i]['icldsc'].'
								</td>';
						
						$whValidacao = ( $_REQUEST['entid'] ) ? " AND vld.entid = ".$_REQUEST['entid'] : "";
						
						$sql = "SELECT * FROM projetos.etapascontrole WHERE iclid = ".$listaChecklist[$i]['iclid']." AND tpvid = 1";
						$etapasControleExecucao = $db->carregar($sql);
						
						if( $etapasControleExecucao )
						{
							$sql = "SELECT vld.vldid,to_char(vld.vlddata,'DD/MM/YYYY') as vlddata,vld.vldsituacao,ent.entnome FROM projetos.validacao vld INNER JOIN entidade.entidade ent ON ent.entid = vld.entid WHERE vld.iclid = ".$listaChecklist[$i]['iclid']." AND vld.tpvid = 1".$whValidacao;
							$validacaoExecucao = $db->carregar($sql);
							
							$arrArquivos = array();
							
							if( $validacaoExecucao )
							{
								for($j=0; $j<count($validacaoExecucao); $j++)
								{
									if( $validacaoExecucao[$j]['vldsituacao'] == 't' )
										$situacao = "Item validado";
									elseif( $validacaoExecucao[$j]['vldsituacao'] == 'f' )
										$situacao = "Item invalidado";
										
									$sql = "SELECT arqid FROM projetos.anexochecklist WHERE vldid = ".$validacaoExecucao[$j]['vldid']." AND ancstatus = 'A'";
									$arqid = $db->pegaUm($sql);
									
									if( $arqid )
									{
										$sql = "SELECT arqid,arqnome,arqextensao FROM public.arquivo WHERE arqid = ".$arqid." AND arqstatus = 'A'";
										$dadosArquivo = $db->carregar($sql);
										
										if( $dadosArquivo )
										{
											$titleArq = "Nome do arquivo: ".$dadosArquivo[0]['arqnome'].'.'.$dadosArquivo[0]['arqextensao']."<br />
														 Usuário: ".$validacaoExecucao[$j]['entnome']."<br />
														 Data de inclusão: ".$validacaoExecucao[$j]['vlddata']."<br />
														 Validação: ".$situacao;
											$arrArquivos[] = '<a title="'.$titleArq.'" style="cursor:pointer;color:blue;" onclick="window.location=\'?modulo=principal/atividade_enem/visualizar_evidencias&acao=A&atiid='.$_REQUEST['atiid'].'&download=S&arqid='.$dadosArquivo[0]['arqid'].'\';" />'.$dadosArquivo[0]['arqnome'].'.'.$dadosArquivo[0]['arqextensao'].'</a>';
										}
									}
								}
								
								if( count($arrArquivos) > 0 ) $arquivos = implode('<br />', $arrArquivos);
							}
							
							if( count($arrArquivos) == 0 )
							{
								//$arquivos = "Não existem evidências cadatradas.";
								$arquivos = "-";
							}
						}
						else
						{
							//$arquivos = 'Este item não possui a etapa <b>Execução</b>.';
							$arquivos = "-";
						}
						
						echo '<td bgcolor="'.$cor.'" align="center">
								'.$arquivos.'
							</td>';
						
						$sql = "SELECT * FROM projetos.etapascontrole WHERE iclid = ".$listaChecklist[$i]['iclid']." AND tpvid = 2";
						$etapasControleValidacao = $db->carregar($sql);
						
						if( $etapasControleValidacao )
						{
							$sql = "SELECT vld.vldid,to_char(vld.vlddata,'DD/MM/YYYY') as vlddata,vld.vldsituacao,ent.entnome FROM projetos.validacao vld INNER JOIN entidade.entidade ent ON ent.entid = vld.entid WHERE vld.iclid = ".$listaChecklist[$i]['iclid']." AND vld.tpvid = 2".$whValidacao;
							$validacaoValidacao = $db->carregar($sql);
							
							$arrArquivos = array();
							
							if( $validacaoValidacao )
							{
								for($j=0; $j<count($validacaoValidacao); $j++)
								{
									if( $validacaoValidacao[$j]['vldsituacao'] == 't' )
										$situacao = "Item validado";
									elseif( $validacaoValidacao[$j]['vldsituacao'] == 'f' )
										$situacao = "Item invalidado";
										
									$sql = "SELECT arqid FROM projetos.anexochecklist WHERE vldid = ".$validacaoValidacao[$j]['vldid']." AND ancstatus = 'A'";
									$arqid = $db->pegaUm($sql);
									
									if( $arqid )
									{
										$sql = "SELECT arqid,arqnome,arqextensao FROM public.arquivo WHERE arqid = ".$arqid." AND arqstatus = 'A'";
										$dadosArquivo = $db->carregar($sql);
										
										if( $dadosArquivo )
										{
											$titleArq = "Nome do arquivo: ".$dadosArquivo[0]['arqnome'].'.'.$dadosArquivo[0]['arqextensao']."<br />
														 Usuário: ".$validacaoValidacao[$j]['entnome']."<br />
														 Data de inclusão: ".$validacaoValidacao[$j]['vlddata']."<br />
														 Validação: ".$situacao;
											$arrArquivos[] = '<a title="'.$titleArq.'" style="cursor:pointer;color:blue;" onclick="window.location=\'?modulo=principal/atividade_enem/visualizar_evidencias&acao=A&atiid='.$_REQUEST['atiid'].'&download=S&arqid='.$dadosArquivo[0]['arqid'].'\';" />'.$dadosArquivo[0]['arqnome'].'.'.$dadosArquivo[0]['arqextensao'].'</a>';
										}
									}
								}
								
								if( count($arrArquivos) > 0 ) $arquivos = implode('<br />', $arrArquivos);
							}
							
							if( count($arrArquivos) == 0 )
							{
								//$arquivos = "Não existem evidências cadatradas.";
								$arquivos = "-";
							}
						}
						else
						{
							//$arquivos = 'Este item não possui a etapa <b>Validação</b>.';
							$arquivos = "-";
						}
						
						echo '<td bgcolor="'.$cor.'" align="center">
								'.$arquivos.'
							</td>';
						
						$sql = "SELECT * FROM projetos.etapascontrole WHERE iclid = ".$listaChecklist[$i]['iclid']." AND tpvid = 3";
						$etapasControleCertificacao = $db->carregar($sql);
						
						if( $etapasControleCertificacao )
						{
							$sql = "SELECT vld.vldid,to_char(vld.vlddata,'DD/MM/YYYY') as vlddata,vld.vldsituacao,ent.entnome FROM projetos.validacao vld INNER JOIN entidade.entidade ent ON ent.entid = vld.entid WHERE vld.iclid = ".$listaChecklist[$i]['iclid']." AND vld.tpvid = 3".$whValidacao;
							$validacaoCertificacao = $db->carregar($sql);
							
							$arrArquivos = array();
							
							if( $validacaoCertificacao )
							{
								for($j=0; $j<count($validacaoCertificacao); $j++)
								{
									if( $validacaoCertificacao[$j]['vldsituacao'] == 't' )
										$situacao = "Item validado";
									elseif( $validacaoCertificacao[$j]['vldsituacao'] == 'f' )
										$situacao = "Item invalidado";
										
									$sql = "SELECT arqid FROM projetos.anexochecklist WHERE vldid = ".$validacaoCertificacao[$j]['vldid']." AND ancstatus = 'A'";
									$arqid = $db->pegaUm($sql);
									
									if( $arqid )
									{
										$sql = "SELECT arqid,arqnome,arqextensao FROM public.arquivo WHERE arqid = ".$arqid." AND arqstatus = 'A'";
										$dadosArquivo = $db->carregar($sql);
										
										if( $dadosArquivo )
										{
											$titleArq = "Nome do arquivo: ".$dadosArquivo[0]['arqnome'].'.'.$dadosArquivo[0]['arqextensao']."<br />
														 Usuário: ".$validacaoCertificacao[$j]['entnome']."<br />
														 Data de inclusão: ".$validacaoCertificacao[$j]['vlddata']."<br />
														 Validação: ".$situacao;
											$arrArquivos[] = '<a title="'.$titleArq.'" style="cursor:pointer;color:blue;" onclick="window.location=\'?modulo=principal/atividade_enem/visualizar_evidencias&acao=A&atiid='.$_REQUEST['atiid'].'&download=S&arqid='.$dadosArquivo[0]['arqid'].'\';" />'.$dadosArquivo[0]['arqnome'].'.'.$dadosArquivo[0]['arqextensao'].'</a>';
										}
									}
								}
								
								if( count($arrArquivos) > 0 ) $arquivos = implode('<br />', $arrArquivos);
							}
							
							if( count($arrArquivos) == 0 )
							{
								//$arquivos = "Não existem evidências cadatradas.";
								$arquivos = "-";
							}
						}
						else
						{
							//$arquivos = 'Este item não possui a etapa <b>Certificação</b>.';
							$arquivos = "-";
						}
						
						echo '<td bgcolor="'.$cor.'" align="center">
								'.$arquivos.'
							</td>';
					}
				}
				else
				{
					echo '<tr><td bgcolor="#f4f4f4" style="color:red;text-align:center;" colspan="4">Não existe item(ns) de checklist cadastrado(s).</td></tr>';
				}
				?>
				</tbody>
			</table>
		</td>
	</tr>
</table>