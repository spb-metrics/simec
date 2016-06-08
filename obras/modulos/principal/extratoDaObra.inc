<?php

/**
 * Arquivo que seleciona os dados do Extrato de uma obra (vinculada, aditivada e todo tipo de consulta
 * da obra)
 * 
 * @author Fernando Araújo Bagno da SIlva
 * @since 11/02/2010
 * 
 */

if( !$_SESSION["obra"]['obrid'] ){
      header( "location:obras.php?modulo=inicio&acao=A" );
      exit;
}

function monta_lista_simples_div($sql,$cabecalho="",$perpage,$pages,$soma='N',$largura='95%', $valormonetario='S') {
	global $db;
	if(!(bool)$largura) $largura = '95%';
	// este método monta uma listagem na tela baseado na sql passada
	//Registro Atual (instanciado na chamada)
	if ($_REQUEST['numero']=='') $numero = 1; else $numero = intval($_REQUEST['numero']);

    if (is_array($sql))
        $RS = $sql;
    else
        $RS = $db->carregar($sql);

	$nlinhas = $RS ? count($RS) : 0;
	if (! $RS) $nl = 0; else $nl=$nlinhas;
	if (($numero+$perpage)>$nlinhas) $reg_fim = $nlinhas; else $reg_fim = $numero+$perpage-1;
	print '<table width="'. $largura . '" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">';
	if ($nlinhas>0)
	{
		//Monta Cabeçalho
		if(is_array($cabecalho))
		{
			print '<thead><tr>';
			for ($i=0;$i<count($cabecalho);$i++)
			{
				print '<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">'.$cabecalho[$i].'</label>';
			}
			print '</tr> </thead>';
		}

        echo '<tbody>';

		//Monta Listagem
		$totais = array();
		$tipovl = array();
		$x = 0;
		for ($i=($numero-1);$i<$reg_fim;$i++)
		{
			$c = 0;
			if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
			print '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">';
			foreach($RS[$i] as $k=>$v) {

				if (is_numeric($v))
				{
					//cria o array totalizador
					if (!$totais['0'.$c]) {$coluna = array('0'.$c => $v); $totais = array_merge($totais, $coluna);} else $totais['0'.$c] = $totais['0'.$c] + $v;
					//Mostra o resultado
					$id1 = $v;
					if( $k == 'obrid' ){
						unset($v); 
					}
//					unset($v); 
					if (strpos($v,'.')) {$v = number_format($v, 2, ',', '.'); if (!$tipovl['0'.$c]) {$coluna = array('0'.$c => 'vl'); $tipovl = array_merge($totais, $coluna);} else $tipovl['0'.$c] = 'vl';}
					if ($v<0) print '<td align="right" title="'.$cabecalho[$c].'">('.$v.')'; else print '<td align="right" title="'.$cabecalho[$c].'">'.$v;
					print ('<br>'.$totais[$c]);
				}
				else print '<td title="'.$cabecalho[$c].'">'.$v;
				print '</td>';
				$c = $c + 1;
			}
			
			print '</tr>';
			print '<tr id="tr_'.$id1.'" style="display:none;">';
				print '<td colspan="'.$c.'" >';
				print '<div id="div_'.$id1.'" style="width:100%; height:100px; overflow:auto;"></div>';
				print '</td>';
			print '</tr>';
			$x++;
		}

        print '</tbody>';

		$somarCampos = $soma!='S' && is_array($soma) && (@count($soma)>0);
		if ($soma=='S' || $somarCampos){
			//totaliza (imprime totais dos campos numericos)
			print '<tfoot><tr>';
			for ($i=0;$i<$c;$i++)
			{
				print '<td align="right" title="'.$cabecalho[$i].'">';

				if ($i==0) print 'Totais:   ';
				if(($somarCampos && $soma[$i]) || $soma=='S') {
					if (is_numeric($totais['0'.$i])) 
						if($valormonetario == 'S'){
							print number_format($totais['0'.$i], 2, ',', '.'); 
						}else{
							print $totais['0'.$i]; 
						}
					else 
						print $totais['0'.$i];
				}
				print '</td>';
			}
			print '</tr></tfoot>';
			//fim totais
		}

	}
	else {
		print '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
	}
	print '</table>';
}


// realiza as ações da tela de acordo com o parâmetro passado
switch( $_REQUEST["requisicao"] ){
	case "visualizar": 
		include_once APPRAIZ . "obras/modulos/principal/visualizarExtratoDaObra.inc";
		die;
	break;
}

