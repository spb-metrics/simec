<?php


function criar_arvore_atividade($atiid,$input='') {
	global $db, $arvore, $nivel;
	$sql = "SELECT * FROM projetos.atividade WHERE atiid = '".$atiid."'";
	$atividade = $db->pegaLinha($sql);
	if($atividade['atiidpai']) $input = criar_arvore_atividade($atividade['atiidpai'],$input)." <img src=../imagens/seta_filho.gif align=absmiddle> ".$atividade['_atinumero']." - ".$atividade['atidescricao']."<br>";;
	return $input;
}


function listaPendenciasAtv($atvid) {
	global $db, $arvore;
	$htmlarvore = criar_arvore_atividade($atvid);
	$html .= "<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center><tr><td>";
	$html .= $htmlarvore;
	$html .= "</td></tr></table>";
	$html .= "<table id=tbExecucao align=center bgcolor=#fcfcfc cellspacing=1 cellpadding=3 style=width:95%;border:1px solid black;>";
	$html .= "<thead><tr><th style=width:5%;>ID</th><th style=width:42%;>Item</th><th style=width:10%;>Prazo</th><th style=width:10%;>Fluxo</th><th style=width:10%;>Execução</th><th style=width:10%;>Validação</th><th style=width:10%;>Certificação</th><th style=width:3%;></th></tr></thead>";
	$html .= "<tbody style=height:400px;overflow-y:scroll;overflow-x:hidden;>";
				
	$sql = "SELECT icl.iclid,
				icl.icldsc,
				to_char(icl.iclprazo, 'dd/mm/YYYY') as iclprazo,
				icl.docid 
			FROM 
				projetos.itemchecklist icl 
			INNER JOIN 
				projetos.atividade atv ON atv.atiid = icl.atiid 
			WHERE 
				icl.atiid = ".$atvid."
			ORDER BY
				icl.iclordem";
	
	$listaChecklist = $db->carregar($sql);
				
	if( $listaChecklist ) {
		for($i=0; $i<count($listaChecklist); $i++) {
	
			$cor = ($i%2) ? "#e0e0e0" : "#f4f4f4";
						 
			$html .= "<tr><td bgcolor=".$cor.">".$listaChecklist[$i]['iclid']."</td><td bgcolor=".$cor.">".$listaChecklist[$i]['icldsc']."</td>";
			$html .= "<td bgcolor=".$cor.">".$listaChecklist[$i]['iclprazo']."</td>";
			$html .= "<td bgcolor=".$cor." align=center><img align=absmiddle style=cursor:pointer; src=../imagens/fluxodoc.gif onclick=\"window.open(\'../geral/workflow/historico.php?modulo=principal/tramitacao&acao=C&docid=".$listaChecklist[$i]['docid']."\', \'alterarEstado\',\'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no\');\" /></td>";
						
			$sql = "SELECT * FROM projetos.etapascontrole WHERE iclid = ".$listaChecklist[$i]['iclid']." AND tpvid = 1";
			$etapasControleExecucao = $db->carregar($sql);
				
			if( $etapasControleExecucao[0] ) {
				$sql = "SELECT * FROM projetos.validacao WHERE iclid = ".$listaChecklist[$i]['iclid']." AND tpvid = 1 AND vldsituacao IS NOT NULL";
				$situacaoExecucao = $db->pegaLinha($sql);
				if( $situacaoExecucao ) {
					if( $situacaoExecucao["vldsituacao"] == 't' )
						$img = "<img ".$class." src=../imagens/check_checklist.png border=0 style=cursor:pointer; onclick=\"window.open(\'enem.php?modulo=principal/atividade_enem/visaoTemporal&acao=A&requisicao=telaAcompanhamentoValidacao&vldid=".$situacaoExecucao['vldid']."\', \'acompanhar\',\'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no\');\" />";
					if( $situacaoExecucao["vldsituacao"] == 'f' )
						$img = "<img ".$class." src=../imagens/erro_checklist.png border=0 style=cursor:pointer; />";
				} else {
					$img = "<img src=../imagens/exclamacao_checklist.png border=0 style=cursor:pointer; />";
				}
			} else {
				$img = "-";
			}
						
			$html .= "<td bgcolor=".$cor." align=center id=item_".$atividades[$a]['atiid']."_".$listaChecklist[$i]['iclid']."_E>".$img."</td>";
				
			$sql = "SELECT * FROM projetos.etapascontrole WHERE iclid = ".$listaChecklist[$i]['iclid']." AND tpvid = 2";
			$etapasControleValidacao = $db->carregar($sql);
				
			if( $etapasControleValidacao[0] ) {
				$sql = "SELECT * FROM projetos.validacao WHERE iclid = ".$listaChecklist[$i]['iclid']." AND tpvid = 2 AND vldsituacao IS NOT NULL";
				$situacaoValidacao = $db->carregar($sql);

				if( $situacaoValidacao[0] ) {
					if( !$situacaoValidacao[0]["vldsituacao"] )
						$img = "<img src=../imagens/exclamacao_checklist.png border=0 style=cursor:pointer;width:30px;height:30px;  />";
					if( $situacaoValidacao[0]["vldsituacao"] == 't' )
						$img = "<img ".$class." src=../imagens/check_checklist.png border=0 style=cursor:pointer;width:30px;height:30px;  />";
					if( $situacaoValidacao[0]["vldsituacao"] == 'f' )
						$img = "<img ".$class." src=../imagens/erro_checklist.png border=0 style=cursor:pointer;width:30px;height:30px;  />";
				} else {
					$img = "<img src=../imagens/exclamacao_checklist.png border=0 style=cursor:pointer;width:30px;height:30px;  />";
				}
			} else {
				$img = "-";
			}
							
			$html .= "<td bgcolor=".$cor." align=center id=item_".$atividades[$a]['atiid']."_".$listaChecklist[$i]['iclid']."_V>".$img."</td>";

			$sql = "SELECT * FROM projetos.etapascontrole WHERE iclid = ".$listaChecklist[$i]['iclid']." AND tpvid = 3";
			$etapasControleCertificacao = $db->carregar($sql);
			
			if( $etapasControleCertificacao[0] ) {
				$sql = "SELECT * FROM projetos.validacao WHERE iclid = ".$listaChecklist[$i]['iclid']." AND tpvid = 3 AND vldsituacao IS NOT NULL";
				$situacaoCertificacao = $db->carregar($sql);
							
				if( $situacaoCertificacao[0] ) {
					if( !$situacaoCertificacao[0]["vldsituacao"] )
						$img = "<img src=../imagens/exclamacao_checklist.png border=0 style=cursor:pointer;width:30px;height:30px; />";
					if( $situacaoCertificacao[0]["vldsituacao"] == 't' )
						$img = "<img ".$class." src=../imagens/check_checklist.png border=0 style=cursor:pointer;width:30px;height:30px; />";
					if( $situacaoCertificacao[0]["vldsituacao"] == 'f' )
						$img = "<img ".$class." src=../imagens/erro_checklist.png border=0 style=cursor:pointer;width:30px;height:30px; />";
				} else {
					$img = "<img src=../imagens/exclamacao_checklist.png border=0 style=cursor:pointer;width:30px;height:30px; />";
				}
			} else {
				$img = "-";
			}
							
			$html .= "<td bgcolor=".$cor." align=center id=item_".$atividades[$a]['atiid']."_".$listaChecklist[$i]['iclid']."_C>".$img."</td></tr>";
		}
	} else {
		$html .= "<tr><td bgcolor=#f4f4f4 style=color:red;text-align:center; colspan=7>Não existe item(ns) de checklist cadastrado(s).</td></tr>";
	}
	$html .= "</tbody></table>";

	return $html;
	
}



