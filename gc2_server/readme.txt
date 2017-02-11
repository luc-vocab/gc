# running gc server
FB_CONFIG_FILE=./sleeptrack-dev.json FB_ROOT=sleeptrack-dev SERVER_KEY=hk_dev SERVER_TYPE=gc_server nodejs gc_server_launcher.js
# running gc data server
FB_CONFIG_FILE=./sleeptrack-dev.json FB_ROOT=sleeptrack-dev SERVER_TYPE=gc_data_server nodejs gc_server_launcher.js
