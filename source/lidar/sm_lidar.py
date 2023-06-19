import time
import numpy as np
from multiprocessing import shared_memory

def receive_data(shm_name):
    print("recv data...")
    shm = shared_memory.SharedMemory(name=shm_name)
    shared_data = np.ndarray((12,), dtype='int', buffer=shm.buf)

    print("Shared Data:", shared_data[:])

if __name__ == "__main__" :
    shared_memory_name = "HADA3_CAM"

    time_start = time.time()
    time_final = 2000
    time_ts = 0.02
    time_cnt = 0
    time_stime = 0
    
    while (time_stime < time_final) :
            
        # Try to access the shared memory object in a loop
        while True:
            try:
                receive_data(shared_memory_name)
                break
            except FileNotFoundError:
                time.sleep(0.5)  # Add a small delay before trying againq

        while(1) :
            
            time_curr = time.time()
            time_del = time_curr - time_start - time_stime
            
            if (time_del > time_ts):
                
                time_cnt += 1
                time_stime = time_cnt * time_ts
                
                break

    # Unlink the shared memory object
    shm = shared_memory.SharedMemory(name=shared_memory_name)
    shm.close()
