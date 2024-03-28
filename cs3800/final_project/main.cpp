#include "process.h"
#include "ioModule.h"
#include "processMgmt.h"

#include <chrono> // for sleep
#include <thread> // for sleep

enum stepActionEnum {noAct, admitNewProc, handleInterrupt, beginRun, continueRun, ioRequest, endRound, complete} stepAction;

void printFinalResults(list<Process> scheduler);
list<Process> runFifoScheduler(string file, long time, long sleepDuration, list<Process> mainProcessList, list<IOInterrupt> mainInterrupts, enum stepActionEnum);
list<Process> runShortestJobFirstScheduler(string file, long time, long sleepDuration, list<Process> mainProcessList, list<IOInterrupt> mainInterrupts, enum stepActionEnum);
list<Process> runShortestTimeRemainingScheduler(string file, long time, long sleepDuration, list<Process> processList, list<IOInterrupt> interrupts, enum stepActionEnum);
list<Process> runRoundRobinScheduler(string file, long time, long sleepDuration, list<Process> processList, list<IOInterrupt> interrupts, enum stepActionEnum);

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
	
	//Final Project Additions
	list<Process> fifoList;
	list<Process> sjfList;
	list<Process> strList;
	list<Process> roundRobinList;

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

    //processMgmt.readProcessFile(file);

	fifoList = runFifoScheduler(file, time, sleepDuration, processList, interrupts, stepAction);

	sjfList = runShortestJobFirstScheduler(file, time, sleepDuration, processList, interrupts, stepAction);

	strList = runShortestTimeRemainingScheduler(file, time, sleepDuration, processList, interrupts, stepAction);

	roundRobinList = runRoundRobinScheduler(file, time, sleepDuration, processList, interrupts, stepAction);

	cout << endl;
	cout << "============================ FIFO LIST RESULTS ===============================" << endl << endl;

	printFinalResults(fifoList);
	
	cout << endl;
	cout << "=========================== SHORTEST JOB FIRST RESULTS ===========================" << endl << endl;

	printFinalResults(sjfList);
	
	cout << endl;
	cout << "=========================== SHORTEST REMAINING TIME RESULTS ==============================" << endl << endl;

	printFinalResults(strList);

	cout << endl;
	cout << "=========================== ROUND ROBIN RESULTS =========================="<< endl << endl;

	printFinalResults(roundRobinList);

	return 0;
}


void printFinalResults(list<Process> scheduler)
{
	list<Process>::iterator select = scheduler.begin();
	int count = 1;
	
	//TODO: Make the scheduler return a list of completed processes in order of completion
	
	while (select != scheduler.end())
	{
		cout << setw(2) << "Process ID: " << select->id << " |";
		cout << setw(3) << " Completion #: " << count << " |";
		cout << setw(3) << " Arrival Time: " << select->arrivalTime << " |";
		cout << setw(3) << " Processor Time: " << select->processorTime << " |";
		cout << endl;
		cout << setw(3) << " Time started processing: " << select->startTime << " |";
		cout << endl;
		cout << setw(3) << " Total time spent waiting to process: " << select->totalWaitTime << " |";
		cout << endl;
		cout << setw(3) << " Time completed processing: " << select->endTime << " |";
		cout << endl;
		cout << "-------------------------------------------";
		cout << endl;
		
		count++;
		select++;
	} //end while
}

