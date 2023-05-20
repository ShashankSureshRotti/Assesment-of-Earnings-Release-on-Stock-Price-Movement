CC = g++
CFLAGS = -Wall -ggdb3 -std=c++11 -lcurl -pthread

Menu: Menu.o DataExtraction.o Calculation.o gnuplot.o
	$(CC) $(CFLAGS) -o Menu Menu.o DataExtraction.o Calculation.o gnuplot.o

Menu.o: Menu.cpp DataExtraction.h Stocks.h  Calculation.h Group.h Bootstrap.h
	$(CC) $(CFLAGS) -c Menu.cpp

Calculation.o: Calculation.h Calculation.cpp
	$(CC) $(CFLAGS) -c Calculation.cpp

DataExtraction.o: DataExtraction.h DataExtraction.cpp Stocks.h
	$(CC) $(CFLAGS) -c DataExtraction.cpp

gnuplot.o: gnuplot.cpp gnuplot.h 
	$(CC) $(CFLAGS) -c gnuplot.cpp	
	
clean:
	rm -rf Menu *.o


