# SAFE - Safe Air For Everybody

This is the code repository for the SAFE project. This project wa funded by the [OpenPlant Biomaker Challenge 2019](https://www.biomaker.org/).

More information in our [hackster webpage](https://www.hackster.io/safe-team/safe-safe-air-for-everyone-5054a2).

The code used to read values from the PM SDS011 sensor was originally based in [this repository](https://github.com/zefanja/aqi "this repository").

## Configuration Instructions for Raspberry Pi (before first use)

### 1. Flashing the microSD

We followed the [official instructions](https://www.raspberrypi.org/documentation/installation/installing-images/README.md "official instructions") from Raspberry Pi. Specifically, we used *balenaEtcher* to flash [*Raspbian Buster with desktop and recommended software*](https://www.raspberrypi.org/downloads/raspbian/ "*Raspbian Buster with desktop and recommended software*") into a microSD card of 16GB.

### 2. Setting up Wifi

After being flashed, you should have a *boot* directory in the microSD, which you should be able to edit in your computer. Create a file named `wpa_supplicant.conf`with the network information for your raspberry pi to connect. Most of your networks should have this configuration (we were not able to connect to eduroam):

```
country=GB # Your 2-digit country code
ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
network={
    ssid="NETWORK_NAME"
    psk="NETWORK_PASSWORD"
    key_mgmt=WPA-PSK
}
```

### 3. Enabling ssh

Instead of having a keyboard and monitor to work directly with your raspberry pi, you can enable ssh. That is achieved simply by creating an empty file in the *boot* directory named `ssh`

### 4. (Optional) Changing raspberry pi hostname

In order to connect multiple units to the same network, it's necessary to change their identification for network access.

You should also have a *root* directory (or a directory with a similar name) in the microSD. You should edit the files `/etc/hostname` and `/etc/hosts`, by changing *raspberry* to whatever name you wish.


## Configuration instructions for the raspberry pi

### 1. SSHing into the unit

In Linux that's easily achieved in a terminal:
```bash
$ ssh pi@raspberrypi
```
The username/password is `pi` / `raspberry` by default, which you should change after.

### 2. Updating the unit

Just run the following commands after ssh'ing there:
```bash
$ sudo apt-get update
$ sudo apt-get upgrade
```

### 3. Downloading/configuring the actual code

This code will download/install the necessary packages and code.

```bash
$ sudo apt install git-core python-serial python-enum lighttpd
$ sudo apt-get install screen
$ sudo chown pi:pi /var/www/html/ 
$ cd /var/www/html/
$ git clone https://github.com/tjiagoM/safe-biomaker-challenge.git
$ cd safe-biomaker-challenge/
$ chmod +x aqi.py
$ chmod +x sleep_sensor.py
$ mkdir /var/www/html/logs
```

### 4. Defining configuration variables

From the previous terminal, you will be in the folder where the main python script (*aqi.py*) will be executed. Two important variables which you might consider changing are:

- `LOGS_LOCATION`: Where the reading logs will be stored
- `RASPBERRY_NAME`: The identification/name of the raspberry pi unit where the code will be run. This is important because when the file is created to save the sensor's readings, it will use this name
- `TIMESTEP`: How many seconds for each reading. By default the script will read every 30 seconds

## Running it!

We recommend running `screen` in the unit's terminal before executing any code so you can leave the code running without the need of always being connected to it. To leave the screen just do `CTRL+D` and to open the screen again run `screen -r` in the terminal next time you ssh.

### Reading from the sensor

You just need to execute the `aqi.py` script. You can specify the name of the experiment so the file name with the logs will be easier to identify:

```bash
$ ./aqi.py EXPERIMENT_NAME
```

### Stopping the sensor

When the sensor is connected to raspberry pi, its fan will be working. You can completely stop the sensor by running:

```bash
$ ./sleep_sensor.py
```

