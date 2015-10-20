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
};

function GcClient(socket) {
    this.socket = socket;

    this.state = CONNECTION_STATES.CONNECTED;
    
    var self = this;
    
    this.socket.on('data', function(data){
        self.log("received data, length: " + data.length);
    
        if(self.state == CONNECTION_STATES.CONNECTED) {
            // console.log("received data");
            var device_id = data.readInt32BE(0);
            var protocol_version = data.readInt32BE(4);
            self.log("device_id: " + device_id + " protocol_version: " + protocol_version);
            
            var mode = data.readInt32BE(8);
            
            if( mode == MODE.REALTIME ) {
                self.state = CONNECTION_STATES.READY_REALTIME;
                self.log("realtime mode");
            } else if ( mode == MODE.DATALOGGING ) {
                self.state == CONNECTION_STATES.READY_DATALOGGING
                self.log("datalogging mode");
            }
           
        } else if (self.state == CONNECTION_STATES.READY_REALTIME) {
            var value = data.readInt32BE(0);
            self.log("received value " + value);
            pubnub_client.publish({
                channel: "sleep-track-data-luc",
                message: {"emg_value": value}
            });
        }
    });
    
    this.socket.on('close', function(data) {
        console.log("connection closed");
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

