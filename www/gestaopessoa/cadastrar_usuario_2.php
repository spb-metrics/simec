<?php
	session_start();

	/**
	 * Sistema Integrado de Monitoramento do Ministério da Educação
	 * Setor responsvel: SPO/MEC
	 * Desenvolvedor: Desenvolvedores Simec
	 * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>, Alexandre Soares Diniz
	 * Programadores: Renê de Lima Barbosa <renedelima@gmail.com>, Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
	 * Módulo: Segurança
	 * Finalidade: Solicitação de cadastro de contas de usuário.
	 * Data de criação:
	 * Última modificação: 31/08/2006
	 */

	// força o uso da base espelho produção
	// $_REQUEST['baselogin'] = 'simec_espelho_producao';

	// carrega as bibliotecas internas do sistema
	include "config.inc";
	require APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";

	// abre conexão com o servidor de banco de dados
	$db = new cls_banco();

	$_SESSION['mnuid'] = 10;
	$_SESSION['sisid'] = 4;

	// captura os dados informados no primeiro passo
	$sisid = 64;
	$usucpf = $_REQUEST['usucpf'];
	
	define("PERFIL_SERVIDOR", 400);
	define("PERFIL_CONSULTOR", 401);
	define("PERFIL_TERCEIRIZADO", 402);

	// atribui o ano atual para o exercício das tarefas
	// ultima modificação: 05/01/2007
	$_SESSION['exercicio_atual'] = $db->pega_ano_atual();
	$_SESSION['exercicio'] = $db->pega_ano_atual();

	// captura os dados do formulário
	$usunome    = $_REQUEST['usunome'];
	$usuemail   = $_REQUEST['usuemail'];
	$usuemail_c = $_REQUEST['usuemail_c'];
	$usufoneddd = $_REQUEST['usufoneddd'];
	$usufonenum = $_REQUEST['usufonenum'];
	
	$entid 		= "389372";
	
	//$orgcod     = $_REQUEST['orgcod'];
	$orgcod 	= "26000";
	$usufuncao  = $_REQUEST['usufuncao'];
	//$unicod     = $_REQUEST['unicod'];
	$unicod     = "26101";
	$ungcod     = $_REQUEST['ungcod'];
	//$regcod     = $_REQUEST['regcod'];
	$regcod     = "DF";
	$ususexo    = $_REQUEST['ususexo'];
	$htudsc     = $_REQUEST['htudsc'];
	$pflcod     = $_REQUEST['pflcod'];
	//$pflcod     = 304; //perfil avaliacao 
	//$orgao      = $_REQUEST['orgao'];
	$orgao      = "26000";
	//$muncod     = $_REQUEST['muncod'];
	$muncod     = "5300108";
	//$tpocod     = $_REQUEST['tpocod'];
	$tpocod     = "1"; 
	// prepara o cpf para ser usado nos comandos sql
	$cpf = corrige_cpf( $usucpf );
	
	/*
	$mat_siape = $db->pegaUm("SELECT nu_matricula_siape FROM siape.tb_siape_cadastro_servidor WHERE nu_cpf='".$cpf."'");
	if($mat_siape) {
		$sql_combo_perfil = "select pflcod as codigo, pfldsc as descricao from seguranca.perfil where sisid = 64 and pflcod in ( ".PERFIL_SERVIDOR.")";
	} else {
		$sql_combo_perfil = "select pflcod as codigo, pfldsc as descricao from seguranca.perfil where sisid = 64 and pflcod in ( ".PERFIL_CONSULTOR.",".PERFIL_TERCEIRIZADO.")";
	}
	*/
	$sql_combo_perfil = "select pflcod as codigo, pfldsc as descricao from seguranca.perfil where sisid = 64 and pflcod in ( ".PERFIL_SERVIDOR.",".PERFIL_TERCEIRIZADO.")";
	

	// verifica se o cpf já está cadastrado no sistema
	$sql = sprintf(
		"SELECT
			u.ususexo,
			o.orgdsc,
			o.orgsigla,
			u.usucpf,
			u.regcod,
			u.usunome,
			u.usuemail,
			u.usustatus,
			u.usufoneddd,
			u.usufonenum,
			u.ususenha,
			u.usudataultacesso,
			u.usunivel,
			u.usufuncao,
			u.ususexo,
			u.orgcod,
			u.unicod,
			u.usuchaveativacao,
			u.usutentativas,
			u.usuobs,
			u.ungcod,
			u.usudatainc,
			u.usuconectado,
			u.suscod,
			u.muncod
		FROM seguranca.usuario u
		LEFT JOIN public.orgao o ON u.orgcod = o.orgcod
		WHERE
			u.usucpf = '%s'  ",
		$cpf
	);
	$usuario = (object) $db->pegaLinha( $sql );
	if ( $usuario->usucpf ) {
		foreach ( $usuario as $atributo => $valor ) {
			$$atributo = $valor;
		}
		$usucpf = formatar_cpf( $usuario->usucpf );
		$cpf_cadastrado = true;
		$editavel = 'N';
	} else {
		$cpf_cadastrado = false;
		$editavel = 'S';
	}

	// verifica se o usuário já está cadastrado no módulo selecionado
	$sql = sprintf(
		"SELECT usucpf, sisid, suscod FROM usuario_sistema WHERE usucpf = '%s' AND sisid = %d",
		$cpf,
		$sisid
	);
	$usuario_sistema = (object) $db->pegaLinha( $sql );
	if ( $usuario_sistema->sisid ) {
		if ( $usuario_sistema->suscod == 'B' ) {
			$_SESSION['MSG_AVISO'] = array( "Sua conta está bloqueada neste sistema. Para solicitar a ativação da sua conta justifique o pedido no formulário abaixo." );
			header( "Location: ../solicitar_ativacao_de_conta.php?sisid=$sisid&usucpf=$usucpf" );
			exit();
		}
		$_SESSION['MSG_AVISO'] = array( "Atenção. CPF já cadastrado no módulo solicitado." );
		header( "Location: cadastrar_usuario.php?sisid=$sisid&usucpf=$usucpf" );
		exit();
	}
	$cpf_cadastrado_sistema = (boolean) $db->pegaUm( $sql );

	$sql = sprintf( "select sisid, sisdsc, sisfinalidade, sispublico, sisrelacionado, sisdiretorio from sistema where sisid = %d", $sisid );
	$sistema = (object) $db->pegaLinha( $sql );

