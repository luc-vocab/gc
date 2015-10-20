(function() {
  'use strict';

  angular
    .module('gc2Website')
    .controller('RealtimeController', RealtimeController);

  /** @ngInject */
  function RealtimeController($timeout, $log) {
    var vm = this;

    init();
    
    function init() {
        $log.info("RealTimeController init");
    }
    
  }
})();
