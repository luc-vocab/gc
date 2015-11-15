(function() {
  'use strict';

  angular
    .module('gc2Website')
    .controller('LoginController', LoginController);

  /** @ngInject */
  function LoginController($log, $scope, $rootScope, firebase_auth) {
    var vm = this;
    
    vm.form_data = {};
    
    vm.login = function() {
        $log.info("LoginController.login");
        firebase_auth.login(vm.form_data.login, vm.form_data.password);
    };    
    
    vm.logout =  function() {
        firebase_auth.logout();
    };
    
   
  }
})();
