<?php

// utilizado para redirecionar 
$parametros = array(
	'visao' => $_REQUEST['visao'],
	'atiid' => $_REQUEST['atiid'],
);

switch( $_REQUEST['evento'] ){

	case 'cadastrar_anexo':
		// identifica a atividade
		$sql = sprintf( "select * from projetos.atividade where atiid = %d and atistatus = 'A'", $_REQUEST['atiid'] );
		$atividade = $db->pegaLinha( $sql );
		if ( !$atividade ) {
			$db->rollback();
			recarregar_popup();
		}
		// cadastra o anexo
		$sql = sprintf(
			"insert into projetos.anexoatividade ( anedescricao, atiid, taaid ) values ( '%s', %d, %d ) returning aneid",
			$_REQUEST['anedescricao'],
			$_REQUEST['atiid'],
			$_REQUEST['taaid']
		);
		$registro = array_pop( $db->carrega_tudo( $db->executar( $sql ) ) );
		$aneid = $registro['aneid'];
//		$aneid = $db->pegaUm( $sql );
		if ( !$aneid ) {
			$db->rollback();
			recarregar_popup();
		}
		// obtém o arquivo
		$arquivo = $_FILES['arquivo'];
		if ( !is_uploaded_file( $arquivo['tmp_name'] ) ) {
			recarregar_popup();
		}
		// cadastra a versão
		$sql = sprintf(
			"insert into projetos.versaoanexoatividade ( aneid, usucpf, vernome, vertipomime ) values ( %d, '%s', '%s', '%s' ) returning verid",
			$aneid,
			$_SESSION['usucpforigem'],
			$arquivo['name'],
			$arquivo['type']
		);
		$registro = array_pop( $db->carrega_tudo( $db->executar( $sql ) ) );
		$verid = $registro['verid'];
//		$verid = $db->pegaUm( $sql );
		if ( !$verid ) {
			$db->rollback();
			recarregar_popup();
		}
		// torna ativa a nova versão cadastrada
		$sql = sprintf(
			"update projetos.anexoatividade set verid = %d where aneid = %d",
			$verid,
			$aneid
		);
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			recarregar_popup();
		}
		// move o arquivo para o diretório final
		$caminho = APPRAIZ . 'arquivos/projetos/'. $verid;
		if ( !move_uploaded_file( $arquivo['tmp_name'], $caminho ) ) {
			$db->rollback();
			recarregar_popup();
		}
		$db->commit();
		recarregar_popup();
		break;

	case 'cadastrar_versao':
		// obtém o arquivo
		$arquivo = $_FILES['arquivo'];
		if ( !is_uploaded_file( $arquivo['tmp_name'] ) ) {
			recarregar_popup();
		}
		// identifica o anexo
		$sql = sprintf( "select * from projetos.anexoatividade where aneid = %d and anestatus = 'A'", $_REQUEST['aneid'] );
		$anexo = $db->pegaLinha( $sql );
		if ( !$anexo ) {
			$db->rollback();
			recarregar_popup();
		}
		// identifica a atividade
		$sql = sprintf( "select * from projetos.atividade where atiid = %d and atistatus = 'A'", $anexo['atiid'] );
		$atividade = $db->pegaLinha( $sql );
		if ( !$atividade ) {
			$db->rollback();
			recarregar_popup();
		}
		// cadastra a versão
		$sql = sprintf(
			"insert into projetos.versaoanexoatividade ( aneid, usucpf, vernome, vertipomime ) values ( %d, '%s', '%s', '%s' ) returning verid",
			$_REQUEST['aneid'],
			$_SESSION['usucpforigem'],
			$arquivo['name'],
			$arquivo['type']
		);
		$registro = array_pop( $db->carrega_tudo( $db->executar( $sql ) ) );
		$verid = $registro['verid'];
		if ( !$verid ) {
			$db->rollback();
			recarregar_popup();
		}
		// torna ativa a nova versão cadastrada
		$sql = sprintf(
			"update projetos.anexoatividade set verid = %d where aneid = %d",
			$verid,
			$_REQUEST['aneid']
		);
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			recarregar_popup();
		}
		// move o arquivo para o diretório final
		$caminho = APPRAIZ . 'arquivos/projetos/'. $verid;
		if ( !move_uploaded_file( $arquivo['tmp_name'], $caminho ) ) {
			$db->rollback();
			recarregar_popup();
		}
		$db->commit();
		recarregar_popup();
		break;

	case 'excluir_anexo':
		$sql = sprintf( "update projetos.anexoatividade set anestatus = 'I' where aneid = %d", $_REQUEST['aneid'] );
		if( !$db->executar( $sql ) ){
			$db->rollback();
		} else {
			$db->commit();
		}
		recarregar_popup();
		break;

	case 'download':
		$sql = sprintf(
			"select v.verid, v.vernome, v.vertipomime, a.atiid from projetos.versaoanexoatividade v inner join projetos.anexoatividade a on a.aneid = v.aneid where v.verid = %d",
			$_REQUEST['verid']
		);
		$versao = $db->pegaLinha( $sql );
		$caminho = APPRAIZ . 'arquivos/projetos/'. $versao['verid'];
		if ( !is_readable( $caminho ) ) {
			$_REQUEST['atiid'] = $versao['atiid'];
			recarregar_popup();
		}
		header( 'Content-type: '. $versao['vertipomime'] );
		header( 'Content-Disposition: attachment; filename=' . $versao['vernome'] );
		readfile( $caminho );
		exit();

	default:
		break;

}

