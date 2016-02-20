var path = require('path');
var gulp = require('gulp');
var shell = require('gulp-shell');
var conf = require('./conf');


gulp.task('copy_blog_files', function() {
  return gulp.src(path.join(conf.paths.blog_site, '/**/*'))
  .pipe(gulp.dest(path.join(conf.paths.tmp, '/serve')));
});

gulp.task('build_blog_dev', shell.task([
  'jekyll build --drafts'
], {cwd: conf.paths.blog_src}));


gulp.task('build_blog_prod', shell.task([
  'jekyll build'
], {cwd: conf.paths.blog_src}));

gulp.task('build_blog', function() {
  var blog_env = process.env.ENV;
  if(!blog_env) {
    throw "ENV not set [ENV=dev]";
  }  
  
  if(blog_env == "dev") {
    return gulp.start('build_blog_dev');
  } else {
    return gulp.start('build_blog_prod');
  }
  
})

gulp.task('build_and_copy_blog', ['build_blog'], function() {
  return gulp.start('copy_blog_files');
});