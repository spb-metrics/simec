<?php
include APPRAIZ . 'includes/classes/dateTime.inc';
$data = new Data();
//dbg($_REQUEST);
//ver($_REQUEST['atidatafim']);

$remetente = array('nome'=>$_SESSION['usunome'], 'email'=>'simec@mec.gov.br');

if($_REQUEST['atidatafim'] && $_REQUEST['flag2'] == 1){
	$retorno = $data->diferencaEntreDatas($_REQUEST['atidatafim'], $_REQUEST['dataFpai'], 'maiorDataBolean', 'string','dd/mm/yyyy');
	
	$sql = "SELECT usucpf FROM projetos.usuarioresponsabilidade WHERE atiid = {$_REQUEST['atiid']}";
	$usucpf = $db->pegaUm($sql);
	if(!($usucpf)){
		$sql = "SELECT usucpf FROM projetos.usuarioresponsabilidade WHERE atiid = (SELECT x.atiidpai FROM projetos.atividade x WHERE x.atiid = {$_REQUEST['atiid']})";
		$usucpf = $db->pegaUm($sql);
	}
	//SE NÃO TIVER, BUSCAR NO NIVEL ACIMA!! SE NÃO TIVER, PACIENCIA!!
	if($retorno && $usucpf){
		$sql = "select usuemail from seguranca.usuario where usucpf = '{$usucpf}'";
		$email = $db->pegaUm($sql);
		if($email){
			$assunto  = "[SIMEC] PDE - Plano de Desenvolvimento da Educação";
			$conteudo = "
				<font size='2'><b>Atividade:</b> <u>{$_REQUEST['atidescricao']}</u><font>
				<br><br>
				<b><font size='2'>A data final da atividade {$_REQUEST['atidescricao']} foi alterada para uma data posterior a data da atividade pai pelo usuário {$_SESSION['usunome']}. Por favor verifique.<font></b>
				<br><br>
				<br>	
				<a href=\"http://simec.mec.gov.br/projetos/projetos.php?modulo=principal/atividade_/atividade&acao=A&atiid={$_REQUEST['atiid']}\">Clique Aqui para acessar a atividade.</a>
				<br>	
				<br>		
				Atenciosamente,
				<br>	
				$assunto";
	
				enviar_email($remetente, 'victor.benzi@mec.gov.br', $assunto, $conteudo );
		}
	}	
}
$sql = "SELECT usucpf FROM projetos.usuarioresponsabilidade WHERE atiid = {$_REQUEST['atiid']}";
	$usucpf = $db->pegaUm($sql);

if($_REQUEST['atidatainicio'] && $_REQUEST['flag1'] == 1){
	$retorno = $data->diferencaEntreDatas($_REQUEST['dataIpai'], $_REQUEST['atidatainicio'], 'maiorDataBolean', 'string','dd/mm/yyyy');
	$sql = "SELECT usucpf FROM projetos.usuarioresponsabilidade WHERE atiid = {$_REQUEST['atiid']}";
	$usucpf = $db->pegaUm($sql);
	if(!($usucpf)){
		$sql = "SELECT usucpf FROM projetos.usuarioresponsabilidade WHERE atiid = (SELECT x.atiidpai FROM projetos.atividade x WHERE x.atiid = {$_REQUEST['atiid']})";
		$usucpf = $db->pegaUm($sql);
	}
	//SE NÃO TIVER, BUSCAR NO NIVEL ACIMA!! SE NÃO TIVER, PACIENCIA!!
	if($retorno && $usucpf){
		$sql = "select usuemail from seguranca.usuario where usucpf = '{$usucpf}'";
		$email = $db->pegaUm($sql);
		if($email){
			$assunto  = "[SIMEC] PDE - Plano de Desenvolvimento da Educação";
			$conteudo = "
				<font size='2'><b>Atividade:</b> <u>{$_REQUEST['atidescricao']}</u><font>
				<br><br>
				<b><font size='2'>A data inicial da atividade {$_REQUEST['atidescricao']} foi alterada para uma data anterior a data da atividade pai pelo usuário {$_SESSION['usunome']}. Por favor verifique.<font></b>
				<br><br>
				<br>	
				<a href=\"http://simec.mec.gov.br/projetos/projetos.php?modulo=principal/atividade_/atividade&acao=A&atiid={$_REQUEST['atiid']}\">Clique Aqui para acessar a atividade.</a>
				<br>	
				<br>		
				Atenciosamente,
				<br>	
				$assunto";
	
				enviar_email($remetente, 'victor.benzi@mec.gov.br', $assunto, $conteudo );
		}
	}	
}

