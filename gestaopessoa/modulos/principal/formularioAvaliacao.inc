	<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<?php 
$perfil = arrayPerfil();
$super = array(Array(PERFIL_ADMINISTRADOR),Array(PERFIL_SUPER_USER));

if( !in_array($perfil,$super) && (Date("c") > '2010-11-30T23:59:59-02:00') ){
	echo "<script>alert('Prazo encerrado.')</script>";
	header('Location: ?modulo=principal/listaPessoalAvaliacao&acao=A');
}

$_SESSION['countDefinicao'] = 0;
if( $_GET['type'] != 'print'){
	include  APPRAIZ."includes/cabecalho.inc";
	echo '<br>';
	if( !$_SESSION['boautoavaliacao'] || $_SESSION['autoavalchefe'] ){ 
		$db->cria_aba( $abacod_tela, $url, '' );
	} 
}else{
	?>
	<style type="">
		@media print {.notprint { display: none } .div_rolagem{display: none} .div_rol{display: 'none'} }        
        
		@media screen {.notscreen { display: none; }.div_rol{display: none;} .}
                       
		.div_rolagem{ overflow-x: auto; overflow-y: auto; height: 50px;}
		.div_rol{ overflow-x: auto; overflow-y: auto; height: 50px;}
                       
	</style>
	<!-- <script src="../includes/prototype.js"></script> -->
	<script language="JavaScript" src="/includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
	<?
}

function limparNotas($tavid){
	global $db;
	$sql = "DELETE 
			FROM 
				gestaopessoa.respostaavaliacao 
			WHERE 
				sercpf = '".$_SESSION['cpfavaliado']."' 
				AND tavid = {$tavid}
				AND resano = {$_SESSION['exercicio']}";
	
	$db->executar( $sql );
	$db->commit();
}

function salvar(){
	global $db;
	if( !existeNotaAvaliado($_SESSION['cpfavaliado']) ){
		limparNotas( TIPO_AVAL_SUPERIOR );
		
		$dadosDefid = $_POST['defid']; 
	
		if( trim( $_POST['pendente'] ) == "gravar"){ 
			$pendente = "t";
		}elseif( trim( $_POST['pendente'] ) == "finalizar" ){
			$pendente = "f";		
		}
		if(is_array($dadosDefid)){
			foreach( $dadosDefid as $defid => $arDefid ){ 
				if(is_array( $arDefid )){
					foreach( $arDefid as $tavid => $arTavid){ 
						$value = $_POST['defid'][$defid][$tavid] ? $_POST['defid'][$defid][$tavid] : 0;
						$sql = "INSERT INTO gestaopessoa.respostaavaliacao( tavid, defid, sercpf, resnota, resano, resavaliacaopendente )
								VALUES ( $tavid, $defid, '".$_SESSION['cpfavaliado']."', '".$value."', ".$_SESSION['exercicio'].", '$pendente')";
						$ins = $db->executar( $sql );
					}
				}
			}
			$db->commit();
		}
		
	}
	
	echo("<script>alert('Operação realizada com sucesso.')\n</script>");
    echo("<script>window.location.href = 'gestaopessoa.php?modulo=principal/formularioAvaliacao&acao=A';</script>"); 
	exit();	
} 
if( $_POST['action'] == 'salvar'){
	salvar();
} 
prazoVencido();
 
monta_titulo( 'Gestão de Pessoas', 'Avaliação' );  
?>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
	 	<td align="center">
	 		<h2>Formulário de Avaliação de Desempenho Individual - <b>GDPGPE</b></h2>
	 	</td>
	</tr>
