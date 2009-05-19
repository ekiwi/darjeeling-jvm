package testvm.classes;

public class Sink
{
	
	public short nodeId;
	public short route;
	public int lastHeard;
	
	private short seenSequenceNrIndex;
	private short[] seenSequenceNrEntries;
	
	public Sink(short nodeId)
	{
		this.nodeId = nodeId;
		seenSequenceNrEntries = new short[12];
		for (short i=0; i<seenSequenceNrEntries.length; i++)
			seenSequenceNrEntries[i] = -1;
	}
	
	public boolean seenSequenceNumber(short sequenceNr)
	{
		// check if we've seen this sequence number before
		for (short i=0; i<seenSequenceNrEntries.length; i++)
			if (seenSequenceNrEntries[i]==sequenceNr) return true;
		
		// add the sequence number to the buffer
		seenSequenceNrEntries[seenSequenceNrIndex] = sequenceNr;
		seenSequenceNrIndex = (short)((seenSequenceNrIndex+1) % seenSequenceNrEntries.length);
		
		return false;
	}

}
