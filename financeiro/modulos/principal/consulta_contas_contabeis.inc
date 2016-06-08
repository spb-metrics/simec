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
				
				
$sql_lista_contas = "SELECT DISTINCT
						icb.icbcod,        		
			        	'<img title=\"Alterar o Cadastro\" align=\"absmiddle\" border=\"0\" src=\"../imagens/alterar.gif\" onclick=\"alteraConta(' || icb.icbcod || ')\">
			        	 <img title=\"Excluir o Cadastro\" align=\"absmiddle\" border=\"0\" src=\"../imagens/excluir.gif\" onclick=\"excluirConta(' || icb.icbcod || ')\">
			        	' AS acao, 		
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
						icb.icbvisualizaconta = 't'	
					ORDER BY
						icb.icbcod
						";

if($_REQUEST["submetido"]) {
	if($_REQUEST["excluir_conta"] != "") {
		$sql_excluir = "DELETE FROM 
							financeiro.informacaoconta
						WHERE 
							icbcod = ".$_REQUEST['excluir_conta'].";
							
						DELETE FROM 
							financeiro.informacaocontabil
						WHERE 
							icbcod = ".$_REQUEST['excluir_conta'].";";
		
		$db2->executar($sql_excluir);
		$db2->commit();
		?>
			<script type="text/javascript">
				alert("Operação realizada com sucesso!");
			</script>
		<?
	}
	else {
		$filtro = array();
		
		if($_REQUEST["desc_resumida"])
			array_push($filtro, " UPPER(icb.icbdscresumida) like UPPER('%" . $_REQUEST['desc_resumida'] . "%') ");
			
		if($_REQUEST["descricao"])
			array_push($filtro, " UPPER(icb.icbdsc) like UPPER('%" . $_REQUEST['descricao'] . "%') ");
			
		// Transforma a data para YYYY-MM-DD
		if($_REQUEST['datainiciovalidade']) {
			$dataini = explode("/", $_REQUEST['datainiciovalidade']);
			$dataini = "{$dataini[2]}-{$dataini[1]}-{$dataini[0]}";
			array_push($filtro, " icb.icbdatainiciovalidade >= '" . $dataini . "' ");
		}
		
		// Transforma a data para YYYY-MM-DD
		if($_REQUEST['datafimvalidade']) {
			$datafim = explode("/", $_REQUEST['datafimvalidade']);
			$datafim = "{$datafim[2]}-{$datafim[1]}-{$datafim[0]}";
			array_push($filtro, " icb.icbdatafimvalidade <= '" . $datafim . "' ");
		}
		
		// Verifica se foi escolhido conta contábil
		if($_REQUEST['contacontabil'][0]) {
			if( is_array($_REQUEST['contacontabil']) ) {
				array_push($filtro, " pc.conconta in ('".implode("','", $_REQUEST['contacontabil'])."') ");
			}
		}
		
		if(!empty($filtro))
			$sql_lista_contas .= " WHERE ".implode(" AND ", $filtro);
	}
}

$dados = $db2->carregar($sql_lista_contas);

