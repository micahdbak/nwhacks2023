import os
import atexit
from . import backend
from flask import Flask, render_template;

SECRET_KEY = 'GENERATE_ME_PLEASE'

def close_socket():
    if len(backend.sock) == 0:
        return

    backend.sock[0].close()

atexit.register(close_socket)

def create_app():
    app = Flask(__name__, template_folder='./templates')

    app.secret_key = SECRET_KEY

    try:
        os.makedirs(app.instance_path)
    except OSError:
        pass

    backend.get_socket()

    @app.route('/hello', methods=['GET','POST'])
    def hello():
        if method == 'POST':
            json = 
        return render_template('index.html')

    return app
