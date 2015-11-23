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
    function MetronicHeaderController(moment, $scope, $state, $log) {
      var vm = this;
      
      $scope.$state = $state;

    }
  }

})();
