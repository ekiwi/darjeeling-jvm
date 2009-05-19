package org.csiro.messaging;

public interface PacketListener
{
	
	public void packetReceived(Packet packet);
	public void packetDelivered(Packet packet, byte tries);
	public void packetNotDelivered(Packet packet, byte tries);

}