?>
<html>
	<head>
		<title>SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação</title>
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Expires" content="-1">
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<style type="text/css">
			.combo { width: 200px; }
		</style>
		<script type="text/javascript">
			
			function cadastrar_anexo(){
				if ( validar_formulario_anexo() ) {
					document.anexo.submit();
				}
			}
			
			function cadastrar_versao( formulario ){
				if ( formulario.arquivo.value == '' ) {
					return;
				}
				formulario.submit();
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
					window.location = '?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&atiid=<?= $_REQUEST['atiid'] ?>&visao=<?= $_REQUEST['visao'] ?>&evento=excluir_anexo&aneid='+ anexo;
				}
			}
			
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
	</head>
	<body onload="window.focus();">
		<?php monta_titulo( $titulo_modulo, '&nbsp;' ); ?>
		
		<!-- ATIVIDADE -->
		<?php
			$sql = sprintf( "select atidescricao, atidatainicio, atidatafim from projetos.atividade where atiid = %d", $_REQUEST['atiid'] );
			$atividade = $db->pegaLinha( $sql );
		?>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td colspan="2" bgcolor="#cdcdcd"><a href="?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&atiid=<?= $_REQUEST['atiid'] ?>"><b>Atividade</b></a></td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
				<td><?= $atividade['atidescricao'] ?></td>
			</tr>
			<?php if( $atividade['atidatainicio'] && $atividade['atidatafim'] ): ?>
			<tr>
				<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Período:</td>
				<td><?= formata_data( $atividade['atidatainicio'] ) ?> à <?= formata_data( $atividade['atidatafim'] ) ?></td>
			</tr>
			<?php endif; ?>
		</table>
		
		<!-- NOVO ANEXO -->
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="margin-top:20px">
			<form method="post" name="anexo" enctype="multipart/form-data">
				<input type="hidden" name="visao" value="<?= $_REQUEST['visao'] ?>"/>
				<input type="hidden" name="evento" value="cadastrar_anexo"/>
				<input type="hidden" name="atiid" value="<?= $_REQUEST['atiid'] ?>"/>
				<tr>
					<td colspan="2" bgcolor="#cdcdcd"><b>Novo Instrumento</b></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Tipo:</td>
					<td>
					<?php
						$sql = "select taaid as codigo, taadescricao as descricao from projetos.tipoanexoatividade order by taadescricao asc";
						$db->monta_combo( "taaid", $sql, 'S', "Selecione o tipo do arquivo", '', '', 'Selecione o tipo do arquivo', 200, 'S' );
					?>
					</td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
					<td><?= campo_textarea( 'anedescricao', 'S', 'S', '', 70, 2, 250 ); ?></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
					<td><input type="file" name="arquivo"/></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">&nbsp;</td>
					<td>
						<input type="button" name="botao" value="Enviar" onclick="cadastrar_anexo();"/>
						<input type="button" name="fechar" value="Fechar" onclick="self.close();">
					</td>
				</tr>
			</form>
		</table>
		
		<!-- LISTA DE ANEXOS -->
		<?php
			$sql = sprintf(
				"select a.aneid, a.anedescricao, t.taadescricao, v.verid
				from projetos.anexoatividade a
				inner join projetos.tipoanexoatividade t on a.taaid = t.taaid
				inner join projetos.versaoanexoatividade v on a.verid = v.verid
				where anestatus='A' and atiid = %d
				order by verdata desc",
				$_REQUEST['atiid']
			);