// cabeçalho padrão do sistema e permissões do módulo
include_once APPRAIZ . "includes/cabecalho.inc";
include_once APPRAIZ . "includes/Agrupador.php";
include_once APPRAIZ . "www/obras/permissoes.php";

// Monta as abas e o título da tela
print "<br/>";
$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( "Extrato da Obra", "Selecione as informações que deseja viasualizar sobra a obra");

?>

<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script type="text/javascript">

	function exibirFotos( campo, supvid ){
		
		var supvidH   = $( '#supvids' ).val();
		
		if (campo.checked == true){
			supvidH = supvidH + "{" + supvid + "}";
			if ( $('#div_' + supvid).html().length < 10 ){
				var url = '?modulo=principal/popup/selecionarFotos&acao=A&supvid=' + supvid;
				$('#div_' + supvid).load(url);
			}
			$('#tr_' + supvid).show();
		} else {
			supvidH = supvidH.replace("{" + supvid + "}", "");
			$('#tr_' + supvid).hide();
		}
		
		$( '#supvids' ).val(supvidH);
		
	}

	function salvaFotosSelecionadas( campo, id ){
		var ids   = $( '#fotoselecionadas' ).val();
    	if ( campo.checked ){
			ids = ids + "{" + id + "}";
    	} else {
    		ids = ids.replace("{" + id + "}", "");
    	}
	    $( '#fotoselecionadas' ).val(ids);
	}

	function verExtrato(){

		var formulario = document.getElementById( "formExtrato" );
		var agrupador  = document.getElementById( "agrupador" );
		
		selectAllOptions( formulario.agrupador );

		if( !agrupador.value ){
			alert("É necessário selecionar pelo menos uma informação sobre a obra!");
			return false;
		}

		formulario.target = 'visualizarExtrato';
		var janela = window.open( '', 'visualizarExtrato', 'width=780, height=465, status=1, menubar=0, toolbar=0, scrollbars=1, resizable=1' );
		janela.focus();
		
		formulario.submit();
			
	}

	function selecionaFotos(){
		window.open( caminho_atual + '?modulo=principal/selecionarFotos&acao=A', 'selecionarFotos', 'width=680, height=465, status=0, menubar=0, toolbar=0, scrollbars=1, resizable=1' );
	}
	
	function abreDados( valor, tipo ){

		switch( tipo ){

			case "coordenadas":

				var tr = document.getElementById( "trMapa" );
				
				if ( valor == 1 ){
					if (document.selection){
						tr.style.display   = 'block';	
					}else{
						tr.style.display   = 'table-row';
					}
				}else{
					tr.style.display   = 'none';
				}
				
			break;

			case "fotos":

				var tr = document.getElementById( "trNumFotos" );
				
				if ( valor == 1 ){
					if (document.selection){
						tr.style.display   = 'block';	
					}else{
						tr.style.display   = 'table-row';
					}
				}else{
					tr.style.display   = 'none';
				}
				
			break;

			case "vistorias":

				var tr = document.getElementById( "trVistoria" );
				
				if ( valor == 1 ){
					if (document.selection){
						tr.style.display   = 'block';	
					}else{
						tr.style.display   = 'table-row';
					}
				}else{
					tr.style.display   = 'none';
				}
				
			break;
			
			
		}
		
	}
	
</script>

