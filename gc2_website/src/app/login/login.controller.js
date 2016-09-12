(function() {
  'use strict';

  angular
    .module('gc2Website')
    .controller('LoginController', LoginController);

  /** @ngInject */
  function LoginController($log, $scope, $rootScope, $q, firebase_auth) {
    var vm = this;
    
    vm.form_data = {};
    
    vm.show_success = false;
    vm.show_error = false;
    vm.show_spinner = false;
    
    vm.login = function() {
        $log.info("LoginController.login");
        var defer = $q.defer();
        vm.show_spinner = true;
        firebase_auth.login(vm.form_data.login, vm.form_data.password, defer);
        defer.promise.then(function(){
          vm.show_success = true;
          vm.show_error = false;
          vm.show_spinner = false;
        }, function(error) {
          $log.info("LoginController error: ", error);
          vm.show_sucess = false;
          vm.show_error = true;
          vm.login_error = error;
          vm.show_spinner = false;
        })
    };    
    
    vm.logout =  function() {
        firebase_auth.logout();
    };
    
   
  }
})();
