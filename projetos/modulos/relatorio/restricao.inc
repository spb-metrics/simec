<?php

// VERIFICA SE PROJETO ESTÁ SELECIONADO
projeto_verifica_selecionado();

$sqlComboAtividades = "
	select
		da.profundidade,
		da.atiid,
		da.numero,
		a.atidescricao
	from projetos.f_dadostodasatividades() da
		inner join projetos.atividade a on
			a.atiid = da.atiid
	where
		da.profundidade < 4 and
		da.projeto = " . PROJETO . " 
	order by
		da.ordem
";
$atividades = $db->carregar( $sqlComboAtividades );
$atividades = $atividades ? $atividades : array();

require_once APPRAIZ . "includes/cabecalho.inc";
echo "<br/>"; 
montar_titulo_projeto();

?>
<script type="text/javascript">
	
	var resultado_popup = null;
	
	function submeterFormulario()
	{
		var form = document.formulario;
		form.submit();
	}
	
	function enviar_email( cpf ){
		var nome_janela = 'janela_enviar_emai_' + cpf;
		window.open(
			'/geral/envia_email.php?cpf=' + cpf,
			nome_janela,
			'width=650,height=557,scrollbars=yes,scrolling=yes,resizebled=yes'
		);
	}

</script>
<form action="" name="formulario" method="post">
	<input type="hidden" name="submetido" value="1"/>
	<table align="center" border="0" cellspacing="1" cellpadding="3" class="tabela" bgcolor="#f5f5f5">
		<tr>
			<td align="right" width="20%" class="SubTituloDireita">
				<b>A partir de</b>
			</td>
			<td>
				<select id="atividade" name="atividade" class="CampoEstilo" style="width: 250px;">
					<option value="">
						<?php
							$sql = "select atidescricao from projetos.atividade where atiid = " . PROJETO;
							echo $db->pegaUm( $sql );
						?>
					</option>
					<?php foreach ( $atividades as $item ) : ?>
						<option
							value="<?=  $item['atiid'] ?>"
							<?= $item['atiid'] == $_REQUEST['atividade'] ? 'selected="selected"' : '' ?>
						>
							<?= str_repeat( '&nbsp;', $item['profundidade'] * 5 ) ?>
							<?=  $item['numero'] ?>
							<?=  $item['atidescricao'] ?>
						</option>
					<?php endforeach; ?>
				</select>
			</td>
		</tr>
		<tr>
			<td align="right" width="20%" class="SubTituloDireita">
				<b>Tipo</b>
			</td>
			<td>
				<input
					type="radio"
					name="tipo"
					id="tipo_superada"
					value="superada"
					<?= $_REQUEST['tipo'] == 'superada' ? 'checked="checked"' : '' ?>
				/>
				<label for="tipo_superada">Superadas</label>
				
				&nbsp;&nbsp;&nbsp;&nbsp;
				
				<input
					type="radio"
					name="tipo"
					id="tipo_naosuperada"
					value="naosuperada"
					<?= $_REQUEST['tipo'] == 'naosuperada' ? 'checked="checked"' : '' ?>
				/>
				<label for="tipo_naosuperada">Não superadas</label>
				
				&nbsp;&nbsp;&nbsp;&nbsp;
				
				<input
					type="radio"
					name="tipo"
					id="tipo_todas"
					value="todas"
					<?= !$_REQUEST['tipo'] || $_REQUEST['tipo'] == 'todas' ? 'checked="checked"' : '' ?>
				/>
				<label for="tipo_todas">Todas</label>
			</td>
		</tr>
		<tr>
			<td align="right" width="20%" class="SubTituloDireita">
				&nbsp;
			</td>
			<td bgcolor="#dcdcdc">
				<input type="button" name="submeter" value="Visualizar" onclick="submeterFormulario();"/>
			</td>
		</tr>
	</table>
</form>
<?php

if ( !isset( $_REQUEST['submetido'] ) )
{
	return;
}

