#include "WPILib.h"


class Robot: public SampleRobot
{

    // Channels for the wheels
    const static int frontLeftChannel	= 2;
    const static int rearLeftChannel	= 3;
    const static int frontRightChannel	= 1;
    const static int rearRightChannel	= 0;
    const static int joystickChannel	= 0;
    AxisCamera *cam;
    Solenoid *exsolenoid;
    RobotDrive robotDrive;	// robot drive system
	Joystick stick;			// only joystick
	Compressor *c;
	LiveWindow *lw;
	DigitalInput *lowerlimitSwitch;
	DigitalInput *upperlimitSwitch;
	bool bigclick;
	Victor testmotor;

	//HSLImage *AxisCamera::GetImage();
public:
	Robot() :
			cam(),
			exsolenoid(0),
			robotDrive(2, 3, 1, 0),	// these must be initialized in the same order
			stick(joystickChannel),							// as they are declared above.
			c(),
			lw(NULL),
			lowerlimitSwitch(0),
			upperlimitSwitch(0),
			bigclick(),
			testmotor(4)
			//HSLImage()

	{
		robotDrive.SetExpiration(0.1);
		robotDrive.SetInvertedMotor(RobotDrive::kFrontLeftMotor, true);	// invert the left side motors
		robotDrive.SetInvertedMotor(RobotDrive::kRearLeftMotor, true);	// you may need to change or remove this to match your robot
	}
	void RobotInit(void)
	{
		lw = LiveWindow::GetInstance();
		exsolenoid = new Solenoid(0);
		exsolenoid->Set(true);
		lowerlimitSwitch = new DigitalInput(1);
		upperlimitSwitch = new DigitalInput(0);
		testmotor.Set(.25,0);
		//cam = CameraServer.getInstance().setImage(frame);
		c = new Compressor();
		c->Start();
	}
	void Autonomous()
		{
			robotDrive.SetSafetyEnabled(false);
			robotDrive.MecanumDrive_Cartesian(0.0, -0.25, 0.0); 	// drive forwards at a quarter speed; half speed is at -0.5
			Wait(2.0);
			robotDrive.MecanumDrive_Cartesian(0.0, 0.0, 0.0); 	// stop robot
		}
	/**
	 * Runs the motors with Mecanum drive.
	 */
	void OperatorControl()
	{
		robotDrive.SetSafetyEnabled(false);
		while (IsOperatorControl() && IsEnabled())
		{
        	// Use the joystick X axis for lateral movement, Y axis for forward movement, and Z axis for rotation.
        	// This sample does not use field-oriented drive, so the gyro input is set to zero.
			//robotDrive.MecanumDrive_Cartesian(-(stick.GetZ()), (stick.GetY()), -(stick.GetX() / 2));
			Wait(0.005); // wait 5ms to avoid hogging CPU cycles
			testmotor.Set(0.25,0);
			if (lowerlimitSwitch->Get())
			{
				bigclick = false;
			}
			if (upperlimitSwitch->Get())
			{
				bigclick = true;
			}
			if (bigclick == true)
			{
				exsolenoid->Set(true);
			}
			if (bigclick == false)
			{
				exsolenoid->Set(false);
		    }
		}
	}
};

START_ROBOT_CLASS(Robot);
