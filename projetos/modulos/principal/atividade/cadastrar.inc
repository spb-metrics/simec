<?php

include_once '_funcoes.inc';

// captura os dados do pai da atividade 
$sql = sprintf(
	"select * from projetos.atividade where atiid = %s",
	$_REQUEST['atiidpai']
);
$pai = $db->pegaLinha( $sql );
if ( !$pai ) {
	?>
	<script type="text/javascript">
		window.close();
	</script>
	<?php
}

// captura os dados submetidos
$atividade = array();
foreach( $pai as $atributo => $valor ) { # a tarefa pai é usada como modelo
	if ( array_key_exists( $atributo, $_REQUEST ) ) {
		$atividade[$atributo] = trim( $_REQUEST[$atributo] );
	}
}
$atividade['atiidpai'] = $pai['atiid'];

// persiste a inserção
if ( $_REQUEST['formulario'] ) {
	// identifica a ordem na qual o registro será inserido
	$sql = sprintf( "select count(*) as total from projetos.atividade where atiidpai = %s and atistatus = 'A'", $atividade['atiidpai'] );
	$atividade['atiordem'] = $db->pegaUm( $sql ) + 1;
	if ( !$atividade['atidatainicio'] ) {
		unset( $atividade['atidatainicio'] );
	}
	if ( !$atividade['atidatafim'] ) {
		unset( $atividade['atidatafim'] );
	}
	if ( !$atividade['atidataconclusao'] ) {
		unset( $atividade['atidataconclusao'] );
	}
	// insere
	$sql = sprintf(
		"insert into projetos.atividade ( %s ) values ( '%s' ) returning atiid",
		implode( ",", array_keys( $atividade ) ),
		implode( "','", $atividade )
	);
	$registro = array_pop( $db->carrega_tudo( $db->executar( $sql ) ) );
	$atiid = $registro['atiid'];
	if ( !$atiid ) {
		$db->rollback();
		fechar_popup( 'Ocorreu um erro durante o cadastro.\nA operação foi cancelada.' );
	} else {
		$db->commit();
//		fechar_popup( 'Atividade Cadastrada' );
		?>
			<script type="text/javascript">
				window.opener.filtrarListagem();
				location.href = '?modulo=principal/atividade/alterar&acao=A&atiid=<?= $atiid ?>';
			</script>
		<?php
	}
}

extract( $atividade );

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
	</head>
	<body onload="window.focus();">
		<?php monta_titulo( $titulo_modulo, '&nbsp;' ); ?>
		<form method="post" name="formulario">
			<input type="hidden" name="formulario" value="1"/>
			<input type="hidden" name="atiidpai" value="<?=  $pai['atiid'] ?>"/>
			<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
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
					<td><?= campo_textarea( 'atidescricao', 'N', 'S', '', 70, 3, 250 ); ?></td>
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
					<td align='right' class="SubTituloDireita" style="vertical-align:top">Data Prevista:</td>
					<td><?= campo_data( 'atidatafim', 'N', 'S', '', 'S' ); ?></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top">Situação:</td>
					<td>
					<?php
						$esaid = $esaid ? $esaid : 1; # coloca por padrão o status Não Iniciado
						$sql = "select e.esaid as codigo, e.esadescricao as descricao from projetos.estadoatividade e";
						$db->monta_combo( "esaid", $sql, 'S', 'Qualquer (selecione)', '', '' );
					?>
					</td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top">Andamento:</td>
					<td>
						<select name="atiporcentoexec" onchange="selecionarAndamento( this );">
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
						<input type="button" class="botao" name="botao" value="Cadastrar" onclick="enviar();">
						<input type="button" class="botao" name="fechar" value="Fechar" onclick="self.close();">
					</td>
				</tr>
			</table>
		</form>
	</body>
	<script type="text/javascript">
		
		function cancelar(){
		}
		
		function enviar(){
			if ( validar_formulario() ) {
//				window.opener.filtrarListagem();
				document.formulario.submit();
			}
		}
		
		function validar_formulario(){
			var validacao = true;
			var mensagem = 'Os seguintes campos estão inválidos:';
			document.formulario.atidetalhamento.value = trim( document.formulario.atidetalhamento.value );
			document.formulario.atimeta.value = trim( document.formulario.atimeta.value );
			document.formulario.atiinterface.value = trim( document.formulario.atiinterface.value );
			document.formulario.atidescricao.value = trim( document.formulario.atidescricao.value );
			document.formulario.atidatainicio.value = trim( document.formulario.atidatainicio.value );
			document.formulario.atidatafim.value = trim( document.formulario.atidatafim.value );
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
</html>
