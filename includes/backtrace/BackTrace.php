<?php

/**
 * Fornece dados da pilha de execu��o.
 * 
 * Supplies execution stack data.
 * 
 * Manipula e fornece acesso aos dados de uma determinada pilha de execu��o. Por
 * meio de m�todos � poss�vel caminhar entre os n�veis e capturar informa��es
 * respectivas a cada um deles.
 * 
 * @author Renan de Lima Barbosa <renandelima@gmail.com>
 */
class BackTrace
{
	
	/**
	 * Indica que o escopo � de arquivo.
	 * 
	 * Indicates the scope is an file.
	 * 
	 * � um dos poss�veis valores retornado pelo m�todo
	 * {@link BackTrace::getScope()}. Indica que o ponteiro de execu��o faz
	 * refer�ncia � uma instru��o cujo escopo � um arquivo.
	 * 
	 * @var string
	 */
	const SCOPE_FILE = 'file';
	
	/**
	 * Indica que o escopo � de fun��o.
	 * 
	 * Indicates the scope is an function.
	 * 
	 * � um dos poss�veis valores retornado pelo m�todo
	 * {@link BackTrace::getScope()}. Indica que o ponteiro de execu��o faz
	 * refer�ncia � uma instru��o localizada em uma fun��o. Para verificar se o
	 * escopo � uma fun��o de classe utilize as constantes
	 * {@link BackTrace::SCOPE_METHOD} e {@link BackTrace::SCOPE_STATIC}.
	 * 
	 * @var string
	 */
	const SCOPE_FUNCTION = 'function';
	
	/**
	 * Indica que o escopo � de m�todo.
	 * 
	 * Indicates the scope is an method.
	 * 
	 * � um dos poss�veis valores retornado pelo m�todo
	 * {@link BackTrace::getScope()}. Indica que o ponteiro de execu��o faz
	 * refer�ncia � uma instru��o localizada em um m�todo. Para verificar se o
	 * escopo � um m�todo est�tico utilize a constante
	 * {@link BackTrace::SCOPE_STATIC}.
	 * 
	 * @var string
	 */
	const SCOPE_METHOD = '->';
	
	/**
	 * Indica que o escopo � de m�todo est�tico.
	 * 
	 * Indicates the scope is an static method.
	 * 
	 * � um dos poss�veis valores retornado pelo m�todo
	 * {@link BackTrace::getScope()}. Indica que o ponteiro de execu��o faz
	 * refer�ncia � uma instru��o localizada em um m�todo est�tico. Para
	 * verificar se o escopo � um m�todo n�o est�tico utilize a constante
	 * {@link BackTrace::SCOPE_METHOD}.
	 * 
	 * @var string
	 */
	const SCOPE_STATIC = '::';
	
	/**
	 * Cont�m os dados do n�vel corrente.
	 * 
	 * Contains data of current level.
	 * 
	 * Ponteiro para um item da pilha {@link BackTrace::$aStack}. Os m�todos
	 * {@link BackTrace::levelDown()} e {@link BackTrace::levelUp()} s�o
	 * respons�veis por manipular este atributo.
	 * 
	 * @var &string[]
	 */
	private $aCurrent = array();
	
	/**
	 * Cont�m os dados de todos os n�veis.
	 * 
	 * Contains data of all levels.
	 * 
	 * Possui uma pilha com os dados de todos os n�veis do rastreamento. O
	 * conte�do deste atributo � o mesmo retornado pela fun��o
	 * {@link debug_backtrace()}.
	 * 
	 * @var string[][]
	 */
	private $aStack = array();
	
	/**
	 * Indicador do n�vel atual.
	 * 
	 * Pointer of current level.
	 * 
	 * Cont�m o �ndice do escopo atual. O valor desta vari�vel � um �ndice de
	 * {@link BackTrace::$aStack}.
	 * 
	 * @var integer
	 */
	private $iLevel = 0;
	
	/**
	 * Captura dados da pilha de execu��o.
	 * 
	 * Captures execution stack data.
	 * 
	 * Os dados da pilha de execu��o s�o capturados e o ponteiro � movido para o
	 * n�vel imediatamente inferior ao ponto de chamada deste construtor caso a
	 * pilha n�o seja informado pelo par�metro, caso contr�rio a pilha informada
	 * � utilizada.
	 * 
	 * @param string[][] $aStack
	 * @return void
	 */
	public function __construct( array $aStack = array() )
	{
		$this->aStack = count( $aStack ) === 0 ? debug_backtrace() : $aStack;
		$this->levelTop();
	}
	
	/**
	 * Gera HTML com detalhes do rastreamento a partir do n�vel atual.
	 * 
	 * Creates HTML with detail of the trace.
	 * 
	 * Captura todos os dados de um rastreamento a partir do n�vel atual da
	 * pilha.
	 * 
	 * @return string
	 * @see BackTraceExplain::perform()
	 */
	public function explain()
	{
		return BackTraceExplain::perform( $this );
	}
	
