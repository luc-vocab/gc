/* globals Firebase: false */
/* globals spark: false */
/* globals angular */

(function() {
  'use strict';

  angular
      .module('gc2Website')
      .service('device_manager', device_manager);

  /** @ngInject */
  function device_manager(firebase_root, firebase_auth, $log, $q) {
    var root_ref = new Firebase(firebase_root);
    var devices_ref = root_ref.child('devices');
    var servers_ref = root_ref.child('servers');
    var spark_login_done =  false;
    
    
    this.spark_login = function(particle_access_token) {
        $log.info("logging in with token: ", particle_access_token);
        return spark.login({accessToken: particle_access_token});
    };
    
    this.get_servers_ref = function() {
        return servers_ref;
    }
    
    this.get_device_ref = function(device_id) {
        return devices_ref.child(device_id);
    };
    
    
    // verify that default device is online and configured (before starting realtime/batch mode)
    this.verify_device = function(uid) {
        var defer = $q.defer();
        var num_tasks = 4;
        
        // retrieve default device
       
        var user_ref = firebase_auth.get_user_ref(uid);
        user_ref.once("value", function(snapshot) {
            var user_data = snapshot.val();
            
            defer.notify({status:"Logging in to Particle", task:1, total:num_tasks});
    
            var login_defer = $q.defer();
            if( ! spark_login_done) {
                // login to spark
                $log.info("logging in to particle");
                var access_token = user_data.particle_access_token;
                if( !access_token ) {
                    defer.reject({message:"No Particle Access Token",go_settings:true});
                } else {
                    spark.login({accessToken: access_token}).then(
                        function(token){
                            $log.info("verify_device: spark login successful ", token);
                            spark_login_done = true;
                            login_defer.resolve();
                        },
                        function(err) {
                            defer.reject({message:"Couldn't login to Particle, please check Particle Access Token",
                                          api_error: err,
                                          go_settings:true});
                        }
                    );                
                }
            } else {
                // no need to login
                $log.info("particle login already done");
                login_defer.resolve();
            }

            defer.notify({status:"Retrieving device information", task:2, total:num_tasks});
            login_defer.promise.then(function(){
               // get device id 
               if(! user_data.device_id || ! user_data.device_name) {
                   defer.reject({message:"No device configured, please check settings.",
                                 go_settings:true});
               } else {
                   defer.notify({status:"Checking device status", task:3, total:num_tasks});
                   var device_name = user_data.device_name;
                   spark.getDevice(device_name, function(err, device) {
                       if(err) {
                           $log.error("verify_device error: ", err);
                           defer.reject({message:"Couldn't obtain device details ",
                                         api_error: err.message,
                                         device_name: device_name,
                                         go_settings:true});
                       } else {
                           defer.notify({status:"Checking that device is online",task:4, total:num_tasks});
                           if(! device.connected) {
                               defer.reject({message:"Device is not connected",
                                             device_name: device_name});
                           } else {
                               // check whether device setup has been done
                               device.getVariable("setup_done", function(err, data) {
                                  if(err) {
                                      defer.reject({message:"Could not check whether device setup was done",
                                                    api_error: err.message,
                                                    device_name: device_name});
                                  } else {
                                      $log.info("retrieved setup_done variable: ", data);
                                      if(data.result == 1) {
                                          defer.resolve({message:"Device ready",
                                                         device: device});
                                      } else {
                                          defer.reject({message:"Device setup not done",
                                                        device_name: device_name,
                                                        go_settings:true});
                                      }
                                      
                                  }
                               });
                               
                           }
                       }
                   });
               }                
            });
            

        });
        
        return defer.promise;   
    };
    
    this.select_device = function(device, uid, server_data) {
        var defer = $q.defer();
    
        // get snapshot under devices
        devices_ref.once("value", function(snapshot) {
            // identify unique number
            var max_int32 = 2147483647;
            var tentative_device_id = Math.floor((Math.random() * max_int32) + 1).toString(); 
            while (snapshot.hasChild(tentative_device_id)) {
                tentative_device_id = Math.floor((Math.random() * max_int32) + 1).toString(); 
            }
            // device id should be unique
            var device_id = tentative_device_id;
            $log.info("device_id: ", device_id);

            // call spark function to set config
            // call "set_config" function
            var config_string = server_data.hostname + "," + server_data.port + "," + device_id;
            device.callFunction('set_config', config_string).then(
            function(result){
                $log.info("device_id result: ", result);
                $log.info("associated device ", device.name, " with id: ", device_id);
                
                // create firebase entry
                var device_ref = devices_ref.child(device_id);
                device_ref.set({
                    owner_uid: uid,
                    device_name: device.name
                });
                
                var user_ref = firebase_auth.get_user_ref(uid);
                user_ref.update({
                    device_name: device.name,
                    device_id: device_id
                });                
                
                defer.resolve(device_id);
            },
            function(error){
                $log.info("device_id error: ", error);
                defer.reject(error.message);
            });

        });
        
        return defer.promise;
    
    };
  
  }
  

})();
