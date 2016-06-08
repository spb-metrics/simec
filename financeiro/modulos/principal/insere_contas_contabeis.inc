<?

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");

// Parâmetros para a nova conexão com o banco do SIAFI
$servidor_bd = $servidor_bd_siafi;
$porta_bd    = $porta_bd_siafi;
$nome_bd     = $nome_bd_siafi;
$usuario_db  = $usuario_db_siafi;
$senha_bd    = $senha_bd_siafi;

$db2 = new cls_banco();

// Parâmetros da nova conexão com o banco do SIAFI para o componente 'combo_popup'.
$dados_conexao = array(
					'servidor_bd' => $servidor_bd_siafi,
					'porta_bd' => $porta_bd_siafi,
					'nome_bd' => $nome_bd_siafi,
					'usuario_db' => $usuario_db_siafi,
					'senha_bd' => $senha_bd_siafi
				);
				
				
if($_REQUEST["submetido"]) {
	if($_REQUEST["tipo_operacao"] == 'alterar') {
		// UPDATE na tabela principal 
		$sql = " UPDATE 
					financeiro.informacaocontabil 
				 SET 
					icbdscresumida = '".$_REQUEST['desc_resumida']."',
					icbdsc = '".$_REQUEST['descricao']."', 
					icbdatainiciovalidade = '".$_REQUEST['datainiciovalidade']."',";
		
		if( $_REQUEST['datafimvalidade'] != '' ) {
        	$sql .= "icbdatafimvalidade = '".$_REQUEST['datafimvalidade']."'"; 
		} else {
	    	$sql .= "icbdatafimvalidade = null";
		}

		$sql .= " WHERE
		 			icbcod = ".$_REQUEST['icbcod'];
		$db2->executar($sql);
		
		// Deletar a tabela auxiliar N:M
		$sql = "DELETE FROM
					financeiro.informacaoconta 
				WHERE 
					icbcod = ".$_REQUEST['icbcod'];
		$db2->executar($sql);

		// Após ter sido deletado os registros, onde icbcod = $_request['icbcod'], insere-se os novos registros da tabela auxiliar. 
		for($i=0; $i<count($_REQUEST["contacontabil"]); $i++) {
			$sql = "INSERT INTO financeiro.informacaoconta (
						icbcod,
						conconta
					)VALUES(
						".$_REQUEST['icbcod'].",
					    '".$_REQUEST["contacontabil"][$i]."'
					)";
				
			$db2->executar($sql);
		}
		
		$db2->commit();
		?>
			<script type="text/javascript">
				alert("Operação realizada com sucesso!");
				location.href = '?modulo=principal/consulta_contas_contabeis&acao=A';
			</script>
		<?
	}
	else {
		$sql = " INSERT INTO 
							financeiro.informacaocontabil
							( icbdscresumida, 
							  icbdsc, 
							  icbdatainiciovalidade, 
							  icbdatafimvalidade) 
				 VALUES
							( '".$_REQUEST['desc_resumida']."',
							  '".$_REQUEST['descricao']."',
							  '".$_REQUEST['datainiciovalidade']."', ";
		
		if( $_REQUEST['datafimvalidade'] != '') {
			$sql .= "'".$_REQUEST['datafimvalidade']."' )";
		} else {
			$sql .= "null)";
		} 			 
		
		$sql .= " RETURNING icbcod";
	    
		$icbcod = $db2->pegaUm($sql);
	
		$sql = "INSERT INTO financeiro.informacaoconta ( 
						icbcod, 
					 	conconta
				)VALUES(
					 	".$icbcod.",
		 			 	".implode("),(".$icbcod."," , $_REQUEST['contacontabil'])."
		 		);";		
		
		$db2->executar($sql);
		$db2->commit();
		?>
			<script type="text/javascript">
				alert("Operação realizada com sucesso!");
				location.href = '?modulo=principal/consulta_contas_contabeis&acao=A';
			</script>
		<?
	}
}
else {
	if($_REQUEST["icbcod"]) {
		$icbcod = $_REQUEST["icbcod"];
		
		$sql_dados = "SELECT DISTINCT		
							icb.icbdsc,				        	
							icb.icbdscresumida, 
							icb.icbdatainiciovalidade, 
							icb.icbdatafimvalidade, 
							pc.conconta
						FROM 
							financeiro.informacaocontabil icb
						INNER JOIN 
							financeiro.informacaoconta ic ON ic.icbcod = icb.icbcod 
						INNER JOIN 
							dw.planoconta pc ON pc.conconta = ic.conconta
						WHERE 
							icb.icbcod = ".$icbcod;
		
		$dados = $db2->carregar($sql_dados);
		
		$desc_resumida = $dados[0]['icbdscresumida'];
		$descricao = $dados[0]['icbdsc'];
		$datainiciovalidade = formata_data($dados[0]['icbdatainiciovalidade']);
		$datafimvalidade = formata_data($dados[0]['icbdatafimvalidade']);
		
		$arrContas = array();
		for($i=0; $i<count($dados); $i++)
			$arrContas[$i] = $dados[$i]['conconta'];
	}
	else {
		$icbcod = "";
	}
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Módulo Financeiro', 'Gerenciamento Dinâmico de Contas Contábeis' );

?>

<script type="text/javascript" src="../includes/calendario.js"></script>
<form method="post" action="" name="formulario" id="formulario">
<input type="hidden" name="submetido" id="submetido" value="1" />
<input type="hidden" name="icbcod" id="icbcod" value="<?=$icbcod?>" />
<input type="hidden" name="tipo_operacao" id="tipo_operacao" value="" />
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita" valign="top">Descrição Resumida</td>
		<td>
			<?
				echo campo_texto( 'desc_resumida', 'N', 'S', '', 50, 200, '', '' );
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Descrição</td>
		<td>
			<?
				echo campo_texto( 'descricao', 'N', 'S', '', 50, 200, '', '' );
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Conta Contábil</td>
		<td>
			<?
				$sql_combo = "SELECT 
									conconta as codigo,  
									conconta || ' - ' || condsc as descricao
							  FROM 
									dw.planoconta 
							  WHERE
									contipocontacorrente in ('16','17','31','80','26','45','50','77','00','02','06','12','52','64','37','72','28','18','76')
							  GROUP BY conconta, condsc 
							  ORDER BY 
  									conconta";
				
				if($icbcod) {
					$sql_carregados = "SELECT 
									conconta as codigo,  
									conconta || ' - ' || condsc as descricao
					        		   FROM 
										 	dw.planoconta 
									   WHERE
											contipocontacorrente in ('16','17','31','80','26','45','50','77','00','02','06','12','52','64','37','72','28','18','76') and
											conconta in ('".implode("','", $arrContas)."')
									  GROUP BY conconta, condsc 
  									   ORDER BY 
  											conconta";
					
					$contacontabil = $db2->carregar( $sql_carregados );
				}
									
				combo_popup( 'contacontabil', $sql_combo, 'Selecione a(s) Conta(s) Contábil(eis)', '400x400', 0, array(), '', 'S', true, false, 10, 400, null, null, $dados_conexao ); 
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Data Inicio Validade</td>
		<td>
				<?
					echo campo_data('datainiciovalidade', 'S', 'S', '', 'S' );
				?>					
		</td>	
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Data Fim Validade</td>
		<td>
				<?
					echo campo_data('datafimvalidade', 'N', 'S', '', 'S' );
				?>	
		</td>
	</tr>
	<tr>
		<td bgcolor="#CCCCCC"></td>
		<td bgcolor="#CCCCCC">
			<? if($icbcod) { ?>
			<input type="button" name="alterar" id="alterar" value="Alterar Conta" onclick="submete('alterar');" />
			<? } else { ?>
			<input type="button" name="incluir" id="incluir" value="Incluir Conta" onclick="submete('incluir');" />
			<? } ?>
			<input type="button" name="voltar" id="voltar" value="Voltar" onclick="history.back(-1);" />
		</td>
	</tr>
</table>
</form>
<script type="text/javascript">

function submete(tipo) {
	selectAllOptions(document.formulario.contacontabil);
		
	document.getElementById(tipo).disabled = true;
	document.getElementById('voltar').disabled = true;
	
	document.getElementById('tipo_operacao').value = tipo;
	document.getElementById('formulario').submit();
}

</script>