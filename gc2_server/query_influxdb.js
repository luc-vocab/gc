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
	
	var query_latest = "select * from emg where time > now() - 30m";
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