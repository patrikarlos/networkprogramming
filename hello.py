from flask import Flask
app = Flask(__name__)

@app.route('/')
def hello_world():
    return 'Hello, World!'

@app.route('/add/<a>/<b>')
def add(a=0,b=0):
    total = float(a) + float(b)
    return(str(total))

@app.route('/sub/<a>/<b>')
def sub(a=0,b=0):
    total = float(a) - float(b)
    return(str(total))
