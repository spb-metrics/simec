<?php 

if(!$_REQUEST['atiid']){
	header("Location: enem.php?modulo=principal/atividade_enem/arvore&acao=A");
}

if($_POST['requisicao']){
	$arrResposta = $_POST['requisicao']();
}

if( $_REQUEST['download'] == 'S' )
{
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec();
	$arqid = $_REQUEST['arqid'];
    $arquivo = $file->getDownloadArquivo($arqid);
    echo"<script>window.location.href = 'enem.php?modulo=principal/atividade_enem/atividade&acao=P&atiid=".$_REQUEST['atiid']."';</script>";
    exit;
}

if( $_REQUEST['arqidDel'] != '' )
{
    $anexos = array();
    
    $sql = "UPDATE projetos.anexoatividade SET anestatus = 'I' WHERE arqid = ".$_REQUEST['arqidDel'];
    $db->executar($sql);
    $sql = "UPDATE public.arquivo SET arqstatus = 'I' WHERE arqid = ".$_REQUEST['arqidDel'];
    $db->executar($sql);
    
    $db->commit();
    
    echo"<script>window.location.href = 'enem.php?modulo=principal/atividade_enem/atividade&acao=P&atiid=".$_REQUEST['atiid']."';</script>";
    die;
}

include APPRAIZ . 'includes/cabecalho.inc';
echo '<br/>';

$db->cria_aba( $abacod_tela, $url, '&atiid=' . $_REQUEST['atiid'] );
montar_titulo_projeto( $atividade['atidescricao'] );

//Carregar dados da Atividade
if($_REQUEST['atiid']){
	$sql = "select 
				atiid,
				atidescricao,
				atidatainicio,
				atidatafim,
				atidetalhamento,
				atimeta,
				nvcid
			from
				projetos.atividade
			where
				atiid = {$_REQUEST['atiid']}
			and
				atistatus = 'A'";
	$arrDados = $db->pegaLinha($sql);
}


//Regras de liberação de edição / inclusão
if( temPerfilSomenteConsulta() || temPerfilExecValidCertif() )
	$somenteLeitura = "N";
else
	$somenteLeitura = "S";

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script>
//Editor de textos
tinyMCE.init({
	theme : "advanced",
	mode: "specific_textareas",
	editor_selector : "text_editor_simple",
	plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
	theme_advanced_buttons1 : "undo,redo,separator,link,bold,italic,underline,forecolor,backcolor,separator,justifyleft,justifycenter,justifyright, justifyfull, separator, outdent,indent, separator, bullist",
	theme_advanced_buttons2 : "",
	theme_advanced_buttons3 : "",
	theme_advanced_toolbar_location : "top",
	theme_advanced_toolbar_align : "left",
	extended_valid_elements : "a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
	language : "pt_br",
	width : "450px",
	entity_encoding : "raw"
	});
function salvarDadoProcesso()
{
	var erro = 0;
	$("[class~=obrigatorio]").each(function() { 
		if(!this.value){
			erro = 1;
			alert('Favor preencher todos os campos obrigatórios!');
			this.focus();
			return false;
		}
	});
	if(erro == 0){
		$("#formulario_processo").submit();
	}	
}

function inserirResponsavel()
{
	janela("enem.php?modulo=principal/atividade_enem/inserir_responsavel_processo&acao=A",600,580,"att_resp");
}

function atualizaResponsavel(entid)
{
	janela("enem.php?modulo=principal/atividade_enem/inserir_responsavel_processo&acao=A&entid=" + entid,600,580,"att_resp");
}

function RemoveLinha( idContato )
{
	if (confirm('Deseja realmente excluir este responsável?')){
		var index = window.document.getElementById('linha_'+idContato).rowIndex;
		table = window.document.getElementById("responsaveiscontato");
		table.deleteRow(index);
	}
}

function inserirProcessoImpacto()
{
	janela("enem.php?modulo=principal/atividade_enem/impacto&acao=A&atiid=<?php echo $_REQUEST['atiid']?>",400,400,"att_resp");
}

function cadastrar_controle(){
	if ( validar_formulario_controle() ) {
		document.controle.submit();
	}
}

function validar_formulario_controle(){
	var validacao = true;
	var mensagem = 'Os seguintes campos não foram preenchidos:';
	if ( tinyMCE.getContent('observacoes') == '' ) {
		mensagem += '\nConteúdo';
		validacao = false;
	}
	if ( !validacao ) {
		alert( mensagem );
	}
	return validacao;
}

