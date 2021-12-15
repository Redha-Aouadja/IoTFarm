import pyrebase
from datetime import datetime
import time
import matplotlib.pyplot as plt
import pandas as pd


config = {
  "apiKey": "hidden",
   "authDomain": "hidden",
  "databaseURL": "hidden",
   "storageBucket": "hidden"
}

firebase = pyrebase.initialize_app(config)
db=firebase.database()
mData=[]
tData=[]
hData=[]
SmData=[]
wlData=[]
wpData=[]
timedata=[]
while True:
    mt = db.child("motion").get()
    mData.append(mt.val())
    hm = db.child("humidity").get()
    hData.append(hm.val())
    # print(hm.val())
    sm = db.child("soil-moisture").get()
    SmData.append(sm.val())
    # print(sm.val())
    tm = db.child("temperature2").get()
    tData.append(tm.val())
    # print(tm.val())
    wl = db.child("waterLevel").get()
    wlData.append(wl.val())
    # print(wl.val())
    wp = db.child("pump").get()
    wpData.append(wp.val())
    # print(wp.val())
    #print(SmData)
    now=datetime.now()
    current_time=now.strftime("%H : %M : %S")
    timedata.append(current_time)
    if wp.val():
        print("water pump on")
    if mt.val():
        print("motion detected")
        print("lights are on")
    if tm.val() >24:
        print("fan is on")

    #print(timedata)
    plt.ion()
    for x in range(5):
        plt.plot(timedata,tData)
        plt.plot(timedata,hData)
        plt.plot(timedata,SmData)
        plt.xlabel("Time")
        plt.ylabel("Sensor Data")
        legend = plt.legend(['Soil Moisture', 'Humidity', 'Temperature'], title="Data",frameon=False)
        #legend=plt.legend(handles=['Soil Moisture', 'Humidity', 'Temperature'])
        plt.draw()
        plt.pause(0.1)
    plt.show()
    # plt.show()


    time.sleep(5)
