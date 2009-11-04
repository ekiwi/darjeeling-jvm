package org.csiro.messaging;

import javax.fleck.Leds;
import javax.radio.Radio;
import javax.util.ArrayBag;

import org.csiro.debug.Debug;

public abstract class PacketHandler {

	private final static byte MAXTRIES = 4;
	private final static byte MAXPENDING = 6;
	private final static int SENDBACKOFFTIME = 100;

	private final ArrayBag<PacketListener> listeners;
	private final ArrayQueue<Packet> pendingPackets;

	private byte tries;

	public PacketHandler() {
		listeners = new ArrayBag<PacketListener>((short) 4);
		pendingPackets = new ArrayQueue<Packet>(MAXPENDING);
		tries = 0;

		// start a new receive thread
		new Thread() {
			public void run() {
				Debug.print(String.concat("Receive thread is : ", Integer.toString(Thread.getCurrentThreadId())));
				while (true) {
					try {
						receive();
					} catch (Throwable t) {
						Debug.print(String.concat("receive: ", t.toString(), "\n"));
					}
				}
			}
		}.start();

		// start a new send thread
		new Thread() {
			public void run() {
				Debug.print(String.concat("Send thread is : ", Integer.toString(Thread.getCurrentThreadId())));
				while (true) {
					// send a packet very SENDBACKOFFTIME milliseconds
					Thread.sleep(SENDBACKOFFTIME);
					try {
						send();
					} catch (Throwable t) {
						Debug.print(String.concat("throwable during send: ", t.toString(), "\n"));
					}
				}
			}
		}.start();

	}

	public void addPacketListener(PacketListener listener) {
		listeners.add(listener);
	}

	public void removePacketListener(PacketListener listener) {
		listeners.remove(listener);
	}

	protected abstract Packet wrap(byte data[]);

	public void send(Packet packet) {
		synchronized (pendingPackets) {
			if (!pendingPackets.offer(packet))
				throw new PacketBufferFullException();
		}

	}

	private void firePacketDeliveredEvent(Packet packet, byte tries) {
		for (short i = 0; i < listeners.size(); i++) {
			try {
				listeners.get(i).packetDelivered(packet, tries);
			} catch (Throwable t) {
				Debug.print("Exception during packet delivery! Listener=");
				Debug.print(listeners.get(i).toString());
				Debug.print(" throwable=");
				Debug.print(t.toString());
				Debug.print("\n");
			}
		}
	}

	private void firePacketNotDeliveredEvent(Packet packet, byte tries) {
		for (short i = 0; i < listeners.size(); i++) {
			try {
				listeners.get(i).packetNotDelivered(packet, tries);
			} catch (Throwable t) {
				Debug.print("Exception during packet not delivered notification! Listener=");
				Debug.print(listeners.get(i).toString());
				Debug.print(" throwable=");
				Debug.print(t.toString());
				Debug.print("\n");
			}
		}
	}

	private void send() {
		Packet packet = null;

		// try getting a packet from the queue
		synchronized (pendingPackets) {
			packet = pendingPackets.peek();
		}

		if (packet != null) {
			if (packet.isBroadcast()) {
				Leds.setLed(0, true);
				Radio.broadcast(packet.getBytes());
				Leds.setLed(0, false);

				synchronized (pendingPackets) {
					pendingPackets.remove();
				}
			} else {
				Leds.setLed(0, true);
				boolean ack = Radio.send(packet.getReceiverId(), packet.getBytes());
				Leds.setLed(0, false);
				tries++;

				// if the packet was delivered, or if the packet could not be
				// delivered (too many retries)
				// notify the listeners and remove the packet from the queue
				if (ack || tries > MAXTRIES) {
					synchronized (pendingPackets) {
						pendingPackets.remove();
					}

					if (ack)
						firePacketDeliveredEvent(packet, tries);
					else
						firePacketNotDeliveredEvent(packet, tries);

					tries = 0;
				}
			}

		}
	}

	private void receive() {

		// wait for a packet
		byte data[] = Radio.receive();

		// deserialise into a Message object
		Packet packet = null;
		try {
			packet = wrap(data);
		} catch (Exception ex) {
			Debug.print(String.concat("The problem is ", ex.toString()));
			// error decoding packet
			return;
		}

		if (packet == null) {
			// unknown packet type
			return;
		}

		// deliver the packet
		for (short i = 0; i < listeners.size(); i++)
			listeners.get(i).packetReceived(packet);

	}

}
