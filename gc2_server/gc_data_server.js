var influent = require('influent');
var util = require('util');
var GcInfluxData = require('./gc_influx_data');
var GcDeviceMonitor = require('./gc_device_monitor');
var GcConfig = require('./gc_config');
var Firebase = require('firebase');

var winston = require('winston');
require('winston-papertrail').Papertrail;

require('ssl-root-cas/latest')
  .inject()
  .addFile(__dirname + '/isrgrootx1.pem')
  .addFile(__dirname + '/letsencryptauthorityx1.pem')
  .addFile(__dirname + '/lets-encrypt-x1-cross-signed.pem');


GcConfig("gc_data_server").then(function(config_data) {
    console.log("config_data:", config_data);

    var config = config_data.config;
    var client = config_data.influx_client;
    var logger = config_data.logger;

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
        
        var influx_data = GcInfluxData(logger, client, firebase_root, device_name, owner_uid, device_id);
        influx_data.subscribe_device_node();
        
        var device_monitor = GcDeviceMonitor(logger, firebase_root, device_name, owner_uid, device_id);
        device_monitor.subscribe_device_node();
       
    });
    
    
});