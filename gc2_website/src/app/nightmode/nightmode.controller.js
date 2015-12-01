
/* globals spark: false */
/* globals angular */

(function() {
  'use strict';

  angular
    .module('gc2Website')
    .controller('NightmodeController', NightmodeController);

  /** @ngInject */
  function NightmodeController($timeout, $log, $scope, $rootScope, $firebaseObject, currentAuth, firebase_auth, device_manager) {
    var vm = this;


    function init() {
        $log.info("NightmodeController init");
        
        vm.uid = currentAuth.uid;
        
        var user_ref = firebase_auth.get_user_ref(vm.uid);
        vm.user_obj = $firebaseObject(user_ref);
        
        vm.show_loading = true;
        vm.verify_device_setup();            
    }
    
    vm.verify_device_setup = function() {
        
        vm.realtime_ready = false;
        vm.device_setup_error = false;
        vm.device_setup_update = false;
        
        // ensure everything is good with the device setup
        var device_setup_promise = device_manager.verify_device(vm.uid);
        device_setup_promise.then(function(success) {
            vm.current_device_name = success.device.name;
            vm.current_device = success.device;
            vm.current_device_id = success.device_id;
            vm.device_setup_update = undefined; // don't display updates
            vm.bind_device_obj();
            vm.realtime_ready = true;
        }, function(error) {
            $log.error("verify_device_setup error: ", error);
            vm.device_setup_error = error;
        }, function(update) {
            $log.info("verify_device_setup update: ", update);
            vm.device_setup_update = update;
        });
    };


    vm.bind_device_obj = function() {
      var device_ref = device_manager.get_device_ref(vm.current_device_id);
      vm.device_obj = $firebaseObject(device_ref);
    };


    vm.enable_nightmode = function() {
        // vm.show_enable_realtime_spinner = true;
        
        vm.current_device.callFunction('set_mode', 'batch', function(err,data) {
            if(err) {
                $log.error("could not enable night mode");
            } 
            // vm.show_enable_nightmode_spinner = false;
            // $scope.$apply();
        });
        
    }


    init();
    
  }
})();
