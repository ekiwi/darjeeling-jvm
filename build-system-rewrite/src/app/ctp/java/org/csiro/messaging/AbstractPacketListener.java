package org.csiro.messaging;

public class AbstractPacketListener implements PacketListener {

	public void packetDelivered(Packet packet, byte tries) {
	}

	public void packetNotDelivered(Packet packet, byte tries) {
	}

	public void packetReceived(Packet packet) {
	}

}
