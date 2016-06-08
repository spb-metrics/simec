<?php

if ( $_REQUEST["pdf"] == 'gerapdf' ){
	
	$_SESSION["extratoObrid"] = $_REQUEST["obrid"];
	
	require_once APPRAIZ . "www/obras/extratopdf.php";
	
	die;
	
}

// Inclusão do arquivo de permissões (somente no módulo de obras)
if ($_SESSION["sisid"] == ID_OBRAS){
	require_once APPRAIZ . "www/obras/permissoes.php";
}

// Inclusão de arquivos padrão do sistema
//require_once APPRAIZ . 'includes/Agrupador.php';

// Inclusão de arquivos do componente de Entidade 
require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

// Pega o caminho atual do usuário (em qual módulo se encontra)
$caminho_atual = $_SERVER["REQUEST_URI"];
$posicao_caminho = strpos($caminho_atual, 'acao');
$caminho_atual = substr($caminho_atual, 0 , $posicao_caminho);

$obras = new Obras();
$dobras = new DadosObra(null);

if ( $_REQUEST["obridtermo"] ){
	
	$obrid = $_REQUEST["obridtermo"];
	$dados = $obras->Dados($obrid);
	$dobras = new DadosObra($dados);
	
}else if ( $_REQUEST['obridorigem'] ){
	
	$obrid = $_REQUEST['obridorigem'];
	$dados = $obras->Dados($obrid , null, 'I');
	$dobras = new DadosObra($dados);
	
}else if ($_SESSION["obra"]["obrid"]){
	// Carrega os dados da obra 
	$obrid = $_SESSION["obra"]["obrid"];
	$dados = $obras->Dados($obrid);
	$dobras = new DadosObra($dados);
}

?>
<html>
	<head>
		<title>RELATÓRIO DE OBRA</title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
		<style type="text/css">
			body{ margin: 2; padding: 2; }
		</style>
		<style>
			 @media print {
			 .notprint { display: none }
			 }
		</style>
		<style>
		<!--
		table.ex
		{
		color:#000000;
		background-color:#f1f1f1;
		font-size: 100%;
		padding:0px;
		border-top: 1px solid black;
		border-left: 1px solid black;
		border-bottom: 0px solid black;
		border-right: 0px solid black;
		}
		
		table.ex th, table.ex td 
		{
		padding-bottom:2px;
		border-top: 0px solid black;
		border-left: 0px solid black;
		border-bottom: 1px solid black;
		border-right: 1px solid black;
		}
		-->
		</style>
		<script type="text/javascript">
			top.window.focus();
			
			function geraPDF(){
			
				var form = document.getElementById('formulario');
				var pdf  = document.getElementById('pdf');
				
				pdf.value = 'gerapdf';
				
				form.submit();
			
			}
			
		</script>
		<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
	</head>

<body>
<?php
$cabecalho='
		<!--  Cabeçalho Brasão -->
		<table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug"  style="border-bottom: 1px solid;">
			<tr bgcolor="#ffffff">
				<td valign="top" width="50" rowspan="2"><img src="../imagens/brasao.gif" width="45" height="45" border="0"></td>
				<td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
					'.$GLOBALS['parametros_sistema_tela']['sigla'].' - '.$GLOBALS['parametros_sistema_tela']['nome_completo'].'<br/>
					Monitoramento de Obras <br /> 
					'.$GLOBALS['parametros_sistema_tela']['unidade'].' / '.$GLOBALS['parametros_sistema_tela']['unidade_pai'].'<br />
				</td>
				<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">
					Impresso por: <b>'. $_SESSION['usunome'] .'</b><br/>
					Hora da Impressão: '.date( 'd/m/Y - H:i:s' ).'<br /><br />
				</td>
			</tr>
		</table>
';
echo $cabecalho;
?>
		
