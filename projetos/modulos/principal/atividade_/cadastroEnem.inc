<?

function excluirObservacao(){
	global $db;
	
	$sql = "update 
				projetos.enemobsinstituicao 
			set
				obsstatus = 'I'
			where
				eoiid = '{$_REQUEST['eoiid']}'";
				
	$db->executar($sql);
	$db->commit($sql);
	
	$sqlObs = "select
					(CASE (select count(eoiid) from  projetos.enemobsinstituicao obs1 where obs1.eniid = {$_REQUEST['eniid']} and obs1.eoidata > obs.eoidata and obsstatus = 'A')
					WHEN 0 THEN 
					(CASE obs.usucpf
						WHEN '{$_SESSION['usucpf']}' THEN '<center><img style=\"cursor:pointer\" src=\"/imagens/alterar.gif\" onclick=\"editarObservacao(\'{$_REQUEST['eniid']}\',\'' || obs.eoiid || '\')\" title=\"Alterar\" /> <img style=\"cursor:pointer\" src=\"/imagens/excluir.gif\" onclick=\"excluirObservacao(\'{$_REQUEST['eniid']}\',\'' || obs.eoiid || '\')\" title=\"Excluir\" /></center>' 
						ELSE '<center><img style=\"cursor:pointer\" src=\"/imagens/alterar.gif\" onclick=\"editarObservacao(\'{$_REQUEST['eniid']}\',\'' || obs.eoiid || '\')\" title=\"Alterar\" /> <img style=\"cursor:pointer\" src=\"/imagens/excluir_01.gif\" onclick=\"alert(\'Operação não permitida!\')\" title=\"Excluir\" /></center>'
				 	END)
					ELSE '<center><img style=\"cursor:pointer\" src=\"/imagens/alterar.gif\" onclick=\"editarObservacao(\'{$_REQUEST['eniid']}\',\'' || obs.eoiid || '\')\" title=\"Alterar\" /> <img style=\"cursor:pointer\" onclick=\"alert(\'Operação não permitida!\')\" src=\"/imagens/excluir_01.gif\" title=\"Excluir\" /></center>'
			 	END)as acao,
					usu.usunome,
					to_char(eoidata,'DD-MM-YYYY / HH24:MI:SS') as eoidata,
					eoidescricao
				from 
					projetos.enemobsinstituicao obs
				inner join
					seguranca.usuario usu ON usu.usucpf = obs.usucpf
				where 
					eniid = {$_REQUEST['eniid']}
				and
					obsstatus = 'A'
				order by
					eoidata";
		
		$cabecalho = array("&nbsp;Ações&nbsp;&nbsp;&nbsp;&nbsp;", "Usuário", "Data / Hora","Comentário");
	
		$db->monta_lista($sqlObs,$cabecalho,100,5,'N','center',$par2);
		
		exit;
	
}

function carregaMunicipio(){
	global $db;
	$sql = "select 
				mun.muncod as codigo,
				mun.mundescricao || ' (' || projetos.enmqtdaluno || ')' as descricao 
			from 
				projetos.enemmunicipio pde
			inner join
				territorios.municipio mun ON mun.muncod = projetos.muncod
			where
				projetos.estuf = '{$_REQUEST['estuf']}'
			group by 
				mun.muncod,
				mun.mundescricao,
				projetos.enmqtdaluno
			order by 
				mun.mundescricao";
	$db->monta_combo('enimunicipio',$sql,'S','Selecione...','','','','200','N',"enimunicipio","",$_REQUEST['enimunicipio']);
	exit;
}

function salvarInstituicao(){
	global $db;
	extract($_POST);
	
	if($eniid && $eniid != ""){
		
		$sql = "update 
					projetos.eneminstituicao 
				set
					enisituacao = '$enisituacao',
					eniresponsavel = '$eniresponsavel'
				where
					eniid = $eniid";
		$db->executar($sql);
		$db->commit($sql);
		
	}else{
		
		$sql = "insert into projetos.eneminstituicao 
					(eninome,eniuf,enimunicipio,enicontato,enisituacao,enirestricao,eniresponsavel,eniresopcao,usucpf)
				values
					('$eninome','$eniuf','$enimunicipio','$enicontato','$enisituacao','$enirestricao','$eniresponsavel','$eniresopcao','".$_SESSION['usucpf']."')
				returning eniid";
		$eniid = $db->pegaUm($sql);
		$db->commit($sql);
		
		echo "<script>alert('Operação Realizada com Sucesso!');window.location.href='projetos.php?modulo=principal/atividade_/listaEnem&acao=A';</script>";
	}
	
	$_REQUEST['eniid'] = $eniid;	
}

