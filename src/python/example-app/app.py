#!/usr/bin/python3

###
# Ejemplo reducido de python flask
###

from flask import Flask, request, render_template, make_response, jsonify


# Creamos la aplicacion flask y tambien las variables globales.
app         =   Flask(__name__)
messages    =   [
    "This is an example message",
    "Esto tambien, pero en espaniol"
]


# La funcion render_template renderiza una plantilla Jinja, he preparado una
# para el ejemplo. No se si lo conoces, pero no me gusta, si conoces algo de 
# javascript tal vez sea mejor idea usar plantillas html y js.
def render_msg():
    return render_template('messages.html', messages=messages)

# Flask usa decoradores para crear los endpoints, se pueden aniadir varios 
# endpoints a una misma funcion. 
@app.route('/')
@app.route('/index')
def index():
    return '<h1>Hello, world<h1>'

# Tambien podemos expecificar que metodos queremos para un endpoint, en este 
# caso el endpoint /messages solo puede accederse con los verbos GET y POST.
# Dentro del objeto 'request' esta toda la informacion del protocolo http.
@app.route('/messages', methods= ['GET', 'POST'])
def operation():

    # Siento la sobreingenieria, pero que quede claro.
    def add_msg():
        def add_from_form():
            messages.append(request.form['msg'])

        def add_from_json():
            messages.append(request.json['msg'])

        if not request.json or not 'msg' in request.json:
            if not request.form or not 'msg' in request.form:
                return make_response(jsonify({'error' : 'Bad request'}), 400)
            else:
                add_from_form()
        else:
            add_from_json()

        return render_msg()

    if request.method == 'POST':
        return add_msg()
    
    if request.method == 'GET':
        return render_msg()

# Por ultimo podemos aniadir variables a la url. Para ello usamos los 
# caracteres '<' y '>'. En este caso la funcion necesita que le pasemos 
# un argumento con el nombre de la variable.
@app.route('/messages/insert/<msg>')
def insert_msg(msg):
    messages.append(msg)
    return render_msg()


if __name__ == '__main__':
    # Esto cambia de version a version y estoy hasta la polla de indagar como
    # se hace en cada version, asi que asi se queda.
    app.run(host='0.0.0.0')
