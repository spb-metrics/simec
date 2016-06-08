<?php
/**
 * classe TLabel
 * Classe para constru��o de r�tulos de texto
 */
class TLabel extends TField
{
    private $fontSize;  // tamanho da fonte
    private $fontFace;  // nome da fonte
    private $fontColor; // cor da fonte
    private $tag;
    /**
     * m�todo construtor
     * instancia o label, cria um objeto <font>
     * @param  $value = Texto do Label
     */
    public function __construct($value)
    {
        // atribui o conte�do do label
        $this->setValue($value);
        // instancia um elemento <font>
        $this->tag = new TElement('font');
        
        // define valores iniciais �s propriedades
        $this->fontSize  = '14';
        $this->fontFace  = 'Arial';
        $this->fontColor = 'black';
    }
    
    /**
     * m�todo setSize
     * Define o tamanho da fonte
     * @param  $size    = tamanho da fonte
     */
    public function setFontSize($size)
    {
        $this->fontSize = $size;
    }
    
    /**
     * m�todo setFontFace
     * Define a fam�lia da fonte
     * @param  $font = nome da fonte
     */
    public function setFontFace($font)
    {
        $this->fontFace = $font;
    }
    
    /**
     * m�todo setFontColor
     * Define a cor da fonte
     * @param  $color   = cor da fonte
     */
    public function setFontColor($color)
    {
        $this->fontColor = $color;
    }
    
    /**
     * m�todo show()
     * exibe o widget na tela
     */
    public function show()
    {
        // define os estilo da tag
        $this->tag->style = "font-family:{$this->fontFace}; ".
                            "color:{$this->fontColor}; ".
                            "font-size:{$this->fontSize}";
        // adiciona o conte�do � tag
        $this->tag->add($this->value);
        // exibe a tag
        $this->tag->show();
    }
}
?>