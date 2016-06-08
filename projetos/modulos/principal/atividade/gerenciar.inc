<?php

include_once dirname( __FILE__ ) . '/_funcoes.inc';

switch( $_REQUEST['acao'] ){

	case 'E':
		$sql = sprintf(
			"select count(*) as total from projetos.atividade where atistatus = 'A' and atiidpai = %d",
			$_REQUEST['atiid']
		);
		if ( $db->pegaUm( $sql ) > 0 ) {
			$db->insucesso( 'A exclusão da atividade foi cancelada.\nSó é possível excluir registros que não possuem dependentes.', array(), $modulo );
		} else  if( !excluir_registro( $_REQUEST['atiid'] ) ) {
			$db->rollback();
			$db->insucesso( 'A exclusão da atividade foi cancelada.\nOcorreu um erro.', array(), $modulo );
		}
		$db->commit();
		unset( $_REQUEST['acao'], $_REQUEST['atiid'] );
		redirecionar( $_REQUEST['modulo'], 'C', $_REQUEST );
		break;

	default:
		break;

}

switch( $_REQUEST['seta'] ){

	case 'cima':
		$sql = sprintf(
			"select a1.* from projetos.atividade a1 where atiid = %d and atistatus = 'A' and a1.atiordem > 1",
			$_REQUEST['atiid']
		);
		$atividade = $db->pegaLinha( $sql );
		if ( !$atividade ) {
			redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'] );
		}
		$sql = sprintf(
			"update projetos.atividade set atiordem = %d where atiordem = %d and atiidpai = %d and atistatus = 'A'",
			$atividade['atiordem'],
			$atividade['atiordem'] - 1,
			$atividade['atiidpai']
		);
		if( !$db->executar( $sql ) ) {
			$db->rollback();
			redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'] );
		}
		$sql = sprintf(
			"update projetos.atividade set atiordem = %d where atiid = %d and atistatus = 'A'",
			$atividade['atiordem'] - 1,
			$atividade['atiid']
		);
		if( !$db->executar( $sql ) ) {
			$db->rollback();
			$db->insucesso( 'Ocorreu um erro, a operação foi cancelada.', array(), $modulo );
		}
		$db->commit();
		unset( $_REQUEST['seta'], $_REQUEST['atiid'] );
		redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $_REQUEST );
		break;

	case 'baixo':
		$sql = sprintf(
			"select a1.* from projetos.atividade a1 where atiid = %d and atistatus = 'A' and a1.atiordem < ( select count(*) from projetos.atividade a2 where a2.atiidpai = a1.atiidpai and atistatus = 'A' )",
			$_REQUEST['atiid']
		);
		$atividade = $db->pegaLinha( $sql );
		if ( !$atividade ) {
			redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'] );
		}
		$sql = sprintf(
			"update projetos.atividade set atiordem = %d where atiordem = %d and atiidpai = %d and atistatus = 'A'",
			$atividade['atiordem'],
			$atividade['atiordem'] + 1,
			$atividade['atiidpai']
		);
		if( !$db->executar( $sql ) ) {
			$db->rollback();
			redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'] );
		}
		$sql = sprintf(
			"update projetos.atividade set atiordem = %d where atiid = %d and atistatus = 'A'",
			$atividade['atiordem'] + 1,
			$atividade['atiid']
		);
		if( !$db->executar( $sql ) ) {
			$db->rollback();
			redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'] );
		}
		$db->commit();
		unset( $_REQUEST['seta'], $_REQUEST['atiid'] );
		redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $_REQUEST );
		break;

	case 'esquerda':
		// carrega os dados da atividade
		$sql = sprintf(
			"select a1.* from projetos.atividade a1 where atiid = %d and atistatus = 'A'",
			$_REQUEST['atiid']
		);
		$atividade = $db->pegaLinha( $sql );
		if ( !$atividade ) {
			redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'] );
		}
		// carrega os dados do antigo pai da atividade
		$sql = sprintf(
			"select a1.* from projetos.atividade a1 where atiid = %d and atistatus = 'A'",
			$atividade['atiidpai']
		);
		$atividade_pai = $db->pegaLinha( $sql );
		if ( !$atividade_pai ) {
			redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'] );
		}
		// desloca os novos irmãos para baixo
		$sql = sprintf(
			"update projetos.atividade set atiordem = atiordem + 1 where atistatus = 'A' and atiidpai = %d and atiordem > %d",
			$atividade_pai['atiidpai'],
			$atividade_pai['atiordem']
		);
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'] );
		}
		// desloca os antigos irmãos para cima
		$sql = sprintf(
			"update projetos.atividade set atiordem = atiordem - 1 where atistatus = 'A' and atiidpai = %d and atiordem > %d",
			$atividade['atiidpai'],
			$atividade['atiordem']
		);
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'] );
		}
		// troca o pai (pelo avô)
		$sql = sprintf(
			"update projetos.atividade set atiidpai = %d, atiordem = %d where atistatus = 'A' and atiid = %d",
			$atividade_pai['atiidpai'],
			$atividade_pai['atiordem'] + 1,
			$atividade['atiid']
		);
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'] );
		}
		$db->commit();
		unset( $_REQUEST['seta'], $_REQUEST['atiid'] );
		redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $_REQUEST );
		break;

	case 'direita':
		// carrega os dados da atividade
		$sql = sprintf(
			"select a1.* from projetos.atividade a1 where atiid = %d and atistatus = 'A'",
			$_REQUEST['atiid']
		);
		$atividade = $db->pegaLinha( $sql );
		if ( !$atividade ) {
			redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'] );
		}
		// carrega o novo pai (irmão que está uma posição acima)
		$sql = sprintf(
			"select a1.* from projetos.atividade a1 where atiidpai = %d and atiordem = %d and atistatus = 'A'",
			$atividade['atiidpai'],
			$atividade['atiordem'] - 1
		);
		$atividade_pai = $db->pegaLinha( $sql );
		if ( !$atividade_pai ) {
			redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'] );
		}
		// desloca os antigos irmãos para cima
		$sql = sprintf(
			"update projetos.atividade set atiordem = atiordem - 1 where atiidpai = %d and atiordem > %d and atistatus = 'A' ",
			$atividade['atiidpai'],
			$atividade['atiordem']
		);
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'] );
		}
		// troca o pai (pelo antigo irmão)
		$sql = sprintf(
			"update projetos.atividade set atiidpai = %d, atiordem = 1 + ( select count(*) from projetos.atividade where atiidpai = %d and atistatus = 'A' ) where atiid = %d and atistatus = 'A'",
			$atividade_pai['atiid'],
			$atividade_pai['atiid'],
			$atividade['atiid']
		);
		if ( !$db->executar( $sql ) ) {
			$db->rollback();
			redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'] );
		}
		$db->commit();
		unset( $_REQUEST['seta'], $_REQUEST['atiid'] );
		redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $_REQUEST );
		break;

}


