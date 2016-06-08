<?php

// ajax que busca os dados da obra
if( isset($_REQUEST['gpdid']) ){

	global $db;
	
	$sql = "SELECT 
				oo.orgdesc as tipo_de_ensino,
				ee.entnome as unidade,
				obrdesc as nome_da_obra,
				tm.mundescricao as municipio,
				CASE WHEN oc.covid IS NOT NULL THEN covnumero ELSE (CASE WHEN numconvenio is not null THEN numconvenio ELSE 'Não Informado' END) END as convenio,
				stodesc as situacao_da_obra,
				CASE WHEN
					(
						SELECT  
							MAX( coalesce((SELECT 
										sum(( icopercsobreobra * supvlrinfsupervisor ) / 100)
									  FROM 
										obras.itenscomposicaoobra i
									  INNER JOIN 
										obras.supervisaoitenscomposicao si ON i.icoid = si.icoid WHERE si.supvid = s.supvid AND obrid = oi.obrid AND i.icovigente = 'A' ),'0') ) as percentual
						FROM
							obras.supervisao s
						INNER JOIN 
							obras.situacaoobra si ON si.stoid = s.stoid
						LEFT JOIN
							obras.realizacaosupervisao rs ON rs.rsuid = s.rsuid 
						WHERE
							s.obrid = oi.obrid AND
							s.supstatus = 'A'
					 ) > 100 THEN 100
				ELSE
					CASE WHEN
						(
							SELECT  
								MAX( coalesce((SELECT 
											sum(( icopercsobreobra * supvlrinfsupervisor ) / 100)
										  FROM 
											obras.itenscomposicaoobra i
										  INNER JOIN 
											obras.supervisaoitenscomposicao si ON i.icoid = si.icoid WHERE si.supvid = s.supvid AND obrid = oi.obrid AND i.icovigente = 'A' ),'0') ) as percentual
							FROM
								obras.supervisao s
							INNER JOIN 
								obras.situacaoobra si ON si.stoid = s.stoid
							LEFT JOIN
								obras.realizacaosupervisao rs ON rs.rsuid = s.rsuid 
							WHERE
								s.obrid = oi.obrid AND
								s.supstatus = 'A'
						 ) IS NULL THEN 0
					ELSE
						(
							SELECT  
								(MAX( coalesce((SELECT 
											sum(( icopercsobreobra * supvlrinfsupervisor ) / 100)
										  FROM 
											obras.itenscomposicaoobra i
										  INNER JOIN 
											obras.supervisaoitenscomposicao si ON i.icoid = si.icoid WHERE si.supvid = s.supvid AND obrid = oi.obrid AND i.icovigente = 'A' ),'0') ) ) as percentual
							FROM
								obras.supervisao s
							INNER JOIN 
								obras.situacaoobra si ON si.stoid = s.stoid
							LEFT JOIN
								obras.realizacaosupervisao rs ON rs.rsuid = s.rsuid 
							WHERE
								s.obrid = oi.obrid AND
								s.supstatus = 'A'
						 )
					END
				END as percentual
				
			FROM
				obras.itemgrupo ig 
			INNER JOIN 
				obras.repositorio r ON r.repid = ig.repid
			INNER JOIN
				obras.obrainfraestrutura oi ON oi.obrid = r.obrid
			INNER JOIN
				obras.orgao oo ON oo.orgid = oi.orgid
			INNER JOIN
				entidade.endereco ed ON ed.endid = oi.endid
			INNER JOIN
				territorios.municipio tm ON tm.muncod = ed.muncod
			INNER JOIN
				obras.situacaoobra so ON so.stoid = oi.stoid
			INNER JOIN
				entidade.entidade ee ON ee.entid = oi.entidunidade	
			LEFT JOIN 
				( SELECT max(frrid), obrid, covid FROM obras.formarepasserecursos GROUP BY obrid, covid ) of ON of.obrid = oi.obrid
			LEFT JOIN
				obras.conveniosobra oc ON oc.covid = of.covid
				
			WHERE 
				ig.gpdid = {$_REQUEST['gpdid']}
				AND r.repstatus = 'A'
			ORDER BY
			itgid";
	
	$titulo = "<table cellspacing='0' cellpadding='2' border='0' align='center' width='95%' class='listagem'>
					<tr>
						<td>
							<b>Tipo de Estabelecimento</b>
						</td>
						<td>
							<b>Unidade</b>
						</td>
						<td>
							<b>Nome da Obra</b> 
						</td>
						<td>
							<b>Município</b> 
						</td>
						<td>
							<b>Convênio</b> 	
						</td>
						<td>
							<b>Situação da Obra</b> 
						</td>
						<td>
							<b>% Executado</b>
						</td>
					</tr>";
	
	$dados = $db->carregar($sql);
	
	$i = 0;
	
	if($dados[0]) {
		
		foreach ($dados as $dado) {
			
			( $i%2 ? $campo="bgcolor=\"\" onmouseout=\"this.bgColor='';\" onmouseover=\"this.bgColor='#ffffcc';\"" : $campo="bgcolor=\"#f7f7f7\" onmouseout=\"this.bgColor='#F7F7F7';\" onmouseover=\"this.bgColor='#ffffcc';\"" );
			
			$titulo .= "<tr {$campo}>";
				$titulo .= "<td>";
				$titulo .= "{$dado['tipo_de_ensino']}";
				$titulo .= "</td>";
				$titulo .= "<td>";
				$titulo .= "&nbsp;{$dado['unidade']}";
				$titulo .= "</td>";
				$titulo .= "<td>";
				$titulo .= "{$dado['nome_da_obra']}";
				$titulo .= "</td>";
				$titulo .= "<td>";
				$titulo .= "{$dado['municipio']}";
				$titulo .= "</td>";
				$titulo .= "<td>";
				$titulo .= "{$dado['convenio']}";
				$titulo .= "</td>";
				$titulo .= "<td>";
				$titulo .= "{$dado['situacao_da_obra']}";
				$titulo .= "</td>";
				$titulo .= "<td>";
				$titulo .= number_format($dado['percentual'], 2, ',', '');
				$titulo .= "</td>";
			$titulo .= "</tr>";
			$i++;
		}
	
	}
	
	$titulo .= "</table>";
	
	echo $titulo;
	exit();
	
}