if(isset($_REQUEST['evento']) && $_REQUEST['evento'] != ""){
	header('content-type: text/html; charset=ISO-8859-1');
	$_REQUEST['evento']();
}

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// VERIFICA DE USUÁRIO POSSUI PERFIL PARA ALOCAÇÃO DE DALAS
$usuario_alocacao_salas = arrayPerfil();

if(in_array(PERFIL_ALOCACAO_SALAS,$usuario_alocacao_salas) && count($usuario_alocacao_salas) == 1){
	$menu = array(0 => array("id" => 3, "descricao" => "Lista de Instituições", "link" => "/projetos/projetos.php?modulo=principal/atividade_/listaEnem&acao=A"),
		  1 => array("id" => 4, "descricao" => "Cadastro de Restrições", "link" => "/projetos/projetos.php?modulo=principal/atividade_/cadastroEnem&acao=A"));
}elseif(in_array(PERFIL_ALOCACAO_SALAS,$usuario_alocacao_salas) && count($usuario_alocacao_salas) > 1){
	$menu = array(0 => array("id" => 3, "descricao" => "Lista de Instituições", "link" => "/projetos/projetos.php?modulo=principal/atividade_/listaEnem&acao=A"),
		  1 => array("id" => 4, "descricao" => "Cadastro de Restrições", "link" => "/projetos/projetos.php?modulo=principal/atividade_/cadastroEnem&acao=A"));
}else{
	$menu = array(0 => array("id" => 1, "descricao" => "Todas Atividades",   "link" => "/projetos/projetos.php?modulo=principal/atividade_/arvore&acao=A"),
		  1 => array("id" => 2, "descricao" => "Minhas Atividades",	 "link" => "/projetos/projetos.php?modulo=principal/atividade_/arvore&acao=R"),
		  2 => array("id" => 3, "descricao" => "Lista de Instituições", "link" => "/projetos/projetos.php?modulo=principal/atividade_/listaEnem&acao=A"),
		  3 => array("id" => 4, "descricao" => "Cadastro de Restrições", "link" => "/projetos/projetos.php?modulo=principal/atividade_/cadastroEnem&acao=A"));	
}

if($_GET['eniid']){
	$menu = array(0 => array("id" => 1, "descricao" => "Todas Atividades",   "link" => "/projetos/projetos.php?modulo=principal/atividade_/arvore&acao=A"),
		  1 => array("id" => 2, "descricao" => "Minhas Atividades",	 "link" => "/projetos/projetos.php?modulo=principal/atividade_/arvore&acao=R"),
		  2 => array("id" => 3, "descricao" => "Lista de Instituições", "link" => "/projetos/projetos.php?modulo=principal/atividade_/listaEnem&acao=A"),
		  3 => array("id" => 4, "descricao" => "Cadastro de Restrições", "link" => "/projetos/projetos.php?modulo=principal/atividade_/cadastroEnem&acao=A&eniid={$_GET['eniid']}"));
}
  
echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);

// titulos da tela
$titulo = "Cadastro de Restrição";
monta_titulo( $titulo, '&nbsp;' );

