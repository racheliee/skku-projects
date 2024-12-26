# Lab 5 controller skeleton
#
# Based on of_tutorial by James McCauley

from pox.core import core
import pox.openflow.libopenflow_01 as of

log = core.getLogger()


class Firewall (object):
    """
    A Firewall object is created for each switch that connects.
    A Connection object for that switch is passed to the __init__ function.
    """

    def __init__(self, connection):
        # Keep track of the connection to the switch so that we can
        # send it messages!
        self.connection = connection

        # This binds our PacketIn event listener
        connection.addListeners(self)

    def do_firewall(self, packet, packet_in):
        # The code in here will be executed for every packet

        def accept():
            # Write code for an accept function
            # important to include:

            # create a new flow of mod message
            msg = of.ofp_flow_mod()

            # Match criteria :check if packet matches the criteria
            msg.match = of.ofp_match.from_packet(packet)
            
            # data: set data
            msg.data = packet_in

            # timeouts: set idle & hard timeouts
            msg.idle_timeout = 45
            msg.hard_timeout = 600

            # actions: forward the packet to the normal port
            msg.actions.append(of.ofp_action_output(port=of.OFPP_NORMAL))
            
            # buffer_id: set buffer_id
            msg.buffer_id = packet_in.buffer_id
            
            # send: send the message to the switch
            self.connection.send(msg)
            
            # log
            print("Packet Accepted - Flow Table Installed on Switches")

        def drop():
            # Write code for a drop function
            # create a new flow of mod message
            msg = of.ofp_flow_mod()
            
            # Match criteria: check if packet matches the criteria
            msg.match = of.ofp_match.from_packet(packet)
            
            # timeouts: set idle & hard timeouts
            msg.idle_timeout = 45
            msg.hard_timeout = 600
            
            # buffer_id: set buffer_id    
            msg.buffer_id = packet_in.buffer_id
        
            # send: send the message to the switch
            self.connection.send(msg)
            
            # log
            print("Packet Dropped - Flow Table Installed on Switches")

        # Write firewall code
        # print("Example Code")

        # Hints:
        #
        # To check the source and destination of an IP packet, you can use
        # the header information... For example:
        #
        # ip_header = packet.find('ipv4')
        #
        # if src == "1.1.1.1":
        #   print "Packet is from 1.1.1.1"
        #
        # Important Note: the "is" comparison DOES NOT work for IP address
        # comparisons in this way. You must use ==.
        #
        # To drop packets, simply omit the action .

        
        laptop = '10.1.1.2'
        server = '10.1.1.1'
        lights = '10.1.2.1'
        fridge = '10.1.2.2'
        
        icmp = packet.find('icmp')
        udp = packet.find('udp')
        tcp = packet.find('tcp')
        arp = packet.find('arp')
        
        if arp:
            accept()
        
        elif icmp:
            ip_header = packet.find('ipv4')
            src = ip_header.srcip
            dst = ip_header.dstip
            if(dst == server):
                drop()
            else:
                accept()
            
        elif udp:
            # two way communication!!
            ip_header = packet.find('ipv4')
            src = ip_header.srcip
            dst = ip_header.dstip
            if(src == laptop and dst == server):
                accept()
            elif(src == server and dst == laptop):
                accept()
            elif(src == laptop and dst == fridge):
                accept()
            elif(src == fridge and dst == laptop):
                accept()
            else:
                drop()
        elif tcp:
            ip_header = packet.find('ipv4')
            src = ip_header.srcip
            dst = ip_header.dstip
            if(src == laptop and dst == server):
                accept()
            elif(src == server and dst == laptop):
                accept()
            elif(src == lights and dst == laptop):
                accept()
            elif(src == laptop and dst == lights):
                accept()
            else:
                drop()
        else:
            drop()

    def _handle_PacketIn(self, event):
        """
        Handles packet in messages from the switch.
        """

        packet = event.parsed  # This is the parsed packet data.
        if not packet.parsed:
            log.warning("Ignoring incomplete packet")
            return

        packet_in = event.ofp  # The actual ofp_packet_in message.
        self.do_firewall(packet, packet_in)


def launch():
    """
    Starts the components
    """
    def start_switch(event):
        log.debug("Controlling %s" % (event.connection,))
        Firewall(event.connection)
    core.openflow.addListenerByName("ConnectionUp", start_switch)
