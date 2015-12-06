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
      controllerAs: 'vm',
      bindToController: true,
      replace: true
    };

    return directive;

    /** @ngInject */
    function DeviceStatusController($scope, $log, firebase_auth) {
      var vm = this;
      
      vm.init = function() {
        // get authentication status
        var auth = firebase_auth.getAuth();
        auth.$onAuth(vm.auth_change);
      };
      
      vm.auth_change = function(authData) {
        $log.info("DeviceStatusController.auth_change: ", authData);
        if(authData) {
          // authentication change
          vm.show_device_status = true;
        } else {
          // not authenticated
          vm.show_device_status = false;
        }
      };
      
      vm.init();
      

    }
  }

})();
