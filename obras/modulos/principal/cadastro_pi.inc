<?php
require_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";
require_once APPRAIZ . "monitora/classes/Pi_PlanoInternoHistorico.class.inc";

if($_POST['requisicao'] == 'vincular'){
	
	extract($_POST);
	
	$retorno = false;
	
	$obPi_PlanoInterno = new Pi_PlanoInterno($pliid);
	$obPi_PlanoInterno->plisituacao = $situacao;
	$obPi_PlanoInterno->salvar();
	$sql = "SELECT plicod FROM monitora.pi_planointernohistorico WHERE pliid = $pliid ORDER BY pihdata DESC LIMIT 1";
	if(!$plicodOrigem = $db->pegaUm($sql)){
		$plicodOrigem = $obPi_PlanoInterno->plicod;	
	}
	
	$obPi_PlanoInternoHistorico = new Pi_PlanoInternoHistorico();
	$obPi_PlanoInternoHistorico->pliid 		  = $pliid;
	$obPi_PlanoInternoHistorico->usucpf 	  = $_SESSION['usucpf'];
	$obPi_PlanoInternoHistorico->pihsituacao  = $situacao;
	$obPi_PlanoInternoHistorico->plicod 	  = $obPi_PlanoInterno->plicod;
	$obPi_PlanoInternoHistorico->plicodorigem = $plicodOrigem;
	$obPi_PlanoInternoHistorico->salvar();
	
	if($obPi_PlanoInternoHistorico->commit()){
		enviaEmailStatusPi($pliid);
		$retorno = true;
	}
	unset($obPi_PlanoInterno);
	unset($obPi_PlanoInternoHistorico);
	
	echo $retorno;
	die;
	
}

function validaCodPi($pi, $pliid = false){
	global $db;
	
	$sql = "SELECT plicod FROM monitora.pi_planointerno WHERE plistatus='A' AND pliano = '{$_SESSION['exercicio']}' AND plicod = '{$pi}'".(($pliid)?" AND pliid != '".$pliid."'":"");
	 
	$plicod = $db->PegaUm($sql);
	
	if(!$plicod) {
		# Comentando a pedido do Henrique, no dia 04/03/2010
		/*$piaux = substr($pi, 0, 6);
		$sql = "SELECT DISTINCT plicod, plititulo FROM monitora.pi_planointerno WHERE plistatus='A' AND plicod like '%{$piaux}%' ".(($pliid)?" AND pliid != '".$pliid."'":"")." ORDER BY plititulo";
		$dados = (array) $db->carregar($sql);
		if($dados[0]){
			$cabecalho = array('Cód PI','Título');
			$retorno = $db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '' );
			echo $retorno;
			exit;
		}
		else{*/
			$retorno = "";
			echo $retorno;
			exit;
		//}
	} else {
		$retorno = "pijaexiste";
		echo $retorno;
		$sql = "SELECT p.plicod as plicod, coalesce(p.plititulo,'Não preenchido') as titulo, 
				coalesce(SUM(pp.pipvalor),0) as total, 
				CASE WHEN p.plisituacao = 'P' THEN ' Pendente ' WHEN p.plisituacao = 'A' THEN ' Aprovado ' 
				     WHEN p.plisituacao = 'H' THEN ' Homologado ' WHEN p.plisituacao = 'R' THEN ' Revisado ' 
				     WHEN p.plisituacao = 'S' THEN ' Cadastrado no SIAFI ' WHEN p.plisituacao = 'E' THEN ' Enviado para Revisão ' END as situacao, 
				u.usunome ||' por '||to_char(p.plidata, 'dd/mm/YYYY hh24:mi'), 
				COALESCE(a._atinumero||' - '||a.atidescricao, 'Não atribuido')as atividade
				FROM monitora.pi_planointerno p 
				LEFT JOIN monitora.pi_planointernoptres pp ON  pp.pliid=p.pliid
				LEFT JOIN seguranca.usuario u ON u.usucpf = p.usucpf 
				LEFT JOIN monitora.pi_planointernoatividade pa on pa.pliid = p.pliid 
				LEFT JOIN pde.atividade a on a.atiid = pa.atiid 
				WHERE p.plicod='".$plicod."' AND p.plistatus = 'A' 
				GROUP BY p.plicod,p.plititulo,u.usunome,p.plidata,p.plisituacao,atividade 
				ORDER BY p.plidata DESC";
		$cabecalho = array("Código PI","Título","Total PI","Situação","Dados inserção","Atividade");
		$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '' );
		exit;
	}
}


function buscaDadosSubacao($sbaid) {
	global $db;
	
	$sql = "SELECT * FROM monitora.pi_subacao WHERE sbaid='".$sbaid."'";
	$subacao = $db->pegaLinha($sql);
	
	
	if($_SESSION['obra']['obrid']) {
		$sql = "SELECT en.entsig FROM obras.obrainfraestrutura ob 
				LEFT JOIN entidade.entidade en ON en.entid = ob.entidunidade 
				WHERE ob.obrid='".$_SESSION['obra']['obrid']."'";
		
		$entsig = $db->pegaUm($sql);
		
	}
	
	echo $subacao['sbacod']."!@#".trim($subacao['sbatitulo']).(($entsig)?"-".$entsig:"");
}


require_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";
require_once APPRAIZ . "monitora/classes/Pi_PlanoInternoHistorico.class.inc";

