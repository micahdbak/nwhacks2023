import socket

PORT = 8080

class Thread:
    def __init__(self, _content, _author, _date):
        self.content = _content
        self.author = _author
        self.date = _date

    def get_content (self):
        return self.content

    def get_author (self):
        return self.author

    def get_date (self):
        return self.date


def transact(msg):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
    
    try:
        sock.connect(('', PORT))
    except:
        return

    try:
        sock.sendall(msg.encode('ascii'))
    except:
        sock.close()

        return

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