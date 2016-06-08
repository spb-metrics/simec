<?php 

$bloquearEdicao = bloqueiaEdicaoFT();

if( $_POST['ajaxestuf'] ){	 
	header('content-type: text/html; charset=ISO-8859-1'); 
	$sql = "select
			 muncod as codigo, mundescricao as descricao 
			from
			 territorios.municipio 
			where
			 estuf = '".$_POST['ajaxestuf']."' 
			order by
			 mundescricao asc"; 
	die($db->monta_combo( "muncod", $sql, 'S', 'Selecione...', 'filtraEstado(this.value);', '', '', '', 'S', 'muncod' ));
}

if( $_POST['ajaxsiape'] ){	 
	header('content-type: text/html; charset=ISO-8859-1'); 
	$sql = "SELECT  
			nu_matricula_siape,
			co_registro_geral,
			co_sexo,
			co_estado_civil,  
			nu_ddd_telefone,
			nu_telefone 
		    FROM gestaopessoa.vw_simec_consulta_servidor_siape 
		    WHERE 
		    replace( replace(nu_cpf,'.', ''), '-','') = replace( replace('".$_POST['ajaxsiape']."','.', ''), '-','') ";
	$rs  = $db->carregar( $sql );
	if( $rs ){
		$res = 
		$rs[0]['nu_matricula_siape'].'_'.
		$rs[0]['co_registro_geral'].'_'.
		$rs[0]['co_sexo'].'_'.
		$rs[0]['co_estado_civil'].'_'.
		$rs[0]['nu_ddd_telefone'].'_'.
		$rs[0]['nu_telefone']; 
		die( $res );
	} 
	die();
}
if( $_POST['ajaxmuncod'] ){	 
	header('content-type: text/html; charset=ISO-8859-1'); 
	$sql = "select
			 estuf  
			from
			 territorios.municipio 
			where
			 muncod = '".$_POST['ajaxmuncod']."' 
			 ";
	die($db->pegaUm($sql));
}

if( $_POST['ajaxremovefoto'] ){	 
	header('content-type: text/html; charset=ISO-8859-1'); 
	
	$sql = "UPDATE public.arquivo SET arqstatus = 'I' 
			WHERE arqid in (select arqid from gestaopessoa.anexogp where fdpcpf = '".$_SESSION['fdpcpf']."')";
	$db->executar( $sql );
	
	$sql = "UPDATE gestaopessoa.anexogp SET agpstatus = 'I' WHERE fdpcpf = '".$_SESSION['fdpcpf']."'";
	$db->executar( $sql );
	
	$db->commit();
	echo "OK";
	exit;
}


include  APPRAIZ."includes/cabecalho.inc";
include_once( APPRAIZ . "www/includes/webservice/cpf.php" );
echo '<br>'; 

if( $_SESSION['fdpcpf'] != '') {  
	$fdpcpf = $_SESSION['fdpcpf'];
	$db->cria_aba( $abacod_tela, $url, '' );
	$cpfBlock = "N";
}else{
	$fdpcpf = $_SESSION['usucpf'];
	echo "<div id = \"espace\" style=\"height:20px;\" >&nbsp;<br></div>";
	//echo "<br>";
	$cpfBlock = 'S';
}
monta_titulo( 'Força de Trabalho', 'Dados Pessoais' ); 
?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script src="../includes/prototype.js"></script>
<script src="../includes/calendario.js"></script>  
<?
echo cabecalhoPessoa($fdpcpf); 
include_once( APPRAIZ. "gestaopessoa/classes/FtDadoPessoal.class.inc" ); 
include_once APPRAIZ . "includes/classes/fileSimec.class.inc"; 
$ft   	= new FtDadoPessoal();
$perfil = arrayPerfil();
                  if( in_array(  PERFIL_SERVIDOR , $perfil ) || 
                  	  in_array(  PERFIL_TERCEIRIZADO, $perfil ) || 
                  	  in_array(  PERFIL_CONSULTOR , $perfil ) ){
                  	$CPF_ = $_SESSION['fdpcpf'];
                  }else{ 
                  	$CPF_ = $_SESSION["fdpcpf"]; 
                  }

$fdpCpf = $ft->pegaUm( "select fdpcpf from gestaopessoa.ftdadopessoal where fdpcpf = '". $CPF_ ."'" );
//die("select fdpcpf from gestaopessoa.ftdadopessoal where fdpcpf = '". $CPF_ ."'");
if( $fdpCpf ){ 	
	
	$ft->carregarPorId( "'".$fdpCpf."'" );
	
	$sql = "SELECT * FROM gestaopessoa.ftdadopessoal WHERE fdpcpf = '".$fdpCpf."'";
	$dados = $db->carregar( $sql );
 	$_SESSION['fdpcpf'] = $fdpCpf;
}
 
