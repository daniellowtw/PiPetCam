var canvas = document.getElementById('myCanvas');
var context = canvas.getContext('2d');

var socket = io.connect('http://192.168.137.179:8887');

var foo = function() {
	socket.emit('clicked foo', {
		action: 'testcam'
	});
	socket.on('received foo', function(data) {
		document.getElementById('result').innerHTML = data.result;
	})
}