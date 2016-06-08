<?php

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php'; 
require_once APPRAIZ . "www/obras/permissoes.php";

print '<br/>';

$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo = "Obra Vinculada";
monta_titulo( $titulo_modulo, "");

$obras  = new Obras();
$dobras = new DadosObra(null);

echo $obras->CabecalhoObras();

$sql 		 = "SELECT obridorigem FROM obras.obrainfraestrutura WHERE obrid= {$_SESSION['obra']['obrid']}";
$obridorigem = $db->pegaUm($sql);

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td>
			<?php
				if ( $obridorigem ){
					
					if($somenteLeitura == "N"){
						$botoes .= "<img src=\"/imagens/alterar_01.gif\" style=\"cursor: pointer\"  border=0 title=\"Editar\">";
					}else{
						$botoes  = "<img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\" visualizarObraOrigem(' || oi.obrid || ', \'vinculada\'); \"/>";
					}

					$sql = "SELECT
								'<center>{$botoes}</center>' as acao,
								oi.obrdesc as nome_obra,
								ee.entnome as unidade,
								mun.mundescricao || '/' || ed.estuf as municipio,
						        to_char(oi.obrdtinicio,'DD/MM/YYYY') as inicio,
								to_char(oi.obrdttermino,'DD/MM/YYYY') as final,
								CASE WHEN oi.obrdtvistoria is not null THEN to_char(oi.obrdtvistoria,'DD/MM/YYYY') ELSE to_char(oi.obsdtinclusao,'DD/MM/YYYY') END as ultimadata,
								(select replace(coalesce(round(SUM(icopercexecutado), 2), '0') || ' %', '.', ',') as total from obras.itenscomposicaoobra WHERE obrid = oi.obrid) as percentual
							FROM
								obras.obrainfraestrutura oi
							INNER JOIN
								obras.orgao oo ON oi.orgid = oo.orgid
							INNER JOIN
								entidade.entidade ee ON ee.entid = oi.entidunidade
							INNER JOIN 
									entidade.endereco ed ON ed.endid = oi.endid
							LEFT JOIN 
								territorios.municipio mun ON ed.muncod = mun.muncod
							WHERE
								obrid = {$obridorigem}";
					
					$cabecalho = array( "Ação", "Nome da Obra", "Unidade", "Município/UF", "Data de Início", "Data de Término", "Última Atualização", "(%) Executado" );
					$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%');
				
				}else{
					echo '<center><span style="color:#ee0000;">Haverá obra vinculada quando em algum momento a obra estiver na situação <b>Contrato Cancelado</b>.</span></center>';
				}
			?>			
		</td>
	</tr>
	<tr bgcolor="#C0C0C0">
			<td>
				<div style="float: left;">
					<input type="button" value="Voltar" style="cursor: pointer" onclick="history.back(-1);">
				</div>
			</td>
		</tr>
</table>
<?php chkSituacaoObra(); ?>