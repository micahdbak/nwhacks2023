from flask import Flask, render_template, request
from . import backend

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

def index(app):
    @app.route('/index.html', methods=['GET','POST'])
    def route_index():
        root = backend.transact('list').decode()
        threads = []

        for row in root.split('\n'):
            column = row.split(' ');
            thread = Thread()

        if request.method == 'POST':
            json = request.get_json()



            print(backend.transact('list').decode())
            print(backend.transact('list CMPT').decode())
            post = backend.transact('list CMPT/Hello').decode()
            print(post)

            id = (post.split(' '))[2].replace("\n", "")

            print(backend.transact(f'view CMPT/Hello/{id}').decode())

            id = (backend.transact('post CMPT/Hello micah').decode())
            print(id)
            print(backend.transact('Hello, world! This is the post content for my post.').decode())

            print(backend.transact(f'view CMPT/Hello/{id}').decode())

            return {
                'link': '/index.html'
            }

        return render_template('index.html')
