<?php

include APPRAIZ . "includes/cabecalho.inc";
include APPRAIZ . 'includes/Agrupador.php';
echo'<br>';
$db->cria_aba($abacod_tela,$url,'');
monta_titulo( $titulo_modulo, '' );

?>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript">

	jQuery.noConflict();
	
	function obras_exibeRelatorioGeral(tipo){
		
		var formulario = document.filtro;
		var agrupador = $( 'colunas' );
		var visualizar = $( 'visualizar' );
		// Tipo de relatorio
		formulario.pesquisa.value='1';

		prepara_formulario();
		selectAllOptions( formulario.colunas );
		selectAllOptions( document.getElementById( 'idimplantadora' ) );
		selectAllOptions( document.getElementById( 'obrid' ) );
		selectAllOptions( document.getElementById( 'estuf' ) );
		selectAllOptions( document.getElementById( 'muncod' ) );
//		selectAllOptions( document.getElementById( 'idempresa' ) );
		selectAllOptions( document.getElementById( 'orgid' ) );
		selectAllOptions( document.getElementById( 'stoid' ) );
		
		
		if( tipo == 'exibir' ){

			if ( !agrupador.options.length ){
				alert( 'Favor selecionar ao menos uma agrupador!' );
				return false;
			}
					
			formulario.target = 'obras_arquivos_supervisao';
			var janela = window.open( '', 'obras_arquivos_supervisao', 'width=900,height=645,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
			formulario.submit();
			janela.focus();
			
			
		}
		
	}

	function onOffCampo( campo ) {
		var div_on  = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input   = document.getElementById( campo + '_campo_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '0';
		}
	}

	function Mascara_Hora(Hora){ 
		var hora01 = ''; 
		hora01 = hora01 + Hora; 
		if (hora01.length == 2){ 
			hora01 = hora01 + ':'; 
			document.forms[0].Hora.value = hora01; 
		} 
		if (hora01.length == 5){ 
			Verifica_Hora(); 
		} 
	} 
		           
	function Verifica_Hora( hora ){ 
		if( hora.value.length > 3 ){
			hrs = (hora.value.substring(0,2)); 
			min = (hora.value.substring(3,5)); 
		}else{
			hrs = 0; 
			min = (hora.value.substring(hora.value.length-2,hora.value.length)); 
		}
		               
		estado = ""; 
		if ((hrs < 00 ) || (hrs > 23) || ( min < 00) ||( min > 59)){ 
			estado = "errada"; 
		} 
		               
		if (hora.value == "") { 
			estado = ""; 
		} 
	
		if (estado == "errada") { 
			hora.value = "";
			alert("Hora inválida!"); 
			hora.focus(); 
		} 
	} 
			

</script>
<form action="/obras/obras.php?modulo=relatorio/relatorioArquivosSupervisao_resultado&acao=A" method="post" name="filtro" id="filtro"> 
	<input type="hidden" name="form" value="1"/>
	<input type="hidden" name="pesquisa" value="1"/>
	<input type="hidden" name="publico" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado -->
	<input type="hidden" name="prtid" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
	<input type="hidden" name="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="SubTituloDireita" width="10%">Agrupadores</td>
			<td>
				<?php
					// Início dos agrupadores
					$agrupador = new Agrupador('filtro','');
					
					// Dados padrão de destino (nulo)
					$destino = isset( $agrupador2 ) ? $agrupador2 : array();
						
					// Dados padrão de origem
					$origem = array(
						'nome_usuario_inserir' => array(
													'codigo'    => 'nome_usuario_inserir',
													'descricao' => '01. Nome do usuário que inseriu'
						),
						'unidade_orcamentaria' => array(
													'codigo'    => 'unidade_orcamentaria',
													'descricao' => '02. Unidade ResponsÃ¡vel pela Obra'
						),
						'nome_obra' => array(
													'codigo'    => 'nome_obra',
													'descricao' => '03. Nome da obra'
						),
						'municipio' => array(
													'codigo'    => 'municipio',
													'descricao' => '04. Município'
						),
						'uf' => array(
													'codigo'    => 'uf',
													'descricao' => '05. UF'
						),
						'convenio' => array(
													'codigo'    => 'convenio',
													'descricao' => '06. Convênio'
						),
						'data_supervisao' => array(
													'codigo'    => 'data_supervisao',
													'descricao' => '07. Data da supervisão'
						),
						'data_inclusao_supervisao' => array(
													'codigo'    => 'data_inclusao_supervisao',
													'descricao' => '08. Data da inclusão (supervisão)'
						),
						'extensao_arquivo' => array(
													'codigo'    => 'extensao_arquivo',
													'descricao' => '09. Extensão do arquivo'
						),
						'data_arquivo' => array(
													'codigo'    => 'data_arquivo',
													'descricao' => '10. Data da inclusão (arquivo)'
						),
						'nome_arquivo' => array(
													'codigo'    => 'nome_arquivo',
													'descricao' => '11. Dados do arquivo'
						),
						'situacao_obra' => array(
													'codigo'    => 'situacao_obra',
													'descricao' => '12. Situação da obra'
						),
						'orgao_obra' => array(
													'codigo'    => 'orgao_obra',
													'descricao' => '13. Orgão da obra'
						),
						'tp_supervisao' => array(
													'codigo'    => 'tp_supervisao',
													'descricao' => '14. Responsável pela supervisão'
						)
						
						
						
						
						
					);
					
					// exibe agrupador
					$agrupador->setOrigem( 'naoColunas', null, $origem );
					$agrupador->setDestino( 'colunas', null, $destino );
					$agrupador->exibir();
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloesquerda" colspan="2">
				<strong>Filtros</strong>
			</td>
		</tr>
			<?php
				//UNIDADE IMPLANTADORA melhorar
				$sql = "SELECT 
							entid as codigo, 
							entnome as descricao
						FROM 
							obras.obrainfraestrutura o 
						INNER JOIN 
							entidade.entidade e ON e.entid = o.entidunidade 
						GROUP BY 
							entid, entnome";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Unidade ResponsÃ¡vel pela Obra', 'idimplantadora',  $sql, '', 'Selecione a(s) Unidade(s)' );

				//OBRA
				$sql = "SELECT 
					        obrid as codigo, 
					        obrdesc as descricao
						FROM 
							obras.obrainfraestrutura";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Obras', 'obrid',  $sql, '', 'Selecione a(s) Obra(s)' );

				// UF 
				$ufSql = " SELECT	
								est.estuf AS codigo,
								est.estdescricao AS descricao
							FROM 
								obras.obrainfraestrutura o
							INNER JOIN
								entidade.endereco ende ON ende.endid = o.endid 
							INNER JOIN 
								territorios.estado est ON est.estuf = ende.estuf
							WHERE
							 	o.obsstatus = 'A' 
							GROUP BY
								est.estuf, est.estdescricao
							ORDER BY
								est.estdescricao";
				$stSqlCarregados = "";
				mostrarComboPopup( 'UF', 'estuf',  $ufSql, '', 'Selecione o(s) Estado(s)' );

				// Municípios
				$munSql = " SELECT
								tm.muncod AS codigo,
								tm.estuf || ' - ' || tm.mundescricao AS descricao
							FROM 
								territorios.municipio tm
							INNER JOIN
								obras.preobra po ON po.muncod = tm.muncod
							WHERE
							 	po.prestatus = 'A'
							ORDER BY
								mundescricao";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Município', 'muncod',  $munSql, '', 'Selecione o(s) Município(s)' );
				
    			//EMPRESA melhorar
//				$sql = "SELECT 
//					        e.entid as codigo, 
//					        e.entnome as descricao
//						FROM 
//							obras.obrainfraestrutura o 
//						INNER JOIN 
//							entidade.entidade e ON e.entid = o.entidempresaconstrutora 
//						GROUP BY 
//							e.entid, e.entnome 
//						ORDER BY e.entnome";
//				
//				$stSqlCarregados = "";
//				mostrarComboPopup( 'Empresa', 'idempresa',  $sql, '', 'Selecione a(s) Empresa(s)' );
//    			Situação do grupo
				$sql = "select
							esdid AS codigo,
							esddsc AS descricao
						from
							workflow.estadodocumento ed
						where
							esdid IN (159, 173, 174, 172, 171)";
				
				$stSqlCarregados = "";
				mostrarComboPopup( 'Situação do grupo', 'esdid',  $sql, '', 'Selecione a(s) situações(s)' );

				//SITUACAO OBRA
				$sql = "select stoid as codigo, 
				   			   stodesc as descricao  
						from obras.situacaoobra 
						WHERE stostatus='A'";
				
				$stSqlCarregados = "";
				mostrarComboPopup( 'Situação da obra', 'stoid',  $sql, '', 'Selecione o(s)' );
				

				//ORGAO
				$sql = "select orgid as codigo, 
				   			   orgdesc as descricao  
						from obras.orgao 
						WHERE orgstatus='A'";
				
				$stSqlCarregados = "";
				mostrarComboPopup( 'Orgão da Obra', 'orgid',  $sql, '', 'Selecione o(s)' );
				
				//RESPONSAVEL SUPERVISAO
				$sql = "select rsuid as codigo, 
				   			   rsudsc as descricao  
						from obras.realizacaosupervisao";
				
				$stSqlCarregados = "";
				mostrarComboPopup( 'Responsável pela supervisão', 'rsuid',  $sql, '', 'Selecione o(s)' );
		?>
		<tr>
			<td bgcolor="#CCCCCC" width="10%"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" id="visualizar" value="Visualizar" onclick="obras_exibeRelatorioGeral('exibir');" style="cursor: pointer;"/>
			</td>
		</tr>
		<tr id="tr_resposta" >
			
			<td colspan="2" style="background-color: white;">
				<div id="div_resposta">
				</div>
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript">
<!--

jQuery(document).ready(function(){

	jQuery('#esdid').click(function(){
		
		var div_on    = document.getElementById( 'esdid_campo_on' );
		
		if(div_on.style.display == 'block'){
			
			jQuery('#filtro_periodo').show();
		}else{
			
			jQuery('#filtro_periodo').hide();	
		}

	});

	jQuery('#tr_esdid').click(function(){
		
		if(jQuery('#esdid_campo_on').css('display') == 'block'){
			
			jQuery('#filtro_periodo').show();
		}else{
			
			jQuery('#filtro_periodo').hide();
		}
	});
	
});

//-->
</script>