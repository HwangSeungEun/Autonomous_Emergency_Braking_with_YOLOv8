import time
import random
import numpy as np
from multiprocessing import shared_memory

def send_data(shm_name):
    shm = shared_memory.SharedMemory(name = shm_name)
    shared_data = np.ndarray((8,), dtype = 'int64', buffer = shm.buf)

    while True:
        for i in range(8):
            shared_data[i] = 1
        time.sleep(0.01)

if __name__ == "__main__":
    
    shared_memory_name = "yoloSM"
    shm_size = 8 * 8 
    shm = shared_memory.SharedMemory(name=shared_memory_name, create=True, size=shm_size)
    print("YOLO : SM sending start...!")

    try:
        send_data(shared_memory_name)

    finally:
        shm.close()
        print("YOLO : SM sending finish...!")
        shared_memory.SharedMemory(name = shared_memory_name).unlink()