if ( !$_REQUEST['atiid'] && $_REQUEST['atiidpai'] ) {
	include 'cadastrar.inc';
	return;
}
$atividade = (array) atividade_pegar( $_REQUEST['atiid'] );
if ( !$atividade ) {
	redirecionar( 'principal/atividade_/arvore', 'A' );
}

$parametros = array(
	'aba' => $_REQUEST['aba'], # mantém a aba ativada
	'atiid' => $_REQUEST['atiid'],
	'atidatafim' => $_REQUEST['atidatafim'],
	'atidatainicio' => $_REQUEST['atidatainicio'],
	'dataFpai' => $_REQUEST['dataFpai'],
	'dataIpai' => $_REQUEST['dataIpai'],
	'flag1' => $_REQUEST['flag1'],
	'flag2' => $_REQUEST['flag2'],
	'atidescricao' => $_REQUEST['atidescricao']
);

$sql = "SELECT a.esaid FROM projetos.atividade a WHERE a.atiid = (SELECT x.atiidpai FROM projetos.atividade x WHERE x.atiid = {$_REQUEST['atiid']})";
$esaidpai = $db->pegaUm($sql);

$sql = "SELECT a._atiprofundidade FROM projetos.atividade a WHERE a.atiid = {$_REQUEST['atiid']}";
$profundidade = $db->pegaUm($sql);

$sql = "SELECT to_char(a.atidatainicio, 'DD/MM/YYYY') FROM projetos.atividade a WHERE a.atiid = {$_REQUEST['atiid']}";
$dataI = $db->pegaUm($sql);

$sql = "SELECT to_char(a.atidatafim, 'DD/MM/YYYY') FROM projetos.atividade a WHERE a.atiid = {$_REQUEST['atiid']}";
$dataF = $db->pegaUm($sql);

$sql = "SELECT to_char(a.atidatainicio, 'DD/MM/YYYY') FROM projetos.atividade a WHERE a.atiid = (SELECT x.atiidpai FROM projetos.atividade x WHERE x.atiid = {$_REQUEST['atiid']})";
$dataIpai = $db->pegaUm($sql);

$sql = "SELECT to_char(a.atidatafim, 'DD/MM/YYYY') FROM projetos.atividade a WHERE a.atiid = (SELECT x.atiidpai FROM projetos.atividade x WHERE x.atiid = {$_REQUEST['atiid']})";
$dataFpai = $db->pegaUm($sql);



