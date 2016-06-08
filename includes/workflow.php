<?php

// carrega constantes necessárias

include_once APPRAIZ . "includes/workflow_constantes.php";
//include_once APPRAIZ . "includes/workflow_teste.php";

// funcoes gerais

function wf_acaoFoiExecutada( $docid, $esdidorigem, $esdiddestino )
{
	global $db;
	$docid = (integer) $docid;
	$esdidorigem = (integer) $esdidorigem;
	$esdiddestino = (integer) $esdiddestino;
	$sql = "
		select
			count(*)
		from workflow.historicodocumento h
			inner join workflow.acaoestadodoc a on a.aedid = h.aedid
		where
			h.docid = " . $docid . " and
			a.esdidorigem = " . $esdidorigem . " and
			a.esdiddestino = " . $esdiddestino . "
	";
	return $db->pegaUm( $sql ) > 0;
}

function wf_acaoNecessitaComentario( $docid, $esdiddestino )
{
	global $db;
	static $acao = array();
	$docid = (integer) $docid;
	$documento = wf_pegarDocumento( $docid );
	$esdidorigem = $documento['esdid'];
	$esdiddestino = (integer) $esdiddestino;
	$acao = wf_pegarAcao( $esdidorigem, $esdiddestino );
	return $acao['esdsncomentario'];
}

function wf_acaoNecessitaComentario2( $aedid )
{
	global $db;
	static $acao = array();

	$acao = wf_pegarAcao2( $aedid );

	return $acao['esdsncomentario'];
}

function wf_acaoPossivel( $docid, $esdiddestino, array $dados )
{
	global $db;
	$esdiddestino = (integer) $esdiddestino;
	// carrega ação correpondente
	$estadoOrigem = wf_pegarEstadoAtual( $docid );
	$acao = wf_pegarAcao( $estadoOrigem['esdid'], $esdiddestino);
	// verifica se ação existe
	if ( !$acao )
	{
		return false;
	}
	// verifica se usuário possui perfil da ação
	wf_verificarPerfil( $acao['aedid'] );
	// realiza condição extra
	return wf_realizarVerificacao( $acao['aedid'], $dados );
}

function wf_acaoPossivel2( $docid, $aedid, array $dados )
{
	global $db;
	// verifica se usuário possui perfil da ação
	wf_verificarPerfil( $aedid );
	// realiza condição extra
	
	return ((wf_realizarVerificacao( $aedid, $dados ) === true)?true:false);
}


function wf_alterarEstado( $docid, $aedid, $cmddsc = '', array $dados )
{
	global $db;
	$docid = (integer) $docid;
	$aedid = (integer) $aedid;
	$cmddsc = trim( $cmddsc );
	$cmddsc = str_replace( "'", "\\'", $cmddsc );

	$acao = wf_pegarAcao2( $aedid );
	$esdiddestino = (integer) $acao['esdiddestino'];

	// verifica se ação é possível
	if ( !wf_acaoPossivel2( $docid, $aedid, $dados ) )
	{
		return false;
	}
	
	// verifica necessidade de comentario
	$necessitaComentario = wf_acaoNecessitaComentario2( $aedid );
	if ( $necessitaComentario && $cmddsc == "" )
	{
		return false;
	}

	// inicia alteração de estado
	$documento = wf_pegarDocumento( $docid );
	
	// cria log no histórico
	$sqlHistorico = "
		insert into workflow.historicodocumento
		( aedid, docid, usucpf, htddata )
		values ( " . $aedid . ", " . $docid . ", '" . $_SESSION['usucpf'] . "', now() )
		returning hstid
	";
	$hstid = (integer) $db->pegaUm( $sqlHistorico );
	if ( !$hstid )
	{
		$db->rollback();
		return false;
	}
	
	// cria comentario, quando necessario
	if ( $necessitaComentario )
	{
		$sqlComentario = "
			insert into workflow.comentariodocumento
			( docid, hstid, cmddsc, cmddata, cmdstatus )
			values ( " . $docid . ", " . $hstid . ", '" . addslashes($cmddsc) . "', now(), 'A' )
		";
		if ( !$db->executar( $sqlComentario ) )
		{
			$db->rollback();
			return false;
		}
	}
	
	// atualiza documento
	$sqlDocumento = "
		update workflow.documento
		set esdid = " . $esdiddestino . "
		where docid = " . $docid;
	
	if ( !$db->executar( $sqlDocumento ) )
	{
		$db->rollback();
		return false;
	}
	
	// realiza pos-acao
	if ( !wf_realizarPosAcao( $aedid, $dados ) )
	{
		$db->rollback();
		return false;
	}
	
	$db->commit();
	return true;
}


