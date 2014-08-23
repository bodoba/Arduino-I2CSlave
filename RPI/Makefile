
all: I2CSlave

I2CSLAVE_OBJS = I2CSlave.o ArduinoSlave/ArduinoSlave.o
I2CSLAVE_HDRS = ArduinoSlave/I2CSlaveCommon.h ArduinoSlave/ArduinoSlave.h 

CXXFLAGS = -Wall -g -O2 

$(I2CSLAVE_OBJS): $(I2CSLAVE_HDRS)

I2CSlave: $(I2CSLAVE_OBJS)
	$(CXX) -o $@ $^ -lm

clean:
	rm -f $(I2CSLAVE_OBJS) I2CSlave *~
