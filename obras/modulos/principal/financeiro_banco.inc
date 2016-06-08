<?php
if( $_REQUEST["obrid"] ){
	include_once APPRAIZ . "www/obras/_permissoes_obras.php";
	require_once APPRAIZ . "www/obras/permissoes.php";
	
	session_unregister("obra");
	$_SESSION["obra"]["obrid"] = $_REQUEST["obrid"];
}

if($_POST['act']){
	switch ($_POST['act']) {
		case "salvar_banco":
			$sql = "UPDATE 
						obras.obrainfraestrutura 
					SET 
						obrbanco   = '{$_POST['obrbanco']}', 
						obragencia = '{$_POST['obragencia']}',
						obrconta   = '{$_POST['obrconta']}'
					WHERE 
						obrid = '{$_SESSION["obra"]["obrid"]}';";
			$db->executar($sql);
			$db->commit();
			$db->sucesso($modulo);
			break;
		case "salvar_saldo":
			$_POST['fibsaldo'] = str_replace(",", ".", str_replace(".", "", $_POST['fibsaldo'])); //arruma valor
			if($_POST['fibid']){
				$sql = "UPDATE 
							obras.financeirobanco 
						SET 
							fibsaldo     = '{$_POST['fibsaldo']}', 
							fibdatasaldo = '{$_POST['fibdatasaldo']}'
						WHERE 
							fibid = '{$_POST['fibid']}';";				
			}else{
				$sql = "INSERT INTO 
							obras.financeirobanco (
								 obrid,
								 fibsaldo,
								 fibdatasaldo,
								 usucpf
						)VALUES( 
							'{$_SESSION["obra"]["obrid"]}', 
							'{$_POST['fibsaldo']}', 
							'{$_POST['fibdatasaldo']}',
							'{$_SESSION['usucpf']}'
							  );";				
			}
			$db->executar($sql);
			$db->commit();
			$db->sucesso($modulo);
			break;
		case "alterar_saldo":
			if($_POST['fibid']){
				$sql = "SELECT fibid, fibdatasaldo, fibsaldo FROM obras.financeirobanco WHERE fibid = '{$_POST['fibid']}';";
				extract($db->pegaLinha($sql));
			}
			break;	
		case "excluir_saldo":
			if($_POST['fibid']){
				$sql = "DELETE FROM obras.financeirobanco WHERE fibid = '{$_POST['fibid']}';";
				$db->executar($sql);
				$db->commit();
				$db->sucesso($modulo);
			}
			break;	
	}
}

include APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";

$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( $titulo_modulo, "<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /> Indica os campos obrigatórios" );

$obras = new Obras();
echo $obras->CabecalhoObras();

