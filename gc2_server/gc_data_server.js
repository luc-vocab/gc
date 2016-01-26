var influent = require('influent');
var util = require('util');
var config = require('./' + process.argv[2]);
var GcInfluxData = require('./gc_influx_data');
var GcDeviceMonitor = require('./gc_device_monitor');
var Firebase = require('firebase');

var winston = require('winston');
require('winston-papertrail').Papertrail;

require('ssl-root-cas/latest')
  .inject()
  .addFile(__dirname + '/isrgrootx1.pem')
  .addFile(__dirname + '/letsencryptauthorityx1.pem')
  .addFile(__dirname + '/lets-encrypt-x1-cross-signed.pem');

var papertrailTransport = new winston.transports.Papertrail({
        host: config.papertrailHost,
        port: config.papertrailPort,
        level: config.loggingLevel,
        program: 'gc_data_server'
    });

var consoleTransport = new winston.transports.Console({
        level: 'debug',
        timestamp: function() {
            return new Date().toString();
        },
        colorize: true
});

var logger = new winston.Logger({
transports: [
    papertrailTransport,
    consoleTransport
]
});

process.on('uncaughtException', (err) => {
    logger.error("unhandled exception:", err, err.stack);
});


influent
.createClient({
    username: config.influxUsername,
    password: config.influxPassword,
    database: config.influxDb,
    server: [
        {
            protocol: config.influxProtocol,
            host:     config.influxHost,
            port:     config.influxPort
        }
    ]
})
.then(function(client) {
    
    // subscribe to all clients that exist
    var firebase_root_ref = new Firebase(config.firebaseRoot);
    var devices_ref = firebase_root_ref.child('devices');
    
    
    devices_ref.on('child_added', function(childSnapshot, prevChildKey) {
        var device_id = childSnapshot.key();
        var data = childSnapshot.val();
        var device_name = data.device_name;
        var owner_uid = data.owner_uid;
        
        logger.info("subscribing to device_id", device_id);
        
        var influx_data = GcInfluxData(logger, client, config.firebaseRoot, device_name, owner_uid, device_id);
        influx_data.subscribe_device_node();
        
        var device_monitor = GcDeviceMonitor(logger, config.firebaseRoot, device_name, owner_uid, device_id);
        device_monitor.subscribe_device_node();
       
    });
    
    
}, function(err) {
    logger.error("could not create influxdb client:", err);
});
