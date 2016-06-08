<?php
ini_set("memory_limit","1000M");

if ( (integer)$_REQUEST['ajax'] == 4 ){
	echo obras_busca_campus( $_REQUEST['entid'], $_REQUEST['orgid'] );
	die;
}

if ( $_GET['AJAX'] ) {
	switch ( $_GET['subAcao'] ){
		case 'ajaxCarregaTipologia' : carregaTipologia( $_GET['cloid'], $_GET['prfid'] ); break;
		case 'ajaxCarregaTipologia_Prog' : carregaTipologiaProg( $_GET['prfid'] ); break;
		case 'ajaxCarregaTipologia_Class' : carregaTipologiaClass( $_GET['cloid'] ); break;
	}
	die;
}
//// Inclusão do arquivo de permissões (somente no módulo de obras)
//if ($_SESSION["sisid"] == ID_OBRAS){
//}
	require_once APPRAIZ . 'includes/cabecalho.inc';
	require_once APPRAIZ . "www/obras/permissoes.php";

// Inclusão de arquivos padrão do sistema
require_once APPRAIZ . 'includes/Agrupador.php';

// Inclusão de arquivos do componente de Entidade
require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

// Pega o caminho atual do usuário (em qual módulo se encontra)
$caminho_atual = $_SERVER["REQUEST_URI"];
$posicao_caminho = strpos($caminho_atual, 'acao');
$caminho_atual = substr($caminho_atual, 0 , $posicao_caminho);

// Pega url para os js
$posicao_caminho_js = strpos($caminho_atual, '?');
$caminho_atual_js   = substr($caminho_atual, 0 , $posicao_caminho_js);

$obras = new Obras();
$infraestrutura = new DadosInfraEstrutura();

switch ($_REQUEST["requisicao"]){
	case "cadastro": $obras->CadastrarObras( $_REQUEST ); break;
	case "atualiza": $obras->AtualizarObras( $_REQUEST ); break;
}

$dobras = new DadosObra(null);
$requisicao = "cadastro";

echo "<br/>";

if ( $_REQUEST['subAcao'] == 'novaObra' ) {
	unset( $_SESSION['obra'] );
	unset( $_REQUEST['obrid'] );
}else{
	if( $_REQUEST["obrid"] ){
	
		// Verifica se existe a obra e se o usuário possui permissão
		include_once APPRAIZ . "www/obras/_permissoes_obras.php";
	
		
		$_SESSION["obrasbkp"]["filtrosbkp"] = $_SESSION["obras"]["filtros"];
		$_SESSION["obrasbkp"]["orgid"]      = $_SESSION["obras"]["orgid"];
		$_SESSION["obrasbkp"]["ordem"]      = $_SESSION["obras"]["ordem"];
		$_SESSION["obrasbkp"]["lista"]      = $_SESSION["obras"]["lista"];
	
		// Cria a sessão com a nova obra
		session_unregister("obra");
		session_unregister("obras");
		$_SESSION["obra"]["obrid"]  = $_REQUEST["obrid"];
		$_SESSION["obras"]["obrid"] = $_REQUEST["obrid"];
	
	}elseif(!$_SESSION["obra"]["obrid"]) {
		$tr_campus = 'display:none;';
		// se não existir obrid então envie o usuário para a tela de seleção de obras
		header( "location:obras.php?modulo=inicio&acao=A" );
	    exit;
	}
}


if ($_SESSION["obra"]["obrid"]){

	$tr_campus = '';
	$requisicao = "atualiza";

	// Carrega os dados da obra
	$obrid = $_SESSION["obra"]["obrid"];
	$dados = $obras->Dados($obrid);
	$dobras = new DadosObra($dados);

	$resultado = $infraestrutura->busca($_SESSION['obra']['obrid']);
	$dados = $infraestrutura->dados($resultado);


	// For uma obra do FNDE não possui campus
	if ($dobras->orgid == ORGAO_FNDE || $dobras->orgid == ORGAO_ADM || $dobras->orgid == ORGAO_REHUF) {
		$tr_campus = 'display:none;';
	}

	// Monta as abas
	$db->cria_aba($abacod_tela,$url,$parametros);

}


// Cria o título da tela
$titulo_modulo = "Dados da Obra";
monta_titulo( $titulo_modulo, "<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /> Indica os campos obrigatórios");


?>

<script src="/includes/prototype.js"></script>
<script src="/includes/entidades.js"></script>
<script src="/includes/calendario.js"></script>
<script>
function abreDadosIndigenas( cloid ){

	var titulo     = document.getElementById('tr_tituloind');
	var territorio = document.getElementById('tr_territorio');
	var povos	   = document.getElementById('tr_povos');

	if ( cloid == 4 ){
		
		if (document.selection){
			titulo.style.display 	 = 'block';
			territorio.style.display = 'block';
			povos.style.display 	 = 'block';
		}else{
			titulo.style.display 	 = 'table-row';
			territorio.style.display = 'table-row';
			povos.style.display 	 = 'table-row';
		}
		
	}else{
		titulo.style.display 	 = 'none';
		territorio.style.display = 'none';
		povos.style.display 	 = 'none';
		
	}

}

