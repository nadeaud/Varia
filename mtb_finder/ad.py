import urllib.request
from urllib.error import URLError, HTTPError

class Advertisement(object):
    price = -1
    url = ''
    name = ''

    def validateAd(self):
        req = urllib.request.Request(self.url)
        try:
            response = urllib.request.urlopen(req)
        except HTTPError as e:
            print('Error: ', e.code)
            return False
        except URLError as e:
            print('Error, reason: ', e.reason)
            return False
        else:
            if response.url != self.url:
                return False
            else:
                return True



    def __init__(self, price, address, name):
        self.price = price
        self.url = address
        self.name = name
