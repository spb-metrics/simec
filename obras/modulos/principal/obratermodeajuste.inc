<?php

/**
 * Tela com a finalidade de controlar os termos de ajuste das obras.
 * 
 * @author Fernando Araújo Bagno da Silva 
 * @since 19/08/2009
 * @version 1.0
 */

ini_set("memory_limit", "1024M");

// cria o objeto da classe termoDeAjuste
$termodeajuste = new termoDeAjuste();

$termodeajuste->verificaSessao();

// cria a variável com o ID do termo, caso exista
$traid = !empty($_SESSION['obra']['traid']) ? $_SESSION["obra"]["traid"] : '';

switch( $_REQUEST["requisicao"] ){
	case "atualiza" :
		$termodeajuste->AtualizaObrasTermo( $_REQUEST );
	break;
	case "exclui" :
		$termodeajuste->ExcluiObrasTermo( $_REQUEST["otaid"] );
	break;
}

if ( $_REQUEST["carga"] ){
	
	$combo_st = $termodeajuste->PegaSituacaoTermo( $_REQUEST["carga"] );
	
	$sql = "SELECT
				'<center>
					<img src=\"/imagens/exclui_p.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"excluiObraTermo(' || ot.otaid || ');\">
				 </center>' as acao,
				ee.entnome as campus,
				'<a style=\"cursor:pointer;\" onclick=\" visualizarObraOrigem(' || oi.obrid || ', \'termo\'); \">' || oi.obrdesc || '</a>' as nome,
				CASE WHEN oi.obrdtinicio is not null THEN to_char(oi.obrdtinicio,'DD/MM/YYYY') ELSE 'Não Informado' END as inicio,
		        CASE WHEN oi.obrdttermino is not null THEN to_char(oi.obrdttermino,'DD/MM/YYYY') ELSE 'Não Informado' END as final,
		        CASE WHEN oi.stoid is not null THEN sto.stodesc ELSE 'Não Informado' END as situacao,
		        CASE WHEN otadtprazo is not null THEN
			        '<center>
			        	<input type=\"text\" name=\"otadtprazo_' || ot.otaid || '\" id=\"otadtprazo\" size=\"12\" 
			        	maxlenght=\"10\" class=\"normal\" onKeyUp=\"this.value=mascaraglobal(\'##/##/####\',this.value);\"
			        	value=\" ' || to_char( otadtprazo, 'DD/MM/YYYY' ) || ' \"/>
			        	<a href=\"javascript:show_calendar(\'formulario.otadtprazo_' || ot.otaid || '\');\">
							<img src=\"../imagens/calendario.gif\" width=\"16\" height=\"15\" border=\"0\" align=\"absmiddle\" alt=\"\">
						</a>
			         </center>'
			         ELSE
			         '<center>
			        	<input type=\"text\" name=\"otadtprazo_' || ot.otaid || '\" size=\"12\" 
			        	maxlenght=\"10\" class=\"normal\" onKeyUp=\"this.value=mascaraglobal(\'##/##/####\',this.value);\"
			        	value=\"\"/>
			        	<a href=\"javascript:show_calendar(\'formulario.otadtprazo_' || ot.otaid || '\');\">
							<img src=\"/imagens/calendario.gif\" width=\"16\" height=\"15\" border=\"0\" align=\"absmiddle\" alt=\"\">
						</a>
			         </center>' END as prazo,
			    CASE WHEN st.staid is not null THEN
				    '<center>
			        	<select class=\"CampoEstilo\" name=\"staid[' || ot.otaid || ']\" style=\"width:100px; \">' || replace( '{$combo_st}', 'value=\''||st.staid||'\'', 'value=\''||st.staid||'\' selected ') || '</select>
			         </center>'
			         ELSE
			         '<center>
			        	<select class=\"CampoEstilo\" name=\"staid[' || ot.otaid || ']\" style=\"width:100px; \">{$combo_st}</select>
			         </center>' END as situacaotermo,
		        '<center>
		        	<img src=\"/imagens/lapis.png\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"insereObsTermoObra( ' || ot.otaid || ', {$_REQUEST["carga"]} );\">
		         </center>' as obs
			FROM
				obras.obrainfraestrutura oi
			INNER JOIN
				obras.obratermoajuste ot ON ot.obrid = oi.obrid
			LEFT JOIN
				obras.situacaoobra sto ON sto.stoid = oi.stoid
			INNER JOIN
				entidade.entidade ee ON ee.entid = oi.entidcampus
			LEFT JOIN
				obras.situacaotermoajuste st ON st.staid = ot.staid
			WHERE
				oi.entidunidade = {$_REQUEST["carga"]} AND
				ot.traid = {$_SESSION["obra"]["traid"]}
			ORDER BY
				campus, nome, situacao";
	
	$cabecalho = array( "Ação", "Campus", "Nome da Obra", "Data de Início", "Data de Término", "Situação da Obra", "Prazo", "Situação", "Obs" );
	$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%');
	
	switch ( $_REQUEST['subAcao'] ) {
		case 'retirarCarga' : unset( $_SESSION['obrasTermo']['carga'] ); die; break; 
		case 'gravarCarga'  : $_SESSION['obrasTermo']['carga'] = $_REQUEST['carga']; break; 
	}
	
	die;
	
}

