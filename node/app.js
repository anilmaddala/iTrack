var port = process.env.PORT || 80,
	http = require('http'),
    fs = require('fs'),
    qs = require('querystring'),
    html = fs.readFileSync(__dirname + '/index.html');
var gParams = null;

// Send index.html to all requests
var server = http.createServer(function(req, res) {
	if (req.method === 'POST') {
 		if (req.url.indexOf('?') >= 0) {
        	gParams = qs.parse(req.url.replace(/^.*\?/, ''));
		}
	} else {
        	res.writeHead(200);
        	res.write(html);
        	res.end();
    }
});

// Socket.io server listens to our server
var io = require('socket.io').listen(server);

// Send current time to all connected clients
function sendLocation() {
    io.emit('location', { location: gParams });
}

// Send location 1 sec
setInterval(sendLocation, 1000);

// Emit welcome message on connection
io.on('connection', function(socket) {
    // Use socket to communicate with this particular client only, sending it it's own id
    socket.emit('welcome', { message: 'Welcome!', id: socket.id });

    // socket.on('i am client', console.log);
});

server.listen(port);
