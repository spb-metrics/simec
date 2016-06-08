<?

define("MAX_NIVEL", 2);
$id=0;

function viz($codigo, $nivel=1, $codigopai = null) {
	global $db, $id;

	if($codigopai) {
		$cl =  "OR ro.ocpid=".$codigopai."";
		//$cl = "AND (oc.ocpid!='".$codigopai."' OR oc.ocpid IS NULL)";
	}
	
	$sql = "SELECT usu.usucpf as codigo, usu.usunome as descricao 
			FROM gestaopessoa.relacionamentoocupacao ro 
			INNER JOIN seguranca.usuario usu ON usu.usucpf = ro.fdpcpf 
			WHERE ro.ocpid IN (SELECT ocpid FROM gestaopessoa.ocupacao WHERE fdpcpf='".$codigo."') ".$cl;
	
	//die($sql);
	
	$vizinhos = $db->carregar($sql);
	
	for($i=0;$i<=$nivel;$i++) {
		$espacamento .= chr(9);
	}
	
	
	if($vizinhos[0]) {
		foreach($vizinhos as $vv) {
			$json_child[] = chr(10).$espacamento.'{"id": "'.($id++).'_'.$vv['codigo'].'", "name": "'.$vv['descricao'].'", "data": {"band": "Jerome Dillon","relation": "member of band"}, "children": ['.(($nivel<MAX_NIVEL && $vv['codigo'])?viz($vv['codigo'],($nivel+1), $codigo):"").']}';

		}
		return implode(",",$json_child);
	}
}

function carregarJsonCodigo($dados) {
	global $db, $id;
	if($dados['idinicio']) $id=$dados['idinicio']; 
	
	$sql = "SELECT u.usucpf as codigo, u.usunome as descricao FROM seguranca.usuario u 
			WHERE u.usucpf = '".$dados['codigo']."'";
	
	$centro = $db->pegaLinha($sql);
	$json = 'var json = {"id":     "'.($id++).'_'.$centro['codigo'].'","name":   "'.$centro['descricao'].'", "children": ['.viz($centro['codigo']).'], "data":   ""};';
	echo $json;
}


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

?>

<!-- CSS Files -->
<link type="text/css" href="../includes/jit/Examples/css/base.css" rel="stylesheet" />
<link type="text/css" href="../includes/jit/Examples/css/Hypertree.css" rel="stylesheet" />

<!--[if IE]><script language="javascript" type="text/javascript" src="../../Extras/excanvas.js"></script><![endif]-->

<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<!-- JIT Library File -->
<script language="javascript" type="text/javascript" src="../includes/jit/jit.js"></script>

<!-- Example File -->
<script language="javascript" type="text/javascript">
var labelType, useGradients, nativeTextSupport, animate;

var carregar = false;

(function() {
  var ua = navigator.userAgent,
      iStuff = ua.match(/iPhone/i) || ua.match(/iPad/i),
      typeOfCanvas = typeof HTMLCanvasElement,
      nativeCanvasSupport = (typeOfCanvas == 'object' || typeOfCanvas == 'function'),
      textSupport = nativeCanvasSupport 
        && (typeof document.createElement('canvas').getContext('2d').fillText == 'function');
  //I'm setting this based on the fact that ExCanvas provides text support for IE
  //and that as of today iPhone/iPad current text support is lame
  labelType = (!nativeCanvasSupport || (textSupport && !iStuff))? 'Native' : 'HTML';
  nativeTextSupport = labelType == 'Native';
  useGradients = nativeCanvasSupport;
  animate = !(iStuff || !nativeCanvasSupport);
})();

var Log = {
  elem: false,
  write: function(text){
    if (!this.elem) 
      this.elem = document.getElementById('log');
    this.elem.innerHTML = text;
    this.elem.style.left = (500 - this.elem.offsetWidth / 2) + 'px';
  }
};

