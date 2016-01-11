var influent = require('influent');
var util = require('util');
var config = require('./' + process.argv[2]);
var GcInfluxData = require('./gc_influx_data');
var Firebase = require('firebase');


influent
.createClient({
    username: config.influxUsername,
    password: config.influxPassword,
    database: config.influxDb,
    server: [
        {
            protocol: "http",
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
        console.log('child');
        
        var device_id = childSnapshot.key();
        var data = childSnapshot.val();
        var device_name = data.device_name;
        var owner_uid = data.owner_uid;
        
        console.log("subscribing to device_id", device_id);
        
        var influx_data = GcInfluxData(client, config.firebaseRoot, device_name, owner_uid, device_id);
        influx_data.subscribe_device_node();
       
    });
    
    
});
