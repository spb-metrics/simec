<?php

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */


if($_POST['operacao'] == "A"){
		salvaObrasRelacionadas($_SESSION['obra']['obrid'],$_POST['obrid']);
		echo "<script>alert('Operação realizada com sucesso!');</script>";
	}
	
if($_REQUEST['requisicao'] == "obrasRelacionadas"){
	$titulo_modulo = "Lista de Obras";
	monta_titulo( $titulo_modulo, '' );
	?>
	<script>
		function pesquisarObrasRelacionadas(){
			document.getElementById('operacao').value = "pesquisar";
			document.getElementById('formulario_obras').submit();
		}
		function todasObrasRelacionadas(){
			window.location.href = 'obras.php?modulo=principal/obras_relacionadas&acao=A&requisicao=obrasRelacionadas&obridrel=<? echo $_SESSION['obra']['obrid'] ?>';
		}
		function carregaConteudoObraTabela(obj,obrid){
			
			var tbl_obra = obj.parentNode.parentNode.parentNode;
			var tr_obra = obj.parentNode.parentNode;
			var tbl_obras_rel = window.opener.document.getElementById('tbl_obras_relacionadas');
			var num_trs = tbl_obras_rel.rows.length;
			
			if(obj.checked == true){
				
				var htmlDocumento = document.getElementById('documento_hidden_' + obrid).innerHTML;
				var htmlFoto = document.getElementById('foto_hidden_' + obrid).innerHTML;
				var htmlRestricao = document.getElementById('restricao_hidden_' + obrid).innerHTML;
				var htmlPI = document.getElementById('pi_hidden_' + obrid).innerHTML;
				var htmlID = " <input type='hidden' name='obrid[" + obrid + "]' value='" + obrid + "' /> " + obrid;
				var htmlNomeObra = tbl_obra.rows[tr_obra.rowIndex - 1].cells[2].innerHTML;
				var htmlMunicipio = tbl_obra.rows[tr_obra.rowIndex - 1].cells[3].innerHTML;
				var htmlSituacaoObra = tbl_obra.rows[tr_obra.rowIndex - 1].cells[4].innerHTML;
				var htmlDataInicio = tbl_obra.rows[tr_obra.rowIndex - 1].cells[5].innerHTML;
				var htmlDataFim = tbl_obra.rows[tr_obra.rowIndex - 1].cells[6].innerHTML;
				var htmlTipoObra = tbl_obra.rows[tr_obra.rowIndex - 1].cells[7].innerHTML;
				var htmlUltimaAtualizacao = tbl_obra.rows[tr_obra.rowIndex - 1].cells[8].innerHTML;
				var htmlExecutado = tbl_obra.rows[tr_obra.rowIndex - 1].cells[9].innerHTML;
				
				var corRow = tbl_obras_rel.rows[num_trs - 1].getAttribute("bgcolor") == '#f7f7f7' ? '' : '#f7f7f7';
				var row = tbl_obras_rel.insertRow(num_trs);
				row.id = "obrid_" + obrid;
				row.setAttribute("bgcolor",corRow);
				var td_documento = row.insertCell(0); td_documento.innerHTML = htmlDocumento;
				var td_foto = row.insertCell(1); td_foto.innerHTML = htmlFoto;
				var td_restricao = row.insertCell(2); td_restricao.innerHTML = htmlRestricao;
				var td_pi = row.insertCell(3); td_pi.innerHTML = htmlPI;
				var td_id = row.insertCell(4); td_id.innerHTML = htmlID;
				var td_nome_obra = row.insertCell(5); td_nome_obra.innerHTML = '<a onclick="javascript:Atualizar(\'?modulo=principal/cadastro&amp;acao=A\',' + obrid + ');" href="#" style="margin: 0pt -20px 0pt 20px; text-transform: capitalize;">' + htmlNomeObra + '</a>'
				var td_municipio = row.insertCell(6); td_municipio.innerHTML = htmlMunicipio;
				var td_situacao = row.insertCell(7); td_situacao.innerHTML = htmlSituacaoObra;
				var td_data_inicio = row.insertCell(8); td_data_inicio.innerHTML = htmlDataInicio;
				var td_data_fim = row.insertCell(9); td_data_fim.innerHTML = htmlDataFim;
				var td_tipo_obra = row.insertCell(10); td_tipo_obra.innerHTML = htmlTipoObra;
				var td_ultima_atu = row.insertCell(11); td_ultima_atu.innerHTML = htmlUltimaAtualizacao;
				var td_executado = row.insertCell(12); td_executado.innerHTML = htmlExecutado;
				
			}else{
				
				var row_index = window.opener.document.getElementById('obrid_' + obrid).rowIndex;
				tbl_obras_rel.deleteRow(row_index);
				
			}
			
			
			
		}
	</script>
	<form name="formulario_obras" id="formulario_obras" method="post" >
	<input type="hidden" name="operacao" id="operacao" value="pesquisa" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
			<td  bgcolor="#CCCCCC" colspan="2"><b>Argumentos da Pesquisa</b></td>
	</tr>
	<tr>
			<td class="SubTituloDireita">Nome da Obra / Número do Convênio</td>
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
	<tr bgcolor="#c0c0c0">
			<td colspan=2 align="center">
				<input type="button" onclick="pesquisarObrasRelacionadas()" style="cursor: pointer;" value="Pesquisar">
				<input type="button" onclick="todasObrasRelacionadas()" style="cursor: pointer;" value="Mostrar Todos">
			</td>
		</tr>
	<tr>
		<td colspan="2" >
		<?carregaObrasRelacionadas(true,$_REQUEST['obridrel'])?>
		</td>
	</tr>
	<tr bgcolor="#c0c0c0">
			<td colspan=2 align="center">
				<input type="button" onclick="window.close()" style="cursor: pointer;" value="Fechar">
			</td>
		</tr>
	</table>
	</form>
	<?php chkSituacaoObra(); ?><?

	exit;
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo = "Obras Relacionadas";
monta_titulo( $titulo_modulo, '' );

$obrid = $_SESSION['obra']['obrid'];

$obras  = new Obras();
$dados = $obras->Dados($obrid);
$dobras = new DadosObra($dados);
echo $obras->CabecalhoObras();

?>
<form id="form_obras_relacionadas"  name="form_obras_relacionadas" method="post" >
<input type="hidden" name="operacao" value="A" /> 
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td>
			<? carregaObrasRelacionadas(); ?>
		</td>
	</tr>
	<?php if($habilitado){ ?>
		<tr>
				<td>
					<img border="0" title="Relacionar Obras" style="cursor: pointer;vertical-align: middle" onclick="obrasRelacionadas()" src="/imagens/gif_inclui.gif">
					<a href="javascript:obrasRelacionadas()" >Inserir / Remover Obras</a>
				</td>
		</tr>
	<?php }?>
	<tr bgcolor="#D0D0D0">
			<td align="center">
				<input type="button" onclick="salvarObrasRelacionadas()"  <?php if($somenteLeitura=="N") echo "disabled"; ?>  style="cursor: pointer;" value="Salvar">
			</td>
		</tr>
</table>
</form>
<script>
	function obrasRelacionadas(){
		var obrid = '<? echo $_SESSION['obra']['obrid'];?>'
		var url = 'obras.php?modulo=principal/obras_relacionadas&acao=A&requisicao=obrasRelacionadas&obridrel=' + obrid
		janela(url,'Obras Relacionadas','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
		
	}
	function salvarObrasRelacionadas(){
		document.getElementById('form_obras_relacionadas').submit();
	}
</script>
<?php chkSituacaoObra(); ?>