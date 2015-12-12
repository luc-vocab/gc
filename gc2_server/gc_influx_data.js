var Firebase = require('firebase');
var util = require('util');
var q = require('promised-io/promise');

var GcInfluxData = function(influx_client, firebase_root, username, uid) {
    
    var firebase_root_ref = new Firebase(firebase_root);
    this.firebase_data_latest_ref = firebase_root_ref.child('data').child(uid).child('latest');

    this.client = influx_client;
    this.username = username;
    this.uid = uid;
    
    var self = this;
    
    this.update_latest_data = function(time_clause) {
        self.compute_total_score(time_clause).then(function(total_score) {
           console.log("total_score: ", total_score);
           
           // update firebase
           self.firebase_data_latest_ref.update({
              total_score: total_score,
              last_update_time: Firebase.ServerValue.TIMESTAMP
           });
        });
        
    };
    
    this.compute_total_score = function(time_clause) {
        var defer = q.defer();
        
        console.log("compute_total_score");
        
	    var query_percentile = "select percentile(emg_value,85) from emg where " + time_clause + " and username='" + self.username + "'";	
	    console.log("query_percentile: ", query_percentile);
	
	    // perform query here
	    self.client
            .query(query_percentile)
            .then(function(result) {
                // console.log(util.inspect(result, {showHidden: false, depth: null}));
                var threshold_value = result.results[0].series[0].values[0][1];
                console.log("threshold_value: ", threshold_value);
                
                var query_sum = "select sum(emg_value) from emg where " + time_clause + " and emg_value > " + 
                                threshold_value + " and username='" + self.username + "'";
                self.client.query(query_sum).then(function(result) {
                    console.log(util.inspect(result, {showHidden: false, depth:null}));                                        
                    var sum = result.results[0].series[0].values[0][1];
                    console.log("sum: ", sum);
                    
                    var query_count = "select count(emg_value) from emg where " + time_clause + " and emg_value > " + 
                        threshold_value + " and username='" + self.username + "'";
                    self.client.query(query_count).then(function(result) {
                        console.log(util.inspect(result, {showHidden: false, depth:null}));
                        var count = result.results[0].series[0].values[0][1];
                        console.log("count: ", count);
                        
                        var total_score = sum - count * threshold_value;
                        console.log("total_score: ", total_score);
                        
                        defer.resolve(total_score);
                        
                    });
                });
                
            }, function(error) {
            	console.log("error: ", error);
            	
            });
    
        return defer.promise;
    };
    
    return this;
};

module.exports = GcInfluxData;

