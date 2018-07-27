/*
 * Copyright (c) 2013,
 * National Instruments Corporation.
 * All rights reserved.
 */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "AIO.h"
#include "MyRio.h"
#include "DIO.h"
#include <sys/time.h>
#include "Encoder.h"
#include <math.h>

extern NiFpga_Session myrio_session;

#if !defined(LoopDuration)
#define LoopDuration    60  /* How long to output the signal, in seconds */
#endif
#define PI 3.14159265
#if !defined(LoopSteps)
#define LoopSteps             1    /* How long to step between printing, in seconds */
#endif
/**
 * Overview:
 * Demonstrates using the analog input and output (AIO). Reads initial values
 * of two analog input channels from connector A and writes the difference of the read
 * values on connector B. Prints the values to the console.
 *
 * Instructions:
 * 1. Connect a DC voltage input (between 0 V and 5 V) to AI0 on connector A.
 * 2. Connect a DC voltage input (between 0 V and 5 V) to AI3 on connector A.
 * 3. Connect a voltmeter to AO0 on connector B.
 * 4. Run this program.
 *
 * Output:
 * The program reads the initial voltage on AI0 and AI3 on connector A, and
 * writes the sum to AO0 on connector B. The output is maintained for 60 s.
 * Input voltages and the sum are written to the console.
 *
 * Note:
 * The Eclipse project defines the preprocessor symbol for the NI myRIO-1900.
 * Change the preprocessor symbol to use this example with the NI myRIO-1950.
 */

