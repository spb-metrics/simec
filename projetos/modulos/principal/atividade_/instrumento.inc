<?php

$atividade = atividade_pegar( $_REQUEST['atiid'] );
if ( !$atividade ) {
	redirecionar( 'principal/atividade_/arvore', 'A' );
}

$parametros = array(
	'aba' => $_REQUEST['aba'], # mantém a aba ativada
	'atiid' => $_REQUEST['atiid']
);

switch( $_REQUEST['evento'] ){

	case 'cadastrar_anexo':
		// obtém o arquivo
		$arquivo = $_FILES['arquivo'];
		if ( !is_uploaded_file( $arquivo['tmp_name'] ) ) {
			$db->rollback();
			redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
		}
		// cadastra o anexo
		$sql = sprintf(
			"insert into projetos.anexoatividade ( anedescricao, atiid, taaid ) values ( '%s', %d, %d ) returning aneid",
			substr( $_REQUEST['anedescricao'], 0, 255 ),
			$_REQUEST['atiid'],
			$_REQUEST['taaid']
		);
		$registro = array_pop( $db->carrega_tudo( $db->executar( $sql ) ) );
		$aneid = $registro['aneid'];
		if ( !$aneid ) {
			$db->rollback();
			redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
		}
		// cadastra a versão
		$sql = sprintf(
			"insert into projetos.versaoanexoatividade ( aneid, usucpf, vernome, vertipomime ) values ( %d, '%s', '%s', '%s' ) returning verid",
			$aneid,
			$_SESSION['usucpf'],
			$arquivo['name'],
			$arquivo['type']
		);
		$registro = array_pop( $db->carrega_tudo( $db->executar( $sql ) ) );
		$verid = $registro['verid'];
		if ( !$verid ) {
			$db->rollback();
			redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
		}
		// torna ativa a nova versão cadastrada
		$sql = sprintf(
			"update projetos.anexoatividade set verid = %d where aneid = %d",
			$verid,
			$aneid
		);
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
		}
		// grava o arquivo
		$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/projetos/'. $verid;
		if ( !move_uploaded_file( $arquivo['tmp_name'], $caminho ) ) {
			$db->rollback();
			redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
		}
		$db->commit();
		redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
		break;

	case 'excluir_anexo':
		$sql = sprintf( "update projetos.anexoatividade set anestatus = 'I' where aneid = %d", $_REQUEST['aneid'] );
		if( !$db->executar( $sql ) ){
			$db->rollback();
		} else {
			$db->commit();
		}
		redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
		break;

	case 'cadastrar_versao':
		// obtém o arquivo
		$arquivo = $_FILES['arquivo'];
		if ( !is_uploaded_file( $arquivo['tmp_name'] ) ) {
			redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
		}
		// cadastra a versão
		$sql = sprintf(
			"insert into projetos.versaoanexoatividade ( aneid, usucpf, vernome, vertipomime ) values ( %d, '%s', '%s', '%s' ) returning verid",
			$_REQUEST['aneid'],
			$_SESSION['usucpf'],
			$arquivo['name'],
			$arquivo['type']
		);
		$registro = array_pop( $db->carrega_tudo( $db->executar( $sql ) ) );
		$verid = $registro['verid'];
		if ( !$verid ) {
			$db->rollback();
			redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
		}
		// torna ativa a nova versão cadastrada
		$sql = sprintf(
			"update projetos.anexoatividade set verid = %d where aneid = %d",
			$verid,
			$_REQUEST['aneid']
		);
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
		}
		// move o arquivo para o diretório final
		$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/projetos/'. $verid;
		if ( !move_uploaded_file( $arquivo['tmp_name'], $caminho ) ) {
			$db->rollback();
			redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
		}
		$db->commit();
		redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
		break;

	case 'download':
		$sql = sprintf(
			"select v.verid, v.vernome, v.vertipomime, a.atiid from projetos.versaoanexoatividade v inner join projetos.anexoatividade a on a.aneid = v.aneid where v.verid = %d",
			$_REQUEST['verid']
		);
		$versao = $db->pegaLinha( $sql );
		$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/projetos/'. $versao['verid'];
		if ( !is_readable( $caminho ) ) {
			redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
		}
		header( 'Content-type: '. $versao['vertipomime'] );
		header( 'Content-Disposition: attachment; filename=' . $versao['vernome'] );
		readfile( $caminho );
		exit();

	default:
		break;

}

