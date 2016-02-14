'use strict';

var path = require('path');
var gulp = require('gulp');
var conf = require('./conf');

var browserSync = require('browser-sync');
var browserSyncSpa = require('browser-sync-spa');

var util = require('util');

var proxyMiddleware = require('http-proxy-middleware');
var fileExists = require('file-exists');

var buildRedirectCleanUrl = function(root) {
  var redirectCleanUrl = function(req, res, next) {
    // console.log("req.url", req.url);
    var original_url = req.url;

    var candidate_url = original_url + '/index.html';
    var candidate_full_path = root + candidate_url;
    if (fileExists(candidate_full_path)) {
      //console.log("File exists !", candidate_full_path);
      req.url = candidate_url;
    }
    
    candidate_url = original_url + '.html';
    candidate_full_path = root + candidate_url;
    if (fileExists(candidate_full_path)) {
      //console.log("File exists !", candidate_full_path);
      req.url = candidate_url;
    }    
    
    return next();
  };
  return redirectCleanUrl;
}

function browserSyncInit(root, baseDir, browser) {
  browser = browser === undefined ? 'default' : browser;

  var routes = null;
  if(baseDir === conf.paths.src || (util.isArray(baseDir) && baseDir.indexOf(conf.paths.src) !== -1)) {
    routes = {
      '/bower_components': 'bower_components'
    };
  }

  var server = {
    baseDir: baseDir,
    routes: routes,
    middleware: buildRedirectCleanUrl(root)
  };

  /*
   * You can add a proxy to your backend by uncommenting the line below.
   * You just have to configure a context which will we redirected and the target url.
   * Example: $http.get('/users') requests will be automatically proxified.
   *
   * For more details and option, https://github.com/chimurai/http-proxy-middleware/blob/v0.0.5/README.md
   */
  // server.middleware = proxyMiddleware('/users', {target: 'http://jsonplaceholder.typicode.com', proxyHost: 'jsonplaceholder.typicode.com'});

  browserSync.instance = browserSync.init({
    startPath: '/',
    server: server,
    browser: browser,
    port: 8080
  });
}

browserSync.use(browserSyncSpa({
  selector: '[ng-app]'// Only needed for angular apps
}));

gulp.task('serve', ['watch'], function () {
  var serve_path = path.join(conf.paths.tmp, '/serve');
  browserSyncInit(serve_path, [serve_path, conf.paths.src]);
});

gulp.task('serve:dist', ['build'], function () {
  browserSyncInit(conf.paths.dist, conf.paths.dist);
});

/*
gulp.task('serve:e2e', ['inject'], function () {
  browserSyncInit([conf.paths.tmp + '/serve', conf.paths.src], []);
});

gulp.task('serve:e2e-dist', ['build'], function () {
  browserSyncInit(conf.paths.dist, []);
});
*/