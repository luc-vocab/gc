/* globals spark: false */

(function() {
  'use strict';

  angular
    .module('gc2Website')
    .controller('SettingsController', SettingsController);

  /** @ngInject */
  function SettingsController($timeout, $log, $q, $scope, $rootScope, $firebaseObject, currentAuth, firebase_auth, device_manager) {
    var vm = this;
    
    vm.show_status = {};
    
   
    vm.init = function() {
        // subscribe to user object
        var user_ref = firebase_auth.get_user_ref(currentAuth.uid);
        vm.user_obj = $firebaseObject(user_ref);
        
        vm.user_obj.$loaded(function(){
            vm.verify_token();
        });
        
    }
    
    vm.list_devices = function() {
        $log.info("list_devices");
        
        vm.show_status.listing_devices = true;
        
        vm.show_status.listing_device_class = "alert-info";
        
        vm.show_status.listing_devices_ok = false;
        vm.show_status.listing_devices_error = false;
        
        var devicesPr = spark.listDevices();
        devicesPr.then(
          function(devices){
            $log.info('Devices: ', devices);
            vm.show_status.listing_devices_ok = true;
            vm.show_status.listing_device_class = "alert-success";
            
            vm.devices = devices;
            
            $scope.$apply();
          },
          function(err) {
            $log.error('List devices call failed: ', err);
            vm.show_status.listing_devices_error = true;
            vm.show_status.listing_devices_error_text = err.toString();
            vm.show_status.listing_device_class = "alert-danger";
            $scope.$apply();
          }
        );                        
    };
    
    vm.verify_token = function() {
        $log.info("verify_token");
        
        var login_promise = device_manager.spark_login(vm.user_obj.particle_access_token);
        
        login_promise.then(function() {
            $log.info("spark login successful");
            vm.list_devices();
           
        },
        function(error) {
            $log.error("spark login error: ", error);
        });
    }
    
    
    vm.init();
    
  }
})();