$permissao = atividade_verificar_responsabilidade( $atividade['atiid'], $_SESSION['usucpf'] );
$permissao_formulario = $permissao ? 'S' : 'N'; # S habilita e N desabilita o formulário

// ----- VERIFICA SE PROJETO ESTÁ SELECIONADO
projeto_verifica_selecionado();

// ----- CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '&atiid=' . $atividade['atiid']  );
montar_titulo_projeto( $atividade['atidescricao'] );

/*
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba(
	$abacod_tela,
	$url,
	'&atiid='.$_REQUEST['atiid']
);
monta_titulo(
	$titulo_modulo,
	$permissao ? '&nbsp;' : '<img src="../imagens/preview.gif" align="absmiddle"/> Atividade disponível apenas para visualização'
);
*/

extract( $atividade ); # mantém o formulário preenchido
?>
<script language="javascript" type="text/javascript">
	function cadastrar_anexo(){
		if ( validar_formulario_anexo() ) {
			document.anexo.submit();
		}
	}
	
	function validar_formulario_anexo(){
		var validacao = true;
		var mensagem = 'Os seguintes campos não foram preenchidos:';
		document.anexo.taaid.value = trim( document.anexo.taaid.value );
		document.anexo.anedescricao.value = trim( document.anexo.anedescricao.value );
		if ( document.anexo.taaid.value == '' ) {
			mensagem += '\nTipo';
			validacao = false;
		}
		if ( document.anexo.anedescricao.value == '' ) {
			mensagem += '\nDescrição';
			validacao = false;
		}
		if ( document.anexo.arquivo.value == '' ) {
			mensagem += '\nArquivo';
			validacao = false;
		}
		if ( !validacao ) {
			alert( mensagem );
		}
		return validacao;
	}
	
	function excluirAnexo( anexo ){
		if ( confirm( 'Deseja excluir o instrumento?' ) ) {
			window.location = '?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&atiid=<?= $_REQUEST['atiid'] ?>&aba=<?= $_REQUEST['aba'] ?>&evento=excluir_anexo&aneid='+ anexo;
		}
	}
	
	function cadastrar_versao( formulario ){
		if ( formulario.arquivo.value == '' ) {
			return;
		}
		formulario.submit();
	}
	
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
<script language="javascript" type="text/javascript">
	var IE = document.all ? true : false;
	function exibirOcultarIncluirVersao( identificador ){
		var anexo = document.getElementById( 'anexo_' + identificador );
		if ( anexo.style.display == "none" ) {
			if( !IE ) {
				anexo.style.display = "table-row";
			} else {
				anexo.style.display = "block";
			}
		} else {
			anexo.style.display = "none";
		}
		var historico = document.getElementById( 'historico_' + identificador );
		if ( historico.style.display != "none" ) {
			historico.style.display = "none";
		}
	}
	
	function exibirOcultarHistoricoVersao( identificador ){
		var historico = document.getElementById( 'historico_' + identificador );
		if ( historico.style.display == "none" ) {
			if( !IE ) {
				historico.style.display = "table-row";
			} else {
				historico.style.display = "block";
			}
		} else {
			historico.style.display = "none";
		}
		var anexo = document.getElementById( 'anexo_' + identificador );
		if ( anexo.style.display != "none" ) {
			anexo.style.display = "none";
		}
	}
