var influent = require('influent');
var influx_config = require('./influx_config.js');
var net = require('net');


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
        self.log("received data");
    
        if(self.state == CONNECTION_STATES.CONNECTED) {
            // console.log("received data");
            var device_id = data.readInt32BE(0);
            var protocol_version = data.readInt32BE(4);
            self.log("device_id: ", device_id, " protocol_version: ", protocol_version);
            
            var mode = data.readInt32BE(8);
            
            if( mode == MODE.REALTIME ) {
                this.state = CONNECTION_STATES.READY_REALTIME;
                self.log("realtime mode");
            } else if ( mode == MODE.DATALOGGING ) {
                this.state == CONNECTION_STATES.READY_DATALOGGING
                self.log("datalogging mode");
            }
           
        } else if (self.state == CONNECTION_STATES.READY_REALTIME) {
            
            
            
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

