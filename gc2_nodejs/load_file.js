var influent = require('influent');
var fs = require('fs');
var csv = require('fast-csv');

var starting_point = new Date(2015, 9, 16, 22, 0, 0, 0);

console.log("starting point: ", starting_point);

var measurements = [];

influent
    .createClient({
        username: "dev",
        password: "dev",
        database: "test_gc_data",
        server: [
            {
                protocol: "https",
                host:     "eightyeight-misterfusion-55.c.influxdb.com",
                port:     8086
            }
        ]
    })
    .then(function(client) {
        console.log("client created");
        
        var stream = fs.createReadStream("/home/dev/data/gc_logs/2013-06-15_GCLOG_43.TXT");
         
        var csvStream = csv({headers:true})
            .on("data", function(data){
                 // console.log(data);
                 var millis = parseInt(data.millis);
                 var emg_value = parseInt(data.sensorValue);
                 
                 var timestamp_millis = starting_point.getTime() + millis;
                 // console.log("timestamp_millis: ", timestamp_millis);
                 var actual_time = new Date(timestamp_millis);
                 
                 console.log("actual_time: ", actual_time, " emg_value: ", emg_value);
                 
                 if( !isNaN( actual_time.getTime() ) ) {
                 
                     // var timestamp_nanos = actual_time.getTime() * 1000000000;
                     var timestamp_nanos = actual_time.getTime().toString() + "000000";
                     
                     // console.log("timestamp_nanos: ", timestamp_nanos);
                     
                    
                     measurements.push({
                            key: "emg_series",
                            tags: {
                                user: "luc",
                            },
                            fields: {
                                emg_value: new influent.Value(emg_value, influent.type.INT64),
                            },
                            timestamp: timestamp_nanos                 
                     });
                     
                 }
                
                /*
                client
                    .writeOne({
                        key: "emg_series",
                        tags: {
                            user: "luc",
                        },
                        fields: {
                            emg_value: new influent.Value(emg_value, influent.type.INT64),
                        },
                        timestamp: timestamp_nanos
                    }).then(function(result){
                        console.log("write done ", result);
                    });
                */
                   
                    
            })
            .on("end", function(){
                 console.log("done parsing, loading into influxDB");
                 
                 client.writeMany(measurements).then(function() {
                    console.log("done writing to influxDB");
                 });
                 
            }, function(error) {
                console.log("error: ", error);
            });

         
        stream.pipe(csvStream);        
        

        
    },
    function(error) {
        console.log("error: ", error);
    });



