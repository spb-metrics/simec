<?php

function carregarMenuObras() {
	global $db;
	
	$perfis = obras_arrayPerfil();
	
	$carq = false;
	if($db->testa_superuser() || in_array(PERFIL_ADMINISTRADOR, $perfis) || in_array(PERFIL_SUPERVISORMEC, $perfis) || in_array(PERFIL_SAA, $perfis)) {
		$carq = true;
	} else {
	
		$sql = "select count(a.arqid) as c
				from obras.fotos f 
				inner join obras.supervisao s on s.supvid=f.supvid 
				inner join public.arquivo a on a.arqid=f.arqid 
				inner join obras.obrainfraestrutura o ON o.obrid = s.obrid 
				inner join obras.situacaoobra so ON so.stoid = o.stoid 
				where a.arqid/1000 between 647 and 725 and 
				supstatus='A' and sisid=15 and obsstatus = 'A' and a.usucpf='".$_SESSION['usucpf']."'";
		
		$carq = $db->pegaUm($sql);
	} 
	
	if($carq) $menu[] = array("id" => 1, "descricao" => "VISTORIAS", "link" => "/obras/obras.php?modulo=sistema/public_arquivo/obras_arquivo&acao=A&tabela=fotos"); 
	
	$carq = false;
	if($db->testa_superuser() || in_array(PERFIL_ADMINISTRADOR, $perfis) || in_array(PERFIL_SUPERVISORMEC, $perfis) || in_array(PERFIL_SAA, $perfis)) {
		$carq = true;
	} else {
	
		$sql = "select count(a.arqid) as c
			 	from obras.arquivosobra f 
				inner join public.arquivo a on a.arqid=f.arqid 
				inner join obras.obrainfraestrutura o ON o.obrid = f.obrid 
				inner join obras.situacaoobra so ON so.stoid = o.stoid 
				where a.arqid/1000 between 647 and 725 and 
				aqostatus='A' and sisid=15  and obsstatus = 'A' and a.usucpf='".$_SESSION['usucpf']."'";
		
		$carq = $db->pegaUm($sql);
	}
	
	if($carq) $menu[] = array("id" => 2, "descricao" => "DOCUMENTOS", "link" => "/obras/obras.php?modulo=sistema/public_arquivo/obras_arquivo&acao=A&tabela=arquivosobra");
			  	  
	return $menu;
	
}

function arquivosobra() {
	
	global $db;
	
	monta_titulo( "Recuperação dos arquivos anexos da obra", "<span style=\"color:#0000FF\" >Depois de selecionar os arquivos, clique no botão <b>SALVAR</b> no final desta página.</span>");
	
	$cabecalho = array("CPF", "Nome", "ID obra", "Unidade", "Nome da obra", "Município / UF", "Convênio", "ID arquivo", "Nome do arquivo", "Descrição", "Tamanho (bytes)", "Data da inclusão (arquivo)", "");
	$clausula = "a.usucpf, u.usunome,";
	
	$perfis = obras_arrayPerfil();
	
	if(!$db->testa_superuser() && !in_array(PERFIL_ADMINISTRADOR, $perfis) && !in_array(PERFIL_SUPERVISORMEC, $perfis) && !in_array(PERFIL_SAA, $perfis)) {
		$cabecalho = array_reverse($cabecalho);
    	array_pop($cabecalho);
    	array_pop($cabecalho);
    	$cabecalho = array_reverse($cabecalho);
		unset($clausula);
	}else{
		?>
		<script>
			function pesquisar()
			{
				document.getElementById('formulario_pesquisa').submit();
			}
			
			function limpar()
			{
				window.location.href = window.location;
			}
		</script>
		<form name="formulario_pesquisa" id="formulario_pesquisa" method="post" action="">
			<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding=3 align="center">
				<tr>
					<td colspan="2" class="subTituloCentro">Argumentos de Pesquisa</td>
				</tr>
				<tr>
					<td width="25%" class="SubTituloDireita">Nome de Usuário:</td>
					<td>
						<?php echo campo_texto("usunome","S","S",'',80,200,"","",'',"",'',"","",$_REQUEST['usunome']) ?>
					</td>
				</tr>
				<tr>
					<td></td>
					<td>
						<input type="button" name="btn_buscar" value="Pesquisar" onclick="pesquisar()" />
						<input type="button" name="btn_buscar" value="Limpar" onclick="limpar()" />
					</td>
				</tr>
			</table>	
		</form>
		<?php
	}
	
	if($_REQUEST['usunome']){
		$arrWhere[] = "UPPER(removeacento(u.usunome)) like removeacento(('%".strtoupper($_REQUEST['usunome'])."%'))";	
	}
		
	$sql = "select {$clausula} f.obrid, ent.entnome, o.obrdesc, m.mundescricao||' / '||m.estuf as munest, o.numconvenio as convenio, a.arqid, a.arqnome||'.'||a.arqextensao, a.arqdescricao, a.arqtamanho, to_char(a.arqdata,'dd/mm/YYYY')||' '||a.arqhora as arqdata, '<span style=\"white-space: nowrap\" ><input type=\"file\" name=\"arquivo[' || a.arqid || ']\" id=\"arquivo_' ||  a.arqid || '\" > <img class=\"middle link\" onclick=\"limpaUpload(\'' || a.arqid || '\')\" src=\"../imagens/excluir.gif\" /></span>' as upload
			from obras.arquivosobra f 
			inner join obras.tipoarquivo ta ON ta.tpaid = f.tpaid 
			inner join public.arquivo a on a.arqid=f.arqid 
			inner join seguranca.usuario u ON u.usucpf = a.usucpf 
			inner join obras.obrainfraestrutura o ON o.obrid = f.obrid 
			left join entidade.entidade ent ON ent.entid = o.entidunidade 
			inner join obras.situacaoobra so ON so.stoid = o.stoid 
			inner join entidade.endereco e ON e.endid = o.endid 
			inner join territorios.municipio m ON m.muncod = e.muncod 
			where a.arqid/1000 between 647 and 725
			".($arrWhere? " and ".implode(" and ",$arrWhere) : "")." 
			and a.arqid not in(select arqid from public.arquivo_recuperado) and aqostatus='A' and sisid=15  and obsstatus = 'A' ".((!$db->testa_superuser() && !in_array(PERFIL_ADMINISTRADOR, $perfis) && !in_array(PERFIL_SUPERVISORMEC, $perfis) && !in_array(PERFIL_SAA, $perfis))?"and a.usucpf='".$_SESSION['usucpf']."'":"")."
			order by u.usunome";
	
	$db->monta_lista($sql,$cabecalho,10,5,'N','center',$par2,'form_arquivo');
	
}