function wf_desenhaBarraNavegacao( $docid, array $dados, $ocultar = null )
{
	/*
	 * $ocultar - Define quais areas serão ocultadas. ex.: $ocultar['historico'] = true;
	 * 
	 * --- Definidas ---
	 * historico       : Oculta linha contendo informações obre o historico
	 * acaosemcondicao : Oculta linha contendo a ação cuja a condição para tramitação não esteja atendida 
	 */
	
	global $db;
	$docid = (integer) $docid;
	
	// captura dados gerais
	$documento = wf_pegarDocumento( $docid );
	if ( !$documento )
	{
		?>
		<table align="center" border="0" cellpadding="5" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #d0d0d0; width: 80px;">
			<tr>
				<td style="text-align: center;">
					Documento inexistente!
				</td>
			</tr>
		</table>
		<br/><br/>
		<?php
		return;
	}

	$estadoAtual = wf_pegarEstadoAtual( $docid );
	//$estados = wf_pegarProximosEstadosPossiveis( $docid, $dados );
	$estados = wf_pegarProximosEstados( $docid, $dados );
	$modificacao = wf_pegarUltimaDataModificacao( $docid );
	$usuario = wf_pegarUltimoUsuarioModificacao( $docid );
	$comentario = trim( substr( wf_pegarComentarioEstadoAtual( $docid ), 0, 50 ) ) . "...";
	
	$dadosHtml = serialize( $dados );
	?>
	<script type="text/javascript">
		
		function wf_atualizarTela( mensagem, janela )
		{
			janela.close();
			alert( mensagem );
			window.location.reload();
		}
		
		function wf_alterarEstado( aedid, docid, esdid, acao )
		{
			if(acao) acao = acao.toLowerCase();
			if ( !confirm( 'Deseja realmente ' + acao + ' ?' ) )
			{
				return;
			}
			var url = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/geral/workflow/alterar_estado.php' +
				'?aedid=' + aedid +
				'&docid=' + docid +
				'&esdid=' + esdid +
				'&verificacao=<?php echo urlencode( $dadosHtml ); ?>';
			var janela = window.open(
				url,
				'alterarEstado',
				'width=550,height=500,scrollbars=no,scrolling=no,resizebled=no'
			);
			janela.focus();
		}
		
		function wf_exibirHistorico( docid )
		{
			var url = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/geral/workflow/historico.php' +
				'?modulo=principal/tramitacao' +
				'&acao=C' +
				'&docid=' + docid;
			window.open(
				url,
				'alterarEstado',
				'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
			);
		}
		
	</script>
	<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
		<?php if ( count( $estadoAtual ) ) : ?>
			<tr style="background-color: #c9c9c9; text-align:center;">
				<td style="font-size:7pt; text-align:center;">
					<span title="estado atual">
						<b>estado atual</b>
					</span>
				</td>
			</tr>
			<tr style="text-align:center;">
				<td style="font-size:7pt; text-align:center;">
					<span title="estado atual">
						<?php echo $estadoAtual['esddsc'] ?>
					</span>
				</td>
			</tr>
		<?php endif; ?>
		<tr style="background-color: #c9c9c9; text-align:center;">
			<td style="font-size:7pt; text-align:center;">
				<span title="estado atual">
					<b>ações</b>
				</span>
			</td>
		</tr>
		<?php if ( count( $estados ) ) : ?>
			<?php $nenhumaacao = true; ?>
			<?php foreach ( $estados as $estado ) : 
						$action = wf_acaoPossivel( $docid, $estado['esdid'], $dados ); ?>
						
				<?php if($action === true) : ?>
				<?php $nenhumaacao = false; ?>
				<tr>
					<td style="font-size: 7pt; text-align: center; border-top: 2px solid #d0d0d0;" onmouseover="this.style.backgroundColor='#ffffdd';" onmouseout="this.style.backgroundColor='';">
						<a
							href="#"
							alt="<?php echo $estado['aeddscrealizar'] ?>"
							title="<?php echo $estado['aeddscrealizar'] ?>"
							onclick="wf_alterarEstado( '<?php echo $estado['aedid'] ?>', '<?php echo $docid ?>', '<?php echo $estado['esdid'] ?>', '<?php echo $estado['aeddscrealizar'] ?>' );"
						><?php echo (($estado['aedicone'])?"<img align=absmiddle src=../imagens/workflow/".$estado['aedicone']." border=0><br/>":""); ?> <?php echo $estado['aeddscrealizar'] ?></a>
					</td>
				</tr>
				<?php else :?>
				
					<? if($action === false) : ?>
					
						<? if(!$ocultar['acaosemcondicao']) : ?>
						<?php $nenhumaacao = false; ?>
							<tr>
								<td style="font-size: 7pt; color: #909090; border-top: 2px solid #d0d0d0; text-align: center;" onclick="alert( '<?php echo $estado['aedobs']; ?>' )" onmouseover="return escape('<? echo $estado['aedobs']; ?>');">
								<?php echo (($estado['aedicone'])?"<img align=absmiddle src=../imagens/workflow/".$estado['aedicone']." border=0><br/>":""); ?> <?php echo $estado['aeddscrealizar'] ?>
								</td>
							</tr>
						<? endif; ?>
						
					<?php else :?>
					<?php $nenhumaacao = false; ?>
					<tr>
						<td style="font-size: 7pt; color: #909090; border-top: 2px solid #d0d0d0; text-align: center;" onclick="alert( '<?php echo $action; ?>' )" onmouseover="return escape('<? echo $action; ?>');">
							<?php echo (($estado['aedicone'])?"<img align=absmiddle src=../imagens/workflow/".$estado['aedicone']." border=0><br/>":""); ?> <?php echo $estado['aeddscrealizar'] ?>
						</td>
					</tr>
					<?php endif; ?>
					
				<?php endif; ?>
			<?php endforeach; ?>
			<?php if($nenhumaacao) : ?>
			<tr>
				<td style="font-size: 7pt; text-align: center; border-top: 2px solid #d0d0d0;">
					nenhuma ação disponível para o documento
				</td>
			</tr>
			<?php endif; ?>
		<?php else: ?>
			<tr>
				<td style="font-size: 7pt; text-align: center; border-top: 2px solid #d0d0d0;">
					nenhuma ação disponível para o documento
				</td>
			</tr>
		<?php endif; ?>
		<? if(!$ocultar['historico']) { ?>
		<tr style="background-color: #c9c9c9; text-align:center;">
			<td style="font-size:7pt; text-align:center;">
				<span title="estado atual">
					<b>histórico</b>
				</span>
			</td>
		</tr>
		<tr style="text-align:center;">
			<td style="font-size:7pt; border-top: 2px solid #d0d0d0;">
				<img
					style="cursor: pointer;"
					src="http://<?php echo $_SERVER['SERVER_NAME'] ?>/imagens/fluxodoc.gif"
					title="<?php echo $usuario['usunome'] . " - " . $modificacao . " - " . htmlentities( $comentario ); ?>"
					onclick="wf_exibirHistorico( '<?php echo $docid ?>' );"
				/>
			</td>
		</tr>
		<? } ?>
	</table>
	<br/><br/>
	<?php
}

