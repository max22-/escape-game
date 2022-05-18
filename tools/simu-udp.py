import tkinter as tk
import threading
import socket
from functools import partial

port = 1234

class App(tk.Tk):
    def __init__(self):
        super().__init__()
        self.idx_labels = []
        self.rx_labels = []
        self.tx_buttons = []
        self.ip_entry = tk.Entry()
        self.ip_entry.grid(row=0, column=0)
        self.start_button = tk.Button(text="start", command=self.start)
        self.start_button.grid(row=0, column=1)
        for i in range(16):
            self.idx_labels.append(tk.Label(
                text=str(i),
                font=("Arial", 25),
                foreground="white",
                background="black"))
            self.idx_labels[i].grid(row=1, column=i)
            self.rx_labels.append(tk.Label(
                text="0",
                font=("Arial", 25),
                foreground="white",
                background="black"))
            self.rx_labels[i].grid(row=2, column=i)
            self.tx_buttons.append(tk.Button(
                text="0",
                command=partial(self.tx, i)))
            self.tx_buttons[i].grid(row=3, column=i)
        self.tx_state = 0
        self.rx_state = 0

    def start(self):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.bind(("0.0.0.0", 1234))
        self.receive_thread = threading.Thread(target=self.receive)
        self.receive_thread.start()

    def tx(self, i):
        self.tx_state ^= 1 << i
        for j in range(16):
            self.tx_buttons[j]["text"] = str((self.tx_state & (1 << j)) >> j)
        msg = self.tx_state.to_bytes(2, 'little')
        print(f"sending {msg}")
        self.sock.sendto(msg, (self.ip_entry.get(), 1234))
        print(self.tx_state)

    def receive(self):
        while(True):
            data, addr = self.sock.recvfrom(5)
            print(f"Received: {data} from {addr}")
            data_int = int.from_bytes(data, 'little')
            for i in range(16):
                self.rx_labels[i]["text"] = str((data_int & (1 << i)) >> i)
            print()

App().mainloop()
            