function carregarJsonCodigo(codigo,id) {
	var jsm;
	
	$.ajax({
   		type: "POST",
   		url: "gestaopessoa.php?modulo=principal/popupVerPoliticos&acao=A",
   		data: "requisicao=carregarJsonCodigo&codigo="+codigo+"&idinicio="+id,
   		async: false,
   		success: function(msg){
   			jsm = msg;
   		}
 		});
 	
 	return jsm;
}

function init(){
	/* Carregando a variavel "json" utilizada */
    var json_ = carregarJsonCodigo('<? echo $_SESSION['fdpcpf']; ?>','0');
    eval(json_);

    var infovis = document.getElementById('infovis');
    var w = infovis.offsetWidth - 50, h = infovis.offsetHeight - 50;
    
    function recarregarArvore(no) {
		var arr = no.split('_');
		var json_ = carregarJsonCodigo(arr[1],arr[0]);
		//alert(json_);
		eval(json_);
		//load JSON data.
		ht.loadJSON(json);
 
	    ht.op.morph(json, {
	            type: 'fade:con',
		 	  	duration: 1000,  
		   	  	fps: 35,  
	            hideLabels: true,
	            onComplete: function(){ Log.write("morph complete!"); }
	   });

	   
    };
    
    
    //init Hypertree
    var ht = new $jit.Hypertree({
      //id of the visualization container
      injectInto: 'infovis',
      //canvas width and height
      width: w,
      height: h,
      //Change node and edge styles such as
      //color, width and dimensions.
      Node: {
          dim: 8,
          color: "#f00"
      },
      Edge: {
          lineWidth: 1,
          color: "#088"
      },
      onBeforeCompute: function(node){
          Log.write("centering");
      },
      //Attach event handlers and add text to the
      //labels. This method is only triggered on label
      //creation
      onCreateLabel: function(domElement, node){
          domElement.innerHTML = node.name;
          $jit.util.addEvent(domElement, 'click', function () {
          	  ht.onClick(node.id);
          });
      },
      //Change node styles when labels are placed
      //or moved.
      onPlaceLabel: function(domElement, node){
          var style = domElement.style;
          style.display = '';
          style.cursor = 'pointer';
          if (node._depth <= 1) {
              style.fontSize = "0.8em";
              style.color = "#ddd";

          } else if(node._depth == 2){
              style.fontSize = "0.7em";
              style.color = "#555";

          } else {
              style.display = 'none';
          }

          var left = parseInt(style.left);
          var w = domElement.offsetWidth;
          style.left = (left - w / 2) + 'px';
      },
      onAfterCompute: function(){
          Log.write("done");
          //Build the right column relations list.
          //This is done by collecting the information (stored in the data property) 
          //for all the nodes adjacent to the centered node.
          var node = ht.graph.getClosestNodeToOrigin("current");
          /*
      	  if(carregar) {
      	  	recarregarArvore(node.id);
			carregar = false;
      	  } else {
      	  	carregar = true;
      	  }
      	  */
          var html = "<h4>" + node.name + "</h4><b>Connections:</b>";
          html += "<ul>";
          node.eachAdjacency(function(adj){
              var child = adj.nodeTo;
              if (child.data) {
                  var rel = (child.data.band == node.name) ? child.data.relation : node.data.relation;
                  html += "<li>" + child.name + " " + "<div class=\"relation\">(relation: " + rel + ")</div></li>";
              }
          });
          html += "</ul>";
          $jit.id('inner-details').innerHTML = html;
      }
    });
    
    //load JSON data.
    ht.loadJSON(json);
    //compute positions and plot.
    ht.refresh();
    //end
    ht.controller.onAfterCompute();
}


</script>
</head>

<body onload="init();">
<div id="container">

<div id="center-container">
	<div id="id-list"></div>
    <div id="infovis"></div>    
</div>

<div id="right-container">

<div id="inner-details"></div>

</div>

<div id="log"></div>
</div>
</body>