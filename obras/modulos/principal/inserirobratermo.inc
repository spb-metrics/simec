<?php

// controle o cache do navegador
header( "Cache-Control: no-store, no-cache, must-revalidate" );
header( "Cache-Control: post-check=0, pre-check=0", false );
header( "Cache-control: private, no-cache" );   
header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
header( "Pragma: no-cache" );

// cria o objeto da classe termoDeAjuste
$termodeajuste = new termoDeAjuste();

// cria a variável com o ID do termo, caso exista
$traid = !empty($_SESSION['obra']['traid']) ? $_SESSION["obra"]["traid"] : '';

// insere as obras no termo
if ( $_REQUEST["requisicao"] == "cadastra" ){
	$termodeajuste->CadastraObrasTermo( $_REQUEST );
}

if ( $_REQUEST["subacao"] == "listaobras" ){
	
	if ( $_REQUEST["entid"] != null ){
		
		$sql = "SELECT
					CASE WHEN ot.otaid is not null THEN
						 '<center> <input type=\"checkbox\" name=\"sel[' || oi.obrid || ']\" id=\"sel\" value=\"' || oi.obrid || '\" checked=\"checked\" /> </center>' 
						 ELSE
						 '<center> <input type=\"checkbox\" name=\"sel[' || oi.obrid || ']\" id=\"sel\" value=\"' || oi.obrid || '\"/> </center>' END as acao,
					CASE WHEN entidcampus is not null THEN entnome ELSE 'Não Informado' END as campus,
					obrdesc as nome
				FROM
					obras.obrainfraestrutura oi
				LEFT JOIN
					entidade.entidade ee ON ee.entid = oi.entidcampus
				LEFT JOIN
					obras.obratermoajuste ot ON ot.obrid = oi.obrid AND ot.traid = {$_SESSION["obra"]["traid"]}
				WHERE
					obsstatus = 'A' AND entidunidade = {$_REQUEST["entid"]} 
				ORDER BY
					entnome, obrdesc";
		
		$cabecalho = array( "Ação", "Campus", "Nome da Obra" );
		$db->monta_lista( $sql, $cabecalho, 100, 30, 'N', 'center', '' );	
		
	}

	die;
	
}

monta_titulo( 'Inserir Obra(s)', 'Utilize os filtros para selecionar a(s) obra(s) desejada(s)' );

?>

<html>
	<head>
		<title><?php echo $GLOBALS['parametros_sistema_tela']['nome_e_orgao']; ?></title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script type="text/javascript" src="../includes/prototype.js"></script>
	    <script type="text/javascript" src="../includes/entidades.js"></script>
	    <script type="text/javascript" src="/includes/estouvivo.js"></script>
	    <script src="/obras/js/obras.js"></script>
	    <script>
	    	function listaObrasTermo( entid ){
    			
    			var url = '?modulo=principal/inserirobratermo&acao=A&subacao=listaobras&entid=' + entid;
	
				var myAjax = new Ajax.Updater(
					"listaobras",
					url,
					{
						method: 'post',
						asynchronous: false
					});
		    
	    	}
	    </script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
		<form action="" method="post" name="formulario" id="formulario">
			<input type="hidden" id="requisicao" name="requisicao" value="cadastra"/>
			<input type="hidden" id="traid" name="traid" value="<?php echo $traid; ?>"/>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
				<tr>
					<td class="subtitulodireita">Unidade</td>
					<td>
						<?php
							$sql = "SELECT DISTINCT
										entid as codigo, 
										entnome as descricao
									FROM 
										entidade.entidade ee
									INNER JOIN
										obras.obrainfraestrutura oi ON oi.entidunidade = ee.entid
									WHERE
										obsstatus = 'A' AND orgid = {$_SESSION['obra']['traid_orgid']}
									ORDER BY
										descricao, codigo";
							
							$db->monta_combo("entid", $sql, 'S', "Selecione...", 'listaObrasTermo(this.value);', '', '', '350', 'N', 'entid');
							
						?>
					</td>
				</tr>
			</table>
			<div id="listaobras">
			</div>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
				<tr bgcolor="#DCDCDC">
					<td>
						<input type="button" value="Salvar" style="cursor: pointer;" onclick="document.getElementById('formulario').submit();"/>
						<input type="button" value="Fechar" style="cursor: pointer;" onclick="self.close();"/>
					</td>
				</tr>
			</table>
		</form>
	</body>
</html>
