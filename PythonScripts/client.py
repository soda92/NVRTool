import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
import argparse

parser = argparse.ArgumentParser(description="Process commands.")
parser.add_argument("command", help="show or hide progress window")
program_args = parser.parse_args()

try:
    s.settimeout(.1)
    s.connect(("localhost", 55555))
    if program_args.command == "show":
        s.sendall(b"show")
        s.close()
    elif program_args.command == "hide":
        s.sendall(b"hide")
        s.close()
except Exception as e:
    pass