//			dbg( $sql, 1 );
			$anexos = $db->carregar( $sql );
		?>
		<?php if( is_array( $anexos ) ): ?>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="margin-top:20px">
			<tr>
				<td colspan="2" bgcolor="#cdcdcd"><b>Instrumentos</b></td>
			</tr>
			<tr>
				<td colspan="2" style="padding:10px">
					<table class='listagem' style="width:100%">
						<thead>
							<tr>
								<td style="width:15%">&nbsp;</td>
								<td style="width:60%">Descrição</td>
								<td style="width:25%">Tipo</td>
							</tr>
						</thead>
						<tbody>
						<?php foreach( $anexos as $indice => $anexo ): ?>
							<tr style="vertical-align:top; background-color: <?= $indice % 2 ? '#e0e0e0' : '#f4f4f4' ?>">
								<td style="padding:5px; text-align:center" nowrap="nowrap">
									<img src="../imagens/consultar.gif" onmouseover="this.style.cursor='pointer'" onclick="exibirOcultarHistoricoVersao( <?= $anexo['aneid'] ?> )" title=""/>
									<img src="../imagens/gif_inclui.gif" onmouseover="this.style.cursor='pointer'" onclick="exibirOcultarIncluirVersao( <?= $anexo['aneid'] ?> )" title=""/>
									<img src="../imagens/excluir.gif" onmouseover="this.style.cursor='pointer'" onclick="excluirAnexo( <?= $anexo['aneid'] ?> );" title="excluir instrumento"/>
								</td>
								<td style="padding:5px;">
									<a href="?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&visao=<?= $_REQUEST['visao'] ?>&verid=<?= $anexo['verid'] ?>&evento=download">
										<?= $anexo['anedescricao'] ?>
									</a>
								</td>
								<td style="padding:5px;"><?= $anexo['taadescricao'] ?></td>
							</tr>
							<tr id="anexo_<?= $anexo['aneid'] ?>" style="display:none; background-color: <?= $indice % 2 ? '#e0e0e0' : '#f4f4f4' ?>">
								<td style="text-align:center">
									<img src="../imagens/seta_filho.gif"/>
								</td>
								<td colspan="2" style="padding:10px">
									<form method="post" enctype="multipart/form-data" id="formulario_<?= $anexo['aneid'] ?>">
										<input type="hidden" name="visao" value="<?= $_REQUEST['visao'] ?>"/>
										<input type="hidden" name="evento" value="cadastrar_versao"/>
										<input type="hidden" name="aneid" value="<?= $anexo['aneid'] ?>"/>
										<input type="file" name="arquivo"/>
										<input type="button" name="botao" value="enviar" onclick="cadastrar_versao( document.getElementById( 'formulario_<?= $anexo['aneid'] ?>' ) )"/>
									</form>
								</td>
							</tr>
							<tr id="historico_<?= $anexo['aneid'] ?>" style="display:none; background-color: <?= $indice % 2 ? '#e0e0e0' : '#f4f4f4' ?>">
								<td style="text-align:center">
									<img src="../imagens/seta_filho.gif"/>
								</td>
								<td colspan="2" style="padding:5px">
									<?php
										$sql = sprintf(
											"select v.verid, v.verdata, u.usunome from projetos.anexoatividade a
											inner join projetos.versaoanexoatividade v on v.aneid = a.aneid
											inner join seguranca.usuario u on u.usucpf = v.usucpf
											where a.aneid = %d and a.anestatus = 'A'
											order by v.verdata desc",
											$anexo['aneid']
										);
										$versoes = $db->carregar( $sql );
									?>
									<?php foreach( $versoes as $versao ): ?>
										<p><a href="?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&visao=<?= $_REQUEST['visao'] ?>&verid=<?= $versao['verid'] ?>&evento=download"><img src="../imagens/salvar.png" style="border:0; vertical-align:middle;"/>&nbsp;<?= formata_data( $versao['verdata'] ) ?> por <?= $versao['usunome'] ?></a></p>
									<?php endforeach; ?>
								</td>
							</tr>
						<?php endforeach; ?>
						</tbody>
					</table>
				</td>
			</tr>
		</table>
		<?php endif; ?>
	</body>
</html>