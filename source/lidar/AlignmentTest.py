from module import *

global lidarDist, azim, yoloBuffer
lidarDist = []
azim = np.linspace(-6.5 ,184.5 ,761)


class READ_DATA(ct.Structure):
    
    _fields_ = [("xCoordinate",ct.c_double*761),("yCoordinate",ct.c_double*761),("dist",ct.c_double*761),("angle",ct.c_double*761)]
         
         
class Lidar_SharedMem :
    
    def __init__(self):
        
        self.is_lidarSM = False
        self.is_yoloSM  = False

    def Lidar_SMopen(self) :
        
        self.FILE_MAP_ALL_ACCESS  = 0x000F001F
        self.FILE_MAP_READ        = 0x0004
        self.INVALID_HANDLE_VALUE = -1
        self.SHMEMSIZE            = 0x100
        self.PAGE_READWRITE       = 0x04
        self.TRUE  = 1
        self.FALSE = 0

        self.kernel32_dll               = ct.windll.kernel32
        self.msvcrt_dll                 = ct.cdll.msvcrt  # To be avoided

        self.CreateFileMapping          = self.kernel32_dll.CreateFileMappingW
        self.CreateFileMapping.argtypes = (wt.HANDLE, wt.LPVOID, wt.DWORD, wt.DWORD, wt.DWORD, wt.LPCWSTR)
        self.CreateFileMapping.restype  = wt.HANDLE

        self.OpenFileMapping            = self.kernel32_dll.OpenFileMappingW
        self.OpenFileMapping.argtypes   = (wt.DWORD, wt.BOOL, wt.LPCWSTR)
        self.OpenFileMapping.restype    = wt.HANDLE

        self.MapViewOfFile              = self.kernel32_dll.MapViewOfFile
        self.MapViewOfFile.argtypes     = (wt.HANDLE, wt.DWORD, wt.DWORD, wt.DWORD, ct.c_ulonglong)
        self.MapViewOfFile.restype      = wt.LPVOID

        self.memcpy                     = self.msvcrt_dll.memcpy
        self.memcpy.argtypes            = (ct.c_void_p, ct.c_void_p, ct.c_size_t)
        self.memcpy.restype             = wt.LPVOID

        self.UnmapViewOfFile            = self.kernel32_dll.UnmapViewOfFile
        self.UnmapViewOfFile.argtypes   = (wt.LPCVOID,)
        self.UnmapViewOfFile.restype    = wt.BOOL

        self.CloseHandle                = self.kernel32_dll.CloseHandle
        self.CloseHandle.argtypes       = (wt.HANDLE,)
        self.CloseHandle.restype        = wt.BOOL

        self.GetLastError               = self.kernel32_dll.GetLastError
        
        # 파일 이름 선언

        self.rfile_mapping_name_ptr = ct.c_wchar_p("Lidar_smdat_ReadData")
        # self.rfile_mapping_name_ptr = ct.c_wchar_p("Hils_smdat_ReadData")

        self.rbyte_len = ct.sizeof(READ_DATA)    



        # r파일 맵핑 및 맵핑 객체 선언
        self.rmapping_handle = self.OpenFileMapping(self.FILE_MAP_ALL_ACCESS, False, self.rfile_mapping_name_ptr)
        if not self.rmapping_handle:
            print("Could not open file mapping object: {:d}".format(self.GetLastError()))
            raise ct.WinError()

        self.rmapped_view_ptr = self.MapViewOfFile(self.rmapping_handle, self.FILE_MAP_ALL_ACCESS, 0, 0, self.rbyte_len)
        if not self.rmapped_view_ptr:
            print("Could not map view of file: {:d}".format(self.GetLastError()))
            self.CloseHandle(self.rmapping_handle)
            raise ct.WinError()
        
        self.is_lidarSM = True
        
        print("Shared memory with lidar Interface program opened ...!")


    def Yolo_SMopen(self) :
        
        self.is_yoloSM  = True
        print("Shared memory with YOLO program opened ...!")
        
        
    def receiveData(self):
        
        global lidarDist, yoloBuffer

        if self.is_lidarSM == True:
            
            read_smdat = READ_DATA()
            rmsg_ptr   = ct.pointer(read_smdat)
            self.memcpy(rmsg_ptr,self.rmapped_view_ptr,self.rbyte_len)
            lidarDist   = read_smdat.dist
     
    def sharedmemory_close(self):
        self.UnmapViewOfFile(self.wmapped_view_ptr)
        self.CloseHandle(self.wmapping_handle)
        self.UnmapViewOfFile(self.rmapped_view_ptr)
        self.CloseHandle(self.rmapping_handle)


