<?php
class DeslocamentoController extends Controller{	
//	private $obModel;
	
	function listaRemuneracaoDeslocamento( $gpdid ){
		$obModel = new Rotas();
		$arParam['gpdid']  	  = $gpdid;
		$arDado = $obModel->dadosRotaCalculado( $arParam );
		
		/*
		 * INÍCIO - VIEW
		 * definição de parametros para serem passados para definição da VIEW, no caso, "Lista"
		 */	
		// CABEÇALHO da lista
		$arCabecalho = array( "Sequência", "Trajetória", "Município", "Tipo Deslocamento", "Detalhamento do Deslocamento Alternativo", "Valor Trajetória", "Complemento");
		//$arCabecalho = array( "Sequência", "Trajetória", "Município", "Tipo Deslocamento", "Detalhamento do Deslocamento Alternativo");
		// parametros que cofiguram as colunas da lista, a ordem do array equivale a ordem do cabeçalho 
		$arParamCol[0] = array("type"  => Lista::TYPESTRING, 
							   "style" => "color:#0066CC;",
							   "align" => "right");
		$arParamCol[5] = array("type"  => Lista::TYPEMONEY);
		// ARRAY de parametros de configuração da tabela
		$arConfig = array("style"	   => "width:100%;",
						  "totalLinha" => true);
		$a = new Lista( $arConfig );
		$a->setClassTr('remuneracaoDesl');
		$a->setCabecalho( $arCabecalho );
		$a->setCorpo( $arDado, $arParamCol );
		$a->show();	
		/*
		 * FIM - VIEW
		 * o método show() renderiza os parametros, cuspindo na tela a lista.
		 */			
	}

	function totalRemuneracaoDeslocamento( $gpdid ){
		$obModel 		  = new Rotas();
		$arParam['gpdid'] = $gpdid;
		$dado 		      = $obModel->pegaTotalRemuneracaoRota( $arParam );

		return $dado;
	}
	
	function totalTrajetos( $gpdid ){
		$obModel 		  = new Rotas();
		$arParam['gpdid'] = $gpdid;
		$dado 		      = $obModel->pegaTotalTrajetos( $arParam );

		return $dado;
	}
}
?>