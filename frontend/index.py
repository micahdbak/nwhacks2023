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
        root = backend.transact('list').decode()
        threads = []

        print(f'Got\n{root}')

        path = ""

        for row in root.split('\n'):
            column = row.split(' ');

            if len(column) < 3:
                continue;

            print(f'Operating on row {row}')

            column  = row.split(' ');
            type    = int(column[0])
            content = ""
            date    = ""

            # type is either a comment or a post
            if type == 0 or type == 1:
                content = backend.transact(f'view {path}{column[2]}').decode()
                date    = column[2]

            author  = column[1]

            # type is a thread
            if type == 2:
                content = column[2]
                date    = column[3]

            threads.append(Thread(type, content, author, date))

        if request.method == 'POST':
            json = request.get_json()

            msg = json['msg']

            print(f'Got message {msg}')

            return {}

        return render_template('index.html', threads=threads)
