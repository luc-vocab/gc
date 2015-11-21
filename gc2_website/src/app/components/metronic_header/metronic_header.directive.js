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
      bindToController: true
    };

    return directive;

    /** @ngInject */
    function MetronicHeaderController(moment, $state, $log) {
      var vm = this;

    }
  }

})();