if( $_POST ){
	
	$ft->fdpcpf 					= str_replace(".","", str_replace( "-", "", $_POST['fdpcpf'] ) );

	$ft->fdpnome  					= $_POST['fdpnome'];
	$ft->fdpsiape  					= $_POST['fdpsiape'];
	$ft->eciid  					= $_POST['eciid'];
	$ft->fdpconjuge  				= $_POST['fdpconjuge'];
	$ft->fdpnomepai 				= $_POST['fdpnomepai'];
	$ft->fdpnomemae 				= $_POST['fdpnomemae'];
	$ft->fdpsexo 					= $_POST['fdpsexo'];
	$ft->fdpdatanascimento 			= formata_data_sql( $_POST['fdpdatanascimento'] );
	$ft->fdpnaturalidade 			= $_POST['fdpnaturalidade'];
	$ft->fdpenderecoresidencial 	= $_POST['fdpenderecoresidencial'];
	$ft->fdpbairro 					= $_POST['fdpbairro'];
	$ft->muncod 					= $_POST['muncod'];
	$ft->estuf 						= $_POST['estuf'];
	$ft->fdpcep 					= $_POST['fdpcep'];
	$ft->fdpdddtelefoneres 			= $_POST['fdpdddtelefoneres'];
	$ft->fdptelefoneres 			= $_POST['fdptelefoneres'];
	$ft->fdpdddcelular				= $_POST['fdpdddcelular'];
	$ft->fdpcelular 				= $_POST['fdpcelular'];
	$ft->fdpsexo					= $_POST['fdpsexo'];
	$ft->fdpgruposanguineo 			= $_POST['fdpgruposanguineo'];
	$ft->fdpfatorrh 				= $_POST['fdpfatorrh'];
	$ft->fdpcor 					= $_POST['fdpcor'];
	$ft->fdpdeficiente 				= $_POST['fdpdeficiente'];
	$ft->fdpdeficiencia 			= $_POST['fdpdeficiencia'];
	$ft->fdpidentidade 				= $_POST['fdpidentidade'];
	$ft->fdpidexpeditor 			= $_POST['fdpidexpeditor'];
	$ft->fdpufidentidade 			= $_POST['fdpufidentidade'];
	$ft->fdpiddataexpedicao 		= formata_data_sql( $_POST['fdpiddataexpedicao'] );
	$ft->fdpregistroprofissional 	= $_POST['fdpregistroprofissional']; 
	$ft->fdpdataregistroprofissional= formata_data_sql( $_POST['fdpdataregistroprofissional']) ? formata_data_sql( $_POST['fdpdataregistroprofissional']) : null;
	$ft->fdptituloeleitor 			= $_POST['fdptituloeleitor'];
	$ft->fdpzonaeleitoral 			= $_POST['fdpzonaeleitoral'];
	$ft->fdpsecaoeleitoral			= $_POST['fdpsecaoeleitoral'];
	$ft->fdplocaleleitoral 			= $_POST['fdplocaleleitoral']; 
	$ft->fdpdataprimeiroemprego 	= formata_data_sql( $_POST['fdpdataprimeiroemprego']) ? formata_data_sql( $_POST['fdpdataprimeiroemprego']) : null; 
	$ft->fstid 						= $_POST['fstid'];
 
	if( $fdpCpf ){
		$ft->fdpcpf = "'".str_replace(".","", str_replace( "-", "", $fdpCpf ) )."'";
		$ft->alterar();
	} else { 
		$sessionCpf = $ft->inserir();
		$_SESSION['fdpcpf'] = $sessionCpf;
	}
	$ft->commit(); 

	$campos	= array("fdpcpf"		  => "'".$_SESSION['fdpcpf']."'",
					"agpdatainclusao" =>"'".date('Y-m-d')."'",
					"agpstatus" => "'A'",
					"usucpf"    => "'".$_SESSION['usucpf']."'"
					);	
				
	$_SESSION["sisdiretorio"] = "gestaopessoa";	 
	$file = new FilesSimec("anexogp", $campos ,"gestaopessoa");
	if( $_FILES['Arquivo']['tmp_name'] != "" ){
//	if( is_array( $_SESSION['sessionFILES'] ) ){
//		$_FILES['Arquivo'] = $_SESSION['sessionFILES'];
//		$_FILES['Arquivo']['tmp_name'] = $_SESSION['session_temp_name'];
		 
		$sql = "UPDATE gestaopessoa.anexogp SET agpstatus = 'I' WHERE fdpcpf = '".$_SESSION['fdpcpf']."'";
		$db->executar( $sql );
		$db->commit();
//		$file->arquivo = $_FILES; 
		$file->setMover($_FILES['Arquivo']['tmp_name'], "image/jpeg");
//		$arquivoSalvo = $file->setUpload();   
	}
    $ft->sucesso( "principal/cadDadosPessoais" );
    $_SESSION["sisdiretorio"] = "gestaopessoa";	  
} 
if( !$_SESSION['fdpcpf']){   
	$onBlur= "carregaDadosWS(this.value);";
}else{
	$onBlur =""; 
} 
function getImagemServidor(){
	global $db;
	$sql = "SELECT * FROM gestaopessoa.anexogp AS agp INNER JOIN public.arquivo AS arq ON arq.arqid = agp.arqid 
			WHERE agp.fdpcpf = '".$_SESSION['fdpcpf']."' AND agp.agpstatus = 'A'";
 
	$dados = $db->carregar( $sql );
	if( $dados ){ 
		//return $dir = 'file:///'.APPRAIZ.'arquivos/gestaopessoa/'.floor($dados[0]['arqid']/1000).'/'.$dados[0]['arqid'].'.'.$dados[0]['arqextensao']; 
		return $dir = $dados[0]['arqid']; 
	
	}else{
		return false;
	}
}
unset( $_SESSION['sessionFILES']);
unset( $_FILES['Arquivo']);
?>
<!-- 
<script src="uploader.js" ></script>
 -->
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<center> Preencha os Dados Pessoais  </center>
 <form name = "formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario" enctype="multipart/form-data">

	    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	  			<tr>
	                <td class ="SubTituloDireita" align="right">CPF: </td>
	                <td>
	                 
	                	 <?=campo_texto('fdpcpf', 'S', $cpfBlock, '', 20, 14, '', '', 'left', '',  0, 'id="fdpcpf" onkeyup="this.value=mascaraglobal(\'###.###.###-##\',this.value);" ','','', $onBlur  ); ?>  
	                </td>
	            </tr> 
	            <tr>
	                <td class ="SubTituloDireita" align="right">Situação no MEC: </td>
	                <td>
	                <?
	                	$sql = "SELECT 
	                            fstid AS codigo, 
	                            fstdescricao AS descricao
	                        FROM
	                            gestaopessoa.ftsituacaotrabalhador
	                         ";
	                	if(   in_array(  PERFIL_SERVIDOR , $perfil ) || 
		                  	  in_array(  PERFIL_TERCEIRIZADO, $perfil ) || 
		                  	  in_array(  PERFIL_CONSULTOR , $perfil ) ){
	                        $sql.= "WHERE fstid in ( ".implode(",",controlaPefilFT('vinculosPermitidos') ).")";
		                } 
	          
	  				$fstid = $dados[0]['fstid'];
                	$_SESSION['sit']= $fstid;
                	//echo $_SESSION['sit'];
                	
	                $db->monta_combo('fstid', $sql, 'S', "Selecione...", '', '', '', '200', 'S', 'fstid');
	                ?> 
	                </td>
	            </tr>
	            <tr>
	                <td class ="SubTituloDireita" align="right">Nome: </td>
	                <td>
	                <? $fdpnome = $dados[0]['fdpnome']; ?>
	                	 <?= campo_texto('fdpnome', 'S', $somenteLeitura, '', 80, 200, '', '', 'left', '',  0, 'id="fdpnome" onblur="MouseBlur(this);"' ); ?>  
	                </td>
	            </tr>
	            <tr>
	            	<td class ="SubTituloDireita" align="right">Foto: </td>
	                <td height="4cm"> 
	     				<? if( !getImagemServidor()  ) { ?>
	     					Imagem não cadastrada.
	     				<? }else{ ?>
	     					<div style="width: 3cm; height: 4cm; border: 8px solid #CDC9C9; text-align: center;">
	     						<div style="width: 3cm; position: relative; height: 4cm; border: 4px solid white; width: 100%; left:-4px; top:-3px;">
	     							<img width="3cm" height="4cm" src="../slideshow/slideshow/verimagem.php?arqid=<?=getImagemServidor();?>&_sisarquivo=gestaopessoa" id="imagem" style="width: 3cm; height: 4cm;">
	     						</div>
	     					</div>
	     					<div style="width: 3cm; text-align: center;">
	     						<a href='javascript:RmvFoto()'><b><font color=red>(x) Remover</font></b></a>
	     					</div>
	     					<br>
	                	<? } 
	                	?> 
	                </td>
	            </tr> 
	            <tr>
	            	<td class ="SubTituloDireita" align="right">Imagem 3x4: </td>
	                <td> 
	                	<? 
		                	include_once( APPRAIZ . "gestaopessoa/classes/AjaxFileUploader.inc.php" );
		                	$ajaxFileUploader = new AjaxFileuploader($uploadDirectory="");	
							echo $ajaxFileUploader->showFileUploader('Arquivo'); 
						 ?> 
	                </td>
	            </tr>
	            <tr>
	                <td class ="SubTituloDireita" align="right">Matrícula SIAPE: </td>
	                <td>
	                  <? 