// efetiva cadastro se o formulário for submetido 
if ( $_POST['formulario'] ) { 
		// atribuições requeridas para que a auditoria do sistema funcione
		$_SESSION['sisid'] = 4; # seleciona o sistema de segurança
		$_SESSION['usucpf'] = $cpf;
		$_SESSION['usucpforigem'] = $cpf;
		
		$tpocod_banco = $tpocod ? (integer) $tpocod : "null"; 
		
		if ( !$cpf_cadastrado ) {
					
			$sql = sprintf(
				"INSERT INTO seguranca.usuario (
					usucpf, usunome, usuemail, usufoneddd, usufonenum,
					usufuncao, orgcod, unicod, usuchaveativacao, regcod,
					ususexo, ungcod, ususenha, suscod, orgao,
					muncod, tpocod
				) values (
					'%s', '%s', '%s', '%s', '%s',
					'%s', '%s', '%s', '%s', '%s',
					'%s', '%s', '%s', '%s', '%s',
					'%s',  %s
				)",
				$cpf,
				str_to_upper( $usunome ),
				strtolower( $usuemail ),
				$usufoneddd,
				$usufonenum,
				$usufuncao,
				$orgcod,
				$unicod,
				'f',
				$regcod,
				$ususexo,
				$ungcod,
				md5_encrypt_senha( strtoupper($db->gerar_senha()), '' ),
				'A',
				$orgao,
				$muncod,
				$tpocod_banco
			);
		
			$db->executar( $sql );
		}
		
		// vincula o usuário com o módulo
    	$sql = sprintf(
    		"INSERT INTO seguranca.usuario_sistema ( usucpf, sisid, pflcod ) values ( '%s', %d, %d )",
	    	$cpf,
	    	$sisid,
	    	$pflcod
    	);
    	$db->executar( $sql );
		
		// modifica o status do usuário (no módulo) para pendente
		$descricao = "Usuário solicitou cadastro e apresentou as seguintes observações: ". $htudsc;
		$db->alterar_status_usuario( $cpf, 'P', $descricao, $sisid );
		
		// executa rotina específica do módulo
		//$arquivo = APPRAIZ . $sistema->sisdiretorio ."/modulos/sistema/usuario/incusuariosql.inc";
		//if ( file_exists( $arquivo ) ) {
		//	include $arquivo;
		//}
		
		//----------- verificando se o sistema deve inserir dados de proposta e os insere caso necessario
				
		$sql = "SELECT
							 s.sisid, lower(s.sisdiretorio) as sisdiretorio
							FROM
							 seguranca.sistema s 
							WHERE
							 sisid = ".$sisid."";
		
		
		$sistema = (object) $db->pegaLinha( $sql );
		
		$sistema->sisdiretorio = $sistema->sisid == 14 ? 'cte' : $sistema->sisdiretorio;
		
		$sql = sprintf("SELECT CASE
							        WHEN
								 (SELECT
								   true 
								  FROM
								   pg_tables 
								  WHERE
								   schemaname='%s' AND 
								   tablename = 'tiporesponsabilidade') 
								THEN true
								WHEN    
								  (SELECT
								   true 
								  FROM
								   pg_tables 
								  WHERE
								   schemaname='%s' AND 
								   tablename = 'tprperfil')
								THEN true
								ELSE false
								END;",
						$sistema->sisdiretorio,
						$sistema->sisdiretorio);
		//echo "<pre>".$sql;
							
		$existeTabela = $db->pegaUm($sql);
		
		if($existeTabela == 't'){
			
		    // exclui todas os programas e ações e etc propostos anteriormente
			//$db->executar( "DELETE FROM ".$sistema->sisdiretorio.".usuariorespproposta WHERE usucpf = ".$cpf." and pflcod = ".$pflcod."" );
		
			$propostos = (array) $_REQUEST["proposto"];					
		
			foreach ( $propostos as  $chave => $valores) {
				
				$sql_tpr = "select tprcampo from ".$sistema->sisdiretorio.".tiporesponsabilidade where tprsigla = '".$chave."'";
					
				$tprcampo = $db->pegaUm($sql_tpr);				
				foreach ( $valores as $chave => $valor) {
					$sql_proposta = "insert into 
						seguranca.usuariorespproposta 
						( urpcampoid, urpcampo, pflcod, usucpf, sisid )
					 	values 
					 	( '".$valor."', '".$tprcampo."', '".$pflcod."', '".$cpf."', ".$sisid." )";
					
					$db->executar( $sql_proposta, false );
				}				
			}			
		}
			
		//---------------------------- fim da verificação e inserção

	// obtém dados da instituição
	$sql = "select ittcod, ittemail_inclusao_usuario, ittemail, itttelefone1, itttelefone2, ittddd, ittfax, ittsistemasigla from public.instituicao where ittstatus = 'A'";
	$instituicao = (object) $db->pegaLinha( $sql );

	if ( $instituicao->ittcod ) {
		// captura email de cópia quando for módulo de monitoramento
		if ( $sisid == 1 )
		{
			$sqlPegaEmailSistema = "
					select	
						sisemail
					from seguranca.sistema
					where sisid = " . ( (integer) $sisid );
			$emailCopia = trim( $db->pegaUm( $sqlPegaEmailSistema ) );
		}
		else
		{
			$emailCopia = "";
		}

		$sql     = "select sistel, sisfax from seguranca.sistema s where s.sisstatus='A' and sismostra='t' AND sisid = $sisid" ;
		$sistema = (object) $db->pegaLinha( $sql );

		// envia email de confirmação
		$remetente = array("nome" => $instituicao->ittsistemasigla,"email" => $instituicao->ittemail);
		$destinatario = $usuemail;
		$assunto = "Solicitação de Cadastro no Simec";
		$conteudo = sprintf(
				"%s<p>%s %s ou no(s) telefone(s): %s Fax %s</p>%s",
		$ususexo == 'M' ? 'Prezado Sr.' : 'Prezada Sra.',
		$instituicao->ittemail_inclusao_usuario,
		$instituicao->ittemail,
		$sistema->sistel,
		$sistema->sisfax,
		$cpf_cadastrado ? '*Usuário já cadastrado' : '*Novo Usuário'
		);
			
		enviar_email( $remetente, $destinatario, $assunto, $conteudo, $emailCopia );
	}

	// leva o usuário para a página de login e exibe confirmação
	$db->commit();
	$sisabrev = $db->pegaUm( "SELECT sisabrev FROM seguranca.sistema WHERE sisid = ". $sisid );
	$mensagem = sprintf(
			"Sua solicitação de cadastro para acesso ao módulo %s foi registrada e será analisada pelo setor responsável. Em breve você receberá maiores informações.",
	$sisabrev
	);
	
		//**************ATIVA OS USUÁRIOS DO SISTEMA DEMANDAS********************
	
		// carrega os dados da conta do usuário
		$sql = sprintf("SELECT 
							usucpf, usuemail, ususexo, usunome, ususenha
						FROM 
							seguranca.usuario
						WHERE 
							usucpf = '%s'",
		$cpf
		);
		$usuariod = (object) $db->pegaLinha( $sql );

		$justificativa = "Ativação automática de usuário no sistema de Gestão de Pessoas";
		$suscod = "A";
		$db->alterar_status_usuario( $usuariod->usucpf, $suscod, $justificativa, $sisid );
		//$email_aprovacao = $usuariosistema->suscod == 'P' && $suscod == 'A' ? true : $email_aprovacao;

		// envia o email de confirmação caso a conta seja aprovada
		//if ( $email_aprovacao) {
		$remetente = array("nome" => "SIMEC","email" => $usuariod->usuemail);
		$destinatario = $usuariod->usuemail;
		$assunto = "Aprovação do Cadastro no Simec - Módulo Gestão de Pessoas";
		$conteudo = "
			<br/>
			<span style='background-color: red;'><b>Esta é uma mensagem gerada automaticamente pelo sistema. </b></span>
			<br/>
			<span style='background-color: red;'><b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b></span>
			<br/>
			";
		$conteudo .= sprintf(
		"%s %s<p>Sua conta está ativa. Sua Senha de acesso é: %s</p>",
		$usuariod->ususexo == 'M' ? 'Prezado Sr.' : 'Prezada Sra.',
		$usuariod->usunome,
		md5_decrypt_senha( $usuariod->ususenha, '' )
		);
		
		$conteudo .= "<br><br>* Caso você já alterou a senha acima, favor desconsiderar este e-mail.";
		enviar_email( $remetente, $destinatario, $assunto, $conteudo );
		//}
		
		
		// cadastra o perfil  
		//deleta os perfis
		$sql = sprintf(
		"DELETE FROM seguranca.perfilusuario WHERE usucpf = '%s' AND pflcod IN ( SELECT p.pflcod FROM seguranca.perfil p WHERE p.sisid = %d )",
		$usuariod->usucpf,
		$sisid
		);
		$db->executar( $sql );

						 
		// inclui os perfis 
		$sql = sprintf(
		"INSERT INTO seguranca.perfilusuario ( usucpf, pflcod ) VALUES ( '%s', %d )",
		$usuariod->usucpf,
		$pflcod
		);
		$db->executar( $sql );

		$db->commit();
		
		$_SESSION['senhademandas'] = md5_decrypt_senha( $usuariod->ususenha, '' );
		$usucpf = formatar_cpf($usuariod->usucpf);
		$ususenha = $_SESSION['senhademandas'];
		include "autenticar.inc";
		
		exit();
}


