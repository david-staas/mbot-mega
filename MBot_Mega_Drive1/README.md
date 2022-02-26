# MBot_Mega_Drive1

This program implements driving the [mBot Mega](https://store.makeblock.com/products/makeblock-mbot-mega-robot-kit) with the [Bluetooth Controller](https://www.makeblock.com/accessories/bluetooth-controller).

![](../doc/images/bt_controller2.png)

## General Background on the Controller

The [Makeblock library](https://github.com/Makeblock-official/Makeblock-Libraries) has this call that allows you to interrogate the controller's values:

``` 
MePS2.MeAnalog()
```

| Call              | Represents              | Values                     |
|-------------------|-------------------------|----------------------------|
| MePS2.MeAnalog(2) | Left Joystick - X Axis  | -255 (left) to 255 (right) |
| MePS2.MeAnalog(4) | Left Joystick - Y Axis  | -255 (down) to 255 (up)    |
| MePS2.MeAnalog(6) | Right Joystick - X Axis | -255 (left) to 255 (right) |
| MePS2.MeAnalog(2) | Right Joystick - X Axis | -255 (down) to 255 (up)    |

## General Background on the Motors for the Wheels

Mine is wired like this, and I followed the factory instructions:

| Wheel       | Port on Board | Motor # |
|-------------|---------------|---------|
| Left Front  | 2B            | 10      |
| Left Rear   | 2A            | 2       |
| Right Front | 1A            | 1       |
| Right Rear  | 1B            | 9       |

If you're looking at the wheel, motor.run(255) runs that wheel clockwise at 100% power, motor.run(-255) runs it counterclockwise at 100% power.

# Proportional Power Based on Joystick

My code runs the robot faster when the joystick is pushed harder in a direction; the joystick
values are used as part of the multiplier when determining the motor power for each wheel.

## Motor Limits at 50%
I also set a max. threshold for the motors at 50% (```MOTOR_LIMIT``` in the code).  You can
change this if you want, but the official documentation says not to run the motors at 100%
when you're using Bluetooth unless you're using Lithium Ion batteries, because there's not
enough power & you get Bluetooth dropouts.  At 50% I haven't had any problems.

# Mecanum Wheel Movements

| Desired Robot Direction | Left Front | Left Rear | Right Front | Right Rear |
|---|------------|-----------|-------------|------------|
| Forward | ↪ CCW ️    | ↪ CCW ️  | ↩ CW      | ↩ CW       |
| Backward | ↩ CW ️     | ↩ CW ️   | ↪ CCW ️    | ↪ CCW️   |
| Slide Left | ↩ CW     | ↪ CCW ️  | ↪ CCW ️    | ↩ CW   ️  |
| Slide Right | ↪ CCW ️   | ↩ CW ️  | ↩ CW ️     | ↪ CCW ️   |

