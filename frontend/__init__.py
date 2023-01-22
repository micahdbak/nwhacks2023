import os
import atexit
from . import backend, index
from flask import Flask, render_template, request;

SECRET_KEY = 'GENERATE_ME_PLEASE'

def stop_backend():
    backend.transact('stop')

atexit.register(stop_backend)

def create_app():
    app = Flask(__name__, template_folder='./templates')

    app.secret_key = SECRET_KEY

    try:
        os.makedirs(app.instance_path)
    except OSError:
        pass

    index.index(app)

    return app

if __name__ == '__main__':
    create_app().run()
