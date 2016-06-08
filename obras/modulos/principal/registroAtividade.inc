<?php
	// Cabeçalho padrão do sistema
	include  APPRAIZ."includes/cabecalho.inc";
	require_once APPRAIZ . "www/obras/permissoes.php";

	print "<br/>";
	// Monta as abas
	$db->cria_aba($abacod_tela,$url,$parametros);
	// Monta Título	
	monta_titulo( "Registro de Atividades de Monitoramento", "" );
	//função Ajax, para inativar Registros de Atividades 
	if ($_POST['ajax'] == "excluir"){
		
		$sql="UPDATE 
					obras.registroatividade
			  SET 
					rtvstatus= 'I'
  			  WHERE 	
			  		rtvid ='".$_POST['rtvid']."'";
					       	 
		$db->executar($sql);
	}
?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script language="javascript" type="text/javascript" src="./js/obras.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>

	<form name="formulario" id="formulario" method="post" action="">
	<input type="hidden" name="excluirRegistro" id="excluirRegistro" value="excluir" />
		<table id="idRegistroAtividade" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding=3 align="center">
			<tr>
				<td style="font-weight:bold; color:black; font-family:Arial,Verdana; background-color:#f0f0f0; text-align:left; " colspan="2">
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Argumentos de Pesquisa
				</td>
			</tr>
			
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Data da Criação:</td>
				<td>
					<?php 
					echo "Início " . campo_data('rtvdtinclusaoinicio', 'N', $somenteLeitura, '', 'S');
					echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
					echo "Fim " . campo_data('rtvdtinclusaofim', 'N', $somenteLeitura, '', 'S' );												
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Usuário:</td>
				<td>
					<?php 
					echo campo_texto( 'usunome', 'N', $somenteLeitura, '', 47, 60, '', '', 'left', '', 0, '');
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Descrição simplificada:</td>
				<td>
					<?php 
					echo campo_texto( 'rtvdscsimplificada', 'N', $somenteLeitura, '', 47 , 60, '', '', 'left', '', 0, '');	
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Descrição detalhada:</td>
				<td>
					<?php 
					echo campo_texto( 'rtvdsccompleta', 'N', $somenteLeitura, '' , 47 , 60, '', '', 'left', '', 0, '');	
					?>
				</td>
			</tr>
			<tr bgcolor="#D0D0D0">
				<td style="width: 190px;"></td>
				<td>
					<input type="button" name="visualizarRegAtividades"  <?php if($somenteLeitura=="N") echo "disabled"; ?>  value="Visualizar" onclick="this.disabled=true;document.getElementById('formulario').submit();" style="cursor: pointer;"/>
					<input type="button" name="verTodosRegAtividades"  <?php if($somenteLeitura=="N") echo "disabled"; ?>  value="Ver Todas" onclick="this.disabled=true;window.location='?modulo=principal/registroAtividade&acao=A'" style="cursor: pointer;"/>
				</td>
			</tr>
	</table>
