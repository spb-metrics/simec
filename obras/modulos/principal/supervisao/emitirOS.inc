<?php

$obOS = new OSController();
$obOS->ativaDadosOrdemServico( array("orsid", "arqid"), $_GET['orsid'] );
$obOS->ativaDadosArquivo(array("arqextensao", "arqnome"));

$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($obOS->orsid/1000) .'/';
$name    = $obOS->arqnome . '.' . $obOS->arqextensao;

if ( file_exists($caminho . $name) ){
	echo file_get_contents( $caminho . $name );
}else{
	echo "<center><font style='color:red;'>Não foi encontrada a OS.</font></center>";	
}
die;
//$obOS = new OSController();
//// ativa no controller o modelo "OrdemServico" e com isso seus atributos
//$obOS->ativaDadosOrdemServico( null, $_GET['orsid'] );
//// ativa no controller o modelo "GrupoDistribuicao" e com isso seus atributos
//$obOS->ativaDadosGrupo();
//// carrega o resultado da pesquisa no controller
//$obOS->carregaDadosEmpresa();
//
?><!--

<html>
	<head>
		<title><?php echo $GLOBALS['parametros_sistema_tela']['nome_e_orgao'];?></title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
		<script src="../includes/calendario.js"></script>
		<script src="../obras/js/obras.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
		<a class="notprint" style="cursor:pointer; float:right; margin-top: 50px; margin-right: 20px;" onclick="window.print();"><img src="../imagens/ico_print.jpg" border="0"></a>
		<center>
			<?php
				$_REQUEST['titulo'] = 'SAA - Subsecretaria de Assuntos Administrativos'; 
				print monta_cabecalho_relatorio( 95 ); 
			?>
			<h4>ORDEM DE SERVIÇO</h4>
		</center>
		<table class="tabela" bgcolor="#ffffff" cellSpacing="1" cellPadding=3 align="center">			
			<tr>	
				<td style='border-collapse: collapse; border: 1px solid #ccc;'>
					<b>OS nº: </b><?=$obOS->orsid; ?>
				</td>
				<td style='border-collapse: collapse; border: 1px solid #ccc;'>
					<b>Data de Emissão: </b><?=formata_data( $obOS->orsdtemissao ); ?>
				</td>
				<td style='border-collapse: collapse; border: 1px solid #ccc;'>
					<b>Processo de Concessão nº: </b><?=$obOS->epcnumproceconc; ?> <br/>
					<b>Contrato nº: </b><?=$obOS->epcnumcontrato; ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloCentro" colspan="3">Bloco 1 - ESPECIFICAÇÃO DO SERVIÇO</td>
			</tr>
			<tr>
				<td colspan="3" style='border-collapse: collapse; border: 1px solid #ccc;'>
					<b>Empresa:</b> <?=$obOS->empresa; ?> <br/>
					<b>CNPJ:</b> <?=$obOS->cnpj; ?>
				</td>
			</tr>
			<tr>
				<td colspan="2" style='border-collapse: collapse; border: 1px solid #ccc;'>
					<b>1.1 - Período de Execução da OS:</b> <?=formata_data($obOS->orsdtinicioexecucao) . ' à ' . formata_data($obOS->orsdtfinalexecucao); ?><br/>
				</td>
				<td colspan="2" style='border-collapse: collapse; border: 1px solid #ccc;'>
					<b>1.2 - Tipo(s) de Deslocamento(s):</b>
					<?=implode(', ', $obOS->buscaTiposDeslocamento()); ?>
				</td>
			</tr>
			<tr>
				<td colspan="4" style='border-collapse: collapse; border: 1px solid #ccc;'>
				<b>1.3 - Local de Execução do serviço:</b><br/>
				<?
				$obOS->listaRotasGrupo();
				?>
				</td>
			</tr>
			<tr>
				<td colspan="3" style='border-collapse: collapse; border: 1px solid #ccc;'>
					<? 
					$arObrid = $obOS->buscaIdObrasOS();
					$totalProcedimento = $obOS->totalValorProcedimentoObra( $arObrid );					
					?>
					<b>1.4 - Tabela de Hora Técnica e Deslocamento:</b> <br/>
					<b>&nbsp;&nbsp;&nbsp;1.4.1 - Total de Hora Técnica do Serviço: (R$ <?=number_format($totalProcedimento, 2, ',', '.'); ?>)</b> <br/>
					<table class="tabela" bgcolor="#ffffff" cellSpacing="1" cellPadding="2" align="center">
						<tr>
							<td>
								<? 
								if ( is_array($arObrid) && $arObrid[0] ):
									foreach( $arObrid as $obrid ):
										$obObra = $obOS->buscaObraPorID( $obrid );
								?>
								<table class="tabela" style="width: 100%;" bgcolor="#ffffff" cellSpacing="1" cellPadding="2" align="center">
									<tr bgcolor="#F0F0F0">
										<td colspan="4"><b>Nome da Obra:</b><br><?=$obObra->obrdesc; ?></td>
									</tr>
									<tr bgcolor="#F0F0F0">
										<td><b>Unidade:</b><br><?=$obObra->unidade; ?></td>
										<td><b>Campus:</b><br><?=$obObra->campus; ?></td>
										<td><b>Município:</b><br><?=$obObra->municipio; ?></td>
										<td><b>UF:</b><br><?=$obObra->uf; ?></td>
									</tr>
									<tr bgcolor="#F0F0F0">
										<td><b>Tipo de Ensino:</b><br><?=$obObra->tipoensino; ?></td>
										<td><b>Área:</b><br><?=$obObra->area; ?></td>
										<td colspan="2"><b>Situação:</b><br><?=$obObra->situacao; ?></td>
									</tr>
									<tr>
										<td colspan="4">&nbsp;&nbsp;&nbsp;Procedimento/Valor:</td>
									</tr>
									<tr>
										<td colspan="4"><?=$obOS->listaProcedimentoValor( $obrid ); ?></td>
									</tr>
								</table>
								<br>
								<? 
									endforeach;
								endif;
								$totalSup = $obOS->totalValorProcedimentoObra( $arObrid, array("orgid" => 1) );
								$totalPro = $obOS->totalValorProcedimentoObra( $arObrid, array("orgid" => 2) );
								$totalBas = $obOS->totalValorProcedimentoObra( $arObrid, array("orgid" => 3) );
								?>
								<table class="tabela" style="width: 100%;" bgcolor="#ffffff" cellSpacing="1" cellPadding="2" align="center">
									<tr bgcolor="#F0F0F0">
										<td>
										<b>Totais:</b><br>
										<b style="margin-left: 15px;">Educação Superior</b> - R$ <?=number_format($totalSup, 2, ',', '.'); ?><br>
										<b style="margin-left: 15px;">Educação Profissional</b> - R$ <?=number_format($totalPro, 2, ',', '.'); ?><br>
										<b style="margin-left: 15px;">Educação Básica</b> - R$ <?=number_format($totalBas, 2, ',', '.'); ?><br>
										</td>
									</tr>
								</table>
							</td>
						</tr>
					</table>
					<br>
					<b>&nbsp;&nbsp;&nbsp;1.4.2 - Remuneração do Deslocamento:</b> <br/>
					<table class="tabela" bgcolor="#ffffff" cellSpacing="1" cellPadding="2" align="center">
						<tr>
							<td><?=$obOS->listaCalculoDeslocamento(); ?></td>
						</tr>
					</table>
				</td>
			</tr>
			<tr>
				<td class="SubTituloCentro" colspan="3">Bloco 2 - REMUNERAÇÃO DO SERVIÇOS</td>
			</tr>
			<? 
				$totalDeslocamento = $obOS->totalRemuneracaoDeslocamento();
			?>
			<tr>
				<td colspan="3" style='border-collapse: collapse; border: 1px solid #ccc;'>
					<b>1.5 - Tabela de Remuneração:</b> <br/>
					<b>&nbsp;&nbsp;&nbsp;1.5.1 - Valor da Remuneração por Hora Técnica (VRht) - </b>R$ <?=number_format($totalProcedimento, 2, ',', '.'); ?> <br/>
					<b>&nbsp;&nbsp;&nbsp;1.5.2 - Valor da Remuneração de Deslocamento (VD) - </b>R$ <?=number_format($totalDeslocamento, 2, ',', '.'); ?> <br/>
					<b>&nbsp;&nbsp;&nbsp;1.5.3 - Valor Total Estimado da OS (VRht + VD) - </b>R$ <?=number_format( ($totalProcedimento + $totalDeslocamento), 2, ',', '.'); ?> <br/>
				
				</td>
			</tr>			
			<tr>
				<td class="SubTituloCentro" colspan="3">Bloco 3 - UNIDADE RESPONSÁVEL PELA AUTORIZAÇÃO DA OS</td>
			</tr>
			<tr>
				<td colspan="3" style='border-collapse: collapse; border: 1px solid #ccc;'>
				Tendo em vista as informações abaixo, solicito autorização final para execução da presente <b>OS</b>:<br>
				a)	O planejamento Global do Ministério da Educação dos Serviços objeto desta OS, foi analisado e aprovado 
				pelo Núcleo Gestor de Monitoramento e Supervisão de Obras.<br>
				b)	Os procedimentos técnicos a serem realizados foram determinados pela(s) Unidade(s) Demandante(s) e consta
				dos seus registros e arquivos de controle.<br>
				c)	Consta disponibilidade orçamentária na unidade gestora da SAA, conforme notas de empenho abaixo:<br>
				<b style="margin-left: 15px;">Educação Superior nº: </b><?=$obOS->orsnotaempeds ?><br>
				<b style="margin-left: 15px;">Educação Profissional nº: </b><?=$obOS->orsnotaempedp ?><br>
				<b style="margin-left: 15px;">Educação Básica nº:</b> <?=$obOS->orsnotaempedb ?>
				</td>
			</tr>
			<tr>
				<td colspan="3">
				<hr>
				Brasília, DF <?=date('d') . " de " . mes_extenso(date('m')) . " de " . date("Y") ?>.<br>
				<br>
				<br>
				<div align="right" style="float:right;">
					<hr style="width: 220px">
					<center>
						<b>Silvio Luis S. da Silva</b><br>
						Coordenador de Documentação e <br>
						Gestão de Processos - Fiscal do contrato.
					</center>
				</div>	
				</td>
			</tr>
			
		</table>
	</body>
</html>-->