<form action="" method="post" name="formulario" id="formulario">
	<input type="hidden" name="pdf" id="pdf" value=""/>
	<input type="hidden" name="obrid" id="obrid" value="<?=$obrid?>"/>
	<table width="100%">
		<tr>
			<td align="right">
				<div class=notprint	><input type="button" value="Imprimir" style="cursor: pointer, info{ display: none; }" onclick="self.print();"></div>
			</td>
		</tr>
	</table>
	<table width="100%" cellspacing="1" cellpadding="1" border="0" align="center" class="tabela">		
		<tr>
			<td width="100%" align="center" colspan="2" style="background-color: #dcdcdc;">
				<br>
				<label class="TituloTela" style="color:#000000;">RELATÓRIO DE OBRA</label>
				<br><br>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top" width="30%">
				<b>Tipo de estabelecimento:</b>
			</td>
			<td>
				<?
			if($orgid = $dobras->getOrgId()){
				$sql = "SELECT orgdesc as descricao 
						FROM obras.orgao 
						WHERE orgid=".$orgid;
				echo $db->pegaUm($sql,'descricao');
			}
				?>
			</td>
    	</tr>	
    	<tr>
			<td class="SubTituloDireita" valign="top" width="30%">
				<b>Unidade ResponsÃ¡vel pela Obra:</b>
			</td>
			<td>
			<?
				$entidade = new Entidade($dobras->getEntIdUnidade());
				echo $entnome = $entidade->entnome;
			?>
			</td>
    	</tr>
    	<tr>
			<td class="SubTituloDireita" valign="top" width="30%">
				<b>Campus / Unidade:</b>
			</td>
			<td>
			<?
				$campus = new Entidade($dobras->getEntIdCampus());
				echo $campusnome = $campus->entnome;
			?>
			</td>
    	</tr>
    	<tr>
			<td class="SubTituloDireita" valign="top" width="30%">
				<b>Nome da Obra:</b>
			</td>
			<td>
			<?
				echo $dobras->getObrDesc();
			?>
			</td>
    	</tr>
    	<tr>
			<td class="SubTituloDireita" valign="top" width="30%">
				<b>Programa/Fonte:</b>
			</td>
			<td>
			<?
				if ($prfid = $dobras->getPrfId()){
					$sql = "SELECT 
								prfdesc as descricao
							FROM 
								obras.programafonte
							WHERE
								prfid= ".$prfid;
					echo $db->pegaUm($sql, 'descricao');
				}
			?>
			</td>
    	</tr>
    	<tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Classificação da Obra:</b>
			</td>
			<td>
			<?
			if ($cloid = $dobras->getCloId()){
			$sql = "
					SELECT 
						clodsc as descricao
					FROM
						obras.classificacaoobra
					WHERE
						cloid = ".$cloid;
			
				echo $db->pegaUm($sql,'descricao');
			}
			?>
			</td>
    	</tr>
    	<tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Tipologia da Obra:</b>
			</td>
			<td>
			<?
			if ($tpoid = $dobras->getTpoId()){
				$sql = "
						SELECT tpodsc as descricao 
						FROM obras.tipologiaobra
						WHERE
						tpoid = ".$tpoid;
				echo $db->pegaUm($sql,'descricao');
			}
			?>
			</td>
    	</tr>						
    	<tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Descrição / Composição da Obra:</b>
			</td>
			<td>
			<?
				echo $dobras->getObrComposicao();
			?>
			</td>
    	</tr>	
		<tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Observação sobre a Obra:</b>
			</td>
			<td>
			<?
				echo $dobras->getObsObra();
			?>
			</td>
    	</tr>	    		
    	 <tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>(%) Concluído:</b>
			</td>
			<td>
			<?
				$percentualExecutado = $obras->ViewPercentualExecutado($obrid);
				if(!$percentualExecutado) { $percentualExecutado = 0; }
				$percentualExecutado = ( $percentualExecutado > 100.00 ) ? 100.00 : $percentualExecutado;
				$percentualExecutado = number_format($percentualExecutado, 2, ',', '.');
				echo $percentualExecutado.=" %";
			?>
			</td>
    	</tr>
		</table>
		<?php
		if ($_REQUEST["localobra"]=="true"){
		?>
		<table width="100%" cellspacing="1" cellpadding="1" border="0" align="center" class="tabela">
		<tr>
			<td width="100%" align="center" colspan="2" style="background-color: #dcdcdc;">
				<br>
				<label class="TituloTela" style="color:#000000;">Local da Obra</label>
				<br><br>
			</td>
		</tr>
    	<tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>CEP:</b>
			</td>
			<td>
			<?
				echo $dobras->getEndCep();
			?>
			</td>
    	</tr>	    		
					
    	<tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Logradouro:</b>
			</td>
			<td>
			<?
				echo $dobras->getEndLog();
			?>
			</td>
    	</tr>	    		
					
    	<tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Número:</b>
			</td>
			<td>
			<?
				echo $dobras->getEndNum();
			?>
			</td>
    	</tr>	    		
					
    	<tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Complemento:</b>
			</td>
			<td>
			<?
				echo $dobras->getEndCom();
			?>
			</td>
    	</tr>	    		
					
    	<tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Bairro:</b>
			</td>
			<td>
			<?
				echo $dobras->getEndBai();
			?>
			</td>
    	</tr>	    		
					
    	<tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Município/UF:</b>
			</td>
			<td>
			<?
				echo $dobras->getMunDescricao()." / ".$dobras->getEstUf();
			?>
			</td>
    	</tr>	    		
		</table>
		<?php
		}
		if ($_REQUEST["coordenadas"]=="true"){
		?>
		<table width="100%" cellspacing="1" cellpadding="1" border="0" align="center" class="tabela">
		<tr>
			<td width="100%" align="center" colspan="2" style="background-color: #dcdcdc;">
				<br>
				<label class="TituloTela" style="color:#000000;">Coordenadas Geográfica</label>
				<br><br>
			</td>
		</tr>
    	<tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Latitude:</b>
			</td>
			<td>
			<?
			$longitude = explode(".", $dobras->getMedLongitude());
			$graulongitude = $longitude[0];
			$minlongitude = $longitude[1];
			$seglongitude = $longitude[2];
			
			$latitude = explode(".", $dobras->getMedLatitude());
			$graulatitude = $latitude[0];
			$minlatitude = $latitude[1];
			$seglatitude = $latitude[2];
			$pololatitude = $latitude[3];

			// Latitude
			echo $graulatitude."° ".$minlatitude."' ".$seglatitude."'' "."  ".$pololatitude;
			?>
			</td>
    	</tr>	    		
    	<tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Longitude:</b>
			</td>
			<td>
			<?
			// Longitude
			echo $graulongitude."° ".$minlongitude."' ".$seglongitude."''";
			?>
			</td>
    	</tr>	
    	</table>    		
		<table align="center" >
		<tr align="center" >
		<td valign="top" colspan=2 align="center" >
		<center>
		<?php
		$latitude = ((( $seglatitude / 60 ) + $minlatitude) / 60 ) + $graulatitude;
		$longitude = ((( $seglongitude / 60 ) + $minlongitude) / 60 ) + $graulongitude;
		if ($_REQUEST["coordenadas_img"]=="true"){
			if ($latitude and $longitude){	
				if($pololatitude == "N"){ ?>
					<iframe width="400" height="400" frameborder="0" scrolling="no" marginheight="0" marginwidth="0" 
					src="http://maps.google.com/?ie=UTF8&amp;t=k&amp;ll=<?php echo $latitude; ?>,-<?php echo $longitude; ?>&amp;spn=0.009417,0.013304&amp;z=17&amp;output=embed&amp;s=AARTsJqzARj-Z8VnW5pkPMLMmZbqrJcYpw">
					</iframe>
				<?php }else { ?>
				
					<iframe width="400" height="400" frameborder="0" scrolling="no" marginheight="0" marginwidth="0" 
					src="http://maps.google.com/?ie=UTF8&amp;t=k&amp;ll=-<?php echo $latitude; ?>,-<?php echo $longitude; ?>&amp;spn=0.009417,0.013304&amp;z=17&amp;output=embed&amp;s=AARTsJqzARj-Z8VnW5pkPMLMmZbqrJcYpw">
					</iframe>
				<?php } ?>
			<?php } else {
			echo "Dados de localização não cadastrados.";
			}
		}?>
		</center>
		</td>
		</tr>
		</table>
		<?php
		}
		if ($_REQUEST["contatos"]=="true"){
		?>
		<table width="100%" cellspacing="1" cellpadding="1" border="0" align="center" class="tabela">
			<tr>
				<td width="100%" align="center" colspan="2" style="background-color: #dcdcdc;">
					<br>
					<label class="TituloTela" style="color:#000000;">Contatos</label>
					<br><br>
				</td>
			</tr>
			<tr> 
				<td>			
					<?php
						$sql = "";
						$sql = "SELECT
									et.entnumcpfcnpj as cpf,
									et.entnome as nome,
									et.entemail as email,
									'(' || et.entnumdddcomercial || ') ' || et.entnumcomercial as telefone,
									tr.tprcdesc as tipo_desc
								FROM 
									obras.responsavelobra r 
								INNER JOIN 
									obras.responsavelcontatos rc ON r.recoid = rc.recoid 
								INNER JOIN 
									entidade.entidade et ON rc.entid = et.entid 
								LEFT JOIN 
									obras.tiporespcontato tr ON rc.tprcid = tr.tprcid
								WHERE 
									r.obrid = '". $obrid . "'  AND 
									rc.recostatus = 'A'";
						
						$cabecalho = array("CPF", "Nome do Responsável", "E-mail", "Telefone", "Tipo de Responsabilidade");
						$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%', 'S' );
						
					?>
				</td>
	    	</tr>	    		
		</table>
		<?php
		}
		if ($_REQUEST["contratacao"]=="true"){
		?>
		<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
		<table width="100%" cellspacing="1" cellpadding="1" border="0" align="center" class="tabela">
		<tr>
			<td width="100%" align="center" colspan="2" style="background-color: #dcdcdc;">
				<br>
				<label class="TituloTela" style="color:#000000;">Contratação</label>
				<br><br>
			</td>
		</tr>
		<tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Nome da Obra :</b>
			</td>
			<td>
			<?
			$obra = $obras->ViewObra($obrid);
			echo $obra['nome'];
			?>
			</td>
    	</tr>	
    	 <tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Valor do Contrato (R$):</b>
			</td>
			<td>
			<?
				echo number_format($obra['obrcustocontrato'],2,',','.');
			?>
			</td>
    	</tr>	
    	 <tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>(%) Concluído:</b>
			</td>
			<td>
			<?
				$percentualExecutado = $obras->ViewPercentualExecutado($obrid);
				if(!$percentualExecutado) { $percentualExecutado = 0; }
				$percentualExecutado = ( $percentualExecutado > 100.00 ) ? 100.00 : $percentualExecutado;
				$percentualExecutado = number_format($percentualExecutado, 2, ',', '.');
				echo $percentualExecutado.=" %";
			?>
			</td>
    	</tr>
    	<tr>
		<td class="SubTitulo" valign="top" width="30%">
				<b>	Sobre a Obra</b>
			</td>
		</tr>
		 <tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Tipo de Obra:</b>
			</td>
			<td>
			<?
				if ($tobaid =$dobras->getTobraId()){
					$sql = "
						SELECT tobadesc AS descricao 
						FROM obras.tipoobra
						WHERE
						tobaid =".$tobaid;
					echo $db->pegaUm($sql,'descricao');
				}
			?>
			</td>
    	</tr>				
		 <tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Situação da Obra:</b>
			</td>
			<td>
			<?

			?>
			</td>
    	</tr>
    	<tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Início programado para:  	</b>
			</td>
			<td>
			<?
				echo formata_data($dobras->getObrDtInicio(), "d/m/Y");
			?>
			</td>
    	</tr>				
		 <tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Término programado para:</b>
			</td>
			<td>
			<?
				echo formata_data($dobras->getObrDtTermino());
			?>
			</td>
    	</tr>			 <tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Área/Quantidade a ser Construída:</b>
			</td>
			<td>
			<?
				if ($umdid=$dobras->getUmdIdObraConstruida()){
					$sql = "
						SELECT umdeesc AS descricao 
							FROM obras.unidademedida
						WHERE umdid =".$umdid;
					$umdid=$db->pegaUm($sql,'descricao');
					
					$obrqtdconstruida = $dobras->getObrQtdConstruida();
					echo $obrqtdconstruida = number_format($obrqtdconstruida,2,',','.') ." ".$umdid;
				}
			?>
			</td>
    	</tr>			 <tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Custo Unitário R$:</b>
			</td>
			<td>
			<?
				echo number_format($dobras->getObrCustoUnitQtdConstruida(),2,',','.');
			?>
			</td>
    	</tr>			 <tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Percentual BDI:</b>
			</td>
			<td>
			<?
				echo number_format($dobras->getObrPercBdi(),2,',','.');
			?>
			</td>
    	</tr>			 <tr>
		<tr>
		<td class="SubTitulo" valign="top" width="30%">
				<b>Sobre a Inauguração</b>
			</td>
		</tr>
		<tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Inaugurada:</b>
			</td>
			<td>
				<?
				$obrstatusinauguracao = $dobras->getObrStatusInauguracao();	
				if($obrstatusinauguracao == "S") echo "Não se Aplica";
				if($obrstatusinauguracao == "N") echo "Não Inaugurada";
				if($obrstatusinauguracao == "I") echo "Inaugurada";
				if($dobras->getObrDtInauguracao())				
					echo "<br> Data de Inauguração: ".formata_data($dobras->getObrDtInauguracao());
				if($dobras->getObrDtPrevInauguracao())				
					echo "<br> Data de Previsão da Inauguração: ".formata_data($dobras->getObrDtPrevInauguracao());
				?> 
			</td>
    	</tr>
		
		<tr>
		<td class="SubTitulo" valign="top" width="30%">
				<b>Contratação da Obra</b>
			</td>
		</tr>
		<?php
				$empresa = new Entidade($dobras->getEntIdEmpresaConstrutora());
				$entnomeempresa = $empresa->entnome;
				if ($dobras->getEntIdEmpresaConstrutora()){
				$sql = '';
				$sql = "SELECT
							entemail as email,
							entnumdddcomercial as ddd,
							entnumcomercial as telefone,
							ed.endlog || ' nº ' || ed.endnum || ', ' || ed.endbai || ' - ' || tm.mundescricao || ', ' || ed.estuf as endereco
						FROM 
							entidade.entidade e
						LEFT JOIN
							entidade.endereco ed ON e.entid = ed.entid 
						LEFT JOIN
							territorios.municipio tm ON ed.muncod = tm.muncod 
						WHERE 
							e.entid = " . $dobras->getEntIdEmpresaConstrutora();
				
				}
				$dados = "";
				$dados = $db->carregar($sql);
				
				
				if ( is_array($dados) ){
					$emailempresa 	 = $dados[0]['email'];
					$enderecoempresa = $dados[0]['endereco'];
					$dddempresa 	 = $dados[0]['ddd'];
					$telefoneempresa = $dados[0]['telefone'];
					$naturezaempresa = $dados[0]['natureza'];	
				}
				
			?>
    	<tr>
			<td class="SubTituloDireita" valign="top" width="30%">
				<b>Empresa Contratada:</b>
			</td>
			<td>
			<?php echo $entnomeempresa; ?>
			</td>
    	</tr>
    	<tr>
			<td class="SubTituloDireita" valign="top" width="30%">
				<b>Endereço</b>
			</td>
			<td>
			<?php echo $enderecoempresa; ?>
			</td>
    	</tr>
    	<tr>
			<td class="SubTituloDireita" valign="top" width="30%">
				<b>Telefone:</b>
			</td>
			<td>
			<?php echo '(' . trim($dddempresa) . ') ' . $telefoneempresa; ?>
			</td>
    	</tr>
    	<tr>
			<td class="SubTituloDireita" valign="top" width="30%">
				<b>E-mail:</b>
			</td>
			<td>
				<?php echo $emailempresa; ?>
			</td>
    	</tr>
    	<tr>
  			<td class="SubTitulo" valign="top" width="30%">
				<b>	Fases de Licitação</b>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"></td>
			<td>
			<?
				$sql = pg_query("
								SELECT 
									fl.*,
									tfl.tfldesc, tfl.tflordem   
								FROM 
									obras.faselicitacao fl 
								INNER JOIN 
									obras.tiposfaseslicitacao tfl ON fl.tflid = tfl.tflid
								WHERE 
									fl.obrid = '". $obrid . "' AND fl.flcstatus = 'A' ORDER BY tfl.tflordem");
						
				echo '<table cellspacing="0" cellpadding="2" border="0" width="50%" class="listagem" style="color: rgb(51, 51, 51);">'
					 . '<thead>'
					 . '<tr>'
					 . '<td class="title" align="center" valign="top" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Fase</td>'
					 . '<td class="title" align="center" valign="top" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Data</td>'
					 . '</thead>';
						while (($dados = pg_fetch_assoc($sql))){
								
								$cor = $i % 2 ? '#f7f7f7' : '#ffffff';
								
								$flcid = $dados['flcid'];
								$tflid = $dados['tflid'];
								$tfldesc = $dados['tfldesc'];
								$flcrecintermotivo = $dados['flcrecintermotivo'];
								$flcordservnum = $dados['flcordservnum'];
								$flcpubleditaldtprev = formata_data($dados['flcpubleditaldtprev']);
								$flcdtrecintermotivo = formata_data($dados['flcdtrecintermotivo']);
								$flcordservdt = formata_data($dados['flcordservdt']);
								$flchomlicdtprev = formata_data($dados['flchomlicdtprev']);
								$flcaberpropdtprev = formata_data($dados['flcaberpropdtprev']);

								
								if($tflid ==2){
									$flcdata = $flcpubleditaldtprev;
								}
								if($tflid ==5){
									$flcdata = $flcdtrecintermotivo;
								}
								if($tflid ==6){
									$flcdata = $flcordservdt;
								}
								if($tflid ==9){
									$flcdata = $flchomlicdtprev;
								}
								if($tflid ==7){
									$flcdata = $flcaberpropdtprev;
								}
								echo "
								<tr bgColor=".$cor.">
									<td>" .$tfldesc."</td>
									<td> &nbsp &nbsp " . $flcdata."</td>
								</tr>
								";
								$i++;
							}
							echo "</table>";
			?>
			</td>
    	</tr>
    	<tr>	
		<td class="SubTitulo" valign="top" width="30%">
			<b>Forma de Repasse de Recursos</b>
			</td>
    	</tr>
	  	<tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Tipo:</b>
			</td>
			<td>
			<?
			
			if($dobras->getFrpId()){
			$frpid = $dobras->getFrpId();
					$sql = "
						SELECT frpdesc AS descricao FROM 
							obras.tipoformarepasserecursos
						WHERE frpid =".$frpid;
			echo $db->pegaUm($sql,'descricao');
			}
			?>
			</td>
    	</tr>
	  	<tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Detalhes:</b>
			</td>
			<td>
			<?
			if ($frpid==2){
				$covid = $dobras->getCovId();
				if ($covid){
									
					$dados_convenio = $db->pegaLinha("SELECT
														*
													  FROM 
													  	obras.conveniosobra
													  WHERE
													  	covid = '{$covid}'");
					
					$dados_aditivo = "SELECT
									  	covano, covobjeto, covprocesso,
									  	to_char(covdtinicio, 'DD/MM/YYYY') as inicio, 
									  	to_char(covdtfinal, 'DD/MM/YYYY') as fim, 
									  	covvalor
									  FROM
									  	obras.conveniosobra
									  WHERE
									  	covnumero = '{$dados_convenio['covnumero']}' AND
									  	covtipo = 'A'";
					$dados_apostilamento = "SELECT
											 	covano, covobjeto, covprocesso,
											  	to_char(covdtinicio, 'DD/MM/YYYY') as inicio, 
											  	to_char(covdtfinal, 'DD/MM/YYYY') as fim, 
											  	covvalor
											FROM
											  	obras.conveniosobra
											WHERE
											  	covnumero = '{$dados_convenio['covnumero']}' AND
											  	covtipo = 'P'";
					
						}
								
						$covnumero = $dados_convenio["covnumero"];
						
				echo'  
				<table>
					<tr>
					<td class="SubTituloDireita" valign="top">Número do Convênio</td><td> '.$covnumero.' </td>  	
					</tr>
					<tr>
					<td class="SubTituloDireita" valign="top">Ano</td><td> '.formata_data($dados_convenio["covano"]).' </td>
					</tr> 	
					<tr>
					<td class="SubTituloDireita" valign="top">Objeto</td><td> '.$dados_convenio["covobjeto"].' </td>
					</tr>
					<tr> 	
					<td class="SubTituloDireita" valign="top">Detalhamento</td><td> '.$dados_convenio["covdetalhamento"].' </td>
					</tr>
					<tr> 	
					<td class="SubTituloDireita" valign="top">Processo</td><td> '.$dados_convenio["covprocesso"].' </td>
					</tr>
					<tr> 	
					<td class="SubTituloDireita" valign="top">Concedente</td><td> '.$dados_convenio["covvlrconcedente"].' </td>
					</tr>
					<tr> 	
					<td class="SubTituloDireita" valign="top">Convenente</td><td> '.number_format($dados_convenio["covvlrconcedente"],2,',','.').' </td>
					</tr>
					<tr> 	
					<td class="SubTituloDireita" valign="top"Valor R$</td><td> '.number_format($dados_convenio["covvlrconvenente"],2,',','.').' </td>
					</tr>
					<tr>
					<td class="SubTituloDireita" valign="top">Início</td><td> '.formata_data($dados_convenio["covdtinicio"]).' </td>
					</tr>
					<tr>
					<td class="SubTituloDireita" valign="top">Fim</td><td> '.formata_data($dados_convenio["covdtfinal"]).' </td>
					</tr>
				</table>';

				if ($covid){
				
					echo "Aditivo";
					$cabecalho = array("Ano", "Objeto", "Processo", "Data de Início", "Data de Término", "Valor" );
					$db->monta_lista( $dados_aditivo, $cabecalho, 5, 10, 'N', 'center', '' );
				
					echo "Apostilamento";
					$cabecalho = array("Ano", "Objeto", "Processo", "Data de Início", "Data de Término", "Valor" );
					$db->monta_lista( $dados_apostilamento, $cabecalho, 5, 10, 'N', 'center', '' );
				
				}
			}
			if ($frpid==3){
				$frrdescinstituicao = $dobras->getFrrDescInstituicao();
							if($frrdescinstituicao == ""){
								$dados2 = $obras->ViewObra($obrid);
								$dobras->setFrrDescInstituicao($dados2["entidade"]);
							}
				$frrdescinstituicao = $dobras->getFrrDescInstituicao();
				$frrdescnumport = $dobras->getFrrDescNumPort();
				$frrdescobjeto = $dobras->getFrrDescObjeto();
				$frrdescvlr = number_format($dobras->getFrrDescVlr(), 2, ',', '.');
			echo'  
				<table>
					<tr>
					<td class="SubTituloDireita" valign="top">Instituição</td><td> '.$frrdescinstituicao.' </td>  	
					</tr>
					<tr>
					<td class="SubTituloDireita" valign="top">Número da Portaria de Descentralização</td><td> '.$frrdescnumport.' </td>
					</tr> 	
					<tr>
					<td class="SubTituloDireita" valign="top">Objeto</td><td> '.$frrdescobjeto.' </td>
					</tr>
					<tr> 	
					<td class="SubTituloDireita" valign="top">Valor R$</td><td> '.$frrdescvlr.' </td>
					</tr>
				</table>';					 	
			}
			if ($frpid==3){
			echo'  
				<table>
					<tr>
					<td class="SubTituloDireita" valign="top">Observação:</td><td> '.$dobras->getFrrObsRecProprio().' </td>  	
					</tr>
				</table>';					 	
			}
			?>
			</td>
    	</tr>
		</table>
		<?php
		}
		if ($_REQUEST["infra"]=="true"){
		?>
		<table width="100%" cellspacing="1" cellpadding="1" border="0" align="center" class="tabela">
		<tr>
			<td width="100%" align="center" colspan="2" style="background-color: #dcdcdc;">
				<br>
				<label class="TituloTela" style="color:#000000;">Infra-Estrutura</label>
				<br><br>
			</td>
		</tr>
		<tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Situação Dominial já Regularizada?:</b>
			</td>
			<td>
			<?
			$infraestrutura = new DadosInfraEstrutura();
			$resultado = $infraestrutura->busca($obrid);	
			$dados = $infraestrutura->dados($resultado);
			if ($infraestrutura->iexsitdominialimovelregulariza == "t"){ 
				echo "Sim";
			} else {
				echo "Não";				
			}
			?>
			</td>
    	</tr>	
    	 <tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Existem Edificações no Local da Obra?:</b>
			</td>
			<td>
			<?
			if ($infraestrutura->iexinfexistedimovel == "t"){ 
				echo "Sim";
			} else {
				echo "Não";				
			}
			?>
			</td>
    	</tr>	
    	 <tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Área Construída:</b>
			</td>
			<td>
			<?
			if ($infraestrutura->umdidareaconstruida){
			$sql = "SELECT 
						umdid AS codigo, 
						umdeesc AS descricao 
					FROM 
						obras.unidademedida
					WHERE umdid=".$infraestrutura->umdidareaconstruida;
			$umdidareaconstruida= $db->pegaUm($sql,'descricao');
			echo number_format($infraestrutura->iexareaconstruida,2,',','.')." ".$umdidareaconstruida;
			}
			?>
			</td>
    	</tr>	
    	 <tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Descrição Sumária da Edificação:</b>
			</td>
			<td>
			<?
				echo $infraestrutura->iexdescsumariaedificacao;
			?>
			</td>
    	</tr>	
    	 <tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>A(s) Edificaçõe(s) Necessita(m) de Reforma(s)?:</b>
			</td>
			<td>
			<?
			if ($infraestrutura->iexqtdareapreforma == "t"){ 
				echo "Sim";
			} else {
				echo "Não";				
			}
			?>
			</td>
    	</tr>	
    	 <tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Há Necessidade de Ampliação?:</b>
			</td>
			<td>
			<?
			if ($infraestrutura->iexqtdareaampliada == "t"){ 
				echo "Sim";
			} else {
				echo "Não";				
			}			?>
			</td>
    	</tr>	
		</table>
		<?php
		}
		if ($_REQUEST["projetos"]=="true"){
		?>
		<table width="100%" cellspacing="1" cellpadding="1" border="0" align="center" class="tabela">
		<tr>
			<td width="100%" align="center" colspan="2" style="background-color: #dcdcdc;">
				<br>
				<label class="TituloTela" style="color:#000000;">Projetos</label>
				<br><br>
			</td>
		</tr>
		<tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Tipo de Projeto:</b>
			</td>
			<td>
			<?
			$faseprojeto = new DadosFasesProjeto();
			$resultado = $faseprojeto->busca($obrid);	
			$dados = $faseprojeto->dados($resultado);
			
			if ($tpaid = $faseprojeto->tpaid){
				$sql = "SELECT 
							tpadesc as descricao 
						FROM 
							obras.tipoprojetoarquitetonico
						WHERE tpaid=".$tpaid;
				echo $db->pegaUm($sql,'descricao');
			}
			?>
			</td>
    	</tr>	
		<tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Forma de Elaboração do projeto:</b>
			</td>
			<td>
			<?
			if ($felid = $faseprojeto->felid){
			$sql = "SELECT 
						feldesc as descricao 
					FROM 
						obras.formaelaboracao
					WHERE felid=".$felid;
			echo $db->pegaUm($sql,'descricao');
			}
			?>
			</td>
    	</tr>	
		<tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Observações:</b>
			</td>
			<td>
			<?
			if ($faseprojeto->felid==1){
				echo $faseprojeto->fprobsexecdireta;
			}
			if ($faseprojeto->felid==2){
				echo'  
				<table>
					<tr>
					<td class="SubTituloDireita" valign="top">Recurso Próprio R$:</td><td> '.number_format($faseprojeto->fprvlrformaelabrecproprio,2,',','.').' </td>  	
					</tr>
					<tr>
					<td class="SubTituloDireita" valign="top">Recurso Repassado R$</td><td> '.number_format($faseprojeto->fprvlrformaelabrrecrepassado,2,',','.').' </td>  	
					</tr>
				</table>';			
			}
			if ($faseprojeto->felid==3){
				echo $faseprojeto->fprobsexecdireta;
			}			
			if ($faseprojeto->felid==4){
				echo $faseprojeto->fprobsexecdireta;
			}
			?>
			</td>
    	</tr>	
		<tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Fases do Projeto:</b>
			</td>
			<td>
			<?
			if ($tfpid = $faseprojeto->tfpid){
			$sql = "SELECT 
						tfpdesc as descricao 
					FROM 
						obras.tipofaseprojeto
					WHERE tfpid=".$tfpid;
			echo $db->pegaUm($sql,'descricao');
			}
			?>
			</td>
    	</tr>	
		<tr>
		<td class="SubTituloDireita" valign="top" width="30%">
				<b>Previsao / Conclusão:</b>
			</td>
			<td>
			<?
			if ($tfpid==1){
				echo formata_data($faseprojeto->fprdtiniciofaseprojeto);				
			}
			if ($tfpid==2){
				echo formata_data($faseprojeto->fprdtprevterminoprojeto);				
			}
			if ($tfpid==3){
				echo formata_data($faseprojeto->fprdtconclusaofaseprojeto);				
			}
			?>
			</td>
    	</tr>	
		</table>
		<?php
		}
		if ($_REQUEST["etapas"]=="true"){
		?>
		<table width="100%" cellspacing="1" cellpadding="1" border="0" align="center" class="tabela">
		<tr>
			<td width="100%" align="center" colspan="2" style="background-color: #dcdcdc;">
				<br>
				<label class="TituloTela" style="color:#000000;">Etapas da Obra</label>
				<br><br>
			</td>
		</tr>
		<tr>
			<td colspan=2>
			<?

			$sql = "SELECT 
						i.itcid,
						i.icovlritem,
						i.icopercsobreobra,
						i.icopercexecutado,
						ic.itcdesc,
						ic.itcdescservico
					FROM 
						obras.itenscomposicaoobra i,
						obras.itenscomposicao ic 
					WHERE 
						i.obrid = " . $obrid . " 
						and i.itcid = ic.itcid 
					ORDER BY 
						i.icoordem";
			
			$count = 1;
			$soma  = 0;
			$somav = 0;
			
			$controleLinha = 1;
			
			$dado = $db->carregar($sql);
			$dado = $dado ? $dado : array(); 

			echo "<table class=tabela align=center width=100%>"
				."	<tr>"
				."		<td align=\"center\"><b>Descrição</b></td>"
				."		<td align=\"center\"><b>Valor do Item (R$)</b></td>"
				."		<td align=\"center\"><b>(%) Referente a Obra</b></td>"
				."	</tr>";
			
			foreach($dado as $dados) {
				
				$itcid 		= $dados['itcid'];
				$icovlritem = $dados['icovlritem'];
				$itcdesc 	= $dados['itcdesc'];
				$icopercsobreobra = $dados['icopercsobreobra'];
				$icopercexecutado = $dados['icopercexecutado'];
				$itcdescservico   = $dados['itcdescservico'];
				
				$somav 		= bcadd($somav, $icovlritem, 2);
				$icovlritem = number_format($icovlritem,2,',','.'); 
				$soma 		= round( $soma, 2 ) + round( $icopercsobreobra, 2 );
				
				$icopercsobreobra = number_format( $icopercsobreobra, 2);
				$icopercsobreobra = str_replace( '.', ',', $icopercsobreobra );
				
				$cor = $count % 2 ? "#e0e0e0" : '#ffffff'; 
								
				echo "<tr  bgColor=\"{$cor}\">
						<td>
							$itcdesc	
						</td>
						<td align=\"right\">
							" . $icovlritem . "					
						</td>
						<td align=\"right\">					
							" . $icopercsobreobra . " %
						</td>
					</tr>";

				$count++;
			
			}
			
		echo "<tr>
				<td><b>Total</b></td>
				<td align=right><b>".number_format($somav,2,',','.')."</b></td>
				<td align=right	><b>".number_format( $soma,2,',','.')."</b></td>
			  </tr>";
				?>
		</table>
		
		<?php
		}
		if ($_REQUEST["cronograma"]=="true"){
		?>
		<table width="100%" cellspacing="1" cellpadding="1" border="0" align="center" class="tabela">
		<tr>
			<td width="100%" align="center" colspan="2" style="background-color: #dcdcdc;">
				<br>
				<label class="TituloTela" style="color:#000000;">Cronograma Físico-Financeiro </label>
				<br><br>
			</td>
		</tr>
		<tr>
		<td colspan=2 align=center>
			<?
			$sql = pg_query("SELECT
								oi.obrpercexec,
								itco.icoid,
								itc.itcid,
								itc.itcdesc,
								itc.itcdescservico,
								itco.icopercsobreobra,
								itco.icovlritem,
								itco.icodtinicioitem,
								itco.icodterminoitem,
								itco.icopercexecutado
							FROM 
								obras.itenscomposicao itc
							INNER JOIN 
								obras.itenscomposicaoobra itco ON itc.itcid = itco.itcid
							INNER JOIN
								obras.obrainfraestrutura oi ON oi.obrid = itco.obrid
							WHERE 
								itco.obrid =".$obrid."
							ORDER BY 
								itco.icoordem");
			echo "<table>
					<tr><td><b>Item da Obra</b></td>
					<td><b>(%) Sobre a Obra (A)</b></td>
					<td><b>Valor (R$)</b></td>
					<td><b>	Data de Início</b></td>
					<td><b>Data de Término</b></td>
					<td><b>(%) Executado do Item Sobre a Obra(B)</b></td>
					<td><b>(%) do Item Executado(B x 100 / A)</b></td>
					</tr>";	
			$icopercexecutado=0;
			$icovlritem=0;
			while (($dados = pg_fetch_assoc($sql))){
				$icopercexecutado=$icopercexecutado+$dados['icopercsobreobra'];
				$icovlritem=$icovlritem+$dados['icovlritem'];
				$porcento_executado = $dados['obrpercexec'];	
				$icopercsobreobra=$icopercsobreobra+$dados['icopercsobreobra'];
				echo "<tr>
						<td>" .$dados['itcdesc']."</td>
						<td align=right>" .number_format( $dados['icopercsobreobra'],2,',','.')."</td>
						<td align=right>" .number_format( $dados['icovlritem'],2,',','.')."</td>
						<td align=right>" .formata_data($dados['icodtinicioitem'])."</td>
						<td align=right>" .formata_data($dados['icodterminoitem'])."</td>
						<td align=right>" .number_format( $dados['icopercsobreobra'],2,',','.')."</td>
						<td align=right>" .number_format( $porcento_executado,2,',','.')."</td>					
					</tr>";		
				
			
			}
			echo "
				<tr><td><b>Total</b></td>
				<td align=right><b>".number_format( $icopercexecutado,2,',','.')."</b></td>
				<td align=right	><b>".number_format( $icovlritem,2,',','.')."</b></td>
				<td align=right	><b></b></td>
				<td align=right	><b></b></td>
				<td align=right	><b>".number_format( $icopercexecutado_t,2,',','.')."</b></td>				
				<td align=right	><b></b></td>				
				</tr></table>";
				?>
			</td>
    	</tr>	    	
    	</table>
		<?php
		}
		if ($_REQUEST["galeria"]=="true"){
		?>
		<div class=QuebraDePagina style="page-break-before: always">
		<table width="100%" cellspacing="1" cellpadding="1" border="0" align="center" class="tabela">
		<tr align="center" >
			<td width="100%" align="center" colspan="2" style="background-color: #dcdcdc;">
				<br>
				<label class="TituloTela" style="color:#000000;">Galeria de Fotos</label>
				<br><br>
			</td>
		</tr>
		<tr>
 		<td valign="top" width="30%" colspan=2 align=center >
			<?
			
			if ($_REQUEST["sel_fotos_galeria"]!="0"){
			$sql = "SELECT arqnome, arq.arqid, arq.arqextensao, arq.arqtipo, arq.arqdescricao FROM public.arquivo arq
					INNER JOIN obras.arquivosobra oar ON arq.arqid = oar.arqid
					INNER JOIN obras.obrainfraestrutura obr ON obr.obrid = oar.obrid 
					INNER JOIN seguranca.usuario seg ON seg.usucpf = oar.usucpf 
					WHERE arq.arqid IN(".$_REQUEST["sel_fotos_galeria"].") AND obr.obrid = {$obrid} AND
						  aqostatus = 'A' AND
						  (arqtipo = 'image/jpeg' OR 
						   arqtipo = 'image/gif' OR 
						   arqtipo = 'image/png') 
					ORDER BY arq.arqid ";
			} else {
			$sql = "SELECT arqnome, arq.arqid, arq.arqextensao, arq.arqtipo, arq.arqdescricao FROM public.arquivo arq
					INNER JOIN obras.arquivosobra oar ON arq.arqid = oar.arqid
					INNER JOIN obras.obrainfraestrutura obr ON obr.obrid = oar.obrid 
					INNER JOIN seguranca.usuario seg ON seg.usucpf = oar.usucpf 
					WHERE obr.obrid = {$obrid} AND
						  aqostatus = 'A' AND
						  (arqtipo = 'image/jpeg' OR 
						   arqtipo = 'image/gif' OR 
						   arqtipo = 'image/png') 
					ORDER BY arq.arqid DESC LIMIT ".$_REQUEST["num_fotos_galeria"];
			}
			$fotos = ($db->carregar($sql));

			if($fotos){
					$_SESSION['imgparams'] = array("filtro" => "cnt.obrid=".$obrid." AND aqostatus = 'A'", "tabela" => "obras.arquivosobra");
					echo "<table>";
					for($i=0;$i < count($fotos);$i++){
							if ($fotos[$i]["arqid"]){
							echo "<tr>
									<td> 
									<img src='../slideshow/slideshow/verimagem.php?newwidth=225&newheight=225&arqid=".$fotos[$i]["arqid"]."' 
									hspace='3' vspace='3' style='width:225px; height:225px;'\n
									<br>".$fotos[$i]["arqdescricao"]."
									</td>";
							}
							$i=$i+1;
							if ($fotos[$i]["arqid"]){
							echo "
									<td> 
									<img src='../slideshow/slideshow/verimagem.php?newwidth=225&newheight=225&arqid=".$fotos[$i]["arqid"]."' 
									hspace='3' vspace='3' style='width:225px; height:225px;'\n
									<br>".$fotos[$i]["arqdescricao"]."
									</td>";
							}
							$i=$i+1;
							if ($fotos[$i]["arqid"]){
							echo "<td> 
									<img src='../slideshow/slideshow/verimagem.php?newwidth=225&newheight=225&arqid=".$fotos[$i]["arqid"]."' 
									hspace='3' vspace='3' style='width:225px; height:225px;'\n
									<br>".$fotos[$i]["arqdescricao"]."
									</td>
									</tr>";								
							}
						}
						echo "</table>";
				} else {
					echo "Não existem fotos cadastradas";
				}			
			?>
		</td>
    	</tr>
	</table>
	</div>
		<?php
		}
		if ($_REQUEST["vistoria"]=="true"){

			// Separando os IDs das Quantidades de Fotos
			$id_vistoria=explode("_",$_REQUEST["vistorias"],-1);
			//dbg($id_vistoria);exit;
			foreach ($id_vistoria as $valor) {
				$valor2=explode(".",$valor);
				$fotos_vistoria[$valor2[0]]=$valor2[1];
				$id_vistorias.=$valor2[0].",";
			}
			$id_vistorias.="0";
			
			if ($id_vistorias!=",0"){	$sql = "
					SELECT
						s.*,
						to_char(s.supvdt,'DD/MM/YYYY') as dtvistoria,
						to_char(s.supdtinclusao,'DD/MM/YYYY') as dtinclusao,						
						u.usunome,
						si.stodesc,
						s.suprealizacao as responsavel,
						s.supvid,
						s.usucpf
					FROM
						obras.supervisao s
					INNER JOIN 
						obras.situacaoobra si ON si.stoid = s.stoid
					INNER JOIN
						seguranca.usuario u ON u.usucpf = s.usucpf
					WHERE
						s.supvid IN (".$id_vistorias.") AND
						s.obrid = '" . $obrid . "' AND
						s.supstatus = 'A'
					ORDER BY 
						s.supdtinclusao ASC";
				//echo $sql;
				
			$dados = $db->carregar($sql);
			}else{
				$dados=false;			   
			}
				
				// Separando os IDs das Fotos a serem selecionadas
				$selecao_fotos_vistoria=explode("_",$_REQUEST["selecao_fotos_vistoria"]);
				
				foreach ($selecao_fotos_vistoria as $valor) {
					$valor2=explode(".",$valor);
					if ($valor2[0]!=""){
						$selecao_fotos_vistoria_array[$valor2[0]]=$valor2[1];
					}
				}
				//dbg($selecao_fotos_vistoria_array);exit;
				
				if($dados){					  
					echo '<table>';

					for($i=0;$i < count($dados);$i++){
						$ordem=$i+1;
						$supvid=$dados[$i]["supvid"];
						echo '<div class=QuebraDePagina style="page-break-before: always">';
						echo "
							<table width='80%' cellspacing=1 cellpadding=1 border=0 align='center' class='tabela'>
							<tr>
								<td align='center' colspan='2' style='background-color: #dcdcdc;'>
									<br>
									<label class='TituloTela' style='color:#000000;'>Vistoria da Obra </label>
									<br><br>
								</td>
							</tr>
							<tr>
							<td  style='background-color: #dcdcdc;'>
								<b>Dados da Vistoria Nº $ordem</b>
								
								<tr>
									<td class=SubTituloDireita width=30%>Data Vistoria</td>
									<td>".$dados[$i]["dtvistoria"]."</td>
								</tr>
								<tr>
									<td class=SubTituloDireita>Data Inclusão</td>
									<td>".$dados[$i]["dtinclusao"]."</td>
								</tr>
																<tr>
									<td class=SubTituloDireita>Responsável</td>
									<td>".$dados[$i]["responsavel"]."</td>
								</tr>
																<tr>
									<td class=SubTituloDireita>Situação da Obra</td>
									<td>".$dados[$i]["stodesc"]."</td>
								</tr>
																<tr>
									<td class=SubTituloDireita>Realizada Por</td>
									<td>".$dados[$i]["usunome"]."</td>
								</tr>
																<tr>
									<td class=SubTituloDireita>Projeto/Especificações</td>
									<td>";
										 if($dados[$i]["supprojespecificacoes"]=="t") echo "Sim"; else echo "Não"; 
									echo "</td>
								</tr>
								<tr>
									<td class=SubTituloDireita>Placa da Obra</td>
									<td>";
										 if($dados[$i]["supplacaobra"]=="t") echo "Sim"; else echo "Não"; 
									echo "</td>
								</tr>
								<tr>
									<td class=SubTituloDireita>Diário da Obra Atualizado</td>
									<td>";
										 if($dados[$i]["supdiarioobra"]=="t") echo "Sim"; else echo "Não"; 
									echo "</td>
									
								</tr>
								<tr>
									<td class=SubTituloDireita>Placa Indicativa do Programa/Dados da obra</td>
									<td>";
										 if($dados[$i]["supplacalocalterreno"]=="t") echo "Sim"; else echo "Não"; 
									echo "</td>
																	</tr>
								<tr>
									<td class=SubTituloDireita>Validade do Alvará da Obra</td>
									<td>";
										 if($dados[$i]["supvalidadealvara"]=="t") echo "Sim"; else echo "Não"; 
									echo "</td>
																	</tr>
								<tr>
									<td class=SubTituloDireita>Qualidade de Execução da Obra/Projeto</td>
									<td>";
									if ($qlbid = $dados[$i]["qlbid"]){
									$sql = "SELECT 
												qlbdesc as descricao 
											FROM 
												obras.qualidadeobra
											WHERE qlbid=".$qlbid;  
									echo $db->pegaUm($sql,'descricao');
									}
									echo "</td>
																	</tr>
								<tr>
									<td class=SubTituloDireita>Desempenho da Construtora/Projetista</td>
									<td>";
										if ($dcnid = $dados[$i]["dcnid"]){
										$sql = "SELECT 
													dcnid as codigo, 
													dcndesc as descricao 
												FROM 
													obras.desempenhoconstrutora
												WHERE dcnid=".$dcnid;   
											echo $db->pegaUm($sql,'descricao');
										}
									echo "</td>
								</tr>
								</table>";
								
							echo '
								<table width="80%" cellspacing="1" cellpadding="1" border="0" align="center" class="tabela">
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
									<tbody>	';
									obras_listaitensvistoria($supvid);
								echo '</tbody>
									</table>';
						// FOTOS da vistoria
						if(isset($fotos_vistoria[$supvid]) or isset($selecao_fotos_vistoria_array[$supvid])){
							
							// Selecionado apenas fotos escolhidas
							if (isset($selecao_fotos_vistoria_array[$supvid])){
								$sql = "SELECT fot.*, arq.arqdescricao FROM obras.fotos AS fot
										LEFT JOIN public.arquivo AS arq ON arq.arqid = fot.arqid
										WHERE arq.arqid IN (".$selecao_fotos_vistoria_array[$supvid].") AND obrid =".$obrid." AND supvid=".$supvid." 
										ORDER BY fotordem DESC";
															
							} else {
							// Selecionando as ultimas X fotos
								$sql = "SELECT fot.*, arq.arqdescricao FROM obras.fotos AS fot
										LEFT JOIN public.arquivo AS arq ON arq.arqid = fot.arqid
										WHERE obrid =".$obrid." AND supvid=".$supvid." 
										ORDER BY fotordem DESC LIMIT ".$fotos_vistoria[$supvid];
							}
							
							$fotos = ($db->carregar($sql));
								
							echo '<table width="100%" cellspacing="1" cellpadding="1" border="0" align="center" class="tabela">';
							for($h=0;$h < count($fotos);$h++){
										if ($fotos[$h]["arqid"]){
										echo "<tr>
												<td> 
												<img src='../slideshow/slideshow/verimagem.php?newwidth=225&newheight=225&arqid=".$fotos[$h]["arqid"]."' 
												hspace='3' vspace='3' style='width:225px; height:225px;'\n
												<br>".$fotos[$h]["arqdescricao"]."
												</td>";
										}
										$h=$h+1;
										if ($fotos[$h]["arqid"]){
										echo "
												<td> 
												<img src='../slideshow/slideshow/verimagem.php?newwidth=225&newheight=225&arqid=".$fotos[$h]["arqid"]."' 
												hspace='3' vspace='3' style='width:225px; height:225px;'\n
												<br>".$fotos[$h]["arqdescricao"]."
												</td>";
										}
										$h=$h+1;
										if ($fotos[$h]["arqid"]){
										echo "<td> 
												<td> 
												<img src='../slideshow/slideshow/verimagem.php?newwidth=225&newheight=225&arqid=".$fotos[$h]["arqid"]."' 
												hspace='3' vspace='3' style='width:225px; height:225px;'\n
												<br>".$fotos[$h]["arqdescricao"]."
												</td>";
											
										}
										echo "</tr>";
									}
									echo "</table>";
							} else {
								echo "<div style='position:relative; left:30px'>Não existem fotos cadastradas para esta vistoria <br><br></div>";
							}			
							echo '</td>
							</tr>
							<tr>
								<td> 
									<table width="100%" cellspacing="1" cellpadding="1" border="0" align="center" class="tabela">
										<tr>
											<td class=SubTituloDireita  width="10%">Observações:</td>
											<td> '.nl2br($dados[$i]["supobs"]).' </td>
										</tr>
									</table>
								</td>
							</tr>';
					}
					echo"</table> </div>";
				} else {
					echo "Não existem vistorias cadastradas";
				}			
				?>
	
		<?php
		}
		?>

	<table width="100%">
		<tr>
			<td align="right">
				<div class=notprint	>
					<input type="button" value="Gerar PDF" style="cursor: pointer, info{ display: none; }" onclick="geraPDF();">
					<input type="button" value="Imprimir" style="cursor: pointer, info{ display: none; }" onclick="self.print();">
				</div>
			</td>
		</tr>
	</table>
</form>
</body>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
</html>