// cabeçalho padrão do sistema
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

// cria o título e as abas da tela
echo '<br/>';
$db->cria_aba( $abacod_tela, $url, $parametros );
monta_titulo( 'Obra(s) - Encaminhamento(s)', '' );

$termodeajuste->cabecalho( $traid );

?>

<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/calendario.js"></script>
<form action="" method="post" name="formulario" id="formulario">
	<input type="hidden" id="requisicao" name="requisicao" value="atualiza"/>
	<input type="hidden" id="traid" name="traid" value="<?php echo $traid; ?>"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td>
				<?php
				
					$sql = "SELECT DISTINCT
								'<center>
									<img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" 
									 border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' || ee.entid || '\" 
									 name=\"+\" onclick=\"desabilitarConteudo( ' || ee.entid || ' ); abreconteudo(\'obras.php?modulo=principal/obratermodeajuste&acao=A&subAcao=gravarCarga&carga=' || ee.entid || '&params=\' + params, ' || ee.entid || ');\"/>
									 </center>' as acao,
								upper(entnome) as unidade,
								orgdesc as tipoensino,
								'<tr>
									<td style=\"padding:0px;margin:0;\"></td><td id=\"td' || ee.entid || '\" colspan=\"2\" 
									 style=\"padding:0px;display:none;border: 5px red\"></td><td style=\"padding:0px;margin:0;\">
									</td>
								 </tr>' as tr
							FROM
								entidade.entidade ee
							INNER JOIN
								obras.obrainfraestrutura oi ON oi.entidunidade = ee.entid
							INNER JOIN
								obras.obratermoajuste ot ON ot.obrid = oi.obrid
							INNER JOIN
								obras.orgao oo ON oo.orgid = oi.orgid
							WHERE
								ot.traid = {$traid}
							ORDER BY
								unidade, acao";
				
					$cabecalho = array( "Ação", "Unidade ResponsÃ¡vel pela Obra", "Tipo de Ensino", "" );
					$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%');
					
				?>
			</td>
		</tr>
		<tr>
			<td>
				<img src="/imagens/incluir_p.gif" style="padding-right: 1px; cursor: pointer; width: 12px;" 
				 align="absmiddle" vspace="1" border="0" onclick="insereObra();" title="Inserir Obras">
				<a style="cursor: pointer;" onclick="insereObra();">Inserir Obras</a>
			</td>
		</tr>
		<tr bgcolor="#DCDCDC">
			<td>
				<input type="button" value="Salvar" style="cursor: pointer;" onclick="document.getElementById('formulario').submit();"/>
				<input type="button" value="Voltar" style="cursor: pointer;" onclick="history.back(-1)"/>
			</td>
		</tr>
	</table>
</form>

<script type="text/javascript">
	var params;

	function desabilitarConteudo( id ){
		var url = 'obras.php?modulo=principal/obratermodeajuste&acao=A&carga='+id;
		if ( document.getElementById('img'+id).name == '-' ) {
			url = url + '&subAcao=retirarCarga';
			var myAjax = new Ajax.Request(
				url,
				{
					method: 'post',
					asynchronous: false
				});
		}
	}
	
<?php if ( $_SESSION['obrasTermo']['carga'] ): ?>
	desabilitarConteudo( <?=$_SESSION['obrasTermo']['carga']?> ); 
	abreconteudo( 'obras.php?modulo=principal/obratermodeajuste&acao=A&subAcao=gravarCarga&carga=<?=$_SESSION['obrasTermo']['carga']?>&params=' + params, '<?=$_SESSION['obrasTermo']['carga']?>' );
<? endif; ?>	
	
</script>
