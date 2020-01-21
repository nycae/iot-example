curl -X POST http://localhost:8086/query --data-urlencode "q=CREATE DATABASE temperatures;" 
curl -X POST http://localhost:8086/query --data-urlencode "q=CREATE RETENTION POLICY one_month ON temperatures DURATION 30d REPLICATION 1 DEFAULT;"

