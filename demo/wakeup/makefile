TARGET = wakeup
SRCS = $(wildcard ./*.c)  
OBJS = $(patsubst %.c, %.o, $(SRCS))
HEADER_PATH = -I../../inc
LIBS = -L. ../../lib/libaiengine-127-linux-2.9.5-20180109140817.so ../../lib/libcjson.so ../../lib/libcjson_utils.so
LOG = *.log
$(TARGET) : $(OBJS)
	gcc -o $(TARGET) $(OBJS) $(LIBS)   

$(OBJS) : $(SRCS) 
	gcc $(HEADER_PATH) -c $(SRCS) 

.PHONY:clean
clean:
	-rm $(TARGET) $(OBJS) $(LOG)