<?php
class Usuario extends AbstractEntity
{
		/**
		 * Nome da tabela do banco onde ficam armazenadas as tarefas
		 *
		 */
		public $TABLE_NAME = 'seguranca.usuario';
		
		/**
		 * Identificador da Tabela
		 */
		public $ROW_NAME_ID 					= 'usucpf';
		
		/**
		 * Identificador da Tabela
		 */
		public $ROW_NAME_STR_NAME				= 'usunome';
		
		/**
		 * Identificador da Tabela
		 */
		public $ROW_NAME_STR_EMAIL				= 'usuemail';
		
		/**
		 * Coluna booleana que informa se a tarefa esta em uso
		 * ou nao. 
		 *
		 */
		public $ROW_NAME_BOOL_ACTIVE_STATUS		= 'usustatus';

		/**
		 * Status ativo
		 *
		 * @var string
		 */
		protected $TYPE_STATUS_ACTIVE 	= 'A';
		
		/**
		 * Status Ativo
		 * 
		 * @var string
		 */
		protected $TYPE_STATUS_INACTIVE 	= 'I';
		
		
		protected $strName;

		protected $strEmail;
		
		public function setNome( $strName )
		{
			$this->strName = $strName;
		}
		
		public function getNome()
		{
			return $this->strName;
		}
		
		public function setEmail( $strEmail )
		{
			$this->strEmail = $strEmail;
		}
		
		public function getEmail()
		{
			return $this->strEmail;
		}
		
		public function insert()
		{
			throw new Exception( 'Metodo nao implementado' );
		}
		
		/**
		 * Altera o valor do identificador do objeto.
		 *
		 * @param integer $intId
		 * @return void
		 */
		public function setId( $intId )
		{
			if( $this->intId != $intId )
			{
				$this->removeInstance();
				$this->intId = $intId;
				$this->addInstance();
			}
		}

		/**
		 * Altera o valor do campo de controle de concorrencia
		 *
		 * @param integer $intId
		 * @return void
		 * @access public 
		 */
		public function setSid( $intSid )
		{

			$this->intSid = $intSid;
		}
		
		public function emerge()
		{
			$strSql =	' SELECT ' .
							'*' . 
						' FROM ' .
							$this->TABLE_NAME .
						' WHERE ' .
							$this->ROW_NAME_ID . ' = ' . escape( $this->getId() ) .
						' AND ' .
							$this->ROW_NAME_BOOL_ACTIVE_STATUS . ' = ' . escape( $this->TYPE_STATUS_ACTIVE ) .
						' LIMIT 1 ' .
						'';
			$objSaida = self::$objDatabase->carregar( $strSql );
			
			if( is_array( $objSaida ) && sizeof( $objSaida ) == 1 )
			{
				$objSaida = $objSaida[ 0 ];

				$this->setId(						unescape( @$objSaida[ $this->ROW_NAME_ID ]						, 'integer' ) );
				$this->setNome(						unescape( @$objSaida[ $this->ROW_NAME_STR_NAME ]				, 'string' ) );
				$this->setEmail(					unescape( @$objSaida[ $this->ROW_NAME_STR_EMAIL ]				, 'string' ) );
			}
			else
			{
				throw new Exception( 'Usuario ( ' . $this->getId() . ' ) não existe' ) ;
			}
		}
	}
?>