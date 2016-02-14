'use strict';

var path = require('path');
var gulp = require('gulp');
var debug = require('gulp-debug');
var shell = require('gulp-shell');
var conf = require('./conf');

var browserSync = require('browser-sync');

function isOnlyChange(event) {
  return event.type === 'changed';
}

gulp.task('watch', ['inject', 'build_and_copy_blog'], function () {

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
  
  
  // watch blog src area
  gulp.watch(path.join(conf.paths.blog_src, '/**/*'), function(event){
    gulp.start('build_blog');
  })
  
  // watch blog dist area
  gulp.watch(path.join(conf.paths.blog_site, '/**/*'), function(event) {
    gulp.start('copy_blog_files');
  });
  
});

gulp.task('copy_blog_files', function() {
  return gulp.src(path.join(conf.paths.blog_site, '/**/*'))
  .pipe(gulp.dest(path.join(conf.paths.tmp, '/serve')));
});

gulp.task('build_blog', shell.task([
  'jekyll build'
], {cwd: conf.paths.blog_src}));

gulp.task('build_and_copy_blog', ['build_blog'], function() {
  return gulp.start('copy_blog_files');
});