</form>
	
	
	<?php 
	//Condições para consultas
	
	//Filtro da consulta por Data inicio da inclusão do Registro
	if($_REQUEST['rtvdtinclusaoinicio']) {
		$filtroprocesso[] = "rg.rtvdtinclusao >= '". formata_data_sql($_REQUEST['rtvdtinclusaoinicio'])."'";
	}
	//Filtro da consulta por Data fim da inclusão do Registro
	if($_REQUEST['rtvdtinclusaofim']) {
		$filtroprocesso[] = "rg.rtvdtinclusao <= '". formata_data_sql($_REQUEST['rtvdtinclusaofim'])."'";
	}
	//Filtro da consulta por Nome do usuário responsável 
	if($_REQUEST['usunome']){
		$filtroprocesso[] = "u.usunome ILIKE '%".strtoupper($_REQUEST['usunome'])."%'";
	}
	//Filtro da consulta por Descrição Simplificada
	if($_REQUEST['rtvdscsimplificada']){
		$filtroprocesso[] = "rg.rtvdscsimplificada ILIKE '%".$_REQUEST['rtvdscsimplificada']."%'";
	}
	//Filtro da consulta por Descrição detalhada
	if($_REQUEST['rtvdsccompleta']) {
		$filtroprocesso[] = "rg.rtvdsccompleta ILIKE '%".$_REQUEST['rtvdsccompleta']."%'";
	}
			//Consulta principal da Tabela obras.registroatividade 
			if($somenteLeitura == "N"){
				$botoes  = "<img src=\"/imagens/excluir_01.gif\" style=\"cursor: pointer\"  border=0 title=\"Excluir Registro\">&nbsp";
				$botoes .= "<img src=\"/imagens/alterar_01.gif\" style=\"cursor: pointer\"  border=0 title=\"Alterar Registro\">";
			}else{
				$botoes  = "<img src=\"/imagens/alterar.gif\" style=\"cursor:pointer;\" border=0 title=\"Alterar Registro\" onclick=\"janela(\'?modulo=principal/popUpRegistrodeAtividades&acao=A&rtvid=' || rg.rtvid || '&rtvid=' || rg.rtvid || '\', 800, 400, \'popUpRegistrodeAtividades\');\">&nbsp";
				$botoes .= "<img src=\"/imagens/excluir.gif\" style=\"cursor:pointer;\" border=0 title=\"Excluir Registro\" onclick=\"excluiRegistro('||  rg.rtvid ||');\">";
			}
	
	
			$sql="SELECT 
					'<center>{$botoes}</center>' as acao,
					rg.rtvid AS sequencia, 
					rg.rtvdscsimplificada AS descricao, 
					to_char(rg.rtvdtinclusao , 'DD/MM/YYYY - HH24:mm:ss' ) AS data,
					u.usunome AS inserido_por 
			  	FROM 
					obras.registroatividade AS rg
			  	INNER JOIN
					seguranca.usuario u
					ON 
					u.usucpf = rg.usucpf
				WHERE 
					rg.obrid = ".$_SESSION["obra"]["obrid"]." 
					AND				
					rg.rtvstatus = 'A'
					".( ( count ( $filtroprocesso ) > 0 )? " 
					AND  
					" . implode( " AND " , $filtroprocesso ) : " " ) ."
				ORDER BY
					rg.rtvid";
			
			
			//cabeçalho da lista
			$cabecalho = array( "Ação", "Seq.", "Descrição", "Data","Inserido por" );
			//lista de Registros de Atividades
			$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', 'center', '','', '', '');

	?>
	<?php if($habilitado){ ?>
		<table class="tabela" bgcolor="#FFFFFF" cellspacing="1" cellpadding=3 align="center">
			<tr>
				<td style="font-weight: bold;">
					<a style="cursor: pointer;" onclick="javascript:popupRegAtividades();" title="Clique para inserir um novo Registro de Atividades">
					<img src="../imagens/obras/incluir.png" style="width: 15px; vertical-align: middle;"/> Inserir novo 				</a>
				</td>
			</tr>
		</table>
	<?php } ?>
	
<script>

//Função que abre a popup para inclusão
function popupRegAtividades(){
	   open("?modulo=principal/popUpRegistrodeAtividades&acao=A","DefaultWindow","width=800,height=400,toolbar=no,scrollbars=yes,status=yes");
	 }

//Função que inativa o Registro de Atividade
function excluiRegistro(idRegistro){
	//Componente que apresenta uma imagem Carregando
	divCarregando();
		if(confirm('Realmente deseja excluir este registro?')){
			var rtvid = idRegistro;
			var url = "?modulo=principal/registroAtividade&acao=A"; 
			new Ajax.Request(url,{
				method: 'post',
				parameters: '&ajax=excluir&rtvid=' + rtvid,
				onComplete: function(res){
					alert('Registro excluido com sucesso!');
					window.location.href='?modulo=principal/registroAtividade&acao=A';
				}
			});
		}
	divCarregado();

}
</script>	
