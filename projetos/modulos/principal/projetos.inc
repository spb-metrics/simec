<?php
// VERIFICA DE USUÁRIO POSSUI PERFIL PARA ALOCAÇÃO DE DALAS
$usuario_alocacao_salas = arrayPerfil();
if(in_array(PERFIL_ALOCACAO_SALAS,$usuario_alocacao_salas) && count($usuario_alocacao_salas) == 1){
	redirecionar( 'principal/atividade_/listaEnem', 'A' );
}elseif(in_array(PERFIL_ALOCACAO_SALAS,$usuario_alocacao_salas) && count($usuario_alocacao_salas) > 1){
	$atiidENEM = "55168";
}
// VERIFICA SE USUÁRIO É GERENTE
$usuario_gerente_projeto = usuario_possui_perfil( PERFIL_GESTOR ) || $db->testa_superuser();

// SELECIONA PROJETO
if ( $_REQUEST['selecionar_projeto'] ) {
	# TODO: verificar se o projeto existe
	$_SESSION['projeto'] = (integer) $_REQUEST['selecionar_projeto'];
	redirecionar( 'principal/atividade_/arvore', 'A' );
} else {
	$_SESSION['projeto'] = null;
}

// CADASTRAR PROJETO
if ( $_REQUEST['cadastrar_projeto'] && $usuario_gerente_projeto ) {
	$descricao = $_REQUEST['cadastrar_projeto'];
	if ( !empty( $descricao ) ) {
		$atiid = $db->pegaUm( "select nextval( 'projetos.atividade_atiid_seq' )" );
		$sql = sprintf(
			"insert into projetos.atividade ( atiid, _atiprojeto, atidescricao, usucpf, atiordem ) values ( %d, %d, '%s', '%s', 0 )",
			$atiid,
			$atiid,
			$descricao,
			$_SESSION['usucpf']
		);
		if ( $db->executar( $sql ) ) {
			atividade_atribuir_responsavel( $atiid, PERFIL_GESTOR, array( $_SESSION['usucpf'] ) );
			$db->commit();
		}
	}
	redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'] );
}

// REMOVER
if ( $_REQUEST['remover'] && $usuario_gerente_projeto ) {
	$atiid_remover = (integer) $_REQUEST['remover'];
	$sql = "select count(*) from projetos.atividade where atiidpai = " . $atiid_remover  . " and atistatus = 'A'";
	if ( !$db->pegaUm( $sql ) ) {
		$sql = "update projetos.atividade set atistatus = 'I' where atiid = " . $atiid_remover;
		$db->executar( $sql );
		$db->commit();
	}
	redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'] );
}

// CAPTURA PROJETOS DO USUÁRIO
$where = "";
#if ( !$db->testa_superuser() || !usuario_possui_perfil( PERFIL_CONSULTA, $_SESSION['usucpf'] ) ) {
$consulta = usuario_possui_perfil( PERFIL_ADMINISTRADOR, $_SESSION['usucpf'] ) || usuario_possui_perfil( PERFIL_CONSULTA, $_SESSION['usucpf'] );
if ( !( $db->testa_superuser() || $consulta ) ) {
//	$sql = "
//		select
//			proj.atiid
//		from projetos.usuarioresponsabilidade ur
//			inner join projetos.atividade a on
//				a.atiid = ur.atiid
//			inner join projetos.atividade proj on
//				proj.atiid = a._atiprojeto
//			inner join seguranca.perfilusuario pu on
//				pu.pflcod = ur.pflcod
//		where
//			ur.usucpf = '" . $_SESSION['usucpf'] . "' and
//			a.atistatus = 'A' and
//			proj.atistatus = 'A'
//		group by
//			proj.atiid
//	";
	if($atiidENEM){
		$sqlUnion = "union 
					select a._atiprojeto
					from projetos.atividade a
					inner join projetos.usuarioresponsabilidade ur on
						ur.atiid = a.atiid
					inner join seguranca.perfilusuario pu on
						pu.pflcod = ur.pflcod
					where a.atistatus = 'A' and a.atiid = '$atiidENEM'
					group by a._atiprojeto";
	}
	
	
	$sql = "
		select a._atiprojeto
		from projetos.atividade a
		inner join projetos.usuarioresponsabilidade ur on
			ur.atiid = a.atiid
		inner join seguranca.perfilusuario pu on
			pu.pflcod = ur.pflcod
		where a.atistatus = 'A' and ur.usucpf = '" . $_SESSION['usucpf'] . "'
		group by a._atiprojeto
	$sqlUnion
	";

	$projetos = $db->carregar( $sql );
	$projetos = $projetos ? $projetos : array();
	$where = array( 0 );
	if ( $projetos ) {
		foreach ( $projetos as $item ){
			array_push( $where, $item['_atiprojeto'] );
		}
	}
	$where = " a.atiid in ( " . implode( ',', $where ) . " ) and ";
}

$sql = "
select
	a.atiid,
	a.atidescricao
from projetos.atividade a
where
	" . $where . "
	a.atiidpai is null and
	a.atistatus = 'A' and
	(a.atiid != " . PROJETO_PDE . " AND a.atiid != " . PROJETOENEM . ")