</table> 
<table  style="padding:15px; background-color:#e9e9e9; color:#404040; "align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1">
	<tr>
	 	<td align="left">
	 		<h2>1 - Instruções</h2>  
	 		<div id="div_instrucao" style=" margin-left:50px; margin-right:50px; text-align: justify;">
		 		A premissa básica deste Instrumento de Avaliação é a de que o avaliado e o avaliador sejam capazes de realizar um 
		 		exercício de maturidade profissional e respeito mútuo, cujo resultado seja uma Avaliação Consensual, fruto de um 
		 		diálogo franco e responsável. Procure desfrutar intensamente este momento, transformando-o em uma demonstração de 
		 		abertura, aprendizagem e auto desenvolvimento. O servidor será avaliado em cada um dos Fatores indicados no 
		 		bloco 3 abaixo, que representam aspectos observáveis do desempenho e referem-se ao trabalho efetivamente realizado 
		 		pelo servidor, podendo a avaliação variar de 0 a 100, sendo multiplicado pelo seu respectivo peso para definição 
		 		da nota final.
	 		</div>
	 	</td>
	</tr>
</table>
<table  style="padding:15px; background-color:#e9e9e9; color:#404040;" align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1">
	<?php
	$dadosPessoa = getDadosPessoa( $_SESSION['cpfavaliado'] );
	?>
	<tr>
	 	<td colspan="2" align="left">
	 		<h2>2 - Identificação</h2> 
	 	</td>
	</tr> 
	<tr>
	 	<td align="left">
	 		<b>Nome do Servidor</b> <br>
	 		<?=$dadosPessoa[1]; ?>
	 	</td>
	 	<td>
	 		<b>SIAPE</b><br>
	 		<?= $dadosPessoa[2]; ?>
	 	</td>
	</tr>
	<tr>
	 	<td align="left" colspan="2">
	 		<b>Cargo Efetivo</b> <br>
	 		<?= $dadosPessoa[3]; ?>
	 	</td>
	</tr>
</table>
<!-- 
<table  style="padding:15px; background-color:#e9e9e9; color:#404040; "align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1">
	<tr>
	 	<td align="left">
	 		<b>Unidade de Avaliação</b><br>
	 		Alguma Unidade
	 	</td>
	 	<td align="left">
	 		<b>Período Avaliado</b><br>
	 		Algum Período
	 	</td>
	</tr>
</table>
 -->
 <table  style="padding:15px; background-color:#e9e9e9; color:#404040; "align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1">
	<tr>
	 	<td align="left">
	 		<b>Chefe</b><br>
	 		<?= $dadosPessoa[4]; ?>
	 	</td>
	</tr>
</table>
<table  style="padding:15px; background-color:#e9e9e9; color:#404040;" align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1">
	<tr>
	 	<td colspan="6" align="left">
	 		<h2>3 - Fatores de Avaliação</h2> 
	 	</td>
	</tr> 