if($_REQUEST['requisicao'] == 'telaAcompanhamentoValidacao') {
	
?>
<html>
	<head>
		<meta name="description" content="SIMEC - Sistema Integrado de Monitoramento Execução e Controle do Ministério da Educação, Permite o Monitoramento Físico e Financeiro e a Avaliação das Ações e Programas do Ministério dentre outras atividades estratégicas">
		<meta name="keywords" content="SIMEC, MEC, PDE, Ministério da Educação, Analistas: ,Cristiano Cabral, Adonias Malosso, Gilberto Xavier">
		<META NAME="Author" CONTENT="Cristiano Cabral, cristiano.cabral@gmail.com">
		<meta name="audience" content="all">
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">

		<meta http-equiv="Expires" content="-1">
		
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		 
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
	<body LEFTMARGIN="0" TOPMARGIN="0" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff">
	<?
	if($_REQUEST['vldid']) {
		
		$sql = "SELECT a.atidescricao, i.icldsc, to_char(i.iclprazo,'dd/mm/YYYY') as iclprazo, en.entnome, v.tpvid,
					   v.vldsituacao, to_char(v.vlddata, 'dd/mm/YYYY HH24:MI') as vlddata, v.vldobservacao, i.iclid 
					    FROM projetos.validacao v 
				INNER JOIN projetos.itemchecklist i ON i.iclid = v.iclid 
				INNER JOIN projetos.atividade a ON a.atiid = i.atiid 
				INNER JOIN entidade.entidade en ON en.entid = v.entid 
				WHERE vldid='".$_REQUEST['vldid']."'";
		
		$dados = $db->pegaLinha($sql);
		
		switch($dados['tpvid']) {
			case 1://execução
				$pessoa =  "Executor(a)";
				$situacao = "Executado";
				$titulo = "Execução";
				break;	
			case 2://validação
				$pessoa = "Validador(a)";
				$situacao = "Validado";
				$titulo = "Validação";
				break;
			case 3://certificação
				$pessoa = "Certificador(a)";
				$situacao = "Certificado";
				$titulo = "Certificação";
				break;
		}
		
	}
	?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloCentro" colspan="2">Dados da <? echo $titulo; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%">Atividade:</td>
			<td><? echo $dados['atidescricao']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%">Item:</td>
			<td><? echo $dados['icldsc']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%">Prazo:</td>
			<td><? echo $dados['iclprazo']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%">Data <? echo $titulo; ?>:</td>
			<td><? echo $dados['vlddata']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%"><? echo $pessoa; ?>:</td>
			<td><? echo $dados['entnome']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%">Situação:</td>
			<td><? echo (($dados['vldsituacao'] == 't')?$situacao:"Não ".$situacao); ?></td>
		</tr>

		<tr>
			<td class="SubTituloDireita" width="20%">Observações:</td>
			<td><? echo $dados['vldobservacao']; ?></td>
		</tr>
		<?
		$arrEtcopcaoevidencia = $db->pegaUm("SELECT etcopcaoevidencia FROM projetos.etapascontrole WHERE iclid='".$dados['iclid']."' AND tpvid = '".$dados['tpvid']."'");
		if($arrEtcopcaoevidencia == 't') {

			$sql = "SELECT to_char(v.vlddata,'dd/mm/YYYY HH24:MI') as vlddata, v.vldobservacao, t.tpvdsc, ar.arqid, usu.usunome FROM projetos.validacao v 
			LEFT JOIN projetos.anexochecklist a ON a.vldid = v.vldid 
			LEFT JOIN public.arquivo ar ON ar.arqid = a.arqid 
			LEFT JOIN seguranca.usuario usu ON usu.usucpf = ar.usucpf 
			LEFT JOIN projetos.tipovalidacao t ON t.tpvid = v.tpvid 
			WHERE v.iclid='".$dados['iclid']."' AND a.ancstatus = 'A'";
		
			$validacao = $db->carregar($sql);
			
			if($validacao[0]) {
			
				echo "<tr>";
				echo "<td colspan=2><table class=listagem width=100%>";
				echo "<thead>";
				echo "<tr><td align=center><b>Data</b></td><td align=center><b>Usuário</b></td><td align=center><b>Observação</b></td><td align=center><b>Fase</b></td><td align=center><b>Download</b></td></tr>";
				echo "</thead>";
							
				foreach($validacao as $val) {
					echo "<tr><td>".$val['vlddata']."</td><td>".$val['usunome']."</td><td>".$val['vldobservacao']."</td><td>".$val['tpvdsc']."</td><td align=center><img src=../imagens/salvar.png align=absmiddle border=0 style=cursor:pointer; onclick=\"window.location='enem.php?modulo=principal/atividade_enem/minhasPendencias&acao=A&requisicao=downloadArquivo&arqid=".$val['arqid']."';\"></td></tr>";
				}
					
				echo "</table></td>";
				echo "</tr>";
				
			}
			
		}
		?>
	</table>
	</body>
</html>
<?
exit;

}



