
/* globals Highcharts: false */
/* globals spark: false */
/* globals angular */

(function() {
  'use strict';

  angular
    .module('gc2Website')
    .controller('RealtimeController', RealtimeController);

  /** @ngInject */
  function RealtimeController($timeout, $log, $scope, $rootScope, $firebaseObject, currentAuth, firebase_auth, device_manager) {
    var vm = this;


    vm.emgGaugeOptions = {
        chart: {
            type: 'solidgauge'
        },

        title: null,

        pane: {
            center: ['50%', '85%'],
            size: '140%',
            startAngle: -90,
            endAngle: 90,
            background: {
                backgroundColor: (Highcharts.theme && Highcharts.theme.background2) || '#EEE',
                innerRadius: '60%',
                outerRadius: '100%',
                shape: 'arc'
            }
        },

        tooltip: {
            enabled: false
        },

        // the value axis
        yAxis: {
            min: 0,
            max: 2000,        
            stops: [
                [0.1, '#55BF3B'], // green
                [0.5, '#DDDF0D'], // yellow
                [0.9, '#DF5353'] // red
            ],
            lineWidth: 0,
            minorTickInterval: null,
            tickPixelInterval: 400,
            tickWidth: 0,
            title: {
                y: 30
            },
            labels: {
                y: 16
            }
        },

        plotOptions: {
            solidgauge: {
                dataLabels: {
                    y: 5,
                    borderWidth: 0,
                    useHTML: true
                }
            }
        },
        
        series: [{
            name: 'Speed',
            data: [80],
            dataLabels: {
                format: '<div style="text-align:center"><span style="font-size:25px;color:' +
                    ((Highcharts.theme && Highcharts.theme.contrastTextColor) || 'black') + '">{y}</span><br/>'
            }
        }]        
        
    };
    
   
    function update_emg_value(emg_value) {
        var chart = angular.element('#container-emg').highcharts();

        if (chart) {
            var point = chart.series[0].points[0];
            point.update(emg_value);
        }        
    }
    
    function init() {
        $log.info("RealTimeController init");
        
        $log.info("currentAuth: ", currentAuth);
        vm.uid = currentAuth.uid;
        
        var user_ref = firebase_auth.get_user_ref(vm.uid);
        vm.user_obj = $firebaseObject(user_ref);
        
        // initialize chart
        $log.info("gauge options: ", vm.emgGaugeOptions);
        angular.element('#container-emg').highcharts(vm.emgGaugeOptions);
                
        vm.show_loading = true;
        vm.spark_setup();            
    }
    
    vm.device_setup = function() {
        vm.show_error = false;
        vm.show_device_not_connected = false;
        vm.show_loading = true;
        
        var device_name = vm.user_obj.device_name;
        var device_id = vm.user_obj.device_id;
        if(device_name && device_id) {
            // we have a device name
            spark.getDevice(vm.user_obj.device_name, function(err, device) {
                if(err) {
                    $log.error("could not get device object for device ", device_name);
                    vm.error_message = "Could not get device object for device [" + device_name + "]";
                    vm.show_error = true;                    
                } else {
                    $log.info("got device object: ", device);
                    if(! device.connected) {
                        $log.info("device not connected: ", device.name);
                        // indicate that device is not connected
                        vm.show_device_not_connected = true;
                    } else {
                        vm.current_device = device;
                        vm.realtime_ready = true;
                    }
                }
                vm.show_loading = false;
                $scope.$apply();                
            });
        
        } else {
            // no device selected - go to settings  
            $log.error("no device selected");
            vm.error_message = "No device selected";
            vm.show_error = true;
            vm.show_loading = false;
            $scope.$apply();
        };
        
    };
    
    vm.spark_setup = function() {
        vm.user_obj.$loaded().then(function(){
            var access_token = vm.user_obj.particle_access_token;
            if(access_token) {
                $log.info("spark access token: ", access_token);
                // login to spark
                spark.login({accessToken: access_token}).then(
                    function(token){
                        $log.info("spark login successful ", token);
                        vm.device_setup();
                    },
                    function(err) {
                        $log.error("spark login error: ", err);
                        vm.error_message = "Particle login error: " + err;
                        vm.show_error = true;   
                        vm.show_loading = false;
                        $scope.$apply();
                    }
                );                
            } else {
                vm.error_message = "No Particle Access Token";
                vm.show_error = true;
                vm.show_loading = false;
            }
        });        
    }
    
    vm.enable_realtime =  function() {
        vm.current_device.callFunction('set_mode', 'realtime');
        var device_ref = device_manager.get_device_ref(vm.user_obj.device_id);
        device_ref.on("value", function(snapshot){
            var data = snapshot.val();
            $log.info("received data: ", data);
            update_emg_value(data.emg_value);
        });
    };
    
    vm.standby = function() {
        vm.current_device.callFunction('set_mode', 'standby');
    };
    
  
    init();
    
  }
})();
