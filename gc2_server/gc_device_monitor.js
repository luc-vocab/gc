var Firebase = require('firebase');
var util = require('util');
var q = require('promised-io/promise');

var GcDeviceMonitor = function(logger, firebase_root, username, uid, device_id) {
    
    var firebase_root_ref = new Firebase(firebase_root);
    this.firebase_data_latest_ref = firebase_root_ref.child('data').child(uid).child('latest');
    this.firebase_data_historical_ref = firebase_root_ref.child('data').child(uid).child('historical');
    this.device_ref = firebase_root_ref.child('devices').child(device_id);

    this.username = username;
    this.uid = uid;
    
    var self = this;
    

    this.log_base = function(level, args) {
        var args = Array.prototype.slice.call(args);
        
        // figure out what we know about this client
        args.unshift(self.username);
        args.unshift(self.uid);
        logger.log(level, args.join(' '));
    }

    this.log_debug = function() {
        self.log_base('debug', arguments);
    }

    this.log_info = function() {
        self.log_base('info', arguments);
    }
    
    this.log_error = function() {
        self.log_base('error', arguments);
    }

    this.subscribe_device_node = function() {
        // keep an open subscription to the device node and update as needed
        self.device_ref.on('value', function(snapshot) {
            
            if(self.timer_id) {
                clearTimeout(self.timer_id);
            }
            self.timer_id = setTimeout(function() {
               self.log_info("no updates in 20mn, marking device offline");
               self.device_ref.update({
                   "mode": "offline"
               });
            }, 20*60*1000);
                        
        });
    };
    

    return this;
};

module.exports = GcDeviceMonitor;

