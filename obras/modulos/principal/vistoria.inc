<?php
unset($_SESSION['supvid']);

ini_set( "memory_limit", "100M" );

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

if( $_REQUEST["obrid"] ){
	
	// Verifica se existe a obra e se o usuário possui permissão
	include_once APPRAIZ . "www/obras/_permissoes_obras.php";
	
	// Cria a sessão com a nova obra
	session_unregister("obra");
	$_SESSION["obra"]["obrid"] = $_REQUEST["obrid"];
	
}

$obras = new Obras();
$dobras = new DadosObra(null);

// @TODO Verificar para adaptar
// Executa as funções da tela

if( $_REQUEST["subacao"] == "VerificaVistoria" ){
	
	// Verifica se existe vistorias com data maior do que a da que esta tentando excluir
	$pode_excluir_vistoria = $obras->VerificaExistenciaVistorias( $_REQUEST["supvid"] );
	$somenteLeitura = obras_podeatualizarvistoria($_REQUEST["supvid"]);

	if( $pode_excluir_vistoria && $somenteLeitura == 'S'){
		
		$boExcluida = $obras->DeletarVistoria( $_REQUEST["supvid"] );
		if ( true !== $boExcluida ){
			echo "<script>
				 alert('" . $boExcluida . "');
				 history.back(-1);
			  </script>";
		}
		
	}else{
		
		echo "<script>
				 alert('Você não possui permissão para excluir esta vistoria!');
				 history.back(-1);
			  </script>";
			
	}
	
}