void delay(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

int run(){
	NiFpga_Status status;
	    //Encoder Stuff
	    MyRio_Encoder encC0;
	    uint8_t selectReg;
	    uint32_t steps;
	    /*const char* direction;*/

	    encC0.cnfg = ENCC_0CNFG;
	    encC0.stat = ENCC_0STAT;
	    encC0.cntr = ENCC_0CNTR;

	    //Digital I/O Stuff
	    double ai_A0;
	    double ai_A3;
	    double ao_A0;
	    NiFpga_Bool dio_A7;
	    NiFpga_Bool dio_A4;
	    MyRio_Dio A7;
	    MyRio_Dio A4;

	    A7.dir = DIOA_70DIR;
		A7.out = DIOA_70OUT;
		A7.in = DIOA_70IN;
		A7.bit = 7;

		A4.dir = DIOA_70DIR;
		A4.out = DIOA_70OUT;
		A4.in = DIOA_70IN;
		A4.bit = 4;


		//Analog I/O Stuff
	    MyRio_Aio A0;
	    MyRio_Aio A3;
	    MyRio_Aio AO0;

	    time_t currentTime;
	    time_t finalTime;
	    time_t printTime;
	    printf("PID Control\n");

	    /*
	     * Specify the registers that correspond to the AIO channel
	     * that needs to be accessed.
	     *
	     * Assign constants to the weight and offset of the connector.
	     */
	    A0.val = AIA_0VAL;
	    A0.wght = AIA_0WGHT;
	    A0.ofst = AIA_0OFST;
	    A0.is_signed = NiFpga_False;

	    A3.val = AIA_3VAL;
	    A3.wght = AIA_3WGHT;
	    A3.ofst = AIA_3OFST;
	    A3.is_signed = NiFpga_False;

	    AO0.val = AOA_0VAL;
	    AO0.wght = AOA_0WGHT;
	    AO0.ofst = AOA_0OFST;
	    AO0.set = AOSYSGO;
	    AO0.is_signed = NiFpga_True;

	    /*
	     * Open the myRIO NiFpga Session.
	     * This function MUST be called before all other functions. After this call
	     * is complete the myRIO target will be ready to be used.
	     */
	    status = MyRio_Open();
	    if (MyRio_IsNotSuccess(status))
	    {
	        return status;
	    }
	    status = NiFpga_ReadU8(myrio_session, SYSSELECTC, &selectReg);
	    MyRio_ReturnValueIfNotSuccess(status, status,
	            "Could not read from the SYSSELECTC register!");

	    selectReg = selectReg | (1 << 0);

	    status = NiFpga_WriteU8(myrio_session, SYSSELECTC, selectReg);
	        MyRio_ReturnValueIfNotSuccess(status, status,
	            "Could not write to the SYSSELECTC register!")

	    Encoder_Configure(&encC0, Encoder_Enable | Encoder_SignalMode,
	                Encoder_Enabled | Encoder_StepDirection);
	    /*
	     * Read the scaling factors, initializing the struct
	     */
	    Aio_Scaling(&A0);
	    Aio_Scaling(&A3);
	    Aio_Scaling(&AO0);

	    /*
	     * Read from AI channels A/AI0 and A/AI3.
	     * Each channel is on Connector A.
	     */
	    ai_A0 = Aio_Read(&A0);
	    ai_A3 = Aio_Read(&A3);

	    /*
	     * Print out the logic level of each channel.
	     */
	    /*printf("A/AI0 = %f\n", ai_A0);
	    printf("A/AI3 = %f\n", ai_A3);*/

	    /*
	     * Calculate the difference between the channels
	     */
	    ao_A0 = 2;

	    /*
	     * Print out the desired logic level of B/AO0.
	     */
	    /*printf("B/AO0 = %f\n", ao_A0);*/

	    /*
	     * Write to channel B/AO0 to set it to the desired value.
	     */


	    /*
	     * Normally, the main function runs a long running or infinite loop.
	     * Keep the program running so that you can measure the output from B7 using
	     * an external instrument.
	     */
	    /*delay = void new delay(5);*/
	    int loopCount= 0;
	    struct timeval stop, start;
	    time(&currentTime);
	    finalTime = currentTime + LoopDuration;
	    printTime = currentTime;
	    /*double sin(double x);*/

	    double refMeasurement;
	    float prevCount=0.0;
	    float derror =0.0;
	    float integral=0.0;
	    float countsArray[6000]={0.0};
	    float error=0.0;
	    float errorArray[6000]={0.0};
	    //float derrorArray[12000]={0.0};
	    float commandArray[6000]={0.0};
	    int directionArray[6000]={0};
	    /*float dataArray[100000];*/
	    /*while (currentTime < finalTime)*/
	    while(loopCount<6000)
	    {
	    	//Enables Motor
	    	Dio_WriteBit(&A4, 1);

	    	float sinusoid1(float time){
	    		return (time/1)-(time*time*time)/6 + (time*time*time*time*time)/120 -
	    				(time*time*time*time*time*time*time)/5040
	    				+(time*time*time*time*time*time*time*time*time)/362880
	    				-(time*time*time*time*time*time*time*time*time*time*time)/39916800
	    				+(time*time*time*time*time*time*time*time*time*time*time*time*time)/6227020800; //13

	    	}
	    	float ref;
	    	if(loopCount<6000){
	    		ref= 250*sinusoid1(.001*loopCount);

	    	}
	    	/*else if(loopCount<5000){
	    		ref=100;
	    	}
	    	else{
	    		ref=200;
	    	}*/
	    		/*for(a=0; a<5.23598775598; a=a+0.78539816339){*/
	    			/*double ref = 250*sin(.6*a);*/
	    	int direction;
			steps = Encoder_Counter(&encC0);
			if ((Encoder_Status(&encC0) & Encoder_StDirection)== Encoder_Incrementing)
				{
					//direction = "incrementing";
				}
			else
				{
					/*direction = "decrementing";*/
				}

	    			/*printf("Ref: %f , Steps: %d\n", ref, steps);*/


	    			float counts =(float) (int32_t) steps;
	    			derror = counts-prevCount;
	    			prevCount=counts;
	    			error= ref-counts;
	    			integral= integral+error;
	    			//.0025 MAX KP
	    			float command= error*.003 - derror*.00025;// + integral*0;

	    			if(command>0){
	    				direction=1;
	    				//Positive Direction
	    			}
	    			else{
	    				command=command*-1;
	    				direction=0;
	    			}
	    			/*printf("Command: %f", command);
	    			printf(" Direction: %d", direction);*/
	    			/*printf("Steps %d, Direction: %d\n", steps, direction);*/
	    			//Speed Set Value
	    			Dio_WriteBit(&A7, direction);
	    			Aio_Write(&AO0, command);


	    			/*printf("\n");*/

	    			//Recording Data To Array
	    			countsArray[loopCount]=counts;

	    			errorArray[loopCount]=error;
	    			commandArray[loopCount]=command;
	    			//derrorArray[loopCount]=derror;
	    			directionArray[loopCount]=direction;
	    			loopCount = loopCount+1;
	    			delay(1000);

	    }


	    /*
	     * Close the myRIO NiFpga Session.
	     * This function MUST be called after all other functions.
	     */


	    /*
	     * Returns 0 if successful.
	     */

	    Aio_Write(&AO0, 0.0);
	    //Cleaning the set Values
	    FILE *loopC = fopen("loop.txt", "w+");
	    FILE *errorC = fopen("error.txt", "w+");
	    FILE *countsC = fopen("count.txt", "w+");
	    FILE *commandC = fopen("command.txt", "w+");
	    FILE *directionC = fopen("direction.txt", "w+");
	    int a;
	    for(a=0; a<loopCount; a=a+1){
	    	    	/*fprintf(fp, "Counts: %f, PrevCounts: %f, Error: %f, Command: %f\n",
	    	    			countsArray[a], prevCountsArray[a], errorArray[a], commandArray[a]);*/
	    	fprintf(loopC, "%i\n", a);
	    	fprintf(countsC, "%f\n", countsArray[a]);
	    	fprintf(errorC, "%f\n", errorArray[a]);
	    	fprintf(commandC, "%f\n", commandArray[a]);
	    	fprintf(directionC, "%i\n", directionArray[a]);

	    	/*fprintf(fp, "Counts:%f\n", 23.3445);*/
	    }
	    fclose(loopC);
	    fclose(countsC);
	    fclose(commandC);
	    fclose(directionC);
	    fclose(errorC);
	    return status;
}
/*double factorial(double denominator)
{

	double answer=1;
    while(denominator>1){

    	answer = answer*denominator;
    	denominator = denominator- 1;
    }
    return answer;
}
double power(double base, double raised){
	double answer1=1;
	while(raised>0){
		answer1 = answer1*base;
		raised -=1;
	}
	return answer1;
}
double term(double x, double order){
	double numerator = power(x, order);
	double denominator = factorial(order);
	return (numerator/denominator);
}*/


int main(int argc, char **argv)
{

	/*double a;
	for(a=0; a<5.23598775598; a=a+0.78539816339){
		double ref = 250*sin(.6*a);
		printf("%f\n", a);

		delay(1000000);
	}*/

	printf("%f\n", sin(1000*.001));


	run();
	return 0;
}








