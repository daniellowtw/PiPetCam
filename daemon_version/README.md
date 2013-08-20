FastcamD
======================

This ia a modified version of 

https://bitbucket.org/niklas_rother/rasperry-pi-userland.git as described in 
http://www.raspberrypi.org/phpBB3/viewtopic.php?t=50075&p=403012#p403012

# Installation
===================
```
chmod +x *.sh
./compile.sh
```

# Usage
=======

* ./raspifastcamd to test if it's working (ie does not complain about errors)
* ./start_camd.sh to start the daemon
* ./do_capture.sh to take a picture
* ./stop_camd.sh to stop the daemonpsa
* look at original.md for deprecated documents

# TODO
======

* Combine this with main program to control taking of picture using signals.
* Investigate whether it crashes the CPU after some time.