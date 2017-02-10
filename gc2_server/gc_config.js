var admin = require("firebase-admin");
var q = require('promised-io/promise');
var influent = require('influent');
var winston = require('winston');
require('winston-papertrail').Papertrail;


var GcConfig = function(server_type) {
    var defer = q.defer();

    // retrieve influxdb and papertrail configs

    // configure firebase
    var firebase_config_file = process.env.FB_CONFIG_FILE;
    if(!firebase_config_file)
    {
        throw "FB_CONFIG_FILE not set";
    }
    
    // retrieve config from firebase
    var firebase_root = process.env.FB_ROOT;
    if (!firebase_root) {
        throw "FB_ROOT not set [FB_ROOT=gcserver-dev.firebaseio.com]";
    }    

    // Fetch the service account key JSON file contents
    var serviceAccount = require(firebase_config_file);
    

    // Initialize the app with a null auth variable, limiting the server's access
    admin.initializeApp({
      credential: admin.credential.cert(serviceAccount),
      databaseURL: "https://"+ firebase_root +".firebaseio.com"
    });
    
    // for debugging of authentication
    // admin.database.enableLogging(true);
    
    // retrieve configuration
    var db = admin.database();
    var config_ref = db.ref("config");
     
    config_ref.once('value', function(snapshot) {

        var config = snapshot.val();
        

        // create logger
        var papertrailTransport = new winston.transports.Papertrail({
            host: config.papertrail.host,
            port: config.papertrail.port,
            level: config.papertrail.logging_level,
            program: 'gc_server'
        });

        var consoleTransport = new winston.transports.Console({
            level: 'info',
            timestamp: function() {
                return new Date().toString();
            },
            colorize: true
        });

        var logger = new winston.Logger({
            transports: [
                papertrailTransport,
                consoleTransport
            ]
        });


        // log unhandled exceptions
        process.on('uncaughtException', (err) => {
            logger.error("unhandled exception:", err, err.stack);
        });


        // create influxdb client
        influent
            .createClient({
                username: config.influxdb.username,
                password: config.influxdb.password,
                database: config.influxdb.db,
                server: [{
                    protocol: 'https',
                    host: config.influxdb.host,
                    port: config.influxdb.port
                }]
            })
            .then(function(client) {

                if (server_type == "gc_server") {
                    var server_key = process.env.SERVER_KEY;
                    if (!server_key) {
                        throw "SERVER_KEY not set";
                    }

                    // retrieve server config
                    var server_ref = db.ref('/servers').child(server_key);

                    server_ref.once('value', function(snapshot) {
                        var server_config = snapshot.val();
                        defer.resolve({
                            firebase_root: firebase_root,
                            influx_client: client,
                            config: config,
                            server_config: server_config,
                            server_key: server_key,
                            logger: logger
                        });
                    });

                }
                else {

                    defer.resolve({
                        firebase_root: firebase_root,
                        config: config,
                        influx_client: client,
                        logger: logger
                    });
                }


            }, function(err) {
                logger.error("could not create influxdb client", err);
            });

    }, function(err) {
        console.log("couldn't retrieve config: ", err);
    });


    return defer.promise;
}

module.exports = GcConfig;