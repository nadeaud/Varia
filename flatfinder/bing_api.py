#! /usr/bin/python

import urllib.request

# Your Bing Maps Key 
f = open('key.txt', 'r')
bingMapsKey = f.read().splitlines()[0]

# input information
longitude = -122.019943
latitude = 37.285989

origin = "10770 Jeanne-Mance Montreal"
encodeOrigin = urllib.parse.quote(origin, safe='')

destination = "10100 Laverdure Montreal"
encodedDest = urllib.parse.quote(destination, safe='')

#routeUrl = "http://dev.virtualearth.net/REST/V1/Routes/Driving?wp.0=" + str(latitude) + "," + str(longitude) + "&wp.1=" + encodedDest + "&key=" + bingMapsKey

routeUrl = "http://dev.virtualearth.net/REST/V1/Routes/Walking?wp.0=" + encodeOrigin + "&wp.1=" + encodedDest + "&key=" + bingMapsKey 
request = urllib.request.Request(routeUrl)
response = urllib.request.urlopen(request)
print (response.read())