if(!$_SESSION["obra"]["obrid"]) 
	die("<script>
			alert('Variavel de obra não encontrada');
			window.location='obras.php?modulo=inicio&acao=A';
		 </script>");
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<form method="post" id="formulario" name="formulario" enctype="multipart/form-data">
	<input type="hidden" name="act" value=""/>
	<input type="hidden" name="fibid" value="<?= $fibid ?>"/>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td colspan="2" style="background-color: #cccccc; text-align:center"><b>Dados Bancários</b></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%">&nbsp;</td>
			<td>
				Banco:
				<?php
					$sql = "SELECT 
								obrbanco, 
								obragencia, 
								obrconta 
							FROM 
								obras.obrainfraestrutura 
							WHERE 
								obrid = '{$_SESSION["obra"]["obrid"]}'";
					$rs = $db->pegaLinha($sql);
					if(is_array($rs))
						extract($rs);
				
				 	$obrbanco = $obrbanco ? $obrbanco : BANCO_FINANCEIRO_PADRAO;
					echo campo_texto( 'obrbanco', 'S', $somenteLeitura, '', 3, 3, '###', '', 'left', '', 0, 'id="obrbanco"'); 
				?> 
				&nbsp;
				Agência:
				<?= campo_texto( 'obragencia', 'S', $somenteLeitura, '', 5, 5, '', '', 'left', '', 0, 'id="obragencia"'); ?> 
				&nbsp;
			    Conta:
				<?= campo_texto( 'obrconta', 'S', $somenteLeitura, '', 14, 14, '', '', 'left', '', 0, 'id="obrconta"'); ?> 
				&nbsp;
				<input type="button" value="Salvar" class="botao" <?php if($somenteLeitura=="N") echo "disabled"; ?> onclick="salvarBanco();"/>
			</td>
		</tr>
		<tr>
			<td colspan="2" style="background-color: #cccccc; text-align:center"><b>Saldo</b></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">&nbsp;</td>
			<td>
				Data:
				<?= campo_data2( 'fibdatasaldo', 'S', $somenteLeitura, '', 'S' ); ?>
				&nbsp;
				Saldo:
				<?php 
					if($fibsaldo)
						$fibsaldo = formata_valor($fibsaldo);
				?>
				<input class='CampoEstilo' type='text' id='fibsaldo' name='fibsaldo' <?php if($somenteLeitura=="N") echo "disabled"; ?> size='25' maxlength='20' value='<?= $fibsaldo ?>' onfocus='this.select();' onkeypress='reais(this,event);' onkeydown='backspace(this,event);'/><?= obrigatorio(); ?>	
				&nbsp;
				<?php if($habilitado){ ?>
					<a style="cursor:pointer;" onclick="salvarSaldo();"><img src="/imagens/<?= $fibid ? "alterar" : "gif_inclui" ?>.gif" style="vertical-align:middle;" border="0" title="<?= $fibid ? "Alterar" : "Inserir Novo" ?> Saldo">&nbsp;<?= $fibid ? "Alterar" : "Inserir Novo" ?> Saldo</a>
				<?php } ?>
				<?php	if($fibid){ ?>
					&nbsp;<a style="cursor:pointer;" onclick="window.location = window.location;"><img src="/imagens/money.gif" style="vertical-align:middle;" border="0" title="Novo Saldo">&nbsp;Novo Saldo</a>
				<?php 
					}
				?>
			</td>
		</tr>
	</table>
</form>
<?php
	$sql = "SELECT 
				'<center>
				<img src=\'/imagens/alterar.gif\' style=\'cursor:pointer;\' border=\'0\' title=\'Alterar\' onclick=\'alterarSaldo('||fib.fibid||')\'>
				<img src=\'/imagens/excluir.gif\' style=\'cursor:pointer;\' border=\'0\' title=\'Excluir\' onclick=\'excluirSaldo('||fib.fibid||')\'>
				</center>' AS acao,
				'<center>' || to_char(fib.fibdatasaldo,'DD/MM/YYYY') || '</center>' AS fibdatasaldo,
				fib.fibsaldo,
				usu.usunome
  			FROM 
  				obras.financeirobanco fib
  			INNER JOIN
  				seguranca.usuario usu ON usu.usucpf = fib.usucpf
  			WHERE
  				obrid = '{$_SESSION["obra"]["obrid"]}'
  			ORDER BY 
  				fib.fibdatasaldo DESC";
	$arrCabecalho = array("Ação","Data", "Saldo", "Inserido por");
	$db->monta_lista($sql, $arrCabecalho,50, 10, "N", "center", "" );
?>
<script type="text/javascript">
	function salvarBanco(){
		var msg = "Os seguintes campos não foram preenchidos:\n";
		var invalido = false;
		if(document.formulario.obrbanco.value == ""){
			msg += "\tBanco\n";
			invalido = true;
		}
		if(document.formulario.obragencia.value == ""){
			msg += "\tAgência\n";
			invalido = true;
		}
		if(document.formulario.obrconta.value == ""){
			msg += "\tConta\n";
			invalido = true;
		}

		if(invalido)
			alert(msg);
		else{
			document.formulario.act.value = 'salvar_banco';
			document.formulario.submit();
		}
	}
	function salvarSaldo(){
		var msg = "Os seguintes campos não foram preenchidos corretamente:\n";
		var invalido = false;
		if(!validaData(document.formulario.fibdatasaldo)){
			msg += "\tData\n";
			invalido = true;
		}
		if(document.formulario.fibsaldo.value == ""){
			msg += "\tSaldo\n";
			invalido = true;
		}
		if(document.formulario.fibsaldo.value.length > 20){
			msg += "\tSaldo - O valor informado é muito grande\n";
			invalido = true;
		}

		if(invalido)
			alert(msg);
		else{
			document.formulario.act.value = 'salvar_saldo';
			document.formulario.submit();
		}
	}

	function alterarSaldo(fibid){
			document.formulario.fibid.value = fibid;
			document.formulario.act.value   = 'alterar_saldo';
			document.formulario.submit();
	}
	
	function excluirSaldo(fibid){
		if(confirm("Deseja excluir este saldo?")){
			document.formulario.fibid.value = fibid;
			document.formulario.act.value   = 'excluir_saldo';
			document.formulario.submit();
		}
	}

	/******************************************/
	// INÍCIO DA LÓGICA PARA MÁSCARA DE REAIS //
	/******************************************/
		documentall = document.all;
	/*
	* função para formatação de valores monetários retirada de
	* http://jonasgalvez.com/br/blog/2003-08/egocentrismo
	*/

	function formatamoney(c) {
	    var t = this; if(c == undefined) c = 2;		
	    var p, d = (t=t.split("."))[1].substr(0, c);
	    for(p = (t=t[0]).length; (p-=3) >= 1;) {
		        t = t.substr(0,p) + "." + t.substr(p);
	    }
	    return t+","+d+Array(c+1-d.length).join(0);
	}

	String.prototype.formatCurrency=formatamoney

	function demaskvalue(valor, currency){
	/*
	* Se currency é false, retorna o valor sem apenas com os números. Se é true, os dois últimos caracteres são considerados as 
	* casas decimais
	*/
	var val2 = '';
	var strCheck = '0123456789';
	var len = valor.length;
		if (len== 0){
			return 0.00;
		}

		if (currency ==true){	
			/* Elimina os zeros à esquerda 
			* a variável  <i> passa a ser a localização do primeiro caractere após os zeros e 
			* val2 contém os caracteres (descontando os zeros à esquerda)
			*/
			
			for(var i = 0; i < len; i++)
				if ((valor.charAt(i) != '0') && (valor.charAt(i) != ',')) break;
			
			for(; i < len; i++){
				if (strCheck.indexOf(valor.charAt(i))!=-1) val2+= valor.charAt(i);
			}

			if(val2.length==0) return "0.00";
			if (val2.length==1)return "0.0" + val2;
			if (val2.length==2)return "0." + val2;
			
			var parte1 = val2.substring(0,val2.length-2);
			var parte2 = val2.substring(val2.length-2);
			var returnvalue = parte1 + "." + parte2;
			return returnvalue;
			
		}
		else{
				/* currency é false: retornamos os valores COM os zeros à esquerda, 
				* sem considerar os últimos 2 algarismos como casas decimais 
				*/
				val3 ="";
				for(var k=0; k < len; k++){
					if (strCheck.indexOf(valor.charAt(k))!=-1) val3+= valor.charAt(k);
				}			
		return val3;
		}
	}

	function reais(obj,teclapres){
	//limita a quantidade de caracteres
	if(document.formulario.fibsaldo.value.length >= 20){
		document.formulario.fibsaldo.value = document.formulario.fibsaldo.value.substr(0,(document.formulario.fibsaldo.value.length-1));
		return false;
	}
	//var whichCode = (window.Event) ? event.which : event.keyCode;

	        if(window.event) { // Internet Explorer
	         var whichCode = teclapres.keyCode; }
	        else if(teclapres.which) { // Nestcape / firefox
	         var whichCode = teclapres.which;
	        }
	      

	/*
	Executa a formatação após o backspace nos navegadores !document.all
	*/
	if (whichCode == 8 && !documentall) {	
	/*
	Previne a ação padrão nos navegadores
	*/
		if (teclapres.preventDefault){ //standart browsers
				teclapres.preventDefault();
			}else{ // internet explorer
				teclapres.returnValue = false;
		}
		var valor = obj.value;
		var x = valor.substring(0,valor.length-1);
		obj.value= demaskvalue(x,true).formatCurrency();
		return false;
	}
	/*
	Executa o Formata Reais e faz o format currency novamente após o backspace
	*/
	FormataReais(obj,'.',',',teclapres);
	} // end reais


	function backspace(obj,teclapres){
	/*
	Essa função basicamente altera o  backspace nos input com máscara reais para os navegadores IE e opera.
	O IE não detecta o keycode 8 no evento keypress, por isso, tratamos no keydown.
	Como o opera suporta o infame document.all, tratamos dele na mesma parte do código.
	*/

	//var whichCode = (window.Event) ? event.which : event.keyCode;
	        if(window.event) { // Internet Explorer
	         var whichCode = teclapres.keyCode; }
	        else if(teclapres.which) { // Nestcape / firefox
	         var whichCode = teclapres.which;
	        }
	    

	if (whichCode == 8 && documentall) {	
		var valor = obj.value;
		var x = valor.substring(0,valor.length-1);
		var y = demaskvalue(x,true).formatCurrency();

		obj.value =""; //necessário para o opera
		obj.value += y;
		
		if (teclapres.preventDefault){ //standart browsers
				teclapres.preventDefault();
			}else{ // internet explorer
				teclapres.returnValue = false;
		}
		return false;

		}// end if		
	}// end backspace

	function FormataReais(fld, milSep, decSep, teclapres) {
	var sep = 0;
	var key = '';
	var i = j = 0;
	var len = len2 = 0;
	var strCheck = '0123456789';
	var aux = aux2 = '';
	//var whichCode = (window.Event) ? e.which : e.keyCode;
	        if(window.event) { // Internet Explorer
	         var whichCode = teclapres.keyCode; }
	        else if(teclapres.which) { // Nestcape / firefox
	         var whichCode = teclapres.which;
	        }    

	//if (whichCode == 8 ) return true; //backspace - estamos tratando disso em outra função no keydown
	if (whichCode == 0 ) return true;
	if (whichCode == 9 ) return true; //tecla tab
	if (whichCode == 13) return true; //tecla enter
	if (whichCode == 16) return true; //shift internet explorer
	if (whichCode == 17) return true; //control no internet explorer
	if (whichCode == 27 ) return true; //tecla esc
	if (whichCode == 34 ) return true; //tecla end
	if (whichCode == 35 ) return true;//tecla end
	if (whichCode == 36 ) return true; //tecla home

	/*
	O trecho abaixo previne a ação padrão nos navegadores. Não estamos inserindo o caractere normalmente, mas via script
	*/

	if (teclapres.preventDefault){ //standart browsers
			teclapres.preventDefault()
		}else{ // internet explorer
			teclapres.returnValue = false
	}

	var key = String.fromCharCode(whichCode);  // Valor para o código da Chave
	if (strCheck.indexOf(key) == -1) return false;  // Chave inválida

	/*
	Concatenamos ao value o keycode de key, se esse for um número
	*/
	fld.value += key;

	var len = fld.value.length;
	var bodeaux = demaskvalue(fld.value,true).formatCurrency();
	fld.value=bodeaux;

	/*
	Essa parte da função tão somente move o cursor para o final no opera. Atualmente não existe como movê-lo no konqueror.
	*/
	  if (fld.createTextRange) {
	    var range = fld.createTextRange();
	    range.collapse(false);
	    range.select();
	  }
	  else if (fld.setSelectionRange) {
	    fld.focus();
	    var length = fld.value.length;
	    fld.setSelectionRange(length, length);
	  }
	  return false;

	}
	/****************************************/
	// FIM DA LÓGICA PARA MÁSCARA DE REAIS  //
	/****************************************/


	
</script>