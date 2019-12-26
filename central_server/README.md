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
Triggered when there are consecutive timestamps with a high enough temperature value.
Queried by the central server every 15 seconds. Since the temperature is recollected every second,
there would be 15 registers per query. If 10 of those are bigger than the threshold of 50º, the 
alarm if set.

Either if the alarm is set or not, a resample of those 15 seconds will be stored in the main db.

### Insufficient stock alert
Triggered when the weight of a part of a shelf is under the threshold for a certain amount of time. 
Queried each minute, with the weight of each shelf part recollected each second. If for 45 seconds
the weight has been under the threshold, an alert is triggered in order to start the restocking 
process.
