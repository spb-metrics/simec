<?php

function carregaProjeto($prjid){
	global $db;
	$sql = "SELECT *
				FROM obras.projetos
			WHERE prjid = $prjid";
	return $db->carregar($sql);
}

function contadorAcesso($prjid){
	global $db;
	
	$sql = "SELECT nacvalor FROM obras.numacesso
			WHERE prjid = $prjid AND usucpf = '{$_SESSION['usucpf']}'";
	
	$acesso = $db->pegaUm($sql);
			
			if(!$acesso){
				$sql = "INSERT INTO obras.numacesso 
					(prjid,usucpf,nacvalor) VALUES
					($prjid, '{$_SESSION['usucpf']}',1)";
				$db->executar($sql);
			}else{
				$sql = "UPDATE obras.numacesso SET
						nacvalor = '".((int)$acesso + 1)."'
					WHERE usucpf = '{$_SESSION['usucpf']}' AND prjid = $prjid";
				$db->executar($sql);
			}
	$db->commit();
}



function salvarProjeto(){
	global $db;
	
	($_REQUEST['stpid'] == '')? $stpid = 'NULL' : $stpid = $_REQUEST['stpid'];
	if($_REQUEST['prjaceite']  == 'T') $prjaceite = 'true';
	else $prjaceite = 'false';
	
	$sql = "INSERT INTO obras.projetos
	 
		(
		usucpf, 
		ctpid, 
		tppid, 
		stpid, 
		entidunidade, 
		prjdsc, 
		prjdetalhamento, 
		prjstatus, 
		prjdtinclusao,
		prjaceite
		)
		values
		(
		'{$_SESSION['usucpf']}',
		'{$_REQUEST['ctpid']}',
		'{$_REQUEST['tppid']}',
		$stpid,
		'{$_REQUEST['entid']}',
		'{$_REQUEST['prjdsc']}',
		'{$_REQUEST['prjdetalhamento']}',
		'A',
		'now()',
		$prjaceite
		) returning prjid";
	$prjid = $db->pegaUm($sql);
	
	//Autores
	if($_REQUEST['praid']){
		foreach($_REQUEST['praid'] as $autor){
			$sql = "select praid 
						from obras.projetoautores 
					where entid = '$autor' and prjid = $prjid";
			$praid = $db->pegaUm($sql);
			if(!$praid){
				 $sql = "insert into obras.projetoautores
				 	(entid, prjid) values ('$autor','$prjid')";
				 $db->executar($sql);
			}
		}
	}
	//Expressão Chave
	if($_REQUEST['expressaochave']){
		foreach($_REQUEST['expressaochave'] as $expdsc){
			$sql = "select expid 
						from obras.expressaochave 
					where expdsc = '$expdsc' and prjid = $prjid";
			$expid = $db->pegaUm($sql);
			if(!$expid){
				 $sql = "insert into obras.expressaochave
				 	(expdsc, prjid,expstatus,expdtinclusao) values ('$expdsc','$prjid','A','now()')";
				 $db->executar($sql);
			}
		}
	}
	$db->commit();
	
	return $prjid;
}


