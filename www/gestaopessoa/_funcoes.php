<?php
function cabecalhoPessoa($usucpf){
	global $db;
	$sql = "SELECT su.usunome FROM gestaopessoa.ftdadopessoal  
			LEFT JOIN seguranca.usuario AS su ON su.usucpf = '".$usucpf."'";
	
	$dados = $db->carregar( $sql );
	
	if( $dados ){
		$cabecalho = "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
		$cabecalho .= "<tr> 
						<td class =\"SubTituloDireita\" align=\"right\">
							Nome
						</td>
					   	<td>
					   		".$dados[0]['usunome']."
					   	</td>
					   </tr>
					   <tr>
					   <td class =\"SubTituloDireita\" align=\"right\">
							CPF
						</td>
					   	<td>
					   		". formatar_cpf($usucpf)."
					   	</td>
					   </tr>";
		$cabecalho .= "</table>";		
	}	
	return $cabecalho;
}
function getAvaliadorHTML( $tipo , $peso = false, $indice = false, $defid = false){
	global $db;
	
	$sql = "SELECT tavid, tavdescricao FROM gestaopessoa.tipoavaliador WHERE tavstatus = 'A'";
	$rs  = $db->carregar( $sql );
	$perfis = arrayPerfil();
	if( $rs ){
		if( !$_SESSION['boautoavaliacao'] ){  
			for( $k =0; $k<count( $rs ); $k++ ){
				if( $tipo == 'TIPO_CABECALHO'){
					if(  ( in_array( PERFIL_AVALIACAO, $perfis ) && !soConsulta() )   ){ 
					
						if( !avaliacaoFinalizada($_SESSION['cpfavaliado'], TIPO_AVAL_SUPERIOR )) { 
							if( $rs[$k]['tavid'] != TIPO_AVAL_CONSENSO ) {?> 
							<th>  <?= $rs[$k]['tavdescricao'] ?> </th> 
							<th> Nota Final </th> 
						<?} 
						}elseif ( $rs[$k]['tavid'] != TIPO_AVAL_CONSENSO  && !avaliacaoFinalizada($_SESSION['cpfavaliado'], TIPO_AUTO_AVAL ) ){
							 ?>
							<th>  <?= $rs[$k]['tavdescricao'] ?> </th> 
							<th> Nota Final </th> 
						<?}elseif(  $rs[$k]['tavid'] == TIPO_AVAL_CONSENSO && !avaliacaoFinalizada($_SESSION['cpfavaliado'], TIPO_AUTO_AVAL)) { 
							continue;?>
							<th>  <?= $rs[$k]['tavdescricao'] ?> 
							
							</th> 
							<th> Nota Final </th> 
						<?}else{?>
							<?php if( !verificaMediaConsenso() ){ echo '<br> ( Média )'; } ?>
							<th>  <?= $rs[$k]['tavdescricao'] ?> 
							
							</th> 
							<th> Nota Final </th> 
						<?}   
					}else{  ?>
						<th> <?= $rs[$k]['tavdescricao'] ?> 
						<?php if( !verificaMediaConsenso() && $rs[$k]['tavid'] == TIPO_AVAL_CONSENSO ){ echo '<br> ( Média )'; } ?>
						</th> 
						<th> Nota Final </th> 
					<?}	
				}elseif( $tipo == 'TIPO_COLUNA'){
					$mascaraGlobalJs = "this.value=mascaraglobal('###',this.value);"; 
 
					if(  ( in_array( PERFIL_AVALIACAO, $perfis ) && !soConsulta()) ){  
					 
						if( !avaliacaoFinalizada($_SESSION['cpfavaliado'], TIPO_AVAL_SUPERIOR ) ){ 
							if( $rs[$k]['tavid'] == TIPO_AVAL_CONSENSO ) { //caso seja perfil de avaliação E não exista nota do avaliado E tipo for == consenso:
								$valor = verificaPontuacao($defid, $_SESSION['cpfavaliado'], $rs[$k]['tavid']); 
								$pesoCalculado = ( $valor * $peso ); 
								$disabled = "disabled = disabled"; 
								continue;
								?> 
								<td align="center"><input type="text" <?=$disabled?> size="3" maxlength="3" onkeyup="<?=$mascaraGlobalJs?> calcula( document.getElementById('pesoDefinicao[<?=$indice;?>]').value, this.value , 'div_defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]', '[<?=$defid?>][<?=$rs[$k]['tavid'];?>]' ); calculaColunas();" name="defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]" id="defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]" value="<?=$valor?>"></td>
								<td align="center"><div id="div_defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]"  style="display: '';"><?=$pesoCalculado;?></div></td>
							<?}elseif(  $rs[$k]['tavid'] == TIPO_AUTO_AVAL ) {  
								if( !avaliacaoFinalizada($_SESSION['cpfavaliado'], TIPO_AUTO_AVAL ) ) {
								 	$valor = '---';  
								 	$pesoCalculado = '---';
									$disabled = "disabled = disabled";
								}else{
									$valor = verificaPontuacao($defid, $_SESSION['cpfavaliado'], $rs[$k]['tavid']); 
									$pesoCalculado = ( $valor * $peso );
									$disabled = "disabled = disabled";
								}?> 
								<td align="center"><input type="text"  size="3" <?=$disabled?> maxlength="3" onkeyup="<?=$mascaraGlobalJs?> calcula( document.getElementById('pesoDefinicao[<?=$indice;?>]').value, this.value , 'div_defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]' , '[<?=$defid?>][<?=$rs[$k]['tavid'];?>]'); calculaColunas();" name="defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]" id="defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]" value="<?=$valor?>"></td>
								<td align="center"><div id="div_defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]"  style="display: '';"><?=$pesoCalculado;?></div></td>
							<?}elseif(  $rs[$k]['tavid'] == TIPO_AVAL_SUPERIOR ){  
								$valor = verificaPontuacao($defid, $_SESSION['cpfavaliado'], $rs[$k]['tavid']); 
								$pesoCalculado = ( $valor * $peso );
								if( avaliacaoFinalizada($_SESSION['cpfavaliado'], TIPO_AVAL_SUPERIOR) ) {
									$disabled = "disabled = disabled";
								}else{
									$disabled = "";
								}?> 
								<td align="center"><input type="text"  size="3" maxlength="3" <?=$disabled; ?>onkeyup="<?=$mascaraGlobalJs?> calcula( document.getElementById('pesoDefinicao[<?=$indice;?>]').value, this.value , 'div_defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]', '[<?=$defid?>][<?=$rs[$k]['tavid'];?>]' ); calculaColunas();" name="defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]" id="defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]" value="<?=$valor?>"></td>
								<td align="center"><div id="div_defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]"  style="display: '';"><?=$pesoCalculado;?></div></td>
							<?}?>
						<?}elseif(  $rs[$k]['tavid'] == TIPO_AUTO_AVAL ) { 
							if( !avaliacaoFinalizada($_SESSION['cpfavaliado'], TIPO_AUTO_AVAL ) ) {
								 	$valor = '---';  
								 	$pesoCalculado = '---';
									$disabled = "disabled = disabled";
								}else{
									$valor = verificaPontuacao($defid, $_SESSION['cpfavaliado'], $rs[$k]['tavid']); 
									$pesoCalculado = ( $valor * $peso );
									$disabled = "disabled = disabled";
								}?> 
							<td align="center"><input type="text" size="3" maxlength="3" disabled = disabled onkeyup="<?=$mascaraGlobalJs?> calcula( document.getElementById('pesoDefinicao[<?=$indice;?>]').value, this.value , 'div_defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]', '[<?=$defid?>][<?=$rs[$k]['tavid'];?>]' ); calculaColunas();" name="defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]" id="defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]" value="<?=$valor?>"></td>
							<td align="center"><div id="div_defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]"  style="display: '';"><?=$pesoCalculado;?></div></td>
						<?}else{ 
								
							if( $rs[$k]['tavid'] == TIPO_AUTO_AVAL ){
						
								if( !avaliacaoFinalizada($_SESSION['cpfavaliado'], TIPO_AUTO_AVAL ) ) {
									
								 	$valor = '---';  
								 	$pesoCalculado = '---';
									$disabled = "disabled = disabled";
								}else{
									
									$disabled = "";
								}
							}elseif( $rs[$k]['tavid'] == TIPO_AVAL_SUPERIOR ){  
								if( avaliacaoFinalizada($_SESSION['cpfavaliado'], TIPO_AVAL_SUPERIOR) ) {  
									$disabled = "disabled = disabled";
								}else{
									$disabled = "";
								}
							}elseif( $rs[$k]['tavid'] == TIPO_AVAL_CONSENSO && !avaliacaoFinalizada($_SESSION['cpfavaliado'], TIPO_AUTO_AVAL) ){
								continue;
								if( avaliacaoFinalizada($_SESSION['cpfavaliado'], TIPO_AVAL_CONSENSO) ) {
									$disabled = "disabled = disabled";
								}else{
									$disabled = "";
								}
							}elseif(!avaliacaoFinalizada($_SESSION['cpfavaliado'], TIPO_AVAL_CONSENSO)){ //pode preencher consenso{
								$disabled = "";
							}else{
								$disabled = "disabled = disabled";
							}
							$valor = verificaPontuacao($defid, $_SESSION['cpfavaliado'], $rs[$k]['tavid']); 
							$pesoCalculado = ( $valor * $peso );
		 
							?> 
							<td align="center"><input type="text" size="3" maxlength="3" <?=$disabled?> onkeyup="<?=$mascaraGlobalJs?> calcula( document.getElementById('pesoDefinicao[<?=$indice;?>]').value, this.value , 'div_defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]', '[<?=$defid?>][<?=$rs[$k]['tavid'];?>]' ); calculaColunas();" name="defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]" id="defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]" value="<?=$valor?>"></td>
							<td align="center"><div id="div_defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]"  style="display: '';"><?=$pesoCalculado;?></div></td>
						<?}?>
					<?}elseif( controlaPermissao('superuser')  ){
					
						$valor = verificaPontuacao($defid, $_SESSION['cpfavaliado'], $rs[$k]['tavid']); 
						$pesoCalculado = ( $valor * $peso );
						$disabled = "";
						?> 
						<td align="center"><input type="text" size="3" maxlength="3" <?=$disabled?> onkeyup="<?=$mascaraGlobalJs?> calcula( document.getElementById('pesoDefinicao[<?=$indice;?>]').value, this.value , 'div_defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]' , '[<?=$defid?>][<?=$rs[$k]['tavid'];?>]'); calculaColunas();" name="defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]" id="defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]" value="<?=$valor?>"></td>
						<td align="center"><div id="div_defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]"  style="display: '';"><?=$pesoCalculado;?></div></td>		
					<?}elseif( controlaPermissao('avaliacao') ){  
					  #caso de ser avaliador e também ser administrador
						if( $rs[$k]['tavid'] == TIPO_AUTO_AVAL ){
							
							if( avaliacaoFinalizada($_SESSION['cpfavaliado'], TIPO_AUTO_AVAL) ) { 
								
								$disabled = "disabled = disabled";
							}else{ 
								
								$disabled = "";
							}
						}elseif( $rs[$k]['tavid'] == TIPO_AVAL_SUPERIOR ){ 
							
							if( avaliacaoFinalizada($_SESSION['cpfavaliado'], TIPO_AVAL_SUPERIOR) ) { 
								
								$disabled = "disabled = disabled";
							}else{
								$disabled = "";
							}
						}elseif( $rs[$k]['tavid'] == TIPO_AVAL_CONSENSO ){ 
							
							if( avaliacaoFinalizada($_SESSION['cpfavaliado'], TIPO_AVAL_CONSENSO) ) {
								$disabled = "disabled = disabled";
							}else{
								$disabled = "";
							}
						} 
						$valor = verificaPontuacao($defid, $_SESSION['cpfavaliado'], $rs[$k]['tavid']); 
						$pesoCalculado = ( $valor * $peso ); 
						?>
						<td align="center"><input type="text" size="3" maxlength="3" <?=$disabled?> onkeyup="<?=$mascaraGlobalJs?> calcula( document.getElementById('pesoDefinicao[<?=$indice;?>]').value, this.value , 'div_defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]' , '[<?=$defid?>][<?=$rs[$k]['tavid'];?>]'); calculaColunas();" name="defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]" id="defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]" value="<?=$valor?>"></td>
						<td align="center"><div id="div_defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]"  style="display: '';"><?=$pesoCalculado;?></div></td>	
					<?} 
				} 
			}
		}elseif( $_SESSION['boautoavaliacao'] ){  
			for( $k =0; $k<count( $rs ); $k++ ){
				if( !avaliacaoFinalizada($_SESSION['cpfavaliado'], TIPO_AVAL_SUPERIOR ) ) {
					if( $rs[$k]['tavid'] == TIPO_AUTO_AVAL ){ 
						if( $tipo == 'TIPO_CABECALHO'){?> 
							<th> <?= $rs[$k]['tavdescricao'] ?> 
							</th> 
							<th> Nota Final </th> 
						<?
						}elseif( $tipo == 'TIPO_COLUNA'){ 
							if( avaliacaoFinalizada($_SESSION['cpfavaliado'], TIPO_AUTO_AVAL) ) {
								$disabled = "disabled = disabled";
							}else{
								$disabled = "";
							}
							$valor = verificaPontuacao($defid, $_SESSION['cpfavaliado'], $rs[$k]['tavid']); 
							$pesoCalculado = ( $valor * $peso );
							
							?> 
							<td align="center"><input type="text" size="3" maxlength="3" <?=$disabled?> onkeyup="<?=$mascaraGlobalJs?> calcula( document.getElementById('pesoDefinicao[<?=$indice;?>]').value, this.value , 'div_defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]' , '[<?=$defid?>][<?=$rs[$k]['tavid'];?>]'); calculaColunas();" name="defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]" id="defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]" value="<?=$valor?>"></td>
							<td align="center"><div id="div_defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]"  style="display: '';"><?=$pesoCalculado;?></div></td>
						<? 
						}	
					}	
					
				}elseif( $rs[$k]['tavid'] == TIPO_AUTO_AVAL ){ 
					if( $tipo == 'TIPO_CABECALHO'){?> 
						<th> <?= $rs[$k]['tavdescricao'] ?> </th> 
						<th> Nota Final </th> 
					<?
					}elseif( $tipo == 'TIPO_COLUNA'){ 
						if( avaliacaoFinalizada($_SESSION['cpfavaliado'], TIPO_AUTO_AVAL) ) {
							$disabled = "disabled = disabled";
						}else{
							$disabled = "";
						}
						$valor = verificaPontuacao($defid, $_SESSION['cpfavaliado'], $rs[$k]['tavid']); 
						$pesoCalculado = ( $valor * $peso );
						
						?> 
						<td align="center"><input type="text" size="3" maxlength="3" <?=$disabled?> onkeyup="<?=$mascaraGlobalJs?> calcula( document.getElementById('pesoDefinicao[<?=$indice;?>]').value, this.value , 'div_defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]' , '[<?=$defid?>][<?=$rs[$k]['tavid'];?>]'); calculaColunas();" name="defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]" id="defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]" value="<?=$valor?>"></td>
						<td align="center"><div id="div_defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]"  style="display: '';"><?=$pesoCalculado;?></div></td>
					<? 
					}	 
				}elseif( $rs[$k]['tavid'] == TIPO_AVAL_SUPERIOR ){ 
					if( $tipo == 'TIPO_CABECALHO'){?> 
						<th> <?= $rs[$k]['tavdescricao'] ?> </th> 
						<th> Nota Final </th> 
					<?
					}elseif( $tipo == 'TIPO_COLUNA'){ 						 
						$disabled = "disabled = disabled";						 
						$valor = verificaPontuacao($defid, $_SESSION['cpfavaliado'], $rs[$k]['tavid']); 
						$pesoCalculado = ( $valor * $peso );						
						?> 
						<td align="center"><input type="text" size="3" maxlength="3" <?=$disabled?> onkeyup="calcula( document.getElementById('pesoDefinicao[<?=$indice;?>]').value, this.value , 'div_defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]' , '[<?=$defid?>][<?=$rs[$k]['tavid'];?>]'); calculaColunas();" name="defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]" id="defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]" value="<?=$valor?>"></td>
						<td align="center"><div id="div_defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]"  style="display: '';"><?=$pesoCalculado;?></div></td>
					<? 
					}	 
				}elseif( $rs[$k]['tavid'] == TIPO_AVAL_CONSENSO ){ 
					if( $tipo == 'TIPO_CABECALHO'){?> 
						<th> <?= $rs[$k]['tavdescricao'] ?>
						<?php if( !verificaMediaConsenso() && $rs[$k]['tavid'] == TIPO_AVAL_CONSENSO ){ echo '<br> ( Média )'; } ?>
						</th> 
						<th> Nota Final </th> 
					<?
					}elseif( $tipo == 'TIPO_COLUNA'){  
						$disabled = "disabled = disabled"; 
						$valor = verificaPontuacao($defid, $_SESSION['cpfavaliado'], $rs[$k]['tavid']); 
						$pesoCalculado = ( $valor * $peso ); 
						?> 
						<td align="center"><input type="text" size="3" maxlength="3" <?=$disabled?> onkeyup="<?=$mascaraGlobalJs?> calcula( document.getElementById('pesoDefinicao[<?=$indice;?>]').value, this.value , 'div_defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]' , '[<?=$defid?>][<?=$rs[$k]['tavid'];?>]'); calculaColunas();" name="defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]" id="defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]" value="<?=$valor?>"></td>
						<td align="center"><div id="div_defid[<?=$defid?>][<?=$rs[$k]['tavid'];?>]"  style="display: '';"><?=$pesoCalculado;?></div></td>
					<? 
					} 
				}
			}
		} 
	}
}
function existeNotaAvaliado($cpf){
	global $db;
	$sql = "SELECT resnota FROM gestaopessoa.respostaavaliacao 
			WHERE sercpf = '$cpf' 
			AND tavid = ".TIPO_AVAL_SUPERIOR."
			AND resavaliacaopendente = 'f'
			AND resano = {$_SESSION['exercicio']}";
	$existe = $db->pegaUm( $sql );
	if( $existe ){
		return true;
	}else{
		return false;
	}
} 
function avaliacaoFinalizada($cpf, $tipo){
	global $db;
	$sql = "SELECT resavaliacaopendente 
			FROM gestaopessoa.respostaavaliacao AS r
			INNER JOIN gestaopessoa.definicao AS d ON d.defid = r.defid
			WHERE r.sercpf = '$cpf' AND d.defanoreferencia = '".date("Y")."' 
			AND r.resavaliacaopendente = 'f'
			AND r.tavid = $tipo";
	$existe = $db->pegaUm( $sql );
	if( $existe ){ 
		return true;
	}else{
		return false;
	} 
} 
function getDadosPessoa($cpf){
	global $db; 
	$sql = "SELECT  s.sernome, s.sersiape, s.sercargo , s.sercpfchefe, s2.sernome as chefe, u.usucpf FROM gestaopessoa.servidor AS s
			LEFT JOIN seguranca.usuario AS u ON u.usucpf = s.sercpf  
			left join gestaopessoa.servidor as s2 on s.sercpfchefe = s2.sercpf
	WHERE u.usucpf = '$cpf'
	AND s2.seranoreferencia = {$_SESSION['exercicio']}
	AND s.seranoreferencia = {$_SESSION['exercicio']}";
	$rs = $db->carregar( $sql );
	if( $rs ){
		$dados = array();	
		array_push( $dados, $rs[0]['usucpf'], $rs[0]['sernome'], $rs[0]['sersiape'] , $rs[0]['sercargo'], $rs[0]['chefe']);
	} 
	return $dados;
}
function existeServidorUsuario(){
	global $db;
	$sql = "SELECT sercpf FROM gestaopessoa.servidor WHERE sercpf = '".$_SESSION['usucpf']."'";
	$existe = $db->pegaUm( $sql );
	if( $existe ){
		return true;
	}else{
		return false;
	}
}
function direcionaAvaliador( $usucpf ){
	global $db;
	$sql = "SELECT s.sercpf FROM gestaopessoa.servidor AS s 
			INNER JOIN seguranca.usuario AS u ON u.usucpf = s.sercpf
			WHERE s.sercpfchefe = '$usucpf'"; 
	$existe = $db->pegaUm( $sql );
	$perfis =  arrayPerfil( $usucpf ); 
	if( $existe || in_array( PERFIL_SUPER_USER  , $perfis) ){ 
		 return true;
	}elseif(!existeServidorUsuario()) { 
		//echo "<script>alert('Servidor não cadastrado')</script>";
		echo'<script> alert(\'Servidor não cadastrado, Favor encaminhar para o link de RH de sua Unidade os seguintes dados:\n\nCPF\nSIAPE\nNome Servidor\nCargo\nSituaçao Funcional\nfunçao\nLotação\nCPF Chefia\nNome Chefia\nSIAPE Chefia \');</script>';
		echo("<script>window.location.href = 'gestaopessoa.php?modulo=inicio&acao=C';</script>"); 
	}else{
		$_SESSION['cpfavaliado']    = $usucpf;
		$_SESSION['boautoavaliacao'] = true;
		header("Location: ?modulo=principal/formularioAvaliacao&acao=A");
	}
}
function verificaPontuacao($defid, $sercpf, $tavid ){
	global $db; 
 
	$sql = "SELECT resnota FROM gestaopessoa.respostaavaliacao WHERE 
			defid = $defid AND sercpf = '$sercpf' AND tavid = $tavid";
	$valor = $db->pegaUm( $sql );
	return $valor;
}
function arrayPerfil(){
	global $db; 
	$sql = sprintf("SELECT
					 pu.pflcod
					FROM
					 seguranca.perfilusuario pu
					 INNER JOIN seguranca.perfil p ON p.pflcod = pu.pflcod AND
					 	p.sisid = 64
					WHERE
					 pu.usucpf = '%s'
					ORDER BY
					 p.pflnivel",
				$_SESSION['usucpf']);
	return (array) $db->carregarColuna($sql,'pflcod');
}
function controlaPermissao($tipo){
	$perfis = arrayPerfil(); 
	 
	switch( $tipo ){
		case 'lista_completa':
			if( !in_array( PERFIL_ADMINISTRADOR, $perfis ) && 
				!in_array( PERFIL_SUPER_USER, $perfis ) && 
				!in_array( PERFIL_CONSULTA, $perfis )  ) {
				return false;
			}else{
				return true;	
			}
			break;
		case 'consulta':
			if( in_array( PERFIL_CONSULTA, $perfis ) && count( $perfis == 1 ) ){
				return true;
			}else{
				return false;
			}
			break;
		case 'superuser':
			if( in_array( PERFIL_SUPER_USER, $perfis ) && count( $perfis == 1 ) ){
				return true;
			}else{
				return false;
			}
			break;
		case 'administrador':
			if( in_array( PERFIL_ADMINISTRADOR, $perfis ) && count( $perfis == 1 ) ){
				return true;
			}else{
				return false;
			}
			break;
		case 'avaliacao':
			if( in_array( PERFIL_AVALIACAO, $perfis ) && count( $perfis == 1 ) ){
				return true;
			}else{
				return false;
			}
			break;
	}
}
function getAvaliadorRodapeHTML(){
	global $db;
	
	$sql = "SELECT tavid, tavdescricao FROM gestaopessoa.tipoavaliador WHERE tavstatus = 'A'";
	$rs  = $db->carregar( $sql );
	$perfis = arrayPerfil();
	if( $rs ){
		if( !$_SESSION['boautoavaliacao'] ){  
			for( $k =0; $k<count( $rs ); $k++ ){ 
				if(   ( in_array( PERFIL_AVALIACAO, $perfis ) && !soConsulta()) ){
					if( !existeNotaAvaliado($_SESSION['cpfavaliado']) ){
						if( $rs[$k]['tavid'] == TIPO_AVAL_CONSENSO ) { 
							continue;
							?> 
							<td align="center" id="total_consenso" > </td>
							<td align="center"><div id="total_consenso_p"  style="display: '';"></div></td>
						<?}elseif(  $rs[$k]['tavid'] == TIPO_AUTO_AVAL ) {   
							?> 
							<td align="center" id="total_auto_aval" > </td>
							<td align="center"><div id="total_auto_aval_p"  style="display: '';"></div></td>
						<?}else{   
							?> 
							<td align="center" id="total_aval_superior"> </td>
							<td align="center"><div id="total_aval_superior_p"  style="display: '';"></div></td>
						<?}?>
					<?}elseif(  $rs[$k]['tavid'] == TIPO_AUTO_AVAL ) {  
						?> 
						<td align="center" id="total_auto_aval"> </td>
						<td align="center"><div id="total_auto_aval_p"  style="display: '';"></div></td>
					<?}elseif(  ( $rs[$k]['tavid'] == TIPO_AVAL_CONSENSO) && avaliacaoFinalizada($_SESSION['cpfavaliado'], TIPO_AUTO_AVAL ) ){  
						?> 
						<td align="center" id="total_consenso"> </td>
						<td align="center"><div id="total_consenso_p"  style="display: '';"></div></td>
					<?} else {  
						?> 
						<td align="center" id="total_aval_superior" > </td>
						<td align="center"><div id="total_aval_superior_p"  style="display: '';"></div></td>
					<?}?>
				<?}else{
					if( $rs[$k]['tavid'] == TIPO_AUTO_AVAL ) {?>
						<td align="center" id="total_auto_aval"> </td>
						<td align="center"><div id="total_auto_aval_p"  style="display: '';"></div></td>	
					<?}elseif( $rs[$k]['tavid'] == TIPO_AVAL_CONSENSO ){ ?>	
						<td align="center" id="total_consenso"> </td>
						<td align="center"><div id="total_consenso_p"  style="display: '';"></div></td>	
					<? }else{ ?>
						<td align="center" id="total_aval_superior"> </td>
						<td align="center"><div id="total_aval_superior_p"  style="display: '';"></div></td>	
					<? } ?>
				<?}  
			} 
		}elseif( $_SESSION['boautoavaliacao'] ){  
			for( $k =0; $k<count( $rs ); $k++ ){ 
				if( !avaliacaoFinalizada($_SESSION['cpfavaliado'], TIPO_AVAL_SUPERIOR ) ) {
					if($rs[$k]['tavid'] == TIPO_AUTO_AVAL){?>
						<td align="center" id="total_auto_aval"> </td>
						<td align="center"><div id="total_auto_aval_p"  style="display: '';"></div></td>	
						<?				
					}
				}elseif($rs[$k]['tavid'] == TIPO_AUTO_AVAL ){?>
					<td align="center" id="total_auto_aval"> </td>
					<td align="center"><div id="total_auto_aval_p"  style="display: '';"></div></td>	
					<?				
				}elseif($rs[$k]['tavid'] == TIPO_AVAL_SUPERIOR ){?>
					<td align="center" id="total_aval_superior"> </td>
					<td align="center"><div id="total_aval_superior_p"  style="display: '';"></div></td>	
					<?				
				}elseif($rs[$k]['tavid'] == TIPO_AVAL_CONSENSO ){?>
					<td align="center" id="total_consenso"> </td>
					<td align="center"><div id="total_consenso_p"  style="display: '';"></div></td>	
					<?				
				}
			}
		} 
	}
}
function soConsulta(){
	global $db;
	$sql = "SELECT sercpfchefe FROM gestaopessoa.servidor WHERE sercpf = '".$_SESSION['cpfavaliado']."'"; 
	$cpf = $db->pegaUm( $sql );  
	if( $cpf == $_SESSION['usucpf'] ){  
		return false; 
	}else{ 
		return true;
	}
}
function getQuantidade($tipo ){
	global $db;
	if( $tipo ){
		if( $tipo == TIPO_AVAL_CONSENSO ){
			$and = " AND s.sermediaconsenso = 'f'";
		}
		$sql = "SELECT coalesce( count(distinct(s.sercpf)), 0) FROM gestaopessoa.servidor AS s
				WHERE s.sercpf IN ( SELECT sercpf FROM gestaopessoa.respostaavaliacao 
									WHERE TAVID = $tipo 
									AND resavaliacaopendente = 'f'
									AND resano = {$_SESSION['exercicio']} )
									$and
									AND seranoreferencia = {$_SESSION['exercicio']}";
		$valor = $db->pegaUm( $sql );
		echo $valor; 
	}
}
function getQtdMedia(){
	global $db; 
	$sql = "SELECT coalesce( count(distinct(s.sercpf)), 0) FROM gestaopessoa.servidor AS s
			where s.sermediaconsenso = 't'
	";
	$valor = $db->pegaUm( $sql );
	echo $valor;  
}
function qtdServidores($cadastrados = FALSE){
	global $db;
	if( $cadastrados ){
		$sql = "
		SELECT count(distinct(s.sercpf)) FROM gestaopessoa.servidor AS s
		INNER JOIN seguranca.usuario AS u ON s.sercpf = u.usucpf
		INNER JOIN seguranca.usuario_sistema AS us ON us.usucpf = u.usucpf
		WHERE u.usucpf IS NOT NULL AND us.sisid = 64 
		AND s.tssid IN (".SITUACAO_ATIVO_PERMANENTE.", 
				  	    				".SITUACAO_CEDIDO.", 
				  	    				".SITUACAO_EXCEDENTE.", 
				  	    				".SITUACAO_ATIVO_PERM_L.", 
				   	    				".SITUACAO_ANISTIADO.", 
				 	    				".SITUACAO_EXERC.")
 	    AND seranoreferencia = {$_SESSION['exercicio']}";
	
	}else{
		$sql = "SELECT count(distinct(s.sercpf)) FROM gestaopessoa.servidor AS s   
				WHERE s.tssid IN (".SITUACAO_ATIVO_PERMANENTE.", 
				  	    				".SITUACAO_CEDIDO.", 
				  	    				".SITUACAO_EXCEDENTE.", 
				  	    				".SITUACAO_ATIVO_PERM_L.", 
				   	    				".SITUACAO_ANISTIADO.", 
				 	    				".SITUACAO_EXERC.")
				AND seranoreferencia = {$_SESSION['exercicio']}";
	}
			 
	$qtd = $db->pegaUm( $sql );
	echo $qtd; 
}
function getSituacaoMEC($cpf){
	global $db;
	$sql = "SELECT fstid FROM gestaopessoa.ftdadopessoal WHERE fdpcpf = '".$cpf."'";
	$tipo = $db->pegaUm( $sql );
	if( $tipo ){
		return $tipo;
	}
}
function controlaDadoFuncional( $tipo ){
	global $db;
	include_once( APPRAIZ. "gestaopessoa/classes/FtDadoFuncional.class.inc" );
	$df = new FtDadoFuncional(); 
	switch( $tipo ){
		case VINCULO_EFETIVO:
			return $df->arEfetivo;
		break;
		case VINCULO_CEDIDO:
			return $df->arCedido;
		break;
		case VINCULO_CTU:
			return $df->arCTU;
		break;
		case VINCULO_CONSULTOR:
			return $df->arConsultor;
		break;
		case VINCULO_EXERCICIODES:
			return $df->arExercicioDes;
		break;
		case VINCULO_EXERCICIOPRO:
			return $df->arExercicioPro;
		break;
		case VINCULO_TERCEIRIZADO:
			return $df->arTerceirizado;
		break;
		case VINCULO_ANISTIADO_CLT:
			return $df->arAnistiadoCLT;
		break;
		case VINCULO_CARGOCOMISSIONADO:
			return $df->arCargoComissionado;
		break;
		case VINCULO_REQUISITADO:
			return $df->arRequisitados;
		break;
		case VINCULO_COLABORACAO_TECNICA:
			return $df->arColaboracaoTecnica;
		break;
	}  
}
function controlaPefilFT($operacao){
	$arPerfis = arrayPerfil();
	/*
	 * 	define( "PERFIL_FT_CONSULTA_GERAL",334);
		define( "PERFIL_FT_ADMINISTRADOR_GERAL",335);
		define( "PERFIL_FT_ADMINISTRADOR_CONTRATO",336);
		define( "PERFIL_FT_ADMINISTRADOR_PESSOAL",337);
		define( "PERFIL_FT_ADMINISTRADOR_PROJETO",338);
		define( "PERFIL_FT_FISCAL_CONTRATO",339);
		define( "PERFIL_FT_FISCAL_PESSOAL",340);
		define( "PERFIL_FT_FISCAL_PROJETO",341);

		define( "PERFIL_SERVIDOR",397);
		define( "PERFIL_CONSULTOR",398);
		define( "PERFIL_TERCEIRIZADO",399);
	 */
	switch( $operacao ){
		case 'soConsulta':
			if( in_array(  PERFIL_FT_CONSULTA_GERAL, $arPerfis ) && count( $arPerfis == 0 )){
				return true;
			}else{
				return false;
			}
			break;
		case 'permissaoTotal':
			if( in_array(  PERFIL_SUPER_USER, $arPerfis  ) || 
				in_array(  PERFIL_FT_ADMINISTRADOR_GERAL, $arPerfis) ||
				in_array(  PERFIL_SUPER_USER, $arPerfis ) ){
					return true;
			}else{
				return false;
			}
		case 'vinculosPermitidos':
			if( in_array( PERFIL_SUPER_USER , $arPerfis) ){
						 $arPermitidos = array();
						 array_push( $arPermitidos,   VINCULO_EFETIVO,
													  VINCULO_CEDIDO,
													  VINCULO_CTU,
													  VINCULO_CONSULTOR,
													  VINCULO_EXERCICIODES,
													  VINCULO_EXERCICIOPRO,
													  VINCULO_TERCEIRIZADO,
													  VINCULO_ANISTIADO_CLT, 
													  VINCULO_CARGOCOMISSIONADO,
													  VINCULO_REQUISITADO,
													  VINCULO_COLABORACAO_TECNICA );
			}elseif( in_array( PERFIL_FT_FISCAL_PESSOAL, $arPerfis ) || in_array( PERFIL_SERVIDOR, $arPerfis ) ){
						 $arPermitidos = array();
						 array_push( $arPermitidos,   VINCULO_EFETIVO,
													  VINCULO_CEDIDO,
													  VINCULO_CTU, 
													  VINCULO_EXERCICIODES,
													  VINCULO_EXERCICIOPRO, 
													  VINCULO_ANISTIADO_CLT, 
													  VINCULO_CARGOCOMISSIONADO,
													  VINCULO_REQUISITADO,
													  VINCULO_COLABORACAO_TECNICA );
			}elseif( in_array( PERFIL_FT_FISCAL_CONTRATO, $arPerfis ) || in_array( PERFIL_TERCEIRIZADO, $arPerfis ) ){
						 $arPermitidos = array();
						 array_push( $arPermitidos,   VINCULO_TERCEIRIZADO );
			}elseif( in_array( PERFIL_FT_FISCAL_PROJETO, $arPerfis ) || in_array( PERFIL_CONSULTOR, $arPerfis ) ){
						 $arPermitidos = array();
						 array_push( $arPermitidos,   VINCULO_CONSULTOR );
			}
		 
			return $arPermitidos;
	}
}
function prazoVencido(){
	include_once APPRAIZ . "includes/classes/dateTime.inc";
	$data = new Data();
	$agora	  = $data->timeStampDeUmaData( date("d/m/Y") );
	$limite   = $data->timeStampDeUmaData( "30/12/2010" ); // utilizar outra data limite
	if( $agora > $limite ){
		return true;
	}else{
		return false; 
	}
}
function verificaMediaConsenso(){
	global $db;
	$boSql = "SELECT sermediaconsenso FROM gestaopessoa.servidor WHERE 
 			 sercpf = '".$_SESSION['cpfavaliado']."'";
	if( $boMedia = $db->pegaUm($boSql) ){
		if( $boMedia == 't'){
			return false;	
		}
		return true;
	}
	return true;	
}
function direcionaFT(){
	global $db;
	$arPerfis = arrayPerfil();
	if( (in_array( PERFIL_SERVIDOR, 	$arPerfis ) ||
		in_array( PERFIL_CONSULTOR, 	$arPerfis ) ||
		in_array( PERFIL_TERCEIRIZADO,  $arPerfis ))		 
		&& 
		!in_array( PERFIL_FT_ADMINISTRADOR_GERAL,	$arPerfis )
		) {
			
		unset($_SESSION['fdpcpf']);
		$fdpCpf = $db->pegaUm( "select fdpcpf from gestaopessoa.ftdadopessoal where fdpcpf = '". $_SESSION['usucpf']."'" ); 
		if( $fdpCpf ){ 	
			include_once( APPRAIZ. "gestaopessoa/classes/FtDadoPessoal.class.inc" ); 
			$ft = new FtDadoPessoal();
			$ft->carregarPorId( "'".$fdpCpf."'" );	
			$sql = "SELECT * FROM gestaopessoa.ftdadopessoal WHERE fdpcpf = '".$fdpCpf."'";
			$dados = $db->carregar( $sql );
		 	$_SESSION['fdpcpf'] = $fdpCpf;
		}
		header("Location: ?modulo=principal/cadDadosPessoais&acao=A");
	}else{
		if( !in_array( PERFIL_SERVIDOR, 	 $arPerfis )&
			!in_array( PERFIL_CONSULTOR, 	 $arPerfis )&
			!in_array( PERFIL_SUPER_USER, 	 $arPerfis )&
			!in_array( PERFIL_TERCEIRIZADO,  $arPerfis )& 
			!in_array( PERFIL_FT_ADMINISTRADOR_GERAL,  $arPerfis )&  
			!in_array( PERFIL_FT_CONSULTA_GERAL,  $arPerfis ) ){ 
			?>
			<script> alert('Acesso negado. Usuário sem perfil cadastrado no sistema.'); </script>
			<script> window.location.href = '?modulo=inicio&acao=C'; </script>
			<?
			exit;
			}
	}
}

