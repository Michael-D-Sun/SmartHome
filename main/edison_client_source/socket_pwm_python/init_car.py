#!/usr/bin/python

import mraa
import time
import socket
import commands
import threading
PWM_PIN_IN1 = 3
PWM_PIN_IN2 = 5
PWM_PIN_IN3 = 6
PWM_PIN_IN4 = 9
pwm1 = mraa.Pwm(PWM_PIN_IN1)
pwm2 = mraa.Pwm(PWM_PIN_IN2)
pwm3 = mraa.Pwm(PWM_PIN_IN3)
pwm4 = mraa.Pwm(PWM_PIN_IN4)



pwm1.write(0);
pwm2.write(0);
pwm3.write(0);
pwm4.write(0);

