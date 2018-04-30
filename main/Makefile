CPP_SRC=main.cpp
#C_SRC1=smart_home_beep.c 
#C_SRC2=smart_home_led.c
CPP_SRC2=smart_home_socket.cpp
OBJ=run
LDDIR=./LDDIR
HEAD_PATH:=$(shell pwd)/include
#CC=gcc
#CPP_CC=g++
CC=arm-none-linux-gnueabi-gcc
CPP_CC=arm-none-linux-gnueabi-g++

CPP_O:=$(basename $(CPP_SRC)).o
CPP_O2:=$(basename $(CPP_SRC2)).o
C_O1:=$(basename $(C_SRC1)).o
C_O2:=$(basename $(C_SRC2)).o
ALL:$(CPP_O2) $(CPP_O) 
	$(CPP_CC) *.o -o $(OBJ) -I $(HEAD_PATH) -lpthread 
	sudo cp $(OBJ) /rootfs/testbin/
	

$(CPP_O):$(CPP_SRC)
	$(MAKE) -C ./socket/ 
	$(MAKE) -C ./device/ 
	$(MAKE) -C ./msg_queue_thread/
	$(CPP_CC) -c $^ -o $@ -I $(HEAD_PATH) -lpthread 
$(CPP_O2):$(CPP_SRC2)
	$(CPP_CC) -c $^ -o $@ -I $(HEAD_PATH) 
#$(C_O1):$(C_SRC1)
#	$(CC) -c $^ -o $@ -I $(HEAD_PATH) 
#$(C_O2):$(C_SRC2)
#	$(CC) -c $^ -o $@ -I $(HEAD_PATH) 

clean:
	rm *.o $(OBJ) 

