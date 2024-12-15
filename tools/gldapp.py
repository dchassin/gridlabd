import sys
import os
import json
from collections import namedtuple
import datetime as dt
from tkinter import *
from tkinter.filedialog import *
from tkinter.messagebox import *

APPNAME = "Arras"
CONFIGDIR = os.path.join(os.environ["HOME"],".arras")

# default config values
WIDTH = 1024
HEIGHT = 768
XPOS = 0
YPOS = 0
FILE = None
try:
    sys.path.insert(0,CONFIGDIR)
    from config import *
except:
    pass

class GldException(Exception):
    pass

class GldApp:

    config_file = None
    default_config = {
        "width" : WIDTH,
        "height" : HEIGHT,
        "xpos" : XPOS,
        "ypos" : YPOS,
        "file" : FILE
    }
    options = {"verbose":False}

    def __init__(self):
        if not self.config_file:
            raise GldException(f"cannot instantiate {self.__class__.__name__} directly")
        self.options = namedtuple('dict',self.options.keys())(**self.options)
        self.read_config()
        self.root = Tk()
        self.root.title(APPNAME)
        # self.root.configure(configure)
        self.root.minsize(800,600)
        self.root.geometry(f"{self.config.width}x{self.config.height}+{self.config.xpos}+{self.config.ypos}")
        if self.config.file is None:
            self.path = "."
            self.new()
        else:
            self.open(file=self.config.file)
        self.items = []

        # replace the "Python" menu with the app menu
        self.menu()
        self.changed = False
        self.root.protocol("WM_DELETE_WINDOW",self.quit)

    def error(self,message):
        e_type,e_value,e_trace = sys.exc_info()
        print(f"ERROR [GldApp:{e_type.__name__}]: {e_value}",file=sys.stderr,flush=True)

    def verbose(self,message):
        if self.options.verbose:
            print(f"VERBOSE [GldApp]: {message}",file=sys.stderr)

    def read_config(self):
        try:
            with open(os.path.join(CONFIGDIR,self.config_file),"r") as fh:
                config = json.load(fh)
        except FileNotFoundError:
            config = self.default_config
        except Exception as err:
            self.error(err)
            config = self.default_config
        self.config = namedtuple('dict',config.keys())(**config)
        self.verbose(self.config)

    def sync_config(self):
        size,xpos,ypos = self.root.winfo_geometry().split("+")
        width,height = size.split("x")
        config = {"width":width,"height":height,"xpos":xpos,"ypos":ypos,"file":self.file}
        self.config = namedtuple('dict',config.keys())(**config)

    def save_config(self):
        self.sync_config()
        try:
            with open(os.path.join(CONFIGDIR,self.config_file),"w") as fh:
                config = {x:getattr(self.config,x) for x in self.config._fields}
                json.dump(config,fh)
        except Exception as err:
            error(err)
            self.config = self.default_config

    def menu(self):
        menu = Menu()
        if sys.platform == "darwin":
            python_menu = Menu(menu,name="apple")
            menu.add_cascade(menu=python_menu)
            self.root['menu'] = menu
            python_menu.destroy()
        self.menu = {}

        # Main menu
        self.menu['main'] = Menu(menu)
        menu.add_cascade(menu=self.menu['main'],label=APPNAME)
        self.menu['main'].add_command(label=f"About {APPNAME}",command=self.about)
        self.menu['main'].add_separator()
        self.menu['main'].add_command(label='Settings...',command=self.settings)
        self.menu['main'].add_separator()
        self.menu['main'].add_command(label=f'Quit {APPNAME}',underline=0,command=self.quit,accelerator="Command+Q")
        self.root.bind("<Command-q>",self.quit)
        
        # File menu
        self.menu['file'] = Menu(menu)
        menu.add_cascade(menu=self.menu['file'], label='File')
        self.menu['file'].add_command(label=f'New...',underline=0,command=self.new, accelerator="Command+N")
        self.root.bind("<Command-n>",self.new)
        self.menu['file'].add_command(label=f'Open...',underline=0,command=self.open, accelerator="Command+O")
        self.root.bind("<Command-o>",self.open)
        
        # Edit menu
        self.menu['edit'] = Menu(menu)
        menu.add_cascade(menu=self.menu['edit'],label='Edit')

        # Project menu
        self.menu['project'] = Menu(menu)
        menu.add_cascade(menu=self.menu['project'],label='Project')
        self.menu['project'].add_command(label="Location")
        self.menu['project'].add_command(label="Timezone")
        self.menu['project'].add_command(label="Clock")

        # Data menu
        self.menu['data'] = Menu(menu)
        menu.add_cascade(menu=self.menu['data'],label='Data')
        self.menu['data'].add_command(label="Weather")
        self.menu['data'].add_command(label="SCADA")
        self.menu['data'].add_command(label="AMI")
        self.menu['data'].add_command(label="Prices")
        self.menu['data'].add_command(label="Tariffs")

        # Simulation menu
        self.menu['simulation'] = Menu(menu)
        menu.add_cascade(menu=self.menu['simulation'],label='Simulation') 
        self.menu['simulation'].add_command(label="Start")
        self.menu['simulation'].add_command(label="Pause")
        self.menu['simulation'].add_command(label="Resume")
        self.menu['simulation'].add_command(label="Stop")
        self.menu['simulation'].add_command(label="Restart")

        # Results menu
        self.menu['results'] = Menu(menu)
        menu.add_cascade(menu=self.menu['results'],label='Results')
        self.menu['results'].add_command(label="Export")
        self.menu['results'].add_command(label="Plot")

        # # View menu
        # self.menu['view'] = Menu(menu)
        # menu.add_cascade(menu=self.menu['view'],label='View')

        # # Window menu
        # self.menu['window'] = Menu(menu)
        # menu.add_cascade(menu=self.menu['window'],label='Window')

        # Help menu
        self.menu['help'] = Menu(menu)
        menu.add_cascade(menu=self.menu['help'], label='Help')
        self.menu['help'].add_command(label=f'{APPNAME} Help') 

        # Panels
        self.panels = {}

    def place(self,items,panel=None,**kwargs):
        for item in items:
            item.pack(**kwargs)
            self.items.append(item)

    def about(self):
        showinfo(title=f"About {APPNAME}",
            message=f"Version 1.0\nCopyright (c) 2024, Eudoxys Sciences LLC",
            icon='info')

    def settings(self):
        showerror(title="Error",message="Not implemented yet")

    def new(self,*args,**kwargs):
        id = kwargs["id"] if "id" in kwargs else 0
        self.name = f"untitled_{id}"
        file = os.path.join(self.path,self.name+".json")
        if os.path.exists(file):
            return self.new(id=id+1)
        self.root.title(f"{APPNAME} - {self.name}")
        self.file = None

    def open(self,*args,**kwargs):
        file = kwargs["file"] if "file" in kwargs else askopenfile()
        if isinstance(file,str):
            file = open(file,"r")
        self.model = json.load(file)
        self.file = file.name
        self.path = os.path.dirname(file.name)
        self.name = os.path.splitext(os.path.basename(file.name))[0]
        self.root.title(f"{APPNAME} - {self.name}")
        self.root.focus_force()

    def run(self):

        self.root.mainloop()

    def quit(self,*args,**kwargs):
        if not self.changed or askyesno(title="Quit?",message="Are you sure you want to quit?"):
            self.save_config()
            self.root.destroy()

if __name__ == "__main__":

    class TestApp(GldApp):
        config_file = "test.json"

    app = GldApp()
    choice = StringVar()
    choice.set("Monday")
    app.place([
        Label(text="Test title"),
        OptionMenu(app.root,choice,*["Monday","Tuesday"])
        ],side=TOP)
    app.run()