$exibe_coluna_ins = true;
$exibe_coluna_met = true;
$exibe_coluna_int = true;
$exibe_coluna_orc = true;
$exibe_coluna_con = true;
$exibe_nivel = 2;
$orc_agrupar = true;
$orc_inicio = 2007;
$orc_fim = 2011;
$dados = array();
if ( isset( $_SESSION['atividades_pde'] ) )
{
	$dados = $_SESSION['atividades_pde'];
}
if ( isset( $_REQUEST['filtro'] )  )
{
	$dados = $_REQUEST;
}
if ( count( $dados ) )
{
	$dados['coluna_visivel'] = (array) $dados['coluna_visivel'];
	$exibe_coluna_ins = in_array( 'ins', $dados['coluna_visivel'] );
	$exibe_coluna_met = in_array( 'met', $dados['coluna_visivel'] );
	$exibe_coluna_int = in_array( 'int', $dados['coluna_visivel'] );
	$exibe_coluna_orc = in_array( 'orc', $dados['coluna_visivel'] );
	$exibe_coluna_con = in_array( 'con', $dados['coluna_visivel'] );
	$exibe_nivel = (integer) $dados['nivel_visivel'];
	$orc_agrupar = (boolean) $dados['orc_agrupar'];
	$orc_inicio = (integer) $dados['orc_inicio'];
	$orc_fim = (integer) $dados['orc_fim'];
	if ( $orc_inicio > $orc_fim )
	{
		$orc_inicio = $orc_fim;
	}
	$_SESSION['atividades_pde'] = $dados;
}

