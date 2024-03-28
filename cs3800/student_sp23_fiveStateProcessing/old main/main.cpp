#include "process.h"
#include "ioModule.h"
#include "processMgmt.h"

#include <chrono> // for sleep
#include <thread> // for sleep

int main(int argc, char* argv[])
{
    // single thread processor
    // it's either processing something or it's not
//    bool processorAvailable = true;

    // vector of processes, processes will appear here when they are created by
    // the ProcessMgmt object (in other words, automatically at the appropriate time)
    list<Process> processList;

    // [STUDENT ADDITIONS]
    list<Process> arrivalList;
    list<Process> readyList;
    list<Process> runningList;
    list<Process> blockedList;
    list<Process> finishedList;
    list<Process> prevList;

    // this will orchestrate process creation in our system, it will add processes to 
    // processList when they are created and ready to be run/managed
    ProcessManagement processMgmt(processList);

    // this is where interrupts will appear when the ioModule detects that an IO operation is complete
    list<IOInterrupt> interrupts;   

    // this manages io operations and will raise interrupts to signal io completion
    IOModule ioModule(interrupts);  

    // Do not touch
    long time = 1;
    long sleepDuration = 50;
    string file;
    stringstream ss;
    enum stepActionEnum {noAct, admitNewProc, handleInterrupt, beginRun, continueRun, ioRequest, complete} stepAction;

    // Do not touch
    switch(argc)
    {
        case 1:
            file = "./procList.txt";  // default input file
            break;
        case 2:
            file = argv[1];         // file given from command line
            break;
        case 3:
            file = argv[1];         // file given
            ss.str(argv[2]);        // sleep duration given
            ss >> sleepDuration;
            break;
        default:
            cerr << "incorrect number of command line arguments" << endl;
            cout << "usage: " << argv[0] << " [file] [sleepDuration]" << endl;
            return 1;
            break;
    }

    processMgmt.readProcessFile(file);


    time = 0;
//    processorAvailable = true;

      int allDone = 0;
//	  bool ioInterrupt = false;
//    bool occupied = false;
//    bool assignedID = false;
        bool working = false;

        list<Process>::iterator activeProcess = processList.begin();
	list<IOEvent>::iterator activeEvent = activeProcess->ioEvents.begin();
	//list<IOInterrupt>::iterator interruption = interrupts.begin();
	list<Process>::iterator findBlock = processList.begin();

    
    //list<Process>::iterator prev;
    //list<Process>::iterator block;

 //   unsigned int activeProcID = 0;


    //keep running the loop until all processes have been added and have run to completion
    while(processMgmt.moreProcessesComing() || (allDone != static_cast<int>(processList.size())))  /* TODO add something to keep going as long as there are processes that arent done! */ 
    {
        //Update our current time step
        ++time;

        //let new processes in if there are any
        processMgmt.activateProcesses(time);

        //update the status for any active IO requests
        ioModule.ioProcessing(time);

        //If the processor is tied up running a process, then continue running it until it is done or blocks
        //   note: be sure to check for things that should happen as the process continues to run (io, completion...)
        //If the processor is free then you can choose the appropriate action to take, the choices (in order of precedence) are:
        // - admit a new process if one is ready (i.e., take a 'newArrival' process and put them in the 'ready' state)
        // - address an interrupt if there are any pending (i.e., update the state of a blocked process whose IO operation is complete)
        // - start processing a ready process if there are any ready


        //init the stepAction, update below
        stepAction = noAct;




	//Queue Logic
	//int numDone;
	//int numBlock;
	//int numReady;
	//int numNew;
	//int index;
//	unsigned int blockID;

//	cout<<"Before ID: ";
//	activeProcess->printProcess();



/*
	//find if there's any new processes to ready
	while(assignedID == false && activeProcess != processList.end() && !occupied)
	{
	   if(activeProcess->state == newArrival)
	   {
	      activeProcID = activeProcess->id;
	      assignedID = true;
	      //stepAction = admitNewProc;
	      cout<<"READY!"<<endl;
	   }
	   activeProcess++;
	}
	//Resetting iterator if there's no new processes to ready
	if(activeProcess == processList.end())
	{
	   activeProcess = processList.begin();
	   //activeProcess++;
	}

	//find if there's any processes to run
	while(assignedID == false && activeProcess != processList.end() && !occupied)
	{
	   if(activeProcess->state == ready)
	   {
	      activeProcID = activeProcess->id;
	      assignedID = true;
	      //stepAction = beginRun;
	      cout<<"RUN! "<<activeProcID<<endl;
	   }
	   activeProcess++;
	}

	//Bypass ID lookup if process is running
	if(occupied == true)
	{
	   activeProcID = prev->id;
	}

	//find the process with the ID
	while(activeProcess->id != activeProcID)
	{
	   activeProcess++;
	   prev = activeProcess;
	}

	cout<<"After ID: ";
	activeProcess->printProcess();
*/

	if(!(interrupts.empty()))
	{
	   cout<<"No interrupts!"<<endl;
/*
	    findBlock = processList.begin();
	    //block = blockedList.begin();
	    cout<<"SEG TEST"<<endl;
	    while((findBlock != processList.end()) && ioInterrupt != true)
	    {
		cout<<"Process ID: "<<findBlock->id<<" "<<interruption->procID<<endl;
		if(interruption->procID == findBlock->id)
		{
		   blockID = findBlock->id;
		   ioInterrupt = true;
		}
		//findBlock->printProcess();
		++findBlock;
	    }
	    cout<<"SEG TEST"<<endl;
	   */
	}
	
	//Maintaining a FIFO structure
/*	if((activeProcess->state != processing))
	{
		//cout<<"FIFO"<<endl;
		activeProcess++;
	}
*/
	if(!(working))
	{
	   activeProcess++;
	}


	if(allDone != static_cast<int>(processList.size()) 
		&& activeProcess == processList.end())
	{
	   activeProcess = processList.begin();
	}


	//Decision Logic
	if(activeProcess->state == processing && stepAction == noAct)
	{
	     //ioRequest
	     if(stepAction == noAct && 
		activeEvent->time == activeProcess->processorTime) 
	     {
		//cout<<"Start Event!"<<activeEvent->time<<endl;

		ioModule.submitIORequest(activeProcess->processorTime,
			activeProcess->ioEvents.front(), *activeProcess);
		
		activeProcess->state = blocked;
		stepAction = ioRequest;


		//blockedList.insert(block, *activeProcess);
			//activeProcess++;
			//assignedID = false;
			//activeProcID = 0;
			//occupied = false;
			//prev = prevList.end();
	     }	     
	     //Completion
	     else if(stepAction == noAct && 
		activeProcess->doneTime == activeProcess->reqProcessorTime)
	     {
	        activeProcess->state = done;
	        stepAction = complete;
		allDone++;
		working = false;
			//occupied = false;
			//assignedID = false;
			//activeProcID = 0;
			//prev = prevList.end();
	     }
	     //Continue Processing
	     else
	     {
	        stepAction = continueRun;
            	activeProcess->processorTime = activeProcess->processorTime+1;
	        activeProcess->doneTime = activeProcess->processorTime;
		

		activeEvent = activeProcess->ioEvents.begin();
//	        activeProcess->printProcess();
	     }
	}
	if(activeProcess->state != processing && stepAction == noAct)
	{
	   //New to Ready
	   if(activeProcess->state == newArrival && stepAction == noAct)
	   {
	      activeProcess->state = ready;
	      stepAction = admitNewProc;
	     

	    //  activeProcID = 0;
	    //  assignedID = false;
	   } 
	   //Handle interrupt
	   else if(interrupts.empty() != true && stepAction == noAct)
	   {
		while(findBlock->id != interrupts.front().procID)
		{
		   findBlock++;
		}
		if(findBlock->state == ready)
		{
			findBlock->state = blocked;
		}
		if(findBlock->state == blocked)
		   findBlock->state = ready;
		
		interrupts.pop_front();
	       	stepAction = handleInterrupt;
		working = false;
		findBlock = processList.begin();
	      
	   }
	   //Ready to Start Processing
	   else if(activeProcess->state == ready && stepAction == noAct)
	   {
	       activeProcess->state = processing;
	       stepAction = beginRun;
	       activeProcess->processorTime = 1;
	       activeProcess->doneTime = activeProcess->processorTime;

	       if(activeProcess->state != ready)
		 working=true;
	   }
	}


/*
	for(findBlock = processList.begin(); findBlock != processList.end(); ++findBlock)
	{
	    findBlock->printProcess();
	}
*/
/*	if(stepAction != noAct || stepAction != beginRun || stepAction != continueRun)
	{
	   prevProcess = activeProcess;
	}*/


	//you should set the action variable based on what you do this time step. you can just copy and paste the lines below and uncomment them, if you want.
        //stepAction = continueRun;  //runnning process is still running
        //stepAction = ioRequest;  //running process issued an io request
        //stepAction = complete;   //running process is finished
        //stepAction = admitNewProc;   //admit a new process into 'ready'
        //stepAction = handleInterrupt;   //handle an interrupt
        //stepAction = beginRun;   //start running a process
 
        

        //   <your code here> 






        // Leave the below alone (at least for final submission, we are counting on the output being in expected format)
        cout << setw(5) << time << "\t"; 
        
        switch(stepAction)
        {
            case admitNewProc:
              cout << "[  admit]\t";
              break;
            case handleInterrupt:
              cout << "[ inrtpt]\t";
              break;
            case beginRun:
              cout << "[  begin]\t";
              break;
            case continueRun:
              cout << "[contRun]\t";
              break;
            case ioRequest:
              cout << "[  ioReq]\t";
              break;
            case complete:
              cout << "[ finish]\t";
              break;
            case noAct:
              cout << "[*noAct*]\t";
              break;
        }

        // You may wish to use a second vector of processes (you don't need to, but you can)
        printProcessStates(processList); // change processList to another vector of processes if desired

        this_thread::sleep_for(chrono::milliseconds(sleepDuration));
    }

    return 0;
}
