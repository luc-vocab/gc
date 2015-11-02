var influent = require('influent');
var pubnub = require("pubnub");
var influx_config = require('./influx_config.js');
var net = require('net');




pubnub_client = pubnub({
    publish_key: "pub-c-879cf9bb-46af-4bf1-8dca-e011ea412cd2",
    subscribe_key: "sub-c-cba703c8-7b42-11e3-9cac-02ee2ddab7fe"
});

MODE = {
    REALTIME: 1,
    DATALOGGING: 2,
};

CONNECTION_STATES = {
    CONNECTED: 0,
    READY_REALTIME: 1,
    READY_DATALOGGING: 2,
    PENDING_DATALOGGING: 3 // some more batch data is expected
};

UINT16_MARKER_START = 6713;
UINT16_MARKER_END = 21826;

function GcClient(socket) {
    this.socket = socket;

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
            
            var mode = data.readUInt32LE(8);
            
            if( mode == MODE.REALTIME ) {
                self.state = CONNECTION_STATES.READY_REALTIME;
                self.log("realtime mode");
            } else if ( mode == MODE.DATALOGGING ) {
                self.state = CONNECTION_STATES.READY_DATALOGGING
                self.log("datalogging mode");
            }
        
        } else if (self.state == CONNECTION_STATES.READY_DATALOGGING) {
        
            offset = 0;

            // read battery charge
            var charged_percent = data.readUInt16LE(offset); offset += 2;
            
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
            self.read_data_packet(data, offset, true, true);
        }
    });
    
    this.process_datalogging_buffer = function() {
    
        var offset = 0;
        for(var i = 0; i < self.num_datapoints; i++) {
            offset = self.read_data_packet(self.data_buffer, offset, false, false);
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
    }
    
    this.read_data_packet = function(data, offset, print_data, publish) {
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
            pubnub_client.publish({
                channel: "sleep-track-data-luc",
                message: {"emg_value": emg_value,
                          "gyro_max": gyro_max,
                          "accel_x": accel_x,
                          "accel_y": accel_y,
                          "accel_z": accel_z}
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

var server = net.createServer(function(socket) {
    console.log('received connection');
    // socket.write('hello from server\r\n');
    
    var gcClient = new GcClient(socket);
});



server.listen(7001, '0.0.0.0');

