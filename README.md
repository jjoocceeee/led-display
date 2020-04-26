# Changing LED 
### By JoCee Porter & Kylee Fluckiger
## Technical Details: 
Using the skills we learned from 3 different labs (UART, Interrupts and DAC) we will created a system, that will receive a message from a Python terminal and change colors and brightness of an LED. The message recieved into the system will be through the UART protocol, and will trigger an interrupt. This interrupt is handled by receiving the message, and translatting the digital data, into a analog data, and fed to the LED.

#### Issues
Our original project was the control a Dot Matrix Display with the SPI protocol. As we were working on this project, we accidently broke the dot matrix display hardware component. After a video call with the Professor, we decided upon this new project.

### Updated Milestones: 
- Create DAC communication with LED. Turn on/off multiple LEDs in Dot Matrix Display through the serial protocol.  

- Accept input from keyboard through UART protocol. Show input was received and displayed on the computer monitor. 

- Write an interrupt-based message controller. LEDs in Dot Matrix Display will be cleared when interrupt is triggered. 

- Entire project working together. A user will be prompted to input a message. The input message will be converted to a LED Design, using our custom library “Calculate Matrix Pattern” and displayed on the Dot Matrix Display. When a button is pressed, the dot matrix display will clear and the user will be prompted for a new message to display. 

# Extra Work
The communication has been wrapped around with a Python script.

 