from rest_framework import serializers

from central_server.models import FireAlarm, TemperatureMetric


# fields = '__all__' Needed as of version 3.3.0

class FireAlarmSerializer(serializers.ModelSerializer):
    class Meta:
        model = FireAlarm
        fields = '__all__'


class TemperatureMetricSerializer(serializers.ModelSerializer):
    class Meta:
        model = TemperatureMetric
        fields = '__all__'
