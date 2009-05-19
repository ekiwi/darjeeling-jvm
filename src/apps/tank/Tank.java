
public class Tank
{
	/*
	
	public Tank()
	{
	}
	
	public void init()
	{
		// we'll use timer 1 for PWM
		Timer.initTimer(Timer.TIMER_1);
		
		// enable PWM for Timer1
		PWM.init(Timer.TIMER_1, PWM.PWM_BITRES_10);
		PWM.setEnabled(Timer.TIMER_1, true);
		PWM.setEnabled(Timer.TIMER_1, PWM.PWM_CHANNEL_A, true);
		PWM.setEnabled(Timer.TIMER_1, PWM.PWM_CHANNEL_B, true);
		PWM.setEnabled(Timer.TIMER_1, PWM.PWM_CHANNEL_C, true);
	}

	private static short clamp(short value)
	{
		if (value<0) value = (short)(-value);
		if (value>1023) value = 1023;
		return value;
	}
	
	public void drive(short left, short right, short turret)
	{
		// TODO: set motor direction
		
		PWM.setDuty(Timer.TIMER_1, PWM.PWM_CHANNEL_A, clamp(left));
		PWM.setDuty(Timer.TIMER_1, PWM.PWM_CHANNEL_B, clamp(right));
		PWM.setDuty(Timer.TIMER_1, PWM.PWM_CHANNEL_C, clamp(turret));
	}
	
	public static void main(String[] args)
	{
		Tank tank = new Tank();
		tank.init();
		
		// vroom!
		tank.drive((short)512, (short)512, (short)0);
		
	}
	*/

}