//	                  $perfil = arrayPerfil();
//	                  if( in_array(  PERFIL_SERVIDOR, $perfil ) ){
//	                  	$obig = 'S';
//	                  }else{
//	                  	$obrig = 'N';
//	                  }
	                  $fdpsiape = $dados[0]['fdpsiape']; ?>
	                	 <?= campo_texto('fdpsiape', 'S', $somenteLeitura, '', 12, 10, '', '', 'left', '',  0, 'id="fdpsiape"  onkeyup="this.value=mascaraglobal(\'[#]\',this.value);" onblur="MouseBlur(this);"' ); ?>  
	                </td>
	            </tr> 
	            <tr>
	                <td class ="SubTituloDireita" align="right">Estado Civíl: </td>
	                <td>
	                <?
	                	$sql = "SELECT 
	                            eciid AS codigo, 
	                            ecidescricao AS descricao
	                        FROM
	                            public.estadocivil
	                       "; 
	 				  $eciid = $dados[0]['eciid'];  
	                $db->monta_combo('eciid', $sql, 'S', "Selecione...", '', '', '', '200', 'S', 'eciid');
	                ?>
	                </td>
	            </tr>
	            <tr>
	                <td class ="SubTituloDireita" align="right">Cônjuge: </td>
	                <td>
	                <? $fdpconjuge = $dados[0]['fdpconjuge']; ?>
	                	 <?= campo_texto('fdpconjuge', 'N', $somenteLeitura, '', 80, 200, '', '', 'left', '',  0, 'id="fdpconjuge" onblur="MouseBlur(this);"' ); ?>  
	                </td>
	            </tr> 
	            <tr>
	                <td class ="SubTituloDireita" align="right">Nome do Pai: </td>
	                <td>
	                 <? $fdpnomepai = $dados[0]['fdpnomepai']; ?>
	                	 <?= campo_texto('fdpnomepai', 'N', $somenteLeitura, '', 80, 200, '', '', 'left', '',  0, 'id="fdpnomepai" onblur="MouseBlur(this);"' ); ?>  
	                </td>
	            </tr> 
	            <tr>
	                <td class ="SubTituloDireita" align="right">Nome da Mãe: </td>
	                <td>
	                	 <? $fdpnomemae = $dados[0]['fdpnomemae']; ?>
	                	 <?= campo_texto('fdpnomemae', 'S', $somenteLeitura, '', 80, 200, '', '', 'left', '',  0, 'id="fdpnomemae" onblur="MouseBlur(this);"' ); ?>  
	                </td>
	            </tr> 
	            <tr>
	                <td class ="SubTituloDireita" align="right">Sexo: </td>
	                <td>
	                 
	                	<input type="radio" name="fdpsexo" id="fdpsexo[m]" <? if( $dados[0]['fdpsexo'] == 'm') echo 'checked = checked'; ?> value="m"> Masculino &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
	                	<input type="radio" name="fdpsexo" id="fdpsexo[f]" <? if( $dados[0]['fdpsexo'] == 'f') echo 'checked = checked'; ?> value="f"> Feminino 
	                </td>
	            </tr> 
	             <tr>
	                <td class ="SubTituloDireita" align="right">Data de Nascimento: </td>
	                <td>
	                <? 
	                
	                $fdpdatanascimento = $dados[0]['fdpdatanascimento']; ?>
	                	 <?= campo_data2( 'fdpdatanascimento','S', 'S', '', 'S' ); ?>  
	                </td>
	            </tr> 
	             <tr>
	                <td class ="SubTituloDireita" align="right">Naturalidade: </td>
	                <td>
	                 <? $fdpnaturalidade = $dados[0]['fdpnaturalidade']; ?>
	                	 <?= campo_texto('fdpnaturalidade', 'S', $somenteLeitura, '', 80, 200, '', '', 'left', '',  0, 'id="fdpnaturalidade" onblur="MouseBlur(this);"' ); ?>  
	                </td>
	            </tr> 
	             <tr>
	                <td class ="SubTituloDireita" align="right">Endereço Residencial: </td>
	                <td>
	                 <? $fdpenderecoresidencial = $dados[0]['fdpenderecoresidencial']; ?>
	                	 <?= campo_texto('fdpenderecoresidencial', 'S', $somenteLeitura, '', 80, 200, '', '', 'left', '',  0, 'id="fdpenderecoresidencial" onblur="MouseBlur(this);"' ); ?>  
	                </td>
	            </tr> 
	             <tr>
	                <td class ="SubTituloDireita" align="right">Bairro: </td>
	                <td>
	                   <? $fdpbairro = $dados[0]['fdpbairro']; ?>
	                	 <?= campo_texto('fdpbairro', 'S', $somenteLeitura, '', 80, 200, '', '', 'left', '',  0, 'id="fdpbairro" onblur="MouseBlur(this);"' ); ?>  
	                </td>
	            </tr> 
	            
	            <tr>
	                <td class ="SubTituloDireita" align="right">UF: </td>
	                <td id="td_estado">
	                 <? $estuf = $dados[0]['estuf']; ?>
	                <?
	                	$sql = "SELECT 
	                            estuf  AS codigo, 
	                            estdescricao AS descricao
	                        FROM
	                            territorios.estado
	                        order by estdescricao
	                       "; 
	                 
	                $db->monta_combo('estuf', $sql, 'S', "Selecione...", 'filtraMunicipio(this.value);', '', '', '200', 'S', 'estuf');
	                ?>
	                </td>
	            </tr>
	             <tr>
	                <td class ="SubTituloDireita" align="right">Cidade: </td>
	                <td id="td_municipio">
	                <? $muncod = $dados[0]['muncod']; ?>
	                <?
	                	$sql = "SELECT 
	                            muncod AS codigo, 
	                            mundescricao AS descricao
	                        FROM
	                            territorios.municipio
	                            order by mundescricao
	                       "; 
	                $db->monta_combo('muncod', $sql, 'S', "Selecione...", '', '', '', '200', 'S', 'muncod');
	                ?>
	                </td>
	            </tr> 
	            <tr>
	                <td class ="SubTituloDireita" align="right">CEP: </td>
	                <td>
	                 <? $fdpcep = $dados[0]['fdpcep']; ?>
	                	 <?= campo_texto('fdpcep', 'S', $somenteLeitura, '', 10, 8, '########', '', 'left', '',  0, 'id="fdpcep" onkeyup="this.value=mascaraglobal(\'########\',this.value); onblur="MouseBlur(this);"' ); ?>  
	                </td>
	            </tr> 
	            <tr>
	                <td class ="SubTituloDireita" align="right">Telefone: </td>
	                <td>
	                <? $fdpdddtelefoneres = $dados[0]['fdpdddtelefoneres']; ?>
	                  <? $fdptelefoneres = $dados[0]['fdptelefoneres']; ?>
	                	 (<?= campo_texto('fdpdddtelefoneres', 'N', $somenteLeitura, '', 2, 2, '##', '', 'left', '',  0, 'id="fdpdddtelefoneres"  onkeyup="this.value=mascaraglobal(\'##\',this.value);" onblur="MouseBlur(this);"' ); ?>)  <?= campo_texto('fdptelefoneres', 'N', $somenteLeitura, '', 11, 9, '', '', 'left', '',  0, 'id="fdptelefoneres"  onkeyup="this.value=mascaraglobal(\'####-####\',this.value);" onblur="MouseBlur(this);"' ); ?>  
	                </td>
	            </tr> 
	            <tr>
	                <td class ="SubTituloDireita" align="right">Celular: </td>
	                <td>
	                   <? $fdpdddcelular = $dados[0]['fdpdddcelular']; ?>
	                   <? $fdpcelular = $dados[0]['fdpcelular']; ?>
	                	 (<?= campo_texto('fdpdddcelular', 'N', $somenteLeitura, '', 2, 2, '##', '', 'left', '',  0, 'id="fdpdddcelular"  onkeyup="this.value=mascaraglobal(\'##\',this.value);" onblur="MouseBlur(this);"' ); ?>)  <?= campo_texto('fdpcelular', 'N', $somenteLeitura, '', 11, 9, '', '', 'left', '',  0, 'id="fdpcelular"  onkeyup="this.value=mascaraglobal(\'####-####\',this.value);" onblur="MouseBlur(this);"' ); ?> 
	                </td>
	            </tr> 
	            <tr>
	            	<td>
	            	<h4>Características Físicas</h4>
	            	</td>
	            	<td>
	            	</td>
	            </tr>
	             <tr>
	                <td class ="SubTituloDireita" align="right">Grupo Sanguíneo: </td>
	                <td>
	                <select class="CampoEstilo" name="fdpgruposanguineo" id="fdpgruposanguineo" value="<?$dados[0]['fdpgruposanguineo'];?>">
	                	<option value="">Selecione...</option>
	                	<option <?if( $dados[0]['fdpgruposanguineo']=='A')  echo "selected=selected"; ?> value="A">A</option>
	                	<option <?if( $dados[0]['fdpgruposanguineo']=='B')  echo "selected=selected"; ?> value="B">B</option>
	                	<option <?if( $dados[0]['fdpgruposanguineo']=='AB') echo "selected=selected"; ?> value="AB">AB</option>
	                	<option <?if( $dados[0]['fdpgruposanguineo']=='O')  echo "selected=selected"; ?> value="O">O</option>
	                </select>  
	                </td>
	            </tr>
	            <tr>
	                <td class ="SubTituloDireita" align="right">Fator RH: </td>
	                <td>
	                <select class="CampoEstilo" name="fdpfatorrh" id="fdpfatorrh" value="<?$dados[0]['fdpfatorrh'];?>">
	                	<option value="">Selecione...</option>
	                	<option <?if( $dados[0]['fdpfatorrh']=='Positivo')  echo "selected=selected"; ?> value="Positivo">Positivo</option>
	                	<option <?if( $dados[0]['fdpfatorrh']=='Negativo')  echo "selected=selected"; ?>  value="Negativo">Negativo</option> 
	                </select>  
	               </td>
	            </tr>
	            <tr>
	                <td class ="SubTituloDireita" align="right">Cor/Origem: </td>
	                <td>
	                <select class="CampoEstilo" name="fdpcor" id="fdpcor" value="<?$dados[0]['fdpcor'];?>">
	                	<option value="">Selecione...</option>
	                	<option <?if( $dados[0]['fdpcor']=='Branca')  echo "selected=selected"; ?> value="Branca">Branca</option>
	                	<option <?if( $dados[0]['fdpcor']=='Afro') { echo "selected=selected"; }?> value="Afro">Afro</option>
	                	<option <?if( $dados[0]['fdpcor']=='Amarela')  echo "selected=selected"; ?>value="Amarela">Amarela</option>
	                	<option <?if( $dados[0]['fdpcor']=='Parda')  echo "selected=selected"; ?>value="Parda">Parda</option>
	                	<option <?if( $dados[0]['fdpcor']=='Indígena')  echo "selected=selected"; ?>value="Indígena">Indígena</option>
	                </select>  
	                </td>
	            </tr>
	            <tr>
	                <td class ="SubTituloDireita" align="right">Pessoa com Deficiência: </td>
	                <td>
	                	<input type="radio" name="fdpdeficiente" id="fdpdeficiente[t]"  <? if( $dados[0]['fdpdeficiente'] == 't') echo 'checked = checked'; ?> value="t"> Sim  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
	                	<input type="radio" name="fdpdeficiente" id="fdpdeficiente[f]"  <? if( $dados[0]['fdpdeficiente'] == 'f') echo 'checked = checked'; ?> value="f"> Não
	                </td>
	            </tr>
	            <tr>
	                <td class ="SubTituloDireita" align="right">Tipo de Deficiência: </td>
	                <td>
	                 <? $fdpdeficiencia = $dados[0]['fdpdeficiencia']; ?>
	                	 <?= campo_texto('fdpdeficiencia', 'N', $somenteLeitura, '', 50, 300, '', '', 'left', '',  0, 'id="fdpdeficiencia" onblur="MouseBlur(this);"' ); ?>  
	                </td>
	            </tr> 
	            <tr>
	            	<td>
	            	<h4>Documentos</h4>
	            	</td>
	            	<td>
	            	</td>
	            </tr>
	            <tr>
	                <td class ="SubTituloDireita" align="right">Carteira de Identidade Nº: </td>
	                <td>
	                  <? $fdpidentidade = $dados[0]['fdpidentidade']; ?>
	                	 <?= campo_texto('fdpidentidade', 'S', $somenteLeitura, '', 20, 20, '', '', 'left', '',  0, 'id="fdpidentidade"  onkeyup="this.value=mascaraglobal(\'[#]\',this.value);" onblur="MouseBlur(this);"' ); ?>  
	                </td>
	            </tr> 
	            <tr>
	                <td class ="SubTituloDireita" align="right"> Órgão Expedidor Identidade: </td>
	                <td>
	                <? $fdpidexpeditor = $dados[0]['fdpidexpeditor']; ?>
	                	 <?= campo_texto('fdpidexpeditor', 'S', $somenteLeitura, '', 10, 10, '', '', 'left', '',  0, 'id="fdpidexpeditor" onblur="MouseBlur(this);"' ); ?>  
	                </td>
	            </tr> 
	            <tr>
	                <td class ="SubTituloDireita" align="right"> UF Identidade: </td>
	                <td>
	                <? $fdpufidentidade = $dados[0]['fdpufidentidade']; ?>
	                <?
	                	$sql = "SELECT 
	                            estuf AS codigo, 
	                            estdescricao AS descricao
	                        FROM
	                            territorios.estado
	                       "; 
	                 
	                $db->monta_combo('fdpufidentidade', $sql, 'S', "Selecione...", '', '', '', '100', 'S', 'fdpufidentidade');
	                ?>
	                </td>
	            </tr> 
	             <tr>
	                <td class ="SubTituloDireita" align="right">Data Exp. Identidade: </td>
	                <td>
	                 <? $fdpiddataexpedicao = $dados[0]['fdpiddataexpedicao']; ?>
	                	 <?= campo_data2( 'fdpiddataexpedicao','S', 'S', '', 'S' ); ?> 
	                </td>
	            </tr>
	            <tr>
	                <td class ="SubTituloDireita" align="right"> Registro Profissional (Conselho): </td>
	                <td>
	                  <? $fdpregistroprofissional = $dados[0]['fdpregistroprofissional']; ?>
	                	 <?= campo_texto('fdpregistroprofissional', 'N', $somenteLeitura, '', 10, 20, '', '', 'left', '',  0, 'id="fdpregistroprofissional" onblur="MouseBlur(this);"' ); ?>  
	                </td>
	            </tr> 
	 
	            <tr>
	                <td class ="SubTituloDireita" align="right">Data Exp. Registro: </td>
	                <td>
	                  <? $fdpdataregistroprofissional =  $dados[0]['fdpdataregistroprofissional']; ?>
	                	 <?= campo_data2( 'fdpdataregistroprofissional','N', 'S', '', 'S' ); ?> 
	                </td>
	            </tr>
	             <tr>
	                <td class ="SubTituloDireita" align="right">Data. 1º Emprego: </td>
	                <td>
	                  <? $fdpdataprimeiroemprego = $dados[0]['fdpdataprimeiroemprego']; ?>
	                	 <?= campo_data2( 'fdpdataprimeiroemprego','N', 'S', '', 'S' ); ?> 
	                </td>
	            </tr>
	            <tr>
	                <td class ="SubTituloDireita" align="right"> Título de Eleitor: </td>	
	                <td>
	              <? $fdptituloeleitor = $dados[0]['fdptituloeleitor']; ?>
	                	 <?= campo_texto('fdptituloeleitor', 'N', $somenteLeitura, '', 18, 20, '', '', 'left', '',  0, 'id="fdptituloeleitor" onblur="MouseBlur(this);"' ); ?>  
	                </td>
	            </tr> 
	            <tr>
	                <td class ="SubTituloDireita" align="right"> Zona:</td>
	                <td>
	                 <? $fdpzonaeleitoral = $dados[0]['fdpzonaeleitoral']; ?>
	                	 <?= campo_texto('fdpzonaeleitoral', 'N', $somenteLeitura, '', 10, 20, '', '', 'left', '',  0, 'id="fdpzonaeleitoral" onblur="MouseBlur(this);"' ); ?>  
	                </td>
	            </tr> 
	            <tr>
	                <td class ="SubTituloDireita" align="right"> Seção: </td>
	                <td>
	                 <? $fdpsecaoeleitoral = $dados[0]['fdpsecaoeleitoral']; ?>
	                	 <?= campo_texto('fdpsecaoeleitoral', 'N', $somenteLeitura, '', 10, 20, '', '', 'left', '',  0, 'id="fdpsecaoeleitoral" onblur="MouseBlur(this);"' ); ?>  
	                </td>
	            </tr> 
	            <tr>
	                <td class ="SubTituloDireita" align="right"> Local: </td>
	                <td>
	                 <? $fdplocaleleitoral = $dados[0]['fdplocaleleitoral']; ?>
	                	 <?= campo_texto('fdplocaleleitoral', 'N', $somenteLeitura, '', 10, 50, '', '', 'left', '',  0, 'id="fdplocaleleitoral" onblur="MouseBlur(this);"' ); ?>  
	                </td>
	            </tr>  
	            <tr>
	            	<td class ="SubTituloDireita" align="right">  </td>
	            	<td><input type="button" name="btSalvar" id="btSalvar" onclick="validaForm();" value="Salvar" <?=$bloquearEdicao;?>></td>
	            </tr> 
	     </table>
      
      </form>

