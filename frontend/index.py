from flask import Flask, render_template, request
from . import backend

def index(app):
    @app.route('/index.html', methods=['GET','POST'])
    def route_index():
        if request.method == 'POST':
            json = request.get_json()
            print('hello')
            data = backend.transact('hello')
            print('hello2')

            return {
                'link': '/hello'
            }

        return render_template('index.html')
