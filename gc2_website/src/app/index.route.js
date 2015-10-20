(function() {
  'use strict';

  angular
    .module('gc2Website')
    .config(routerConfig);

  /** @ngInject */
  function routerConfig($stateProvider, $urlRouterProvider) {
    $stateProvider
      .state('home', {
        url: '/',
        templateUrl: 'app/main/main.html',
        controller: 'MainController',
        controllerAs: 'main'
      })
      .state('realtime', {
        url: '/realtime',
        templateUrl: 'app/realtime/realtime.html',
        controller: 'RealtimeController',
        controllerAs: 'realtime'
      })      
      ;

    $urlRouterProvider.otherwise('/');
  }

})();
