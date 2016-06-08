<?php
class ObrasController extends Controller{
		
	function __construct($obrid){
		if ( !empty( $obrid ) ){
			self::carregaDadosObras($obrid);
		}	
	}
	
	function carregaDadosObras($obrid = null, Array $arAtt = null, Array $arParam = null){
		$obrid = $obrid ? $obrid : $this->obrid;  
		$obModel = new ObraInfraestrutura( $obrid );
		$this->carregaDadosModel( $obModel, $arAtt );
	}
	
	function getTipoEnsino(){
		if ( !$this->dado['orgdesc'] && $this->orgid ){
			$obModel = new Orgao( $this->orgid );
			$this->carregaDadosModel( $obModel, array("orgdesc") );
		}
		return $this->orgdesc;
	}
	
	function getUnidade(){
		if ( !$this->dado['unidade'] && $this->entidunidade ){
			$obModel = new Entidade();
			$arAtt 	 = array("unidade" => $obModel->buscaNomeEntidade( $this->entidunidade ));	
			$this->carregaDados($arAtt);
		}
		return $this->dado['unidade'];
	}
	
	function getCampus(){
		if ( !$this->dado['campus'] && $this->entidcampus ){
			$obModel = new Entidade();
			$arAtt   = array("campus" => $obModel->buscaNomeEntidade( $this->entidcampus ));	
			$this->carregaDados($arAtt);
		}
		return $this->dado['campus'];
	}
	
	function getMunicipio(){
		if ( !$this->dado['municipio'] && $this->endid ){
			$obModel = new Municipio();
			$arAtt   = array("municipio" => $obModel->buscaMunicipioPorObra( $this->endid ));	
			$this->carregaDados($arAtt);
		}
		return $this->dado['municipio'];
	}
	
	function getUf(){
		if ( !$this->dado['uf'] && $this->endid ){
			$obModel = new Estado();
			$arAtt   = array("uf" => $obModel->buscaUFPorObra( $this->endid ));	
			$this->carregaDados($arAtt);
		}
		return $this->dado['uf'];
	}
	
	function getSituacao(){
		if ( !$this->dado['situacao'] && $this->stoid ){
			$obModel = new ObraInfraestrutura();
			$arAtt   = array("situacao" => $obModel->pegaSituacaoObra( $this->stoid ));	
			$this->carregaDados($arAtt);
		}
		return $this->dado['situacao'];
	}
	
	function getArea(){
		if ( !$this->dado['area'] && $this->obrqtdconstruida && $this->umdidobraconstruida ){
			$obModel = new ObraInfraestrutura();
			$arAtt   = array("area" => number_format($this->obrqtdconstruida, 2, ',', '.') . ' ' .  $obModel->pegaUnidadeMedida( $this->umdidobraconstruida ));	
			$this->carregaDados($arAtt);
		}
		return $this->dado['area'];
	}
}	
?>