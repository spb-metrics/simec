<?php 
	$arrPerfisWorkflow = array(
							PERFIL_SUPERUSUARIO, 
							PERFIL_GESTORORGAO,
							/* PERFIL_GESTORUNIDADE, */
							PERFIL_SUPERVISORORGAO,
							PERFIL_SUPERVISORUNIDADE
							  );
	if(($_REQUEST['esdid'] == WF_ESTADO_EM_CADASTRAMENTO || $_REQUEST['esdid'] == WF_ESTADO_ENVIADO_PARA_VALIDACAO)){
		$esdid    = $_REQUEST['esdid'];
		//verifica se o usuário possui perfil para tramitar
		if(possuiPerfil($arrPerfisWorkflow)){
			$checkBox = false;
			//verifica se o perfil do usuário tem permissão de tramitar neste estado
			if(possuiPerfil(array(PERFIL_SUPERVISORUNIDADE/*,PERFIL_GESTORUNIDADE*/)) 
			&& $_REQUEST['esdid'] == WF_ESTADO_EM_CADASTRAMENTO )
				$checkBox = true;
			
			//verifica se o perfil do usuário tem permissão de tramitar neste estado
			if(possuiPerfil(array(PERFIL_SUPERVISORORGAO,PERFIL_GESTORORGAO)) 
			&& $_REQUEST['esdid'] == WF_ESTADO_ENVIADO_PARA_VALIDACAO )
				$checkBox = true;
				
			//tramita o documento
			if($_REQUEST['act'] == 'tramitar'){
				//verifica os estados para executar as ações
				if($esdid == WF_ESTADO_EM_CADASTRAMENTO)
					$aedid = WF_ACAO_ENVIAR_VALIDACAO_ORGAO;
				elseif($esdid == WF_ESTADO_ENVIADO_PARA_VALIDACAO)
					$aedid = WF_ACAO_ENVIAR_PARA_VALIDADO;
					
				foreach ($_REQUEST['docid'] as $docid){
					$obrid = $db->pegaUm("SELECT obrid FROM obras.supervisao WHERE docid = {$docid}");
					wf_alterarEstado( $docid, $aedid, '', array('obrid' => $obrid));
				}
				$db->sucesso('vistorias_pendentes');
			}	
		}else
			$checkBox = false;
	}else
		$esdid = WF_ESTADO_EM_CADASTRAMENTO."', '".WF_ESTADO_ENVIADO_PARA_VALIDACAO ;
		
	include  APPRAIZ."includes/cabecalho.inc";
	echo "<br>";
	monta_titulo($titulo_modulo, '');
	$perfis = implode(" ',' ", pegaPerfilArray($_SESSION['usucpf']));
	
	$sql = "
			SELECT DISTINCT
				esd.esdid  AS codigo,
				esd.esddsc AS descricao
			FROM 
				workflow.estadodocumento esd
			INNER JOIN
				workflow.acaoestadodoc aed ON esd.esdid = aed.esdiddestino 
							  AND aed.aedstatus = 'A'
			INNER JOIN
				workflow.estadodocumentoperfil edp ON aed.aedid = edp.aedid 
							         -- AND edp.pflcod IN ('{$perfis}')
			WHERE
				(esd.esdid = ".WF_ESTADO_EM_CADASTRAMENTO." 
			  OR esd.esdid = ".WF_ESTADO_ENVIADO_PARA_VALIDACAO.") 
			 	AND esd.esdstatus = 'A'";
	$rs = $db->carregar($sql);
	?>
