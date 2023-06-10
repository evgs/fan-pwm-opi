#!/bin/sh

# This script will remove all cron tasks running fan.sh

crontab -l > /tmp/crontab-tasks
awk '!/fan\.sh/' /tmp/crontab-tasks > /tmp/crontab-upd
crontab /tmp/crontab-upd


