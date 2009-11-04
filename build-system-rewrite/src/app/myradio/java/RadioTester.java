import javax.darjeeling.Darjeeling;
import javax.fleck.Leds;
import javax.radio.*;


public class RadioTester {
    public static void main(String args[])
    {
        boolean state=true;
		Radio.init();
		
		new Thread() {
			
			public void run() {
				while(true){
					byte[] received  = Radio.receive();
					Darjeeling.print("MESSAGE RECEIVED IS : ");
					for (int i = 0; i < received.length; i++){
						Darjeeling.print((int) received[i]);
					}
				}
			}
		}.start();
		
        new Thread(){
			int counter = 1;
        	public void run(){
                while(true)
                {
                	try {
        				//TODO: notice that most of the time Darjeeling.getNodeId() is not implemented,
                		//so we comment the message with node_id
                		//String str = String.concat(String.concat(String.concat("message no. ", Integer.toString(counter++)), " from node "), Integer.toString(Darjeeling.getNodeId()));
                		byte[] message = new byte[2];
                		message[0] = (byte) counter;
                		message[1] = message[0];//Darjeeling.getNodeId();
        				Radio.broadcast(message);
        				
        				//you can use also unicast for your message communication, comment out the above message creation and broadcast
        				//and replace it with the following, but notice that in  unicast you have to know node_id. Therefore,
        				//check if Darjeeling.getNodeId() is implemented in your Darjeeling port (check javax_radio_Radio.c files for your port)
/*        				if (Darjeeling.getNodeId() == 3){
                			byte[] message = new byte[2];
	                		message[0] = counter ++;
	                		message[1] = message[0];//Darjeeling.getNodeId();
        					Radio.send((short)10, message);
        				}
*/
        				//then wait for 20 seconds before sending the next message,
        			    Thread.sleep(20000);
        			    counter++;
        			} catch (Exception e) {
        				Darjeeling.print(String.concat("The problem is ", e.toString()));
        			}
                }
        	}
        }.start();
        
    }
}