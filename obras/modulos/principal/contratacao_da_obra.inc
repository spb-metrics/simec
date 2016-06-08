<?php

if ( $_REQUEST['requisicao'] == 'excluirAditivo' ){
	apagaAditivo( $_GET['traid'], $_SESSION['obra']['obrid'] );
	
	// atualizando o campo obrvlrrealobra da tabela obras.obrainfraestrutura
	$sql = "UPDATE obras.obrainfraestrutura set obrvlrrealobra=".pegaObMaiorVlrAditivo()." WHERE obrid={$_SESSION['obra']['obrid']}";
	$db->carregar($sql);
	$db->commit();
	
	die("<script>
			alert('Operação realizada com sucesso!');
			window.location.href = '?modulo=principal/contratacao_da_obra&acao=A';
	 	 </script>");
}

if ( $_REQUEST['requisicao'] == 'vincularobranova' ){
	obras_cria_nova_obra( $_REQUEST['obrid'] );
}

if ( $_REQUEST["requisicao"] == "calcula" ){
	echo obrasCalculaDias( $_REQUEST["termino"], $_REQUEST["inicio"] );
	die;	
}

if($_REQUEST["ajax"] == 1){
	
	echo obras_buscaconvenio($_REQUEST["convenio"]);
	die;
	
}

if($_REQUEST["ajax"] == 2){
	
	$dados_aditivo = "SELECT
					  	covano, covobjeto, covprocesso,
					  	to_char(covdtinicio, 'DD/MM/YYYY') as inicio, 
					  	to_char(covdtfinal, 'DD/MM/YYYY') as fim, 
					  	covvalor
					  FROM
					  	obras.conveniosobra
					  WHERE
					  	covnumero = '" . $_REQUEST["covnumero"] . "' AND
					  	covtipo = 'A'";
							
	$cabecalho = array("Ano", "Objeto", "Processo", "Data de Início", "Data de Término", "Valor" );
	$db->monta_lista_simples( $dados_aditivo, $cabecalho, 5, 10, 'N', '90%', '' );
	die;
	
}

if($_REQUEST["ajax"] == 3){

	$dados_apostilamento = "SELECT
							 	covano, covobjeto, covprocesso,
							  	to_char(covdtinicio, 'DD/MM/YYYY') as inicio, 
							  	to_char(covdtfinal, 'DD/MM/YYYY') as fim, 
							  	covvalor
							FROM
							  	obras.conveniosobra
							WHERE
							  	covnumero = '" . $_REQUEST["covnumero"] . "' AND
							  	covtipo = 'P'";
	
	$cabecalho = array("Ano", "Objeto", "Processo", "Data de Início", "Data de Término", "Valor" );
	$db->monta_lista_simples( $dados_apostilamento, $cabecalho, 5, 10, 'N', '90%', '' );
	die;
	
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php'; 
require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

// Pega o caminho atual do usuário (em qual módulo se encontra)
$caminho_atual   = $_SERVER["REQUEST_URI"];
$posicao_caminho = strpos($caminho_atual, '&acao');
$caminho_atual   = substr($caminho_atual, 0 , $posicao_caminho);

$obras = new Obras();
$dobras = new DadosObra(null);

if($_SESSION["obra"]["obrid"]){
	$dados = $obras->Dados($_SESSION["obra"]["obrid"]);
	$dobras = new DadosObra($dados);
} else {
	die("<script>alert('Variavel da obra não encontrado.');window.location='obras.php?modulo=inicio&acao=A';</script>");	  
}

// Executa as funcionalidades da tela
if ($_REQUEST['requisicao'] == 'executa'){
	$obras->CadastrarContratacaoObras($_REQUEST); 	
}

?>

<br/>

<?php

$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo = "Contratação da Obra";
monta_titulo( $titulo_modulo, "<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /> Indica os campos obrigatórios");

echo $obras->CabecalhoObras();

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script src="../includes/prototype.js"></script>
<script src="../includes/entidades.js"></script>
<script src="../includes/calendario.js"></script>
<script type="text/javascript" src="/includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="/includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="/includes/ModalDialogBox/ajax.js"></script>
<script type="text/javascript"><!--

messageObj = new DHTML_modalMessage();	// We only create one object of this class
messageObj.setShadowOffset(5);	// Large shadow

function displayMessage(url){
	
	messageObj.setSource(url);
	messageObj.setCssClassMessageBox(false);
	messageObj.setSize(400,233);
	messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
	messageObj.display();
}

function displayStaticMessage(messageContent,cssClass){
	messageObj.setHtmlContent(messageContent);
	messageObj.setSize(400,250);
	messageObj.setCssClassMessageBox(cssClass);
	messageObj.setSource(false);	// no html source since we want to use a static message here.
	messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
	messageObj.display();
	
	
}

function closeMessage(){
	messageObj.close();	
}

function abreInauguracao(valor){

	var inaugurada = document.getElementById('inaugurada');
	var dataprev   = document.getElementById("dtprevisao");
	var datain 	   = document.getElementById("dtinauguracao");
	
	if(valor == 1 || valor == 3){
		if (document.selection){
			inaugurada.style.display = 'block';
		}else{
			inaugurada.style.display = 'table-row';
		}
	}else{
		inaugurada.style.display = 'none';
		dataprev.style.display = 'none';
		datain.style.display   = 'none';
	}

}

function abreData(valor){
	
	var dataprev 			 = document.getElementById("dtprevisao");
	var datain 				 = document.getElementById("dtinauguracao");
	//var obrdtprevinauguracao = document.getElementById("obrdtprevinauguracao");
	//var obrdtinauguracao 	 = document.getElementById("obrdtinauguracao");
	
	if(valor == '' || valor == 'S'){
		
		obrdtinauguracao.value	 = '';
		obrdtprevinauguracao.value = '';
		
		if (document.selection){
			dataprev.style.display = 'none';
			datain.style.display   = 'none';	
		}else{
			dataprev.style.display = 'none';
			datain.style.display   = 'none';
		}
	}
	
	if(valor == 'N'){
		
		obrdtinauguracao.value	 = '';
	
		if (document.selection){
			dataprev.style.display = 'block';
			datain.style.display   = 'none';	
		}else{
			dataprev.style.display = 'table-row';
			datain.style.display   = 'none';
		}
	}
	
	if(valor == 'I'){
	
		obrdtprevinauguracao.value = '';
	
		if (document.selection){
			dataprev.style.display = 'none';
			datain.style.display   = 'block';	
		}else{
			dataprev.style.display = 'none';
			datain.style.display   = 'table-row';
		}
	}
}
	
function RemoveLinha(index,id){
	table = window.document.getElementById("faseslicitacao");
	var conteiner = window.document.getElementById("conteinerAcao");
	conteiner.innerHTML += "<input type='hidden' name='acaoFases[]' id='acaoFases"+id+"' value='"+id+"'/>";
	table.deleteRow(index);
}			


function verDataHomologadao( superuser ){
	
	if( document.formulario.flchomlicdtprev.value == '' && (superuser == 1) ){
		alert('Para inserir a Data de Assinatura do Contrato é necessário cadastrar a Data de Homologação da Licitação!');
		return false;
	}
	
	return true;
	
}

function validaRec( superuser ){
	
	var obData = new Data();
	
	if( verDataHomologadao( superuser ) ){
	
		
		var mensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
		var validacao = true;

		var entidempresa = document.formulario.entidempresa.value;
		if (entidempresa == ''){
			mensagem += 'Empresa Contratada \n';
			validacao = false;
		}
		
		var obrdtassinaturacontrato = document.formulario.obrdtassinaturacontrato.value;
		if (obrdtassinaturacontrato == ''){
			mensagem += 'Data de Assinatura do Contrato \n';
			validacao = false;
		}else{
			if (!validaData(document.formulario.obrdtassinaturacontrato)){
				alert("A data informada é inválida");
				document.formulario.obrdtassinaturacontrato.focus();
				return false;
			}
		}
		
		var obrprazovigencia = document.formulario.obrprazovigencia.value;
		if (obrprazovigencia == ''){
			mensagem += 'Prazo de Vigência do Contrato \n';
			validacao = false;
		}
		
		var obrdtordemservico = document.formulario.obrdtordemservico.value;
		if (obrdtordemservico == ''){
			mensagem += 'Data da Ordem de Serviço \n';
			validacao = false;
		}else{
			if (!validaData(document.formulario.obrdtordemservico)){
				alert("A data informada é inválida");
				document.formulario.obrdtordemservico.focus();
				return false;
			}
		}
		
		var obrdtinicio = document.formulario.obrdtinicio.value;
		if (obrdtinicio == ''){
			mensagem += 'Início de Execução da Obra \n';
			validacao = false;
		}
		
		var obrprazoexec = document.formulario.obrprazoexec.value;
		if (obrprazoexec == ''){
			mensagem += 'Prazo de Execução \n';
			validacao = false;
		}
		
		var obrcustocontrato = document.formulario.obrcustocontrato.value;
		if (obrcustocontrato == ''){
			mensagem += 'Valor Contratado da Obra \n';
			validacao = false;
		}
		
		var obrqtdconstruida = document.formulario.obrqtdconstruida.value;
		if (obrqtdconstruida == ''){
			mensagem += 'Área / Quantidade a ser Construída \n';
			validacao = false;
		}
		
		var frpid = document.formulario.frpid.value;
		if (frpid == ''){
			mensagem += 'Tipo de Origem de Recursos \n';
			validacao = false;
		}

		if( (document.formulario.obrdtassinaturacontrato.value != "") && (document.formulario.flchomlicdtprev.value != "") ){
			if( !obData.comparaData( document.formulario.obrdtassinaturacontrato.value, document.formulario.flchomlicdtprev.value, ">=" ) ){
				alert( "A Data de Assinatuda do Contrato deve ser igual ou posterior a Data de Homologação da Licitação! ( "+document.formulario.flchomlicdtprev.value+" )" );
				return false;
			}
		}

		if( ( document.formulario.obrdtordemservico.value != "" ) && ( document.formulario.obrdtassinaturacontrato.value != "" ) ){
			if( !obData.comparaData( document.formulario.obrdtordemservico.value, document.formulario.obrdtassinaturacontrato.value, ">=" ) ){
				alert( "A Data de Ordem de Serviço deve ser igual ou posterior a Data de Assinatura do Contrato! ( "+document.formulario.obrdtassinaturacontrato.value+" ) " );
				return false;
			}
		}

		if( ( document.formulario.obrdtinicio.value != "" ) && ( document.formulario.obrdtordemservico.value != "" ) ){
			if( !obData.comparaData( document.formulario.obrdtinicio.value, document.formulario.obrdtordemservico.value, ">=" ) ){
				alert( "A Data de Início da Execução da Obra deve ser igual ou posterior a Data de Ordem de Serviço! ( "+document.formulario.obrdtordemservico.value+" ) " );
				return false;
			}
		}

		if( ( document.formulario.obrdttermino.value != "" ) && ( document.formulario.dtterminocontrato.value != "" ) ){
			if( !obData.comparaData( document.formulario.obrdttermino.value, document.formulario.dtterminocontrato.value, "<=" ) ){
				alert( "A Data de Término da Execução da Obra deve ser menor ou igual a Data de Término do Contrato! ( "+ document.formulario.dtterminocontrato.value+" ) " );
				return false;
			}
		}
		
		if (document.formulario.obrdtinicio.value != ""){
			if (!validaData(document.formulario.obrdtinicio)){
				alert("A data de início informada é inválida");
				document.formulario.obrdtinicio.focus();
				return false;
			}
		}
		
		if (document.formulario.obrdttermino.value != ""){
			if (!validaData(document.formulario.obrdttermino)){
				alert("A data de termino informada é inválida");
				document.formulario.obrdttermino.focus();
				return false;
			}
		}	
		
		if (document.formulario.obrdtinicio.value != "" && document.formulario.obrdttermino.value != ""){
			if (!validaDataMaior(document.formulario.obrdtinicio, document.formulario.obrdttermino)){
				alert("A data de término não deve ser maior do que a de início da obra.");
				document.formulario.obrdtinicio.focus();
				return false;
			}
		}

		var frpid = document.getElementById('frpid');
		var covnumero = document.getElementById('covnumero');

		if(frpid.value == '2' || frpid.value == '7'){
			if(covnumero.value == ''){
				alert('campo obrigatório.');
				covnumero.focus();
				return false;
			}
		}
		
		if( !validacao && ( superuser == 1 ) ){
			alert( mensagem );
			return validacao;
		} else{
			document.formulario.submit();
		}
		
	}
}

function dataTerminoContrato(){
	var dtAssinaturaContrato 	 = document.formulario.obrdtassinaturacontrato.value;
	var diaPrazoVigenciaContrato = document.formulario.obrprazovigencia.value;
	var dtTerminoContrato		 = "";
	
	if (dtAssinaturaContrato && diaPrazoVigenciaContrato){
		var obDt = new Data();
		dtTerminoContrato = obDt.dtAddDia(dtAssinaturaContrato, diaPrazoVigenciaContrato);
	}
	document.formulario.dtterminocontrato.value = dtTerminoContrato;
}
--></script>

<form method="post" name="formulario" id="formulario" action="<?php echo $caminho_atual; ?>&acao=A">
	<input type="hidden" name="requisicao" value="executa"/>
	<input type="hidden" name="flchomlicdtprev" id="flchomlicdtprev" value="<?php print $_SESSION["obra"]["flchomlicdtprev"]; ?>"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td>Contratação da Obra</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Empresa Contratada</td>
			<td>
				<?php 
					$empresa 		= new Entidade($dobras->getEntIdEmpresaConstrutora());
					$entnomeempresa = $empresa->entnome;
					$entidempresa 	= $empresa->getPrimaryKey();
				?>
			  <span id="entnomeempresa"><?php echo $entnomeempresa; ?></span>
			  <input type="hidden" name="entidempresa" id="entidempresa" value="<? if( isset($_SESSION["obra"]["obrid"]) ) echo $entidempresa; ?>">
			  <?php if($habilitado){ ?>
			  	<input type="button" name="pesquisar_entidade" value="Pesquisar" style="cursor: pointer;" onclick="inserirEmpresa(document.getElementById('entidempresa').value);" <?php if($somenteLeitura=="N") echo "disabled"; ?>>
			  <?php }  print obrigatorio();?>
			  
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Data de Assinatura do Contrato:</td>
			<td>
				<?php $obrdtassinaturacontrato = $dobras->getObrDtAssinaturaContrato(); ?>
				<?= campo_data2( 'obrdtassinaturacontrato', 'S', $somenteLeitura, '', 'S', '', 'dataTerminoContrato();verDataHomologadao();' ); ?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Prazo de Vigência do Contrato (dias):</td>
			<td>
				<?php $obrprazovigencia = $dobras->getObrPrazoVigencia(); ?>
				<?= campo_texto( 'obrprazovigencia', 'S', $somenteLeitura, '', 8, 6, '######', '', 'left', '', 0, 'id="obrprazovigencia" onchange="dataTerminoContrato();"', 'dataTerminoContrato();'); ?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Data de têrmino do contrato:</td>
			<td>
				<?php $dtterminocontrato = $dobras->getDtTerminoContrato(); ?>
				<?= campo_data2( 'dtterminocontrato', 'N', 'N', '', 'S', '', '' ); ?>
			</td>
		</tr>
		<tr>
			<td>Sobre a Obra</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Situação da Obra:</td>
			<td>
				<?php
					if ( $_SESSION["obra"]["obrid"] ){
						$sql = "SELECT
								 oi.stoid as codigo,
								 so.stodesc as descricao
							   FROM
							  	 obras.obrainfraestrutura oi
							   INNER JOIN
							  	 obras.situacaoobra so ON so.stoid = oi.stoid
							   WHERE
								 oi.obrid = {$_SESSION["obra"]["obrid"]}";
						
//						ver($_SESSION["obra"]["obrid"]);
						
						$dados_situacao = $db->pegaLinha($sql);
						
						if ( !$dados_situacao ){

							$sql = "SELECT
									stoid as codigo,
									stodesc as descricao
								FROM
									obras.situacaoobra
								WHERE
									stoid = 99";
								
						}
						
					}else{
						$sql = "SELECT
									stoid as codigo,
									stodesc as descricao
								FROM
									obras.situacaoobra
								WHERE
									stoid = 99";
					}
					
					$db->monta_combo("stoid", $sql, "N", "", '', '', '', '190', 'N', 'stoid' );
					
					$situacao_atual = obras_pega_situacao( $_SESSION['obra']['obrid'] );

					if ( $situacao_atual == NAO_CONCLUIDA ){
				?>
					<input type="button" <?=(!possuiPerfil( array(PERFIL_SUPERUSUARIO, PERFIL_ADMINISTRADOR, PERFIL_SUPERVISORMEC, PERFIL_SUPERVISORUNIDADE )) ? "disabled=\"disabled\"" : " " ) ?> value="Criar Nova Obra" onclick="criaObraNova(<?=$_SESSION['obra']['obrid'] ?>);" style="cursor: pointer;" />
				<? } ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Histórico de Paralisações</td>
			<td>
				
				<?php
					$sql = "";
					$sql = "SELECT 
								'<center><img style=\"cursor:pointer;\" src=\"/imagens/consultar.gif\" border=\"0\" title=\"Visualizar Observação\" onclick=\"displayMessage(\'?modulo=principal/obshistorico&acao=A&hprid=' || hr.hprid || '\')\"></center>' as acao,
								tp.tpldsc as tipo,
								to_char(hprdtinclusao, 'DD/MM/YYYY') as dtparalisacao,
								CASE WHEN hprdtliberacao is null THEN 'Nõo Liberada'
									 ELSE to_char(hprdtliberacao, 'DD/MM/YYYY') END as dtliberacao,
								su.usunome as nome
							FROM
								obras.historicoparalisacao hr
							INNER JOIN
								obras.tipoparalisacao tp ON hr.tplid = tp.tplid
							LEFT JOIN
								obras.supervisao s ON s.supvid = hr.supvidparalisacao
							LEFT JOIN
								obras.supervisao s1 ON s1.supvid = hr.supvidliberacao
							INNER JOIN
								seguranca.usuario su ON s.usucpf = su.usucpf
							WHERE
								hr.obrid = {$_SESSION['obra']['obrid']}";
					
					$cabecalho = array('Visualizar', 'Tipo de Paralisação', 'Data da Paralisação', 'Data da Liberação', 'Responsável');
					$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%', 'S' );
				?>
				
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Histórico da Contratação</td>
			<td>
			<?php
				/*$onClick = "AbrirPopUp('obras.php?modulo=principal/extratoAditivo&acao=A&obrid= || o1.obrid || &traid= || t.traid || ', 'popUpExtatoAditivo', 'toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes,resizable=yes,copyhistory=yes,width=720,height=540')";
				$sql = "SELECT 
							'<center><img src=\"../imagens/consultar.gif\" onclick=\"" . str_replace(array(" || o1.obrid || ", " || t.traid || "), array("' || o1.obrid || '", "' || t.traid || '"), htmlspecialchars($onClick, ENT_QUOTES))  . "\" title=\"Extrato\"></center>' AS img,
							t.traseq,
							t.tradsc,
							tt.ttadsc,
							to_char(t.tradtassinatura, 'DD/MM/YYYY'),
							u.usunome			
						FROM
							obras.obrainfraestrutura o
						JOIN obras.termoaditivo t USING( obrid )
						JOIN obras.tipotermoaditivo tt USING( ttaid )
						JOIN seguranca.usuario u ON u.usucpf = t.usucpf
						JOIN obras.obrainfraestrutura o1 ON o1.obrid = o.obridaditivo
						WHERE
							o.obrid = {$_SESSION['obra']['obrid']}";
				
				$cabecalho = array('Extrato','Nï¿½ do Termo Aditivo', 'Denominaï¿½ï¿½o', 'Tipo do Aditivo', 'Data de Assinatura do Aditivo', 'Inserido Por');
				$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%', 'S' );*/
			
				$sql = "SELECT DISTINCT
							o1.obrid
						FROM
							obras.obrainfraestrutura o
						INNER JOIN
							obras.obrainfraestrutura o1 ON o1.obrid = o.obridaditivo
						WHERE
							o.obrid = {$_SESSION['obra']['obrid']}";
				$obridAux = $db->pegaUm($sql);
				
				if($obridAux)
				{
					$obridAditivos 		= array();
					$obridAditivos[] 	= $obridAux;
					$flag				= true;
					$i				  = 0;
					
					while($flag)
					{
						$sql = "SELECT DISTINCT
									o1.obrid
								FROM
									obras.obrainfraestrutura o
								INNER JOIN
									obras.obrainfraestrutura o1 ON o1.obrid = o.obridaditivo
								WHERE
									o.obrid = {$obridAditivos[$i]}";
						$obridAux = $db->pegaUm($sql);
						
						if($obridAux)
						{
							$i++;
							$obridAditivos[$i] = $obridAux;
						}
						else
						{
							$flag = false;
							unset($i);
						}
					}
					
//					dbg($obridAditivos,1);
					
					echo '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
							<thead>
								<tr>
									<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Extrato</td>
									<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Nº do Termo Aditivo</td>
									<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Denominação</td>
									<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Tipo do Aditivo</td>
									<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Data de Assinatura do Aditivo</td>
									<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Inserido Por</td>
								</tr>
							</thead>
							<tbody>';
					
					if(count($obridAditivos)>0) {
						for($i=0; $i<count($obridAditivos); $i++)
						{
							$sql = "SELECT
										t.traid,
										t.traseq,
										t.tradsc,
										tt.ttadsc,
										to_char(t.tradtassinatura, 'DD/MM/YYYY') as tradtassinatura,
										u.usunome			
									FROM
										obras.obrainfraestrutura o
									INNER JOIN obras.termoaditivo	  t ON t.obrid  = o.obrid
									INNER JOIN obras.tipotermoaditivo tt ON tt.ttaid = t.ttaid 
									INNER JOIN seguranca.usuario	   u ON u.usucpf = t.usucpf
									WHERE
										t.trastatus = 'A'
										AND o.obrid = {$obridAditivos[$i]}";
							
							$dadosObraAditivos = $db->carregar($sql);
	//						ver($obridAditivos[$i], $dadosObraAditivos);
							
							if($dadosObraAditivos)
							{
								for($k=0; $k<count($dadosObraAditivos); $k++)
								{
									echo '<tr bgcolor="" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'\';">
											<td valign="top" title="Extrato">
												<center><img src="../imagens/consultar.gif" onclick="AbrirPopUp(&#039;obras.php?modulo=principal/extratoAditivo&amp;acao=A&amp;obrid='.$obridAditivos[$i].'&amp;traid='.$dadosObraAditivos[$k]['traid'].'&#039;, &#039;popUpExtatoAditivo&#039;, &#039;toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes,resizable=yes,copyhistory=yes,width=720,height=540&#039;)" title="Extrato"></center>
									  		</td>
									  		<td align="right" valign="top" style="color:#999999;" title="Nº do Termo Aditivo">'.$dadosObraAditivos[$k]['traseq'].'</td>
									  		<td valign="top" title="Denominação">'.$dadosObraAditivos[$k]['tradsc'].'</td>
									  		<td valign="top" title="Tipo do Aditivo">'.$dadosObraAditivos[$k]['ttadsc'].'</td>
									  		<td valign="top" title="Data de Assinatura do Aditivo">'.$dadosObraAditivos[$k]['tradtassinatura'].'</td>
									  		<td valign="top" title="Inserido Por">'.$dadosObraAditivos[$k]['usunome'].'</td>
									  	  </tr>';
								}
							}
						}
					}
					
					echo '</tbody>
						</table>';
				}
				else
				{
					echo '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem"><tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr></table>';
				}
			?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Data da Ordem de Serviço:</td>
			<td>
				<?php $obrdtordemservico = $dobras->getObrDtOrdemServico(); ?>
				<?= campo_data2( 'obrdtordemservico', 'S', $somenteLeitura, '', 'S', '', '' ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Início de Execução da Obra:</td>
			<td>
				<?php $obrdtinicio = $dobras->getObrDtInicio(); $_SESSION["obras"]["obrdtinicio"] = formata_data($dobras->getObrDtInicio());?>
				<?= campo_data2( 'obrdtinicio', 'S', $somenteLeitura, '', 'S' ); ?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Prazo de Execução (dias):</td>
			<td>
				<?php $obrprazoexec = $dobras->getObrPrazoExec(); ?>
				<?= campo_texto( 'obrprazoexec', 'S', $somenteLeitura, '', 8, 6, '######', '', 'left', '', 0, 'id="obrprazoexec"', 'calculaDias( this.value );'); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Término de Execução da Obra:</td>
			<td>
				<?php $obrdttermino = $dobras->getObrDtTermino(); $_SESSION["obras"]["obrdttermino"] = formata_data($dobras->getObrDtTermino());?>
				<?= campo_data2( 'obrdttermino', 'S', 'N', '', 'S', '', '' ); ?>
			</td>
		</tr>
		
		
		<tr>
			<td class="SubTituloDireita">Valor Contratado da Obra (R$):</td>
			<td>
				<?php
				
					// verifica se tem etapa cadastrada, se tiver bloquea o campo
					$sql = "SELECT
								count(icoid)
							FROM
								obras.itenscomposicaoobra
							WHERE
								obrid = {$_SESSION["obra"]["obrid"]}
								AND icostatus = 'A'";
				
					$existe_item = $db->pegaUm($sql);
					
					//Permissï¿½o para que apenas os Perfis a seguir possam alterar o valor do Contrato da Obra, com Itens cadastrados ou nï¿½o.  
					(possuiPerfil(PERFIL_ADMINISTRADOR, PERFIL_SUPERUSUARIO)? $habilitado = 'S' : $habilitado = 'N' );
					//$vlr_habilitado = $existe_item ? 'N' : $somenteLeitura;
					$vlr_habilitado = $existe_item ? $habilitado : $somenteLeitura;
					
					$obrcustocontrato = $dobras->getObrCustoContrato();
					$_SESSION["obrcustocontrato"] = $obrcustocontrato;
					$obrcustocontrato = number_format($obrcustocontrato,2,',','.');
				?>
				<?= campo_texto( 'obrcustocontrato', 'S', $vlr_habilitado, '', 17, 15, '', '', 'left', '', 0, 'id="obrcustocontrato" onkeyup="this.value=mascaraglobal(\'###.###.###.###,##\',this.value); preencheValor();" onfocus="this.select();"'); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Área/Quantidade a ser Construída:</td>
			<td>
			<?php
				$obrqtdconstruida = $dobras->getObrQtdConstruida();
				$obrqtdconstruida = number_format($obrqtdconstruida,2,',','.');
			?>
			<?= campo_texto( 'obrqtdconstruida', 'S', $somenteLeitura, '', 17, 13, '', '', 'left', '', 0, 'id="obrqtdconstruida" onkeyup="this.value=mascaraglobal(\'###.###.###.###,##\',this.value); preencheValor();" onfocus="this.select();"');?> 
			Unidade de Medida: 
			<?php
				$umdidobraconstruida = $dobras->getUmdIdObraConstruida();					
				$sql = "
					SELECT umdid AS codigo, umdeesc AS descricao 
						FROM obras.unidademedida
				";
				$db->monta_combo("umdidobraconstruida", $sql, $somenteLeitura, '', '', '', '', '100', 'N');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Custo Unitário R$:</td>
			<td>
				<?php 
					$obrcustounitqtdconstruida = $dobras->getObrCustoUnitQtdConstruida();
					$obrcustounitqtdconstruida = number_format($obrcustounitqtdconstruida,2,',','.');									
				?>
				<?= campo_texto( 'obrcustounitqtdconstruida', 'N', 'N', '', 17, 20, '###.###.###.###,##', '', 'left', '', 0, 'id="obrcustounitqtdconstruida"'); ?>
				(R$ / Unidade de Medida)
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Percentual BDI:</td>
			<td>
				<?php 
					$obrpercbdi = $dobras->getObrPercBdi();
					$obrpercbdi = number_format($obrpercbdi,2,',','.');									
				?>
				<?= campo_texto( 'obrpercbdi', 'S', $somenteLeitura, '', 17, 20, '###,##', '', 'left', '', 0, 'id="obrpercbdi"'); ?>
				(Administração, taxas, emolumentos, impostos e lucro.)
			</td>
		</tr>
		<tr>
			<td>Supervisões - Empresa</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top"> Histórico de Supervisões:<br></td>
			<td>
			<?
		  $sql="SELECT DISTINCT
						   '<center><a style=\"color: blue;\" />' || os.orsid || '</a></center>' AS ordem,
						   '<center><a style=\"color: blue;\" />' || ig.gpdid || '</a></center>' AS grupo,
							e.entnome AS empresa,
						   '<center><a style=\"color: red;\" />'|| to_char(os.orsdtinicioexecucao, 'DD/MM/YYYY' )||'</a></center>'  AS orsdtinicioexecucao,
						   '<center><a style=\"color: red;\" />'|| to_char(os.orsdtfinalexecucao, 'DD/MM/YYYY' ) ||'</a></center>'  AS orsdtfinalexecucao,
							os.orsvalor AS valor,
						   	u.usunome AS contratante
				FROM
					obras.obrainfraestrutura oi
				INNER JOIN
					obras.repositorio ore ON ore.obrid = oi.obrid
				LEFT JOIN
					obras.itemgrupo ig ON ig.repid = ore.repid
				LEFT JOIN
					obras.grupodistribuicao gd ON gd.gpdid = ig.gpdid
				LEFT JOIN 
					obras.empresacontratada ec ON ec.epcid = gd.epcid 	
				LEFT JOIN 
					entidade.entidade e ON  e.entid = ec.entid 	 
				INNER JOIN
					obras.ordemservico os ON os.gpdid = gd.gpdid --ordem de serviço
				LEFT JOIN 
					seguranca.usuario u ON u.usucpf  = os.usucpf
				LEFT JOIN
					workflow.documento wd ON wd.docid = gd.docid
				LEFT JOIN
					workflow.estadodocumento we ON we.esdid = wd.esdid
				INNER JOIN
					entidade.entidade ee ON ee.entid = oi.entidunidade
				INNER JOIN
					obras.situacaoobra so ON so.stoid = oi.stoid
				INNER JOIN
					entidade.endereco ed ON ed.endid = oi.endid
				INNER JOIN
					territorios.municipio tm ON tm.muncod = ed.muncod
				LEFT JOIN
					obras.unidademedida ou ON ou.umdid = oi.umdidobraconstruida
				INNER JOIN
					obras.orgao oo ON oo.orgid = oi.orgid
				LEFT JOIN 
					obras.formarepasserecursos of ON of.obrid = oi.obrid
				LEFT JOIN
					obras.conveniosobra oc ON oc.covid = of.covid 
				WHERE
					oi.obrid = '{$_SESSION['obra']['obrid']}'
					AND
					os.orsstatus = 'A' 
					AND
					obsstatus = 'A' 
					AND 
					repstatus = 'A' 
					AND 
					we.esdid = ". OBRSUPFINALIZADA ." 
				ORDER BY
					empresa ASC
					";		

		$cabecalho = array('Nº OS', 'Nº do Grupo ', 'Empresa', 'Execução Início', 'Execução Término', 'Valor (R$)','Cadastrante');
		$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%', 'S','','','','' );
			
			
			
			?>
			</td>
		</tr>
		<tr>
			<td>Aditivos</td>
		</tr>
<? 
$srcImg  = "../imagens/gif_inclui.gif";
$onClick = "AbrirPopUp('obras.php?modulo=principal/popUpInserirAditivo&acao=A', 'popUpInsertAditivo', 'toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes,resizable=yes,copyhistory=yes,width=720,height=540')";

$pflcods = array(
					1 => PERFIL_SUPERUSUARIO,
					2 => PERFIL_ADMINISTRADOR,
					3 => PERFIL_SUPERVISORMEC,
					4 => PERFIL_EMPRESA
					);

if ( ($_SESSION['obras']['orgid'] == 3 || $_SESSION['obra']['orgid'] == 3) && !(possuiPerfil($pflcods)) ){
	$msg	= '* O seu perfil não pode adicionar aditivos.\n';	
	$srcImg = "../imagens/gif_inclui_d.gif";
	$onClick = "";
}

if ( obras_pega_situacao( $_SESSION['obra']['obrid'] ) != 1 ){
	$msg	.= '\n* A situação da obra estiver em execução.\n';	
	$srcImg = "../imagens/gif_inclui_d.gif";
	$onClick = "";
}
// Calcula a diferenï¿½a entre a data atual e a ultima vistoria
//list($dtUltVistoria) = explode(" ", $dados['obrdtvistoria']);
//$difUltVistoria 	 = $obData->quantidadeDeDiasEntreDuasDatas($dtUltVistoria, date("Y-m-d"), "YYYY-MM-DD");

/*if ( $difUltVistoria > 30 ){
	$msg	.= '* A vistoria estiver atualizada.\n';	
	$srcImg  = "../imagens/gif_inclui_d.gif";
	$onClick = "";
}*/

$msg = $msg ? 'O termo aditivo só pode ser incluído se:\n\n' . $msg : $msg;
?>		
		<tr>
			<td class="SubTituloDireita" valign="top">
				Histórico de Aditivos:<br>
			</td>
			<td>
			<? 
			$op = <<<EOT
					'<img src="/imagens/alterar.gif" style="cursor:pointer;" border=0 title="Alterar Aditivo" onclick="janela(\'obras.php?modulo=principal/popUpInserirAditivo&acao=A&traid=' || traid || ' \',720,540,\'popupAditivo\')">&nbsp;		
					 <img src="/imagens/excluir.gif" style="cursor:pointer;" border=0 title="Excluir Aditivo" onclick="confirmExcluir(\'Deseja excluir o aditivo (' || traseq || ' - ' || tradsc || ')?\', \'?modulo=principal/contratacao_da_obra&acao=A&requisicao=excluirAditivo&traid=' || traid || '\');">'		
EOT;
//					'<img src="/imagens/alterar_01.gif" style="cursor:pointer;" border=0 title="Alterar Aditivo">&nbsp;		
			$op2 = <<<EOT
					'<img src="/imagens/alterar.gif" style="cursor:pointer;" border=0 title="Alterar Aditivo" onclick="janela(\'obras.php?modulo=principal/popUpInserirAditivo&acao=A&traid=' || traid || ' \',720,540,\'popupAditivo\')">&nbsp;		
					 <img src="/imagens/excluir_01.gif" style="cursor:pointer;" border=0 title="Excluir Aditivo" onclick="">'		
EOT;
			$op3 = <<<EOT
					'<img src="/imagens/alterar.gif" style="cursor:pointer;" border=0 title="Alterar Aditivo" onclick="janela(\'obras.php?modulo=principal/popUpInserirAditivo&acao=A&traid=' || traid || ' \',720,540,\'popupAditivo\')">&nbsp;		
					 <img src="/imagens/excluir_01.gif" style="cursor:pointer;" border=0 title="Excluir Aditivo" onclick="">'		
EOT;
			
			$sql = "SELECT 
						CASE WHEN ( traseq = (SELECT 
											  	MAX(traseq) 
											  FROM 
											  	obras.termoaditivo 
											  WHERE 
											  	trastatus = 'A' 
											  	AND obrid = " . $_SESSION['obra']['obrid'] . ") ) 
								  AND ( usucpf = '" . $_SESSION['usucpf'] . "' 
										OR  1 = " . (possuiPerfil( array(PERFIL_SUPERVISORMEC, PERFIL_ADMINISTRADOR, PERFIL_GESTORMEC) ) ? 1 : 0) . " )
								  AND ( 0 = (SELECT 
								  				COUNT(*) 
								  			  FROM 
									  			obras.supervisao s 
											  JOIN obras.supervisaoitenscomposicao sup ON sup.supvid = s.supvid
									  		  JOIN obras.itenscomposicaoobra ito ON ito.icoid = sup.icoid
									  		  										AND ito.traid = ta.traid
									  		 										AND ito.obrid = " . $_SESSION['obra']['obrid'] . "
									  		  WHERE
									  			s.supstatus = 'A'
									  			AND s.obrid = " . $_SESSION['obra']['obrid'] . "))
							THEN $op
						WHEN ( 0 < (SELECT 
								  				COUNT(*) 
								  			  FROM 
									  			obras.supervisao s 
											  JOIN obras.supervisaoitenscomposicao sup ON sup.supvid = s.supvid
									  		  JOIN obras.itenscomposicaoobra ito ON ito.icoid = sup.icoid
									  		  										AND ito.traid = ta.traid
									  		 										AND ito.obrid = " . $_SESSION['obra']['obrid'] . "
									  		  WHERE
									  			s.supstatus = 'A'
									  			AND s.obrid = " . $_SESSION['obra']['obrid'] . "))	   
							THEN $op3
							ELSE $op2
						END AS acao,
						traseq,
						tradsc,
						ttadsc,
						TO_CHAR(tradtinclusao, 'DD-MM-YYYY'),
						usunome
					FROM
						obras.termoaditivo ta
					JOIN seguranca.usuario u USING(usucpf)
					JOIN obras.tipotermoaditivo USING(ttaid)
					WHERE
						ta.trastatus = 'A'
						AND obrid = " . $_SESSION['obra']['obrid'] . "
					ORDER BY
						traseq;";
			$cabecalho = array('Ação', 'Nº do termo', 'Denominação', 'Tipo', 'Data de inclusão', 'Inserido por');
			$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%', 'S' );
			?>
			<a href="#" style="text-align: left; color: #0F55A9;" onclick="<?=($msg ? "alert('{$msg}');" : "") ?><?=$onClick?>">
				Incluir Aditivo
			</a>
			</td>
		</tr>
<? 
$obAditivo = pegaObUltimoDadosAditivo();	
if ($obAditivo->traid):
?>		
		<tr>
			<td>Valor/Prazo Aditivado (último aditivo)</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">
				Prazo de prorrogação da vigência do contrato após o aditivo (dias):
			</td>
			<td><?=($obAditivo->traprazovigencia ? $obAditivo->traprazovigencia : '-' )?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">
				Data de término da vigência do contrato após aditivo:
			</td>
			<td><?=($obAditivo->traterminovigencia ? $obAditivo->traterminovigencia : '-')?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">
				Prazo de prorrogação da execução da obra após o aditivo (dias):
			</td>
			<td><?=($obAditivo->traprazoaditivadoexec ? $obAditivo->traprazoaditivadoexec : '-')?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">
				Data de término da execução da obra após aditivo:
			</td>
			<td><?=($obAditivo->traterminoexec ? $obAditivo->traterminoexec : '-')?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">
				Valor do aditivo (R$):
			</td>
			<td><?=($obAditivo->travlraditivo ? number_format($obAditivo->travlraditivo ,2,',','.') : '-')?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">
				Valor contratado da obra após o aditivo (R$):
			</td>
			<td><?=($obAditivo->travlrfinalobra ? number_format($obAditivo->travlrfinalobra ,2,',','.') : '-')?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">
				Alteração da área/quantidade:
			</td>
			<td><?=(($obAditivo->travlrqtdareaalterada || $obAditivo->travlrqtdareaacresc) ? number_format(($obAditivo->travlrqtdareaalterada ? $obAditivo->travlrqtdareaalterada : $obAditivo->travlrqtdareaacresc) ,2,',','.') : '-')?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">
				Área/Quantidade final incluindo aditivo:
			</td>
			<td><?=($obAditivo->travlrqtdareafinal ? number_format($obAditivo->travlrqtdareafinal ,2,',','.') : '-')?></td>
		</tr>
<? 
endif;
?>		
		<tr>
			<td>Origem dos Recursos</td>
		</tr>
		<tr>
			<td style="width:20%" class="SubTituloDireita">Tipo</td>	
			<td>
				<?php 
					$frpid = $dobras->getFrpId();
					
					$sql = "
						SELECT frpid AS codigo, frpdesc AS descricao FROM 
							obras.tipoformarepasserecursos
					";
					$db->monta_combo('frpid', $sql, $somenteLeitura, "Selecione...", 'abreCamposRecursos', '', '', '150', 'S', 'frpid');
				?>	
			</td>
		</tr>
		<tr id="convenio" style="display: none;">
			<td colspan="2">
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" >
					<tr>
						<td>Convênio</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Número do Convênio</td>
						<td>
							<?php 
								
								$covid = $dobras->getCovId();
								
								if ($covid){
									
									$dados_convenio = $db->pegaLinha("SELECT
																		*
																	  FROM 
																	  	obras.conveniosobra
																	  WHERE
																	  	covid = '{$covid}'");

								}
								
								$covnumero = $dados_convenio["covnumero"];
								
							?>
							<?= campo_texto( 'covnumero', 'N', 'S', '', 22, 20, '', '', 'left', '', 0, 'id="covnumero"'); ?>
							<?php if($habilitado){?>
<!--							<input type="button" id="busca_convenio" name="busca_convenio" value="Pesquisar" style="cursor: pointer;" onclick="window.open( '?modulo=principal/inserir_convenio&acao=A', 'blank', 'height=600, width=800, status=no, toolbar=no, menubar=no, scrollbars=yes, location=no, resizable=no');"/>-->
<!--							<input type="button" id="limpa_convenio" name="limpa_convenio" value="Remover" style="cursor: pointer;" onclick="removeConvenio();"/>-->
							<?php }?>
							<input type="hidden" name="covid" id="covid" value="<?php if($covid){ echo $covid; } ?>"/>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Ano</td>
						<td>
							<?php $covano = $dados_convenio["covano"]; ?>
							<?= campo_texto( 'covano', 'N', 'S', '', 4, 5, '', '', 'left', '', 0, 'id="covano"'); ?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Objeto</td>
						<td>
							<?php $covobjeto = $dados_convenio["covobjeto"]; ?>
							<?= campo_textarea( 'covobjeto', 'N', 'S', '', '50', '4', '1000'); ?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Detalhamento</td>
						<td>
							<?php $covdetalhamento = $dados_convenio["covdetalhamento"]; ?>
							<?= campo_textarea( 'covdetalhamento', 'N', 'S', '', '50', '4', '1000'); ?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Processo</td>
						<td>
							<?php $covprocesso = $dados_convenio["covprocesso"]; ?>
							<?= campo_texto( 'covprocesso', 'N', 'S', '', 22, 20, '', '', 'left', '', 0, 'id="covprocesso"'); ?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Concedente</td>
						<td>
							<?php 
								$covvlrconcedente = $dados_convenio["covvlrconcedente"];
								$covvlrconcedente = number_format($covvlrconcedente,2,',','.'); 
							?>
							<?= campo_texto( 'covvlrconcedente', 'N', 'S', '', 22, 17, '###.###.###,##', '', 'left', '', 0, 'id="covvlrconcedente"'); ?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Convenente</td>
						<td>
							<?php 
								$covvlrconvenente = $dados_convenio["covvlrconvenente"];
								$covvlrconvenente = number_format($covvlrconvenente,2,',','.'); 
							?>
							<?= campo_texto( 'covvlrconvenente', 'N', 'S', '', 22, 17, '###.###.###,##', '', 'left', '', 0, 'id="covvlrconvenente"'); ?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Valor R$</td>
						<td>
							<?php 
								$covvalor = $dados_convenio["covvalor"];
								$covvalor = number_format($covvalor,2,',','.'); 
							?>
							<?= campo_texto( 'covvalor', 'N', 'S', '', 22, 17, '###.###.###,##',  '', 'left', '', 0, 'id="covvalor"'); ?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Início</td>
						<td>
							<?php $covdtinicio = $dados_convenio["covdtinicio"]; ?>
							<?= campo_data2( 'covdtinicio', 'N', 'S', '', 'S' ); ?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Fim</td>
						<td>
							<?php $covdtfinal = $dados_convenio["covdtfinal"]; ?>
							<?= campo_data2( 'covdtfinal', 'N', 'S', '', 'S' ); ?>
						</td>
					</tr>
					<tr>
						<td colspan="2">Aditivo</td>
					</tr>
					<tr>
						<td colspan="2">
							<div id="covaditivo">
							<?php
								 
								$dados_aditivo = "SELECT
												  	covano, covobjeto, covprocesso,
												  	to_char(covdtinicio, 'DD/MM/YYYY') as inicio, 
												  	to_char(covdtfinal, 'DD/MM/YYYY') as fim, 
												  	covvalor
												  FROM
												  	obras.conveniosobra
												  WHERE
												  	covnumero = '" . ( $covid != null ? $covnumero : '000000') . "' AND
												  	covtipo = 'A'";
														
								$cabecalho = array("Ano", "Objeto", "Processo", "Data de Início", "Data de Término", "Valor" );
								$db->monta_lista_simples( $dados_aditivo, $cabecalho, 5, 10, 'N', '90%', '' );
							 ?>
							</div>
						</td>
					</tr>
					<tr>
						<td colspan="2">Apostilamento</td>
					</tr>
					<tr>
						<td colspan="2">
							<div id="covapostilamento">
							<?php
								
								$dados_apostilamento = "SELECT
														 	covano, covobjeto, covprocesso,
														  	to_char(covdtinicio, 'DD/MM/YYYY') as inicio, 
														  	to_char(covdtfinal, 'DD/MM/YYYY') as fim, 
														  	covvalor
														FROM
														  	obras.conveniosobra
														WHERE
														  	covnumero = '" . ( $covid != null ? $covnumero : '000000') . "' AND
														  	covtipo = 'P'";
							
								$cabecalho = array("Ano", "Objeto", "Processo", "Data de Início", "Data de Término", "Valor" );
								$db->monta_lista_simples( $dados_apostilamento, $cabecalho, 5, 10, 'N', '90%', '' );
							 ?>
							 </div>
						</td>
					</tr>
				</table>
			</td>
		</tr>




		<tr id="contrato-de-repasse" style="display: none;">
			<td colspan="2">
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" >
					<tr>
						<td>Contrato de Repasse</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Número do Contrato de Repasse</td>
						<td>
							<?php 
								
								$covid = $dobras->getCovId();
								
								if ($covid){
									
									$dados_convenio2 = $db->pegaLinha("SELECT
																		*
																	  FROM 
																	  	obras.conveniosobra
																	  WHERE
																	  	covid = '{$covid}'");

								}
								
								$covnumero2 = $dados_convenio2["covnumero"];
								
							?>
							<?= campo_texto( 'covnumero2', 'N', 'S', '', 22, 20, '', '', 'left', '', 0, 'id="covnumero2"'); ?>
							<?php if($habilitado){?>
<!--							<input type="button" id="busca_convenio" name="busca_convenio" value="Pesquisar" style="cursor: pointer;" onclick="window.open( '?modulo=principal/inserir_convenio&acao=A', 'blank', 'height=600, width=800, status=no, toolbar=no, menubar=no, scrollbars=yes, location=no, resizable=no');"/>-->
<!--							<input type="button" id="limpa_convenio" name="limpa_convenio" value="Remover" style="cursor: pointer;" onclick="removeConvenio();"/>-->
							<?php }?>
							<input type="hidden" name="covid" id="covid" value="<?php if($covid){ echo $covid; } ?>"/>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Ano</td>
						<td>
							<?php $covano2 = $dados_convenio2["covano"]; ?>
							<?= campo_texto( 'covano2', 'N', 'S', '', 4, 5, '', '', 'left', '', 0, 'id="covano2"'); ?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Objeto</td>
						<td>
							<?php $covobjeto2 = $dados_convenio2["covobjeto"]; ?>
							<?= campo_textarea( 'covobjeto2', 'N', 'S', '', '50', '4', '1000'); ?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Detalhamento</td>
						<td>
							<?php $covdetalhamento2 = $dados_convenio2["covdetalhamento"]; ?>
							<?= campo_textarea( 'covdetalhamento2', 'N', 'S', '', '50', '4', '1000'); ?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Processo</td>
						<td>
							<?php $covprocesso2 = $dados_convenio2["covprocesso"]; ?>
							<?= campo_texto( 'covprocesso2', 'N', 'S', '', 22, 20, '', '', 'left', '', 0, 'id="covprocesso2"'); ?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Concedente</td>
						<td>
							<?php 
								$covvlrconcedente2 = $dados_convenio2["covvlrconcedente"];
								$covvlrconcedente2 = number_format($covvlrconcedente2,2,',','.'); 
							?>
							<?= campo_texto( 'covvlrconcedente2', 'N', 'S', '', 22, 17, '###.###.###,##', '', 'left', '', 0, 'id="covvlrconcedente2"'); ?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Convenente</td>
						<td>
							<?php 
								$covvlrconvenente2 = $dados_convenio2["covvlrconvenente"];
								$covvlrconvenente2 = number_format($covvlrconvenente2,2,',','.'); 
							?>
							<?= campo_texto( 'covvlrconvenente2', 'N', 'S', '', 22, 17, '###.###.###,##', '', 'left', '', 0, 'id="covvlrconvenente2"'); ?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Valor R$</td>
						<td>
							<?php 
								$covvalor2 = $dados_convenio2["covvalor"];
								$covvalor2 = number_format($covvalor2,2,',','.'); 
							?>
							<?= campo_texto( 'covvalor2', 'N', 'S', '', 22, 17, '###.###.###,##',  '', 'left', '', 0, 'id="covvalor2"'); ?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Início</td>
						<td>
							<?php $covdtinicio2 = $dados_convenio2["covdtinicio"]; ?>
							<?= campo_data2( 'covdtinicio2', 'N', 'S', '', 'S' ); ?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Fim</td>
						<td>
							<?php $covdtfinal2 = $dados_convenio2["covdtfinal"]; ?>
							<?= campo_data2( 'covdtfinal2', 'N', 'S', '', 'S' ); ?>
						</td>
					</tr>
					<tr>
						<td colspan="2">Aditivo</td>
					</tr>
					<tr>
						<td colspan="2">
							<div id="covaditivo">
							<?php
								 
								$dados_aditivo = "SELECT
												  	covano, covobjeto, covprocesso,
												  	to_char(covdtinicio, 'DD/MM/YYYY') as inicio, 
												  	to_char(covdtfinal, 'DD/MM/YYYY') as fim, 
												  	covvalor
												  FROM
												  	obras.conveniosobra
												  WHERE
												  	covnumero = '" . ( $covid != null ? $covnumero2 : '000000') . "' AND
												  	covtipo = 'A'";
														
								$cabecalho = array("Ano", "Objeto", "Processo", "Data de Início", "Data de Término", "Valor" );
								$db->monta_lista_simples( $dados_aditivo, $cabecalho, 5, 10, 'N', '90%', '' );
							 ?>
							</div>
						</td>
					</tr>
					<tr>
						<td colspan="2">Apostilamento</td>
					</tr>
					<tr>
						<td colspan="2">
							<div id="covapostilamento">
							<?php
								
								$dados_apostilamento = "SELECT
														 	covano, covobjeto, covprocesso,
														  	to_char(covdtinicio, 'DD/MM/YYYY') as inicio, 
														  	to_char(covdtfinal, 'DD/MM/YYYY') as fim, 
														  	covvalor
														FROM
														  	obras.conveniosobra
														WHERE
														  	covnumero = '" . ( $covid != null ? $covnumero2 : '000000') . "' AND
														  	covtipo = 'P'";

							

								$cabecalho = array("Ano", "Objeto", "Processo", "Data de Início", "Data de Término", "Valor" );
								$db->monta_lista_simples( $dados_apostilamento, $cabecalho, 5, 10, 'N', '90%', '' );
							 ?>
							 </div>
						</td>
					</tr>
				</table>
			</td>
		</tr>



		<tr id="descentralizacao" style="display: none;">
			<td colspan="2">
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr>
						<td>Descentralização</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Instituição</td>
						<td>
							<?
							$frrdescinstituicao = $dobras->getFrrDescInstituicao();
							if($frrdescinstituicao == ""){
								$dados2 = $obras->ViewObra($_SESSION["obra"]["obrid"]);
								$dobras->setFrrDescInstituicao($dados2["entidade"]);
							}
							$frrdescinstituicao = $dobras->getFrrDescInstituicao();
							?>
							<?= campo_texto( 'frrdescinstituicao', 'N', $somenteLeitura, '', 60, 60, '',  '', 'left', '', 0); ?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Número da Portaria de Descentralização</td>
						<td>
							<? $frrdescnumport = $dobras->getFrrDescNumPort(); ?>
							<?= campo_texto( 'frrdescnumport', 'N', $somenteLeitura, '', 22, 20, '',  '', 'left', '', 0); ?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Objeto</td>
						<td>
							<? $frrdescobjeto = $dobras->getFrrDescObjeto(); ?>
							<?= campo_texto( 'frrdescobjeto', 'N', $somenteLeitura, '', 60, 60, '',  '', 'left', '', 0); ?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Valor R$</td>
						<td>
							<? 
								$frrdescvlr = $dobras->getFrrDescVlr();
								$frrdescvlr = number_format($frrdescvlr, 2, ',', '.'); 
							?>
							<?= campo_texto( 'frrdescvlr', 'N', $somenteLeitura, '', 17, 14, '###.###.###,##',  '', 'left', '', 0); ?>
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr id="recurso" style="display: none;">
			<td class="SubTituloDireita">Observação:</td>
			<td>
				<? $frrobsrecproprio = $dobras->getFrrObsRecProprio(); ?>
				<?= campo_textarea( 'frrobsrecproprio', 'N', $somenteLeitura, '', '70', '4', '300'); ?>
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td></td>
			<td>
				<div style="float: left;">
					<input type="hidden" name="frrid" value="<? echo $dobras->getFrrId();?>">
					<input type="hidden" name="obrid" value="<? echo $_SESSION["obra"]["obrid"];?>">
					<?php if($habilitado) { ?>
						<input type="button" id="salvar" value="Salvar" onclick="validaRec('1<?=possuiPerfil(array(PERFIL_SUPERVISORMEC, PERFIL_ADMINISTRADOR));?>')" style="cursor: pointer" <?php if($somenteLeitura == "N") { echo "disabled"; } ?>>
					<?php } ?> 
					<input type="button" value="Voltar" style="cursor: pointer" onclick="history.back(-1);">
				</div>
			</td>
		</tr>
</table>
</form>

<script>
function calculaDias( prazo ){

	var inicio = document.getElementById( "obrdtinicio" ).value;

	if ( inicio && prazo ){ 
		
		var obDt = new Data();
		var termino = obDt.dtAddDia(inicio, prazo);
		 
	}else{
		var termino  = ""; 
	}	
	
	document.getElementById( "obrdttermino" ).value = termino;
	
}

function abreCamposRecursos(id){
	var tr_convenio  = document.getElementById( 'convenio' );
	var tr_contratorepasse = document.getElementById('contrato-de-repasse');
	var tr_descentralizacao  = document.getElementById( 'descentralizacao' );
	var tr_rec  = document.getElementById( 'recurso' );
//	var btn_pesquisa  = document.getElementById( 'busca_convenio' );
//	var btn_remove  = document.getElementById( 'limpa_convenio' );

	tr_convenio.style.display = 'none';
	tr_descentralizacao.style.display = 'none';
	tr_rec.style.display = 'none';
	tr_contratorepasse.style.display = 'none';
//	btn_pesquisa.style.diplay = 'block';
//	btn_remove.style.diplay = 'block';

	if(id == 2){
//		btn_pesquisa.style.diplay = 'none';
//		btn_remove.style.diplay = 'none';
		if (document.selection){
			tr_convenio.style.display = 'block';
		}else{
			tr_convenio.style.display = 'table-row';
		}
	}
	if(id == 3){
		if (document.selection){
			tr_descentralizacao.style.display = 'block';
		}else{
			tr_descentralizacao.style.display = 'table-row';
		}
	}
	if(id == 4){
		if (document.selection){
			tr_rec.style.display = 'block';
		}else{
			tr_rec.style.display = 'table-row';
		}
	}
	if(id == 7){
//		btn_pesquisa.style.diplay = 'none';
//		btn_remove.style.diplay = 'none';
		if (document.selection){
//				tr_convenio.style.display = 'block';
				tr_contratorepasse.style.display = 'block';
		}else{
//				tr_convenio.style.display = 'table-row';
				tr_contratorepasse.style.display = 'table-row';
		}
	}
}

</script>

<?php

if( $stoid == EM_CONSTRUCAO || $stoid == FINALIZADA ){
	echo "
		<script>
			
			if (document.selection){
				document.getElementById('inaugurada').style.display = 'block';
			}else{
				document.getElementById('inaugurada').style.display = 'table-row';
			}

			abreData('" . $obrstatusinauguracao . "');
			
		</script>";
}

echo "<script>";
	echo "abreCamposRecursos(".$dobras->getFrpId().");";
	echo "dataTerminoContrato();";
echo "</script>";
?>
