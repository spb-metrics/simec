<?
//Habilitar quando estiver em Produ��o
?>
<html>
	<head>
		<title>Acesso Negado</title>
	</head>
	<body>
		<script type="text/javascript">
		   alert ( 'Acesso Negado!\nVoc� n�o tem permiss�o para acessar esta tela.' );
		   if (navigator.appName.indexOf('Netscape') == 0)
		   		history.go(-1);
		   else
		   		history.go(-2);
		</script>
	</body>
</html>
