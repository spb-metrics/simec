<?php

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

$obras = new Obras();
$dobras = new DadosObra(null);
$infraestrutura = new DadosInfraEstrutura();

// Executa as ações do módulo
if($_REQUEST["requisicao"] ){
	
	if ( $_REQUEST['requisicao'] == 'excluir' ){
		$obras->DeletarDocumento( $_REQUEST, 'principal/infraestrutura' );	
	}else{

		if ( !empty( $_FILES['arquivo']['name'] ) ){

			if ( empty($_REQUEST["tpaid"]) ){
				echo '<script>
						alert("Favor Selecionar o Tipo de Arquivo!");
						window.location="?modulo=principal/infraestrutura&acao=A";	
					  </script>';
				die;
			}  
			
			switch( $_REQUEST['requisicao'] ){
				case "1":
					$dir = 'infraestrutura&acao=A';
					$obras->EnviarArquivo( $_FILES, $_POST, $dir );
					exit;
				break;
				case "download":
					$obras->DownloadArquivo( $_REQUEST );
				break;
			}
					
		}
		
		$obras->CadastrarInfraEstrutura( $_REQUEST );	
		
	}
		
}

if(isset($_SESSION['obra']['obrid']) && ($_SESSION['obra']['obrid'] != '')) {
	$resultado = $infraestrutura->busca($_SESSION['obra']['obrid']);
	$dados = $infraestrutura->dados($resultado);
	//pega o tipo obra
	$tobraid = ($db->pegaUm("SELECT tobraid FROM obras.obrainfraestrutura WHERE obrid = {$_SESSION['obra']['obrid']}"));
} else {
	die("<script>alert('Variavel da obra não encontrado.');window.location='obras.php?modulo=inicio&acao=A';</script>");
}

if ($_REQUEST["acao"] == "B"){
	$obras->DeletarRestricao($_REQUEST["rstoid"]);
}


?>

<br/>
<script src="../includes/calendario.js"></script>
<script src="../includes/jquery.js"></script>
<script type="text/javascript">

function habilitaCampo(vlr){
	
	var area = window.document.getElementById("iexareaconstruida");
	var unidade = window.document.getElementById("umdid");
	var descricao = window.document.getElementById("iexdescsumariaedificacao");
	var reforma = window.document.getElementById("iexedificacaoreforma");
	var ampliacao = window.document.getElementById("iexampliacao");
	var iexqtdareapreforma = window.document.getElementById("iexqtdareapreforma");
	var umdidareareforma = window.document.getElementById("umdidareareforma");
	var iexvlrareapreforma = window.document.getElementById("iexvlrareapreforma");
	var iexqtdareaampliada = window.document.getElementById("iexqtdareaampliada");
	var umdidareaampliada = window.document.getElementById("umdidareaampliada");
	var iexvlrareaampliada = window.document.getElementById("iexvlrareaampliada");
	
	if (vlr == 0){
		area.disabled = true;
		unidade.disabled = true;
		descricao.disabled = true;
		reforma.disabled = true;
		ampliacao.disabled = true;
		
		area.value = '';
		unidade.value = '';
		descricao.value = '';
		reforma.value = '';
		ampliacao.value = '';
		iexqtdareapreforma.value = '';
		umdidareareforma.value = '';
		iexvlrareapreforma.value = '';
		iexqtdareaampliada.value = '';
		umdidareaampliada.value = '';
		iexvlrareaampliada.value = '';
		abreAmpliacao(0);
		abreReforma(0);
		
		document.formulario.iexedificacaoreforma[1].checked = true;
		document.formulario.iexampliacao[1].checked = true;
		
		// Esconde linhas da tabela que não devem ser mostradas
		window.document.getElementById("trAreaConstruida").style.display = 'none';
		window.document.getElementById("trDescricaoSumariaEdificacao").style.display = 'none';
		window.document.getElementById("trEdificacoesReforma").style.display = 'none';
		window.document.getElementById("trNecessidadeAmpliacao").style.display = 'none';
		
	}
	
	if (vlr == 1){
		area.disabled      = false;
		unidade.disabled   = false;
		descricao.disabled = false;
		reforma.disabled   = false;
		ampliacao.disabled = false;
		
		area.readOnly      = false;
		unidade.readOnly   = false;
		descricao.readOnly = false;
		reforma.readOnly   = false;
		ampliacao.readOnly = false;
		
		// Define a unidade como m2 como padrão
		unidade.value = 4;
		
		// Esconde linhas da tabela que não devem ser mostradas
		window.document.getElementById("trAreaConstruida").style.display = '';
		window.document.getElementById("trDescricaoSumariaEdificacao").style.display = '';
		window.document.getElementById("trEdificacoesReforma").style.display = '';
		window.document.getElementById("trNecessidadeAmpliacao").style.display = '';
	}

}	