// funções de captura de dados

function wf_pegarAcao( $esdidorigem, $esdiddestino)
{
	global $db;
	static $acao = array();
	$esdidorigem = (integer) $esdidorigem;
	$esdiddestino = (integer) $esdiddestino;
	$chave = $esdidorigem . "." . $esdiddestino;
	if ( !array_key_exists( $chave, $acao ) )
	{	
			$sql = "
			select
				a.aedid,
				a.esdidorigem,
				a.esdiddestino,
				a.aeddscrealizar,
				a.aeddscrealizada,
				a.aedcondicao,
				a.esdsncomentario,
				a.aedposacao,
				ed.esddsc as esddscdestino,
				eo.esddsc as esddscorigem
			from workflow.acaoestadodoc a
				inner join workflow.estadodocumento eo on eo.esdid = a.esdidorigem
				inner join workflow.estadodocumento ed on ed.esdid = a.esdiddestino
			where
				ed.esdstatus   = 'A' and
				a.aedstatus    = 'A' and
				a.esdidorigem  = " . $esdidorigem . " and
				a.esdiddestino = " . $esdiddestino;
		$acaoTemp = $db->recuperar( $sql );
		
		if ( !$acaoTemp )
		{
			$acaoTemp = array();
		}
		else
		{
			$acaoTemp['esdsncomentario'] = $acaoTemp['esdsncomentario'] == 't';
		}
		$acao[$chave] = $acaoTemp;
	}
	return $acao[$chave];
}

function wf_pegarAcao2( $aedid )
{
	global $db;
	static $acao = array();
	$aedid = (integer) $aedid;
	if ( !array_key_exists( $aedid, $acao ) )
	{
		$sql = "
			select
				a.aedid,
				a.esdidorigem,
				a.esdiddestino,
				a.aeddscrealizar,
				a.aeddscrealizada,
				a.aedcondicao,
				a.esdsncomentario,
				a.aedposacao,
				ed.esddsc as esddscdestino,
				eo.esddsc as esddscorigem
			from workflow.acaoestadodoc a
				inner join workflow.estadodocumento eo on eo.esdid = a.esdidorigem
				inner join workflow.estadodocumento ed on ed.esdid = a.esdiddestino
			where
				a.aedid  = " . $aedid;
		$acaoTemp = $db->recuperar( $sql );
		if ( !$acaoTemp )
		{
			$acaoTemp = array();
		}
		else
		{
			$acaoTemp['esdsncomentario'] = $acaoTemp['esdsncomentario'] == 't';
		}
		$acao[$aedid] = $acaoTemp;
	}
	return $acao[$aedid];
}

