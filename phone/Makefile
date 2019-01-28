CC	= gcc
CFLAGS	= -g
TARGET	= phone
OBJS	= phone.o phone_fmt.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGETS) $(OBJS)

phone.o: phone.c phone_fmt.h
	$(CC) $(CFLAGS) -c phone.c

phone_fmt.o: phone_fmt.c phone_fmt.h
	$(CC) $(CFLAGS) -c phone_fmt.c

clean:
	/bin/rm -f *.o $(TARGET)