if(count($dados) > 1) {
	foreach($dados as $val) {
        if($val['icbcod'] != $icbcodAtual) {
        	$z += isset($z) ? 1 : 0;
        	
			$dados1[$z] = array("acao" 	   => $val['acao'], 
								"resumido" => $val['icbdscresumida'], 
								"inicio"   => formata_data($val['icbdatainiciovalidade']), 
								"fim"      => formata_data($val['icbdatafimvalidade']) , 
								"contas"   => "<div style='color: rgb(0, 102, 204);'>".$val['conconta']."</div>");
				
        	$icbcodAtual = $val['icbcod'];
        } else {
        	$dados1[$z]["contas"] .= "<div style='color: rgb(0, 102, 204);'>".$val['conconta']."</div>";
        }
	}
}
else {
	$dados1 = array();
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Módulo Financeiro', 'Gerenciamento Dinâmico de Contas Contábeis' );

?>

<script type="text/javascript" src="../includes/calendario.js"></script>
<form method="post" action="" name="formulario" id="formulario">
<input type="hidden" name="submetido" id="submetido" value="1" />
<input type="hidden" name="excluir_conta" id="excluir_conta" value="" />
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita" valign="top">Descrição Resumida</td>
		<td>
			<?
				$desc_resumida = $_REQUEST['desc_resumida'];
				echo campo_texto( 'desc_resumida', 'N', 'S', '', 50, 200, '', '' );
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Descrição</td>
		<td>
			<?
				$descricao = $_REQUEST['descricao'];
				echo campo_textarea( 'descricao', 'N', 'S', '', '80', '5', '200', '' , 0, '');
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
				
				if( $_REQUEST['contacontabil'] && $_REQUEST['contacontabil'][0] != '' ) {
					$sql_carregados = "SELECT 
											conconta as codigo,  
											conconta || ' - ' || condsc as descricao
					        		   FROM 
										 	dw.planoconta 
									   WHERE
											contipocontacorrente in ('16','17','31','80','26','45','50','77','00','02','06','12','52','64','37','72','28','18','76') and
											conconta in ('".implode("','", $_REQUEST['contacontabil'])."')
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
					if($_REQUEST['datainiciovalidade']) {
						$arrDataInicio = explode('/', $_REQUEST['datainiciovalidade']);
						$datainiciovalidade = $arrDataInicio[1] . '/' . $arrDataInicio[0] . '/' . $arrDataInicio[2];
					}
					echo campo_data('datainiciovalidade', 'N', 'S', '', 'S' );
				?>					
		</td>	
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Data Fim Validade</td>
		<td>
				<?
					if($_REQUEST['datafimvalidade']) {
						$arrDataFim = explode('/', $_REQUEST['datafimvalidade']);
						$datafimvalidade = $arrDataFim[1] . '/' . $arrDataFim[0] . '/' . $arrDataFim[2];
					}
					echo campo_data('datafimvalidade', 'N', 'S', '', 'S' );
				?>	
		</td>
	</tr>
	<tr>
		<td bgcolor="#CCCCCC"></td>
		<td bgcolor="#CCCCCC">
			<input type="button" name="consultar" id="consultar" value="Consultar" onclick="submete('consultar');" />
			<input type="button" name="incluir" id="incluir" value="Incluir Conta" onclick="submete('incluir');" />
		</td>
	</tr>
</table>
</form>

<?
	$cabecalho = array("Ações", "Descrição Resumida", "Início Validade", "Fim Validade", "Contas Contábeis");
	$db2->monta_lista($dados1, $cabecalho, 10, 20, 'N', '', '');
?>

<script type="text/javascript">

function submete(tipo) {
	if(tipo == 'consultar') {
		selectAllOptions(document.formulario.contacontabil);
		
		document.getElementById('consultar').disabled = true;
		document.getElementById('incluir').disabled = true;
		
		/*if( (document.formulario.desc_resumida.value == "") && (document.formulario.descricao.value == "") &&
			(document.formulario.contacontabil.value == "") && (document.formulario.datainiciovalidade.value == "") && 
			(document.formulario.datafimvalidade.value == "") )
		{
			alert('Você deve informar pelo menos um parâmetro para a consulta.');
			document.getElementById('consultar').disabled = false;
			document.getElementById('incluir').disabled = false;
		}
		else {*/
			document.getElementById('formulario').submit();
		//}
	}
	else {
		location.href = '?modulo=principal/insere_contas_contabeis&acao=A';
	}
}

function alteraConta(cod) {
	location.href = '?modulo=principal/insere_contas_contabeis&acao=A&icbcod='+cod;
}

function excluirConta(cod) {
 	if( confirm('Deseja excluir a conta?') ) {
		document.getElementById('excluir_conta').value = cod;
		document.getElementById('formulario').submit();
	}
}

</script>