<script>
var fdpcpf = document.getElementById('fdpcpf'); 
var fdpnome = document.getElementById('fdpnome'); 
var fdpsiape = document.getElementById('fdpsiape'); 
var eciid = document.getElementById('eciid'); 
var fdpconjuge = document.getElementById('fdpconjuge'); 
var fdpnomepai = document.getElementById('fdpnomepai'); 
var fdpnomemae = document.getElementById('fdpnomemae');  
var fdpdatanascimento = document.getElementById('fdpdatanascimento'); 
var fdpnaturalidade = document.getElementById('fdpnaturalidade'); 
var fdpenderecoresidencial = document.getElementById('fdpenderecoresidencial'); 
var fdpbairro = document.getElementById('fdpbairro'); 
var estuf = document.getElementById('estuf'); 
var muncod = document.getElementById('muncod');
var fdpcep = document.getElementById('fdpcep'); 
var fdpdddtelefoneres = document.getElementById('fdpdddtelefoneres'); 
var fdptelefoneres = document.getElementById('fdptelefoneres'); 
var fdpdddcelular = document.getElementById('fdpdddcelular'); 
var fdpcelular = document.getElementById('fdpcelular'); 
var fdpgruposanguineo = document.getElementById('fdpgruposanguineo'); 
var fdpfatorrh = document.getElementById('fdpfatorrh'); 
var fdpcor = document.getElementById('fdpcor'); 
var fdpdeficiencia = document.getElementById('fdpdeficiencia'); 
var fdpidentidade = document.getElementById('fdpidentidade'); 
var fdpidexpeditor = document.getElementById('fdpidexpeditor'); 
var fdpufidentidade = document.getElementById('fdpufidentidade'); 
var fdpiddataexpedicao = document.getElementById('fdpiddataexpedicao'); 
var fdpregistroprofissional = document.getElementById('fdpregistroprofissional'); 
var fdpdataregistroprofissional = document.getElementById('fdpdataregistroprofissional'); 
var fdptituloeleitor = document.getElementById('fdptituloeleitor'); 
var fdpzonaeleitoral = document.getElementById('fdpzonaeleitoral'); 
var fdpsecaoeleitoral = document.getElementById('fdpsecaoeleitoral'); 
var fdplocaleleitoral = document.getElementById('fdplocaleleitoral'); 
var fdpserieeleitoral = document.getElementById('fdpserieeleitoral'); 
var fdpdataprimeiroemprego = document.getElementById('fdpdataprimeiroemprego'); 
 
