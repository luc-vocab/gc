var Firebase = require('firebase');
var util = require('util');
var q = require('promised-io/promise');

var GcInfluxData = function(logger, influx_client, firebase_root, username, uid, device_id) {
    
    var firebase_root_ref = new Firebase(firebase_root);
    this.firebase_data_latest_ref = firebase_root_ref.child('data').child(uid).child('latest');
    this.firebase_data_historical_ref = firebase_root_ref.child('data').child(uid).child('historical');
    this.device_ref = firebase_root_ref.child('devices').child(device_id);

    this.client = influx_client;
    this.username = username;
    this.uid = uid;
    this.device_id = device_id;
    
    var self = this;
    

    this.log_base = function(level, args) {
        var args = Array.prototype.slice.call(args);
        
        // figure out what we know about this client
        args.unshift(self.device_id);
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
            if(snapshot.val().mode=="night") {
                self.log_info("updating data");
                self.update_latest_data(); 
            }
        });
    };
    
    this.update_latest_data = function() {
        
        self.get_current_night_intervals().then(function(intervals) {
            self.compute_total_score(intervals.time_clause).then(function(total_score) {
                self.log_info("total_score:", total_score);
               
               var data = {
                  start_timestamp: intervals.start_timestamp,
                  end_timestamp: intervals.end_timestamp,
                  total_score: total_score,
                  collected_duration: intervals.collected_duration,
                  last_update_time: Firebase.ServerValue.TIMESTAMP
               };
               
               // update firebase "latest" data
               self.firebase_data_latest_ref.update(data);
               
               // update historical data
               var historical_ref = self.firebase_data_historical_ref.child(intervals.start_timestamp);
               historical_ref.update(data);
                               
               
            });            
        });
        

        
    };
    
    this.get_current_night_intervals = function() {
        self.log_debug("get_current_night_intervals");
        
        var defer = q.defer();
        
        self.device_ref.once('value', function(snapshot) {
            var data = snapshot.val();
            var start_timestamp = data.collection_start;
            var end_timestamp = data.last_upload_time;
            var start_date = new Date(start_timestamp);
            var end_date = new Date(end_timestamp);
            
            var result = {
                "start_timestamp": start_timestamp,
                "end_timestamp": end_timestamp,
                "time_clause": "time > '" + start_date.toISOString() + "' and time < '" + end_date.toISOString() + "'",
                "collected_duration": data.collected_duration
            }
            
            self.log_debug("get_current_night_intervals result ", result);
            
            defer.resolve(result);
        })
        
        return defer.promise;
    };
    
    this.compute_total_score = function(time_clause) {
        var defer = q.defer();
        

        self.log_debug("compute_total_score, time_clause: ", time_clause);
        
        
	    var query_percentile = "select percentile(emg_value,85) from emg where " + time_clause + " and username='" + self.username + "'";	
	    self.log_debug("query_percentile: ", query_percentile);
	
	    // perform query here
	    self.client
            .query(query_percentile)
            .then(function(result) {
                //console.log(util.inspect(result, {showHidden: false, depth: null}));
                var threshold_value = result.results[0].series[0].values[0][1];
                self.log_debug("threshold_value: ", threshold_value);
                
                var query_sum = "select sum(emg_value) from emg where " + time_clause + " and emg_value > " + 
                                threshold_value + " and username='" + self.username + "'";
                self.client.query(query_sum).then(function(result) {
                    self.log_debug(util.inspect(result, {showHidden: false, depth:null}));                                        
                    var sum = result.results[0].series[0].values[0][1];
                    self.log_debug("sum: ", sum);
                    
                    var query_count = "select count(emg_value) from emg where " + time_clause + " and emg_value > " + 
                        threshold_value + " and username='" + self.username + "'";
                    self.client.query(query_count).then(function(result) {
                        self.log_debug(util.inspect(result, {showHidden: false, depth:null}));
                        var count = result.results[0].series[0].values[0][1];
                        self.log_debug("count: ", count);
                        
                        var total_score = sum - count * threshold_value;
                        self.log_debug("total_score: ", total_score);
                        
                        defer.resolve(total_score);
                        
                    }, function(error) {
                        self.log_error(error);
                    });
                }, function(error) {
                    self.log_error(error);
                });
                
            }, function(error) {
            	self.log_error(error);
            	
            });
    
        return defer.promise;
    };
    
    return this;
};

module.exports = GcInfluxData;