?>
<!-- LISTA DE RESTRIÇÕES -->
<?php
	//$filtro = $_REQUEST['filtro'] ? ' and o.obssolucao = false ' : '';
	//$filtro = ' and o.obssolucao = false ';
	
	$filtro = '';
	
	switch ( $_REQUEST['tipo'] )
	{
		case 'superada':
			$filtro = ' and o.obssolucao = true ';
			break;
		case 'naosuperada':
			$filtro = ' and o.obssolucao = false ';
			break;
		case 'todas':
		default:
			break;
	}
	
	$filtro .= ' and a._atiprojeto = ' . PROJETO . ' ';
	
	if ( $_REQUEST['atividade'] )
	{
		$atiid = $_REQUEST['atividade'];
		$sql = "select _atinumero from projetos.atividade where atiid = " . $atiid;
		$numero = $db->pegaUm( $sql );
		$filtro .= " and a._atinumero like '" . $numero . ".%' ";
	}
	
	$sql = sprintf(
		"select
			
			a.atiid,
			a._atinumero,
			a.atiporcentoexec,
			a.atidatainicio,
			a.atidatafim,
			a.esaid,
			ea.esadescricao,
			
			o.obsid, o.obsdata, o.obsdescricao,
			o.obssolucao, o.obsdatasolucao, o.obsmedida,
			
			autor.usucpf as cpfautor, autor.usunome as nomeautor,
			autor.usufoneddd as dddautor, autor.usufonenum as telefoneautor,

			unidadeautor.unidsc as unidadeautor,
			
			responsavel.usucpf as cpfresponsavel,
			responsavel.usunome as nomeresponsavel,
			responsavel.usufoneddd as dddresponsavel,
			responsavel.usufonenum as telefoneresponsavel,
			unidaderesponsavel.unidsc as unidaderesponsavel

		from projetos.observacaoatividade o
			left join seguranca.usuario autor on
				autor.usucpf = o.usucpf
			left join public.unidade unidadeautor on
				unidadeautor.unicod = autor.unicod
			left join seguranca.usuario responsavel on
				responsavel.usucpf = o.usucpfsolucao
			left join public.unidade unidaderesponsavel on
				unidaderesponsavel.unicod = responsavel.unicod
			inner join projetos.atividade a on
				a.atiid = o.atiid
			inner join projetos.estadoatividade ea on
				ea.esaid = a.esaid

		where
			o.obsstatus = 'A' and
			a.atistatus = 'A'
			%s
		order by
			a._atiordem",
		$filtro
	);
	$restricoes_temp = $db->carregar( $sql );
	$restricoes_temp = $restricoes_temp ? $restricoes_temp : array();
	
	$restricoesAtividade = array();
	foreach ( $restricoes_temp as $item )
	{
		if ( !array_key_exists( $item['_atinumero'], $restricoesAtividade ) )
		{
			$restricoesAtividade[$item['_atinumero']] = array();
		}
		array_push( $restricoesAtividade[$item['_atinumero']], $item );
	}
	
