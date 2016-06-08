<?php

function excluirProjeto($prjid){
	global $db;
	
	$sql = "UPDATE obras.projetos SET prjstatus = 'I' WHERE prjid = $prjid";
	$db->executar($sql);
	
	$sql = "UPDATE obras.expressaochave SET expstatus = 'I' WHERE prjid = $prjid";
	$db->executar($sql);
	
	$sql = "UPDATE obras.anexosprojeto SET anpstatus = 'I' WHERE prjid = $prjid";
	$db->executar($sql);
	
	$sql = "select arqid FROM obras.anexosprojeto WHERE prjid = $prjid";
	$dados = array();
	$dados = $db->carregar($sql);
	
	if($dados){
		foreach($dados as $arqid){
			$sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid = {$arqid['arqid']}";
			$db->executar($sql);
		}
	}
	
	$db->commit();
}

function mostraProjetos($entid){
	global $db;
			
	// ----------- FILTROS ----------- 
	$condicaoInterna = array();
	$condicaoExterna = array();
	//categoria
	if ($_REQUEST['ctpidAjax'])
	{
		array_push( $condicaoExterna, " ctpid = {$_REQUEST['ctpidAjax']} " );
	}
	//tipo
	if ($_REQUEST['tppidAjax'])
	{
		array_push( $condicaoExterna, " tppid = {$_REQUEST['tppidAjax']} " );
	}
	//subtipo
	if ($_REQUEST['stpidAjax'])
	{
		array_push( $condicaoExterna, " stpid = {$_REQUEST['stpidAjax']} " );
	}
	//autor
	if ($_REQUEST['autidAjax'])
	{
		array_push( $condicaoInterna, " p.prjid in
		(
		SELECT prjid
			FROM obras.projetoautores
		WHERE entid = {$_REQUEST['autidAjax']}
		)" );
	}
	//unidade
	if ($_REQUEST['entidAjax'])
	{
		array_push( $condicaoExterna, " entidunidade = {$_REQUEST['entidAjax']} " );
	}
	//expressao chave
	if ($_REQUEST['expdscAjax'])
	{
		array_push( $condicaoInterna, " p.prjid in
		(
		SELECT prjid
			FROM obras.expressaochave
		WHERE expdsc like '%{$_REQUEST['expdscAjax']}%'
		AND expstatus = 'A'
		)" );
	}
	//fotos
	if ($_REQUEST['fotosAjax'])
		{
			($_REQUEST['fotosAjax'] == 'sim')? $fotos = '> 0 ' : $fotos = ' = 0 ';
			array_push( $condicaoExterna, " qtd_fotos ".$fotos."");
	}
	//Construindo a condição Externa
	$whereExterno = count( $condicaoExterna) ? " where " . implode( " and ", $condicaoExterna) : "";
	//Construindo a condição Interna
	$whereInterno = count( $condicaoInterna) ? " and " . implode( " and ", $condicaoInterna) : "";
	
	
	$sql = "SELECT  acao,
			(CASE qtd_fotos
				WHEN 0 THEN ''
				ELSE '<img src=\"/imagens/cam_foto.gif\" border=0 title=\"Fotos\" style=\"cursor:pointer;\" onclick=\"javascript:verFotos(' || id || ');\">'
			END
			),
			data,
			tipo,
			nome,
			qtd_anexos,
			qtd_acessos,
			instituicao
		
		FROM
			(
			SELECT
							 p.prjid AS id,
							 (CASE p.usucpf
							 		WHEN '{$_SESSION['usucpf']}' THEN
							 			'<img src=\"/imagens/alterar.gif\" border=0 title=\"Alterar\" style=\"cursor:pointer;\" onclick=\"javascript:editarProjeto(' || p.prjid || ');\">
										<img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"javascript:excluirProjeto(' || p.prjid || ');\">'
							 		ELSE
							 		(CASE p.prjaceite
							 			WHEN TRUE THEN 
							 			'<img src=\"/imagens/alterar.gif\" border=0 title=\"Alterar\" style=\"cursor:pointer;\" onclick=\"javascript:editarProjeto(' || p.prjid || ');\">
										<img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"javascript:excluirProjeto(' || p.prjid || ');\">'
							 			ELSE 
							 			'<img src=\"/imagens/alterar_01.gif\" border=0 title=\"Alterar\" style=\"cursor:pointer;\">
										 <img src=\"/imagens/excluir_01.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\">'
							 		END)
							 	END) as acao, 
										
							(
								SELECT count(*)
									FROM obras.fotosprojeto AS f
								WHERE f.prjid = p.prjid
							) AS qtd_fotos,
							to_char(p.prjdtinclusao,'dd/mm/yyyy') AS data,
							tp.tppdesc AS tipo,
							p.prjdsc AS nome,
							(
								SELECT count(*)
									FROM obras.anexosprojeto AS a
								WHERE a.prjid = p.prjid AND a.anpstatus = 'A'
							) || ' '  AS qtd_anexos,
							coalesce((
									SELECT SUM(nacvalor)
										FROM obras.numacesso AS n
									WHERE n.prjid = p.prjid
									GROUP BY n.prjid
								),0) || ' '  AS qtd_acessos,
							upper(ent.entnome) AS instituicao,
							p.entidunidade AS entidunidade,
							p.ctpid AS ctpid,
							p.tppid AS tppid,
							p.stpid AS stpid
							--a.entid AS autid,
							--exp.expdsc AS expdsc
							
						
						FROM obras.projetos AS p
						
						LEFT JOIN obras.categoriaprojeto AS c ON c.ctpid = p.ctpid
						LEFT JOIN obras.tipoprojeto AS tp ON p.tppid = tp.tppid
						LEFT JOIN entidade.entidade AS ent ON ent.entid = p.entidunidade
						--LEFT JOIN obras.expressaochave AS exp ON p.prjid = exp.prjid
						--LEFT JOIN obras.projetoautores AS a ON p.prjid = a.prjid
						
						WHERE p.prjstatus = 'A' AND ent.entid = $entid
						$whereInterno
			) AS pl
			
				$whereExterno
			
			order by data
							
	
		";
		$cabecalho = array( "Ação","F","Data da Inclusão","Tipo","Nome do Projeto","Nº de Anexos","Nº de Acessos","Instituição");
		$db->monta_lista( $sql, $cabecalho, 100, 30, '', 'center', '' );
}

function montaPesquisa(){
	global $db;
	
	// ----------- FILTROS ----------- 
	$condicaoInterna = array();
	$condicaoExterna = array();
	$condicaoExterna2 = array();
	//categoria
	if ($_REQUEST['ctpid'])
	{
		array_push( $condicaoExterna, " ctpid = {$_REQUEST['ctpid']} " );
	}
	//tipo
	if ($_REQUEST['tppid'])
	{
		array_push( $condicaoExterna, " tppid = {$_REQUEST['tppid']} " );
	}
	//subtipo
	if ($_REQUEST['stpid'])
	{
		array_push( $condicaoExterna, " stpid = {$_REQUEST['stpid']} " );
	}
	//autor
	if ($_REQUEST['autid'])
	{
		array_push( $condicaoInterna, " p.prjid in
		(
		SELECT prjid
			FROM obras.projetoautores
		WHERE entid = {$_REQUEST['autid']}
		)" );
	}
	//unidade
	if ($_REQUEST['entid'])
	{
		array_push( $condicaoExterna, " entidunidade = {$_REQUEST['entid']} " );
	}
	//expressao chave
	if ($_REQUEST['expdsc'])
	{
		array_push( $condicaoInterna, " p.prjid in
		(
		SELECT prjid
			FROM obras.expressaochave
		WHERE expdsc like '%{$_REQUEST['expdsc']}%'
		AND expstatus = 'A'
		)" );
	}
	//fotos
	if($_SESSION['visualizar'] == 'projeto'){
		if ($_REQUEST['fotos'])
		{
			($_REQUEST['fotos'] == 'sim')? $fotos = '> 0 ' : $fotos = ' = 0 ';
			array_push( $condicaoExterna, " qtd_fotos ".$fotos."");
		}
	}
	if($_SESSION['visualizar'] == 'unidade'){
		if ($_REQUEST['fotos'])
		{
			($_REQUEST['fotos'] == 'sim')? $fotos = '> 0 ' : $fotos = ' = 0 ';
			array_push( $condicaoExterna2, " qtd_fotos ".$fotos."");
		}
	}
	//Construindo a condição Externa
	$whereExterno = count( $condicaoExterna) ? " where " . implode( " and ", $condicaoExterna) : "";
	//Construindo a condição Interna
	$whereInterno = count( $condicaoInterna) ? " and " . implode( " and ", $condicaoInterna) : "";
	
	if($_SESSION['visualizar'] == 'unidade'){
	unset($whereExterno);
	$whereExterno = count( $condicaoExterna) ? " and " . implode( " and ", $condicaoExterna) : "";
	$whereExterno2 = count( $condicaoExterna2) ? " where " . implode( " and ", $condicaoExterna2) : "";
		
		$sql = "SELECT DISTINCT(acao),
							nome,
							qtd_proj,
							'<tr><td style=\"padding:0px;margin:0;\"></td><td id=\"td' || entid || '\" colspan=\"2\" style=\"padding:0px;display:none;border: 5px red\"></td><td style=\"padding:0px;margin:0;\"></td></tr>' as tr
				FROM 
			(
			SELECT 
							DISTINCT(ent.entid) AS entid,
							'<img src=\"/imagens/mais.gif\" id=\"img_mais_' || ent.entid || '\" border=0 title=\"Exibir Projetos\" style=\"cursor:pointer;\" onclick=\"javascript:mostraProjetos(' || ent.entid || ');\">
							<img src=\"/imagens/menos.gif\" id=\"img_menos_' || ent.entid || '\" border=0 title=\"Esconder Projetos\" style=\"cursor:pointer;display:none;\" onclick=\"javascript:escondeProjetos(' || ent.entid || ');\">' AS acao, 
							upper(ent.entnome) AS nome,
							(
								SELECT count(prjid)
									FROM obras.projetos AS p
								WHERE ent.entid = p.entidunidade
								
								$whereInterno $whereExterno
								
								AND prjstatus = 'A'
							) as qtd_proj,
							(
								SELECT count(*)
									FROM obras.fotosprojeto AS f
								WHERE f.prjid = p.prjid
							) AS qtd_fotos
							
								FROM entidade.entidade AS ent
							RIGHT JOIN obras.projetos AS p ON ent.entid = p.entidunidade
							
							WHERE prjstatus = 'A' $whereInterno $whereExterno
		
							ORDER BY ent.entid
			)AS tbl $whereExterno2 ";
		
		$cabecalho = array( "","Instituição","Quantidade de Projetos");

	}
	if($_SESSION['visualizar'] == 'projeto'){
		$sql = "SELECT  acao,
			(CASE qtd_fotos
				WHEN 0 THEN ''
				ELSE '<img src=\"/imagens/cam_foto.gif\" border=0 title=\"Fotos\" style=\"cursor:pointer;\" onclick=\"javascript:verFotos(' || id || ');\">'
			END
			),
			data,
			tipo,
			nome,
			qtd_anexos,
			qtd_acessos,
			instituicao
		
		FROM
			(
			SELECT
							 p.prjid AS id,
							(CASE p.usucpf
							 		WHEN '{$_SESSION['usucpf']}' THEN
							 			'<img src=\"/imagens/alterar.gif\" border=0 title=\"Alterar\" style=\"cursor:pointer;\" onclick=\"javascript:editarProjeto(' || p.prjid || ');\">
										<img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"javascript:excluirProjeto(' || p.prjid || ');\">'
							 		ELSE
							 		(CASE p.prjaceite
							 			WHEN TRUE THEN 
							 			'<img src=\"/imagens/alterar.gif\" border=0 title=\"Alterar\" style=\"cursor:pointer;\" onclick=\"javascript:editarProjeto(' || p.prjid || ');\">
										<img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"javascript:excluirProjeto(' || p.prjid || ');\">'
							 			ELSE 
							 			'<img src=\"/imagens/alterar_01.gif\" border=0 title=\"Alterar\" style=\"cursor:pointer;\">
										 <img src=\"/imagens/excluir_01.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\">'
							 		END)
							 	END) as acao,  
										
							(
								SELECT count(*)
									FROM obras.fotosprojeto AS f
								WHERE f.prjid = p.prjid
							) AS qtd_fotos,
							to_char(p.prjdtinclusao,'dd/mm/yyyy') AS data,
							tp.tppdesc AS tipo,
							p.prjdsc AS nome,
							(
								SELECT count(*)
									FROM obras.anexosprojeto AS a
								WHERE a.prjid = p.prjid AND a.anpstatus = 'A'
							) || ' '  AS qtd_anexos,
							coalesce((
									SELECT SUM(nacvalor)
										FROM obras.numacesso AS n
									WHERE n.prjid = p.prjid
									GROUP BY n.prjid
								),0) || ' '  AS qtd_acessos,
							upper(ent.entnome) AS instituicao,
							p.entidunidade AS entidunidade,
							p.ctpid AS ctpid,
							p.tppid AS tppid,
							p.stpid AS stpid
							--a.entid AS autid,
							--exp.expdsc AS expdsc
							
						
						FROM obras.projetos AS p
						
						LEFT JOIN obras.categoriaprojeto AS c ON c.ctpid = p.ctpid
						LEFT JOIN obras.tipoprojeto AS tp ON p.tppid = tp.tppid
						LEFT JOIN entidade.entidade AS ent ON ent.entid = p.entidunidade
						--LEFT JOIN obras.expressaochave AS exp ON p.prjid = exp.prjid
						--LEFT JOIN obras.projetoautores AS a ON p.prjid = a.prjid
						
						WHERE p.prjstatus = 'A'
						$whereInterno
			) AS pl
			
				$whereExterno
			
			order by data
							
	
		";
		$cabecalho = array( "Ação","F","Data da Inclusão","Tipo","Nome do Projeto","Nº de Anexos","Nº de Acessos","Instituição");
	
	}
	
	$db->monta_lista( $sql, $cabecalho, 100, 30, '', 'center', '' );
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
		     	
	$db->monta_combo("tppid", $sql_categoria, "S", "Todos...", 'seleciona_subtipo', '', '', '300', 'N','tipo');
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
		     	
	$db->monta_combo("stpid", $sql_categoria, "S", "Todos...", '', '', '', '300', 'N','subtipo');
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

if($_REQUEST['AjaxMostraProjetos']){
	header('content-type: text/html; charset=ISO-8859-1');
	mostraProjetos($_REQUEST['AjaxMostraProjetos']);
	exit;
}

if($_REQUEST['opt'] == 'Excluir' && $_REQUEST['prjid']){
	excluirProjeto($_REQUEST['prjid']);
	print "<script>window.location = 'obras.php?modulo=principal/biblioteca_projeto&acao=A';alert('Operação Realizada com Sucesso!');</script>";
}

if($_REQUEST['visualizar']){
	$_SESSION['visualizar'] = $_REQUEST['visualizar'];
}

if($_REQUEST['verTodos']){
	unset($_SESSION['visualizar']);
	header( "Location: obras.php?modulo=principal/biblioteca_projeto&acao=A" );
	exit();
}

//Destroi a sessão
unset($_SESSION['prjid']);

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br/>";

$titulo_modulo = "Biblioteca de Projetos";
monta_titulo( $titulo_modulo, 'Biblioteca de Projetos' );

?>
<html>
<script type="text/javascript" src="/includes/prototype.js"></script>
<body>
<form name="formulario" id="formulario" method="POST" action="">
	<table align="center" border="0" cellpadding="5" cellspacing="1" class="tabela" cellpadding="0" cellspacing="0">
		<tr>
			<td  bgcolor="#CCCCCC" colspan="2"><input type="hidden" id="pesquisa" name="pesquisa" value="1"><b>Argumentos da Pesquisa</b></td>
		</tr>
		<tr>
			<td width="25%"  class="SubTituloDireita">Visualizar por:</td>
			<td>
				<input type='radio' name='visualizar' id="visualizar_projeto" value='projeto' <? if($_SESSION["visualizar"] == "projeto") { echo "checked"; } ?> /> Projeto
				<input type='radio' name='visualizar' id="visualizar_unidade" value='unidade' <? if($_SESSION["visualizar"] == "unidade") echo "checked"; ?> /> Unidade
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Categoria:</td>
			<td>
			<?php
			 if($_REQUEST["ctpid"]){
			 		$ctpid = $_REQUEST["ctpid"];
			 }
			
				$sql_categoria = "
						SELECT 
							ctpid as codigo, ctpdsc as descricao 
						FROM
							obras.categoriaprojeto
						order by ctpdsc";
		     	
				$db->monta_combo("ctpid", $sql_categoria, "S", "Todas...", 'seleciona_tipo', '', '', '300', 'N','categoria');
			?>
			</td>
		</tr>
		<tr id="tr_tipo">
			<td class="SubTituloDireita">Tipo:</td>
			<td id="td_tipo">
			<?php
				$tppid = $_REQUEST['tppid'];
			($ctpid)? $montaCombo = 'S' : $montaCombo = 'N';
			
			if($ctpid){
				$sql_tipo = "
						SELECT 
							tppid as codigo, tppdesc as descricao 
						FROM
							obras.tipoprojeto
						WHERE ctpid = {$ctpid}
						order by tppdesc";	
			}else{
				$sql_tipo = "
						SELECT 
							tppid as codigo, tppdesc as descricao 
						FROM
							obras.tipoprojeto
						order by tppdesc";	
			}
		     	
				$db->monta_combo("tppid", $sql_tipo, $montaCombo, "Todas...", 'seleciona_subtipo', '', '', '300', 'N','tipo');
			?>
			</td>
		</tr>
		<tr id="tr_subtipo">
			<td class="SubTituloDireita">Sub-Tipo:</td>
			<td id="td_subtipo">
			<?php
				$stpid = $_REQUEST['stpid'];
			($tppid)? $montaCombo = 'S' : $montaCombo = 'N';
			
			if($tppid){
				$sql_subtipo = "
						SELECT 
							stpid as codigo, stpdsc as descricao 
						FROM
							obras.subtipoprojeto
						WHERE tppid = {$tppid}
						order by stpdsc";	
			}else{
				$sql_subtipo = "
						SELECT 
							stpid as codigo, stpdsc as descricao 
						FROM
							obras.subtipoprojeto
						order by stpdsc";	
			}

				$db->monta_combo("stpid", $sql_subtipo, $montaCombo, "Todos...", '', '', '', '300', 'N','subtipo');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Autor(es):</td>
			<td>
			<?php
			if($_REQUEST["autid"]){
					$autid = $_REQUEST["autid"];
			}
			
			$sql = "
				SELECT 
						distinct(ent.entid) as codigo, upper(ent.entnome) as descricao 
					FROM
						entidade.entidade AS ent
					RIGHT JOIN obras.projetoautores AS p ON p.entid = ent.entid
					order by descricao";
		     	
				$db->monta_combo("autid", $sql, "S", "Todos...", '', '', '', '300', 'N','autor');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Unidade:</td>
			<td>
			<?php
				if($_REQUEST["entid"]){
					$entid = $_REQUEST["entid"];
				}
				$sql_unidade = "
						SELECT 
							ent.entid as codigo, upper(ent.entnome) as descricao 
						FROM
							entidade.entidade ent
						INNER JOIN 
							obras.projetos p ON ent.entid= p.entidunidade 
							GROUP BY ent.entnome, ent.entid order by ent.entnome";
		     	
				$db->monta_combo("entid", $sql_unidade, "S", "Todas...", '', '', '', '300', 'N','unidade');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Expresão Chave:</td>
			<td>
			<?php 
			$expdsc = $_REQUEST["expdsc"];
			echo campo_texto( 'expdsc', 'N', 'S', '', 30, 30, '', '', 'left', '', 0, '');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Possui Fotos?</td>
			<td>
				<input type='radio' name='fotos' value='sim' <? if($_REQUEST["fotos"] == "sim") { echo "checked"; } ?> /> Sim
				<input type='radio' name='fotos' value='nao' <? if($_REQUEST["fotos"] == "nao") echo "checked"; ?> /> Não
			</td>
		</tr>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input style="cursor:pointer;" type="button" onCLick="pesquisar()" value="Pesquisar" /><input style="cursor:pointer;" type="button" value="Ver Todos" onCLick="verTodosProjetos();" />
			</td>
		</tr>
	</table>
</form>
<table align="center" border="0" class="tabela" cellpadding="0" cellspacing="0" bgcolor="#CCCCCC">
	<tbody>
		<tr>
			<td>
					<input type="button" name="cadastrar_projeto" id="cadastrar_projeto" value="Cadastrar Projeto" style="cursor: pointer;" onclick="cadastrar_projeto();"/>
			</td>
		</tr>
	</tbody>
</table>
<div id="erro"></div>
<?php	
	if($_REQUEST['pesquisa'] == 1 || $_SESSION['visualizar']){
		montaPesquisa();
	}	
	else{
		$sql = "SELECT  acao,
				(CASE qtd_fotos
					WHEN 0 THEN ''
					ELSE '<img src=\"/imagens/cam_foto.gif\" border=0 title=\"Fotos\" style=\"cursor:pointer;\" onclick=\"javascript:verFotos(' || id || ');\">'
				END
				),
				data,
				tipo,
				nome,
				qtd_anexos,
				qtd_acessos,
				instituicao
			
			FROM
				(
				SELECT
								 p.prjid AS id,
								 (CASE p.usucpf
							 		WHEN '{$_SESSION['usucpf']}' THEN
							 			'<img src=\"/imagens/alterar.gif\" border=0 title=\"Alterar\" style=\"cursor:pointer;\" onclick=\"javascript:editarProjeto(' || p.prjid || ');\">
										<img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"javascript:excluirProjeto(' || p.prjid || ');\">'
							 		ELSE
							 		(CASE p.prjaceite
							 			WHEN TRUE THEN 
							 			'<img src=\"/imagens/alterar.gif\" border=0 title=\"Alterar\" style=\"cursor:pointer;\" onclick=\"javascript:editarProjeto(' || p.prjid || ');\">
										<img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"javascript:excluirProjeto(' || p.prjid || ');\">'
							 			ELSE 
							 			'<img src=\"/imagens/alterar_01.gif\" border=0 title=\"Alterar\" style=\"cursor:pointer;\">
										 <img src=\"/imagens/excluir_01.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\">'
							 		END)
							 	END) as acao, 
											
								(
									SELECT count(*)
										FROM obras.fotosprojeto AS f
									WHERE f.prjid = p.prjid
								) AS qtd_fotos,
								to_char(p.prjdtinclusao,'dd/mm/yyyy') AS data,
								tp.tppdesc AS tipo,
								p.prjdsc AS nome,
								(
									SELECT count(*)
										FROM obras.anexosprojeto AS a
									WHERE a.prjid = p.prjid AND a.anpstatus = 'A'
								) || ' '  AS qtd_anexos,
								coalesce((
									SELECT SUM(nacvalor)
										FROM obras.numacesso AS n
									WHERE n.prjid = p.prjid
									GROUP BY n.prjid
								),0) || ' '  AS qtd_acessos,
								upper(ent.entnome) AS instituicao
							
							FROM obras.projetos AS p
				
							LEFT JOIN obras.categoriaprojeto AS c ON c.ctpid = p.ctpid
							LEFT JOIN obras.tipoprojeto AS tp ON p.tppid = tp.tppid
							LEFT JOIN entidade.entidade AS ent ON ent.entid = p.entidunidade
							
							WHERE p.prjstatus = 'A'
				) AS pl
				order by data
								
		
		";
		$cabecalho = array( "Ação","F","Data da Inclusão","Tipo","Nome do Projeto","Nº de Anexos","Nº de Acessos","Instituição");
		$db->monta_lista( $sql, $cabecalho, 100, 30, '', 'center', '' );
	}
?>
<table align="center" border="0" class="tabela" cellpadding="0" cellspacing="0" bgcolor="#CCCCCC">
	<tbody>
		<tr>
			<td>
				<input type="button" name="cadastrar_projeto_2" id="cadastrar_projeto_2" value="Cadastrar Projeto" style="cursor: pointer;" onclick="cadastrar_projeto();"/>
			</td>
		</tr>
	</tbody>
</table>
</body>
	<script>
		function validaForm(){
			if (document.getElementById('visualizar_projeto').checked == false && document.getElementById('visualizar_unidade').checked == false) {
				document.getElementById('visualizar_projeto').checked = true;
				return true;
			}
			else{
				return true;
			}
		}
		function pesquisar(){
			if(validaForm() == true){
			formulario.submit();
			}
		}
		function cadastrar_projeto(){
			window.location = 'obras.php?modulo=principal/cadastra_projeto&acao=A';
		}
		function verTodosProjetos(){
			window.location = 'obras.php?modulo=principal/biblioteca_projeto&acao=A&verTodos=1';
		}
		function verFotos(prjid){
			if(prjid){
				window.location = 'obras.php?modulo=principal/album_projeto&acao=A&verFotos=' + prjid;
			}
		}
		function seleciona_tipo(y){
			var x = y;
			if(x){
			// Faz uma requisição ajax
			div = document.getElementById('td_tipo');
			var req = new Ajax.Request('obras.php?modulo=principal/biblioteca_projeto&acao=A', {
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
			var req = new Ajax.Request('obras.php?modulo=principal/biblioteca_projeto&acao=A', {
									        method:     'post',
									        parameters: '&Ajaxsubtipo=' + x,
									        onComplete: function (res)
									        {			
												div.innerHTML = res.responseText;
									        }
									  });
			}
		}
		function editarProjeto(y){
			if(y){
				window.location = 'obras.php?modulo=principal/cadastra_projeto&acao=A&opt=editar&prjid=' + y;
			} 
		}
		function excluirProjeto(y){
			if(y){
				if(confirm('Deseja realmente excluir o Projeto?')){
					window.location = 'obras.php?modulo=principal/biblioteca_projeto&acao=A&opt=Excluir&prjid=' + y;
				}
			}
		}
		function mostraProjetos(entid){
			var x = entid;
			<?
			if($_REQUEST['pesquisa']){
				(!$_REQUEST['ctpid'])? $ctpidAjax = 0 : $ctpidAjax = $_REQUEST['ctpid'];
				(!$_REQUEST['stpid'])? $stpidAjax = 0 : $stpidAjax = $_REQUEST['stpid'];
				(!$_REQUEST['tppid'])? $tppidAjax = 0 : $tppidAjax = $_REQUEST['tppid'];
				(!$_REQUEST['autid'])? $autidAjax = 0 : $autidAjax = $_REQUEST['autid'];
				(!$_REQUEST['entid'])? $entidAjax = 0 : $entidAjax = $_REQUEST['entid'];
				(!$_REQUEST['expdsc'])? $expdscAjax = '' : $expdscAjax = $_REQUEST['expdsc'];
				(!$_REQUEST['fotos'])? $fotosAjax = 0 : $fotosAjax = $_REQUEST['fotos'];
			}
			 ?>
			if(x){
			// Faz uma requisição ajax
			div = document.getElementById('td' + x);	
			diverro = document.getElementById('erro');
			img_menos = document.getElementById('img_menos_' + x);
			img_mais = document.getElementById('img_mais_' + x);
			var req = new Ajax.Request('obras.php?modulo=principal/biblioteca_projeto&acao=A', {
									        method:     'post',
									        parameters: '&AjaxMostraProjetos=' + x + '<? print "&ctpidAjax=$ctpidAjax&stpidAjax=$stpidAjax&tppidAjax=$tppidAjax&autidAjax=$autidAjax&entidAjax=$entidAjax&expdscAjax=$expdscAjax&fotosAjax=$fotosAjax"; ?>',
									        onComplete: function (res)
									        {			
												div.innerHTML = res.responseText;
												div.style.display = '';
												img_mais.style.display = 'none';
												img_menos.style.display = '';
									        }
									  });
			}
		}
		function escondeProjetos(entid){
			var x = entid;
			if(x){
				div = document.getElementById('td' + x);	
				img_menos = document.getElementById('img_menos_' + x);
				img_mais = document.getElementById('img_mais_' + x);
				div.style.display = 'none';
				div.innerHTML = '';
				img_mais.style.display = '';
				img_menos.style.display = 'none';
			}
		}
	</script>
</html>