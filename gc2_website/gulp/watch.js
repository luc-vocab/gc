'use strict';

var path = require('path');
var gulp = require('gulp');
var debug = require('gulp-debug');
var conf = require('./conf');

var browserSync = require('browser-sync');

function isOnlyChange(event) {
  return event.type === 'changed';
}

gulp.task('watch', ['inject', 'copy_blog_files'], function () {

  gulp.watch([path.join(conf.paths.src, '/*.html'), 'bower.json'], ['inject']);

  gulp.watch([
    path.join(conf.paths.src, '/app/**/*.css'),
    path.join(conf.paths.src, '/app/**/*.scss')
  ], function(event) {
    if(isOnlyChange(event)) {
      gulp.start('styles');
    } else {
      gulp.start('inject');
    }
  });

  gulp.watch(path.join(conf.paths.src, '/app/**/*.js'), function(event) {
    if(isOnlyChange(event)) {
      gulp.start('scripts');
    } else {
      gulp.start('inject');
    }
  });

  gulp.watch(path.join(conf.paths.src, '/app/**/*.html'), function(event) {
    browserSync.reload(event.path);
  });
  
  // watch blog area
  gulp.watch(path.join(conf.paths.blog_site, '/**/*'), function(event) {
    gulp.start('copy_blog_files');
  });
  
});

gulp.task('copy_blog_files', function() {
  gulp.src(conf.paths.blog_site)
  .pipe(gulp.dest(path.join(conf.paths.tmp, '/serve')));
});