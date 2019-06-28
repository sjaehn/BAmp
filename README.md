# B.Amplifier (BAmp)

Basic testing plugin for using BWidgets for LV2. See also eg-amp (https://github.com/drobilla/lv2/tree/master/plugins/eg-amp.lv2) and BadAmp (https://github.com/badosu/BadAmp).

Installation
------------

Clone it from this repository and call

```
make
sudo make install PREFIX=/usr
```

from the directory of the clone to install it to the default LV2 dirctory (usr/lib/lv2/).
For installation into an alternative directory (e.g., /usr/local/lib/lv2), change the variable `PREFIX` while installing:

```
sudo make PREFIX=/usr/local install
```


Dependencies: dev versions of lv2, cairo, and X11 (for linux systems)

Running
-------

After the installation Ardour, Carla, and any other LV2 host should automatically detect B.Amplifier.

If jalv is installed, you can also call it
```
jalv.gtk https://www.jahnichen.de/plugins/lv2/BAmp
```

to run it stand-alone and connect it to the JACK system.
