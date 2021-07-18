
#include <bits/stdc++.h>

using namespace std;


double returnRandom()
{
    return ((double)rand()/RAND_MAX);
}
double exp_distribute(double mean)
{
    double random_variable=returnRandom();
    while((random_variable==0) && (random_variable==1))
    {
        random_variable = returnRandom();
    }
    double exp= -mean*(log(random_variable));
    return exp;
}

struct Node
{
    double value;
    Node* nextnode;
    Node()
    {
        value=0.0;
        nextnode=NULL;
    }
};

struct List{
    Node first;
    Node second;
    
    List()
    {
        first.nextnode=&second;
    }

    
};

int main(int argc, char **argv)
{
    double total_simulation_time=100000.0;
    double lambda=100.0;
    double mu=1000.0;
    int capacity = 6; 		// No of chairs that each counter can have



    cout << "Enter the value of λ"<<endl;
    cin >> lambda;
    cout << "Enter the value of μ"<<endl;
    cin >> mu;




    if(lambda > mu)
    {
        cout<<"Arrival rate is greater than Service rate"<<endl;
        cout<<"Exiting..."<<endl;
        return 0;
    }


    double Total_Time = total_simulation_time;    
    double Arrival_Time = 1 / lambda;     
    double Departure_Time = 1 / mu;  

    List time,nextArrival_Time,nextDeparture_Time,busyTime,NoOfWorkers,lastEventTime,lastBusyTime,Waiting_Time;
    nextDeparture_Time.first.value=Total_Time;
    nextDeparture_Time.second.value=Total_Time;
    unsigned int n[2] = {0, 0};         
    unsigned int departures[2] = {0, 0};


    while (time.first.value < Total_Time && time.second.value < Total_Time)
    {
        // Arrival in Queue1 occurred
        
            if (nextArrival_Time.first.value < nextDeparture_Time.first.value)
            {
                time.first.value = nextArrival_Time.first.value;
                if(n[0]<capacity){
                NoOfWorkers.first.value = NoOfWorkers.first.value + n[0] * (time.first.value - lastEventTime.first.value);  // Update area under "NoOfWorkers" curve
                n[0]++;   
                
                if (n[0] == 1) 
                {
                    lastBusyTime.first.value = time.first.value;    
                    nextDeparture_Time.first.value = time.first.value + exp_distribute(Departure_Time);
                    Waiting_Time.first.value -= nextDeparture_Time.first.value - time.first.value;
                }

            	}
            	lastEventTime.first.value = time.first.value; 
                nextArrival_Time.first.value = time.first.value + exp_distribute(Arrival_Time);
            }
            // Departure in Queue 1 occurred
            else
            {
                time.first.value = nextDeparture_Time.first.value;
                NoOfWorkers.first.value = NoOfWorkers.first.value + n[0] * (time.first.value - lastEventTime.first.value); // Update area under "NoOfWorkers" curve
                n[0]--;  
                lastEventTime.first.value = time.first.value;  
                departures[0]++;       
                if (n[0] > 0)
                {
                    nextDeparture_Time.first.value = time.first.value + exp_distribute(Departure_Time);
                    Waiting_Time.first.value -= nextDeparture_Time.first.value - time.first.value;
                }
                else
                {
                    nextDeparture_Time.first.value = Total_Time;
                    busyTime.first.value = busyTime.first.value + time.first.value - lastBusyTime.first.value;
                }
            }

 // Arrival in Queue2 occurred
             if (nextArrival_Time.second.value < nextDeparture_Time.second.value)
            {
                time.second.value = nextArrival_Time.second.value;
                if(n[1]<capacity){
                NoOfWorkers.second.value = NoOfWorkers.second.value + n[1] * (time.second.value - lastEventTime.second.value);  // Update area under "NoOfWorkers" curve
                n[1]++;    
                
                if (n[1] == 1) 
                {
                    lastBusyTime.second.value = time.second.value;    
                    nextDeparture_Time.second.value = time.second.value + exp_distribute(Departure_Time);
                    Waiting_Time.second.value -= nextDeparture_Time.second.value - time.second.value;
                }

            	}
            	lastEventTime.second.value = time.second.value;   
                nextArrival_Time.second.value = time.second.value + exp_distribute(Arrival_Time);
            }
           // Departure in Queue 2 occurred
            else
            {
                time.second.value = nextDeparture_Time.second.value;
                NoOfWorkers.second.value = NoOfWorkers.second.value + n[1] * (time.second.value - lastEventTime.second.value); // Update area under "NoOfWorkers" curve
                n[1]--;    
                lastEventTime.second.value = time.second.value;   
                departures[1]++;           
                if (n[1] > 0)
                {
                    nextDeparture_Time.second.value = time.second.value + exp_distribute(Departure_Time);
                    Waiting_Time.second.value -= nextDeparture_Time.second.value - time.second.value;
                }
                else
                {
                    nextDeparture_Time.second.value = Total_Time;
                    busyTime.second.value = busyTime.second.value + time.second.value - lastBusyTime.second.value;
                }
            }
        
    }
    FILE * fp= fopen("2c.txt","w");

   

    printf("-    λ   = %.4f workers/sec\n", lambda);
    printf("-    μ      = %.4f workers/sec\n\n", mu);
     // Storing output in file

    fprintf(fp,"-    λ   = %.4f workers/sec\n", lambda);
    fprintf(fp,"-    μ      = %.4f workers/sec\n\n", mu);
    // Compute outputs
    
     
        double NoOfWorkers_checked,Response_Time,WorkerInQueue;

        NoOfWorkers_checked = NoOfWorkers.first.value/time.first.value;
        WorkerInQueue= (NoOfWorkers.first.value-busyTime.first.value)/time.first.value;
        Waiting_Time.first.value += (NoOfWorkers.first.value);
        Waiting_Time.first.value=(Waiting_Time.first.value)/(time.first.value*(departures[0] / time.first.value));
        Response_Time=((NoOfWorkers_checked)/(departures[0] / time.first.value));


        // Output results
        cout<<"Queue 1 output:"<<endl;
        cout<<"-    Average number of workers getting checked                                 = "<<NoOfWorkers_checked<<" workers "<<endl;
        cout<<"-    Average response time for workers in getting checked.                     = "<<Response_Time<<" sec "<<endl;
        cout<<"-    Average time for which a worker has to wait until getting checked.        = "<<Waiting_Time.first.value<<" sec "<<endl;
        cout<<"-    Average number of workers waiting in the queue before each officer.       = "<<WorkerInQueue<<" workers "<<endl;
        
        cout<<endl<<endl;


         // Storing output in file
      
        fprintf(fp,"Queue 1 output:\n");
        fprintf(fp,"-    Average number of workers getting checked                                 = %f workers \n", NoOfWorkers_checked);
        fprintf(fp,"-    Average response time for workers in getting checked.                     = %f sec \n", Response_Time);
        fprintf(fp,"-    Average time for which a worker has to wait until getting checked.        = %f sec \n", Waiting_Time.first.value);
        fprintf(fp,"-    Average number of workers waiting in the queue before each officer.       = %f workers \n", WorkerInQueue);
    
    


        NoOfWorkers_checked = NoOfWorkers.second.value/time.second.value;
        WorkerInQueue= (NoOfWorkers.second.value-busyTime.second.value)/time.second.value;
        Waiting_Time.second.value += (NoOfWorkers.second.value);
        Waiting_Time.second.value=(Waiting_Time.second.value)/(time.second.value*(departures[1] / time.second.value));
        Response_Time=((NoOfWorkers_checked)/(departures[1] / time.second.value));


     
        cout<<"Queue 2 output:"<<endl;
        cout<<"-    Average number of workers getting checked                                 = "<<NoOfWorkers_checked<<" workers "<<endl;
        cout<<"-    Average response time for workers in getting checked.                     = "<<Response_Time<<" sec "<<endl;
        cout<<"-    Average time for which a worker has to wait until getting checked.        = "<<Waiting_Time.second.value<<" sec "<<endl;
        cout<<"-    Average number of workers waiting in the queue before each officer.       = "<<WorkerInQueue<<" workers "<<endl;



        fprintf(fp,"Queue 1 output:\n");
        fprintf(fp,"-    Average number of workers getting checked                                 = %f workers \n", NoOfWorkers_checked);
        fprintf(fp,"-    Average response time for workers in getting checked.                     = %f sec \n", Response_Time);
        fprintf(fp,"-    Average time for which a worker has to wait until getting checked.        = %f sec \n", Waiting_Time.second.value);
        fprintf(fp,"-    Average number of workers waiting in the queue before each officer.       = %f workers \n", WorkerInQueue);


    fclose(fp);
    return 0;
}

