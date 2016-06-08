<?php

switch( $_REQUEST["tipo"] ){
	
	case "obra":
		
		$sql = "SELECT
					ee.entnome as unidade,
					ee2.entnome as campus,
					obrdesc as obra,
					obrdtinicio as inicio,
  					obrdttermino as termino, 
					obrpercexec as percentual,
					obrqtdconstruida as area,
					umdeesc as unidademedida
				FROM
					obras.obrainfraestrutura oi
				INNER JOIN
					entidade.entidade ee ON ee.entid = oi.entidunidade
				LEFT JOIN
					entidade.entidade ee2 ON ee2.entid = oi.entidcampus
				INNER JOIN
					obras.unidademedida ou ON ou.umdid = oi.umdidobraconstruida
				WHERE
					obrid = {$_REQUEST["id"]}";
		
		$dados = $db->pegaLinha($sql);
		
		// endereço
		$sql = "SELECT
					endcep,
					endlog,
					endcom,
					endnum,
					endbai,
					mundescricao,
					ed.estuf
				FROM
					entidade.endereco ed
				INNER JOIN
					territorios.municipio tm ON tm.muncod = ed.muncod
				INNER JOIN
					obras.obrainfraestrutura oi ON oi.endid = ed.endid
				WHERE
					obrid = {$_REQUEST["id"]}";
		
		$endereco = $db->pegaLinha($sql);
		
	break;
}

?>