function fotos() {
	
	global $db;
	
	monta_titulo( "Recuperação dos arquivos de vistoria da obra", "<span style=\"color:#0000FF\" >Depois de selecionar os arquivos, clique no botão <b>SALVAR</b> no final desta página.</span>");
	
	$cabecalho = array("CPF", "Nome", "ID obra", "Unidade", "Nome da obra", "Município / UF", "Convênio", "Data da Supervisão", "Data da inclusão (supervisão)", "ID arquivo", "Nome do arquivo", "Descrição", "Tamanho (bytes)", "Data da inclusão (arquivo)", "");
	$clausula = "a.usucpf, u.usunome,";
	
	$perfis = obras_arrayPerfil();
	
	if(!$db->testa_superuser() && !in_array(PERFIL_ADMINISTRADOR, $perfis) && !in_array(PERFIL_SUPERVISORMEC, $perfis) && !in_array(PERFIL_SAA, $perfis)) {
		$cabecalho = array_reverse($cabecalho);
    	array_pop($cabecalho);
    	array_pop($cabecalho);
    	$cabecalho = array_reverse($cabecalho);
		unset($clausula);
	}else{ ?>
	<script>
			function pesquisar()
			{
				document.getElementById('formulario_pesquisa').submit();
			}
			
			function limpar()
			{
				window.location.href = window.location;
			}
		</script>
		<form name="formulario_pesquisa" id="formulario_pesquisa" method="post" action="">
			<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding=3 align="center">
				<tr>
					<td colspan="2" class="subTituloCentro">Argumentos de Pesquisa</td>
				</tr>
				<tr>
					<td width="25%" class="SubTituloDireita">Nome de Usuário:</td>
					<td>
						<?php echo campo_texto("usunome","S","S",'',80,200,"","",'',"",'',"","",$_REQUEST['usunome']) ?>
					</td>
				</tr>
				<tr>
					<td></td>
					<td>
						<input type="button" name="btn_buscar" value="Pesquisar" onclick="pesquisar()" />
						<input type="button" name="btn_buscar" value="Limpar" onclick="limpar()" />
					</td>
				</tr>
			</table>	
		</form>
		<?php
	}
	
	if($_REQUEST['usunome']){
		$arrWhere[] = "UPPER(removeacento(u.usunome)) like removeacento(('%".strtoupper($_REQUEST['usunome'])."%'))";	
	}
		
	$sql = "select {$clausula} f.obrid, ent.entnome, o.obrdesc, m.mundescricao||' / '||m.estuf as munest, o.numconvenio as convenio, to_char(s.supvdt, 'dd/mm/YYYY') as datasupervisao, to_char(s.supdtinclusao, 'dd/mm/YYYY HH24:MI') as datainclusaosuper, a.arqid, a.arqnome||'.'||a.arqextensao, a.arqdescricao, a.arqtamanho, to_char(a.arqdata,'dd/mm/YYYY')||' '||a.arqhora as arqdata, '<span style=\"white-space: nowrap\" ><input type=\"file\" name=\"arquivo[' || a.arqid || ']\" id=\"arquivo_' ||  a.arqid || '\" > <img class=\"middle link\" onclick=\"limpaUpload(\'' || a.arqid || '\')\" src=\"../imagens/excluir.gif\" /></span>' as upload
			from obras.fotos f 
			inner join obras.supervisao s on s.supvid=f.supvid 
			inner join public.arquivo a on a.arqid=f.arqid 
			inner join seguranca.usuario u ON u.usucpf = a.usucpf 
			inner join obras.obrainfraestrutura o ON o.obrid = s.obrid 
			left join entidade.entidade ent ON ent.entid = o.entidunidade 
			inner join obras.situacaoobra so ON so.stoid = o.stoid 
			inner join entidade.endereco e ON e.endid = o.endid 
			inner join territorios.municipio m ON m.muncod = e.muncod 
			left join obras.tiposupervisao ts ON ts.tpsid = s.tpsid 
			where a.arqid/1000 between 647 and 725
			".($arrWhere? " and ".implode(" and ",$arrWhere) : "")."
			and a.arqid not in(select arqid from public.arquivo_recuperado) and supstatus='A' and sisid=15 and obsstatus = 'A' ".((!$db->testa_superuser() && !in_array(PERFIL_ADMINISTRADOR, $perfis) && !in_array(PERFIL_SUPERVISORMEC, $perfis) && !in_array(PERFIL_SAA, $perfis))?"and a.usucpf='".$_SESSION['usucpf']."'":"")." 
			order by u.usunome, o.obrdesc, s.supvdt";

	$db->monta_lista($sql,$cabecalho,10,5,'N','center',$par2,'form_arquivo');
	
}

?>