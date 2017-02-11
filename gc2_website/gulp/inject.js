'use strict';

var path = require('path');
var gulp = require('gulp');
var conf = require('./conf');

var $ = require('gulp-load-plugins')();

var wiredep = require('wiredep').stream;
var _ = require('lodash');



gulp.task('config', function () {
  var gc_env = process.env.GC_ENV;
  if(!gc_env)
  {
    throw "GC_ENV not set [GC_ENV=dev]";
  }

  // load the firebase config file
  //var firebase_config = require(path.join(conf.paths.src, '/app/firebase-conf.json'));
  var firebase_config = require('../src/app/firebase-conf');
  var firebase_env_config = firebase_config[gc_env];
  
  
  var constants = {"firebase_config": firebase_env_config};
  $.ngConstant({
      name: 'gc2Website',
      templatePath: path.join(conf.paths.src, '/app/config-template.ejs'),
      constants: constants,
      stream: true
    })
    .pipe($.rename("index.constants.js"))
    .pipe(gulp.dest('src/app'));
});

gulp.task('inject', ['scripts', 'styles', 'config'], function () {
  var injectStyles = gulp.src([
    path.join(conf.paths.tmp, '/serve/app/**/*.css'),
    path.join('!' + conf.paths.tmp, '/serve/app/vendor.css')
  ], { read: false });

  var injectScripts = gulp.src([
    path.join(conf.paths.src, '/app/**/*.module.js'),
    path.join(conf.paths.src, '/app/**/*.js'),
    path.join('!' + conf.paths.src, '/app/**/*.spec.js'),
    path.join('!' + conf.paths.src, '/app/**/*.mock.js'),
  ])
  .pipe($.angularFilesort()).on('error', conf.errorHandler('AngularFilesort'));

  var injectOptions = {
    ignorePath: [conf.paths.src, path.join(conf.paths.tmp, '/serve')],
    addRootSlash: false
  };

  return gulp.src(path.join(conf.paths.src, '/*.html'))
    .pipe($.inject(injectStyles, injectOptions))
    .pipe($.inject(injectScripts, injectOptions))
    .pipe(wiredep(_.extend({}, conf.wiredep)))
    .pipe($.rename(function(path) {
      if (path.basename == "app" && path.extname == ".html" ) {
        path.dirname = "app";
        path.basename = "index";
     }
    }))
    .pipe(gulp.dest(path.join(conf.paths.tmp, '/serve')));
});
