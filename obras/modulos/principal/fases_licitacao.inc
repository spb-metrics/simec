<?php
$titulo_modulo = "Fases de Licitação";
monta_titulo( $titulo_modulo, 'Selecione a fase desejada' );
$obras = new Obras();
$dobras = new DadosObra(null);
if($_SESSION["obra"]["obrid"]){
	
	$dados = $obras->Dados($_SESSION["obra"]["obrid"]);
	$dobras = new DadosObra($dados);
}      
?>
<html>
	<head>
		<title>Fases da Licitação</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script src="../includes/calendario.js"></script>
		<script type="text/javascript">
			<!--
				function populaValoresCamposFaseLicitacao(id){
					
					var form = document.formulario;
					for(i = 0 ; i < form.elements.length ; i++){
						var CampoAtual = form.elements[i];
						if(CampoAtual.name =="flcpubleditaldtprev"){
							CampoAtual.value = window.opener.document.getElementById("flcpubleditaldtprev_"+id+"").value;
						}
						if(CampoAtual.name =="flcdtrecintermotivo"){
							CampoAtual.value = window.opener.document.getElementById("flcdtrecintermotivo_"+id+"").value;
						}

						if(CampoAtual.name =="flcrecintermotivo"){
							CampoAtual.value = window.opener.document.getElementById("flcrecintermotivo_"+id+"").value;
						}

						if(CampoAtual.name =="flcordservdt"){
							CampoAtual.value = window.opener.document.getElementById("flcordservdt_"+id+"").value;
						}
						
						if(CampoAtual.name =="flcordservnum"){
							CampoAtual.value = window.opener.document.getElementById("flcordservnum_"+id+"").value;
						}
						
						if(CampoAtual.name =="flchomlicdtprev"){
							CampoAtual.value = window.opener.document.getElementById("flchomlicdtprev_"+id+"").value;
						}
						
						if(CampoAtual.name =="flcaberpropdtprev"){
							CampoAtual.value = window.opener.document.getElementById("flcaberpropdtprev_"+id+"").value;
						}							
						
					}					
				}

				
				function abreCamposFaseLicitacao(id)
				{
				var tr_public  = document.getElementById( 'publicacao' );
				var tr_rec  = document.getElementById( 'recurso' );
				var tr_ord  = document.getElementById( 'ordem_de_servico' );
				var tr_hom  = document.getElementById( 'homologacao' );
				var tr_aberprop = document.getElementById( 'abertura_de_proposta' );
				
				if(id == ''){
					if (document.selection){
						tr_public.style.display = 'none';
						tr_rec.style.display = 'none';
						tr_ord.style.display = 'none';
						tr_hom.style.display = 'none';
						tr_aberprop.style.display = 'none';
					}else{
						tr_public.style.display = 'none';
						tr_rec.style.display = 'none';
						tr_ord.style.display = 'none';
						tr_hom.style.display = 'none';
						tr_aberprop.style.display = 'none';
					}
				}
				if(id == 2){
					if (document.selection){
						tr_public.style.display = 'block';
						tr_rec.style.display = 'none';
						tr_ord.style.display = 'none';
						tr_hom.style.display = 'none';
						tr_aberprop.style.display = 'none';
						document.formulario.flcdtrecintermotivo.value = '';
						document.formulario.flcrecintermotivo.value = '';
						document.formulario.flcordservdt.value = '';
						document.formulario.flcordservnum.value = '';
						document.formulario.flchomlicdtprev.value = '';
						document.formulario.flcaberpropdtprev.value = '';
					}else{
						tr_public.style.display = 'table-row';
						tr_rec.style.display = 'none';
						tr_ord.style.display = 'none';
						tr_hom.style.display = 'none';
						tr_aberprop.style.display = 'none';
					}
				}
				if(id == 5){
					if (document.selection){
						tr_rec.style.display = 'block';
						tr_public.style.display = 'none';
						tr_ord.style.display = 'none';
						tr_hom.style.display = 'none';
						tr_aberprop.style.display = 'none';
						document.formulario.flcpubleditaldtprev.value = '';
						document.formulario.flcordservdt.value = '';
						document.formulario.flcordservnum.value = '';
						document.formulario.flchomlicdtprev.value = '';
						document.formulario.flcaberpropdtprev.value = '';
					}else{
						tr_rec.style.display = 'table-row';
						tr_public.style.display = 'none';
						tr_ord.style.display = 'none';
						tr_hom.style.display = 'none';
						tr_aberprop.style.display = 'none';
					}
				}
				if(id == 6){
					if (document.selection){
						tr_rec.style.display = 'none';
						tr_public.style.display = 'none';
						tr_ord.style.display = 'block';
						tr_hom.style.display = 'none';
						tr_aberprop.style.display = 'none';
						document.formulario.flcpubleditaldtprev.value = '';
						document.formulario.flcdtrecintermotivo.value = '';
						document.formulario.flcrecintermotivo.value = '';
						document.formulario.flchomlicdtprev.value = '';
						document.formulario.flcaberpropdtprev.value = '';
					}else{
						tr_rec.style.display = 'none';
						tr_public.style.display = 'none';
						tr_ord.style.display = 'table-row';
						tr_hom.style.display = 'none';
						tr_aberprop.style.display = 'none';
					}
				}
				if(id == 9){
					if (document.selection){
						tr_rec.style.display = 'none';
						tr_public.style.display = 'none';
						tr_ord.style.display = 'none';
						tr_hom.style.display = 'block';
						tr_aberprop.style.display = 'none';
						document.formulario.flcpubleditaldtprev.value = '';
						document.formulario.flcdtrecintermotivo.value = '';
						document.formulario.flcrecintermotivo.value = '';
						document.formulario.flcordservdt.value = '';
						document.formulario.flcordservnum.value = '';
						document.formulario.flcaberpropdtprev.value = '';
					}else{
						tr_rec.style.display = 'none';
						tr_public.style.display = 'none';
						tr_ord.style.display = 'none';
						tr_hom.style.display = 'table-row';
						tr_aberprop.style.display = 'none';
					}
				}
				if(id == 7){
					if (document.selection){
						tr_rec.style.display = 'none';
						tr_public.style.display = 'none';
						tr_ord.style.display = 'none';
						tr_hom.style.display = 'none';
						tr_aberprop.style.display = 'block';
						document.formulario.flcpubleditaldtprev.value = '';
						document.formulario.flcdtrecintermotivo.value = '';
						document.formulario.flcrecintermotivo.value = '';
						document.formulario.flcordservdt.value = '';
						document.formulario.flcordservnum.value = '';
						document.formulario.flchomlicdtprev.value = '';
					}else{
						tr_rec.style.display = 'none';
						tr_public.style.display = 'none';
						tr_ord.style.display = 'none';
						tr_hom.style.display = 'none';
						tr_aberprop.style.display = 'table-row';
					}
				}
			}
			
			-->
		</script>
		
		<script type="text/javascript"><!--
					
			function salvafases(form){
				
				var formOpener = window.opener.document.getElementById("formulario");
				var flcid = window.opener.document.getElementById("flcid");
				
				var flag = "";
				var add = false;
				var osNum = null;
				var flcrecintermotivoDesc = null;
				for(i = 0 ; i < form.elements.length ; i++){
					
					var CampoAtual = form.elements[i];
					
					if(CampoAtual.type == "select-one" ){
						var index = CampoAtual.selectedIndex;
						combo_id = CampoAtual.options[index].value;
						tr_id = CampoAtual.options[index].value;
						combo_text = CampoAtual.options[index].text;
					}
					if(CampoAtual.name.indexOf("dt") != -1){
						if(CampoAtual.value.length > 0){
							data_value = CampoAtual.value;
							data_name = CampoAtual.name;
						}
					}
					if(CampoAtual.name.indexOf("flcordservnum") != -1){
						if(CampoAtual.value.length > 0){
							osNum = CampoAtual.value;
							osName = CampoAtual.name;
						}
					}
					if(CampoAtual.name == "flcrecintermotivo" ){
						if(CampoAtual.value.length > 0){
							flcrecintermotivoDesc = CampoAtual.value;
						}
					}
					
					
				}
				add = true;
				// Verifica se a data foi preenchda
				try{
					document.formulario.data_form.value=data_value;
					//alert(data_value+" - "+data_name);
				}
				catch(e){
					add = false;
					window.alert("Favor preencher a Data.");
				}
				novo=window.document.getElementById("flcid_f");
				if (novo.value != "")
					novo=false;
				else
					novo=true;
				if(novo){
					// Verifica se a data do registro está maior que a última data inserida				
					try{
						na_tela_flcdata=window.opener.document.getElementsByName("flcdata[]");
						data_opener=na_tela_flcdata[parseInt(na_tela_flcdata.length)-1].value;
						document.formulario.data_opener.value=data_opener;
						data_form=document.getElementById("data_form");
						data_opener=document.getElementById("data_opener");
						//alert(data_form.value +"-"+data_opener.value);
						if (!validaDataMaior(data_opener, data_form)){
							
							add = false;
							msg_erro="A data deve ser maior que a última data inserida.";
						}
					}catch(e){
						//window.alert("Data");
				  	}
					// Verificando se o registro é o único
					try{
						// Verifica se o registro já consta nas fases de contratação
						na_tela_tflid=window.opener.document.getElementsByName("tflid[]");
						for(i = 0 ; i < na_tela_tflid.length ; i++){
							if(na_tela_tflid[i].value == combo_id){
								add = false;
								msg_erro="Item já inserido.";
								i=na_tela_tflid.length;
							}
						} 
					}catch(e){
						//window.alert("Unico");
				  	}
			  	}
				if(add){
					
					var tabela = window.opener.document.getElementById("faseslicitacao");
					var flcid = document.getElementById("flcid_f");
					
					if(flcid.value == ""){
						var flcidNum = "_A"+Math.random(10);
						var tamanho = tabela.rows.length;
						var tr = tabela.insertRow(tamanho);	
						tr.id = "tr_"+flcidNum;
					}else{
						var flcidNum =flcid.value;
						linhaid = window.opener.document.getElementById('tr_'+flcidNum).rowIndex;
						tabela.deleteRow(linhaid);
						tr = tabela.insertRow(linhaid);
						tr.id = "tr_"+flcidNum;
					}
										
					var colAcao = tr.insertCell(0);
					var colDesc = tr.insertCell(1);
					var colData = tr.insertCell(2);
					
					colAcao.style.textAlign = "center";
				
					var botao =   ""
						+ "<img src='/imagens/alterar.gif' style='cursor: pointer' border=0 title='Editar' onclick=\"atualizaFase(" + combo_id + ",'"+flcidNum+"')\";/>&nbsp&nbsp&nbsp"
						+ "<img src='/imagens/excluir.gif' style='cursor: pointer'  border=0 title='Excluir' onclick='RemoveLinha(window.document.getElementById(\"tr_"+flcidNum+"\").rowIndex,\""+flcidNum+"\");'>"; 
													
					colAcao.innerHTML = botao;						
						
					colDesc.innerHTML = '<input type="hidden" name="tflid[]" id="tflid_'+flcidNum+'" value="' + combo_id + '">' + combo_text;
					colData.innerHTML = '';
					
					if(data_name == "flcpubleditaldtprev"){
						colData.innerHTML += '<input type="hidden" name="flcpubleditaldtprev[]" id="flcpubleditaldtprev_'+flcidNum+'" value="' + data_value + '">';
						if( window.opener.document.getElementById('dtiniciolicitacao') ){
							window.opener.document.getElementById('dtiniciolicitacao').value = data_value;
						}
					}else{
						colData.innerHTML += '<input type="hidden" name="flcpubleditaldtprev[]" id="flcpubleditaldtprev_'+flcidNum+'" value="">';
					}
					if(data_name =="flcdtrecintermotivo"){
						colData.innerHTML += '<input type="hidden" name="flcdtrecintermotivo[]" id="flcdtrecintermotivo_'+flcidNum+'" value="' + data_value + '">';
					}else{
						colData.innerHTML += '<input type="hidden" name="flcdtrecintermotivo[]" id="flcdtrecintermotivo_'+flcidNum+'" value="">';						
					}
					if(data_name =="flcordservdt"){
					colData.innerHTML += '<input type="hidden" name="flcordservdt[]" id="flcordservdt_'+flcidNum+'" value="' + data_value + '">';
					}else{
					colData.innerHTML += '<input type="hidden" name="flcordservdt[]" id="flcordservdt_'+flcidNum+'" value="">';						
					}
					if(data_name =="flchomlicdtprev"){
					colData.innerHTML += '<input type="hidden" name="flchomlicdtprev[]" id="flchomlicdtprev_'+flcidNum+'" value="' + data_value + '">';
					}else{
					colData.innerHTML += '<input type="hidden" name="flchomlicdtprev[]" id="flchomlicdtprev_'+flcidNum+'" value="">';						
					}
					if(data_name =="flcaberpropdtprev"){
					colData.innerHTML += '<input type="hidden" name="flcaberpropdtprev[]" id="flcaberpropdtprev_'+flcidNum+'" value="' + data_value + '">';
					}else{
					colData.innerHTML += '<input type="hidden" name="flcaberpropdtprev[]" id="flcaberpropdtprev_'+flcidNum+'" value="">';						
					}
					
					colData.innerHTML += '' + data_value;
					colData.innerHTML += '<input type="hidden" name="flcid[]" id="flcid_'+flcidNum+'" value="'+flcidNum+'">';
					colData.innerHTML += '<input type="hidden" name="flcordservnum[]" id="flcordservnum_'+flcidNum+'" value="' + osNum + '">';
					colData.innerHTML += '<input type="hidden" name="flcdata[]" id="flcdata'+flcidNum+'" value="'+data_value+'">';
					colData.innerHTML += '<input type="hidden" name="flcrecintermotivo[]" id="flcrecintermotivo_'+flcidNum+'" value="'+flcrecintermotivoDesc+'">';
						
					window.close();	
				}else{
					alert(msg_erro);
					return false;
				}
				window.close();
				return true;
			}
			
			function BuscaCampo(form,camponame){
				for(i = 0 ; i < form.elements.length ; i++){
					var CampoAtual = form.elements[i];
					if(CampoAtual.name == camponame){
						return true;
					}else{
						return false;
					}
				}
			}
			
		--></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
		<form name="formulario" method="post" onSubmit="return salvafases(this);">
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
				<tr>
					<td class="subtitulodireita">Selecione a Fase</td>
					<td>
						<?php
							$novo=true;
							if ($_REQUEST["tflid"]){
								$tflid = $_REQUEST["tflid"];
								$novo=false;
							}
							// Filtra os registros de acordo com a tela chamada
							$stFiltro = '';
							if ( $_REQUEST['objeto'] ) {
								switch ( $_REQUEST['objeto'] ){
									case 'licitacao' : $stFiltro = "WHERE tflexibefl = true AND tflstatus = 'A' "; break;
									case 'projeto'   : $stFiltro = "WHERE tflexibefp = true AND tflstatus = 'A' "; break;
								}
							}
							$sql = "SELECT tflid AS codigo, tfldesc AS descricao 
									FROM obras.tiposfaseslicitacao ".$stFiltro." ORDER BY tflordem ";
							
							if ($novo)
								$db->monta_combo('tflid', $sql, 'S', "Selecione...", 'abreCamposFaseLicitacao', '', '', null, 'S', 'flcid'); 
							else
								$db->monta_combo('tflid', $sql, 'N', "Selecione...", 'abreCamposFaseLicitacao', '', '', null, 'S', 'flcid');							
						?>
					</td>
				</tr>
				<tr id="publicacao" style="display: none;">
					<td class="SubTituloDireita">Data</td>
					<td>
						<?
						if ($novo)
							echo campo_data( 'flcpubleditaldtprev', 'S', 'S', '', 'S' ); 
						else
							echo campo_data( 'flcpubleditaldtprev', 'S', 'N', '', 'S' );?>
					</td>
				</tr>
				<tr id="recurso" style="display: none;">
					<td align="center" colspan="2">
						<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
							<tr>
								<td class="SubTituloDireita">Data Prevista</td>
								<td>
									<? #$flcdtrecintermotivo = $dadosObra['flcdata']; ?>
									<? 
									if ($novo)
										echo campo_data( 'flcdtrecintermotivo', 'S', 'S', '', 'S' ); 
									else
										echo campo_data( 'flcdtrecintermotivo', 'S', 'N', '', 'S' );?>
								</td>
							</tr>
							<tr>
								<td class="SubTituloDireita">Motivo</td>
								<td>
									<? #$flcrecintermotivo = $dadosObra['flcrecintermotivo']; ?>
									<?= campo_textarea( 'flcrecintermotivo', 'N', 'S', '', '50', '4', '500'); ?>
								</td>
							</tr>
						</table>						
					</td>
				</tr>
				<tr id="ordem_de_servico" style="display: none;">
					<td align="center" colspan="2">
						<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
							<tr>
								<td class="SubTituloDireita">Data da O.S.</td>
								<td>
									<? 
									#$flcordservdt = $dadosObra['flcdata']; ?>
									<?
									if ($novo)
										echo campo_data( 'flcordservdt', 'S', 'S', '', 'S' );
									else
										echo campo_data( 'flcordservdt', 'S', 'N', '', 'S' );
									 ?>
								</td>
							</tr>
							<tr>
								<td class="SubTituloDireita">Número da O.S.</td>
								<td>
									<?
									
									#$flcordservnum = $dadosObra['flcordservnum']; 
									?>
									<?= campo_texto( 'flcordservnum', 'N', 'S', '', 17, 15, '',  '', 'left', '', 0); ?>
								</td>
							</tr>
						</table>
					</td>
				</tr>
				<tr id="homologacao" style="display: none;">
					<td class="SubTituloDireita">Data Prevista</td>
					<td>
						<? #$flchomlicdtprev = $dadosObra['flchomlicdtprev'] ?>
						<?
						if ($novo)
							echo campo_data( 'flchomlicdtprev', 'S', 'S', '', 'S' ); 
						else
							echo campo_data( 'flchomlicdtprev', 'S', 'N', '', 'S' )?>
						
					</td>
				</tr>
				<tr id="abertura_de_proposta" style="display: none;">
					<td class="SubTituloDireita">Data Prevista</td>
					<td>
						<? #$flcaberpropdtprev = $dadosObra['flcaberpropdtprev']; ?>
						<?
						if ($novo)
							echo campo_data( 'flcaberpropdtprev', 'S', 'S', '', 'S' );
						else
							echo campo_data( 'flcaberpropdtprev', 'S', 'N', '', 'S' ); ?>
					</td>
				</tr>
				<tr bgcolor="#C0C0C0">
					<td colspan="2">
						<div style="float: left;">
							<input type="submit" name="ok" value="Ok">
						</div>
					</td>
				</tr>
			</table>
			<input type=hidden name="data_form" id="data_form" value="">
			<input type=hidden name="data_opener" id="data_opener" value="">
			<?php 
			echo "<input type='hidden' name='flcid_f' id='flcid_f' value='".$_REQUEST["flcid"]."'/>";
			?>			
		</form>
	</body>
</html>

<?php
	if($_REQUEST["tflid"]){
		echo "<script>";
			echo "abreCamposFaseLicitacao(".$_REQUEST["tflid"]."); ";
			echo " populaValoresCamposFaseLicitacao('".$_REQUEST["flcid"]."');";
		echo "</script>";
	}
	
?>