//valida cod pi AJAX
if ($_REQUEST['piAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	validaCodPi($_POST['piAjax'], $_POST['pliid']);
	exit;
}

//valida cod pi AJAX
if ($_REQUEST['sbaAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	buscaDadosSubacao($_POST['sbaAjax'], '');
	exit;
}

// controlador do numero sequencial do gerador
include APPRAIZ."/includes/controlegeradorsequenciapi.inc";
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
$db->cria_aba($abacod_tela,$url,'');

$_SESSION['obra']['obrid'] = $_SESSION['obra']['obrid'] ? $_SESSION['obra']['obrid'] : $_REQUEST['obrid'];

/* Selecionando a unidade da obra */
if($_SESSION['obra']['obrid']){
	$boExisteObrid = $db->pegaUm("select oie.obrid from obras.obrainfraestrutura oie where oie.obrid = {$_SESSION['obra']['obrid']}");
	
	if( !$boExisteObrid ){
		echo "<script>
				alert('A Obra informada não existe!');
				history.back(-1);
			  </script>";
		die;
	}
	
	$unicod = $db->pegaUm("select e.entunicod from obras.obrainfraestrutura oie inner join entidade.entidade e ON oie.entidunidade = e.entid where oie.obrid = {$_SESSION['obra']['obrid']}");
	if(!$unicod) die("<script>alert('Unidade não encontrada');history.back(-1);</script>");
} else {
	die("<script>
			alert('Variaveis de obra não identificadas.');
			window.location='obras.php?modulo=inicio&acao=A';
		 </script>");
}
$boSelecionaSubacao = false;

if(!$_REQUEST['anoexercicio']) {
	$_REQUEST['anoexercicio'] = '2011';
}

$pliano = $_REQUEST['anoexercicio'];

$boPerfilSomenteLeitura = false;
if (!$db->testa_superuser()) {
	if(possuiPerfil( array(PERFIL_CONSULTAGERAL,PERFIL_CONSULTAESTADUAL,PERFIL_CONSULTATIPOENSINO,PERFIL_CONSULTAUNIDADE) ) && !possuiPerfil( array(PERFIL_SUPERVISORUNIDADE,PERFIL_GESTORUNIDADE,PERFIL_SUPERVISORMEC,PERFIL_EMPRESA,PERFIL_ADMINISTRADOR,PERFIL_GESTORMEC) )){
		$boPerfilSomenteLeitura = true;
		$disabled = " disabled='disabled' ";
	}
	else{
		$boPerfilSomenteLeitura = false;
		$disabled = "";
	}
}

$permissao_formulario = (!$boPerfilSomenteLeitura) ? 'S' : 'N'; # S habilita e N desabilita o formulário

// verifica se ocorre algum evento
if(isset($_POST['evento']) && ($_POST['evento'] != '') ) {
	
	if($_REQUEST['combo_sbaid']) {
		$_REQUEST['sbaid'] = $_REQUEST['combo_sbaid']; 	
	}
	switch($_POST['evento']) {
		// atualizar os dados do plano interno
		case "G":
			/*
			 * VERIFICAÇÃO SE CÓDIGO DO PI FOI ALTERADO,
			 * CASO SIM, GERAR NOVO CONTROLE
			 */
			if($_REQUEST['sbaid']) {
				$plicodsubacao = '';
				$sbacod = $db->pegaUm("SELECT sbacod FROM monitora.pi_subacao WHERE sbaid='".$_REQUEST['sbaid']."'");
			} else {
				$plicodsubacao = $_REQUEST['plicodsubacao'];
				$sbacod = $_REQUEST['plicodsubacao'];
				
			}
			
			/* if($_POST['neeid']!=$_POST['neeid_'] ||
			   $_POST['capid']!=$_POST['capid_']) {
 
				$eqdcod = $db->pegaUm("SELECT eqdcod FROM monitora.pi_enquadramentodespesa WHERE eqdid='".$_POST['eqdid']."'");
				$sbacod = $db->pegaUm("SELECT sbacod FROM monitora.pi_subacao WHERE sbaid='".$_REQUEST['sbaid']."'");
				$neecod = $db->pegaUm("SELECT neecod FROM monitora.pi_niveletapaensino WHERE neeid='".$_POST['neeid']."'");
				$capcod = $db->pegaUm("SELECT capcod FROM monitora.pi_categoriaapropriacao WHERE capid='".$_POST['capid']."'");
				
				$identificador = $eqdcod.$sbacod.$neecod.$capcod;
				$seq = $db->pegaUm("SELECT gspseq FROM public.geradorsequencialpi WHERE gspidentificador = '".$identificador."' ORDER BY gspid DESC");
				if($seq) {
					$gspseq = retornaseq(substr($seq, -3));
					$gspseq = str_pad($gspseq, 4, "0", STR_PAD_LEFT);
				} else {
					$gspseq = "0001";
				}
				$sql = "INSERT INTO geradorsequencialpi(gspseq, gspidentificador)
		    			VALUES ('".$gspseq."', '".$identificador."');";
				$db->executar($sql);
				$db->commit();
				
				// 	No caso da subação retorna apenas os dois últimos campos do código gerado.
				$gerador = substr($gspseq, -3);
				$plicodnovo = $eqdcod.$sbacod.$neecod.$capcod.strtoupper($gerador);
				$sql = "UPDATE monitora.pi_planointerno SET plicod='".$plicodnovo."' WHERE plicod='".$_POST['plicod']."'";
				$db->executar($sql);
				$_POST['plicod'] = $plicodnovo;
			} */
			
			$db->executar("DELETE FROM monitora.pi_planointernoptres WHERE pliid='".$_REQUEST['pliid']."'");
			
			// reinserindo os que ja existiam
			if($_POST['plivalored']) {
				foreach($_POST['plivalored'] as $ptrid => $valor) {
					$valor = str_replace(array(".",","), array("","."), $valor);
					$sql = "INSERT INTO monitora.pi_planointernoptres(
				            pliid, ptrid, pipvalor)
				    		VALUES ('".$_REQUEST['pliid']."', '".$ptrid."', '".$valor."');";
					$db->executar($sql);
				}
			}
			
			// inserindo novos
			if(is_array($_POST['acaid'])) {
				foreach($_POST['acaid'] as $ptres => $ptrid) {
					$valor = $_POST['plivalor'][$ptres][$ptrid] ? $_POST['plivalor'][$ptres][$ptrid] : 'null';		
					$valor = str_replace(array(".",","), array("","."), $valor);
					$sql = "INSERT INTO monitora.pi_planointernoptres(
				            pliid, ptrid, pipvalor)
				    		VALUES ('".$_REQUEST['pliid']."', '".$ptrid."', '".$valor."');";
					
					$db->executar($sql);

					
				}
			}
			
			$plicodorigem = $db->pegaUm("SELECT plicod FROM monitora.pi_planointerno WHERE pliid='".$_REQUEST['pliid']."'");
			 
			$qry = "INSERT INTO monitora.pi_planointernohistorico(
            	 	plicod, pihobs, pihdata, usucpf, pihsituacao, plicodorigem)
    			 	VALUES ('".$_POST['plicod']."', NULL, NOW(), '".$_SESSION['usucpf']."', 'P', '".$plicodorigem."')";
			
			//$db->executar($qry);
			
			$existe_plicod = $db->pegaUm("SELECT pliid FROM monitora.pi_planointerno WHERE plistatus='A' AND plicod='".$plicodnew."'");
			if($existe_plicod) die("<script>alert('O Código do PI ja existe.');window.location='?modulo=principal/cadastro_pi&acao=A';</script>");
			
			
			$sql = "UPDATE monitora.pi_planointerno SET plicod='".$_POST['plicod']."', 
													    mdeid=".(($_POST['mdeid'])?"'".$_POST['mdeid']."'":"NULL").",
													    eqdid='".$_POST['eqdid']."', 
													    neeid='".$_POST['neeid']."', 
													    capid='".$_POST['capid']."', 
													    plilivre='".$_POST['plilivre']."', 
													    plidsc='".$_POST['plidsc']."', 
													    usucpf='".$_SESSION['usucpf']."', 
													    plicodsubacao='".$plicodsubacao."', 
													    plidata='".date("Y-m-d")."', 
													    plititulo='".$_POST['plititulo']."' WHERE pliid='".$_REQUEST['pliid']."'";
			
			$db->executar($sql);
			
			$db->commit();
			
			//die("<script>alert('Dados gravados com sucesso');window.location='monitora.php?modulo=principal/cadastro_pi&acao=A';</script>");
			die("<script>alert('Dados salvos com sucesso.');window.location = '?modulo=principal/cadastro_pi&acao=A&obrid=".$_POST['obrid']."';</script>");
			
		break;
		
		// carregar os dados do pi
		case 'A':
			$planointerno = $db->pegaLinha("SELECT * FROM monitora.pi_planointerno pi 
											LEFT JOIN monitora.pi_subacao sb ON sb.sbaid = pi.sbaid 
											WHERE pliid = '".$_POST['pliid']."'");
			if($planointerno) {
				$pliid = $planointerno['pliid'];
				$plicod   = $planointerno['plicod'];
				$plititulo = trim($planointerno['plititulo']);
				$plisituacao = $planointerno['plisituacao'];
				$plidsc = $planointerno['plidsc'];
				$_REQUEST['sbaid'] = $planointerno['sbaid'];
				$plititulo_sba = trim($planointerno['sbatitulo']).' - ';
				$sbaidAtual = $_REQUEST['sbaid']; 
				
				$mdeid = $planointerno['mdeid'];
				$eqdid = $planointerno['eqdid'];
				$neeid = $planointerno['neeid'];
				$capid = $planointerno['capid'];
				$plilivre = $planointerno['plilivre'];
				$pliano   = $planointerno['pliano'];
				$plicodsubacao   = $planointerno['plicodsubacao'];
				
				$sql = "SELECT
							pl.pliid,
							ptr.ptres,
							pt.ptrid,
							pt.pipvalor, 
							ptr.acaid,
							trim(ac.prgcod||'.'||ac.acacod||'.'||ac.unicod||'.'||ac.loccod||' - '||ac.acadsc) as descricao,
							ptr.ptrdotacao as dotacaoinicial,
							round(sum( coalesce(sad.sadvalor,0) ),2) as dotacaosubacao,
							coalesce(sum(dt2.valorpi),0.00) as detalhamento
						FROM monitora.pi_planointerno pl
						INNER JOIN monitora.pi_planointernoptres pt ON pt.pliid = pl.pliid 
						LEFT JOIN monitora.ptres ptr ON ptr.ptrid = pt.ptrid 
						LEFT JOIN monitora.acao ac ON ac.acaid = ptr.acaid
						LEFT JOIN monitora.pi_subacaodotacao sad ON ptr.ptrid = sad.ptrid and sad.sbaid = pl.sbaid
						LEFT JOIN ( select pi.sbaid, ptres.ptrid, ptrano, coalesce(sum(pipvalor),0) as valorpi 
								from monitora.pi_planointerno pi 
								inner join monitora.pi_planointernoptres pip ON pip.pliid = pi.pliid 
								inner join monitora.ptres ptres ON ptres.ptrid = pip.ptrid 
								where pi.plistatus = 'A' 
								group by pi.sbaid, ptres.ptrid, ptrano  ) dt2 ON ptr.ptrid = dt2.ptrid 
						WHERE
								pl.pliid = '".$pliid."' AND
								pl.plistatus='A' AND
								ac.unicod = '".$unicod."'
				    	GROUP BY pl.pliid, pt.ptrid, ptr.ptres, pl.plistatus, pt.pipvalor, ac.prgcod, ptr.acaid, ac.acacod, ac.unicod, ac.loccod, ac.acadsc, ptr.ptrdotacao
						
						union all
						
						
						SELECT
							pl.pliid,
							ptr.ptres,
							pt.ptrid,
							pt.pipvalor, 
							ptr.acaid,
							trim(ac.prgcod||'.'||ac.acacod||'.'||ac.unicod||'.'||ac.loccod||' - '||ac.acadsc) as descricao,
							ptr.ptrdotacao as dotacaoinicial,
							round(sum( coalesce(sad.sadvalor,0) ),2) as dotacaosubacao,
							coalesce(sum(dt2.valorpi),0.00) as detalhamento
						FROM monitora.pi_planointerno pl
						INNER JOIN monitora.pi_planointernoptres pt ON pt.pliid = pl.pliid 
						LEFT JOIN monitora.ptres ptr ON ptr.ptrid = pt.ptrid 
						LEFT JOIN monitora.acao ac ON ac.acaid = ptr.acaid
						LEFT JOIN monitora.pi_subacaodotacao sad ON ptr.ptrid = sad.ptrid 
						LEFT JOIN ( select pi.sbaid, ptres.ptrid, ptrano, coalesce(sum(pipvalor),0) as valorpi 
								from monitora.pi_planointerno pi 
								inner join monitora.pi_planointernoptres pip ON pip.pliid = pi.pliid 
								inner join monitora.ptres ptres ON ptres.ptrid = pip.ptrid 
								where pi.plistatus = 'A' 
								group by pi.sbaid, ptres.ptrid, ptrano  ) dt2 ON ptr.ptrid = dt2.ptrid 
						WHERE
								pl.pliid = '".$pliid."' AND
								pl.plistatus='A' AND
								ac.unicod = '26101'
				    	GROUP BY pl.pliid, pt.ptrid, ptr.ptres, pl.plistatus, pt.pipvalor, ac.prgcod, ptr.acaid, ac.acacod, ac.unicod, ac.loccod, ac.acadsc, ptr.ptrdotacao
				    	 	";

				$acoespl = $db->carregar($sql);
				
				$boSelecionaSubacao = true;
				
				# S habilita e N desabilita o formulário
				//$permissao_formulario = 'N';
				$disabled = " disabled='disabled' ";
				if($permissao_formulario == 'S'){
					if($plisituacao == 'P' || $plisituacao == 'R' || $plisituacao == 'E'){
						$permissao_formulario = 'S';
						$disabled = "";
	
					}
				}
				
			} else {
				
				die("<script>alert('Plano interno não encontrado');window.location='?modulo=principal/cadastro_pi&acao=A';</script>");

			}
			
		break;
		// inserir plano interno
		case 'I':
			
			$eqdcod = $db->pegaUm("SELECT eqdcod FROM monitora.pi_enquadramentodespesa WHERE eqdid='".$_POST['eqdid']."'");
			
			if (!$db->testa_superuser()) {
				if ( possuiPerfil( array( PERFIL_SUPERVISORMEC, PERFIL_EMPRESA, PERFIL_GESTORMEC, PERFIL_ADMINISTRADOR )  )) {
					$unidade_pi = ADM;
					$codlivre = '';
					$filtro2 = " sbaid = ".(($_REQUEST['sbaid'])?"'".$_REQUEST['sbaid']."'":"NULL")." and capid = ".$_POST['capid']." and ";
				}
				elseif  ( possuiPerfil( array( PERFIL_SUPERVISORUNIDADE, PERFIL_GESTORUNIDADE )  )) {
					$unidade_pi = $db->pegaUm("SELECT entidunidade FROM obras.obrainfraestrutura WHERE obrid='".$_SESSION['obra']['obrid']."'");
					$filtroUn = " obrid='".$_SESSION['obra']['obrid']."' AND ";
					$sql = "SELECT codlivre  as plilivre FROM obras.unidadeobrasubacao WHERE {$filtroUn} entidunidade='".$unidade_pi."'";
					$codlivre = $db->pegaUm($sql);
				}
			}
			else {
					$unidade_pi = ADM;
					$codlivre = '';
					$filtro2 = " sbaid = ".(($_REQUEST['sbaid'])?"'".$_REQUEST['sbaid']."'":"NULL")." and capid = ".$_POST['capid']." and ";
			}

			if($_REQUEST['sbaid']) {
				
				$sbacod = $db->pegaUm("SELECT sbacod FROM monitora.pi_subacao WHERE sbaid='".$_REQUEST['sbaid']."'");
				$plicodsubacao = '';

				//$sql = "SELECT codlivre FROM obras.unidadeobrasubacao WHERE obrid='".$_SESSION['obra']['obrid']."' AND entidunidade='".$unidade_pi."'";
				//$codlivre = $db->pegaUm($sql);
				
				if(!$codlivre) {
					
					$sql = "SELECT CASE WHEN MAX(CAST(codlivre as integer)) IS NULL THEN 1 ELSE MAX(CAST(codlivre as integer))+1 END as plilivre FROM obras.unidadeobrasubacao WHERE {$filtro2} entidunidade='".$unidade_pi."'";
					$codlivre = $db->pegaUm($sql);
					if ( $unidade_pi == ADM ) {
						$sql = "INSERT INTO obras.unidadeobrasubacao(entidunidade, obrid, codlivre, sbaid, capid)
	    						VALUES ('".$unidade_pi."', '".$_SESSION['obra']['obrid']."', '".sprintf("%03d", $codlivre)."', '".$_REQUEST['sbaid']."', '".$_POST['capid']."' );";
					} else
					{
						$sql = "INSERT INTO obras.unidadeobrasubacao(entidunidade, obrid, codlivre)
	    						VALUES ('".$unidade_pi."', '".$_SESSION['obra']['obrid']."', '".sprintf("%03d", $codlivre)."');";
					}
					$db->executar($sql);
					$db->commit();
					
				}
				
				$gerador = sprintf("%03d", $codlivre);
								
			} else {
				
				$sbacod  = $_REQUEST['plicodsubacao'];
				$plicodsubacao  = $_REQUEST['plicodsubacao'];
				
				//$sql = "SELECT codlivre FROM obras.unidadeobrasubacao WHERE obrid='".$_SESSION['obra']['obrid']."' AND entidunidade='".$unidade_pi."'";
				//$codlivre = $db->pegaUm($sql);
				
				if(!$codlivre) {
					$sql = "SELECT CASE WHEN MAX(CAST(codlivre as integer)) IS NULL THEN 1 ELSE MAX(CAST(codlivre as integer))+1 END as plilivre FROM obras.unidadeobrasubacao WHERE entidunidade='".$unidade_pi."'";
					$codlivre = $db->pegaUm($sql);
					$sql = "INSERT INTO obras.unidadeobrasubacao(entidunidade, obrid, codlivre)
    						VALUES ('".$unidade_pi."', '".$_SESSION['obra']['obrid']."', '".sprintf("%03d", $codlivre)."');";
					$db->executar($sql);
					$db->commit();
					
				}
				
				$gerador = sprintf("%03d", $codlivre); 
			}
			

			$neecod = $db->pegaUm("SELECT neecod FROM monitora.pi_niveletapaensino WHERE neeid='".$_POST['neeid']."'");
			$capcod = $db->pegaUm("SELECT capcod FROM monitora.pi_categoriaapropriacao WHERE capid='".$_POST['capid']."'");
			

			$plicod = $eqdcod.$sbacod.$neecod.$capcod.$gerador;
			
			// verificando a se existe algum plicod no BD
			$existe_plicod = $db->pegaUm("SELECT pliid FROM monitora.pi_planointerno WHERE plistatus='A' AND plicod='".$_POST['plicod']."' AND unicod = '".$_REQUEST['unicod']."' ");
			if($existe_plicod) {
				die("<script>alert('Não foi possível criar o Plano Interno. Código já existente.');window.location='?modulo=principal/cadastro_pi&acao=A';</script>");
			}
			
			$sql_I = "INSERT INTO monitora.pi_planointerno ( plicod, 
														  mdeid, 
														  eqdid, 
														  neeid, 
														  capid, 
														  plilivre, 
														  plidsc, 
														  usucpf, 
														  plidata, 
														  plititulo, 
														  plistatus,
														  plisituacao,
														  sbaid,
														  unicod,
														  plicodsubacao,
														  pliano ) 
     			  	  VALUES ('".$plicod."', 
     			  	  		  ".(($_POST['mdeid'])?"'".$_POST['mdeid']."'":"NULL").", 
     			  	  		  '".$_POST['eqdid']."', 
     			  	  		  '".$_POST['neeid']."', 
     			  	  		  '".$_POST['capid']."', 
     			  	  		  upper('".$gerador."'), 
     			  	  		  '".$_POST['plidsc']."', 
     			  	  		  '".$_SESSION['usucpf']."',
     			  	  		  '".date("Y-m-d")."',
     			  	  		  '".$_POST['plititulo']."',
     			  	  		  'A',
     			  	  		  'P',
     			  	  		  ".(($_REQUEST['sbaid'])?"'".$_REQUEST['sbaid']."'":"NULL").",
     			  	  		  '".$_REQUEST['unicod']."',
     			  	  		  '".$plicodsubacao."',
     			  	  		  '".date("Y")."'
     			  	  		  )
     			  	  RETURNING pliid";	
			$pliid = $db->pegaUm($sql_I);
			
			$qry = "INSERT INTO monitora.pi_planointernohistorico(pliid, plicod, pihobs, pihdata, usucpf, pihsituacao, plicodorigem)
    			    VALUES ('".$pliid."','".$plicod."', NULL, NOW(), '".$_SESSION['usucpf']."', 'P', '".$_POST['plicod']."')";
			$db->executar($qry);
			
			
			$qry = "INSERT INTO monitora.pi_obra(pliid, obrid)
    			    VALUES ('".$pliid."','".$_SESSION['obra']['obrid']."')";
			$db->executar($qry);
			

			if(is_array($_POST['acaid'])) {
				foreach($_POST['acaid'] as $ptres => $ptrid) {
					$valor = $_POST['plivalor'][$ptres][$ptrid] ? $_POST['plivalor'][$ptres][$ptrid] : 'null';		
					$valor = str_replace(array(".",","), array("","."), $valor);
					$sql = "INSERT INTO monitora.pi_planointernoptres(pliid, ptrid, pipvalor)
				    		VALUES ('".$pliid."', '".$ptrid."', '".$valor."');";
					$db->executar($sql);
					
				}
			}
			
			$db->commit();
			
			die("<script>alert('Dados salvos com sucesso.');window.location = '?modulo=principal/cadastro_pi&acao=A&obrid=".$_POST['obrid']."';</script>");				
		break;
		case 'E':
			$sql_D = "UPDATE monitora.pi_planointerno SET plistatus = 'I' where pliid = '".$_POST['pliid']."'";
			$db->executar($sql_D);
			$db->commit();
			die("<script>alert('Registro removido com sucesso.');window.location = '?modulo=principal/cadastro_pi&acao=A';</script>");
		break;
	}
} elseif($_REQUEST['evento']=='R') {
	$db->executar("DELETE FROM monitora.pi_obra WHERE pliid='".$_REQUEST['pliid']."' AND obrid='".$_REQUEST['obrid']."'");
	$db->commit();
	die("<script>alert('Registro removido com sucesso.');window.location = 'obras.php?modulo=principal/cadastro_pi&acao=A&obrid=".$_SESSION['obra']['obrid']."&anoexecicio=2009';</script>");
}

