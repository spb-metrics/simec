<?php

if( !$_SESSION["obras"]["gpdid"] ){
	header( "location:obras.php?modulo=inicio&acao=A" );
	exit;
}

$supervisao = new supervisao();

// requisições da tela
switch( $_REQUEST["requisicao"] ){
	
	case "novarota":
		$_SESSION["obras"]["rotid"] = null;
	break;
	
	case "salvar":
		$supervisao->obrSalvaRota( $_REQUEST );
	break;
	
	case "aprovar":
		$supervisao->obrAprovaRota( $_REQUEST["rotid"] );
	break;
	
	case "cancelaraprovacao":
		$supervisao->obrCancelaAprovacaoRota( $_REQUEST["gpdid"] );
	break;
		
	case "proporrota":
		$supervisao->obrProporRota( $_REQUEST["rotid"] );	
	break;
	
}

// cria a sessão e verifica se é inserção ou atualização
if( $_REQUEST["rotid"] || $_SESSION["obras"]["rotid"] ){
	
	$_SESSION["obras"]["rotid"] = $_REQUEST["rotid"] ? $_REQUEST["rotid"] : $_SESSION["obras"]["rotid"];
	$obrProponente = $supervisao->obrBuscaProponente( $_SESSION["obras"]["rotid"] );	
	
}

// verifica os perfis do usuário
$obrMec = possuiPerfil( array( PERFIL_SAA, PERFIL_ADMINISTRADOR ) );

// cabecalho padrão do sistema
include_once APPRAIZ . "includes/cabecalho.inc";

// Monta as abas
print "<br/>";
$db->cria_aba( $abacod_tela, $url, $parametros );
monta_titulo( "Inserir Rotas", "" );

if( $_REQUEST["gpdid"] || $_SESSION["obras"]["gpdid"] ){
	
	$_SESSION["obras"]["gpdid"] = $_REQUEST["gpdid"] ? $_REQUEST["gpdid"] : $_SESSION["obras"]["gpdid"];
	
	// verifica se tem rota aprovada
	$rotAprovada = $supervisao->obrVerRotaAprovada($_SESSION["obras"]["gpdid"]);
	
	// monta o cabeclho do grupo
	$supervisao->obrMontaCabecalhoGrupo($_SESSION["obras"]["gpdid"]);

	// ações workflow
	$esdid = obrPegarEstadoAtual( $_SESSION["obras"]["gpdid"] );
	$docid = obrCriarDocumento( $_SESSION["obras"]["gpdid"] );
	
}

?>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>

<script type="text/javascript">

	$(document).ready(function() {

		/**
		 * função que bloqueia os campos caso o usuário tenha marcado Sim na coluna Aprovação
		 * e salva o valor dos selects bloqueados dentro da div id="bloqueados"
		 *
		 **/
		$("input[name^='aprovacao_']").each(function() {

			if( this.checked && this.value == 1 ){
				
				var codigo 	 = this.name.replace('aprovacao_','');
				var entidade = this.id.replace('aprovacao_','');

				$("input[id^='aprovacao_"+entidade+"']").attr('readonly', true);
				
				$("select[id^='tdeid_"+codigo+"']").attr('disabled', true);
				$("select[id^='tdeid_"+entidade+"']").attr('disabled', true);
				
				// pegando o conteúdo da div que armazenará os valores dos campos disabled
				var html = $('#bloqueados').html();

				var valor1 = $("select[id^='tdeid_"+codigo+"']").val();
				var valor2 = $("select[id^='tdeid_"+entidade+"']").val();
				
				if( isNaN(valor1) ){
					var hidden = '<input type="hidden" name="tdeid['+codigo+']" id="tdeid_'+entidade+'" value="'+valor2+'">';
				}else{
					var hidden = '<input type="hidden" name="tdeidempresa_'+codigo+'" id="tdeid_'+codigo+'" value="'+valor1+'">';
				}
				
				$("input[id^='trjkm_"+entidade+"']").attr('readonly', true);
				
				$("input[id^='trjvlrpedagio_"+entidade+"']").attr('readonly', true);
				
				$("input[id^='trjtempo_"+entidade+"']").attr('readonly', true);

				// colocando os valores dentro da div
				$('#bloqueados').html(html+hidden);
				
			}
			
		});

	});

</script>

