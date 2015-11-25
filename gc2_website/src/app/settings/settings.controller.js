/* globals spark: false */
/* globals _ */
/* globals angular */

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
        
    };
    
    vm.query_firmware = function(devices) {
      // query GC firmware version for all devices in array
      
      _.forEach(devices, function(device, index) {
        device.getVariable('gc_version', function(err,data) {
          if(err) {
            $log.info("err: ", err);
            devices[index].running_gc_firmware = false;
          } else {
            $log.info("data: ", data);
            devices[index].running_gc_firmware = true;
            devices[index].gc_firmware_version = data.result;
          }
          $scope.$apply();
        })
      });
      
    };
    
    vm.list_devices = function() {
        $log.info("list_devices");
      
        vm.show_status.refresh_spinner = true;
        
        vm.show_status.listing_devices = true;
        
        vm.show_status.listing_device_class = "alert-info";
        
        vm.show_status.listing_devices_ok = false;
        vm.show_status.listing_devices_error = false;
        
        var devicesPr = spark.listDevices();
        devicesPr.then(
          function(devices){
            $log.info('Devices: ', devices);
            
            // query firmware for each device
            vm.query_firmware(devices);
            
            vm.show_status.refresh_spinner = false;
            
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
            
            vm.show_status.refresh_spinner = false;
            
            $scope.$apply();
          }
        );                        
    };
    
    vm.select_device = function(index) {
      var selected_device = vm.devices[index];
      
      if(selected_device.running_gc_firmware) {
        // proceed
        $log.info("selected device: ", selected_device.name);
        vm.devices[index].selecting_progress = true;
        // $scope.$apply();
        
        device_manager.select_device(selected_device, currentAuth.uid).then(
        function(device_id){
          vm.show_status.select_device_error = false;
          vm.devices[index].selecting_progress = false;
        }, function(error) {
          $log.error("error selecting device: ", error);
          vm.show_status.select_device_error = true;
          vm.show_status.select_device_error_text = error;
          vm.devices[index].selecting_progress = false;
        })
      }
      
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
