// Declaring our variables
var express = require('express'),
	sys = require('sys'),
	app = express(),
	server = require('http').createServer(app),
	path = require('path'),
	exec = require('child_process').exec,
	io = require('socket.io').listen(server),
	spawn = require('child_process').spawn

var heatmap_store = {
	max: 20,
	data: []
};


// Setting up our server environment
app.set('port', process.env.TEST_PORT || 8887);
app.use(express.favicon());
app.use(express.logger('dev'));
app.use(express.bodyParser());
app.use(express.methodOverride());
app.use(express.static(path.join(__dirname, '/'))); // we need this so that we can link static files rather than using absolute pathname
app.get('/', function(req, res) {
	res.sendfile(__dirname + '/main.html');
});

server.listen(app.get('port'), function() {
	console.log('Express server listening on port ' + app.get('port'));
});

app.get('/snapshot', function(req, res) {
	res.sendfile('snapshot.html');
});
app.get('/heatmap', function(req, res) {
		child = exec("pwd", puts);

	res.sendfile('heatmap.html');
});
app.get('/streaming', function(req, res) {
	res.sendfile('streaming.html');
});
app.get('/addpoint', function(req, res) {
	if (req.query.x == null || req.query.y == null) return;
	heatmap_store.data.push({
		x: req.query.x,
		y: req.query.y
	});
	res.sendfile('heatmap.html');
});

// This is the callback required for our exec function below
var puts = function(error, stdout, stderr) {
	if (stdout !== null && stdout !== "")
		sys.print('stdout: ' + stdout);
	if (stderr !== null && stderr !== "")
		sys.print('stderr: ' + stderr);
	if (error !== null && error !== "") {
		console.log('exec error: ' + error);
	}
}

io.sockets.on('connection', function(socket) {
	socket.on('snapshot', function(data) {
		console.log("Taking a picture");
		child = exec("sh capture.sh", puts);
	});
	socket.on('send_command', function(data) {
		console.log("sending the command:" + data);
		// write_socket(data);
	});
	socket.on('send_coord', function(data) {
		console.log("Sending user selection");
		// write_socket(data);
	});
	socket.on('add_data_point', function(data){
		heatmap_store.data.push({
			x: data.x,
			y: data.y
		});
	});
	socket.on('heatmap_data', function(data) {
		console.log("sending heatmap data");
		socket.emit('data', heatmap_store);
	})
});
