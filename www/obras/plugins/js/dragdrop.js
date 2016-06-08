	var operaBrowser = false;
	if(navigator.userAgent.indexOf('Opera')>=0)operaBrowser=1;
	var MSIE = navigator.userAgent.indexOf('MSIE')>=0?true:false;
	var navigatorVersion = navigator.appVersion.replace(/.*?MSIE (\d\.\d).*/g,'$1')/1;
	
	function cancelEvent()
	{
		return false;
	}
	var activeImage = false;
	var readyToMove = false;
	var moveTimer = -1;
	var dragDropDiv;
	var insertionMarker;
	
	var offsetX_marker = -3;	// offset X - element that indicates destinaton of drop
	var offsetY_marker = 0;	// offset Y - element that indicates destinaton of drop
	
	var firefoxOffsetX_marker = -3;
	var firefoxOffsetY_marker = -2;
	
	if(navigatorVersion<6 && MSIE){	/* IE 5.5 fix */
		offsetX_marker-=23;
		offsetY_marker-=10;		
	}
	
	var destinationObject = false;
	
	var divXPositions = new Array();
	var divYPositions = new Array();
	var divWidth = new Array();
	var divHeight = new Array();
	
	var PaidivXPositions = new Array();
	var PaidivYPositions = new Array();
	var PaidivWidth = new Array();
	var PaidivHeight = new Array();
	
		
	var tmpLeft = 0;
	var tmpTop  = 0;
	
	var eventDiff_x = 0;
	var eventDiff_y = 0;
		
	function getTopPos(inputObj)
	{		
	  var returnValue = inputObj.offsetTop;
	  while((inputObj = inputObj.offsetParent) != null){
	  	if(inputObj.tagName!='HTML'){
	  		returnValue += (inputObj.offsetTop - inputObj.scrollTop);
	  		if(document.all)returnValue+=inputObj.clientTop;
	  	}
	  } 
	  return returnValue;
	}
	function OpenerGetTopPos(inputObj)
	{		
	  var returnValue = inputObj.offsetTop;
	  while((inputObj = inputObj.offsetParent) != null){
	  	if(inputObj.tagName!='HTML'){
	  		returnValue += (inputObj.offsetTop - inputObj.scrollTop);
	  		if(window.opener.document.all)returnValue+=inputObj.clientTop;
	  	}
	  } 
	  return returnValue;
	}
	function OpenerGetLeftPos(inputObj)
	{	  
	  var returnValue = inputObj.offsetLeft;
	  while((inputObj = inputObj.offsetParent) != null){
	  	if(inputObj.tagName!='HTML'){
	  		returnValue += inputObj.offsetLeft;
	  		if(window.opener.document.all)returnValue+=inputObj.clientLeft;
	  	}
	  }
	  return returnValue;
	}
	
	function getLeftPos(inputObj)
	{	  
	  var returnValue = inputObj.offsetLeft;
	  while((inputObj = inputObj.offsetParent) != null){
	  	if(inputObj.tagName!='HTML'){
	  		returnValue += inputObj.offsetLeft;
	  		if(document.all)returnValue+=inputObj.clientLeft;
	  	}
	  }
	  return returnValue;
	}
			
	function selectImage(e)
	{
		if(document.all && !operaBrowser)e = event;
		var obj = this.parentNode;
		if(activeImage)activeImage.className='imageBox';
		obj.className = 'imageBoxHighlighted';
		activeImage = obj;
		readyToMove = true;
		moveTimer=0;
		
		tmpLeft = e.clientX + Math.max(document.body.scrollLeft,document.documentElement.scrollLeft);
		tmpTop = e.clientY + Math.max(document.body.scrollTop,document.documentElement.scrollTop);
		
		startMoveTimer();	
		
		return false;	
	}
	
	function startMoveTimer(){
		if(moveTimer>=0 && moveTimer<10){
			moveTimer++;
			setTimeout('startMoveTimer()',15);
		}
		if(moveTimer==10){
			getDivCoordinates();
			var subElements = dragDropDiv.getElementsByTagName('DIV');
			if(subElements.length>0){
				dragDropDiv.removeChild(subElements[0]);
			}
		
			dragDropDiv.style.display='block';
			var newDiv = activeImage.cloneNode(true);
			newDiv.className='imageBox';	
			newDiv.id='';
			dragDropDiv.appendChild(newDiv);	
			dragDropDiv.style.top = tmpTop + 'px';
			dragDropDiv.style.left = tmpLeft + 'px';
							
		}
		return false;
	}
	
	function dragDropEnd()
	{
		readyToMove = false;
		moveTimer = -1;

		dragDropDiv.style.display='none';
		
		insertionMarker.style.display='none';
		
		if(destinationObject && destinationObject!=activeImage){
			var parentObj = destinationObject.parentNode;
			parentObj.insertBefore(activeImage,destinationObject);		
			activeImage.className='imageBox';
			activeImage = false;
			destinationObject=false;
			saveImageOrder();
			getDivCoordinates();
			if(Criterio.atualizar) {
				window.opener.AtualizaFotos();
			}
			
		}
		return false;
	}
	
	function dragDropMove(e)
	{
		if(moveTimer==-1)return;
		if(document.all && !operaBrowser)e = event;
		var leftPos = e.clientX + document.documentElement.scrollLeft - eventDiff_x;
		var topPos = e.clientY + document.documentElement.scrollTop - eventDiff_y;
		
		dragDropDiv.style.top = topPos + 'px';
		dragDropDiv.style.left = leftPos + 'px';
		leftPos = leftPos + eventDiff_x;
		topPos = topPos + eventDiff_y;
		
		if(e.button!=1 && document.all &&  !operaBrowser)dragDropEnd();
		var elementFound = false;
		
		for(var prop in divXPositions){
			if(divXPositions[prop]/1 < leftPos/1 && (divXPositions[prop]/1 + divWidth[prop]*0.7)>leftPos/1 && divYPositions[prop]/1<topPos/1 && (divYPositions[prop]/1 + divWidth[prop])>topPos/1){
				
				if(document.all ){
					offsetX = offsetX_marker;
					offsetY = offsetY_marker;
				}else{
					offsetX = firefoxOffsetX_marker;
					offsetY = firefoxOffsetY_marker;
				}
				insertionMarker.style.top = divYPositions[prop] + offsetY + 'px';
				insertionMarker.style.left = divXPositions[prop] + offsetX + 'px';
				insertionMarker.style.display='block';	
				destinationObject = document.getElementById(prop);
				elementFound = true;	
				break;	
			}				
		}
		
		
		if(!elementFound){
			insertionMarker.style.display='none';
			destinationObject = false;
		}
		
		return false;
		
	}

	function getDivCoordinates()
	{
		var divs = document.getElementsByTagName('DIV');
				
		for(var no=0;no<divs.length;no++){	
			if(divs[no].className=='imageBox' || divs[no].className=='imageBoxHighlighted' && divs[no].id){
				
				divXPositions[divs[no].id] = getLeftPos(divs[no]);
				divYPositions[divs[no].id] = getTopPos(divs[no]);			
				divWidth[divs[no].id] = divs[no].offsetWidth;					
				divHeight[divs[no].id] = divs[no].offsetHeight;
			}
		}			
	}
	
	function saveImageOrder()
	{
		var orderString = "";
		
		var objects = document.getElementsByTagName('DIV');
		var pai_objects = window.opener.document.getElementById('thumbnails');
		var divs = new Array();
		
		//apaga todas as divs do opener thumbnails
		while (pai_objects.firstChild) {
			pai_objects.removeChild(pai_objects.firstChild);
		}

		var controledivs=0;
		for(var no=0;no<objects.length;no++){
			
			if(objects[no].className=='imageBox'){
				//if(Find(divs,objects[no].id) == -1){
				var attributes = new Array();
				attributes["id"] = "imageBox"+controledivs;
				controledivs++;
				attributes["class"] = "imageBox";
				if(objects[no].id != ""){
					
					var new_div = CreateElement(false,"div",pai_objects,attributes);
					
					if(MSIE){
						new_div.id = objects[no].id;
						new_div.className = "imageBox";
					}
				
					divs.push(new_div.id);
					
					var attributes = new Array();
					attributes["class"] = "imageBox_theImage";
					attributes["style"] = "margin:2px;";
					attributes["src"] = objects[no].firstChild.src;
					attributes["id"] = objects[no].firstChild.id;
					
					var new_img = CreateElement(false,"img",new_div,attributes);
					
					if(MSIE){
						new_img.id = objects[no].firstChild.id;
						new_img.className = "imageBox_theImage";
						new_img.style.margin = "0px";
						new_img.src = objects[no].firstChild.src;
					}
					
					var attributes      = new Array();
					
					attributes["type"]  = "hidden"; 
					attributes["name"]  = objects[no].id;
					attributes["value"] = objects[no].firstChild.id;
					attributes["id"]    = objects[no].id+"_"+objects[no].firstChild.id;
					var new_hidden = CreateElement(false,"input",new_div,attributes);
					
					if(MSIE){
						new_hidden.id = objects[no].id+"_"+objects[no].firstChild.id;
						new_hidden.value = objects[no].firstChild.id;
						new_hidden.type = "hidden";
						new_hidden.name = objects[no].id;
					}
						
				}	
				
			//}		
		}
				
	}
		
		var t = 0;
		
		for(var k=0;k<20;k++){
			
			var div_new = "imageBox"+k;
															
			if(Find(divs,div_new) == -1){
										
				var attributes = new Array();
				attributes["class"] = "imageBox";
				attributes["id"] = div_new;	
				var new__div = CreateElement(false,"div",pai_objects,attributes);
				if(MSIE){
					new__div.id = div_new;
					new__div.className = "imageBox";
				}
				
					
			}			
		}
	
	}
	function initGallery()
	{
		
		var divs = document.getElementsByTagName('IMG');
		for(var no=0;no<divs.length;no++){
		
			if(divs[no].className=='imageBox_theImage' || divs[no].className=='imageBox_label'){
				divs[no].onmousedown = selectImage;	

			}
		}
		
		var insObj = document.getElementById('insertionMarker');
		var images = insObj.getElementsByTagName('IMG');
		
		document.body.onselectstart = cancelEvent;
		document.body.ondragstart = cancelEvent;
		document.body.onmouseup = dragDropEnd;
		document.body.onmousemove = dragDropMove;

		
		window.onresize = getDivCoordinates;
		
		dragDropDiv = document.getElementById('dragDropContent');
		insertionMarker = document.getElementById('insertionMarker');
		getDivCoordinates();
	}