if ( $_REQUEST['formulario'] ) {
	// verifica alteração no status
	if ( $_REQUEST['esaid'] != $atividade['esaid'] && $_REQUEST['esaid'] == STATUS_CONCLUIDO ) {
		$_REQUEST['atidataconclusao'] = $_REQUEST['atidataconclusao'] ? $_REQUEST['atidataconclusao'] : date( 'Y-m-d' );
	}
	// captura e filtra as informações enviadas pelo usuário
	$registro = array();
	
	foreach( $atividade as $atributo => $valor ){
		if ( isset( $_REQUEST[$atributo] ) ) {
			$registro[$atributo] = $_REQUEST[$atributo];
		}
	}
	unset( $registro['esadescricao'], $registro['numero'], $registro['projeto'] );
	$datas = array( "atidatainicio", "atidatafim", "atidataconclusao" );
	foreach ( $datas as $campo ) {
		if ( array_key_exists( $campo, $registro ) ) {
			$data = explode( "/", $registro[$campo] );
			//dbg($data, 1);
			if ( !checkdate( (integer) $data[1], (integer) $data[0], (integer) $data[2] ) ) {
				unset( $registro[$campo] );
			}
		}
	}
	
	$registro['atidatainicio'] = formata_data_sql($registro['atidatainicio']);
	$registro['atidatafim'] = formata_data_sql($registro['atidatafim']);
	$registro['atidataconclusao'] = formata_data_sql($registro['atidataconclusao']);
	
	$atribuicao = array();
	foreach ( $registro as $campo => $valor ) {
		array_push( $atribuicao, sprintf( " %s = %s ", $campo, empty( $valor ) ? 'null' : "'".trim( $valor )."'" ) );
	}
	$atribuicao = implode( ',', $atribuicao );
	$sql = sprintf(
		"update projetos.atividade set %s where atiid = %d",
		$atribuicao,
		$registro['atiid']
	);
	if ( !$db->executar( $sql ) ) {
		$db->rollback();
		redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
	}
	// cadastra as atividades selecionadas	
	if($_POST['atividade'] == 'permitido'){
		$sql = sprintf(
		"DELETE FROM projetos.atividadegrupoatividade WHERE atiid = %d",
		$_REQUEST['atiid']				
		);		
		$db->executar( $sql );
		
		$_REQUEST['grupoatividade'] = ($_REQUEST['grupoatividade'])?$_REQUEST['grupoatividade']:array();
		foreach ( $_REQUEST['grupoatividade'] as $graid ) {
			if ( empty( $graid ) ) {
				continue;
			}
			$sql = sprintf(
				"INSERT INTO projetos.atividadegrupoatividade( atiid, graid ) VALUES ( '%s', %d )",
				$_REQUEST['atiid'],
				$graid
			);	
			$db->executar( $sql );
		}
	}
	
	$db->commit();
	//ver($parametros, d);
	redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
//	ver('aqui',d);
}

//alimentado o combo_popup
$sql_grupoatividade = "select g.graid as codigo, g.gradsc as descricao 
						from projetos.grupoatividade g
						inner join projetos.atividadegrupoatividade ag on ag.graid= g.graid						
						where ag.atiid = ".$_REQUEST['atiid']."
						order by gradsc ";

$grupoatividade =$db->carregar($sql_grupoatividade);


$permissao = atividade_verificar_responsabilidade( $atividade['atiid'], $_SESSION['usucpf'] );
$permissao_formulario = $permissao ? 'S' : 'N'; # S habilita e N desabilita o formulário

// VERIFICA SE PROJETO ESTÁ SELECIONADO
projeto_verifica_selecionado( $atividade['atiid'] );

// CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '&atiid=' . $atividade['atiid'] );
montar_titulo_projeto( $atividade['atidescricao'] );

extract( $atividade ); # mantém o formulário preenchido


$sql_pfl_assessores = "
					select pflcod
					from seguranca.perfil
					where pfldsc = 'Assessores' and sisid = 10";
$sql_pfl_gestor = "
					select pflcod
					from seguranca.perfil
					where pfldsc = 'Gestor do PDE' and sisid = 10";
$sql_pfl_apoio = "
					select pflcod
					from seguranca.perfil
					where pfldsc = 'Equipe de Apoio ao Gestor do PDE' and sisid = 10";

$assessores = $db->pegaUm($sql_pfl_assessores);
$gestor = $db->pegaUm($sql_pfl_gestor);
$apoio = $db->pegaUm($sql_pfl_apoio);


