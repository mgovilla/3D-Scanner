import serial
import tkinter.ttk as tkk



def main():
    hc = serial.Serial('COM12', 9600, timeout=1)
    hc.write(b'0')
    print(hc.read())

if __name__ == "__main__":
    main()
