(function() {
  'use strict';

  angular
    .module('gc2Website')
    .controller('RealtimeController', RealtimeController);

  /** @ngInject */
  function RealtimeController($timeout, $log, $scope, $rootScope, PubNub) {
    var vm = this;

    
    vm.channel = "sleep-track-data-luc";
    
    init();
    
    vm.emg_plot_values = [40, 100, 30];
    
    vm.chartConfig = {
        options: {
            chart: {
                type: 'column'
            },
        },
        series: [{
            data: vm.emg_plot_values
        }],        
        title: {
            text: 'EMG Value'
        }        
    };
    
    function add_value(emg_value) {
        $log.info("adding value: ", emg_value);

        vm.chartConfig.series[0].data.push(emg_value);     
        if( vm.chartConfig.series[0].data.length > 10 ) {
            vm.chartConfig.series[0].data.splice(0, 1);
        }

        $scope.$apply();
    }
    
    function init() {
        $log.info("RealTimeController init");
        
        PubNub.init({
            publish_key: 'pub-c-879cf9bb-46af-4bf1-8dca-e011ea412cd2',
            subscribe_key: 'sub-c-cba703c8-7b42-11e3-9cac-02ee2ddab7fe'
        });
        
        subscribe();
    }
    
    function subscribe() {
        PubNub.ngSubscribe({ channel: vm.channel });
    }
    
    
    $rootScope.$on(PubNub.ngMsgEv(vm.channel), function(event, payload) {
        // payload contains message, channel, env...
        $log.info("payload: ", payload.message);
        add_value(payload.message.emg_value);
    })    
    
  }
})();
