var influent = require('influent');
var util = require('util');

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

	var query_percentile = "select percentile(emg_value,85) from emg where time > now() - 30m";	
	
	// perform query here
	client
            .query(query_percentile)
            .then(function(result) {
                // console.log(util.inspect(result, {showHidden: false, depth: null}));
                var threshold_value = result.results[0].series[0].values[0][1];
                console.log("threshold_value: ", threshold_value);
                
                var query_sum = "select sum(emg_value) from emg where time > now() - 30m and emg_value > " + threshold_value;
                client.query(query_sum).then(function(result) {
                    console.log(util.inspect(result, {showHidden: false, depth:null}));                                        
                    var sum = result.results[0].series[0].values[0][1];
                    console.log("sum: ", sum);
                    
                    var query_count = "select count(emg_value) from emg where time > now() - 30m and emg_value > " + threshold_value;
                    client.query(query_count).then(function(result) {
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
	
});


/*
query ideas
// get the value at the 85th percentile
select percentile(emg_value,85) from emg where time > now() - 30m

// sum all datapoints above that value
select sum(emg_value) from emg where time > now() - 30m and emg_value > 121

// count all datapoints above that value
select count(emg_value) from emg where time > now() - 30m and emg_value > 121

// then subtract the 2
sum - count*threshold

*/