## Ping

To Use this, we need to give root the right to open the ICMP socket

```shell
sudo sysctl -w net.ipv4.ping.group_range="0 0"
```