//echo montarAbasArray(carregardadosplanotrabalhoUN_raiz(), "/monitora/monitora.php?modulo=principal/planotrabalhoUN/cadastro_piUN&acao=A".(($_REQUEST['sbaid'])?"&sbaid=".$_REQUEST['sbaid']:""));
monta_titulo("Subação/PI",'<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');

?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<script>
function titulopi() {
	if(document.getElementById('plititulo_sub').value != '') {
		document.getElementById('plititulo').value = document.getElementById('plititulo_sub').value + document.getElementById('plititulo').value.substr(document.getElementById('plititulo_sub').value.length);
	}
	return true;
}

//coloca tabindex no campo valor
function tabindexcampo(){
	var x = document.getElementsByTagName("input");
	var y = 1;
	for(i=0;i<x.length;i++) {
		if(x[i].type=="text"){
			if(x[i].name.substr(0,8) == 'plivalor'){
				x[i].tabIndex=y;
				y++;
			}
		}
	}
}
</script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<form method="POST"  name="formulario" id="formulario">
<input type="hidden" name="evento" id="evento" value="<? echo (($_POST['pliid'])?"G":"I"); ?>">
<input type="hidden" name="plicod" id="plicod" value="<?=$plicod;?>">
<input type="hidden" name="pliid"  id="pliid" value="<?=$_POST['pliid'];?>">
<input type="hidden" name="obrid"  id="obrid" value="<?=$_SESSION['obra']['obrid'];?>">
<input type="hidden" name="unicod" id="unicod" value="<?=$unicod;?>">
<input type="hidden" name="sbaidAnterior"  id="sbaidAnterior" value="">
<input type="hidden" name="sbaidAtual"  id="sbaidAtual" value="<?php echo $sbaidAtual;?>">
<?
if($_REQUEST['req'] == 'vincular') {
	
	$anos= array(0 => array("id" => 1, "descricao" => "2009", "link" => "/obras/obras.php?modulo=principal/cadastro_pi&acao=A&obrid=".$_REQUEST['obrid']."&anoexercicio=2009&req=vincular"),
				 1 => array("id" => 2, "descricao" => "2010", "link" => "/obras/obras.php?modulo=principal/cadastro_pi&acao=A&obrid=".$_REQUEST['obrid']."&anoexercicio=2010".(($_REQUEST['req'])?"&req=".$_REQUEST['req']:"")),
				 2 => array("id" => 3, "descricao" => "2011", "link" => "/obras/obras.php?modulo=principal/cadastro_pi&acao=A&obrid=".$_REQUEST['obrid']."&anoexercicio=2011".(($_REQUEST['req'])?"&req=".$_REQUEST['req']:""))
				 );
	echo "<br />";				 
	echo montarAbasArray($anos, "/obras/obras.php?modulo=principal/cadastro_pi&acao=A&obrid=".$_REQUEST['obrid']."&anoexercicio=".$_REQUEST['anoexercicio']."&req=vincular");
?>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<? if($_REQUEST['anoexercicio'] == '2010') { ?>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="radio" name="req" onclick="if(this.checked){window.location='obras.php?modulo=principal/cadastro_pi&acao=A&obrid=<? echo $_SESSION['obra']['obrid']; ?>&anoexercicio=<? echo $_REQUEST['anoexercicio']; ?>';}"> Criar &nbsp;&nbsp;&nbsp;&nbsp; <input type="radio" name="req" onclick="if(this.checked){window.location='obras.php?modulo=principal/cadastro_pi&acao=A&obrid=<? echo $_SESSION['obra']['obrid']; ?>&req=vincular';}" checked> Vincular</td>
</tr>
<? } ?>
<tr>
	<td class="SubTituloCentro">Vincular plano interno</td>
</tr>
<tr>
<td>
<?
$sql = "SELECT 	'<center><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"removerVinculacao(\'".$_SESSION['obra']['obrid']."\',\''|| p.pliid ||'\')\"></center>' as acao,'<a style=cursor:pointer; onclick=abrir_dadospi(\''||p.pliid||'\');>'||p.plicod||'</a>' as plicod,
				coalesce(p.plititulo,'Não preenchido') as titulo,
				(Select coalesce(sum(pt.pipvalor),0) as total from monitora.pi_planointernoptres pt where pt.pliid = p.pliid ) as total,
				CASE WHEN p.plisituacao = 'P' THEN ' <font color=\"red\">Pendente</font> '
					 WHEN p.plisituacao = 'A' THEN ' <font color=\"green\">Aprovado</font> '
					 WHEN p.plisituacao = 'H' THEN ' <font color=\"blue\">Homologado</font> '
					 WHEN p.plisituacao = 'R' THEN	' <font color=\"#3F85FF\">Revisado</font> '
					 WHEN p.plisituacao = 'C' THEN	' <font color=\"#AF7817\">Cadastrado no SIAFI</font> '
					 WHEN p.plisituacao = 'E' THEN	' <font color=\"#EAC117\">Enviado para Revisão</font> ' END as situacao,
				(SELECT usunome ||' por '||to_char(pihdata, 'dd/mm/YYYY hh24:mi') FROM monitora.pi_planointernohistorico p1 LEFT JOIN seguranca.usuario u1 ON u1.usucpf = p1.usucpf WHERE p1.pliid=p.pliid ORDER BY p1.pihdata DESC LIMIT 1) as hst
				FROM monitora.pi_planointerno p 
				LEFT JOIN monitora.pi_subacaounidade su ON su.sbaid = p.sbaid
				INNER JOIN monitora.pi_obra o ON p.pliid = o.pliid
				$inner
				WHERE 
				p.pliano = '".$_REQUEST['anoexercicio']."' AND
				( p.unicod = '".$unicod."' or p.ungcod is not null ) AND
				--p.obrid is not null AND
				p.plistatus = 'A' AND
				o.obrid = {$_SESSION['obra']['obrid']}
				$where 
				GROUP BY p.pliid, p.plicod,p.plititulo,p.plidata,p.plisituacao 
				ORDER BY p.plidata DESC";
//dbg($sql);
$cabecalho = array("&nbsp;","Código do PI","Título","Valor Previsto(R$)","Situação","Última atualização");
$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);

?>
</td>
</tr>
<tr>
	<td><input type="button" value="Vincular PIs" onclick="window.open('obras.php?modulo=principal/vincular_pi&acao=A&obrid=<? echo $_SESSION['obra']['obrid']; ?>&anoexercicio=<? echo $_REQUEST['anoexercicio']; ?>','vincular','scrollbars=yes,height=600,width=600,status=no,toolbar=no,menubar=no,location=no');"></td>
</tr>
</table>
<?
} else {
	
	$res = obras_pegarOrgaoPermitido();
	
	if($_SESSION["obra"]["orgid"]) {
		$orgid = $_SESSION["obra"]["orgid"];
	} else {
		$orgid = $res[0]['id'];
	}
	
	$anos= array(0 => array("id" => 1, "descricao" => "2009", "link" => "/obras/obras.php?modulo=principal/cadastro_pi&acao=A&obrid=".$_REQUEST['obrid']."&anoexercicio=2009&req=vincular"),
				 1 => array("id" => 2, "descricao" => "2010", "link" => "/obras/obras.php?modulo=principal/cadastro_pi&acao=A&obrid=".$_REQUEST['obrid']."&anoexercicio=2010"),
				 2 => array("id" => 3, "descricao" => "2011", "link" => "/obras/obras.php?modulo=principal/cadastro_pi&acao=A&obrid=".$_REQUEST['obrid']."&anoexercicio=2011")
				 );
	
	echo "<br />";
				 
	echo montarAbasArray($anos, "/obras/obras.php?modulo=principal/cadastro_pi&acao=A&obrid=".$_REQUEST['obrid']."&anoexercicio=".$_REQUEST['anoexercicio']);
	
?>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="radio" name="req" onclick="if(this.checked){window.location='obras.php?modulo=principal/cadastro_pi&acao=A&obrid=<? echo $_SESSION['obra']['obrid']; ?>&anoexercicio=<? echo $_REQUEST['anoexercicio']; ?>';}" <? if(!$_REQUEST['req']) echo "checked"; ?> > Criar &nbsp;&nbsp;&nbsp;&nbsp; <input type="radio" name="req" onclick="if(this.checked){window.location='obras.php?modulo=principal/cadastro_pi&acao=A&obrid=<? echo $_SESSION['obra']['obrid']; ?>&anoexercicio=<? echo $_REQUEST['anoexercicio']; ?>&req=vincular';}"> Vincular</td>
</tr>
<tr>
	<td width="40%" align='right' class="SubTituloDireita">Subação:</td>
	<td>
	<? 
	$wh_subacao = "";

	if (!$db->testa_superuser()) {
		if ( possuiPerfil( array( PERFIL_SUPERVISORMEC, PERFIL_EMPRESA, PERFIL_GESTORMEC, PERFIL_ADMINISTRADOR )  )) {
		
			$sql = "SELECT sub.sbaid as codigo, sub.sbacod || ' - ' || sub.sbatitulo as descricao FROM monitora.pi_subacao sub 
				INNER JOIN obras.programafonte pr ON pr.sbaid = sub.sbaid  
				WHERE sub.sbastatus='A' AND sub.sbasituacao='A' AND sub.sbaobras = true AND pr.orgid='".$orgid."' 
				group by sub.sbaid , sub.sbacod || ' - ' || sub.sbatitulo 
				order by 1,2";
			$wh_subacao = " AND su.unicod <> '".$unicod."'";
			
		}
		elseif  ( possuiPerfil( array( PERFIL_SUPERVISORUNIDADE, PERFIL_GESTORUNIDADE )  )) {
			$sql = "SELECT sub.sbaid as codigo, sub.sbacod || ' - ' || sub.sbatitulo as descricao FROM monitora.pi_subacao sub 
					INNER JOIN monitora.pi_subacaounidade sau ON sau.sbaid = sub.sbaid 
					WHERE sub.sbastatus='A' AND sub.sbasituacao='A' AND sub.sbaobras = true AND sau.unicod = '".$unicod."' 
					group by sub.sbaid , sub.sbacod || ' - ' || sub.sbatitulo 
					order by 1,2";
			$wh_subacao = " AND su.unicod = '".$unicod."'";
		} 
	} else {
	
		$sql = "SELECT sub.sbaid as codigo, sub.sbacod || ' - ' || sub.sbatitulo as descricao FROM monitora.pi_subacao sub 
				INNER JOIN obras.programafonte pr ON pr.sbaid = sub.sbaid  
				WHERE sub.sbastatus='A' AND sub.sbasituacao='A' AND sub.sbaobras = true AND pr.orgid='".$orgid."' 
				group by sub.sbaid , sub.sbacod || ' - ' || sub.sbatitulo 
				UNION ALL 
				SELECT sub.sbaid as codigo, sub.sbacod || ' - ' || sub.sbatitulo as descricao FROM monitora.pi_subacao sub 
				INNER JOIN monitora.pi_subacaounidade sau ON sau.sbaid = sub.sbaid 
				WHERE sub.sbastatus='A' AND sub.sbasituacao='A' AND sub.sbaobras = true AND sau.unicod = '".$unicod."' 
				group by sub.sbaid , sub.sbacod || ' - ' || sub.sbatitulo 
				order by 1,2";
			$wh_subacao = " AND su.unicod <> '".$unicod."'";
	}
	$habilitado = 'S';

	$db->monta_combo('combo_sbaid', $sql, $permissao_formulario,'Não utilizar subação','selecionarsubacao','','',400, '', 'sbaid', '', $_REQUEST['sbaid']);

	?>
	</td>
</tr>
<tr>
	<td colspan="2">
	
	<table cellpadding="0" border="0" width="98%" align="center" id="orcamento"  style="BORDER-RIGHT: #C9C9C9 1px solid; BORDER-TOP: #C9C9C9 1px solid; BORDER-LEFT: #C9C9C9 1px solid; BORDER-BOTTOM: #C9C9C9 1px solid;" onmouseover="tabindexcampo();">
	<tr>
		<td style="background-color: #C9C9C9;" colspan="7" align="center"><b>Detalhamento Orçamentário</b></td>
	</tr>
	<tr>
		<td style="background-color: #C9C9C9;" align="center" nowrap><b>PTRES</b><input type="hidden" name="pliptres"></td>
		<td style="background-color: #C9C9C9; width:45%;" align="center" nowrap><b>Ação</b></td>
		<td style="background-color: #C9C9C9; width:100px;" align="center" nowrap><b>Dotação Autorizada</b></td>
		<td style="background-color: #C9C9C9; width:100px;" align="center" nowrap><b>Dotação Subação</b></td>
		<td style="background-color: #C9C9C9; width:100px;" align="center" nowrap><b>Detalhado no PI</b></td>
		<td style="background-color: #C9C9C9; width:100px;" align="center"><b>Saldo Disponível</b></td>
		<td style="background-color: #C9C9C9;" align="center"><b>Valor Previsto(Anual)</b></td>
	</tr>
	<?php
	if($acoespl[0]) {
		$valortotalpi = 0;
		$cor = 0;
		foreach($acoespl as $acpl) { 
	?>
        <tr style="height:30px;<? echo (($cor%2)?"":"background-color:#DCDCDC;"); ?>" id="ptres_<? echo $acpl['ptres']; ?>">
			<td align="center"><? echo $acpl['ptres']; ?></td>
			<td align="left"><? echo $acpl['descricao']; ?></td>
		    <td align="right"><? echo number_format($acpl['dotacaoinicial'],2,',','.'); ?></td>
		    <td align="right"><? echo number_format($acpl['dotacaosubacao'],2,',','.'); ?></td>
		    <td align="right"><a href="javascript:detfin('<?=$acpl['ptres']?>')"><? echo number_format($acpl['detalhamento'],2,',','.'); ?></a></td>
		    <td align="right"><? if ( $acpl['dotacaosubacao'] > 0 ) { echo number_format(($acpl['dotacaosubacao']-$acpl['detalhamento']),2,',','.'); } else { echo number_format(($acpl['dotacaoinicial']-$acpl['detalhamento']),2,',','.'); }?></td>
		    <td align="center"><input type="text" name="plivalored[<? echo $acpl['ptrid']; ?>]" size="28" maxlength="" value="<? echo number_format($acpl['pipvalor'],2,',','.'); ?>" onKeyUp="this.value=mascaraglobal('###.###.###.###,##',this.value);calculovalorPI();"  class="normal"  onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this); verificaDisponivel(this,'<? echo $acpl['ptres']; ?>','<? echo number_format($acpl['pipvalor'],2,',','.'); ?>');" style="text-align : right; width:25ex;" title='' /></td>
		</tr>
	<?php
			$cor++;
			$valortotalpi = $valortotalpi + $acpl['pipvalor']; 
		}
	} 
	?>
	<tr style="height: 30px;">
		<td align="right" valign="top" colspan="6"><b>TOTAL :</b></td>
		<td align="center" valign="top"><input type="text" name="valortotalpi" id="valortotalpi" size="28" maxlength="" value="<? echo number_format($valortotalpi,2,',','.'); ?>" onKeyUp="this.value=mascaraglobal('###.###.###.###,##',this.value);" disabled  class="disabled"  onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="text-align : right; width:25ex;" title='' /></td>
	</tr>
	<tr>
		<td align="right" colspan="7">
		<input type="button" onclick="abrir_lista();" id="btn_selecionar_acaptres" value="Selecionar Ação/PTRES" <?php echo $disabled; ?> >
		</td>
	</tr>
	</table>
	</td>
