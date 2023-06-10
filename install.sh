die() { echo "$*" 1>&2 ; exit 1; }

gcc -o fan-pwm fan-pwm.c -lwiringPi  || die "Compiling failed"

sudo systemctl stop fan-pwm.service
sudo systemctl disable fan-pwm.service

sudo cp fan-pwm /usr/local/bin || die "Cannot install binary fan-pwm"

sudo cp fan-pwm.service /etc/systemd/system/  || die "Cannot add fan-pwm service"
sudo systemctl enable fan-pwm.service
sudo systemctl start fan-pwm.service




