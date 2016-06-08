<?php
class OSController extends Controller{
//	const SITUACAOOSGERADA = 1;
	
	private $obModel;
	
//	function sqlGrupoOS($coluna = null, Array $arWhere = null){
//		return $this->obModel['OrdemServico']->sqlGrupoOS($coluna, $arWhere);
//	}
	function getEmpresa(){
		if ( $this->epcid ){
			$obModelEmpresa =  new EmpresaContratada();
			$nomeEmpresa    = $obModelEmpresa->buscaNomeEmpresa( $this->epcid );
		}
		
		return $nomeEmpresa;
	}	
	
	function carregaDadosEmpresa(){
		if ( $this->epcid ){
			$obModelEmpresa =  new EmpresaContratada();
			$arDados    	= $obModelEmpresa->buscaDadosEmpresa( $this->epcid );
			$this->carregaDados($arDados, array("empresa", "cnpj", "epcnumproceconc", "epcnumcontrato"));
		}
	}	
	
	function buscaTiposDeslocamento( $gpdid = null ){
		$gpdid 	 = $gpdid ? $gpdid : $this->gpdid;
		$obModel = new TipoDeslocamento();  
		return $obModel->buscaTipoDeslocamentoPorGrupo( array("gpdid" => $gpdid) );	
	}
	
	function ativaDadosArquivo(Array $arAtt = null, $arqid = null, Array $arParam = null){
		$arqid = $arqid ? $arqid : $this->arqid;  
		$this->carregaDadosModel( new Arquivo( $arqid ), $arAtt );
	}
	
	function ativaDadosGrupo(Array $arAtt = null, $gpdid = null, Array $arParam = null){
		$gpdid = $gpdid ? $gpdid : $this->gpdid;  
//		if ( $gpdid ){
//			$this->obModel['GrupoDistribuicao'] = new GrupoDistribuicao( $gpdid );
//			$this->carregaDadosModel( $this->obModel['GrupoDistribuicao'], $arAtt );
			$this->carregaDadosModel( new GrupoDistribuicao( $gpdid ), $arAtt );
//		}
	}
	
	function ativaDadosOrdemServico(Array $arAtt = null, $orsid = null, Array $arParam = null){
		$orsid = $orsid ? $orsid : $this->orsid;  
//		if ( $orsid ){
//			$this->obModel['OrdemServico'] = new OrdemServico($orsid);
//			$this->carregaDadosModel( $this->obModel['OrdemServico'], $arAtt );
			$this->carregaDadosModel( new OrdemServico( $orsid ), $arAtt );
//		}
	}
	
	function verificaOsGrupoAtivo( $gpdid = null, $orsid = null ){
		$retorno = false;
		if ($gpdid && $orsid){
			$obModel = new OrdemServico();
			$retorno = $obModel->contaOsGrupoAtivo( $gpdid, $orsid );
		}
		
		return ($retorno ? true : false);
	}	
	
	function buscaIdObrasOS(){
		if ( $this->gpdid ){
			$obModel = new ObraInfraestrutura();
			$arObrid = $obModel->listaIdObraPorGrupo( $this->gpdid );
		}
		
		return $arObrid;
	}

	function buscaObraPorID($obrid){
		$obObra = new ObrasController( $obrid );
		return $obObra;			
	}
	
	function buscaTempoTrajetoPorObrid($obrid){
		$obModel = new OrdemServico( $obrid );
		return $obModel->buscaTempoTrajetoPorObrid($obrid);
	}
	
	function buscaValorHoraTecnicaPorUf($estuf){
		$obModel = new OrdemServico( $obrid );
		return $obModel->buscaValorHoraTecnicaPorUf($estuf);			
	}
	
	function buscaValorCombustivelPorUf($estuf){
		$obModel = new OrdemServico( $obrid );
		return $obModel->buscaValorCombustivelPorUf($estuf);			
	}
	
	function buscaTipoDeslocamentoPorObra($obrid){
		$obModel = new TipoDeslocamento( $obrid );
		return $obModel->buscaTipoDeslocamentoPorObra($obrid);			
	}

	function buscaDadosEmpresa(){
		$obModel = new GrupoDistribuicao();
		$param["vinculoGrupo"] = true;
		$param["coluna"] 	   = "oe.epcid AS codigo, entnome AS descricao";
		$dados = $obModel->buscaEmpresa( $param );
		
		return ( is_array( $dados ) ? $dados : array());
	}
	
	function buscaDadosRota($calculado = true){
		$obModel = new Rotas();
		
		$dados = $calculado 
					? $obModel->dadosRotaCalculado(array('gpdid' => $this->gpdid))
					: $obModel->dadosRota(array('gpdid' => $this->gpdid));
		
		return $dados;
	}
	
	function buscaDadosSituacaoOS(){
		$obModel = new OrdemServico();
		$param["coluna"] = "stoid AS codigo, stodsc AS descricao";
		$dados = $obModel->buscaSituacao( $param );
		
		return ( is_array( $dados ) ? $dados : array());
	}
	
	function salvarOS(){
		$post = $_POST;

		$post['orsdtemissao']		 = $post['orsdtemissao'] 	 	? formata_data_sql( $post['orsdtemissao'] ) 	   : null; 
		$post['orsdtinicioexecucao'] = $post['orsdtinicioexecucao'] ? formata_data_sql( $post['orsdtinicioexecucao'] ) : null; 
		$post['orsdtfinalexecucao']  = $post['orsdtfinalexecucao']  ? formata_data_sql( $post['orsdtfinalexecucao'] )  : null; 
		$post['orsidderivada']  	 = $post['orsidderivada']       ? $post['orsidderivada'] 						   : null; 
		
		$obModel = new OrdemServico();
		// caso tenha o id carrega-se o objeto, que será atualizado.
		if ( is_numeric(trim($post['orsid'])) && $post['orsid'] > 0 ){
			$obModel->carregarPorId( $post['orsid'] );
		}
		
		// campos que teram valores no insert|update
		$arCampos = array(
							"gpdid",
							"stoid",
							"orsidderivada",
							"arqid",
							"orsdtemissao",
							"orsdtinicioexecucao",
							"orsdtfinalexecucao",
							"orsnumprotec",
							"orsnotaempeds",
							"orsnotaempedp",
							"orsnotaempedb",
							"orsobs",
							"orsvalor",
							"orsstatus",
							"usucpf"
						 );
						 
		$obModel->popularObjeto($arCampos, $post);		
		$orsid = $obModel->salvar();
		$obModel->commit();
		
		return $orsid;
	}

	function deletaOS( $id = null ){
		$obModel = new OrdemServico( $id );
		// campos que teram valores no insert|update
		$arCampos = array("orsstatus");
		$arDados  = array("orsstatus" => 'I');
						 
		$obModel->popularObjeto($arCampos, $arDados);		
		$orsid = $obModel->salvar();
		$obModel->commit();
	}
	