</tr>
<tr>
	<td class='SubTituloDireita'>Enquadramento da Despesa:</td>
	<td>
	<?php
	if($eqdid){
		$sql = "SELECT eqdid as codigo, eqdcod ||' - '|| eqddsc as descricao FROM monitora.pi_enquadramentodespesa WHERE eqdid = $eqdid";
		$arEnquadramento = $db->pegaLinha($sql);
		$arEnquadramento = ($arEnquadramento) ? $arEnquadramento : array();
		echo $arEnquadramento['descricao'];
		echo "<input type=\"hidden\" name=\"eqdid\" id=\"eqdid\" value=\"{$arEnquadramento['codigo']}\">";
	} else {
		$sql = "SELECT eqdid as codigo, eqdcod ||' - '|| eqddsc as descricao
			    FROM monitora.pi_enquadramentodespesa 
			    WHERE eqdstatus='A' and eqdano = '$pliano'
			    ORDER BY eqdcod";
		$db->monta_combo('eqdid', $sql, $permissao_formulario, 'Selecione', 'atualizarPrevisaoPI', '', '', '240', 'S', 'eqdid');
	} 
	?>
	</td>
</tr>
<tr>
	<td class='SubTituloDireita'>Nível/Etapa de Ensino:</td>
	<td>
		<?php
		if($eqdid){
			$sql = "SELECT neeid as codigo, neecod ||' - '|| needsc as descricao FROM monitora.pi_niveletapaensino WHERE neeid = $neeid";
			$arNivelEtapa = $db->pegaLinha($sql);
			$arNivelEtapa = ($arNivelEtapa) ? $arNivelEtapa : array();
			echo $arNivelEtapa['descricao'];
			echo "<input type=\"hidden\" name=\"neeid\" id=\"neeid\" value=\"{$arNivelEtapa['codigo']}\">";
		} else {

			if($orgid == ORGAO_SESU) {
				$sql = "SELECT neeid as codigo, neecod ||' - '|| needsc as descricao
					    FROM monitora.pi_niveletapaensino 
					    WHERE neeano='".$pliano."' AND neestatus='A' AND neecod='G' 
					    ORDER BY neecod";
				
				$niveletapaensino = $db->carregar($sql);
				
			} elseif($orgid == ORGAO_SETEC) {
				
				$sql = "SELECT neeid as codigo, neecod ||' - '|| needsc as descricao
					    FROM monitora.pi_niveletapaensino 
					    WHERE neeano='".$pliano."' AND neestatus='A' AND neecod='P' 
					    ORDER BY neecod";
				
				$niveletapaensino = $db->carregar($sql);
				
			} else {
				
				$niveletapaensino = array();
				
			}
			
			$db->monta_combo('neeid', $niveletapaensino, $permissao_formulario, '', 'atualizarPrevisaoPI', '', '', '240', 'S', 'neeid');
		} 
		?>
	</td>