list<Process> runFifoScheduler(string file, long time, long sleepDuration, list<Process> processList, list<IOInterrupt> interrupts, enum stepActionEnum) 
{
	time = 0;
	int allDone = 0;
	int allNotNew = 0;
	bool interruptHandled = false;

	list<Process> arrivalList;
	list<Process> readyList;
	list<Process> doneList;
	
	list<Process>::iterator activeProcess = processList.begin();
    list<IOEvent>::iterator activeEvent = activeProcess->ioEvents.begin();
    list<Process>::iterator findNotActive = processList.begin();
    list<Process>::iterator findReady;
    list<Process>::iterator findArrival;
	
	//Final Project Addition
	list<Process>::iterator updateWait;
	
	ProcessManagement processMgmt(processList);
	IOModule ioModule(interrupts);

	processMgmt.readProcessFile(file);
	
	while(processMgmt.moreProcessesComing() || (allDone != static_cast<int>(processList.size())))
	{
		++time;
		
		processMgmt.activateProcesses(time);
		
		ioModule.ioProcessing(time);
		
		stepAction = noAct;
		
	//Construct the arrivalList
	if(arrivalList.empty() == true 
	   && allNotNew != static_cast<int>(processList.size()))
	{
	   findNotActive = processList.begin();
	   while(findNotActive != processList.end())
	   {
	      if(findNotActive->state == newArrival)
	      {
			allNotNew++;
			arrivalList.push_back(*findNotActive);
	      }
	      findNotActive++;
	   }
	}
	
	//Final Project Addition
	//Update totalWaitTime
	if(readyList.empty() != true)
	{
	   findReady = readyList.begin();
	   //cout<<"Entered Update IF"<<endl;
           while(findReady != readyList.end())
	   {
		updateWait = processList.begin();
		while(updateWait != processList.end())
	   	{
		  if(updateWait->id == findReady->id)
		  {
		     updateWait->totalWaitTime++;
			
		    // cout<<"Entered Update WHILE. ID: "<<findReady->id<<" Total Wait Time: "<<updateWait->totalWaitTime <<endl;
	   	  }
		  updateWait++;
		}
		findReady++;
	   }
	   findReady = readyList.begin();
	}

	//Maintaining a FIFO structure
	if((activeProcess->state != processing || activeProcess->state == done)
		 && interruptHandled != true)
	{
		activeProcess++;
	}

	//Looping from end of the list back to the start
	if(activeProcess->state != processing 
		&& allDone != static_cast<int>(processList.size()) 
		&& activeProcess == processList.end())
	{
	   activeProcess = processList.begin();
	}
  

	//Decision Logic
	if(activeProcess->state == processing && stepAction == noAct)
	{

	    //ioRequest
	    if(stepAction == noAct && activeProcess->ioEvents.empty() != true
			&& activeEvent->time == activeProcess->processorTime) 
	    {
			ioModule.submitIORequest(time, activeProcess->ioEvents.front(),
			 *activeProcess);
			activeProcess->ioEvents.pop_front();
			activeProcess->state = blocked;
		
			stepAction = ioRequest;
	    }

	    //Completion
	    else if(stepAction == noAct && 
			activeProcess->doneTime == activeProcess->reqProcessorTime)
	    {
	        activeProcess->state = done;
	        activeProcess->endTime = time; //Final Project Addtion
			stepAction = complete;
			doneList.push_back(*activeProcess); //Final Project Addition
			allDone++;
	    }

	    //Continue Processing
	    else if(stepAction == noAct && activeProcess->state != done)
	    {
	        stepAction = continueRun;
            activeProcess->processorTime++;
	        activeProcess->doneTime = activeProcess->processorTime;
		
			if(activeProcess->ioEvents.empty() != true)
			{
				activeEvent = activeProcess->ioEvents.begin();
			}
	    }
	}

	if(activeProcess->state != processing && stepAction == noAct)
	{
		//New to Ready
		if( arrivalList.empty() != true  && stepAction == noAct)
		{

			findArrival = arrivalList.begin();
			findNotActive = processList.begin();

			while(findNotActive->id != findArrival->id)
			{
				findNotActive++;
			}

			findNotActive->state = ready;
			stepAction = admitNewProc;
		  
			//Final project additon
			findNotActive->startTime = time;

			//End Addtion

			readyList.push_back(*activeProcess);
			arrivalList.pop_front();
		}

		//Handle interrupt
		else if(interrupts.empty() != true && stepAction == noAct)
		{
			while(findNotActive->id != interrupts.front().procID)
			{
				findNotActive++;
			}

			findNotActive->state = ready;
		
			interrupts.pop_front();
	       		stepAction = handleInterrupt;

			readyList.push_back(*findNotActive);

			findNotActive = processList.begin();
			interruptHandled = true;
		}
	   
		//Ready to Start Processing
		else if(readyList.empty() != true &&  stepAction == noAct)
		{
	       findReady = readyList.begin();
	       activeProcess = processList.begin();
	       while(activeProcess->id != findReady->id)
	       {
				activeProcess++;
	       }

	       activeProcess->state = processing;
	       stepAction = beginRun;
	       activeProcess->processorTime++;
		   activeProcess->doneTime = activeProcess->processorTime;

	       readyList.pop_front();

	       activeEvent = activeProcess->ioEvents.begin();

	       interruptHandled = false;
		}
	}

	//you should set the action variable based on what you do this time step. you can just copy and paste the lines below and uncomment them, if you want.
        //stepAction = continueRun;  //runnning process is still running
        //stepAction = ioRequest;  //running process issued an io request
        //stepAction = complete;   //running process is finished
        //stepAction = admitNewProc;   //admit a new process into 'ready'
        //stepAction = handleInterrupt;   //handle an interrupt
        //stepAction = beginRun;   //start running a process
 
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
            case endRound:		//Final Project Addition
              cout << "[endRoun]\t";
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
	
		return doneList;
	}


list<Process> runShortestJobFirstScheduler(string file, long time, long sleepDuration, list<Process> processList, list<IOInterrupt> interrupts, enum stepActionEnum)
{
	time = 0;
	int allDone = 0;
	int allNotNew = 0;
	bool interruptHandled = false;

	list<Process> arrivalList;
	list<Process> readyList;
	list<Process> doneList;

	ProcessManagement processMgmt(processList);
	IOModule ioModule(interrupts);
	
	list<Process>::iterator activeProcess = processList.begin();
	list<IOEvent>::iterator activeEvent = activeProcess->ioEvents.begin();
	list<Process>::iterator findNotActive = processList.begin();
	list<Process>::iterator findReady;
	list<Process>::iterator findArrival;
	
	//Final Project Addition
	list<Process>::iterator updateWait;

	processMgmt.readProcessFile(file);
	
	while(processMgmt.moreProcessesComing() || (allDone != static_cast<int>(processList.size())))
	{
		++time;
		
		processMgmt.activateProcesses(time);
		
		ioModule.ioProcessing(time);
		
		stepAction = noAct;
		
	//Construct the arrivalList
	if(arrivalList.empty() == true 
	   && allNotNew != static_cast<int>(processList.size()))
	{
	   findNotActive = processList.begin();
	   while(findNotActive != processList.end())
	   {
	      if(findNotActive->state == newArrival)
	      {
		  allNotNew++;
	 	  arrivalList.push_back(*findNotActive);
	      }
	      findNotActive++;
	   }
	}
	
	//Final Project Addition
	//Update totalWaitTime
	if(readyList.empty() != true)
	{
		findReady = readyList.begin();
		while(findReady != readyList.end())
		{
			updateWait = processList.begin();
			while(updateWait != processList.end())
			{
				if(updateWait->id == findReady->id)
				{
					updateWait->totalWaitTime++;
				}
				updateWait++;
			}
			findReady++;
		}
		findReady = readyList.begin();
	}
	//End Addition

	//Maintaining a FIFO structure
	if((activeProcess->state != processing || activeProcess->state == done)
		 && interruptHandled != true)
	{
		activeProcess++;
	}

	//Looping from end of the list back to the start
	if(activeProcess->state != processing 
		&& allDone != static_cast<int>(processList.size()) 
		&& activeProcess == processList.end())
	{
	   activeProcess = processList.begin();
	}
  
	//Decision Logic
	if(activeProcess->state == processing && stepAction == noAct)
	{

	    //ioRequest
	    if(stepAction == noAct && activeProcess->ioEvents.empty() != true
			&& activeEvent->time == activeProcess->processorTime) 
	    {
			ioModule.submitIORequest(time, activeProcess->ioEvents.front(),
			 *activeProcess);
			activeProcess->ioEvents.pop_front();
			activeProcess->state = blocked;
		
			stepAction = ioRequest;
	    }

	    //Completion
	    else if(stepAction == noAct && 
			activeProcess->doneTime == activeProcess->reqProcessorTime)
	    {
	        activeProcess->state = done;
	        activeProcess->endTime = time; //Final Project Addtion
			stepAction = complete;
			doneList.push_back(*activeProcess); //Final Project Addition
			allDone++;
	    }

	    //Continue Processing
	    else if(stepAction == noAct && activeProcess->state != done)
	    {

			stepAction = continueRun;
            activeProcess->processorTime++;
			activeProcess->doneTime = activeProcess->processorTime;
		
			if(activeProcess->ioEvents.empty() != true)
			{
				activeEvent = activeProcess->ioEvents.begin();
			}
	    }
	}

	if(activeProcess->state != processing && stepAction == noAct)
	{
		//New to Ready
		if( arrivalList.empty() != true  && stepAction == noAct)
		{

	      findArrival = arrivalList.begin();
	      findNotActive = processList.begin();

	      while(findNotActive->id != findArrival->id)
	      {
			findNotActive++;
	      }

	      findNotActive->state = ready;
	      stepAction = admitNewProc;
		  
	      //Final project additon
	      findNotActive->startTime = time;
	      //End Addtion

	      readyList.push_back(*findNotActive);
	      arrivalList.pop_front();
		}

		//Handle interrupt
		else if(interrupts.empty() != true && stepAction == noAct)
		{
			while(findNotActive->id != interrupts.front().procID)
			{
				findNotActive++;
			}

			findNotActive->state = ready;
		
			interrupts.pop_front();
	       	stepAction = handleInterrupt;

			readyList.push_back(*findNotActive);

			findNotActive = processList.begin();
			interruptHandled = true;

		}
	   
		//Ready to Start Processing
		else if(readyList.empty() != true &&  stepAction == noAct)
		{
			//Final Project Addition
		  
			//Inline definition of std::list.sort() for sorting processes by
			//processor time for clarity.
			readyList.sort([](const Process &first, const Process &second)
				{return first.reqProcessorTime < second.reqProcessorTime;});

			//End Addition

			findReady = readyList.begin();

			activeProcess = processList.begin();
			while(activeProcess->id != findReady->id)
			{
				activeProcess++;
			}

			activeProcess->state = processing;
			stepAction = beginRun;
			activeProcess->processorTime++;
			activeProcess->doneTime = activeProcess->processorTime;
		   
			readyList.pop_front();

			activeEvent = activeProcess->ioEvents.begin();

			interruptHandled = false;
		}
	}

	//you should set the action variable based on what you do this time step. you can just copy and paste the lines below and uncomment them, if you want.
        //stepAction = continueRun;  //runnning process is still running
        //stepAction = ioRequest;  //running process issued an io request
        //stepAction = complete;   //running process is finished
        //stepAction = admitNewProc;   //admit a new process into 'ready'
        //stepAction = handleInterrupt;   //handle an interrupt
        //stepAction = beginRun;   //start running a process

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
            case endRound:		//Final Project Addition
              cout << "[endRoun]\t";
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
	
		return doneList;
}


list<Process> runShortestTimeRemainingScheduler(string file, long time, long sleepDuration, list<Process> processList, list<IOInterrupt> interrupts, enum stepActionEnum)
{
	time = 0;
	int allDone = 0;
	int allNotNew = 0;
	bool interruptHandled = false;

	list<Process> arrivalList;
	list<Process> readyList;
	list<Process> doneList;

	ProcessManagement processMgmt(processList);
	IOModule ioModule(interrupts);
	
	list<Process>::iterator activeProcess = processList.begin();
	list<IOEvent>::iterator activeEvent = activeProcess->ioEvents.begin();
	list<Process>::iterator findNotActive = processList.begin();
	list<Process>::iterator findReady;
	list<Process>::iterator findArrival;
	
	//Final Project Addition
	list<Process>::iterator updateWait;

	processMgmt.readProcessFile(file);
	
	while(processMgmt.moreProcessesComing() || (allDone != static_cast<int>(processList.size())))
	{
		++time;
		
		processMgmt.activateProcesses(time);
		
		ioModule.ioProcessing(time);
		
		stepAction = noAct;
		
		//Construct the arrivalList
		if(arrivalList.empty() == true 
			&& allNotNew != static_cast<int>(processList.size()))
		{
			findNotActive = processList.begin();
			while(findNotActive != processList.end())
			{
				if(findNotActive->state == newArrival)
				{
					allNotNew++;
					arrivalList.push_back(*findNotActive);
				}
				findNotActive++;
			}
		}
	
	//Final Project Addition
	//Update totalWaitTime
	if(readyList.empty() != true)
	{
		findReady = readyList.begin();
		while(findReady != readyList.end())
		{
			updateWait = processList.begin();
			while(updateWait != processList.end())
			{
				if(updateWait->id == findReady->id)
				{
					updateWait->totalWaitTime++;
				}
				updateWait++;
			}
			findReady++;
		}
		findReady = readyList.begin();
	}
	//End Addition

	//Maintaining a FIFO structure
	if((activeProcess->state != processing || activeProcess->state == done)
		 && interruptHandled != true)
	{
		activeProcess++;
	}

	//Looping from end of the list back to the start
	if(activeProcess->state != processing 
		&& allDone != static_cast<int>(processList.size()) 
		&& activeProcess == processList.end())
	{
	   activeProcess = processList.begin();
	}
  
	//Decision Logic
	if(activeProcess->state == processing && stepAction == noAct)
	{

	    //ioRequest
	    if(stepAction == noAct && activeProcess->ioEvents.empty() != true
			&& activeEvent->time == activeProcess->processorTime) 
	    {
			ioModule.submitIORequest(time, activeProcess->ioEvents.front(),
			 *activeProcess);
			activeProcess->ioEvents.pop_front();
			activeProcess->state = blocked;
		
			stepAction = ioRequest;
	    }

	    //Completion
	    else if(stepAction == noAct && 
			activeProcess->doneTime == activeProcess->reqProcessorTime)
	    {
	        activeProcess->state = done;
	        activeProcess->endTime = time; //Final Project Addtion
			stepAction = complete;
			doneList.push_back(*activeProcess); //Final Project Addition
			allDone++;
	    }

	    //Continue Processing
	    else if(stepAction == noAct && activeProcess->state != done)
	    {

			stepAction = continueRun;
            activeProcess->processorTime++;
			activeProcess->doneTime = activeProcess->processorTime;
			activeProcess->remainingTime--; //Final Project Addition
		
			if(activeProcess->ioEvents.empty() != true)
			{
				activeEvent = activeProcess->ioEvents.begin();
			}
	    }
	}

	if(activeProcess->state != processing && stepAction == noAct)
	{
		//New to Ready
		if( arrivalList.empty() != true  && stepAction == noAct)
		{

	      findArrival = arrivalList.begin();
	      findNotActive = processList.begin();

	      while(findNotActive->id != findArrival->id)
	      {
			findNotActive++;
	      }

	      findNotActive->state = ready;
	      stepAction = admitNewProc;
		  
	      //Final project additon
	      findNotActive->startTime = time;
		  findNotActive->remainingTime = findNotActive->reqProcessorTime;
	      //End Addtion

	      readyList.push_back(*findNotActive);
	      arrivalList.pop_front();
		}

		//Handle interrupt
		else if(interrupts.empty() != true && stepAction == noAct)
		{
			while(findNotActive->id != interrupts.front().procID)
			{
				findNotActive++;
			}

			findNotActive->state = ready;
		
			interrupts.pop_front();
	       	stepAction = handleInterrupt;

			readyList.push_back(*findNotActive);

			findNotActive = processList.begin();
			interruptHandled = true;
		}
	   
		//Ready to Start Processing
		else if(readyList.empty() != true &&  stepAction == noAct)
		{
			//Final Project Addition
		  
			//Inline definition of std::list.sort() for sorting processes by
			//remaining time for clarity.
			readyList.sort([](const Process &first, const Process &second)
				{return first.remainingTime < second.remainingTime;});

			//End Addition

			findReady = readyList.begin();

			activeProcess = processList.begin();
			while(activeProcess->id != findReady->id)
			{
				activeProcess++;
			}

			activeProcess->state = processing;
			stepAction = beginRun;
			activeProcess->processorTime++;
			activeProcess->remainingTime--; //Final Project Addition
			activeProcess->doneTime = activeProcess->processorTime;
		   
			readyList.pop_front();

			activeEvent = activeProcess->ioEvents.begin();

			interruptHandled = false;
		}
	}

	//you should set the action variable based on what you do this time step. you can just copy and paste the lines below and uncomment them, if you want.
        //stepAction = continueRun;  //runnning process is still running
        //stepAction = ioRequest;  //running process issued an io request
        //stepAction = complete;   //running process is finished
        //stepAction = admitNewProc;   //admit a new process into 'ready'
        //stepAction = handleInterrupt;   //handle an interrupt
        //stepAction = beginRun;   //start running a process

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
            case endRound:		//Final Project Addition
              cout << "[endRoun]\t";
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
	
		return doneList;
}


list<Process> runRoundRobinScheduler(string file, long time, long sleepDuration, list<Process> processList, list<IOInterrupt> interrupts, enum stepActionEnum)
{
	time = 0;
	int allDone = 0;
	int allNotNew = 0;
	int maxRound = 20; //Final Project Addition
	int currentRound = 0; //Final Project Addition
	bool interruptHandled = false;

	list<Process> arrivalList;
	list<Process> readyList;
	list<Process> doneList;
	
	list<Process>::iterator activeProcess = processList.begin();
    list<IOEvent>::iterator activeEvent = activeProcess->ioEvents.begin();
    list<Process>::iterator findNotActive = processList.begin();
    list<Process>::iterator findReady;
    list<Process>::iterator findArrival;
	
	//Final Project Addition
	list<Process>::iterator updateWait;
	
	ProcessManagement processMgmt(processList);
	IOModule ioModule(interrupts);

	processMgmt.readProcessFile(file);
	
	while(processMgmt.moreProcessesComing() || (allDone != static_cast<int>(processList.size())))
	{
		++time;
		
		processMgmt.activateProcesses(time);
		
		ioModule.ioProcessing(time);
		
		stepAction = noAct;
		
	//Construct the arrivalList
	if(arrivalList.empty() == true 
	   && allNotNew != static_cast<int>(processList.size()))
	{
	   findNotActive = processList.begin();
	   while(findNotActive != processList.end())
	   {
	      if(findNotActive->state == newArrival)
	      {
			allNotNew++;
			arrivalList.push_back(*findNotActive);
	      }
	      findNotActive++;
	   }
	}
	
	//Final Project Addition
	//Update totalWaitTime
	if(readyList.empty() != true)
	{
	   findReady = readyList.begin();
	   //cout<<"Entered Update IF"<<endl;
           while(findReady != readyList.end())
	   {
		updateWait = processList.begin();
		while(updateWait != processList.end())
	   	{
		  if(updateWait->id == findReady->id)
		  {
		     updateWait->totalWaitTime++;
			
		    // cout<<"Entered Update WHILE. ID: "<<findReady->id<<" Total Wait Time: "<<updateWait->totalWaitTime <<endl;
	   	  }
		  updateWait++;
		}
		findReady++;
	   }
	   findReady = readyList.begin();
	}

	//Final Project Addition
	//Checking if the round is over
	if(currentRound == maxRound)
	{
		activeProcess->state = ready;
		readyList.push_back(*activeProcess);
		stepAction = endRound;
		
		activeProcess++;

		//Reset Round
		currentRound = 0;
	}

	//Maintaining a FIFO structure
	if((activeProcess->state != processing || activeProcess->state == done)
		 && interruptHandled != true)
	{
		activeProcess++;
	}

	//Looping from end of the list back to the start
	if(activeProcess->state != processing 
		&& allDone != static_cast<int>(processList.size()) 
		&& activeProcess == processList.end())
	{
	   activeProcess = processList.begin();
	}
  

	//Decision Logic
	if(activeProcess->state == processing && stepAction == noAct)
	{

	    //ioRequest
	    if(stepAction == noAct && activeProcess->ioEvents.empty() != true
			&& activeEvent->time == activeProcess->processorTime) 
	    {
			ioModule.submitIORequest(time, activeProcess->ioEvents.front(),
			 *activeProcess);
			activeProcess->ioEvents.pop_front();
			activeProcess->state = blocked;
		
			//Final Project Addition
			//Reset round
			currentRound = 0;

			stepAction = ioRequest;
	    }

	    //Completion
	    else if(stepAction == noAct && 
			activeProcess->doneTime == activeProcess->reqProcessorTime)
	    {
	        activeProcess->state = done;
	        activeProcess->endTime = time; //Final Project Addtion
			stepAction = complete;
			doneList.push_back(*activeProcess); //Final Project Addition
			//Reset Round
			currentRound = 0; //Final Project Addition
			allDone++;
	    }

	    //Continue Processing
	    else if(stepAction == noAct && activeProcess->state != done)
	    {
	        stepAction = continueRun;
		activeProcess->processorTime++;
	        activeProcess->doneTime = activeProcess->processorTime;
		
		//Final Project Addition
		currentRound++;

			if(activeProcess->ioEvents.empty() != true)
			{
				activeEvent = activeProcess->ioEvents.begin();
			}
	    }
	}

	if(activeProcess->state != processing && stepAction == noAct)
	{
		//New to Ready
		if( arrivalList.empty() != true  && stepAction == noAct)
		{

			findArrival = arrivalList.begin();
			findNotActive = processList.begin();

			while(findNotActive->id != findArrival->id)
			{
				findNotActive++;
			}

			findNotActive->state = ready;
			stepAction = admitNewProc;
		  
			//Final project additon
			findNotActive->startTime = time;

			//End Addtion

			readyList.push_back(*activeProcess);
			arrivalList.pop_front();
		}

		//Handle interrupt
		else if(interrupts.empty() != true && stepAction == noAct)
		{
			while(findNotActive->id != interrupts.front().procID)
			{
				findNotActive++;
			}

			findNotActive->state = ready;
		
			interrupts.pop_front();
			stepAction = handleInterrupt;

			readyList.push_back(*findNotActive);

			findNotActive = processList.begin();
			interruptHandled = true;
		}
	   
		//Ready to Start Processing
		else if(readyList.empty() != true && stepAction == noAct)
		{
			findReady = readyList.begin();
			activeProcess = processList.begin();
			while(activeProcess->id != findReady->id)
			{
				activeProcess++;
			}

			activeProcess->state = processing;
			stepAction = beginRun;
			activeProcess->processorTime++;
			activeProcess->doneTime = activeProcess->processorTime;

			readyList.pop_front();

			activeEvent = activeProcess->ioEvents.begin();

			interruptHandled = false;
		}
	}

	//you should set the action variable based on what you do this time step. you can just copy and paste the lines below and uncomment them, if you want.
        //stepAction = continueRun;  //runnning process is still running
        //stepAction = ioRequest;  //running process issued an io request
        //stepAction = complete;   //running process is finished
        //stepAction = admitNewProc;   //admit a new process into 'ready'
        //stepAction = handleInterrupt;   //handle an interrupt
        //stepAction = beginRun;   //start running a process
 
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
            case endRound:		//Final Project Addition
              cout << "[endRoun]\t";
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
	
		return doneList;
}

