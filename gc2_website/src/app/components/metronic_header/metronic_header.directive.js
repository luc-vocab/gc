/* globals angular */

(function() {
  'use strict';

  angular
    .module('gc2Website')
    .directive('metronicheader', metronicHeader);

  /** @ngInject */
  function metronicHeader() {
    var directive = {
      restrict: 'E',
      templateUrl: 'app/components/metronic_header/metronic_header.html',
      scope: {
      },
      controller: MetronicHeaderController,
      controllerAs: 'vm',
      bindToController: true,
      replace: true
    };

    return directive;

    /** @ngInject */
    function MetronicHeaderController($scope, $rootScope,
                                      $log, $state) {
      var vm = this;
      
      $scope.$state = $state;
      
      
      var destroy_callback = $rootScope.$on("$stateChangeSuccess", function(event, toState, toParams, fromState, fromParams, error) {
        $log.info("stateChangeSuccess: ", toState);
        
        if(toState.name == "data-latest" || toState.name == "data") {
          vm.current_tab = "data";
        } else {
          vm.current_tab = toState.name;
        }
        
        $log.info("current_tab: ", vm.current_tab);
        
      });    
    
      $rootScope.$on('$destroy',  destroy_callback);

    }
  }

})();
