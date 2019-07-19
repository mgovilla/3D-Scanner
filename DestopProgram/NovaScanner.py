from tkinter import *
import serial
import tkinter.ttk as tkk



def main():
    master = Tk()

    v = IntVar()

    Radiobutton(master, text="One", variable=v, value=1).pack(anchor=W)
    Radiobutton(master, text="Two", variable=v, value=3).pack(anchor=W)
    Radiobutton(master, text="Three", variable=v, value=5).pack(anchor=W)
    
    mainloop()

if __name__ == "__main__":
    main()
