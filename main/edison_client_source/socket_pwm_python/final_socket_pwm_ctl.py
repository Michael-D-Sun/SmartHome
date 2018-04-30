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

pwm1.period_us(5000)
pwm2.period_us(5000)
pwm3.period_us(5000)
pwm4.period_us(5000) # Set the period as 5000 us or 5ms

pwm1.enable(True)
pwm2.enable(True)
pwm3.enable(True)
pwm4.enable(True)# enable PWM

value = 0
delta = 0.5


def start_run():
    global value
    global delta              
    global pwm1
    global pwm2
    global pwm3
    global pwm4
    pwm1.write(1)
    pwm2.write(0)
    pwm3.write(1)
    pwm4.write(0)
    time.sleep(0.5)
    print "done"

def running(speed):
    global value
    global delta              
    global pwm1
    global pwm2
    global pwm3
    global pwm4
    pwm1.write(1)
    pwm2.write(0)
    pwm3.write(1)
    pwm4.write(0)
    time.sleep(0.5)
    print "running done"       
        
def left(speed):
    global value
    global delta              
    global pwm1
    global pwm2
    global pwm3
    global pwm4
    pwm1.write(0)
    pwm2.write(1)
    pwm3.write(1)
    pwm4.write(0)
    time.sleep(0.5)
    print "left done"       
       
def right(speed):
    global value
    global delta              
    global pwm1
    global pwm2
    global pwm3
    global pwm4
    pwm1.write(1)
    pwm2.write(0)
    pwm3.write(0)
    pwm4.write(1)
    time.sleep(0.5)

    print "right done"

def stop(speed):
    global pwm1
    global pwm2
    global pwm3
    global pwm4
    pwm1.write(0)
    pwm2.write(0)
    pwm3.write(0)
    pwm4.write(0)
    time.sleep(0.5)
    print "stop done"


def back(speed):
    global value
    global delta              
    global pwm1
    global pwm2
    global pwm3
    global pwm4
    pwm1.write(0)
    pwm2.write(1)
    pwm3.write(0)
    pwm4.write(1)
    time.sleep(0.5)
    print "back done"

class M_pthread(threading.Thread):
    def __init__(self,HOST,PORT):
        threading.Thread.__init__(self)
        self.thread_stop = False
        self.HOST = HOST
        self.PORT = PORT
        print "pthread init"
    def run(self):
        print "run init"
        s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        s.bind((self.HOST,self.PORT))
        s.listen(1)
        while True:
            print "ready to accept connect"
            confd,addr=s.accept()
            print "accept suceess"
            while True:
                print "while 2 2"
                data = confd.recv(1024)
                print data
                if data == "forward":
                    running(1)
                elif data == "backward":
                    back(1)
                elif data == "left":
                    left(1)
                elif data == "right":
                    right(1)
                elif data == "start":
                    running(1)
                elif data == "stop":
                    stop(1)
                elif 0 == len(data):
                    confd.close()
                    break
                else :
                    continue
                
    def stop(self):
        self.thread_stop =True
        print "pthread stopped"

    



def main():
    print "main"
    stop(1)
    time.sleep(5)
    test = M_pthread("192.168.1.111",5000)
    test.run()


if __name__ == "__main__":
    main()
