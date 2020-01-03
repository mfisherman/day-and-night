# Day and Night
[![pipeline status](https://gitlab.com/mfisherman/day-and-night/badges/master/pipeline.svg)](https://gitlab.com/mfisherman/day-and-night/commits/master)

This project contains two applications (`day` and `night`) with the goal to start a server only if required and to shut it down if there is no traffic over a certain period of time.
An example purpose is the following: You have a server at home which should run only if needed.
The application `night` is then installed on the server.
If you want to reach the server, you can wake up the server by running `day` on a small device, such as a Raspberry Pi.
More details below.


## Overview

The respository contains two applications: `day` and `night`


### day

A tool which listens for ARP requests and wakes up a remote computer by Wake On Lan (WOL).
You can configure the application such that the WOL is sent only when the number of ARP requests exceeds a certain threshold in a certain time window.
You can set the threshold for each hour, e.g. allowing you to set it higher during night and lower during day.
In the use case described above, you would install this application on a small device such as a Raspberry Pi.


### night

A tool which listens for IPV4 traffic.
If there is little traffic over some period of time (you can configure this threshold), the computer - on which the application runs - is shut down.
Since not all traffic is equally important, you can assign weights to traffic that runs over some ports.
E.g. a single ssh packet should immediately exceed the set threshold, such that the server does not shut down when an ssh connection is running. 


## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.


### Prerequisites

The following libraries need to be installed

* liblog4cpp5-dev for logging
* libpcap-dev to sniff for packets
* libjsoncpp-dev to read the configuration files

```
sudo apt-get install liblog4cpp5-dev libpcap-dev libjsoncpp-dev
```


### Installing

Clone the repository and then run the cmake script:

```
mkdir build
cmake ..
make
```

### Running

The applications take one single parameter as input: the path to the configuration file


```
./main_day /my/path/to/config_day.json
```

```
./main_night /my/path/to/config_night.json
```

Since the applications use libpcap, you must make sure that they have appropriate privileges.
See e.g. the [tcpdump.org](https://www.tcpdump.org/manpages/pcap.3pcap.html) site at the paragraph "Under Linux:".


### Configuration

There is a configuration file each: `config_day.json` and `config_night.json`.
The repository already provides these files with values that have been proven to be useful.
For a quick start, change the values of the following parameters:

- host_ip and wol_address in config_day.json
- host_ip in config_night.json

#### config_day.json

```
{
    "iface_name": the interface on which the application should listen for ARP requests
    "log_file_name": path to where the application should write the log file

    "wol_address": the MAC address to which the WOL should be sent
    "wol_log_name": the log name of CWOL 

    "main_log_name": the log name of the main function
    "arp_listener_log_name": the log name of CARPListener
    "arp_listener_interval_msec": the interval for which the ARP requests are considered
    "arp_listener_frequency": the different thresholds (24 in total, one entry per hour); if the number of ARP requests exceeds the given threshold in the given interval, a WOL requests is sent
    "sniffer_log_name": the log anme of CSnifferListener
    "ethernet_parser_log_name": the log name of CEthernetParser
    "arp_parser_log_name": the log name of CArpParser
    "ip_parser_log_name": the log name of CIPParser
    "tcp_parser_log_name": the log name of CTCPParser

    "log_priority_level": the log priority level. One of "emerg", "fatal", "alert", "crit", "error", "warn", "notice", "info", "debug", "notset"
    "host_ip": the host ip that should be waken up
    "startup_timeout_ms": startup timeout (time in milliseconds before one listens for ARP reqeusts)
}
```

#### config_night.json

```
{
    "iface_name": interface on which the application should listen for IP traffic
    "log_file_name": path to where the application should write the log file

    "host_ip": ip of the host

    "main_log_name": the log name of the main function
    "arp_listener_log_name": the log name of CARPListener
    "sniffer_log_name": the log name of CSniffer
    "ethernet_parser_log_name": the log name of CEthernetParser
    "arp_parser_log_name": the log name of CArpParser
    "ip_parser_log_name": the log name of CIPParser
    "tcp_parser_log_name": the log name of CTCPParser

    "log_priority_level": the log priority level. One of "emerg", "fatal", "alert", "crit", "error", "warn", "notice", "info", "debug", "notset"

    "tcp_listener_timeout_msec": interval in milliseconds in which one should check if there is not sufficient IP traffic
    "tcp_listener_log_name": the log name of CTCPListener
    "startup_timeout_ms": startup timeout (time in milliseconds before one listens for IP traffic)
    "request_threshold": the threshold for IP traffic, if the timeout of CTCPListener is reached and the number of IP packets received is smaller than this threshold, the server is shut down

    "port_weight_list": a list of tuples of ports and weights that tell how important a packet for a given port is
}

```


### Coding style tests

The CMake script runs clang-tidy if installed.


## Contributing

Any contributions you make are **greatly appreciated**.

1. Fork the project
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a pull request


## License

This project is licensed under Aladdin Free Public License, see the [LICENSE](LICENSE) file for details or [the tldrlegal.com website](https://tldrlegal.com/license/aladdin-free-public-license#summary).


