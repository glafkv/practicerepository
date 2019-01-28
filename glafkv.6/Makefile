queue: Queue.o QueueSim.o
	gcc -o queue Queue.o QueueSim.o

Queue.o: Queue.c QueueTypes.h
	 gcc -c Queue.c

QueueSim.o: QueueSim.c QueueInterface.h
	 gcc -c QueueSim.c

.PHONY: clean
clean:
	/bin/rm -f *.o *~ queue 
