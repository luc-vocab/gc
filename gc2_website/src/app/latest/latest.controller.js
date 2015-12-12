
/* globals angular */

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
        var latest_data_ref = data_manager.latest_data_ref(vm.uid);
        vm.latest_data_obj = $firebaseObject(latest_data_ref);
    };


    vm.init(); 
    
  }
})();
