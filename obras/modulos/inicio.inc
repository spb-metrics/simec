<?php
/**
 * Página inicial do módulo Monitoramento de Obras
 * @author Fernando Bagno <fernandosilva@mec.gov.br>
 * @since 27/04/2010
 * @version 2.0
 * 
 */
if( $_POST['agrupamento'] ){
	$_SESSION["obras"]["filtros"]["agrupamento"] = $_POST['agrupamento'];
	$valida = '1';
}

//if( !$db->testa_superuser() && possuiPerfil( PERFIL_EMPRESA ) ){
//	header( "location:obras.php?modulo=principal/supervisao/rotas&acao=A" );
//	die;
//}

ini_set("memory_limit", "1024M");

// Objetos da página
$obrInicio = new inicio();
$obras	   = new Obras();

// Requisições diversas da tela
switch( $_REQUEST["requisicao"] ){
	
	case "atualizaarvore":
		$_SESSION["obrasarvore"]["arEntid"]       = $_REQUEST["arEntid"];	
		$_SESSION["obrasarvore"]["arEntidCampus"] = $_REQUEST["arEntidCampus"];	
		$_SESSION["obrasbkp"]["filtrosbkp"]       = $_SESSION["obras"]["filtros"]["filtros"];
		die;
	break;	
	
	case "filtrar":
		$_SESSION["obras"]["filtros"]["filtros"] = $obrInicio->filtraListaDeObras( $_REQUEST );	
	break;
	
	case "limpar":
		$_SESSION["obras"]["filtros"] = null;
	break;
	
	case "listafilho":
		$obrInicio->listaFilhos( $_REQUEST["entid"], $_SESSION["obras"]["filtros"]["filtros"] );
		die;	
	break;
	
	case "listaobra":
		$obrInicio->listaObras( $_REQUEST["entid"], $_SESSION["obras"]["filtros"]["filtros"], '', '', '', 'simples' );
		die;	
	break;

	case "listaobraSC":
		$obrInicio->listaObras( $_REQUEST["entid"], $_SESSION["obras"]["filtros"]["filtros"], 'abresc', '', '', 'simples' );
		die;	
	break;

	case "excluir":
		if ( true !== $stMensagem = $obras->DeletarObras($_REQUEST["obrid"]) ){
			alert( $stMensagem );
		}
	break;
	
	case "ordem":
		
		$_SESSION["obras"]["lista"] = "lista";
		
		switch( $_POST["ordem"] ){
			
			case 1 :
				$_SESSION["obras"]["ordem"] = "anexo";
			break;
			case 2 :
				$_SESSION["obras"]["ordem"] = "foto";
			break;
			case 3 :
				$_SESSION["obras"]["ordem"] = "restricoes";
			break;
			case 4 :
				$_SESSION["obras"]["ordem"] = "pi";
			break;
			case 5 :
				$_SESSION["obras"]["ordem"] = "aditivo";
			break;
			case 6 :
				$_SESSION["obras"]["ordem"] = "id";
			break;
			case 7 :
				$_SESSION["obras"]["ordem"] = "oi.numconvenio";
			break;
			case 8 :
				$_SESSION["obras"]["ordem"] = "nome";
			break;
			case 9 :
				$_SESSION["obras"]["ordem"] = "entdescricao";
			break;
			case 10 :
				$_SESSION["obras"]["ordem"] = "municipiouf";
			break;
			case 11 :
				$_SESSION["obras"]["ordem"] = "inicio";
			break;
			case 12 :
				$_SESSION["obras"]["ordem"] = "termino";
			break;
			case 13 :
				$_SESSION["obras"]["ordem"] = "situacao";
			break;
			case 14 :
				$_SESSION["obras"]["ordem"] = "atualizacao";
			break;
			case 15 :
				$_SESSION["obras"]["ordem"] = "executado";
			break;
			
		}
		die;
	break;
	
}