function abreReforma(vlr){
	
	var iexqtdareapreforma = window.document.getElementById("iexqtdareapreforma");
	var umdidareareforma = window.document.getElementById("umdidareareforma");
	var iexvlrareapreforma = window.document.getElementById("iexvlrareapreforma");
	var simReforma = window.document.getElementById("simReforma"); 
	
	if(vlr == 1){
		if (document.selection){
			simReforma.style.display = "block";
		}else{
			simReforma.style.display = "table-row";
		}	
	}
	
	if(vlr == 0){
		if (document.selection){
			simReforma.style.display = "none";
		}else{
			simReforma.style.display = "none";
		}
			
		iexqtdareapreforma.value = '';
		umdidareareforma.value = '';
		iexvlrareapreforma.value = '';
	}
	
}

function abreDocumentosInfra( id ){
	if ( document.selection ){
		document.getElementById( 'documentosinfra' ).style.display = 'block';
	} else{
		document.getElementById( 'documentosinfra' ).style.display = 'table-row';
	}	
	
}

function abreAmpliacao(vlr){

	var iexqtdareaampliada = window.document.getElementById("iexqtdareaampliada");
	var umdidareaampliada = window.document.getElementById("umdidareaampliada");
	var iexvlrareaampliada = window.document.getElementById("iexvlrareaampliada");
	var simAmpliacao = window.document.getElementById("simAmpliacao"); 
	
	if(vlr == 1){
		if (document.selection){
			simAmpliacao.style.display = "block";
		}else{
			simAmpliacao.style.display = "table-row";
		}	
	}
	
	if(vlr == 0){
		if (document.selection){
			simAmpliacao.style.display = "none";
			
		}else{
			simAmpliacao.style.display = "none";
		}
		
		iexqtdareaampliada.value = '';
		umdidareaampliada.value = '';
		iexvlrareaampliada.value = '';
			
	}
}

