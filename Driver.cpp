/*
Munem Rastgir
CSCI 340
OPERATING SYSTEM SIMULATION PROJECT

*/

#include <iostream>
#include <string>
#include <vector>
#include <locale>
#include<queue>
using namespace std;

//struct of a PCB
struct PCB {
	int priority;			//holds priority, memory and PID
	long long int memory;
	int PID;
	PCB* next;				//also hold pointer to next struct and 2 ints representing start and end of position in memory
	long long int start;
	long long int end;
};

struct memoryList {
	PCB* head;
};

void askRam(long long int& RAM)	//function that accepts int input and asks the user to input amount of RAM
{
	cout << "Enter the total RAM of your device up to 4 billion" << endl;
	cin >> RAM;
	if (cin.fail())		//if user input is invalid, clear result and go to while loop
	{
		cin.clear();
		cin.ignore(1000, '\n');
	}
	while (RAM < 1 || RAM > 4000000000)	//continues to ask user to enter correct RAM value
	{
		cout << "Invalid entry, number has to be between 1-4 billion" << endl;
		cin >> RAM;

		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1000, '\n');
		}
	}

}

void askDevice(int& device, string name)	//function that accepts int input and asks the user to input amount of printers/disks
{
	cout << "Enter the amount of " << name << " in your Computer" << endl;
	cin >> device;
	if (cin.fail())
	{
		cin.clear();
		cin.ignore(1000, '\n');
	}
	while (device < 1 || device > 10)
	{
		cout << "Invalid entry, number has to be between 1-10" << endl;
		cin >> device;

		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1000, '\n');
		}
	}
}

//Pushes new process into either the CPU if its empty or the appropriate priority queue
void scheduling(PCB head, PCB& CPU, vector<PCB>& a, vector<PCB>& b, vector<PCB>& c, vector<PCB>& d, vector<PCB>& e)
{
	if (head.priority == 0)			//5 if else statements starting from priority 0 to priority 4:
	{
		if (CPU.PID == -1)
			CPU = head;
		else a.push_back(head);
	}
	else if (head.priority == 1)
	{
		if (CPU.PID == -1)
			CPU = head;
		else if (CPU.priority < 1)
		{
			b.push_back(CPU);
			CPU = head;
		}
		else b.push_back(head);
	}
	else if (head.priority == 2)
	{
		if (CPU.PID == -1)
			CPU = head;
		else if (CPU.priority < 2)
		{
			c.push_back(CPU);
			CPU = head;
		}
		else c.push_back(head);
	}
	else if (head.priority == 3)
	{
		if (CPU.PID == -1)
			CPU = head;
		else if (CPU.priority < 3)
		{
			d.push_back(CPU);
			CPU = head;
		}
		else d.push_back(head);
	}
	else if (head.priority == 4)
	{
		if (CPU.PID == -1)
			CPU = head;
		else if (CPU.priority < 4)
		{
			e.push_back(CPU);
			CPU = head;
		}
		else e.push_back(head);
	}
}

bool num_check(string s)	//function checks if string input is a number
{
	string::iterator it = s.begin();
	while (it != s.end() && isdigit(*it)) 
		it++;
	return !s.empty() && it == s.end();
}

bool aValid(string x, vector<string>& values)	//function that checks if input has 2 numbers after the 'A'
{
	values.clear();
	string temp = "";
	for (string::iterator it = x.begin(); it != x.end(); it++)	//iterators to parse string for A x y
	{
		if (*it == ' ') {
			values.push_back(temp);					//if iterator is a space, store the string temp into vector
			temp = "";
		}
		else if (it != x.end() && *it != ' ')		//if iterator points to a char, add it to the string
		{
			temp += *it;
		}
	}
	values.push_back(temp);
	if (values.size() != 3)		//if vector doesnt have 3 strings, return false
		return false;
	else if (!num_check(values[1]) || !num_check(values[2]))	//else if the second and third strings arent numbers, return false
		return false;
	else if (stoi(values[1]) > 4 || stoi(values[1]) < 0 || stoll(values[2]) > 4000000000 || stoll(values[2]) < 1)
		return false;
	else return true;	//else return true
}

