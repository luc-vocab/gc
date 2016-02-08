
/* global angular:false */

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
      .state('login', {
        url: '/login',
        templateUrl: 'app/login/login.html',
        controller: 'LoginController',
        controllerAs: 'login'
      })
      .state('control', {
        url: '/control',
        templateUrl: 'app/control/control.html'
      })
      .state('control-realtime', {
        url: '/control/realtime',
        templateUrl: 'app/realtime/realtime.html',
        controller: 'RealtimeController',
        controllerAs: 'realtime',
        resolve: {
          // controller will not be loaded until $requireAuth resolves
          // Auth refers to our $firebaseAuth wrapper in the example above
          "currentAuth": ["firebase_auth", function(firebase_auth) {
            // $requireAuth returns a promise so the resolve waits for it to complete
            // If the promise is rejected, it will throw a $stateChangeError (see above)
            return firebase_auth.getAuth().$requireAuth();
          }]
        }        
      })
      .state('control-night', {
        url: '/control/night',
        templateUrl: 'app/nightmode/nightmode.html',
        controller: 'NightmodeController',
        controllerAs: 'nightmode',
        resolve: {
          // controller will not be loaded until $requireAuth resolves
          // Auth refers to our $firebaseAuth wrapper in the example above
          "currentAuth": ["firebase_auth", function(firebase_auth) {
            // $requireAuth returns a promise so the resolve waits for it to complete
            // If the promise is rejected, it will throw a $stateChangeError (see above)
            return firebase_auth.getAuth().$requireAuth();
          }]
        }        
      })      
      .state('data', {
        url: '/data',
        templateUrl: 'app/data/data.html'
      })
      .state('data-latest', {
        url: '/data/latest',
        templateUrl: 'app/latest/latest.html',
        controller: 'LatestController',
        controllerAs: 'latest',
        resolve: {
          // controller will not be loaded until $requireAuth resolves
          // Auth refers to our $firebaseAuth wrapper in the example above
          "currentAuth": ["firebase_auth", function(firebase_auth) {
            // $requireAuth returns a promise so the resolve waits for it to complete
            // If the promise is rejected, it will throw a $stateChangeError (see above)
            return firebase_auth.getAuth().$requireAuth();
          }]
        }        
      })
      .state('settings', {
        url: '/settings',
        templateUrl: 'app/settings/settings.html',
        controller: 'SettingsController',
        controllerAs: 'settings',
        resolve: {
          // controller will not be loaded until $requireAuth resolves
          // Auth refers to our $firebaseAuth wrapper in the example above
          "currentAuth": ["firebase_auth", function(firebase_auth) {
            // $requireAuth returns a promise so the resolve waits for it to complete
            // If the promise is rejected, it will throw a $stateChangeError (see above)
            return firebase_auth.getAuth().$requireAuth();
          }]
        }        
      })
      ;

    $urlRouterProvider.otherwise('/');
  }

})();
