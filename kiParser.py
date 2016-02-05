import urllib2
import re

class Appartement:
    def __init__(self,html):
        self.html = html
    def setPrice(self, price):
        self.price = price
        
        
class kiParser :
    
    def parsePage(self, url):
        
        response = urllib2.urlopen(url)
        html = response.read()
        
        apparts = re.findall('<table[^>]*?appartement[\w\W]*?</table>', html)
        
        return apparts

    def findPrice(self, appartement):
        
        price = re.search('<td[\s]*?class="price">[\w\W]*?</td>', appartement.html).group(0)
        price = re.search('[0-9]{1,4},[0-9]{0,2}', price).group(0)
        price = float(re.sub(',', '.', price))
        appartement.setPrice(price)
        
    def foo(self):
        print('foo')
