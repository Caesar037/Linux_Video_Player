target = test
src=$(wildcard *.c ./src/*.c)
obj=$(patsubst %.c, %.o, $(src))
include=-I ./include
flag=-pthread
cc=arm-linux-gcc

$(target):$(obj)
	$(cc) $^ $(flag) -o $@

%.o:%.c
	$(cc) $< -c $(include) -o $@

.PHONY:clean

clean:
	-rm $(obj) $(target) -f

