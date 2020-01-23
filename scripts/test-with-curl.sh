#!/bin/bash

curl localhost:5000/temp/new
curl -X POST -H "Content-Type: application/json" -d '{"msg": 38.8, "id" : 1}' localhost:5000/temp
