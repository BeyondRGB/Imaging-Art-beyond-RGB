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

s = None
conn = None
addr = None

def setup(is_dev):
    """ setup
    [IN] is_dev     : are we in debug mode?
    """
    if is_dev:
        global port, address
        port = 9002
        address = ('127.0.0.1', port)

        global s
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.bind(address)
        s.listen()
        global conn, addr
        print('accepting')
        conn, addr = s.accept()
        print('accepted')


def receive_message():
    """ Read in message from frontend
    [OUT] data     : the data from the frontend
    """


    print('checking socket for message')
    global conn, addr
    with conn:

        # handle length of message (length should be 4 bytes)
        if _is_socket_closed():
            conn, addr = s.accept()

        raw_data_length = None
        try:
            raw_data_length = conn.recv(4)
        except:
            return None
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

    length = bytes(len(message), 'utf-8')
    s.sendall(length + bytes(message, 'utf-8'))


def _is_socket_closed():
    try:
        # this will try to read bytes without blocking and also without removing them from buffer (peek only)
        data = s.recv(16, socket.MSG_DONTWAIT | socket.MSG_PEEK)
        if len(data) == 0:
            return True
    except BlockingIOError:
        return False  # socket is open and reading from it would block
    except ConnectionResetError:
        return True  # socket was closed for some other reason
    except Exception as e:
        print("unexpected exception when checking if a socket is closed")
        return False
    return False


if __name__ == "__main__":
    receive_message()