function wf_pegarAcaoPorId( $aedid )
{
	
	global $db;
	static $acao = array();
	$aedid = (integer) $aedid;
	if ( !array_key_exists( $aedid, $acao ) )
	{
		$sql = "
			select
				aedid,
				esdidorigem,
				esdiddestino,
				aeddscrealizar,
				aeddscrealizada,
				aedcondicao,
				esdsncomentario,
				aedposacao
			from workflow.acaoestadodoc
			where
				aedid = " . $aedid;
		$acaoTemp = $db->recuperar( $sql );
		if ( !$acaoTemp )
		{
			$acaoTemp = array();
		}
		else
		{
			$acaoTemp['aedsncomentario'] = $acaoTemp['aedsncomentario'] == 't';
		}
		$acao[$aedid] = $acaoTemp;
	}
	return $acao[$aedid];
}

function wf_pegarComentarioEstadoAtual( $docid )
{
	global $db;
	static $comentario = array();
	$docid = (integer) $docid;
	if ( !array_key_exists( $docid, $comentario ) )
	{
		$sql = "
			select
				cmddsc
			from workflow.historicodocumento hd
				left join workflow.comentariodocumento cd on
					cd.hstid = hd.hstid
			where
				hd.docid = " . $docid . "
			order by
				hd.htddata desc
			limit 1
		";
		$comentario[$docid] = (string) $db->pegaUm( $sql );
	}
	return $comentario[$docid];
}

function wf_pegarDocumento( $docid )
{
	global $db;
	static $documento = array();
	$docid = (integer) $docid;
	if ( !array_key_exists( $docid, $documento ) )
	{
		$sql = "
			select
				docid,
				docdsc,
				esdid
			from workflow.documento
			where
				docid = " . $docid;
		$documentoTemp = $db->recuperar( $sql );
		$documento[$docid] = $documentoTemp ? $documentoTemp : array();
	}
	return $documento[$docid];
}

function wf_pegarEstadoAtual( $docid )
{
	global $db;
	static $estado = array();
	$docid = (integer) $docid;
	if ( !array_key_exists( $docid, $estado ) )
	{
		$sql = "
			select
				ed.esdid,
				ed.esddsc
			from workflow.documento d
				inner join workflow.estadodocumento ed on ed.esdid = d.esdid
			where
				d.docid = " . $docid;
		$estadoTemp = $db->recuperar( $sql );
		$estado[$docid] = $estadoTemp ? $estadoTemp : array();
	}
	return $estado[$docid];
}

function wf_pegarEstadoInicial( $tpdid )
{
	global $db;
	$tpdid = (integer) $tpdid;
	$sql = "
		select
			esdid,
			esddsc
		from workflow.estadodocumento
		where
			tpdid = " . $tpdid . " and
			esdstatus = 'A'
		order by
			esdordem
		limit 1
	";
	$dados = $db->recuperar( $sql );
	return $dados ? $dados : array();
}

function wf_pegarHistorico( $docid )
{
	global $db;
	$docid = (integer) $docid;
	$sql = "
		select
			ed.esddsc,
			ac.aeddscrealizada,
			us.usunome,
			hd.htddata,
			cd.cmddsc
		from workflow.historicodocumento hd
			inner join workflow.acaoestadodoc ac on
				ac.aedid = hd.aedid
			inner join workflow.estadodocumento ed on
				ed.esdid = ac.esdidorigem
			inner join seguranca.usuario us on
				us.usucpf = hd.usucpf
			left join workflow.comentariodocumento cd on
				cd.hstid = hd.hstid
		where
			hd.docid = " . $docid . "
		order by
			hd.htddata asc
	";
	$dados = $db->carregar( $sql );
	if ( !$dados )
	{
		return array();
	}
	foreach ( $dados as &$dado )
	{
		$dataHora = explode( ' ', $dado['htddata'] );
		$hora = substr( $dataHora[1], 0, 8 );
		$data = explode( '-', $dataHora[0] );
		$data = $data[2] . "/" . $data[1] . "/" . $data[0];
		$dado['htddata'] = $data . " " . $hora;
	}
	return $dados;
}

function wf_pegarTipo( $tpdid )
{
	global $db;
	$tpdid = (integer) $tpdid;
	$sql = "
		select
			tpdid,
			tpddsc
		from workflow.tipodocumento
		where
			tpdid = " . $tpdid;
	$dados = $db->recuperar( $sql );
	return $dados ? $dados : array();
}

function wf_pegarUltimaDataModificacao( $docid )
{
	global $db;
	static $dataMod = array();
	$docid = (integer) $docid;
	if ( !array_key_exists( $docid, $dataMod ) )
	{
		$sql = "
			select
				max ( htddata )
			from workflow.historicodocumento
			where
				docid = " . $docid;
		$dataTemp = $db->pegaUm( $sql );
		if ( !$dataTemp )
		{
			$dataTemp = "";
		}
		else
		{
			$dataHora = explode( ' ', $dataTemp );
			$hora = substr( $dataHora[1], 0, 8 );
			$data = explode( '-', $dataHora[0] );
			$data = $data[2] . "/" . $data[1] . "/" . $data[0];
			$dataTemp = $data . " " . $hora;
		}
		$dataMod[$docid] = $dataTemp;
	}
	return $dataMod[$docid];
}

