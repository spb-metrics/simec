<?php

$supervisao = new supervisao();

switch( $_REQUEST["requisicao"] ){
	
	case "pesquisa":
		$filtros = $supervisao->obrFiltraListaEmpresas();
	break;
	
	case "excluir":
		
		if( $supervisao->obrVerficaDadoRequisicao( $_REQUEST["epcid"], "empresacontratada", "epcid" ) ){
			$supervisao->obrExcluiEmpresa( $_REQUEST["epcid"] );	
		}else{
			$supervisao->obrExibeMsgErro( "A empresa informa não existe ou já foi excluída!" );
		}
		
	break;
	
}

// cabecalho padrão do simec
include APPRAIZ . "includes/cabecalho.inc";

// Monta as abas
print "<br/>";
$db->cria_aba( $abacod_tela, $url, $parametros );
monta_titulo( "Empresas Contratadas", "" );


?>
<form action="" method="post" name="formulario" id="obrFormPesquisaEmpresa">
	<input type="hidden" name="requisicao" id="requisicao" value="pesquisa"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td class="SubTituloCentro" colspan="2">Argumentos de Pesquisa</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="190px">CNPJ:</td>
			<td>
				<?php 
					$entnumcpfcnpj = formatar_cnpj( $_REQUEST["entnumcpfcnpj"] );
					print campo_texto( "entnumcpfcnpj", "N", "S", "", 21, 20, "##.###.###/####-##", "", "left", "", 0, "entnumcpfcnpj");
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Empresa:</td>
			<td>
				<?php 
				
					$epcid = $_REQUEST["epcid"];
					
					$sql = "SELECT DISTINCT
								epcid as codigo,
								entnome as descricao
							FROM
								entidade.entidade ee
							INNER JOIN
								obras.empresacontratada ec ON ee.entid = ec.entid
							WHERE
								entstatus = 'A'
							ORDER BY
								entnome";
	
					$db->monta_combo("epcid", $sql, "S", "Selecione...", '', '', '', '', 'N','epcid');
						
				?>
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td></td>
			<td>
				<input type="button" value="Pesquisar" onclick="document.getElementById('obrFormPesquisaEmpresa').submit();" style="cursor: pointer;"/>
			</td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2">Lista de Empresas</td>
		</tr>
	</table>
</form>

<?php $supervisao->obrMontaListaEmpresas( $filtros ); ?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
	<tr bgcolor="#D0D0D0">
		<td>
			<input type="button" value="Inserir Empresa" onclick="location.href='?modulo=principal/supervisao/inserirEmpresaContratada&acao=A&requisicao=novo';" style="cursor: pointer;"/>
		</td>
	</tr>
</table>
