<?
function pesquisarInstituicao(){
	if($_POST['eninome']){
		$where[] = "upper(eninome) like upper('%{$_POST['eninome']}%')";
	}
	if($_POST['enicontato']){
		$where[] = "upper(enicontato) like upper('%{$_POST['enicontato']}%')";
	}
	if($_POST['enirestricao']){
		$where[] = "upper(enirestricao) like upper('%{$_POST['enirestricao']}%')";
	}
	if($_POST['eniuf']){
		$where[] = "eniuf = '{$_POST['eniuf']}'";
	}
	if($_POST['enimunicipio']){
		$where[] = "enimunicipio = '{$_POST['enimunicipio']}'";
	}
	if($_POST['eniresponsavel']){
		$where[] = "eniresponsavel = '{$_POST['eniresponsavel']}'";
	}
	if($_POST['enisituacao']){
		$where[] = "enisituacao = '{$_POST['enisituacao']}'";
	}
	
	return $where;
	
}

function carregaMunicipio(){
	global $db;
	$sql = "select 
				muncod as codigo,
				mundescricao as descricao 
			from 
				territorios.municipio
			where
				estuf = '{$_REQUEST['estuf']}' 
			order by 
				mundescricao";
	$db->monta_combo('enimunicipio',$sql,'S','Selecione...','','','','200','N',"muncod","",$_REQUEST['enimunicipio']);
	exit;
}

function excluirInstituicao(){
	global $db;
	$sql = "update 
					projetos.eneminstituicao 
				set
					enistatus = 'I'
				where
					eniid = '{$_REQUEST['eniid']}'";
		$db->executar($sql);
		$db->commit($sql);
}

function mostrarTodos(){
	unset($_REQUEST['eninome']);
	unset($_REQUEST['enicontato']);
	unset($_REQUEST['enirestricao']);
	unset($_REQUEST['eniuf']);
	unset($_REQUEST['enimunicipio']);
	unset($_REQUEST['eniresponsavel']);
	unset($_REQUEST['enisituacao']);
}

if(isset($_REQUEST['evento'])){
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
	
echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);

// titulos da tela
$titulo = "Lista de Instituições";
monta_titulo( $titulo, '&nbsp;' );

