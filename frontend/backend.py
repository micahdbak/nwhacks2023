import socket

PORT = 8080

def transact(msg):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
    sock.connect(('', PORT))

    try:
        sock.sendall(msg.encode('ascii'))
    except:
        sock.close()

        return

    rsp = sock.recv(1024)

    sock.close()

    return rsp