	/**
	 * Obt�m todos os dados de todos os n�veis.
	 * 
	 * Gets all data of all levels.
	 * 
	 * Retorna os dados contidos na pilha constru�da pelo construtor dessa
	 * classe. O formato � id�ntico ao retornado pela fun��o
	 * {@link debug_backtrace()}.
	 * 
	 * @return string[]
	 */
	public function getAll()
	{
		return $this->aStack;
	}
	
	/**
	 * Obt�m os argumentos de entrada do n�vel atual.
	 * 
	 * Captures input arguments of current level.
	 * 
	 * Se o escopo do n�vel corrente for uma fun��o ou m�todo retorna uma lista
	 * com os par�metros de chamada. Caso contr�rio retorna uma lista vazia.
	 * 
	 * @return string[]
	 */
	public function getArguments()
	{
		if ( array_key_exists( 'args', $this->aCurrent ) === false )
		{
			return array();
		}
		return (array) $this->aCurrent['args'];
	}
	
	/**
	 * Obt�m o nome da classe do n�vel atual.
	 * 
	 * Captures class name of current level.
	 * 
	 * Se o escopo do n�vel atual for um m�todo ou m�todo est�tico o nome da
	 * classe � retornado. Caso contr�rio retornar� um texto vazio. Para
	 * verificar se o escopo atual � um m�todo, m�todo est�tico, fun��o ou
	 * arquivo utilize {@link BackTrace::getScope()}.
	 * 
	 * @return string
	 */
	public function getClass()
	{
		if ( array_key_exists( 'class', $this->aCurrent ) === false )
		{
			return '';
		}
		return (string) $this->aCurrent['class'];
	}
	
	/**
	 * Obt�m todos os dados do n�vel corrente.
	 * 
	 * Captures all data of current level.
	 *
	 * Retorna os dados do n�vel corrente da pilha de execu��o. O formato �
	 * igual a um item da pilha retornada pela fun��o {@link debug_backtrace()}.
	 *
	 * @return string[]
	 */
	public function getCurrent()
	{
		return $this->aCurrent;
	}
	
	/**
	 * Obt�m o nome do arquivo do n�vel corrente.
	 * 
	 * Captures file of current level.
	 * 
	 * Retorna o caminho completo contendo o nome do arquivo do n�vel atual.
	 * 
	 * @param boolean $bOnlyName captura somente o nome do arquivo
	 * @return string
	 */
	public function getFile( $bOnlyName = false )
	{
		if ( array_key_exists( 'file', $this->aCurrent ) === false )
		{
			return '';
		}
		if ( $bOnlyName === true )
		{
			return basename( (string) $this->aCurrent['file'] );
		}
		return (string) $this->aCurrent['file'];
	}
	
	/**
	 * Obt�m o nome da fun��o do n�vel corrente.
	 * 
	 * Captures function name of current level.
	 * 
	 * Se o escopo do n�vel atual for uma fun��o, m�todo ou m�todo est�tico o
	 * nome desse � retornado. Caso contr�rio retornar� um texto vazio. Para
	 * verificar se o escopo atual � uma fun��o, m�todo, m�todo est�tico ou
	 * arquivo utilize {@link BackTrace::getScope()}.
	 * 
	 * @return string
	 */
	public function getFunction()
	{
		if ( array_key_exists( 'function', $this->aCurrent ) === false )
		{
			return '';
		}
		return (string) $this->aCurrent['function'];
	}
	
	/**
	 * Obt�m a posi��o do n�vel atual do n�vel corrente.
	 * 
	 * Captures position of current level.
	 * 
	 * @return integer
	 */
	public function getLevel()
	{
		return $this->iLevel;
	}
	
	/**
	 * Obt�m n�mero da linha do arquivo do n�vel corrente.
	 * 
	 * Captures file line name of current level.
	 * 
	 * Caso o n�mero da linha n�o esteja dispon�vel o valor 0 � retornado.
	 * 
	 * @return integer
	 */
	public function getLine()
	{
		if ( array_key_exists( 'line', $this->aCurrent ) === false )
		{
			return 0;
		}
		return (integer) $this->aCurrent['line'];
	}
	
	/**
	 * Obt�m o escopo do n�vel corrente.
	 * 
	 * Captures scope of current level.
	 * 
	 * O tipo de escopo define o ambiente do n�vel atual, ele pode ser fun��o,
	 * m�todo (fun��o de classe), est�tico (m�todo est�tico) ou arquivo. Esses
	 * tipos est�o definidos nessa classe pelas constantes com prefixo "SCOPE_".
	 * Os poss�veis valores retornados s�o: {@link BackTrace::SCOPE_FILE},
	 * {@link BackTrace::SCOPE_FUNCTION}, {@link BackTrace::SCOPE_METHOD} e
	 * {@link BackTrace::SCOPE_STATIC}. Os escopos est�o definidos de forma
	 * parecida com o �ndice "type" da lista retornada pela fun��o
	 * {@link debug_backtrace()}, por�m existe diferencia��o entre o escopo
	 * fun��o e arquivo. Para obter o mesmo comportamento desse �ndice "type"
	 * utilize {@link BackTrace::getType()}.
	 * 
	 * @return string
	 */
	public function getScope()
	{
		$sType = $this->getType();
		if ( $sType === '' )
		{
			$sType = $this->getFunction() === '' ? self::SCOPE_FILE : self::SCOPE_FUNCTION;
		}
		return $sType;
	}
	
