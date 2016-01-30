# build image
docker build -t gc_server .
# run image
docker run -e "FB_ROOT=gcserver-dev.firebaseio.com" -e "SERVER_KEY=hk_dev" -it --rm --name gc_server_dev_hk gc_server