function wf_pegarUltimoUsuarioModificacao( $docid )
{
	global $db;
	static $usuario = array();
	$docid = (integer) $docid;
	if ( !array_key_exists( $docid, $usuario ) )
	{
		$sql = "
			select
				u.usucpf,
				u.usunome
			from workflow.historicodocumento hd
				inner join seguranca.usuario u on u.usucpf = hd.usucpf
			where
				docid = " . $docid . "
			order by hd.htddata desc
			limit 1
		";
		$usuarioTemp = $db->recuperar( $sql );
		$usuario[$docid] = $usuarioTemp ? $usuarioTemp : array();
	}
	return $usuario[$docid];
}

// funções de apoio às funções gerais e de captura de dados

function wf_cadastrarDocumento( $tpdid, $docdsc )
{
	global $db;
	$tpdid = (integer) $tpdid;
	$docdsc = str_replace( "'", "\\'", $docdsc );
	// verifica se existe tipo
	$tipo = wf_pegarTipo( $tpdid );	
	if ( !$tipo['tpdid'] )
	{
		return null;
	}
	// verifica se existe estado inicial
	$estadoInicial = wf_pegarEstadoInicial( $tpdid );
	$esdid = (integer) $estadoInicial['esdid'];
	if ( !$esdid )
	{
		return null;
	}
	// grava documento
	$sql = "
		insert into workflow.documento
		( tpdid, esdid, docdsc )
		values ( " . $tpdid . ", " . $esdid . ", '" . $docdsc . "' )
		returning docid
	";
	
	
	$docid = $db->pegaUm( $sql );
	return $docid ? $docid : null;
}

function wf_pegarProximosEstados( $docid, array $dados = array() )
{
	global $db;
	$docid = (integer) $docid;
	$documento = wf_pegarDocumento( $docid );
	$esdidorigem = (integer) $documento['esdid'];
	$sql = "
		select
			a.aedid,
			a.aeddscrealizar,
			ed.esdid,
			ed.esddsc,
			a.aedobs,
			a.aedicone
		from workflow.acaoestadodoc a
			inner join workflow.estadodocumento ed on
				ed.esdid = a.esdiddestino
		where
			esdidorigem = " . $esdidorigem . " and
			aedstatus = 'A' and
			aedvisivel = true
		";
	// captura os estados possíveis
	$estados = $db->carregar( $sql );
	$estados = $estados ? $estados : array();
	$estadosFinais = array();
	// para cada estado possível realiza a verificao externa
	foreach ( $estados as $estado )
	{
		// verifica se usuário possui perfil da ação
		if ( wf_verificarPerfil( $estado['aedid'] ) )
		{
			array_push( $estadosFinais, $estado );
		}
	}
	return $estadosFinais;
}

/*
function wf_pegarProximosEstadosPossiveis( $docid, array $dados = array() )
{
	global $db;
	$docid = (integer) $docid;
	$documento = wf_pegarDocumento( $docid );
	$esdidorigem = (integer) $documento['esdid'];
	$sql = "
		select
			a.aedid,
			a.aeddscrealizar,
			ed.esdid,
			ed.esddsc,
			a.aedobs
		from workflow.acaoestadodoc a
			inner join workflow.estadodocumento ed on
				ed.esdid = a.esdiddestino
		where
			esdidorigem = " . $esdidorigem . " and
			aedstatus = 'A'
		";
	// captura os estados possíveis
	$estados = $db->carregar( $sql );
	$estados = $estados ? $estados : array();
	$estadosFinais = array();
	// para cada estado possível realiza a verificao externa
	foreach ( $estados as $estado )
	{
		// caso a verifica externa retorne false o estado é descartado
		if ( wf_acaoPossivel( $docid, $estado['esdid'], $dados ) )
		{
			unset( $estado['aedid'] );
			array_push( $estadosFinais, $estado );
		}
	}
	return $estadosFinais;
}
*/

function wf_realizarVerificacao( $aedid, array $dados )
{
	global $db;
	$aedid = (integer) $aedid;
	
	// verifica se há condição a ser realizada
	$acao = wf_pegarAcaoPorId( $aedid );
	$aedcondicao = trim( $acao['aedcondicao'] );
	
	// captura dados da chamada
	$chamada = wf_tratarChamada( $aedcondicao, $dados );
	$funcao = $chamada['funcao'];
	$parametros = $chamada['parametros'];
	
	if ( !$funcao )
	{
		return true;
	}
	else
	{
		// realiza a verificação externa
		return call_user_func_array( $funcao, $parametros );
	}
}