bool dValid(string x, vector<string>& values)	//function checks if input has a letter and a number
{
	values.clear();
	string temp = "";
	for (string::iterator it = x.begin(); it != x.end(); it++)	//iterators to parse string for Device #
	{
		if (*it == ' ') {
			values.push_back(temp);					//if iterator is a space, store the string temp into vector
			temp = "";
		}
		else if (it != x.end() && *it != ' ')		//if iterator points to a char, add it to the string
		{
			temp += *it;
		}
	}
	values.push_back(temp);
	if (values.size() != 2)		//if size of vector is not 2, its false
		return false;
	else if (!num_check(values[1]))	//if second word is not a number, its false
		return false;
	else if (stoi(values[1]) > 9 || stoi(values[1]) < 0)	//if the number is greater than 9 or less than 0, false
		return false;
	else return true;
}

PCB newProcess(string x, vector<string> values)	//function creates a PCB with memory and priority taken from vector
{
	PCB newP;
	newP.priority = stoi(values[1]);
	newP.memory = stoll(values[2]);

	return newP;
}

void addtoMem(PCB*& head, PCB newPCB, int& pNum, long long int tRam, PCB& CPU, vector<PCB>& a, vector<PCB>& b, vector<PCB>& c, vector<PCB>& d, vector<PCB>& e) //function adds process to memory and CPU/priority queue
{
	bool x = false;	//x is false as long as we dont find a place in memory
	long long int lastNum;
	if (head == NULL)	//If the head pointer points to NULL, our memory is completly empty so add the new process in the first slot
	{
		head = new PCB;
		head->priority = newPCB.priority;
		head->memory = newPCB.memory;
		head->PID = pNum;
		pNum++;
		head->start = 0;
		head->end = head->memory - 1;
		head->next = NULL;
		x = true;
		scheduling(*head, CPU, a, b, c, d, e);
	}
	else {
		PCB* tmpptr = head;
			while (tmpptr->next != NULL)	//go through the linked list of memory until we reach the end
			{
				if (tmpptr->PID == 0 && (tmpptr->end - tmpptr->start + 1) == newPCB.memory)	//if empty memory hole is same size as process we want to add
				{
					tmpptr->PID = pNum;
					pNum++;
					tmpptr->priority = newPCB.priority;
					x = true;

					scheduling(*tmpptr, CPU, a, b, c, d, e);
					break;
				}
				else if (tmpptr->PID == 0 && (tmpptr->end - tmpptr->start + 1) > newPCB.memory) //if empty memory hole has more memory than new process
				{
					//Add new process to the hole, and modify the new size of the hole:
					PCB* temp = tmpptr->next;	//new pointer points pcb after hole
					tmpptr->next = new PCB;	//After the hole, create new PCB that is going to be another hole
					tmpptr->PID = pNum;		//Update the PID number instead of 0
					pNum++;
					tmpptr->priority = newPCB.priority;
					long long int tempMem = tmpptr->memory;
					tmpptr->memory = newPCB.memory;
					tmpptr->end = newPCB.memory + tmpptr->start - 1;
					long long int tempStart = tmpptr->end + 1;

					scheduling(*tmpptr, CPU, a, b, c, d, e);

					tmpptr = tmpptr->next;
					tmpptr->PID = 0;
					tmpptr->start = tempStart;
					tmpptr->end = tempMem - newPCB.memory + tempStart - 1;
					tmpptr->memory = tempMem - tempStart + 1;
					tmpptr->next = temp;
					x = true;
					temp = NULL;
					delete temp;
					break;
				}
				//lastNum = tmpptr->end + 1;
				tmpptr = tmpptr->next;
			}
			lastNum = tmpptr->end + 1;
		if (tmpptr->next == NULL && newPCB.memory <= (tRam - lastNum)) //If we reach the end of memory and have space 
		{
			tmpptr->next = new PCB;
			tmpptr = tmpptr->next;
			tmpptr->priority = newPCB.priority;
			tmpptr->memory = newPCB.memory;
			tmpptr->PID = pNum;
			pNum++;
			tmpptr->start = lastNum;
			tmpptr->end = tmpptr->memory + lastNum - 1;
			tmpptr->next = NULL;
			
			scheduling(*tmpptr, CPU, a, b, c, d, e);
			tmpptr = NULL;
			delete tmpptr;
			x = true;
		}
		else {
			tmpptr = NULL;
			delete tmpptr;
		}
	}	
		if (x == false) cout << "Unable to find a spot in memory for this process" << endl;
		else cout << "Added Process to Computer" << endl;
	
}

