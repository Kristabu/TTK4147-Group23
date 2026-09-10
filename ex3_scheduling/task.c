enum taskState
{
    idle,
    running,
    preemptive,
    finished
};

struct Task
{
    enum taskState state;
    int ID;
    int arrivalTime;
    int startTime;      // Some time unit
    int totalRuntime;   // Some time unit
    int currentRuntime; // Some time unit
};