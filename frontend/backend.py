import socket

PORT = 8080
sock = []

def get_socket():
    sock.append(socket.socket(socket.AF_INET, socket.SOCK_STREAM))
    sock[0].connect(('', PORT))

def send(arr):
    if len(sock) == 0:
        get_socket()

    b = arr.encode('ascii')
    try:
        sock[0].sendall(b)
    except:
        print('EXCEPTION')

def receive():
    if len(sock) == 0:
        get_socket()

    r = sock[0].recv(1024)

    return r

def transact(msg):
    print('1')
    send(msg)
    print('2')

    return receive()
