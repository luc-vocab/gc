// require('es6-promise').polyfill();
var influent = require('influent');
 
console.log("starting up");
 
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
        
        client
            .writeOne({
                key: "emg_series",
                tags: {
                    user: "luc",
                },
                fields: {
                    emg_value: new influent.Value(10, influent.type.INT64),
                },
                timestamp: Date.now()
            });
        
    },
    function(error) {
        console.log("error: ", error);
    });