if( $_SESSION["obrasbkp"]["orgid"] ){
	
	$_SESSION["obras"]["filtros"] = $_SESSION["obrasbkp"]["filtrosbkp"];
	$_SESSION["obras"]["orgid"]   = $_SESSION["obrasbkp"]["orgid"];
	$_SESSION["obras"]["ordem"]   = $_SESSION["obrasbkp"]["ordem"];
	$_SESSION["obras"]["lista"]   = $_SESSION["obrasbkp"]["lista"];
	
	//session_unregister("obrasbkp");
	unset($_SESSION["obrasbkp"]);
	
	header( "location:obras.php?modulo=inicio&acao=A&orgid=".$_SESSION["obras"]["orgid"] );
	
}
//window.location='obras.php?modulo=inicio&acao=A&orgid=3'
// cria a sessão com o tipo de ensino selecionado
$_SESSION["obras"]["orgid"] = !empty($_REQUEST["orgid"]) ? $_REQUEST["orgid"] : $_SESSION["obras"]["orgid"]; 
$_SESSION["obra"]["orgid"]  = !empty($_REQUEST["orgid"]) ? $_REQUEST["orgid"] : $_SESSION["obra"]["orgid"]; 

if ( !possuiPerfil( PERFIL_ADMINISTRADOR ) ) {
	$dados = obrPegaOrgidPermitido( $_SESSION["usucpf"] );
	$i=0;
	if ( is_array($dados) && $dados[0] ){
		foreach( $dados as $orgao ){
			$orgid[$i] = $orgao['id'];
			$i++;
		}
		if ( !in_array($_SESSION["obras"]["orgid"],$orgid) ){
			$_SESSION["obras"]["orgid"] = $orgid[0];
			header( "location:obras.php?modulo=inicio&acao=A&orgid=".$orgid[0] );
			die;
		}
	}
}

// Cabeçalho padrão do sistema
include  APPRAIZ."includes/cabecalho.inc";


/*
 * Testa se abre o popup
 */
if(!$db->testa_superuser()) {
	$sql = "select count(a.arqid) as c
			from obras.fotos f 
			inner join obras.supervisao s on s.supvid=f.supvid 
			inner join public.arquivo a on a.arqid=f.arqid 
			inner join obras.obrainfraestrutura o ON o.obrid = s.obrid 
			inner join obras.situacaoobra so ON so.stoid = o.stoid 
			where a.arqid/1000 between 647 and 725
			and a.arqid not in ( select distinct arqid from public.arquivo_recuperado)  
			and supstatus='A' and sisid=15 and obsstatus = 'A' and a.usucpf='".$_SESSION['usucpf']."'";
	$carq1 = $db->pegaUm($sql);
}

if(!$db->testa_superuser()) {
	$sql = "select count(a.arqid) as c
		 	from obras.arquivosobra f 
			inner join public.arquivo a on a.arqid=f.arqid 
			inner join obras.obrainfraestrutura o ON o.obrid = f.obrid 
			inner join obras.situacaoobra so ON so.stoid = o.stoid 
			where a.arqid/1000 between 647 and 725
			and a.arqid not in ( select distinct arqid from public.arquivo_recuperado)  
			and aqostatus='A' and sisid=15  and obsstatus = 'A' and a.usucpf='".$_SESSION['usucpf']."'";
	$carq2 = $db->pegaUm($sql);
}

$carq = ($carq1 + $carq2);

if($carq > 0 ) {
	$texto = "<center>
				<img src=\"../imagens/alerta_sistema.gif\" />
			    <p><font size=3 color=red><b>Nota do Sistema!</b></font></p>
           	     <p style=\"font-weight:bold\" ><font size=3>Alguns arquivos que você anexou foram corrompidos,<br />
           	     para corrigir o problema você poderá enviá-los novamente.<br />
           	     Clique abaixo para ver a relação e proceder a correção.</p>
           	     <input type=\"button\" value=\"Ver a relação de arquivos\" style=\"cursor:pointer;font:16px Trebuchet Ms,Arial,Tahoma,Verdana,Helvetica,Sans-Serif;height:33px\" onclick=\"window.location='obras.php?modulo=sistema/public_arquivo/obras_arquivo&acao=A'\" />
          	  </center>";
	popupAlertaGeral($texto,"580px","260px");
}

/*
 * FIM - Testa se abre o popup
 */