var fstid = document.getElementById('fstid'); 

<?php if( !$_SESSION['fdpcpf'] ){?> 
	carregaDadosWS(<?=$_SESSION['usucpf']?>);
<?}?>
function carregaDadosWS(cpf){ 
    if( !cpf ){
    	return false;
    }
	var comp = new dCPF();
	comp.buscarDados(cpf); 
	fdpnomemae.value =comp.dados.no_mae_rf; 
	fdpnome.value =comp.dados.no_pessoa_rf; 
 
	var anonasc = comp.dados.dt_nascimento_rf.substr(0,4);
	var mesnasc = comp.dados.dt_nascimento_rf.substr(4,2);
	var dianasc = comp.dados.dt_nascimento_rf.substr(6,7);
 	
	var datanasc =  dianasc+'/'+mesnasc+'/'+anonasc; 
	 
	//buscando dados do SIAPE na view vw_simec_consulta_servidor_siape 
 	var req = new Ajax.Request('gestaopessoa.php?modulo=principal/cadDadosPessoais&acao=A', {
							        method:     'post',
							        parameters: '&ajaxsiape=' + cpf,							         
							        onComplete: function (res)							        {			
										 
										var arResp = res.responseText.split("_");
		 
										var nu_matricula_siape = arResp[0];
										var co_registro_geral  = arResp[1];
										var co_sexo 		   = arResp[2];
										var co_estado_civil    = arResp[3];
										var nu_ddd_telefone    = arResp[4];
										var nu_telefone 	   = arResp[5];
 							 
 										var fdpsexo 		   = document.getElementById('fdpsexo['+co_sexo+']');
 										 
 										fdpsiape.value 		= nu_matricula_siape;
 										  
 								 		document.getElementById('eciid').value =  co_estado_civil;
 								 		 
 								 		fdpdddtelefoneres.value = nu_ddd_telefone.substr(0, 3);
 								 		fdptelefoneres.value = nu_telefone;
 								 		
 								 		fdpsexo.value       = co_sexo; 
 										fdpsexo.checked = 1;
 								 		 
							        } 
							  });
	fdpdatanascimento.value = datanasc;
	fdpenderecoresidencial.value = comp.dados.ds_logradouro+' '+comp.dados.ds_logradouro_comp+' '+comp.dados.ds_numero;
	fdpbairro.value = comp.dados.ds_bairro; 
	muncod.value = Number( comp.dados.co_cidade );
	filtraEstado(Number( comp.dados.co_cidade )); 
	fdpcep.value = comp.dados.nu_cep;  
	fdpidentidade.value = comp.dados.nu_rg; 
	fdpidexpeditor.value = comp.dados.ds_orgao_expedidor_rg; 
	fdpiddataexpedicao.value = comp.dados.dt_emissao_rg; 
	fdptituloeleitor.value = comp.dados.nu_titulo_eleitor_rf;	
}
function validaForm(){
	var muncod = document.getElementById('muncod');
	 
	if( fdpcpf.value == '' ){
		alert( 'O campo CPF não pode ser vazio' );
		return false;
	}
	if( fstid.value == '' ){
		alert( 'O campo Situação no MEC não pode ser vazio' );
		return false;
	}
	if( fdpnome.value == '' ){
		alert( 'O campo Nome não pode ser vazio' );
		return false;
	}


	if( (fdpsiape.value == '') &&  ( fstid.value != '6' ) ){
		alert( 'O campo SIAPE não pode ser vazio' );
			return false;
	}

	
	<?
//	if( in_array(   PERFIL_SERVIDOR, $perfil ) ){ ?>
//		if( fdpsiape.value == '' ){
//		alert( 'O campo SIAPE não pode ser vazio' );
//		return false;
//	}
	<?//}?>
	
	if( eciid.value == '' ){
		alert( 'O campo Estado Cívil não pode ser vazio' );
		return false;
	}

	if( fdpnomemae.value == '' ){
		alert( 'O campo Nome da Mãe não pode ser vazio' );
		return false;
	}
//	if( document.getElementById('fdpsexo[\'M\']') == '' && 
//		document.getElementById('fdpsexo[\'F\']') == '' ){
//			alert( 'O campo "Sexo" não pode ser vazio' );
//		return false;
//	}
	if( document.getElementById('fdpsexo[m]').checked == 0 && 
			document.getElementById('fdpsexo[f]').checked == 0 ){
			alert( 'O campo "Sexo" deve ser respondido' );
			return false;
	}
	if( fdpdatanascimento.value == '' ){
		alert( 'O campo "Data de nascimento" não pode ser vazio' );
		return false;
	}
	if( fdpdatanascimento.value.length < 10 ){
		alert( 'Insira uma data de nascimento no formato \'dd/mm/aaaa\'.');
		return false;
	}
	if(!validaData(fdpdatanascimento)){
		alert( 'O campo "Data de nascimento" inválido.' );
		return false;
	}
	if( fdpnaturalidade.value == '' ){
		alert( 'O campo Naturalidade não pode ser vazio' );
		return false;
	}
	if( fdpenderecoresidencial.value == '' ){
		alert( 'O campo Endereço residencial não pode ser vazio' );
		return false;
	}
	if( fdpbairro.value == '' ){
		alert( 'O campo Bairro não pode ser vazio' );
		return false;
	}
	if( estuf.value == '' ){
		alert( 'O campo UF não pode ser vazio' );
		return false;
	}
	if( muncod.value == '' ){
		alert( 'O campo Município não pode ser vazio' );
		return false;
	}
	if( fdpcep.value == '' ){
		alert( 'O campo CEP não pode ser vazio' );
		return false;
	}

//	if( fdpgruposanguineo.value == '' ){
//		alert( 'O campo Grupo sanguíneo não pode ser vazio' );
//		return false;
//	}
//	if( fdpfatorrh.value == '' ){
//		alert( 'O campo Fator RH não pode ser vazio' );
//		return false;
//	}
//	if( fdpcor.value == '' ){
//		alert( 'O campo Cor não pode ser vazio' );
//		return false;
//	} 
	if( document.getElementById('fdpdeficiente[t]').checked == 0 && 
		document.getElementById('fdpdeficiente[f]').checked == 0 ){
		alert( 'O campo "Possui deficiência" deve ser respondido' );
		return false;
	}
	if( fdpidentidade.value == '' ){
		alert( 'O campo Identidade não pode ser vazio' );
		return false;
	}
	if( fdpidexpeditor.value == '' ){
		alert( 'O campo Orgão expeditor não pode ser vazio' );
		return false;
	}
	if( fdpufidentidade.value == '' ){
		alert( 'O campo UF orgão expeditor não pode ser vazio' );
		return false;
	}
	if( fdpiddataexpedicao.value == '' ){
		alert( 'O campo Data de expedicao UF não pode ser vazio' );
		return false;
	}
	if( ( fdpiddataexpedicao.value.length > 0 ) && ( fdpiddataexpedicao.value.length < 10 ) ){
		alert( 'Insira uma data de expedicao no formato \'dd/mm/aaaa\'.');
		return false;
	}
	if(!validaData(fdpiddataexpedicao)){
		alert( 'O campo Data de expedicao UF inválido' );
		return false;
	}
	if( ( fdpdataregistroprofissional.value.length > 0 ) && ( fdpdataregistroprofissional.value.length < 10 ) ){
		alert( 'Insira uma data de registro profissional no formato \'dd/mm/aaaa\'.');
		return false;
	}
	if(fdpdataregistroprofissional.value){
		if(!validaData(fdpdataregistroprofissional)){
			alert( 'O campo Data de registro profissional inválido' );
			return false;
		}
	}	
	if( ( fdpdataprimeiroemprego.value.length > 0 ) && ( fdpdataprimeiroemprego.value.length < 10 ) ){
		alert( 'Insira uma data de 1º emprego no formato \'dd/mm/aaaa\'.');
		return false;
	}
	if(fdpdataprimeiroemprego.value){
		if(!validaData(fdpdataprimeiroemprego)){
			alert( 'O campo Data de 1º emprego inválido' );
			return false;
		}
	}	
//	if( fdptituloeleitor.value == '' ){
//		alert( 'O campo Título de Eleitor não pode ser vazio' );
//		return false;
//	}
 	if( fdplocaleleitoral.value.length > 0 ){
 		if( fdplocaleleitoral.value.length > 50 ){
 			alert("Quantidade de cadacteres excedida para o campo \"Local Eleitoral\"");
 			return false;
 		}
 	}	
	  
	document.formulario.submit();
}
function filtraMunicipio(estuf){

 	if( !estuf ){
 		return false;
 	}
	var td_municipio = document.getElementById('td_municipio');
	var td_estado	 = document.getElementById('td_estado'); 
  	 
	var req = new Ajax.Request('gestaopessoa.php?modulo=principal/cadDadosPessoais&acao=A', {
							        method:     'post',
							        parameters: '&ajaxestuf=' + estuf,							         
							        onComplete: function (res)
							        {			
										td_municipio.innerHTML = res.responseText;
							        }
							        
							  });
    
}

