""" api.py
Api definition for connecting to a frontend. Called with --api flag

Classes:

Authors:
    Keenan Miller

License:
    © 2022 BeyondRGB
    This code is licensed under the MIT license (see LICENSE.txt for details)
"""
import socket
import struct

port = 47382
address = ('127.0.0.1', port)


"""
Notes:
Do we need our non-cli "main method" here? - slightly unclear on reqs
I've tested receiving and sending messages from two python threads, will have
to work out some more of the architecture (and likely argument parsing) before
testing connection with the frontend.
"""


def setup(is_dev):
    """ setup
    [IN] is_dev     : are we in debug mode?
    """
    if is_dev:
        global port, address
        port = 9001
        address = ('127.0.0.1', port)


def receive_message():
    """ Read in message from frontend
    [OUT] data     : the data from the frontend
    """

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind(address)
        s.listen()
        conn, addr = s.accept()
        with conn:

            # handle length of message (length should be 4 bytes)
            raw_data_length = conn.recv(4)
            if not raw_data_length:
                return None
            data_length = struct.unpack('>I', raw_data_length)[0]

            data = bytearray()
            while len(data) < data_length:
                packet = conn.recv(data_length)
                if not packet:
                    return None
                data.extend(packet)

                return data.decode()
    s.close()


def send_message(message):
    """ Send message to the frontend
    [IN] message    : message to be sent
    """
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect(address)
        length = struct.pack('>I', len(message))
        s.sendall(length + message)

    s.close()


if __name__ == "__main__":
    receive_message()
