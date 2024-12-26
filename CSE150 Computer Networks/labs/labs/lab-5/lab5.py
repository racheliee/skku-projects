#!/usr/bin/python
from mininet.topo import Topo
from mininet.net import Mininet
from mininet.cli import CLI
from mininet.node import RemoteController

class MyTopology(Topo):
  def __init__(self):
    Topo.__init__(self)
   
    #Switches and links
    s1 = self.addSwitch('s1')
    s2 = self.addSwitch('s2')
    self.addLink(s1,s2)


    #Switch1 and devices 
    server = self.addHost('server',mac ='00:00:00:00:00:10', ip='10.1.1.1') 
    laptop = self.addHost('laptop',mac ='00:00:00:00:00:20', ip='10.1.1.2') 
    self.addLink(server,s1)
    self.addLink(laptop,s1)

    #Switch2 and devices 
    lights = self.addHost('Lights',mac ='00:00:00:00:00:30', ip='10.1.2.1')
    fridge = self.addHost('Fridge',mac ='00:00:00:00:00:40', ip='10.1.2.2')
    self.addLink(lights,s2)
    self.addLink(fridge,s2)
    

   
if __name__ == '__main__':
  #This part of the script is run when the script is executed
  topo = MyTopology() #Creates a topology
  c0 = RemoteController(name='c0', controller=RemoteController, ip='127.0.0.1', port=6633) #Creates a remote controller
  net = Mininet(topo=topo, controller=c0) #Loads the topology
  net.start() #Starts mininet
  CLI(net) #Opens a command line to run commands on the simulated topology
  net.stop() #Stops mininet