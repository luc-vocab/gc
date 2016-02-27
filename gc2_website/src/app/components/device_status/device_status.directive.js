/* globals angular */

(function() {
  'use strict';

  angular
    .module('gc2Website')
    .directive('devicestatus', deviceStatus);

  /** @ngInject */
  function deviceStatus() {
    var directive = {
      restrict: 'E',
      templateUrl: 'app/components/device_status/device_status.html',
      scope: {
      },
      controller: DeviceStatusController,
      controllerAs: 'device',
      bindToController: true,
      replace: true
    };

    return directive;

    /** @ngInject */
    function DeviceStatusController($scope, $log, $firebaseObject, firebase_auth, device_manager) {
      var vm = this;
      
      vm.init = function() {
        // get authentication status
        var auth = firebase_auth.getAuth();
        auth.$onAuth(vm.auth_change);
      };
      
      vm.auth_change = function(authData) {
        vm.show_device_status = false;
        
        $log.info("DeviceStatusController.auth_change: ", authData);
        if(authData) {
          // authentication change
          // bind to user reference
          $log.info("user uid: ", authData.uid);
          var user_ref = firebase_auth.get_user_ref(authData.uid);
          
          
          // subscribe to updates on a user's device id
          var device_id_ref = user_ref.child('device_id');
          vm.device_id_obj = $firebaseObject(device_id_ref);
          
          vm.device_id_obj.$watch(function() {
            var device_id = vm.device_id_obj.$value;
            if(device_id) {
              $log.info("found device_id:", device_id);  
              vm.subscribe_device_id(device_id);
            } else {
              vm.no_device();
            }
            vm.show_device_status = true;
          })
        } 
      };
      
      vm.no_device = function() {
        $log.info("no device configured");
        vm.device_configured = false;
      }
      
      vm.subscribe_device_id = function(device_id) {
        // get device ref
        var device_ref = device_manager.get_device_ref(device_id);
        vm.device_obj = $firebaseObject(device_ref);
        vm.device_configured = true;
        $log.info("DeviceStatusController found device", device_id, " showing status");        
      }
      
      vm.init();
      

    }
  }

})();
