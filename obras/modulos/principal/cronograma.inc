<?php
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

$obras = new Obras();
$dobras = new DadosObra(null);

// Executa as funções do módulo

if ( $_REQUEST["acao"] == 'A' ) {
	if ($_REQUEST["requisicao"] ){
		$obras->CadastrarCronogramaObras($_REQUEST);
	}
}

if($_SESSION["obra"]["obrid"]){
	$dados = $obras->Dados($_SESSION["obra"]["obrid"]);
	$dobras = new DadosObra($dados);
}   
   
// Pega o caminho atual do usuário (em qual módulo se encontra)
$caminho_atual = $_SERVER["REQUEST_URI"];
$posicao_caminho = strpos($caminho_atual, 'acao');
$caminho_atual = substr($caminho_atual, 0 , $posicao_caminho);

?>

<br/>

<?php

$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo = "Monitoramento de Obras/Infraestrutura";
monta_titulo( $titulo_modulo, 'Cronograma Físico-Financeiro' );
echo $obras->CabecalhoObras();

?>

<script src="../includes/calendario.js"></script>
<form method="post" name="formulario" id="formulario" action="<?php echo $caminho_atual; ?>acao=A">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td>Detalhamento do Cronograma Físico/Financeiro</td>
		</tr>
		<tr>
			<td>
				<?php
					
					$sql = "
						SELECT
							itco.icoid,
							itc.itcid,
							itc.itcdesc,
							itc.itcdescservico,
							itco.icopercsobreobra,
							itco.icovlritem,
							itco.icodtinicioitem,
							itco.icodterminoitem,
							itco.icopercexecutado
						FROM 
							obras.itenscomposicao itc 
						INNER JOIN 
							obras.itenscomposicaoobra itco ON itc.itcid = itco.itcid
						WHERE 
							itco.obrid =".$_SESSION["obra"]["obrid"]."
						ORDER BY 
							itco.icoordem";
					
					$itens = ($db->carregar($sql));
									
					$dados = array();
					$itcid  = null;
					
					/*
					for($i=0;$i<count($itens);$i++){
						$itcid .= $itens[$i]["itcid"];
						if($i < (count($itens)-1)){
							 $itcid .= "|";  		
						}
					}
					*/
					
			
					if($itens){
						
						foreach ($itens as $i => $linha) {							
							$valor = "icovlritem_".$linha['itcid'];
							if(number_format($linha['icovlritem'],2,',','.') == "0,00")	
								$$valor = "";
							else
								$$valor = number_format($linha['icovlritem'],2,',','.');													
							
							$total_valor += $linha['icovlritem'];
							
							$inicio = "icodtinicioitem_".$linha['itcid'];
							$$inicio = $linha['icodtinicioitem'];

							$termino = "icodterminoitem_".$linha['itcid'];
							$$termino = $linha['icodterminoitem'];
							
							$projetado = "icopercsobreobra";
							
							if(number_format($linha['icopercsobreobra'],2,',','.') == "0,00")
								$$projetado = "";
							else
								$$projetado = number_format($linha['icopercsobreobra'],2,',','.');
							
							$total_sobreaobra += $linha['icopercsobreobra'];
								
							$executado = "icopercexecutado";
							
							if(number_format($linha['icopercexecutado'],2,',','.') == "0,00")
								$$executado = "";
							else
								$$executado = number_format($linha['icopercexecutado'],2,',','.');
							
							$total_executado += $linha['icopercexecutado'];	
							
							if($$projetado == "" || $$projetado == "0" || $$projetado == "0.00" || $$projetado == "0,00")
								$porcento_executado = 0;
							else
								$porcento_executado = ($linha['icopercexecutado']*100)/$linha['icopercsobreobra'];
							
							
							if($linha['itcdescservico'] != "")
								$title = "onmouseover=\"return escape('".$linha['itcdescservico']."');\"\"";
							else
								$title = "";
							
								
							$dados[] = array(
											 "<div ".$title." >".$linha['itcdesc']."</div><input type='hidden' name='item_".$linha['itcid']."' value='".$linha['itcid']."' />",
											 "<div align=right>".number_format($linha['icopercsobreobra'],2,',','.')." %</div>",
											 $linha['icovlritem'],
											 campo_data( 'icodtinicioitem_'.$linha['itcid'], 'N', $somenteLeitura, '', 'S' ),
											 campo_data( 'icodterminoitem_'.$linha['itcid'], 'N', $somenteLeitura, '', 'S' ),										
											 "<div align=right>".number_format($linha['icopercexecutado'],2,',','.')." %</div>",
											 "<div align=right>".number_format($porcento_executado,2,',','.')." %</div>");
						}
						
						//LINHA TOTAL
						$dados[$i+1] = array("<B>TOTAL</B>","<DIV ALIGN=right><B>".number_format($total_sobreaobra,2,',','.')." %</B></DIV>","<DIV ALIGN=right><B>".number_format($total_valor,2,',','.')."</B></DIV>","","","<DIV ALIGN=right><B>".number_format($total_executado,2,',','.')." %</B></DIV>","");
	
						$cabecalho = array( "Item da Obra", 
											"(%) Sobre a Obra <br>(A)",
											"Valor (R$)",
											"Data de Início ",
											"Data de Término",
											"(%) Executado do Item Sobre a Obra <br>(B)",
											"(%) do Item Executado <br>(B x 100 / A)");
						$db->monta_lista_simples( $dados, $cabecalho, 50, 10, 'N', '98%', '' );
					}else {
						$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '98%', '' );
					}
				?>
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td>
				<div style="float: left;">
							<?
						$var = "";
						if($somenteLeitura=="N"){
							$var = "disabled";	
						}
					?>
					<input type="hidden" name="obrid" value="<? echo $_SESSION["obra"]["obrid"];?>" />
					<input type="hidden" name="requisicao" value="1"/>
					<?php if($habilitado){ ?>						
						<input type="button" value="Salvar" style="cursor:pointer;" onclick="submeteForm();" <?php if($somenteLeitura == "N") echo "disabled=\"disabled\"" ?> />
					<?php } ?> 
					<input type="button" value="Voltar" style="cursor: pointer" onclick="history.back(-1);">
				</div>
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript">