<form name="formulario" method="post" action="">
	<input type="hidden" name="act" value="">
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding=3 align="center" border="0">
	<?php
	if($rs){
//		if(count($rs) > 1){
	?>
		<tr>
			<td class="subTituloDireita" width="30%">Estado:</td>
			<td>
			<?php 
				$db->monta_combo('esdid', $rs, 'S', 'Selecione...', 'submit', '', ''); 
			?>
			</td>
		</tr>
	<?php 
//		}else{
//			$esdid = $rs[0]['codigo']; 		
//		}
	}
	
	//busca as responsabilidades do usuário
	$responsabilidades = pegaResponsabilidadePorPerfil();
	//possui responsabilidade
	if(is_array($responsabilidades)){
		foreach ($responsabilidades as $index=>$value){
			$index = $index == 'entid' ? 'entidunidade' : $index;
			$responsabilidadesTmp[] = "obr.{$index} IN ('".implode("','", $value)."')";
		}
			$responsabilidades = "(".implode(" OR ", $responsabilidadesTmp).")";
	}elseif($responsabilidades === true)
		$responsabilidades = 'TRUE'; //pode ver tudo
	else
		$responsabilidades = 'FALSE'; //não pode ver nada

	$sql = "
		SELECT distinct
			obr.obrid,
			upper(obr.obrdesc) AS obrdesc,
		    esd.esddsc,
			to_char(s.supvdt,'DD/MM/YYYY') as dtvistoria,
			to_char(s.supdtinclusao,'DD/MM/YYYY') as dtinclusao,						
			COALESCE(t.traseq || ' - ' || t.tradsc, '-') AS aditivo,
			u.usunome,
			si.stodesc,
			UPPER(e.entnome) as vistoriador,
			rs.rsudsc as responsavel,
			s.supvid,
			s.usucpf,
			e.entnumcpfcnpj as cpfvistoriador,
			s.supdtinclusao,
			coalesce((SELECT 
						sum(( icopercsobreobra * supvlrinfsupervisor ) / 100)
					  FROM 
						obras.itenscomposicaoobra i
					  INNER JOIN 
						obras.supervisaoitenscomposicao si ON i.icoid = si.icoid 
					  WHERE si.supvid = s.supvid 
						),'0') as percentual,
			doc.docid
			FROM
				obras.supervisao s
			LEFT JOIN(SELECT 
					DISTINCT
					t.traseq,
					t.tradsc,
					sic.supvid
				  FROM 
					obras.termoaditivo t
					JOIN obras.itenscomposicaoobra ico ON t.traid = ico.traid
					JOIN obras.supervisaoitenscomposicao sic ON sic.icoid = ico.icoid 
				 ) t ON t.supvid = s.supvid	
			INNER JOIN 
				obras.situacaoobra si ON si.stoid = s.stoid
			INNER JOIN
				seguranca.usuario u ON u.usucpf = s.usucpf
			LEFT JOIN
				entidade.entidade e ON e.entid = s.supvistoriador
			LEFT JOIN
				obras.realizacaosupervisao rs ON rs.rsuid = s.rsuid 
			INNER JOIN
				workflow.documento doc ON s.docid = doc.docid AND doc.esdid IN ('{$esdid}')
			INNER JOIN
				workflow.estadodocumento esd ON doc.esdid = esd.esdid
			INNER JOIN
				obras.obrainfraestrutura obr ON obr.obrid = s.obrid
			WHERE
				s.supstatus = 'A' AND {$responsabilidades}
			ORDER BY
			s.supdtinclusao ASC";
	$rs = $db->carregar($sql);

	if(empty($rs))
		$checkBox = false;
	
	
	if($checkBox):
	?>
		<tr>
			<td colspan="2">
				<input type="checkbox" id='cbxMarcarTodos' name='cbxMarcarTodos' onclick="marcarTodos($(this).attr('checked')); habilitaBotao();"/> <b>Selecionar todos</b>
			</td>
		</tr>
	<?php 
	endif;	
	?>
		<tr>
			<td colspan="2">
	<?php 
		$arCabecalho = array("Ação",
							 "ID",
							 "Obra",
							 "Estado Atual",
							 "Data Vistoria",
							 "Data Inclusão",
							 "Aditivo",
							 "Inserido Por",
							 "Situação da Obra",
							 "Vistoriador",
							 "Realizada Por",
							 "% da Vistoria");
		
		$acao = '<center>';
		if($checkBox)
			$acao.= '<input type="checkbox" id="docid_{docid}" name="docid[]" value="{docid}" onclick="habilitaBotao(); habilitaChecked();"/>';
		else 
			$acao.= '-';
		$acao.= '</center>';
		
		$arParamCol[0]  = array("html" => '<a href="obras.php?modulo=principal/inserir_vistoria&acao=A&obrid={obrid}&supvid={supvid}">{obrid}</a>');
		$arParamCol[1]  = array("html" => '<a href="obras.php?modulo=principal/inserir_vistoria&acao=A&obrid={obrid}&supvid={supvid}">{obrdesc}</a>');
		$arParamCol[10] = array("html" => '<php>
											$percentual = {percentual};
											$percentual = $percentual > 100.00 ? 100.00 : $percentual;
											return number_format($percentual,2,",",".") . " %";
										  </php>');
		// ARRAY de parametros de configuração da tabela
		$arConfig = array("style" => "width:100%;",
						  "totalLinha" => false,
						  "totalRegistro" => true);
		
		$oLista = new Lista($arConfig);
		$oLista->setCabecalho( $arCabecalho );
		$oLista->setCorpo( $rs, $arParamCol );
		$oLista->setAcao( $acao );
		$oLista->show();
		?>
			</td>
		</tr>
		<tr>
			<td class="subTituloDireita" align="right" colspan="2">
			<?php if($checkBox) :?>
				<input type="button" class="botao" value="Tramitar" id='btnTramitar' name='btnTramitar' onclick="tramitar()" disabled="disabled">
			<?php endif;?>
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script type="text/javascript">
function tramitar(){
	document.formulario.act.value = 'tramitar';
	document.formulario.submit();
}
function habilitaBotao(){
	var qtd = $("input[type=checkbox]:checked").length;
	if(qtd > 0)
		document.formulario.btnTramitar.disabled = '';
	else
		document.formulario.btnTramitar.disabled = 'disabled';
}
function habilitaChecked(){
	var qtd = $("input[type=checkbox]:not(:checked)").length;
	if(qtd == 1 && !$('#cbxMarcarTodos').attr('checked'))
		$('#cbxMarcarTodos').attr('checked', true);
	else
		$('#cbxMarcarTodos').attr('checked', false);
}
function marcarTodos(marcado){
	$("input[type=checkbox]").each(function(){
		this.checked = marcado;
	});
}
</script>