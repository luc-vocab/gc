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
	
	var query_latest = "select * from emg where time > now() - 30m and time < now() - 29m";
	var query_percentile = "select percentile(emg_value,85) from emg where time > now() - 30m";
	var query = "select count (emg_value) from emg";
	
	// perform query here
	client
            .query(query_latest)
            .then(function(result) {
                // console.log(result.results[0]);
                console.log(util.inspect(result, {showHidden: false, depth: null}));
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