(function() {
  'use strict';

  angular
    .module('gc2Website')
    .run(runBlock);

  /** @ngInject */
  function runBlock($log, $rootScope, $state) {
    
    $rootScope.$on("$stateChangeError", function(event, toState, toParams, fromState, fromParams, error) {
      // We can catch the error thrown when the $requireAuth promise is rejected
      // and redirect the user back to the home page
      if (error === "AUTH_REQUIRED") {
        $log.info("authentication required");
        $state.go("login");
      }
    });    
  

    $log.debug('runBlock end');
  }

})();
