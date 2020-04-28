# RGB LED Controller 
### By JoCee Porter & Kylee Fluckiger
## System Overview
Using the skills we learned from 3 different labs (UART, Interrupts and DAC) we created a system that will receive a message from a Python terminal and change colors and brightness of an RGB LED. The message recieved into the system will be through the UART protocol, and will trigger an interrupt. This interrupt is handled by receiving the message, and translates the digital data into analog data which is fed to the LED to change its color. We have also provided a Python script to facilitate easy user control via a terminal.

We utilized the STM32 Discovery board to process the serial UART communication with the user and perform digital to analog (DAC) conversion to control the red and green channels of the RGB LED, thus changing the color as desired. In all, this board, a UART-USB cable, an RGB LED, resistors, jumper cables, and Python are required to fully implement our system; see the next section for details on how to find out more.

## Documentation
For an in-depth look at our system, hardware diagram, software flow, and a video demonstration, please see the Docs folder of this repo.

### Note
Our original project was the control a Dot Matrix Display with the SPI protocol. As we were working on this project, we accidently broke the dot matrix display hardware component. After a video call with the professor, we decided upon this new project. We have met all requirements outlined in the discussion and have satisfied our specific project requirements.


 
