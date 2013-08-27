Setting up server
===============

With the pi running, we want to set up a network so that devices can connect to the pi directly, which is useful for serving local content.

The idea is to use nodejs as a server and execute command line commands via a client, such as a phone or another computer.

To set up an access point, the following tutorial is a good resource if one use the **[Edimax Nano USB WiFi adapter (EW-7811Un)][dongle]**. 

http://www.daveconroy.com/turn-your-raspberry-pi-into-a-wifi-hotspot-with-edimax-nano-usb-ew-7811un-rtl8188cus-chipset/

http://www.cyberciti.biz/faq/howto-ubuntu-debian-squeeze-dhcp-server-setup-tutorial/

```
sudo apt-get install bridge-utils hostapd
```

If you're using the above mentioned wifi dongle:
```
wget http://www.daveconroy.com/wp3/wp-content/uploads/2013/07/hostapd.zip
unzip hostapd.zip 
sudo mv /usr/sbin/hostapd /usr/sbin/hostapd.bak
sudo mv hostapd /usr/sbin/hostapd.edimax 
sudo ln -sf /usr/sbin/hostapd.edimax /usr/sbin/hostapd 
sudo chown root.root /usr/sbin/hostapd 
sudo chmod 755 /usr/sbin/hostapd
```

Do `sudo nano /etc/network/interfaces` and add
```
auto br0
iface br0 inet dhcp
bridge_ports eth0 wlan0
```

Do `sudo nano /etc/hostapd/hostapd.conf` and add
```
interface=wlan0
driver=rtl871xdrv
bridge=br0
ssid=YourRaspberryPi
channel=1
wmm_enabled=0
wpa=1
wpa_passphrase=APassword
wpa_key_mgmt=WPA-PSK
wpa_pairwise=TKIP
rsn_pairwise=CCMP
auth_algs=1
macaddr_acl=0
```

Do `sudo reboot` and test with `sudo hostapd -dd /etc/hostapd/hostapd.conf`

Add to startup by `sudo nano /etc/default/hostapd` and add

```
DAEMON_CONF="/etc/hostapd/hostapd.conf"
```

[dongle]:http://www.amazon.co.uk/dp/B003MTTJOY/

Optional tweaks
==============

Generate QR code to access the pi's server from a phone
http://goqr.me/
http://qrcode.kaywa.com/