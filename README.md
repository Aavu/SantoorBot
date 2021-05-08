### Dependencies
Requires cmake, fmt and avahi libraries. They can be installed by
```shell
$ sudo apt-get cmake install libfmt-dev libavahi-client-dev
```

and ofcourse ```gcc>=8.3.0``` compiler.

### Compile
To compile using cmake,
```shell
$ git clone https://github.com/Aavu/SantoorBot.git
$ cd SantoorBot && cmake ..
```
Before running, make sure to start the avahi-daemon by
```shell
$ sudo systemctl start avahi-daemon
```

### Run
To run the app,
```shell
$ ./SantoorBot
```

All the constant parameters are defined in ```include/Defines.h``` file.
