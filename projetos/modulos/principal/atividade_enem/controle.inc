<?php

$atividade = atividade_pegar( $_REQUEST['atiid'] );
if ( !$atividade ) {
	redirecionar( 'principal/atividade_enem/arvore', 'A' );
}

$parametros = array(
	'aba' => $_REQUEST['aba'], # mantém a aba ativada
	'atiid' => $_REQUEST['atiid'] 
);

// impede que usuário sem permissão acione eventos
if ( $_REQUEST['evento'] && !atividade_verificar_responsabilidade( $_REQUEST['atiid'], $_SESSION['usucpf'] ) ) {
	redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
}

switch( $_REQUEST['evento'] ){

	case 'cadastrar_controle':
		$sql = sprintf(
			"insert into projetos.observacaoatividade ( obsdescricao, obsmedida, atiid, usucpf ) values ( '%s', '%s', %d, '%s' )",
			$_REQUEST['obsdescricao'],
			$_REQUEST['obsmedida'],
			$_REQUEST['atiid'],
			$_SESSION['usucpf']
		);
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
		} else {
			$db->commit();
		}
		redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
		break;

	case 'excluir_controle':
		$sql = sprintf(
			"update projetos.observacaoatividade set obsstatus = 'I' where obsid = %d",
			$_REQUEST['obsid']
		);
		if( !$db->executar( $sql ) ){
			$db->rollback();
		} else {
			$db->commit();
		}
		redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
		break;

	case 'controlar_solucao':
	 
//		echo'<pre>';
//		print_r( $_REQUEST);
//		echo'</pre>';
//		die();
		if ( $_REQUEST['obssolucao'] == 't' ) {
			$sql = sprintf(
				"update projetos.observacaoatividade set
					obssolucao = true,
					obsdescricao = '%s',
					obsdata = '%s',
					obsdatasolucao = '%s',
					obsmedida = '%s',
					usucpfsolucao = '%s'
				where obsid = %d",
				$_POST['hiddenDesc'][$_POST['id']],
				formata_data_sql( date( 'd/m/Y' ) ),
				formata_data_sql( date( 'd/m/Y' ) ),  
				$_REQUEST['obsmedida'],
				$_SESSION['usucpf'],
				$_REQUEST['obsid']
			);
//			dbg( $sql );
//			die();
		} else {
			$sql = sprintf(
				"update projetos.observacaoatividade set
					obssolucao = false,
					obsdescricao = '%s',
					obsdata = '%s',
					obsdatasolucao = null,
					obsmedida = '%s',
					usucpfsolucao = null
					where obsid = %d",
				$_POST['hiddenDesc'][$_POST['id']],
				formata_data_sql( date( 'd/m/Y' ) ),
				$_REQUEST['obsmedida'],
				$_REQUEST['obsid']
			);
		}
		if( !$db->executar( $sql ) ){
			$db->rollback();
		} else {
			$db->commit();
		}
		redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
		break;

	default:
		break;

}

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

// VERIFICA SE PROJETO ESTÁ SELECIONADO
projeto_verifica_selecionado();

// CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '&atiid=' . $atividade['atiid']  );
montar_titulo_projeto( $atividade['atidescricao'] );

extract( $atividade ); # mantém o formulário preenchido
?>
<!-- habilita o tiny -->
<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
   <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script src="../includes/prototype.js"></script>
    <script src="../includes/entidades.js"></script>
    <script src="../includes/calendario.js"></script>
    
<script language="JavaScript">
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
</script>

