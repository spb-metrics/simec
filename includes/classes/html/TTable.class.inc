<?php
/**
 * classe TTable
 * responsсvel pela exibiчуo de tabelas
 */
class TTable extends TElement
{
    /**
     * mщtodo construtor
     * Instancia uma nova tabela
     */
    public function __construct()
    {
        parent::__construct('table');
        
        $this->cellspacing="1";
        $this->cellpadding="3";
        $this->bgcolor="#f5f5f5";
        $this->align="center";
        $this->width="95%";
        $this->class="tabela";
        
    }

    /**
     * mщtodo addRow
     * Agrega um novo objeto linha (TTableRow) na tabela
     */
    public function addRow()
    {
        // instancia objeto linha
        $row = new TTableRow;
        // armazena no aray de linhas
        parent::add($row);
        return $row;
    }
}
?>