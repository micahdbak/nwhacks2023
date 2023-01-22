import socket

PORT = 8080



def transact(msg):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
    
    try:
        sock.connect(('', PORT))
    except:
        return b'fail'

    try:
        sock.sendall(msg.encode('ascii'))
    except:
        sock.close()

        return b'fail'

    rsp = sock.recv(1024)

    sock.close()

    return rsp


def retrieve ():
    data = get_data ()  # Idk, just parse raw data here
    posts = []
    for entry in data:
        thr = Thread (translate(entry))  # Idk, translate from your format to values
        posts.append(thr)

    return posts