class PROJECTION :

    def __init__(self) :
        
        self.colorYellow = (25, 255, 255)
        self.colorRed    = (0, 0, 255)
        self.colorBlue   = (255, 0, 0)
        self.colorGreen  = (0, 255, 0)
        
        self.D2R         = pi/180
        self.camHeight   = 0.93
        self.camRecede   = 0.77
        
        self.Alphadeg    = 109.5
        self.Alpha       = self.Alphadeg * self.D2R
        self.Beta        = 0 * self.D2R
        self.Gamma       = 0 * self.D2R
        
        self.realHeight  = sqrt(self.camHeight**2 + self.camRecede**2) * sin(atan(self.camHeight/self.camRecede) - (self.Alphadeg - 90) * self.D2R)
        self.realRecede  = sqrt(self.camHeight**2 + self.camRecede**2) * cos(atan(self.camHeight/self.camRecede) - (self.Alphadeg - 90) * self.D2R)
        
        self.focalLen    = 0.00367
        self.imgWidth    = 640
        self.imgHeight   = 480
        self.fovX        = 60.92    * pi/180
        self.fovY        = 53.1432  * pi/180
        self.ox          = self.imgWidth/2                                              
        self.oy          = self.imgHeight/2
        self.sx          = self.focalLen * math.tan(0.5 * self.fovX)/(0.5 * self.imgWidth)  
        self.sy          = self.focalLen * math.tan(0.5 * self.fovY)/(0.5 * self.imgHeight)
        
        self.projectionX = []
        self.projectionY = []
        self.lidarX      = []
        self.lidarY      = []
        self.candidates  = []
        self.avgDist     = 0
        self.distance    = 0
        self.distances   = []
        self.distVal     = 0
        self.maxDiff     = 20
        
        self.rotX = np.array([[1 ,          0             ,              0        ], 
                              [0 ,   np.cos(self.Alpha)   ,   -np.sin(self.Alpha) ], 
                              [0 ,   np.sin(self.Alpha)   ,    np.cos(self.Alpha) ]])   

        self.rotY = np.array([[np.cos(self.Beta)  ,   0  ,    np.sin(self.Beta) ], 
                              [    0              ,   1  ,        0             ], 
                              [-np.sin(self.Beta) ,   0  ,    np.cos(self.Beta) ]])

        self.rotZ = np.array([[np.cos(self.Gamma)    ,   -np.sin(self.Gamma) ,    0 ], 
                              [np.sin(self.Gamma)    ,   np.cos(self.Gamma)  ,    0 ], 
                              [    0                 ,        0              ,    1 ]])    
        
        
        self.rotMat   = self.rotZ @ self.rotY @ self.rotX
        
        
        self.transMat = np.array([[      0      ],
                                  [self.realHeight],
                                  [self.realRecede]]) 
        
        
        self.M_ext = np.hstack((self.rotMat, self.transMat))


        self.M_int = np.array([[self.focalLen/self.sx , 0                       , self.ox ],            
                               [0                     , self.focalLen/self.sy   , self.oy ],
                               [0                     , 0                       , 1       ]])  
        
        
        self.projectionMat = self.M_int @ self.M_ext 
        

        
        
    def polar2xy(self, dist, azi) : 

        n = len(azi)
        x = np.zeros(n)
        y = np.zeros(n)

        for i in range(n) :

            x[i] = dist[i] * cos(azi[i] * self.D2R)
            y[i] = dist[i] * sin(azi[i] * self.D2R)

        return x, y



    def lidarCamProjection(self, frame) :                                 

        self.projectionX = []
        self.projectionY = []

        for i, dis in enumerate(lidarDist):

            lidarDist[i] = lidarDist[i] / 500

        self.lidarX , self.lidarY = self.polar2xy(lidarDist,azim)                      

        for i in range(len(azim)):

            XY_im  = 0
            XY_cam = 0

            lx = self.lidarX[i]
            ly = self.lidarY[i]
            lz = 0
            Cz = ly + self.realRecede

            XY_cam = np.array([[lx],[ly],[lz],[1]])
            XY_im = 1/Cz * self.projectionMat @ XY_cam

            xx = int(XY_im[0])
            yy = int(XY_im[1])

            self.projectionX.append(xx)
            self.projectionY.append(yy)

            cv.circle(frame, (round(xx) ,round(yy)), 3, self.colorYellow)
            
        

if __name__ == "__main__" :
    
    sim     = Lidar_SharedMem()
    project = PROJECTION()
    
    sim.Lidar_SMopen()
    sim.Yolo_SMopen()

    time_start = time.time()
    
    webcam = cv.VideoCapture(cv.CAP_DSHOW +  1)
    
    if not webcam.isOpened():
        
        print("Could not open webcam")
        exit()

    while (time_stime < time_final and webcam.isOpened()):
        
        status, frame = webcam.read()
        
        webcam.set(cv.CAP_PROP_FOURCC, cv.VideoWriter_fourcc('M','J','P','G'))

        sim.receiveData()
        
        if status:
            
            project.lidarCamProjection(frame)
           
            cv.imshow("test",frame)
                
        if cv.waitKey(1) & 0xFF == ord('q'):
            break
          
        while(1):
            
            time_curr = time.time()
            time_del = time_curr - time_start - time_stime
            
            if (time_del > time_ts) :
                
                time_cnt += 1
                time_stime = time_cnt*time_ts
                
                break
            

    sim.sharedmemory_close()
