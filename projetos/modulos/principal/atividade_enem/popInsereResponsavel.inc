<?

// controla o cache do navegador
header( "Cache-Control: no-store, no-cache, must-revalidate" );
header( "Cache-Control: post-check=0, pre-check=0", false );
header( "Cache-control: private, no-cache" );   
header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
header( "Pragma: no-cache" );

include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

?>

<html>
	<head>
		<title>SIMEC - Sistema Integrado de Monitoramento Execução e Controle do Ministério da Educação, Permite o Monitoramento Físico e Financeiro e a Avaliação das Ações e Programas do Ministério dentre outras atividades estratégicas</title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script type="text/javascript" src="/includes/JQuery/jquery.js"></script>
		<script type="text/javascript" src="/includes/JQuery/interface.js"></script>
	</head>
	<body>
		<table border="0" cellspacing="3" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none; width:100%;">
			<tr>
				<td width="100%" align="center">
					<label class="TituloTela" style="color: #000000;"> 
						Incluir Responsável em 
						<? 
						if($_REQUEST["tipo"]=="E") echo "Execução"; 
						elseif($_REQUEST["tipo"]=="V") echo "Validação";
						else echo "Certificação";
						?>
					</label>
				</td>
			</tr>
		</table>
		<table width="100%" align="center" border="0" cellspacing="3" cellpadding="3" class="listagem">
			<?php
				$innerFunid = "";
				if( $_REQUEST["tipo"] == "E" )
				{
					$innerFunid = " AND fun.funid = 91 ";
				}
				if( $_REQUEST["tipo"] == "V" )
				{
					$innerFunid = " AND fun.funid = 92 ";
				}
				if( $_REQUEST["tipo"] == "C" )
				{
					$innerFunid = " AND fun.funid = 93 ";
				}
				
				$sql = "SELECT
							ent.entid,
							ent.entnome
						FROM
							entidade.entidade ent
						--INNER JOIN
							--entidade.funcaoentidade fue ON fue.entid = ent.entid
													   --AND fue.fuestatus = 'A'
						--INNER JOIN
							--entidade.funcao fun ON fun.funid = fue.funid
												--{$innerFunid}
											   --AND fun.funtipo = 'F'
											   --AND fun.funstatus = 'A'
						INNER JOIN
							projetos.responsavelatividade rpa ON rpa.entid = ent.entid
														AND rpa.atiid = {$_REQUEST['atiid']}
														AND rpa.rpastatus = 'A'
						WHERE
							ent.entstatus = 'A'
						ORDER BY
							ent.entnome ASC";
				$dados = $db->carregar($sql);
				
				if( $dados )
				{
					for($i=0; $i<count($dados); $i++)
					{
						$cor = ($i%2) ? '#e0e0e0' : '#f4f4f4';
						
						echo '<tr bgcolor="'.$cor.'">
								<td align="center">
									<input type="checkbox" name="entid[]" value="'.$dados[$i]["entid"].'" onclick="insereResponsavel(this, \''.$_REQUEST["tipo"].'\', '.$dados[$i]["entid"].', \''.$dados[$i]["entnome"].'\');" />
								</td>
								<td>
									'.$dados[$i]["entnome"].'
								</td>
							</tr>';
					}
				}
				else
				{
					echo '<tr bgcolor="#f4f4f4">
							<td align="center" colspan="2" style="color:red">
								Nenhum registro encontrado.
							</td>
						</tr>';
				}
			?>
			<tr bgcolor="#c0c0c0">
				<td colspan="2" align="center">
					<input type="button" id="bt_fechar" value="Fechar Janela" onclick="self.close();">
				</td>
			</tr>
		</table>
	</body>
</html>

<script>
<!--

$(document).ready(function()
{
	if( '<?php echo $_REQUEST["tipo"]; ?>' == 'E' )
		var name 	= "executadopor[]";
	if( '<?php echo $_REQUEST["tipo"]; ?>' == 'V' )
		var name 	= "validadopor[]";
	if( '<?php echo $_REQUEST["tipo"]; ?>' == 'C' )
		var name 	= "certificadopor[]";

	var inputPai 	= window.opener.document.getElementsByName(name);
	var inputFilho	= document.getElementsByName('entid[]');

	for(var i=0; i<inputFilho.length; i++)
	{
		for(var j=0; j<inputPai.length; j++)
		{
			if( inputFilho[i].value == inputPai[j].value )
			{
				inputFilho[i].checked = true;
			}
		}
	}
});

function insereResponsavel(obj, tipo, entid, entnome)
{
	if( tipo == 'E' )
	{
		var tabela 	= window.opener.document.getElementById("tbExecucao");
		var name 	= "executadopor[]";
		var id 		= "executado_"+entid;
	}
	if( tipo == 'V' )
	{
		var tabela 	= window.opener.document.getElementById("tbValidacao");
		var name 	= "validadopor[]";
		var id 		= "validado_"+entid;
	}
	if( tipo == 'C' )
	{
		var tabela 	= window.opener.document.getElementById("tbCertificacao");
		var name 	= "certificadopor[]";
		var id 		= "certificado_"+entid;
	}
	
	if( obj.checked )
	{
		var tamanho = tabela.rows.length;
		
		if(tamanho == 2)
		{
			var linha = tabela.insertRow(tamanho-1);
			linha.style.backgroundColor = "#f4f4f4";
		} 
		else
		{
			var linha = tabela.insertRow(tamanho-1);
			
			if(tabela.rows[tamanho-2].style.backgroundColor == "rgb(224, 224, 224)")
			{
				linha.style.backgroundColor = "#f4f4f4";					
			}
			else
			{
				linha.style.backgroundColor = "#e0e0e0";					
			}
		}
		var coluna = linha.insertCell(0);
		coluna.style.textAlign = "left";
		
		coluna.innerHTML = '<input type="hidden" id="'+id+'" name="'+name+'" value="'+entid+'" />' + entnome;
	}
	else
	{
		if( tipo == 'E' )
			var hidden = window.opener.document.getElementById("executado_"+entid);
		if( tipo == 'V' )
			var hidden = window.opener.document.getElementById("validado_"+entid);
		if( tipo == 'C' )
			var hidden = window.opener.document.getElementById("certificado_"+entid);

		tabela.deleteRow( hidden.parentNode.parentNode.rowIndex );
	}
}

-->
</script>