function editarProjeto($prjid){
	global $db;
	
	($_REQUEST['stpid'] == '')? $stpid = 'NULL' : $stpid = $_REQUEST['stpid'];
	if($_REQUEST['prjaceite']  == 'T') $prjaceite = 'true';
	else $prjaceite = 'false';
	
	$sql = "UPDATE obras.projetos SET 
		ctpid = '{$_REQUEST['ctpid']}',
		tppid = '{$_REQUEST['tppid']}',
		stpid = $stpid,
		entidunidade = '{$_REQUEST['entid']}',
		prjdsc = '{$_REQUEST['prjdsc']}',
		prjdetalhamento = '{$_REQUEST['prjdetalhamento']}',
		prjaceite = $prjaceite
			WHERE prjid = '$prjid'";
	$db->executar($sql);
	
	//Autores
	if($_REQUEST['praid']){
		$sql = "DELETE 
					from obras.projetoautores 
				where prjid = $prjid";
		$db->executar($sql);
			
		foreach($_REQUEST['praid'] as $autor){
			$sql = "select praid 
						from obras.projetoautores 
					where entid = '$autor' and prjid = $prjid";
			$praid = $db->pegaUm($sql);
			if(!$praid){
				 $sql = "insert into obras.projetoautores
				 	(entid, prjid) values ('$autor','$prjid')";
				 $db->executar($sql);
			}
		}
	}
	//Expressão Chave - Inativos
	$sql = "UPDATE obras.expressaochave SET
		expstatus = 'I' 
			WHERE prjid = $prjid";
	$db->executar($sql);
	
	//Expressão Chave - Novos
	if($_REQUEST['expressaochave']){
		foreach($_REQUEST['expressaochave'] as $expdsc){
			$sql = "select expid 
						from obras.expressaochave 
					where expdsc = '$expdsc' and prjid = $prjid";
			$expid = $db->pegaUm($sql);
			if(!$expid){
				 $sql = "insert into obras.expressaochave
				 	(expdsc, prjid,expstatus,expdtinclusao) values ('$expdsc','$prjid','A','now()')";
				 $db->executar($sql);
			}
		}
	}
	
	//Expressão Chave - Antigos
	if($_REQUEST['expressaochave_antiga']){
		$n = 0;
		foreach($_REQUEST['expressaochave_antiga'] as $expdsc2){
			$sql = "UPDATE obras.expressaochave SET
				expdsc = '$expdsc2',
				expstatus = 'A'
				WHERE expid = {$_REQUEST['expid'][$n]}";
			$db->executar($sql);
			$n++;
		}
	
	}
	
	$db->commit();
}


function excluirAutor($entid){
	global $db;
	$sql = "DELETE FROM obras.projetoautores WHERE entid = $entid";
	$db->executar($sql);
	$db->commit();
}

function montaTipo($x){
	global $db;
	$sql_categoria = "
						SELECT 
							tppid as codigo, tppdesc as descricao 
						FROM
							obras.tipoprojeto
						WHERE ctpid = $x
						order by tppdesc";
		     	
	$db->monta_combo("tppid", $sql_categoria, "S", "Selecione...", 'seleciona_subtipo', '', '', '300', 'S','tipo');
}

function montaSubTipo($x){
	global $db;
	$sql_categoria = "
						SELECT 
							stpid as codigo, stpdsc as descricao 
						FROM
							obras.subtipoprojeto
						WHERE tppid = $x
						order by stpdsc";
		     	
	$db->monta_combo("stpid", $sql_categoria, "S", "Selecione...", '', '', '', '300', 'N','subtipo');
}

if($_REQUEST['Ajaxtipo']){
	header('content-type: text/html; charset=ISO-8859-1');
	montaTipo($_REQUEST['Ajaxtipo']);
	exit;
}
if($_REQUEST['Ajaxsubtipo']){
	header('content-type: text/html; charset=ISO-8859-1');
	montaSubTipo($_REQUEST['Ajaxsubtipo']);
	exit;
}

if($_REQUEST['AjaxExcluiAutor']){
	header('content-type: text/html; charset=ISO-8859-1');
	excluirAutor($_REQUEST['AjaxExcluiAutor']);
	exit;
}

if(!$_REQUEST['prjid'] && $_REQUEST['prjdsc'] && $_REQUEST['entid'] && $_REQUEST['ctpid'] && $_REQUEST['tppid']){
	$prjid = salvarProjeto();
	$_SESSION['prjid'] = $prjid;
	print "<script>alert('Operação Realizada com Sucesso!');window.location = 'obras.php?modulo=principal/cadastra_projeto&acao=A'</script>";
}

