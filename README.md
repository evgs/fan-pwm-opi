# fan-pwm-opi

Сервис управления оборотами вентилятора охлаждения одноплатного компьютера Orange Pi3 LTS.

Скорость вращения определяется пороговыми значениями температуры, заданными в файле конфигурации.
Поддерживается как линейное увеличение оборотов (PWM), так и релейное включение/выключение вентилятора.

Используется softPWM, возможно подключение вентилятора к любому GPIO

## Требования:
- Вентилятор должен быть подсоединён к одноплатнику через управляющий транзисторный ключ, (по умолчанию управление через пин PD16)
- ИЛИ наличие кросс-платы https://github.com/evgs/kross-pi-hat
- Установленная библиотека [WiringOP](https://github.com/orangepi-xunlong/wiringOP)

## Установка:

Далее выполнить команды:
```shell
cd ~
git clone https://github.com/evgs/fan-pwm-opi
cd fan-pwm-opi
./install.sh
```
Если ранее был настроен скрипт fan.sh с запуском через crontab, потребуется его отключить, например, скриптом 
```shell 
./remove-old-fan-script.sh
```


## Настройка

Файл конфигурации расположен в ```/etc/fan.cfg```, редактируется с правами суперпользователя

```fan.cfg
# set pwm = 0  for on/off mode
# set pwm = 1  for linear mode
pwm = 1

# if temp <= min_temp, pwm will be 0
# if temp >= max_temp, pwm will be 100
min_temp = 50
max_temp = 70

# fan will be ON after temp will be higher min_temp+min_hyst
min_hyst=5

# some fans requires min_pwm >= 20% to startup
min_pwm = 20
max_pwm = 100
```

После редактирования требуется перезапуск сервиса:
```
$ sudo systemctl stop fan-pwm.service
$ sudo systemctl start fan-pwm.service
```

## Наблюдение за температурой

Может быть полезно при тонкой настройке порогов. Выполнить команду 

```watch -n 1 'cat /sys/class/thermal/thermal_zone0/temp' ```

Нажать ^C для прерывания команды watch

## Отладочный запуск для наблюдения за величиной PWM

Сервис потребуется остановить, а затем запустить для вывода значений в терминал:
```
$ sudo systemctl stop fan-pwm.service
$ cd ~/fan-pwm-opi
$ sudo ./fan-pwm
```

В терминале можно наблюдать такие значения
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
Нажать ^C для прерывания команды, затем перезапустить сервис
```
$ sudo systemctl start fan-pwm.service
```
