rm -rf dist
GC_ENV=prod gulp build && firebase deploy -P prod
