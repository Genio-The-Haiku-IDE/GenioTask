#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Application.h>

class MainApp : public BApplication
{
public:
						MainApp(void);
						~MainApp();
			void		MessageReceived(BMessage *msg);
			void		ReadyToRun(void);
			
private:
};

#endif