?>
<?php if( count( $restricoesAtividade ) ): ?>
	<?php foreach( $restricoesAtividade as $atinumero => $restricoes ): ?>
		<?php
			$dados_atividade = $restricoes[0];
			$rastro = array();
			$lista = atividade_pegar_rastro( $atinumero );
			if ( count( $lista ) == 0 )
			{
				continue;
			}
			foreach ( $lista as $indice => $item )
			{
				$texto = sprintf(
					'<p style="margin: 0 0 5px %dpx;">%s<a href="?modulo=%s&acao=%s&atiid=%d">%s %s</a></p>',
						$indice * 20,
						$indice != 0 ? '<img src="../imagens/seta_filho.gif" align="absmiddle" border="0">&nbsp;' : '',
						"principal/atividade_/controle",
						$_REQUEST['acao'],
						$item['atiid'],
						$item['numero'],
						$item['atidescricao']
				);
				array_push( $rastro, $texto );
			}
			$rastro = sprintf( '<div style="margin: 5px 5px 0 5px">%s</div>', implode( '', $rastro ) );
		?>
		<table
			align="center"
			class="tabela"
			bgcolor="#f5f5f5"
			style="
				width:95%;
				margin-top: 15px;
				<?= $dados_atividade['obssolucao'] == 't' ? 'color:#454545;' : '' ?>"
			cellpadding="3"
			cellspacing="1"
		>
			<thead>
				<tr>
					<td colspan="2" style="background-color: #e9e9e9;">
						<table width="100%" border="0">
							<tr>
								<td>
									<?php echo $rastro; ?>
								</td>
								<td width="90" valign="middle" align="center" style="padding-bottom: 7px;">
									<?= montar_barra_execucao( $dados_atividade, true ) ?>
								</td>
								<td width="90" valign="middle" align="center" style="padding-bottom: 7px; font-size: 10px;">
									início
									<br/>
									&nbsp;
									<?= formata_data( $dados_atividade['atidatainicio'] ) ?>
									&nbsp;
								</td>
								<td width="90" valign="middle" align="center" style="padding-bottom: 7px; font-size: 10px;">
									término<br/>
									&nbsp;
									<?= formata_data( $dados_atividade['atidatafim'] ) ?>
									&nbsp;
								</td>
							</tr>
						</table>
					</td>
				</tr>
			</thead>
			<tbody>
				<?php foreach( $restricoes as $restricao ): ?>
					<tr>
						<td class="SubTituloDireita" style="border-top: 1px solid #ababab; vertical-align:top; width:25%;">Descrição:</td>
						<td style="border-top: 1px solid #ababab; <?php if( $restricao['obssolucao'] == 'f' ): ?>font-weight:bold;<?php endif; ?>">
							<?php if( $restricao['obssolucao'] == 'f' ): ?>
								<img src="../imagens/restricao.png" border="0" align="absmiddle" style="margin: 0 3px 0 3px;"/>
							<?php endif; ?>
							<?= $restricao['obsdescricao'] ?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita" style="vertical-align:top; width:25%;">Data:</td>
						<td><?= formata_data( $restricao['obsdata'] ); ?></td>
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
							<?= $restricao['obssolucao'] == 't' ? 'Sim' : 'Não' ?>
						</td>
					</tr>
					<?php if( $restricao['obssolucao'] == 't' ): ?>
						<?php if( !empty( $restricao['obsmedida'] ) ): ?>
							<tr id="providencia_<?= $restricao['obsid'] ?>">
								<td class="SubTituloDireita" style="vertical-align:top; width:25%;">Providência:</td>
								<td><?= $restricao['obsmedida'] ?></td>
							</tr>
						<?php endif; ?>
						<tr id="datasuperacao_<?= $restricao['obsid'] ?>">
							<td class="SubTituloDireita" style="vertical-align:top; width:25%;">Data da providência:</td>
							<td><?= formata_data( $restricao['obsdatasolucao'] ) ?></td>
						</tr>
						<tr id="responsavel_<?= $restricao['obsid'] ?>">
							<td class="SubTituloDireita" style="vertical-align:top; width:25%;">Autor da providência:</td>
							<td>
								<div>
									<img onclick="enviar_email( '<?= $restricao['cpfresponsavel'] ?>' );" title="enviar e-mail" src="../imagens/email.gif" align="absmiddle" style="border:0; cursor:pointer;"/>
									<?= $restricao['nomeresponsavel'] ?>
								</div>
								<div style="color:#959595;"><?= $restricao['unidaderesponsavel'] ?> - Tel: (<?= $restricao['dddresponsavel'] ?>) <?= $restricao['telefoneresponsavel'] ?></div>
							</td>
						</tr>
					<?php else: ?>
						<tr id="titulo_providencia_<?= $restricao['obsid'] ?>" style="background-color: #cccccc;<?= $restricao['obssolucao'] == 'f' ? 'display:none;' : '' ?>">
							<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
							<td align='left' style="vertical-align:top;"><b>Providência</b></td>
						</tr>
						<tr id="providencia_<?= $restricao['obsid'] ?>" <?= $restricao['obssolucao'] == 'f' ? 'style="display:none;"' : '' ?>>
							<td class="SubTituloDireita" style="vertical-align:top; width:25%;">Descrição:</td>
							<td><?= campo_textarea( 'obsmedida', 'S', 'N', '', 70, 2, 250 ); ?></td>
						</tr>
					<?php endif; ?>
				<?php endforeach; ?>
			</tbody>
		</table>
	<?php endforeach; ?>
<?php else: ?>
	<table align="center" class='tabela' style="width:95%;" cellpadding="3">
		<tbody>
			<td style="text-align:center;padding:15px;background-color:#f5f5f5;">
				Item selecionado não possui restrições.
			</td>
		</tbody>
	</table>
<?php endif; ?>













