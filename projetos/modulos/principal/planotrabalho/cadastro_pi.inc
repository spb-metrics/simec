<?php

$visibleButton = usuario_possui_perfil(159) ? 'visible' : 'hidden';
if ($visibleButton == 'hidden') $visibleButton = usuario_possui_perfil(82) ? 'visible' : 'hidden';
if ($visibleButton == 'hidden') $visibleButton = usuario_possui_perfil(85) ? 'visible' : 'hidden';


$atividade = (array) atividade_pegar( $_REQUEST['atiid'] );
if ( !$atividade ) {
	redirecionar( 'principal/atividade_/arvore', 'A' );
}

if( ! isset($_REQUEST['prgano']) || ($_REQUEST['prgano'] == '') ) $prgano = date('Y');
else $prgano = $_REQUEST['prgano'];

$pliorigem = ((integer)$prgano > 2008) ? 'm' : 'f';

//EXCLUI
if($_REQUEST['excluir'] && $_REQUEST['atiidExcl'] && $_REQUEST['pliidExcl']){
	$sql = "DELETE FROM projetos.planointernoatividade WHERE pliid = {$_REQUEST['pliidExcl']} AND atiid = {$_REQUEST['atiidExcl']}";
	$db->executar($sql);
	$db->commit();
	die;
}


// VERIFICA SE PROJETO ESTÁ SELECIONADO
projeto_verifica_selecionado( $atividade['atiid'] );

// CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '&atiid=' . $atividade['atiid'] );
montar_titulo_projeto( $atividade['atidescricao'] );

print '<table border="0" width="95%" align="center">';
print '<tr bgcolor="#f1f1f1"><td>';
print '<br/>';


$sql = "SELECT usucpf FROM projetos.usuarioresponsabilidade WHERE atiid = {$atividade['atiid']}";
$usucpf = $db->pegaUm($sql);
if($usucpf == $_SESSION['usucpf']){
	$visibleButton = 'visible';
} else {
	$sql = "SELECT _atiprofundidade FROM projetos.atividade WHERE atiid = {$atividade['atiid']}";
	$prof = $db->pegaUm($sql);
	$atiid = $atividade['atiid'];
	if($prof){
		for ($i = 1; $i <= $prof; $i++) {
			$sql = "SELECT atiidpai FROM projetos.atividade WHERE atiid = {$atiid}";
			$atiid = $db->pegaUm($sql);
			$sql = "SELECT usucpf FROM projetos.usuarioresponsabilidade WHERE atiid = {$atiid}";
			$usucpf = $db->pegaUm($sql);
			if($usucpf == $_SESSION['usucpf']){
				$visibleButton = 'visible';
			}    
		}
	}
}

//---------------- montando abas ----------------------------

$sql_anos = "SELECT DISTINCT rofano FROM financeiro.execucao WHERE rofano > '2007'";

$anos = $db->carregar($sql_anos);

foreach ($anos as $key => $ano) {	
	$menu[$key] = array("descricao" => "".$ano['rofano']."", "link"=> "/projetos/projetos.php?modulo=principal/planotrabalho/cadastro_pi&acao=A&atiid=".$_REQUEST['atiid']."&prgano=".$ano['rofano']."");
}
echo montarAbasArray($menu, "/projetos/projetos.php?modulo=principal/planotrabalho/cadastro_pi&acao=A&atiid=".$_REQUEST['atiid']."&prgano=".$prgano."");

//---------------- fim abas ----------------------------

extract( $atividade ); # mantém o formulário preenchido
?>
<script type="text/javascript" src="../includes/prototype.js"></script>	
<script type="text/javascript">
function enviar() {
	alert(document.formulario.pliid.innerHTML);
	if ( document.formulario.pliid ) {
		selectAllOptions( document.formulario.pliid );
	}			
	document.formulario.submit();
}