if($_REQUEST['loadXml'] || $_REQUEST['loadJSON']) {
	if($_REQUEST['loadXml']){
		header('content-type:text/xml; charset=ISO-8859-1');
	}
	
	$arrMes = array("Jan","Fev","Mar","Abr","Mai","Jun","Jul","Ago","Set","Out","Nov","Dez");
	
	$sql = "select
			a.atiid,
			a.atidescricao,
			COALESCE(a.atidetalhamento,'N/A') as atidetalhamento,
			a._atiprofundidade as profundidade,
			a._atinumero as numero,
			a.atiporcentoexec,
			a.esaid,
			CASE WHEN a.atidatainicio IS NULL
				THEN ( select min(atidatainicio)  from projetos.atividade where atividade._atiprojeto = a._atiprojeto and atiidpai = a.atiid)
				ELSE a.atidatainicio
			END as atidatainicio,
			CASE WHEN a.atidatafim IS NULL
				THEN ( select max(atidatafim)  from projetos.atividade where atividade._atiprojeto = a._atiprojeto and atiidpai = a.atiid)
				ELSE a.atidatafim
			END as atidatafim 
		from projetos.atividade a 
		left join projetos.itemchecklist i on i.atiid = a.atiid 
		left join workflow.documento d on d.docid = i.docid
		where
			a.atistatus = 'A'
			and a._atiprofundidade < 10
			and a._atiprojeto = ".PROJETOENEM."
			".(($_REQUEST['iclcritico'])?"and iclcritico=TRUE":"")."
			".(($_REQUEST['iclpendente'])?"and d.esdid != ".ENEM_EST_EM_FINALIZADO:"")."
			".(($_REQUEST['iclfinalizada'])?"and a.esaid = 5":"")."
			".(($_REQUEST['dataini'] && $_REQUEST['datafim'])?"and i.iclprazo BETWEEN '".formata_data_sql($_REQUEST['dataini'])."' and '".formata_data_sql($_REQUEST['datafim'])."'":"")."
			".(($_REQUEST['_atinumero'])?"a._atinumero ILIKE '".$_REQUEST['_atinumero'].".%'":"")."
		group by 
			a.atiid,
			a.atidescricao,
			a.atidetalhamento,
			a._atiprofundidade,
			a._atinumero,
			a.atiporcentoexec,
			a.esaid,
			a.atidatainicio,
			a.atidatafim,
			a._atiprojeto,
			a._atiordem 
		order by
			a._atiordem";
	
	$arrDados = $db->carregar($sql);
	
	if($arrDados[0]){
		if($_REQUEST['loadXml']){
			$xml = "<data date-time-format=\"iso8601\" >";
			foreach($arrDados as $dado){
				if($dado['atidatainicio'] && $dado['atidatafim']){
					$xml.= "<event start='{$dado['atidatainicio']}' end='{$dado['atidatafim']}' title='{$dado['atidescricao']}' >";
					$xml.= " ".$dado['atidetalhamento']." ";
					$xml.= "Inicia em : ".date("d/m/Y",strtotime($dado['atidatainicio']))." ";
					$xml.= "Termina em: ".date("d/m/Y",strtotime($dado['atidatafim']))."";
					$xml.= "</event>";
				}
			}
			$xml.= "</data>";
		}else{
			$xml = "{";
				$xml.= " 'dateTimeFormat': 'iso8601', 'events' : [";
				foreach($arrDados as $dado){
					if($dado['atidatainicio'] && $dado['atidatafim']){
						$qtdrestricao = $db->pegaUm("SELECT COUNT(obsid) as qtdrestricao FROM projetos.observacaoatividade WHERE atiid='".$dado['atiid']."' AND obsstatus='A'");
						$xml.= "{'start': '{$dado['atidatainicio']}',";
						$xml.= "'end': '{$dado['atidatafim']}',";
						//$xml.= "'durationEvent': 'true',";
						$xml.= "'isDuration': 'true',";
						//$xml.= "'latestStart': '1935',";
						//$xml.= "'earliestEnd': 'true',";
						$xml.= "'icon': 'http://".$_SERVER['HTTP_HOST']."/includes/timeline/images/dull-blue-circle.png',";
						//$xml.= "'color': 'red',";
						//$xml.= "'textColor': 'green',";
						//$xml.= "'tapeImage': '/includes/timeline/images/dull-blue-circle.png',"; //desenhar a linha do tempo
						$xml.= "'caption': '{$dado['atidetalhamento']}',";
						//$xml.= "'classname': 'hot_event',";
						$xml.= "'title' : '".(($qtdrestricao > 0)?"<img src=../imagens/restricao.png border=0 align=absmiddle title=\'".$qtdrestricao." restrições\' />":"")." <font color=\"#888888\" />".date("d",strtotime($dado['atidatainicio'])). " ".$arrMes[date("n",strtotime($dado['atidatainicio']))-1]. " a ".date("d",strtotime($dado['atidatafim']))." ".$arrMes[date("n",strtotime($dado['atidatafim']))-1]." </font> - ".str_replace("'","\"",$dado['atidescricao'])."',";
						$html = listaPendenciasAtv($dado['atiid']);
						$xml.= "'description' : '{$html} <br />";							
						$xml.= "<span style=\"color:#CCCCCC\" >Inicia em : <b>".date("d/m/Y",strtotime($dado['atidatainicio']))."</b><br />";
						$xml.= "Termina em: <b>".date("d/m/Y",strtotime($dado['atidatafim']))."</b></span>";
						$xml.= "'},";
					}
				}
				$xml.= "]";
			$xml.= "}";
		$xml = str_replace(",]}","]}",$xml);
		}
	}
	echo $xml;
	die;
}

// CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba( $abacod_tela, $url, '' );

montar_titulo_projeto();

?>
<script language="javascript" type="text/javascript" >
Timeline_ajax_url="/includes/timeline/ajax/api/simile-ajax-api.js";
Timeline_urlPrefix='/includes/timeline/';    
</script>
<style type="text/css">
        .t-highlight1 { background-color: #ccf; }
        .p-highlight1 { background-color: #fcc; }

        .timeline-highlight-label-start .label_t-highlight1 { color: #f00; }
        .timeline-highlight-label-end .label_t-highlight1 { color: #aaf; }

        .timeline-band-events .important { color: #f00; }
        .timeline-band-events .small-important { background: #c00; }


        /*---------------------------------*/

        .dark-theme { color: #eee; }
        .dark-theme .timeline-band-0 .timeline-ether-bg { background-color: #333 }
        .dark-theme .timeline-band-1 .timeline-ether-bg { background-color: #111 }
        .dark-theme .timeline-band-2 .timeline-ether-bg { background-color: #222 }
        .dark-theme .timeline-band-3 .timeline-ether-bg { background-color: #444 }

        .dark-theme .t-highlight1 { background-color: #003; }
        .dark-theme .p-highlight1 { background-color: #300; }

        .dark-theme .timeline-highlight-label-start .label_t-highlight1 { color: #f00; }
        .dark-theme .timeline-highlight-label-end .label_t-highlight1 { color: #115; }

        .dark-theme .timeline-band-events .important { color: #c00; }
        .dark-theme .timeline-band-events .small-important { background: #c00; }

        .dark-theme .timeline-date-label-em { color: #fff; }
        .dark-theme .timeline-ether-lines { border-color: #555; border-style: solid; }
        .dark-theme .timeline-ether-highlight { background: #555; }

        .dark-theme .timeline-event-tape,
        .dark-theme .timeline-small-event-tape { background: #f60; }
        .dark-theme .timeline-ether-weekends { background: #111; }
    </style>

<script language="javascript" type="text/javascript" src="/includes/timeline/timeline-api.js"></script>
<script language="javascript" type="text/javascript" src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<!-- (INÍCIO) BIBLIOTECAS - PARA USO DOS COMPONENTES (CALENDÁRIO E SLIDER) -->
<script	language="javascript" type="text/javascript" src="../includes/blendtrans.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/_start.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/slider/slider.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" >
	$(function() {
		onLoad();
		onResize();	
	centerTimeline("<?php echo date("Y-m-d") ?>");
	});
	
	function verificarAtvFitros() {
	
		if(document.getElementById('atvcritica').checked == true) {
			critica="&iclcritico=1";
	    } else {
			critica="";
	    }
	    
		if(document.getElementById('atvpendente').checked == true) {
			pendente="&iclpendente=1";
	    } else {
			pendente="";
	    }

		if(document.getElementById('atvfinalizada').checked == true) {
			pendente="&iclfinalizada=1";
	    } else {
			pendente="";
	    }
	    
		if(document.getElementById('dataini').value != "" && document.getElementById('datafim').value != "") {
			prazo="&dataini="+document.getElementById('dataini').value+"&datafim="+document.getElementById('datafim').value;
	    } else {
			prazo="";
	    }
	    
	    if(document.getElementById('_atinumero').value != "") {
	    	etapa="&etapa="+document.getElementById('_atinumero').value;
	    } else {
	    	etapa="";
	    }
	    document.getElementById("jewish-controls").innerHTML="";
	    onLoad();
	}
	
	// variaveis do filtro
	var critica="";
	var pendente="";
	var finalizada="";
	var prazo="";
	var etapa="";
	// fim variaveis do filtro
	
	var tl;
	function onLoad(){
	
	var eventSource = new Timeline.DefaultEventSource();
	
	var theme = Timeline.ClassicTheme.create();
	theme.event.bubble.width = 600;
	theme.event.bubble.height = 600;
            
            var d = Timeline.DateTime.parseGregorianDateTime(2011);
            var bandInfos = [
                Timeline.createBandInfo({
                    width:          "80%", 
                    intervalUnit:   Timeline.DateTime.MONTH, 
                    intervalPixels: 200,
                    eventSource:    eventSource,
                    date:           d,
                    theme:          theme,
                    layout:         'original'  // original, overview, detailed
                }),
                Timeline.createBandInfo({
                    width:          "20%", 
                    intervalUnit:   Timeline.DateTime.YEAR, 
                    intervalPixels: 200,
                    eventSource:    eventSource,
                    date:           d,
                    theme:          theme,
                    layout:         'overview'  // original, overview, detailed
                })
            ];
            bandInfos[1].syncWith = 0;
            bandInfos[1].highlight = true;

            
            tl = Timeline.create(document.getElementById("tl"), bandInfos, Timeline.HORIZONTAL);

            tl.loadJSON("enem.php?modulo=principal/atividade_enem/visaoTemporal&acao=A&loadJSON=1"+critica+prazo+etapa+pendente+finalizada, function(json, url) {
                eventSource.loadJSON(json, url);
            });

             setupFilterHighlightControls(document.getElementById("jewish-controls"), tl, [0,1], theme);
            
        }
        function centerTimeline(data) {
            var d = data.split("-");
            if(navigator.appName == 'Microsoft Internet Explorer')
			{
				tl.getBand(0).setCenterVisibleDate(new Date( (d[0]*1) + 1, d[1], d[2]));
			}else{
				tl.getBand(0).setCenterVisibleDate(new Date(d[0], d[1], d[2]));
			}
        }
        
        var resizeTimerID = null;
        function onResize() {
            if (resizeTimerID == null) {
                resizeTimerID = window.setTimeout(function() {
                    resizeTimerID = null;
                    tl.layout();
                }, 500);
            }
        }
        
        function themeSwitch(){
          var timeline = document.getElementById('tl');		
          timeline.className = (timeline.className.indexOf('dark-theme') != -1) ? timeline.className.replace('dark-theme', '') : timeline.className += ' dark-theme';
        }
        
        function centerSimileAjax(date) {
   	tl.getBand(0).setCenterVisibleDate(SimileAjax.DateTime.parseGregorianDateTime(date));
}

function setupFilterHighlightControls(div, timeline, bandIndices, theme) {
    var table = document.createElement("table");
    var tr = table.insertRow(0);
    
    var td = tr.insertCell(0);
    td.innerHTML = "Filtro:";
    
    td = tr.insertCell(1);
    td.innerHTML = "Marcar:";
    
    var handler = function(elmt, evt, target) {
        onKeyPress(timeline, bandIndices, table);
    };
    
    tr = table.insertRow(1);
    tr.style.verticalAlign = "top";
    
    td = tr.insertCell(0);
    
    var input = document.createElement("input");
    input.type = "text";
    SimileAjax.DOM.registerEvent(input, "keypress", handler);
    td.appendChild(input);
    
    for (var i = 0; i < theme.event.highlightColors.length; i++) {
        td = tr.insertCell(i + 1);
        
        input = document.createElement("input");
        input.type = "text";
        SimileAjax.DOM.registerEvent(input, "keypress", handler);
        td.appendChild(input);
        
        var divColor = document.createElement("div");
        divColor.style.height = "0.5em";
        divColor.style.background = theme.event.highlightColors[i];
        td.appendChild(divColor);
    }
    
    td = tr.insertCell(tr.cells.length);
    var button = document.createElement("button");
    button.innerHTML = "Limpar Filtros";
    SimileAjax.DOM.registerEvent(button, "click", function() {
        clearAll(timeline, bandIndices, table);
    });
    td.appendChild(button);
    
    td = tr.insertCell(tr.cells.length);
    var button = document.createElement("button");
    button.innerHTML = "Alterar Tema";
    SimileAjax.DOM.registerEvent(button, "click", function() {
       themeSwitch();
    });
    td.appendChild(button);
    
    div.appendChild(table);
}

var timerID = null;
function onKeyPress(timeline, bandIndices, table) {
    if (timerID != null) {
        window.clearTimeout(timerID);
    }
    timerID = window.setTimeout(function() {
        performFiltering(timeline, bandIndices, table);
    }, 300);
}
function cleanString(s) {
    return s.replace(/^\s+/, '').replace(/\s+$/, '');
}
function performFiltering(timeline, bandIndices, table) {
    timerID = null;
    
    var tr = table.rows[1];
    var text = cleanString(tr.cells[0].firstChild.value);
    
    var filterMatcher = null;
    if (text.length > 0) {
        var regex = new RegExp(text, "i");
        filterMatcher = function(evt) {
            return regex.test(evt.getText()) || regex.test(evt.getDescription());
        };
    }
    
    var regexes = [];
    var hasHighlights = false;
    for (var x = 1; x < tr.cells.length - 1; x++) {
        var input = tr.cells[x].firstChild;
        var text2 = cleanString(input.value);
        if (text2.length > 0) {
            hasHighlights = true;
            regexes.push(new RegExp(text2, "i"));
        } else {
            regexes.push(null);
        }
    }
    var highlightMatcher = hasHighlights ? function(evt) {
        var text = evt.getText();
        var description = evt.getDescription();
        for (var x = 0; x < regexes.length; x++) {
            var regex = regexes[x];
            if (regex != null && (regex.test(text) || regex.test(description))) {
                return x;
            }
        }
        return -1;
    } : null;
    
    for (var i = 0; i < bandIndices.length; i++) {
        var bandIndex = bandIndices[i];
        timeline.getBand(bandIndex).getEventPainter().setFilterMatcher(filterMatcher);
        timeline.getBand(bandIndex).getEventPainter().setHighlightMatcher(highlightMatcher);
    }
    timeline.paint();
}
function clearAll(timeline, bandIndices, table) {
    var tr = table.rows[1];
    for (var x = 0; x < tr.cells.length - 1; x++) {
        tr.cells[x].firstChild.value = "";
    }
    
    for (var i = 0; i < bandIndices.length; i++) {
        var bandIndex = bandIndices[i];
        timeline.getBand(bandIndex).getEventPainter().setFilterMatcher(null);
        timeline.getBand(bandIndex).getEventPainter().setHighlightMatcher(null);
    }
    timeline.paint();
}

    </script>
         <div style="width: 100%">
         <table class="tabela" bgcolor="#f5f5f5" cellspacing="0" cellpadding="0" align="center">
			  <tr>
				  <td class="SubTituloEsquerda" width="13%"><input type="checkbox" name="atvcritica" id="atvcritica" value="sim"> Atividades críticas</td>
				  <td class="SubTituloEsquerda" width="13%"><input type="checkbox" name="atvpendente" id="atvpendente" value="sim"> Atividades pendentes</td>
				  <td class="SubTituloEsquerda" width="13%"><input type="checkbox" name="atvfinalizada" id="atvfinalizada" value="sim"> Atividades concluídas</td>
				  <td class="SubTituloEsquerda" width="26%">Prazos de <? echo campo_data2('dataini','N', 'S', 'Data de inicío', 'S' ); ?> até <? echo campo_data2('datafim','N', 'S', 'Data final', 'S' ); ?></td>
				  <td class="SubTituloEsquerda" width="26%">Etapas : <? 
				  	$sql = "SELECT _atinumero as codigo, atidescricao as descricao from projetos.atividade where atiidpai = '".PROJETOENEM."' AND atistatus='A' ORDER BY atiordem";
					$db->monta_combo('_atinumero', $sql, 'S', 'Selecione', '', '', '', '300', 'N', '_atinumero');
				   ?></td>
				  <td class="SubTituloEsquerda" width="9%"><input type="button" name="filtrar" value="Filtrar" onclick="verificarAtvFitros();"></td>
			  </tr>
		  </table>

          <table style="text-align: center; width: 100%">
              <tr>
                  <td><a href="javascript:centerTimeline('2011-01-01');">Jan/2011</a></td>
                  <td><a href="javascript:centerTimeline('2011-02-01');">Fev/2011</a></td>
                  <td><a href="javascript:centerTimeline('2011-03-01');">Mar/2011</a></td>
                  <td><a href="javascript:centerTimeline('2011-04-01');">Abr/2011</a></td>
                  <td><a href="javascript:centerTimeline('2011-05-01');">Mai/2011</a></td>
                  <td><a href="javascript:centerTimeline('2011-06-01');">Jun/2011</a></td>
                  <td><a href="javascript:centerTimeline('2011-07-01');">Jul/2011</a></td>
                  <td><a href="javascript:centerTimeline('2011-08-01');">Ago/2011</a></td>
                  <td><a href="javascript:centerTimeline('2011-09-01');">Set/2011</a></td>
                  <td><a href="javascript:centerTimeline('2011-10-01');">Out/2011</a></td>
                  <td><a href="javascript:centerTimeline('2011-11-01');">Nov/2011</a></td>
                  <td><a href="javascript:centerTimeline('2011-12-01');">Dez/2011</a></td>
              </tr>
          </table>
      </div>
   
<table class="tabela" bgcolor="#f5f5f5" cellspacing="0" cellpadding="10" align="center">
	<tr>
		<td align="center">
			<div id="tl" class="timeline-default" style="height: 550px"></div>
			<div class="controls" id="jewish-controls"></div>
		</td>
	</tr>
</table>