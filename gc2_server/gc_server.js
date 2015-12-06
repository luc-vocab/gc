var influent = require('influent');
var pubnub = require("pubnub");
var net = require('net');
var Firebase = require('firebase');
var config = require('./' + process.argv[2]);


/* global pubnub_client: true */
/* global MODE: true */
/* global CONNECTION_STATES: true */
/* global UINT16_MARKER_START: true */
/* global UINT16_MARKER_END: true */

pubnub_client = pubnub({
    publish_key: "pub-c-879cf9bb-46af-4bf1-8dca-e011ea412cd2",
    subscribe_key: "sub-c-cba703c8-7b42-11e3-9cac-02ee2ddab7fe"
});

MODE = {
    REALTIME: 1,
    DATALOGGING: 2,
    CONNECTION_TEST: 3
};

CONNECTION_STATES = {
    CONNECTED: 0,
    READY_REALTIME: 1,
    READY_DATALOGGING: 2,
    PENDING_DATALOGGING: 3 // some more batch data is expected
};

UINT16_MARKER_START = 6713;
UINT16_MARKER_END = 21826;

function GcClient(socket, influx_client, config) {
    this.socket = socket;
    this.influx_client = influx_client;
    this.config = config;
    
    // firebase setup
    this.firebaseRoot = new Firebase(config.firebaseRoot);
    this.firebaseDevicesRoot = this.firebaseRoot.child('devices');

    this.state = CONNECTION_STATES.CONNECTED;
    
    var self = this;
    
    this.socket.on('data', function(data){
        var data_length = data.length;
        console.log("received data, length: ", data_length, " current state: ", self.state);
    
        if(self.state == CONNECTION_STATES.CONNECTED) {
            // console.log("received data");
            var device_id = data.readUInt32LE(0);
            var protocol_version = data.readUInt32LE(4);
            self.log("device_id: " + device_id + " protocol_version: " + protocol_version);
            
            self.firebaseDeviceRef = self.firebaseDevicesRoot.child(device_id.toString());
            // get the user_name (to be added as an influxdb tag)
            self.firebaseDeviceRef.once('value', function(snapshot){
               var data = snapshot.val();
               self.user_name = data.user_name;
            });
            
            var mode = data.readUInt32LE(8);
            
            if( mode == MODE.REALTIME ) {
                self.state = CONNECTION_STATES.READY_REALTIME;
                self.log("realtime mode");
            } else if ( mode == MODE.DATALOGGING ) {
                self.state = CONNECTION_STATES.READY_DATALOGGING;
                self.log("datalogging mode");
            } else if ( mode == MODE.CONNECTION_TEST ) {
                self.log("connection test");
                var random_number = data.readUInt32LE(12);
                // write data on firebase to show we received data
                self.firebaseDeviceRef.update({
                    "ping_test": random_number
                });
            }
        
        } else if (self.state == CONNECTION_STATES.READY_DATALOGGING) {
        
            var offset = 0;

            // read battery charge
            var charged_percent = data.readUInt16LE(offset); offset += 2;
            
            // read number of seconds collected
            var collected_duration = data.readUInt32LE(offset); offset += 4;
            
            // read starting timestamp
            var starting_timestamp = data.readUInt32LE(offset); offset += 4;
            // read starting millis
            var starting_millis = data.readUInt32LE(offset); offset += 4;
            
            self.starting_timestamp = starting_timestamp * 1000 + starting_millis % 1000;
            self.starting_millis = starting_millis;
            
            // read stats about data uploaded
            var batches_uploaded = data.readUInt16LE(offset); offset += 2;
            var error_count = data.readUInt16LE(offset); offset += 2;
            var abandon_count = data.readUInt16LE(offset); offset += 2;

            // update firebase with a few stats
            self.firebaseDeviceRef.update({
                "battery_charge": charged_percent / 100.0,
                "batches_uploaded": batches_uploaded,
                "error_count": error_count,
                "abandon_count": abandon_count,
                "last_upload_time": Firebase.ServerValue.TIMESTAMP,
                "collected_duration": collected_duration,
                "mode": "night"
            });            

            
            // read number of datapoints
            var num_datapoints = data.readUInt16LE(offset); offset += 2;
            // read total buffer size to expect
            var buffer_size = data.readUInt32LE(offset); offset += 4;
            // read starting marker
            var starting_marker = data.readUInt16LE(offset); offset += 2;
            
            if(starting_marker != UINT16_MARKER_START) {
                console.log("ERROR starting marker incorrect");
            }

            self.expect_buffer_size = buffer_size - offset;
            self.data_buffer = new Buffer(buffer_size);
            self.data_buffer_offset = 0;
            self.num_datapoints = num_datapoints;
            self.state = CONNECTION_STATES.PENDING_DATALOGGING;
            
            console.log("datalogging header", new Date(),
                        "charged_percent:", charged_percent / 100.0,
                        "num_datapoints:", num_datapoints,
                        "total buffer_size:", buffer_size,
                        "expect buffer_size:", self.expect_buffer_size);
            
            console.log("stats:", "batches: ", batches_uploaded,
                                  "errors: ", error_count,
                                  "abandons: ", abandon_count);
            
            if( data_length > offset || data_length == buffer_size) {
                // more data is available than what we read in the buffer
                data.copy(self.data_buffer, self.data_buffer_offset, offset);
                self.data_buffer_offset += data_length - offset;
            } 
            
            if (data_length == buffer_size) {
                // all done
                console.log("ready to process buffer");
                self.process_datalogging_buffer();
                self.state = CONNECTION_STATES.READY_DATALOGGING;                
            }
            
        } else if (self.state == CONNECTION_STATES.PENDING_DATALOGGING) {
        
            data.copy(self.data_buffer, self.data_buffer_offset);
            self.data_buffer_offset += data_length;
            
            console.log("appended to data_buffer: ", data_length,
                        "data_buffer_offset:", self.data_buffer_offset);
            
            if(self.data_buffer_offset == self.expect_buffer_size) {
                // received full data size
                console.log("ready to process buffer");
                
                self.process_datalogging_buffer();
                
                self.state = CONNECTION_STATES.READY_DATALOGGING;
            }
        
        } else if (self.state == CONNECTION_STATES.READY_REALTIME) {
            offset = 0;
            self.read_data_packet(data, offset, true, true, false);
        }
    });
    
    this.process_datalogging_buffer = function() {
        
        // reset the measurements array
        self.measurements = [];
    
        var offset = 0;
        for(var i = 0; i < self.num_datapoints; i++) {
            offset = self.read_data_packet(self.data_buffer, offset, false, false, true);
        }
        
        // read end marker
        var end_marker = self.data_buffer.readUInt16LE(offset); offset += 2;
        if( end_marker != UINT16_MARKER_END ) {
            console.log("ERROR invalid end marker");
        }
        
        
        var final_byte = new Buffer(1);
        final_byte.writeUInt8(1,0);
        self.socket.write(final_byte);
        
        console.log("processed datalogging buffer");
        console.log("writing to influxdb");
        
        self.influx_client.writeMany(self.measurements).then(function() {
            console.log("done writing to influxDB");
        });        
    }
    
    this.read_data_packet = function(data, offset, print_data, publish, push_to_influxdb) {
        var milliseconds = data.readUInt32LE(offset); offset += 4;
        
        var diff = milliseconds - self.starting_millis;
        var timestamp = self.starting_timestamp + diff;
        var datetime = new Date(timestamp);
        
        // read EMG value
        var emg_value = data.readUInt16LE(offset); ; offset += 2;
        
        // read gyro max
        var gyro_max_adj = data.readInt16LE(offset); offset += 2;
        // read accel values
        var accel_x_adj = data.readInt16LE(offset); offset += 2;
        var accel_y_adj = data.readInt16LE(offset); offset += 2;
        var accel_z_adj = data.readInt16LE(offset); offset += 2;
        
        var gyro_max = gyro_max_adj / 100.0;
        var accel_x = accel_x_adj / 10000.0;
        var accel_y = accel_y_adj / 10000.0;
        var accel_z = accel_z_adj / 10000.0;
        
        if(print_data) {
            console.log("time: ", datetime,
                        " millisecond diff: ", diff,
                        " emg_value: ", emg_value,
                        " gyro_max: ", gyro_max,
                        " accel_x: ", accel_x,
                        " accel_y: ", accel_y,
                        " accel_z: ", accel_z);    
        }

        if(publish) {
        
            // publish to firebase
            this.firebaseDeviceRef.update({
                "emg_value": emg_value,
                "gyro_max": gyro_max,
                "accel_x": accel_x,
                "accel_y": accel_y,
                "accel_z": accel_z
            });
        
        }

        var tags = {user: self.user_name,
                    env:  self.config.environment};
        
        if(push_to_influxdb) {
            var timestamp_nanos = datetime.getTime().toString() + "000000";
            // EMG sensor value
            self.measurements.push({
                key: "emg",
                tags: tags,
                fields: {
                    emg_value: new influent.Value(emg_value, influent.type.INT64),
                },
                timestamp: timestamp_nanos
            });
            self.measurements.push({
                key: "imu",
                tags: tags,
                fields: {
                    gyro: new influent.Value(gyro_max, influent.type.FLOAT64),
                    accel_x: new influent.Value(accel_x, influent.type.FLOAT64),
                    accel_y: new influent.Value(accel_y, influent.type.FLOAT64),
                    accel_z: new influent.Value(accel_z, influent.type.FLOAT64),
                },
                timestamp: timestamp_nanos
            });
        }

        return offset;
    };
    
    this.socket.on('close', function(data) {
        console.log("connection closed");
    });
    
    this.socket.on('error', function(error) {
        console.log("error:", error);
    });
    
    this.log = function(log_entry) {
        console.log(log_entry);
    };
    
    
}

influent
.createClient({
    username: "dev",
    password: "dev",
    database: "gc_dev",
    server: [
        {
            protocol: "http",
            host:     "influxdb.dev.sleeptrack.io",
            port:     8086
        }
    ]
})
.then(function(client) {
    var server = net.createServer(function(socket) {
        console.log('received connection');
        
        var gcClient = new GcClient(socket, client, config);
    });
    server.listen(7001, '0.0.0.0');
    
    // mark server online
    var firebaseRoot = new Firebase(config.firebaseRoot);
    var serverRef = firebaseRoot.child('servers').child(config.serverKey);
    serverRef.update({
        online: true
    });
    var presenceRef = serverRef.child("online");
    presenceRef.onDisconnect().set(false);
    
});



