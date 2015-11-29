
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
            vm.device_setup_update = undefined; // don't display updates
            vm.realtime_ready = true;
        }, function(error) {
            $log.error("verify_device_setup error: ", error);
            vm.device_setup_error = error;
        }, function(update) {
            $log.info("verify_device_setup update: ", update);
            vm.device_setup_update = update;
        });
    };


    vm.enable_realtime =  function() {
        vm.show_enable_realtime_spinner = true;
        
        var device_ref = device_manager.get_device_ref(vm.user_obj.device_id);
        device_ref.on("value", function(snapshot){
            var data = snapshot.val();
            $log.info("received data: ", data);
            update_emg_value(data.emg_value);
        });        
        
        vm.current_device.callFunction('set_mode', 'realtime', function(err,data) {
            if(err) {
                $log.error("could not enable realtime mode");
            } 
            vm.show_enable_realtime_spinner = false;
            $scope.$apply();
        });
    };
    
    vm.standby = function() {
        vm.current_device.callFunction('set_mode', 'standby');
    };
    
  
    init();
    
  }
})();