<script language="javascript" type="text/javascript">
	
	function cadastrar_controle(){
		if ( validar_formulario_controle() ) {
			document.controle.submit();
		}
	}
	
	function validar_formulario_controle(){
		var validacao = true;
		var mensagem = 'Os seguintes campos não foram preenchidos:';
		if ( tinyMCE.getContent('obsdescricao') == '' ) {
			mensagem += '\nConteúdo';
			validacao = false;
		}
		if ( !validacao ) {
			alert( mensagem );
		}
		return validacao;
	}
	
	function excluirControle( controle ){
		if ( confirm( 'Deseja excluir o controle?' ) ) {
			window.location = '?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&atiid=<?= $_REQUEST['atiid'] ?>&aba=<?= $_REQUEST['aba'] ?>&evento=excluir_controle&obsid='+ controle;
		}
	}

	function filtrar_restricoes( filtro ){
		if ( filtro ) {
			window.location = '?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&atiid=<?= $_REQUEST['atiid'] ?>&aba=<?= $_REQUEST['aba'] ?>';
		} else {
			window.location = '?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&atiid=<?= $_REQUEST['atiid'] ?>&aba=<?= $_REQUEST['aba'] ?>&filtro=1';
		}
	}

	function enviar_email( cpf ){
		var nome_janela = 'janela_enviar_emai_' + cpf;
		window.open(
			'/geral/envia_email.php?cpf=' + cpf,
			nome_janela,
			'width=650,height=557,scrollbars=yes,scrolling=yes,resizebled=yes'
		);
	}

	function trim(str)
	{
		return str.replace(/^\s+|\s+$/g,"");
	}
	
	function alteraCampos(id)
	{ 
		top.altera = 'true'; 
 		var antigoValueDesc = trim(document.getElementById('celDescricao_'+id).innerHTML);
 		var antigoValueData = document.getElementById('celData_'+id).innerHTML;
 		
 		//var inputDesc = "<input type ='text' onkeyup='capturaValorDesc("+id+",this.value);' name='inputDescricao_"+id+" id='inputDescricao_"+id+" value="+antigoValueDesc+">";
 		//var inputData = '<input type ="text"  onkeyup="capturaValorData('+id+',this.value);" name="inputData_'+id+'" id="inputData_'+id+'" value="'+antigoValueData+'"  onkeyup=this.value=mascaraglobal("##/##/####",this.value); >';
 
 		var inputDesc = '<div class="notprint">'+
							'<textarea  id="inputDescricao_'+id+'" name="inputDescricao_'+id+'" cols="10" rows="2"  onkeyup="capturaValorDesc('+id+',this.value);" onmouseover="MouseOver( this );" onfocus="MouseClick( this );" onmouseout="MouseOut( this );" onblur="MouseBlur( this );" style="width:70ex;" onkeydown="textCounter( this.form.inputDescricao_'+id+', this.form.no_inputDescricao_'+id+', 250 );"  onkeyup="textCounter( this.form.inputDescricao_'+id+', this.form.no_inputDescricao_'+id+', 250);" >'+antigoValueDesc+'</textarea>'+ 
								'<img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório." />'+
								'<br>'+
							'<input readonly style="text-align:right;border-left:#888888 3px solid;color:#808080;" type="text" name="no_inputDescricao_'+id+'" size="6" maxlength="6" value="250" class="CampoEstilo">'+
								'<font color="red" size="1" face="Verdana"> máximo de caracteres</font>'+
						'</div>'+
						'<div id="print_inputDescricao_'+id+'" class="notscreen" style="text-align: left;"></div>'+             
	    	 			'</div>';
	    	 			 
		document.getElementById('celDescricao_'+id).innerHTML = inputDesc;
 		//document.getElementById('celData_'+id).innerHTML = inputData;
 		
 		document.getElementById('altera['+id+']').style.visibility= "hidden";
		document.getElementById('altera['+id+']').style.position= "absolute";
		
	}
	
	function capturaValorDesc(id,value)
	{ 
		document.getElementById('hiddenDesc['+id+']').value = value;
		//document.getElementById('hiddenData['+id+']').value = <?=date("Y-m-d"); ?>;
	}
	function capturaValorData(id,value)
	{
		document.getElementById('hiddenData['+id+']').value = value;
	}
</script>

<script language="javascript" type="text/javascript">
	
	function ltrim( value ){
		var re = /\s*((\S+\s*)*)/;
		return value.replace(re, "$1");
	}
	
	function rtrim( value ){
		var re = /((\s*\S+)*)\s*/;
		return value.replace(re, "$1");
	}
	
	function trim( value ){
		return ltrim(rtrim(value));
	}
