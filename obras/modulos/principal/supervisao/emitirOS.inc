<?php

$obOS = new OSController();
$obOS->ativaDadosOrdemServico( array("orsid", "arqid"), $_GET['orsid'] );
$obOS->ativaDadosArquivo(array("arqextensao", "arqnome"));

$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($obOS->orsid/1000) .'/';
$name    = $obOS->arqnome . '.' . $obOS->arqextensao;

if ( file_exists($caminho . $name) ){
	echo file_get_contents( $caminho . $name );
}else{
	echo "<center><font style='color:red;'>N�o foi encontrada a OS.</font></center>";	
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
			<h4>ORDEM DE SERVI�O</h4>
		</center>
		<table class="tabela" bgcolor="#ffffff" cellSpacing="1" cellPadding=3 align="center">			
			<tr>	
				<td style='border-collapse: collapse; border: 1px solid #ccc;'>
					<b>OS n�: </b><?=$obOS->orsid; ?>
				</td>
				<td style='border-collapse: collapse; border: 1px solid #ccc;'>
					<b>Data de Emiss�o: </b><?=formata_data( $obOS->orsdtemissao ); ?>
				</td>
				<td style='border-collapse: collapse; border: 1px solid #ccc;'>
					<b>Processo de Concess�o n�: </b><?=$obOS->epcnumproceconc; ?> <br/>
					<b>Contrato n�: </b><?=$obOS->epcnumcontrato; ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloCentro" colspan="3">Bloco 1 - ESPECIFICA��O DO SERVI�O</td>
			</tr>
			<tr>
				<td colspan="3" style='border-collapse: collapse; border: 1px solid #ccc;'>
					<b>Empresa:</b> <?=$obOS->empresa; ?> <br/>
					<b>CNPJ:</b> <?=$obOS->cnpj; ?>
				</td>
			</tr>
			<tr>
				<td colspan="2" style='border-collapse: collapse; border: 1px solid #ccc;'>
					<b>1.1 - Per�odo de Execu��o da OS:</b> <?=formata_data($obOS->orsdtinicioexecucao) . ' � ' . formata_data($obOS->orsdtfinalexecucao); ?><br/>
				</td>
				<td colspan="2" style='border-collapse: collapse; border: 1px solid #ccc;'>
					<b>1.2 - Tipo(s) de Deslocamento(s):</b>
					<?=implode(', ', $obOS->buscaTiposDeslocamento()); ?>
				</td>
			</tr>
			<tr>
				<td colspan="4" style='border-collapse: collapse; border: 1px solid #ccc;'>
				<b>1.3 - Local de Execu��o do servi�o:</b><br/>
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
					<b>1.4 - Tabela de Hora T�cnica e Deslocamento:</b> <br/>
					<b>&nbsp;&nbsp;&nbsp;1.4.1 - Total de Hora T�cnica do Servi�o: (R$ <?=number_format($totalProcedimento, 2, ',', '.'); ?>)</b> <br/>
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
										<td><b>Munic�pio:</b><br><?=$obObra->municipio; ?></td>
										<td><b>UF:</b><br><?=$obObra->uf; ?></td>
									</tr>
									<tr bgcolor="#F0F0F0">
										<td><b>Tipo de Ensino:</b><br><?=$obObra->tipoensino; ?></td>
										<td><b>�rea:</b><br><?=$obObra->area; ?></td>
										<td colspan="2"><b>Situa��o:</b><br><?=$obObra->situacao; ?></td>
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
										<b style="margin-left: 15px;">Educa��o Superior</b> - R$ <?=number_format($totalSup, 2, ',', '.'); ?><br>
										<b style="margin-left: 15px;">Educa��o Profissional</b> - R$ <?=number_format($totalPro, 2, ',', '.'); ?><br>
										<b style="margin-left: 15px;">Educa��o B�sica</b> - R$ <?=number_format($totalBas, 2, ',', '.'); ?><br>
										</td>
									</tr>
								</table>
							</td>
						</tr>
					</table>
					<br>
					<b>&nbsp;&nbsp;&nbsp;1.4.2 - Remunera��o do Deslocamento:</b> <br/>
					<table class="tabela" bgcolor="#ffffff" cellSpacing="1" cellPadding="2" align="center">
						<tr>
							<td><?=$obOS->listaCalculoDeslocamento(); ?></td>
						</tr>
					</table>
				</td>
			</tr>
			<tr>
				<td class="SubTituloCentro" colspan="3">Bloco 2 - REMUNERA��O DO SERVI�OS</td>
			</tr>
			<? 
				$totalDeslocamento = $obOS->totalRemuneracaoDeslocamento();
			?>
			<tr>
				<td colspan="3" style='border-collapse: collapse; border: 1px solid #ccc;'>
					<b>1.5 - Tabela de Remunera��o:</b> <br/>
					<b>&nbsp;&nbsp;&nbsp;1.5.1 - Valor da Remunera��o por Hora T�cnica (VRht) - </b>R$ <?=number_format($totalProcedimento, 2, ',', '.'); ?> <br/>
					<b>&nbsp;&nbsp;&nbsp;1.5.2 - Valor da Remunera��o de Deslocamento (VD) - </b>R$ <?=number_format($totalDeslocamento, 2, ',', '.'); ?> <br/>
					<b>&nbsp;&nbsp;&nbsp;1.5.3 - Valor Total Estimado da OS (VRht + VD) - </b>R$ <?=number_format( ($totalProcedimento + $totalDeslocamento), 2, ',', '.'); ?> <br/>
				
				</td>
			</tr>			
			<tr>
				<td class="SubTituloCentro" colspan="3">Bloco 3 - UNIDADE RESPONS�VEL PELA AUTORIZA��O DA OS</td>
			</tr>
			<tr>
				<td colspan="3" style='border-collapse: collapse; border: 1px solid #ccc;'>
				Tendo em vista as informa��es abaixo, solicito autoriza��o final para execu��o da presente <b>OS</b>:<br>
				a)	O planejamento Global do Minist�rio da Educa��o dos Servi�os objeto desta OS, foi analisado e aprovado 
				pelo N�cleo Gestor de Monitoramento e Supervis�o de Obras.<br>
				b)	Os procedimentos t�cnicos a serem realizados foram determinados pela(s) Unidade(s) Demandante(s) e consta
				dos seus registros e arquivos de controle.<br>
				c)	Consta disponibilidade or�ament�ria na unidade gestora da SAA, conforme notas de empenho abaixo:<br>
				<b style="margin-left: 15px;">Educa��o Superior n�: </b><?=$obOS->orsnotaempeds ?><br>
				<b style="margin-left: 15px;">Educa��o Profissional n�: </b><?=$obOS->orsnotaempedp ?><br>
				<b style="margin-left: 15px;">Educa��o B�sica n�:</b> <?=$obOS->orsnotaempedb ?>
				</td>
			</tr>
			<tr>
				<td colspan="3">
				<hr>
				Bras�lia, DF <?=date('d') . " de " . mes_extenso(date('m')) . " de " . date("Y") ?>.<br>
				<br>
				<br>
				<div align="right" style="float:right;">
					<hr style="width: 220px">
					<center>
						<b>Silvio Luis S. da Silva</b><br>
						Coordenador de Documenta��o e <br>
						Gest�o de Processos - Fiscal do contrato.
					</center>
				</div>	
				</td>
			</tr>
			
		</table>
	</body>
</html>-->
