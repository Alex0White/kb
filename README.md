# Keyboard project

I wanted to build a fully programmable wireless keyboard with a unique key layout.

So I did some research and discovered there were already a few people making hand wired keyboards. I realized I just needed to wire up a matrix where half the pins on the micro controller where outputs and the other half were inputs. From there I needed to measure the signals produced using a combination of the timings of the specific outputs calculated with the measurements taken from the inputs. This concept can be more easily understood with the below diagram of a basic keyboard wiring matrix.

![Alt text](https://pcbheaven.com/wikipages/images/howkeymatricesworks_1277649177.gif)


I wanted the keyboard to be wireless so I chose the Adafruit Feather nRF52 bluetooth micro controller. I also used an old cash register keyboard chassis because of its unique ortholinear key layout and space for the electronics. I replaced the Cherry black switches with Kailth box white clicky switches.  

Below are pictures of the progress of the build.

First test matrix using a small 4 by 4 grid.
![Alt text](https://github.com/Alex0White/kb/blob/master/Photos/First%20matrix%20test.jpg?raw=true "First test matrix")
Building out the larger matrix. 
![Alt text](https://github.com/Alex0White/kb/blob/master/Photos/Larger%20matrix.jpg?raw=true "Larger matrix")
Full working matrix using all the free pins on the Adafruit board. 
![Alt text](https://github.com/Alex0White/kb/blob/master/Photos/Full%20working%20matrix.jpg?raw=true "Full working matrix")
Test wiring all the available keys using and IO extender chip to add an additional matrix to connect the top two rows of keys.
Unfortunately signal was not clean enough to get accurate readings from the IO chip. 
![Alt text](https://github.com/Alex0White/kb/blob/master/Photos/Test%20extended%20matrix.jpg?raw=true "Test extended matrix")
