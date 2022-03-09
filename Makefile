TARGET := nvml-example

all:
	gcc -o ${TARGET} main.c -I/usr/local/cuda/include -L/usr/local/cuda/lib64 -lnvidia-ml

clean:
	rm -f ${TARGET}
