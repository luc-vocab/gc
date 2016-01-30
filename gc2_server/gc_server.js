var influent = require('influent');
var pubnub = require("pubnub");
var net = require('net');
var Firebase = require('firebase');
var q = require('promised-io/promise');
var winston = require('winston');
var GcClient = require('./gc_server_connection');
var GcConfig = require('./gc_config');
require('winston-papertrail').Papertrail;

require('ssl-root-cas/latest')
  .inject()
  .addFile(__dirname + '/isrgrootx1.pem')
  .addFile(__dirname + '/letsencryptauthorityx1.pem')
  .addFile(__dirname + '/lets-encrypt-x1-cross-signed.pem');


GcConfig(true).then(function(config_data) {
    console.log("config_data:", config_data);
    
    var config = config_data.config;
    var server_config = config_data.server_config;

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
    
    
    process.on('uncaughtException', (err) => {
        logger.error("unhandled exception:", err, err.stack);
    });
    
    
    influent
    .createClient({
        username: config.influxdb.username,
        password: config.influxdb.password,
        database: config.influxdb.db,
        server: [
            {
                protocol: 'https',
                host:     config.influxdb.host,
                port:     config.influxdb.port
            }
        ]
    })
    .then(function(client) {
        var server = net.createServer(function(socket) {
            
            logger.info(socket.remoteAddress, ':', socket.remotePort, "received connection");
            var gcClient = new GcClient(socket, client, config, config_data.firebase_root, logger);
        });
        
        var server_key = config_data.server_key;
        
        logger.info("server", server_key, "listening on", server_config.port);
        server.listen(server_config.port, '0.0.0.0');
        
        // mark server online
        var server_ref = new Firebase(config_data.firebase_root).child('servers').child(server_key);
        server_ref.update({
            online: true
        });
        var presenceRef = server_ref.child("online");
        presenceRef.onDisconnect().set(false);
        
    }, function(error) {
        logger.error("could not create influxdb client:", error);
    });
    
});