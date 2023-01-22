import socket

PORT = 8080

def transact(msg):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
    
    try:
        sock.connect(('', PORT))
    except:
        return b'failure'

    try:
        sock.sendall(msg.encode('ascii'))
    except:
        sock.close()

        return b'failure'

    rsp = sock.recv(1024)

    sock.close()

    return rsp
