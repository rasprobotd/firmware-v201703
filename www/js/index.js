/*$(document).ready(function(){
	setInterval(function(){
		image1.src="webcam.php?t=" + Date.now();
	}, 1000);
});*/

function updateControl(s){
	
	var types = ['forward', 'backward', 'turnleft', 'turnright', 'stop'];
	for(var i in types){
		var t = types[i];
		if($('.hands-control').hasClass(t) && s != t){
			$('.hands-control').removeClass(t);
		}

		if(!$('.hands-control').hasClass(t) && s == t){
			$('.hands-control').addClass(t);
		}
	}
}

var type = "stop";
var connected = false;
var roboscript_working = false;
$(document).keydown(function(e){
	if($('#roboscript textarea').is(":focus") || roboscript_working){
		return;
	}
	// console.log("keydown ", e.keyCode);
	var keysCode = {38: 'forward', 40: 'backward', 37: 'turnleft', 39: 'turnright'}
	var new_type = 'stop';
	if(keysCode[e.keyCode]){
		new_type = keysCode[e.keyCode];
	}
	changeType(new_type);
});

$(document).keyup(function(e){
	if($('#roboscript textarea').is(":focus") || roboscript_working){
		return;
	}
	// console.log("keyup ", e.keyCode);
	changeType("stop");
});

var cmdh = {'forward': drive, 'backward': drive, 'turnleft': drive, 'turnright': drive, 'stop': stop_}
function changeType(t){
	if(type != t){
		type = t;
		if(connected == true){
			cmdh[type](type);
			updateControl(type);
		}
	}
}

/*
setInterval(function(){

	if(type == "forward"){
		drive("forward");
	}

	if(type == "backward"){
		drive("backward");
	}

	if(type == "turnleft"){
		drive("turnleft");
	}

	if(type == "turnright"){
		drive("turnright");
	}
	
	if(type == "stop"){
		stop();
	}
	updateControl(type);
}, 200);*/

var lastSendedCommand = ''; 

function drive(cmd){
	if(connected == false)
		return;
	if(lastSendedCommand != cmd){
		lastSendedCommand = cmd;
		window.socket.send(JSON.stringify({'cmd':cmd}));
	}
}

function sleep_(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}

async function sleep(ms) {
  await sleep_(ms);
}

function forward(){
	changeType('forward');
}

function backward(){
	changeType('backward');
}

function turnleft(){
	changeType('turnleft');
}

function turnright(){
	changeType('turnright');
}

function stop(){
	changeType('stop');
}

function stop_(){
	if(connected == false)
		return;
	if(lastSendedCommand != 'stop'){
		lastSendedCommand = 'stop';
		window.socket.send(JSON.stringify({'cmd':'stop'}));
	}
}


if(window.localStorage.getItem('roboscript') == null){
	window.localStorage.setItem('roboscript', ""
		+ "  turnleft(); sleep(1000); stop();\n"
		+ "  forward(); sleep(1000); stop();\n"
		+ "  backward(); sleep(1000); stop();\n"
		+ "  turnright(); sleep(1000); stop();\n"
	)
}

$(document).ready(function(){
	$('#connect').click(function(){
		$('.connectionform').hide();
		window.socket = new WebSocket($('#address_ws').val());
		window.socket.onopen = function() {
			$('.controlpanel').css({'display': 'table'});
			connected = true;
		};

		window.socket.onclose = function(event) {
			$('.controlpanel').hide();
			$('.connectionform').show();
			
			if (event.wasClean) {
				console.log('Disconnected');
			} else {
				console.log('Connection refused');
			}
			console.log('Code: ' + event.code + ' Reason: ' + event.reason);
			connected = false;
		};

		window.socket.onmessage = function(event) {
			
			if(event.data instanceof Blob){
				
				/*var ctx = document.getElementById('canvas').getContext('2d');
				var img = new Image;
				img.onload = function() {
					ctx.drawImage(img, 20,20);
					alert('the image is drawn');
				}
				img.src = URL.createObjectURL(e.target.files[0]);*/

				// $('.controlpanel').css({'display': 'table'});
				// $('#webcam').attr({'src' : URL.createObjectURL(event.data)})
			}else{
				console.log("Recieved data " + event.data);
			}
			
		};

		window.socket.onerror = function(error) {
			console.log("Error " + error.message);
			// $('.connectionform').hide();
			// $('#status').hide();
			// $('#webcam').hide();
			// connected = false;
		};
	});

	$('#roboscript textarea').val(window.localStorage.getItem('roboscript'));
	$('#roboscript textarea').unbind().bind('input propertychange', function(){
		window.localStorage.setItem('roboscript', $('#roboscript textarea').val());
	});
	
	$('#run').unbind().bind('click', function(){
		var text = $('#roboscript textarea').val();
		// text = text.replaceAll('sleep', 'await sleep');
		text = text.replace(new RegExp('sleep', 'g'), "await sleep");
		console.log(text);
		text = 'window.execute_roboscript = async function() {roboscript_started(); ' + text + ' roboscript_ended();}';
		eval(text);
		window.execute_roboscript_t = setTimeout(execute_roboscript,1);
	});
})

function roboscript_started(){
	roboscript_working = true;
	console.log("roboscript_started");
	$('#roboscript textarea').css({'background': 'silver'});
}

function roboscript_ended(){
	roboscript_working = false;
	console.log("roboscript_ended");
	$("#roboscript textarea").removeAttr('readonly');
	$('#roboscript textarea').css({'background': ''});
	stop();
}
