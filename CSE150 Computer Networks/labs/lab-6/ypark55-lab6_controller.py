from pox.core import core
import pox.openflow.libopenflow_01 as of

log = core.getLogger()

class Firewall(object):
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
            # Create a new flow mod message
            msg = of.ofp_flow_mod()
            msg.match = of.ofp_match.from_packet(packet)
            msg.data = packet_in
            msg.idle_timeout = 45
            msg.hard_timeout = 600
            msg.actions.append(of.ofp_action_output(port=of.OFPP_NORMAL))
            msg.buffer_id = packet_in.buffer_id
            self.connection.send(msg)
            log.info("Packet Accepted - Flow Table Installed on Switch")

        def drop():
            # Create a new flow mod message to drop the packet
            msg = of.ofp_flow_mod()
            msg.match = of.ofp_match.from_packet(packet)
            msg.idle_timeout = 45
            msg.hard_timeout = 600
            msg.buffer_id = packet_in.buffer_id
            self.connection.send(msg)
            log.info("Packet Dropped - Flow Table Installed on Switch")

        # Define subnets as simple string checks for comparisons
        student_housing_subnet = "10.0.2."
        faculty_subnet = "10.0.1."
        it_department_subnet = "10.40.3."
        data_center_subnet = "10.100.100."
        trusted_pc_ip = "10.0.203.6"
        exam_server_ip = "10.100.100.2"

        # Check ARP
        arp_packet = packet.find('arp')
        if arp_packet:
            accept()
            return

        # Check for packet type first (ICMP, TCP, UDP)
        icmp_packet = packet.find('icmp')
        udp_packet = packet.find('udp')
        tcp_packet = packet.find('tcp')
        ip_header = packet.find('ipv4')

        if ip_header:
            src = str(ip_header.srcip)
            dst = str(ip_header.dstip)

            if icmp_packet:
                log.info("ICMP packet received")
                if self.icmp_is_allowed(src, dst):
                    accept()
                else:
                    drop()

            elif tcp_packet:
                log.info("TCP packet received")
                if self.tcp_is_allowed(src, dst):
                    accept()
                else:
                    drop()

            elif udp_packet:
                log.info("UDP packet received")
                if self.udp_is_allowed(src, dst):
                    accept()
                else:
                    drop()
            else:
                log.info("Unknown packet type received, dropping")
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
        
    def is_same_subnet(self, src, dst, subnet_prefix):
        """
        Helper function to check if both src and dst are within the same subnet.
        """
        return src.startswith(subnet_prefix) and dst.startswith(subnet_prefix)

    def icmp_is_allowed(self, src, dst):
        """
        Rule 1: ICMP traffic is forwarded only between the Student Housing LAN, 
        Faculty LAN, and IT Department subnets or between devices on the same subnet.
        """
        # check for discord server
        # discord_server_ip = "10.0.205.1"
        # if src == discord_server_ip and dst.startswith("10.0.2."):
        #     return True
        # if dst == discord_server_ip and src.startswith("10.0.2."):
        #     return True
        
        allowed_subnets = ["10.0.2.", "10.0.1.", "10.40.3."]
        for src_subnet in allowed_subnets:
            for dst_subnet in allowed_subnets:
                if src.startswith(src_subnet) and dst.startswith(dst_subnet):
                    return True
        
        same_subnet = ["10.100.100", "10.0.198.",  "10.0.2.", "10.0.1.", "10.40.3."]
        return any(self.is_same_subnet(src, dst, subnet) for subnet in same_subnet)

    def tcp_is_allowed(self, src, dst):
        """
        Rule 2: TCP traffic is forwarded only between the University Data Center, 
        IT Department, Faculty LAN, Student Housing LAN, trustedPC, or between devices 
        on the same subnet; only the Faculty LAN may access the exam server.
        """
        # check for discord server
        # discord_server_ip = "10.0.205.1"
        # if src == discord_server_ip and dst.startswith("10.0.2."):
        #     return True
        # if dst == discord_server_ip and src.startswith("10.0.2."):
        #     return True
        
        allowed_subnets = ["10.0.2.", "10.0.1.", "10.40.3.", "10.100.100."]
        same_subnet = ["10.100.100", "10.0.198.",  "10.0.2.", "10.0.1.", "10.40.3."]
        trusted_pc_ip = "10.0.203.6"
        exam_server_ip = "10.100.100.2"

        if dst == exam_server_ip: 
            if src.startswith("10.0.1."):
                return True
            return False
        for src_subnet in allowed_subnets:
            for dst_subnet in allowed_subnets:
                if src.startswith(src_subnet) and dst.startswith(dst_subnet):
                    return True
        if any(self.is_same_subnet(src, dst, subnet) for subnet in same_subnet):
            return True
        if src == trusted_pc_ip and dst.startswith(tuple(allowed_subnets)):
            return True
        if dst == trusted_pc_ip and src.startswith(tuple(allowed_subnets)):
            return True
        return False

    def udp_is_allowed(self, src, dst):
        """
        Rule 3: UDP traffic is forwarded only between the University Data Center, 
        IT Department, Faculty LAN, Student Housing LAN, or between devices on the same subnet.
        """
        # check for discord server
        # discord_server_ip = "10.0.205.1"
        # if src == discord_server_ip and dst.startswith("10.0.2."):
        #     return True
        # if dst == discord_server_ip and src.startswith("10.0.2."):
        #     return True
        
        allowed_subnets = ["10.0.2.", "10.0.1.", "10.40.3.", "10.100.100."]
        for src_subnet in allowed_subnets:
            for dst_subnet in allowed_subnets:
                if src.startswith(src_subnet) and dst.startswith(dst_subnet):
                    return True
        same_subnet = ["10.100.100", "10.0.198.",  "10.0.2.", "10.0.1.", "10.40.3."]
        return any(self.is_same_subnet(src, dst, subnet) for subnet in same_subnet)

def launch():
    """
    Starts the components
    """
    def start_switch(event):
        log.debug("Controlling %s" % (event.connection,))
        Firewall(event.connection)
    core.openflow.addListenerByName("ConnectionUp", start_switch)
