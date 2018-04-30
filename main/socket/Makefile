SRC:=*.c
CC=arm-none-linux-gnueabi-gcc
#CC=gcc
HEAD_PATH=-I ../include/
SRC_O:=$(foreach i,$(SRC),$(basename $(i)).o)


ALL:$(SRC_O)
	mv $^ ../ 

$(SRC_O):$(SRC)
	echo $(SRC)
	$(foreach n,$^,$(shell $(CC) -c $(n) $(HEAD_PATH) -o $(basename $(n)).o))