function pesqPlanoInterno () {
	janela = window.open('projetos.php?modulo=principal/planotrabalho/pesqPlanoInterno&acao=A&atiid=<?php echo $_GET[atiid]; ?>&ano=<?=$prgano?>', 'janela1', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width='+(screen.width-120)+',height=680' ); janela.focus();
}

function directAcao (acaid){
	
}

function excluir( pliid, atiid ){
	var exclui = new Ajax.Request(window.location.href, {
        method:     'get',
        parameters: 'excluir=true&atiidExcl='+atiid+'&pliidExcl='+pliid,
        onComplete: function (res){	
        	window.location.reload(); //gato!!
        }
  });
}

function mostra_dados_pi(plicod,prgano) {
	var janela = window.open( '/projetos/projetos.php?modulo=principal/planotrabalho/dados_pi&acao=A&plicod='+plicod+'&ano='+prgano+'&tipoacao=consulta', 'relatorio', 'width=800,height=700,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	janela.focus();
}
</script>

<?php

$sql = "SELECT
		 at.atiid,
		 at._atinumero,
		 at._atinumero ||' - '|| at.atidescricao AS atidesc,
		 pl.plicod as codigopi,
		 pl.plicod || ' - ' || pl.plidsc as plano_interno,
		 a.acaid,
		 a.unicod || ' - ' || u.unidsc as unidsc,
		 a.prgcod || '.' || a.acacod || '.' || a.unicod || '.' || a.loccod || ' - ' || a.acadsc || ' - ' || a.sacdsc as acao, 
		 max(v.rofdatainclusao) as dataatu,
		 --COALESCE( sum(pl.dotacaopi), 0 ) AS rofdotori,
		 pli.plivalor AS rofdotori,
		 COALESCE( sum(v.rofempenhado), 0 ) AS empenhado,
		 COALESCE( sum(v.rofliquidado_favorecido), 0 ) AS rofliquidado_favorecido,
		 COALESCE( sum(v.rofpago), 0 ) as rofpago,
		 CASE WHEN sum( COALESCE( v.rofautorizado, 0 ) ) > 0
		  THEN 
		   TRIM(to_char(( sum( COALESCE( v.rofpago, 0 ) ) * 100 ) / sum( COALESCE( v.rofautorizado, 0 ) ), '999' ) || ' %')
		  ELSE 		
		   '0 %' 
		 END AS autorizado_porcentagem,
		 p.pliid
		FROM
		 projetos.atividade at
		 INNER JOIN projetos.planointernoatividade p ON p.atiid = at.atiid and p.pliorigem = '$pliorigem'
		 inner JOIN projetos.planointerno pl on pl.pliid = p.pliid
		 inner join monitora.planointerno pli on pli.pliid = pl.pliid 
		 inner join monitora.acao a on a.acaid = pl.acaid
		 inner JOIN public.unidade u on u.unicod = a.unicod and u.unitpocod='U'
		 left join financeiro.execucao v on v.plicod=pl.plicod and v.ptres=pl.pliptres and v.rofano = '".$prgano."'
		WHERE 
		( _atinumero = (SELECT _atinumero FROM projetos.atividade WHERE atiid = ".$_GET[atiid]." ) or 
		 _atinumero LIKE (SELECT _atinumero||'.' FROM projetos.atividade WHERE atiid = ".$_GET[atiid]." )||'%' ) AND	
		 at.atistatus = 'A' AND 
		 at._atiprojeto = 3 AND 
		 a.prgano = '".$prgano."' 
		GROUP BY
		 p.pliid,
		 a.acaid, 
		 a.prgcod,
		 a.acacod,
		 a.sacdsc,
		 a.unicod,
		 a.loccod,
		 a.acadsc,
		 pl.plicod,
		 pl.plidsc,
		 u.unidsc,
		 atidesc,
		 at._atinumero,
		 at.atiid,
		 pli.plivalor
		ORDER BY
		 at._atinumero
		 ";

 function agrupar( $lista, $agrupadores ) {
	$existeProximo = count( $agrupadores ) > 0; 
	if ( $existeProximo == false )
	{
		return array();
	}
	$campo = array_shift( $agrupadores );
	
	$novo = array();
	foreach ( $lista as $item ):
		$chave = $item[$campo];
		if ( array_key_exists( $chave, $novo ) == false ){			
	//ver($item['atiid'],d);
			$novo[$chave] = array(
				"atiid"		 => $item['atiid'],
				"acaid"		 => '',				
				"empenhado"  => 0,
				"liquidado"	 => 0,
				"pago"	  	 => 0,
				"pagoAutor"	 => '',
				"dotOrig"	 => 0,
				"sempenhado" => 0,
				"agrupador"  => $campo,
				"sub_itens"  => array(),
				"plicod"	 => '',
				"pliid"	 => $item['pliid'],
				"atiid2"	 => $item['atiid']
			
			);
		}
		if ($campo == 'plano_interno'):		
	
			$novo[$chave][dotOrig]    = $item[rofdotori];			
			$novo[$chave][empenhado]  = $item[empenhado];
			$novo[$chave][liquidado]  = $item[rofliquidado_favorecido];
			$novo[$chave][pago] 	  = $item[rofpago];
			$novo[$chave][pagoAutor]  = $item[autorizado_porcentagem];																	
		endif;
		
		$novo[$chave]['atiid'] = $campo == 'atidesc' ? $item['atiid'] : '';
		$novo[$chave]['acaid'] = $campo == 'acao' 	 ? $item['acaid'] : '';
					
		$novo[$chave][sempenhado]  += $item[empenhado];
		$novo[$chave][sdotOrig]    += $item[rofdotori];		
		$novo[$chave][sliquidado]  += $item[rofliquidado_favorecido];
		$novo[$chave][spago] 	   += $item[rofpago];
		
		$novo[$chave][plicod] 	   = $item[codigopi];
		
		if ( $existeProximo )
			array_push( $novo[$chave]["sub_itens"], $item );

	endforeach;
	if ( $existeProximo ):
		foreach ( $novo as $chave => $dados )
			$novo[$chave]["sub_itens"] = agrupar( $novo[$chave]["sub_itens"], $agrupadores );		
	endif;
	return $novo;
}

function exibir( $lista, $profundidade = 0 ){
	global $agrupadorr,$totEmpenhado,
		   $totLiquidado,$totPago,$totDotacao,
		   $subEmpenhado,$subLiquidado,
		   $subPago,$subDotacao;	
	//static $subAutorizado=0,$subEmpenhado=0,$subLiquidado=0,$subPago=0,$subDotacao=0;	   
	
	if ( count( $lista ) == 0 )
	{		
		return;
	}	
	$contCor = 0;
	
	foreach ( $lista as $chave => $dados ):
		$empenhado  	 = (string) $dados['empenhado'];	
		$liquidado  	 = (string) $dados['liquidado'];	
		$pago 			 = (string) $dados['pago'];	
		$pagoAutor  	 = (string) $dados['pagoAutor'];	
		$agrupador  	 = (string) $dados["agrupador"];
		$dotacaoOriginal = (string) $dados["dotOrig"];
		$atiid2			 = (int)	$dados["atiid2"];
		$acaid			 = (int)	$dados['acaid'];				
		$pliid 			 = (int)    $dados["pliid"];
			
		$sempenhado  	  = (string) $dados['sempenhado'];	
		$sliquidado  	  = (string) $dados['sliquidado'];	
		$spago 			  = (string) $dados['spago'];	
		$spagoAutor  	  = (string) $dados['spagoAutor'];	
		$sdotacaoOriginal = (string) $dados["sdotOrig"];
		
		$plicod = (string) $dados['plicod'];
		
	/*	
		 if (($contCor != 0 && $agrupador == 'atidesc')):

		 	echo '<tr style="background: #DFDFDF;">
		 			<td><b>Totais Atividade:</b></td>
					<td align="right" style="color: rgb(0, 102, 204); backgroud-color:#ccc;">'.number_format($subDotacao,2,',','.').'</td>
					<td align="right" style="color: rgb(0, 102, 204);">'.number_format($subAutorizado,2,',','.').'</td>
					<td align="right" style="color: rgb(0, 102, 204);">'.number_format($subEmpenhado,2,',','.').'</td>	
					<td align="right" style="color: rgb(0, 102, 204);">'.number_format($subLiquidado,2,',','.').'</td>
					<td align="right" style="color: rgb(0, 102, 204);">'.number_format($subPago,2,',','.').'</td>	
					<td align="right">'.number_format(($subPago * 100) / ($subAutorizado ? $subAutorizado : 1),2,'.',',').'%</td>
		 		  </tr>';
		 	
		 	$subAutorizado = 0;
			$subEmpenhado  = 0;
			$subLiquidado  = 0;
			$subPago 	   = 0;
			$subDotacao	   = 0;
	 	 endif;			
	*/	
		if ($profundidade == 0)
			echo '<tr bgcolor="#DDDDDD" onmouseout="this.bgColor=\'#DDDDDD\';" onmouseover="this.bgColor=\'#ffffcc\';">';
		elseif ($profundidade == 1)
			echo '<tr bgcolor="#E9E9E9" onmouseout="this.bgColor=\'#E9E9E9\';" onmouseover="this.bgColor=\'#ffffcc\';">'; 
		elseif ($profundidade == 2)
			echo '<tr bgcolor="#f1f1f1" onmouseout="this.bgColor=\'#f1f1f1\';" onmouseover="this.bgColor=\'#ffffcc\';">'; 		
		else
			echo '<tr bgcolor="#f9f9f9" onmouseout="this.bgColor=\'#f9f9f9\';" onmouseover="this.bgColor=\'#ffffcc\';">'; 		
			
		$colspan = $agrupador != 'plano_interno' ? 'colspan="10"' : '';	
		
		$size  = $agrupador == 'unidsc' || $agrupador == 'atidesc' ? '12' : '10';
		
		if ($profundidade == 0)
			$cl = '#000088';
		elseif ($profundidade == 1) 
			$cl = 'rgb(0, 102, 204)';
		elseif ($profundidade == 2) 
			$cl = 'green'; 		
		else
			$cl = '#666666';		
		
		//$color = $agrupador == 'plano_interno' ? '#666666' : '#333333';
		//$color = $agrupador == 'unidsc' ? '#000000' : $color;
?>
			<td <?//=$colspan ?> style="color:<?=$cl;?>;font-size:<?=$size ?>px; padding-left:<?= $profundidade * 20 ?>px;">
				<?php if ( $profundidade > 0 ): ?><img src="../imagens/seta_filho.gif" align="absmiddle"/><?php else: echo '&nbsp;'; endif; ?>
				<?
					global $prgano;
				
					if ($profundidade == 0){
						echo "<b><a href='?modulo=principal/atividade_/planoInterno&acao=A&atiid={$atiid}' title='Visualizar esta Atividade' style=\"color:{$cl};\">{$chave}</a></b>";
					}elseif ($profundidade == 2){
						echo "<b><a href='/monitora/monitora.php?modulo=principal/acao/monitoraacao&acao=A&acaid={$acaid}' target='new_{$acaid}' title='Visualizar esta Ação' style=\"color:{$cl};\">{$chave}</a></b>";
					}elseif ($profundidade == 1){
						echo "<a href='#' onclick=\"mostra_dados_pi('{$plicod}','{$prgano}');\" style='cursor:pointer;' title='Visualizar este PI'>".$chave."</a>";
					}else{
						echo "<img src=\"/imagens/exclui_p2.gif \" style=\"cursor: pointer\" onclick=\"excluir('{$pliid}','{$atiid2}');\" border=0 alt=\"Excluir\" title=\"Excluir\"><a href='#' onclick=\"mostra_dados_pi('{$plicod}','{$prgano}');\" style='cursor:pointer;' title='Visualizar este PI'>".$chave."</a>";
					}	
				?>	
			</td>
<?php
	$subEmpenhado  += $empenhado;
	$subLiquidado  += $liquidado;
	$subPago 	   += $pago;
	$subDotacao	   += $dotacaoOriginal;

	if ($agrupador == 'plano_interno'):
		$totEmpenhado  += $empenhado;
		$totLiquidado  += $liquidado;
		$totPago 	   += $pago;
		$totDotacao	   += $dotacaoOriginal;	
	endif;
		
?>		
			<td align="right" style="color: <?=$cl?>;"><?=number_format(($agrupador == 'plano_interno' ? $dotacaoOriginal : $sdotacaoOriginal ),2,',','.');?></td>
			<td align="right" style="color: <?=$cl?>;"><?=number_format(($agrupador == 'plano_interno' ? $empenhado  : $sempenhado ),2,',','.'); ?></td>	
			<td align="right" style="color: <?=$cl?>;"><?=number_format(($agrupador == 'plano_interno' ? $liquidado  : $sliquidado ),2,',','.'); ?></td>
			<td align="right" style="color: <?=$cl?>;"><?=number_format(($agrupador == 'plano_interno' ? $pago		 : $spago ),2,',','.')?></td>	
			<td align="right" style="color: <?=$cl?>;">
			<?
				if($agrupador == 'plano_interno') {
					if($dotacaoOriginal == 0)
						$var = 0;
					else
						$var = number_format((($empenhado / $dotacaoOriginal)*100),2,',','.');
				} else {
					if($sdotacaoOriginal == 0)
						$var = 0;
					else
						$var = number_format((($sempenhado / $sdotacaoOriginal)*100),2,',','.');
				}
				
				echo $var;
			?>
			</td>			
		</tr>
<?		
//	endif;
	
			exibir( $dados["sub_itens"], $profundidade + 1 );
		$contCor++;	
	endforeach;
}

$dados = $db->carregar($sql);

echo '<table width="95%" cellspacing="0" cellpadding="0" align="center" class="tabela"><TR style="background:#FFF;"><TD colspan="10">'.montar_resumo_atividade( $atividade ).'</TD></TR>';
if (!$dados){
?>
	<TR style="background:#FFF; color:red;">
		<TD colspan="10" align="center">Não existem PI´s associados a esta atividade
		</TD>
	</TR>
</table>

<?
}else{
	# Monta data da ultima atualização
	if (is_array($dados)) {	
		foreach ($dados as $date){
			if ($date['dataatu']){
				$datArr = explode("-",$dados[0][dataatu]);
				$data   = "{$datArr[2]}-{$datArr[1]}-{$datArr[0]}"; 
				break;
			}	
		}		
		unset($datArr);
	}
	
	$total  = count($dados);
	
	$dados = is_array($dados) ? $dados : array();
	
	$agp = array("atidesc", "unidsc", "acao", "plano_interno");
	$dados = agrupar( $dados, $agp );
//	ver($dados,d);
	
	echo '<table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem">';
	echo '	<TR style="background:#DFDFDF;">';
	echo '		<TD valign="top" style="font-weight:bold;"><div>Atividade</div><div style="padding-left:20px;"><img src="../imagens/seta_filho.gif" align="absmiddle"/> Unidade Orçamentária<div style="padding-left:20px;"><img src="../imagens/seta_filho.gif" align="absmiddle"/> Ação</div><div style="padding-left:40px;"><img src="../imagens/seta_filho.gif" align="absmiddle"/> Plano Interno</div></TD>';
	echo '		<TD valign="top" style="font-weight:bold;">Valor Previsto / PI</TD>';
	echo '		<TD valign="top" style="font-weight:bold;">Empenhado</TD>';
	echo '		<TD valign="top" style="font-weight:bold;">Liquidado</TD>';
	echo '		<TD valign="top" style="font-weight:bold;">Pago</TD>';
	echo '		<TD valign="top" style="font-weight:bold;">% do Empenhado s/<BR>Valor Previsto</TD>';
	echo '	</TR>';
	exibir($dados);
	
	$tot = $totPago > 0 && $totEmpenhado > 0 ? number_format(( ($totPago / $totEmpenhado) * 100),2,',','') : '0';
/*	
	echo '<tr style="background: #DFDFDF;">
	 			<td><b>Totais Atividade:</b></td>
				<td align="right" style="color: rgb(0, 102, 204); backgroud-color:#ccc;">'.number_format($subDotacao,2,',','.').'</td>
				<td align="right" style="color: rgb(0, 102, 204);">'.number_format($subAutorizado,2,',','.').'</td>
				<td align="right" style="color: rgb(0, 102, 204);">'.number_format($subEmpenhado,2,',','.').'</td>	
				<td align="right" style="color: rgb(0, 102, 204);">'.number_format($subLiquidado,2,',','.').'</td>
				<td align="right" style="color: rgb(0, 102, 204);">'.number_format($subPago,2,',','.').'</td>	
				<td align="right">'.number_format(($subPago * 100) / ($subAutorizado ? $subAutorizado : 1),2,'.',',').'%</td>
	 	  </tr>';
*/			 	
	echo '	<TR style="background:#DFDFDF;">
				<TD>&nbsp;&nbsp;&nbsp;&nbsp;<b>Totais:</b></TD>
				<td align="right" style="color: rgb(0, 102, 204);">'.number_format($totDotacao,2,',','.').'</td>
				<td align="right" style="color: rgb(0, 102, 204);">'.number_format($totEmpenhado,2,',','.').'</td>	
				<td align="right" style="color: rgb(0, 102, 204);">'.number_format($totLiquidado,2,',','.').'</td>
				<td align="right" style="color: rgb(0, 102, 204);">'.number_format($totPago,2,',','.').'</td>	
				<td align="right">'.$tot.'%</td>
			</TR>';	
	
	$data = $data ? "* Dados financeiros atualizados até: $data" : "";		
		
	echo '<TR style="background:#FFFFFF;">
			<TD colspan="10" align="right" style="font-weight:bold; font-size:9px; border-top:2px solid black; border-bottom:2px solid black;"><div style="float:left; font-size:11px;">Total de registros: '.$total.'</div>'.$data.'</TD>
		  </tr>';
	echo "</table>";

}

$habilitado = ((integer)$prgano > 2008) ? "" : "disabled=\"disabled\"";
?>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
    <tr style="background-color: #cccccc">	
		<td>
			<input type="button" name="botao" value="Vincular Plano Interno" onclick="javascript:pesqPlanoInterno();" style="visibility: <?php echo $visibleButton; ?>" <?=$habilitado?> />
			<input type="hidden" name="atiid" value="<?php echo $_GET[atiid]; ?>">
		</td>
	</tr>		
</table>

</td></tr>
</table>

