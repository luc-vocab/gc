var util = require('util');

var GcInfluxData = function(influx_client) {
    
    console.log("created GcInfluxData");
    
    this.client = influx_client;
    
    var self = this;
    
    this.compute_total_score = function() {
        console.log("compute_total_score");
        
	    var query_percentile = "select percentile(emg_value,85) from emg where time > now() - 30m";	
	
	    // perform query here
	    self.client
            .query(query_percentile)
            .then(function(result) {
                // console.log(util.inspect(result, {showHidden: false, depth: null}));
                var threshold_value = result.results[0].series[0].values[0][1];
                console.log("threshold_value: ", threshold_value);
                
                var query_sum = "select sum(emg_value) from emg where time > now() - 30m and emg_value > " + threshold_value;
                self.client.query(query_sum).then(function(result) {
                    console.log(util.inspect(result, {showHidden: false, depth:null}));                                        
                    var sum = result.results[0].series[0].values[0][1];
                    console.log("sum: ", sum);
                    
                    var query_count = "select count(emg_value) from emg where time > now() - 30m and emg_value > " + threshold_value;
                    self.client.query(query_count).then(function(result) {
                        console.log(util.inspect(result, {showHidden: false, depth:null}));
                        var count = result.results[0].series[0].values[0][1];
                        console.log("count: ", count);
                        
                        var total_score = sum - count * threshold_value;
                        console.log("total_score: ", total_score);
                        
                    });
                });
                
            }, function(error) {
            	console.log("error: ", error);
            	
            });
    
    };
    
    return this;
};

module.exports = GcInfluxData;

