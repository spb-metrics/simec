<?php
if($_REQUEST['requisicao']){
	$arrRetorno = $_REQUEST['requisicao']();
}

include  APPRAIZ."includes/cabecalho.inc";
echo '<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Força de Trabalho', 'Cursos' );
echo cabecalhoPessoa($_SESSION['fdpcpf']);

if($_SESSION['fdpcpf']){
	$sql = "select * from gestaopessoa.ocupacao where fdpcpf = '{$_SESSION['fdpcpf']}'";
	$arrDados = $db->pegaLinha($sql);
	if($arrDados){
		extract($arrDados);
	}
	
	$sql = "select * from gestaopessoa.ocupacao where fdpcpf != '{$_SESSION['fdpcpf']}'";
	$relacionamentoPossivel = $db->pegaUm($sql);
	
	if($ocpid){
		$sql = "select
					dap.fdpnome,
					dap.fdpcpf,
					rlo.rloobs
				from 
					gestaopessoa.ftdadopessoal dap
				inner join
					gestaopessoa.relacionamentoocupacao rlo ON rlo.fdpcpf = dap.fdpcpf
				where 
					ocpid = $ocpid
				order by
					dap.fdpnome";
		$arrRelacionamentos = $db->carregar($sql);
	}
}

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<style>
	.SubtituloTabela{background-color:#cccccc}
	.negrito{font-weight:bold}
	.bold{font-weight:bold}
	.normal{font-weight:normal}
	.center{text-align: center;}
	.direita{text-align: right;}
	.esquerda{text-align: left;}
	.msg_erro{color:#990000}
	.link{cursor: pointer}
	.mini{width:12px;height:12px}
	.sucess_msg{color: blue;}
	.img_middle{vertical-align:middle}
	.hidden{display:none}
	.absolute{position:absolute;padding-top:5px;padding-bottom:5px;padding-left:5px;margin-top:5px;margin-left:50px;border:solid 1px black;background-color: #FFFFFF}
	.fechar{position:relative;right:-5px;top:-26px;}
	.img{background-color:#FFFFFF}
</style>
<script type="text/javascript">
$(function() {
	$('#btn_salvar').click(function() {
			var erro = 0;
			$("[class~=obrigatorio]").each(function() { 
				if(!this.value){
					erro = 1;
					alert('Favor preencher todos os campos obrigatório');
					this.focus();
					return false;
				}
			});
			if(erro == 0){
				$("#formulario_ocupacao").submit();
			}
		});
});
function addRelacionamento(ocpid)
{
	janela('gestaopessoa.php?modulo=principal/popupDadosPoliticos&acao=A&ocpid=' + ocpid,580,420,'relacionamento');
}

function seeRelacionamento() {
	janela('gestaopessoa.php?modulo=principal/popupVerPoliticos&acao=A',1050,602,'relacionamento_show');
}

function excluirRelacionamento(fdpcpf)
{
	$('#tr_'+ fdpcpf).remove();
	alterarCoresTRTabela();
}
function alterarCoresTRTabela()
{
	var rel = $('[name=arrFdpcpf[]]');
	var n = 0;
	var cor = "";
	$.each(rel, function(key, obj) { 
		if(n%2 == 1){
			cor = "#FFFFFF";
		}else{
			cor = "";
		}
		$('#tr_'+ obj.value).attr("bgcolor","" + cor + "");
		$('#tr_'+ obj.value).attr("onmouseover","this.bgColor='" + cor + "'");
		$('#tr_'+ obj.value).attr("onmouseout","this.bgColor='" + cor + "'");
		n++; 
	});
}

</script>
<form name="formulario_ocupacao" id="formulario_ocupacao"  method="post" action="" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td colspan="2" class="SubTituloCentro">Preencha os Dados Ocupacionais</td>
		</tr>
		<tr>
			<td width="25%" class="SubtituloDireita" >Cargo</td>
			<td><?php echo campo_texto("ocpcargo","S","S","Cargo","60","","","",""); ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Código do Cargo</td>
			<td><?php echo campo_texto("ocpcodcargo","S","S","","10","","","",""); ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Valor</td>
			<td><?php
					$ocpvalor = $ocpvalor ? number_format($ocpvalor,0,'','.') : "" ;
					echo campo_texto("ocpvalor","S","S","","20","","[.###]","","");
				 ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Lotação</td>
			<td><?php echo campo_texto("ocplotacao","S","S","","60","","","",""); ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Data de Ingresso</td>
			<td><?php 
					$ocpdtingresso = $ocpdtingresso ? date("d/m/Y",strtotime($ocpdtingresso)) : "";
					echo campo_data2("ocpdtingresso","S","S",'','##/##/####',"",'',"") 
				 ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Data de Desligamento</td>
			<td><?php 
					$ocpdtdesligamento = $ocpdtdesligamento ? date("d/m/Y",strtotime($ocpdtdesligamento)) : "" ; 
					echo campo_data2("ocpdtdesligamento","N","S",'','##/##/####',"",'',"") 
				 ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Indicação Política</td>
			<td><?php echo campo_texto("ocpindpolitica","N","S","","60","","","","");?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Força Política</td>
			<td><?php echo campo_texto("ocpforcapolitica","N","S","","60","","","",""); ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Partido</td>
			<td><?php echo campo_texto("ocppartido","N","S","","60","","","",""); ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Observação</td>
			<td><?php echo campo_textarea("ocpobs","N","S",'',"80","5",'500','','','','','',''); ?></td>
		</tr>
		<tr>
			<td colspan="2" class="SubTituloCentro">Relacionamentos Ocupacionais</td>
		</tr>
		<tr>
					<td colspan="2" valign="top" align="center">
						<table id="tbl_relacionamento" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" >
							<tr>
								<td class="SubtituloTabela bold center" width="10%" >Ação</td>
								<td class="SubtituloTabela bold center" width="50%" >Nome</td>
								<td class="SubtituloTabela bold center" >Observação</td>
							</tr>
							<?php if($arrRelacionamentos): ?>
								<?php $c = 0; ?>
								<?php foreach($arrRelacionamentos as $arrRel): ?>
									<?php $color = $c%2 == 0 ? "" : "#FFFFFF" ?>
									<tr  bgcolor="<?php echo $color ?>" onmouseout="this.bgColor='<?php echo $color ?>';" onmouseover="this.bgColor='#ffffcc';"  id="tr_<?php echo $arrRel['fdpcpf'] ?>" >
										<td>
											<img onclick="excluirRelacionamento('<?php echo $arrRel['fdpcpf'] ?>')" class="link" src="../imagens/excluir.gif" />
											<input type="hidden" name="arrFdpcpf[]" value="<?php echo $arrRel['fdpcpf'] ?>"  />
										</td>
										<td><?php echo $arrRel['fdpnome'] ?></td>
										<td><?php echo campo_texto("rloobs[]","N","S","","60","","","","","","","","",$arrRel['rloobs']); ?></td>
									</tr>
									<?php $c++; ?>
								<?php endforeach; ?>
							<?php endif; ?>
						</table>
					</td>
		</tr>
		<tr>
			<td class="SubtituloDireita esquerda" colspan="2" ><a href="javascript:addRelacionamento('<?php echo $ocpid ?>')" /><img class="img_middle" style="border:none" src="../imagens/gif_inclui.gif" /> Adicionar Relacionamento</a> | <a href="javascript:seeRelacionamento('<?php echo $ocpid ?>')" /><img class="img_middle" style="border:none" src="../imagens/gif_inclui.gif" /> Ver estrutura</a></td>
		</tr>
		<tr>
			<td colspan="2" class="SubtituloTabela bold center" >
				<input type="hidden" name="ocpid" value="<?php echo $ocpid ?>" />
				<input type="hidden" name="requisicao" value="salvarOcupacao" />
				<input type="button" id="btn_salvar" name="btn_salvar" value="Salvar" />
			</td>
		</tr>
	</table>
</form>
<?php if($arrRetorno['msg']): ?>
<script>
	alert('<?php echo $arrRetorno['msg'] ?>');
</script>
<?php endif; ?>