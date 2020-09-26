 ## <a name="code"></a>Controlling Motor PWM
The first code version moves the robot at a set speed for a specified period of time. What we what to do in the <i>FRCRobot-version-2</i> code is control the speed of the robot. To do that we are going to use <i>Pulse Width Modulation</i> (PWM) signals to power the motor . The following two videos give a good explaination of PWM signals.
- [Pulse Width Modulation (PWM) - Electronics Basics 23](https://www.youtube.com/watch?v=GQLED3gmONg)
- [Arduino Tutorial 8: Understanding Pulse Width Modulation (PWM)](https://www.youtube.com/watch?v=YfV-vYT3yfQ)

- Experiments on minimum motor power required to overcome inertia.
- For this version we'll only implement linear motion (forward and backward).  Angular motion (turning) will be reintroduced later when we learn about gyroscopes.  
- Separate control of each wheel.