<form action="" method="post" name="formulario" id="obrFormRotas" >
	<input type="hidden" name="requisicao" id="requisicao" value="salvar"/>
	<input type="hidden" name="gpdid" id="gpdid" value="<?php print $_SESSION["obras"]["gpdid"]; ?>"/>
	<input type="hidden" name="rotid" id="rotid" value="<?php print $_SESSION["obras"]["rotid"]; ?>"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td>
				<fieldset style="text-align: center;">
					<legend>LEGENDA</legend>
					<img src="../imagens/editar_nome_vermelho.gif" style="vertical-align: middle;"/> Possui Observação &nbsp;&nbsp;&nbsp;
					<img src="../imagens/editar_nome.gif" style="vertical-align: middle;"/> Não Possui Observação
				</fieldset>
			</td>
		</tr>
<!-- </table> fim da tabela de legenda
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center"> início da tabela com os resultados -->
		<tr>
			<td>
				<br/>
				
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="2" cellPadding=3 align="center" id="tbListaRota">
					<tr>
						<td rowspan="2" class="SubTituloCentro" style="width: 5%;">Sequência</td>
						<td class="SubTituloCentro" rowspan="2" style="width: 10%;">Aprovação</td>
						<td class="SubTituloCentro" rowspan="2" style="width: 25%;">Trajetória</td>
						<td class="SubTituloCentro" rowspan="2" style="width: 10%;">Município</td>
						<td class="SubTituloCentro" colspan="2" style="width: 30%;">Deslocamento</td>
						<td class="SubTituloCentro" colspan="3" style="width: 20%;">Dados da Trajetória</td>
						<td class="SubTituloCentro" rowspan="2" style="width: 5%;">Ordem</td>
					</tr>
					<tr>
						<td class="SubTituloCentro">Tipo</td>
						<td class="SubTituloCentro">Detalhamento -  Trajeto Alternativo (R$)</td>
						<td class="SubTituloCentro">Distância (Km)</td>
						<td class="SubTituloCentro">Valor do Pedágio (R$)</td>
						<td class="SubTituloCentro">Tempo (Hrs)</td>
					</tr>
					<tr bgcolor="#DDDDDD">
						<td style="text-align: center;">1</td>
						<td style="text-align: center;">-</td>
						<td>
							<?php

								$sql = "SELECT
											ee.entid as id,
											mun.mundescricao,
											entnome as nome,
											ee.entid
										FROM
											entidade.entidade ee
										INNER JOIN
											obras.empresacontratada ec ON ec.entid = ee.entid
										INNER JOIN
											obras.empresaufatuacao oe ON oe.epcid = ec.epcid
										INNER JOIN
											territorios.municipio mun ON mun.muncod = oe.muncod
										INNER JOIN
											obras.grupodistribuicao og ON og.epcid = ec.epcid AND og.estuf = oe.estuf
										WHERE
											og.gpdid = {$_SESSION["obras"]["gpdid"]}";
							
								$empresa = $db->pegaLinha( $sql );
								
								$entnome = $empresa["nome"];
								
							?>
							<img src='../imagens/globo_terrestre.png' onclick="abreMapaObras('<?=$empresa['id'];?>')" style='vertical-align:middle; cursor: pointer; margin-right: 34px;' title='Visualizar Mapa'/>
							<?=$entnome?>
						</td>
						<td style="text-align: left;"><?=$empresa['mundescricao'];?></td>
						<td style="text-align: center;"> - </td>
						<td style="text-align: center;"> - </td>
						<td style="text-align: center;"> - </td>
						<td style="text-align: center;"> - </td>
						<td style="text-align: center;"> - </td>	
						<td style="text-align: center;"> - </td>
					</tr>
					<?php 
					
						$supervisao->obrMontaListaTrajetorias( $_SESSION["obras"]["gpdid"], $_SESSION["obras"]["rotid"], $esdid, $obrProponente );
					
					?>
				</table>
				
				<br/>
				
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td>
				
				<?php
					if( $obrProponente == 2 &&  $_SESSION["obras"]["rotid"] && $obrMec ){

						print "<input type='button' value='Salvar' style='cursor: pointer;' onclick='validaDados();' />";
						print "<input type='button' value='Aprovar Rota' style='cursor:pointer;' onclick='obrAprovaRota({$_SESSION["obras"]["rotid"]});'/>";
						
					}else if( $_SESSION["obras"]["rotid"] && $obrMec && ($esdid == OBREMAVALIAMEC) && !$rotAprovada ){
						print "<input type='button' value='Salvar' onclick='validaDados();'/>"; // tinha disabled="disabled", a pedido do Mario foi removido
						print "<input type='button' value='Aprovar Rota' style='cursor:pointer;' onclick='obrAprovaRota({$_SESSION["obras"]["rotid"]});'/>";
						print "<input type='button' value='Propor Rota Apartir Desta' style='cursor: pointer;' onclick='obrProporRota({$_SESSION["obras"]["rotid"]});'/>";		
						
					}else if( $_SESSION["obras"]["gpdid"] && $obrMec && ($esdid == OBREMAVALIAMEC) && $rotAprovada ){
						print "<input type='button' value='Salvar' disabled='disabled'/>";
						print "<input type='button' value='Cancelar Aprovação' " . ($supervisao->obrGrupoVinculoOS( $_SESSION["obras"]["gpdid"] ) ? 'disabled="disabled"' : '') . " style='cursor:pointer;' onclick='obrCancelaAprovacao({$_SESSION["obras"]["gpdid"]});'/>";
						
//					}else if( (($esdid == OBREMAVALIAMEC && $obrProponente == 1) || $rotAprovada) && !$db->testa_superuser() ){
					}else if( $esdid == OBREMDEFINROTA ){
						print "<input type='button' value='Salvar' style='cursor: pointer;' onclick='validaDados();' />";
					}else if( (($esdid == OBREMAVALIAMEC && $obrProponente == 1) || $rotAprovada) && !possuiPerfil( array( PERFIL_SAA, PERFIL_ADMINISTRADOR ) ) ){
						print "<input type='button' value='Salvar' disabled='disabled'/>";
					}else{
						print "<input type='button' value='Salvar' style='cursor: pointer;' onclick='validaDados();' />";
					}
					
				?>
				
				<input type="button" value="Voltar" style="cursor: pointer;" onclick="history.back(-1);" />
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript">