function RmvFoto()
{

	if(confirm('Deseja realmente remover a foto?')){
		
		var req = new Ajax.Request('gestaopessoa.php?modulo=principal/cadDadosPessoais&acao=A', {
	        method:     'post',
	        parameters: '&ajaxremovefoto=1',							         
	        onComplete: function (res)
	        {			
				if(res.responseText=='OK');
				{
					/*
					document.getElementById('removefoto').innerHTML = '';
					document.getElementById('imagem').innerHTML='Imagem não cadastrada.'; 
			        document.getElementById('div_files').innerHTML='';	
			        http=createRequestObject();
			        */
			        window.location.reload();										
				}
	
	        }
	        
	  	});
	  	
	}

}

function filtraEstado(muncod){
	if( !muncod ){
 		return false;
 	}
	var td_municipio = document.getElementById('td_municipio');
	var td_estado	 = document.getElementById('td_estado');
	var estuf	 = document.getElementById('estuf');
  
	var req = new Ajax.Request('gestaopessoa.php?modulo=principal/cadDadosPessoais&acao=A', {
							        method:     'post',
							        parameters: '&ajaxmuncod=' + muncod,							         
							        onComplete: function (res)
							        {			 
										estuf.value = res.responseText;
							        }							        
							  });
    
}

var http=createRequestObject();
var uploader="";
var uploadDir="";
var dirname="";
var filename="";
var timeInterval="";
var idname="";
var uploaderId="";
 
