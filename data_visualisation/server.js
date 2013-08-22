var http = require("http");
var sys = require('sys');
var exec = require('child_process').exec;
var url = require('url');

var puts = function (error, stdout, stderr) {
  if (stdout !== null && stdout !== "") 
  	sys.print('stdout: ' + stdout);
  if (stderr !== null && stderr !== "") 
	sys.print('stderr: ' + stderr);
  if (error !== null && error !== "") {
    console.log('exec error: ' + error);
  }
}


function route(pathname) {

	switch (pathname) {
		case "/testcam":
		  	console.log("started testcam");
			child = exec("export DISPLAY=:0.0;../raspicam_pet/build/pet", puts);
			break;
		case "/reboot":
			console.log("rebooting");
			child = exec("sudo reboot", puts);
			break;
		case "/stats":
			console.log("rebooting");
			child = exec("./stats.sh", puts);
			break;
		case "/snapshot":
			console.log("Taking a picture");
			child = exec("raspistill -t 0 -o ~/Desktop/latest.jpg", puts);
			break;
		case "/display_last":
			console.log("displaying a picture");
			child = exec("export DISPLAY=:0.0;feh ~/Desktop/latest.jpg", puts);
			break;
		default:
  			console.log("unknown " + pathname);		
	}
}


child = exec("export DISPLAY=:0.0", puts);

http.createServer(function(request, response) {
	var pathname = url.parse(request.url).pathname;
	route(pathname);
  response.writeHead(200, {"Content-Type": "text/plain"});
  response.write("Hello World !!");
  response.end();
}).listen(8888);