void enterCPU(PCB& CPU, vector<PCB>& a, vector<PCB>& b, vector<PCB>& c, vector<PCB>& d, vector<PCB>& e)	//makes the highest priority process in the priority queues use the CPU when it's empty
{
	if (!e.empty())
	{
		CPU = e[0];
		e.erase(e.begin());
	}
	else if (!d.empty())
	{
		CPU = d[0];
		d.erase(d.begin());
	}
	else if (!c.empty())
	{
		CPU = c[0];
		c.erase(c.begin());
	}
	else if (!b.empty())
	{
		CPU = b[0];
		b.erase(b.begin());
	}
	else if (!a.empty())
	{
		CPU = a[0];
		a.erase(a.begin());
	}
}

void terminate(PCB*& memoryHead, PCB& CPU, long long int& tRam, vector<PCB>& a, vector<PCB>& b, vector<PCB>& c, vector<PCB>& d, vector<PCB>& e) //terminates process using CPU
{
	PCB* delPointer = memoryHead;
	if (CPU.PID == -1)
		cout << "Invalid input, CPU is empty" << endl;
	else {
		int j = 0;
		while (delPointer != NULL)		//While parsing linked list to find program in memory
		{
			if (delPointer->PID == CPU.PID)		//if we find program in memory: Go through if statements until we find the right condition
			{
				PCB* nextPoint = delPointer->next;	//create pointers previous and next for the surrounding programs
				PCB* prevPoint = memoryHead;
				for (int i = 1; i < j; i++)
				{
					prevPoint = prevPoint->next;
				}
				if (nextPoint == NULL)			//If next is NULL it means that our deleted program is last in memory
				{
					if (prevPoint->PID != 0)	//if previous is not a hole:
					{
						prevPoint->next = NULL;
						if (prevPoint->PID == delPointer->PID)	//if prev and delete are in the same spot(they are both first)
						{
							memoryHead = NULL;
							delPointer = NULL;
							nextPoint = NULL;
							prevPoint = NULL;
							delete delPointer;
							delete nextPoint;
							delete prevPoint;
							break;						//So delete the program by emptying the linked list and deleting all the pointers

						}
						else {						//else just delete the program since it's last so we dont need to worry about a hole
							prevPoint->next = NULL;
							delete delPointer;
							delete nextPoint;

							prevPoint = NULL;
							delete prevPoint;
							break;
						}
					}
					else if (j - 1 == 0)		//else if the process is last in memory but has a hole before it that is first in memory
					{
						memoryHead = NULL;
						delPointer = NULL;
						nextPoint = NULL;
						prevPoint = NULL;
						delete delPointer;
						delete nextPoint;
						delete prevPoint;
						break;
					}
					else {									//else previous is a hole so we need to remove the hole and the program
						PCB* nHolder = memoryHead;
						for (int k = 1; k < j - 1; k++)		//create pointer nHolder to mark the postion before prev hole
						{
							nHolder = nHolder->next;
						}
						nHolder->next = NULL;				//nHolder points to NULL since it's the last program in the list
						nHolder = NULL;						//now delete all the other pointers

						delete nHolder;
						delete prevPoint;
						delete delPointer;
						delete nextPoint;
						break;
					}

				}
				else if (prevPoint->PID != 0 && nextPoint->PID != 0)	//else if theres no hole between the program
				{
					delPointer->PID = 0;		//replace program with hole

					delPointer = NULL;
					prevPoint = NULL;
					nextPoint = NULL;
					delete delPointer;
					delete prevPoint;
					delete nextPoint;
					break;
				}
				else if (prevPoint->PID == 0 && nextPoint->PID != 0)	//else if theres a hole before the program BUT not after
				{
					prevPoint->memory = prevPoint->memory + delPointer->memory;	//make the previous hole bigger
					prevPoint->end = delPointer->end;
					prevPoint->next = nextPoint;
					delete delPointer;					//delete the program

					prevPoint = NULL;
					nextPoint = NULL;
					delete prevPoint;
					delete nextPoint;
					break;
				}
				else if (prevPoint->PID != 0 && nextPoint->PID == 0)		//else if theres a hole after BUT not before
				{
					delPointer->PID = 0;					//replace the deleted program with a hole
					delPointer->end = nextPoint->end;
					delPointer->memory = delPointer->memory + nextPoint->memory;
					delPointer->next = nextPoint->next;
					delete nextPoint;						//delete the original hole

					prevPoint = NULL;
					delPointer = NULL;
					delete prevPoint;
					break;
 				}
				else if (prevPoint->PID == 0 && nextPoint->PID == 0)	//else if theres a hole before AND after
				{
					PCB* pCheck = memoryHead;				//reallocate the hole to prev hole and delete the other 2
					for (int k = 1; k < j - 1; k++)
						pCheck = pCheck->next;

					prevPoint->end = nextPoint->end;
					prevPoint->memory = prevPoint->memory + nextPoint->memory + delPointer->memory;
					prevPoint->next = nextPoint->next;

					prevPoint = NULL;
					delete prevPoint;
					delete delPointer;
					delete nextPoint;
					break;
				}
			}
			j++;
			delPointer = delPointer->next;
		}
		CPU.PID = -1;
		tRam = tRam + CPU.memory;
		enterCPU(CPU, a, b, c, d, e);
		cout << "Process using CPU has been deleted" << endl;
	}
}

