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
    print "start_run"
    delta = 0.1
    pwm2.write(0)
    pwm4.write(0)
    while 1:
        if(value < 1):
            value = value + delta
            time.sleep(0.1)
            pwm1.write(value)
            pwm3.write(value)
            if(value == 1):
                break
    print "done"

def running(speed):
    global value
    global delta              
    global pwm1
    global pwm2
    global pwm3
    global pwm4
    pwm2.write(0)
    pwm4.write(0)
    while 1:
        if(speed < value):
            value = value + 0.05 
            pwm1.write(value)
            pwm2.write(value)
            time.sleep(0.1)
            if(value == speed):
                break
        elif(speed >value):
            value = value - 0.05
            pwm1.write(value)
            pwm2.write(value)
            time.sleep(0.1)
            if(value == speed):
                break
    print "running done"       
        
def left(speed):
    print "right"
    global value
    global delta              
    global pwm1
    global pwm2
    global pwm3
    global pwm4
    pwm1.write(0)
    pwm2.write(0)
    pwm4.write(0)
    while 1:
        value = value + 0.05
        pwm3.write(value)
        time.sleep(0.1)
        if(value == speed):
            break
       
def right(speed):
    global value
    global delta              
    global pwm1
    global pwm2
    global pwm3
    global pwm4
    pwm2.write(0)
    pwm3.write(0)
    pwm4.write(0)
    while 1:
        value = value + 0.05
        pwm1.write(value)
        time.sleep(0.1)
        if(value == speed):
            break

def slow_down(speed):
    global value
    global delta              
    global pwm1
    global pwm2
    global pwm3
    global pwm4
    if(speed < value):
        while 1:
            value = value + 0.05
            pwm1.write(value)
            time.sleep(0.1)
            if(value == speed):
                 break
    



def back(speed):
    global value
    global delta              
    global pwm1
    global pwm2
    global pwm3
    global pwm4
    pwm1.write(0)
    pwm3.write(0)
    while 1:
        value = value + 0.05
        if(value == speed):
            break
        pwm2.write(value)
        pwm4.write(value)
        time.sleep(0.1)
       
class M_pthread(threading.Thread):
    def __init__(self,HOST,PORT):
        threading.Thread.__init__(self)
        self.thread_stop = False
        self.HOST = HOST
        self.PORT = PORT
    def run(self):
        s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        s.bind((self.HOST,self.PORT))
        s.listen(1)
        while True:
            confd,addr=s.accept()
            print "accept sucess"
            while True:
                data = confd.recv(1024)
                print data
                if data == "frist":
                    confd.send("welcome to server!")
                elif data == "start":
                    print "recv start"
                    start_run()
                elif data == "forword":
                    print "recv forword"
                    running(1)
                elif data == "backword":
                    print "recv backword"
                    back(1)
                elif data == "right":
                    print "recv right"
                    right(1)
                elif data == "left":
                    print "recv left"
                    left(1)
                elif data == "stop":
                    print "recv stop"
                    running(0)
                elif not len(data):
                    confd.close()
                    break;
                else:
                    print"shit things"

        print "pthread running"

    def stop(self):
        self.thread_stop = True
        print "pthread stopped"

def main():
    print "main"
    test = M_pthread("192.168.1.111",5000)
    test.run()




if __name__ == "__main__":
    main()
