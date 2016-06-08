<?php
	ini_set( "memory_limit", "300M" );
	// inicializa sistema
	require_once "config.inc";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";
	$db = new cls_banco();

	//controla cache de navega��o
	header( "Connection: Keep-Alive" );
	header("Cache-Control: no-store, no-cache, must-revalidate");
	header("Cache-Control: post-check=0, pre-check=0", false);
	header("Cache-control: private, no-cache");
	header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
	header("Pragma: no-cache");
	//header( "Content-type: text/plain; charset=iso-8859-1" );
	
	// carrega dados referentes ao combo ( array definido na fun��o campo_popup() )
	if ( !isset( $_SESSION['indice_sessao_campo_popup'][$_REQUEST['nome']] ) )
	{
		print '<html><script language="javascript"> alert( "Dados da sess�o perdidos. Possivelmente sua sess�o expirou." ); window.close(); </script></hmtl>';
		exit();
	}
	$dados_combo = $_SESSION['indice_sessao_campo_popup'][$_REQUEST['nome']];

	if($_REQUEST['autocomplete']==1){
		ob_clean();
		$q = strtolower($_GET["q"]);
		if ($q === '') return;
		
//		$dados_combo['sql'] = str_replace("WHERE","WHERE upper(removeacento( ".$dados_combo['whereAuto']." )) like upper(removeacento('%".utf8_decode($q)."%')) AND ", $dados_combo['sql']);
		$sql = "SELECT
					codigo, descricao
				FROM (".$dados_combo['sql'].") foo
				WHERE
					upper(removeacento( descricao )) like upper(removeacento('%".utf8_decode($q)."%'))";
//		$sql = $dados_combo['sql'].' LIMIT 50';

		$arDados = $db->carregar($sql);
		$arDados = ($arDados) ? $arDados : array();
//		ver($sql);
		if(count($arDados)>0){
			foreach ($arDados as $key=>$value){
				if (strpos(strtolower(removeAcentos($value['descricao'])), strtolower(removeAcentos(utf8_decode($q)))) !== false) {
					$codigo = trim($value['codigo']);
					$descricao = trim($value['descricao']);
					echo $descricao."|".$codigo."\n";
				}
			}
		}else{
			echo "<label style=\"color:red\">N�o encontrados registros.</label>|\n";
		}
		
		die;
	
	}
	
	// vari�veis da p�gina
	$sql = $dados_combo['sql'];
	
	$titulo 	   = $dados_combo['titulo'];
	$nome_popup    = $_REQUEST['nome'];
	$whereArr	   = $dados_combo['where'];
	$function	   = $dados_combo['func'];
	$_POST['filtro'] = $_POST['filtro'] ? $_POST['filtro'] : $_SESSION['indice_sessao_campo_popup']['filtrobkp'];
	if ( is_array($_POST['filtro']) ){
		$_SESSION['indice_sessao_campo_popup']['filtrobkp'] = $_POST['filtro'];
		foreach ($_POST['filtro'] as $k => $val):
			${'filtro['.$k.']'} = $val;
			if (trim($val) != ''){
				if($_REQUEST['autocomplete']==1){
					$where[] = $_POST['numeric'][$k] ? "$k = '".intval($val)."'" : "upper(removeacento(" . str_replace("\'", "'", $k) . ")) like upper(removeacento('".utf8_decode($val)."%'))";  
				}else{
					$where[] = $_POST['numeric'][$k] ? "$k = '".intval($val)."'" : "upper(removeacento(" . str_replace("\'", "'", $k) . ")) like upper(removeacento('".$val."%'))";
				}
			}
		endforeach;
		
		if ($where) $sql = str_ireplace("WHERE","WHERE ".implode(' AND ',$where)." AND ", $sql);
	}
