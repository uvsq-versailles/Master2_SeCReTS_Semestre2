all:
	g++ -m32 ./proto.o ./libproto3.so -lcrypto -o proto

attack:
	gcc -shared -o attack.so attack.c -m32
	LD_PRELOAD=./attack.so ./proto