function excluirAnexo( arqid )
{
	if ( confirm( 'Deseja excluir o Documento?' ) )
	{
		location.href= window.location+'&arqidDel='+arqid+'&atiid=<?=$_REQUEST['atiid']?>';
	}
}
</script>
<style>
	.SubtituloTabela{background-color:#cccccc}
	.negrito{font-weight:bold}
	.normal{font-weight:normal}
	.center{text-align: center;}
	.direita{text-align: right;}
</style>

<form name="formulario_processo" id="formulario_processo"  method="post" action="" enctype="multipart/form-data">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td></td>
			<td></td>
			<td rowspan="3">
			<?php 
			$sql = "SELECT
						 arq.arqid
					FROM
                        public.arquivo arq 
                    INNER JOIN 
                    	projetos.anexoatividade a ON arq.arqid = a.arqid
                    WHERE
                        arq.arqstatus = 'A'
                        AND a.atiid = ".$_REQUEST['atiid']."";
			$arqid = $db->pegaUm($sql);
			
			if($arqid){
			?>
			<div style="padding:10px;border:0px solid #c0c0c0;background-color:#f5f5f5;text-align:center;">
			<a style="cursor:pointer;color:blue;" onclick="window.location='?modulo=principal/atividade_enem/atividade&acao=P&atiid=<?=$_REQUEST['atiid']?>&download=S&arqid=<?=$arqid?>';" />
			Visualizar/Baixar o Diagrama
			</a>
			</div>
			<?php } ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" width="25%" >Nome / Descrição</td>
			<td id="descricao" name="descricao"><?php echo campo_texto( 'atidescricao', 'S', $somenteLeitura, '', 65, 60, '', '', 'left', '', 0,'','',$arrDados['atidescricao']); ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Data Início</td>
			<td id="data_inicio" name="data_inicio"><?php echo campo_data2( 'atidatainicio', 'S', $somenteLeitura, '', 'S' ,'','',$arrDados['atidatainicio']); ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Data Fim</td>
			<td id="data_fim" name="data_fim"><?php echo campo_data2( 'atidatafim', 'S', $somenteLeitura, '', 'S' ,'','',$arrDados['atidatafim']); ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Observações</td>
			<td>
			<textarea name="observacoes" id="observacoes" rows="8" cols="60" class="text_editor_simple"><?= $atidetalhamento ?></textarea>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Produtos Gerados - Entrada/Saída</td>
			<td id="produtos" name="produtos"><?php echo campo_textarea( 'atimeta', 'N', $somenteLeitura, '', '50' ,'','250','','','','','',$arrDados['atimeta']); ?></td>
		</tr>
		  <tr>
			<td colspan="3" class="SubtituloTabela center negrito" >Diagrama</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Anexar arquivo</td>
			<td>
				<input type="file" id="arquivo" name="arquivo" />
			</td>
		</tr>
		<tr>
	        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">
	        	Tipo do arquivo:
	        </td>
	        <td>
	        <?php 
	          $sql = "SELECT 
	          				taaid AS codigo, 
	          				taadescricao AS descricao
	                  FROM 
	                  		projetos.tipoanexoatividade
	                  --WHERE
	                  	--tpastatus = 'A' 
	                  ORDER BY 
	                  	taadescricao";
	        	$db->monta_combo('taaid', $sql, $somenteLeitura, "Selecione...", '', '', '', '100', 'N', 'taaid'); 
	        ?>
	        </td>
	    </tr>
	    <tr>
	        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">
	        	Descrição do arquivo:
	        </td>
	        <td>
	        	<?= campo_textarea( 'anedescricao', 'N', $somenteLeitura, '', 60, 2, 250 ); ?>
	        </td>
	    </tr>
		<tr>
			<td colspan="3">
			<?php
			
			if( temPerfilSomenteConsulta() || temPerfilExecValidCertif() )
				$acao = "'<center><a><img src=\"/imagens/excluir_01.gif\" border=0 title=\"Excluir\"></a></center>'";
			else
				$acao = "'<center><a style=\" cursor: pointer;\"onclick=\"javascript:excluirAnexo(' || arq.arqid || ');\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a></center>'";
				
			$sql = "SELECT
                        ".$acao." as acao,
                        to_char(arq.arqdata,'DD/MM/YYYY'),
                        taa.taadescricao,
                        '<a style=\"cursor: pointer; color: blue;\" onclick=\"window.location=\'?modulo=principal/atividade_enem/atividade&acao=P&atiid=".$_REQUEST['atiid']."&download=S&arqid=' || arq.arqid || '\';\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a>',
                        arq.arqtamanho || ' kbs' as tamanho, 
                        arq.arqdescricao
                    FROM
                        ((public.arquivo arq INNER JOIN projetos.anexoatividade a
                        ON arq.arqid = a.arqid) INNER JOIN projetos.tipoanexoatividade taa
                        ON taa.taaid = a.taaid) 
                    WHERE
                        arq.arqstatus = 'A'
                        AND a.atiid = ".$_REQUEST['atiid']."";
		
	        $cabecalho = array( "Ação",
	                            "Data Inclusão",
	                            "Tipo Arquivo",
	                            "Nome Arquivo",
	                            "Tamanho (Mb)",
	                            "Descrição Arquivo"
	                            //"Responsável"
	                            );
        	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', 'center', '' );
        	?>
			</td>
		</tr>
		<tr>
			<td colspan="3" class="SubtituloTabela center negrito" >Responsáveis</td>
		</tr>
		<tr>
			<td colspan="3" class="center" >
				<table id="responsaveiscontato" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
					<tr>
						<td class="subtitulocentro" width="15%">Ação</td>
						<td class="subtitulocentro" width="55%">Nome</td>
					</tr>
					<?php $sql = "select 
									ent.entid,
									ent.entnome
								from
									entidade.entidade ent
								inner join
									projetos.responsavelatividade rpa ON rpa.entid = ent.entid
								where
									rpa.rpastatus = 'A'
								and
									atiid = {$arrDados['atiid']}
								order by
									ent.entnome" ?>
					<?php $arrResp = $db->carregar($sql) ?>
					<?php if($arrResp):?>
						<?php foreach($arrResp as $resp):?>
							<tr id="linha_<?php echo $resp['entid'] ?>" >
								<td class="center" >
								<?php if( temPerfilSomenteConsulta() || temPerfilExecValidCertif() ): ?>
									<img src="/imagens/alterar_01.gif" border=0 title="Editar">
							       	<img src="/imagens/excluir_01.gif" border=0 title="Excluir">
							    <?php else: ?>
							    	<img src="/imagens/alterar.gif" style="cursor: pointer" border=0 title="Editar" onclick="atualizaResponsavel('<?php echo $resp['entid'] ?>')">
							       	<img src="/imagens/excluir.gif" style="cursor: pointer"  border=0 title="Excluir" onClick="RemoveLinha('<?php echo $resp['entid'] ?>')">
							    <?php endif; ?>
								</td>
								<td align="center" id="entnome_<?php echo $resp['entid'] ?>" >
									<?php echo $resp['entnome'] ?>
									<input type="hidden" name="entidresp[]" value="<?php echo $resp['entid'] ?>" />
								</td>
								</tr>
						<?php endforeach;?>
					<?php endif;?>
				</table>
			</td>
		</tr>
		<tr>
			<td colspan="3">
				<?php if( temPerfilSomenteConsulta() || temPerfilExecValidCertif() ): ?>
					<img src="/imagens/gif_inclui_d.gif" border="0" title="Inserir Contatos"> 
					Inserir Responsáveis 
				<?php else: ?>
				<a href="#" onclick="inserirResponsavel(); return false;">
					<img src="/imagens/gif_inclui.gif" style="cursor: pointer;" border="0" title="Inserir Contatos"> 
					Inserir Responsáveis
				</a>
				<?php endif;?>
			</td>
		</tr>
		<tr>
			<td colspan="3" class="SubtituloTabela center negrito" >Impactos</td>
		</tr>
		<tr>
			<td colspan="3" class="center" >
				<table id="processoatividade" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
					<tr>
						<td class="subtitulocentro" width="50%">Processo</td>
						<td class="subtitulocentro" width="50%">Observação</td>
					</tr>
					<?php $sql = "select 
									atiidimpacto,
									ipcdsc
								from
									projetos.impacto
								where
									ipcstatus = 'A'
								and
									atiid = {$_REQUEST['atiid']}" ?>
					<?php $arrProcessos = $db->carregar($sql) ?>
					<?php if($arrProcessos):?>
						<?php foreach($arrProcessos as $proc):?>
							<tr id="tr_processo_<?php echo $proc['atiidimpacto'] ?>" >
								<td>
									<?php echo $db->pegaUm("select atidescricao from projetos.atividade where atiid = {$proc['atiidimpacto']}") ?>
									<input type="hidden" name="atiidprocesso[]" value="<?php echo $proc['atiidimpacto'] ?>" />
								</td>
								<td>
									<?php echo campo_textarea( 'ipcdsc[]', 'S', $somenteLeitura, '', 120, 1, '', '', '', '','','',$proc['ipcdsc']); ?>
								</td>
							</tr>
						<?php endforeach;?>
					<?php endif;?>
				</table>
			</td>
		</tr>
		<tr>
			<td colspan="3">
			<?php if( temPerfilSomenteConsulta() || temPerfilExecValidCertif() ): ?>
				<img src="/imagens/gif_inclui_d.gif" border="0" title="Inserir Contatos"> 
				Inserir Processos
			<?php else: ?>
				<a href="#" onclick="inserirProcessoImpacto(); return false;">
					<img src="/imagens/gif_inclui.gif" style="cursor: pointer;" border="0" title="Inserir Contatos"> 
					Inserir Processos
				</a>
			<?php endif; ?>
			</td>
		</tr>
		<tr>
			<td colspan="3" class="SubtituloTabela center negrito" >
				<input type="hidden" name="atiid" value="<?php echo $arrDados['atiid'] ?>" />
				<input type="hidden" name="requisicao" value="salvarAtividade" />
				
				<?php if( temPerfilSomenteConsulta() || temPerfilExecValidCertif() ): ?>
				<input type="button" value="Salvar" disabled="disabled" />
				<?php else: ?>
				<input type="button" value="Salvar" onclick="salvarDadoProcesso()" />
				<?php endif; ?>
				
				<input type="button" value="Voltar" onclick="" />
			</td>
		</tr>
	</table>
</form>
<?php if($arrResposta['msg']):?>
	<script>
		alert('<?php echo $arrResposta['msg']?>')
	</script>
<?php endif;?>