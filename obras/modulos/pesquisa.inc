<?php

$arOrgid = obras_recuperarPermissoesEntid( $arPerfilEntid );

if(!$habilitado){
	$excluir = '';
}else{
	$excluir = '<img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"javascript:Excluir(\\\'?modulo=inicio&acao=A&requisicao=excluir\\\', \' || oi.obrid || \');\">';
}

$obrOrgIdLista = $_REQUEST['org'] ? $_REQUEST['org'] : $res[0]['id'];

$argumento = "";

$sql = "
	SELECT
        '<center><img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' || et.entid || '\" name=\"+\" onclick=\"desabilitarConteudo( ' || et.entid || ' ); formatarParametros();abreconteudo(\'obras.php?modulo=inicio&acao=A&subAcao=gravarCarga&org={$_REQUEST['org']}&carga=' || et.entid || '&params=\' + params, ' || et.entid || ');\"/></center>' as img,
        CASE WHEN et.entsig <> '' THEN upper(et.entsig) || ' - ' || upper(et.entnome) ELSE upper(et.entnome) END as nome,
        COUNT(distinct oi.obrid) as total,
        '<tr><td style=\"padding:0px;margin:0;\"></td><td id=\"td' || et.entid || '\" colspan=\"2\" style=\"padding:0px;display:none;border: 5px red\"></td><td style=\"padding:0px;margin:0;\"></td></tr>' as tr
	FROM 
		obras.obrainfraestrutura oi 
	INNER JOIN 
		entidade.entidade et ON oi.entidunidade = et.entid and et.entstatus = 'A'
	INNER JOIN
		entidade.endereco ed ON oi.endid = ed.endid
	LEFT JOIN
		territorios.municipio mun ON ed.muncod = mun.muncod 
	LEFT JOIN 
		obras.situacaoobra sto ON oi.stoid = sto.stoid
	LEFT JOIN(
        SELECT fri.obrid, covi.covid, covi.covnumero
        FROM obras.formarepasserecursos fri
        INNER JOIN obras.conveniosobra covi ON covi.covid = fri.covid  )fr ON fr.obrid = oi.obrid
	LEFT JOIN
        obras.arquivosobra aa ON aa.obrid = oi.obrid and aa.tpaid = 21
    LEFT JOIN 
		obras.supervisao s ON s.obrid = oi.obrid AND 
		s.supstatus = 'A'
	LEFT JOIN
		monitora.pi_obra o ON o.obrid = oi.obrid 
	LEFT JOIN
		monitora.pi_planointerno mpi ON mpi.pliid = o.pliid AND mpi.plistatus = 'A'
	LEFT JOIN 
		obras.restricaoobra r ON r.obrid = oi.obrid AND 
		r.rststatus = 'A'
	LEFT JOIN
		obras.classificacaoobra cla ON oi.cloid = cla.cloid 
	INNER JOIN 
		obras.orgao org ON oi.orgid = org.orgid 
		%s %s AND  
		entnome <> ''
	GROUP BY 
		et.entsig, et.entnome, et.entid
	ORDER BY 
		et.entnome";

$res = obras_pegarOrgaoPermitido();

// Verifica as responsabilidades do usuário
if( !($db->testa_superuser()) && ( !possuiPerfil(PERFIL_CONSULTAGERAL) && 
								   !possuiPerfil( PERFIL_GESTORMEC ) ) ){

	// Pega os perfis do usuário
	$perfis = obras_arrayPerfil();
	
	$filtroObra = $_REQUEST['org'] == ORGAO_FNDE ? " ur.obrid = oi.obrid OR " : "";
	
	$argumento = "
				INNER JOIN
					obras.usuarioresponsabilidade ur ON ur.rpustatus = 'A' AND
					ur.pflcod in (" . implode("," , $perfis) . ") AND
					( {$filtroObra} ur.entid = oi.entidunidade OR
					( ur.estuf = ed.estuf AND
					  ur.orgid = org.orgid AND
					  ur.estuf IS NOT NULL ) OR
					( ur.orgid = org.orgid AND
					  ur.estuf IS NULL ) )"; 
	$criteria = " AND ur.usucpf = '{$_SESSION["usucpf"]}' ";

}else{
	$criteria = " AND oi.orgid = " . ($_REQUEST['org'] ? $_REQUEST['org'] : $res[0]['id']);	
}

