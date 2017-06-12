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

if(window.localStorage.getItem('autocontrolscript') == null){
	window.localStorage.setItem('autocontrolscript', ""
		+ "  var let_left = check_let0();\n"
		+ "  var let_center = check_let1();\n"
		+ "  var let_right = check_let2();\n"
		+ "  \n"
		+ "  if(!let_left && !let_center && !let_right){\n"
		+ "     console.log('_1')\n"
		+ "     forward(100);\n"
		+ "  }else if(!let_left && let_center && let_right){\n"
		+ "     console.log('_2')\n"
		+ "     backward(400);\n"
		+ "     turnleft(400);\n"
		+ "  }else if(!let_left && !let_center && let_right){\n"
		+ "     console.log('_3')\n"
		+ "     backward(200);\n"
		+ "     turnleft(400);\n"
		+ "  }else if(let_left && let_center && !let_right){\n"
		+ "     console.log('_4')\n"
		+ "     backward(200);\n"
		+ "     turnright(400);\n"
		+ "  }else if(let_left && !let_center && !let_right){\n"
		+ "     console.log('_5')\n"
		+ "     backward(200);\n"
		+ "     turnright(300);\n"
		+ "  }else if(let_left && let_center && let_right){\n"
		+ "     console.log('_6')\n"
		+ "     backward(400);\n"
		+ "     turnleft(400);\n"
		+ "  }else if(!let_left && let_center && !let_right){\n"
		+ "     console.log('_7')\n"
		+ "     backward(200);\n"
		+ "     turnleft(200);\n"
		+ "  }else if(let_left && !let_center && let_right){\n"
		+ "  console.log('_8')\n"
		+ "     backward(400);\n"
		+ "     turnleft(400);\n"
		+ "  }\n"
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
	
	$('.autocontrol-programming').unbind().bind('click', function(){
		$('.connectionform').hide();
		$('.controlpanel').hide();
		$('.autocontrolpanel').css({'display': 'table'});
		simulate_draw();
	});
	
	$('#simulate_start').unbind().bind('click', function(){
		simulate_begin();
	});
	
	$('#simulate_stop').unbind().bind('click', function(){
		simulate_end();
	});
	
	$('#autocontrolscript textarea').val(window.localStorage.getItem('autocontrolscript'));
	$('#autocontrolscript textarea').unbind().bind('input propertychange', function(){
		console.log('1');
		window.localStorage.setItem('autocontrolscript', $('#autocontrolscript textarea').val());
	});
	
	// for test
	$('.connectionform').hide();
	$('.controlpanel').hide();
	$('.autocontrolpanel').css({'display': 'table'});
	simulate_draw();
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

var objects = [
	{ x: 466, y: 47,  w: 95,  h: 150 },
	{ x: 320, y: 174, w: 570, h: 8 },
	{ x: 114, y: 276, w: 26,  h: 371 },
	{ x: 453, y: 432, w: 451, h: 144 }
];

function getRandomInt(min, max){
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

function randomize(){
	objects = [];
	objects.push({
		x: getRandomInt(0,600),
		y: getRandomInt(0,600),
		w: getRandomInt(0,600),
		h: getRandomInt(0,600),
	});
	simulate_draw();
}

function draw_objects(){
	var ctx = document.getElementById("simulate_cnv").getContext('2d');
	for(var i in objects){
		var o = objects[i];
		ctx.strokeRect(o.x, o.y, o.w, o.h);
		ctx.fillStyle = "#F10101";
		ctx.fillRect(o.x, o.y, o.w, o.h);
	}
}

var robot = {
	x: 300,
	y: 300,
	a: 0,
	w: 80,
	h: 102, // w*1.27
	t: 37, // h*0.36
	l: 37 //
};

function pass_border(x,max_x){
	if(x < 0 || x > max_x){
		simulate_fail();
	}
}

function calculate_let0(){
	var let0 = {}

	let0.x1 = robot.x + (-1*(0)*Math.cos(robot.a) - (-1*(robot.h/2)*Math.sin(robot.a)));
	let0.y1 = robot.y + (-1*(0)*Math.sin(robot.a) + (-1*(robot.h/2)*Math.cos(robot.a)));
	
	let0.x2 = robot.x + ((-35)*Math.cos(robot.a) - (-1*(robot.h/2 + robot.l)*Math.sin(robot.a)));
	let0.y2 = robot.y + ((-35)*Math.sin(robot.a) + (-1*(robot.h/2 + robot.l)*Math.cos(robot.a)));
	return let0;
}

function calculate_let1(){
	var let1 = {}
	
	var t2 = (robot.h/2 + robot.l);
	let1.x1 = robot.x + (-1*(0)*Math.cos(robot.a) - (-1*(robot.h/2)*Math.sin(robot.a)));
	let1.y1 = robot.y + (-1*(0)*Math.sin(robot.a) + (-1*(robot.h/2)*Math.cos(robot.a)));
	
	let1.x2 = robot.x + ((0)*Math.cos(robot.a) - (-1*(robot.h/2 + robot.l)*Math.sin(robot.a)));
	let1.y2 = robot.y + ((0)*Math.sin(robot.a) + (-1*(robot.h/2 + robot.l)*Math.cos(robot.a)));
	return let1;
}

function calculate_let2(){
	var let1 = {}
	
	var t2 = (robot.h/2 + robot.l);
	let1.x1 = robot.x + (-1*(0)*Math.cos(robot.a) - (-1*(robot.h/2)*Math.sin(robot.a)));
	let1.y1 = robot.y + (-1*(0)*Math.sin(robot.a) + (-1*(robot.h/2)*Math.cos(robot.a)));
	
	let1.x2 = robot.x + ((35)*Math.cos(robot.a) - (-1*(robot.h/2 + robot.l)*Math.sin(robot.a)));
	let1.y2 = robot.y + ((35)*Math.sin(robot.a) + (-1*(robot.h/2 + robot.l)*Math.cos(robot.a)));
	return let1;
}

function draw_robot(){
	var canvas = document.getElementById("simulate_cnv");
	var ctx = canvas.getContext('2d');
	window.max_x = canvas.width;
	window.max_y = canvas.height;
	
	var x1 = robot.x + (-1*(robot.w/2)*Math.cos(robot.a) - (-1*(robot.h/2)*Math.sin(robot.a)));
	var y1 = robot.y + (-1*(robot.w/2)*Math.sin(robot.a) + (-1*(robot.h/2)*Math.cos(robot.a)));
	
	var x2 = robot.x + (-1*(robot.w/2)*Math.cos(robot.a) - ((robot.h/2)*Math.sin(robot.a)));
	var y2 = robot.y + (-1*(robot.w/2)*Math.sin(robot.a) + ((robot.h/2)*Math.cos(robot.a)));
	
	var x3 = robot.x + ((robot.w/2)*Math.cos(robot.a) - ((robot.h/2)*Math.sin(robot.a)));
	var y3 = robot.y + ((robot.w/2)*Math.sin(robot.a) + ((robot.h/2)*Math.cos(robot.a)));
	
	var x4 = robot.x + ((robot.w/2)*Math.cos(robot.a) - (-1*(robot.h/2)*Math.sin(robot.a)));
	var y4 = robot.y + ((robot.w/2)*Math.sin(robot.a) + (-1*(robot.h/2)*Math.cos(robot.a)));
	
	ctx.strokeStyle = '#000';
	ctx.beginPath();
	ctx.moveTo(x1, y1);
	ctx.lineTo(x2, y2);
	ctx.lineTo(x3, y3);
	ctx.lineTo(x4, y4);
	ctx.lineTo(x1, y1);
	ctx.stroke();
	ctx.fillStyle = "#e1e1e1";
	ctx.fill();
	
	pass_border(x1, max_x);
	pass_border(x2, max_x);
	pass_border(x3, max_x);
	pass_border(x4, max_x);
	pass_border(y1, max_y);
	pass_border(y2, max_y);
	pass_border(y3, max_y);
	pass_border(y4, max_y);
	
	var t = (robot.h/2 - robot.t);
	var x5 = robot.x + (-1*(robot.w/2)*Math.cos(robot.a) - (-1*(t)*Math.sin(robot.a)));
	var y5 = robot.y + (-1*(robot.w/2)*Math.sin(robot.a) + (-1*(t)*Math.cos(robot.a)));
	
	var x6 = robot.x + ((robot.w/2)*Math.cos(robot.a) - (-1*(t)*Math.sin(robot.a)));
	var y6 = robot.y + ((robot.w/2)*Math.sin(robot.a) + (-1*(t)*Math.cos(robot.a)));
	
	ctx.strokeStyle = '#000';
	ctx.beginPath();
	ctx.moveTo(x5, y5);
	ctx.lineTo(x6, y6);
	ctx.stroke();
	
	var let0 = calculate_let0();
	
	ctx.strokeStyle = '#e1e1e1';
	ctx.beginPath();
	ctx.moveTo(let0.x1, let0.y1);
	ctx.lineTo(let0.x2, let0.y2);
	ctx.stroke();
	
	var let1 = calculate_let1();

	ctx.strokeStyle = '#e1e1e1';
	ctx.beginPath();
	ctx.moveTo(let1.x1, let1.y1);
	ctx.lineTo(let1.x2, let1.y2);
	ctx.stroke();
	
	
	var let2 = calculate_let2();

	ctx.strokeStyle = '#e1e1e1';
	ctx.beginPath();
	ctx.moveTo(let2.x1, let2.y1);
	ctx.lineTo(let2.x2, let2.y2);
	ctx.stroke();
		
	// TODO Implement more detalization
}

function simulate_func(timing){}

var simulate_commands = [];

var simulate_status = '';

function simulate_end(){
	simulate_status = 'simulate_end';
}

function simulate_fail(){
	simulate_status = 'simulate_fail';
}

function simulate_turnleft(t){
	console.log("turnleft");
	simulate_status = 'turnleft';
	simulate_commands.push({cmd: 'turnleft', 't': t});
}

function simulate_turnright(t){
	console.log("turnright");
	simulate_status = 'turnright';
	simulate_commands.push({cmd: 'turnright', 't': t});
}

function simulate_forward(t){
	console.log("forward");
	simulate_status = 'forward';
	simulate_commands.push({cmd: 'forward', 't': t});
}

function simulate_backward(t){
	console.log("backward");
	simulate_status = 'backward';
	simulate_commands.push({cmd: 'backward', 't': t});
}

function simulate_stop(t){
	console.log("stop");
	simulate_status = 'stop';
	simulate_commands.push({cmd: 'stop', 't': t});
}

function check_cross(x1,y1,x2,y2,let0){
	
	var dx1 = x2 - x1;
	var dy1 = y2 - y1;
	var dx2 = let0.x2 - let0.x1;
	var dy2 = let0.y2 - let0.y1;
	var x = dy1 * dx2 - dy2 * dx1;
	if(!x || !dx2){
		console.error("simulate_check_let0 Failed");
		return false;
	}else{
		if(x == 0){
			return false;
		}
		var y = let0.x1 * let0.y2 - let0.y1 * let0.x2;
		x = ((x1 * y2 - y1 * x2) * dx2 - y * dx1) / x;
		y = (dy2 * x - y) / dx2;
		
		var x1_ = Math.min(x1,x2);
		var x2_ = Math.max(x1,x2);
		var let0_x1_ = Math.min(let0.x1,let0.x2);
		var let0_x2_ = Math.max(let0.x1,let0.x2);
		
		var y1_ = Math.min(y1,y2);
		var y2_ = Math.max(y1,y2);
		var let0_y1_ = Math.min(let0.y1,let0.y2);
		var let0_y2_ = Math.max(let0.y1,let0.y2);
		
		// result = ((x1 <= x && x2 >= x) || (x2 <= x && x1 >= x)) && ((let0.x1 <= x && let0.x2 >= x) || (let0.x2 <= x && let0.x1 >= x));
		result = (x1_ <= x && x2_ >= x) && (let0_x1_ <= x && let0_x2_ >= x) && (y1_ <= y && y2_ >= y) && (let0_y1_ <= y && let0_y2_ >= y);
		
		if(result){
			/*console.log("x = " + x + " y = " + y);
			var canvas = document.getElementById("simulate_cnv"),
			ctx = canvas.getContext('2d');
			ctx.strokeStyle = '#000';
			ctx.beginPath();
			ctx.arc(x, y, 5, 0, 2 * Math.PI, false);
			ctx.fillStyle = 'green';
			ctx.fill();
			ctx.lineWidth = 5;
			ctx.strokeStyle = '#003300';
			ctx.stroke();*/
			return true;
		}
	}
	return false;
}

function simulate_check_let0(){
	console.log("check_let0");
	var result = false;
	var let0 = calculate_let0();
	var let1 = calculate_let1();
	var let2 = calculate_let2();
	for(var i in objects){
		var o = objects[i];
		var x = o.x;
		var y = o.y;
		var h = o.h;
		var w = o.w;
	
		if(check_cross(x, y, x + w, y,let0)
			|| check_cross(x, y, x, y + h,let0)
			|| check_cross(x + w, y, x + w, y + h,let0)
			|| check_cross(x, y + h, x + w, y + h,let0)
			|| check_cross(0, 0, max_x, 0,let0)
			|| check_cross(0, max_y, max_x, max_y,let0)
			|| check_cross(0, 0, 0, max_y,let0)
			|| check_cross(max_x, 0, max_x, max_y,let0)
		){
			return true;
		}
	}
	return result;
}

function simulate_check_let1(){
	console.log("check_let1");
	var result = false;
	var let1 = calculate_let1();
	for(var i in objects){
		var o = objects[i];
		var x = o.x;
		var y = o.y;
		var h = o.h;
		var w = o.w;
	
		if(check_cross(x, y, x + w, y,let1)
			|| check_cross(x, y, x, y + h,let1)
			|| check_cross(x + w, y, x + w, y + h,let1)
			|| check_cross(x, y + h, x + w, y + h,let1)
			|| check_cross(0, 0, max_x, 0,let1)
			|| check_cross(0, max_y, max_x, max_y,let1)
			|| check_cross(0, 0, 0, max_y,let1)
			|| check_cross(max_x, 0, max_x, max_y,let1)
		){
			return true;
		}
	}
	return result;
}

function simulate_check_let2(){
	console.log("check_let2");
	var result = false;
	var let2 = calculate_let2();
	for(var i in objects){
		var o = objects[i];
		var x = o.x;
		var y = o.y;
		var h = o.h;
		var w = o.w;
	
		if(check_cross(x, y, x + w, y, let2)
			|| check_cross(x, y, x, y + h,let2)
			|| check_cross(x + w, y, x + w, y + h,let2)
			|| check_cross(x, y + h, x + w, y + h,let2)
			|| check_cross(0, 0, max_x, 0,let2)
			|| check_cross(0, max_y, max_x, max_y,let2)
			|| check_cross(0, 0, 0, max_y,let2)
			|| check_cross(max_x, 0, max_x, max_y,let2)
		){
			return true;
		}
	}
	return result;
}

function simulate_begin(){
	simulate_status = '';
	var canvas = document.getElementById("simulate_cnv");
	
	robot.x = 300;
	robot.y = 300;
	robot.a = 0;
	simulate_draw();
	
	var text = $('#autocontrolscript textarea').val();
	// text = text.replaceAll('sleep', 'await sleep');
	// text = text.replace(new RegExp('sleep', 'g'), "await sleep");
	text = text.replace(new RegExp('forward', 'g'), "simulate_forward");
	text = text.replace(new RegExp('backward', 'g'), "simulate_backward");
	text = text.replace(new RegExp('turnleft', 'g'), "simulate_turnleft");
	text = text.replace(new RegExp('turnright', 'g'), "simulate_turnright");
	text = text.replace(new RegExp('check_let0', 'g'), "simulate_check_let0");
	text = text.replace(new RegExp('check_let1', 'g'), "simulate_check_let1");
	text = text.replace(new RegExp('check_let2', 'g'), "simulate_check_let2");
	text = text.replace(new RegExp('stop', 'g'), "simulate_stop");
	text = 'window.simulate_func = function(timing) {' + text + '}';
	console.log(text);
	eval(text);
	// window.simulate_func = setTimeout(execute_roboscript,1);
		
	var inter = setInterval(function(){
		console.log("1");
		if(simulate_status == 'simulate_end'){
			clearInterval(inter);
			inter = undefined;
			return;
		}

		if(simulate_status == 'simulate_fail'){
			simulate_status = 'simulate_end';
			simulate_draw('fail');
			clearInterval(inter);
			inter = undefined;
			return;
		}
	
		if(simulate_commands.length != 0){
			if(simulate_commands[0].t <= 0){
				simulate_commands = simulate_commands.slice(1);
			}
		}
		
		if(simulate_commands.length == 0){
			simulate_func();
		}

		if(simulate_commands.length == 0){
			return;
		}
		
		var cmd = simulate_commands[0].cmd;
		var t = simulate_commands[0].t;
		
		simulate_commands[0].t -= 100;

		if(cmd == 'turnleft'){
			robot.a -= 0.1;
		}else if(cmd == 'turnright'){
			robot.a += 0.1;
		}else if(cmd == 'forward'){
			var dx = (-1*0*Math.cos(robot.a) - (-1*(5)*Math.sin(robot.a)));
			var dy = (-1*0*Math.sin(robot.a) + (-1*(5)*Math.cos(robot.a)));
			robot.x += dx;
			robot.y += dy;
			
			robot.x += dx;
			robot.y += dy;
			
		}else if(cmd == 'backward'){
			var dx = (-1*0*Math.cos(robot.a) - (-1*(5)*Math.sin(robot.a)));
			var dy = (-1*0*Math.sin(robot.a) + (-1*(5)*Math.cos(robot.a)));
			robot.x -= dx;
			robot.y -= dy;
		}
		simulate_draw();
	},100);
}

function simulate_draw(text){
	var canvas = document.getElementById("simulate_cnv"),
	ctx = canvas.getContext('2d');
	ctx.fillStyle = "#FFF";
	ctx.strokeStyle = '#000';
	ctx.lineWidth = 1;
	ctx.fillRect(0, 0, canvas.width, canvas.height);
	ctx.strokeRect(0, 0, canvas.width, canvas.height);
	draw_robot();
	draw_objects();

	if(text){
		ctx.font = "30px Arial";
		ctx.fillStyle = "red";
		ctx.textAlign = "center";
		ctx.fillText(text,canvas.width/2,canvas.height/2);
	}
}
