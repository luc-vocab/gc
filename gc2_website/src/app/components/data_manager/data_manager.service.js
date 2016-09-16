/* globals Firebase: false */
/* globals angular */
/* globals influent */
/* globals _ */

(function() {
    'use strict';

    angular
        .module('gc2Website')
        .service('data_manager', data_manager);

    /** @ngInject */
    function data_manager(firebase_root, $log, $q) {
        var root_ref = new Firebase(firebase_root);
        var data_ref = root_ref.child('data');
        
        var self = this;
        
        this.latest_data_ref = function(uid) {
            return data_ref.child(uid).child('latest');
        };
        
     
        this.get_influx_client = function() {
            var defer = $q.defer();
            
            var influx_config_ref = new Firebase(firebase_root).child('config').child('influxdb');
            influx_config_ref.once('value', function(snapshot) {
                var data = snapshot.val();
                
                influent.createHttpClient({
                    username: data.username,
                    password: data.password,
                    database: data.db,
                    server: [
                        {
                            protocol: "https",
                            host: data.host,
                            port: data.port
                        }
                    ]
                }).then(function(client) {
                    defer.resolve(client);
                });
            })
            
            
            return defer.promise;
        };
        
        this.get_latest_emg_data = function(username, start_time, end_time) {
            var defer = $q.defer();
            

            var username_clause = "username='" + username + "'";
            var time_clause = "time > '" + start_time.toISOString() + "' and time < '" + end_time.toISOString() + "'";

            var query = "select MAX(emg_value) from emg where " + username_clause + 
                        " and " + time_clause + " GROUP BY time(1s)";
            $log.info("latest_emg_data query: ", query);

            self.get_influx_client().then(function(client) {
                client.query(query).then(function(result) {
                   $log.info("result: ", result); 
                   var data = result.results[0].series[0].values;
                   data = _.map(data, function(entry) {
                      var result = [new Date(entry[0]).getTime(), entry[1]];
                      return result;
                   });
                   $log.info("data: ", data);
                   defer.resolve(data);
                });
            });                              
          
            return defer.promise;
        };
        
        return this;

    }


})();
