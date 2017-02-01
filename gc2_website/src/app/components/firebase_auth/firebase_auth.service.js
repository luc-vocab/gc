/* globals Firebase: false */
/* globals firebase: false */
/* globals angular */

(function() {
  'use strict';

  angular
      .module('gc2Website')
      .service('firebase_auth', firebase_auth);

  /** @ngInject */
  function firebase_auth(firebase_config, $log, $firebaseAuth) {
    /*
    var root_ref = new Firebase(firebase_root);
    var users_ref = root_ref.child('users');
    var auth = $firebaseAuth(root_ref);
    */
    
    firebase.initializeApp(firebase_config);
    var root_ref = firebase.database().ref();
    var users_ref = root_ref.child('users');
    var auth = $firebaseAuth();

    this.getAuth = function() {
        return auth;
    };
    
    this.login = function(username, password, defer){
        $log.info("logging in with username: ", username);
        auth.$authWithPassword({
            email: username,
            password: password
        }).then(function(authData) {
            $log.info("Logged in as:", authData.uid);
            var user_ref = users_ref.child(authData.uid);
            user_ref.update({
                last_login: Firebase.ServerValue.TIMESTAMP
            });
            defer.resolve();
        }).catch(function(error) {
            $log.error("Authentication failed:", error);
            defer.reject(error.message);
        });        
        
    };
    
    this.logout = function() {
        $log.info("logging out");
        auth.$unauth();
    };
    
    this.get_user_ref = function(uid) {
        var user_ref = users_ref.child(uid);
        return user_ref;
    };

  }
  

})();
