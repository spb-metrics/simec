<?php

function curPageURL() {
	 $pageURL = 'http';
	 if ($_SERVER["HTTPS"] == "on") {$pageURL .= "s";}
	 $pageURL .= "://";
	 if ($_SERVER["SERVER_PORT"] != "80") {
	  $pageURL .= $_SERVER["SERVER_NAME"].":".$_SERVER["SERVER_PORT"].$_SERVER["REQUEST_URI"];
	 } else {
	  $pageURL .= $_SERVER["SERVER_NAME"].$_SERVER["REQUEST_URI"];
	 }
	 return $pageURL;
}

$local= explode("/",curPageURL());

?>
<html>
	<head>
		<title><?php echo $GLOBALS['parametros_sistema_tela']['nome_e_orgao']; ?></title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script src="/includes/prototype.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

		<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;sensor=false&amp;key=<?= $GLOBALS['parametros_chave_googlemaps']['simec'] ?>" type="text/javascript"></script>

		<script type="text/javascript">
		
			document.write('<script type="text/javascript" src="../includes/extlargemapcontrol'+(document.location.search.indexOf('packed')>-1?'_packed':'')+'.js"><'+'/script>');

			var marcadores = new Array();
			var markerGroups = new Array();

			var map;
			
			// Criando o ícones do mapa
			var customIcons = [];

			var marca = new GIcon();

			marca.iconSize = new GSize(9, 14);
			marca.iconAnchor = new GPoint(9, 15);
			marca.infoWindowAnchor = new GPoint(9, 2);
			marca.image = '../imagens/icone_capacete_1.png';
			customIcons[1] = marca;

			var marca = new GIcon();
			marca.iconSize = new GSize(9, 14);
			marca.iconAnchor = new GPoint(9, 15);
			marca.infoWindowAnchor = new GPoint(9, 2);
			marca.image = '../imagens/icone_capacete_2.png';
			customIcons[2] = marca;

			var marca = new GIcon();
			marca.iconSize = new GSize(9, 14);
			marca.iconAnchor = new GPoint(9, 15);
			marca.infoWindowAnchor = new GPoint(9, 2);
			marca.image = '../imagens/icone_capacete_3.png';
			customIcons[3] = marca;

			var marca = new GIcon();
			marca.iconSize = new GSize(9, 14);
			marca.iconAnchor = new GPoint(9, 15);
			marca.infoWindowAnchor = new GPoint(9, 2);
			marca.image = '../imagens/icone_capacete_4.png';
			customIcons[4] = marca;

			var marca = new GIcon();
			marca.iconSize = new GSize(9, 14);
			marca.iconAnchor = new GPoint(9, 15);
			marca.infoWindowAnchor = new GPoint(9, 2);
			marca.image = '../imagens/icone_capacete_101.png';
			customIcons[101] = marca;

			var marca = new GIcon();
			marca.iconSize = new GSize(9, 14);
			marca.iconAnchor = new GPoint(9, 15);
			marca.infoWindowAnchor = new GPoint(9, 2);
			marca.image = '../imagens/icone_capacete_102.png';
			customIcons[102] = marca;

			var marca = new GIcon();
			marca.iconSize = new GSize(9, 14);
			marca.iconAnchor = new GPoint(9, 15);
			marca.infoWindowAnchor = new GPoint(9, 2);
			marca.image = '../imagens/icone_capacete_103.png';
			customIcons[103] = marca;

			var marca = new GIcon();
			marca.iconSize = new GSize(9, 14);
			marca.iconAnchor = new GPoint(9, 15);
			marca.infoWindowAnchor = new GPoint(9, 2);
			marca.image = '../imagens/icone_capacete_104.png';
			customIcons[104] = marca;

			markerGroups = {1:[], 2:[], 3:[], 4:[], 101:[], 102:[], 103:[], 104:[]};
		 
			
			function visao(tipo){
				if (tipo=='mapa')
					map.setMapType(G_NORMAL_MAP);
				if (tipo=='satelite')
					map.setMapType(G_SATELLITE_MAP);
				if (tipo=='hibrido')
					map.setMapType(G_HYBRID_MAP);
				if (tipo=='terreno')
					map.setMapType(G_PHYSICAL_MAP);		
			}
			
			function mapa_original (){
				// Preenchendo o valor inicial do mapa
				var zoom = 4;	var lat_i = -14.689881; var lng_i = -52.373047;		
				map.setCenter(new GLatLng(lat_i,lng_i), parseInt(zoom));
			}
		
			function initialize() {
		      if (GBrowserIsCompatible()) {
		       var opts = {
				 googleBarOptions : {
				   style : 'new'
				 }
				}
				map = new GMap2(document.getElementById("mapa"), opts);
				// Controles      	
				map.addControl(new GLargeMapControl3D());
		        map.addControl(new GOverviewMapControl());
		        map.enableScrollWheelZoom();
		        map.addMapType(G_PHYSICAL_MAP);
		
		       	// Preenchendo o valor inicial do mapa
				var zoom = 4;	var lat_i = -14.689881; var lng_i = -52.373047;		
				map.setCenter(new GLatLng(lat_i,lng_i), parseInt(zoom));
		
				// Mostrar Lat e Lng MouseMove
				GEvent.addListener(map,"mousemove",function(latlng) {
			       	document.getElementById('lat').value=dec2grau(latlng.lat());
			        document.getElementById('lng').value=dec2grau(latlng.lng());
		        });
		
				} 
			}
		
		   function dec2grau(valor){
				ddLat=valor+"";
				if (ddLat.substr(0,1) == "-") {
					ddLatVal = ddLat.substr(1,ddLat.length-1);
				} else {
					ddLatVal = ddLat;
				}
				// Graus 
				ddLatVals = ddLatVal.split(".");
				dmsLatDeg = ddLatVals[0];
				// * 60 = mins
				ddLatRemainder  = ("0." + ddLatVals[1]) * 60;
				dmsLatMinVals   = ddLatRemainder.toString().split(".");
				dmsLatMin = dmsLatMinVals[0];
				// * 60 novamente = secs
				ddLatMinRemainder = ("0." + dmsLatMinVals[1]) * 60;
				dmsLatSec  = Math.round(ddLatMinRemainder);
				
				if (ddLat.substr(0,1) == "-") {
					valor="-"+dmsLatDeg+unescape('%B0')+" "+dmsLatMin+"' "+dmsLatSec+"''";
				} else {
					valor=dmsLatDeg+unescape('%B0')+" "+dmsLatMin+"' "+dmsLatSec+"''";
				}
		
		     	return valor;
		     }

		   function createMarker(point,obrdesc,type,obrid) {
				var icone = customIcons[type];
				var marker = new GMarker(point,{title: obrdesc, icon: icone, draggable:false });
				GEvent.addListener(marker, 'click', function() {
					html='<iframe src=/obras/obras.php?modulo=principal/mapa_balao&acao=A&requisicao=supervisao&obrid='+obrid+' width=350 frameborder=0 ></iframe>';
			        marker.openInfoWindowHtml(html);
			    	});
			 return marker;
			}
			
		   function carregar(texto_busca){

			   	// Limpar todos os marcadores
				map.clearOverlays();

				var xml_filtro = "mapa_gera_xml.php?requisicao=supervisao&orgid=" + <?php print $_SESSION["obras"]["orgidRepositorio"]; ?>;

				// window.open(xml_filtro);
				
				// Criando os Marcadores com o resultado
				GDownloadUrl(xml_filtro, function(data) {
					var xml = GXml.parse(data);
					var markers = xml.documentElement.getElementsByTagName("marker");
					if(markers.length > 1) {
					var lat_ant=0;
					var lng_ant=0;
					for (var i = 0; i < markers.length; i++) {
						var obrdesc = markers[i].getAttribute("obrdesc");
						var obrid = markers[i].getAttribute("idobra");
						type = parseInt(markers[i].getAttribute("orgid"));
			
						// Verifica pontos em um mesmo lugar e move o seguinte para a direita
						if(lat_ant==markers[i].getAttribute("lat") && lng_ant==markers[i].getAttribute("lng"))
							var point = new GLatLng(markers[i].getAttribute("lat"),	markers[i].getAttribute("lng"));
						else
							var point = new GLatLng(markers[i].getAttribute("lat"),	parseFloat(markers[i].getAttribute("lng"))+0.0005);				
			
						lat_ant=markers[i].getAttribute("lat");
						lng_ant=markers[i].getAttribute("lng");
						
						// Cria o marcador na tela
						var marker=createMarker(point,obrdesc,type,obrid);
						markerGroups[type].push(marker);
						marcadores[obrid]=marker;
						map.addOverlay(marker);
			          }
					} else {
						alert('Não existem obras no repositório!');
					}
			        });
		     }    
		</script>
		<script type="text/javascript" src="../includes/FusionCharts/FusionCharts.js"></script>
	</head>
	<body>
		<table bgcolor=#c3c3c3 width=100%>
		    <tr>
		    	<td align=left><input type="button" value="Mapa Brasil" onClick="javascript: mapa_original();" style="cursor: pointer;"></td>
		    	<td align=center></td><td align=center></td>
		    	<td align="right">
		    		<input type="button" value="Mapa" onClick="javascript: visao('mapa');" style="cursor: pointer;">
		    		<input type="button" value="Satélite" onClick="javascript: visao('satelite');" style="cursor: pointer;">
		    		<input type="button" value="Híbrido" onClick="javascript: visao('hibrido');" style="cursor: pointer;">
		    		<input type="button" value="Terreno" onClick="javascript: visao('terreno');" style="cursor: pointer;">
		    	</td>
		    </tr>
		</table>
		
		<!-- Mapa -->
		<div id="mapa" style="width: 100%; height: 520px; position: relative"></div>
		<!-- Mapa -->
		
		<table bgcolor=#c3c3c3 width=100%>
		    <tr>
		    	<td>
		    		Latitude: <input type=text id=lat name=lat value="0" STYLE="border:none ; background-color:#c3c3c3" size=8 /> 
		    		Longitude: <input type=text id=lng name=lng value="0" STYLE="border:none; background-color:#c3c3c3" size=8 />
		    	</td>
			</tr>
		</table>
		<script type="text/javascript">initialize(); carregar();</script>
	</body>
</html>
