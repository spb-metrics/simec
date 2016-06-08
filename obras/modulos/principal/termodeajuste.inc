<?php

/**
 * Tela com a finalidade de controlar os termos de ajuste das obras.
 * 
 * @author Fernando Araújo Bagno da Silva 
 * @since 19/08/2009
 * @version 1.0
 */

// pega os orgão (tipo de ensino) permitidos para o usuário
$res      = obras_pegarOrgaoPermitido();
$orgidRes = array();

for( $i = 0; $i < count( $res ); $i++ ){
	foreach( $res[$i] as $chave=>$valor ){
		if ( $chave == "id" ){
			$orgidRes[] = $valor;
		}
	}
}

// cria o objeto da classe termoDeAjuste
$termodeajuste = new termoDeAjuste();

// cadastra ou atualiza o termo de ajuste 
switch ( $_REQUEST['requisicao'] ){
	case 'cadastra':
		$termodeajuste->CadastraTermoAjuste( $_REQUEST );
	break;
	case 'atualiza':
		$termodeajuste->AtualizaTermoAjuste( $_REQUEST );
	break;
}

// exclui os participantes ou a unidade do participante do termo de ajuste
switch ( $_REQUEST["subacao"] ){
	case "excluiparticipante":
		$termodeajuste->ExcluiParticipante( $_REQUEST["ptaid"] );
		break;
	case "excluiunidade":
		$termodeajuste->ExcluiUnidadeParticipante( $_REQUEST["ptaid"] );
		break;	
}

// quando o usuário seleciona o termo de ajuste pela lista, cria a sessão
if ( $_REQUEST['traid'] ){
	$termodeajuste->verificaDados( $_REQUEST['traid'], $_REQUEST['orgid'] );
}

// cria a variável com o ID do termo, caso exista
$traid = !empty($_SESSION['obra']['traid']) ? $_SESSION["obra"]["traid"] : 'null';

if ( $traid != 'null' ){
	$dadosTermoAjuste = $termodeajuste->buscaTermoAjuste( $traid );
}

// verifica o tipo de requisicao da tela ( inserção / atualização )
$requisicao = $traid != 'null' ? 'atualiza' : 'cadastra';

// cabeçalho padrão do sistema
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

// cria o título e as abas da tela
echo '<br/>';
if ( $traid != 'null' ){
	$db->cria_aba( $abacod_tela, $url, $parametros );	
}
monta_titulo( 'Termo de Ajuste', '' );

?>

<script src="../includes/calendario.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
<script language="JavaScript">
	//Editor de textos
	tinyMCE.init({
		mode : "textareas",
		theme : "advanced",
		plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
		theme_advanced_buttons1 : "undo,redo,separator,bold,italic,underline,separator,justifyleft,justifycenter,justifyright, justifyfull",
		theme_advanced_buttons2 : "",
		theme_advanced_buttons3 : "",
		theme_advanced_toolbar_location : "top",
		theme_advanced_toolbar_align : "left",
		extended_valid_elements : "a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
		language : "pt_br",
		entity_encoding : "raw"
		});
