/* globals Firebase: false */
/* globals angular */

(function() {
    'use strict';

    angular
        .module('gc2Website')
        .service('data_manager', data_manager);

    /** @ngInject */
    function data_manager(firebase_root, firebase_auth, $log, $q, 
                          $timeout) {
        var root_ref = new Firebase(firebase_root);
        var data_ref = root_ref.child('data');
        
        this.latest_data_ref = function(uid) {
            return data_ref.child(uid).child('latest');
        };
        
        
        return this;

    }


})();
