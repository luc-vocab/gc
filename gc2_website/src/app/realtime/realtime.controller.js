(function() {
  'use strict';

  angular
    .module('gc2Website')
    .controller('RealtimeController', RealtimeController);

  /** @ngInject */
  function RealtimeController($timeout, $log, $scope, $rootScope, $firebaseObject, PubNub, currentAuth, firebase_auth) {
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
            },
        }]        
        
    };
    
   
    function update_emg_value(emg_value) {
        var chart = $('#container-emg').highcharts();

        if (chart) {
            var point = chart.series[0].points[0];
            point.update(emg_value);
        }        
    }
    
    function init() {
        $log.info("RealTimeController init");
        
        $log.info("currentAuth: ", currentAuth);
        
        var user_ref = firebase_auth.get_user_ref(currentAuth.uid);
        vm.user_obj = $firebaseObject(user_ref);
        
        // initialize chart
        $log.info("container emg: ", $('#container-emg'));
        $log.info("gauge options: ", vm.emgGaugeOptions);
        $('#container-emg').highcharts(vm.emgGaugeOptions);
                
        PubNub.init({
            publish_key: 'pub-c-879cf9bb-46af-4bf1-8dca-e011ea412cd2',
            subscribe_key: 'sub-c-cba703c8-7b42-11e3-9cac-02ee2ddab7fe'
        });
        
        subscribe();
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
    };
    
    function spark_list_devices() {
        var devicesPr = spark.listDevices();
        devicesPr.then(
          function(devices){
            $log.info('Devices: ', devices);
            vm.devices = devices;
            if(vm.devices.length == 1) {
                vm.current_device = vm.devices[0];
            }
            $scope.$apply();
          },
          function(err) {
            $log.error('List devices call failed: ', err);
          }
        );    
    };
    
    function subscribe() {
        PubNub.ngSubscribe({ channel: vm.channel });
    }
    
    
    $rootScope.$on(PubNub.ngMsgEv(vm.channel), function(event, payload) {
        // payload contains message, channel, env...
        $log.info("payload: ", payload.message);
        update_emg_value(payload.message.emg_value);
    })    
    
    init();
    
  }
})();
