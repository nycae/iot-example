#!/bin/bash

curl -X POST -H "Content-Type: application/json" -d '{"msg": "Este mensaje ha llegado con Curl via servicio rest"}' localhost:5000/messages
