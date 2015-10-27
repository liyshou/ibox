CXX = g++
INCDIR= -I. -I$(HOME)/include -I$(HOME)/include/pubinc
TARGET = libkssoap.so
TARGETDIR=$(HOME)/lib/
OFLAGS= -fPIC -shared -o 

SOURCES = ks_soap.cpp soapC.cpp soapClient.cpp stdsoap2.cpp

$(TARGET):
	$(CXX) $(OFLAGS) $(TARGET) $(SOURCES)

install:
	cp $(TARGET) $(TARGETDIR)
	cp *.h $(HOME)/include/pubinc
clean:
	rm -f $(TARGET)
	rm -rf *.o                                                                                                                                                    
