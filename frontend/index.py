from flask import Flask, render_template, request
from . import backend

class Thread:
    def __init__(self, _type, _content, _author, _date, _index):
        self.type    = _type
        self.content = _content
        self.author  = _author
        self.date    = _date
        self.index   = _index

def index(app):
    @app.route('/', methods=['GET','POST'])
    @app.route('/<path:filename>', methods=['GET','POST'])
    def route_index(filename=None):
        path = ""

        if filename is not None:
            path = filename.replace("~", "/")

            print(path)

        root = backend.transact(f'list {path}').decode()

        if root == 'failure':
            return render_template('404.html')
        elif root == 'empty':
            root = ''

        print(backend.transact(f'view {path}').decode())

        threads = []

        # Each row is na individual entry in form of 'type author content` for folders
        for row in root.split('\n'):
            column = row.split('~')

            if len(column) < 3:
                continue

            print(f'Operating on row {row}')

            column = row.split('~')
            type = int(column[0])
            content = ""
            epoch = ""
            index = ""

            # type is either a comment or a post
            if type == 0 or type == 1:
                content = backend.transact(f'view {path}/{column[2]}').decode()
                print(f"Got post content {content}")
                epoch = column[2]
                index = epoch

            author = column[1]

            # type is a thread
            if type == 2:
                content = column[2]
                epoch = column[3]
                index = content

            thread_path = ""

            threads.append(Thread(type, content, author, epoch, index))

        if request.method == 'POST':
            json = request.get_json()

            author = json['author']
            content = json['content']

            response = backend.transact(f'post {path} {author}')

            if response != 'failure':
                backend.transact(content)

            return {}

        return render_template('index.html', path=path, threads=threads)

# Converts replies from list command into the form of: type~content~author~epoch
def format_reply (cmd):
    reply = backend.transact(cmd).decode()
    # Default is for Folders that come in defined form
    formatted = reply

    # For posts and comments that come in form type~author~epoch
    if reply[0] != '2':
        formatted = reply[0] + '~' + reply[1:]

    return formatted