function abreInauguracao(valor){

	var inaugurada = document.getElementById('inaugurada');
	var dataprev   = document.getElementById("dtprevisao");
	var datain 	   = document.getElementById("dtinauguracao");
	
	if(valor == 1 || valor == 3){
		if (document.selection){
			inaugurada.style.display = 'block';
		}else{
			inaugurada.style.display = 'table-row';
		}
	}else{
		inaugurada.style.display = 'none';
		dataprev.style.display = 'none';
		datain.style.display   = 'none';
	}

}


function abreData(valor){
	
	var dataprev 			 = document.getElementById("dtprevisao");
	var datain 				 = document.getElementById("dtinauguracao");
	var obrdtprevinauguracao = document.getElementById("obrdtprevinauguracao");
	var obrdtinauguracao 	 = document.getElementById("obrdtinauguracao");
	
	if(valor == '' || valor == 'S'){
		
		obrdtinauguracao.value     = '';
		obrdtprevinauguracao.value = '';
		
		if (document.selection){
			dataprev.style.display = 'none';
			datain.style.display   = 'none';	
		}else{
			dataprev.style.display = 'none';
			datain.style.display   = 'none';
		}
	}
	
	if(valor == 'N'){
		
		obrdtinauguracao.value     = '';
	
		if (document.selection){
			dataprev.style.display = 'block';
			datain.style.display   = 'none';	
		}else{
			dataprev.style.display = 'table-row';
			datain.style.display   = 'none';
		}
	}
	
	if(valor == 'I'){
	
		obrdtprevinauguracao.value = '';
	
		if (document.selection){
			dataprev.style.display = 'none';
			datain.style.display   = 'block';	
		}else{
			dataprev.style.display = 'none';
			datain.style.display   = 'table-row';
		}
	}
}

</script>
<form id="formulario" name="formulario" method="post"
	onSubmit="return Validacao( <? echo ((possuiPerfil( array(PERFIL_ADMINISTRADOR,PERFIL_SUPERVISORMEC) ))?"true":"false"); ?> );"
	action="<?php echo $caminho_atual; ?>acao=A">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
	<tr>
		<td class="SubTituloDireita" width="190px">Tipo de estabelecimento:</td>
		<td><?php
			
		// Monta o combo de orgaos de acordo com a permissão do usuário
		$res = obras_pegarOrgaoPermitido();
		$arOrgaos = array();
		foreach( $res as $arOrgao ){
			$arOrgaos[] = array( 'codigo' => $arOrgao['id'], 'descricao' => $arOrgao['descricao'] );
		}
		if($_REQUEST["org"]) {
			$orgid = $_REQUEST["org"];
		} elseif($dobras->orgid) {
			$orgid = $dobras->orgid;
		} else {
			$orgid = $res[0]['id'];
		}
			
		$_SESSION["obra"]["orgid"]  = $orgid;
		$_SESSION["obras"]["orgid"] = $orgid;
			
		$db->monta_combo("orgid", $arOrgaos, 'N', "Selecione...", '', '', '', '', 'S', 'orgid');
			
		?> <input type="hidden" name="obrid" id="obrid"
			value="<?if($_SESSION["obra"]["obrid"]) echo $_SESSION["obra"]["obrid"];?>" />
		</td>
		<td rowspan="24" width="100" valign="top" align="center">
		<? 
		if ( $obrid ){
			
			$sql = "SELECT
						docid
					FROM
						obras.obrainfraestrutura
					WHERE
						obrid = ".$obrid;
			$docid = $db->pegaUm($sql);
			
			if( $docid ){
				
				$sql = "SELECT
							ed.esdid
						FROM 
							workflow.documento d
						INNER JOIN 
							workflow.estadodocumento ed ON ed.esdid = d.esdid
						WHERE
							d.docid = " . $docid;
				
				$esdid = (integer) $db->pegaUm( $sql );		
				$gpdid = buscaGrupoPelaObra( $obrid );
				if ( $esdid ){
	//			if ( $esdid == OBREMAPROVAMEC ){
	//				$docid = obrPegaDocidObra( $obrid );
	//				if ( $docid ){
						wf_desenhaBarraNavegacao( $docid , array( 'obrid' => $obrid, 'gpdid' => $gpdid) );
	//				}
				}
			}
		}
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Unidade Responsável pela Obra:</td>
		<td><?php 
			
		$entid_permitidas = obras_pegarUnidadesPermitidas();
			
		if ( !$_SESSION["obra"]["obrid"] ){

			if ( $entid_permitidas ){
					
				$sql = "SELECT
							e.entid as codigo,
							e.entnome as descricao
						FROM
							entidade.entidade e
						WHERE
							entid in ( ". implode( ", ", $entid_permitidas ) ." )";
					
				$db->monta_combo("entid", $sql, 'S', "Selecione...", 'abreCampus(this.value, '.$orgid.');', '', '', '340', 'S', 'entid');

			}else{
					
				$entidade = new Entidade($dobras->getEntIdUnidade());
				$entnome  = $entidade->entnome;
				$entid    = $entidade->getPrimaryKey();
				?> <input type="hidden" name="entid" id="entid"
			value="<? if(isset($obrid)) echo $entid; ?>"> <?
			}

		}else{

			$entidade = new Entidade($dobras->getEntIdUnidade());
			$entnome = $entidade->entnome;
			$entid   = $entidade->getPrimaryKey();
			?> <input type="hidden" name="entid" id="entid"
			value="<? if(isset($obrid)) echo $entid; ?>"> <?
		}
		?> <span id="entnome"><?php echo $entnome; ?> <input type="hidden"
			name="entnome_h" id="entnome_h" value="<?php echo $entnome; ?>"></span>
			<?php if( $habilitado && !$entid_permitidas ){ ?> <input
			type="button" name="pesquisar_entidade" value="Pesquisar"
			style="cursor: pointer;"
			onclick="inserirEntidade(document.getElementById('entid').value,document.getElementById('orgid').value);"
			<?php if($somenteLeitura=="N") echo "disabled"; ?>> <img
			src="../imagens/obrig.gif" title="Indica campo obrigatório."
			border="0"> <?php } ?></td>
	</tr>