function obras_monta_coluna_relatorio3(){
	
	$arOrgid = $_POST['orgid'];
	
	$coluna = array();
	foreach ( $arOrgid as $orgid ){

		switch( $orgid ){
			
			// início sesu
			case ORGAO_SESU:
				array_push( $coluna, array("campo" 	  => "valortotalsup",
								   		   "label" 	  => "B=Valor Total da OS (Superior)") );

				array_push( $coluna, array("campo" 	  => "qtdsup",
								   		   "label" 	  => "C=Em Supervisão (Superior)",
										   "type"	  => "numeric") );
				
				array_push( $coluna, array("campo" 	  => "psusesu",
								   		   "label" 	  => "D=Obras Previstas (Superior)",
										   "type"	  => "numeric") );
				
				/*array_push( $coluna, array("campo" 	  => "sesudivisao",
								   		   "label" 	  => "E=C/D (Superior)") );*/
				
				array_push( $coluna, array("campo"    => "sesudivisao",
                                           "type"     => "numeric",
                                           "label"    => "E=C/D*100 (Superior)", 
                                           "php"      => array("expressao" => "is_numeric({qtdsup}) && {psusesu} > 0",
										   				"var"	=> "percent",
                                           				"true"  => "round( {qtdsup} / {psusesu} * 100, 2)",        
                                           				"false" => "0",
                                           				"type"  => "numeric",                        
                                           				"html"  => "{percent}%")
										   )
						   );
			break;
			// fim sesu
			// início setec
			case ORGAO_SETEC:
				array_push( $coluna, array("campo" 	  => "valortotalpro",
								   		   "label" 	  => "F=Valor Total da OS (Profissional)") );
				
				array_push( $coluna, array("campo" 	  => "qtdpro",
								   		   "label" 	  => "G=Em Supervisão (Profissional)",
										   "type"	  => "numeric") );
				
				array_push( $coluna, array("campo" 	  => "psusetec",
								   		   "label" 	  => "H=Obras Previstas (Profissional)",
										   "type"	  => "numeric") );
				
				/*array_push( $coluna, array("campo" 	  => "setecdivisao",
								   		   "label" 	  => "I=G/H (Profissional)") );*/
				
				array_push( $coluna, array("campo"    => "setecdivisao",
                                           "type"     => "numeric",
                                           "label"    => "I=G/H*100 (Profissional)", 
                                           "php"      => array("expressao" => "is_numeric({qtdpro}) && {psusetec} > 0",
										   				"var"	=> "percent",
                                           				"true"  => "round( {qtdpro} / {psusetec} * 100, 2)",        
                                           				"false" => "0",
                                           				"type"  => "numeric",                        
                                           				"html"  => "{percent}%")
										   )
						   );
				
			break;
			// fim setec
			// início fnde
			case ORGAO_FNDE:
				array_push( $coluna, array("campo" 	  => "valortotalbas",
								   		   "label" 	  => "J=Valor Total da OS (Básica)") );
				
				array_push( $coluna, array("campo" 	  => "qtdbas",
								   		   "label" 	  => "K=Em Supervisão (Básica)",
										   "type"	  => "numeric") );
				
				array_push( $coluna, array("campo" 	  => "psufnde",
								   		   "label" 	  => "L=Obras Previstas (Básica)",
										   "type"	  => "numeric") );
				
				/*array_push( $coluna, array("campo" 	  => "fndedivisao",
								   		   "label" 	  => "M=K/L (Básica)") );*/
				
				array_push( $coluna, array("campo"    => "fndedivisao",
                                           "type"     => "numeric",
                                           "label"    => "M=K/L*100 (Básica)", 
                                           "php"      => array("expressao" => "is_numeric({qtdbas}) && {psufnde} > 0",
										   				"var"	=> "percent",
                                           				"true"  => "round( {qtdbas} / {psufnde} * 100, 2)",        
                                           				"false" => "0",
                                           				"type"  => "numeric",                        
                                           				"html"  => "{percent}%")
										   )
						   );
				
			break;
			// fim fnde
		}
		
	}
	
	array_push( $coluna, array("campo" 	  => "deslocamento",
					   		   "label" 	  => "N = Deslocamento") );
	
	array_push( $coluna, array("campo" 	  => "valorDaOs",
					   		   "label" 	  => "O=Valor Da OS") );
	
	array_push( $coluna, array("campo" 	  => "totalObrasPrevistas",
					   		   "label" 	  => "P=Total Obras Previstas",
							   "type"	  => "numeric") );
	
	array_push( $coluna, array("campo" 	  => "totalObrasDaOs",
					   		   "label" 	  => "Q=Total Obras da OS",
							   "type"	  => "numeric") );
	
	/*array_push( $coluna, array("campo" 	  => "R",
					   		   "label" 	  => "R=Q/P",
							   "html"     => "{R}%"));*/
	
	array_push( $coluna, array("campo"    => "R",
							   "type"     => "numeric",
                               "label"    => "R = Q/P*100", 
                               "php"      => array("expressao" => "is_numeric({totalObrasDaOs}) && {totalObrasPrevistas} > 0",
													"var"	=> "percent",
                                           			"true"  => "round( {totalObrasDaOs} / {totalObrasPrevistas} * 100, 2)",        
                                           			"false" => "0",
                                           			"type"  => "numeric",                        
                                           			"html"  => "{percent}%")
							  )
			  );
	
	/*array_push( $coluna, array("campo" 	  => "S",
					   		   "label" 	  => "S=N/O",
							   "html"     => "{S}%") );*/
	
	array_push( $coluna, array("campo"    => "S",
							   "type"     => "numeric",
                               "label"    => "S = N/O*100", 
                               "php"      => array("expressao" => "is_numeric({deslocamento}) && {valorDaOs} > 0",
													"var"	=> "percent",
                                           			"true"  => "round( {deslocamento} / {valorDaOs} * 100, 2)",        
                                           			"false" => "0",
                                           			"type"  => "numeric",                        
                                           			"html"  => "{percent}%")
							  )
			  );

	return $coluna;
	
}