<html>
	<head>
		<title><?php echo $GLOBALS['parametros_sistema_tela']['nome_e_orgao']; ?></title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script src="/includes/prototype.js"></script>
		<script src="../includes/calendario.js"></script>
		<script src="../obras/js/obras.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
		<table class="tabela" bgcolor="#ffffff" cellSpacing="1" cellPadding=3 align="center">
			<tr>
				<td class="SubTituloCentro" colspan="2"><?php print "Dados da " . ucfirst($_REQUEST["tipo"]); ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">ID:</td>
				<td><b><?php print $_REQUEST["id"]; ?></b></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Unidade ResponsÃ¡vel pela Obra:</td>
				<td><?php print $dados["unidade"]; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Campus:</td>
				<td><?php print !empty( $dados["campus"] ) ? $dados["campus"] : "Não Informado" ; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Nome da Obra:</td>
				<td><?php print $dados["obra"]; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Área Construída:</td>
				<td><?php print number_format( $dados["area"], 2, ",", "." ) . " " . $dados["unidademedida"]; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Data de Início:</td>
				<td><?php print !empty( $dados["inicio"] ) ? formata_data( $dados["inicio"] ) : "Não Informado"; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Data de Término:</td>
				<td><?php print !empty( $dados["termino"] ) ? formata_data( $dados["termino"] ) : "Não Informado"; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">% Executado:</td>
				<td><?php print number_format( $dados["percentual"], 2, ",", "." ); ?></td>
			</tr>
			<tr>
				<td class="SubTituloCentro" colspan="2">Contatos da Obra</td>
			</tr>
			<tr>
				<td colspan="2" align="center">
					<?php 
					
						$sql = "SELECT
									et.entnumcpfcnpj as cpf,
									et.entnome as nome,
									et.entemail as email,
									'(' || et.entnumdddcomercial || ') ' || et.entnumcomercial as telefone,
									tr.tprcdesc as tipo_desc
								FROM 
									obras.responsavelobra r 
								INNER JOIN 
									obras.responsavelcontatos rc ON r.recoid = rc.recoid 
								INNER JOIN 
									entidade.entidade et ON rc.entid = et.entid 
								LEFT JOIN 
									obras.tiporespcontato tr ON rc.tprcid = tr.tprcid
								WHERE 
									r.obrid = '{$_REQUEST["id"]}'  AND 
									rc.recostatus = 'A'";
					
						$cabecalho = array( "CPF", "Nome", "E-mail", "Telefone", "Tipo de Responsabilidade" );
		
						$db->monta_lista( $sql, $cabecalho, 100, 10, "N", "center", "" );
						
						
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloCentro" colspan="2">Endereço</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">CEP:</td>
				<td><?php print formata_cep($endereco["endcep"]); ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Logradouro:</td>
				<td><?php print $endereco["endlog"]; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Número:</td>
				<td><?php print !empty( $dados["endnum"] ) ? $dados["endnum"] : "Não Informado" ; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Complemento:</td>
				<td><?php print !empty( $dados["endcom"] ) ? $dados["endcom"] : "Não Informado" ; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Bairro:</td>
				<td><?php print $endereco["endbai"]; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Município / UF:</td>
				<td><?php print $endereco["mundescricao"] . ' / ' . $endereco["estuf"]; ?></td>
			</tr>
			<tr>
				<td class="SubTituloCentro" colspan="2">Fotos</td>
			</tr>
			<tr>
				<td colspan="2" align="center">
					
					<?php 
					
						$sql = "SELECT 
									arqnome, 
									arq.arqid, 
									arq.arqextensao, 
									arq.arqtipo, 
									arq.arqdescricao 
								FROM 
									public.arquivo arq
								INNER JOIN 
									obras.arquivosobra oar ON arq.arqid = oar.arqid
								INNER JOIN 
									obras.obrainfraestrutura obr ON obr.obrid = oar.obrid 
								INNER JOIN 
									seguranca.usuario seg ON seg.usucpf = oar.usucpf 
								WHERE 
									obr.obrid = {$_REQUEST["id"]} AND
									aqostatus = 'A' AND
									(arqtipo = 'image/jpeg' OR 
									 arqtipo = 'image/gif' OR 
									 arqtipo = 'image/png') 
								ORDER BY 
									arq.arqid LIMIT 5";
						
						
						$fotos = ($db->carregar($sql));
						
						if( !$fotos ){
							
							print "<table class='tabela' cellSpacing='1' cellPadding='3' align='center'>";
							
							for( $i = 0; $i < count( $fotos ); $i++ ){
				
								if( $fotos[$i]["arqid"] ){
									
									print "<tr>"
										. "    <td align='center'>"
										. "        <img src='../slideshow/slideshow/verimagem.php?newwidth=225&newheight=225&arqid={$fotos[$i]["arqid"]}' 
										 		   hspace='3' vspace='3' style='width:100px; height:100px;' /> <br>{$fotos[$i]["arqdescricao"]}"
										. "    </td>";
												 
								}
								
								$i = $i+1;
								
								if( $fotos[$i]["arqid"] ){
									
									print "    <td align='center'>"
										. "       <img src='../slideshow/slideshow/verimagem.php?newwidth=225&newheight=225&arqid={$fotos[$i]["arqid"]}' 
										 		  hspace='3' vspace='3' style='width:100px; height:100px;' /> <br>{$fotos[$i]["arqdescricao"]}"
										. "    </td>";
												 
								}
								
								$i = $i+1;
								
								if( $fotos[$i]["arqid"] ){
									
									print "    <td align='center'>"
										. "        <img src='../slideshow/slideshow/verimagem.php?newwidth=225&newheight=225&arqid={$fotos[$i]["arqid"]}' 
													hspace='3' vspace='3' style='width:100px; height:100px;' /> <br>{$fotos[$i]["arqdescricao"]}"
										. "    </td>"
										. "</tr>";
												 
								}
								
							}
							
							print "</table>";
						
						}else{
							
							print "<span style='text-align:center;color:#dd0000;'>Não existem fotos cadastradas para a obra.</span>";
							
						}
					
					?>
					
				</td>
			</tr>
			<tr bgcolor="#D0D0D0">
				<td colspan="2">
					<input type="button" value="Fechar" style="cursor: pointer;" onclick="self.close();"/>
				</td>
			</tr>
		</table>
	</body>
</html>
