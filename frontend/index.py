from flask import Flask, render_template, request
from . import backend

class Thread:
    def __init__(self, _type, _content, _author, _date):
        self.type    = _type
        self.content = _content
        self.author  = _author
        self.date    = _date

def index(app):
    @app.route('/', methods=['GET','POST'])
    def route_index():
        threads = init_threads()

        if request.method == 'POST':
            json = request.get_json()

            print(f'Got message {json["msg"]}')

            return {
                # Replies a string formatted to type~content~author~epoch
                'reply' : format_reply(json['cmd'])
            }

        return render_template('index.html', threads=threads)

# Retrieves the threads at depth 1, aka the main folders
def init_threads ():
    root = backend.transact('list').decode()
    threads = []

    print(f'Got\n{root}')

    path = ""

    # Each row is na individual entry in form of 'type author content` for folders
    for row in root.split('\n'):
        column = row.split('~')

        if len(column) < 3:
            continue

        print(f'Operating on row {row}')

        column = row.split('~')
        type = int(column[0])
        content = ""
        date = ""

        # type is either a comment or a post
        if type == 0 or type == 1:
            content = backend.transact(f'view {path}{column[2]}').decode()
            date = column[2]

        author = column[1]

        # type is a thread
        if type == 2:
            content = column[2]
            date = column[3]

        threads.append(Thread(type, content, author, date))

    return threads

# Converts replies from list command into the form of: type~content~author~epoch
def format_reply (cmd):
    reply = backend.transact(json["cmd"]).decode()
    # Default is for Folders that come in defined form
    formatted = reply

    # For posts and comments that come in form type~author~epoch
    if reply[0] != '2':
        formatted = reply[0] + '~' + reply[1:]

    return formatted