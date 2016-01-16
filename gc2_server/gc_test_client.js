
/* global MODE: true */
/* global UINT16_MARKER_HANDSHAKE: true */
/* global UINT16_MARKER_START: true */
/* global UINT16_MARKER_END: true */
/* global NUM_ITERATIONS: true */
/* global NUM_DATAPOINTS: true */
/* global DATALOGGING_HEADER_SIZE: true */
/* global DATALOGGING_FOOTER_SIZE: true */


MODE = {
    REALTIME: 1,
    DATALOGGING: 2,
    CONNECTION_TEST: 3
};

UINT16_MARKER_HANDSHAKE = 39780;
UINT16_MARKER_START = 6713;
UINT16_MARKER_END = 21826;

// full night setttings
// NUM_ITERATIONS = 32;
// NUM_DATAPOINTS = 3600;
// log every 1mn settings
NUM_ITERATIONS = 5;
NUM_DATAPOINTS = 240;

DATALOGGING_HEADER_SIZE = 28;
DATALOGGING_FOOTER_SIZE = 2;

var net = require('net');
var async = require('async');
var q = require('promised-io/promise');


var add_datapoint = function(starting_timestamp_full, data_buffer, 
                             data_buffer_offset) {
            
    // console.log("writing datapoint");
            
    // write millis
    var current_millis = new Date().getTime() - starting_timestamp_full;
    data_buffer.writeUInt32LE(current_millis, data_buffer_offset); data_buffer_offset += 4;
    
    // write EMG value
    // randomly generate EMG value
    
    var emg_value = Math.floor((Math.random() * 10) + 1) + 100;
    if(current_millis % 60000 < 5000) {
        // during the 5s  of every 60s interval, use higher value
        emg_value = Math.floor((Math.random() * 10) + 1) + 300;
    }
    
    data_buffer.writeUInt16LE(emg_value, data_buffer_offset); data_buffer_offset += 2;
    
    // write gyro and accel values
    data_buffer.writeInt16LE(0, data_buffer_offset); data_buffer_offset += 2;
    data_buffer.writeInt16LE(0, data_buffer_offset); data_buffer_offset += 2;
    data_buffer.writeInt16LE(0, data_buffer_offset); data_buffer_offset += 2;
    data_buffer.writeInt16LE(0, data_buffer_offset); data_buffer_offset += 2;            

    return data_buffer_offset;    
}

var do_iteration = function(context) {
    var iteration_finished_defered = q.defer();

    var num_datapoints = 0;
    var data_buffer_offset = 0;
    var data_buffer_max_size = NUM_DATAPOINTS * 14;
    var data_buffer = new Buffer(data_buffer_max_size);
    
    var starting_timestamp = new Date().getTime() / 1000;
    var starting_millis = new Date().getTime() - context.starting_timestamp_full;
    
    var buffer_ready_defer = q.defer();
    
    var repeat = setInterval(function() {
        
        data_buffer_offset = add_datapoint(context.starting_timestamp_full,
                                data_buffer, data_buffer_offset);
        num_datapoints++;
        if (data_buffer_offset == data_buffer_max_size) {
            // filled up data buffer
            clearInterval(repeat);
            // release promise
            buffer_ready_defer.resolve();
            
            console.log("done collecting datapoints");
        }
    }, 250);


    buffer_ready_defer.promise.then(function() {
        console.log("writing data buffer");

        // start handshake buffer
        var handshake_buffer = new Buffer(DATALOGGING_HEADER_SIZE);
        var handshake_offset = 0;
        
        // write battery charge
        handshake_buffer.writeUInt16LE(10000, handshake_offset); handshake_offset += 2;
        // write collection start timestamp
        handshake_buffer.writeUInt32LE(context.collection_start_timestamp, handshake_offset); handshake_offset += 4;
        // write starting timestamp
        handshake_buffer.writeUInt32LE(starting_timestamp, handshake_offset); handshake_offset += 4;
        // write starting millis
        handshake_buffer.writeUInt32LE(starting_millis,  handshake_offset); handshake_offset += 4;
        // write batch information
        handshake_buffer.writeUInt16LE(context.batches_uploaded, handshake_offset); handshake_offset += 2;
        handshake_buffer.writeUInt16LE(0, handshake_offset); handshake_offset += 2;
        handshake_buffer.writeUInt16LE(0, handshake_offset); handshake_offset += 2;
        // write buffer information
        handshake_buffer.writeUInt16LE(num_datapoints, handshake_offset); handshake_offset += 2;
        handshake_buffer.writeUInt32LE(data_buffer_offset + DATALOGGING_HEADER_SIZE + DATALOGGING_FOOTER_SIZE, handshake_offset); handshake_offset += 4;
        // write starting marker
        handshake_buffer.writeUInt16LE(UINT16_MARKER_START, handshake_offset); handshake_offset += 2;
        
        // write out handshake buffer
        client.write(handshake_buffer);
        // write out full data buffer
        client.write(data_buffer);
        
        // write end header
        var end_buffer = new Buffer(DATALOGGING_FOOTER_SIZE);
        end_buffer.writeUInt16LE(UINT16_MARKER_END, 0);
        client.write(end_buffer);            
        
        context.batches_uploaded++;
        
        // all done
        iteration_finished_defered.resolve();
    });
    
    return iteration_finished_defered.promise;
    
}


var client = new net.Socket();
client.connect(7001, '127.0.0.1', function() {
    console.log('connected to server');
    
    // create buffer
    var buffer = new Buffer(14);
    
    var offset = 0;
    // write UINT16_MARKER_HANDSHAKE
    buffer.writeUInt16LE(UINT16_MARKER_HANDSHAKE, offset); offset += 2;
    // write device id (932326611)
    buffer.writeInt32LE(932326611, offset); offset += 4;
    // write protocol version
    buffer.writeInt32LE(100, offset); offset += 4;
    // write realtime mode
    buffer.writeInt32LE(MODE.DATALOGGING, offset); offset += 4;
    
    client.write(buffer);
    console.log("wrote handshake buffer");
    
    // time at which we start logging data
    var collection_start_timestamp = new Date().getTime() / 1000;
    var millis_start = new Date().getTime();
    
    var batches_uploaded = 0;
    var error_count = 0;
    var abandon_count = 0;

    var starting_timestamp_full = new Date().getTime();
    var starting_timestamp = new Date().getTime() / 1000;
    var starting_millis = 0;
    var current_millis;
    
    var num_datapoints;
    var data_buffer_offset;
    var data_buffer;
    

    var context = {starting_timestamp_full: starting_timestamp_full,
                   collection_start_timestamp: collection_start_timestamp,
                   batches_uploaded: 0};


    var tasks = [];
    for(var i = 0; i < NUM_ITERATIONS; i++) {
        tasks.push(function(cb) {
            var iteration_promise = do_iteration(context);
            iteration_promise.then(function(){
               cb(null); 
            });
        });
    }


    async.series(tasks);
    
});