?>
	<?php require_once APPRAIZ . "includes/cabecalho.inc"; ?>
	<style>
		.coluna_medida { color:#333333; width:40%; cursor: poiter; }
		.coluna_data { color:#4488CC; }
		.coluna_meta { color:#008000; text-align: center; }
		.coluna_instrumento { color: #2277bb; text-align: left; font-size: 10px; }
		.coluna_orcamento { color:#3366CC; text-align: right; }
		.coluna_controle { text-align: center; color:#CC9933; }	
	</style>
	<script type="text/javascript">
		
		/**
		 * Abre uma janela que permite anexar documentos.
		 */
		function popup_anexo( atividade ){
			window.open(
				"projetos.php?modulo=principal/atividade/alterar&acao=A&atiid=" + atividade + "&visao=anexo",
				"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=500'"
			);
		}
		
		/**
		 * Abre uma janela que permite alterar a atividade.
		 */
		function popup_alterar_atividade( atividade ){
			window.open(
				"projetos.php?modulo=principal/atividade/alterar&acao=A&atiid=" + atividade,
				"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=500'"
			);
		}
		
		/**
		 * Abre uma janela que permite cadastrar novas atividades.
		 */
		function popup_cadastrar_atividade( atividade ){
			window.open(
				"projetos.php?modulo=principal/atividade/cadastrar&acao=C&atiidpai=" + atividade,
				"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=500'"
			);
		}
		
		/**
		 * Abre uma janela que permite controlar a atividade.
		 */
		function popup_controle( atividade ){
			window.open(
				"projetos.php?modulo=principal/atividade/alterar&acao=A&atiid=" + atividade + "&visao=controle",
				"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=500'"
			);
		}
		
		/**
		 * Abre uma janela que permite orçar a atividade.
		 */
		function popup_orcamento( atividade ){
			window.open(
				"projetos.php?modulo=principal/atividade/alterar&acao=A&atiid=" + atividade + "&visao=orcamento",
				"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=500'"
			);
		}
		
		/**
		 * Exclui a atividade indicada.
		 */
		function excluir_atividade( atividade ){
			document.filtro.action = "projetos.php?modulo=<?= $_REQUEST['modulo'] ?>&acao=E&atiid=" + atividade;
			if ( confirm( 'Deseja excluir a atividade?' ) ) {
				filtrarListagem();
//				location.href = "projetos.php?modulo=<?= $_REQUEST['modulo'] ?>&acao=E&atiid=" + atividade
			}
		}
		
		function armazenaNoCookieVisibilidadeAtividade( strIdAtividade , boolStatus )
		{
//			if( !window.finished )
			{
				return;
			}
			
//			alert( 'strIdAtividade: ' + strIdAtividade );
//			alert( 'boolStatus: ' + boolStatus );
			 
			// 1. Carregando o Array do Cookie , se existir //

			var objAtividadesAbertasCookie = new cookieElement( "atividadesAbertas" );
			var objAtividadesFechadasCookie = new cookieElement( "atividadesFechadas" );
			
			if( objAtividadesAbertasCookie.getValue() == undefined )
			{
				var arrAtividades = Array();
			}
			else
			{
				var arrAtividades = explode( ',' , objAtividadesAbertasCookie.getValue() );
			}
			
			if( objAtividadesFechadasCookie.getValue() == undefined )
			{
				var arrAtividadesFechadas = Array();
			}
			else
			{
				var arrAtividadesFechadas = explode( ',' , objAtividadesFechadasCookie.getValue() );
			}
			
			// 2. Alterando o Array em memoria a partir da mudanca da atividade //
		
			// 2.1 Atividades Abertas //
			
			if( array_search( strIdAtividade , arrAtividades ) == -1 )
			{
				if( boolStatus )
				{
					arrAtividades.push( strIdAtividade );
				}
			}
			else
			{
				if( !boolStatus )
				{
					arrAtividades.splice(  arrAtividades.indexOf( strIdAtividade )  , 1 );
				}
			}
			
			// 2.2 Atividades Fechadas //
			
			if( array_search( strIdAtividade , arrAtividadesFechadas ) == -1 )
			{
				if( !boolStatus )
				{
					arrAtividadesFechadas.push( strIdAtividade );
				}
			}
			else
			{
				if( boolStatus )
				{
					arrAtividadesFechadas.splice(  arrAtividadesFechadas.indexOf( strIdAtividade )  , 1 );
				}
			}  	
			  	
			// 3. Salvando o Array alterado no cookie //
			var strAtividadesAbertas	= implode( ',' , arrAtividades );
			var strAtividadesFechadas	= implode( ',' , arrAtividadesFechadas );
//			document.title = strAtividadesAbertas;
			objAtividadesAbertasCookie.setValue( strAtividadesAbertas );
//			objAtividadesFechadasCookie.setValue( strAtividadesFechadas );
		}
			
		/**
		 * Controla a visibilidade dos itens da árvore.
		 */
//		var IE = document.all ? true : false;
		
		function atualizaAtividadesPeloCookie()
		{
			var objAtividadesAbertasCookie	= new cookieElement( "atividadesAbertas" );
			var objAtividadesFechadasCookie	= new cookieElement( "atividadesFechadas" );
//			document.title = objAtividadesAbertasCookie.getValue();
			return;
			if( objAtividadesAbertasCookie.getValue() != null )
			{ 
				var arrAtividadesAbertas	= explode( ',' , trim( objAtividadesAbertasCookie.getValue() ) );
			}
			else
			{
				var arrAtividadesAbertas	= Array();
			}
			
			if( objAtividadesFechadasCookie.getValue() != null )
			{
				var arrAtividadesFechadas	= explode( ',' , trim( objAtividadesFechadasCookie.getValue() ) );
			}
			else
			{
				var arrAtividadesFechadas	= Array();
			}
			
			for( var i = 0 ; i < arrAtividadesAbertas.length ; i++ )
			{
				if ( arrAtividadesAbertas[ i ] != '' )
				{
//					alert( "$" + arrAtividadesAbertas[ i ] + "$" );
					var objAtividade = document.getElementById( 'tr' + arrAtividadesAbertas[ i ] );
					if( objAtividade )
					{
						if( !IE )
						{
							objAtividade.style.display = "table-row";
						} 
						else
						{
							objAtividade.style.display = "block";
						}
					}
				}
			}
			for( var i = 0 ; i < arrAtividadesFechadas.length ; i++ )
			{
				if ( arrAtividadesFechadas[ i ] != '' )
				{
//					alert( "@" + arrAtividadesFechadas[ i ] + "@" );
					var objAtividade = document.getElementById( 'tr' + arrAtividadesFechadas[ i ] );
					if( objAtividade )
					{
						objAtividade.style.display = "none";
					}
				}
			}
		}
		
		function exibirOcultarAtividadesFilhas( atividade, imagem, origem ){
			var atividades = document.getElementById( 'atividades' ).getElementsByTagName( 'tr' );
			for( var i = 0; i < atividades.length ; ++i ) {
				if( atividades[i].getAttribute( 'parent' ) == atividade ) {
					if ( atividades[i].style.display == "none" ) {
						armazenaNoCookieVisibilidadeAtividade( atividades[i].id , true );
						if( !IE ) {
							atividades[i].style.display = "table-row";
						} else {
							atividades[i].style.display = "block";
						}
						if ( origem == true ) {
							imagem.src = imagem.src.replace( 'mais' , 'menos' );
						}
					} else {
						armazenaNoCookieVisibilidadeAtividade( atividades[i].id , false );
						atividades[i].style.display = "none";
						if ( origem == true ) {
							imagem.src = imagem.src.replace( 'menos' , 'mais' );
						}
					}
					var imagens = atividades[i].getElementsByTagName( 'img' );
					for( var j = 0; j < imagens.length ; ++j ) {
						if( imagens[j].getAttribute( 'atividade' ) != null && imagens[j].src.indexOf( 'menos' ) > 0 ) {
							exibirOcultarAtividadesFilhas( imagens[j].getAttribute( 'atividade' ), imagens[j], false );
						}
					}
				}
			}
		}
		
		function filtrarListagem() {
			selectAllOptions( document.getElementById( 'coluna_visivel' ) );
			document.filtro.submit();
		}
		
		function reiniciarFormulario(){
			document.filtro.reset();
		}
		
		
		
		function setaCima( atiid ){
			document.filtro.action = "projetos.php?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&seta=cima&atiid=" + atiid;
			filtrarListagem();
		}
		
		function setaBaixo( atiid ){
			document.filtro.action = "projetos.php?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&seta=baixo&atiid=" + atiid;
			filtrarListagem();
		}
		
		function setaEsquerda( atiid ){
			document.filtro.action = "projetos.php?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&seta=esquerda&atiid=" + atiid;
			filtrarListagem();
		}
		
		function setaDireita( atiid ){
			document.filtro.action = "projetos.php?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&seta=direita&atiid=" + atiid;
			filtrarListagem();
		}
		
	</script>
	<br/>
	<?php if ($_REQUEST['idnivel']) 
					{$idnivel=$_REQUEST['idnivel'];} 
						else 
					{$idnivel=3;} ?>
	<?php $sql="select atidescricao from projetos.atividade where atiid = " . $idnivel; 
	      $titulo = $db->pegaum("select atidescricao from projetos.atividade where atiid=" . $idnivel);?>
	<?php monta_titulo( $titulo, 'Escolha uma ação desejada: <img border="0" align="absmiddle" src="../imagens/gif_inclui.gif"/> Cadastrar <img border="0" align="absmiddle" src="../imagens/alterar.gif"/> Alterar <img border="0" align="absmiddle" src="../imagens/excluir.gif"/> Excluir' ); ?>
	
	<script type="text/javascript" src="../../includes/JsLibrary/_start.js"></script>
	<script type="text/javascript" src="../../includes/JsLibrary/cookies/cookies.js"></script>
	
	<form name="filtro" action="/projetos/projetos.php?modulo=principal/atividade/gerenciar&acao=C" method="post">
		<input type="hidden" name="filtro" value="1"/>
		<table width="95%" cellspacing="0" cellpadding="0" border="0" align="center" class="tabela" style="color: rgb(51, 51, 51); border-botom:none;">
			<tr>
				<td width="1">
					<table cellspacing="0" cellpadding="0" border="0" align="center" style="color: rgb(51, 51, 51);">
						<tr bgcolor="#f0f0f0">
							<td align="center" bgcolor="#dcdcdc" align="center" width="50">
								Mostrar
							</td>
							<td width="10">&nbsp;</td>
							<td align="center" bgcolor="#dcdcdc" width="50">
								Ocultar
							</td>
						</tr>
						<tr>
							<td colspan="3">
								<?php
								
									include APPRAIZ . 'includes/Agrupador.php';
									$invisivel = array();
									$visivel = array();
									
									$coluna_ins = array(
										'codigo'    => 'ins',
										'descricao' => 'Instrumento'
									);
									if ( !$exibe_coluna_ins ) {
										array_push( $invisivel, $coluna_ins );
									} else {
										array_push( $visivel, $coluna_ins );
									}
									
									$coluna_met = array(
										'codigo'    => 'met',
										'descricao' => 'Meta'
									);
									if ( !$exibe_coluna_met ) {
										array_push( $invisivel, $coluna_met );
									} else {
										array_push( $visivel, $coluna_met );
									}
				
									$coluna_int = array(
										'codigo'    => 'int',
										'descricao' => 'Órgãos participantes'
									);
									if ( !$exibe_coluna_int ) {
										array_push( $invisivel, $coluna_int );
									} else {
										array_push( $visivel, $coluna_int );
									}
				
									$coluna_orc = array(
										'codigo'    => 'orc',
										'descricao' => 'Orçamento'
									);
									if ( !$exibe_coluna_orc ) {
										array_push( $invisivel, $coluna_orc );
									} else {
										array_push( $visivel, $coluna_orc );
									}
				
									$coluna_con = array(
										'codigo'    => 'con',
										'descricao' => 'Observação'
									);
									if ( !$exibe_coluna_con ) {
										array_push( $invisivel, $coluna_con );
									} else {
										array_push( $visivel, $coluna_con );
									}
									
									$agrupador = new Agrupador( 'filtro' );
									$agrupador->setDestino( 'coluna_invisivel', null, $invisivel );
									$agrupador->setOrigem( 'coluna_visivel', null, $visivel );
									$agrupador->exibir();
								?>
							</td>
						</tr>
					</table>
				</td>
				<td style="padding: 0 0 0 30px;" bgcolor="#f0f0f0">
					<table align="left" border="0" cellpadding="3" cellspacing="1" height="100%" width="100%">
						<tr>
						<?php
//							$sql = "select atiid as codigo, substr(numero ||' - '||atidescricao, 0, 40)||case when char_length(numero ||' - '||atidescricao)>39 then '...' else '' end as descricao  from(
//									select trim(to_char(a1.atiordem,'99')) as numero, a1.atiid, a1.atidescricao, a1.atiid as atiidpai, a1.atiordem from projetos.atividade a1 where a1.atiidpai=3 and a1.atistatus='A'
//									
//									union all
//									
//									select '&nbsp;&nbsp;&nbsp;'||trim(to_char(a1.atiordem,'99'))||'.'||trim(to_char(a2.atiordem,'99')) as numero, a2.atiid, a2.atidescricao, a2.atiidpai, a2.atiordem from projetos.atividade a1 
//									inner join projetos.atividade a2 on a2.atiidpai=a1.atiid
//									where a1.atiidpai=3 and a1.atistatus='A' and a2.atistatus='A') as foo order by atiidpai, atiid, atiordem";
							$sql = "select atiordempai || atiordem, atiid as codigo, substr(numero ||' - '||atidescricao, 0, 40)||case when char_length(numero ||' - '||atidescricao)>39 then '...' else '' end as descricao
								from(
								select trim(to_char(a1.atiordem,'99')) as numero, a1.atiid, a1.atidescricao, a1.atiid as atiidpai, 0 as atiordem, a1.atiordem as atiordempai from projetos.atividade a1 where a1.atiidpai=3 and a1.atistatus='A'
								union all
								select '&nbsp;&nbsp;&nbsp;'||trim(to_char(a1.atiordem,'99'))||'.'||trim(to_char(a2.atiordem,'99')) as numero, a2.atiid, a2.atidescricao, a2.atiidpai, a2.atiordem, a1.atiordem as atiordempai
								from projetos.atividade a1 
								inner join projetos.atividade a2 on a2.atiidpai=a1.atiid
								where a1.atiidpai=3 and a1.atistatus='A' and a2.atistatus='A'
								) as foo order by atiordempai, atiordem";
						?>
							<td bgcolor="#dcdcdc">
								<label for="orc_agrupar">Nível Inicial</label>
							</td>
							<td style="padding: 0 20px 0 10px;">
								<?php $db->monta_combo( "idnivel", $sql, 'S', '0 - Raiz PDE', '', '' );?>
							</td>
						</tr>
					    <tr>
							<td bgcolor="#dcdcdc" width="150">Níveis Visíveis</td>
							<td style="padding: 0 20px 0 10px;">
								<select name="nivel_visivel">
									<option value="-1" <?php echo $exibe_nivel == 0 ? 'selected="selected"' : ''; ?>>
										&nbsp;&nbsp;todos
									</option>
									<option value="0" <?php echo $exibe_nivel == 0 ? 'selected="selected"' : ''; ?>>
										&nbsp;&nbsp;0&nbsp;&nbsp;
									</option>
									<option value="1" <?php echo $exibe_nivel == 1 ? 'selected="selected"' : ''; ?>>
										&nbsp;&nbsp;1&nbsp;&nbsp;
									</option>
									<option value="2" <?php echo $exibe_nivel == 2 ? 'selected="selected"' : ''; ?>>
										&nbsp;&nbsp;2&nbsp;&nbsp;
									</option>
									<option value="3" <?php echo $exibe_nivel == 3 ? 'selected="selected"' : ''; ?>>
										&nbsp;&nbsp;3&nbsp;&nbsp;
									</option>
								</select>
							</td>
						</tr>
						<tr>
							<td bgcolor="#dcdcdc">Período do Orçamento</td>
							<td style="padding: 0 20px 0 10px;">
								<select name="orc_inicio">
									<?php foreach ( range( 2007, 2011 ) as $ano ) : ?>
										<option value="<?php echo $ano; ?>"
										<?php echo $ano == $orc_inicio ? 'selected="selected"' : '';?>
										>
											<?php echo $ano; ?>
										</option>
									<?php endforeach; ?>
								</select>
								<select name="orc_fim">
									<?php foreach ( range( 2007, 2011 ) as $ano ) : ?>
										<option value="<?php echo $ano; ?>"
										<?php echo $ano == $orc_fim ? 'selected="selected"' : '';?>
										>
											<?php echo $ano; ?>
										</option>
									<?php endforeach; ?>
								</select>
							</td>
						</tr>
						<tr>
							<td bgcolor="#dcdcdc">
								<label for="orc_agrupar">Agrupar Orçamento</label>
							</td>
							<td style="padding: 0 20px 0 10px;">
								<input type="checkbox" name="orc_agrupar" value="1" id="orc_agrupar"
								<?php echo $orc_agrupar ? 'checked="checked"' : '';?>
								/>
							</td>
						</tr>
					</table>
				</td>
				<td bgcolor="#dcdcdc" align="center" width="100">
					<input type="button" name="filtrar" value="Filtrar" onclick="filtrarListagem();"/>
				</td>
			</tr>
		</table>
	</form>
	<table id="atividades" width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem" style="color: rgb(51, 51, 51);">
		<thead>
			<tr style="text-align: center">
				<td valign="top" class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">nº</td>
				<td valign="top" class="title notprint" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Ações</td>
				<td valign="top" class="title notprint" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Recuo</td>
				<td valign="top" class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Medida</td>
				<?php if ( $exibe_coluna_ins ) : ?>
					<td align="top" class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Instrumento</td>
				<?php endif; ?>
				<?php if ( $exibe_coluna_met ) : ?>
					<td valign="top" class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Meta</td>
				<?php endif; ?>
				<?php if ( $exibe_coluna_int ) : ?>
					<td valign="top" class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Órgãos participantes</td>
				<?php endif; ?>
				<?php if ( $exibe_coluna_orc ) : ?>
					<?php if ( !$orc_agrupar ) : ?>
						<?php foreach ( range( $orc_inicio, $orc_fim ) as $ano ) : ?>
							<td valign="top" class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
								<?php echo $ano; ?>
							</td>
						<?php endforeach ; ?>
					<?php else : ?>
						<td valign="top" class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
							<?php echo $orc_inicio; ?>-<?php echo $orc_fim; ?>
						</td>
					<?php endif; ?>
				<?php endif; ?>
				<?php if ( $exibe_coluna_con ) : ?>
					<td valign="top" class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Observação</td>
				<?php endif; ?>
				<td valign="top" class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Início</td>
				<td valign="top" class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Fim</td>
				<td valign="top" class="title notprint" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">Ordem</td>
			</tr>
		</thead>
		<tbody>
			<?php
				$raiz = array(
					'atiid' => $idnivel
				);
				$sql = sprintf( "select * from projetos.atividade where atiid=%d", $idnivel );
				$raiz = $db->carregar( $sql );
				if( $raiz[0]['atiidpai'] ){
					foreach ( $raiz as $registro ) {
						exibir_registro(
							$registro,
							$orc_agrupar,
							$orc_inicio,
							$orc_fim,
							0,
							'',
							count( $filhos ),
							$exibe_nivel,
							$exibe_coluna_ins,
							$exibe_coluna_met,
							$exibe_coluna_int,
							$exibe_coluna_orc,
							$exibe_coluna_con
						);
					}
				} else {
					$filhos = pegar_filhos( $raiz[0] );
					foreach ( $filhos as $registro ) {
						exibir_registro(
							$registro,
							$orc_agrupar,
							$orc_inicio,
							$orc_fim,
							0,
							'',
							count( $filhos ),
							$exibe_nivel,
							$exibe_coluna_ins,
							$exibe_coluna_met,
							$exibe_coluna_int,
							$exibe_coluna_orc,
							$exibe_coluna_con
						);
					}
				}
			?>
		</tbody>
		<tfoot>
			<tr>
				<td colspan="13" style="background-color:#ffffff; text-align:center; padding:15px">
					<div onclick="popup_cadastrar_atividade( <?= $raiz[0]['atiid'] ?> )" title="Cadastrar Atividade" onmouseover="this.style.cursor='pointer'">
						<img border="0" src="../imagens/gif_inclui.gif" style="vertical-align:middle;"/>
						Cadastrar Atividade
					</div>
				</td>
			</tr>
		</tfoot>
	</table>
	<script>
		window.finished = true;
		atualizaAtividadesPeloCookie();
	</script>
