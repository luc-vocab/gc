/* global moment:false */
/* global angular:false */
(function() {
  'use strict';

  angular
    .module('gc2Website')
    .constant('moment', moment)
    
    // firebase setup
    .constant('firebase_root', 'https://gcserver-dev.firebaseio.com/');

})();