</table>
<table style="padding:15px; background-color:#e9e9e9; color:#404040;" align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1">
<form name = "formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
<input type="hidden" id="action" name="action" value="0">
<input type="hidden" id="pendente" name="pendente" value="0">
	<tr>
		<th><center><b>Competência</b></center></th>
		<th><center><b>Definição</b></center></th>
		<th><center><b>Peso</b></center></th>
		<?php echo getAvaliadorHTML('TIPO_CABECALHO'); ?> 
	</tr>
	<?php
	$sql = "SELECT d.defid, d.defdescricao,d.defpeso, c.comdescricao 
			FROM gestaopessoa.definicao AS d 
			INNER JOIN gestaopessoa.competencia AS c ON c.comid = d.comid
			INNER JOIN gestaopessoa.avaliacao ON avaanoreferencia = {$_SESSION["exercicio"]}
			WHERE d.defanoreferencia = {$_SESSION["exercicio"]}
			AND c.comano = {$_SESSION["exercicio"]}";
	$rsDados = $db->carregar( $sql );
	$sqlMa = "SELECT 
				--ROUND(AVG(resnota))	
				ROUND(count(resnota))					
				FROM gestaopessoa.respostaavaliacao 
				WHERE sercpf = '".$_SESSION['cpfavaliado']."' 
				--AND AND resano = {$_SESSION['exercicio']}
				AND tavid = ".TIPO_AUTO_AVAL;
	$mediaMA = $db->pegaUm( $sqlMa );
	$sqlMs = "SELECT 
				--ROUND(AVG(resnota))		
				ROUND(count(resnota))				
				FROM gestaopessoa.respostaavaliacao 
				WHERE sercpf = '".$_SESSION['cpfavaliado']."' 
				--AND AND resano = {$_SESSION['exercicio']}
				AND tavid = ".TIPO_AVAL_SUPERIOR;
	$mediaMS = $db->pegaUm( $sqlMs );
	$sqlMC = "SELECT 
				--ROUND(AVG(resnota))	
				ROUND(count(resnota))					
				FROM gestaopessoa.respostaavaliacao 
				WHERE sercpf = '".$_SESSION['cpfavaliado']."'
				--AND AND resano = {$_SESSION['exercicio']}
				AND tavid = ".TIPO_AVAL_CONSENSO;
	$mediaMC = $db->pegaUm( $sqlMC );			
	if( $rsDados ){
		for( $i = 0; $i<count( $rsDados ); $i++ ){?>
			<tr>
				<td><center> <?php echo $rsDados[$i]['comdescricao'] ?> </center></td>
				<td><center> <?php echo $rsDados[$i]['defdescricao'] ?> </center></td>
				<td><center> <?php echo $rsDados[$i]['defpeso'] ?> 		</center></td>
				<input type="hidden" value ="<?php echo $rsDados[$i]['defpeso'] ?>" name="pesoDefinicao[<?=$i;?>]" id ="pesoDefinicao[<?=$i;?>]"> 
				<?php echo getAvaliadorHTML( 'TIPO_COLUNA' , $rsDados[$i]['defpeso'], $i, $rsDados[$i]['defid']); ?> 
			</tr> 
		<?}
	}?>		
	<input type="hidden" name="countDefid" id="countDefid" value="<?=$i?>"> 
		<tr  style="padding:15px; background-color:#B5B5B5; color:#404040;">
		<td>Totais</td>
		<td></td>
		<td></td>
		<? getAvaliadorRodapeHTML(); ?>
	</tr>
</table>
 
 
<table style="padding:15px; background-color:#e9e9e9; color:#404040;" align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1">
	<tr> 
		<td align="center"><b> Nota Final: <a id="td_nota_final"> </a></b></td>
	</tr>
</table>
<?php if( avaliacaoFinalizada( $_SESSION['cpfavaliado'], TIPO_AVAL_CONSENSO)) {?> 
	<table style="padding:15px; background-color:#e9e9e9; color:#404040;" align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1">
		<tr>
			<td align="center"><b> Pontos GDPGPE: <a id="td_pontos"> </a></b></td>
		</tr>
	</table>
<?}?>  
<?
if( !controlaPermissao('superuser')){
	if( $_SESSION['boautoavaliacao'] ){  
		if( avaliacaoFinalizada( $_SESSION['cpfavaliado'], TIPO_AVAL_SUPERIOR)   || prazoVencido()) {
			$blocked = 'disabled = disabled';
		}else{
			$blocked = '';
		}
	}else{
		if( avaliacaoFinalizada( $_SESSION['cpfavaliado'], TIPO_AVAL_SUPERIOR)  || prazoVencido()) {
			$blocked = 'disabled = disabled';
		}else{
			$blocked = '';
		}
	}
}elseif( prazoVencido() ){
	$blocked = 'disabled = disabled';
}else{
	$blocked = '';
}
?>
<div id = "div_rolagem" class="notprint">
	<table style="padding:15px; background-color:#e9e9e9; color:#404040;" align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1">
		<tr>
			<td align="left" width="5%"><input type="button" <?=$blocked?> name="bt_gravar" value="Gravar" onclick="validaForm('gravar');"></td>
			<td align="left"><input type="button" name="bt_finalizar" <?=$blocked?> value="Finalizar" onclick="validaForm('finalizar');"></td>
			<td align="right">
				<?php if( avaliacaoFinalizada($_SESSION['cpfavaliado'], TIPO_AVAL_CONSENSO) ){ ?>
					<img onclick="imprimeAvaliacao();" style="cursor: pointer;"src="../imagens/print.png"><a href="javascript: imprimeAvaliacao();"> Imprimir Avaliação </a> 
				<? } ?>
			</td>
		</tr>
	</table>
