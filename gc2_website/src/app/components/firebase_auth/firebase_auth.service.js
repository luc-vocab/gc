(function() {
  'use strict';

  angular
      .module('gc2Website')
      .service('firebase_auth', firebase_auth);

  /** @ngInject */
  function firebase_auth(firebase_root, $log, $firebaseAuth) {
    var root_ref = new Firebase(firebase_root);
    var users_ref = root_ref.child('users');
    var auth = $firebaseAuth(root_ref);
    
    
    this.getAuth = function() {
        return auth;
    };
    
    this.login = function(username, password){
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
        }).catch(function(error) {
            $log.error("Authentication failed:", error);
        });        
        
    };
    
    this.logout = function() {
        $log.info("logging out");
        auth.$unauth();
    }    
    
  }
  

})();
