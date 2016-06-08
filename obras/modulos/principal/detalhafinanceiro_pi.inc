<?
$ptres=$_REQUEST['ptres'];
?>
<html>
<head>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
</head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<table cellpadding="0" border="0" style="width:95.3%" align="center" >
	<tr>
		<td style="background-color: #C9C9C9;" align="center"><font size=2><b>Detalhamento Financeiro do PI</font></b></td>
	</tr>
	<tr>
		<td align='center' style="background-color: #dcdcdc;"><b>PTRES:</b> <?= $ptres ?></td>
		<td></td>
	</tr>
</table>
<?php
$sql = "SELECT pi.plicod, pi.plititulo, coalesce(SUM(pip.pipvalor),0) as plivalor 
		FROM monitora.pi_planointerno pi
			inner join monitora.pi_planointernoptres pip ON pi.pliid = pip.pliid
			inner join monitora.ptres ptres ON pip.ptrid = ptres.ptrid
		WHERE pi.plistatus='A' 
			and ptres.ptres = '".$ptres."'
			and ptres.ptrano = '{$_GET['pliano']}'
			group by pi.plicod, pi.plititulo


";
$cabecalho = array ("Código", "Título", "Valor");
$db->monta_lista($sql,$cabecalho,60,20,'S','95%','');
?>
<br>
<br>
<center>
	<input type="button" name="bot" value="Fechar" onclick="self.close();">
</center>
</body>
</html>