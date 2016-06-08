<?
ini_set("memory_limit", "64M");
header('Content-type: text/html; charset="iso-8859-1"',true);

if ( $_POST['operacao'] == 'salvar' ){
	$possuiSupervisao = false;
	if ( $_POST['traid'] ){
		$sql = "SELECT 
	  				COUNT(*) 
	  			FROM 
		  			obras.supervisao s 
				JOIN obras.supervisaoitenscomposicao sup ON sup.supvid = s.supvid
		  		JOIN obras.itenscomposicaoobra ito ON ito.icoid = sup.icoid
		  		  									  AND ito.traid = '{$_POST['traid']}'";
		$possuiSupervisao = $db->pegaUm( $sql );
	}
	if ( !$possuiSupervisao ){
		$obridnova = salvaObraAditivo($_SESSION['obra']['obrid'], $_POST);
	}	
	die("<script>
				alert('Operação realizada com sucesso!');
				window.opener.location.href = '?modulo=principal/contratacao_da_obra&acao=A';
				window.close();
		 </script>");
	
}

if ($_REQUEST['traid']){
	$sql = "SELECT 
				traid, ttaid, usucpf, umdidareaacresc, umdidareafinal, umdidareaalterada, 
       			obrid, tradsc, traseq, tradtassinatura, traprazovigencia, 
       			TO_CHAR(traterminovigencia, 'DD/MM/YYYY') AS traterminovigencia, 
       			traprazoaditivadoexec, 
       			TRIM( TO_CHAR(travlraditivo, '999G999G999G999D99') ) AS travlraditivo, 
       			TRIM( TO_CHAR(travlrfinalobra, '999G999G999G999D99') ) AS travlrfinalobra, 
       			TRIM( TO_CHAR(travlrqtdareaacresc, '999G999G999G999D99') ) AS travlrqtdareaacresc, 
       			TRIM( TO_CHAR(travlrqtdareafinal, '999G999G999G999D99') ) AS travlrqtdareafinal, 
       			TRIM( TO_CHAR(travlrqtdareaalterada, '999G999G999G999D99') ) AS travlrqtdareaalterada, 
       			travlraditivo AS travlraditivonum, 
       			travlrfinalobra AS travlrfinalobranum, 
       			travlrqtdareaacresc AS travlrqtdareaacrescnum, 
       			travlrqtdareafinal AS travlrqtdareafinalnum, 
       			travlrqtdareaalterada AS travlrqtdareaalteradanum, 
       			trajustificativa, 
       			TO_CHAR(traterminoexec, 'DD/MM/YYYY') AS traterminoexec,
       			trasupressao
  			FROM 
  				obras.termoaditivo
  			WHERE
  				traid = {$_REQUEST['traid']}";
	
	$dados = (array) $db->pegaLinha( $sql );
	extract($dados);	
}

$trasupressao = $trasupressao ? $trasupressao : 'N';
$obObras = new Obras();
$obAditivo = pegaObUltimoDadosAditivo();
// Carrega dados da obra
$dados = $obObras->Dados( $_SESSION["obra"]['obrid'] );
if ($_POST['tipoTela'] == 1){
?>
	<table class="Tabela" align="center" style="border: 0px; width:100%;">
	  <tr>
	    <th class="SubTituloDireita">Nº do Aditivo:</th>
	    <td>
		<?
//		$sql = "SELECT 
//					obridaditivo 
//				FROM obras.obrainfraestrutura 
//				WHERE 
//					obsstatus = 'A' AND
//					obrid = ".$_SESSION['obra']['obrid'];
//		
//		$obridant = $db->pegaUm( $sql ); 
//		if ($obridant) {
//			$sql = "SELECT 
//						(MAX(traseq) + 1) as traseq 
//				    FROM obras.termoaditivo
//				    WHERE obrid = ".$obridant;
//			$traseq = $db->pegaUm( $sql );
//		}else{
//			$traseq = 1;
//		}
		if ( !$traseq ){
			$sql = "SELECT 
						(MAX(traseq) + 1) as traseq 
				    FROM 
				    	obras.termoaditivo
				    WHERE
				    	trastatus = 'A' 
				    	AND obrid = " . $_SESSION['obra']['obrid'];
			
			$traseq = $db->pegaUm( $sql );
			$traseq = $traseq ? $traseq : 1;
		}
		echo campo_texto( 'traseq', 'S', 'N', '', 20, 60, '', '', 'left', '', 0); 
		?>    
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Denominação:</th>
	    <td>
		<?= campo_texto( 'tradsc', 'S', 'S', '', 65, 60, '', '', 'left', '', 0); ?>    
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Data de Assinatura do Aditivo:</th>
	    <td>
		<?= campo_data2( 'tradtassinatura', 'S', 'S', '', 'S', '', 'valDtAssinaturaContrato();' ); ?>
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Prazo de Vigência do Aditivo do Contrato (dias):</th>
	    <td>
		<?= campo_texto( 'traprazovigencia', 'S', 'S', '', 20, 10, '[#]', '', 'left', '', 0, 'onchange="valPrazoVigenciaExecucao(); dtVigencia();"', 'dtVigencia();'); ?>    
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Data de Término da Vigência do Aditivo do Contrato:</th>
	    <td>
		<?= campo_texto( 'traterminovigencia', 'S', 'N', '', 20, 10, '', '', 'left', '', 0); ?>    
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Prazo do Aditivo de Execução da Obra (dias):</th>
	    <td>
		<?= campo_texto( 'traprazoaditivadoexec', 'S', 'S', '', 20, 10, '[#]', '', 'left', '', 0, 'onchange="valPrazoVigenciaExecucao(); dtExecucao();"', 'dtExecucao();'); ?>    
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Término da Execução do Aditivo da Obra:</th>
	    <td>
		<?= campo_texto( 'traterminoexec', 'S', 'N', '', 20, 10, '', '', 'left', '', 0); ?>    
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Justificativa:</th>
	    <td>
		<?=campo_textarea( 'trajustificativa', 'S', 'S', '', '70', '8', 2000); ?>    
		</td>
	  </tr>
	</table>
<?
die;
}
elseif ($_POST['tipoTela'] == 2){
?>
	<table class="Tabela" align="center" style="border: 0px; width:100%;">
	  <tr>
	    <th class="SubTituloDireita">Nº do Aditivo:</th>
	    <td>
		<? 
//		$sql = "SELECT 
//					obridaditivo 
//				FROM obras.obrainfraestrutura 
//				WHERE 
//					obrid = ".$_SESSION['obra']['obrid'];
//		
//		$obridant = $db->pegaUm( $sql ); 
//		if ($obridant) {
//			$sql = "SELECT 
//						(MAX(traseq) + 1) as traseq 
//				    FROM obras.termoaditivo
//				    WHERE obrid = ".$obridant;
//			
//			$traseq = $db->pegaUm( $sql );
//		}else{
//			$traseq = 1;
//		}
		if ( !$traseq ){
			$sql = "SELECT 
						(MAX(traseq) + 1) as traseq 
				    FROM 
				    	obras.termoaditivo
				    WHERE
				    	trastatus = 'A' 
				    	AND obrid = " . $_SESSION['obra']['obrid'];
			
			$traseq = $db->pegaUm( $sql );
			$traseq = $traseq ? $traseq : 1;
		}				
		echo campo_texto( 'traseq', 'S', 'N', '', 20, 60, '', '', 'left', '', 0); 
		?>    
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Denominação:</th>
	    <td>
		<?= campo_texto( 'tradsc', 'S', 'S', '', 65, 60, '', '', 'left', '', 0); ?>    
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Data de Assinatura do Aditivo:</th>
	    <td>
		<?= campo_data2( 'tradtassinatura', 'S', 'S', '', 'S', '', 'valDtAssinaturaContrato();' ); ?>
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Aditivo de Supressão:</th>
	    <td>
		<?
			$opcoes = array(
							'Sim' => array(
								'callback' => 'calculoValorFinal();',
								'valor'	   => 'S',
								'id'	   => null
								),
							'Não' => array(
//								'default'  => true,
								'callback' => 'calculoValorFinal();',
								'valor'    => 'N',
								'id'	   => null
								)
							);
			
			echo campo_radio('trasupressao', $opcoes, 'h');
		?>
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Valor do Aditivo(R$):</th>
	    <td>
		<?= campo_texto( 'travlraditivo', 'S', 'S', '', 20, 11, '[###.]###,##', '', 'left', '', 0, 'onchange="comparacaoValAditivo(); calculoValorFinal();"', 'calculoValorFinal();' ); ?>
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Valor Final da Obra Incluindo Aditivo(R$):</th>
	    <td>
		<?= campo_texto( 'travlrfinalobra', 'S', 'N', '', 20, 11, '[###.]###,##', '', 'left', '', 0, '', '' ); ?>
	  </td>
	  <tr>
	    <th class="SubTituloDireita">Acréscimo ou Supressão de Área/Quantidade:</th>
	    <td>
		<?= campo_texto( 'travlrqtdareaacresc', 'S', 'S', '', 20, 11, '[###.]###,##', '', 'left', '', 0, 'onchange="calculoQtdFinal();"', 'calculoQtdFinal();' ); ?>
		<?php 
			$sql = "SELECT
							umdid as codigo,
							umdeesc as descricao
					FROM
							obras.unidademedida
					WHERE
							umdstatus = 'A'";
			$umdidareaacresc = $dados['umdidobraconstruida'];
			$db->monta_combo( 'umdidareaacresc', $sql, 'N', 'Selecione...', '', '', '', 100, '', 'traunidade' ); ?>
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Área/Quantidade Final Incluindo Aditivo(R$):</th>
	    <td>
		<?= campo_texto( 'travlrqtdareafinal', 'S', 'N', '', 20, 11, '[###.]###,##', '', 'left', '', 0, '', '' ); ?>
		<?php 
			$sql = "SELECT
							umdid as codigo,
							umdeesc as descricao
					FROM
							obras.unidademedida
					WHERE
							umdstatus = 'A'";
			$umdidareafinal = $dados['umdidobraconstruida'];
			$db->monta_combo( 'umdidareafinal', $sql, 'N', 'Selecione...', '', '', '', 100, '', 'traunidade2' ); ?>
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Justificativa:</th>
	    <td>
		<?=campo_textarea( 'trajustificativa', 'S', 'S', '', '70', '8', 2000); ?>    
		</td>
	  </tr>
	</table>
<?
die;
}
elseif ($_POST['tipoTela'] == 3){
?>
	<table class="Tabela" align="center" style="border: 0px; width:100%;">
	  <tr>
	    <th class="SubTituloDireita">Nº do Aditivo:</th>
	    <td>
		<? 
//		$sql = "SELECT 
//					obridaditivo 
//				FROM obras.obrainfraestrutura 
//				WHERE 
//					obrid = ".$_SESSION['obra']['obrid'];
//		
//		$obridant = $db->pegaUm( $sql ); 
//		if ($obridant) {
//			$sql = "SELECT 
//						(MAX(traseq) + 1) as traseq 
//				    FROM obras.termoaditivo
//				    WHERE obrid = ".$obridant;
//			
//			$traseq = $db->pegaUm( $sql );
//		}else{
//			$traseq = 1;
//		}
		if ( !$traseq ){
			$sql = "SELECT 
						(MAX(traseq) + 1) as traseq 
				    FROM 
				    	obras.termoaditivo
				    WHERE
				    	trastatus = 'A' 
				    	AND obrid = " . $_SESSION['obra']['obrid'];
			
			$traseq = $db->pegaUm( $sql );
			$traseq = $traseq ? $traseq : 1;
		}				
		echo campo_texto( 'traseq', 'S', 'N', '', 20, 60, '', '', 'left', '', 0); 
		?>    
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Denominação:</th>
	    <td>
		<?= campo_texto( 'tradsc', 'S', 'S', '', 65, 60, '', '', 'left', '', 0); ?>    
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Data de Assinatura do Aditivo:</th>
	    <td>
		<?= campo_data2( 'tradtassinatura', 'S', 'S', '', 'S', '', 'valDtAssinaturaContrato();' ); ?>
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Prazo de Vigência do Aditivo do Contrato (dias):</th>
	    <td>
		<?= campo_texto( 'traprazovigencia', 'S', 'S', '', 20, 10, '[#]', '', 'left', '', 0, 'onchange="valPrazoVigenciaExecucao(); dtVigencia();"', 'dtVigencia();'); ?>    
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Data de Término da Vigência do Aditivo do Contrato:</th>
	    <td>
		<?= campo_texto( 'traterminovigencia', 'S', 'N', '', 20, 10, '', '', 'left', '', 0); ?>    
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Prazo do Aditivo de Execução da Obra (dias):</th>
	    <td>
		<?= campo_texto( 'traprazoaditivadoexec', 'S', 'S', '', 20, 10, '[#]', '', 'left', '', 0, 'onchange="valPrazoVigenciaExecucao(); dtExecucao();"', 'dtExecucao();'); ?>    
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Término da Execução do Aditivo da Obra:</th>
	    <td>
		<?= campo_texto( 'traterminoexec', 'S', 'N', '', 20, 10, '', '', 'left', '', 0); ?>    
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Aditivo de Supressão:</th>
	    <td>
		<?
			$opcoes = array(
							'Sim' => array(
								'callback' => 'calculoValorFinal();',
								'valor'	   => 'S',
								'id'	   => null
								),
							'Não' => array(
//								'default'  => true,
								'callback' => 'calculoValorFinal();',
								'valor'    => 'N',
								'id'	   => null
								)
							);
			
			echo campo_radio('trasupressao', $opcoes, 'h');
		?>
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Valor do Aditivo(R$):</th>
	    <td>
		<?= campo_texto( 'travlraditivo', 'S', 'S', '', 20, 11, '[###.]###,##', '', 'left', '', 0, 'onchange="comparacaoValAditivo(); calculoValorFinal();"', 'calculoValorFinal();' ); ?>
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Valor Final da Obra Incluindo Aditivo(R$):</th>
	    <td>
		<?= campo_texto( 'travlrfinalobra', 'S', 'N', '', 20, 11, '[###.]###,##', '', 'left', '', 0, '', '' ); ?>
	  </td>
	  <tr>
	    <th class="SubTituloDireita">Alteração da Área/Quantidade:</th>
	    <td>
		<?= campo_texto( 'travlrqtdareaalterada', 'S', 'S', '', 20, 11, '[###.]###,##', '', 'left', '', 0, 'onchange="calculoQtdFinal();"', 'calculoQtdFinal();' ); ?>
		<?php 
			$sql = "SELECT
							umdid as codigo,
							umdeesc as descricao
					FROM
							obras.unidademedida
					WHERE
							umdstatus = 'A'";
			$umdidareaalterada = $dados['umdidobraconstruida'];
			$db->monta_combo( 'umdidareaalterada', $sql, 'N', 'Selecione...', '', '', '', 100, '', 'traunidade' ); ?>
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Área/Quantidade Final Incluindo Aditivo(R$):</th>
	    <td>
		<?= campo_texto( 'travlrqtdareafinal', 'S', 'N', '', 20, 11, '[###.]###,##', '', 'left', '', 0, '', '' ); ?>
		<?php 
			$sql = "SELECT
							umdid as codigo,
							umdeesc as descricao
					FROM
							obras.unidademedida
					WHERE
							umdstatus = 'A'";
			$umdidareafinal = $dados['umdidobraconstruida'];
			$db->monta_combo( 'umdidareafinal', $sql, 'N', 'Selecione...', '', '', '', 100, '', 'traunidade2' ); ?>
		</td>
	  </tr>
	  <tr>
	    <th class="SubTituloDireita">Justificativa:</th>
	    <td>
		<?=campo_textarea( 'trajustificativa', 'S', 'S', '', '70', '8', 2000); ?>    
		</td>
	  </tr>
	</table>
<?
die;
}
?>
<html>
<head>
	<title>Inserir Aditivo</title>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
	<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
	<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
</head>
<body topmargin="0" leftmargin="0" onload="abreAditivo('<?=$ttaid ?>');">
<form method="post" action="" name="form_aditivo" id="form_aditivo">
<table class="Tabela" align="center">
  <tr>
    <th class="SubTituloDireita">Tipo de Aditivo:</th>
    <td>
    <? 
	$sql = "SELECT 
				ttaid AS codigo, 
				ttadsc AS descricao
			FROM 
				obras.tipotermoaditivo;";
	    					  
	$db->monta_combo("ttaid", $sql, 'S', "Selecione...", "abreAditivo", '', '', '', 'S', 'ttaid');


	$sql = "SELECT 
				MAX(traseq) AS traseqmax,
				traid AS traidmax,
				traterminoexec AS traterminoexecmax,
				traterminovigencia AS traterminovigenciamax,
				traprazovigencia AS traprazovigenciamax,
				travlrfinalobra AS travlrfinalobramax,
				travlrqtdareafinal AS travlrqtdareafinalmax
			FROM 
				obras.termoaditivo
			WHERE
				trastatus = 'A'
				AND obrid = {$_SESSION['obra']['obrid']}
			GROUP BY
				traid,
				traterminoexec,
				traterminovigencia,
				traprazovigencia,
				travlrfinalobra,
				travlrqtdareafinal
			ORDER BY
				traseqmax DESC
			LIMIT 1";
	$dadosTermo = $db->pegaLinha( $sql );
	$dadosTermo['traprazovigenciamax'] = pegaSomaVlrAditivo("traprazovigencia");  
	$dadosTermo['travlrfinalobramax']  = pegaObMaiorVlrAditivo();  
    ?>
    <!-- 
    Devido a retificação da maneira de se aditivar uma obra, diversos campos seram substituídos (quando tiverem valor) a fim de que atenda a
    nova regra objetivando manter os valores iniciais da obra.
    (obrdttermino 	   => traterminoexec
     dtterminocontrato => traterminovigencia
     obrprazovigencia  => traprazovigencia
     obrcustocontrato  => travlrfinalobra
     obrqtdconstruida  => travlrqtdareafinal)
     -->
     
     <?php //ver($obAditivo->travlrfinalobra ? number_format($obAditivo->travlrfinalobra ,2,',','.') : ($dadosTermo['travlrfinalobramax'] ? ($dadosTermo['travlrfinalobramax']) : $dados['obrcustocontrato'])); ?>
     <?php 
		     if ($obAditivo->travlrfinalobra) {
		     	$valorContrato = number_format($obAditivo->travlrfinalobra ,2,',','.');
		     	
		     }elseif ($dadosTermo['travlrfinalobramax']){
		     	$valorContrato = $dadosTermo['travlrfinalobramax'];
		     	
		     }else{
		     	$valorContrato = number_format($dados['obrcustocontrato'],2,',','.');
		     }
     ?>
     
    <input type="hidden" name="operacao" 				value="salvar">
    <input type="hidden" name="traid" 					value="<?=$traid ?>">
    <input type="hidden" name="obrdttermino" 			value="<?=($obAditivo->traterminoexec ? $obAditivo->traterminoexec : ($dadosTermo['traterminoexecmax'] ? $dadosTermo['traterminoexecmax'] : formata_data($dados['obrdttermino'])))?>">
    <input type="hidden" name="dtterminocontrato" 		value="<?=($obAditivo->traterminovigencia ? $obAditivo->traterminovigencia : ($dadosTermo['traterminovigenciamax'] ? formata_data($dadosTermo['traterminovigenciamax']) : formata_data($dados['dtterminocontrato'])))?>">
    <input type="hidden" name="obrdtassinaturacontrato" value="<?=formata_data($dados['obrdtassinaturacontrato']) ?>">
    <input type="hidden" name="obrprazovigencia" 		value="<?=($dadosTermo['traprazovigenciamax'] ? ($dadosTermo['traprazovigenciamax'] + $dados['obrprazovigencia']) : $dados['obrprazovigencia'])?>">
    
    <input type="hidden" name="obrcustocontrato" 		value="<?=$valorContrato ?>">
    <input type="hidden" name="obrqtdconstruida" 		value="<?=($obAditivo->travlrqtdareafinal ? number_format($obAditivo->travlrqtdareafinal ,2,',','.') : ($dadosTermo['travlrqtdareafinalmax'] ? ($dadosTermo['travlrqtdareafinalmax']) : number_format($dados['obrqtdconstruida'] ,2,',','.')))?>">
    <input type="hidden" name="tobraid" 				value="<?=$dados['tobraid']?>">
    
    </td>
  </tr>
  <tr style="display:none;" id="trTelaAditivo">
  	<td id="telaAditivo" colspan="2"></td>
  </tr>
  <tr style="display:none;" id="trButton" bgcolor="#C0C0C0">
  	<td colspan="2" align="center">
  		<input type="button" value="Salvar" onclick="validaForm(); //valTerminoVigenciaExecucao();">
  	</td>
  </tr>
</table>
</form>

<script type="text/javascript">
function abreAditivo(value){
	if (value){
		divCarregando();
		var traid = '<?=$_REQUEST['traid'] ?>';
		var dado = {async: false, "tipoTela" : value, "traid" : traid};
		$('#telaAditivo').load("obras.php?modulo=principal/popUpInserirAditivo&acao=A", dado, function (){ 
					if(traid){atualizaParaEdicao();}
					<? 
					if ($traid){
						$sql = "SELECT 
					  				COUNT(*) 
					  			FROM 
						  			obras.supervisao s 
								JOIN obras.supervisaoitenscomposicao sup ON sup.supvid = s.supvid
						  		JOIN obras.itenscomposicaoobra ito ON ito.icoid = sup.icoid
						  		  									  AND ito.traid = {$traid}";
						$possuiSupervisao = $db->pegaUm( $sql );
						if ($possuiSupervisao || ($traid != $dadosTermo['traidmax']) ){
					?>
						$(document).ready(function (){
							$('#form_aditivo :input').attr('disabled', true);
						});
					<?		
						}
					}
					?>
					divCarregado(); 
				});
		$('#trTelaAditivo').show();
		$('#trButton').show();
	}else{
		$('#telaAditivo').html("");
		$('#trTelaAditivo').hide();
		$('#trButton').hide();
	}	
}



var msgIni = '';
if ($.trim($('[name="dtterminocontrato"]').val()) == ''){
	msgIni = 'A "Data de término do contrato (aba contratação)", não foi informado!\n';
}

if ($.trim($("[name='obrdttermino']").val()) == ''){
	msgIni += 'O "Término de execução da obra (aba contratação)", não foi informado!\n';
}

if ($.trim($('[name="obrdtassinaturacontrato"]').val()) == ''){
	msgIni += 'A "Data de Assinatura do Contrato (aba contratação)" não foi informada!\n';
}

if ($.trim($('[name="obrprazovigencia"]').val()) == ''){
	msgIni += 'O "Prazo de Vigência do Contrato (aba contratação)" não foi informado!\n';
}

if ( msgIni != '' ){ alert( msgIni + 'Informe-o antes de incluir o aditivo.' ); }


function atualizaParaEdicao(){
	var obDt  = new Data();
	var obCal = new Calculo();
	
	/* Cálculos de números */
	// Campos de comparação (digitados)
	var travlraditivo 		  = $("[name='travlraditivo']");
	var travlrqtdareaalterada = $("[name='travlrqtdareaalterada']");
	var travlrqtdareaacresc   = $("[name='travlrqtdareaacresc']");
	// Campos de comparação (hidden)
	var obrcustocontrato = $('[name="obrcustocontrato"]');
	var obrqtdconstruida = $('[name="obrqtdconstruida"]');
	
	if ( obrcustocontrato[0] && travlraditivo[0] ){
		var aditivoSup	 = $("[name='trasupressao']:checked").val();
		// Ao contrário, para tornar o valor real 
		var operador = aditivoSup == 'S' ? '+' : '-'; 
		
		obrcustocontrato.val( mascaraglobal('[###.]###,##', (obCal.operacao(obrcustocontrato.val(), travlraditivo.val(), operador)) ) );
	}
	if ( obrqtdconstruida[0] && travlrqtdareaalterada[0] ){
		obrqtdconstruida.val( mascaraglobal('[###.]###,##', (obCal.operacao(obrqtdconstruida.val(), travlrqtdareaalterada.val(), '-')) ) );
	}else if ( obrcustocontrato[0] && travlrqtdareaacresc[0] ){
		obrqtdconstruida.val( mascaraglobal('[###.]###,##', (obCal.operacao(obrqtdconstruida.val(), travlrqtdareaacresc.val(), '-')) ) );
	}
	/* FIM - Cálculos de números */
	/* Cálculos de datas */
	// Campos de comparação (digitados)
	var traprazoaditivadoexec = $('[name="traprazoaditivadoexec"]');
	var traprazovigencia	  = $('[name="traprazovigencia"]');
	// Campos de comparação (hidden)
	var obrdttermino	  = $('[name="obrdttermino"]');
	var dtterminocontrato = $('[name="dtterminocontrato"]');
	
	if ( obrdttermino[0] && traprazoaditivadoexec[0] ){
		obrdttermino.val( obDt.dtAddDia(obrdttermino.val(), (- traprazoaditivadoexec.val())) );
	}
	
	if ( dtterminocontrato[0] && traprazovigencia[0] ){
		dtterminocontrato.val( obDt.dtAddDia(dtterminocontrato.val(), (- traprazovigencia.val())) );
	}
	
	/* FIM - Cálculos de datas */
}

/*
 *	Funções utilizadas no aditivo de PRAZO
 */
function dtVigencia(){
	//Em conformidade com o solicitado pelo Mário, substituição da "Data de assinatura do aditivo" por "Data de término do contrato"
	//var dt   = $('#tradtassinatura').val();
	var dt   = $("[name='dtterminocontrato']").val();
	var dia  = $("[name='traprazovigencia']").val();
	
	if ( dt && dia ){ 
		var obDt 		  = new Data();
		dtTerminoVigencia = obDt.dtAddDia(dt, dia); 
	}else{
		if ( !dt ){
			alert('O "Data de término do contrato (aba contratação)", não foi informado!\nInforme-o antes de incluir o aditivo.');
		}
		$("[name='traprazovigencia']").val('');	
		dtTerminoVigencia = ""; 
	}	
	$("[name='traterminovigencia']").val( dtTerminoVigencia );
}

function dtExecucao(){
	var dt   = $("[name='obrdttermino']").val();
	var dia  = $("[name='traprazoaditivadoexec']").val();
	
	if ( dt && dia ){ 
		var obDt 	  = new Data();
		dtTerminoExec = obDt.dtAddDia(dt, dia); 
	}else{
		if ( !dt ){
			alert('O "Término de execução da obra (aba contratação)", não foi informado!\nInforme-o antes de incluir o aditivo.');
		}
		$("[name='traprazoaditivadoexec']").val('');
		dtTerminoExec = ""; 
	}
	$("[name='traterminoexec']").val( dtTerminoExec );
}

function valDtAssinaturaContrato(){
	var msg;
	var dtAssinaturaAditivo = $("#tradtassinatura").val();
	var dtAssinaturaContrat = $('[name="obrdtassinaturacontrato"]').val();
	var diaVigenciaContrat	= $('[name="obrprazovigencia"]').val();
	
	if ( dtAssinaturaAditivo && dtAssinaturaContrat && diaVigenciaContrat ){
		var obDt = new Data();
		var dtTerminoVigenciaContrat = obDt.dtAddDia(dtAssinaturaContrat, diaVigenciaContrat);
		
		if ( !obDt.comparaData(dtAssinaturaAditivo, dtTerminoVigenciaContrat, '<=') ){
			alert('A "Data de assinatura do aditivo" deve ser MENOR ou IGUAL ao "Término da vigência do contrato ( '+dtTerminoVigenciaContrat+' - aba contratação)"');
			$("#tradtassinatura").val("");
			return false;
		}
		
		if ( !obDt.comparaData(dtAssinaturaAditivo, dtAssinaturaContrat, '>') ){
			alert('A "Data de assinatura do aditivo" deve ser MAIOR do que a "Data de Assinatura do Contrato ( '+dtAssinaturaContrat+' - aba contratação)"');
			$("#tradtassinatura").val("");
			return false;
		}
		
	}else{
		if ( dtAssinaturaContrat == "" ){
			msg = 'A "Data de Assinatura do Contrato (aba contratação)" não foi informada!\nInforme-a antes de incluir o aditivo.\n';
		}
		if ( diaVigenciaContrat == "" ){
			msg += 'O "Prazo de Vigência do Contrato (aba contratação)" não foi informado!\nInforme-o antes de incluir o aditivo.\n';
		}
		if (msg != ""){
			alert( msg );
			$("#tradtassinatura").val("");
			return false;
		}
	}
	return true;
}

function valPrazoVigenciaExecucao(){
//	var prazoVigencia = $("[name='traprazovigencia']").val() ? new Number( $("[name='traprazovigencia']").val() ) : '';
//	var prazoExecucao = $("[name='traprazoaditivadoexec']").val() ? new Number( $("[name='traprazoaditivadoexec']").val() ) : '';
	var prazoVigencia = $("[name='traterminovigencia']").val() ;
	var prazoExecucao = $("[name='traterminoexec']").val() ;
	
	if ( prazoVigencia && prazoExecucao ){
		var obDt = new Data();
		if ( obDt.comparaData(prazoVigencia, prazoExecucao, '<') ){
			alert('A "Data de Término da Vigência do Aditivo do Contrato" deve ser MAIOR ou IGUAL ao "Término da Execução do Aditivo da Obra"!');
//			alert('O "Prazo de Vigência do Aditivo do Contrato" deve ser MAIOR ou IGUAL ao "Prazo do Aditivo de Execução da Obra"!');
			return false;
		}
	}
	return true;
}

function valTerminoVigenciaExecucao(){
	var dtTerminoVigencia = $("[name='traterminovigencia']").val();
	var dtTerminoExecucao = $("[name='traterminoexec']").val();
	
	if ( dtTerminoVigencia && dtTerminoExecucao ){
		var obDt = new Data();
		if ( !obDt.comparaData(dtTerminoVigencia, dtTerminoExecucao, '>=') ){
			alert('O "Data de Término da Vigência do Aditivo do Contrato" deve ser MAIOR ou IGUAL ao "Término da Execução do Aditivo da Obra"!');
			return false;
		}
	}
	return true;
}
/*
 *	FIM - Funções utilizadas no aditivo de PRAZO
 */

/*
 * Funções usadas no aditivo de VALOR 
*/
function calculoValorFinal(){
	var vlrAditivo   = $("[name='travlraditivo']").val();
	var vlrCustoObra = $("[name='obrcustocontrato']").val();
	var aditivoSup	 = $("[name='trasupressao']:checked").val(); 
	var operador = aditivoSup == 'S' ? '-' : '+'; 
	
	if ( vlrCustoObra == "" ){
		alert('O "valor Contratado da Obra (aba contratação)" não foi informado!\nInforme-o antes de incluir o aditivo.');
		$("[name='travlraditivo']").val("");
		return false;
	}
	
	var obCal = new Calculo();
	var vlrFinal = obCal.operacao(vlrAditivo, vlrCustoObra, operador);
	vlrFinal = mascaraglobal('[###.]###,##', vlrFinal );
	$("[name='travlrfinalobra']").val( vlrFinal );
	
	return true;
}

function calculoQtdFinal(){
	var qtdConstruida = $("[name='obrqtdconstruida']")[0] 	   ? $("[name='obrqtdconstruida']").val() 	   : "";
	var qtdAcresArea  = $("[name='travlrqtdareaalterada']")[0] ? $("[name='travlrqtdareaalterada']").val() : "";
	var qtdAlterArea  = $("[name='travlrqtdareaacresc']")[0]   ? $("[name='travlrqtdareaacresc']").val()   : "";
	
	if ( qtdConstruida == "" ){
		alert('A "Área/Quantidade a ser Construída (aba contratação)" não foi informada!\nInforme-a antes de incluir o aditivo.');
		$("[name='travlrqtdareaalterada']").val("");
		$("[name='travlrqtdareaacresc']").val("");
		return false;
	}
	
	var obCal = new Calculo();
	qtd = qtdAcresArea ? qtdAcresArea : qtdAlterArea;
	
	if ( qtd != 0 && qtd != "" ){
		var qtdFinal = obCal.operacao(qtd, qtdConstruida);
		qtdFinal 	 = mascaraglobal('[###.]###,##', qtdFinal );
		$("[name='travlrqtdareafinal']").val( qtdFinal );
	}else{
		$("[name='travlrqtdareafinal']").val("");
	}	
	return true;
}

function comparacaoValAditivo(){
//	3 => "Ampliação"
//	4 => "Reforma"
//	1 => "Construção"
//	5 => "Reforma/Ampliação"

	var tipoObra 	 = $("[name='tobraid']").val();
	var vlrAditivo   = $("[name='travlraditivo']").val();
	var vlrCustoObra = $("[name='obrcustocontrato']").val();

	var obCal = new Calculo();
	
	vlrAditivo   = vlrAditivo ? obCal.converteMonetario( vlrAditivo ) : 0;
	vlrCustoObra = vlrCustoObra ? obCal.converteMonetario( vlrCustoObra ) : 0;

	var percent = (vlrAditivo / vlrCustoObra);  

	if ( ((tipoObra == 3 || tipoObra == 1) && percent >= 0.25) || ((tipoObra == 5 || tipoObra == 4) && percent >= 0.5) ){
		alert('O Valor do aditivo ultrapassa os limites do Art 65 da Lei 8.666/93 - \nO usuário deverá inserir na justificativa o enquadramento no 2º do mesmo Art 65.');
	}
}
 
/*
 * FIM -> Funções usadas no aditivo de VALOR 
*/

function valDtTermino(){
	var retorno = true;
	if ($.trim($('[name="dtterminocontrato"]').val()) == ''){
		alert('A "Data de término do contrato (aba contratação)", não foi informado!\n');
		retorno = false;
	}
	
	return retorno;
}

function validaForm(){
	try{
		$(":text").each(function (){
			if ( $(this).val() == "" ){
				alert("Preencha todos os campos obrigatórios!");
				$(this).focus();
				$(this).select();
				throw "Erro: Campo Obrigatório.";
				return false;
			}
		});
		
		$("textarea").each(function (){
			if ( $(this).val() == "" ){
				alert("Preencha todos os campos obrigatórios!");
				$(this).focus();
				$(this).select();
				throw "Erro: Campo Obrigatório.";
			}
		});
		
		if ( !valDtAssinaturaContrato() ) 	 throw "Erro: valDtAssinaturaContrato();";
		if ( !valPrazoVigenciaExecucao() ) throw "Erro: valPrazoVigenciaExecucao();";
		if ( !valTerminoVigenciaExecucao() ) throw "Erro: valTerminoVigenciaExecucao();";
		if ( !valDtTermino() ) throw "Erro: valDtTermino();";

		$('#form_aditivo').submit();		
	}catch(err){
		return false; 
	}

}

</script>
</body>
</html>