</script>
<table class="tabela" bgcolor="#fbfbfb" cellspacing="0" cellpadding="10" align="center">
	<tr>
		<td>
			<?= montar_resumo_atividade( $atividade ) ?>
			<!-- NOVO ANEXO -->
			<?php if( $permissao ): ?>
				<form method="post" name="anexo" enctype="multipart/form-data">
					<input type="hidden" name="evento" value="cadastrar_anexo"/>
					<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 100%;">
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
							<td>
								<input type="file" name="arquivo"/>
								<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
							</td>
						</tr>
						<!--
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Tipo:</td>
							<td>
							<?php
								$sql = "select taaid as codigo, taadescricao as descricao from projetos.tipoanexoatividade order by taadescricao asc";
								$db->monta_combo( "taaid", $sql, 'S', '&nbsp;', '', '', '', 200, 'S' );
							?>
							</td>
						</tr>
						-->
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Tipo:</td>
							<td>
								<select style="width: 200px;" class="CampoEstilo" name="taaid">
									<optgroup label="Instrumento Legal">
										<?php
											$sql = sprintf( "select taaid, taadescricao from projetos.tipoanexoatividade where taalegal = true order by taadescricao asc" );
										?>
										<?php foreach( $db->carregar( $sql ) as $tipo ): ?>
										<option value="<?= $tipo['taaid'] ?>"><?= $tipo['taadescricao'] ?></option>
										<?php endforeach; ?>
									</optgroup>
									<optgroup label="Instrumento de Trabalho">
										<?php
											$sql = sprintf( "select taaid, taadescricao from projetos.tipoanexoatividade where taalegal = false order by taadescricao asc" );
										?>
										<?php foreach( $db->carregar( $sql ) as $tipo ): ?>
										<option value="<?= $tipo['taaid'] ?>"><?= $tipo['taadescricao'] ?></option>
										<?php endforeach; ?>
									</optgroup>
								</select>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
							<td><?= campo_textarea( 'anedescricao', 'S', 'S', '', 70, 2, 250 ); ?></td>
						</tr>
						<tr style="background-color: #cccccc">
							<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
							<td><input type="button" name="botao" value="Salvar" onclick="cadastrar_anexo();"/></td>
						</tr>
					</table>
				</form>
			<?php endif; ?>
			<!-- LISTA DE INSTRUMENTOS LEGAIS -->
			<?php
				$sql = sprintf(
					"select a.aneid, a.anedescricao, t.taadescricao, v.verid
					from projetos.anexoatividade a
					inner join projetos.tipoanexoatividade t on a.taaid = t.taaid
					inner join projetos.versaoanexoatividade v on a.verid = v.verid
					where anestatus='A' and atiid = %d and t.taalegal = true
					order by verdata desc",
					$_REQUEST['atiid']
				);
				$anexos = $db->carregar( $sql );
			?>
			<?php if( is_array( $anexos ) ): ?>
				<table class='tabela' style="width:100%;" cellpadding="3">
					<thead>
						<tr style="background-color: #e0e0e0">
							<td style="font-weight:bold; text-align:center; width:5%">&nbsp;</td>
							<td style="font-weight:bold; text-align:center; width:70%">Instrumento Legal</td>
							<td style="font-weight:bold; text-align:center; width:25%">Tipo</td>
						</tr>
					</thead>
					<tbody>
					<?php foreach( $anexos as $indice => $anexo ): ?>
						<?php $cor = $cor == '#fafafa' ? '#f0f0f0' : '#fafafa'; ?>
						<tr style="vertical-align:top; background-color: <?= $cor ?>" onmouseover="this.style.backgroundColor='#ffffcc';" onmouseout="this.style.backgroundColor='<?= $cor ?>';">
							<td style="text-align:center" nowrap="nowrap">
								<?php if( $permissao ): ?>
									<img align="absmiddle" src="../imagens/gif_inclui.gif" onmouseover="this.style.cursor='pointer'" onclick="exibirOcultarIncluirVersao( <?= $anexo['aneid'] ?> )" title="inserir arquivo"/>
									<img align="absmiddle" src="../imagens/excluir.gif" onmouseover="this.style.cursor='pointer'" onclick="excluirAnexo( <?= $anexo['aneid'] ?> );" title="excluir instrumento"/>
								<?php else: ?>
									<img align="absmiddle" src="../imagens/gif_inclui_d.gif"/>
									<img align="absmiddle" src="../imagens/excluir_01.gif"/>
								<?php endif; ?>
							</td>
							<td>
								<span onmouseover="this.style.cursor='pointer'" onclick="exibirOcultarHistoricoVersao( <?= $anexo['aneid'] ?> )">
									<?= $anexo['anedescricao'] ?>
								</span>
							</td>
							<td><?= $anexo['taadescricao'] ?></td>
						</tr>
						<tr id="anexo_<?= $anexo['aneid'] ?>" style="display:none; background-color: <? $cor ?>">
							<td style="text-align:center">
								<img src="../imagens/seta_filho.gif"/>
							</td>
							<td colspan="2" style="padding:5px;vertical-align:top;">
								<form method="post" enctype="multipart/form-data" id="formulario_<?= $anexo['aneid'] ?>">
									<input type="hidden" name="visao" value="<?= $_REQUEST['visao'] ?>"/>
									<input type="hidden" name="evento" value="cadastrar_versao"/>
									<input type="hidden" name="aneid" value="<?= $anexo['aneid'] ?>"/>
									<input type="file" name="arquivo"/>
									<input type="button" name="botao" value="Salvar" onclick="cadastrar_versao( document.getElementById( 'formulario_<?= $anexo['aneid'] ?>' ) )"/>
								</form>
							</td>
						</tr>
						<tr id="historico_<?= $anexo['aneid'] ?>" style="display:none; background-color: <?= $cor ?>">
							<td style="text-align:center">
								<img src="../imagens/seta_filho.gif"/>
							</td>
							<td colspan="2" style="padding:5px;vertical-align:top;">
								<?php
									$sql = sprintf(
										"select v.vernome, v.verid, v.verdata, u.usunome from projetos.anexoatividade a
										inner join projetos.versaoanexoatividade v on v.aneid = a.aneid
										inner join seguranca.usuario u on u.usucpf = v.usucpf
										where a.aneid = %d and a.anestatus = 'A'
										order by v.verdata desc",
										$anexo['aneid']
									);
									$versoes = $db->carregar( $sql );
								?>
								<?php foreach( $versoes as $versao ): ?>
									<p>
										<a href="?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&atiid=<?= $_REQUEST['atiid'] ?>&aba=<?= $_REQUEST['aba'] ?>&verid=<?= $versao['verid'] ?>&evento=download" title="arquivo anexado por <?= $versao['usunome'] ?> no dia <?= formata_data( $versao['verdata'] ) ?>">
											<img src="../imagens/salvar.png" style="border:0; vertical-align:middle;"/>
											&nbsp;<?= $versao['vernome'] ?>
										</a>
									</p>
								<?php endforeach; ?>
							</td>
						</tr>
					<?php endforeach; ?>
					</tbody>
				</table>
			<?php else: ?>
				<table class='tabela' style="width:100%;" cellpadding="3">
					<tbody>
						<td style="text-align:center;padding:15px;background-color:#f5f5f5;">
							A atividade não possui instrumentos legais.
						</td>
					</tbody>
				</table>
			<?php endif; ?>
			
			<!-- LISTA DE INSTRUMENTOS NÃO LEGAIS -->
			<?php
				$sql = sprintf(
					"select a.aneid, a.anedescricao, t.taadescricao, v.verid
					from projetos.anexoatividade a
					inner join projetos.tipoanexoatividade t on a.taaid = t.taaid
					inner join projetos.versaoanexoatividade v on a.verid = v.verid
					where anestatus='A' and atiid = %d and t.taalegal = false
					order by verdata desc",
					$_REQUEST['atiid']
				);
				$anexos = $db->carregar( $sql );
			?>
			<?php if( is_array( $anexos ) ): ?>
				<table class='tabela' style="width:100%;" cellpadding="3">
					<thead>
						<tr style="background-color: #e0e0e0">
							<td style="font-weight:bold; text-align:center; width:5%">&nbsp;</td>
							<td style="font-weight:bold; text-align:center; width:70%">Documento</td>
							<td style="font-weight:bold; text-align:center; width:25%">Tipo</td>
						</tr>
					</thead>
					<tbody>
					<?php foreach( $anexos as $indice => $anexo ): ?>
						<?php $cor = $cor == '#fafafa' ? '#f0f0f0' : '#fafafa'; ?>
						<tr style="vertical-align:top; background-color: <?= $cor ?>" onmouseover="this.style.backgroundColor='#ffffcc';" onmouseout="this.style.backgroundColor='<?= $cor ?>';">
							<td style="text-align:center" nowrap="nowrap">
								<?php if( $permissao ): ?>
									<img align="absmiddle" src="../imagens/gif_inclui.gif" onmouseover="this.style.cursor='pointer'" onclick="exibirOcultarIncluirVersao( <?= $anexo['aneid'] ?> )" title="inserir arquivo"/>
									<img align="absmiddle" src="../imagens/excluir.gif" onmouseover="this.style.cursor='pointer'" onclick="excluirAnexo( <?= $anexo['aneid'] ?> );" title="excluir instrumento"/>
								<?php else: ?>
									<img align="absmiddle" src="../imagens/gif_inclui_d.gif"/>
									<img align="absmiddle" src="../imagens/excluir_01.gif"/>
								<?php endif; ?>
							</td>
							<td>
								<span onmouseover="this.style.cursor='pointer'" onclick="exibirOcultarHistoricoVersao( <?= $anexo['aneid'] ?> )">
									<?= $anexo['anedescricao'] ?>
								</span>
							</td>
							<td><?= $anexo['taadescricao'] ?></td>
						</tr>
						<tr id="anexo_<?= $anexo['aneid'] ?>" style="display:none; background-color: <? $cor ?>">
							<td style="text-align:center">
								<img src="../imagens/seta_filho.gif"/>
							</td>
							<td colspan="2" style="padding:5px;vertical-align:top;">
								<form method="post" enctype="multipart/form-data" id="formulario_<?= $anexo['aneid'] ?>">
									<input type="hidden" name="visao" value="<?= $_REQUEST['visao'] ?>"/>
									<input type="hidden" name="evento" value="cadastrar_versao"/>
									<input type="hidden" name="aneid" value="<?= $anexo['aneid'] ?>"/>
									<input type="file" name="arquivo"/>
									<input type="button" name="botao" value="Salvar" onclick="cadastrar_versao( document.getElementById( 'formulario_<?= $anexo['aneid'] ?>' ) )"/>
								</form>
							</td>
						</tr>
						<tr id="historico_<?= $anexo['aneid'] ?>" style="display:none; background-color: <?= $cor ?>">
							<td style="text-align:center">
								<img src="../imagens/seta_filho.gif"/>
							</td>
							<td colspan="2" style="padding:5px;vertical-align:top;">
								<?php
									$sql = sprintf(
										"select v.vernome, v.verid, v.verdata, u.usunome from projetos.anexoatividade a
										inner join projetos.versaoanexoatividade v on v.aneid = a.aneid
										inner join seguranca.usuario u on u.usucpf = v.usucpf
										where a.aneid = %d and a.anestatus = 'A'
										order by v.verdata desc",
										$anexo['aneid']
									);
									$versoes = $db->carregar( $sql );
								?>
								<?php foreach( $versoes as $versao ): ?>
									<p>
										<a href="?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&atiid=<?= $_REQUEST['atiid'] ?>&aba=<?= $_REQUEST['aba'] ?>&verid=<?= $versao['verid'] ?>&evento=download" title="arquivo anexado por <?= $versao['usunome'] ?> no dia <?= formata_data( $versao['verdata'] ) ?>">
											<img src="../imagens/salvar.png" style="border:0; vertical-align:middle;"/>
											&nbsp;<?= $versao['vernome'] ?>
										</a>
									</p>
								<?php endforeach; ?>
							</td>
						</tr>
					<?php endforeach; ?>
					</tbody>
				</table>
			<?php else: ?>
				<table class='tabela' style="width:100%;" cellpadding="3">
					<tbody>
						<td style="text-align:center;padding:15px;background-color:#f5f5f5;">
							A atividade não possui instrumentos de trabalho.
						</td>
					</tbody>
				</table>
			<?php endif; ?>
		</td>
	</tr>
</table>