client:
	gcc -o client client.c

client.o: client.c
	gcc -c client.c 

clean: 
	rm client