if( $_REQUEST["pesquisa"] || $_SESSION['pesquisaObra'] ){
	
	# Se houver pesquisa/submit grava na sessão o filtro
	if ( $_REQUEST["pesquisa"] ){
		if( isset( $_SESSION['pesquisaObra'] ) ) {
			
			# Se tiver que carregar o detalhe/lista de obras
			switch ( true ) {
				case isset( $_SESSION['pesquisaObra']["carga"] ) : $carga = $_SESSION['pesquisaObra']["carga"]; break;
				case isset( $_SESSION['pesquisaObra']["cargaCampus"] )   : $_REQUEST["cargaCampus"] = $_SESSION['pesquisaObra']["cargaCampus"]; break;
				case isset( $_SESSION['pesquisaObra']["org"] )   : $_REQUEST["org"] = $_SESSION['pesquisaObra']["org"]; break;

			}
			#unset( $_SESSION['pesquisaObra']["carga"] );
			$_SESSION['pesquisaObra']["org"]    = $_REQUEST["org"];
		}
		$_SESSION['pesquisaObra']["orgid"]       = $_REQUEST["orgid"];
		$_SESSION['pesquisaObra']["stoid"]       = $_REQUEST["stoid"];
		$_SESSION['pesquisaObra']["tobaid"]      = $_REQUEST["tobaid"];
		$_SESSION['pesquisaObra']["cloid"]       = $_REQUEST["cloid"];
		$_SESSION['pesquisaObra']["prfid"]       = $_REQUEST["prfid"];
		$_SESSION['pesquisaObra']["entid"]       = $_REQUEST["entid"];
		$_SESSION['pesquisaObra']["obrdesc"]     = $_REQUEST["obrdesc"];
		$_SESSION['pesquisaObra']["foto"]        = $_REQUEST["foto"];
		$_SESSION['pesquisaObra']["convenio"]    = $_REQUEST["convenio"]; 
		$_SESSION['pesquisaObra']["vistoria"]    = $_REQUEST["vistoria"];
		$_SESSION['pesquisaObra']["restricao"]   = $_REQUEST["restricao"];
		$_SESSION['pesquisaObra']["planointerno"]= $_REQUEST["planointerno"];
		$_SESSION['pesquisaObra']["agrupamento"] = $_REQUEST["agrupamento"];
		$_SESSION['pesquisaObra']["org"]         = $_REQUEST["org"];
		$_SESSION['pesquisaObra']["estuf"]       = $_REQUEST["estuf"];
		$_SESSION['pesquisaObra']["percentualinicial"] = $_REQUEST["percentualinicial"];
		$_SESSION['pesquisaObra']["percentualfinal"]   = $_REQUEST["percentualfinal"];
		
	}
	
	$_REQUEST["orgid"]       = $_SESSION['pesquisaObra']["orgid"];
	$_REQUEST["stoid"]       = $_SESSION['pesquisaObra']["stoid"];
	$_REQUEST["tobaid"]      = $_SESSION['pesquisaObra']["tobaid"];
	$_REQUEST["cloid"]       = $_SESSION['pesquisaObra']["cloid"];
	$_REQUEST["prfid"]       = $_SESSION['pesquisaObra']["prfid"];
	$_REQUEST["entid"]       = $_SESSION['pesquisaObra']["entid"];
	$_REQUEST["obrdesc"]     = $_SESSION['pesquisaObra']["obrdesc"];
	$_REQUEST["foto"]        = $_SESSION['pesquisaObra']["foto"];
	$_REQUEST["convenio"]    = $_SESSION['pesquisaObra']["convenio"];
	$_REQUEST["vistoria"]    = $_SESSION['pesquisaObra']["vistoria"];
	$_REQUEST["restricao"]   = $_SESSION['pesquisaObra']["restricao"];
	$_REQUEST["planointerno"]= $_SESSION['pesquisaObra']["planointerno"];
	$_REQUEST["agrupamento"] = $_SESSION['pesquisaObra']["agrupamento"];
	$_REQUEST["org"]         = $_SESSION['pesquisaObra']["org"];
	$_REQUEST["estuf"]       = $_SESSION['pesquisaObra']["estuf"];
	$_REQUEST["percentualinicial"] = $_SESSION['pesquisaObra']["percentualinicial"];
	$_REQUEST["percentualfinal"] = $_SESSION['pesquisaObra']["percentualfinal"];
	
}

