var canvas;
var context;
var imageObj;
var xx; //for heatmap
// canvas = document.getElementById('myCanvas');
// context = canvas.getContext('2d');
// imageObj = new Image();
// var socket = io.connect('http://192.168.137.211:8887');
var socket = io.connect('http://192.168.137.57:8887');

function getMousePos(canvas, evt) {
	var rect = canvas.getBoundingClientRect();
	return {
		x: evt.clientX - rect.left,
		y: evt.clientY - rect.top
	};
}

function heatmap_setup() {
	xx = h337.create({
		"element": document.getElementById("heatmapArea"),
		"radius": 55,
		"visible": true,
	});
	xx.get("canvas").onclick = function(ev) {
		var pos = h337.util.mousePosition(ev);
		xx.store.addDataPoint(pos[0], pos[1]);
	};
	xx.get("element").style["background"] = "url(../latest.jpg)";
	xx.get("element").style["backgroundSize"] = 'cover';
	get_latest();
	socket.emit('load_heatmap_data',"pet_location.txt");
}

function snapshot_setup(canvas, statusbox) {
	context = canvas.getContext('2d');

	var selection = {};
	var selection_started = false;
	var first_point, second_point;

	canvas.addEventListener('mousemove', function(evt) {
		var mousePos = getMousePos(canvas, evt);
		var message = parseInt(mousePos.x) + ',' + parseInt(mousePos.y);
		// console.log(mousePos);
		statusbox.value = message;
		// writeMessage(canvas, message);
	}, false);
	canvas.addEventListener('mousedown', function(evt) {});
	canvas.addEventListener('mouseup', function(evt) {
		if (second_point != null) return;
		var mousePos = getMousePos(canvas, evt);
		if (!selection_started) {
			selection_started = true;
			first_point = mousePos;
		} else {
			selection_started = false;
			second_point = mousePos;
			draw_rect(first_point, second_point);
			send_selection(first_point, second_point);
		}
	}, false);
}

function draw_rect(first, second) {
	// context.clearRect(0, 0, canvas.width, canvas.height);
	// update_picture();
	var x = Math.min(first.x, second.x);
	var y = Math.min(first.y, second.y);
	var w = Math.abs(first.x - second.x);
	var h = Math.abs(first.y - second.y);

	context.rect(x, y, w, h);
	context.lineWidth = 2;
	context.strokeStyle = 'red';
	context.stroke();
}

function capture() {
	socket.emit('snapshot');
}

function update_picture() {
	imageObj = new Image();
	imageObj.src = 'latest.jpg';
	imageObj.onload = display;
}

function display() {
	context.drawImage(imageObj, 0, 0, canvas.width, canvas.height);
}

function send_selection(first, second) {
	var payload = {
		"first": first,
		"second": second
	};
	socket.emit('send_coord', payload);
}

function send_command(command) {
	socket.emit('send_command', command);
}

function get_latest() {
	if (xx == null) return;
	socket.emit('heatmap_data');
	socket.on('data', function(data) {
		xx.store.setDataSet(data);
	})
}