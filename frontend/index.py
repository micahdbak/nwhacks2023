from flask import Flask, render_template, request
from . import backend

def index(app):
    @app.route('/index.html', methods=['GET','POST'])
    def route_index():
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