?>
<!-- habilita o tiny -->
<!-- habilita o tiny -->
<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
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
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script language="javascript" type="text/javascript">
/*
	function enviar(){
		/*
		if ( validar_formulario() ) {
		
			//var formulario = document.getElementById( 'formulario' );
			//var gr_atividade = formulario.grupoatividade;
			
			if ( document.formulario.grupoatividade ) {
				selectAllOptions( document.formulario.grupoatividade );
			}			
			document.formulario.submit();
		}
		
		document.formulario.submit();
	}
*/	
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
	function selecionarSituacao( valor ){
		switch ( valor ) {
			case '1':
				document.formulario.atiporcentoexec.value = '0';
				break;
			case '5':
				document.formulario.atiporcentoexec.value = '100';
				break;
			default:
				break;
		}
	}
	
	function selecionarAndamento( valor ){
		if ( document.formulario.esaid.value == '3' || document.formulario.esaid.value == '4' ) {
			return;
		}
		switch ( valor ) {
			case '0':
				document.formulario.esaid.value = '1';
				break;
			case '100':
				document.formulario.esaid.value = '5';
				break;
			default:
				document.formulario.esaid.value = '2';
				break;
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

</script>
<table class="tabela" bgcolor="#fbfbfb" cellspacing="0" cellpadding="10" align="center">
	<tr>
		<td>
			<?/*= montar_resumo_atividade( $atividade ) */?>
			<form method="post" name="formulario" id="formulario">
				<input type="hidden" name="formulario" value="1"/>
				<input type="hidden" name="esaidpai" value="<?= $esaidpai ?>">
				<input type="hidden" name="profundidade" value="<?= $profundidade ?>">
				<input type="hidden" name="dataI" value="<?= $dataI ?>">
				<input type="hidden" name="dataF" value="<?= $dataF ?>">
				<input type="hidden" name="dataIpai" value="<?= $dataIpai ?>">
				<input type="hidden" name="dataFpai" value="<?= $dataFpai ?>">
				<input type="hidden" name="atiid" value="<?= $atiid ?>"/>
				<input type="hidden" name="atiidpai" value="<?= $atiidpai ?>"/>
				<input type="hidden" name="flag1" value="0"/>
				<input type="hidden" name="flag2" value="0"/>
				<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 100%;">
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Título:</td>
						<td><?= campo_textarea( 'atidescricao', 'S', $permissao_formulario, '', 70, 3, 500 ); ?></td>
					</tr>
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Descrição:</td>
						<td>
							<?php if( $permissao ): ?>
								<textarea name="atidetalhamento" rows="15" cols="80" class="text_editor_simple"><?= $atidetalhamento ?></textarea>
							<?php else: ?>
								<?= $atidetalhamento ?>
							<?php endif; ?>
						</td>
					</tr>
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Órgãos Participantes:</td>
						<td><?= campo_textarea( 'atiinterface', 'N', $permissao_formulario, '', 70, 3, 250 ); ?></td>
					</tr>
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Situação:</td>
						<td>
						<?php
							$sql = "select e.esaid as codigo, e.esadescricao as descricao from projetos.estadoatividade e";
							$db->monta_combo( "esaid", $sql, $permissao_formulario, '', 'selecionarSituacao', '' );
						?>
						</td>
					</tr>
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Andamento:</td>
						<td>
							<select name="atiporcentoexec" onchange="selecionarAndamento( this.value );" class="CampoEstilo" <?= $permissao_formulario == 'S' ? '' : 'disabled="disabled"' ?>>
								<?php for( $porcentagem = 0; $porcentagem <= 100; $porcentagem += 10 ): ?>
									<option <?= $atiporcentoexec == $porcentagem ? 'selected' : '' ?> value="<?= $porcentagem ?>"><?= $porcentagem ?>%</option>
								<?php endfor; ?>
							</select>
						</td>
					</tr>
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Data de Início:</td>
						<td><?= campo_data( 'atidatainicio', 'N', $permissao_formulario, '', 'S' ); ?></td>
					</tr>
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Data de Conclusão Prevista:</td>
						<td><?= campo_data( 'atidatafim', 'N', $permissao_formulario, '', 'S' ); ?></td>
					</tr>
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top">Data de Conclusão:</td>
						<td><?= campo_data( 'atidataconclusao', 'N', $permissao_formulario, '', 'S' ); ?></td>
					</tr>
					
					<?
						//perfis q podem editar e ver as atividades
						$editar_atividade = array($assessores);
						$ver_atividade = array($gestor, $apoio);
						
						if((verificaPerfil($editar_atividade)) || ($_SESSION['superuser'])){						
						?>
						<input type="hidden" name="atividade" id="atividade" value="permitido"/>
						<tr>
							<td align='right' class="SubTituloDireita">Atividades:</td>
							<td>
								<?php
								$sql =
									"select graid as codigo, gradsc as descricao 
									 from projetos.grupoatividade
									 order by gradsc";
								combo_popup(
									'grupoatividade',
									$sql,
									'Selecione as Atividades', 
									'400x400', 
									0,
									array(), 
									'',
									'S',
									false,
									false,
									5,
									350,
									'',
									''
								);
								?>		
							</td>
							</tr>
						<?							
						}else if(verificaPerfil($ver_atividade)){						
						?>
						<input type="hidden" name="atividade" id="atividade" value="acessonegado"/>
						<tr>
						<td align='right' class="SubTituloDireita">Atividades:</td>
						<td>
							<?php
							$sql =
								"select graid as codigo, gradsc as descricao 
								 from projetos.grupoatividade
								 order by gradsc";
							combo_popup(
								'grupoatividade',
								$sql,
								'Selecione as Atividades', 
								'400x400', 
								0,
								array(), 
								'',
								'N',
								false,
								false,
								10,
								400,
								'',
								''
							);
							?>		
						</td>
						</tr>
						<?
						}else{
						?>
						<input type="hidden" name="atividade" id="atividade" value="acessonegado"/>
						<?						
						}
					?>
					
					<tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Meta:</td>
						<td><?= campo_textarea( 'atimeta', 'N', $permissao_formulario, '', 70, 3, 250 ); ?></td>
					</tr>
					<?php
						$sql = sprintf(
							"select
								u.usunome, u.usucpf, u.usuemail, u.usufoneddd, u.usufonenum,
								pu.pflcod,
								un.unidsc
							from seguranca.usuario u
							inner join seguranca.usuario_sistema us on
								us.usucpf = u.usucpf
							inner join seguranca.perfilusuario pu on
								pu.usucpf = u.usucpf
							inner join projetos.usuarioresponsabilidade ur on
								ur.usucpf = u.usucpf and ur.pflcod = pu.pflcod
							left join public.unidade un on
								un.unicod = u.unicod
							where
								u.suscod = 'A'
								and us.suscod = 'A' and us.sisid = %d
								and pu.pflcod = %d
								and ur.atiid = %d and ur.rpustatus = 'A'
							order by pflcod, usunome",
							$_SESSION['sisid'],
							PERFIL_GERENTE,
							$atividade['atiid']
						);
						$usuarios = $db->carregar( $sql );
					?>
					<?php if( $usuarios ): ?>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Gerente:</td>
							<td style="padding-left: 5px;">
								<?php foreach( $usuarios as $usuario ): ?>
									<div style="padding: 5px;">
										<div style="font-weight: bold">
											<img title="enviar e-mail" src="../imagens/email.gif" align="absmiddle" style="border:0; cursor:pointer;" onclick="enviar_email( '<?= $usuario['usucpf'] ?>' );"/>
											<?= $usuario['usunome'] ?>
										</div>
										<div style="color:#858585; margin:2px;"><?= $usuario['unidsc'] ?> - Tel: (<?= $usuario['usufoneddd'] ?>) <?= $usuario['usufonenum'] ?></div>
									</div>
								<?php endforeach; ?>
							</td>
						</tr>
					<?php endif; ?>
					<?php
						$sql = sprintf(
							"select
								u.usunome, u.usucpf, u.usuemail, u.usufoneddd, u.usufonenum,
								pu.pflcod,
								un.unidsc
							from seguranca.usuario u
							inner join seguranca.usuario_sistema us on
								us.usucpf = u.usucpf
							inner join seguranca.perfilusuario pu on
								pu.usucpf = u.usucpf
							inner join projetos.usuarioresponsabilidade ur on
								ur.usucpf = u.usucpf and ur.pflcod = pu.pflcod
							left join public.unidade un on
								un.unicod = u.unicod
							where
								u.suscod = 'A'
								and us.suscod = 'A' and us.sisid = %d
								and pu.pflcod = %d
								and ur.atiid = %d and ur.rpustatus = 'A'
							order by pflcod, usunome",
							$_SESSION['sisid'],
							PERFIL_EQUIPE_APOIO_GERENTE,
							$atividade['atiid']
						);
						$usuarios = $db->carregar( $sql );
					?>
					<?php if( $usuarios ): ?>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Equipe de Apoio:</td>
							<td style="padding-left: 5px;">
								<?php foreach( $usuarios as $usuario ): ?>
									<div style="padding: 5px;">
										<div>
											<img title="enviar e-mail" src="../imagens/email.gif" align="absmiddle" style="border:0; cursor:pointer;" onclick="enviar_email( '<?= $usuario['usucpf'] ?>' );"/>
											<?= $usuario['usunome'] ?>
										</div>
										<div style="color:#858585; margin:2px;"><?= $usuario['unidsc'] ?> - Tel: (<?= $usuario['usufoneddd'] ?>) <?= $usuario['usufonenum'] ?></div>
									</div>
								<?php endforeach; ?>
							</td>
						</tr>
					<?php endif; ?>
					<?php if( $permissao ): ?>
						<tr style="background-color: #cccccc">
							<td align='right' style="vertical-align:top; width:25%;">&nbsp;</td>
							<td><input type="button" name="botao" value="Salvar" onclick="enviar();"/></td>
						</tr>
					<?php endif; ?>
				</table>
			</form>
		</td>
	</tr>
</table>
<script>
function enviar(){
	var validacao = true;
	var mensagem = 'Os seguintes campos não foram preenchidos:';
	document.formulario.atidetalhamento.value = trim( document.formulario.atidetalhamento.value );
	document.formulario.atimeta.value = trim( document.formulario.atimeta.value );
	document.formulario.atiinterface.value = trim( document.formulario.atiinterface.value );
	document.formulario.atidescricao.value = trim( document.formulario.atidescricao.value );
	document.formulario.atidatainicio.value = trim( document.formulario.atidatainicio.value );
	document.formulario.atidatafim.value = trim( document.formulario.atidatafim.value );
	if ( document.formulario.atidescricao.value == '' ) {
		mensagem += '\nTítulo';
		validacao = false;
	}
	if(document.formulario.esaid.value == 2 && document.formulario.atiporcentoexec.value == 0){
		if(!(confirm("Tem certeza que deseja manter a porcentagem em 0%?"))){
			return false;
		}
	}
	
	if(document.formulario.profundidade.value > 3){
		if(document.formulario.esaid.value == 2 && document.formulario.esaidpai.value == 1){
			if(!(confirm("Tem certeza que deseja manter o processo em\nandamento com o processo pai sem ser iniciado?"))){
				return false;
			}
		}

		var data1 = document.formulario.atidatainicio.value;
		var data2 = document.formulario.dataIpai.value;

		if(data3 && data4){
			if(document.formulario.atidatainicio.value != document.formulario.dataI.value){
				if ( parseInt( data2.split( "/" )[2].toString() + data2.split( "/" )[1].toString() + data2.split( "/" )[0].toString() ) > parseInt( data1.split( "/" )[2].toString() + data1.split( "/" )[1].toString() + data1.split( "/" )[0].toString() ) )
				{
					if(!(confirm("Tem certeza que deseja alterar a data\ninicial para uma data anterior da atividade pai?"))){
						return false;
					}else {
						document.formulario.flag1.value = 1;
					}
				}
			}
		}
		
		var data4 = document.formulario.atidatafim.value;
		var data3 = document.formulario.dataFpai.value;

		if(data3 && data4){
			if(document.formulario.atidatafim.value != document.formulario.dataF.value){
				if ( parseInt( data4.split( "/" )[2].toString() + data4.split( "/" )[1].toString() + data4.split( "/" )[0].toString() ) > parseInt( data3.split( "/" )[2].toString() + data3.split( "/" )[1].toString() + data3.split( "/" )[0].toString() ) )
				{
					if(!(confirm("Tem certeza que deseja alterar a data\nfinal para uma data posterior da atividade pai?"))){
						return false;
					} else {
						document.formulario.flag2.value = 1;
					}
				}
			}
		}
	}
	
	if ( !validacao ) {
		alert( mensagem );
	}
	
	document.formulario.submit();
	return true;
}
</script>
