#include <windows.h>
#include <string>
#include <GL\GL.h>

//function pointer typdefs
typedef void (APIENTRY *PFNWGLEXTSWAPCONTROLPROC) (int);
typedef int (*PFNWGLEXTGETSWAPINTERVALPROC) (void);

//declare functions
PFNWGLEXTSWAPCONTROLPROC wglSwapIntervalEXT = NULL;
PFNWGLEXTGETSWAPINTERVALPROC wglGetSwapIntervalEXT = NULL;
 
//init VSync func
void InitVSync()
{
   //get extensions of graphics card
   char* extensions = (char*)glGetString(GL_EXTENSIONS);
  
   //is WGL_EXT_swap_control in the string? VSync switch possible?
   if (strstr(extensions,"WGL_EXT_swap_control"))
   {
	  //get address's of both functions and save them
	  wglSwapIntervalEXT = (PFNWGLEXTSWAPCONTROLPROC)wglGetProcAddress("wglSwapIntervalEXT");
	  wglGetSwapIntervalEXT = (PFNWGLEXTGETSWAPINTERVALPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
  }
}
 
bool VSyncEnabled()
{
   //if interval is positif, it is not 0 so enabled ;)
   return (wglGetSwapIntervalEXT() > 0);
}
 
void SetVSyncState(bool enable)
{
	if (enable)
	   wglSwapIntervalEXT(1); //set interval to 1 -&gt; enable
	else
	   wglSwapIntervalEXT(0); //disable
}
