<?php

class Declaracao extends Modelo
{
	const SITUACAO_DECLARACAO_GERADA    = 1;
	const SITUACAO_DECLARACAO_CANCELADA = 2;
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras.declaracao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "dclid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos = array(
							  	'dclid' 		=> null, 
							  	'usucpf' 		=> null, 
							  	'arqid' 		=> null, 
							  	'gpdid' 		=> null, 
							  	'stdid' 		=> null, 
							  	'dclvalor' 		=> null, 
							  	'dclstatus' 	=> null, 
							  	'dcldtemissao' 	=> null,
    							'orsid' 		=> null,
    							'dclordembanc'	=> null
							  );
							  
	function lista( Array $arParam = null, $retorno = 'array' )
	{
		/*
		 * COLUNAS - IN�CIO
		 */
		$coluna = ($arParam['coluna'] ? implode(",", (array) $arParam['coluna']) : "*");
		/*
		 * COLUNAS - FIM
		 */
		/*
		 * Cl�usulas de parametro - IN�CIO
		 */
		$whereP = $arParam['filtro'];
		// Filtra padr�o de status = ativo
		$arWhere[] = "gpd.gpdstatus = 'A'";
		// Filtra pelo ID do GRUPODISTRIBUICAO.
		if ( is_numeric($whereP['gpdid']) ){
			$arWhere[] = "gpd.gpdid = {$whereP['gpdid']}";
		}
		// Filtra pelo ID da EMPRESACONTRATADA.
		if ( is_numeric($whereP['epcid']) ){
			$arWhere[] = "epc.epcid = {$whereP['epcid']}";
		}
		// Filtra a partir da data da EMISS�O DA DISTRIBUI��O.
		if ( $whereP['dcldtemissaoini'] ){
			$arWhere[] = "dcl.dcldtemissao >= '" . formata_data_sql( $whereP['dcldtemissaoini'] ) . "'";
		}
		// Filtra a at� a data da EMISS�O DA DISTRIBUI��O.
		if ( $whereP['dcldtemissaofim'] ){
			$arWhere[] = "dcl.dcldtemissao <= '" . formata_data_sql( $whereP['dcldtemissaofim'] ) . "'";
		}
		// Filtra a UF.
		if ( $whereP['estuf'] ){
			$arWhere[] = "gpd.estuf = '" . ( $whereP['estuf'] ) . "'";
		}
		// Filtra por grupos que possuem declara��o, n�o possuem, e todos.
		if( $whereP['possui_declaracao'] == 'S' )
		{
			$joinDeclaracao = "INNER";
		}
		elseif( $whereP['possui_declaracao'] == 'N' )
		{
			$joinDeclaracao	= "LEFT";
			$arWhere[] 		= "gpd.gpdid not in (SELECT gpdid FROM obras.declaracao WHERE dcl.dclstatus = 'A' AND dcl.stdid = ".Declaracao::SITUACAO_DECLARACAO_GERADA.")";
		}
		else
		{
			$joinDeclaracao = "LEFT";
		}
		
		// Monta string WHERE
		$where = (count( $arWhere ) > 0 ? "WHERE (" . implode(') AND (', $arWhere) . ")" : "");
		/*
		 * Cl�usulas de parametro - FIM
		 */
		
		$sql = "SELECT DISTINCT
					{$coluna}
				FROM
					obras.grupodistribuicao gpd
				{$joinDeclaracao} JOIN
					obras.declaracao dcl ON dcl.gpdid = gpd.gpdid
										AND dcl.dclstatus = 'A'
										--AND dcl.stdid = ".Declaracao::SITUACAO_DECLARACAO_GERADA."
										AND dcl.dclid = (SELECT max(dcl2.dclid) FROM obras.declaracao dcl2 WHERE dcl2.gpdid = gpd.gpdid AND dcl.dclstatus = 'A')
				INNER JOIN
					obras.ordemservico ors ON ors.gpdid = gpd.gpdid
										  AND ors.orsstatus = 'A'
										  AND ors.stoid = ".OrdemServico::SITUACAOOS_GERADA."
				INNER JOIN
					workflow.documento doc ON doc.docid = gpd.docid
										  AND doc.esdid = ".ESDID_SUPERVISAO_FINALIZADA."
				INNER JOIN
					obras.empresacontratada epc ON epc.epcid = gpd.epcid
				INNER JOIN
					entidade.entidade ent ON ent.entid = epc.entid
				LEFT JOIN
					seguranca.usuario usu ON usu.usucpf = dcl.usucpf
				LEFT JOIN
					obras.situacaodeclaracao std ON std.stdid = dcl.stdid
				{$where}
				ORDER BY 
					gpd.gpdid ASC";
				
		if($retorno == 'array'){
			return $this->carregar( $sql );
		} elseif ($retorno == 'string') {
			return $sql;		
		}
	}
	
	function antesSalvar()
	{
		$this->usucpf 	 	= $_SESSION['usucpf'];
		$this->stdid     	= self::SITUACAO_DECLARACAO_GERADA;
		$this->dclstatus 	= 'A';
		
		// Calcula Valor total da OS
		// Busca obras vinculadas ao grupo escolhido
		//$obModel = new ObraInfraestrutura();
		//$arObrid = $obModel->listaIdObraPorGrupo( $this->gpdid );
		// calcula total dos procedimentos
		//$obModel 		   = new GrupoDistribuicao();
		//$totalProcedimento = $obModel->pegaTotalValorProcedimento( $arObrid );
		// calcula total do Deslocamento (Rota)
		//$obModel 		   = new DeslocamentoController();
		//$totalDeslocamento = $obModel->totalTrajetos( $this->gpdid );
		# $totalDeslocamento = $obModel->totalRemuneracaoDeslocamento( $this->gpdid );
		
		//$this->dclvalor = ($totalProcedimento + $totalDeslocamento['valorTotal']);
		
		return true;
	}
	
	function depoisSalvar()
	{
		// Caso n�o tenha o atributo
		if( !$this->arqid )
		{
			// � necess�rio comitar antes de gerar o arquivo HTML, pois quando se faz uma nova inst�ncia da classe modelo
			// perde-se a transa��o aberta. 
			$this->commit();
			// Gera o arquivo da Declara��o
			$arqid = DeclaracaoController::gerarArquivoHTML( $this->dclid );
			if( $arqid )
			{
				$this->arqid = $arqid;
				// Atualiza o atributo "arqid"
				parent::salvar(false, false);
			}
		}
		
		return true;
	}
}

?>