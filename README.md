# hid_hvgamenote
Fixes Havit Mouse side buttons.

This was build and tested on Arch Linux in kernel version: `**6.8.9-arch1-1**`

DKMS install was not needed, and was not tested!

## Prerequisites (on Ubuntu, Debian)
    $ sudo apt install build-essential linux-headers-generic dkms
## Prerequisites (Arch Linux, Arch based distro)
    $ pacman -S linux-lts-headers dkms
    $ pacman -S linux-headers dksm
    
(If you use a non-generic kernel, you should install the headers for that. DKMS is optional.)
## Building
    $ make
## Installing
    $ sudo make install
## DKMS
    $ sudo dkms add .
    $ sudo dkms install hid_hvgamenote/1.0
## Uninstalling
    $ make uninstall
## Uninstalling DKMS
    $ sudo dkms remove hid_hvgamenote/1.0 --all



# If the driver does not load automatically try the steps bellow.

## Force driver load:

Write two conf files according to your driver name in the following places:
- File:
    ```
    sudo vim /etc/modules-load.d/hid-hvgamenote.conf
    ```
    - Content:
        ```
        havit_driver
        hid-hvgamenote
        ```
-------------
- File:
    ```
    sudo vim /etc/modprobe.d/hid-hvgamenote.conf
    ```
    - Content:
        ```
        install hid-hvgamenote /sbin/modprobe -i -f hid-hvgamenote
        ```


## Force device to attach to the module

run `lsusb` to check the vendor id and device id
Generally the output will be like:  
`$ Bus 000 Device 000: ID vendorId:ProductId Name of the device` 
Example:
```
Bus 003 Device 003: ID 248a:5b4a Maxxter Wireless Receiver
```

With this information you will need to create a rule for udev:  
`in our rule we have two product Ids because it changes when connecting using the wireless dongle and usb cable, the driver module also have the two ids.`
- File:
    ```
    sudo vim /etc/udev/rules.d/10-havit_gaming.rules 
    ```
    - Content:
        ```
        SUBSYSTEM=="usb", ATTRS{idVendor}=="248a", ATTRS{idProduct}=="5b49" || ATTRS{idProduct}=="5b4a"), ACTION=="add", DRIVER=="hid_hvgamenote"
        ```

After this you can reboot or run `$ sudo udevadm control --reload-rules` then unplug and plug your device again to see if it works.


# To work in a new module driver:

First thing you will have to do is check if you really need a driver in some cases you can use [Input Remapper][https://aur.archlinux.org/packages/input-remapper-git] and just adjust your mouse buttons.

To check if your buttons are working or not use:

**get device event**:  
    `$ sudo libinput list-devices`

**Test events of the device**:  
    `$ sudo evtest --grab /dev/input/event5`  
If you press your buttons and it does not show any event you will need to build a module by your own probably.

**Check device event hex code**:  
To check device output and get the code mapping to put in your `module.c` file run:  
    `$ sudo hid-recorder`  
The output will be the codes you need to build your driver and if you start to click you will may see the events of your mouse in Hexadecimal or Binary.  

You can check driver examples in the official kernel modules repo: https://github.com/torvalds/linux/blob/master/drivers/hid/hid-xiaomi.c

# Credits

This repo was inspired by one person who passed through this process before to solve his own mouse issue `matega`.  
Follow the links to understand!  
https://askubuntu.com/questions/1302323/back-forward-mouse-buttons-dont-show-up-in-evtest  
https://github.com/matega/hid_mimouse  
