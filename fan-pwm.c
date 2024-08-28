// Based on https://github.com/bastilmr/opi5-fan-control
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <signal.h>
#include <string.h>
#include "parser.h"


int FAN_PIN = 10;
int MIN_TEMP = 50;
int MIN_HYST = 5;
int MAX_TEMP = 70;
int MIN_PWM = 20;
int MAX_PWM = 100;
int PWM_EN = 1;

int DBGOUT = 0;

#define CPU_TEMP_FILE "/sys/class/thermal/thermal_zone0/temp"

void sigkill(int sig){
    softPwmWrite(FAN_PIN, 100);
    exit(0);
}
void sigint(int sig){
    softPwmWrite(FAN_PIN, 0);
    exit (0);
}

void read_config(const char *cfg_path) {
    config_option_t co;
    
    if ((co = read_config_file(cfg_path)) == NULL) {
        fprintf(stderr, "Can't read configuration (%s)", cfg_path);
        exit (0);
    }
    while(1) {
        if (strcmp(co->key, "min_temp")==0) MIN_TEMP = atoi(co->value);
        if (strcmp(co->key, "min_hyst")==0) MIN_HYST = atoi(co->value);
        if (strcmp(co->key, "max_temp")==0) MAX_TEMP = atoi(co->value);
        if (strcmp(co->key, "min_pwm")==0) MIN_PWM = atoi(co->value);
        if (strcmp(co->key, "max_pwm")==0) MAX_PWM = atoi(co->value);
        if (strcmp(co->key, "fan_pin")==0) FAN_PIN = atoi(co->value);
        if (strcmp(co->key, "pwm")==0) PWM_EN = atoi(co->value);
        if (strcmp(co->key, "debug")==0) DBGOUT = atoi(co->value);
        //printf("Key: %s\nValue: %s\n", co->key, co->value);
        if (co->prev != NULL) {
            co = co->prev;
        } else {
            break;
        }
    }
    
}

int main() {
    int temp;
    float temp_ratio, pwm_ratio;
    int pwm_value = 0;
 
    read_config("/etc/fan.cfg");
    
    
    if (wiringPiSetup() == -1) {
        printf("wiringPiSetup failed\n");
        return 1;
    }

    signal(SIGINT, sigint);
    signal(SIGKILL, sigkill);
    
    pinMode(FAN_PIN, OUTPUT);
    softPwmCreate(FAN_PIN, 0, MAX_PWM);
    
    while(1) {
        FILE* temp_file = fopen(CPU_TEMP_FILE, "r");
        
        if (temp_file == NULL) {
            printf("Failed to read CPU temperature file\n");
            return 1;
        }
        
        fscanf(temp_file, "%d", &temp);
        fclose(temp_file);
        
        temp /= 1000;
        
        if (temp <= MIN_TEMP) {
            pwm_value = 0;
        }
        
        if ((temp <= MIN_TEMP + MIN_HYST) && (pwm_value == 0)) {
            // hold pwm off if below threshold
            pwm_value = 0;
        } else if (PWM_EN) {
            temp_ratio = (float)(temp - MIN_TEMP) / (MAX_TEMP - MIN_TEMP);
            pwm_ratio = temp_ratio * (MAX_PWM - MIN_PWM);
            pwm_value = (int)(pwm_ratio + MIN_PWM);
        }

        if (temp >= MAX_TEMP) {
            pwm_value = MAX_PWM;
        }
        
        softPwmWrite(FAN_PIN, pwm_value);
        
	if (DBGOUT)
	    printf("CPU: %d°C | PWM: %d%%\n", temp, pwm_value);
        
        sleep(5);
    }
    
    return 0;
}
