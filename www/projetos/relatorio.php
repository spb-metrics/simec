<?php

function atividade_pegar_responsaveis( $atividade, $perfil ){
	global $db;
	$sql =
<<<EOT
		select
			u.*
		from projetos.usuarioresponsabilidade ur
			inner join seguranca.usuario u on
				u.usucpf = ur.usucpf
		where
			ur.rpustatus = 'A' and
			u.suscod = 'A' and
			ur.atiid = %d and
			ur.pflcod = %d
EOT;
	$sql = sprintf( $sql, $atividade, $perfil );
	$usuarios = $db->carregar( $sql );
	return $usuarios ? $usuarios : array();
}

define( 'PERFIL_GESTOR',  82 );
define( 'PERFIL_GERENTE', 90 );
define( 'PERFIL_EQUIPE_APOIO_GESTOR',  85 );
define( 'PERFIL_EQUIPE_APOIO_GERENTE', 91 );

// INDICA OS ESTADOS DAS ATIVIDADES
define( 'STATUS_NAO_INICIADO', 1 );
define( 'STATUS_EM_ANDAMENTO', 2 );
define( 'STATUS_SUSPENSO',     3 );
define( 'STATUS_CANCELADO',    4 );
define( 'STATUS_CONCLUIDO',    5 );

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

// carrega as funções do módulo
include_once '_funcoes.php';
include_once '_componentes.php';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

//$ids = array( 337, 756, 526, 1209, 3032 );
$ids = array( 337, 765, 526, 1209, 3032 );

?>
<?php foreach( $ids as $id ): ?>
	<table border="0" width="100%">
		<?php foreach( atividade_pegar_filhas( 3, $id ) as $atividade ): ?>
		<?php
		$gerencia = atividade_pegar_responsaveis( $atividade['atiid'], PERFIL_GERENTE );
		$apoio = atividade_pegar_responsaveis( $atividade['atiid'], PERFIL_EQUIPE_APOIO_GERENTE );
		if ( count( $gerencia ) == 0 && count( $apoio ) == 0 ) {
			continue;
		}
		?>
		<tr style="font-weight: bold;">
			<td width="10%" valign="top" align="left" rowspan="2"><?= $atividade['numero'] ?></td>
			<td width="80%" valign="top"><?= $atividade['atidescricao'] ?></td>
		</tr>
		<tr>
			<td width="80%" valign="top">
				<ul>
					<?php foreach( $gerencia as $usuario ): ?>
					<li><?= $usuario['usucpf'] ?> <?= $usuario['usunome'] ?> (responsável)</li>
					<?php endforeach;?>
				</ul>
				<ul>
					<?php foreach( $apoio as $usuario ): ?>
					<li><?= $usuario['usucpf'] ?> <?= $usuario['usunome'] ?></li>
					<?php endforeach;?>
				</ul>
			</td>
		</tr>
		<?php endforeach;?>
	</table>
<?php endforeach;?>