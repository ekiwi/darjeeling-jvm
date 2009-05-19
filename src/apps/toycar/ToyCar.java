import javax.fleck.boards.Motorcar;

public class ToyCar
{
    public static void main(String[] args)
	{
        // initialise the daughterboard
        Motorcar.init();
        Thread.sleep(2000);

        // actual position of the sensors on the car
        byte LEFT=5;
        byte FRONT=6;
            
        //behaviour of the car
        short SPEED_MAX = 255;
        short WALL_DIST = 350;
        short WALL_MAX = 350;
        short COLLISION_THRESHOLD = 425;
            
        Motorcar.setSpeed(SPEED_MAX);
            
        while (true)
        {
            // Read IR sensors. The closer the wall, the greater the value
            short irLeft = Motorcar.readSensor(LEFT); 
            short irFront = Motorcar.readSensor(FRONT);
                
            // if we are in front of an obstacle
            if( irFront > COLLISION_THRESHOLD )
            {
                // let's manoeuver
                Motorcar.drive( Motorcar.DRIVE_STOP );
                Thread.sleep(100);
                Motorcar.steer( Motorcar.STEER_LEFT );
                Motorcar.drive( Motorcar.DRIVE_BACKWARD );
                Thread.sleep(500);
                Motorcar.steer( Motorcar.STEER_RIGHT );
                Motorcar.drive( Motorcar.DRIVE_FORWARD );
                Thread.sleep(300);
                    
            }
            else {
                Motorcar.drive( Motorcar.DRIVE_FORWARD );
                    
                // Keep close to the left-hand wall
                if( irLeft < WALL_DIST )
                    Motorcar.steer(Motorcar.STEER_LEFT); 
                else
                    Motorcar.steer(Motorcar.STEER_RIGHT);
            }
                
            Thread.sleep(10);	
                
            }
        }
}