?>
<html>
	<head>
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Connection" content="Keep-Alive">
		<meta http-equiv="Expires" content="-1">
		<title><?= $titulo ?><?= $maximo != 0 ? ' - Ecolha no m�ximo ' . $maximo . ' itens' : '' ; ?></title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script type="text/javascript">
			
			<!--
				
				window.focus();
				
				/**
				 * Nome do popup que est� sendo manipulado.
				 * 
				 * @var string
				 */
				var nome_popup = '<?= str_replace( "'", "\\'", $nome_popup ) ?>';
								
				/**
				 * Adiciona ou remove um item do campo select da p�gina origem
				 * 
				 * @param string cod
				 * @param string descricao
				 * @param object checkbox
				 * @return void
				 */
				function campo_popup_altera_item( cod, descricao, checkbox )
				{
					window.opener.campo_popup_adicionar_item( nome_popup, cod, descricao );
					<?=$function ? 'window.opener.' . $function . '( cod );' : '' ?>
					window.close();		
				}
				
		
		</script>
	</head>
	<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0" marginheight="0" bgcolor="#ffffff">
		 <?
		 	echo monta_busta( $whereArr );
		 	
			$registro = $db->carregar( $sql );
			$registro = $registro ? $registro : array();			
			
			$a = 0;
			foreach ($registro as $item){
				$texto = htmlentities( $item['descricao'] ); 
				$dados[$a]['codigo']	= $item['codigo'];//'<a href="javascript:void(0);" id="campo_popup_checkbox_' . $item['codigo'] . '" onclick="' . $onclick . '">' . $item['codigo'] . '</a>';
				$dados[$a]['descricao'] = $texto;//'<a href="javascript:void(0);" id="campo_popup_checkbox_' . $item['codigo'] . '" onclick="' . $onclick . '">' . $item['descricao'] . '</a>';
				
				if ( isset($item['value']) ){
					$dados[$a]['value']	= $item['value'];
					$existValue = 1;
				}
				$a++;
			}
//			dbg($_POST);
			$onclick   = 'javascript:campo_popup_altera_item( \'' . ($existValue ? '{campo[3]}' : '{campo[1]}') . '\', \'{campo[2]}\', this )'; 	

			$html 	   = array('<a href="javascript:void(0);" id="campo_popup_checkbox_{campo[1]}" onclick="' . $onclick . '">{campo[1]}</a>',
		 				  	   '<a href="javascript:void(0);" id="campo_popup_checkbox_{campo[1]}_1" onclick="' . $onclick . '">{campo[2]}</a>');
		 	$cabecalho = array("C�digo" , 
		 					   "Descri��o");
		 	
		 	$db->monta_lista_array( $dados, $cabecalho, 50, 10, 'N', '', $html);
		 ?>
	</body>
</html>
<?php 
function monta_busta( $array = array() ){
	global $titulo;
	
	$form = '';
	if (is_array($array)){
		$form = '<form action="" method="post" name="form_filtro">
				 <table  class="tabela" style="width:100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr bgcolor="#cdcdcd">
						<td align="center" width="100%" colspan="2" height="25" ><label style="color: rgb(0, 0, 0); font-size:12px;" class="TituloTela">'.$titulo.'</label></td>
					</tr>';			
		
		foreach ($array as $k):
				global ${$k['codigo']}; 
				$form .= '<tr>
                            <td class="SubTituloDireita">'.$k['descricao'].'</td>
                            <td>
                            	<input type="hidden" value="" name="filtro[bugfix]">
                                '.campo_texto('filtro['.$k['codigo'].']','','','',30,100, ($k['numeric'] ? '###################################################' : '' ) ,'','','','','','', $_POST['filtro'][str_replace("'", "\'", $k['codigo'])]).'
                                <input type="hidden" name="numeric['.$k['codigo'].']" value="'.$k['numeric'].'">
                            </td>
                          </tr>';
		endforeach;
		
		$form .= '	<tr bgcolor="#cdcdcd">
						<td height="20">&nbsp;</td>
						<td><input type="submit" value="Filtrar"></td>
					  </tr>
				  </table>
				  </form>';
	}
	return $form;
}
?>