if($_SESSION["obra"]["obrid"]){
	$dados = $obras->Dados($_SESSION["obra"]["obrid"]);
	$dobras = new DadosObra($dados);
} else {
	die("<script>
			alert('Problemas nas variáveis. Refaça o procedimento.');
			window.location='obras.php?modulo=inicio&acao=A';
		 </script>");      
}

?>

<br/>

<?php

$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo = "Monitoramento de Obras/Infraestrutura";
monta_titulo( $titulo_modulo, 'Vistoria' );

echo $obras->CabecalhoObras();

?>
<script src="/includes/prototype.js"></script>
<form name="formulario" id="formulario" method="post" onSubmit="return Validacao();" action="<?php echo $caminho_atual;?>acao=A"> 
	<input type="hidden" name="requisicao" id="requisicao" value="executar"/> 
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td>
			<?php
				$stSql = "
					SELECT distinct
						to_char(s.supvdt,'DD/MM/YYYY') as dtvistoria,
						to_char(s.supdtinclusao,'DD/MM/YYYY') as dtinclusao,						
						COALESCE(t.traseq || ' - ' || t.tradsc, '-') AS aditivo,
						u.usunome,
						si.stodesc,
						rs.rsudsc as responsavel,
						UPPER(e.entnome) as vistoriador,
						s.supvid,
						s.usucpf,
						e.entnumcpfcnpj as cpfvistoriador,
						s.supdtinclusao,
						coalesce((SELECT 
									sum(( icopercsobreobra * supvlrinfsupervisor ) / 100)
								  FROM 
								  	obras.itenscomposicaoobra i
								  INNER JOIN 
								  	obras.supervisaoitenscomposicao si ON i.icoid = si.icoid 
								  WHERE si.supvid = s.supvid 
								  	AND obrid = {$_SESSION['obra']['obrid']} 
								  	/*AND i.icovigente = 'A'*/ ),'0') as percentual,
						doc.esdid
					FROM
						obras.supervisao s
					LEFT JOIN(SELECT 
								DISTINCT
								t.traseq,
								t.tradsc,
								sic.supvid
							  FROM 
								obras.termoaditivo t
								JOIN obras.itenscomposicaoobra ico ON t.traid = ico.traid
								JOIN obras.supervisaoitenscomposicao sic ON sic.icoid = ico.icoid 
							  WHERE
								t.obrid = " . $_SESSION["obra"]["obrid"] . " 
							 ) t ON t.supvid = s.supvid	
					INNER JOIN 
						obras.situacaoobra si ON si.stoid = s.stoid
					INNER JOIN
						seguranca.usuario u ON u.usucpf = s.usucpf
					LEFT JOIN
						entidade.entidade e ON e.entid = s.supvistoriador
					LEFT JOIN
						obras.realizacaosupervisao rs ON rs.rsuid = s.rsuid 
					LEFT JOIN
						workflow.documento doc ON doc.docid = s.docid 
						
					--inner join
					LEFT JOIN
						obras.itenscomposicaoobra i on i.obrid = {$_SESSION["obra"]["obrid"]}
					--inner join
					LEFT JOIN
						obras.itenscomposicao ic on i.itcid = ic.itcid						
						
					WHERE
						s.obrid = '" . $_SESSION["obra"]["obrid"] . "' AND
						s.supstatus = 'A'
						
						and ( i.icovigente='A' OR i.icovigente IS NULL ) 
					
					/*ORDER BY 
						s.supdtinclusao ASC*/
					ORDER BY
						s.supdtinclusao ASC";
				
				$dados = $db->carregar( $stSql );			   
				$cabecalho = array(	"Ação",
									"Aditivo Vinculado",
									"Ordem",
									"Data Vistoria",
									"Data Inclusão",
									"Inserido Por",
									"Situação da Obra",
									"Vistoriador",
									"Realizada Por",
									"% da Vistoria"); 
			?>
			<table class="tabela" align="center" style="width: 98%;">
				<tr>
				<? $i = 1; ?>
				<? if(is_array($cabecalho)){ ?>
				<? foreach($cabecalho as $indices => $titulos){ ?>
					<th><? echo  $titulos  ?></th>
				<? }} ?>
				
				</tr>
				<?php
				if(is_array($dados)){ 
					$stBotaoExcluir = '<img src="/imagens/excluir_01.gif" style="cursor:pointer;" title="Não é possível excluir essa vistoria" border="0" onclick=" alert(\'Não é possível excluir essa vistoria\')"> ';
//					$stBotaoExcluir = '';
					$stBotaoExcluirHabilitado = '<img src="/imagens/excluir.gif" border="0" title="Excluir" style="cursor:pointer;" onclick="javascript:ExcluirVistoria(\'' . $caminho_atual . 'acao=A\', %s );">';
					$inRegistros = count( $dados );
					
					foreach($dados as $chave){ ?>
							
				<tr <? if($i%2) print("bgcolor=#f0f0f0") ?>>
					<td>
						<img src="/imagens/alterar.gif" border="0" title="Editar" style="cursor:pointer;" onclick="javascript:AtualizarVistoria('?modulo=principal/inserir_vistoria&acao=A', <?php echo $chave["supvid"]; ?>);"> 
						
						<?php
							echo ( (($chave["usucpf"] == $_SESSION['usucpf'] && $inRegistros == $i) || ($db->testa_superuser() && $inRegistros == $i) || possuiPerfil(array(PERFIL_ADMINISTRADOR)) ) && ( $chave['esdid'] != WF_ESTADO_VALIDADO )) ? sprintf( $stBotaoExcluirHabilitado, $chave['supvid'] ): $stBotaoExcluir;
						?>
						
					</td>
					<td align="center">
						<? print($chave['aditivo']); ?>
					</td>
					<td align="center">
						<? print($i); ?>
					</td>
					<td>
						<? print($chave["dtvistoria"]); ?>
					</td>
					<td>
						<? print($chave["dtinclusao"]); ?>
					</td>
					<td>
						<?php if( possuiPerfil(array(PERFIL_SUPERVISORMEC, PERFIL_EMPRESA, PERFIL_ADMINISTRADOR) ) ){ ?>
							<img border="0" onclick='envia_email("<?php echo $chave["usucpf"] ?>");' title="Enviar e-mail ao Gestor" src="../imagens/email.gif" style="cursor: pointer;"/>
						<?php } ?>
						<? print($chave["usunome"]); ?>
					</td>
					<td>
						<? print($chave["stodesc"]); ?>
					</td>
					<td>
						<?php if( !empty($chave["vistoriador"]) && ( possuiPerfil(array(PERFIL_SUPERVISORMEC, PERFIL_EMPRESA, PERFIL_ADMINISTRADOR) ) ) ){ ?>
							<img border="0" onclick='envia_email("<?php echo $chave["cpfvistoriador"] ?>");' title="Enviar e-mail ao Gestor" src="../imagens/email.gif" style="cursor: pointer;"/>
						<?php } ?>
						<? !empty($chave["vistoriador"]) ? print($chave["vistoriador"]) : print('NÃO INFORMADO'); ?>
					</td>
					<td>
						<? print($chave["responsavel"]); ?>
					</td>
					<td>
						<?php
							$percentual = $chave["percentual"];
							$percentual = $percentual > 100.00 ? 100.00 : $percentual;
							print(number_format($percentual,2,',','.')); 
						?> %
					</td>
				</tr>
				<? $i++; }} ?>
				
			</table>
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td>
				<div style="float: left;">
					<?php
						$sql = "SELECT DISTINCT 
									stoid, max(supdtinclusao) as supdtinclusao 
								FROM 
									obras.supervisao s
								inner join (SELECT max(supdtinclusao) as dtinclusao 
								FROM 
									obras.supervisao 
								WHERE 
									supstatus = 'A' AND obrid = {$_SESSION['obra']['obrid']} ) a on a.dtinclusao = 	s.supdtinclusao
								
								WHERE 
									supstatus = 'A' AND obrid = {$_SESSION['obra']['obrid']} 
								GROUP BY 
									stoid, supstatus";
					
						$dado_situacao = $db->carregar($sql);
					?>
					<?php 
					//Recuperando Dados do Aditivo.
					$obAditivo = pegaObUltimoAditivo();
					$traid 	   = $obAditivo->traid;
					$traseq    = $obAditivo->traseq;
					$arrParam  = array("traid" => $traid, "traseq" => $traseq);
					//Recupera o Valor Total das etapas do Cronograma Físico-Financeiro.
					$valorTotalCronograma = recuperaValorTotalCronograma($_SESSION['obra']['obrid'],$arrParam);
					//Recupera o Valor Total do Contrato da Obra com ou sem Aditivo.
					$valorTotalContrato   = recuperaValorTotalContrato($_SESSION['obra']['obrid'], $arrParam);
					//Se o Valor Total do Cronograma Físico-Financeiro for divergente do Valor Total do Contrato da Obra com ou sem Aditivo,
					//não será possível inserir uma nova Vistoria.
					if($valorTotalCronograma != $valorTotalContrato){
						//Desabilitando o botão para inserir uma novo Vistoria.
						$desabilitaBotao = 'disabled=\"disabled\"';
						//Mensagem que será apresentada quando os Valores do Cronograma e do Contrato forem divergentes.
						$mensagem = '&nbsp;&nbsp;&nbsp;<font style="color: red;"><b> Valor das Etapas divergente do valor do Contrato! </b></font>';
					}
					?> 
					<?php if( ($habilitado && $dado_situacao[0]['stoid'] != 3) || ( $dado_situacao[0]['stoid'] != 3 && !obraAditivoPossuiVistoria()) ){ ?>
						<input <?=$desabilitaBotao;?> style="cursor: pointer;" type="button" name="inserir_vistoria" value="Inserir Vistoria" onclick="window.location='?modulo=principal/inserir_vistoria&acao=A'" />
					<?php // era }else if( $dado_situacao[0]['stoid'] == 3 && possuiPerfil( array( PERFIL_SUPERVISORMEC, PERFIL_GESTORMEC, PERFIL_EMPRESA, PERFIL_SUPERVISORUNIDADE, PERFIL_GESTORUNIDADE ) ) ){
						  }else if( possuiPerfil( array( PERFIL_SUPERVISORMEC, PERFIL_GESTORMEC, PERFIL_EMPRESA, PERFIL_SUPERVISORUNIDADE, PERFIL_GESTORUNIDADE ) ) ){  ?>
						<input <?=$desabilitaBotao;?> style="cursor: pointer;" type="button" name="inserir_vistoria" value="Inserir Vistoria" onclick="window.location='?modulo=principal/inserir_vistoria&acao=A'" />
					<?php }else{ ?>
						<input <?=$desabilitaBotao; ?><?=(($habilitado)? "" : "disabled=\"disabled\"" );?> style="cursor: pointer;" type="button" name="inserir_vistoria" value="Inserir Vistoria" onclick="window.location='?modulo=principal/inserir_vistoria&acao=A'"/>
					<?php } ?>
					<input <?=(($habilitado)? "" : "disabled=\"disabled\"" );?> style="cursor: pointer;" type="button" name="gerar_formulario" value="Formulário de Vistoria" onclick="window.open('?modulo=principal/relatorio_supervisao_imp&acao=A','blank','height=450,width=550,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');"/>
					<?php echo $mensagem; ?>
				</div> 
			</td>
		</tr>
	</table>
</form>

<script type="text/javascript">
	function atualizar(){
		document.getElementById('requisicao').value = 'atualizarRegistros';
		document.getElementById('formulario').submit();
	}	
</script>

<?php chkSituacaoObra(); ?>