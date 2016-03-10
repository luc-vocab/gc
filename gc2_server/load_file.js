var influent = require('influent');
var fs = require('fs');
var csv = require('fast-csv');
var _ = require('underscore');
var q = require('promised-io/promise');
var async = require('async');

require('ssl-root-cas/latest')
  .inject()
  .addFile(__dirname + '/isrgrootx1.pem')
  .addFile(__dirname + '/letsencryptauthorityx1.pem')
  .addFile(__dirname + '/lets-encrypt-x1-cross-signed.pem');

// console.log("starting point: ", starting_point);


var LoadGCData = function(filename, client, starting_point, cb) {
    var defer = q.defer();

    var stream = fs.createReadStream(filename);

    var measurements = [];
     
    var csvStream = csv({headers:true})
        .on("data", function(data){
             // console.log(data);
             var millis = parseInt(data.millis);
             var emg_value = parseInt(data.sensorValue);
             var alarm = parseInt(data.warning_mode);
             
             var timestamp_millis = starting_point.getTime() + millis;
             // console.log("timestamp_millis: ", timestamp_millis);
             var actual_time = new Date(timestamp_millis);
             
             //console.log("actual_time: ", actual_time, " emg_value: ", emg_value, "alarm", alarm);
             
             if( !isNaN( actual_time.getTime() ) ) {
             
                 // var timestamp_nanos = actual_time.getTime() * 1000000000;
                 var timestamp_nanos = actual_time.getTime().toString() + "000000";
                 
                 // console.log("timestamp_nanos: ", timestamp_nanos);
                 
                
                 measurements.push({
                        key: "emg",
                        tags: {
                            username: "andrew",
                        },
                        fields: {
                            emg_value: new influent.Value(emg_value, influent.type.INT64),
                            alarm_value: new influent.Value(alarm, influent.type.INT64)
                        },
                        timestamp: timestamp_nanos
                 });
                 
             }
            

        })
        .on("end", function(){
             console.log("done parsing, loading into influxDB");
             
             // modify measurements so that first and last EMG values are 0
             measurements[0].fields.emg_value = new influent.Value(0, influent.type.INT64);
             measurements[measurements.length - 1].fields.emg_value = new influent.Value(0, influent.type.INT64);
             
             //console.log(measurements);
             
             client.writeMany(measurements).then(function() {
                console.log("done writing to influxDB, records: ", measurements.length);
                defer.resolve();
             });
             
        }, function(error) {
            console.log("error: ", error);
        });

     
    stream.pipe(csvStream);        

    return defer.promise;
}

influent
    .createClient({
        username: "gc_dev",
        password: "gc_dev",
        database: "gc1_data",
        server: [
            {
                protocol: "https",
                host:     "influxdb.dev.sleeptrack.io",
                port:     8086
            }
        ]
    })
    .then(function(client) {
        console.log("client created");
        
        
        
        var tasks = [];
        var starting_day = 2;
        
        var files = fs.readdirSync('/home/dev/data/andrew_gc_logs');
        //console.log(files);
        _.forEach(files, function(file) {
            
            //console.log("file:", file);
        
            var starting_point = new Date(2016, 0, starting_day, 12, 0, 0, 0);
            var filename = "/home/dev/data/andrew_gc_logs/" + file;
            
            
            tasks.push(function(cb) {
                console.log("processing file", filename, "starting_point", starting_point);
                var promise = LoadGCData(filename, client, starting_point);
                promise.then(function() {
                    cb(null);
                })
            });
            
        
            
            starting_day++;
        })
        
        async.series(tasks, function() {
            console.log("done with all files");
        })
        

        
    },
    function(error) {
        console.log("error: ", error);
    });



