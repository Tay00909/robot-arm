import struct, serial, time
PORT = "/dev/ttyACM0"      # may be ttyUSB0 on older Arduinos # used to connect to rasberry/arduino
BAUD = 115200
# cached target angles in µs

pulse = {0: 1500, 1: 1500, 2: 1500} #that setting how long each servo should move dictionarry

def send(id_, us): # sends info to UART(onto arduino)
    """Pack 3-byte command and write to UART."""
    pkt = struct.pack("<BH", id_, us)
    ser.write(pkt)
def parse_arduino_line(line): # takes output from arduino and turns it ointo a range to show exactly how far it is from the ultrasonic sensor

    if line.startswith("R"):

        mm = int(line[1:])

        print(f"Range: {mm} mm")
ser = serial.Serial(PORT, BAUD, timeout=0.1)
try:
    while True: #how to control wrist 
        # demo: sweep wrist
        pulse[1] += 10 #slightly moves each time
        if pulse[1] > 2300: pulse[1] = 700
        send(1, pulse[1])
        # read back sensor lines
        for _ in range(4):# reading sensor output and hunting for info each time looking for range distance between sensoir and the object
            line = ser.readline().decode().strip()
            if line: parse_arduino_line(line)
        time.sleep(0.02)
except KeyboardInterrupt:
    ser.close()