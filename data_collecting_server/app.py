from flask import Flask, request, make_response, jsonify

app = Flask(__name__)

def add_temp(temp_reading):
    pass

def get_temps():
    pass

def add_weight(weight_reading):
    pass

def get_weights():
    pass

@app.route('/temp', methods = ['POST', 'GET'])
def attend_temperatures():
    if request.method == 'POST':
        if not request.json or 'msg' in request.json:
            return make_response(jsonify({'error' : 'Bad Request'}), 400)
        else:
            add_temp(request.json['msg'])
            return make_response(jsonify({'success': True}), 200)

    if request.method == 'GET':
        return make_response(jsonify({ 'data' : get_temps() }), 200)

@app.route('/weight', methods = ['POST', 'GET'])
def attend_weights():
    if request.method == 'POST':
        if not request.json or 'msg' in request.json:
            return make_response(jsonify({'error' : 'Bad Request'}), 400)
        else:
            add_temp(request.json['msg'])
            return make_response(jsonify({'success': True}), 200)

    if request.method == 'GET':
        return make_response(jsonify({ 'data' : get_weights() }), 200)


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
