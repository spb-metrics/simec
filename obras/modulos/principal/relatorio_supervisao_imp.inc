<?php

include APPRAIZ . 'includes/Agrupador.php';
include APPRAIZ . 'www/cte/_funcoes.php'; 

$somenteLeitura = "";

if(cte_possuiPerfil(157)) {
	if((!cte_possuiPerfil(155)) && (!cte_possuiPerfil(159)) && (!cte_possuiPerfil(160)) && (!cte_possuiPerfil(156)) && (!cte_possuiPerfil(158))) {
		$somenteLeitura = "N";
	}
}

$obras = new Obras();
$dobras = new DadosObra(null);
$obras->setAcao("R");

if($_SESSION["obra"]["obrid"]) {
	$dados = $obras->Dados($_SESSION["obra"]["obrid"]);
	$dobras = new DadosObra($dados);	
}
?> 
<html>
	<head>
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Expires" content="-1">
		<title>Formulário de Vistoria</title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		 <style>
			 @media print {
			 .notprint { display: none }
			 }
		</style>		
	</head>
	
	<div class="notprint">
	<table width="98%">
		<tr>
			<td align="right">
				<input type="button" value="Imprimir" style="cursor: pointer" onclick="self.print();">
			</td>
		</tr>
	</table>
	</div>

<?

$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo = "Monitoramento de Obras/Infraestrutura";
monta_titulo( 'Formulário de Supervisão', '' );
echo $obras->CabecalhoObras();

