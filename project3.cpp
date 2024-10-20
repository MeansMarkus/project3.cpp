#include <iostream>
using namespace std;

class CPUJob {
public:
    int job_id; // Unique identifier for the job
    int priority; // Priority level of the job (1-10)
    int job_type; // Job type (1-10)
    int cpu_time_consumed; // Total CPU time consumed by the job
    int memory_consumed; // Total memory consumed thus far

    CPUJob(int id, int prio, int type, int cpu, int mem)
        : job_id(id), priority(prio), job_type(type), cpu_time_consumed(cpu), memory_consumed(mem) {}

    void printJob() const {
        cout << "Job ID: " << job_id << ", Priority: " << priority
             << ", Job Type: " << job_type << ", CPU Time: " << cpu_time_consumed
             << ", Memory Consumed: " << memory_consumed << endl;
    }
};

template <class DT>
class Queue {
public:
    DT JobPointer; // Direct pointer to a job (e.g., CPUJob*)
    Queue<DT>* next; // Pointer to the next node in the queue

    Queue(DT job) : JobPointer(job), next(nullptr) {}
};

template <class DT>
class NovelQueue {
public:
    Queue<DT>* front; // Pointer to the front of the queue
    Queue<DT>* rear;  // Pointer to the rear of the queue
    int size; // Number of elements in the queue

    NovelQueue() : front(nullptr), rear(nullptr), size(0) {}

    void enqueue(DT job) {
        Queue<DT>* newNode = new Queue<DT>(job);
        if (rear == nullptr) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        size++;
        cout << "Enqueued Job:" << endl;
        job->printJob();
        cout << "Jobs after enqueue:" << endl;
        display();
    }

    DT dequeue() {
        if (front == nullptr) return nullptr;
        Queue<DT>* temp = front;
        front = front->next;
        if (front == nullptr) rear = nullptr;
        DT job = temp->JobPointer;
        delete temp;
        size--;
        return job;
    }

    void modify(int job_id, int new_priority, int new_job_type, int new_cpu_time, int new_memory) {
        Queue<DT>* temp = front;
        while (temp != nullptr) {
            if (temp->JobPointer->job_id == job_id) {
                temp->JobPointer->priority = new_priority;
                temp->JobPointer->job_type = new_job_type;
                temp->JobPointer->cpu_time_consumed = new_cpu_time;
                temp->JobPointer->memory_consumed = new_memory;
                cout << "Modified Job ID " << job_id << ":" << endl;
                temp->JobPointer->printJob();
                cout << "Jobs after modification:" << endl;
                display();
                return;
            }
            temp = temp->next;
        }
    }

    void change(int job_id, int field_index, int new_value) {
        Queue<DT>* temp = front;
        while (temp != nullptr) {
            if (temp->JobPointer->job_id == job_id) {
                switch (field_index) {
                    case 1: temp->JobPointer->priority = new_value; break;
                    case 2: temp->JobPointer->job_type = new_value; break;
                    case 3: temp->JobPointer->cpu_time_consumed = new_value; break;
                    case 4: temp->JobPointer->memory_consumed = new_value; break;
                }
                cout << "Changed Job ID " << job_id << " field " << field_index << " to " << new_value << ":" << endl;
                temp->JobPointer->printJob();
                cout << "Jobs after changing field:" << endl;
                display();
                return;
            }
            temp = temp->next;
        }
    }

    void promote(int job_id, int positions) {
        Queue<DT>* temp = front;
        while (temp != nullptr) {
            if (temp->JobPointer->job_id == job_id) {
                temp->JobPointer->priority += positions;
                cout << "Promoted Job ID " << job_id << " by " << positions << " Position(s):" << endl;
                temp->JobPointer->printJob();
                cout << "Jobs after promotion:" << endl;
                display();
                                return;
            }
            temp = temp->next;
        }
    }

