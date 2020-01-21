from flask import Flask

from temps import temp_blueprint
from weights import weight_blueprint


app = Flask(__name__)

app.register_blueprint(temp_blueprint)
#app.register_blueprint(weight_blueprint)


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
