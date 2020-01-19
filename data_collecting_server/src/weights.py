from flask import Blueprint, request, make_response, jsonify
from influxdb import InfluxDBClient

from datetime import datetime

weight_blueprint = Blueprint('weights', __name__)
db_weight_client = InfluxDBClient(host="influxdb", 
                                port=8086, 
                                username='admin', 
                                password='admin',
                                database='weights')

weight_insert_msg_body = [
    {
        "measurement": "weights",
        "tags": {
            "host": "data_server"
        },
        "time": None, 
        "fields": {
            "measurement": None
        }
    }
]


def add_weight(weight_reading):
    insert_query = weight_insert_msg_body
    insert_query[0]["time"] = datetime.utcnow().strftime('%Y-%m-%dT%H:%M:%SZ')
    insert_query[0]["fields"]["measurement"] = weight_reading

    db_weight_client.write_points(insert_query)

def get_weights():
    measurements    = []
    result          = db_weight_client.query("SELECT * FROM weights;")

    for i in result:
        for j in i:
            measurements.append(j['measurement'])

    return measurements
    

@weight_blueprint.route('/weight', methods = ['POST', 'GET'])
def attend_weights():
    if request.method == 'POST':
        if not request.json or not'msg' in request.json:
            return make_response(jsonify({'error' : 'Bad Request'}), 400)
        else:
            add_weight(request.json['msg'])
            return make_response(jsonify({'success': True}), 200)

    if request.method == 'GET':
        return make_response(jsonify({ 'data' : get_weights() }), 200)
