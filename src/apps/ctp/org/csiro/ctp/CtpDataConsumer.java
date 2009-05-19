package org.csiro.ctp;

public interface CtpDataConsumer
{
	
	public void dataReceived(short data[], short origin, short timeHasLived);

}