function wf_realizarPosAcao( $aedid, $dados )
{
	global $db;
	$aedid = (integer) $aedid;

	// verifica se há condição a ser realizada
	$acao = wf_pegarAcaoPorId( $aedid );
	$aedposacao = trim( $acao['aedposacao'] );

	// captura dados da chamada
	$chamada    = wf_tratarChamada( $aedposacao, $dados );
	$funcao     = $chamada['funcao'];
	$parametros = $chamada['parametros'];

    $return     = true;
    
	if ( $funcao )
		$return = call_user_func_array( $funcao, $parametros );

	return $return;
}

function wf_tratarChamada( $chamada, array $dados )
{
	
	// verifica se formato básico da condição
	$posAbre = strpos( $chamada, "(" );
	$posFecha = strrpos( $chamada, ")" );
	if ( $posAbre === false || $posFecha === false )
	{
		return array(
			"funcao" => "",
			"parametros" => array()
		);
	}
	
	// captura a funcao
	$funcao = trim( substr( $chamada, 0, $posAbre ) );
	
	// verifica se função é "chamável" 
	if ( !is_callable( $funcao ) )
	{
		return array(
			"funcao" => "",
			"parametros" => array()
		);
	}
	
	// captura parâmetros
	$parametrosCru = substr( $chamada, $posAbre + 1, $posFecha - $posAbre - 1 );
	$parametrosCru = explode( ",", trim( $parametrosCru ) );
	$parametrosCru = array_map( "trim", $parametrosCru );
	$parametros = array();
	foreach ( $parametrosCru as $item )
	{
		if ( array_key_exists( $item, $dados ) )
		{
			array_push( $parametros, $dados[$item] );
		}
	}
	
	return array(
		"funcao" => $funcao,
		"parametros" => $parametros
	);
}

function wf_verificarPerfil( $aedid )
{
	global $db;
	$aedid = (integer) $aedid;
	if ( $db->testa_superuser() )
	{
		return true;
	}
	$sql = "
		select
			pflcod
		from workflow.estadodocumentoperfil
		where
			aedid = " . $aedid . "
		group by
			pflcod
	";
	$perfis = $db->carregar( $sql );
	$perfis = $perfis ? $perfis : array();
	$pflcods = array();
	foreach ( $perfis as $perfil )
	{
		array_push( $pflcods, $perfil['pflcod'] );
	}
	if ( count( $pflcods ) == 0 )
	{
		return false;
	}
	$sql = "
		select
			count(*)
		from seguranca.perfilusuario
		where
			usucpf = '" . $_SESSION['usucpf'] . "' and
			pflcod in ( " . implode( ",", $pflcods ) . " )
	";
	return !!$db->pegaUm( $sql );
}

// mensagem

function wf_registrarMensagem( $mensagem ){
	$_SESSION["wf"]["mensagem"] = $mensagem;
}

function wf_pegarMensagem(){
	$mensagem = $_SESSION["wf"]["mensagem"];
	$_SESSION["wf"]["mensagem"] = null;
	return (string) $mensagem;
}





