from flask import Flask, render_template, request
from . import backend

def index(app):
    @app.route('/index.html', methods=['GET','POST'])
    def route_index():
        if request.method == 'POST':
            json = request.get_json()
            data = backend.transact('list CMPT')

            return {
                'link': '/index.html'
            }

        return render_template('index.html')