order by
	a.atidescricao
";

$projetos = $db->carregar( $sql );

$projetos = $projetos ? $projetos : array();
foreach ( $projetos as $chave => $projeto ) {
	$sql = "select count(*) from projetos.atividade where atiidpai = " . $projeto['atiid'] . " and atistatus = 'A'";
	$projetos[$chave]['possui_atividade'] = (boolean) $db->pegaUm( $sql );
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, 'Clique no nome do projeto para iniciar o trabalho.' );

function projeto_pegar_gestor( $atividade )
{
	global $db;
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
		PERFIL_GESTOR,
		$atividade
	);
	$usuario = $db->recuperar( $sql );
	return $usuario ? $usuario : array();
}

?>
<script type="text/javascript">
	
	function removerProjeto( id, nome ) {
		if ( confirm( 'Deseja excluir o projeto \'' + nome + '\'?' ) ) {
			window.location.href = '?modulo=principal/projetos&acao=A&remover=' + id;
		}
	}
	
	function selecionarProjeto( id ){
		window.location.href = '?modulo=principal/projetos&acao=A&selecionar_projeto=' + id;
	}
	
	function cadastrarProjeto(){
		var titulo = window.prompt( 'Título do projeto:', 'Novo Projeto' );
		if ( titulo ) {
			window.location.href = '?modulo=principal/projetos&acao=A&cadastrar_projeto=' + titulo;
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
<?php if ( $usuario_gerente_projeto ) : ?>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:none;">
		<tr>
			<td style="padding: 10px;">
				<span
					style="cursor: pointer"
					onclick="cadastrarProjeto();"
					title="novo projeto"
				>
					<img
						align="absmiddle"
						src="/imagens/gif_inclui.gif"
					/>
					Cadastrar Projeto
				</span>
			</td>
		</tr>
	</table>
<?php endif; ?>

<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<?php if ( count( $projetos ) ) : ?>
		<thead>
			<tr bgcolor="#dfdfdf">
				<td align="center" width="70"><b>Ações</b></td>
				<td align="center"><b>Projeto</b></td>
			</tr>
		</thead>
		<?php $cor = ''; ?>
		<?php foreach ( $projetos as $projeto ) : ?>
			<?php $cor = $cor == '#f5f5f5' ? '#fdfdfd' : '#f5f5f5' ; ?>
			<tr
				bgcolor="<?= $cor ?>"
				onmouseout="this.style.backgroundColor='<?= $cor ?>';"
				onmouseover="this.style.backgroundColor='#ffffcc';"
			>
				<td align="center">
				
				
				<?php if ( atividade_verificar_responsabilidade( $projeto['atiid'], $_SESSION['usucpf'] ) ) : ?>
					<img
						align="absmiddle"
						src="/imagens/alterar.gif"
						style="cursor: pointer"
						onclick="window.location.href = '?modulo=principal/projeto&acao=A&atiid=<?= $projeto['atiid'] ?>'"
						title="alterar informações do projeto"
					/>
					<?php if ( !$projeto['possui_atividade'] ): ?>
						<img
							align="absmiddle"
							src="/imagens/excluir.gif"
							style="cursor: pointer"
							onclick="removerProjeto( <?= $projeto['atiid'] ?>, '<?= $projeto['atidescricao'] ?>' );"
							title="excluir projeto"
						/>
					<?php else: ?>
						<img align="absmiddle" src="/imagens/excluir_01.gif"/>
					<?php endif; ?>
				<?php else: ?>
					<img align="absmiddle" src="/imagens/alterar_01.gif"/>
					<img align="absmiddle" src="/imagens/excluir_01.gif"/>
				<?php endif; ?>
				</td>
				<td>
				<? if($atiidENEM && $projeto['atiid'] == $atiidENEM){ ?>
					<a href="javascript:window.location.href='projetos.php?modulo=principal/atividade_/listaEnem&acao=A';" title="selecionar projeto">
				<? }else{ ?>
					<a href="javascript:selecionarProjeto( <?= $projeto['atiid'] ?> );" title="selecionar projeto">
				<? } ?>
						<?= $projeto['atidescricao'] ?>
					</a>
					<br/>
					<?php $gestor = projeto_pegar_gestor( $projeto['atiid'] ); ?>
					<?php if ( $gestor['usunome'] ) : ?>
						<a href="#" onclick="enviar_email( '<?= $gestor['usucpf'] ?>' ); return false;" style="text-decoration: none;">
							<span style="font-size: 9px; font-weight: normal; color: #808080;" title="Clique para enviar e-mail.">
								Gestor: <?= $gestor['usunome'] ?>
							</span>
						</a>
					<?php endif; ?>
				</td>
			</tr>
		<?php endforeach; ?>
	<?php else : ?>
		<tr>
			<td style="text-align:center; padding:15px; background-color:#fafafa; color:#404040; font-weight:bold; font-size: 10px;" colspan="2">
				Não há projetos.
			</td>
		</tr>
	<?php endif; ?>
</table>