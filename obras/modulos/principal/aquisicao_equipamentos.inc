<?php

include APPRAIZ . 'includes/cabecalho.inc';

$obras = new Obras();
$aquisicao = new AquisicaoEquipamentos();
$dobras = new DadosObra(null);

// Pega o caminho atual do usu�rio (em qual m�dulo se encontra)
$caminho_atual   = $_SERVER["REQUEST_URI"];
$posicao_caminho = strpos($caminho_atual, 'acao');
$caminho_atual   = substr($caminho_atual, 0 , $posicao_caminho);

// Executa as fun��es da tela de acordo com suas a��es
if($_REQUEST["requisicao"] == "executar"){
	$obras->CadastrarAquisicao($_REQUEST);
}


if($_SESSION['obra']['obrid']){
		
	$resultado = $aquisicao->busca($_SESSION['obra']['obrid']);	
	$dados = $aquisicao->dados($resultado);
		
}

echo "<br>";

$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo = "Aquisi��o de Equipamentos";
monta_titulo( $titulo_modulo, "<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigat�rio.' /> Indica os campos obrigat�rios" );

echo $obras->CabecalhoObras();

?>
<html>
	<head>
		<title>Anexar Documentos</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<!--<script src="/obras/js/aquisicao_equipamentos.js" ></script>-->
		<script src="../includes/calendario.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script>
			function Validacaoaq(){
		
				if (document.formulario.faeid.value == ""){
					alert("O campo Forma de Aquisi��o � Obrigat�rio");
					document.formulario.faeid.focus();
					return;
				}
				if (document.formulario.aeqdtpubledital.value != ""){
					if (!validaData(document.formulario.aeqdtpubledital)){
						alert("A Data da Publica��o do Edital informada � inv�lida");
						document.formulario.aeqdtpubledital.focus();
						return;
					}
				}
				if (document.formulario.aeqdtpublreslicitacao.value != ""){
					if (!validaData(document.formulario.aeqdtpublreslicitacao)){
						alert("A Data da Publica��o do Resultado da Licita��o informada � inv�lida");
						document.formulario.aeqdtpublreslicitacao.focus();
						return;
					}
				}
				if (document.formulario.aeqdtpubledital.value != "" && document.formulario.aeqdtpublreslicitacao.value != ""){
					if (!validaDataMaior(document.formulario.aeqdtpubledital, document.formulario.aeqdtpublreslicitacao)){
						alert("Data do Resultado menor do que a Data de Publica��o.");
						document.formulario.obrdtinicio.focus();
						return;
					}
				}
				
				document.formulario.submit();
				
			}
	</script>
	</head>
	<body>
		<form method="post" name="formulario" action="<?php echo $caminho_atual; ?>acao=A">
			<input type="hidden" name="obrid" value="<?=$_SESSION['obra']['obrid']?>">
			<input type="hidden" name="requisicao" value="executar">
			<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Forma de Aquisi��o:</td>
					<td><?php
					$faeid = $aquisicao->faeid;
					$sql = "SELECT faeid AS codigo, faedesc AS descricao 
							FROM obras.formaaquisicaoequipamento order by 2";
					
					$db->monta_combo('faeid', $sql, $somenteLeitura, "Selecione...", '', '', '', '130', 'S');
				?></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Data da Publica��o <br>do Edital:</td>
					<td>
						<?$aeqdtpubledital = $aquisicao->aeqdtpubledital;?>
						<?= campo_data( 'aeqdtpubledital', 'N', $somenteLeitura, '', 'S' ); ?>
					</td>
						
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Data da Publica��o <br>do Resultado da Licita��o:</td>
					<td><?$aeqdtpublreslicitacao = $aquisicao->aeqdtpublreslicitacao;?>
						<?= campo_data( 'aeqdtpublreslicitacao', 'N', $somenteLeitura, '', 'S' ); ?>
					</td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Observa��o:</td>
					<td><?$aeqobs = $aquisicao->aeqobs;?>
						<?= campo_textarea( 'aeqobs', 'N', $somenteLeitura, '', 60, 5, 300 ); ?></td>
				</tr>
				<tr style="background-color: #cccccc">
					<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
					<td>
						<input type="button" name="botao" value="Salvar" onclick="Validacaoaq()">
						<input type="button" value="Voltar" style="cursor: pointer" onclick="history.back(-1);">
					</td>
				</tr>
			</table>
		</form>
	</body>
</html>
