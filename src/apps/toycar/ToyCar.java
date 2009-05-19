/*
 *	ToyCar.java
 * 
 *	Copyright (c) 2008 CSIRO, Delft University of Technology.
 * 
 *	This file is part of Darjeeling.
 * 
 *	Darjeeling is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	Darjeeling is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 * 
 *	You should have received a copy of the GNU General Public License
 *	along with Darjeeling.  If not, see <http://www.gnu.org/licenses/>.
 */
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
