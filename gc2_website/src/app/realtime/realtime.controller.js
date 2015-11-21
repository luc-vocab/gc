
/* globals Highcharts: false */
/* globals spark: false */

(function() {
  'use strict';

  angular
    .module('gc2Website')
    .controller('RealtimeController', RealtimeController);

  /** @ngInject */
  function RealtimeController($timeout, $log, $scope, $rootScope, $firebaseObject, currentAuth, firebase_auth, device_manager) {
    var vm = this;

    vm.channel = "sleep-track-data-luc";
    
   
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
                
        spark_setup();
    }
    
    function spark_setup() {
        vm.user_obj.$loaded().then(function(){
            $log.info("spark access token: ", vm.user_obj.particle_access_token);
            // login to spark
            spark.login({accessToken: vm.user_obj.particle_access_token}).then(
                function(token){
                    $log.info("spark login successful ", token);
                    spark_list_devices();
                },
                function(err) {
                    $log.error("spark login error: ", err);
                }
            );
        });        
    }
    
    function spark_list_devices() {
        var devicesPr = spark.listDevices();
        devicesPr.then(
          function(devices){
            $log.info('Devices: ', devices);
            vm.devices = devices;
            if(vm.devices.length == 1) {
                vm.current_device = vm.devices[0];
                device_manager.create_device_id(vm.current_device, vm.uid).
                then(function(device_id) {
                    vm.current_device_id = device_id;
                    $log.info("current_device_id: ", vm.current_device_id);
                });
            }
            $scope.$apply();
          },
          function(err) {
            $log.error('List devices call failed: ', err);
          }
        );    
    }
    
    vm.enable_realtime =  function() {
        vm.current_device.callFunction('set_mode', 'realtime');
        var device_ref = device_manager.get_device_ref(vm.current_device_id);
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
