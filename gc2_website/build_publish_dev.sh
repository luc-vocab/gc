rm -rf dist
GC_ENV=dev gulp build && firebase deploy -P dev