</tr>
<tr>
	<td class='SubTituloDireita'>Categoria de Apropriação:</td>
	<td>
		<?php
		if($eqdid){
			$sql = "SELECT capid as codigo, capcod ||' - '|| capdsc as descricao FROM monitora.pi_categoriaapropriacao WHERE capid = $capid";
			$arCatApropri = $db->pegaLinha($sql);
			$arCatApropri = ($arCatApropri) ? $arCatApropri : array();
			echo $arCatApropri['descricao'];
			echo "<input type=\"hidden\" name=\"capid\" id=\"capid\" value=\"{$arCatApropri['codigo']}\">"; 
		} else {
			$sql = "SELECT capid as codigo, capcod ||' - '|| capdsc as descricao
				    FROM monitora.pi_categoriaapropriacao 
				    WHERE capano='".$pliano."' and capstatus='A' AND capcod IN('41','42','43')
				    ORDER BY capcod";
			$db->monta_combo('capid', $sql, $permissao_formulario, 'Selecione', 'atualizarPrevisaoPI', '', '', '340', 'S', 'capid');
		} 
		?>
	</td>
</tr>	
<tr id="tr_plicodsubacao">
<?php 
//$sbacod  = $_REQUEST['plicodsubacao'];
//$plicodsubacao = $sbacod;
?>
	<td align='right' class="SubTituloDireita">Código da subacao:</td>
	<td><? echo campo_texto('plicodsubacao','S',$permissao_formulario,'',5,4,'','',null,null,null,'id="plicodsubacao"','digitar_plicodsubacao();'); ?></td>
