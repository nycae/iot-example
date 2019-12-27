from rest_framework import views
from django.http import JsonResponse
import json

TEMPERATURE_THRESHOLD = 50
N_TEMPERATURE_INCREMENTED_THRESHOLD = 10


class FireAlarmView(views.APIView):
    """
    API endpoint to manage fire alarm triggers
    """

    def post(self, request=None, format=None):
        """
        Checks if the alarm should be set
        """
        n_temperature_incremented = 0
        body_unicode = request.body.decode('utf-8')
        body = json.loads(body_unicode)
        samples = body['data']
        samples_ordered_by_temperature = sorted(samples, key=lambda k: k['temperature'])
        print(str(samples_ordered_by_temperature))
        for sample in samples_ordered_by_temperature:
            if sample['temperature'] >= TEMPERATURE_THRESHOLD:
                n_temperature_incremented += 1
            else:
                n_temperature_incremented = 0
        if n_temperature_incremented >= N_TEMPERATURE_INCREMENTED_THRESHOLD:
            print("There's a fire! Set the alarm!")
            # Put here HTTP request to the board to activate a light

        return JsonResponse({'data': body['data']})
