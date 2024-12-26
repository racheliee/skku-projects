#!/usr/bin/python
from mininet.topo import Topo
from mininet.net import Mininet
from mininet.cli import CLI
from mininet.node import RemoteController

class MyTopology(Topo):
    def __init__(self):
        Topo.__init__(self)

        # Add hosts
        facultyWS = self.addHost('facultyWS', ip='10.0.1.2/24', defaultRoute="facultyWS-eth1")
        facultyPC = self.addHost('facultyPC', ip='10.0.1.4/24', defaultRoute="facultyPC-eth1")
        printer = self.addHost('printer', ip='10.0.1.3/24', defaultRoute="printer-eth1")

        labWS = self.addHost('labWS', ip='10.0.2.3/24', defaultRoute="labWS-eth1")
        studentPC1 = self.addHost('studentPC1', ip='10.0.2.2/24', defaultRoute="studentPC1-eth1")
        studentPC2 = self.addHost('studentPC2', ip='10.0.2.40/24', defaultRoute="studentPC2-eth1")

        itWS = self.addHost('itWS', ip='10.40.3.30/24', defaultRoute="itWS-eth1")
        itPC = self.addHost('itPC', ip='10.40.3.254/24', defaultRoute="itPC-eth1")

        examServer = self.addHost('examServer', ip='10.100.100.2/24', defaultRoute="examServer-eth1")
        webServer = self.addHost('webServer', ip='10.100.100.20/24', defaultRoute="webServer-eth1")
        dnsServer = self.addHost('dnsServer', ip='10.100.100.56/24', defaultRoute="dnsServer-eth1")

        trustedPC = self.addHost('trustedPC', ip='10.0.203.6/32', defaultRoute="trustedPC-eth1")
        guest1 = self.addHost('guest1', ip='10.0.198.6/32', defaultRoute="guest1-eth1")
        guest2 = self.addHost('guest2', ip='10.0.198.10/32', defaultRoute="guest2-eth1")

        # Add switches
        coreSwitch = self.addSwitch('s1')
        faculty_s = self.addSwitch('s2')
        student_s = self.addSwitch('s3')
        it_s = self.addSwitch('s4')
        datacenter_s = self.addSwitch('s5')

        # Add links
        self.addLink(facultyWS, faculty_s, port1=1, port2=1)
        self.addLink(facultyPC, faculty_s, port1=1, port2=2)
        self.addLink(printer, faculty_s, port1=1, port2=3)

        self.addLink(labWS, student_s, port1=1, port2=1)
        self.addLink(studentPC1, student_s, port1=1, port2=2)
        self.addLink(studentPC2, student_s, port1=1, port2=3)

        self.addLink(itWS, it_s, port1=1, port2=1)
        self.addLink(itPC, it_s, port1=1, port2=2)

        self.addLink(examServer, datacenter_s, port1=1, port2=1)
        self.addLink(webServer, datacenter_s, port1=1, port2=2)
        self.addLink(dnsServer, datacenter_s, port1=1, port2=3)

        # Connect switches to the core switch with specific ports
        self.addLink(faculty_s, coreSwitch, port1=4, port2=1)
        self.addLink(student_s, coreSwitch, port1=4, port2=2)
        self.addLink(it_s, coreSwitch, port1=4, port2=3)
        self.addLink(datacenter_s, coreSwitch, port1=4, port2=4)
        # internet connected to coreSwitch
        self.addLink(trustedPC, coreSwitch, port1=1, port2=5)
        self.addLink(guest1, coreSwitch, port1=1, port2=6)
        self.addLink(guest2, coreSwitch, port1=1, port2=7)

if __name__ == '__main__':
    topo = MyTopology()
    c0 = RemoteController(name='c0', controller=RemoteController, ip='127.0.0.1', port=6633)
    net = Mininet(topo=topo, controller=c0)
    net.start()
    CLI(net)
    net.stop()