function verificarTipoDeslocamento(obj) {
	if(obj.parentNode.parentNode.cells[4].childNodes[0]) {
		if(obj.value == 3) {
		
			obj.parentNode.parentNode.cells[4].childNodes[0].src='../imagens/alterar.gif';
			obj.parentNode.parentNode.cells[4].childNodes[0].style.cursor = 'pointer';
			
		} else {
			obj.parentNode.parentNode.cells[4].childNodes[0].src='../imagens/alterar_01.gif';
			obj.parentNode.parentNode.cells[4].childNodes[0].style.cursor = '';
			obj.parentNode.parentNode.cells[4].childNodes[2].value = '';
		}
	}
}

function detalharTrajAlternativa(trjid, obj) {
	if(obj.parentNode.parentNode.cells[4].childNodes[0].value == 3) {
		window.open('obras.php?modulo=principal/supervisao/detalharTrajAlternativa&acao=A&trjid='+trjid,'Observações','scrollbars=no,height=300,width=500,status=no,toolbar=no,menubar=no,location=no');
	}
}



function contabilizaKM(){
 
	var trajetos = document.getElementById('trajetos_id').value;
	var arrTrj=trajetos.split(",");
	var trj = new Array();
	var trj_total = 0;
	
	for ( var i in arrTrj ){
	    if(document.getElementById('trjkm_' + arrTrj[i]) && (i != arrTrj.length - 1) ){
	    	trj[i] = document.getElementById('trjkm_' + arrTrj[i]).value
	    	trj[i] = trj[i].replace(".", "");
	    	trj[i] = trj[i].replace(",", ".");
	    	trj_total += eval(trj[i]);
	    }
	}
	trj_total = trj_total.toFixed(1);
	document.getElementById('trjkm_total').value = mascaraglobal("[.###],#",trj_total);

}

function contabilizaTempo(){
	
	var trajetos = document.getElementById('trajetos_id').value;
	var arrTrj=trajetos.split(",");
	var trj = new Array();
	var total_horas = 0;
	var hora = 0;
	for ( var i in arrTrj ){
	    if(document.getElementById('trjtempo_' + arrTrj[i]) && (i != arrTrj.length - 1) ){
	    	hora = !document.getElementById('trjtempo_' + arrTrj[i]).value ? 0 : document.getElementById('trjtempo_' + arrTrj[i]).value;
	    	total_horas = somarHoras(total_horas , hora );
	    }
	}
	
	document.getElementById('trjtempo_total').value = total_horas;

}

function contabilizaPedagio(){
	
	var trajetos = document.getElementById('trajetos_id').value;
	var arrTrj = trajetos.split(",");
	var trj = new Array();
	var total_pegagio = 0;
	var pedagio = 0;
	for ( var i in arrTrj ){
	    if(document.getElementById('trjvlrpedagio_' + arrTrj[i]) && (i != arrTrj.length - 1) ){
	    	pedagio = !document.getElementById('trjvlrpedagio_' + arrTrj[i]).value ? 0 : document.getElementById('trjvlrpedagio_' + arrTrj[i]).value.replace( ",", "." );
	    	total_pegagio = Number(total_pegagio) + Number( pedagio );
	    }
	}
	str_valor = total_pegagio.toString();
	var divisao = str_valor.split(".");
	var reais=divisao[0];
	var centavos=divisao[1];
	if(centavos){
		var formato_cent=centavos.substring(0,2);
		if(formato_cent.length == 1)
			formato_cent = formato_cent + '0';
	}
	else
		var formato_cent = '00';

	document.getElementById('trjvlrpedagio_total').value = reais + ',' +  formato_cent;
	document.getElementById('trjvlrpedagio_total').value = mascaraglobal('#.###,##', document.getElementById('trjvlrpedagio_total').value);

}