if(isset($_REQUEST['eniid']) && $_REQUEST['eniid'] != ""){
	$sql = "select
				int.eniid,
				int.eninome,
				int.eniuf,
				int.enimunicipio,
				int.enicontato,
				int.enirestricao,
				int.enisituacao,
				int.eniresponsavel,
				int.eniresopcao
			from
				projetos.eneminstituicao int
			where
				int.eniid = {$_REQUEST['eniid']}";
	$dados = $db->pegaLinha($sql);
	
}
?>
<script language="javascript" type="text/javascript" src="/includes/prototype.js"></script>
<form method="POST"  id="formulario" name="formulario">
	<input type="hidden" id="evento" name="evento" value="" />
	<input type="hidden" id="eniid" name="eniid" value="<?php echo $dados['eniid'] ?>" />
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td align='right' width=25% class="SubTituloDireita">Nome da Instituição:</td>
			<td><?php
					$permissao = $dados['eninome'] ? "N" : "S";
					echo campo_texto('eninome','S',$permissao,'',60,60,'','','','','',"id='eninome'",'',trim($dados['eninome'])); 
				?>
			</td>
		</tr>
		<tr>
			<?php
			$sql = "select 
						est.estuf as codigo,
						est.estdescricao as descricao 
					from 
						projetos.enemmunicipio pde
					inner join
						territorios.estado est ON est.estuf = projetos.estuf 
					group by
						est.estuf,
						est.estdescricao
					order by 
						estdescricao";
			?>
			<td align='right' class="SubTituloDireita">Estado:</td>
			<td><?php 
				$permissao = $dados['eniuf'] ? "N" : "S";
				echo $db->monta_combo('eniuf',$sql,$permissao,'Selecione...','carregarMunicipio','','','200','S',"eniuf","",trim($dados['eniuf']));?></td>
		</tr> 
		<tr>
			<?php 
			
			if($dados['eniuf'] && $dados['enimunicipio']){
				$permissao = "N";
				$sql = "select 
							mun.muncod as codigo,
							mun.mundescricao || ' (' || projetos.enmqtdaluno || ')' as descricao 
						from 
							projetos.enemmunicipio pde
						inner join
							territorios.municipio mun ON mun.muncod = projetos.muncod
						where
							projetos.estuf = '{$dados['eniuf']}'
						group by 
							mun.muncod,
							mun.mundescricao,
							projetos.enmqtdaluno
						order by 
							mun.mundescricao";
			}else{
				$permissao = "N";
				$sql = "select 
							estuf as codigo,
							estdescricao as descricao 
						from 
							territorios.estado
						where
							1 = 2 
						order by 
							estdescricao";
			}
			?>
			<td align='right' class="SubTituloDireita">Município:</td>
			<td id="td_municipio" ><?php echo $db->monta_combo('enimunicipio',$sql,$permissao,'Selecione...','','','','200','S',"enimunicipio","",trim($dados['enimunicipio']));?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Contato:</td>
			<?php $permissao = $dados['enicontato'] ? "N" : "S"; 
				$enicontato = trim($dados['enicontato']); ?>
			<td><?php echo campo_textarea( 'enicontato', 'S' , $permissao, '', 60, 3, 500); ?></td>
		</tr>
		<tr bgcolor="#cccccc">
			<td colspan="2">
				<b>Restrição</b>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Opção:</td>
			<td>
			<?php $permissao = $dados['eniresopcao'] ? "disabled='disabled'" : "";?>
				<select id="eniresopcao" <?php echo $permissao ?> class="CampoEstilo" style="width:350px;" name="eniresopcao">
					<option value="">Selecione...</option>
					<option <?php  echo trim($dados['eniresopcao']) == "1" ? "selected=selected" : "" ?> value="1">Indisponibilidade de espaço físico na data do exame.</option>
					<option <?php  echo trim($dados['eniresopcao']) == "2" ? "selected=selected" : "" ?> value="2">Valor financeiro fora do padrão de mercado.</option>
					<option <?php  echo trim($dados['eniresopcao']) == "3" ? "selected=selected" : "" ?> value="3">Outro</option>
				</select>
				<?php echo obrigatorio(); ?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Descrição:</td>
			<?php $permissao = $dados['enirestricao'] ? "N" : "S"; $enirestricao = $dados['enirestricao']; ?>
			<td><?php echo campo_textarea( 'enirestricao', 'S', $permissao , '', 60, 3, 500); ?></td>
		</tr>
		<?php if($dados['eniid']){ ?>
			<tr>
				<td align='right' class="SubTituloDireita">Responsável:</td>
				<td>
					<select id="eniresponsavel" class="CampoEstilo" style="width: 200px;" name="eniresponsavel">
						<option value="">Selecione...</option>
						<option <?php  echo trim($dados['eniresponsavel']) == "SEB" ? "selected=selected" : "" ?> value="SEB">SEB</option>
						<option <?php  echo trim($dados['eniresponsavel']) == "SESU" ? "selected=selected" : "" ?> value="SESU">SESU</option>
						<option <?php  echo trim($dados['eniresponsavel']) == "SETEC" ? "selected=selected" : "" ?> value="SETEC">SETEC</option>
					</select>
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Situação:</td>
				<td>
					<input <?php  echo $dados['enisituacao'] == "D" ? "checked=checked" : "" ?> type="radio" name="enisituacao" id="enisituacao_d" value="D" />Descartada
					<input style="margin-left:20px" <?php  echo !$dados['enisituacao'] || $dados['enisituacao'] == "P" ? "checked=checked" : "" ?> type="radio" name="enisituacao" id="enisituacao_p" value="P" />Pendente
					<input style="margin-left:20px" <?php  echo $dados['enisituacao'] == "S" ? "checked=checked" : "" ?> type="radio"  id="enisituacao_s" name=enisituacao value="S" />Solucionada 
					 
				</td>
			</tr>
		<?php }else{ ?>
		<tr style="display:none">
				<td align='right' class="SubTituloDireita">Situação:</td>
				<td>
					<input type="hidden" name="eniresponsavel" id="eniresponsavel" value=""  />
					<input type="radio" name="enisituacao" id="enisituacao_p" checked="checked" value="P" />
				</td>
		</tr>
		<?php } ?>
		<tr bgcolor="#cccccc">
		<td></td>
			<td>
			<input type="button" class="botao" name="btassociar" value="Salvar" onclick="salvar();">
			<input type="button" class="botao" name="btassociar" value="Cancelar" onclick="history.back(-1);">
			</td>
		</tr>
		<?php if($dados['enirestricao'] || $dados['eniresopcao'] ){ ?>
		<tr>
			<td align='left' colspan="2">
			<span style="cursor: pointer;" onclick="cadastrar_observacao( <?php echo $_REQUEST['eniid'] ?> );" title="nova observação"> <img align="absmiddle" src="/imagens/gif_inclui.gif"/> Cadastrar Observação / Comentário </span>
			</td>
		</tr>
		<tr bgcolor="#cccccc">
			<td colspan="2">
				<b>Comentários / Observações</b>
			</td>
		</tr>
		<?php } ?>
		
	</table>
