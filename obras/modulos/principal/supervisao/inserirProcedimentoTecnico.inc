<?php

$supervisao = new supervisao();

if( $supervisao->obrVerficaDadoRequisicao( $_REQUEST["itgid"], "itemgrupo", "itgid", "" ) ){
	$itgid = $_REQUEST["itgid"];
}else{
	$supervisao->obrExibeMsgErro( "A obra informada não existe ou foi excluída!" );
	print "<script>self.close();</script>";
}

if( $_REQUEST["requisicao"] == "salvar" ){
	$supervisao->obrInserirProcedimentoTecnico( $_REQUEST );
}

monta_titulo( "Inserir Procedimento Técnico", "Selecione os procedimentos desejados" );

?>

<html>
	<head>
		<title><?php echo $GLOBALS['parametros_sistema_tela']['nome_e_orgao']; ?></title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<script type="text/javascript" src="../includes/prototype.js"></script>
		<script type="text/javascript" src="../includes/entidades.js"></script>
		<script type="text/javascript" src="/includes/estouvivo.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
		<form id="formProcTecnico" name="formProcTecnico" method="post" action="">
			<input type="hidden" name="requisicao" value="salvar"/>
			<input type="hidden" name="itgid" id="itgid" value="<?php print $itgid; ?>"/>
			<?php 
			
				$sql = "SELECT
							'<center><input type=\"checkbox\" name=\"tppid[]\" value=\"' || tp.tppid || '\" id=\"tppid_' || tp.tppid || '\" ' || CASE WHEN pt.tppid is not null THEN 'checked=\"checked\"' ELSE '' END || '/></center>' as acao,
							tppsigla,
							tppdsc
						FROM
							obras.tipoprocedimento tp
						LEFT JOIN
							obras.procedimentotecnico pt ON tp.tppid = pt.tppid AND itgid = {$itgid}";
			
				$cabecalho = array( "Ação", "Sigla", "Descrição" );
				
				$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '95%');
				
			?>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
				<tr>
					<td>
						&nbsp;
						&nbsp;
						<input type="checkbox" id="selecionaTodos" name="selecionaTodos" onclick="obrSelecionaTodosProcedimentos();"/>
						Selecionar Todos
					</td>
				</tr>
				<tr bgcolor="#D0D0D0">
					<td colspan="2">
						<input type="button" value="Salvar" style="cursor: pointer;" onclick="document.getElementById('formProcTecnico').submit();"/>
						<input type="button" value="Fechar" onclick="self.close();" style="cursor: pointer;"/>
					</td>
				</tr>
			</table>
		</form>
	</body>
</html>
