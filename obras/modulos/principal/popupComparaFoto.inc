<?php
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

$obrid = $_SESSION["obra"]["obrid"];
$obras = new Obras($obrid);
$dados = $obras->Dados($obrid);
$dobras = new DadosObra($dados);
$endereco = new Endereco($dobras->getEndId());
echo $obras->CabecalhoObras();
$cmfid = $_GET['cmfid'];
$sql = "select 
			cmf.arqid,
			cmf.arqidpar,
			arq.arqdescricao 
		from 
			obras.compararfoto cmf
		inner join
			public.arquivo arq ON arq.arqid = cmf.arqid 
		where 
			cmf.cmfid = $cmfid";
$foto = $db->pegaLinha($sql);

?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<link href="../includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" type="text/css" rel="Stylesheet" />
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/afterbefore/jquery.beforeafter.min.js"></script>
<script>
$(function(){
	$('#container').beforeAfter({ imagePath:'../includes/JQuery/afterbefore/' , showFullLinks : false });
	$('#foto_depois>img').fadeTo(0,0);
	
	if($('#foto_depois>img').width() >= $('#foto_antes>img').width()){
		var div = "foto_depois";
		$('#foto_antes').css({
					'margin-left': ($('#foto_depois>img').width() - $('#foto_antes>img').width()) / 2 + "px",
				}).css;
	}else{
		var div = "foto_antes";
		$('#foto_depois').css({
					'margin-left': ($('#foto_antes>img').width() - $('#foto_depois>img').width()) / 2 + "px",
				}).css;
	}
	
	$('#fotos').css({
					'text-aling': "center",
					'width': $('#' + div + '>img').width(),
					'height': $('#' + div + '>img').height(),
				}).css;
	$('#slider').css({
					'width': $('#foto_depois>img').width(),
					'margin-top': '10px',
				}).css;
	$('#slider').slider({
					value:0,
					min:0,
					max:100
				});
	$('#slider').bind('slide',function(){
					$('#foto_depois>img').fadeTo(0,$(this).slider('value')/100);
				});
	
});
<?php if ($endereco->medlatitude && $endereco->medlongitude ):?>
	function abreMapaPopUP()
	{
		<?php
			$medlatitude = $endereco->medlatitude;
			$latitude = explode(".", $medlatitude);
			$graulatitude = $latitude[0];
			$minlatitude = $latitude[1];
			$seglatitude = $latitude[2];
			$pololatitude = $latitude[3];
			
			$medlongitude = $endereco->medlongitude;
			$longitude = explode(".", $medlongitude);
			$graulongitude = $longitude[0];
			$minlongitude = $longitude[1];
			$seglongitude = $longitude[2];
			
		?>
		var graulatitude = "<?php echo $graulatitude ?>";
		var minlatitude  = "<?php echo $minlatitude ?>";
		var seglatitude  = "<?php echo $seglatitude ?>";
		
		var graulongitude = "<?php echo $graulongitude ?>";
		var minlongitude  = "<?php echo $minlongitude ?>";
		var seglongitude  = "<?php echo $seglongitude ?>";
		
		var latitude  = ((( Number(seglatitude) / 60 ) + Number(minlatitude)) / 60 ) + Number(graulatitude);
		var longitude = ((( Number(seglongitude) / 60 ) + Number(minlongitude)) / 60 ) + Number(graulongitude);
		
		window.location.href= 'obras.php?modulo=relatorio/mapa_resultado&acao=A&janela=popup&longitude='+longitude+'&latitude='+latitude;
	}
<?php endif; ?>
</script>
<style>
	#foto_antes {z-index:2;position:absolute;z-index:1}
	#foto_depois {position:absolute;z-index:2}
	#slider {width:500px}
</style>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
	    <td align='center' style="background-color:#cccccc;font-weight:bold" >
	    	Comparação de Fotos
	    </td>
	</tr>
	<?php if ($endereco->medlatitude && $endereco->medlongitude ):?>
		<tr>
		    <td align='center' style="font-weight:bold" >
		    	<a href="javascript:abreMapaPopUP()" >Visualizar no Mapa</a>
		    </td>
		</tr>
	<?php endif; ?>
	<tr>
	    <td align='center'>
	    	<div id="content">
				<div id="container">
					<div><img alt="antes" src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $foto['arqid'] ?>&newwidth=550&newheight=500" /></div>
					<div><img alt="depois" src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $foto['arqidpar'] ?>&newwidth=550&newheight=500" /></div>
				</div>
			</div>
	    </td>
	</tr>
	<tr>
	    <td align='center' ><?php echo $foto['arqdescricao'] ?></td>
	</tr>
	<tr>
	    <td align='center'>
	    	<div id="fotos" />
				<div id="foto_depois">
					<img alt="depois" src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $foto['arqidpar'] ?>&newwidth=550&newheight=500" />
				</div>
				<div id="foto_antes">
		    		<img alt="antes" src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $foto['arqid'] ?>&newwidth=550&newheight=500" />
				</div>
			</div>
	    </td>
	</tr>
	<tr>
	    <td align='center'><div id="slider"></div></td>
	</tr>
	<tr>
	    <td align='center' ><?php echo $foto['arqdescricao'] ?></td>
	</tr>
</table>