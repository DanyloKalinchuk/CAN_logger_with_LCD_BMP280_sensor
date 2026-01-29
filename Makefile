final: cpp_can_logger

can.o : src/can/can.cpp
	g++ -c src/can/can.cpp -o can.o

logger.o : src/logger/logger.cpp
	g++ -c src/logger/logger.cpp -o logger.o

timer.o : src/timer/timer.cpp
	g++ -c src/timer/timer.cpp

hd44780.o : src/lcd/4_bit/hd44780_4bit.cpp
	g++ -c src/lcd/4_bit/hd44780_4bit.cpp -o hd44780.o

bmp_lcd.o : src/lcd/bmp_lcd.cpp
	g++ -c src/lcd/bmp_lcd.cpp -o bmp_lcd.o

bmp_c.o : src/bmp/bmp_c/bmp.c
	gcc -c src/bmp/bmp_c/bmp.c -o bmp_c.o

bmp.o : src/bmp/bmp.cpp src/bmp/bmp_c/bmp.c
	g++ -c src/bmp/bmp.cpp -o bmp.o

main.o : src/main.cpp
	g++ -c src/main.cpp -o main.o

cpp_can_logger : can.o logger.o timer.o bmp_lcd.o hd44780.o bmp.o bmp_c.o main.o
	g++ can.o logger.o timer.o bmp_lcd.o hd44780.o bmp.o bmp_c.o main.o -o cpp_can_logger -li2c -lgpiod

clean:
	rm -f *.o