if($_REQUEST['prjid'] && $_REQUEST['prjdsc'] && $_REQUEST['entid'] && $_REQUEST['ctpid'] && $_REQUEST['tppid']){
	editarProjeto($_REQUEST['prjid']);
	$_SESSION['prjid'] = $_REQUEST['prjid'];
	print "<script>alert('Operação Realizada com Sucesso!');</script>";
}

if($_REQUEST['opt'] == 'editar' && $_REQUEST['prjid']){
	$_SESSION['prjid'] = $_REQUEST['prjid'];
	header( "Location: obras.php?modulo=principal/cadastra_projeto&acao=A" );
	exit();
}

if($_SESSION['prjid']){
	$projeto = carregaProjeto($_SESSION['prjid']);
	contadorAcesso($_SESSION['prjid']);
	//unset($_SESSION['prjid']);
	
}


include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br/>";

// Cria o título da tela
$titulo_modulo = "Cadastro de Projeto";
if($_SESSION['prjid']){
	//Cria a aba
	$db->cria_aba($abacod_tela,$url,'');
	$titulo_modulo = "Dados do Projeto";
}
monta_titulo( $titulo_modulo, "<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /> Indica os campos obrigatórios");
?>
<html>
<script src="/includes/prototype.js"></script>
<script src="/includes/entidades.js"></script>
<script src="/includes/calendario.js"></script>
<body>
<form name="formulario" id="pesquisar" method="POST" action="">
	<table align="center" border="0" cellpadding="5" cellspacing="1" class="tabela" cellpadding="0" cellspacing="0">
		<tr>
			<td class="SubTituloDireita">Nome do Projeto:</td>
			<td>
			<input type="hidden" name="prjid" id="prjid" value="<? echo $projeto[0]['prjid']; ?>">
				<? 
					$prjdsc = $projeto[0]['prjdsc']; 
				?>
				<?= campo_texto( 'prjdsc', 'S', $somenteLeitura, '', 65, 60, '', '', 'left', '', 0); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Unidade:</td>
			<td>
			<?php
				if($projeto[0]['entidunidade']){
					$sql = "SELECT entid,entnome
								FROM entidade.entidade
							WHERE entid = {$projeto[0]['entidunidade']}";
					$entidade = $db->carregar($sql);
					$entid = $entidade[0]['entid'];
					$entnome = $entidade[0]['entnome'];
				}
			
			?>
			
				<span id="entnome"><?php echo $entnome; ?></span>
			  <input type="hidden" name="entid" id="entid" value="<? echo $entid; ?>">
				<input type="button" name="pesquisar_entidade" value="Pesquisar" style="cursor: pointer;" onclick="inserirEntidade(document.getElementById('entid').value,2);" <?php if($somenteLeitura=="N") echo "disabled"; ?>><img src="../imagens/obrig.gif" title="Indica campo obrigatório." border="0">
			<input type="hidden" name="campus" id="campus" value="<? if(isset($campus)) echo $campus; ?>">
			<div style="display:none;" id="mostracampus"></div>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Categoria:</td>
			<td>
			<?php
			
			$ctpid = $projeto[0]['ctpid'];
				
				$sql_categoria = "
						SELECT 
							ctpid as codigo, ctpdsc as descricao 
						FROM
							obras.categoriaprojeto
						order by ctpdsc";
		     	
				$db->monta_combo("ctpid", $sql_categoria, "S", "Selecione...", 'seleciona_tipo', '', '', '300', 'S','categoria');
			?>
			</td>
		</tr>
		<tr id="tr_tipo">
			<td class="SubTituloDireita">Tipo:</td>
			<td id="td_tipo">
			<?php
			
			$tppid = $projeto[0]['tppid'];
			($tppid)? $montaCombo = 'S' : $montaCombo = 'N';
			
			if($projeto[0]['ctpid']){
				$sql_tipo = "
						SELECT 
							tppid as codigo, tppdesc as descricao 
						FROM
							obras.tipoprojeto
						WHERE ctpid = {$projeto[0]['ctpid']}
						order by tppdesc";	
			}else{
				$sql_tipo = "
						SELECT 
							tppid as codigo, tppdesc as descricao 
						FROM
							obras.tipoprojeto
						order by tppdesc";	
			}
		     	
				$db->monta_combo("tppid", $sql_tipo, $montaCombo, "Selecione...", 'seleciona_subtipo', '', '', '300', 'S','tipo');
			?>
			</td>
		</tr>
		<tr id="tr_subtipo">
			<td class="SubTituloDireita">Sub-Tipo:</td>
			<td id="td_subtipo">
			<?php
			
			$stpid = $projeto[0]['stpid'];
			($stpid)? $montaCombo = 'S' : $montaCombo = 'N';
			
			if($projeto[0]['tppid']){
				$sql_subtipo = "
						SELECT 
							stpid as codigo, stpdsc as descricao 
						FROM
							obras.subtipoprojeto
						WHERE tppid = {$projeto[0]['tppid']}
						order by stpdsc";	
			}else{
				$sql_subtipo = "
						SELECT 
							stpid as codigo, stpdsc as descricao 
						FROM
							obras.subtipoprojeto
						order by stpdsc";	
			}

				$db->monta_combo("stpid", $sql_subtipo, $montaCombo, "Selecione...", '', '', '', '300', 'N','subtipo');
				?>
			
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Detalhamento:</td>
			<td>
			<?$prjdetalhamento = $projeto[0]['prjdetalhamento'];?>
			<?= campo_textarea( 'prjdetalhamento', 'N', $somenteLeitura, '', '68', '6', '500', '' , 0, $desc_composicao); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Termos de aceite para<br /> publicação do projeto:</td>
			<td>
			<?
			$sql = "SELECT tacdsc FROM obras.termos WHERE tacstatus = 'A'";
			$termos = $db->pegaUm($sql); 
			?>
			<?= campo_textarea( 'termos', 'N', $somenteLeitura, '', '68', '6', '', '' , 0, $desc_composicao); ?>
			<?$prjtermo = $projeto[0]['prjaceite'];?>
			<input type="radio" <? if($prjtermo == 't')print "checked='checked'"; ?>  name="prjaceite" value="T" id="concordo" >Concordo <input type="radio" <? if($prjtermo == 'f')print "checked='checked'"; ?> name="prjaceite" value="F" id="nao_concordo" >Não Concordo 
			</td>
		</tr>
		<tr>
			<td  bgcolor="#CCCCCC" colspan="2"><b>Autor(es)</b></td>
		</tr>
		<tr>
		<td colspan="2">
			<table id="tabela_autor" width="100%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
			<tr>
				<td valign="top" align="center" class="title" style="width:80px; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
				<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Nome</strong></td>
			</tr>
			<?
			if($projeto[0]['prjid']){
				$sql = "select 
							ent.entid AS id,
							ent.entnome AS nome
						
						from obras.projetoautores AS ob
						left join obras.projetos AS prj ON prj.prjid = ob.prjid
						left join entidade.entidade AS ent ON ent.entid = ob.entid
						WHERE ob.prjid = {$projeto[0]['prjid']}";
				$autores = $db->carregar($sql);
				if($autores[0]) {
					foreach($autores as $in) {
						$acoes = "<input type=\"hidden\" value=\"".$in['id']."\" id=\"entid_".$in['id']."\" name=\"praid[]\"/>
									  <img src=\"/imagens/alterar.gif\" style=\"cursor:pointer\" border=\"0\" title=\"Editar\" onclick=\"atualizaAutor('linha_" . $in['id']."');\"/> 
									  <img src=\"/imagens/excluir.gif\" style=\"cursor:pointer\"  border=\"0\" title=\"Excluir\" onclick=\"deletarlinhaAutor(this,{$in['id']});\"/>";
						echo "<tr>";
						echo "<td><center>".$acoes."</center></td>";
						echo "<td>".strtoupper($in['nome'])."</td>";
					}
				}
			}
			?>
			</table>
			<a href="#" onclick="inserirAutor();"><img src="/imagens/gif_inclui.gif" style="cursor:pointer;" border="0" title="Inserir Autor(es)"> Inserir Autor(es)</a>
		</td>
	</tr>
		
		<tr>
			<td  bgcolor="#CCCCCC" colspan="2"><b>Expressão Chave</b></td>
		</tr>
		<tr>
		<td colspan="2">
			<table id="tabela_expressao" width="100%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
			<tr>
				<td valign="top" align="center" class="title" style="width:80px; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
				<td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Expressão Chave</strong></td>
			</tr>
			<tr>
				<td valign="top" align="center" class="title" style="width:80px; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><img style="cursor:pointer;" src="../imagens/gif_inclui.gif"  onclick="cadastrarExpressao();"></td>
				<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><input type="text" class="normal" id="expressao_chave" size="30" maxlength="20" value="<?=$_REQUEST["expressao_chave"]?>"></td>
			</tr>
			<?
			if($projeto[0]['prjid']){
				$sql = "SELECT * 
							FROM obras.expressaochave 
						WHERE prjid = {$projeto[0]['prjid']} 
						AND expstatus = 'A'";
				$pchave = $db->carregar($sql);
				if($pchave[0]) {
					foreach($pchave as $in) {
						$acoes = "<input type='hidden' name='expressaochave_antiga[]' value='".$in['expdsc']."'><input type='hidden' name='expid[]' value='".$in['expid']."'>
								  <img src=\"/imagens/alterar.gif\" style=\"cursor:pointer\" border=\"0\" title=\"Editar\" onclick='editarExpressao(this.parentNode.parentNode.parentNode.rowIndex);'/> 
								  <img src=\"/imagens/excluir.gif\" style=\"cursor:pointer\"  border=\"0\" title=\"Excluir\" onclick=\"deletarExpressao(this.parentNode.parentNode.parentNode.rowIndex);\"/>";
						echo "<tr>";
						echo "<td><center>".$acoes."</center></td>";
						echo "<td>".$in['expdsc']."</td>";
					}
				}
			}
			?>
			</table>
		</td>
	</tr>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input style="cursor:pointer;" type="button" value="Salvar" onclick="enviar();" /><input style="cursor:pointer;" type="button" value="Voltar" onCLick="voltar()" />
			</td>
		</tr>
	</table>
