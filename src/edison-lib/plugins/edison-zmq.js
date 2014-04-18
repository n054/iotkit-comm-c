var zeromq = require('zmq');
var socket;

function ZeroMQ(port, type, ip) {
    "use strict";
    if (type == 'pub') {
        socket = zeromq.socket('pub');
        socket.bindSync('tcp://*:' + port);
    } else {
        socket = zeromq.socket('sub');
        socket.connect('tcp://' + ip + ':' + port);
    }
}

ZeroMQ.prototype.component = "communication";
ZeroMQ.prototype.name = "zmqpubsub";

ZeroMQ.prototype.send = function (topic, message) {
    socket.send(topic + ' ' + message);
};

ZeroMQ.prototype.subscribe = function (topic) {
    socket.subscribe(topic);
};

ZeroMQ.prototype.setReceivedMessageHandler = function (handler) {
  "use strict";

  socket.on('message', function (message) {
    handler(message);
  });
}

ZeroMQ.prototype.unsubscribe = function () {
  "use strict";
};

ZeroMQ.prototype.done = function () {
    socket.close();
};

module.exports = ZeroMQ;