?>
<script language="javascript" type="text/javascript" src="/includes/prototype.js"></script>
<form method="POST"  id="formulario" name="formulario">
	<input type="hidden" id="evento" name="evento" value="" />
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td align='right' width=25% class="SubTituloDireita">Nome da Instituição:</td>
			<td><?php echo campo_texto('eninome','N','S','',60,60,'','','','','',"id='eninome'",'',$_REQUEST['eninome']); ?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Contato:</td>
			<td><?php echo campo_texto('enicontato','N','S','',50,50,'','','','','',"id='enicontato'",'',$_REQUEST['enicontato']); ?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Restrição:</td>
			<td><?php echo campo_texto('enirestricao','N','S','',50,50,'','','','','',"id='enirestricao'",'',$_REQUEST['enirestricao']); ?></td>
		</tr>
		<tr>
			<?php 
			$sql = "select 
						estuf as codigo,
						estdescricao as descricao 
					from 
						territorios.estado 
					order by 
						estdescricao";
			?>
			<td align='right' class="SubTituloDireita">Estado:</td>
			<td><?php echo $db->monta_combo('eniuf',$sql,'S','Selecione...','carregarMunicipio','','','200','N',"eniuf","",$_REQUEST['eniuf']);?></td>
		</tr> 
		<tr>
			<?php 
			if($_REQUEST['eniuf'] && $_REQUEST['enimunicipio']){
				$permissao = "S";
				$sql = "select 
							muncod as codigo,
							mundescricao as descricao 
						from 
							territorios.municipio
						where
							estuf = '{$_REQUEST['eniuf']}'
						order by 
							mundescricao";
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
			<td id="td_municipio" ><?php echo $db->monta_combo('enimunicipio',$sql,$permissao,'Selecione...','','','','200','N',"enimunicipio","",$_REQUEST['enimunicipio']);?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Responsável:</td>
			<td>
				<select id="eniresponsavel" class="CampoEstilo" style="width: 200px;" name="eniresponsavel">
					<option value="">Selecione...</option>
					<option <?php  echo trim($_REQUEST['eniresponsavel']) == "SEB" ? "selected=selected" : "" ?> value="SEB">SEB</option>
					<option <?php  echo trim($_REQUEST['eniresponsavel']) == "SESU" ? "selected=selected" : "" ?> value="SESU">SESU</option>
					<option <?php  echo trim($_REQUEST['eniresponsavel']) == "SETEC" ? "selected=selected" : "" ?> value="SETEC">SETEC</option>
				</select>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Situação:</td>
			<td>
				<select id="enisituacao" class="CampoEstilo" style="width: 200px;" name="enisituacao">
					<option value="">Selecione...</option>
					<option <?php  echo trim($_REQUEST['enisituacao']) == "D" ? "selected=selected" : "" ?> value="D">Descartada</option>
					<option <?php  echo trim($_REQUEST['enisituacao']) == "E" ? "selected=selected" : "" ?> value="E">Encaminhada</option>
					<option <?php  echo trim($_REQUEST['enisituacao']) == "P" ? "selected=selected" : "" ?> value="P">Pendente</option>
					<option <?php  echo trim($_REQUEST['enisituacao']) == "S" ? "selected=selected" : "" ?> value="S">Solucionada</option>
				</select>
			</td>
		</tr>
		<tr bgcolor="#cccccc">
		<td></td>
			<td>
			<input type="button" class="botao" name="btassociar" value="Pesquisar" onclick="submeter('P');">
			<input type="button" class="botao" name="btassociar" value="Mostrar Todos" onclick="mostrarTodos();">
			<input type="button" class="botao" name="btassociar" value="Cadastrar" onclick="window.location.href='projetos.php?modulo=principal/atividade_/cadastroEnem&acao=A';">
			</td>
		</tr>
	</table>
</form>
<?php 

if($_POST['evento'] == "pesquisarInstituicao"){
	$where = pesquisarInstituicao();
}
 
$sql = "select
				(CASE (select count(eoiid) from  projetos.enemobsinstituicao obs where obs.eniid = int.eniid and obs.obsstatus = 'A')
					WHEN 0 THEN 
					(CASE int.usucpf
						WHEN '{$_SESSION['usucpf']}' THEN '<center><img style=\"cursor:pointer\" src=\"/imagens/alterar.gif\" onclick=\"editarInstituicao(\'' || int.eniid || '\')\" title=\"Alterar\" /> <img style=\"cursor:pointer\" src=\"/imagens/excluir.gif\" onclick=\"excluirInstituicao(\'' || int.eniid || '\')\" title=\"Excluir\" /></center>' 
						ELSE '<center><img style=\"cursor:pointer\" src=\"/imagens/alterar.gif\" onclick=\"editarInstituicao(\'' || int.eniid || '\')\" title=\"Alterar\" /> <img style=\"cursor:pointer\" src=\"/imagens/excluir_01.gif\" onclick=\"alert(\'Operação não permitida!\')\" title=\"Excluir\" /></center>'
				 	END)
					ELSE '<center><img style=\"cursor:pointer\" src=\"/imagens/alterar.gif\" onclick=\"editarInstituicao(\'' || int.eniid || '\')\" title=\"Alterar\" /> <img style=\"cursor:pointer\" onclick=\"alert(\'Não é possível excluir esta instituição! Existem comentários atribuídos.\')\" src=\"/imagens/excluir_01.gif\" title=\"Excluir\" /></center>'
			 	END)as acao,
				int.eninome,
				est.estdescricao,
				mun.mundescricao,
				int.enicontato,
				(CASE int.eniresopcao
					WHEN '1' THEN 'Indisponibilidade de espaço físico na data do exame. ' || int.enirestricao
					WHEN '2' THEN 'Valor financeiro fora do padrão de mercado. ' || int.enirestricao	
					ELSE int.enirestricao
				 END) as enirestricao,
				(CASE enisituacao
					WHEN 'E' THEN 'Encaminhada'
					WHEN 'P' THEN 'Pendente'
					WHEN 'S' THEN 'Solucionada'
					WHEN 'D' THEN 'Descartada'
					ELSE 'N/A'
				 END) as enisituacao,
				 (CASE int.eniresponsavel
					WHEN '' THEN 'N/A'
					WHEN NULL THEN 'N/A'
					ELSE int.eniresponsavel
				 END) as eniresponsavel,
				 usu.usunome
			from
				projetos.eneminstituicao int
			left join
				territorios.estado est ON int.eniuf = est.estuf
			left join
				territorios.municipio mun ON int.enimunicipio = mun.muncod
			left join
				seguranca.usuario usu ON int.usucpf = usu.usucpf
			where
				enistatus = 'A' ".( count($where)? " AND ".implode(" AND " , $where) : "" );

$cabecalho = array("&nbsp;Ações&nbsp;&nbsp;&nbsp;&nbsp;", "Nome da Instituição", "Estado", "Município", "Contato", "Restrição","Situação","Responsável","Usuário");
$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);

?>

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
				}
		});
	}else{
		td_municipio.innerHTML = '<select id="muncod" class="CampoEstilo" disabled="disabled" style="width: 200px;" name="muncod_disable"><option value="">Selecione...</option></select>'; 
		return false;
	}
}

function mostrarTodos(){
	document.getElementById('evento').value = "mostrarTodos";
	document.getElementById('formulario').submit();
}

function submeter(opcao){
	document.getElementById('evento').value = "pesquisarInstituicao";
	document.getElementById('formulario').submit();
}

function editarInstituicao(eniid){
	window.location.href = 'projetos.php?modulo=principal/atividade_/cadastroEnem&acao=A&eniid=' + eniid;
}

function excluirInstituicao(eniid){
	if(confirm('Deseja realmente excluir?')){
		window.location.href = 'projetos.php?modulo=principal/atividade_/listaEnem&acao=A&eniid=' + eniid + '&evento=excluirInstituicao';
	}
}

</script>