# BCIT PSU

Modified BCIT PSU - custom PCB, voltage measurement and readout, extra 3.3V output

## Log

BCIT PSU journal

2019/12/20 -----
- breadboarded LED circuit - realized that need to connect microcontroller and LED grounds to prevent flickering
- 36 bits for controlling LED non standard SPI protocol, had to bit bang outputs

2019/12/24 -----
- breadboarded LED circuit - realized ta 3.3V for VLED 3.3V voltage reg and LED display very hot - need around 100ohm resistor in front of VLED to limit current
- breadboarded op-amp circuit
- tried using two op amps - one inverting amp for attenuation, one inverting amp with unity gain to flip voltage back to positive
	- trialed with V+ = 5V and V- = -15V
	- with Rf = 3k and Ri = 10k supposed to get 4.5V for full 15V
	- problem: LM258P saturates at 3.55V with 5V positive supply, need +15V to get 4.5V out)
		- refer to output voltage swing (swing from rail prevents Vo from fully reaching V+)
- tried using voltage divider and op amp as voltage follower, 3k/13k = 3.46V for 5V 
	- this way we avoid saturation, but we can only measure up to 15.38V (=3.55V) before we saturate

- Next steps:
	- program ADC
	- make a board for the 7-seg display

2019/12/25 -----
- programmed ADC and made it write out to 7-segment display

2019/12/26 -----
- printed out PCB layouts for 7-seg display and main PCB

Main PCB
1) have to make the board slightly smaller to fit in chassis - fixed 2019/12/26
2) Should move ground pins and C6 slightly - fixed 2019/12/26
3) Should add a capacitor to the 3.3V output - fixed 2019/12/26
4) Should add an RC filter for the ADC (from online, use 500ohm resistor and 470pF cap) https://www.embeddedrelated.com/showarticle/110.php - fixed 2019/12/26
	- internal capacitance of Arduino ADC is around 14pF https://electronics.stackexchange.com/questions/67171/input-impedance-of-arduino-uno-analog-pins
5) Make holes smaller (1/2") instead of 1/4" - fixed 2019/12/26

LED PCB
1) Traces should be on bottom copper - fixed 2019/12/26
2) Need to add 3.3V input for VLED - fixed 2019/12/26
3) Make holes smaller (1/2") instead of 1/4" - fixed 2019/12/26

Good news - all the pads match up!

2019/12/28 -----
- submitted PCB quote to OMNI http://www.omnicircuitboards.com/filesandquote
- submitted PCB quote to Canadian Circuits https://www.canadiancircuits.com/request-a-quote/
- PCB cart quote: $40 for 5 prototype boards https://www.pcbcart.com/quote/prototype-pcb
- JLCPCB: $2.62 CAD for one prototype board, $13 shipping - 15/20 day delivery lead time https://jlcpcb.com/quote#/?orderType=1&stencilWidth=93&stencilLength=93&stencilCounts=5&stencilLayer=2&stencilPly=1.6&steelmeshSellingPriceRecordNum=A8256537-5522-491C-965C-646F5842AEC9&purchaseNumber=

PCB trace widths:
- For 1A current using 0.025 mil trace width, should use 1oz copper thickness https://www.pcbcart.com/article/content/copper-trace-and-capacity-relationship.html

2019/12/30 -----
- Fixed voltage reg pins (each voltage reg has different pin outs)
- Redrew PCB with ground plane on the bottom, minimal wiring on bottom (better grounding, fewer islands)

2019/12/31 -----
- Breadboarded low pass filter after ADC to Arduino
- Readings seem very much more stable!

2020/01/01 -----
- Final revision of PCBs (upped trace width to 35mil to handle up to 2A)
- Sent PCBs out for fab - JCL PLCB

2020/03/16 -----
Assembled PSU
Found that lowest output from 15V adj is 6.2V???? 5V offset???
Also fuse blows often when arduino attached because only rated for 350 mA

2020/03/17 -----
- fixed 5V offset from 15V adj (because pot was wired to 5V instead of ground)
- fixed negative voltage readings from moving average function (100 samples is too big, causes overflow)
- working!
- just need final assembly

2020/03/18 -----
- LED display doesn't work when unplugged from USB, but works when plugged into computer...
- likely not input A0 voltage or AREF
- likely Arduino not reading properly when not powered by USB, or LED screen not updating...
	- run a screen update test - ran through numbers from 0-9. Works even if not on USB. - No issue with 7-seg connections/grounding
	- run another ADC test (eg. blink a light at diff. speed depending on ADC reading) - ADC doesn't work. Frequency of light flashing doesn't update (but does when on USB)
- Try with the Arduino Uno instead of the nano and see what happens.
- Could be that the knockoff board AREF doesn't work properly on external power...

2020/03/21 ----
- solved issue: Vin is for 6-12V input only (feeds thru internal voltage regulator) - have to feed regulated 5V to 5V pin on Arduino instead
- Project Complete!