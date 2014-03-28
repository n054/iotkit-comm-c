var edison = require('../edison-lib');
var path = require('path');
var fs = require('fs');

var serviceSpec = JSON.parse(fs.readFileSync(path.join(edison.config.libRoot, edison.config.serviceDir, "audioService.json")));

edison.localDirectory.discoverServices(serviceSpec.type, function(service) {
	console.log("found " + service.type.name + " service at " + service.addresses[service.addresses.length-1] + ":" + service.port);
});
