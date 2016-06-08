<?php
/*
   Sistema Simec-financeiro
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano
   Programador: Marcus Vinicius Arouck de Souza  (e-mail: marcus.souza@mec.gov.br)
   Módulo:importarsiafi.inc
   Finalidade: permitir importar arquivos do SIAFI
*/

  
$DB = new cls_banco();
$sql = "select * from financeiro.contacorrentecontabil";
$RS = $db->record_set($sql);

$i = pg_num_fields($RS);

echo $i;




?>

<html>
<head>
<title>Importação SIAFI</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
</head>

<body bgcolor="#FFFFFF" text="#000000">
<h2 align="center">IMPORTA&Ccedil;&Atilde;O SIAFI</h2>
<form name="upload" action="/importa&acao=I" method="post" enctype="multipart/form-data" onsubmit="return teste()">
  <table align="center">
    <tr bordercolor="#000000" bgcolor="#CCCCCC"> 
      <td width="2%" bgcolor="#FFFFFF"> 
        <div align="center"></div>
      </td>
      <td width="9%" bgcolor="#FFFFFF"> 
        <div align="center">Sele&ccedil;&atilde;o</div>
      </td>
      <td width="22%"> 
        <div align="center">Nome</div>
      </td>
      <td width="22%"> 
        <div align="center">C&oacute;digo</div>
      </td>
      <td width="18%"> 
        <div align="center">Data</div>
      </td>
      <td width="27%"> 
        <div align="center">Tabela</div>
      </td>
    </tr>
    <tr> 
      <td width="2%"><font face="Arial, Helvetica, sans-serif">1</font></td>
      <td width="9%"> 
        <div align="center"><font face="Arial, Helvetica, sans-serif"> 
          <input type="checkbox" name="sel1" value="checkbox">
          </font></div>
      </td>
      <td width="22%"> 
        <div align="center"><font face="Arial, Helvetica, sans-serif"> 
          <input type="text" name="nome1" value="saldo_contabil">
          </font></div>
      </td>
      <td width="22%"> 
        <div align="center"><font face="Arial, Helvetica, sans-serif"> 
          <input type="text" name="codigo1" value="MC150014" size="14">
          </font></div>
      </td>
      <td width="18%"> 
        <div align="center"><font face="Arial, Helvetica, sans-serif"> 
          <input type="text" name="data1" value="20060801" maxlength="8" size="12">
          </font></div>
      </td>
      <td width="27%"> 
        <div align="center"><font face="Arial, Helvetica, sans-serif"> 
          <input type="text" name="tabela1" value="tb_siof_saldo_contabil">
          </font></div>
      </td>
    </tr>
    <tr> 
      <td width="2%">2</td>
      <td width="9%"> 
        <div align="center"> 
          <input type="checkbox" name="sel2" value="checkbox">
        </div>
      </td>
      <td width="22%"> 
        <div align="center"> 
          <input type="text" name="nome2" value="notas_de_emprenho">
        </div>
      </td>
      <td width="22%"> 
        <div align="center"> 
          <input type="text" name="codigo2" value="NE150014" size="14">
        </div>
      </td>
      <td width="18%"> 
        <div align="center"> 
          <input type="text" name="data2" value="20060801" size="12" maxlength="8">
        </div>
      </td>
      <td width="27%"> 
        <div align="center"> 
          <input type="text" name="tabela2" value="tb_sof_nota_empenho">
        </div>
      </td>
    </tr>
    <tr> 
      <td width="2%">3</td>
      <td width="9%"> 
        <div align="center"> 
          <input type="checkbox" name="sel3" value="checkbox">
        </div>
      </td>
      <td width="22%"> 
        <div align="center"> 
          <input type="text" name="nome3" value="nota_do_sistema">
        </div>
      </td>
      <td width="22%"> 
        <div align="center"> 
          <input type="text" name="codigo3" value="NS150014" size="14">
        </div>
      </td>
      <td width="18%"> 
        <div align="center"> 
       <input type="text" name="data3" value= "20060801" size="12"  maxlength="8" > 
        </div>
      </td>
      <td width="27%"> 
        <div align="center"> 
          <input type="text" name="tabela3" value="tb_nota_sistema">
        </div>
      </td>
    </tr>
    <tr> 
      <td width="2%">4</td>
      <td width="9%"> 
        <div align="center"> 
          <input type="checkbox" name="sel4" value="checkbox">
        </div>
      </td>
      <td width="22%"> 
        <div align="center"> 
          <input type="text" name="nome4" value="ordem_bancaria">
        </div>
      </td>
      <td width="22%"> 
        <div align="center"> 
          <input type="text" name="codigo4" value="OB150014" size="14">
        </div>
      </td>
      <td width="18%"> 
        <div align="center"> 
          <input type="text" name="data4" value="20060801" size="12" maxlength="8">
        </div>
      </td>
      <td width="27%"> 
        <div align="center"> 
          <input type="text" name="tabela4" value="tb_ordem_bancaria">
        </div>
      </td>
    </tr>
    <tr> 
      <td width="2%">5</td>
      <td width="9%"> 
        <div align="center"> 
          <input type="checkbox" name="sel5" value="checkbox">
        </div>
      </td>
      <td width="22%"> 
        <div align="center"> 
          <input type="text" name="nome5">
        </div>
      </td>
      <td width="22%"> 
        <div align="center"> 
          <input type="text" name="codigo5" size="14">
        </div>
      </td>
      <td width="18%"> 
        <div align="center"> 
          <input type="text" name="data5" size="12" maxlength="8">
        </div>
      </td>
      <td width="27%"> 
        <div align="center"> 
          <input type="text" name="tabela5">
        </div>
      </td>
    </tr>
  </table>
  <p align="center">Local do arquivo TXT: 
    <input name="arquivo" type="file"><BR>
 
  </p>
  <p align="center"> 
    <input type="submit" name="Submit" value="Importar">
  </p>
</form>
</body>
</html>