<form action="" method="post" id="formExtrato" name="formExtrato">
	<input type="hidden" value="visualizar" name="requisicao" id="requisicao"/>
	<input type="hidden" value="<?php echo $_SESSION["obra"]["obrid"]; ?>" name="obrid" id="obrid"/>
	<input type="hidden" name="fotoselecionadas" id="fotoselecionadas"><!--</td>-->
	<input type="hidden" name="supvids" id="supvids"><!--</td>-->
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="subtitulodireita" width="190px;">Informações da Obra</td>
			<td>
				<?php 
				
					// Início dos agrupadores
						$agrupador = new Agrupador('formExtrato','');
						
						// Dados padrão de destino (nulo)
						$destino = isset( $agrupador2 ) ? $agrupador2 : array();
						
						// Dados padrão de origem
						$origem = array(
							'localobra' => array(
								'codigo'    => 'localobra',
								'descricao' => 'Local da Obra'
							),
							'contatos' => array(
								'codigo'    => 'contatos',

								'descricao' => 'Contatos'
							),
							'contratacao' => array(
								'codigo'    => 'contratacao',
								'descricao' => 'Contratação'
							),
							'licitacao' => array(
								'codigo'    => 'licitacao',
								'descricao' => 'Licitação'
							),
							/*'execucao' => array(
								'codigo'    => 'execucao',
								'descricao' => 'Execução Orçamentária'
							),*/
							'projetos' => array(
								'codigo'    => 'projetos',
								'descricao' => 'Projetos'
							),
							'etapasobra' => array(
								'codigo'    => 'etapasobra',
								'descricao' => 'Cronograma Físico-Financeiro'
							),
													
						);
						
						// exibe agrupador
						$agrupador->setOrigem( 'naoAgrupador', null, $origem );
						$agrupador->setDestino( 'agrupador', null, $destino );
						$agrupador->exibir();
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Coordenadas Geográficas</td>
			<td>
				<input type="radio" name="coordenada" id="coordenadasim" value="1" onclick="abreDados(this.value, 'coordenadas');"> Sim
				<input type="radio" name="coordenada" id="coordenadanao" value="0" onclick="abreDados(this.value, 'coordenadas');" checked="checked"> Não
			</td>
		</tr>
		<tr id="trMapa" style="display: none;">
			<td class="subtitulodireita">Imprimir Mapa</td>
			<td>
				<input type="radio" name="mapa" id="mapasim" value="1"> Sim
				<input type="radio" name="mapa" id="mapanao" value="0" checked="checked"> Não
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Galeria de Fotos</td>
			<td>
				<input type="radio" name="foto" id="fotosim" value="1" onclick="abreDados(this.value, 'fotos');"> Sim
				<input type="radio" name="foto" id="fotonao" value="0" onclick="abreDados(this.value, 'fotos');" checked="checked"> Não
			</td>
		</tr>
		<tr id="trNumFotos" style="display: none;">
			<td class="subtitulodireita">Nº de fotos a ser exibido</td>
			<td>
				<?php echo campo_texto( 'numfotos', 'N', 'N', '', 2, 2, '', '', 'left', '', 0, 'id="numfotos"'); ?>
				<input type="hidden" name="fotoseleciona" id="fotoselecionada" value=""/>
				<img src='../imagens/consultar.gif' align="absmiddle" onclick="selecionaFotos();" style="cursor: pointer;" title="Selecionar Fotos"/>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Vistoria</td>
			<td>
				<input type="radio" name="vistoria" id="vistoriasim" value="1" onclick="abreDados(this.value, 'vistorias');"> Sim
				<input type="radio" name="vistoria" id="vistorianao" value="0" onclick="abreDados(this.value, 'vistorias');" checked="checked"> Não
			</td>
		</tr>
		<tr id="trVistoria" style="display: none;">
			<td colspan="2">
			
				<br/>
			
				<?php 
				
					$sql = "SELECT
								'<div align=\"center\" ><input type=\"checkbox\" name=\"exibe_' || s.supvid || '\" onclick=\"exibirFotos(this, ' || s.supvid || ')\" /></div>' as exibicao,
								s.supvid,
								to_char(s.supvdt,'DD/MM/YYYY') as dtvistoria,
								to_char(s.supdtinclusao,'DD/MM/YYYY') as dtinclusao,						
								u.usunome,
								si.stodesc,
								s.suprealizacao as responsavel
							FROM
								obras.supervisao s
							INNER JOIN 
								obras.situacaoobra si ON si.stoid = s.stoid
							INNER JOIN
								seguranca.usuario u ON u.usucpf = s.usucpf
							WHERE
								s.obrid = {$_SESSION["obra"]["obrid"]} AND
								s.supstatus = 'A'
							ORDER BY 
								s.supdtinclusao ASC";
					
					$dadosVistoria = $db->carregar( $sql );
					
					$cabecalho = array("Selecionar", "Numero","Data da Vistoria","Data da Inclusão","Realizada Por","Situação da Obra","Responsável");
					
					monta_lista_simples_div( $sql, $cabecalho, 50, 10, 'N', '100%','N');
					
				?>
				
				<br/>
				
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td></td>
			<td>
				<input type="button" value="Visualizar" onclick="verExtrato();" style="cursor: pointer;"/>
				<input type="button" value="Voltar" onclick="history.back(-1);" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>
<?php chkSituacaoObra(); ?>
