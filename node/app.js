var port = process.env.PORT || 80,
	http = require('http'),
    fs = require('fs'),
    qs = require('querystring'),
    html = fs.readFileSync(__dirname + '/index.html');
var gParams = null;
var tempParams = null;

// Send index.html to all requests
var server = http.createServer(function(req, res) {
	if (req.method === 'GET') {
 		if (req.url.indexOf('?') >= 0) {
        	tempParams = qs.parse(req.url.replace(/^.*\?/, ''));
		}

        if(tempParams === null) {
            res.writeHead(200);
            res.write(html);
            res.end();
        } else {
            gParams = tempParams;
            tempParams = null;
                    var body = '';

            req.on('data', function(chunk) {
                body += chunk;
            });

            req.on('end', function() {
                res.writeHead(200, 'OK', {'Content-Type': 'text/plain'});
                res.end();
            });
        }
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

server.listen(port);
