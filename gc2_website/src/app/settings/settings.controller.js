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
    
    vm.save_button_enabled = false;
    
   
    vm.init = function() {
        // subscribe to user object
        var user_ref = firebase_auth.get_user_ref(currentAuth.uid);
        vm.user_obj = $firebaseObject(user_ref);
        vm.servers_obj = $firebaseObject(device_manager.get_servers_ref());
        
        vm.user_obj.$loaded(function(){
            vm.verify_token();
        });
        
    };
    
    vm.query_firmware = function(devices) {
      // query GC firmware version for all devices in array
      
      _.forEach(devices, function(device, index) {
        // mark device as selected if we recognize the name
        if(device.name === vm.user_obj.device_name) {
          vm.selected_device = device;
        }
        
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
      vm.selected_device = vm.devices[index];
      vm.save_button_enabled = true;
    };
    

    vm.select_server = function(server_key) {
      vm.user_obj.server = server_key;
      vm.save_button_enabled = true;
    };
    
    vm.set_token = function() {
      $log.info("set_token");
      vm.save_button_enabled = true;
      vm.verify_token();
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
    };
    
    vm.set_username = function() {
      $log.info("set_username");
      vm.save_button_enabled = true;
    };
    
    vm.save_settings = function() {
      $log.info("save_settings");
      vm.save_in_progress = true;
      vm.save_error = false;
      device_manager.save_settings(vm.selected_device, 
                                   currentAuth.uid, 
                                   vm.servers_obj[vm.user_obj.server], 
                                   vm.user_obj.user_name,
                                   vm.user_obj.particle_access_token)
      .then(
        function(device_id){
          vm.save_in_progress = false;
          vm.save_button_enabled = false;
        }, function(error) {
          $log.error("error selecting device: ", error);
          vm.save_in_progress = false;
          vm.save_error = true;
          vm.save_error_message = error;
        });
                                          
    };
    
    vm.init();
    
  }
})();
