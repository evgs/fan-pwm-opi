# fan-pwm-opi

## How to use:

First run ```remove-old-fan-script.sh``` to uninstall old script fan.sh from crontab

```shell
$ cd ~
$ git clone https://github.com/evgs/fan-pwm-opi
$ cd fan-pwm-opi
$ ./install.sh
```


## How to configure

All settings placed in /etc/fan.cfg

```fan.cfg
# set pwm = 0  for on/off mode
# set pwm = 1  for linear mode
pwm = 1

# if temp <= min_temp, pwm will be 0
# if temp >= max_temp, pwm will be 100
min_temp = 50
max_temp = 70

# some fans requires min_pwm >= 20% to startup
min_pwm = 20
max_pwm = 100
```
## How to control temperature

Execute command

```watch -n 1 'cat /sys/class/thermal/thermal_zone0/temp' ```
Hit ^C to abort output

## How to debug PWM settings

```
$ sudo systemctl stop fan-pwm.service
$ cd ~/fan-pwm-opi
$ sudo ./fan-pwm
```

You will receive
```
CPU: 50°C | PWM: 0%
CPU: 50°C | PWM: 0%
CPU: 68°C | PWM: 92%
CPU: 68°C | PWM: 92%
CPU: 70°C | PWM: 100%
CPU: 70°C | PWM: 100%
CPU: 69°C | PWM: 96%
CPU: 68°C | PWM: 92%
CPU: 51°C | PWM: 24%
CPU: 49°C | PWM: 0%
CPU: 49°C | PWM: 0%
```
Hit ^C to abort output