</tr>

<tr>
	<td align='right' class="SubTituloDireita">Título:</td>
    <td>
    <input type="hidden" name="plititulo_sub" id="plititulo_sub" value="<? echo $plititulo_sba; ?>">
    <?
	echo campo_texto('plititulo','S',$permissao_formulario,'',50,45,'','',null,null,null,'id="plititulo" onKeyUp="titulopi();"');  
   	?>
    </td>
</tr>
<tr>
    <td align='right' class="SubTituloDireita" valign="top">Descrição / Finalidade:</td>
    <td><?php
    	$plidsc_ = $db->pegaUm("SELECT obrcomposicao FROM obras.obrainfraestrutura where obrid = {$_SESSION['obra']['obrid']}");
    	$plidsc_ = substr($plidsc_,0,500);
    	echo campo_textarea( 'plidsc_', 'N', $permissao_formulario, '', 60, 2, 250 );
    	echo "<input type='hidden' id='plidsc' name='plidsc' value='".$plidsc_."'>";
    	?>
    </td>
</tr>
<tr>
    <td align='right' class="SubTituloDireita">Previsão PI:</td>
    <td align="left"> 
        <table style="background-color: #C9C9C9" cellpadding="" border="0" width="126px" >
        		<tr>
	        		<td>
	        		<table cellpadding="0" border="0" width="98%" >
		        		<tr>
		        		<td style="width: 30px; height: 20px; background-color: #C9C9C9;" align="center"><b>Enquadramento</b></td>
		        		<td align="center" colspan="3"><b>Subação</b></td>
		        		<td style="width: 30px; height: 20px; background-color: #C9C9C9;" align="center"><b>Nível</b></td>
		        		<td style="width: 30px; height: 20px; background-color: #C9C9C9;" align="center"><b>Apropriação</b></td>
		        		<td style="width: 30px; height: 20px; background-color: #C9C9C9;" align="center"><b>Codificação</b></td>
		        		</tr>
		        		<tr>
		        			<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" ><span id="enquadramento"><? echo substr($planointerno['plicod'],0,1); ?></span></td>
		        			<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" colspan="3"><span id="subacao"><?php echo ($planointerno['plicodsubacao']) ? $planointerno['plicodsubacao'] : 'XXXX' ?></span></td>
							<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center"><span id="nivel"><? echo substr($planointerno['plicod'],5,1); ?></span></td>
							<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" ><span id="apropriacao"><? echo substr($planointerno['plicod'],6,2); ?></span></td>
							<td style="width: 30px; height: 20px; background-color: #FFFFDD" align="center" ><span id="codificacao"><? echo substr($planointerno['plicod'],8,3); ?></span></td>		        			        			  
		        		</tr>
		        		<tr>
		        			<td colspan="7" align="center"><b>Código PI / SIAFI: <span id="enquadramento_i"><? echo substr($planointerno['plicod'],0,1); ?></span><span id="subacao_i"><?php echo ($planointerno['plicodsubacao']) ? $planointerno['plicodsubacao'] : 'XXXX' ?></span><span id="nivel_i"><? echo substr($planointerno['plicod'],5,1); ?></span><span id="apropriacao_i"><? echo substr($planointerno['plicod'],6,2); ?></span><span id="codificacao_i"><? echo substr($planointerno['plicod'],8,3); ?></span></b></td>
		        		</tr>
		        	</table>	        		
	        		</td>
        		</tr>
        </table>
        
	</td>
</tr>

<tr bgcolor="#cccccc">
	<td colspan="2" align="right">
		<?php if($_POST['pliid'] && $plisituacao == 'E'){ ?>
		<input type="button" value="Enviar para Homologação" onclick="vincular('R', '<?php echo $_POST['pliid']; ?>')" style="cursor: pointer;"/>
		<?php } ?>
		<input type="button" class="botao" name="btg" value="Salvar" onclick="submeter('<?=$plicod?>');" <?php echo $disabled; ?> >
		<input type="button" class="botao" name="btn" value="Novo" onclick="window.location='?modulo=principal/cadastro_pi&acao=A&obrid=<?php echo $_SESSION['obra']['obrid']; ?>';">
	</td>
	
</tr>

<tr>
	<td colspan="2" class="SubTituloCentro">Lista de PIs - <? echo $db->pegaUm("SELECT UPPER(unidsc) FROM public.unidade WHERE unicod='".$unicod."'"); ?></td>
