<?php

/**
 * Arquivo que seleciona as fotos da galeria para o Extrato da obra
 * 
 * @author Fernando Araújo Bagno da SIlva
 * @since 11/02/2010
 * 
 */


monta_titulo( "Galeria de Fotos", "");

?>
<html>
	<head>
		<title><?php echo $GLOBALS['parametros_sistema_tela']['nome_e_orgao'];?></title>
		<script type="text/javascript">

			function salvaFotosSelecionadas(){

				var objForm = document.getElementsByName("fotos[]"); 
				var ids   = 0;
				var total = 0;
				
			    for ( var i = 0; i < objForm.length; i++ ){

			    	if (objForm[i].checked){
			    		ids += "," + objForm[i].id;
			    		total += 1;
			    	}
		    		
				}

			    window.opener.document.getElementById("numfotos").value = total;
				window.opener.document.getElementById("fotoselecionada").value = ids;

				self.close();
				
			}
			
		</script>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script type="text/javascript" src="../includes/prototype.js"></script>
	    <script type="text/javascript" src="../includes/entidades.js"></script>
	    <script type="text/javascript" src="/includes/estouvivo.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
	</head>
	<body>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">

	<?php 
	
		$sql = "SELECT 
					arq.arqid, 
					arq.arqdescricao 
				FROM 
					public.arquivo arq
				INNER JOIN 
					obras.arquivosobra oar ON arq.arqid = oar.arqid
				INNER JOIN 
					obras.obrainfraestrutura obr ON obr.obrid = oar.obrid 
				WHERE 
					obr.obrid = {$_SESSION["obra"]["obrid"]} AND
					aqostatus = 'A' AND
					(arqtipo = 'image/jpeg' OR 
					 arqtipo = 'image/gif' OR 
					 arqtipo = 'image/png') 
				ORDER BY 
					arq.arqid";
		
		$fotosObras = ($db->carregar($sql));
		
		if ( $fotosObras ){
			
			print "<tr>"
				. "    <td class='subtitulocentro'>Selecionar</td>"
				. "    <td class='subtitulocentro'>Foto</td>"
				. "<tr>";
			
			for( $i = 0; $i < count($fotosObras); $i++ ){
				
				$cor = ( $i % 2 ) ? "#f4f4f4" : "#E0E0E0";
				
				print "<tr bgcolor='{$cor}'>"
					. "    <td align='center'>"
					. "        <input type='checkbox' name='fotos[]' id='{$fotosObras[$i]["arqid"]}'/>"
					. "    </td>"
					. "    <td>"
					. "        <img src='../slideshow/slideshow/verimagem.php?newwidth=100&newheight=100&arqid={$fotosObras[$i]["arqid"]}' hspace='3' vspace='3' style='width:100px; height:100px; '\n>"
					. "        <br/>{$fotosObras[$i]["arqdescricao"]}"
					. "    </td>"
					. "<tr>";
				
			}
			 
		}else{
			
			print "<tr><td align='center' style='color:#ee0000'> Não existem fotos cadastradas para esta obra. </td></tr>";
			
		}
		
	?>
		
		<tr bgcolor="#D0D0D0">
			<td colspan="2">
				<input type="button" value="OK" onclick="salvaFotosSelecionadas();" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
	</body>
</html>