	function listaOSTodos( Array $param = null ){
		/*
		 * INÍCIO - MODEL
		 * definição de parametros a serem passados para o modelo
		 */				    
		// COLUNAs a serem retornadas da query				    
		$arParam['coluna'] = array(
									"orsid",
									"to_char(orsdtemissao, 'DD/MM/YYYY' ) AS orsdtemissao",
									"gd.gpdid ||' / '|| e.entnome AS empresa",
									"to_char(orsdtinicioexecucao, 'DD/MM/YYYY' ) AS orsdtinicioexecucao",
									"to_char(orsdtfinalexecucao, 'DD/MM/YYYY' ) AS orsdtfinalexecucao",
									"so.stodsc",
									"os.orsvalor AS valor",
									"u.usunome"
								  );
		// FILTRO da query (WHERE)						  
		$arParam['filtro'] = $param['filtro'];			  
		$obModel 		   = new OrdemServico();
		$arDados 		   = $obModel->listaOS( $arParam );
		/*
		 * FIM - MODEL
		 * termina buscando uma lista de dados provindos do modelo de "OrdemServico"
		 */				    
		
		/*
		 * INÍCIO - VIEW
		 * definição de parametros para serem passados para definição da VIEW, no caso, "Lista"
		 */				    
		// CABEÇALHO da lista
		$arCabecalho = array(
								"Nº OS",
								"Data Emissão",
								"Grupo / Empresa",
								"Execução" => array("Início", "Término"),
								"Situação",
								"Valor (R$)",
								"Cadastrante"
						    );
		
		// parametros que cofiguram as colunas da lista, a ordem do array equivale a ordem do cabeçalho 
		$arParamCol[0] = array("type" => "string", 
										 "style" => "color:#0066CC;",
										 "align" => "right");
		$arParamCol[1] = array("type" => "date");
		$arParamCol[3] = array("type" => "date");
		$arParamCol[4] = array("type" => "date");
		
		// ARRAY de parametros de configuração da tabela
		$arConfig = array("style" => "width:100%;");
		
		$a = new Lista($arConfig);
		$a->setCabecalho( $arCabecalho );
		$a->setCorpo( $arDados, $arParamCol );
		$a->setAcao( $acao );
		$a->show();	
		/*
		 * FIM - VIEW
		 * o método show() renderiza os parametros, cuspindo na tela a lista.
		 */	
	}	
	