</tr>
<tr>
	<td colspan="2">
	<?
	if($_REQUEST["atiid"]){
		$inner = "INNER JOIN monitora.pi_planointernoatividade pia ON p.pliid = pia.pliid";
		$where = "AND pia.atiid = {$_REQUEST["atiid"]} ";
	}
	
	
	if(!$boPerfilSomenteLeitura) {
		$btExluir = "<a style=\"cursor:pointer;\" onclick=\"removerpi(\''||p.pliid||'\');\"><img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\"></a>";
	} else {
		$btExluir = "<a><img src=\"/imagens/excluir_01.gif \" border=0 title=\"Excluir\"></a>";
	}
	
	$sql = "SELECT '<center><a style=\"cursor:pointer;\" onclick=\"alterarpi(\''||p.pliid||'\');\"><img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\"></a>'|| CASE p.plisituacao 
					WHEN 'P' THEN ' $btExluir ' 
					WHEN 'R' THEN ' $btExluir ' 
					WHEN 'E' THEN ' $btExluir ' 
				ELSE ''
					END ||'</center>' as acao,
				p.plicod as plicod,
				coalesce(p.plititulo,'Não preenchido') as titulo,
				coalesce(SUM(pt.pipvalor),0) as total,
				CASE WHEN p.plisituacao = 'P' THEN ' <font color=\"red\">Pendente</font> '
					 WHEN p.plisituacao = 'A' THEN ' <font color=\"green\">Aprovado</font> '
					 WHEN p.plisituacao = 'H' THEN ' <font color=\"blue\">Homologado</font> '
					 WHEN p.plisituacao = 'R' THEN	' <font color=\"#3F85FF\">Revisado</font> '
					 WHEN p.plisituacao = 'C' THEN	' <font color=\"#AF7817\">Cadastrado no SIAFI</font> '
					 WHEN p.plisituacao = 'E' THEN	' <font color=\"#EAC117\">Enviado para Revisão</font> ' END as situacao,
				(SELECT usunome ||' por '||to_char(pihdata, 'dd/mm/YYYY hh24:mi') FROM monitora.pi_planointernohistorico p1 LEFT JOIN seguranca.usuario u1 ON u1.usucpf = p1.usucpf WHERE p1.pliid=p.pliid ORDER BY p1.pihdata DESC LIMIT 1) as hst
				FROM monitora.pi_planointerno p 
				LEFT JOIN monitora.pi_planointernoptres pt ON pt.pliid = p.pliid 
				LEFT JOIN monitora.pi_subacaounidade su ON su.sbaid = p.sbaid $wh_subacao
				INNER JOIN monitora.pi_obra o ON p.pliid = o.pliid
				$inner
				WHERE 
				p.pliano = '".$_REQUEST['anoexercicio']."' AND
				p.unicod = '".$unicod."' AND
				p.plistatus = 'A' AND
				o.obrid = {$_SESSION['obra']['obrid']}
				$where 
				GROUP BY p.pliid, p.plicod,p.plititulo,p.plidata,p.plisituacao 
				ORDER BY p.plidata DESC";
//dbg($sql);
	$cabecalho = array("","Código do PI","Título","Valor Previsto(R$)","Situação","Última atualização");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);

	?>
	</td>
</tr>
</table>
<?
}
?>
</form>

<script type="text/javascript">

function vincular(situacao, pliid){
	if(pliid){
	 	var url = window.location.href;
		var parametros = "requisicao=vincular&pliid="+pliid+'&situacao='+situacao ;
		var myAjax = new Ajax.Request(
			url,
			{
				method: 'post',
				parameters: parametros,
				asynchronous: false,
				onComplete: function(r) {
					if(r.responseText){
						alert('Dados gravados com Sucesso.');
						// feito isso por causa da presa.
						//window.location.reload();
						document.formulario.submit();
					}
				}
			}
		);
	}
}

function Trim(str){return str.replace(/^\s+|\s+$/g,"");}

function removerVinculacao(obrid,pliid) {
	var conf =  confirm("Deseja realmente excluir a vinculação do PI?");
	if(conf) {
		window.location = 'obras.php?modulo=principal/cadastro_pi&acao=A&evento=R&pliid='+pliid+'&obrid='+obrid;
	}
}

function digitar_plicodsubacao() {
	document.getElementById("plicodsubacao").value = document.getElementById("plicodsubacao").value.toUpperCase();
	document.getElementById("subacao").innerHTML   = document.getElementById("plicodsubacao").value;
	document.getElementById("subacao_i").innerHTML = document.getElementById("plicodsubacao").value;
}

function selecionarsubacao(sbaid) {

	var sbaidAnterior = document.getElementById("sbaidAnterior").value;
	
	document.getElementById("sbaidAtual").value = sbaid;
	
	if(!sbaidAnterior){
		document.getElementById("sbaidAnterior").value = sbaid;
	}
	if(sbaid) {
		document.getElementById("plicodsubacao").value = '';
		document.getElementById("tr_plicodsubacao").style.display = 'none';
		
		document.getElementById("subacao").innerHTML   = "XXXX";
		document.getElementById("subacao_i").innerHTML = "XXXX";
		
		for(var i=(document.getElementById("orcamento").rows.length-3); i > 1 ;i--) {
			document.getElementById("orcamento").deleteRow(i);
		}
	
		var req = new Ajax.Request(window.location.href, {
								        method:     'post',
								        parameters: '&sbaAjax=' + sbaid,
								        asynchronous: false,
								        onComplete: function (res) {
	        
											if(sbaid != sbaidAnterior){
												document.getElementById("plititulo").value = "";
												document.getElementById("plititulo_sub").value = "";
											}
	        
	    						        	var dados = res.responseText;
	    						        	dados = dados.split("!@#");
											document.getElementById("subacao").innerHTML   = dados[0];
											document.getElementById("subacao_i").innerHTML = dados[0];
											var plititulo = Trim(dados[1]) + '-' + Trim(document.getElementById("plititulo").value);
											document.getElementById("plititulo").value = Trim(plititulo.substr(0,45));
											document.getElementById("plititulo_sub").value = Trim(plititulo.substr(0,45));
	
								        }
								  });
		document.getElementById("sbaidAnterior").value = sbaid;
	} else {
	
		document.getElementById("plicodsubacao").value = '';
		document.getElementById("tr_plicodsubacao").style.display = '';
	
		document.getElementById("subacao").innerHTML   = "XXXX";
		document.getElementById("subacao_i").innerHTML = "XXXX";
		document.getElementById("plititulo").value     = "";
		document.getElementById("plititulo_sub").value = "";
		
		for(var i=(document.getElementById("orcamento").rows.length-3); i > 1 ;i--) {
			document.getElementById("orcamento").deleteRow(i);
		}
	}
}


function submeter(pliid){
	var validado = true;
	if(validar(pliid)){
		if(validado) {
			document.formulario.submit();
		}
	}

}

function submeterComSituacao(pliid,situacao) {
	var validado = true;
	
	if(!validar()){
		return false;
	}
	
	if(validado) {
		document.formulario.plisituacao.value=situacao;
		document.formulario.submit();
	}
}


function removerpi(pliid){
	var conf = confirm("Você realmente deseja excluir este PI?");	
	if(conf) {
		document.getElementById('evento').value = 'E';
		document.getElementById('pliid').value = pliid;
		document.formulario.submit();	
	}
}

function alterarpi(pliid){
	document.getElementById('evento').value = 'A';
	document.getElementById('pliid').value = pliid;
	document.formulario.submit();
}


/* Função para subustituir todos */
function replaceAll(str, de, para){
    var pos = str.indexOf(de);
    while (pos > -1){
		str = str.replace(de, para);
		pos = str.indexOf(de);
	}
    return (str);
}

function validar(x){	

	var msg = "";
	
	var tabela = document.getElementById('orcamento');
	// validando se existe ação selecionado/ valor
	if(tabela.rows.length == 4) {
		msg+="A escolha das ações é obrigatório.\n";
	} else {
		for(i=2;i<(tabela.rows.length-2);i++) {
			if(!tabela.rows[i].cells[6].firstChild.value) {
				msg+="Valor do PTRES: '"+tabela.rows[i].cells[0].innerHTML+"' é obrigatório.\n";
			}
			else if(parseFloat(replaceAll(replaceAll(tabela.rows[i].cells[6].firstChild.value,".",""),",",".")) <= 0){
				msg+="Valor do PTRES: '"+tabela.rows[i].cells[0].innerHTML+"' informado deverá ser maior que 0(zero)\n";
			}
		}
	}

	if(document.formulario.plititulo.value == '') {
		msg+="O preenchimento do campo Título é obrigatório.\n";
	}
	if(document.formulario.plidsc_.value == ''){
		msg+="O preenchimento do campo Descrição é obrigatório.\n";
	}

	// testa se foi selecionada o enquadramento
	if(document.getElementById("eqdid").value == ''){
		msg+="O preenchimento do campo Enquadramento da Despesa é obrigatório.\n";
	}
	if(document.getElementById("neeid").value == ''){
		msg+="O preenchimento do campo Nível/Etapa de Ensino é obrigatório.\n";
	}
	if(document.getElementById("capid").value == ''){
		msg+="O preenchimento do campo Categoria de Apropriação é obrigatório.\n";
	}
	
	if(document.getElementById("sbaid").value == ''){
		if(document.getElementById("plicodsubacao").value.length != 4){
			msg+="O preenchimento do campo Código da subação é obrigatório e igual a 4(quatro dígitos).\n";
		}
	}

	
	if(msg != "") {
	
		alert(msg);
		return false;
	
	}else{
	
		 var pi = (document.getElementById("enquadramento").innerHTML + 
		 		  document.getElementById("subacao").innerHTML + 
		 		  document.getElementById("nivel").innerHTML +
		 		  document.getElementById("apropriacao").innerHTML +
		 		  document.getElementById("codificacao").innerHTML 
		 		  );
		 
		 if(validapi(pi)) {
		 	document.getElementById("plicod").value = pi; 
		 	return true;
		 } else {
		 	return false;
		 }
		 
	
	}	
}

