(function() {
  'use strict';

  angular
    .module('gc2Website')
    .run(runBlock);

  /** @ngInject */
  function runBlock($log) {

    $log.debug('runBlock end');
  }

})();
