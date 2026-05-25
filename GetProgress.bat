@echo off
call BuildVC6Release
call .venv\Scripts\activate
cd ./build/bin/vc6/x32/RelWithDebInfo
reccmp-reccmp --target DELAYLAMA --html ../../../../../progress.html --silent
cd ../../../../../
call .venv\Scripts\deactivate