<!--	<tr id="campus" style="<?php echo $tr_campus; ?>">
		<td class="SubTituloDireita">Campus / Reitoria:</td>
		<td id="mostracampus"><?php

		$campus = new Entidade($dobras->getEntIdCampus());
		$campusnome = $campus->entnome;
		$entidcampus = $campus->getPrimaryKey();

		if ($_SESSION["obra"]["obrid"] && $entid){
			switch ($dobras->orgid){
				case 1:
					$orgaoSesu  = 'AND ef.funid = 18';
					$orgaoSesu2 = 'AND ef.funid = 75';
					break;
				case 2:
					$orgaoSesu  = 'AND ef.funid = 17';
					$orgaoSesu2 = 'AND ef.funid = 75';
					break;
				case ORGAO_REHUF:
					$orgaoSesu  = 'AND ef.funid = '.ID_HOSPITAL;
					$orgaoSesu2 = 'AND ef.funid = '.ID_HOSPITAL;
					break;
					

			}

			$sql = "
							SELECT
								et.entid as codigo,
								entnome as descricao
							FROM
								entidade.entidade et
							INNER JOIN
								entidade.funcaoentidade ef ON et.entid = ef.entid
							INNER JOIN
								entidade.funentassoc ea ON ea.fueid = ef.fueid 
							WHERE
								ea.entid = {$entid} " . $orgaoSesu .  " OR
								ea.entid = {$entid} " . $orgaoSesu2 . " AND
								et.entstatus = 'A' ";

			$db->monta_combo("entidcampus", $sql, $somenteLeitura, "Selecione...", '', '', '', '340', 'S', 'entidcampus');
		}

		?></td>
	</tr> -->
	<tr>
		<td class="SubTituloDireita">Nome da Obra:</td>
		<td><? 
		$obrdesc = $dobras->getObrDesc();
		$obrdesc = str_replace('"', '&#34', $obrdesc);
			
		?> <?= campo_texto( 'obrdesc', 'S', $somenteLeitura, '', 65, 120, '', '', 'left', '', 0); ?>
		<input type=hidden id=obrdesc_title value='<? echo $obrdesc; ?>'></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tipo de Obra:</td>
		<td><?php

		$bloqueadotipo = 'S';
		if ( !possuiPerfil( PERFIL_SUPERUSUARIO ) && !possuiPerfil( PERFIL_ADMINISTRADOR ) && $_SESSION["obra"]["orgid"] == ORGAO_FNDE ){
			$bloqueadotipo = 'N';
		}
			
		$tobraid = $dobras->getTobraId();
		$sql = "
					SELECT tobaid AS codigo, tobadesc AS descricao 
						FROM obras.tipoobra

				";
		$db->monta_combo("tobraid", $sql, ( $bloqueadotipo == 'N' ? $bloqueadotipo : $somenteLeitura ) , "Selecione...", 'abreInauguracao', '', '', '340', 'S', 'tobraid');
		?></td>
	</tr>
	<tr>
		<!--<td class="SubTituloDireita">Programa / Fonte:</td>-->
		<td class="SubTituloDireita">Programa:</td>
		<td>
		<?php

		$bloqueiaprg = "S";
