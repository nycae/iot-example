# Athemes

## Cómo correr el proyecto

### Requisitos previos
Esto es un prototipo para un proyecto IoT llamado \<\<Athemes>>. Para correr este proyecto es necesario, por lo menos, una placa arduino \<\<«wemos d1 r32»>> y un tablero de expansión multifunción \<\<DHT11 LM35>>.

También necesitaremos un equipo compatible con docker-compose en el cual desplegar el clúster propuesto. Si el sistema operativo de nuestro equipo usa dpkg como gestor de paquete podemos instalarlo de la siguiente forma:

```
# apt install docker.io
# apt install docker-compose
```

### Descarga de proyecto.

Este proyecto puede descargarse desde la sección \<\<tags>> de github o bien usando git. La versión de descargable en formato comprimido es igual que cualquier otra distribución.

Si se optó por descargar este repositorio usando git es menester no olvidar de clonar también los submódulos.
```
$ git clone --recursive https://github.com/RoberPlaza/sistemas-ciberfisicos
```
o
```
$ git clone https://github.com/RoberPlaza/sistemas-ciberfisicos
$ git submodule init
$ git submodule update
```

### Lanzar el clúster Athemes

Para lanzar el clúster Athemes al completo tan solo necesitamos una llamada.
```
$ docker-compose up
```

Si queremos desplegar sólo parte de las aplicaciones las especificamos en el comando:
```
$ docker-compose up <nombre-del-servicio>
```

**Posible problema:** El servidor central necesita que el servidor mysql esté levantado, para esto espera 5 minutos. En un caso anómalo de correrse en un equipo de muy bajas prestaciones será necesario lanzar otra vez el comando.

### Crear la base de datos de temperaturas.

La primera vez que se lanza la suite es necesario crear la base de datos de temperaturas, para ello se ha provisto de un archivo en la carpeta scripts:

```bash
$ sh scritps/influxdb-init.sh
```
Aunque también se puede crear a mano.
```
curl -X POST http://localhost:8086/query --data-urlencode "q=CREATE DATABASE temperatures;" 
```

### Lanzar arduino.

Para lanzar arduino es necesario editar el archivo fuente \<\<arduino_readings/src/main.cpp>> y editamos las líneas 9, 10 y 11 con los siguientes valores: el nombre de nuestra red, la contraseña y la ip de red del equipo donde hemos desplegado el clúster. Después será necesario compilar el proyecto con PlatformIO y subirlo al controlador que estemos utilizando.

### Final.

Una vez completados los anteriores pasos todo el modelo queda desplegado, y podemos visitar los endpoints localhost:3000 y localhost:8888 para trastear con grafana y cronograf.

### ¿Qué hago si no tengo un arduino a mano?

Puedes modificar el archivo docker-compose y añadir un servicio nodered a la pila, recuerda redireccionar el puerto 1800.
