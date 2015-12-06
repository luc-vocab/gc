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
          var user_obj = $firebaseObject(user_ref);
          user_obj.$loaded().then(function() {
            // check whether there is a device
            if(user_obj.device_id) {
              // get device ref
              var device_ref = device_manager.get_device_ref(user_obj.device_id);
              vm.device_obj = $firebaseObject(device_ref);
              vm.show_device_status = true;
              $log.info("DeviceStatusController found device, showing status");
            }            
          });

        } 
      };
      
      vm.init();
      

    }
  }

})();