function inserirModulos(){
	return windowOpen( caminho_atual + '?modulo=principal/inserir_modulos&acao=A','blank','height=450,width=400,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}

function excluiItem(id) {		
	var linha = document.getElementById("linha_"+id).rowIndex;
	var tabela = document.getElementById("tabela_modulos");
	tabela.deleteRow(linha);	
	if(tabela.rows.length == 2) {
		tabela.deleteRow(1);
	}
}

</script>

<?php

$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo = "Infra-Estrutura";
monta_titulo( $titulo_modulo, 'Informe do detalhamento da Infra-estrutura da Obra' );
echo $obras->CabecalhoObras();

?>
<form name="formulario" id="formulario" method="post" enctype="multipart/form-data" onsubmit="return validaInfraEstrutura();" action="<?php echo $caminho_atual; ?>acao=A"> 
	<input type="hidden" name="requisicao" value="1"/> 
	<table class="tabela" id="infraEstrutura" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloDireita">Tipo de Aquisição do Terreno</td>
			<td>
				<?php
					$aqiid = $infraestrutura->aqiid;
					
					$sql = "SELECT 
								aqiid AS codigo, 
								aqidsc AS descricao 
							FROM 
								obras.tipoaquisicaoimovel
							ORDER BY
								aqiid";
					 
					$db->monta_combo("aqiid", $sql, 'S','Selecione...', '', '', '', '', 'N', 'aqiid');
				?>
				
			</td>
		</tr>
		<tr>
			<td width="300px" class="SubTituloDireita">Situação Dominial já Regularizada?</td>
			<td>
				<?php
					$iexsitdominialimovelregulariza = $infraestrutura->iexsitdominialimovelregulariza;
					if ($iexsitdominialimovelregulariza == "t"){ 
				?>
					<input type="radio" name="iexsitdominialimovelregulariza" id="iexsitdominialimovelregulariza" value="1" checked <? echo $disabled ?> /> Sim
					<input type="radio" name="iexsitdominialimovelregulariza" id="iexsitdominialimovelregulariza" value="0" <? echo $disabled ?> /> Não
				<?php }else { ?>
					<input type="radio" name="iexsitdominialimovelregulariza" id="iexsitdominialimovelregulariza" value="1" <? echo $disabled ?> /> Sim
					<input type="radio" name="iexsitdominialimovelregulariza" id="iexsitdominialimovelregulariza" value="0" checked <? echo $disabled ?> /> Não
				<?php } ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Existem Edificações no Local da Obra?</td>
			<td>
				<?php 
					$iexinfexistedimovel = $infraestrutura->iexinfexistedimovel;
					if ($iexinfexistedimovel == "t"){
						$somenteLeituraLocalObra = 'S'; 
				?>
					<input type="radio" name="iexinfexistedimovel" id="iexinfexistedimovel" value="1" onclick="habilitaCampo(1);" checked <? echo $disabled ?> /> Sim
					<input type="radio" name="iexinfexistedimovel" id="iexinfexistedimovel" value="0" onclick="habilitaCampo(0);" <? echo $disabled ?> /> Não
				<?php }else {
						$somenteLeituraLocalObra = 'N';
					?>
					<input type="radio" name="iexinfexistedimovel" id="iexinfexistedimovel" value="1" onclick="habilitaCampo(1);" <? echo $disabled ?> /> Sim
					<input type="radio" name="iexinfexistedimovel" id="iexinfexistedimovel" value="0" onclick="habilitaCampo(0);" checked <? echo $disabled ?> /> Não
				<?php } ?>
			</td>
		</tr>
		<tr id="trAreaConstruida">
			<td class="SubTituloDireita">Área Construída</td>
			<td>
				<?php 
					$iexareaconstruida = $infraestrutura->iexareaconstruida;
					$iexareaconstruida = number_format($iexareaconstruida,2,',','.'); 
				?>
				<?= campo_texto( 'iexareaconstruida', 'N', $somenteLeituraLocalObra, '', 14, 10, '###.###,##', '', 'left', '', 0, 'id="iexareaconstruida"'); ?>
			
				Unidade de Medida
			
				<?php
					$umdidareaconstruida = $infraestrutura->umdidareaconstruida;
					
					$sql = "SELECT 
								umdid AS codigo, 
								umdeesc AS descricao 
							FROM 
								obras.unidademedida";
					 
					$db->monta_combo("umdidareaconstruida", $sql, $somenteLeituraLocalObra,'', '', '', '', '100', 'N', 'umdid');
				?>
				
			</td>
		</tr>
		<tr id="trDescricaoSumariaEdificacao">
			<td class="SubTituloDireita">Descrição Sumária da Edificação</td>
			<td>
				<?php $iexdescsumariaedificacao = $infraestrutura->iexdescsumariaedificacao; ?>
				<?= campo_textarea( 'iexdescsumariaedificacao', 'N', $somenteLeituraLocalObra, '', '70', '4', '500'); ?>
			</td>
		</tr>
		<tr id="trEdificacoesReforma">
			<td class="SubTituloDireita">A(s) Edificaçõe(s) Necessita(m) de Reforma(s)?</td>
			<td>
				<?php 
					$iexedificacaoreforma = $infraestrutura->iexedificacaoreforma;
					if ($iexedificacaoreforma == "t"){ 
				?>
				<input type="radio" name="iexedificacaoreforma" id="iexedificacaoreforma" value="1" onclick="abreReforma(1)" checked <? echo $disabled ?> /> Sim
				<input type="radio" name="iexedificacaoreforma" id="iexedificacaoreforma" value="0" onclick="abreReforma(0);" <? echo $disabled ?> /> Não
				<?php }else{ ?>
				<input type="radio" name="iexedificacaoreforma" id="iexedificacaoreforma" value="1" onclick="abreReforma(1)" <? echo $disabled ?> /> Sim
				<input type="radio" name="iexedificacaoreforma" id="iexedificacaoreforma" value="0" onclick="abreReforma(0);" checked <? echo $disabled ?> /> Não
				<?php } ?>
			</td>
		</tr>
		<tr id="simReforma" style="display: none;">
			<td colspan="2">
				<br/>
					<center>
						<table class='tabela' style="width:70%;" cellpadding="3">
							<tr>
								<td class="SubTituloDireita">Área a ser Reformada</td>
								<td>
									<?php 
										$iexqtdareapreforma = $infraestrutura->iexqtdareapreforma;
										$iexqtdareapreforma = number_format($iexqtdareapreforma,2,',','.'); 
									?>
									<?= campo_texto( 'iexqtdareapreforma', 'N', $somenteLeitura, '', 16, 10, '###.###,##', '', 'left', '', 0, 'id="iexqtdareapreforma"'); ?>
																			
									Unidade de Medida
									
									<?php
										$umdidareareforma = $infraestrutura->umdidareareforma;
										$sql = "SELECT 
													umdid AS codigo, 
													umdeesc AS descricao 
												FROM 
													obras.unidademedida";
										 
										$db->monta_combo("umdidareareforma", $sql, $somenteLeitura, '', '', '', '', '100', 'N', 'umdidareareforma');
									?>
									
								</td>
							</tr>
							<tr>
								<td class="SubTituloDireita">Custo Estimado R$</td>
								<td>
									<?php 
										$iexvlrareapreforma = $infraestrutura->iexvlrareapreforma;
										$iexvlrareapreforma = number_format($iexvlrareapreforma,2,',','.'); 
									?>
									<?= campo_texto( 'iexvlrareapreforma', 'N', $somenteLeitura, '', 16, 14, '###.###.###,##', '', 'left', '', 0, 'id="iexvlrareapreforma"'); ?>
								</td>
							</tr>
						</table>
					</center>
				<br/>
			</td>
		</tr>
		<tr id="trNecessidadeAmpliacao">
			<td class="SubTituloDireita">Há Necessidade de Ampliação?</td>
			<td>
				<?php 
					$iexampliacao = $infraestrutura->iexampliacao;
					if ($iexampliacao == "t"){ 
				?>
				<input type="radio" name="iexampliacao" id="iexampliacao" value="1" onclick="abreAmpliacao(1)" checked <? echo $disabled ?> /> Sim
				<input type="radio" name="iexampliacao" id="iexampliacao" value="0" onclick="abreAmpliacao(0)" <? echo $disabled ?> /> Não
				<?php } else { ?>
				<input type="radio" name="iexampliacao" id="iexampliacao" value="1" onclick="abreAmpliacao(1)" <? echo $disabled ?> /> Sim
				<input type="radio" name="iexampliacao" id="iexampliacao" value="0" onclick="abreAmpliacao(0)" checked <? echo $disabled ?> /> Não
				<?php } ?>
			</td>
		</tr>
		<tr id="simAmpliacao" style="display: none;">
			<td colspan="2">
				<br/>
					<center>
						<table class='tabela' style="width:70%;" cellpadding="3">
							<td class="SubTituloDireita">Área a ser Reformada</td>
								<td>
									<?php 
										$iexqtdareaampliada = $infraestrutura->iexqtdareaampliada;
										$iexqtdareaampliada = number_format($iexqtdareaampliada,2,',','.'); 
									?>
									<?= campo_texto( 'iexqtdareaampliada', 'N', $somenteLeitura, '', 16, 10, '###.###,##', '', 'left', '', 0, 'id="iexqtdareaampliada"'); ?>
																			
									Unidade de Medida
									
									<?php
										$umdidareaampliada = $infraestrutura->umdidareaampliada;
										$sql = "SELECT 
													umdid AS codigo, 
													umdeesc AS descricao 
												FROM 
													obras.unidademedida";
										 
										$db->monta_combo("umdidareaampliada", $sql, $somenteLeitura, '', '', '', '', '100', 'N', 'umdidareaampliada');
									?>
									
								</td>
							</tr>
							<tr>
								<td class="SubTituloDireita">Custo Estimado R$</td>
								<td>
									<?php 
										$iexvlrareaampliada = $infraestrutura->iexvlrareaampliada;
										$iexvlrareaampliada = number_format($iexvlrareaampliada,2,',','.'); 
									?>
									<?= campo_texto( 'iexvlrareaampliada', 'N', $somenteLeitura, '', 16, 14, '###.###.###,##', '', 'left', '', 0, 'id="iexvlrareaampliada"'); ?>
								</td>
							</tr>
							<tr>
								<td>Módulos de Ampliação Necessários</td>
							</tr>
							<tr>
								<td colspan="2">
									<table id="tabela_modulos" width="80%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
										<thead>
											<tr id="cabecalho">
												<td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
												<td width="90%" valign="top" align="center" class="title" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Descrição</strong></td>
											</tr>
										</thead>
										<tbody>
											<?php
												
												$iexid = ($db->pegaUm("
													SELECT 
														iexid 
													FROM 
														obras.obrainfraestrutura
													WHERE
														obrid = {$_SESSION['obra']['obrid']}"));
												
												if ($iexid){
													$sql = pg_query("
														SELECT 
															mod.tmaid,
															tip.tmadesc 
														FROM 
															obras.modulosampliacao mod
														INNER JOIN
															obras.tipomoduloampliacao tip
														ON
															mod.tmaid = tip.tmaid
														WHERE 
															iexid = {$iexid}");

													$count = 1;
													
													while (($dados = pg_fetch_array($sql)) != false){
														
														$tmaid = $dados["tmaid"];
														$tmadesc = $dados["tmadesc"];
														
														if ($habilitado){
															$excluir_modulo = "<img src='/imagens/excluir.gif' style='cursor:pointer;' border='0' title='Excluir' onclick='excluiItem(". $tmaid .");'>";
														}else{
															$excluir_modulo = "<img src='/imagens/excluir_01.gif' style='cursor:pointer;' border='0'/>";
														}
														
														$cor = "#f4f4f4";
														$count++;
														if ($count % 2){
															$cor = "#e0e0e0";
														}
														
														echo "
															<tr id=\"linha_".$tmaid."\" bgcolor=\"" . $cor . "\">
																<td align=\"center\">
																	" . $excluir_modulo . "
																</td>
																<td>" . $tmadesc . "<input type='hidden' id='tmaid[" . $tmaid . "]' name='tmaid[]' value='" . $tmaid . "'></td>
															</tr>";
													}
												}
											?>
										</tbody>
									</table>
								</td>
							</tr>
							<tr>
								<td>
									<?php if($habilitado){ ?>
										<a href="#" onclick="inserirModulos(); return false;"><img src="/imagens/gif_inclui.gif" style="cursor:pointer;" border="0" title="Inserir Módulos"> Inserir Módulos</a>
									<?php } ?>
								</td>
							</tr>
						</table>
					</center>
				<br/>
			</td>
		</tr>
		<tr>
			<td colspan="2"> 
				Anexar Documentos
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
			<td>
				<?php if( $habilitado ){ ?>
					<input type="file" name="arquivo"/>
				<?php } ?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Tipo:</td>
			<td><?php
			
			$sql = "
				SELECT 
					tpaid AS codigo, tpadesc AS descricao 
				FROM 
					obras.tipoarquivo
				WHERE 
					tpaid in (22, 23, 24)
				ORDER BY
					tpadesc
			";
			
			$db->monta_combo('tpaid', $sql, $somenteLeitura, "Selecione...", '', '', '', '', 'N');
		?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
			<td><?= campo_textarea( 'arqdescricao', 'N', $somenteLeitura, '', 60, 2, 250 ); ?></td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td></td>
			<td>
				<div style="float: left;">
					<?php if($habilitado){ ?>
						<input type="submit" id="salvar" value="Salvar" style="cursor: pointer" <? echo $disabled ?>>
					<?php } ?> 
					<input type="button" value="Voltar" style="cursor: pointer" onclick="history.back(-1);">
				</div>
			</td>
		</tr>
	</table>
</form>
<?php
	$sql = "SELECT
					'<center><a href=\"#\" onclick=\"javascript:ExcluirDocumento(\'" . $caminho_atual . "acao=A&requisicao=excluir" . "\',' || arq.arqid || ',' || aqb.aqoid || ');\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a></center>' as acao,						
					to_char(aqb.aqodtinclusao,'DD/MM/YYYY'),
					tarq.tpadesc,
					'<a style=\"cursor: pointer; color: blue;\" onclick=\"DownloadArquivo(' || arq.arqid || ');\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a>',
					arq.arqtamanho || ' kbs' as tamanho ,
					arq.arqdescricao,								
					usu.usunome
				FROM
					((public.arquivo arq INNER JOIN obras.arquivosobra aqb
					ON arq.arqid = aqb.arqid) INNER JOIN obras.tipoarquivo tarq
					ON tarq.tpaid = aqb.tpaid) INNER JOIN seguranca.usuario usu
					ON usu.usucpf = aqb.usucpf
				WHERE
					aqb.tpaid in (22, 23, 24) AND
					aqb.aqostatus = 'A' AND	aqb.obrid = '" . $_SESSION["obra"]["obrid"] . "'";
	
	$cabecalho = array( "Ação", 
						"Data Inclusão",
						"Tipo Arquivo",
						"Nome Arquivo",
						"Tamanho (Mb)",
						"Descrição Arquivo",
						"Responsável");
	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '' );
?>

<?php if ($tobraid == "4"){ ?>
<script type="text/javascript">
	abreReforma(1);
	abreAmpliacao(0);
	document.formulario.iexedificacaoreforma[0].checked = true;
	document.formulario.iexampliacao[1].checked = true;
	document.formulario.iexampliacao[0].disabled = true;
</script>
<?php 
}
if ($tobraid == "3"){ 
?>
<script type="text/javascript">
	abreAmpliacao(1);
	abreReforma(0);
	document.formulario.iexedificacaoreforma[1].checked = true;
	document.formulario.iexedificacaoreforma[0].disabled = true;
	document.formulario.iexampliacao[0].checked = true;
	//document.formulario.iexedificacaoreforma[1].disabled = true;
</script>
<?php } ?>
<?php 
if ($tobraid == "1" || $tobraid == ""){ 
?>
<script type="text/javascript">
	abreAmpliacao(0);
	abreReforma(0);
	document.formulario.iexedificacaoreforma[1].checked = true;
	document.formulario.iexedificacaoreforma[0].disabled = true;
	//document.formulario.iexedificacaoreforma[1].disabled = true;

	document.formulario.iexampliacao[1].checked = true;
	document.formulario.iexampliacao[0].disabled = true;
	//document.formulario.iexampliacao[1].disabled = true;

</script>
<?php } ?>