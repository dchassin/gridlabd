import os
from gldapp import *
import subprocess

APPNAME = "Arras Weather"

class WeatherApp(GldApp):
    config_file = "config_weather.json"

app = WeatherApp()

weather_stations = subprocess.run(["gridlabd","weather","index"],capture_output=True).stdout.decode('utf-8').strip().split("\n")
weather = {}
for station in weather_stations:
    state,city = station.split("-",1)
    if not state in weather:
        weather[state] = {}
    weather[state][os.path.splitext(city)[0].replace("_"," ").title()] = station

state = StringVar()
state.set(list(weather)[0])

city = StringVar()
city.set(list(weather[state.get()])[0])

state_ui = OptionMenu(app.root,state,*list(weather))
state_ui.grid(row=0,column=0,sticky=W,pady=2)

city_ui = OptionMenu(app.root,city,*list(weather[state.get()]))
city_ui.grid(row=0,column=1,sticky=W,pady=2)

station = weather[state.get()][city.get()]
subprocess.run(["gridlabd","weather","get",station])
station_info = subprocess.run(["gridlabd","weather","info",station],capture_output=True).stdout.decode('utf-8').strip().split("\n")
station_info = dict(zip(station_info[0].split(","),station_info[1].split(',')))
row = 1
for label,value in station_info.items():
    ui_label = Label(app.root,text=label)
    ui_label.grid(row=row,column=0,sticky=W,pady=2)
    value = value.strip('"')
    if "/" in value:
        value = "$GLD_ETC/"+"/".join(value.split('/')[-2:])
    ui_value = Label(app.root,text=value)
    ui_value.grid(row=row,column=1,sticky=W,pady=2)
    row += 1

app.root.title(f"{APPNAME} - {os.path.basename(station_info['Filepath'])}")

app.run()
