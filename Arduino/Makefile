# ---------------------------------------------------------------------------------------
# Copyright 2014 by Bodo Bauer <bb@bb-zone.com>
#
# This file is part of I2C Arduino-Slave Library.
#
# The I2C Arduino Slave Library is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# The I2C Arduino Slave Library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
# --------------------------------------------------------------------------------------
all: install_arduino

ARDUINO_LIBRARY_PATH = "/Applications/Arduino 1.5.app/Contents/Resources/Java/libraries"
RPI_PROJECT_PATH     = "../RPI/ArduinoSlave"

install_arduino:
	mkdir -p ${ARDUINO_LIBRARY_PATH}/I2CSlave
	cp I2CSlave.cpp I2CSlave.h I2CSlaveCommon.h ${ARDUINO_LIBRARY_PATH}/I2CSlave
	mkdir -p ${ARDUINO_LIBRARY_PATH}/I2CSlave/examples/I2CSlave
	cp examples/I2CSlave/I2CSlave.ino ${ARDUINO_LIBRARY_PATH}/I2CSlave/examples/I2CSlave
	cp I2CSlaveCommon.h ${RPI_PROJECT_PATH}