function obras_monta_agp_relatorio3(){
	
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array("valortotalsup", "qtdsup", "psusesu", "sesudivisao", 
										  "valortotalpro", "qtdpro", "psusetec", "setecdivisao", 
										  "valortotalbas", "qtdbas", "psufnde", "fndedivisao",
										  "deslocamento", "valorDaOs", "totalObrasPrevistas", "totalObrasDaOs",
										  "R", "S"	
										  )
				);
				
	array_push($agp['agrupador'], array(
										"campo" => "regdescricao",
								  		"label" => "Região")										
						   				);			
	array_push($agp['agrupador'], array(
										"campo" => "estuf",
								  		"label" => "UF")										
						   				);			
	array_push($agp['agrupador'], array(
										"campo" => "os",
								  		"label" => "Grupo/OS")										
						   				);			
						   				
	return $agp;
	
}

if( isset($_POST['relatorio']) ){

	include("resultado_relatorio_geral3.inc");
	
	exit();
}

// cabecalho padrão do sistema
include APPRAIZ . "includes/cabecalho.inc";
print "<br/>";
monta_titulo( "Relatório de Supervisão", "" );

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script type="text/javascript">
	$(document).ready(function() {
	
		$("#visualizar").click(function () {
			
            // validando data início e data fim
            var d = new Data();
			if( d.comparaData($("#dataini").val(), $("#datafim").val(), ">") ){
				alert("Preencha o Período Corretamente.");
				$("#dataini").val("");
				$("#datafim").val("");
				return false;
			}
			
			var ok = 0;
			// verificando se o usuário selecionou um tipo de ensino
			$("input[id^='orgid_']").each(function() {
				if(this.checked){
					ok++;
				}
            });
			
			if(ok==0){
				alert("Selecione algum Tipo de Estabelecimento.");
				return false;
			}else{
				selectAllOptions( formulario.estuf );
				return true;
			}
			
			
		})
	
	});