function submeteForm() {
	var form = document.getElementById("formulario");
	var cont = 0;
	
	for(var i=0; i < form.length; i++) {
		campo = form.elements[i].id.substr(0,16);
					
	  	if(campo == 'icodtinicioitem_') {
	  		id 	   = form.elements[i].id.substr(16,4);
	  		inicio = document.getElementById("icodtinicioitem_"+id);
	  		fim    = document.getElementById("icodterminoitem_"+id);
	  		
	  		if((inicio.value != "") || (fim.value != "")) {
	  			if(!validaData(document.getElementById("icodtinicioitem_"+id))){
					alert("A Data de Inicio informada é inválida!");
					inicio.focus();
					return false;
				}
				
				if(!validaData(document.getElementById("icodterminoitem_"+id))){
					alert("A Data de Término informada é inválida!");
					fim.focus();
					return false;
				}
				
		  		if(inicio.value == "") {
		  			alert("A Data de Início deve ser informada.");
		  			inicio.focus();		  			
		  			cont++;
		  			break;
		  		}
		  		
		  		if(fim.value == ""){
		  			alert("A Data de Término deve ser informada.");
		  			fim.focus();
		  			cont++;
		  			break;
		  		}		  		
			}
			if((inicio.value != "") && (fim.value != "")) {
				if(parseInt(fim.value.split( "/" )[2].toString() + fim.value.split( "/" )[1].toString() + fim.value.split( "/" )[0].toString() ) < parseInt(inicio.value.split( "/" )[2].toString() + inicio.value.split( "/" )[1].toString() + inicio.value.split( "/" )[0].toString() )) {
					alert("A Data de Término não pode ser menor que a Data de Início.");
					fim.focus();
					cont++;
					break;
				}
			}
	  	}
	}
	
	if(cont == 0)
		form.submit();
}

</script>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>