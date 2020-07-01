# Athemes

## How-To

### Prerequisites
Athemes is an IoT proyect stored in this repo. In order to run it you need, at least one, arduino «wemos d1 r32» coupled with a multifunction shield «DHT11 LM35».

A sistem able to run docker an docker compose is also neded. If your operating system uses dpkg as its main packet manager you can install docker and docker-compose running the following commands:
```
# apt install docker.io
# apt install docker-compose
```

### Download

This proyect can be downloaded in the latest release from this repository or cloning it with git. All versions are the same.

Don't forget to pull the submodules if you are using git.
```
$ git clone --recursive https://github.com/RoberPlaza/sistemas-ciberfisicos
```
or
```
$ git clone https://github.com/RoberPlaza/sistemas-ciberfisicos
$ git submodule init
$ git submodule update
```

### Launch athemes infrastructure

Simply use doccker compose
```
$ docker-compose up
```

If you want to use only a single module go ahead:
```
$ docker-compose up <module_name>
```

**Possible failure:** The main server waits for the database 5 minutes or less. In a case of an anomalous slow start a cluster restart may be needed.

### Create temperatures database

The first time you run this suite it will be necessary to create the temperatures database, there is a script provided to ease your life:

```bash
$ sh scritps/influxdb-init.sh
```
But you can also do it by hand:
```
curl -X POST http://localhost:8086/query --data-urlencode "q=CREATE DATABASE temperatures;" 
```

### Launch the arduino.

In order to run the proyect as expected you may need to rewrite the file «arduino_readings/src/main.cpp». In the lines 9, 10 and 11 there are strings with information about the network name, the password and the central server ip, update them with your values. After that user PlatformIO to compile the program and uploading it to the microcontroller.

### Final.

Once everything is deployed and running you can visit localhost:3000 and localhost:8888 in order to play a bit with grafana an cronograf.

### What if I dont have an arduino?

You can use «Node-RED» to simulate it. Add the image to the docker compose, login to localhost:1800 and create a periodic post request on localhost:5000/temp sending a json with the fields "id": number and "msg": number, this last one with the reading.
