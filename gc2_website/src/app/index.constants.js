/* global malarkey:false, moment:false */
(function() {
  'use strict';

  angular
    .module('gc2Website')
    .constant('malarkey', malarkey)
    .constant('moment', moment)
    
    // firebase setup
    .constant('firebase_root', 'https://gcserver-dev.firebaseio.com/');

})();
