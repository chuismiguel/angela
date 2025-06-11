from py_compile import main
import RPi.GPIO as GPIO
import time
import neopixel
import board

MAX_DISTANCE = 0.5
TH = 0.1

class Ultrasound: # one pin distance sensor
    def __init__(self, pin, max_distance, inverval=.2):
        GPIO.setmode(GPIO.BCM)
        self._pin = pin
        self.max_distance = max_distance
        self.inverval = inverval
        self._distance = max_distance
        self.when_in_range = self.when_out_of_range = None

    def get_distance(self):
        GPIO.setup(self._pin, GPIO.OUT)
        # set Trigger to HIGH
        GPIO.output(self._pin, False)
        time.sleep(0.000002)
        GPIO.output(self._pin, True)

        # set Trigger after 0.01ms to LOW
        time.sleep(0.00001)
        GPIO.output(self._pin, False)

        GPIO.setup(self._pin, GPIO.IN, pull_up_down=GPIO.PUD_UP)
        StartTime = StopTime = TriggerTime = time.time()

        # save StartTime
        while GPIO.input(self._pin) == 0:
            StartTime = time.time()
            if StartTime - TriggerTime > 0.01:
                return

        # save time of arrival
        while GPIO.input(self._pin) == 1:
            pass
        StopTime = time.time()

        # time difference between start and arrival
        TimeElapsed = StopTime - StartTime
        # multiply with the sonic speed (34300 cm/s)
        # and divide by 2, because there and back
        self._last_distance = self._distance
        self._distance = min(TimeElapsed * 171.5, self.max_distance)

        return self._distance        
    
class SonarLeds:
    def __init__(self, pin):
        self.pixels = neopixel.NeoPixel(getattr(board, pin), 1)
        self.pixels.fill((255,255,255))

    def color(self, color):
        self.pixels.fill(color)
