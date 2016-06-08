<?php

include_once APPRAIZ . "www/obras/_funcoes.php";

$res = obras_pegarOrgaoPermitido();

if ( empty( $_GET['org'] ) ){
	$_REQUEST['org'] = $_SESSION['pesquisaObra']["org"];
}

$org = array();

if ($_REQUEST['org']){
	$org = (array) $_REQUEST['org'];
}elseif (is_array($res)){
	foreach ($res as $r){
		if ($r['id']) $org[] = $r['id'];
	}
}

if (  $_SESSION['obra']['orgid'] == ORGAO_FNDE ){
	
	$podeVer = verificaPermissaoObra( $_SESSION["usucpf"], $_REQUEST["obrid"], $_SESSION['obra']['orgid'] );
	if ( !$podeVer ){
		echo "<script>
				alert('Você não possui permissão para ver esta obra!');
				history.back(-1);
			  </script>";
		die;
	}
	
}

$arPerfilEntid = array( PERFIL_SUPERVISORUNIDADE, PERFIL_GESTORUNIDADE,PERFIL_GESTORORGAO, PERFIL_SUPERVISORMEC, PERFIL_ADMINISTRADOR );
$arPerfilConsulta = array( PERFIL_CONSULTAPR, PERFIL_CONSULTAORGAO, PERFIL_CONSULTAUNIDADE,PERFIL_SAMPR);
$arPerfilOrgid = array(PERFIL_SUPERVISORMEC, PERFIL_ADMINISTRADOR, PERFIL_GESTORORGAO, PERFIL_SUPERVISORORGAO );


$arrEdita = array(PERFIL_SUPERUSUARIO,PERFIL_SUPERVISORUNIDADE,PERFIL_GESTORUNIDADE,PERFIL_GESTORORGAO,PERFIL_SUPERVISORORGAO);
$arrVisualiza = array(PERFIL_CONSULTAPR, PERFIL_CONSULTAORGAO, PERFIL_CONSULTAUNIDADE,PERFIL_SUPERVISORPR,PERFIL_SAMPR );

//if ( $org ){
//	$habilitado = obras_possuiPerfilOrgao( $arPerfilEntid, $arPerfilOrgid, $org );	
//}
//		

$arrMenuNotBloq = array(
						'inicio',
						'principal/etapas_da_obra'
						);
//

if( possuiPerfil($arrEdita) ){
	$habilitado = true;
}elseif(possuiPerfil($arrVisualiza)){
	$habilitado = false;
}

if ( !in_array($_GET['modulo'] , $arrMenuNotBloq) ){						
	if ($habilitado) $habilitado = obraAditivoPossuiCronograma();
	if ($habilitado) $habilitado = obraAditivoPossuiVistoria();
}

//
//
//if( possuiPerfil($arPerfilConsulta) && !$db->testa_superuser() ){
//	$habilitado = false;
//}






$somenteLeitura = $habilitado  ? 'S' : 'N';
$disabled = $habilitado ? '' : 'disabled';
?>