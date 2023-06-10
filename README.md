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
