from rest_framework.viewsets import ModelViewSet
from rest_framework.views import APIView
from django.http import JsonResponse
from central_server.models import FireAlarm, TemperatureMetric
from central_server.serializers import FireAlarmSerializer, TemperatureMetricSerializer
import json

MAX_NUMBER_TEMPS_METRICS = 15


class FireAlarmViewSet(ModelViewSet):
    """
    API endpoint to manage fire alarm triggers
    """
    serializer_class = FireAlarmSerializer

    queryset = FireAlarm.objects.all()

    def perform_create(self, serializer):
        # Perform create
        serializer.save()
        print("Fire alarm created")


class TemperatureCollection(APIView):
    http_method_names = ['post']

    def post(self, request, fire_alarm_id):
        body_unicode = request.body.decode('utf-8')
        body = json.loads(body_unicode)
        temp = body['temp']

        fire_alarm = FireAlarm.objects.get(_id=fire_alarm_id)

        TemperatureMetric.objects.create(
            temp=temp,
            fire_alarm=fire_alarm,
        )

        if len(fire_alarm.temperaturemetric_set.all().values()) \
                >= MAX_NUMBER_TEMPS_METRICS:
            fire_alarm.alarm_check()

        return JsonResponse({'temp': str(temp)})