function bloqueiaEdicaoFT() {
	
	$perfis = arrayPerfil();
	
	if( !in_array( PERFIL_ADMINISTRADOR, $perfis ) && 
		!in_array( PERFIL_SUPER_USER, $perfis ) && 
		!in_array( PERFIL_CONSULTA, $perfis ) &&
		!in_array( PERFIL_AVALIACAO, $perfis ) &&
		!in_array( PERFIL_TERCEIRIZADO, $perfis ) &&
		!in_array( PERFIL_SERVIDOR, $perfis ) ) {		
	
		$resultado = "disabled=disabled";
				
	} else {
		 
		$resultado = "";
	}
	
	return $resultado;
}

function ft_monta_sql_relatorio(){
	
	$where = array();
	
	extract($_REQUEST);	
	
	// Situação no MEC
	if( $fstid[0] && $fstid_campo_flag ){
		array_push($where, " st.fstid " . (!$fstid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $fstid ) . "') ");
	}
		
	// Estado Civil
	if( $eciid[0] && $eciid_campo_flag ){
		array_push($where, " dp.eciid " . (!$eciid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $eciid ) . "') ");
	}

	// Sexo
	if ( $fdpsexo ) {
		array_push($where, " dp.fdpsexo IN ('" . implode( "','", $fdpsexo ) . "') ");				
	}
	
	// Data de Nascimento	
	if( $dtnascinicio && !$dtnascfim){
		$dtnascinicio 	= explode("/", $dtnascinicio);
		$dtnascinicio 	= $dtnascinicio[2]."-".$dtnascinicio[1]."-".$dtnascinicio[0];
		array_push($where, " us.usudatanascimento = '" . $dtnascinicio . "' ");
	} elseif ($dtnascinicio && $dtnascfim) {
		$dtnascinicio 	= explode("/", $dtnascinicio);
		$dtnascinicio 	= $dtnascinicio[2]."-".$dtnascinicio[1]."-".$dtnascinicio[0];
		$dtnascfim 		= explode("/", $dtnascfim);
		$dtnascfim		= $dtnascfim[2]."-".$dtnascfim[1]."-".$dtnascfim[0]; 
		array_push($where, " us.usudatanascimento >= '" . $dtnascinicio . "' AND us.usudatanascimento <= '" . $dtnascfim . "' ");
	}
	
	// UF
	if( $estuf[0]  && $estuf_campo_flag ){
		array_push($where, " dp.estuf " . (!$estuf_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "') ");
	}
	
	// Grupo Sanguineo	
	if( $fdpgruposanguineo ){
		array_push($where, " dp.fdpgruposanguineo IN ('" . implode( "','", $fdpgruposanguineo ) . "') ");
	}
	
	// Fator RH	
	if( $fdpfatorrh ){
		array_push($where, " dp.fdpfatorrh IN ('" . implode( "','", $fdpfatorrh ) . "') ");
	}	
	
	// Pessoa com Deficiência
	if ( $fdpdeficiente ) {
		array_push($where, " dp.fdpdeficiente IN ('" . implode( "','", $fdpdeficiente ) . "') ");		
	}
	
	// Tipo de Deficiência
	if( $fdpdeficiencia[0]  && $fdpdeficiencia_campo_flag ){
		array_push($where, " dp.fdpdeficiencia " . (!$fdpdeficiencia_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $fdpdeficiencia ) . "') ");
	}
	
	// Cargo Efetivo no MEC
	if( $fcmid[0]  && $fcmid_campo_flag ){
		array_push($where, " df.fcmid " . (!$fcmid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $fcmid ) . "') ");
	}
	
	// Exerce Cargo ou Função
	if ( $fdpexercecargofuncao ) {
		array_push($where, " df.fdfexercecargofuncao IN ('" . implode( "','", $fdpexercecargofuncao ) . "') ");			
	}	
	
	// Unidade de Lotação
	if( $fulid[0]  && $fulid_campo_flag ){
		array_push($where, " df.fulid " . (!$fulid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $fulid ) . "') ");
	}
	
	// Grau de Escolaridade
	if( $tfoid[0]  && $tfoid_campo_flag ){
		array_push($where, " fa.tfoid " . (!$tfoid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $tfoid ) . "') ");
	}
	
	// Situação do Curso
	if( $ffasituacao ){
		array_push($where, " fa.ffasituacao = '" . $ffasituacao . "' ");
	}

	// Ano de Conclusão do Curso
	if( $ffaanoconclusao ){
		array_push($where, " fa.ffaanoconclusao = '" . $ffaanoconclusao . "' ");
	}
	
	// Idioma
	if( $ftiid[0]  && $ftiid_campo_flag ){
		array_push($where, " id.ftiid " . (!$ftiid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $ftiid ) . "') ");
	}
	
	// Conceito Idioma	
	if( $ftcidleitura ){		
		array_push($where, " id.ftcidleitura IN ('" . implode( "','", $ftcidleitura ) . "') ");
	}
	if( $ftcidfala ){		
		array_push($where, " id.ftcidfala IN ('" . implode( "','", $ftcidfala ) . "') ");
	}
	if( $ftcidescrita ){		
		array_push($where, " id.ftcidescrita IN ('" . implode( "','", $ftcidescrita ) . "') ");
	}
	
	// Atividade Desenvolvida
	if( $ftaid[0]  && $ftaid_campo_flag ){
		array_push($where, " ad.ftaid " . (!$ftaid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $ftaid ) . "') ");
	}
	
	// Nível de atividade desenvolvida 
	if( $fnaid ){		
		array_push($where, " ad.fnaid IN ('" . implode( "','", $fnaid ) . "') ");
	}
	
	// Tipo de Experiência Anterior
	if( $fteid[0]  && $fteid_campo_flag ){
		array_push($where, " ea.fteid " . (!$fteid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $fteid ) . "') ");
	}
	
	// MONTA SQL
	$sql = "SELECT 

				UPPER(fdpnome) as nomedapessoa,
				fdpnome as nomedapessoaxls,	 
				fstdescricao as fstid, 
				fuldescricao as fulid, 
				estuf, 
				df.fcmid as fcmid,
				tf.tfodsc as tfoid,
				ti.ftidescricao as ftiid, 
				ta.ftadescricao as ftaid, 
				te.ftedescricao as fteid,
				INITCAP(dp.fdpdeficiencia) as fdpdeficiencia,
				INITCAP(fa.ffacurso) as ffacurso				 
				
			FROM gestaopessoa.ftdadopessoal dp
			
			INNER JOIN gestaopessoa.ftdadofuncional df 				ON df.fdpcpf = dp.fdpcpf
			INNER JOIN gestaopessoa.ftformacaoacademica fa			ON fa.fdpcpf = dp.fdpcpf
			INNER JOIN gestaopessoa.idioma id						ON id.fdpcpf = dp.fdpcpf
			INNER JOIN gestaopessoa.ftatividadedesenvolvida ad		ON ad.fdpcpf = dp.fdpcpf
			INNER JOIN gestaopessoa.ftexperienciaanterior ea		ON ea.fdpcpf = dp.fdpcpf
			INNER JOIN gestaopessoa.ftsituacaotrabalhador st 		ON st.fstid = dp.fstid
			LEFT JOIN gestaopessoa.ftcargoefetivomec ce 			ON ce.fcmid = df.fcmid
			INNER JOIN gestaopessoa.ftunidadelotacao ul				ON ul.fulid = df.fulid
			INNER JOIN gestaopessoa.fttipoatividadedesenvolvida ta	ON ta.ftaid = ad.ftaid
			INNER JOIN gestaopessoa.ftitipoidioma ti				ON ti.ftiid = id.ftiid
			INNER JOIN gestaopessoa.fttipoexperienciaanterior te	ON te.fteid = ea.fteid
			INNER JOIN seguranca.usuario us							ON us.usucpf = dp.fdpcpf
			INNER JOIN public.tipoformacao tf						ON tf.tfoid = fa.tfoid			
			
			/*
			INNER JOIN public.estadocivil ec 						ON ec.eciid = dp.eciid --Pode sair
			INNER JOIN territorios.estado et 						ON et.estuf = dp.estuf --Pode sair
			*/
						 			
			" . ( is_array($where) ? ' WHERE' . implode(' AND ', $where) : '' )	. $stFiltro . "
			
			GROUP BY 
				nomedapessoa, nomedapessoaxls, st.fstdescricao, ul.fuldescricao, dp.estuf, df.fcmid, df.fulid, fa.tfoid,
				id.ftiid, ad.ftaid, ea.fteid, ta.ftadescricao, ti.ftidescricao, tf.tfodsc, te.ftedescricao, dp.fdpdeficiencia,
				fa.ffacurso
			 
			ORDER BY
				" . (is_array( $agrupador ) ?  implode(",", $agrupador) : "pais");		
//	ver($sql, $_REQUEST);
//	die();	
	return $sql;
	
}

function ft_monta_agp_relatorio(){
	
	$agrupador = $_REQUEST['agrupadorNovo'] ? $_REQUEST['agrupadorNovo'] : $_REQUEST['agrupador'];	
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array("fstid",
										  "fulid",
										  "tfoid",
										  "fdpdeficiencia",
										  "ffacurso"),	  
				);	
		
				
	foreach ( $agrupador as $val ){
		switch( $val ){
			case "fstid":
				array_push($agp['agrupador'], array(
													"campo" => "fstid",
											  		"label" => "Situação no MEC")										
									   				);
			break;
			case "estuf":
				array_push($agp['agrupador'], array(
													"campo" => "estuf",
											  		"label" => "UF")										
									   				);
			break;
			case "fcmid":
				array_push($agp['agrupador'], array(
													"campo" => "fcmid",
											  		"label" => "Cargo Efetivo no MEC")										
									   				);
			break;
			case "fulid":
				array_push($agp['agrupador'], array(
													"campo" => "fulid",
											  		"label" => "Unidade de Lotação")										
									   				);
			break;
			case "tfoid":
				array_push($agp['agrupador'], array(
													"campo" => "tfoid",
											  		"label" => "Grau de Escolaridade")										
									   				);
			break;
			case "ftiid":
				array_push($agp['agrupador'], array(
													"campo" => "ftiid",
											  		"label" => "Idioma")										
									   				);
			break;

			case "ftaid":
				array_push($agp['agrupador'], array(
													"campo" => "ftaid",
											  		"label" => "Atividade Desenvolvida")										
									   				);
			break;
			/*
			case "fdpdeficiencia":
				array_push($agp['agrupador'], array(
													"campo" => "fdpdeficiencia",
											  		"label" => "Deficiência")										
									   				);
			break;
			*/
			case "fteid":
				array_push($agp['agrupador'], array(
													"campo" => "fteid",
											  		"label" => "Tipo de Experiência Anterior")										
									   				);
			break;
			
			case "nomedapessoa":
				array_push($agp['agrupador'], array(
													"campo" => "nomedapessoa",
											  		"label" => "Nome da Pessoa")										
									   				);
			break;
			case "nomedapessoaxls":
				array_push($agp['agrupador'], array(
													"campo" => "nomedapessoaxls",
											  		"label" => "Nome da Pessoa")										
									   				);
			break;
			case "nivelpreenchimento":
				array_push($agp['agrupador'], array(
													"campo" => "nivelpreenchimento",
											  		"label" => "Nível de Preenchimento")										
									   				);
			break;									
		}	
	}
	
	array_push($agp['agrupador'], array(
										"campo" => "nomedapessoa",
								  		"label" => "Nome da Pessoa")										
						   				);
	
	
	return $agp;
	
}

