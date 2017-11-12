#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "file_writer.h"

//Static Parameters
#define ALPHA 0.25   //initial probability of transmission, 1/4 in our case
#define Nodes 4		//Number of nodes, 4 in our case
#define T 40   	   //Total time slots

//3 possible node states
#define STATE_SILENT 0
#define STATE_RETX 	 1
#define STATE_TX 	 2


//probability of retransmission
static double p = 0.04;

//time in simulation
long t  = 0; //value of t is initially 0

//incrementally computing the mean delay
double update_mean_delay (double sample)
{
	static int i = 1;
	static double sample_mean = 0.0;

	sample_mean = sample_mean + (1.0/(double)i)*(sample - sample_mean);

	++i;
	return sample_mean;
}

int main()
{
	//seed the random variable
	srand (time(NULL));
	
	int state[Nodes];
	long tx_time[Nodes];

	FILE *delay_file = init_out_file("delays.txt");
	FILE *queue_file = init_out_file("queue.txt");
	
	unsigned long transmitted_packets = 0;
	unsigned long collision_counter = 0;
	unsigned long transmission_counter = 0;
	double mean_delay = 0.0;

	//initialize the state for each node, i.e silent
	for (int i = 0; i < Nodes; ++i)
	{
		state[i] = STATE_SILENT;
	}

	//iterate over the entire time_stot range and 
	//simulate transmissions using rand() fuction
	for (t = 0; t < T; ++t)
	{
		//iterate over each node
		for (int i = 0; i < Nodes; ++i)
		{
			//if the node is not transmitting
			if(state[i] == STATE_SILENT){
				//if allowed by initial probability ALPHA
				if ((double)rand()/(double)RAND_MAX <= ALPHA)
				{
					// node i will transmit
					tx_time[i] = t; //note the time of initial transmission
					state[i] = STATE_TX; //change the node state to transmitting
					transmission_counter ++;
				}
			}else{
				//if allowed by the retransmission probabilty limit
				if ((double)rand()/(double)RAND_MAX <= p){
					//node i will retransmit
					state[i] = STATE_TX;
					transmission_counter ++;
				}
			}
		}

		//After all nodes make a transmission verify/ check
		//transmission or collision status

		unsigned int tx_counter = 0; 	//transmissions counter
		unsigned int retx_counter = 0; 	//retransmission counter
		int last_tx_user = -1;			//assign blame in-case of a successful transaction
		
		for (int i = 0; i < Nodes; ++i)
		{
			if(state[i] == STATE_TX){
				++tx_counter;
				++retx_counter;
				last_tx_user = i;
			} else if (state[i] == STATE_RETX)
			{
				++retx_counter;
			}
		}

		if(tx_counter == 1){
			state[last_tx_user] = STATE_SILENT;  	//bring back to ground state
			transmitted_packets++;				 	//increment the number of transmitted packets
			long delay = t - tx_time[last_tx_user]; //calculate delay for the transaction
			write_long(delay_file, delay);			//log the delay
			mean_delay = update_mean_delay(delay);  //calculate the mean delay
		}else if(tx_counter > 1){
			//collision: more than one transactions occured
			collision_counter ++; //track the number of collisions
			for (int i = 0; i < Nodes; ++i)
			{
			//retransmit state	
				if(state[i] == STATE_TX){
					state[i] = STATE_RETX;
				}
			}
		}
		write_uint(queue_file, retx_counter);		//log retransmissions
	}

	printf("p = %.3f\n", p);
	printf("Time Slots: %d\n", T);
	printf("Total Attempted Transmissions: %lu\n", transmission_counter);
	printf("Transmitted packets: %lu\n", transmitted_packets);
	printf("Ratio (Transmitted:Attempted) :   %f\n", (double)transmitted_packets/(double)transmission_counter);
	printf("Mean delay: %f\n", mean_delay);


	fclose(delay_file); //close files
	fclose(queue_file);  //close files

	return 0;
}