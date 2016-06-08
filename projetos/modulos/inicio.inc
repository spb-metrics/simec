<?php

/**
 * Página de apresentação do módulo. 
 *
 * @author Renê de Lima Barbosa <renebarbosa@mec.gov.br> 
 * @since 22/03/2207
 */

// configurações

$cor_avaliacao = array(
	1 => 'verde',
	2 => 'amarelo',
	3 => 'vermelho'
);

// identifica e executa ações

if ( $_REQUEST['exibir'] ) {
	dbg( $_REQUEST['exibir'] );
}

if ( $_REQUEST['alterar'] ) {
	dbg( $_REQUEST['alterar'] );
}

if ( $_REQUEST['excluir'] ) {
	$sql = sprintf(
		"update projetos.projetoespecial set pjestatus='I' where pjeid='%s'",
		$_REQUEST['excluir']
	);
    if( $db->executar( $sql ) ) {
		$db->commit();
		$db->sucesso( $modulo );
    } else {
		$db->insucesso( 'Ocorreu um erro na exclusão do projeto. A operação foi cancelada.' );
		exit();
    }
}

include APPRAIZ . "includes/cabecalho.inc";

// carrega lista de projetos
$sql = sprintf(
	"select
		pe.pjeid, pe.pjecod, pe.pjedsc, pe.pjeprevistoano,
		ts.tpsdsc,
		ug.ungabrev
	from projetos.projetoespecial pe
	inner join public.tiposituacao ts on ts.tpscod = pe.tpscod
	inner join public.unidadegestora ug on ug.ungcod = pe.ungcod
	order by ug.ungabrev, pe.pjecod"
);
$projetos = $db->carregar( $sql );
if ( !$projetos ) {
	$projetos = array();
}

?>
<br>
<link rel="stylesheet" type="text/css" href="../includes/listagem2.css">
<script language="JavaScript" src="../includes/funcoes.js"></script>
<?php monta_titulo( 'Plano de Desenvolvimento da Educação', 'Gerenciamento de Projetos' ); ?>

	<form name="formulario" method="post">
		<input type="hidden" name="alterar" value="0"/>
		<input type="hidden" name="exibir" value="0"/>
		<input type="hidden" name="excluir" value="0"/>
		<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
			<thead>
				<tr>
					<td valign="top" class="title">&nbsp;</td>
					<td valign="top" class="title"><strong>Código</strong></td>
					<td valign="top" class="title"><strong>Descrição</strong></td>
					<td valign="top" class="title"><strong>Coordenador</strong></td>
					<td valign="top" class="title"><strong>Status</strong></td>      
					<td valign="top" class="title"><strong>Acompanhamento</strong></td>       
				</tr>
			</thead>
			<tbody>
				<?php foreach( $projetos as $indice => $projeto ): ?>
					<tr style="background-color:<?= $indice % 2 ? '#efefef' : '#ffffff' ?>">
						<td style="text-align:center">
							<?php if( true ): ?>
								<a href="projetos.php?modulo=principal/projeto/planodetrabalho&acao=A&pjeid=<?= $projeto['pjeid'] ?>" title="Gerenciar Tarefas"><img src="../imagens/consultar.gif" style="border:0"/></a>
								&nbsp;
								<a href="projetos.php?modulo=principal/projeto/cadprojespec&acao=A&pjeid=<?= $projeto['pjeid'] ?>" title="Alterar Projeto"><img src="../imagens/alterar.gif" style="border:0"/></a>
								&nbsp;
								<a href="#" title="Excluir Projeto" onclick="excluir_projeto( '<?= $projeto['pjeid'] ?>', '<?= $projeto['pjedsc'] ?>' )"><img src="../imagens/excluir.gif" style="border:0"/></a>
							<?php else: ?>
								&nbsp;
							<?php endif; ?>
						</td>
						<td><?= $projeto['ungabrev'] ?>&nbsp;<?= $projeto['pjecod'] ?></td>
						<td><?= $projeto['pjedsc'] ?></td>
						<td>
							<?php
								// identifica o responsável pelo projeto
								$sql = sprintf(
									"select u.usucpf, u.usunome, u.usufoneddd, u.usufonenum
									from projetos.usuarioresponsabilidade ur
									inner join seguranca.usuario u on u.usucpf = ur.usucpf
									where ur.pjeid = '%s'",
									$projeto['pjeid']
								);
								$usuario = $db->pegaLinha( $sql );
							?>
							<?php if( $usuario ): ?>
								<?= $usuario['usunome'] ?>
								<br/>
								<img src="../imagens/email.gif" title="" border="0" onclick="envia_email( '<?= $usuario['usucpf'] ?>' );"/>
								&nbsp;<?= $usuario['usufoneddd'] ?> <?= $usuario['usufonenum'] ?>
							<?php else: ?>
								-
							<?php endif; ?>
						</td>
						<td><?= $projeto['tpsdsc'] ?></td>
						<td>
							<?php if( $projeto['pjeprevistoano'] ): ?>
								<?php
									// obtém informações sobre a avaliação do projeto
									$sql = sprintf(
										"select ag.avgtexto, ag.avgrealizado, ag.corcod
										from projetos.avaliacaogenerico ag
										where ag.avgliberada = 't' and ag.avgstatus='A' and ag.pjeid = '%s'
										order by ag.avgdata desc
										limit 1",
										$projeto['pjeid']
									);
									$realizacao = $db->pegaLinha( $sql );
									if ( $projeto['pjeprevistoano'] != 0 ) {
										$realizado = ( $realizacao['avgrealizado'] / $projeto['pjeprevistoano'] ) * 100;
									}
									$realizacao['corcod'] = array_key_exists( $realizacao['corcod'], $cor_avaliacao ) ? $realizacao['corcod'] : 3;
								?>
								<span onmouseover="return escape('<?= $realizacao['avgtexto'] ?>')">
									<img src="../imagens/p_<?= $cor_avaliacao[$realizacao['corcod']] ?>.gif" style="vertical-align:bottom"/>
									&nbsp;<?= number_format( $realizado, 0 ) ?> %
								</span>
							<?php else: ?>
								-
							<?php endif; ?>
						</td>
					</tr>
				<?php endforeach; ?>
				<tr>
					<td class="title" colspan="6" style="vertical-align:top; background-color:#ffffff; padding:10px">
						<a href="#">
						<img src="../imagens/gif_inclui.gif" onclick="exibir_projeto( '<?= $projeto['pjeid'] ?>' );" style="vertical-align:bottom; border:0"/>
						&nbsp;Cadastrar Projeto
						</a>
					</td>
				</tr>
			</tbody>
		</table>
	</form>
	<script type="text/javascript" language="javascript">
		
		function cadastrar_projeto(){
			return;
		}
		
		function excluir_projeto( pjeid, pjedsc ){
			if( window.confirm( "Excluir o projeto "+ pjedsc +"?" ) ) {
				document.formulario.excluir.value = pjeid;
				document.formulario.submit();
			} else {
				document.formulario.excluir.value = 0;
			}
		}
		
		function exibir_projeto( pjeid )
		{
			location.href = 'projetos.php?modulo=principal/projeto/cadprojespec&acao=A&pjeid='+pjeid;
		}
		
		function envia_email( usucpf ){
			return;
		}
		
	</script>