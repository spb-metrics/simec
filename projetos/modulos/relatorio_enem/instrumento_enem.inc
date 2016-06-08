<?php

/*
insert into seguranca.menu (
	mnucod,mnuidpai,mnudsc,mnutransacao,mnulink,mnutipo,mnustile,mnuhtml, mnusnsubmenu,mnushow,mnustatus,abacod,sisid
) values (
	23000,656,'Documento','Relatório de Documentos','projetos.php?modulo=relatorio/instrumento&acao=A','2','','','f','t','A',null,11
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
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Tipo:</td>
			<td>
				<select style="width: 200px;" class="CampoEstilo" name="taaid">
					<option value="">- selecione -</option>
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
	
	if ( $_REQUEST['taaid'] ) {
		$filtro .= ' and a.taaid = ' . (integer) $_REQUEST['taaid'];
	}
	
	$filtro_texto = array();
	foreach ( explode( " ", $_REQUEST['texto'] ) as $texto ) {
		$texto = trim( $texto );
		if ( empty( $texto ) ) {
			continue;
		}
		array_push( $filtro_texto, " lower( a.anedescricao ) like lower( '%" . $texto . "%' ) " );
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
			a.aneid, a.anedescricao, t.taadescricao, v.verid, v.vernome, v.verdata,
			at.atiid, at._atinumero, at.atiporcentoexec, at.atidatainicio, at.atidatafim, at.esaid, ea.esadescricao
		from projetos.anexoatividade a
		inner join projetos.tipoanexoatividade t on a.taaid = t.taaid
		inner join projetos.versaoanexoatividade v on a.verid = v.verid
		inner join projetos.atividade at on at.atiid = a.atiid and at._atiprojeto = %d
		inner join projetos.estadoatividade ea on ea.esaid = at.esaid
		where anestatus='A' %s
		order by at._atiordem",
		PROJETO,
		$filtro
	);
	
	$instrumentos_temp = $db->carregar( $sql );
	$instrumentos_temp = $instrumentos_temp ? $instrumentos_temp : array();
	
	$instrumentosAtividade = array();
	foreach ( $instrumentos_temp as $item )
	{
		if ( !array_key_exists( $item['_atinumero'], $instrumentosAtividade ) )
		{
			$instrumentosAtividade[$item['_atinumero']] = array();
		}
		array_push( $instrumentosAtividade[$item['_atinumero']], $item );
	}
?>
<?php if( count( $instrumentosAtividade ) ): ?>
	<?php foreach( $instrumentosAtividade as $atinumero => $instrumentos ): ?>
		<?php
			
			$dados_atividade = $instrumentos[0];
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
						"principal/atividade_/instrumento",
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
					<td colspan="4" style="background-color: #e9e9e9;">
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
				<?php foreach( $instrumentos as $instrumento ): ?>
				<tr>
					<td style="width: 150px; text-align: left; border: 2px solid #ffffff;">
						<a href="?modulo=principal/atividade_/instrumento&acao=A&atiid=<?= $instrumento['atiid'] ?>&verid=<?= $instrumento['verid'] ?>&evento=download" title="download">
							<img src="../imagens/salvar.png" style="border:0; vertical-align:middle;"/>
							<?= $instrumento['vernome'] ?>
						</a>
					</td>
					<td style="width: 150px; text-align: center; border: 2px solid #ffffff; font-weight: bold;">
						<?= $instrumento['taadescricao'] ?>
					</td>
					<td style="width: 150px; text-align: center; border: 2px solid #ffffff;">
						<?= formata_data( $dados_atividade['verdata'] ) ?>
					</td>
					<td style="border: 2px solid #ffffff;">
						<?= $instrumento['anedescricao'] ?>
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
				Não há documentos que satisfaçam seus cristérios de pesquisa.
			</td>
		</tbody>
	</table>
<?php endif; ?>