function validaDados(){
	
	var trajetos = document.getElementById('trajetos_id').value;
	var arrTrj=trajetos.split(",");
	var trj = new Array();
	var erro = false;
	
	$("[id*='tdeid_']").each(function (){
					if( $(this).val().trim() == "" ){
			    		alert('Favor informar o Tipo de Deslocamento.');
						$(this).focus();
						erro = true;
						return false;
					}		
				});
	if ( !erro ){			
		$("[id*='trjkm_']").each(function (){
						if( $(this).val().trim() == "" ){
					    	alert('Favor informar a Distância (Km) da Trajetória.');
							$(this).focus();
							erro = true;
							return false;
						}		
					});
	}			
	
	if ( !erro ){			
		$("[id*='trjtempo_']").each(function (){
						if( $(this).val().trim() == "" ){
				    		alert('Favor informar o Tempo (Hrs)da Trajetória.');
							$(this).focus();
							erro = true;
							return false;
						}		
					});
	}
//	for ( var i in arrTrj ){
//	    if(document.getElementById('tdeid_' + arrTrj[i])){
//	    	
//	    	if(!document.getElementById('tdeid_' + arrTrj[i]).value || document.getElementById('tdeid_' + arrTrj[i]).value == ""){
//	    		alert('Favor informar o Tipo de Deslocamento.');
//	    		document.getElementById('tdeid_' + arrTrj[i]).focus();
//	    		erro = true;
//	    		return false;
//	    	}
//	    	if(!document.getElementById('trjkm_' + arrTrj[i]).value || document.getElementById('trjkm_' + arrTrj[i]).value == ""){
//	    		alert('Favor informar a Distância (Km) da Trajetória.');
//	    		document.getElementById('trjkm_' + arrTrj[i]).focus();
//	    		erro = true;
//	    		return false;
//	    	}
//	    	if(!document.getElementById('trjtempo_' + arrTrj[i]).value || document.getElementById('trjtempo_' + arrTrj[i]).value == ""){
//	    		alert('Favor informar o Tempo (Hrs)da Trajetória.');
//	    		document.getElementById('trjtempo_' + arrTrj[i]).focus();
//	    		erro = true;
//	    		return false;
//	    	}
//	    	
//	    }
//	}
	
	if(document.getElementById('epcid').value != '') {
		if(document.getElementById('tdeid_' + document.getElementById('epcid').value == "")) {
			alert('Favor informar o Tipo de Deslocamento.');
	    		document.getElementById('tdeid_' + document.getElementById('epcid').value).focus();
	    		erro = true;
	    		return false;
		}
	}
	
	if(erro == false){
		document.getElementById("obrFormRotas").submit()
	}
	
}

function somarHoras(hora1,hora2){
	
	var hora1 = new String( !hora1 ? 0 : hora1);
	var hora2 = new String( !hora2 ? 0 : hora2);
	
	var min1 = 0;
	var min2 = 0;
	var min_total = 0
	var hora_total = 0
	var hr_por_min = 0;
	
	if(hora1.search(":") >= 0){
		var h_m = hora1.split(":");
		hora1 = h_m[0];
		min1 = h_m[1]; 
	}
	if(hora2.search(":") >= 0){
		var h_m2 = hora2.split(":");
		hora2 = h_m2[0];
		min2 = h_m2[1];
	}
	
	hora1 = Number(hora1);
	hora2 = Number(hora2);
	min1 = Number(min1);
	min2 = Number(min2);
	
	min_total = min1 + min2;
	hora_total = hora1 + hora2;
		
	if(min_total > 59){
		hr_por_min = parseInt(min_total / 60);
		min_total -= parseInt(hr_por_min * 60);
		hora_total += hr_por_min;
	}
	
	hora_total = hora_total < 10 ? "0" + hora_total : hora_total;
	min_total = min_total < 10 ? "0" + min_total : min_total;
	
	
	return hora_total + ":" + min_total;
	

}
contabilizaKM();
contabilizaPedagio();
contabilizaTempo();
</script>
