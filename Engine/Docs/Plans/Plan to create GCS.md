**Plan for creating a GC system:**

a) Object dependencies and cleanup

1. Analyze object dependencies for proper tagging of living and dead entities. Also, bind components to entities.
2. Create a method to destroy and free such objects using the Sweep() method.
3. Create a function that triggers the entire cycle of element processing and their cleanup, e.g., Collect Garbage.
4. Create a method for registering an object for tracking and deregistering: RegisterObject()/UnregisterObject().

b) GC execution

The above describes how to create methods for object cleanup, now regarding the actual GC execution. First, we need to understand whether these processes need to be triggered at all, as they consume a lot of processor resources, and it is better to run them only in certain cases (so to speak).

1. We need to determine how full the computer's RAM is. Based on this, we will decide whether to start the GC or not. The method is getAvailableMemory().
2. The next method, shouldRunGC, will be called at higher levels (possibly at every frame) to understand whether it is necessary to trigger cleanup or not. One way: it will check available memory and compare it with a threshold. If the threshold is exceeded, cleanup occurs. Another method, which is probably more efficient, is to clean up resources consistently so that they do not accumulate. The less memory there is, the more often the cleanup method will be triggered.

c) Next, there are three methods for optimizing the cleanup process:

1. **optimizeMemoryUsage()** — A method for the second way, it will have several tasks: first, define a Threshold — the minimum number of bytes to trigger cleanup; second, be responsible for optimizing memory usage.
2. **monitorGCEfficiency()** — Responsible for monitoring the GC's efficiency, its speed, execution of cleanup, recording, and analysis of this data.
3. **collectGarbageDuringIdleTime()** — As the name suggests, it performs cleanup while the window is minimized.