function valida2(pi) {
	 if(!pi){
		return true;
	 }
	 else{
	 	if(pi.substr(0,10) == 'pijaexiste'){
	 		pi = pi.replace("pijaexiste","");
		 	var alertaDisplay = '<div class="titulo_box" >Atenção!</div><div class="texto_box" >Plano Interno já existe!</div><div class="conteudo_box" >Veja abaixo os dados o Plano Interno cadastrado :</div><div class="conteudo_box" >' + pi + '</div><div class="links_box" ><input type="button" onclick=\'closeMessage();return false\' value="Cancelar" /></center>';
		 	displayStaticMessage(alertaDisplay,false);
	 		return false;
	 	}
	 	var alertaDisplay = '<div class="titulo_box" >Atenção!</div><div class="texto_box" >Já existe(m) PI(s) criado(s) com esta estrutura. Veja abaixo a relação dos PI(s) encontrados:</div><div class="conteudo_box" >' + pi + '</div><div class="links_box" >Deseja realmente criar?<br><center><input type="button" onclick=\'document.formulario.submit();\' value="Confirmar" /> <input type="button" onclick=\'closeMessage();return false\' value="Cancelar" /></center>';
	 	displayStaticMessage(alertaDisplay,false);
	 	return false;
	 }
	
}

function validapi(pi) {
	var retorno = true;
	
	var req = new Ajax.Request(window.location.href, {
							        method:     'post',
							        parameters: '&piAjax=' + pi + '&pliid=' + document.getElementById('pliid').value,
							        asynchronous: false,
							        onComplete: function (res) {
    						        	x = res.responseText;
    									retorno = valida2(x);
							        }
							  });

	return retorno;	
}

function abrir_lista() {
	<? if ( possuiPerfil( array( PERFIL_SUPERVISORMEC, PERFIL_EMPRESA, PERFIL_GESTORMEC, PERFIL_ADMINISTRADOR )  )) { ?>
		if( !document.getElementById("sbaidAtual").value ) {
			alert('É necessário escolher uma subação!');
			return;
		}
	<? } ?>
	var anoexercicio = '<?php echo $_REQUEST['anoexercicio'] ?>';
	janela = window.open('/obras/obras.php?modulo=principal/listarProgramaObra&acao=A&sbaid='+document.getElementById("sbaidAtual").value+'&unicod='+document.getElementById("unicod").value+'&anoexercicio='+anoexercicio, 'janela1', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width='+(screen.width-120)+',height=680' ); janela.focus();
}

function abrir_dadospi(pliid) {
	janela = window.open('/obras/obras.php?modulo=principal/dados_pi&acao=A&pliid='+pliid, 'janela1', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width='+(screen.width-120)+',height=680'); 
	janela.focus();
}


function atualizarPrevisaoPI(){
	if(document.getElementById('capid').value) {
		var apropriacao = document.getElementById('capid').options[document.getElementById('capid').selectedIndex].text.split(" - ");
		document.getElementById("apropriacao").innerHTML = apropriacao[0]; 
		document.getElementById("apropriacao_i").innerHTML = apropriacao[0];
	}
	if(document.getElementById('eqdid').value) {
		var enquadramento = document.getElementById('eqdid').options[document.getElementById('eqdid').selectedIndex].text.split(" - ");
		document.getElementById("enquadramento").innerHTML = enquadramento[0]; 
		document.getElementById("enquadramento_i").innerHTML = enquadramento[0];
	}
	if(document.getElementById('neeid').value) {
		var nivel = document.getElementById('neeid').options[document.getElementById('neeid').selectedIndex].text.split(" - ");
		document.getElementById("nivel").innerHTML = nivel[0]; 
		document.getElementById("nivel_i").innerHTML = nivel[0];
	}

}


function atualizarCodigoLivre() {
	document.getElementById("idCodificacao").value = document.getElementById("idCodificacao").value.toUpperCase();
	document.getElementById("codificacao").innerHTML = document.getElementById("idCodificacao").value;
}

function calculovalorPI() {
	var tabela = document.getElementById('orcamento');
	var tot = 0;
	for(i=2;i<tabela.rows.length-2;i++) {
		if(tabela.rows[i].cells[6].firstChild.value != "") {
			tot = tot + parseFloat(replaceAll(replaceAll(tabela.rows[i].cells[6].firstChild.value,".",""),",","."));
		}
	}

	var c = tot.toString();
	if(c.indexOf('.') == -1) {
		document.getElementById('valortotalpi').value = tot.toFixed(2);
	} else {
		document.getElementById('valortotalpi').value = Arredonda(tot,2);
	}
	document.getElementById('valortotalpi').onkeyup();
}

function Arredonda( valor , casas ){
   var novo = Math.round( valor * Math.pow( 10 , casas ) ) / Math.pow( 10 , casas );
   var c = novo.toString();
   if(c.indexOf('.') == -1) {
	   	alert(novo);
   		return novo;
   } else {
   		return novo.toFixed(casas);
   }
}

function verificaDisponivel(campo, ptres, vlold) {
	var linha = document.getElementById('ptres_'+ptres);
	valordisp = parseFloat(replaceAll(replaceAll(linha.cells[5].innerHTML,".",""),",","."));
	valoratual = parseFloat(replaceAll(replaceAll(campo.value,".",""),",","."));
	if(valoratual>(valordisp+parseFloat(replaceAll(replaceAll(vlold,".",""),",",".")))) {	
		alert('Valor não pode ser maior do que o Disponível');
		campo.value = vlold;
		calculovalorPI();
	}
}

function detfin(ptres){
	var pliano = '<?php echo $pliano; ?>';
	janela = window.open('obras.php?modulo=principal/detalhafinanceiro_pi&acao=A&ptres='+ptres+'&pliano='+pliano, 'janela2', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width='+(screen.width-420)+',height=280' ); janela.focus();
}

function selecionarprograma(sba) {
	document.formulario.evento.value = "SUBACAO";
	document.formulario.submit();
}

messageObj = new DHTML_modalMessage();	// We only create one object of this class
messageObj.setShadowOffset(5);	// Large shadow


function displayMessage(url)
{
	
	messageObj.setSource(url);

	messageObj.setCssClassMessageBox(false);
	messageObj.setSize(400,200);
	messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
	messageObj.display();
}

function displayStaticMessage(messageContent,cssClass) {
	messageObj.setHtmlContent(messageContent);
	messageObj.setSize(600,500);
	messageObj.setCssClassMessageBox(cssClass);
	messageObj.setSource(false);	// no html source since we want to use a static message here.

	messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
	messageObj.display();
	
	
}

function closeMessage()
{
	messageObj.close();	
}

function carregarsubacaocodigo(sbaid,boPiTitulo) {
	if(sbaid) {
		var req = new Ajax.Request(window.location.href, {
								        method:     'post',
								        parameters: '&sbaAjax=' + sbaid,
								        asynchronous: false,
								        onComplete: function (res) {
	    						        	var dados = res.responseText;
	    						        	dados = dados.split("!@#");
											document.getElementById("subacao").innerHTML   = dados[0];
											document.getElementById("subacao_i").innerHTML = dados[0];

											if(boPiTitulo){
												var plititulo = Trim(dados[1]) + ' - ' + Trim(document.getElementById("plititulo").value);
												document.getElementById("plititulo").value = Trim(plititulo.substr(0,45));
												document.getElementById("plititulo_sub").value = Trim(plititulo.substr(0,45));
											}
								        }
								  });
							  
	}
}


var boSelecionaSubacao = '<?php echo $boSelecionaSubacao; ?>';
var sbaid = '<?php echo $planointerno['sbaid']; ?>';
if(boSelecionaSubacao){
	var atiid = '<?php echo $_GET['atiid']; ?>';
	var boPiTitulo = false;
	if(atiid){
		boPiTitulo = true;
	}
	carregarsubacaocodigo(sbaid,boPiTitulo);
}

document.observe("dom:loaded", function() {
  atualizarPrevisaoPI();
});

if(sbaid){
//	selecionarsubacao(sbaid);
	document.getElementById("tr_plicodsubacao").style.display = 'none';
}

</script>