function createRequestObject() {
    var obj;
    var browser = navigator.appName;
    if(browser == "Microsoft Internet Explorer"){
    	document.getElementById('div_files').style.left = "-446px";
    	document.getElementById('div_files').style.top = "60px";
    	document.getElementById('imagem').style.left = "0px";
    	document.getElementById('imagem').style.top = "0px";
    	return new ActiveXObject("Microsoft.XMLHTTP");
    }
    else{
    	return new XMLHttpRequest();
    }   
}
function traceUpload() {
   http.onreadystatechange = handleResponse;
   http.open("GET", 'gestaopessoa.php?modulo=principal/imageupload&acao=I&uploadDir='+uploadDir+'&dirname='+dirname+'&filename='+filename+'&uploader='+uploader); 
   http.send(null);   
}
function handleResponse() {
	
	if(http.readyState == 4){
		
		var response=http.responseText; 

		if(response != '0'){ 
			clearInterval(timeInterval); 
		}

		if (response.indexOf("#")!=-1){
		    arResponse = response.split("#");  
	        document.getElementById('imagem').innerHTML=arResponse[0]; 
	        document.getElementById('div_files').innerHTML=arResponse[1];
	        document.getElementById('removefoto').innerHTML = "<a href='javascript:RmvFoto()'><b><font color=red>(x) Remover</font></b></a>";
		} 
		else{
			document.getElementById('imagem').innerHTML='Erro Upload, aperte F5.'; 
	        document.getElementById('div_files').innerHTML='';
	        document.getElementById('removefoto').innerHTML = '';
	        /*
	        alert('Erro no Upload da Foto.');
	        window.location.reload();
	        */
		}
    }
    else {
    	document.getElementById('imagem').innerHTML = "<img src='/imagens/wait.gif' align=\"center\" border\"0\">";
    	document.getElementById(uploaderId).innerHTML="Uploading File. Please wait...";
    	
    }
}
function uploadFile(obj, dname) {
	 
	uploadDir=obj.value;
	idname=obj.name;
	dirname=dname;
	 
	filename=uploadDir.substr(uploadDir.lastIndexOf('\\')+1);
 
	uploaderId = 'uploader'+obj.name;
	uploader = obj.name;
	var df = document.getElementById('formName'+obj.name);
	document.getElementById('formName'+obj.name).submit(); 
	
	//verifica os tipos válidos; 
 	var arFileName = filename.split(".");
    if( ( arFileName[1] != 'jpeg')  &&  ( arFileName[1] != 'jpg') &&  ( arFileName[1] != 'gif') &&  ( arFileName[1] != 'png')&&  ( arFileName[1] != 'bmp') ){
   	     alert('O arquivo informado não foi reconhecido como um arquivo de imagem. Favor utilizar uma das extensões:\n\njpg, gif, png ou bmp');
		 return false;
    } 
    //chama a função com intervalo de 1500 ms
	timeInterval=setInterval("traceUpload()", 1500);
}
</script>