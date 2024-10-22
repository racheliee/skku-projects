#!/usr/bin/python

from mininet.topo import Topo
from mininet.net import Mininet
from mininet.cli import CLI
from mininet.link import TCLink ##Think about what this does, dig around the API doc and discuss in section!

class MyTopology(Topo):
    """
    A basic topology
    """
    def __init__(self):
        Topo.__init__(self)

        # Set Up Topology Here
        switch1 = self.addSwitch('switch1')    ## Adds a Switch
        switch2 = self.addSwitch('switch2')    

        Desktop = self.addHost('Desktop',ip='10.1.1.1')       ## Adds a Host named Desktop 
        Laptop = self.addHost('Laptop',ip='10.1.1.2')       ## Adds a Host named Laptop 
        Lights = self.addHost('Lights', ip='10.1.2.1')
        Fridge = self.addHost('Fridge', ip='10.1.2.2')

        self.addLink(switch1, Desktop, delay='20ms')      ## Add a link between switch1 and Desktop
        self.addLink(switch1, Laptop, delay='20ms')      ## Add a link between switch1 and Laptop
        self.addLink(switch2, Lights, delay='20ms')
        self.addLink(switch2, Fridge, delay='20ms')
        self.addLink(switch1, switch2, delay='20ms')


if __name__ == '__main__':
    """
    If this script is run as an executable (by chmod +x), this is
    what it will do
    """

    topo = MyTopology()   		 ## Creates the topology
    net = Mininet( topo=topo, link=TCLink )   	 ## Loads the topology, invokes TCLink 
    net.start()                      ## Starts Mininet

    # Commands here will run on the simulated topology
    CLI(net)

    net.stop()                       ## Stops Mininet