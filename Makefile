obj: orvibo_cmd.h orvibo_remote_control.h orvibo_socket.h
	gcc -g -Wall main.c orvibo_cmd.c  orvibo_cmd.h orvibo_socket.c -o zyc
clean:
	rm -rf t
	rm -rf core.*
