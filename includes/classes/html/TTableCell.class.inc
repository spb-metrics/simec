<?php
/**
 * classe TTableCell
 * Reponsсvel pela exibiчуo de uma cщlula de uma tabela
 */
class TTableCell extends TElement
{
    /**
     * mщtodo construtor
     * Instancia uma nova cщlula
     * @param  $value = conteњdo da cщlula
     */
    public function __construct($value)
    {
        parent::__construct('td');
        parent::add($value);
        
        
    }
    
    function setAlign($align){
    	
    	if (strlen($align) == 1){
	        $this->class = ( strtoupper($align) == 'L' ? "SubTituloEsquerda" : "SubTituloDireita");
    	}else{
	        $this->align = $align;
    	}    
    }
}
?>