?>
<html>
	<head>
		<title>Simec - Ministério da Educação</title>
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<style type=text/css>
			form {
				margin: 0px;
			}
		</style>
	</head>
	<body bgcolor=#ffffff vlink=#666666 bottommargin="0" topmargin="0" marginheight="0" marginwidth="0" rightmargin="0" leftmargin="0">
		<?php include "cabecalho.php"; ?>
		<br/>
		<?php
			$titulo_modulo = 'Ficha de Solicitação de Cadastro de Usuários';
			$subtitulo_modulo = 'Preencha os Dados Abaixo e clique no botão: "Enviar Solicitação".<br>'. obrigatorio() .' Indica Campo Obrigatório.';
			monta_titulo( $titulo_modulo, $subtitulo_modulo );
		?>
		<form method="POST" name="formulario">
			<input type=hidden name="formulario" value="1"/>
			<table width='95%' align='center' border="0" cellspacing="1" cellpadding="3" style="border: 1px Solid Silver; background-color:#f5f5f5;">
				<tr>
					<td align='right' class="subtitulodireita" width="150px">Módulo:</td>
					<td>
						<?php
							$sql = "select s.sisid as codigo, s.sisabrev as descricao from seguranca.sistema s where s.sisstatus='A' and sismostra='t'";
							$db->monta_combo( "sisid", $sql, 'N', "Selecione o sistema desejado", '', '' );
						?>
					</td>
				</tr>
				<?php if( $sistema->sisid ): ?>
					<tr>
						<td align='right' class="subtitulodireita">&nbsp;</td>
						<td>
							<font color="#555555" face="Verdana">
								<b><?= $sistema->sisdsc ?></b><br/>
								<p><?= $sistema->sisfinalidade ?></p>
								<ul>
									<li><span style="color: #000000">Público-Alvo:</span> <?= $sistema->sispublico ?><br></li>
									<li><span style="color: #000000">Sistemas Relacionados:</span> <?= $sistema->sisrelacionado ?></li>
								</ul>
							</font>
						</td>
					</tr>
					<tr>
						<td align='right' class="subtitulodireita">CPF:</td>
						<td>
							<?= campo_texto( 'usucpf', $obrig, 'N' ,'' , 19, 14, '###.###.###-##', '' ); ?>
							<?= obrigatorio(); ?>
						</td>
					</tr>
				<?php endif; ?>
				<?php if ( $cpf_cadastrado_sistema ): ?>
					<tr bgcolor="#C0C0C0">
						<td>&nbsp;</td>
						<td><input type="Button" value="Voltar" onclick="location.href='./cadastrar_usuario.php?sisid=<?= $sisid ?>&usucpf=<?= $usucpf ?>'"></td>
					</tr>
				<?php else: ?>
					<tr>
						<td align='right' class="subtitulodireita" width="150px">Nome:</td>
						<td>
							<?= campo_texto( 'usunome', $obrig, $editavel, '', 50, 50, '', '' ); ?>
							<?= obrigatorio(); ?>
						</td>
					</tr>
					<tr>
						<td align = 'right' class="subtitulodireita">Sexo:</td>
						<td>
							<input id="sexo_masculino" type="radio" name="ususexo" value="M" <?=($ususexo=='M'?"CHECKED":"")?> <?= $cpf_cadastrado ? 'disabled="disabled"' : '' ?>/>
							<label for="sexo_masculino">Masculino</label>
							<input id="sexo_feminino" type="radio" name="ususexo" value="F" <?=($ususexo=='F'?"CHECKED":"")?> <?= $cpf_cadastrado ? 'disabled="disabled"' : '' ?>/>
							<label for="sexo_feminino">Feminino</label>
							<?= obrigatorio(); ?>
						</td>
					</tr>
					<tr bgcolor="#F2F2F2">
						<td align='right' class="subtitulodireita">Unidade Federal do Órgão:</td>
						<td>
							<?php
								$sql = "SELECT regcod AS codigo, regcod||' - '||descricaouf AS descricao FROM uf WHERE codigoibgeuf IS NOT NULL ORDER BY 2";
								$db->monta_combo("regcod",$sql,$editavel,"&nbsp;",'listar_municipios','');
							?>
							<?= obrigatorio(); ?>
						</td>
					</tr>
					<tr>
					<td align='right' class="subtitulodireita" width="150px">Perfil:</td>
					<td>
						<?php
							$db->monta_combo( "pflcod", $sql_combo_perfil, 'S', "Selecione o perfil desejado", '', '' );
							echo obrigatorio();
						?>
					</td>
				</tr>
					<tr>
						<td align='right' class="SubTituloDireita">Município:</td>
						<td>
							<div id="muncod_on" style="display:none;">
								<select
									id="muncod"
									name="muncod"
									onchange=""
									class="CampoEstilo"
									<?= $editavel == 'S' ? "" : 'disabled="disabled"' ?>
								></select>
								<?= obrigatorio(); ?>
							</div>
							<div id="muncod_off" style="color:#909090;">A Unidade Federal selecionada não possui municípios.</div>
						</td>
					</tr>
					<tr>
						<td align='right' class="subtitulodireita">Órgão:</td>
						<td>
							<?php
								$sql = "SELECT orgcod AS codigo, orgcod||' - '||orgdsc AS descricao FROM public.orgao WHERE organo = '". $_SESSION['exercicio'] ."' order by orgdsc";
								//Comentário 
								$db->monta_combo("orgcod",$sql,$editavel,"&nbsp;",'selecionar_orgao','','','350','S');
							?>
						</td>
					</tr>
					
					<?php if ( $orgcod == '99999' ): ?>
						<tr>
							<td align='right' class="SubTituloDireita">Tipo do Órgão:</td>
							<td>
								<?php
									$sql = "
										select
											tpocod as codigo,
											tpodsc as descricao
										from public.tipoorgaoexterno
										where
											tposts = 'A'
										order by
											tpodsc
									";
									$db->monta_combo( 'tpocod', $sql, 'S', 'Selecione o tipo do seu órgão', '', '' );
									echo obrigatorio();
								?>
							</td>
						</tr>
						<tr>
							<td align='right' class="subtitulodireita">Nome do Órgão:</td>
							<td>
								<?	
								echo campo_texto( 'orgao', '', $editavel, '', 50, 50, '', '' );
								echo obrigatorio();
								?>
							</td>
						</tr>
					<?php endif; ?>
					
					<?php
						$uo_total = $db->pegaUm( "SELECT count(*) FROM unidade WHERE unistatus='A' and unitpocod='U' and orgcod ='$orgcod'" );
					?>
					<?php if ( $uo_total > 0 ): ?>
						<tr bgcolor="#F2F2F2">
							<td align = 'right' class="subtitulodireita">Unidade Orçamentária:</td>
							<td>
								<?php
									$sql = "SELECT unicod AS codigo, unicod||' - '||unidsc AS descricao FROM unidade WHERE unistatus='A' and unitpocod='U' and orgcod ='$orgcod' order by unidsc";
									$db->monta_combo("unicod",$sql,$editavel,"&nbsp;",'selecionar_unidade_orcamentaria','');
									echo obrigatorio();
								?>
							</td>
						</tr>
					<?php endif; ?>
					<?php if ( $unicod == '26101' AND $orgcod == '26000' ): ?>
						<tr bgcolor="#F2F2F2">
							<td align = 'right' class="subtitulodireita">Unidade Gestora:</td>
							<td>
								<?php
									$sql = "SELECT ungcod AS codigo, ungcod||' - '||ungdsc as descricao FROM unidadegestora WHERE ungstatus = 'A' AND unitpocod = 'U' AND unicod = '".$unicod."' ORDER BY ungdsc";
									$db->monta_combo("ungcod",$sql,$editavel,"&nbsp;",'','');
								?>
								<?= obrigatorio(); ?>
							</td>
						</tr>
					<?php endif; ?>
					<tr>
						<td align='right' class="subtitulodireita">(DDD) + Telefone:</td>
						<td>
							<?= campo_texto('usufoneddd','',$editavel,'',3,2,'##',''); ?>
							<?= campo_texto('usufonenum','S',$editavel,'',18,15,'###-####|####-####',''); ?>
						</td>
					</tr>
					<tr >
						<td align='right' class="subtitulodireita">Seu E-Mail:</td>
						<td>
							<?= campo_texto('usuemail','S',$editavel,'',50,100,'',''); ?>
						</td>
					</tr>
					<?php if ( !$cpf_cadastrado ): ?>
						<tr>
							<td align = 'right' class="subtitulodireita">Confirme o Seu E-Mail:</td>
							<td><?= campo_texto('usuemail_c','S','','',50,100,'',''); ?>
							<br/>
							<font color="#202020" face="verdana">Este e-mail é para uso individual, <b>não utilize endereço coletivo</b>.</font>
							</td>
						</tr>
					<?php endif; ?>
					<tr>
						<td align='right' class="subtitulodireita">Função/Cargo:</td>
						<td>
							<?= campo_texto('usufuncao','S',$editavel,'',50,100,'',''); ?>
						</td>
					</tr>
					<tr bgcolor="#F2F2F2">
						<td align = 'right' class="subtitulodireita" colspan="2">&nbsp;</td>
				 	</tr>
					<tr>
						<td align='right' class="subtitulodireita">Observações:</td>
						<td>
							<?= campo_textarea('htudsc','N','S','',100,3,''); ?><br>
						</td>
					</tr>
					<?php
						// inclui campos requeridos pelo módulo no qual o usuário pretende se cadastrar
						$arquivo = APPRAIZ . $sistema->sisdiretorio ."/modulos/sistema/usuario/incusuario.inc";
						if ( file_exists( $arquivo ) ){
							include $arquivo;
						}
					?>
					<tr bgcolor="#C0C0C0">
						<td>&nbsp;</td>
						<td>
							<input type="button" name="btinserir" value="Cadastrar" onclick="enviar_formulario()" />
							&nbsp;&nbsp;&nbsp;
							<input type="Button" value="Voltar" onclick="location.href='./cadastrar_usuario.php?sisid=<?= $sisid ?>&usucpf=<?= $usucpf ?>'"/>
						</td>
					</tr>
				<?php endif; ?>
			</table>
		</form>
		<br/>
		<?php include "./rodape.php"; ?>
	</body>