void nextDevice(vector<vector<PCB>>& mainQ, PCB devArr[9], int devNum)	//makes the next process in the device queue use the device when empty
{
	if (!mainQ[devNum].empty())
	{
		devArr[devNum] = mainQ[devNum][0];
		mainQ[devNum].erase(mainQ[devNum].begin());
	}
}

int main()
{
	PCB CPU;			//CPU is just a PCB struct
	CPU.PID = -1;		//PID == 1 symbolizes the CPU is empty
	vector<PCB> pr0;	//pr0 - pr4 are the priority queues
	vector<PCB> pr1;
	vector<PCB> pr2;
	vector<PCB> pr3;
	vector<PCB> pr4;

	string h;
	vector<string> test;
	vector<PCB> devQ;

	//Disk Drive and Printer I/O queues, and Device arrays
	vector<vector<PCB>> IOq;  PCB diskArr[10];
	vector<vector<PCB>> IOq2; PCB printArr[10];

	PCB* memoryHead = NULL;

	int pNum = 1;			//pNum represents PID, starts at 1
	long long int RAM = -1;
	int printer = -1;
	int disks = -1;
	
	askRam(RAM);
	askDevice(disks, "Disks");
	askDevice(printer, "Printers");
	
	long long int availRAM = RAM;
	
	cout << "The Computer is ready to execute programs." << endl;

	for (int f = 0; f < disks; f++)
	{
		IOq.push_back(devQ);
		diskArr[f].PID = -1;
	}
	for (int q = 0; q < printer; q++)
	{
		IOq2.push_back(devQ);
		printArr[q].PID = -1;
	}

	//While Loop asks for user cin input and runs the Operating System
	while (getline(cin, h))
	{
		if (h[0] == 'A')		//If A input is valid, add program to memory and priority queues
		{
			if (aValid(h, test))
			{
				if (newProcess(h, test).memory > availRAM)
					cout << "Not enough memory for this process" << endl;
				else {
					addtoMem(memoryHead, newProcess(h, test), pNum, RAM, CPU, pr0, pr1, pr2, pr3, pr4);
				}
			}
			else  cout << "Not a valid input" << endl;
		}
		else if (h == "t")		//if t, terminate the program using CPU
		{
			terminate(memoryHead, CPU, availRAM, pr0, pr1, pr2, pr3, pr4);

		}
		else if (h[0] == 'd')
		{
			if (dValid(h, test))
			{
				int dnum = stoi(test[1]);
				if (disks == dnum)
					cout << "Not a valid Disk Drive number, please retry" << endl;
				else if (dnum > disks || dnum < 0)
					cout << "Not a valid Disk Drive number, please retry" << endl;
				else if (CPU.PID == -1)
					cout << "No process is using the CPU right now" << endl;
				else if (diskArr[dnum].PID == -1)		//if the array# is empty, add the device to the array
				{
					diskArr[dnum] = CPU;
					CPU.PID = -1;
					enterCPU(CPU, pr0, pr1, pr2, pr3, pr4);
					cout << "Process added to disk " << dnum << endl;
				}
				else { 
					IOq[dnum].push_back(CPU);
					CPU.PID = -1;
					enterCPU(CPU, pr0, pr1, pr2, pr3, pr4);
					cout << "Process added to device queue" << endl;
				}
			}
			else cout << "Not a valid Input" << endl;
		}
		else if (h[0] == 'p')
		{
			if (dValid(h, test))
			{
				int pnum = stoi(test[1]);
				if (printer == pnum)
					cout << "Not a valid Printer number, please retry" << endl;
				else if (pnum > printer || pnum < 0)
					cout << "Not a valid Printer number, please retry" << endl;
				else if (CPU.PID == -1)
					cout << "No process is using the CPU right now" << endl;
				else if (printArr[pnum].PID == -1)		//if the array# is empty, add the device to the array
				{
					printArr[pnum] = CPU;
					CPU.PID = -1;
					enterCPU(CPU, pr0, pr1, pr2, pr3, pr4);
					cout << "Process added to printer " << pnum << endl;
				}
				else {
					IOq2[pnum].push_back(CPU);
					CPU.PID = -1;
					enterCPU(CPU, pr0, pr1, pr2, pr3, pr4);
					cout << "Process added to device queue" << endl;
				}
			}
			else cout << "Not a valid Input" << endl;
		}
		else if (h[0] == 'D')	//Process is done using I/O device and enters the CPU
		{
			if (dValid(h, test))
			{
				int Dnum = stoi(test[1]);
				if (Dnum >= disks || Dnum < 0)
					cout << "Disk " << Dnum << " does not exist" << endl;
				else if (diskArr[Dnum].PID == -1)
					cout << "No process is using that Hard Disk" << endl;
				else
				{
					scheduling(diskArr[Dnum], CPU, pr0, pr1, pr2, pr3, pr4);
					diskArr[Dnum].PID = -1;
					nextDevice(IOq, diskArr, Dnum);
					cout << "Process has been removed from I/O device" << endl;
				}
			}
			else cout << "Invalid Input, please try again" << endl;
		}
		else if (h[0] == 'P')
		{
			if (dValid(h, test))
			{
				int Pnum = stoi(test[1]);
				if (Pnum >= printer || Pnum < 0)
					cout << "Printer " << Pnum << " does not exist" << endl;
				else if (printArr[Pnum].PID == -1)
					cout << "No process is using that Printer" << endl;
				else
				{
					scheduling(printArr[Pnum], CPU, pr0, pr1, pr2, pr3, pr4);
					printArr[Pnum].PID = -1;
					nextDevice(IOq2, printArr, Pnum);
					cout << "Process has been removed from I/O device" << endl;
				}
			}
			else cout << "Invalid Input, please try again" << endl;
		}
		else if (h == "S r")		//The S inputs prints to the screen information about our processes
		{
			if (CPU.PID == -1)
				cout << "No process is currently using the CPU" << endl;
			else {
				cout << "Process " << CPU.PID << " is using the CPU with Priority: " << CPU.priority << " and Memory: " << CPU.memory << endl << endl;
				cout << "Inside ready queue Level 4: " << endl;
				for (int h = 0; h < pr4.size(); h++)
				{
					cout << "Process " <<pr4[h].PID << " with memory " << pr4[h].memory << endl;
				}
				cout << endl << "Inside ready queue Level 3: " << endl;
				for (int h = 0; h < pr3.size(); h++)
				{
					cout << "Process " << pr3[h].PID << " with memory " << pr3[h].memory << endl;
				}
				cout << endl << "Inside ready queue Level 2: " << endl;
				for (int h = 0; h < pr2.size(); h++)
				{
					cout << "Process " << pr2[h].PID << " with memory " << pr2[h].memory << endl;
				}
				cout << endl << "Inside ready queue Level 1: " << endl;
				for (int h = 0; h < pr1.size(); h++)
				{
					cout << "Process " << pr1[h].PID << " with memory " << pr1[h].memory << endl;
				}
				cout << endl << "Inside ready queue Level 0: " << endl;
				for (int h = 0; h < pr0.size(); h++)
				{
					cout << "Process " << pr0[h].PID << " with memory " << pr0[h].memory << endl;
				}
			}
		}
		else if (h == "S i")
		{
			for (int u = 0; u < disks; u++)
			{
				if (diskArr[u].PID != -1)
					cout << "For disk " << u << ": "  << "Process " << diskArr[u].PID << " is using it" << endl;
				if (!IOq[u].empty())
				{
					cout << "Processes waiting to use disk " << u << ":";
					for (int v = 0; v < IOq[u].size(); v++)
						cout << " " << IOq[u][v].PID << ",";
					cout << endl;
				}
			}
			for (int u = 0; u < printer; u++)
			{
				if (printArr[u].PID != -1)
					cout << "For printer " << u << ": " << "Process " << printArr[u].PID << " is using it" << endl;
				if (!IOq2[u].empty())
				{
					cout << "Processes waiting to use printer " << u << ":";
					for (int v = 0; v < IOq2[u].size(); v++)
						cout <<" "<< IOq2[u][v].PID << ",";
					cout << endl;
				}
			}
		}
		else if (h == "S m")
		{
			if (memoryHead == NULL)
				cout << "Memory is empty" << endl;
			else {
				PCB* readptr = memoryHead;
				int totalHole = 1;
				while (readptr != NULL)
				{
					if (readptr->PID == 0)
						totalHole++;
					if (readptr->PID != 0)
						cout << "Process " << readptr->PID << " of size " << readptr->memory << " is occupying range " << readptr->start << "-" << readptr->end << endl;
					readptr = readptr->next;
				}
				/*readptr = memoryHead;
				cout << endl << "There are " << totalHole << " holes at: " << endl;
				while (readptr->next != NULL)
				{
					if (readptr->PID == 0)
						cout << readptr->start << "-" << readptr->end << endl;
					readptr = readptr->next;
				}
				if (availRAM != 0)
				cout << readptr->end + 1 << "-" << RAM << endl;*/
				readptr = NULL;
				delete readptr;
			}
		}

	}

}