	public static function listaOS( Array $param = null ){
		
		if(!empty($param) && is_array($param) ){
			extract($param);
		}
		
		/*
		 * INÍCIO - MODEL
		 * definição de parametros a serem passados para o modelo
		 */				    
		// COLUNAs a serem retornadas da query				    
		$arParam['coluna'] = array(
									"'' as oshtml",
									"orsid",
									"to_char(orsdtemissao, 'DD/MM/YYYY' ) AS orsdtemissao",
									"CASE WHEN (SELECT count(orsidderivada) FROM obras.ordemservico WHERE orsid = os.orsid AND orsidderivada IS NOT NULL)   > 0 
										THEN '<img src=\'../imagens/obras/check.png\' onclick=\"location.href=\'?modulo=principal/supervisao/listaOSAssociada&acao=A&orsidderivada=' || orsid || '\'\" style=\'cursor: pointer; width: 15px;\'>'
										ELSE ''
									 END AS possuios",
									"estuf",
									"gd.gpdid ||' / '|| e.entnome AS empresa",
									"to_char(orsdtinicioexecucao, 'DD/MM/YYYY' ) AS orsdtinicioexecucao",
									"to_char(orsdtfinalexecucao, 'DD/MM/YYYY' ) AS orsdtfinalexecucao",
									"COALESCE(to_char(htddata, 'DD/MM/YYYY' ), '-') AS htddata",
									"so.stodsc",
									"os.orsvalor AS valor",
									"u.usunome",
									"gd.gpdid"
								  );
		$param['filtro']		  = is_array( $param['filtro'] ) ? $param['filtro'] : $param;						  
		$param['filtro']['stoid'] = $param['filtro']['stoid'] ? $param['filtro']['stoid'] : OrdemServico::SITUACAOOS_GERADA;						  
		// FILTRO da query (WHERE)						  
		$arParam['filtro'] = $param['filtro'];			  
		$obModel 		   = new OrdemServico();
		//$arDados 		   = $obModel->listaOS( $arParam );
		$sql 		       = $obModel->listaOS( $arParam, 'string' );
		/*
		 * FIM - MODEL
		 * termina buscando uma lista de dados provindos do modelo de "OrdemServico"
		 */				    
		
		/*
		 * INÍCIO - VIEW
		 * definição de parametros para serem passados para definição da VIEW, no caso, "Lista"
		 */				    
		// CABEÇALHO da lista
		$arCabecalho = array(
								"Ação",
								"OS (HTML)",
								"Nº OS",
								"Data Emissão",
								"Possui OS <br>associada",
								"UF",
								"Grupo / Empresa",
								"Execução" => array("Início", "Término"),
								"Data de Finalização da Supervisão",
								"Situação",
								"Valor (R$)",
								"Cadastrante"
						    );
						    
		// AÇÃO que será posta na primeira coluna de todas as linhas
		//(possuiPerfil( array(PERFIL_SAA, PERFIL_ADMINISTRADOR, PERFIL_SUPERUSUARIO) ) ?	"<img src='../imagens/excluir.gif' title='Excluir OS' style='cursor:pointer; margin-left:3px' onclick='excluir({orsid}, this);'>" : "<img src='../imagens/excluir_01.gif' title='Sem permissão' style='cursor:pointer; margin-left:3px'>") .
		$acao = "<center>
				   <img src='../imagens/alterar.gif' title='Editar OS' style='cursor:pointer; margin-left:3px' onclick='location.href=\"?modulo=principal/supervisao/cadOS&acao=A&orsid={orsid}\"'>
				  <center>";
		
		// parametros que cofiguram as colunas da lista, a ordem do array equivale a ordem do cabeçalho 
		$arParamCol[0] = array("type"  => "string", 
							   "align" => "center", 
							   "html"  => "<img src='../imagens/ico_html.gif' width='19' height='19' title='Visualizar OS' style='cursor:pointer; margin-left:3px;' onclick='visualizarOS({orsid});'>");
		$arParamCol[1] = array("type" => "string", 
										 "style" => "color:#0066CC;",
										 "align" => "right",
										 "html" =>"<img border=\"0\" onclick=\"envia_email_empresa({orsid});\" title=\"Enviar OS por e-mail para Empresa.\" src=\"../imagens/email.gif\" style=\"cursor: pointer;\"/>
										 <font>{orsid}</font>");
		$arParamCol[2] = array("type" => "date");
		$arParamCol[3] = array("type" => "string",
							   "align"=> "center");
		$arParamCol[4] = array("type" => "string", 
										 "align" => "center");
		$arParamCol[5] = array("html" => "<img src='../imagens/edit_on.gif' width='12' height='12' title='Ver Detalhes' style='cursor:pointer; margin-left:3px;' onclick=\"window.open('?modulo=principal/supervisao/listaOS&acao=A&operacao=listaGrupoTotal&gpdid={gpdid}','width=850,height=600,resizable=yes', this );\"
										  <font>{empresa}</font>");
		$arParamCol[6] = array("type" => "date");
		$arParamCol[7] = array("type" => "date");
		
		// ARRAY de parametros de configuração da tabela
		$arConfig = array("style" => "width:100%;");
		
		/*$a = new Lista($arConfig);
		$a->setCabecalho( $arCabecalho );
		$a->setCorpo( $arDados, $arParamCol );
		$a->setAcao( $acao );
		$a->show();*/
		
		$oPaginacaoAjax = new PaginacaoAjax();
		$oPaginacaoAjax->setNrPaginaAtual($nrPaginaAtual);
		$oPaginacaoAjax->setNrRegPorPagina($nrRegPorPagina);
		$oPaginacaoAjax->setNrBlocoPaginacaoMaximo($nrBlocoPaginacaoMaximo);
		$oPaginacaoAjax->setNrBlocoAtual($nrBlocoAtual);
		$oPaginacaoAjax->setDiv( 'listaOS' );
		$oPaginacaoAjax->setCabecalho( $arCabecalho );
		$oPaginacaoAjax->setSql( $sql );
		$oPaginacaoAjax->setAcao( $acao );
		$oPaginacaoAjax->setParamCol( $arParamCol );
		$oPaginacaoAjax->setConfig( $arConfig );
		$oPaginacaoAjax->show();
		
		
		/*
		 * FIM - VIEW
		 * o método show() renderiza os parametros, cuspindo na tela a lista.
		 */	
	}
	
	public static function listaOSAssociados( Array $param = null ){
		
		if(!empty($param) && is_array($param) ){
			extract($param);
		}
		
		/*
		 * INÍCIO - MODEL
		 * definição de parametros a serem passados para o modelo
		 */				    
		// COLUNAs a serem retornadas da query				    
		$arParam['coluna'] = array(
									"'' as oshtml",
									"orsid",
									"to_char(orsdtemissao, 'DD/MM/YYYY' ) AS orsdtemissao",
									"to_char(orsdtinicioexecucao, 'DD/MM/YYYY' ) AS orsdtinicioexecucao",
									"to_char(orsdtfinalexecucao, 'DD/MM/YYYY' ) AS orsdtfinalexecucao",
									"COALESCE(to_char(htddata, 'DD/MM/YYYY' ), '-') AS htddata",
									"so.stodsc",
									"os.orsvalor AS valor",
									"u.usunome",
									"gd.gpdid"
								  );
		$param['filtro']		  = is_array( $param['filtro'] ) ? $param['filtro'] : $param;						  
		$param['filtro']['stoid'] = $param['filtro']['stoid'] ? $param['filtro']['stoid'] : OrdemServico::SITUACAOOS_GERADA;						  
		// FILTRO da query (WHERE)						  
		$arParam['filtro'] = $param['filtro'];			  
		$obModel 		   = new OrdemServico();
		//$arDados 		   = $obModel->listaOS( $arParam );
		$sql 		       = $obModel->listaOS( $arParam, 'string' );
		/*
		 * FIM - MODEL
		 * termina buscando uma lista de dados provindos do modelo de "OrdemServico"
		 */				    
		
		/*
		 * INÍCIO - VIEW
		 * definição de parametros para serem passados para definição da VIEW, no caso, "Lista"
		 */				    
		// CABEÇALHO da lista
		$arCabecalho = array(
								"Ação",
								"OS (HTML)",
								"Nº OS",
								"Data Emissão",
								"Execução" => array("Início", "Término"),
								"Data de Finalização da Supervisão",
								"Situação",
								"Valor (R$)",
								"Cadastrante"
						    );
						    
		// AÇÃO que será posta na primeira coluna de todas as linhas
		$acao = "<center>
				   <img src='../imagens/alterar.gif' title='Editar OS' style='cursor:pointer; margin-left:3px' onclick='location.href=\"?modulo=principal/supervisao/cadOS&acao=A&orsid={orsid}\"'>" . 
					(possuiPerfil( array(PERFIL_SAA, PERFIL_ADMINISTRADOR, PERFIL_SUPERUSUARIO) ) ?				   
						"<img src='../imagens/excluir.gif' title='Excluir OS' style='cursor:pointer; margin-left:3px' onclick='excluir({orsid}, this);'>"
																								  :		
						"<img src='../imagens/excluir_01.gif' title='Sem permissão' style='cursor:pointer; margin-left:3px'>") .
				  "<center>";
		
		// parametros que cofiguram as colunas da lista, a ordem do array equivale a ordem do cabeçalho 
		$arParamCol[0] = array("type"  => "string", 
							   "align" => "center", 
							   "html"  => "<img src='../imagens/ico_html.gif' width='19' height='19' title='Visualizar OS' style='cursor:pointer; margin-left:3px;' onclick='visualizarOS({orsid});'>");
		$arParamCol[1] = array("type" => "string", 
										 "style" => "color:#0066CC;",
										 "align" => "right",
										 "html" =>"<img border=\"0\" onclick=\"envia_email_empresa({orsid});\" title=\"Enviar e-mail para Empresa\" src=\"../imagens/email.gif\" style=\"cursor: pointer;\"/>
										 <font>{orsid}</font>");
		$arParamCol[2] = array("type" => "date");
		$arParamCol[3] = array("type" => "string",
							   "align"=> "center");
		$arParamCol[4] = array("type" => "string", 
										 "align" => "center");
		$arParamCol[5] = array("html" => "<img src='../imagens/edit_on.gif' width='12' height='12' title='Ver Detalhes' style='cursor:pointer; margin-left:3px;' onclick=\"window.open('?modulo=principal/supervisao/listaOS&acao=A&operacao=listaGrupoTotal&gpdid={gpdid}','width=850,height=600,resizable=yes', this );\"
										  <font>{empresa}</font>");
		$arParamCol[6] = array("type" => "date");
		$arParamCol[7] = array("type" => "date");
		
		// ARRAY de parametros de configuração da tabela
		$arConfig = array("style" => "width:100%;");
		
		/*$a = new Lista($arConfig);
		$a->setCabecalho( $arCabecalho );
		$a->setCorpo( $arDados, $arParamCol );
		$a->setAcao( $acao );
		$a->show();*/
		
		$oPaginacaoAjax = new PaginacaoAjax();
		$oPaginacaoAjax->setNrPaginaAtual($nrPaginaAtual);
		$oPaginacaoAjax->setNrRegPorPagina($nrRegPorPagina);
		$oPaginacaoAjax->setNrBlocoPaginacaoMaximo($nrBlocoPaginacaoMaximo);
		$oPaginacaoAjax->setNrBlocoAtual($nrBlocoAtual);
		$oPaginacaoAjax->setDiv( 'listaOS' );
		$oPaginacaoAjax->setCabecalho( $arCabecalho );
		$oPaginacaoAjax->setSql( $sql );
		$oPaginacaoAjax->setAcao( $acao );
		$oPaginacaoAjax->setParamCol( $arParamCol );
		$oPaginacaoAjax->setConfig( $arConfig );
		$oPaginacaoAjax->show();
		
		
		/*
		 * FIM - VIEW
		 * o método show() renderiza os parametros, cuspindo na tela a lista.
		 */	
	}
	
	function listaObrasGrupo(){
		/*
		 * INÍCIO - MODEL
		 * definição de parametros a serem passados para o modelo
		 */				    
		// COLUNAs a serem retornadas da query				    
		$arCol = array(
						"oi.obrid",
						"oi.obrdesc",
						"euni.entnome as unidade",
						"ecam.entnome as campus",
						"ed.estuf || ' / ' || m.mundescricao AS uf",
						"oi.obrpercexec",
						"so.stodesc"
					  );
					  
		// JOINs com as tabelas necessárias			  
		$arJoin = array( 
						"inner" => array(
											"obras.itemgrupo it"			=> "it.gpdid = gp.gpdid",
											"obras.repositorio r"			=> "r.repid = it.repid
																				AND r.repstatus = 'A'",
											"obras.obrainfraestrutura oi"	=> "oi.obrid = r.obrid",
											"entidade.entidade euni"		=> "euni.entid = oi.entidunidade",
											"entidade.endereco ed"			=> "ed.endid = oi.endid",
											"territorios.municipio m"		=> "m.muncod = ed.muncod",
											"obras.situacaoobra so "		=> "so.stoid = oi.stoid",
										),
						"left" => array(
											"entidade.entidade ecam"		=> "ecam.entid = oi.entidcampus"
										)
					 );		
					 				
		// Claúsulas de filtro			  
		$arWhere = array("gp.gpdid = {$this->gpdid}");
		// Carrega os dados do modelo						  				  
		$obModel = new GrupoDistribuicao();
		$arDados = $obModel->lista($arCol, $arWhere, $arJoin, array('alias' => 'gp'));
		/*
		 * FIM - MODEL
		 * termina buscando uma lista de dados provindos do modelo de "GrupoDistribuicao"
		 */
			
		/*
		 * INÍCIO - VIEW
		 * definição de parametros para serem passados para definição da VIEW, no caso, "Lista"
		 */				    
		// CABEÇALHO da lista
		$arCabecalho = array(
								"<font style='float:left;'>Dados das Obras</font>" => array(
																							 "Codigo",
																							 "Obra",
																							 "Unidade",
																							 "Campus",
																							 "UF / Município",
																							 "% Exec",
																							 "Situação"
																							)
						    );
		// parametros que cofiguram as colunas da lista, a ordem do array equivale a ordem do cabeçalho 
		$arParamCol = array();
		array_push($arParamCol, array("type"  => Lista::TYPESTRING, 
									  "style" => "color:#0066CC;",
									  "align" => "right"));
		array_push($arParamCol, array("type" => Lista::TYPESTRING));
		array_push($arParamCol, array("type" => Lista::TYPESTRING));
		array_push($arParamCol, array("type" => Lista::TYPESTRING));
		array_push($arParamCol, array("type" => Lista::TYPESTRING));
		array_push($arParamCol, array("type" => Lista::TYPENUMERIC));
		// ARRAY de parametros de configuração da tabela
		$arConfig = array("style" 		  => "width:100%;",
						  "totalRegistro" => true);
		
		$a = new Lista( $arConfig );
		$a->setCabecalho( $arCabecalho );
		$a->setCorpo( $arDados, $arParamCol );
		$a->show();	
		/*
		 * FIM - VIEW
		 * o método show() renderiza os parametros, cuspindo na tela a lista.
		 */			
	}
	
	function addValidacaoMetragem( Array $arr ){
		$obModel = new ObraInfraestrutura();
		$arParam = array("coluna" => array("obrqtdconstruida", 
										   "obrdesc")
						);
		$arDados = $obModel->listaDadosObraPorGrupo( $arr['gpdid'], $arParam );
		
		$arTxtObras = array();
		if ( $arDados ){
			foreach ($arDados as $dados){
				if ( $dados['obrqtdconstruida'] == 0 ){
					$arTxtObras[] = "{$dados['obrdesc']}";	
				}
			}
		}
		$arr['validametragem'] = addslashes(implode("||", $arTxtObras));
		
		return $arr;
	}
	
	function listaSelecaoGrupo(){
		/*
		 * INÍCIO - MODEL
		 * definição de parametros a serem passados para o modelo
		 */
		 $col = array(
        			 "gp.gpdid",
        			 "e.entnome AS empresa",
        			 "gp.estuf AS uf",
        			 "to_char(gpddtcriacao, 'dd-mm-YYYY') AS datacriacao",
		 			 "to_char(gpddtinicio, 'dd-mm-YYYY') AS gpddtinicio",
		 			 "to_char(gpdtermino, 'dd-mm-YYYY') AS gpdtermino"
        			);
        			
		$obModel = new GrupoDistribuicao();	
		$arDados = $obModel->dadosSelecaoGrupoOS($col, array("gpdidEdicao" => $this->gpdid));	
		
		if ( $arDados ){
			$arDados = array_map(array($this, "addValidacaoMetragem"), $arDados);
		}
		/*
		 * FIM - MODEL
		 * termina buscando uma lista de dados provindos do modelo de "GrupoDistribuicao"
		 */
		/*
		 * INÍCIO - VIEW
		 * definição de parametros para serem passados para definição da VIEW, no caso, "Lista"
		 */
		// CABEÇALHO da lista
		$arCabecalho = array("Ação", "Código", "Empresa", "UF", "Data de Criação");
		
		// AÇÃO que será posta na primeira coluna de todas as linhas
		$acao = "<center>
				   <input onclick=\"this.checked = selecionarGrupo('{gpddtinicio}', '{gpdtermino}', '{validametragem}')\" {checked} type=\"radio\" name=\"rdo_grupo\" id=\"rdo_grupo_{gpdid}\" value=\"{gpdid}\" > 
				   <img style=\"cursor:pointer\" onclick=\"exibeDadosEmpresa({gpdid})\" src=\"../imagens/consultar.gif\" />
				 <center>";
		
		// parametros que cofiguram as colunas da lista, a ordem do array equivale a ordem do cabeçalho 
		$arParamCol = array();
		array_push($arParamCol, "");
		array_push($arParamCol, array("html" => "<span id=\"{gpdid}_grp_desc\" >{empresa}</span>"));
		array_push($arParamCol, "");
		array_push($arParamCol, array("type" => Lista::TYPEDATE));

		// ARRAY de parametros de configuração da tabela
		$arConfig = array("style" => "width:100%;");
		
		$a = new Lista( $arConfig );
		$a->setCabecalho( $arCabecalho );
		$a->setCorpo( $arDados, $arParamCol );
		$a->setAcao( $acao );
		$a->show();				
		/*
		 * FIM - VIEW
		 * o método show() renderiza os parametros, cuspindo na tela a lista.
		 */		
	}
	
	function listaRotasGrupo(){
		/*
		 * INÍCIO - MODEL
		 * definição de parametros a serem passados para o modelo
		 */
		//Busca rota aprovada
		$obModel = new Rotas();
		$rota = $obModel->buscaRotaAprovada($this->gpdid, "to_char(rotdtinclusao, 'DD-MM-YYYY') as rota");
		$rota = is_array($rota) ? end($rota) : '-';						    
					 				
		// Claúsulas de filtro			  
		$arWhere = array("gpdid" => $this->gpdid);
		// Carrega os dados do modelo						  				  
		$arDados = $obModel->dadosRota( $arWhere );
		
		if ( is_array($arDados) ){
			do{
				$seq++;
				$key = key($arDados);
				$arDados[$key]['trjseq'] = $seq;
			}while( next($arDados) );	
		}
		
		/*
		 * FIM - MODEL
		 * termina buscando uma lista de dados provindos do modelo de "GrupoDistribuicao"
		 */
		/*
		 * INÍCIO - VIEW
		 * definição de parametros para serem passados para definição da VIEW, no caso, "Lista"
		 */	
		// CABEÇALHO da lista
		$arCabecalho = array( "<font style='float:left;'>Rotas</font>" => array("<font style='float:left;'>Data de Criação: {$rota}</font>" => array("Sequência",
																													"Trajetória",
																													"Município",
																													"Tipo de Deslocamento",
																													"Dados da Trajetória" => array("Detalhamento - Trajeto Alternativo (R$)",
																																				   "Distância (Km)", 
																																				   "Valor do Pedágio (R$)", 
																																				   "Tempo (Hrs)")
																												 	)
																				)						 	
						    );
		// parametros que cofiguram as colunas da lista, a ordem do array equivale a ordem do cabeçalho 
		$arParamCol    = array();
		$arParamCol[0] = array("type"  => Lista::TYPESTRING, 
							   "style" => "color:#0066CC;",
							   "align" => "right");
		$arParamCol[4] = array("type"  => Lista::TYPEMONEY);
		$arParamCol[5] = array("type"  => Lista::TYPENUMERIC);
		$arParamCol[6] = array("type"  => Lista::TYPEMONEY);
		$arParamCol[7] = array("type"  => Lista::TYPETIME);
		// ARRAY de parametros de configuração da tabela
		$arConfig = array("style"	   => "width:100%;",
						  "totalLinha" => true);
		
		$a = new Lista( $arConfig );
		$a->setCabecalho( $arCabecalho );
		$a->setCorpo( $arDados, $arParamCol );
		$a->show();	
		/*
		 * FIM - VIEW
		 * o método show() renderiza os parametros, cuspindo na tela a lista.
		 */			
	}

	function listaProcedimentoValor($obrid){
		$obModel = new GrupoDistribuicao();
		$arDados = $obModel->listaProcedimentoValor( $obrid );
		
		// CABEÇALHO da lista
		$arCabecalho = array("",
							 "Valor p/ Hora<br>(R$)",
							 "Total<br>(R$)");
		// parametros que cofiguram as colunas da lista, a ordem do array equivale a ordem do cabeçalho 
		$arParamCol    = array();
		$arParamCol[0] = array("type"  => Lista::TYPESTRING);
		$arParamCol[1] = array("type"  => Lista::TYPEMONEY,
							   "totalLinha" => false);
		$arParamCol[2] = array("type"  => Lista::TYPEMONEY);
		// ARRAY de parametros de configuração da tabela
		$arConfig = array("style"=> "width:100%", "totalLinha" => true);
		
		$a = new Lista( $arConfig );
		$a->setCabecalho( $arCabecalho );
		$a->setCorpo( $arDados, $arParamCol );
		$a->show();			
	}

	function listaCalculoDeslocamento(){
		$obModel = new DeslocamentoController();
		$obModel->listaRemuneracaoDeslocamento( $this->gpdid );	
	}
	
	function totalRemuneracaoDeslocamento(){
		$obModel = new DeslocamentoController();
		$dado = $obModel->totalRemuneracaoDeslocamento( $this->gpdid );	
		
		return $dado;
	}
	
	function totalKmRota(Array $arParam = array()){
		$obModel = new Rotas();
		$arParam['gpdid'] = $arParam['gpdid'] ? $arParam['gpdid'] : $this->gpdid;
		
		$dado = $obModel->pegaTotalKmRota($arParam);
		
		return $dado;
	}
	
	function totalRota($arParam = null){
		$obModel = new Rotas();

		$arParam['gpdid'] = $arParam['gpdid'] ? $arParam['gpdid'] : $this->gpdid;
		return $obModel->pegaTotalTrajetos($arParam);
	}
	
	function totalTrajetos(){
		$obModel = new DeslocamentoController();
		$dado = $obModel->totalTrajetos( $this->gpdid );	
		
		return $dado;
	}
	
	function totalValorProcedimentoObra( $obrid, Array $param = null ){
		$obModel = new GrupoDistribuicao();
		$dado 	 = $obModel->pegaTotalValorProcedimento( $obrid, $param );
		
		return $dado;
	}

	static function gerarArquivoHTML( $orsid ){
		// Ativa o buffer de saída
		ob_start();
		// Limpa o buffer de saída
		ob_clean();
		
		// O arquivo de OS só pode ser gerado uma vez.
		$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($orsid/1000) .'/';
		$name    = 'OS_'. $orsid . '.html';
		if (file_exists($caminho . $name)) {
			unlink( $caminho . $name );
		}
		
		$obOS = new OSController();
		// ativa no controller o modelo "OrdemServico" e com isso seus atributos
		$obOS->ativaDadosOrdemServico( null, $orsid );
		// ativa no controller o modelo "GrupoDistribuicao" e com isso seus atributos
		$obOS->ativaDadosGrupo();
		// carrega o resultado da pesquisa no controller
		$obOS->carregaDadosEmpresa();
		
		?>
		<html>
			<head>
				<title><?php echo $GLOBALS['parametros_sistema_tela']['nome_e_orgao'];?></title>
				<script language="JavaScript" src="../../includes/funcoes.js"></script>
				<!--<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>-->
				<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
				<script src="../includes/calendario.js"></script>
				<script src="../obras/js/obras.js"></script>
				<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
				<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
				<script type="text/javascript">
				<!--
					// Feito em javascript por causa da presa.
					$(document).ready(function() {

						const TRANS_ROD_TRAJUNICO		= 1;	
						const TRANS_ROD_TRAJROTEIRO		= 2;	
						const TRANS_NROD_TRAJALTERNATIO	= 3;
						
						var valorHoraTecnica = $('#valorHoraTecnica').val();
						var valorHoraTecnicaFormatado = "";
						valorHoraTecnica = valorHoraTecnica / 2;
						valorHoraTecnica = Math.round(parseInt(valorHoraTecnica * 100)) / 100;
						valorHoraTecnicaFormatado = mascaraglobal('###.###.###.###,##',valorHoraTecnica.toFixed(2).replace(".",","))
						var somaValorHoraTecnica = 0;
						
						$('.valorHoraTecnica').each(function(i){
							var tdeid = $('input[name=tipoDeslocamento[]]')[i].value;

							var tempoTrajeto = $('input[name=tempoTrajeto[]]')[i].value;
							var tempoTrajetoEmHora = tempoTrajeto.substr(0,2);

							var obrid = $('input[name=obra[]]')[i].value;
														
							if(tdeid == TRANS_NROD_TRAJALTERNATIO && parseInt(tempoTrajetoEmHora) > 2){
								var trTotal = $(this).find('tr:last');
									trTotal.find('td:first').text('Valor da remuneração:');
	
								var valorRemuneracao = trTotal.find('td:last').text()
									valorRemuneracao = parseFloat(replaceAll(replaceAll(valorRemuneracao,".",""),",","."))
								
								var valorTotal = parseFloat(valorHoraTecnica) + parseFloat(valorRemuneracao);
								valorTotal = Math.round(parseInt(valorTotal * 100)) / 100;
								valorTotal = mascaraglobal('###.###.###.###,##',valorTotal.toFixed(2).replace(".",","))
								
								somaValorHoraTecnica = somaValorHoraTecnica + parseFloat(replaceAll(replaceAll(valorHoraTecnicaFormatado,".",""),",","."));
									
								var html = '<tr bgcolor="#e3e3e3" id="comp_'+obrid+'"><td>Complemento (50% Hora Téc.):</td><td nowrap="nowrap">-</td><td align="right" nowrap="nowrap" style="color: rgb(0, 102, 204);">'+valorHoraTecnicaFormatado+'</td></tr>';
								var htmlValorTotal = '<tr bgcolor="#e3e3e3"><td>Valor Total</td><td nowrap="nowrap">-</td><td align="right" nowrap="nowrap" style="color: rgb(0, 102, 204);">'+valorTotal+'</td></tr>';
	
								trTotal.after(htmlValorTotal).after(html);
							}							

						});

						$('#totalComplemento').text(mascaraglobal('###.###.###.###,##',somaValorHoraTecnica.toFixed(2).replace(".",",")));

						/**
						* refaz a soma com o total de complemento..
						*/
						var remuneracao_1 = parseFloat(replaceAll(replaceAll($('#remuneracao_1').text(),".",""),",","."))
						var remuneracao_2 = parseFloat(replaceAll(replaceAll($('#remuneracao_2').text(),".",""),",","."))
						
						var remuneracao_1 = remuneracao_1 + somaValorHoraTecnica;						
						$('#remuneracao_1').text(mascaraglobal('###.###.###.###,##',remuneracao_1.toFixed(2).replace(".",",")));

						var remuneracao_3 = remuneracao_1 + remuneracao_2;
						$('#remuneracao_3').text(mascaraglobal('###.###.###.###,##',remuneracao_3.toFixed(2).replace(".",",")));


						var totalRemuDesl = $('.remuneracaoDesl').length;
						$('.remuneracaoDesl').each(function(i){
							// maior que 0 por que ele repete uma row
							if(i > 0){
								$(this).find('td:last').text('');

								var trajetoria = $(this).find('td:first').next().text();
								var obridTemp = trajetoria.split("(");
									obridTemp = obridTemp[1].split(")");
									
								var obrid = obridTemp[0];

								var indiceObra = i-1;
								if( obrid == $('input[name=obra[]]')[indiceObra].value){
									if($('#comp_'+obrid).attr('tagName') == 'TR'){
										var complemento = $('#comp_'+obrid).find('td:last').text();
										$(this).find('td:last').text(complemento);	
									}
								}
							}

							// verificamos se é a ultima tr
							if((i+1)==totalRemuDesl){
								// limpa o total da tr Totais
								$(this).next('tr').find('td:last').text('');
							}

						});

					});
				//-->
				</script>
			</head>
			<body>
				<a class="notprint" style="cursor:pointer; float:right; margin-top: 50px; margin-right: 20px;" onclick="window.print();"><img src="../imagens/ico_print.jpg" border="0"></a>
				<center>
					<?php
						$_REQUEST['titulo'] = 'SAA - Subsecretaria de Assuntos Administrativos'; 
						print monta_cabecalho_relatorio( 95 ); 
					?>
					<h4>ORDEM DE SERVIÇO</h4>
				</center>
				<table class="tabela" bgcolor="#ffffff" cellSpacing="1" cellPadding=3 align="center">			
					<tr>	
						<td style='border-collapse: collapse; border: 1px solid #ccc;'>
							<b>OS nº: </b><?=$obOS->orsid; ?>
						</td>
						<td style='border-collapse: collapse; border: 1px solid #ccc;'>
							<b>Data de Emissão: </b><?=formata_data( $obOS->orsdtemissao ); ?>
						</td>
						<td style='border-collapse: collapse; border: 1px solid #ccc;'>
							<b>Processo de Concessão nº: </b><?=$obOS->epcnumproceconc; ?> <br/>
							<b>Contrato nº: </b><?=$obOS->epcnumcontrato; ?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloCentro" colspan="3">Bloco 1 - ESPECIFICAÇÃO DO SERVIÇO</td>
					</tr>
					<tr>
						<td colspan="2" style='border-collapse: collapse; border: 1px solid #ccc;'>
							<b>Empresa:</b> <?=$obOS->empresa; ?> <br/>
							<b>CNPJ:</b> <?=$obOS->cnpj; ?>
						</td>
						<td colspan="2" style='border-collapse: collapse; border: 1px solid #ccc;'>
							<b>Grupo:</b> <?php echo $obOS->gpdid.' - '.$obOS->estuf; ?> <br/>
						</td>
					</tr>
					<tr>
						<td colspan="2" style='border-collapse: collapse; border: 1px solid #ccc;'>
							<b>1.1 - Período de Execução da OS:</b> <?=formata_data($obOS->orsdtinicioexecucao) . ' à ' . formata_data($obOS->orsdtfinalexecucao); ?><br/>
						</td>
						<td colspan="2" style='border-collapse: collapse; border: 1px solid #ccc;'>
							<b>1.2 - Tipo(s) de Deslocamento(s):</b>
							<?=implode(', ', $obOS->buscaTiposDeslocamento()); ?>
						</td>
					</tr>
					<tr>
						<td colspan="4" style='border-collapse: collapse; border: 1px solid #ccc;'>
						<b>1.3 - Local de Execução do serviço:</b><br/>
						<?
						$obOS->listaRotasGrupo();
						?>
						</td>
					</tr>
					<tr>
						<td colspan="3" style='border-collapse: collapse; border: 1px solid #ccc;'>
							<? 
							$arObrid = $obOS->buscaIdObrasOS();
							$totalProcedimento = $obOS->totalValorProcedimentoObra( $arObrid );
							?>
							<b>1.4 - Tabela de Hora Técnica e Deslocamento:</b> <br/>
							<b>&nbsp;&nbsp;&nbsp;1.4.1 - Total de Hora Técnica do Serviço: (R$ <?=number_format($totalProcedimento, 2, ',', '.'); ?>)</b> <br/>
							<table class="tabela" bgcolor="#ffffff" cellSpacing="1" cellPadding="2" align="center">
								<tr>
									<td>
										<? 
										$arTrajPorMunicipio = array();
										$count = 1;
										if ( is_array($arObrid) && $arObrid[0] ):
											foreach( $arObrid as $obrid ):
												$obObra = $obOS->buscaObraPorID( $obrid );
												$trjtempo = $obOS->buscaTempoTrajetoPorObrid( $obrid );
												$tdeid = $obOS->buscaTipoDeslocamentoPorObra( $obrid );
												
												// Carrega Array para ser listado no item 1.6
												array_push($arTrajPorMunicipio, array(
																					"obra" 		 => '(' . $obrid . ') ' . $obObra->obrdesc,
																					"municipio"  => $obObra->municipio,
																					"vlrtrajeto" => 0,
																					"vlrservico" => round($obOS->totalValorProcedimentoObra($obrid), 2),
																					"total" 	 => 0,
																					"tempo" 	 => $trjtempo,
																					"tdeid" 	 => $tdeid
																				 ));
																				 
												echo "<input type=\"hidden\" value=\"$trjtempo\" name=\"tempoTrajeto[]\" />";
												
												// Sempre vai ser uma ordem para o mesmo estado então buscamos só uma vez
												// Feito Assim por causa da pressa, e colocando no jquery
												if($count == 1){
													$valorHoraTecnica = $obOS->buscaValorHoraTecnicaPorUf($obObra->uf);
													$valorCombustivel = $obOS->buscaValorCombustivelPorUf($obObra->uf);
													echo "<input type=\"hidden\" value=\"$valorHoraTecnica\" id=\"valorHoraTecnica\" />";
												}
												echo "<input type=\"hidden\" value=\"$obrid\" name=\"obra[]\" />";
												echo "<input type=\"hidden\" value=\"$tdeid\" name=\"tipoDeslocamento[]\" />";
										?>
										<table class="tabela valorHoraTecnica" style="width: 100%;" bgcolor="#ffffff" cellSpacing="1" cellPadding="2" align="center">
											<tr bgcolor="#F0F0F0">
												<td colspan="4"><b>Nome da Obra:</b><br>(<?=$obrid?>) <?=$obObra->obrdesc; ?></td>
											</tr>
											<tr bgcolor="#F0F0F0">
												<td><b>Unidade:</b><br><?=$obObra->unidade; ?></td>
												<td><b>Campus:</b><br><?=$obObra->campus; ?></td>
												<td><b>Município:</b><br><?=$obObra->municipio; ?></td>
												<td><b>UF:</b><br><?=$obObra->uf; ?></td>
											</tr>
											<tr bgcolor="#F0F0F0">
												<td><b>Tipo de Ensino:</b><br><?=$obObra->tipoensino; ?></td>
												<td><b>Área:</b><br><?=$obObra->area; ?></td>
												<td colspan="2"><b>Situação:</b><br><?=$obObra->situacao; ?></td>
											</tr>
											<tr>
												<td colspan="4">&nbsp;&nbsp;&nbsp;Procedimento/Valor:</td>
											</tr>
											<tr>
												<td colspan="4"><?=$obOS->listaProcedimentoValor( $obrid ); ?></td>
											</tr>
										</table>
										<br>
										<? 
											$count++;
											endforeach;
											
											$totalKmRota 		   = $obOS->totalKmRota(array('tdeid' => 2));
											
											if(!$totalKmRota){
												$totalKmRota = $obOS->totalKmRota(array('tdeid' => 1));	
											}
											
											$arTotalRotaSemPedagio = $obOS->totalRota(array('semPedagio' => true));
											
											$vlrRotaPorKm		   = (($totalKmRota)?$arTotalRotaSemPedagio['trajetoRoteiro']['valor'] / $totalKmRota:0);
											$arDadosTrajeto 	   = $obOS->buscaDadosRota();

											if($arDadosTrajeto)	array_shift($arDadosTrajeto);
											for($a=0; $a < count($arDadosTrajeto); $a++){
												$dadosTrajeto = $arDadosTrajeto[$a];
												for($i=0; $i < count($arTrajPorMunicipio); $i++){
													$tempoHora = intval(substr($arTrajPorMunicipio[$i]['tempo'], 0, 2));
													
													$obraUM   = substr($dadosTrajeto['obrdesc'], 1, (strpos($dadosTrajeto['obrdesc'], ')') -1));
													$obraDOIS = substr($arTrajPorMunicipio[$i]['obra'], 1, (strpos($arTrajPorMunicipio[$i]['obra'], ')') -1));

													if ($obraUM == $obraDOIS){
														$calculo = is_numeric($dadosTrajeto['calculo']) ? $dadosTrajeto['calculo'] : ($dadosTrajeto['calculo']['valDesloc'] * $vlrRotaPorKm);
														$arTrajPorMunicipio[$i]['vlrtrajeto'] = $calculo + ($dadosTrajeto['calculo']['pedagio'] ? $dadosTrajeto['calculo']['pedagio'] : 0);
														if ($arTrajPorMunicipio[$i]['tdeid'] == 3 && $tempoHora > 2){
															$arTrajPorMunicipio[$i]['vlrservico'] += intval(($valorHoraTecnica / 2) * 100) / 100; 
														}											
														$arTrajPorMunicipio[$i]['total']	  = $arTrajPorMunicipio[$i]['vlrtrajeto'] + $arTrajPorMunicipio[$i]['vlrservico'];
														continue 2;
													}elseif ( ($a == (count($arDadosTrajeto) -1) && $i == (count($arTrajPorMunicipio) -1)) ){
														$calculo = is_numeric($dadosTrajeto['calculo']) ? $dadosTrajeto['calculo'] : ($dadosTrajeto['calculo']['valDesloc'] * $vlrRotaPorKm);
														$arTrajPorMunicipio[$i]['vlrtrajeto'] += $calculo + ($dadosTrajeto['calculo']['pedagio'] ? $dadosTrajeto['calculo']['pedagio'] : 0);
														if ($arTrajPorMunicipio[$i]['tdeid'] == 3 && $tempoHora > 2){
															$arTrajPorMunicipio[$i]['vlrservico'] += $valorHoraTecnica / 2; 
														}											
														$arTrajPorMunicipio[$i]['total']	   = $arTrajPorMunicipio[$i]['vlrtrajeto'] + $arTrajPorMunicipio[$i]['vlrservico'];	 														
														continue 2;
													}	
												}
												next($arDadosTrajeto);
											}
											
										endif;
										$totalSup = $obOS->totalValorProcedimentoObra( $arObrid, array("orgid" => 1) );
										$totalPro = $obOS->totalValorProcedimentoObra( $arObrid, array("orgid" => 2) );
										$totalBas = $obOS->totalValorProcedimentoObra( $arObrid, array("orgid" => 3) );
										?>
										<table class="tabela" style="width: 100%;" bgcolor="#ffffff" cellSpacing="1" cellPadding="2" align="center">
											<tr bgcolor="#F0F0F0">
												<td>
												<b>Totais:</b><br>
												<b style="margin-left: 15px;">Educação Superior</b> - R$ <?=number_format($totalSup, 2, ',', '.'); ?><br>
												<b style="margin-left: 15px;">Educação Profissional</b> - R$ <?=number_format($totalPro, 2, ',', '.'); ?><br>
												<b style="margin-left: 15px;">Educação Básica</b> - R$ <?=number_format($totalBas, 2, ',', '.'); ?><br>
												</td>
											</tr>
										</table>
									</td>
								</tr>
							</table>
							<br>
							<b>&nbsp;&nbsp;&nbsp;1.4.2 - Remuneração do Deslocamento:</b> <br/>
							<table class="tabela" id="remuneracaoDesl" bgcolor="#ffffff" cellSpacing="1" cellPadding="2" align="center">
								<tr>
									<td>
										<?php 
											//echo $obOS->listaCalculoDeslocamento();
											$arTotalTrajetos = $obOS->totalTrajetos();
										?>
										Trajeto por Roteiro: <?php echo number_format($arTotalTrajetos['trajetoRoteiro']['valor'], 2, ',', '.'); ?><br />
										Trajeto Alternativo: <?php echo number_format($arTotalTrajetos['trajetoAlternativo'], 2, ',', '.'); ?><br />
										Total Complemento (50% Hora Téc.): <span id="totalComplemento"></span><br />
										Trajeto Único: <?php echo number_format($arTotalTrajetos['trajetoUnico'], 2, ',', '.'); ?><br />
										&nbsp;&nbsp;** Valor da hora técnica: <?php echo number_format($valorHoraTecnica, 2, ',', '.'); ?> - Valor do combustível: <?php echo number_format($valorCombustivel, 2, ',', '.'); ?>
									</td>
								</tr>
							</table>
						</td>
					</tr>
					<tr>
						<td class="SubTituloCentro" colspan="3">Bloco 2 - REMUNERAÇÃO DO SERVIÇOS</td>
					</tr>
					<? 
						//$totalDeslocamento = $obOS->totalRemuneracaoDeslocamento();
					?>
					<tr>
						<td colspan="3" style='border-collapse: collapse; border: 1px solid #ccc;'>
							<b>1.5 - Tabela de Remuneração:</b> <br/>
							<b>&nbsp;&nbsp;&nbsp;1.5.1 - Valor da Remuneração por Hora Técnica (VRht) - </b>R$ <span id="remuneracao_1"><?=number_format($totalProcedimento, 2, ',', '.'); ?></span> <br/>
							<b>&nbsp;&nbsp;&nbsp;1.5.2 - Valor da Remuneração de Deslocamento (VD) - </b>R$ <span id="remuneracao_2"><?=number_format($arTotalTrajetos['valorTotal'], 2, ',', '.'); ?></span> <br/>
							<b>&nbsp;&nbsp;&nbsp;1.5.3 - Valor Total Estimado da OS (VRht + VD) - </b>R$ <span id="remuneracao_3"><?=number_format( ($totalProcedimento + $arTotalTrajetos['valorTotal']), 2, ',', '.'); ?></span> <br/>
							<b>1.6 - Total de Serviços p/ Município</b> <br/>
							<center>
							<div style="border-collapse: collapse; border: 1px solid #ccc; width: 95%;">
								<?php 
								// CABEÇALHO da lista
								$arCabecalho = array("Obra",
													 "Município",
													 "Trajetória<br>(R$)",
													 "Serviços<br>(R$)",
													 "Total<br>(R$)");
								// parametros que cofiguram as colunas da lista, a ordem do array equivale a ordem do cabeçalho 
//								$arParamCol[3]["type"] = Lista::TYPEMONEY;
//								$arParamCol[3]["html"] = '{vlrtrajeto}';
//								$arParamCol[3]["totalLinha"] = true;
//								$arParamCol[4]["type"] = Lista::TYPEMONEY;
//								$arParamCol[4]["html"] = '{total}';
//								$arParamCol[4]["totalLinha"] = true;
								// ARRAY de parametros de configuração da tabela
								$arConfig = array("style"=> "width:100%", "totalLinha" => true);
								
								$a = new Lista($arConfig);
								$a->setCabecalho($arCabecalho);
								$a->setCorpo($arTrajPorMunicipio);
								$a->show();	
								?>
							</div>
							</center>
						</td>
					</tr>			
					<tr>
						<td class="SubTituloCentro" colspan="3">Bloco 3 - UNIDADE RESPONSÁVEL PELA AUTORIZAÇÃO DA OS</td>
					</tr>
					<tr>
						<td colspan="3" style='border-collapse: collapse; border: 1px solid #ccc;'>
						Tendo em vista as informações abaixo, solicito autorização final para execução da presente <b>OS</b>:<br>
						a)	O planejamento Global do Ministério da Educação dos Serviços objeto desta OS, foi analisado e aprovado 
						pelo Núcleo Gestor de Monitoramento e Supervisão de Obras.<br>
						b)	Os procedimentos técnicos a serem realizados foram determinados pela(s) Unidade(s) Demandante(s) e consta
						dos seus registros e arquivos de controle.<br>
						c)	Consta disponibilidade orçamentária na unidade gestora da SAA, conforme notas de empenho abaixo:<br>
						<b style="margin-left: 15px;">Educação Superior nº: </b><?=$obOS->orsnotaempeds ?><br>
						<b style="margin-left: 15px;">Educação Profissional nº: </b><?=$obOS->orsnotaempedp ?><br>
						<b style="margin-left: 15px;">Educação Básica nº:</b> <?=$obOS->orsnotaempedb ?>
						</td>
					</tr>
					<tr>
						<td colspan="3">
						<hr>
						Brasília, DF <?=date('d') . " de " . mes_extenso(date('m')) . " de " . date("Y") ?>.<br>
						<br>
						<br>
						<div align="right" style="float:right;">
							<hr style="width: 220px">
							<center>
								<b>Silvio Luis S. da Silva</b><br>
								Coordenador de Documentação e <br>
								Gestão de Processos - Fiscal do contrato.
							</center>
						</div>	
						</td>
					</tr>
					
				</table>
			</body>
		</html>
		<? 
		$output = ob_get_contents();
		ob_clean();
		
		
		if(!is_dir('../../arquivos')) {
			mkdir(APPRAIZ.'/arquivos', 0777);
		}
		if(!is_dir('../../arquivos/' . $_SESSION['sisdiretorio'] . '')) {
			mkdir(APPRAIZ.'/arquivos/' . $_SESSION['sisdiretorio'] . '', 0777);
		}
		if(!is_dir('../../arquivos/' . $_SESSION['sisdiretorio'] . '/'.floor($orsid/1000))) {
			mkdir(APPRAIZ.'/arquivos/' . $_SESSION['sisdiretorio'] . '/'.floor($orsid/1000), 0777);
		}
		
		$arqGerado = file_put_contents($caminho . $name, $output);
		if ( $arqGerado ){
			$obModel = new Arquivo();
			// campos que teram valores no insert|update
			$arCampos = array(
							  	'arqnome', 
							  	'arqdescricao', 
							  	'arqextensao', 
							  	'arqtipo', 
							  	'arqtamanho', 
							  	'arqdata', 
							  	'arqhora', 
							  	'arqstatus', 
							  	'usucpf', 
							  	'sisid', 
							 );
					
			$arDados = array(
								'arqnome' 		=> current( explode(".",$name) ), 
							  	'arqdescricao'  => "Arquivo de ordem de serviço(OS) gerada (OBRAS)", 
							  	'arqextensao' 	=> "html", 
							  	'arqtipo'	  	=> "text/html", 
							  	'arqtamanho'  	=> filesize( $caminho . $name ),		
							);		
									 
			$obModel->popularObjeto($arCampos, $arDados);		
			$arqid = $obModel->salvar();
			$obModel->commit();
		}
		
		return $arqid;
	}
}
?>
