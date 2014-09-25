obj: orvibo_cmd.h orvibo_remote_control.h orvibo_socket.h orvibo_util.h orvibo.h
	gcc -g main.c orvibo_cmd.c  orvibo_cmd.h orvibo_socket.c orvibo_util.c orvibo_remote_control.c orvibo.c -o zyc1
clean:
	rm -rf t
	rm -rf core.*
	rm -rf core
	rm -rf or
	rm -rf zyc
	rm -rf zyc1
