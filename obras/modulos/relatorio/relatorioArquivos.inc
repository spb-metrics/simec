<?php

include APPRAIZ . "includes/cabecalho.inc";
include APPRAIZ . 'includes/Agrupador.php';
echo'<br>';
$db->cria_aba($abacod_tela,$url,'');
monta_titulo( $titulo_modulo, '' );

?>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript">

	jQuery.noConflict();
	
	function obras_exibeRelatorioGeral(tipo){
		
		var formulario = document.filtro;
		var agrupador = $( 'colunas' );
		var visualizar = $( 'visualizar' );
		//visualizar.disabled = true;
		// Tipo de relatorio
		formulario.pesquisa.value='1';

		prepara_formulario();
		selectAllOptions( formulario.colunas );
		
		if( tipo == 'exibir' ){

			if ( !agrupador.options.length ){
				alert( 'Favor selecionar ao menos uma agrupador!' );
				return false;
			}
			
//			// Agrupador 
//			selectAllOptions( agrupador );
//			var colunas = "";
//			var selObj = $('colunas');
//			var i;
//			for (i=0; i<selObj.options.length; i++) {
//				if (selObj.options[i].selected) {
//					colunas +="{"+selObj.options[i].value+"}";
//			  	}
//			}
//			
//			// Usuario 
//			var selObj = $('usucpf');
//			selectAllOptions( selObj );
//			var usucpf = "";
//			var i;
//			for (i=0; i<selObj.options.length; i++) {
//				if ( selObj.options[i].value != "" ) {
//					usucpf +="{\'"+selObj.options[i].value+"\'}";
//			  	}
//			}
//			// Obrid 
//			var obridid = document.getElementById('obridid').value;
//			// Unidade 
//			var selObj = $('entid');
//			selectAllOptions( selObj );
//			var entid = "";
//			var i;
//			for (i=0; i<selObj.options.length; i++) {
//				if ( selObj.options[i].value != "" ) {
//					entid +="{\'"+selObj.options[i].value+"\'}";
//			  	}
//			}
//			// Unidade 
//			var selObj = $('entid');
//			selectAllOptions( selObj );
//			var entid = "";
//			var i;
//			for (i=0; i<selObj.options.length; i++) {
//				if ( selObj.options[i].value != "" ) {
//					entid +="{\'"+selObj.options[i].value+"\'}";
//			  	}
//			}
//			// Obras 
//			var selObj = $('obrid');
//			selectAllOptions( selObj );
//			var obrid = "";
//			var i;
//			for (i=0; i<selObj.options.length; i++) {
//				if ( selObj.options[i].value != "" ) {
//					obrid +="{\'"+selObj.options[i].value+"\'}";
//			  	}
//			}
//			// UF 
//			var selObj = $('estuf');
//			selectAllOptions( selObj );
//			var estuf = "";
//			var i;
//			for (i=0; i<selObj.options.length; i++) {
//				if ( selObj.options[i].value != "" ) {
//					estuf +="{\'"+selObj.options[i].value+"\'}";
//			  	}
//			}
//			// Municipio 
//			var selObj = $('muncod');
//			selectAllOptions( selObj );
//			var muncod = "";
//			var i;
//			for (i=0; i<selObj.options.length; i++) {
//				if ( selObj.options[i].value != "" ) {
//					muncod +="{\'"+selObj.options[i].value+"\'}";
//			  	}
//			}
//			// Convênio 
//			var selObj = $('numconvenio');
//			selectAllOptions( selObj );
//			var numconvenio = "";
//			var i;
//			for (i=0; i<selObj.options.length; i++) {
//				if ( selObj.options[i].value != "" ) {
//					numconvenio +="{\'"+selObj.options[i].value+"\'}";
//			  	}
//			}
//			// Situação 
//			var selObj = $('stoid');
//			selectAllOptions( selObj );
//			var stoid = "";
//			var i;
//			for (i=0; i<selObj.options.length; i++) {
//				if ( selObj.options[i].value != "" ) {
//					stoid +="{\'"+selObj.options[i].value+"\'}";
//			  	}
//			}
//			// Orgão 
//			var selObj = $('orgid');
//			selectAllOptions( selObj );
//			var orgid = "";
//			var i;
//			for (i=0; i<selObj.options.length; i++) {
//				if ( selObj.options[i].value != "" ) {
//					orgid +="{\'"+selObj.options[i].value+"\'}";
//			  	}
//			}
			
			var notusucpf		= $('usucpf_campo_excludente').checked;
			var notentid		= $('entid_campo_excludente').checked;
			var notobrid		= $('obrid_campo_excludente').checked;
			var notestuf		= $('estuf_campo_excludente').checked;
			var notmuncod		= $('muncod_campo_excludente').checked;
			var notnumconvenio  = $('numconvenio_campo_excludente').checked;
			var notstoid		= $('stoid_campo_excludente').checked;
			var notorgid		= $('orgid_campo_excludente').checked;
					
			var div = $('div_resposta');

			formulario.target = 'obras_arquivos_supervisao';
			var janela = window.open( '', 'obras_arquivos_supervisao', 'width=900,height=645,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
			formulario.submit();
			janela.focus();

			
//			var myAjax = new Ajax.Request(
//					'/obras/obras.php?modulo=relatorio/relatorioArquivos_resultado&acao=A',
//		    		{
//		    			method: 'post',
//		    			parameters: '&notorgid='+notorgid+'&notstoid='+notstoid+'&notnumconvenio='+notnumconvenio+'&notmuncod='+notmuncod+'&notestuf='+notestuf+'&notobrid='+notobrid+'&notentid='+notentid+'&notusucpf='+notusucpf+'&colunas='+colunas+'&orgid='+orgid+'&stoid='+stoid+'&numconvenio='+numconvenio+'&muncod='+muncod+'&estuf='+estuf+'&obrid='+obrid+'&entid='+entid+'&obridid='+obridid+'&usucpf='+usucpf,
//		    			asynchronous: false,
//		    			onComplete: function(resp) {
//							$('tr_resposta').style.display = 'table-row';
//		    				extrairScript(resp.responseText);
//		    				div.innerHTML = resp.responseText;
//		    				visualizar.disabled = false;
//		    			}
//		    		});
			
			
		}
		
	}

	function onOffCampo( campo ) {
		var div_on  = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input   = document.getElementById( campo + '_campo_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '0';
		}
	}

	function Mascara_Hora(Hora){ 
		var hora01 = ''; 
		hora01 = hora01 + Hora; 
		if (hora01.length == 2){ 
			hora01 = hora01 + ':'; 
			document.forms[0].Hora.value = hora01; 
		} 
		if (hora01.length == 5){ 
			Verifica_Hora(); 
		} 
	} 
		           
	function Verifica_Hora( hora ){ 
		if( hora.value.length > 3 ){
			hrs = (hora.value.substring(0,2)); 
			min = (hora.value.substring(3,5)); 
		}else{
			hrs = 0; 
			min = (hora.value.substring(hora.value.length-2,hora.value.length)); 
		}
		               
		estado = ""; 
		if ((hrs < 00 ) || (hrs > 23) || ( min < 00) ||( min > 59)){ 
			estado = "errada"; 
		} 
		               
		if (hora.value == "") { 
			estado = ""; 
		} 
	
		if (estado == "errada") { 
			hora.value = "";
			alert("Hora inválida!"); 
			hora.focus(); 
		} 
	} 
			

</script>
<form action="/obras/obras.php?modulo=relatorio/relatorioArquivos_resultado&acao=A" method="post" name="filtro" id="filtro"> 
	<input type="hidden" name="form" value="1"/>
	<input type="hidden" name="pesquisa" value="1"/>
	<input type="hidden" name="publico" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado -->
	<input type="hidden" name="prtid" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
	<input type="hidden" name="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="SubTituloDireita" width="10%">Agrupadores</td>
			<td>
				<?php
					// Início dos agrupadores
					$agrupador = new Agrupador('filtro','');
					
					// Dados padrão de destino (nulo)
					$destino = isset( $agrupador2 ) ? $agrupador2 : array();
					
					// Dados padrão de origem
					$origem = array(
						'usunome' => array(
													'codigo'    => 'usunome',
													'descricao' => '01. Nome do usuário que inseriu'
						),
//						'obrid' => array(
//													'codigo'    => 'obrid',
//													'descricao' => '02. ID da Obra'
//						),
						'entnome' => array(
													'codigo'    => 'entnome',
													'descricao' => '02. Unidade ResponsÃ¡vel pela Obra'
						),
						'obrdesc' => array(
													'codigo'    => 'obrdesc',
													'descricao' => '03. Nome da Obra'
						),
						'estuf' => array(
													'codigo'    => 'estuf',
													'descricao' => '04. UF'
						),
						'mundescricao' => array(
													'codigo'    => 'mundescricao',
													'descricao' => '05. Município'
						),
						'numconvenio' => array(
													'codigo'    => 'numconvenio',
													'descricao' => '06. Convênio'
						),
						'arqdata' => array(
													'codigo'    => 'arqdata',
													'descricao' => '07. Data da inclusão (arquivo)'
						),
						'stoid' => array(
													'codigo'    => 'stodesc',
													'descricao' => '08. Situação da Obra'
						),
						'orgid' => array(
													'codigo'    => 'orgdesc',
													'descricao' => '09. Orgão da obra'
						),
						'arqnome' => array(
													'codigo'    => 'arqnome',
													'descricao' => '10. Dados do Arquivo'
						)
					);
					
					// exibe agrupador
					$agrupador->setOrigem( 'naoColunas', null, $origem );
					$agrupador->setDestino( 'colunas', null, $destino );
					$agrupador->exibir();
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloesquerda" colspan="2">
				<strong>Filtros</strong>
			</td>
		</tr>
			<?php
				//Usuario
				$sql = "SELECT 
							u.usucpf as codigo,
							u.usunome as descricao
						FROM 
							obras.arquivosobra f 
						INNER JOIN obras.tipoarquivo 	   ta ON ta.tpaid = f.tpaid 
						INNER JOIN public.arquivo 		    a ON a.arqid=f.arqid 
						INNER JOIN seguranca.usuario 		u ON u.usucpf = a.usucpf 
						INNER JOIN obras.obrainfraestrutura o ON o.obrid = f.obrid 
						LEFT  JOIN entidade.entidade      ent ON ent.entid = o.entidunidade 
						INNER JOIN obras.situacaoobra      so ON so.stoid = o.stoid 
						INNER JOIN entidade.endereco        e ON e.endid = o.endid 
						INNER JOIN territorios.municipio    m ON m.muncod = e.muncod 
						WHERE 
							a.arqid/1000 BETWEEN 647 
							AND 725 
							AND a.arqid NOT IN(select arqid FROM public.arquivo_recuperado) 
							AND aqostatus='A' AND sisid=15  AND obsstatus = 'A' 
						ORDER BY 
							u.usunome";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Nome do usuário que inseriu', 'usucpf',  $sql, '', 'Selecione o(s) Usuário(s)' );

				//Unidade
				$sql = "SELECT 
							ent.entid as codigo,
							ent.entnome as descricao
						FROM 
							obras.arquivosobra f 
						INNER JOIN obras.tipoarquivo 	   ta ON ta.tpaid = f.tpaid 
						INNER JOIN public.arquivo 		    a ON a.arqid=f.arqid 
						INNER JOIN seguranca.usuario 		u ON u.usucpf = a.usucpf 
						INNER JOIN obras.obrainfraestrutura o ON o.obrid = f.obrid 
						LEFT  JOIN entidade.entidade      ent ON ent.entid = o.entidunidade 
						INNER JOIN obras.situacaoobra      so ON so.stoid = o.stoid 
						INNER JOIN entidade.endereco        e ON e.endid = o.endid 
						INNER JOIN territorios.municipio    m ON m.muncod = e.muncod 
						WHERE 
							a.arqid/1000 BETWEEN 647 
							AND 725 
							AND a.arqid NOT IN(select arqid FROM public.arquivo_recuperado) 
							AND aqostatus='A' AND sisid=15  AND obsstatus = 'A' 
						ORDER BY 
							u.usunome";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Unidade ResponsÃ¡vel pela Obra', 'entid',  $sql, '', 'Selecione a(s) Unidade(s)' );
				
    			// Nome Obra
				$sql = "SELECT 
							o.obrid as codigo,
							o.obrdesc as descricao
						FROM 
							obras.arquivosobra f 
						INNER JOIN obras.tipoarquivo 	   ta ON ta.tpaid = f.tpaid 
						INNER JOIN public.arquivo 		    a ON a.arqid=f.arqid 
						INNER JOIN seguranca.usuario 		u ON u.usucpf = a.usucpf 
						INNER JOIN obras.obrainfraestrutura o ON o.obrid = f.obrid 
						LEFT  JOIN entidade.entidade      ent ON ent.entid = o.entidunidade 
						INNER JOIN obras.situacaoobra      so ON so.stoid = o.stoid 
						INNER JOIN entidade.endereco        e ON e.endid = o.endid 
						INNER JOIN territorios.municipio    m ON m.muncod = e.muncod 
						WHERE 
							a.arqid/1000 BETWEEN 647 
							AND 725 
							AND a.arqid NOT IN(select arqid FROM public.arquivo_recuperado) 
							AND aqostatus='A' AND sisid=15  AND obsstatus = 'A' 
						ORDER BY 
							u.usunome";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Obras', 'obrid',  $sql, '', 'Selecione a(s) Obras(s)' );
				
    			// UF
				$sql = "SELECT 
							m.estuf as codigo,
							m.estuf as descricao
						FROM 
							obras.arquivosobra f 
						INNER JOIN obras.tipoarquivo 	   ta ON ta.tpaid = f.tpaid 
						INNER JOIN public.arquivo 		    a ON a.arqid=f.arqid 
						INNER JOIN seguranca.usuario 		u ON u.usucpf = a.usucpf 
						INNER JOIN obras.obrainfraestrutura o ON o.obrid = f.obrid 
						LEFT  JOIN entidade.entidade      ent ON ent.entid = o.entidunidade 
						INNER JOIN obras.situacaoobra      so ON so.stoid = o.stoid 
						INNER JOIN entidade.endereco        e ON e.endid = o.endid 
						INNER JOIN territorios.municipio    m ON m.muncod = e.muncod 
						WHERE 
							a.arqid/1000 BETWEEN 647 
							AND 725 
							AND a.arqid NOT IN(select arqid FROM public.arquivo_recuperado) 
							AND aqostatus='A' AND sisid=15  AND obsstatus = 'A' 
						ORDER BY 
							u.usunome";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Estados', 'estuf',  $sql, '', 'Selecione a(s) Estado(s)' );
				
    			// Municipios
				$sql = "SELECT 
							m.muncod as codigo,
							replace(m.mundescricao, '\'', '') as descricao
						FROM 
							obras.arquivosobra f 
						INNER JOIN obras.tipoarquivo 	   ta ON ta.tpaid = f.tpaid 
						INNER JOIN public.arquivo 		    a ON a.arqid=f.arqid 
						INNER JOIN seguranca.usuario 		u ON u.usucpf = a.usucpf 
						INNER JOIN obras.obrainfraestrutura o ON o.obrid = f.obrid 
						LEFT  JOIN entidade.entidade      ent ON ent.entid = o.entidunidade 
						INNER JOIN obras.situacaoobra      so ON so.stoid = o.stoid 
						INNER JOIN entidade.endereco        e ON e.endid = o.endid 
						INNER JOIN territorios.municipio    m ON m.muncod = e.muncod 
						WHERE 
							a.arqid/1000 BETWEEN 647 
							AND 725 
							AND a.arqid NOT IN(select arqid FROM public.arquivo_recuperado) 
							AND aqostatus='A' AND sisid=15  AND obsstatus = 'A' 
						ORDER BY 
							u.usunome";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Municípios', 'muncod',  $sql, '', 'Selecione o(s) Municipio(s)' );
				
    			// Convênio
				$sql = "SELECT 
							o.numconvenio as codigo,
							o.numconvenio as descricao
						FROM 
							obras.arquivosobra f 
						INNER JOIN obras.tipoarquivo 	   ta ON ta.tpaid = f.tpaid 
						INNER JOIN public.arquivo 		    a ON a.arqid=f.arqid 
						INNER JOIN seguranca.usuario 		u ON u.usucpf = a.usucpf 
						INNER JOIN obras.obrainfraestrutura o ON o.obrid = f.obrid 
						LEFT  JOIN entidade.entidade      ent ON ent.entid = o.entidunidade 
						INNER JOIN obras.situacaoobra      so ON so.stoid = o.stoid 
						INNER JOIN entidade.endereco        e ON e.endid = o.endid 
						INNER JOIN territorios.municipio    m ON m.muncod = e.muncod 
						WHERE 
							a.arqid/1000 BETWEEN 647 
							AND 725 
							AND a.arqid NOT IN(select arqid FROM public.arquivo_recuperado) 
							AND aqostatus='A' AND sisid=15  AND obsstatus = 'A' 
						ORDER BY 
							u.usunome";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Convênio', 'numconvenio',  $sql, '', 'Selecione o(s) Convênio(s)' );
				
    			// Situação
				$sql = "SELECT 
							so.stoid as codigo,
							so.stodesc as descricao
						FROM 
							obras.arquivosobra f 
						INNER JOIN obras.tipoarquivo 	   ta ON ta.tpaid = f.tpaid 
						INNER JOIN public.arquivo 		    a ON a.arqid=f.arqid 
						INNER JOIN seguranca.usuario 		u ON u.usucpf = a.usucpf 
						INNER JOIN obras.obrainfraestrutura o ON o.obrid = f.obrid 
						LEFT  JOIN entidade.entidade      ent ON ent.entid = o.entidunidade 
						INNER JOIN obras.situacaoobra      so ON so.stoid = o.stoid 
						INNER JOIN entidade.endereco        e ON e.endid = o.endid 
						INNER JOIN territorios.municipio    m ON m.muncod = e.muncod 
						WHERE 
							a.arqid/1000 BETWEEN 647 
							AND 725 
							AND a.arqid NOT IN(select arqid FROM public.arquivo_recuperado) 
							AND aqostatus='A' AND sisid=15  AND obsstatus = 'A' 
						ORDER BY 
							u.usunome";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Situação da Obra', 'stoid',  $sql, '', 'Selecione a(s) Situação(ões)' );
				
    			// Orgão
				$sql = " SELECT 
							orgid as codigo, 
							orgdesc as descricao 
						 FROM 
							obras.orgao";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Orgão da obra', 'orgid',  $sql, '', 'Selecione o(s) Orgão(s)' );

				//RESPONSAVEL SUPERVISAO
				$sql = "select rsuid as codigo, 
				   			   rsudsc as descricao  
						from obras.realizacaosupervisao";
				
				$stSqlCarregados = "";
				mostrarComboPopup( 'Responsável pelo documento', 'rsuid',  $sql, '', 'Selecione o(s)' );
		?>
		<tr>
			<td bgcolor="#CCCCCC" width="10%"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" id="visualizar" value="Visualizar" onclick="obras_exibeRelatorioGeral('exibir');" style="cursor: pointer;"/>
			</td>
		</tr>
		<tr id="tr_resposta" >
			
			<td colspan="2" style="background-color: white;">
				<div id="div_resposta">
				</div>
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript">
<!--

jQuery(document).ready(function(){

	jQuery('#esdid').click(function(){
		
		var div_on    = document.getElementById( 'esdid_campo_on' );
		
		if(div_on.style.display == 'block'){
			
			jQuery('#filtro_periodo').show();
		}else{
			
			jQuery('#filtro_periodo').hide();	
		}

	});

	jQuery('#tr_esdid').click(function(){
		
		if(jQuery('#esdid_campo_on').css('display') == 'block'){
			
			jQuery('#filtro_periodo').show();
		}else{
			
			jQuery('#filtro_periodo').hide();
		}
	});
	
});

//-->
</script>