var influent = require('influent');
var influx_config = require('./influx_config.js');


console.log("connecting to: ", influx_config.config);
 
influent
    .createClient(influx_config.config)
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
