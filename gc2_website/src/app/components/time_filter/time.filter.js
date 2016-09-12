
/* global angular:false */

(function() {
    'use strict';

    angular
        .module('gc2Website')
        .filter('timefilter', time_filter);

    /** @ngInject */
    function time_filter() {
        return function(seconds) {
            var hours = Math.floor(seconds / 3600);
            var minutes = Math.floor((seconds % 3600) / 60);
            var timeString = '';
            if(hours > 0) timeString += hours + " hr ";
            if(minutes >= 0) timeString += minutes + " mn ";
            return timeString; 
        }
    }


})();
