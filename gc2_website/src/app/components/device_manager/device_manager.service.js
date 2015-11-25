/* globals Firebase: false */
/* globals spark: false */

(function() {
  'use strict';

  angular
      .module('gc2Website')
      .service('device_manager', device_manager);

  /** @ngInject */
  function device_manager(firebase_root, firebase_auth, $log, $q) {
    var root_ref = new Firebase(firebase_root);
    var devices_ref = root_ref.child('devices');
    
    
    this.spark_login = function(particle_access_token) {
        $log.info("logging in with token: ", particle_access_token);
        return spark.login({accessToken: particle_access_token});
    };
    
    this.get_device_ref = function(device_id) {
        return devices_ref.child(device_id);
    };
    
    this.select_device = function(device, uid) {
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
            })
            
            // call spark function to set device id
            // calldevice_id function
            device.callFunction('device_id', device_id).then(
            function(result){
                $log.info("device_id result: ", result);
            },
            function(error){
                $log.info("device_id error: ", error);
            });

            $log.info("associated device ", device.name, " with id: ", device_id);
            
            defer.resolve(device_id);

        });
        
        return defer.promise;
    
    };
  
  }
  

})();
