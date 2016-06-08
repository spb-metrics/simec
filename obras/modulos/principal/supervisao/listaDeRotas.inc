<?php

$supervisao = new supervisao();

switch( $_REQUEST["requisicao"] ){
	
	case "excluir":
		$supervisao->obrExcluiRota( $_REQUEST["rotid"] );	
	break;
	
}

// cabecalho padrão do sistema
include_once APPRAIZ . "includes/cabecalho.inc";

// Monta as abas
print "<br/>";
$db->cria_aba( $abacod_tela, $url, $parametros );
monta_titulo( "Lista de Rotas", "" );

if( $_REQUEST["gpdid"] || $_SESSION["obras"]["gpdid"] ){

	$_SESSION["obras"]["gpdid"] = $_REQUEST["gpdid"] ? $_REQUEST["gpdid"] : $_SESSION["obras"]["gpdid"];
	
	$supervisao->obrMontaCabecalhoGrupo($_SESSION["obras"]["gpdid"]);

	$dadosGrupo = $supervisao->obrBuscaDadosGrupo( $_SESSION["obras"]["gpdid"] );
	extract( $dadosGrupo );
	
	$esdid = obrPegarEstadoAtual( $_SESSION["obras"]["gpdid"] );
	$docid = obrCriarDocumento( $_SESSION["obras"]["gpdid"] );
	
}

?>

<table class="tabela" cellSpacing="1" cellPadding=3 align="center">
	<tr>
		<td width="100%" valign="top" style="background: none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
			<div style="overflow: auto; height: 300px;">
				<?php $supervisao->obrListaRotasGrupo( $_SESSION["obras"]["gpdid"], $esdid ); ?>
			</div>
		</td>
		<td width="100%" valign="top" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
			<?php wf_desenhaBarraNavegacao( $docid , array( 'gpdid' => $_SESSION["obras"]["gpdid"], 'epcid' => $epcid ) ); ?>
		</td>
	</tr>
	<tr bgcolor="#D0D0D0">
		<td colspan="2">
			<?php 
				if( $esdid == OBREMDEFINROTA ){
					print "<input type='button' value='Criar Rota' style='cursor: pointer;' onclick='obrCriarRota({$_SESSION["obras"]["gpdid"]});'>";
				}else{
					print "<input type='button' value='Criar Rota' disabled='disabled'>";
				} 
			?>
			
			<input type="button" value="Voltar" style="cursor: pointer;" onclick="history.back(-1);">
		</td>
	</tr>
</table>