</script>

<form name="formulario" method="post" target="_blank">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td align='right' class="SubTituloDireita">Tipo de Estabelecimento:</td>
		<td>
		<?php
			
			$sql = "SELECT 
						orgid, 
						orgdesc  
					FROM 
						obras.orgao 
					WHERE 
						orgstatus = 'A'
						-- forçando o orgid para que não seja exibido Administrativo e Hospitais
						AND orgid IN ( ".ORGAO_SESU.", ".ORGAO_SETEC.", ".ORGAO_FNDE." )
					--ORDER BY orgdesc";
			
			$tipos = $db->carregar($sql);
			foreach ($tipos as $tipo) {
				echo "<label><input type='checkbox' id='orgid_{$tipo['orgid']}' name='orgid[]' value='{$tipo['orgid']}'>&nbsp;{$tipo['orgdesc']}</label>&nbsp;";
			}
		
		?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Período:</td>
		<td>
			<?php echo campo_data2( 'dataini','N', 'S', '', 'N' ); ?>
			&nbsp;até&nbsp;
			<?php echo campo_data2( 'datafim','N', 'S', '', 'N' ); ?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">UF:</td>
		<td>
		<?php
			
			// UF
			$sql = " SELECT
							estuf AS codigo,
							estdescricao AS descricao
						FROM 
							territorios.estado
						ORDER BY
							estdescricao ";
			$stSqlCarregados = "";
			combo_popup( "estuf", $sql, "UF", "400x400", 0, array(), "", "S", false, false, 5, 400 );
		
		?>
		</td>
	</tr>
	<tr>
		<td bgcolor="#cccccc"></td>
		<td bgcolor="#cccccc">
			<input type="submit" style="cursor: pointer;" id="visualizar" value="Visualizar">
		</td>
	</tr>
</table>
<input type="hidden" name="relatorio" value="gerar">
</form>
