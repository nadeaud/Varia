import urllib2
import re

respone = urllib2.urlopen('http://www.kijiji.ca/b-appartement-condo-studio-2-1-2/ville-de-montreal/c212l1700281')

html = respone.read()

appart = re.search('<table class="regular-ad js-hover"?*appartement?*</table>', html)

print(appart)