</form>
<div id="listaObs"> 
<?php 
	if($_REQUEST['eniid'] && ($dados['enirestricao']  || $dados['eniresopcao'] ) ){
		$sqlObs = "select
					(CASE (select count(eoiid) from  projetos.enemobsinstituicao obs1 where obs1.eniid = {$_REQUEST['eniid']} and obs1.eoidata > obs.eoidata and obsstatus = 'A')
					WHEN 0 THEN 
					(CASE obs.usucpf
						WHEN '{$_SESSION['usucpf']}' THEN '<center><img style=\"cursor:pointer\" src=\"/imagens/alterar.gif\" onclick=\"editarObservacao(\'{$_REQUEST['eniid']}\',\'' || obs.eoiid || '\')\" title=\"Alterar\" /> <img style=\"cursor:pointer\" src=\"/imagens/excluir.gif\" onclick=\"excluirObservacao(\'{$_REQUEST['eniid']}\',\'' || obs.eoiid || '\')\" title=\"Excluir\" /></center>' 
						ELSE '<center><img style=\"cursor:pointer\" src=\"/imagens/alterar.gif\" onclick=\"editarObservacao(\'{$_REQUEST['eniid']}\',\'' || obs.eoiid || '\')\" title=\"Alterar\" /> <img style=\"cursor:pointer\" src=\"/imagens/excluir_01.gif\" onclick=\"alert(\'Operação não permitida!\')\" title=\"Excluir\" /></center>'
				 	END)
					ELSE '<center><img style=\"cursor:pointer\" src=\"/imagens/alterar.gif\" onclick=\"editarObservacao(\'{$_REQUEST['eniid']}\',\'' || obs.eoiid || '\')\" title=\"Alterar\" /> <img style=\"cursor:pointer\" onclick=\"alert(\'Operação não permitida!\')\" src=\"/imagens/excluir_01.gif\" title=\"Excluir\" /></center>'
			 	END)as acao,
					usu.usunome,
					to_char(eoidata,'DD-MM-YYYY / HH24:MI:SS') as eoidata,
					eoidescricao
				from 
					projetos.enemobsinstituicao obs
				inner join
					seguranca.usuario usu ON usu.usucpf = obs.usucpf
				where 
					eniid = {$_REQUEST['eniid']}
				and
					obsstatus = 'A'
				order by
					eoidata";
		
		$cabecalho = array("&nbsp;Ações&nbsp;&nbsp;&nbsp;&nbsp;", "Usuário", "Data / Hora","Comentário");
		$db->monta_lista($sqlObs,$cabecalho,100,5,'N','center',$par2);
	}

