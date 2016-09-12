var path = require('path');
var gulp = require('gulp');
var shell = require('gulp-shell');
var runSequence = require('run-sequence');
var spawn = require('child_process').spawn;
var conf = require('./conf');


gulp.task('copy_blog_files', function() {
  return gulp.src(path.join(conf.paths.blog_site, '/**/*'))
  .pipe(gulp.dest(path.join(conf.paths.tmp, '/serve')));
});


gulp.task('build_blog', function(callback) {
  var blog_env = process.env.ENV;
  if(!blog_env) {
    throw "ENV not set [ENV=dev]";
  }  
  
  if(blog_env == "dev") {
    var cmd = spawn('jekyll', ['build', '--drafts'], {stdio: 'inherit', cwd: conf.paths.blog_src});
    cmd.on('close', function (code) {
      callback();
    });    
  } else {
    var cmd = spawn('jekyll', ['build'], {stdio: 'inherit', cwd: conf.paths.blog_src});
    cmd.on('close', function (code) {
      callback();
    });    
  }
  
})


gulp.task('build_and_copy_blog', function(callback) {
  runSequence('build_blog',
              'copy_blog_files',
              'copy_misc_blog_files',
              callback);
})

gulp.task('copy_misc_blog_files', function() {
  return gulp.src(path.join(conf.paths.blog_site, '/feed.xml'))
     .pipe(gulp.dest(path.join(conf.paths.dist, '/')))
});