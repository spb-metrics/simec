<?php

include_once '_funcoes.inc';

// identifica o tipo de edição
switch( $_REQUEST['visao'] ){
	case 'anexo':
		include 'alterar_anexo.inc';
		exit();
	case 'controle':
		include 'alterar_controle.inc';
		exit();
	case 'orcamento':
		include 'alterar_orcamento.inc';
		exit();
	default:
		break;
}

// utilizado para redirecionar 
$parametros = array(
	'atiid' => $_REQUEST['atiid'],
);

// captura os dados da atividade
$sql = sprintf(
	"select a.* from projetos.atividade a where atiid = %s",
	$_REQUEST['atiid']
);
$atividade_original = $db->pegaLinha( $sql );
$atividade = $atividade_original;
if ( !$atividade ) {
	dbg( 1, 1 ); # TODO: fechar
}
// captura os dados do pai da atividade 
if ( $atividade['atiidpai'] ) {
	$sql = sprintf(
		"select * from projetos.atividade where atiid = %s and atistatus = 'A'",
		$atividade['atiidpai']
	);
	$pai = $db->pegaLinha( $sql );
	if ( !$pai ) {
		dbg( 1, 1 ); # TODO: fechar
	}
}
// carrega para memória os dados da atividade
foreach( $atividade as $atributo => $valor ) {
	if ( array_key_exists( $atributo, $_REQUEST ) ) {
		$atividade[$atributo] = trim( $_REQUEST[$atributo] );
	}
}

if ( $_REQUEST['formulario'] ) {
	// filtra as datas
//	if ( !$atividade['atidatainicio'] ) {
//		unset( $atividade['atidatainicio'] );
//	}
//	if ( !$atividade['atidatafim'] ) {
//		unset( $atividade['atidatafim'] );
//	}
//	if ( !$atividade['atidataconclusao'] ) {
//		unset( $atividade['atidataconclusao'] );
//	}
	// atualiza os dados da atividade
	if ( $atividade['atiidpai'] != $atividade_original['atiidpai'] ) {
		$sql = sprintf( "select count(*) + 1 from projetos.atividade where atiidpai = %d and atistatus = 'A'", $atividade['atiidpai'] );
		$atividade['atiordem'] = $db->pegaUm( $sql );
	}
	$atribuicao = array();
	foreach ( $atividade as $campo => $valor ) {
		array_push( $atribuicao, sprintf( " %s = %s ", $campo, empty( $valor ) ? 'null' : "'".trim( $valor )."'" ) );
	}
	$atribuicao = implode( ',', $atribuicao );
	$sql = sprintf(
		"update projetos.atividade set %s where atiid = %d",
		$atribuicao,
		$atividade['atiid']
	);
//	dbg( $sql, 1 );
	if ( !$db->executar( $sql ) ) {
		$db->rollback();
		redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
	}
	// reordena as atividades que tem o mesmo pai
	if ( $atividade['atiidpai'] != $atividade_original['atiidpai'] ) {
		$sql = sprintf(
			"update projetos.atividade set atiordem = atiordem - 1 where atiidpai = %s and atiordem > %s and atistatus = 'A'",
			$atividade_original['atiidpai'],
			$atividade_original['atiordem']
		);
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
		}
	}
	$db->commit();
	recarregar_popup();
}

extract( $atividade );

$msg_importacao = '';
if ( $_REQUEST['importarmsproject'] )
{
	include APPRAIZ . 'includes/TarefaProject.php';
	$caminho = $_FILES['arquivo']['tmp_name'];
	if ( $caminho )
	{
		$arquivo = fopen( $caminho, 'r' );
		$tarefas = TarefaProject::lerCSV( $arquivo );
		foreach ( $tarefas as $tarefa )
		{
			$tarefa->importarParaPDE( $atiid );
		}
		$msg_importacao = 'Importação realizada com sucesso.';
	}
	else
	{
		$msg_importacao = 'Falha ao receber arquivo de importação.';
	}
}