    NovelQueue<DT>* reorder(int attr) {
        NovelQueue<DT>* reorderedQueue = new NovelQueue<DT>();
        Queue<DT>* temp = front;

        // Copy jobs into an array for sorting
        DT jobArray[size];
        int i = 0;
        while (temp != nullptr) {
            jobArray[i++] = temp->JobPointer;
            temp = temp->next;
        }

        // Sort the jobArray based on the attribute (1=priority, 2=job_type)
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                bool swap = false;
                if (attr == 1 && jobArray[j]->priority < jobArray[j + 1]->priority)
                    swap = true;
                else if (attr == 2 && jobArray[j]->job_type > jobArray[j + 1]->job_type)
                    swap = true;

                if (swap) {
                    DT tempJob = jobArray[j];
                    jobArray[j] = jobArray[j + 1];
                    jobArray[j + 1] = tempJob;
                }
            }
        }

        // Enqueue the sorted jobs into the reordered queue
        for (int i = 0; i < size; i++) {
            reorderedQueue->enqueue(jobArray[i]);
        }

        return reorderedQueue;
    }

    void display() const {
        Queue<DT>* temp = front;
        while (temp != nullptr) {
            temp->JobPointer->printJob();
            temp = temp->next;
        }
    }

    int count() const {
        return size;
    }

    void listJobs() const {
        display();
    }
};

int main() {
    int n; // Number of commands
    cin >> n; // Read the number of commands

    // Instantiate a NovelQueue for CPUJob pointers
    NovelQueue<CPUJob*>* myNovelQueue = new NovelQueue<CPUJob*>();

    char command; // Variable to store the command type
    // Variables for job attributes
    int job_id, priority, job_type, cpu_time_consumed, memory_consumed;
    // Variables for modifying a job
    int new_priority, new_job_type, new_cpu_time_consumed;
    int new_memory_consumed;
    int field_index, new_value;
    // Variable for the number of positions in the 'Promote' command
    int positions;
    int attribute_index; // Variable for the 'Reorder' command

    /************** Read each command Process ***************/
    for (int i = 0; i < n; ++i) {
        cin >> command; // Read the command type
        switch (command) {
            case 'A': { // Add (Enqueue)
                cin >> job_id >> priority >> job_type;
                cin >> cpu_time_consumed >> memory_consumed;
                CPUJob* newJob = new CPUJob(job_id, priority, job_type, cpu_time_consumed, memory_consumed);
                myNovelQueue->enqueue(newJob);
                break;
            }
            case 'R': { // Remove (Dequeue)
                CPUJob* removedJob = myNovelQueue->dequeue();
                if (removedJob) {
                    cout << "Dequeued Job: ";
                    removedJob->printJob();
                    delete removedJob; // Clean up memory after use
                }
                break;
            }
            case 'M': { // Modify
                cin >> job_id >> new_priority >> new_job_type;
                cin >> new_cpu_time_consumed >> new_memory_consumed;
                myNovelQueue->modify(job_id, new_priority, new_job_type, new_cpu_time_consumed, new_memory_consumed);
                break;
            }
            case 'C': { // Change Job Values
                cin >> job_id >> field_index >> new_value;
                myNovelQueue->change(job_id, field_index, new_value);
                break;
            }
            case 'P': { // Promote
                cin >> job_id >> positions;
                myNovelQueue->promote(job_id, positions);
                break;
            }
            case 'O': { // Reorder
                cin >> attribute_index;
                NovelQueue<CPUJob*>* reorderedQueue = myNovelQueue->reorder(attribute_index);
                cout << "Reordered Queue by attribute " << attribute_index << ":" << endl;
                reorderedQueue->display();
                delete reorderedQueue;
                break;
            }
            case 'D': { // Display
                cout << "Displaying all jobs in the queue:" << endl;
                myNovelQueue->display();
                                break;
            }
            case 'N': { // Count
                cout << "Number of elements in the queue: " << myNovelQueue->count() << endl;
                break;
            }
            case 'L': { // List Jobs
                cout << "List of jobs sorted by job IDs:" << endl;
                myNovelQueue->listJobs();
                break;
            }
            default:
                cout << "Invalid command!" << endl;
        }
    }

    delete myNovelQueue; // Clean up the NovelQueue after all operations
    return 0;
}