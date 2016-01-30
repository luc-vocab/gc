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


GcConfig("gc_server").then(function(config_data) {
    console.log("config_data:", config_data);

    var config = config_data.config;
    var server_config = config_data.server_config;
    var client = config_data.influx_client;
    var logger = config_data.logger;


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


});