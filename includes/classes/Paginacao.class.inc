<?php
class Paginacao
{
//	private $sql;
	private $db;
	private $numRegistro;
	private $param;
	public static $identificador;
	
	private function __construct($db, $numRegistro){
		self::$identificador++;
		self::preparaParam();
		$this->db 		   = $db;
		$this->numRegistro = $numRegistro;
	}
	
//	public function setTotalRegistro($totalRegistro)
//	{
//		$this->totalRegistro = $totalRegistro;
//	}
	public static function getPaginacao(cls_banco $db, $sql, $numRegistro=30){
		$pag = new Paginacao($db, $numRegistro);
		return $pag->paginar($sql);
	}
	
	private function paginar($sql)
	{
		// Pega TOTAL de registros do banco
		$sqlCount 	   = self::preparaQueryCount($sql);
		$totalRegistro = $this->db->pegaUm($sqlCount);
		// Pega offset da paginação (de onde deve começar a lista)
		$offset 	   = self::offsetPaginacao($totalRegistro);		
		$numRegistro   = $this->numRegistro; 
		if ($this->param['ordem']){
			$sql .= " ORDER BY " . $this->param['ordem'];
		}
		$sql 		   .= " LIMIT {$numRegistro} OFFSET {$offset}";
		$dados 		   = $this->db->carregar($sql);
		return array("total"  		 => $totalRegistro,
					 "totalPagina" 	 => ceil($totalRegistro / $this->numRegistro),
					 "ordem" 	   	 => explode(" ", $this->param['ordem']),
					 "pagina" 	   	 => $this->param['pag'],
					 "identificador" => self::$identificador,
					 "dados"  		 => ($dados ? $dados : array()));
	}
	
	private function preparaParam()
	{
		$this->param['ordem'] = $_POST['ordem'][self::$identificador] ? $_POST['ordem'][self::$identificador] : ''; 
		$this->param['pag']   = $_POST['pag'][self::$identificador] > 0 ? $_POST['pag'][self::$identificador] : 1; 
	}
	
	private function preparaQueryCount($sql){
		$sql = "SELECT 
					COUNT(f.*)
				FROM ({$sql}) AS f;";
//		list($ini, $fim) = self::marcadorQuery($sql);
//		$sql			 = substr_replace($sql, " count(*) ", $ini, $fim);
		return $sql;
	}
	
//	private function marcadorQuery($sql)
//	{
////		$sql = str_ireplace(array("select", "from"), array("SELECT", "FROM"), $sql);
////		
////		$marcador[0] = (strpos($sql, "SELECT") + 6);
////		$marcador[1] = (strpos($sql, "FROM") - 6);
//		$marcador[0] = (stripos($sql, "SELECT") + 6);
//		$marcador[1] = (stripos($sql, "FROM") - 6);
//		
//		return $marcador;
//	}
	
	private function offsetPaginacao($totalRegistro)
	{
		$offSet = 0;
		$pag = ($this->param['pag'] - 1);
		if ($pag >= 0){
			$pages = ceil($totalRegistro / $this->numRegistro);
			$pag   = $pag > $pages ? $pages : $pag; 
			$offSet = $pag * $this->numRegistro; 
		}		
		return $offSet;
//		$limit = ($totalRegistro - $this->numRegistro);
//		$limit = $limit > 0 ? $limit : 0;
//		return rand(0, $limit);
	}
}
?>