?>

	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td width="100%" align="center" colspan="2">
				<label class="TituloTela" style="color:#000000;">Dados da Vistoria</label>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Logradouro:</b></td>
			<td width=80% class=SubTituloDireita style='text-align:left;background:#EEE;' >
				<? $logradouro = $dobras->getEndLog(); ?>
				<?= $logradouro; ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Número:</b></td>
			<td width=80% class=SubTituloDireita style='text-align:left;background:#EEE;' >
				<? $numero = $dobras->getendnum(); ?>
				<?= $numero; ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Complemento:</b></td>
			<td width=80% class=SubTituloDireita style='text-align:left;background:#EEE;' >
				<? $complemento = $dobras->getEndCom(); ?>
				<?= $complemento; ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Bairro:</b></td>
			<td width=80% class=SubTituloDireita style='text-align:left;background:#EEE;' >
				<? $bairro = $dobras->getEndBai(); ?>
				<?= $bairro; ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Município:</b></td>
			<td width=80% class=SubTituloDireita style='text-align:left;background:#EEE;' >
				<? $municipio = $dobras->getMunDescricao(); ?>
				<?= $municipio; ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>UF:</b></td>
			<td width=80% class=SubTituloDireita style='text-align:left;background:#EEE;' >
				<? $uf = $dobras->getEstUf(); ?>
				<?= $uf; ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>CEP:</b></td>
			<td width=80% class=SubTituloDireita style='text-align:left;background:#EEE;' >
				<? $cep = $dobras->getEndCep(); ?>
				<? $cep = formata_cep($cep); ?>
				<?= $cep; ?>
			</td>
		</tr>
	</table>

	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td width="100%" align="center"> 
				<label class="TituloTela" style="color:#000000;">Sobre a Obra</label>
			</td>
		</tr>
		<tr>
			<td width="100%">
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td class="SubTituloDireita"><b>Tipo de Obra:</b></td>
						<td class=SubTituloDireita style='text-align:left;background:#EEE;' >
							<? $tipoobra = $dobras->getTobraId(); ?>
							<?= $tipoobra; ?>
							&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
							&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
							&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
						</td>
						<td class="SubTituloDireita"><b>Início Programado para:</b></td>
						<td class=SubTituloDireita style='text-align:left;background:#EEE;' >
							<? $datainicio = $dobras->getObrDtInicio(); ?>
							<?= formata_data($datainicio); ?>
						</td>
						<td class="SubTituloDireita"><b>Término Programado para:</b></td>
						<td class=SubTituloDireita style='text-align:left;background:#EEE;' >
							<? $datatermino = $dobras->getObrDtTermino(); ?>
							<?=  formata_data($datatermino); ?>
						</td>
					</tr>
				</table>					
			</td>
		</tr>
	</table>

	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td width="100%"  align="center">
				<label class="TituloTela" style="color:#000000;">Observações Sobre a Obra</label>
			</td>
		</tr>
		<tr>
			<td align=center HEIGHT=120>
				&nbsp;
			</td>
		</tr>
	</table>

	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td width="100%" align="center" colspan="2">
				<label class="TituloTela" style="color:#000000;">Dados de Supervisão</label>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%" ><b>Situação da Obra:</b></td>
			<td>
				<?
					$sql = "SELECT	*	FROM obras.situacaoobra	order by stodesc  ";
					$itens = ($db->carregar($sql));
					if($itens){
						foreach ($itens as $i => $linha) {
							echo "<input type='checkbox'>".$linha['stodesc']."&nbsp;&nbsp;&nbsp;";
						}
					}
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Projeto/Especificações:</b></td>
			<td>
				<input type="checkbox">Sim &nbsp;&nbsp;&nbsp;&nbsp;
				<input type="checkbox">Não &nbsp;&nbsp;&nbsp;
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Placa da Obra:</b></td>
			<td>
				<input type="checkbox">Sim &nbsp;&nbsp;&nbsp;&nbsp;
				<input type="checkbox">Não &nbsp;&nbsp;&nbsp;
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Placa Indicativa do<br>Programa/Localização Terreno:</b></td>
			<td>
				<input type="checkbox">Sim &nbsp;&nbsp;&nbsp;&nbsp;
				<input type="checkbox">Não &nbsp;&nbsp;&nbsp;
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Alvarã dentro da Validade:</b></td>
			<td>
				<input type="checkbox">Sim &nbsp;&nbsp;&nbsp;&nbsp;
				<input type="checkbox">Não &nbsp;&nbsp;&nbsp;
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Qualidade de Execução da Obra:</b></td>
			<td>
				<?
					$sql = "SELECT	*	FROM obras.qualidadeobra	order by qlbdesc  ";
					$itens = ($db->carregar($sql));
					if($itens){
						foreach ($itens as $i => $linha) {
							echo "<input type='checkbox'>".$linha['qlbdesc']."&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
						}
					}
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Desempenho da Construtora:</b></td>
			<td>
				<?
					$sql = "SELECT	*	FROM obras.desempenhoconstrutora	order by dcndesc  ";
					$itens = ($db->carregar($sql));
					if($itens){
						foreach ($itens as $i => $linha) {
							echo "<input type='checkbox'>".$linha['dcndesc']."&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
						}
					}
				?>
			</td>
		</tr>
	</table>

	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td>Detalhamento de Supervisão e Acompanhamento</td>
		</tr>
		<tr>
			<td colspan="2">
				<?php
					if($_REQUEST["supvid"]){
						$sql = "
							SELECT
								itco.icoid,
								itc.itcdesc,
								itco.icovlritem,
								itco.icopercsobreobra, 
								itco.icodtinicioitem,
								itco.icodterminoitem,
								itco.icopercprojperiodo,
								itco.icopercexecutado,
								sup.supvlrinfsuperivisor,
								sup.supvlexecutadoanterior,
								sup.supvid
							FROM 
								obras.itenscomposicao itc,
								obras.itenscomposicaoobra itco,
								obras.supervisaoitenscomposicao sup
							WHERE
								sup.icoid = itco.icoid AND
								itc.itcid = itco.itcid AND
								itco.obrid = " . $_SESSION["obra"]["obrid"] . " AND
								sup.supvid = " . $_REQUEST["supvid"] ;	
					}else{				
						$sql = "
							SELECT
								itco.icoid,
								itc.itcdesc,
								itco.icovlritem,
								itco.icopercsobreobra, 
								itco.icodtinicioitem,
								itco.icodterminoitem,
								itco.icopercexecutado
							FROM 
								obras.itenscomposicao itc
							INNER JOIN 
								obras.itenscomposicaoobra itco ON itc.itcid = itco.itcid
							WHERE
								itco.obrid =".$_SESSION["obra"]["obrid"]."";
					}
					
					$itens = ($db->carregar($sql));
									
					$dados = array();
					
					if(is_array($itens)){
						$j = 0;
						foreach ($itens as $i => $linha) {
	
							$valor = "supvlrinfsuperivisor_".$linha['icoid'];	
							if (number_format($linha['supvlrinfsuperivisor'],2,',','.') == "0,00"){	
								$$valor = "";
							}else{
								$$valor = number_format($linha['supvlrinfsuperivisor'],2,',','.');	
							}
							
							$projetado = "icopercsobreobra";	
							if (number_format($linha['icopercsobreobra'],2,',','.') == "0,00"){	
								$$projetado = "";
							}else{
								$$projetado = number_format($linha['icopercsobreobra'],2,',','.');
							}
							
							$executado = "icopercexecutado";	
							if (number_format($linha['icopercexecutado'],2,',','.') == "0,00"){	
								$$executado = "";
							}else{
								$$executado = number_format($linha['icopercexecutado'],2,',','.');	
							}
							
							$total = ($$executado * $$projetado) / 100;
							$total = number_format($total, 2, ',', '.');
							
							$$executado = str_replace(',', '.', $$executado);
							$$valor = str_replace(',', '.', $$valor);
							
							$totalSupervisao = bcadd($$executado, $$valor, 2);
							
							$$executado = str_replace('.', ',', $$executado);
							$$valor = str_replace('.', ',', $$valor);
							
							$projetado = "icopercsobreobra";	
							if (number_format($linha['icopercsobreobra'],2,',','.') == "0,00"){	
								$$projetado = "";
							}else{
								$$projetado = number_format($linha['icopercsobreobra'],2,',','.');	
							}
							
							$executado = "icopercexecutado";	
							if (number_format($linha['icopercexecutado'],2,',','.') == "0,00"){	
								$$executado = "";
							}else{
								$$executado = number_format($linha['icopercexecutado'],2,',','.');	
							}
							
							$total = ($$executado * $$projetado) / 100;
							$total = number_format($total, 2, ',', '.');
							
							$$executado = str_replace(',', '.', $$executado);
							$$valor = str_replace(',', '.', $$valor);
							
							$totalSupervisao = bcadd($$executado, $$valor, 2);
							
							$$executado = str_replace('.', ',', $$executado);
							$$valor = str_replace('.', ',', $$valor);
							
							$valor_antigo = "supvlrinfsuperivisor[]";
							$$valor_antigo = intval($$valor);
							
							$executado =  (isset($linha["icopercexecutado"])) ? $executado = intval($linha["icopercexecutado"]) : $executado = 0;
							$supvlr    =  (isset($linha["supvlrinfsuperivisor"]))? $supvlr  = $linha["supvlrinfsuperivisor"] : $supvlr = 0;
							$exec_anterior = (isset($linha["supvlexecutadoanterior"]))? $exec_anterior  = intval($linha['supvlexecutadoanterior']) : $exec_anterior = 0;
							$exec_sobre_obra = (isset($linha["icopercsobreobra"]))? $exec_sobre_obra  = intval($linha['icopercsobreobra']) : $exec_sobre_obra = 0; 							
							$campoapos = "'percexecapos_".$j."'";
							
							
							$dados[] = array(
											 $linha['itcdesc'] . "<input type='hidden' name='item[]' value='" . $linha['icoid'] . "' />",
											 $linha['icopercsobreobra'] . "<input type='hidden' id='icopercsobreobra[".$linha['icoid']."]' name='icopercsobreobra' value='".$linha['icopercsobreobra']."'/>",
											 $linha['icovlritem'],
											 formata_data($linha['icodtinicioitem']),
											 formata_data($linha['icodterminoitem']),
											 $exec_anterior,
											 $executado,
											 "" //campo_texto('supvlrinfsuperivisor[]', 'N', $somenteLeitura, '', 10, 6, '###', '', 'left', '', 0, 'onBlur="VerificaPercentual('.$executado.','.$supvlr.',this,'.$campoapos.','.$exec_sobre_obra.','."'".$obras->getAcao()."'".');" id="supvlrinfsuperivisor_'.$linha['icoid'].'"')
											 );
						$j++;
						}
	
						$cabecalho = array( "Item da Obra", 
											"(%) Sobre a Obra <br/> A",
											"Valor (R$)",
											"Data de Início ",
											"Data de Término",
											"% Executado Anterior ",
											"% Executado ", 
											"(%) Informado pelo Supervisor"
											);
						$db->monta_lista( $dados, $cabecalho, 50, 10, 'N', 'center', '' );
					}else {
						$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '' );
					}
					
				?>
			</td>
		</tr>
	</table>

	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td width="100%" align="center">
				<label class="TituloTela" style="color:#000000;">Observações da Vistoria</label>
			</td>
		</tr>
		<tr>
			<td align=center HEIGHT=120>
				&nbsp;
			</td>
		</tr>
	</table>

