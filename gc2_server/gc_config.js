var Firebase = require('firebase');
var q = require('promised-io/promise');


var GcConfig = function(is_gc_server) {
    var defer = q.defer();
    
    // retrieve influxdb and papertrail configs
    
    // retrieve config from firebase
    var firebase_root = process.env.FB_ROOT;
    if(!firebase_root) {
        throw "FB_ROOT not set [FB_ROOT=gcserver-dev.firebaseio.com]";
    }

    var firebase_root = 'https://' + firebase_root + '/';
    var firebase_root_ref = new Firebase(firebase_root);
    var config_ref = firebase_root_ref.child('config');
    config_ref.once('value', function(snapshot) {
       var config = snapshot.val();
       
       if(is_gc_server == true) {
            var server_key = process.env.SERVER_KEY;
            if(!server_key) {
                throw "SERVER_KEY not set";
            }
           
            // retrieve server config
            var server_ref = firebase_root_ref.child('servers').child(server_key);
            
            server_ref.once('value', function(snapshot){
                var server_config = snapshot.val();
                defer.resolve({
                    firebase_root: firebase_root,
                    config: config,
                    server_config: server_config,
                    server_key: server_key
                });
            });
            
       } else {
           defer.resove(firebase_root_ref, config);
       }
    }, function(err) {
        console.err("couldn't retrieve config: ", err);
    });

    
    return defer.promise;
}

module.exports = GcConfig;