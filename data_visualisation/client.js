var canvas = document.getElementById('myCanvas');
var context = canvas.getContext('2d');
var imageObj = new Image();
imageObj.src = 'latest.jpg';

var socket = io.connect('http://192.168.137.179:8887');

function clear_load() {
	document.getElementById('spinner').style.display = 'none';
}

function start_load() {
	document.getElementById('spinner').style.display = 'block';
}

var foo = function(action) {
	// start_load();
	socket.emit('clicked foo', {
		action: action
	});
	socket.on('received foo', function(data) {
		if (data.target) document.getElementById(data.target).innerHTML = data.data;
		document.getElementById('result').innerHTML = data.result;
		if (data.show_image) update_last();
	});
	// socket.on('finished loading', clear_load());
};

function update_last() {
	// imageObj.onload = function() {
		context.drawImage(imageObj, 0, 0, canvas.width, canvas.height);
	// };
	imageObj.src = 'latest.jpg';
}

function writeMessage(canvas, message) {
	var context = canvas.getContext('2d');
	context.clearRect(0, 0, canvas.width, canvas.height);
	context.drawImage(imageObj, 0, 0, canvas.width, canvas.height);
	context.font = '18pt Calibri';
	context.fillStyle = 'black';
	context.fillText(message, 10, 25);
}

function getMousePos(canvas, evt) {
	var rect = canvas.getBoundingClientRect();
	return {
		x: evt.clientX - rect.left,
		y: evt.clientY - rect.top
	};
}

canvas.addEventListener('mousemove', function(evt) {
	var mousePos = getMousePos(canvas, evt);
	var message = 'Mouse position: ' + mousePos.x + ',' + mousePos.y;
	writeMessage(canvas, message);
}, false);

// imageObj.onload = function() {
// 	var ratioWidth = canvas.width/ imageObj.width
// 	, ratioHeight = canvas.height/ imageObj.height;	
// }