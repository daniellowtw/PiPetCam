// var app = require('http').createServer(handler),
// 	io = require('socket.io').listen(app),
// 	fs = require('fs'),
// 	exec = require('child_process').exec,
// 	url = require('url');
// app.listen(8887);

var express = require('express')
	, sys = require('sys')
  , app = express()  
  , server = require('http').createServer(app)
  , path = require('path')
  , exec = require('child_process').exec
  , io = require('socket.io').listen(server)
  , spawn = require('child_process').spawn

// all environments
app.set('port', process.env.TEST_PORT || 8887);
app.use(express.favicon());
app.use(express.logger('dev'));
app.use(express.bodyParser());
app.use(express.methodOverride());
app.use(express.static(path.join(__dirname, '/')));
// app.use(omx());

app.get('/', function (req, res) {
  res.sendfile(__dirname + '/client.html');
});
// app.get('/style.css', function (req, res) {
//   res.sendfile(__dirname + '/style.css');
// });
app.get('/latest.jpg', function (req, res) {
  res.sendfile('/home/pi/Desktop/latest.jpg');
});


//Socket.io Config
io.set('log level', 1);

server.listen(app.get('port'), function(){
  console.log('Express server listening on port ' + app.get('port'));
});

// For the sake of simplicity, I've combined routing in this file

// function handler(req, res) {
// 	var pathname = url.parse(req.url).pathname;
// 	switch (pathname) {
// 		case "/":
// 			var file_name = '/index.html';
// 			break;
// 		case '/client':
// 		res.sendfile(__dirname + '/client.html');
// 			// var file_name = '/client.html';
// 			break;
// 		default:
// 			var file_name = '/client.html';
// 	}

// 	// fs.readFile(__dirname + file_name,
// 	// 	function(err, data) {
// 	// 		if (err) {
// 	// 			res.writeHead(500);
// 	// 			return res.end('Error loading index.html');
// 	// 		}
// 	// 		res.writeHead(200);
// 	// 		res.end(data);
// 	// 	});
// }

var puts = function(error, stdout, stderr) {
	if (stdout !== null && stdout !== "")
		sys.print('stdout: ' + stdout);
	if (stderr !== null && stderr !== "")
		sys.print('stderr: ' + stderr);
	if (error !== null && error !== "") {
		console.log('exec error: ' + error);
	}
}


function route(socket, result, pathname) {

		switch (pathname) {
			case "testcam":
				console.log("started testcam");
				child = exec("export DISPLAY=:0.0;../raspicam_pet/build/pet", puts);
				break;
			case "reboot":
				console.log("rebooting");
				child = exec("sudo reboot", puts);
				break;
			case "shutdown":
				console.log("rebooting");
				child = exec("sudo reboot", puts);
				break;
			case "stats":
				console.log("stats");
				run_shell("./stats.sh",[], function (me, buffer) {
            		result.data += buffer.toString();
            		result.target = "stats";
         		},
		        function () {
		        	console.log ("new" + result);
					socket.emit('received foo', result);
		        });
				break;
			case "snapshot":
				child = exec("./capture.sh", puts);
				console.log("Taking a picture");
				break;
			case "/display_last":
				console.log("displaying a picture");
				child = exec("export DISPLAY=:0.0;feh ~/Desktop/latest.jpg", puts);
				break;
			default:
				console.log("unknown " + pathname);
		}
	}

	//socket io stuff
io.sockets.on('connection', function(socket) {
	socket.on('clicked foo', function(data) {
		console.log(data.action);
		var result = {};
		result.result = "okay";
		var test = route(socket, result, data.action);
		// foo_action(data.action);
		// console.log(result); // TODO fix this. This is currently async
	});
});

// Respond based on action

function foo_action(action) {
	switch (action) {
		case "testcam":
			confirm("are you sure?");
			console.log("started testcam");
			child = exec("export DISPLAY=:0.0;../raspicam_pet/build/pet", puts);
			break;
	}

}

//Run and pipe shell script output

function run_shell(cmd, args, cb, end) {
	var spawn = require('child_process').spawn,
		child = spawn(cmd, args),
		me = this;
	child.stdout.on('data', function(buffer) {
		cb(me, buffer);
	});
	child.stdout.on('end', end);
}