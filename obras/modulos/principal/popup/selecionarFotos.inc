<?php

/**
 * Arquivo que seleciona as fotos da galeria para o Extrato da obra
 * 
 * @author Eduardo Dunice Neto
 * @since 21/06/2010
 * 
 */

$supvid = $_REQUEST['supvid'];

?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">

	<?php 
	
		$sql = "SELECT 
					f.arqid,
					fotdsc,
					to_char(a.arqdata,'dd/mm/yyyy') as aqodtinclusao
				FROM obras.supervisao s
				INNER JOIN obras.fotos    f ON f.supvid = s.supvid
				INNER JOIN public.arquivo a ON a.arqid  = f.arqid
				WHERE s.supvid = ".$supvid;
	
		$fotosVistorias = ($db->carregar($sql));
		
		if ( $fotosVistorias ){
			
			print "<tr>";
			
			$arqids = "{}";
			
			$j = 1;
			for( $i = 0; $i < count($fotosVistorias); $i++ ){
				
				print "    <td align='center'>"
					. "        <img src='../slideshow/slideshow/verimagem.php?_sisarquivo=obras&newwidth=100&newheight=100&arqid={$fotosVistorias[$i]["arqid"]}' hspace='3' vspace='3' style='width:50px; height:50px; '\n>"
					. "        <br/>{$fotosVistorias[$i]["aqodtinclusao"]}"
					. "        <br/><input type='checkbox' name='fotos[]' id='{$fotosVistorias[$i]["arqid"]}' onBlur='salvaFotosSelecionadas(this, {$fotosVistorias[$i]["arqid"]})'  onclick='salvaFotosSelecionadas(this, {$fotosVistorias[$i]["arqid"]})' />"
					. "    </td>";
				
				if( is_int($j/5) ){
					print "</tr><tr>";
				}
				$arqids .= ",".$fotosVistorias[$i]["arqid"]."";
				$j++;
			}
			
			$arqids = "[".str_replace("{},", "", $arqids)."]";
			
			print "    <td align='center'>"
				. "        <a onclick=\"selecionarFotos({$arqids});\">Selecinar Tudo</a>"
				. "    </td>";
			
			print "</tr>";
			 
		}else{
			
			print "<tr><td align='center' style='color:#ee0000'> Não existem fotos cadastradas para esta obra. </td></tr>";
			
		}
		
	?>
	</table>
	<script type="text/javascript">

	function selecionarFotos( arqids ) {

		for( i=0; i<arqids.length; i++ ){
			if( document.getElementById(arqids[i]).checked ){
				document.getElementById(arqids[i]).checked = false;
			}else{
				document.getElementById(arqids[i]).checked = true;
			}
		}
		
	}

	
	</script>
