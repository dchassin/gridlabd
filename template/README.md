# Create a new module

1. Create the new module folder
~~~
bash$ mkdir my_module
~~~

2. Copy the module template files to the new module folder
~~~
bash$ cp templates/module/* my_module
~~~

3. Rename the module header file
~~~
bash$ mv '{{{module}}}.h' my_module.h
~~~

4. Edit all the new files and replace {{{XXX}}} as follows

| Pattern        | Replacement       |
| -------------- | ----------------- |
| `{{{MODULE}}}` | `MY_MODULE`       |
| `{{{module}}}` | `my_module`       |
| `{{{CLASS1}}}` | `MY_FIRST_CLASS`  |
| `{{{class1}}}` | `my_first_class`  |
| `{{{CLASS2}}}` | `MY_SECOND_CLASS` |
| `{{{class2}}}` | `my_second_class` |
| `{{{CLASSN}}}` | `MY_NTH_CLASS`    |
| `{{{classN}}}` | `my_nth_class`    |

5. Add the new module to the modules Makefile

~~~
bash$ echo 'include my_module/Makefile.mk' >> modules.mk
~~~

6. Rebuild the system

~~~
bash$ make reconfigure && make -j30 system
~~~

# Create a class