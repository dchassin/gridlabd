import os
import csv
from io import StringIO
import tkinter as tk
from gldapp import *
import subprocess
import pandas as pd
import pandastable as pt

APPNAME = "Arras Weather"

class WeatherError(Exception):
    pass

class WeatherApp(GldApp):
    config_file = "config_weather.json"

class Weather:

    def __init__(self,*cmd,strip=True,decode='utf-8'):
        self.result = subprocess.run(["gridlabd-weather"]+list(cmd),capture_output=True)
        self.output = self.result.stdout
        if decode:
            self.output = self.output.decode(decode)
        if strip:
            self.output = self.output.strip(strip if isinstance(strip,str) else None)

    def as_str(self):
        return self.output

    def as_list(self,split='\n',strip=True):
        return [x.strip(strip if isinstance(strip,str) else None) if strip else x for x in self.output.split(split)]

    def as_dict(self,split='\n',strip=True,delim=','):
        lines = self.as_list(split=split)
        return dict([[y.strip(strip if isinstance(strip,str) else None) for y in x.split(delim)] for x in lines])

    def as_csv(self,**kwargs):
        return [list(x) for x in csv.reader(StringIO(self.output),**kwargs)]

    def as_dataframe(self,**kwargs):
        return pd.read_csv(StringIO(self.output),**kwargs)

if __name__ == "__main__":

    app = WeatherApp()
    app.root.minsize(820,600)

    # weather_config = Weather("config","show").as_dict(delim='=',strip='"')
    # print(weather_config)

    # quit()

    # weather_stations = Weather("index").as_list()
    weather_stations = [x for x in os.listdir(os.path.join(os.environ['GLD_ETC'],"weather/US")) if x[2] == '-']
    weather = {}
    for station in weather_stations:
        state,city = station.split("-",1)
        if not state in weather:
            weather[state] = {}
        weather[state][os.path.splitext(city)[0].replace("_"," ").title()] = station

    state = StringVar()
    state.set(list(weather)[0])

    def change_state(value):
        state.set(value)
        city_ui['menu'].delete(0,'end')
        for name in weather[state.get()].values():
            city_ui['menu'].add_command(label=name,command=tk._setit(city,name))
        city.set(list(weather[state.get()])[0])

    city = StringVar()
    city.set(list(weather[state.get()])[0])

    state_ui = OptionMenu(app.root,state,*list(weather),command=change_state)
    state_ui.grid(row=0,column=0,sticky=W,pady=2)

    city_ui = OptionMenu(app.root,city,*list(weather[state.get()]))
    city_ui.grid(row=0,column=1,sticky=W,pady=2)

    download_button = Button(text="Download")
    download_button.grid(row=0,column=2)

    delete_button = Button(text="Delete")
    delete_button.grid(row=0,column=3)

    local_ui = Checkbutton(app.root,text="Local only")
    local_ui.grid(row=0, column=4)
    
    station_info = {'Filepath':os.path.join(os.environ['GLD_ETC'],'weather/US',f"{state.get()}-{city.get().replace(' ','_')}.tmy3")}
    # station = weather[state.get()][city.get()]
    # Weather("get",station)
    # # station_info = subprocess.run(["gridlabd","weather","info",station],capture_output=True).stdout.decode('utf-8').strip().split("\n")
    # station_info = Weather("info",station).as_dataframe(header=0).transpose()
    # station_info = station_info.to_dict()[0]
    
    # row = 2
    # info = {}
    # for label,value in station_info.items():
    #     ui_label = Label(app.root,text=label)
    #     ui_label.grid(row=row,column=0,sticky=W,pady=2)
    #     if "/" in str(value):
    #         value = "$GLD_ETC/weather/"+"/".join(value.split('/')[-2:])
    #     ui_value = Label(app.root,text=value)
    #     ui_value.grid(row=row,column=1,sticky=W,pady=2)
    #     row += 1

    app.root.title(APPNAME)

    pd.options.display.max_colwidth = None
    pd.options.display.max_columns = None
    pd.options.display.width = None
    station_data = pd.read_csv(os.path.join(station_info['Filepath']),
        header=1,
        usecols=[
            0,1,4,7,10, # solars
            25,28, # clouds
            31,34, # temps
            37, # humitidy
            40, # pressure
            43,46, # windws
            ],
        low_memory=False)
    dates = [dt.datetime.strptime(f"{d} {int(t.split(':')[0])-1}",'%m/%d/%Y %H') 
        for d,t in zip(station_data[station_data.columns[0]],station_data[station_data.columns[1]])]
    station_data.index = pd.DatetimeIndex(dates)
    station_data.drop([station_data.columns[0],station_data.columns[1]],axis=1,inplace=True)
    # print(json.dumps(list(enumerate(station_data.columns)),indent=4),flush=True)
    # quit()
    # print(station_data)
    # ui_table = pt.Table(app.root,dataframe=station_data,showtoobar=True,showstatusbar=True)
    # ui.table.show()#.grid(row,1)

    Label(text=station_data.index.name).grid(row=1,column=0)
    for n,data in enumerate(station_data.columns):
        Label(text=data.split(' ')[0]).grid(row=1,column=n+1)
        Label(text=data.split(' ')[1]).grid(row=2,column=n+1)
    n = 3
    for dt,data in station_data.iterrows():
        Label(text=dt.strftime("%-d %b %H:00")).grid(row=n,column=0)
        for m,value in enumerate(data):
            Label(text=str(value)).grid(row=n,column=m+1)
        if n > 24:
            break
        # break
        n += 1

    # print(station_data)
    app.run()
