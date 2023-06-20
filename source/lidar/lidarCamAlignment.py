# -------------------------------------------------------------------
#
#   @   HADA code  :   Track guidance
#   @   Update     :   2023.6.03
#   @   Purpose    :   Lidar Cam calibration
#
# -------------------------------------------------------------------

from module import *
from serial_node import *


global lidarDist, azim
lidarDist  = []
azim = np.linspace(-5,185,761)


class READ_DATA(ct.Structure):
    
    _fields_ = [("xCoordinate",ct.c_double*761),("yCoordinate",ct.c_double*761),("dist",ct.c_double*761),("angle",ct.c_double*761)]
         
         
class Lidar_SharedMem :
    
    def __init__(self):
        
        self.is_lidarSM = False

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
        
    def receiveDist(self):
        
        global lidarDist

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
        
        self.colorYellow    = (25, 255, 255)
        self.colorWhite     = (255, 255, 255)
        self.colorRed       = (0, 0, 255)
        self.colorBlue      = (255, 0, 0)
        self.colorGreen     = (0, 255, 0)

        self.D2R            = pi/180
        self.R2D            = 180/pi
        
        self.Alphadeg       = 109.5
        self.Alpha          = self.Alphadeg * self.D2R
        self.Beta           = 0 * self.D2R
        self.Gamma          = 0 * self.D2R
        
        self.camHeight      = 0.93
        self.camRecede      = 0.77
        self.focalLen       = 0.00367
        self.imgWidth       = 640
        self.imgHeight      = 480
        self.fovX           = 60.92 * pi/180
        self.fovY           = 53.1432  * pi/180
        self.ox             = self.imgWidth/2                                              
        self.oy             = self.imgHeight/2
        self.sx             = self.focalLen * math.tan(0.5 * self.fovX)/(0.5 * self.imgWidth);      
        self.sy             = self.focalLen * math.tan(0.5 * self.fovY)/(0.5 * self.imgHeight);   
        
        self.realHeight     = sqrt(self.camHeight**2 + self.camRecede**2) * sin(atan(self.camHeight/self.camRecede) - (self.Alphadeg - 90) * self.D2R)
        self.realRecede     = sqrt(self.camHeight**2 + self.camRecede**2) * cos(atan(self.camHeight/self.camRecede) - (self.Alphadeg - 90) * self.D2R)
        
        self.projectionX    = []
        self.projectionY    = []
        self.lidarX         = []
        self.lidarY         = []
        
        self.lidarxList     = []
        self.lidaryList     = []
        
        self.rotX = np.array([[1 ,          0             ,              0        ], 
                              [0 ,   np.cos(self.Alpha)   ,   -np.sin(self.Alpha) ], 
                              [0 ,   np.sin(self.Alpha)   ,    np.cos(self.Alpha) ]])   

        self.rotY = np.array([[np.cos(self.Beta)  ,   0  ,    np.sin(self.Beta) ], 
                              [    0              ,   1  ,        0             ], 
                              [-np.sin(self.Beta) ,   0  ,    np.cos(self.Beta) ]])

        self.rotZ = np.array([[np.cos(self.Gamma)    ,   -np.sin(self.Gamma) ,   0 ], 
                              [np.sin(self.Gamma)    ,   np.cos(self.Gamma)  ,   0 ], 
                              [    0                 ,        0              ,   1 ]])    
        
        
        self.rotMat   = self.rotZ @ self.rotY @ self.rotX
        
        
        self.transMat = np.array([[      0       ],
                                  [self.realHeight],
                                  [self.realRecede]]) 
        
        self.M_ext = np.hstack((self.rotMat, self.transMat))


        self.M_int = np.array([[self.focalLen/self.sx , 0                       , self.ox ],            
                               [0                     , self.focalLen/self.sy   , self.oy ],
                               [0                     , 0                       , 1       ]])  
        
        self.projectionMat = self.M_int @ self.M_ext 
        
        self.candidates     = []
        self.rubberDist     = 0
        self.distance       = 0
        self.yoloBuffer     = []
        self.zeroCnt        = 0
        self.distVal        = 0
        self.maxDiff        = 25
        self.isyoloReady    = False
        self.steerCmd       = 0
        
        self.nObject        = 0
        self.azimuth        = 0
        self.objectDist     = 0
        self.isWarn         = False
        
        self.markerSize     = 12
        self.userFont       = cv.FONT_HERSHEY_COMPLEX
        
        self.Ux             = 0
        self.Uy             = 0 
        self.prevsteerCmd   = 0
        self.startFlag      = 0
        
        self.portNum        = 'COM3'
        self.velocity       = 0     # [km/h]
        
        
    def polar2xy(self, dist, azi) : 

        n = len(azi)
        x = np.zeros(n)
        y = np.zeros(n)

        for i in range(n) :

            x[i] = dist[i] * cos(azi[i] * self.D2R)
            y[i] = dist[i] * sin(azi[i] * self.D2R)

        return x, y

        

    def recieveYolo(self, frame) :
        
        shm = shared_memory.SharedMemory(name = "HADA3_CAM")
        self.yoloBuffer = np.ndarray((12,), dtype='int', buffer = shm.buf)
        
        self.nObject = 0
        
        for i in range(0, len(self.yoloBuffer), 2) :
            
            if(self.yoloBuffer[i] < 50) : self.yoloBuffer[i] = self.yoloBuffer[i] - 25
            
            if(self.yoloBuffer[i] > 590) : self.yoloBuffer[i] = self.yoloBuffer[i] + 25
            
            cv.circle(frame, (self.yoloBuffer[i], self.yoloBuffer[i+1]), 5, self.colorYellow, -1)    
        
        """
            - YOLO 객체 인식이 먼저 켜지면 오류가 생기는데, 방지하기 위해 startFlag를 사용
        """        

        if self.yoloBuffer[0] and self.yoloBuffer[1] == 0 :
            
            cv.putText(frame, "NO OBJECT DETECTED", (10,310), cv.FONT_HERSHEY_COMPLEX, 0.7, self.colorGreen, 2)
            cv.putText(frame, "EMERGENCY STOP : OFF", (10, 410), cv.FONT_HERSHEY_COMPLEX, 0.7, self.colorWhite, 2)
            cv.putText(frame, "SAFE !", (10, 460), cv.FONT_HERSHEY_COMPLEX, 0.7, self.colorGreen, 2)

        else : 
            
            self.isyoloReady = True
            
            if self.startFlag > 0 :
                
                self.lidarxList, self.lidaryList = [], []
                
                for Idx in range(0, len(self.yoloBuffer), 2) :
                
                    objectcenX, objectcenY = self.yoloBuffer[Idx], self.yoloBuffer[Idx+1]

                    self.candidates = []

                    if objectcenX == 0 or objectcenY == 0 :
                        continue

                    for i in range(761):

                        if abs(self.projectionX[i] - objectcenX) < self.maxDiff and self.projectionY[i] > objectcenY - 30:

                            self.candidates.append(i)
                            
                            if len(self.candidates) == 4   :  break

                    
                    if self.lidarY[i] > 0 :
                        
                        self.lidarxList.append(self.lidarX[i])
                        self.lidaryList.append(self.lidarY[i])
                        
                        self.nObject += 1
                        
                        self.objectDist = round(sqrt(self.lidarX[i]**2 + self.lidarY[i]**2), 2)
                        self.azimuth    = round(90 - atan2(self.lidarY[i], self.lidarX[i]) * self.R2D, 2)
                
                        cv.putText(frame, f"DIST : {self.objectDist}",(objectcenX - 50, objectcenY + 50), cv.FONT_HERSHEY_COMPLEX, 0.7, self.colorYellow, 2)
                        # Warning flag
                        self.isWarn = True if self.objectDist < 2.0 else False
                      
                        
            if (self.isWarn) :
                
                cv.putText(frame, "EMERGENCY STOP : ON", (10, 410), cv.FONT_HERSHEY_COMPLEX, 0.7, self.colorRed, 2)
                cv.putText(frame, "WARNING !", (10, 460), cv.FONT_HERSHEY_COMPLEX, 0.7, self.colorRed, 2)

            
            else :
                
                cv.putText(frame, "EMERGENCY STOP : OFF", (10, 410), cv.FONT_HERSHEY_COMPLEX, 0.7, self.colorWhite, 2)
                cv.putText(frame, "SAFE !", (10, 460), cv.FONT_HERSHEY_COMPLEX, 0.7, self.colorGreen, 2)        


            if self.azimuth < 0 :
                cv.putText(frame, f"POSITION : [LEFT] {abs(self.azimuth)} [deg]", (10, 360), cv.FONT_HERSHEY_COMPLEX, 0.7, self.colorWhite, 2) 
            
            
            elif self.azimuth > 0 :
                cv.putText(frame, f"POSITION : [RIGHT] {abs(self.azimuth)} [deg]", (10, 360), cv.FONT_HERSHEY_COMPLEX, 0.7, self.colorWhite, 2)
            
            
            cv.putText(frame, f"# of detected object : {self.nObject}",(10,310), cv.FONT_HERSHEY_COMPLEX, 0.7, self.colorYellow, 2)
            # drawnow.drawnow(self.plotLidar)
            
            self.startFlag += 1
            
            
            

    def platformControl(self) :
        
        # Velocity & Steer cmd
        sendCommand = erpSerial(self.portNum)
        sendCommand.send_ctrl_cmd(self.velocity, int(self.steerCmd))
        
        if self.isWarn == False :
            
            self.velocity = 6
            
        elif self.isWarn == True :
            
            self.velocity = 0
            
            
    def plotLidar(self) :
        
        plt.plot(self.lidarxList, self.lidaryList, 'ro', markersize = self.markerSize)
        plt.xlabel('x coor [m]', fontsize='large', fontweight='bold')
        plt.ylabel('y coor [m]', fontsize='large', fontweight='bold')
        plt.title('Relative coordinate of lidar', fontsize='x-large', fontweight='bold')
        plt.xlim([-3, 3])
        plt.ylim([0, 6])
        plt.grid(True)
        


    def lidarCamProjection(self, frame) :                                 

        self.projectionX = []
        self.projectionY = []

        for i, dis in enumerate(lidarDist):

            lidarDist[i] = lidarDist[i] / 500

        self.lidarX , self.lidarY = self.polar2xy(lidarDist,azim)                      

        for i in range(len(azim)):

            pixelXY  = 0
            pointcloudXY = 0

            lx = self.lidarX[i]
            ly = self.lidarY[i]
            lz = 0
            Cz = ly + self.realRecede            

            pointcloudXY = np.array([[lx],[ly],[lz],[1]])
            pixelXY = 1/Cz * self.projectionMat @ pointcloudXY

            pixelX = int(pixelXY[0])
            pixelY = int(pixelXY[1])

            self.projectionX.append(pixelX)
            self.projectionY.append(pixelY)

            cv.circle(frame, (round(pixelX) ,round(pixelY)), 3, self.colorWhite)
            
        # cv.imshow("test",frame)




if __name__ == "__main__" :
    
    fourcc   = cv.VideoWriter_fourcc(*'XVID')
    outVideo = cv.VideoWriter('./output/outputVideo2.avi', fourcc, 20.0, (640, 480))        
    
    sim     = Lidar_SharedMem()
    project = PROJECTION()
    
    sim.Lidar_SMopen()
    sim.Yolo_SMopen()
    
    time_start = time.time()

    blackorgImg = np.zeros((480, 640, 3), dtype = np.uint8)

    while (time_stime < time_final):

        blackImg = np.copy(blackorgImg)
        
        # Recieve data
        sim.receiveDist()
        
        project.recieveYolo(blackImg)
            
        project.lidarCamProjection(blackImg)

        # project.platformControl()
        
        outVideo.write(blackImg)
        
        cv.imshow("test",blackImg)
        
        if cv.waitKey(27) & 0xFF == ord('q'):
            
            outVideo.release()
            break
          
        while(1):
            
            time_curr = time.time()
            time_del = time_curr - time_start - time_stime
            
            if (time_del > time_ts) :
                
                time_cnt += 1
                time_stime = time_cnt*time_ts
                
                break
            

    sim.sharedmemory_close()