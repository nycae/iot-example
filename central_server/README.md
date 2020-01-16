# Admin Portal
## Introduction

The admin portal component corresponds to the central server of the application. 

This includes a frontend view for the administrator of the system, a mysql database with the 
products in stock and achieved data used for analytics and an influxdb database which stores 
certain metrics needed to trigger events.

The difference between the achieved data of the mysql db integrated with the DJango model used to 
build the portal and the influxdb is that the achieved data is a summary of the data stored in 
influxdb. It lacks the details needed to trigger the events of the business and the goal is to have
historic data to apply data analytics in order to obtain knowledge on the business process.

## Launching the server

```
$> cd $ATHEMES_ROOT
$> docker-compose up central_server
```
## Events

The backend queries information from the influxdb cyclically every set amount of seconds. After the
information is processed and the needed processes are executed, a sample of that data is stored 
inside the mysql database.

### Fire alarm
Triggered when there are consecutive timestamps with a high enough temperature value. Every 
to be determined amount of time, the sensor will send a POST petition with the server which is a 
dictionary with the only key "temp" and the current temperature that it registered.

After 15 entries the central server studies if it's needed to set the alarm of not. If more than 
10 are equal or over to 50 degree celsius, the alarm if set. If not, nothing is set off.

The DJango db only stores the last 15 entries of every fire alarm, so every time the alarm is 
checked, the entries are deleted afterwards. There would be a historic of all the temperature data
stored in the influxdb, which will be updated automatically after each check.

#### Setting up 

Creating an alarm. The petition will return the id. The id is used in the endpoint
`fire_alarm/<fire_alarm_id>/temp` used to introduce temperature metrics by the fire alarm sensor

`curl -X post localhost:8000/fire_alarm/`

Taking into consideration that the fire alarm id is 1;

Example to trigger the alarm:
Send 15 times:

`curl -X post -H "Content-Type: application/json" -d '{"temp":50}' localhost:8000/fire_alarm/1/temp`

Example to not trigger the alarm:
Send 15 times:

`curl -X post -H "Content-Type: application/json" -d '{"temp":30}' localhost:8000/fire_alarm/1/temp`

### Insufficient stock alert
Triggered when the weight of a part of a shelf is under the threshold for a certain amount of time. 
Queried each minute, with the weight of each shelf part recollected each second. If for 45 seconds
the weight has been under the threshold, an alert is triggered in order to start the restocking 
process.