// 		if($requisicao == "atualiza") {
// 			$sql_existe = "SELECT o.pliid FROM monitora.pi_obra o inner join monitora.pi_planointerno pi on o.pliid = pi.pliid WHERE o.obrid='".$_SESSION["obra"]["obrid"]."' AND pi.plistatus='A'";
// 			$existepi = $db->pegaUm($sql_existe);
// 			if($existepi) {
// 				$bloqueiaprg = "N";
// 			}
// 		}

		$bloqueadosub = 'S';
		if ( !possuiPerfil( PERFIL_SUPERUSUARIO ) && !possuiPerfil( PERFIL_ADMINISTRADOR ) && $orgid == ORGAO_FNDE ){
			$bloqueadosub = 'N';
		}

		$prfid = $dobras->getPrfId();
		$sql = "SELECT
				prfid as codigo,
				prfdesc as descricao
				FROM 
		 			obras.programafonte
		 		WHERE
		 			orgid = ". ($_REQUEST["org"] ? $_REQUEST["org"] : $orgid);
		$db->monta_combo("prfid", $sql, ( ( $bloqueiaprg == "N" || $bloqueadosub == 'N' ) ? 'N' : $somenteLeitura ), "Selecione...", 'ajaxCarregaTipologiaProg', '', '', '340', 'S', 'prfid');
		
		?>
		</td>
	</tr>
	<?php if($_SESSION["obras"]["orgid"] == ORGAO_FNDE){?>
	<tr>
		<td class="SubTituloDireita">Fonte:</td>
		<td>
		<?php

		$bloqueiaprg = "S";
		if($requisicao == "atualiza") {
			$sql_existe = "SELECT o.pliid FROM monitora.pi_obra o inner join monitora.pi_planointerno pi on o.pliid = pi.pliid WHERE o.obrid='".$_SESSION["obra"]["obrid"]."' AND pi.plistatus='A'";
			$existepi = $db->pegaUm($sql_existe);
			if($existepi) {
				$bloqueiaprg = "N";
			}
		}

		$bloqueadosub = 'S';
		if ( !possuiPerfil( PERFIL_SUPERUSUARIO ) && !possuiPerfil( PERFIL_ADMINISTRADOR ) && $orgid == ORGAO_FNDE ){
			$bloqueadosub = 'N';
		}
		//Recupera o ID da Fonte da Tabela "obras.obrainfraestrutura" pelo campo "fntid"
		$fntid = $dobras->getFntId();
		
		$sql = " SELECT
		         		fntid AS codigo, 
		         		fnddsc AS descricao
  				 FROM 
  				 		obras.fonte
			     WHERE
		 			orgid = ". ($_REQUEST["org"] ? $_REQUEST["org"] : $orgid);
		
		$db->monta_combo("fntid", $sql, ( ( $bloqueiaprg == "N" || $bloqueadosub == 'N' ) ? 'N' : $somenteLeitura ), "Selecione...", '', '', '', '340', 'S', 'fntid');
		
		?>
		</td>
	</tr>
	<?php }?>


	<tr>
		<td class="SubTituloDireita">Classificação da Obra:</td>
		<td><?php
		$cloid = $dobras->getCloId();
		$sql = "
							SELECT 
								cloid as codigo,
								clodsc as descricao
							FROM
								obras.classificacaoobra
							ORDER BY
								cloid";
			
		$db->monta_combo("cloid", $sql, $somenteLeitura, "Selecione...", 'abreDadosIndigenas( this.value ); ajaxCarregaTipologiaClass', '', '', '340', 'S', 'cloid');
		?></td>
	</tr>

<!--
	<tr>
		<td class="SubTituloDireita">Tipologia da Obra:</td>
		<td><span id="spanComboTopologia"> <?php
		$prfid = $dobras->getPrfId();
		$tpoid = $dobras->getTpoId();

		if ( $cloid && $prfid ){
			carregaTipologia( $cloid, $prfid );
		}else{
			carregaTipologiaClass($cloid);
		}
		//		$prfid = $dobras->getPrfId();
		//		$tpoid = $dobras->getTpoId();
		//
		//		if( ! $prfid && $cloid ){
		//			carregaTipologiaClass($cloid);
		//		}else{
		//			carregaTipologia( $cloid, $prfid );
		//		}
		//
		// Carrega arrays javascript com as descrições da tipologia
		$tpodetalhe = '';
		$arTipologias = $db->carregar( 'SELECT tpoid, tpodetalhe FROM obras.tipologiaobra' );

		if ( is_array( $arTipologias ) ) {
			echo '<script>
						var arTipologias = new Array();';
			foreach( $arTipologias as $arTipologia ) {
				echo "\n".'arTipologias[' . $arTipologia['tpoid'] . '] = new String("' . rtrim( $arTipologia['tpodetalhe'] ) . '");';
				//				if ( $arTipologia['tpoid'] == $tpoid ){ //$prfid
				//					$tpodetalhe = $arTipologia['tpodetalhe'];
				//				}
			} // foreach
			echo '</script>';
		} //is_array( $arTipologias )
		?> </span> <script>
			
			$$('#spanComboTopologia table tr td > img[src*="/imagens/exclui_p.gif"]').each(function(item) {
			      Event.observe(item, 'click', function (){	$('obrcomposicao').readOnly = false;
															$('obrcomposicao').value 	= '';});
			    });