function wf_gerencimentoFluxo($tpdid, $docids = array(), $cxentrada = 'pendencias', $parametros = array()) {
	global $db;
	
	$sql = "SELECT * FROM workflow.tipodocumento WHERE tpdid='".$tpdid."'";
	$tipodocumento = $db->pegaLinha($sql);
	
	if($docids['pendencias'][0]) {
		foreach($docids['pendencias'] as $docid) {
			
			$arrDocumentos[] = $db->pegaLinha("SELECT CASE WHEN (SELECT to_char(htddata, 'dd/mm/YYYY') as data FROM workflow.historicodocumento WHERE docid=doc.docid ORDER BY htddata DESC LIMIT 1) IS NULL THEN to_char(docdatainclusao, 'dd/mm/YYYY') ELSE (SELECT to_char(htddata, 'dd/mm/YYYY') as data FROM workflow.historicodocumento WHERE docid=doc.docid ORDER BY htddata DESC LIMIT 1) END as data_cadastro,
													  doc.docdsc as descricao_documento,
													  esd.esddsc as descricao_estado,
													  doc.docid  as documento
											   FROM workflow.documento doc 
											   INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid 
											   WHERE doc.docid = '".$docid."' 
											   ORDER BY data_cadastro");
			
			
		}
		
	}
	
	if($docids['futuras']) {
		foreach($docids['futuras'] as $docid) {
			
			$arrDocumentosFut[] = $db->pegaLinha("SELECT CASE WHEN (SELECT to_char(htddata, 'dd/mm/YYYY') as data FROM workflow.historicodocumento WHERE docid=doc.docid ORDER BY htddata DESC LIMIT 1) IS NULL THEN to_char(docdatainclusao, 'dd/mm/YYYY') ELSE (SELECT to_char(htddata, 'dd/mm/YYYY') as data FROM workflow.historicodocumento WHERE docid=doc.docid ORDER BY htddata DESC LIMIT 1) END as data_cadastro,
													  doc.docdsc as descricao_documento,
													  esd.esddsc as descricao_estado,
													  doc.docid  as documento,
													  esd.esdid as estado
											   FROM workflow.documento doc 
											   INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid 
											   WHERE doc.docid = '".$docid."' 
											   ORDER BY data_cadastro");
			
			
		}
		
	}
	
	$arrHistoricos = $db->carregar("SELECT
										doc.docdsc,
										ed.esddsc as origem,
										ed2.esddsc as destino,
										ac.aeddscrealizada,
										us.usunome,
										to_char(hd.htddata, 'dd/mm/YYYY HH24:MI') as htddata,
										cd.cmddsc
									FROM workflow.historicodocumento hd
									INNER JOIN workflow.documento doc ON hd.docid = doc.docid 
									INNER JOIN workflow.acaoestadodoc ac ON	ac.aedid = hd.aedid
									INNER JOIN workflow.estadodocumento ed ON ed.esdid = ac.esdidorigem
									INNER JOIN workflow.estadodocumento ed2 ON ed2.esdid = ac.esdiddestino
									INNER JOIN seguranca.usuario us ON us.usucpf = hd.usucpf
									LEFT JOIN workflow.comentariodocumento cd ON cd.hstid = hd.hstid
									WHERE hd.usucpf='".$_SESSION['usucpf']."' AND doc.tpdid='".$tpdid."'
									ORDER BY hd.htddata DESC");
	
	
	
?>
<script>
	
function abrirDocumento(docid, obj) {

	var linha  = obj.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode;
	
	for(var i=0;i<tabela.rows.length;i++) {
		tabela.rows[i].style.backgroundColor='';
	}
	
	linha.style.backgroundColor='#ffffcc';
		
	<? if($tipodocumento['tpdendereco']) { ?>
	document.getElementById('detalhes_documento').innerHTML="Carregando...";
	
	$.ajax({
   		type: "POST",
   		url: "<? echo $tipodocumento['tpdendereco']; ?>",
   		data: "docid="+docid,
   		async: false,
   		success: function(msg){document.getElementById('detalhes_documento').innerHTML=msg;}
 		});
 	<? } else { ?>
 	
	$.ajax({
   		type: "POST",
   		url: "../geral/workflow/workflow_gerenciamento.php",
   		data: "docid="+docid,
   		async: false,
   		success: function(msg){document.getElementById('detalhes_documento').innerHTML=msg;}
 		});
 		
 	
 	<? } ?>
}

</script>

<table width="100%">
<tr>
<td colspan="2"><h1><? echo $tipodocumento['tpddsc']; ?></h1></td>
</tr>
<tr>
	<td valign="top" width="15%">
	<h2>Caixa de Entrada</h2>
	<ul>
	  <li><a style="cursor:pointer;" href="<? echo $_SERVER['REQUEST_URI']; ?>&cxentrada=pendencias"><? echo (($cxentrada == 'pendencias')?'<b>Pendências</b>':'Pendências'); ?> (<? echo count($docids['pendencias']); ?>)</a></li>
	  <li><a style="cursor:pointer;" href="<? echo $_SERVER['REQUEST_URI']; ?>&cxentrada=resolvidas"><? echo (($cxentrada == 'resolvidas')?'<b>Resolvidas</b>':'Resolvidas'); ?> (<? echo (($arrHistoricos[0])?count($arrHistoricos):"0"); ?>)</a></li>
	  <li><a style="cursor:pointer;" href="<? echo $_SERVER['REQUEST_URI']; ?>&cxentrada=futuras"><? echo (($cxentrada == 'futuras')?'<b>Futuras</b>':'Futuras'); ?> (<? echo count($docids['futuras']); ?>)</a></li>
	</ul>	
	</td>
	<td valign="top">
	
	<? if($cxentrada == 'pendencias') : ?>
	
	<h2>Lista de Pendências</h2>
	<table class="listagem" width="100%" cellSpacing="1" cellPadding="3">
	<thead>
	<tr>
	<td align="center"><b>Data</b></td>
	<td align="center"><b>Descrição</b></td>
	<td align="center"><b>Situação Atual</b></td>
	<td align="center"><b>Ação</b></td>
	<? echo ((count($arrDocumentos) > 15)?"<td style=width:12px;>&nbsp;</td>":""); ?>
	</tr>
	</thead>
	<tbody style="<? echo ((count($arrDocumentos) > 15)?"height:250px;overflow-y:scroll;overflow-x:hidden;":""); ?>">
	<?
	if($arrDocumentos) {
		foreach($arrDocumentos as $documento) {
			echo "<tr>";
			echo "<td>".$documento['data_cadastro']."</td>";
			echo "<td style=cursor:pointer; onclick=abrirDocumento('".$documento['documento']."',this)>".$documento['descricao_documento']."</td>";
			echo "<td>".$documento['descricao_estado']."</td>";
			echo "<td align=center><img align=absmiddle src=../../imagens/valida2.gif border=0 style=cursor:pointer; onclick=abrirDocumento('".$documento['documento']."',this)> <img align=absmiddle style=cursor:pointer; src=../imagens/fluxodoc.gif onclick=\"window.open('../geral/workflow/historico.php?modulo=principal/tramitacao&acao=C&docid=".$documento['documento']."', 'alterarEstado','width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no');\" /></td>";
			echo "</tr>";
			
		}
	} else {
		echo "<tr>";
		echo "<td colspan=3>Não existem pendências.</td>";
		echo "</tr>";
	}
	?>
	</tbody>
	</table>
	
	<h2>Detalhe da Pendência</h2>

	<table class="listagem" width="100%">
	<tr>
	<td><div id="detalhes_documento"><b>Clique na ação para detalhar</b></div></td>
	</tr>
	</table>
	
	<? elseif($cxentrada == 'resolvidas') : ?>
	
	<h2>Lista de Resolvidas</h2>
	<table class="listagem" width="100%" cellSpacing="1" cellPadding="3">
	<thead>
	<tr>
	<td align="center"><b>Data/Hora</b></td>
	<td align="center"><b>Descrição</b></td>
	<td align="center"><b>Estado inicial</b></td>
	<td align="center"><b>Açao realizada</b></td>
	<td align="center"><b>Estado final</b></td>
	<? echo ((count($arrHistoricos) > 15)?"<td style=width:12px;>&nbsp;</td>":""); ?>
	</tr>
	</thead>
	<tbody style="<? echo ((count($arrHistoricos) > 15)?"height:250px;overflow-y:scroll;overflow-x:hidden;":""); ?>">
	<?
	if($arrHistoricos) {
		foreach($arrHistoricos as $historico) {
			echo "<tr>";
			echo "<td>".$historico['htddata']."</td>";
			echo "<td>".$historico['docdsc']."</td>";
			echo "<td>".$historico['origem']."</td>";
			echo "<td>".$historico['aeddscrealizada']."</td>";
			echo "<td>".$historico['destino']."</td>";
			echo "</tr>";
		}
	} else {
		echo "<tr>";
		echo "<td colspan=3>Não existem pendências resolvidas.</td>";
		echo "</tr>";
	}
	?>
	</tbody>
	</table>
	
	<? elseif($cxentrada == 'futuras') : ?>
	
	<h2>Lista de Futuras</h2>
	<table class="listagem" width="100%" cellSpacing="1" cellPadding="3">
	<thead>
	<tr>
	<td align="center"><b>Data/Hora</b></td>
	<td align="center"><b>Descrição</b></td>
	<? echo (($parametros['capturar_responsavel'])?"<td align=\"center\"><b>Responsável</b></td>":""); ?>
	<td align="center"><b>Estado atual</b></td>
	<td align="center"><b>Possíveis ações realizadas</b></td>
	<td align="center"><b>Possíveis estados finais</b></td>
	<? echo ((count($arrDocumentosFut) > 15)?"<td style=width:12px;>&nbsp;</td>":""); ?>
	</tr>
	</thead>
	<tbody style="<? echo ((count($arrDocumentosFut) > 15)?"height:250px;overflow-y:scroll;overflow-x:hidden;":""); ?>">
	<?
	if($arrDocumentosFut) {
		foreach($arrDocumentosFut as $futuras) {
			echo "<tr>";
			echo "<td>".$futuras['data_cadastro']."</td>";
			echo "<td>".$futuras['descricao_documento']."</td>";
			if($parametros['capturar_responsavel']) echo "<td>".$parametros['capturar_responsavel']($futuras['documento'])."</td>";
			echo "<td>".$futuras['descricao_estado']."</td>";
			$possiveisAcoes = $db->carregar("SELECT aed.aeddscrealizar, esd.esddsc FROM workflow.acaoestadodoc aed
											 INNER JOIN workflow.estadodocumento esd ON esd.esdid = aed.esdiddestino  
											 WHERE aed.esdidorigem='".$futuras['estado']."'");
			
			unset($possRealizadas,$possEstFinais);
			if($possiveisAcoes[0]) {
				foreach($possiveisAcoes as $key => $acoes) {
					$possRealizadas[] = ($key+1).". ".$acoes['aeddscrealizar'];
					$possEstFinais[]  = ($key+1).". ".$acoes['esddsc'];
				}
			}
			echo "<td>".(($possRealizadas)?implode("<br/>",$possRealizadas):"&nbsp;")."</td>";
			echo "<td>".(($possEstFinais)?implode("<br/>",$possEstFinais):"&nbsp;")."</td>";
			echo "</tr>";
		}
	} else {
		echo "<tr>";
		echo "<td colspan=3>Não existem pendências a serem repassadas.</td>";
		echo "</tr>";
	}
	?>
	</tbody>
	</table>

	<? endif; ?>
	</td>
</tr>
</table>
<?
}
?>