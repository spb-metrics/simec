<?php

class DeclaracaoController extends Controller
{
	private $obModel;
	
	public static function lista( Array $param = null )
	{
		if( !empty($param) && is_array($param) )
		{
			extract($param);
		}
		
		/*
		 * INÍCIO - MODEL
		 * definição de parametros a serem passados para o modelo
		 */				    
		// COLUNAs a serem retornadas da query
		$arParam['coluna'] = array(
									"CASE WHEN dcl.dclid is not null THEN '<img src=\"../imagens/alterar.gif\" title=\"Alterar a Declaração\" style=\"cursor:pointer;\" onclick=\"alterarDeclaracao('||dcl.dclid||');\">&nbsp;<img src=\"../imagens/excluir.gif\" title=\"Cancelar a Declaração\" style=\"cursor:pointer;\" onclick=\"cancelarDeclaracao('||dcl.dclid||');\">' ELSE '<img src=\"../imagens/alterar_01.gif\" style=\"cursor:pointer;\">&nbsp;<img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\">' END AS cancelar",
									"gpd.gpdid AS num_grupo",
									"CASE WHEN dcl.dclid is not null THEN '<img src=\"../imagens/ico_html.gif\" width=\"19\" height=\"19\" title=\"Visualizar Declaração\" style=\"cursor:pointer; margin-left:3px;\" onclick=\"visualizarDeclaracao('||dcl.dclid||');\">' ELSE '<img src=\"../imagens/ico_html_gray.gif\" width=\"19\" height=\"19\" title=\"Declaração Inexistente\" style=\"cursor:pointer; margin-left:3px;\">' END AS html",
									"CASE WHEN dcl.dclid is not null THEN ''||dcl.dclid ELSE 'Declaração inexistente' END AS num_declaracao",
									"CASE WHEN dcl.dclid is not null THEN '<img border=\"0\" onclick=\"envia_email_declaracao_empresa('||dcl.dclid||');\" title=\"Enviar Declaração por e-mail para Empresa.\" src=\"../imagens/email.gif\" style=\"cursor: pointer;\"/>' ELSE '-' END AS enviar_email",
									"CASE WHEN dcl.orsid is not null THEN ''||dcl.orsid ELSE '-' END AS os",
									"CASE WHEN dcl.dcldtemissao is not null THEN to_char(dcl.dcldtemissao, 'DD/MM/YYYY' ) ELSE '-' END AS data_emissao",
									"ent.entnome AS empresa",
									"gpd.estuf AS uf",
									"CASE WHEN dcl.dclvalor is not null THEN ''||dcl.dclvalor ELSE '-' END AS valor",
									"CASE WHEN dcl.dclid is not null THEN std.stddsc ELSE '-' END AS situacao",
									"CASE WHEN dcl.dclordembanc is not null THEN ''||dcl.dclordembanc ELSE '-' END AS ordem_bancaria",
									"CASE WHEN usu.usunome is not null THEN usu.usunome ELSE '-' END AS criado_por",
									"dcl.dclid"
								  );
								  
		$param['filtro']		  = is_array( $param['filtro'] ) ? $param['filtro'] : $param;						  
		//$param['filtro']['stdid'] = $param['filtro']['stdid'] ? $param['filtro']['stdid'] : Declaracao::SITUACAO_DECLARACAO_GERADA;
								  
		// FILTRO da query (WHERE)						  
		$arParam['filtro'] = $param['filtro'];			  
		$obModel 		   = new Declaracao();
		//$arDados 		   = $obModel->listaOS( $arParam );
		$sql 		       = $obModel->lista( $arParam, 'string' );
		/*
		 * FIM - MODEL
		 * termina buscando uma lista de dados provindos do modelo de "Declaracao"
		 */				    
		
		/*
		 * INÍCIO - VIEW
		 * definição de parametros para serem passados para definição da VIEW, no caso, "Lista"
		 */				    
		// CABEÇALHO da lista
		$arCabecalho = array(
								"Alterar/Cancelar",
								"N° Grupo",
								"HTML",
								"N° Declaracao Vigente",
								"Enviar Email",
								"N° da OS",
								"Data Emissão",
								"Empresa",
								"UF",
								"Valor R$",
								"Situação",
								"N° Ordem Bancária",
								"Criado Por"
						    );
						    
		// AÇÃO que será posta na primeira coluna de todas as linhas
		/*$acao = "<center>
				   <img src='../imagens/alterar.gif' title='Editar OS' style='cursor:pointer; margin-left:3px' onclick='location.href=\"?modulo=principal/supervisao/cadOS&acao=A&orsid={orsid}\"'>" . 
					(possuiPerfil( array(PERFIL_SAA, PERFIL_ADMINISTRADOR, PERFIL_SUPERUSUARIO) ) ?				   
						"<img src='../imagens/excluir.gif' title='Excluir OS' style='cursor:pointer; margin-left:3px' onclick='excluir({orsid}, this);'>"
																								  :		
						"<img src='../imagens/excluir_01.gif' title='Sem permissão' style='cursor:pointer; margin-left:3px'>") .
				  "<center>";*/
		
		// parametros que cofiguram as colunas da lista, a ordem do array equivale a ordem do cabeçalho
		$arParamCol[0] = array("type"  => "string",
							   "align" => "center");
		 
		$arParamCol[1] = array("type"  => "string", 
							   "align" => "center", 
							   "html"  => "<img border=\"0\" id=\"img_{num_grupo}\" onclick=\"visualizarDeclaracoes({num_grupo});\" title=\"Visualizar as declarações não vigentes.\" src=\"../imagens/mais.gif\" style=\"cursor:pointer;\"/>
							   			   {num_grupo}");
		
		$arParamCol[2] = array("type"  => "string",
							   "align" => "center");
		
		$arParamCol[3] = array("type"  => "string", 
							   "style" => "color:#0066CC;",
							   "align" => "center");
		
		$arParamCol[4] = array("type"  => "string",
							   "align" => "center");

		$arParamCol[5] = array("type"  => "string",
							   "align" => "center");
		
		$arParamCol[6] = array("type" => "date");
		
		$arParamCol[7] = array("type"  => "string",
							   "align" => "left");
		
		$arParamCol[8] = array("type"  => "string", 
							   "align" => "center");
		
		$arParamCol[9] = array("type" => "numeric");
		
		$arParamCol[10] = array("type"  => "string", 
							   "align" => "center");
		
		$arParamCol[11] = array("type" => "numeric");
		
		$arParamCol[12] = array("type"  => "string", 
							   "align" => "center", 
							   "html"  => "{criado_por}
							   			   </td></tr>
							   			   <tr id=\"linha_{num_grupo}\" style=\"display:none;\"><td align=\"center\" colspan=\"8\">");
		
		// ARRAY de parametros de configuração da tabela
		$arConfig = array("style" => "width:100%;");
		
		/*$a = new Lista($arConfig);
		$a->setCabecalho( $arCabecalho );
		$a->setCorpo( $arDados, $arParamCol );
		$a->setAcao( $acao );
		$a->show();*/
		
		$oPaginacaoAjax = new PaginacaoAjax();
		$oPaginacaoAjax->setNrPaginaAtual($nrPaginaAtual);
		$oPaginacaoAjax->setNrRegPorPagina($nrRegPorPagina);
		$oPaginacaoAjax->setNrBlocoPaginacaoMaximo($nrBlocoPaginacaoMaximo);
		$oPaginacaoAjax->setNrBlocoAtual($nrBlocoAtual);
		$oPaginacaoAjax->setDiv( 'listaDeclaracao' );
		$oPaginacaoAjax->setCabecalho( $arCabecalho );
		$oPaginacaoAjax->setSql( $sql );
		$oPaginacaoAjax->setAcao( $acao );
		$oPaginacaoAjax->setParamCol( $arParamCol );
		$oPaginacaoAjax->setConfig( $arConfig );
		$oPaginacaoAjax->show();
		
		
		/*
		 * FIM - VIEW
		 * o método show() renderiza os parametros, cuspindo na tela a lista.
		 */	
	}
	
	function ativaDadosDeclaracao(Array $arAtt = null, $dclid = null)
	{
		$dclid = $dclid ? $dclid : $this->dclid;
		
		$this->carregaDadosModel( new Declaracao( $dclid ), $arAtt );
	}
	
	function ativaDadosArquivo(Array $arAtt = null, $arqid = null)
	{
		$arqid = $arqid ? $arqid : $this->arqid;
		$this->carregaDadosModel( new Arquivo( $arqid ), $arAtt );
	}
	
	function salvarDeclaracao($gpdid = null)
	{
		$gpdid = ( is_null($gpdid) ) ? $_POST['gpdid'] : $gpdid;
		
		$obModel = new Declaracao();
		
		// campos que teram valores no insert
		$arCampos = array(
							"usucpf",
							"arqid",
							"gpdid",
							"stdid",
							"dclvalor",
							"dclstatus",
							"dcldtemissao",
							"orsid"
						 );
		
		$post					= array();
		$post['gpdid'] 			= $gpdid;
		$post['dcldtemissao'] 	= date('Y-m-d H:i:s');
		
		$ob 				= new Modelo();
		$post['orsid'] 		= $ob->pegaUm("SELECT orsid FROM obras.ordemservico WHERE gpdid = ".$gpdid." AND orsstatus = 'A' AND stoid = ".OrdemServico::SITUACAOOS_GERADA);
		$post['dclvalor'] 	= $ob->pegaUm("SELECT orsvalor FROM obras.ordemservico WHERE gpdid = ".$gpdid." AND orsstatus = 'A' AND stoid = ".OrdemServico::SITUACAOOS_GERADA);
		
		$obModel->popularObjeto($arCampos, $post);		
		$dclid = $obModel->salvar();
		$obModel->commit();
		
		return $dclid;
	}
	
	function ativaDadosGrupo(Array $arAtt = null, $gpdid = null)
	{
		$gpdid = $gpdid ? $gpdid : $this->gpdid;
		
		$this->carregaDadosModel( new GrupoDistribuicao( $gpdid ), $arAtt );
	}
	
	function carregaDadosEmpresa()
	{
		if( $this->epcid )
		{
			$obModelEmpresa =  new EmpresaContratada();
			$arDados    	= $obModelEmpresa->buscaDadosEmpresa( $this->epcid );
			$this->carregaDados($arDados, array("empresa", "cnpj", "epcnumproceconc", "epcnumcontrato"));
		}
	}
	
	static function gerarArquivoHTML( $dclid )
	{
		// Ativa o buffer de saída
		ob_start();
		// Limpa o buffer de saída
		ob_clean();
		
		// O arquivo da Declaração só pode ser gerado uma vez.
		$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($dclid/1000) .'/';
		$name    = 'Declaracao_'. $dclid . '.html';
		if (file_exists($caminho . $name)) {
			unlink( $caminho . $name );
		}
		
		$obDeclaracao = new DeclaracaoController();
		// ativa no controller o modelo "Declaracao" e com isso seus atributos
		$obDeclaracao->ativaDadosDeclaracao( null, $dclid );
		// ativa no controller o modelo "GrupoDistribuicao" e com isso seus atributos
		$obDeclaracao->ativaDadosGrupo();
		// carrega o resultado da pesquisa no controller
		$obDeclaracao->carregaDadosEmpresa();
		
		// carrega os dados do workflow
		$docid = (integer) $obDeclaracao->docid;
		$documento = wf_pegarDocumento( $docid );
		$atual = wf_pegarEstadoAtual( $docid );
		$historico = wf_pegarHistorico( $docid );
		
		?>
		<html>
			<head>
				<title><?php echo $GLOBALS['parametros_sistema_tela']['nome_e_orgao'];?></title>
				<script language="JavaScript" src="../../includes/funcoes.js"></script>
				<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
				<script src="../includes/calendario.js"></script>
				<script src="../obras/js/obras.js"></script>
				<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
				<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
				<script type="text/javascript">
				<!--
					IE = !!document.all;
					
					function exebirOcultarComentario( docid )
					{
						id = 'comentario' + docid;
						div = document.getElementById( id );
						if ( !div )
						{
							return;
						}
						var display = div.style.display != 'none' ? 'none' : 'table-row';
						if ( display == 'table-row' && IE == true )
						{
							display = 'block';
						}
						div.style.display = display;
					}
					
					$(document).ready(function()
					{
						
					});
				//-->
				</script>
			</head>
			<body>
				<a class="notprint" style="cursor:pointer; float:right; margin-top: 50px; margin-right: 20px;" onclick="window.print();"><img src="../imagens/ico_print.jpg" border="0"></a>
				<table class="tabela" bgcolor="#ffffff" cellSpacing="1" cellPadding=3 align="center">			
					<tr>
						<td class="SubTituloCentro" style="height:50px;">Declaração n° <?=$obDeclaracao->dclid?></td>
					</tr>
					<tr>	
						<td style='border-collapse: collapse; border: 1px solid #ccc;'>
							<br />
							Declaro para os devidos fins que a empresa <b><?=$obDeclaracao->empresa?></b>, por mim representada, realizou o serviço de monitoramento e supervisão das obras referentes à <b>Declaração n° <?=$obDeclaracao->dclid?></b>, grupo <b><?=$obDeclaracao->gpdid?></b>, no valor de <b>R$ <?=$obDeclaracao->dclvalor?></b>, que teve a seguinte tramitação:
							<br /><br />
						</td>
					</tr>
					<tr>
						<td class="SubTituloCentro" style="height:50px;">Fluxo de Tramitação</td>
					</tr>
					<tr>
						<td style='border-collapse: collapse; border: 1px solid #ccc;'>
							<br />
							<table class="listagem" cellspacing="0" cellpadding="3" align="center" style="width: 650px;border-top:0px;">
								<thead>
									<tr>
										<td style="text-align: center; background-color: #e0e0e0;" colspan="6">
											<b style="font-size: 10pt;">Histórico de Tramitações<br/></b>
											<div><?php echo $documento['docdsc']; ?></div>
										</td>
									</tr>
									<?php if ( count( $historico ) ) : ?>
										<tr>
											<td style="width: 20px;"><b>Seq.</b></td>
											<td style="width: 200px;"><b>Onde Estava</b></td>
											<td style="width: 200px;"><b>O que aconteceu</b></td>
											<td style="width: 90px;"><b>Quem fez</b></td>
											<td style="width: 120px;"><b>Quando fez</b></td>
											<td style="width: 17px;">&nbsp;</td>
										</tr>
									<?php endif; ?>
								</thead>
								<?php $i = 1; ?>
								<?php foreach ( $historico as $item ) : ?>
									<?php $marcado = $i % 2 == 0 ? "" : "#f7f7f7";?>
									<tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
										<td align="right"><?=$i?>.</td>
										<td style="color:#008000;">
											<?php echo $item['esddsc']; ?>
										</td>
										<td valign="middle" style="color:#133368">
											<?php echo $item['aeddscrealizada']; ?>
										</td>
										<td style="font-size: 6pt;">
											<?php echo $item['usunome']; ?>
										</td>
										<td style="color:#133368">
											<?php echo $item['htddata']; ?>
										</td>
										<td style="color:#133368; text-align: center;">
											<?php if( $item['cmddsc'] ) : ?>
												<img
													align="middle"
													style="cursor: pointer;"
													src="http://<?php echo $_SERVER['SERVER_NAME'] ?>/imagens/restricao.png"
													onclick="exebirOcultarComentario( '<?php echo $i; ?>' );"
												/>
											<?php endif; ?>
										</td>
									</tr>
									<tr id="comentario<?php echo $i; ?>" style="display: none;" bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
										<td colspan="6">
											<div >
												<?php echo htmlentities( $item['cmddsc'] ); ?>
											</div>
										</td>
									</tr>
									<?php $i++; ?>
								<?php endforeach; ?>
								<?php $marcado = $i++ % 2 == 0 ? "" : "#f7f7f7";?>
								<tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
									<td style="text-align: right;" colspan="6">
										Estado atual: <span style="color:#008000;"><?php echo $atual['esddsc']; ?></span>
									</td>
								</tr>
							</table>
							<br /><br />
						</td>
					</tr>
					<tr>
						<td class="SubTituloCentro" style="height:50px;">Parecer - MEC</td>
					</tr>
					<tr>	
						<td style='border-collapse: collapse; border: 1px solid #ccc;'>
							<br />
							<table class="listagem" cellspacing="0" cellpadding="3" align="center" style="width: 650px;border-top:0px;">
								<thead>
									<tr>
										<td style="width:40%;" align="center"><b>N° de Parecer(es) favorável(eis)</b></td>
										<td style="width:40%;" align="center"><b>N° de Parecer(es) não favorável(eis)</b></td>
										<td style="width:20%;" align="center"><b>Total de Paracer(es)</b></td>
									</tr>
								</thead>
								<tbody>
									<tr bgcolor="#f7f7f7" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='#f7f7f7';">
										<?php // feito na pressa, arrumar depois... ?>
										<?php $obModel 	= new Modelo(); ?>
										<?php $orsid 	= $obModel->pegaUm("SELECT orsid FROM obras.ordemservico WHERE gpdid = {$obDeclaracao->gpdid} AND orsstatus = 'A' AND stoid = ".OrdemServico::SITUACAOOS_GERADA); ?>
										<?php $chkids 	= $obModel->carregar("SELECT * FROM obras.checklistvistoria WHERE orsid = {$orsid} AND chkstatus = 'A'"); ?>
										<?php $itensFav 	= 0; ?>
										<?php $itensNaoFav 	= 0; ?>
										<?php $otimo 		= 0; ?>
										<?php $bom 			= 0; ?>
										<?php $regular 		= 0; ?>
										<?php $pessimo 		= 0; ?>
										<?php if( $chkids ): ?>
										<?php foreach($chkids as $checklist): ?>
										<?php $movparecercklist = $obModel->carregar("SELECT nisid,mpcsituacao FROM obras.movparecercklist WHERE chkid = ".$checklist['chkid']." AND mpcstatus = 'A'"); ?>
										<?php if( $movparecercklist[0]['mpcsituacao'] && $movparecercklist[0]['mpcsituacao'] == 't' ): ?>
										<?php $itensFav++; ?>
										<?php else: ?>
										<?php $itensNaoFav++; ?>
										<?php endif; ?>
										<?php if($movparecercklist[0]['nisid'] == 1) $pessimo++; ?>
										<?php if($movparecercklist[0]['nisid'] == 2) $regular++; ?>
										<?php if($movparecercklist[0]['nisid'] == 3) $bom++; ?>
										<?php if($movparecercklist[0]['nisid'] == 4) $otimo++; ?>
										<?php endforeach; ?>
										<?php endif; ?>
										<td align="center"><?=$itensFav?></td>
										<td align="center"><?=$itensNaoFav?></td>
										<td align="center"><?=($itensFav + $itensNaoFav)?></td>
									</tr>
								</tbody>
							</table>
							<br /><br />
						</td>
					</tr>
					<tr>
						<td class="SubTituloCentro" style="height:50px;">Qtd. de Avaliações - Nível de Satisfação - MEC</td>
					</tr>
					<tr>	
						<td style='border-collapse: collapse; border: 1px solid #ccc;'>
							<br />
							<table class="listagem" cellspacing="0" cellpadding="3" align="center" style="width: 650px;border-top:0px;">
								<thead>
									<tr>
										<td style="width:20%;" align="center"><b>Ótimo</b></td>
										<td style="width:20%;" align="center"><b>Bom</b></td>
										<td style="width:20%;" align="center"><b>Regular</b></td>
										<td style="width:20%;" align="center"><b>Péssimo</b></td>
										<td style="width:20%;" align="center"><b>Total de Avaliações</b></td>
									</tr>
								</thead>
								<tbody>
									<tr bgcolor="#f7f7f7" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='#f7f7f7';">
										
										<td align="center"><?=$otimo?></td>
										<td align="center"><?=$bom?></td>
										<td align="center"><?=$regular?></td>
										<td align="center"><?=$pessimo?></td>
										<td align="center"><?=($otimo + $bom + $regular + $pessimo)?></td>
									</tr>
								</tbody>
							</table>
							<br /><br />
						</td>
					</tr>
					<tr>
						<td align="center">
						<hr>
						<br />
						<br />
						Brasília, ______ de __________________________________ de <?=date('Y')?>.
						<br /><br />
						<br /><br />
						<br /><br />
						<br /><br />
						<div>
							<hr style="width: 400px">
							<center>
								<b>Nome do Representante da Empresa</b>
							</center>
						</div>
						<br /><br />
						</td>
					</tr>
				</table>
			</body>
		</html>
		<? 
		$output = ob_get_contents();
		ob_clean();
		
		
		if(!is_dir(APPRAIZ.'arquivos')) {
			mkdir(APPRAIZ.'arquivos', 0777);
		}
		if(!is_dir(APPRAIZ.'arquivos/' . $_SESSION['sisdiretorio'] . '')) {
			mkdir(APPRAIZ.'arquivos/' . $_SESSION['sisdiretorio'] . '', 0777);
		}
		if(!is_dir(APPRAIZ.'arquivos/' . $_SESSION['sisdiretorio'] . '/'.floor($dclid/1000))) {
			mkdir(APPRAIZ.'arquivos/' . $_SESSION['sisdiretorio'] . '/'.floor($dclid/1000), 0777);
		}
		
		$arqGerado = file_put_contents($caminho . $name, $output);
		if( $arqGerado )
		{
			$obModel = new Arquivo();
			// campos que teram valores no insert|update
			$arCampos = array(
							  	'arqnome', 
							  	'arqdescricao', 
							  	'arqextensao', 
							  	'arqtipo', 
							  	'arqtamanho', 
							  	'arqdata', 
							  	'arqhora', 
							  	'arqstatus', 
							  	'usucpf', 
							  	'sisid', 
							 );
					
			$arDados = array(
								'arqnome' 		=> current( explode(".",$name) ), 
							  	'arqdescricao'  => "Arquivo de Declaração gerada (OBRAS)", 
							  	'arqextensao' 	=> "html", 
							  	'arqtipo'	  	=> "text/html", 
							  	'arqtamanho'  	=> filesize( $caminho . $name ),		
							);		
									 
			$obModel->popularObjeto($arCampos, $arDados);		
			$arqid = $obModel->salvar();
			$obModel->commit();
		}
		
		return $arqid;
	}
}

?>
