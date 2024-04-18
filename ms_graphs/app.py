# app.py

from flask import Flask, render_template, jsonify
import json
import random

app = Flask(__name__)

@app.route('/')
def index():
    # Render a template that will display the graph
    return render_template('index.html')

@app.route('/data')
def data():
    # Read the values from 'values.json'
    with open('values.json', 'r') as json_file:
        data = json.load(json_file)
    # Return the data as JSON
    return jsonify(data)
# @app.route('/data')
# def data():
#     # Simulate new data being generated for each call
#     # In a real application, you would query your sensors or a database for the latest data
#     data = {
#         'ms_0': random.randint(0, 100),
#         'ms_1': random.randint(0, 100),
#         'ms_2': random.randint(0, 100),
#         'ms_3': random.randint(0, 100),
#         'ms_4': random.randint(0, 100)
#     }
#     return jsonify(data)


if __name__ == '__main__':
    app.run(debug=True)
