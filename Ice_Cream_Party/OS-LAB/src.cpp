#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <fstream>
#include <cmath>
#include <semaphore.h>

using namespace std;

int lambda;
int mu;
int alpha;
int beta;
int K;
int M;
int n;
int jj;
int x;
int y;
ofstream outFile;
mutex outMutex;
sem_t icecream_present;
sem_t family_ready;
sem_t vendor_refill;

double exponential(double t) {
    double u = (double)rand() / RAND_MAX;
    return -log(1 - u) / t;
}

string getTime()
{
 auto time = chrono::system_clock::now();
  auto time_milli = chrono::time_point_cast<chrono::milliseconds>(time);
     auto value = time_milli.time_since_epoch().count();
 auto j = value % 1000;
 time_t time_now = chrono::system_clock::to_time_t(time);
     char buffer[40];
      strftime(buffer, sizeof(buffer), "%H:%M:%S", localtime(&time_now));
sprintf(buffer, "%s.%03ld", buffer,j);
    return string(buffer);
}


void Family(int id)
{
for( int i = 0;i<n;i++)
{
    this_thread::sleep_for(chrono::milliseconds((int)(exponential(alpha)*1000)));

    outMutex.lock();
    outFile << "Family " << id << " is hungry"<< endl;
    outMutex.unlock();

    sem_wait(&icecream_present);
    sem_post(&family_ready);

    outMutex.lock();
    outFile << "Family " << id << " eats from the Pot" << getTime() <<endl;
    x = x + 1;
    outMutex.unlock();

    if(x==M)
    {
        y=y+1;
    outFile << "Vendor refills the ice cream " << y << " times"<<endl;
        x = 0;
    }

}
    outMutex.lock();
        outFile << "Family "<< id << " has eaten" << n << "times. Hence,exits."<<endl;
        outMutex.unlock();
    this_thread::sleep_for(chrono::milliseconds((int)(exponential(beta)*1000)));

}


int main()
{
  ifstream inFile("inp.txt");
  inFile >> K >> M >> n >> alpha >> beta >> lambda >> mu;
  inFile.close();

int o = K*M/n;

outFile.open("output.txt");
  sem_init(&icecream_present,M,M);
  sem_init(&family_ready,o,K);
  sem_init(&vendor_refill,0,M);

thread family_threads[K];
    for(int i=0;i<K;i++)
    {
    family_threads[i]=thread(Family,i+1);
    }

outFile << K << "threads created";

// thread vendor_threads[1];
    // vendor_threads[1]=thread(Vendor,2);

for(int i=0;i<K;i++)
{
    family_threads[i].join();
    }
// for(int i=0;i<1;i++)
// {
//     vendor_threads[i].detach();
// }

sem_destroy(&icecream_present);
sem_destroy(&family_ready);
sem_destroy(&vendor_refill);
outFile.close();
return 0;
}