function ft_monta_coluna_relatorio(){
	
	global $_REQUEST;	
	
	$coluna = array();
	
	/*foreach ( $_REQUEST['modalidade'] as $valor ){		
		
		switch( $valor ){
			
			case 'M':
				array_push( $coluna, array("campo" 	  => "medio",
								   		   "label" 	  => "Ensino Médio",
								   		   "blockAgp" => "nomedaescola",
								   		   "type"	  => "character") );
			break;
			case 'F':
				array_push( $coluna, array("campo" 	  => "fundamental",
								   		   "label" 	  => "Ensino Fundamental",
								   		   "blockAgp" => "nomedaescola",
								   		   "type"	  => "character") );
			break;			
		}
		
	}*/		
		
	array_push( $coluna, array(			"campo" 	=> "fstid",
								   		"label" 	=> "Situação no MEC",
								   		//"blockAgp" 	=> "nomedapessoa",
								   		"type"	 	=> "character") );
	
	array_push( $coluna, array(			"campo" 	=> "fulid",
								   		"label" 	=> "Unidade de Lotação",
								   		//"blockAgp" 	=> "nomedapessoa",
								   		"type"	 	=> "character") );
	
	array_push( $coluna, array(			"campo" 	=> "tfoid",
								   		"label" 	=> "Formação",
								   		//"blockAgp" 	=> "nomedapessoa",
								   		"type"	 	=> "character") );
	
	array_push( $coluna, array(			"campo" 	=> "ffacurso",
								   		"label" 	=> "Curso",
								   		//"blockAgp" 	=> "nomedapessoa",
								   		"type"	 	=> "character") );
	
	if($_REQUEST['fdpdeficiente'][0] != ''){
		foreach($_REQUEST['fdpdeficiente'] as $dados){
				
			if($dados['fdpdeficiente'] == 't'){			
				array_push( $coluna, array(			"campo" 	=> "fdpdeficiencia",
											   		"label" 	=> "Deficiência",
											   		//"blockAgp" 	=> "nomedapessoa",
											   		"type"	 	=> "character") );
			}
		}
	}
	
	return $coluna;
	
}