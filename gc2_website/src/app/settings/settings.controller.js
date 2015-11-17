(function() {
  'use strict';

  angular
    .module('gc2Website')
    .controller('SettingsController', SettingsController);

  /** @ngInject */
  function SettingsController($timeout, $log, $q, $scope, $rootScope, $firebaseObject, currentAuth, firebase_auth, device_manager) {
    var vm = this;
    
    $scope.show_status = {};
    
   
    this.init = function() {
        // subscribe to user object
        var user_ref = firebase_auth.get_user_ref(currentAuth.uid);
        $scope.user_obj = $firebaseObject(user_ref);
        
        $scope.user_obj.$loaded(function(data){
            vm.verify_token();
        });
        
    }
    
    this.list_devices = function() {
        $log.info("list_devices");
        
        $scope.show_status.listing_devices = true;
        
        $scope.show_status.listing_device_class = "alert-info";
        
        $scope.show_status.listing_devices_ok = false;
        $scope.show_status.listing_devices_error = false;
        
        var devicesPr = spark.listDevices();
        devicesPr.then(
          function(devices){
            $log.info('Devices: ', devices);
            $scope.show_status.listing_devices_ok = true;
            $scope.show_status.listing_device_class = "alert-success";
            
            vm.devices = devices;
            
            $scope.$apply();
          },
          function(err) {
            $log.error('List devices call failed: ', err);
            $scope.show_status.listing_devices_error = true;
            $scope.show_status.listing_devices_error_text = err.toString();
            $scope.show_status.listing_device_class = "alert-danger";
            $scope.$apply();
          }
        );                        
    };
    
    this.verify_token = function() {
        $log.info("verify_token");
        
        var login_promise = device_manager.spark_login($scope.user_obj.particle_access_token);
        
        login_promise.then(function(token) {
            $log.info("spark login successful");
            vm.list_devices();
           
        },
        function(error) {
            $log.error("spark login error");
        });
    }
    
    
    this.init();
    
  }
})();