</script>
<form action="" method="post" name="formulario" id="formulario">
	<input type="hidden" name="requisicao" value="<?php echo $requisicao; ?>"/>
	<input type="hidden" name="traid" value="<?php echo $traid; ?>"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td width="180px" class="subtitulodireita">Tipo de Ensino</td>
			<td>
				<?php

					$orgid = $dadosTermoAjuste['orgid']; 
					
					$haborgao = ( $somenteLeitura == "S" && count($orgidRes) > 1 ) ? "S" : "N";
				
					if( count($orgidRes) > 0 ){
						
						$sql = "SELECT orgid as codigo, orgdesc as descricao
								FROM obras.orgao WHERE orgid in ( " . (implode(", ", $orgidRes) ) . " )";
						
						$db->monta_combo("orgid", $sql, $haborgao, "Selecione...", '', '', '', '', 'S','orgid');
						
					}
				
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Assunto</td>
			<td>
				<?php
					$traassunto = $dadosTermoAjuste['traassunto']; 
					echo campo_texto( 'traassunto', 'S', $somenteLeitura, '', 50, 60, '', '', 'left', '', 0, 'id="traassunto"'); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Local</td>
			<td>
				<?php
					$tralocal = $dadosTermoAjuste['tralocal'];
					echo campo_texto( 'tralocal', 'S', $somenteLeitura, '', 50, 60, '', '', 'left', '', 0, 'id="tralocal"'); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Data</td>
			<td>
				<?php 
					$tradtcriacao = $dadosTermoAjuste['tradtcriacao'];
					echo campo_data( 'tradtcriacao', 'S', $somenteLeitura, '', 'S' ); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Texto da Ata</td>
			<td>
				<?php
					$tratextoata = stripcslashes($dadosTermoAjuste['tratextoata']); 
					echo campo_textarea("tratextoata", 'N', $somenteLeitura, '',80, 15, '') ;
					
				?>
			</td>
		</tr>
		<tr>
			<td colspan="2" class="subtitulocentro">Participante(s)</td>
		</tr>
		<tr>
			<td colspan="2">
				<div id="listaParticipantes">
					<?php 
					
						// monta a lista com os participantes do termo de ajuste
						$sql = "SELECT 
									'<center>
										<img src=\"/imagens/exclui_p.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"excluiParticipante(' || ptaid || ');\">
									</center>' as acao,
									ee.entnumcpfcnpj as cpf,
									'<img style=\"cursor:pointer;\" src=\"/imagens/consultar.gif\" border=\"0\" title=\"Visualizar Termo\" onclick=\"insereParticipante(' || ee.entid || ', \'consulta\');\"> &nbsp;' || ee.entnome as nome,
									CASE WHEN entidunidade is null THEN 
											'<img src=\"/imagens/incluir_p.gif\" style=\"padding-right: 1px; cursor: pointer; width: 12px;\" 
						 				  	 align=\"absmiddle\" vspace=\"1\" border=\"0\" onclick=\"insereUnidadeParticipante(' || ptaid || ');\"> 
						 				 	 <a style=\"cursor:pointer;\" onclick=\"insereUnidadeParticipante(' || ptaid || ');\"> Inserir Unidade </a>'
					 				 	 ELSE
					 				 	 	'<img src=\"/imagens/check_p.gif\" style=\"padding-right: 1px; cursor: pointer;\" 
						 				  	 align=\"absmiddle\" vspace=\"1\" border=\"0\" onclick=\"insereUnidadeParticipante(' || ptaid || ');\">
						 				  	 <img src=\"/imagens/exclui_p.gif\" style=\"padding-right: 1px; cursor: pointer;\" 
						 				  	 align=\"absmiddle\" vspace=\"1\" border=\"0\" onclick=\"excluiUnidadeParticipante(' || ptaid || ');\">&nbsp;' || ee2.entnome 
					 				END as entidade
								FROM 
									entidade.entidade ee
								INNER JOIN
									obras.participantetermoajuste pt ON pt.entidparticipante = ee.entid
								LEFT JOIN
									entidade.entidade ee2 ON pt.entidunidade = ee2.entid
								WHERE
									pt.traid = {$traid}
								ORDER BY
									ee.entnome";
						
						$cabecalho = array( "Ação", "CPF", "Nome do Autor", "Unidade" );
						$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '95%');
						
					?>
				</div>
				<br/>
				<?php if ( $traid != 'null' ){ ?>
					<img src="/imagens/incluir_p.gif" style="padding-right: 1px; cursor: pointer; width:12px;" 
					 align="absmiddle" vspace="1" border="0" onclick="insereParticipante( '', 'cadastro' );" title="Inserir Participantes">
					<a style="cursor: pointer;" onclick="insereParticipante( '', 'cadastro' );">Inserir Participantes</a>
				<? } ?>
			</td>
		</tr>
		<tr bgcolor="#DCDCDC">
			<td></td>
			<td>
				<input type="button" value="Salvar" style="cursor: pointer;" onclick="obrasValidaTermoAjuste();"/>
				<input type="button" value="Voltar" style="cursor: pointer;" onclick="history.back(-1)"/>
			</td>
		</tr>
	</table>
</form>
