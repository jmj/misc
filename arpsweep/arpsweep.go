package main

import (
	"fmt"
	arp "github.com/mdlayher/arp"
	"github.com/3th1nk/cidr"
	"net/netip"
	"net"
	"time"
	"flag"
//	"os"
//	"errors"
)

var testip = "192.168.1.1"
var testcidr = "192.168.1.0/24"


var iprange string
var timeout int
var ifname string

func main() {

	flag.StringVar(&iprange, "i", "", "CIDR Range to scan")
	flag.IntVar(&timeout, "t", 300, "Time out (in milliseconds) for arp requests")
	flag.StringVar(&ifname, "e", "wlp0s20f3", "Physical ethernet interface to use")

	flag.Parse()

	netif, err := net.InterfaceByName(ifname)
	if err != nil {
		panic(err)
	}

	cidr, err := cidr.Parse(iprange)
	if err != nil {
		panic(err)
	}
	c, err := arp.Dial(netif)
	if err != nil {
		panic(err)
	}
	defer c.Close()


	cidr.Each(func(ip string) bool {
		pip := net.ParseIP(ip)
		if pip.Equal(cidr.Network()) {
			return true
		}
		if pip.Equal(cidr.EndIP()) {
			//fmt.Printf("\n")
			return false
		}

		ipaddr, err := netip.ParseAddr(ip)
		if err != nil {
			panic(err)
		}

		//c.SetDeadline(time.Now().Add(time.Millisecond*300))
		c.SetDeadline(time.Now().Add(time.Millisecond*time.Duration(timeout)))

		fmt.Printf("%s: ", ip)
		hw, err := c.Resolve(ipaddr)
		if err != nil {
			//panic(err)
			//if (!errors.Is(err, os.ErrDeadlineExceeded)) {
			//	panic(err)
			//}
			//fmt.Printf("\r")
			fmt.Print("\033[2K\r")
			return true
		}

		fmt.Printf("%s\n", hw.String())

		return true
	})
}
