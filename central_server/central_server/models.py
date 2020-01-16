from django.db import models

TEMPERATURE_THRESHOLD = 50
N_TEMPERATURE_INCREMENTED_THRESHOLD = 10


class FireAlarm(models.Model):
    _id = models.AutoField(primary_key=True)
    alarm_is_active = models.BooleanField(default=False)
    creation_date = models.DateTimeField(auto_now_add=True)
    update_date = models.DateTimeField(auto_now=True)

    def alarm_check(self, request=None, format=None):
        """
        Checks if the alarm should be set
        """
        n_temperature_incremented = 0

        samples_query_set = TemperatureMetric.objects.filter(fire_alarm=self)
        samples = samples_query_set.values()
        samples_ordered_by_temperature = sorted(samples, key=lambda k: k['temp'])
        print("Samples stored: {0}".format(str(samples_ordered_by_temperature)))

        for sample in samples_ordered_by_temperature:
            if sample['temp'] >= TEMPERATURE_THRESHOLD:
                n_temperature_incremented += 1
            else:
                n_temperature_incremented = 0
        if n_temperature_incremented >= N_TEMPERATURE_INCREMENTED_THRESHOLD:
            self.alarm_is_active = True
            print("There's a fire! Set the alarm!")
            # Put here HTTP request to the board to activate a light
        else:
            self.alarm_is_active = False

        samples_query_set.delete()


class TemperatureMetric(models.Model):
    _id = models.AutoField(primary_key=True)
    temp = models.FloatField(null=False, default=0)
    fire_alarm = models.ForeignKey(FireAlarm, null=False, on_delete='cascade')
