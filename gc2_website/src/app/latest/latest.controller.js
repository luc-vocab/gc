
/* globals angular */
/* globals Highcharts */

(function() {
  'use strict';

  angular
    .module('gc2Website')
    .controller('LatestController', LatestController);

  /** @ngInject */
  function LatestController($log, $scope, $rootScope, $firebaseObject, 
                           currentAuth, firebase_auth, data_manager) {
    var vm = this;

    
    vm.init = function() {
        // bind to the latest data object for this user
        vm.uid = currentAuth.uid;
        
        var user_ref = firebase_auth.get_user_ref(vm.uid);
        vm.user_obj = $firebaseObject(user_ref);
        
        var latest_data_ref = data_manager.latest_data_ref(vm.uid);
        vm.latest_data_obj = $firebaseObject(latest_data_ref);
        
        vm.user_obj.$loaded().then(function(){
            
            vm.latest_data_obj.$watch(function(){
               vm.load_data(); 
            });
            vm.load_data();
        })
    };
    
    vm.load_data = function() {
      $log.info("LatestController.load_data");
      var start_time = new Date(vm.latest_data_obj.end_timestamp - 300*1000);
      var end_time = new Date(vm.latest_data_obj.end_timestamp);
      data_manager.get_latest_emg_data(vm.user_obj.user_name, 
                                       start_time,
                                       end_time).
      then(function(data) {
        // plot data on chart
        vm.chart_emg_data(data);  
      })
    };
    
    
    vm.chart_emg_data = function(data) {
        Highcharts.setOptions({
          "global": {
            useUTC: false
          }
        });
      
        angular.element('#container-emg-chart').highcharts({
            chart: {
                type: "area"
            },
            title: {
                text: 'Latest EMG data'
            },
            xAxis: {
                type: 'datetime'
            },
            yAxis: {
                title: {
                    text: 'EMG Value'
                }
            },
            legend: {
                enabled: false
            },
            plotOptions: {
                area: {
                    fillColor: {
                        linearGradient: {
                            x1: 0,
                            y1: 0,
                            x2: 0,
                            y2: 1
                        },
                        stops: [
                            [0, Highcharts.getOptions().colors[0]],
                            [1, Highcharts.Color(Highcharts.getOptions().colors[0]).setOpacity(0).get('rgba')]
                        ]
                    },
                    marker: {
                        radius: 2
                    },
                    lineWidth: 1,
                    states: {
                        hover: {
                            lineWidth: 1
                        }
                    },
                    threshold: null
                }
            },            
            series: [{
                type: 'area',
                name: 'EMG',
                data: data
            }]
        });      
        
    };


    vm.init(); 
    
  }
})();
