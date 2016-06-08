<?php

//include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php'; 
//include APPRAIZ . 'www/obras/_funcoes.php';

$rstoid = "";
$obras = new Obras();

$dobras    = new DadosObra(null);
$restricao = new DadosRestricao();

if($_REQUEST["rstoid"]){
	$requisicao = "atualizar";
	$resultado = $restricao->busca($_REQUEST["rstoid"]);	
	$dados     = $restricao->dados($resultado);
}else{
	$requisicao = "cadastrar";
}

if ($_REQUEST["subimete"]){
	switch ($_REQUEST["requisicao"]){
		case "cadastrar" :
			$obras->CadastrarRestricao($_REQUEST);
		break;	
		case "atualizar" :
			$obras->AtualizarRestricaoObra($_REQUEST);
		break;
	}
}

?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script src="./js/restricao.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
<script language="JavaScript">
	//Editor de textos
	tinyMCE.init({
		mode : "textareas",
		theme : "advanced",
		plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
		theme_advanced_buttons1 : "undo,redo,separator,bold,italic,underline,separator,justifyleft,justifycenter,justifyright, justifyfull",
		theme_advanced_buttons2 : "",
		theme_advanced_buttons3 : "",
		theme_advanced_toolbar_location : "top",
		theme_advanced_toolbar_align : "left",
		extended_valid_elements : "a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
		language : "pt_br",
		entity_encoding : "raw"
		});
</script>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<br/>
<?php
	monta_titulo( 'Restrições e Providências da Obra', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  );
?>
<br/>
<form method="post" id="formulario" name="formulario" onsubmit="return validar(<?php echo $_REQUEST["rstoid"]; ?>);" action="<?php echo $caminho_atual.'acao=A'?>">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloDireita" align="right">Situação da Obra na Restrição:</td>
			<td>
			<?php
				$fsrid = $restricao->fsrid;
				$sql = "SELECT
							fsrid 	as codigo,
							fsrdsc as descricao
						FROM
							obras.faserestricao
						";
					
				$db->monta_combo( 'fsrid', $sql, $somenteLeitura, 'Selecione', '', '', '', '115', 'S', 'fsrid');   
				unset($sql);
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" align="right">Tipo de Restrição:</td>
			<td>
			<?php
				$trtid = $restricao->trtid;
				$sql = "SELECT
							trtid 	as codigo,
							trtdesc as descricao
						FROM
							obras.tiporestricao
						ORDER BY
							trtdesc";
					
				$db->monta_combo( 'trtid', $sql, $somenteLeitura, 'Selecione', '', '', '', '', 'S', 'trtid');   
				unset($sql);
			?>
			</td>
		</tr>
	    <tr>
	        <td align='right' class="SubTituloDireita">Restrição:</td>
	        <td>
	        <?php $rstdesc = $restricao->rstdesc; ?>
			<?=campo_textarea("rstdesc",'N',$somenteLeitura,'',78,8,'');?>
		    </td>
	    </tr>
	    <tr>
	        <td align='right' class="SubTituloDireita">Previsão da Providência:</td>
	        <td>
	        <?php $rstdtprevisaoregularizacao = formata_data($restricao->rstdtprevisaoregularizacao); ?>
		    <?= campo_data2( 'rstdtprevisaoregularizacao', 'N', $somenteLeitura, '', 'N' ); ?>
		    </td>
	    </tr>    
	    <tr>
	        <td align='right' class="SubTituloDireita">Providência:</td>
	        <td>
	        <?php $rstdescprovidencia = $restricao->rstdescprovidencia; ?>
			<?=campo_textarea("rstdescprovidencia", 'N', $somenteLeitura, '', 78, 8, '');?>
		    </td>
	    </tr>
	    
	    <?php if ($_REQUEST["rstoid"]){?>
	    
	    <tr>
	    	<td align='right' class="SubTituloDireita">Restrição superada?</td>
	    	<td colspan="2">
	    		<?php 
	    			$rstsituacao = $restricao->rstsituacao;
	    			if ($rstsituacao == "t"){
	    		?>
	    		<input type="radio" name="rstsituacao" id="rstsituacao" value="true" checked> Sim
	    		<input type="radio" name="rstsituacao" id="rstsituacao" value="false" onclick="document.formulario.rstdtsuperacao.value = '';"> Não
	    		<?php } else{ ?>
	    		<input type="radio" name="rstsituacao" id="rstsituacao" value="true"> Sim
	    		<input type="radio" name="rstsituacao" id="rstsituacao" value="false" onclick="document.formulario.rstdtsuperacao.value = '';" checked> Não
	    		<?php } ?>
	    	 	&nbsp Se <b>sim</b>, entre com a data: 
	    	 	<?php $rstdtsuperacao = formata_data($restricao->rstdtsuperacao); ?>
	    	 	<?= campo_data2( 'rstdtsuperacao', 'N', 'S', '', 'N' ); ?>
	    	 	<input type="hidden" name="rstoid" id="rstoid" value="<? echo $_REQUEST["rstoid"]; ?>">
	    	 </td>
	    </tr>
		
		<?php }?>
		
		<tr bgcolor="#C0C0C0">
			<td>&nbsp;</td>
			<td>
				<input type="hidden" name="requisicao" value="<?php echo $requisicao; ?>"/>
				<input type="hidden" name="subimete" value='2'/>
				<?php if($habilitado){ ?>
					<input type='submit' class='botao' name='Salvar' value='Salvar' />
				<?php } ?>
				<input type='button' class='botao' value='Voltar' id='btFechar' name='btFechar' onclick='window.opener.location.replace(window.opener.location); window.close();'/>
			</td>			
		</tr>        		
	</table>
</form>
</body>