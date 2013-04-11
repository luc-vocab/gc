
import sys
import csv
import math


class Reading():
    def __init__(self, millis, value, average):
        self.millis = millis
        
        # convert millis to human-readable time
        hours = math.floor(millis / (3600 * 1000))
        remaining = millis - hours * 3600 * 1000
        minutes = math.floor(remaining / (60 * 1000))
        remaining = remaining - minutes * 60 * 1000
        seconds = math.floor(remaining / 1000)
        milliseconds = remaining - seconds * 1000
        
        self.time_str = "%02d:%02d:%02d.%03d" %(hours, minutes, seconds, milliseconds)
        
        self.value = value
        self.average = average
       
    def __str__(self):
        return "%s reading: %4d average: %4d" %(self.time_str, self.value, self.average)
    

class GcLog():
    def __init__(self, filename):
        self.readings = []
        reader = csv.reader(open(filename))
        for row in reader:
            millis = int(row[0])
            value = int(row[1])
            average = int(row[2])
            reading = Reading(millis, value, average)
            self.readings.append(reading)
            
    def all(self):
        return self.readings
        
    def distribution(self):
        buckets = [[25, 35],
                   [35, 40],
                   [40, 50],
                   [50, 60],
                   [60, 70],
                   [70, 80],
                   [80, 90],
                   [90, 100],
                   [100, 110],
                   [110, 120],
                   [120, 130],
                   [130, 140],
                   [140, 999],]
        
        print("total readings: %d" %(len(self.readings)))
        for bucket in buckets:
            min = bucket[0]
            max = bucket[1]
            readings_between = self.reading_between(min, max)
            num_readings = len(readings_between)
            pct_readings = 100.0 * (num_readings / len(self.readings))
            print("between %3d and %3d: %5d (%.2f pct)" %(min, max, num_readings, pct_readings))
            #print("%d,%d,%d,%f" %(min, max, num_readings, pct_readings))
        
    def average_distribution(self):
        buckets = [[25, 30],
                   [30, 32],
                   [32, 35],
                   [35, 40],
                   [40, 45],
                   [45, 50],
                   [50, 55],
                   [55, 60],
                   [60, 65],
                   [65, 70]]
        
        print("total readings: %d" %(len(self.readings)))
        for bucket in buckets:
            min = bucket[0]
            max = bucket[1]
            readings_between = self.average_between(min, max)
            num_readings = len(readings_between)
            pct_readings = 100.0 * (num_readings / len(self.readings))
            print("between %3d and %3d: %5d (%.2f pct)" %(min, max, num_readings, pct_readings))
            #print("%d,%d,%d,%f" %(min, max, num_readings, pct_readings))        
        
    def average_between(self, min, max):
        readings = [reading for reading in self.readings if reading.average >= min and reading.average < max]
        return readings        
        
    def reading_between(self, min, max):
        readings = [reading for reading in self.readings if reading.value >= min and reading.value < max]
        return readings
        
    def reading_above(self, threshold):
        readings = [reading for reading in self.readings if reading.value > threshold]
        return readings
        
    def average_above(self, threshold):
        readings = [reading for reading in self.readings if reading.average > threshold]
        return readings        

if __name__ == '__main__':
    filename = sys.argv[1]
    mode = sys.argv[2]
    gclog = GcLog(filename)
    readings = []
    if mode == "all":
        readings = gclog.all()
    elif mode == "value_above":
        threshold = int(sys.argv[3])
        readings = gclog.reading_above(threshold)
    elif mode == "average_above":
        threshold = int(sys.argv[3])
        readings = gclog.average_above(threshold)
    elif mode == "distribution":
        gclog.distribution()
    elif mode == "avg_distribution":
        gclog.average_distribution()
    else:
        print("argument %s not supported" %(mode))
        sys.exit(1)
    for reading in readings:
        print(reading)