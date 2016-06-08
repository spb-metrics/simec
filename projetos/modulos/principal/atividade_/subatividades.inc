<?php

$atividade = atividade_pegar( $_REQUEST['atiid'] );
if ( !$atividade ) {
	redirecionar( 'principal/atividade_/arvore', 'A' );
}

$permissao = atividade_verificar_responsabilidade( $atividade['atiid'], $_SESSION['usucpf'] );
$permissao_formulario = $permissao ? 'S' : 'N'; # S habilita e N desabilita o formulário

// ----- VERIFICA SE PROJETO ESTÁ SELECIONADO
projeto_verifica_selecionado();

// ----- CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '&atiid=' . $atividade['atiid'] );
montar_titulo_projeto( $atividade['atidescricao'] );

extract( $atividade ); # mantém o formulário preenchido


if ( isset( $_REQUEST["formulario_filtro_arvore"] ) )
{
	$situacao = $_REQUEST['situacao'];
	$usuario = $_REQUEST["usucpf"];
}
else
{
	$situacao = array(
		STATUS_NAO_INICIADO,
		STATUS_EM_ANDAMENTO,
		STATUS_SUSPENSO,
		STATUS_CANCELADO,
		STATUS_CONCLUIDO
	);
	$usuario = null;
}


?>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="0" cellpadding="10" align="center">
	<tr>
		<td valign="top">
			<?php if ( $_REQUEST['acao'] != 'R' ) : ?>
				<script language="javascript" type="text/javascript">
					
					function formulario_filtro_arvore_submeter()
					{
						document.formulario_filtro_arvore.submit();
					}
					
				</script>
				<form name="formulario_filtro_arvore" action="" method="post">
					<input type="hidden" name="formulario_filtro_arvore" value="1"/>
					<input type="hidden" name="atiid" value="<?php echo $_REQUEST['atiid']; ?>"/>
					<table border="0" cellpadding="5" cellspacing="0" width="100%">
						<tr>
							<td align="right">
								Profundidade
							</td>
							<td>
								<?php
								
								// força o preenchimento do formulário
								$_REQUEST["profundidade"] = $_REQUEST["profundidade"] ? $_REQUEST["profundidade"] : 3;
								
								?>
								<select name="profundidade" class="CampoEstilo">
									<option value="1" <?= $_REQUEST["profundidade"] == 1 ? 'selected="selected"' : '' ?>>1 nível</option>
									<option value="2" <?= $_REQUEST["profundidade"] == 2 ? 'selected="selected"' : '' ?>>2 níveis</option>
									<option value="3" <?= $_REQUEST["profundidade"] == 3 ? 'selected="selected"' : '' ?>>3 níveis</option>
									<option value="4" <?= $_REQUEST["profundidade"] == 4 ? 'selected="selected"' : '' ?>>4 níveis</option>
									<option value="5" <?= $_REQUEST["profundidade"] == 5 ? 'selected="selected"' : '' ?>>5 níveis</option>
									<option value="6" <?= $_REQUEST["profundidade"] == 6 ? 'selected="selected"' : '' ?>>6 níveis</option>
								</select>
							</td>
						</tr>
						<script language="javascript" type="text/javascript">
							
							function SetAllCheckBoxes( FormName, FieldName, CheckValue ) {
								if(!document.forms[FormName])
									return;
								var objCheckBoxes = document.forms[FormName].elements[FieldName];
								if(!objCheckBoxes)
									return;
								var countCheckBoxes = objCheckBoxes.length;
								if(!countCheckBoxes)
									objCheckBoxes.checked = CheckValue;
								else
									for(var i = 0; i < countCheckBoxes; i++)
										objCheckBoxes[i].checked = CheckValue;
							}
							
						</script>
						<tr>
							<td align="right">
								Situação
								(<a href="" onclick="SetAllCheckBoxes( 'formulario_filtro_arvore', 'situacao[]', true ); return false;">todos</a>)
							</td>
							<td>
								<?php
								
								// força o preenchimento do formulário
								if ( $_REQUEST["formulario_filtro_arvore"] ) {
									$situacao = $_REQUEST["situacao"];
								} else {
									$situacao = array(
										STATUS_NAO_INICIADO,
										STATUS_EM_ANDAMENTO
									);
								}
								
								?>
								<label style="margin: 0 10px 0 0;"><input type="checkbox" name="situacao[]" value="<?= STATUS_NAO_INICIADO ?>" <?= in_array( STATUS_NAO_INICIADO, (array) $situacao ) ? 'checked="checked"' : '' ?>/>não iniciado</label>
								<label style="margin: 0 10px 0 0;"><input type="checkbox" name="situacao[]" value="<?= STATUS_EM_ANDAMENTO ?>" <?= in_array( STATUS_EM_ANDAMENTO, (array) $situacao ) ? 'checked="checked"' : '' ?>/>em andamento</label>
								<label style="margin: 0 10px 0 0;"><input type="checkbox" name="situacao[]" value="<?= STATUS_SUSPENSO ?>" <?= in_array( STATUS_SUSPENSO, (array) $situacao ) ? 'checked="checked"' : '' ?>/>suspenso</label>
								<label style="margin: 0 10px 0 0;"><input type="checkbox" name="situacao[]" value="<?= STATUS_CANCELADO ?>" <?= in_array( STATUS_CANCELADO, (array) $situacao ) ? 'checked="checked"' : '' ?>/>cancelado</label>
								<label style="margin: 0 10px 0 0;"><input type="checkbox" name="situacao[]" value="<?= STATUS_CONCLUIDO ?>" <?= in_array( STATUS_CONCLUIDO, (array) $situacao ) ? 'checked="checked"' : '' ?>/>concluído</label>
							</td>
						</tr>
						<?php if( atividade_verificar_responsabilidade( PROJETO, $_SESSION["usucpf"] ) ): ?>
							<tr>
								<td align="right">Sob Responsabilidade</td>
								<td>
									<?php
									
									// força o preenchimento do formulário
									$usucpf = $_REQUEST["usucpf"];
									
									$sql = "
										select
											u.usucpf as codigo,
											u.usunome as descricao
										from seguranca.usuario u
											inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf
											inner join projetos.usuarioresponsabilidade ur on ur.usucpf = u.usucpf
											inner join seguranca.perfil p on p.pflcod = ur.pflcod
											inner join seguranca.perfilusuario pu on pu.pflcod = p.pflcod and pu.usucpf = u.usucpf
											inner join projetos.atividade a on a.atiid = ur.atiid
										where
											u.suscod = 'A'
											and us.suscod = 'A'
											and us.sisid = ". $_SESSION["sisid"] ."
											and ur.rpustatus = 'A'
											and ur.pflcod = ". PERFIL_GERENTE ."
											and a.atistatus = 'A'
											and a._atiprojeto = ". PROJETO ."
										group by u.usucpf, u.usunomeguerra, u.usunome
										order by u.usunome
									";
									$db->monta_combo(
										"usucpf",
										$sql,
										"S",
										"- selecione -",
										"", ""
									);
									
									?>
								</td>
							</tr>
						<?php endif; ?>
						<tr>
							<td align="right">&nbsp;</td>
							<td>
								<input
									type="button"
									name="alterar_arvore"
									value="Atualizar Árvore"
									onclick="formulario_filtro_arvore_submeter();"
								/>
							</td>
						</tr>
					</table>
				</form>
			<?php endif; ?>
		</td>
		<td valign="top" width="250">&nbsp;</td>
	</tr>
</table>
<table class="tabela" bgcolor="#fbfbfb" cellspacing="0" cellpadding="10" align="center" style="border-top: none;">
	<tr>
		<td>
			<?= montar_resumo_atividade( $atividade ) ?>
			<?php
				$profundidade = $_REQUEST["profundidade"] + $atividade["_atiprofundidade"];
				echo arvore( $_REQUEST["atiid"], $profundidade, $situacao, $usuario, null, null, array() );
			?>
		</td>
	</tr>
</table>