var influent = require('influent');
var util = require('util');
var config = require('./' + process.argv[2]);
var GcInfluxData = require('./gc_influx_data');


influent
.createClient({
    username: config.influxUsername,
    password: config.influxPassword,
    database: config.influxDb,
    server: [
        {
            protocol: "http",
            host:     config.influxHost,
            port:     config.influxPort
        }
    ]
})
.then(function(client) {
    var influx_data = GcInfluxData(client, config.firebaseRoot, "luc-dev-1", "f0f624e0-055f-41f3-9954-0ffb1bde7d8d", "932326611");
    // console.log("influx_data: ", influx_data);
    influx_data.update_latest_data();
    // influx_data.get_current_night_intervals().then(function(result) {
    //   console.log(result); 
    // });  
    
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