?>
</div>

<script>

function carregarMunicipio(estuf){
	var td_municipio = document.getElementById('td_municipio');
	if(estuf){
		td_municipio.innerHTML = '<select id="muncod" class="CampoEstilo" disabled="disabled" style="width: 200px;" name="muncod_disable"><option value="">Carregando...</option></select>';
		var myAjax = new Ajax.Request(
		window.location.href,
		{
				method: 'post',
				parameters: 'evento=carregaMunicipio&estuf=' + estuf,
				asynchronous: false,
				onComplete: function(resp){
					td_municipio.innerHTML = resp.responseText;
					td_municipio.innerHTML += ' <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>';
				}
		});
	}else{
		td_municipio.innerHTML = '<select id="muncod" class="CampoEstilo" disabled="disabled" style="width: 200px;" name="muncod_disable"><option value="">Selecione...</option></select>';
		td_municipio.innerHTML += ' <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>'; 
		return false;
	}
}

function salvar(){
	var erro = 0;
	if(!document.getElementById('eninome').value){
		alert('Favor informar o Nome da Instituição.');
		document.getElementById('eninome').focus();
		erro = 1;
		return false;
	}
	if(!document.getElementById('eniuf').value){
		alert('Favor informar o Estado.');
		document.getElementById('eniuf').focus();
		erro = 1;
		return false;
	}
	if(!document.getElementById('enimunicipio').value){
		alert('Favor informar o Município.');
		document.getElementById('enimunicipio').focus();
		erro = 1;
		return false;
	}
	if(!document.getElementById('enicontato').value){
		alert('Favor informar o Contato.');
		document.getElementById('enicontato').focus();
		erro = 1;
		return false;
	}
	if(!document.getElementById('eniresopcao').value){
		alert('Favor selecionar a Opção da Restrição.');
		document.getElementById('eniresopcao').focus();
		erro = 1;
		return false;
	}
	if(!document.getElementById('enirestricao').value && document.getElementById('eniresopcao').value == 3){
		alert('Favor informar a Descrição da Restrição.');
		document.getElementById('enirestricao').focus();
		erro = 1;
		return false;
	}
	/*if(!document.getElementById('eniresponsavel').value){
		alert('Favor selecionar o Responsável.');
		document.getElementById('eniresponsavel').focus();
		erro = 1;
		return false;
	}*/
	<?php if($dados['eniid']){ ?>
	if(document.getElementById('enisituacao_d').checked == false && document.getElementById('enisituacao_s').checked == false && document.getElementById('enisituacao_p').checked == false && document.getElementById('enisituacao_e').checked == false){
		alert('Favor selecionar a Situação da Restrição.');
		erro = 1;
		return false;
	}
	<?php } ?>
	if(erro == 0){
		document.getElementById('evento').value = "salvarInstituicao";
		document.getElementById('formulario').submit();
	}else{
		alert('Não foi possível realizar a operação!');
		history.back(-1);
	}
}

function cadastrar_observacao(eniid){
	window.open('cadastroObservacaoEnem.php?eniid=' + eniid,'Observação / Comentário','scrollbars=yes,height=220,width=440,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}

function editarObservacao(eniid,eoiid){
	window.open('cadastroObservacaoEnem.php?eniid=' + eniid + '&eoiid=' + eoiid,'Observação / Comentário','scrollbars=yes,height=220,width=440,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}

function excluirObservacao(eniid,eoiid){
	if(confirm("Deseja realmente excluir?")){
		var listaObs = document.getElementById('listaObs');
		var myAjax = new Ajax.Request(
			window.location.href,
			{
					method: 'post',
					parameters: 'evento=excluirObservacao&eoiid=' + eoiid + '&eniid=' + eniid,
					asynchronous: false,
					onComplete: function(resp){
						listaObs.innerHTML = resp.responseText;
					}
			});
	}
}
</script>