//			$$('#spanComboTopologia table tr td > img[src*="/imagens/exclui_p.gif"]')[0].observe( 
//																								'click', 
//																								function (){ $('obrcomposicao').readOnly = false;
//																											 $('obrcomposicao').value 	= '';});
			
		</script></td>
	</tr> -->
	<tr>
		<td class="SubTituloDireita">Descrição / Composição da Obra:</td>
		<td><?php
		$obrcomposicao = $dobras->getObrComposicao();

		// Se nao tiver sido obrid assume o tpoDetalhe
		if ( empty( $dados['obrid'] ) && empty( $obrcomposicao ) ) {
			$obrcomposicao = $tpodetalhe;;
		}

//		$habil = 'S';//($cloid && $prfid) ? 'N' : 'S';

		$desc_composicao = "Fazer descrição funcional da obra na unidade especificando a área construída e os ambientes disponibilizados, além de outras informações relevantes.<br/><br/>Exemplo:<br/><br/>Bloco B<br/>Área Construída: 12.734 m²<br/>22 salas de aula com 1362 lugares<br/>08 laboratórios de T.I. com 248 lugares<br/>01 auditório para 60 pessoas<br/>Capacidade para atender cerca de 3200 alunos (matutino e noturno)<br/>Ar condicionado central para todas as salas";
		?> <?= campo_textarea( 'obrcomposicao', 'S', $somenteLeitura, '', '68', '6', '1500', '' , 0, $desc_composicao); ?>

		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Valor Previsto (R$):</td>
		<td><?php $obrvalorprevisto = number_format($dobras->getObrValorPrevisto(), 2, ",", "."); ?>
		<?= campo_texto( 'obrvalorprevisto', 'S', $somenteLeitura, '', 17, 15, '###.###.###.###,##', '', 'left', '', 0, 'id="obrcustocontrato"'); ?>
		</td>
	</tr>
	<?php

	$cloid = $dobras->getCloId();
	// Terra Indígena
	$boTerraIndigena = ( $cloid == 4 ) ? true : false;
	$endereco = new Endereco($dobras->getEndId());
	$entidade->enderecos[0] = $endereco;
	echo ''
	. '<tr>'
	. '<td colspan="2">Local da Obra</td>'
	. '</tr>'

	. '<tr>'
	. '<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>CEP:</label></td>'
	. '<td>'
	. '<input type="text" name="endereco[endcep]"';
	echo ( $somenteLeitura == "S" ) ? '' : ' disabled ';
	echo 'onkeyup="this.value=mascaraglobal(\'##.###-###\', this.value);" onblur="Entidade.__getEnderecoPeloCEP(this);" class="CampoEstilo" id="endcep" value="' . $entidade->enderecos[0]->endcep . '" size="13" maxlength="10" />'
	. ' <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>'
	. '</td>'
	. '</tr>'

	. '<tr id="escolha_logradouro_id" style="display:none">'
	. '<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Sugestão de Logradouro:</label></td>'
	. '<td>'
	. '<input readonly="readonly" type="text" name="endlog" class="CampoEstilo" id="endlog" value="' . $entidade->enderecos[0]->endlog . '" size="48" />'
	. '</td>'
	. '</tr>'

	. '<tr>'
	. '<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
                		<label id="lbLogadouro">';
	echo ( $boTerraIndigena === true ) ? 'Terra Indígena:' : 'Logradouro:';
	echo '</label>
                	</td>'
                	. '<td>'
                	. '<input readonly="readonly" type="text" name="endereco[endlog]" class="CampoEstilo" id="endlogradouro" value="' . $entidade->enderecos[0]->endlog . '" size="65" />'
                	. '</td>'
                	. '</tr>'
				
                	. '<tr id="trComunidadeIndigena" ';
                	echo ( $boTerraIndigena === true ) ? '>' : 'style="display:none" >';
                	echo '<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Comunidade:</label></td>'
                	. '<td>';
                	$endcomunidade = $entidade->enderecos[0]->endcomunidade;
                	echo campo_textarea( 'endcomunidade', 'S', $somenteLeitura, '', 68, 4, 200);
                	echo '</td>'
                	. '</tr>'
                
                	. '<tr>'
                	. '<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Número:</label></td>'
                	. '<td>'
                	. '<input type="text"';
					echo ( $somenteLeitura == "S" ) ? '' : ' disabled ';
					echo 'name="endereco[endnum]" class="CampoEstilo" id="endnum" value="' . $entidade->enderecos[0]->endnum . '" size="13" maxlength="8" />'
                	. '</td>'
                	. '</tr>'

                	. '<tr>'
                	. '<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Complemento:</label></td>'
                	. '<td>'
                	. '<input type="text"';
					echo ( $somenteLeitura == "S" ) ? '' : ' disabled ';
					echo '	name="endereco[endcom]" class="CampoEstilo" id="endcom" value="' . $entidade->enderecos[0]->endcom . '" size="65" maxlength="100" />'
                	. '</td>'
                	. '</tr>'

                	. '<tr>'
                	. '<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Bairro:</label></td>'
                	. '<td>'
                	. '<input readonly="readonly" type="text" name="endereco[endbai]" class="CampoEstilo" id="endbai" value="' . $entidade->enderecos[0]->endbai . '" size="20" />'
                	. '</td>'
                	. '</tr>'

                	. '<tr>'
                	. '<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Município/UF: </label></td>'
                	. '<td>'
                	. '<input readonly="readonly" type="text" name="mundescricao" class="CampoEstilo" id="mundescricao" value="' . $entidade->enderecos[0]->getMunDescricao() . '" size="20" />'
                	. '<input type="hidden" name="endereco[muncod]" id="muncod" class="CampoEstilo" value="' . $entidade->enderecos[0]->muncod . '" />'
                	. '<input readonly="readonly" type="text" name="endereco[estuf]" class="CampoEstilo" id="estuf" value="' . $entidade->enderecos[0]->estuf . '" style="width: 5ex; padding-left: 2px" />'
                	. '</td>'
                	. '</tr>';

                	?>

	<tr>
		<td>Coordenadas Geográficas</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Latitude</td>
		<td><?php 
		$medlatitude = $endereco->medlatitude;
		$latitude = explode(".", $medlatitude);
		$graulatitude = $latitude[0];
		$minlatitude = $latitude[1];
		$seglatitude = $latitude[2];
		$pololatitude = trim($latitude[3]);
		
			
		?> <?= campo_texto( 'graulatitude', 'N', 'N', '', 2, 2, '##', '', 'left', '', 0, 'id="graulatitude"'); ?>
		° <?= campo_texto( 'minlatitude', 'N', 'N', '', 2, 2, '##', '', 'left', '', 0, 'id="minlatitude" '); ?>
		' <?= campo_texto( 'seglatitude', 'N', 'N', '', 2, 2, '##', '', 'left', '', 0, 'id="seglatitude" '); ?>
		'' <input type="hidden" name="pololatitude" id="pololatitude" value='<?= $pololatitude; ?>'>
		<?php 
		if (trim($pololatitude) == "S") {
			echo "&nbsp;<span id='pololatitude_'>S</span>"; 
		} elseif(trim($pololatitude) == "N") {
			echo "&nbsp;<span id='pololatitude_'>N</span>";
		} else {
			echo "&nbsp;<span id='pololatitude_'></span>";
		}
		?>
		<?php print obrigatorio(); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Longitude</td>
		<td><?php 
		$medlongitude = $endereco->medlongitude;
		$longitude = explode(".", $medlongitude);
		$graulongitude = $longitude[0];
		$minlongitude = $longitude[1];
		$seglongitude = $longitude[2];
		?> <?= campo_texto( 'graulongitude', 'N', 'N', '', 2, 2, '##', '', 'left', '', 0, 'id="graulongitude"'); ?>
		° <?= campo_texto( 'minlongitude', 'N', 'N', '', 2, 2, '##', '', 'left', '', 0, 'id="minlongitude"'); ?>
		' <?= campo_texto( 'seglongitude', 'N', 'N', '', 2, 2, '##', '', 'left', '', 0, 'id="seglongitude"');  ?>
		'' &nbsp W <?php print obrigatorio(); ?></td>
	</tr>
	<?php
	#if($graulongitude != "" && $minlongitude != "" && $seglongitude != "" && $graulatitude != "" && $minlatitude != "" && $seglatitude != ""){ ?>
	<tr>
		<td class="SubTituloDireita"></td>
		<td><a href="#" onclick="abreMapa();">Visualizar / Buscar No Mapa</a>
		<input style="display: none;" type="text" name="endereco[endzoom]"
			id="endzoom"
			value=<? if ($entidade->enderecos[0]->endzoom ==null) echo "15"; else echo $entidade->enderecos[0]->endzoom;?>>
		</td>
	</tr>
	<?php #} ?>
	<tr>
		<td colspan="2">Situação do Imóvel <br />
		<span style="color: #dd0000; font-weight: bold;">O arquivo com o
		documento indicado em Situação Dominial deve ser inserido na aba
		Documentos (PDF).</span></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tipo de Aquisição do Terreno</td>
		<td><?php

		$aqiid = $infraestrutura->aqiid;
			
		$sql = "SELECT
							aqiid AS codigo, 
							aqidsc AS descricao 
						FROM 
							obras.tipoaquisicaoimovel
						ORDER BY
							aqiid";
			
		$db->monta_combo("aqiid", $sql, $somenteLeitura,'Selecione...', '', '', '', '', 'S', 'aqiid');
		?></td>
	</tr>
	<tr>
		<td width="300px" class="SubTituloDireita">Situação Dominial já
		Regularizada?</td>
		<td><?php
		$iexsitdominialimovelregulariza = $infraestrutura->iexsitdominialimovelregulariza;
		if ($iexsitdominialimovelregulariza == "t"){
			?> <input type="radio" name="iexsitdominialimovelregulariza"
			id="iexsitdominialimovelregulariza" value="1" checked
			<? echo $disabled ?> /> Sim <input type="radio"
			name="iexsitdominialimovelregulariza"
			id="iexsitdominialimovelregulariza" value="0" <? echo $disabled ?> />
		Não <?php }else { ?> <input type="radio"
			name="iexsitdominialimovelregulariza"
			id="iexsitdominialimovelregulariza" value="1" <? echo $disabled ?> />
		Sim <input type="radio" name="iexsitdominialimovelregulariza"
			id="iexsitdominialimovelregulariza" value="0" checked
			<? echo $disabled ?> /> Não <?php } ?> <?php print obrigatorio(); ?>
		</td>
	</tr>
	<tr>
		<td colspan="3">Sobre a Inauguração</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Inaugurada</td>
		<td colspan="2"><?php $obrstatusinauguracao = $dobras->getObrStatusInauguracao(); ?>

		<input type="radio" name="obrstatusinauguracao" id="naoseaplica"
			value="S" onclick="abreData(this.value);"
			<?php if($obrstatusinauguracao == "S") echo 'checked="checked"'; echo $disabled; ?> />
		Não se Aplica <br />
		<input type="radio" name="obrstatusinauguracao" id="naoinaugurada"
			value="N" onclick="abreData(this.value);"
			<?php if($obrstatusinauguracao == "N") echo 'checked="checked"';  echo $disabled; ?> />
		Não Inaugurada <br />
		<span style="display: none;" id="inaugurada"> <input type="radio"
			name="obrstatusinauguracao" id="ainaugurada" value="I"
			onclick="abreData(this.value);"
			<?php if($obrstatusinauguracao == "I") echo 'checked="checked"';  echo $disabled; ?> />
		Inaugurada </span></td>
	</tr>
	<tr id="dtprevisao" style="display: none;">
		<td class="SubTituloDireita">Data de Previsão da Inauguração</td>
		<td><?php $obrdtprevinauguracao = $dobras->getObrDtPrevInauguracao(); ?>
		<?= campo_data( 'obrdtprevinauguracao', 'N', $somenteLeitura, '', 'S' ); ?>
		</td>
	</tr>
	<tr id="dtinauguracao" style="display: none;">
		<td class="SubTituloDireita">Data de Inauguração</td>
		<td><?php $obrdtinauguracao = $dobras->getObrDtInauguracao(); ?> <?= campo_data( 'obrdtinauguracao', 'N', $somenteLeitura, '', 'S' ); ?>
		</td>
	</tr>
	<tr>
		<td colspan="3" class="subtitulocentro">Contatos</td>
	</tr>
	<tr>
		<td colspan="3"><script type="text/javascript">
				<!--
					function RemoveLinha( idContato ){
						if (confirm('Deseja realmente excluir este contato?')){
							var index = window.document.getElementById('linha_'+idContato).rowIndex;
							table = window.document.getElementById("responsaveiscontato");
							table.deleteRow(index);
						}
					}
				-->
			</script>
		<table id="responsaveiscontato" class="tabela" bgcolor="#f5f5f5"
			cellSpacing="1" cellPadding=3 align="center" width="95%">
			<tr>
				<td class="subtitulocentro" width="15%">Ação</td>
				<td class="subtitulocentro" width="55%">Nome</td>
				<td class="subtitulocentro" width="30%">Categoria de Responsabilidade</td>
			</tr>
			<?php

			if($_SESSION['obra']['obrid']){
				$sql = pg_query("
									SELECT
										rc.tprcid as tipo, 
										rc.recoid as responsavel,
										rc.entid as entidade, 
										et.entnome as nome,
										et.entnumcpfcnpj as cpf  
									FROM 
										obras.responsavelobra r 
									INNER JOIN 
										obras.responsavelcontatos rc ON r.recoid = rc.recoid
									INNER JOIN 
										entidade.entidade et ON rc.entid = et.entid
									WHERE 
										r.obrid = '". $_SESSION['obra']['obrid'] . "'  AND rc.recostatus = 'A'");
					
				while (($dados = pg_fetch_assoc($sql))){
					$tipo 		 = $dados["tipo"];
					$responsavel = $dados['responsavel'];
					$entidade    = $dados['entidade'];
					$id   = $dados['recoid'];
					$nome = $dados['nome'];
					$cpf  = $dados['cpf'];

					$sql2 = pg_query("
									SELECT 
										tiporesp.tprcid, 
										tiporesp.tprcdesc
									FROM 
										obras.tiporespcontato AS tiporesp");

					if($somenteLeitura == "S")
					$combo_responsabilidade = "<select style='width:170px' class='CampoEstilo' name=\"tprcid[" . $entidade . "]\" id=\"tprcid[" . $entidade . "]\">";
					else
					$combo_responsabilidade = "<select style='width:170px' class='CampoEstilo' name=\"tprcid[" . $entidade . "]\" id=\"tprcid[" . $entidade . "]\" disabled>";

					while ($tipos = pg_fetch_array($sql2)){
							
						$selected = "";
						$tprcid = $tipos['tprcid'];
						$tprcdescricao = $tipos['tprcdesc'];
							
						if ($tipo == $tprcid){
							$selected = "selected";

						}
						$combo_responsabilidade .= "<option value='{$tprcid}' {$selected}>" . $tprcdescricao . "</option>";
					}

					$combo_responsabilidade .= "</select>";

					if($somenteLeitura == "S"){
						$botoes = "<img src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" border=0 title=\"Editar\" onclick=\"atualizaResponsavel('" . $entidade . "')\">
							       &nbsp&nbsp&nbsp<img src=\"/imagens/excluir.gif\" style=\"cursor: pointer\"  border=0 title=\"Excluir\" onClick=\"RemoveLinha(" . $entidade . ");\">";
					}else{
						$botoes = "<img src=\"/imagens/alterar_pb.gif\" style=\"cursor: pointer\" border=0 title=\"Editar\">&nbsp&nbsp&nbsp
								   <img src=\"/imagens/excluir_01.gif\" style=\"cursor: pointer\"  border=0 title=\"Excluir\">";
					}

					if( $db->testa_superuser() || possuiPerfil(PERFIL_SUPERVISORMEC, PERFIL_EMPRESA) ||
					possuiPerfil(PERFIL_ADMINISTRADOR) ){
						$bt_envia_email = '&nbsp&nbsp&nbsp<img border="0" onclick=\'envia_email("' . $cpf . '");\' title="Enviar e-mail ao Gestor" src="../imagens/email.gif" style="cursor: pointer;"/>';
					}


					echo "
									<tr id=\"linha_" . $entidade . "\">
										<td bgcolor=\"#F7F7F7\" align=\"center\">" . $botoes . "</td>
										<td id=\"nomeEntidade_" . $entidade . "\">" . $codigo . $bt_envia_email . ' ' . $nome . $id . "</td>
										<td>" . $combo_responsabilidade . "</td>
									</tr>";
				}
			}
			?>
		</table>
		</td>
	</tr>
	<tr>
		<td colspan="3"><?php if($habilitado){ ?> <a href="#"
			onclick="inserirResponsavel(); return false;"> <img
			src="/imagens/gif_inclui.gif" style="cursor: pointer;" border="0"
			title="Inserir Contatos"> Inserir Contatos </a> <?php } ?></td>
	</tr>

	<?php if(  in_array($_SESSION["obra"]["orgid"], array(ORGAO_FNDE, ORGAO_SETEC, ORGAO_SESU)) ){ ?>

	<tr>
		<td colspan="3" class="subtitulocentro">Responsáveis pela Obra</td>
	</tr>
	<tr>
		<td colspan="3">
		<table id="responsaveisobra" class="tabela" bgcolor="#f5f5f5"
			cellSpacing="1" cellPadding=3 align="center" width="95%">
			<tr>
				<td class="subtitulocentro" width="15%">Ação</td>
				<td class="subtitulocentro" width="30%">CPF</td>
				<td class="subtitulocentro" width="55%">Nome</td>
			</tr>
			<?php litaResponsavelObra(); ?>
		</table>
		</td>
	</tr>
	<tr>
		<td colspan="3"><?php if($habilitado && possuiPerfil(array(PERFIL_ADMINISTRADOR, PERFIL_SUPERUSUARIO)) ){ ?>
		<a href="#"
			onclick="inserirEditorObras(<?=$dobras->getEntIdUnidade()?>);"> <img
			src="/imagens/gif_inclui.gif" style="cursor: pointer;" border="0"
			title="Associar Responsáveis"> Associar Responsáveis </a> <?php } ?>
		</td>
	</tr>

	<?php } ?>

	<tr>
		<td class="SubTituloDireita">Observação sobre a Obra:</td>
		<td colspan="2" ><? $obsobra = stripcslashes($dobras->getObsObra()); ?> <?= campo_textarea( 'obsobra', 'N', $somenteLeitura, '', '80', '6', '1000'); ?>
		</td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td></td>
		<td colspan="2">
		<div style="float: left;"><input type="hidden" name="requisicao"
			value="<?php echo $requisicao; ?>" /> <?php if($habilitado){ ?> <input
			type="submit" value="Salvar" style="cursor: pointer"
			<?php if($somenteLeitura=="N") echo "disabled"; ?>> <?php } ?> <input
			type="button" value="Voltar" style="cursor: pointer"
			onclick="history.back(-1);"></div>
		</td>
	</tr>
</table>
</form>

<script>abreDadosIndigenas(<?php print $dobras->getCloId()?>);</script>
			<?php

			if( $dobras->getStoId() == EM_CONSTRUCAO || $dobras->getStoId() == FINALIZADA ){
				echo "
		<script>
			
			if (document.selection){
				document.getElementById('inaugurada').style.display = 'block';
			}else{
				document.getElementById('inaugurada').style.display = 'table-row';
			}

			abreData('" . $obrstatusinauguracao . "');
		</script>";
			}
			?>
			<?php chkSituacaoObra();?>
			<?php 
				if($obrid){ 
						$gpdid = buscaGrupoPelaObra( $obrid );
						tramitaGrupo($gpdid); 
				}
			?>
