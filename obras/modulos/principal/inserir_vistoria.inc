<?php
ini_set("memory_limit", "2048M");

$docid = pegarDocidSupervisao($_REQUEST['supvid']);

if($_REQUEST['atualizarFotosAjax']){
	$sql = "select fot.*, arq.arqdescricao from obras.fotos AS fot
						left join public.arquivo AS arq ON arq.arqid = fot.arqid
						where obrid =".$_SESSION["obra"]["obrid"]." AND supvid=".$_REQUEST["supvid"]." ORDER BY fotordem;";
				$fotos = ($db->carregar($sql)); 
	$sql = "SELECT
				arq.arqid
			FROM 
				public.arquivo arq
			INNER JOIN 
				obras.arquivosobra oar ON arq.arqid = oar.arqid
			INNER JOIN 
				obras.obrainfraestrutura obr ON obr.obrid = oar.obrid 
			INNER JOIN 
				seguranca.usuario seg ON seg.usucpf = oar.usucpf 
			WHERE 
				obr.obrid = {$_SESSION["obra"]["obrid"]}
			AND
				aqostatus = 'A'
			AND
				(arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png') 
			ORDER BY 
				arq.arqid";
	$arrFotosGaleria = $db->carregar($sql);
	$arrArqidGaleria = array();
	if(is_array($arrFotosGaleria)){
		foreach($arrFotosGaleria as $foto){
			$arrArqidGaleria[] = $foto['arqid'];
		}
	}?>
	<?php if(is_array($fotos)): $arrFotos = $fotos;?>
		<?php $n = 1 ?>
		<?php foreach($arrFotos as $foto): ?>
			<?php $pagina = floor($n/16); ?>
			<li id="foto_<?php echo $foto['arqid']?>" class="draggable<?php echo in_array($foto['arqid'],$arrArqidGaleria) ? " f_selected" : ""?>">
				<img class="img_foto" ondblclick="abrirGaleria('<?php echo $foto['arqid'] ?>','<?php echo $pagina ?>')" src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $foto['arqid'] ?>&newwidth=100" />
			</li>
			<?php $n++ ?>
		<?php endforeach; ?>
	<?php endif;
	exit;
}

if( !$_SESSION["obra"]['obrid'] ){
      header( "location:obras.php?modulo=inicio&acao=A" );
      exit;
}

if( $_REQUEST['ajaxFotosGaleria'] && ($_REQUEST['fotosSupervisao'] || $_REQUEST['fotosGaleria']) ){
	if(atualizarFotosVistoria()){
		echo "true";
	}else{
		echo "false";
	}
	exit();
}

// busca as datas para validação da situação da vistoria;
$sql = "SELECT
			obrdtinicio,
			obrdttermino,
			dtiniciolicitacao,
			dtfinallicitacao,
			fprdtiniciofaseprojeto,
			fprdtconclusaofaseprojeto
		FROM
			obras.obrainfraestrutura oi
		LEFT JOIN
			obras.faseprojeto of ON of.obrid = oi.obrid
		WHERE
			oi.obrid = '".$_SESSION["obra"]["obrid"]."'";

$datasValidacao = $db->pegaLinha($sql);

obras_verificasessao();

$dado_situacao = obras_pega_situacao_vistoria( $_SESSION["obra"]["obrid"] );

if ( $dado_situacao == 3 && !$_REQUEST['supvid'] && !possuiPerfil( array( PERFIL_SUPERVISORMEC, PERFIL_GESTORMEC, PERFIL_EMPRESA,PERFIL_SUPERVISORUNIDADE ) ) ){
	print '<script>alert("Não é possivel inserir uma nova vistoria!");history.back(-1);</script>';
}

$somenteLeitura = ( $somenteLeitura == 'N' && !obraAditivoPossuiVistoria() ) ? 'S' : $somenteLeitura;
$habilitado     = ( !$habilitado && !obraAditivoPossuiVistoria() ) ? true : $habilitado;

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$obras = new Obras();
$dobras = new DadosObra(null);

// Realiza as ações do componente de fotos 
if($_REQUEST["AJAX"]){
	
	switch ($_REQUEST["subacao"]){
		case "ShowImage":
			$obras->ShowImage( $_REQUEST["img"], $_REQUEST["dir"] );
		break;
		case "UpdateListFoto":
			$obras->UpdateListFoto();
		break;
		case "FotosVistoria":
			$obras->AtualizarFotosVistoria( $_REQUEST );
		break;
		case "editar":
			$obras->AtualizarFotosVistoria( $_REQUEST );
		break;
		case "deletar":
			$obras->DeletarFotoVistoria( $_REQUEST );
		break;
		case "galeria":
			$obras->EnviarGaleria( $_REQUEST["imagens"] );
		break;
	}
	
	die;
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

$requisicao = 'cadastra';

if($_REQUEST["supvid"] && $_SESSION["obra"]["obrid"]){
	
	$somenteLeitura = obras_podeatualizarvistoria($_REQUEST["supvid"]);
	
	$requisicao = 'atualiza';
	$_SESSION["supvid"] = $_REQUEST["supvid"]; 
	$obrid = $_SESSION["obra"]["obrid"];
	$dados = $obras->Dados($obrid,$_SESSION["supvid"]);
	$dobras = new DadosObra($dados);
	
	$sql = "SELECT
				s.supvid
			FROM
				obras.supervisao s
			WHERE
				s.supdtinclusao = (select max(ss.supdtinclusao) from obras.supervisao ss where ss.obrid = ".$_SESSION["obra"]["obrid"]." AND supstatus = 'A')";	
	
	if($db->pegaUm($sql) != $_REQUEST["supvid"]){
		$somenteLeitura = "N";
	}
	
	$_SESSION["obrcustocontrato"] = $db->pegaUm("SELECT 
													 obrcustocontrato
												 FROM
													 obras.obrainfraestrutura
												 WHERE
													 obrid = {$_SESSION["obra"]["obrid"]}");
	
}

// Executa as funções da tela de acordo com suas ações
switch ($_REQUEST["requisicao"]){
	case "cadastra":
		// Cadastra a vistoria 
		if(!$_REQUEST['obrid']) {
			
			die("<script>
					alert('Erro de parametros!');
					window.location='obras.php?modulo=principal/vistoria&acao=A';
				 </script>");
			
		}
		$obras->CadastrarVistoria( $_REQUEST );
	break;
	case "atualiza":
		if(!$_REQUEST['supvid']) {
			
			die("<script>
					alert('Erro de parametros!');
					window.location='obras.php?modulo=principal/vistoria&acao=A';
				 </script>");
			
		}
		// Atualiza a vistoria
		$obras->AtualizarVistoria( $_REQUEST );
		
	break;
	
}

/* Limpar a variavel de sessão quando for cadastro (não carregar fotos da supervisão da sessão) */

if($requisicao == 'cadastra') {
	unset($_SESSION["supvid"]);
}

?>
<br/>
<?php

$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo = "Vistoria da Obra";
monta_titulo( $titulo_modulo, '' );

echo $obras->CabecalhoObras();

// Pega a situação da obra
$situacao_obra = obras_pega_situacao_vistoria( $_SESSION["obra"]["obrid"] );
?>

<link rel="stylesheet" href="css/obras.css" type="text/css">
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script>
	var valor_antigo = new Array();
</script>
<script src="../includes/prototype.js"  ></script>
<script src="/obras/js/vistoria.js" 	></script>
<form id="formulario" name="formulario" method="post" action="<?php echo $caminho_atual; ?>acao=A">

	<input type="hidden" name="situacaoatual" 			  id="situacaoatual" 			 value="<?php echo $situacao_obra; ?>" />
	<input type="hidden" name="requisicao" 				  								 value="<?php echo $requisicao; ?>" />
	<input type="hidden" name="obrcustocontrato" 		  id="obrcustocontrato" 		 value="<?php echo $_SESSION["obrcustocontrato"]; ?>" />
	<input type="hidden" name="obrdtinicio" 			  id="obtdtinicio" 				 value="<?php echo formata_data( $datasValidacao["obrdtinicio"] ); ?>" />
	<input type="hidden" name="obrdttermino" 			  id="obrdttermino" 			 value="<?php echo formata_data( $datasValidacao["obrdttermino"] ); ?>" />
	<input type="hidden" name="dtiniciolicitacao" 		  id="dtiniciolicitacao"   		 value="<?php echo formata_data( $datasValidacao["dtiniciolicitacao"] ); ?>" />
	<input type="hidden" name="dtfinallicitacao"          id="dtfinallicitacao" 		 value="<?php echo formata_data( $datasValidacao["dtfinallicitacao"] ); ?>" />
	<input type="hidden" name="fprdtiniciofaseprojeto"    id="fprdtiniciofaseprojeto" 	 value="<?php echo formata_data( $datasValidacao["fprdtiniciofaseprojeto"] ); ?>" />
	<input type="hidden" name="fprdtconclusaofaseprojeto" id="fprdtconclusaofaseprojeto" value="<?php echo formata_data( $datasValidacao["fprdtconclusaofaseprojeto"] ); ?>" />
	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td colspan="3">Dados da Vistoria</td>
		</tr>
<!--		<tr>-->
<!--			<td class="SubTituloDireita" width="190px;">Tipo de Vistoria</td>-->
<!--			<td>-->
				<?php
				
//					$sql = " SELECT
//								tpsid AS codigo,
//								tpsdsc AS descricao 
//							 FROM
//								obras.tiposupervisao";
//					$tpsid = $dobras->getTpsid();
//					$db->monta_combo("tpsid", $sql, 'S', "Selecione...", "", '', '', '', 'S', 'tpsid');
				?>
<!--			</td>-->
<!--		</tr>		-->
		<tr>
			<td class="SubTituloDireita">Data da Vistoria</td>
			<td>
				<?php $supvdt = $dobras->getSupvDt(); ?>
				<?= campo_data2( 'supvdt', 'S', $somenteLeitura, '', 'S','','validaDataVistoria();' ); ?>
			</td>
			<td rowspan="8" align="right" valign="top" width="1">
			<?php 
			// Barra de estado WORKFLOW
			wf_desenhaBarraNavegacao($docid, array('obrid' =>  $_SESSION["obra"]["obrid"], 'supvid' => $_SESSION["supvid"]));	
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nome do Vistoriador</td>
			<td>
				<?php
					$vistoriador = new Entidade($dobras->getSupVistoriador());
					$entnomevistoriador = $vistoriador->entnome;
					$entidvistoriador = $vistoriador->getPrimaryKey();
				?>
				<span id="entnomevistoriador"><?php echo $entnomevistoriador; ?></span>
			  	<input type="hidden" name="entidvistoriador" id="entidvistoriador" value="<? if( isset($_SESSION["obra"]["obrid"]) ) echo $entidvistoriador; ?>">
			  	<?php if($somenteLeitura == 'S' && $entnomevistoriador == ''){?>
				<input type="button" name="pesquisar_entidade" value="Pesquisar" style="cursor: pointer;" onclick="inserirVistoriador(document.getElementById('entidvistoriador').value);"/>
				<?php }?>
				<?php print obrigatorio(); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Situação atual</td>
			<td>
				<?php
				
					$sql = " SELECT
								ROUND(SUM(icopercexecutado), 2) 
							 FROM
								obras.itenscomposicaoobra 
							 WHERE 
							 	obrid = {$_SESSION["obra"]["obrid"]}";
				
					$percentual = $db->pegaUm( $sql );
					$stoid = $dado_situacao ? $dado_situacao : $dobras->getStoId();
					$sql   = obras_situacao_possivel($situacao_obra);
					 
					$db->monta_combo("stoid", $sql, $somenteLeitura, "Selecione...", "validasituacao(this.value,'1".possuiPerfil(array(PERFIL_SUPERVISORMEC, PERFIL_ADMINISTRADOR,PERFIL_SUPERVISORUNIDADE))."');verificasituacao", '', '', '', 'S', 'stoid');
				?>
				<span id="msg_paralisacao" style="display:none; color:#ee0000;">Favor preencher a aba <b>Restrições e Providências</b> com as informações sobre a paralisação.</span>
			</td>
		</tr>
		<tr style="display: none;" id="tr_elaboracao">
			<td class="SubTituloDireita">Necessita de Licenciamento Ambiental</td>
			<td>
				<?php 
					$obrlincambiental = $dobras->getObrLincAmbiental(); 
					
					if( $_REQUEST["supvid"] ){
						if ($obrlincambiental == "t"){
							echo "<input type=\"radio\" checked=\"checked\" name=\"obrlincambiental\" value=\"1\"> Sim";
							echo "<input type=\"radio\" name=\"obrlincambiental\" value=\"0\"> Não";
						} else {
							echo "<input type=\"radio\" name=\"obrlincambiental\" value=\"1\"> Sim";
							echo "<input type=\"radio\" checked=\"checked\" name=\"obrlincambiental\" value=\"0\"> Não";
						}	
					}else{
						echo "<input type=\"radio\" name=\"obrlincambiental\" value=\"1\"> Sim";
						echo "<input type=\"radio\" checked=\"checked\" name=\"obrlincambiental\" value=\"0\"> Não";
					}
				?>
			</td>
		</tr>
		<tr style="display: none;" id="tr_elaboracao1">
			<td class="SubTituloDireita"> Necessita de aprovação junto ao patrimônio histórico</td>
			<td>
				<?php 
					$obraprovpatrhist = $dobras->getObrAprovPatrHist(); 
					
					if( $_REQUEST["supvid"] ){
						if ($obraprovpatrhist == "t"){
							echo "<input type=\"radio\" checked=\"checked\" name=\"obraprovpatrhist\" value=\"1\"> Sim";
							echo "<input type=\"radio\" name=\"obraprovpatrhist\" value=\"0\"> Não";
						} else {
							echo "<input type=\"radio\" name=\"obraprovpatrhist\" value=\"1\"> Sim";
							echo "<input type=\"radio\" checked=\"checked\" name=\"obraprovpatrhist\" value=\"0\"> Não";
						}	
					}else{
						echo "<input type=\"radio\" name=\"obraprovpatrhist\" value=\"1\"> Sim";
						echo "<input type=\"radio\" checked=\"checked\" name=\"obraprovpatrhist\" value=\"0\"> Não";
					}
				?>
			</td>
		</tr>
		<tr style="display: none;" id="tr_elaboracao2">
			<td class="SubTituloDireita">Previsão de entrega do(s) projeto(s)</td>
			<td>
				<?php $obrdtprevprojetos = $dobras->getObrDtPrevProjetos(); ?>
				<?= campo_data2( 'obrdtprevprojetos', 'S', $somenteLeitura, '', 'S','','' ); ?>
			</td>
		</tr>
		<tr style="display: none;" id="tr_tplid">
			<td class="SubTituloDireita">Tipo de Paralisação</td>
			<td>
				<?php
					$tplid = $dobras->getTplId();
					$sql = "SELECT
								tplid as codigo,
								tpldsc as descricao
							FROM
								obras.tipoparalisacao";
					$db->monta_combo("tplid", $sql, $somenteLeitura, "Selecione...", '', '', '', '', 'S', "tplid");
				?>
			</td>
		</tr>
		<tr style="display: none;" id="tr_hprobs">
			<td class="SubTituloDireita">Observações da Paralisação</td>
			<td>
				<?php
					$hprobs1 = $dobras->getHprObs();
					echo campo_textarea( 'hprobs1', 'N', 'S', '', '70', '8', '1000', '' , 0, '', '', false, '', ''); 
				?>
			</td>
		</tr>
		<tr id="tr1" style="display: none;">
			<td class="SubTituloDireita">Projeto/Especificações</td>
			<td>
				<?php 
					if($somenteLeitura != 'S'){
						$disabled ="disabled"; 
					}
					$supprojespecificacoes = $dobras->getSupProjEspecificacoes(); 
					
					if($_REQUEST["supvid"]){
						if ($supprojespecificacoes == "t"){
							echo "<input type=\"radio\" $disabled checked=\"checked\" name=\"supprojespecificacoes\" value=\"1\"> Sim";
							echo "<input type=\"radio\" $disabled name=\"supprojespecificacoes\" value=\"0\"> Não";
						} else {
							echo "<input type=\"radio\" $disabled name=\"supprojespecificacoes\" value=\"1\"> Sim";
							echo "<input type=\"radio\" $disabled checked=\"checked\" name=\"supprojespecificacoes\" value=\"0\"> Não";
						}	
					}else{
						echo "<input type=\"radio\"  name=\"supprojespecificacoes\" value=\"1\"> Sim";
						echo "<input type=\"radio\"  checked=\"checked\" name=\"supprojespecificacoes\" value=\"0\"> Não";
					}
				?>
			</td>
		</tr>
		<tr id="tr2" style="display: none;">
			<td class="SubTituloDireita">Placa da Obra</td>
			<td>
				<?php 
					$supplacaobra = $dobras->getSupPlacaObra(); 
					
					if($_REQUEST["supvid"]){
						if ($supplacaobra == "t"){
							echo "<input type=\"radio\" $disabled checked=\"checked\" name=\"supplacaobra\" value=\"1\"> Sim";
							echo "<input type=\"radio\" $disabled name=\"supplacaobra\" value=\"0\"> Não";
						} else {
							echo "<input type=\"radio\" $disabled name=\"supplacaobra\" value=\"1\"> Sim";
							echo "<input type=\"radio\" $disabled checked=\"checked\" name=\"supplacaobra\" value=\"0\"> Não";
						}	
					}else{
						echo "<input type=\"radio\"  name=\"supplacaobra\" value=\"1\"> Sim";
						echo "<input type=\"radio\"  checked=\"checked\" name=\"supplacaobra\" value=\"0\"> Não";
					}
				?>
			</td>
		</tr>
		<tr id="tr3" style="display: none;">
			<td class="SubTituloDireita">Diário da Obra Atualizado</td>
			<td>
				<?php 
					$supdiarioobra = $dobras->getSupDiarioObra();
					
					if($_REQUEST["supvid"]){
						if ($supdiarioobra == "t"){
							echo "<input type=\"radio\" $disabled checked=\"checked\" name=\"supdiarioobra\" value=\"1\"> Sim";
							echo "<input type=\"radio\" $disabled name=\"supdiarioobra\" value=\"0\"> Não";
						} else {
							echo "<input type=\"radio\" $disabled name=\"supdiarioobra\" value=\"1\"> Sim";
							echo "<input type=\"radio\" $disabled checked=\"checked\" name=\"supdiarioobra\" value=\"0\"> Não";
						}	
					}else{
						echo "<input type=\"radio\" name=\"supdiarioobra\" value=\"1\"> Sim";
						echo "<input type=\"radio\" checked=\"checked\" name=\"supdiarioobra\" value=\"0\"> Não";
					}
				?>
			</td>
		</tr>
		
		<!-- 
		
		<tr id="tr4" style="display: none;">
			<td class="SubTituloDireita">Placa Indicativa do <br /> Programa/Dados da obra</td>
			<td>
				<?php 
					$supplacalocalterreno = $dobras->getSupPlacaLocalTerreno();

					if($_REQUEST["supvid"]){
						if ($supplacalocalterreno == "t"){
							echo "<input type=\"radio\" checked=\"checked\" name=\"supplacalocalterreno\" value=\"1\"> Sim";
							echo "<input type=\"radio\" name=\"supplacalocalterreno\" value=\"0\"> Não";
						} else {
							echo "<input type=\"radio\" name=\"supplacalocalterreno\" value=\"1\"> Sim";
							echo "<input type=\"radio\" checked=\"checked\" name=\"supplacalocalterreno\" value=\"0\"> Não";
						}	
					}else{
						echo "<input type=\"radio\" name=\"supplacalocalterreno\" value=\"1\"> Sim";
						echo "<input type=\"radio\" checked=\"checked\" name=\"supplacalocalterreno\" value=\"0\"> Não";
					}
				?>
			</td>
		</tr>
		<tr id="tr5" style="display: none;">
			<td class="SubTituloDireita">Validade do Alvará da Obra</td>
			<td>
				<?php 
					$supvalidadealvara = $dobras->getSupValidadeAlvara();
					
					if($_REQUEST["supvid"]){
						if ($supvalidadealvara == "t"){
							echo "<input type=\"radio\" checked=\"checked\" name=\"supvalidadealvara\" value=\"1\"> Sim";
							echo "<input type=\"radio\" name=\"supvalidadealvara\" value=\"0\"> Não";
						} else {
							echo "<input type=\"radio\" name=\"supvalidadealvara\" value=\"1\"> Sim";
							echo "<input type=\"radio\" checked=\"checked\" name=\"supvalidadealvara\" value=\"0\"> Não";
						}	
					}else{
						echo "<input type=\"radio\" name=\"supvalidadealvara\" value=\"1\"> Sim";
						echo "<input type=\"radio\" checked=\"checked\" name=\"supvalidadealvara\" value=\"0\"> Não";
					}
				?>
			</td>
		</tr>
		
		-->
		
		<tr id="tr6" style="display: none;">
			<td class="SubTituloDireita">Qualidade de Execução da Obra/Projeto</td>
			<td>
				<?php
				$qlbid = $dobras->getQlbId();
				$sql = "SELECT 
							qlbid as codigo, 
							qlbdesc as descricao 
						FROM 
							obras.qualidadeobra"; 
				$db->monta_combo("qlbid", $sql, $somenteLeitura, "Selecione...", '', '', '', '100', 'S');
				
				?>
			</td>
		</tr>
		<tr id="tr7" style="display: none;">
			<td class="SubTituloDireita">Desempenho da Construtora/Projetista</td>
			<td>
				<?php
				$dcnid = $dobras->getDcnId();
				$sql = "SELECT 
							dcnid as codigo, 
							dcndesc as descricao 
						FROM 
							obras.desempenhoconstrutora"; 
				$db->monta_combo("dcnid", $sql, $somenteLeitura, "Selecione...", '', '', '', '100', 'S');
				
				?>
			</td>
		</tr>
		<tr id="tr8" style="display: none;">
			<td colspan="3">Detalhamento de Supervisão e Acompanhamento</td>
		</tr>
		<tr id="tr9" style="display: none;">
			<td colspan="3">
				<table class="listagem" width="100%">
					<thead>
						<tr>
							<td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Item da Obra</b></td>
							<td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Valor (R$)</b></td>
							<td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) Sobre a Obra</b></td>
							<td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Data de Início</b></td>
							<td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>Data de Término</b></td>
							<td colspan="2" rowspan="1" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title" ><b>Última Supervisão</b></td>
							<td colspan="2" rowspan="1" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title" ><b>Supervisão Atual</b></td>
						</tr>
						<tr>
							<td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) do Item já Executado</b></td>
							<td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) do Item já Executado sobre a Obra</b></td>
							<td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) Supervisão</b></td>
							<td colspan="1" rowspan="2" valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" class="title"><b>(%) do Item já Executado sobre a Obra após Supervisão</b></td>
						</tr>
					</thead>
					<tbody>
						<?php
							$supvid = $_REQUEST["supvid"] ? $_REQUEST["supvid"] : 'NULL';
							
							// Função que lista os itens da supervisão.
							obras_listaitensvistoria( $supvid );
						?>
					</tbody>
				</table>
			</td>
		</tr>
		<tr>
			<!--<td class="SubTituloDireita">Observação da Vistoria</td>-->
			<td class="SubTituloDireita">Relatório Técnico da Vistoria</td>
			<td colspan="2">
				<?php 
				$supobs = $dobras->getSupObs(); 
				$desc_relatorio = "No relatório deverão constar informações pertinentes a execução da obra, tais como:<br/><br/>&nbsp;&nbsp;&nbsp;<b>a.</b> O nome do Responsável Técnico da obra e o número da ART de execução.<br/>&nbsp;&nbsp;&nbsp;<b>b.</b> A existência ou não de todos os documentos necessários para a execução da obra, tais como: projetos, especificações, memoriais, ART´s, licenças, alvarás, caderno de encargos, diários de obras (preenchido e atualizado), dentre outros.<br/>&nbsp;&nbsp;&nbsp;<b>c.</b> Informações sobre o canteiro de obra (existência, organização e segurança do Trabalho).<br/>&nbsp;&nbsp;&nbsp;<b>d.</b> Atrasos na execução da obra e a necessidade ou não de aditivos.<br/>&nbsp;&nbsp;&nbsp;<b>e.</b> Comentário sobre a situação geral da obra e/ou de cada serviço da obra.<br/>&nbsp;&nbsp;&nbsp;<b>f.</b> Outros apontamentos.";
				?>
				<?= campo_textarea( 'supobs', 'S', $somenteLeitura, '', '100', '8', '5000','','',$desc_relatorio); ?>
			</td>
		</tr>
		<?php
			if($_REQUEST["supvid"]){
				$sql = "select fot.*, arq.arqdescricao from obras.fotos AS fot
						left join public.arquivo AS arq ON arq.arqid = fot.arqid
						where obrid =".$_SESSION["obra"]["obrid"]." AND supvid=".$_REQUEST["supvid"]." ORDER BY fotordem;";
				$fotos = ($db->carregar($sql));
			}
		?>
		<tr id="tr10" style="display: none;">
			<td colspan="2" align="center">
			</td>
		</tr>
	</table>
	<input type="hidden" name="supvid" value="<?php echo $_REQUEST["supvid"]; ?>"/>
	<input type="hidden" name="obrid" value="<? echo $_SESSION["obra"]["obrid"]; ?>">
	<input type="hidden" name="percexec" value="<? echo $percentual; ?>">
	<input type="hidden" name="totalvalor" id="totalvalor" value="<?php echo $_SESSION["obras"]["totalvalor"]; ?>" />
	<input type="hidden" name="hdn_fotos_galeria" id="hdn_fotos_galeria" value="" />
	<input type="hidden" name="hdn_fotos_supervisao" id="hdn_fotos_supervisao" value="" />
</form>


<?php if ( $dobras->getStoId() == PARALIZADA ){ ?>
		
	<script>
		if (document.selection){
			document.getElementById('tr_tplid').style.display = 'block';
			document.getElementById('tr_hprobs').style.display = 'block';
		}else{
			document.getElementById('tr_tplid').style.display = 'table-row';
			document.getElementById('tr_hprobs').style.display = 'table-row';
		}
	</script>
		
<?php }else if ( $dobras->getStoId() == EM_ELABORACAO_DE_PROJETOS ) { ?>

	<script>
		if (document.selection){
			document.getElementById('tr_elaboracao').style.display  = 'block';
			document.getElementById('tr_elaboracao1').style.display = 'block';
			document.getElementById('tr_elaboracao2').style.display = 'block';
		}else{
			document.getElementById('tr_elaboracao').style.display  = 'table-row';
			document.getElementById('tr_elaboracao1').style.display = 'table-row';
			document.getElementById('tr_elaboracao2').style.display = 'table-row';
		}
	</script>

<?php }

print "<script>verificasituacao({$situacao_obra});</script>";

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<script>
	jQuery.noConflict();
	
<?php if(possuiPerfil( array(PERFIL_SUPERVISORMEC, PERFIL_ADMINISTRADOR, PERFIL_EMPRESA, PERFIL_SUPERVISORUNIDADE) )){ ?>
	jQuery(function() {
		jQuery( "#fotos_supervisao" ).sortable({
			placeholder: "draggable_space",
			connectWith: "#fotos_supervisao",
			cancel: ".nodraggable"
		});
		jQuery( "#fotos_galeria" ).droppable({
			drop: function( event, ui ) {
				if(!jQuery("#s_" + ui.draggable.attr("id")).html()){
					jQuery( "<li id=\"s_" + ui.draggable.attr("id") + "\" class=\"nodraggable\" ><img src=\"../imagens/fechar.jpeg\" title=\"Remover Foto\" class=\"fechar\" onclick=\"removerFotoGaleria('" + ui.draggable.attr("id") + "')\" />" + ui.draggable.html() + "</li>" ).appendTo( this );
					jQuery("#s_" + ui.draggable.attr("id") + " .img_foto" ).attr("style","margin-top:-15px");
					jQuery("#" + ui.draggable.attr("id") ).attr("class","draggable f_selected");

				}
			}
		}).sortable({
			cancel: ".nodraggable"
		});
		
	});

	function removerFotoGaleria(foto)
	{
		jQuery("#s_" + foto ).remove();
		jQuery("#" + foto ).attr("class","draggable");
	}
<?php } ?>
	
	function abrirGaleria(arqid,pagina)
	{
		window.open("../slideshow/slideshow/index.php?pagina=" + pagina + "&arqid=" + arqid,"imagem","width=850,height=600,resizable=yes");
	}
	
	function salvarFotos()
	{
		jQuery("[name=btn_salvar_fotos]").val("Carregando...");
		jQuery("[name=btn_salvar_fotos]").attr("disabled","disabled");
		var arrFotosSupervisao = jQuery( "#fotos_supervisao").sortable( "serialize" , {key:'fotosSupervisao[]'} );
		var arrFotosGaleria    = jQuery( "#fotos_galeria").sortable( "serialize" , {key:'fotosGaleria[]'} );
		var url = window.location + "&ajaxFotosGaleria=true&" + arrFotosSupervisao + "&" + arrFotosGaleria;
		jQuery.ajax({
		  	url: url,
		  	success: function(data) {
		    if(data == "true"){
		    	alert("Operação realizada com sucesso!");
		    	jQuery("[name=btn_salvar_fotos]").val("Salvar Fotos");
				jQuery("[name=btn_salvar_fotos]").attr("disabled","");
		    }else{
		    	alert("Não foi possível realizar a operação!");
		    	jQuery("[name=btn_salvar_fotos]").val("Salvar Fotos");
				jQuery("[name=btn_salvar_fotos]").attr("disabled","");
		    }
		  }
		});
	}
	
</script>
<style>
	.div_fotos { list-style-type: none; margin: 0; padding: 0;padding-top:3px}
	.div_fotos li { font-size: 1.2em; height: 110px; height:90px;padding: 1px}
	html>body .div_fotos li { height: 80px; line-height: 1.2em; }
	.field_fotos{padding:10px;}
	.div_fotos{height:300px;overflow:auto;}
	.draggable{width:110px;height:90px;margin:3px;border:solid 1px black;float:left;cursor:move;text-align:center;background-color:#FFFFFF}
	.nodraggable{width:110px;height:90px;margin:3px;border:solid 1px black;float:left;text-align:center;background-color:#FFFFFF}
	.draggable_space{line-height: 1.2em;width:110px;height:90px;margin:3px;float:left;cursor:pointer;background-color:#CCCCCC}
	.f_selected{border: solid 1px red;}
	.fechar{position:relative;margin-left:105px;top:-8px;cursor:pointer}
	.img_foto{z-index:2;}
	.img_class{margin-top:-15px;}
</style>
<?php 
$_SESSION['downloadfiles']['pasta'] = array("origem" => "obras","destino" => "obras");
$_SESSION['imgparams'] = array("filtro" => "cnt.obrid =".$_SESSION["obra"]["obrid"]." AND cnt.supvid=".$_REQUEST["supvid"], "tabela" => "obras.fotos");

if(!$_REQUEST['supvid']){
	$sql = "SELECT
				arq.arqid
			FROM 
				public.arquivo arq
			INNER JOIN 
				obras.arquivosobra oar ON arq.arqid = oar.arqid
			INNER JOIN 
				obras.obrainfraestrutura obr ON obr.obrid = oar.obrid 
			INNER JOIN 
				seguranca.usuario seg ON seg.usucpf = oar.usucpf
			INNER JOIN
				obras.supervisao supv ON supv.supvid = oar.obrid
			WHERE 
				obr.obrid = {$_SESSION["obra"]["obrid"]}
			AND
				aqostatus = 'A'
			AND
				tpaid = ".TIPO_ARQUIVO_FOTO_VISTORIA."
			AND
				(arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png') 
			ORDER BY 
				arq.arqid";
}else{
	$sql = "SELECT DISTINCT
				arq.arqid
			FROM 
				obras.fotos fot
			INNER JOIN
				public.arquivo arq ON arq.arqid = fot.arqid
			INNER JOIN 
				obras.arquivosobra oar ON arq.arqid = oar.arqid
			WHERE 
				fot.obrid = {$_SESSION["obra"]["obrid"]}
				AND fot.supvid = {$_REQUEST['supvid']}
				AND oar.tpaid = ".TIPO_ARQUIVO_FOTO_VISTORIA."
				AND oar.aqostatus = 'A'
			ORDER BY 
				arq.arqid";
}
$arrFotosGaleria = $db->carregar($sql);
// array que armazenará o arqid da Galeria
$arrArqidGaleria = array();
if(is_array($arrFotosGaleria)){
	foreach($arrFotosGaleria as $foto){
		$arrArqidGaleria[] = $foto['arqid'];
	}
}
?>
<?php if( 1==1 ){  ?>
	<table class="tabela" id="tbl_graggable" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td bgcolor="#DCDCDC" colspan="2">
				<b>Fotos de Vistoria</b>
				<?php if($_REQUEST["supvid"]): ?>
					*Arraste as imagens para a área de 'Fotos da Galeria' para adicioná-las à galeria. Após adicioná-las, clique no botão 'Salvar' para confirmar as alterações.
				<?php else: ?>
					*Após adicionar as fotos, clique no botão 'Salvar' para confirmar as alterações.
				<?php endif; ?>
			</td>
		</tr>
		<tr>
			<td <?php echo $_REQUEST["supvid"]? "width=\"50%\"" : "colspan='2'" ?> >
				<fieldset class="field_fotos">
					<legend>Fotos da Supervisão</legend>
					<ul id="fotos_supervisao" class="div_fotos">
						<?php if(is_array($fotos)): $arrFotos = $fotos;?>
							<?php $n = 1 ?>
							<?php foreach($arrFotos as $foto): ?>
								<?php $pagina = floor($n/16); ?>
								
								<?php $sql = "SELECT arqtipo, arqid  FROM public.arquivo 
											WHERE arqid = '". $foto['arqid'] ."'";
									$dados = $db->pegaLinha($sql);
									
									$imgend = APPRAIZ.'arquivos/'.(($_REQUEST["_sisarquivo"])?$_REQUEST["_sisarquivo"]:$_SESSION["sisarquivo"]).'/'. floor($foto['arqid']/1000) .'/'.$foto['arqid'];
									
									if(is_file($imgend)){
										$img_max_dimX = 100;
										$img_max_dimY = 85;
										
										$imginfo = getimagesize($imgend);
										
										$width = $imginfo[0];
										$height = $imginfo[1];
									
										if (($width >$img_max_dimX) or ($height>$img_max_dimY)){
											if ($width > $height){
											  	$w = $width * 0.9;
												  while ($w > $img_max_dimX){
													  $w = $w * 0.9;
												  }
												  $w = round($w);
												  $h = ($w * $height)/$width;
											  }else{
												  $h = $height * 0.9;
												  while ($h > $img_max_dimY){
													  $h = $h * 0.9;
												  }
												  $h = round($h);
												  $w = ($h * $width)/$height;
											  }
										}else{
											  $w = $width;
											  $h = $height;
										}
										
										$tamanho = " width=\"$w\" height=\"$h\" ";
									}else{
										$tamanho = "";
									}
								?>
								<li id="foto_<?php echo $foto['arqid']?>" class="draggable<?php echo in_array($foto['arqid'],$arrArqidGaleria) ? " f_selected" : ""?>">
									<img <?php echo $tamanho;?> class="img_foto" ondblclick="abrirGaleria('<?php echo $foto['arqid'] ?>','<?php echo $pagina ?>')" src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $foto['arqid'] ?>&newwidth=100&newheight=85" />
								</li>
								<?php $n++ ?>
							<?php endforeach; ?>
						<?php endif; ?>
					</ul>
				</fieldset>
			</td>
			<?php if($_REQUEST["supvid"]): ?>
				<td>
					<fieldset class="field_fotos">
						<legend>Fotos da Galeria</legend>
						<ul id="fotos_galeria" class="div_fotos">
							<?php if(is_array($arrFotosGaleria)):?>
								<?php $n = 1 ?>
								<?php foreach($arrFotosGaleria as $foto): ?>
									<?php $pagina = floor($n/16); ?>
									
									<?php $sql = "SELECT arqtipo, arqid  FROM public.arquivo 
												WHERE arqid = '". $foto['arqid'] ."'";
										$dados = $db->pegaLinha($sql);
										
										$imgend = APPRAIZ.'arquivos/'.(($_REQUEST["_sisarquivo"])?$_REQUEST["_sisarquivo"]:$_SESSION["sisarquivo"]).'/'. floor($foto['arqid']/1000) .'/'.$foto['arqid'];
										
										if(is_file($imgend)){
											$img_max_dimX = 100;
											$img_max_dimY = 85;
											
											$imginfo = getimagesize($imgend);
											
											$width = $imginfo[0];
											$height = $imginfo[1];
										
											if (($width >$img_max_dimX) or ($height>$img_max_dimY)){
												if ($width > $height){
												  	$w = $width * 0.9;
													  while ($w > $img_max_dimX){
														  $w = $w * 0.9;
													  }
													  $w = round($w);
													  $h = ($w * $height)/$width;
												  }else{
													  $h = $height * 0.9;
													  while ($h > $img_max_dimY){
														  $h = $h * 0.9;
													  }
													  $h = round($h);
													  $w = ($h * $width)/$height;
												  }
											}else{
												  $w = $width;
												  $h = $height;
											}
											
											$tamanho = " width=\"$w\" height=\"$h\" ";
										}else{
											$tamanho = "";
										}
									?>
									
									<li id="s_foto_<?php echo $foto['arqid']?>" class="nodraggable">
										<img src="../imagens/fechar.jpeg" title="Remover Foto" class="fechar" onclick="removerFotoGaleria('foto_<?php echo $foto['arqid'] ?>')" />
										<img <?php echo $tamanho;?> class="img_class" ondblclick="abrirGaleria('<?php echo $foto['arqid'] ?>','<?php echo $pagina ?>')" src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $foto['arqid'] ?>&newwidth=100&newheight=85" />
									</li>
									<?php $n++ ?>
								<?php endforeach; ?>
							<?php endif; ?>
						</ul>
					</fieldset>
				</td>
			<?php endif; ?>
		</tr>
		<tr>
			<td bgcolor="#c0c0c0" colspan="2" align="center">
				<?php //if($habilitado && $somenteLeitura == 'S' || possuiPerfil(PERFIL_SUPERVISORMEC)){ ?>
					<input <?=(($habilitado)? "" :"disabled=\"disabled\""); ?> type="button" name="add_fotos" onmouseover="return escape('Gerenciamento de fotos da vistoria (inserir, alterar e remover).');" value="Adicionar Fotos" onclick="ImageComponent(<? if(isset($_REQUEST['supvid'])){ echo "'?funcao=AtualizaFotos&supvid=".$_REQUEST["supvid"]."'";}else{ echo "' '";}?>);">
					<?php //if(1==2): ?>
					<?php //if($_REQUEST['supvid']): ?>
					<input <?=(($habilitado)? "" :"disabled=\"disabled\""); ?> type="button" name="btn_salvar_fotos" onmouseover="return escape('Salvar as alterações das fotos de supervisão e galeria.');" value="Salvar Fotos" onclick="salvarFotos()">
					<?php //endif; ?>
				<?php //} ?>
			</td>
		</tr>
		<tr>
			<td bgcolor="#c0c0c0" colspan="2" align="center">
					<?php if($habilitado && $somenteLeitura == 'S' || possuiPerfil(PERFIL_SUPERVISORMEC)){ ?>
						<input type="button" value="Salvar" id="salva_vistoria" style="cursor: pointer" onclick="enviaFormulario('1<?=possuiPerfil(array(PERFIL_SUPERVISORMEC, PERFIL_ADMINISTRADOR,PERFIL_SUPERVISORUNIDADE,PERFIL_EMPRESA));?>');">
					<?php } ?> 
					<input type="button" value="Voltar" style="cursor: pointer" onclick="history.back(-1);">
			</td>
		</tr>
	</table>
<?php } ?>