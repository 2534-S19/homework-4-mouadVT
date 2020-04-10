#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "homework4.h"

int main(void)
{
    char rChar;
    char *response = "\n\n\r2534 is the best course in the curriculum!\r\n\n";
    int i = 0; // the symbol to run the string in the while loop
    // TODO: Declare the variables that main uses to interact with your state machine.
    int pass;
    char key;
    bool finished;
/*    bool All_done(key){
        if( Success_key == '\0')
            finished = false;
 */
    // Stops the Watchdog timer.
    initBoard();
    // TODO: Declare a UART config struct as defined in uart.h.

    //       To begin, configure the UART for 9600 baud, 8-bit payload (LSB first), no parity, 1 stop bit.
    eUSCI_UART_ConfigV1 theConfig = {
            EUSCI_A_UART_CLOCKSOURCE_SMCLK, 19, 8, 0x55,
            EUSCI_A_UART_NO_PARITY,
            EUSCI_A_UART_LSB_FIRST,
            EUSCI_A_UART_ONE_STOP_BIT,
            EUSCI_A_UART_MODE,
            EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,
            EUSCI_A_UART_8_BIT_LEN };

    // TODO: Make sure Tx AND Rx pins of EUSCI_A0 work for UART and not as regular GPIO pins.
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN2,GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN3,GPIO_PRIMARY_MODULE_FUNCTION);

    // TODO: Initialize EUSCI_A0

    UART_initModule(EUSCI_A0_BASE, &theConfig);

    // TODO: Enable EUSCI_A0

    UART_enableModule(EUSCI_A0_BASE);

    while (1)
    {
        // TODO: Check the receive interrupt flag to see if a received character is available.
        //       Return 0xFF if no character is available.
        if (UART_getInterruptStatus(EUSCI_A0_BASE,EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG) == EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
            rChar = UART_receiveData(EUSCI_A0_BASE); // here the char get from mobaXterm to LaunchPad
        else
            rChar = 0xFF;

        // TODO: If an actual character was received, echo the character to the terminal AND use it to update the FSM.
        //       Check the transmit interrupt flag prior to tranmistting the character.

        if (rChar != 0xFF)
        {
          if (UART_getInterruptStatus(EUSCI_A0_BASE,EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG) == EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG)
            {
                UART_transmitData(EUSCI_A0_BASE, rChar);// here the char get from LaunchPad back to mobaXterm to display
            }
        }
                if ((rChar == '2'))
                {
                    charFSM(rChar);
                }
                if ((rChar == '5'))
                {
                    charFSM(rChar);
                }
                if ((rChar == '3'))
                {
                    charFSM(rChar);
                }
                if ((rChar == '4'))
                {
                    charFSM(rChar);
                }
                if (charFSM(rChar)|| (rChar == '4') )
                {
                    pass = 1;
                }
                else
                {
                    pass = 0;
                }


                // TODO: If the FSM indicates a successful string entry, transmit the response string.
                //       Check the transmit interrupt flag prior to transmitting each character and moving on to the next one.
                //       Make sure to reset the success variable after transmission.


                   if (pass == 1) // if condition satisfied the while loop start acting
                   {
                    while (response[i] != '\0')
                    {
                        if (UART_getInterruptStatus(EUSCI_A0_BASE,EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG) == EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG)
                        {
                            UART_transmitData(EUSCI_A0_BASE, response[i]); // each element on the string pushed to MobaXterm and displyed
                            i++;
                            key = response[i]; //success variable after transmission
                        }
                                if( key == '\0')
                                 finished = false;
                    }

                    }


            }
        }


void initBoard()
{
    WDT_A_hold(WDT_A_BASE);
}

// TODO: FSM for detecting character sequence.

bool charFSM(char rChar)
{
    bool finished ;

    typedef enum {S2, S25, S253, S2534} All_states; // there are 4 states
    static All_states state = S2;

    switch (state)
    {
    case S2:
        if (rChar == '2')
        {
            state = S25;
            finished = false;
        }
        else
        {
            state = S2;
            finished = false;
        }
        break;

    case S25:
        if (rChar == '5')
        {
            state = S253;
            finished = false;
        }
        else if (rChar == '2')
        {
            state = S25;
            finished = false;
        }
        else
        {
            state = S2;
            finished = false;
        }
        break;

    case S253:
        if (rChar == '3')
        {
            state = S2534;
            finished = false;
        }
        else if (rChar == '2')
        {
            state = S25;
            finished = false;
        }
        else
        {
            state = S2;
            finished = false;
        }
        break;

    case S2534:
        if (rChar == '4')
        {
            state = S2;
            finished = true; // the state where the FSM returns the true value
        }
        else if (rChar == '2')
        {
            state = S25;
            finished = false;
        }
        else
        {
            state = S2;
            finished = false;
        }

        break;
    }

    return  finished;
}


