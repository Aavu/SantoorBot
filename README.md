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
Now on your Mac, inside Audio MIDI Setup, you should see "Santoor Bot" appearing. Connect it to the session and it will receive MIDI messages when you send MIDI to this session from Logic Pro X!

### Parameters
All the constant parameters are defined in ```include/Defines.h``` file.
