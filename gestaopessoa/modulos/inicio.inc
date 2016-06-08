<?php 
include  APPRAIZ."includes/cabecalho.inc";
echo '<br>';
monta_titulo( 'Gestão de Pessoas', '' );   
?>

<script src="../includes/prototype.js"></script>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"> 
	<tr>
		<th width ="50%">
			<a style="cursor:pointer;" onclick="window.location.href='gestaopessoa.php?modulo=principal/listaPessoalAvaliacao&acao=A'">Avaliação - GDPGPE</a>
		</th>
		<th>
		 	<!-- onclick="window.location.href='gestaopessoa.php?modulo=principal/listaPessoa&acao=A'"  -->
			<a  onclick="window.location.href='gestaopessoa.php?modulo=principal/listaPessoa&acao=A'"  style="cursor:pointer;" >Força de Trabalho</a>
		</th>
	</tr> 
	<tr>
		<td valign="top">
		<? if( controlaPermissao('administrador') || controlaPermissao('consulta') || controlaPermissao('superuser') ) { ?>
			<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" >
				<form id="formulario" name="formulario" method="post" action="?modulo=principal/listaPessoalAvaliacao&acao=A" >
				<input type="hidden" name="filtro_auto_aval"  id="filtro_auto_aval" value="" >
				<input type="hidden" name="filtro_aval_superior"  id="filtro_aval_superior" value="" >
				<input type="hidden" name="filtro_consenso"  id="filtro_consenso" value="" > 
				<input type="hidden" name="filtro_media" id="filtro_media" value = "">
				</form>
				<tr>
					<td> <a href="javascript: listaPessoalCompleta(); " >Total de Servidores </a></td>
					<td style="color:rgb(0, 102, 204);text-align:right"><?=qtdServidores(); ?></td>
				</tr>
				<!-- <tr>
					<td>Total de servidores cadastrados</td>
					<td style="color:rgb(0, 102, 204);text-align:right"><?=qtdServidores(1);?></td>
				</tr> -->
				<!-- <tr>
					<td><a href="javascript: listaPessoal('filtro_auto_aval', <?=TIPO_AUTO_AVAL;?>)">Total com Auto-avaliação preenchida </a></td>
					<td style="color:rgb(0, 102, 204);text-align:right"><?= getQuantidade(TIPO_AUTO_AVAL); ?></td>
				</tr> -->
				<tr>
					<td><a href="javascript: listaPessoal('filtro_aval_superior', <?=TIPO_AVAL_SUPERIOR;?>)">Total com Avaliação preenchida</a></td>
					<td style="color:rgb(0, 102, 204);text-align:right"><?= getQuantidade(TIPO_AVAL_SUPERIOR); ?></td>
				</tr>
				<!-- <tr>
					<td><a href="javascript: listaPessoal('filtro_consenso', <?=TIPO_AVAL_CONSENSO;?>)">Total com Consenso preenchido</a></td>
					<td style="color:rgb(0, 102, 204);text-align:right"><?= getQuantidade(TIPO_AVAL_CONSENSO); ?></td>
				</tr>
				<tr>
					<td><a href="javascript: listaPessoalMedia('filtro_media', <?=TIPO_MEDIA_CALCULADA;?>)">Total com Média Calculada</a></td>
					<td style="color:rgb(0, 102, 204);text-align:right"><?= getQtdMedia(); ?></td>
				</tr> -->
			</table>
		<?} ?>
		</td>
		<? if( controlaPermissao('administrador') || controlaPermissao('consulta') || controlaPermissao('superuser') ) { ?>
			<td>
				<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
				<form id="formulario" name="formulario" method="post" action="?modulo=principal/listaPessoalAvaliacao&acao=A" >
				<input type="hidden" name="filtro_auto_aval"  id="filtro_auto_aval" value="" >
				<input type="hidden" name="filtro_aval_superior"  id="filtro_aval_superior" value="" >
				<input type="hidden" name="filtro_consenso"  id="filtro_consenso" value="" > 
				<input type="hidden" name="filtro_media" id="filtro_media" value = "">
				</form>
				<tr>
					<td colspan="2">
					<b>Geral</b>
					</td>
				</tr>
				<tr>
					<td>  Total de Servidores </td>
					<?php 
					$sqlb = "	SELECT count( fdpcpf ) 
								FROM gestaopessoa.ftdadopessoal as fdp  
								INNER JOIN gestaopessoa.ftsituacaotrabalhador AS fst ON fst.fstid = fdp.fstid 
								WHERE fdp.fdpcpf IS NOT NULL
								AND fst.fstid <> 4								
								"; 
					?>
					<td style="color:rgb(0, 102, 204);text-align:right"><?= $db->pegaUm($sqlb); ?></td>
				</tr>
				<tr>
					<td colspan="2">
					<img id="img_unidade" onclick="onOff('unidade');" src="../imagens/mais.gif">&nbsp;<b>Total de servidores X Unidade de Lotação</b>
					</td>
				</tr>
				<tr id="unidade" style="display:none;" >
				<td colspan="2">
				<?php $sql = "SELECT * FROM gestaopessoa.ftunidadelotacao"; 
					$rs = $db->carregar( $sql );
					foreach( $rs as $tp ){ ?>
							  - <a href="javascript: listaPessoalFiltro('filtro_ftu_fulid', <?=$tp['fulid'];?>);"><?=$tp['fuldescricao'];?></a>
								<a style="position: absolute; right:3%;color:rgb(0, 102, 204); "> 
								<?
								$sqlb = "	SELECT count(x.contagem) FROM			
											  ( SELECT ful.fdpcpf as contagem
												FROM 
												gestaopessoa.ftdadopessoal as fdp 
												INNER JOIN gestaopessoa.ftdadofuncional as ful ON fdp.fdpcpf = ful.fdpcpf 
												INNER JOIN gestaopessoa.ftsituacaotrabalhador AS fst ON fst.fstid = fdp.fstid 
												WHERE fdp.fdpcpf IS NOT NULL
												AND fst.fstid <> 4					
												AND fulid = ".$tp['fulid'] . " ) AS x ";			
								echo $db->pegaUm($sqlb); ?>
								</a><br>
						 <hr>
					<?}
				?>
				</td>
				</tr>
				<tr>
					<td colspan="2">
					<img id="img_situacao" onclick="onOff('situacao');"src="../imagens/mais.gif">&nbsp;<b>Dados Funcionais por Situação no MEC</b>
					</td>
				</tr>
				<tr id="situacao" style="display: none;">
				<td colspan="2">
				<?php $sql = "SELECT * FROM gestaopessoa.ftsituacaotrabalhador WHERE fstid <> 4"; 
					$rs = $db->carregar( $sql );
					foreach( $rs as $tp ){ ?>
						  - <a href="javascript: listaPessoalFiltro('filtro_fst_fstid', <?=$tp['fstid'];?>);"><?=$tp['fstdescricao'];?></a>
							<a style="position: absolute; right:3%; color:rgb(0, 102, 204);">
							<?
								$sqlb = "	SELECT count( fdpcpf ) 
											FROM 
											gestaopessoa.ftdadopessoal as fdp 
											INNER JOIN gestaopessoa.ftsituacaotrabalhador AS fst ON fst.fstid = fdp.fstid 
											WHERE fdp.fdpcpf IS NOT NULL
											AND fst.fstid = ".$tp['fstid'];					
								echo $db->pegaUm($sqlb); 
							?>		
							</a></br>
							<hr>
					<?}
				?>
				</td>
				</tr>
				<tr>
					<td colspan="2">
					<b>Formação Acadêmica</b>
					</td>
				</tr>
				
				<tr>
					<td colspan="2">
					<img id="img_formacao" onclick="onOff('formacao');" src="../imagens/mais.gif">&nbsp;<b>Formação Acadêmica X Grau de Escolaridade</b>
					</td>
				</tr>
				<tr id="formacao" style="display: none;">
				<td colspan="2">
				
				<?php $sql = "SELECT  tfoid, tfodsc from public.tipoformacao where tfoid not in (5,6,7)"; 
					$rs = $db->carregar( $sql );
					foreach( $rs as $tp ){ ?>
						  - <a href="javascript: listaPessoalFiltro('filtro_tfo_tfoid', <?=$tp['tfoid'];?>);"><?=$tp['tfodsc'];?></a>
							<a style="position: absolute; right:3%;color:rgb(0, 102, 204);">
							<?
							$sqlb = "	SELECT count(x.contagem) FROM 
 										  ( SELECT distinct fdp.fdpcpf as contagem 
 											FROM 
 											gestaopessoa.ftdadopessoal as fdp 
 											INNER JOIN gestaopessoa.ftsituacaotrabalhador AS fst ON fst.fstid = fdp.fstid
 											LEFT  JOIN gestaopessoa.ftformacaoacademica as formacao ON fdp.fdpcpf = formacao.fdpcpf     
 											WHERE fdp.fdpcpf IS NOT NULL
 											AND fst.fstid <> 4
 											AND formacao.tfoid = " . $tp['tfoid'] . " ) AS x ";
								echo $db->pegaUm($sqlb);  
							?>	
							</a></br>							
							<hr>
					<?}					
				?>
				</td>
				</tr> 
				<tr>
					<td colspan="2">
					<img id="img_idioma" onclick="onOff('idioma');" src="../imagens/mais.gif">&nbsp;<b>Idioma</b>
					</td>
				</tr>
				<tr id="idioma" style="display: none;">
				<td colspan="2">
				<?php $sql = "SELECT * FROM gestaopessoa.ftitipoidioma"; 
					$rs = $db->carregar( $sql );
					foreach( $rs as $tp ){ ?>
 						  - <a href="javascript: listaPessoalFiltro('filtro_fti_ftiid', <?=$tp['ftiid'];?>);"><?=$tp['ftidescricao'];?></a> 
							<a style="position: absolute; right:3%;color:rgb(0, 102, 204);">  
							<?
							$sqlb = "	SELECT count(x.contagem) FROM 
 										  ( SELECT distinct idioma.fdpcpf as contagem 
 											FROM 
 											gestaopessoa.ftdadopessoal as fdp 
 											INNER JOIN gestaopessoa.ftsituacaotrabalhador AS fst ON fst.fstid = fdp.fstid
 											LEFT  JOIN gestaopessoa.idioma as idioma ON fdp.fdpcpf = idioma.fdpcpf 
 											WHERE fdp.fdpcpf IS NOT NULL
 											AND fst.fstid <> 4
 											AND idioma.ftiid = " . $tp['ftiid'] . " ) AS x ";
								echo $db->pegaUm($sqlb);  
							?>									
							</a><br>
							<hr>
					<?}
				?>
				</td>
				</tr>
				<tr>
					<td colspan="2">
					<b>Atividade Desenvolvida</b>
					</td>
				</tr>				
				<tr>
					<td colspan="2">
					<img id="img_atividade" onclick="onOff('atividade');" src="../imagens/mais.gif">&nbsp;<b>Atividade Desenvolvida X Tipo</b>
					</td>
				</tr>
				<tr id="atividade" style="display: none;">
				<td colspan="2">
				<?php $sql = "SELECT * FROM gestaopessoa.fttipoatividadedesenvolvida"; 
					$rs = $db->carregar( $sql );
					foreach( $rs as $tp ){ ?>
					 	- <a href="javascript: listaPessoalFiltro('filtro_fta_ftaid', <?=$tp['ftaid'];?>);"><?=$tp['ftadescricao'];?></a>
						  <a style="position: absolute; right:3%; color:rgb(0, 102, 204);">
						  <?
						  $sqlb = "	SELECT count(x.contagem) FROM 
 										  ( SELECT distinct fdp.fdpcpf as contagem 
 											FROM 
 											gestaopessoa.ftdadopessoal as fdp 
 											INNER JOIN gestaopessoa.ftsituacaotrabalhador AS fst ON fst.fstid = fdp.fstid
 											LEFT  JOIN gestaopessoa.ftatividadedesenvolvida as atividadedesenv ON fdp.fdpcpf = atividadedesenv.fdpcpf 
 											WHERE fdp.fdpcpf IS NOT NULL
 											AND fst.fstid <> 4
 											AND atividadedesenv.ftaid = " . $tp['ftaid'] . " ) AS x ";
						  echo $db->pegaUm($sqlb);  						
						  ?>	
						</a></br>
						<hr>
					<?}
				?>
				</td>
				</tr>
				<tr>
					<td colspan="2">
					<img id="img_atividade_nivel" src="../imagens/mais.gif" onclick="onOff('atividade_nivel');">&nbsp;<b>Atividade Desenvolvida X Nível</b>
					</td>
				</tr>
				<tr id="atividade_nivel" style="display:none;">
				<td colspan="2">
				<?php $sql = "SELECT * FROM gestaopessoa.fttiponivelatividadedesenvolvid"; 
					$rs = $db->carregar( $sql );
					foreach( $rs as $tp ){ ?>
 					  - <a href="javascript: listaPessoalFiltro('filtro_fta_fnaid', <?=$tp['fnaid'];?>);"><?=$tp['fnadescricao'];?></a> 
						<a style="position: absolute; right:3%; color:rgb(0, 102, 204);">
  						  <?
						  $sqlb = "	SELECT count(x.contagem) FROM 
 										  ( SELECT distinct fdp.fdpcpf as contagem 
 											FROM 
 											gestaopessoa.ftdadopessoal as fdp 
 											INNER JOIN gestaopessoa.ftsituacaotrabalhador AS fst ON fst.fstid = fdp.fstid
 											LEFT  JOIN gestaopessoa.ftatividadedesenvolvida as atividadedesenv ON fdp.fdpcpf = atividadedesenv.fdpcpf 
 											WHERE fdp.fdpcpf IS NOT NULL
 											AND fst.fstid <> 4
 											AND atividadedesenv.fnaid = " . $tp['fnaid'] . " ) AS x ";
						  echo $db->pegaUm($sqlb);  						
						  ?>							
						</a><br>
						<hr> 
					<?}
				?>
				</td>
				</tr>
				<tr>
					<td colspan="2">
 						<b>Experiência Anterior</b>
					</td>
				</tr> 
				<tr>
					<td colspan="2">
					<img id="img_experiencia_tipo" src="../imagens/mais.gif" onclick="onOff('experiencia_tipo');">&nbsp;<b>Experiência Anterior X Tipo</b>
					</td>
				</tr>
				<tr id="experiencia_tipo" style="display:none;">
				<td colspan="2">
				<?php $sql = "SELECT * FROM gestaopessoa.fttipoexperienciaanterior"; 
					$rs = $db->carregar( $sql );
					foreach( $rs as $tp ){ ?>
						- <a href="javascript: listaPessoalFiltro('filtro_fte_fteid', <?=$tp['fteid'];?>);"><?=$tp['ftedescricao'];?></a>
  						  <a style="position: absolute; right:3%; color:rgb(0, 102, 204);">
  						  <?
						  $sqlb = "	SELECT count(x.contagem) FROM 
 										  ( SELECT distinct fdp.fdpcpf as contagem 
 											FROM 
 											gestaopessoa.ftdadopessoal as fdp 
 											INNER JOIN gestaopessoa.ftsituacaotrabalhador AS fst ON fst.fstid = fdp.fstid
 											LEFT  JOIN gestaopessoa.ftexperienciaanterior AS fte ON fdp.fdpcpf = fte.fdpcpf 
 											WHERE fdp.fdpcpf IS NOT NULL
 											AND fst.fstid <> 4
 											AND fte.fteid = " . $tp['fteid'] . " ) AS x ";
						  echo $db->pegaUm($sqlb);  						
						  ?>  						 
	 					  </a><hr>
				  <?}
				?>
				</td>
				</tr> 
				<tr>
					<td colspan="2">
					<img id="img_experiencia_nivel" src="../imagens/mais.gif" onclick="onOff('experiencia_nivel');">&nbsp;<b>Experiência Anterior X Nível</b>
					</td>
				</tr>
				<tr id="experiencia_nivel" style="display: none;">
				<td colspan="2">
				<?php $sql = "SELECT * FROM gestaopessoa.fttiponivelexperienciaanterior"; 
					$rs = $db->carregar( $sql );
					foreach( $rs as $tp ){ ?>
					  - <a href="javascript: listaPessoalFiltro('filtro_fte_fneid', <?=$tp['fneid'];?>);"><?=$tp['fnedescricao'];?></a>
						<a style="position: absolute; right:3%; color:rgb(0, 102, 204);">
  						  <?
						  $sqlb = "	SELECT count(x.contagem) FROM 
 										  ( SELECT distinct fdp.fdpcpf as contagem 
 											FROM 
 											gestaopessoa.ftdadopessoal as fdp 
 											INNER JOIN gestaopessoa.ftsituacaotrabalhador AS fst ON fst.fstid = fdp.fstid
 											LEFT  JOIN gestaopessoa.ftexperienciaanterior AS fte ON fdp.fdpcpf = fte.fdpcpf 
 											WHERE fdp.fdpcpf IS NOT NULL
 											AND fst.fstid <> 4
 											AND fte.fneid = " . $tp['fneid'] . " ) AS x ";
						  echo $db->pegaUm($sqlb);  						
						  ?>  						 
	 					</a><hr>						
					<?}
				?>
				 </td>
				</tr>
				 
			</table>
					<?} ?>
			</td>
		</tr>
</table>
<script>
 
function listaPessoalFiltro(filtro, valor){ 
	window.location.href='gestaopessoa.php?modulo=principal/listaPessoa&acao=A&' + filtro + '=' + valor;
}
function listaPessoal(filtro, id){ 
	var filtro = document.getElementById(filtro);
	filtro.value = id;
	document.formulario[0].submit();
}
function listaPessoalMedia(filtro, id){ 
	var filtro = document.getElementById(filtro);
	filtro.value = 't';
	document.formulario[0].submit();
 
}
function listaPessoalCompleta(){ 
	 window.location.href='gestaopessoa.php?modulo=principal/listaPessoalAvaliacao&acao=A'; 
}
function onOff( div ){
	if( !div){
		return false;
	}
	if( document.getElementById(div).style.display == 'none'){
		document.getElementById(div).style.display = '';
		document.getElementById('img_'+div).src= '../imagens/menos.gif';
	}else{
		document.getElementById(div).style.display = 'none';
		document.getElementById('img_'+div).src= '../imagens/mais.gif';
	} 
}
</script>