</div>

 
<div id = "div_rol" class="notscreen" >
	<table style="padding:15px; background-color:#e9e9e9; color:#404040;" align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1">
		<tr>
			<td align="left" colspan="4">4 -<b>Ciência </b></td> 
		</tr>
		<tr>
			<td align="center" colspan="2"><b>Avaliado(Servidor)</b></td>
			<td align="center" colspan="2"><b>Avaliador(Chefia)</b></td>
		</tr>
		<tr>
			<td align="center" ><br><br>____/____/____<br></td>
			<td align="center"  ><br><br>Assinatura<br></td>
			<td align="center" ><br><br>____/____/____<br></td>
			<td align="center"  ><br><br>Assinatura</td>
		</tr>
	</table>
	</div>

</form> 
<script type="text/javascript">
function validaForm(type){ 
	var action = document.getElementById('action');
	var pendente   = document.getElementById('pendente');
	action.value = 'salvar'; 
	pendente.value = type;
	if( type == 'finalizar' ){
		if( podeFinalizar() ){
			if ( confirm( 'Após Finalizar a avaliação, não será possivel alterar nenhuma nota. Tem certeza que deseja finalizar?' ) ) {
				document.formulario.submit();
			}else{
				return false;
			}
		}else{
			alert('É necessário preencher todas as notas para finalizar esta avaliação.');
			return false;
		}
	}
	document.formulario.submit();
}
function calcula(peso, valor, div_id, campo){  
	if( Number(valor) > 100 ){
		alert('Nota não pode ser maior que 100');
		document.getElementById('defid'+campo).value=0;
		return false;
	}	
	var pesoCalculado = Number( peso * valor );
	var div = document.getElementById(div_id); 
	if( !isNaN( pesoCalculado ) ){
		div.innerHTML = pesoCalculado.toFixed();  
	}else{
		div.innerHTML = 0;  
	}
}
function calculaColunas(){
	var total_aval_superior_p = 0;
	var total_auto_aval_p = 0;
	var total_consenso_p = 0;
	var total_aval_superior = 0;
	var total_auto_aval = 0;
	var total_consenso = 0;
	var soma1 = 0;
	var soma2= 0;
	var soma3= 0;
	var soma1_p= 0;
	var soma2_p= 0;
	var soma3_p= 0;	
	var countDefid = document.getElementById('countDefid');
	var td_nota_final = document.getElementById('td_nota_final');
	var countK = 0;
	
	$('[id^="defid["]').each(function () { // início da nova contagem

		for( var k = 1; k < 4; k++ ){
			
			var parte = this.id.replace('defid[','');
			var i = parte.replace(']['+k+']','');
			
			if( document.getElementById('defid['+i+']['+k+']') ){
				
				if( k == 1 ){ 
					soma1 += parseFloat( document.getElementById('defid['+i+']['+k+']').value  );   
					soma1_p += Number(document.getElementById('div_defid['+i+']['+k+']').innerHTML   ); 
 
					total_auto_aval = Number( soma1   ); 
					total_auto_aval_p = Number( soma1_p  );
					if( !isNaN( total_auto_aval ) && !isNaN( total_auto_aval_p ) ){ 
						document.getElementById('total_auto_aval').innerHTML = Math.round( total_auto_aval.toFixed() ); 
						document.getElementById('total_auto_aval_p').innerHTML = Math.round( total_auto_aval_p.toFixed() );  
					}else{
						document.getElementById('total_auto_aval').innerHTML = 0;
						document.getElementById('total_auto_aval_p').innerHTML = 0; 
					}	
				}else
				if( k == 2 ){
					soma2 += Number(document.getElementById('defid['+i+']['+k+']').value); 
					soma2_p += Number(document.getElementById('div_defid['+i+']['+k+']').innerHTML   );
 
					total_aval_superior = Number( soma2   );
					total_aval_superior_p = Number( soma2_p   ); 
					if( !isNaN( total_aval_superior ) && !isNaN( total_aval_superior_p ) ){ 
						document.getElementById('total_aval_superior').innerHTML = Math.round( total_aval_superior.toFixed() ); 
						document.getElementById('total_aval_superior_p').innerHTML = Math.round( total_aval_superior_p.toFixed() ); 
					}else{
						document.getElementById('total_aval_superior').innerHTML = 0; 
						document.getElementById('total_aval_superior_p').innerHTML = 0; 
					} 
				}else
				if( k == 3 ){
					soma3 += Number(document.getElementById('defid['+i+']['+k+']').value); 
					soma3_p += Number(document.getElementById('div_defid['+i+']['+k+']').innerHTML   );
 
					total_consenso = Number( soma3  );
					total_consenso_p = Number( soma3_p  );  
					if( !isNaN( total_consenso ) && !isNaN( total_consenso_p ) ){
						document.getElementById('total_consenso').innerHTML = Math.round( total_consenso.toFixed() ); 
						document.getElementById('total_consenso_p').innerHTML = Math.round( total_consenso_p.toFixed() );
					}else{
					 	document.getElementById('total_consenso').innerHTML = 0; 
						document.getElementById('total_consenso_p').innerHTML = 0;
					}
				}
			}  		
		}
	})//fim da nova contagem
	
	/*for( var i = 2; i < 10; i++ ){ // início da antiga contagem
		for( var k = 1; k < 4; k++ ){
			if( document.getElementById('defid['+i+']['+k+']') ){
				
				if( k == 1 ){ 
					soma1 += parseFloat( document.getElementById('defid['+i+']['+k+']').value  );   
					soma1_p += Number(document.getElementById('div_defid['+i+']['+k+']').innerHTML   ); 
 
					total_auto_aval = Number( soma1   ); 
					total_auto_aval_p = Number( soma1_p  );
					if( !isNaN( total_auto_aval ) && !isNaN( total_auto_aval_p ) ){ 
						document.getElementById('total_auto_aval').innerHTML = total_auto_aval.toFixed(); 
						document.getElementById('total_auto_aval_p').innerHTML = total_auto_aval_p.toFixed();  
					}else{
						document.getElementById('total_auto_aval').innerHTML = 0;
						document.getElementById('total_auto_aval_p').innerHTML = 0; 
					}	
				}else
				if( k == 2 ){
					soma2 += Number(document.getElementById('defid['+i+']['+k+']').value); 
					soma2_p += Number(document.getElementById('div_defid['+i+']['+k+']').innerHTML   );
 
					total_aval_superior = Number( soma2   );
					total_aval_superior_p = Number( soma2_p   ); 
					if( !isNaN( total_aval_superior ) && !isNaN( total_aval_superior_p ) ){ 
						document.getElementById('total_aval_superior').innerHTML = total_aval_superior.toFixed(); 
						document.getElementById('total_aval_superior_p').innerHTML = total_aval_superior_p.toFixed(); 
					}else{
						document.getElementById('total_aval_superior').innerHTML = 0; 
						document.getElementById('total_aval_superior_p').innerHTML = 0; 
					} 
				}else
				if( k == 3 ){
					soma3 += Number(document.getElementById('defid['+i+']['+k+']').value); 
					soma3_p += Number(document.getElementById('div_defid['+i+']['+k+']').innerHTML   );
 
					total_consenso = Number( soma3  );
					total_consenso_p = Number( soma3_p  );  
					if( !isNaN( total_consenso ) && !isNaN( total_consenso_p ) ){
						document.getElementById('total_consenso').innerHTML = total_consenso.toFixed(); 
						document.getElementById('total_consenso_p').innerHTML = total_consenso_p.toFixed();
					}else{
					 	document.getElementById('total_consenso').innerHTML = 0; 
						document.getElementById('total_consenso_p').innerHTML = 0;
					}
				}
			}  		
		}
	} //fim da antiga contagem  */
	var nota_final = total_consenso_p.toFixed();
	if( !isNaN( nota_final ) ){  
		td_nota_final.innerHTML = nota_final; 
	} //todas as linhas de código abaixo estavam comentadas
	<?php if( $_SESSION['boautoavaliacao'] && !avaliacaoFinalizada($_SESSION['cpfavaliado'], TIPO_AVAL_SUPERIOR ))  {?>
		var nota_final = ( total_aval_superior_p ) // /1; // tirando a média
		if( !isNaN( nota_final ) ){
			td_nota_final.innerHTML = Math.round( nota_final.toFixed() );
		}
	<?php }elseif( $_SESSION['boautoavaliacao'] && avaliacaoFinalizada($_SESSION['cpfavaliado'], TIPO_AVAL_SUPERIOR )){?>
		var nota_final = ( total_aval_superior_p ) // /3; // tirando a média
		if( !isNaN( nota_final ) ){ 
			td_nota_final.innerHTML = Math.round( nota_final.toFixed() );
		}
	<?php } elseif( !$_SESSION['boautoavaliacao'] && !existeNotaAvaliado( $_SESSION['cpfavaliado'])){?>
		var nota_final = ( total_aval_superior_p ) // /2; // tirando a média
		if( !isNaN( nota_final ) ){ 
			td_nota_final.innerHTML = Math.round( nota_final.toFixed() );
		}
	<?php } else {?> 
		var nota_final = ( total_aval_superior_p ) // /3; // tirando a média
		if( !isNaN( nota_final ) ){  
			td_nota_final.innerHTML = Math.round( nota_final.toFixed() ); 
		} //até aqui estava comentado
	<?php }  
	 
	if( avaliacaoFinalizada( $_SESSION['cpfavaliado'], TIPO_AVAL_CONSENSO ) ){?>
	
		var pontos = 0;
		var td_pontos = document.getElementById('td_pontos');
		if( nota_final <= 30 ){
			pontos = 6;
		}else
		if( ( nota_final >=31 ) && (nota_final <=40) ){
			pontos = 8;
		}else
		if(  ( nota_final >=41 ) && (nota_final <=50) ){
			pontos = 10;
		}else
		if(  ( nota_final >=51 ) && (nota_final <=60) ){  
			pontos = 12;
		}else
		if(  ( nota_final >=61 ) && (nota_final <=70) ){
			pontos = 14;
		}else
		if(  ( nota_final >=71 ) && (nota_final <=80) ){
			pontos = 16;
		}else
		if(  ( nota_final >=81 ) && (nota_final <=90) ){
			pontos = 18;
		}else
		if(  ( nota_final >=91 ) && (nota_final <=100) ){
			pontos = 20;
		}
		td_pontos.innerHTML = pontos;
	<? } ?> 
	
 }
function podeFinalizar(){
  
	for( var i = 2; i < 10; i++ ){
		for( var k = 1; k < 4; k++ ){
			if( document.getElementById('defid['+i+']['+k+']') ){
				if( document.getElementById('defid['+i+']['+k+']').disabled == false ){
					if( document.getElementById('defid['+i+']['+k+']').value == '' ){
					 	return false;
					}
				}
			}  		
		} 
	} 
	return true;
}
function bloqueiaCampos(){
  
	for( var i = 2; i < 10; i++ ){
		for( var k = 1; k < 4; k++ ){
			if( document.getElementById('defid['+i+']['+k+']') ){
				 document.getElementById('defid['+i+']['+k+']').disabled = true; 
			}  		
		} 
	} 
}
 
<?php 
if( prazoVencido() ){?>
	bloqueiaCampos();
<?}?>
function imprimeAvaliacao(){
	window.print();
}
podeFinalizar();
calculaColunas();
</script>