if ($res[0]["id"] != null){
		
?>
<html>
<body>
<form name="formulario" id="pesquisar" method="POST" action="">
	<table align="center" border="0" cellpadding="5" cellspacing="1" class="tabela" cellpadding="0" cellspacing="0">
		<input type="hidden" name="pesquisa" value="1">
		<tr>
			<td  bgcolor="#CCCCCC" colspan="2"><b>Argumentos da Pesquisa</b></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Agrupamento</td>
			<td>
				<input type='radio' name='agrupamento' value='obras' <? if($_REQUEST["agrupamento"] == "obras") { echo "checked"; } ?> /> Obras
				<input type='radio' name='agrupamento' value='unidades' <? if($_REQUEST["agrupamento"] == "unidades") echo "checked"; ?> /> Unidades
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Tipo de Obra</td>
			<td>
			<?
			$sql_radio = "SELECT tobaid AS codigo, tobadesc AS descricao FROM obras.tipoobra ORDER BY tobadesc;";
			
			$tobaid = $_REQUEST['tobaid'];
			
			$result = $db->carregar($sql_radio);
			for($k=0;$k<count($result);$k++){
				
				if($tobaid == $result[$k]["codigo"])
					$checked = "checked";
				else
					$checked = "";
					
				echo "<input type='radio' name='tobaid' value=".$result[$k]["codigo"]." {$checked}/> ".$result[$k]["descricao"];				
			}
			
			$checked = "";
			if($tobaid == '') {
				$checked = "checked";
			}
			?>			
			<input type='radio' name='tobaid' value='' <?=$checked?> /> Todas
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Situação da Obra</td>
			<td>
			<?
			$sql_radio = "SELECT stoid AS codigo, stodesc AS descricao FROM obras.situacaoobra ORDER BY stoordem;";
			
			$stoid = $_REQUEST['stoid'];
			
			$result = $db->carregar($sql_radio);
			for($k=0;$k<count($result);$k++){
				
				if($stoid == $result[$k]["codigo"])
					$checked = "checked";
				else
					$checked = "";
					
				echo "<input type='radio' name='stoid' value=".$result[$k]["codigo"]." {$checked}/> ".$result[$k]["descricao"];				
			}
			
			$checked = "";
			if($stoid == '') {
				$checked = "checked";
			}
			?>			
			<input type='radio' name='stoid' value='' <?=$checked?> /> Todas
			</td>
		</tr>
<!--
		<tr>
			<td class="SubTituloDireita">Classificação da Obra</td>
			<td>
				<?php
					$sql_class = "SELECT 
									cloid as codigo,
									clodsc as descricao
								  FROM 
								  	obras.classificacaoobra";
					
					$cloid = $_REQUEST['cloid'];
					
					$result = $db->carregar($sql_class);

					for($k = 0; $k < count($result); $k++){
				
						if( $cloid == $result[$k]["codigo"] ){
							$checked = "checked";
						}else{
							$checked = "";
						}	
						echo "<input type='radio' name='cloid' value=".$result[$k]["codigo"]." {$checked}/> ".$result[$k]["descricao"];
										
					}
					
					$checked = "";
					if($cloid == '') {
						$checked = "checked";
					}
					?>			
				<input type='radio' name='cloid' value='' <?=$checked?> /> Todas
			</td>
		</tr>
-->

		<tr>
			<td class="SubTituloDireita">Programa / Fonte</td>
			<td>
				<?php
					
					if( !empty( $_REQUEST["prfid"] ) ){
						$prfid = $_REQUEST["prfid"];
					}
					
					$sql_prog = "SELECT 
									prfid as codigo,
									prfdesc as descricao
								  FROM 
								  	obras.programafonte
								  WHERE
								  	orgid = " . ($_REQUEST['org'] ? $_REQUEST['org'] : $res[0]['id']);
					
					$db->monta_combo("prfid", $sql_prog, "S", "Todos", '', '', '', '250', 'N','programa');
					
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Unidade</td>
			<td>
			<?php
				if( !empty( $_REQUEST["entid"] ) ){
					$entid = $_REQUEST["entid"];
				}else{
					$where = "";
				}
	
				if ( !possuiPerfil( PERFIL_SUPERUSUARIO ) || !possuiPerfil( PERFIL_GESTORMEC ) || 
					 !possuiPerfil( PERFIL_CONSULTAGERAL ) ){
					$where = " obra.obsstatus = 'A' AND obra.orgid = " . ($_REQUEST["org"] ? $_REQUEST['org'] : $res[0]["id"]);	
				}else{
					$where = " obra.orgid = " . ($_REQUEST["org"] ? $_REQUEST['org'] : 1) . " AND obra.obsstatus = 'A'";
				}
				
				$sql_unidade = "
						SELECT 
							ent.entid as codigo, upper(ent.entnome) as descricao 
						FROM
							entidade.entidade ent
						INNER JOIN 
							obras.obrainfraestrutura obra ON obra.entidunidade = ent.entid 
						WHERE
							" . $where . " GROUP BY ent.entnome, ent.entid order by ent.entnome";
		     	
				$db->monta_combo("entid", $sql_unidade, "S", "Todas", '', '', '', '', 'N','unidade');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nome da Obra / Nº do Convênio / Nº do PI</td>
			<td>
			<? 
			$obrdesc = $_REQUEST["obrdesc"];
			echo campo_texto( 'obrdesc', 'N', 'S', '', 47, 60, '', '', 'left', '', 0, '');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">UF</td>
			<td>
				<?php
					if( !empty( $_REQUEST["estuf"] ) ){
						$estuf = $_REQUEST["estuf"];
					}
					
					$sql_uf = "SELECT
								estuf as codigo,
								estdescricao as descricao
							FROM
								territorios.estado";
					$db->monta_combo("estuf", $sql_uf, "S", "Todos", '', '', '', '150', 'N','estado');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Possui foto</td>
			<td>
				<input type='radio' name='foto' value='sim' <? if($_REQUEST["foto"] == "sim") echo "checked"; ?> /> Sim
				<input type='radio' name='foto' value='nao' <? if($_REQUEST["foto"] == "nao") echo "checked"; ?> /> Não
				<input type='radio' name='foto' value='todos' <? if((!isset($_REQUEST["foto"])) || ($_REQUEST["foto"] == "todos")) echo "checked"; ?> /> Todas
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Possui vistoria</td>
			<td>
				<input type='radio' name='vistoria' value='sim' <? if($_REQUEST["vistoria"] == "sim") echo "checked"; ?> /> Sim
				<input type='radio' name='vistoria' value='nao' <? if($_REQUEST["vistoria"] == "nao") echo "checked"; ?> /> Não
				<input type='radio' name='vistoria' value='todos' <? if((!isset($_REQUEST["vistoria"])) || ($_REQUEST["vistoria"] == "todos")) echo "checked"; ?> /> Todas
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Possui restrição</td>
			<td>
				<input type='radio' name='restricao' value='sim' <? if($_REQUEST["restricao"] == "sim") echo "checked"; ?> /> Sim
				<input type='radio' name='restricao' value='nao' <? if($_REQUEST["restricao"] == "nao") echo "checked"; ?> /> Não
				<input type='radio' name='restricao' value='todos' <? if((!isset($_REQUEST["restricao"])) || ($_REQUEST["restricao"] == "todos")) echo "checked"; ?> /> Todas
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Possui PI</td>
			<td>
				<input type='radio' name='planointerno' value='sim' <? if($_REQUEST["planointerno"] == "sim") echo "checked"; ?> /> Sim
				<input type='radio' name='planointerno' value='nao' <? if($_REQUEST["planointerno"] == "nao") echo "checked"; ?> /> Não
				<input type='radio' name='planointerno' value='todos' <? if((!isset($_REQUEST["planointerno"])) || ($_REQUEST["planointerno"] == "todos")) echo "checked"; ?> /> Todas
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">% Executado da Obra</td>
			<td>
				<table>
					<tr>
						<th>Mínimo</th>
						<th>Máximo</th>
					</tr>
					<tr>
				<?php
					
					$arPercentual[]  = array( 'codigo' =>  0 , 'descricao' => '0 %' );
					$arPercentual[]  = array( 'codigo' =>  5 , 'descricao' => '5 %' );
					$arPercentual[]  = array( 'codigo' => 10 , 'descricao' => '10 %' );
					$arPercentual[]  = array( 'codigo' => 15 , 'descricao' => '15 %' );
					$arPercentual[]  = array( 'codigo' => 20 , 'descricao' => '20 %' );
					$arPercentual[]  = array( 'codigo' => 25 , 'descricao' => '25 %' );
					$arPercentual[]  = array( 'codigo' => 30 , 'descricao' => '30 %' );
					$arPercentual[]  = array( 'codigo' => 35 , 'descricao' => '35 %' );
					$arPercentual[]  = array( 'codigo' => 40 , 'descricao' => '40 %' );
					$arPercentual[]  = array( 'codigo' => 45 , 'descricao' => '45 %' );
					$arPercentual[]  = array( 'codigo' => 50 , 'descricao' => '50 %' );
					$arPercentual[]  = array( 'codigo' => 55 , 'descricao' => '55 %' );
					$arPercentual[]  = array( 'codigo' => 60 , 'descricao' => '60 %' );
					$arPercentual[]  = array( 'codigo' => 65 , 'descricao' => '65 %' );
					$arPercentual[]  = array( 'codigo' => 70 , 'descricao' => '70 %' );
					$arPercentual[]  = array( 'codigo' => 75 , 'descricao' => '75 %' );
					$arPercentual[]  = array( 'codigo' => 80 , 'descricao' => '80 %' );
					$arPercentual[]  = array( 'codigo' => 85 , 'descricao' => '85 %' );
					$arPercentual[]  = array( 'codigo' => 90 , 'descricao' => '90 %' );
					$arPercentual[]  = array( 'codigo' => 95 , 'descricao' => '95 %' );
					$arPercentual[]  = array( 'codigo' => 100 , 'descricao' => '100 %' );
					
					$percentualinicial = $_REQUEST['percentualinicial'];
					$percentualfinal   = $_REQUEST['percentualfinal'];
					echo '<td>';
					$db->monta_combo("percentualinicial", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualinicial');
					echo '</td><td>';
					$db->monta_combo("percentualfinal", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualfinal', false, 100);
					echo '</td>';
				?>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input style="cursor:pointer;" type="submit" value="Pesquisar" /><input style="cursor:pointer;" type="button" value="Ver Todos" onCLick="VertodasObras()" />
			</td>
		</tr>
</table>
</form>
<table align="center" border="0" class="tabela" cellpadding="0" cellspacing="0" bgcolor="#CCCCCC">
	<tbody>
		<tr>
			<td>
				<?php
				if ( $org == 3 && !$db->testa_superuser() && !possuiPerfil( PERFIL_ADMINISTRADOR )  ){
						echo '<input type="button" name="cadastrar_obra" id="cadastrar_obra" value="Cadastrar Obra" style="cursor: pointer;" disabled="disabled"/>';	
					}else {
						$orgid = isset( $_REQUEST['org'] ) ? $_REQUEST['org'] : $res[0]["id"];
				?>
					<input type="button" name="cadastrar_obra" id="cadastrar_obra" value="Cadastrar Obra" style="cursor: pointer;" onclick="Cadastrar('?modulo=principal/cadastro&acao=A&subAcao=novaObra&org=<?php echo $orgid; ?>');" <?php echo $disabled; ?>/>
				<?php } ?>
			</td>
		</tr>
	</tbody>
</table>
<?php

	if($_REQUEST["agrupamento"] == "obras"){
		
		$sql = retornarSQLPesquisa();
		$sql = $db->carregar($sql);
		
		$arrayHtml = array("", 
						   "", 
						   "", 
						   "",
						   "",
						   "",
						   "",
						   "<a style=\"margin: 0 -20px 0 20px; text-transform:capitalize;\" href=\"#\" onclick=\"javascript:Atualizar('?modulo=principal/cadastro&acao=A',{campo[7]});\"> {campo[8]} </a>");
		
		$cabecalho = array( "Ação", "A", "F", "R", "PI", "AD", "ID", "Nome da Obra", "Unidade", "Município/UF", "Data de Início", "Data de Término", "Situação da Obra", "Ultima Atualização", "(%) Executado" );
		$db->monta_lista_array( $sql, $cabecalho, 200, 10, 'N', 'center', $arrayHtml ); 
		
	}else{
		$criteria1 .= ' WHERE ' . retornarFiltroPesquisa() . $criteria;
		$sql = sprintf( $sql, $argumento, $criteria1 );
		$cabecalho = array( "", "Unidade", "Quantidade de Obras" );
		$totais_obras = 'S';
		$db->monta_lista( $sql, $cabecalho, 100, 30, $totais_obras, 'center', '' );
				
	}
		
	
	
?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<table align="center" border="0" class="tabela" cellpadding="0" cellspacing="0" bgcolor="#CCCCCC">
	<tbody>
		<tr>
			<td>
				<?php
					if ( $org == 3 && !$db->testa_superuser() && !possuiPerfil( PERFIL_ADMINISTRADOR )  ){
						echo '<input type="button" name="cadastrar_obra" id="cadastrar_obra" value="Cadastrar Obra" style="cursor: pointer;" disabled="disabled"/>';	
					}else {
				?>
					<input type="button" name="cadastrar_obra" id="cadastrar_obra" value="Cadastrar Obra" style="cursor: pointer;" onclick="Cadastrar('?modulo=principal/cadastro&acao=A&subAcao=novaObra&org=<?php echo $orgid; ?>');" <?php echo $disabled; ?>/>
				<?php } ?>
			</td>
		</tr>
	</tbody>
</table>
<? }else { ?>

<table align="center" border="0" cellpadding="5" cellspacing="1" class="tabela" cellpadding="0" cellspacing="0">
	<tr style="text-align: center; color: #ff0000;">
		<td>
			Usuário sem permissões para visualizar obras
		</td>
	</tr>
</table>

<?} ?>
<div id="loading"></div>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">
var params;
function formatarParametros()
{
    params = Form.serialize($('pesquisar'));
    
}

function desabilitarConteudo( id ){
	var url = caminho_atual + '?modulo=inicio&acao=A&carga='+id;
	if ( document.getElementById('img'+id).name == '-' ) {
		url = url + '&subAcao=retirarCarga';
		var myAjax = new Ajax.Request(
			url,
			{
				method: 'post',
				asynchronous: false
			});
	}
	/*
	else{
		if ( document.getElementById('img'+id).name == '+' ) {
			url = url + '&subAcao=gravarCarga';
		}
	}*/
		
}

<?php
if ( !isset( $_REQUEST["pesquisa"] ) && !empty( $_SESSION['pesquisaObra']["carga"] ) ){
	if ( is_array( $_SESSION['pesquisaObra']["carga"] ) ){
		foreach ( $_SESSION['pesquisaObra']["carga"] as $carga => $lixo) {
			echo "
				formatarParametros();
				abreconteudo('obras.php?modulo=inicio&acao=A&carga=".$carga."&params=' + params, ".$carga.");";
		}
	}
}
?>
</script>
</body>
</html>