if ( $_REQUEST['exportarmsproject'] )
{
	$nome = str_replace( ' ', '_', $atidescricao ) . '_' . date( 'd-m-Y-h-i-s' );
	include APPRAIZ . 'includes/TarefaProject.php';
	//header( 'Content-Type: text/x-csv;' );
	header( 'Content-type: text/comma-separated-values; charset=iso-8859-1' );
	header( 'Content-Disposition: attachment; filename=' . $nome . '.csv' );
	echo TarefaProject::exportarDoPDE( $atiid );
	exit();
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
		<script language="JavaScript" src="../includes/calendario.js"></script>
		<style type="text/css">
			.combo { width: 200px; }
		</style>
		<script type="text/javascript">
	
			function enviar(){
				if ( validar_formulario() ) {
					document.formulario.submit();
				}
			}
	
			function validar_formulario(){
				var validacao = true;
				var mensagem = 'Os seguintes campos não foram preenchidos:';
				document.formulario.atidetalhamento.value = trim( document.formulario.atidetalhamento.value );
				document.formulario.atiidpai.value = trim( document.formulario.atiidpai.value );
				document.formulario.atimeta.value = trim( document.formulario.atimeta.value );
				document.formulario.atiinterface.value = trim( document.formulario.atiinterface.value );
				document.formulario.atidescricao.value = trim( document.formulario.atidescricao.value );
				if ( document.formulario.atiidpai.value == '' ) {
					mensagem += '\nAtividade Pai';
					validacao = false;
				}
				if ( document.formulario.atidescricao.value == '' ) {
					mensagem += '\nDescrição';
					validacao = false;
				}				
				if ( !validacao ) {
					alert( mensagem );
				}
				return validacao;
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
			
			function exibirEscondeItem( id, img ){
				var src = img.src.substring( img.src.length - 6 );
				img.src = src == 'is.gif' ? '../imagens/menos.gif' : '../imagens/mais.gif';
				var elemento = document.getElementById( id );
				elemento.style.display = elemento.style.display == 'inline' ? 'none' : 'inline'; 
			}
			
			function selecionarAndamento( valor ){
				if ( valor == '0' ) {
					// TODO: alterar andamento e data de conclusão
				} else if ( valor == '100' ) {
					// TODO: alterar andamento e data de conclusão
				}
			}
			
			function selecionarSituacao( valor ){
			}
			
		</script>
	</head>
	<body onload="window.focus();">
		<?php if ( $msg_importacao ) : ?>
			<script type="text/javascript">
				alert( '<?php echo $msg_importacao; ?>' );
			</script>
		<?php endif; ?>
		<?php monta_titulo( 'Editar Atividade', '&nbsp;' ); ?>
		<form method="post" name="formulario" enctype="multipart/form-data">
			<input type="hidden" name="formulario" value="1"/>
			<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:none;">
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top">Atividade Pai:</td>
					<td>
						<?php
//							$sql = sprintf( "select atiid as codigo, atidescricao as descricao from projetos.atividade where atiid <> %d and atistatus = 'A'", $atiid );
//							$db->monta_combo( 'atiidpai', $sql, 'S', '&nbsp;', '', '', '', '300', 'S' );
						?>
						<input type="hidden" name="atiidpai" value="<?= $atiidpai ?>"/>
						<?php
							$sql = sprintf( "select atiid, atidescricao from projetos.atividade where atiid = %d", $atiidpai );
							$atividadepai = $db->pegaLinha( $sql );
							if ( $atividadepai['atiid'] == 3 ) { 
								echo $atividadepai['atidescricao'];
							} else {
								echo "<img src='../imagens/seta_pai.png' style='float: left; margin: 0 5px 0 0;'/><a href='projetos.php?modulo=principal/atividade/alterar&acao=A&atiid=". $atividadepai['atiid'] ."'>". $atividadepai['atidescricao'] ."</a>";
							}
						?>
					</td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top">Título:</td>
					<td><?= campo_textarea( 'atidescricao', 'N', 'S', '', 70, 3, 500 ); ?></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top">Detalhamento:</td>
					<td><?= campo_textarea( 'atidetalhamento', 'N', 'S', '', 70, 3, '' ); ?></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top">Data Início:</td>
					<td><?= campo_data( 'atidatainicio', 'N', 'S', '', 'S' ); ?></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top">Data Prevista Finalização:</td>
					<td><?= campo_data( 'atidatafim', 'N', 'S', '', 'S' ); ?></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top">Situação:</td>
					<td>
					<?php
						$sql = "select e.esaid as codigo, e.esadescricao as descricao from projetos.estadoatividade e";
						$db->monta_combo( "esaid", $sql, 'S', '', 'selecionarSituacao', '' );
					?>
					</td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top">Andamento:</td>
					<td>
						<select name="atiporcentoexec" onchange="selecionarAndamento( this.value );">
							<option <?= $atiporcentoexec == 0 ? 'selected' : '' ?> value="0">0%</option>
							<option <?= $atiporcentoexec == 10 ? 'selected' : '' ?> value="10">10%</option>
							<option <?= $atiporcentoexec == 20 ? 'selected' : '' ?> value="20">20%</option>
							<option <?= $atiporcentoexec == 30 ? 'selected' : '' ?> value="30">30%</option>
							<option <?= $atiporcentoexec == 40 ? 'selected' : '' ?> value="40">40%</option>
							<option <?= $atiporcentoexec == 50 ? 'selected' : '' ?> value="50">50%</option>
							<option <?= $atiporcentoexec == 60 ? 'selected' : '' ?> value="60">60%</option>
							<option <?= $atiporcentoexec == 70 ? 'selected' : '' ?> value="70">70%</option>
							<option <?= $atiporcentoexec == 80 ? 'selected' : '' ?> value="80">80%</option>
							<option <?= $atiporcentoexec == 90 ? 'selected' : '' ?> value="90">90%</option>
							<option <?= $atiporcentoexec == 100 ? 'selected' : '' ?> value="100">100%</option>
						</select>
					</td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top">Data de Conclusão:</td>
					<td><?= campo_data( 'atidataconclusao', 'N', 'S', '', 'S' ); ?></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top">Meta:</td>
					<td><?= campo_textarea( 'atimeta', 'N', 'S', '', 70, 3, 250 ); ?></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top">Órgãos Participantes:</td>
					<td><?= campo_textarea( 'atiinterface', 'N', 'S', '', 70, 3, 250 ); ?></td>
				</tr>
				<tr bgcolor="#C0C0C0">
						<td width="20%">&nbsp;</td>
						<td>
							<input type="button" class="botao" name="botao" value="Alterar" onclick="enviar();">
							<input type="button" class="botao" name="fechar" value="Fechar" onclick="self.close();">
						</td>
				</tr>
			</table>
		</form>
		
		<!-- LISTA DE ATIVIDADES FILHAS -->
		<?php
			$sql = sprintf(
				"select a.atiordem, a.atiid, a.atidescricao, a.atidatainicio, a.atidatafim
				from projetos.atividade a
				where a.atiidpai = %d and a.atistatus = 'A'
				order by atiordem",
				$_REQUEST['atiid']
			);
			$filhas = $db->carregar( $sql );
		?>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-top:none;">
			<tr>
				<td colspan="2" bgcolor="#cdcdcd">
					<img src="../imagens/mais.gif" onclick="exibirEscondeItem( 'atitivadesfilhas', this );"/>
					<b>Atividades Filhas</b>
				</td>
			</tr>
			<tr>
				<td colspan="2" height="0">
					<div id="atitivadesfilhas" style="padding:10px;display:block;">
						<?php if( $filhas ): ?>
							<table class='listagem' style="width:100%">
								<thead>
									<tr>
										<td style="width:70%">Descrição</td>
										<td style="width:15%">Data Início</td>
										<td style="width:15%">Data Fim</td>
									</tr>
								</thead>
								<tbody>
								<?php foreach( $filhas as $indice => $filha ): ?>
									<tr style="vertical-align:top; background-color: <?= $indice % 2 ? '#e0e0e0' : '#f4f4f4' ?>">
										<td style="padding:5px;">
											<a href="?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&visao=editar&atiid=<?= $filha['atiid'] ?>&evento=download">
												<?= $filha['atiordem'] ?> - <?= $filha['atidescricao'] ?>
											</a>
										</td>
										<td style="padding:5px;"><?= formata_data( $filha['atidatainicio'] ) ?></td>
										<td style="padding:5px;"><?= formata_data( $filha['atidatafim'] ) ?></td>
									</tr>
								<?php endforeach; ?>
								</tbody>
							</table>
						<?php else: ?>
							<p style="text-align: center">Não há atividades filhas cadastradas</p>
						<?php endif; ?>
						<p style="width: 100%; text-align: center">
						<a href="?modulo=principal/atividade/cadastrar&acao=C&atiidpai=<?= $_REQUEST['atiid'] ?>" style="text-align: center">
							<img border="0" src="../imagens/gif_inclui.gif" style="vertical-align:middle;"/>
							Cadastrar Nova Atividade
						</a>
						</p>
					</div>
				</td>
			</tr>
		</table>
		
		
		<!-- LISTA DE ANEXOS -->
		<?php
			$sql = sprintf(
				"select a.aneid, a.anedescricao, t.taadescricao, a.verid
				from projetos.anexoatividade a
				inner join projetos.tipoanexoatividade t on a.taaid = t.taaid
				where a.anestatus='A' and a.atiid = %d
				order by a.taaid, a.anedescricao",
				$_REQUEST['atiid']
			);
			$anexos = $db->carregar( $sql );
		?>
		<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-top:none;">
			<tr>
				<td colspan="2" bgcolor="#cdcdcd">
					<img src="../imagens/mais.gif" onclick="exibirEscondeItem( 'anexos', this );"/>
					<a href="?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&visao=anexo&atiid=<?= $_REQUEST['atiid'] ?>"><b>Instrumento</b></a>
				</td>
			</tr>
			<tr>
				<td colspan="2" height="0">
					<div id="anexos" style="padding:10px;display:none;">
						<?php if( is_array( $anexos ) ): ?>
							<table class='listagem' style="width:100%">
								<thead>
									<tr>
										<td style="width:70%">Descrição</td>
										<td style="width:30%">Tipo</td>
									</tr>
								</thead>
								<tbody>
								<?php foreach( $anexos as $indice => $anexo ): ?>
									<tr style="vertical-align:top; background-color: <?= $indice % 2 ? '#e0e0e0' : '#f4f4f4' ?>">
										<td style="padding:5px;">
											<a href="?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&visao=anexo&verid=<?= $anexo['verid'] ?>&evento=download">
												<img src="../imagens/salvar.png" style="border:0; vertical-align:middle;"/>&nbsp;<?= $anexo['anedescricao'] ?>
											</a>
										</td>
										<td style="padding:5px;"><?= $anexo['taadescricao'] ?></td>
									</tr>
								<?php endforeach; ?>
								</tbody>
							</table>
						<?php endif; ?>
					</div>
				</td>
			</tr>
		</table>
		
		<!-- LISTA DE CONTROLES -->
		<?php
			$sql = sprintf(
				"select o.obsid, o.obsdata, o.obsdescricao, u.usunome, u.usucpf
				from projetos.observacaoatividade o
				inner join seguranca.usuario u on u.usucpf = o.usucpf
				where o.atiid = %d and o.obsstatus = 'A'
				order by o.obsdata desc",
				$_REQUEST['atiid']
			);
			$controles = $db->carregar( $sql );
		?>
		<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-top:none;">
			<tr>
				<td colspan="2" bgcolor="#cdcdcd">
					<img src="../imagens/mais.gif" onclick="exibirEscondeItem( 'controles', this );"/>
					<a href="?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&visao=controle&atiid=<?= $_REQUEST['atiid'] ?>"><b>Observações</b></a>
				</td>
			</tr>
			<tr>
				<td colspan="2" height="0">
					<div id="controles" style="padding:10px;display:none;">
						<?php if( is_array( $controles ) ): ?>
							<table class='listagem' style="width:100%">
								<thead>
									<tr>
										<td style="width:20%">Data</td>
										<td style="width:50%">Descrição</td>
										<td style="width:30%">Usuário</td>
									</tr>
								</thead>
								<tbody>
								<?php foreach( $controles as $indice => $controle ): ?>
									<tr style="vertical-align:top; background-color: <?= $indice % 2 ? '#e0e0e0' : '#f4f4f4' ?>">
										<td style="padding:5px; text-align:center"><?= formata_data( $controle['obsdata'] ) ?></td>
										<td style="padding:5px;"><?= $controle['obsdescricao'] ?></td>
										<td style="padding:5px;<?php if( $controle['usucpf'] == $_SESSION['usucpforigem'] ): ?>font-weight:bold<?php endif; ?>"><?= $controle['usunome'] ?></td>
									</tr>
								<?php endforeach; ?>
								</tbody>
							</table>
						<?php endif; ?>
					</div>
				</td>
			</tr>
		</table>
		
		<!-- LISTA DE ORCAMENTOS -->
		<?php
			$sql = sprintf(
				"select o.orcid, o.orcano, o.orcvalor
				from projetos.orcamentoatividade o
				where o.atiid = %d
				order by o.orcano desc",
				$_REQUEST['atiid']
			);
			$orcamentos = $db->carregar( $sql );
		?>
		<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-top:none;">
			<tr>
				<td colspan="2" bgcolor="#cdcdcd">
					<img src="../imagens/mais.gif" onclick="exibirEscondeItem( 'orcamentos', this );"/>
					<a href="?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&visao=orcamento&atiid=<?= $_REQUEST['atiid'] ?>"><b>Orcamento</b></a>
				</td>
			</tr>
			<tr>
				<td colspan="2" height="0">
					<div id="orcamentos" style="padding:10px;display:none;">
						<?php if( is_array( $orcamentos ) ): ?>
							<table class='listagem' style="width:100%">
								<thead>
									<tr>
										<td style="width:20%">Ano</td>
										<td style="width:80%">Valor</td>
									</tr>
								</thead>
								<tbody>
								<?php foreach( $orcamentos as $indice => $orcamento ): ?>
									<tr style="vertical-align:top; background-color: <?= $indice % 2 ? '#e0e0e0' : '#f4f4f4' ?>">
										<td style="padding:5px; text-align:center"><?= $orcamento['orcano'] ?></td>
										<td style="padding:5px; text-align:right">R$ <?= number_format( $orcamento['orcvalor'], 2, ',', '.' ) ?></td>
									</tr>
								<?php endforeach; ?>
								</tbody>
							</table>
						<?php endif; ?>
					</div>
				</td>
			</tr>
		</table>
		
		<!-- LISTA DE ORCAMENTOS -->
		<?php
			$sql = sprintf(
				"select o.orcid, o.orcano, o.orcvalor
				from projetos.orcamentoatividade o
				where o.atiid = %d
				order by o.orcano desc",
				$_REQUEST['atiid']
			);
			$orcamentos = $db->carregar( $sql );
		?>
		<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-top:none;">
			<tr>
				<td colspan="2" bgcolor="#cdcdcd">
					<img src="../imagens/mais.gif" onclick="exibirEscondeItem( 'msprojet', this );"/>
					<b>MicrosoftProject</b>
				</td>
			</tr>
			<tr>
				<td colspan="2" height="0">
					<div id="msprojet" style="padding:10px;display:none;">
						<script type="text/javascript">
						
							function exportarMicrosoftProject()
							{
								url = '?modulo=principal/atividade/alterar&acao=A&atiid=<?php echo $_REQUEST['atiid']; ?>&exportarmsproject=1';
								window.location.href = url, 'Exportar';
							}
						
						</script>
						<form name="importarmsproject" method="post" action="" enctype="multipart/form-data">
							<input type="hidden" name="importarmsproject" value="1"/>
							<input type="hidden" name="atiid" value="<?php echo $_REQUEST['atiid']; ?>"/>
							<input type="file" name="arquivo"/>
							<input type="submit" name="importar" value="Importar"/>
							<input type="button" name="exportar" value="Exportar" onclick="exportarMicrosoftProject();"/>
						</form>
					</div>
				</td>
			</tr>
		</table>
		
	</body>
</html>