</form>
</body>
	<script>
		function validaForm(){
			if (!document.formulario.prjdsc.value ) {
				alert( 'Informe o Nome do Projeto!' );
				document.formulario.prjdsc.focus();
				return false;
			}
			if (!document.formulario.entid.value ) {
				alert( 'Informe a Unidade do Projeto!' );
				document.formulario.entid.focus();
				return false;
			}
			if (!document.formulario.ctpid.value ) {
				alert( 'Selecione a Categoria do Projeto!' );
				document.formulario.ctpid.focus();
				return false;
			}
			if (!document.formulario.tppid.value ) {
				alert( 'Selecione o Tipo do Projeto!' );
				document.formulario.tppid.focus();
				return false;
			}
			if (!document.getElementById('nao_concordo').checked && !document.getElementById('concordo').checked) {
				alert( 'Selecione o Termo de Aceite de Publicação do Projeto!' );
				document.getElementById('nao_concordo').focus();
				return false;
			}
			else{
				return true;
			}
		}
		function enviar(){
			if(validaForm() == true){
			formulario.submit();
			}
		}
		function voltar(){
			window.location = 'obras.php?modulo=principal/biblioteca_projeto&acao=A';
		}
		function inserirAutor(){
			windowOpen('?modulo=principal/autores_projeto&acao=A','blank','height=700,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
		}
		function atualizaAutor(ent){
			var entid = ent.replace('linha_', '');
			return windowOpen( '?modulo=principal/autores_projeto&acao=A&busca=entnumcpfcnpj&entid=' + entid,'blank','height=700,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' )	;
		}
		function deletarlinhaAutor(t,ent) {
			if(confirm("Deseja realmente excluir o autor?")) {
				var tab = document.getElementById('tabela_autor'); 
				var line = t.parentNode.parentNode.parentNode.rowIndex;
				tab.deleteRow(line);
				excluirAutor(ent);
			}
		}
		function RemoveLinha( idContato ){
				if (confirm('Deseja realmente excluir o autor?')){
					var index = window.document.getElementById('linha_' + idContato).rowIndex;
					table = window.document.getElementById("tabela_autor");
					table.deleteRow(index);
				}
		}
		function excluirAutor(ent){
			if(ent){
			// Faz uma requisição ajax
			div = document.getElementById('erro');	
			var req = new Ajax.Request('obras.php?modulo=principal/cadastra_projeto&acao=A', {
									        method:     'post',
									        parameters: '&AjaxExcluiAutor=' + ent,
									        onComplete: function (res)
									        {			
												div.innerHTML = res.responseText;
									        }
									  });
			}
		}
		
		function seleciona_tipo(y){
			var x = y;
			if(x){
			// Faz uma requisição ajax
			div = document.getElementById('td_tipo');	
			var req = new Ajax.Request('obras.php?modulo=principal/cadastra_projeto&acao=A', {
									        method:     'post',
									        parameters: '&Ajaxtipo=' + x,
									        onComplete: function (res)
									        {			
												div.innerHTML = res.responseText;
									        }
									  });
			}
		}
		function seleciona_subtipo(y){
			var x = y;
			if(x){
			// Faz uma requisição ajax
			div = document.getElementById('td_subtipo');	
			var req = new Ajax.Request('obras.php?modulo=principal/cadastra_projeto&acao=A', {
									        method:     'post',
									        parameters: '&Ajaxsubtipo=' + x,
									        onComplete: function (res)
									        {			
												div.innerHTML = res.responseText;
									        }
									  });
		}
		}
				
		function deletarExpressao(idlinha) {
			if(confirm("Deseja realmente excluir a expressão?")) {
				var tabela = document.getElementById('tabela_expressao');
				var linha = tabela.rows[idlinha];
				tabela.deleteRow(linha.rowIndex);
			}
		}
		
		function editarExpressao(idlinha) {
			var tabela = document.getElementById('tabela_expressao');
			if(document.getElementById('expressao_chave').value) {
				cadastrarExpressao();
				var linha = tabela.rows[idlinha+1];
			} else {
				var linha = tabela.rows[idlinha];
			}
			document.getElementById('expressao_chave').value = linha.cells[1].innerHTML;
			tabela.deleteRow(linha.rowIndex);
		}
		
		function cadastrarExpressao() {
			var expressao = document.getElementById('expressao_chave').value;
			if(expressao){
				var tabela = document.getElementById('tabela_expressao');
				var contador = document.getElementById('contador_exp');
				var linha = tabela.insertRow(2);
				cell1 = linha.insertCell(0);
				cell2 = linha.insertCell(1);
				cell1.style.textAlign = "center";
				cell1.innerHTML = "<img src='/imagens/alterar.gif' style='cursor:pointer;' border='0' title='Alterar' onclick='editarExpressao(this.parentNode.parentNode.rowIndex);'> " +
								  "<img src='/imagens/excluir.gif' style='cursor:pointer;' border='0' title='Excluir' onclick='deletarExpressao(this.parentNode.parentNode.rowIndex);'>" +
							  	  "<input type='hidden' name='expressaochave[]' value='"+expressao+"'>";
				cell2.innerHTML = expressao;	  	  
				document.getElementById('expressao_chave').value = '';
			}
			else{
				alert('Informe a Expressão Chave!');
			}
		}
				
	</script>
</html>