</html>
<script type="text/javascript">
	
	<?php
		$sql = "SELECT estuf, muncod, estuf || ' - ' || mundescricao as mundsc FROM territorios.municipio ORDER BY 3 ";
	?>
	var lista_mun = new Array();
	<? $ultimo_cod = null; ?>
	<? foreach ( $db->carregar( $sql ) as $unidade ) :?>
		<? if ( $ultimo_cod != $unidade['estuf'] ) : ?>
			lista_mun['<?= $unidade['estuf'] ?>'] = new Array();
			<? $ultimo_cod = $unidade['estuf']; ?>
		<? endif; ?>
		lista_mun['<?= $unidade['estuf'] ?>'].push( new Array( '<?= $unidade['muncod'] ?>', '<?= addslashes( trim( $unidade['mundsc'] ) ) ?>' ) );
	<? endforeach; ?>
	
	
	function listar_municipios( regcod )
	{
		var campo_select = document.getElementById( 'muncod' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );
		
		var div_on = document.getElementById( 'muncod_on' );
		var div_off = document.getElementById( 'muncod_off' );

		if ( !lista_mun[regcod] )
		{
			validar_mun = false;
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			return;
		}
		
		validar_mun = true;
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		var j = lista_mun[regcod].length;
		for ( var i = 0; i < j; i++ )
		{
			campo_select.options[campo_select.options.length] = new Option( lista_mun[regcod][i][1], lista_mun[regcod][i][0], false, lista_mun[regcod][i][0] == '<?= $muncod ?>' );
		}
		for ( i=0; i< campo_select.options.length; i++ )
		{
			if ( campo_select.options[i].value == '<?= $muncod ?>' ) {
				campo_select.options[i].selected = true;
			}
		}
	}

	
	function trim( valor )
	{
		//return valor.replace( /^\s*|\s*$/g, '' );
		return valor.replace( /^\s+|\s+$/g,"" );
	}

	function selecionar_orgao( valor ) {
		document.formulario.formulario.value = "";
		document.formulario.submit();
	}

	function selecionar_unidade_orcamentaria() {
		document.formulario.formulario.value = "";
		document.formulario.submit();
	}
	
	function enviar_formulario() {
		if ( validar_formulario() ) {
			document.formulario.submit();
		}
	}
	

	function validar_formulario() {
		
		//alert('tamanho do nome'+ document.formulario.usunome.value.length);
		
		var validacao = true;
		var mensagem = 'Os seguintes campos não foram preenchidos corretamente:\n';
		if ( document.formulario.sisid.value == '' || !validar_cpf( document.formulario.usucpf.value ) ) {
			// TODO: voltar para o primeiro formulário
		}
		
		<?php if ( !$cpf_cadastrado ): ?>
			document.formulario.usunome.value = trim( document.formulario.usunome.value );
			if ( ( document.formulario.usunome.value == '')  || (document.formulario.usunome.value.length < 5 )) {
				mensagem += '\n\tNome';
				validacao = false;
			}
			if ( !validar_radio( document.formulario.ususexo, 'Sexo' ) ) {
				mensagem += '\n\tSexo';
				validacao = false;
			}
			if ( document.formulario.regcod.value == '' ) {
				mensagem += '\n\tUnidade Federal';
				validacao = false;
			} else if ( document.formulario.muncod.value == '' ) { 
				mensagem += '\n\tMunicípio';
				validacao = false;
			}
			if ( document.formulario.orgcod.value == '' ) {
				mensagem += '\n\tÓrgão';
				validacao = false;
			}
			<?php if ( $uo_total > 0 ): ?>
				if ( document.formulario.unicod.value == '' ) {
					mensagem += '\n\tUnidade Orçamentária';
					validacao = false;
				}
			<?php endif; ?>
			<?php if ( $orgcod == '99999' ): ?>
				if ( document.formulario.tpocod.value == '' ) {
					mensagem += '\n\tTipo do Órgão';
					validacao = false;
				}	
				
				document.formulario.orgao.value = trim( document.formulario.orgao.value );
				if (    document.formulario.orgao.value == '' ||
					document.formulario.orgao.value.length < 5
				    )
				{
					mensagem += '\n\tNome do Órgão';
					validacao = false;
				}
				
			<?php endif; ?>
			if ( document.formulario.orgcod.value == '26000' && document.formulario.unicod.value == '26101' ) {
				if ( document.formulario.ungcod.value == '' ) {
					mensagem += '\n\tUnidade Gestora';
					validacao = false;
				}
			}
			document.formulario.usufoneddd.value = trim( document.formulario.usufoneddd.value );
			document.formulario.usufonenum.value = trim( document.formulario.usufonenum.value );
			if ( 
				document.formulario.usufoneddd.value == '' || 
				document.formulario.usufonenum.value == '' || 
				
				document.formulario.usufoneddd.value.length < 2 ||
				document.formulario.usufonenum.value.length < 7
			   ) 
			{
				mensagem += '\n\tTelefone';
				validacao = false;
			}
			document.formulario.usuemail.value = trim( document.formulario.usuemail.value );
			if ( !validaEmail( document.formulario.usuemail.value ) ) {
				mensagem += '\n\tEmail';
				validacao = false;
			}
			document.formulario.usuemail_c.value = trim( document.formulario.usuemail_c.value );
			if ( !validaEmail( document.formulario.usuemail_c.value ) ) {
				mensagem += '\n\tConfirmação do Email';
				validacao = false;
			}
			if ( validaEmail( document.formulario.usuemail.value ) && validaEmail( document.formulario.usuemail_c.value ) && document.formulario.usuemail.value != document.formulario.usuemail_c.value ) {
				mensagem += '\n\tOs campos Email e Confirmação do Email não coincidem.';
				validacao = false;
			}
			document.formulario.usufuncao.value = trim( document.formulario.usufuncao.value );
			if ( 
				document.formulario.usufuncao.value == '' ||
				document.formulario.usufuncao.value.length < 5 
			    )
			{
				mensagem += '\n\tFunção';
				validacao = false;
			}
		<?php endif; ?>
		
		if ( document.formulario.pflcod )
		{
			if ( document.formulario.pflcod.value == '' ) {
				mensagem += '\n\tPerfil';
				validacao = false;
			}
			
			// seleciona todos as ações
			var acoes = document.getElementById( "proposto_A" );
			if ( acoes ) {
				if ( acoes.options.length == 1 && acoes.options[0].value == '' ) {
					mensagem += '\n\tAções';
					validacao = false;
				} else {
					for ( var i=0; i < acoes.options.length; i++ ) {
						acoes.options[i].selected = true;
					}
				}
			}
			
			// seleciona todos os programas
			var programas = document.getElementById( "proposto_P" );
			if ( programas ) {
				if ( programas.options.length == 1 && programas.options[0].value == '' ) {
					mensagem += '\n\tProgramas';
					validacao = false;
				} else {
					for ( var i=0; i < programas.options.length; i++ ) {
						programas.options[i].selected = true;
					}
				}
			}
			
			// seleciona todas as unidades
			var unidades = document.getElementById( "proposto_U" );
			if ( unidades ) {
				if ( unidades.options.length == 0 && unidades.options[0].value == '' ) {
					mensagem += '\n\tUnidades';
					validacao = false;
				} else {
					for ( var i=0; i < unidades.options.length; i++ ) {
						unidades.options[i].selected = true;
					}
				}
			}
		}
		
		if ( !validacao ) {
			alert( mensagem );
		}
		return validacao;		
	}
	
	<?php if ( $regcod ): ?>
		listar_municipios( '<?= $regcod ?>' );
	<?php endif; ?>
	
</script>