</script>
<table class="tabela" bgcolor="#fbfbfb" cellspacing="0" cellpadding="10" align="center">
	<tr>
		<td>
			<?= montar_resumo_atividade( $atividade ) ?>
			<!-- NOVO CONTROLE -->
			<?php if( $permissao ): ?>
				<form method="post" name="controle">
					<input type="hidden" name="evento" value="cadastrar_controle"/>
					<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 100%;">
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Restrição:</td>
							<td>
								<textarea name="obsdescricao" id="obsdescricao" rows="10" cols="70" class="text_editor_simple"><?= $obsdescricao ?></textarea>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Providência:</td>
							<td><?= campo_textarea( 'obsmedida', 'N', 'S', '', 70, 2, 250 ); ?></td>
						</tr>
						<tr style="background-color: #cccccc">
							<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
							<td><input type="button" name="botao" value="Salvar" onclick="cadastrar_controle();"/></td>
						</tr>
					</table>
				</form>
			<?php endif; ?>
			<!-- LISTA DE RESTRIÇÕES -->
			<?php
				$filtro = $_REQUEST['filtro'] ? ' and o.obssolucao = false ' : '';
				$sql = sprintf(
					"select
						o.obsid, o.obsdata, o.obsdescricao,
						o.obssolucao, o.obsdatasolucao, o.obsmedida,
						autor.usucpf as cpfautor, autor.usunome as nomeautor, autor.usufoneddd as dddautor, autor.usufonenum as telefoneautor, unidadeautor.unidsc as unidadeautor,
						responsavel.usucpf as cpfresponsavel, responsavel.usunome as nomeresponsavel, responsavel.usufoneddd as dddresponsavel, responsavel.usufonenum as telefoneresponsavel, unidaderesponsavel.unidsc as unidaderesponsavel
					from projetos.observacaoatividade o
					left join seguranca.usuario autor on
						autor.usucpf = o.usucpf
					left join public.unidade unidadeautor on
						unidadeautor.unicod = autor.unicod
					left join seguranca.usuario responsavel on
						responsavel.usucpf = o.usucpfsolucao
					left join public.unidade unidaderesponsavel on
						unidaderesponsavel.unicod = responsavel.unicod
					where
						o.atiid = %d and o.obsstatus = 'A' %s
					order by o.obsdata desc",
					$_REQUEST['atiid'],
					$filtro
				);
				
				$restricoes = $db->carregar( $sql );
				//dbg( $restricoes);
			?>
			<?php if( is_array( $restricoes ) ): ?>
				<label onclick="filtrar_restricoes( <?= $_REQUEST['filtro'] ?> );">
					<input type="checkbox" name="filtro" value="1" class="normal" <?= $_REQUEST['filtro'] ? 'checked="checked"' : '' ?>/>
					Exibir apenas as restrições <b>não superadas</b>.
				</label>
				<?php foreach( $restricoes as $restricao ): ?>
				<form method="post" id="formulario_<?= $restricao['obsid'] ?>">
					<input type="hidden" name="evento" value="controlar_solucao"/>
					<input type="hidden" name="obsid" value="<?= $restricao['obsid'] ?>"/>
					<table class='tabela' bgcolor="#f5f5f5" style="width:100%; margin-top: 15px;<?= $restricao['obssolucao'] == 't' ? 'color:#454545;' : '' ?>" cellpadding="3">
						<tbody>
							<tr style="background-color: #cccccc">
								<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
								<td align='left'  style="vertical-align:top;">
									<b>Restrição</b>
									<?php if( $restricao['obssolucao'] != 't' ): ?><img src="../imagens/restricao.png" border="0" align="absmiddle" style="margin: 0 3px 0 3px;"/><?php endif; ?>
								</td>
							</tr>
							<tr>
								<td class="SubTituloDireita" style="vertical-align:top; width:25%;">Descrição:</td>
								<input type="hidden"  id="hiddenDesc[<?= $restricao['obsid'] ?>]" name="hiddenDesc[<?= $restricao['obsid'] ?>]" value="<?= $restricao['obsdescricao'] ?>"/>
								<input type="hidden" name="id" value="<?= $restricao['obsid'] ?>"/>
								<td id="celDescricao_<?= $restricao['obsid'] ?>" name="celDescricao_<?= $restricao['obsid'] ?>" >
								
									<?= $restricao['obsdescricao'] ?>
								</td>
							</tr>
							<tr>
								<td class="SubTituloDireita" style="vertical-align:top; width:25%;">Data:</td>
								<input type="hidden" id="hiddenData[<?= $restricao['obsid'] ?>]" name="hiddenData[<?= $restricao['obsid'] ?>]" value="<?= $restricao['obsdata'] ?>"/>
								<td id="celData_<?= $restricao['obsid'] ?>" name="celData_<?= $restricao['obsid'] ?>"><?= formata_data( $restricao['obsdata'] ); ?></td>
							</tr>
							<tr>
								<td class="SubTituloDireita" style="vertical-align:top; width:25%;">Autor:</td>
								<td>
									<div>
										<img onclick="enviar_email( '<?= $restricao['cpfautor'] ?>' );" title="enviar e-mail" src="../imagens/email.gif" align="absmiddle" style="border:0; cursor:pointer;"/>
										<?= $restricao['nomeautor'] ?>
									</div>
									<div style="color:#959595;"><?= $restricao['unidadeautor'] ?> - Tel: (<?= $restricao['dddautor'] ?>) <?= $restricao['telefoneautor'] ?></div>
								</td>
							</tr>
							<tr>
								<td class="SubTituloDireita" style="vertical-align:top; width:25%;">Restrição superada?</td>
								<td>
									<?php if( $permissao ): ?>
										<label title="indica que a restrição está superada">
											<input type="radio" name="obssolucao" value="t" <?= $restricao['obssolucao'] == 't' ? 'checked="checked"' : '' ?>/>
											Sim
										</label>
										&nbsp;&nbsp;
										<label title="indica que a restrição não está superada">
											<input type="radio" name="obssolucao" value="f" <?= $restricao['obssolucao'] == 'f' ? 'checked="checked"' : '' ?>/>
											Não
										</label>
									<?php else: ?>
										<?= $restricao['obssolucao'] == 't' ? 'Sim' : 'Não' ?>
									<?php endif; ?>
								</td>
							</tr>
							<tr>
								<td class="SubTituloDireita" style="vertical-align:top; width:25%;">Providência:</td>
								<td>
								<?php
								
								$obsmedida = $restricao["obsmedida"];
								echo campo_textarea( 'obsmedida', 'N', 'S', '', 70, 2, 250 );
								
								?>
								</td>
							</tr>
							
							<?php if( $restricao['obssolucao'] == 't' ): ?>
								<tr id="titulo_providencia_<?= $restricao['obsid'] ?>" style="background-color: #cccccc;">
									<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
									<td align='left' style="vertical-align:top;"><b>Superação</b></td>
								</tr>
								<tr id="datasuperacao_<?= $restricao['obsid'] ?>">
									<td class="SubTituloDireita" style="vertical-align:top; width:25%;">Data:</td>
									<td id="<?= $restricao['obsid'] ?>"><?= formata_data( $restricao['obsdatasolucao'] ) ?></td>
								</tr>
								<tr id="responsavel_<?= $restricao['obsid'] ?>">
									<td class="SubTituloDireita" style="vertical-align:top; width:25%;">Autor:</td>
									<td>
										<div>
											<img onclick="enviar_email( '<?= $restricao['cpfresponsavel'] ?>' );" title="enviar e-mail" src="../imagens/email.gif" align="absmiddle" style="border:0; cursor:pointer;"/>
											<?= $restricao['nomeresponsavel'] ?>
										</div>
										<div style="color:#959595;"><?= $restricao['unidaderesponsavel'] ?> - Tel: (<?= $restricao['dddresponsavel'] ?>) <?= $restricao['telefoneresponsavel'] ?></div>
									</td>
								</tr>
							<?php endif; ?>
							
							<?php if( $permissao ): ?>
								<tr style="background-color: #cccccc">
									<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
									<td>
									<?php
									if( $restricao['cpfautor'] == $_SESSION['usucpf'])
									{
									?>
										<input type="button" name="altera[<?= $restricao['obsid'] ?>]" id="altera[<?= $restricao['obsid'] ?>]" value="Alterar" onclick="alteraCampos(<?= $restricao['obsid'] ?>);"/>
									<?php
									}
									?>
										<input type="submit" name="botao" value="Salvar"/>
										<input type="button" name="botao" value="Excluir" onclick="excluirControle( <?= $restricao['obsid'] ?> );" />
									</td>
								</tr>
							<?php endif; ?>
						</tbody>
					</table>
				</form>
				<?php endforeach; ?>
			<?php else: ?>
				<table class='tabela' style="width:100%;" cellpadding="3">
					<tbody>
						<td style="text-align:center;padding:15px;background-color:#f5f5f5;">
							A atividade não possui restrições.
						</td>
					</tbody>
				</table>
			<?php endif; ?>
		</td>
	</tr>
</table>