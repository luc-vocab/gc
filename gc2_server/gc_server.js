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
            // read number of datapoints
            var num_datapoints = data.readUInt16LE(offset); offset += 2;
            // read total buffer size to expect
            var buffer_size = data.readUInt32LE(offset); offset += 4;
            
            console.log("datalogging header", new Date(),
                        "charged_percent:", charged_percent / 100.0,
                        "num_datapoints:", num_datapoints,
                        "buffer_size:", buffer_size);
                        
            self.expect_buffer_size = buffer_size;
            self.data_buffer = new Buffer(buffer_size);
            self.data_buffer_offset = 0;
            self.num_datapoints = num_datapoints;
            self.state = CONNECTION_STATES.PENDING_DATALOGGING;

            if( data_length > offset ) {
                // more data is available than what we read in the buffer
                data.copy(self.data_buffer, self.data_buffer_offset, offset);
                self.data_buffer_offset += data_length - offset;
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
            self.read_data_packet(data, offset);
        }
    });
    
    this.process_datalogging_buffer = function() {
    
        var offset = 0;
        for(var i = 0; i < self.num_datapoints; i++) {
            offset = self.read_data_packet(self.data_buffer, offset, false);
        }
        
        var final_byte = new Buffer(1);
        final_byte.writeUInt8(1,0);
        self.socket.write(final_byte);
        
        console.log("processed datalogging buffer");
    }
    
    this.read_data_packet = function(data, offset, print_data) {
        var timestamp = data.readUInt32LE(offset); offset += 4;
        var milliseconds = data.readUInt16LE(offset); offset += 2;
        
        // read EMG value
        var emg_value = data.readUInt16LE(offset); ; offset += 2;
        
        // read gyro max
        var gyro_max = data.readFloatLE(offset); offset += 4;
        // read accel values
        var accel_x = data.readFloatLE(offset); offset += 4;
        var accel_y = data.readFloatLE(offset); offset += 4;
        var accel_z = data.readFloatLE(offset); offset += 4;
        
        if(print_data) {
            console.log("timestamp: ", timestamp,
                        " milliseconds: ", milliseconds,
                        " emg_value: ", emg_value,
                        " gyro_max: ", gyro_max,
                        " accel_x: ", accel_x,
                        " accel_y: ", accel_y,
                        " accel_z: ", accel_z);    
        }

        /*
        pubnub_client.publish({
            channel: "sleep-track-data-luc",
            message: {"emg_value": value}
        });
        */                    
                    
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