if( (obrPegaOrgidPermitido($_SESSION["usucpf"]) /*&& !possuiPerfil(PERFIL_SAA)*/) || $db->testa_superuser()){
	
	// Monta o título da dela no padrão do sistema
	print "<br/>";
	print obrMontaAbasTipoEnsino( $_SESSION["usucpf"], $_REQUEST["orgid"] );
	monta_titulo( "Lista de Obras", "" );
	// cria a sessão com o tipo de ensino selecionado
	$_SESSION["obras"]["orgid"] = !empty($_REQUEST["orgid"]) ? $_REQUEST["orgid"] : $_SESSION["obras"]["orgid"]; 
	$_SESSION["obra"]["orgid"]  = !empty($_REQUEST["orgid"]) ? $_REQUEST["orgid"] : $_SESSION["obra"]["orgid"]; 
		
?>
	
	<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
	
	<form name="formulario" id="obrFormPesquisa" method="post" action="">
		<input type="hidden" name="requisicao" id="requisicao" value="filtrar"/>
		<input type="hidden" name="numero" value="" />
		<input type="hidden" name="arEntid"       id="arEntid" 	     value="<?=$_SESSION["obrasarvore"]["arEntid"] ?>" />
		<input type="hidden" name="arEntidCampus" id="arEntidCampus" value="<?=$_SESSION["obrasarvore"]["arEntidCampus"] ?>"/>
		<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding=3 align="center">
			<tr>
				<td colspan="2" class="subTituloCentro">Argumentos de Pesquisa</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Agrupar por:</td>
				<td> <!--Se sessão da Página Ensino Básico for orgid = 3, checar radio button, Obras.  -->
					<?php 
					if( $valida <> '1' && !($_SESSION["obras"]["filtros"]["agrupamento"]) || $_POST['agrupamento'] == 'O' || $_SESSION["obras"]["filtros"]["agrupamento"] == 'O' ){
						$chkObras = 'checked="checked"';
						$chkUnidades = '';
					}
					/*Se sessão da Página Ensino Superior for orgid = 1	ou Ensino Profissional for orgid = 2, checar radio button, Unidade.*/
					elseif( $valida <> '1' || $_POST['agrupamento'] == 'U' || $_SESSION["obras"]["filtros"]["agrupamento"] == 'U' ){
						$chkUnidades = 'checked="checked"';
						$chkObras = '';
					}
//					$chkObras = 'checked="checked"';
//					$chkUnidades = '';
					?> 
					<input type="radio" name="agrupamento" id="agrupamentoU" value="U"  <?php echo $chkUnidades; ?>/> Unidades
					<input type="radio" name="agrupamento" id="agrupamentoO" value="O"  <?php echo $chkObras; ?>/> Obras
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Tipo de Obra:</td>
				<td>
					<?php 
					
						$tobaid = $_SESSION["obras"]["filtros"]["tobaid"];
						$sql = "SELECT 
									tobaid as codigo, 
									tobadesc as descricao 
								FROM 
									obras.tipoobra 
								ORDER BY 
									tobadesc";
					
						$db->monta_combo( "tobaid", $sql, "S", "Todos", "", "", "", "", "N", "tobaid" );
						
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Situação da Obra:</td>
				<td>
					<?php 
					
						$stoid = $_SESSION["obras"]["filtros"]["stoid"];
						
						$sql = "SELECT 
									stoid as codigo, 
									stodesc as descricao 
								FROM 
									obras.situacaoobra 
								ORDER BY 
									stodesc";
					
						$db->monta_combo( "stoid", $sql, "S", "Todas", "", "", "", "", "N", "stoid" );
						
					?>
				</td>
			</tr>
<!--
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Classificação da Obra:</td>
				<td>
					<?php 
					
						$cloid = $_SESSION["obras"]["filtros"]["cloid"];
					
						$sql = "SELECT 
									cloid as codigo,
									clodsc as descricao
								FROM 
								  	obras.classificacaoobra
								ORDER BY
									clodsc";
					
						$db->monta_combo( "cloid", $sql, "S", "Todas", "", "", "", "", "N", "cloid" );
						
					?>
				</td>
			</tr>
-->
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Programa / Fonte:</td>
				<td>
					<?php 
					
						$prfid = $_SESSION["obras"]["filtros"]["prfid"];
						
						$sql = "SELECT 
									prfid as codigo,
									prfdesc as descricao
							  	FROM 
							  		obras.programafonte
							  	WHERE
							  		orgid = {$_SESSION["obras"]["orgid"]}
							  	ORDER BY
							  		prfdesc";
					
						$db->monta_combo( "prfid", $sql, "S", "Todos", "", "", "", "", "N", "prfid" );
						
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Unidade:</td>
				<td>
					<?php 
					
						$entidunidade = $_SESSION["obras"]["filtros"]["entidunidade"];
						
						$sql = "SELECT 
									ee.entid as codigo, 
									upper(ee.entnome) as descricao 
								FROM
									entidade.entidade ee
								INNER JOIN 
									obras.obrainfraestrutura oi ON oi.entidunidade = ee.entid 
								WHERE
									orgid = {$_SESSION["obras"]["orgid"]} AND
									obsstatus = 'A'
								GROUP BY 
									ee.entnome, 
									ee.entid 
								ORDER BY 
									ee.entnome";
					
						$db->monta_combo( "entidunidade", $sql, "S", "Todos", "", "", "", "", "N", "entidunidade" );
						
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Última atualização:</td>
				<td>
					<?php 
					
						$ultatualizacao = $_SESSION["obras"]["filtros"]["ultatualizacao"];
						
						$arSel = array(
										array("codigo"    => 1,
										      "descricao" => "Em até 45 dias"),
										array("codigo"    => 2,
										      "descricao" => "Entre 45 e 60 dias"),
										array("codigo"    => 3,
										      "descricao" => "Mais de 60 dias"),
									  );
					
						$db->monta_combo("ultatualizacao", $arSel, "S", "Todos", "", "", "", "", "N", "ultatualizacao" );
						
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Nome da Obra / Nº do Convênio / ID:</td>
				<td>
					<?php $obrtextobusca = $_SESSION["obras"]["filtros"]["obrtextobusca"]; ?>
					<?php print campo_texto( 'obrtextobusca', 'N', 'S', '', 47, 60, '', '', 'left', '', 0, ''); ?>
				</td>
			</tr>
			<tr>

				<td class="SubTituloDireita" style="width: 190px;">UF:</td>
				<td>
					<?php 
					
						$estuf = $_SESSION["obras"]["filtros"]["estuf"];
						
						$sql = "SELECT
									estuf as codigo,
									estdescricao as descricao
								FROM
									territorios.estado
								ORDER BY
									estdescricao";
					
						$db->monta_combo( "estuf", $sql, "S", "Todas", "", "", "", "", "N", "estuf" );
						
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Possui foto:</td>
				<td>
					<input type="radio" name="foto" id="" value="S" <?php if( $_SESSION["obras"]["filtros"]["foto"] == "S" ){ print "checked='checked'"; } ?>/> Sim
					<input type="radio" name="foto" id="" value="N" <?php if( $_SESSION["obras"]["filtros"]["foto"] == "N" ){ print "checked='checked'"; } ?>/> Não
					<input type="radio" name="foto" id="" value="" <?php if( $_SESSION["obras"]["filtros"]["foto"] == "" ){ print "checked='checked'"; } ?> /> Todas
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Possui vistoria:</td>
				<td>
					<input type="radio" name="vistoria" id="" value="S" <?php if( $_SESSION["obras"]["filtros"]["vistoria"] == "S" ){ print "checked='checked'"; } ?>/> Sim
					<input type="radio" name="vistoria" id="" value="N" <?php if( $_SESSION["obras"]["filtros"]["vistoria"] == "N" ){ print "checked='checked'"; } ?>/> Não
					<input type="radio" name="vistoria" id="" value="" <?php if( $_SESSION["obras"]["filtros"]["vistoria"] == "" ){ print "checked='checked'"; } ?>/> Todas
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Possui restrição:</td>
				<td>
					<input type="radio" name="restricao" id="" value="S" <?php if( $_SESSION["obras"]["filtros"]["restricao"] == "S" ){ print "checked='checked'"; } ?>/> Sim
					<input type="radio" name="restricao" id="" value="N" <?php if( $_SESSION["obras"]["filtros"]["restricao"] == "N" ){ print "checked='checked'"; } ?>/> Não
					<input type="radio" name="restricao" id="" value="" <?php if( $_SESSION["obras"]["filtros"]["restricao"] == "" ){ print "checked='checked'"; } ?>/> Todas
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Possui PI:</td>
				<td>
					<input type="radio" name="planointerno" id="" value="S" <?php if( $_SESSION["obras"]["filtros"]["planointerno"] == "S" ){ print "checked='checked'"; } ?>/> Sim
					<input type="radio" name="planointerno" id="" value="N" <?php if( $_SESSION["obras"]["filtros"]["planointerno"] == "N" ){ print "checked='checked'"; } ?>/> Não
					<input type="radio" name="planointerno" id="" value="" <?php if( $_SESSION["obras"]["filtros"]["planointerno"] == "" ){ print "checked='checked'"; } ?>/> Todas
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Possui Aditivo:</td>
				<td>
					<input type="radio" name="aditivo" id="" value="S" <?php if( $_SESSION["obras"]["filtros"]["aditivo"] == "S" ){ print "checked='checked'"; } ?>/> Sim
					<input type="radio" name="aditivo" id="" value="N" <?php if( $_SESSION["obras"]["filtros"]["aditivo"] == "N" ){ print "checked='checked'"; } ?>/> Não
					<input type="radio" name="aditivo" id="" value="" <?php if( $_SESSION["obras"]["filtros"]["aditivo"] == "" ){ print "checked='checked'"; } ?>/> Todas
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Possui Supervisão:</td>
				<td>
					<input type="radio" name="supervisao" id="" value="S" <?php if( $_SESSION["obras"]["filtros"]["supervisao"] == "S" ){ print "checked='checked'"; } ?>/> Sim
					<input type="radio" name="supervisao" id="" value="N" <?php if( $_SESSION["obras"]["filtros"]["supervisao"] == "N" ){ print "checked='checked'"; } ?>/> Não
					<input type="radio" name="supervisao" id="" value="" <?php if( $_SESSION["obras"]["filtros"]["supervisao"] == "" ){ print "checked='checked'"; } ?>/> Todas
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Valor da Obra:</td>
				<td>
					De:&nbsp;
					<?php $vlrmenor = $_SESSION["obras"]["filtros"]["vlrmenor"]; echo campo_texto( 'vlrmenor', 'N', 'S', '', 11, 30, '[#].##', '', 'left', '', 0, '');?>
					Até:&nbsp;
					<?php $vlrmaior = $_SESSION["obras"]["filtros"]["vlrmaior"]; echo campo_texto( 'vlrmaior', 'N', 'S', '', 11, 30, '[#].##', '', 'left', '', 0, '');?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">% Executado da Obra:</td>
				<td>
					<table>
						<tr>
							<th>Mínimo</th>
							<th>Máximo</th>
						</tr>
						<tr>
							<?php
								
								$arPercentual[] = array( 'codigo' =>  0 , 'descricao' => '0 %' );
								$arPercentual[] = array( 'codigo' =>  5 , 'descricao' => '5 %' );
								$arPercentual[] = array( 'codigo' => 10 , 'descricao' => '10 %' );
								$arPercentual[] = array( 'codigo' => 15 , 'descricao' => '15 %' );
								$arPercentual[] = array( 'codigo' => 20 , 'descricao' => '20 %' );
								$arPercentual[] = array( 'codigo' => 25 , 'descricao' => '25 %' );
								$arPercentual[] = array( 'codigo' => 30 , 'descricao' => '30 %' );
								$arPercentual[] = array( 'codigo' => 35 , 'descricao' => '35 %' );
								$arPercentual[] = array( 'codigo' => 40 , 'descricao' => '40 %' );
								$arPercentual[] = array( 'codigo' => 45 , 'descricao' => '45 %' );
								$arPercentual[] = array( 'codigo' => 50 , 'descricao' => '50 %' );
								$arPercentual[] = array( 'codigo' => 55 , 'descricao' => '55 %' );
								$arPercentual[] = array( 'codigo' => 60 , 'descricao' => '60 %' );
								$arPercentual[] = array( 'codigo' => 65 , 'descricao' => '65 %' );
								$arPercentual[] = array( 'codigo' => 70 , 'descricao' => '70 %' );
								$arPercentual[] = array( 'codigo' => 75 , 'descricao' => '75 %' );
								$arPercentual[] = array( 'codigo' => 80 , 'descricao' => '80 %' );
								$arPercentual[] = array( 'codigo' => 85 , 'descricao' => '85 %' );
								$arPercentual[] = array( 'codigo' => 90 , 'descricao' => '90 %' );
								$arPercentual[] = array( 'codigo' => 95 , 'descricao' => '95 %' );
								$arPercentual[] = array( 'codigo' => 100 , 'descricao' => '100 %' );
								
								$percentualinicial = $_SESSION["obras"]["filtros"]['percentualinicial'];
								$percentualfinal   = $_SESSION["obras"]["filtros"]['percentualfinal'];
								
								$percfinal = $percentualfinal == '' ? 100 : $percentualfinal; 
								
								print '<td>';
								$db->monta_combo("percentualinicial", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualinicial');
								print '</td><td>';
								$db->monta_combo("percentualfinal", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualfinal', false,$percfinal);
								print '</td>';
								
							?>
						</tr>
					</table>
				</td>
			</tr>
			<tr bgcolor="#D0D0D0">
				<td style="width: 190px;"></td>
				<td>
					<input type="button" name="obrBtPesquisaInicio" value="Pesquisar" onclick="obrFiltraLista();" style="cursor: pointer;"/>
					<input type="button" name="obrBtVerTudoInicio" value="Ver Todas" onclick="obrVerTodas();" style="cursor: pointer;"/>
				</td>
			</tr>
		</table>
	</form>
		<?php 
		//fazendo o include da classe
		require_once APPRAIZ . "includes/classes/Atualizacao.class.inc";
//		$mensagem = '<p align="center" style="font-size: 15px;"><font size="4" color="red"><b>Atenção!</b></font><br><br>Existem novas atualizações.<br><br>Para visualizar as atualizações recentes clique no ícone <br><br><table><tbody><tr><td align="right"><img border="0" align="right" src="../includes/layout/montanhas/img/bt_help.png" title="Atualizações" alt="Atualizações"></td><td align="left"><font color="#dd0000"><strong>Atualizações</strong></font></td></tr></tbody></table></p>';
		$Atualizacao = new Atualizacao("float: right; position: absolute; top: 153px; right: 75px;");
	?>
	
	<?php 
	$arPerfilIncluiObra = $_SESSION["obras"]["orgid"] == ORGAO_FNDE ? array(PERFIL_ADMINISTRADOR) : array( PERFIL_ADMINISTRADOR, PERFIL_SUPERVISORUNIDADE,PERFIL_SUPERVISORORGAO, PERFIL_GESTORORGAO, PERFIL_SUPERVISORMEC, PERFIL_GESTORUNIDADE );
	$permissaoIncluiObra = possuiPerfil( $arPerfilIncluiObra );
	
	if( $permissaoIncluiObra ): ?>
	<table class="tabela" bgcolor="#FFFFFF" cellspacing="1" cellpadding=3 align="center">
		<tr>
			<td style="font-weight: bold;">
				<a style="cursor: pointer;" onclick="obrIrParaCaminho( '', 'novaobra', <?php print $_SESSION["obras"]["orgid"]; ?> );" title="Clique para incluir uma nova obra no sistema">
					<img src="../imagens/obras/incluir.png" style="width: 15px; vertical-align: middle;"/> Incluir nova obra
				</a>
			</td>
		</tr>
	</table>
	<?php endif; ?>
	
	<?php
		// Valida a listas cadastradas no sistema
		//Se Ensino Básico com orgid = 3 e valida != 1, listar Obras.  
		if( $valida <> '1' && !($_SESSION["obras"]["filtros"]["agrupamento"]) ){
			$_SESSION["obras"]["filtros"]["agrupamento"] = 'O';
		//Senão se Ensino Superior com orgid = 1 ou Ensino Profissional com orgid = 2 e valida != 1, listar Unidades.  
		}
//		elseif($_SESSION['obras']['orgid'] <> 3 && $valida <> '1'){
//			$_SESSION["obras"]["filtros"]["agrupamento"] = 'U';
//		}
//		$_SESSION["obras"]["filtros"]["agrupamento"] = 'O';
		// Monta a lista de obras cadastradas no sistema 
//		ver($_SESSION["obras"]["filtros"]["filtros"]);
		$obrInicio->listaDeObras( $_SESSION["obras"]["filtros"]["filtros"] );
	?>
	
	<?php if( $permissaoIncluiObra ): ?>
	<table class="tabela" bgcolor="#FFFFFF" cellspacing="1" cellpadding=3 align="center">
		<tr>
			<td style="font-weight: bold;">
				<a style="cursor: pointer;" onclick="obrIrParaCaminho( '', 'novaobra', <?php print $_SESSION["obras"]["orgid"]; ?> );" title="Clique para incluir uma nova obra no sistema">
					<img src="../imagens/obras/incluir.png" style="width: 15px; vertical-align: middle;"/> Incluir nova obra
				</a>
			</td>
		</tr>
	</table>
	<?php endif; ?>
<?php }else{ ?>

	<?php print "<br/>"; ?>
	<?php monta_titulo( "Monitoramento de Obras", "" ); ?>
	<table align="center" border="0" cellpadding="5" cellspacing="1" class="tabela" cellpadding="0" cellspacing="0">
		<tr style="text-align: center; color: #ff0000;">
			<td>
				Usuário sem permissões para visualizar obras
			</td>
		</tr>
	</table>
<?php } ?>
