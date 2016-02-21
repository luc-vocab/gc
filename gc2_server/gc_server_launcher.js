var influent = require('influent');
var net = require('net');
var Firebase = require('firebase');
var q = require('promised-io/promise');
var winston = require('winston');
var GcClient = require('./gc_server_connection');
var GcConfig = require('./gc_config');
var GcInfluxData = require('./gc_influx_data');
var GcDeviceMonitor = require('./gc_device_monitor');
var GcConfig = require('./gc_config');
require('winston-papertrail').Papertrail;

require('ssl-root-cas/latest')
    .inject()
    .addFile(__dirname + '/isrgrootx1.pem')
    .addFile(__dirname + '/letsencryptauthorityx1.pem')
    .addFile(__dirname + '/lets-encrypt-x1-cross-signed.pem');

var server_type = process.env.SERVER_TYPE;
if(!server_type) {
    throw "SERVER_TYPE not set";
}


GcConfig(server_type).then(function(config_data) {
    console.log("config_data:", config_data);

    var config = config_data.config;
    var client = config_data.influx_client;
    var logger = config_data.logger;

    
    if(server_type == "gc_server") {

        var server_config = config_data.server_config;
    
        var server = net.createServer(function(socket) {
    
            logger.info(socket.remoteAddress, ':', socket.remotePort, "received connection");
            var gcClient = new GcClient(socket, client, config, config_data.firebase_root, logger);
        });
    
        var server_key = config_data.server_key;
    
        logger.info("server", server_key, "listening on", server_config.port);
        server.listen(server_config.port, '0.0.0.0');
    
        // mark server online
        var server_ref = new Firebase(config_data.firebase_root).child('servers').child(server_key);
        server_ref.update({
            online: true
        });
        var presenceRef = server_ref.child("online");
        presenceRef.onDisconnect().set(false);


    } else if (server_type == "gc_data_server") {
        
        // subscribe to all clients that exist
        var firebase_root = config_data.firebase_root;
        var firebase_root_ref = new Firebase(firebase_root);
        var devices_ref = firebase_root_ref.child('devices');
        
        
        devices_ref.on('child_added', function(childSnapshot, prevChildKey) {
            var device_id = childSnapshot.key();
            var data = childSnapshot.val();
            var device_name = data.device_name;
            var owner_uid = data.owner_uid;
            
            logger.info("subscribing to device_id", device_id);
            
            var influx_data = new GcInfluxData(logger, client, firebase_root, device_name, owner_uid, device_id);
            influx_data.subscribe_device_node();
            
            var device_monitor = GcDeviceMonitor(logger, firebase_root, device_name, owner_uid, device_id);
            device_monitor.subscribe_device_node();
           
        });

        
    }
});