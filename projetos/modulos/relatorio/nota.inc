<?php

/*
insert into seguranca.menu (
	mnucod,mnuidpai,mnudsc,mnutransacao,mnulink,mnutipo,mnustile,mnuhtml, mnusnsubmenu,mnushow,mnustatus,abacod,sisid
) values (
	24000,656,'Observação','Relatório de Observações','projetos.php?modulo=relatorio/nota&acao=A','2','','','f','t','A',null,11
);
*/

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
			<td align='right' class="SubTituloDireita">Filtro:</td>
			<td>
				<?php
				$texto = $_REQUEST['texto'];
				echo campo_texto('texto','','','',50,50,'','');
				unset( $texto );
				?>
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
<!-- LISTA DE DOCUMENTOS -->
<?php
	
	$filtro = '';
	
	$filtro_texto = array();
	foreach ( explode( " ", $_REQUEST['texto'] ) as $texto ) {
		$texto = trim( $texto );
		if ( empty( $texto ) ) {
			continue;
		}
		array_push( $filtro_texto, " lower( n.notdescricao ) like lower( '%" . $texto . "%' ) " );
	}
	if ( !empty( $filtro_texto ) ) {
		$filtro .= ' and ( '. implode( " and ", $filtro_texto ) .' ) ';
	}
	
	if ( $_REQUEST['atividade'] )
	{
		$atiid = $_REQUEST['atividade'];
		$sql = "select _atinumero from projetos.atividade where atiid = " . $atiid;
		$numero = $db->pegaUm( $sql );
		$filtro .= " and at._atinumero like '" . $numero . ".%' ";
	}
	
	$sql = sprintf(
		"select
			n.notid, n.notdata, n.notdescricao,
			usu.usucpf, usu.usunome, usu.usufoneddd, usu.usufonenum,
			uni.unidsc,
			at.atiid, at._atinumero, at.atiporcentoexec, at.atidatainicio, at.atidatafim, at.esaid, ea.esadescricao
		from projetos.notaatividade n
			left join seguranca.usuario usu on usu.usucpf = n.usucpf
			left join public.unidade uni on uni.unicod = usu.unicod
			inner join projetos.atividade at on at.atiid = n.atiid and at._atiprojeto = %d
			inner join projetos.estadoatividade ea on ea.esaid = at.esaid
		where n.notstatus = 'A' %s
		order by at._atiordem",
		PROJETO,
		$filtro
	);
	
	$observacoes_temp = $db->carregar( $sql );
	$observacoes_temp = $observacoes_temp ? $observacoes_temp : array();
	
	$observacoesAtividade = array();
	foreach ( $observacoes_temp as $item )
	{
		if ( !array_key_exists( $item['_atinumero'], $observacoesAtividade ) )
		{
			$observacoesAtividade[$item['_atinumero']] = array();
		}
		array_push( $observacoesAtividade[$item['_atinumero']], $item );
	}
?>
<?php if( count( $observacoesAtividade ) ): ?>
	<?php foreach( $observacoesAtividade as $atinumero => $observacoes ): ?>
		<?php
			
			$dados_atividade = $observacoes[0];
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
						"principal/atividade_/observacao",
						$_REQUEST['acao'],
						$item['atiid'],
						$item['numero'],
						$item['atidescricao']
				);
				array_push( $rastro, $texto );
			}
			$rastro = sprintf( '<div style="margin: 5px 5px 0 5px">%s</div>', implode( '', $rastro ) );
			
		?>
		<table align="center" class="tabela" bgcolor="#f5f5f5" style="width:95%; margin-top: 15px;" cellpadding="3" cellspacing="1">
			<thead>
				<tr>
					<td colspan="3" style="background-color: #e9e9e9;">
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
				<?php foreach( $observacoes as $observacao ): ?>
				<tr>
					<td style="width: 100px; text-align: center; border: 2px solid #ffffff; vertical-align: top;">
						<?= formata_data( $observacao['notdata'] ) ?>
					</td>
					<td style="width: 200px; text-align: left; border: 2px solid #ffffff; vertical-align: top;">
						<img onclick="enviar_email( '<?= $observacao['usucpf'] ?>' );" title="enviar e-mail" src="../imagens/email.gif" align="absmiddle" style="border:0; cursor:pointer;"/>
						<?= $observacao['usunome'] ?>
						<div style="color:#959595;">Tel: (<?= $observacao['usufoneddd'] ?>) <?= $observacao['usufonenum'] ?></div>
					</td>
					<td style="border: 2px solid #ffffff; vertical-align: top;">
						<?= $observacao['notdescricao'] ?>
					</td>
				</tr>
				<?php endforeach; ?>
			</tbody>
		</table>
	<?php endforeach; ?>
<?php else: ?>
	<table align="center" class='tabela' style="width:95%;" cellpadding="3">
		<tbody>
			<td style="text-align:center;padding:15px;background-color:#f5f5f5;">
				Não há observações que satisfaçam seus cristérios de pesquisa.
			</td>
		</tbody>
	</table>
<?php endif; ?>