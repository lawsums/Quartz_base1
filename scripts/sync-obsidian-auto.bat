@echo off
REM sync-obsidian-auto.bat - Windows Task Scheduler wrapper for sync-obsidian.sh
REM Sets PATH so that node/npx/git are available inside Git Bash

set PATH=G:\Git\bin;G:\Git\usr\bin;G:\MyPath\Nodejs;%PATH%
cd /d G:\MyPath\quartz
"G:\Git\bin\bash.exe" scripts/sync-obsidian.sh
