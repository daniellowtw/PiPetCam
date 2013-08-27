# Chapter 3: Creating a server

Up till now our program runs locally but requires the user to attach a monitor, mouse, and keyboard to use the program. We want to remove this restriction. This can be done by serving a server from the raspberry pi, and allow users to connect to it using other devices. 

## Setup

For this tutorial, we will be using nodejs to run a web server. One nice feature of nodejs is its package manager. Just like apt-get, npm is the package manager for nodejs. To install nodejs and npm, do `sudo apt-get install nodejs npm`.

## Theory

The socket is the channel of the communication between any two programs. The socket contains a IP address and a port number.

## Step 1: Creating our server

While it is possible to create a server without additional addons, we want to make it easier by using other libraries used for such purposes. To find out more about a particular module, we can look at the documentations found at [here][node API] Create a javascript file called server.js in /home/pi/server

	// Declaring our variables
	var express = require('express'),
		sys = require('sys'),
		app = express(),
		server = require('http').createServer(app),
		path = require('path'),
		exec = require('child_process').exec,
		io = require('socket.io').listen(server),
		spawn = require('child_process').spawn

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

Now we create a simple html page called `main.html`

	<html>
	<head>
		<title></title>
	</head>
	<body>
		Hello world
	</body>
	</html>

We start our server by typing node `server.js` in our terminal. If we launch a browser and go to the ip of our raspberry pi, we should see the familiar `Hello world`.

## Step 2: Remote controlling

Now that we have a server running, we want to be able to use it to activate some functionalities of the pi. We need to communicate between the browser and the server. To do this, we use the extension [socket.io][socketio]. Socket.io is event driven. To use it, we set up listeners and then send events from one page to another. Let's start by setting up a listener on the server and sending a reply when we received an event.

~! A good way to debug the program is to include `console.log(VARIALBE)` to see what the variable is in the console !~

	//socket io stuff
	io.sockets.on('connection', function(socket) {
		socket.on('my event', function(data) {
			socket.emit('my reply', {reply: "This is my reply"});
		});
	});

In our `main.html` we need to set up the listener as well.

	<html>
		<head>
			<title></title>
			<script src="/socket.io/socket.io.js" type="text/javascript"></script>
		</head>
		<body>
			<button onclick="foo()">Press me</button>
			<script type="text/javascript">
				var socket = io.connect('http://192.168.137.179:8887');

				function foo(){
					socket.emit('my event');
					socket.on('my reply', function(data){
						alert(data.reply);
					})
				}
			</script>
		</body>
	</html>

## Step 3: Taking remote snapshots

Now let's try to take a picture using the browser and display it.

We do this by launching the raspistill program from the server. This can be done by executing a the raspistill command with the appropriate arguments.

Create a file called capture.sh that contains

	raspistill -t 0 -o latest.jpg -n -vf &

This script will take a picture and save it which we can then load on the browser.

~! Make sure to set it as executable with `chmod +x capture.sh` !~

Now in our server, we create a new page for this and add a router to load that page. We also need to add a function to execute our capture.sh script

In server.js
	
	app.get('/snapshot', function (req, res) {
	  res.sendfile('snapshot.html');
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
			child = exec("./capture.sh", puts);
			console.log("Taking a picture");
		});
	});

Create a file called `snapshot.html`

	<html>
		<head>
			<title></title>
			<script src="/socket.io/socket.io.js" type="text/javascript"></script>
		</head>
		<body>
			<button onclick="capture()">Take a picture</button>
			<button onclick="update_picture()">Load last picture taken</button>
			</br>
			<canvas id="myCanvas" width="540" height="360"></canvas> 

			<script type="text/javascript">
				var socket = io.connect('http://192.168.137.104:8887');
				var canvas = document.getElementById('myCanvas');
				var context = canvas.getContext('2d');
				var imageObj = new Image();

				function capture(){
					socket.emit('snapshot');
				}
				function update_picture(){
					imageObj.src = 'latest.jpg';
					imageObj.onload = display;
				}
				function display(){
					context.drawImage(imageObj, 0, 0, canvas.width, canvas.height);
				}
			</script>
		</body>
	</html>

[node API]:http://nodejs.org/api/
[socketio]:http://socket.io