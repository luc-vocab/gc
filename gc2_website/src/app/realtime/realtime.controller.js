
/* globals angular */
/* globals SmoothieChart */
/* globals TimeSeries */
/* globals _ */
/* globals JustGage */

(function() {
  'use strict';

  angular
    .module('gc2Website')
    .controller('RealtimeController', RealtimeController);

  /** @ngInject */
  function RealtimeController($timeout, $log, $scope, $rootScope, $firebaseObject, $document, $window, currentAuth, firebase_auth, device_manager) {
    var vm = this;


    
    function init() {
        $log.info("RealTimeController init");
        
        vm.realtime_mode_on = false;
        
        $log.info("currentAuth: ", currentAuth);
        vm.uid = currentAuth.uid;
        
        var user_ref = firebase_auth.get_user_ref(vm.uid);
        vm.user_obj = $firebaseObject(user_ref);
        
        vm.init_charts();

        vm.show_loading = true;
        vm.verify_device_setup();            
    }
    
    vm.init_charts = function() {

        // initialize justgage
        vm.gauge_current_value = vm.init_justgage("gauge_current_value", "Current Value");

        
        // resize canvas to something reasonable
        var margin = 60;
        var window_width = $window.innerWidth;
        if(window_width < 600 - margin) {
            var new_width = window_width - margin;
            $log.info("setting smoothie chart width to " + new_width);
            //var smoothie_chart_elt = angular.element( document.querySelector( '#smoothie-chart' ) );
            var smoothie_chart_elt = document.getElementById("smoothie-chart");
            smoothie_chart_elt.width = new_width;
            smoothie_chart_elt.height = new_width/2;
        }

        
        // initialize smoothie chart        
        var smoothie_chart_options = {
            grid:{
                fillStyle:'transparent',
                strokeStyle:'transparent',
                sharpLines:true,
                borderVisible:false},
                labels:{
                    fillStyle:'#000000',
                    fontFamily:'Oswald',
                    fontSize: 13,
                    precision: 0
                    
                },
                timestampFormatter:SmoothieChart.timeFormatter,
                horizontalLines:[
                    {color:'#ffffff',lineWidth:1,value:0},
                    {color:'#880000',lineWidth:2,value:3333},
                    {color:'#880000',lineWidth:2,value:-3333}]};
        var smoothie_chart = new SmoothieChart(smoothie_chart_options);
        var canvas = angular.element('#smoothie-chart')[0];
        
        vm.smoothie_series = new TimeSeries();
        
        smoothie_chart.addTimeSeries(vm.smoothie_series, {lineWidth:2.6,strokeStyle:'#5959f4',fillStyle:'#93a8ff'});
        smoothie_chart.streamTo(canvas, 500);

    }
    
    vm.init_justgage = function(id, title) {
        return new JustGage({
            id: id,
            value: 0,
            min: 0,
            max: 2000,
            title: title,
            startAnimationTime: 0
        });
    }
    
    vm.init_emg_stat_arrays = function() {
        vm.alltime_max = 0;
        
        // longer time horizon
        vm.emg_stat_array_1 = [];
        vm.emg_stat_duration_1 = 30;
        vm.emg_stats_1 = {};
        
        // shorter time horizon
        vm.emg_stat_array_2 = [];
        vm.emg_stat_duration_2 = 5;
        vm.emg_stats_2 = {};
        
    }


    function update_emg_value(emg_value) {

        vm.emg_stat_process_value(emg_value);        
        vm.gauge_current_value.refresh(emg_value, vm.alltime_max);
        
        vm.smoothie_series.append(new Date().getTime(), emg_value);

    }

    vm.emg_stat_process_value = function(emg_value) {
        if(emg_value > vm.alltime_max) {
            vm.alltime_max = emg_value;
        }
        
        var timestamp = new Date().getTime();
        vm.emg_stat_add_value_to_array(emg_value, vm.emg_stat_array_1, timestamp, vm.emg_stat_duration_1);
        vm.emg_stat_add_value_to_array(emg_value, vm.emg_stat_array_2, timestamp, vm.emg_stat_duration_2);
        
        vm.emg_stat_update();
    }

    vm.emg_stat_add_value_to_array = function(emg_value, stat_array, timestamp, duration) {
        var min_timestamp = timestamp - duration * 1000;
        
        stat_array.push({emg: emg_value,
                         timestamp: timestamp
                        });
                        
        var first_value = stat_array[0];
        while(first_value.timestamp < min_timestamp) {
            // remove this value
            stat_array.shift();
            first_value = stat_array[0];
        }
        
    }
    
    vm.emg_stat_update = function() {
        // get min, avg and max for both arrays
        vm.emg_stats_1.min = _.min(vm.emg_stat_array_1, function(item) { return item.emg; }).emg;
        vm.emg_stats_1.max = _.max(vm.emg_stat_array_1, function(item) { return item.emg; }).emg;
        var stats_1_sum = _.reduce(vm.emg_stat_array_1, function(memo, item){ return memo + item.emg; }, 0);
        //$log.info("stats_1_sum:", stats_1_sum);
        vm.emg_stats_1.avg = stats_1_sum / vm.emg_stat_array_1.length;
        
        /*
        vm.gauge_1_min.refresh(vm.emg_stats_1.min, vm.alltime_max);
        vm.gauge_1_avg.refresh(vm.emg_stats_1.avg, vm.alltime_max);
        vm.gauge_1_max.refresh(vm.emg_stats_1.max, vm.alltime_max);
        */
        
        vm.emg_stats_2.min = _.min(vm.emg_stat_array_2, function(item) { return item.emg; }).emg;
        vm.emg_stats_2.max = _.max(vm.emg_stat_array_2, function(item) { return item.emg; }).emg;        
        var stats_2_sum = _.reduce(vm.emg_stat_array_2, function(memo, item){ return memo + item.emg; }, 0);
        vm.emg_stats_2.avg = stats_2_sum / vm.emg_stat_array_2.length;

        /*
        vm.gauge_2_min.refresh(vm.emg_stats_2.min, vm.alltime_max);
        vm.gauge_2_avg.refresh(vm.emg_stats_2.avg, vm.alltime_max);
        vm.gauge_2_max.refresh(vm.emg_stats_2.max, vm.alltime_max);        
        */
        
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

    vm.enable_realtime =  function() {
        vm.show_enable_realtime_spinner = true;
        
        vm.init_emg_stat_arrays();
        
        var device_ref = device_manager.get_device_ref(vm.user_obj.device_id);
        device_ref.on("value", function(snapshot){
            var data = snapshot.val();
            // $log.info("received data: ", data);
            update_emg_value(data.emg_value);
        });        
        
        vm.current_device.callFunction('set_mode', 'realtime', function(err,data) {
            if(err) {
                $log.error("could not enable realtime mode");
            } else {
                $log.info("enabled realtime mode: ", data)
            }
            vm.show_enable_realtime_spinner = false;
            vm.realtime_mode_on = true;
            $scope.$apply();
        });
    };
    
    vm.standby = function() {
        vm.current_device.callFunction('set_mode', 'standby');
        vm.realtime_mode_on = false;
    };
    
  
    init();
    
  }
})();
