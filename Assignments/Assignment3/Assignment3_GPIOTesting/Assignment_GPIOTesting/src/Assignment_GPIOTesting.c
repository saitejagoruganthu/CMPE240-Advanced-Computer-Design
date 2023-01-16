/*
===============================================================================
 Name        : Assignment_GPIOTesting.c
 Author      : Tirumala Saiteja Goruganthu
 SJSU ID	 : 016707210
 Course	 	 : CMPE240 - Advanced Computer Design
 Professor	 : Harry Li
 Description : This program written to test the GPIO functions of LPC1769.
 	 	 	   A toggle switch is used to send logic '1' or '0' to CPU where
 	 	 	   the input pin (P2.13) will read this logic state and trigger the
 	 	 	   output pin (P0.21) accordingly. Note that FIOSET and FIOCLR registers
 	 	 	   are used to control the output pin bit and FIOPIN register is used to
 	 	 	   control the input pin bit.
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>

// TODO: insert other include files here

// TODO: insert other definitions and declarations here
//Initialize the port and pin as outputs.
void GPIOinitOut(uint8_t portNum, uint32_t pinNum)
{
	if (portNum == 0)
	{
		LPC_GPIO0->FIODIR |= (1 << pinNum);
	}
	else if (portNum == 1)
	{
		LPC_GPIO1->FIODIR |= (1 << pinNum);
	}
	else if (portNum == 2)
	{
		LPC_GPIO2->FIODIR |= (1 << pinNum);
	}
	else
	{
		puts("Not a valid port!\n");
	}
}

//Initialize the port and pin as inputs.
void GPIOinitIn(uint8_t portNum, uint32_t pinNum)
{
	if (portNum == 0)
	{
		LPC_GPIO0->FIODIR |= (0 << pinNum);
	}
	else if (portNum == 1)
	{
		LPC_GPIO1->FIODIR |= (0 << pinNum);
	}
	else if (portNum == 2)
	{
		LPC_GPIO2->FIODIR |= (0 << pinNum);
	}
	else
	{
		puts("Not a valid port!\n");
	}
}

void setGPIO(uint8_t portNum, uint32_t pinNum)
{
	if (portNum == 0)
	{
		LPC_GPIO0->FIOSET = (1 << pinNum);
		printf("Pin 0.%d has been set.\n",pinNum);
	}
	else
	{
		puts("Only port 0 is used, try again!\n");
	}
}

//Deactivate the pin
void clearGPIO(uint8_t portNum, uint32_t pinNum)
{
	if (portNum == 0)
	{
		LPC_GPIO0->FIOCLR = (1 << pinNum);
		printf("Pin 0.%d has been cleared.\n", pinNum);
	}
	else
	{
		puts("Only port 0 is used, try again!\n");
	}
}

int main(void)
{
	//declare switch status variable and pin number
	uint32_t switchstatus;
	uint32_t switchpinnumber = 13;

	//LPC_PINCON->PINMODE4 = 0x08000000;

	//Set pin 0.21 as output
	GPIOinitOut(0,21);

	//Set pin 2.31 as input
	GPIOinitIn(2, 13);

	while(1)
	{
		//Get the switch status using FIOPIN Register
		switchstatus = (LPC_GPIO2->FIOPIN >> switchpinnumber) & 0x01;

		//Since all pins that are configured as Input will be in high state
		//due to the internal pull-ups which is why switch status is toggled
		//using logic '0'
		if (switchstatus == 1)
		{
			//Activate pin 0.21
			setGPIO(0,21);
		}
		else
		{
			clearGPIO(0, 21);
		}
	}
	//0 should never be returned, due to infinite while loop
	return 0;
}