	/**
	 * Obt�m o tipo do n�vel corrente.
	 * 
	 * Captures type of current level.
	 * 
	 * O tipo de escopo define o ambiente do n�vel atual, ele pode ser m�todo
	 * (fun��o de classe), est�tico (m�todo est�tico) ou nenhum. Para obter um
	 * maior detalhamento do escopo utilize {@link BackTrace::getScope()}.
	 * 
	 * @return string
	 */
	public function getType()
	{
		if ( array_key_exists( 'type', $this->aCurrent ) === false )
		{
			return '';
		}
		return (string) $this->aCurrent['type'];
	}
	
	/**
	 * Move o ponteiro de rastreamento para o n�vel mais baixo.
	 * 
	 * Moves trace pointer to last level.
	 * 
	 * @return void
	 */
	public function levelBottom()
	{
		$this->setLevel( count( $this->aStack ) - 1 );
	}
	
	/**
	 * Move o ponteiro de rastreamento para baixo.
	 * 
	 * Moves trace pointer to down.
	 * 
	 * Quando o valor retronado � verdadeiro a classe passa a manipular as
	 * informa��es do n�vel imediatamente inferior ao atual. Caso contr�rio,
	 * quando o retorno � falso, o ponteiro n�o � alterado. A quantidade de
	 * n�veis a ser abaixado � por padr�o o valor 1, por�m ele pode ser
	 * redefinido pelo primeiro par�metro. Caso n�o seja poss�vel descer a
	 * quantidade de n�veis desejado o segundo par�metro, que por padr�o �
	 * falso, indica se � para descer at� onde for poss�vel.
	 * 
	 * @param integer $iDepth
	 * @param boolean $bTry caso o n�vel n�o exista tentar at� onde for poss�vel
	 * @return boolean
	 */
	public function levelDown( $iDepth = 1, $bTry = false )
	{
		$iDepth = (integer) $iDepth;
		$bReturn = $this->setLevel( $this->iLevel + $iDepth );
		if ( $bReturn === false && $bTry === true )
		{
			$iDepth > 0 ? $this->levelTop() : $this->levelBottom();
		}
		return $bReturn;
	}
	
	/**
	 * Move o ponteiro de rastreamento para o n�vel mais alto.
	 * 
	 * Moves trace pointer to first level.
	 * 
	 * @return void
	 */
	public function levelTop()
	{
		$this->setLevel( 0 );
	}
	
	/**
	 * Move o ponteiro de rastreamento para cima.
	 * 
	 * Moves trace pointer to up.
	 * 
	 * Quando o valor retronado � verdadeiro a classe passa a manipular as
	 * informa��es do n�vel imediatamente superior ao atual. Caso contr�rio,
	 * quando o retorno � falso, o ponteiro n�o � alterado. A quantidade de
	 * n�veis a ser subido � por padr�o o valor 1, por�m ele pode ser redefinido
	 * pelo primeiro par�metro. Caso n�o seja poss�vel subir a quantidade de
	 * n�veis desejado o segundo par�metro, que por padr�o � falso, indica se �
	 * para subir at� onde for poss�vel.
	 * 
	 * @param integer $iDepth
	 * @param boolean $bTry caso o n�vel n�o exista tentar at� onde for poss�vel
	 * @return boolean
	 */
	public function levelUp( $iDepth = 1, $bTry = false )
	{
		$iDepth = (integer) $iDepth;
		$bReturn = $this->setLevel( $this->iLevel - $iDepth );
		if ( $bReturn === false && $bTry === true )
		{
			$iDepth > 0 ? $this->levelBottom() : $this->levelTop();
		}
		return $bReturn;
	}
	
	/**
	 * Move o ponteiro de rastreamento.
	 * 
	 * Moves trace pointer.
	 * 
	 * Quando o valor retronado � verdadeiro a classe passa a manipular as
	 * informa��es do n�vel indicado pelo par�metro. Caso contr�rio, quando o
	 * retorno � falso, o ponteiro n�o � alterado.
	 * 
	 * @param integer $iLevel n�vel do rastreamento
	 * @return boolean
	 */
	public function setLevel( $iLevel )
	{
		$iLevel = (integer) $iLevel;
		if ( array_key_exists( $iLevel, $this->aStack ) === true )
		{
			$this->iLevel = $iLevel;
		}
		$this->aCurrent = &$this->aStack[$this->iLevel];
		return $this->iLevel === $iLevel;
	}
	
}
