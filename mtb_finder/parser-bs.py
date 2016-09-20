#! /usr/bin/python3

import requests
from html.parser import HTMLParser
from bs4 import BeautifulSoup
import xml.etree.ElementTree as ET
import re
from ad import Advertisement
import pickle

file = 'ad_list.pk1'
base_url = 'http://www.kijiji.ca/b-velo-de-montagne/ville-de-montreal/'
category = 'c647l1700281'

# Attributes for filtering : to be improved
minPrice = 600
maxPrice = 1600

def mergeLists(ad_list_1, ad_list_2):
    mergeList = []
    mergeDict = {}
    for ad in ad_list_1:
        if mergeDict.setdefault(ad.url, 0) == 0:
            mergeDict[ad.url] = 1
            mergeList.append(ad)
    for ad in ad_list_2:
        if mergeDict.setdefault(ad.url, 0) == 0:
            mergeDict[ad.url] = 1
            mergeList.append(ad)
    return mergeList

def verifyAdList(ad_list):
    for ad in ad_list:
        if  not isinstance(ad, Advertisement):
            ad_list.remove(ad)
            continue
        if ad.validateAd():
            continue
        else:
            ad_list.remove(ad)

def saveAdList(ad_list):
    if not ad_list:
        return
    verifyAdList(ad_list)
    with open(file, 'wb') as output:
        pickle.dump(ad_list, output, pickle.HIGHEST_PROTOCOL)
    return

def loadAdList():
    try:
        with open(file, 'rb') as input:
            ad_list = pickle.load(input)
            verifyAdList(ad_list)
            return ad_list
    except FileNotFoundError:
        ad_list = []
        return ad_list
    except EOFError:
        ad_list=[]
        return ad_list

def parsePage(url):
    ad_list = []
    response = requests.get(url)
    soup = BeautifulSoup(response.text, 'html.parser')

    for ad in soup.findAll('div', { 'class' : 'search-item regular-ad'}):
        a = ad.find('a')
        if a == None:
            continue
        price = ad.find('div', {'class' : 'price'})
        if price == None:
            continue
        if price.text == None:
            continue
        res = re.search('([0-9]{1,4}),[0-9]{0,2}', price.prettify(formatter= lambda s: s.replace(u'\xa0', ' ')).replace(' ', ''))
        if res == None:
            continue
        name = ad.find('a', {'class' : 'title'})
        if name == None:
            continue
        name = name.text
        price = int(res.group(1))
        if price > minPrice and price < maxPrice:
            ad = Advertisement(price, 'http://www.kijiji.ca' + a.get('href'), name)
            ad_list.append(ad)
    return ad_list

def getAds():
    ad_list = []
    for i in range(1,5):
        url = base_url + 'page-' + str(i) + '/' + category
        ads = parsePage(url)
        ad_list.extend(ads)
    return ad_list
        
def pretty_print_list(ad_list):
    i = 1
    for ad in ad_list:
        print(str(i))
        print('\t' + ad.name)
        print('\t' + str(ad.price) + '$')
        print('\t' + ad.url)
        i+=1


if __name__ == '__main__':
    ad_list = getAds()
    verifyAdList(ad_list) 
    ad_list_2 = loadAdList()
    final_ad_list = mergeLists(ad_list, ad_list_2)
    saveAdList(final_ad_list)
    pretty_print_list(ad_list)
