#pragma once

//*****************************************************************************
// Structure representing a 3d point cloud, with position and intensity data.
//*****************************************************************************
struct SPoint3d
   {
   MIL_FLOAT x;
   MIL_FLOAT y;
   MIL_FLOAT z;
   };

struct SColor
   {
   void ConvertRGBBGR(const SColor& OtherColor)
      {
      x = OtherColor.z;
      y = OtherColor.y;
      z = OtherColor.x;
      }

   MIL_UINT8 x;
   MIL_UINT8 y;
   MIL_UINT8 z;
   MIL_UINT8 a;
   };

struct SPoint3dI
   {
   SPoint3d Point;
   MIL_FLOAT i;
   };


template <class T> bool Init(T* pSensor3d);
template <class T> bool SetupForExample(T* pSensor3d);
template <class T> bool Grab(T* pSensor3d, MIL_ID MilContainer);
template <class T> bool Free(T* pSensor3d);


//*****************************************************************************
// Main.
//*****************************************************************************
template <class T>
int Interface3dCameraExample(T* pSensor3d, MIL_FLOAT InvalidValue)
   {
   // Allocate and initialize MIL application objects.
   MIL_ID MilApplication = MappAlloc(M_NULL, M_DEFAULT, M_NULL);
   MIL_ID MilSystem = MsysAlloc(MilApplication, M_SYSTEM_HOST, M_DEFAULT, M_DEFAULT, M_NULL);

   bool Success = false;

   if(Init(pSensor3d))
      {
      // Setup the camera for the example.
      if(SetupForExample(pSensor3d))
         {
         // Allocate a buffer container.
         MIL_ID MilContainer = MbufAllocContainer(MilSystem, M_PROC+M_DISP, M_DEFAULT, M_NULL);

         // Acquire and display 3d point cloud data.
         MosPrintf(MIL_TEXT("\nThe 3d sensor will be started and triggered to acquire a 3d point cloud.\n\n"));
         MosPrintf(MIL_TEXT("Press <Enter> to continue.\n\n"));
         MosGetch();

         if(!Grab(pSensor3d, MilContainer))
            {
            MosPrintf(MIL_TEXT("Unable to acquire point cloud!\n\n"));
            }
         else
            {
            MbufControlContainer(MilContainer,  M_COMPONENT_RANGE, M_3D_INVALID_DATA_FLAG, M_TRUE);
            
			   // Display the point cloud container.
            DisplayPointCloudData(MilSystem, MilContainer);

            Success = true;
            }

         MbufFree(MilContainer);
         }

      // Free the device.
      Free(pSensor3d);
      }

   if(!Success)
      MosPrintf(MIL_TEXT("\nAn error occured using the 3d camera!\n\n"));

   MosPrintf(MIL_TEXT("Press <Enter> to end.\n\n"));
   MosGetch();


   // Cleanup MIL objects.
   MsysFree(MilSystem      );
   MappFree(MilApplication );
   return 0;
   }

//*****************************************************************************
// Allocates a 3D display and returns its MIL identifier.  
//*****************************************************************************
MIL_ID Alloc3dDisplayId(MIL_ID MilSystem)
   {
   MappControl(M_DEFAULT, M_ERROR, M_PRINT_DISABLE);
   MIL_ID MilDisplay3D = M3ddispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, M_NULL);
   MappControl(M_DEFAULT, M_ERROR, M_PRINT_ENABLE);

   if (!MilDisplay3D)
      {
      MosPrintf(MIL_TEXT("\n")
         MIL_TEXT("The current system does not support the 3D display.\n\n"));
      }
   return MilDisplay3D;
   }

//*****************************************************************************
// allocates a MIL 3D display and displays the depth map.
//*****************************************************************************
void DisplayPointCloudData(MIL_ID MilSystem, MIL_ID MilContainer)
   {
   MosPrintf(MIL_TEXT("\nThe point cloud is displayed.\n\n"));

   MIL_ID M3dDisplay = Alloc3dDisplayId(MilSystem);
   if (M3dDisplay)
      {
      M3ddispSelect(M3dDisplay, MilContainer, M_SELECT, M_DEFAULT);

      M3ddispSetView(M3dDisplay, M_INTEREST_POINT, 0, 0, 1, M_DEFAULT);
      M3ddispSetView(M3dDisplay, M_VIEWPOINT, 0, 0, 0, M_DEFAULT);
      M3ddispSetView(M3dDisplay, M_UP_VECTOR, 0, -1, 0, M_DEFAULT);

      M3ddispControl(M3dDisplay, M_FOV_HORIZONTAL_ANGLE, 60);
      }

   MosPrintf(MIL_TEXT("\nPress <Enter> to continue.\n\n"));
   MosGetch();
   if (M3dDisplay)
      M3ddispFree(M3dDisplay);
   }

